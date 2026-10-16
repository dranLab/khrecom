#ifndef GAME_ENEMY_ENEMYSEANEON_H
#define GAME_ENEMY_ENEMYSEANEON_H

#include "Game/Enemy/Heartless.h"

class CEnemySeaneon : public CHeartless {
private:
    FLOAT m_fAtk1Spd; /* 0x880 */

public:
    CEnemySeaneon(CTask* inParent);
    virtual ~CEnemySeaneon();
protected:
    virtual void beginStateMain();
    virtual void stateMain();
    virtual void endStateMain();
    virtual void beginStateAttack1();
    virtual void stateAttack1();
    virtual void beginStateAttack2();
    virtual void stateAttack2();
};

#endif /* GAME_ENEMY_ENEMYSEANEON_H */
