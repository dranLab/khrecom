#ifndef GAME_GIMMICK_GIMMICKWALLHOLE_H
#define GAME_GIMMICK_GIMMICKWALLHOLE_H

#include "khtypes.h"
#include "Game/Gimmick/BaseGimmick.h"

class CGimmickWallHole : public CBaseGimmick {
private:
    CHAR m_Step;           /* 0x220 */
    CHAR m_Stat;           /* 0x221 */
    FLOAT m_Wait;          /* 0x224 */
    FLOAT m_AtkUpDateWait; /* 0x228 */

public:
    CGimmickWallHole(CTask* inParent);
    virtual ~CGimmickWallHole();
    virtual BOOL initalize(INT inGroupID, INT inRsrcID);
    virtual void AfterInit();
    virtual void beginStateMain();
    virtual void stateMain();
    virtual void endStateMain();
    void HoleMain();
    void WhipMain();
    virtual void beginStateHitAttack();
    virtual void stateHitAttack();
    virtual void endStateHitAttack();
};

#endif /* GAME_GIMMICK_GIMMICKWALLHOLE_H */
