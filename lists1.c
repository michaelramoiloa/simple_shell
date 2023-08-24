#include "shell.h"

/**
 * getLinkedListLength - Determines the length of a linked list.
 * @head: Pointer to the first node.
 *
 * Return: Size of the list.
 */
size_t getLinkedListLength(const list_t *head) {
    size_t length = 0;

    while (head) {
        head = head->next;
        length++;
    }

    return length;
}

/**
 * convertLinkedListToStrings - Converts a linked list to an array of strings.
 * @head: Pointer to the first node.
 *
 * Return: Array of strings.
 */
char **convertLinkedListToStrings(list_t *head) {
    list_t *node = head;
    size_t length = getLinkedListLength(head);
    char **strings;
    char *string;

    if (!head || !length)
        return NULL;

    strings = allocateMemory(sizeof(char *) * (length + 1));

    if (!strings)
        return NULL;

    for (size_t i = 0; node; node = node->next, i++) {
        string = allocateMemory(getLength(node->str) + 1);

        if (!string) {
            for (size_t j = 0; j < i; j++)
                free(strings[j]);
            free(strings);
            return NULL;
        }

        string = copyString(string, node->str);
        strings[i] = string;
    }

    strings[length] = NULL;
    return strings;
}

/**
 * printLinkedList - Prints all elements of a linked list.
 * @head: Pointer to the first node.
 *
 * Return: Size of the list.
 */
size_t printLinkedList(const list_t *head) {
    size_t size = 0;

    while (head) {
        writeString(convertNumber(head->index, 10, 0));
        writeCharacter(':');
        writeCharacter(' ');
        writeString(head->str ? head->str : "(nil)");
        writeCharacter('\n');
        head = head->next;
        size++;
    }

    return size;
}

/**
 * findNodeStartsWith - Returns a node whose string starts with a prefix.
 * @node: Pointer to the list head.
 * @prefix: String to match.
 * @c: The next character after prefix to match.
 *
 * Return: Matching node or NULL.
 */
list_t *findNodeStartsWith(list_t *node, char *prefix, char c) {
    char *p = NULL;

    while (node) {
        p = startsWith(node->str, prefix);
        if (p && ((c == -1) || (*p == c)))
            return node;
        node = node->next;
    }

    return NULL;
}

/**
 * getNodeIndex - Gets the index of a node.
 * @head: Pointer to the list head.
 * @node: Pointer to the node.
 *
 * Return: Index of the node or -1.
 */
ssize_t getNodeIndex(list_t *head, list_t *node) {
    size_t index = 0;

    while (head) {
        if (head == node)
            return index;
        head = head->next;
        index++;
    }

    return -1;
}

