#ifndef WXC_H
#define WXC_H

char* wxcompile(char* source);
void wxfcompile(char* input, char* output);
char* read_file(const char* filename);
void write_file(const char* filename, char* buffer);

#endif