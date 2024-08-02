/* Desenvolvido pelo Grupo K
    - LEO HERNANDES DE VASCONCELOS - 323961
    - VITOR CARUSO RODRIGUES FERRER - 327023
*/

%{
#include "ast.h"
#include "tableStack.h"
#include "semanticErrors.h"

#define ERR_UNDECLARED 10 // 2.3
#define ERR_DECLARED 11   // 2.3
#define ERR_VARIABLE 20   // 2.4
#define ERR_FUNCTION 21   // 2.4

int yylex(void);
void yyerror (char const *mensagem);
extern ast_t *ast_root;
extern table_stack_t *table_stack;
%}

%define parse.error verbose
%code requires {#include "ast.h"}


%union {
  lexical_data_t *value;
  ast_t *tree_node;
  data_type_t data_type;
}

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

%token<value> TK_IDENTIFICADOR
%token<value> TK_LIT_INT
%token<value> TK_LIT_FLOAT
%token<value> TK_LIT_FALSE
%token<value> TK_LIT_TRUE
%token TK_ERRO

%type<tree_node> program
%type<tree_node> elements_list
%type<tree_node> element
%type<tree_node> function_declaration
%type<tree_node> function_header
%type<tree_node> function_body
%type<tree_node> command_block
%type<tree_node> command_block_without_scope_rules
%type<tree_node> commands_list
%type<tree_node> command
%type<tree_node> attribution_command
%type<tree_node> function_call
%type<tree_node> function_arguments
%type<tree_node> arguments_list
%type<tree_node> return_command
%type<tree_node> conditional_command
%type<tree_node> if_command
%type<tree_node> else_command
%type<tree_node> while_command
%type<tree_node> expression
%type<tree_node> expr_or
%type<tree_node> expr_and
%type<tree_node> expr_eq_ne
%type<tree_node> expr_comparisons
%type<tree_node> expr_plus_minus
%type<tree_node> expr_times_div_mod
%type<tree_node> expr_unary
%type<tree_node> expr_parentheses
%type<tree_node> operands
%type<data_type> type

%%

/* ======================= Program generic structure ======================= */
program: 
  CREATE_SCOPE elements_list REMOVE_SCOPE           {ast_root = $2;}
|                                                   {ast_root = NULL;}
;

elements_list: element elements_list
{
    if ($1 != NULL)
    {
      $$ = $1;
      ast_add_child($$, $2);
    }
    else
    {
      // if this element is a NULL node, consider the next
      // eg. variable declaration element
      $$ = $2; 
    }
}
| element                               {$$ = $1;}
;

element:
  variable_declaration ','              {$$ = NULL;}
| function_declaration                  {$$ = $1;}
;

/* ======================= Variable declaration ======================= */
variable_declaration:
  type variables_list   {symbol_table_fill_unknown_types(table_stack->top->symbol_table, $1);}                
;

variables_list:
  variables_list ';' TK_IDENTIFICADOR 
  {
    symbol_t* symbol = symbol_new(IDENTIFIER_NATURE, UNKNOWN, $3);
    short success = symbol_table_add(table_stack->top->symbol_table, symbol);
    if(!success) return ERR_DECLARED;

  }
| TK_IDENTIFICADOR
  {
    symbol_t* symbol = symbol_new(IDENTIFIER_NATURE, UNKNOWN, $1);
    short success = symbol_table_add(table_stack->top->symbol_table, symbol);
    if(!success) return ERR_DECLARED;
  }
;

/* ======================= Function declaration ======================= */
function_declaration:
  CREATE_SCOPE function_header function_body REMOVE_SCOPE       {$$ = $2; ast_add_child($$, $3);}
;

function_header:
  function_parameters TK_OC_OR type '/' TK_IDENTIFICADOR         
  {
    $$ = ast_new_lexeme_node($5, $3);
    
    symbol_t* symbol = symbol_new(FUNCTION_NATURE, $3, $5);
    short success = symbol_table_add(table_stack->top->prev->symbol_table, symbol);
    if(!success) return ERR_DECLARED;
  }
;

function_parameters:
  '(' parameters_list ')' | '(' ')'
;

parameters_list:
  parameters_list ';' type TK_IDENTIFICADOR
  {
    symbol_t* symbol = symbol_new(IDENTIFIER_NATURE, $3, $4);
    short success = symbol_table_add(table_stack->top->symbol_table, symbol);
    if(!success) return ERR_DECLARED;
  }
| type TK_IDENTIFICADOR
  {
    symbol_t* symbol = symbol_new(IDENTIFIER_NATURE, $1, $2);
    short success = symbol_table_add(table_stack->top->symbol_table, symbol);
    if(!success) return ERR_DECLARED;
  }
;

function_body:
  command_block_without_scope_rules                 {$$ = $1;}
;

/* ======================= Commands ======================= */
command_block_without_scope_rules:
  '{' commands_list '}'                             {$$ = $2;}
| '{' '}'                                           {$$ = NULL;}
;

command_block:
  '{' CREATE_SCOPE commands_list REMOVE_SCOPE '}'   {$$ = $3;}
| '{' '}'                                           {$$ = NULL;}
;

commands_list: command commands_list
{
    if ($1 != NULL)
    {
      $$ = $1;
      ast_add_child($$, $2);
    }
    else
    {
      // if this command is a NULL node, consider the next
      // eg. variable declaration command
      $$ = $2; 
    }
}
| command                                 {$$ = $1;}
;

command: 
  command_block ','                       {$$ = $1;}    /* Recursive block */
| variable_declaration ','                {$$ = NULL;}
| attribution_command ','                 {$$ = $1;}
| function_call ','                       {$$ = $1;}
| return_command ','                      {$$ = $1;}
| conditional_command ','                 {$$ = $1;}
| while_command ','                       {$$ = $1;}
;

/* ======================= Commands: attribution */
attribution_command: 
  TK_IDENTIFICADOR '=' expression         
  {
    symbol_t * symbol = table_stack_find_symbol_or_null(table_stack, $1->lexeme);
    if(symbol == NULL){
      print_undeclared_usage_error($1->lexeme, $1->line_number);
      return ERR_UNDECLARED;
    } 

    if(symbol->nature == FUNCTION_NATURE)
    {
      print_function_as_variable_error($1->lexeme, symbol->lex_data->line_number, $1->line_number);
      return ERR_FUNCTION;
    }

    $$ = ast_new_node("=", symbol->type);
    ast_add_child($$, ast_new_lexeme_node($1, symbol->type));
    ast_add_child($$, $3);

    char* scope = table_stack_find_symbol_scope(table_stack, $1->lexeme);
    char* offset_str = get_offset_string(symbol->offset);

    $$->code = $3->code;

    iloc_instruction_t storeai = new_3_operand_instruction("storeAI", $3->temp, scope, offset_str);
    push_instruction($$->code, storeai);
  }
;

/* ======================= Commands: function call */
function_call:
  TK_IDENTIFICADOR function_arguments     
  {
    symbol_t * symbol = table_stack_find_symbol_or_null(table_stack, $1->lexeme);
    
    if(symbol == NULL){
      print_undeclared_usage_error($1->lexeme, $1->line_number);
      return ERR_UNDECLARED;
    } 

    if(symbol->nature == IDENTIFIER_NATURE)
    {
      print_variable_as_function_error($1->lexeme, symbol->lex_data->line_number, $1->line_number);
      return ERR_VARIABLE;
    }

    $$ = ast_new_lexeme_node_prefix_label($1, "call ", symbol->type);
    ast_add_child($$, $2);
  }
;

function_arguments:
  '(' arguments_list ')'                  {$$ = $2;}
| '(' ')'                                 {$$ = NULL;}
;

arguments_list: 
  expression ';' arguments_list           {$$ = $1; ast_add_child($$, $3);}
| expression                              {$$ = $1;}
;

/* ======================= Commands: return */
return_command: 
  TK_PR_RETURN expression                           {$$ = ast_new_node("return", $2->type); ast_add_child($$, $2);}
;

/* ======================= Commands: conditional */
conditional_command:
  if_command else_command                           {$$ = $1; ast_add_child($$, $2);}
| if_command                                        {$$ = $1;}
;

if_command: 
  TK_PR_IF '(' expression ')' command_block         {$$ = ast_new_node("if", BOOL); ast_add_child($$, $3); ast_add_child($$, $5);}
;

else_command:
  TK_PR_ELSE command_block                          {$$ = $2;}
;

/* ======================= Commands: while */
while_command:
  TK_PR_WHILE '(' expression ')' command_block
  {
    $$ = ast_new_node("while", BOOL);
    ast_add_child($$, $3);
    ast_add_child($$, $5);

    char* init_while = generate_label();
    char* true_branch = generate_label();
    char* false_branch = generate_label();
 
    /* 
      init_while: nop
      $3->code
      cbr $3->temp true_branch false_branch
      true_branch: nop
      $5->code
      jump init_while
      false_label: nop
    */
    $$->code = new_program();
    push_instruction($$->code, new_label_instruction(init_while));
    concat_programs($$->code, $3->code);
    push_instruction($$->code, new_3_operand_instruction("cbr", $3->temp, true_branch, false_branch));
    push_instruction($$->code, new_label_instruction(true_branch));
    concat_programs($$->code, $5->code);
    push_instruction($$->code, new_1_operand_instruction("jump", init_while));
    push_instruction($$->code, new_label_instruction(false_branch));
  }
;

/* ======================= Expressions ======================= */
expression:
  expr_or                                     {$$ = $1;}
;

expr_or:
  expr_or TK_OC_OR expr_and                             ///////////* 7: OR *///////////       
  {
    $$ = ast_new_node("|", infer_type($1->type, $3->type));
    ast_add_child($$, $1);
    ast_add_child($$, $3);
    
    $$->temp = generate_register();
    $$->code = $1->code;

    concat_programs($$->code, $3->code);
    iloc_instruction_t or = new_3_operand_instruction("or", $1->temp, $3->temp, $$->temp);
    push_instruction($$->code, or);
  }   
| expr_and                                    {$$ = $1;}
;

expr_and:
  expr_and TK_OC_AND expr_eq_ne                         ///////////* 6: AND *///////////        
  {
    $$ = ast_new_node("&", infer_type($1->type, $3->type));
    ast_add_child($$, $1);
    ast_add_child($$, $3);

    $$->temp = generate_register();
    $$->code = $1->code;

    concat_programs($$->code, $3->code);
    iloc_instruction_t and = new_3_operand_instruction("and", $1->temp, $3->temp, $$->temp);
    push_instruction($$->code, and);
  }
| expr_eq_ne                                  {$$ = $1;}
;

expr_eq_ne:
  expr_eq_ne TK_OC_EQ expr_comparisons                  ///////////* 5: EQUAL *///////////  
  {
    $$ = ast_new_node("==", infer_type($1->type, $3->type));
    ast_add_child($$, $1);
    ast_add_child($$, $3);

    $$->temp = generate_register();
    $$->code = $1->code;

    concat_programs($$->code, $3->code);
    iloc_instruction_t cmp_EQ = new_3_operand_instruction("cmp_EQ", $1->temp, $3->temp, $$->temp);
    push_instruction($$->code, cmp_EQ);
  }  
| expr_eq_ne TK_OC_NE expr_comparisons                  ///////////* 5: NOT EQUAL *///////////
  {
    $$ = ast_new_node("!=", infer_type($1->type, $3->type));
    ast_add_child($$, $1);
    ast_add_child($$, $3);

    $$->temp = generate_register();
    $$->code = $1->code;

    concat_programs($$->code, $3->code);
    iloc_instruction_t cmp_NE = new_3_operand_instruction("cmp_NE", $1->temp, $3->temp, $$->temp);
    push_instruction($$->code, cmp_NE);
  }  
| expr_comparisons                            {$$ = $1;}
;

expr_comparisons: 
  expr_comparisons TK_OC_GE expr_plus_minus             ///////////* 4: GREATER OR EQUAL *///////////
  {
    $$ = ast_new_node(">=", infer_type($1->type, $3->type));
    ast_add_child($$, $1);
    ast_add_child($$, $3);

    $$->temp = generate_register();
    $$->code = $1->code;

    concat_programs($$->code, $3->code);
    iloc_instruction_t cmp_GE = new_3_operand_instruction("cmp_GE", $1->temp, $3->temp, $$->temp);
    push_instruction($$->code, cmp_GE);
  } 
| expr_comparisons TK_OC_LE expr_plus_minus             ///////////* 4: LESS OR EQUAL *///////////
  {
    $$ = ast_new_node("<=", infer_type($1->type, $3->type));
    ast_add_child($$, $1);
    ast_add_child($$, $3);

    $$->temp = generate_register();
    $$->code = $1->code;

    concat_programs($$->code, $3->code);
    iloc_instruction_t cmp_LE = new_3_operand_instruction("cmp_LE", $1->temp, $3->temp, $$->temp);
    push_instruction($$->code, cmp_LE);
  }  
| expr_comparisons '>' expr_plus_minus                  ///////////* 4: GREATER *///////////
  {
    $$ = ast_new_node(">", infer_type($1->type, $3->type));
    ast_add_child($$, $1);
    ast_add_child($$, $3);

    $$->temp = generate_register();
    $$->code = $1->code;

    concat_programs($$->code, $3->code);
    iloc_instruction_t cmp_GT = new_3_operand_instruction("cmp_GT", $1->temp, $3->temp, $$->temp);
    push_instruction($$->code, cmp_GT);
  }   
| expr_comparisons '<' expr_plus_minus                  ///////////* 4: LESS *///////////
  {
    $$ = ast_new_node("<", infer_type($1->type, $3->type));
    ast_add_child($$, $1);
    ast_add_child($$, $3);

    $$->temp = generate_register();
    $$->code = $1->code;

    concat_programs($$->code, $3->code);
    iloc_instruction_t cmp_LT = new_3_operand_instruction("cmp_LT", $1->temp, $3->temp, $$->temp);
    push_instruction($$->code, cmp_LT);
  }   
| expr_plus_minus                             {$$ = $1;}
;

expr_plus_minus:
  expr_plus_minus '+' expr_times_div_mod                ///////////* 3  PLUS *///////////
  {
    $$ = ast_new_node("+", infer_type($1->type, $3->type));
    ast_add_child($$, $1);
    ast_add_child($$, $3);

    $$->temp = generate_register();
    $$->code = $1->code;

    concat_programs($$->code, $3->code);
    iloc_instruction_t add = new_3_operand_instruction("add", $1->temp, $3->temp, $$->temp);
    push_instruction($$->code, add);
  }   
| expr_plus_minus '-' expr_times_div_mod                ///////////* 3  MINUS *///////////
  {
    $$ = ast_new_node("-", infer_type($1->type, $3->type));
    ast_add_child($$, $1);
    ast_add_child($$, $3);

    $$->temp = generate_register();
    $$->code = $1->code;

    concat_programs($$->code, $3->code);
    iloc_instruction_t sub = new_3_operand_instruction("sub", $1->temp, $3->temp, $$->temp);
    push_instruction($$->code, sub);
  }   
| expr_times_div_mod                          {$$ = $1;}
;

expr_times_div_mod:
  expr_times_div_mod '*' expr_unary                     ///////////* 2: MULTIPLICATION *///////////
  {
    $$ = ast_new_node("*", infer_type($1->type, $3->type));
    ast_add_child($$, $1);
    ast_add_child($$, $3);

    $$->temp = generate_register();
    $$->code = $1->code;

    concat_programs($$->code, $3->code);
    iloc_instruction_t mult = new_3_operand_instruction("mult", $1->temp, $3->temp, $$->temp);
    push_instruction($$->code, mult);
  }
| expr_times_div_mod '/' expr_unary                     ///////////* 2: DIVISION *///////////
  {
    $$ = ast_new_node("/", infer_type($1->type, $3->type));
    ast_add_child($$, $1);
    ast_add_child($$, $3);

    $$->temp = generate_register();
    $$->code = $1->code;

    concat_programs($$->code, $3->code);
    iloc_instruction_t div = new_3_operand_instruction("div", $1->temp, $3->temp, $$->temp);
    push_instruction($$->code, div);
  }   
| expr_times_div_mod '%' expr_unary           {$$ = ast_new_node("%", infer_type($1->type, $3->type)); ast_add_child($$, $1); ast_add_child($$, $3);}   /* 2: MODULE */
| expr_unary                                  {$$ = $1;}
;

expr_unary: 
  '-' expr_unary                                        ///////////* 1: UNARY MINUS *///////////
  {
    $$ = ast_new_node("-", $2->type); ast_add_child($$, $2);

    $$->temp = generate_register();
    $$->code = $2->code;

    iloc_instruction_t multI = new_3_operand_instruction("multI", $2->temp, "-1", $$->temp);
    push_instruction($$->code, multI);
  }   
| '!' expr_unary                                        ///////////* 1: NEGATE      *////////////
  {
    $$ = ast_new_node("!", $2->type); ast_add_child($$, $2);

    $$->temp = generate_register();
    $$->code = $2->code;

    char* temp_for_const_zero = generate_register();
    iloc_instruction_t loadi = new_2_operand_instruction("loadI", 0, temp_for_const_zero);
    iloc_instruction_t cmp_eq = new_3_operand_instruction("cmp_EQ", $2->temp, temp_for_const_zero, $$->temp);
    push_instruction($$->code, loadi);
    push_instruction($$->code, cmp_eq);

    free(temp_for_const_zero);
  }   
| expr_parentheses                            {$$ = $1;}
;

expr_parentheses: 
  '(' expression ')'                          {$$ = $2;}  ///////////* 0: PARENTHESES *///////////
| operands                                    {$$ = $1;}
;

operands: 
  TK_IDENTIFICADOR                            
  {
    symbol_t * symbol = table_stack_find_symbol_or_null(table_stack, $1->lexeme);
    if(symbol == NULL){
      print_undeclared_usage_error($1->lexeme, $1->line_number);
      return ERR_UNDECLARED;
    } 

    if(symbol->nature == FUNCTION_NATURE)
    {
      print_function_as_variable_error($1->lexeme, symbol->lex_data->line_number, $1->line_number);
      return ERR_FUNCTION;
    }

    char* scope = table_stack_find_symbol_scope(table_stack, $1->lexeme);
    $$ = ast_new_lexeme_node($1, UNKNOWN);

    $$->temp = generate_register();
    $$->code = new_program();

    char* offset_str = get_offset_string(symbol->offset);
    iloc_instruction_t loadai = new_3_operand_instruction("loadAI", scope, offset_str, $$->temp);
    push_instruction($$->code, loadai);
  }
| TK_LIT_TRUE                                 {$$ = ast_new_lexeme_node($1, BOOL);}
| TK_LIT_FALSE                                {$$ = ast_new_lexeme_node($1, BOOL);}
| TK_LIT_INT                                  
  {
    $$ = ast_new_lexeme_node($1, INT);
    
    $$->temp = generate_register();
    $$->code = new_program();

    iloc_instruction_t loadi = new_2_operand_instruction("loadI", $1->lexeme, $$->temp);
    push_instruction($$->code, loadi);
  }
| TK_LIT_FLOAT                                {$$ = ast_new_lexeme_node($1, FLOAT);}
| function_call                               {$$ = $1;}
;

/* ======================= Primitives types ======================= */
type:
  TK_PR_INT     {$$ = INT;}
| TK_PR_FLOAT   {$$ = FLOAT;}
| TK_PR_BOOL    {$$ = BOOL;}
;

/* ==\===================== Semantic Analysis Methods ======================= */
CREATE_SCOPE: 
{
  table_stack_push_default_table(table_stack);
};

REMOVE_SCOPE: 
{
  symbol_table_t* table = table_stack_pop(table_stack);
  symbol_table_free(table);
};
%%



#include <stdio.h>

extern int yylineno;
void yyerror (char const *mensagem)
{
    fprintf(stderr, "At line %d: %s\n", yylineno, mensagem);
}