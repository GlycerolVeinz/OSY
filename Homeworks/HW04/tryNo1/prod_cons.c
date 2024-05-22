#include "prod_cons.h"

int main(int argc, char const *argv[])
{
    int num_of_consuments = getNumOfConsuments(argc, argv);

    // create shared data
    ThreadSharedData *sharedData = sharedData_init(num_of_consuments);

    // Create producer thread pass shared data
    pthread_t producer_id;
    if (pthread_create(&producer_id, NULL, producer, sharedData) != 0)
    {
        perror("Error: producer thread creation failed");
        sharedData_destroy(sharedData);
        exit(PROGRAMME_FAIL);
    }

    // create consumer threads (num_of_consuments) pass shared data

    for (int i = 0; i < num_of_consuments; ++i)
    {
        if (pthread_create(&sharedData->consumer_ids[i], NULL, consumer, sharedData) != 0)
        {
            perror("Error: consumer thread creation failed");
            sharedData_destroy(sharedData);
            exit(PROGRAMME_FAIL);
        }
    }

    // join producer thread
    int producer_ret;
    pthread_join(producer_id, (void **)&producer_ret);

    if (producer_ret != SUCCESS)
    {
        perror("Error: producer failed");

        for (int i = 0; i < num_of_consuments; ++i)
        {
            pthread_cancel(sharedData->consumer_ids[i]);
        }
        sharedData_destroy(sharedData);
        exit(PROGRAMME_FAIL);
    }

    // join consumer threads
    int *consumer_ret;
    for (int i = 0; i < num_of_consuments; ++i)
    {
        pthread_join(sharedData->consumer_ids[i], (void **)&consumer_ret);
        if ((void *) consumer_ret == NULL){
            continue;
        }
        if (*consumer_ret != SUCCESS)
        {
            perror("Error: consumer failed");
            sharedData_destroy(sharedData);
            exit(PROGRAMME_FAIL);
        }
    }

    sharedData_destroy(sharedData);
    exit(SUCCESS);
}

/*
    CONSUMER

    Waits for dataItem in array
        read dataItem
        print dataItem in format "Thread %threadID: " + count * msg

*/
void *consumer(void *arg)
{
    ThreadSharedData *sharedData = (ThreadSharedData *)arg;

    // find consumer id, and assign it an int value for print
    int id = getMyId(sharedData);
    if (id == -1)
    {
        perror("Error: consumer id not found");
        pthread_exit((void *)PROGRAMME_FAIL);
    }
    int sem_ret;

    // wait for dataItem in array
    while (!sharedData->terminate || !isBufferEmpty(sharedData))
    {
        sem_ret = sem_wait(&sharedData->semaphore);
        if (sem_ret == THREAD_FAIL)
        {
            perror("Error: semaphore wait failed");
            pthread_exit((void *)PROGRAMME_FAIL);
        }
        readWriteData(sharedData, id);
    }

    pthread_exit(SUCCESS);
}

int getMyId(ThreadSharedData *sharedData)
{
    int id = -1;
    pthread_t self = pthread_self();

    pthread_mutex_lock(&sharedData->mutex);

    for (int i = 0; i < sharedData->bufferSize; ++i)
    {
        if (sharedData->consumer_ids[i] == self)
        {
            id = i;
            break;
        }
    }

    pthread_mutex_unlock(&sharedData->mutex);

    return id;
}

void readWriteData(ThreadSharedData *sharedData, int id)
{
    // read dataItem safely
    DataItem *dataItem = NULL;
    int at;

    pthread_mutex_lock(&sharedData->mutex);

    for (int i = 0; i < sharedData->bufferSize; ++i)
    {
        if (!sharedData->isBufferIndexEmpty[i])
        {
            dataItem = &sharedData->buffer[i];
            at = i;
            break;
        }
    }
    pthread_mutex_unlock(&sharedData->mutex);


    // print dataItem in format "Thread %threadID: " + count * msg
    if (dataItem != NULL && dataItem->str != NULL)
    {
        printf("\nThread %d:", id);
        for (int i = 0; i < dataItem->count; ++i)
        {
            printf("%s ", dataItem->str);
        }

        pthread_mutex_lock(&sharedData->mutex);
        sharedData->buffer[at].str = NULL;
        sharedData->isBufferIndexEmpty[at] = TRUE;
        free(dataItem->str);
        pthread_mutex_unlock(&sharedData->mutex);
    }
}

/*
    PRODUCER

    Reads from stdin lines formatted as "%d %ms"
        if not valid, exit(1)

        send data to array
        wake up maximally P consumers for P added dataItems

    free memory
    exit(0)
*/
void *producer(void *arg)
{
    ThreadSharedData *sharedData = (ThreadSharedData *)arg;

    // read from stdin
    int read, int_in;
    String str_in;
    DataItem msg;

    while (2 == (read = scanf("%d %ms\n", &int_in, &str_in)))
    {
        if (read != 2)
        {
            perror("Error: producer failed in scanf");
            pthread_exit((void *)PROGRAMME_FAIL);
        }

        msg.count = int_in;
        msg.str = str_in;

        // send data pointer to array
        if (writeToBuffer(sharedData, msg) == PROGRAMME_FAIL)
        {
            perror("Error: producer failed in writeToBuffer");
            pthread_exit((void *)PROGRAMME_FAIL);
        }
        // wake up maximally P consumers for P added dataItems
        sem_post(&sharedData->semaphore);
    }

    terminateConsumers(sharedData);

    pthread_exit(SUCCESS);
}

int writeToBuffer(ThreadSharedData *sharedData, DataItem dataItem)
{
    int ret = PROGRAMME_FAIL;

    pthread_mutex_lock(&sharedData->mutex);

    for (int i = 0; i < sharedData->bufferSize; ++i)
    {
        if (sharedData->isBufferIndexEmpty[i])
        {
            sharedData->isBufferIndexEmpty[i] = FALSE;
            sharedData->buffer[i] = dataItem;
            ret = SUCCESS;
            break;
        }
    }

    pthread_mutex_unlock(&sharedData->mutex);

    return ret;
}

void terminateConsumers(ThreadSharedData *sharedData)
{
    pthread_mutex_lock(&sharedData->mutex);
    sharedData->terminate = true;
    pthread_mutex_unlock(&sharedData->mutex);

    for (int i = 0; i < sharedData->bufferSize; ++i)
    {
        sem_post(&sharedData->semaphore);
    }
}
