#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#include "token.h"
#include "parser.h"

// printf("\n%d:%s\n", parser->token->type, parser->token->value);

// Initialization function for the parser
parser_t* init_parser(lexer_t* lexer) {

    // Allocate space for the parser data structure
    parser_t* parser = calloc(1, sizeof(struct PARSER));

    // Set members of struct to passed value
    parser->lexer = lexer;
    parser->token = lexer_next_token(lexer);

    // Return our created parser
    return parser;
}

// Function to advance to next token
token_t* parser_advance(parser_t* parser, int token_type) {

    // Check to see if the current token type is the same as the one passed
    if(parser->token->type != token_type) {
        // If the token is incorrect, respond with an unexpected token error
        printf("Unexpected token[%d]: \"%s\", expected %d", parser->token->type, parser->token->value, token_type);
        exit(1);
    }

    // Save the current token and parse the next token retrieved by the lexer
    parser->token = lexer_next_token(parser->lexer);

    return parser->token;
}

// Helper function to parse statements
ast_t* parser_parse(parser_t* parser) {
    return parser_parse_compound(parser);
}

// Function to parse multiple statements
ast_t* parser_parse_compound(parser_t* parser) {
    bool isClosed = true;

    if(parser->token->type == TOKEN_EOF) {
        return init_ast(AST_NOP);
    }

    // For compound statements we init using a compound node
    ast_t* compound = init_ast(AST_COMPOUND);

    if(parser->token->type == TOKEN_OPENCURLYBRACKET) {
        compound->name = parser->token->value;
        parser_advance(parser, TOKEN_OPENCURLYBRACKET);
        isClosed = false;
    }

    while((parser->token->type != TOKEN_EOF)&&(parser->token->type != TOKEN_CLOSECURLYBRACKET)) {
        if(parser->token->type == TOKEN_NEWLINE) {
            parser_advance(parser, TOKEN_NEWLINE);
            
        } else {
            list_push(compound->children, parser_parse_expression(parser));
        }
    }

    if(!isClosed) {
        parser_advance(parser, TOKEN_CLOSECURLYBRACKET);
    }

    // Return the compound statement node
    return compound;
}

// Function to parse ids
ast_t* parser_parse_id(parser_t* parser) {

    char* value = calloc(strlen(parser->token->value) + 1, sizeof(char));
    strcpy(value, parser->token->value);

    parser_advance(parser, TOKEN_ID);

    printf("type-%d\n", parser->token->type);
    ast_t* ast = init_ast(AST_VARIABLE);
    ast->name = value;

    if(parser->token->type == TOKEN_EQUALS) {
        // Parsing assignment
        parser_advance(parser, TOKEN_EQUALS);
        ast->type = AST_ASSIGNMENT;
        ast->value = parser_parse_expression(parser);
    } else if(parser->token->type == TOKEN_OPENPARENTHESIS) {
        // Parsing call
        ast->type = AST_CALL;
        ast->value = parser_parse_list(parser);
    }
    
    return ast;
}

// Function to parse expressions
ast_t* parser_parse_expression(parser_t* parser) {
    printf("%s", token_to_string(parser->token));

    // Switch on the type of token recieved
    switch(parser->token->type) {
        case TOKEN_INT:
            return parser_parse_int(parser);
        case TOKEN_STRING:
            return parser_parse_string(parser);
        case TOKEN_ID:
            return parser_parse_id(parser);
        case TOKEN_OPENPARENTHESIS:
            return parser_parse_list(parser);
        case TOKEN_OPENBRACKET:
            return parser_parse_list(parser);
        case TOKEN_OPENCURLYBRACKET:
            return parser_parse_compound(parser);
            //return parser_parse_function(parser, init_ast(AST_FUNCTION));
        default:
            printf("Unexpected token[%d]: \"%s\"", parser->token->type, parser->token->value);
            exit(1);
    }

    return init_ast(AST_NOP);
}

// Function to parse lists within parenthesis
ast_t* parser_parse_list(parser_t* parser) {

    // For compound statements we init using a compound node
    ast_t* node = init_ast(AST_COMPOUND);

    bool isArray = false;

    if(parser->token->type == TOKEN_OPENPARENTHESIS) {
        parser_advance(parser, TOKEN_OPENPARENTHESIS);
    } else {
        parser_advance(parser, TOKEN_OPENBRACKET);
        isArray = true;
    }

    while((parser->token->type != TOKEN_CLOSEPARENTHESIS)&&(parser->token->type != TOKEN_CLOSEBRACKET)) {
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
    node->string_value = parser->token->value;
    parser_advance(parser, TOKEN_STRING);

    return node;
}

// Function to parse integers
ast_t* parser_parse_int(parser_t* parser) {

    ast_t* node = init_ast(AST_INT);
    node->int_value = atoi(parser->token->value);
    parser_advance(parser, TOKEN_INT);

    return node;
}
