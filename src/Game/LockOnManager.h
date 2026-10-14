// PARTIAL: added for Game/Gimmick/GimmickHitMore
#ifndef GAME_LOCKONMANAGER_H
#define GAME_LOCKONMANAGER_H

#include "khtypes.h"
#include "System/Task.h"

class CEntity; // PARTIAL: added for Game/Player/FriendAriel

namespace LockOnManager {

class CLockOnManager : public CTask {
public:
    CLockOnManager();
    static BOOL registerLockOnObject(HANDLE inHandle);
    // PARTIAL: added for Game/Player/FriendAriel
    static HANDLE getLockOnObject(CEntity& inEntity);
    static HANDLE getRandomLockOnObject();
};

} // namespace LockOnManager

#endif /* GAME_LOCKONMANAGER_H */
