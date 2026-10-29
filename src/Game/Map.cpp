#define GAME_MAP_CPP
#define TASK_H_NO_SLC_OVERFLOW_MSG /* MATCH: the original Map .rodata has no SLC overflow message */
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
#include "System/Graphic/Effect/EffectMan.h"
#include "System/Graphic/Effect/effect.h"
#include "System/Graphic/Effect/effect_inl.h"
#include "include_asm.h"

inline void operator delete[](void* pv) { com_free(pv); }

#define SJ_FOREACH(T, pEntry, list) \
    for (T *pEntry = (list).getHead(), *__pn = pEntry ? pEntry->getNext() : NULL; pEntry; \
         pEntry = __pn, __pn = __pn ? __pn->getNext() : NULL)

class CMCSemiModel : public CSJointT<CMCSemiModel> { /* PARTIAL: stand-in for Game/Collision/CollMapCamera CMCSemiModel: owner not written (defined in CollMapCamera.cpp; ref: lost+found) */
public:
    FVECTOR center;
    FLOAT transparency;
    INT flag;
};
class CCMCSemiModelList : public CSJointListT<CMCSemiModel> {}; /* PARTIAL: stand-in for Game/Collision/CollMapCamera CCMCSemiModelList: owner not written */

class CMapModel : public CModel {
    INT m_nGridID; /* 0x120 */
    INT _pad[3];   /* PARTIAL: the original CModel is 16-byte aligned (size 0x130) */
};

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
extern "C" char D_004809E0[];
extern "C" char D_00480EB0[];
extern "C" char D_00480FA0[];
extern "C" char D_00480FC8[];
extern "C" char D_00480FF0[];
extern "C" char D_00480CE8[];
extern "C" char D_00480D08[];
class CBuf;
class CRsrcData { /* PARTIAL: stand-in for System/RsrcData CRsrcData: gated by RSRCDATA_H_FULL */
    CBuf* m_pDataSrc;
    CHAR m_szName[1];

public:
    CHAR* getName() const { return (CHAR*)m_szName; }
};
class CRsrcDB { /* PARTIAL: stand-in for System/RsrcDB CRsrcDB: gated by RSRCDATA_H_FULL */
public:
    static CRsrcData* getFirstRsrc(INT nRsrcType, INT nGroupID, BOOL bFollowLink);
};
extern "C" char D_00480F58[];
extern "C" char D_00480F80[];
extern "C" char D_00480A10[];
extern "C" char D_00480A38[];
extern MAPUNITTABLE g_DefaultGimmick;
void RotMatrixZ(FMATRIX& out, FMATRIX& in);
void setTransparency(const CCoord& crd, CModel& model);

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
    TRACE(D_004809E0, infinite_suu, point_suu, spot_suu, dum0);
}
void RotMatrixZ(FMATRIX& out, FMATRIX& in) {
    FMATRIX tmp = in;
    tmp.m[0][0] = -tmp.m[0][0];
    tmp.m[0][1] = -tmp.m[0][1];
    tmp.m[0][2] = -tmp.m[0][2];
    tmp.m[0][3] = -tmp.m[0][3];
    tmp.m[1][0] = -tmp.m[1][0];
    tmp.m[1][1] = -tmp.m[1][1];
    tmp.m[1][2] = -tmp.m[1][2];
    tmp.m[1][3] = -tmp.m[1][3];
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
            TRACE(D_00480A10, parts->effectId, szEffIDName);
            if (!defaultTbl.create(getSysTask(defaultTbl.kind), parts)) TRACE(D_00480A38);
        }
    }
    }
}
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Game/Map", __4CMapP5CTaskUi);
/* Parked: draft not verified yet; waits for the weak tail - as CMap's key function it emits every CMap inline,
   which collide with the INCLUDE_ASM'd weak copies. */
#if 0
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
#else
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Game/Map", _$_4CMap);
#endif
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Game/Map", init__4CMapii);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Game/Map", _exclusiveSetup__4CMapi);
void CMap::resetFog() {
    CWorldEnv::setFogParam(m_pFog->fognear, m_pFog->fogfar, m_pFog->fogmax, m_pFog->fogmin);
    CWorldEnv::setDispFar(m_pFog->dispfar);
    CWorldEnv::setFogColor(m_pFog->color.x, m_pFog->color.y, m_pFog->color.z);
    CWorldEnv::setScreenZ(m_pFog->screenz);
    CWorldEnv::setDefScreenZ(m_pFog->screenz);
    DebugWork::g_dw.nakamich.fCamVAngle = ProjectionToViewAngle(m_pFog->screenz);
    INT max = m_pFog->skynum;
    if (max > 4) {
        TRACE(D_00480EB0);
        max = 4;
    }
    for (INT i = 0; i < max; i++) {
        m_Back[i].m_C0.set(m_pFog->sky[i + 1].x, m_pFog->sky[i + 1].y, m_pFog->sky[i + 1].z, 0);
        m_Back[i].m_C1.set(m_pFog->sky[i + 1].x, m_pFog->sky[i + 1].y, m_pFog->sky[i + 1].z, 0);
        m_Back[i].m_C2.set(m_pFog->sky[i].x, m_pFog->sky[i].y, m_pFog->sky[i].z, 0x80);
        m_Back[i].m_C3.set(m_pFog->sky[i].x, m_pFog->sky[i].y, m_pFog->sky[i].z, 0x80);
    }
}
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
    FVECTOR ctr = crd.m_World.v[3];
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
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Game/Map", disp__4CMap);
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
        TRACE(D_00480F58, id);
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
    TRACE(D_00480F80, rsrc->getName(), no);
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
#define RET_ADDR() ({ UINT nRA; __asm__ volatile("daddu %0, $31, $0" : "=r"(nRA)); nRA; })
inline void* operator new[](size_t size, char* msg) throw() { return com_malloc_d(size, msg, RET_ADDR()); }
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
extern "C" FLOAT sinf(FLOAT);
/* Parked: 3 lines - the three &m_pRotShift[i] before the LimitPI/sinf calls come out as base+offset (addu s0,s0,s1), the
   original adds offset+base; reference, pointer, index-first, inline-helper forms all keep it (permuter can't take the
   initializer-list constructor). */
#if 0
CSky::CSky(CTask* pInParent, void* pAZF, ZONEDATA* pZone) : CTaskSecondary(pInParent) {
    setPriority((TASK_PRIORITY)0x8000);
    m_modelSuu = pZone->skynum;
    m_pModel = new (D_00480FA0) CModelBG[m_modelSuu];
    m_pRot = new (D_00480FC8) FVECTOR[m_modelSuu];
    m_pRotShift = new (D_00480FF0) FVECTOR[m_modelSuu];
    m_vColor.setAll(1.0f);
    addCoord(m_modelSuu + 1);
    CCoord& crd = Crd(0);
    _unitMatrixVu0(crd.m_Local);
    crd.m_Flag = 1;
    USHORT nLayer = 3;
    for (INT i = 0; i < m_modelSuu; i++) {
        CModelBG& model = m_pModel[i];
        ZONEPARTS* pZoneparts = &pZone->parts[i];
        m_pRot[i].set(0.0f, 0.0f, 0.0f, 0.0f);
        m_pRotShift[i]._q = 0;
        if (pZoneparts->rot.x) {
            FVECTOR& v = m_pRotShift[i];
            v.x = sinf(Math3D::LimitPI(pZoneparts->rot.x * 0.01f * 0.017453292f)) * 0.1f;
        }
        if (pZoneparts->rot.y) {
            FVECTOR& v = m_pRotShift[i];
            v.y = sinf(Math3D::LimitPI(pZoneparts->rot.y * 0.01f * 0.017453292f)) * 0.1f;
        }
        if (pZoneparts->rot.z) {
            FVECTOR& v = m_pRotShift[i];
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
        Crd(i + 1).m_Flag = 1;
        model.assign(this, d, i + 1);
        if (model.getSemiModel()) model.getSemiModel()->setTestFlag(0x3360D);
        if (nLayer < 9) nLayer += 2;
    }
}
#else
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Game/Map", __4CSkyP5CTaskPvP8ZONEDATA);
#endif
CSky::~CSky() {
    if (m_pModel) delete[] m_pModel;
    m_pModel = NULL;
    if (m_pRot) delete m_pRot;
    m_pRot = NULL;
    if (m_pRotShift) delete m_pRotShift;
    m_pRotShift = NULL;
}
extern "C" void sceVu0UnitMatrix(FMATRIX& m);
extern "C" void sceVu0RotMatrix(FMATRIX& m0, FMATRIX& m1, FVECTOR& rot);
void ScaleVector(FVECTOR& v0, const FVECTOR& v1, FLOAT t);
/* Parked: 6 lines - the loop's i*16 offset goes to v1 and is copied to s3 (the original computes it into s3 directly, so
   `move a0,s4` fills the first test's delay slot); FVECTOR::addRot with `this` as asm operand, zero-test, reference,
   declaration and index forms and an 8 min permuter run (12k candidates) all keep the copy. */
#if 0
void CSky::main() {
    if (CSceneEnv::getStopTypeBit() & 0x1000) return;
    FLOAT fRate = getFrameRateF();
    for (INT i = 0; i < m_modelSuu; i++) {
        FMATRIX lMat;
        FVECTOR vAddRot;
        if (m_pRotShift[i]._ul[0] == 0 && m_pRotShift[i]._ul[1] == 0) continue;
        ScaleVector(vAddRot, m_pRotShift[i], fRate);
        m_pRot[i].addRot(vAddRot);
        sceVu0UnitMatrix(lMat);
        sceVu0RotMatrix(Crd(i + 1).m_Local, lMat, m_pRot[i]);
    }
}
#else
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Game/Map", main__4CSky);
#endif
void MulVector(FVECTOR& v0, const FVECTOR& v1, const FVECTOR& v2);
void CSky::disp() {
    if (DebugWork::g_dw.sys.m_iSkyDisp < -1) DebugWork::g_dw.sys.m_iSkyDisp = m_modelSuu;
    else if (m_modelSuu < DebugWork::g_dw.sys.m_iSkyDisp) DebugWork::g_dw.sys.m_iSkyDisp = -1;
    if (DebugWork::g_dw.sys.m_iSkyDisp < 0) return;
    CCamera* pCamera = CWorldEnv::getActiveCamera();
    if (!pCamera) return;
    Crd(0).m_Local.v[3] = pCamera->Crd(0).m_World.v[3];
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
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Game/Map", func_00206010);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Game/Map", func_00206028);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Game/Map", func_002060F8);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Game/Map", _$_8CMapGrid);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Game/Map", _$_9CMapModel);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Game/Map", getMapID__C4CMap);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Game/Map", getFogData__4CMap);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Game/Map", setFogData__4CMapP7FOGDATA);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Game/Map", getLightData__4CMap);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Game/Map", getMapLightData__C4CMap);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Game/Map", getModel__4CMap);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Game/Map", getModel__4CMapi);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Game/Map", getModelMax__4CMap);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Game/Map", getMapFlag__4CMap);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Game/Map", setMapFlag__4CMapUiUi);
