#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "tracker.h"
#include "logger.h"

static MemBlock* head = NULL;

void add_block(void* ptr, size_t size) {
    MemBlock* block = malloc(sizeof(MemBlock));
    block->address = ptr;
    block->size = size;
    block->timestamp = time(NULL);
    block->next = head;
    head = block;
    log_allocation(ptr, size, block->timestamp);
}

void remove_block(void* ptr) {
    MemBlock *curr = head, *prev = NULL;
    while (curr) {
        if (curr->address == ptr) {
            if (prev) prev->next = curr->next;
            else head = curr->next;
            free(curr);
            return;
        }
        prev = curr;
        curr = curr->next;
    }
}

void* track_malloc(size_t size) {
    void* ptr = malloc(size);
    if (ptr) add_block(ptr, size);
    return ptr;
}

void* track_calloc(size_t num, size_t size) {
    void* ptr = calloc(num, size);
    if (ptr) add_block(ptr, num * size);
    return ptr;
}

void* track_realloc(void* ptr, size_t size) {
    void* new_ptr = realloc(ptr, size);
    if (new_ptr) {
        remove_block(ptr);
        add_block(new_ptr, size);
    }
    return new_ptr;
}

void track_free(void* ptr) {
    if (ptr) {
        remove_block(ptr);
        free(ptr);
    }
}

void free_all_memory() {
    MemBlock* temp = head;
    while (temp) {
        free(temp->address);
        MemBlock* toFree = temp;
        temp = temp->next;
        free(toFree);
    }
    head = NULL;
}

void print_memory_summary() {
    size_t total = 0, count = 0;
    MemBlock* temp = head;
    while (temp) {
        count++;
        total += temp->size;
        temp = temp->next;
    }

    if (count == 0) {
        printf("All memory successfully freed. No leaks detected.\n");
    } else {
        printf("\n--- Memory Summary ---\n");
        printf("Leaked Memory: %zu bytes\n", total);
        printf("Total leaked blocks: %zu\n", count);
    }
}

void print_allocations() {
    if (!head) {
        printf("No active memory allocations.\n");
        return;
    }
    printf("\n===== Live Memory Tracker [%s] =====\n", __TIME__);
    MemBlock* temp = head;
    size_t total = 0, blocks = 0;
    while (temp) {
        printf("Address: %p | Size: %zu bytes\n", temp->address, temp->size);
        total += temp->size;
        blocks++;
        temp = temp->next;
    }
    printf("Total blocks: %zu | Total memory: %zu bytes\n", blocks, total);
}
