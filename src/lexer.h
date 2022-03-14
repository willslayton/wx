#ifndef LEXER_H
#define LEXER_H
#include "token.h"

// This struct is the basis of our lexer
typedef struct LEXER {
    // source is the string of the program file
    char* source;
    // source_size is the length of the contents
    int source_size;
    // c is the current character
    char c;
    // i is the current index in the contents
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