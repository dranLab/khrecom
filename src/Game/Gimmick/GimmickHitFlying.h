#ifndef GAME_GIMMICK_GIMMICKHITFLYING_H
#define GAME_GIMMICK_GIMMICKHITFLYING_H

#include "khtypes.h"
#include "System/Vector.h"
#include "Game/Gimmick/BaseGimmick.h"

class CGimmickHitFlying : public CBaseGimmick {
private:
    CHAR m_Stat;         /* 0x220 */
    FLOAT m_WaitCounter; /* 0x224 */
    FVECTOR m_FlyDir;    /* 0x230 */

public:
    CGimmickHitFlying(CTask* inParent);
    virtual ~CGimmickHitFlying();
    virtual BOOL initalize(INT inGroupID, INT inRsrcID);
    virtual void AfterInit();
    virtual void beginStateMain();
    virtual void stateMain();
    virtual void endStateMain();
    virtual void beginStateHitAttack();
    virtual void stateHitAttack();
    virtual void endStateHitAttack();
    void FlyGimmick();
    BOOL moveFly();
};

#endif /* GAME_GIMMICK_GIMMICKHITFLYING_H */
