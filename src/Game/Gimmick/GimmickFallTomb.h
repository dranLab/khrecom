#ifndef GAME_GIMMICK_GIMMICKFALLTOMB_H
#define GAME_GIMMICK_GIMMICKFALLTOMB_H

#include "khtypes.h"
#include "Game/Gimmick/BaseGimmick.h"

class CGimmickFallTomb : public CBaseGimmick {
private:
    CHAR m_Step;       /* 0x220 */
    CHAR m_Stat;       /* 0x221 */
    CHAR m_HP;         /* 0x222 */
    FLOAT m_Distance;  /* 0x224 */
    FLOAT m_DistRot;   /* 0x228 */

public:
    CGimmickFallTomb(CTask* inParent);
    virtual ~CGimmickFallTomb();
    virtual BOOL initalize(INT inGroupID, INT inRsrcID);
    virtual void AfterInit();
    virtual void beginStateMain();
    virtual void stateMain();
    virtual void endStateMain();
    virtual void beginStateHitAttack();
    virtual void stateHitAttack();
    virtual void endStateHitAttack();
};

#endif /* GAME_GIMMICK_GIMMICKFALLTOMB_H */
