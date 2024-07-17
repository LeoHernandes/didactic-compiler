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
    symbol_t *table;
    unsigned int length;
    unsigned int size;
} symbol_table_t;

symbol_table_t *symbol_table_new(unsigned int size);
void symbol_table_free(symbol_table_t table);
// ====================================== STACK ======================================

typedef struct _node_stack
{
    symbol_table_t *symbol_table;
    symbol_table_t *prev;
} _node_stack_t;

typedef struct table_stack
{
    _node_stack_t *top;
    unsigned int length;
} table_stack_t;

table_stack_t *table_stack_new();
symbol_table_t *table_stack_pop(table_stack_t *stack);
void table_stack_push(table_stack_t *stack, symbol_table_t *table);

#endif