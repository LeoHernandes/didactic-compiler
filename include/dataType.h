/*
 * Represents the possible data types for each recognized symbol
 */
enum data_type
{
    BOOL,
    INT,
    FLOAT,

    /*
     * Currently represents unwanted states in semantic analysis
     * or missing implementations that require the symbols table (TODO)
     * NOTE: This should be an unnused type at the end of E4 so we can remove it
     */
    UNKNOWN,
};
typedef enum data_type data_type_t;