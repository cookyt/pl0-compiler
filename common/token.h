#ifndef TOKEN_H_
#define TOKEN_H_

/* Max size of an identifier + 1 */
#define IDENTIFIER_SIZE 12

/* Max size of a number literal + 1 */
#define NUMBER_SIZE 6

/**
 * Encapsulates a token of the language.
 *
 * type The integral type of the token as defined in the token_type table
 * lexeme The string which generated this token
 **/
typedef struct Token_
{
	int type;
	char lexeme[IDENTIFIER_SIZE];
    int line; //line where found
    int col;  //column where found
} Token;

/**
 * This table defines the numeric values of tokens in a program. The following
 * is a rundown of acceptable symbols in a program.
 *
 * Reserved Words:
 *   const      constsym
 *   int        intsym
 *   procedure  procsym
 *   call       callsym
 *   begin      beginsym
 *   end        endsym
 *   if         ifsym
 *   then       thensym
 *   else       elsesym
 *   while      whilesym
 *   do         dosym
 *   in         intsym
 *   out        outsym
 *   odd        oddsym
 *
 * Single-char tokens:
 *   + plussym
 *   - minussym
 *   * multsym
 *   / slashsym
 *   ( lparentsym
 *   ) rparentsym
 *   = eqsym
 *   , commasym
 *   . periodsym
 *   < lessym
 *   > gtrsym
 *   ; semicolonsym
 *
 * Multi-char tokens:
 *   <> neqsym
 *   <= leqsym
 *   >= geqsym
 *   := becomessym
 *
 * Identifiers:
 *   L(L|D){0,10}   identsym
 * 
 * Numbers:
 *   D{1,5}         numbersym
 * 
 * Whitespace (not tokenized):
 *   tab     '\t'
 *   space   ' '
 *   newline '\n'
 *
 * Comments (not tokenized):
 *   Ascii C-style: / *   * /
 *   (extra spaces here to not break comment)
 **/
typedef enum {
    nulsym=1,   identsym,    numbersym,   plussym,     minussym,  multsym,
    slashsym,   oddsym,      eqsym,       neqsym,      lessym,    leqsym,
    gtrsym,     geqsym,      lparentsym,  rparentsym,  commasym,  semicolonsym,
    periodsym,  becomessym,  beginsym,    endsym,      ifsym,     thensym,
    whilesym,   dosym,       callsym,     constsym,    intsym,    procsym,
    outsym,     insym,       elsesym
} TokenType;

#endif /* ifndef TOKEN_H_ */
