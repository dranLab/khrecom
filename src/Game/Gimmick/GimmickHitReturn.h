#ifndef GAME_GIMMICK_GIMMICKHITRETURN_H
#define GAME_GIMMICK_GIMMICKHITRETURN_H

#include "khtypes.h"
#include "Game/Gimmick/BaseGimmick.h"

class CGimmickHitReturn : public CBaseGimmick {
private:
    CHAR m_Step;       /* 0x220 */
    CHAR m_Stat;       /* 0x221 */
    CHAR m_RideFlag;   /* 0x222 */
    SHORT m_ID;        /* 0x224 */
    FLOAT m_Wait;      /* 0x228 */
    BYTE m_RepeatFlag; /* 0x22c */

public:
    CGimmickHitReturn(CTask* inParent, INT in_GimID);
    virtual ~CGimmickHitReturn();
    virtual BOOL initalize(INT inGroupID, INT inRsrcID);
    virtual void AfterInit();
    virtual void beginStateMain();
    virtual void stateMain();
    virtual void endStateMain();
    virtual void beginStateHitAttack();
    virtual void stateHitAttack();
    virtual void endStateHitAttack();
};

#endif /* GAME_GIMMICK_GIMMICKHITRETURN_H */
