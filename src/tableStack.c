#include "tableStack.h"

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
    stack->top = stack->top->prev;
    stack->length--;
    return top_table;
}

symbol_t *table_stack_find_symbol_or_null(table_stack_t *stack, char *lexeme)
{
    _node_stack_t *current_stack_node = stack->top;
    symbol_t *symbol = NULL;
    for (int i = 0; i < stack->length; i++)
    {
        symbol = symbol_table_get_or_null(current_stack_node->symbol_table, lexeme);
        if (symbol != NULL)
            return symbol;

        current_stack_node = current_stack_node->prev;
    }
    return NULL;
}

char *table_stack_find_symbol_scope(table_stack_t *stack, char *lexeme)
{
    symbol_t *symbol = NULL;
    symbol = symbol_table_get_or_null(stack->top->symbol_table, lexeme);
    if (symbol != NULL)
    {
        return "rfp";
    }
    return "rbss";
}