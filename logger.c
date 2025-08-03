#include <stdio.h>
#include <time.h>
#include "logger.h"

void log_allocation(void* address, size_t size, time_t timestamp) {
    FILE* file = fopen("memory_log.txt", "a");
    if (file) {
        fprintf(file, "Time: %sAllocated Address: %p | Size: %zu bytes\n\n",
                ctime(&timestamp), address, size);
        fclose(file);
    }
}
