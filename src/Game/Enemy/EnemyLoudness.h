#ifndef GAME_ENEMY_ENEMYLOUDNESS_H
#define GAME_ENEMY_ENEMYLOUDNESS_H

#include "Game/Enemy/Heartless.h"

class CEnemyLoudness : public CHeartless {
public:
    CEnemyLoudness(CTask* inParent);
    virtual ~CEnemyLoudness();
protected:
    virtual void beginStateMain();
    virtual void stateMain();
    virtual void endStateMain();
    virtual void beginStateAttack1();
    virtual void stateAttack1();
    virtual void endStateAttack1();
    virtual void beginStateAttack2();
    virtual void stateAttack2();
    virtual void endStateAttack2();
    virtual void beginStateAttack3();
    virtual void stateAttack3();
    virtual void endStateAttack3();
    virtual void aiCardBattle();
private:
    HANDLE searchRecoverEnemy_();
    BOOL canRecoverEnemy_();
    BOOL canSetEnemy_(const FVECTOR& inTarget);
};

#endif /* GAME_ENEMY_ENEMYLOUDNESS_H */
