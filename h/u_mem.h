#include <stdlib.h>

static void *uMemAlloc(size_t size)
{
    void *allocation = malloc(size);
/*
    static void **curAllocs;

    size_t newAllocSize = curAllocs == NULL ? 1 : (sizeof(*curAllocs) / sizeof(void *)) + 1;
    void **newAllocs = (void **)malloc(newAllocSize * sizeof(void *));
    for (size_t i = 0; i < newAllocSize - 1; i++)
    {
        newAllocs[i] = curAllocs[i];
    }
    newAllocs[newAllocSize - 1] = allocation;
    curAllocs = newAllocs;
*/
    return allocation;
}