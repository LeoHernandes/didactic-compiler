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

    /* Program generic structure */
program: elements_list | ;
elements_list : elements_list element | element ;
element : variable_declaration ',' | function_declaration ;

    /* Variable declaration */
variable_declaration: type variables_list ;
variables_list: variables_list ';' TK_IDENTIFICADOR | TK_IDENTIFICADOR ;

    /* Function declaration */
function_declaration: function_header function_body ;
function_header: function_parameters TK_OC_OR type '/' TK_IDENTIFICADOR ;
function_parameters: '(' parameters_list ')' | '(' ')' ; 
parameters_list: parameters_list ';' type TK_IDENTIFICADOR | type TK_IDENTIFICADOR ;  
function_body: command_block ;

    /* Commands */
command_block: '{' commands_list '}' | '{' '}' ;
commands_list: commands_list command | command ;
command: 
  variable_declaration ','
| attribution_command ','
| function_call ','
| return_command ','
| conditional_command ','
| while_command ','
;

attribution_command: TK_IDENTIFICADOR '=' expression ;

function_call: TK_IDENTIFICADOR function_parameters  ;
return_command: TK_PR_RETURN expression ;

conditional_command: if_command else_command  | if_command  ;  
if_command: TK_PR_IF '(' expression ')' command_block ;
else_command: TK_PR_ELSE command_block ;

while_command: TK_PR_WHILE '(' expression ')' command_block ;

    /* Expressions */
expression: TK_LIT_TRUE ;

    /* Primitives types */
type: TK_PR_INT | TK_PR_FLOAT | TK_PR_BOOL ;
%%

#include <stdio.h>

void yyerror (char const *mensagem)
{
    fprintf(stderr, "%s\n", mensagem);
}