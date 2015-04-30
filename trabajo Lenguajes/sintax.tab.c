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
#define YYLAST   266

/* YYNTOKENS -- Number of terminals.  */
#define YYNTOKENS  55
/* YYNNTS -- Number of nonterminals.  */
#define YYNNTS  65
/* YYNRULES -- Number of rules.  */
#define YYNRULES  118
/* YYNRULES -- Number of states.  */
#define YYNSTATES  276

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
     167,   168,   178,   185,   186,   201,   206,   210,   214,   216,
     218,   220,   222,   224,   226,   227,   228,   240,   247,   254,
     261,   268,   275,   282,   289,   296,   303,   304,   309,   310,
     314,   316,   319,   320,   324,   325,   329,   330,   334,   335,
     339,   340,   344,   345,   349,   350,   352,   353,   358,   359,
     364,   366,   367,   372,   373,   378,   382,   384,   389,   391,
     393,   395,   397,   399,   401,   405,   407,   408,   411
};

/* YYRHS -- A `-1'-separated list of the rules' RHS.  */
static const yytype_int8 yyrhs[] =
{
      56,     0,    -1,    -1,    -1,     3,    24,    57,    41,   119,
      65,    59,    58,    69,    -1,    -1,    -1,    13,    24,    42,
      64,    43,    44,    68,   119,    60,    65,    73,    41,   119,
      61,    59,    -1,    -1,    -1,    13,    24,    42,    64,    43,
      44,    14,   119,    62,    65,    76,    41,   119,    63,    59,
      -1,    -1,    64,    45,    24,    44,    68,    -1,    24,    44,
      68,    -1,    -1,     9,   119,    66,    -1,    -1,    66,    67,
      44,    68,    41,   119,    -1,    67,    44,    68,    41,   119,
      -1,    67,    45,    24,    -1,    24,    -1,     4,    -1,     5,
      -1,    11,    -1,    12,    -1,     6,    -1,    -1,    -1,    21,
      70,   119,    79,    22,    71,   119,    -1,    46,   119,    79,
      47,   119,    -1,    -1,    -1,    46,   119,    74,    79,    23,
     102,    75,   119,    47,   119,    -1,    -1,    -1,    46,   119,
      77,    79,    47,    78,   119,    -1,    80,    79,    -1,    -1,
      81,    -1,    83,    -1,    86,    -1,    87,    -1,    92,    -1,
      96,    -1,    95,    -1,    97,    -1,    98,    -1,    -1,    24,
      48,   102,    82,    41,   119,    -1,    -1,    -1,     7,    42,
     102,    43,    84,   119,    72,    85,    99,    -1,    10,    42,
     101,    43,    41,   119,    -1,    -1,    17,    42,    24,    43,
      18,    88,   119,    46,   119,    89,    47,   119,    41,   119,
      -1,    89,    90,    44,    80,    -1,    90,    44,    80,    -1,
      90,    45,    91,    -1,    91,    -1,    36,    -1,    37,    -1,
      38,    -1,    39,    -1,    40,    -1,    -1,    -1,    15,    42,
     102,    43,    93,    16,   119,    72,    41,    94,   119,    -1,
      20,    42,   110,    43,    41,   119,    -1,    19,    42,    24,
      43,    41,   119,    -1,    30,    42,   110,    43,    41,   119,
      -1,    31,    42,   110,    43,    41,   119,    -1,    32,    42,
     110,    43,    41,   119,    -1,    33,    42,   110,    43,    41,
     119,    -1,    34,    42,   110,    43,    41,   119,    -1,    35,
      42,   110,    43,    41,   119,    -1,    24,    42,   118,    43,
      41,   119,    -1,    -1,     8,   100,   119,    72,    -1,    -1,
     101,    45,   102,    -1,   102,    -1,   110,   103,    -1,    -1,
      49,   104,   110,    -1,    -1,    50,   105,   110,    -1,    -1,
      27,   106,   110,    -1,    -1,    28,   107,   110,    -1,    -1,
      25,   108,   110,    -1,    -1,    26,   109,   110,    -1,    -1,
     113,    -1,    -1,   110,    51,   111,   113,    -1,    -1,   110,
      52,   112,   113,    -1,   116,    -1,    -1,   113,    53,   114,
     116,    -1,    -1,   113,    54,   115,   116,    -1,    42,   102,
      43,    -1,   117,    -1,    24,    42,   118,    43,    -1,    24,
      -1,    36,    -1,    37,    -1,    38,    -1,    39,    -1,    40,
      -1,   118,    45,   110,    -1,   110,    -1,    -1,    29,   119,
      -1,    -1
};

/* YYRLINE[YYN] -- source line where rule number YYN was defined.  */
static const yytype_uint16 yyrline[] =
{
       0,   831,   831,   835,   831,   848,   863,   845,   874,   892,
     872,   899,   905,   915,   922,   927,   928,   932,   942,   953,
     958,   965,   966,   967,   968,   969,   973,   980,   973,   989,
     993,   998,   992,  1009,  1014,  1008,  1023,  1024,  1028,  1029,
    1030,  1031,  1032,  1033,  1034,  1035,  1036,  1041,  1040,  1076,
    1099,  1075,  1107,  1114,  1113,  1139,  1140,  1144,  1154,  1167,
    1171,  1175,  1181,  1186,  1199,  1223,  1197,  1232,  1262,  1288,
    1294,  1299,  1304,  1309,  1314,  1324,  1356,  1355,  1363,  1367,
    1389,  1415,  1419,  1419,  1428,  1427,  1437,  1436,  1447,  1446,
    1457,  1456,  1466,  1465,  1474,  1478,  1480,  1479,  1491,  1490,
    1504,  1506,  1505,  1515,  1514,  1525,  1526,  1530,  1575,  1588,
    1593,  1599,  1605,  1610,  1617,  1637,  1657,  1661,  1662
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
  "asignacion", "$@13", "condicion", "$@14", "$@15", "escritura", "casos",
  "$@16", "options", "values", "constants", "ciclo", "$@17", "$@18",
  "sleep", "lectura", "especial", "llamadas", "def_else", "$@19",
  "mensaje", "expresion", "mas_expr", "$@20", "$@21", "$@22", "$@23",
  "$@24", "$@25", "exp", "$@26", "$@27", "termino", "$@28", "$@29",
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
      85,    83,    86,    88,    87,    89,    89,    90,    90,    91,
      91,    91,    91,    91,    93,    94,    92,    95,    96,    97,
      97,    97,    97,    97,    97,    98,   100,    99,    99,   101,
     101,   102,   104,   103,   105,   103,   106,   103,   107,   103,
     108,   103,   109,   103,   103,   110,   111,   110,   112,   110,
     113,   114,   113,   115,   113,   116,   116,   117,   117,   117,
     117,   117,   117,   117,   118,   118,   118,   119,   119
};

/* YYR2[YYN] -- Number of symbols composing right hand side of rule YYN.  */
static const yytype_uint8 yyr2[] =
{
       0,     2,     0,     0,     9,     0,     0,    15,     0,     0,
      15,     0,     5,     3,     0,     3,     0,     6,     5,     3,
       1,     1,     1,     1,     1,     1,     0,     0,     7,     5,
       0,     0,    10,     0,     0,     7,     2,     0,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     0,     6,     0,
       0,     9,     6,     0,    14,     4,     3,     3,     1,     1,
       1,     1,     1,     1,     0,     0,    11,     6,     6,     6,
       6,     6,     6,     6,     6,     6,     0,     4,     0,     3,
       1,     2,     0,     3,     0,     3,     0,     3,     0,     3,
       0,     3,     0,     3,     0,     1,     0,     4,     0,     4,
       1,     0,     4,     0,     4,     3,     1,     4,     1,     1,
       1,     1,     1,     1,     3,     1,     0,     2,     0
};

/* YYDEFACT[STATE-NAME] -- Default reduction number in state STATE-NUM.
   Performed when YYTABLE doesn't specify something else to do.  Zero
   means the default is an error.  */
static const yytype_uint8 yydefact[] =
{
       0,     0,     0,     2,     1,     0,   118,   118,    16,   117,
     118,    11,     0,     0,     3,    20,    15,     0,     0,     0,
       0,     0,     0,    14,    26,     4,     0,    21,    22,    25,
      23,    24,     0,    19,     0,     0,   118,     0,   118,     0,
       0,     0,    37,   118,    18,    13,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,    37,    38,    39,    40,    41,    42,    44,    43,
      45,    46,    17,   118,   118,     0,     0,     0,     0,     0,
       0,     0,   116,     0,     0,     0,     0,     0,     0,     0,
      27,    36,     8,     5,    12,   108,   109,   110,   111,   112,
     113,     0,     0,    94,    95,   100,   106,     0,    80,     0,
       0,     0,     0,   115,     0,    47,     0,     0,     0,     0,
       0,     0,   118,    16,    16,   116,     0,    49,    90,    92,
      86,    88,    82,    84,    96,    98,    81,   101,   103,     0,
       0,    64,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,    28,     0,     0,     0,   105,   118,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     118,    79,     0,    53,   118,   118,   118,   114,   118,   118,
     118,   118,   118,   118,   118,   118,     0,   118,     0,   107,
       0,    91,    93,    87,    89,    83,    85,    97,    99,   102,
     104,    52,   118,   118,    68,    67,    75,    48,    69,    70,
      71,    72,    73,    74,    33,   118,    30,   118,   118,    50,
       0,     0,    37,     9,    37,     6,    37,    78,     0,   118,
       0,    11,     0,    11,     0,    76,    51,    65,     0,    34,
      10,     0,     7,   118,   118,   118,    59,    60,    61,    62,
      63,     0,     0,    58,   118,    31,    29,     0,    66,   118,
       0,     0,     0,    35,   118,    77,     0,     0,    56,    57,
       0,   118,    55,   118,    54,    32
};

/* YYDEFGOTO[NTERM-NUM].  */
static const yytype_int16 yydefgoto[] =
{
      -1,     2,     5,    19,    14,   124,   233,   123,   231,    35,
      11,    16,    17,    32,    25,    36,   122,   219,   188,   224,
     264,   186,   222,   254,    61,    62,    63,   147,    64,   159,
     227,    65,    66,   203,   251,   252,   253,    67,   172,   245,
      68,    69,    70,    71,   236,   244,   107,   102,   136,   164,
     165,   162,   163,   160,   161,   103,   166,   167,   104,   168,
     169,   105,   106,   114,     8
};

/* YYPACT[STATE-NUM] -- Index in YYTABLE of the portion describing
   STATE-NUM.  */
#define YYPACT_NINF -216
static const yytype_int16 yypact[] =
{
       3,   -14,    35,  -216,  -216,    10,    36,    36,    65,  -216,
      36,    57,    82,   111,  -216,  -216,    82,   -23,    75,   120,
      -6,   116,   124,   127,  -216,  -216,   116,  -216,  -216,  -216,
    -216,  -216,   112,  -216,   110,    62,    36,   114,    36,   116,
     113,   132,    13,    36,  -216,  -216,    97,   115,   118,   119,
     122,   128,   140,   142,    -8,   143,   144,   145,   146,   147,
     148,   136,    13,  -216,  -216,  -216,  -216,  -216,  -216,  -216,
    -216,  -216,  -216,    36,    36,   116,    58,    58,    58,   138,
     167,    58,    58,    58,    58,    58,    58,    58,    58,    58,
    -216,  -216,  -216,  -216,  -216,   150,  -216,  -216,  -216,  -216,
    -216,    58,   123,    29,    84,  -216,  -216,    71,  -216,   151,
     154,   155,   -25,    34,    80,  -216,    -2,    20,    25,    32,
      61,    67,    36,    65,    65,    58,   156,  -216,  -216,  -216,
    -216,  -216,  -216,  -216,  -216,  -216,  -216,  -216,  -216,   152,
      58,  -216,   182,   160,   161,   162,    58,   163,   164,   165,
     166,   168,   171,   172,  -216,   169,   170,    81,  -216,    36,
      58,    58,    58,    58,    58,    58,    58,    58,    58,    58,
      36,  -216,   198,  -216,    36,    36,    36,    34,    36,    36,
      36,    36,    36,    36,    36,    36,   176,    36,   177,  -216,
     173,    34,    34,    34,    34,    34,    34,    84,    84,  -216,
    -216,  -216,    36,    36,  -216,  -216,  -216,  -216,  -216,  -216,
    -216,  -216,  -216,  -216,  -216,    36,  -216,    36,    36,  -216,
     173,   174,    13,  -216,    13,  -216,    13,   213,   183,    36,
     178,    57,   200,    57,   179,  -216,  -216,  -216,    93,  -216,
    -216,    58,  -216,    36,    36,    36,  -216,  -216,  -216,  -216,
    -216,    22,    95,  -216,    36,  -216,  -216,   173,  -216,    36,
      98,    13,    93,  -216,    36,  -216,   186,    13,  -216,  -216,
     181,    36,  -216,    36,  -216,  -216
};

/* YYPGOTO[NTERM-NUM].  */
static const yytype_int16 yypgoto[] =
{
    -216,  -216,  -216,  -216,   -97,  -216,  -216,  -216,  -216,  -216,
      21,  -216,   214,   -22,  -216,  -216,  -216,  -215,  -216,  -216,
    -216,  -216,  -216,  -216,   -43,  -168,  -216,  -216,  -216,  -216,
    -216,  -216,  -216,  -216,  -216,   -18,   -33,  -216,  -216,  -216,
    -216,  -216,  -216,  -216,  -216,  -216,  -216,   -76,  -216,  -216,
    -216,  -216,  -216,  -216,  -216,   -73,  -216,  -216,   -20,  -216,
    -216,   -19,  -216,   106,    -7
};

/* YYTABLE[YYPACT[STATE-NUM]].  What to do in state STATE-NUM.  If
   positive, shift that token.  If negative, reduce the rule which
   number is the opposite.  If YYTABLE_NINF, syntax error.  */
#define YYTABLE_NINF -1
static const yytype_uint16 yytable[] =
{
       9,   108,   109,    12,    37,   228,     1,   115,   112,   113,
       3,   116,   117,   118,   119,   120,   121,    45,   144,    91,
      48,    21,    22,    49,    74,   126,   134,   135,    50,    42,
      51,    44,    52,    53,    82,     4,    72,    54,    26,    22,
      83,   148,   265,    55,    56,    57,    58,    59,    60,   134,
     135,     6,   113,    94,   128,   129,   130,   131,   246,   247,
     248,   249,   250,   149,   171,     7,    92,    93,   150,   259,
      13,   134,   135,   177,    10,   151,   134,   135,   132,   133,
     134,   135,    95,   134,   135,   134,   135,   191,   192,   193,
     194,   195,   196,   268,    96,    97,    98,    99,   100,   272,
     101,    27,    28,    29,   152,    40,    15,    41,    30,    31,
     153,    73,   134,   135,   139,   154,   140,    23,   134,   135,
      27,    28,    29,   145,   189,   146,   146,    30,    31,   246,
     247,   248,   249,   250,   240,    18,   242,   137,   138,   261,
     262,    24,   267,   262,   155,   156,   197,   198,    33,   199,
     200,    34,   190,    38,    39,    43,    47,    46,    90,    75,
      76,    77,   110,   201,    78,   255,   127,   204,   205,   206,
      79,   207,   208,   209,   210,   211,   212,   213,   214,   230,
     216,   232,    80,   234,    81,    84,    85,    86,    87,    88,
      89,   111,   125,   170,   141,   220,   221,   142,   143,   158,
     173,   174,   175,   176,   178,   179,   180,   181,   223,   182,
     225,   226,   183,   184,   202,   185,   187,   215,   217,   218,
     229,   235,   238,   241,   237,   239,   243,   271,   273,   269,
      20,   157,     0,   260,     0,     0,   256,   257,   258,     0,
       0,     0,     0,     0,     0,     0,     0,   263,     0,     0,
       0,     0,   266,     0,     0,     0,     0,   270,     0,     0,
       0,     0,     0,     0,   274,     0,   275
};

#define yypact_value_is_default(yystate) \
  ((yystate) == (-216))

#define yytable_value_is_error(yytable_value) \
  YYID (0)

static const yytype_int16 yycheck[] =
{
       7,    77,    78,    10,    26,   220,     3,    83,    81,    82,
      24,    84,    85,    86,    87,    88,    89,    39,    43,    62,
       7,    44,    45,    10,    46,   101,    51,    52,    15,    36,
      17,    38,    19,    20,    42,     0,    43,    24,    44,    45,
      48,    43,   257,    30,    31,    32,    33,    34,    35,    51,
      52,    41,   125,    75,    25,    26,    27,    28,    36,    37,
      38,    39,    40,    43,   140,    29,    73,    74,    43,    47,
      13,    51,    52,   146,     9,    43,    51,    52,    49,    50,
      51,    52,    24,    51,    52,    51,    52,   160,   161,   162,
     163,   164,   165,   261,    36,    37,    38,    39,    40,   267,
      42,     4,     5,     6,    43,    43,    24,    45,    11,    12,
      43,    14,    51,    52,    43,   122,    45,    42,    51,    52,
       4,     5,     6,    43,    43,    45,    45,    11,    12,    36,
      37,    38,    39,    40,   231,    24,   233,    53,    54,    44,
      45,    21,    44,    45,   123,   124,   166,   167,    24,   168,
     169,    24,   159,    41,    44,    41,    24,    44,    22,    44,
      42,    42,    24,   170,    42,   241,    43,   174,   175,   176,
      42,   178,   179,   180,   181,   182,   183,   184,   185,   222,
     187,   224,    42,   226,    42,    42,    42,    42,    42,    42,
      42,    24,    42,    41,    43,   202,   203,    43,    43,    43,
      18,    41,    41,    41,    41,    41,    41,    41,   215,    41,
     217,   218,    41,    41,    16,    46,    46,    41,    41,    46,
      46,     8,   229,    23,    41,    47,    47,    41,    47,   262,
      16,   125,    -1,   251,    -1,    -1,   243,   244,   245,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   254,    -1,    -1,
      -1,    -1,   259,    -1,    -1,    -1,    -1,   264,    -1,    -1,
      -1,    -1,    -1,    -1,   271,    -1,   273
};

/* YYSTOS[STATE-NUM] -- The (internal number of the) accessing
   symbol of state STATE-NUM.  */
static const yytype_uint8 yystos[] =
{
       0,     3,    56,    24,     0,    57,    41,    29,   119,   119,
       9,    65,   119,    13,    59,    24,    66,    67,    24,    58,
      67,    44,    45,    42,    21,    69,    44,     4,     5,     6,
      11,    12,    68,    24,    24,    64,    70,    68,    41,    44,
      43,    45,   119,    41,   119,    68,    44,    24,     7,    10,
      15,    17,    19,    20,    24,    30,    31,    32,    33,    34,
      35,    79,    80,    81,    83,    86,    87,    92,    95,    96,
      97,    98,   119,    14,    68,    44,    42,    42,    42,    42,
      42,    42,    42,    48,    42,    42,    42,    42,    42,    42,
      22,    79,   119,   119,    68,    24,    36,    37,    38,    39,
      40,    42,   102,   110,   113,   116,   117,   101,   102,   102,
      24,    24,   110,   110,   118,   102,   110,   110,   110,   110,
     110,   110,    71,    62,    60,    42,   102,    43,    25,    26,
      27,    28,    49,    50,    51,    52,   103,    53,    54,    43,
      45,    43,    43,    43,    43,    43,    45,    82,    43,    43,
      43,    43,    43,    43,   119,    65,    65,   118,    43,    84,
     108,   109,   106,   107,   104,   105,   111,   112,   114,   115,
      41,   102,    93,    18,    41,    41,    41,   110,    41,    41,
      41,    41,    41,    41,    41,    46,    76,    46,    73,    43,
     119,   110,   110,   110,   110,   110,   110,   113,   113,   116,
     116,   119,    16,    88,   119,   119,   119,   119,   119,   119,
     119,   119,   119,   119,   119,    41,   119,    41,    46,    72,
     119,   119,    77,   119,    74,   119,   119,    85,    72,    46,
      79,    63,    79,    61,    79,     8,    99,    41,   119,    47,
      59,    23,    59,    47,   100,    94,    36,    37,    38,    39,
      40,    89,    90,    91,    78,   102,   119,   119,   119,    47,
      90,    44,    45,   119,    75,    72,   119,    44,    80,    91,
     119,    41,    80,    47,   119,   119
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
			cout<<"Incompatible types! expected condition line_num"<<line_num<<endl;
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

  case 53:

/* Line 1806 of yacc.c  */
#line 1114 "sintax.y"
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

  case 54:

/* Line 1806 of yacc.c  */
#line 1132 "sintax.y"
    {
	myQuadStructure<<"ENDCASE"<<endl;
	}
    break;

  case 57:

/* Line 1806 of yacc.c  */
#line 1145 "sintax.y"
    {
	if (tipoCase!=(yyvsp[(3) - (3)].ival))
	{
		cout<<"Incompatible types! on line: "<<line_num<<endl;
		exit(-1);
	}
	myQuadStructure<<"EXTRAOPTION\t"<<optionStr<<endl;
}
    break;

  case 58:

/* Line 1806 of yacc.c  */
#line 1155 "sintax.y"
    {
	if (tipoCase!=(yyvsp[(1) - (1)].ival))
	{
		cout<<"Incompatible types! on line: "<<line_num<<endl;
		exit(-1);
	}
	myQuadStructure<<"OPTION\t"<<optionStr<<endl;
}
    break;

  case 59:

/* Line 1806 of yacc.c  */
#line 1167 "sintax.y"
    {
		(yyval.ival)=2;
		optionStr=(yyvsp[(1) - (1)].sval);
		}
    break;

  case 60:

/* Line 1806 of yacc.c  */
#line 1171 "sintax.y"
    {
		(yyval.ival)=3;
		optionStr=(yyvsp[(1) - (1)].sval);
		}
    break;

  case 61:

/* Line 1806 of yacc.c  */
#line 1175 "sintax.y"
    {

		(yyval.ival)=1;
		optionStr=(yyvsp[(1) - (1)].sval);
		
		}
    break;

  case 62:

/* Line 1806 of yacc.c  */
#line 1181 "sintax.y"
    {
		(yyval.ival)=0;
		optionStr=(yyvsp[(1) - (1)].sval);

	}
    break;

  case 63:

/* Line 1806 of yacc.c  */
#line 1186 "sintax.y"
    {
		(yyval.ival)=4;
		optionStr=(yyvsp[(1) - (1)].sval);
	}
    break;

  case 64:

/* Line 1806 of yacc.c  */
#line 1199 "sintax.y"
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

  case 65:

/* Line 1806 of yacc.c  */
#line 1223 "sintax.y"
    {
	 	myQuadStructure<<"ENDWHILE"<<endl;
	 }
    break;

  case 67:

/* Line 1806 of yacc.c  */
#line 1233 "sintax.y"
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

  case 68:

/* Line 1806 of yacc.c  */
#line 1264 "sintax.y"
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

  case 69:

/* Line 1806 of yacc.c  */
#line 1289 "sintax.y"
    {
		if (generaEspecial(0)==-1)
			exit(-1);
	}
    break;

  case 70:

/* Line 1806 of yacc.c  */
#line 1295 "sintax.y"
    {
		if (generaEspecial(1)==-1)
			exit(-1);
	}
    break;

  case 71:

/* Line 1806 of yacc.c  */
#line 1300 "sintax.y"
    {
		if (generaEspecial(2)==-1)
			exit(-1);
	}
    break;

  case 72:

/* Line 1806 of yacc.c  */
#line 1305 "sintax.y"
    {
		if (generaEspecial(3)==-1)
			exit(-1);
	}
    break;

  case 73:

/* Line 1806 of yacc.c  */
#line 1310 "sintax.y"
    {
		if (generaEspecial(4)==-1)
			exit(-1);
	}
    break;

  case 74:

/* Line 1806 of yacc.c  */
#line 1315 "sintax.y"
    {
		if (generaEspecial(5)==-1)
			exit(-1);
	}
    break;

  case 75:

/* Line 1806 of yacc.c  */
#line 1325 "sintax.y"
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
					cout<<"Parametros no coinciden"<<endl;
					exit(-1);
				}
		}
	}
    break;

  case 76:

/* Line 1806 of yacc.c  */
#line 1356 "sintax.y"
    {
		myQuadStructure<<"ELSE"<<endl;
	}
    break;

  case 77:

/* Line 1806 of yacc.c  */
#line 1360 "sintax.y"
    {
		myQuadStructure<<"ENDELSE"<<endl;	
	}
    break;

  case 79:

/* Line 1806 of yacc.c  */
#line 1368 "sintax.y"
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

  case 80:

/* Line 1806 of yacc.c  */
#line 1390 "sintax.y"
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

  case 82:

/* Line 1806 of yacc.c  */
#line 1419 "sintax.y"
    {
			pilaOper.push(">");
		}
    break;

  case 83:

/* Line 1806 of yacc.c  */
#line 1423 "sintax.y"
    {
			if(generaExpresion()==-1)
			exit(-1);
		}
    break;

  case 84:

/* Line 1806 of yacc.c  */
#line 1428 "sintax.y"
    {
			pilaOper.push("<");
		}
    break;

  case 85:

/* Line 1806 of yacc.c  */
#line 1432 "sintax.y"
    {
	 		if(generaExpresion()==-1)
			exit(-1);	
	 	}
    break;

  case 86:

/* Line 1806 of yacc.c  */
#line 1437 "sintax.y"
    {
			pilaOper.push("<=");
		}
    break;

  case 87:

/* Line 1806 of yacc.c  */
#line 1442 "sintax.y"
    {
	 		if(generaExpresion()==-1)
			exit(-1);	
	 	}
    break;

  case 88:

/* Line 1806 of yacc.c  */
#line 1447 "sintax.y"
    {
			pilaOper.push(">=");
		}
    break;

  case 89:

/* Line 1806 of yacc.c  */
#line 1451 "sintax.y"
    {
	 		if(generaExpresion()==-1)
			exit(-1);	
	 	}
    break;

  case 90:

/* Line 1806 of yacc.c  */
#line 1457 "sintax.y"
    {
			pilaOper.push("<>");
		}
    break;

  case 91:

/* Line 1806 of yacc.c  */
#line 1461 "sintax.y"
    {
	 		if(generaExpresion()==-1)
			exit(-1);	
	 	}
    break;

  case 92:

/* Line 1806 of yacc.c  */
#line 1466 "sintax.y"
    {
			pilaOper.push("==");
		}
    break;

  case 93:

/* Line 1806 of yacc.c  */
#line 1470 "sintax.y"
    {
			if(generaExpresion()==-1)
			exit(-1);	
		}
    break;

  case 96:

/* Line 1806 of yacc.c  */
#line 1480 "sintax.y"
    {
		pilaOper.push("+");
		
	}
    break;

  case 97:

/* Line 1806 of yacc.c  */
#line 1486 "sintax.y"
    {
		if(generaExpresion()==-1)
			exit(-1);
	 }
    break;

  case 98:

/* Line 1806 of yacc.c  */
#line 1491 "sintax.y"
    {
		pilaOper.push("-");
	}
    break;

  case 99:

/* Line 1806 of yacc.c  */
#line 1495 "sintax.y"
    {
		if(generaExpresion()==-1)
			exit(-1);
	 }
    break;

  case 101:

/* Line 1806 of yacc.c  */
#line 1506 "sintax.y"
    {
		pilaOper.push("*");
	}
    break;

  case 102:

/* Line 1806 of yacc.c  */
#line 1510 "sintax.y"
    {
		if(generaExpresion()==-1)
			exit(-1);
	}
    break;

  case 103:

/* Line 1806 of yacc.c  */
#line 1515 "sintax.y"
    {
	pilaOper.push("/");
	}
    break;

  case 104:

/* Line 1806 of yacc.c  */
#line 1519 "sintax.y"
    {
		if(generaExpresion()==-1)
			exit(-1);
	 }
    break;

  case 107:

/* Line 1806 of yacc.c  */
#line 1531 "sintax.y"
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
				cout<<"Parametros no coinciden"<<endl;
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

  case 108:

/* Line 1806 of yacc.c  */
#line 1576 "sintax.y"
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

  case 109:

/* Line 1806 of yacc.c  */
#line 1588 "sintax.y"
    {

		pilaO.push((yyvsp[(1) - (1)].sval));
		pTipos.push(2);
		}
    break;

  case 110:

/* Line 1806 of yacc.c  */
#line 1593 "sintax.y"
    {

		pilaO.push((yyvsp[(1) - (1)].sval));
		pTipos.push(3);

		}
    break;

  case 111:

/* Line 1806 of yacc.c  */
#line 1599 "sintax.y"
    {

		pilaO.push((yyvsp[(1) - (1)].sval));
		pTipos.push(1);
		
		}
    break;

  case 112:

/* Line 1806 of yacc.c  */
#line 1605 "sintax.y"
    {
		pilaO.push((yyvsp[(1) - (1)].sval));
		pTipos.push(0);

	}
    break;

  case 113:

/* Line 1806 of yacc.c  */
#line 1610 "sintax.y"
    {
		pilaO.push((yyvsp[(1) - (1)].sval));
		pTipos.push(4);
	}
    break;

  case 114:

/* Line 1806 of yacc.c  */
#line 1618 "sintax.y"
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

  case 115:

/* Line 1806 of yacc.c  */
#line 1638 "sintax.y"
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
#line 3453 "sintax.tab.c"
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
#line 1664 "sintax.y"


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

