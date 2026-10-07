#ifndef SYSTEM_GRAPHIC_EFFECT_LIBPPP_PPPALLOC_H
#define SYSTEM_GRAPHIC_EFFECT_LIBPPP_PPPALLOC_H

#include "System/Graphic/Effect/libppp/pppPart.h"

void pppDumpHeap(pppMemMng *mng);
void pppCreateHeap(pppMemMng *mng, long128 *addr, u_word size);
void *pppAlloc(pppMemMng *mng, u_word size);
void pppFree(pppMemMng *mng, void *ptr);

#endif /* SYSTEM_GRAPHIC_EFFECT_LIBPPP_PPPALLOC_H */
