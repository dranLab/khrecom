// PARTIAL: added for Game/Gimmick/GimmickHitReturn
#ifndef SYSTEM_GRAPHIC_EFFECT_EFFECTMAN_H
#define SYSTEM_GRAPHIC_EFFECT_EFFECTMAN_H

#include "khtypes.h"

union FMATRIX;
union FVECTOR; // PARTIAL: added for Game/Gimmick/GimmickTutorial
class CEffect; // PARTIAL: added for Game/Player/SummonTink
class CCoord;  // PARTIAL: added for Form/Menu/WorldSelect

#include "System/Graphic/Effect/ObjType.h"

// PARTIAL: moved here from Game/SceneEnv.h for Game/Player/Summon (CEffectMan::killAll)
namespace ObjType {
enum eObjBit {
    OBJ_BIT_CLEAR = 0,
    OBJ_BIT_NONE = 1,
    OBJ_BIT_PLAYER = 2,
    OBJ_BIT_FRIEND = 131072,
    OBJ_BIT_ENEMY = 4,
    OBJ_BIT_GIMMICK = 8,
    OBJ_BIT_AVATAR = 16,
    OBJ_BIT_BULLET = 32,
    OBJ_BIT_PRIZE = 64,
    OBJ_BIT_DOOR_RU = 128,
    OBJ_BIT_DOOR_RD = 256,
    OBJ_BIT_DOOR_LD = 512,
    OBJ_BIT_DOOR_LU = 1024,
    OBJ_BIT_MAP = 2048,
    OBJ_BIT_BG = 4096,
    OBJ_BIT_ATEL = 8192,
    OBJ_BIT_FUKIDASI = 16384,
    OBJ_BIT_CAMERA = 32768,
    OBJ_BIT_2D = 65536,
    OBJ_BIT_ENCOUNT = 1073741824,
    /* the enum is 8 bytes (0x80000000 and -1 together need a 64-bit type): 0x80000000 and 0xFFFEFFFF are
       positive (the debug info prints them truncated to 32 bits) */
    OBJ_BIT_STILL_SCR = 0x80000000L,
    OBJ_BIT_DOOR = 1920,
    OBJ_BIT_ALL = 0xFFFEFFFFL,
    OBJ_BIT_TRUE_ALL = -1
};
} // namespace ObjType

class CEffectMan {
public:
    static void setFogRange(FLOAT fNear, FLOAT fFar);            // PARTIAL: added for System/Graphic/WorldEnv
    static void setFogClamp(unsigned char nMin, unsigned char nMax);
    static HANDLE setEffect(const CHAR* inName, UINT inFlag, ObjType::eObjType inType, const FMATRIX* inMat, FLOAT inScale);
    // PARTIAL: added for Game/Gimmick/GimmickTutorial
    static HANDLE setEffect(const CHAR* inName, UINT inFlag, ObjType::eObjType inType, const FVECTOR* inPos,
                            const FVECTOR* inRot, FLOAT inScale);
    static CEffect* getEffect(HANDLE inHandle); // PARTIAL: added for Game/Player/SummonTink
    // PARTIAL: added for Form/Menu/WorldSelect
    static HANDLE attachEffect(const CHAR* inName, UINT inNo, ObjType::eObjType inType, const CCoord* inParent,
                               const FVECTOR* inPos, const FVECTOR* inRot, FLOAT inWait);
    static void killEffect(HANDLE inHandle, FLOAT inTime);
    static void killAll(ObjType::eObjBit inBit, FLOAT inTime); // PARTIAL: added for Game/Player/Summon
    static void killAll(const CHAR* inName, FLOAT inTime); // PARTIAL: added for Game/Player/PlayerManager
};

#endif /* SYSTEM_GRAPHIC_EFFECT_EFFECTMAN_H */
