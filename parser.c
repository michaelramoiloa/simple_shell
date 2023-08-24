#include "shell.h"

/**
 * isExecutableCommand - Determines if a file is an executable command.
 * @info: Pointer to the info struct.
 * @filePath: Path to the file.
 *
 * Return: 1 if true, 0 otherwise.
 */
int isExecutableCommand(info_t *info, char *filePath) {
    struct stat fileStat;

    (void)info;

    if (!filePath || stat(filePath, &fileStat))
        return 0;

    if (fileStat.st_mode & S_IFREG) {
        return 1;
    }

    return 0;
}

/**
 * duplicateCharacters - Duplicates characters from a string.
 * @inputString: The input string.
 * @startIndex: Starting index.
 * @stopIndex: Stopping index.
 *
 * Return: Pointer to a new buffer.
 */
char *duplicateCharacters(char *inputString, int startIndex, int stopIndex) {
    static char buffer[1024];
    int bufferIndex = 0, i = 0;

    for (i = startIndex; i < stopIndex; i++) {
        if (inputString[i] != ':') {
            buffer[bufferIndex++] = inputString[i];
        }
    }

    buffer[bufferIndex] = '\0';
    return buffer;
}

/**
 * findExecutablePath - Finds the full path of a command in the PATH string.
 * @info: Pointer to the info struct.
 * @pathString: The PATH string.
 * @command: The command to find.
 *
 * Return: Full path of the command if found, otherwise NULL.
 */
char *findExecutablePath(info_t *info, char *pathString, char *command) {
    int i = 0, currentPos = 0;
    char *path;

    if (!pathString)
        return NULL;

    if ((getLength(command) > 2) && startsWith(command, "./")) {
        if (isExecutableCommand(info, command))
            return command;
    }

    while (1) {
        if (!pathString[i] || pathString[i] == ':') {
            path = duplicateCharacters(pathString, currentPos, i);
            if (!*path) {
                concatenateString(path, command);
            } else {
                concatenateString(path, "/");
                concatenateString(path, command);
            }

            if (isExecutableCommand(info, path))
                return path;

            if (!pathString[i])
                break;

            currentPos = i;
        }

        i++;
    }

    return NULL;
}

