#ifndef WXC_H
#define WXC_H

char* wxcompile(char* source);
void wxfcompile(char* input, char* output);
char* read_file(const char* filename);
void write_file(const char* filename, char* buffer);

#if defined(_WIN64) || defined(_WIN32)

    #define OS "WIN"
    #define COPY_COMMAND_PORTION "copy temp.exe "
    #define CLEAN_COMMAND "del temp.s temp.o temp.exe"

    #if defined(_WIN64)
        #define COMPILE_COMMAND "nasm -f win64 temp.s -o temp.o"

    #elif defined(_WIN32)
        #define COMPILE_COMMAND "nasm -f win32 temp.s -o temp.o"

    #endif

#else //defined(unix) || defined(__unix__) || defined(__unix)
    #define OS "UNIX"
    #define COMPILE_COMMAND "nasm -f elf64 temp.s -o temp.o"
    #define COPY_COMMAND_PORTION "cp temp.exe "
    #define CLEAN_COMMAND "rm temp.s temp.o temp.exe"

#endif

#endif
