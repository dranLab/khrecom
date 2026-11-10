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
void* (com_malloc_d)(size_t bytes, char* msg, unsigned int ra);
void* (com_realloc_d)(void* mem, size_t newsize, char* msg, unsigned int ra);
void* (com_calloc_d)(size_t n_elements, size_t elem_size, char* msg, unsigned int ra);
void* (com_memalign_d)(size_t alignment, size_t bytes, char* msg, unsigned int ra);
void _heap_dump(char* pszFileName, bool bCreate);
void dump_heap(char* pszFileName, bool bCreate);
}

void dbgReportHeapPerFile();

class CHeapMan {
public:
    static void createHeap(void* base, size_t capacity);
    static void resetHeap();
    static size_t getFreeHeapSize();
    static size_t getUsedHeapSize();
    static float getUsedHeapRatio();
    static size_t getAllHeapSize();
    static size_t getMaxFreeBlockSize();
    static void reportMemory();
    static CHAR* getDebugComment(void* pMem);
};

/* The global operators, as the original's System/HeapMan.h has them (its .mdebug line labels: new 134, delete
 * 136/137, new(size_t, char*) 150/152/153, new[] 156/158/159, placement new 168, all throw() where the caller
 * null-checks: placement new too, V3 decides). The sites are `new (__FILE__ "(N)") T` and `new (__FILE__ "(N)") T[n]`
 * (every com_malloc_d call of the original that passes $31 is one of these two operators). new(size_t, char*) and
 * new[] keep the return address in a function-level `UINT nRA` (the ref's locals; .mdebug block `{ { nRA } }` per
 * site). MATCH: the return address is inline asm (a move from $31): __builtin_return_address(0) reloads ra from the
 * frame instead. */
inline void* operator new(size_t size) throw() { return com_malloc(size); }
inline void operator delete(void* mem) { com_free(mem); }
inline void operator delete[](void* mem) { com_free(mem); }
inline void* operator new(size_t size, char* msg) throw() {
    UINT nRA;
    KH_DEBUG(__asm__ volatile("daddu %0, $31, $0" : "=r"(nRA));)
    return com_malloc_d(size, msg, nRA);
}

inline void* operator new[](size_t size, char* msg) throw() {
    UINT nRA;
    KH_DEBUG(__asm__ volatile("daddu %0, $31, $0" : "=r"(nRA));)
    return com_malloc_d(size, msg, nRA);
}
inline void* operator new(size_t, void* p) throw() { return p; } KH_RELEASE(inline void* operator new[](size_t size) throw() { return com_malloc(size); })

#endif /* SYSTEM_HEAPMAN_H */
