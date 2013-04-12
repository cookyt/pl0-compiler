/**
 * By:  Carlos Valera-Leon
 * PID: C3151942
 *
 * By:  Jon Hellmann
 * PID: J1607378
 *
 * P-Code Virtual Machine
 **/

#ifndef CODE_H_
#define CODE_H_

#define MAX_CODE_LENGTH 500
#define MAX_LEXI_LEVELS 3

typedef struct Instruction_
{
	int op;  /* OP Code */
	int l;   /* Lexicographical Level */
	int m;   /* Instruction-dependent parameter */
} Instruction;

typedef enum {
	OPR_RTN = 0,
	OPR_NEG,
	OPR_ADD,
	OPR_SUB,
	OPR_MUL,
	OPR_DIV,
	OPR_ODD,
	OPR_MOD,
	OPR_EQL,
	OPR_NEQ,
	OPR_LSS,
	OPR_LEQ,
	OPR_GTR,
	OPR_GEQ
} OPRType;

typedef enum {
	SIO_WRITE= 1,
	SIO_READ = 2
} SIOType;

typedef enum {
	INSTR_LIT = 1,
	INSTR_OPR,
	INSTR_LOD,
	INSTR_STO,
	INSTR_CAL,
	INSTR_INC,
	INSTR_JMP,
	INSTR_JPC,
	INSTR_SIO_R,    // annoyingly, SIO has two op codes.
    INSTR_SIO_W     // one for input, another for output.
} INSTRType;

#endif
