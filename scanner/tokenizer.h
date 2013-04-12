#ifndef TOKENIZER_H_
#define TOKENIZER_H_

#define TOKENIZER_ALIAS_READ_WRITE

typedef struct Buffer_ Buffer;
typedef struct TokenBuffer_ TokenBuffer;
typedef struct FileBuffer_ FileBuffer;

TokenBuffer *tokenize(FileBuffer *file_buffer);
int isReserved(char *word);
int getSlash(Buffer *b, FileBuffer *file_buffer);
int getIdentifier(Buffer *b, FileBuffer *file_buffer);
int getNumber(Buffer *b, FileBuffer *file_buffer);
int getLessThan(Buffer *b, FileBuffer *file_buffer);
int getGreaterThan(Buffer *b, FileBuffer *file_buffer);
int getColon(Buffer *b, FileBuffer *file_buffer);

#endif
