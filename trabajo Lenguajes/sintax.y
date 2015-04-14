%{
#include <cstdio>
#include <iostream>
#include <stdbool.h>
#include "Structures/stack.h"
#include "Structures/queue.h"
#include "Structures/Table.h"
#include "Structures/cubo.h"
using namespace std;

extern "C" int yylex();
extern "C" int yyparse();
extern "C" FILE *yyin;
extern int line_num;
void yyerror(const char *s);


//Direcciones Virtuales
//char Global
int CHARG=0;
//string Global
int STRINGG=0;
//int Global 
int INTG=0;
//float Global
int FLOATG=0;
//bool Global
int BOOLG=0;

//char Local
int CHARL=0;
//string Local
int STRINGL=0;
//int Local 
int INTL=0;
//float Global
int FLOATL=0;
//bool Global
int BOOLL=0;

//offset Local
int offLocal=250;

//contador de funciones
int contFunctions=0;
int dirFunctions=500;

//Espacio para datos
int MEMSIZEARRAYDATA=50;
//Variables temporales
//Declarar variables locales
bool localFlag=false;
//Fila para declarar IDs
Queue<string> IDQueue;
string IDstring;

varEntry variables=varEntry();
tableEntry entry=tableEntry();
table pragma= table();



bool saveVars(int tipoVar)
{
	int offset;
	if (localFlag)
		{
			offset=offLocal;
		}
		else
		{
			offset = 0;	
		}

		while(IDQueue.dequeue(IDstring))
		{
			
			variables.setNombreVar(IDstring);
			switch(tipoVar)
			{
				case 0: variables.setDirVirtual(tipoVar*MEMSIZEARRAYDATA + CHARG + offset);
				CHARG++;
				//Añadir a memoria
				//Generar el cuadruplo
				break;
				case 1: variables.setDirVirtual(tipoVar*MEMSIZEARRAYDATA + STRINGG + offset);
				STRINGG++;
				//Añadir a memoria
				break;
				case 2: variables.setDirVirtual(tipoVar*MEMSIZEARRAYDATA + INTG + offset);
				INTG++;
				//Añadir a memoria
				break;
				case 3: variables.setDirVirtual(tipoVar*MEMSIZEARRAYDATA + FLOATG + offset);
				FLOATG++;
				//Añadir a memoria
				break;
				case 4: variables.setDirVirtual(tipoVar*MEMSIZEARRAYDATA+BOOLG + offset);
				BOOLG++;
				//Añadir a memoria
				break;
			}

			cout<<"Variable: "<<IDstring <<" 	Direccion: "<<(tipoVar*MEMSIZEARRAYDATA+INTG)<<endl;
		
		if (localFlag)
				if(!entry.addVarTable(variables))
				{
				cout << "Redeclaration Variable on line: " << line_num << endl;
				return false;
				}	
		else
			if(!pragma.addGlobalVarTable(variables))
			{
				cout << "Redeclaration Variable on line: " << line_num << endl;
				return false;
			}
			
		}

			return true;	
} 



%}

// Bison fundamentally works by asking flex to get the next token, which it
// returns as an object of type "yystype".  But tokens could be of any
// arbitrary data type!  So we deal with that in Bison by defining a C union
// holding each of the types of tokens that Flex could return, and have Bison
// use that union instead of "int" for the definition of "yystype":
%union {
	int ival;
	float fval;
	char *sval;
	char cval;
	bool bval;
}

// define the constant-string tokens:
%token PROG
%token INT
%token FLOAT
%token BOOL
%token IF
%token ELSE
%token VAR
%token PRINT
%token STRING
%token CHAR
%token FUNCTION
%token VOID
%token WHILE
%token DO
%token CASE
%token OF
%token READ

%token TBEGIN
%token TEND
%token TRET

%token <sval> ID
%token DIFF
%token ENDL

%token ROTATEWAIST
%token ROTATESHOULDER
%token ROTATEELBOW
%token ROTATEWRISTM
%token ROTATEWRISTR
%token ROTATETOOL



// define the "terminal symbol" token types I'm going to use (in CAPS
// by convention), and associate each with a field of the union:
%token <ival> CTE_I
%token <fval> CTE_F
%token <sval> CTE_STRING
%token <cval> CTE_CHAR
%token <bval> CTE_BOOL

//Declaracion de tipos de las expresiones
%type <ival> tipo

%start programa


%error-verbose
%%

programa:
	PROG ID	{
		pragma.setNombre($2);
	} 
	';' endl vars functions  {
		localFlag=false;
	}
	coding {  cout << "Fin del analisis del programa " << endl; }
	;

functions:
	FUNCTION ID '(' def_param ')' ':' tipo endl {
		localFlag=true;
		entry.setNombre($2);
		entry.setDirVirtual(dirFunctions+contFunctions);
		contFunctions++;
		} 
		vars bloque_func ';' endl functions 
	| FUNCTION ID '(' def_param ')' ':' VOID endl {
		localFlag=true;
		entry.setNombre($2);
		entry.setDirVirtual(dirFunctions+contFunctions);
		contFunctions++;
		} vars bloque_func2 ';' endl functions 
	|
	;

def_param:
	def_param ',' ID ':' tipo  
	| ID ':' tipo 
	;

vars:
	VAR endl def_vars 
	|
	;

def_vars:
	def_vars def_id ':' tipo ';' endl 
		{
			if (!saveVars($4))
			exit (-1);
		}

	| def_id ':' tipo ';' endl  
		{
			if (!saveVars($3))
			exit (-1);
		}
	;

def_id:
	def_id ',' ID 
	{
		IDQueue.enqueue($3);
		
	}
	|  ID  
	{
		IDQueue.enqueue($1);
			}
	;

tipo:
	INT  {$$=2;}
	| FLOAT {$$=3;}
	| STRING {$$=1;}
	| CHAR {$$=0;}
	| BOOL {$$=4;}
	;

coding:
	TBEGIN endl def_estatuto TEND endl
	;

bloque:
	'{' endl def_estatuto '}' endl 
	;
bloque_func:
	'{' endl def_estatuto TRET expresion endl '}' endl
	;
bloque_func2:
	'{' endl def_estatuto '}' endl
	;

def_estatuto:
	estatuto def_estatuto
	|
	;

estatuto:
	asignacion 
	| condicion 
	| escritura
	| casos
	| ciclo
	| lectura 
	| especial  // added
	;

asignacion:
	ID '=' expresion ';' endl
	;

condicion:
	IF '(' expresion ')' endl bloque def_else
	;

escritura:
	PRINT '(' mensaje ')' ';' endl
	;
casos:
	CASE ID OF endl '{' mas_expr ':' estatuto ';' endl '}' ';' endl
	;

ciclo:
	WHILE '(' expresion ')' DO endl bloque ';' endl
	;

lectura:
	READ '(' ID ')' ';' endl
	;

// added
especial: 
	ROTATEWAIST '(' varcte ')' ';' endl
	| ROTATESHOULDER '(' varcte ')' ';' endl
	| ROTATEELBOW '(' varcte ')' ';' endl
	| ROTATEWRISTM '(' varcte ')' ';' endl
	| ROTATEWRISTR '(' varcte ')' ';' endl
	| ROTATETOOL '(' varcte ')' ';' endl
	;

def_else:
	ELSE endl bloque
	| 
	;

mensaje:
	mensaje ',' expresion  
	| expresion
	;

expresion:
	exp mas_expr
	;

mas_expr:
	'>' exp
	| '<' exp
	| DIFF exp
	| '=' exp
	|
	;

exp:
	termino
	| exp '+' termino
	| exp '-' termino
	;


termino:
	factor
	| termino '*' factor
	| termino '/' factor
	;
factor:
	'(' expresion ')'
	| varcte
	| '+' varcte 
	| '-' varcte
	;

varcte:
	ID
	| CTE_I
	| CTE_F
	| CTE_STRING
	| CTE_CHAR
	| CTE_BOOL
	;

endl:
	ENDL endl
	|
	;
%%

main() {

	// open a file handle to a particular file:
	FILE *myfile = fopen("input.file", "r");
	// make sure it is valid:
	if (!myfile) {
		cout << "No se pudo abrir ningun archivo" << endl;
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
	cout << "Parse error on line " << line_num << "!  Message: " << s << endl;
	// might as well halt now:
	exit(-1);
}
