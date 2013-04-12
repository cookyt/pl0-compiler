#include <string.h>
#include <stdio.h>

#include "debug.h"
#include "util.h"
#include "filebuffer.h"
#include "buffer.h"

FileBuffer *createFileBuffer(FILE *fin)
{
    Buffer *dest = createBuffer();

    int chars_read;
    while ((chars_read = fread((dest->buf)+dest->length, sizeof(char), 
                              (dest->max_length - dest->length - 1), fin)) > 0)
    {
        dest->length += chars_read;
        if (dest->length >= dest->max_length-1)
        {
            dest->max_length *= 2;
            char *newString = (char *)smalloc(sizeof(char)*(dest->max_length));
            memcpy(newString, dest->buf, sizeof(char)*(dest->length+1));
            free(dest->buf);
            dest->buf = newString;
#ifdef DEBUG
            char message[120];
            sprintf(message, "Needed to increase the internal size of "
                    "the file buffer to (%d).\n", dest->max_length);
            warning(WARN_INC_BUFFER_SIZE, message);
#endif
        }
    }

    dest->buf[dest->length] = '\0';

    FileBuffer *ret = (FileBuffer *)smalloc(sizeof(FileBuffer));
    ret->buf = dest;
    ret->pos = 0;
    ret->line = 1;
    ret->col = 0;
    return ret;
}

void freeFileBuffer(FileBuffer *fil)
{
    freeBuffer(fil->buf);
    free(fil);
}

int fbuf_getc(FileBuffer *fil)
{
    if (fil->pos == fil->buf->length)
        return EOF;

    char c = fil->buf->buf[fil->pos];
    fil->pos += 1;
    if (c == '\n')
    {
        fil->line++;
        fil->col = 0;
    }
    else
    {
        fil->col++;
    }
    return c;
}

int fbuf_ungetc(int c, FileBuffer *fil)
{
    if (fil->pos == 0)
        return EOF;

    fil->pos -= 1;
    fil->buf->buf[fil->pos] = c;
    if (c == '\n')
    {
        fil->line++;
        fil->col = -1;
    }
    else
    {
        fil->col--;
    }
    return c;
}

int fbuf_eof(FileBuffer *fil)
{
    return (fil->pos == fil->buf->length);
}

void printFileBuffer(FileBuffer *fil, FILE *fout)
{
    fwrite(fil->buf->buf, sizeof(fil->buf->buf[0]), fil->buf->length, fout);
}
