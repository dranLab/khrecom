// PARTIAL: added for Game/Enemy/MapEnemyGround
#ifndef GAME_ENEMY_ENEMYPARAM_H
#define GAME_ENEMY_ENEMYPARAM_H

#include "khtypes.h"
#include "System/MainApp.h"

// PARTIAL: added for Game/Enemy/EnemyAirpirate (layouts from types.h, only the fields used so far)
struct ENEMY_ACT {
    INT iCard[3];     /* 0x00 */
    INT iAttribute;   /* 0x0c */
    FLOAT fPower;     /* 0x10 */
    FLOAT fViewAngle; /* 0x14 */
    INT iActNo;       /* 0x18 */
    FLOAT fDistance;  /* 0x1c */
};

struct ENEMY_PARAM {
    UBYTE _m_0x00[0x4c]; /* 0x00 szCode .. 0x48 pCards */
    INT iActCount;       /* 0x4c */
    ENEMY_ACT* pActs;    /* 0x50 */
    UBYTE _m_0x54[0x5c]; /* 0x54 iEnemyCard .. 0xa8 pad */
};

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
