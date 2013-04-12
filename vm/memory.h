/**
 * By:  Carlos Valera-Leon
 * PID: C3151942
 *
 * By:  Jon Hellmann
 * PID: J1607378
 *
 * P-Code Virtual Machine
 * Fetch/execute cycle & Stack tracing
 **/

#ifndef MEMORY_H_
#define MEMORY_H_

#include "instruction.h"
#include "register.h"

void disassemble(FILE *fout, Instruction *text);
void run(FILE *fout, Instruction *text);
void printState(FILE* fout, Registers reg, int line, int *data);

#endif
