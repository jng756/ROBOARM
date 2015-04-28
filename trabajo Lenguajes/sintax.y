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


//Tablas de variables
varEntry variables=varEntry();
tableEntry entry=tableEntry();
table pragma= table();


//Estructura para hacer las definiciones de ID
struct IDs {
	string IDstr;
	int tipoVar;
} tempStr;

Queue<IDs> IDstruct;


//Lista de parametros para las llamadas
Queue<string> lista_params;




//string de parametros
string parametros="";

//Tipo de Funcion
int tipoFunction;

//Tipo para case
int tipoCase;
string optionStr;

//Stacks para expresiones
Stack<string> pilaO;
Stack<int> pTipos;
Stack<string> pilaOper;

//Declaracion de archivo de cuadruplos
ofstream myQuadStructure ("output.file");

//Funciones

//Funcion para guardar Variables
bool guardaVars(int tipoVar);

//Funcion para guardar parametros
bool guardaParametros(string IDstr, int tipoParam);

//Funcion para guardar funciones
void guardaFuncion(int n, string nombre, int tipo, string params);

//Función para convertir enteros a string
string itos(int i);

//Funcion para decodificar el numero del tipo a un string
//(0=char,1=string,2=int, 3=float, 4=bool)
string decodificaTipo(int tipo); 

//Busca el ID de una variable segun sea local o global el alojamiento 
//Regresa el tipo de la variable
int buscaID(string IDstr);

//Busca una función de la tabla de globales
//Regresa el tipo de la funcion leida
int buscaFuncion(string IDstr, string &params);

//Genera cuadruplos para Expresiones
//Regresa el tipo del resultado para una expresion
int generaExpresion();


//Genera cuadruplos para los estatutos especiales
//Regresa el tipo del parametro de la funcion especial (int o float)
int generaEspecial(int special);


bool guardaVars(int tipoVar)
{
	if (localFlag)
		{
		cout<<"*****Variables Locales*****"<<endl;
		while(IDQueue.dequeue(IDstring))
			{
				variables.setNombreVar(IDstring);
				variables.setDirVirtual(tipoVar);

				cout<<"Variable: "<<IDstring <<" 	tipoVar: "<<tipoVar<<endl;
				//Generar Cuadruplo
				myQuadStructure<<"VAR\t"<<IDstring<<"\ttipo\t"<<decodificaTipo(tipoVar)<<endl;
		
			if(!entry.addVarTable(variables))
				{
				
				return false;
				}

			}
		}
		else
		{
			cout<<"*****Variables Globales*****"<<endl;
			while(IDQueue.dequeue(IDstring))
			{
				
				variables.setNombreVar(IDstring);
				 variables.setDirVirtual(tipoVar);
					cout<<"Variable: "<<IDstring <<" 	tipoVar: "<<tipoVar<<endl;
					//Generar el cuadruplo
				myQuadStructure<<"VAR\t"<<IDstring<<"\ttipo\t"<<decodificaTipo(tipoVar)<<endl;		
				
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
				

				cout<<"Parametro : "<<IDstr <<" 	tipoVar: "<<tipoParam<<endl;
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

	int tipoID=-1;
	if (localFlag)
	{
		//Busca en la tabla local, luego busca en la tabla de globales	
		if (!entry.searchVarTable(IDstr,tipoID))
			if (!pragma.searchVarGlobalTable(IDstr,tipoID))
			{
				return -1;
			}
	}
	else 
	{
		//Busca en la tabla de globales solamente
	if (!pragma.searchVarGlobalTable(IDstr,tipoID))
			{
			
				return -1;
			}
	}	
return tipoID;
}

//Busca la funcion
//La busca por el IDstring, devuelve el string  de parametros a traves de params
//Regresa el tipo de la funcion	
int buscaFuncion(string IDstr, string &params) 
{

		int tipoVar=-1;
		//Busca en la tabla la variable
		//Devuelve el tipo y los parametros
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


int generaExpresion()
{
		int tipo1;
		int tipo2;
		int tipoRes;
	 	string operador1="";
	 	string operador2="";
	 	string resultado="";
	 	string tempSimb;
		
	 	if (pilaOper.pop(tempSimb))
	 	{
	 		if (tempSimb=="+")
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
	 			
	 			resultado="t["+itos(contTemp)+"]";

	 			pilaO.push(resultado);

		 		tipoRes=cubo(tipo1,tipo2,0);
		 		if (tipoRes==-1)
		 		{
		 			cout<<"Invalid Operation! line number:"<<line_num<<endl;
		 			return -1;
		 		}

		 		pTipos.push(tipoRes);

	 			cout<<"Resultado: "<<resultado<<"\tTipo:"<< tipoRes<<endl;
	 			myQuadStructure<<"+\t"<<operador1<<"\t"<<operador2<< "\t t["<<contTemp<<"]"<<endl;
	 		}
	 		else if (tempSimb=="-")
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
	 			resultado="t["+itos(contTemp)+"]";
	 			
	 			pilaO.push(resultado);

		 		tipoRes=cubo(tipo1,tipo2,1);
		 		if (tipoRes==-1)
		 			{
		 			cout<<"Invalid Operation! line number:"<<line_num<<endl;
		 			return -1;
		 		}
		 		pTipos.push(tipoRes);

	 			cout<<"Resultado: "<<resultado<<"\tTipo:"<< tipoRes<<endl;
	 			myQuadStructure<<"-\t"<<operador1<<"\t"<<operador2<< "\t t["<<contTemp<<"]"<<endl;


	 		}
	 		else if (tempSimb=="*")
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
	 			resultado="t["+itos(contTemp)+"]";
	
	 			pilaO.push(resultado);

		 		tipoRes=cubo(tipo1,tipo2,2);
		 		if (tipoRes==-1)
		 		{
		 			cout<<"Invalid Operation! line number:"<<line_num<<endl;
		 				return -1;
		 		}
		 		pTipos.push(tipoRes);

	 			cout<<"Resultado: "<<resultado<<"\tTipo:"<< tipoRes<<endl;
	 			myQuadStructure<<"*\t"<<operador1<<"\t"<<operador2<< "\t t["<<contTemp<<"]"<<endl;

	 		}

	 		else if (tempSimb=="/")
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
	 			resultado="t["+itos(contTemp)+"]";

	 	
	 			pilaO.push(resultado);

		 		tipoRes=cubo(tipo1,tipo2,3);
		 		if (tipoRes==-1)
		 		{
		 			cout<<"Invalid Operation! line number:"<<line_num<<endl;
		 				return -1;
		 		}
		 		pTipos.push(tipoRes);

	 			cout<<"Resultado: "<<resultado<<"\tTipo:"<< tipoRes<<endl;
	 			myQuadStructure<<"/\t"<<operador1<<"\t"<<operador2<< "\t t["<<contTemp<<"]"<<endl;

	 		}

	 		if (tempSimb=="<")
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
	 			
	 		
	 			resultado="t["+itos(contTemp)+"]";
	 			pilaO.push(resultado);

		 		tipoRes=cubo(tipo1,tipo2,4);
		 		if (tipoRes!=4)
		 		{
		 			cout<<"Invalid Operation! line number:"<<line_num<<endl;
		 			return -1;
		 		}

		 		pTipos.push(tipoRes);

	 			cout<<"Resultado: "<<resultado<<"\tTipo:"<< tipoRes<<endl;
	 			myQuadStructure<<"<\t"<<operador1<<"\t"<<operador2<< "\t t["<<contTemp<<"]"<<endl;
	 		}


	 		if (tempSimb==">")
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
	 			
	 		
	 			resultado="t["+itos(contTemp)+"]";
	 			pilaO.push(resultado);

		 		tipoRes=cubo(tipo1,tipo2,5);
		 		if (tipoRes!=4)
		 		{
		 			cout<<"Invalid Operation! line number:"<<line_num<<endl;
		 			return -1;
		 		}

		 		pTipos.push(tipoRes);

	 			cout<<"Resultado: "<<resultado<<"\tTipo:"<< tipoRes<<endl;
	 			myQuadStructure<<">\t"<<operador1<<"\t"<<operador2<< "\t t["<<contTemp<<"]"<<endl;
	 		}

	 		if (tempSimb=="<=")
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
	 			
	 		
	 			resultado="t["+itos(contTemp)+"]";
	 			pilaO.push(resultado);

		 		tipoRes=cubo(tipo1,tipo2,6);
		 		if (tipoRes!=4)
		 		{
		 			cout<<"Invalid Operation! line number:"<<line_num<<endl;
		 			return -1;
		 		}

		 		pTipos.push(tipoRes);

	 			cout<<"Resultado: "<<resultado<<"\tTipo:"<< tipoRes<<endl;
	 			myQuadStructure<<"<=\t"<<operador1<<"\t"<<operador2<< "\t t["<<contTemp<<"]"<<endl;
	 		}

	 		if (tempSimb==">=")
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
	 			
	 		
	 			resultado="t["+itos(contTemp)+"]";
	 			pilaO.push(resultado);

		 		tipoRes=cubo(tipo1,tipo2,7);
		 		if (tipoRes!=4)
		 		{
		 			cout<<"Invalid Operation! line number:"<<line_num<<endl;
		 			return -1;
		 		}

		 		pTipos.push(tipoRes);

	 			cout<<"Resultado: "<<resultado<<"\tTipo:"<< tipoRes<<endl;
	 			myQuadStructure<<">=\t"<<operador1<<"\t"<<operador2<< "\t t["<<contTemp<<"]"<<endl;
	 		}

	 		if (tempSimb=="==")
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
	 			
	 		
	 			resultado="t["+itos(contTemp)+"]";
	 			pilaO.push(resultado);

		 		tipoRes=cubo(tipo1,tipo2,8);
		 		if (tipoRes!=4)
		 		{
		 			cout<<"Invalid Operation! line number:"<<line_num<<endl;
		 			return -1;
		 		}

		 		pTipos.push(tipoRes);

	 			cout<<"Resultado: "<<resultado<<"\tTipo:"<< tipoRes<<endl;
	 			myQuadStructure<<"==\t"<<operador1<<"\t"<<operador2<< "\t t["<<contTemp<<"]"<<endl;
	 		}

	 		if (tempSimb=="<>")
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
	 			
	 		
	 			resultado="t["+itos(contTemp)+"]";
	 			pilaO.push(resultado);

		 		tipoRes=cubo(tipo1,tipo2,9);
		 		if (tipoRes!=4)
		 		{
		 			cout<<"Invalid Operation! line number:"<<line_num<<endl;
		 			return -1;
		 		}

		 		pTipos.push(tipoRes);

	 			cout<<"Resultado: "<<resultado<<"\tTipo:"<< tipoRes<<endl;
	 			myQuadStructure<<"<>\t"<<operador1<<"\t"<<operador2<< "\t t["<<contTemp<<"]"<<endl;
	 		}


	 				contTemp++;
	 			return tipoRes;

	 		
	 	}
	 	cout<<"No operand"<<endl;
	 	return -1;

}

int generaEspecial(int special) {
	string operador;
	int tipoAngle;

		if (!pilaO.pop(operador))
		{
			cout<<"No hay operador"<<endl;
			return -1;
		}
		if (!pTipos.pop(tipoAngle))
		{
			cout<<"No hay tipo"<<endl;
			return -1;
		}

		if (tipoAngle!=2 && tipoAngle!=3)
		{
			cout<<"Incompatible Types on special function! on line number:"<<line_num<<endl;
			return -1;
		}

	switch (special) {
	case 0: //Rotate waist
		myQuadStructure<<"RWAIST\t"<<operador<<endl;
		break;
	case 1: //Rotate shoulder
		myQuadStructure<<"RSHOULDER\t"<<operador<<endl;
		break;
	case 2: //Rotate elbow
		myQuadStructure<<"RELBOW\t"<<operador<<endl;
		break;
	case 3: //Rotate wristM
		myQuadStructure<<"RWRISTM\t"<<operador<<endl;
		break;
	case 4: //Rotate wristR
		myQuadStructure<<"RWRISTR\t"<<operador<<endl;
		break;
	case 5: //Rotate Tool
		myQuadStructure<<"RTOOL\t"<<operador<<endl;
		break;
	}

	return tipoAngle;

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

				myQuadStructure<<"FUNCTION \t" <<nombre<<" \t" <<decodificaTipo(tipo)<<endl;



				while (IDstruct.dequeue(tempStr))
				{
		
					myQuadStructure<<"PASS	\t"<<tempStr.IDstr<< "\t"<<decodificaTipo(tempStr.tipoVar)<<endl;
				
				}
				myQuadStructure<<"ENDFUNCTION"<<endl;
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
			
			myQuadStructure<<"FUNCTION \t" <<nombre <<" \t" <<"void"<<endl;
				while (IDstruct.dequeue(tempStr))
				{
					myQuadStructure<<"pass	\t"<<tempStr.IDstr<< "\t"<<decodificaTipo(tempStr.tipoVar)<<endl;
				}
				myQuadStructure<<"ENDFUNCTION"<<endl;

		
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
%token SLEEP

%token TBEGIN
%token TEND
%token TRET

%token <sval> ID
%token DIFF
%token EQUALS
%token LESSTHAN
%token GREATERTHAN


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
%type <ival> constants


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
	FUNCTION ID 

		'(' def_param ')' ':' tipo endl 
		{
			localFlag=true;
			guardaFuncion(1,$2,$7, parametros);
			tipoFunction=$7;
			//Vaciar params
			parametros="";
		}
		vars bloque_func ';' endl

		{	
			//vaciar la estructura de variables para iniciar una nueva funcion
			entry.setVarTable(new HashMap<varEntry>);
			
	
		}

		 functions 
	
	| FUNCTION ID 
	'(' def_param ')' ':' VOID endl {
		localFlag=true;
		guardaFuncion(2,$2,5, parametros);
		tipoFunction=5;
			//Vaciar params
			parametros="";
		}
		 vars bloque_func2 ';' endl 

		{	
			//vaciar la estructura de variables para iniciar una nueva funcion
			entry.setVarTable(new HashMap<varEntry>);
		
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
	TBEGIN {
		myQuadStructure<<"BEGIN"<<endl;
		cout<<"*****Coding Start****"<<endl;
	}
		endl def_estatuto TEND 
		{
			myQuadStructure<<"END"<<endl;
			cout<<"****Coding Finish*****"<<endl;
		}
			endl
	;

bloque:
	'{' endl def_estatuto '}' endl 
	;
bloque_func:
	'{' endl 
	{
		cout<<"****Function  Code****"<<endl;
	}
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
			cout<<"Return not compatible with function! on line:"<<line_num<<endl;
			exit(-1);
		}

		myQuadStructure<<"RETURN\t"<< operador<<endl;

	}

	endl '}' endl
	;
bloque_func2:
	'{' endl 
	{
		cout<<"****Function  Code****"<<endl;
	}
		def_estatuto '}'
	{
		cout<<"Return void"<<endl;
		myQuadStructure<<"RETVOID"<<endl;
	} 
	endl
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
	| sleep 	//added
	| especial  // added
	| llamadas  // added
	;

asignacion:
	ID '=' expresion 
	{
		int tipoID;
		int tipoExp;
		string operador;
		tipoID=buscaID($1);

		if (tipoID == -1)
		{
			cout<<"Variable not defined on line:"<<line_num<<endl;
			exit(-1);
		}

		if (!pTipos.pop(tipoExp))
		{
			cout<<"No hay tipo"<<endl;
		}
		if (!pilaO.pop(operador))
		{
			cout<<"No hay operador"<<endl;
		}
		
		if (tipoID!=tipoExp)
		{
			cout<<"Incompatible Types! line_num:"<<line_num<<endl;
			exit(-1);
		}

		myQuadStructure<<"=\t"<<operador<<"\t"<<$1<<endl;

	}
	';' endl
	;

condicion:
	IF	'(' expresion ')' 
	{
		int tipoBool;
		string operador;
		if (!pTipos.pop(tipoBool))
		{
			cout<<"No hay tipo"<<endl;
		}
		if (tipoBool!=4)
		{
			cout<<"Incompatible types! expected condition line_num"<<line_num<<endl;
		}

		if (!pilaO.pop(operador))
		{
			cout<<"No hay operador"<<endl;
		}


		myQuadStructure<<"IF\t"<<operador<<endl;


	}
	endl bloque def_else
	{
		myQuadStructure<<"ENDIF"<<endl;
	}
	;

escritura:
	PRINT '(' mensaje ')' ';' endl
	;


casos:
	CASE '(' ID ')' OF 
	{
	

		tipoCase=buscaID($3);

		if (tipoCase == -1)
		{
			cout<<"Variable not defined on line:"<<line_num<<endl;
			exit(-1);
		}

		myQuadStructure<<"CASE\t"<<$3<<endl;
	}
	

	endl '{' endl options '}' endl ';' endl
	

	{
	myQuadStructure<<"ENDCASE"<<endl;
	}

	;

options:
options values ':' estatuto
| values ':' estatuto
;

values:
values ',' constants
{
	if (tipoCase!=$3)
	{
		cout<<"Incompatible types! on line: "<<line_num<<endl;
	}
	myQuadStructure<<"EXTRAOPTION\t"<<optionStr<<endl;
}

| constants
{
	if (tipoCase!=$1)
	{
		cout<<"Incompatible types! on line: "<<line_num<<endl;
	}
	myQuadStructure<<"OPTION\t"<<optionStr<<endl;
}

;

constants:
	CTE_I {
		$$=2;
		optionStr=$1;
		}
	| CTE_F {
		$$=3;
		optionStr=$1;
		}
	| CTE_STRING {

		$$=1;
		optionStr=$1;
		
		}
	| CTE_CHAR {
		$$=0;
		optionStr=$1;

	}
	| CTE_BOOL {
		$$=4;
		optionStr=$1;
	}
	;





ciclo:
	WHILE '(' expresion ')'

	{

		int tipoBool;
		string operador;
		if (!pTipos.pop(tipoBool))
		{
			cout<<"No hay tipo"<<endl;
		}
		if (tipoBool!=4)
		{
			cout<<"Incompatible types! expected condition line_num"<<line_num<<endl;
		}

		if (!pilaO.pop(operador))
		{
			cout<<"No hay operador"<<endl;
		}


		myQuadStructure<<"WHILE\t"<<operador<<endl;

	}

	 DO endl bloque ';'
	 {
	 	myQuadStructure<<"ENDWHILE"<<endl;
	 } 
	 endl
	
	;


sleep:
	SLEEP '(' exp ')' ';' endl
	{
		
	string operador;
	int tipoDelay;

		if (!pilaO.pop(operador))
		{
			cout<<"No hay operador"<<endl;
			return -1;
		}
		if (!pTipos.pop(tipoDelay))
		{
			cout<<"No hay tipo"<<endl;
			return -1;
		}

		if (tipoDelay!=2 )
		{
			cout<<"Incompatible Types on special function! on line number:"<<line_num<<endl;
			return -1;
		}


		myQuadStructure<<"SLEEP\t"<<operador<<endl;

	}


lectura:
	READ '(' ID ')' ';' endl

	{
		int tipoID;

		tipoID=buscaID($3);

		if (tipoID == -1)
		{
			cout<<"Variable not defined on line:"<<line_num<<endl;
			exit(-1);
		}


		if (tipoID==4)
		{
			cout<<"Cannot read type Bool!on line:"<<line_num<<endl;
			exit(-1);
		}

		myQuadStructure<<"READ\t"<<$3<<endl;
	}
	;

// added
especial: 
	ROTATEWAIST '(' exp ')' ';' endl
	{
		if (generaEspecial(0)==-1)
			exit(-1);
	}

	| ROTATESHOULDER '(' exp ')' ';' endl
	{
		if (generaEspecial(1)==-1)
			exit(-1);
	}
	| ROTATEELBOW '(' exp ')' ';' endl
	{
		if (generaEspecial(2)==-1)
			exit(-1);
	}
	| ROTATEWRISTM '(' exp ')' ';' endl
	{
		if (generaEspecial(3)==-1)
			exit(-1);
	}
	| ROTATEWRISTR '(' exp ')' ';' endl
	{
		if (generaEspecial(4)==-1)
			exit(-1);
	}
	| ROTATETOOL '(' exp ')' ';' endl
	{
		if (generaEspecial(5)==-1)
			exit(-1);
	}
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
	ELSE 
	{
		myQuadStructure<<"ELSE"<<endl;
	}
	endl bloque 
	{
		myQuadStructure<<"ENDELSE"<<endl;	
	}
	| 
	;

mensaje:
	mensaje ',' expresion 
	{
		int tipoString;
		string operador;
		if (!pTipos.pop(tipoString))
		{
			cout<<"No hay tipo"<<endl;
		}
		// if (tipoString!=1)
		// {
		// 	cout<<"Incompatible types! expected string line_num"<<line_num<<endl;
		// }

		if (!pilaO.pop(operador))
		{
			cout<<"No hay operador"<<endl;
		}
		myQuadStructure<<"PRINT\t"<<operador<<endl;

	} 
	| expresion
	{
		int tipoString;
		string operador;
		if (!pTipos.pop(tipoString))
		{
			cout<<"No hay tipo"<<endl;
		}
		// if (tipoString!=1)
		// {
		// 	cout<<"Incompatible types! expected string line_num"<<line_num<<endl;
		// }

		if (!pilaO.pop(operador))
		{
			cout<<"No hay operador"<<endl;
		}
		myQuadStructure<<"PRINT\t"<<operador<<endl;

	} 

	;

expresion:
	exp mas_expr
	;

mas_expr:
	'>' {
			pilaOper.push(">");
		}
	exp
		{
			if(generaExpresion()==-1)
			exit(-1);
		}
	| '<'
		{
			pilaOper.push("<");
		}
	 exp
	 	{
	 		if(generaExpresion()==-1)
			exit(-1);	
	 	}
	|LESSTHAN
		{
			pilaOper.push("<=");
		}
	 
	 exp
	 	{
	 		if(generaExpresion()==-1)
			exit(-1);	
	 	} 
	|GREATERTHAN
		{
			pilaOper.push(">=");
		}
	 exp
	 	{
	 		if(generaExpresion()==-1)
			exit(-1);	
	 	} 	

	| DIFF
		{
			pilaOper.push("<>");
		}
	 exp
	 	{
	 		if(generaExpresion()==-1)
			exit(-1);	
	 	}
	| EQUALS 
		{
			pilaOper.push("==");
		}
		exp
		{
			if(generaExpresion()==-1)
			exit(-1);	
		}
	|
	;

exp:
	termino 
	| exp '+'
	{
		pilaOper.push("+");
		
	}
	 
	 termino
	 {
		if(generaExpresion()==-1)
			exit(-1);
	 }
	| exp '-' 
	{
		pilaOper.push("-");
	}
	termino
	 {
		if(generaExpresion()==-1)
			exit(-1);
	 }
	
	;


termino:
	factor
	| termino '*' 
	{
		pilaOper.push("*");
	}
	factor
	{
		if(generaExpresion()==-1)
			exit(-1);
	}
	| termino '/' 
	{
	pilaOper.push("/");
	}
	factor
	{
		if(generaExpresion()==-1)
			exit(-1);
	 }
	;
factor:
	'(' expresion ')'
	| varcte
	;

varcte:
	ID '(' params ')'
		{
		int tipo=-1;
		string params="";
		string variable="";
		string acum="";
		tipo=buscaFuncion($1, params);
		cout<<"Funcion "<<$1<<"\tTipo:"<<tipo<<"\tParametros registrados: "<<params<<endl;
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
			parametros="";

			//Para producir el operando
			variable+=$1;
			variable+="(";
			if(lista_params.dequeue(acum))
				variable+=acum;
				
			while(lista_params.dequeue(acum))
			{
			 variable+=",";
			 variable+=acum;
			}
			variable+=")";

		pTipos.push(tipo);
		pilaO.push(variable);
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
		string operando;
		if (!pTipos.pop(tipoExp))
		{
			cout<<"No hay expresion"<<endl;
			exit(-1);
		}
		parametros+=itos(tipoExp);


		if (!pilaO.pop(operando))
		{
			cout<<"No hay operando"<<endl;
			exit(-1);
		}

		lista_params.enqueue(operando);

	}
	| exp
	{
		int tipoExp;
		string operando;
		if (!pTipos.pop(tipoExp))
		{
			cout<<"No hay expresion"<<endl;
			exit(-1);
		}
			parametros+=itos(tipoExp);

		if (!pilaO.pop(operando))
		{
			cout<<"No hay operando"<<endl;
			exit(-1);
		}

		lista_params.enqueue(operando);

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
