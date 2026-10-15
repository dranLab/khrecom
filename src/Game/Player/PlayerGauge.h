// PARTIAL: added for Game/Player/Summon
#ifndef GAME_PLAYER_PLAYERGAUGE_H
#define GAME_PLAYER_PLAYERGAUGE_H

#include "khtypes.h"
#include "System/Task.h"
#include "System/Vector.h"

namespace Gauge {

class CLockonMarker : public CTask {
private:
    UBYTE _m_0x40[0x20];  /* 0x40 m_pGauge .. 0x50 m_nPreSeqNo */
    FVECTOR m_vPos;             /* 0x60 */
    FVECTOR m_vUseCardPos;      /* 0x70 */
    FVECTOR m_vCameraTargetPos; /* 0x80 */
    FVECTOR m_vDispPos;         /* 0x90 */
    HANDLE m_hTarget;           /* 0xa0 */
    HANDLE m_hLockon;           /* 0xa4 */
    HANDLE m_hHoldTarget;       /* 0xa8 */
    INT m_iDist;                /* 0xac */
    INT m_iMode;                /* 0xb0 */
    BOOL m_bTarget;             /* 0xb4 */
    BOOL m_bLockon;             /* 0xb8 */
    BOOL m_bDisp;               /* 0xbc */
    BOOL m_bFlag;               /* 0xc0 */
    BOOL m_bTrg;                /* 0xc4 */
    UINT m_uiFlag;              /* 0xc8 */
    UINT m_uiKind;              /* 0xcc */
    FLOAT m_fDist;              /* 0xd0 */
    FLOAT m_fScale;             /* 0xd4 */

public:
    CLockonMarker(CTask* inParent);
    void setTarget(HANDLE inTarget);
    void setLockon(HANDLE inTarget, INT inDist);
    void onPush(INT inKind);
    void offPush();
    void holdTarget(INT inFlag);
    void cancelLockon(INT& ioHandle, INT inFlag); // PARTIAL: added for Game/Player/PlayerRiku
    void chkLockon(INT& ioHandle);                // PARTIAL: added for Game/Player/PlayerRiku
    void setFlag(BOOL inFlag) { m_bFlag = inFlag; } // PARTIAL: added for Game/Player/PlayerRiku (original name unknown)
    void changeMode() {
        m_iMode = 1;
        m_bTrg = TRUE;
        m_hLockon = 0;
    }
    // PARTIAL: added for Game/Player/PlayerRiku (original name unknown; stores in this order)
    void setMode(INT inMode) {
        m_iMode = inMode;
        m_hLockon = 0;
        m_bTrg = TRUE;
    }
    void changeMode(INT inMode) {
        m_bTrg = TRUE;
        m_iMode = inMode;
        m_hLockon = 0;
    }
};

} // namespace Gauge

#endif /* GAME_PLAYER_PLAYERGAUGE_H */
