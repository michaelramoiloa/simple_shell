#include "shell.h"

/**
 * bufferInput - buffers chained commands
 * @info: parameter structure
 * @buf: address of buffer
 * @length: address of length variable
 *
 * Return: bytes read
 */
ssize_t bufferInput(info_t *info, char **buf, size_t *length)
{
    ssize_t bytesRead = 0;
    size_t bufferLength = 0;

    if (!*length)
    {
        free(*buf);
        *buf = NULL;
        signal(SIGINT, blockCtrlC);
#if USE_GETLINE
        bytesRead = getline(buf, &bufferLength, stdin);
#else
        bytesRead = getLine(info, buf, &bufferLength);
#endif
        if (bytesRead > 0)
        {
            if ((*buf)[bytesRead - 1] == '\n')
            {
                (*buf)[bytesRead - 1] = '\0';
                bytesRead--;
            }
            info->lineCountFlag = 1;
            removeComments(*buf);
            buildHistoryList(info, *buf, info->historyCount++);
        }
    }
    return bytesRead;
}

/**
 * getInput - gets a line minus the newline
 * @info: parameter structure
 *
 * Return: bytes read
 */
ssize_t getInput(info_t *info)
{
    static char *buffer; 
    static size_t i = 0, j = 0, bufferLength = 0;
    ssize_t bytesRead = 0;
    char **bufferPtr = &(info->arguments), *p;

    customPutChar(BUF_FLUSH);
    bytesRead = bufferInput(info, &buffer, &bufferLength);

    if (bytesRead == -1)
        return (-1);

    if (bufferLength)
    {
        j = i;
        p = buffer + i;

        checkChain(info, buffer, &j, i, bufferLength);
        while (j < bufferLength)
        {
            if (isChain(info, buffer, &j))
                break;
            j++;
        }

        i = j + 1;
        if (i >= bufferLength)
        {
            i = bufferLength = 0;
            info->commandBufferType = CMD_NORMAL;
        }

        *bufferPtr = p;
        return (customStrlen(p));
    }

    *bufferPtr = buffer;
    return (bytesRead);
}

/**
 * readBuffer - reads a buffer
 * @info: parameter structure
 * @buf: buffer
 * @i: size
 *
 * Return: bytesRead
 */
ssize_t readBuffer(info_t *info, char *buf, size_t *i)
{
    ssize_t bytesRead = 0;

    if (*i)
        return (0);

    bytesRead = read(info->readFd, buf, READ_BUFFER_SIZE);

    if (bytesRead >= 0)
        *i = bytesRead;

    return (bytesRead);
}

/**
 * getLine - gets the next line of input from STDIN
 * @info: parameter structure
 * @ptr: address of pointer to buffer, preallocated or NULL
 * @length: size of preallocated ptr buffer if not NULL
 *
 * Return: bytesRead
 */
int getLine(info_t *info, char **ptr, size_t *length)
{
    static char buf[READ_BUFFER_SIZE];
    static size_t i = 0, bufferLength = 0;
    size_t k;
    ssize_t bytesRead = 0, bytesReadTotal = 0;
    char *p = NULL, *newP = NULL, *c;

    p = *ptr;
    if (p && length)
        bytesReadTotal = *length;

    if (i == bufferLength)
    {
        i = bufferLength = 0;
    }

    bytesRead = readBuffer(info, buf, &bufferLength);

    if (bytesRead == -1 || (bytesRead == 0 && bufferLength == 0))
        return (-1);

    c = _strchr(buf + i, '\n');
    k = c ? 1 + (unsigned int)(c - buf) : bufferLength;

    newP = customRealloc(p, bytesReadTotal, bytesReadTotal ? bytesReadTotal + k : k + 1);
    if (!newP)
    {
        if (p)
            free(p);
        return (p ? -1 : -1);
    }

    if (bytesReadTotal)
        customStrncat(newP, buf + i, k - i);
    else
        customStrncpy(newP, buf + i, k - i + 1);

    bytesReadTotal += k - i;
    i = k;
    p = newP;

    if (length)
        *length = bytesReadTotal;

    *ptr = p;
    return bytesReadTotal;
}

/**
 * blockCtrlC - blocks ctrl-C
 * @signalNumber: the signal number
 *
 * Return: void
 */
void blockCtrlC(__attribute__((unused)) int signalNumber)
{
    customPuts("\n");
    customPuts("$ ");
    customPutChar(BUF_FLUSH);
}

