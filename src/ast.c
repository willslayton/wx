#include <stdio.h>
#include <stdlib.h>
#include "ast.h"

// Initialization function for our AST node
ast_t* init_ast(int type) {
    ast_t* ast = calloc(1, sizeof(struct AST));

    ast->type = type;

    // Our compound nodes are the only ones which need to have children
    // I wonder if I could use this for binops to save on memory
    if(type == AST_COMPOUND) {
        ast->children = init_list(sizeof(struct AST*));
    }

    ast->multiplier = 1;
    ast->dtype = DATA_TYPE_UNKNOWN;

    return ast;
}

// Function to illustrate the AST structure through printf
void ast_traverse(ast_t* node, int level) {

    // Starting level 0 for root
    if(level == 0) {
        printf("\n== PROGRAM AST ==\n");
    } else {
        // Print tree-like characters to show level structure
        for(int i = 0; i < (level - 1); i++) {
            if(i == 0) {
                printf(" |-");
            } else {
                printf("---");
            }
        }

        // Enumerated node type is printed, denoted by [type]
        printf("[%d]", node->type);
        if(node->value) {
            printf(":{%d}\n", node->value->type);
        } else {
            printf("\n");
        }
    }

    // Check for compound node, since these are the only nodes with children
    // Without this check we somehow fall into infinite recursion (although it shoudln't?)
    if(node->type == AST_COMPOUND || node->type == AST_FUNCTION) {
        // For each of the children recurse on this function
        for(int i = 0; i < node->children->size; i++) {
            ast_traverse(node->children->items[i], level + 1);
        }
    }

}
