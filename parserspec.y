%{
#include <cstdio>
#include <iostream>
#include "Proceso.cpp"

using namespace std;

// Definiciones de flex que Bison necesita
extern "C" int yylex();
extern "C" int yyparse();
extern "C" FILE *yyin;
 
  int pid;
  int prioridad;
  int ciclo;
  std::list<int> first;

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
bursts  { Proceso p (pid, prioridad, ciclo, first);
   std::cout << p.getPid();
   std::cout  << p.getBursts().front();
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
    cout << "Response : ";
    cin >> algoritmo;
    cout << "\n";
}


main() {

  
	// open a file handle to a particular file:
	FILE *myfile = fopen("entrada.txt", "r");
	// make sure it is valid:
	if (!myfile) {
		cout << "I can't open a.snazzle.file!" << endl;
		return -1;
	}
	// set flex to read from it instead of defaulting to STDIN:
	yyin = myfile;
	
	// parse through the input until there is no more:
	do {
		yyparse();
	} while (!feof(yyin));
	
}

void yyerror(const char *s) {
	cout << "Error: " << s << endl;
	// might as well halt now:
	exit(-1);
}
