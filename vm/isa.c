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

#include <stdio.h>

#include "isa.h"
#include "debug.h"

int base(int L, int begin_bp, int *data);

const char *getInstructionName(Instruction inst)
{
    char string[40];
	switch (inst.op)
	{
		case 1:
			return "LIT";
		case 2:
			return "OPR";
		case 3:
			return "LOD";
		case 4:
			return "STO";
		case 5:
			return "CAL";
		case 6:
			return "INC";
		case 7:
			return "JMP";
		case 8:
			return "JPC";
		case 9:
		case 10:
			return "SIO";
		default:
            sprintf(string, "Unrecognized Instruction %d", inst.op);
            die(ERR_INVALID_INSTRUCION, string);
	}
	return (const char *)0;
}

const char *getOprName(Instruction inst)
{
	ASSERT(inst.op == 2);
    char string[50];
	switch (inst.m)
	{
		case 0:
			return "RET";
		case 1:
			return "NEG";
		case 2:
			return "ADD";
		case 3:
			return "SUB";
		case 4:
			return "MUL";
		case 5:
			return "DIV";
		case 6:
			return "ODD";
		case 7:
			return "MOD";
		case 8:
			return "EQL";
		case 9:
			return "NEQ";
		case 10:
			return "LSS";
		case 11:
			return "LEQ";
		case 12:
			return "GTR";
		case 13:
			return "GEQ";
		default:
            sprintf(string, "Unrecognized OPR Instruction %d", inst.op);
            die(ERR_INVALID_INSTRUCION, string);
	}
	return (const char *)0;
}

int base(int L, int begin_bp, int *data)
{
	int target_bp = begin_bp;
	while (L > 0)
	{
		target_bp = data[target_bp-1];
		L--;
	}
	return target_bp;
}

void LIT(Registers *reg, int *data)
{
	reg->sp++;
	data[reg->sp-1] = reg->ir.m;
}

void OPR(Registers *reg, int *data)
{
	switch(reg->ir.m)
	{
		case 0:
			reg->sp = reg->bp-1;
			reg->pc = data[reg->sp + 2];
			reg->bp = data[reg->sp + 1];
			break;
		case 1:
			data[reg->sp-1] = -data[reg->sp-1];
			break;
		case 2:
			reg->sp--;
			data[reg->sp-1] = data[reg->sp-1] + data[reg->sp];
			break;
		case 3:
			reg->sp--;
			data[reg->sp-1] = data[reg->sp-1] - data[reg->sp];
			break;
		case 4:
			reg->sp--;
			data[reg->sp-1] = data[reg->sp-1] * data[reg->sp];
			break;
		case 5:
			reg->sp--;
			data[reg->sp-1] = data[reg->sp-1] / data[reg->sp];
			break;
		case 6:
			data[reg->sp-1] = data[reg->sp-1]%2;
			break;
		case 7:
			reg->sp--;
			data[reg->sp-1] = data[reg->sp-1] % data[reg->sp];
			break;
		case 8:
			reg->sp--;
			data[reg->sp-1] = data[reg->sp-1] == data[reg->sp];
			break;
		case 9:
			reg->sp--;
			data[reg->sp-1] = data[reg->sp-1] != data[reg->sp];
			break;
		case 10:
			reg->sp--;
			data[reg->sp-1] = data[reg->sp-1] < data[reg->sp];
			break;
		case 11:
			reg->sp--;
			data[reg->sp-1] = data[reg->sp-1] <= data[reg->sp];
			break;
		case 12:
			reg->sp--;
			data[reg->sp-1] = data[reg->sp-1] > data[reg->sp];
			break;
		case 13:
			reg->sp--;
			data[reg->sp-1] = data[reg->sp-1] >= data[reg->sp];
			break;
	}
}

void LOD(Registers *reg, int *data)
{
	reg->sp++;
	data[reg->sp-1] = data[base(reg->ir.l, reg->bp, data)-1 + reg->ir.m];
}

void STO(Registers *reg, int *data)
{
	data[base(reg->ir.l, reg->bp, data)-1 + reg->ir.m] = data[reg->sp-1];
	reg->sp--;
}

void CAL(Registers *reg, int *data)
{
	data[reg->sp] = base(reg->ir.l, reg->bp, data);   /* SL */
	data[reg->sp+1] = reg->bp;                        /* DL */
	data[reg->sp+2] = reg->pc;                        /* RA */
	reg->bp = reg->sp + 1;
	reg->pc = reg->ir.m;
}

void INC(Registers *reg, int *data)
{
	reg->sp += reg->ir.m;
}

void JMP(Registers *reg, int *data)
{
	reg->pc = reg->ir.m;
}

void JPC(Registers *reg, int *data)
{
	if (data[reg->sp-1] == 0) reg->pc = reg->ir.m;
	reg->sp--;
}

void SIO(Registers *reg, int *data, FILE *fin, FILE *fout)
{
    char string[50];

	//TODO what do we read from/write to?
	switch(reg->ir.m)
	{
		case 1:
			fprintf(fout, "Output Value: %d\n", data[reg->sp-1]);
			reg->sp--;
			break;
		case 2:
			reg->sp++;
			fprintf(fout, "Input Value: ");
			fscanf(fin, "%d", &data[reg->sp-1]);
			break;
		default:
            sprintf(string, "Unrecognized IO Instruction %d", reg->ir.m);
            die(ERR_INVALID_INSTRUCION, string);
	}
}
