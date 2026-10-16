#ifndef GAME_ENEMY_ENEMYSEARCHGHOST_H
#define GAME_ENEMY_ENEMYSEARCHGHOST_H

#include "Game/Enemy/Heartless.h"

class CEnemySearchghost : public CHeartless {
private:
    HANDLE m_hAbsorb; /* 0x880 */

public:
    CEnemySearchghost(CTask* inParent);
    virtual ~CEnemySearchghost();
protected:
    virtual void beginStateMain();
    virtual void stateMain();
    virtual void endStateMain();
    virtual void beginStateAttack1();
    virtual void stateAttack1();
    virtual void beginStateAttack2();
    virtual void stateAttack2();
};

#endif /* GAME_ENEMY_ENEMYSEARCHGHOST_H */
