//
// Created by glycerolveinz on 22.11.23.
//

#ifndef TRYNO2_LINKEDLIST_H
#define TRYNO2_LINKEDLIST_H


#include <stdlib.h>
#include <stdio.h>

void *checked_mallock(size_t size);
void *checked_calloc(size_t nmemb, size_t size);


typedef struct Node{
    void *data;
    struct Node *next;
    struct Node *prev;
} node;

typedef struct {
    node *head;
    node *tail;
    int size;
} LinkedList;

LinkedList *linkedList_init();
void addNode(LinkedList *list, void *data);
node *removeFirst(LinkedList *list);
node *removeLast(LinkedList *list);
node *removeNode(LinkedList *list, int index);
void linkedList_destroy(LinkedList *list);

#endif //TRYNO2_LINKEDLIST_H
