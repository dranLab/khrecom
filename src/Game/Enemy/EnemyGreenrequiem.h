#ifndef GAME_ENEMY_ENEMYGREENREQUIEM_H
#define GAME_ENEMY_ENEMYGREENREQUIEM_H

#include "Game/Enemy/Heartless.h"

class CEnemyGreenrequiem : public CHeartless {
private:
    HANDLE m_hCureTarget; /* 0x880 */

public:
    CEnemyGreenrequiem(CTask* inParent);
    virtual ~CEnemyGreenrequiem();
protected:
    virtual void beginStateMain();
    virtual void stateMain();
    virtual void endStateMain();
    virtual void beginStateAttack1();
    virtual void stateAttack1();
private:
    HANDLE searchRecoverEnemy_();
    void recover_();
};

#endif /* GAME_ENEMY_ENEMYGREENREQUIEM_H */
