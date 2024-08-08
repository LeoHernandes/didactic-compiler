#include "asm.h"

void print_main_function_info()
{
    const char *str = "\t.long\t3\n"
                      "\t.text\n"
                      "\t.globl\tmain\n"
                      "\t.type\tmain, @function\n";
    printf("%s", str);
}

void print_data_segment(iloc_program_t *program)
{
    // print_global_variables_info(program);
    print_main_function_info();
}

void print_asm(iloc_program_t *program)
{
    print_data_segment(program);
    // print_code_segment(program);
}