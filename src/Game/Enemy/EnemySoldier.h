#ifndef GAME_ENEMY_ENEMYSOLDIER_H
#define GAME_ENEMY_ENEMYSOLDIER_H

#include "Game/Enemy/Heartless.h"

class CEnemySoldier : public CHeartless {
public:
    CEnemySoldier(CTask* inParent);
    virtual ~CEnemySoldier();
protected:
    virtual void beginStateMain();
    virtual void stateMain();
    virtual void endStateMain();
    virtual void beginStateAttack1();
    virtual void stateAttack1();
    virtual void beginStateAttack2();
    virtual void stateAttack2();
};

#endif /* GAME_ENEMY_ENEMYSOLDIER_H */
