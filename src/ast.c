#include "ast.h"

void ast_free(ast_t *tree)
{
    if (tree == NULL)
    {
        printf("Warning: tried to free a NULL tree\n");
        return;
    }

    for (int i = 0; i < tree->number_of_children; i++)
    {
        ast_free(tree->children[i]);
    }
    free(tree->children);
    free(tree->label);
    free(tree->value);
    free(tree);
}