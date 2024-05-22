#include "LinkedListQueue.h"

LinkedListQueue *createLinkedListQueue() {
    LinkedListQueue *queue = (LinkedListQueue *) checked_mallock(sizeof(LinkedListQueue));
    queue->list = linkedList_init();

    return queue;
}

void destroyLinkedListQueue(LinkedListQueue *queue){
    linkedList_destroy(queue->list);
    free(queue);
}

void push(LinkedListQueue *queue, void *data) {
    addNode(queue->list, data);
}

void *pop(LinkedListQueue *queue) {
    if(queue->list->size == 0){
        fprintf(stderr, "Queue is empty. No element to pop.\n");
        return NULL;
    }
    return removeFirst(queue->list);
}