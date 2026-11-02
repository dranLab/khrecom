#ifndef GAME_GIMMICK_GIMMICKDECEIVEENEMY_H
#define GAME_GIMMICK_GIMMICKDECEIVEENEMY_H

#include "khtypes.h"
#include "Game/Gimmick/BaseGimmick.h"

class CGimmickDeceiveEnemy : public CBaseGimmick {
private:
    CHAR m_Step;         /* 0x220 */
    SHORT m_GimID;       /* 0x222 */
    INT m_DmgKind;       /* 0x224 */
    FLOAT m_WaitCounter; /* 0x228 */

public:
    CGimmickDeceiveEnemy(CTask* inParent, INT inGimmickID);
    virtual ~CGimmickDeceiveEnemy();
    virtual BOOL initalize(INT inGroupID, INT inRsrcID);
    void setDmgKind(INT in_DmgKind) { m_DmgKind = in_DmgKind; }
    virtual void beginStateMain();
    virtual void stateMain();
    virtual void endStateMain();
    void BarrelSpider();
    void TresureEnemy();
};

#endif /* GAME_GIMMICK_GIMMICKDECEIVEENEMY_H */
