#include <stdio.h>
#include <stdlib.h>

#include "debug.h"
#include "ioutils.h"
#include "filebuffer.h"
#include "tokenbuffer.h"
#include "instructionbuffer.h"
#include "symboltree.h"
#include "scanner/tokenizer.h"
#include "scanner/printtoken.h"
#include "parser/syntax.h"

int main(int argc, char **argv)
{
    // Pseudo command-line args; uncomment the following lines and replace the
    // main function's signature with "int main(void)" in order to hard-code
    // the file paths to input.txt and output.txt.
#if 0
    int argc = 5;
    char* argv[] = {"./plrun", "-i", "input.txt", "-o", "output.txt"};
#endif

    char *finName = NULL;
    char *foutName = NULL;

    FILE *fin;
    FILE *fout;

    parseCommandLineArgs(argc, argv, &foutName, &finName,
        "A Compiler for PL/0 with an integrated virtual machine.");

    fin = modified_fopen(finName, "r", stdin);
    fout = modified_fopen(foutName, "w", stdout);

    /* SCANNING */
    FileBuffer *file_buffer = createFileBuffer(fin);
    fclose(fin);
    TokenBuffer *token_buffer = tokenize(file_buffer);
    freeFileBuffer(file_buffer);

    if (_IO_show_lexemes)
    {
        printTokenList(token_buffer, fout);
        fputs("\n\n", fout);
        printSymbolicTokenList(token_buffer, fout);
        fputs("\n\n", fout);
    }

    InstructionBuffer *code = createInstructionBuffer();
    SymbolTree *tree = createSymbolTree();

    program(token_buffer, code, tree);

    freeSymbolTree(tree);
    freeTokenBuffer(token_buffer);

    fputs("No errors, program is syntactically correct.\n\n", fout);
    if (_IO_show_assembler)
    {
        fputs("Instructions: \n", fout);
        disassemble(fout, code->buf);
    }

    run(fout, code->buf);

    fclose(fout);
    freeInstructionBuffer(code);
    return EXIT_SUCCESS;
}

