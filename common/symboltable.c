#include <string.h>
#include <stdlib.h>

#include "symboltable.h"
#include "symbol.h"
#include "token.h"
#include "debug.h"
#include "util.h"

/**
 * Reads two bytes from the string at a time (treating them as 16-bit unsigned
 * integers) adds them together, and returns the result mod max_size.
 **/
unsigned int hash(char *string)
{
    int i;
    int sum = 0;
    for (i=0;;i++)
    {
#ifdef DEBUG
        if (i == IDENTIFIER_SIZE)
        {
            char *message = (char *)smalloc(strlen(string) + 50);
            sprintf(message, "Hashing string '%s' larger than an identifier",
                    string);
            warning(WARN_HASH_STRING_TOO_LONG, message);
            free(message);
        }
#endif
        if (*(string+i) == 0)
            return sum;

        sum += (unsigned int) (*(unsigned short *)string);

        if (*(string+i+1) == 0)
            return sum;
    }
}

/**
 * Allocate's mem for both the table and the array of linked lists in it
 **/
SymbolTable *createSymbolTable(int size)
{
    SymbolTable *ret = (SymbolTable *)smalloc(sizeof(SymbolTable));
    ret->size = size;
    ret->data = (SymbolListNode **)smalloc(sizeof(SymbolListNode *)*size);
    memset(ret->data, 0, sizeof(SymbolListNode *)*size);
    return ret;
}

/**
 * Steps though every slot in the table's array of linked lists, and deletes
 * the list if there is anything in it. Then frees the array and table.
 **/
void freeSymbolTable(SymbolTable *table)
{
    int i;
    for (i=0; i<table->size; i++)
    {
        SymbolListNode* cur_symbol = table->data[i];
        while (cur_symbol != NULL)
        {
            SymbolListNode *del = cur_symbol;
            cur_symbol = cur_symbol->next;
            freeSymbol(del->data);
            free(del);
        }
    }
    free(table->data);
    free(table);
}

/**
 * Wraps the symbol in a list node, and inserts the node at the head of the
 * linked list located in the table's array of linked lists, indexed by the has
 * of the symbol's identifier.
 **/
void insertIntoSymbolTable(SymbolTable *table, Symbol *symbol)
{
    Symbol *already_in_check = getSymbolFromTable(table, symbol->name);
    if (already_in_check == NULL)
    {
        unsigned int hash_value = hash(symbol->name) % table->size;
        SymbolListNode *new_node = 
                (SymbolListNode *)smalloc(sizeof(SymbolListNode));
        new_node->data = symbol;
        new_node->next = table->data[hash_value];
        table->data[hash_value] = new_node;
    }
    else
    {
        char *message = (char *)smalloc(strlen(symbol->name) + 30);
        sprintf(message, "Redeclaration of symbol '%s'", symbol->name);
        die(ERR_SYMBOL_REDEFINITION, message);
    }
}

/**
 * calculates the hash of the identifier, then searches the appropriate linked
 * list for the first symbol with the same name. This gives the last symbol
 * declared with this name.
 **/
Symbol *getSymbolFromTable(SymbolTable *table, char *identifier)
{
    unsigned int hash_value = hash(identifier)%table->size;
    SymbolListNode *cur_node = table->data[hash_value];
    while(cur_node != NULL)
    {
        if (strcmp(cur_node->data->name, identifier) == 0)
            return cur_node->data;
        cur_node = cur_node->next;
    }
    return NULL;
}
