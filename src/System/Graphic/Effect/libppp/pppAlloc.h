#ifndef SYSTEM_GRAPHIC_EFFECT_LIBPPP_PPPALLOC_H
#define SYSTEM_GRAPHIC_EFFECT_LIBPPP_PPPALLOC_H

#include "System/Graphic/Effect/libppp/partMng.h"

typedef struct _pppEMCB { /* 0x10 */
    struct _pppEMCB *next;
    struct _pppEMCB *before;
    int owner;
    int wdummy;
} pppEMCB;

void pppDumpHeap(pppMemMng *mng);
void pppCreateHeap(pppMemMng *mng, long128 *addr, u_word size);
void *pppAlloc(pppMemMng *mng, u_word size);
void pppFree(pppMemMng *mng, void *ptr);

#endif /* SYSTEM_GRAPHIC_EFFECT_LIBPPP_PPPALLOC_H */
