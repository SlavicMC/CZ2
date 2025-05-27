/* A Bison parser, made by GNU Bison 3.8.2.  */

/* Bison interface for Yacc-like parsers in C

   Copyright (C) 1984, 1989-1990, 2000-2015, 2018-2021 Free Software Foundation,
   Inc.

   This program is free software: you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation, either version 3 of the License, or
   (at your option) any later version.

   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with this program.  If not, see <https://www.gnu.org/licenses/>.  */

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

/* DO NOT RELY ON FEATURES THAT ARE NOT DOCUMENTED in the manual,
   especially those whose name start with YY_ or yy_.  They are
   private implementation details that can be changed or removed.  */

#ifndef YY_YY_DRZEWIARZ_TAB_H_INCLUDED
# define YY_YY_DRZEWIARZ_TAB_H_INCLUDED
/* Debug traces.  */
#ifndef YYDEBUG
# define YYDEBUG 0
#endif
#if YYDEBUG
extern int yydebug;
#endif

/* Token kinds.  */
#ifndef YYTOKENTYPE
# define YYTOKENTYPE
  enum yytokentype
  {
    YYEMPTY = -2,
    YYEOF = 0,                     /* "end of file"  */
    YYerror = 256,                 /* error  */
    YYUNDEF = 257,                 /* "invalid token"  */
    ZMIENNA = 258,                 /* ZMIENNA  */
    POLECENIE = 259,               /* POLECENIE  */
    JESLI = 260,                   /* JESLI  */
    POKI = 261,                    /* POKI  */
    DODAJ = 262,                   /* DODAJ  */
    ODEJMIJ = 263,                 /* ODEJMIJ  */
    MNOZ = 264,                    /* MNOZ  */
    DZIEL = 265,                   /* DZIEL  */
    RESZTA = 266,                  /* RESZTA  */
    NADAJ = 267,                   /* NADAJ  */
    NDODAJ = 268,                  /* NDODAJ  */
    NODEJMIJ = 269,                /* NODEJMIJ  */
    NMNOZ = 270,                   /* NMNOZ  */
    NDZIEL = 271,                  /* NDZIEL  */
    NRESZTA = 272,                 /* NRESZTA  */
    NIE = 273,                     /* NIE  */
    ROWNE = 274,                   /* ROWNE  */
    ROZNE = 275,                   /* ROZNE  */
    WIEKSZE = 276,                 /* WIEKSZE  */
    MNIEJSZE = 277,                /* MNIEJSZE  */
    WIEKSZE_BADZ_ROWNE = 278,      /* WIEKSZE_BADZ_ROWNE  */
    MNIEJSZE_BADZ_ROWNE = 279,     /* MNIEJSZE_BADZ_ROWNE  */
    I = 280,                       /* I  */
    LUB = 281,                     /* LUB  */
    SREDNIK = 282,                 /* SREDNIK  */
    PRZECINEK = 283,               /* PRZECINEK  */
    LNAWIAS = 284,                 /* LNAWIAS  */
    PNAWIAS = 285,                 /* PNAWIAS  */
    LSPIECIE = 286,                /* LSPIECIE  */
    PSPIECIE = 287                 /* PSPIECIE  */
  };
  typedef enum yytokentype yytoken_kind_t;
#endif

/* Value type.  */
#if ! defined YYSTYPE && ! defined YYSTYPE_IS_DECLARED
union YYSTYPE
{
#line 24 "drzewiarz.y"

    size_t zmienna;
    char* polecenie;
    GalazPodwojna* galazPodwojna;
    Rozgalezienie* rozgalezienie;

#line 103 "drzewiarz.tab.h"

};
typedef union YYSTYPE YYSTYPE;
# define YYSTYPE_IS_TRIVIAL 1
# define YYSTYPE_IS_DECLARED 1
#endif


extern YYSTYPE yylval;


int yyparse (void);


#endif /* !YY_YY_DRZEWIARZ_TAB_H_INCLUDED  */
