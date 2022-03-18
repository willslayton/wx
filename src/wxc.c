#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <string.h>
#include "wxc.h"
#include "lexer.h"
#include "parser.h"
#include "asmf.h"

int main(int argc, char* argv[]) {
    if(argc < 2) {
        printf("Please specify input file");
        return 1;
    }
    printf("Starting compilation process...\n");
    
    wxfcompile(argv[1]);
    printf("Done!\n");
    return 0;
}

// Function: wx compile
void wxcompile(char* source) {
    lexer_t* lexer = init_lexer(source);

    printf("Lexing...\n");
    parser_t* parser = init_parser(lexer);

    printf("Parsing...\n");
    ast_t* root = parser_parse(parser);

    ast_traverse(root, 0);
    printf("\n");

    printf("Assembling...\n");
    char* assembly = asm_f_start(root);

    printf("\n%s\n", assembly);

}

// Function: wx file compile
void wxfcompile(char* filename) {
    printf("Reading from file...\n");
    char* source = read_file(filename);
    wxcompile(source);
    printf("Freeing allocated resources...\n");
    free(source);
}

// Helper function to read contents of file as string
// This is probably absolutely horrible for memory but oh well
char* read_file(const char* filename) {
    FILE* file;
    char* line = NULL;
    size_t length = 0;
    ssize_t read;

    file = fopen(filename, "rb");
    if(file == NULL) {
        printf("File could not be opened: %s\n", filename);
        exit(1);
    }

    char* buffer = (char*)calloc(1, sizeof(char));
    buffer[0] = '\0';

    while((read = getline(&line, &length, file)) != -1) {
        buffer = (char*)realloc(buffer, (strlen(buffer) + strlen(line) + 1) * sizeof(char));
        strcat(buffer, line);
    }

    fclose(file);
    if(line) {
        free(line);
    }

    return buffer;
}