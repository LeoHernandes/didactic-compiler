#ifndef _SYMBOLTABLE_H_
#define _SYMBOLTABLE_H_

#include "dataType.h"
#include "ast.h"
#include "semanticErrors.h"
#include <string.h>

#define ERR_UNDECLARED 10 // 2.3
#define ERR_DECLARED 11   // 2.3
#define ERR_VARIABLE 20   // 2.4
#define ERR_FUNCTION 21   // 2.4

#define DEFAULT_TABLE_SIZE 50

enum nature
{
    FUNCTION_NATURE,
    IDENTIFIER_NATURE,
};
typedef enum nature nature_t;

typedef struct symbol
{
    nature_t nature;
    data_type_t type;
    lexical_data_t *lex_data;

} symbol_t;

symbol_t *symbol_new(nature_t nature, data_type_t type, lexical_data_t *lex_data);
typedef struct symbol_table
{
    symbol_t **symbols;
    unsigned int symbol_count;
    unsigned int size;
} symbol_table_t;

symbol_table_t *symbol_table_new(unsigned int size);

void symbol_table_free(symbol_table_t *table);

/*
* Adds a symbol to table
* Returns:
    1 - Insertion was successful
    0 - ERROR: symbol already exists
*/

short symbol_table_add(symbol_table_t *table, symbol_t *symbol);

/*
 * Iterates through the table and stores the given type on the symbols
 * that have unknown data type
 */
void symbol_table_fill_unknown_types(symbol_table_t *table, data_type_t correct_type);

symbol_t *symbol_table_get_or_null(symbol_table_t *table, char *lexeme);

int _hash(symbol_table_t *table, char *lexeme);

#endif