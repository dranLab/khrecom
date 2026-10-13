#ifndef GAME_GIMMICK_GIMMICKHITREPETITION_H
#define GAME_GIMMICK_GIMMICKHITREPETITION_H

#include "khtypes.h"
#include "Game/Gimmick/BaseGimmick.h"

class CGimmickHitRepetition : public CBaseGimmick {
private:
    CHAR m_Step;               /* 0x220 */
    CHAR m_Stat;               /* 0x221 */
    SHORT m_ID;                /* 0x222 */
    BYTE m_RepeatFlag;         /* 0x224 */
    BYTE m_ReloadFlag;         /* 0x225 */
    FLOAT m_ReloadWaitCounter; /* 0x228 */

public:
    CGimmickHitRepetition(CTask* inParent, INT in_GimID);
    virtual ~CGimmickHitRepetition();
    virtual BOOL initalize(INT inGroupID, INT inRsrcID);
    virtual void AfterInit();
    void putPrize();
    virtual void beginStateMain();
    virtual void stateMain();
    virtual void endStateMain();
    virtual void beginStateHitAttack();
    virtual void stateHitAttack();
    virtual void endStateHitAttack();
};

#endif /* GAME_GIMMICK_GIMMICKHITREPETITION_H */
