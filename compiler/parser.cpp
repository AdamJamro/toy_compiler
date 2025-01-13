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
#line 1 "/mnt/c/Users/adame/CLionProjects/jftt_compiler/compiler/parser.y"

#include <cstring>
#include <cstdio>
#include <iostream>
#include <fstream>
#include <string>
#include <utility>
#include <functional>
#include <unordered_map>
#include <unordered_set>
#include "parser_utils.h"
#include <typeinfo>
#include <algorithm>


extern int yylineno;
extern FILE* yyin;
extern char* yytext;
std::ofstream output_file;

void yyerror(const char *);
void yyerror(const std::string&, int, const std::string&);
extern int my_yylex();
#define yylex my_yylex

using namespace std;

register_table regs;
funs_table funs; // fun name -> line number

//TODO MAKE IT REGULAR SET:
unordered_set<long> cached_constants = {};
//unordered_set<long> constants = {};


#line 107 "../compiler/parser.cpp"

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

#include "parser.h"
/* Symbol kind.  */
enum yysymbol_kind_t
{
  YYSYMBOL_YYEMPTY = -2,
  YYSYMBOL_YYEOF = 0,                      /* "end of file"  */
  YYSYMBOL_YYerror = 1,                    /* error  */
  YYSYMBOL_YYUNDEF = 2,                    /* "invalid token"  */
  YYSYMBOL_NUMBER = 3,                     /* NUMBER  */
  YYSYMBOL_pidentifier = 4,                /* pidentifier  */
  YYSYMBOL_ASSIGNMENT = 5,                 /* ASSIGNMENT  */
  YYSYMBOL_NEQ = 6,                        /* NEQ  */
  YYSYMBOL_GEQ = 7,                        /* GEQ  */
  YYSYMBOL_LEQ = 8,                        /* LEQ  */
  YYSYMBOL_BEGIN_KW = 9,                   /* BEGIN_KW  */
  YYSYMBOL_PROGRAM = 10,                   /* PROGRAM  */
  YYSYMBOL_PROCEDURE = 11,                 /* PROCEDURE  */
  YYSYMBOL_IS = 12,                        /* IS  */
  YYSYMBOL_END = 13,                       /* END  */
  YYSYMBOL_IF = 14,                        /* IF  */
  YYSYMBOL_THEN = 15,                      /* THEN  */
  YYSYMBOL_ELSE = 16,                      /* ELSE  */
  YYSYMBOL_ENDIF = 17,                     /* ENDIF  */
  YYSYMBOL_WHILE = 18,                     /* WHILE  */
  YYSYMBOL_DO = 19,                        /* DO  */
  YYSYMBOL_ENDWHILE = 20,                  /* ENDWHILE  */
  YYSYMBOL_FOR = 21,                       /* FOR  */
  YYSYMBOL_FROM = 22,                      /* FROM  */
  YYSYMBOL_TO = 23,                        /* TO  */
  YYSYMBOL_DOWNTO = 24,                    /* DOWNTO  */
  YYSYMBOL_ENDFOR = 25,                    /* ENDFOR  */
  YYSYMBOL_REPEAT = 26,                    /* REPEAT  */
  YYSYMBOL_UNTIL = 27,                     /* UNTIL  */
  YYSYMBOL_READ = 28,                      /* READ  */
  YYSYMBOL_WRITE = 29,                     /* WRITE  */
  YYSYMBOL_T = 30,                         /* T  */
  YYSYMBOL_31_ = 31,                       /* ';'  */
  YYSYMBOL_32_ = 32,                       /* '('  */
  YYSYMBOL_33_ = 33,                       /* ')'  */
  YYSYMBOL_34_ = 34,                       /* ','  */
  YYSYMBOL_35_ = 35,                       /* '['  */
  YYSYMBOL_36_ = 36,                       /* ':'  */
  YYSYMBOL_37_ = 37,                       /* ']'  */
  YYSYMBOL_38_ = 38,                       /* '+'  */
  YYSYMBOL_39_ = 39,                       /* '-'  */
  YYSYMBOL_40_ = 40,                       /* '*'  */
  YYSYMBOL_41_ = 41,                       /* '/'  */
  YYSYMBOL_42_ = 42,                       /* '%'  */
  YYSYMBOL_43_ = 43,                       /* '='  */
  YYSYMBOL_44_ = 44,                       /* '>'  */
  YYSYMBOL_45_ = 45,                       /* '<'  */
  YYSYMBOL_YYACCEPT = 46,                  /* $accept  */
  YYSYMBOL_program_all = 47,               /* program_all  */
  YYSYMBOL_procedures = 48,                /* procedures  */
  YYSYMBOL_main = 49,                      /* main  */
  YYSYMBOL_commands = 50,                  /* commands  */
  YYSYMBOL_command = 51,                   /* command  */
  YYSYMBOL_proc_head = 52,                 /* proc_head  */
  YYSYMBOL_proc_call = 53,                 /* proc_call  */
  YYSYMBOL_declarations = 54,              /* declarations  */
  YYSYMBOL_args_decl = 55,                 /* args_decl  */
  YYSYMBOL_args = 56,                      /* args  */
  YYSYMBOL_expression = 57,                /* expression  */
  YYSYMBOL_condition = 58,                 /* condition  */
  YYSYMBOL_value = 59,                     /* value  */
  YYSYMBOL_tidentifier = 60,               /* tidentifier  */
  YYSYMBOL_identifier = 61                 /* identifier  */
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
typedef yytype_uint8 yy_state_t;

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
#define YYFINAL  3
/* YYLAST -- Last index in YYTABLE.  */
#define YYLAST   271

/* YYNTOKENS -- Number of terminals.  */
#define YYNTOKENS  46
/* YYNNTS -- Number of nonterminals.  */
#define YYNNTS  16
/* YYNRULES -- Number of rules.  */
#define YYNRULES  49
/* YYNSTATES -- Number of states.  */
#define YYNSTATES  134

/* YYMAXUTOK -- Last valid token kind.  */
#define YYMAXUTOK   285


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
       2,     2,     2,     2,     2,     2,     2,    42,     2,     2,
      32,    33,    40,    38,    34,    39,     2,    41,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,    36,    31,
      45,    43,    44,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,    35,     2,    37,     2,     2,     2,     2,     2,     2,
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
      25,    26,    27,    28,    29,    30
};

#if YYDEBUG
/* YYRLINE[YYN] -- Source line where rule number YYN was defined.  */
static const yytype_int16 yyrline[] =
{
       0,    77,    77,   132,   175,   194,   201,   205,   211,   217,
     223,   250,   272,   288,   308,   338,   468,   469,   472,   491,
     511,   519,   566,   576,   592,   601,   618,   628,   638,   647,
     659,   664,   671,   686,   763,   767,   771,   775,   782,   794,
     805,   816,   827,   838,   852,   857,   865,   878,   890,   913
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
  "\"end of file\"", "error", "\"invalid token\"", "NUMBER",
  "pidentifier", "ASSIGNMENT", "NEQ", "GEQ", "LEQ", "BEGIN_KW", "PROGRAM",
  "PROCEDURE", "IS", "END", "IF", "THEN", "ELSE", "ENDIF", "WHILE", "DO",
  "ENDWHILE", "FOR", "FROM", "TO", "DOWNTO", "ENDFOR", "REPEAT", "UNTIL",
  "READ", "WRITE", "T", "';'", "'('", "')'", "','", "'['", "':'", "']'",
  "'+'", "'-'", "'*'", "'/'", "'%'", "'='", "'>'", "'<'", "$accept",
  "program_all", "procedures", "main", "commands", "command", "proc_head",
  "proc_call", "declarations", "args_decl", "args", "expression",
  "condition", "value", "tidentifier", "identifier", YY_NULLPTR
};

static const char *
yysymbol_name (yysymbol_kind_t yysymbol)
{
  return yytname[yysymbol];
}
#endif

#define YYPACT_NINF (-24)

#define yypact_value_is_default(Yyn) \
  ((Yyn) == YYPACT_NINF)

#define YYTABLE_NINF (-1)

#define yytable_value_is_error(Yyn) \
  0

/* YYPACT[STATE-NUM] -- Index in YYTABLE of the portion describing
   STATE-NUM.  */
static const yytype_int16 yypact[] =
{
     -24,     8,    28,   -24,     0,     6,   -24,    17,   -14,    11,
      -5,   242,    10,     3,    43,    47,    16,    60,    60,    57,
     242,    64,    60,    41,   -24,    58,    80,   242,    84,   -24,
      86,    24,   242,    15,    62,    87,    68,   -24,    65,    88,
      -2,   -24,    82,   -24,    89,   121,    71,    73,   -24,   -24,
     -24,    60,   148,    77,   -24,   -24,     7,   166,   242,   103,
     -24,    44,    76,    92,   242,    60,    60,    60,    60,    60,
      60,   242,    60,    60,   -24,   -24,    99,    55,   -24,   128,
     -24,   129,   -24,   184,    95,   -24,   130,   -24,   -24,    -1,
     -24,   -24,   -24,   -24,   -24,   -24,   197,    51,   105,   -24,
      60,    60,    60,    60,    60,   101,   -24,   -24,   -24,   -24,
     242,   -24,   -24,    60,    60,   -24,   -24,   -24,   -24,   -24,
     -24,   135,   210,   122,   124,   107,   -24,   242,   242,   -24,
     216,   229,   -24,   -24
};

/* YYDEFACT[STATE-NUM] -- Default reduction number in state STATE-NUM.
   Performed when YYTABLE does not specify something else to do.  Zero
   means the default is an error.  */
static const yytype_int8 yydefact[] =
{
       5,     0,     0,     1,     0,     0,     2,     0,     0,     0,
      24,     0,     0,     0,     0,     0,    47,     0,     0,     0,
       0,     0,     0,     0,     9,     0,     0,     0,     0,    28,
       0,     0,     0,     0,     0,     0,     0,    44,    47,     0,
       0,    45,     0,    46,     0,     0,     0,     0,     7,     8,
      17,     0,     0,    22,    29,    20,     0,     0,     0,     0,
      31,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,    18,    19,     0,    32,     6,     0,
      26,     0,     4,     0,     0,    21,     0,    49,    48,     0,
      39,    42,    43,    38,    40,    41,     0,     0,     0,    10,
       0,     0,     0,     0,     0,     0,    27,     3,    25,    30,
       0,    12,    13,     0,     0,    14,    33,    34,    35,    36,
      37,     0,     0,     0,     0,     0,    11,     0,     0,    23,
       0,     0,    15,    16
};

/* YYPGOTO[NTERM-NUM].  */
static const yytype_int8 yypgoto[] =
{
     -24,   -24,   -24,   -24,   -18,   -23,   -24,   -24,   126,   -24,
     -24,   -24,   -17,    54,   -24,    14
};

/* YYDEFGOTO[NTERM-NUM].  */
static const yytype_int8 yydefgoto[] =
{
       0,     1,     2,     6,    23,    24,     9,    25,    12,    31,
      61,    76,    39,    40,    44,    26
};

/* YYTABLE[YYPACT[STATE-NUM]] -- What to do in state STATE-NUM.  If
   positive, shift that token.  If negative, reduce the rule whose
   number is the opposite.  If YYTABLE_NINF, syntax error.  */
static const yytype_uint8 yytable[] =
{
      49,    42,    45,    16,    65,    66,    67,    29,     3,    52,
       8,    80,     7,    17,    57,   110,   111,    18,    13,    27,
      19,    10,    49,    14,    58,    20,    11,    21,    22,    49,
      15,    41,    41,    30,    49,    46,    41,    81,     4,     5,
      83,    68,    69,    70,    28,    16,    89,    10,    35,    28,
      34,    36,    32,    96,    48,    17,    98,    55,    56,    18,
      49,    43,    19,    37,    38,    41,    49,    20,    38,    21,
      22,    62,    63,    49,   113,   114,    47,    85,    86,    41,
      41,    41,    41,    41,    41,    51,    41,    41,    53,    50,
      54,    60,   122,   100,   101,   102,   103,   104,    59,    49,
      36,    71,    74,    64,    75,    77,    84,    49,    49,   130,
     131,    72,    79,    87,    41,    41,    41,    41,    41,    90,
      91,    92,    93,    94,    95,    16,    97,    41,    41,    88,
      99,   105,   108,   106,   109,    17,   115,   121,   125,    18,
      33,   127,    19,   128,   129,     0,     0,    20,    73,    21,
      22,     0,    16,     0,   116,   117,   118,   119,   120,     0,
       0,    78,    17,     0,     0,     0,    18,   123,   124,    19,
      16,     0,     0,     0,    20,     0,    21,    22,     0,    82,
      17,     0,     0,     0,    18,     0,     0,    19,    16,     0,
       0,     0,    20,     0,    21,    22,     0,   107,    17,     0,
       0,    16,    18,     0,     0,    19,     0,     0,     0,     0,
      20,    17,    21,    22,    16,    18,     0,   112,    19,     0,
      16,     0,     0,    20,    17,    21,    22,   126,    18,     0,
      17,    19,     0,    16,    18,     0,    20,    19,    21,    22,
       0,   132,    20,    17,    21,    22,    16,    18,     0,     0,
      19,     0,     0,     0,   133,    20,    17,    21,    22,     0,
      18,     0,     0,    19,     0,     0,     0,     0,    20,     0,
      21,    22
};

static const yytype_int16 yycheck[] =
{
      23,    18,    20,     4,     6,     7,     8,     4,     0,    27,
       4,     4,    12,    14,    32,    16,    17,    18,    32,     9,
      21,     4,    45,    12,     9,    26,     9,    28,    29,    52,
      35,    17,    18,    30,    57,    21,    22,    30,    10,    11,
      58,    43,    44,    45,    34,     4,    64,     4,    32,    34,
       3,    35,     9,    71,    13,    14,    73,    33,    34,    18,
      83,     4,    21,     3,     4,    51,    89,    26,     4,    28,
      29,     3,     4,    96,    23,    24,    22,    33,    34,    65,
      66,    67,    68,    69,    70,     5,    72,    73,     4,    31,
       4,     4,   110,    38,    39,    40,    41,    42,    36,   122,
      35,    19,    31,    15,    31,    51,     3,   130,   131,   127,
     128,    22,    35,    37,   100,   101,   102,   103,   104,    65,
      66,    67,    68,    69,    70,     4,    72,   113,   114,    37,
      31,     3,    37,     4,     4,    14,    31,    36,     3,    18,
      14,    19,    21,    19,    37,    -1,    -1,    26,    27,    28,
      29,    -1,     4,    -1,   100,   101,   102,   103,   104,    -1,
      -1,    13,    14,    -1,    -1,    -1,    18,   113,   114,    21,
       4,    -1,    -1,    -1,    26,    -1,    28,    29,    -1,    13,
      14,    -1,    -1,    -1,    18,    -1,    -1,    21,     4,    -1,
      -1,    -1,    26,    -1,    28,    29,    -1,    13,    14,    -1,
      -1,     4,    18,    -1,    -1,    21,    -1,    -1,    -1,    -1,
      26,    14,    28,    29,     4,    18,    -1,    20,    21,    -1,
       4,    -1,    -1,    26,    14,    28,    29,    17,    18,    -1,
      14,    21,    -1,     4,    18,    -1,    26,    21,    28,    29,
      -1,    25,    26,    14,    28,    29,     4,    18,    -1,    -1,
      21,    -1,    -1,    -1,    25,    26,    14,    28,    29,    -1,
      18,    -1,    -1,    21,    -1,    -1,    -1,    -1,    26,    -1,
      28,    29
};

/* YYSTOS[STATE-NUM] -- The symbol kind of the accessing symbol of
   state STATE-NUM.  */
static const yytype_int8 yystos[] =
{
       0,    47,    48,     0,    10,    11,    49,    12,     4,    52,
       4,     9,    54,    32,    12,    35,     4,    14,    18,    21,
      26,    28,    29,    50,    51,    53,    61,     9,    34,     4,
      30,    55,     9,    54,     3,    32,    35,     3,     4,    58,
      59,    61,    58,     4,    60,    50,    61,    59,    13,    51,
      31,     5,    50,     4,     4,    33,    34,    50,     9,    36,
       4,    56,     3,     4,    15,     6,     7,     8,    43,    44,
      45,    19,    22,    27,    31,    31,    57,    59,    13,    35,
       4,    30,    13,    50,     3,    33,    34,    37,    37,    50,
      59,    59,    59,    59,    59,    59,    50,    59,    58,    31,
      38,    39,    40,    41,    42,     3,     4,    13,    37,     4,
      16,    17,    20,    23,    24,    31,    59,    59,    59,    59,
      59,    36,    50,    59,    59,     3,    17,    19,    19,    37,
      50,    50,    25,    25
};

/* YYR1[RULE-NUM] -- Symbol kind of the left-hand side of rule RULE-NUM.  */
static const yytype_int8 yyr1[] =
{
       0,    46,    47,    48,    48,    48,    49,    49,    50,    50,
      51,    51,    51,    51,    51,    51,    51,    51,    51,    51,
      52,    53,    54,    54,    54,    54,    55,    55,    55,    55,
      56,    56,    57,    57,    57,    57,    57,    57,    58,    58,
      58,    58,    58,    58,    59,    59,    60,    61,    61,    61
};

/* YYR2[RULE-NUM] -- Number of symbols on the right-hand side of rule RULE-NUM.  */
static const yytype_int8 yyr2[] =
{
       0,     2,     2,     8,     7,     0,     6,     5,     2,     1,
       4,     7,     5,     5,     5,     9,     9,     2,     3,     3,
       4,     4,     3,     8,     1,     6,     3,     4,     1,     2,
       3,     1,     1,     3,     3,     3,     3,     3,     3,     3,
       3,     3,     3,     3,     1,     1,     1,     1,     4,     4
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
  case 2: /* program_all: procedures main  */
#line 77 "/mnt/c/Users/adame/CLionProjects/jftt_compiler/compiler/parser.y"
                    {
        auto& main_block = (yyvsp[0].attr)->translation;
        auto& procedures_block = (yyvsp[-1].attr)->translation;
        long line_count = 0;
        long translation_header_offset = 0;

        // TODO uncomment:
        // TODO ENHANCE postprocessing HERE
        // postprocessing moved to the end of the file


        // translate procedures block
        if (!procedures_block.empty()) {
            long procedures_block_size = 0; // instead of adding extra one to reach over the last line

            for (const auto& line : procedures_block) {
                procedures_block_size += 1 + std::ranges::count(line, '\n');
            }

            output_file << "JMP " << procedures_block_size + 1 << endl; // step over the block
            line_count++;
            //translation_header_offset++;

            for (auto& line : procedures_block) {
                //parse_line(line, line_count, translation_header_offset, cache_regs); // replaces this_line with actual line number considering header_offset
                output_file << line << endl;
                line_count++;
                line_count += std::ranges::count(line, '\n');;
            }
        }


        for (auto& line : main_block) {
            //parse_line(line, line_count, translation_header_offset, cache_regs); // replaces this_line with actual line number
            /*if (line.find("SET") != string::npos && line.find("[") == string::npos) {
                const auto rvalue = line.substr(4, line.find("#") == string::npos ? line.length() : line.find("#"));
                cout << "value: " << rvalue << "; count: " << cached_constants.count(stol(rvalue))  << endl;
                if (cached_constants.contains(stol(rvalue))) {
                    output_file << "LOAD [" << rvalue << "]" << line.substr(4 + rvalue.length(), line.length()) << endl;
                    //TODO replace by:
                    //output_file << "LOAD [" << regs.at(rvalue) << "]" << endl;
                    continue;
                }
            }*/ // no clue what this was supposed to do

            output_file << line << endl;
            line_count++;
            line_count += std::ranges::count(line, '\n');
        }
        output_file << "HALT";
        free((yyvsp[0].attr));
    }
#line 1309 "../compiler/parser.cpp"
    break;

  case 3: /* procedures: procedures PROCEDURE proc_head IS declarations BEGIN_KW commands END  */
#line 132 "/mnt/c/Users/adame/CLionProjects/jftt_compiler/compiler/parser.y"
                                                                         {
        const auto& fun_name = (yyvsp[-5].attr)->str_value;
        const auto& arguments = (yyvsp[-5].attr)->translation;
        const auto& declarations = (yyvsp[-3].attr)->translation;
        auto& proc_commands = (yyvsp[-1].attr)->translation;

        if (regs.contains(fun_name)){
            yyerror("procedure's name is ambiguous", yylineno, fun_name);
        }
        const auto return_reg = regs.add(fun_name);

        // store procedure's data
        const auto fun_line_no = (yyvsp[-7].attr)->translation.size(); // line indexes are shifted down by one
        const auto first_arg = regs.at(arguments.front());
        funs.add(fun_name, fun_line_no, arguments.size(), first_arg);

        // parse commands
        // switch arguments values into references
        for (auto& line : proc_commands) {
            list<long> argument_registers = {};
            for (const auto& pid : arguments) {
                argument_registers.emplace_back(regs.at(pid));
            }

            parse_proc_line(line, argument_registers);
        }

        (yyval.attr) = (yyvsp[-7].attr);
        (yyvsp[-7].attr)->translation.splice((yyvsp[-7].attr)->translation.end(), proc_commands);
        (yyval.attr)->translation.emplace_back("RTRN " + to_string(return_reg));

        // forget this context after moving to the next procedure
        for (const auto& pid : arguments) {
            regs.forget_pid(pid);
        }
        // TODO forget regular declarations
        for (const auto& pid : declarations) {
            regs.forget_pid(pid);
        }
        free((yyvsp[-5].attr));
        free((yyvsp[-3].attr));
        free((yyvsp[-1].attr));
    }
#line 1357 "../compiler/parser.cpp"
    break;

  case 4: /* procedures: procedures PROCEDURE proc_head IS BEGIN_KW commands END  */
#line 175 "/mnt/c/Users/adame/CLionProjects/jftt_compiler/compiler/parser.y"
                                                              {
        const auto& fun_name = (yyvsp[-4].attr)->str_value;
        const auto& arguments = (yyvsp[-4].attr)->translation;

        if (regs.contains(fun_name)){
            yyerror("procedure's name is ambiguous", yylineno, fun_name);
        }
        const auto return_reg = regs.add(fun_name);

        // store procedure's data
        const auto fun_line_no = (yyvsp[-6].attr)->translation.size() + 1;
        const auto first_arg = regs.at(arguments.front());
        funs.add(fun_name, fun_line_no, arguments.size(), first_arg);

        (yyval.attr) = (yyvsp[-6].attr);
        (yyvsp[-6].attr)->translation.splice((yyvsp[-6].attr)->translation.end(), (yyvsp[-1].attr)->translation);

        (yyval.attr)->translation.emplace_back("RTRN " + to_string(return_reg));
    }
#line 1381 "../compiler/parser.cpp"
    break;

  case 5: /* procedures: %empty  */
#line 194 "/mnt/c/Users/adame/CLionProjects/jftt_compiler/compiler/parser.y"
             {
        (yyval.attr) = new TokenAttribute();
        (yyval.attr)->translation = {};
    }
#line 1390 "../compiler/parser.cpp"
    break;

  case 6: /* main: PROGRAM IS declarations BEGIN_KW commands END  */
#line 201 "/mnt/c/Users/adame/CLionProjects/jftt_compiler/compiler/parser.y"
                                                  {
            (yyval.attr) = (yyvsp[-1].attr);
            free((yyvsp[-3].attr));
        }
#line 1399 "../compiler/parser.cpp"
    break;

  case 7: /* main: PROGRAM IS BEGIN_KW commands END  */
#line 205 "/mnt/c/Users/adame/CLionProjects/jftt_compiler/compiler/parser.y"
                                       {
            (yyval.attr) = (yyvsp[-1].attr);
        }
#line 1407 "../compiler/parser.cpp"
    break;

  case 8: /* commands: commands command  */
#line 211 "/mnt/c/Users/adame/CLionProjects/jftt_compiler/compiler/parser.y"
                     {
            //($1->translation).splice($1->translation.end(), $2->translation) // TODO why does it crash?
            (yyvsp[-1].attr)->translation.splice((yyvsp[-1].attr)->translation.end(), (yyvsp[0].attr)->translation);
            (yyval.attr) = (yyvsp[-1].attr);
            free((yyvsp[0].attr));
        }
#line 1418 "../compiler/parser.cpp"
    break;

  case 9: /* commands: command  */
#line 217 "/mnt/c/Users/adame/CLionProjects/jftt_compiler/compiler/parser.y"
              {
            (yyval.attr) = (yyvsp[0].attr);
        }
#line 1426 "../compiler/parser.cpp"
    break;

  case 10: /* command: identifier ASSIGNMENT expression ';'  */
#line 223 "/mnt/c/Users/adame/CLionProjects/jftt_compiler/compiler/parser.y"
                                         {
            /*if ($3->type == STRING) {
                output_file << "LOAD "<< $3->register_no << "\t#" << $3->str_value << endl;
            } else {
                output_file << "SET "<< $3->long_value << endl;
            }*/ // <- moved down the parse tree TODO : DELETE THIS

            (yyval.attr) = (yyvsp[-1].attr); // put expression->translation into $$->translation to avoid extra copies

            if ((yyvsp[-3].attr)->type == ADDRESS) {
                if ((yyvsp[-3].attr)->translation.empty()) { // pid[num] todo DELETE THIS CASE AS TRANSLATION CANNOT BE EMPTY ANYMORE!
                    // expression puts its value into r0!
                    (yyval.attr)->translation.emplace_back("STOREI " + to_string((yyvsp[-3].attr)->register_no) + "\t#" + (yyvsp[-3].attr)->str_value);
                    throw std::runtime_error("address translation is empty");
                } else { // pid[pid]
                    const auto tmp_reg = regs.add_rval(); // store address here
                    (yyvsp[-3].attr)->translation.emplace_back("STORE " + to_string(tmp_reg) + "\t#tmp pid address");
                    (yyval.attr)->translation.splice((yyval.attr)->translation.begin(), (yyvsp[-3].attr)->translation);
                    // expression puts its value into r0
                    (yyval.attr)->translation.emplace_back("STOREI " + to_string(tmp_reg) + "\t#" + (yyvsp[-3].attr)->str_value + "[..]");
                }
            } else {
                // expression value already in r0!
                (yyval.attr)->translation.emplace_back("STORE " + to_string((yyvsp[-3].attr)->register_no) + "\t#" + (yyvsp[-3].attr)->str_value + ((regs.get_pid((yyvsp[-3].attr)->str_value).size > 1) ? ("[" + to_string((yyvsp[-3].attr)->register_no - regs.at((yyvsp[-3].attr)->str_value) + regs.get_pid((yyvsp[-3].attr)->str_value).index_shift) + "]") : ""));
            }
            free((yyvsp[-3].attr));
        }
#line 1458 "../compiler/parser.cpp"
    break;

  case 11: /* command: IF condition THEN commands ELSE commands ENDIF  */
#line 250 "/mnt/c/Users/adame/CLionProjects/jftt_compiler/compiler/parser.y"
                                                     {
            (yyval.attr) = (yyvsp[-5].attr);
            if ((yyvsp[-5].attr)->type != LONG) { // condition is an lval
                (yyvsp[-5].attr)->translation.front().append("\t# if-else head");
                (yyval.attr)->translation.back().append(" " + to_string((yyvsp[-3].attr)->translation.size() + 2));  // enter adequate block (last jump opcode lacks an argument by default)

                (yyvsp[-3].attr)->translation.front().append("\t# if (cond ture) block");
                (yyval.attr)->translation.splice((yyval.attr)->translation.end(), (yyvsp[-3].attr)->translation); // paste if block
                (yyval.attr)->translation.emplace_back("JUMP " + to_string((yyvsp[-1].attr)->translation.size() + 1)); // omit else block

                (yyvsp[-1].attr)->translation.front().append("\t# else (cond false) block");
                (yyval.attr)->translation.splice((yyval.attr)->translation.end(), (yyvsp[-1].attr)->translation); // paste else block
            } else { // condition is an rval and is evaluated at compile-time
                if((yyvsp[-5].attr)->long_value == 0) {
                    (yyval.attr)->translation = (yyvsp[-3].attr)->translation; // just the if block
                } else {
                    (yyval.attr)->translation = (yyvsp[-1].attr)->translation; // just else block
                }
            }
            free((yyvsp[-3].attr));
            free((yyvsp[-1].attr));
        }
#line 1485 "../compiler/parser.cpp"
    break;

  case 12: /* command: IF condition THEN commands ENDIF  */
#line 272 "/mnt/c/Users/adame/CLionProjects/jftt_compiler/compiler/parser.y"
                                       {
            (yyval.attr) = (yyvsp[-3].attr);
            if ((yyvsp[-3].attr)->type != LONG) { // it's an lval
                (yyval.attr)->translation.front().append("\t# if-cond head");
                (yyval.attr)->translation.back().append(" " + to_string((yyvsp[-1].attr)->translation.size() + 1));
                (yyvsp[-1].attr)->translation.front().append("\t# if block");
                (yyval.attr)->translation.splice((yyval.attr)->translation.end(), (yyvsp[-1].attr)->translation);
            } else {
                if((yyvsp[-3].attr)->long_value != 0) { //cond always true
                    (yyval.attr)->translation = (yyvsp[-1].attr)->translation;
                } else { //cond false
                    (yyval.attr)->translation = {};
                }
            }
            free((yyvsp[-1].attr));
        }
#line 1506 "../compiler/parser.cpp"
    break;

  case 13: /* command: WHILE condition DO commands ENDWHILE  */
#line 288 "/mnt/c/Users/adame/CLionProjects/jftt_compiler/compiler/parser.y"
                                           {
            (yyval.attr) = (yyvsp[-3].attr);
            if ((yyvsp[-3].attr)->type == STRING) { //it's lval
                (yyvsp[-3].attr)->translation.front().append("\t# while-do head");
                (yyval.attr)->translation.back().append(" " + to_string((yyvsp[-1].attr)->translation.size() + 2)); // navigate case where condition is not true

                (yyvsp[-1].attr)->translation.front().append("\t# while commands block");
                (yyval.attr)->translation.splice((yyval.attr)->translation.end(), (yyvsp[-1].attr)->translation);
                (yyval.attr)->translation.emplace_back("JUMP -" + to_string((yyval.attr)->translation.size()));
            } else { // rval
                if ((yyvsp[-3].attr)->long_value == 0) { // always true
                    cerr << "Warning on line: " << yylineno << " an endless while loop!";
                    (yyval.attr)->translation = (yyvsp[-1].attr)->translation;
                    (yyval.attr)->translation.emplace_back("JUMP -" + to_string((yyval.attr)->translation.size()));
                } else {
                    (yyval.attr)->translation = {};
                }
            }
            free((yyvsp[-1].attr));
    }
#line 1531 "../compiler/parser.cpp"
    break;

  case 14: /* command: REPEAT commands UNTIL condition ';'  */
#line 308 "/mnt/c/Users/adame/CLionProjects/jftt_compiler/compiler/parser.y"
                                          {
            (yyval.attr) = (yyvsp[-3].attr);
            if ((yyvsp[-1].attr)->type == STRING) { //lval
                (yyval.attr)->translation.front().append("\t# repeat-until commands block");

                // invert condition (used only for shorter repeat-until loops)
                // TODO replace by some invert_condition($4) function call
                if ((yyvsp[-1].attr)->translation.back() == "JUMP") {
                    (yyvsp[-1].attr)->translation.pop_back();
                    (yyvsp[-1].attr)->translation.back() = (yyvsp[-1].attr)->translation.back().substr(0, (yyvsp[-1].attr)->translation.back().find(' '));

                } else {
                    (yyvsp[-1].attr)->translation.back() = (yyvsp[-1].attr)->translation.back().substr(0, (yyvsp[-1].attr)->translation.back().find(' '));
                    (yyvsp[-1].attr)->translation.back().append(" 2");
                    (yyvsp[-1].attr)->translation.emplace_back("JUMP");
                }

                (yyvsp[-1].attr)->translation.front().append("\t# repeat-until footer");
                (yyval.attr)->translation.splice((yyval.attr)->translation.end(), (yyvsp[-1].attr)->translation);
                (yyval.attr)->translation.back().append(" -" + to_string((yyval.attr)->translation.size() - 1));
            } else { // rval
                if ((yyvsp[-1].attr)->long_value == 0) {
                    cerr << "Warning on line: " << yylineno << " an endless while loop!" << endl;
                    (yyval.attr)->translation.emplace_back("JUMP -" + to_string((yyvsp[-3].attr)->translation.size()));
                } else {
                    // pass, commands already in $$
                }
            }
            free((yyvsp[-1].attr));
        }
#line 1566 "../compiler/parser.cpp"
    break;

  case 15: /* command: FOR tidentifier FROM value TO value DO commands ENDFOR  */
#line 338 "/mnt/c/Users/adame/CLionProjects/jftt_compiler/compiler/parser.y"
                                                             {
            (yyval.attr) = (yyvsp[-7].attr);

            const auto tid_register = regs.at((yyvsp[-7].attr)->str_value);

            // examine illegal alteration of temporary-identifier inside the loop
            auto line_count = 0;
            for (const auto& line : (yyvsp[-1].attr)->translation) {
                line_count++;
                if ((line.compare(0, 6 + (yyvsp[-7].attr)->str_value.length(), "STORE " + to_string(tid_register)) == 0) ||
                (line.compare(0, 4 + (yyvsp[-7].attr)->str_value.length(), "GET " + to_string(tid_register)) == 0)) {
                    yyerror("for-loop iterator modification inside the loop is forbidden!", (yyvsp[-7].attr)->lineno + line_count, (yyvsp[-7].attr)->str_value);
                }
            }
            // check for invalid range
            if ((yyvsp[-5].attr)->type == LONG && (yyvsp[-3].attr)->type == LONG &&
                (yyvsp[-5].attr)->long_value > (yyvsp[-3].attr)->long_value) {
                const auto error_msg = "FROM " + to_string((yyvsp[-5].attr)->long_value) + " TO " + to_string((yyvsp[-3].attr)->long_value);
                yyerror("invalid range in for loop", (yyvsp[-7].attr)->lineno, error_msg);
            }

            // create for_head
            string CONDITIONAL_JUMP; // used to remember if we did v1 - v2 or v2 - v1 to decide when to stop the loop in the footer
            list<string> for_head;
            list<string> for_footer;
            if ((yyvsp[-5].attr)->type == LONG && (yyvsp[-3].attr)->type == LONG) { // SPECIAL CASE [rVAL, rVAL] can be optimized like:
                for_head = {
                    "LOAD [" + to_string((yyvsp[-5].attr)->long_value - (yyvsp[-3].attr)->long_value) + "]", // store only one value v == v1-v2 <= 0
                    "STORE " + to_string(tid_register),
                };
                CONDITIONAL_JUMP = "JPOS";
                cached_constants.insert((yyvsp[-5].attr)->long_value - (yyvsp[-3].attr)->long_value);
            } else if ((yyvsp[-5].attr)->type == LONG && (yyvsp[-3].attr)->type == STRING) {
                for_head = {
                    "LOAD [" + to_string((yyvsp[-5].attr)->long_value) + "]",
                    "SUB "  + to_string((yyvsp[-3].attr)->register_no),
                    "STORE "+ to_string(tid_register),
                };
                CONDITIONAL_JUMP = "JPOS";
                cached_constants.insert((yyvsp[-5].attr)->long_value);
            } else if ((yyvsp[-5].attr)->type == STRING && (yyvsp[-3].attr)->type == LONG) {
                for_head = {
                    "LOAD [" + to_string((yyvsp[-3].attr)->long_value) + "]",
                    "SUB "  + to_string((yyvsp[-5].attr)->register_no),
                    "STORE "+ to_string(tid_register),
                };
                CONDITIONAL_JUMP = "JNEG";
                cached_constants.insert((yyvsp[-3].attr)->long_value);
            } else if ((yyvsp[-5].attr)->type == STRING && (yyvsp[-3].attr)->type == STRING) {
                for_head = {
                    "LOAD " + to_string((yyvsp[-3].attr)->register_no),
                    "SUB "  + to_string((yyvsp[-5].attr)->register_no),
                    "STORE "+ to_string(tid_register),
                };
                CONDITIONAL_JUMP = "JNEG";
            } else if ((yyvsp[-5].attr)->type == ADDRESS && (yyvsp[-3].attr)->type == ADDRESS) {
                const auto tmp_reg = regs.add_rval();
                for_head = (yyvsp[-5].attr)->translation; // load address1 to r0
                for_head.emplace_back("LOADI 0");
                for_head.emplace_back("STORE " + to_string(tmp_reg));
                for_head.splice(for_head.end(), (yyvsp[-3].attr)->translation);
                for_head.emplace_back("LOADI 0");
                for_head.emplace_back("SUB "   + to_string(tmp_reg));
                CONDITIONAL_JUMP = "JPOS";
            } else if ((yyvsp[-5].attr)->type == ADDRESS && (yyvsp[-3].attr)->type == LONG) {
                // TODO important not to forget that simple SET cache optimization fails for this case
                const auto tmp_reg = regs.add_rval();
                for_head = (yyvsp[-5].attr)->translation;
                for_head.emplace_back("LOADI 0");
                for_head.emplace_back("STORE " + to_string(tmp_reg));
                for_head.emplace_back("LOAD [" + to_string((yyvsp[-3].attr)->long_value) + "]");
                for_head.emplace_back("SUB "   + to_string(tmp_reg));
                CONDITIONAL_JUMP = "JNEG";
                cached_constants.insert((yyvsp[-3].attr)->long_value);
            } else if ((yyvsp[-5].attr)->type == LONG && (yyvsp[-3].attr)->type == ADDRESS) {
                const auto tmp_reg = regs.add_rval();
                for_head = (yyvsp[-3].attr)->translation;
                for_head.emplace_back("LOADI 0");
                for_head.emplace_back("STORE " + to_string(tmp_reg));
                for_head.emplace_back("LOAD [" + to_string((yyvsp[-5].attr)->long_value) + "]");
                for_head.emplace_back("SUB "   + to_string(tmp_reg));
                CONDITIONAL_JUMP = "JPOS";
                cached_constants.insert((yyvsp[-5].attr)->long_value);
            } else if ((yyvsp[-5].attr)->type == ADDRESS && (yyvsp[-3].attr)->type == STRING) {
                for_head = {
                    "LOADI 0",
                    "SUB " + to_string((yyvsp[-3].attr)->register_no),
                };
                for_head.splice(for_head.begin(), (yyvsp[-5].attr)->translation);
                CONDITIONAL_JUMP = "JPOS";
            } else if ((yyvsp[-5].attr)->type == STRING && (yyvsp[-3].attr)->type == ADDRESS) {
                for_head = {
                    "LOADI 0",
                    "SUB "  + to_string((yyvsp[-5].attr)->register_no),
                };
                for_head.splice(for_head.begin(), (yyvsp[-3].attr)->translation);
                CONDITIONAL_JUMP = "JNEG";
            }
            // conditional jump over the command block will soon be added down below!

            for_footer = {
                "LOAD " + to_string(tid_register),
                CONDITIONAL_JUMP + " 5", // exit for
                "ADD [1]",
                "STORE " + to_string(tid_register)
            };
            cached_constants.insert(1);// we need to cache one

            for_footer.emplace_back("JUMP -" + to_string(for_footer.size() + (yyvsp[-1].attr)->translation.size()));
            // jump to the first line of commands inside the loop ($8->translation)

            //comments
            for_head.front().append("\t# for loop head");
            for_footer.front().append("\t# for loop cond-footer");
            (yyvsp[-1].attr)->translation.front().append("\t# for loop commands block");

            //translation scheme
            (yyval.attr)->translation = for_head;
            if ((yyvsp[-5].attr)->type != LONG || (yyvsp[-3].attr)->type != LONG) {
                (yyval.attr)->translation.emplace_back(CONDITIONAL_JUMP + " " + to_string((yyvsp[-1].attr)->translation.size() + for_footer.size() + 1)); // jump out of the loop
            }
            (yyval.attr)->translation.splice((yyval.attr)->translation.end(), (yyvsp[-1].attr)->translation);
            (yyval.attr)->translation.splice((yyval.attr)->translation.end(), for_footer); // most important part of the loop

            //cleanup
            regs.remove((yyvsp[-7].attr)->str_value); // SAME AS regs.remove(tid_register);
            free((yyvsp[-5].attr));
            free((yyvsp[-3].attr));
            free((yyvsp[-1].attr));
        }
#line 1701 "../compiler/parser.cpp"
    break;

  case 17: /* command: proc_call ';'  */
#line 469 "/mnt/c/Users/adame/CLionProjects/jftt_compiler/compiler/parser.y"
                    {
            (yyval.attr) = (yyvsp[-1].attr);
    }
#line 1709 "../compiler/parser.cpp"
    break;

  case 18: /* command: READ identifier ';'  */
#line 472 "/mnt/c/Users/adame/CLionProjects/jftt_compiler/compiler/parser.y"
                          {
            (yyval.attr) = (yyvsp[-1].attr);
            if ((yyvsp[-1].attr)->type == ADDRESS) {
                const auto tmp_reg = regs.add_rval();
                // store the address to tmp_reg
                (yyval.attr)->translation.emplace_back("STORE " + to_string(tmp_reg));
                // load the value into r0
                (yyval.attr)->translation.emplace_back("GET 0");
                // store it where the tmp address points to
                (yyval.attr)->translation.emplace_back("STOREI " + to_string(tmp_reg));
            } else { // STRING or LONG
                // if it was a long then register_no is expected be 0
                (yyval.attr)->translation.emplace_back("GET " + to_string((yyvsp[-1].attr)->register_no));
            }

            // Deprecated:
            //$$ = $2;
            //$$->translation.emplace_back("GET " + to_string($2->register_no));
        }
#line 1733 "../compiler/parser.cpp"
    break;

  case 19: /* command: WRITE value ';'  */
#line 491 "/mnt/c/Users/adame/CLionProjects/jftt_compiler/compiler/parser.y"
                      {
            (yyval.attr) = (yyvsp[-1].attr);
            if ((yyvsp[-1].attr)->type == LONG) {
                (yyval.attr)->translation.emplace_back("LOAD [" + to_string((yyvsp[-1].attr)->long_value) + "]");
                (yyvsp[-1].attr)->register_no = 0;
                cached_constants.insert((yyvsp[-1].attr)->long_value);
            } else if ((yyvsp[-1].attr)->type == STRING) {
                // $2 already sets its value to r0
            } else if ((yyvsp[-1].attr)->type == ADDRESS) {
                (yyval.attr)->translation.emplace_back("LOADI 0");
                (yyvsp[-1].attr)->register_no = 0;
            } else {
                yyerror("invalid value type in WRITE operation");
            }

            (yyval.attr)->translation.emplace_back("PUT " + to_string((yyvsp[-1].attr)->register_no));
        }
#line 1755 "../compiler/parser.cpp"
    break;

  case 20: /* proc_head: pidentifier '(' args_decl ')'  */
#line 511 "/mnt/c/Users/adame/CLionProjects/jftt_compiler/compiler/parser.y"
                                  {
        (yyval.attr) = (yyvsp[-1].attr); // get translation from args_decl
        (yyval.attr)->str_value = (yyvsp[-3].attr)->str_value;
        free((yyvsp[-3].attr));
    }
#line 1765 "../compiler/parser.cpp"
    break;

  case 21: /* proc_call: pidentifier '(' args ')'  */
#line 519 "/mnt/c/Users/adame/CLionProjects/jftt_compiler/compiler/parser.y"
                             {
        const auto& fun_name = (yyvsp[-3].attr)->str_value;
        if (!regs.contains(fun_name)) {
            yyerror("procedure not declared", yylineno, fun_name);
        }

        (yyval.attr) = (yyvsp[-3].attr);
        (yyval.attr)->translation = {};
        const auto& arguments = (yyvsp[-1].attr)->translation;

        auto arg_count = 0;
        const auto arg_no = funs.get_arg_count(fun_name);
        const auto first_arg_register = funs.get_first_arg_register(fun_name);
        for (const auto& pid : arguments) {
            arg_count++;
            if (arg_count > arg_no) {
                yyerror("too many arguments in procedure call", yylineno, fun_name);
            }

            const auto& pid_type = regs.get_pid(pid);
            if (pid_type.size == 1) {
                (yyval.attr)->translation.emplace_back("LOAD [" + to_string(regs.at(pid)) + "]" + "\t# passing address");
            } else {
                (yyval.attr)->translation.emplace_back("LOAD [" + to_string(regs.at(pid) - pid_type.index_shift) + "]\t# passing address of an array");
            }


            (yyval.attr)->translation.emplace_back(
                "STORE " + to_string(first_arg_register + arg_count - 1)
            );
            cached_constants.insert(regs.at(pid));
        }
        if (arg_count < arg_no) {
            yyerror("too few arguments in procedure call. expected: " + to_string(arg_no) + ", got: " + to_string(arg_count), yylineno, fun_name);
        }

        // TODO figure out the line_no
        //const auto this_line = 101010101010;
        (yyval.attr)->translation.emplace_back("LOAD [this_line + 3]");
        (yyval.attr)->translation.emplace_back("STORE " + to_string(regs.at(fun_name)));
        // TODO cached_constants.insert(this_line + 3);
        (yyval.attr)->translation.emplace_back("JUMP " + to_string(funs.get_line_no(fun_name)) + " - this_line");
        free((yyvsp[-1].attr));
    }
#line 1814 "../compiler/parser.cpp"
    break;

  case 22: /* declarations: declarations ',' pidentifier  */
#line 566 "/mnt/c/Users/adame/CLionProjects/jftt_compiler/compiler/parser.y"
                                 {
            const auto& pid = (yyvsp[0].attr)->str_value;
            if (regs.contains(pid)) {
                yyerror("identifier redeclaration", yylineno-1, pid);
            }
            regs.add(pid);
            (yyval.attr) = (yyvsp[-2].attr);
            (yyval.attr)->translation.emplace_back(pid);
            free((yyvsp[0].attr));
        }
#line 1829 "../compiler/parser.cpp"
    break;

  case 23: /* declarations: declarations ',' pidentifier '[' NUMBER ':' NUMBER ']'  */
#line 576 "/mnt/c/Users/adame/CLionProjects/jftt_compiler/compiler/parser.y"
                                                             {
            const auto& pid = (yyvsp[-5].attr)->str_value;
            if (regs.contains(pid)) {
                yyerror("identifier redeclaration", yylineno-1, pid);
            }
            try {
                regs.add_table(pid, (yyvsp[-3].attr)->long_value, (yyvsp[-1].attr)->long_value);
            } catch (std::runtime_error e) {
                yyerror(e.what());
            }
            (yyval.attr) = (yyvsp[-7].attr);
            (yyval.attr)->translation.emplace_back(pid);
            free((yyvsp[-5].attr));
            free((yyvsp[-3].attr));
            free((yyvsp[-1].attr));
        }
#line 1850 "../compiler/parser.cpp"
    break;

  case 24: /* declarations: pidentifier  */
#line 592 "/mnt/c/Users/adame/CLionProjects/jftt_compiler/compiler/parser.y"
                  {
            const auto& pid = (yyvsp[0].attr)->str_value;
            if (regs.contains(pid)) {
                yyerror("identifier redeclaration", yylineno-1, pid);
            }
            regs.add(pid);
            (yyval.attr) = (yyvsp[0].attr);
            (yyval.attr)->translation = {pid};
        }
#line 1864 "../compiler/parser.cpp"
    break;

  case 25: /* declarations: pidentifier '[' NUMBER ':' NUMBER ']'  */
#line 601 "/mnt/c/Users/adame/CLionProjects/jftt_compiler/compiler/parser.y"
                                            {
            const auto& pid = (yyvsp[-3].attr)->str_value;
            if (regs.contains(pid)) {
                yyerror("identifier redeclaration", yylineno-1, pid);
            }
            try {
                regs.add_table((yyvsp[-5].attr)->str_value, (yyvsp[-3].attr)->long_value, (yyvsp[-1].attr)->long_value);
            } catch (std::runtime_error e) {
                yyerror(e.what());
            }
            (yyval.attr) = (yyvsp[-5].attr);
            (yyval.attr)->translation = {pid};
            free((yyvsp[-3].attr));
            free((yyvsp[-1].attr));
        }
#line 1884 "../compiler/parser.cpp"
    break;

  case 26: /* args_decl: args_decl ',' pidentifier  */
#line 618 "/mnt/c/Users/adame/CLionProjects/jftt_compiler/compiler/parser.y"
                              {
        const auto& pid = (yyvsp[0].attr)->str_value;
        if (regs.contains(pid)) {
          yyerror("identifier redeclaration", yylineno-1, pid);
        }
        regs.add(pid);
        (yyval.attr) = (yyvsp[-2].attr);
        (yyval.attr)->translation.emplace_back(pid);
        free((yyvsp[0].attr));
    }
#line 1899 "../compiler/parser.cpp"
    break;

  case 27: /* args_decl: args_decl ',' T pidentifier  */
#line 628 "/mnt/c/Users/adame/CLionProjects/jftt_compiler/compiler/parser.y"
                                  {
        const auto& pid = (yyvsp[0].attr)->str_value;
        if (regs.contains(pid)) {
            yyerror("identifier redeclaration", yylineno-1, pid);
        }
        regs.add_proc_table(pid);
        (yyval.attr) = (yyvsp[-3].attr);
        (yyval.attr)->translation.emplace_back(pid);
        free((yyvsp[0].attr));
    }
#line 1914 "../compiler/parser.cpp"
    break;

  case 28: /* args_decl: pidentifier  */
#line 638 "/mnt/c/Users/adame/CLionProjects/jftt_compiler/compiler/parser.y"
                  {
        const auto& pid = (yyvsp[0].attr)->str_value;
        if (regs.contains(pid)) {
          yyerror("identifier redeclaration", yylineno-1, pid);
        }
        regs.add(pid);
        (yyval.attr) = (yyvsp[0].attr);
        (yyval.attr)->translation = {pid};
    }
#line 1928 "../compiler/parser.cpp"
    break;

  case 29: /* args_decl: T pidentifier  */
#line 647 "/mnt/c/Users/adame/CLionProjects/jftt_compiler/compiler/parser.y"
                    {
        const auto& pid = (yyvsp[0].attr)->str_value;
        if (regs.contains(pid)) {
            yyerror("identifier redeclaration", yylineno-1, pid);
        }
        regs.add(pid);
        (yyval.attr) = (yyvsp[0].attr);
        (yyval.attr)->translation = {pid};
    }
#line 1942 "../compiler/parser.cpp"
    break;

  case 30: /* args: args ',' pidentifier  */
#line 659 "/mnt/c/Users/adame/CLionProjects/jftt_compiler/compiler/parser.y"
                         {
        (yyval.attr) = (yyvsp[-2].attr);
        (yyval.attr)->translation.emplace_back((yyvsp[0].attr)->str_value);
        free((yyvsp[0].attr));
    }
#line 1952 "../compiler/parser.cpp"
    break;

  case 31: /* args: pidentifier  */
#line 664 "/mnt/c/Users/adame/CLionProjects/jftt_compiler/compiler/parser.y"
                  {
        (yyval.attr) = (yyvsp[0].attr);
        (yyval.attr)->translation = {(yyvsp[0].attr)->str_value};
    }
#line 1961 "../compiler/parser.cpp"
    break;

  case 32: /* expression: value  */
#line 671 "/mnt/c/Users/adame/CLionProjects/jftt_compiler/compiler/parser.y"
          {
            (yyval.attr) = (yyvsp[0].attr);
            if ((yyvsp[0].attr)->type == STRING) {
                (yyval.attr)->translation = {"LOAD " + to_string((yyvsp[0].attr)->register_no)};
            } else if ((yyvsp[0].attr)->type == LONG) {
                cached_constants.insert((yyvsp[0].attr)->long_value); // missed optimization if constant is used only once and only in main outside of any loop
                (yyval.attr)->translation = {"LOAD [" + to_string((yyvsp[0].attr)->long_value) + "]"};
                cached_constants.insert((yyvsp[0].attr)->long_value);
            } else if ((yyvsp[0].attr)->type == ADDRESS) {
                (yyval.attr)->translation.push_back("LOADI 0");
                (yyval.attr)->type = STRING; // treat as string TODO: it may turn out useless
            } else {
                yyerror("value has invalid type");
            }
        }
#line 1981 "../compiler/parser.cpp"
    break;

  case 33: /* expression: value '+' value  */
#line 686 "/mnt/c/Users/adame/CLionProjects/jftt_compiler/compiler/parser.y"
                      {
            const auto val = (yyvsp[-2].attr)->long_value + (yyvsp[0].attr)->long_value;

            // TODO refactor (maybe pass a reference to cached_constants to parse_expression):
            if((yyvsp[-2].attr)->type == LONG || (yyvsp[0].attr)->type == LONG) {
                long new_constant;
                if ((yyvsp[-2].attr)->type == LONG && (yyvsp[0].attr)->type == LONG) {
                    new_constant = val;
                } else if ((yyvsp[-2].attr)->type == LONG) {
                    new_constant = (yyvsp[-2].attr)->long_value;
                } else if ((yyvsp[0].attr)->type == LONG) {
                    new_constant = (yyvsp[0].attr)->long_value;
                }
                cached_constants.insert(new_constant); // missed optimization
            }

            (yyval.attr) = parse_expression((yyvsp[-2].attr), (yyvsp[0].attr), "ADD", "ADD", val, regs.add_rval());

            /* todo delete:
            if ($1->type == ADDRESS || $3->type == ADDRESS) {
                auto swap = $1->type != ADDRESS;
                auto * const adr_token = swap ? $3 : $1;
                auto * const other_token = swap ? $1 : $3;
                $$ = address_token; // 1st address already in r0
                $$->translation.emplace_back("LOADI 0");

                if (other_token->type == ADDRESS) {
                    const auto tmp_reg = regs.add_rval();
                    $$->translation.emplace_back("STORE " + to_string(tmp_reg));
                    $$->translation.splice($$->translation.end(), other_token->translation);
                    $$->translation.emplace_back("LOADI 0");
                    $$->translation.emplace_back("ADD " + to_string(tmp_reg));
                } else if (other_token->type == STRING) {
                    $$->translation.emplace_back("ADD " + to_string(other_token->register_no);
                } else if (other_token->type == LONG) {
                    $$->translation.emplace_back("ADD [" + to_string(other_token->long_value) + "]");
                } else {
                    yyerror("invalid value type inside of expression");
                }
                free(other_token);
            } else if ($1->type == $3->type) {
                $$ = $1;
                if ($1->type == STRING) {
                    // PID + PID
                    $$->translation.emplace_back("LOAD " + to_string($1->register_no));
                    $$->translation.emplace_back("ADD " + to_string($3->register_no));
                } else {
                    // RVAL + RVAL
                    $$->long_value = $1->long_value + $3->long_value;
                    $$->translation.emplace_back("LOAD [" + to_string($$->long_value) + "]");
                    cached_constants.insert($$->long_value);
                }
                free($3);
            } else {
                // RVAL + PID
                TokenAttribute* str_token;
                TokenAttribute* num_token;
                if ($1->type == STRING) {
                    str_token = $1;
                    num_token = $3;
                } else {
                    str_token = $3;
                    num_token = $1;
                }
                $$ = str_token; // ensure $$ has the rval register and string type

                $$->translation.emplace_back("LOAD [" + to_string(num_token->long_value) + "]");
                cached_constants.insert($$->long_value);
                $$->translation.emplace_back("ADD " + to_string(str_token->register_no));
                free(num_token);
            }

            $$->str_value = "rval";
            $$->register_no = 0; // RESULT OF THE EXPRESSION STORED IN R0!
            //EXCESS TOKEN ALREADY CLEANED UP!
            */
        }
#line 2063 "../compiler/parser.cpp"
    break;

  case 34: /* expression: value '-' value  */
#line 763 "/mnt/c/Users/adame/CLionProjects/jftt_compiler/compiler/parser.y"
                      {
        const auto val = (yyvsp[-2].attr)->long_value - (yyvsp[0].attr)->long_value;
        (yyval.attr) = parse_expression((yyvsp[-2].attr), (yyvsp[0].attr), "SUB", "RSUB", val, regs.add_rval());
    }
#line 2072 "../compiler/parser.cpp"
    break;

  case 35: /* expression: value '*' value  */
#line 767 "/mnt/c/Users/adame/CLionProjects/jftt_compiler/compiler/parser.y"
                        {
        const auto val = (yyvsp[-2].attr)->long_value * (yyvsp[0].attr)->long_value;
        (yyval.attr) = parse_expression((yyvsp[-2].attr), (yyvsp[0].attr), "MUL", "MUL", val, regs.add_rval());
    }
#line 2081 "../compiler/parser.cpp"
    break;

  case 36: /* expression: value '/' value  */
#line 771 "/mnt/c/Users/adame/CLionProjects/jftt_compiler/compiler/parser.y"
                        {
        const auto val = (yyvsp[0].attr)->long_value == 0 ? 0 : (yyvsp[-2].attr)->long_value / (yyvsp[0].attr)->long_value;
        (yyval.attr) = parse_expression((yyvsp[-2].attr), (yyvsp[0].attr), "DIV", "RDIV", val, regs.add_rval());
    }
#line 2090 "../compiler/parser.cpp"
    break;

  case 37: /* expression: value '%' value  */
#line 775 "/mnt/c/Users/adame/CLionProjects/jftt_compiler/compiler/parser.y"
                        {
        const auto val = (yyvsp[0].attr)->long_value == 0 ? 0 : (yyvsp[-2].attr)->long_value % (yyvsp[0].attr)->long_value;
        (yyval.attr) = parse_expression((yyvsp[-2].attr), (yyvsp[0].attr), "MOD", "RMOD", val, regs.add_rval());
    }
#line 2099 "../compiler/parser.cpp"
    break;

  case 38: /* condition: value '=' value  */
#line 782 "/mnt/c/Users/adame/CLionProjects/jftt_compiler/compiler/parser.y"
                    { //EXCESS TOKEN ALREADY CLEANED UP BY PARSE FUNCTION!
            (yyval.attr) = parse_condition(
                (yyvsp[-2].attr), (yyvsp[0].attr),
                list<string>({"JZERO 2", "JUMP"}), list<string>({"JZERO 2", "JUMP"}),
                ((yyvsp[-2].attr)->long_value == (yyvsp[0].attr)->long_value),
                regs.add_rval(),
                yylineno,
                cached_constants
            );
            // store an inverse jump (a weird somersault for smaller repeat-until loops)
            (yyval.attr)->str_value = "JZERO";
        }
#line 2116 "../compiler/parser.cpp"
    break;

  case 39: /* condition: value NEQ value  */
#line 794 "/mnt/c/Users/adame/CLionProjects/jftt_compiler/compiler/parser.y"
                        {
            (yyval.attr) = parse_condition(
                (yyvsp[-2].attr), (yyvsp[0].attr),
                list<string>({"JZERO"}), list<string>({"JZERO"}),
                ((yyvsp[-2].attr)->long_value != (yyvsp[0].attr)->long_value),
                regs.add_rval(),
                yylineno,
                cached_constants
            );
            (yyval.attr)->str_value = "JZERO 2\nJUMP";
        }
#line 2132 "../compiler/parser.cpp"
    break;

  case 40: /* condition: value '>' value  */
#line 805 "/mnt/c/Users/adame/CLionProjects/jftt_compiler/compiler/parser.y"
                        { //EXCESS TOKEN ALREADY CLEANED UP!
            (yyval.attr) = parse_condition(
                (yyvsp[-2].attr), (yyvsp[0].attr),
                list<string>({"JPOS 2", "JUMP"}), list<string>({"JNEG 2", "JUMP"}),
                ((yyvsp[-2].attr)->long_value > (yyvsp[0].attr)->long_value),
                regs.add_rval(),
                yylineno,
                cached_constants
            );
            (yyval.attr)->str_value = "JPOS";
        }
#line 2148 "../compiler/parser.cpp"
    break;

  case 41: /* condition: value '<' value  */
#line 816 "/mnt/c/Users/adame/CLionProjects/jftt_compiler/compiler/parser.y"
                        {
            (yyval.attr) = parse_condition(
                (yyvsp[-2].attr), (yyvsp[0].attr),
                list<string>({"JNEG 2", "JUMP"}), list<string>({"JPOS 2", "JUMP"}),
                ((yyvsp[-2].attr)->long_value < (yyvsp[0].attr)->long_value),
                regs.add_rval(),
                yylineno,
                cached_constants
            );
            (yyval.attr)->str_value = "JNEG";
        }
#line 2164 "../compiler/parser.cpp"
    break;

  case 42: /* condition: value GEQ value  */
#line 827 "/mnt/c/Users/adame/CLionProjects/jftt_compiler/compiler/parser.y"
                        {
            (yyval.attr) = parse_condition(
                (yyvsp[-2].attr), (yyvsp[0].attr),
                list<string>({"JNEG"}), list<string>({"JPOS"}),
                ((yyvsp[-2].attr)->long_value >= (yyvsp[0].attr)->long_value),
                regs.add_rval(),
                yylineno,
                cached_constants
            );
            (yyval.attr)->str_value = "JNEG 2\nJUMP";
        }
#line 2180 "../compiler/parser.cpp"
    break;

  case 43: /* condition: value LEQ value  */
#line 838 "/mnt/c/Users/adame/CLionProjects/jftt_compiler/compiler/parser.y"
                        {
            (yyval.attr) = parse_condition(
                (yyvsp[-2].attr), (yyvsp[0].attr),
                list<string>({"JPOS"}), list<string>({"JNEG"}),
                ((yyvsp[-2].attr)->long_value > (yyvsp[0].attr)->long_value),
                regs.add_rval(),
                yylineno,
                cached_constants
            );
            (yyval.attr)->str_value = "JPOS 2\nJUMP";
        }
#line 2196 "../compiler/parser.cpp"
    break;

  case 44: /* value: NUMBER  */
#line 852 "/mnt/c/Users/adame/CLionProjects/jftt_compiler/compiler/parser.y"
           {
            (yyval.attr) = (yyvsp[0].attr);
            //$$->str_value = "rval";
            (yyval.attr)->type = LONG;
        }
#line 2206 "../compiler/parser.cpp"
    break;

  case 45: /* value: identifier  */
#line 857 "/mnt/c/Users/adame/CLionProjects/jftt_compiler/compiler/parser.y"
                 {
            (yyval.attr) = (yyvsp[0].attr);
            //$$->str_value = $1->str_value;
            //$$->register_no = $1->register_no;
        }
#line 2216 "../compiler/parser.cpp"
    break;

  case 46: /* tidentifier: pidentifier  */
#line 865 "/mnt/c/Users/adame/CLionProjects/jftt_compiler/compiler/parser.y"
                {
            if (regs.contains((yyvsp[0].attr)->str_value)) {
                yyerror("ambiguous declaration");
                //throw std::runtime_error("Error: line no: " + to_string(yylineno) + " ambuguous identifier " + $1->str_value);
            }
            (yyval.attr) = (yyvsp[0].attr);
            (yyval.attr)->str_value = (yyvsp[0].attr)->str_value;
            (yyval.attr)->lineno = yylineno;
            (yyval.attr)->register_no = regs.add((yyvsp[0].attr)->str_value);
        }
#line 2231 "../compiler/parser.cpp"
    break;

  case 47: /* identifier: pidentifier  */
#line 878 "/mnt/c/Users/adame/CLionProjects/jftt_compiler/compiler/parser.y"
                {
            //translation stays empty as we know the pid register location
            (yyval.attr) = (yyvsp[0].attr);
            //$$->str_value = $1->str_value;
            (yyval.attr)->type = STRING;
            (yyval.attr)->lineno = yylineno;
            if (!regs.contains((yyvsp[0].attr)->str_value)) {
                yyerror("undefined identifier", yylineno, (yyval.attr)->str_value);
            }
            (yyval.attr)->register_no = regs.at((yyvsp[0].attr)->str_value);
            //cout << "pid: " << $$->str_value << " with register_no " << $$->register_no << endl;
        }
#line 2248 "../compiler/parser.cpp"
    break;

  case 48: /* identifier: pidentifier '[' pidentifier ']'  */
#line 890 "/mnt/c/Users/adame/CLionProjects/jftt_compiler/compiler/parser.y"
                                      {
            (yyval.attr) = (yyvsp[-3].attr);
            if (!regs.contains((yyvsp[-3].attr)->str_value)) {
                yyerror("undefined identifier", yylineno, (yyvsp[-3].attr)->str_value);
            }

            const auto& table_pid_type = regs.get_pid((yyvsp[-3].attr)->str_value);

            //CASE: IT IS A TABLE
            if (table_pid_type.size != 0) {
                const auto tab_offset = regs.at((yyvsp[-3].attr)->str_value) - regs.get_pid((yyvsp[-3].attr)->str_value).index_shift;
                cached_constants.insert(tab_offset);    // missed optimization
                (yyval.attr)->translation.emplace_back("LOAD [" + to_string(tab_offset) + "]");
                (yyval.attr)->translation.emplace_back("ADD " + to_string(regs.at((yyvsp[-1].attr)->str_value)));
            } else { //CASE: IT IS A TABLE REFERENCE
                (yyval.attr)->translation.emplace_back("LOAD " + to_string(regs.at((yyvsp[-3].attr)->str_value)) + "\t# passing address");
                (yyval.attr)->translation.emplace_back("ADD " + to_string(regs.at((yyvsp[-1].attr)->str_value)));
            }

            (yyval.attr)->type = ADDRESS;
            (yyval.attr)->lineno = yylineno;
            free((yyvsp[-1].attr));
        }
#line 2276 "../compiler/parser.cpp"
    break;

  case 49: /* identifier: pidentifier '[' NUMBER ']'  */
#line 913 "/mnt/c/Users/adame/CLionProjects/jftt_compiler/compiler/parser.y"
                                 {
            (yyval.attr) = (yyvsp[-3].attr);
            if (!regs.contains((yyvsp[-3].attr)->str_value)) {
                yyerror("undefined identifier", yylineno, (yyvsp[-3].attr)->str_value);
            }
            const auto value = (yyvsp[-1].attr)->long_value;
            const auto table_pid_type = regs.get_pid((yyvsp[-3].attr)->str_value);
            const auto is_reference = table_pid_type.size == 0;

            //CASE: IT IS A TABLE REFERENCE
            if (is_reference) {
                //create a translation that puts address in r0
                (yyval.attr)->translation.emplace_back("LOAD " + to_string(regs.at((yyvsp[-3].attr)->str_value)) + "\t# passing address");
                (yyval.attr)->translation.emplace_back("ADD [" + to_string(value) + "]");
                (yyval.attr)->type = ADDRESS;
                cached_constants.insert(value);
            } else { //CASE: IT IS A TABLE
                //translation stays empty as we know the pid register location
                try {
                    (yyval.attr)->register_no = regs.at((yyvsp[-3].attr)->str_value, (yyvsp[-1].attr)->long_value);
                } catch (const std::runtime_error& e) {
                    yyerror(e.what(), yylineno, (yyvsp[-3].attr)->str_value + "[" + to_string(value) + "]");
                }
                (yyval.attr)->type = STRING; // treat as regular pid as we know exact location of the register
            }

            (yyval.attr)->lineno = yylineno;
            free((yyvsp[-1].attr));
        }
#line 2310 "../compiler/parser.cpp"
    break;


#line 2314 "../compiler/parser.cpp"

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

#line 944 "/mnt/c/Users/adame/CLionProjects/jftt_compiler/compiler/parser.y"



void yyerror(const char *s) {
    cerr << "\nError: " << s << endl;
    if (yytext && yytext[0] != '\n' && yytext[0] != '\r') {

    }
    fprintf(stdout, "Unexpected token: '%s'\n", yytext ? yytext : "UNKNOWN");

    cerr << "Line number: " << yylineno << endl;

    exit(1);

//    yyparse();
}

void yyerror(const std::string& msg, int lineno, const std::string& lexem) {
    cerr << "\nError: " << msg << endl;
    //if (yytext && yytext[0] != '\n' && yytext[0] != '\r') {
    cerr << "Unexpected token: " << lexem << endl;
    cerr << "Line number: " << lineno << endl;
    exit(1);
//    yyparse(); //TODO
}

int main(int argc, char* argv[]) {
    // correct number of arguments is provided
    if (argc != 3) {
        cerr << "Usage: " << argv[0] << " <input_file> <output_file>" << endl;
        return 1;
    }

    auto output_filename = argv[2];
    auto input_filename = argv[1];

    // Open the output file
    output_file.open(output_filename);
    if (!output_file) {
        cerr << "Error: Could not open output file " << argv[2] << endl;
        return 1;
    }

    // Open the input file
    yyin = fopen(input_filename, "r");
    if (!yyin) {
        cerr << "Error: Could not open input file " << argv[1] << endl;
        return 1;
    }

    yylineno = 1;
    int parse_result = yyparse();

    fclose(yyin);
    output_file.close();

    postprocess(output_filename, regs);

    if (parse_result == 0) {
        printf("Parsing completed successfully\n");
    } else {
        fprintf(stderr, "Parsing failed\n");
        return 1;
    }

    cout << "Generated output into: " << argv[2] << endl;
    return 0;
}

