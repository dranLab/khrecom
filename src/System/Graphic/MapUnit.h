// PARTIAL: added for Game/Gimmick/SampleGimmick
#ifndef SYSTEM_GRAPHIC_MAPUNIT_H
#define SYSTEM_GRAPHIC_MAPUNIT_H

#include "khtypes.h"
#include "System/HeapMan.h"
#include "Game/MapUnitTable.h"

class CTask;
struct ZONEPARTS;

inline void* operator new(size_t, void* p) { return p; }

/* Creates a map unit object of class T (the function pointer in MAPUNITTABLE). */
#line 75 "System/Graphic/MapUnit.h"
template <class T> CTask* NewClass(CTask* map, const ZONEPARTS& parts) {
    CTask* p;
    char* msg = __FILE__ "(80)";
    UINT nRA;
    __asm__ volatile("daddu %0, $31, $0" : "=r"(nRA));
    p = (CTask*)com_malloc_d(sizeof(T), msg, nRA);
    if (p != NULL) p = new (p) T(map, parts);
    return p;
}

#endif /* SYSTEM_GRAPHIC_MAPUNIT_H */
