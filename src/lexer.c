#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "lexer.h"
#include "macros.h"

// Initialization function for our lexer
lexer_t* init_lexer(char* source) {

    // Allocate space for the lex data structure
    lexer_t* lexer = calloc(1, sizeof(struct LEXER));

    // Set members of struct to passed value
    lexer->source = source;
    lexer->source_size = strlen(source);
    lexer->i = 0;
    lexer->c = source[lexer->i];

    // Return our created lexer
    return lexer;
}

// Function to advance the lexer by one character
void lexer_advance(lexer_t* lexer) {
    // The if statements check if the current character is NULL and EOF
    if((lexer->c != '\0') && (lexer->i < lexer->source_size)) {
        // If not, advance the index by one and get the next character
        lexer->i += 1;
        lexer->c = lexer->source[lexer->i];
    }
    if((lexer->c == '\0') && (lexer->i == lexer->source_size)) {
        // If so, advance the index by one and set the final character to a newline
        lexer->i += 1;
        // We do this to ensure the final line of the file is executed
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

char lexer_peek(lexer_t* lexer, int offset) {
    return lexer->source[MIN(lexer->i + offset, lexer->source_size)];
}

// Function to skip whitespace characters in the source
void lexer_skip_whitespace(lexer_t* lexer) {
    // As long as the current character is not a whitespace, then advance the lexer
    while((lexer->c == ' ')||(lexer->c == '\t')||(lexer->c == 13)) {
        lexer_advance(lexer);
    }
}

// Function to get the next token in the source
token_t* lexer_next_token(lexer_t* lexer) {
    
    // While the current character is not NULL and is not EOF
    while((lexer->c != '\0') && (lexer->i < strlen(lexer->source))) {
        // Skip whitespace
        lexer_skip_whitespace(lexer);

        // If it is alphanumeric, find the id
        if(isalnum(lexer->c)) {
            return lexer_parse_id(lexer);
        }
        // If it is numeric, find the id
        if(isdigit(lexer->c)) {
            return lexer_parse_number(lexer);
        }
        // Otherwise we use a switch statement to select for different tokens
        switch(lexer->c){
            case '=':
                return lexer_advance_with_current(lexer, TOKEN_EQUALS);
            case '>':
                //if(lexer_peek(lexer, 1) == '=')
                return lexer_advance_with_current(lexer, TOKEN_GREATERTHAN);
            case '<':
                //if(lexer_peek(lexer, 1) == '=')
                return lexer_advance_with_current(lexer, TOKEN_LESSTHAN);
            case 10:
                //(char[]){lexer->c, 0});
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
                printf("Unexpected character: %s\n", lexer->c);
                exit(1);
                break;
        }
    }
    // If there is no next token, then we return a void pointer
    return init_token(TOKEN_EOF, "\0");
}

// Function to get the next string from the source
token_t* lexer_parse_string(lexer_t* lexer) {
    // First the lexer is moved forward to pass the opening quotation mark
    lexer_advance(lexer);
    // A string, value, is then created to hold our lexed string characters
    char* value = calloc(1, sizeof(char));
    // We initialize it to a blank string when starting
    value[0] = '\0';

    // While the current character is not the closing quotation mark
    while(lexer->c != '"') {
        // We can then reallocate space for value and concatenate the string and character together
        value = realloc(value, (strlen(value) + 2) * sizeof(char));
        strcat(value, (char[]){lexer->c, 0});
        // Finally advancing the lexer when the character is "read"
        lexer_advance(lexer);
    }
    // If the closing quotation mark is found we advance again
    lexer_advance(lexer);

    // Return a token holding the string value
    return init_token(TOKEN_STRING, value);
}

// Function to get the next integer from the source
token_t* lexer_parse_number(lexer_t* lexer) {
    // A string, value, is then created to hold our lexed id characters
    char* value = calloc(1, sizeof(char));
    // We initialize it to a blank string when starting
    value[0] = '\0';

    // While the current character is numeric
    while(isdigit(lexer->c)) {
        // We can then reallocate space for value and concatenate the string and character together
        value = realloc(value, (strlen(value) + 2) * sizeof(char));
        strcat(value, (char[]){lexer->c, 0});
        // Finally advancing the lexer when the character is "read"
        lexer_advance(lexer);
    }

    // Return a token holding the id value
    return init_token(TOKEN_INT, value);
}

// Function to get the next id from the source
token_t* lexer_parse_id(lexer_t* lexer) {
    // A string, value, is then created to hold our lexed id characters
    char* value = calloc(1, sizeof(char));
    // We initialize it to a blank string when starting
    value[0] = '\0';

    // While the current character is alphanumeric
    while(isalnum(lexer->c)) {
        // We can then reallocate space for value and concatenate the string and character together
        value = realloc(value, (strlen(value) + 2) * sizeof(char));
        strcat(value, (char[]){lexer->c, 0});
        // Finally advancing the lexer when the character is "read"
        lexer_advance(lexer);
    }

    // Return a token holding the id value
    return init_token(TOKEN_ID, value);
}