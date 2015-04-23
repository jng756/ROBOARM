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

varEntry variables=varEntry();
tableEntry entry=tableEntry();
table pragma= table();

struct IDs {
	string IDstr;
	int tipoVar;
} tempStr;

Queue<IDs> IDstruct;

Queue<string> lista_params;

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
		cout<<"Variables Locales"<<endl;
		while(IDQueue.dequeue(IDstring))
			{
				variables.setNombreVar(IDstring);
				variables.setDirVirtual(tipoVar);

				cout<<"Variable: "<<IDstring <<" 	tipoVar: "<<tipoVar<<endl;
				//Generar Cuadruplo

				myQuadStructure<<"var\t"<<IDstring<<"\ttipo\t"<<decodificaTipo(tipoVar)<<endl;
			
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
				myQuadStructure<<"var\t"<<IDstring<<"\ttipo\t"<<decodificaTipo(tipoVar)<<endl;		
				
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




/* Line 268 of yacc.c  */
#line 468 "sintax.tab.c"

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
     TBEGIN = 275,
     TEND = 276,
     TRET = 277,
     ID = 278,
     DIFF = 279,
     EQUALS = 280,
     ENDL = 281,
     ROTATEWAIST = 282,
     ROTATESHOULDER = 283,
     ROTATEELBOW = 284,
     ROTATEWRISTM = 285,
     ROTATEWRISTR = 286,
     ROTATETOOL = 287,
     CTE_I = 288,
     CTE_F = 289,
     CTE_STRING = 290,
     CTE_CHAR = 291,
     CTE_BOOL = 292
   };
#endif



#if ! defined YYSTYPE && ! defined YYSTYPE_IS_DECLARED
typedef union YYSTYPE
{

/* Line 293 of yacc.c  */
#line 402 "sintax.y"

	int ival;
	float fval;
	char *sval;
	char cval;
	bool bval;



/* Line 293 of yacc.c  */
#line 551 "sintax.tab.c"
} YYSTYPE;
# define YYSTYPE_IS_TRIVIAL 1
# define yystype YYSTYPE /* obsolescent; will be withdrawn */
# define YYSTYPE_IS_DECLARED 1
#endif


/* Copy the second part of user declarations.  */


/* Line 343 of yacc.c  */
#line 563 "sintax.tab.c"

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
#define YYLAST   226

/* YYNTOKENS -- Number of terminals.  */
#define YYNTOKENS  52
/* YYNNTS -- Number of nonterminals.  */
#define YYNNTS  45
/* YYNRULES -- Number of rules.  */
#define YYNRULES  88
/* YYNRULES -- Number of states.  */
#define YYNSTATES  235

/* YYTRANSLATE(YYLEX) -- Bison symbol number corresponding to YYLEX.  */
#define YYUNDEFTOK  2
#define YYMAXUTOK   292

#define YYTRANSLATE(YYX)						\
  ((unsigned int) (YYX) <= YYMAXUTOK ? yytranslate[YYX] : YYUNDEFTOK)

/* YYTRANSLATE[YYLEX] -- Bison symbol number corresponding to YYLEX.  */
static const yytype_uint8 yytranslate[] =
{
       0,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
      39,    40,    50,    48,    42,    49,     2,    51,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,    41,    38,
      47,    45,    46,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,    43,     2,    44,     2,     2,     2,     2,
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
      35,    36,    37
};

#if YYDEBUG
/* YYPRHS[YYN] -- Index of the first RHS symbol of rule number YYN in
   YYRHS.  */
static const yytype_uint16 yyprhs[] =
{
       0,     0,     3,     4,     5,    15,    16,    17,    33,    34,
      35,    51,    52,    58,    62,    66,    67,    74,    80,    84,
      86,    88,    90,    92,    94,    96,   102,   108,   109,   119,
     125,   128,   129,   131,   133,   135,   137,   139,   141,   143,
     145,   146,   153,   154,   163,   170,   184,   194,   201,   208,
     215,   222,   229,   236,   243,   250,   254,   255,   259,   261,
     264,   267,   270,   273,   276,   277,   279,   280,   285,   286,
     291,   293,   294,   299,   300,   305,   309,   311,   316,   318,
     320,   322,   324,   326,   328,   332,   334,   335,   338
};

/* YYRHS -- A `-1'-separated list of the rules' RHS.  */
static const yytype_int8 yyrhs[] =
{
      53,     0,    -1,    -1,    -1,     3,    23,    54,    38,    96,
      62,    56,    55,    66,    -1,    -1,    -1,    13,    23,    39,
      61,    40,    41,    65,    96,    57,    62,    68,    38,    96,
      58,    56,    -1,    -1,    -1,    13,    23,    39,    61,    40,
      41,    14,    96,    59,    62,    70,    38,    96,    60,    56,
      -1,    -1,    61,    42,    23,    41,    65,    -1,    23,    41,
      65,    -1,     9,    96,    63,    -1,    -1,    63,    64,    41,
      65,    38,    96,    -1,    64,    41,    65,    38,    96,    -1,
      64,    42,    23,    -1,    23,    -1,     4,    -1,     5,    -1,
      11,    -1,    12,    -1,     6,    -1,    20,    96,    71,    21,
      96,    -1,    43,    96,    71,    44,    96,    -1,    -1,    43,
      96,    71,    22,    85,    69,    96,    44,    96,    -1,    43,
      96,    71,    44,    96,    -1,    72,    71,    -1,    -1,    73,
      -1,    75,    -1,    77,    -1,    78,    -1,    79,    -1,    80,
      -1,    81,    -1,    82,    -1,    -1,    23,    45,    85,    74,
      38,    96,    -1,    -1,     7,    39,    85,    40,    76,    96,
      67,    83,    -1,    10,    39,    84,    40,    38,    96,    -1,
      17,    23,    18,    96,    43,    86,    41,    72,    38,    96,
      44,    38,    96,    -1,    15,    39,    85,    40,    16,    96,
      67,    38,    96,    -1,    19,    39,    23,    40,    38,    96,
      -1,    27,    39,    94,    40,    38,    96,    -1,    28,    39,
      94,    40,    38,    96,    -1,    29,    39,    94,    40,    38,
      96,    -1,    30,    39,    94,    40,    38,    96,    -1,    31,
      39,    94,    40,    38,    96,    -1,    32,    39,    94,    40,
      38,    96,    -1,    23,    39,    95,    40,    38,    96,    -1,
       8,    96,    67,    -1,    -1,    84,    42,    85,    -1,    85,
      -1,    87,    86,    -1,    46,    87,    -1,    47,    87,    -1,
      24,    87,    -1,    25,    87,    -1,    -1,    90,    -1,    -1,
      87,    48,    88,    90,    -1,    -1,    87,    49,    89,    90,
      -1,    93,    -1,    -1,    90,    50,    91,    93,    -1,    -1,
      90,    51,    92,    93,    -1,    39,    85,    40,    -1,    94,
      -1,    23,    39,    95,    40,    -1,    23,    -1,    33,    -1,
      34,    -1,    35,    -1,    36,    -1,    37,    -1,    95,    42,
      87,    -1,    87,    -1,    -1,    26,    96,    -1,    -1
};

/* YYRLINE[YYN] -- source line where rule number YYN was defined.  */
static const yytype_uint16 yyrline[] =
{
       0,   464,   464,   467,   464,   477,   484,   476,   493,   500,
     493,   507,   512,   522,   533,   534,   538,   548,   559,   564,
     571,   572,   573,   574,   575,   579,   583,   590,   586,   615,
     619,   620,   624,   625,   626,   627,   628,   629,   630,   631,
     636,   635,   671,   670,   679,   682,   686,   690,   695,   696,
     697,   698,   699,   700,   705,   735,   736,   740,   741,   745,
     749,   750,   751,   752,   753,   757,   759,   758,   769,   768,
     781,   783,   782,   791,   790,   800,   801,   805,   848,   861,
     866,   872,   878,   883,   890,   909,   929,   933,   934
};
#endif

#if YYDEBUG || YYERROR_VERBOSE || YYTOKEN_TABLE
/* YYTNAME[SYMBOL-NUM] -- String name of the symbol SYMBOL-NUM.
   First, the terminals, then, starting at YYNTOKENS, nonterminals.  */
static const char *const yytname[] =
{
  "$end", "error", "$undefined", "PROG", "INT", "FLOAT", "BOOL", "IF",
  "ELSE", "VAR", "PRINT", "STRING", "CHAR", "FUNCTION", "VOID", "WHILE",
  "DO", "CASE", "OF", "READ", "TBEGIN", "TEND", "TRET", "ID", "DIFF",
  "EQUALS", "ENDL", "ROTATEWAIST", "ROTATESHOULDER", "ROTATEELBOW",
  "ROTATEWRISTM", "ROTATEWRISTR", "ROTATETOOL", "CTE_I", "CTE_F",
  "CTE_STRING", "CTE_CHAR", "CTE_BOOL", "';'", "'('", "')'", "':'", "','",
  "'{'", "'}'", "'='", "'>'", "'<'", "'+'", "'-'", "'*'", "'/'", "$accept",
  "programa", "$@1", "$@2", "functions", "$@3", "$@4", "$@5", "$@6",
  "def_param", "vars", "def_vars", "def_id", "tipo", "coding", "bloque",
  "bloque_func", "$@7", "bloque_func2", "def_estatuto", "estatuto",
  "asignacion", "$@8", "condicion", "$@9", "escritura", "casos", "ciclo",
  "lectura", "especial", "llamadas", "def_else", "mensaje", "expresion",
  "mas_expr", "exp", "$@10", "$@11", "termino", "$@12", "$@13", "factor",
  "varcte", "params", "endl", 0
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
     285,   286,   287,   288,   289,   290,   291,   292,    59,    40,
      41,    58,    44,   123,   125,    61,    62,    60,    43,    45,
      42,    47
};
# endif

/* YYR1[YYN] -- Symbol number of symbol that rule YYN derives.  */
static const yytype_uint8 yyr1[] =
{
       0,    52,    54,    55,    53,    57,    58,    56,    59,    60,
      56,    56,    61,    61,    62,    62,    63,    63,    64,    64,
      65,    65,    65,    65,    65,    66,    67,    69,    68,    70,
      71,    71,    72,    72,    72,    72,    72,    72,    72,    72,
      74,    73,    76,    75,    77,    78,    79,    80,    81,    81,
      81,    81,    81,    81,    82,    83,    83,    84,    84,    85,
      86,    86,    86,    86,    86,    87,    88,    87,    89,    87,
      90,    91,    90,    92,    90,    93,    93,    94,    94,    94,
      94,    94,    94,    94,    95,    95,    95,    96,    96
};

/* YYR2[YYN] -- Number of symbols composing right hand side of rule YYN.  */
static const yytype_uint8 yyr2[] =
{
       0,     2,     0,     0,     9,     0,     0,    15,     0,     0,
      15,     0,     5,     3,     3,     0,     6,     5,     3,     1,
       1,     1,     1,     1,     1,     5,     5,     0,     9,     5,
       2,     0,     1,     1,     1,     1,     1,     1,     1,     1,
       0,     6,     0,     8,     6,    13,     9,     6,     6,     6,
       6,     6,     6,     6,     6,     3,     0,     3,     1,     2,
       2,     2,     2,     2,     0,     1,     0,     4,     0,     4,
       1,     0,     4,     0,     4,     3,     1,     4,     1,     1,
       1,     1,     1,     1,     3,     1,     0,     2,     0
};

/* YYDEFACT[STATE-NAME] -- Default reduction number in state STATE-NUM.
   Performed when YYTABLE doesn't specify something else to do.  Zero
   means the default is an error.  */
static const yytype_uint8 yydefact[] =
{
       0,     0,     0,     2,     1,     0,    88,    88,    15,    87,
      88,    11,     0,     0,     3,    19,    14,     0,     0,     0,
       0,     0,     0,     0,    88,     4,     0,    20,    21,    24,
      22,    23,     0,    18,     0,     0,    31,     0,    88,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,    31,    32,    33,    34,    35,
      36,    37,    38,    39,    88,    17,    13,     0,     0,     0,
       0,     0,     0,     0,    86,     0,     0,     0,     0,     0,
       0,     0,    88,    30,    16,    88,    88,     0,    78,    79,
      80,    81,    82,    83,     0,     0,    64,    65,    70,    76,
       0,    58,     0,    88,     0,    85,     0,    40,     0,     0,
       0,     0,     0,     0,    25,     8,     5,    12,    86,     0,
      42,     0,     0,     0,     0,    66,    68,    59,    71,    73,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,    15,    15,     0,    75,    88,    62,
      63,    60,    61,     0,     0,     0,     0,    88,    57,    88,
      64,    88,    88,    84,    88,    88,    88,    88,    88,    88,
      88,     0,     0,    77,     0,    67,    69,    72,    74,    44,
       0,     0,    47,    54,    41,    48,    49,    50,    51,    52,
      53,    88,     0,    88,     0,    88,    56,     0,     0,    31,
      88,    31,    88,    31,    88,    43,    88,     0,     0,     9,
       0,     6,     0,     0,    46,    88,    88,    11,     0,    11,
      88,    55,     0,    29,    10,    27,     7,    26,     0,    88,
      88,     0,    45,    88,    28
};

/* YYDEFGOTO[NTERM-NUM].  */
static const yytype_int16 yydefgoto[] =
{
      -1,     2,     5,    19,    14,   145,   219,   144,   217,    35,
      11,    16,    17,    32,    25,   196,   194,   229,   192,    54,
      55,    56,   137,    57,   148,    58,    59,    60,    61,    62,
      63,   205,   100,    95,   127,    96,   153,   154,    97,   155,
     156,    98,    99,   106,     8
};

/* YYPACT[STATE-NUM] -- Index in YYTABLE of the portion describing
   STATE-NUM.  */
#define YYPACT_NINF -176
static const yytype_int16 yypact[] =
{
       9,    -4,    27,  -176,  -176,   -29,    21,    21,    41,  -176,
      21,    42,    38,    44,  -176,  -176,    38,   -19,    20,    51,
      23,    99,    63,    72,    21,  -176,    99,  -176,  -176,  -176,
    -176,  -176,    82,  -176,    60,   -24,    53,    83,    21,    99,
      81,   100,    85,    86,    87,   104,    89,   -32,    90,    91,
      92,    93,    94,    95,   114,    53,  -176,  -176,  -176,  -176,
    -176,  -176,  -176,  -176,    21,  -176,  -176,    88,    96,     6,
       6,     6,   118,   115,     6,     6,    54,    54,    54,    54,
      54,    54,    21,  -176,  -176,    21,    21,    99,   101,  -176,
    -176,  -176,  -176,  -176,     6,   102,   -14,    47,  -176,  -176,
     -16,  -176,   103,    21,   105,    25,   -12,  -176,   106,   107,
     108,   111,   113,   116,  -176,  -176,  -176,  -176,     6,   125,
    -176,     6,     6,     6,     6,  -176,  -176,  -176,  -176,  -176,
     129,     6,   123,   126,   132,   133,     6,   134,   135,   136,
     137,   138,   139,   140,    41,    41,     4,  -176,    21,    25,
      25,    25,    25,     6,     6,     6,     6,    21,  -176,    21,
     -10,    21,    21,    25,    21,    21,    21,    21,    21,    21,
      21,   142,   144,  -176,   146,    47,    47,  -176,  -176,  -176,
     146,   141,  -176,  -176,  -176,  -176,  -176,  -176,  -176,  -176,
    -176,    21,   143,    21,   145,    21,   171,   152,    53,    53,
      21,    53,    21,    53,    21,  -176,    21,   153,   148,  -176,
     122,  -176,   150,   146,  -176,    21,    21,    42,     6,    42,
      21,  -176,   154,  -176,  -176,  -176,  -176,  -176,   158,    21,
      21,   156,  -176,    21,  -176
};

/* YYPGOTO[NTERM-NUM].  */
static const yytype_int16 yypgoto[] =
{
    -176,  -176,  -176,  -176,  -161,  -176,  -176,  -176,  -176,  -176,
     -38,  -176,   164,   -18,  -176,  -175,  -176,  -176,  -176,   -35,
       3,  -176,  -176,  -176,  -176,  -176,  -176,  -176,  -176,  -176,
    -176,  -176,  -176,   -69,    43,   -70,  -176,  -176,   -45,  -176,
    -176,   -37,    36,    84,    -7
};

/* YYTABLE[YYPACT[STATE-NUM]].  What to do in state STATE-NUM.  If
   positive, shift that token.  If negative, reduce the rule which
   number is the opposite.  If YYTABLE_NINF, syntax error.  */
#define YYTABLE_NINF -1
static const yytype_uint8 yytable[] =
{
       9,   101,   102,    12,   105,   197,   107,    74,    37,     6,
     121,   122,     1,    75,   121,   122,    40,    36,    41,     3,
      83,    66,    21,    22,   130,   119,   131,     4,   135,    88,
     136,    65,   123,   124,   125,   126,   123,   124,   221,    89,
      90,    91,    92,    93,   173,    94,   136,     7,   105,    86,
      10,   149,   150,   151,   152,    13,   224,    84,   226,    23,
      42,    15,   158,    43,    26,    22,   163,    18,    44,   117,
      45,    24,    46,   125,   126,   114,    47,    88,   115,   116,
      48,    49,    50,    51,    52,    53,    33,    89,    90,    91,
      92,    93,    27,    28,    29,    34,   133,   128,   129,    30,
      31,    39,    85,    27,    28,    29,   171,   172,   175,   176,
      30,    31,   108,   109,   110,   111,   112,   113,   177,   178,
      38,    64,    67,    68,    69,    70,    71,    72,    73,    76,
      77,    78,    79,    80,    81,    82,   103,    87,   104,   159,
     118,   174,   120,   132,   218,   134,   138,   139,   140,   225,
     179,   141,   180,   142,   182,   183,   143,   184,   185,   186,
     187,   188,   189,   190,   208,   147,   210,   157,   212,   160,
     161,   162,   164,   165,   166,   167,   168,   169,   170,   204,
      20,   200,   198,   202,   199,   191,   201,   193,   203,   195,
     206,   215,   216,   209,   220,   211,   230,   213,   228,   214,
     233,   207,   146,   181,     0,     0,     0,     0,   222,   223,
       0,     0,     0,   227,     0,     0,     0,     0,     0,     0,
       0,     0,   231,   232,     0,     0,   234
};

#define yypact_value_is_default(yystate) \
  ((yystate) == (-176))

#define yytable_value_is_error(yytable_value) \
  YYID (0)

static const yytype_int16 yycheck[] =
{
       7,    70,    71,    10,    74,   180,    75,    39,    26,    38,
      24,    25,     3,    45,    24,    25,    40,    24,    42,    23,
      55,    39,    41,    42,    40,    94,    42,     0,    40,    23,
      42,    38,    46,    47,    48,    49,    46,    47,   213,    33,
      34,    35,    36,    37,    40,    39,    42,    26,   118,    67,
       9,   121,   122,   123,   124,    13,   217,    64,   219,    39,
       7,    23,   131,    10,    41,    42,   136,    23,    15,    87,
      17,    20,    19,    48,    49,    82,    23,    23,    85,    86,
      27,    28,    29,    30,    31,    32,    23,    33,    34,    35,
      36,    37,     4,     5,     6,    23,   103,    50,    51,    11,
      12,    41,    14,     4,     5,     6,   144,   145,   153,   154,
      11,    12,    76,    77,    78,    79,    80,    81,   155,   156,
      38,    38,    41,    23,    39,    39,    39,    23,    39,    39,
      39,    39,    39,    39,    39,    21,    18,    41,    23,    16,
      39,   148,    40,    40,    22,    40,    40,    40,    40,   218,
     157,    40,   159,    40,   161,   162,    40,   164,   165,   166,
     167,   168,   169,   170,   199,    40,   201,    38,   203,    43,
      38,    38,    38,    38,    38,    38,    38,    38,    38,     8,
      16,    38,    41,    38,   191,    43,   193,    43,   195,    43,
      38,    38,    44,   200,    44,   202,    38,   204,    44,   206,
      44,   198,   118,   160,    -1,    -1,    -1,    -1,   215,   216,
      -1,    -1,    -1,   220,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   229,   230,    -1,    -1,   233
};

/* YYSTOS[STATE-NUM] -- The (internal number of the) accessing
   symbol of state STATE-NUM.  */
static const yytype_uint8 yystos[] =
{
       0,     3,    53,    23,     0,    54,    38,    26,    96,    96,
       9,    62,    96,    13,    56,    23,    63,    64,    23,    55,
      64,    41,    42,    39,    20,    66,    41,     4,     5,     6,
      11,    12,    65,    23,    23,    61,    96,    65,    38,    41,
      40,    42,     7,    10,    15,    17,    19,    23,    27,    28,
      29,    30,    31,    32,    71,    72,    73,    75,    77,    78,
      79,    80,    81,    82,    38,    96,    65,    41,    23,    39,
      39,    39,    23,    39,    39,    45,    39,    39,    39,    39,
      39,    39,    21,    71,    96,    14,    65,    41,    23,    33,
      34,    35,    36,    37,    39,    85,    87,    90,    93,    94,
      84,    85,    85,    18,    23,    87,    95,    85,    94,    94,
      94,    94,    94,    94,    96,    96,    96,    65,    39,    85,
      40,    24,    25,    46,    47,    48,    49,    86,    50,    51,
      40,    42,    40,    96,    40,    40,    42,    74,    40,    40,
      40,    40,    40,    40,    59,    57,    95,    40,    76,    87,
      87,    87,    87,    88,    89,    91,    92,    38,    85,    16,
      43,    38,    38,    87,    38,    38,    38,    38,    38,    38,
      38,    62,    62,    40,    96,    90,    90,    93,    93,    96,
      96,    86,    96,    96,    96,    96,    96,    96,    96,    96,
      96,    43,    70,    43,    68,    43,    67,    67,    41,    96,
      38,    96,    38,    96,     8,    83,    38,    72,    71,    96,
      71,    96,    71,    96,    96,    38,    44,    60,    22,    58,
      44,    67,    96,    96,    56,    85,    56,    96,    44,    69,
      38,    96,    96,    44,    96
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
#line 464 "sintax.y"
    {
		pragma.setNombre((yyvsp[(2) - (2)].sval));
	}
    break;

  case 3:

/* Line 1806 of yacc.c  */
#line 467 "sintax.y"
    {
		localFlag=false;
	}
    break;

  case 4:

/* Line 1806 of yacc.c  */
#line 472 "sintax.y"
    {  cout << "Fin del analisis del programa " << endl; }
    break;

  case 5:

/* Line 1806 of yacc.c  */
#line 477 "sintax.y"
    {
			localFlag=true;
			guardaFuncion(1,(yyvsp[(2) - (8)].sval),(yyvsp[(7) - (8)].ival), parametros);
			tipoFunction=(yyvsp[(7) - (8)].ival);
		}
    break;

  case 6:

/* Line 1806 of yacc.c  */
#line 484 "sintax.y"
    {	
			//vaciar la estructura de variables para iniciar una nueva funcion
			entry.setVarTable(new HashMap<varEntry>);
			//Vaciar params
			parametros="";
		}
    break;

  case 8:

/* Line 1806 of yacc.c  */
#line 493 "sintax.y"
    {
		localFlag=true;
		guardaFuncion(2,(yyvsp[(2) - (8)].sval),5, parametros);
		tipoFunction=5;
		}
    break;

  case 9:

/* Line 1806 of yacc.c  */
#line 500 "sintax.y"
    {	
			//vaciar la estructura de variables para iniciar una nueva funcion
			entry.setVarTable(new HashMap<varEntry>);
			//Vaciar params
			parametros="";
		}
    break;

  case 12:

/* Line 1806 of yacc.c  */
#line 513 "sintax.y"
    {

		if (!guardaParametros((yyvsp[(3) - (5)].sval),(yyvsp[(5) - (5)].ival)))
			{
				exit(-1);
			}
		
		
		}
    break;

  case 13:

/* Line 1806 of yacc.c  */
#line 523 "sintax.y"
    {
		if (!guardaParametros((yyvsp[(1) - (3)].sval),(yyvsp[(3) - (3)].ival)))
			{
				exit(-1);

			}
	}
    break;

  case 16:

/* Line 1806 of yacc.c  */
#line 539 "sintax.y"
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
#line 549 "sintax.y"
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
#line 560 "sintax.y"
    {
		IDQueue.enqueue((yyvsp[(3) - (3)].sval));
		
	}
    break;

  case 19:

/* Line 1806 of yacc.c  */
#line 565 "sintax.y"
    {
		IDQueue.enqueue((yyvsp[(1) - (1)].sval));
	}
    break;

  case 20:

/* Line 1806 of yacc.c  */
#line 571 "sintax.y"
    {(yyval.ival)=2;}
    break;

  case 21:

/* Line 1806 of yacc.c  */
#line 572 "sintax.y"
    {(yyval.ival)=3;}
    break;

  case 22:

/* Line 1806 of yacc.c  */
#line 573 "sintax.y"
    {(yyval.ival)=1;}
    break;

  case 23:

/* Line 1806 of yacc.c  */
#line 574 "sintax.y"
    {(yyval.ival)=0;}
    break;

  case 24:

/* Line 1806 of yacc.c  */
#line 575 "sintax.y"
    {(yyval.ival)=4;}
    break;

  case 27:

/* Line 1806 of yacc.c  */
#line 590 "sintax.y"
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

		myQuadStructure<<"return\t"<< operador<<endl;

	}
    break;

  case 40:

/* Line 1806 of yacc.c  */
#line 636 "sintax.y"
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
			cout<<"Expresion no compatible"<<endl;
			exit(-1);
		}

		myQuadStructure<<"=\t"<<(yyvsp[(1) - (3)].sval)<<"\t"<<operador<<endl;

	}
    break;

  case 42:

/* Line 1806 of yacc.c  */
#line 671 "sintax.y"
    {


	}
    break;

  case 54:

/* Line 1806 of yacc.c  */
#line 706 "sintax.y"
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

  case 66:

/* Line 1806 of yacc.c  */
#line 759 "sintax.y"
    {
		pilaOper.push('+');
		
	}
    break;

  case 67:

/* Line 1806 of yacc.c  */
#line 765 "sintax.y"
    {
		generaExpresion();
	 }
    break;

  case 68:

/* Line 1806 of yacc.c  */
#line 769 "sintax.y"
    {
		pilaOper.push('-');
	}
    break;

  case 69:

/* Line 1806 of yacc.c  */
#line 773 "sintax.y"
    {
		generaExpresion();
	 }
    break;

  case 71:

/* Line 1806 of yacc.c  */
#line 783 "sintax.y"
    {
		pilaOper.push('*');
	}
    break;

  case 72:

/* Line 1806 of yacc.c  */
#line 787 "sintax.y"
    {
		generaExpresion();
	}
    break;

  case 73:

/* Line 1806 of yacc.c  */
#line 791 "sintax.y"
    {
	pilaOper.push('/');
	}
    break;

  case 74:

/* Line 1806 of yacc.c  */
#line 795 "sintax.y"
    {
		generaExpresion();
	 }
    break;

  case 77:

/* Line 1806 of yacc.c  */
#line 806 "sintax.y"
    {
		int tipo=-1;
		string params="";
		string variable="";
		string acum="";
		tipo=buscaFuncion((yyvsp[(1) - (4)].sval), params);
		cout<<"Funcion: "<<(yyvsp[(1) - (4)].sval)<<"\tTipo:"<<tipo<<"\tParametros registrados: "<<params<<endl;
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

  case 78:

/* Line 1806 of yacc.c  */
#line 849 "sintax.y"
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

  case 79:

/* Line 1806 of yacc.c  */
#line 861 "sintax.y"
    {

		pilaO.push((yyvsp[(1) - (1)].sval));
		pTipos.push(2);
		}
    break;

  case 80:

/* Line 1806 of yacc.c  */
#line 866 "sintax.y"
    {

		pilaO.push((yyvsp[(1) - (1)].sval));
		pTipos.push(3);

		}
    break;

  case 81:

/* Line 1806 of yacc.c  */
#line 872 "sintax.y"
    {

		pilaO.push((yyvsp[(1) - (1)].sval));
		pTipos.push(1);
		
		}
    break;

  case 82:

/* Line 1806 of yacc.c  */
#line 878 "sintax.y"
    {
		pilaO.push((yyvsp[(1) - (1)].sval));
		pTipos.push(0);

	}
    break;

  case 83:

/* Line 1806 of yacc.c  */
#line 883 "sintax.y"
    {
		pilaO.push((yyvsp[(1) - (1)].sval));
		pTipos.push(4);
	}
    break;

  case 84:

/* Line 1806 of yacc.c  */
#line 891 "sintax.y"
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

  case 85:

/* Line 1806 of yacc.c  */
#line 910 "sintax.y"
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
#line 2506 "sintax.tab.c"
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
#line 936 "sintax.y"


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

