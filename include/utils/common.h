#ifndef COMMON_H
#define COMMON_H
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <return.h>

int exec(const char* cmd);
int exec_with_log(const char* cmd);
void clean_stdin(void);
void mkdir_(char* pathname, int mode);
FILE* mkfile_(char* pathname, int mode);
int is_end_of_archive(const char *p);
int parse_octal(const char* p, size_t n);
char* project_directory(char* root, char* path);
char* str_replace(char *orig, char *rep, char *with);

#endif