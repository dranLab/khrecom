#ifndef SYSTEM_DLMALLOC_H
#define SYSTEM_DLMALLOC_H

#include "khtypes.h"
#include <string.h>

typedef void* mspace;

#ifdef __cplusplus
extern "C" {
#endif

mspace create_mspace(size_t capacity, int locked);
mspace create_mspace_with_base(void* base, size_t capacity, int locked);
size_t destroy_mspace(mspace msp);
void* mspace_malloc(mspace msp, size_t bytes);
void mspace_free(mspace msp, void* mem);
void* mspace_calloc(mspace msp, size_t n_elements, size_t elem_size);
void* mspace_realloc(mspace msp, void* oldmem, size_t bytes);
void* mspace_memalign(mspace msp, size_t alignment, size_t bytes);
void** mspace_independent_calloc(mspace msp, size_t n_elements, size_t elem_size, void** chunks);
void** mspace_independent_comalloc(mspace msp, size_t n_elements, size_t* sizes, void** chunks);
int mspace_trim(mspace msp, size_t pad);
void mspace_malloc_stats(mspace msp);
size_t mspace_footprint(mspace msp);
int mspace_mallopt(int param_number, int value);
void* mspace_malloc_d(mspace msp, size_t bytes, char* msg, unsigned int ra);
void* mspace_realloc_d(mspace msp, void* mem, size_t newsize, char* msg, unsigned int ra);
void* mspace_calloc_d(mspace msp, size_t n_elements, size_t elem_size, char* msg, unsigned int ra);
void* mspace_memalign_d(mspace msp, size_t alignment, size_t bytes, char* msg, unsigned int ra);
void* mspace_get_next_chunk(mspace msp, void* chunk);
void* mspace_get_top_chunk(mspace msp);
size_t mspace_get_chunk_size(void* chunk);
int mspace_chunk_inuse(void* chunk);
int mspace_chunk_is_small(size_t size);
void* mspace_chunk2mem(void* chunk);
void* mspace_mem2chunk(void* chunk);
void* mspace_get_rest_heap(mspace msp);
char* mspace_get_chunk_comment(void* chunk);
unsigned int mspace_get_chunk_ra(void* chunk);
int mspace_check_inuse_mem(mspace msp, void* mem);
size_t mspace_get_rest_heap_size(mspace msp);
size_t mspace_get_chunk_size_from_mem(void* mem);

#ifdef __cplusplus
}
#endif

#endif /* SYSTEM_DLMALLOC_H */
