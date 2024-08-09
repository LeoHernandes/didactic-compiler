#ifndef _ASM_H_
#define _ASM_H_

#include <stdio.h>
#include <iloc.h>
#include <symbolTable.h>

void print_main_function_info();
void print_global_variable_info(char *identifier, int variable_pos);
void print_global_variables_info(iloc_program_t *program, symbol_table_t *global_table);
void print_data_segment(iloc_program_t *program, symbol_table_t *global_table);
void print_code_segment(iloc_program_t *program, symbol_table_t *global_table);
void print_asm(iloc_program_t *program, symbol_table_t *global_table);

#endif
