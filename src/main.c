#include <stdio.h>
#include "ast.h"

extern int yyparse(void);
extern int yylex_destroy(void);

ast_t *ast_root = NULL;
void exporta(ast_t *ast_root);

int main(int argc, char **argv)
{
  int ret = yyparse();
  yylex_destroy();

  if (ast_root != NULL) // if it isn't a empty program
  {
    exporta(ast_root);
    ast_free(ast_root);
  }

  return ret;
}

void exporta(ast_t *ast_root)
{
  int i;
  if (ast_root != NULL)
  {
    for (i = 0; i < ast_root->number_of_children; i++)
    {
      printf("%p, %p\n", ast_root, ast_root->children[i]);
      exporta(ast_root->children[i]);
    }
    printf("%p [label=\"%s\"]\n", ast_root, ast_root->label);
  }
}