#include <string.h>
#include <stdio.h>

#include "debug.h"
#include "ioutils.h"
#include "util.h"

int _IO_show_lexemes = 0;
int _IO_show_assembler = 0;
int _IO_show_stack_trace = 0;

/**
 * Prints the entire contents of the one file into another.
 *
 * NOTE: It makes one complete pass over the File pointer, so it's not good for
 *   reprinting anything from standard input.
 * Solutions:
 *   read file into a buffer once and tokenize directly from the buffer.
 *   read characters into extra buffer as tokenization happens
 *     could just concatenate contents of main_buffer to file buffer before empting
 *     would also need to specially add to file buffer for whitespace and comments
 *
 * @param fin the file to read from, opened with read mode. This file pointer
 *   is restored to its position prior to calling this function.
 * @param fout to file to write to, opened with write mode.
 **/
void print_file(FILE *fin, FILE *fout)
{
    fpos_t pos;
    fgetpos(fin, &pos);

    rewind(fin);
    for (;;)
    {
        char peek = fgetc(fin);

        if (feof(fin))
            break;

        fputc(peek, fout);
    }

    fsetpos(fin, &pos);
}

/**
 * Tries to open a file. If the name of the file is a NULL pointer or the file
 * cannot be opened, the file defaults to default_stream and prints out a
 * warning given by message. The program is killed if, at the end of this
 * function, the file pointer is NULL.
 *
 * @param name  the name of the desired file to open.
 * @param mode  the mode the file is desired in.
 * @param default_stream  a stream to point to if the file cannot be opened
 *
 *
 * @return A pointer to the opened file.
 **/
FILE *modified_fopen(char *name, char *mode, FILE *default_stream)
{
    FILE *fileptr;
    if (name == NULL)
    {
        if (default_stream == NULL)
            die(ERR_FILE, "Unspecified file.");

        fileptr = default_stream;
    }
    else
    {
        fileptr = fopen(name, mode);
        if (fileptr == NULL)
        {
            if (default_stream == NULL)
            {
                int varlengths = strlen(name) + strlen(mode);
                char *ermessage = (char *)smalloc(varlengths + 50);
                sprintf(ermessage, "Could not open file %s with mode %s.", 
                        name, mode);
                die(ERR_FILE, ermessage);
            }

            fileptr = default_stream;
        }
    }
    return fileptr;
}

/**
 * Parses command line arguments and sets the file names to their correct
 * values. The list of applicable switches is given by the -h or --help
 * argument
 *
 * @param argc [in] the number of command line arguments
 * @param argv [in] array of command-line arguments
 * @param foutName [out] the name of the output file (pass by pointer)
 * @param finName [out] the name of the input file (pass by pointer)
 **/
void parseCommandLineArgs(int argc, char **argv, char **foutName,
    char **finName, char *description)
{
    *foutName = NULL;
    *finName = NULL;

    int i;
    for (i=1; i<argc; i++)
    {
        if (argv[i][0] == '-')
        {
            int j;
            int len = strlen(argv[i]);
            for (j=1; j<len; j++)
            {
                switch (argv[i][j])
                {
                    case 'w':
                        _DEBUG_no_warnings  = 1;
                        break;
                    case 'e':
                        _DEBUG_no_errors  = 1;
                        break;
                    case 'l':
                        _IO_show_lexemes = 1;
                        break;
                    case 'a':
                        _IO_show_assembler = 1;
                        break;
                    case 'v':
                        _IO_show_stack_trace = 1;
                        break;
                    case 'o':
                        if ((i+1) >= argc)
                            die(ERR_FILE, "Expected file name after -o switch");
                        *foutName = argv[i+1];
                        break;
                    case 'i':
                        if ((i+1) >= argc)
                            die(ERR_FILE, "Expected file name after -i switch");
                        *finName = argv[i+1];
                        break;
                    case '-':
                        if (strcmp("--help", argv[i]) != 0) break;
                    case 'h':
                        fprintf(stderr,
                            "%s\n"
                            "Usage: %s [OPTIONS]\n"
                            "\t-h or --help     prints this help file and terminates the program\n"
                            "\t-l               show lexeme list (scanner output)\n"
                            "\t-a               show assembled code (parser output)\n"
                            "\t-v               show stack trace as program runs\n"
                            "\t-w               don't print warnings to stderr\n"
                            "\t-e               don't print errors to stderr\n"
                            "\t-i file_name     input file (stdin by default)\n"
                            "\t-o file_name     output file (stdout by default)\n",
                            description, argv[0]);
                        exit(EXIT_FAILURE);
                        break;
                }
            }
        }
    }
    if (*foutName != NULL && *finName != NULL)
        if (strcmp(*foutName, *finName) == 0)
            die(ERR_FILE, "Input and output file cannot be the same.");
}
