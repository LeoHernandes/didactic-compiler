/* Desenvolvido pelo Grupo K
    - LEO HERNANDES DE VASCONCELOS - 323961
    - VITOR CARUSO RODRIGUES FERRER - 327023
*/

%{
    #include "parser.tab.h"
    #include "string.h"
    #include "ast.h"
%}

/* Stops processing after EOF */
%option noyywrap

/* Enables line counter */
%option yylineno

/* Multiline comment context variable */
%x comment

digit [0-9]
letter_ [a-z_]
blank [ \t\n\r]
special_characters [-!*/%+<>{}()=,;]
single_line_comment \/\/.*

%%

    /* ================= */
    /* Reserved keywords */
"int"       {return TK_PR_INT;}
"float"     {return TK_PR_FLOAT;}
"bool"      {return TK_PR_BOOL;}
"if"        {return TK_PR_IF;}
"else"      {return TK_PR_ELSE;}
"while"     {return TK_PR_WHILE;}
"return"    {return TK_PR_RETURN;}

    /* ================== */
    /* Compound operators */
"<="    {return TK_OC_LE;}
">="    {return TK_OC_GE;}
"=="    {return TK_OC_EQ;}
"!="    {return TK_OC_NE;}
"&"     {return TK_OC_AND;}
"|"     {return TK_OC_OR;}

    /* ======== */
    /* Literals */
{digit}+                        {yylval.value = lexical_data_new(yylineno, LITERAL, strdup(yytext)); return TK_LIT_INT;} 
{digit}*\.{digit}+              {yylval.value = lexical_data_new(yylineno, LITERAL, strdup(yytext)); return TK_LIT_FLOAT;}
"false"                         {yylval.value = lexical_data_new(yylineno, LITERAL, strdup(yytext)); return TK_LIT_FALSE;}
"true"                          {yylval.value = lexical_data_new(yylineno, LITERAL, strdup(yytext)); return TK_LIT_TRUE;}

    /* =========== */
    /* Identifiers */
{letter_}({letter_}|{digit})*   {yylval.value = lexical_data_new(yylineno,IDENTIFIER, strdup(yytext)); return TK_IDENTIFICADOR;}

    /* ================== */
    /* Special Characters */
{special_characters}            {return yytext[0];}


    /* ============== */
    /* Ignored tokens */
{blank}                 {}
{single_line_comment}   {}

"/*"                    BEGIN(comment); /* Multiline comment */
<comment>[^*\n]*        {}
<comment>[^*\n]*\n      {}
<comment>"*"+[^*/\n]*   {}
<comment>"*"+[^*/\n]*\n {}
<comment>"*"+"/"        BEGIN(INITIAL);

    /* ============= */
    /* Unknown token */
. {return TK_ERRO;}

%%

int get_line_number (void){
    return yylineno;
}