// PARTIAL: added for Game/Player/FriendJack
#ifndef GAME_PLAYER_PLAYERCAMERA_H
#define GAME_PLAYER_PLAYERCAMERA_H

#include "khtypes.h"
#include "System/Vector.h" // PARTIAL: added for Game/Player/FriendKing

// PARTIAL: added for Game/Player/FriendKing
struct PCAM_DATA {
    FLOAT fViewAngle;
    FLOAT fSize;
    FLOAT fAdjust;
    FLOAT fDistance;
    FVECTOR vEye;
    FVECTOR vAim;
};

namespace PlayerCamera {

// PARTIAL: moved into namespace PlayerCamera for Game/Player/SummonMushu (mangled Q212PlayerCamera10BCAM_PARAM)
struct BCAM_PARAM {
    PCAM_DATA Normal;
    PCAM_DATA Lockon;
};

class CPlayerCamera {
private:
    static CPlayerCamera* m_pInstance; // PARTIAL: added for Game/Player/PlayerManager
    UBYTE _m_0x000[0x100]; /* 0x000 .. 0x0fc m_pData */ // PARTIAL: added for Game/Player/SummonDumbo
    INT m_iMode;           /* 0x100 */ // PARTIAL: split out for Game/Player/PlayerManager
    UBYTE _m_0x104[0xc];   /* 0x104 m_iPreMode .. 0x10c m_iModeFieldFar */
    UINT m_uiFlag;         /* 0x110 */
    UBYTE _m_0x114[0x1bc]; /* 0x114 .. 0x2c0 m_vAim */ // PARTIAL: added for Game/Gimmick/GimmickCarry
    FVECTOR m_vEye;        /* 0x2d0 */
public:
    FVECTOR getNowCamEye() { return m_vEye; } // PARTIAL: added for Game/Gimmick/GimmickCarry
    // PARTIAL: added for Game/Player/SummonDumbo
    void setFlag(UINT inFlag, BOOL inOn) {
        if (inOn) m_uiFlag |= inFlag;
        else m_uiFlag &= ~inFlag;
    }
    void changeNowPlayer(HANDLE inPlayer, HANDLE inTarget);
    void changeCamMode(INT inMode, INT inReset);
    // PARTIAL: added for Game/Player/SummonMushu
    static void setBattleCamData(HANDLE inHandle, const BCAM_PARAM* inParam);
    static void getBattleCamData(BCAM_PARAM& outParam);
    static INT getNowCamMode() { return m_pInstance ? m_pInstance->m_iMode : 0; } // PARTIAL: added for Game/Player/PlayerManager
};

// PARTIAL: added for Game/Player/PlayerManager
extern char* szTestCamMode[6];
extern PCAM_DATA* pTestCamData[6];

} // namespace PlayerCamera

using PlayerCamera::BCAM_PARAM;

#endif /* GAME_PLAYER_PLAYERCAMERA_H */
