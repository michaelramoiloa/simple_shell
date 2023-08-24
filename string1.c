#include "shell.h"

/**
 * customStrcpy - copies a string
 * @destination: the destination
 * @source: the source
 *
 * Return: pointer to destination
 */
char *customStrcpy(char *destination, const char *source)
{
	int index = 0;

	if (destination == source || source == NULL)
		return destination;

	while (source[index])
	{
		destination[index] = source[index];
		index++;
	}
	destination[index] = '\0';
	return destination;
}

/**
 * customStrdup - duplicates a string
 * @str: the string to duplicate
 *
 * Return: pointer to the duplicated string
 */
char *customStrdup(const char *str)
{
	int length = 0;
	char *result;

	if (str == NULL)
		return NULL;

	while (*str++)
		length++;

	result = malloc(sizeof(char) * (length + 1));

	if (!result)
		return NULL;

	for (length++; length--;)
		result[length] = *--str;

	return result;
}

/**
 * customPuts - prints an input string
 * @str: the string to be printed
 *
 * Return: Nothing
 */
void customPuts(char *str)
{
	int index = 0;

	if (!str)
		return;

	while (str[index] != '\0')
	{
		_putchar(str[index]);
		index++;
	}
}

/**
 * customPutchar - writes the character c to stdout
 * @c: The character to print
 *
 * Return: On success 1.
 * On error, -1 is returned, and errno is set appropriately.
 */
int customPutchar(char c)
{
	static int currentIndex;
	static char buffer[WRITE_BUF_SIZE];

	if (c == BUF_FLUSH || currentIndex >= WRITE_BUF_SIZE)
	{
		write(1, buffer, currentIndex);
		currentIndex = 0;
	}

	if (c != BUF_FLUSH)
		buffer[currentIndex++] = c;

	return 1;
}

