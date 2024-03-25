%{
#include "tokens.h"
%}
%option noyywrap
%option yylineno
%%

"int" {return TK_PR_INT;}
. {return TK_ERRO;}
%%

int get_line_number (void){
    return yylineno;
}