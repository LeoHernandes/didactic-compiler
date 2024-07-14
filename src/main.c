#include <stdio.h>
#include "ast.h"

extern int yyparse(void);
extern int yylex_destroy(void);

ast_t *ast_root = NULL;
/**
 * @deprecated Printing the AST isn't necessary anymore
 */
// void exporta(ast_t *ast_root);

int main(int argc, char **argv)
{
  int ret = yyparse();
  yylex_destroy();

  if (ast_root != NULL) // if it isn't a empty program
  {
    ast_free(ast_root);
  }

  return ret;
}
