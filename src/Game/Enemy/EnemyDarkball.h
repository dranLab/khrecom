#ifndef GAME_ENEMY_ENEMYDARKBALL_H
#define GAME_ENEMY_ENEMYDARKBALL_H

#include "Game/Enemy/Heartless.h"

class CEnemyDarkball : public CHeartless {
public:
    CEnemyDarkball(CTask* inParent);
    virtual ~CEnemyDarkball();
    virtual BOOL initalize(INT inGroupID, INT inRsrcID);
protected:
    virtual void beginStateMain();
    virtual void stateMain();
    virtual void endStateMain();
    virtual void beginStateAttack1();
    virtual void stateAttack1();
    virtual void beginStateAttack2();
    virtual void stateAttack2();
};

#endif /* GAME_ENEMY_ENEMYDARKBALL_H */
