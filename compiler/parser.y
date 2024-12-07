
%{
#include <cstring>
#include <cstdio>
#include <iostream>
#include <fstream>
#include <string>

extern int yylineno;
extern FILE* yyin;
extern char* yytext;

void yyerror(const char *);
int yylex(void);

using namespace std;
%}

%token NUMBER pidentifier
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
    procedures PROCEDURE proc_head IS declarations BEGIN_KW commands END
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
    identifier ASSIGNMENT expression ';'
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
    fprintf(stderr, "\nError: %s\n", s);

    if (yytext && yytext[0] != '\n' && yytext[0] != '\r') {
        fprintf(stdout, "Unexpected token: '%s'\n\n", yytext ? yytext : "UNKNOWN");
    }

    if (yylineno > 1) {
        fprintf(stderr, "Line number: %d\n\n", yylineno);
    }

//    yyparse();
}

int main(int argc, char* argv[]) {

    // correct number of arguments is provided
    if (argc != 3) {
        cerr << "Usage: " << argv[0] << " <input_file> <output_file>" << endl;
        return 1;
    }

    // Open the input file
    ifstream inputFile(argv[1]);
    if (!inputFile) {
        cerr << "Error: Could not open input file " << argv[1] << endl;
        return 1;
    }

    // Open the output file
    ofstream outputFile(argv[2]);
    if (!outputFile) {
        cerr << "Error: Could not open output file " << argv[2] << endl;
        return 1;
    }

    // Read from input file and write to output file line by line
    std::string line;
    while (std::getline(inputFile, line)) {
        outputFile << yyparse() << '\n';
    }

    // Close the files
    inputFile.close();
    outputFile.close();

    cout << "Generated output into: " << argv[2] << endl;
    return 0;
}