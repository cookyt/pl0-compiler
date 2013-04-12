/**
 * Carlos Valera-Leon 
 * PID: c3151942
 *
 * Defines functions and macros for the purpose of printing user and program
 * errors, warnings, and messages.
 *
 * MSG
 * ASSERT(EXPR) - if EXPR evaluates to 0 (false) the program is killed
 *
 **/

#ifndef DEBUG_H_
#define DEBUG_H_

#include <stdio.h>
#include <stdlib.h>


// when the DEBUG macro is defined, warnings and errors print the line number
// and file where they occurred. Some warnings are surrounded in 'ifdef DEBUG'
// and only print if this flag is defined.
// #define DEBUG

// The ENABLE_WARNING_COLORS macro defines whether warnings and errors display
// with a brightly colored message (only on available on ANSI terms)
// #define ENABLE_WARNING_COLORS 

#if defined WIN32 || defined _WIN32 
# undef ENABLE_WARNING_COLORS
#endif

typedef enum WarningCodes_
{
    WARN_HASH_STRING_TOO_LONG   = -2,
    WARN_INC_BUFFER_SIZE        = -1,
    WARN_UNDEFINED              = 0,
    WARN_UNTERMINATED_COMMENT   = 1
} WarningCodes;

typedef enum ErrorCodes_
{
    ERR_POP_FROM_EMPTY_BUFFER   = -3,
    ERR_READ_END_OF_BUFFER      = -2,
    ERR_ASSERT                  = -1,
    ERR_UNDEFINED               = 0,
    ERR_FILE                    = 1,
    ERR_ILLIGAL_CHAR            = 2,
    ERR_BAD_IDENTIFIER          = 3,
    ERR_ALLOC                   = 4,
    ERR_INVALID_INSTRUCION      = 5,
    ERR_CODE_TOO_LARGE          = 6,
    ERR_RUNTIME_STACK_OVERFLOW  = 7,
    ERR_NO_SUCH_SYMBOL          = 8,
    ERR_BAD_ASSIGNMENT          = 9,
    ERR_SYMBOL_REDEFINITION     = 10,
    ERR_NO_SUCH_PROC            = 11,
    ERR_PROC_NOT_FACTOR         = 12
} ErrorCodes;

// prints a message to stderr (for quick prototyping only)
#define MSG(...)                                       \
do                                                     \
{                                                      \
    fprintf(stderr, __VA_ARGS__);                      \
    fflush(stderr);                                    \
}                                                      \
while(0)

// will kill program if the expression
// EXPR evaluates to false (0)
#define ASSERT(EXPR)                                           \
do                                                             \
{                                                              \
    if ((EXPR) == 0) die(ERR_ASSERT, "Assert failed: " #EXPR); \
}                                                              \
while(0)


/* Silence the warnings given by warning() */
extern int _DEBUG_no_warnings;

/* Silence error messages given by die() and ASSERT */
extern int _DEBUG_no_errors;

/* NOTE warnings and errors with codes less than 0 only print if the DEBUG
 * preprocessor variable is defined; however, errors still kill the program
 * regardless.
 **/
void warning(int code, char *message);
void die(int code, char *message);

#endif // ifndef DEBUG_H_
