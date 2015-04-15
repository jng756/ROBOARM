%{
#include <cstdio>
#include <iostream>
#include <stdbool.h>
#include <fstream>
#include <sstream>
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

int contTemp=1;
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

struct IDs {
	string IDstr;
	int tipoVar;
} tempStr;

Queue<IDs> IDstruct;

//Stacks para expresiones
Stack<string> pilaO;
Stack<int> tipo;
Stack<char> pilaOper;

//Declaracion de archivo de cuadruplos
ofstream myQuadStructure ("output.file");



bool saveVars(int tipoVar)
{
	if (localFlag)
		{
		while(IDQueue.dequeue(IDstring))
		{
			
			variables.setNombreVar(IDstring);
			switch(tipoVar)
			{
				case 0: variables.setDirVirtual(tipoVar*MEMSIZEARRAYDATA + CHARL+offLocal);
				CHARL++;
				//Añadir a memoria
				//Generar el cuadruplo
				break;
				case 1: variables.setDirVirtual(tipoVar*MEMSIZEARRAYDATA + STRINGL+offLocal);
				STRINGL++;
				//Añadir a memoria
				break;
				case 2: variables.setDirVirtual(tipoVar*MEMSIZEARRAYDATA + INTL+offLocal);
				INTL++;
				cout<<"Variable: "<<IDstring <<" 	Direccion: "<<tipoVar*MEMSIZEARRAYDATA + INTL+offLocal<<endl;
				//Añadir a memoria
				break;
				case 3: variables.setDirVirtual(tipoVar*MEMSIZEARRAYDATA + FLOATL+offLocal);
				FLOATL++;
				//Añadir a memoria
				break;
				case 4: variables.setDirVirtual(tipoVar*MEMSIZEARRAYDATA+BOOLL+offLocal);
				BOOLL++;
				//Añadir a memoria
				break;
			}
			if(!entry.addVarTable(variables))
				{
				cout << "Redeclaration Variable on line: " << line_num << endl;
				return false;
				}

		}
	}
		else
		{
			while(IDQueue.dequeue(IDstring))
			{
				
				variables.setNombreVar(IDstring);
				switch(tipoVar)
				{
					case 0: variables.setDirVirtual(tipoVar*MEMSIZEARRAYDATA + CHARG);
					CHARG++;
					//Añadir a memoria
					//Generar el cuadruplo
					break;
					case 1: variables.setDirVirtual(tipoVar*MEMSIZEARRAYDATA + STRINGG);
					STRINGG++;
					//Añadir a memoria
					break;
					case 2: variables.setDirVirtual(tipoVar*MEMSIZEARRAYDATA + INTG);
					INTG++;
					cout<<"Variable: "<<IDstring <<" 	Direccion: "<<(tipoVar*MEMSIZEARRAYDATA+INTG)<<endl;
					//Añadir a memoria
					break;
					case 3: variables.setDirVirtual(tipoVar*MEMSIZEARRAYDATA + FLOATG);
					FLOATG++;
					//Añadir a memoria
					break;
					case 4: variables.setDirVirtual(tipoVar*MEMSIZEARRAYDATA+BOOLG);
					BOOLG++;
					//Añadir a memoria
					break;
				}
				if(!pragma.addGlobalVarTable(variables))
			{
				cout << "Redeclaration Variable on line: " << line_num << endl;
				return false;
			}
		}
	}

			return true;	
} 


int buscaID(string IDstr)
{

	int dirVirtual;
	if (localFlag)
	{
		if (!entry.searchVarTable(IDstr,dirVirtual))
			if (!pragma.searchVarGlobalTable(IDstr,dirVirtual))
			{
				cout<<"Variable no definida"<<endl;
				return -1;
			}
	}
	else 
	if (!pragma.searchVarGlobalTable(IDstr,dirVirtual))
			{
				cout<<"Variable no definida"<<endl;
				return -1;
			}
if 	(dirVirtual<MEMSIZEARRAYDATA)
	return 0;
else if (dirVirtual>MEMSIZEARRAYDATA*1-1 && dirVirtual<MEMSIZEARRAYDATA*2)
	return 1;
else if (dirVirtual>MEMSIZEARRAYDATA*2-1 && dirVirtual<MEMSIZEARRAYDATA*3)
	return 2;
else if (dirVirtual>MEMSIZEARRAYDATA*3-1 && dirVirtual<MEMSIZEARRAYDATA*4)
	return 3;
else if (dirVirtual>MEMSIZEARRAYDATA*4-1 && dirVirtual<MEMSIZEARRAYDATA*5)
	return 4;
else if (dirVirtual>MEMSIZEARRAYDATA*4-1 && dirVirtual<MEMSIZEARRAYDATA*5)
	return 5;


	return 1;	
} 

string decodificaTipo(int tipo)
{
	switch(tipo)
	{
		case 0:
		return "char";
		break;
		case 1:
		return "string";
		break;
		case 2:
		return "int";
		break;
		case 3:
		return "float";
		break;
		case 4:
		return "bool";
	}

}


void generaExpresion()
{
	 	string operador1="";
	 	string operador2="";
	 	string resultado="";
	 	char tempSimb=' ';
	 	stringstream ss;
		
	 	 if (pilaOper.pop(tempSimb))
	 	{
	 		if (tempSimb=='+')
	 		{
	 			if (!pilaO.pop(operador2))
	 				operador1="";
	 			if (!pilaO.pop(operador1))
	 				operador2="";
	 			cout<<"simbolo:"<<tempSimb<<endl;
	 			cout<<"operador 1:"<<operador1<<endl;
	 			cout<<"operador 2:"<<operador2<<endl;
	 			ss<<contTemp;
	 			resultado="t"+ss.str();
	 			pilaO.push(resultado);
	 			cout<<"resultado: "<<resultado<<endl;
	 			myQuadStructure<<"+\t"<<operador1<<"\t"<<operador2<< "\t t"<<contTemp<<endl;
	 		}
	 		else if (tempSimb=='-')
	 		{
	 			if (!pilaO.pop(operador2))
	 				operador1="";
	 			if (!pilaO.pop(operador1))
	 				operador2="";
	 			cout<<"simbolo:"<<tempSimb<<endl;
	 			cout<<"operador 1:"<<operador1<<endl;
	 			cout<<"operador 2:"<<operador2<<endl;
	 			ss<<contTemp;
	 			resultado="t"+ss.str();
	 			pilaO.push(resultado);
	 			cout<<"resultado: "<<resultado<<endl;
	 			pilaO.push(resultado);
	 			myQuadStructure<<"-\t"<<operador1<<"\t"<<operador2<< "\t t"<<contTemp<<endl;
	 		}
	 		else if (tempSimb=='*')
	 		{
	 			if (!pilaO.pop(operador2))
	 				operador1="";
	 			if (!pilaO.pop(operador1))
	 				operador2="";
	 			cout<<"simbolo:"<<tempSimb<<endl;
	 			cout<<"operador 1:"<<operador1<<endl;
	 			cout<<"operador 2:"<<operador2<<endl;
	 			ss<<contTemp;
	 			resultado="t"+ss.str();
	 			pilaO.push(resultado);
	 			cout<<"resultado: "<<resultado<<endl;
	 			cout<<"resultado: "<<resultado;
	 			pilaO.push(resultado);
	 			myQuadStructure<<"*\t"<<operador1<<"\t"<<operador2<< "\t t"<<contTemp<<endl;
	 		}

	 		else if (tempSimb=='/')
	 		{
	 			if (!pilaO.pop(operador2))
	 				operador1="";
	 			if (!pilaO.pop(operador1))
	 				operador2="";
	 			cout<<"simbolo:"<<tempSimb<<endl;
	 			cout<<"operador 1:"<<operador1<<endl;
	 			cout<<"operador 2:"<<operador2<<endl;
	 			ss<<contTemp;
	 			resultado="t"+ss.str();
	 			pilaO.push(resultado);
	 			cout<<"resultado: "<<resultado<<endl;
	 			myQuadStructure<<"/\t"<<operador1<<"\t"<<operador2<< "\t t"<<contTemp<<endl;
	 		}

	 		contTemp++;
	 	}

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
%type <ival> varcte
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
	FUNCTION ID '(' def_param ')' ':' tipo endl 
	{
		localFlag=true;
		entry.setNombre($2);
		entry.setDirVirtual(dirFunctions+contFunctions);
		contFunctions++;
			if (!pragma.tableAddEntry(entry))
			{
				cout<<"No se pudo agregar: "<< $2 <<endl;
			}
			else
			{
				cout<<"Funcion: "<< $2<<" Direccion: "<<dirFunctions+contFunctions<<"	Tipo:" <<$7<<endl;
			}

			myQuadStructure<<"function \t" <<$2<<" \t" <<decodificaTipo($7)<<endl;



			while (IDstruct.dequeue(tempStr))
			{
	
				myQuadStructure<<"pass	\t"<<tempStr.IDstr<< "\t"<<decodificaTipo(tempStr.tipoVar)<<endl;
			
			}
			myQuadStructure<<"end Function"<<endl;


	}
		

		vars bloque_func ';' endl

		{entry.setVarTable(new HashMap<varEntry>);}

		 functions 
	
	| FUNCTION ID '(' def_param ')' ':' VOID endl {
		localFlag=true;
		entry.setNombre($2);
		entry.setDirVirtual(dirFunctions+contFunctions);
		contFunctions++;
		if (!pragma.tableAddEntry(entry))
			{
				cout<<"No se pudo agregar: "<< $2 <<endl;
			}
		else
			{
			cout<<"Se agrega la funcion: "<< $2<<" Direccion: "<<dirFunctions+contFunctions<<"	Tipo: void"<<endl;
			}
		}
		 vars bloque_func2 ';' endl 

		{entry.setVarTable(new HashMap<varEntry>);}
		functions 
	|
	;

def_param:
	def_param ',' ID ':' tipo  
		{
		 tempStr.IDstr=$3;
		 tempStr.tipoVar=$5;
		IDstruct.enqueue(tempStr);
		}
	| ID ':' tipo 
	{

		 tempStr.IDstr=$1;
		 tempStr.tipoVar=$3;
		IDstruct.enqueue(tempStr);
	}
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
	| exp '+'
	{
		pilaOper.push('+');
		cout<<"Se encuentra +"<<endl;
	}
	 
	 termino
	 {
		generaExpresion();
	 }
	| exp '-' 
	{
		pilaOper.push('-');
	}
	termino
	 {
		generaExpresion();
	 }
	
	;


termino:
	factor
	| termino '*' 
	{
		pilaOper.push('*');
	}
	factor
	{
		generaExpresion();
	}
	| termino '/' 
	{
	pilaOper.push('/');
	}
	factor
	{
	generaExpresion();
	 }
	;
factor:
	'(' expresion ')'
	| varcte
	| '+' varcte 
	| '-' varcte
	;

varcte:
	ID {$$=buscaID($1);	
		pilaO.push($1);
	}
	| CTE_I {$$=2;}
	| CTE_F {$$=3;}
	| CTE_STRING {$$=1;}
	| CTE_CHAR {$$=0;}
	| CTE_BOOL {$$=4;}
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

	myQuadStructure.close();

}

void yyerror(const char *s) {
	cout << "Parse error on line " << line_num << "!  Message: " << s << endl;
	// might as well halt now:
	exit(-1);
}
