#include "syntax.h"
#include "debug.h"
#include "token.h"
#include "symbol.h"
#include "symboltree.h"
#include "instructionbuffer.h"
#include "instruction.h"
#include "tokenbuffer.h"

void statement(TokenBuffer *tokens, InstructionBuffer *code, SymbolTree *tree)
{
	if (getCurrentToken(tokens).type == identsym){
		//then perform (ident ":=" expression).

		//Save the current token.
		Token myToken;
		myToken = getCurrentToken(tokens);

		if (getCurrentToken(tokens).type != identsym)
			die(ERR_ILLIGAL_CHAR, "Expected identifier after 'const'");
		nextToken(tokens);

		if (getCurrentToken(tokens).type != becomessym)
			die(ERR_ILLIGAL_CHAR, "':=' expected");
		nextToken(tokens);

		expression(tokens, code, tree);

		//Retrieve L and M before generating instruction on next line.
		Symbol *mySymbol = getSymbolFromTree(tree, myToken.lexeme);
        if (mySymbol->kind != KIND_VAR)
            die(ERR_BAD_ASSIGNMENT, 
                "After ':=', only variables can be assigned to");
		int L = tree->level - mySymbol->level;
		int M = mySymbol->adr;
		generateInstruction(code, INSTR_STO, L, M);
	} //end (ident ":=" expression).
	else if (getCurrentToken(tokens).type == beginsym){
		//then perform ("begin" statement {";" statement} "end").
		nextToken(tokens);
		statement(tokens, code, tree);
		while (getCurrentToken(tokens).type == semicolonsym){
			nextToken(tokens);
			statement(tokens, code, tree);
		}

		if (getCurrentToken(tokens).type != endsym)
			die(ERR_ILLIGAL_CHAR, "'end' expected");
		nextToken(tokens);

	} //end ("begin" statement {";" statement} "end").
	else if (getCurrentToken(tokens).type == ifsym){
		//then perform ("if" condition "then" statement ["else" statement]).
		int cx1;
		nextToken(tokens);
		condition(tokens, code, tree);
		if (getCurrentToken(tokens).type != thensym){
			//Then thensym was not found. Generate an error.
			die(ERR_ILLIGAL_CHAR, "'then' expected");
		}
		else {
			//Then thensym was found.
			nextToken(tokens);
		}
		cx1 = getCodePosition(code);   //cx1 = cx.
		generateInstruction(code, INSTR_JPC, 0, 0); //M will be updated soon. Initially 0.
		statement(tokens, code, tree);
		if (getCurrentToken(tokens).type == elsesym){
			//Then there is an ["else" statement].
			nextToken(tokens);
			int cx2 = getCodePosition(code);   //cx2 = cx.
			generateInstruction(code, INSTR_JMP, 0, 0); //M will be updated soon. Initially 0.
			statement(tokens, code, tree);
			//Update Instructions for an if-then-else statement:
			code->buf[cx1].m = cx2+1; //code[cx1].m = cx2+1.
			code->buf[cx2].m = getCodePosition(code); //code[cx2].m = cx.
		}
		else{
			//Then there is not an ["else" statement].
			//Update Instructions for an if-then statement:
			code->buf[cx1].m = getCodePosition(code);  //code[cx1].m = cx.
		}
	} //end ("if" condition "then" statement ["else" statement]).
	else if (getCurrentToken(tokens).type == whilesym){
		//then perform ("while" condition "do" statement).
		int cx1, cx2;
		cx1 = getCodePosition(code);   //cx1 = cx;.
		nextToken(tokens);
		condition(tokens, code, tree);
		cx2 = getCodePosition(code);   //cx2 = cx;.
		generateInstruction(code, INSTR_JPC, 0, 0); //M will be changed soon. Initially 0.
		if (getCurrentToken(tokens).type != dosym){
			//Then dosym was not found. Generate an error.
			die(ERR_ILLIGAL_CHAR, "'do' expected");
		}
		else {
			//Then dosym was found.
			nextToken(tokens);
		}
		statement(tokens, code, tree);
		generateInstruction(code, INSTR_JMP, 0, cx1);
		code->buf[cx2].m = getCodePosition(code); //code[cx2].m = cx;.
	} //end ("while" condition "do" statement).
    else if(getCurrentToken(tokens).type == callsym)
    {
        nextToken(tokens);
        Token cur_token = getCurrentToken(tokens);
        if (cur_token.type != identsym)
            die(ERR_ILLIGAL_CHAR, "Expected identifier after 'call'");

		Symbol *sym = getSymbolFromTree(tree, cur_token.lexeme);
        if (sym->kind != KIND_PROC)
            die(ERR_NO_SUCH_PROC, 
                "Identifier is not a procedure or cannot be accesed from here");
        generateInstruction(code, INSTR_CAL, (tree->level)-(sym->level), sym->adr);
        nextToken(tokens);
    } //end ("call" ident).
	else if (getCurrentToken(tokens).type == insym){
		//Then perform ("in" ident).
		nextToken(tokens);
		//Save the current token.
		Token myToken;
		myToken = getCurrentToken(tokens);

		if (myToken.type != identsym){
			//Then identsym was not found. Generate an error.
			die(ERR_ILLIGAL_CHAR, "Expected identifier after 'in'");
		}
		else{
			//Then identsym was found. Proceed.
			nextToken(tokens);
		}

		Symbol *mySymbol = getSymbolFromTree(tree, myToken.lexeme);
        if (mySymbol->kind != KIND_VAR)
            die(ERR_BAD_ASSIGNMENT, 
                "After 'in', only variables can be assigned to");
		//Retrieve L and M before generating the instructions.
		int L = tree->level - mySymbol->level;
		int M = mySymbol->adr;
		generateInstruction(code, INSTR_SIO_R, 0, SIO_READ);
		generateInstruction(code, INSTR_STO, L, M);
	} //end ("in" ident).
    else if(getCurrentToken(tokens).type == outsym)
    {
        nextToken(tokens);
        expression(tokens, code, tree);
        generateInstruction(code, INSTR_SIO_W, 0, SIO_WRITE);
    } //end ("out" expression).
}
//end statement.


/* JON */
void program(TokenBuffer *tokens, InstructionBuffer *code, SymbolTree *tree)
{
	block(tokens, code, tree);

    // periodsym was not found. Generate an error.
	if (getCurrentToken(tokens).type != periodsym){
		die(ERR_ILLIGAL_CHAR, "'.' expected");
	}

	generateInstruction(code, INSTR_OPR, 0, OPR_RTN);

    //NULL instruction denotes end of instruction buffer (used for printing)
	generateInstruction(code, 0, 0, 0);
}
//end program.


void const_declaration(TokenBuffer *tokens, InstructionBuffer *code, SymbolTree *tree)
{
	//Optional Constant Declaration.
	if (getCurrentToken(tokens).type == constsym){
		int number;
		nextToken(tokens);

		if (getCurrentToken(tokens).type != identsym)
			die(ERR_ILLIGAL_CHAR, "Expected identifier after 'const'");

		//ident. Save the current token.
		Token myToken;
		myToken = getCurrentToken(tokens);

		nextToken(tokens);

		if (getCurrentToken(tokens).type != eqsym)
			die(ERR_ILLIGAL_CHAR, "'=' expected");
		nextToken(tokens);

		if (getCurrentToken(tokens).type != numbersym)
			die(ERR_ILLIGAL_CHAR, "Expected number during constant declaration.");

		//Get number.
        Token temp = getCurrentToken(tokens);
		number = atoi(temp.lexeme);

		//Add constant to symbol table. //TODO. Double Check This.
		Symbol *sym = createSymbol(KIND_CONST,    			//kind
		                                   myToken.lexeme,  //name
		                                   number,          //value
		                                   tree->level, 	//lexi-level
		                                   0);    			//address
		insertIntoSymbolTree(tree, sym);
		//End Add constant to symbol table.
		nextToken(tokens);

		while (getCurrentToken(tokens).type == commasym){
			nextToken(tokens);

			//ident. Save the current token.
			Token myToken;
			myToken = getCurrentToken(tokens);

			nextToken(tokens);

			if (getCurrentToken(tokens).type != eqsym)
				die(ERR_ILLIGAL_CHAR, "'=' expected");
			nextToken(tokens);

			if (getCurrentToken(tokens).type != numbersym)
				die(ERR_ILLIGAL_CHAR, "Expected number during constant declaration.");

			//Get number.
			Token temp = getCurrentToken(tokens);
			number = atoi(temp.lexeme);

			//Add constant to symbol table. //TODO. Double Check This.
			Symbol *sym = createSymbol(KIND_CONST,    			//kind
											   myToken.lexeme,  //name
											   number,          //value
											   tree->level, 	//lexi-level
											   0);    			//address
			insertIntoSymbolTree(tree, sym);
			//End Add constant to symbol table.
			nextToken(tokens);
		}
		//end while.

		if (getCurrentToken(tokens).type != semicolonsym)
			die(ERR_ILLIGAL_CHAR, "';' expected");
		nextToken(tokens);

	}
	//end Optional Constant Declaration.
}
//end const_declaration.


void condition(TokenBuffer *tokens, InstructionBuffer *code, SymbolTree *tree)
{

	if (getCurrentToken(tokens).type == oddsym){
		//Then we are performing ("odd" expression).
		nextToken(tokens);
		expression(tokens, code, tree);
		generateInstruction(code, INSTR_OPR, 0, OPR_ODD);
	}
	else {
		//Then we are performing (expression rel-op expression).
		expression(tokens, code, tree);

		//rel-op.
		int operation = getCurrentToken(tokens).type;
		if (operation != eqsym && operation != neqsym && operation !=lessym && operation != leqsym && operation != gtrsym && operation != geqsym){
			//Then a relational operator was not found. Generate an error and exit.
			die(ERR_ILLIGAL_CHAR, "Relational operator expected");
		}
		else {
			//Then a relational operator was found.
			//Convert operation to OPR_xxx equivalent.
			switch (operation){
				case eqsym: operation = OPR_EQL; break;
				case neqsym: operation = OPR_NEQ; break;
				case lessym: operation = OPR_LSS; break;
				case leqsym: operation = OPR_LEQ; break;
				case gtrsym: operation = OPR_GTR; break;
				case geqsym: operation = OPR_GEQ; break;
			}
			//end switch.
			nextToken(tokens);
			expression(tokens, code, tree);
			generateInstruction(code, INSTR_OPR, 0, operation);
		}
	}
	//end (expression rel-op expression).
}
//end condition.


void term(TokenBuffer *tokens, InstructionBuffer *code, SymbolTree *tree)
{
	int operation;
	factor(tokens, code, tree);
	while ( getCurrentToken(tokens).type == multsym || getCurrentToken(tokens).type == slashsym)
	{
		operation = getCurrentToken(tokens).type;
		nextToken(tokens);
		factor(tokens, code, tree);
		if (operation == multsym)
			generateInstruction(code, INSTR_OPR, 0, OPR_MUL); //Multiplication
		else
			generateInstruction(code, INSTR_OPR, 0, OPR_DIV); //Division.
	}
	//end while.
}
//end term.


/* CARLOS */
void block(TokenBuffer *tokens, InstructionBuffer *code, SymbolTree *tree)
{
    const_declaration(tokens, code, tree);
    var_declaration(tokens, code, tree);

    int code_pos = getCodePosition(code);
    generateInstruction(code, INSTR_JMP, 0, 0);
    proc_declaration(tokens, code, tree);

    int new_code_pos = getCodePosition(code);

    // proc_declaration didn't generate any code, undo the jump instruction
    if (new_code_pos-code_pos == 1)
        popInstruction(code);
    else
        code->buf[code_pos].m = new_code_pos;

    statement(tokens, code, tree);
}

void proc_declaration(TokenBuffer *tokens, InstructionBuffer *code,
                     SymbolTree *tree)
{
    Token cur = getCurrentToken(tokens);
    while (cur.type == procsym)
    {
        nextToken(tokens);
        cur = getCurrentToken(tokens);
        if (cur.type != identsym)
            die(ERR_ILLIGAL_CHAR, "Expected identifier after 'procedure'");

        int proc_beginning = getCodePosition(code);

        Symbol *sym = createSymbol(KIND_PROC,       //kind
                                   cur.lexeme,      //name
                                   0,               //value
                                   tree->level,     //lexi-level
                                   proc_beginning); //address

        insertIntoSymbolTree(tree, sym);

        nextToken(tokens);
        cur = getCurrentToken(tokens);
        if (cur.type != semicolonsym)
            die(ERR_ILLIGAL_CHAR, "Expected ';' after procedure declaration");

        SymbolTree *proc_tree = addLexiLevel(tree);
        nextToken(tokens);
        block(tokens, code, proc_tree);

        cur = getCurrentToken(tokens);
        if (cur.type != semicolonsym)
            die(ERR_ILLIGAL_CHAR, "Expected ';' after procedure definition");
        generateInstruction(code, INSTR_OPR, 0, OPR_RTN);

        nextToken(tokens);
        cur = getCurrentToken(tokens);
    }
}

void var_declaration(TokenBuffer *tokens, InstructionBuffer *code,
                     SymbolTree *tree)
{
    Token cur = getCurrentToken(tokens);

    // positions 0, 1, and 2 are reserved for the SL DL and RA
    int address = 3;
    if (cur.type == intsym)
    {
        do
        {
            nextToken(tokens);
            cur = getCurrentToken(tokens);
            if (cur.type != identsym)
                die(ERR_ILLIGAL_CHAR, "Expected identifier after 'int'");

            Symbol *sym = createSymbol(KIND_VAR,    //kind
                                       cur.lexeme,  //name
                                       0,           //value
                                       tree->level, //lexi-level
                                       address);    //address

            insertIntoSymbolTree(tree, sym);
            address++;

            nextToken(tokens);
            cur = getCurrentToken(tokens);

        } while (cur.type == commasym);

        if (cur.type != semicolonsym)
            die(ERR_ILLIGAL_CHAR, "Expected ';' after declaration");

        nextToken(tokens);
    }

    //allocate all the variables at once
    generateInstruction(code, INSTR_INC, 0, address);
}

void expression(TokenBuffer *tokens, InstructionBuffer *code, SymbolTree *tree)
{
    Token cur = getCurrentToken(tokens);
    int negate = 0;
    switch (cur.type)
    {
        case minussym:
            negate = 1;
        case plussym:
            nextToken(tokens);
    }
    term(tokens, code, tree);
    if (negate)
        generateInstruction(code,
                            INSTR_OPR,  // op
                            0,          // L
                            OPR_NEG);   // M

    cur = getCurrentToken(tokens);
    while (cur.type == plussym || cur.type == minussym)
    {
        int operation;
        if (cur.type == plussym)
            operation = OPR_ADD;
        else
            operation = OPR_SUB;
        nextToken(tokens);
        term(tokens, code, tree);
        generateInstruction(code,
                            INSTR_OPR,      // op
                            0,              // L
                            operation);     // M
        cur = getCurrentToken(tokens);
    }
}

void factor(TokenBuffer *tokens, InstructionBuffer *code, SymbolTree *tree)
{
    Token cur = getCurrentToken(tokens);
    Symbol *sym;
    int val;
    switch (cur.type)
    {
        case identsym:
            sym = getSymbolFromTree(tree, cur.lexeme);
            if (sym->kind == KIND_VAR)
                generateInstruction(code,
                                    INSTR_LOD, 
                                    tree->level-sym->level,
                                    sym->adr);
            else if (sym->kind == KIND_CONST)
                generateInstruction(code,
                                    INSTR_LIT, 
                                    0,
                                    sym->val);
            else if (sym->kind == KIND_PROC)
                die(ERR_PROC_NOT_FACTOR, 
                    "Procedures cannot be used as a value");
            break;
        case numbersym:
            val = atoi(cur.lexeme);
            generateInstruction(code,
                                INSTR_LIT, 
                                0,
                                val);
            break;
        case lparentsym:
            nextToken(tokens);
            expression(tokens, code, tree);
            cur = getCurrentToken(tokens);
            if (cur.type != rparentsym)
                die(ERR_ILLIGAL_CHAR, "expected ')' after expression");
            break;
        default:
            die(ERR_ILLIGAL_CHAR, "Dangling factor");
    }
    nextToken(tokens);
}

