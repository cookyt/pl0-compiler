/**
 * PL/0 Grammer definitions
 *
 *  Created on: Jul 9, 2012
 *      Author: John Helmann
 *      Author: Carlos Valera
 **/

#ifndef SYNTAX_H_
#define SYNTAX_H_

typedef struct InstructionBuffer_ InstructionBuffer;
typedef struct TokenBuffer_ TokenBuffer;
typedef struct SymbolTree_ SymbolTree;


/* JOHN */
void program(TokenBuffer *tokens, InstructionBuffer *code, SymbolTree *tree);
void const_declaration(TokenBuffer *tokens, InstructionBuffer *code,
                       SymbolTree *tree);
void condition(TokenBuffer *tokens, InstructionBuffer *code, SymbolTree *tree);
void term(TokenBuffer *tokens, InstructionBuffer *code, SymbolTree *tree);
void statement(TokenBuffer *tokens, InstructionBuffer *code, SymbolTree *tree);


/* CARLOS */
void proc_declaration(TokenBuffer *tokens, InstructionBuffer *code,
                      SymbolTree *tree);
void block(TokenBuffer *tokens, InstructionBuffer *code,
           SymbolTree *tree);
void var_declaration(TokenBuffer *tokens, InstructionBuffer *code,
                     SymbolTree *tree);
void expression(TokenBuffer *tokens, InstructionBuffer *code,
                SymbolTree *tree);
void factor(TokenBuffer *tokens, InstructionBuffer *code, SymbolTree *tree);


#endif /* SYNTAX_H_ */
