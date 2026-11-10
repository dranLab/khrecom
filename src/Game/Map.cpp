

#include "khtypes.h"
#include "System/Task.h"
#include "System/Graphic/Gprim.h"
#include "System/Graphic/Model.h"
#include "System/Graphic/Camera.h"
#include "System/Graphic/LightUnit.h"
#include "System/Graphic/TexAnm.h"
#include "System/Graphic/TexCtrl.h"
#include "Game/Map.h"
#include "Game/MapUnitTable.h"
#include "Game/Game.h"
#include "Game/SceneEnv.h"
#include "Game/Collision/CollMapCamera.h"
#include "System/SJoint.h"
#include "System/Graphic/WorldEnv.h"
#include "Game/Debug/DebugWork.h"
#include "System/Math3D.h"
#include "System/RsrcData.h"
#include "System/Buf.h"
#include "System/Graphic/ShadowModel.h"
#include "System/Tsin.h"
#include "System/Graphic/Glare.h"
#include "Game/AtWater.h"
#include "Game/SceneInfo.h"
#include "System/Graphic/Effect/EffectMan.h"
#include "System/Graphic/Effect/effect.h"
#include "System/Graphic/Effect/effect_inl.h"
#include "include_asm.h"


#define SJ_FOREACH(T, pEntry, list) \
    for (T *pEntry = (list).getHead(), *__pn = pEntry ? pEntry->getNext() : NULL; pEntry; \
         pEntry = __pn, __pn = __pn ? __pn->getNext() : NULL)

class CMCSemiModel : public CSJointT<CMCSemiModel> { /* PARTIAL: stand-in for Game/Collision/CollMapCamera CMCSemiModel: the class sits in CollMapCamera.cpp; in the header CCMCSemiModelList's inline dtor is emitted in another unit (2026-11-07) (defined in CollMapCamera.cpp; ref: lost+found) */
public:
    FVECTOR center;
    FLOAT transparency;
    INT flag;
};
class CCMCSemiModelList : public CSJointListT<CMCSemiModel> {}; /* PARTIAL: stand-in for Game/Collision/CollMapCamera CCMCSemiModelList: owner not written */


void ApplyMatrix(FVECTOR& v0, const FMATRIX& m0, const FVECTOR& v1);


class CSky : public CTaskSecondary {
public:
    FVECTOR m_vColor;       /* 0x40 */
    CModelBG* m_pModel;     /* 0x50 */
    INT m_modelSuu;         /* 0x54 */
    FVECTOR* m_pRot;        /* 0x58 */
    FVECTOR* m_pRotShift;   /* 0x5c */
    CSky(CTask* pInParent, void* pAZF, ZONEDATA* pZone);
    virtual ~CSky();
    virtual void main();
    virtual void disp();
    void setColorScale(FVECTOR& inColorScale);
};

extern LIGHTDATA ALF_default;
extern "C" char D_00480B70[];
extern const FVECTOR ONE_VECTOR;
extern "C" char D_00480CE8[];
extern "C" char D_00480D08[];
















#include "System/RsrcDB.h"






extern MAPUNITTABLE g_DefaultGimmick;
void RotMatrixZ(FMATRIX& out, FMATRIX& in);
void setTransparency(const CCoord& crd, CModel& model);

FOGDATA FOG_default = {
    {{0, 0, 0, 0}}, 200.0f, 300.0f, 0.0f, 1,
    {{{0, 0x21, 0x1D, 0}}, {{0, 9, 9, 0x1C0}}, {{0, 0, 0, 0}}, {{0, 0, 0, 0}}, {{0, 0, 0, 0}}},
    255.0f, 0.0f, 0.0f, {0}, {0xFFFFFFFF, 0xFFFFFFFF}, 0.200000003f, 0.300000012f,
    {{-1, -1, -1, -1}}, {{-1, -1, -1, -1}}
};
static INFINITELIGHT map_inflight[4] = {
    {{{-822.740173f, 547.139526f, -646.249939f, 0.0f}}, {{0.0f, 0.0f, 0.0f, 0.0f}}},
    {{{822.740173f, 547.139526f, -646.249939f, 0.0f}}, {{0.0f, 0.0f, 0.0f, 0.0f}}},
    {{{-822.740173f, 547.139526f, 646.249939f, 0.0f}}, {{0.0f, 0.0f, 0.0f, 0.0f}}},
    {{{822.740173f, 547.139526f, 646.249939f, 0.0f}}, {{0.0f, 0.0f, 0.0f, 0.0f}}},
};
static AMBIENTLIGHT map_ambientlight = {{{0.0f, 0.0f, 0.0f, 0.0f}}};
LIGHTDATA ALF_map_default = {4, 0, 0, 0, map_inflight, NULL, NULL, &map_ambientlight};

void LIGHTDATA::set(const ALF_DATA* pData) {
    if (!pData) {
        *this = ALF_default;
    } else {
        infinite_suu = pData->infinite_suu;
        point_suu = pData->point_suu;
        spot_suu = pData->spot_suu;
        dum0 = pData->dum0;
        infinite = NULL;
        point = NULL;
        spot = NULL;
        ambient = NULL;
        void* d = (void*)(pData + 1);
        if (infinite_suu > 0) {
            infinite = (INFINITELIGHT*)d;
            d = (INFINITELIGHT*)d + infinite_suu;
        }
        if (point_suu > 0) {
            point = (POINTLIGHT*)d;
            d = (POINTLIGHT*)d + point_suu;
        }
        if (spot_suu > 0) {
            spot = (SPOTLIGHT*)d;
            d = (SPOTLIGHT*)d + spot_suu;
        }
        ambient = (AMBIENTLIGHT*)d;
    }
    TRACE("LIGHT infinite:%d point:%d spot:%d ??:%d\n", infinite_suu, point_suu, spot_suu, dum0);
}
void RotMatrixZ(FMATRIX& out, FMATRIX& in) {
    FMATRIX tmp = in;
    tmp.v[0] *= -1.0f;



    tmp.v[1] *= -1.0f;



    out = tmp;
}
INT CMap::_checkEffId(ZONEPARTS* parts, BOOL bUseMapUnit) {
    MAPUNITID id;
    if (!parts->effectId || !bUseMapUnit) return -1;
    id.ui = parts->effectId;
    if (id.c[0] == 'E') return 1;
    if (id.c[0] == 'S') return 2;
    if (id.c[0] != 'P') return 0;
    return (id.c[1] != '1') ? -1 : 3;
}
inline CTask* MAPUNITTABLE::create(CTask* inParent, ZONEPARTS* inParts) {
    if (inParts->effectId != id.ui) return NULL;
    return maptable(inParent, *inParts);
}
void CMap::_createEffIdProc(ZONEPARTS* parts, INT nEType) {
    if (!parts->effectId) return;
    if (nEType < 0) return;
    switch (nEType) {
    case 1:
        createMapEffect_(*parts);
        break;
    case 2:
        createEnvSE(*parts);
        break;
    case 3:
        break;
    case 0:
    default: {
        int flg = 0;
        MAPUNITTABLE* t;
        MAPUNITTABLE defaultTbl = g_DefaultGimmick;
        while ((t = mapUnitTableSearch(parts)) != NULL) {
            if (t->create(getSysTask(t->kind), parts)) {
                flg = 1;
                break;
            }
        }
        if (!flg) {
            char szEffIDName[5];
            defaultTbl.id.ui = parts->effectId;
            strncpy(szEffIDName, defaultTbl.id.c, 4);
            szEffIDName[4] = 0;
            TRACE("\225\317\202\310\203G\203t\203F\203N\203g\202h\202c\202\305\202\267 => %x (%s)\n", parts->effectId, szEffIDName);
            if (!defaultTbl.create(getSysTask(defaultTbl.kind), parts)) TRACE("\201\252\201\252\201\252\225\317\202\310\203G\203t\203F\203N\203g\202h\202c\202\305\201A\215X\202\311\203f\203t\203H\203\213\203g\211\274\224z\222u\202\340\202\305\202\253\202\334\202\271\202\361\n");
        }
    }
    }
}
CMap::CMap(CTask* p, UINT nRsrcGrpID)
    : CTask(p, 0x4000), m_nRsrcGrpID(nRsrcGrpID), m_pTexAnm(NULL), m_nTexAnmNum(0), m_pModelTbl(NULL), m_nModelMax(0),
      m_uiMapFlag(0), m_pZone(NULL), m_pSky(NULL), m_LightUnit(NULL) {
    declareSysTask(2);
    enableCoordCalc(FALSE);
    TRACE("CMap::CMap() nRsrcGrpID %d\n", nRsrcGrpID);
    m_vColor.setAll(1.0f);
    CRsrcData* rsrc = nRsrcGrpID ? CRsrcDB::getFirstRsrc(9, nRsrcGrpID, TRUE) : NULL;
    if (!rsrc) {
        rsrc = CRsrcDB::getFirstRsrc_Scene(9);
        if (rsrc) m_nRsrcGrpID = rsrc->getRsrcGroupID() & 0x0FFFFFFF;
    }
    if (rsrc) {
        BYTE* pData = rsrc->getRsrcData();
        m_pZone = pData ? *(ZONEDATA**)pData : NULL;
    } else {
        TRACE("CMap::CMap() : \203]\201[\203\223\203f\201[\203^\202\252\214\251\202\302\202\251\202\350\202\334\202\271\202\361\n");
    }
    TRACE("CMap::CMap() m_nRsrcGrpID %d\n", m_nRsrcGrpID);
    m_pFog = (FOGDATA*)CRsrcDB::getFirstData(0xD, m_nRsrcGrpID, TRUE);
    if (!m_pFog) {
        m_pFog = &FOG_default;
    } else if (m_pFog->dispfar < 1.0f || 65536.0f < m_pFog->dispfar) {
        m_pFog->dispfar = m_pFog->fogfar;
        m_pFog->fogmax = 255.0f;
        m_pFog->fogmin = 0.0f;
    }
    resetFog();
    if (*(UINT*)&m_pFog->sdwColor == 0xFFFFFFFF) m_pFog->sdwColor = 0.2f;
    if (*(UINT*)&m_pFog->sdwAlpha == 0xFFFFFFFF) m_pFog->sdwAlpha = 0.3f;
    CShadowModelMan::setColorDensity(m_pFog->sdwColor);
    CShadowModelMan::setAlphaDensity(m_pFog->sdwAlpha);
    m_gpPosDir.setCoord(NULL);
    m_gpPosDir.setLayerFG(0);
    G_STRIP Line[2];
    Line[0].P.init0();
    Line[1].Term = 1;
    Line[0].Bone = 0;
    Line[0].Term = 0;
    Line[1].Bone = 0;
    Line[0].C.set(0xFF, 0, 0, 0x80);
    Line[1].C.set(0xFF, 0, 0, 0x80);
    Line[1].P.set(1.0f, 0.0f, 0.0f, 1.0f);
    m_gpPosDir.m_vVERT.push_back(Line[0]);
    m_gpPosDir.m_vVERT.push_back(Line[1]);
    Line[0].C.set(0, 0xFF, 0, 0x80);
    Line[1].C.set(0, 0xFF, 0, 0x80);
    Line[1].P.set(0.0f, 1.0f, 0.0f, 1.0f);
    m_gpPosDir.m_vVERT.push_back(Line[0]);
    m_gpPosDir.m_vVERT.push_back(Line[1]);
    Line[0].C.set(0, 0, 0xFF, 0x80);
    Line[1].C.set(0, 0, 0xFF, 0x80);
    Line[1].P.set(0.0f, 0.0f, 1.0f, 1.0f);
    m_gpPosDir.m_vVERT.push_back(Line[0]);
    m_gpPosDir.m_vVERT.push_back(Line[1]);
}
CMap::~CMap() {
    CTexCtrl::delFixTexAnm(m_pTexAnm, m_nTexAnmNum);
    delete[] m_pModelTbl;
    m_pModelTbl = NULL;
    delete[] m_LightUnit;
    m_LightUnit = NULL;
    delete[] m_pTexAnm;
    m_pTexAnm = NULL;
    CGame::stopConstEnvSE(-1);
}
static inline void _unitMatrixVu0(FMATRIX& m) {
    asm volatile("vmr32.xyzw $vf1, $vf0\n"
                 "sqc2 $vf0, 0x30(%0)\n"
                 "vmr32.xyzw $vf2, $vf1\n"
                 "sqc2 $vf1, 0x20(%0)\n"
                 "vmr32.xyzw $vf3, $vf2\n"
                 "sqc2 $vf2, 0x10(%0)\n"
                 "sqc2 $vf3, 0x0(%0)\n"
                 :
                 : "r"(&m)
                 : "memory");
}
void MulMatrix(FMATRIX& m0, const FMATRIX& m1, const FMATRIX& m2);









static inline void* _getAZFTbl(void* pInAZF, INT nIndex) {
    void* pTbl;
    if (!pInAZF) pTbl = NULL;
    else pTbl = ((void**)pInAZF)[nIndex];
    return pTbl;
}
static inline void _rotMatrixZ(FMATRIX& m, const TDEG& a) {
    FMATRIX rz;
    _unitMatrixVu0(rz);
    rz.m[0][0] = rz.m[1][1] = tcos(a);
    FLOAT s = tsin(a);
    rz.m[0][1] = s;
    rz.m[1][0] = -s;
    m = MulMatrix(m, rz);
}
static inline void _rotMatrixY(FMATRIX& m, const TDEG& a) {
    FMATRIX ry;
    _unitMatrixVu0(ry);
    ry.m[2][2] = ry.m[0][0] = tcos(a);
    FLOAT s = tsin(a);
    ry.m[2][0] = s;
    ry.m[0][2] = -s;
    m = MulMatrix(m, ry);
}
static inline void _rotMatrixX(FMATRIX& m, const TDEG& a) {
    FMATRIX rx;
    _unitMatrixVu0(rx);
    rx.m[1][1] = rx.m[2][2] = tcos(a);
    FLOAT s = tsin(a);
    rx.m[1][2] = s;
    rx.m[2][1] = -s;
    m = MulMatrix(m, rx);
}
/* Parked: 82 of 1190 lines, 8 bytes short (2026-11-09) - only xmin/xmax/zmin/zmax's registers and the division block's
   schedule differ: the original keeps xmax's quotient apart (mflo v1; move s6,v1: cse kept the variable canonical, so
   xmax dies twice and goes to global-alloc, s6 after the fptodp temps s3-s5), ours lets cse replace xmax by the
   quotient (local-alloc ties them: xmin s2, xmax s1, zmax s3). make_regs_eqv keeps the variable only when it is
   referenced outside the cse path (from the qq loop's end to the else label), not visible in the original's code;
   the 8 bytes are that copy and its schedule.
   Worked: ApplyMatrix through Coord.h's by-value inline (line markers Coord.h:634-635; 87 -> 82), Coord.h's by-value
   MulMatrix (642-643), _getAZFTbl inline, angles by const TDEG&, vec in its own scope, min/max init before v[], zero-store
   order. Tried: division block in line order / per-coordinate / one-line forms, xmax if/ternary/compare forms, int
   declarations before the floats, v[] in its own block; permuter 15 min on this draft (11.9k, no gain); earlier
   for/do/while/goto loop shapes, 10 min permuter. */
#if 0
void CMap::init(BOOL bUseMapUnit, BOOL bTexClampRepeat) {
    INT nModelCount = 0;
    TRACE("---------------- CMap::init() start ----------------\n");
    m_nModelMax = 0;
    if (m_pZone) {
        nModelCount = m_pZone->partsnum - m_pZone->skynum;
        INT partsIdx = m_pZone->skynum;
        for (INT i = 0; i < nModelCount; i++, partsIdx++) {
            if (_checkEffId(&m_pZone->parts[partsIdx], bUseMapUnit) < 0) m_nModelMax++;
        }
    }
    void* pAZF = NULL;
    addCoord(m_nModelMax + 1);
    CRsrcAZF* pRsrcAzf = (CRsrcAZF*)CRsrcDB::getFirstRsrc(9, m_nRsrcGrpID, TRUE);
    if (pRsrcAzf) {
        pAZF = pRsrcAzf->getRsrcData();
        if (pRsrcAzf->isTexIncomplete()) pRsrcAzf->reSetupTex();
    }
    ALF_DATA* pALF = (ALF_DATA*)CRsrcDB::getFirstData(0x16, m_nRsrcGrpID, TRUE);
    m_Light.set(pALF);
    pALF = (ALF_DATA*)CRsrcDB::getFirstData(0x29, m_nRsrcGrpID, TRUE);
    if (pALF) m_MapLight.set(pALF);
    else m_MapLight = ALF_map_default;
    {
        CCoord& crd = Crd(0);
        _unitMatrixVu0(crd.m_Local);
        crd.recalcWorldMatrix();
        crd.m_Flag = 1;
    }
    if (!pAZF || !m_pZone) {
        if (!pAZF) TRACE("CMap::init() AZF\202\252\214\251\202\302\202\251\202\350\202\334\202\271\202\361\n");
        if (!m_pZone) TRACE("CMap::init() m_pZone\202\252\202\240\202\350\202\334\202\271\202\361\n");
        return;
    }
    TEXANM_DATA* pTXAData = CRsrcDB::getRsrcTXA(m_nRsrcGrpID);
    if (pTXAData) {
        m_nTexAnmNum = pTXAData->getDataNum();
        if (m_nTexAnmNum > 0) {
            m_pTexAnm = new (__FILE__ "(576)") CTexAnm[m_nTexAnmNum];
            for (INT i = 0; i < m_nTexAnmNum; i++) {
                m_pTexAnm[i].setup(*pTXAData, i);
                m_pTexAnm[i].setSeq(0);
            }
        }
    }
    _exclusiveSetup(bUseMapUnit);
    m_pModelTbl = new (__FILE__ "(596)") CMapModel[m_nModelMax];
    if (bTexClampRepeat) {
        for (INT i = 0; i < m_nModelMax; i++) m_pModelTbl[i].setClampRepeat(TRUE);
    }
    {
        MODELDATA* pTbl = (MODELDATA*)_getAZFTbl(pAZF, m_pZone->parts[m_pZone->skynum].modelIndex + 1);
        if (pTbl->getRawMode() & 4) {
            m_LightUnit = new (__FILE__ "(606)") CMapLightUnit[m_nModelMax];
        }
    }
    if (m_pZone->skynum) {
        m_pSky = new (__FILE__ "(611)") CSky(this, pAZF, m_pZone);
    }
    INT iGridSizeX = 40;
    INT iGridSizeZ = 40;
    TRACE("CMap::init()  All:%d Model:%d/%d Sky:%d Group:%d Chip:%d\n", m_pZone->partsnum, m_nModelMax, nModelCount,
          m_pZone->skynum, m_pZone->group_suu, m_pZone->chip_suu);
    if (m_pZone->zoneSizeX > 0) {
        iGridSizeX = (m_pZone->zoneSizeX * m_pZone->gridsizeX) >> 3;
        iGridSizeZ = (m_pZone->zoneSizeZ * m_pZone->gridsizeZ) >> 3;
        if (iGridSizeX > 40) iGridSizeX = 40;
        if (iGridSizeZ > 40) iGridSizeZ = 40;
    } else {
        TRACE("zoneSizeX == 0\n");
    }
    FLOAT fGridSizeX = iGridSizeX;
    FLOAT fGridSizeZ = iGridSizeZ;
    TRACE("CMap::init()  gX = %d gZ = %d\n", iGridSizeX, iGridSizeZ);
    for (INT z = 0; z < 8; z++) {
        for (INT x = 0; x < 8; x++) {
            CMapGrid& grid = m_grid[z * 8 + x];
            grid.m_bbox.y[0] = 16000000.0f;
            grid.m_bbox.y[1] = -16000000.0f;
            grid.m_bbox.z[0] = fGridSizeZ * (z - 4);
            grid.m_bbox.z[1] = grid.m_bbox.z[0] + fGridSizeZ;
            grid.m_bbox.x[0] = fGridSizeX * (x - 4);
            grid.m_bbox.x[1] = grid.m_bbox.x[0] + fGridSizeX;
            grid.m_pCrd = &Crd(0);
            grid.flg = -1;
        }
    }
    INT i;
    INT nDispModel;
    nDispModel = 0;
    i = 0;
    INT nPartsMax = m_pZone->partsnum;
    for (INT partsIdx = m_pZone->skynum; partsIdx < nPartsMax; partsIdx++) {
        ZONEPARTS* parts = &m_pZone->parts[partsIdx];
        parts->crdIndex = -1;
        INT nEType = _checkEffId(parts, bUseMapUnit);
        if (nEType >= 0) {
            _createEffIdProc(parts, nEType);
            continue;
        }
        if (parts->modelIndex < 0) {
            TRACE("\203]\201[\203\223\203t\203@\203C\203\213\222\206\202\311\223o\230^\202\265\202\304\202\242\202\310\202\242\203\202\203f\203\213\202\326\202\314\216Q\217\306\202\252\202\240\202\350\202\334\202\267\n");
            continue;
        }
        MODELDATA* pTbl = (MODELDATA*)_getAZFTbl(pAZF, m_pZone->parts[partsIdx].modelIndex + 1);
        if (!pTbl) {
            TRACE("%d:\203\202\203f\203\213\203f\201[\203^\202\252\214\251\202\302\202\251\202\350\202\334\202\271\202\361 %d\n", partsIdx, m_pZone->parts[partsIdx].modelIndex);
            AssertApp(__FILE__, 703, "d", 0);
        }
        nDispModel++;
        if (!(i < m_nModelMax)) AssertApp(__FILE__, 706, "i<m_nModelMax", 0);
        parts->crdIndex = i;
        CMapModel& model = m_pModelTbl[i];
        model.m_nGridID = -2;
        CCoord& crd = Crd(i + 1);
        {
        FVECTOR vec;
        vec._q = 0;
        _unitMatrixVu0(crd.m_Local);
        if (parts->rot.z) _rotMatrixZ(crd.m_Local, parts->rot.z);
        if (parts->rot.y) _rotMatrixY(crd.m_Local, parts->rot.y);
        if (parts->rot.x) _rotMatrixX(crd.m_Local, parts->rot.x);
        crd.m_Local.v[0] *= parts->scale[0];
        crd.m_Local.v[1] *= parts->scale[1];
        crd.m_Local.v[2] *= parts->scale[2];
        crd.m_Local.v[3].x = parts->trans[0];
        crd.m_Local.v[3].y = parts->trans[1];
        crd.m_Local.v[3].z = parts->trans[2];
        crd.m_Local.v[3].w = 1.0f;
        crd.m_Local.m[0][3] = 0.0f;
        crd.m_Local.m[1][3] = 0.0f;
        crd.m_Local.m[2][3] = 0.0f;
        }
        Crd(i + 1).m_pParent = &Crd(0);
        Crd(i + 1).recalcWorldMatrix();
        Crd(i + 1).m_Flag = 1;
        model.assign(this, pTbl, i + 1);
        model.linkTexAnm(m_pTexAnm, m_nTexAnmNum);
        BBOX* bb = model.getBBox(0);
        FLOAT xminf = 16000000.0f;
        FLOAT xmaxf = -16000000.0f;
        FLOAT zminf = xminf;
        FLOAT zmaxf = xmaxf;
        FVECTOR v[4];
        for (int zz = 0; zz < 2; zz++) {
            for (int xx = 0; xx < 2; xx++) {
                v[zz * 2 + xx].x = bb->x[xx];
                v[zz * 2 + xx].y = 0.0f;
                v[zz * 2 + xx].z = bb->z[zz];
                v[zz * 2 + xx].w = 1.0f;
            }
        }
        for (int qq = 0; qq < 4; qq++) {
            FVECTOR p = ApplyMatrix(Crd(i + 1).m_Local, v[qq]);
            if (p.x < xminf) xminf = p.x;
            if (xmaxf < p.x) xmaxf = p.x;
            if (p.z < zminf) zminf = p.z;
            if (zmaxf < p.z) zmaxf = p.z;
        }
        int xmin = (int)xminf;
        int xmax = (int)xmaxf;
        int zmin = (int)zminf;
        int zmax = (int)zmaxf;
        TRACE("%d: Z(%f %f)->(%d %d) X(%f %f)->(%d %d)\n", i, zmaxf, zminf, zmax, zmin, xmaxf, xminf, xmax, xmin);
        xmin += iGridSizeX * 4;
        xmax += iGridSizeX * 4;
        zmin += iGridSizeZ * 4;
        zmax += iGridSizeX * 4;
        if (zmin < 0) zmin -= iGridSizeZ;
        zmin /= iGridSizeZ;
        if (xmin < 0) xmin -= iGridSizeX;
        xmin /= iGridSizeX;
        if (xmax < 0) xmax -= iGridSizeX;
        xmax /= iGridSizeX;
        if (zmax < 0) zmax -= iGridSizeZ;
        zmax /= iGridSizeX;
        INT idx = zmin * 8 + xmin;
        TRACE("%d: Z(%d %d) X(%d %d) = %d\n", i, zmax, zmin, xmax, xmin, idx);
        if (xmax - xmin < 2 && zmax - zmin < 2 && xmin >= 0 && xmin < 8 && zmin >= 0 && zmin < 8) {
            FLOAT ymin = bb->y[0] + Crd(i + 1).m_Local.v[3].y;
            FLOAT ymax = bb->y[1] + Crd(i + 1).m_Local.v[3].y;
            model.m_nGridID = idx;
            CMapGrid& grid = m_grid[idx];
            if (ymin < grid.m_bbox.y[0]) grid.m_bbox.y[0] = ymin;
            if (grid.m_bbox.y[1] < ymax) grid.m_bbox.y[1] = ymax;
            if (xminf < grid.m_bbox.x[0]) grid.m_bbox.x[0] = xminf;
            if (grid.m_bbox.x[1] < xmaxf) grid.m_bbox.x[1] = xmaxf;
            if (zminf < grid.m_bbox.z[0]) grid.m_bbox.z[0] = zminf;
            if (grid.m_bbox.z[1] < zmaxf) grid.m_bbox.z[1] = zmaxf;
        } else {
            model.m_nGridID = -1;
        }
        if (m_LightUnit) {
            CMapLightUnit& lu = m_LightUnit[i];
            lu.setAnchor(&Crd(i + 1), *pTbl->m_pBBox, 0);
            m_LightUnit[i].init();
            model.setLightUnit(&m_LightUnit[i]);
        }
        i++;
    }
    TRACE("CMap::init()  DispModel:%d\n", nDispModel);
    CTexCtrl::addFixTexAnm(m_pTexAnm, m_nTexAnmNum);
    if (m_pFog->skynum) {
        for (i = 0; i < m_pFog->skynum; i++) {
            m_Back[i].setLayer(2, 3);
            m_Back[i].setSemiTrans((ALPHA)1);
        }
    }
    TRACE("---------------- CMap::init() end ----------------\n");
}
#else
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Game/Map", init__4CMapii);
#endif
/* MATCH: CMap::init's .rodata while it is INCLUDE_ASM */
__asm__(".rdata\n"
        ".align 3\n"
        ".asciz \"---------------- CMap::init() start ----------------\\n\"\n"
        ".align 2\n"
        ".word 0x70614D43\n"
        ".word 0x6E693A3A\n"
        ".word 0x29287469\n"
        ".word 0x465A4120\n"
        ".word 0xA98CAA82\n"
        ".word 0xA982C282\n"
        ".word 0xDC82E882\n"
        ".word 0xF182B982\n"
        ".word 0x0000000A\n"
        ".word 0x00000000\n"
        ".word 0x70614D43\n"
        ".word 0x6E693A3A\n"
        ".word 0x29287469\n"
        ".word 0x705F6D20\n"
        ".word 0x656E6F5A\n"
        ".word 0xA082AA82\n"
        ".word 0xDC82E882\n"
        ".word 0xF182B982\n"
        ".word 0x0000000A\n"
        ".word 0x00000000\n"
        ".align 3\n"
        ".asciz \"c:/KHCOM/MainApp/Game/Map.cpp(576)\"\n"
        ".align 2\n"
        ".asciz \"\"\n"
        ".align 2\n"
        ".align 3\n"
        ".asciz \"c:/KHCOM/MainApp/Game/Map.cpp(596)\"\n"
        ".align 2\n"
        ".asciz \"\"\n"
        ".align 2\n"
        ".align 3\n"
        ".asciz \"c:/KHCOM/MainApp/Game/Map.cpp(606)\"\n"
        ".align 2\n"
        ".asciz \"\"\n"
        ".align 2\n"
        ".align 3\n"
        ".asciz \"c:/KHCOM/MainApp/Game/Map.cpp(611)\"\n"
        ".align 2\n"
        ".asciz \"\"\n"
        ".align 2\n"
        ".align 3\n"
        ".asciz \"CMap::init()  All:%d Model:%d/%d Sky:%d Group:%d Chip:%d\\n\"\n"
        ".align 2\n"
        ".asciz \"\"\n"
        ".align 2\n"
        ".align 3\n"
        ".asciz \"zoneSizeX == 0\\n\"\n"
        ".align 2\n"
        ".align 3\n"
        ".asciz \"CMap::init()  gX = %d gZ = %d\\n\"\n"
        ".align 2\n"
        ".word 0x5B815D83\n"
        ".word 0x74839383\n"
        ".word 0x43834083\n"
        ".word 0x86928B83\n"
        ".word 0x6F93C982\n"
        ".word 0xB5825E98\n"
        ".word 0xA282C482\n"
        ".word 0xA282C882\n"
        ".word 0x66838283\n"
        ".word 0xD6828B83\n"
        ".word 0x518ECC82\n"
        ".word 0xAA82C68F\n"
        ".word 0xE882A082\n"
        ".word 0xB782DC82\n"
        ".word 0x0000000A\n"
        ".word 0x00000000\n"
        ".word 0x833A6425\n"
        ".word 0x83668382\n"
        ".word 0x8166838B\n"
        ".word 0x825E835B\n"
        ".word 0x82A98CAA\n"
        ".word 0x82A982C2\n"
        ".word 0x82DC82E8\n"
        ".word 0x20F182B9\n"
        ".word 0x000A6425\n"
        ".word 0x00000000\n"
        ".align 3\n"
        ".asciz \"c:/KHCOM/MainApp/Game/Map.cpp\"\n"
        ".align 2\n"
        ".align 3\n"
        ".asciz \"d\"\n"
        ".align 2\n"
        ".asciz \"\"\n"
        ".align 2\n"
        ".align 3\n"
        ".asciz \"i<m_nModelMax\"\n"
        ".align 2\n"
        ".align 3\n"
        ".asciz \"%d: Z(%f %f)->(%d %d) X(%f %f)->(%d %d)\\n\"\n"
        ".align 2\n"
        ".asciz \"\"\n"
        ".align 2\n"
        ".align 3\n"
        ".asciz \"%d: Z(%d %d) X(%d %d) = %d\\n\"\n"
        ".align 2\n"
        ".asciz \"\"\n"
        ".align 2\n"
        ".align 3\n"
        ".asciz \"CMap::init()  DispModel:%d\\n\"\n"
        ".align 2\n"
        ".asciz \"\"\n"
        ".align 2\n"
        ".align 3\n"
        ".asciz \"---------------- CMap::init() end ----------------\\n\"\n"
        ".align 2\n"
        ".asciz \"\"\n"
        ".align 2\n"
        ".align 3\n"
        ".text\n");
/* MATCH: CMap::init allocates CMapModel[] and grows CVector<G_STRIP> (vtable, weak copies); stripped section */
__asm__(".section .sdata\n");
__attribute__((section(".sdata"))) void _mapInitPlaceholder(G_STRIP_VECTOR& v, const G_STRIP& x) {
    new (D_00480B70) CMapModel[1];
    v.push_back(x);
}
void CMap::_exclusiveSetup(BOOL bUseMapUnit) {
    CGlare::create(this);
    CSceneEnv::setColor((ObjType::eObjBit)0xFFFFFFFF, 0.0f, ONE_VECTOR, NULL, 3);
    switch (CSceneInfo::getCurrentWorldID(TRUE)) {
    case 0:
        break;
    case 11:
        CGlare::sysOn(1.0f);
        break;
    case 2:
        if (m_nRsrcGrpID != 0x7F35A) break;
    case 7:
        CAtWater::create(this);
        break;
    case 12:
        if (CSceneInfo::getCurrentSceneNum() == 30) {
            CGlare::sysOn(1.0f);
            CGlare::checkZ(FALSE);
        }
        break;
    case 13:
        if (CSceneInfo::getCurrentSceneNum() == 90) {
            CGlare::sysOn(1.0f);
        } else switch (m_nRsrcGrpID) {
        case 630003:
        case 630004:
        case 630005:
        case 630006:
        case 630007:
        case 630008:
        case 630009:
        case 630013:
        case 630019:
        case 630024:
        case 630025:
            CGlare::sysOn(1.25f);
            CGlare::checkZ(FALSE);
            break;
        }
        switch (m_nRsrcGrpID) {
        case 630001:
        case 630005:
        case 630006:
        case 630007:
        case 630008:
        case 630010:
        case 630011:
        case 630012:
        case 630014:
        case 630015:
        case 630016:
        case 630017:
        case 630018:
        case 630020:
        case 630021:
        case 630022:
        case 630023:
        case 630024:
        case 631001: {
            FVECTOR vColor = {{0.82f, 0.82f, 0.82f, 1.0f}};
            CSceneEnv::setColor((ObjType::eObjBit)0xF88, 0.0f, vColor, NULL, 3);
            break;
        }
        }
        break;
    }
    if (bUseMapUnit) {
        switch (m_nRsrcGrpID) {
        case 0x8B6AA: {
            CEffect* pEff = CEffectMan::getEffect(CEffectMan::setEffect("rec_bg_E062", 0, (ObjType::eObjType)0xB, (const FVECTOR*)NULL, (const FVECTOR*)NULL, 0.0f));
            if (pEff) pEff->setDrawFirst(0);
            break;
        }
        case 0x975FE:
        case 0x979FA:
        case 0x97A5F:
        case 0x97E17: {
            char* pszMapEff12 = "rec_bg_E111";
            int priTbl[8] = {0, 1, 2, 3, 4, 5, 6, 7};
            for (int i = 0; i < 8; i++) {
                CEffect* pEff = CEffectMan::getEffect(CEffectMan::setEffect(pszMapEff12, i, (ObjType::eObjType)0xB, (const FVECTOR*)NULL, (const FVECTOR*)NULL, 0.0f));
                if (pEff) {
                    pEff->setDrawFirst(priTbl[i]);
                    if (m_nRsrcGrpID == 0x979FA) {
                        pEff->setScale(2.0f, 2.0f, 2.0f);
                    }
                }
            }
            break;
        }
        }
    }
}
void CMap::resetFog() {
    CWorldEnv::setFogParam(m_pFog->fognear, m_pFog->fogfar, m_pFog->fogmax, m_pFog->fogmin);
    CWorldEnv::setDispFar(m_pFog->dispfar);
    CWorldEnv::setFogColor(m_pFog->color.x, m_pFog->color.y, m_pFog->color.z);
    CWorldEnv::setScreenZ(m_pFog->screenz);
    CWorldEnv::setDefScreenZ(m_pFog->screenz);
    DebugWork::g_dw.nakamich.fCamVAngle = ProjectionToViewAngle(m_pFog->screenz);
    INT max = m_pFog->skynum;
    if (max > 4) {
        TRACE("FOG\203f\201[\203^\202\252\211\363\202\352\202\304\202\351\202\251\202\340\202\346\n");
        max = 4;
    }
    for (INT i = 0; i < max; i++) {
        m_Back[i].m_C0.set(m_pFog->sky[i + 1].x, m_pFog->sky[i + 1].y, m_pFog->sky[i + 1].z, 0);
        m_Back[i].m_C1.set(m_pFog->sky[i + 1].x, m_pFog->sky[i + 1].y, m_pFog->sky[i + 1].z, 0);
        m_Back[i].m_C2.set(m_pFog->sky[i].x, m_pFog->sky[i].y, m_pFog->sky[i].z, 0x80);
        m_Back[i].m_C3.set(m_pFog->sky[i].x, m_pFog->sky[i].y, m_pFog->sky[i].z, 0x80);
    }
}
#define COLLUTIL_H_PATH "c:/KHCOM/MainApp/Game/Collision/CollUtil.h" /* MATCH: __FILE__ as in the original .rodata */
#include "Game/Collision/CollUtil.h" /* MATCH: included here: its strings follow resetFog's */
#include "System/HeapMan.h"
void CMap::main() {
    if (CSceneEnv::getStopTypeBit() & 0x800) return;
    if (!m_pTexAnm) return;
    for (INT i = 0; i < m_nTexAnmNum; i++) {
        m_pTexAnm[i].update(getFrameRate() * 0.020000001f);
    }
}
void setTransparency(const CCoord& crd, CModel& model) {
    if (!CCollMapCamera::m_pSemiModelList) return;
    FLOAT fAlpha = model.getAlphaScale();
    FVECTOR ctr = crd.m_World.getTrans();
    SJ_FOREACH(CMCSemiModel, pEntry, *CCollMapCamera::m_pSemiModelList) {
        float dx = pEntry->center.x - ctr.x;
        if (dx < 0.0f) dx = -dx;
        float dz = pEntry->center.z - ctr.z;
        if (dz < 0.0f) dz = -dz;
        if (dx <= 0.1f && dz <= 0.1f) {
            FLOAT t = pEntry->transparency * 0.5f;
            fAlpha *= t;
            break;
        }
    }
    model.setAlphaScale(fAlpha);
}
void setTransparency(CModel& model) {
    setTransparency(model.getCrd(0), model);
}
/* Parked: 709 of 1308 lines, 44 bytes short - structure as the original (debug clamp, shader modes, group selection by
   player position, pipelined kick/resultCheckBBox display via group table or grid, sky polygons). The 44 bytes: the
   original keeps two copies of the "flush the last model" block (group and grid path; s1/s2 swapped), ours gets the same
   registers in both and jump2 cross-jumps them into one; the rest is register allocation. */
#if 0
void MulVector(FVECTOR& v0, const FVECTOR& v1, const FVECTOR& v2);
INT checkBBox(const FMATRIX& mWorldMatrix, const BBOX* bbox);
INT resultCheckBBox(CCoord& crd, const BBOX* bbox);
void ITOF0Vector(FVECTOR& v0, const IVECTOR& v1);
void FTOI0Vector(IVECTOR& v0, const FVECTOR& v1);
static inline void _kickCheckBBox(FMATRIX* m, const BBOX* bbox) {
    if (!(bbox->x[1] < bbox->x[0])) {
        FVECTOR* v = (FVECTOR*)0x70000000;
        v[0].x = bbox->x[0];
        v[0].y = bbox->y[0];
        v[0].z = bbox->z[0];
        v[1].x = bbox->x[1];
        v[1].y = bbox->y[1];
        v[1].z = bbox->z[1];
        asm volatile("lqc2 $vf20, 0(%0)" : : "r"(v));
        asm volatile("lqc2 $vf27, 0x10(%0)" : : "r"(v));
        asm volatile("lqc2 $vf21, 0x0(%0)\n"
                     "lqc2 $vf22, 0x10(%0)\n"
                     "lqc2 $vf23, 0x20(%0)\n"
                     "lqc2 $vf24, 0x30(%0)" : : "r"(m));
        asm volatile("vcallms 0x20");
    }
}
void CMap::disp() {
    INT group_no;
    FLOAT fR;
    CMapModel* pModel;
    INT nModel;
    FVECTOR vColor;
    FVECTOR* pSceneColor;
    if (!m_pZone || !m_pModelTbl) return;
    if (DebugWork::g_dw.sys.m_iMapDisp < -1) DebugWork::g_dw.sys.m_iMapDisp = m_nModelMax;
    else if (m_nModelMax < DebugWork::g_dw.sys.m_iMapDisp) DebugWork::g_dw.sys.m_iMapDisp = -1;
    if (DebugWork::g_dw.sys.m_iMapDisp < 0) return;
    if (DebugWork::g_dw.sys.m_iMapDisp > 0) {
        m_gpPosDir.setCoord(&Crd(DebugWork::g_dw.sys.m_iMapDisp));
        m_gpPosDir.disp();
    }
    {
    CMapModel* pModel = m_pModelTbl;
    INT nModel = m_nModelMax;
    if (DebugWork::g_dw.sys.m_iMapBack == 0) {
        for (INT i = 0; i < nModel; i++, pModel++) pModel->defaultShaderOption();
    } else if (DebugWork::g_dw.sys.m_iMapBack == 1) {
        for (INT i = 0; i < nModel; i++, pModel++) pModel->setShaderOption(2);
    } else if (DebugWork::g_dw.sys.m_iMapBack == 2) {
        for (INT i = 0; i < nModel; i++, pModel++) pModel->resetShaderOption(2);
    }
    }
    if (m_uiMapFlag & 1) return;
    group_no = -1;
    fR = getFrameRateF();
    if (CSceneEnv::getStopTypeBit() & 0x800) fR = 0.0f;
    pModel = m_pModelTbl;
    nModel = m_nModelMax;
    pSceneColor = CSceneEnv::getTypeColor((ObjType::eObjType)0xB);
    if (pSceneColor) MulVector(vColor, m_vColor, *pSceneColor);
    else vColor = m_vColor;
    for (INT i = 0; i < nModel; i++, pModel++) {
        pModel->setColorScale(vColor);
        pModel->dispUpdate(fR);
    }
    if (m_pZone->group_suu > 0) {
        CTask* pl = getSysTask(3);
        if (pl) {
            FVECTOR vec = pl->Crd(0).m_World.v[3];
            for (int i = 0; i < m_pZone->chip_suu; i++) {
                ZONEPARTS* zd = &m_pZone->parts[i];
                if (zd->groupNo < 0) continue;
                if (zd->crdIndex < 0) continue;
                FMATRIX* m = &Crd(zd->crdIndex + 1).m_World;
                BBOX* bb = m_pModelTbl[zd->crdIndex].getBBox(0);
                FVECTOR lVec;
                FMATRIX iMat;
                sceVu0InversMatrix(iMat, *m);
                sceVu0ApplyMatrix(lVec, iMat, vec);
                if (bb->x[0] <= lVec.x && lVec.x < bb->x[1] && bb->z[0] <= lVec.z && lVec.z < bb->z[1]) {
                    group_no = zd->groupNo;
                    break;
                }
            }
        }
    }
    if (group_no >= 0) {
        INT ii = -1;
        for (INT f = 0; f < m_pZone->grouptable[group_no][0]; f++) {
            INT j = m_pZone->grouptable[group_no][f + 1];
            CMapModel* pModel = &m_pModelTbl[j];
            if (pModel->m_nGridID == 2) continue;
            BBOX* bb = pModel->getBBox(0);
            if (ii >= 0) {
                INT r = resultCheckBBox(Crd(ii + 1), m_pModelTbl[ii].getBBox(0));
                _kickCheckBBox(&Crd(j + 1).m_World, bb);
                if (r >= 0) {
                    setTransparency(Crd(ii + 1), m_pModelTbl[ii]);
                    if (m_LightUnit) m_LightUnit[ii].calc();
                    m_pModelTbl[ii].setBBoxFlag(r);
                    if (!DebugWork::g_dw.sys.m_iMapDisp || DebugWork::g_dw.sys.m_iMapDisp - 1 == ii) m_pModelTbl[ii].dispImpl();
                }
            } else {
                _kickCheckBBox(&Crd(j + 1).m_World, bb);
            }
            ii = j;
        }
        if (ii >= 0) {
            INT r = resultCheckBBox(Crd(ii + 1), m_pModelTbl[ii].getBBox(0));
            if (r >= 0) {
                setTransparency(Crd(ii + 1), m_pModelTbl[ii]);
                if (m_LightUnit) m_LightUnit[ii].calc();
                m_pModelTbl[ii].setBBoxFlag(r);
                if (!DebugWork::g_dw.sys.m_iMapDisp || DebugWork::g_dw.sys.m_iMapDisp - 1 == ii) m_pModelTbl[ii].dispImpl();
            }
        }
    } else {
        for (INT g = 0; g < 64; g++) {
            CMapGrid& grid = m_grid[g];
            if (grid.m_bbox.y[0] <= grid.m_bbox.y[1]) grid.flg = checkBBox(grid.m_pCrd->m_World, &grid.m_bbox);
        }
        INT ii = -1;
        for (INT i = 0; i < m_nModelMax; i++) {
            CMapModel* pModel = &m_pModelTbl[i];
            if (pModel->m_nGridID == -2) continue;
            if (pModel->m_nGridID != -1 && m_grid[pModel->m_nGridID].flg < 0) continue;
            BBOX* bb = pModel->getBBox(0);
            if (ii >= 0) {
                INT r = resultCheckBBox(Crd(ii + 1), m_pModelTbl[ii].getBBox(0));
                _kickCheckBBox(&Crd(i + 1).m_World, bb);
                if (r >= 0) {
                    setTransparency(Crd(ii + 1), m_pModelTbl[ii]);
                    if (m_LightUnit) m_LightUnit[ii].calc();
                    m_pModelTbl[ii].setBBoxFlag(r);
                    if (!DebugWork::g_dw.sys.m_iMapDisp || DebugWork::g_dw.sys.m_iMapDisp - 1 == ii) m_pModelTbl[ii].dispImpl();
                }
            } else {
                _kickCheckBBox(&Crd(i + 1).m_World, bb);
            }
            ii = i;
        }
        if (ii >= 0) {
            INT r = resultCheckBBox(Crd(ii + 1), m_pModelTbl[ii].getBBox(0));
            if (r >= 0) {
                setTransparency(Crd(ii + 1), m_pModelTbl[ii]);
                if (m_LightUnit) m_LightUnit[ii].calc();
                m_pModelTbl[ii].setBBoxFlag(r);
                if (!DebugWork::g_dw.sys.m_iMapDisp || DebugWork::g_dw.sys.m_iMapDisp - 1 == ii) m_pModelTbl[ii].dispImpl();
            }
        }
    }
    if (m_pFog->skynum > 0) {
        pSceneColor = CSceneEnv::getTypeColor((ObjType::eObjType)0xC);
        FMATRIX& wv = CWorldEnv::getWorldViewMatrix();
        INT max = m_pFog->skynum;
        INT nHorizon = (INT)((wv.m[0][1] * wv.m[2][0] + wv.m[2][1] * wv.m[2][2]) * 481.28f * 16.0f
                             / (wv.m[0][2] * wv.m[2][0] + wv.m[2][2] * wv.m[2][2]));
        if (max > 4) max = 4;
        for (INT i = 0; i < max; i++) {
            IVECTOR c0, c1;
            FVECTOR v;
            if (pSceneColor) {
                if (i == 0) {
                    ITOF0Vector(v, m_pFog->sky[0]);
                    MulVector(v, v, *pSceneColor);
                    FTOI0Vector(c1, v);
                }
                c0 = c1;
                ITOF0Vector(v, m_pFog->sky[i + 1]);
                MulVector(v, v, *pSceneColor);
                FTOI0Vector(c1, v);
            } else {
                c0 = m_pFog->sky[i];
                c1 = m_pFog->sky[i + 1];
            }
            CPolyG4& back = m_Back[i];
            back.m_C0.set(c1.x, c1.y, c1.z, 0);
            back.m_C1.set(c1.x, c1.y, c1.z, 0);
            back.m_C2.set(c0.x, c0.y, c0.z, 0x80);
            back.m_C3.set(c0.x, c0.y, c0.z, 0x80);
            INT y0 = nHorizon - (m_pFog->sky[i].w << 4);
            INT y1 = nHorizon - (m_pFog->sky[i + 1].w << 4);
            if ((-0xF40 < y0 && y0 < 0xF40) || (-0xF40 < y1 && y1 < 0xF40)) {
                back.m_P0.set(-0x1000, y1, 0);
                back.m_P1.set(0x1000, y1, 0);
                back.m_P2.set(-0x1000, y0, 0);
                back.m_P3.set(0x1000, y0, 0);
                back.disp();
            }
        }
    }
}
#else
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Game/Map", disp__4CMap);
#endif
void CMap::setColorScale(FVECTOR& inColorScale) {
    m_vColor = inColorScale;
    if (m_pSky) m_pSky->setColorScale(inColorScale);
}
void CMap::setSkyColorScale(FVECTOR& inColorScale) {
    if (m_pSky) m_pSky->setColorScale(inColorScale);
}
void CMap::getBBoxMax(FVECTOR& outMax) {
    INT n = m_nModelMax;
    if (n <= 0) {
        outMax.z = outMax.y = outMax.x = 100000.0f;
        return;
    }
    outMax.z = outMax.y = outMax.x = -100000.0f;
    for (INT i = 0; i < n; i++) {
        CMapModel& model = m_pModelTbl[i];
        if (!model.getModelData()) continue;
        BBOX* pBBox = model.getModelData()->m_pBBox;
        FVECTOR v0;
        FVECTOR v1;
        v0.set(pBBox->x[0], pBBox->y[0], pBBox->z[0]);
        v1.set(pBBox->x[1], pBBox->y[1], pBBox->z[1]);
        ApplyMatrix(v0, model.getCrd(0).m_World, v0);
        ApplyMatrix(v1, model.getCrd(0).m_World, v1);
        if (v0.x > outMax.x) {
            outMax.x = v0.x;
        } else if (v1.x > outMax.x) {
            outMax.x = v1.x;
        }
        if (v0.y > outMax.y) {
            outMax.y = v0.y;
        } else if (v1.y > outMax.y) {
            outMax.y = v1.y;
        }
        if (v0.z > outMax.z) {
            outMax.z = v0.z;
        } else if (v1.z > outMax.z) {
            outMax.z = v1.z;
        }
    }
}
void CMap::getBBoxMin(FVECTOR& outMin) {
    INT n = m_nModelMax;
    if (n <= 0) {
        outMin.z = outMin.y = outMin.x = -100000.0f;
        return;
    }
    outMin.z = outMin.y = outMin.x = 100000.0f;
    for (INT i = 0; i < n; i++) {
        CMapModel& model = m_pModelTbl[i];
        if (!model.getModelData()) continue;
        BBOX* pBBox = model.getModelData()->m_pBBox;
        FVECTOR v0;
        FVECTOR v1;
        v0.set(pBBox->x[0], pBBox->y[0], pBBox->z[0]);
        v1.set(pBBox->x[1], pBBox->y[1], pBBox->z[1]);
        ApplyMatrix(v0, model.getCrd(0).m_World, v0);
        ApplyMatrix(v1, model.getCrd(0).m_World, v1);
        if (v0.x < outMin.x) {
            outMin.x = v0.x;
        } else if (v1.x < outMin.x) {
            outMin.x = v1.x;
        }
        if (v0.y < outMin.y) {
            outMin.y = v0.y;
        } else if (v1.y < outMin.y) {
            outMin.y = v1.y;
        }
        if (v0.z < outMin.z) {
            outMin.z = v0.z;
        } else if (v1.z < outMin.z) {
            outMin.z = v1.z;
        }
    }
}
HANDLE CMap::createMapEffect_(const ZONEPARTS& inParts) {
    MAPUNITID unitId;
    unitId.ui = inParts.effectId;
    INT id = (unitId.c[1] - '0') * 100 + (unitId.c[2] - '0') * 10;
    id += unitId.c[3] - '0';
    id += 8000;
    CRsrcData* rsrc = CRsrcDB::getFirstRsrc(5, id + 30000, TRUE);
    if (!rsrc) {
        TRACE("DPX\202\252\203\215\201[\203h\202\263\202\352\202\304\202\334\202\271\202\361\202\252\202\310 = %d\n", id);
        return 0;
    }
    INT no = (USHORT)(inParts.effectParam / 100);
    FVECTOR rot;
    rot.x = inParts.rot.x * 0.1f * 0.017453292f;
    rot.y = inParts.rot.y * 0.1f * 0.017453292f;
    rot.z = inParts.rot.z * 0.1f * 0.017453292f;
    rot.w = 1.0f;
    Math3D::LimitPIVector(rot);
    HANDLE handle = 0;
    TRACE("    \203}\203b\203v\203G\203t\203F\203N\203g %s %d\n", rsrc->getName(), no);
    if (!DebugWork::g_dw.sys.m_iEffectNoEntry) {
        handle = CEffectMan::setEffect(rsrc->getName(), no, (ObjType::eObjType)0xB, (const FVECTOR*)inParts.trans, &rot, 0.0f);
        CEffect* effect = CEffectMan::getEffect(handle);
        if (effect) effect->setScale(*(const FVECTOR*)inParts.scale);
    }
    return handle;
}
void CMap::createEnvSE(const ZONEPARTS& inParts) {
    MAPUNITID unitId;
    unitId.ui = inParts.effectId;
    INT nFile = (unitId.c[2] - '0') * 10;
    INT nID = inParts.effectParam / 100;
    nFile += 100;
    nFile += unitId.c[3] - '0';
    if (unitId.c[1] == '0') CGame::playConstEnvSE(nFile * 1000 + nID, -1, FALSE);
}
extern "C" FLOAT sinf(FLOAT);
CSky::CSky(CTask* pInParent, void* pAZF, ZONEDATA* pZone) : CTaskSecondary(pInParent) {
    setPriority((TASK_PRIORITY)0x8000);
    m_modelSuu = pZone->skynum;
    m_pModel = new ("c:/KHCOM/MainApp/Game/Map.cpp(1813)") CModelBG[m_modelSuu];
    m_pRot = new ("c:/KHCOM/MainApp/Game/Map.cpp(1816)") FVECTOR[m_modelSuu];
    m_pRotShift = new ("c:/KHCOM/MainApp/Game/Map.cpp(1817)") FVECTOR[m_modelSuu];
    m_vColor.setAll(1.0f);
    addCoord(m_modelSuu + 1);
    CCoord& crd = Crd(0);
    _unitMatrixVu0(crd.m_Local);
    crd.touch();
    USHORT nLayer = 3;
    for (INT i = 0; i < m_modelSuu; i++) {
        CModelBG& model = m_pModel[i];
        ZONEPARTS* pZoneparts = &pZone->parts[i];
        m_pRot[i].set(0.0f, 0.0f, 0.0f, 0.0f);
        m_pRotShift[i].zero();
        if (pZoneparts->rot.x) {
            FVECTOR& v = *(FVECTOR*)((i << 4) + (INT)m_pRotShift); // MATCH: offset + base (addu s0,s1,s0); &m_pRotShift[i] adds base first
            v.x = sinf(Math3D::LimitPI(pZoneparts->rot.x * 0.01f * 0.017453292f)) * 0.1f;
        }
        if (pZoneparts->rot.y) {
            FVECTOR& v = *(FVECTOR*)((i << 4) + (INT)m_pRotShift);
            v.y = sinf(Math3D::LimitPI(pZoneparts->rot.y * 0.01f * 0.017453292f)) * 0.1f;
        }
        if (pZoneparts->rot.z) {
            FVECTOR& v = *(FVECTOR*)((i << 4) + (INT)m_pRotShift);
            v.z = sinf(Math3D::LimitPI(pZoneparts->rot.z * 0.01f * 0.017453292f)) * 0.1f;
        }
        m_pRotShift[i].w = 0.0f;
        model.setLayerDepth(nLayer);
        INT nIndex = pZone->parts[i].modelIndex + 1;
        MODELDATA* d;
        if (!pAZF) d = NULL;
        else d = ((MODELDATA**)pAZF)[nIndex];
        if (!d) AssertApp(D_00480CE8, 1846, D_00480D08, 0);
        _unitMatrixVu0(Crd(i + 1).m_Local);
        Crd(i + 1).m_pParent = &Crd(0);
        Crd(i + 1).touch();
        model.assign(this, d, i + 1);
        if (model.getSemiModel()) model.getSemiModel()->setTestFlag(0x3360D);
        if (nLayer < 9) nLayer += 2;
    }
}
CSky::~CSky() {
    if (m_pModel) delete[] m_pModel;
    m_pModel = NULL;
    if (m_pRot) delete m_pRot;
    m_pRot = NULL;
    if (m_pRotShift) delete m_pRotShift;
    m_pRotShift = NULL;
}
void ScaleVector(FVECTOR& v0, const FVECTOR& v1, FLOAT t);
void CSky::main() {
    if (CSceneEnv::getStopTypeBit() & 0x1000) return;
    FLOAT fRate = getFrameRateF();
    for (INT i = 0; i < m_modelSuu; i++) {
        FMATRIX lMat;
        INT ofs = i << 4; // MATCH: not in the ref; one offset for both arrays, offset + base (computed into s3 directly)
        FVECTOR vAddRot;
        if ((*(FVECTOR*)(ofs + (INT)m_pRotShift))._ul[0] == 0 && (*(FVECTOR*)(ofs + (INT)m_pRotShift))._ul[1] == 0) continue;
        ScaleVector(vAddRot, (*(FVECTOR*)(ofs + (INT)m_pRotShift)), fRate);
        (*(FVECTOR*)(ofs + (INT)m_pRot)).addRot(vAddRot);
        sceVu0UnitMatrix(lMat.m);
        sceVu0RotMatrix(Crd(i + 1).m_Local.m, lMat.m, ((FVECTOR*)(ofs + (INT)m_pRot))->v);
    }
}
void MulVector(FVECTOR& v0, const FVECTOR& v1, const FVECTOR& v2);
void CSky::disp() {
    if (DebugWork::g_dw.sys.m_iSkyDisp < -1) DebugWork::g_dw.sys.m_iSkyDisp = m_modelSuu;
    else if (m_modelSuu < DebugWork::g_dw.sys.m_iSkyDisp) DebugWork::g_dw.sys.m_iSkyDisp = -1;
    if (DebugWork::g_dw.sys.m_iSkyDisp < 0) return;
    CCamera* pCamera = CWorldEnv::getActiveCamera();
    if (!pCamera) return;
    Crd(0).m_Local.setTrans(pCamera->Crd(0).m_World.v[3]);
    adjustCoord();
    FVECTOR vColor;
    FVECTOR* pSceneColor = CSceneEnv::getTypeColor((ObjType::eObjType)0xC);
    if (pSceneColor) MulVector(vColor, m_vColor, *pSceneColor);
    else vColor = m_vColor;
    FLOAT fR = getFrameRateF();
    if (CSceneEnv::getStopTypeBit() & 0x1000) fR = 0.0f;
    for (INT i = 0; i < m_modelSuu; i++) {
        m_pModel[i].setColorScale(vColor);
        m_pModel[i].dispUpdate(fR);
        if (!DebugWork::g_dw.sys.m_iSkyDisp || DebugWork::g_dw.sys.m_iSkyDisp - 1 == i) m_pModel[i].disp();
    }
}
void CSky::setColorScale(FVECTOR& inColorScale) { m_vColor = inColorScale; }
// SOURCE DATA: .data .rodata
