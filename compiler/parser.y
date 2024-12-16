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
void yyerror(const std::string&, int, const std::string&);
extern int my_yylex();
#define yylex my_yylex

using namespace std;

register_table regs;
set<long> cached_constants = {};

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
        ADDRESS=3
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
            for (auto const constant : cached_constants) { // TODO postprocessing
                output_file << "SET " << constant << endl;
                output_file << "STORE [" << constant << "]" << endl;
            }
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
        }
    ;

command:
    identifier ASSIGNMENT expression ';' {
            /*if ($3->type == STRING) {
                output_file << "LOAD "<< $3->register_no << "\t#" << $3->str_value << endl;
            } else {
                output_file << "SET "<< $3->long_value << endl;
            }*/ // <- moved down the parse tree TODO : DELETE THIS

            $$ = $3; // put expression->translation into $$->translation

            if ($1->type == ADDRESS) {
                if ($1->translation.empty()) { // pid[num] todo DELETE THIS CASE AS TRANSLATION CANNOT BE EMPTY ANYMORE!
                    // expression puts its value into r0!
                    $$->translation.emplace_back("STOREI " + to_string($1->register_no) + "\t#" + $1->str_value);
                } else { // pid[pid]
                    const auto tmp_reg = regs.add_rval(); // store address here
                    $1->translation.emplace_back("STORE " + to_string(tmp_reg) + "\t#tmp pid address");
                    $$->translation.splice($$->translation.begin(), $1->translation);
                    // expression puts its value into r0
                    $$->translation.emplace_back("STOREI " + to_string(tmp_reg) + "\t#" + $1->str_value + "[..]");
                }
            } else {
                // expression value already in r0!
                $$->translation.emplace_back("STORE " + to_string($1->register_no) + "\t#" + $1->str_value);
            }
            free($1);
        }
    | IF condition THEN commands ELSE commands ENDIF {
            $$ = $2;
            if ($2->type != LONG) { // condition is an lval
                $2->translation.front().append("\t# if-else head");
                $$->translation.back().append(" " + to_string($4->translation.size() + 2));  // enter adequate block (last jump opcode lacks an argument by default)

                $4->translation.front().append("\t# if (cond ture) block");
                $$->translation.splice($$->translation.end(), $4->translation); // paste if block
                $$->translation.emplace_back("JUMP " + to_string($6->translation.size() + 1)); // omit else block

                $6->translation.front().append("\t# else (cond false) block");
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
            if ($2->type != LONG) { // it's an lval
                $$->translation.front().append("\t# if-cond head");
                $$->translation.back().append(" " + to_string($4->translation.size() + 1));
                $4->translation.front().append("\t# if block");
                $$->translation.splice($$->translation.end(), $4->translation);
            } else {
                if($2->long_value != 0) { //cond always true
                    $$->translation = $4->translation;
                } else { //cond false
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
                // TODO replace by some invert_condition($4) function call
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
            $$ = $2;

            const auto tid_register = regs.at($2->str_value);

            // examine illegal alteration of temporary-identifier inside the loop
            auto line_count = 0;
            for (const auto& line : $8->translation) {
                line_count++;
                if ((line.compare(0, 6 + $2->str_value.length(), "STORE " + to_string(tid_register)) == 0) ||
                (line.compare(0, 4 + $2->str_value.length(), "GET " + to_string(tid_register)) == 0)) {
                    yyerror("for-loop iterator modification inside the loop is forbidden!", $2->lineno + line_count, $2->str_value);
                }
            }
            // check for invalid range
            if ($4->type == LONG && $6->type == LONG &&
                $4->long_value > $6->long_value) {
                const auto error_msg = "FROM " + to_string($4->long_value) + " TO " + to_string($6->long_value);
                yyerror("invalid range in for loop", $2->lineno, error_msg);
            }

            // create for_head
            string CONDITIONAL_JUMP; // remember if we did v1 - v2 or v2 - v1 to decide when to stop the loop in the footer
            list<string> for_head;
            list<string> for_footer;
            if ($4->type == LONG && $6->type == LONG) { // SPECIAL CASE [rVAL, rVAL] can be optimized like:
                for_head = {
                    "SET " + to_string($4->long_value - $6->long_value), // store only one value v == v1-v2 <= 0
                    "STORE " + to_string(tid_register),
                };
                CONDITIONAL_JUMP = "JPOS";
            } else if ($4->type == LONG && $6->type == STRING) {
                for_head = {
                    "SET "  + to_string($4->long_value),
                    "SUB "  + to_string($6->register_no),
                    "STORE "+ to_string(tid_register),
                };
                CONDITIONAL_JUMP = "JPOS";
            } else if ($4->type == STRING && $6->type == LONG) {
                for_head = {
                    "SET "  + to_string($6->long_value),
                    "SUB "  + to_string($4->register_no),
                    "STORE "+ to_string(tid_register),
                };
                CONDITIONAL_JUMP = "JNEG";
            } else if ($4->type == STRING && $6->type == STRING) {
                for_head = {
                    "LOAD " + to_string($6->register_no),
                    "SUB "  + to_string($4->register_no),
                    "STORE "+ to_string(tid_register),
                };
                CONDITIONAL_JUMP = "JNEG";
            } else if ($4->type == ADDRESS && $6->type == ADDRESS) {
                const auto tmp_reg = regs.add_rval();
                for_head = $4->translation; // load address1 to r0
                for_head.emplace_back("LOADI 0");
                for_head.emplace_back("STORE " + to_string(tmp_reg));
                for_head.splice(for_head.end(), $6->translation);
                for_head.emplace_back("LOADI 0");
                for_head.emplace_back("SUB "   + to_string(tmp_reg));
                CONDITIONAL_JUMP = "JPOS";
            } else if ($4->type == ADDRESS && $6->type == LONG) {
                // TODO important not to forget that simple SET cache optimization fails for this case
                const auto tmp_reg = regs.add_rval();
                for_head = $4->translation;
                for_head.emplace_back("LOADI 0");
                for_head.emplace_back("STORE " + to_string(tmp_reg));
                for_head.emplace_back("SET "   + to_string($6->long_value));
                for_head.emplace_back("SUB "   + to_string(tmp_reg));
                CONDITIONAL_JUMP = "JNEG";
            } else if ($4->type == LONG && $6->type == ADDRESS) {
                const auto tmp_reg = regs.add_rval();
                for_head = $6->translation;
                for_head.emplace_back("LOADI 0");
                for_head.emplace_back("STORE " + to_string(tmp_reg));
                for_head.emplace_back("SET "   + to_string($4->long_value));
                for_head.emplace_back("SUB "   + to_string(tmp_reg));
                CONDITIONAL_JUMP = "JPOS";
            } else if ($4->type == ADDRESS && $6->type == STRING) {
                for_head = {
                    "LOADI 0",
                    "SUB " + to_string($6->register_no),
                };
                for_head.splice(for_head.begin(), $4->translation);
                CONDITIONAL_JUMP = "JPOS";
            } else if ($4->type == STRING && $6->type == ADDRESS) {
                for_head = {
                    "LOADI 0",
                    "SUB "  + to_string($4->register_no),
                };
                for_head.splice(for_head.begin(), $6->translation);
                CONDITIONAL_JUMP = "JNEG";
            }
            // conditional jump over the command block will soon be added down below!

            for_footer = {
                "LOAD " + to_string(tid_register),
                CONDITIONAL_JUMP + " 5", // exit for
                "ADD [one]",
                "STORE " + to_string(tid_register)
            };
            cached_constants.insert(1);// we need to cache one

            for_footer.emplace_back("JUMP -" + to_string(for_footer.size() + $8->translation.size()));
            // jump to the first line of commands inside the loop ($8->translation)

            //comments
            for_head.front().append("\t# for loop head");
            for_footer.front().append("\t# for loop cond-footer");
            $8->translation.front().append("\t# for loop commands block");

            //translation scheme
            $$->translation = for_head;
            if ($4->type != LONG || $6->type != LONG) {
                $$->translation.emplace_back(CONDITIONAL_JUMP + " " + to_string($8->translation.size() + for_footer.size() + 1)); // jump out of the loop
            }
            $$->translation.splice($$->translation.end(), $8->translation);
            $$->translation.splice($$->translation.end(), for_footer); // most important part of the loop

            //cleanup
            regs.remove($2->str_value); // regs.remove(tid_register);
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
            const auto& pid = $3->str_value;
            if (regs.contains(pid)) {
                yyerror("identifier redeclaration", yylineno-1, pid);
            }
            regs.add(pid);
            free($3);
        }
    | declarations ',' pidentifier '[' NUMBER ':' NUMBER ']' {
            const auto& pid = $3->str_value;
            if (regs.contains(pid)) {
                yyerror("identifier redeclaration", yylineno-1, pid);
            }
            try {
                regs.add_table(pid, $5->long_value, $7->long_value);
            } catch (std::runtime_error e) {
                yyerror(e.what());
            }
            free($3);
            free($5);
            free($7);
        }
    | pidentifier {
            const auto& pid = $1->str_value;
            if (regs.contains(pid)) {
                yyerror("identifier redeclaration", yylineno-1, pid);
            }
            regs.add(pid);
            free($1);
        }
    | pidentifier '[' NUMBER ':' NUMBER ']' {
            const auto& pid = $3->str_value;
            if (regs.contains(pid)) {
                yyerror("identifier redeclaration", yylineno-1, pid);
            }
            try {
                regs.add_table($1->str_value, $3->long_value, $5->long_value);
            } catch (std::runtime_error e) {
                yyerror(e.what());
            }
            free($1);
            free($3);
            free($5);
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
            if ($1->type == STRING) {
                $$->translation = {"LOAD " + to_string($1->register_no)};
            } else {
                $$->translation = {"SET " + to_string($1->long_value)};
            }
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
                regs.add_rval(),
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
                regs.add_rval(),
                yylineno
            );
            $$->str_value = "JZERO 2\nJUMP";
        }
    | value '>' value   { //EXCESS TOKEN ALREADY CLEANED UP!
            $$ = parse_condition(
                $1, $3,
                list<string>({"JPOS 2", "JUMP"}), list<string>({"JNEG 2", "JUMP"}),
                ($1->long_value > $3->long_value),
                regs.add_rval(),
                yylineno
            );
            $$->str_value = "JPOS";
        }
    | value '<' value   {
            $$ = parse_condition(
                $1, $3,
                list<string>({"JNEG 2", "JUMP"}), list<string>({"JPOS 2", "JUMP"}),
                ($1->long_value < $3->long_value),
                regs.add_rval(),
                yylineno
            );
            $$->str_value = "JNEG";
        }
    | value GEQ value   {
            $$ = parse_condition(
                $1, $3,
                list<string>({"JNEG"}), list<string>({"JPOS"}),
                ($1->long_value >= $3->long_value),
                regs.add_rval(),
                yylineno
            );
            $$->str_value = "JNEG 2\nJUMP";
        }
    | value LEQ value   {
            $$ = parse_condition(
                $1, $3,
                list<string>({"JPOS"}), list<string>({"JNEG"}),
                ($1->long_value > $3->long_value),
                regs.add_rval(),
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
            //$$->str_value = $1->str_value;
            //$$->register_no = $1->register_no;
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
            //translation stays empty as we know the pid register location
            $$ = $1;
            //$$->str_value = $1->str_value;
            $$->type = STRING;
            $$->lineno = yylineno;
            if (!regs.contains($1->str_value)) {
                yyerror("undefined identifier", yylineno, $$->str_value);
            }
            $$->register_no = regs.at($1->str_value);
            //cout << "pid: " << $$->str_value << " with register_no " << $$->register_no << endl;
        }
    | pidentifier '[' pidentifier ']' {
            $$ = $1;
            const auto tab_offset = regs.at($1->str_value) - regs.get_pid($1->str_value).index_shift;
            cached_constants.insert(tab_offset); // cache offset
            $$->translation.emplace_back("LOAD [" + to_string(tab_offset) + "]"); // we know exact location where the table begin!
            $$->translation.emplace_back("ADD " + to_string(regs.at($3->str_value)));
            $$->type = ADDRESS;
            $$->lineno = yylineno;
            free($3);
        }
    | pidentifier '[' NUMBER ']' {
            //translation stays empty as we know the pid register location
            $$ = $1;
            try {
                $$->register_no = regs.at($1->str_value, $3->long_value);
            } catch (const std::runtime_error& e) {
                yyerror(e.what(), yylineno, $1->str_value + "[" + to_string($3->long_value) + "]");
            }
            $$->type = STRING; // treat as regular pid as we know exact location of the register
            $$->lineno = yylineno;
            free($3);
        }
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

