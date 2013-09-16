#pragma once
#include <iostream>
#include <list>
#include <queue>
#include "ColaFCFS.cpp"
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

  Sistema() {
    cola_ready = new ColaFCFS();
    cola_waiting = new ColaFCFS();
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
    Proceso* ref = &p1;
    Sistema s = Sistema();
    std::cout << "PID desde pointer: " << ref->getPid() << '\n';
    std::cout << "pointer ref: " << ref << '\n';
    (s.ready())->Agregar(ref);
    Proceso* siguiente = s.ready()->Siguiente();
    std::cout << "pointer cola ready: " << s.ready() << '\n';
    std::cout << "pointer siguiente: " << siguiente << '\n';

    std::cout << "PID desde pointer siguiente: "  << siguiente->getPid();
    

}
