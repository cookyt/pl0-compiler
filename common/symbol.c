#include <stdlib.h>
#include <string.h>

#include "symbol.h"
#include "token.h"
#include "util.h"

Symbol *createSymbol(int kind, char *name, int val, int level, int address)
{
    Symbol *ret = (Symbol *)smalloc(sizeof(Symbol));
    ret->kind = kind;
    strncpy(ret->name, name, IDENTIFIER_SIZE);
    ret->val = val;
    ret->level = level;
    ret->adr = address;
    return ret;
}

void freeSymbol(Symbol* symbol)
{
    free(symbol);
}
