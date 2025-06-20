#include <libraCore.h>

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#include "windows/libraC-windows.h"

// ===================================================================================================================
// Utility Operations
// ===================================================================================================================
void LC_SwapIntegers(int32 *x, int32 *y) {
    const int32 temp = *x;
    *x = *y;
    *y = temp;
}

// ===================================================================================================================
// Memory Allocations
// ===================================================================================================================
bool LC_IsPowerOfTwo(const uintptr_t x) {
    return (x & (x - 1)) == 0;
}

uintptr_t LC_AlignForward(const uintptr_t ptr, const size_t align) {
    assert(LC_IsPowerOfTwo(align));

    uintptr_t p = ptr;
    const uintptr_t a = align;
    // Same as (p % a) but faster as 'a' is a power of two
    const uintptr_t modulo = p & (a - 1);

    if (modulo != 0) {
        // if 'p' address is not aligned, push the address to the next value which is aligned
        p += a - modulo;
    }

    return p;
}

void* LC_AllocateAndAlignArena(Arena *arena, const size_t size, const size_t align) {
    // Align 'currentOffset' forward to the specified alignment
    const uintptr_t currentPointer = (uintptr_t)arena->buffer + arena->currentOffset;
    uintptr_t offset = LC_AlignForward(currentPointer, align);
    offset -= (uintptr_t)arena->buffer;

    // check to see if the backing memory has space left
    if (offset + size <= arena->bufferLength) {
        void *pointer = &arena->buffer[offset];
        arena->previousOffset = offset;
        arena->currentOffset = offset + size;

        // zero new memory by default
        memset(pointer, 0, size);
        return pointer;
    }
    // return NULL if the arena is out of memory (or handle differently)
    return NULL;
}

void* LC_AllocateArena(Arena *arena, const size_t size) {
    return LC_AllocateAndAlignArena(arena, size, DEFAULT_ALIGNMENT);
}

void LC_InitializeArena(Arena *arena, void *backingBuffer, const size_t backingBufferLength) {
    arena->buffer = (uchar*)backingBuffer;
    arena->bufferLength = backingBufferLength;
    arena->currentOffset = 0;
    arena->previousOffset = 0;
}

void LC_FreeArena(Arena *arena, void *pointer) {
    // do nothing
}

void* LC_ResizeAndAlignArena(Arena *arena, void *oldMemory, const size_t oldSize, const size_t newSize,
                             const size_t align) {
    assert(LC_IsPowerOfTwo(align));

    if (oldMemory == NULL || oldSize == 0) return LC_AllocateAndAlignArena(arena, newSize, align);
    if (arena->buffer <= (uchar*)oldMemory && (uchar*)oldMemory < arena->buffer + arena->bufferLength) {
        if (arena->buffer + arena->previousOffset == oldMemory) {
            arena->currentOffset = arena->previousOffset + newSize;
            if (newSize > oldSize) {
                memset(&arena->buffer[arena->currentOffset], 0, newSize - oldSize);
            }
            return oldMemory;
        }
        void *newMemory = LC_AllocateAndAlignArena(arena, newSize, align);
        const size_t copySize = oldSize < newSize ? oldSize : newSize;
        // copy across old memory ot the new memory
        memmove(newMemory, oldMemory, copySize);
        return newMemory;
    }
    assert(0 && "Memory is out of bounds of the buffer in this arena");
    return NULL;
}

void* LC_ResizeArena(Arena *arena, void *oldMemory, const size_t oldSize, const size_t newSize) {
    return LC_ResizeAndAlignArena(arena, oldMemory, oldSize, newSize, DEFAULT_ALIGNMENT);
}

void LC_FreeAllArena(Arena *arena) {
    arena->currentOffset = 0;
    arena->previousOffset = 0;
}

TemporaryArenaMemory LC_BeginTemporaryArenaMemory(Arena *arena) {
    TemporaryArenaMemory temporaryArena;
    temporaryArena.arena = arena;
    temporaryArena.previousOffset = arena->previousOffset;
    temporaryArena.currentOffset = arena->currentOffset;

    return temporaryArena;
}

void LC_EndTemporaryArena(const TemporaryArenaMemory temporaryArena) {
    temporaryArena.arena->previousOffset = temporaryArena.previousOffset;
    temporaryArena.arena->currentOffset = temporaryArena.currentOffset;
}

// ===================================================================================================================
// File Operations
// ===================================================================================================================

void LC_GetFileContent(Arena *arena, const char *filePath, char **fileContents) {
    if (filePath == NULL) return;

    // Open the file in "read mode"
    FILE *file = fopen(filePath, "r");

    if (file == NULL) return;
    // Move the point32er of the current position in the file to the end.
    fseek(file, 0, SEEK_END);
    // Traverse the file from the beginning of the file to the current position. The current position of the point32er
    // was set to be the end of the file with SEEK_END in the previous statement.
    const int32 length = ftell(file);
    // Move the point32er of the current position back to the beginning of the file
    fseek(file, 0, SEEK_SET);

    // Allocate memory for the string. We add 1 to the length to store the null terminating character '\0' at the end.
    *fileContents = LC_AllocateArena(arena, sizeof(char) * (length + 1));

    char c;
    int32 i = 0;

    // fgetc will return each character until it get to the end of file where it will return EOF and the loop will end.
    while ((c = (char)fgetc(file)) != EOF) {
        (*fileContents)[i] = c;
        i++;
    }

    (*fileContents)[i] = '\0';

    fclose(file);
}

// ===================================================================================================================
// Strings and String Operations
// ===================================================================================================================

void LC_InitializeString(LC_String *string, char *cString) {
    uint32 count = 0;

    while (cString[count] != '\0') {
        count++;
    }

    string->length = count;
    string->data = cString;
}

// ===================================================================================================================
// Sorting Algorithms
// ===================================================================================================================
void LC_QuickSortIntegers(int32 *array, const int32 length) {
    srand(time(nullptr));

    LC_QuickSortIntegersRecursive(array, 0, length - 1);
}

void LC_QuickSortIntegersRecursive(int32 *array, const int32 low, int32 high) {
    if (low < high) {
        const int pivotIndex = LC_QSIntegersPartition(array, low, high);
        LC_QuickSortIntegersRecursive(array, low, pivotIndex - 1);
        LC_QuickSortIntegersRecursive(array, pivotIndex + 1, high);
    }
}

int32 LC_QSIntegersPartition(int32 *array, const int32 low, const int32 high) {
    const int32 pivotIndex = low + (rand() % (high - low));
    if (pivotIndex != high) LC_SwapIntegers(&array[pivotIndex], &array[high]);

    const int32 pivotValue = array[high];

    int32 i = low;

    for (int32 j = low; j < high; ++j) {
        if (array[j] <= pivotValue) {
            LC_SwapIntegers(&array[i], &array[j]);
            i++;
        }
    }
    LC_SwapIntegers(&array[i], &array[high]);
    return i;
}

void LC_MergeSortIntegers(int32 *array, const uint32 size) {
    if (size <= 0) return;
    LC_MergeSortIntegersRecursive(array, 0, size - 1);
}

void LC_MergeSortIntegersRecursive(int32 *array, const int32 low, const int32 high) {
    if (high <= low) return;

    const int mid = low + (high - low) / 2;
    LC_MergeSortIntegersRecursive(array, low, mid);
    LC_MergeSortIntegersRecursive(array, mid + 1, high);
    LC_MergeIntegers(array, low, mid, high);
}

void LC_MergeIntegers(int32 *array, const int32 low, const int32 mid, const int32 high) {
    // Create left ← array[p..q] and right ← array[q+1..r]
    int32 n1 = mid - low + 1;
    int32 n2 = high - mid;

    int32 left[n1], right[n2];

    for (size_t i = 0; i < n1; i++)
        left[i] = array[low + i];
    for (size_t j = 0; j < n2; j++)
        right[j] = array[mid + 1 + j];

    // Maintain current index of sub-arrays and main array
    int32 i = 0;
    int32 j = 0;
    int32 k = low;

    // Until we reach either end of either left or right, pick larger among
    // elements of left and right and place them in the correct position at array[p..r]
    while (i < n1 && j < n2) {
        if (left[i] <= right[j]) {
            array[k] = left[i];
            i++;
        }
        else {
            array[k] = right[j];
            j++;
        }
        k++;
    }

    // When we run out of elements in either left or right,
    // pick up the remaining elements and put in array[p..r]
    while (i < n1) {
        array[k] = left[i];
        i++;
        k++;
    }

    while (j < n2) {
        array[k] = right[j];
        j++;
        k++;
    }
}

// ===================================================================================================================
// Environment Information
// ===================================================================================================================
void LC_GetCurrentWorkingDirectory(Arena *arena, char **currentDirectory) {
    constexpr size_t size = 128;
    *currentDirectory = LC_AllocateArena(arena, size);
    if (*currentDirectory == NULL) {
        *currentDirectory = "Could not allocate memory!";
        return;
    }
#ifdef __WIN32
    LC_Win32_GetCurrentWorkingDirectory(*currentDirectory, size);
#elif __linux__
    LC_Linux_GetCurrentWorkingDirectory(*currentDirectory, size);
#elif __APPLE__
    // Not yet implemented
#endif
}
