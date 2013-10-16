#pragma once
#include <algorithm>
#include <iostream>
#include <fstream>
#include <list>
#include <queue>
#include <chrono>
#include <thread>
#include <unistd.h>
#include <limits>
#include "ColaFCFS.cpp"
#include "ColaPrioridad.cpp"
#include "ColaSJF.cpp"
#include "ICalendarizador.cpp"

using namespace std;


class Sistema
{
private:
  // Ciclos totales de CPU
  int cpu_total;

  int cpu_siguiente;
  
  int io_siguiente;

  int cpu_acumulado;

  int io_acumulado;

  int ejecutados;

  Proceso *cpu_actual;
  
  Proceso *io_actual;
  
  Proceso *null;

  ICalendarizador *cola_ready;

  ICalendarizador *cola_waiting;

  ICalendarizador *cola_terminated;
  
  int count;
  
  int quantum_cpu;
  
  int quantum_io;


public:

  Sistema(int algoritmo) {
    cpu_total = 0;
    count = 0;
    cpu_acumulado = 0;
    io_acumulado = 0;
    std::vector<int> v;
    null = new Proceso(-1, -1, -1, v);
    quantum_cpu = 0;
    quantum_io = 0;
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

    case 3:
      cola_ready = new ColaSJF();
      cola_waiting = new ColaSJF();
      break;

    case 4:
      quantum_cpu = 3;
      quantum_io = 3;
      cola_ready = new ColaFCFS();
      cola_waiting = new ColaFCFS();
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
    ejecutados = cola_ready->Size();
    cpu_actual = cola_ready->Siguiente();
    io_actual = null;
    if (quantum_cpu == 0) {
      cpu_siguiente = cpu_actual->getBurst();
    } else {
      cpu_siguiente = cpu_actual->getBurst(quantum_cpu, quantum_io);
    }
    io_siguiente = cpu_siguiente;
    printState();
    std::cout<<"\n";
    while (!(cpu_actual==null && io_actual==null)  ) {
      count++;
      if (quantum_cpu != 0) {
        ProcesarSiguienteRR();
      } else {
        ProcesarSiguiente();
      }
      printState();
      cout<<"\n";
    }
    printResumen();

  }

  void ProcesarSiguienteRR() {
    int minimo = min(cpu_siguiente, io_siguiente);
    Proceso *sig_cpu;
    Proceso *sig_io;
   
    if (cpu_siguiente == minimo && io_siguiente == minimo){
      if (cpu_actual == null && io_actual == null) return;
      // Cambiar proceso en ejecución
      sig_io = cpu_actual;
      
      // Cambiar proceso en IO
      sig_cpu = io_actual;

      // Actualizar estado
      if (sig_io != null && !sig_io->ciclosTerminados()) {
	if (sig_io->switches(quantum_cpu, quantum_io)){
	  cola_waiting->Agregar(sig_io);		    
	} else {
	  cola_ready->Agregar(sig_io);		    
	}  
      }

      if (sig_cpu != null && !sig_cpu->ciclosTerminados()) {
	if (sig_cpu->switches(quantum_cpu, quantum_io)) {
	  cola_ready->Agregar(sig_cpu);
	} else {
	  cola_waiting->Agregar(sig_cpu);
	}
      }

      if (!cola_ready->Vacio()){
	cpu_actual = cola_ready->Siguiente();
	//cpu_siguiente = cpu_actual->getBurst(quantum_cpu, quantum_io);
      } else {
	cpu_actual = null;
      }

      if (!cola_waiting->Vacio()) {
	io_actual = cola_waiting->Siguiente();
	//io_siguiente = io_actual->getBurst(quantum_cpu, quantum_io);
      } 
      else {
	io_actual = null;
      }
      io_siguiente = io_actual->getBurst(quantum_cpu, quantum_io);
      cpu_siguiente = cpu_actual->getBurst();
      actualizarTotal(minimo);
      return;
    }
    
    if (cpu_siguiente == minimo) {
      if (!cpu_actual->ciclosTerminados()) {
	if (!cola_waiting->Vacio() || io_actual != null) {
	  if (cpu_actual->getSize()!= 1) {
	    if (cpu_actual->switches(quantum_cpu, quantum_io)) {
	      cola_waiting->Agregar(cpu_actual);	
	    } else {
	      cola_ready->Agregar(cpu_actual);	
	    }
	    io_siguiente = io_siguiente-cpu_siguiente;
	  } else {
	    asignarCpuACpu(cpu_actual);
	  }
	} else {
	  if (cpu_actual->getSize()!= 1) {
	    if (cpu_actual->switches(quantum_cpu, quantum_io)) {
	      io_actual = cpu_actual;
	    } else {
	      asignarCpuACpu(cpu_actual);
	    }
	    io_siguiente = io_actual->getBurst(quantum_cpu, quantum_io);
	  } else {
	    asignarCpuACpu(cpu_actual);
	  }
	}
      }
      if (!cola_ready->Vacio()) {
	cpu_actual = cola_ready->Siguiente();
      } else {
	cpu_actual = null;
      }

      // Se calcula la diferencia que queda para que IO termine
      cpu_siguiente = cpu_actual->getBurst(quantum_cpu, quantum_io);

      actualizarTotal(minimo);           
      return;
    }

    if (io_siguiente == minimo) {
      std::cout<<"\n";
      if (!io_actual->ciclosTerminados()) {
	if (!cola_ready->Vacio() || cpu_actual != null) {
	  if (io_actual->switches(quantum_cpu, quantum_io)) {
	    cola_ready->Agregar(io_actual);
	  } else {
	    cola_waiting->Agregar(io_actual);
	  }
	  cpu_siguiente = cpu_siguiente-io_siguiente;
	} else {
	  if (io_actual->switches(quantum_cpu, quantum_io)) {
	    cpu_actual = io_actual;
	  } else {
	    //todo
	    asignarIOaIO(io_actual);
	  }
	  cpu_siguiente = cpu_actual->getBurst(quantum_cpu, quantum_io);
	}
      }

      if (!cola_waiting->Vacio()){
	io_actual = cola_waiting->Siguiente();      
      } else {
	io_actual = null;
      }

      // Se calcula la diferencia que queda para que CPU termine
      io_siguiente = io_actual->getBurst(quantum_cpu, quantum_io);
      actualizarTotal(minimo);
      return;
    }
  }

  void ProcesarSiguiente() {

    int minimo = min(cpu_siguiente, io_siguiente);
    Proceso *sig_cpu;
    Proceso *sig_io;
   
    if (cpu_siguiente == minimo && io_siguiente == minimo){
      if (cpu_actual == null && io_actual == null) return;
      // Cambiar proceso en ejecución
      sig_io = cpu_actual;
      
      // Cambiar proceso en IO
      sig_cpu = io_actual;

      // Actualizar estado
      if (sig_io != null && !sig_io->ciclosTerminados()) {
	if (sig_io->getSize() != 1) {
	  cola_waiting->Agregar(sig_io);		
	} else {

	}
      }

      if (sig_cpu != null && !sig_cpu->ciclosTerminados()) {
	cola_ready->Agregar(sig_cpu);
      }

      if (!cola_ready->Vacio()){
	cpu_actual = cola_ready->Siguiente();
	//cpu_siguiente = cpu_actual->getBurst();
      } else {
	cpu_actual = null;
      }

      if (!cola_waiting->Vacio()) {
	io_actual = cola_waiting->Siguiente();
	//io_siguiente = io_actual->getBurst();
      } 
      else {
	io_actual = null;
      }

      io_siguiente = io_actual->getBurst();
      cpu_siguiente = cpu_actual->getBurst();
      actualizarTotal(minimo);
      return;
    }
    
    if (cpu_siguiente == minimo) {
      if (!cpu_actual->ciclosTerminados()) {
	if (!cola_waiting->Vacio() || io_actual != null) {
	  if (cpu_actual->getSize()!= 1) {
	    cola_waiting->Agregar(cpu_actual);	
	    io_siguiente = io_siguiente-cpu_siguiente;
	  } else {
	    //todo
	    asignarCpuACpu(cpu_actual);
	  }
	} else {
	  if (cpu_actual->getSize()!= 1) {
	    io_actual = cpu_actual;
	    io_siguiente = io_actual->getBurst();
	  } else {
	    asignarCpuACpu(cpu_actual);
	  }
	}
      }
      if (!cola_ready->Vacio()) {
	cpu_actual = cola_ready->Siguiente();
      } else {
	cpu_actual = null;
      }

      // Se calcula la diferencia que queda para que IO termine
      cpu_siguiente = cpu_actual->getBurst();

      actualizarTotal(minimo);           
      return;
    }

    if (io_siguiente == minimo) {
      if (!io_actual->ciclosTerminados()) {
	if (!cola_ready->Vacio() || cpu_actual != null) {
	  cola_ready->Agregar(io_actual);
	  cpu_siguiente = cpu_siguiente-io_siguiente;
	} else {
	  cpu_actual = io_actual;
	  cpu_siguiente = cpu_actual->getBurst();
	}
      }

      if (!cola_waiting->Vacio()){
	io_actual = cola_waiting->Siguiente();      
      } else {
	io_actual = null;
      }

      // Se calcula la diferencia que queda para que CPU termine
      io_siguiente = io_actual->getBurst();
      actualizarTotal(minimo);
      return;
    }

  }

  
  void asignarCpuACpu(Proceso *cpu_actual) {
    if (cola_ready->Vacio()) {
      cola_ready->Agregar(cpu_actual);
    } else {
      cpu_actual = cpu_actual;
    }
  }

  void asignarIOaIO(Proceso *io_actual) {
    if (cola_waiting->Vacio()) {
      cola_waiting->Agregar(io_actual);
    } else {
      io_actual = io_actual;
    }
  }

  void actualizarTotal(int dif) {
    if(cpu_actual->ciclosTerminados()) {
      cpu_actual = null;
    }
    if (io_actual->ciclosTerminados()) {
      io_actual = null;
    }
    
    cpu_total = cpu_total + dif;
    return;
  }

  inline void printResumen() {

    cout<<"********************************************\n";
    cout<<ejecutados<<" procesos ejecutados en un total de tiempo de "<<cpu_total<<"\n";
    float pcpu = (float)cpu_acumulado / (float) cpu_total;
    float pio= (float)io_acumulado / (float) cpu_total;
    cout<<"Utilización CPU: "<<pcpu<<"\n";
    cout<<"Utilización IO: "<<pio<<"\n";
    cout<<"Utilización IO: "<<pio<<"\n";
    cout<<"********************************************\n";

  }

  inline void printState() {
    ofstream salida;
    salida.open ("salida.txt", ios::out | ios::app | ios::trunc);
    cout<<"CPU total"<<cpu_total<<"\n";
    cout<<"CPU sig"<<cpu_siguiente<<"\n";
    cout<<"IO sig"<<io_siguiente<<"\n";
    cout<<"Utilización CPU acumulado: "<<cpu_acumulado<<"\n";
    cout<<"Utilización IO acumulado: "<<io_acumulado<<"\n";
    if (!cola_ready->Vacio())cout<<"cola_ready"<<cola_ready->front()<<"\n";
    if (!cola_waiting->Vacio())cout<<"cola_waiting"<<cola_waiting->front()<<"\n";
    if (cpu_actual != null) {
      cpu_acumulado = cpu_acumulado + cpu_siguiente;
      cout<<"CPU actual"<<": ";
      cout<<"Pid: "<<cpu_actual->getPid() <<" ,";
      cout<<"indice: "<<cpu_actual->getIndiceActual() <<" ,";
      cout<<"ciclo: "<<cpu_actual->getCicloActual()<<"\n";
    }
    
    if (io_actual != null) {
      io_acumulado = io_acumulado + io_siguiente;
      cout<<"IO actual"<<": ";
      cout<<"Pid: "<<io_actual->getPid() <<" ,";
      cout<<"indice: "<<io_actual->getIndiceActual() <<" ,";
      cout<<"ciclo: "<<io_actual->getCicloActual()<<"\n";
    }

    salida<<"CPU total"<<cpu_total<<"\n";
    //salida<<"CPU sig"<<cpu_siguiente<<"\n";
    //salida<<"IO sig"<<io_siguiente<<"\n";
    
    if (cpu_actual != null) {
      salida<<"CPU actual"<<": ";
      salida<<"Pid: "<<cpu_actual->getPid() <<" ,";
      //salida<<"indice: "<<cpu_actual->getIndiceActual() <<" ,";
      salida<<"ciclo: "<<cpu_actual->getCicloActual()<<"\n";
    }
    
    if (io_actual != null) {
      salida<<"IO actual"<<": ";
      salida<<"Pid: "<<io_actual->getPid() <<" ,";
      //salida<<"indice: "<<io_actual->getIndiceActual() <<" ,";
      salida<<"ciclo: "<<io_actual->getCicloActual()<<"\n";
    }
    salida<<"\n";
    salida.close();
  }
};


// int main() {
//     std::vector<int> first;
//     first.push_back(4);
//     first.push_back(4);

//     std::vector<int> second;
//     second.push_back(5);
//     Proceso p1 = Proceso(1, 1, 2, first);

//     Proceso p2 = Proceso(2, 3, 1, second);

//     Proceso* ref = &p1;
//     Proceso* ref2 = &p2;
    
//     // FCFS
//     Sistema s = Sistema(1);
//     (s.ready())->Agregar(ref);
//     (s.ready())->Agregar(ref2);
    
//     //
//     // Prioridad
//     Sistema prioridad = Sistema(2);
//     (prioridad.ready())->Agregar(ref2);
//     (prioridad.ready())->Agregar(ref);    
//     prioridad.Simular();
  
//     // Shortest-job-first
//     Sistema sjf = Sistema(3);
//     (sjf.ready())->Agregar(ref2);
//     (sjf.ready())->Agregar(ref);    
    
//     // Round robin
//     Sistema rr = Sistema(4);
//     (rr.ready())->Agregar(ref2);
//     (rr.ready())->Agregar(ref);    
//     rr.Simular();

// }

