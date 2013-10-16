#pragma once
#include <list>
#include <limits>

//using namespace std;
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
  bool switch_flag;

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
    switch_flag = false;
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
    switch_flag = rr;
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
    if (indice_actual == -1) {
      indice_actual = 0;
      ciclo_actual = 0;
    }
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

  void decIndice() {
    indice_actual++;
    if ((indice_actual % bursts.size()) == 0) {
      bursts = bursts_modelo;
      ciclo_actual++;
    }
  }

  inline int getBurst(int quantum_cpu, int quantum_io) {
    if (pid == -1) return std::numeric_limits<int>::max();
    // Round robin
    //indice_actual++;
    int b = bursts[getIndiceActual()];
    if (quantum_cpu < b) {
      std::cout<<"Quantum: "<<"\n";
      bursts[getIndiceActual()] = b - quantum_cpu;
      //indice_actual--;
      switch_flag = false;
      if (bursts.size() == 2)       std::cout<<"bursts quedan: "<< bursts[0] << ", " << bursts[1] << "\n";
      return quantum_cpu;
    } else {
      indice_actual++;
      std::cout<<"Indice cambio: "<<indice_actual<<"\n";
      int indx = (indice_actual) % bursts.size();
      if (indx == 0) {
	std::cout<<"entra: "<<indice_actual<<"\n";	
	ciclo_actual++;
	if (ciclo_actual != -1) {
	  bursts = bursts_modelo;
	} 
      }
      std::cout<<"Burst cambio: "<<b<<"\n";
      switch_flag = true;
      if (bursts.size() == 2)       std::cout<<"bursts quedan: "<< bursts[0] << ", " << bursts[1] << "\n";
      return b;
    }
  }

  int peekBurst() {
    int b = bursts[getIndiceActual()];
    return b;
  }
  
  bool switches(int quantum_cpu, int quantum_io) {
    if (bursts.size() == 1) return false;
    return switch_flag;
  }

};
