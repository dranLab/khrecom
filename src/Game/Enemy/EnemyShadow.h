#ifndef GAME_ENEMY_ENEMYSHADOW_H
#define GAME_ENEMY_ENEMYSHADOW_H

#include "Game/Enemy/Heartless.h"

class CEnemyShadow : public CHeartless {
private:
    INT m_iDigStep;     /* 0x880 */
    TIMEQ m_tqDigTimer; /* 0x884 */
    TIMEQ m_tqDigTime;  /* 0x888 */
    HANDLE m_hDeadDemo; /* 0x88c */

public:
    CEnemyShadow(CTask* inParent);
    virtual ~CEnemyShadow();
    virtual BOOL initalize(INT inGroupID, INT inRsrcID);
protected:
    virtual void updateAfter();
    virtual void beginStateAppear();
    virtual void stateAppear();
    virtual void endStateAppear();
    virtual void beginStateMain();
    virtual void stateMain();
    virtual void endStateMain();
    virtual void beginStateAttack1();
    virtual void stateAttack1();
    virtual void beginStateAttack2();
    virtual void stateAttack2();
    virtual void beginStateDead();
    virtual void stateDead();
    virtual void endStateDead();
private:
    void setupDig_(INT inStep, TIMEQ inTime) {
        m_tqDigTimer = inTime;
        m_tqDigTime = inTime;
        m_iDigStep = inStep;
    }
};

#endif /* GAME_ENEMY_ENEMYSHADOW_H */
