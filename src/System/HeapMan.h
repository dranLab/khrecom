// PARTIAL: added for Game/CardKind
#ifndef SYSTEM_HEAPMAN_H
#define SYSTEM_HEAPMAN_H

#include "khtypes.h"
#include <string.h>

extern "C" void* com_malloc_d(size_t bytes, char* msg, unsigned int ra);

// PARTIAL: added for Form/Menu/MoogleShop
extern "C" void com_free(void* mem);

inline void operator delete(void* mem) { com_free(mem); }

#endif /* SYSTEM_HEAPMAN_H */
