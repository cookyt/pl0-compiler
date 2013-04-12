#ifndef REGISTER_H_
#define REGISTER_H_

#define MAX_STACK_HEIGHT 2000

#include "instruction.h"

typedef struct Registers_
{
	int sp;          /* Stack Pointer */
	int bp;          /* Base Pointer */
	int pc;          /* Program Counter */
	Instruction ir;  /* Instruction Register */
} Registers;

#endif
