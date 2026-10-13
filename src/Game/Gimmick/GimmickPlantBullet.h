#ifndef GAME_GIMMICK_GIMMICKPLANTBULLET_H
#define GAME_GIMMICK_GIMMICKPLANTBULLET_H

#include "khtypes.h"
#include "Game/Gimmick/BaseGimmick.h"

class CGimmickPlantBullet : public CBaseGimmick {
private:
    CHAR m_Stat;         /* 0x220 */
    CHAR m_Step;         /* 0x221 */
    CHAR m_AtkStep;      /* 0x222 */
    CHAR m_RotStep;      /* 0x223 */
    CHAR m_Dest;         /* 0x224 */
    FLOAT m_addRot;      /* 0x228 */
    FLOAT m_WaitCounter; /* 0x22c */

public:
    CGimmickPlantBullet(CTask* inParent);
    virtual ~CGimmickPlantBullet();
    virtual BOOL initalize(INT inGroupID, INT inRsrcID);
    virtual void AfterInit();
    virtual void beginStateMain();
    virtual void stateMain();
    virtual void endStateMain();
    void loockAtPlayer();
    BOOL AtkPlayer();
    virtual void beginStateHitAttack();
    virtual void stateHitAttack();
    virtual void endStateHitAttack();
};

#endif /* GAME_GIMMICK_GIMMICKPLANTBULLET_H */
