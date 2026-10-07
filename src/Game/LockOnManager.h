// PARTIAL: added for Game/Gimmick/GimmickHitMore
#ifndef GAME_LOCKONMANAGER_H
#define GAME_LOCKONMANAGER_H

#include "khtypes.h"
#include "System/Task.h"

namespace LockOnManager {

class CLockOnManager : public CTask {
public:
    CLockOnManager();
    static BOOL registerLockOnObject(HANDLE inHandle);
};

} // namespace LockOnManager

#endif /* GAME_LOCKONMANAGER_H */
