#ifndef GAME_GIMMICK_GIMMICKSTEAMTRAP_H
#define GAME_GIMMICK_GIMMICKSTEAMTRAP_H

#include "khtypes.h"
#include "Game/Gimmick/BaseGimmick.h"

class CGimmickSteamTrap : public CBaseGimmick {
private:
    CHAR m_Step;       /* 0x220 */
    CHAR m_Stat;       /* 0x221 */
    BYTE m_RepeatFlag; /* 0x222 */
    FLOAT m_Wait;      /* 0x224 */
    FLOAT m_MotWait;   /* 0x228 */
    FLOAT m_SteamWait; /* 0x22c */
    FLOAT m_AtkWait;   /* 0x230 */

public:
    CGimmickSteamTrap(CTask* inParent);
    virtual void AfterInit();
    virtual ~CGimmickSteamTrap();
    virtual BOOL initalize(INT inGroupID, INT inRsrcID);
    virtual void beginStateMain();
    virtual void stateMain();
    virtual void endStateMain();
    virtual void beginStateHitAttack();
    virtual void stateHitAttack();
    virtual void endStateHitAttack();
};

#endif /* GAME_GIMMICK_GIMMICKSTEAMTRAP_H */
