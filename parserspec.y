%{
#pragma once
#include <cstdio>
#include <iostream>
#include "Sistema.h"



  //using namespace std;

  // Definiciones de flex que Bison necesita
  extern "C" int yylex();
  extern "C" int yyparse();
  extern "C" FILE *yyin;
 
  int pid;
  int prioridad;
  int ciclo;
  std::vector<int> first;
  std::vector<Proceso*> procesos;
  int algoritmo = 0;

  void yyerror(const char *s);
%}

// Union para los diferentes valores que puedan tener los tokens
%union {
  int ival;
}

// Definición de tokens
%token PID
%token PRIORIDAD
%token CICLO
%token CPU
%token IO

%token <ival> INT

%%
// Gramática

archivo:

  archivo pcb
	| pcb
	;

pcb:
id
prioridad
ciclo
{ first.clear();}
bursts  {   
  procesos.push_back(new Proceso(pid, prioridad, ciclo, first));
}
;

id:
 PID INT      { pid = $2;}
;

prioridad:
PRIORIDAD ':' INT  { prioridad = $3; }
;

ciclo:
CICLO ':' INT  { ciclo = $3; }
;

bursts:
bursts lineas
| lineas
;

lineas:
linea_cpu linea_io;
| linea_cpu
;

linea_cpu:
 CPU ':' INT   { first.push_back($3); }
;

linea_io:
IO ':' INT   { first.push_back($3); }
;

%%

void menu() {
    algoritmo = 0;
    cout << "\n";
    cout << "Algoritmo:\n";
    cout << "1. FCFS\n";
    cout << "2. Shortest job first\n";
    cout << "3. Prioridad\n";
    cout << "4. Round-robin \n";
    cout << "5. Exit \n";
    cin >> algoritmo;
    cout << "\n";
}


int main() {

  menu();

  Sistema sistema = Sistema(algoritmo);
  
  if (algoritmo < 5 && algoritmo > 0) {
      // open a file handle to a particular file:
    FILE *myfile = fopen("entrada.txt", "r");
    // make sure it is valid:
    if (!myfile) {
      cout << "No se puede abrir el archivo" << endl;
      return -1;
    }
    // set flex to read from it instead of defaulting to STDIN:
    yyin = myfile;
    
    // parse through the input until there is no more:
    do {
      yyparse();
    } while (!feof(yyin));

    for (int i = 0; i < procesos.size(); ++i)
    {
      (sistema.ready())->Agregar(procesos.at(i));
    }

    sistema.Simular();

    

  } else {
    cout << "kthxbye\n";
    return 0;
  }
	
}

void yyerror(const char *s) {
	cout << "Error: " << s << endl;
	// might as well halt now:
	exit(-1);
}


