/* A Bison parser, made by GNU Bison 2.5.  */

/* Bison implementation for Yacc-like parsers in C
   
      Copyright (C) 1984, 1989-1990, 2000-2011 Free Software Foundation, Inc.
   
   This program is free software: you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation, either version 3 of the License, or
   (at your option) any later version.
   
   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.
   
   You should have received a copy of the GNU General Public License
   along with this program.  If not, see <http://www.gnu.org/licenses/>.  */

/* As a special exception, you may create a larger work that contains
   part or all of the Bison parser skeleton and distribute that work
   under terms of your choice, so long as that work isn't itself a
   parser generator using the skeleton or a modified version thereof
   as a parser skeleton.  Alternatively, if you modify or redistribute
   the parser skeleton itself, you may (at your option) remove this
   special exception, which will cause the skeleton and the resulting
   Bison output files to be licensed under the GNU General Public
   License without this special exception.
   
   This special exception was added by the Free Software Foundation in
   version 2.2 of Bison.  */

/* C LALR(1) parser skeleton written by Richard Stallman, by
   simplifying the original so-called "semantic" parser.  */

/* All symbols defined below should begin with yy or YY, to avoid
   infringing on user name space.  This should be done even for local
   variables, as they might otherwise be expanded by user macros.
   There are some unavoidable exceptions within include files to
   define necessary library symbols; they are noted "INFRINGES ON
   USER NAME SPACE" below.  */

/* Identify Bison output.  */
#define YYBISON 1

/* Bison version.  */
#define YYBISON_VERSION "2.5"

/* Skeleton name.  */
#define YYSKELETON_NAME "yacc.c"

/* Pure parsers.  */
#define YYPURE 0

/* Push parsers.  */
#define YYPUSH 0

/* Pull parsers.  */
#define YYPULL 1

/* Using locations.  */
#define YYLSP_NEEDED 0



/* Copy the first part of user declarations.  */

/* Line 268 of yacc.c  */
#line 1 "sintax.y"

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

//Funcion para guardar Variables en la tabla de variables
//Recibe tipoVar: tipo de variable a guardar
//Regresa false si no se puede guardar la variable 
bool guardaVars(int tipoVar);

//Funcion para registrar los parametros
//Recibe tipoVar: tipo de variable a guardar
//Regresa false si no se puede guardar la variable 
bool guardaParametros(string IDstr, int tipoParam);

//Funcion para guardar funciones
//Recibe n: (1: Guarda las funciones de tipos, 2: Guarda la función void
//Recibe nombre: Nombre de la función a guardar
//Recibe tipo: tipo de función a guardar
//Recibe params: string de parametros registrados
//Regresa false si no se puede guardar la función  
int guardaFuncion(int n, string nombre, int tipo, string params);

//Función para convertir enteros a string
string itos(int i);

//Funcion para decodificar el numero del tipo a un string
//(0=char,1=string,2=int, 3=float, 4=bool)
//Recibe el tipo y devuelve el string de tipo
string decodificaTipo(int tipo); 

//Busca el ID de una variable segun sea local o global el alojamiento 
//Recibe un string para buscarlo en la tabla de variables
//Regresa el tipo de la variable; en caso de encontrar errores regresa -1
int buscaID(string IDstr);

//Busca una función en la tabla de globales
//Regresa el tipo de la funcion leida
//Regresa -1 si falla la función

int buscaFuncion(string IDstr, string &params);

//Genera cuadruplos para Expresiones
//Regresa el tipo del resultado para una expresion
int generaExpresion();


//Genera cuadruplos para los estatutos especiales
//Regresa el tipo del parametro de la funcion especial (int o float)
int generaEspecial(int special);

//Genera cuadruplos para el estatuto de retorno 

int generaRET();



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

int generaRET()
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
			return -1;
		}

		myQuadStructure<<"RETURN\t"<< operador<<endl;
}




string itos(int i) // convert int to string
{
    stringstream s;
    s << i;
    return s.str();
}

int guardaFuncion(int n, string nombre, int tipo, string params)
{

	if (n==1)
	{
			entry.setNombre(nombre);
			entry.setDirVirtual(tipo);
			entry.setParams(params);

				if (!pragma.tableAddEntry(entry))
				{
					cout<<"Function previously declared on line:: "<<line_num<<"\t"<<nombre <<endl;
					return -1;
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

				return tipo;
	}
	else if (n==2)
	{

			entry.setNombre(nombre);
			entry.setDirVirtual(tipo); 
			entry.setParams(params);
			if (!pragma.tableAddEntry(entry))
				{
					cout<<"Function previously declared on line: "<<line_num<<"\t"<<nombre <<endl;
					return -1;
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
				return tipo;

		
	}



}




/* Line 268 of yacc.c  */
#line 827 "sintax.tab.c"

/* Enabling traces.  */
#ifndef YYDEBUG
# define YYDEBUG 0
#endif

/* Enabling verbose error messages.  */
#ifdef YYERROR_VERBOSE
# undef YYERROR_VERBOSE
# define YYERROR_VERBOSE 1
#else
# define YYERROR_VERBOSE 1
#endif

/* Enabling the token table.  */
#ifndef YYTOKEN_TABLE
# define YYTOKEN_TABLE 0
#endif


/* Tokens.  */
#ifndef YYTOKENTYPE
# define YYTOKENTYPE
   /* Put the tokens into the symbol table, so that GDB and other debuggers
      know about them.  */
   enum yytokentype {
     PROG = 258,
     INT = 259,
     FLOAT = 260,
     BOOL = 261,
     IF = 262,
     ELSE = 263,
     VAR = 264,
     PRINT = 265,
     STRING = 266,
     CHAR = 267,
     FUNCTION = 268,
     VOID = 269,
     WHILE = 270,
     DO = 271,
     CASE = 272,
     OF = 273,
     READ = 274,
     SLEEP = 275,
     TBEGIN = 276,
     TEND = 277,
     TRET = 278,
     ID = 279,
     DIFF = 280,
     EQUALS = 281,
     LESSTHAN = 282,
     GREATERTHAN = 283,
     ENDL = 284,
     ROTATEWAIST = 285,
     ROTATESHOULDER = 286,
     ROTATEELBOW = 287,
     ROTATEWRISTM = 288,
     ROTATEWRISTR = 289,
     ROTATETOOL = 290,
     CTE_I = 291,
     CTE_F = 292,
     CTE_STRING = 293,
     CTE_CHAR = 294,
     CTE_BOOL = 295
   };
#endif



#if ! defined YYSTYPE && ! defined YYSTYPE_IS_DECLARED
typedef union YYSTYPE
{

/* Line 293 of yacc.c  */
#line 761 "sintax.y"

	int ival;
	float fval;
	char *sval;
	char cval;
	bool bval;



/* Line 293 of yacc.c  */
#line 913 "sintax.tab.c"
} YYSTYPE;
# define YYSTYPE_IS_TRIVIAL 1
# define yystype YYSTYPE /* obsolescent; will be withdrawn */
# define YYSTYPE_IS_DECLARED 1
#endif


/* Copy the second part of user declarations.  */


/* Line 343 of yacc.c  */
#line 925 "sintax.tab.c"

#ifdef short
# undef short
#endif

#ifdef YYTYPE_UINT8
typedef YYTYPE_UINT8 yytype_uint8;
#else
typedef unsigned char yytype_uint8;
#endif

#ifdef YYTYPE_INT8
typedef YYTYPE_INT8 yytype_int8;
#elif (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
typedef signed char yytype_int8;
#else
typedef short int yytype_int8;
#endif

#ifdef YYTYPE_UINT16
typedef YYTYPE_UINT16 yytype_uint16;
#else
typedef unsigned short int yytype_uint16;
#endif

#ifdef YYTYPE_INT16
typedef YYTYPE_INT16 yytype_int16;
#else
typedef short int yytype_int16;
#endif

#ifndef YYSIZE_T
# ifdef __SIZE_TYPE__
#  define YYSIZE_T __SIZE_TYPE__
# elif defined size_t
#  define YYSIZE_T size_t
# elif ! defined YYSIZE_T && (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
#  include <stddef.h> /* INFRINGES ON USER NAME SPACE */
#  define YYSIZE_T size_t
# else
#  define YYSIZE_T unsigned int
# endif
#endif

#define YYSIZE_MAXIMUM ((YYSIZE_T) -1)

#ifndef YY_
# if defined YYENABLE_NLS && YYENABLE_NLS
#  if ENABLE_NLS
#   include <libintl.h> /* INFRINGES ON USER NAME SPACE */
#   define YY_(msgid) dgettext ("bison-runtime", msgid)
#  endif
# endif
# ifndef YY_
#  define YY_(msgid) msgid
# endif
#endif

/* Suppress unused-variable warnings by "using" E.  */
#if ! defined lint || defined __GNUC__
# define YYUSE(e) ((void) (e))
#else
# define YYUSE(e) /* empty */
#endif

/* Identity function, used to suppress warnings about constant conditions.  */
#ifndef lint
# define YYID(n) (n)
#else
#if (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
static int
YYID (int yyi)
#else
static int
YYID (yyi)
    int yyi;
#endif
{
  return yyi;
}
#endif

#if ! defined yyoverflow || YYERROR_VERBOSE

/* The parser invokes alloca or malloc; define the necessary symbols.  */

# ifdef YYSTACK_USE_ALLOCA
#  if YYSTACK_USE_ALLOCA
#   ifdef __GNUC__
#    define YYSTACK_ALLOC __builtin_alloca
#   elif defined __BUILTIN_VA_ARG_INCR
#    include <alloca.h> /* INFRINGES ON USER NAME SPACE */
#   elif defined _AIX
#    define YYSTACK_ALLOC __alloca
#   elif defined _MSC_VER
#    include <malloc.h> /* INFRINGES ON USER NAME SPACE */
#    define alloca _alloca
#   else
#    define YYSTACK_ALLOC alloca
#    if ! defined _ALLOCA_H && ! defined EXIT_SUCCESS && (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
#     include <stdlib.h> /* INFRINGES ON USER NAME SPACE */
#     ifndef EXIT_SUCCESS
#      define EXIT_SUCCESS 0
#     endif
#    endif
#   endif
#  endif
# endif

# ifdef YYSTACK_ALLOC
   /* Pacify GCC's `empty if-body' warning.  */
#  define YYSTACK_FREE(Ptr) do { /* empty */; } while (YYID (0))
#  ifndef YYSTACK_ALLOC_MAXIMUM
    /* The OS might guarantee only one guard page at the bottom of the stack,
       and a page size can be as small as 4096 bytes.  So we cannot safely
       invoke alloca (N) if N exceeds 4096.  Use a slightly smaller number
       to allow for a few compiler-allocated temporary stack slots.  */
#   define YYSTACK_ALLOC_MAXIMUM 4032 /* reasonable circa 2006 */
#  endif
# else
#  define YYSTACK_ALLOC YYMALLOC
#  define YYSTACK_FREE YYFREE
#  ifndef YYSTACK_ALLOC_MAXIMUM
#   define YYSTACK_ALLOC_MAXIMUM YYSIZE_MAXIMUM
#  endif
#  if (defined __cplusplus && ! defined EXIT_SUCCESS \
       && ! ((defined YYMALLOC || defined malloc) \
	     && (defined YYFREE || defined free)))
#   include <stdlib.h> /* INFRINGES ON USER NAME SPACE */
#   ifndef EXIT_SUCCESS
#    define EXIT_SUCCESS 0
#   endif
#  endif
#  ifndef YYMALLOC
#   define YYMALLOC malloc
#   if ! defined malloc && ! defined EXIT_SUCCESS && (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
void *malloc (YYSIZE_T); /* INFRINGES ON USER NAME SPACE */
#   endif
#  endif
#  ifndef YYFREE
#   define YYFREE free
#   if ! defined free && ! defined EXIT_SUCCESS && (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
void free (void *); /* INFRINGES ON USER NAME SPACE */
#   endif
#  endif
# endif
#endif /* ! defined yyoverflow || YYERROR_VERBOSE */


#if (! defined yyoverflow \
     && (! defined __cplusplus \
	 || (defined YYSTYPE_IS_TRIVIAL && YYSTYPE_IS_TRIVIAL)))

/* A type that is properly aligned for any stack member.  */
union yyalloc
{
  yytype_int16 yyss_alloc;
  YYSTYPE yyvs_alloc;
};

/* The size of the maximum gap between one aligned stack and the next.  */
# define YYSTACK_GAP_MAXIMUM (sizeof (union yyalloc) - 1)

/* The size of an array large to enough to hold all stacks, each with
   N elements.  */
# define YYSTACK_BYTES(N) \
     ((N) * (sizeof (yytype_int16) + sizeof (YYSTYPE)) \
      + YYSTACK_GAP_MAXIMUM)

# define YYCOPY_NEEDED 1

/* Relocate STACK from its old location to the new one.  The
   local variables YYSIZE and YYSTACKSIZE give the old and new number of
   elements in the stack, and YYPTR gives the new location of the
   stack.  Advance YYPTR to a properly aligned location for the next
   stack.  */
# define YYSTACK_RELOCATE(Stack_alloc, Stack)				\
    do									\
      {									\
	YYSIZE_T yynewbytes;						\
	YYCOPY (&yyptr->Stack_alloc, Stack, yysize);			\
	Stack = &yyptr->Stack_alloc;					\
	yynewbytes = yystacksize * sizeof (*Stack) + YYSTACK_GAP_MAXIMUM; \
	yyptr += yynewbytes / sizeof (*yyptr);				\
      }									\
    while (YYID (0))

#endif

#if defined YYCOPY_NEEDED && YYCOPY_NEEDED
/* Copy COUNT objects from FROM to TO.  The source and destination do
   not overlap.  */
# ifndef YYCOPY
#  if defined __GNUC__ && 1 < __GNUC__
#   define YYCOPY(To, From, Count) \
      __builtin_memcpy (To, From, (Count) * sizeof (*(From)))
#  else
#   define YYCOPY(To, From, Count)		\
      do					\
	{					\
	  YYSIZE_T yyi;				\
	  for (yyi = 0; yyi < (Count); yyi++)	\
	    (To)[yyi] = (From)[yyi];		\
	}					\
      while (YYID (0))
#  endif
# endif
#endif /* !YYCOPY_NEEDED */

/* YYFINAL -- State number of the termination state.  */
#define YYFINAL  4
/* YYLAST -- Last index in YYTABLE.  */
#define YYLAST   267

/* YYNTOKENS -- Number of terminals.  */
#define YYNTOKENS  55
/* YYNNTS -- Number of nonterminals.  */
#define YYNNTS  66
/* YYNRULES -- Number of rules.  */
#define YYNRULES  119
/* YYNRULES -- Number of states.  */
#define YYNSTATES  277

/* YYTRANSLATE(YYLEX) -- Bison symbol number corresponding to YYLEX.  */
#define YYUNDEFTOK  2
#define YYMAXUTOK   295

#define YYTRANSLATE(YYX)						\
  ((unsigned int) (YYX) <= YYMAXUTOK ? yytranslate[YYX] : YYUNDEFTOK)

/* YYTRANSLATE[YYLEX] -- Bison symbol number corresponding to YYLEX.  */
static const yytype_uint8 yytranslate[] =
{
       0,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
      42,    43,    53,    51,    45,    52,     2,    54,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,    44,    41,
      50,    48,    49,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,    46,     2,    47,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     1,     2,     3,     4,
       5,     6,     7,     8,     9,    10,    11,    12,    13,    14,
      15,    16,    17,    18,    19,    20,    21,    22,    23,    24,
      25,    26,    27,    28,    29,    30,    31,    32,    33,    34,
      35,    36,    37,    38,    39,    40
};

#if YYDEBUG
/* YYPRHS[YYN] -- Index of the first RHS symbol of rule number YYN in
   YYRHS.  */
static const yytype_uint16 yyprhs[] =
{
       0,     0,     3,     4,     5,    15,    16,    17,    33,    34,
      35,    51,    52,    58,    62,    63,    67,    68,    75,    81,
      85,    87,    89,    91,    93,    95,    97,    98,    99,   107,
     113,   114,   115,   126,   127,   128,   136,   139,   140,   142,
     144,   146,   148,   150,   152,   154,   156,   158,   159,   166,
     167,   168,   178,   179,   187,   188,   203,   208,   212,   216,
     218,   220,   222,   224,   226,   228,   229,   230,   242,   249,
     256,   263,   270,   277,   284,   291,   298,   305,   306,   311,
     312,   316,   318,   321,   322,   326,   327,   331,   332,   336,
     337,   341,   342,   346,   347,   351,   352,   354,   355,   360,
     361,   366,   368,   369,   374,   375,   380,   384,   386,   391,
     393,   395,   397,   399,   401,   403,   407,   409,   410,   413
};

/* YYRHS -- A `-1'-separated list of the rules' RHS.  */
static const yytype_int8 yyrhs[] =
{
      56,     0,    -1,    -1,    -1,     3,    24,    57,    41,   120,
      65,    59,    58,    69,    -1,    -1,    -1,    13,    24,    42,
      64,    43,    44,    68,   120,    60,    65,    73,    41,   120,
      61,    59,    -1,    -1,    -1,    13,    24,    42,    64,    43,
      44,    14,   120,    62,    65,    76,    41,   120,    63,    59,
      -1,    -1,    64,    45,    24,    44,    68,    -1,    24,    44,
      68,    -1,    -1,     9,   120,    66,    -1,    -1,    66,    67,
      44,    68,    41,   120,    -1,    67,    44,    68,    41,   120,
      -1,    67,    45,    24,    -1,    24,    -1,     4,    -1,     5,
      -1,    11,    -1,    12,    -1,     6,    -1,    -1,    -1,    21,
      70,   120,    79,    22,    71,   120,    -1,    46,   120,    79,
      47,   120,    -1,    -1,    -1,    46,   120,    74,    79,    23,
     103,    75,   120,    47,   120,    -1,    -1,    -1,    46,   120,
      77,    79,    47,    78,   120,    -1,    80,    79,    -1,    -1,
      81,    -1,    83,    -1,    86,    -1,    88,    -1,    93,    -1,
      97,    -1,    96,    -1,    98,    -1,    99,    -1,    -1,    24,
      48,   103,    82,    41,   120,    -1,    -1,    -1,     7,    42,
     103,    43,    84,   120,    72,    85,   100,    -1,    -1,    10,
      42,   102,    43,    87,    41,   120,    -1,    -1,    17,    42,
      24,    43,    18,    89,   120,    46,   120,    90,    47,   120,
      41,   120,    -1,    90,    91,    44,    80,    -1,    91,    44,
      80,    -1,    91,    45,    92,    -1,    92,    -1,    36,    -1,
      37,    -1,    38,    -1,    39,    -1,    40,    -1,    -1,    -1,
      15,    42,   103,    43,    94,    16,   120,    72,    41,    95,
     120,    -1,    20,    42,   111,    43,    41,   120,    -1,    19,
      42,    24,    43,    41,   120,    -1,    30,    42,   111,    43,
      41,   120,    -1,    31,    42,   111,    43,    41,   120,    -1,
      32,    42,   111,    43,    41,   120,    -1,    33,    42,   111,
      43,    41,   120,    -1,    34,    42,   111,    43,    41,   120,
      -1,    35,    42,   111,    43,    41,   120,    -1,    24,    42,
     119,    43,    41,   120,    -1,    -1,     8,   101,   120,    72,
      -1,    -1,   102,    45,   103,    -1,   103,    -1,   111,   104,
      -1,    -1,    49,   105,   111,    -1,    -1,    50,   106,   111,
      -1,    -1,    27,   107,   111,    -1,    -1,    28,   108,   111,
      -1,    -1,    25,   109,   111,    -1,    -1,    26,   110,   111,
      -1,    -1,   114,    -1,    -1,   111,    51,   112,   114,    -1,
      -1,   111,    52,   113,   114,    -1,   117,    -1,    -1,   114,
      53,   115,   117,    -1,    -1,   114,    54,   116,   117,    -1,
      42,   103,    43,    -1,   118,    -1,    24,    42,   119,    43,
      -1,    24,    -1,    36,    -1,    37,    -1,    38,    -1,    39,
      -1,    40,    -1,   119,    45,   111,    -1,   111,    -1,    -1,
      29,   120,    -1,    -1
};

/* YYRLINE[YYN] -- source line where rule number YYN was defined.  */
static const yytype_uint16 yyrline[] =
{
       0,   831,   831,   835,   831,   848,   863,   845,   874,   892,
     872,   899,   905,   915,   922,   927,   928,   932,   942,   953,
     958,   965,   966,   967,   968,   969,   973,   980,   973,   989,
     993,   998,   992,  1009,  1014,  1008,  1023,  1024,  1028,  1029,
    1030,  1031,  1032,  1033,  1034,  1035,  1036,  1041,  1040,  1076,
    1099,  1075,  1108,  1107,  1117,  1116,  1142,  1143,  1147,  1157,
    1170,  1174,  1178,  1184,  1189,  1202,  1226,  1200,  1235,  1265,
    1291,  1297,  1302,  1307,  1312,  1317,  1327,  1359,  1358,  1366,
    1370,  1392,  1418,  1422,  1422,  1431,  1430,  1440,  1439,  1450,
    1449,  1460,  1459,  1469,  1468,  1477,  1481,  1483,  1482,  1494,
    1493,  1507,  1509,  1508,  1518,  1517,  1528,  1529,  1533,  1578,
    1591,  1596,  1602,  1608,  1613,  1620,  1640,  1660,  1664,  1665
};
#endif

#if YYDEBUG || YYERROR_VERBOSE || YYTOKEN_TABLE
/* YYTNAME[SYMBOL-NUM] -- String name of the symbol SYMBOL-NUM.
   First, the terminals, then, starting at YYNTOKENS, nonterminals.  */
static const char *const yytname[] =
{
  "$end", "error", "$undefined", "PROG", "INT", "FLOAT", "BOOL", "IF",
  "ELSE", "VAR", "PRINT", "STRING", "CHAR", "FUNCTION", "VOID", "WHILE",
  "DO", "CASE", "OF", "READ", "SLEEP", "TBEGIN", "TEND", "TRET", "ID",
  "DIFF", "EQUALS", "LESSTHAN", "GREATERTHAN", "ENDL", "ROTATEWAIST",
  "ROTATESHOULDER", "ROTATEELBOW", "ROTATEWRISTM", "ROTATEWRISTR",
  "ROTATETOOL", "CTE_I", "CTE_F", "CTE_STRING", "CTE_CHAR", "CTE_BOOL",
  "';'", "'('", "')'", "':'", "','", "'{'", "'}'", "'='", "'>'", "'<'",
  "'+'", "'-'", "'*'", "'/'", "$accept", "programa", "$@1", "$@2",
  "functions", "$@3", "$@4", "$@5", "$@6", "def_param", "vars", "def_vars",
  "def_id", "tipo", "coding", "$@7", "$@8", "bloque", "bloque_func", "$@9",
  "$@10", "bloque_func2", "$@11", "$@12", "def_estatuto", "estatuto",
  "asignacion", "$@13", "condicion", "$@14", "$@15", "escritura", "$@16",
  "casos", "$@17", "options", "values", "constants", "ciclo", "$@18",
  "$@19", "sleep", "lectura", "especial", "llamadas", "def_else", "$@20",
  "mensaje", "expresion", "mas_expr", "$@21", "$@22", "$@23", "$@24",
  "$@25", "$@26", "exp", "$@27", "$@28", "termino", "$@29", "$@30",
  "factor", "variable", "params", "endl", 0
};
#endif

# ifdef YYPRINT
/* YYTOKNUM[YYLEX-NUM] -- Internal token number corresponding to
   token YYLEX-NUM.  */
static const yytype_uint16 yytoknum[] =
{
       0,   256,   257,   258,   259,   260,   261,   262,   263,   264,
     265,   266,   267,   268,   269,   270,   271,   272,   273,   274,
     275,   276,   277,   278,   279,   280,   281,   282,   283,   284,
     285,   286,   287,   288,   289,   290,   291,   292,   293,   294,
     295,    59,    40,    41,    58,    44,   123,   125,    61,    62,
      60,    43,    45,    42,    47
};
# endif

/* YYR1[YYN] -- Symbol number of symbol that rule YYN derives.  */
static const yytype_uint8 yyr1[] =
{
       0,    55,    57,    58,    56,    60,    61,    59,    62,    63,
      59,    59,    64,    64,    64,    65,    65,    66,    66,    67,
      67,    68,    68,    68,    68,    68,    70,    71,    69,    72,
      74,    75,    73,    77,    78,    76,    79,    79,    80,    80,
      80,    80,    80,    80,    80,    80,    80,    82,    81,    84,
      85,    83,    87,    86,    89,    88,    90,    90,    91,    91,
      92,    92,    92,    92,    92,    94,    95,    93,    96,    97,
      98,    98,    98,    98,    98,    98,    99,   101,   100,   100,
     102,   102,   103,   105,   104,   106,   104,   107,   104,   108,
     104,   109,   104,   110,   104,   104,   111,   112,   111,   113,
     111,   114,   115,   114,   116,   114,   117,   117,   118,   118,
     118,   118,   118,   118,   118,   119,   119,   119,   120,   120
};

/* YYR2[YYN] -- Number of symbols composing right hand side of rule YYN.  */
static const yytype_uint8 yyr2[] =
{
       0,     2,     0,     0,     9,     0,     0,    15,     0,     0,
      15,     0,     5,     3,     0,     3,     0,     6,     5,     3,
       1,     1,     1,     1,     1,     1,     0,     0,     7,     5,
       0,     0,    10,     0,     0,     7,     2,     0,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     0,     6,     0,
       0,     9,     0,     7,     0,    14,     4,     3,     3,     1,
       1,     1,     1,     1,     1,     0,     0,    11,     6,     6,
       6,     6,     6,     6,     6,     6,     6,     0,     4,     0,
       3,     1,     2,     0,     3,     0,     3,     0,     3,     0,
       3,     0,     3,     0,     3,     0,     1,     0,     4,     0,
       4,     1,     0,     4,     0,     4,     3,     1,     4,     1,
       1,     1,     1,     1,     1,     3,     1,     0,     2,     0
};

/* YYDEFACT[STATE-NAME] -- Default reduction number in state STATE-NUM.
   Performed when YYTABLE doesn't specify something else to do.  Zero
   means the default is an error.  */
static const yytype_uint8 yydefact[] =
{
       0,     0,     0,     2,     1,     0,   119,   119,    16,   118,
     119,    11,     0,     0,     3,    20,    15,     0,     0,     0,
       0,     0,     0,    14,    26,     4,     0,    21,    22,    25,
      23,    24,     0,    19,     0,     0,   119,     0,   119,     0,
       0,     0,    37,   119,    18,    13,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,    37,    38,    39,    40,    41,    42,    44,    43,
      45,    46,    17,   119,   119,     0,     0,     0,     0,     0,
       0,     0,   117,     0,     0,     0,     0,     0,     0,     0,
      27,    36,     8,     5,    12,   109,   110,   111,   112,   113,
     114,     0,     0,    95,    96,   101,   107,     0,    81,     0,
       0,     0,     0,   116,     0,    47,     0,     0,     0,     0,
       0,     0,   119,    16,    16,   117,     0,    49,    91,    93,
      87,    89,    83,    85,    97,    99,    82,   102,   104,    52,
       0,    65,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,    28,     0,     0,     0,   106,   119,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,    80,     0,    54,   119,   119,   119,   115,   119,   119,
     119,   119,   119,   119,   119,   119,     0,   119,     0,   108,
       0,    92,    94,    88,    90,    84,    86,    98,   100,   103,
     105,   119,   119,   119,    69,    68,    76,    48,    70,    71,
      72,    73,    74,    75,    33,   119,    30,   119,   119,    50,
      53,     0,     0,    37,     9,    37,     6,    37,    79,     0,
     119,     0,    11,     0,    11,     0,    77,    51,    66,     0,
      34,    10,     0,     7,   119,   119,   119,    60,    61,    62,
      63,    64,     0,     0,    59,   119,    31,    29,     0,    67,
     119,     0,     0,     0,    35,   119,    78,     0,     0,    57,
      58,     0,   119,    56,   119,    55,    32
};

/* YYDEFGOTO[NTERM-NUM].  */
static const yytype_int16 yydefgoto[] =
{
      -1,     2,     5,    19,    14,   124,   234,   123,   232,    35,
      11,    16,    17,    32,    25,    36,   122,   219,   188,   225,
     265,   186,   223,   255,    61,    62,    63,   147,    64,   159,
     228,    65,   170,    66,   203,   252,   253,   254,    67,   172,
     246,    68,    69,    70,    71,   237,   245,   107,   102,   136,
     164,   165,   162,   163,   160,   161,   103,   166,   167,   104,
     168,   169,   105,   106,   114,     8
};

/* YYPACT[STATE-NUM] -- Index in YYTABLE of the portion describing
   STATE-NUM.  */
#define YYPACT_NINF -217
static const yytype_int16 yypact[] =
{
       3,   -14,    19,  -217,  -217,    10,    36,    36,    30,  -217,
      36,     9,    46,    50,  -217,  -217,    46,    41,    64,   127,
      90,   116,   129,   130,  -217,  -217,   116,  -217,  -217,  -217,
    -217,  -217,    76,  -217,   107,    -5,    36,   114,    36,   116,
     112,   133,    13,    36,  -217,  -217,    97,   115,   118,   119,
     120,   121,   122,   123,   -21,   128,   140,   142,   143,   144,
     145,   136,    13,  -217,  -217,  -217,  -217,  -217,  -217,  -217,
    -217,  -217,  -217,    36,    36,   116,    58,    58,    58,   164,
     165,    58,    58,    58,    58,    58,    58,    58,    58,    58,
    -217,  -217,  -217,  -217,  -217,   148,  -217,  -217,  -217,  -217,
    -217,    58,   149,    29,    83,  -217,  -217,    62,  -217,   150,
     154,   155,   -17,    87,    71,  -217,    -2,    20,    25,    32,
      61,    67,    36,    30,    30,    58,   156,  -217,  -217,  -217,
    -217,  -217,  -217,  -217,  -217,  -217,  -217,  -217,  -217,  -217,
      58,  -217,   173,   159,   160,   161,    58,   162,   163,   166,
     168,   171,   172,   174,  -217,   170,   175,    80,  -217,    36,
      58,    58,    58,    58,    58,    58,    58,    58,    58,    58,
     176,  -217,   189,  -217,    36,    36,    36,    87,    36,    36,
      36,    36,    36,    36,    36,    36,   177,    36,   178,  -217,
     179,    87,    87,    87,    87,    87,    87,    83,    83,  -217,
    -217,    36,    36,    36,  -217,  -217,  -217,  -217,  -217,  -217,
    -217,  -217,  -217,  -217,  -217,    36,  -217,    36,    36,  -217,
    -217,   179,   180,    13,  -217,    13,  -217,    13,   198,   181,
      36,   167,     9,   197,     9,   182,  -217,  -217,  -217,    93,
    -217,  -217,    58,  -217,    36,    36,    36,  -217,  -217,  -217,
    -217,  -217,    22,    96,  -217,    36,  -217,  -217,   179,  -217,
      36,    98,    13,    93,  -217,    36,  -217,   183,    13,  -217,
    -217,   184,    36,  -217,    36,  -217,  -217
};

/* YYPGOTO[NTERM-NUM].  */
static const yytype_int16 yypgoto[] =
{
    -217,  -217,  -217,  -217,  -108,  -217,  -217,  -217,  -217,  -217,
      21,  -217,   211,   -22,  -217,  -217,  -217,  -216,  -217,  -217,
    -217,  -217,  -217,  -217,   -44,  -169,  -217,  -217,  -217,  -217,
    -217,  -217,  -217,  -217,  -217,  -217,   -24,   -33,  -217,  -217,
    -217,  -217,  -217,  -217,  -217,  -217,  -217,  -217,   -76,  -217,
    -217,  -217,  -217,  -217,  -217,  -217,   -73,  -217,  -217,   -20,
    -217,  -217,   -19,  -217,   108,    -7
};

/* YYTABLE[YYPACT[STATE-NUM]].  What to do in state STATE-NUM.  If
   positive, shift that token.  If negative, reduce the rule which
   number is the opposite.  If YYTABLE_NINF, syntax error.  */
#define YYTABLE_NINF -1
static const yytype_uint16 yytable[] =
{
       9,   108,   109,    12,    37,   229,     1,   115,   112,   113,
       3,   116,   117,   118,   119,   120,   121,    45,    91,     4,
      48,    82,    13,    49,    74,   126,   144,    83,    50,    42,
      51,    44,    52,    53,   134,   135,    72,    54,    40,    10,
      41,   148,   266,    55,    56,    57,    58,    59,    60,   134,
     135,     6,   113,    94,   128,   129,   130,   131,   247,   248,
     249,   250,   251,   149,   171,     7,    92,    93,   150,   260,
      15,   134,   135,   177,    18,   151,   134,   135,   132,   133,
     134,   135,    95,   134,   135,    21,    22,   191,   192,   193,
     194,   195,   196,   269,    96,    97,    98,    99,   100,   273,
     101,    27,    28,    29,   152,   139,    23,   140,    30,    31,
     153,    73,   134,   135,   145,   154,   146,    38,   134,   135,
      27,    28,    29,   189,   241,   146,   243,    30,    31,   247,
     248,   249,   250,   251,    26,    22,   137,   138,   134,   135,
     262,   263,   268,   263,   155,   156,   197,   198,    24,   199,
     200,    39,   190,    33,    34,    43,    46,    47,    90,    75,
      76,    77,    78,    79,    80,    81,   256,   204,   205,   206,
      84,   207,   208,   209,   210,   211,   212,   213,   214,   231,
     216,   233,    85,   235,    86,    87,    88,    89,   110,   111,
     125,   173,   127,   141,   220,   221,   222,   142,   143,   158,
     174,   175,   176,   178,   179,   202,   236,   180,   224,   181,
     226,   227,   182,   183,   240,   184,   185,   201,   215,   217,
     242,   187,   238,   239,   272,   218,   230,    20,   261,   244,
     270,   274,     0,   157,     0,     0,     0,   257,   258,   259,
       0,     0,     0,     0,     0,     0,     0,     0,   264,     0,
       0,     0,     0,   267,     0,     0,     0,     0,   271,     0,
       0,     0,     0,     0,     0,   275,     0,   276
};

#define yypact_value_is_default(yystate) \
  ((yystate) == (-217))

#define yytable_value_is_error(yytable_value) \
  YYID (0)

static const yytype_int16 yycheck[] =
{
       7,    77,    78,    10,    26,   221,     3,    83,    81,    82,
      24,    84,    85,    86,    87,    88,    89,    39,    62,     0,
       7,    42,    13,    10,    46,   101,    43,    48,    15,    36,
      17,    38,    19,    20,    51,    52,    43,    24,    43,     9,
      45,    43,   258,    30,    31,    32,    33,    34,    35,    51,
      52,    41,   125,    75,    25,    26,    27,    28,    36,    37,
      38,    39,    40,    43,   140,    29,    73,    74,    43,    47,
      24,    51,    52,   146,    24,    43,    51,    52,    49,    50,
      51,    52,    24,    51,    52,    44,    45,   160,   161,   162,
     163,   164,   165,   262,    36,    37,    38,    39,    40,   268,
      42,     4,     5,     6,    43,    43,    42,    45,    11,    12,
      43,    14,    51,    52,    43,   122,    45,    41,    51,    52,
       4,     5,     6,    43,   232,    45,   234,    11,    12,    36,
      37,    38,    39,    40,    44,    45,    53,    54,    51,    52,
      44,    45,    44,    45,   123,   124,   166,   167,    21,   168,
     169,    44,   159,    24,    24,    41,    44,    24,    22,    44,
      42,    42,    42,    42,    42,    42,   242,   174,   175,   176,
      42,   178,   179,   180,   181,   182,   183,   184,   185,   223,
     187,   225,    42,   227,    42,    42,    42,    42,    24,    24,
      42,    18,    43,    43,   201,   202,   203,    43,    43,    43,
      41,    41,    41,    41,    41,    16,     8,    41,   215,    41,
     217,   218,    41,    41,    47,    41,    46,    41,    41,    41,
      23,    46,    41,   230,    41,    46,    46,    16,   252,    47,
     263,    47,    -1,   125,    -1,    -1,    -1,   244,   245,   246,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   255,    -1,
      -1,    -1,    -1,   260,    -1,    -1,    -1,    -1,   265,    -1,
      -1,    -1,    -1,    -1,    -1,   272,    -1,   274
};

/* YYSTOS[STATE-NUM] -- The (internal number of the) accessing
   symbol of state STATE-NUM.  */
static const yytype_uint8 yystos[] =
{
       0,     3,    56,    24,     0,    57,    41,    29,   120,   120,
       9,    65,   120,    13,    59,    24,    66,    67,    24,    58,
      67,    44,    45,    42,    21,    69,    44,     4,     5,     6,
      11,    12,    68,    24,    24,    64,    70,    68,    41,    44,
      43,    45,   120,    41,   120,    68,    44,    24,     7,    10,
      15,    17,    19,    20,    24,    30,    31,    32,    33,    34,
      35,    79,    80,    81,    83,    86,    88,    93,    96,    97,
      98,    99,   120,    14,    68,    44,    42,    42,    42,    42,
      42,    42,    42,    48,    42,    42,    42,    42,    42,    42,
      22,    79,   120,   120,    68,    24,    36,    37,    38,    39,
      40,    42,   103,   111,   114,   117,   118,   102,   103,   103,
      24,    24,   111,   111,   119,   103,   111,   111,   111,   111,
     111,   111,    71,    62,    60,    42,   103,    43,    25,    26,
      27,    28,    49,    50,    51,    52,   104,    53,    54,    43,
      45,    43,    43,    43,    43,    43,    45,    82,    43,    43,
      43,    43,    43,    43,   120,    65,    65,   119,    43,    84,
     109,   110,   107,   108,   105,   106,   112,   113,   115,   116,
      87,   103,    94,    18,    41,    41,    41,   111,    41,    41,
      41,    41,    41,    41,    41,    46,    76,    46,    73,    43,
     120,   111,   111,   111,   111,   111,   111,   114,   114,   117,
     117,    41,    16,    89,   120,   120,   120,   120,   120,   120,
     120,   120,   120,   120,   120,    41,   120,    41,    46,    72,
     120,   120,   120,    77,   120,    74,   120,   120,    85,    72,
      46,    79,    63,    79,    61,    79,     8,   100,    41,   120,
      47,    59,    23,    59,    47,   101,    95,    36,    37,    38,
      39,    40,    90,    91,    92,    78,   103,   120,   120,   120,
      47,    91,    44,    45,   120,    75,    72,   120,    44,    80,
      92,   120,    41,    80,    47,   120,   120
};

#define yyerrok		(yyerrstatus = 0)
#define yyclearin	(yychar = YYEMPTY)
#define YYEMPTY		(-2)
#define YYEOF		0

#define YYACCEPT	goto yyacceptlab
#define YYABORT		goto yyabortlab
#define YYERROR		goto yyerrorlab


/* Like YYERROR except do call yyerror.  This remains here temporarily
   to ease the transition to the new meaning of YYERROR, for GCC.
   Once GCC version 2 has supplanted version 1, this can go.  However,
   YYFAIL appears to be in use.  Nevertheless, it is formally deprecated
   in Bison 2.4.2's NEWS entry, where a plan to phase it out is
   discussed.  */

#define YYFAIL		goto yyerrlab
#if defined YYFAIL
  /* This is here to suppress warnings from the GCC cpp's
     -Wunused-macros.  Normally we don't worry about that warning, but
     some users do, and we want to make it easy for users to remove
     YYFAIL uses, which will produce warnings from Bison 2.5.  */
#endif

#define YYRECOVERING()  (!!yyerrstatus)

#define YYBACKUP(Token, Value)					\
do								\
  if (yychar == YYEMPTY && yylen == 1)				\
    {								\
      yychar = (Token);						\
      yylval = (Value);						\
      YYPOPSTACK (1);						\
      goto yybackup;						\
    }								\
  else								\
    {								\
      yyerror (YY_("syntax error: cannot back up")); \
      YYERROR;							\
    }								\
while (YYID (0))


#define YYTERROR	1
#define YYERRCODE	256


/* YYLLOC_DEFAULT -- Set CURRENT to span from RHS[1] to RHS[N].
   If N is 0, then set CURRENT to the empty location which ends
   the previous symbol: RHS[0] (always defined).  */

#define YYRHSLOC(Rhs, K) ((Rhs)[K])
#ifndef YYLLOC_DEFAULT
# define YYLLOC_DEFAULT(Current, Rhs, N)				\
    do									\
      if (YYID (N))                                                    \
	{								\
	  (Current).first_line   = YYRHSLOC (Rhs, 1).first_line;	\
	  (Current).first_column = YYRHSLOC (Rhs, 1).first_column;	\
	  (Current).last_line    = YYRHSLOC (Rhs, N).last_line;		\
	  (Current).last_column  = YYRHSLOC (Rhs, N).last_column;	\
	}								\
      else								\
	{								\
	  (Current).first_line   = (Current).last_line   =		\
	    YYRHSLOC (Rhs, 0).last_line;				\
	  (Current).first_column = (Current).last_column =		\
	    YYRHSLOC (Rhs, 0).last_column;				\
	}								\
    while (YYID (0))
#endif


/* This macro is provided for backward compatibility. */

#ifndef YY_LOCATION_PRINT
# define YY_LOCATION_PRINT(File, Loc) ((void) 0)
#endif


/* YYLEX -- calling `yylex' with the right arguments.  */

#ifdef YYLEX_PARAM
# define YYLEX yylex (YYLEX_PARAM)
#else
# define YYLEX yylex ()
#endif

/* Enable debugging if requested.  */
#if YYDEBUG

# ifndef YYFPRINTF
#  include <stdio.h> /* INFRINGES ON USER NAME SPACE */
#  define YYFPRINTF fprintf
# endif

# define YYDPRINTF(Args)			\
do {						\
  if (yydebug)					\
    YYFPRINTF Args;				\
} while (YYID (0))

# define YY_SYMBOL_PRINT(Title, Type, Value, Location)			  \
do {									  \
  if (yydebug)								  \
    {									  \
      YYFPRINTF (stderr, "%s ", Title);					  \
      yy_symbol_print (stderr,						  \
		  Type, Value); \
      YYFPRINTF (stderr, "\n");						  \
    }									  \
} while (YYID (0))


/*--------------------------------.
| Print this symbol on YYOUTPUT.  |
`--------------------------------*/

/*ARGSUSED*/
#if (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
static void
yy_symbol_value_print (FILE *yyoutput, int yytype, YYSTYPE const * const yyvaluep)
#else
static void
yy_symbol_value_print (yyoutput, yytype, yyvaluep)
    FILE *yyoutput;
    int yytype;
    YYSTYPE const * const yyvaluep;
#endif
{
  if (!yyvaluep)
    return;
# ifdef YYPRINT
  if (yytype < YYNTOKENS)
    YYPRINT (yyoutput, yytoknum[yytype], *yyvaluep);
# else
  YYUSE (yyoutput);
# endif
  switch (yytype)
    {
      default:
	break;
    }
}


/*--------------------------------.
| Print this symbol on YYOUTPUT.  |
`--------------------------------*/

#if (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
static void
yy_symbol_print (FILE *yyoutput, int yytype, YYSTYPE const * const yyvaluep)
#else
static void
yy_symbol_print (yyoutput, yytype, yyvaluep)
    FILE *yyoutput;
    int yytype;
    YYSTYPE const * const yyvaluep;
#endif
{
  if (yytype < YYNTOKENS)
    YYFPRINTF (yyoutput, "token %s (", yytname[yytype]);
  else
    YYFPRINTF (yyoutput, "nterm %s (", yytname[yytype]);

  yy_symbol_value_print (yyoutput, yytype, yyvaluep);
  YYFPRINTF (yyoutput, ")");
}

/*------------------------------------------------------------------.
| yy_stack_print -- Print the state stack from its BOTTOM up to its |
| TOP (included).                                                   |
`------------------------------------------------------------------*/

#if (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
static void
yy_stack_print (yytype_int16 *yybottom, yytype_int16 *yytop)
#else
static void
yy_stack_print (yybottom, yytop)
    yytype_int16 *yybottom;
    yytype_int16 *yytop;
#endif
{
  YYFPRINTF (stderr, "Stack now");
  for (; yybottom <= yytop; yybottom++)
    {
      int yybot = *yybottom;
      YYFPRINTF (stderr, " %d", yybot);
    }
  YYFPRINTF (stderr, "\n");
}

# define YY_STACK_PRINT(Bottom, Top)				\
do {								\
  if (yydebug)							\
    yy_stack_print ((Bottom), (Top));				\
} while (YYID (0))


/*------------------------------------------------.
| Report that the YYRULE is going to be reduced.  |
`------------------------------------------------*/

#if (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
static void
yy_reduce_print (YYSTYPE *yyvsp, int yyrule)
#else
static void
yy_reduce_print (yyvsp, yyrule)
    YYSTYPE *yyvsp;
    int yyrule;
#endif
{
  int yynrhs = yyr2[yyrule];
  int yyi;
  unsigned long int yylno = yyrline[yyrule];
  YYFPRINTF (stderr, "Reducing stack by rule %d (line %lu):\n",
	     yyrule - 1, yylno);
  /* The symbols being reduced.  */
  for (yyi = 0; yyi < yynrhs; yyi++)
    {
      YYFPRINTF (stderr, "   $%d = ", yyi + 1);
      yy_symbol_print (stderr, yyrhs[yyprhs[yyrule] + yyi],
		       &(yyvsp[(yyi + 1) - (yynrhs)])
		       		       );
      YYFPRINTF (stderr, "\n");
    }
}

# define YY_REDUCE_PRINT(Rule)		\
do {					\
  if (yydebug)				\
    yy_reduce_print (yyvsp, Rule); \
} while (YYID (0))

/* Nonzero means print parse trace.  It is left uninitialized so that
   multiple parsers can coexist.  */
int yydebug;
#else /* !YYDEBUG */
# define YYDPRINTF(Args)
# define YY_SYMBOL_PRINT(Title, Type, Value, Location)
# define YY_STACK_PRINT(Bottom, Top)
# define YY_REDUCE_PRINT(Rule)
#endif /* !YYDEBUG */


/* YYINITDEPTH -- initial size of the parser's stacks.  */
#ifndef	YYINITDEPTH
# define YYINITDEPTH 200
#endif

/* YYMAXDEPTH -- maximum size the stacks can grow to (effective only
   if the built-in stack extension method is used).

   Do not make this value too large; the results are undefined if
   YYSTACK_ALLOC_MAXIMUM < YYSTACK_BYTES (YYMAXDEPTH)
   evaluated with infinite-precision integer arithmetic.  */

#ifndef YYMAXDEPTH
# define YYMAXDEPTH 10000
#endif


#if YYERROR_VERBOSE

# ifndef yystrlen
#  if defined __GLIBC__ && defined _STRING_H
#   define yystrlen strlen
#  else
/* Return the length of YYSTR.  */
#if (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
static YYSIZE_T
yystrlen (const char *yystr)
#else
static YYSIZE_T
yystrlen (yystr)
    const char *yystr;
#endif
{
  YYSIZE_T yylen;
  for (yylen = 0; yystr[yylen]; yylen++)
    continue;
  return yylen;
}
#  endif
# endif

# ifndef yystpcpy
#  if defined __GLIBC__ && defined _STRING_H && defined _GNU_SOURCE
#   define yystpcpy stpcpy
#  else
/* Copy YYSRC to YYDEST, returning the address of the terminating '\0' in
   YYDEST.  */
#if (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
static char *
yystpcpy (char *yydest, const char *yysrc)
#else
static char *
yystpcpy (yydest, yysrc)
    char *yydest;
    const char *yysrc;
#endif
{
  char *yyd = yydest;
  const char *yys = yysrc;

  while ((*yyd++ = *yys++) != '\0')
    continue;

  return yyd - 1;
}
#  endif
# endif

# ifndef yytnamerr
/* Copy to YYRES the contents of YYSTR after stripping away unnecessary
   quotes and backslashes, so that it's suitable for yyerror.  The
   heuristic is that double-quoting is unnecessary unless the string
   contains an apostrophe, a comma, or backslash (other than
   backslash-backslash).  YYSTR is taken from yytname.  If YYRES is
   null, do not copy; instead, return the length of what the result
   would have been.  */
static YYSIZE_T
yytnamerr (char *yyres, const char *yystr)
{
  if (*yystr == '"')
    {
      YYSIZE_T yyn = 0;
      char const *yyp = yystr;

      for (;;)
	switch (*++yyp)
	  {
	  case '\'':
	  case ',':
	    goto do_not_strip_quotes;

	  case '\\':
	    if (*++yyp != '\\')
	      goto do_not_strip_quotes;
	    /* Fall through.  */
	  default:
	    if (yyres)
	      yyres[yyn] = *yyp;
	    yyn++;
	    break;

	  case '"':
	    if (yyres)
	      yyres[yyn] = '\0';
	    return yyn;
	  }
    do_not_strip_quotes: ;
    }

  if (! yyres)
    return yystrlen (yystr);

  return yystpcpy (yyres, yystr) - yyres;
}
# endif

/* Copy into *YYMSG, which is of size *YYMSG_ALLOC, an error message
   about the unexpected token YYTOKEN for the state stack whose top is
   YYSSP.

   Return 0 if *YYMSG was successfully written.  Return 1 if *YYMSG is
   not large enough to hold the message.  In that case, also set
   *YYMSG_ALLOC to the required number of bytes.  Return 2 if the
   required number of bytes is too large to store.  */
static int
yysyntax_error (YYSIZE_T *yymsg_alloc, char **yymsg,
                yytype_int16 *yyssp, int yytoken)
{
  YYSIZE_T yysize0 = yytnamerr (0, yytname[yytoken]);
  YYSIZE_T yysize = yysize0;
  YYSIZE_T yysize1;
  enum { YYERROR_VERBOSE_ARGS_MAXIMUM = 5 };
  /* Internationalized format string. */
  const char *yyformat = 0;
  /* Arguments of yyformat. */
  char const *yyarg[YYERROR_VERBOSE_ARGS_MAXIMUM];
  /* Number of reported tokens (one for the "unexpected", one per
     "expected"). */
  int yycount = 0;

  /* There are many possibilities here to consider:
     - Assume YYFAIL is not used.  It's too flawed to consider.  See
       <http://lists.gnu.org/archive/html/bison-patches/2009-12/msg00024.html>
       for details.  YYERROR is fine as it does not invoke this
       function.
     - If this state is a consistent state with a default action, then
       the only way this function was invoked is if the default action
       is an error action.  In that case, don't check for expected
       tokens because there are none.
     - The only way there can be no lookahead present (in yychar) is if
       this state is a consistent state with a default action.  Thus,
       detecting the absence of a lookahead is sufficient to determine
       that there is no unexpected or expected token to report.  In that
       case, just report a simple "syntax error".
     - Don't assume there isn't a lookahead just because this state is a
       consistent state with a default action.  There might have been a
       previous inconsistent state, consistent state with a non-default
       action, or user semantic action that manipulated yychar.
     - Of course, the expected token list depends on states to have
       correct lookahead information, and it depends on the parser not
       to perform extra reductions after fetching a lookahead from the
       scanner and before detecting a syntax error.  Thus, state merging
       (from LALR or IELR) and default reductions corrupt the expected
       token list.  However, the list is correct for canonical LR with
       one exception: it will still contain any token that will not be
       accepted due to an error action in a later state.
  */
  if (yytoken != YYEMPTY)
    {
      int yyn = yypact[*yyssp];
      yyarg[yycount++] = yytname[yytoken];
      if (!yypact_value_is_default (yyn))
        {
          /* Start YYX at -YYN if negative to avoid negative indexes in
             YYCHECK.  In other words, skip the first -YYN actions for
             this state because they are default actions.  */
          int yyxbegin = yyn < 0 ? -yyn : 0;
          /* Stay within bounds of both yycheck and yytname.  */
          int yychecklim = YYLAST - yyn + 1;
          int yyxend = yychecklim < YYNTOKENS ? yychecklim : YYNTOKENS;
          int yyx;

          for (yyx = yyxbegin; yyx < yyxend; ++yyx)
            if (yycheck[yyx + yyn] == yyx && yyx != YYTERROR
                && !yytable_value_is_error (yytable[yyx + yyn]))
              {
                if (yycount == YYERROR_VERBOSE_ARGS_MAXIMUM)
                  {
                    yycount = 1;
                    yysize = yysize0;
                    break;
                  }
                yyarg[yycount++] = yytname[yyx];
                yysize1 = yysize + yytnamerr (0, yytname[yyx]);
                if (! (yysize <= yysize1
                       && yysize1 <= YYSTACK_ALLOC_MAXIMUM))
                  return 2;
                yysize = yysize1;
              }
        }
    }

  switch (yycount)
    {
# define YYCASE_(N, S)                      \
      case N:                               \
        yyformat = S;                       \
      break
      YYCASE_(0, YY_("syntax error"));
      YYCASE_(1, YY_("syntax error, unexpected %s"));
      YYCASE_(2, YY_("syntax error, unexpected %s, expecting %s"));
      YYCASE_(3, YY_("syntax error, unexpected %s, expecting %s or %s"));
      YYCASE_(4, YY_("syntax error, unexpected %s, expecting %s or %s or %s"));
      YYCASE_(5, YY_("syntax error, unexpected %s, expecting %s or %s or %s or %s"));
# undef YYCASE_
    }

  yysize1 = yysize + yystrlen (yyformat);
  if (! (yysize <= yysize1 && yysize1 <= YYSTACK_ALLOC_MAXIMUM))
    return 2;
  yysize = yysize1;

  if (*yymsg_alloc < yysize)
    {
      *yymsg_alloc = 2 * yysize;
      if (! (yysize <= *yymsg_alloc
             && *yymsg_alloc <= YYSTACK_ALLOC_MAXIMUM))
        *yymsg_alloc = YYSTACK_ALLOC_MAXIMUM;
      return 1;
    }

  /* Avoid sprintf, as that infringes on the user's name space.
     Don't have undefined behavior even if the translation
     produced a string with the wrong number of "%s"s.  */
  {
    char *yyp = *yymsg;
    int yyi = 0;
    while ((*yyp = *yyformat) != '\0')
      if (*yyp == '%' && yyformat[1] == 's' && yyi < yycount)
        {
          yyp += yytnamerr (yyp, yyarg[yyi++]);
          yyformat += 2;
        }
      else
        {
          yyp++;
          yyformat++;
        }
  }
  return 0;
}
#endif /* YYERROR_VERBOSE */

/*-----------------------------------------------.
| Release the memory associated to this symbol.  |
`-----------------------------------------------*/

/*ARGSUSED*/
#if (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
static void
yydestruct (const char *yymsg, int yytype, YYSTYPE *yyvaluep)
#else
static void
yydestruct (yymsg, yytype, yyvaluep)
    const char *yymsg;
    int yytype;
    YYSTYPE *yyvaluep;
#endif
{
  YYUSE (yyvaluep);

  if (!yymsg)
    yymsg = "Deleting";
  YY_SYMBOL_PRINT (yymsg, yytype, yyvaluep, yylocationp);

  switch (yytype)
    {

      default:
	break;
    }
}


/* Prevent warnings from -Wmissing-prototypes.  */
#ifdef YYPARSE_PARAM
#if defined __STDC__ || defined __cplusplus
int yyparse (void *YYPARSE_PARAM);
#else
int yyparse ();
#endif
#else /* ! YYPARSE_PARAM */
#if defined __STDC__ || defined __cplusplus
int yyparse (void);
#else
int yyparse ();
#endif
#endif /* ! YYPARSE_PARAM */


/* The lookahead symbol.  */
int yychar;

/* The semantic value of the lookahead symbol.  */
YYSTYPE yylval;

/* Number of syntax errors so far.  */
int yynerrs;


/*----------.
| yyparse.  |
`----------*/

#ifdef YYPARSE_PARAM
#if (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
int
yyparse (void *YYPARSE_PARAM)
#else
int
yyparse (YYPARSE_PARAM)
    void *YYPARSE_PARAM;
#endif
#else /* ! YYPARSE_PARAM */
#if (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
int
yyparse (void)
#else
int
yyparse ()

#endif
#endif
{
    int yystate;
    /* Number of tokens to shift before error messages enabled.  */
    int yyerrstatus;

    /* The stacks and their tools:
       `yyss': related to states.
       `yyvs': related to semantic values.

       Refer to the stacks thru separate pointers, to allow yyoverflow
       to reallocate them elsewhere.  */

    /* The state stack.  */
    yytype_int16 yyssa[YYINITDEPTH];
    yytype_int16 *yyss;
    yytype_int16 *yyssp;

    /* The semantic value stack.  */
    YYSTYPE yyvsa[YYINITDEPTH];
    YYSTYPE *yyvs;
    YYSTYPE *yyvsp;

    YYSIZE_T yystacksize;

  int yyn;
  int yyresult;
  /* Lookahead token as an internal (translated) token number.  */
  int yytoken;
  /* The variables used to return semantic value and location from the
     action routines.  */
  YYSTYPE yyval;

#if YYERROR_VERBOSE
  /* Buffer for error messages, and its allocated size.  */
  char yymsgbuf[128];
  char *yymsg = yymsgbuf;
  YYSIZE_T yymsg_alloc = sizeof yymsgbuf;
#endif

#define YYPOPSTACK(N)   (yyvsp -= (N), yyssp -= (N))

  /* The number of symbols on the RHS of the reduced rule.
     Keep to zero when no symbol should be popped.  */
  int yylen = 0;

  yytoken = 0;
  yyss = yyssa;
  yyvs = yyvsa;
  yystacksize = YYINITDEPTH;

  YYDPRINTF ((stderr, "Starting parse\n"));

  yystate = 0;
  yyerrstatus = 0;
  yynerrs = 0;
  yychar = YYEMPTY; /* Cause a token to be read.  */

  /* Initialize stack pointers.
     Waste one element of value and location stack
     so that they stay on the same level as the state stack.
     The wasted elements are never initialized.  */
  yyssp = yyss;
  yyvsp = yyvs;

  goto yysetstate;

/*------------------------------------------------------------.
| yynewstate -- Push a new state, which is found in yystate.  |
`------------------------------------------------------------*/
 yynewstate:
  /* In all cases, when you get here, the value and location stacks
     have just been pushed.  So pushing a state here evens the stacks.  */
  yyssp++;

 yysetstate:
  *yyssp = yystate;

  if (yyss + yystacksize - 1 <= yyssp)
    {
      /* Get the current used size of the three stacks, in elements.  */
      YYSIZE_T yysize = yyssp - yyss + 1;

#ifdef yyoverflow
      {
	/* Give user a chance to reallocate the stack.  Use copies of
	   these so that the &'s don't force the real ones into
	   memory.  */
	YYSTYPE *yyvs1 = yyvs;
	yytype_int16 *yyss1 = yyss;

	/* Each stack pointer address is followed by the size of the
	   data in use in that stack, in bytes.  This used to be a
	   conditional around just the two extra args, but that might
	   be undefined if yyoverflow is a macro.  */
	yyoverflow (YY_("memory exhausted"),
		    &yyss1, yysize * sizeof (*yyssp),
		    &yyvs1, yysize * sizeof (*yyvsp),
		    &yystacksize);

	yyss = yyss1;
	yyvs = yyvs1;
      }
#else /* no yyoverflow */
# ifndef YYSTACK_RELOCATE
      goto yyexhaustedlab;
# else
      /* Extend the stack our own way.  */
      if (YYMAXDEPTH <= yystacksize)
	goto yyexhaustedlab;
      yystacksize *= 2;
      if (YYMAXDEPTH < yystacksize)
	yystacksize = YYMAXDEPTH;

      {
	yytype_int16 *yyss1 = yyss;
	union yyalloc *yyptr =
	  (union yyalloc *) YYSTACK_ALLOC (YYSTACK_BYTES (yystacksize));
	if (! yyptr)
	  goto yyexhaustedlab;
	YYSTACK_RELOCATE (yyss_alloc, yyss);
	YYSTACK_RELOCATE (yyvs_alloc, yyvs);
#  undef YYSTACK_RELOCATE
	if (yyss1 != yyssa)
	  YYSTACK_FREE (yyss1);
      }
# endif
#endif /* no yyoverflow */

      yyssp = yyss + yysize - 1;
      yyvsp = yyvs + yysize - 1;

      YYDPRINTF ((stderr, "Stack size increased to %lu\n",
		  (unsigned long int) yystacksize));

      if (yyss + yystacksize - 1 <= yyssp)
	YYABORT;
    }

  YYDPRINTF ((stderr, "Entering state %d\n", yystate));

  if (yystate == YYFINAL)
    YYACCEPT;

  goto yybackup;

/*-----------.
| yybackup.  |
`-----------*/
yybackup:

  /* Do appropriate processing given the current state.  Read a
     lookahead token if we need one and don't already have one.  */

  /* First try to decide what to do without reference to lookahead token.  */
  yyn = yypact[yystate];
  if (yypact_value_is_default (yyn))
    goto yydefault;

  /* Not known => get a lookahead token if don't already have one.  */

  /* YYCHAR is either YYEMPTY or YYEOF or a valid lookahead symbol.  */
  if (yychar == YYEMPTY)
    {
      YYDPRINTF ((stderr, "Reading a token: "));
      yychar = YYLEX;
    }

  if (yychar <= YYEOF)
    {
      yychar = yytoken = YYEOF;
      YYDPRINTF ((stderr, "Now at end of input.\n"));
    }
  else
    {
      yytoken = YYTRANSLATE (yychar);
      YY_SYMBOL_PRINT ("Next token is", yytoken, &yylval, &yylloc);
    }

  /* If the proper action on seeing token YYTOKEN is to reduce or to
     detect an error, take that action.  */
  yyn += yytoken;
  if (yyn < 0 || YYLAST < yyn || yycheck[yyn] != yytoken)
    goto yydefault;
  yyn = yytable[yyn];
  if (yyn <= 0)
    {
      if (yytable_value_is_error (yyn))
        goto yyerrlab;
      yyn = -yyn;
      goto yyreduce;
    }

  /* Count tokens shifted since error; after three, turn off error
     status.  */
  if (yyerrstatus)
    yyerrstatus--;

  /* Shift the lookahead token.  */
  YY_SYMBOL_PRINT ("Shifting", yytoken, &yylval, &yylloc);

  /* Discard the shifted token.  */
  yychar = YYEMPTY;

  yystate = yyn;
  *++yyvsp = yylval;

  goto yynewstate;


/*-----------------------------------------------------------.
| yydefault -- do the default action for the current state.  |
`-----------------------------------------------------------*/
yydefault:
  yyn = yydefact[yystate];
  if (yyn == 0)
    goto yyerrlab;
  goto yyreduce;


/*-----------------------------.
| yyreduce -- Do a reduction.  |
`-----------------------------*/
yyreduce:
  /* yyn is the number of a rule to reduce with.  */
  yylen = yyr2[yyn];

  /* If YYLEN is nonzero, implement the default value of the action:
     `$$ = $1'.

     Otherwise, the following line sets YYVAL to garbage.
     This behavior is undocumented and Bison
     users should not rely upon it.  Assigning to YYVAL
     unconditionally makes the parser a bit smaller, and it avoids a
     GCC warning that YYVAL may be used uninitialized.  */
  yyval = yyvsp[1-yylen];


  YY_REDUCE_PRINT (yyn);
  switch (yyn)
    {
        case 2:

/* Line 1806 of yacc.c  */
#line 831 "sintax.y"
    {
		//Registra el nombre del programa
		pragma.setNombre((yyvsp[(2) - (2)].sval));
			}
    break;

  case 3:

/* Line 1806 of yacc.c  */
#line 835 "sintax.y"
    {
		
		//Al terminar de declarar variables desahbilita que las variables de alojen de forma local a la función
		localFlag=false;
	}
    break;

  case 5:

/* Line 1806 of yacc.c  */
#line 848 "sintax.y"
    {
			//Activia alojamiento local a la funcion
			localFlag=true;
			//Guarda la función en la tabla
			if (guardaFuncion(1,(yyvsp[(2) - (8)].sval),(yyvsp[(7) - (8)].ival), parametros)==-1)
			{
				exit(-1);
			}
			//Guarda el tipo en una variable para verificar que el return de la función sea compatible
			tipoFunction=(yyvsp[(7) - (8)].ival);
			//Vacia params
			parametros="";
		}
    break;

  case 6:

/* Line 1806 of yacc.c  */
#line 863 "sintax.y"
    {	
			//vaciar la estructura de variables para iniciar una nueva funcion
			entry.setVarTable(new HashMap<varEntry>);
			
	
		}
    break;

  case 8:

/* Line 1806 of yacc.c  */
#line 874 "sintax.y"
    {
		//Activia alojamiento local a la funcion
		localFlag=true;
	
		//Guarda la función en la tabla
			if (guardaFuncion(2,(yyvsp[(2) - (8)].sval),5, parametros)==-1)
			{
				exit(-1);
			}
		//Guarda que la funcion es tipo void
		tipoFunction=5;
			//Vaciaa params
			parametros="";

		
	}
    break;

  case 9:

/* Line 1806 of yacc.c  */
#line 892 "sintax.y"
    {	

			//vaciar la estructura de variables para iniciar una nueva funcion
			entry.setVarTable(new HashMap<varEntry>);
		
		}
    break;

  case 12:

/* Line 1806 of yacc.c  */
#line 906 "sintax.y"
    {
		
		if (!guardaParametros((yyvsp[(3) - (5)].sval),(yyvsp[(5) - (5)].ival)))
			{
				exit(-1);
			}
		
		
		}
    break;

  case 13:

/* Line 1806 of yacc.c  */
#line 916 "sintax.y"
    {
		if (!guardaParametros((yyvsp[(1) - (3)].sval),(yyvsp[(3) - (3)].ival)))
			{
				exit(-1);
			}
	}
    break;

  case 17:

/* Line 1806 of yacc.c  */
#line 933 "sintax.y"
    {

			if (!guardaVars((yyvsp[(4) - (6)].ival)))
			{
				cout << "Redeclaration Variable on line: " << line_num << endl;
			exit (-1);
			}
		}
    break;

  case 18:

/* Line 1806 of yacc.c  */
#line 943 "sintax.y"
    {
			if (!guardaVars((yyvsp[(3) - (5)].ival)))
			{
				cout << "Redeclaration Variable on line: " << line_num << endl;
			exit (-1);
			}
		}
    break;

  case 19:

/* Line 1806 of yacc.c  */
#line 954 "sintax.y"
    {
		IDQueue.enqueue((yyvsp[(3) - (3)].sval));
		
	}
    break;

  case 20:

/* Line 1806 of yacc.c  */
#line 959 "sintax.y"
    {
		IDQueue.enqueue((yyvsp[(1) - (1)].sval));
	}
    break;

  case 21:

/* Line 1806 of yacc.c  */
#line 965 "sintax.y"
    {(yyval.ival)=2;}
    break;

  case 22:

/* Line 1806 of yacc.c  */
#line 966 "sintax.y"
    {(yyval.ival)=3;}
    break;

  case 23:

/* Line 1806 of yacc.c  */
#line 967 "sintax.y"
    {(yyval.ival)=1;}
    break;

  case 24:

/* Line 1806 of yacc.c  */
#line 968 "sintax.y"
    {(yyval.ival)=0;}
    break;

  case 25:

/* Line 1806 of yacc.c  */
#line 969 "sintax.y"
    {(yyval.ival)=4;}
    break;

  case 26:

/* Line 1806 of yacc.c  */
#line 973 "sintax.y"
    {

		//Debug
		myQuadStructure<<"BEGIN"<<endl;
		cout<<"*****Coding Start****"<<endl;
	}
    break;

  case 27:

/* Line 1806 of yacc.c  */
#line 980 "sintax.y"
    {
			//Debug
			myQuadStructure<<"END"<<endl;
			cout<<"****Coding Finish*****"<<endl;
		}
    break;

  case 30:

/* Line 1806 of yacc.c  */
#line 993 "sintax.y"
    {
		//Debug
		cout<<"****Function  Code****"<<endl;
	}
    break;

  case 31:

/* Line 1806 of yacc.c  */
#line 998 "sintax.y"
    {
		if (generaRET()==-1)
		{
			exit(-1);
		}
	}
    break;

  case 33:

/* Line 1806 of yacc.c  */
#line 1009 "sintax.y"
    {
		//Debug
		cout<<"****Function  Code****"<<endl;
	}
    break;

  case 34:

/* Line 1806 of yacc.c  */
#line 1014 "sintax.y"
    {
		//Debug
		cout<<"Return void"<<endl;
		myQuadStructure<<"RETVOID"<<endl;
	}
    break;

  case 47:

/* Line 1806 of yacc.c  */
#line 1041 "sintax.y"
    {
		int tipoID;
		int tipoExp;
		string operador;
		tipoID=buscaID((yyvsp[(1) - (3)].sval));

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

		myQuadStructure<<"=\t"<<operador<<"\t"<<(yyvsp[(1) - (3)].sval)<<endl;

	}
    break;

  case 49:

/* Line 1806 of yacc.c  */
#line 1076 "sintax.y"
    {
		int tipoBool;
		string operador;
		if (!pTipos.pop(tipoBool))
		{
			cout<<"No hay tipo"<<endl;
		}
		if (tipoBool!=4)
		{
			cout<<"Incompatible types! expected condition on line:"<<line_num<<endl;
		}

		if (!pilaO.pop(operador))
		{
			cout<<"No hay operador"<<endl;
		}


		myQuadStructure<<"IF\t"<<operador<<endl;


	}
    break;

  case 50:

/* Line 1806 of yacc.c  */
#line 1099 "sintax.y"
    {
		myQuadStructure<<"ENDIF"<<endl;
	}
    break;

  case 52:

/* Line 1806 of yacc.c  */
#line 1108 "sintax.y"
    {
		myQuadStructure<<"ENDL"<<endl;
	}
    break;

  case 54:

/* Line 1806 of yacc.c  */
#line 1117 "sintax.y"
    {
	

		tipoCase=buscaID((yyvsp[(3) - (5)].sval));

		if (tipoCase == -1)
		{
			cout<<"Variable not defined on line:"<<line_num<<endl;
			exit(-1);
		}

		myQuadStructure<<"CASE\t"<<(yyvsp[(3) - (5)].sval)<<endl;
	}
    break;

  case 55:

/* Line 1806 of yacc.c  */
#line 1135 "sintax.y"
    {
	myQuadStructure<<"ENDCASE"<<endl;
	}
    break;

  case 58:

/* Line 1806 of yacc.c  */
#line 1148 "sintax.y"
    {
	if (tipoCase!=(yyvsp[(3) - (3)].ival))
	{
		cout<<"Incompatible types! on line: "<<line_num<<endl;
		exit(-1);
	}
	myQuadStructure<<"EXTRAOPTION\t"<<optionStr<<endl;
}
    break;

  case 59:

/* Line 1806 of yacc.c  */
#line 1158 "sintax.y"
    {
	if (tipoCase!=(yyvsp[(1) - (1)].ival))
	{
		cout<<"Incompatible types! on line: "<<line_num<<endl;
		exit(-1);
	}
	myQuadStructure<<"OPTION\t"<<optionStr<<endl;
}
    break;

  case 60:

/* Line 1806 of yacc.c  */
#line 1170 "sintax.y"
    {
		(yyval.ival)=2;
		optionStr=(yyvsp[(1) - (1)].sval);
		}
    break;

  case 61:

/* Line 1806 of yacc.c  */
#line 1174 "sintax.y"
    {
		(yyval.ival)=3;
		optionStr=(yyvsp[(1) - (1)].sval);
		}
    break;

  case 62:

/* Line 1806 of yacc.c  */
#line 1178 "sintax.y"
    {

		(yyval.ival)=1;
		optionStr=(yyvsp[(1) - (1)].sval);
		
		}
    break;

  case 63:

/* Line 1806 of yacc.c  */
#line 1184 "sintax.y"
    {
		(yyval.ival)=0;
		optionStr=(yyvsp[(1) - (1)].sval);

	}
    break;

  case 64:

/* Line 1806 of yacc.c  */
#line 1189 "sintax.y"
    {
		(yyval.ival)=4;
		optionStr=(yyvsp[(1) - (1)].sval);
	}
    break;

  case 65:

/* Line 1806 of yacc.c  */
#line 1202 "sintax.y"
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
    break;

  case 66:

/* Line 1806 of yacc.c  */
#line 1226 "sintax.y"
    {
	 	myQuadStructure<<"ENDWHILE"<<endl;
	 }
    break;

  case 68:

/* Line 1806 of yacc.c  */
#line 1236 "sintax.y"
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
			cout<<"Incompatible Types on sleep function! on line number:"<<line_num<<endl;
			return -1;
		}


		myQuadStructure<<"SLEEP\t"<<operador<<endl;

	}
    break;

  case 69:

/* Line 1806 of yacc.c  */
#line 1267 "sintax.y"
    {
		int tipoID;

		tipoID=buscaID((yyvsp[(3) - (6)].sval));

		if (tipoID == -1)
		{
			cout<<"Variable not defined on line:"<<line_num<<endl;
			exit(-1);
		}


		if (tipoID!=4)
		{
			cout<<"Cannot read variable! on line:"<<line_num<<endl;
			exit(-1);
		}

		myQuadStructure<<"READ\t"<<(yyvsp[(3) - (6)].sval)<<endl;
	}
    break;

  case 70:

/* Line 1806 of yacc.c  */
#line 1292 "sintax.y"
    {
		if (generaEspecial(0)==-1)
			exit(-1);
	}
    break;

  case 71:

/* Line 1806 of yacc.c  */
#line 1298 "sintax.y"
    {
		if (generaEspecial(1)==-1)
			exit(-1);
	}
    break;

  case 72:

/* Line 1806 of yacc.c  */
#line 1303 "sintax.y"
    {
		if (generaEspecial(2)==-1)
			exit(-1);
	}
    break;

  case 73:

/* Line 1806 of yacc.c  */
#line 1308 "sintax.y"
    {
		if (generaEspecial(3)==-1)
			exit(-1);
	}
    break;

  case 74:

/* Line 1806 of yacc.c  */
#line 1313 "sintax.y"
    {
		if (generaEspecial(4)==-1)
			exit(-1);
	}
    break;

  case 75:

/* Line 1806 of yacc.c  */
#line 1318 "sintax.y"
    {
		if (generaEspecial(5)==-1)
			exit(-1);
	}
    break;

  case 76:

/* Line 1806 of yacc.c  */
#line 1328 "sintax.y"
    {

		{
			int tipo=-1;
			string params="";
			tipo=buscaFuncion((yyvsp[(1) - (6)].sval), params);
			cout<<"Funcion: "<<(yyvsp[(1) - (6)].sval)<<"\tTipo:"<<tipo<<"\tParametros registrados: "<<params<<endl;
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
					cout<<"Wrong parameters! on line number"<<line_num<<endl;
					exit(-1);
				}
		}
	}
    break;

  case 77:

/* Line 1806 of yacc.c  */
#line 1359 "sintax.y"
    {
		myQuadStructure<<"ELSE"<<endl;
	}
    break;

  case 78:

/* Line 1806 of yacc.c  */
#line 1363 "sintax.y"
    {
		myQuadStructure<<"ENDELSE"<<endl;	
	}
    break;

  case 80:

/* Line 1806 of yacc.c  */
#line 1371 "sintax.y"
    {
		int tipoString;
		string operador;
		if (!pTipos.pop(tipoString))
		{
			cout<<"No hay tipo"<<endl;
		}
		
		if (tipoString==3 || tipoString==4)
		 {
		 	cout<<"Incompatible types! to print on line:"<<line_num<<endl;
		 	exit(-1);
		 }

		if (!pilaO.pop(operador))
		{
			cout<<"No hay operador"<<endl;
		}
		myQuadStructure<<"PRINT\t"<<operador<<endl;

	}
    break;

  case 81:

/* Line 1806 of yacc.c  */
#line 1393 "sintax.y"
    {
		int tipoString;
		string operador;
		if (!pTipos.pop(tipoString))
		{
			cout<<"No hay tipo"<<endl;
		}
		
		if (tipoString==3 || tipoString==4)
		 {
		 	cout<<"Incompatible types! to print on line:"<<line_num<<endl;
		 	exit(-1);
		 }

		if (!pilaO.pop(operador))
		{
			cout<<"No hay operador"<<endl;
		}
		myQuadStructure<<"PRINT\t"<<operador<<endl;

	}
    break;

  case 83:

/* Line 1806 of yacc.c  */
#line 1422 "sintax.y"
    {
			pilaOper.push(">");
		}
    break;

  case 84:

/* Line 1806 of yacc.c  */
#line 1426 "sintax.y"
    {
			if(generaExpresion()==-1)
			exit(-1);
		}
    break;

  case 85:

/* Line 1806 of yacc.c  */
#line 1431 "sintax.y"
    {
			pilaOper.push("<");
		}
    break;

  case 86:

/* Line 1806 of yacc.c  */
#line 1435 "sintax.y"
    {
	 		if(generaExpresion()==-1)
			exit(-1);	
	 	}
    break;

  case 87:

/* Line 1806 of yacc.c  */
#line 1440 "sintax.y"
    {
			pilaOper.push("<=");
		}
    break;

  case 88:

/* Line 1806 of yacc.c  */
#line 1445 "sintax.y"
    {
	 		if(generaExpresion()==-1)
			exit(-1);	
	 	}
    break;

  case 89:

/* Line 1806 of yacc.c  */
#line 1450 "sintax.y"
    {
			pilaOper.push(">=");
		}
    break;

  case 90:

/* Line 1806 of yacc.c  */
#line 1454 "sintax.y"
    {
	 		if(generaExpresion()==-1)
			exit(-1);	
	 	}
    break;

  case 91:

/* Line 1806 of yacc.c  */
#line 1460 "sintax.y"
    {
			pilaOper.push("<>");
		}
    break;

  case 92:

/* Line 1806 of yacc.c  */
#line 1464 "sintax.y"
    {
	 		if(generaExpresion()==-1)
			exit(-1);	
	 	}
    break;

  case 93:

/* Line 1806 of yacc.c  */
#line 1469 "sintax.y"
    {
			pilaOper.push("==");
		}
    break;

  case 94:

/* Line 1806 of yacc.c  */
#line 1473 "sintax.y"
    {
			if(generaExpresion()==-1)
			exit(-1);	
		}
    break;

  case 97:

/* Line 1806 of yacc.c  */
#line 1483 "sintax.y"
    {
		pilaOper.push("+");
		
	}
    break;

  case 98:

/* Line 1806 of yacc.c  */
#line 1489 "sintax.y"
    {
		if(generaExpresion()==-1)
			exit(-1);
	 }
    break;

  case 99:

/* Line 1806 of yacc.c  */
#line 1494 "sintax.y"
    {
		pilaOper.push("-");
	}
    break;

  case 100:

/* Line 1806 of yacc.c  */
#line 1498 "sintax.y"
    {
		if(generaExpresion()==-1)
			exit(-1);
	 }
    break;

  case 102:

/* Line 1806 of yacc.c  */
#line 1509 "sintax.y"
    {
		pilaOper.push("*");
	}
    break;

  case 103:

/* Line 1806 of yacc.c  */
#line 1513 "sintax.y"
    {
		if(generaExpresion()==-1)
			exit(-1);
	}
    break;

  case 104:

/* Line 1806 of yacc.c  */
#line 1518 "sintax.y"
    {
	pilaOper.push("/");
	}
    break;

  case 105:

/* Line 1806 of yacc.c  */
#line 1522 "sintax.y"
    {
		if(generaExpresion()==-1)
			exit(-1);
	 }
    break;

  case 108:

/* Line 1806 of yacc.c  */
#line 1534 "sintax.y"
    {
		int tipo=-1;
		string params="";
		string variable="";
		string acum="";
		tipo=buscaFuncion((yyvsp[(1) - (4)].sval), params);
		cout<<"Funcion "<<(yyvsp[(1) - (4)].sval)<<"\tTipo:"<<tipo<<"\tParametros registrados: "<<params<<endl;
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
					cout<<"Wrong parameters! on line number:"<<line_num<<endl;
					exit(-1);
			}
			parametros="";

			//Para producir el operando
			variable+=(yyvsp[(1) - (4)].sval);
			variable+="(";
			//El primero es sin la coma
			if(lista_params.dequeue(acum))
				variable+=acum;
			//Los siguientes se les antepone una coma
			while(lista_params.dequeue(acum))
			{
			 variable+=",";
			 variable+=acum;
			}
			variable+=")";

		pTipos.push(tipo);
		pilaO.push(variable);
		}
    break;

  case 109:

/* Line 1806 of yacc.c  */
#line 1579 "sintax.y"
    {
		int tipo;
		tipo=buscaID((yyvsp[(1) - (1)].sval));
		cout<<"Variable read: "<<(yyvsp[(1) - (1)].sval)<<"\tTipo:"<<tipo<<endl;
		if (tipo == -1)
		{
			cout<<"Variable not defined on line:"<<line_num<<endl;
			exit(-1);
		}
		pTipos.push(tipo);
		pilaO.push((yyvsp[(1) - (1)].sval));
	}
    break;

  case 110:

/* Line 1806 of yacc.c  */
#line 1591 "sintax.y"
    {

		pilaO.push((yyvsp[(1) - (1)].sval));
		pTipos.push(2);
		}
    break;

  case 111:

/* Line 1806 of yacc.c  */
#line 1596 "sintax.y"
    {

		pilaO.push((yyvsp[(1) - (1)].sval));
		pTipos.push(3);

		}
    break;

  case 112:

/* Line 1806 of yacc.c  */
#line 1602 "sintax.y"
    {

		pilaO.push((yyvsp[(1) - (1)].sval));
		pTipos.push(1);
		
		}
    break;

  case 113:

/* Line 1806 of yacc.c  */
#line 1608 "sintax.y"
    {
		pilaO.push((yyvsp[(1) - (1)].sval));
		pTipos.push(0);

	}
    break;

  case 114:

/* Line 1806 of yacc.c  */
#line 1613 "sintax.y"
    {
		pilaO.push((yyvsp[(1) - (1)].sval));
		pTipos.push(4);
	}
    break;

  case 115:

/* Line 1806 of yacc.c  */
#line 1621 "sintax.y"
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
    break;

  case 116:

/* Line 1806 of yacc.c  */
#line 1641 "sintax.y"
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
    break;



/* Line 1806 of yacc.c  */
#line 3462 "sintax.tab.c"
      default: break;
    }
  /* User semantic actions sometimes alter yychar, and that requires
     that yytoken be updated with the new translation.  We take the
     approach of translating immediately before every use of yytoken.
     One alternative is translating here after every semantic action,
     but that translation would be missed if the semantic action invokes
     YYABORT, YYACCEPT, or YYERROR immediately after altering yychar or
     if it invokes YYBACKUP.  In the case of YYABORT or YYACCEPT, an
     incorrect destructor might then be invoked immediately.  In the
     case of YYERROR or YYBACKUP, subsequent parser actions might lead
     to an incorrect destructor call or verbose syntax error message
     before the lookahead is translated.  */
  YY_SYMBOL_PRINT ("-> $$ =", yyr1[yyn], &yyval, &yyloc);

  YYPOPSTACK (yylen);
  yylen = 0;
  YY_STACK_PRINT (yyss, yyssp);

  *++yyvsp = yyval;

  /* Now `shift' the result of the reduction.  Determine what state
     that goes to, based on the state we popped back to and the rule
     number reduced by.  */

  yyn = yyr1[yyn];

  yystate = yypgoto[yyn - YYNTOKENS] + *yyssp;
  if (0 <= yystate && yystate <= YYLAST && yycheck[yystate] == *yyssp)
    yystate = yytable[yystate];
  else
    yystate = yydefgoto[yyn - YYNTOKENS];

  goto yynewstate;


/*------------------------------------.
| yyerrlab -- here on detecting error |
`------------------------------------*/
yyerrlab:
  /* Make sure we have latest lookahead translation.  See comments at
     user semantic actions for why this is necessary.  */
  yytoken = yychar == YYEMPTY ? YYEMPTY : YYTRANSLATE (yychar);

  /* If not already recovering from an error, report this error.  */
  if (!yyerrstatus)
    {
      ++yynerrs;
#if ! YYERROR_VERBOSE
      yyerror (YY_("syntax error"));
#else
# define YYSYNTAX_ERROR yysyntax_error (&yymsg_alloc, &yymsg, \
                                        yyssp, yytoken)
      {
        char const *yymsgp = YY_("syntax error");
        int yysyntax_error_status;
        yysyntax_error_status = YYSYNTAX_ERROR;
        if (yysyntax_error_status == 0)
          yymsgp = yymsg;
        else if (yysyntax_error_status == 1)
          {
            if (yymsg != yymsgbuf)
              YYSTACK_FREE (yymsg);
            yymsg = (char *) YYSTACK_ALLOC (yymsg_alloc);
            if (!yymsg)
              {
                yymsg = yymsgbuf;
                yymsg_alloc = sizeof yymsgbuf;
                yysyntax_error_status = 2;
              }
            else
              {
                yysyntax_error_status = YYSYNTAX_ERROR;
                yymsgp = yymsg;
              }
          }
        yyerror (yymsgp);
        if (yysyntax_error_status == 2)
          goto yyexhaustedlab;
      }
# undef YYSYNTAX_ERROR
#endif
    }



  if (yyerrstatus == 3)
    {
      /* If just tried and failed to reuse lookahead token after an
	 error, discard it.  */

      if (yychar <= YYEOF)
	{
	  /* Return failure if at end of input.  */
	  if (yychar == YYEOF)
	    YYABORT;
	}
      else
	{
	  yydestruct ("Error: discarding",
		      yytoken, &yylval);
	  yychar = YYEMPTY;
	}
    }

  /* Else will try to reuse lookahead token after shifting the error
     token.  */
  goto yyerrlab1;


/*---------------------------------------------------.
| yyerrorlab -- error raised explicitly by YYERROR.  |
`---------------------------------------------------*/
yyerrorlab:

  /* Pacify compilers like GCC when the user code never invokes
     YYERROR and the label yyerrorlab therefore never appears in user
     code.  */
  if (/*CONSTCOND*/ 0)
     goto yyerrorlab;

  /* Do not reclaim the symbols of the rule which action triggered
     this YYERROR.  */
  YYPOPSTACK (yylen);
  yylen = 0;
  YY_STACK_PRINT (yyss, yyssp);
  yystate = *yyssp;
  goto yyerrlab1;


/*-------------------------------------------------------------.
| yyerrlab1 -- common code for both syntax error and YYERROR.  |
`-------------------------------------------------------------*/
yyerrlab1:
  yyerrstatus = 3;	/* Each real token shifted decrements this.  */

  for (;;)
    {
      yyn = yypact[yystate];
      if (!yypact_value_is_default (yyn))
	{
	  yyn += YYTERROR;
	  if (0 <= yyn && yyn <= YYLAST && yycheck[yyn] == YYTERROR)
	    {
	      yyn = yytable[yyn];
	      if (0 < yyn)
		break;
	    }
	}

      /* Pop the current state because it cannot handle the error token.  */
      if (yyssp == yyss)
	YYABORT;


      yydestruct ("Error: popping",
		  yystos[yystate], yyvsp);
      YYPOPSTACK (1);
      yystate = *yyssp;
      YY_STACK_PRINT (yyss, yyssp);
    }

  *++yyvsp = yylval;


  /* Shift the error token.  */
  YY_SYMBOL_PRINT ("Shifting", yystos[yyn], yyvsp, yylsp);

  yystate = yyn;
  goto yynewstate;


/*-------------------------------------.
| yyacceptlab -- YYACCEPT comes here.  |
`-------------------------------------*/
yyacceptlab:
  yyresult = 0;
  goto yyreturn;

/*-----------------------------------.
| yyabortlab -- YYABORT comes here.  |
`-----------------------------------*/
yyabortlab:
  yyresult = 1;
  goto yyreturn;

#if !defined(yyoverflow) || YYERROR_VERBOSE
/*-------------------------------------------------.
| yyexhaustedlab -- memory exhaustion comes here.  |
`-------------------------------------------------*/
yyexhaustedlab:
  yyerror (YY_("memory exhausted"));
  yyresult = 2;
  /* Fall through.  */
#endif

yyreturn:
  if (yychar != YYEMPTY)
    {
      /* Make sure we have latest lookahead translation.  See comments at
         user semantic actions for why this is necessary.  */
      yytoken = YYTRANSLATE (yychar);
      yydestruct ("Cleanup: discarding lookahead",
                  yytoken, &yylval);
    }
  /* Do not reclaim the symbols of the rule which action triggered
     this YYABORT or YYACCEPT.  */
  YYPOPSTACK (yylen);
  YY_STACK_PRINT (yyss, yyssp);
  while (yyssp != yyss)
    {
      yydestruct ("Cleanup: popping",
		  yystos[*yyssp], yyvsp);
      YYPOPSTACK (1);
    }
#ifndef yyoverflow
  if (yyss != yyssa)
    YYSTACK_FREE (yyss);
#endif
#if YYERROR_VERBOSE
  if (yymsg != yymsgbuf)
    YYSTACK_FREE (yymsg);
#endif
  /* Make sure YYID is used.  */
  return YYID (yyresult);
}



/* Line 2067 of yacc.c  */
#line 1667 "sintax.y"


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

