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
      std::cout << "retornar pointer: " << retorno << '\n';
      cola.pop();
      return retorno;
    } else {
      return 0;
    }
  }

  virtual void Agregar(Proceso* proceso) {
    std::cout << "agregar pointer: " << proceso << '\n';
    cola.push(proceso);
  }
};
