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
        int register_no;
        int lineno;
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

%type <attr> identifier expression value
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
            // $1->register_no -> exact location of where to put the value
            // $3->register_no -> location of the value
            //int pid_reg = $1->register_no;
            //int exp_reg = $3->register_no;
            output_file << "LOAD "<< $3->register_no << endl;
            output_file << "STORE "<< $1->register_no << endl;
            free($1);
            free($3);
        }
    | IF condition THEN commands ELSE commands ENDIF
    | IF condition THEN commands ENDIF
    | WHILE condition DO commands ENDWHILE
    | REPEAT commands UNTIL condition ';'
    | FOR pidentifier FROM value TO value DO commands ENDFOR
    | FOR pidentifier FROM value DOWNTO value DO commands ENDFOR
    | proc_call ';'
    | READ identifier ';' {
            output_file << "READ "<< $2->register_no << endl;
            free($2);
        }
    | WRITE value ';' {
            output_file << "WRITE "<< $2->register_no << endl;
            free($2);
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
            cout << "$$: " << $$ << endl;
        }
    | value '+' value {
            if ($1->type == $3->type) {
                if ($1->type == STRING) {
                    int tmp;
                } else {

                }
            } else {
                if ($1->type == STRING) {

                } else {

                }
            }
        }
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
    NUMBER {
            $$ = $1;
            $$->long_value = $1->long_value;
            $$->type = LONG;
        }
    | identifier {
            $$ = $1;
            $$->str_value = $1->str_value;
            $$->type = STRING;
            $$->register_no = $1->register_no;
        }
    ;

identifier:
    pidentifier {
            $$ = $1;
            $$->str_value = $1->str_value;
            $$->lineno = yylineno;
            $$->register_no = regs.at($1->str_value);
            cout << "pid: " << $$->str_value << " with register_no " << $$->register_no << endl;
        }
    | pidentifier '[' pidentifier ']' {
            int tmp_reg = regs.add("tmp");

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