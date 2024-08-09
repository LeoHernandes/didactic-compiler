#ifndef _ILOC_H
#define _ILOC_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/* ================================== ILOC Operation (instruction) ================================== */

enum iloc_op_code
{
    ADD,
    AND,
    CBR,
    CMP_EQ,
    CMP_GE,
    CMP_GT,
    CMP_LE,
    CMP_LT,
    CMP_NE,
    DIV,
    JUMPI,
    LOADAI,
    LOADI,
    MULT,
    MULTI,
    NOP,
    OR,
    RET,
    STOREAI,
    SUB,
};

typedef enum iloc_op_code iloc_op_code_t;

typedef struct iloc_instruction
{
    char *label;
    iloc_op_code_t op_code;
    char *operand_1;
    char *operand_2;
    char *operand_3;
} iloc_instruction_t;

iloc_instruction_t new_label_instruction(char *label);
iloc_instruction_t new_1_operand_instruction(iloc_op_code_t op_code, char *dest);
iloc_instruction_t new_2_operand_instruction(iloc_op_code_t op_code, char *src, char *dest);
iloc_instruction_t new_3_operand_instruction(iloc_op_code_t op_code, char *operand_1, char *operand_2, char *dest);

/* ================================== Vector of ILOC operations (program) ================================== */

typedef struct iloc_program
{
    iloc_instruction_t *instructions;
    int length;
} iloc_program_t;

iloc_program_t *new_program();
void concat_programs(iloc_program_t *dest, iloc_program_t *src);
void push_instruction(iloc_program_t *program, iloc_instruction_t instruction);
void free_program_labels(iloc_program_t *program);

/**
 * @deprecated from now on, we'll generate asm code directly from iloc_program.
 * Printing iloc program isn't necessary anymore
 */
// void print_program(iloc_program_t *program);

/* ================================== Auxiliar functions ================================== */

char *generate_label();
char *generate_register();
char *get_offset_string(unsigned int offset);

#endif