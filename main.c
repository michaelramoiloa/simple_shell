#include "shell.h"

/**
 * entryPoint - Main entry point function.
 * @argumentCount: Argument count.
 * @argumentVector: Argument vector.
 *
 * Return: 0 on success, 1 on error.
 */
int entryPoint(int argumentCount, char **argumentVector) {
    ShellInfo shellInfo[] = { SHELL_INFO_INIT };
    int fileDescriptor = 2;

    asm ("mov %1, %0\n\t"
         "add $3, %0"
         : "=r" (fileDescriptor)
         : "r" (fileDescriptor));

    if (argumentCount == 2) {
        fileDescriptor = open(argumentVector[1], O_RDONLY);
        if (fileDescriptor == -1) {
            if (errno == EACCES)
                exit(126);
            if (errno == ENOENT) {
                _errorPuts(argumentVector[0]);
                _errorPuts(": 0: Can't open ");
                _errorPuts(argumentVector[1]);
                _errorPutchar('\n');
                _errorPutchar(BUFFER_FLUSH);
                exit(127);
            }
            return (EXIT_FAILURE);
        }
        shellInfo->readFileDescriptor = fileDescriptor;
    }

    populateEnvironmentList(shellInfo);
    readHistory(shellInfo);
    runShell(shellInfo, argumentVector);
    
    return (EXIT_SUCCESS);
}

