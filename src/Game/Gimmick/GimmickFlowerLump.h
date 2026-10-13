#ifndef GAME_GIMMICK_GIMMICKFLOWERLUMP_H
#define GAME_GIMMICK_GIMMICKFLOWERLUMP_H

#include "khtypes.h"
#include "Game/Gimmick/BaseGimmick.h"

class CGimmickFlowerLump : public CBaseGimmick {
private:
    CHAR m_Step;         /* 0x220 */
    CHAR m_Stat;         /* 0x221 */
    FLOAT m_WaitCounter; /* 0x224 */

public:
    CGimmickFlowerLump(CTask* inParent);
    virtual ~CGimmickFlowerLump();
    virtual BOOL initalize(INT inGroupID, INT inRsrcID);
    virtual void AfterInit();
    virtual void beginStateMain();
    virtual void stateMain();
    virtual void endStateMain();
    virtual void beginStateHitAttack();
    virtual void stateHitAttack();
    virtual void endStateHitAttack();
};

#endif /* GAME_GIMMICK_GIMMICKFLOWERLUMP_H */
