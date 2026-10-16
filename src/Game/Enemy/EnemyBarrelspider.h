#ifndef GAME_ENEMY_ENEMYBARRELSPIDER_H
#define GAME_ENEMY_ENEMYBARRELSPIDER_H

#include "Game/Enemy/Heartless.h"

class CEnemyBarrelspider : public CHeartless {
private:
    TIMEQ m_tqBaseTime; /* 0x880 */

public:
    CEnemyBarrelspider(CTask* inParent);
    virtual ~CEnemyBarrelspider();
    virtual BOOL initalize(INT inGroupID, INT inRsrcID);
protected:
    virtual void beginStateMain();
    virtual void stateMain();
    virtual void endStateMain();
    virtual void beginStateAttack1();
    virtual void stateAttack1();
    virtual void beginStateAttack2();
    virtual void stateAttack2();
    virtual void beginStateTerror();
private:
    CEntity* createExplode_(INT inIndex);
};

#endif /* GAME_ENEMY_ENEMYBARRELSPIDER_H */
