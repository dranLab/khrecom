#ifndef GAME_ENEMY_ENEMYPOWERWILD_H
#define GAME_ENEMY_ENEMYPOWERWILD_H

#include "Game/Enemy/Heartless.h"

class CEnemyPowerwild : public CHeartless {
public:
    CEnemyPowerwild(CTask* inParent);
    virtual ~CEnemyPowerwild();
protected:
    virtual void beginStateMain();
    virtual void stateMain();
    virtual void endStateMain();
    virtual void beginStateAttack1();
    virtual void stateAttack1();
    virtual void beginStateAttack2();
    virtual void stateAttack2();
};

#endif /* GAME_ENEMY_ENEMYPOWERWILD_H */
