#include <string.h>
#include <stdlib.h>
#include <stdio.h>

#include "symboltree.h"
#include "symboltable.h"
#include "symbol.h"
#include "debug.h"
#include "util.h"

SymbolTree *createSymbolTree()
{
    SymbolTree *ret = (SymbolTree *)smalloc(sizeof(SymbolTree));
    ret->level = 0;
    ret->parent = NULL;
    ret->children = NULL;
    ret->next = NULL;
    ret->table = createSymbolTable(STD_SYMBOL_TABLE_SIZE);

    return ret;
}

void freeSymbolTree(SymbolTree *tree)
{
    SymbolTree *cur_child = tree->children;
    while(cur_child != NULL)
    {
        SymbolTree *del = cur_child;
        cur_child = cur_child->next;
        freeSymbolTree(del);
    }
    freeSymbolTable(tree->table);
    free(tree);
}

SymbolTree *addLexiLevel(SymbolTree *tree)
{
    SymbolTree *child = createSymbolTree();
    child->level = tree->level+1;
    child->parent = tree;
    child->next = tree->children;
    tree->children = child;

    return child;
}

void insertIntoSymbolTree(SymbolTree *tree, Symbol *symbol)
{
    insertIntoSymbolTable(tree->table, symbol);
}

Symbol *getSymbolFromTree(SymbolTree *tree, char *identifier)
{
    SymbolTree *cur_tree = tree;
    while (cur_tree != NULL)
    {
        Symbol *ret = getSymbolFromTable(cur_tree->table, identifier);
        if (ret != NULL)
            return ret;

        cur_tree = cur_tree->parent;
    }
    char *message = (char *)smalloc(strlen(identifier) + 20);
    sprintf(message, "Symbol '%s' not found.", identifier);
    die(ERR_NO_SUCH_SYMBOL, message);
    return NULL;
}
