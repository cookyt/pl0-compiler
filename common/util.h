#ifndef UTIL_H_
#define UTIL_H_

#include <stdlib.h>

/**
 * Utility, malloc with a NULL check to mitigate allocation errors.
 *
 * @param size the size (in bytes) of the memory to allocate.
 *
 * @return a pointer to the beginning of the allocated memory block.
 **/
void *smalloc(size_t size);

#endif /* UTIL_H_ */
