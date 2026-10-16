#ifndef GAME_ENEMY_ENEMYWHITEMUSHROOM_H
#define GAME_ENEMY_ENEMYWHITEMUSHROOM_H

#include "Game/Enemy/Heartless.h"

class CEnemyWhitemushroom : public CHeartless {
private:
    INT m_iStep;       /* 0x880 */
    INT m_iHappyCount; /* 0x884 */
    INT m_iCounter;    /* 0x888 */

public:
    CEnemyWhitemushroom(CTask* inParent);
    virtual ~CEnemyWhitemushroom();
protected:
    virtual void beginStateMain();
    virtual void stateMain();
    virtual void endStateMain();
    virtual void onInvalid();
    virtual void beginStateSmallDamage();
    virtual void stateSmallDamage();
    virtual void endStateSmallDamage();
    virtual void beginStateBigDamage();
    virtual void stateBigDamage();
    virtual void endStateBigDamage();
    virtual void beginStateDead();
    virtual void stateDead();
private:
    void beginDamage_();
    void damage_();
    void endDamage_();
    void setupDisappear_();
};

#endif /* GAME_ENEMY_ENEMYWHITEMUSHROOM_H */
