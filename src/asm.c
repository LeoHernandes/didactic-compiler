#include "asm.h"

void print_main_function_info()
{
    printf(
        "\t.long\t3\n"
        "\t.text\n"
        "\t.globl\tmain\n"
        "\t.type\tmain, @function\n"
        "main:\n");
}
void print_global_variable_info(char *identifier, int variable_pos)
{
    printf("\t.globl\t%s\n", identifier);

    if (variable_pos == 0)
    {
        printf("\t.data\n");
    }

    printf(
        "\t.align 4\n"
        "\t.type\t%s, @object\n"
        "\t.size\t%s, 4\n"
        "%s:\n",
        identifier, identifier, identifier);
}

void print_global_variables_info(iloc_program_t *program, symbol_table_t *global_table)
{
    symbol_t *current_symbol;
    int variable_pos = 0, table_pos = 0;

    while (variable_pos < global_table->symbol_count)
    {
        current_symbol = global_table->symbols[table_pos];
        if (current_symbol != NULL)
        {
            if (current_symbol->nature == IDENTIFIER_NATURE)
            {
                print_global_variable_info(current_symbol->lex_data->lexeme, variable_pos);
            }
            variable_pos++;
        }
        table_pos++;
    }
}

void print_data_segment(iloc_program_t *program, symbol_table_t *global_table)
{
    print_global_variables_info(program, global_table);
    print_main_function_info();
}

void print_asm(iloc_program_t *program, symbol_table_t *global_table)
{
    print_data_segment(program, global_table);
    print_code_segment(program, global_table);
}

void print_code_segment(iloc_program_t *program, symbol_table_t *global_table)
{
    iloc_instruction_t instruction;
    char *label, *scope, *offset;

    for (int pos = 0; pos < program->length; pos++)
    {
        instruction = program->instructions[pos];
        if (instruction.op_code != NULL &&
            (strcmp(instruction.op_code, "loadAI") == 0 ||
             strcmp(instruction.op_code, "storeAI") == 0))
        {
            if (strcmp(instruction.op_code, "loadAI") == 0)
            {
                scope = instruction.operand_1;
                offset = instruction.operand_2;
            }
            else
            {
                scope = instruction.operand_2;
                offset = instruction.operand_3;
            }
            label = symbol_table_get_identifier_label_from_offset_or_null(global_table, (unsigned int)atoi(offset));
            if (label != NULL)
            {
                printf("Global variable %s used in %s instruction with %s offset\n", label, instruction.op_code, offset);
            }
        }
    }
}