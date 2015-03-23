%{
#include <iostream>
#include <stdbool.h>
#include "sintax.tab.h"
using namespace std;
#define YY_DECL extern "C" int yylex()
#define TRUE 1
#define FALSE 0
int line_num = 1;
%}

%%
[ \t] ;
			  
program		  { return PROG;}
var               { return VAR; }
int               { return INT; }
float 		  { return FLOAT;}
string 		  { return STRING;}
char 		  { return CHAR;}
bool              { return BOOL;}
function 	  { return FUNCTION;}
void 		  { return VOID;}
if                { return IF; }
else		  { return ELSE; }
while		  { return WHILE; }
do	          { return DO; }
case		  { return CASE; }
of	          { return OF; }
print 		  { return PRINT; }
read		  { return READ; }
true 		  { yylval.bval = true ; return CTE_BOOL;}
false         {yylval.bval = false; return CTE_BOOL;}


		
[a-zA-Z][a-zA-Z0-9_]* {yylval.sval = strdup(yytext); return ID; }

[0-9]+         { yylval.ival = atoi(yytext); return CTE_I; }

[0-9]+\.[0-9]+(("E"|"e")("+"|"-")?[0-9]+)? 	{ yylval.fval = atof(yytext); return CTE_F; }

\"[[:alnum:][:digit:][:space:]]*\"   { yylval.sval = strdup(yytext);	return CTE_STRING;}

'[a-zA-Z0-9]'   { yylval.cval = yytext[1];	return CTE_CHAR;}




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
