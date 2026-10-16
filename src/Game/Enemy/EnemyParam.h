// PARTIAL: added for Game/Enemy/MapEnemyGround
#ifndef GAME_ENEMY_ENEMYPARAM_H
#define GAME_ENEMY_ENEMYPARAM_H

#include "khtypes.h"
#include "System/MainApp.h"

/* Placeholders for the original header's inline functions: their strings ("@ENP", the parameter file's
 * signature, then the index asserts) are in .rodata of every includer, in this order. Line numbers are not known. */
inline BOOL _enemyParamCheckSignature(const CHAR* inData) {
    return *(const INT*)inData == *(const INT*)"@ENP";
}
inline void _enemyParamCheckIndex(INT inFloor, INT inIndex) {
    if (!(0 <= inFloor && inFloor < 4)) AssertApp(__FILE__, __LINE__, "0 <= inFloor && inFloor < BASIC_MAX", 0);
    if (!(0 <= inIndex && inIndex < 4)) AssertApp(__FILE__, __LINE__, "0 <= inIndex && inIndex < SPE_MAX", 0);
    if (!(0 <= inIndex && inIndex < 4)) AssertApp(__FILE__, __LINE__, "0 <= inIndex && inIndex < DMG_MOT_MAX", 0);
    if (!(0 <= inIndex && inIndex < 4)) AssertApp(__FILE__, __LINE__, "0 <= inIndex && inIndex < FAINT_MOT_MAX", 0);
}

#endif /* GAME_ENEMY_ENEMYPARAM_H */
