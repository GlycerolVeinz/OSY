#ifndef LINKEDLISTQUEUE_H
#define LINKEDLISTQUEUE_H
#include "LinkedList.h"

typedef struct LinkedListQueue {
    LinkedList* list;
} LinkedListQueue;

LinkedListQueue* createLinkedListQueue();
void destroyLinkedListQueue(LinkedListQueue* queue);
void push(LinkedListQueue* queue, void *data);
void *pop(LinkedListQueue* queue);

#endif // LINKEDLISTQUEUE_H
