// PARTIAL: added for Game/Gimmick/GimmickDeceiveEnemy
#ifndef GAME_ENEMY_MAPENEMYMANAGER_H
#define GAME_ENEMY_MAPENEMYMANAGER_H

#include "khtypes.h"

class CMapEnemyManager {
public:
    static void setEncountEnemy(SHORT inEnemyID, SHORT inParam, UBYTE inDmgKind, UBYTE inFlag);
    static void setEncountEventEnemy(INT inEnemyID, UBYTE inParam); // PARTIAL: added for Game/Gimmick/GimmickTutorial
};

#endif /* GAME_ENEMY_MAPENEMYMANAGER_H */
