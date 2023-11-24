#include "prod-cons.h"

int producerRetVal = 0;

int main(int argc, char const *argv[])
{
    int numOfConsuments = getNumOfConsuments(argc, argv);
    ThreadSharedData *sharedData = sharedData_init(numOfConsuments);

    pthread_t producerId;
    if (pthread_create(&producerId, NULL, producer, sharedData) != SUCCESS)
    {
        perror("Error: producer thread creation failed");
        sharedData_destroy(sharedData);
        exit(PROGRAMME_FAIL);
    }

    for (int i = 0; i < numOfConsuments; ++i)
    {
        pthread_t consumerId;
        if (pthread_create(&consumerId, NULL, consumer, sharedData) != SUCCESS)
        {
            perror("Error: consumer thread creation failed");
            sharedData_destroy(sharedData);
            exit(PROGRAMME_FAIL);
        }
        sharedData->consumerIds[i] = consumerId;
    }

    pthread_join(producerId, NULL);

//    pthread_mutex_lock(&sharedData->readMutex);
    int producer_ret = producerRetVal;
//    pthread_mutex_unlock(&sharedData->readMutex);

     if (producer_ret != SUCCESS){
//        pthread_mutex_lock(&sharedData->readMutex);
        fprintf(stderr, "Error, producer exit");
//        pthread_mutex_unlock(&sharedData->readMutex);

         for (int i = 0; i < sharedData->bufferSize; ++i)
         {
             pthread_cancel(sharedData->consumerIds[i]);
         }
        sharedData_destroy(sharedData);
        exit(PROGRAMME_FAIL);
    }

    for (int i = 0; i < numOfConsuments; ++i)
    {
        pthread_join(sharedData->consumerIds[i], NULL);

        pthread_mutex_lock(&sharedData->readMutex);
        int consumer_ret = sharedData->retVal;
        pthread_mutex_unlock(&sharedData->readMutex);

        if (consumer_ret != SUCCESS)
        {
            fprintf(stderr, "Error, consumer %d failed", i);
            sharedData_destroy(sharedData);
            exit(PROGRAMME_FAIL);
        }
    }

    sharedData_destroy(sharedData);
    exit(SUCCESS);
}

void *producer(void *arg)
{
    ThreadSharedData *sharedData = (ThreadSharedData *)arg;
    int ret;
    int int_in;
    String str_in;

    while ((ret = scanf("%d %ms\n", &int_in, &str_in)) == 2)
    {
        if (ret != 2)
        {

            fprintf(stderr, "Error: scanf failed");
            producerRetVal = PROGRAMME_FAIL;

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

int getMyId(ThreadSharedData *sharedData)
{
    int id = -1;
    pthread_t self = pthread_self();

    pthread_mutex_lock(&sharedData->readMutex);
    for (int i = 0; i < sharedData->bufferSize; ++i)
    {
        if (sharedData->consumerIds[i] == self)
        {
            id = i;
            break;
        }
    }
    pthread_mutex_unlock(&sharedData->readMutex);

    return id;
}

bool shouldTerminate(ThreadSharedData *sharedData)
{
    pthread_mutex_lock(&sharedData->readMutex);
    bool terminate = sharedData->terminate;
    pthread_mutex_unlock(&sharedData->readMutex);
    return terminate;
}

void *consumer(void *arg){
    ThreadSharedData *sharedData = (ThreadSharedData *)arg;
    int ret = SUCCESS;
    int id = getMyId(sharedData);
    if (id == -1){
        fprintf(stderr, "Error: can't find myself (consumer)");
        ret = PROGRAMME_FAIL;
        pthread_mutex_lock(&sharedData->readMutex);
        sharedData->retVal = ret;
        pthread_mutex_unlock(&sharedData->readMutex);
        pthread_exit(NULL);
    }

    while (sem_wait(&sharedData->semaphore) && (!isBufferEmpty(sharedData) || !shouldTerminate(sharedData)))
    {
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

