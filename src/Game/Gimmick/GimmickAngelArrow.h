#ifndef GAME_GIMMICK_GIMMICKANGELARROW_H
#define GAME_GIMMICK_GIMMICKANGELARROW_H

#include "khtypes.h"
#include "Game/Gimmick/BaseGimmick.h"

class CGimmickAngelArrow : public CBaseGimmick {
private:
    CHAR m_Stat;          /* 0x220 */
    CHAR m_Step;          /* 0x221 */
    FLOAT m_addRot;       /* 0x224 */
    FLOAT m_WaitCounter;  /* 0x228 */

public:
    CGimmickAngelArrow(CTask* inParent);
    virtual ~CGimmickAngelArrow();
    virtual BOOL initalize(INT inGroupID, INT inRsrcID);
    virtual void AfterInit();
    virtual void beginStateMain();
    virtual void stateMain();
    virtual void endStateMain();
    BOOL loockAtPlayer();
    virtual void lastUpdate();
    virtual void beginStateHitAttack();
    virtual void stateHitAttack();
    virtual void endStateHitAttack();
};

#endif /* GAME_GIMMICK_GIMMICKANGELARROW_H */
