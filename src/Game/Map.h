// PARTIAL: added for Game/MapUnitTable
#ifndef GAME_MAP_H
#define GAME_MAP_H

#include "khtypes.h"
#include "System/Vector.h" // PARTIAL: added for System/Graphic/LightUnit

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

// PARTIAL: added for System/Graphic/LightUnit
struct POINTLIGHT;

struct INFINITELIGHT {
    FVECTOR dir;
    FVECTOR col;
};

struct SPOTLIGHT {
    FVECTOR pos;
    FVECTOR col;
};

struct AMBIENTLIGHT {
    FVECTOR col;
};

struct LIGHTDATA {
    INT infinite_suu;
    INT point_suu;
    INT spot_suu;
    INT dum0;
    INFINITELIGHT* infinite;
    POINTLIGHT* point;
    SPOTLIGHT* spot;
    AMBIENTLIGHT* ambient;
};

// PARTIAL: added for System/Graphic/LightUnit (only the members it uses). The original derives from CTask;
// the base is left out (as padding) so that includers without System/Task.h (Game/MapUnitTable) don't get its
// .rodata string: cast CTaskMan::getSysTask()'s result.
class CMap {
private:
    UBYTE _pad[0x15E8];
    LIGHTDATA m_Light;    /* 0x15e8 */
    LIGHTDATA m_MapLight; /* 0x1608 */

public:
    LIGHTDATA* getLightData() { return &m_Light; }
    LIGHTDATA* getMapLightData() { return &m_MapLight; }
};

#endif // GAME_MAP_H
