#ifndef GAME_ENEMY_ENEMYYELLOWOPERA_H
#define GAME_ENEMY_ENEMYYELLOWOPERA_H

#include "Game/Enemy/Heartless.h"

class CEnemyYellowopera : public CHeartless {
public:
    CEnemyYellowopera(CTask* inParent);
    virtual ~CEnemyYellowopera();
protected:
    virtual void beginStateMain();
    virtual void stateMain();
    virtual void endStateMain();
    virtual void beginStateAttack1();
    virtual void stateAttack1();
    virtual void beginStateAttack2();
    virtual void stateAttack2();
    virtual void endStateAttack2();
};

#endif /* GAME_ENEMY_ENEMYYELLOWOPERA_H */
