#pragma once
#include <iostream>
#include <queue>
#include "ICalendarizador.cpp"
#include "Proceso.cpp"


using namespace std;

class CompararSJF {
    public:
    bool operator()(Proceso *p1, Proceso *p2) {
      if (p1->peekBurst() < p2->peekBurst()) return true;
       return false;
    }
};

class ColaSJF: public ICalendarizador {
private:
  std::priority_queue<Proceso*, vector<Proceso*>, CompararSJF> cola;
  
public: 
  ColaSJF() { 
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
  
  virtual Proceso* front() {
    return cola.top();
  }

};
