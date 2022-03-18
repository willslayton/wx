#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#include "token.h"
#include "parser.h"

// Initialization function for the parser
parser_t* init_parser(lexer_t* lexer) {

    parser_t* parser = calloc(1, sizeof(struct PARSER));

    parser->lexer = lexer;
    parser->token = lexer_next_token(lexer);

    return parser;
}

// Function to advance to next token
token_t* parser_advance(parser_t* parser, int token_type) {

    // Check to see if the current token type is the same as the one passed
    if(parser->token->type != token_type) {
        printf("Unexpected token[%d]: \"%s\", expected %d", parser->token->type, parser->token->value, token_type);
        exit(1);
    }

    // Save the current token and parse the next token retrieved by the lexer
    parser->token = lexer_next_token(parser->lexer);

    return parser->token;
}

// This is where the parsing process begins, creation of a root AST node
ast_t* parser_parse(parser_t* parser) {
    return parser_parse_compound(parser);
}

// Function to parse compound blocks of code
ast_t* parser_parse_compound(parser_t* parser) {
    bool isFunction = false;

    if(parser->token->type == TOKEN_EOF) {
        return init_ast(AST_NOP);
    }

    // For compound statements we init using a compound node
    ast_t* compound = init_ast(AST_COMPOUND);

    // This case should only occur if there is a function's code block
    if(parser->token->type == TOKEN_OPENCURLYBRACKET) {
        compound->name = parser->token->value;
        parser_advance(parser, TOKEN_OPENCURLYBRACKET);
        compound->type = AST_FUNCTION;
        isFunction = true;
    }

    while((parser->token->type != TOKEN_EOF)&&(parser->token->type != TOKEN_CLOSECURLYBRACKET)) {
        // Eat any redundant newlines
        if(parser->token->type == TOKEN_NEWLINE) {
            parser_advance(parser, TOKEN_NEWLINE);
            
        // Push any expressions as children of this compound node
        } else if(parser->token->type == TOKEN_STATEMENT) {
            list_push(compound->children, parser_parse_statement(parser));
        } else {
            list_push(compound->children, parser_parse_expression(parser));
        }
    }

    if(isFunction) {
        parser_advance(parser, TOKEN_CLOSECURLYBRACKET);
    }

    return compound;
}

// Function to parse ids
ast_t* parser_parse_id(parser_t* parser) {

    // Store the value of the id
    char* value = calloc(strlen(parser->token->value) + 1, sizeof(char));
    strcpy(value, parser->token->value);

    parser_advance(parser, TOKEN_ID);

    // If the id matches the keyword "function"
    if(strcmp(value, "function") == 0) {
        // Get the value of the next id (function name)
        value = realloc(value, (strlen(parser->token->value) + 1) * sizeof(char));
        strcpy(value, parser->token->value);

        parser_advance(parser, TOKEN_ID);

        // We can get the arguments as a compound node and use this as the value
        ast_t* arg_list = parser_parse_list(parser);

        // The main node has the code of the function as the list children
        ast_t* node = parser_parse_compound(parser);
        node->name = value;
        node->value = arg_list;

        return node;
    }

    // Presume the id is a variable name
    ast_t* node = init_ast(AST_VARIABLE);
    node->name = value;

    // Parse assignment when the token "="
    if(parser->token->type == TOKEN_EQUALS) {
        
        parser_advance(parser, TOKEN_EQUALS);
        node->type = AST_ASSIGNMENT;
        node->value = parser_parse_expression(parser);

    // Parse as a call when the token "("
    } else if(parser->token->type == TOKEN_OPENPARENTHESIS) {
        node->type = AST_CALL;
        node->value = parser_parse_list(parser);
    }
    
    return node;
}

// Function to parse factors from a binop
ast_t* parser_parse_factor(parser_t* parser) {
    switch (parser->token->type) {
        case TOKEN_ID: 
            return parser_parse_id(parser);
        case TOKEN_OPENPARENTHESIS:
            return parser_parse_list(parser);
        case TOKEN_INT:
            return parser_parse_int(parser);
        case TOKEN_STRING:
            return parser_parse_string(parser);
        default:
            printf("Unexpected token[%d] in expression: \"%s\"", parser->token->type, parser->token->value);
            exit(1);
    }
}

// Function to parse terms of a binop
ast_t* parser_parse_term(parser_t* parser) {

    ast_t* left = parser_parse_factor(parser);

    while(parser->token->type == TOKEN_ASTERISK || parser->token->type == TOKEN_SLASH) {
        ast_t* binop = init_ast(AST_BINOP);

        binop->left = left;
        binop->op = parser->token->type;

        parser_advance(parser, parser->token->type);

        binop->right = parser_parse_factor(parser);

        return binop;
    }  

    return left;
}

// Function to parse expressions
ast_t* parser_parse_expression(parser_t* parser) {

    // Edge case, no expression
    if(parser->token->type == TOKEN_NEWLINE || parser->token->type == TOKEN_CLOSECURLYBRACKET) {
        ast_t* node = init_ast(AST_NOP);

        node->int_value = 0;

        return node;
    }

    // Otherwise parse as binop
    ast_t* left = parser_parse_term(parser);

    while(parser->token->type == TOKEN_PLUS || parser->token->type == TOKEN_MINUS) {
        ast_t* binop = init_ast(AST_BINOP);

        binop->left = left;
        binop->op = parser->token->type;

        parser_advance(parser, parser->token->type); // It's not safe to accept ANY type, but is only way for it to work for now
        binop->right = parser_parse_expression(parser);

        return binop;
    }

    return left;
}

// Function to parse statements
ast_t* parser_parse_statement(parser_t* parser) {
    ast_t* node = init_ast(AST_STATEMENT_RETURN); // Right now we assume the only statement type is a return

    node->name = parser->token->value;

    parser_advance(parser, TOKEN_STATEMENT);
    node->value = parser_parse_expression(parser);

    return node;
}

// Function to parse lists within parenthesis
ast_t* parser_parse_list(parser_t* parser) {

    // For compound statements we init using a compound node
    ast_t* node = init_ast(AST_COMPOUND);

    // Checking with this bool can help us parse lists or arrays
    bool isArray = false;

    if(parser->token->type == TOKEN_OPENPARENTHESIS) {
        parser_advance(parser, TOKEN_OPENPARENTHESIS);
    } else {
        parser_advance(parser, TOKEN_OPENBRACKET);
        isArray = true;
    }

    while((parser->token->type != TOKEN_CLOSEPARENTHESIS)&&(parser->token->type != TOKEN_CLOSEBRACKET)) {
        // Push to list for each item
        list_push(node->children, parser_parse_expression(parser));

        if(parser->token->type == TOKEN_COMMA) {
            parser_advance(parser, TOKEN_COMMA);
        } else if(parser->token->type == TOKEN_SEMICOLON) {
            parser_advance(parser, TOKEN_SEMICOLON);
        }
    }

    if(isArray) {
        parser_advance(parser, TOKEN_CLOSEBRACKET);
    } else {
        parser_advance(parser, TOKEN_CLOSEPARENTHESIS);
    }

    // Return the compound statement node
    return node;
}

// Function to parse strings
ast_t* parser_parse_string(parser_t* parser) {

    ast_t* node = init_ast(AST_STRING);
    node->string_value = parser->token->value; // We are just using a string primative/literal for now
    parser_advance(parser, TOKEN_STRING);

    return node;
}

// Function to parse integers
ast_t* parser_parse_int(parser_t* parser) {

    ast_t* node = init_ast(AST_INT);
    node->int_value = atoi(parser->token->value); // We are just using an int primative/literal for now
    parser_advance(parser, TOKEN_INT);

    return node;
}
