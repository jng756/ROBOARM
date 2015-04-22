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

//Contador de temporales
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


//string de parametros
string parametros="";

//Tipo de Funcion
int tipoFunction;

//Stacks para expresiones
Stack<string> pilaO;
Stack<int> pTipos;
Stack<char> pilaOper;

//Declaracion de archivo de cuadruplos
ofstream myQuadStructure ("output.file");

//Funciones
bool guardaVars(int tipoVar);
bool guardaParametros(string IDstr, int tipoParam);
void guardaFuncion(int n, string nombre, int tipo, string params);
string itos(int i);// convert int to string
string decodificaTipo(int tipo); //tipo to string (0=char,1=string,2=int, 3=float, 4=bool)
int buscaID(string IDstr);
int buscaFuncion(string IDstr, string &params);
void generaExpresion();

bool guardaVars(int tipoVar)
{
	if (localFlag)
		{
		while(IDQueue.dequeue(IDstring))
			{
				variables.setNombreVar(IDstring);
				variables.setDirVirtual(tipoVar);

				cout<<"Variable: "<<IDstring <<" 	tipoVar: "<<tipoVar<<endl;
				//Generar Cuadruplo
			
			if(!entry.addVarTable(variables))
				{
				
				return false;
				}

			}
		}
		else
		{
			cout<<"Variables Globales"<<endl;
			while(IDQueue.dequeue(IDstring))
			{
				
				variables.setNombreVar(IDstring);
				 variables.setDirVirtual(tipoVar);
					cout<<"Variable: "<<IDstring <<" 	tipoVar: "<<tipoVar<<endl;
					//Generar el cuadruplo
					
				
					if(!pragma.addGlobalVarTable(variables))
				{
					return false;
				}
			}
		}

			return true;	
} 

bool guardaParametros(string IDstr, int tipoParam)
{
	//Registrar los parametros de la funcion
		parametros+=itos(tipoParam);

		//Guardar los parametros como variables de la funcion 
		variables.setNombreVar(IDstr);
		variables.setDirVirtual(tipoParam);
				cout<<"Parametro (Variable): "<<IDstr <<" 	tipoVar: "<<tipoParam<<endl;
			
			if(!entry.addVarTable(variables))
				{
					cout<<"Redeclaracion de parametro"<<endl;
					return false;
				}

		//Guardar la lista de parametros en una lista para que al terminar la funcion generar los cuadruplos
		tempStr.IDstr=IDstr;
		tempStr.tipoVar=tipoParam;
		IDstruct.enqueue(tempStr);

		return true;
}



int buscaID(string IDstr)
{

	int dirVirtual=-1;
	if (localFlag)
	{
		if (!entry.searchVarTable(IDstr,dirVirtual))
			if (!pragma.searchVarGlobalTable(IDstr,dirVirtual))
			{
				return -1;
			}
	}
	else 
	{

	if (!pragma.searchVarGlobalTable(IDstr,dirVirtual))
			{
			
				return -1;
			}
	}	
return dirVirtual;
}

int buscaFuncion(string IDstr, string &params) 
{
		int tipoVar=-1;
	if (!pragma.tableSearchEntry(IDstr,tipoVar, params))
			{
				return -1;
			}
	return tipoVar;
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
		int tipo1;
		int tipo2;
		int tipoRes;
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

	 			if (!pTipos.pop(tipo2))
	 				tipo2=-1;
	 			if (!pTipos.pop(tipo1))
	 				tipo1=-1;

				cout<<"simbolo:"<<tempSimb<<endl;
	 			cout<<"operador 1:"<<operador1<<endl;
	 			cout<<"operador 2:"<<operador2<<endl;
	 			ss<<contTemp;
	 			resultado="t"+ss.str();
	 			pilaO.push(resultado);

		 		tipoRes=cubo(tipo1,tipo2,0);
		 		if (tipoRes==-1)
		 			cout<<"operacion Invalida"<<endl;

		 		pTipos.push(tipoRes);

	 			cout<<"Resultado: "<<resultado<<"\tTipo:"<< tipoRes<<endl;
	 			myQuadStructure<<"+\t"<<operador1<<"\t"<<operador2<< "\t t"<<contTemp<<endl;
	 		}
	 		else if (tempSimb=='-')
	 		{
	 			if (!pilaO.pop(operador2))
	 				operador1="";
	 			if (!pilaO.pop(operador1))
	 				operador2="";

	 			if (!pTipos.pop(tipo2))
	 				tipo2=-1;
	 			if (!pTipos.pop(tipo1))
	 				tipo1=-1;

				cout<<"simbolo:"<<tempSimb<<endl;
	 			cout<<"operador 1:"<<operador1<<endl;
	 			cout<<"operador 2:"<<operador2<<endl;
	 			ss<<contTemp;
	 			resultado="t"+ss.str();
	 			pilaO.push(resultado);

		 		tipoRes=cubo(tipo1,tipo2,1);
		 		if (tipoRes==-1)
		 			cout<<"operacion Invalida"<<endl;

		 		pTipos.push(tipoRes);

	 			cout<<"Resultado: "<<resultado<<"\tTipo:"<< tipoRes<<endl;
	 			myQuadStructure<<"-\t"<<operador1<<"\t"<<operador2<< "\t t"<<contTemp<<endl;
	 		}
	 		else if (tempSimb=='*')
	 		{
	 			if (!pilaO.pop(operador2))
	 				operador1="";
	 			if (!pilaO.pop(operador1))
	 				operador2="";

	 			if (!pTipos.pop(tipo2))
	 				tipo2=-1;
	 			if (!pTipos.pop(tipo1))
	 				tipo1=-1;

				cout<<"simbolo:"<<tempSimb<<endl;
	 			cout<<"operador 1:"<<operador1<<endl;
	 			cout<<"operador 2:"<<operador2<<endl;
	 			ss<<contTemp;
	 			resultado="t"+ss.str();
	 			pilaO.push(resultado);

		 		tipoRes=cubo(tipo1,tipo2,2);
		 		if (tipoRes==-1)
		 			cout<<"operacion Invalida"<<endl;

		 		pTipos.push(tipoRes);

	 			cout<<"Resultado: "<<resultado<<"\tTipo:"<< tipoRes<<endl;
	 			myQuadStructure<<"*\t"<<operador1<<"\t"<<operador2<< "\t t"<<contTemp<<endl;
	 		}

	 		else if (tempSimb=='/')
	 		{
	 			if (!pilaO.pop(operador2))
	 				operador1="";
	 			if (!pilaO.pop(operador1))
	 				operador2="";

	 			if (!pTipos.pop(tipo2))
	 				tipo2=-1;
	 			if (!pTipos.pop(tipo1))
	 				tipo1=-1;

				cout<<"simbolo:"<<tempSimb<<endl;
	 			cout<<"operador 1:"<<operador1<<endl;
	 			cout<<"operador 2:"<<operador2<<endl;
	 			ss<<contTemp;
	 			resultado="t"+ss.str();
	 			pilaO.push(resultado);

		 		tipoRes=cubo(tipo1,tipo2,3);
		 		if (tipoRes==-1)
		 			cout<<"operacion Invalida"<<endl;

		 		pTipos.push(tipoRes);

	 			cout<<"Resultado: "<<resultado<<"\tTipo:"<< tipoRes<<endl;
	 			myQuadStructure<<"/\t"<<operador1<<"\t"<<operador2<< "\t t"<<contTemp<<endl;
	 		}

	 		contTemp++;
	 	}

}


string itos(int i) // convert int to string
{
    stringstream s;
    s << i;
    return s.str();
}

void guardaFuncion(int n, string nombre, int tipo, string params)
{

	if (n==1)
	{
			entry.setNombre(nombre);
			entry.setDirVirtual(tipo);
			entry.setParams(params);

				if (!pragma.tableAddEntry(entry))
				{
					cout<<"No se pudo agregar: "<< nombre <<endl;
				}
				else
				{
					cout<<"Funcion: "<< nombre<<"\t Tipo:" <<tipo<<endl;
				}

				myQuadStructure<<"Funcion: \t" <<nombre<<" \t" <<decodificaTipo(tipo)<<endl;



				while (IDstruct.dequeue(tempStr))
				{
		
					myQuadStructure<<"pass	\t"<<tempStr.IDstr<< "\t"<<decodificaTipo(tempStr.tipoVar)<<endl;
				
				}
				myQuadStructure<<"end Function"<<endl;
	}
	else if (n==2)
	{

			entry.setNombre(nombre);
			entry.setDirVirtual(tipo); 
			entry.setParams(params);
			if (!pragma.tableAddEntry(entry))
				{
					cout<<"No se pudo agregar: "<< nombre <<endl;
				}
			else
				{
				cout<<"Funcion: "<< nombre<<" Tipo: void"<<endl;
				}
			
			myQuadStructure<<"Funcion \t" <<nombre <<" \t" <<"void"<<endl;
				while (IDstruct.dequeue(tempStr))
				{
					myQuadStructure<<"pass	\t"<<tempStr.IDstr<< "\t"<<decodificaTipo(tempStr.tipoVar)<<endl;
				}
				myQuadStructure<<"end Function"<<endl;

		
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
%token <sval> CTE_I
%token <sval> CTE_F
%token <sval> CTE_STRING
%token <sval> CTE_CHAR
%token <sval> CTE_BOOL

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

	coding 
	{  cout << "Fin del analisis del programa " << endl; }
	;

functions:
	FUNCTION ID '(' def_param ')' ':' tipo endl 
		{
			localFlag=true;
			guardaFuncion(1,$2,$7, parametros);
			tipoFunction=$7;
		}
		vars bloque_func ';' endl

		{	
			//vaciar la estructura de variables para iniciar una nueva funcion
			entry.setVarTable(new HashMap<varEntry>);
			//Vaciar params
			parametros="";
		}

		 functions 
	
	| FUNCTION ID '(' def_param ')' ':' VOID endl {
		localFlag=true;
		guardaFuncion(2,$2,5, parametros);
		tipoFunction=5;
		}
		 vars bloque_func2 ';' endl 

		{	
			//vaciar la estructura de variables para iniciar una nueva funcion
			entry.setVarTable(new HashMap<varEntry>);
			//Vaciar params
			parametros="";
		}
		functions 
	|
	;


def_param:
	def_param ',' ID ':' tipo  
		{

		if (!guardaParametros($3,$5))
			{
				exit(-1);
			}
		
		
		}
	| ID ':' tipo 
	{
		if (!guardaParametros($1,$3))
			{
				exit(-1);

			}
	}
	;

vars:
	VAR endl def_vars 
	|
	;

def_vars:
	def_vars def_id ':' tipo ';' endl 
		{

			if (!guardaVars($4))
			{
				cout << "Redeclaration Variable on line: " << line_num << endl;
			exit (-1);
			}
		}

	| def_id ':' tipo ';' endl  
		{
			if (!guardaVars($3))
			{
				cout << "Redeclaration Variable on line: " << line_num << endl;
			exit (-1);
			}
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
	'{' endl 
	def_estatuto
	TRET 
	expresion 
	{
		int tipof;
		string operador;
		if (!pTipos.pop(tipof))
		{
			cout<<"No hay tipo"<<endl;
		}
		if (!pilaO.pop(operador))
		{
			cout<<"No hay operador"<<endl;
		}

		if (tipoFunction!=tipof)
		{
			cout<<"Expresion no compatible"<<endl;
			exit(-1);
		}

	}

	endl '}' endl
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
	| llamadas  // added
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

llamadas:  
	//Solo las funciones de tipo 5
	ID '(' params ')' ';' endl
	{
		{
			int tipo=-1;
			string params="";
			tipo=buscaFuncion($1, params);
			cout<<"Funcion: "<<$1<<"\tTipo:"<<tipo<<"\tParametros registrados: "<<params<<endl;
				if (tipo == -1)
				{
					cout<<"Function not defined on line:"<<line_num<<endl;
					exit(-1);
				}
				if (tipo != 5)
				{
					cout<<"Function not void:"<<line_num<<endl;
					exit(-1);
				} 

				cout<<"Parametros Leidos:"<<parametros<< endl;		
				if (parametros!=params)
				{
					cout<<"Parametros no coinciden"<<endl;
					exit(-1);
				}
		}
	}

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
	ID '(' params ')'
		{
		int tipo=-1;
		string params="";
		tipo=buscaFuncion($1, params);
		cout<<"Funcion: "<<$1<<"\tTipo:"<<tipo<<"\tParametros registrados: "<<params<<endl;
			if (tipo == -1)
			{
				cout<<"Function not defined on line:"<<line_num<<endl;
				exit(-1);
			}
			if (tipo == 5)
			{
				cout<<"Function void declared as an asignation:"<<line_num<<endl;
				exit(-1);
			} 

			cout<<"Parametros Leidos:"<<parametros<< endl;		
			if (parametros!=params)
			{
				cout<<"Parametros no coinciden"<<endl;
				exit(-1);
			}

		pTipos.push(tipo);
		pilaO.push($1);
		}

	|ID 
	{
		int tipo;
		tipo=buscaID($1);
		cout<<"Variable read: "<<$1<<"\tTipo:"<<tipo<<endl;
		if (tipo == -1)
		{
			cout<<"Variable not defined on line:"<<line_num<<endl;
			exit(-1);
		}
		pTipos.push(tipo);
		pilaO.push($1);
	}
	| CTE_I {

		pilaO.push($1);
		pTipos.push(2);
		}
	| CTE_F {

		pilaO.push($1);
		pTipos.push(3);

		}
	| CTE_STRING {

		pilaO.push($1);
		pTipos.push(1);
		
		}
	| CTE_CHAR {
		pilaO.push($1);
		pTipos.push(0);

	}
	| CTE_BOOL {
		pilaO.push($1);
		pTipos.push(4);
	}
	;

	params:
	params ',' exp
	{	int tipoExp;
		if (!pTipos.pop(tipoExp))
		{
			cout<<"No hay expresion"<<endl;
			exit(-1);
		}

			parametros+=itos(tipoExp);
	}
	| exp
	{
		int tipoExp;
		if (!pTipos.pop(tipoExp))
		{
			cout<<"No hay expresion"<<endl;
			exit(-1);
		}

			parametros+=itos(tipoExp);

	}
	|
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
