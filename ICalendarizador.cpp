#pragma once
#include "Proceso.cpp"

class ICalendarizador
{
public:
  virtual Proceso* Siguiente() = 0;
  virtual void Agregar(Proceso* proceso) = 0;
  virtual bool Vacio() = 0;
};
