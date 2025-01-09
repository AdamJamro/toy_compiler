%{
#include <ctype.h>
#include "parser.h"
#include "parser_utils.h"
#include <iostream>

extern int yylex();
extern int my_yylext();
extern int yylineno;

extern void yyerror(const char*);
extern register_table regs;

%}

number              -?[0-9]+
pidentifier         [_a-z]+
comment_line        (("\\"|"#")([^\n]*))$
line_break          \\[ \t]*\n
whitespace          [ \t]
newline             \n

%%

"BEGIN"         { return BEGIN_KW; }
"PROGRAM"       { return PROGRAM; }
"PROCEDURE"     { return PROCEDURE; }
"IS"            { return IS; }
"END"           { return END; }
"IF"            { return IF; }
"THEN"          { return THEN; }
"ELSE"          { return ELSE; }
"ENDIF"         { return ENDIF; }
"WHILE"         { return WHILE; }
"DO"            { return DO; }
"ENDWHILE"      { return ENDWHILE; }
"FOR"           { return FOR; }
"FROM"          { return FROM; }
"TO"            { return TO; }
"DOWNTO"        { return DOWNTO; }
"ENDFOR"        { return ENDFOR; }
"REPEAT"        { return REPEAT; }
"UNTIL"         { return UNTIL; }
"T"             { return T; }
"WRITE"         { return WRITE; }
"READ"          { return READ; }

"+"			    { return '+'; }

"-"			    { return '-'; }

"*"			    { return '*'; }

"/"			    { return '/'; }

"%"			    { return '%'; }

"["			    { return '['; }

"]"			    { return ']'; }

"("			    { return '('; }

")"			    { return ')'; }

";"             { return ';'; }

":"             { return ':'; }

","             { return ','; }

":="            { return ASSIGNMENT; }

"="             { return '='; }

"<"             { return '<'; }

">"             { return '>'; }

"!="            { return NEQ; }

">="            { return GEQ; }

"<="            { return LEQ; }

{pidentifier}   {
                    //yylval.attr = static_cast<TokenAttribute*>(malloc(sizeof(TokenAttribute)));
                    yylval.attr = new TokenAttribute();
                    yylval.attr->type = STRING;
                    yylval.attr->lineno = yylineno;
                    yylval.attr->str_value = yytext;
                    yylval.attr->translation = {};
                    //yylval.attr->register_no = regs.add(yytext);
                    return pidentifier;
                }

{number}	    {
                    //yylval.attr = static_cast<TokenAttribute*>(malloc(sizeof(TokenAttribute)));
                    yylval.attr = new TokenAttribute();
                    yylval.attr->type = LONG;
                    yylval.attr->long_value = std::atoll(yytext);
                    yylval.attr->lineno = yylineno;
                    yylval.attr->translation = {};
                    //printf("%d ", yylval);
                    return NUMBER;
                }

{line_break}    ;
{comment_line}	;
{whitespace}    ;
\n              { yylineno++; }
\r              { /*"some random doubled space somebody thought up to throw into space inside a text file because why not"*/ }
.			    { std::cout << "unexpected lexem: " << yytext << std::endl; }

%%

int yywrap() {
    return 1; // DO NOT WAIT FOR ANOTHER INPUT FILE
}


int my_yylex() {
    int token = yylex();
    //printf("TokenID: %d;\t", token);
    // //std::cout << "Token: \"" << (yytext ? yytext : "UNKNOWN") << "\"" << std::endl;
    //printf("Token: \"%s\"\n", yytext ? yytext : "UNKNOWN");
    return token;
}