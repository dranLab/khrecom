#ifndef GAME_GIMMICK_GIMMICKPLANTPARENT_H
#define GAME_GIMMICK_GIMMICKPLANTPARENT_H

#include "khtypes.h"
#include "Game/Gimmick/BaseGimmick.h"

class CGimmickPlantParent : public CBaseGimmick {
private:
    CHAR m_Step;     /* 0x220 */
    CHAR m_Stat;     /* 0x221 */
    int m_hChild[4]; /* 0x224 */

public:
    CGimmickPlantParent(CTask* inParent);
    virtual ~CGimmickPlantParent();
    virtual BOOL initalize(INT inGroupID, INT inRsrcID);
    virtual void AfterInit();
    virtual void beginStateMain();
    virtual void stateMain();
    virtual void endStateMain();
    virtual void beginStateHitAttack();
    virtual void stateHitAttack();
    virtual void endStateHitAttack();
};

#endif /* GAME_GIMMICK_GIMMICKPLANTPARENT_H */
