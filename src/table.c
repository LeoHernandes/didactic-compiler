#include "table.h"

symbol_table_t *symbol_table_new(unsigned int size)
{
    symbol_table_t *ht;
    ht->table = malloc(size * sizeof(symbol_t));

    return ht;
};