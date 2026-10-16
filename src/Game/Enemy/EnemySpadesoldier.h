#ifndef GAME_ENEMY_ENEMYSPADESOLDIER_H
#define GAME_ENEMY_ENEMYSPADESOLDIER_H

#include "Game/Enemy/Heartless.h"

class CEnemySpadesoldier : public CHeartless {
public:
    CEnemySpadesoldier(CTask* inParent);
    virtual ~CEnemySpadesoldier();
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
    virtual void beginStateAttack2();
    virtual void stateAttack2();
    virtual void endStateAttack2();
    virtual void beginStateDead();
    virtual void stateDead();
    virtual void endStateDead();
};

#endif /* GAME_ENEMY_ENEMYSPADESOLDIER_H */
