#include "ast.h"

lexical_data_t *lexical_data_new(int line_number, lexical_value_token_t token, char *lexeme)
{
    lexical_data_t *data = NULL;
    data = malloc(sizeof(lexical_data_t));

    if (data != NULL)
    {
        data->line_number = line_number;
        data->token = token;
        data->lexeme = lexeme;
    }

    return data;
}

ast_t *ast_new_node(char *label)
{
    ast_t *node = NULL;
    node = malloc(sizeof(ast_t));

    if (node != NULL)
    {
        node->label = label;

        node->value = NULL;
        node->children = NULL;
        node->number_of_children = 0;
    }

    return node;
}

ast_t *ast_new_lexeme_node(lexical_data_t *lex_data)
{
    ast_t *node = NULL;
    node = malloc(sizeof(ast_t));

    if (node != NULL)
    {
        node->value = lex_data;
        node->label = lex_data->lexeme;

        node->children = NULL;
        node->number_of_children = 0;
    }

    return node;
}

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
    // free(tree->label); // TODO: find why this is causing invalid pointer
    free(tree->value);
    free(tree);
}

void ast_add_child(ast_t *node, ast_t *child)
{
    if (node != NULL && child != NULL)
    {
        node->number_of_children++;
        node->children = realloc(node->children, node->number_of_children * sizeof(ast_t *));
        node->children[node->number_of_children - 1] = child;
    }
    else if (node != NULL)
    {
        printf("Erro: %s recebeu parâmetro node = %s com child NULL.\n", __FUNCTION__, node->label);
    }
    else if (child != NULL)
    {
        printf("Erro: %s recebeu parâmetro node NULL com child = %s.\n", __FUNCTION__, child->label);
    }
    else
    {
        printf("Erro: %s recebeu parâmetro node e child NULL.\n", __FUNCTION__);
    }
}