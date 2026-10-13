#ifndef GAME_ENEMY_MAPENEMYSHADOW_H
#define GAME_ENEMY_MAPENEMYSHADOW_H

#include "khtypes.h"
#include "Game/Enemy/MapEnemy.h"

class CMapEnemyShadow : public CMapEnemy {
private:
    INT m_iStep; /* 0x340 */

public:
    CMapEnemyShadow(CTask* inParent);
    virtual ~CMapEnemyShadow();
    virtual BOOL initalize(INT inGroupID, INT inRsrcID);

private:
    virtual void beginStateAppear();
    virtual void stateAppear();
    virtual void endStateAppear();
    virtual void beginStateMain();
    virtual void stateMain();
    virtual void endStateMain();
    virtual void beginStateDisappear();
    virtual void stateDisappear();
    virtual void endStateDisappear();
};

#endif /* GAME_ENEMY_MAPENEMYSHADOW_H */
