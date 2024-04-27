#include <stdio.h>
#include "ast.h"

extern int yyparse(void);
extern int yylex_destroy(void);

ast_t *ast_root = NULL;
void exporta(void *ast_root);

int main(int argc, char **argv)
{
  int ret = yyparse();
  yylex_destroy();
  exporta(ast_root);
  ast_free(ast_root);
  return ret;
}

void exporta(void *ast_root)
{
  printf("FOOBAR");
}