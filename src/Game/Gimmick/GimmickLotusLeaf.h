#ifndef GAME_GIMMICK_GIMMICKLOTUSLEAF_H
#define GAME_GIMMICK_GIMMICKLOTUSLEAF_H

#include "khtypes.h"
#include "Game/Gimmick/BaseGimmick.h"

class CGimmickKLotusLeaf : public CBaseGimmick {
private:
    CHAR m_Step; /* 0x220 */
    CHAR m_Stat; /* 0x221 */

public:
    CGimmickKLotusLeaf(CTask* inParent);
    virtual ~CGimmickKLotusLeaf();
    virtual void AfterInit();
    virtual BOOL initalize(INT inGroupID, INT inRsrcID);
    virtual void beginStateMain();
    virtual void stateMain();
    virtual void endStateMain();
    virtual void beginStateHitAttack();
    virtual void stateHitAttack();
    virtual void endStateHitAttack();
    virtual void getLockOnPos(FVECTOR& outPos);
};

#endif /* GAME_GIMMICK_GIMMICKLOTUSLEAF_H */
