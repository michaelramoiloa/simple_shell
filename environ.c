#include "shell.h"

/**
 * myExit - exits the shell
 * @info: Structure containing potential arguments. Used to maintain
 * constant function prototype.
 * 
 * Returns: Exits with a given exit status (0) if info->argv[0] != "exit"
 */
int myExit(info_t *info)
{
    int exitStatus;

    switch (info->argc) 
    {
        case 1:
            info->err_num = -1;
            return -2;
        case 2:
            exitStatus = _erratoi(info->argv[1]);
            if (exitStatus == -1)
            {
                info->status = 2;
                printError(info, "Illegal number: ");
                _ePuts(info->argv[1]);
                _ePutChar('\n');
                return 1;
            }
            info->err_num = exitStatus;
            return -2;
        default:
            // Handle invalid argument count
            info->status = 2;
            printError(info, "Too many arguments for exit\n");
            return 1;
    }
}

/**
 * myCd - changes the current directory of the process
 * @info: Structure containing potential arguments. Used to maintain
 * constant function prototype.
 * 
 * Returns: Always 0
 */
int myCd(info_t *info)
{
    char *currentDir, *newDir, buffer[1024];
    int chdirResult;

    currentDir = getcwd(buffer, 1024);
    if (!currentDir)
        _puts("TODO: >>getcwd failure emsg here<<\n");

    switch (info->argc)
    {
        case 1:
            newDir = _getenv(info, "HOME=");
            if (!newDir)
                chdirResult = /* TODO: what should this be? */
                    chdir((newDir = _getenv(info, "PWD=")) ? newDir : "/");
            else
                chdirResult = chdir(newDir);
            break;
        case 2:
            if (_strcmp(info->argv[1], "-") == 0)
            {
                if (!_getenv(info, "OLDPWD="))
                {
                    _puts(currentDir);
                    _putChar('\n');
                    return 1;
                }
                _puts(_getenv(info, "OLDPWD="));
                _putChar('\n');
                chdirResult = /* TODO: what should this be? */
                    chdir((newDir = _getenv(info, "OLDPWD=")) ? newDir : "/");
            }
            else
            {
                chdirResult = chdir(info->argv[1]);
            }
            break;
        default:
            // Handle invalid argument count
            info->status = 2;
            printError(info, "Usage: cd [directory]\n");
            return 1;
    }

    if (chdirResult == -1)
    {
        printError(info, "can't cd to ");
        _ePuts(info->argv[1]);
        _ePutChar('\n');
    }
    else
    {
        _setenv(info, "OLDPWD", _getenv(info, "PWD="));
        _setenv(info, "PWD", getcwd(buffer, 1024));
    }

    return 0;
}

/**
 * myHelp - prints help information
 * @info: Structure containing potential arguments. Used to maintain
 * constant function prototype.
 * 
 * Returns: Always 0
 */
int myHelp(info_t *info)
{
    char **argArray;

    argArray = info->argv;
    _puts("Help call works. Function not yet implemented\n");
    if (0)
        _puts(*argArray); // Temporary workaround for unused variable
    return 0;
}

