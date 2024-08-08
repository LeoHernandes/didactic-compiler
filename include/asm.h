#ifndef _ASM_H_
#define _ASM_H_

#include <stdio.h>
#include <iloc.h>
#include <tableStack.h>
#include <symbolTable.h>

void print_main_function_info();
void print_global_variable_info(char *identifier, int variable_pos);
void print_global_variables_info(iloc_program_t *program, table_stack_t *stack);
void print_data_segment(iloc_program_t *program, table_stack_t *stack);
void print_code_segment(iloc_program_t *program);
void print_asm(iloc_program_t *program, table_stack_t *stack);

#endif
