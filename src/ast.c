#include <stdio.h>
#include <stdlib.h>
#include "ast.h"

// Initialization function for our AST node
ast_t* init_ast(int type) {
    // Allocate space for the AST data structure
    ast_t* ast = calloc(1, sizeof(struct AST));

    // Set members of struct to passed value
    ast->type = type;

    if(type == AST_COMPOUND) {
        ast->children = init_list(sizeof(struct AST*));
    }

    // Return our created AST node
    return ast;
}

// Function to illustrate the AST structure through printf
void ast_traverse(ast_t* node, int level) {

    // Starting level 0 for root
    if(level == 0) {
        printf("\n== PROGRAM AST ==\n");
    }

    // Print tree-like characters to show level structure
    for(int i = 0; i < level; i++) {
        if(i == 0) {
            printf(" |-");
        } else {
            printf("---");
        }
    }

    // Enumerated node type is printed, denoted by [type]
    printf("[%d]\n", node->type);

    // Check for compound node, since these are the only nodes with children
    // Without this check we fall into infinite recursion (although it may not seem like it)
    if(node->type == AST_COMPOUND) {
        // For each of the children recurse on this function
        for(int i = 0; i < node->children->size; i++) {
            ast_traverse(node->children->items[i], level + 1);
        }
    }

}
