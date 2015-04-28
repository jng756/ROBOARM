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




/* Line 268 of yacc.c  */
#line 782 "sintax.tab.c"

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
#line 716 "sintax.y"

	int ival;
	float fval;
	char *sval;
	char cval;
	bool bval;



/* Line 293 of yacc.c  */
#line 868 "sintax.tab.c"
} YYSTYPE;
# define YYSTYPE_IS_TRIVIAL 1
# define yystype YYSTYPE /* obsolescent; will be withdrawn */
# define YYSTYPE_IS_DECLARED 1
#endif


/* Copy the second part of user declarations.  */


/* Line 343 of yacc.c  */
#line 880 "sintax.tab.c"

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
#define YYLAST   265

/* YYNTOKENS -- Number of terminals.  */
#define YYNTOKENS  55
/* YYNNTS -- Number of nonterminals.  */
#define YYNNTS  64
/* YYNRULES -- Number of rules.  */
#define YYNRULES  116
/* YYNRULES -- Number of states.  */
#define YYNSTATES  275

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
      35,    51,    52,    58,    62,    66,    67,    74,    80,    84,
      86,    88,    90,    92,    94,    96,    97,    98,   106,   112,
     113,   114,   125,   126,   127,   135,   138,   139,   141,   143,
     145,   147,   149,   151,   153,   155,   157,   158,   165,   166,
     175,   182,   183,   198,   203,   207,   211,   213,   215,   217,
     219,   221,   223,   224,   225,   237,   244,   251,   258,   265,
     272,   279,   286,   293,   300,   301,   306,   307,   311,   313,
     316,   317,   321,   322,   326,   327,   331,   332,   336,   337,
     341,   342,   346,   347,   349,   350,   355,   356,   361,   363,
     364,   369,   370,   375,   379,   381,   386,   388,   390,   392,
     394,   396,   398,   402,   404,   405,   408
};

/* YYRHS -- A `-1'-separated list of the rules' RHS.  */
static const yytype_int8 yyrhs[] =
{
      56,     0,    -1,    -1,    -1,     3,    24,    57,    41,   118,
      65,    59,    58,    69,    -1,    -1,    -1,    13,    24,    42,
      64,    43,    44,    68,   118,    60,    65,    73,    41,   118,
      61,    59,    -1,    -1,    -1,    13,    24,    42,    64,    43,
      44,    14,   118,    62,    65,    76,    41,   118,    63,    59,
      -1,    -1,    64,    45,    24,    44,    68,    -1,    24,    44,
      68,    -1,     9,   118,    66,    -1,    -1,    66,    67,    44,
      68,    41,   118,    -1,    67,    44,    68,    41,   118,    -1,
      67,    45,    24,    -1,    24,    -1,     4,    -1,     5,    -1,
      11,    -1,    12,    -1,     6,    -1,    -1,    -1,    21,    70,
     118,    79,    22,    71,   118,    -1,    46,   118,    79,    47,
     118,    -1,    -1,    -1,    46,   118,    74,    79,    23,   101,
      75,   118,    47,   118,    -1,    -1,    -1,    46,   118,    77,
      79,    47,    78,   118,    -1,    80,    79,    -1,    -1,    81,
      -1,    83,    -1,    85,    -1,    86,    -1,    91,    -1,    95,
      -1,    94,    -1,    96,    -1,    97,    -1,    -1,    24,    48,
     101,    82,    41,   118,    -1,    -1,     7,    42,   101,    43,
      84,   118,    72,    98,    -1,    10,    42,   100,    43,    41,
     118,    -1,    -1,    17,    42,    24,    43,    18,    87,   118,
      46,   118,    88,    47,   118,    41,   118,    -1,    88,    89,
      44,    80,    -1,    89,    44,    80,    -1,    89,    45,    90,
      -1,    90,    -1,    36,    -1,    37,    -1,    38,    -1,    39,
      -1,    40,    -1,    -1,    -1,    15,    42,   101,    43,    92,
      16,   118,    72,    41,    93,   118,    -1,    20,    42,   109,
      43,    41,   118,    -1,    19,    42,    24,    43,    41,   118,
      -1,    30,    42,   109,    43,    41,   118,    -1,    31,    42,
     109,    43,    41,   118,    -1,    32,    42,   109,    43,    41,
     118,    -1,    33,    42,   109,    43,    41,   118,    -1,    34,
      42,   109,    43,    41,   118,    -1,    35,    42,   109,    43,
      41,   118,    -1,    24,    42,   117,    43,    41,   118,    -1,
      -1,     8,    99,   118,    72,    -1,    -1,   100,    45,   101,
      -1,   101,    -1,   109,   102,    -1,    -1,    49,   103,   109,
      -1,    -1,    50,   104,   109,    -1,    -1,    27,   105,   109,
      -1,    -1,    28,   106,   109,    -1,    -1,    25,   107,   109,
      -1,    -1,    26,   108,   109,    -1,    -1,   112,    -1,    -1,
     109,    51,   110,   112,    -1,    -1,   109,    52,   111,   112,
      -1,   115,    -1,    -1,   112,    53,   113,   115,    -1,    -1,
     112,    54,   114,   115,    -1,    42,   101,    43,    -1,   116,
      -1,    24,    42,   117,    43,    -1,    24,    -1,    36,    -1,
      37,    -1,    38,    -1,    39,    -1,    40,    -1,   117,    45,
     109,    -1,   109,    -1,    -1,    29,   118,    -1,    -1
};

/* YYRLINE[YYN] -- source line where rule number YYN was defined.  */
static const yytype_uint16 yyrline[] =
{
       0,   786,   786,   789,   786,   801,   810,   798,   820,   829,
     819,   835,   840,   850,   861,   862,   866,   876,   887,   892,
     899,   900,   901,   902,   903,   907,   912,   907,   920,   924,
     930,   923,   956,   960,   955,   968,   969,   973,   974,   975,
     976,   977,   978,   979,   980,   981,   986,   985,  1021,  1020,
    1050,  1056,  1055,  1081,  1082,  1086,  1095,  1107,  1111,  1115,
    1121,  1126,  1139,  1163,  1137,  1172,  1202,  1228,  1234,  1239,
    1244,  1249,  1254,  1264,  1296,  1295,  1303,  1307,  1327,  1351,
    1355,  1355,  1364,  1363,  1373,  1372,  1383,  1382,  1393,  1392,
    1402,  1401,  1410,  1414,  1416,  1415,  1427,  1426,  1440,  1442,
    1441,  1451,  1450,  1461,  1462,  1466,  1510,  1523,  1528,  1534,
    1540,  1545,  1552,  1572,  1592,  1596,  1597
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
  "asignacion", "$@13", "condicion", "$@14", "escritura", "casos", "$@15",
  "options", "values", "constants", "ciclo", "$@16", "$@17", "sleep",
  "lectura", "especial", "llamadas", "def_else", "$@18", "mensaje",
  "expresion", "mas_expr", "$@19", "$@20", "$@21", "$@22", "$@23", "$@24",
  "exp", "$@25", "$@26", "termino", "$@27", "$@28", "factor", "varcte",
  "params", "endl", 0
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
      59,    59,    64,    64,    65,    65,    66,    66,    67,    67,
      68,    68,    68,    68,    68,    70,    71,    69,    72,    74,
      75,    73,    77,    78,    76,    79,    79,    80,    80,    80,
      80,    80,    80,    80,    80,    80,    82,    81,    84,    83,
      85,    87,    86,    88,    88,    89,    89,    90,    90,    90,
      90,    90,    92,    93,    91,    94,    95,    96,    96,    96,
      96,    96,    96,    97,    99,    98,    98,   100,   100,   101,
     103,   102,   104,   102,   105,   102,   106,   102,   107,   102,
     108,   102,   102,   109,   110,   109,   111,   109,   112,   113,
     112,   114,   112,   115,   115,   116,   116,   116,   116,   116,
     116,   116,   117,   117,   117,   118,   118
};

/* YYR2[YYN] -- Number of symbols composing right hand side of rule YYN.  */
static const yytype_uint8 yyr2[] =
{
       0,     2,     0,     0,     9,     0,     0,    15,     0,     0,
      15,     0,     5,     3,     3,     0,     6,     5,     3,     1,
       1,     1,     1,     1,     1,     0,     0,     7,     5,     0,
       0,    10,     0,     0,     7,     2,     0,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     0,     6,     0,     8,
       6,     0,    14,     4,     3,     3,     1,     1,     1,     1,
       1,     1,     0,     0,    11,     6,     6,     6,     6,     6,
       6,     6,     6,     6,     0,     4,     0,     3,     1,     2,
       0,     3,     0,     3,     0,     3,     0,     3,     0,     3,
       0,     3,     0,     1,     0,     4,     0,     4,     1,     0,
       4,     0,     4,     3,     1,     4,     1,     1,     1,     1,
       1,     1,     3,     1,     0,     2,     0
};

/* YYDEFACT[STATE-NAME] -- Default reduction number in state STATE-NUM.
   Performed when YYTABLE doesn't specify something else to do.  Zero
   means the default is an error.  */
static const yytype_uint8 yydefact[] =
{
       0,     0,     0,     2,     1,     0,   116,   116,    15,   115,
     116,    11,     0,     0,     3,    19,    14,     0,     0,     0,
       0,     0,     0,     0,    25,     4,     0,    20,    21,    24,
      22,    23,     0,    18,     0,     0,   116,     0,   116,     0,
       0,     0,    36,   116,    17,    13,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,    36,    37,    38,    39,    40,    41,    43,    42,
      44,    45,    16,   116,   116,     0,     0,     0,     0,     0,
       0,     0,   114,     0,     0,     0,     0,     0,     0,     0,
      26,    35,     8,     5,    12,   106,   107,   108,   109,   110,
     111,     0,     0,    92,    93,    98,   104,     0,    78,     0,
       0,     0,     0,   113,     0,    46,     0,     0,     0,     0,
       0,     0,   116,    15,    15,   114,     0,    48,    88,    90,
      84,    86,    80,    82,    94,    96,    79,    99,   101,     0,
       0,    62,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,    27,     0,     0,     0,   103,   116,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     116,    77,     0,    51,   116,   116,   116,   112,   116,   116,
     116,   116,   116,   116,   116,   116,     0,   116,     0,   105,
       0,    89,    91,    85,    87,    81,    83,    95,    97,   100,
     102,    50,   116,   116,    66,    65,    73,    47,    67,    68,
      69,    70,    71,    72,    32,   116,    29,   116,   116,    76,
       0,     0,    36,     9,    36,     6,    36,    74,    49,     0,
     116,     0,    11,     0,    11,     0,   116,    63,     0,    33,
      10,     0,     7,   116,     0,   116,    57,    58,    59,    60,
      61,     0,     0,    56,   116,    30,    28,    75,    64,   116,
       0,     0,     0,    34,   116,     0,     0,    54,    55,     0,
     116,    53,   116,    52,    31
};

/* YYDEFGOTO[NTERM-NUM].  */
static const yytype_int16 yydefgoto[] =
{
      -1,     2,     5,    19,    14,   124,   234,   123,   232,    35,
      11,    16,    17,    32,    25,    36,   122,   219,   188,   224,
     264,   186,   222,   254,    61,    62,    63,   147,    64,   159,
      65,    66,   203,   251,   252,   253,    67,   172,   245,    68,
      69,    70,    71,   228,   236,   107,   102,   136,   164,   165,
     162,   163,   160,   161,   103,   166,   167,   104,   168,   169,
     105,   106,   114,     8
};

/* YYPACT[STATE-NUM] -- Index in YYTABLE of the portion describing
   STATE-NUM.  */
#define YYPACT_NINF -211
static const yytype_int16 yypact[] =
{
      48,    11,   105,  -211,  -211,    90,   115,   115,   132,  -211,
     115,   133,   121,   123,  -211,  -211,   121,   -39,   106,   128,
     -23,   112,   126,   127,  -211,  -211,   112,  -211,  -211,  -211,
    -211,  -211,   113,  -211,   109,    61,   115,   114,   115,   112,
     116,   134,    13,   115,  -211,  -211,    57,   117,   120,   122,
     124,   140,   142,   143,    28,   144,   145,   146,   147,   148,
     149,   135,    13,  -211,  -211,  -211,  -211,  -211,  -211,  -211,
    -211,  -211,  -211,   115,   115,   112,    58,    58,    58,   168,
     169,    58,    58,    58,    58,    58,    58,    58,    58,    58,
    -211,  -211,  -211,  -211,  -211,   152,  -211,  -211,  -211,  -211,
    -211,    58,   154,    29,    80,  -211,  -211,    76,  -211,   155,
     156,   157,   -25,    62,    77,  -211,     7,    32,    34,    50,
      56,    60,   115,   132,   132,    58,   158,  -211,  -211,  -211,
    -211,  -211,  -211,  -211,  -211,  -211,  -211,  -211,  -211,   118,
      58,  -211,   138,   129,   161,   162,    58,   163,   164,   165,
     166,   171,   172,   173,  -211,   170,   174,    87,  -211,   115,
      58,    58,    58,    58,    58,    58,    58,    58,    58,    58,
     115,  -211,   193,  -211,   115,   115,   115,    62,   115,   115,
     115,   115,   115,   115,   115,   115,   176,   115,   177,  -211,
     175,    62,    62,    62,    62,    62,    62,    80,    80,  -211,
    -211,  -211,   115,   115,  -211,  -211,  -211,  -211,  -211,  -211,
    -211,  -211,  -211,  -211,  -211,   115,  -211,   115,   115,   207,
     175,   178,    13,  -211,    13,  -211,    13,  -211,  -211,   181,
     115,   179,   133,   196,   133,   180,   115,  -211,    89,  -211,
    -211,    58,  -211,   115,   175,   115,  -211,  -211,  -211,  -211,
    -211,     2,    65,  -211,   115,  -211,  -211,  -211,  -211,   115,
      91,    13,    89,  -211,   115,   184,    13,  -211,  -211,   183,
     115,  -211,   115,  -211,  -211
};

/* YYPGOTO[NTERM-NUM].  */
static const yytype_int16 yypgoto[] =
{
    -211,  -211,  -211,  -211,  -160,  -211,  -211,  -211,  -211,  -211,
      14,  -211,   212,   -22,  -211,  -211,  -211,  -210,  -211,  -211,
    -211,  -211,  -211,  -211,   -43,  -201,  -211,  -211,  -211,  -211,
    -211,  -211,  -211,  -211,   -20,   -30,  -211,  -211,  -211,  -211,
    -211,  -211,  -211,  -211,  -211,  -211,   -76,  -211,  -211,  -211,
    -211,  -211,  -211,  -211,   -73,  -211,  -211,   -27,  -211,  -211,
     -26,  -211,   108,    -7
};

/* YYTABLE[YYPACT[STATE-NUM]].  What to do in state STATE-NUM.  If
   positive, shift that token.  If negative, reduce the rule which
   number is the opposite.  If YYTABLE_NINF, syntax error.  */
#define YYTABLE_NINF -1
static const yytype_uint16 yytable[] =
{
       9,   108,   109,    12,    37,    21,    22,   115,   112,   113,
     229,   116,   117,   118,   119,   120,   121,    45,   144,    91,
      48,    26,    22,    49,    74,   126,   134,   135,    50,    42,
      51,    44,    52,    53,   257,     3,    72,    54,   246,   247,
     248,   249,   250,    55,    56,    57,    58,    59,    60,   259,
     148,     1,   113,    94,   128,   129,   130,   131,   134,   135,
     267,    27,    28,    29,   171,   271,    92,    93,    30,    31,
      82,    73,   240,   177,   242,   149,    83,   150,   132,   133,
     134,   135,    95,   134,   135,   134,   135,   191,   192,   193,
     194,   195,   196,   151,    96,    97,    98,    99,   100,   152,
     101,   134,   135,   153,    40,     4,    41,   134,   135,   261,
     262,   134,   135,   134,   135,   154,    27,    28,    29,   139,
     145,   140,   146,    30,    31,   246,   247,   248,   249,   250,
     189,     6,   146,   137,   138,   266,   262,   155,   156,   197,
     198,    10,   199,   200,     7,    15,    13,    18,    23,    24,
      33,    34,   190,    39,    38,    43,   173,    90,    47,   170,
      46,    75,    76,   201,    77,   255,    78,   204,   205,   206,
     174,   207,   208,   209,   210,   211,   212,   213,   214,   231,
     216,   233,    79,   235,    80,    81,    84,    85,    86,    87,
      88,    89,   110,   111,   125,   220,   221,   127,   141,   142,
     143,   158,   175,   176,   178,   179,   180,   181,   223,   202,
     225,   226,   182,   183,   184,   227,   185,   215,   217,   241,
     187,   218,   237,   238,   230,   270,   239,   243,    20,   244,
     272,   260,   268,   157,     0,     0,   256,     0,   258,     0,
       0,     0,     0,     0,     0,     0,     0,   263,     0,     0,
       0,     0,   265,     0,     0,     0,     0,   269,     0,     0,
       0,     0,     0,   273,     0,   274
};

#define yypact_value_is_default(yystate) \
  ((yystate) == (-211))

#define yytable_value_is_error(yytable_value) \
  YYID (0)

static const yytype_int16 yycheck[] =
{
       7,    77,    78,    10,    26,    44,    45,    83,    81,    82,
     220,    84,    85,    86,    87,    88,    89,    39,    43,    62,
       7,    44,    45,    10,    46,   101,    51,    52,    15,    36,
      17,    38,    19,    20,   244,    24,    43,    24,    36,    37,
      38,    39,    40,    30,    31,    32,    33,    34,    35,    47,
      43,     3,   125,    75,    25,    26,    27,    28,    51,    52,
     261,     4,     5,     6,   140,   266,    73,    74,    11,    12,
      42,    14,   232,   146,   234,    43,    48,    43,    49,    50,
      51,    52,    24,    51,    52,    51,    52,   160,   161,   162,
     163,   164,   165,    43,    36,    37,    38,    39,    40,    43,
      42,    51,    52,    43,    43,     0,    45,    51,    52,    44,
      45,    51,    52,    51,    52,   122,     4,     5,     6,    43,
      43,    45,    45,    11,    12,    36,    37,    38,    39,    40,
      43,    41,    45,    53,    54,    44,    45,   123,   124,   166,
     167,     9,   168,   169,    29,    24,    13,    24,    42,    21,
      24,    24,   159,    44,    41,    41,    18,    22,    24,    41,
      44,    44,    42,   170,    42,   241,    42,   174,   175,   176,
      41,   178,   179,   180,   181,   182,   183,   184,   185,   222,
     187,   224,    42,   226,    42,    42,    42,    42,    42,    42,
      42,    42,    24,    24,    42,   202,   203,    43,    43,    43,
      43,    43,    41,    41,    41,    41,    41,    41,   215,    16,
     217,   218,    41,    41,    41,     8,    46,    41,    41,    23,
      46,    46,    41,   230,    46,    41,    47,    47,    16,   236,
      47,   251,   262,   125,    -1,    -1,   243,    -1,   245,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   254,    -1,    -1,
      -1,    -1,   259,    -1,    -1,    -1,    -1,   264,    -1,    -1,
      -1,    -1,    -1,   270,    -1,   272
};

/* YYSTOS[STATE-NUM] -- The (internal number of the) accessing
   symbol of state STATE-NUM.  */
static const yytype_uint8 yystos[] =
{
       0,     3,    56,    24,     0,    57,    41,    29,   118,   118,
       9,    65,   118,    13,    59,    24,    66,    67,    24,    58,
      67,    44,    45,    42,    21,    69,    44,     4,     5,     6,
      11,    12,    68,    24,    24,    64,    70,    68,    41,    44,
      43,    45,   118,    41,   118,    68,    44,    24,     7,    10,
      15,    17,    19,    20,    24,    30,    31,    32,    33,    34,
      35,    79,    80,    81,    83,    85,    86,    91,    94,    95,
      96,    97,   118,    14,    68,    44,    42,    42,    42,    42,
      42,    42,    42,    48,    42,    42,    42,    42,    42,    42,
      22,    79,   118,   118,    68,    24,    36,    37,    38,    39,
      40,    42,   101,   109,   112,   115,   116,   100,   101,   101,
      24,    24,   109,   109,   117,   101,   109,   109,   109,   109,
     109,   109,    71,    62,    60,    42,   101,    43,    25,    26,
      27,    28,    49,    50,    51,    52,   102,    53,    54,    43,
      45,    43,    43,    43,    43,    43,    45,    82,    43,    43,
      43,    43,    43,    43,   118,    65,    65,   117,    43,    84,
     107,   108,   105,   106,   103,   104,   110,   111,   113,   114,
      41,   101,    92,    18,    41,    41,    41,   109,    41,    41,
      41,    41,    41,    41,    41,    46,    76,    46,    73,    43,
     118,   109,   109,   109,   109,   109,   109,   112,   112,   115,
     115,   118,    16,    87,   118,   118,   118,   118,   118,   118,
     118,   118,   118,   118,   118,    41,   118,    41,    46,    72,
     118,   118,    77,   118,    74,   118,   118,     8,    98,    72,
      46,    79,    63,    79,    61,    79,    99,    41,   118,    47,
      59,    23,    59,    47,   118,    93,    36,    37,    38,    39,
      40,    88,    89,    90,    78,   101,   118,    72,   118,    47,
      89,    44,    45,   118,    75,   118,    44,    80,    90,   118,
      41,    80,    47,   118,   118
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
#line 786 "sintax.y"
    {
		pragma.setNombre((yyvsp[(2) - (2)].sval));
	}
    break;

  case 3:

/* Line 1806 of yacc.c  */
#line 789 "sintax.y"
    {
		localFlag=false;
	}
    break;

  case 4:

/* Line 1806 of yacc.c  */
#line 794 "sintax.y"
    {  cout << "Fin del analisis del programa " << endl; }
    break;

  case 5:

/* Line 1806 of yacc.c  */
#line 801 "sintax.y"
    {
			localFlag=true;
			guardaFuncion(1,(yyvsp[(2) - (8)].sval),(yyvsp[(7) - (8)].ival), parametros);
			tipoFunction=(yyvsp[(7) - (8)].ival);
			//Vaciar params
			parametros="";
		}
    break;

  case 6:

/* Line 1806 of yacc.c  */
#line 810 "sintax.y"
    {	
			//vaciar la estructura de variables para iniciar una nueva funcion
			entry.setVarTable(new HashMap<varEntry>);
			
	
		}
    break;

  case 8:

/* Line 1806 of yacc.c  */
#line 820 "sintax.y"
    {
		localFlag=true;
		guardaFuncion(2,(yyvsp[(2) - (8)].sval),5, parametros);
		tipoFunction=5;
			//Vaciar params
			parametros="";
		}
    break;

  case 9:

/* Line 1806 of yacc.c  */
#line 829 "sintax.y"
    {	
			//vaciar la estructura de variables para iniciar una nueva funcion
			entry.setVarTable(new HashMap<varEntry>);
		
		}
    break;

  case 12:

/* Line 1806 of yacc.c  */
#line 841 "sintax.y"
    {

		if (!guardaParametros((yyvsp[(3) - (5)].sval),(yyvsp[(5) - (5)].ival)))
			{
				exit(-1);
			}
		
		
		}
    break;

  case 13:

/* Line 1806 of yacc.c  */
#line 851 "sintax.y"
    {
		if (!guardaParametros((yyvsp[(1) - (3)].sval),(yyvsp[(3) - (3)].ival)))
			{
				exit(-1);

			}
	}
    break;

  case 16:

/* Line 1806 of yacc.c  */
#line 867 "sintax.y"
    {

			if (!guardaVars((yyvsp[(4) - (6)].ival)))
			{
				cout << "Redeclaration Variable on line: " << line_num << endl;
			exit (-1);
			}
		}
    break;

  case 17:

/* Line 1806 of yacc.c  */
#line 877 "sintax.y"
    {
			if (!guardaVars((yyvsp[(3) - (5)].ival)))
			{
				cout << "Redeclaration Variable on line: " << line_num << endl;
			exit (-1);
			}
		}
    break;

  case 18:

/* Line 1806 of yacc.c  */
#line 888 "sintax.y"
    {
		IDQueue.enqueue((yyvsp[(3) - (3)].sval));
		
	}
    break;

  case 19:

/* Line 1806 of yacc.c  */
#line 893 "sintax.y"
    {
		IDQueue.enqueue((yyvsp[(1) - (1)].sval));
	}
    break;

  case 20:

/* Line 1806 of yacc.c  */
#line 899 "sintax.y"
    {(yyval.ival)=2;}
    break;

  case 21:

/* Line 1806 of yacc.c  */
#line 900 "sintax.y"
    {(yyval.ival)=3;}
    break;

  case 22:

/* Line 1806 of yacc.c  */
#line 901 "sintax.y"
    {(yyval.ival)=1;}
    break;

  case 23:

/* Line 1806 of yacc.c  */
#line 902 "sintax.y"
    {(yyval.ival)=0;}
    break;

  case 24:

/* Line 1806 of yacc.c  */
#line 903 "sintax.y"
    {(yyval.ival)=4;}
    break;

  case 25:

/* Line 1806 of yacc.c  */
#line 907 "sintax.y"
    {
		myQuadStructure<<"BEGIN"<<endl;
		cout<<"*****Coding Start****"<<endl;
	}
    break;

  case 26:

/* Line 1806 of yacc.c  */
#line 912 "sintax.y"
    {
			myQuadStructure<<"END"<<endl;
			cout<<"****Coding Finish*****"<<endl;
		}
    break;

  case 29:

/* Line 1806 of yacc.c  */
#line 924 "sintax.y"
    {
		cout<<"****Function  Code****"<<endl;
	}
    break;

  case 30:

/* Line 1806 of yacc.c  */
#line 930 "sintax.y"
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
    break;

  case 32:

/* Line 1806 of yacc.c  */
#line 956 "sintax.y"
    {
		cout<<"****Function  Code****"<<endl;
	}
    break;

  case 33:

/* Line 1806 of yacc.c  */
#line 960 "sintax.y"
    {
		cout<<"Return void"<<endl;
		myQuadStructure<<"RETVOID"<<endl;
	}
    break;

  case 46:

/* Line 1806 of yacc.c  */
#line 986 "sintax.y"
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

  case 48:

/* Line 1806 of yacc.c  */
#line 1021 "sintax.y"
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

  case 49:

/* Line 1806 of yacc.c  */
#line 1044 "sintax.y"
    {
		myQuadStructure<<"ENDIF"<<endl;
	}
    break;

  case 51:

/* Line 1806 of yacc.c  */
#line 1056 "sintax.y"
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

  case 52:

/* Line 1806 of yacc.c  */
#line 1074 "sintax.y"
    {
	myQuadStructure<<"ENDCASE"<<endl;
	}
    break;

  case 55:

/* Line 1806 of yacc.c  */
#line 1087 "sintax.y"
    {
	if (tipoCase!=(yyvsp[(3) - (3)].ival))
	{
		cout<<"Incompatible types! on line: "<<line_num<<endl;
	}
	myQuadStructure<<"EXTRAOPTION\t"<<optionStr<<endl;
}
    break;

  case 56:

/* Line 1806 of yacc.c  */
#line 1096 "sintax.y"
    {
	if (tipoCase!=(yyvsp[(1) - (1)].ival))
	{
		cout<<"Incompatible types! on line: "<<line_num<<endl;
	}
	myQuadStructure<<"OPTION\t"<<optionStr<<endl;
}
    break;

  case 57:

/* Line 1806 of yacc.c  */
#line 1107 "sintax.y"
    {
		(yyval.ival)=2;
		optionStr=(yyvsp[(1) - (1)].sval);
		}
    break;

  case 58:

/* Line 1806 of yacc.c  */
#line 1111 "sintax.y"
    {
		(yyval.ival)=3;
		optionStr=(yyvsp[(1) - (1)].sval);
		}
    break;

  case 59:

/* Line 1806 of yacc.c  */
#line 1115 "sintax.y"
    {

		(yyval.ival)=1;
		optionStr=(yyvsp[(1) - (1)].sval);
		
		}
    break;

  case 60:

/* Line 1806 of yacc.c  */
#line 1121 "sintax.y"
    {
		(yyval.ival)=0;
		optionStr=(yyvsp[(1) - (1)].sval);

	}
    break;

  case 61:

/* Line 1806 of yacc.c  */
#line 1126 "sintax.y"
    {
		(yyval.ival)=4;
		optionStr=(yyvsp[(1) - (1)].sval);
	}
    break;

  case 62:

/* Line 1806 of yacc.c  */
#line 1139 "sintax.y"
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

  case 63:

/* Line 1806 of yacc.c  */
#line 1163 "sintax.y"
    {
	 	myQuadStructure<<"ENDWHILE"<<endl;
	 }
    break;

  case 65:

/* Line 1806 of yacc.c  */
#line 1173 "sintax.y"
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
    break;

  case 66:

/* Line 1806 of yacc.c  */
#line 1204 "sintax.y"
    {
		int tipoID;

		tipoID=buscaID((yyvsp[(3) - (6)].sval));

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

		myQuadStructure<<"READ\t"<<(yyvsp[(3) - (6)].sval)<<endl;
	}
    break;

  case 67:

/* Line 1806 of yacc.c  */
#line 1229 "sintax.y"
    {
		if (generaEspecial(0)==-1)
			exit(-1);
	}
    break;

  case 68:

/* Line 1806 of yacc.c  */
#line 1235 "sintax.y"
    {
		if (generaEspecial(1)==-1)
			exit(-1);
	}
    break;

  case 69:

/* Line 1806 of yacc.c  */
#line 1240 "sintax.y"
    {
		if (generaEspecial(2)==-1)
			exit(-1);
	}
    break;

  case 70:

/* Line 1806 of yacc.c  */
#line 1245 "sintax.y"
    {
		if (generaEspecial(3)==-1)
			exit(-1);
	}
    break;

  case 71:

/* Line 1806 of yacc.c  */
#line 1250 "sintax.y"
    {
		if (generaEspecial(4)==-1)
			exit(-1);
	}
    break;

  case 72:

/* Line 1806 of yacc.c  */
#line 1255 "sintax.y"
    {
		if (generaEspecial(5)==-1)
			exit(-1);
	}
    break;

  case 73:

/* Line 1806 of yacc.c  */
#line 1265 "sintax.y"
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

  case 74:

/* Line 1806 of yacc.c  */
#line 1296 "sintax.y"
    {
		myQuadStructure<<"ELSE"<<endl;
	}
    break;

  case 75:

/* Line 1806 of yacc.c  */
#line 1300 "sintax.y"
    {
		myQuadStructure<<"ENDELSE"<<endl;	
	}
    break;

  case 77:

/* Line 1806 of yacc.c  */
#line 1308 "sintax.y"
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
    break;

  case 78:

/* Line 1806 of yacc.c  */
#line 1328 "sintax.y"
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
    break;

  case 80:

/* Line 1806 of yacc.c  */
#line 1355 "sintax.y"
    {
			pilaOper.push(">");
		}
    break;

  case 81:

/* Line 1806 of yacc.c  */
#line 1359 "sintax.y"
    {
			if(generaExpresion()==-1)
			exit(-1);
		}
    break;

  case 82:

/* Line 1806 of yacc.c  */
#line 1364 "sintax.y"
    {
			pilaOper.push("<");
		}
    break;

  case 83:

/* Line 1806 of yacc.c  */
#line 1368 "sintax.y"
    {
	 		if(generaExpresion()==-1)
			exit(-1);	
	 	}
    break;

  case 84:

/* Line 1806 of yacc.c  */
#line 1373 "sintax.y"
    {
			pilaOper.push("<=");
		}
    break;

  case 85:

/* Line 1806 of yacc.c  */
#line 1378 "sintax.y"
    {
	 		if(generaExpresion()==-1)
			exit(-1);	
	 	}
    break;

  case 86:

/* Line 1806 of yacc.c  */
#line 1383 "sintax.y"
    {
			pilaOper.push(">=");
		}
    break;

  case 87:

/* Line 1806 of yacc.c  */
#line 1387 "sintax.y"
    {
	 		if(generaExpresion()==-1)
			exit(-1);	
	 	}
    break;

  case 88:

/* Line 1806 of yacc.c  */
#line 1393 "sintax.y"
    {
			pilaOper.push("<>");
		}
    break;

  case 89:

/* Line 1806 of yacc.c  */
#line 1397 "sintax.y"
    {
	 		if(generaExpresion()==-1)
			exit(-1);	
	 	}
    break;

  case 90:

/* Line 1806 of yacc.c  */
#line 1402 "sintax.y"
    {
			pilaOper.push("==");
		}
    break;

  case 91:

/* Line 1806 of yacc.c  */
#line 1406 "sintax.y"
    {
			if(generaExpresion()==-1)
			exit(-1);	
		}
    break;

  case 94:

/* Line 1806 of yacc.c  */
#line 1416 "sintax.y"
    {
		pilaOper.push("+");
		
	}
    break;

  case 95:

/* Line 1806 of yacc.c  */
#line 1422 "sintax.y"
    {
		if(generaExpresion()==-1)
			exit(-1);
	 }
    break;

  case 96:

/* Line 1806 of yacc.c  */
#line 1427 "sintax.y"
    {
		pilaOper.push("-");
	}
    break;

  case 97:

/* Line 1806 of yacc.c  */
#line 1431 "sintax.y"
    {
		if(generaExpresion()==-1)
			exit(-1);
	 }
    break;

  case 99:

/* Line 1806 of yacc.c  */
#line 1442 "sintax.y"
    {
		pilaOper.push("*");
	}
    break;

  case 100:

/* Line 1806 of yacc.c  */
#line 1446 "sintax.y"
    {
		if(generaExpresion()==-1)
			exit(-1);
	}
    break;

  case 101:

/* Line 1806 of yacc.c  */
#line 1451 "sintax.y"
    {
	pilaOper.push("/");
	}
    break;

  case 102:

/* Line 1806 of yacc.c  */
#line 1455 "sintax.y"
    {
		if(generaExpresion()==-1)
			exit(-1);
	 }
    break;

  case 105:

/* Line 1806 of yacc.c  */
#line 1467 "sintax.y"
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
    break;

  case 106:

/* Line 1806 of yacc.c  */
#line 1511 "sintax.y"
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

  case 107:

/* Line 1806 of yacc.c  */
#line 1523 "sintax.y"
    {

		pilaO.push((yyvsp[(1) - (1)].sval));
		pTipos.push(2);
		}
    break;

  case 108:

/* Line 1806 of yacc.c  */
#line 1528 "sintax.y"
    {

		pilaO.push((yyvsp[(1) - (1)].sval));
		pTipos.push(3);

		}
    break;

  case 109:

/* Line 1806 of yacc.c  */
#line 1534 "sintax.y"
    {

		pilaO.push((yyvsp[(1) - (1)].sval));
		pTipos.push(1);
		
		}
    break;

  case 110:

/* Line 1806 of yacc.c  */
#line 1540 "sintax.y"
    {
		pilaO.push((yyvsp[(1) - (1)].sval));
		pTipos.push(0);

	}
    break;

  case 111:

/* Line 1806 of yacc.c  */
#line 1545 "sintax.y"
    {
		pilaO.push((yyvsp[(1) - (1)].sval));
		pTipos.push(4);
	}
    break;

  case 112:

/* Line 1806 of yacc.c  */
#line 1553 "sintax.y"
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

  case 113:

/* Line 1806 of yacc.c  */
#line 1573 "sintax.y"
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
#line 3398 "sintax.tab.c"
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
#line 1599 "sintax.y"


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

