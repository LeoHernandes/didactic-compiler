/* Desenvolvido pelo Grupo K
    - LEO HERNANDES DE VASCONCELOS - 323961
    - VITOR CARUSO RODRIGUES FERRER - 327023
*/

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
  command_block ','         /* Recursive block */
| variable_declaration ','
| attribution_command ','
| function_call ','
| return_command ','
| conditional_command ','
| while_command ','
;

    /* Commands: attribution */
attribution_command: TK_IDENTIFICADOR '=' expression ;

    /* Commands: function call */
function_call: TK_IDENTIFICADOR function_arguments  ;
function_arguments: '(' arguments_list ')' | '(' ')' ;
arguments_list: arguments_list ';' expression | expression ;

    /* Commands: return */
return_command: TK_PR_RETURN expression ;

    /* Commands: conditional */
conditional_command: if_command else_command  | if_command  ;  
if_command: TK_PR_IF '(' expression ')' command_block ;
else_command: TK_PR_ELSE command_block ;

    /* Commands: while */
while_command: TK_PR_WHILE '(' expression ')' command_block ;

    /* Expressions */
expression: expr_or ;
expr_or: expr_or TK_OC_OR expr_and | expr_and ;          /* 7: OR  */
expr_and: expr_and TK_OC_AND expr_eq_ne | expr_eq_ne ;   /* 6: AND */
expr_eq_ne:
  expr_eq_ne TK_OC_EQ expr_comparisons                   /* 5: EQUAL     */
| expr_eq_ne TK_OC_NE expr_comparisons                   /* 5: NOT EQUAL */
| expr_comparisons
;

expr_comparisons:
  expr_comparisons TK_OC_GE expr_plus_minus    /* 4: GREATER OR EQUAL */
| expr_comparisons TK_OC_LE expr_plus_minus    /* 4: LESS OR EQUAL    */
| expr_comparisons '>' expr_plus_minus         /* 4: GREATER          */   
| expr_comparisons '<' expr_plus_minus         /* 4: LESS             */
| expr_plus_minus
;

expr_plus_minus:                                    
  expr_plus_minus '+' expr_times_div_mod       /* 3  PLUS */      
| expr_plus_minus '-' expr_times_div_mod       /* 3: PLUS */
| expr_times_div_mod
;

expr_times_div_mod:
  expr_times_div_mod '*' expr_unary            /* 2: MULTIPLICATION */
| expr_times_div_mod '/' expr_unary            /* 2: DIVISION */
| expr_times_div_mod '%' expr_unary            /* 2: MODULE */
| expr_unary
;

expr_unary:
  '-' expr_unary                               /* 1: UNARY MINUS */
| '!' expr_unary                               /* 1: NEGATE      */
| expr_parentheses
;

expr_parentheses: '(' expression ')' | operands ; /* 0: PARENTHESES */

operands: 
  TK_IDENTIFICADOR 
| TK_LIT_TRUE 
| TK_LIT_FALSE 
| TK_LIT_INT 
| TK_LIT_FLOAT 
| function_call
;

    /* Primitives types */
type: TK_PR_INT | TK_PR_FLOAT | TK_PR_BOOL ;
%%

#include <stdio.h>

extern int yylineno;
void yyerror (char const *mensagem)
{
    fprintf(stderr, "At line %d: %s\n", yylineno, mensagem);
}