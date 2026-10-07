// PARTIAL: added for Game/Gimmick/GimmickPlantChild
#ifndef SYSTEM_GRAPHIC_MODELDATA_H
#define SYSTEM_GRAPHIC_MODELDATA_H

#include "khtypes.h"
#include "System/MainApp.h"

/* Placeholder for the original header's inline function asserting _pTexInfo: its strings
 * ("System/Graphic/modeldata.h", "_pTexInfo") are in .rodata of every includer. The line number is not known. */
#line 1 "System/Graphic/modeldata.h"
inline void _modelDataCheckTexInfo(void* _pTexInfo) {
    if (!_pTexInfo) AssertApp(__FILE__, __LINE__, "_pTexInfo", 0);
}

#endif /* SYSTEM_GRAPHIC_MODELDATA_H */
