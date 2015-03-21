%{
#include <iostream>
#include "sintax.tab.h"
using namespace std;
#define YY_DECL extern "C" int yylex()
int line_num = 1;
%}

%%
[ \t] ;
			  
program		  { return PROG;}
var           { return VAR; }
int           { return INT; }
float 		  { return FLOAT;}
string 		  { return STRING;}
char 		  { return CHAR;}
function 	  { return FUNCTION;}
void 		  { return VOID;}
if            { return IF; }
else		  { return ELSE; }
while		  { return WHILE; }
do			  { return DO; }
case		  { return CASE; }
of			  { return OF; }
print 		  { return PRINT; }
read		  { return READ; }


		
[a-zA-Z][a-zA-Z0-9_]* {yylval.sval = strdup(yytext); return ID; }

[0-9]+         { yylval.ival = atoi(yytext); return CTE_I; }

[0-9]+\.[0-9]+(("E"|"e")("+"|"-")?[0-9]+)? 	{ yylval.fval = atof(yytext); return CTE_F; }

\"[[:alnum:][:digit:][:space:]]*\"   { yylval.sval = strdup(yytext);	return CTE_STRING;}

\'[[:alnum:][:digit:][:space:]]?\'   { yylval.sval = strdup(yytext);	return CTE_CHAR;}



","			  { return ','; }
";"			  { return ';'; }
"{"			  { return '{'; }
"}"			  { return '}'; }
"("			  { return '('; }
")"			  { return ')';}
"="			  { return '='; }
"+"			  { return '+';  }
"-"			  { return '-'; }
"*"			  { return '*'; }
"/"			  { return '/'; }
":"			  { return ':';}
">"			  { return '>';}
"<"			  { return '<';}
"<>"		  { return DIFF;}
\n            { ++line_num; return ENDL;   }
.              ;
%%