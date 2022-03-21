#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "lexer.h"
#include "macros.h"

// Initialization function for our lexer
lexer_t* init_lexer(char* source) {

    lexer_t* lexer = calloc(1, sizeof(struct LEXER));

    lexer->source = source;
    lexer->source_size = strlen(source);
    lexer->i = 0;
    lexer->c = source[lexer->i];

    return lexer;
}

// Function to advance the lexer by one character
void lexer_advance(lexer_t* lexer) {
    // Check if the current character is NULL and EOF
    if((lexer->c != '\0') && (lexer->i < lexer->source_size)) {
        lexer->i += 1;
        lexer->c = lexer->source[lexer->i];
    }
    if((lexer->c == '\0') && (lexer->i == lexer->source_size)) {
        lexer->i += 1;
        // We do this to ensure the final line of the file is taken as a newline and executed
        lexer->c = 10;
    }
}

// Helper function to advance the lexer and return the passed token
token_t* lexer_advance_with_current(lexer_t* lexer, int type) {
    char* value = calloc(2, sizeof(char));
    value[0] = lexer->c;
    value[1] = '\0';

    token_t* token = init_token(type, value);
    lexer_advance(lexer);

    return token;
}

// Helper function to return character at any index
char lexer_peek(lexer_t* lexer, int offset) {
    return lexer->source[MIN(lexer->i + offset, lexer->source_size)];
}

// Function to skip whitespace characters in the source
void lexer_skip_whitespace(lexer_t* lexer) {
    while((lexer->c == ' ')||(lexer->c == '\t')||(lexer->c == 13)) {
        lexer_advance(lexer);
    }
}

// Function to skip every character until a newline, used for comments
void lexer_skip_line(lexer_t* lexer) {
    while(lexer->c != '\n') {
        lexer_advance(lexer);
    }
}

// Function to get the next token in the source
token_t* lexer_next_token(lexer_t* lexer) {
    
    // While the current character is not NULL and is not EOF
    while((lexer->c != '\0') && (lexer->i < strlen(lexer->source))) {

        lexer_skip_whitespace(lexer);

        // If c is numeric, find the id
        if(isdigit(lexer->c)) {
            return lexer_parse_number(lexer);
        }
        // If c is alphanumeric, find the id
        if(isalnum(lexer->c)) {
            return lexer_parse_id(lexer);
        }

        // Otherwise we use a switch statement to select for different tokens
        switch(lexer->c){
            case '#':
                lexer_skip_line(lexer);
                break;
            case '=':
                return lexer_advance_with_current(lexer, TOKEN_EQUALS);
            case '>':
                return lexer_advance_with_current(lexer, TOKEN_GREATERTHAN);
            case '<':
                return lexer_advance_with_current(lexer, TOKEN_LESSTHAN);
            case '+':
                return lexer_advance_with_current(lexer, TOKEN_PLUS);
            case '-':
                return lexer_advance_with_current(lexer, TOKEN_MINUS);
            case '*':
                return lexer_advance_with_current(lexer, TOKEN_ASTERISK);
            case '/':
                return lexer_advance_with_current(lexer, TOKEN_SLASH);
            case 10:
                return lexer_advance_with_current(lexer, TOKEN_NEWLINE);
            case '(':
                return lexer_advance_with_current(lexer, TOKEN_OPENPARENTHESIS);
            case ')':
                return lexer_advance_with_current(lexer, TOKEN_CLOSEPARENTHESIS);
            case '[':
                return lexer_advance_with_current(lexer, TOKEN_OPENBRACKET);
            case ']':
                return lexer_advance_with_current(lexer, TOKEN_CLOSEBRACKET);
            case '{':
                return lexer_advance_with_current(lexer, TOKEN_OPENCURLYBRACKET);
            case '}':
                return lexer_advance_with_current(lexer, TOKEN_CLOSECURLYBRACKET);
            case ',':
                return lexer_advance_with_current(lexer, TOKEN_COMMA);
            case ':':
                return lexer_advance_with_current(lexer, TOKEN_COLON);
            case '"':
                return lexer_parse_string(lexer);
            case '\0':
                printf("lexEOF\n");
                return init_token(TOKEN_EOF, 0);
            default:
                printf("Unexpected character: %c\n", lexer->c);
                exit(1);
                break;
        }
    }
    
    return init_token(TOKEN_EOF, "\0");
}

// Function to get the next string from the source
token_t* lexer_parse_string(lexer_t* lexer) {
    
    // Eat opening quotation
    lexer_advance(lexer);

    char* value = calloc(1, sizeof(char));
    value[0] = '\0';

    // While the current character is not the closing quotation mark
    while(lexer->c != '"') {
        // We can then reallocate space for value and concatenate the string and character together
        value = realloc(value, (strlen(value) + 2) * sizeof(char));
        strcat(value, (char[]){lexer->c, 0});

        lexer_advance(lexer);
    }

    // Eat closing quotation
    lexer_advance(lexer);

    return init_token(TOKEN_STRING, value);
}

// Function to get the next integer from the source
token_t* lexer_parse_number(lexer_t* lexer) {

    char* value = calloc(1, sizeof(char));
    value[0] = '\0';

    while(isdigit(lexer->c)) {
        value = realloc(value, (strlen(value) + 2) * sizeof(char));
        strcat(value, (char[]){lexer->c, 0});

        lexer_advance(lexer);
    }

    return init_token(TOKEN_INT, value);
}

// Function to get the next id from the source
token_t* lexer_parse_id(lexer_t* lexer) {
    int token_type = TOKEN_ID;

    char* value = calloc(1, sizeof(char));
    value[0] = '\0';

    while(isalnum(lexer->c)) {
        value = realloc(value, (strlen(value) + 2) * sizeof(char));
        strcat(value, (char[]){lexer->c, 0});

        lexer_advance(lexer);
    }

    if(strcmp(value, "return") == 0 || strcmp(value, "if") == 0 || strcmp(value, "else") == 0 || strcmp(value, "for") == 0 || strcmp(value, "while") == 0) {
        token_type = TOKEN_STATEMENT;
    }

    return init_token(token_type, value);
}