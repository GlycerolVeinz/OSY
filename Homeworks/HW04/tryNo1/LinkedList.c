#include "LinkedList.h"

LinkedList *linkedList_init() {
    LinkedList *list = (LinkedList *) checked_mallock(sizeof(LinkedList));
    list->head = NULL;
    list->tail = NULL;
    list->size = 0;
    return list;
}

void addNode(LinkedList *list, void *data){
    node *nod = (node *) checked_mallock(sizeof(node));
    nod->data = data;
    nod->next = NULL;
    nod->prev = NULL;

    if(list->head == NULL){
        list->head = nod;
        list->tail = nod;
    }else{
        nod->prev = list->tail;
        list->tail->next = nod;
        list->tail = nod;
    }
    list->size++;
}

node *removeFirst(LinkedList *list){
    if (list->head == NULL) return NULL;

    node *temp = list->head;
    list->head = temp->next;
    if (list->head != NULL) list->head->prev = NULL;
    else list->tail = NULL;
    list->size--;
    return temp;
}

node *removeLast(LinkedList *list){
    if (list->tail == NULL) return NULL;

    node *temp = list->tail;
    list->tail = temp->prev;
    if (list->tail != NULL) list->tail->next = NULL;
    else list->head = NULL;
    list->size--;
    return temp;
}

node *removeNode(LinkedList *list, int index){
    if (index < 0 || index >= list->size) return NULL;
    if (index == 0) return removeFirst(list);
    if (index == list->size - 1) return removeLast(list);

    node *temp = list->head;
    for(int i = 0; i < index; i++){
        temp = temp->next;
    }
    temp->prev->next = temp->next;
    temp->next->prev = temp->prev;
    list->size--;
    return temp;
}

void linkedList_destroy(LinkedList *list){
    node *temp = list->head;
    while(temp != NULL){
        node *next = temp->next;
        free(temp);
        temp = next;
    }
    free(list);
}