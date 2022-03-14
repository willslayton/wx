#ifndef AST_H
#define AST_H
#include <stdlib.h>
#include "list.h"

// To parse the file an abstract syntax tree needs to be build
// This struct implements an AST which we can use with the parser
typedef struct AST {
    // The types of syntax expressions are enumerated
    enum {
        // Special
        AST_COMPOUND,
        AST_NOP,

        // Assignment or definition
        AST_ASSIGNMENT,
        AST_FUNCTION,
        
        // Calls
        AST_VARIABLE,
        AST_CALL,

        // Data types
        AST_STRING,
        AST_INT,

    } type;

    // For each type we assign appropriate members, some of which building our recursive tree structure
    list_t* children;
    char* name;
    struct AST* value;
    int int_value;
    char* string_value;
    int data_type;

} ast_t;

// init function for our AST nodes
ast_t* init_ast(int type);
void ast_traverse(ast_t* node, int level);

#endif