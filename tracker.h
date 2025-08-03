#ifndef TRACKER_H
#define TRACKER_H

#include <time.h>

typedef struct MemBlock {
    void* address;
    size_t size;
    time_t timestamp;
    struct MemBlock* next;
} MemBlock;

void* track_malloc(size_t size);
void* track_calloc(size_t num, size_t size);
void* track_realloc(void* ptr, size_t size);
void track_free(void* ptr);
void free_all_memory();
void print_memory_summary();
void print_allocations();

#endif
