#ifndef SYMBOLTABLE_H_
#define SYMBOLTABLE_H_

#define STD_SYMBOL_TABLE_SIZE 16

typedef struct Symbol_ Symbol;

typedef struct SymbolListNode_
{
    Symbol *data;
    struct SymbolListNode_ *next;
} SymbolListNode;

typedef struct SymbolTable_
{
    int size;
    SymbolListNode **data;
} SymbolTable;

/**
 * Creates a new SymbolTable
 *
 * @param size The size of this new table. Passing a value less than 1 results
 *   in a size of DEFAULT_SYMBOL_TABLE_SIZE.
 **/
SymbolTable *createSymbolTable(int size);

/**
 * Frees the memory allocated for the table and all the symbols in it.
 **/
void freeSymbolTable(SymbolTable *table);

/**
 * Inserts the given symbol into the given hash table. The hash of the string
 * 'identifier' is used to position the symbol.
 *
 * @param symbol The symbol to add to the table, it's memory is freed when
 *   freeSymbolTable() is called.
 **/
void insertIntoSymbolTable(SymbolTable *table, Symbol *symbol);

/**
 * Searches for a symbol in the table.
 *
 * @return the found symbol or NULL of it can't be found
 **/
Symbol *getSymbolFromTable(SymbolTable *table, char *identifier);

/**
 * Computes a unique hash for the given NULL-terminated string. The resulting
 * value goes from 0 to INT_MAX. Value likely not larger than 2^16. 
 *
 * @param string a NUL-terminated string
 *
 * @warning (debug) if the string is longer than IDENTIFIER_SIZE
 **/
unsigned int hash(char *string);

#endif /* SYMBOLTABLE_H_ */
