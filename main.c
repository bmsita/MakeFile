#include <stdio.h>
#include <stdlib.h>
#include "tracker.h"

int main() {
    void* ptr[100] = { NULL };
    int choice, index;
    size_t size;

    printf("===== Memory Tracker Menu =====\n");
    while (1) {
        printf("\n1. Allocate Memory (malloc)\n");
        printf("2. Allocate Memory (calloc)\n");
        printf("3. Reallocate Memory\n");
        printf("4. Free Memory\n");
        printf("5. View Current Allocations\n");
        printf("6. Free ALL allocated memory\n");
        printf("0. Exit\n");
        printf("Enter your choice: ");
        scanf("%d", &choice);

        switch (choice) {
            case 1:
                printf("Enter size to malloc: ");
                scanf("%zu", &size);
                ptr[0] = track_malloc(size);
                printf("Memory allocated at ptr[0] = %p\n", ptr[0]);
                break;

            case 2:
                printf("Enter size to calloc: ");
                scanf("%zu", &size);
                ptr[1] = track_calloc(1, size);
                printf("Memory allocated at ptr[1] = %p\n", ptr[1]);
                break;

            case 3:
                printf("Realloc ptr[0]. Enter new size: ");
                scanf("%zu", &size);
                ptr[0] = track_realloc(ptr[0], size);
                printf("Reallocated ptr[0] to %p\n", ptr[0]);
                break;

            case 4:
                printf("Free which ptr (0 or 1)? ");
                scanf("%d", &index);
                track_free(ptr[index]);
                ptr[index] = NULL;
                break;

            case 5:
                print_allocations();
                break;

            case 6:
                free_all_memory();
                printf("Freed all allocations.\n");
                break;

            case 0:
                print_memory_summary();
                return 0;

            default:
                printf("Invalid choice.\n");
        }
    }
}
