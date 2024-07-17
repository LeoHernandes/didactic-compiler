#include "dataType.h"

data_type_t infer_type(data_type_t type_a, data_type_t type_b)
{
    if (type_a == INT && type_b == INT)
        return INT;

    if (type_a == FLOAT && type_b == FLOAT)
        return FLOAT;

    if (type_a == BOOL && type_b == BOOL)
        return BOOL;

    if (type_a == FLOAT && type_b == INT || type_b == FLOAT && type_a == INT)
        return FLOAT;

    if (type_a == BOOL && type_b == INT || type_b == FLOAT && type_a == INT)
        return INT;

    if (type_a == BOOL && type_b == FLOAT || type_b == BOOL && type_a == FLOAT)
        return FLOAT;

    return UNKNOWN;
}