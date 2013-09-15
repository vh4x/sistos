%{
#include <iostream>
using namespace std;
#include "parserspec.tab.h"  // to get the token types that we return
#define YY_DECL extern "C" int yylex()


%}
%%
[ \t\n]           ;
PID               {return PID; }
PRIORIDAD     {return PRIORIDAD; }
CPU              {return CPU; }
CICLO            {return CICLO; }
IO                 {return IO; }
[0-9]+            { yylval.ival = atoi(yytext); return INT; }

\:                {  return ':'; }
%%

