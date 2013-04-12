/**
 * By:  Carlos Valera-Leon
 * PID: C3151942
 *
 * By:  Jon Hellmann
 * PID: J1607378
 *
 * P-Code Virtual Machine
 * ISA implementation
 **/

#ifndef ISA_H_
#define ISA_H_

#include "register.h"
#include "instruction.h"

const char *getInstructionName(Instruction inst);
const char *getOprName(Instruction inst);

void LIT(Registers *reg, int *data);
void OPR(Registers *reg, int *data);
void LOD(Registers *reg, int *data);
void STO(Registers *reg, int *data);
void CAL(Registers *reg, int *data);
void INC(Registers *reg, int *data);
void JMP(Registers *reg, int *data);
void JPC(Registers *reg, int *data);
void SIO(Registers *reg, int *data, FILE *fin, FILE *fout);

#endif
