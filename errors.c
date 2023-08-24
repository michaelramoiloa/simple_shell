#include "shell.h"

/**
 * customPutStr - prints an input string
 * @str: the string to be printed
 * 
 * Returns: Nothing
 */
void customPutStr(char *str)
{
    int index = 0;

    if (!str)
        return;

    while (str[index] != '\0')
    {
        customPutChar(str[index]);
        index++;
    }
}

/**
 * customPutChar - writes the character c to stderr
 * @c: The character to print
 * 
 * Returns: On success 1.
 * On error, -1 is returned, and errno is set appropriately.
 */
int customPutChar(char c)
{
    static int i;
    static char buf[WRITE_BUF_SIZE];

    if (c == BUF_FLUSH || i >= WRITE_BUF_SIZE)
    {
        write(2, buf, i);
        i = 0;
    }
    if (c != BUF_FLUSH)
        buf[i++] = c;
    
    return 1;
}

/**
 * customPutFd - writes the character c to given fd
 * @c: The character to print
 * @fd: The file descriptor to write to
 * 
 * Returns: On success 1.
 * On error, -1 is returned, and errno is set appropriately.
 */
int customPutFd(char c, int fd)
{
    static int i;
    static char buf[WRITE_BUF_SIZE];

    if (c == BUF_FLUSH || i >= WRITE_BUF_SIZE)
    {
        write(fd, buf, i);
        i = 0;
    }
    if (c != BUF_FLUSH)
        buf[i++] = c;

    return 1;
}

/**
 * customPutStrFd - prints an input string
 * @str: the string to be printed
 * @fd: the file descriptor to write to
 * 
 * Returns: the number of characters put
 */
int customPutStrFd(char *str, int fd)
{
    int index = 0;
    int charCount = 0;

    if (!str)
        return 0;

    while (str[index])
    {
        charCount += customPutFd(str[index], fd);
        index++;
    }
    
    return charCount;
}

