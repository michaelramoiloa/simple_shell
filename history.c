#include "shell.h"

/**
 * getHistoryFile - Obtains the history file path.
 * @shellInfo: Parameter structure.
 *
 * Return: Allocated string containing the history file path.
 */
char *getHistoryFile(ShellInfo *shellInfo) {
    char *buffer, *homeDir;

    homeDir = getEnvironmentValue(shellInfo, "HOME=");
    if (!homeDir)
        return NULL;

    buffer = allocateMemory(sizeof(char) * (getLength(homeDir) + getLength(HIST_FILE) + 2));
    if (!buffer)
        return NULL;

    buffer[0] = '\0';
    copyString(buffer, homeDir);
    concatenateString(buffer, "/");
    concatenateString(buffer, HIST_FILE);

    return buffer;
}

/**
 * writeHistory - Creates or appends to the history file.
 * @shellInfo: Parameter structure.
 *
 * Return: 1 on success, -1 on failure.
 */
int writeHistory(ShellInfo *shellInfo) {
    ssize_t fileDescriptor;
    char *filename = getHistoryFile(shellInfo);
    List *node = NULL;

    if (!filename)
        return -1;

    fileDescriptor = open(filename, O_CREAT | O_TRUNC | O_RDWR, 0644);
    free(filename);

    if (fileDescriptor == -1)
        return -1;

    for (node = shellInfo->history; node; node = node->next) {
        writeStringToFileDescriptor(node->str, fileDescriptor);
        writeCharacterToFileDescriptor('\n', fileDescriptor);
    }

    writeCharacterToFileDescriptor(BUFFER_FLUSH, fileDescriptor);
    close(fileDescriptor);

    return 1;
}

/**
 * readHistory - Reads history from the history file.
 * @shellInfo: Parameter structure.
 *
 * Return: Histcount on success, 0 otherwise.
 */
int readHistory(ShellInfo *shellInfo) {
    int i, last = 0, lineCount = 0;
    ssize_t fileDescriptor, readLength, fileSize = 0;
    struct stat statBuffer;
    char *buffer = NULL, *filename = getHistoryFile(shellInfo);

    if (!filename)
        return 0;

    fileDescriptor = open(filename, O_RDONLY);
    free(filename);

    if (fileDescriptor == -1)
        return 0;

    if (!fstat(fileDescriptor, &statBuffer))
        fileSize = statBuffer.st_size;

    if (fileSize < 2)
        return 0;

    buffer = allocateMemory(sizeof(char) * (fileSize + 1));

    if (!buffer)
        return 0;

    readLength = read(fileDescriptor, buffer, fileSize);
    buffer[fileSize] = '\0';

    if (readLength <= 0)
        return (free(buffer), 0);

    close(fileDescriptor);

    for (i = 0; i < fileSize; i++) {
        if (buffer[i] == '\n') {
            buffer[i] = '\0';
            buildHistoryList(shellInfo, buffer + last, lineCount++);
            last = i + 1;
        }
    }

    if (last != i)
        buildHistoryList(shellInfo, buffer + last, lineCount++);

    free(buffer);
    shellInfo->histCount = lineCount;

    while (shellInfo->histCount-- >= HIST_MAX)
        deleteNodeAtIndex(&(shellInfo->history), 0);

    renumberHistory(shellInfo);

    return shellInfo->histCount;
}

/**
 * buildHistoryList - Adds an entry to the history linked list.
 * @shellInfo: Parameter structure.
 * @buf: Buffer.
 * @lineCount: History line count.
 *
 * Return: Always 0.
 */
int buildHistoryList(ShellInfo *shellInfo, char *buf, int lineCount) {
    List *node = NULL;

    if (shellInfo->history)
        node = shellInfo->history;

    addNodeAtEnd(&node, buf, lineCount);

    if (!shellInfo->history)
        shellInfo->history = node;

    return 0;
}

/**
 * renumberHistory - Renews history linked list numbering after changes.
 * @shellInfo: Parameter structure.
 *
 * Return: The new histCount.
 */
int renumberHistory(ShellInfo *shellInfo) {
    List *node = shellInfo->history;
    int i = 0;

    while (node) {
        node->number = i++;
        node = node->next;
    }

    return (shellInfo->histCount = i);
}

