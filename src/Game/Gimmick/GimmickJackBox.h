#ifndef GAME_GIMMICK_GIMMICKJACKBOX_H
#define GAME_GIMMICK_GIMMICKJACKBOX_H

#include "khtypes.h"
#include "Game/Gimmick/BaseGimmick.h"

class CGimmickJackBox : public CBaseGimmick {
private:
    CHAR m_Step; /* 0x220 */
    CHAR m_Stat; /* 0x221 */

public:
    CGimmickJackBox(CTask* inParent);
    virtual void AfterInit();
    virtual ~CGimmickJackBox();
    virtual BOOL initalize(INT inGroupID, INT inRsrcID);
    virtual void beginStateMain();
    virtual void stateMain();
    virtual void endStateMain();
    virtual void beginStateHitAttack();
    virtual void stateHitAttack();
    virtual void endStateHitAttack();
};

#endif /* GAME_GIMMICK_GIMMICKJACKBOX_H */
