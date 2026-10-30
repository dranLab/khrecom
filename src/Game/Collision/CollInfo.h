#ifndef GAME_COLLISION_COLLINFO_H
#define GAME_COLLISION_COLLINFO_H

#include "khtypes.h"

/* PARTIAL: no header of its own in the ref (debug info: types.h, owner Game/Movement); string-free, so the include
 * position does not matter */
union COLL_INFO_UNION {
    INT info;
    struct {
        BYTE infoID;
        BYTE infoATTR;
        BYTE infoPID;
        BYTE infoSTAT;
    };
};

#endif /* GAME_COLLISION_COLLINFO_H */
