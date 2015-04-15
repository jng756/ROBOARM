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
Stack<int> Ptipos;
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
//Direcciones Globales

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

//Direcciones locales

else if (dirVirtual>offLocal && dirVirtual<offLocal+MEMSIZEARRAYDATA)
	return 0;
else if (dirVirtual>offLocal+MEMSIZEARRAYDATA*1-1 && dirVirtual<offLocal+ MEMSIZEARRAYDATA*2)
	return 1;
else if (dirVirtual>offLocal+MEMSIZEARRAYDATA*2-1 && dirVirtual<offLocal+ MEMSIZEARRAYDATA*3)
	return 2;
else if (dirVirtual>offLocal+MEMSIZEARRAYDATA*3-1 && dirVirtual<offLocal+ MEMSIZEARRAYDATA*4)
	return 3;
else if (dirVirtual>offLocal+MEMSIZEARRAYDATA*4-1 && dirVirtual<offLocal+MEMSIZEARRAYDATA*5)
	return 4;

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





/* Line 268 of yacc.c  */
#line 391 "sintax.tab.c"

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
     ENDL = 280,
     ROTATEWAIST = 281,
     ROTATESHOULDER = 282,
     ROTATEELBOW = 283,
     ROTATEWRISTM = 284,
     ROTATEWRISTR = 285,
     ROTATETOOL = 286,
     CTE_I = 287,
     CTE_F = 288,
     CTE_STRING = 289,
     CTE_CHAR = 290,
     CTE_BOOL = 291
   };
#endif



#if ! defined YYSTYPE && ! defined YYSTYPE_IS_DECLARED
typedef union YYSTYPE
{

/* Line 293 of yacc.c  */
#line 325 "sintax.y"

	int ival;
	float fval;
	char *sval;
	char cval;
	bool bval;



/* Line 293 of yacc.c  */
#line 473 "sintax.tab.c"
} YYSTYPE;
# define YYSTYPE_IS_TRIVIAL 1
# define yystype YYSTYPE /* obsolescent; will be withdrawn */
# define YYSTYPE_IS_DECLARED 1
#endif


/* Copy the second part of user declarations.  */


/* Line 343 of yacc.c  */
#line 485 "sintax.tab.c"

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
#define YYLAST   214

/* YYNTOKENS -- Number of terminals.  */
#define YYNTOKENS  51
/* YYNNTS -- Number of nonterminals.  */
#define YYNNTS  40
/* YYNRULES -- Number of rules.  */
#define YYNRULES  81
/* YYNRULES -- Number of states.  */
#define YYNSTATES  224

/* YYTRANSLATE(YYLEX) -- Bison symbol number corresponding to YYLEX.  */
#define YYUNDEFTOK  2
#define YYMAXUTOK   291

#define YYTRANSLATE(YYX)						\
  ((unsigned int) (YYX) <= YYMAXUTOK ? yytranslate[YYX] : YYUNDEFTOK)

/* YYTRANSLATE[YYLEX] -- Bison symbol number corresponding to YYLEX.  */
static const yytype_uint8 yytranslate[] =
{
       0,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
      38,    39,    49,    47,    41,    48,     2,    50,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,    40,    37,
      46,    44,    45,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,    42,     2,    43,     2,     2,     2,     2,
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
      35,    36
};

#if YYDEBUG
/* YYPRHS[YYN] -- Index of the first RHS symbol of rule number YYN in
   YYRHS.  */
static const yytype_uint16 yyprhs[] =
{
       0,     0,     3,     4,     5,    15,    16,    17,    33,    34,
      35,    51,    52,    58,    62,    66,    67,    74,    80,    84,
      86,    88,    90,    92,    94,    96,   102,   108,   117,   123,
     126,   127,   129,   131,   133,   135,   137,   139,   141,   147,
     155,   162,   176,   186,   193,   200,   207,   214,   221,   228,
     235,   239,   240,   244,   246,   249,   252,   255,   258,   261,
     262,   264,   265,   270,   271,   276,   278,   279,   284,   285,
     290,   294,   296,   299,   302,   304,   306,   308,   310,   312,
     314,   317
};

/* YYRHS -- A `-1'-separated list of the rules' RHS.  */
static const yytype_int8 yyrhs[] =
{
      52,     0,    -1,    -1,    -1,     3,    23,    53,    37,    90,
      61,    55,    54,    65,    -1,    -1,    -1,    13,    23,    38,
      60,    39,    40,    64,    90,    56,    61,    67,    37,    90,
      57,    55,    -1,    -1,    -1,    13,    23,    38,    60,    39,
      40,    14,    90,    58,    61,    68,    37,    90,    59,    55,
      -1,    -1,    60,    41,    23,    40,    64,    -1,    23,    40,
      64,    -1,     9,    90,    62,    -1,    -1,    62,    63,    40,
      64,    37,    90,    -1,    63,    40,    64,    37,    90,    -1,
      63,    41,    23,    -1,    23,    -1,     4,    -1,     5,    -1,
      11,    -1,    12,    -1,     6,    -1,    20,    90,    69,    21,
      90,    -1,    42,    90,    69,    43,    90,    -1,    42,    90,
      69,    22,    80,    90,    43,    90,    -1,    42,    90,    69,
      43,    90,    -1,    70,    69,    -1,    -1,    71,    -1,    72,
      -1,    73,    -1,    74,    -1,    75,    -1,    76,    -1,    77,
      -1,    23,    44,    80,    37,    90,    -1,     7,    38,    80,
      39,    90,    66,    78,    -1,    10,    38,    79,    39,    37,
      90,    -1,    17,    23,    18,    90,    42,    81,    40,    70,
      37,    90,    43,    37,    90,    -1,    15,    38,    80,    39,
      16,    90,    66,    37,    90,    -1,    19,    38,    23,    39,
      37,    90,    -1,    26,    38,    89,    39,    37,    90,    -1,
      27,    38,    89,    39,    37,    90,    -1,    28,    38,    89,
      39,    37,    90,    -1,    29,    38,    89,    39,    37,    90,
      -1,    30,    38,    89,    39,    37,    90,    -1,    31,    38,
      89,    39,    37,    90,    -1,     8,    90,    66,    -1,    -1,
      79,    41,    80,    -1,    80,    -1,    82,    81,    -1,    45,
      82,    -1,    46,    82,    -1,    24,    82,    -1,    44,    82,
      -1,    -1,    85,    -1,    -1,    82,    47,    83,    85,    -1,
      -1,    82,    48,    84,    85,    -1,    88,    -1,    -1,    85,
      49,    86,    88,    -1,    -1,    85,    50,    87,    88,    -1,
      38,    80,    39,    -1,    89,    -1,    47,    89,    -1,    48,
      89,    -1,    23,    -1,    32,    -1,    33,    -1,    34,    -1,
      35,    -1,    36,    -1,    25,    90,    -1,    -1
};

/* YYRLINE[YYN] -- source line where rule number YYN was defined.  */
static const yytype_uint16 yyrline[] =
{
       0,   387,   387,   390,   387,   398,   430,   397,   434,   450,
     434,   452,   456,   462,   472,   473,   477,   483,   491,   496,
     503,   504,   505,   506,   507,   511,   515,   518,   521,   525,
     526,   530,   531,   532,   533,   534,   535,   536,   540,   544,
     548,   551,   555,   559,   564,   565,   566,   567,   568,   569,
     573,   574,   578,   579,   583,   587,   588,   589,   590,   591,
     595,   597,   596,   607,   606,   619,   621,   620,   629,   628,
     638,   639,   640,   641,   645,   653,   654,   655,   656,   657,
     661,   662
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
  "ENDL", "ROTATEWAIST", "ROTATESHOULDER", "ROTATEELBOW", "ROTATEWRISTM",
  "ROTATEWRISTR", "ROTATETOOL", "CTE_I", "CTE_F", "CTE_STRING", "CTE_CHAR",
  "CTE_BOOL", "';'", "'('", "')'", "':'", "','", "'{'", "'}'", "'='",
  "'>'", "'<'", "'+'", "'-'", "'*'", "'/'", "$accept", "programa", "$@1",
  "$@2", "functions", "$@3", "$@4", "$@5", "$@6", "def_param", "vars",
  "def_vars", "def_id", "tipo", "coding", "bloque", "bloque_func",
  "bloque_func2", "def_estatuto", "estatuto", "asignacion", "condicion",
  "escritura", "casos", "ciclo", "lectura", "especial", "def_else",
  "mensaje", "expresion", "mas_expr", "exp", "$@7", "$@8", "termino",
  "$@9", "$@10", "factor", "varcte", "endl", 0
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
     285,   286,   287,   288,   289,   290,   291,    59,    40,    41,
      58,    44,   123,   125,    61,    62,    60,    43,    45,    42,
      47
};
# endif

/* YYR1[YYN] -- Symbol number of symbol that rule YYN derives.  */
static const yytype_uint8 yyr1[] =
{
       0,    51,    53,    54,    52,    56,    57,    55,    58,    59,
      55,    55,    60,    60,    61,    61,    62,    62,    63,    63,
      64,    64,    64,    64,    64,    65,    66,    67,    68,    69,
      69,    70,    70,    70,    70,    70,    70,    70,    71,    72,
      73,    74,    75,    76,    77,    77,    77,    77,    77,    77,
      78,    78,    79,    79,    80,    81,    81,    81,    81,    81,
      82,    83,    82,    84,    82,    85,    86,    85,    87,    85,
      88,    88,    88,    88,    89,    89,    89,    89,    89,    89,
      90,    90
};

/* YYR2[YYN] -- Number of symbols composing right hand side of rule YYN.  */
static const yytype_uint8 yyr2[] =
{
       0,     2,     0,     0,     9,     0,     0,    15,     0,     0,
      15,     0,     5,     3,     3,     0,     6,     5,     3,     1,
       1,     1,     1,     1,     1,     5,     5,     8,     5,     2,
       0,     1,     1,     1,     1,     1,     1,     1,     5,     7,
       6,    13,     9,     6,     6,     6,     6,     6,     6,     6,
       3,     0,     3,     1,     2,     2,     2,     2,     2,     0,
       1,     0,     4,     0,     4,     1,     0,     4,     0,     4,
       3,     1,     2,     2,     1,     1,     1,     1,     1,     1,
       2,     0
};

/* YYDEFACT[STATE-NAME] -- Default reduction number in state STATE-NUM.
   Performed when YYTABLE doesn't specify something else to do.  Zero
   means the default is an error.  */
static const yytype_uint8 yydefact[] =
{
       0,     0,     0,     2,     1,     0,    81,    81,    15,    80,
      81,    11,     0,     0,     3,    19,    14,     0,     0,     0,
       0,     0,     0,     0,    81,     4,     0,    20,    21,    24,
      22,    23,     0,    18,     0,     0,    30,     0,    81,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,    30,    31,    32,    33,    34,
      35,    36,    37,    81,    17,    13,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
      81,    29,    16,    81,    81,     0,    74,    75,    76,    77,
      78,    79,     0,     0,     0,     0,    59,    60,    65,    71,
       0,    53,     0,    81,     0,     0,     0,     0,     0,     0,
       0,     0,    25,     8,     5,    12,     0,    72,    73,    81,
       0,     0,     0,     0,    61,    63,    54,    66,    68,     0,
       0,     0,     0,     0,    81,     0,     0,     0,     0,     0,
       0,    15,    15,    70,     0,    57,    58,    55,    56,     0,
       0,     0,     0,    81,    52,    81,    59,    81,    38,    81,
      81,    81,    81,    81,    81,     0,     0,    81,    51,    62,
      64,    67,    69,    40,     0,     0,    43,    44,    45,    46,
      47,    48,    49,    81,     0,    81,     0,    30,    81,    39,
       0,     0,    30,    81,    30,    81,     0,     0,    81,     0,
       0,     9,     0,     6,    81,    50,    42,    81,    81,    11,
       0,    11,    26,     0,    28,    10,    81,     7,     0,     0,
      81,    81,    41,    27
};

/* YYDEFGOTO[NTERM-NUM].  */
static const yytype_int16 yydefgoto[] =
{
      -1,     2,     5,    19,    14,   142,   211,   141,   209,    35,
      11,    16,    17,    32,    25,   168,   186,   184,    54,    55,
      56,    57,    58,    59,    60,    61,    62,   189,   100,    95,
     126,    96,   149,   150,    97,   151,   152,    98,    99,     8
};

/* YYPACT[STATE-NUM] -- Index in YYTABLE of the portion describing
   STATE-NUM.  */
#define YYPACT_NINF -168
static const yytype_int16 yypact[] =
{
      15,    22,    52,  -168,  -168,    27,    47,    47,    66,  -168,
      47,    92,    85,    86,  -168,  -168,    85,   -15,    72,    91,
      18,    10,    90,    93,    47,  -168,    10,  -168,  -168,  -168,
    -168,  -168,    77,  -168,    75,    -2,    64,    80,    47,    10,
      78,    96,    82,    83,    84,   100,    87,    88,    95,    97,
      99,   101,   102,   106,   103,    64,  -168,  -168,  -168,  -168,
    -168,  -168,  -168,    47,  -168,  -168,    49,    89,     0,     0,
       0,   108,   105,     0,    34,    34,    34,    34,    34,    34,
      47,  -168,  -168,    47,    47,    10,  -168,  -168,  -168,  -168,
    -168,  -168,     0,    34,    34,   110,    -4,    36,  -168,  -168,
      39,  -168,   112,    47,   119,    94,   120,   122,   123,   124,
     125,   126,  -168,  -168,  -168,  -168,   127,  -168,  -168,    47,
       0,     0,     0,     0,  -168,  -168,  -168,  -168,  -168,   130,
       0,   114,   128,   131,    47,   132,   134,   135,   136,   137,
     138,    66,    66,  -168,   140,    41,    41,    41,    41,     0,
       0,     0,     0,    47,  -168,    47,     5,    47,  -168,    47,
      47,    47,    47,    47,    47,   141,   142,    47,   139,    36,
      36,  -168,  -168,  -168,   140,    98,  -168,  -168,  -168,  -168,
    -168,  -168,  -168,    47,   143,    47,   148,    64,    47,  -168,
     150,    64,    64,    47,    64,    47,   146,   140,    47,   153,
     149,  -168,   155,  -168,    47,  -168,  -168,    47,    47,    92,
       0,    92,  -168,   151,  -168,  -168,    47,  -168,   156,   152,
      47,    47,  -168,  -168
};

/* YYPGOTO[NTERM-NUM].  */
static const yytype_int16 yypgoto[] =
{
    -168,  -168,  -168,  -168,  -127,  -168,  -168,  -168,  -168,  -168,
     -40,  -168,   118,   -20,  -168,  -167,  -168,  -168,   -51,   -12,
    -168,  -168,  -168,  -168,  -168,  -168,  -168,  -168,  -168,   -68,
     -11,   -23,  -168,  -168,   -46,  -168,  -168,   -45,   -66,    -7
};

/* YYTABLE[YYPACT[STATE-NUM]].  What to do in state STATE-NUM.  If
   positive, shift that token.  If negative, reduce the rule which
   number is the opposite.  If YYTABLE_NINF, syntax error.  */
#define YYTABLE_NINF -1
static const yytype_uint8 yytable[] =
{
       9,   101,   102,    12,    81,   105,    37,   190,   106,   107,
     108,   109,   110,   111,    27,    28,    29,    36,     1,    65,
     120,    30,    31,    86,   116,    21,    22,   117,   118,   120,
     205,    64,    87,    88,    89,    90,    91,    40,    92,    41,
     121,   122,   123,   124,   125,     3,    84,    93,    94,   121,
     122,   123,     4,    27,    28,    29,    82,    86,    26,    22,
      30,    31,   154,    83,     6,   115,    87,    88,    89,    90,
      91,    42,     7,   112,    43,    10,   113,   114,   129,    44,
     130,    45,   215,    46,   217,   127,   128,    47,   124,   125,
      48,    49,    50,    51,    52,    53,   132,   145,   146,   147,
     148,   165,   166,   169,   170,    13,   171,   172,    15,    18,
      23,    24,   144,    33,    38,    39,    34,    63,    66,    67,
      68,    69,    70,    71,    80,    72,   103,   158,   104,    85,
     155,   134,    73,    74,    20,    75,   196,    76,   191,    77,
      78,   200,   216,   202,    79,   175,   173,   188,   174,   119,
     176,   131,   177,   178,   179,   180,   181,   182,   133,   135,
     187,   136,   137,   138,   139,   140,   143,   153,   157,   159,
     156,   160,   161,   162,   163,   164,   192,   210,   194,   199,
     193,   197,   167,   183,   185,   195,   201,   198,   203,   204,
     207,   206,   208,   220,   218,   221,     0,   212,     0,     0,
     213,   214,     0,     0,     0,     0,     0,     0,     0,   219,
       0,     0,     0,   222,   223
};

#define yypact_value_is_default(yystate) \
  ((yystate) == (-168))

#define yytable_value_is_error(yytable_value) \
  YYID (0)

static const yytype_int16 yycheck[] =
{
       7,    69,    70,    10,    55,    73,    26,   174,    74,    75,
      76,    77,    78,    79,     4,     5,     6,    24,     3,    39,
      24,    11,    12,    23,    92,    40,    41,    93,    94,    24,
     197,    38,    32,    33,    34,    35,    36,    39,    38,    41,
      44,    45,    46,    47,    48,    23,    66,    47,    48,    44,
      45,    46,     0,     4,     5,     6,    63,    23,    40,    41,
      11,    12,   130,    14,    37,    85,    32,    33,    34,    35,
      36,     7,    25,    80,    10,     9,    83,    84,    39,    15,
      41,    17,   209,    19,   211,    49,    50,    23,    47,    48,
      26,    27,    28,    29,    30,    31,   103,   120,   121,   122,
     123,   141,   142,   149,   150,    13,   151,   152,    23,    23,
      38,    20,   119,    23,    37,    40,    23,    37,    40,    23,
      38,    38,    38,    23,    21,    38,    18,   134,    23,    40,
      16,    37,    44,    38,    16,    38,   187,    38,    40,    38,
      38,   192,   210,   194,    38,   156,   153,     8,   155,    39,
     157,    39,   159,   160,   161,   162,   163,   164,    39,    39,
     167,    39,    39,    39,    39,    39,    39,    37,    37,    37,
      42,    37,    37,    37,    37,    37,   183,    22,   185,   191,
      37,   188,    42,    42,    42,    37,   193,    37,   195,    43,
      37,   198,    43,    37,    43,    43,    -1,   204,    -1,    -1,
     207,   208,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   216,
      -1,    -1,    -1,   220,   221
};

/* YYSTOS[STATE-NUM] -- The (internal number of the) accessing
   symbol of state STATE-NUM.  */
static const yytype_uint8 yystos[] =
{
       0,     3,    52,    23,     0,    53,    37,    25,    90,    90,
       9,    61,    90,    13,    55,    23,    62,    63,    23,    54,
      63,    40,    41,    38,    20,    65,    40,     4,     5,     6,
      11,    12,    64,    23,    23,    60,    90,    64,    37,    40,
      39,    41,     7,    10,    15,    17,    19,    23,    26,    27,
      28,    29,    30,    31,    69,    70,    71,    72,    73,    74,
      75,    76,    77,    37,    90,    64,    40,    23,    38,    38,
      38,    23,    38,    44,    38,    38,    38,    38,    38,    38,
      21,    69,    90,    14,    64,    40,    23,    32,    33,    34,
      35,    36,    38,    47,    48,    80,    82,    85,    88,    89,
      79,    80,    80,    18,    23,    80,    89,    89,    89,    89,
      89,    89,    90,    90,    90,    64,    80,    89,    89,    39,
      24,    44,    45,    46,    47,    48,    81,    49,    50,    39,
      41,    39,    90,    39,    37,    39,    39,    39,    39,    39,
      39,    58,    56,    39,    90,    82,    82,    82,    82,    83,
      84,    86,    87,    37,    80,    16,    42,    37,    90,    37,
      37,    37,    37,    37,    37,    61,    61,    42,    66,    85,
      85,    88,    88,    90,    90,    81,    90,    90,    90,    90,
      90,    90,    90,    42,    68,    42,    67,    90,     8,    78,
      66,    40,    90,    37,    90,    37,    69,    90,    37,    70,
      69,    90,    69,    90,    43,    66,    90,    37,    43,    59,
      22,    57,    90,    90,    90,    55,    80,    55,    43,    90,
      37,    43,    90,    90
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
#line 387 "sintax.y"
    {
		pragma.setNombre((yyvsp[(2) - (2)].sval));
	}
    break;

  case 3:

/* Line 1806 of yacc.c  */
#line 390 "sintax.y"
    {
		localFlag=false;
	}
    break;

  case 4:

/* Line 1806 of yacc.c  */
#line 393 "sintax.y"
    {  cout << "Fin del analisis del programa " << endl; }
    break;

  case 5:

/* Line 1806 of yacc.c  */
#line 398 "sintax.y"
    {
		localFlag=true;
		entry.setNombre((yyvsp[(2) - (8)].sval));
		entry.setDirVirtual(dirFunctions+contFunctions);
		contFunctions++;
			if (!pragma.tableAddEntry(entry))
			{
				cout<<"No se pudo agregar: "<< (yyvsp[(2) - (8)].sval) <<endl;
			}
			else
			{
				cout<<"Funcion: "<< (yyvsp[(2) - (8)].sval)<<" Direccion: "<<dirFunctions+contFunctions<<"	Tipo:" <<(yyvsp[(7) - (8)].ival)<<endl;
			}

			myQuadStructure<<"Funcion \t" <<(yyvsp[(2) - (8)].sval)<<" \t" <<decodificaTipo((yyvsp[(7) - (8)].ival))<<endl;



			while (IDstruct.dequeue(tempStr))
			{
	
				myQuadStructure<<"pass	\t"<<tempStr.IDstr<< "\t"<<decodificaTipo(tempStr.tipoVar)<<endl;
			
			}
			myQuadStructure<<"end Function"<<endl;


	}
    break;

  case 6:

/* Line 1806 of yacc.c  */
#line 430 "sintax.y"
    {entry.setVarTable(new HashMap<varEntry>);}
    break;

  case 8:

/* Line 1806 of yacc.c  */
#line 434 "sintax.y"
    {
		localFlag=true;
		entry.setNombre((yyvsp[(2) - (8)].sval));
		entry.setDirVirtual(dirFunctions+contFunctions);
		contFunctions++;
		if (!pragma.tableAddEntry(entry))
			{
				cout<<"No se pudo agregar: "<< (yyvsp[(2) - (8)].sval) <<endl;
			}
		else
			{
			cout<<"Funcion: "<< (yyvsp[(2) - (8)].sval)<<" Direccion: "<<dirFunctions+contFunctions<<"	Tipo: void"<<endl;
			}
		}
    break;

  case 9:

/* Line 1806 of yacc.c  */
#line 450 "sintax.y"
    {entry.setVarTable(new HashMap<varEntry>);}
    break;

  case 12:

/* Line 1806 of yacc.c  */
#line 457 "sintax.y"
    {
		 tempStr.IDstr=(yyvsp[(3) - (5)].sval);
		 tempStr.tipoVar=(yyvsp[(5) - (5)].ival);
		IDstruct.enqueue(tempStr);
		}
    break;

  case 13:

/* Line 1806 of yacc.c  */
#line 463 "sintax.y"
    {

		 tempStr.IDstr=(yyvsp[(1) - (3)].sval);
		 tempStr.tipoVar=(yyvsp[(3) - (3)].ival);
		IDstruct.enqueue(tempStr);
	}
    break;

  case 16:

/* Line 1806 of yacc.c  */
#line 478 "sintax.y"
    {
			if (!saveVars((yyvsp[(4) - (6)].ival)))
			exit (-1);
		}
    break;

  case 17:

/* Line 1806 of yacc.c  */
#line 484 "sintax.y"
    {
			if (!saveVars((yyvsp[(3) - (5)].ival)))
			exit (-1);
		}
    break;

  case 18:

/* Line 1806 of yacc.c  */
#line 492 "sintax.y"
    {
		IDQueue.enqueue((yyvsp[(3) - (3)].sval));
		
	}
    break;

  case 19:

/* Line 1806 of yacc.c  */
#line 497 "sintax.y"
    {
		IDQueue.enqueue((yyvsp[(1) - (1)].sval));
	}
    break;

  case 20:

/* Line 1806 of yacc.c  */
#line 503 "sintax.y"
    {(yyval.ival)=2;}
    break;

  case 21:

/* Line 1806 of yacc.c  */
#line 504 "sintax.y"
    {(yyval.ival)=3;}
    break;

  case 22:

/* Line 1806 of yacc.c  */
#line 505 "sintax.y"
    {(yyval.ival)=1;}
    break;

  case 23:

/* Line 1806 of yacc.c  */
#line 506 "sintax.y"
    {(yyval.ival)=0;}
    break;

  case 24:

/* Line 1806 of yacc.c  */
#line 507 "sintax.y"
    {(yyval.ival)=4;}
    break;

  case 61:

/* Line 1806 of yacc.c  */
#line 597 "sintax.y"
    {
		pilaOper.push('+');
		
	}
    break;

  case 62:

/* Line 1806 of yacc.c  */
#line 603 "sintax.y"
    {
		generaExpresion();
	 }
    break;

  case 63:

/* Line 1806 of yacc.c  */
#line 607 "sintax.y"
    {
		pilaOper.push('-');
	}
    break;

  case 64:

/* Line 1806 of yacc.c  */
#line 611 "sintax.y"
    {
		generaExpresion();
	 }
    break;

  case 66:

/* Line 1806 of yacc.c  */
#line 621 "sintax.y"
    {
		pilaOper.push('*');
	}
    break;

  case 67:

/* Line 1806 of yacc.c  */
#line 625 "sintax.y"
    {
		generaExpresion();
	}
    break;

  case 68:

/* Line 1806 of yacc.c  */
#line 629 "sintax.y"
    {
	pilaOper.push('/');
	}
    break;

  case 69:

/* Line 1806 of yacc.c  */
#line 633 "sintax.y"
    {
	generaExpresion();
	 }
    break;

  case 74:

/* Line 1806 of yacc.c  */
#line 645 "sintax.y"
    {
		int tipo;
		tipo=buscaID((yyvsp[(1) - (1)].sval));
		(yyval.ival)=tipo;	
		cout<<"Variable: "<<(yyvsp[(1) - (1)].sval)<<"\tTipo:"<<tipo<<endl;
		pilaO.push((yyvsp[(1) - (1)].sval));

	}
    break;

  case 75:

/* Line 1806 of yacc.c  */
#line 653 "sintax.y"
    {(yyval.ival)=2;}
    break;

  case 76:

/* Line 1806 of yacc.c  */
#line 654 "sintax.y"
    {(yyval.ival)=3;}
    break;

  case 77:

/* Line 1806 of yacc.c  */
#line 655 "sintax.y"
    {(yyval.ival)=1;}
    break;

  case 78:

/* Line 1806 of yacc.c  */
#line 656 "sintax.y"
    {(yyval.ival)=0;}
    break;

  case 79:

/* Line 1806 of yacc.c  */
#line 657 "sintax.y"
    {(yyval.ival)=4;}
    break;



/* Line 1806 of yacc.c  */
#line 2203 "sintax.tab.c"
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
#line 664 "sintax.y"


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

