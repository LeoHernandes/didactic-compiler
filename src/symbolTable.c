#include "symbolTable.h"

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

    if (table == NULL)
    {
        printf("ERROR: %s couldn't allocate memory\n", __FUNCTION__);
        return NULL;
    }

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

short symbol_table_add(symbol_table_t *table, symbol_t *symbol)
{
    int pos;
    char *lexeme = symbol->lex_data->lexeme;

    pos = _hash(table, lexeme);
    while (table->symbols[pos] != NULL)
    {
        // If symbol already exists in table
        if (strcmp(lexeme, table->symbols[pos]->lex_data->lexeme) == 0)
        {
            print_duplicated_declaration_error(lexeme, table->symbols[pos]->lex_data->line_number, symbol->lex_data->line_number);
            return 0;
        }

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

    return 1;
}

void symbol_table_fill_unknown_types(symbol_table_t *table, data_type_t correct_type)
{
    for (int pos = 0; pos < table->size; pos++)
    {
        symbol_t *symbol = table->symbols[pos];
        if (symbol != NULL && symbol->type == UNKNOWN)
        {
            symbol->type = correct_type;
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