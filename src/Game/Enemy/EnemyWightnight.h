#ifndef GAME_ENEMY_ENEMYWIGHTNIGHT_H
#define GAME_ENEMY_ENEMYWIGHTNIGHT_H

#include "Game/Enemy/Heartless.h"

class CEnemyWightnight : public CHeartless {
public:
    CEnemyWightnight(CTask* inParent);
    virtual ~CEnemyWightnight();
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
};

#endif /* GAME_ENEMY_ENEMYWIGHTNIGHT_H */
