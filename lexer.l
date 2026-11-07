%option noyywrap
%option yylineno

%{
#include <cstdlib>
#include <cstring>
#include "parser.tab.h"
%}

DIGIT       [0-9]
LETTER      [a-zA-Z_]
IDENTIFIER  {LETTER}({LETTER}|{DIGIT})*
NUMBER      {DIGIT}+(\.{DIGIT}+)?

%%

"int"           { return INT; }
"print"         { return PRINT; }

{NUMBER}        {
                    yylval.number = atof(yytext);
                    return NUMBER;
                }

{IDENTIFIER}    {
                    yylval.text = strdup(yytext);
                    return IDENTIFIER;
                }

"="             { return ASSIGN; }
"+"             { return PLUS; }
"-"             { return MINUS; }
"*"             { return MULTIPLY; }
"/"             { return DIVIDE; }
"("             { return LPAREN; }
")"             { return RPAREN; }
";"             { return SEMICOLON; }

"//"[^\n]*      { /* Пропускаємо однорядкові коментарі */ }

[ \t\r\n]+      { /* Пропускаємо пробіли та переноси рядків */ }

.               {
                    fprintf(stderr,
                            "Lexical error at line %d: unexpected character '%s'\n",
                            yylineno, yytext);
                    return yytext[0];
                }

%%