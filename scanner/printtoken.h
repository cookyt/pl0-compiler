/*
 * printToken.h
 *
 *  Created on: Jun 20, 2012
 *      Author: Jon
 */

#ifndef PRINTTOKEN_H_
#define PRINTTOKEN_H_

#include <stdio.h>
typedef struct TokenBuffer_ TokenBuffer;

void printTokenTable(TokenBuffer* b, FILE* fout);
void printTokenList(TokenBuffer* b, FILE* fout);
void printSymbolicTokenList(TokenBuffer* b, FILE* fout);

#endif /* PRINTTOKEN_H_ */
