#include "shell.h"

/**
 * isChainDelimiter - tests if the current char in buffer is a chain delimiter
 * @info: the parameter struct
 * @buffer: the char buffer
 * @pos: address of current position in buffer
 *
 * Return: 1 if chain delimiter, 0 otherwise
 */
int isChainDelimiter(info_t *info, char *buffer, size_t *pos)
{
	size_t currentPos = *pos;

	switch (buffer[currentPos]) {
		case '|':
			if (buffer[currentPos + 1] == '|') {
				buffer[currentPos] = '\0';
				(*pos)++;
				info->cmd_buf_type = CMD_OR;
				return 1;
			}
			break;

		case '&':
			if (buffer[currentPos + 1] == '&') {
				buffer[currentPos] = '\0';
				(*pos)++;
				info->cmd_buf_type = CMD_AND;
				return 1;
			}
			break;

		case ';':
			buffer[currentPos] = '\0'; /* replace semicolon with null */
			info->cmd_buf_type = CMD_CHAIN;
			return 1;

		default:
			return 0;
	}

	return 0;
}

/**
 * checkChain - checks if we should continue chaining based on the last status
 * @info: the parameter struct
 * @buffer: the char buffer
 * @pos: address of current position in buffer
 * @startPos: starting position in buffer
 * @length: length of buffer
 *
 * Return: Void
 */
void checkChain(info_t *info, char *buffer, size_t *pos, size_t startPos, size_t length)
{
	size_t currentPos = *pos;

	switch (info->cmd_buf_type) {
		case CMD_AND:
			if (info->status) {
				buffer[startPos] = '\0';
				currentPos = length;
			}
			break;

		case CMD_OR:
			if (!info->status) {
				buffer[startPos] = '\0';
				currentPos = length;
			}
			break;
	}

	*pos = currentPos;
}

/**
 * replaceAlias - replaces aliases in the tokenized string
 * @info: the parameter struct
 *
 * Return: 1 if replaced, 0 otherwise
 */
int replaceAlias(info_t *info)
{
	int i;
	list_t *node;
	char *p;

	for (i = 0; i < 10; i++) {
		node = nodeStartsWith(info->alias, info->argv[0], '=');
		if (!node)
			return 0;

		free(info->argv[0]);
		p = _strchr(node->str, '=');
		if (!p)
			return 0;

		p = _strdup(p + 1);
		if (!p)
			return 0;

		info->argv[0] = p;
	}
	return 1;
}

/**
 * replaceVars - replaces variables in the tokenized string
 * @info: the parameter struct
 *
 * Return: 1 if replaced, 0 otherwise
 */
int replaceVars(info_t *info)
{
	int i = 0;
	list_t *node;

	for (i = 0; info->argv[i]; i++) {
		if (info->argv[i][0] != '$' || !info->argv[i][1])
			continue;

		if (!_strcmp(info->argv[i], "$?")) {
			replaceString(&(info->argv[i]),
						  _strdup(convertNumber(info->status, 10, 0)));
			continue;
		}

		if (!_strcmp(info->argv[i], "$$")) {
			replaceString(&(info->argv[i]),
						  _strdup(convertNumber(getpid(), 10, 0)));
			continue;
		}

		node = nodeStartsWith(info->env, &info->argv[i][1], '=');
		if (node) {
			replaceString(&(info->argv[i]),
						  _strdup(_strchr(node->str, '=') + 1));
			continue;
		}

		replaceString(&info->argv[i], _strdup(""));
	}

	return 0;
}

/**
 * replaceString - replaces string
 * @oldStr: address of old string
 * @newStr: new string
 *
 * Return: 1 if replaced, 0 otherwise
 */
int replaceString(char **oldStr, char *newStr)
{
	free(*oldStr);
	*oldStr = newStr;
	return 1;
}

