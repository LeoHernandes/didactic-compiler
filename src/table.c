#include "table.h"

symbol_table_t *symbol_table_new(unsigned int size)
{
    symbol_table_t *ht;
    ht->symbols = malloc(size * sizeof(symbol_t));

    int i;
    for (i = 0; i < size; i++)
    {
        ht->symbols[i] = NULL;
    }

    return ht;
};

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
};

int symbol_table_add(symbol_table_t *table, symbol_t symbol)
{
    int pos;
    pos = _hash(table, symbol.lex_data->lexeme);
    while (table->symbols[pos] != NULL)
    {
        pos = (pos + 1) % table->size;
    }
    table->symbols[pos] = &symbol;
    table->length++;
    // TODO: reallocate memory if length > size
    return 0;
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
