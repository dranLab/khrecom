#ifndef GAME_ENEMY_MAPENEMYAIR_H
#define GAME_ENEMY_MAPENEMYAIR_H

#include "khtypes.h"
#include "Game/Enemy/MapEnemy.h"

class CMapEnemyAir : public CMapEnemy {
private:
    INT m_iStep; /* 0x340 */

public:
    CMapEnemyAir(CTask* inParent);
    virtual ~CMapEnemyAir();

protected:
    virtual void beginStateAppear();
    virtual void stateAppear();
    virtual void endStateAppear();
    virtual void beginStateMain();
    virtual void stateMain();
    virtual void endStateMain();
};

#endif /* GAME_ENEMY_MAPENEMYAIR_H */
