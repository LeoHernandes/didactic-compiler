#include <stdio.h>
#include "asm.h"
#include "ast.h"
#include "tableStack.h"

extern int yyparse(void);
extern int yylex_destroy(void);

ast_t *ast_root = NULL;
table_stack_t *table_stack;
/**
 * @deprecated Printing the AST isn't necessary anymore
 */
// void exporta(ast_t *ast_root);

int main(int argc, char **argv)
{
  table_stack = table_stack_new();

  int ret = yyparse();
  yylex_destroy();

  if (ast_root != NULL) // if it isn't a empty program
  {
    if (ast_root->code != NULL)
    {
      symbol_table_t *global_table = table_stack_peek_bottom(table_stack);
      print_asm(ast_root->code, global_table);
      free_program_labels(ast_root->code);
    }
    ast_free(ast_root);
  }

  table_stack_free(table_stack);

  return ret;
}
