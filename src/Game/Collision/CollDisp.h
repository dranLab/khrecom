// PARTIAL: added for Game/Debug/KisEnemy
#ifndef GAME_COLLISION_COLLDISP_H
#define GAME_COLLISION_COLLDISP_H

#include "khtypes.h"
#include "System/MainApp.h"
#include "System/HeapMan.h"

/* Placeholders for CCollDisp's inline singleton constructor/creation: their strings
 * ("Game/Collision/CollDisp.h", "__null == m_pInstance", "Game/Collision/CollDisp.h(49)", "m_pInstance")
 * are in .rodata of every includer. Only line 49 is known (it is part of the allocation message). */
/* PARTIAL: path override added for Game/GameSubField ("c:/KHCOM/MainApp/Game/Collision/CollDisp.h") */
#ifndef COLLDISP_H_PATH
#define COLLDISP_H_PATH "Game/Collision/CollDisp.h"
#endif
#line 1 COLLDISP_H_PATH
inline void _collDispCheckInstance(void* inInstance) {
    if (!(__null == inInstance)) AssertApp(__FILE__, __LINE__, "__null == m_pInstance", 0);
}
inline void* _collDispCreate() {
    void* p = com_malloc_d(4, __FILE__ "(49)", (UINT)-1);
    if (!p) AssertApp(__FILE__, __LINE__, "m_pInstance", 0);
    return p;
}

// PARTIAL: added for Game/Bullet/Bullet
class CTask;
class CCollDisp {
private:
    UBYTE _pad[0x60]; /* PARTIAL: the original derives from CTask (size 0x60) */
public:
    // PARTIAL: added for Game/GameSubField (create/destroy are inline in the original)
    CCollDisp(CTask* pParent, INT nDisp, INT nMapZoneDisp);
    static CCollDisp* m_pInstance;
    static BOOL m_fgDisp;
    // PARTIAL: added for Form/DebugWindowTBL
    static UINT m_fgMapZoneDisp;
    static SHORT m_bMapZoneColR;
    static SHORT m_bMapZoneColG;
    static SHORT m_bMapZoneColB;
};

#endif /* GAME_COLLISION_COLLDISP_H */
