// PARTIAL: added for Game/MapUnitTable
#ifndef GAME_MAP_H_STRUCTS
#define GAME_MAP_H_STRUCTS

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

    void set(/* parameters unknown */);
    void init0(/* parameters unknown */);
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

struct ALF_DATA {
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
    void set(const ALF_DATA* pData);
};

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
struct ZONEDATA { // 0x20
    SHORT partsnum;
    SHORT skynum;
    ZONEPARTS* parts;
    SHORT gridsizeX;
    SHORT zoneSizeX;
    SHORT gridsizeZ;
    SHORT zoneSizeZ;
    INT group_suu;
    INT** grouptable;
    INT chip_suu;
    INT __parts_suu;
};

#endif /* GAME_MAP_H_STRUCTS */

/* MATCH: with MAP_H_STRUCTS_ONLY defined, only the structs: the classes need System/Task.h, Gprim.h and Model.h,
 * whose strings the .rodata of Game/MapUnitTable and Game/Gimmick/GimmickDoor lacks */
#if !defined(GAME_MAP_H) && !defined(MAP_H_STRUCTS_ONLY)
#define GAME_MAP_H

#include "System/Task.h"
#include "System/Graphic/Gprim.h"
#include "System/Graphic/Model.h"

class CSky;
class CMapLightUnit;
class CTexAnm;
class CMapModel;

class CMapGrid : public CBBox {
public:
    INT flg; /* 0x3c */
};

class CMapModel : public CModel {
public:
    CMapModel() : CModel(NULL) { setTestFlag(0x500FD); }
    INT getGridID();
    void setGridID(INT nInID);
    INT m_nGridID; /* 0x120 */
    INT _pad[3];   /* PARTIAL: the original CModel is 16-byte aligned (size 0x130) */
};

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
    C3DTile m_LightMarker[8]; /* 0x070 */
    CPolyG4 m_Back[4];        /* 0x470 */
    CMapGrid m_grid[64];      /* 0x5e0 */
    FOGDATA* m_pFog;          /* 0x15e0 */
    ZONEDATA* m_pZone;        /* 0x15e4 */
    LIGHTDATA m_Light;        /* 0x15e8 */
    LIGHTDATA m_MapLight;     /* 0x1608 */
    CSky* m_pSky;             /* 0x1628 */
    CMapLightUnit* m_LightUnit; /* 0x162c */
    C3DLineStrip m_gpPosDir;  /* 0x1630 */

public:
    CMap(CTask* p, UINT nRsrcGrpID);
    virtual ~CMap();
    void init(BOOL bUseMapUnit, BOOL bTexClampRepeat);
    virtual void main();
    virtual void disp();
    INT getMapID() const { return m_nRsrcGrpID; }
    FOGDATA* getFogData() { return m_pFog; }
    void setFogData(FOGDATA* pFogData) { m_pFog = pFogData; }
    void resetFog();
    LIGHTDATA* getLightData() { return &m_Light; }
    LIGHTDATA* getMapLightData() const { return (LIGHTDATA*)&m_MapLight; }
    CMapModel* getModel() { return m_pModelTbl; }
    CMapModel* getModel(INT nIdx) {
        if (nIdx >= 0 && nIdx < m_nModelMax) return &m_pModelTbl[nIdx];
        return NULL;
    }
    INT getModelMax() { return m_nModelMax; }
    UINT getMapFlag() { return m_uiMapFlag; }
    void setMapFlag(UINT inOn, UINT inOff) { m_uiMapFlag = (m_uiMapFlag | inOn) & ~inOff; }
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

#endif /* GAME_MAP_H */
