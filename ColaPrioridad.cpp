#pragma once
#include <iostream>
#include <queue>
#include "ICalendarizador.cpp"
#include "Proceso.cpp"


using namespace std;

class CompararPrioridad {
    public:
    bool operator()(Proceso *p1, Proceso *p2) {
      if (p1->getPrioridad() < p2->getPrioridad()) return true;
       return false;
    }
};

class ColaPrioridad : public ICalendarizador {
private:
  std::priority_queue<Proceso*, vector<Proceso*>, CompararPrioridad> cola;
  
public: 
  ColaPrioridad() { 
  }

  virtual Proceso* Siguiente() {
    if(!cola.empty()) {
      Proceso* retorno = cola.top();     
      cola.pop();
      return retorno;
    } else {
      return 0;
    }
  }

  virtual void Agregar(Proceso* proceso) {
    cola.push(proceso);
  }

  virtual bool Vacio() {
    return cola.empty();
  }
};
