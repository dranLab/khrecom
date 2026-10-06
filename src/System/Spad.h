#ifndef SYSTEM_SPAD_H
#define SYSTEM_SPAD_H

#include "khtypes.h"

/* Scratchpad RAM (0x70000000, 16 KB) stack allocator. */
namespace SPAD {
void* allocSpad(UINT nInSize);
void freeSpad(UINT nInSize);
void freeAllSpad();
}

#endif /* SYSTEM_SPAD_H */
