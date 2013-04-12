#include <stdlib.h>
#include <stdio.h>

#include "instructionbuffer.h"
#include "debug.h"
#include "util.h"
#include "instruction.h"

InstructionBuffer *createInstructionBuffer()
{
    InstructionBuffer *ret = 
            (InstructionBuffer *)smalloc(sizeof(InstructionBuffer));
    ret->length = 0;
    ret->max_length = MAX_CODE_LENGTH;
    ret->buf = (Instruction *)smalloc(sizeof(Instruction)*MAX_CODE_LENGTH);
    return ret;
}

void freeInstructionBuffer(InstructionBuffer *code)
{
    free(code->buf);
    free(code);
}

void generateInstruction(InstructionBuffer *code, int op, int l, int m)
{
    if (code->length >= code->max_length)
        die(ERR_CODE_TOO_LARGE,
            "Cannot generate more p-code, code is too long");

    code->buf[code->length].op = op;
    code->buf[code->length].l  = l;
    code->buf[code->length].m  = m;
    code->length += 1;
}

void popInstruction(InstructionBuffer *code)
{
    if (code->length <= 0)
        die(ERR_POP_FROM_EMPTY_BUFFER, 
            "Cannot pop from an empty instruction buffer");
    code->length -= 1;
}

int getCodePosition(InstructionBuffer *code)
{
    return code->length;
}

void printInstructionBuffer(InstructionBuffer *code, FILE *fout)
{
    int i;
    for (i=0; i<code->length; i++)
    {
        fprintf(fout, "%d  %d  %d\n",
                code->buf[i].op,
                code->buf[i].l,
                code->buf[i].m);
    }
}
