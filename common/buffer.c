#include <string.h>

#include "debug.h"
#include "util.h"
#include "buffer.h"

/**
 * Allocates space for a character buffer.
 *
 * @return A pointer to the new Buffer. This buffer must later be freed with
 * the function freeBuffer.
 **/
Buffer *createBuffer()
{
    Buffer *ret = (Buffer *) malloc(sizeof(Buffer));
    ret->buf = (char *)smalloc(sizeof(char)*BASE_BUFFER_SIZE);
    ret->max_length = BASE_BUFFER_SIZE;
    emptyBuffer(ret);
    return ret;
}

/**
 * Frees the space allocated for a character buffer.
 *
 * @param b the Buffer to free.
 **/
void freeBuffer(Buffer *b)
{
    free(b->buf);
    free(b);
}

/**
 * Adds a character to a buffer. It includes bound checking to grow the buffer
 * as needed.
 *
 * @param c the character to add
 * @param b the buffer to add it to (pass by pointer)
 **/
void buf_putc(char c, Buffer *b)
{
    // It's length+1 to include the NULL byte at the end of the string.
    if (b->length+1 >= b->max_length)
    {
        b->max_length *= 2;
        char *newString = (char *)smalloc(sizeof(char)*(b->max_length));
        memcpy(newString, b->buf, sizeof(char)*(b->length+1));
        free(b->buf);
        b->buf = newString;
#ifdef DEBUG
        char message[120];
        sprintf(message, "Needed to increase the internal size of "
                "the buffer to (%d).\n", b->max_length);
        warning(WARN_INC_BUFFER_SIZE, message);
#endif
    }

    b->buf[b->length] = c;
    b->length += 1;
    b->buf[b->length] = '\0';
}

/**
 * Appends the contents of the src Buffer to the end of the dest Buffer,
 * and stores the result in dest.  Buffers are grown as needed.
 *
 * @param c the character to add
 **/
void buf_cat(Buffer *dest, Buffer *src)
{
    int target_len = (src->length + dest->length + 1);
    if (target_len > dest->max_length)
    {
        while (target_len > dest->max_length)
        {
            dest->max_length *= 2;
        }
        char *newString = (char *)smalloc(sizeof(char)*(dest->max_length));
        memcpy(newString, dest->buf, sizeof(char)*(dest->length+1));
        free(dest->buf);
        dest->buf = newString;
#ifdef DEBUG
        char message[120];
        sprintf(message, "Needed to increase the internal size of "
                "the buffer to (%d).\n", dest->max_length);
        warning(WARN_INC_BUFFER_SIZE, message);
#endif
    }
    strcat(dest->buf, src->buf);
    dest->length += src->length;
}

/**
 * Empties a given Buffer, leaving it with an empty string.
 *
 * @param b The buffer to empty.
 **/
void emptyBuffer(Buffer *b)
{
    b->length = 0;
    b->buf[0] = '\0';
}

