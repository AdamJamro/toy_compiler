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
        INTEGER=0, // TODO: delete int
        STRING=1,
        LONG=2,
        ADDRESS=3
    };
    #endif

}

%union {
    TokenAttribute* attr;
}

%type <attr> main procedures proc_head proc_call args_decl args declarations identifier tidentifier expression value command commands condition
%token <attr> NUMBER pidentifier
%token ASSIGNMENT NEQ GEQ LEQ
%token BEGIN_KW
%token PROGRAM PROCEDURE IS END
%token IF THEN ELSE ENDIF WHILE DO ENDWHILE FOR FROM TO DOWNTO ENDFOR REPEAT UNTIL
%token READ WRITE T

%%

program_all:
    procedures main {
        auto& main_block = $2->translation;
        auto& procedures_block = $1->translation;
        long line_count = 0;
        long translation_header_offset = 0;

        // TODO uncomment:
        // list<long> cache_regs = regs.unused_registers(cached_constants.size()) // outputs registers never yet returned by any add/assign method
        // cache_register = cache_regs.begin()
        for (auto const constant : cached_constants) { // TODO ENHANCE postprocessing HERE
            // TODO cache uncached [constants]
            // TODO un-cache unused constants
            output_file << "SET " << constant << endl;
            //regs.add(to_string(constant), cache_register); //???
            output_file << "STORE [" << constant << "]" << endl;
            // output_file << "STORE " << regs.at(to_string(constant)) << endl;
            line_count += 2;
            translation_header_offset += 2;
        }


        // translate procedures block
        if (!procedures_block.empty()) {
            long procedures_block_size = 1; // instead of adding extra one to reach over the last line

            for (const auto& line : procedures_block) {
                procedures_block_size += 1 + std::ranges::count(line, '\n');
            }

            output_file << "JMP " << procedures_block_size << endl;
            line_count++;
            //translation_header_offset++;
        }
        for (auto& line : procedures_block) {
            parse_line(line, line_count, translation_header_offset); // replaces this_line with actual line number considering header_offset
            output_file << line << endl;
            line_count++;
            line_count += std::ranges::count(line, '\n');;
        }

        for (auto& line : main_block) {
            parse_line(line, line_count, translation_header_offset); // replaces this_line with actual line number
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
        free($2);
    }
    ;

procedures:
    procedures PROCEDURE proc_head IS declarations BEGIN_KW commands END {
        const auto& fun_name = $3->str_value;
        const auto& arguments = $3->translation;
        const auto& declarations = $5->translation;
        auto& proc_commands = $7->translation;

        if (regs.contains(fun_name)){
            yyerror("procedure's name is ambiguous", yylineno, fun_name);
        }
        const auto return_reg = regs.add(fun_name);

        // store procedure's data
        const auto fun_line_no = $1->translation.size(); // line indexes are shifted down by one
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

        $$ = $1;
        $1->translation.splice($1->translation.end(), proc_commands);
        $$->translation.emplace_back("RTRN " + to_string(return_reg));

        // forget this context after moving to the next procedure
        for (const auto& pid : arguments) {
            regs.forget_pid(pid);
        }
        // TODO forget regular declarations
        for (const auto& pid : declarations) {
            regs.forget_pid(pid);
        }
        free($3);
        free($5);
        free($7);
    }
    | procedures PROCEDURE proc_head IS BEGIN_KW commands END {
        const auto& fun_name = $3->str_value;
        const auto& arguments = $3->translation;

        if (regs.contains(fun_name)){
            yyerror("procedure's name is ambiguous", yylineno, fun_name);
        }
        const auto return_reg = regs.add(fun_name);

        // store procedure's data
        const auto fun_line_no = $1->translation.size() + 1;
        const auto first_arg = regs.at(arguments.front());
        funs.add(fun_name, fun_line_no, arguments.size(), first_arg);

        $$ = $1;
        $1->translation.splice($1->translation.end(), $6->translation);

        $$->translation.emplace_back("RTRN " + to_string(return_reg));
    }
    | %empty {
        $$ = new TokenAttribute();
        $$->translation = {};
    }
    ;

main:
    PROGRAM IS declarations BEGIN_KW commands END {
            $$ = $5;
            free($3);
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

            $$ = $3; // put expression->translation into $$->translation to avoid extra copies

            if ($1->type == ADDRESS) {
                if ($1->translation.empty()) { // pid[num] todo DELETE THIS CASE AS TRANSLATION CANNOT BE EMPTY ANYMORE!
                    // expression puts its value into r0!
                    $$->translation.emplace_back("STOREI " + to_string($1->register_no) + "\t#" + $1->str_value);
                    throw std::runtime_error("address translation is empty");
                } else { // pid[pid]
                    const auto tmp_reg = regs.add_rval(); // store address here
                    $1->translation.emplace_back("STORE " + to_string(tmp_reg) + "\t#tmp pid address");
                    $$->translation.splice($$->translation.begin(), $1->translation);
                    // expression puts its value into r0
                    $$->translation.emplace_back("STOREI " + to_string(tmp_reg) + "\t#" + $1->str_value + "[..]");
                }
                cout << "i'm here with " << $1->str_value << " of type " << $1->type << endl;
            } else {
                cout << "i'm here with " << $1->str_value << " of type " << $1->type << endl;
                // expression value already in r0!
                $$->translation.emplace_back("STORE " + to_string($1->register_no) + "\t#" + $1->str_value + ((regs.get_pid($1->str_value).size > 1) ? ("[" + to_string($1->register_no - regs.at($1->str_value) + regs.get_pid($1->str_value).index_shift) + "]") : ""));
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
            } else { // condition is an rval and is evaluated at compile-time
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
                $$->translation.back().append(" " + to_string($4->translation.size() + 2)); // navigate case where condition is not true

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
            string CONDITIONAL_JUMP; // used to remember if we did v1 - v2 or v2 - v1 to decide when to stop the loop in the footer
            list<string> for_head;
            list<string> for_footer;
            if ($4->type == LONG && $6->type == LONG) { // SPECIAL CASE [rVAL, rVAL] can be optimized like:
                for_head = {
                    "LOAD [" + to_string($4->long_value - $6->long_value) + "]", // store only one value v == v1-v2 <= 0
                    "STORE " + to_string(tid_register),
                };
                CONDITIONAL_JUMP = "JPOS";
                cached_constants.insert($4->long_value - $6->long_value);
            } else if ($4->type == LONG && $6->type == STRING) {
                for_head = {
                    "LOAD [" + to_string($4->long_value) + "]",
                    "SUB "  + to_string($6->register_no),
                    "STORE "+ to_string(tid_register),
                };
                CONDITIONAL_JUMP = "JPOS";
                cached_constants.insert($4->long_value);
            } else if ($4->type == STRING && $6->type == LONG) {
                for_head = {
                    "LOAD [" + to_string($6->long_value) + "]",
                    "SUB "  + to_string($4->register_no),
                    "STORE "+ to_string(tid_register),
                };
                CONDITIONAL_JUMP = "JNEG";
                cached_constants.insert($6->long_value);
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
                for_head.emplace_back("LOAD [" + to_string($6->long_value) + "]");
                for_head.emplace_back("SUB "   + to_string(tmp_reg));
                CONDITIONAL_JUMP = "JNEG";
                cached_constants.insert($6->long_value);
            } else if ($4->type == LONG && $6->type == ADDRESS) {
                const auto tmp_reg = regs.add_rval();
                for_head = $6->translation;
                for_head.emplace_back("LOADI 0");
                for_head.emplace_back("STORE " + to_string(tmp_reg));
                for_head.emplace_back("LOAD [" + to_string($4->long_value) + "]");
                for_head.emplace_back("SUB "   + to_string(tmp_reg));
                CONDITIONAL_JUMP = "JPOS";
                cached_constants.insert($4->long_value);
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
                "ADD [1]",
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
            regs.remove($2->str_value); // SAME AS regs.remove(tid_register);
            free($4);
            free($6);
            free($8);
        }
    | FOR tidentifier FROM value DOWNTO value DO commands ENDFOR
    | proc_call ';' {
            $$ = $1;
    }
    | READ identifier ';' {
            $$ = $2;
            if ($2->type == ADDRESS) {
                const auto tmp_reg = regs.add_rval();
                // store the address to tmp_reg
                $$->translation.emplace_back("STORE " + to_string(tmp_reg));
                // load the value into r0
                $$->translation.emplace_back("GET 0");
                // store it where the tmp address points to
                $$->translation.emplace_back("STOREI " + to_string(tmp_reg));
            } else { // STRING or LONG
                // if it was a long then register_no is expected be 0
                $$->translation.emplace_back("GET " + to_string($2->register_no));
            }

            // Deprecated:
            //$$ = $2;
            //$$->translation.emplace_back("GET " + to_string($2->register_no));
        }
    | WRITE value ';' {
            $$ = $2;
            if ($2->type == LONG) {
                $$->translation.emplace_back("LOAD [" + to_string($2->long_value) + "]");
                $2->register_no = 0;
                cached_constants.insert($2->long_value);
            } else if ($2->type == STRING) {
                // $2 already sets its value to r0
            } else if ($2->type == ADDRESS) {
                $$->translation.emplace_back("LOADI 0");
                $2->register_no = 0;
            } else {
                yyerror("invalid value type in WRITE operation");
            }

            $$->translation.emplace_back("PUT " + to_string($2->register_no));
        }
    ;

proc_head:
    pidentifier '(' args_decl ')' {
        $$ = $3; // get translation from args_decl
        $$->str_value = $1->str_value;
        free($1);
    }
    ;

proc_call:
    pidentifier '(' args ')' {
        const auto& fun_name = $1->str_value;
        if (!regs.contains(fun_name)) {
            yyerror("procedure not declared", yylineno, fun_name);
        }

        $$ = $1;
        $$->translation = {};
        const auto& arguments = $3->translation;

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
                $$->translation.emplace_back("LOAD [" + to_string(regs.at(pid)) + "]" + "\t# passing address");
            } else {
                $$->translation.emplace_back("LOAD [" + to_string(regs.at(pid) - pid_type.index_shift) + "]\t# passing address of an array");
            }


            $$->translation.emplace_back(
                "STORE " + to_string(first_arg_register + arg_count - 1)
            );
            cached_constants.insert(regs.at(pid));
        }
        if (arg_count < arg_no) {
            yyerror("too few arguments in procedure call. expected: " + to_string(arg_no) + ", got: " + to_string(arg_count), yylineno, fun_name);
        }

        // TODO figure out the line_no
        //const auto this_line = 101010101010;
        $$->translation.emplace_back("LOAD [this_line + 3]");
        $$->translation.emplace_back("STORE " + to_string(regs.at(fun_name)));
        // TODO cached_constants.insert(this_line + 3);
        $$->translation.emplace_back("JUMP " + to_string(funs.get_line_no(fun_name)) + " - this_line");
        free($3);
    }
    ;

declarations:
    declarations ',' pidentifier {
            const auto& pid = $3->str_value;
            if (regs.contains(pid)) {
                yyerror("identifier redeclaration", yylineno-1, pid);
            }
            regs.add(pid);
            $$ = $1;
            $$->translation.emplace_back(pid);
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
            $$ = $1;
            $$->translation.emplace_back(pid);
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
            $$ = $1;
            $$->translation = {pid};
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
            $$ = $1;
            $$->translation = {pid};
            free($3);
            free($5);
        }

args_decl:
    args_decl ',' pidentifier {
        const auto& pid = $3->str_value;
        if (regs.contains(pid)) {
          yyerror("identifier redeclaration", yylineno-1, pid);
        }
        regs.add(pid);
        $$ = $1;
        $$->translation.emplace_back(pid);
        free($3);
    }
    | args_decl ',' T pidentifier {
        const auto& pid = $4->str_value;
        if (regs.contains(pid)) {
            yyerror("identifier redeclaration", yylineno-1, pid);
        }
        regs.add_proc_table(pid);
        $$ = $1;
        $$->translation.emplace_back(pid);
        free($4);
    }
    | pidentifier {
        const auto& pid = $1->str_value;
        if (regs.contains(pid)) {
          yyerror("identifier redeclaration", yylineno-1, pid);
        }
        regs.add(pid);
        $$ = $1;
        $$->translation = {pid};
    }
    | T pidentifier {
        const auto& pid = $2->str_value;
        if (regs.contains(pid)) {
            yyerror("identifier redeclaration", yylineno-1, pid);
        }
        regs.add(pid);
        $$ = $2;
        $$->translation = {pid};
    }
    ;

args:
    args ',' pidentifier {
        $$ = $1;
        $$->translation.emplace_back($3->str_value);
        free($3);
    }
    | pidentifier {
        $$ = $1;
        $$->translation = {$1->str_value};
    }
    ;

expression: // simply puts result of the expression into r0
    value {
            $$ = $1;
            if ($1->type == STRING) {
                $$->translation = {"LOAD " + to_string($1->register_no)};
            } else if ($1->type == LONG) {
                cached_constants.insert($1->long_value); // missed optimization if constant is used only once and only in main outside of any loop
                $$->translation = {"LOAD [" + to_string($1->long_value) + "]"};
                cached_constants.insert($1->long_value);
            } else if ($1->type == ADDRESS) {
                $$->translation.push_back("LOADI 0");
                $$->type = STRING; // treat as string TODO: it may turn out useless
            } else {
                yyerror("value has invalid type");
            }
        }
    | value '+' value {
            const auto val = $1->long_value + $3->long_value;

            // TODO refactor (maybe pass a reference to cached_constants to parse_expression):
            if($1->type == LONG || $3->type == LONG) {
                long new_constant;
                if ($1->type == LONG && $3->type == LONG) {
                    new_constant = val;
                } else if ($1->type == LONG) {
                    new_constant = $1->long_value;
                } else if ($3->type == LONG) {
                    new_constant = $3->long_value;
                }
                cached_constants.insert(new_constant); // missed optimization
            }

            $$ = parse_expression($1, $3, "ADD", "ADD", val, regs.add_rval());

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
    | value '-' value {
        const auto val = $1->long_value - $3->long_value;
        $$ = parse_expression($1, $3, "SUB", "RSUB", val, regs.add_rval());
    }
    | value '*' value   {
        const auto val = $1->long_value * $3->long_value;
        $$ = parse_expression($1, $3, "MUL", "MUL", val, regs.add_rval());
    }
    | value '/' value   {
        const auto val = $3->long_value == 0 ? 0 : $1->long_value / $3->long_value;
        $$ = parse_expression($1, $3, "DIV", "RDIV", val, regs.add_rval());
    }
    | value '%' value   {
        const auto val = $3->long_value == 0 ? 0 : $1->long_value % $3->long_value;
        $$ = parse_expression($1, $3, "MOD", "RMOD", val, regs.add_rval());
    }
    ;

condition: // evaluates condition and leaves a blank jump to a "else" clause (specified one step up the parse tree)
    value '=' value { //EXCESS TOKEN ALREADY CLEANED UP BY PARSE FUNCTION!
            $$ = parse_condition(
                $1, $3,
                list<string>({"JZERO 2", "JUMP"}), list<string>({"JZERO 2", "JUMP"}),
                ($1->long_value == $3->long_value),
                regs.add_rval(),
                yylineno,
                cached_constants
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
                yylineno,
                cached_constants
            );
            $$->str_value = "JZERO 2\nJUMP";
        }
    | value '>' value   { //EXCESS TOKEN ALREADY CLEANED UP!
            $$ = parse_condition(
                $1, $3,
                list<string>({"JPOS 2", "JUMP"}), list<string>({"JNEG 2", "JUMP"}),
                ($1->long_value > $3->long_value),
                regs.add_rval(),
                yylineno,
                cached_constants
            );
            $$->str_value = "JPOS";
        }
    | value '<' value   {
            $$ = parse_condition(
                $1, $3,
                list<string>({"JNEG 2", "JUMP"}), list<string>({"JPOS 2", "JUMP"}),
                ($1->long_value < $3->long_value),
                regs.add_rval(),
                yylineno,
                cached_constants
            );
            $$->str_value = "JNEG";
        }
    | value GEQ value   {
            $$ = parse_condition(
                $1, $3,
                list<string>({"JNEG"}), list<string>({"JPOS"}),
                ($1->long_value >= $3->long_value),
                regs.add_rval(),
                yylineno,
                cached_constants
            );
            $$->str_value = "JNEG 2\nJUMP";
        }
    | value LEQ value   {
            $$ = parse_condition(
                $1, $3,
                list<string>({"JPOS"}), list<string>({"JNEG"}),
                ($1->long_value > $3->long_value),
                regs.add_rval(),
                yylineno,
                cached_constants
            );
            $$->str_value = "JPOS 2\nJUMP";
        }
    ;

value:
    NUMBER {
            $$ = $1;
            //$$->str_value = "rval";
            $$->type = LONG;
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
            if (!regs.contains($1->str_value)) {
                yyerror("undefined identifier", yylineno, $1->str_value);
            }

            const auto& table_pid_type = regs.get_pid($1->str_value);

            //CASE: IT IS A TABLE
            if (table_pid_type.size != 0) {
                const auto tab_offset = regs.at($1->str_value) - regs.get_pid($1->str_value).index_shift;
                cached_constants.insert(tab_offset);    // missed optimization
                $$->translation.emplace_back("LOAD [" + to_string(tab_offset) + "]");
                $$->translation.emplace_back("ADD " + to_string(regs.at($3->str_value)));
            } else { //CASE: IT IS A TABLE REFERENCE
                $$->translation.emplace_back("LOAD " + to_string(regs.at($1->str_value)) + "\t# passing address");
                $$->translation.emplace_back("ADD " + to_string(regs.at($3->str_value)));
            }

            $$->type = ADDRESS;
            $$->lineno = yylineno;
            free($3);
        }
    | pidentifier '[' NUMBER ']' {
            $$ = $1;
            if (!regs.contains($1->str_value)) {
                yyerror("undefined identifier", yylineno, $1->str_value);
            }
            const auto value = $3->long_value;
            const auto table_pid_type = regs.get_pid($1->str_value);
            const auto is_reference = table_pid_type.size == 0;

            //CASE: IT IS A TABLE REFERENCE
            if (is_reference) {
                //create a translation that puts address in r0
                $$->translation.emplace_back("LOAD " + to_string(regs.at($1->str_value)) + "\t# passing address");
                $$->translation.emplace_back("ADD [" + to_string(value) + "]");
                $$->type = ADDRESS;
                cached_constants.insert(value);
            } else { //CASE: IT IS A TABLE
                //translation stays empty as we know the pid register location
                try {
                    $$->register_no = regs.at($1->str_value, $3->long_value);
                } catch (const std::runtime_error& e) {
                    yyerror(e.what(), yylineno, $1->str_value + "[" + to_string(value) + "]");
                }
                $$->type = STRING; // treat as regular pid as we know exact location of the register
            }

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

