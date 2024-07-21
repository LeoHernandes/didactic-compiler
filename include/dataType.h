#ifndef _DATATYPE_H_
#define _DATATYPE_H_

/*
 * Represents the possible data types for each recognized symbol
 */
enum data_type
{
    BOOL,
    INT,
    FLOAT,

    /*
     * Data type used only as a temporary value
     * If a searched symbol has an unknown type, there is an error in code analysis
     */
    UNKNOWN,
};
typedef enum data_type data_type_t;

data_type_t infer_type(data_type_t type_a, data_type_t type_b);

#endif