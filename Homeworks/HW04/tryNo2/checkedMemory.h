#ifndef CHECKEDMEMORY_H
#define CHECKEDMEMORY_H

#include <stdlib.h>
#include <stdio.h>

void *checked_mallock(size_t size);
void *checked_calloc(size_t nmemb, size_t size);

#endif //CHECKEDMEMORY_H
