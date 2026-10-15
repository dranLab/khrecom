// PARTIAL: added for Game/Player/SummonMushu
#ifndef GAME_PLAYER_PLAYERMANAGER_H
#define GAME_PLAYER_PLAYERMANAGER_H

#include "khtypes.h"
#include "System/Vector.h"

#include "System/Task.h" // PARTIAL: added for Game/Player/PlayerWeapon

class CPlayer;
namespace Summon {
class CSummon; // PARTIAL: added for Game/Player/Summon
}
namespace Friend {
class CFriend;
}

namespace PLM {
INT getWorldRotY(FLOAT& outRotY, const FMATRIX& inMat);

// PARTIAL: added for Game/Player/PlayerWeapon
extern UINT g_uiPlayerRcid[7];
extern UINT g_uiSummonRcid[8]; // PARTIAL: added for Game/Player/Summon
extern UINT g_uiFriendRcid[14]; // PARTIAL: added for Game/Player/Friend

class CPlayerManager : public CTask {
private:
    static CPlayerManager* m_pInstance;
    UBYTE _m_0x40[0x4];         /* 0x040 m_pCB */
    CPlayer* m_pPlayer;         /* 0x044 */ // PARTIAL: split out for Game/Player/Summon
    UBYTE _m_0x48[0x4];         /* 0x048 m_pPlayerEx */
    Friend::CFriend* m_pFriend; /* 0x04c */
    UBYTE _m_0x50[0x8];         /* 0x050 m_pFriendEx, m_pSummon */ // PARTIAL: added for Game/Player/Friend
    UINT m_uiFlag;              /* 0x058 */

public:
    CPlayerManager();
    CPlayerManager(CTask* pParent); // PARTIAL: added for Game/MinigameManager
    static CPlayerManager*& getPLM() { return m_pInstance; }
    Friend::CFriend* getFriend() const { return m_pFriend; }
    // PARTIAL: added for Game/Player/Summon
    CPlayer* getPlayer() const { return m_pPlayer; }
    void notifySummon(Summon::CSummon* inSummon, INT inFlag);
    // PARTIAL: added for Game/Player/Friend
    void notifyFriend(Friend::CFriend* inFriend, INT inFlag, INT inEx);
    UINT getFlag() const { return m_uiFlag; }
};
}

#endif /* GAME_PLAYER_PLAYERMANAGER_H */
