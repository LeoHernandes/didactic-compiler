#include <stdio.h>
#include "ast.h"
#include "table.h"

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
  symbol_table_t *global_table = symbol_table_new(DEFAULT_TABLE_SIZE);
  table_stack_push(table_stack, global_table);

  int ret = yyparse();
  yylex_destroy();

  if (ast_root != NULL) // if it isn't a empty program
  {
    ast_free(ast_root);
  }

  table_stack_free(table_stack);

  return ret;
}
