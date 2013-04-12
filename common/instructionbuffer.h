/*
 * instructionbuffer.h
 *
 *  Created on: Jul 9, 2012
 *      Author: Jon
 */

#ifndef INSTRUCTIONBUFFER_H_
#define INSTRUCTIONBUFFER_H_

#include <stdio.h>

typedef struct Instruction_ Instruction;

typedef struct InstructionBuffer_
{
	int length;
	int max_length;
	Instruction *buf;
} InstructionBuffer;

InstructionBuffer *createInstructionBuffer();
void freeInstructionBuffer(InstructionBuffer *code);

/* Generates an instruction and pushes it into the buffer */
void generateInstruction(InstructionBuffer *code, int op, int l, int m);

/* Rolls the instruction buffer back by one instruction */
void popInstruction(InstructionBuffer *code);

/* Returns the index of the first empty position in the code */
int getCodePosition(InstructionBuffer *code);

void printInstructionBuffer(InstructionBuffer *code, FILE *fout);

#endif /* INSTRUCTIONBUFFER_H_ */
