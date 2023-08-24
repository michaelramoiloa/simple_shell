#ifndef _SHELL_H_
#define _SHELL_H_

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/stat.h>
#include <limits.h>
#include <fcntl.h>
#include <errno.h>

#define READ_BUF_SIZE 1024
#define WRITE_BUF_SIZE 1024
#define BUF_FLUSH -1

#define CMD_NORMAL 0
#define CMD_OR 1
#define CMD_AND 2
#define CMD_CHAIN 3

#define CONVERT_LOWERCASE 1
#define CONVERT_UNSIGNED 2

#define USE_GETLINE 0
#define USE_STRTOK 0

#define HISTORY_FILE ".simple_shell_history"
#define MAX_HISTORY_SIZE 4096

extern char **environ;

typedef struct ListNode {
    int num;
    char *str;
    struct ListNode *next;
} LinkedList;

typedef struct CommandInfo {
    char *arg;
    char **argv;
    char *path;
    int argc;
    unsigned int lineCount;
    int errorNumber;
    int lineCountFlag;
    char *fileName;
    LinkedList *env;
    LinkedList *history;
    LinkedList *alias;
    char **customEnviron;
    int isEnvChanged;
    int statusCode;
    char **commandBuffer;
    int commandBufferType;
    int readFd;
    int historyCount;
} Info;

#define INFO_INIT {NULL, NULL, NULL, 0, 0, 0, 0, NULL, NULL, NULL, NULL, NULL, 0, 0, NULL, 0, 0, 0}

typedef struct BuiltinCommand {
    char *name;
    int (*function)(Info *);
} BuiltinTable;

int shellLoop(Info *, char **);
int findBuiltin(Info *);
void findCommand(Info *);
void forkCommand(Info *);

int isExecutable(Info *, char *);
char *duplicateSubstring(char *, int, int);
char *findExecutablePath(Info *, char *);

int mainShellLoop(char **);
void printErrorMessage(char *);
int printErrorCharacter(char);
int printToFileDescriptor(char, int);
int printToStandardOutput(char *);

int stringLength(char *);
int stringCompare(char *, char *);
char *stringStartsWith(const char *, const char *);
char *stringConcatenate(char *, char *);

char *stringCopy(char *, char *);
char *stringDuplicate(const char *);
void printString(char *);
int printCharacter(char);

char *stringCopyN(char *, char *, int);
char *stringConcatenateN(char *, char *, int);
char *stringFindCharacter(char *, char);

char **tokenizeString(char *, char *);
char **tokenizeStringV2(char *, char);

char *memorySet(char *, char, unsigned int);
void freeStringArray(char **);
void *reallocateMemory(void *, unsigned int, unsigned int);
int freeBlock(void **);

int baseConversion(char *);
int isDelimiter(char, char *);
int isAlphabetic(int);
int toInteger(char *);

int customAtoi(char *);
void printCustomError(Info *, char *);
int printDigit(int, int);
char *convertNumber(long int, int, int);
void removeComments(char *);

int exitShell(Info *);
int changeDirectory(Info *);
int showHelp(Info *);

int showHistory(Info *);
int manageAlias(Info *);

ssize_t getInput(Info *);
int getLine(Info *, char **, size_t *);
void signalInterruptHandler(int);

void clearInformation(Info *);
void setInformation(Info *, char **);
void freeInformation(Info *, int);

char *_getEnvironment(Info *, const char *);
int showEnvironment(Info *);
int setEnvironment(Info *);
int unsetEnvironment(Info *);
int populateEnvironmentList(Info *);

char **getEnvironmentStrings(Info *);
int unsetEnvironmentVariable(Info *, char *);
int setEnvironmentVariable(Info *, char *, char *);

char *getHistoryFile(Info *);
int writeHistory(Info *);
int readHistory(Info *);
int buildHistoryList(Info *, char *, int);
int renumberHistory(Info *);

LinkedList *addNode(LinkedList **, const char *, int);
LinkedList *addNodeEnd(LinkedList **, const char *, int);
size_t printListStrings(const LinkedList *);
int deleteNodeAtIndex(LinkedList **, unsigned int);
void freeLinkedList(LinkedList **);

size_t countListItems(const LinkedList *);
char **convertListToStrings(LinkedList *);
size_t printList(const LinkedList *);
LinkedList *nodeStartsWith(LinkedList *, char *, char);
ssize_t getNodeIndex(LinkedList *, LinkedList *);

int isCommandChaining(Info *, char *, size_t *);
void checkCommandChaining(Info *, char *, size_t *, size_t, size_t);
int replaceAlias(Info *);
int replaceVariables(Info *);
int replaceString(char **, char *);

#endif

