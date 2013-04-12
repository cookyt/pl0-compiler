#include <stdio.h>

#include "printtoken.h"
#include "tokenbuffer.h"
#include "token.h"


/*
 * printTokenTable  prints the Lexeme Table to std output.
 * printTokenList prints the Lexeme List to std output and to the file lexlist.txt.
 *
 * TODO Does print out nulsym. Should this be printed? When will this token be generated
 * in the first place?
 *
 */

void printTokenTable(TokenBuffer* b, FILE* fout){
	int numTokens = b->length;

	fprintf(fout, "Lexeme Table:");
	fprintf(fout, "\nlexeme        token type");
	int i = 0;
	for ( i = 0; i < numTokens; i++){
		fprintf(fout, "\n%-14s%d", b->buf[i].lexeme, b->buf[i].type);
	}
}
//end printTokenTable

void printTokenList(TokenBuffer* b, FILE* fout){
	int numTokens = b->length;

	fprintf(fout, "Lexeme List:\n");
	int i = 0;
	for ( i = 0; i < numTokens; i++){
		if ((b->buf[i].type == 2) || (b->buf[i].type == 3)){
			fprintf(fout, "%d %s ", b->buf[i].type, b->buf[i].lexeme);
		}
		else{
			fprintf(fout, "%d ", b->buf[i].type);
		}
	}
}
//end printTokenList

void printSymbolicTokenList(TokenBuffer* b, FILE* fout){
	int numTokens = b->length;

	fprintf(fout, "Lexeme List:\n");
	int i = 0;
	for ( i = 0; i < numTokens; i++){
        switch (b->buf[i].type)
        {
            case nulsym: fputs("nulsym ", fout); break;
            case identsym: fprintf(fout, "identsym %s ", b->buf[i].lexeme); break;
            case numbersym: fprintf(fout, "numbersym %s ", b->buf[i].lexeme); break;
            case plussym: fputs("plussym ", fout); break;
            case minussym: fputs("minussym ", fout); break;
            case multsym: fputs("multsym ", fout); break;
            case slashsym: fputs("slashsym ", fout); break;
            case oddsym: fputs("oddsym ", fout); break;
            case eqsym: fputs("eqsym ", fout); break;
            case neqsym: fputs("neqsym ", fout); break;
            case lessym: fputs("lessym ", fout); break;
            case leqsym: fputs("leqsym ", fout); break;
            case gtrsym: fputs("gtrsym ", fout); break;
            case geqsym: fputs("geqsym ", fout); break;
            case lparentsym: fputs("lparentsym ", fout); break;
            case rparentsym: fputs("rparentsym ", fout); break;
            case commasym: fputs("commasym ", fout); break;
            case semicolonsym: fputs("semicolonsym ", fout); break;
            case periodsym: fputs("periodsym ", fout); break;
            case becomessym: fputs("becomessym ", fout); break;
            case beginsym: fputs("beginsym ", fout); break;
            case endsym: fputs("endsym ", fout); break;
            case ifsym: fputs("ifsym ", fout); break;
            case thensym: fputs("thensym ", fout); break;
            case whilesym: fputs("whilesym ", fout); break;
            case dosym: fputs("dosym ", fout); break;
            case callsym: fputs("callsym ", fout); break;
            case constsym: fputs("constsym ", fout); break;
            case intsym: fputs("intsym ", fout); break;
            case procsym: fputs("procsym ", fout); break;
            case outsym: fputs("outsym ", fout); break;
            case insym: fputs("insym ", fout); break;
            case elsesym: fputs("elsesym ", fout); break;
        }
	}
}
//end printTokenList
