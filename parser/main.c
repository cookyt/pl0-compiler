#include <stdio.h>
#include <stdlib.h>

#include "debug.h"
#include "ioutils.h"

int main(int argc, char **argv)
{
    // Pseudo command-line args; uncomment the following lines and replace the
    // main function's signature with "int main(void)" in order to hard-code
    // the file paths to input.txt and output.txt.
#if 0
    int argc = 5;
    char* argv[] = {"./plparse", "-i", "input.txt", "-o", "output.txt"};
#endif

    char *finName = NULL;
    char *foutName = NULL;

    FILE *fin;
    FILE *fout;

    parseCommandLineArgs(argc, argv, &foutName, &finName,
        "A scanner for the PL/0 language.");

    fin = modified_fopen(finName, "r", stdin, NULL);
    fout = modified_fopen(foutName, "w", stdout, NULL);

    /* DO STUFF */

    fclose(fin);
    fclose(fout);
    return EXIT_SUCCESS;
}

