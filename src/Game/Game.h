// PARTIAL: added for Game/Gimmick/GimmickSavePoint
#ifndef GAME_GAME_H
#define GAME_GAME_H

#include "khtypes.h"
#include "System/Task.h"

class CGame : public CTask {
public:
    CGame();
    static BOOL reqSave();
    static BOOL isSoraGame(); // PARTIAL: added for Game/Gimmick/GimmickWarpPoint
    static void enablePause(BOOL bEnable); // PARTIAL: added for Form/Menu/GameOver
    static BOOL reqContinue_GameOver();    // PARTIAL: added for Form/Menu/GameOver
    static BOOL reqExit_GameOver();        // PARTIAL: added for Form/Menu/GameOver
    static BOOL isSuspended();             // PARTIAL: added for Game/SceneEnv
};

#endif /* GAME_GAME_H */
