#pragma once
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

  ICalendarizador* cola_ready;

  ICalendarizador* cola_waiting;


public:

  Sistema(int algoritmo) {
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

};

int main() {
    std::list<int> first;
    Proceso p1 = Proceso(3, 3, 3, first);
    std::cout << p1.getPid() << '\n';
    Proceso p2 = Proceso(4, 3, 3, first);
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
    (prioridad.ready())->Agregar(ref);
    (prioridad.ready())->Agregar(ref2);

    siguiente = prioridad.ready()->Siguiente();
    siguiente2 = prioridad.ready()->Siguiente();
    std::cout << "PID desde pointer siguiente: "  << siguiente->getPid();
    std::cout << "PID desde pointer siguiente: "  << siguiente2->>getPid();

    
}
