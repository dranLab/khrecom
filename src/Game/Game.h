// PARTIAL: added for Game/Gimmick/GimmickSavePoint
#ifndef GAME_GAME_H
#define GAME_GAME_H

#include "khtypes.h"
#include "System/Task.h"

class CGame : public CTask {
public:
    CGame();
    CGame(CTask* pParent);              // PARTIAL: added for System/MainTaskFactory
    CGame(CTask* pParent, INT nMode);   // PARTIAL: added for System/MainTaskFactory
    static BOOL reqSave();
    static BOOL isSoraGame(); // PARTIAL: added for Game/Gimmick/GimmickWarpPoint
    static BOOL isRikuGame(); // PARTIAL: added for Game/Player/NaviMap
    static void enablePause(BOOL bEnable); // PARTIAL: added for Form/Menu/GameOver
    static BOOL reqContinue_GameOver();    // PARTIAL: added for Form/Menu/GameOver
    static BOOL reqExit_GameOver();        // PARTIAL: added for Form/Menu/GameOver
    static BOOL isSuspended();             // PARTIAL: added for Game/SceneEnv
    static INT getMapID();                 // PARTIAL: added for System/DebugProcInfo
};

#endif /* GAME_GAME_H */
