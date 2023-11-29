#include "prod-cons.h"

int producerRetVal = 0;

int main(int argc, char const *argv[]) {
    int numOfConsuments = getNumOfConsuments(argc, argv);
    ThreadSharedData *sharedData = sharedData_init(numOfConsuments);

    pthread_t producerId;
    if (pthread_create(&producerId, NULL, producer, sharedData) != SUCCESS) {
        perror("Error: producer thread creation failed");
        sharedData_destroy(sharedData);
        exit(PROGRAMME_FAIL);
    }

    for (int i = 0; i < numOfConsuments; ++i) {
        pthread_t consumerId;
        if (pthread_create(&consumerId, NULL, consumer, sharedData) != SUCCESS) {
            perror("Error: consumer thread creation failed");
            sharedData_destroy(sharedData);
            exit(PROGRAMME_FAIL);
        }
        sharedData->consumerIds[i] = consumerId;
    }

    pthread_join(producerId, NULL);

    if (getProducerRetVal(sharedData) != SUCCESS) {
        cancelConsumers(sharedData);
        sharedData_destroy(sharedData);
        exit(PROGRAMME_FAIL);
    }

    cancelConsumers(sharedData);

    sharedData_destroy(sharedData);
    exit(SUCCESS);
}

void *producer(void *arg) {
    ThreadSharedData *sharedData = (ThreadSharedData *) arg;
    int ret;
    int int_in;
    String str_in;

    while ((ret = scanf("%d %ms\n", &int_in, &str_in)) == 2) {
        if (ret != 2) {

            fprintf(stderr, "Error: scanf failed");

            pthread_mutex_lock(&sharedData->readMutex);
            sharedData->terminate = true;
            sharedData->cancel = true;
            sharedData->retVal = PROGRAMME_FAIL;
            pthread_mutex_unlock(&sharedData->readMutex);

            pthread_exit(NULL);
        }

        DataItem *dataItem = (DataItem *) checked_mallock(sizeof(DataItem));
        dataItem->str = str_in;
        dataItem->count = int_in;

        pthread_mutex_lock(&sharedData->readMutex);
        push(sharedData->buffer, dataItem);
        pthread_mutex_unlock(&sharedData->readMutex);
        sem_post(&sharedData->semaphore);
    }

    pthread_mutex_lock(&sharedData->readMutex);
    sharedData->terminate = true;
    sharedData->retVal = SUCCESS;
    pthread_mutex_unlock(&sharedData->readMutex);

    pthread_exit(NULL);
}


void *consumer(void *arg) {
    ThreadSharedData *sharedData = (ThreadSharedData *) arg;
    int id = getMyId(sharedData);

    if (id == -1) {
        fprintf(stderr, "Error: can't find myself (consumer)");
//        ret = PROGRAMME_FAIL;
//        pthread_mutex_lock(&sharedData->readMutex);
//        sharedData->retVal = ret;
//        pthread_mutex_unlock(&sharedData->readMutex);
//        pthread_exit(NULL);
    }

    while (!isBufferEmpty(sharedData) || !shouldTerminate(sharedData)) {
        sem_wait(&sharedData->semaphore);

        if (shouldCancel(sharedData) && isBufferEmpty(sharedData)) {
            pthread_exit(NULL);
        }

        pthread_mutex_lock(&sharedData->readMutex);
        node *n = (node *) pop(sharedData->buffer);
        DataItem *item = (DataItem *) n->data;
        pthread_mutex_unlock(&sharedData->readMutex);

        pthread_mutex_lock(&sharedData->writeMutex);
        printf("Thread %d:", id);
        for (int i = 0; i < item->count; ++i) {
            printf(" %s", item->str);
        }
        printf("\n");
        pthread_mutex_unlock(&sharedData->writeMutex);

        free(item->str);
        free(item);
        free(n);
    }

    fprintf(stderr, "consumer exited\n");
    pthread_exit(NULL);
}

