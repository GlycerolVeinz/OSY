
#include <mem.h>

void *checked_mallock(size_t size)
{
    void *ptr = malloc(size);
    if (!ptr) {
        fprintf(stderr, "Error: malloc failed");
        exit(1);
    }
    return ptr;
}

void *checked_calloc(size_t nmemb, size_t size)
{
    void *ptr = calloc(nmemb, size);
    if (!ptr) {
        fprintf(stderr, "Error: calloc failed");
        exit(1);
    }
    return ptr;
}