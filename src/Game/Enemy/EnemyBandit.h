#ifndef GAME_ENEMY_ENEMYBANDIT_H
#define GAME_ENEMY_ENEMYBANDIT_H

#include "Game/Enemy/Heartless.h"

class CEnemyBandit : public CHeartless {
public:
    CEnemyBandit(CTask* inParent);
    virtual ~CEnemyBandit();
protected:
    virtual void beginStateMain();
    virtual void stateMain();
    virtual void endStateMain();
    virtual void beginStateAttack1();
    virtual void stateAttack1();
    virtual void beginStateAttack2();
    virtual void stateAttack2();
private:
    BOOL checkFront_();
};

#endif /* GAME_ENEMY_ENEMYBANDIT_H */
