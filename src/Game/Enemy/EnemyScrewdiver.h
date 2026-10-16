#ifndef GAME_ENEMY_ENEMYSCREWDIVER_H
#define GAME_ENEMY_ENEMYSCREWDIVER_H

#include "Game/Enemy/Heartless.h"

class CEnemyScrewdiver : public CHeartless {
public:
    CEnemyScrewdiver(CTask* inParent);
    virtual ~CEnemyScrewdiver();
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

#endif /* GAME_ENEMY_ENEMYSCREWDIVER_H */
