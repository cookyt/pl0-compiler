/**
 * By:  Carlos Valera-Leon
 * PID: C3151942
 *
 * By:  Jon Hellmann
 * PID: J1607378
 *
 * P-Code Virtual Machine
 * Fetch/execute cycle and IO
 **/

#include <stdio.h>

#include "debug.h"
#include "isa.h"
#include "memory.h"

extern int _IO_show_stack_trace;

void printStack(FILE* fout, int cur_bp, int prev_bp, int *data, int pipe);

/**
 * Prints the disassembly of the instructions held in the text buffer. Uses an
 * instruction with an OP code of 0 as the stopping condition
 *
 * @param fout the file to print the disassembly to
 * @param text the buffer holding the instructions
 **/
void disassemble(FILE *fout, Instruction *text)
{
	fputs("LINE    OP      L       M\n", fout);

	int line;
	for (line=0; text[line].op != 0; line++)
		fprintf(fout, "%-8d%-8s%-8d%-8d\n", line, getInstructionName(text[line]),
			text[line].l, text[line].m);
	fputs("\n\n", fout);
	fflush(fout);
}


/**
 * Executes machine instructions from a buffer, printing the results to a file.
 *
 * @param fout the file to print execution to
 * @param text the buffer holding the instructions
 **/
void run(FILE *fout, Instruction *text)
{
	int data[MAX_STACK_HEIGHT];
	int line;
	Registers reg;

	reg.sp = 0;
	reg.bp = 1;
	reg.pc = 0;

    if (_IO_show_stack_trace)
    {
        fputs("LINE OP   L    M       PC   BP   SP   STACK\n", fout);
        fprintf(fout, "INITIAL VALUES         %-4d %-4d %-4d ", reg.pc, reg.bp,
            reg.sp);
        printStack(fout, reg.bp, reg.sp, data, 0);
        fputc('\n', fout);
    }

	for (;;)
	{
		/* Fetch phase */
		reg.ir = text[reg.pc];
		line = reg.pc;
		reg.pc++;

        char message[50];
		switch(reg.ir.op)
		{
			case 1:
				LIT(&reg, data);
				break;
			case 2:
				OPR(&reg, data);
				break;
			case 3:
				LOD(&reg, data);
				break;
			case 4:
				STO(&reg, data);
				break;
			case 5:
				CAL(&reg, data);
				break;
			case 6:
				INC(&reg, data);
				break;
			case 7:
				JMP(&reg, data);
				break;
			case 8:
				JPC(&reg, data);
				break;
			case 9:
			case 10:
				SIO(&reg, data, stdin, stdout);
				break;
			default:
                sprintf(message,"Unrecognized Instruction %d", reg.ir.op);
                die(ERR_INVALID_INSTRUCION, message);
		}
        if (_IO_show_stack_trace)
            printState(fout, reg, line, data);
		if (reg.sp >= MAX_STACK_HEIGHT)
        {
            char message[50];
            sprintf(message, "Stack Overflow (stack max: %d)",
                    MAX_STACK_HEIGHT);
            die(ERR_INVALID_INSTRUCION, message);
        }
		if (reg.bp == 0) break;
	}
}

/**
 * Prints the current state of the program.
 * FORMAT:
 *    LINE_NO INSTRUCTION L M PC BP SP STACK_STATE
 *
 * @param fout File to print to.
 * @param reg  The current register values.
 * @param line The line number of the current instruction.
 * @param data Pointer to the base of the stack.
 **/
void printState(FILE* fout, Registers reg, int line, int *data)
{
	fprintf(fout, "%-4d %-4s %-4d %-4d    %-4d %-4d %-4d ", line,
		getInstructionName(reg.ir), reg.ir.l, reg.ir.m, reg.pc, reg.bp,
		reg.sp);
	printStack(fout, reg.bp, reg.sp, data, 0);
	fputc('\n', fout);
}

/**
 * Prints the values in the stack, putting a pipe character '|' between each
 * stack frame.
 *
 * @param fout      The file to print to.
 * @param cur_bp    The base pointer of the last stack frame
 * @param prev_bp   Used for recursive calls, pass the initial stack pointer+1
 * @param data      Pointer to the stack
 **/
void printStack(FILE* fout, int cur_bp, int prev_bp, int *data, int pipe)
{
	if (cur_bp == 0) return;
	if (cur_bp > 1)
	{
		printStack(fout, data[cur_bp], cur_bp-1, data, 1);
	}
	while(cur_bp <= prev_bp)
	{
		fprintf(fout, "%d ", data[cur_bp-1]);
		cur_bp++;
	}
	if (pipe)
		fputs("| ", fout);
}
