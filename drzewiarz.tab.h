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
    JESLI = 259,                   /* JESLI  */
    POKI = 260,                    /* POKI  */
    DODAJ = 261,                   /* DODAJ  */
    ODEJMIJ = 262,                 /* ODEJMIJ  */
    MNOZ = 263,                    /* MNOZ  */
    DZIEL = 264,                   /* DZIEL  */
    RESZTA = 265,                  /* RESZTA  */
    NADAJ = 266,                   /* NADAJ  */
    NDODAJ = 267,                  /* NDODAJ  */
    NODEJMIJ = 268,                /* NODEJMIJ  */
    NMNOZ = 269,                   /* NMNOZ  */
    NDZIEL = 270,                  /* NDZIEL  */
    NRESZTA = 271,                 /* NRESZTA  */
    NIE = 272,                     /* NIE  */
    ROWNE = 273,                   /* ROWNE  */
    ROZNE = 274,                   /* ROZNE  */
    WIEKSZE = 275,                 /* WIEKSZE  */
    MNIEJSZE = 276,                /* MNIEJSZE  */
    WIEKSZE_BADZ_ROWNE = 277,      /* WIEKSZE_BADZ_ROWNE  */
    MNIEJSZE_BADZ_ROWNE = 278,     /* MNIEJSZE_BADZ_ROWNE  */
    I = 279,                       /* I  */
    LUB = 280,                     /* LUB  */
    SREDNIK = 281,                 /* SREDNIK  */
    LNAWIAS = 282,                 /* LNAWIAS  */
    PNAWIAS = 283,                 /* PNAWIAS  */
    LSPIECIE = 284,                /* LSPIECIE  */
    PSPIECIE = 285                 /* PSPIECIE  */
  };
  typedef enum yytokentype yytoken_kind_t;
#endif

/* Value type.  */
#if ! defined YYSTYPE && ! defined YYSTYPE_IS_DECLARED
union YYSTYPE
{
#line 17 "drzewiarz.y"

    size_t zmienna;
    GalazPodwojna* galazPodwojna;
    Rozgalezienie* rozgalezienie;

#line 100 "drzewiarz.tab.h"

};
typedef union YYSTYPE YYSTYPE;
# define YYSTYPE_IS_TRIVIAL 1
# define YYSTYPE_IS_DECLARED 1
#endif


extern YYSTYPE yylval;


int yyparse (void);


#endif /* !YY_YY_DRZEWIARZ_TAB_H_INCLUDED  */
