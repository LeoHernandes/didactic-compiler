#include "asm.h"

/* ================================== Data Segment ================================== */

void _print_main_function_info()
{
    printf(
        "\t.text\n"
        "\t.globl\tmain\n"
        "\t.type\tmain, @function\n"
        "main:\n"
        "\tpushq\t%%rbp\n"
        "\tmovq\t%%rsp, %%rbp\n");
}

void _print_global_variable_info(char *identifier)
{
    printf(
        "\t.globl\t%s\n"
        "\t.align 4\n"
        "\t.type\t%s, @object\n"
        "\t.size\t%s, 4\n"
        "%s:\n"
        "\t.zero\t4\n", // <== Initialize the 4 byte space with zeros for the variable
        identifier, identifier, identifier, identifier);
}

void _print_pseudo_register_info(int register_number)
{
    printf(
        "\t.globl\t__register_temp_r%d\n"
        "\t.align 4\n"
        "\t.type\t__register_temp_r%d, @object\n"
        "\t.size\t__register_temp_r%d, 4\n"
        "__register_temp_r%d:\n"
        "\t.zero\t4\n", // <== Initialize the 4 byte space with zeros for the variable
        register_number, register_number, register_number, register_number);
}

void _print_global_variables_info(symbol_table_t *global_table)
{
    symbol_t *current_symbol;
    int table_pos = 0;

    while (table_pos < global_table->symbol_count)
    {
        current_symbol = global_table->symbols[table_pos];
        if (current_symbol != NULL)
        {
            if (current_symbol->nature == IDENTIFIER_NATURE)
            {
                _print_global_variable_info(current_symbol->lex_data->lexeme);
            }
        }
        table_pos++;
    }
}

void _print_pseudo_registers(int num_registers)
{
    for (int i = 0; i < num_registers; i++)
    {
        _print_pseudo_register_info(i);
    }
}

void _print_data_segment(iloc_program_t *program, symbol_table_t *global_table)
{
    int num_registers = get_iloc_temp_register_quantity();

    if (global_table->symbol_count > 0 || num_registers > 0)
    {
        printf("\t.bss\n"); // <== Block Starting Symbol: portion of an asm code that contains statically
                            //     allocated variables that have not been assigned a value yet
    }

    _print_global_variables_info(global_table);
    _print_pseudo_registers(num_registers);
    _print_main_function_info();
}

/* ================================== Code Segment ================================== */

void _print_instruction(iloc_instruction_t instruction, symbol_table_t *global_table)
{
    switch (instruction.op_code)
    {
    case ADD:
    {
        char *left_op = instruction.operand_1;
        char *right_op = instruction.operand_2;
        char *temp_register = instruction.operand_3;
        printf("\tmovl    __register_temp_%s(%%rip), %%edx\n", left_op);
        printf("\tmovl    __register_temp_%s(%%rip), %%eax\n", right_op);
        printf("\taddl    %%edx, %%eax\n");
        printf("\tmovl    %%eax, __register_temp_%s(%%rip)\n", temp_register);
        break;
    }

    case AND:
    {
        char *left_op = instruction.operand_1;
        char *right_op = instruction.operand_2;
        char *temp_register = instruction.operand_3;
        printf("\tmovl    __register_temp_%s(%%rip), %%edx\n", left_op);
        printf("\tmovl    __register_temp_%s(%%rip), %%eax\n", right_op);
        printf("\tand     %%edx, %%eax\n");
        printf("\tmovl    %%eax, __register_temp_%s(%%rip)\n", temp_register);
        break;
    }

    case CBR:
        break;

    case CMP_EQ:
        break;

    case CMP_GE:
        break;

    case CMP_GT:
        break;

    case CMP_LE:
        break;

    case CMP_LT:
        break;

    case CMP_NE:
        break;

    case DIV:
        break;

    case JUMPI:
        break;
    case LOADAI:
    {
        char *scope = instruction.operand_1;
        char *offset = instruction.operand_2;
        char *temp_register = instruction.operand_3;
        if (strcmp(scope, "rbss") == 0)
        {
            char *label = symbol_table_get_identifier_label_from_offset_or_null(global_table, (unsigned int)atoi(offset));
            printf("\tmovl    %s(%%rip), %%edx\n", label);
            printf("\tmovl    %%edx, __register_temp_%s(%%rip)\n", temp_register);
        }
        else
        {
            printf("\tmovl    -%s(%%rbp), %%edx\n", offset);
            printf("\tmovl    %%edx, __register_temp_%s(%%rip)\n", temp_register);
        }
        break;
    }
    case LOADI:
    {
        char *immediate_value = instruction.operand_1;
        char *temp_register = instruction.operand_3;
        printf("\tmovl    $%s, %%edx\n", immediate_value);
        printf("\tmovl    %%edx, __register_temp_%s(%%rip)\n", temp_register);
        break;
    }
    case MULT:
        break;

    case MULTI:
        break;

    case NOP:
        break;

    case OR:
    {
        char *left_op = instruction.operand_1;
        char *right_op = instruction.operand_2;
        char *temp_register = instruction.operand_3;
        printf("\tmovl    __register_temp_%s(%%rip), %%edx\n", left_op);
        printf("\tmovl    __register_temp_%s(%%rip), %%eax\n", right_op);
        printf("\tor      %%edx, %%eax\n");
        printf("\tmovl    %%eax, __register_temp_%s(%%rip)\n", temp_register);
        break;
    }

    case RET:
    {
        char *return_temp_register = instruction.operand_3;
        printf("\tmovl    __register_temp_%s(%%rip), %%eax\n", return_temp_register);
        printf("\tpopq    %%rbp\n");
        printf("\tret\n");
        break;
    }

    case STOREAI:
    {
        char *temp_register = instruction.operand_1;
        char *scope = instruction.operand_2;
        char *offset = instruction.operand_3;
        if (strcmp(scope, "rbss") == 0)
        {
            char *label = symbol_table_get_identifier_label_from_offset_or_null(global_table, (unsigned int)atoi(offset));
            printf("\tmovl    __register_temp_%s(%%rip), %%edx\n", temp_register);
            printf("\tmovl    %%edx, %s(%%rip)\n", label);
        }
        else
        {
            printf("\tmovl    __register_temp_%s(%%rip), %%edx\n", temp_register);
            printf("\tmovl    %%edx, -%s(%%rbp)\n", offset);
        }
        break;
    }

    case SUB:
        break;
    }
}

void _print_code_segment(iloc_program_t *program, symbol_table_t *global_table)
{
    iloc_instruction_t *instruction = program->instructions;
    char *label, *scope, *offset;

    for (int pos = 0; pos < program->length; pos++)
    {
        _print_instruction(program->instructions[pos], global_table);
    }
}

/* ================================== Main Printing Function ================================== */

void print_asm(iloc_program_t *program, symbol_table_t *global_table)
{
    printf("\t.file\t\"etapa6.c\"\n");
    printf("\t.text\n");

    _print_data_segment(program, global_table);
    _print_code_segment(program, global_table);

    printf("\t.size\tmain, .-main\n");
    printf("\t.section\t.note.GNU-stack,\"\",@progbits\n");
}