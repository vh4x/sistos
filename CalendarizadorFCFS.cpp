#include "ICalendarizador"
#include "Proceso"

using namespace std;

class CalendarizadorFCFS : public ICalendarizador {
private:
  std::queue<Proceso> cola;
  
public: 
  CalendarizadorFCFS() {
    cola 
  }
  Proceso Siguiente() {
    return null;
  }
};
