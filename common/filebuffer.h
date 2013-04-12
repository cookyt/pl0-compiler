/**
 * A library which aids in reading an entire file into an internal buffer and
 * reading from it.
 **/

#ifndef FILEBUFFER_H_
#define FILEBUFFER_H_

#include <stdio.h>

typedef struct Buffer_ Buffer;

typedef struct FileBuffer_
{
    Buffer *buf;
    int pos;
    int line;
    int col;
} FileBuffer;

FileBuffer *createFileBuffer(FILE *fin);
void freeFileBuffer(FileBuffer *fil);

/**
 * works just like fgetc
 **/
int fbuf_getc(FileBuffer *fil);

/**
 * works just like ungetc
 **/
int fbuf_ungetc(int c, FileBuffer *fil);

/**
 * works just like feof
 **/
int fbuf_eof(FileBuffer *fil);

/**
 * Prints the contents of the file buffer to a FILE
 **/
void printFileBuffer(FileBuffer *fil, FILE *fout);

#endif /* FILEBUFFER_H_ */
