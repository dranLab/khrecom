#define GAME_MAP_CPP
#include "khtypes.h"
#include "System/Task.h"
#include "System/Graphic/Gprim.h"
#include "System/Graphic/Model.h"
#include "System/Graphic/Camera.h"
#include "System/Graphic/LightUnit.h"
#include "System/Graphic/TexAnm.h"
#include "Game/Map.h"
#include "Game/MapUnitTable.h"
#include "Game/Game.h"
#include "Game/SceneEnv.h"
#include "Game/Collision/CollMapCamera.h"
#include "System/SJoint.h"
#include "include_asm.h"

#define SJ_FOREACH(T, pEntry, list) \
    for (T *pEntry = (list).getHead(), *__pn = pEntry ? pEntry->getNext() : NULL; pEntry; \
         pEntry = __pn, __pn = __pn ? __pn->getNext() : NULL)

/* Game/Collision/CollMapCamera.cpp */
class CMCSemiModel : public CSJointT<CMCSemiModel> {
public:
    FVECTOR center;
    FLOAT transparency;
    INT flag;
};
class CCMCSemiModelList : public CSJointListT<CMCSemiModel> {};

class CSky : public CTask {
public:
    FVECTOR m_vColor; /* 0x40 */
    CSky(CTask* pInParent, void* pAZF, ZONEDATA* pZone);
    virtual ~CSky();
    virtual void main();
    virtual void disp();
    void setColorScale(FVECTOR& inColorScale);
};

extern LIGHTDATA ALF_default;
extern "C" char D_004809E0[];
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
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Game/Map", _createEffIdProc__4CMapP9ZONEPARTSi);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Game/Map", __4CMapP5CTaskUi);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Game/Map", _$_4CMap);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Game/Map", init__4CMapii);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Game/Map", _exclusiveSetup__4CMapi);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Game/Map", resetFog__4CMap);
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
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Game/Map", getBBoxMax__4CMapR7FVECTOR);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Game/Map", getBBoxMin__4CMapR7FVECTOR);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Game/Map", createMapEffect___4CMapRC9ZONEPARTS);
void CMap::createEnvSE(const ZONEPARTS& inParts) {
    MAPUNITID unitId;
    unitId.ui = inParts.effectId;
    INT nFile = (unitId.c[2] - '0') * 10;
    INT nID = inParts.effectParam / 100;
    nFile += 100;
    nFile += unitId.c[3] - '0';
    if (unitId.c[1] == '0') CGame::playConstEnvSE(nFile * 1000 + nID, -1, FALSE);
}
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Game/Map", __4CSkyP5CTaskPvP8ZONEDATA);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Game/Map", _$_4CSky);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Game/Map", main__4CSky);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Game/Map", disp__4CSky);
void CSky::setColorScale(FVECTOR& inColorScale) { m_vColor = inColorScale; }
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Game/Map", __getSize__t7CVector1Z7G_STRIPi_00206010);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Game/Map", __alloc__t7CVector1Z7G_STRIPi_00206028);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Game/Map", _$_t7CVector1Z7G_STRIP_002060F8);
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
