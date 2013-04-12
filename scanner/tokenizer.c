#include <string.h>
#include <stdio.h>
#include <ctype.h>

#include "debug.h"
#include "tokenizer.h"
#include "token.h"
#include "buffer.h"
#include "filebuffer.h"
#include "tokenbuffer.h"

/**
 * Creates a tokenization of a given file and stores the contents of the file
 * in a buffer.
 *
 * @param file_buffer Pointer to the buffer where the file should be stored.
 *
 * @return a pointer to a TokenBuffer holding the tokenization of this program.
 **/
TokenBuffer *tokenize(FileBuffer *file_buffer)
{
    Buffer *main_buffer = createBuffer();
    TokenBuffer *token_buffer = createTokenBuffer();

    for(;;)
    {
        char peek = fbuf_getc(file_buffer);
        int line = file_buffer->line;
        int col = file_buffer->col;
        int type = 0;

        if(fbuf_eof(file_buffer))
            break;

        if(peek == ' ' || peek == '\t' || peek == '\n' || peek == '\r')
        {
            continue;
        }
        else if (peek == '+')
        {
            buf_putc(peek, main_buffer);
            type = plussym;
        }
        else if (peek == '-')
        {
            buf_putc(peek, main_buffer);
            type = minussym;
        }
        else if (peek == '*')
        {
            buf_putc(peek, main_buffer);
            type = multsym;
        }
        else if (peek == '(')
        {
            buf_putc(peek, main_buffer);
            type = lparentsym;
        }
        else if (peek == ')')
        {
            buf_putc(peek, main_buffer);
            type = rparentsym;
        }
        else if (peek == '=')
        {
            buf_putc(peek, main_buffer);
            type = eqsym;
        }
        else if (peek == ',')
        {
            buf_putc(peek, main_buffer);
            type = commasym;
        }
        else if (peek == '.')
        {
            buf_putc(peek, main_buffer);
            type = periodsym;
        }
        else if (peek == ';')
        {
            buf_putc(peek, main_buffer);
            type = semicolonsym;
        }

        /* MULTI-CHAR TOKENS */
        else if (peek == '<')
        {
            buf_putc(peek, main_buffer);
            type = getLessThan(main_buffer, file_buffer);
        }
        else if (peek == '>')
        {
        	buf_putc(peek, main_buffer);
        	type = getGreaterThan(main_buffer, file_buffer);
        }
        else if (peek == ':')
        {
            buf_putc(peek, main_buffer);
            type = getColon(main_buffer, file_buffer);
        }
        else if (peek == '/')
        {
            buf_putc(peek, main_buffer);
            type = getSlash(main_buffer, file_buffer);
        }
        else if (isalpha(peek))
        {
            buf_putc(peek, main_buffer);
            type = getIdentifier(main_buffer, file_buffer);
        }
        else if (isdigit(peek))
        {
            buf_putc(peek, main_buffer);
            type = getNumber(main_buffer, file_buffer);
        }
        else
        {
            char message[40];
            sprintf(message, "Stray character 0x%02x", (int)peek);
            die(ERR_ILLIGAL_CHAR, message);
        }

        if (type != 0)
        {
            insert_token(token_buffer, type, main_buffer->buf, line, col);
            emptyBuffer(main_buffer);
        }
    }

    freeBuffer(main_buffer);
    return token_buffer;
}

/**
 * Checks to see whether a given string is a reserved word in PL0
 *
 * @param word the string to check for reserved words.
 *
 * @return The token_type of the word if it is reserved or 0 if it is not.
 **/
int isReserved(char *word)
{
    if (strcmp(word, "const") == 0) 
        return constsym;
    if (strcmp(word, "int") == 0) 
        return intsym;
    if (strcmp(word, "procedure") == 0) 
        return procsym;
    if (strcmp(word, "call") == 0) 
        return callsym;
    if (strcmp(word, "begin") == 0) 
        return beginsym;
    if (strcmp(word, "end") == 0) 
        return endsym;
    if (strcmp(word, "if") == 0) 
        return ifsym;
    if (strcmp(word, "then") == 0) 
        return thensym;
    if (strcmp(word, "else") == 0) 
        return elsesym;
    if (strcmp(word, "while") == 0) 
        return whilesym;
    if (strcmp(word, "do") == 0) 
        return dosym;
    if (strcmp(word, "in") == 0) 
        return insym;
    if (strcmp(word, "out") == 0) 
        return outsym;
    if (strcmp(word, "odd") == 0) 
        return oddsym;

#ifdef TOKENIZER_ALIAS_READ_WRITE
    // Due to a typo on the specifications, 'read' and 'write' are aliases for
    // 'in' and 'out'
    if (strcmp(word, "read") == 0)  
        return insym;               
    if (strcmp(word, "write") == 0) 
        return outsym;
#endif

    return 0;
}

/**
 * Checks whether a forward slash character corresponds to a stand-alone slash
 * or the beginning of a comment. If it is a comment, it advances the file
 * buffer until just past the commented area and empties the buffer. Assumed
 * the slash has already been placed in the Buffer.
 *
 * @param b The buffer which holds the slash (it's emptied if there turns out
 *   to be a comment
 * @param file_buffer The file buffer to take characters from.
 *
 * @return slashsym if the character is a standalone slash, 0 if it is a
 *   comment.
 *
 * @warning If the comment is unterminated.
 **/
int getSlash(Buffer *b, FileBuffer *file_buffer)
{
    char peek = fbuf_getc(file_buffer);
    if (peek == '*')
    {
        for(;;)
        {
            peek = fbuf_getc(file_buffer);
            if (fbuf_eof(file_buffer))
            {
                warning(1, "Unterminated comment");
                break;
            }

            if (peek == '*')
            {
                peek = fbuf_getc(file_buffer);
                if (fbuf_eof(file_buffer))
                {
                    warning(1, "Unterminated comment");
                    break;
                }

                if (peek == '/')
                {
                    break;
                }
                else 
                    fbuf_ungetc(peek, file_buffer);
            }
        }
        emptyBuffer(b);
        return 0;
    }
    else
    {
        fbuf_ungetc(peek, file_buffer);
        return slashsym;
    }
}

/**
 * Tries to break an identifier from a file buffer and store it in a buffer.
 * Assumes the first character of the identifier is already in the buffer. An
 * identifier has only letters and digits and starts with a letter.
 *
 * @param b The buffer to store the identifier in.
 * @param file_buffer The file buffer to take characters from.
 *
 * @return identsym if it is a regular identifier, or the corresponding
 *   token_type if it is a reserved word.
 *
 * @error if the identifier is longer than IDENT_SIZE-1
 **/
int getIdentifier(Buffer *b, FileBuffer *file_buffer)
{
    for (;;)
    {
        char peek = fbuf_getc(file_buffer);
        
        if (fbuf_eof(file_buffer))
            break;

        if (isalpha(peek) || isdigit(peek))
        {
            buf_putc(peek, b);
        }
        else
        {
            fbuf_ungetc(peek, file_buffer);
            break;
        }
    }

    if (b->length >= IDENTIFIER_SIZE)
    {
        char *message = (char *)smalloc(b->length + 100);
        sprintf(message, "Identifier '%s' of length %d exceeds "
                "the max allowable size for an identifier, %d", 
                b->buf, b->length, IDENTIFIER_SIZE-1);
        die(ERR_BAD_IDENTIFIER, message);
    }

    int reserved = isReserved(b->buf);

    if (reserved != 0)
        return reserved;
    else
        return identsym;
}

/**
 * Tries to break a number from a file and store it in a buffer. Assumes
 * the first digit of the number is already in the buffer.
 *
 * @param b The buffer to store the number in.
 * @param file_buffer The file buffer to take digits from.
 *
 * @return numbersym from the token_type table
 *
 * @error if the number is longer than NUMBER_SIZE-1
 **/
int getNumber(Buffer *b, FileBuffer *file_buffer)
{
    for (;;)
    {
        char peek = fbuf_getc(file_buffer);

        if (fbuf_eof(file_buffer))
            break;

        if(isdigit(peek))
        {
            buf_putc(peek, b);
        }
        else if (isalpha(peek))
        {
            die(ERR_BAD_IDENTIFIER, "Identifier must start with a letter.");
        }
        else
        {
            fbuf_ungetc(peek, file_buffer);
            break;
        }
    }

    if (b->length >= NUMBER_SIZE)
    {
        char *message = (char *)smalloc(b->length + 100);
        sprintf(message, "Number '%s' of length %d exceeds "
                "the max allowable size for an number, %d", 
                b->buf, b->length, NUMBER_SIZE-1);
        die(ERR_BAD_IDENTIFIER, message);
    }

    return numbersym;
}

int getLessThan(Buffer* b, FileBuffer* file_buffer)
{
	char peek = fbuf_getc(file_buffer);
	if (peek == '='){
		buf_putc(peek, b);
		return leqsym;
	}
	else if (peek == '>'){
		buf_putc(peek, b);
		return neqsym;
	}
	else{
		fbuf_ungetc(peek, file_buffer);
		return lessym;
	}
}


int getGreaterThan(Buffer* b, FileBuffer* file_buffer){

	char peek = fbuf_getc(file_buffer);
	if (peek == '='){
		buf_putc(peek, b);
		return geqsym;
	}
	else{
		fbuf_ungetc(peek, file_buffer);
		return gtrsym;
	}

}

int getColon(Buffer* b, FileBuffer* file_buffer){

	char peek = fbuf_getc(file_buffer);

    if (peek == '='){
    	buf_putc(peek, b);
    }
    else
    	die(ERR_ILLIGAL_CHAR, "Expected '=' after ':'.");

    return becomessym;
}

