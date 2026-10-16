#ifndef GAME_ENEMY_ENEMYLARGEBODY_H
#define GAME_ENEMY_ENEMYLARGEBODY_H

#include "Game/Enemy/Heartless.h"

class CEnemyLargebody : public CHeartless {
private:
    FLOAT m_fAtkCollRadius; /* 0x880 */

public:
    CEnemyLargebody(CTask* inParent);
    virtual ~CEnemyLargebody();
protected:
    virtual void beginStateMain();
    virtual void stateMain();
    virtual void endStateMain();
    virtual void beginStateAttack1();
    virtual void stateAttack1();
    virtual void beginStateAttack2();
    virtual void stateAttack2();
};

#endif /* GAME_ENEMY_ENEMYLARGEBODY_H */
