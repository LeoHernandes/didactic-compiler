#include "table.h"

symbol_table_t *symbol_table_new(unsigned int size)
{
    symbol_table_t *ht;
    ht->size = size;
    ht->symbols = malloc(ht->size * sizeof(symbol_t));

    int i;
    for (i = 0; i < size; i++)
    {
        ht->symbols[i] = NULL;
    }

    return ht;
}

/*
Hash function using a prime number
*/
int _hash(symbol_table_t *table, char *lexeme)
{
    int pos = 1;
    size_t size = strlen(lexeme);
    const int prime = 31;
    int prime_pow = 1;
    int i;

    for (i = 0; i < size; i++)
    {
        pos += (lexeme[i] * i) % prime;
    }
    pos %= table->size;
    return pos;
}

void symbol_table_add(symbol_table_t *table, symbol_t symbol)
{
    int pos;
    pos = _hash(table, symbol.lex_data->lexeme);
    while (table->symbols[pos] != NULL)
    {
        pos = (pos + 1) % table->size;
    }
    table->symbols[pos] = &symbol;
    table->length++;
    if (table->length == table->size)
    {
        table->size *= 2;
        table->symbols = realloc(table->symbols, table->size * sizeof(symbol_t));
    }
}

symbol_t *symbol_table_get(symbol_table_t *table, char *lexeme)
{
    if (table->length == 0)
    {
        return NULL;
    }
    int pos = _hash(table, lexeme);

    while (table->symbols[pos] != NULL)
    {
        if (strcmp(table->symbols[pos]->lex_data->lexeme, lexeme) == 0)
        {
            return table->symbols[pos];
        }
        pos = (pos + 1) % table->size;
    }
    return NULL;
}

table_stack_t *table_stack_new()
{
    table_stack_t *stack = malloc(sizeof(table_stack_t));
    stack->top = NULL;
    stack->length = 0;
    return stack;
}

int table_stack_is_empty(table_stack_t *stack)
{
    return stack->length == 0;
}

_node_stack_t *_node_stack_new(symbol_table_t *table)
{
    _node_stack_t *node;
    node->symbol_table = table;
    node->prev = NULL;

    return node;
}

void table_stack_push(table_stack_t *stack, symbol_table_t *table)
{
    _node_stack_t *node = _node_stack_new(table);
    if (table_stack_is_empty(stack))
    {
        stack->top = node;
    }
    else
    {
        node->prev = stack->top;
        stack->top = node;
    }
    stack->length++;
}

symbol_table_t *table_stack_pop(table_stack_t *stack)
{
    if (table_stack_is_empty(stack))
    {
        return NULL;
    }
    symbol_table_t *top_table = stack->top->symbol_table;
    stack->top == stack->top->prev;
    stack->length--;
    return top_table;
}

symbol_table_t *table_stack_peek(table_stack_t *stack)
{
    return stack->top->symbol_table;
}