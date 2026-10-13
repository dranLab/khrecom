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

struct BCAM_PARAM {
    PCAM_DATA Normal;
    PCAM_DATA Lockon;
};

namespace PlayerCamera {

class CPlayerCamera {
private:
    UBYTE _m_0x000[0x110]; /* 0x000 .. 0x10c m_iModeFieldFar */ // PARTIAL: added for Game/Player/SummonDumbo
    UINT m_uiFlag;         /* 0x110 */
public:
    // PARTIAL: added for Game/Player/SummonDumbo
    void setFlag(UINT inFlag, BOOL inOn) {
        if (inOn) m_uiFlag |= inFlag;
        else m_uiFlag &= ~inFlag;
    }
    void changeNowPlayer(HANDLE inPlayer, HANDLE inTarget);
    void changeCamMode(INT inMode, INT inReset);
};

} // namespace PlayerCamera

#endif /* GAME_PLAYER_PLAYERCAMERA_H */
