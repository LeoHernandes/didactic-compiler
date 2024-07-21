#include "semanticErrors.h"

void print_duplicated_declaration_error(char *identifier, int first_line, int duplication_line)
{
    printf("ERROR: duplicated declaration. Identifier '%s' was declared at line %d, but redeclared at line %d\n", identifier, first_line, duplication_line);
}
