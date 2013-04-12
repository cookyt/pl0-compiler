#ifndef SYMBOLTREE_H_
#define SYMBOLTREE_H_

typedef struct SymbolTable_ SymbolTable;
typedef struct Symbol_ Symbol;

/**
 * This represents a single node in the SymbolTree. 
 **/
typedef struct SymbolTree_
{
    int level;                     //lexicoraphical level of this node
    struct SymbolTree_ *parent;    //Parent of this node
    struct SymbolTree_ *children;  //Linked list of this node's children
    struct SymbolTree_ *next;      //The next sibling of this node
    SymbolTable *table;
} SymbolTree;

/**
 * @return The root of a new symbol tree.
 **/
SymbolTree *createSymbolTree();
void freeSymbolTree(SymbolTree *tree);

/**
 * Call when a creating a new lexicographical level (procedure).
 * Creates a new symbol table for the level, and adds it to the children of the
 * passed tree. Its level is the level of the current tree + 1
 *
 * @return the newly created symbol tree node.
 **/
SymbolTree *addLexiLevel(SymbolTree *tree);

/**
 * Inserts a symbol into the tree.
 **/
void insertIntoSymbolTree(SymbolTree *tree, Symbol *symbol);

/**
 * Searches the tables of all the tree nodes from the current to the root for
 * the given identifier.
 *
 * @error if the symbol could not be found anywhere in the tree.
 * @return the found symbol or NULL of it can't be found
 **/
Symbol *getSymbolFromTree(SymbolTree *tree, char *identifier);

#endif /* SYMBOLTREE_H_ */
