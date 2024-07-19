#ifndef _TABLE_H_
#define _TABLE_H_

#include "dataType.h"
#include "ast.h"

#define ERR_UNDECLARED 10 // 2.3
#define ERR_DECLARED 11   // 2.3
#define ERR_VARIABLE 20   // 2.4
#define ERR_FUNCTION 21   // 2.4

enum nature
{
    FUNCTION_NATURE,
    IDENTIFIER_NATURE,
};
typedef enum nature nature_t;

// =================================== SYMBOL TABLE ===================================
typedef struct symbol
{
    nature_t nature;
    data_type_t type;
    lexical_data_t *lex_data;

} symbol_t;

typedef struct symbol_table
{
    symbol_t **symbols;
    unsigned int symbol_count;
    unsigned int size;
} symbol_table_t;

symbol_table_t *symbol_table_new(unsigned int size);
void symbol_table_free(symbol_table_t *table);
void symbol_table_add(symbol_table_t *table, symbol_t symbol);
symbol_t *symbol_table_get_or_null(symbol_table_t *table, char *lexeme);
int _hash(symbol_table_t *table, char *lexeme);

// ====================================== STACK ======================================

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
table_stack_t *table_stack_free();
symbol_table_t *table_stack_pop(table_stack_t *stack);
symbol_table_t *table_stack_peek(table_stack_t *stack);
void table_stack_push(table_stack_t *stack, symbol_table_t *table);
int table_stack_is_empty(table_stack_t *stack);
_node_stack_t *_node_stack_new(symbol_table_t *table);

#endif