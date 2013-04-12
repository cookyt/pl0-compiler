#ifndef SYMBOL_H_
#define SYMBOL_H_

#include "token.h"

typedef enum Kinds_
{
    KIND_CONST=1, KIND_VAR, KIND_PROC
} Kinds;

typedef struct Symbol_
{
    int kind;
    char name[IDENTIFIER_SIZE]; 
    int val;                    // value (for const)
    int level;                  // L level
    int adr;                    // M adderes
} Symbol;

Symbol *createSymbol(int kind, char *name, int val, int level, int address);
void freeSymbol(Symbol* symbol);

#endif /* SYMBOL_H_ */
