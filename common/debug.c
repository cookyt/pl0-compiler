#include <stdio.h>
#include <stdlib.h>
#include <limits.h>

#include "debug.h"

/* Silence the warnings given by WARN */
int _DEBUG_no_warnings = 0;

/* Silence error messages (given by DIE and ASSERT) */
int _DEBUG_no_errors = 0;

/* NOTE negative numbers are used for program debuging errors,
 * positive for userspace errors */

/**
 * WARNING codes:
 * 0    undefined
 * -1   increasing buffer size
 * 1    Unterminated comment
 **/
void warning(int code, char *message)
{
    if (!_DEBUG_no_warnings)
    {
        if (code < 0)
        {
#ifdef DEBUG
# ifndef ENABLE_WARNING_COLORS
            fprintf(stderr, "WARNING: #%d %s\n", code, message);
# else
            fprintf(stderr, "\x1b[33m\x1b[1mWARNING: #%d\x1b[0m %s\n",
                    code, message);
# endif
#endif
        }
        else
        {
#ifndef ENABLE_WARNING_COLORS
            fprintf(stderr, "WARNING: #%d %s\n", code, message);
#else
            fprintf(stderr, "\x1b[33m\x1b[1mWARNING: #%d\x1b[0m %s\n",
                    code, message);
#endif
        }
        fflush(stderr);
    }
}

/**
 * ERROR codes:
 * 0    undefined
 * -2   reading past end of buffer
 * -1   assertion failure
 * 1    File error
 * 2    Illigal character or token
 * 3    Bad identifier or literal
 * 4    Allocation error
 * 5    Invalid instruction
 * 6    Code size too large
 * 7    Runtime stack overflow
 * 8    no symbol found
 * 9    bad assignment
 **/
void die(int code, char *message)
{
    if (!_DEBUG_no_errors)
    {
        if (code < 0)
        {
#ifdef DEBUG
# ifndef ENABLE_WARNING_COLORS
            fprintf(stderr, "ERROR: #%d %s\n", code, message);
# else
            fprintf(stderr, "\x1b[31m\x1b[1mERROR: #%d\x1b[0m %s\n",
                    code, message);
# endif
#else
# ifndef ENABLE_WARNING_COLORS
            fprintf(stderr, "ERROR: #0\n");
# else
            fprintf(stderr, "\x1b[31m\x1b[1mERROR: #0\x1b[0m\n");
# endif
#endif
        }
        else
        {
#ifndef ENABLE_WARNING_COLORS
            fprintf(stderr, "ERROR: #%d %s\n", code, message);
#else
            fprintf(stderr, "\x1b[31m\x1b[1mERROR: #%d\x1b[0m %s\n",
                    code, message);
#endif
        }
        fflush(stderr);
    }
    if (code != 0)
        exit(code);
    else
        exit(INT_MAX);
}
