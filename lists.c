#include "shell.h"

/**
 * addNode - Adds a node to the start of the linked list.
 * @head: Address of the pointer to the head node.
 * @data: String data for the new node.
 * @index: Index used by history.
 *
 * Return: Pointer to the new head of the linked list.
 */
list_t *addNode(list_t **head, const char *data, int index) {
    list_t *newHead;

    if (!head)
        return NULL;

    newHead = allocateMemory(sizeof(list_t));
    if (!newHead)
        return NULL;

    initializeMemory(newHead, sizeof(list_t));
    newHead->index = index;

    if (data) {
        newHead->str = duplicateString(data);
        if (!newHead->str) {
            free(newHead);
            return NULL;
        }
    }

    newHead->next = *head;
    *head = newHead;

    return newHead;
}

/**
 * addNodeEnd - Adds a node to the end of the linked list.
 * @head: Address of the pointer to the head node.
 * @data: String data for the new node.
 * @index: Index used by history.
 *
 * Return: Pointer to the new node added.
 */
list_t *addNodeEnd(list_t **head, const char *data, int index) {
    list_t *newNode, *node;

    if (!head)
        return NULL;

    node = *head;
    newNode = allocateMemory(sizeof(list_t));
    if (!newNode)
        return NULL;

    initializeMemory(newNode, sizeof(list_t));
    newNode->index = index;

    if (data) {
        newNode->str = duplicateString(data);
        if (!newNode->str) {
            free(newNode);
            return NULL;
        }
    }

    if (node) {
        while (node->next)
            node = node->next;
        node->next = newNode;
    } else {
        *head = newNode;
    }

    return newNode;
}

/**
 * printListString - Prints only the 'str' element of a linked list.
 * @head: Pointer to the first node.
 *
 * Return: Size of the linked list.
 */
size_t printListString(const list_t *head) {
    size_t size = 0;

    while (head) {
        writeString(head->str ? head->str : "(nil)");
        writeString("\n");
        head = head->next;
        size++;
    }

    return size;
}

/**
 * deleteNodeAtIndex - Deletes a node at the given index.
 * @head: Address of the pointer to the first node.
 * @index: Index of the node to delete.
 *
 * Return: 1 on success, 0 on failure.
 */
int deleteNodeAtIndex(list_t **head, unsigned int index) {
    list_t *node, *prevNode;
    unsigned int i = 0;

    if (!head || !*head)
        return 0;

    if (!index) {
        node = *head;
        *head = (*head)->next;
        free(node->str);
        free(node);
        return 1;
    }

    node = *head;

    while (node) {
        if (i == index) {
            prevNode->next = node->next;
            free(node->str);
            free(node);
            return 1;
        }

        i++;
        prevNode = node;
        node = node->next;
    }

    return 0;
}

/**
 * freeList - Frees all nodes of a linked list.
 * @headPtr: Address of the pointer to the head node.
 *
 * Return: void.
 */
void freeList(list_t **headPtr) {
    list_t *node, *nextNode, *head;

    if (!headPtr || !*headPtr)
        return;

    head = *headPtr;
    node = head;

    while (node) {
        nextNode = node->next;
        free(node->str);
        free(node);
        node = nextNode;
    }

    *headPtr = NULL;
}

