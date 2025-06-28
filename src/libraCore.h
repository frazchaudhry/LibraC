#ifndef LIBRACORE_H
#define LIBRACORE_H

#include <libraC.h>

// ===================================================================================================================
// Utility Operations
// ===================================================================================================================
void LC_SwapIntegers(int32 *x, int32 *y);

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
} Arena;

typedef struct {
    Arena *arena;
    size_t previousOffset;
    size_t currentOffset;
} TemporaryArenaMemory;

bool LC_IsPowerOfTwo(uintptr_t x);
uintptr_t LC_AlignForward(uintptr_t ptr, size_t align);
void* LC_AllocateAndAlignArena(Arena *arena, size_t size, size_t align);
void* LC_AllocateArena(Arena *arena, size_t size);
void LC_InitializeArena(Arena *arena, void *backingBuffer, size_t backingBufferLength);
void LC_FreeArena(Arena *arena, void *pointer);
void* LC_ResizeAndAlignArena(Arena *arena, void *oldMemory, size_t oldSize, size_t newSize, size_t align);
void* LC_ResizeArena(Arena *arena, void *oldMemory, size_t oldSize, size_t newSize);
void LC_FreeAllArena(Arena *arena);

TemporaryArenaMemory LC_BeginTemporaryArenaMemory(Arena *arena);
void LC_EndTemporaryArena(TemporaryArenaMemory temporaryArena);

// ===================================================================================================================
// File Operations
// ===================================================================================================================

void LC_GetFileContentString(Arena *arena, const char *filePath, char **fileContents);
void LC_GetFileContentBinary(Arena *arena, const char *filePath, uchar **fileContents, size_t *fileSize);

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
// Sorting Algorithms
// ===================================================================================================================
void LC_QuickSortIntegers(int32 *array, int32 length);
void LC_QuickSortIntegersRecursive(int32 *array, int32 low, int32 high);
int32 LC_QSIntegersPartition(int32 *array, int32 low, int32 high);

void LC_MergeSortIntegers(int32 *array, const uint32 size);
void LC_MergeSortIntegersRecursive(int32 *array, int32 low, int32 high);
void LC_MergeIntegers(int32 *array, int32 low, int32 mid, int32 high);

// ===================================================================================================================
// Environment Information
// ===================================================================================================================
void LC_GetCurrentWorkingDirectory(Arena *arena, char **currentDirectory);

#endif //LIBRACORE_H
