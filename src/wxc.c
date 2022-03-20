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
    
    wxfcompile(argv[1], "");
    printf("Done!\n");
    return 0;
}

static char* sh(char* cmd) {
    char* output = calloc(1, sizeof(char));
    output[0] = '\0';

    FILE *file;
    char path[1024];

    file = popen(cmd, "r");

    if(file == NULL) {
        printf("Failed to run command.\n");
        exit(1);
    }

    while(fgets(path, sizeof(path), file) != NULL) {
        output = realloc(output, (strlen(output) + strlen(path) + 1) * sizeof(char));
        strcat(output, path);
    }

    pclose(file);
    free(cmd);

    return output;
}

// Function: wx compile
char* wxcompile(char* source) {
    

    lexer_t* lexer = init_lexer(source);

    printf("Lexing...\n");
    parser_t* parser = init_parser(lexer);

    printf("Parsing...\n");
    ast_t* root = parser_parse(parser);

    // Test code for AST correctness
    //ast_traverse(root, 0);
    //printf("\n");

    printf("Assembling...\n");
    
    return asm_f_start(root);

}

// Function: wx file compile
void wxfcompile(char* input, char* output) {
    if(!strlen(output)) {
        output = calloc(strlen(input), sizeof(char));
        strncpy(output, input, strlen(input) - 2);
        strcat(output, "s");
    }
    
    printf("Reading from file...\n");
    char* source = read_file(input);
    char* assembly = wxcompile(source);
    sh("mkdir temp");
    write_file("temp/a.s", assembly);
    sh("nasm -f win32 temp/a.s -o temp/a.o");
    sh("gcc temp/a.o -o a.exe");
    //sh("rmdir temp");

    printf("Freeing allocated resources...\n");
    free(source);
    free(assembly);
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

void write_file(const char* filename, char* buffer) {
    FILE* file;

    file = fopen(filename, "wb");
    if(file == NULL) {
        printf("File could not be opened: %s\n", filename);
        exit(1);
    }

    //fwrite(buffer, sizeof(char), strlen(buffer), file);
    fputs(buffer, file);

    fclose(file);
}