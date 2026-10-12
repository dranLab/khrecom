// PARTIAL: added for Game/Gimmick/GimmickWarpPoint
#ifndef FORM_MENU_WORLDWARP_H
#define FORM_MENU_WORLDWARP_H

#include "khtypes.h"

class CTask;

class CWorldWarp {
    UBYTE _m_0x0[0x1f0] __attribute__((aligned(16))); /* CMenu base .. m_vPosLightOld */

public:
    CWorldWarp(CTask* p, BOOL bWarpOrCard);
};

#endif /* FORM_MENU_WORLDWARP_H */
