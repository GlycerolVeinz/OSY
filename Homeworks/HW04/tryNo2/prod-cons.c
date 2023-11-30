#include "prod-cons.h"

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

        pthread_mutex_lock(&sharedData->readMutex);
        sharedData->consumerIds[i] = consumerId;
        pthread_mutex_unlock(&sharedData->readMutex);
    }

    pthread_join(producerId, NULL);

    if (getProducerRetVal(sharedData) != SUCCESS) {
        cancelConsumers(sharedData);
        sharedData_destroy(sharedData);
        exit(PROGRAMME_FAIL);
    }

    joinConsumers(sharedData);

    int ret = sharedData->retVal;
    sharedData_destroy(sharedData);
    exit(ret);
}

void *producer(void *arg) {
    fprintf(stderr, "producer started\n");
    ThreadSharedData *sharedData = (ThreadSharedData *) arg;
    int ret;
    int int_in;
    String str_in;

    int firstChar = fgetc(stdin);
    fprintf(stderr, "firstChar = %d\n", firstChar);
    ungetc(firstChar, stdin);
    bool notEndInput = firstChar != EOF && firstChar != 10;
    if (notEndInput) {
        while ((ret = scanf("%d %ms", &int_in, &str_in))) {
            if (ret != 2) {
                break;
            }
            DataItem *dataItem = (DataItem *) checked_mallock(sizeof(DataItem));
            dataItem->str = str_in;
            dataItem->count = int_in;

            pthread_mutex_lock(&sharedData->readMutex);
            push(sharedData->buffer, dataItem);
            pthread_mutex_unlock(&sharedData->readMutex);
            sem_post(&sharedData->semaphore);
        }

        bool scanfEndedNotProperly = ret != 2 && ret != EOF && ret != 0;
        firstChar = fgetc(stdin);
        notEndInput = firstChar != EOF && firstChar != 10;
        if (scanfEndedNotProperly || notEndInput) {
            fprintf(stderr, "Error: scanf failed\n");

            pthread_mutex_lock(&sharedData->readMutex);
            sharedData->terminate = true;
            sharedData->cancel = true;
            sharedData->retVal = PROGRAMME_FAIL;
            pthread_mutex_unlock(&sharedData->readMutex);

            pthread_exit(NULL);
        }
    }

    pthread_mutex_lock(&sharedData->readMutex);
    sharedData->terminate = true;
    sharedData->retVal = SUCCESS;
    pthread_mutex_unlock(&sharedData->readMutex);

    fprintf(stderr, "producer exited\n");
    pthread_exit(NULL);
}


void *consumer(void *arg) {
    ThreadSharedData *sharedData = (ThreadSharedData *) arg;
    int id = getMyId(sharedData);
    fprintf(stderr, "consumer %d started\n", id);
    int ret = SUCCESS;

    if (id == -1) {
        fprintf(stderr, "Error: can't find myself (consumer)\n");
//        ret = PROGRAMME_FAIL;
//        pthread_mutex_lock(&sharedData->readMutex);
//        sharedData->retVal = ret;
//        pthread_mutex_unlock(&sharedData->readMutex);
//        pthread_exit(NULL);
    }

    while (!isBufferEmpty(sharedData) || !shouldTerminate(sharedData)) {
        sem_wait(&sharedData->semaphore);

        if ((shouldCancel(sharedData) || shouldTerminate(sharedData)) && isBufferEmpty(sharedData)) {
            fprintf(stderr, "consumer %d cancelled\n", id);
            pthread_exit(NULL);
        }

        pthread_mutex_lock(&sharedData->readMutex);
        node *n = (node *) pop(sharedData->buffer);
        DataItem *item = (DataItem *) n->data;
        pthread_mutex_unlock(&sharedData->readMutex);


        int atoiRet = atoi(item->str);
        if (atoiRet == 0 && item->count >= 0) {
            pthread_mutex_lock(&sharedData->writeMutex);
            printf("Thread %d:", id);
            for (int i = 0; i < item->count; ++i) {
                printf(" %s", item->str);
            }
            printf("\n");
            pthread_mutex_unlock(&sharedData->writeMutex);
        } else {
            fprintf(stderr, "Error: invalid input\n");
            ret = PROGRAMME_FAIL;
            pthread_mutex_lock(&sharedData->readMutex);
            sharedData->retVal = ret;
            pthread_mutex_unlock(&sharedData->readMutex);
        }

        free(item->str);
        free(item);
        free(n);
    }

    fprintf(stderr, "consumer %d exited\n", id);
    pthread_exit(NULL);
}

