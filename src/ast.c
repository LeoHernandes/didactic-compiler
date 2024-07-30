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

ast_t *ast_new_node(const char *label, data_type_t type)
{
    ast_t *node = NULL;
    node = malloc(sizeof(ast_t));

    if (node != NULL)
    {
        node->label = strdup(label);

        node->value = NULL;
        node->children = NULL;
        node->number_of_children = 0;
        node->type = type;

        node->temp = NULL;
        node->code = NULL;
    }

    return node;
}

ast_t *ast_new_lexeme_node(lexical_data_t *lex_data, data_type_t type)
{
    ast_t *node = NULL;
    node = malloc(sizeof(ast_t));

    if (node == NULL)
    {
        printf("ERROR: %s couldn't allocate memory\n", __FUNCTION__);
        return NULL;
    }

    node->value = lex_data;
    node->label = lex_data->lexeme;

    node->children = NULL;
    node->number_of_children = 0;
    node->type = type;

    node->temp = NULL;
    node->code = NULL;

    return node;
}

ast_t *ast_new_lexeme_node_prefix_label(lexical_data_t *lex_data, const char *label, data_type_t type)
{
    ast_t *node = NULL;
    node = malloc(sizeof(ast_t));

    if (node == NULL)
    {
        printf("ERROR: %s couldn't allocate memory\n", __FUNCTION__);
        return NULL;
    }

    char *prefixed_label = strdup(label);
    strcat(prefixed_label, lex_data->lexeme);

    node->value = lex_data;
    node->label = prefixed_label;

    node->children = NULL;
    node->number_of_children = 0;
    node->type = type;

    node->temp = NULL;
    node->code = NULL;

    return node;
}

void ast_free(ast_t *tree)
{
    if (tree != NULL)
    {
        for (int i = 0; i < tree->number_of_children; i++)
        {
            ast_free(tree->children[i]);
        }
        free(tree->children);
        free(tree->label);
        free(tree->value);
        free(tree);

        if (tree->temp != NULL)
            free(tree->temp);

        if (tree->code != NULL)
            free(tree->code);
    }
}

void ast_add_child(ast_t *node, ast_t *child)
{
    if (node == NULL)
    {
        printf("ERROR: tried to add a child node to a NULL parent node\n");
        return;
    }

    if (child != NULL)
    {
        node->number_of_children++;
        node->children = realloc(node->children, node->number_of_children * sizeof(ast_t *));
        node->children[node->number_of_children - 1] = child;
    }
}