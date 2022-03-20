#ifndef ASMF_H
#define ASMF_H
#include "ast.h"

char* asm_f(ast_t* node);
char* asm_f_find_function(ast_t* node);
char* asm_f_start(ast_t* node);
char* asm_f_compound(ast_t* node);
char* asm_f_assignment(ast_t* node);
char* asm_f_variable(ast_t* node);
char* asm_f_function(ast_t* node);
char* asm_f_call(ast_t* node);
char* asm_f_statement(ast_t* node);
char* asm_f_int(ast_t* node);
char* asm_f_string(ast_t* node);

#endif