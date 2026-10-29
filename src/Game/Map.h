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

struct ALF_DATA { // PARTIAL: added for Game/Map
    USHORT infinite_suu;
    USHORT nVer;
    USHORT point_suu;
    USHORT nPad0;
    USHORT spot_suu;
    USHORT nPad1;
    INT dum0;
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
    void set(const ALF_DATA* pData); // PARTIAL: added for Game/Map
};

#ifdef GAME_MAP_CPP
/* The complete class, for Game/Map (graphics members as padding). */
struct FOGDATA { // 0xb0
    IVECTOR color;
    FLOAT fognear;
    FLOAT fogfar;
    FLOAT screenz;
    INT skynum;
    IVECTOR sky[5];
    FLOAT fogmax;
    FLOAT fogmin;
    FLOAT dispfar;
    unsigned int Pad0[1];
    unsigned int Pad1[2];
    FLOAT sdwColor;
    FLOAT sdwAlpha;
    IVECTOR ivdum1;
    IVECTOR ivdum0;
};
struct ZONEDATA;
class CSky;
class CMapLightUnit;
class CTexAnm;
class CMapModel;

class CMap : public CTask {
private:
    FVECTOR m_vColor;         /* 0x040 */
    INT m_nRsrcGrpID;         /* 0x050 */
    CTexAnm* m_pTexAnm;       /* 0x054 */
    INT m_nTexAnmNum;         /* 0x058 */
    CMapModel* m_pModelTbl;   /* 0x05c */
    INT m_nModelMax;          /* 0x060 */
    FLOAT m_Deg;              /* 0x064 */
    UINT m_uiMapFlag;         /* 0x068 */
    UBYTE _m_0x6c[0x404];     /* 0x06c m_LightMarker[8] (C3DTile, 16-aligned at 0x70) */
    CPolyG4 m_Back[4];        /* 0x470 */
    UBYTE _m_0x5e0[0x1000];   /* 0x5e0 m_grid[64] */
    FOGDATA* m_pFog;          /* 0x15e0 */
    ZONEDATA* m_pZone;        /* 0x15e4 */
    LIGHTDATA m_Light;        /* 0x15e8 */
    LIGHTDATA m_MapLight;     /* 0x1608 */
    CSky* m_pSky;             /* 0x1628 */
    CMapLightUnit* m_LightUnit; /* 0x162c */
    UBYTE _m_0x1630[0x50];    /* 0x1630 m_gpPosDir */

public:
    CMap(CTask* p, UINT nRsrcGrpID);
    virtual ~CMap();
    void init(BOOL bUseMapUnit, BOOL bTexClampRepeat);
    virtual void main();
    virtual void disp();
    void resetFog();
    LIGHTDATA* getLightData() { return &m_Light; }
    LIGHTDATA* getMapLightData() { return &m_MapLight; }
    void getBBoxMax(FVECTOR& outMax);
    void getBBoxMin(FVECTOR& outMin);
    void setColorScale(FVECTOR& inColorScale);
    void setSkyColorScale(FVECTOR& inColorScale);
private:
    HANDLE createMapEffect_(const ZONEPARTS& inParts);
    void createEnvSE(const ZONEPARTS& inParts);
    void _exclusiveSetup(BOOL bUseMapUnit);
    INT _checkEffId(ZONEPARTS* parts, BOOL bUseMapUnit);
    void _createEffIdProc(ZONEPARTS* parts, INT nEType);
};
#else
// PARTIAL: added for System/Graphic/LightUnit (only the members it uses). The original derives from CTask;
// the base is left out (as padding) so that includers without System/Task.h (Game/MapUnitTable) don't get its
// .rodata string: cast CTaskMan::getSysTask()'s result.
class CMap {
private:
    UBYTE _pad[0x15E8];
    LIGHTDATA m_Light;    /* 0x15e8 */
    LIGHTDATA m_MapLight; /* 0x1608 */
    UBYTE _pad1628[0x58]; /* 0x1628 .. (size 0x1680) */ // PARTIAL: added for Game/GameSubField

public:
    CMap(class CTask* pParent, UINT nMapID); // PARTIAL: added for Game/GameSubField
    void init(INT nMode0, INT nMode1);
    LIGHTDATA* getLightData() { return &m_Light; }
    LIGHTDATA* getMapLightData() { return &m_MapLight; }
};

#endif /* GAME_MAP_CPP */

#endif // GAME_MAP_H
