// memory_tracker.c
#include "memory_tracker.h"

MemoryBlock *head = NULL;

static void add_to_list(void *address, size_t size) {
    MemoryBlock *new_block = (MemoryBlock *)malloc(sizeof(MemoryBlock));
    new_block->address = address;
    new_block->size = size;
    new_block->timestamp = time(NULL);
    new_block->is_freed = 0;
    new_block->next = head;
    head = new_block;
}

static MemoryBlock *find_block(void *address) {
    MemoryBlock *current = head;
    while (current) {
        if (current->address == address)
            return current;
        current = current->next;
    }
    return NULL;
}

void *track_malloc(size_t size) {
    void *ptr = malloc(size);
    if (ptr) {
        add_to_list(ptr, size);
        log_allocation(ptr, size);
    }
    return ptr;
}

void *track_calloc(size_t num, size_t size) {
    void *ptr = calloc(num, size);
    if (ptr) {
        add_to_list(ptr, num * size);
        log_allocation(ptr, num * size);
    }
    return ptr;
}

void *track_realloc(void *ptr, size_t size) {
    if (!ptr) return track_malloc(size);
    MemoryBlock *block = find_block(ptr);
    if (block && !block->is_freed) {
        void *new_ptr = realloc(ptr, size);
        if (new_ptr) {
            block->address = new_ptr;
            block->size = size;
            block->timestamp = time(NULL);
            log_allocation(new_ptr, size);
        }
        return new_ptr;
    }
    return NULL;
}

void track_free(void *ptr) {
    if (!ptr) return;
    MemoryBlock *block = find_block(ptr);
    if (block && !block->is_freed) {
        block->is_freed = 1;
        log_deallocation(ptr);
        free(ptr);
    } else {
        printf("Warning: Double free or invalid free detected.\n");
    }
}

void log_allocation(void *address, size_t size) {
    FILE *log = fopen("memory_log.txt", "a");
    if (log) {
        fprintf(log, "ALLOC | Address: %p | Size: %zu | Time: %s", address, size, ctime(&(time_t){time(NULL)}));
        fclose(log);
    }
}

void log_deallocation(void *address) {
    FILE *log = fopen("memory_log.txt", "a");
    if (log) {
        fprintf(log, "FREE  | Address: %p | Time: %s", address, ctime(&(time_t){time(NULL)}));
        fclose(log);
    }
}

void print_current_allocations() {
    MemoryBlock *current = head;
    printf("\n--- Active Memory Allocations ---\n");
    while (current) {
        if (!current->is_freed) {
            printf("Address: %p, Size: %zu bytes\n", current->address, current->size);
        }
        current = current->next;
    }
}

void free_all_memory() {
    MemoryBlock *current = head;
    while (current) {
        if (!current->is_freed) {
            free(current->address);
            current->is_freed = 1;
            log_deallocation(current->address);
        }
        current = current->next;
    }
}

void print_memory_summary() {
    MemoryBlock *current = head;
    size_t total_leaked = 0;
    int leaked_blocks = 0;
    while (current) {
        if (!current->is_freed) {
            total_leaked += current->size;
            leaked_blocks++;
        }
        current = current->next;
    }
    printf("\n--- Memory Summary ---\n");
    printf("Leaked Memory: %zu bytes in %d blocks\n", total_leaked, leaked_blocks);
}
