#ifndef GAME_GIMMICK_GIMMICKDECEIVE_H
#define GAME_GIMMICK_GIMMICKDECEIVE_H

#include "khtypes.h"
#include "Game/Gimmick/BaseGimmick.h"

class CGimmickDeceive : public CBaseGimmick {
private:
    BOOL m_EnemyFlag;    /* 0x220 */
    CHAR m_Step;         /* 0x224 */
    CHAR m_Stat;         /* 0x225 */
    FLOAT m_WaitCounter; /* 0x228 */

public:
    CGimmickDeceive(CTask* inParent);
    virtual ~CGimmickDeceive();
    virtual BOOL initalize(INT inGroupID, INT inRsrcID);
    virtual void AfterInit();
    virtual void beginStateMain();
    virtual void stateMain();
    virtual void endStateMain();
    virtual void beginStateHitAttack();
    virtual void stateHitAttack();
    virtual void endStateHitAttack();
};

#endif /* GAME_GIMMICK_GIMMICKDECEIVE_H */
