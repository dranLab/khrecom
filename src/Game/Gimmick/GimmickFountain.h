#ifndef GAME_GIMMICK_GIMMICKFOUNTAIN_H
#define GAME_GIMMICK_GIMMICKFOUNTAIN_H

#include "khtypes.h"
#include "Game/Gimmick/BaseGimmick.h"

class CGimmickFountain : public CBaseGimmick {
private:
    CHAR m_Step;          /* 0x220 */
    CHAR m_Stat;          /* 0x221 */
    FLOAT m_addRot;       /* 0x224 */
    FLOAT m_WaitCounter;  /* 0x228 */

public:
    CGimmickFountain(CTask* inParent);
    virtual ~CGimmickFountain();
    virtual BOOL initalize(INT inGroupID, INT inRsrcID);
    virtual void AfterInit();
    virtual void beginStateMain();
    virtual void stateMain();
    virtual void endStateMain();
    BOOL loockAtPlayer();
    virtual void lastUpdate();
};

#endif /* GAME_GIMMICK_GIMMICKFOUNTAIN_H */
