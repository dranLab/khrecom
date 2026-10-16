#ifndef GAME_ENEMY_ENEMYTORNADOSTEP_H
#define GAME_ENEMY_ENEMYTORNADOSTEP_H

#include "Game/Enemy/Heartless.h"

class CEnemyTornadostep : public CHeartless {
private:
    INT m_iMoveStep; /* 0x880 */

public:
    CEnemyTornadostep(CTask* inParent);
    virtual ~CEnemyTornadostep();
protected:
    virtual void beginStateMain();
    virtual void stateMain();
    virtual void endStateMain();
    virtual void beginStateAttack1();
    virtual void stateAttack1();
    virtual void beginStateAttack2();
    virtual void stateAttack2();
private:
    virtual void endStateAttack1();
    virtual void endStateAttack2();
};

#endif /* GAME_ENEMY_ENEMYTORNADOSTEP_H */
