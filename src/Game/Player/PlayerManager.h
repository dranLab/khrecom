// PARTIAL: added for Game/Player/SummonMushu
#ifndef GAME_PLAYER_PLAYERMANAGER_H
#define GAME_PLAYER_PLAYERMANAGER_H

#include "khtypes.h"
#include "System/Vector.h"

#include "System/Task.h" // PARTIAL: added for Game/Player/PlayerWeapon

class CPlayer;
namespace Friend {
class CFriend;
}

namespace PLM {
INT getWorldRotY(FLOAT& outRotY, const FMATRIX& inMat);

// PARTIAL: added for Game/Player/PlayerWeapon
extern UINT g_uiPlayerRcid[7];

class CPlayerManager : public CTask {
private:
    static CPlayerManager* m_pInstance;
    UBYTE _m_0x40[0xc];         /* 0x040 m_pCB, m_pPlayer, m_pPlayerEx */
    Friend::CFriend* m_pFriend; /* 0x04c */

public:
    CPlayerManager();
    CPlayerManager(CTask* pParent); // PARTIAL: added for Game/MinigameManager
    static CPlayerManager* getPLM() { return m_pInstance; }
    Friend::CFriend* getFriend() const { return m_pFriend; }
};
}

#endif /* GAME_PLAYER_PLAYERMANAGER_H */
