#ifndef GAME_GIMMICK_GIMMICKPLANTCHILD_H
#define GAME_GIMMICK_GIMMICKPLANTCHILD_H

#include "khtypes.h"
#include "Game/Gimmick/BaseGimmick.h"

class CGimmickPlantChild : public CBaseGimmick {
private:
    CHAR m_Step; /* 0x220 */

public:
    CGimmickPlantChild(CTask* inParent);
    virtual ~CGimmickPlantChild();
    virtual BOOL initalize(INT inGroupID, INT inRsrcID);
    virtual void AfterInit();
    virtual void beginStateMain();
    virtual void stateMain();
    virtual void endStateMain();
    void killPlant();
};

#endif /* GAME_GIMMICK_GIMMICKPLANTCHILD_H */
