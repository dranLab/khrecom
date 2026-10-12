#ifndef GAME_GIMMICK_GIMMICKTOLDPIPE_H
#define GAME_GIMMICK_GIMMICKTOLDPIPE_H

#include "khtypes.h"
#include "Game/Gimmick/BaseGimmick.h"

class CGimmickToldPipe : public CBaseGimmick {
private:
    CHAR m_Step;                /* 0x220 */
    CHAR m_Stat;                /* 0x221 */
    CGimmickToldPipe* m_pChild; /* 0x224 */

public:
    CGimmickToldPipe(CTask* inParent);
    virtual ~CGimmickToldPipe();
    virtual BOOL initalize(INT inGroupID, INT inRsrcID);
    virtual void beginStateMain();
    virtual void stateMain();
    virtual void endStateMain();
    virtual void AfterInit();
    virtual void beginStateHitAttack();
    virtual void stateHitAttack();
    virtual void endStateHitAttack();
    void reaction();
};

#endif /* GAME_GIMMICK_GIMMICKTOLDPIPE_H */
