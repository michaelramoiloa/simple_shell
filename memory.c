#include "shell.h"

/**
 * freeAndNull - Frees a pointer and sets the address to NULL.
 * @pointer: Address of the pointer to be freed.
 *
 * Return: 1 if freed, otherwise 0.
 */
int freeAndNull(void **pointer) {
    if (pointer && *pointer) {
        free(*pointer);
        *pointer = NULL;
        return 1;
    }
    return 0;
}

