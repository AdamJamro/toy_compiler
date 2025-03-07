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

#ifndef YY_YY_COMPILER_PARSER_H_INCLUDED
# define YY_YY_COMPILER_PARSER_H_INCLUDED
/* Debug traces.  */
#ifndef YYDEBUG
# define YYDEBUG 0
#endif
#if YYDEBUG
extern int yydebug;
#endif
/* "%code requires" blocks.  */
#line 37 "/mnt/c/Users/adame/CLionProjects/jftt_compiler/compiler/parser.y"

    #include <string> //TODO consider carefully legitimacy of including something here
    #include <list>

    #ifndef TOKENATTRIBUTE_DEFINED
    #define TOKENATTRIBUTE_DEFINED
    typedef struct TokenAttribute {
        int type;
        std::string str_value;
        int int_value;
        long long long_value;
        int register_no;
        std::list<std::string> translation;
        int lineno;
    } TokenAttribute;
    enum attributetype {
        INTEGER=0, // TODO: delete int
        STRING=1,
        LONG=2,
        ADDRESS=3
    };
    #endif


#line 74 "../compiler/parser.h"

/* Token kinds.  */
#ifndef YYTOKENTYPE
# define YYTOKENTYPE
  enum yytokentype
  {
    YYEMPTY = -2,
    YYEOF = 0,                     /* "end of file"  */
    YYerror = 256,                 /* error  */
    YYUNDEF = 257,                 /* "invalid token"  */
    NUMBER = 258,                  /* NUMBER  */
    pidentifier = 259,             /* pidentifier  */
    ASSIGNMENT = 260,              /* ASSIGNMENT  */
    NEQ = 261,                     /* NEQ  */
    GEQ = 262,                     /* GEQ  */
    LEQ = 263,                     /* LEQ  */
    BEGIN_KW = 264,                /* BEGIN_KW  */
    PROGRAM = 265,                 /* PROGRAM  */
    PROCEDURE = 266,               /* PROCEDURE  */
    IS = 267,                      /* IS  */
    END = 268,                     /* END  */
    IF = 269,                      /* IF  */
    THEN = 270,                    /* THEN  */
    ELSE = 271,                    /* ELSE  */
    ENDIF = 272,                   /* ENDIF  */
    WHILE = 273,                   /* WHILE  */
    DO = 274,                      /* DO  */
    ENDWHILE = 275,                /* ENDWHILE  */
    FOR = 276,                     /* FOR  */
    FROM = 277,                    /* FROM  */
    TO = 278,                      /* TO  */
    DOWNTO = 279,                  /* DOWNTO  */
    ENDFOR = 280,                  /* ENDFOR  */
    REPEAT = 281,                  /* REPEAT  */
    UNTIL = 282,                   /* UNTIL  */
    READ = 283,                    /* READ  */
    WRITE = 284,                   /* WRITE  */
    T = 285                        /* T  */
  };
  typedef enum yytokentype yytoken_kind_t;
#endif

/* Value type.  */
#if ! defined YYSTYPE && ! defined YYSTYPE_IS_DECLARED
union YYSTYPE
{
#line 62 "/mnt/c/Users/adame/CLionProjects/jftt_compiler/compiler/parser.y"

    TokenAttribute* attr;

#line 125 "../compiler/parser.h"

};
typedef union YYSTYPE YYSTYPE;
# define YYSTYPE_IS_TRIVIAL 1
# define YYSTYPE_IS_DECLARED 1
#endif


extern YYSTYPE yylval;


int yyparse (void);


#endif /* !YY_YY_COMPILER_PARSER_H_INCLUDED  */
