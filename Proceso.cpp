#pragma once
#include <list>

using namespace std;

class Proceso
{
private:
  int pid;
  int prioridad;
  int ciclo;
  int ciclo_actual;
  int indice_actual;
  std::list<int> bursts;

public:
  Proceso(int id, int pr, int ci, std::list<int> bu) {
    pid = id;
    prioridad = pr;
    ciclo = ci;
    bursts = bu;
    ciclo_actual = 0;
    indice_actual = 0;
  }

  void incCiclo() {
    ciclo++;
  }

  void incIndice() {
    indice_actual++;
  }

  int getCiclo_actual(){
    return ciclo_actual;
  }

  int getIndice_actual() {
    return indice_actual % 3;
  }

  int getPid() {
    return pid;
  }

  int getPrioridad() {
    return prioridad;
  }

  int getCiclo() {
    return ciclo;
  }

  std::list<int> getBursts() {
    return bursts;
  }

  int getBurst(int indice) {
    return 1;
  }

};
