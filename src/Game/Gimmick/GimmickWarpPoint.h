#ifndef GAME_GIMMICK_GIMMICKWARPPOINT_H
#define GAME_GIMMICK_GIMMICKWARPPOINT_H

#include "khtypes.h"
#include "Game/Gimmick/BaseGimmick.h"

class CGimmickWarpPoint : public CBaseGimmick {
private:
    CHAR m_Step;    /* 0x220 */
    BYTE m_reqFlag; /* 0x221 */

public:
    CGimmickWarpPoint(CTask* inParent);
    virtual ~CGimmickWarpPoint();
    virtual BOOL initalize(INT inGroupID, INT inRsrcID);
    virtual INT getTypeID() const;
    virtual void beginStateMain();
    virtual void stateMain();
    virtual void endStateMain();
    virtual void AfterInit();
};

#endif /* GAME_GIMMICK_GIMMICKWARPPOINT_H */
