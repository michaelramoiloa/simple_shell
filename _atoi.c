#include "shell.h"

/**
 * checkInteractiveMode - checks if the shell is in interactive mode
 * @shellInfo: struct containing shell information
 *
 * Returns: 1 if in interactive mode, 0 otherwise
 */
int checkInteractiveMode(ShellInfo *shellInfo)
{
    return (isatty(STDIN_FILENO) && shellInfo->readFd <= 2);
}

/**
 * isDelimiter - checks if a character is a delimiter
 * @character: the character to check
 * @delimiterStr: the delimiter string
 * 
 * Returns: 1 if true, 0 if false
 */
int isDelimiter(char character, char *delimiterStr)
{
    while (*delimiterStr)
    {
        if (*delimiterStr++ == character)
        {
            return 1;
        }
    }
    return 0;
}

/**
 * isAlphabetic - checks if a character is alphabetic
 * @character: the character to check
 * 
 * Returns: 1 if character is alphabetic, 0 otherwise
 */
int isAlphabetic(int character)
{
    if ((character >= 'a' && character <= 'z') || (character >= 'A' && character <= 'Z'))
    {
        return 1;
    }
    else
    {
        return 0;
    }
}

/**
 * customAtoi - converts a string to an integer
 * @inputStr: the string to be converted
 * 
 * Returns: 0 if no numbers in string, converted number otherwise
 */
int customAtoi(char *inputStr)
{
    int index, sign = 1, flag = 0, output;
    unsigned int result = 0;

    for (index = 0; inputStr[index] != '\0' && flag != 2; index++)
    {
        switch (inputStr[index])
        {
            case '-':
                sign *= -1;
                break;
            case '0'...'9':
                flag = 1;
                result *= 10;
                result += (inputStr[index] - '0');
                break;
            default:
                if (flag == 1)
                {
                    flag = 2;
                }
                break;
        }
    }

    if (sign == -1)
    {
        output = -result;
    }
    else
    {
        output = result;
    }

    return output;
}

