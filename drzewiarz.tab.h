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
    WIERZCH = 267,                 /* WIERZCH  */
    SPOD = 268,                    /* SPOD  */
    NADAJ = 269,                   /* NADAJ  */
    NDODAJ = 270,                  /* NDODAJ  */
    NODEJMIJ = 271,                /* NODEJMIJ  */
    NMNOZ = 272,                   /* NMNOZ  */
    NDZIEL = 273,                  /* NDZIEL  */
    NRESZTA = 274,                 /* NRESZTA  */
    NIE = 275,                     /* NIE  */
    ROWNE = 276,                   /* ROWNE  */
    ROZNE = 277,                   /* ROZNE  */
    WIEKSZE = 278,                 /* WIEKSZE  */
    MNIEJSZE = 279,                /* MNIEJSZE  */
    WIEKSZE_BADZ_ROWNE = 280,      /* WIEKSZE_BADZ_ROWNE  */
    MNIEJSZE_BADZ_ROWNE = 281,     /* MNIEJSZE_BADZ_ROWNE  */
    I = 282,                       /* I  */
    LUB = 283,                     /* LUB  */
    SREDNIK = 284,                 /* SREDNIK  */
    PRZECINEK = 285,               /* PRZECINEK  */
    LNAWIAS = 286,                 /* LNAWIAS  */
    PNAWIAS = 287,                 /* PNAWIAS  */
    LSPIECIE = 288,                /* LSPIECIE  */
    PSPIECIE = 289,                /* PSPIECIE  */
    WYKONAJ = 290,                 /* WYKONAJ  */
    KROPKA = 291                   /* KROPKA  */
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

#line 107 "drzewiarz.tab.h"

};
typedef union YYSTYPE YYSTYPE;
# define YYSTYPE_IS_TRIVIAL 1
# define YYSTYPE_IS_DECLARED 1
#endif


extern YYSTYPE yylval;


int yyparse (void);


#endif /* !YY_YY_DRZEWIARZ_TAB_H_INCLUDED  */
