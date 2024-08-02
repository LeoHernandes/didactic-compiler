#ifndef _ILOC_H
#define _ILOC_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/* ================================== ILOC Operation (instruction) ================================== */

typedef struct iloc_instruction
{
    char *label;
    char *op_code;
    char *operand_1;
    char *operand_2;
    char *operand_3;
} iloc_instruction_t;

iloc_instruction_t new_label_instruction(char *label);
iloc_instruction_t new_1_operand_instruction(char *op_code, char *dest);
iloc_instruction_t new_2_operand_instruction(char *op_code, char *src, char *dest);
iloc_instruction_t new_3_operand_instruction(char *op_code, char *operand_1, char *operand_2, char *dest);
void print_instruction(iloc_instruction_t instruction);

/* ================================== Vector of ILOC operations (program) ================================== */

typedef struct iloc_program
{
    iloc_instruction_t *instructions;
    int length;
} iloc_program_t;

iloc_program_t *new_program();
void concat_programs(iloc_program_t *dest, iloc_program_t *src);
void push_instruction(iloc_program_t *program, iloc_instruction_t instruction);
void print_program(iloc_program_t *program);

/* ================================== Auxiliar functions ================================== */

char *generate_label();
char *generate_register();
char *get_offset_string(unsigned int offset);

#endif