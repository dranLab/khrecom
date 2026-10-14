#ifndef SYSTEM_HEAPMAN_H
#define SYSTEM_HEAPMAN_H

#include "khtypes.h"
#include <string.h>

extern "C" {
void* com_malloc(size_t bytes);
void* com_realloc(void* mem, size_t newsize);
void* com_calloc(size_t n_elements, size_t elem_size);
void* com_memalign(size_t alignment, size_t bytes);
void com_free(void* mem);
void* libc_malloc(size_t size);
void libc_free(void* buffer);
void* com_malloc_d(size_t bytes, char* msg, unsigned int ra);
void* com_realloc_d(void* mem, size_t newsize, char* msg, unsigned int ra);
void* com_calloc_d(size_t n_elements, size_t elem_size, char* msg, unsigned int ra);
void* com_memalign_d(size_t alignment, size_t bytes, char* msg, unsigned int ra);
void _heap_dump(char* pszFileName, bool bCreate);
void dump_heap(char* pszFileName, bool bCreate);
}

void dbgReportHeapPerFile();

inline void operator delete(void* mem) { com_free(mem); }

#endif /* SYSTEM_HEAPMAN_H */
