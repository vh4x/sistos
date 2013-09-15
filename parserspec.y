%{
#include <cstdio>
#include <iostream>
using namespace std;

// Definiciones de flex que Bison necesita
extern "C" int yylex();
extern "C" int yyparse();
extern "C" FILE *yyin;
 
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
bursts
;

id:
PID INT      { cout << "pid int: " << $2 << endl; }
;

prioridad:
PRIORIDAD ':' INT  { cout << "prioridad int: " << $3 << endl; }
;

ciclo:
CICLO ':' INT  { cout << "ciclo int: " << $3 << endl; }
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
CPU ':' INT   { cout << "cpu int: " << $3 << endl; }
;

linea_io:
IO ':' INT   { cout << "io int: " << $3 << endl; }
;

%%

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
