#include "Game/Collision/CollUtilA.h"
#include "System/HeapMan.h"
#define COLLUTIL_H_PATH "c:/KHCOM/MainApp/Game/Collision/CollUtil.h" /* MATCH: the original's path in CollUtil.h's strings */
#include "Game/Collision/CollUtil.h"

// CCollision has an inline virtual destructor: its weak copy ends this object, its weak vtable ends the .rodata
// (an unnamed copy at 0x4894E8: the original linker kept Game/SEMotion's name; symbol_addrs.txt names this one).

#include "Game/Collision/CollObj.h"




























































#include "System/Coord.h"


INT CCollUtil::_checkHangEdge(CCollision& collision, FVECTOR& inDir, FVECTOR& inHeightDepth) {
    CCollision coll = collision;
    FVECTOR vec0;
    FVECTOR vec1;
    FLOAT h;

    coll.m_inWE = coll.m_inWS;
    coll.m_inWE.y += inHeightDepth.y;
    if (_detectMCD(coll) && coll.m_outNormal.y < -0.5) {
        return 0;
    }
    coll.m_inWS = coll.m_outWE;
    vec0.x = inDir.x;
    vec0.y = 0.0f;
    vec0.z = inDir.z;
    Normalize(vec0, vec0);
    ScaleVector(vec0, vec0, inHeightDepth.z);
    coll.m_inWE = coll.m_inWS + vec0;
    if (_detectMCD(coll)) {
        return 0;
    }
    vec0 = coll.m_inWS;
    coll.m_inWS = coll.m_outWE;
    coll.m_inWE = coll.m_outWE;
    coll.m_inWE.y += inHeightDepth.x;
    if (!_detectMCD(coll)) {
        return 0;
    }
    if (coll.m_outNormal.y < 0.975f) {
        return 0;
    }
    h = coll.m_inWS.y - coll.m_outWE.y + 0.1f;
    coll.m_inWE = coll.m_inWS;
    coll.m_inWS = vec0;
    coll.m_inWE.y -= h;
    coll.m_inWS.y -= h;
    coll.m_fgStatus = 0;
    coll.m_Type = COLL_TYPE_POINT;
    coll.m_shiftY = 0.0f;
    if (!_detectMCD(coll)) {
        return 0;
    }
    if (coll.m_outNormal.y > 0.025f) {
        return 0;
    }
    if (coll.m_outInfo.infoATTR == 1) {
        return 0;
    }
    coll.m_outWE.y += 0.1f;
    inHeightDepth = coll.m_outWE;
    inDir = coll.m_outNormal;
    return 1;
}

INT CCollUtil::_checkLadderUp(FVECTOR& inPos, FVECTOR& inDir, FVECTOR& inHeightDepth) {
    CCollision coll;
    FVECTOR vec0;
    FVECTOR vec1;

    coll.initForMap();
    coll.m_inWS = inPos;
    coll.m_inWE = inPos;
    coll.m_inWE.y += inHeightDepth.y;
    if (_detectMCD(coll)) {
        return -1;
    }
    vec0.x = inDir.x;
    coll.m_inWS = coll.m_outWE;
    vec0.z = inDir.z;
    vec0.y = 0.0f;
    Normalize(vec0, vec0);
    ScaleVector(vec0, vec0, inHeightDepth.z);
    coll.m_inWE = coll.m_inWS + vec0;
    if (_detectMCD(coll)) {
        if (coll.m_outInfo.infoATTR == 2) {
            return 0;
        }
        return -1;
    }
    return 1;
}

void CCollUtil::_getPassingPosition(FVECTOR& outPos, FVECTOR& outNorm) {
    if (m_pDetectMatP) {
        ApplyMatrix(outPos, *m_pDetectMatP, m_vDetectP);
        m_vNormalP.w = 0.0f;
        ApplyMatrix(outNorm, *m_pDetectMatP, m_vNormalP);
    }
}

FLOAT CCollUtil::_getLadderPosition(FVECTOR& outPos, FVECTOR& outNorm) {
    MC_DATA* pMCData;
    INT nTotal;
    MC_PRIM* pPrim;
    FVECTOR* pVert;
    FLOAT fHeightMax;
    INT i;
    SHORT* pIdx;
    INT j;
    FLOAT fHeight;

    if (!m_pDetectMatA) {
        return 0.0f;
    }
    outPos = m_pDetectMatA->v[3];
    m_vNormalA.w = 0.0f;
    ApplyMatrix(outNorm, *m_pDetectMatA, m_vNormalA);
    pMCData = (MC_DATA*)m_pDetectChipA->data;
    nTotal = ((UINT)pMCData->prim - (UINT)pMCData->prim4) / sizeof(MC_PRIM);
    pPrim = pMCData->prim4;
    pVert = pMCData->vert;
    fHeightMax = -1000000.0f;
    for (i = 0; i < nTotal; i++, pPrim++) {
        pIdx = &pPrim->v0;
        for (j = 0; j < 4; j++, pIdx++) {
            fHeight = pVert[*pIdx].y;
            if (fHeightMax < fHeight) {
                fHeightMax = fHeight;
            }
            INT fg = pPrim->kind == 0;
            if (j != 2) {
                fg = 0;
            }
            if (fg) {
                break;
            }
        }
    }
    return fHeightMax;
}

INT CCollUtil::_checkWaterSurface(FVECTOR& inPos) {
    FVECTOR dummy;

    m_pCheckColl->m_Radius = 0.0f;
    m_pCheckColl->m_inWS = m_pCheckColl->m_inWE = inPos;
    m_pCheckColl->m_inWS.y += 1.0f;
    _detectMCD(*m_pCheckColl);
    if (m_pCheckColl->m_outInfo.infoPID == 2) {
        _getPassingPosition(inPos, dummy);
        return 1;
    }
    return 0;
}
// SOURCE DATA: .rodata
