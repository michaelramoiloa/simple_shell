#include "shell.h"

/**
 * customAtoi - converts a string to an integer
 * @s: the string to be converted
 * 
 * Returns: 0 if no numbers in string, converted number otherwise
 *          -1 on error
 */
int customAtoi(char *s)
{
    int index = 0;
    unsigned long int result = 0;

    if (*s == '+')
        s++;

    for (index = 0;  s[index] != '\0'; index++)
    {
        if (s[index] >= '0' && s[index] <= '9')
        {
            result *= 10;
            result += (s[index] - '0');
            if (result > INT_MAX)
                return (-1);
        }
        else
        {
            return (-1);
        }
    }
    return (result);
}

/**
 * printError - prints an error message
 * @info: the parameter & return info struct
 * @errorType: string containing specified error type
 * 
 * Returns: Nothing
 */
void printError(info_t *info, char *errorType)
{
    _ePuts(info->fileName);
    _ePuts(": ");
    printDecimal(info->lineCount, STDERR_FILENO);
    _ePuts(": ");
    _ePuts(info->argv[0]);
    _ePuts(": ");
    _ePuts(errorType);
}

/**
 * printDecimal - function prints a decimal (integer) number (base 10)
 * @input: the input
 * @fd: the file descriptor to write to
 * 
 * Returns: number of characters printed
 */
int printDecimal(int input, int fd)
{
    int (*customPutCharFunc)(char) = customPutChar;
    int index, charCount = 0;
    unsigned int absoluteValue, current;

    if (fd == STDERR_FILENO)
        customPutCharFunc = _eputchar;

    if (input < 0)
    {
        absoluteValue = -input;
        customPutCharFunc('-');
        charCount++;
    }
    else
    {
        absoluteValue = input;
    }

    current = absoluteValue;
    for (index = 1000000000; index > 1; index /= 10)
    {
        if (absoluteValue / index)
        {
            customPutCharFunc('0' + current / index);
            charCount++;
        }
        current %= index;
    }
    customPutCharFunc('0' + current);
    charCount++;

    return (charCount);
}

/**
 * convertNumber - converter function, a clone of itoa
 * @num: number
 * @base: base
 * @flags: argument flags
 * 
 * Returns: string
 */
char *convertNumber(long int num, int base, int flags)
{
    static char *charArray;
    static char buffer[50];
    char sign = 0;
    char *ptr;
    unsigned long n = num;

    if (!(flags & CONVERT_UNSIGNED) && num < 0)
    {
        n = -num;
        sign = '-';
    }

    charArray = flags & CONVERT_LOWERCASE ? "0123456789abcdef" : "0123456789ABCDEF";
    ptr = &buffer[49];
    *ptr = '\0';

    do  {
        *--ptr = charArray[n % base];
        n /= base;
    } while (n != 0);

    if (sign)
        *--ptr = sign;
    
    return (ptr);
}

/**
 * removeComments - function replaces first instance of '#' with '\0'
 * @buf: address of the string to modify
 * 
 * Returns: Always 0;
 */
void removeComments(char *buf)
{
    int index;

    for (index = 0; buf[index] != '\0'; index++)
    {
        if (buf[index] == '#' && (!index || buf[index - 1] == ' '))
        {
            buf[index] = '\0';
            break;
        }
    }
}

