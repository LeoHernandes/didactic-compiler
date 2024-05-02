#ifndef _AST_H_
#define _AST_H_

#include <stdlib.h>
#include <stdio.h>
#include <stddef.h>
#include <string.h>

enum lexical_value_token
{
    IDENTIFIER,
    LITERAL,
};
typedef enum lexical_value_token lexical_value_token_t;

typedef struct lexical_data
{
    int line_number;
    lexical_value_token_t token;
    char *lexeme;
} lexical_data_t;

/*
 * Allocate memory and create instance of `lexical_data` struct
 */
lexical_data_t *lexical_data_new(int line_number, lexical_value_token_t token, char *lexeme);

typedef struct ast
{
    char *label;
    lexical_data_t *value;
    int number_of_children;
    struct ast **children;
} ast_t;

/*
 * Creates a `ast_tree` node
 */
ast_t *ast_new_node(const char *label);

/*
 * Creates a `ast_tree` node with lexical data
 */
ast_t *ast_new_lexeme_node(lexical_data_t *data);

/*
 * Frees allocated memory of the current node and its children
 */
void ast_free(ast_t *tree);

/*
 * Add :child as a child of :node
 */
void ast_add_child(ast_t *node, ast_t *child);

#endif