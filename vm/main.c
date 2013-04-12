/**
 * By:  Carlos Valera-Leon
 * PID: C3151942
 *
 * P-Code Virtual Machine
 * Entry Point
 **/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "debug.h"
#include "memory.h"
#include "isa.h"
#include "instruction.h"
#include "register.h"
#include "ioutils.h"

int main(int argc, char **argv)
{
	char *finName = 0;
	char *foutName = 0;
	char *assemblyName = 0;

	FILE *fin;
	FILE *fout;

	Instruction text[MAX_CODE_LENGTH+1];

	parseCommandLineArgs(argc, argv, &foutName, &finName,
        "A virtual machine for running p-code.");

    fin = modified_fopen(finName, "r", stdin, NULL);
    fout = modified_fopen(foutName, "w", stdout, NULL);
	
	/* Read instructions into internal buffer */
	int i=0;
	for (;;)
	{
		int items = fscanf(fin, "%d %d %d", &text[i].op, &text[i].l, &text[i].m);
		if(feof(fin))
			break;

        if (items != 3)
        {
            char message[25];
            int c = fgetc(fin);
            sprintf(message, "Illigal character 0x%02x", c);
            die(ERR_ILLIGAL_CHAR, message);
        }
		i++;
		if (i >= MAX_CODE_LENGTH)
        {
            char message[70];
            sprintf(message, "This machine cannot run more "
                    "than %d instructions\n", MAX_CODE_LENGTH);
            die(ERR_CODE_TOO_LARGE, message);
        }
	}
	text[i].op = 0;
	text[i].l = 0;
	text[i].m = 0;

	disassemble(fout, text);
	run(fout, text);

	return EXIT_SUCCESS;
}
