// PARTIAL: added for System/Movie/MSStream
#ifndef SYSTEM_RSRCDATA_H
#define SYSTEM_RSRCDATA_H

#include "khtypes.h"
#include "System/HeapMan.h"

/* Placeholder for the original header's inline allocation: its string ("System/RsrcData.h(156)") is in .rodata
 * of every includer. */
#line 1 "System/RsrcData.h"
inline void* _rsrcDataAlloc(size_t n) {
    return com_malloc_d(n, __FILE__ "(156)", (UINT)-1);
}

#endif /* SYSTEM_RSRCDATA_H */
