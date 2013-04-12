#include "util.h"
#include "debug.h"

void *smalloc(size_t size)
{
    void *ptr = malloc(size);
    if (ptr == NULL)
        die(ERR_ALLOC, "Allocation error.");

    return ptr;
}

