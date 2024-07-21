#include "semanticErrors.h"

void print_duplicated_declaration_error(char *identifier, int first_line, int duplication_line)
{
    printf("ERROR: duplicated declaration at line %d. ", duplication_line);
    printf("Identifier '%s' was previously declared at line %d.\n", identifier, first_line);
}

void print_undeclared_usage_error(char *identifier, int line)
{
    printf("ERROR: using undeclared identifier '%s' at line %d.\n", identifier, line);
}

void print_function_as_variable_error(char *identifier, int declaration_line, int usage_line)
{
    printf("ERROR: function identifier being used as variable at line %d. ", usage_line);
    printf("Identifier '%s' was previously declared as function at line %d.\n", identifier, declaration_line);
}
