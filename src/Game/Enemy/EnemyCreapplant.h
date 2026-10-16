#ifndef GAME_ENEMY_ENEMYCREAPPLANT_H
#define GAME_ENEMY_ENEMYCREAPPLANT_H

#include "Game/Enemy/Heartless.h"

class CEnemyCreapplant : public CHeartless {
public:
    CEnemyCreapplant(CTask* inParent);
    virtual ~CEnemyCreapplant();
protected:
    virtual void beginStateMain();
    virtual void stateMain();
    virtual void endStateMain();
    virtual void beginStateAttack1();
    virtual void stateAttack1();
    virtual void endStateAttack1();
    virtual void beginStateAttack2();
    virtual void stateAttack2();
    virtual void endStateAttack2();
    virtual void beginStateTerror();
    virtual void stateTerror();
    virtual void endStateTerror();
    virtual void onCardBreak();
};

#endif /* GAME_ENEMY_ENEMYCREAPPLANT_H */
