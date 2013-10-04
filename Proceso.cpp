#pragma once
#include <list>
#include <limits>

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
  std::vector<int> bursts_modelo;
  bool round_robin;

public:
  Proceso(int id, int pr, int ci, std::vector<int> bu) {
    if (pid = -1) {
      ciclo = 0;
      prioridad = 0;
    }
    pid = id;
    prioridad = pr;
    ciclo = ci;
    bursts = bu;
    ciclo_actual = -1;
    indice_actual = -1;
    round_robin = false;
    bursts_modelo = bursts;
  }

  Proceso(int id, int pr, int ci, std::vector<int> bu, bool rr, int q_cpu, int q_io) {
    if (pid = -1) {
      ciclo = 0;
      prioridad = 0;
    }
    pid = id;
    prioridad = pr;
    ciclo = ci;
    bursts = bu;
    ciclo_actual = -1;
    indice_actual = -1;
    round_robin = rr;
    bursts_modelo = bursts;
  }

  void incCiclo() {
    ciclo++;
  }

  void incIndice() {
    indice_actual++;
    if ((indice_actual % bursts.size()) == 0) {
      bursts = bursts_modelo;
      ciclo_actual++;
    }
  }

  int getCicloActual(){
    return ciclo_actual;
  }

  int getIndiceActual() {
    return indice_actual % bursts.size();
  }
  
  int getSigIndice() {
    return (indice_actual + 1) % bursts.size();
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

  int getSize() {
    return bursts.size();
  }

  bool ciclosTerminados() {
    if (pid == -1) return true;
    return ciclo_actual >= ciclo;
  }

  std::vector<int> getBursts() {
    return bursts;
  }

  int getBurst() {
    if (pid == -1) return std::numeric_limits<int>::max();
    
    incIndice();
    int b = bursts[getIndiceActual()];
    return b;
  }

  int getBurst(int quantum_cpu, int quantum_io) {
    if (pid == -1) return std::numeric_limits<int>::max();
    // Round robin
    incIndice();
    int b = bursts[getIndiceActual()];;

      if (quantum_cpu < b) {
	bursts[getIndiceActual()] = b - quantum_cpu;
	indice_actual--;
	return quantum_cpu;
      } else {
	incIndice();
	return b;
      }
  }

  int peekBurst() {
    int b = bursts[getIndiceActual()];
    return b;
  }
  
  bool switches(int quantum_cpu, int quantum_io) {
    int b = bursts[getSigIndice()];;
    return (!(quantum_cpu < b));
  }

};
