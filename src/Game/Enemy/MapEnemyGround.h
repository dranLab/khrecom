#ifndef GAME_ENEMY_MAPENEMYGROUND_H
#define GAME_ENEMY_MAPENEMYGROUND_H

#include "khtypes.h"
#include "Game/Enemy/MapEnemy.h"

class CMapEnemyGround : public CMapEnemy {
private:
    INT m_iStep; /* 0x340 */

public:
    CMapEnemyGround(CTask* inParent);
    virtual ~CMapEnemyGround();

protected:
    virtual void beginStateAppear();
    virtual void stateAppear();
    virtual void endStateAppear();
    virtual void beginStateMain();
    virtual void stateMain();
    virtual void endStateMain();
};

#endif /* GAME_ENEMY_MAPENEMYGROUND_H */
