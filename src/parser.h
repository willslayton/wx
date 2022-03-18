#ifndef PARSER_H
#define PARSER_H
#include "lexer.h"
#include "ast.h"

// The members of our parser struct should include a lexer and current token
typedef struct PARSER {
    lexer_t* lexer;
    token_t* token;
} parser_t;

parser_t* init_parser(lexer_t* lexer);
token_t* parser_advance(parser_t* parser, int token_type);
ast_t* parser_parse(parser_t* parser);
ast_t* parser_parse_compound(parser_t* parser);
ast_t* parser_parse_id(parser_t* parser);
ast_t* parser_parse_factor(parser_t* parser);
ast_t* parser_parse_term(parser_t* parser);
ast_t* parser_parse_expression(parser_t* parser);
ast_t* parser_parse_statement(parser_t* parser);
ast_t* parser_parse_list(parser_t* parser);
ast_t* parser_parse_function(parser_t* parser);
ast_t* parser_parse_string(parser_t* parser);
ast_t* parser_parse_int(parser_t* parser);

#endif