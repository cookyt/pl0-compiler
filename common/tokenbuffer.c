#include <string.h>

#include "debug.h"
#include "util.h"
#include "tokenbuffer.h"
#include "token.h"

/**
 * Creates space for a TokenBuffer.
 *
 * @return pointer to a new TokenBuffer. This pointer must be deallocated with
 *   the function freeTokenBuffer.
 **/
TokenBuffer *createTokenBuffer()
{
    TokenBuffer *ret = (TokenBuffer *)smalloc(sizeof(TokenBuffer));
    ret->buf = (Token *)smalloc(sizeof(Token)*BASE_TOKEN_BUFFER_SIZE);
    ret->length = 0;
    ret->max_length = BASE_TOKEN_BUFFER_SIZE;
    ret->pos = 0;
    return ret;
}

/**
 * Frees the space allocated to a TokenBuffer.
 *
 * @param b the buffer to free
 **/
void freeTokenBuffer(TokenBuffer *b)
{
    free(b->buf);
    free(b);
}

/**
 * Adds a token to the end of a given TokenBuffer. Grows the buffer as needed.
 *
 * @param b the buffer to add it to (pass by pointer)
 * @param type the type of the new token from the type table
 * @param lexeme the string which produced this token
 **/
void insert_token(TokenBuffer *b, int type, char *lexeme, int line, int col)
{
    //Dynamically grow the buffer's internal array
    //max size doubles every time you hit a wall
    if (b->length >= b->max_length)
    {
        b->max_length *= 2;
        Token *newTokens = (Token *)smalloc(sizeof(Token)*(b->max_length));
        memcpy(newTokens, b->buf, sizeof(Token)*(b->length));
        free(b->buf);
        b->buf = newTokens;

#ifdef DEBUG
        char message[120];
        sprintf(message, "Needed to increase the internal size of "
                "the buffer to (%d).\n", b->max_length);
        warning(WARN_INC_BUFFER_SIZE, message);
#endif
    }

    b->buf[b->length].type = type;
    b->buf[b->length].line = line;
    b->buf[b->length].col = col;
    strcpy(b->buf[b->length].lexeme, lexeme);
    b->length += 1;
}

void nextToken(TokenBuffer *tokens)
{
    if (tokens->pos >= tokens->length)
        die(ERR_READ_END_OF_BUFFER, "Tried reading past end of buffer");
    tokens->pos += 1;
}

Token getCurrentToken(TokenBuffer *tokens)
{
    return tokens->buf[tokens->pos];
}
