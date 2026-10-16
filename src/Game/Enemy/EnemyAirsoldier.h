#ifndef GAME_ENEMY_ENEMYAIRSOLDIER_H
#define GAME_ENEMY_ENEMYAIRSOLDIER_H

#include "Game/Enemy/Heartless.h"

class CEnemyAirsoldier : public CHeartless {
public:
    CEnemyAirsoldier(CTask* inParent);
    virtual ~CEnemyAirsoldier();
    virtual BOOL initalize(INT inGroupID, INT inRsrcID);
protected:
    virtual void beginStateMain();
    virtual void stateMain();
    virtual void endStateMain();
    virtual void beginStateAttack1();
    virtual void stateAttack1();
    virtual void beginStateAttack2();
    virtual void stateAttack2();
    virtual void aiCardBattle();
};

#endif /* GAME_ENEMY_ENEMYAIRSOLDIER_H */
