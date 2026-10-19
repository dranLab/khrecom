// PARTIAL: split out of System/Graphic/Effect/EffectMan.h for System/Graphic/Effect/effect.h (original header unknown)
#ifndef SYSTEM_GRAPHIC_EFFECT_OBJTYPE_H
#define SYSTEM_GRAPHIC_EFFECT_OBJTYPE_H

namespace ObjType {
enum eObjType {
    OBJ_TYPE_NONE = 0,
    OBJ_TYPE_PLAYER = 1,
    OBJ_TYPE_ENEMY = 2,
    OBJ_TYPE_GIMMICK = 3,
    OBJ_TYPE_AVATAR = 4,
    OBJ_TYPE_BULLET = 5,
    OBJ_TYPE_PRIZE = 6,
    OBJ_TYPE_DOOR_RU = 7,
    OBJ_TYPE_DOOR_RD = 8,
    OBJ_TYPE_DOOR_LD = 9,
    OBJ_TYPE_DOOR_LU = 10,
    OBJ_TYPE_MAP = 11,
    OBJ_TYPE_BG = 12,
    OBJ_TYPE_ATEL = 13,
    OBJ_TYPE_FUKIDASI = 14,
    OBJ_TYPE_CAMERA = 15,
    OBJ_TYPE_2D = 16,
    OBJ_TYPE_FRIEND = 17,
    OBJ_TYPE_ENCOUNT = 30,
    OBJ_TYPE_STILL_SCR = 31,
    OBJ_TYPE_MAX = 32
};
} // namespace ObjType

#endif /* SYSTEM_GRAPHIC_EFFECT_OBJTYPE_H */
