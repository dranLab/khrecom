#ifndef GAME_ENEMY_ENEMYBOUNCYWILD_H
#define GAME_ENEMY_ENEMYBOUNCYWILD_H

#include "Game/Enemy/Heartless.h"

class CEnemyBouncywild : public CHeartless {
public:
    CEnemyBouncywild(CTask* inParent);
    virtual ~CEnemyBouncywild();
protected:
    virtual void beginStateMain();
    virtual void stateMain();
    virtual void endStateMain();
    virtual void beginStateAttack1();
    virtual void stateAttack1();
    virtual void beginStateAttack2();
    virtual void stateAttack2();
};

#endif /* GAME_ENEMY_ENEMYBOUNCYWILD_H */
