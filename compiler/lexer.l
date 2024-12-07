%{
#include <ctype.h>
#include "parser.h"

using namespace std;

%}

number              -?[0-9]+
identifier          [_a-z]+
comment_line        (("\\"|"#")([^\n]*))$\n
line_break          \\[ \t]*\n
whitespace          [ \t]

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

"+"			{ return '+'; }

"-"			{ return '-'; }

"*"			{ return '*'; }

"/"			{ return '/'; }

"%"			{ return '%'; }

"("			{ return '('; }

")"			{ return ')'; }

":="        { return ASSIGNMENT; }

"!="        { return NEQ; }

">="        { return GEQ; }

"<="        { return LEQ; }

{identifier}    { return pidentifier; }

{number}	{
				yylval = atoi(yytext);
				//printf("%d ", yylval);
				return NUMBER;
			}


{line_break}    ;
{comment_line}	;
{whitespace}    ;
\n              ;
.			    { return YYUNDEF; }

%%

int yywrap() {
    return 1; // DO NOT WAIT FOR ANOTHER INPUT FILE
}
