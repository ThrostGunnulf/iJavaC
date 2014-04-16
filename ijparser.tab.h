/* A Bison parser, made by GNU Bison 2.3.  */

/* Skeleton interface for Bison's Yacc-like parsers in C

   Copyright (C) 1984, 1989, 1990, 2000, 2001, 2002, 2003, 2004, 2005, 2006
   Free Software Foundation, Inc.

   This program is free software; you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation; either version 2, or (at your option)
   any later version.

   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with this program; if not, write to the Free Software
   Foundation, Inc., 51 Franklin Street, Fifth Floor,
   Boston, MA 02110-1301, USA.  */

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
     INT = 258,
     BOOL = 259,
     NEW = 260,
     IF = 261,
     ELSE = 262,
     WHILE = 263,
     PRINT = 264,
     PARSEINT = 265,
     CLASS = 266,
     PUBLIC = 267,
     STATIC = 268,
     VOID = 269,
     STRING = 270,
     DOTLENGTH = 271,
     RETURN = 272,
     AND = 273,
     OR = 274,
     RELCOMPAR = 275,
     BOOLLIT = 276,
     ID = 277,
     INTLIT = 278,
     RESERVED = 279,
     EQUALITY = 280,
     ADDITIVE = 281,
     MULTIPLIC = 282,
     UNARY = 283,
     EXPR1REDUCE = 284,
     IFX = 285
   };
#endif
/* Tokens.  */
#define INT 258
#define BOOL 259
#define NEW 260
#define IF 261
#define ELSE 262
#define WHILE 263
#define PRINT 264
#define PARSEINT 265
#define CLASS 266
#define PUBLIC 267
#define STATIC 268
#define VOID 269
#define STRING 270
#define DOTLENGTH 271
#define RETURN 272
#define AND 273
#define OR 274
#define RELCOMPAR 275
#define BOOLLIT 276
#define ID 277
#define INTLIT 278
#define RESERVED 279
#define EQUALITY 280
#define ADDITIVE 281
#define MULTIPLIC 282
#define UNARY 283
#define EXPR1REDUCE 284
#define IFX 285




#if ! defined YYSTYPE && ! defined YYSTYPE_IS_DECLARED
typedef int YYSTYPE;
# define yystype YYSTYPE /* obsolescent; will be withdrawn */
# define YYSTYPE_IS_DECLARED 1
# define YYSTYPE_IS_TRIVIAL 1
#endif

extern YYSTYPE yylval;

