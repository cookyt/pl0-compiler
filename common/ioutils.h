#ifndef IOUTILS_H_
#define IOUTILS_H_

#include <stdio.h>

extern int _IO_show_lexemes;
extern int _IO_show_assembler;
extern int _IO_show_stack_trace;

void print_file(FILE *fin, FILE *fout);

void parseCommandLineArgs(int argc, char **argv, char **foutName,
    char **finName, char *description);

FILE *modified_fopen(char *name, char *mode, FILE *default_stream);

#endif
