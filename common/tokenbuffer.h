#ifndef TOKENBUFFER_H_
#define TOKENBUFFER_H_

#define BASE_TOKEN_BUFFER_SIZE 512

typedef struct Token_ Token;

/**
 * A buffer for Tokens which grows automatically.
 *
 * length       The number of tokens in this buffer.
 * max_length   The most Tokens this buffer can currently hold.
 * buf          The contents of the buffer (always NULL-terminated)
 **/
typedef struct TokenBuffer_
{
    int length;
    int max_length;
    Token *buf;
    int pos;
} TokenBuffer;

TokenBuffer *createTokenBuffer();
void freeTokenBuffer(TokenBuffer *b);
void insert_token(TokenBuffer *b, int type, char *lexeme, int line, int col);
void nextToken(TokenBuffer *tokens);
Token getCurrentToken(TokenBuffer *tokens);

#endif
