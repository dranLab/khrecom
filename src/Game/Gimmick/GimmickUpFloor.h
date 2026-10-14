#ifndef GAME_GIMMICK_GIMMICKUPFLOOR_H
#define GAME_GIMMICK_GIMMICKUPFLOOR_H

#include "khtypes.h"
#include "Game/Gimmick/BaseGimmick.h"

class CGimmickUpFloorSwitch : public CBaseGimmick {
private:
    CHAR m_Step; /* 0x220 */
    CHAR m_Stat; /* 0x221 */
    SHORT m_ID;  /* 0x222 */

public:
    CGimmickUpFloorSwitch(CTask* inParent, INT in_GimID);
    virtual ~CGimmickUpFloorSwitch();
    virtual BOOL initalize(INT inGroupID, INT inRsrcID);
    virtual void AfterInit();
    virtual void beginStateMain();
    virtual void stateMain();
    virtual void endStateMain();
    virtual void beginStateHitAttack();
    virtual void stateHitAttack();
    virtual void endStateHitAttack();
};

class CGimmickUpFloor : public CBaseGimmick {
private:
    FLOAT m_DefaultY; /* 0x220 */
    FLOAT m_targetY;  /* 0x224 */
    FLOAT m_Wait;     /* 0x228 */
    CHAR m_Step;      /* 0x22c */
    CHAR m_Stat;      /* 0x22d */
    SHORT m_ID;       /* 0x22e */

public:
    CGimmickUpFloor(CTask* inParent, INT in_GimID);
    virtual ~CGimmickUpFloor();
    virtual BOOL initalize(INT inGroupID, INT inRsrcID);
    virtual void AfterInit();
    virtual void beginStateMain();
    virtual void stateMain();
    virtual void endStateMain();
    void changeState(INT in_Stat);

private:
    BOOL upFloor();
    BOOL downFloor();
};

#endif /* GAME_GIMMICK_GIMMICKUPFLOOR_H */
