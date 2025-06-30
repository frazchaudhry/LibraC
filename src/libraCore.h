#ifndef LIBRACORE_H
#define LIBRACORE_H

#include <libraC.h>

// ===================================================================================================================
// Strings and String Operations
// ===================================================================================================================

typedef struct {
    uint32 length;
    char *data;
} LC_String;

void LC_InitializeString(LC_String *string, char *cString);
uint32 LC_GetStringLengthSkipSpaces(const LC_String *string);

// ===================================================================================================================
// Utility Operations
// ===================================================================================================================
void LC_SwapValues(void *x, void *y, size_t sizeOfElement, bool *success);

// ===================================================================================================================
// Memory Allocations
// ===================================================================================================================
#ifndef DEFAULT_ALIGNMENT
#define DEFAULT_ALIGNMENT (2*sizeof(void *))
#endif

typedef struct {
    uchar *buffer;
    size_t bufferLength;
    size_t previousOffset;
    size_t currentOffset;
} LC_Arena;

typedef struct {
    LC_Arena *arena;
    size_t previousOffset;
    size_t currentOffset;
} TemporaryArenaMemory;

bool LC_IsPowerOfTwo(uintptr_t x);
uintptr_t LC_AlignForward(uintptr_t ptr, size_t align);
void* LC_AllocateAndAlignArena(LC_Arena *arena, size_t size, size_t align);
void* LC_AllocateArena(LC_Arena *arena, size_t size);
void LC_InitializeArena(LC_Arena *arena, void *backingBuffer, size_t backingBufferLength);
void LC_FreeArena(LC_Arena *arena, void *pointer);
void* LC_ResizeAndAlignArena(LC_Arena *arena, void *oldMemory, size_t oldSize, size_t newSize, size_t align);
void* LC_ResizeArena(LC_Arena *arena, void *oldMemory, size_t oldSize, size_t newSize);
void LC_FreeAllArena(LC_Arena *arena);

TemporaryArenaMemory LC_BeginTemporaryArenaMemory(LC_Arena *arena);
void LC_EndTemporaryArena(TemporaryArenaMemory temporaryArena);

// ===================================================================================================================
// File Operations
// ===================================================================================================================

void LC_GetFileContentString(LC_Arena *arena, const char *filePath, char **fileContents);
void LC_GetFileContentBinary(LC_Arena *arena, const char *filePath, uchar **fileContents, size_t *fileSize);

// ===================================================================================================================
// Data Structures
// ===================================================================================================================
typedef struct list {
    uchar *_data;
    uint32 _length;
    uint32 _actualBufferSize;
    size_t _sizeOfElement;
} LC_List;

void LC_ListInitialize(LC_List *list, size_t sizeOfElement);
void LC_ListAdd(LC_List *list, void *element);
void LC_ListDestroy(LC_List *list);

// ===================================================================================================================
// Sorting Algorithms
// ===================================================================================================================
void LC_QuickSortIntegers(int32 *array, int32 length);
void LC_QuickSortIntegersRecursive(int32 *array, int32 low, int32 high);
int32 LC_QSIntegersPartition(int32 *array, int32 low, int32 high);

void LC_MergeSortIntegers(int32 *array, uint32 size);
void LC_MergeSortIntegersRecursive(int32 *array, int32 low, int32 high);
void LC_MergeIntegers(int32 *array, int32 low, int32 mid, int32 high);

// ===================================================================================================================
// Environment Information
// ===================================================================================================================
void LC_GetCurrentWorkingDirectory(LC_Arena *arena, char **currentDirectory);

#endif //LIBRACORE_H
