#ifndef TOKEN_H
#define TOKEN_H

// The tokens which form our grammar are defined in this struct
typedef struct TOKEN {
    // We enumerate the different tokens as different types
    enum {
        // Keywords
        TOKEN_ID,

        // Data types
        TOKEN_INT,
        TOKEN_STRING,

        // Symbols
        TOKEN_EQUALS,
        TOKEN_LESSTHAN,
        TOKEN_GREATERTHAN,
        TOKEN_COMMA,
        TOKEN_COLON,
        TOKEN_SEMICOLON,

        // Grouping characters
        TOKEN_OPENCURLYBRACKET,
        TOKEN_CLOSECURLYBRACKET,
        TOKEN_OPENBRACKET,
        TOKEN_CLOSEBRACKET,
        TOKEN_OPENPARENTHESIS,
        TOKEN_CLOSEPARENTHESIS,
        
        // Special
        TOKEN_NEWLINE,
        TOKEN_EOF
    } type;

    // The value of that token when being lexed is stored as a string
    char* value;
} token_t;

token_t* init_token(int type, char* value);
char* token_to_string(token_t* token);

#endif