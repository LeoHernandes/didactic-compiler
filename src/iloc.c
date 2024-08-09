#include "iloc.h"

/* ================================== ILOC Operation (instruction) ================================== */

iloc_instruction_t new_1_operand_instruction(iloc_op_code_t op_code, char *dest)
{
    iloc_instruction_t instruction = {
        .label = NULL,
        .op_code = op_code,
        .operand_1 = NULL,
        .operand_2 = NULL,
        .operand_3 = dest};

    return instruction;
}

iloc_instruction_t new_2_operand_instruction(iloc_op_code_t op_code, char *src, char *dest)
{
    iloc_instruction_t instruction = {
        .label = NULL,
        .op_code = op_code,
        .operand_1 = src,
        .operand_2 = NULL,
        .operand_3 = dest};

    return instruction;
}

iloc_instruction_t new_3_operand_instruction(iloc_op_code_t op_code, char *operand_1, char *operand_2, char *dest)
{
    iloc_instruction_t instruction = {
        .label = NULL,
        .op_code = op_code,
        .operand_1 = operand_1,
        .operand_2 = operand_2,
        .operand_3 = dest};

    return instruction;
}

iloc_instruction_t new_label_instruction(char *label)
{
    iloc_instruction_t instruction = {
        .label = label,
        .op_code = NOP,
        .operand_1 = NULL,
        .operand_2 = NULL,
        .operand_3 = NULL};

    return instruction;
}

/* ================================== Vector of ILOC operations (program) ================================== */

iloc_program_t *new_program()
{
    iloc_program_t *program = malloc(sizeof(iloc_program_t));

    if (program == NULL)
    {
        printf("ERROR: %s couldn't allocate memory\n", __FUNCTION__);
        return NULL;
    }

    program->instructions = NULL;
    program->length = 0;
    return program;
}

void concat_programs(iloc_program_t *dest, iloc_program_t *src)
{
    int new_length = dest->length + src->length;
    dest->instructions = realloc(dest->instructions, sizeof(iloc_instruction_t) * new_length);

    if (dest->instructions == NULL)
    {
        printf("ERROR: %s couldn't allocate memory\n", __FUNCTION__);
    }

    memcpy(
        dest->instructions + dest->length, // Destination: pointer arithmetic to get the start of new memory allocated
        src->instructions,                 // Source: pointer to program_2 instructions array start
        sizeof(iloc_instruction_t) * src->length);

    dest->length = new_length;
}

void push_instruction(iloc_program_t *program, iloc_instruction_t instruction)
{
    program->length++;
    program->instructions = realloc(program->instructions, sizeof(iloc_instruction_t) * program->length);

    if (program->instructions == NULL)
    {
        printf("ERROR: %s couldn't allocate memory\n", __FUNCTION__);
    }

    program->instructions[program->length - 1] = instruction;
}

void free_program_labels(iloc_program_t *program)
{
    for (int i = 0; i < program->length; i++)
    {
        char *label = program->instructions[i].label;
        if (label != NULL)
            free(label);
    }
}

/* ================================== Auxiliar functions ================================== */
unsigned int number_of_digits(int number)
{
    unsigned int number_of_digits = 1;
    while (number >= 10)
    {
        number /= 10;
        number_of_digits++;
    }

    return number_of_digits;
}

char *generate_label()
{
    static unsigned int label_number = 0;

    // Buffer must store 'L[digits]\0'
    unsigned int buffer_size = number_of_digits(label_number) + 1 + 1;
    char *buffer = malloc(sizeof(char) * buffer_size);

    if (buffer == NULL)
    {
        printf("ERROR: %s couldn't allocate memory\n", __FUNCTION__);
        return NULL;
    }

    sprintf(buffer, "L%d", label_number);
    label_number++;

    return buffer;
}

char *generate_register()
{
    static unsigned int register_number = 0;

    // Buffer must store 'r[digits]\0'
    unsigned int buffer_size = number_of_digits(register_number) + 1 + 1;
    char *buffer = malloc(sizeof(char) * buffer_size);

    if (buffer == NULL)
    {
        printf("ERROR: %s couldn't allocate memory\n", __FUNCTION__);
        return NULL;
    }

    sprintf(buffer, "r%d", register_number);
    register_number++;

    return buffer;
}

char *get_offset_string(unsigned int offset)
{
    unsigned int buffer_size = number_of_digits(offset) + 1;
    char *buffer = malloc(sizeof(char) * buffer_size);

    if (buffer == NULL)
    {
        printf("ERROR: %s couldn't allocate memory\n", __FUNCTION__);
        return NULL;
    }

    sprintf(buffer, "%d", offset);
    return buffer;
}
