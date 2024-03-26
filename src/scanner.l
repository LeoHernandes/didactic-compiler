%{
#include "tokens.h"
%}
%option noyywrap
%option yylineno

%x digit letter_ number

digit [0-9]
letter_ [a-z_]

%%

"int" {return TK_PR_INT;}
"float" {return TK_PR_FLOAT;}
"bool" {return TK_PR_BOOL;}
"if" {return TK_PR_IF;}
"else" {return TK_PR_ELSE;}
"while" {return TK_PR_WHILE;}
"return" {return TK_PR_RETURN;}

"<=" {return TK_OC_LE;}
">=" {return TK_OC_GE;}
"==" {return TK_OC_EQ;}
"!=" {return TK_OC_NE;}
"&" {return TK_OC_AND;}
"|" {return TK_OC_OR;}

{digit}+ {return TK_LIT_INT;} 
{digit}*\.{digit}+ {return TK_LIT_FLOAT;}
"false" {return TK_LIT_FALSE;}
"true" {return TK_LIT_TRUE;}
{letter_}({letter_}|{digit})* {return TK_IDENTIFICADOR;}

. {return TK_ERRO;}

%%

int get_line_number (void){
    return yylineno;
}