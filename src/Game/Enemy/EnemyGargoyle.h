#ifndef GAME_ENEMY_ENEMYGARGOYLE_H
#define GAME_ENEMY_ENEMYGARGOYLE_H

#include "Game/Enemy/Heartless.h"

class CEnemyGargoyle : public CHeartless {
public:
    CEnemyGargoyle(CTask* inParent);
    virtual ~CEnemyGargoyle();
protected:
    virtual void beginStateMain();
    virtual void stateMain();
    virtual void endStateMain();
    virtual void beginStateAttack1();
    virtual void stateAttack1();
    virtual void beginStateAttack2();
    virtual void stateAttack2();
};

#endif /* GAME_ENEMY_ENEMYGARGOYLE_H */
