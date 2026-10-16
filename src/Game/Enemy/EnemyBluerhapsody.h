#ifndef GAME_ENEMY_ENEMYBLUERHAPSODY_H
#define GAME_ENEMY_ENEMYBLUERHAPSODY_H

#include "Game/Enemy/Heartless.h"

class CEnemyBluerhapsody : public CHeartless {
public:
    CEnemyBluerhapsody(CTask* inParent);
    virtual ~CEnemyBluerhapsody();
protected:
    virtual void beginStateMain();
    virtual void stateMain();
    virtual void endStateMain();
    virtual void beginStateAttack1();
    virtual void stateAttack1();
    virtual void beginStateAttack2();
    virtual void stateAttack2();
};

#endif /* GAME_ENEMY_ENEMYBLUERHAPSODY_H */
