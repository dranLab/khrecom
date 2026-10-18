#ifndef GAME_ENEMY_ENEMYNEOSHADOW_H
#define GAME_ENEMY_ENEMYNEOSHADOW_H

#include "Game/Enemy/Heartless.h"

class CEnemyNeoshadow : public CHeartless {
private:
    INT m_iDigStep;    /* 0x880 */
    TIMEQ m_tqDigTime; /* 0x884 */

public:
    CEnemyNeoshadow(CTask* inParent);
    virtual ~CEnemyNeoshadow();
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
    virtual void endStateAttack1();
    virtual void beginStateAttack2();
    virtual void stateAttack2();
    virtual void endStateAttack2();
private:
    void setupDig_(INT inStep, TIMEQ inTime) {
        setTimer(inTime);
        m_tqDigTime = inTime;
        m_iDigStep = inStep;
    }
};

#endif /* GAME_ENEMY_ENEMYNEOSHADOW_H */
