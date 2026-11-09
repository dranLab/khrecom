
#ifndef SYSTEM_GRAPHIC_MAPUNIT_H
#define SYSTEM_GRAPHIC_MAPUNIT_H

#include "khtypes.h"
#include "System/HeapMan.h"
#include "Game/MapUnitTable.h"

class CTask;
struct ZONEPARTS;



/* Creates a map unit object of class T (the function pointer in MAPUNITTABLE). */
#ifndef MAPUNIT_H_PATH // MATCH: path override for System/Graphic/MapUnit
#define MAPUNIT_H_PATH "System/Graphic/MapUnit.h"
#endif
#line 75 MAPUNIT_H_PATH
template <class T> CTask* NewClass(CTask* map, const ZONEPARTS& parts) {




    return new (__FILE__ "(80)") T(map, parts);


}

#endif /* SYSTEM_GRAPHIC_MAPUNIT_H */
