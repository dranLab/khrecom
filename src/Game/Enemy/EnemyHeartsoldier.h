#ifndef GAME_ENEMY_ENEMYHEARTSOLDIER_H
#define GAME_ENEMY_ENEMYHEARTSOLDIER_H

#include "Game/Enemy/Heartless.h"

class CEnemyHeartsoldier : public CHeartless {
public:
    CEnemyHeartsoldier(CTask* inParent);
    virtual ~CEnemyHeartsoldier();
protected:
    virtual void beginStateAppear();
    virtual void stateAppear();
    virtual void endStateAppear();
    virtual void beginStateMain();
    virtual void stateMain();
    virtual void endStateMain();
    virtual void beginStateAttack1();
    virtual void stateAttack1();
    virtual void endStateAttack1();
    virtual void beginStateDead();
    virtual void stateDead();
    virtual void endStateDead();
};

#endif /* GAME_ENEMY_ENEMYHEARTSOLDIER_H */
