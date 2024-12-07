%{
#include <cstring>
#include <cstdio>
#include <iostream>
#include <fstream>
#include <string>
#include <map>

extern int yylineno;
extern FILE* yyin;
extern char* yytext;

void yyerror(const char *);
extern int my_yylex();
#define yylex my_yylex

using namespace std;


class register_table {
    int available_register(void) {
        if (free_register != 0) {
            free_register = 0;
            return free_register;
        }

        int index = 1;
        while(registers.contains(index))
            index++;
        }
        return index;
    }

    void delete(string pid) {
        free_register = reg;
    }

    void add(string pid) {
        table[pid] = available_register(void)
    }

    int free_register = 1;
    unordered_set<int> registers;
    unordered_map<string, int> table;
}

%}

%code requires {
    #include <string> //TODO consider carefully legitimacy of including something here

    #ifndef TOKENATTRIBUTE_DEFINED
    #define TOKENATTRIBUTE_DEFINED
    typedef struct TokenAttribute {
        int type;
        std::string str_value;
        int int_value;
        long long long_value;
        int line_num;
    } TokenAttribute;
    enum attributetype {
        INTEGER=0,
        STRING=1,
        LONG=2
    };
    #endif
}

%union {
    TokenAttribute* attr;
}


%token <attr> NUMBER pidentifier
%token ASSIGNMENT NEQ GEQ LEQ
%token BEGIN_KW
%token PROGRAM PROCEDURE IS END
%token IF THEN ELSE ENDIF WHILE DO ENDWHILE FOR FROM TO DOWNTO ENDFOR REPEAT UNTIL
%token READ WRITE T

%%

program_all:
    procedures main
    ;

procedures:
    procedures PROCEDURE proc_head IS declarations BEGIN_KW commands END { cout << "PROCEDURE" << endl;}
    | procedures PROCEDURE proc_head IS BEGIN_KW commands END
    | %empty
    ;

main:
    PROGRAM IS declarations BEGIN_KW commands END
    | PROGRAM IS BEGIN_KW commands END
    ;

commands:
    commands command
    | command
    ;

command:
    identifier ASSIGNMENT expression ';' {
            ids[$1] = 0;
            cout << ""
        }
    | IF condition THEN commands ELSE commands ENDIF
    | IF condition THEN commands ENDIF
    | WHILE condition DO commands ENDWHILE
    | REPEAT commands UNTIL condition ';'
    | FOR pidentifier FROM value TO value DO commands ENDFOR
    | FOR pidentifier FROM value DOWNTO value DO commands ENDFOR
    | proc_call ';'
    | READ identifier ';'
    | WRITE value ';'
    ;

proc_head:
    pidentifier '(' args_decl ')'
    ;

proc_call:
    pidentifier '(' args ')'
    ;

declarations:
    declarations ',' pidentifier
    | declarations ',' pidentifier '[' NUMBER ':' NUMBER ']'
    | pidentifier
    | pidentifier '[' NUMBER ':' NUMBER ']'

args_decl:
    args_decl ',' pidentifier
    | args_decl ',' T pidentifier
    | pidentifier
    | T pidentifier
    ;

args:
    args ',' pidentifier
    | pidentifier
    ;

expression:
    value
    | value '+' value
    | value '-' value
    | value '*' value
    | value '/' value
    | value '%' value
    ;

condition:
    value '=' value
    | value NEQ value
    | value '>' value
    | value '<' value
    | value GEQ value
    | value LEQ value
    ;

value:
    NUMBER
    | identifier
    ;

identifier:
    pidentifier
    | pidentifier '[' pidentifier ']'
    | pidentifier '[' NUMBER ']'
    ;

%%


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
    ofstream output_file(argv[2]);
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