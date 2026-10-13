// PARTIAL: added for Game/Gimmick/GimmickDeceiveEnemy
#ifndef GAME_SCENEENV_H
#define GAME_SCENEENV_H

#include "khtypes.h"

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
    /* PARTIAL: Form/Menu/Pause - the enum is 8 bytes (0x80000000 and -1 together need a 64-bit type):
       0x80000000 and 0xFFFEFFFF are positive (the debug info prints them truncated to 32 bits) */
    OBJ_BIT_STILL_SCR = 0x80000000L,
    OBJ_BIT_DOOR = 1920,
    OBJ_BIT_ALL = 0xFFFEFFFFL,
    OBJ_BIT_TRUE_ALL = -1
};
} // namespace ObjType

class CSceneEnv {
public:
    static void setStop(ObjType::eObjBit inBit);
    static ObjType::eObjBit getStopAndRequestTypeBit(); // PARTIAL: added for Form/Menu/Pause
};

#endif /* GAME_SCENEENV_H */
