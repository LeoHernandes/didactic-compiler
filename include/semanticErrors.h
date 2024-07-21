#ifndef _SEMANTICERRORS_H_
#define _SEMANTICERRORS_H_

#include <stdio.h>

void print_duplicated_declaration_error(char *identifier, int first_line, int duplication_line);

#endif