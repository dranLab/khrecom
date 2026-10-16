#ifndef GAME_ENEMY_ENEMYAIRPIRATE_H
#define GAME_ENEMY_ENEMYAIRPIRATE_H

#include "Game/Enemy/Heartless.h"

class CEnemyAirpirate : public CHeartless {
public:
    CEnemyAirpirate(CTask* inParent);
    virtual ~CEnemyAirpirate();
protected:
    virtual void beginStateMain();
    virtual void stateMain();
    virtual void endStateMain();
    virtual void beginStateAttack1();
    virtual void stateAttack1();
    virtual void beginStateAttack2();
    virtual void stateAttack2();
    virtual void aiCardBattle();
};

#endif /* GAME_ENEMY_ENEMYAIRPIRATE_H */
