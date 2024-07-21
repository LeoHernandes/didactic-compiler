#include "table.h"

symbol_t *symbol_new(nature_t nature, data_type_t type, lexical_data_t *lex_data)
{
    symbol_t *symbol = malloc(sizeof(symbol_t));
    symbol->nature = nature;
    symbol->type = type;
    symbol->lex_data = lex_data;

    return symbol;
}

symbol_table_t *symbol_table_new(unsigned int size)
{
    symbol_table_t *table = malloc(sizeof(symbol_table_t));

    table->symbol_count = 0;
    table->size = size;
    table->symbols = malloc(size * sizeof(symbol_t *));

    if (table->symbols == NULL)
    {
        printf("ERROR: %s couldn't allocate memory\n", __FUNCTION__);
        return NULL;
    }

    for (int i = 0; i < size; i++)
    {
        table->symbols[i] = NULL;
    }

    return table;
};

void symbol_table_free(symbol_table_t *table)
{
    if (table != NULL)
    {
        if (table->symbols != NULL)
        {
            free(table->symbols);
        }
        free(table);
    }
}

/*
Hash function using a prime number
*/
int _hash(symbol_table_t *table, char *lexeme)
{
    int pos = 1;
    size_t size = strlen(lexeme);
    const int prime = 31;

    for (int i = 0; i < size; i++)
    {
        pos += (lexeme[i] * i) % prime;
    }
    pos %= table->size;
    return pos;
}

void symbol_table_add(symbol_table_t *table, symbol_t *symbol)
{
    int pos;
    pos = _hash(table, symbol->lex_data->lexeme);
    while (table->symbols[pos] != NULL)
    {
        pos = (pos + 1) % table->size;
    }
    table->symbols[pos] = symbol;
    table->symbol_count++;

    if (table->symbol_count >= table->size)
    {
        table->size *= 2;
        table->symbols = realloc(table->symbols, table->size * sizeof(symbol_t *));

        if (table->symbols == NULL)
        {
            printf("ERROR: %s couldn't reallocate memory\n", __FUNCTION__);
        }
    }
}

symbol_t *symbol_table_get_or_null(symbol_table_t *table, char *lexeme)
{
    if (table->symbol_count == 0)
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

    if (stack == NULL)
    {
        printf("ERROR: %s couldn't allocate memory\n", __FUNCTION__);
        return NULL;
    }

    stack->top = NULL;
    stack->length = 0;
    return stack;
}

void table_stack_free(table_stack_t *stack)
{
    if (stack != NULL)
    {
        if (stack->length > 0)
        {
            _node_stack_t *current_node = stack->top;
            for (int i = 0; i < stack->length; i++)
            {
                symbol_table_free(current_node->symbol_table);
                current_node = current_node->prev;
            }
            free(stack->top);
        }
        free(stack);
    }
}

int table_stack_is_empty(table_stack_t *stack)
{
    return stack->length == 0;
}

_node_stack_t *_node_stack_new(symbol_table_t *table)
{
    _node_stack_t *node = malloc(sizeof(_node_stack_t));

    if (node == NULL)
    {
        printf("ERROR: %s couldn't allocate memory\n", __FUNCTION__);
        return NULL;
    }

    node->symbol_table = table;
    node->prev = NULL;

    return node;
}

void table_stack_push_default_table(table_stack_t *stack)
{
    symbol_table_t *table = symbol_table_new(DEFAULT_TABLE_SIZE);
    table_stack_push(stack, table);
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