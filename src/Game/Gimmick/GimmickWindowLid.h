#ifndef GAME_GIMMICK_GIMMICKWINDOWLID_H
#define GAME_GIMMICK_GIMMICKWINDOWLID_H

#include "khtypes.h"
#include "Game/Gimmick/BaseGimmick.h"

class CGimmickWindowLid : public CBaseGimmick {
private:
    CHAR m_Step;          /* 0x220 */
    FLOAT m_WaitCounter;  /* 0x224 */

public:
    CGimmickWindowLid(CTask* inParent);
    virtual ~CGimmickWindowLid();
    virtual void AfterInit();
    virtual BOOL initalize(INT inGroupID, INT inRsrcID);
    virtual void beginStateMain();
    virtual void stateMain();
    virtual void endStateMain();
};

#endif /* GAME_GIMMICK_GIMMICKWINDOWLID_H */
