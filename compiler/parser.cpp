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

extern int yylineno;
extern FILE* yyin;
extern char* yytext;
std::ofstream output_file;

void yyerror(const char *);
extern int my_yylex();
#define yylex my_yylex

using namespace std;

struct PairHash {
    template <typename T1, typename T2>
    std::size_t operator()(const std::pair<T1, T2>& pair) const {
        std::size_t h1 = std::hash<T1>()(pair.first);
        std::size_t h2 = std::hash<T2>()(pair.second);
        return h1 ^ (h2 << 1); // Combine the two hashes
    }
};

class register_table {
private:
    struct pid_type {
        //auto type = INTEGER (this is a toy compiler)
        size_t size;
        int register_no; //first reg occupied by this pid
    };

    int available_register(void) {
        if (!free_registers.empty()) {
            auto it = free_registers.begin();
            auto reg = (*it).first;

            if (it->first + 1 < it->second) {
                free_registers.insert(make_pair(it->first + 1, it->second));
            }
            free_registers.erase(it);

            return reg;
        }
        return -1;
        //TODO throw something
    }

    std::unordered_set<std::pair<int, int>, PairHash> free_registers; // (interval)
    std::unordered_map<std::string, pid_type> table; // (pid, (size,register))

public:
    register_table() : free_registers(), table() {
        free_registers.insert(make_pair(1,2<<10));
    }

    void remove(const string& pid) {
        pid_type pid_t = table.at(pid);
        int from = pid_t.register_no;
        int to = from + pid_t.size;
        free_registers.insert(make_pair(from, to));
        table.erase(pid);
    }

    int at(const string& pid) {
        return table.at(pid).register_no;
    }

    int add(const string& pid) {
        if (table.contains(pid)){
            //TODO report pid redeclaration
            return -1;
        }

        pid_type new_pid;
        new_pid.size = 1;
        new_pid.register_no = available_register();

        table[pid] = new_pid;
        return new_pid.register_no;
    }

    void add_table(const string& pid, int from, int to) {

    }
};

register_table regs;


#line 169 "../compiler/parser.cpp"

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
  YYSYMBOL_identifier = 60                 /* identifier  */
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
#define YYLAST   287

/* YYNTOKENS -- Number of terminals.  */
#define YYNTOKENS  46
/* YYNNTS -- Number of nonterminals.  */
#define YYNNTS  15
/* YYNRULES -- Number of rules.  */
#define YYNRULES  48
/* YYNSTATES -- Number of states.  */
#define YYNSTATES  133

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
       0,   135,   135,   139,   140,   141,   145,   146,   150,   151,
     155,   165,   166,   167,   168,   169,   170,   171,   172,   176,
     183,   187,   191,   195,   198,   199,   204,   205,   206,   207,
     211,   212,   216,   226,   227,   228,   229,   230,   234,   235,
     236,   237,   238,   239,   243,   248,   257,   264,   282
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
  "condition", "value", "identifier", YY_NULLPTR
};

static const char *
yysymbol_name (yysymbol_kind_t yysymbol)
{
  return yytname[yysymbol];
}
#endif

#define YYPACT_NINF (-27)

#define yypact_value_is_default(Yyn) \
  ((Yyn) == YYPACT_NINF)

#define YYTABLE_NINF (-1)

#define yytable_value_is_error(Yyn) \
  0

/* YYPACT[STATE-NUM] -- Index in YYTABLE of the portion describing
   STATE-NUM.  */
static const yytype_int16 yypact[] =
{
     -27,    17,     8,   -27,    15,    25,   -27,     7,    21,    31,
      27,   258,     3,     2,    11,    57,    14,    44,    44,    60,
     258,    63,    44,   119,   -27,    45,    78,   258,    82,   -27,
      83,   -26,   258,     4,    52,    85,    47,   -27,    55,    77,
      -3,   -27,    74,    72,   146,    64,    65,   -27,   -27,   -27,
      44,   164,    62,   -27,   -27,     6,   182,   258,    96,   -27,
      37,    67,    68,   258,    44,    44,    44,    44,    44,    44,
     258,    44,    44,   -27,   -27,    69,   -16,   -27,    98,   -27,
     106,   -27,   200,    90,   -27,   107,   -27,   -27,    40,   -27,
     -27,   -27,   -27,   -27,   -27,   213,    50,    71,   -27,    44,
      44,    44,    44,    44,    92,   -27,   -27,   -27,   -27,   258,
     -27,   -27,    44,    44,   -27,   -27,   -27,   -27,   -27,   -27,
     126,   226,   111,   112,    97,   -27,   258,   258,   -27,   232,
     245,   -27,   -27
};

/* YYDEFACT[STATE-NUM] -- Default reduction number in state STATE-NUM.
   Performed when YYTABLE does not specify something else to do.  Zero
   means the default is an error.  */
static const yytype_int8 yydefact[] =
{
       5,     0,     0,     1,     0,     0,     2,     0,     0,     0,
      24,     0,     0,     0,     0,     0,    46,     0,     0,     0,
       0,     0,     0,     0,     9,     0,     0,     0,     0,    28,
       0,     0,     0,     0,     0,     0,     0,    44,    46,     0,
       0,    45,     0,     0,     0,     0,     0,     7,     8,    17,
       0,     0,    22,    29,    20,     0,     0,     0,     0,    31,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,    18,    19,     0,    32,     6,     0,    26,
       0,     4,     0,     0,    21,     0,    48,    47,     0,    39,
      42,    43,    38,    40,    41,     0,     0,     0,    10,     0,
       0,     0,     0,     0,     0,    27,     3,    25,    30,     0,
      12,    13,     0,     0,    14,    33,    34,    35,    36,    37,
       0,     0,     0,     0,     0,    11,     0,     0,    23,     0,
       0,    15,    16
};

/* YYPGOTO[NTERM-NUM].  */
static const yytype_int8 yypgoto[] =
{
     -27,   -27,   -27,   -27,   -18,   -23,   -27,   -27,   121,   -27,
     -27,   -27,   -17,    53,    13
};

/* YYDEFGOTO[NTERM-NUM].  */
static const yytype_int8 yydefgoto[] =
{
       0,     1,     2,     6,    23,    24,     9,    25,    12,    31,
      60,    75,    39,    40,    26
};

/* YYTABLE[YYPACT[STATE-NUM]] -- What to do in state STATE-NUM.  If
   positive, shift that token.  If negative, reduce the rule whose
   number is the opposite.  If YYTABLE_NINF, syntax error.  */
static const yytype_uint8 yytable[] =
{
      48,    42,    44,    64,    65,    66,    29,    54,    55,    51,
      79,    10,    27,    57,    56,    10,    11,     3,     4,     5,
      32,    48,    99,   100,   101,   102,   103,     7,    48,     8,
      41,    41,    30,    48,    45,    41,    80,    28,    28,    82,
      67,    68,    69,    14,    16,    88,    35,    37,    38,    36,
      61,    62,    95,    13,    17,    97,   109,   110,    18,    48,
      34,    19,    15,    41,    43,    48,    20,    38,    21,    22,
      84,    85,    48,   112,   113,    46,    49,    41,    41,    41,
      41,    41,    41,    50,    41,    41,    52,    53,    58,    59,
      36,   121,    63,    70,    71,    73,    74,    78,    48,    83,
      98,   104,   114,    76,    86,    87,    48,    48,   129,   130,
     105,   108,    41,    41,    41,    41,    41,    89,    90,    91,
      92,    93,    94,    16,    96,    41,    41,   107,   120,   124,
     126,   127,    47,    17,   128,    33,     0,    18,     0,     0,
      19,     0,     0,     0,     0,    20,     0,    21,    22,     0,
      16,     0,   115,   116,   117,   118,   119,     0,     0,     0,
      17,     0,     0,     0,    18,   122,   123,    19,    16,     0,
       0,     0,    20,    72,    21,    22,     0,    77,    17,     0,
       0,     0,    18,     0,     0,    19,    16,     0,     0,     0,
      20,     0,    21,    22,     0,    81,    17,     0,     0,     0,
      18,     0,     0,    19,    16,     0,     0,     0,    20,     0,
      21,    22,     0,   106,    17,     0,     0,    16,    18,     0,
       0,    19,     0,     0,     0,     0,    20,    17,    21,    22,
      16,    18,     0,   111,    19,     0,    16,     0,     0,    20,
      17,    21,    22,   125,    18,     0,    17,    19,     0,    16,
      18,     0,    20,    19,    21,    22,     0,   131,    20,    17,
      21,    22,    16,    18,     0,     0,    19,     0,     0,     0,
     132,    20,    17,    21,    22,     0,    18,     0,     0,    19,
       0,     0,     0,     0,    20,     0,    21,    22
};

static const yytype_int16 yycheck[] =
{
      23,    18,    20,     6,     7,     8,     4,    33,    34,    27,
       4,     4,     9,     9,    32,     4,     9,     0,    10,    11,
       9,    44,    38,    39,    40,    41,    42,    12,    51,     4,
      17,    18,    30,    56,    21,    22,    30,    34,    34,    57,
      43,    44,    45,    12,     4,    63,    32,     3,     4,    35,
       3,     4,    70,    32,    14,    72,    16,    17,    18,    82,
       3,    21,    35,    50,     4,    88,    26,     4,    28,    29,
      33,    34,    95,    23,    24,    22,    31,    64,    65,    66,
      67,    68,    69,     5,    71,    72,     4,     4,    36,     4,
      35,   109,    15,    19,    22,    31,    31,    35,   121,     3,
      31,     3,    31,    50,    37,    37,   129,   130,   126,   127,
       4,     4,    99,   100,   101,   102,   103,    64,    65,    66,
      67,    68,    69,     4,    71,   112,   113,    37,    36,     3,
      19,    19,    13,    14,    37,    14,    -1,    18,    -1,    -1,
      21,    -1,    -1,    -1,    -1,    26,    -1,    28,    29,    -1,
       4,    -1,    99,   100,   101,   102,   103,    -1,    -1,    -1,
      14,    -1,    -1,    -1,    18,   112,   113,    21,     4,    -1,
      -1,    -1,    26,    27,    28,    29,    -1,    13,    14,    -1,
      -1,    -1,    18,    -1,    -1,    21,     4,    -1,    -1,    -1,
      26,    -1,    28,    29,    -1,    13,    14,    -1,    -1,    -1,
      18,    -1,    -1,    21,     4,    -1,    -1,    -1,    26,    -1,
      28,    29,    -1,    13,    14,    -1,    -1,     4,    18,    -1,
      -1,    21,    -1,    -1,    -1,    -1,    26,    14,    28,    29,
       4,    18,    -1,    20,    21,    -1,     4,    -1,    -1,    26,
      14,    28,    29,    17,    18,    -1,    14,    21,    -1,     4,
      18,    -1,    26,    21,    28,    29,    -1,    25,    26,    14,
      28,    29,     4,    18,    -1,    -1,    21,    -1,    -1,    -1,
      25,    26,    14,    28,    29,    -1,    18,    -1,    -1,    21,
      -1,    -1,    -1,    -1,    26,    -1,    28,    29
};

/* YYSTOS[STATE-NUM] -- The symbol kind of the accessing symbol of
   state STATE-NUM.  */
static const yytype_int8 yystos[] =
{
       0,    47,    48,     0,    10,    11,    49,    12,     4,    52,
       4,     9,    54,    32,    12,    35,     4,    14,    18,    21,
      26,    28,    29,    50,    51,    53,    60,     9,    34,     4,
      30,    55,     9,    54,     3,    32,    35,     3,     4,    58,
      59,    60,    58,     4,    50,    60,    59,    13,    51,    31,
       5,    50,     4,     4,    33,    34,    50,     9,    36,     4,
      56,     3,     4,    15,     6,     7,     8,    43,    44,    45,
      19,    22,    27,    31,    31,    57,    59,    13,    35,     4,
      30,    13,    50,     3,    33,    34,    37,    37,    50,    59,
      59,    59,    59,    59,    59,    50,    59,    58,    31,    38,
      39,    40,    41,    42,     3,     4,    13,    37,     4,    16,
      17,    20,    23,    24,    31,    59,    59,    59,    59,    59,
      36,    50,    59,    59,     3,    17,    19,    19,    37,    50,
      50,    25,    25
};

/* YYR1[RULE-NUM] -- Symbol kind of the left-hand side of rule RULE-NUM.  */
static const yytype_int8 yyr1[] =
{
       0,    46,    47,    48,    48,    48,    49,    49,    50,    50,
      51,    51,    51,    51,    51,    51,    51,    51,    51,    51,
      52,    53,    54,    54,    54,    54,    55,    55,    55,    55,
      56,    56,    57,    57,    57,    57,    57,    57,    58,    58,
      58,    58,    58,    58,    59,    59,    60,    60,    60
};

/* YYR2[RULE-NUM] -- Number of symbols on the right-hand side of rule RULE-NUM.  */
static const yytype_int8 yyr2[] =
{
       0,     2,     2,     8,     7,     0,     6,     5,     2,     1,
       4,     7,     5,     5,     5,     9,     9,     2,     3,     3,
       4,     4,     3,     8,     1,     6,     3,     4,     1,     2,
       3,     1,     1,     3,     3,     3,     3,     3,     3,     3,
       3,     3,     3,     3,     1,     1,     1,     4,     4
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
  case 3: /* procedures: procedures PROCEDURE proc_head IS declarations BEGIN_KW commands END  */
#line 139 "/mnt/c/Users/adame/CLionProjects/jftt_compiler/compiler/parser.y"
                                                                         { cout << "PROCEDURE" << endl;}
#line 1321 "../compiler/parser.cpp"
    break;

  case 10: /* command: identifier ASSIGNMENT expression ';'  */
#line 155 "/mnt/c/Users/adame/CLionProjects/jftt_compiler/compiler/parser.y"
                                         {
            // $1->register_no -> exact location of where to put the value
            // $3->register_no -> location of the value
            //int pid_reg = $1->register_no;
            //int exp_reg = $3->register_no;
            output_file << "LOAD "<< (yyvsp[-1].attr)->register_no << endl;
            output_file << "STORE "<< (yyvsp[-3].attr)->register_no << endl;
            free((yyvsp[-3].attr));
            free((yyvsp[-1].attr));
        }
#line 1336 "../compiler/parser.cpp"
    break;

  case 18: /* command: READ identifier ';'  */
#line 172 "/mnt/c/Users/adame/CLionProjects/jftt_compiler/compiler/parser.y"
                          {
            output_file << "READ "<< (yyvsp[-1].attr)->register_no << endl;
            free((yyvsp[-1].attr));
        }
#line 1345 "../compiler/parser.cpp"
    break;

  case 19: /* command: WRITE value ';'  */
#line 176 "/mnt/c/Users/adame/CLionProjects/jftt_compiler/compiler/parser.y"
                      {
            output_file << "WRITE "<< (yyvsp[-1].attr)->register_no << endl;
            free((yyvsp[-1].attr));
        }
#line 1354 "../compiler/parser.cpp"
    break;

  case 22: /* declarations: declarations ',' pidentifier  */
#line 191 "/mnt/c/Users/adame/CLionProjects/jftt_compiler/compiler/parser.y"
                                 {
            regs.add((yyvsp[0].attr)->str_value);
            free((yyvsp[0].attr));
        }
#line 1363 "../compiler/parser.cpp"
    break;

  case 23: /* declarations: declarations ',' pidentifier '[' NUMBER ':' NUMBER ']'  */
#line 195 "/mnt/c/Users/adame/CLionProjects/jftt_compiler/compiler/parser.y"
                                                             {
            throw std::runtime_error("Not yet implemented");
        }
#line 1371 "../compiler/parser.cpp"
    break;

  case 24: /* declarations: pidentifier  */
#line 198 "/mnt/c/Users/adame/CLionProjects/jftt_compiler/compiler/parser.y"
                  { regs.add((yyvsp[0].attr)->str_value); free((yyvsp[0].attr)); }
#line 1377 "../compiler/parser.cpp"
    break;

  case 25: /* declarations: pidentifier '[' NUMBER ':' NUMBER ']'  */
#line 199 "/mnt/c/Users/adame/CLionProjects/jftt_compiler/compiler/parser.y"
                                            {
            throw std::runtime_error("Not yet implemented");
        }
#line 1385 "../compiler/parser.cpp"
    break;

  case 32: /* expression: value  */
#line 216 "/mnt/c/Users/adame/CLionProjects/jftt_compiler/compiler/parser.y"
          {
        switch((yyvsp[0].attr)->type) {
            case(LONG):
                break;
            case(STRING):
                break;
        }
        (yyval.attr) = (yyvsp[0].attr);
        cout << "$$: " << (yyval.attr) << endl;
     }
#line 1400 "../compiler/parser.cpp"
    break;

  case 44: /* value: NUMBER  */
#line 243 "/mnt/c/Users/adame/CLionProjects/jftt_compiler/compiler/parser.y"
           {
            (yyval.attr) = (yyvsp[0].attr);
            (yyval.attr)->long_value = (yyvsp[0].attr)->long_value;
            (yyval.attr)->type = LONG;
        }
#line 1410 "../compiler/parser.cpp"
    break;

  case 45: /* value: identifier  */
#line 248 "/mnt/c/Users/adame/CLionProjects/jftt_compiler/compiler/parser.y"
                 {
            (yyval.attr) = (yyvsp[0].attr);
            (yyval.attr)->str_value = (yyvsp[0].attr)->str_value;
            (yyval.attr)->type = STRING;
            (yyval.attr)->register_no = (yyvsp[0].attr)->register_no;
        }
#line 1421 "../compiler/parser.cpp"
    break;

  case 46: /* identifier: pidentifier  */
#line 257 "/mnt/c/Users/adame/CLionProjects/jftt_compiler/compiler/parser.y"
                {
            (yyval.attr) = (yyvsp[0].attr);
            (yyval.attr)->str_value = (yyvsp[0].attr)->str_value;
            (yyval.attr)->lineno = yylineno;
            (yyval.attr)->register_no = regs.at((yyvsp[0].attr)->str_value);
            cout << "pid: " << (yyval.attr)->str_value << " with register_no " << (yyval.attr)->register_no << endl;
        }
#line 1433 "../compiler/parser.cpp"
    break;

  case 47: /* identifier: pidentifier '[' pidentifier ']'  */
#line 264 "/mnt/c/Users/adame/CLionProjects/jftt_compiler/compiler/parser.y"
                                      {
            int tmp_reg = regs.add("tmp");

            output_file << "LOAD "<< regs.at((yyvsp[-1].attr)->str_value) << endl;
            output_file << "STORE "<< tmp_reg << endl;
            output_file << "LOAD "<< regs.at((yyvsp[-3].attr)->str_value) << endl;
            output_file << "ADD "<< tmp_reg << endl;
            output_file << "STORE "<< tmp_reg << endl;


            //const string& pid = $1->str_value;
            //const string& pid = $1->long_value;
            (yyval.attr)->str_value = "tmp";
            (yyval.attr)->register_no = tmp_reg;
            (yyval.attr)->lineno = yylineno;
            free((yyvsp[-3].attr));
            free((yyvsp[-1].attr));
        }
#line 1456 "../compiler/parser.cpp"
    break;


#line 1460 "../compiler/parser.cpp"

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

#line 285 "/mnt/c/Users/adame/CLionProjects/jftt_compiler/compiler/parser.y"



void yyerror(const char *s) {
    cerr << "\nError: " << s << endl;
    if (yytext && yytext[0] != '\n' && yytext[0] != '\r') {

    }
    fprintf(stdout, "Unexpected token: '%s'\n", yytext ? yytext : "UNKNOWN");

    cerr << "Line number: " << yylineno << endl;

//    yyparse();
}

int main(int argc, char* argv[]) {

    // correct number of arguments is provided
    if (argc != 3) {
        cerr << "Usage: " << argv[0] << " <input_file> <output_file>" << endl;
        return 1;
    }

    // Open the output file
    output_file.open(argv[2]);
    if (!output_file) {
        cerr << "Error: Could not open output file " << argv[2] << endl;
        return 1;
    }

    // Open the input file
    yyin = fopen(argv[1], "r");
    if (!yyin) {
        cerr << "Error: Could not open input file " << argv[1] << endl;
        return 1;
    }

    yylineno = 1;
    int parse_result = yyparse();

    fclose(yyin);
    output_file.close();

    if (parse_result == 0) {
        printf("Parsing completed successfully\n");
        return 0;
    } else {
        fprintf(stderr, "Parsing failed\n");
        return 1;
    }

    cout << "Generated output into: " << argv[2] << endl;
    return 0;
}
