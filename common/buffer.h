#ifndef BUFFER_H_
#define BUFFER_H_

#define BASE_BUFFER_SIZE 256

/**
 * An autosizing buffer for building the lexeme strings.
 *
 * length       The length of the string. Alternatively, the position of the
 *                  first NULL byte in the string.
 * max_length   The most characters this buffer can currently hold.
 * buf          The contents of the buffer (always NULL-terminated)
 **/
typedef struct Buffer_
{
	int length;
    int max_length;
	char *buf;
} Buffer;

void *smalloc(size_t size);

Buffer *createBuffer();
void freeBuffer(Buffer *b);
void buf_putc(char c, Buffer *b);
void buf_cat(Buffer *dest, Buffer *src);
void emptyBuffer(Buffer *b);

#endif
