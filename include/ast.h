#ifndef _AST_H_
#define _AST_H_

#include <stdlib.h>
#include <stdio.h>
#include <stddef.h>
#include "parser.tab.h"

typedef struct lexical_data
{
    int line_number;
    yytoken_kind_t token;
    char *lexeme;
} lexical_data_t;

typedef struct ast
{
    char *label;
    lexical_data_t *value;
    int number_of_children;
    struct ast **children;
} ast_t;

/*
 * Frees allocated memory of the current node and its children
 */
void ast_free(ast_t *tree);

#endif