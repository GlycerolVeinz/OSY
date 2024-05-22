#include "utils.h"

void *sharedData_init(int bufferSize)
{
    ThreadSharedData *sharedData = (ThreadSharedData *) checked_mallock(sizeof(ThreadSharedData));
    sharedData->bufferSize = bufferSize;

    sharedData->buffer = createLinkedListQueue();

    sem_t semaphore;
    if (sem_init(&semaphore, 0, 0) != SUCCESS)
    {
        perror("Error: semaphore init failed");
        exit(PROGRAMME_FAIL);
    }
    sharedData->semaphore = semaphore;

    pthread_mutex_t mutex, writeMutex;
    if (pthread_mutex_init(&mutex, NULL) != SUCCESS)
    {
        perror("Error: mutex init failed");
        exit(PROGRAMME_FAIL);
    }
    if (pthread_mutex_init(&writeMutex, NULL) != SUCCESS)
    {
        perror("Error: mutex init failed");
        exit(PROGRAMME_FAIL);
    }
    sharedData->writeMutex = writeMutex;

    pthread_t *consumer_ids = (pthread_t *) checked_calloc(bufferSize, sizeof(pthread_t));
    sharedData->consumer_ids = consumer_ids;

    sharedData->terminate = false;

    return sharedData;
}

bool isBufferEmpty(ThreadSharedData *sharedData)
{
    bool isEmpty = true;
    pthread_mutex_lock(&sharedData->mutex);
    isEmpty = sharedData->buffer->list->size == 0;
    pthread_mutex_unlock(&sharedData->mutex);
    return isEmpty;
}

void sharedData_destroy(ThreadSharedData *sharedData)
{
    destroyLinkedListQueue(sharedData->buffer);
    sem_destroy(&sharedData->semaphore);
    pthread_mutex_destroy(&sharedData->mutex);
    pthread_mutex_destroy(&sharedData->writeMutex);
    free(sharedData->consumer_ids);
    free(sharedData);
}

int getNumOfConsuments(int argc, char const *argv[])
{
    int numOfConsuments = 1;
    if (argc > 1)
    {
        int argv_1 = atoi(argv[1]);

        if (argv_1 <= ((int)sysconf(_SC_NPROCESSORS_ONLN)) && argv_1 > 0)
            numOfConsuments = argv_1;

        else
        {
            perror("Error: invalid number of consumers");
            exit(PROGRAMME_FAIL);
        }

    }
    return numOfConsuments;
}

void *checked_mallock(size_t size)
{
    void *ptr = malloc(size);
    if (!ptr) {
        perror("Error: malloc failed");
        exit(PROGRAMME_FAIL);
    }
    return ptr;
}

void *checked_calloc(size_t nmemb, size_t size)
{
    void *ptr = calloc(nmemb, size);
    if (!ptr) {
        perror("Error: calloc failed");
        exit(PROGRAMME_FAIL);
    }
    return ptr;
}
