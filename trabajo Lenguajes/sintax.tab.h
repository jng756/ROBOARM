/* A Bison parser, made by GNU Bison 2.5.  */

/* Bison interface for Yacc-like parsers in C
   
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

/* Line 2068 of yacc.c  */
#line 20 "sintax.y"

	int ival;
	float fval;
	char *sval;
	char cval;
	bool bval;



/* Line 2068 of yacc.c  */
#line 96 "sintax.tab.h"
} YYSTYPE;
# define YYSTYPE_IS_TRIVIAL 1
# define yystype YYSTYPE /* obsolescent; will be withdrawn */
# define YYSTYPE_IS_DECLARED 1
#endif

extern YYSTYPE yylval;


