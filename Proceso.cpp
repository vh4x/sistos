#include <list>

using namespace std;

class Proceso
{
private:
  int pid;
  int prioridad;
  int ciclo;
  std::list<int> bursts;

public:

  Proceso(int id, int pr, int ci, std::list<int> bu) {
    pid = id;
    prioridad = pr;
    ciclo = ci;
    bursts = bu;
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

};
