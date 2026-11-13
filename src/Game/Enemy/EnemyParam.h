// PARTIAL: added for Game/Enemy/MapEnemyGround
#ifndef GAME_ENEMY_ENEMYPARAM_H
#define GAME_ENEMY_ENEMYPARAM_H

#include "khtypes.h"
#include "System/MainApp.h"
#include <string.h>

// PARTIAL: added for Game/Enemy/EnemyAirpirate (layouts from types.h, only the fields used so far)
struct ENEMY_ACT {
    INT iCard[3];     /* 0x00 */
    INT iAttribute;   /* 0x0c */
    FLOAT fPower;     /* 0x10 */
    FLOAT fViewAngle; /* 0x14 */
    INT iActNo;       /* 0x18 */
    FLOAT fDistance;  /* 0x1c */
};

// PARTIAL: added for Game/Enemy/Heartless
struct ENEMY_CARD {
    INT iCardKind;      /* 0x00 */
    BYTE bNumber[10];   /* 0x04 */
    BYTE pad[2];
};


struct EXTRA_ENEMY_PARAM {
    char szName[12]; /* 0x0 */
    FLOAT fValue;    /* 0xc */
};

struct BASIC_ENEMY_PARAM {
    INT iHP;  /* 0x0 */
    INT iAP;  /* 0x4 */
    INT iExp; /* 0x8 */
    INT pad;  /* 0xc */
};

struct ENEMY_PARAM {
    char szCode[4];              /* 0x00 */
    UINT uiVersion;              /* 0x04 */
    struct BASIC_ENEMY_PARAM* pBasic; /* 0x08 */
    UINT uiSize;                 /* 0x0c */
    UINT uiEnemyCardDrop;        /* 0x10 */
    float fAttr[5];              /* 0x14 */
    int iSpecialEffect[7];       /* 0x28 */
    INT iCardCount;      /* 0x44 */
    struct ENEMY_CARD* pCards; /* 0x48 */
    INT iActCount;       /* 0x4c */
    ENEMY_ACT* pActs;    /* 0x50 */
    INT iEnemyCard;      /* 0x54 */
    unsigned char byEnemyCards[4]; /* 0x58 */
    int iDamageMotionIndex[10]; /* 0x5c */
    int iFaintMotionIndex[3]; /* 0x84 */
    INT iExtraParamCount;            /* 0x90 */
    struct EXTRA_ENEMY_PARAM* pExtraParam; /* 0x94 */
    int iAttackRate[2];              /* 0x98 */
    INT iAppearMotionIndex;    /* 0xa0 */
    INT iDisappearMotionIndex; /* 0xa4 */
    INT pad[2];                /* 0xa8 */

    BOOL addrMapping(/* parameters unknown */);
    BASIC_ENEMY_PARAM* getBasicParam(/* parameters unknown */);
    ENEMY_ACT* getEnemyAct(/* parameters unknown */);
    int getSpecialEffect(INT inIndex) const { return iSpecialEffect[inIndex]; }
    FLOAT getAttr(/* parameters unknown */);
    INT getDmgMotIndex(/* parameters unknown */);
    BOOL isDamageMotion(INT inIndex) const {
        for (INT i = 0; i < 10; i++) {
            if (iDamageMotionIndex[i] == inIndex) return TRUE;
        }
        return FALSE;
    }
    INT getFaintMotIndex(/* parameters unknown */);
    FLOAT getExtraParam(char* inName) {
        for (INT i = 0; i < iExtraParamCount; i++) {
            if (!strcmp(pExtraParam[i].szName, inName)) return pExtraParam[i].fValue;
        }
        return 0.0f;
    }
};

/* Placeholders for the original header's inline functions: their strings ("@ENP", the parameter file's
 * signature, then the index asserts) are in .rodata of every includer, in this order. Line numbers are not known. */
inline BOOL _enemyParamCheckSignature(const CHAR* inData) {
    return *(const INT*)inData == *(const INT*)"@ENP";
}
inline void _enemyParamCheckIndex(INT inFloor, INT inIndex) {
    KH_DEBUG(if (!(0 <= inFloor && inFloor < 4)) AssertApp(__FILE__, __LINE__, "0 <= inFloor && inFloor < BASIC_MAX", 0);)
    KH_DEBUG(if (!(0 <= inIndex && inIndex < 4)) AssertApp(__FILE__, __LINE__, "0 <= inIndex && inIndex < SPE_MAX", 0);)
    KH_DEBUG(if (!(0 <= inIndex && inIndex < 4)) AssertApp(__FILE__, __LINE__, "0 <= inIndex && inIndex < DMG_MOT_MAX", 0);)
    KH_DEBUG(if (!(0 <= inIndex && inIndex < 4)) AssertApp(__FILE__, __LINE__, "0 <= inIndex && inIndex < FAINT_MOT_MAX", 0);)
}

#endif /* GAME_ENEMY_ENEMYPARAM_H */
