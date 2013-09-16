#include <list>

using namespace std;

class Despachador
{
private:
  int pid;
  int prioridad;
  int ciclo;
  std::list<int> bursts;

public:

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
