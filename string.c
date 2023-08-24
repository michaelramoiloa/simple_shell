#include "shell.h"

/**
 * customStrlen - returns the length of a string
 * @str: the string whose length to check
 *
 * Return: integer length of string
 */
int customStrlen(char *str)
{
    int length = 0;

    if (!str)
        return 0;

    while (*str++)
        length++;

    return length;
}

/**
 * customStrcmp - performs lexicographic comparison of two strings.
 * @str1: the first string
 * @str2: the second string
 *
 * Return: negative if str1 < str2, positive if str1 > str2, zero if str1 == str2
 */
int customStrcmp(char *str1, char *str2)
{
    while (*str1 && *str2)
    {
        if (*str1 != *str2)
            return (*str1 - *str2);

        str1++;
        str2++;
    }

    if (*str1 == *str2)
        return 0;
    else
        return (*str1 < *str2 ? -1 : 1);
}

/**
 * customStartsWith - checks if needle starts with haystack
 * @haystack: string to search
 * @needle: the substring to find
 *
 * Return: address of next char of haystack or NULL
 */
char *customStartsWith(const char *haystack, const char *needle)
{
    while (*needle)
    {
        if (*needle++ != *haystack++)
            return NULL;
    }

    return (char *)haystack;
}

/**
 * customStrcat - concatenates two strings
 * @destination: the destination buffer
 * @source: the source buffer
 *
 * Return: pointer to destination buffer
 */
char *customStrcat(char *destination, char *source)
{
    char *result = destination;

    while (*destination)
        destination++;

    while (*source)
    {
        *destination++ = *source++;
    }

    *destination = *source;
    return result;
}

