#ifndef LEXER_H
#define LEXER_H
#include "token.h"

// This struct is the basis of our lexer
typedef struct LEXER {
    char* source;
    int source_size;

    // c is the current char and i is the current index in the source
    char c;
    unsigned int i;

} lexer_t;

lexer_t* init_lexer(char* source);
void lexer_advance(lexer_t* lexer);
char lexer_peek(lexer_t* lexer, int offset);
void lexer_skip_whitespace(lexer_t* lexer);
void lexer_skip_line(lexer_t* lexer);
token_t* lexer_next_token(lexer_t* lexer);
token_t* lexer_parse_string(lexer_t* lexer);
token_t* lexer_parse_number(lexer_t* lexer);
token_t* lexer_parse_id(lexer_t* lexer);
token_t* lexer_advance_with_current(lexer_t* lexer, int type);

#endif