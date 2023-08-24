#include "shell.h"

/**
 * fillMemory - Fills memory with a constant byte.
 * @memory: The pointer to the memory area.
 * @value: The byte to fill *memory with.
 * @size: The amount of bytes to be filled.
 * 
 * Return: A pointer to the memory area.
 */
char *fillMemory(char *memory, char value, unsigned int size) {
    unsigned int i;

    for (i = 0; i < size; i++)
        memory[i] = value;

    return memory;
}

/**
 * freeArrayOfStrings - Frees a string of strings.
 * @stringArray: String of strings.
 */
void freeArrayOfStrings(char **stringArray) {
    char **arrayPointer = stringArray;

    if (!stringArray)
        return;

    while (*stringArray)
        free(*stringArray++);

    free(arrayPointer);
}

/**
 * reallocateMemory - Reallocates a block of memory.
 * @pointer: Pointer to the previous dynamically allocated block.
 * @oldSize: Byte size of the previous block.
 * @newSize: Byte size of the new block.
 * 
 * Return: Pointer to the reallocated memory block.
 */
void *reallocateMemory(void *pointer, unsigned int oldSize, unsigned int newSize) {
    char *newPointer;

    if (!pointer)
        return allocateMemory(newSize);

    if (!newSize) {
        free(pointer);
        return NULL;
    }

    if (newSize == oldSize)
        return pointer;

    newPointer = allocateMemory(newSize);

    if (!newPointer)
        return NULL;

    oldSize = oldSize < newSize ? oldSize : newSize;

    while (oldSize--)
        newPointer[oldSize] = ((char *)pointer)[oldSize];

    free(pointer);
    return newPointer;
}

