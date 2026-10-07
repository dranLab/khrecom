#ifndef GAME_GIMMICK_GIMMICKHITMORE_H
#define GAME_GIMMICK_GIMMICKHITMORE_H

#include "khtypes.h"
#include "Game/Gimmick/BaseGimmick.h"

class CGimmickHitMore : public CBaseGimmick {
private:
    CHAR m_Step;     /* 0x220 */
    CHAR m_Stat;     /* 0x221 */
    BYTE m_HitPoint; /* 0x222 */

public:
    CGimmickHitMore(CTask* inParent);
    virtual ~CGimmickHitMore();
    virtual BOOL initalize(INT inGroupID, INT inRsrcID);
    virtual void AfterInit();
    virtual void beginStateMain();
    virtual void stateMain();
    virtual void endStateMain();
    virtual void beginStateHitAttack();
    virtual void stateHitAttack();
    virtual void endStateHitAttack();
};

#endif /* GAME_GIMMICK_GIMMICKHITMORE_H */
