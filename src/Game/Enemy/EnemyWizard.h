#ifndef GAME_ENEMY_ENEMYWIZARD_H
#define GAME_ENEMY_ENEMYWIZARD_H

#include "Game/Enemy/Heartless.h"

class CEnemyWizard : public CHeartless {
public:
    CEnemyWizard(CTask* inParent);
    virtual ~CEnemyWizard();
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
    virtual void onAbsorb();
};

#endif /* GAME_ENEMY_ENEMYWIZARD_H */
