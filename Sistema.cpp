#pragma once
#include <algorithm>
#include <iostream>
#include <list>
#include <queue>
#include <chrono>
#include <thread>
#include <unistd.h>
#include "ColaFCFS.cpp"
#include "ColaPrioridad.cpp"
#include "ICalendarizador.cpp"

using namespace std;

class Sistema
{
private:
  // Ciclos totales de CPU
  int cpu_total;

  int cpu_siguiente;
  
  int io_siguiente;

  Proceso *cpu_actual;
  
  Proceso *io_actual;
  
  Proceso *null;

  ICalendarizador *cola_ready;

  ICalendarizador *cola_waiting;

  ICalendarizador *cola_terminated;


public:

  Sistema(int algoritmo) {
    cpu_total = 0;
    std::vector<int> v;
    null = new Proceso(-1, -1, -1, v);
    switch (algoritmo) {
      // FCFS
    case 1:
      cola_ready = new ColaFCFS();
      cola_waiting = new ColaFCFS();
      break;

    case 2:
      cola_ready = new ColaPrioridad();
      cola_waiting = new ColaPrioridad();
      break;
    }
  }
 
  ICalendarizador *ready() {
    return cola_ready;
  }

  ICalendarizador *waiting() {
    return cola_waiting;
  }

  ICalendarizador *terminated() {
    return cola_terminated;
  }

  void Simular() {
    cpu_actual = cola_ready->Siguiente();
    io_actual = null;
    cpu_siguiente = cpu_actual->getBurst();
    io_siguiente = cpu_siguiente;
    while ( (!cola_ready->Vacio() || !cola_waiting->Vacio()) || (!cpu_actual->ciclosTerminados() || !io_actual->ciclosTerminados())) {
      ProcesarSiguiente();
    }
  }

  void ProcesarSiguiente() {
    int minimo = min(cpu_siguiente, io_siguiente);
    Proceso *sig_cpu;
    Proceso *sig_io;
   
    if (cpu_siguiente == minimo && io_siguiente == minimo){
      // Cambiar proceso en ejecución
      sig_io = cpu_actual;
      
      // Cambiar proceso en IO
      sig_cpu = io_actual;

      // Actualizar estado
      if (sig_io != null && !sig_io->ciclosTerminados()) {
	cola_waiting->Agregar(sig_io);		
      }

      if (sig_cpu != null && !sig_cpu->ciclosTerminados()) {
	cola_ready->Agregar(sig_cpu);
      }

      if (!cola_ready->Vacio()){
	cpu_actual = cola_ready->Siguiente();
	cpu_siguiente = cpu_actual->getBurst();
      } else {
	cpu_siguiente = 9999;
      }

      if (!cola_waiting->Vacio()) {
	io_actual = cola_waiting->Siguiente();
	io_siguiente = io_actual->getBurst();
      } 
      else {
	io_siguiente = 9999;
      }
      

      actualizarTotal(minimo);
      cout<<(cpu_total);
      return;
    }
    
    if (cpu_siguiente == minimo) {
      cout<<("mincpu");
      if (!cpu_actual->ciclosTerminados()) {
	if (!cola_waiting->Vacio()) {
	  cola_waiting->Agregar(cpu_actual);	
	} else {
	  io_actual = cpu_actual;
	}
      }
      if (!cola_ready->Vacio())
	cpu_actual = cola_ready->Siguiente();
      // Se calcula la diferencia que queda para que IO termine
      io_siguiente = io_siguiente-cpu_siguiente;

      cpu_siguiente = cpu_actual->getBurst();

      actualizarTotal(minimo);           
      cout<<(cpu_total);
      return;
    }

    if (io_siguiente == minimo) {
      cout<<("minio");
      if (!io_actual->ciclosTerminados()) {
	if (!cola_ready->Vacio()) {
	  cola_ready->Agregar(io_actual);
	} else {
	  cpu_actual = io_actual;
	}
      }
      if (!cola_waiting->Vacio())
	io_actual = cola_waiting->Siguiente();      
      // Se calcula la diferencia que queda para que CPU termine
      cpu_siguiente = cpu_siguiente-io_siguiente;
      io_siguiente = io_actual->getBurst();
      actualizarTotal(minimo);
      cout<<(cpu_total);
      return;
    }

  }

  void actualizarTotal(int dif) {
    cpu_total = cpu_total + dif;
    return;
  }

};

int main() {
    std::vector<int> first;
    first.push_back(10);
    first.push_back(200);
    Proceso p1 = Proceso(1, 1, 2, first);

    Proceso p2 = Proceso(2, 1, 3, first);

    Proceso* ref = &p1;
    Proceso* ref2 = &p2;

    // FCFS
    Sistema s = Sistema(1);
    (s.ready())->Agregar(ref);
    (s.ready())->Agregar(ref2);
    s.Simular();

    // Prioridad
    Sistema prioridad = Sistema(2);
    (prioridad.ready())->Agregar(ref2);
    (prioridad.ready())->Agregar(ref);
    
}
