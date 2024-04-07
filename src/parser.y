%{
int yylex(void);
void yyerror (char const *mensagem);
%}

%define parse.error verbose
%token TK_PR_INT
%token TK_PR_FLOAT
%token TK_PR_BOOL
%token TK_PR_IF
%token TK_PR_ELSE
%token TK_PR_WHILE
%token TK_PR_RETURN
%token TK_OC_LE
%token TK_OC_GE
%token TK_OC_EQ
%token TK_OC_NE
%token TK_OC_AND
%token TK_OC_OR
%token TK_IDENTIFICADOR
%token TK_LIT_INT
%token TK_LIT_FLOAT
%token TK_LIT_FALSE
%token TK_LIT_TRUE
%token TK_ERRO

%%

program: elements_list | ;
elements_list : elements_list element | element;
element : global_declaration | function_declaration;
global_declaration: type variables_list ',';
type: TK_PR_INT | TK_PR_FLOAT | TK_PR_BOOL;
variables_list: variables_list ';' TK_IDENTIFICADOR | TK_IDENTIFICADOR ;
function_declaration: TK_LIT_FALSE;

%%

#include <stdio.h>

void yyerror (char const *mensagem)
{
    fprintf(stderr, "%s\n", mensagem);
}