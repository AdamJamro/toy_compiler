%{
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

extern int yylineno;
extern FILE* yyin;
extern char* yytext;
std::ofstream output_file;

void yyerror(const char *);
void yyerror(const char *, int, std::string);
extern int my_yylex();
#define yylex my_yylex

using namespace std;

register_table regs;

%}

%code requires {
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
        INTEGER=0,
        STRING=1,
        LONG=2,
        COMMAND_BLOCK=3
    };
    #endif

}

%union {
    TokenAttribute* attr;
}

%type <attr> identifier tidentifier expression value command commands main condition
%token <attr> NUMBER pidentifier
%token ASSIGNMENT NEQ GEQ LEQ
%token BEGIN_KW
%token PROGRAM PROCEDURE IS END
%token IF THEN ELSE ENDIF WHILE DO ENDWHILE FOR FROM TO DOWNTO ENDFOR REPEAT UNTIL
%token READ WRITE T

%%

program_all:
    procedures main {
            const auto& main_block = $2->translation;
            // TODO UNCOMMENT LINES BELLOW:
            //const auto& procedures_block = $1->translation;
            //output_file << "JMP " << procedures_block.size(); << endl;
            //for (const auto& line : procedures_block) {
            //    output_file << line << endl;
            //}
            for (const auto& line : main_block) {
                output_file << line << endl;
            }
            output_file << "HALT" << endl;
            free($2);
        }
    ;

procedures:
    procedures PROCEDURE proc_head IS declarations BEGIN_KW commands END { cout << "PROCEDURE" << endl;}
    | procedures PROCEDURE proc_head IS BEGIN_KW commands END
    | %empty
    ;

main:
    PROGRAM IS declarations BEGIN_KW commands END {
            $$ = $5;
        }
    | PROGRAM IS BEGIN_KW commands END {
            $$ = $4;
        }
    ;

commands:
    commands command {
            //($1->translation).splice($1->translation.end(), $2->translation) // TODO why does it crash?
            $1->translation.splice($1->translation.end(), $2->translation);
            $$ = $1;
            free($2);
        }
    | command {
            $$ = $1;
            $$->type = COMMAND_BLOCK;
        }
    ;

command:
    identifier ASSIGNMENT expression ';' {
            /*if ($3->type == STRING) {
                output_file << "LOAD "<< $3->register_no << "\t#" << $3->str_value << endl;
            } else {
                output_file << "SET "<< $3->long_value << endl;
            }*/ // <- moved down the parse tree TODO : DELETE THIS

            // expression already in r0!
            $$ = $1;
            $$->translation.splice($$->translation.end(), $3->translation);
            $$->translation.emplace_back("STORE " + to_string($1->register_no) + "\t#" + $1->str_value);
            free($3);
        }
    | IF condition THEN commands ELSE commands ENDIF {
            $$ = $2;
            if ($2->type == STRING) { // condition is an lval
                $2->translation.front().append("\t# if-else head");
                $$->translation.back().append(" " + to_string($4->translation.size() + 2));  // enter adequate block

                $4->translation.front().append("\t# if block");
                $$->translation.splice($$->translation.end(), $4->translation); // paste if block
                $$->translation.emplace_back("JUMP " + to_string($6->translation.size() + 1)); // omit else block

                $6->translation.front().append("\t# else block");
                $$->translation.splice($$->translation.end(), $6->translation); // paste else block
            } else { // condition is an rval
                if($2->long_value == 0) {
                    $$->translation = $4->translation; // just the if block
                } else {
                    $$->translation = $6->translation; // just else block
                }
            }
            free($4);
            free($6);
        }
    | IF condition THEN commands ENDIF {
            $$ = $2;
            if ($2->type == STRING) { // it's an lval
                $$->translation.front().append("\t# if-cond head");
                $$->translation.back().append(" " + to_string($4->translation.size() + 1));
                $4->translation.front().append("\t# if block");
                $$->translation.splice($$->translation.end(), $4->translation);
            } else {
                if($2->long_value == 0) { //cond always true
                    $$->translation = $4->translation;
                } else {
                    $$->translation = {};
                }
            }
            free($4);
        }
    | WHILE condition DO commands ENDWHILE {
            $$ = $2;
            if ($2->type == STRING) { //it's lval
                $2->translation.front().append("\t# while-do head");
                $$->translation.back().append(" " + to_string($4->translation.size() + 2));

                $4->translation.front().append("\t# while commands block");
                $$->translation.splice($$->translation.end(), $4->translation);
                $$->translation.emplace_back("JUMP -" + to_string($$->translation.size()));
            } else { // rval
                if ($2->long_value == 0) { // always true
                    cerr << "Warning on line: " << yylineno << " an endless while loop!";
                    $$->translation = $4->translation;
                    $$->translation.emplace_back("JUMP -" + to_string($$->translation.size()));
                } else {
                    $$->translation = {};
                }
            }
            free($4);
    }
    | REPEAT commands UNTIL condition ';' {
            $$ = $2;
            if ($4->type == STRING) { //lval
                $$->translation.front().append("\t# repeat-until commands block");

                // invert condition (used only for shorter repeat-until loops)
                // TODO replace by invert_condition($4);
                if ($4->translation.back() == "JUMP") {
                    $4->translation.pop_back();
                    $4->translation.back() = $4->translation.back().substr(0, $4->translation.back().find(' '));

                } else {
                    $4->translation.back() = $4->translation.back().substr(0, $4->translation.back().find(' '));
                    $4->translation.back().append(" 2");
                    $4->translation.emplace_back("JUMP");
                }

                $4->translation.front().append("\t# repeat-until footer");
                $$->translation.splice($$->translation.end(), $4->translation);
                $$->translation.back().append(" -" + to_string($$->translation.size() - 1));
            } else { // rval
                if ($4->long_value == 0) {
                    cerr << "Warning on line: " << yylineno << " an endless while loop!" << endl;
                    $$->translation.emplace_back("JUMP -" + to_string($2->translation.size()));
                } else {
                    // pass, commands already in $$
                }
            }
            free($4);
        }
    | FOR tidentifier FROM value TO value DO commands ENDFOR {
            // Could be optimized if we knew what value resides in value tokens
            $$ = $2;

            const auto pid_register = regs.at($2->str_value);
            const auto boundary_register = regs.add();

            if ($4->type != STRING && $6->type != STRING) { // [rVAL, rVAL]

            }

            // examine alteration of pid inside the loop
            auto line_count = 0;
            for (const auto& line : $8->translation) {
                line_count++;
                if ((line.compare(0, 6 + $2->str_value.length(), "STORE " + to_string(pid_register)) == 0) ||
                (line.compare(0, 4 + $2->str_value.length(), "GET " + to_string(pid_register)) == 0)) {
                    yyerror("for-loop iterator moddification inside the loop is forbidden!", $2->lineno + line_count, $2->str_value);
                }
            }

            list<string> for_head;
            list<string> condition_translation;

            if ($4->type != STRING && $6->type != STRING) { // SPECIAL CASE [rVAL, rVAL] can be optimized like:
                if ($4->long_value > $6->long_value) {
                    const auto error_msg = "FROM " + to_string($4->long_value) + " TO " + to_string($6->long_value);
                    yyerror("invalid range in for loop", $2->lineno, error_msg);
                }
                for_head = {
                    "SET " + to_string($4->long_value - $6->long_value), // store only one value v == v1-v2 <= 0
                    "STORE " + to_string(pid_register),
                    "JUMP " + to_string($8->translation.size() + 2),
                };
                condition_translation = {
                    "LOAD " + to_string(pid_register),
                    "JPOS 5", // exit for
                    "SET 1", // TODO OPTIMIZE
                    "ADD " + to_string(pid_register),
                    "STORE " + to_string(pid_register)
                };
                condition_translation.emplace_back("JUMP -" + to_string(condition_translation.size() + $8->translation.size()));
            } else { // DEFAULT FOR LOOP
                for_head = {
                    ($6->type == STRING ? "LOAD " + to_string($6->register_no) : "SET " + to_string($6->long_value)),
                    "STORE " + to_string(boundary_register),
                    ($4->type == STRING ? "LOAD " + to_string($4->register_no) : "SET " + to_string($4->long_value)),
                    "STORE " + to_string(pid_register),
                    "JUMP " + to_string($8->translation.size() + 2),
                };
                condition_translation = {
                    "LOAD " + to_string(pid_register),
                    "SUB " + to_string(boundary_register),
                    "JPOS 5", // exit for
                    "SET 1", // TODO OPTIMIZE
                    "ADD " + to_string(pid_register),
                    "STORE " + to_string(pid_register)
                };
                condition_translation.emplace_back("JUMP -" + to_string(condition_translation.size() + $8->translation.size()));
            }

            //comments
            for_head.front().append("\t# for loop head");
            condition_translation.front().append("\t# for loop cond-footer");
            $8->translation.front().append("\t# for loop commands block");

            //translation scheme
            $$->translation = for_head;
            $$->translation.splice($$->translation.end(), $8->translation);
            $$->translation.splice($$->translation.end(), condition_translation); // most heavily repeated loop part

            //cleanup
            regs.remove($2->str_value);
            regs.remove(boundary_register);
            free($4);
            free($6);
            free($8);
        }
    | FOR tidentifier FROM value DOWNTO value DO commands ENDFOR
    | proc_call ';'
    | READ identifier ';' {
            $$ = $2;
            $$->translation.emplace_back("GET " + to_string($2->register_no));
        }
    | WRITE value ';' {
            $$ = $2;
            if ($2->type != STRING) {
                $$->translation.emplace_back("SET " + to_string($2->long_value));
                $2->register_no = 0;
            }
            $$->translation.emplace_back("PUT " + to_string($2->register_no));
        }
    ;

proc_head:
    pidentifier '(' args_decl ')'
    ;

proc_call:
    pidentifier '(' args ')'
    ;

declarations:
    declarations ',' pidentifier {
            regs.add($3->str_value);
            free($3);
        }
    | declarations ',' pidentifier '[' NUMBER ':' NUMBER ']' {
            throw std::runtime_error("Not yet implemented");
        }
    | pidentifier { regs.add($1->str_value); free($1); }
    | pidentifier '[' NUMBER ':' NUMBER ']' {
            throw std::runtime_error("Not yet implemented");
        }

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
    value {
            $$ = $1;
        }
    | value '+' value {
            //int tmp_reg = regs.add_rval(); TODO DELETE

            if ($1->type == $3->type) {
                $$ = $1;
                if ($1->type == STRING) {
                    // PID + PID
                    $$->translation.emplace_back("LOAD " + to_string($1->register_no));
                    $$->translation.emplace_back("ADD " + to_string($3->register_no));
                } else {
                    // RVAL + RVAL
                    $$->long_value = $1->long_value + $3->long_value;
                    $$->translation.emplace_back("SET " + to_string($$->long_value));
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

                $$->translation.emplace_back("SET " + to_string(num_token->long_value));
                $$->translation.emplace_back("ADD " + to_string(str_token->register_no));
                free(num_token);
            }

            $$->str_value = "rval";
            $$->register_no = 0; // RESULT OF THE EXPRESSION STORED IN R0!
            //EXCESS TOKEN ALREADY CLEANED UP!
        }
    | value '-' value   {throw std::runtime_error("NOT IMPLEMENTED");}
    | value '*' value   {throw std::runtime_error("NOT IMPLEMENTED");}
    | value '/' value   {throw std::runtime_error("NOT IMPLEMENTED");}
    | value '%' value   {throw std::runtime_error("NOT IMPLEMENTED");}
    ;

condition:
    value '=' value { //EXCESS TOKEN ALREADY CLEANED UP!
            $$ = parse_condition(
                $1, $3,
                list<string>({"JZERO 2", "JUMP"}), list<string>({"JZERO 2", "JUMP"}),
                ($1->long_value == $3->long_value),
                yylineno
            );
            // store an inverse jump (a weird somersault for smaller repeat-until loops)
            $$->str_value = "JZERO";
        }
    | value NEQ value   {
            $$ = parse_condition(
                $1, $3,
                list<string>({"JZERO"}), list<string>({"JZERO"}),
                ($1->long_value != $3->long_value),
                yylineno
            );
            $$->str_value = "JZERO 2\nJUMP";
        }
    | value '>' value   { //EXCESS TOKEN ALREADY CLEANED UP!
            $$ = parse_condition(
                $1, $3,
                list<string>({"JPOS 2", "JUMP"}), list<string>({"JNEG 2", "JUMP"}),
                ($1->long_value > $3->long_value),
                yylineno
            );
            $$->str_value = "JPOS";
        }
    | value '<' value   {
            $$ = parse_condition(
                $1, $3,
                list<string>({"JNEG 2", "JUMP"}), list<string>({"JPOS 2", "JUMP"}),
                ($1->long_value < $3->long_value),
                yylineno
            );
            $$->str_value = "JNEG";
        }
    | value GEQ value   {
            $$ = parse_condition(
                $1, $3,
                list<string>({"JNEG"}), list<string>({"JPOS"}),
                ($1->long_value >= $3->long_value),
                yylineno
            );
            $$->str_value = "JNEG 2\nJUMP";
        }
    | value LEQ value   {
            $$ = parse_condition(
                $1, $3,
                list<string>({"JPOS"}), list<string>({"JNEG"}),
                ($1->long_value > $3->long_value),
                yylineno
            );
            $$->str_value = "JPOS 2\nJUMP";
        }
    ;

value:
    NUMBER {
            $$ = $1;
            //$$->str_value = "rval";
            //$$->type = LONG;
        }
    | identifier {
            $$ = $1;
            $$->str_value = $1->str_value;
            $$->type = STRING;
            $$->register_no = $1->register_no;
        }
    ;

tidentifier:
    pidentifier {
            if (regs.contains($1->str_value)) {
                yyerror("ambiguous declaration");
                //throw std::runtime_error("Error: line no: " + to_string(yylineno) + " ambuguous identifier " + $1->str_value);
            }
            $$ = $1;
            $$->str_value = $1->str_value;
            $$->lineno = yylineno;
            $$->register_no = regs.add($1->str_value);
        }
    ;

identifier:
    pidentifier {
            $$ = $1;
            $$->str_value = $1->str_value;
            $$->lineno = yylineno;
            if (!regs.contains($1->str_value)) {
                yyerror("undefined identifier", yylineno, $$->str_value);
            }
            $$->register_no = regs.at($1->str_value);
            //cout << "pid: " << $$->str_value << " with register_no " << $$->register_no << endl;
        }
    | pidentifier '[' pidentifier ']' {
            int tmp_reg = regs.add("tmp");


            throw std::runtime_error("NOT IMPLEMENTED");
            output_file << "LOAD "<< regs.at($3->str_value) << endl;
            output_file << "STORE "<< tmp_reg << endl;
            output_file << "LOAD "<< regs.at($1->str_value) << endl;
            output_file << "ADD "<< tmp_reg << endl;
            output_file << "STORE "<< tmp_reg << endl;


            //const string& pid = $1->str_value;
            //const string& pid = $1->long_value;
            $$->str_value = "tmp";
            $$->register_no = tmp_reg;
            $$->lineno = yylineno;
            free($1);
            free($3);
        }
    | pidentifier '[' NUMBER ']'
    ;

%%


void yyerror(const char *s) {
    cerr << "\nError: " << s << endl;
    if (yytext && yytext[0] != '\n' && yytext[0] != '\r') {

    }
    fprintf(stdout, "Unexpected token: '%s'\n", yytext ? yytext : "UNKNOWN");

    cerr << "Line number: " << yylineno << endl;

    exit(1);

//    yyparse();
}

void yyerror(const char *s, int lineno, string lexem) {
    cerr << "\nError: " << s << endl;
    if (yytext && yytext[0] != '\n' && yytext[0] != '\r') {

    }
    cerr << "Unexpected token: " << lexem << endl;
    cerr << "Line number: " << lineno << endl;

    exit(1);

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
    } else {
        fprintf(stderr, "Parsing failed\n");
        return 1;
    }

    cout << "Generated output into: " << argv[2] << endl;
    return 0;
}

