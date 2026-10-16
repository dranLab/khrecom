#ifndef GAME_ENEMY_ENEMYFATBANDIT_H
#define GAME_ENEMY_ENEMYFATBANDIT_H

#include "Game/Enemy/Heartless.h"

class CEnemyFatbandit : public CHeartless {
public:
    CEnemyFatbandit(CTask* inParent);
    virtual ~CEnemyFatbandit();
protected:
    virtual void beginStateMain();
    virtual void stateMain();
    virtual void endStateMain();
    virtual void beginStateAttack1();
    virtual void stateAttack1();
    virtual void beginStateAttack2();
    virtual void stateAttack2();
};

#endif /* GAME_ENEMY_ENEMYFATBANDIT_H */
