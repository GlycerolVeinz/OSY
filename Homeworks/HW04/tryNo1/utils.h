#ifndef MY_UTILS_H_
#define MY_UTILS_H_

#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>

#include <pthread.h>
#include <semaphore.h>

#include "checkedMemory.h"
#include "LinkedListQueue.h"

#define THREAD_FAIL (-1)
#define PROGRAMME_FAIL 1
#define SUCCESS 0

#define TRUE 1
#define FALSE 0

typedef char* String;

typedef struct
{
    int count;
    String str;
} DataItem;

typedef struct
{
    sem_t semaphore;
    pthread_mutex_t mutex;
    pthread_mutex_t writeMutex;

    pthread_t *consumer_ids;
    bool terminate;

    int bufferSize;
    LinkedListQueue *buffer;

} ThreadSharedData;

void *sharedData_init(int bufferSize);
bool isBufferEmpty(ThreadSharedData *sharedData);
void sharedData_destroy(ThreadSharedData *sharedData);

int getNumOfConsuments(int argc, char const *argv[]);
void *checked_mallock(size_t size);
void *checked_calloc(size_t nmemb, size_t size);

#endif // MY_UTILS_H_