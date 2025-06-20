/* A Bison parser, made by GNU Bison 3.8.2.  */

/* Bison implementation for Yacc-like parsers in C

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

/* C LALR(1) parser skeleton written by Richard Stallman, by
   simplifying the original so-called "semantic" parser.  */

/* DO NOT RELY ON FEATURES THAT ARE NOT DOCUMENTED in the manual,
   especially those whose name start with YY_ or yy_.  They are
   private implementation details that can be changed or removed.  */

/* All symbols defined below should begin with yy or YY, to avoid
   infringing on user name space.  This should be done even for local
   variables, as they might otherwise be expanded by user macros.
   There are some unavoidable exceptions within include files to
   define necessary library symbols; they are noted "INFRINGES ON
   USER NAME SPACE" below.  */

/* Identify Bison output, and Bison version.  */
#define YYBISON 30802

/* Bison version string.  */
#define YYBISON_VERSION "3.8.2"

/* Skeleton name.  */
#define YYSKELETON_NAME "yacc.c"

/* Pure parsers.  */
#define YYPURE 0

/* Push parsers.  */
#define YYPUSH 0

/* Pull parsers.  */
#define YYPULL 1




/* First part of user prologue.  */
#line 1 "drzewiarz.y"

#include "dzielone.h"
#include "powszechne.h"
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

extern int yylex(void);
void yyerror(const char *s);

Czastka* czastki = NULL;
size_t odnosnikCzastki;
size_t liczbaCzastek;
Rozgalezienie* drzewo = NULL;

//#define WYKONAJ 101

char* polozenie;
char* pPola;
char* kPola;


#line 94 "drzewiarz.tab.c"

# ifndef YY_CAST
#  ifdef __cplusplus
#   define YY_CAST(Type, Val) static_cast<Type> (Val)
#   define YY_REINTERPRET_CAST(Type, Val) reinterpret_cast<Type> (Val)
#  else
#   define YY_CAST(Type, Val) ((Type) (Val))
#   define YY_REINTERPRET_CAST(Type, Val) ((Type) (Val))
#  endif
# endif
# ifndef YY_NULLPTR
#  if defined __cplusplus
#   if 201103L <= __cplusplus
#    define YY_NULLPTR nullptr
#   else
#    define YY_NULLPTR 0
#   endif
#  else
#   define YY_NULLPTR ((void*)0)
#  endif
# endif

#include "drzewiarz.tab.h"
/* Symbol kind.  */
enum yysymbol_kind_t
{
  YYSYMBOL_YYEMPTY = -2,
  YYSYMBOL_YYEOF = 0,                      /* "end of file"  */
  YYSYMBOL_YYerror = 1,                    /* error  */
  YYSYMBOL_YYUNDEF = 2,                    /* "invalid token"  */
  YYSYMBOL_ZMIENNA = 3,                    /* ZMIENNA  */
  YYSYMBOL_POLECENIE = 4,                  /* POLECENIE  */
  YYSYMBOL_JESLI = 5,                      /* JESLI  */
  YYSYMBOL_POKI = 6,                       /* POKI  */
  YYSYMBOL_DODAJ = 7,                      /* DODAJ  */
  YYSYMBOL_ODEJMIJ = 8,                    /* ODEJMIJ  */
  YYSYMBOL_MNOZ = 9,                       /* MNOZ  */
  YYSYMBOL_DZIEL = 10,                     /* DZIEL  */
  YYSYMBOL_RESZTA = 11,                    /* RESZTA  */
  YYSYMBOL_WIERZCH = 12,                   /* WIERZCH  */
  YYSYMBOL_SPOD = 13,                      /* SPOD  */
  YYSYMBOL_NADAJ = 14,                     /* NADAJ  */
  YYSYMBOL_NDODAJ = 15,                    /* NDODAJ  */
  YYSYMBOL_NODEJMIJ = 16,                  /* NODEJMIJ  */
  YYSYMBOL_NMNOZ = 17,                     /* NMNOZ  */
  YYSYMBOL_NDZIEL = 18,                    /* NDZIEL  */
  YYSYMBOL_NRESZTA = 19,                   /* NRESZTA  */
  YYSYMBOL_NIE = 20,                       /* NIE  */
  YYSYMBOL_ROWNE = 21,                     /* ROWNE  */
  YYSYMBOL_ROZNE = 22,                     /* ROZNE  */
  YYSYMBOL_WIEKSZE = 23,                   /* WIEKSZE  */
  YYSYMBOL_MNIEJSZE = 24,                  /* MNIEJSZE  */
  YYSYMBOL_WIEKSZE_BADZ_ROWNE = 25,        /* WIEKSZE_BADZ_ROWNE  */
  YYSYMBOL_MNIEJSZE_BADZ_ROWNE = 26,       /* MNIEJSZE_BADZ_ROWNE  */
  YYSYMBOL_I = 27,                         /* I  */
  YYSYMBOL_LUB = 28,                       /* LUB  */
  YYSYMBOL_SREDNIK = 29,                   /* SREDNIK  */
  YYSYMBOL_PRZECINEK = 30,                 /* PRZECINEK  */
  YYSYMBOL_LNAWIAS = 31,                   /* LNAWIAS  */
  YYSYMBOL_PNAWIAS = 32,                   /* PNAWIAS  */
  YYSYMBOL_LSPIECIE = 33,                  /* LSPIECIE  */
  YYSYMBOL_PSPIECIE = 34,                  /* PSPIECIE  */
  YYSYMBOL_WYKONAJ = 35,                   /* WYKONAJ  */
  YYSYMBOL_KROPKA = 36,                    /* KROPKA  */
  YYSYMBOL_YYACCEPT = 37,                  /* $accept  */
  YYSYMBOL_calosc = 38,                    /* calosc  */
  YYSYMBOL_wyrazenia = 39,                 /* wyrazenia  */
  YYSYMBOL_wyrazenie = 40,                 /* wyrazenie  */
  YYSYMBOL_dzialania = 41,                 /* dzialania  */
  YYSYMBOL_dzialanie = 42                  /* dzialanie  */
};
typedef enum yysymbol_kind_t yysymbol_kind_t;




#ifdef short
# undef short
#endif

/* On compilers that do not define __PTRDIFF_MAX__ etc., make sure
   <limits.h> and (if available) <stdint.h> are included
   so that the code can choose integer types of a good width.  */

#ifndef __PTRDIFF_MAX__
# include <limits.h> /* INFRINGES ON USER NAME SPACE */
# if defined __STDC_VERSION__ && 199901 <= __STDC_VERSION__
#  include <stdint.h> /* INFRINGES ON USER NAME SPACE */
#  define YY_STDINT_H
# endif
#endif

/* Narrow types that promote to a signed type and that can represent a
   signed or unsigned integer of at least N bits.  In tables they can
   save space and decrease cache pressure.  Promoting to a signed type
   helps avoid bugs in integer arithmetic.  */

#ifdef __INT_LEAST8_MAX__
typedef __INT_LEAST8_TYPE__ yytype_int8;
#elif defined YY_STDINT_H
typedef int_least8_t yytype_int8;
#else
typedef signed char yytype_int8;
#endif

#ifdef __INT_LEAST16_MAX__
typedef __INT_LEAST16_TYPE__ yytype_int16;
#elif defined YY_STDINT_H
typedef int_least16_t yytype_int16;
#else
typedef short yytype_int16;
#endif

/* Work around bug in HP-UX 11.23, which defines these macros
   incorrectly for preprocessor constants.  This workaround can likely
   be removed in 2023, as HPE has promised support for HP-UX 11.23
   (aka HP-UX 11i v2) only through the end of 2022; see Table 2 of
   <https://h20195.www2.hpe.com/V2/getpdf.aspx/4AA4-7673ENW.pdf>.  */
#ifdef __hpux
# undef UINT_LEAST8_MAX
# undef UINT_LEAST16_MAX
# define UINT_LEAST8_MAX 255
# define UINT_LEAST16_MAX 65535
#endif

#if defined __UINT_LEAST8_MAX__ && __UINT_LEAST8_MAX__ <= __INT_MAX__
typedef __UINT_LEAST8_TYPE__ yytype_uint8;
#elif (!defined __UINT_LEAST8_MAX__ && defined YY_STDINT_H \
       && UINT_LEAST8_MAX <= INT_MAX)
typedef uint_least8_t yytype_uint8;
#elif !defined __UINT_LEAST8_MAX__ && UCHAR_MAX <= INT_MAX
typedef unsigned char yytype_uint8;
#else
typedef short yytype_uint8;
#endif

#if defined __UINT_LEAST16_MAX__ && __UINT_LEAST16_MAX__ <= __INT_MAX__
typedef __UINT_LEAST16_TYPE__ yytype_uint16;
#elif (!defined __UINT_LEAST16_MAX__ && defined YY_STDINT_H \
       && UINT_LEAST16_MAX <= INT_MAX)
typedef uint_least16_t yytype_uint16;
#elif !defined __UINT_LEAST16_MAX__ && USHRT_MAX <= INT_MAX
typedef unsigned short yytype_uint16;
#else
typedef int yytype_uint16;
#endif

#ifndef YYPTRDIFF_T
# if defined __PTRDIFF_TYPE__ && defined __PTRDIFF_MAX__
#  define YYPTRDIFF_T __PTRDIFF_TYPE__
#  define YYPTRDIFF_MAXIMUM __PTRDIFF_MAX__
# elif defined PTRDIFF_MAX
#  ifndef ptrdiff_t
#   include <stddef.h> /* INFRINGES ON USER NAME SPACE */
#  endif
#  define YYPTRDIFF_T ptrdiff_t
#  define YYPTRDIFF_MAXIMUM PTRDIFF_MAX
# else
#  define YYPTRDIFF_T long
#  define YYPTRDIFF_MAXIMUM LONG_MAX
# endif
#endif

#ifndef YYSIZE_T
# ifdef __SIZE_TYPE__
#  define YYSIZE_T __SIZE_TYPE__
# elif defined size_t
#  define YYSIZE_T size_t
# elif defined __STDC_VERSION__ && 199901 <= __STDC_VERSION__
#  include <stddef.h> /* INFRINGES ON USER NAME SPACE */
#  define YYSIZE_T size_t
# else
#  define YYSIZE_T unsigned
# endif
#endif

#define YYSIZE_MAXIMUM                                  \
  YY_CAST (YYPTRDIFF_T,                                 \
           (YYPTRDIFF_MAXIMUM < YY_CAST (YYSIZE_T, -1)  \
            ? YYPTRDIFF_MAXIMUM                         \
            : YY_CAST (YYSIZE_T, -1)))

#define YYSIZEOF(X) YY_CAST (YYPTRDIFF_T, sizeof (X))


/* Stored state numbers (used for stacks). */
typedef yytype_int8 yy_state_t;

/* State numbers in computations.  */
typedef int yy_state_fast_t;

#ifndef YY_
# if defined YYENABLE_NLS && YYENABLE_NLS
#  if ENABLE_NLS
#   include <libintl.h> /* INFRINGES ON USER NAME SPACE */
#   define YY_(Msgid) dgettext ("bison-runtime", Msgid)
#  endif
# endif
# ifndef YY_
#  define YY_(Msgid) Msgid
# endif
#endif


#ifndef YY_ATTRIBUTE_PURE
# if defined __GNUC__ && 2 < __GNUC__ + (96 <= __GNUC_MINOR__)
#  define YY_ATTRIBUTE_PURE __attribute__ ((__pure__))
# else
#  define YY_ATTRIBUTE_PURE
# endif
#endif

#ifndef YY_ATTRIBUTE_UNUSED
# if defined __GNUC__ && 2 < __GNUC__ + (7 <= __GNUC_MINOR__)
#  define YY_ATTRIBUTE_UNUSED __attribute__ ((__unused__))
# else
#  define YY_ATTRIBUTE_UNUSED
# endif
#endif

/* Suppress unused-variable warnings by "using" E.  */
#if ! defined lint || defined __GNUC__
# define YY_USE(E) ((void) (E))
#else
# define YY_USE(E) /* empty */
#endif

/* Suppress an incorrect diagnostic about yylval being uninitialized.  */
#if defined __GNUC__ && ! defined __ICC && 406 <= __GNUC__ * 100 + __GNUC_MINOR__
# if __GNUC__ * 100 + __GNUC_MINOR__ < 407
#  define YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN                           \
    _Pragma ("GCC diagnostic push")                                     \
    _Pragma ("GCC diagnostic ignored \"-Wuninitialized\"")
# else
#  define YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN                           \
    _Pragma ("GCC diagnostic push")                                     \
    _Pragma ("GCC diagnostic ignored \"-Wuninitialized\"")              \
    _Pragma ("GCC diagnostic ignored \"-Wmaybe-uninitialized\"")
# endif
# define YY_IGNORE_MAYBE_UNINITIALIZED_END      \
    _Pragma ("GCC diagnostic pop")
#else
# define YY_INITIAL_VALUE(Value) Value
#endif
#ifndef YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
# define YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
# define YY_IGNORE_MAYBE_UNINITIALIZED_END
#endif
#ifndef YY_INITIAL_VALUE
# define YY_INITIAL_VALUE(Value) /* Nothing. */
#endif

#if defined __cplusplus && defined __GNUC__ && ! defined __ICC && 6 <= __GNUC__
# define YY_IGNORE_USELESS_CAST_BEGIN                          \
    _Pragma ("GCC diagnostic push")                            \
    _Pragma ("GCC diagnostic ignored \"-Wuseless-cast\"")
# define YY_IGNORE_USELESS_CAST_END            \
    _Pragma ("GCC diagnostic pop")
#endif
#ifndef YY_IGNORE_USELESS_CAST_BEGIN
# define YY_IGNORE_USELESS_CAST_BEGIN
# define YY_IGNORE_USELESS_CAST_END
#endif


#define YY_ASSERT(E) ((void) (0 && (E)))

#if !defined yyoverflow

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
#    if ! defined _ALLOCA_H && ! defined EXIT_SUCCESS
#     include <stdlib.h> /* INFRINGES ON USER NAME SPACE */
      /* Use EXIT_SUCCESS as a witness for stdlib.h.  */
#     ifndef EXIT_SUCCESS
#      define EXIT_SUCCESS 0
#     endif
#    endif
#   endif
#  endif
# endif

# ifdef YYSTACK_ALLOC
   /* Pacify GCC's 'empty if-body' warning.  */
#  define YYSTACK_FREE(Ptr) do { /* empty */; } while (0)
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
#   if ! defined malloc && ! defined EXIT_SUCCESS
void *malloc (YYSIZE_T); /* INFRINGES ON USER NAME SPACE */
#   endif
#  endif
#  ifndef YYFREE
#   define YYFREE free
#   if ! defined free && ! defined EXIT_SUCCESS
void free (void *); /* INFRINGES ON USER NAME SPACE */
#   endif
#  endif
# endif
#endif /* !defined yyoverflow */

#if (! defined yyoverflow \
     && (! defined __cplusplus \
         || (defined YYSTYPE_IS_TRIVIAL && YYSTYPE_IS_TRIVIAL)))

/* A type that is properly aligned for any stack member.  */
union yyalloc
{
  yy_state_t yyss_alloc;
  YYSTYPE yyvs_alloc;
};

/* The size of the maximum gap between one aligned stack and the next.  */
# define YYSTACK_GAP_MAXIMUM (YYSIZEOF (union yyalloc) - 1)

/* The size of an array large to enough to hold all stacks, each with
   N elements.  */
# define YYSTACK_BYTES(N) \
     ((N) * (YYSIZEOF (yy_state_t) + YYSIZEOF (YYSTYPE)) \
      + YYSTACK_GAP_MAXIMUM)

# define YYCOPY_NEEDED 1

/* Relocate STACK from its old location to the new one.  The
   local variables YYSIZE and YYSTACKSIZE give the old and new number of
   elements in the stack, and YYPTR gives the new location of the
   stack.  Advance YYPTR to a properly aligned location for the next
   stack.  */
# define YYSTACK_RELOCATE(Stack_alloc, Stack)                           \
    do                                                                  \
      {                                                                 \
        YYPTRDIFF_T yynewbytes;                                         \
        YYCOPY (&yyptr->Stack_alloc, Stack, yysize);                    \
        Stack = &yyptr->Stack_alloc;                                    \
        yynewbytes = yystacksize * YYSIZEOF (*Stack) + YYSTACK_GAP_MAXIMUM; \
        yyptr += yynewbytes / YYSIZEOF (*yyptr);                        \
      }                                                                 \
    while (0)

#endif

#if defined YYCOPY_NEEDED && YYCOPY_NEEDED
/* Copy COUNT objects from SRC to DST.  The source and destination do
   not overlap.  */
# ifndef YYCOPY
#  if defined __GNUC__ && 1 < __GNUC__
#   define YYCOPY(Dst, Src, Count) \
      __builtin_memcpy (Dst, Src, YY_CAST (YYSIZE_T, (Count)) * sizeof (*(Src)))
#  else
#   define YYCOPY(Dst, Src, Count)              \
      do                                        \
        {                                       \
          YYPTRDIFF_T yyi;                      \
          for (yyi = 0; yyi < (Count); yyi++)   \
            (Dst)[yyi] = (Src)[yyi];            \
        }                                       \
      while (0)
#  endif
# endif
#endif /* !YYCOPY_NEEDED */

/* YYFINAL -- State number of the termination state.  */
#define YYFINAL  5
/* YYLAST -- Last index in YYTABLE.  */
#define YYLAST   310

/* YYNTOKENS -- Number of terminals.  */
#define YYNTOKENS  37
/* YYNNTS -- Number of nonterminals.  */
#define YYNNTS  6
/* YYNRULES -- Number of rules.  */
#define YYNRULES  41
/* YYNSTATES -- Number of states.  */
#define YYNSTATES  84

/* YYMAXUTOK -- Last valid token kind.  */
#define YYMAXUTOK   291


/* YYTRANSLATE(TOKEN-NUM) -- Symbol number corresponding to TOKEN-NUM
   as returned by yylex, with out-of-bounds checking.  */
#define YYTRANSLATE(YYX)                                \
  (0 <= (YYX) && (YYX) <= YYMAXUTOK                     \
   ? YY_CAST (yysymbol_kind_t, yytranslate[YYX])        \
   : YYSYMBOL_YYUNDEF)

/* YYTRANSLATE[TOKEN-NUM] -- Symbol number corresponding to TOKEN-NUM
   as returned by yylex.  */
static const yytype_int8 yytranslate[] =
{
       0,     2,     2,     2,     2,     2,     2,     2,     2,     2,
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
/* YYRLINE[YYN] -- Source line where rule number YYN was defined.  */
static const yytype_int8 yyrline[] =
{
       0,    57,    57,    58,    63,    64,    68,    69,    70,    71,
      72,    73,    74,    78,    79,    83,    84,    85,    86,    87,
      88,    89,    90,    91,    92,    93,    94,    95,    96,    97,
      98,    99,   100,   101,   102,   103,   104,   105,   106,   107,
     108,   109
};
#endif

/** Accessing symbol of state STATE.  */
#define YY_ACCESSING_SYMBOL(State) YY_CAST (yysymbol_kind_t, yystos[State])

#if YYDEBUG || 0
/* The user-facing name of the symbol whose (internal) number is
   YYSYMBOL.  No bounds checking.  */
static const char *yysymbol_name (yysymbol_kind_t yysymbol) YY_ATTRIBUTE_UNUSED;

/* YYTNAME[SYMBOL-NUM] -- String name of the symbol SYMBOL-NUM.
   First, the terminals, then, starting at YYNTOKENS, nonterminals.  */
static const char *const yytname[] =
{
  "\"end of file\"", "error", "\"invalid token\"", "ZMIENNA", "POLECENIE",
  "JESLI", "POKI", "DODAJ", "ODEJMIJ", "MNOZ", "DZIEL", "RESZTA",
  "WIERZCH", "SPOD", "NADAJ", "NDODAJ", "NODEJMIJ", "NMNOZ", "NDZIEL",
  "NRESZTA", "NIE", "ROWNE", "ROZNE", "WIEKSZE", "MNIEJSZE",
  "WIEKSZE_BADZ_ROWNE", "MNIEJSZE_BADZ_ROWNE", "I", "LUB", "SREDNIK",
  "PRZECINEK", "LNAWIAS", "PNAWIAS", "LSPIECIE", "PSPIECIE", "WYKONAJ",
  "KROPKA", "$accept", "calosc", "wyrazenia", "wyrazenie", "dzialania",
  "dzialanie", YY_NULLPTR
};

static const char *
yysymbol_name (yysymbol_kind_t yysymbol)
{
  return yytname[yysymbol];
}
#endif

#define YYPACT_NINF (-72)

#define yypact_value_is_default(Yyn) \
  ((Yyn) == YYPACT_NINF)

#define YYTABLE_NINF (-5)

#define yytable_value_is_error(Yyn) \
  0

/* YYPACT[STATE-NUM] -- Index in YYTABLE of the portion describing
   STATE-NUM.  */
static const yytype_int16 yypact[] =
{
      64,   -21,    34,   260,   -72,   -72,     5,   -72,    19,    29,
      88,    88,   -72,    30,    84,    88,    88,    47,   134,   -72,
      88,    88,    88,    88,    88,    88,    88,    88,    88,    88,
      88,    88,    88,    88,    88,    88,    88,    88,    88,    88,
      88,   -72,    88,   -72,   -23,   224,   164,   194,   -72,   274,
     274,    50,    50,    50,    47,    47,   224,   224,   224,   224,
     224,   224,   266,   266,   266,   266,   266,   266,   246,   246,
     -72,    88,   -72,    76,   107,   224,   -72,   -72,   -72,   -72,
      68,    72,   -72,   -72
};

/* YYDEFACT[STATE-NUM] -- Default reduction number in state STATE-NUM.
   Performed when YYTABLE does not specify something else to do.  Zero
   means the default is an error.  */
static const yytype_int8 yydefact[] =
{
       0,     0,     0,     2,     3,     1,    41,     8,     0,     0,
       0,     0,     5,     0,     0,     0,     0,    28,     0,     7,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     6,     0,    39,     0,    13,     0,     0,    40,    21,
      22,    23,    24,    25,    26,    27,    15,    16,    17,    18,
      19,    20,    29,    30,    31,    32,    33,    34,    35,    36,
      37,     0,    38,     0,     0,    14,     4,    10,     4,    12,
       0,     0,     9,    11
};

/* YYPGOTO[NTERM-NUM].  */
static const yytype_int8 yypgoto[] =
{
     -72,   -72,   -43,   -71,   -72,   -10
};

/* YYDEFGOTO[NTERM-NUM].  */
static const yytype_int8 yydefgoto[] =
{
       0,     2,     3,    12,    44,    13
};

/* YYTABLE[YYPACT[STATE-NUM]] -- What to do in state STATE-NUM.  If
   positive, shift that token.  If negative, reduce the rule whose
   number is the opposite.  If YYTABLE_NINF, syntax error.  */
static const yytype_int8 yytable[] =
{
      17,    18,    77,    79,    45,    46,    47,    71,     4,    72,
      49,    50,    51,    52,    53,    54,    55,    56,    57,    58,
      59,    60,    61,    62,    63,    64,    65,    66,    67,    68,
      69,    19,    70,    80,     5,    81,    14,    20,    21,    22,
      23,    24,    25,    26,    27,    28,    29,    30,    31,    32,
      15,    33,    34,    35,    36,    37,    38,    39,    40,    41,
      16,    75,    25,    26,    -4,     1,    42,    -4,    -4,    -4,
      -4,     6,     7,     8,     9,     6,     7,     8,     9,     6,
       7,     8,     9,    42,    -4,     0,    42,     6,    10,     0,
       0,     6,    10,     0,     0,    -4,    10,     0,     0,    11,
       0,     0,    82,    11,    10,     0,    83,    11,    10,    76,
       6,     7,     8,     9,     0,    11,    43,     0,     0,    11,
       0,     0,     0,     0,     0,     0,     0,    10,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,    11,     0,
      78,    20,    21,    22,    23,    24,    25,    26,    27,    28,
      29,    30,    31,    32,     0,    33,    34,    35,    36,    37,
      38,    39,    40,     0,     0,     0,    48,     0,     0,     0,
      42,    20,    21,    22,    23,    24,    25,    26,    27,    28,
      29,    30,    31,    32,     0,    33,    34,    35,    36,    37,
      38,    39,    40,     0,     0,     0,    73,     0,     0,     0,
      42,    20,    21,    22,    23,    24,    25,    26,    27,    28,
      29,    30,    31,    32,     0,    33,    34,    35,    36,    37,
      38,    39,    40,     0,     0,     0,    74,     0,     0,     0,
      42,    20,    21,    22,    23,    24,    25,    26,    27,    28,
      29,    30,    31,    32,     0,    33,    34,    35,    36,    37,
      38,    39,    40,    20,    21,    22,    23,    24,    25,    26,
      42,     0,     0,     6,     7,     8,     9,    33,    34,    35,
      36,    37,    38,    20,    21,    22,    23,    24,    25,    26,
      10,     0,    42,    22,    23,    24,    25,    26,     0,     0,
       0,    11,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,    42,     0,     0,     0,     0,     0,     0,     0,
      42
};

static const yytype_int8 yycheck[] =
{
      10,    11,    73,    74,    14,    15,    16,    30,    29,    32,
      20,    21,    22,    23,    24,    25,    26,    27,    28,    29,
      30,    31,    32,    33,    34,    35,    36,    37,    38,    39,
      40,     1,    42,    76,     0,    78,    31,     7,     8,     9,
      10,    11,    12,    13,    14,    15,    16,    17,    18,    19,
      31,    21,    22,    23,    24,    25,    26,    27,    28,    29,
      31,    71,    12,    13,     0,     1,    36,     3,     4,     5,
       6,     3,     4,     5,     6,     3,     4,     5,     6,     3,
       4,     5,     6,    36,    20,    -1,    36,     3,    20,    -1,
      -1,     3,    20,    -1,    -1,    31,    20,    -1,    -1,    31,
      -1,    -1,    34,    31,    20,    -1,    34,    31,    20,    33,
       3,     4,     5,     6,    -1,    31,    32,    -1,    -1,    31,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    20,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    31,    -1,
      33,     7,     8,     9,    10,    11,    12,    13,    14,    15,
      16,    17,    18,    19,    -1,    21,    22,    23,    24,    25,
      26,    27,    28,    -1,    -1,    -1,    32,    -1,    -1,    -1,
      36,     7,     8,     9,    10,    11,    12,    13,    14,    15,
      16,    17,    18,    19,    -1,    21,    22,    23,    24,    25,
      26,    27,    28,    -1,    -1,    -1,    32,    -1,    -1,    -1,
      36,     7,     8,     9,    10,    11,    12,    13,    14,    15,
      16,    17,    18,    19,    -1,    21,    22,    23,    24,    25,
      26,    27,    28,    -1,    -1,    -1,    32,    -1,    -1,    -1,
      36,     7,     8,     9,    10,    11,    12,    13,    14,    15,
      16,    17,    18,    19,    -1,    21,    22,    23,    24,    25,
      26,    27,    28,     7,     8,     9,    10,    11,    12,    13,
      36,    -1,    -1,     3,     4,     5,     6,    21,    22,    23,
      24,    25,    26,     7,     8,     9,    10,    11,    12,    13,
      20,    -1,    36,     9,    10,    11,    12,    13,    -1,    -1,
      -1,    31,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    36,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      36
};

/* YYSTOS[STATE-NUM] -- The symbol kind of the accessing symbol of
   state STATE-NUM.  */
static const yytype_int8 yystos[] =
{
       0,     1,    38,    39,    29,     0,     3,     4,     5,     6,
      20,    31,    40,    42,    31,    31,    31,    42,    42,     1,
       7,     8,     9,    10,    11,    12,    13,    14,    15,    16,
      17,    18,    19,    21,    22,    23,    24,    25,    26,    27,
      28,    29,    36,    32,    41,    42,    42,    42,    32,    42,
      42,    42,    42,    42,    42,    42,    42,    42,    42,    42,
      42,    42,    42,    42,    42,    42,    42,    42,    42,    42,
      42,    30,    32,    32,    32,    42,    33,    40,    33,    40,
      39,    39,    34,    34
};

/* YYR1[RULE-NUM] -- Symbol kind of the left-hand side of rule RULE-NUM.  */
static const yytype_int8 yyr1[] =
{
       0,    37,    38,    38,    39,    39,    40,    40,    40,    40,
      40,    40,    40,    41,    41,    42,    42,    42,    42,    42,
      42,    42,    42,    42,    42,    42,    42,    42,    42,    42,
      42,    42,    42,    42,    42,    42,    42,    42,    42,    42,
      42,    42
};

/* YYR2[RULE-NUM] -- Number of symbols on the right-hand side of rule RULE-NUM.  */
static const yytype_int8 yyr2[] =
{
       0,     2,     1,     2,     0,     2,     2,     2,     1,     7,
       5,     7,     5,     1,     3,     3,     3,     3,     3,     3,
       3,     3,     3,     3,     3,     3,     3,     3,     2,     3,
       3,     3,     3,     3,     3,     3,     3,     3,     4,     3,
       3,     1
};


enum { YYENOMEM = -2 };

#define yyerrok         (yyerrstatus = 0)
#define yyclearin       (yychar = YYEMPTY)

#define YYACCEPT        goto yyacceptlab
#define YYABORT         goto yyabortlab
#define YYERROR         goto yyerrorlab
#define YYNOMEM         goto yyexhaustedlab


#define YYRECOVERING()  (!!yyerrstatus)

#define YYBACKUP(Token, Value)                                    \
  do                                                              \
    if (yychar == YYEMPTY)                                        \
      {                                                           \
        yychar = (Token);                                         \
        yylval = (Value);                                         \
        YYPOPSTACK (yylen);                                       \
        yystate = *yyssp;                                         \
        goto yybackup;                                            \
      }                                                           \
    else                                                          \
      {                                                           \
        yyerror (YY_("syntax error: cannot back up")); \
        YYERROR;                                                  \
      }                                                           \
  while (0)

/* Backward compatibility with an undocumented macro.
   Use YYerror or YYUNDEF. */
#define YYERRCODE YYUNDEF


/* Enable debugging if requested.  */
#if YYDEBUG

# ifndef YYFPRINTF
#  include <stdio.h> /* INFRINGES ON USER NAME SPACE */
#  define YYFPRINTF fprintf
# endif

# define YYDPRINTF(Args)                        \
do {                                            \
  if (yydebug)                                  \
    YYFPRINTF Args;                             \
} while (0)




# define YY_SYMBOL_PRINT(Title, Kind, Value, Location)                    \
do {                                                                      \
  if (yydebug)                                                            \
    {                                                                     \
      YYFPRINTF (stderr, "%s ", Title);                                   \
      yy_symbol_print (stderr,                                            \
                  Kind, Value); \
      YYFPRINTF (stderr, "\n");                                           \
    }                                                                     \
} while (0)


/*-----------------------------------.
| Print this symbol's value on YYO.  |
`-----------------------------------*/

static void
yy_symbol_value_print (FILE *yyo,
                       yysymbol_kind_t yykind, YYSTYPE const * const yyvaluep)
{
  FILE *yyoutput = yyo;
  YY_USE (yyoutput);
  if (!yyvaluep)
    return;
  YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
  YY_USE (yykind);
  YY_IGNORE_MAYBE_UNINITIALIZED_END
}


/*---------------------------.
| Print this symbol on YYO.  |
`---------------------------*/

static void
yy_symbol_print (FILE *yyo,
                 yysymbol_kind_t yykind, YYSTYPE const * const yyvaluep)
{
  YYFPRINTF (yyo, "%s %s (",
             yykind < YYNTOKENS ? "token" : "nterm", yysymbol_name (yykind));

  yy_symbol_value_print (yyo, yykind, yyvaluep);
  YYFPRINTF (yyo, ")");
}

/*------------------------------------------------------------------.
| yy_stack_print -- Print the state stack from its BOTTOM up to its |
| TOP (included).                                                   |
`------------------------------------------------------------------*/

static void
yy_stack_print (yy_state_t *yybottom, yy_state_t *yytop)
{
  YYFPRINTF (stderr, "Stack now");
  for (; yybottom <= yytop; yybottom++)
    {
      int yybot = *yybottom;
      YYFPRINTF (stderr, " %d", yybot);
    }
  YYFPRINTF (stderr, "\n");
}

# define YY_STACK_PRINT(Bottom, Top)                            \
do {                                                            \
  if (yydebug)                                                  \
    yy_stack_print ((Bottom), (Top));                           \
} while (0)


/*------------------------------------------------.
| Report that the YYRULE is going to be reduced.  |
`------------------------------------------------*/

static void
yy_reduce_print (yy_state_t *yyssp, YYSTYPE *yyvsp,
                 int yyrule)
{
  int yylno = yyrline[yyrule];
  int yynrhs = yyr2[yyrule];
  int yyi;
  YYFPRINTF (stderr, "Reducing stack by rule %d (line %d):\n",
             yyrule - 1, yylno);
  /* The symbols being reduced.  */
  for (yyi = 0; yyi < yynrhs; yyi++)
    {
      YYFPRINTF (stderr, "   $%d = ", yyi + 1);
      yy_symbol_print (stderr,
                       YY_ACCESSING_SYMBOL (+yyssp[yyi + 1 - yynrhs]),
                       &yyvsp[(yyi + 1) - (yynrhs)]);
      YYFPRINTF (stderr, "\n");
    }
}

# define YY_REDUCE_PRINT(Rule)          \
do {                                    \
  if (yydebug)                          \
    yy_reduce_print (yyssp, yyvsp, Rule); \
} while (0)

/* Nonzero means print parse trace.  It is left uninitialized so that
   multiple parsers can coexist.  */
int yydebug;
#else /* !YYDEBUG */
# define YYDPRINTF(Args) ((void) 0)
# define YY_SYMBOL_PRINT(Title, Kind, Value, Location)
# define YY_STACK_PRINT(Bottom, Top)
# define YY_REDUCE_PRINT(Rule)
#endif /* !YYDEBUG */


/* YYINITDEPTH -- initial size of the parser's stacks.  */
#ifndef YYINITDEPTH
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






/*-----------------------------------------------.
| Release the memory associated to this symbol.  |
`-----------------------------------------------*/

static void
yydestruct (const char *yymsg,
            yysymbol_kind_t yykind, YYSTYPE *yyvaluep)
{
  YY_USE (yyvaluep);
  if (!yymsg)
    yymsg = "Deleting";
  YY_SYMBOL_PRINT (yymsg, yykind, yyvaluep, yylocationp);

  YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
  YY_USE (yykind);
  YY_IGNORE_MAYBE_UNINITIALIZED_END
}


/* Lookahead token kind.  */
int yychar;

/* The semantic value of the lookahead symbol.  */
YYSTYPE yylval;
/* Number of syntax errors so far.  */
int yynerrs;




/*----------.
| yyparse.  |
`----------*/

int
yyparse (void)
{
    yy_state_fast_t yystate = 0;
    /* Number of tokens to shift before error messages enabled.  */
    int yyerrstatus = 0;

    /* Refer to the stacks through separate pointers, to allow yyoverflow
       to reallocate them elsewhere.  */

    /* Their size.  */
    YYPTRDIFF_T yystacksize = YYINITDEPTH;

    /* The state stack: array, bottom, top.  */
    yy_state_t yyssa[YYINITDEPTH];
    yy_state_t *yyss = yyssa;
    yy_state_t *yyssp = yyss;

    /* The semantic value stack: array, bottom, top.  */
    YYSTYPE yyvsa[YYINITDEPTH];
    YYSTYPE *yyvs = yyvsa;
    YYSTYPE *yyvsp = yyvs;

  int yyn;
  /* The return value of yyparse.  */
  int yyresult;
  /* Lookahead symbol kind.  */
  yysymbol_kind_t yytoken = YYSYMBOL_YYEMPTY;
  /* The variables used to return semantic value and location from the
     action routines.  */
  YYSTYPE yyval;



#define YYPOPSTACK(N)   (yyvsp -= (N), yyssp -= (N))

  /* The number of symbols on the RHS of the reduced rule.
     Keep to zero when no symbol should be popped.  */
  int yylen = 0;

  YYDPRINTF ((stderr, "Starting parse\n"));

  yychar = YYEMPTY; /* Cause a token to be read.  */

  goto yysetstate;


/*------------------------------------------------------------.
| yynewstate -- push a new state, which is found in yystate.  |
`------------------------------------------------------------*/
yynewstate:
  /* In all cases, when you get here, the value and location stacks
     have just been pushed.  So pushing a state here evens the stacks.  */
  yyssp++;


/*--------------------------------------------------------------------.
| yysetstate -- set current state (the top of the stack) to yystate.  |
`--------------------------------------------------------------------*/
yysetstate:
  YYDPRINTF ((stderr, "Entering state %d\n", yystate));
  YY_ASSERT (0 <= yystate && yystate < YYNSTATES);
  YY_IGNORE_USELESS_CAST_BEGIN
  *yyssp = YY_CAST (yy_state_t, yystate);
  YY_IGNORE_USELESS_CAST_END
  YY_STACK_PRINT (yyss, yyssp);

  if (yyss + yystacksize - 1 <= yyssp)
#if !defined yyoverflow && !defined YYSTACK_RELOCATE
    YYNOMEM;
#else
    {
      /* Get the current used size of the three stacks, in elements.  */
      YYPTRDIFF_T yysize = yyssp - yyss + 1;

# if defined yyoverflow
      {
        /* Give user a chance to reallocate the stack.  Use copies of
           these so that the &'s don't force the real ones into
           memory.  */
        yy_state_t *yyss1 = yyss;
        YYSTYPE *yyvs1 = yyvs;

        /* Each stack pointer address is followed by the size of the
           data in use in that stack, in bytes.  This used to be a
           conditional around just the two extra args, but that might
           be undefined if yyoverflow is a macro.  */
        yyoverflow (YY_("memory exhausted"),
                    &yyss1, yysize * YYSIZEOF (*yyssp),
                    &yyvs1, yysize * YYSIZEOF (*yyvsp),
                    &yystacksize);
        yyss = yyss1;
        yyvs = yyvs1;
      }
# else /* defined YYSTACK_RELOCATE */
      /* Extend the stack our own way.  */
      if (YYMAXDEPTH <= yystacksize)
        YYNOMEM;
      yystacksize *= 2;
      if (YYMAXDEPTH < yystacksize)
        yystacksize = YYMAXDEPTH;

      {
        yy_state_t *yyss1 = yyss;
        union yyalloc *yyptr =
          YY_CAST (union yyalloc *,
                   YYSTACK_ALLOC (YY_CAST (YYSIZE_T, YYSTACK_BYTES (yystacksize))));
        if (! yyptr)
          YYNOMEM;
        YYSTACK_RELOCATE (yyss_alloc, yyss);
        YYSTACK_RELOCATE (yyvs_alloc, yyvs);
#  undef YYSTACK_RELOCATE
        if (yyss1 != yyssa)
          YYSTACK_FREE (yyss1);
      }
# endif

      yyssp = yyss + yysize - 1;
      yyvsp = yyvs + yysize - 1;

      YY_IGNORE_USELESS_CAST_BEGIN
      YYDPRINTF ((stderr, "Stack size increased to %ld\n",
                  YY_CAST (long, yystacksize)));
      YY_IGNORE_USELESS_CAST_END

      if (yyss + yystacksize - 1 <= yyssp)
        YYABORT;
    }
#endif /* !defined yyoverflow && !defined YYSTACK_RELOCATE */


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

  /* YYCHAR is either empty, or end-of-input, or a valid lookahead.  */
  if (yychar == YYEMPTY)
    {
      YYDPRINTF ((stderr, "Reading a token\n"));
      yychar = yylex ();
    }

  if (yychar <= YYEOF)
    {
      yychar = YYEOF;
      yytoken = YYSYMBOL_YYEOF;
      YYDPRINTF ((stderr, "Now at end of input.\n"));
    }
  else if (yychar == YYerror)
    {
      /* The scanner already issued an error message, process directly
         to error recovery.  But do not keep the error token as
         lookahead, it is too special and may lead us to an endless
         loop in error recovery. */
      yychar = YYUNDEF;
      yytoken = YYSYMBOL_YYerror;
      goto yyerrlab1;
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
  yystate = yyn;
  YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
  *++yyvsp = yylval;
  YY_IGNORE_MAYBE_UNINITIALIZED_END

  /* Discard the shifted token.  */
  yychar = YYEMPTY;
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
| yyreduce -- do a reduction.  |
`-----------------------------*/
yyreduce:
  /* yyn is the number of a rule to reduce with.  */
  yylen = yyr2[yyn];

  /* If YYLEN is nonzero, implement the default value of the action:
     '$$ = $1'.

     Otherwise, the following line sets YYVAL to garbage.
     This behavior is undocumented and Bison
     users should not rely upon it.  Assigning to YYVAL
     unconditionally makes the parser a bit smaller, and it avoids a
     GCC warning that YYVAL may be used uninitialized.  */
  yyval = yyvsp[1-yylen];


  YY_REDUCE_PRINT (yyn);
  switch (yyn)
    {
  case 2: /* calosc: wyrazenia  */
#line 57 "drzewiarz.y"
                        { drzewo = (yyvsp[0].rozgalezienie); }
#line 1217 "drzewiarz.tab.c"
    break;

  case 3: /* calosc: error SREDNIK  */
#line 58 "drzewiarz.y"
                        { yyerror("Nieoczekiwany srednik"); yyerrok; }
#line 1223 "drzewiarz.tab.c"
    break;

  case 4: /* wyrazenia: %empty  */
#line 63 "drzewiarz.y"
                                    { (yyval.rozgalezienie) = utworzRozgalezienie(); }
#line 1229 "drzewiarz.tab.c"
    break;

  case 5: /* wyrazenia: wyrazenia wyrazenie  */
#line 64 "drzewiarz.y"
                                    { dodajGalaz((yyvsp[-1].rozgalezienie), (yyvsp[0].galazPodwojna)); (yyval.rozgalezienie) = (yyvsp[-1].rozgalezienie); }
#line 1235 "drzewiarz.tab.c"
    break;

  case 6: /* wyrazenie: dzialanie SREDNIK  */
#line 68 "drzewiarz.y"
                                                                    { (yyval.galazPodwojna) = (yyvsp[-1].galazPodwojna); }
#line 1241 "drzewiarz.tab.c"
    break;

  case 7: /* wyrazenie: dzialanie error  */
#line 69 "drzewiarz.y"
                                                                    { yyerror("Brak srednika"); yyerrok; }
#line 1247 "drzewiarz.tab.c"
    break;

  case 8: /* wyrazenie: POLECENIE  */
#line 70 "drzewiarz.y"
                                                                    { (yyval.galazPodwojna) = utworzPolecenieJakoGalaz((yyvsp[0].polecenie)); }
#line 1253 "drzewiarz.tab.c"
    break;

  case 9: /* wyrazenie: JESLI LNAWIAS dzialanie PNAWIAS LSPIECIE wyrazenia PSPIECIE  */
#line 71 "drzewiarz.y"
                                                                    { (yyval.galazPodwojna) = utworzWyrazenieKluczowe(JESLI, (yyvsp[-4].galazPodwojna), (yyvsp[-1].rozgalezienie)); }
#line 1259 "drzewiarz.tab.c"
    break;

  case 10: /* wyrazenie: JESLI LNAWIAS dzialanie PNAWIAS wyrazenie  */
#line 72 "drzewiarz.y"
                                                                    { (yyval.galazPodwojna) = utworzWyrazenieKluczowe(JESLI, (yyvsp[-2].galazPodwojna), pojedynczeRozgalezienie((yyvsp[0].galazPodwojna))); }
#line 1265 "drzewiarz.tab.c"
    break;

  case 11: /* wyrazenie: POKI LNAWIAS dzialanie PNAWIAS LSPIECIE wyrazenia PSPIECIE  */
#line 73 "drzewiarz.y"
                                                                    { (yyval.galazPodwojna) = utworzWyrazenieKluczowe(POKI, (yyvsp[-4].galazPodwojna), (yyvsp[-1].rozgalezienie)); }
#line 1271 "drzewiarz.tab.c"
    break;

  case 12: /* wyrazenie: POKI LNAWIAS dzialanie PNAWIAS wyrazenie  */
#line 74 "drzewiarz.y"
                                                                    { (yyval.galazPodwojna) = utworzWyrazenieKluczowe(POKI, (yyvsp[-2].galazPodwojna), pojedynczeRozgalezienie((yyvsp[0].galazPodwojna))); }
#line 1277 "drzewiarz.tab.c"
    break;

  case 13: /* dzialania: dzialanie  */
#line 78 "drzewiarz.y"
                                        { (yyval.rozgalezienie) = utworzRozgalezienie(); dodajGalaz((yyval.rozgalezienie), (yyvsp[0].galazPodwojna)); }
#line 1283 "drzewiarz.tab.c"
    break;

  case 14: /* dzialania: dzialania PRZECINEK dzialanie  */
#line 79 "drzewiarz.y"
                                        { dodajGalaz((yyvsp[-2].rozgalezienie), (yyvsp[0].galazPodwojna)); (yyval.rozgalezienie) = (yyvsp[-2].rozgalezienie); }
#line 1289 "drzewiarz.tab.c"
    break;

  case 15: /* dzialanie: dzialanie NADAJ dzialanie  */
#line 83 "drzewiarz.y"
                                                { (yyval.galazPodwojna) = utworzDzialanie(NADAJ, (yyvsp[-2].galazPodwojna), (yyvsp[0].galazPodwojna)); }
#line 1295 "drzewiarz.tab.c"
    break;

  case 16: /* dzialanie: dzialanie NDODAJ dzialanie  */
#line 84 "drzewiarz.y"
                                                { (yyval.galazPodwojna) = utworzDzialanie(NDODAJ, (yyvsp[-2].galazPodwojna), (yyvsp[0].galazPodwojna)); }
#line 1301 "drzewiarz.tab.c"
    break;

  case 17: /* dzialanie: dzialanie NODEJMIJ dzialanie  */
#line 85 "drzewiarz.y"
                                                { (yyval.galazPodwojna) = utworzDzialanie(NODEJMIJ, (yyvsp[-2].galazPodwojna), (yyvsp[0].galazPodwojna)); }
#line 1307 "drzewiarz.tab.c"
    break;

  case 18: /* dzialanie: dzialanie NMNOZ dzialanie  */
#line 86 "drzewiarz.y"
                                                { (yyval.galazPodwojna) = utworzDzialanie(NMNOZ, (yyvsp[-2].galazPodwojna), (yyvsp[0].galazPodwojna)); }
#line 1313 "drzewiarz.tab.c"
    break;

  case 19: /* dzialanie: dzialanie NDZIEL dzialanie  */
#line 87 "drzewiarz.y"
                                                { (yyval.galazPodwojna) = utworzDzialanie(NDZIEL, (yyvsp[-2].galazPodwojna), (yyvsp[0].galazPodwojna)); }
#line 1319 "drzewiarz.tab.c"
    break;

  case 20: /* dzialanie: dzialanie NRESZTA dzialanie  */
#line 88 "drzewiarz.y"
                                                { (yyval.galazPodwojna) = utworzDzialanie(NRESZTA, (yyvsp[-2].galazPodwojna), (yyvsp[0].galazPodwojna)); }
#line 1325 "drzewiarz.tab.c"
    break;

  case 21: /* dzialanie: dzialanie DODAJ dzialanie  */
#line 89 "drzewiarz.y"
                                                { (yyval.galazPodwojna) = utworzDzialanie(DODAJ, (yyvsp[-2].galazPodwojna), (yyvsp[0].galazPodwojna)); }
#line 1331 "drzewiarz.tab.c"
    break;

  case 22: /* dzialanie: dzialanie ODEJMIJ dzialanie  */
#line 90 "drzewiarz.y"
                                                { (yyval.galazPodwojna) = utworzDzialanie(ODEJMIJ, (yyvsp[-2].galazPodwojna), (yyvsp[0].galazPodwojna)); }
#line 1337 "drzewiarz.tab.c"
    break;

  case 23: /* dzialanie: dzialanie MNOZ dzialanie  */
#line 91 "drzewiarz.y"
                                                { (yyval.galazPodwojna) = utworzDzialanie(MNOZ, (yyvsp[-2].galazPodwojna), (yyvsp[0].galazPodwojna)); }
#line 1343 "drzewiarz.tab.c"
    break;

  case 24: /* dzialanie: dzialanie DZIEL dzialanie  */
#line 92 "drzewiarz.y"
                                                { (yyval.galazPodwojna) = utworzDzialanie(DZIEL, (yyvsp[-2].galazPodwojna), (yyvsp[0].galazPodwojna)); }
#line 1349 "drzewiarz.tab.c"
    break;

  case 25: /* dzialanie: dzialanie RESZTA dzialanie  */
#line 93 "drzewiarz.y"
                                                { (yyval.galazPodwojna) = utworzDzialanie(RESZTA, (yyvsp[-2].galazPodwojna), (yyvsp[0].galazPodwojna)); }
#line 1355 "drzewiarz.tab.c"
    break;

  case 26: /* dzialanie: dzialanie WIERZCH dzialanie  */
#line 94 "drzewiarz.y"
                                                { (yyval.galazPodwojna) = utworzDzialanie(WIERZCH, (yyvsp[-2].galazPodwojna), (yyvsp[0].galazPodwojna)); }
#line 1361 "drzewiarz.tab.c"
    break;

  case 27: /* dzialanie: dzialanie SPOD dzialanie  */
#line 95 "drzewiarz.y"
                                                { (yyval.galazPodwojna) = utworzDzialanie(SPOD, (yyvsp[-2].galazPodwojna), (yyvsp[0].galazPodwojna)); }
#line 1367 "drzewiarz.tab.c"
    break;

  case 28: /* dzialanie: NIE dzialanie  */
#line 96 "drzewiarz.y"
                                                { (yyval.galazPodwojna) = utworzDzialanie(NIE, (yyvsp[0].galazPodwojna), NULL); }
#line 1373 "drzewiarz.tab.c"
    break;

  case 29: /* dzialanie: dzialanie ROWNE dzialanie  */
#line 97 "drzewiarz.y"
                                                { (yyval.galazPodwojna) = utworzDzialanie(ROWNE, (yyvsp[-2].galazPodwojna), (yyvsp[0].galazPodwojna)); }
#line 1379 "drzewiarz.tab.c"
    break;

  case 30: /* dzialanie: dzialanie ROZNE dzialanie  */
#line 98 "drzewiarz.y"
                                                { (yyval.galazPodwojna) = utworzDzialanie(ROZNE, (yyvsp[-2].galazPodwojna), (yyvsp[0].galazPodwojna)); }
#line 1385 "drzewiarz.tab.c"
    break;

  case 31: /* dzialanie: dzialanie WIEKSZE dzialanie  */
#line 99 "drzewiarz.y"
                                                { (yyval.galazPodwojna) = utworzDzialanie(WIEKSZE, (yyvsp[-2].galazPodwojna), (yyvsp[0].galazPodwojna)); }
#line 1391 "drzewiarz.tab.c"
    break;

  case 32: /* dzialanie: dzialanie MNIEJSZE dzialanie  */
#line 100 "drzewiarz.y"
                                                { (yyval.galazPodwojna) = utworzDzialanie(MNIEJSZE, (yyvsp[-2].galazPodwojna), (yyvsp[0].galazPodwojna)); }
#line 1397 "drzewiarz.tab.c"
    break;

  case 33: /* dzialanie: dzialanie WIEKSZE_BADZ_ROWNE dzialanie  */
#line 101 "drzewiarz.y"
                                                { (yyval.galazPodwojna) = utworzDzialanie(WIEKSZE_BADZ_ROWNE, (yyvsp[-2].galazPodwojna), (yyvsp[0].galazPodwojna)); }
#line 1403 "drzewiarz.tab.c"
    break;

  case 34: /* dzialanie: dzialanie MNIEJSZE_BADZ_ROWNE dzialanie  */
#line 102 "drzewiarz.y"
                                                { (yyval.galazPodwojna) = utworzDzialanie(MNIEJSZE_BADZ_ROWNE, (yyvsp[-2].galazPodwojna), (yyvsp[0].galazPodwojna)); }
#line 1409 "drzewiarz.tab.c"
    break;

  case 35: /* dzialanie: dzialanie I dzialanie  */
#line 103 "drzewiarz.y"
                                                            { (yyval.galazPodwojna) = utworzDzialanie(I, (yyvsp[-2].galazPodwojna), (yyvsp[0].galazPodwojna)); }
#line 1415 "drzewiarz.tab.c"
    break;

  case 36: /* dzialanie: dzialanie LUB dzialanie  */
#line 104 "drzewiarz.y"
                                                            { (yyval.galazPodwojna) = utworzDzialanie(LUB, (yyvsp[-2].galazPodwojna), (yyvsp[0].galazPodwojna)); }
#line 1421 "drzewiarz.tab.c"
    break;

  case 37: /* dzialanie: dzialanie KROPKA dzialanie  */
#line 105 "drzewiarz.y"
                                                            { (yyval.galazPodwojna) = utworzDzialanie(KROPKA, (yyvsp[-2].galazPodwojna), (yyvsp[0].galazPodwojna)); }
#line 1427 "drzewiarz.tab.c"
    break;

  case 38: /* dzialanie: ZMIENNA LNAWIAS dzialania PNAWIAS  */
#line 106 "drzewiarz.y"
                                                          { (yyval.galazPodwojna) = utworzDzialanie(WYKONAJ, utworzZmiennaJakoGalaz((yyvsp[-3].zmienna)), (yyvsp[-1].rozgalezienie)); }
#line 1433 "drzewiarz.tab.c"
    break;

  case 39: /* dzialanie: ZMIENNA LNAWIAS PNAWIAS  */
#line 107 "drzewiarz.y"
                                                          { (yyval.galazPodwojna) = utworzDzialanie(WYKONAJ, utworzZmiennaJakoGalaz((yyvsp[-2].zmienna)), utworzRozgalezienie()); }
#line 1439 "drzewiarz.tab.c"
    break;

  case 40: /* dzialanie: LNAWIAS dzialanie PNAWIAS  */
#line 108 "drzewiarz.y"
                                                            { (yyval.galazPodwojna) = (yyvsp[-1].galazPodwojna); }
#line 1445 "drzewiarz.tab.c"
    break;

  case 41: /* dzialanie: ZMIENNA  */
#line 109 "drzewiarz.y"
                                                            { (yyval.galazPodwojna) = utworzZmiennaJakoGalaz((yyvsp[0].zmienna)); }
#line 1451 "drzewiarz.tab.c"
    break;


#line 1455 "drzewiarz.tab.c"

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
  YY_SYMBOL_PRINT ("-> $$ =", YY_CAST (yysymbol_kind_t, yyr1[yyn]), &yyval, &yyloc);

  YYPOPSTACK (yylen);
  yylen = 0;

  *++yyvsp = yyval;

  /* Now 'shift' the result of the reduction.  Determine what state
     that goes to, based on the state we popped back to and the rule
     number reduced by.  */
  {
    const int yylhs = yyr1[yyn] - YYNTOKENS;
    const int yyi = yypgoto[yylhs] + *yyssp;
    yystate = (0 <= yyi && yyi <= YYLAST && yycheck[yyi] == *yyssp
               ? yytable[yyi]
               : yydefgoto[yylhs]);
  }

  goto yynewstate;


/*--------------------------------------.
| yyerrlab -- here on detecting error.  |
`--------------------------------------*/
yyerrlab:
  /* Make sure we have latest lookahead translation.  See comments at
     user semantic actions for why this is necessary.  */
  yytoken = yychar == YYEMPTY ? YYSYMBOL_YYEMPTY : YYTRANSLATE (yychar);
  /* If not already recovering from an error, report this error.  */
  if (!yyerrstatus)
    {
      ++yynerrs;
      yyerror (YY_("syntax error"));
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
  /* Pacify compilers when the user code never invokes YYERROR and the
     label yyerrorlab therefore never appears in user code.  */
  if (0)
    YYERROR;
  ++yynerrs;

  /* Do not reclaim the symbols of the rule whose action triggered
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
  yyerrstatus = 3;      /* Each real token shifted decrements this.  */

  /* Pop stack until we find a state that shifts the error token.  */
  for (;;)
    {
      yyn = yypact[yystate];
      if (!yypact_value_is_default (yyn))
        {
          yyn += YYSYMBOL_YYerror;
          if (0 <= yyn && yyn <= YYLAST && yycheck[yyn] == YYSYMBOL_YYerror)
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
                  YY_ACCESSING_SYMBOL (yystate), yyvsp);
      YYPOPSTACK (1);
      yystate = *yyssp;
      YY_STACK_PRINT (yyss, yyssp);
    }

  YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
  *++yyvsp = yylval;
  YY_IGNORE_MAYBE_UNINITIALIZED_END


  /* Shift the error token.  */
  YY_SYMBOL_PRINT ("Shifting", YY_ACCESSING_SYMBOL (yyn), yyvsp, yylsp);

  yystate = yyn;
  goto yynewstate;


/*-------------------------------------.
| yyacceptlab -- YYACCEPT comes here.  |
`-------------------------------------*/
yyacceptlab:
  yyresult = 0;
  goto yyreturnlab;


/*-----------------------------------.
| yyabortlab -- YYABORT comes here.  |
`-----------------------------------*/
yyabortlab:
  yyresult = 1;
  goto yyreturnlab;


/*-----------------------------------------------------------.
| yyexhaustedlab -- YYNOMEM (memory exhaustion) comes here.  |
`-----------------------------------------------------------*/
yyexhaustedlab:
  yyerror (YY_("memory exhausted"));
  yyresult = 2;
  goto yyreturnlab;


/*----------------------------------------------------------.
| yyreturnlab -- parsing is finished, clean up and return.  |
`----------------------------------------------------------*/
yyreturnlab:
  if (yychar != YYEMPTY)
    {
      /* Make sure we have latest lookahead translation.  See comments at
         user semantic actions for why this is necessary.  */
      yytoken = YYTRANSLATE (yychar);
      yydestruct ("Cleanup: discarding lookahead",
                  yytoken, &yylval);
    }
  /* Do not reclaim the symbols of the rule whose action triggered
     this YYABORT or YYACCEPT.  */
  YYPOPSTACK (yylen);
  YY_STACK_PRINT (yyss, yyssp);
  while (yyssp != yyss)
    {
      yydestruct ("Cleanup: popping",
                  YY_ACCESSING_SYMBOL (+*yyssp), yyvsp);
      YYPOPSTACK (1);
    }
#ifndef yyoverflow
  if (yyss != yyssa)
    YYSTACK_FREE (yyss);
#endif

  return yyresult;
}

#line 111 "drzewiarz.y"


int zmienniana = ZMIENNA;
int poleceniana = POLECENIE;
int wykonaniana = WYKONAJ;
int kluczowe[] = { 0, JESLI, POKI};
size_t liczbaSlowKluczowych = sizeof(kluczowe)/sizeof(kluczowe[0]);
// char* dzialania[] = {"==", "!=", ">=", "<=", "||", "&&", "+=", "-=", "*=", "/=", "%=", "><", "<>", "+", "-", "*", "/", "%", "=", "!", ">", "<", "."};
int dzialaniowe[] = {ROWNE, ROZNE, WIEKSZE_BADZ_ROWNE, MNIEJSZE_BADZ_ROWNE, LUB, I, NDODAJ, NODEJMIJ, NMNOZ, NDZIEL, NRESZTA, WIERZCH, SPOD, DODAJ, ODEJMIJ, MNOZ, DZIEL, RESZTA, NADAJ, NIE, WIEKSZE, MNIEJSZE, KROPKA};
size_t liczbaDzialan = sizeof(dzialaniowe)/sizeof(dzialaniowe[0]);

GalazPodwojna* utworzZmiennaJakoGalaz(size_t z)
{
    GalazPodwojna* galaz = malloc(sizeof(GalazPodwojna));
    if(!galaz)
    {
        fprintf(stderr, "Brak pamieci\n");
        exit(EXIT_FAILURE);
    }
    galaz->rodzaj = ZMIENNA;
    galaz->wartosc = (uintptr_t)z;
    galaz->lewa = NULL;
    galaz->prawa = NULL;
    return galaz;
}

GalazPodwojna* utworzPolecenieJakoGalaz(char* p)
{
    GalazPodwojna* galaz = malloc(sizeof(GalazPodwojna));
    if(!galaz)
    {
        fprintf(stderr, "Brak pamieci\n");
        exit(EXIT_FAILURE);
    }
    galaz->rodzaj = POLECENIE;
    galaz->wartosc = (uintptr_t)p;
    galaz->lewa = NULL;
    galaz->prawa = NULL;
    return galaz;
}


Rozgalezienie* utworzRozgalezienie()
{
    Rozgalezienie* wyrazenia = malloc(sizeof(Rozgalezienie));
    if (!wyrazenia)
    {
        fprintf(stderr, "Brak pamieci\n");
        exit(EXIT_FAILURE);
    }
    wyrazenia->rozmiar = 0;
    wyrazenia->pojemnosc = 2;
    wyrazenia->galezie = malloc(wyrazenia->pojemnosc * sizeof(GalazPodwojna*));
    if (!wyrazenia->galezie)
    {
        fprintf(stderr, "Brak pamieci\n");
        exit(EXIT_FAILURE);
    }
    return wyrazenia;
}

Rozgalezienie* pojedynczeRozgalezienie(GalazPodwojna* galaz)
{
    Rozgalezienie* r = utworzRozgalezienie();
    dodajGalaz(r, galaz);
    return r;
}


GalazPodwojna* utworzDzialanie(int dz, GalazPodwojna* lewy, void* prawy)
{
    GalazPodwojna* galaz = malloc(sizeof(GalazPodwojna));
    if(!galaz)
    {
        fprintf(stderr, "Brak pamieci\n");
        exit(EXIT_FAILURE);
    }
    galaz->rodzaj = dz;
    galaz->wartosc = 0;
    galaz->lewa = lewy;
    galaz->prawa = prawy;
    return galaz;
}

GalazPodwojna* utworzWyrazenieKluczowe(int dz, GalazPodwojna* lewy, Rozgalezienie* prawy)
{
    GalazPodwojna* galaz = malloc(sizeof(GalazPodwojna));
    if(!galaz)
    {
        fprintf(stderr, "Brak pamieci\n");
        exit(EXIT_FAILURE);
    }
    galaz->rodzaj = dz;
    galaz->wartosc = 0;
    galaz->lewa = lewy;
    galaz->prawa = prawy;
    return galaz;
}

int yylex(void)
{
    //printf("yylex rusza!\n");
    if(odnosnikCzastki >= liczbaCzastek) return 0;
    Czastka czastka = czastki[odnosnikCzastki++];
    polozenie = czastka.polozenie;
    if(czastka.rodzaj == 0) // nieznany (nawiasy, klamry i średniki)
    {
        yylval.zmienna = 0;
        //printf("Wyslano nieznany: %c\n", ((char*)czastka.zawartosc)[0]);
        //getchar();
        switch (*((char*)czastka.zawartosc)) // nieznany to zawsze jeden bajt
        {
            case ';': return SREDNIK;
            case ',': return PRZECINEK;
            case '(': return LNAWIAS;
            case ')': return PNAWIAS;
            case '{': return LSPIECIE;
            case '}': return PSPIECIE;
            default:
                fprintf(stderr, "Nieznany token\n");
                getchar();
                exit(EXIT_FAILURE);
        }
    }
    else if(czastka.rodzaj == 1) // słowo kluczowe
    {
        yylval.zmienna = 0;
        for(int i = 1; i < sizeof(kluczowe) / sizeof(int); i++)
        {
            if((char*)czastka.zawartosc == slowaKluczowe[i]) 
            {
                //printf("Wyslano kluczowe: %s\n", slowaKluczowe[i]);
                //getchar();
                return kluczowe[i]; // porównywanie wskaźników a nie stringów
            }
        }

        for (int i = 0; i < sizeof(kluczowe) / sizeof(int); i++) // jeśli nic się nie znajdzie porównujemy zawartość dla pewności
        {
            if(strcmp((char*)czastka.zawartosc, slowaKluczowe[i]) == 0) return kluczowe[i];
        }
        fprintf(stderr, "Nieznany token\n");
        getchar();
        exit(EXIT_FAILURE);
    }
    else if(czastka.rodzaj == 3) // działanie
    {
        yylval.zmienna = 0;

        for(int i = 0; i < sizeof(dzialaniowe) / sizeof(int); i++)
        {
            if((char*)czastka.zawartosc == dzialania[i]) 
            {
                //printf("Wyslano dzialanie: %s\n", dzialania[i]);
                //getchar();
                return dzialaniowe[i]; // porównywanie wskaźników a nie stringów
            }
        }

        for (int i = 0; i < sizeof(dzialaniowe) / sizeof(int); i++) // jeśli nic się nie znajdzie porównujemy zawartość dla pewności
        {
            if(strcmp((char*)czastka.zawartosc, dzialania[i]) == 0) return dzialaniowe[i];
        }
        fprintf(stderr, "Nieznany token\n");
        getchar();
        exit(EXIT_FAILURE);
    }
    else if(czastka.rodzaj == 4) // surowe polecenie
    {
        yylval.polecenie = (char*)czastka.zawartosc;
        return POLECENIE;
    }
    else if(czastka.rodzaj == 2) // zmienna
    {
        yylval.zmienna = czastka.zawartosc;
        //printf("Wyslano zmienna: %zu\n", (size_t)czastka.zawartosc);
        //getchar();
        return ZMIENNA;
    }
    fprintf(stderr, "Nieznany token\n");
    getchar();
    exit(EXIT_FAILURE);
}

void yyerror(const char *s)
{
    fprintf(stderr, "\033[1;31m[BLAD SKLADNIOWY] %s\033[0m\n", s);
    FILE* pekBledu = fopen("blad.txt", "wb+");
    if(pPola == NULL || kPola == NULL)
    {
        fprintf(stderr, "\033[1;31mNieznane zrodlo\033[0m\n");
    }
    else if(pekBledu == NULL)
    {
        //perror("fopen (blad.txt)");
        fprintf(stderr, "\033[1;31mNie mozna otworzyc peku bledu (wb+)\033[0m\n");
    }
    else
    {
        size_t pionowa = 1;
        size_t pozioma = 1;
        char* i;
        for(i = pPola; i < polozenie; i++)
        {
            pozioma++;
            if(*i == '\n')
            {
                pozioma = 0;
                pionowa++;
            }
            fputc(*i, pekBledu);
        }
        if(i < kPola && *i != '\n')
        {
            while(i < kPola && *i != '\n') fputc(*(i++), pekBledu);
        }
        else fputc('\n', pekBledu);
        i++;
        if(pozioma)
        {
            for(size_t j = 1; j < pozioma; j++) fputc(' ', pekBledu);
            fprintf(pekBledu, "/\\\n");
            if(pozioma > 1)
            {
                for(size_t j = 2; j < pozioma; j++) fputc(' ', pekBledu);
                fprintf(pekBledu, "/||\\\n");
            }
            else
            {
                for(size_t j = 1; j < pozioma; j++) fputc(' ', pekBledu);
                fprintf(pekBledu, "||\\\n");
            }
            for(size_t j = 1; j < pozioma; j++) fputc(' ', pekBledu);
            fprintf(pekBledu, "||\n");
            for(size_t j = 1; j < pozioma; j++) fputc(' ', pekBledu);
            fprintf(pekBledu, "┕┙\n");
        }
        else
        {
            fprintf(pekBledu, "\\\n");
            fprintf(pekBledu, "|\\\n");
            fprintf(pekBledu, "|\n");
            fprintf(pekBledu, "┙\n");
        }
        if(kPola > i) fwrite(i, 1, kPola - i, pekBledu);
        fclose(pekBledu);
        fprintf(stderr, "\033[1;31mPolozenie bledu: [ %zu : %zu ]\033[0m\n", pionowa, pozioma);
    }

    getchar();
    exit(EXIT_FAILURE);
}

Rozgalezienie* robDrzewo(Czastka* cz, size_t l, char* p, char* k)
{
    czastki = cz;
    odnosnikCzastki = 0;
    liczbaCzastek = l;
    if(!czastki || !liczbaCzastek)
    {
        //printf("Podano puste dane!\n");
        //getchar();
        return NULL;
    }
    pPola = p;
    kPola = k;
    drzewo = NULL;
    //printf("Rozpoczynamy?");
    //getchar();
    int result = yyparse();
    if (result != 0)
    {
        //fprintf(stderr, "Błąd podczas parsowania (kod: %d)\n", result);
        //getchar();
        return NULL;
    }
    return drzewo;
}

void dodajGalaz(Rozgalezienie* rozgalezienie, GalazPodwojna* galaz)
{
    if(rozgalezienie->pojemnosc == rozgalezienie->rozmiar)
    {
        rozgalezienie->pojemnosc *= 2;
        rozgalezienie->galezie = realloc(rozgalezienie->galezie, rozgalezienie->pojemnosc * sizeof(GalazPodwojna*));
        if(!rozgalezienie->galezie)
        {
            fprintf(stderr, "Brak pamieci\n");
            exit(EXIT_FAILURE);
        }
    }
    rozgalezienie->galezie[rozgalezienie->rozmiar++] = galaz;
}

void zetnijGalaz(GalazPodwojna* galaz)
{
    if(galaz == NULL) return;
    zetnijGalaz(galaz->lewa);
    if(galaz->rodzaj == JESLI || galaz->rodzaj == POKI || galaz->rodzaj == WYKONAJ) zetnijRozgalezienie(galaz->prawa);
    else zetnijGalaz(galaz->prawa);
    free(galaz);
}

void zetnijRozgalezienie(Rozgalezienie* rozgalezienie)
{
    if (rozgalezienie == NULL) return;
    for(int i = 0; i < rozgalezienie->rozmiar; i++)
    {
        zetnijGalaz(rozgalezienie->galezie[i]);
    }
    free(rozgalezienie->galezie);
    free(rozgalezienie);
}

const char* nazwaRodzaju(int rodzaj)
{
    if(rodzaj == ZMIENNA) return "ZMIENNA";
    if(rodzaj == WYKONAJ) return "WYKONANIE";
    if(rodzaj == POLECENIE) return "POLECENIE";
    for(size_t i = 0; i < sizeof(dzialaniowe)/sizeof(dzialaniowe[0]); i++)
    {
        if(dzialaniowe[i] == rodzaj) return dzialania[i];
    }
    for(size_t i = 0; i < sizeof(kluczowe)/sizeof(kluczowe[0]); i++)
    {
        if(kluczowe[i] == rodzaj) return slowaKluczowe[i];
    }
    return "???";
}

void wypiszGalaz(GalazPodwojna* galaz, int wciecie)
{
    if(!galaz) return;

    for(int i = 0; i < wciecie; i++) printf("  ");

    printf("Rodzaj: %s", nazwaRodzaju(galaz->rodzaj));

    if(galaz->rodzaj == ZMIENNA)
    {
        printf(", Zmienna #%zu\n", galaz->wartosc);
    }
    else if(galaz->rodzaj == WYKONAJ)
    {
        printf(", Zmienna #%zu\n", ((GalazPodwojna*)galaz->lewa)->wartosc);
    }
    else if(galaz->rodzaj == POLECENIE)
    {
        printf("\n");
    }
    else if(galaz->rodzaj == JESLI || galaz->rodzaj == POKI)
    {
        printf(" {\n");
        for (int i = 0; i < wciecie + 1; i++) printf("  ");
        printf("Warunek:\n");
        wypiszGalaz(galaz->lewa, wciecie + 2);
        for (int i = 0; i < wciecie + 1; i++) printf("  ");
        printf("Spiecie:\n");
        wypiszDrzewo((Rozgalezienie*)galaz->prawa, wciecie + 2);
        for (int i = 0; i < wciecie; i++) printf("  ");
        printf("}\n");
    }
    else
    {
        printf("\n");
        wypiszGalaz(galaz->lewa, wciecie + 1);
        wypiszGalaz(galaz->prawa, wciecie + 1);
    }
}

void wypiszDrzewo(Rozgalezienie* rozgalezienie, int wciecie)
{
    if(!rozgalezienie) return;

    for(size_t i = 0; i < rozgalezienie->rozmiar; i++)
    {
        wypiszGalaz(rozgalezienie->galezie[i], wciecie);
    }
}
