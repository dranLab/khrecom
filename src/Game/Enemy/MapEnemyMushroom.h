#ifndef GAME_ENEMY_MAPENEMYMUSHROOM_H
#define GAME_ENEMY_MAPENEMYMUSHROOM_H

#include "khtypes.h"
#include "Game/Enemy/MapEnemy.h"

class CMapEnemyMushroom : public CMapEnemy {
private:
    INT m_iStep; /* 0x340 */

public:
    CMapEnemyMushroom(CTask* inParent);
    virtual ~CMapEnemyMushroom();

protected:
    virtual void beginStateAppear();
    virtual void stateAppear();
    virtual void endStateAppear();
    virtual void beginStateMain();
    virtual void stateMain();
    virtual void endStateMain();
    virtual void beginStateDisappear();
    virtual void stateDisappear();
    virtual void endStateDisappear();

private:
    BOOL isWhitemushroom() const;
};

#endif /* GAME_ENEMY_MAPENEMYMUSHROOM_H */
