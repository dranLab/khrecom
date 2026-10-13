#ifndef GAME_GIMMICK_GIMMICKRIDEDEST_H
#define GAME_GIMMICK_GIMMICKRIDEDEST_H

#include "khtypes.h"
#include "Game/Gimmick/BaseGimmick.h"

class CGimmickRideDest : public CBaseGimmick {
private:
    CHAR m_Step;     /* 0x220 */
    CHAR m_Stat;     /* 0x221 */
    CHAR m_BodyFlag; /* 0x222 */

public:
    CGimmickRideDest(CTask* inParent);
    virtual ~CGimmickRideDest();
    virtual BOOL initalize(INT inGroupID, INT inRsrcID);
    virtual void AfterInit();
    virtual void beginStateMain();
    virtual void stateMain();
    virtual void endStateMain();
    virtual void beginStateHitAttack();
    virtual void stateHitAttack();
    virtual void endStateHitAttack();
};

#endif /* GAME_GIMMICK_GIMMICKRIDEDEST_H */
