#ifndef GAME_BULLET_BULLETMANAGER_H
#define GAME_BULLET_BULLETMANAGER_H

#include "khtypes.h"
#include "System/Task.h"

class CBulletManager : public CTask {
public:
    CBulletManager(CTask* inParent);
    virtual ~CBulletManager();
    INT kill(HANDLE inOwner);
    BOOL isExist(HANDLE inOwner);
};

#endif /* GAME_BULLET_BULLETMANAGER_H */
