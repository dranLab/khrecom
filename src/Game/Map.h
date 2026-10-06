// PARTIAL: added for Game/MapUnitTable
#ifndef GAME_MAP_H
#define GAME_MAP_H

#include "khtypes.h"

union ISVECTOR {
    short int v[4];
    struct {
        SHORT x;
        SHORT y;
        SHORT z;
        SHORT w;
    };
    ULONGLONG _u64;
    LONGLONG _s64;
};

struct ZONEPARTS {
    SHORT modelIndex;
    SHORT crdIndex;
    SHORT groupNo;
    SHORT __semiTransKind;
    ISVECTOR rot;
    FLOAT trans[4]; // FVECTOR in the original
    FLOAT scale[4]; // FVECTOR in the original
    UINT effectId;
    unsigned char __envId[4];
    FLOAT __waterLine;
    USHORT effectParam;
    USHORT _sdum0;
};

#endif // GAME_MAP_H
