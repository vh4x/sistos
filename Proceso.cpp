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
  std::vector<int> bursts;

public:
  Proceso(int id, int pr, int ci, std::vector<int> bu) {
    pid = id;
    prioridad = pr;
    ciclo = ci;
    bursts = bu;
    ciclo_actual = 1;
    indice_actual = -1;
  }

  void incCiclo() {
    ciclo++;
  }

  void incIndice() {
    indice_actual++;
    if ((indice_actual % bursts.size()) == (bursts.size()-1)) ciclo_actual++;
  }

  int getCicloActual(){
    return ciclo_actual;
  }

  int getIndiceActual() {
    return indice_actual % bursts.size();
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

  int getTipoBurst() {
    if (getIndiceActual() % 2 == 0)
      return 1;
    else return 0;
  }

  bool ciclosTerminados() {
    return ciclo_actual > ciclo;
  }

  std::vector<int> getBursts() {
    return bursts;
  }

  int getBurst() {
    incIndice();
    int b = bursts[getIndiceActual()];
    return b;
  }

};
