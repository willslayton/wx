#ifndef AST_H
#define AST_H
#include <stdlib.h>
#include "list.h"
#include "types.h"

// To parse the file an abstract syntax tree needs to be build
typedef struct AST {
    enum {
        // Special
        AST_COMPOUND,
        AST_NOP,
        AST_BINOP,
        AST_ACCESS,
        AST_STATEMENT_RETURN,

        // Assignment or definition
        AST_DEFINITION_TYPE,
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
    
    // This group is used for binops! Redundant! I don't know how else to do it!
    struct AST* left;
    struct AST* right;
    int id;
    int op;

    int stack_index;
    int multiplier;
    data_type dtype;
    struct AST* value;

    // These literals for the values should ONLY be temporary... but I don't know how to properly implement
    int int_value;
    char* string_value;

} ast_t;

// init function for our AST nodes
ast_t* init_ast(int type);
void ast_traverse(ast_t* node, int level);

#endif