#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "token.h"

// Initialization function for our tokens
token_t* init_token(int type, char* value) {

    token_t* token = calloc(1, sizeof(struct TOKEN));

    token->type = type;
    token->value = value;

    return token;
}

// For simplicity in debugging and error handling this function prints the token type/value
char* token_to_string(token_t* token) {
    const char* temp = "<type=%d, value=`%s`>\n";

    char* string_struct = calloc(strlen(temp) + sizeof(int) * 2, sizeof(char));
    sprintf(string_struct, temp, token->type, token->value);

    return string_struct;
}