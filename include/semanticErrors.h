#ifndef _SEMANTICERRORS_H_
#define _SEMANTICERRORS_H_

#include <stdio.h>

void print_duplicated_declaration_error(char *identifier, int first_line, int duplication_line);
void print_undeclared_usage_error(char *identifier, int line);
void print_function_as_variable_error(char *identifier, int declaration_line, int usage_line);

#endif