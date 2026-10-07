#ifndef GAME_COLLPARAM_H
#define GAME_COLLPARAM_H

#include "khtypes.h"

enum ATK_KIND {
    ATK_KIND_NONE = 0,
    ATK_KIND_DMG_SMALL = 1,
    ATK_KIND_DMG_BIG = 2,
    ATK_KIND_DMG_BLOWN = 3,
    ATK_KIND_DMG_TOSS = 4,
    ATK_KIND_DMG_BEAT = 5,
    ATK_KIND_DMG_VOLLEY = 6,
    ATK_KIND_STUN = 7,
    ATK_KIND_STOP = 8,
    ATK_KIND_BIND = 9,
    ATK_KIND_CONFUSE = 10,
    ATK_KIND_TERROR = 11,
    ATK_KIND_FREEZE = 12,
    ATK_KIND_GRAVITY = 13,
    ATK_KIND_DEGENERATOR = 14,
    ATK_KIND_DEGENERATE = 15,
    ATK_KIND_SHOCK = 16,
    ATK_KIND_TORNADO = 17,
    ATK_KIND_QUAKE = 18,
    ATK_KIND_SYNCHRO = 19,
    ATK_KIND_MAGNET = 20,
    ATK_KIND_DRAIN = 21,
    ATK_KIND_SUSPEND = 22,
    ATK_KIND_ABSOLUTE_STOP = 23,
    ATK_KIND_BANANA_SKIN = 24,
    ATK_KIND_RECOVER = 25,
    ATK_KIND_WITHOUT = 26,
    ATK_KIND_BLADE_STORM = 27,
    ATK_KIND_THUNDER = 28,
    ATK_KIND_THUNDRA = 29,
    ATK_KIND_THUNDAGA = 30,
    ATK_KIND_DEGENERATE_BREAK = 31,
    ATK_KIND_DARK_PUNISHER = 32,
    ATK_KIND_PRONOUNCE = 33,
    ATK_KIND_DEATH = 34,
    ATK_KIND_DISCARD = 35,
    ATK_KIND_GM_DAMAGE = 36,
    ATK_KIND_GM_FAINT = 37,
    ATK_KIND_GM_BIG_DAMAGE = 38,
    ATK_KIND_GM_JACK_BOX = 39,
    ATK_KIND_GM_TENTACLE = 40,
    ATK_KIND_GM_DECISION = 41,
    ATK_KIND_GM_CONFUSE = 42
};

struct BONE_OFFSET { // 0x20
    /* 0x00 */ char szBoneName[16];
    /* 0x10 */ INT iBoneIndex;
    /* 0x14 */ FLOAT fOffsetX;
    /* 0x18 */ FLOAT fOffsetY;
    /* 0x1c */ FLOAT fOffsetZ;
};

struct BODY_COLL_PARAM { // 0x50
    /* 0x00 */ BONE_OFFSET BoneOffset[2];
    /* 0x40 */ INT iCollType;
    /* 0x44 */ FLOAT fRadius;
    /* 0x48 */ UINT uiFlag;
    /* 0x4c */ FLOAT fCoefficient;
};

struct ATK_COLL_PARAM { // 0xa0
    /* 0x00 */ BONE_OFFSET BoneOffset[2];
    /* 0x40 */ char szName[16];
    /* 0x50 */ INT iMotIndex;
    /* 0x54 */ INT iCollType;
    /* 0x58 */ FLOAT fRadius;
    /* 0x5c */ TIMEQ tqStartTime;
    /* 0x60 */ TIMEQ tqEndTime;
    /* 0x64 */ INT iAtkDataIndex;
    /* 0x68 */ INT iAtkKind;
    /* 0x6c */ INT iHitEffectNo;
    /* 0x70 */ char szHitEffectDpx[32];
    /* 0x90 */ INT iHitSEFileID;
    /* 0x94 */ INT iHitSESEID;
    /* 0x98 */ BOOL bEnable;
    /* 0x9c */ UINT uiFlag;

    ATK_KIND getAtkKind() const;
};

struct OTHER_PARAM; // 0xc0

struct COLL_PARAM_HEADER {
    /* 0x00 */ char szCode[4];
    /* 0x04 */ UINT uiVersion;
    /* 0x08 */ INT iBodyCollParamNum;
    /* 0x0c */ BODY_COLL_PARAM* pBodyCollParam;
    /* 0x10 */ INT iAtkCollParamNum;
    /* 0x14 */ ATK_COLL_PARAM* pAtkCollParam;
    /* 0x18 */ OTHER_PARAM* pOtherParam;

    BOOL addrMapping();
};

#endif // GAME_COLLPARAM_H
