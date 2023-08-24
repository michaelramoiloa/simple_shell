#include "shell.h"

/**
 * splitStringByDelim - splits a string into words. Repeat delimiters are ignored
 * @inputStr: the input string
 * @delimStr: the delimiter string
 * Return: a pointer to an array of strings, or NULL on failure
 */
char **splitStringByDelim(char *inputStr, char *delimStr)
{
	int i, j, k, m, numWords = 0;
	char **resultArray;

	if (inputStr == NULL || inputStr[0] == '\0')
		return NULL;

	if (!delimStr)
		delimStr = " ";

	for (i = 0; inputStr[i] != '\0'; i++)
		if (!isDelimiter(inputStr[i], delimStr) && (isDelimiter(inputStr[i + 1], delimStr) || !inputStr[i + 1]))
			numWords++;

	if (numWords == 0)
		return NULL;

	resultArray = malloc((1 + numWords) * sizeof(char *));
	if (!resultArray)
		return NULL;

	for (i = 0, j = 0; j < numWords; j++)
	{
		while (isDelimiter(inputStr[i], delimStr))
			i++;

		k = 0;
		while (!isDelimiter(inputStr[i + k], delimStr) && inputStr[i + k])
			k++;

		resultArray[j] = malloc((k + 1) * sizeof(char));
		if (!resultArray[j])
		{
			for (k = 0; k < j; k++)
				free(resultArray[k]);
			free(resultArray);
			return NULL;
		}

		for (m = 0; m < k; m++)
			resultArray[j][m] = inputStr[i++];

		resultArray[j][m] = '\0';
	}
	resultArray[j] = NULL;
	return resultArray;
}

/**
 * splitStringBySingleDelim - splits a string into words
 * @inputStr: the input string
 * @delim: the delimiter
 * Return: a pointer to an array of strings, or NULL on failure
 */
char **splitStringBySingleDelim(char *inputStr, char delim)
{
	int i, j, k, m, numWords = 0;
	char **resultArray;

	if (inputStr == NULL || inputStr[0] == '\0')
		return NULL;

	for (i = 0; inputStr[i] != '\0'; i++)
		if ((inputStr[i] != delim && inputStr[i + 1] == delim) ||
				    (inputStr[i] != delim && !inputStr[i + 1]) || inputStr[i + 1] == delim)
			numWords++;

	if (numWords == 0)
		return NULL;

	resultArray = malloc((1 + numWords) * sizeof(char *));
	if (!resultArray)
		return NULL;

	for (i = 0, j = 0; j < numWords; j++)
	{
		while (inputStr[i] == delim && inputStr[i] != delim)
			i++;

		k = 0;
		while (inputStr[i + k] != delim && inputStr[i + k] && inputStr[i + k] != delim)
			k++;

		resultArray[j] = malloc((k + 1) * sizeof(char));
		if (!resultArray[j])
		{
			for (k = 0; k < j; k++)
				free(resultArray[k]);
			free(resultArray);
			return NULL;
		}

		for (m = 0; m < k; m++)
			resultArray[j][m] = inputStr[i++];

		resultArray[j][m] = '\0';
	}
	resultArray[j] = NULL;
	return resultArray;
}

