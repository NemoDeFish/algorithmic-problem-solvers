/*
    Implemented by Lim Si Yong.

    Implementation details for module which contains doubly-linked list
    specification data structures and functions.
*/

#include <assert.h>
#include <stdio.h>
#include <stdlib.h>

#include "linkedList.h"

struct linkedListNode;

struct linkedList {
    struct linkedListNode *head;
    struct linkedListNode *tail;
};

struct linkedListNode {
    struct linkedListNode *prev;
    struct linkedListNode *next;
    long double x;
    long double y;
};

/* Print out each value in the list from the first value to the final value.
    Each value is printed with the format "(x, y) " where x is the x value
    set when inserting the item and y is the y value set when inserting
    the item. */
void traverseRingForward(struct linkedList *list) {
    struct linkedListNode *node = list->head;

    while (node) {
        printf("(%Lf, %Lf) ", node->x, node->y);
        node = node->next;
    }
}

/* Print out first the first item of the list, then print out each value in
    the list from the final value to the second value.
    Each value is printed with the format "(x, y) " where x is the x value
    set when inserting the item and y is the y value set when inserting
    the item. */
void traverseRingBackwards(struct linkedList *list) {
    struct linkedListNode *node = list->head;

    if (node) {
        printf("(%Lf, %Lf) ", node->x, node->y);
        node = list->tail;
        while (node->prev) {
            printf("(%Lf, %Lf) ", node->x, node->y);
            node = node->prev;
        }
    }
}

/* Return a new empty list. */
struct linkedList *newList() {
    struct linkedList *list = malloc(sizeof *list);
    assert(list);

    list->head = NULL;
    list->tail = NULL;

    return list;
}

/* Insert the given x, y pair at the head of the list. */
void insertHead(struct linkedList *list, long double x, long double y) {
    assert(list != NULL);

    /* Create and initialise a new list node. */
    struct linkedListNode *node = malloc(sizeof *node);
    assert(node);

    /* Assign data to new node. */
    node->x = x;
    node->y = y;

    /* Point next of new node to the first node of the doubly linked list. */
    node->next = list->head;
    /* Point prev to NULL. */
    node->prev = NULL;

    /* Point prev of the second node to the new first node. */
    if (list->head != NULL) {
        list->head->prev = node;
    } else {
        /* If list was empty, this new node is also the last node now. */
        list->tail = node;
    }
    /* Place it at the start of the list. */
    list->head = node;
}

/* Insert the given x, y pair at the tail of the list. */
void insertTail(struct linkedList *list, long double x, long double y) {
    assert(list != NULL);

    /* Initialise a new list node to store this data. */
    struct linkedListNode *node = malloc(sizeof *node);
    assert(node);

    /* Assign data to new node. */
    node->x = x;
    node->y = y;
    /* As the last node, there's no next node. */
    node->next = NULL;

    if (!list->head) {
        /* If the list was empty, new node is now the first node. */
        list->head = node;
        node->prev = NULL;
    } else {
        /* Otherwise, it goes after the current last node. */
        list->tail->next = node;
    }

    /* Assign prev of node to tail pointer. */
    node->prev = list->tail;
    /* Place this new node at the end of the list. */
    list->tail = node;
}

/* Free all items in the given list. */
void freeList(struct linkedList *list) {
    assert(list != NULL);
    /* Free each node. */
    struct linkedListNode *node = list->head;
    struct linkedListNode *next = NULL;

    while (node) {
        next = node->next;
        free(node);
        node = next;
    }

    /* Free the list struct itself. */
    free(list);
}
