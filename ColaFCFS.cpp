#pragma once
#include <iostream>
#include "ICalendarizador.cpp"
#include "Proceso.cpp"


using namespace std;

class ColaFCFS : public ICalendarizador {
private:
  std::queue<Proceso*> cola;
  
public: 
  ColaFCFS() { 
  }

  virtual Proceso* Siguiente() {
    if(!cola.empty()) {
      Proceso* retorno = cola.front();     
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
    return cola.front();
  }

  virtual int Size() {
    return cola.size();
  }
};
