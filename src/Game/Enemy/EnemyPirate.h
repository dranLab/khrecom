#ifndef GAME_ENEMY_ENEMYPIRATE_H
#define GAME_ENEMY_ENEMYPIRATE_H

#include "Game/Enemy/Heartless.h"

class CEnemyPirate : public CHeartless {
public:
    CEnemyPirate(CTask* inParent);
    virtual ~CEnemyPirate();
protected:
    virtual void beginStateMain();
    virtual void stateMain();
    virtual void endStateMain();
    virtual void beginStateAttack1();
    virtual void stateAttack1();
    virtual void beginStateAttack2();
    virtual void stateAttack2();
};

#endif /* GAME_ENEMY_ENEMYPIRATE_H */
