#include "shell.h"

/**
 * retrieveEnvironment - Returns a copy of the environment strings.
 * @shellInfo: Structure containing relevant information.
 * Return: Pointer to the environment strings.
 */
char **retrieveEnvironment(ShellInfo *shellInfo)
{
    if (!shellInfo->environment || shellInfo->isEnvChanged)
    {
        shellInfo->environment = convertListToStrings(shellInfo->envList);
        shellInfo->isEnvChanged = 0;
    }

    return (shellInfo->environment);
}

/**
 * removeEnvVariable - Delete an environment variable.
 * @shellInfo: Structure containing relevant information.
 * @variable: The environment variable to be deleted.
 * Return: 1 if deleted, 0 otherwise.
 */
int removeEnvVariable(ShellInfo *shellInfo, char *variable)
{
    List *node = shellInfo->envList;
    size_t index = 0;
    char *ptr;

    if (!node || !variable)
        return (0);

    while (node)
    {
        ptr = startsWith(node->str, variable);
        if (ptr && *ptr == '=')
        {
            shellInfo->isEnvChanged = deleteNodeAtIndex(&(shellInfo->envList), index);
            index = 0;
            node = shellInfo->envList;
            continue;
        }
        node = node->next;
        index++;
    }
    return (shellInfo->isEnvChanged);
}

/**
 * setEnvVariable - Initialize a new environment variable or modify an existing one.
 * @shellInfo: Structure containing relevant information.
 * @variable: The environment variable property.
 * @value: The environment variable value.
 * Return: Always 0.
 */
int setEnvVariable(ShellInfo *shellInfo, char *variable, char *value)
{
    char *buffer = NULL;
    List *node;
    char *ptr;

    if (!variable || !value)
        return (0);

    buffer = malloc(getLength(variable) + getLength(value) + 2);
    if (!buffer)
        return (1);

    copyString(buffer, variable);
    concatenateString(buffer, "=");
    concatenateString(buffer, value);

    node = shellInfo->envList;
    while (node)
    {
        ptr = startsWith(node->str, variable);
        if (ptr && *ptr == '=')
        {
            free(node->str);
            node->str = buffer;
            shellInfo->isEnvChanged = 1;
            return (0);
        }
        node = node->next;
    }

    addNodeAtEnd(&(shellInfo->envList), buffer, 0);
    free(buffer);
    shellInfo->isEnvChanged = 1;
    return (0);
}

