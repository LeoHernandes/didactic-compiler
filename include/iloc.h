#ifndef _ILOC_H
#define _ILOC_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define enum_to_str(x) #x

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
    I2I,
    JUMP,
    MULT,
    OR,
    SUB,
};
typedef enum iloc_op_code iloc_op_code_t;

typedef struct iloc_instruction
{
    iloc_op_code_t op_code;
    char *operand_1;
    char *operand_2;
    char *operand_3;
} iloc_instruction_t;

iloc_instruction_t new_1_operand_instruction(iloc_op_code_t op_code, char *operand);
iloc_instruction_t new_2_operand_instruction(iloc_op_code_t op_code, char *operand_1, char *operand_2);
iloc_instruction_t new_3_operand_instruction(iloc_op_code_t op_code, char *operand_1, char *operand_2, char *operand_3);

/* ================================== Vector of ILOC operations (program) ================================== */

typedef struct iloc_program
{
    iloc_instruction_t *instructions;
    int length;
} iloc_program_t;

iloc_program_t *new_program();
void concat_programs(iloc_program_t *dest, iloc_program_t *src);
void push_operation(iloc_program_t *program, iloc_instruction_t instruction);

/* ================================== Auxiliar functions ================================== */

char *generate_label();
char *generate_register();

#endif