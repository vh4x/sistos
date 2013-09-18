#pragma once
#include <algorithm>
#include <iostream>
#include <list>
#include <queue>
#include "ColaFCFS.cpp"
#include "ColaPrioridad.cpp"
#include "ICalendarizador.cpp"

using namespace std;

class Sistema
{
private:
  // Ciclos totales de CPU
  int cpu_total;

  int cpu_parcial;

  int cpu_siguiente;
  
  int io_siguiente;

  Proceso *cpu_actual;
  
  Proceso *io_actual;

  ICalendarizador *cola_ready;

  ICalendarizador *cola_waiting;

  ICalendarizador *cola_terminated;


public:

  Sistema(int algoritmo) {
    cpu_total = 0;
    cpu_parcial = 0;
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
    while (!cola_ready->Vacio() || !cola_waiting->Vacio()) {
      ProcesarSiguiente();
    }
  }

  void ProcesarSiguiente() {
    if (cpu_total >= cpu_siguiente) {
      cpu_actual = cola_ready->Siguiente();
    }

    if (cpu_total >= io_siguiente) {
      io_actual = cola_waiting->Siguiente();
      
    }
  }

};

int main() {
    std::list<int> first;
    Proceso p1 = Proceso(3, 3, 3, first);
    std::cout << p1.getPid() << '\n';
    Proceso p2 = Proceso(4, 4, 4, first);
    std::cout << p2.getPid() << '\n';
    Proceso* ref = &p1;
    Proceso* ref2 = &p2;
    Sistema s = Sistema(1);
    (s.ready())->Agregar(ref);
    (s.ready())->Agregar(ref2);
    Proceso* siguiente = s.ready()->Siguiente();
    Proceso* siguiente2 = s.ready()->Siguiente();
    std::cout << "PID desde pointer siguiente: "  << siguiente->getPid();
    std::cout << "PID desde pointer siguiente: "  << siguiente2->getPid();

    Sistema prioridad = Sistema(2);
    (prioridad.ready())->Agregar(ref2);
    (prioridad.ready())->Agregar(ref);

    std::cout << "PID desde pointer siguiente: "  << prioridad.ready()->Siguiente()->getPid();
    std::cout << "PID desde pointer siguiente: "  << prioridad.ready()->Siguiente()->getPid();


    
}
