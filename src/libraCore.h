#ifndef LIBRACORE_H
#define LIBRACORE_H

#include <libraC.h>

// ===================================================================================================================
// #defines
// ===================================================================================================================

#define ASSERT(c, m) \
do { \
    if (!(c)) { \
        SDL_LogError(SDL_LOG_CATEGORY_ERROR, __FILE__ ":%d: assertion %s failed: %s", \
        __LINE__, #c, m); \
        debug_break(); \
    } \
} while(0)

#ifndef DEFAULT_ALIGNMENT
#define DEFAULT_ALIGNMENT (2*sizeof(void *))
#endif

// ===================================================================================================================
// Structs
// ===================================================================================================================

typedef struct {
    uint32 length;
    char *data;
} LC_String;


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

typedef struct list {
    uchar *_data;
    uint32 _length;
    uint32 _actualBufferSize;
    size_t _sizeOfElement;
} LC_List;


// ===================================================================================================================
// Strings and String Operations
// ===================================================================================================================

void LC_String_Initialize(LC_String *string, char *cString);
void LC_String_InitializeByCopy(LC_Arena *arena, LC_String *string, const char *cString);
bool LC_String_IsEqualCString(const LC_String *string, const char *cString);
bool LC_String_IsEqual(const LC_String *str1, const LC_String *str2);
uint32 LC_GetStringLengthSkipSpaces(const char *string, uint32 length);

// ===================================================================================================================
// Utility Operations
// ===================================================================================================================

void LC_SwapValues(void *x, void *y, size_t sizeOfElement, bool *success);

// ===================================================================================================================
// Memory Allocations
// ===================================================================================================================
bool LC_IsPowerOfTwo(uintptr_t x);
uintptr_t LC_AlignForward(uintptr_t ptr, size_t align);
void* LC_AllocateAndAlignArena(LC_Arena *arena, size_t size, size_t align);
void* LC_Arena_Allocate(LC_Arena *arena, size_t size);
void LC_Arena_Initialize(LC_Arena *arena, void *backingBuffer, size_t backingBufferLength);
void LC_Arena_Free(LC_Arena *arena, void *pointer);
void* LC_Arena_ResizeAndAlign(LC_Arena *arena, void *oldMemory, size_t oldSize, size_t newSize, size_t align);
void* LC_Arena_Resize(LC_Arena *arena, void *oldMemory, size_t oldSize, size_t newSize);
void LC_Arena_FreeAll(LC_Arena *arena);

TemporaryArenaMemory LC_Arena_BeginTemporaryMemory(LC_Arena *arena);
void LC_Arena_EndTemporary(TemporaryArenaMemory temporaryArena);

// ===================================================================================================================
// File Operations
// ===================================================================================================================

void LC_GetFileContentString(LC_Arena *arena, const char *filePath, char **fileContents);
bool LC_GetFileContentBinary(LC_Arena *arena, const char *filePath, uchar **fileContents, size_t *fileSize, char *errorLog);

// ===================================================================================================================
// Data Structures
// ===================================================================================================================

void LC_List_Initialize(LC_List *list, size_t sizeOfElement);
uint32 LC_List_GetLength(const LC_List *list);
void* LC_List_GetData(const LC_List *list);
void* LC_List_AddElement(LC_List *list, const void *element);
void* LC_List_GetElement(const LC_List *list, uint32 index);
void LC_List_Destroy(LC_List *list);

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
