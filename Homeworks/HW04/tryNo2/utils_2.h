#ifndef PROD_CONS_UTILS_H
#define PROD_CONS_UTILS_H

#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>

#include <pthread.h>
#include <semaphore.h>

#include "checkedMemory.h"
#include "LinkedListQueue.h"

#define PROGRAMME_FAIL 1
#define SUCCESS 0

typedef struct{
    char *str;
    int count;
} DataItem;

typedef struct {
    pthread_mutex_t readMutex;
    pthread_mutex_t writeMutex;
    sem_t semaphore;

    pthread_t *consumerIds; //array

    int retVal;

    int bufferSize;
    bool terminate;
    LinkedListQueue *buffer; 
} ThreadSharedData;

typedef char* String;

ThreadSharedData *sharedData_init(int bufferSize);
bool isBufferEmpty(ThreadSharedData *sharedData);
void sharedData_destroy(ThreadSharedData *sharedData);
int getNumOfConsuments(int argc, char const *argv[]);

DataItem *dataItem_init(int int_in, String str_in);
void sharedData_destroyMutexes(ThreadSharedData *sharedData);
void sharedData_initMutexes(ThreadSharedData *sharedData);


#endif