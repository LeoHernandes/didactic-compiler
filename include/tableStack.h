#ifndef _TABLESTACK_H_
#define _TABLESTACK_H_

#include "symbolTable.h"
#include <string.h>

typedef struct _node_stack
{
    symbol_table_t *symbol_table;
    struct _node_stack *prev;
} _node_stack_t;

typedef struct table_stack
{
    _node_stack_t *top;
    unsigned int length;
} table_stack_t;

table_stack_t *table_stack_new();

void table_stack_free(table_stack_t *stack);

symbol_table_t *table_stack_pop(table_stack_t *stack);

void table_stack_push(table_stack_t *stack, symbol_table_t *table);

/*
 * Inserts an empty default table on the stack top
 */
void table_stack_push_default_table(table_stack_t *stack);

int table_stack_is_empty(table_stack_t *stack);

_node_stack_t *_node_stack_new(symbol_table_t *table);

/*
 * Returns the first matching symbol, searching from top to bottom on the stack
 * If the symbol is missing, returns NULL
 */
symbol_t *table_stack_find_symbol_or_null(table_stack_t *stack, char *lexeme);

symbol_table_t *table_stack_peek_bottom(table_stack_t *stack);

char *table_stack_find_symbol_scope(table_stack_t *stack, char *lexeme);

#endif