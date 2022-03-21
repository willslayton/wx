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

#if defined(_WIN64) || defined(_WIN32)

    #define ASM_MAIN_SECTION "global _WinMain@16\nsection .text\n_WinMain@16:\n%s\nmov ebx, eax\nmov eax, 1\nret\n\n"

#else //defined(unix) || defined(__unix__) || defined(__unix)

    #define ASM_MAIN_SECTION "section .text\nglobal _start\n_start:\n%s\nmov ebx, eax\nmov eax, 1\nint 0x80\n\n"

#endif

#endif