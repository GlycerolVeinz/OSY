#include "utils_2.h"

ThreadSharedData *sharedData_init(int bufferSize) {
    ThreadSharedData *sharedData = checked_mallock(sizeof(ThreadSharedData));
    sharedData->bufferSize = bufferSize;
    sharedData->terminate = false;
    sharedData->cancel = false;

    sharedData_initMutexes(sharedData);

    sharedData->consumerIds = (pthread_t *) checked_calloc(bufferSize, sizeof(pthread_t));
    sharedData->buffer = createLinkedListQueue();

    sharedData->retVal = 0;

    return sharedData;
}

void sharedData_initMutexes(ThreadSharedData *sharedData) {
    pthread_mutex_t readMutex;
    pthread_mutex_t writeMutex;
    sem_t semaphore;

    if (pthread_mutex_init(&readMutex, NULL) != SUCCESS) {
        fprintf(stderr, "Error: read mutex init failed\n");
        exit(PROGRAMME_FAIL);
    }
    if (pthread_mutex_init(&writeMutex, NULL) != SUCCESS) {
        fprintf(stderr, "Error: write mutex init failed\n");
        exit(PROGRAMME_FAIL);
    }

    if (sem_init(&semaphore, 0, 0) != SUCCESS) {
        fprintf(stderr, "Error: semaphore init failed\n");
        exit(PROGRAMME_FAIL);
    }

    sharedData->readMutex = readMutex;
    sharedData->writeMutex = writeMutex;
    sharedData->semaphore = semaphore;
}

bool isBufferEmpty(ThreadSharedData *sharedData) {
    pthread_mutex_lock(&sharedData->readMutex);
    bool ret = sharedData->buffer->list->size == 0;
    pthread_mutex_unlock(&sharedData->readMutex);
    return ret;
}

void sharedData_destroy(ThreadSharedData *sharedData) {
    sharedData_destroyMutexes(sharedData);
    destroyLinkedListQueue(sharedData->buffer);
//    free(sharedData->retVal);
    free(sharedData->consumerIds);
    free(sharedData);
}

void sharedData_destroyMutexes(ThreadSharedData *sharedData) {
    pthread_mutex_destroy(&sharedData->readMutex);
    pthread_mutex_destroy(&sharedData->writeMutex);
    sem_destroy(&sharedData->semaphore);
}

int getNumOfConsuments(int argc, char const *argv[]) {
    int numOfConsuments = 1;
    if (argc == 1) {
        return numOfConsuments;
    } else if (argc > 2) {
        fprintf(stderr, "Error: invalid number of arguments\n");
        exit(PROGRAMME_FAIL);
    } else {

        int argv_1 = atoi(argv[1]);

        if (argv_1 <= ((int) sysconf(_SC_NPROCESSORS_ONLN)) && argv_1 > 0)
            numOfConsuments = argv_1;
        else {
            fprintf(stderr, "Error: invalid number of consuments\n");
            exit(PROGRAMME_FAIL);
        }
    }

    return numOfConsuments;
}

int getMyId(ThreadSharedData *sharedData) {
    int id = -1;
    pthread_t self = pthread_self();

    pthread_mutex_lock(&sharedData->readMutex);
    for (int i = 0; i < sharedData->bufferSize; ++i) {
        if (sharedData->consumerIds[i] == self) {
            id = i;
            break;
        }
    }
    pthread_mutex_unlock(&sharedData->readMutex);

    return id;
}

bool shouldCancel(ThreadSharedData *sharedData) {
    pthread_mutex_lock(&sharedData->readMutex);
    bool cancel = sharedData->cancel;
    pthread_mutex_unlock(&sharedData->readMutex);
    return cancel;
}

bool shouldTerminate(ThreadSharedData *sharedData) {
    pthread_mutex_lock(&sharedData->readMutex);
    bool terminate = sharedData->terminate;
    pthread_mutex_unlock(&sharedData->readMutex);
    return terminate;
}

int getProducerRetVal(ThreadSharedData *sharedData) {
    pthread_mutex_lock(&sharedData->readMutex);
    int retVal = sharedData->retVal;
    pthread_mutex_unlock(&sharedData->readMutex);
    return retVal;
}

void joinConsumers(ThreadSharedData *sharedData) {
    for (int i = 0; i < sharedData->bufferSize; ++i) {
        pthread_join(sharedData->consumerIds[i], NULL);
    }
}

void cancelConsumers(ThreadSharedData *sharedData) {
    fprintf(stderr, "Error, producer exit");
    pthread_mutex_lock(&sharedData->readMutex);
    sharedData->cancel = true;
    pthread_mutex_unlock(&sharedData->readMutex);

    for (int i = 0; i < sharedData->bufferSize; ++i) {
        sem_post(&sharedData->semaphore);
    }

    joinConsumers(sharedData);
}
