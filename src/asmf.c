#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "asmf.h"
#include "ast.h"

// Function to start building assembly file
char* asm_f_start(ast_t* node) {
    
    // This const will hold the template for the main function call
    const char* section = "global _WinMain@16\nsection .text\n_WinMain@16:\n%s\nmov ebx, eax\nmov eax, 1\nret\n\n";

    char* value = calloc((strlen(section) + 1), sizeof(char));
    strcpy(value, section);

    // next holds the string of recusive descent in main while functions holds any function definitons 
    char* next = asm_f(node);
    char* functions = asm_f_find_function(node);
    value = realloc(value, strlen(value) + strlen(next) + strlen(functions) + 1 * sizeof(char));

    // We have to concatenate them like this since we cannot have nested function definitions
    sprintf(value, section, next);
    strcat(value, functions);

    return value;
}

// Function for general switching on node type
char* asm_f(ast_t* node) {
    char* value = calloc(1, sizeof(char));
    char* next = calloc(1, sizeof(char));
    
    //printf("[%d]\n", node->type);
    
    switch(node->type) {
        case AST_COMPOUND:
            next = asm_f_compound(node);
            break;
        case AST_ASSIGNMENT:
            next = asm_f_assignment(node);
            break;
        case AST_VARIABLE:
            next = asm_f_variable(node);
            break;
        case AST_FUNCTION:
            break;
        case AST_CALL:
            next = asm_f_call(node);
            break;
        case AST_INT:
            next = asm_f_int(node);
            break;
        case AST_STRING:
            next = asm_f_string(node);
            break;
        case AST_STATEMENT_RETURN:
            next = asm_f_statement(node);
        case AST_NOP:
            break;
        default:
            printf("Assembler cannot take abstract syntax node of type %d\n", node->type);
            exit(1);
    }

    value = realloc(value, (strlen(next) + strlen(value) + 1) * sizeof(char));

    strcat(value, next);
    return value;
}

// Function to create function definitions
char* asm_f_find_function(ast_t* node) {
    char* value = calloc(1, sizeof(char));
    char* next = calloc(1, sizeof(char));
    
    // We only want to find function definitons nodes, so we recurse until there is a type match
    switch(node->type) {
        case AST_COMPOUND:
            if(node->children->size > 0) {
                for(int i = 0; i < node->children->size; i++) {
                    next = asm_f_find_function(node->children->items[i]);
                }
            }
            break;
        case AST_FUNCTION:
            next = asm_f_function(node);
            break;
        default:
            break;
    }

    value = realloc(value, (strlen(next) + strlen(value) + 1) * sizeof(char));

    strcat(value, next);

    return value;
}

char* asm_f_compound(ast_t* node) {
    char* value = calloc(1, sizeof(char));

    if(node->children->size > 0) {
        for(int i = 0; i < node->children->size; i++) {

            char* next = asm_f(node->children->items[i]);
            value = realloc(value, (strlen(next) + 1) * sizeof(char));
            strcat(value, next);
        }
    }

    return value;
}

char* asm_f_assignment(ast_t* node) {

    char* value = calloc(1, sizeof(char));

    /*if(node->value->type == AST_FUNCTION) {
        ast_t* assignment_value = node->value;
        char* assignment_value_value = asm_f(assignment_value->value);

        value = realloc(value, (strlen(value) + strlen(assignment_value_value) + 1) * sizeof(char));
        strcat(value, assignment_value_value);
    }*/

    return value;
}

char* asm_f_variable(ast_t* node) {
    return "";
}

char* asm_f_function(ast_t* node) {

    char* value = calloc(1, sizeof(char));

    const char* temp = "global %s\n%s:\n";

    value = realloc(value, (strlen(temp) + strlen(node->name) * 2 + 1) * sizeof(char));
    sprintf(value, temp, node->name, node->name);
    
    char* node_string = asm_f_compound(node);

    value = realloc(value, (strlen(value) + strlen(node_string) + 1) * sizeof(char));
    strcat(value, node_string);

    return value;
}

char* asm_f_call(ast_t* node) {
    char* s = calloc(1, sizeof(char));

    char* final_prefix = calloc(0, sizeof(char));

    for(int i = 0; i < node->value->children->size; i++) {
        ast_t* arg = node->value->children->items[i];
        char* arg_string = asm_f(arg);

        s = realloc(s, (strlen(s) + strlen(arg_string) + 1) * sizeof(char));
        strcat(s, arg_string);
    }
    /*for(int i = 0; i < node->value->children->size; i++) {
        const char* temp = "# call arg\npushl %d(\%ebp)\n";
        char* push = calloc(strlen(temp) * sizeof(int), sizeof(char));

        sprintf(push, temp, (node->))
    }*/

    const char* temp = "call %s\nmov eax, ebx\n";


    printf("\n\n%d\n\n", node->value->children->size);

    
}

char* asm_f_statement(ast_t* node) {
    char* s = calloc(1, sizeof(char));

    if(strcmp(node->name, "return") == 0) {
        //const char* temp = "%s\njmp return_statement\n";
        const char* temp = "%s\nret\n";

        char* value = asm_f(node->value);
        char* return_string = calloc(strlen(temp) + strlen(value) + sizeof(int), sizeof(char));

        sprintf(return_string, temp, value);
        s = realloc(s, (strlen(return_string) + 1) * sizeof(char));
        strcpy(s, return_string);
    }

    return s;
}

char* asm_f_int(ast_t* node) {
    return "";
}

char* asm_f_string(ast_t* node) {
    return "";
}
