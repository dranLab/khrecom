#ifndef GAME_ENEMY_ENEMYREDNOCTURNE_H
#define GAME_ENEMY_ENEMYREDNOCTURNE_H

#include "Game/Enemy/Heartless.h"

struct BULLET_EFFECT_INFO;

class CEnemyRednocturne : public CHeartless {
public:
    CEnemyRednocturne(CTask* inParent);
    virtual ~CEnemyRednocturne();
protected:
    virtual void beginStateMain();
    virtual void stateMain();
    virtual void endStateMain();
    virtual void beginStateAttack1();
    virtual void stateAttack1();
    virtual void beginStateAttack2();
    virtual void stateAttack2();
private:
    void createFire_(INT inAtkIndex, const BULLET_EFFECT_INFO* inInfo, const FVECTOR& inPos, FLOAT inSpeed, FLOAT inRadius);
};

#endif /* GAME_ENEMY_ENEMYREDNOCTURNE_H */
