#ifndef GAME_GIMMICK_GIMMICKBELL_H
#define GAME_GIMMICK_GIMMICKBELL_H

#include "khtypes.h"
#include "Game/Gimmick/BaseGimmick.h"

class CGimmickBell : public CBaseGimmick {
private:
    CHAR m_Step; /* 0x220 */
    CHAR m_Stat; /* 0x221 */

public:
    CGimmickBell(CTask* inParent);
    virtual ~CGimmickBell();
    virtual void AfterInit();
    virtual BOOL initalize(INT inGroupID, INT inRsrcID);
    virtual void beginStateMain();
    virtual void stateMain();
    virtual void endStateMain();
    virtual void beginStateHitAttack();
    virtual void stateHitAttack();
    virtual void endStateHitAttack();
};

#endif /* GAME_GIMMICK_GIMMICKBELL_H */
