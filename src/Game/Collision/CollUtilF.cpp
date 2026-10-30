#include "Game/Collision/CollUtilA.h"
#include "System/HeapMan.h"

// CCollision has an inline virtual destructor: its weak copy ends this object. Its weak vtable _vt$10CCollision
// is not part of this object (the original linker kept Game/SEMotion's copy); the build drops ours.

enum COLL_TYPE {
    COLL_TYPE_NONE = 0,
    COLL_TYPE_POINT = 1,
};

enum COLL_KIND {
    COLL_KIND_PLAYER = 0,
};

#include "Game/Collision/CollInfo.h"









class CCollision {
public:
    /* 0x00 */ FVECTOR m_inWS;
    /* 0x10 */ FVECTOR m_inWE;
    /* 0x20 */ FVECTOR m_outWE;
    /* 0x30 */ FVECTOR m_outNormal;
    /* 0x40 */ FVECTOR m_outDetect;
    /* 0x50 */ COLL_TYPE m_Type;
    /* 0x54 */ COLL_KIND m_Kind;
    /* 0x58 */ INT m_outStatus;
    /* 0x5c */ INT m_fgStatus;
    /* 0x60 */ FLOAT m_Radius;
    /* 0x64 */ FLOAT m_shiftY;
    /* 0x68 */ COLL_INFO_UNION m_outInfo;
    /* 0x6c */ COLL_INFO_UNION m_outInfoO;

    CCollision() {
        m_Kind = COLL_KIND_PLAYER;
        m_outInfoO.info = 0;
        m_outInfo.info = 0;
        m_shiftY = 0.0f;
        m_Radius = 0.0f;
        m_fgStatus = 0;
    }
    CCollision(const CCollision& c) {
        m_inWS = c.m_inWS;
        m_inWE = c.m_inWE;
        m_outWE = c.m_outWE;
        m_outNormal = c.m_outNormal;
        m_outDetect = c.m_outDetect;
        m_Type = c.m_Type;
        m_Kind = c.m_Kind;
        m_outStatus = c.m_outStatus;
        m_fgStatus = c.m_fgStatus;
        m_Radius = c.m_Radius;
        m_shiftY = c.m_shiftY;
        m_outInfo = c.m_outInfo;
        m_outInfoO = c.m_outInfoO;
    }
    virtual ~CCollision() {}
};

// PARTIAL: from System/Coord.h
void Normalize(FVECTOR& v0, const FVECTOR& v1);
void ScaleVector(FVECTOR& v0, const FVECTOR& v1, FLOAT f);
void ApplyMatrix(FVECTOR& v0, const FMATRIX& m0, const FVECTOR& v1);

inline FVECTOR operator+(const FVECTOR& v0, const FVECTOR& v1) {
    FVECTOR v = v0;
    v.x += v1.x;
    v.y += v1.y;
    v.z += v1.z;
    v.w += v1.w;
    return v;
}

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

