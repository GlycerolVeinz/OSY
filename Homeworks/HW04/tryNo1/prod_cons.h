#ifndef PROD_CONS_H
#define PROD_CONS_H

#include "utils.h"

void *producer(void *arg);
int writeToBuffer(ThreadSharedData *sharedData, DataItem dataItem);
void terminateConsumers(ThreadSharedData *sharedData);
void *consumer(void *arg);
int getMyId(ThreadSharedData *sharedData);
void readWriteData(ThreadSharedData *sharedData, int id);


#endif // PROD_CONS_H
