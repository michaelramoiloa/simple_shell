#include "shell.h"

/**
 * mainShellLoop - main shell loop
 * @info: the parameter & return info struct
 * @av: the argument vector from main()
 *
 * Return: 0 on success, 1 on error, or error code
 */
int mainShellLoop(info_t *info, char **av)
{
    ssize_t readResult = 0;
    int builtinResult = 0;

    while (readResult != -1 && builtinResult != -2)
    {
        clearInformation(info);

        if (interactive(info))
            _puts("$ ");

        _eputchar(BUF_FLUSH);
        readResult = get_input(info);

        if (readResult != -1)
        {
            setInformation(info, av);
            builtinResult = find_builtin(info);

            if (builtinResult == -1)
                find_cmd(info);
        }
        else if (interactive(info))
            _putchar('\n');

        freeInformation(info, 0);
    }

    write_history(info);
    freeInformation(info, 1);

    if (!interactive(info) && info->statusCode)
        exit(info->statusCode);

    if (builtinResult == -2)
    {
        if (info->errorNumber == -1)
            exit(info->statusCode);

        exit(info->errorNumber);
    }

    return (builtinResult);
}

/**
 * find_builtin - finds a builtin command
 * @info: the parameter & return info struct
 *
 * Return: -1 if builtin not found,
 *         0 if builtin executed successfully,
 *         1 if builtin found but not successful,
 *         2 if builtin signals exit()
 */
int find_builtin(info_t *info)
{
    int i, builtinResult = -1;
    builtin_table builtinTable[] = {
        {"exit", _myexit},
        {"env", _myenv},
        {"help", _myhelp},
        {"history", _myhistory},
        {"setenv", _mysetenv},
        {"unsetenv", _myunsetenv},
        {"cd", _mycd},
        {"alias", _myalias},
        {NULL, NULL}
    };

    for (i = 0; builtinTable[i].type; i++)
    {
        if (_strcmp(info->argv[0], builtinTable[i].type) == 0)
        {
            info->lineCount++;
            builtinResult = builtinTable[i].func(info);
            break;
        }
    }

    return builtinResult;
}

/**
 * find_cmd - finds a command in PATH
 * @info: the parameter & return info struct
 *
 * Return: void
 */
void find_cmd(info_t *info)
{
    char *path = NULL;
    int i, k;

    info->path = info->argv[0];

    if (info->lineCountFlag == 1)
    {
        info->lineCount++;
        info->lineCountFlag = 0;
    }

    for (i = 0, k = 0; info->arg[i]; i++)
    {
        if (!is_delim(info->arg[i], " \t\n"))
            k++;
    }

    if (!k)
        return;

    path = find_path(info, _getenv(info, "PATH="), info->argv[0]);

    if (path)
    {
        info->path = path;
        fork_cmd(info);
    }
    else
    {
        if ((interactive(info) || _getenv(info, "PATH=") || info->argv[0][0] == '/') && is_cmd(info, info->argv[0]))
            fork_cmd(info);
        else if (*(info->arg) != '\n')
        {
            info->statusCode = 127;
            print_error(info, "not found\n");
        }
    }
}

/**
 * fork_cmd - forks an exec thread to run cmd
 * @info: the parameter & return info struct
 *
 * Return: void
 */
void fork_cmd(info_t *info)
{
    pid_t child_pid;

    child_pid = fork();

    if (child_pid == -1)
    {
        perror("Error:");
        return;
    }

    if (child_pid == 0)
    {
        if (execve(info->path, info->argv, getEnvironmentStrings(info)) == -1)
        {
            freeInformation(info, 1);

            if (errno == EACCES)
                exit(126);

            exit(1);
        }
    }
    else
    {
        wait(&(info->statusCode));

        if (WIFEXITED(info->statusCode))
        {
            info->statusCode = WEXITSTATUS(info->statusCode);

            if (info->statusCode == 126)
                print_error(info, "Permission denied\n");
        }
    }
}

