#ifndef _ASM_H_
#define _ASM_H_

#include <stdio.h>
#include <iloc.h>

void print_main_function_info();
void print_global_variables_info();
void print_data_segment(iloc_program_t *program);
void print_code_segment(iloc_program_t *program);
void print_asm(iloc_program_t *program);

#endif
