#include "shell.h"

/**
 * initializeInfo - Initializes the info_t structure.
 * @info: Pointer to the info_t structure.
 */
void initializeInfo(info_t *info) {
    info->arguments = NULL;
    info->argVector = NULL;
    info->executionPath = NULL;
    info->argumentCount = 0;
}

/**
 * fillInfo - Fills the info_t structure with data.
 * @info: Pointer to the info_t structure.
 * @argumentVector: Argument vector.
 */
void fillInfo(info_t *info, char **argumentVector) {
    int i = 0;

    info->fileName = argumentVector[0];
    if (info->arguments) {
        info->argVector = tokenizeString(info->arguments, " \t");
        if (!info->argVector) {
            info->argVector = allocateMemory(sizeof(char *) * 2);
            if (info->argVector) {
                info->argVector[0] = duplicateString(info->arguments);
                info->argVector[1] = NULL;
            }
        }
        for (i = 0; info->argVector && info->argVector[i]; i++)
            ;
        info->argumentCount = i;

        replaceAliases(info);
        replaceVariables(info);
    }
}

/**
 * releaseInfo - Releases resources used by the info_t structure.
 * @info: Pointer to the info_t structure.
 * @freeAll: 1 if freeing all fields, otherwise 0.
 */
void releaseInfo(info_t *info, int freeAll) {
    free(info->argVector);
    info->argVector = NULL;
    info->executionPath = NULL;
    if (freeAll) {
        if (!info->commandBuffer)
            free(info->arguments);
        if (info->environment)
            freeList(&(info->environment));
        if (info->history)
            freeList(&(info->history));
        if (info->aliasList)
            freeList(&(info->aliasList));
        free(info->environmentCopy);
        info->environmentCopy = NULL;
        free(info->commandBuffer);
        if (info->readFileDescriptor > 2)
            close(info->readFileDescriptor);
        writeCharacterToOutputBuffer(BUFFER_FLUSH);
    }
}

