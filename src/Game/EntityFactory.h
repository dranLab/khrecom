// PARTIAL: added for Game/Bullet/CreateBullet
#ifndef GAME_ENTITYFACTORY_H
#define GAME_ENTITYFACTORY_H

#include "khtypes.h"
#include "System/Task.h"

class CEntity;
struct BULLET_PARAM;

class CEntityFactory : public CTask {
public:
    CEntityFactory();
    virtual ~CEntityFactory();
    CEntity* createBullet(const BULLET_PARAM& inParam, CTask* inParent);
};

#endif /* GAME_ENTITYFACTORY_H */
