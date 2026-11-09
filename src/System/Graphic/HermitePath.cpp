
#define GPRIM_H_PATH "c:/KHCOM/MainApp/System/Graphic/Gprim.h" /* MATCH: this unit names Gprim.h so (header default "System/Graphic/Gprim.h") */
#include "System/Graphic/Gprim.h"
#include "System/Graphic/PathData.h"
#include "System/Graphic/HermitePath.h"
#include "System/Math3D.h"
#include "System/Debug.h"
#include "System/HeapMan.h"
extern "C" float atan2f(float y, float x);

void SubVectorXYZ(FVECTOR& v0, const FVECTOR& v1, const FVECTOR& v2);
void AddVectorXYZ(FVECTOR& v0, const FVECTOR& v1, const FVECTOR& v2);
void MulVectorXYZ(FVECTOR& v0, const FVECTOR& v1, const FVECTOR& v2);
void ScaleVectorXYZ(FVECTOR& v0, const FVECTOR& v1, FLOAT r);
void Normalize(FVECTOR& v0, const FVECTOR& v1);


#define ASSERT(e) if (!(e)) AssertApp(__FILE__, __LINE__, #e, 0)

static inline FLOAT _fabsf(FLOAT x) {
    FLOAT ret;
    asm volatile("abs.s %0, %1" : "=f"(ret) : "f"(x));
    return ret;
}

static inline FLOAT _sqrtf(FLOAT x) {
    FLOAT ret;
    asm volatile("sqrt.s %0, %1" : "=f"(ret) : "f"(x));
    return ret;
}

CHermitePath::CHermitePath() {
    m_pPathData = 0;
    m_pRollData = 0;
    m_iBufSize = 0;
    m_uiStateFlag = 0;
    m_iDataNum = 0;
    m_iPathNum = 0;
    m_iNowBlock = 0;
    m_iStartBlock = 0;
    m_iEndBlock = 0;
    m_fAllDist = 0.0f;
    m_fProgress = 0.0f;
    m_fSpeed = 0.0f;
    m_fDataSpeed = 0.0f;
    m_pPathVect = 0;
}

CHermitePath::CHermitePath(INT inBufSize) {
    m_pPathData = 0;
    m_pRollData = 0;
    m_iBufSize = 0;
    m_uiStateFlag = 0;
    m_iDataNum = 0;
    m_iPathNum = 0;
    m_iNowBlock = 0;
    m_iStartBlock = 0;
    m_iEndBlock = 0;
    m_fAllDist = 0.0f;
    m_fProgress = 0.0f;
    m_fSpeed = 0.0f;
    m_fDataSpeed = 0.0f;
    m_pPathVect = 0;
    reserveBuf(inBufSize);
}

CHermitePath::~CHermitePath() {
    _deleteMem();
}

BOOL CHermitePath::reserveBuf(INT inBufSize) {
    return _allocateMem(inBufSize);
}

BOOL CHermitePath::_allocateMem(INT inNum) {
    BOOL bAlloc;
    if (inNum <= 0) {
        TRACE("CHermitePath : \x83\x6f\x83\x62\x83\x74\x83\x40\x83\x54\x83\x43\x83\x59\x82\xaa\x95\x73\x90\xb3\x82\xc5\x82\xb7\x81\x42\n");
        return FALSE;
    }
    if (inNum <= 1) {
        inNum++;
    }
    bAlloc = TRUE;
    if (m_pPathData) {
        if (m_iBufSize >= inNum) {
            bAlloc = FALSE;
        } else {
            TRACE("CHermitePath : \x83\x6f\x83\x62\x83\x74\x83\x40\x82\xaa\x8d\xc4\x8a\x6d\x95\xdb\x82\xb3\x82\xea\x82\xdc\x82\xb5\x82\xbd\x81\x42\n");
        }
    }
    if (bAlloc) {
        _deleteMem();
        m_pPathData = new (__FILE__ "(126)") UNIT_DATA[inNum];
        m_pRollData = new (__FILE__ "(127)") FLOAT[inNum];
#line 128
        ASSERT((m_pPathData)&&(m_pRollData));
        m_iBufSize = inNum;
    }
    m_iDataNum = inNum;
    m_iPathNum = inNum - 1;
    return TRUE;
}

void CHermitePath::_deleteMem() {
    if (m_pRollData) {
        com_free(m_pRollData);
    }
    m_pRollData = 0;
    if (m_pPathData) {
        com_free(m_pPathData);
    }
    m_pPathData = 0;
    m_iBufSize = m_iDataNum = m_iPathNum = 0;
    m_uiStateFlag = 0;
}

void CHermitePath::makePath(const FVECTOR3* inVect, INT inNum, UINT uiOptionFlag) {
    m_pPathVect = 0;
    m_uiStateFlag &= ~9;
    if (!inVect) {
        TRACE("CHermitePath : \x83\x66\x81\x5b\x83\x5e\x82\xaa\x8e\x77\x92\xe8\x82\xb3\x82\xea\x82\xc4\x82\xa2\x82\xdc\x82\xb9\x82\xf1\x81\x42\n");
        return;
    }
    if (!_allocateMem(inNum)) {
        return;
    }
    const FVECTOR3* pVect = inVect;
    m_uiStateFlag = 0;
    if (inNum < 2) {
        FVECTOR fvTmp[2];
        fvTmp[0].set(pVect[0].x, pVect[0].y, pVect[0].z, 0.0f);
        fvTmp[1] = fvTmp[0];
        _make2PointPath(fvTmp);
    } else if (inNum < 3) {
        FVECTOR fvTmp[2];
        fvTmp[0].set(pVect[0].x, pVect[0].y, pVect[0].z, 0.0f);
        fvTmp[1].set(pVect[1].x, pVect[1].y, pVect[1].z, 0.0f);
        _make2PointPath(fvTmp);
    } else {
        UNIT_DATA* pPath = m_pPathData;
        FLOAT* pRoll = m_pRollData;
        for (INT i = 0; i < m_iDataNum; i++) {
            pPath->fvPoint.set(pVect->x, pVect->y, pVect->z);
            *pRoll = 0.0f;
            pVect++;
            pPath++;
            pRoll++;
        }
        recalcHermitePath(uiOptionFlag);
    }
    m_fDataSpeed = 0.0f;
    goStartPos();
}

void CHermitePath::makePathFromFVector(const FVECTOR* inVect, INT inNum, UINT uiOptionFlag) {
    m_pPathVect = 0;
    m_uiStateFlag &= ~9;
    if (!inVect) {
        TRACE("CHermitePath : \x83\x66\x81\x5b\x83\x5e\x82\xaa\x8e\x77\x92\xe8\x82\xb3\x82\xea\x82\xc4\x82\xa2\x82\xdc\x82\xb9\x82\xf1\x81\x42\n");
        return;
    }
    if (!_allocateMem(inNum)) {
        return;
    }
    m_uiStateFlag = 0;
    if (inNum < 2) {
        FVECTOR fvTmp[2];
        fvTmp[0] = inVect[0];
        fvTmp[1] = fvTmp[0];
        _make2PointPath(fvTmp);
    } else if (inNum < 3) {
        _make2PointPath(inVect);
    } else {
        UNIT_DATA* pPath = m_pPathData;
        FLOAT* pRoll = m_pRollData;
        const FVECTOR* pVect = inVect;
        for (INT i = 0; i < m_iDataNum; i++) {
            pPath->fvPoint = *pVect;
            *pRoll = pVect->w;
            pVect++;
            pPath++;
            pRoll++;
        }
        recalcHermitePath(uiOptionFlag);
    }
    m_fDataSpeed = 0.0f;
    goStartPos();
}

void CHermitePath::makePathFromPathVector(const CPathVector* inpPath) {
    PATH_PARAM stParam;
    m_pPathVect = 0;
    m_uiStateFlag &= ~9;
    if (!inpPath) {
        TRACE("CHermitePath : \x83\x66\x81\x5b\x83\x5e\x82\xaa\x8e\x77\x92\xe8\x82\xb3\x82\xea\x82\xc4\x82\xa2\x82\xdc\x82\xb9\x82\xf1\x81\x42\n");
        return;
    }
    m_pPathVect = (CPathVector*)inpPath;
    inpPath->getPathParam(stParam);
    INT inNum = inpPath->m_iVecMax;
    if (!_allocateMem(inNum)) {
        return;
    }
    m_uiStateFlag = 0;
    if (inNum < 2) {
        FVECTOR fvTmp[2];
        fvTmp[0] = inpPath->m_pVec[0];
        fvTmp[0].w = inpPath->getPathRoll(0);
        fvTmp[1] = fvTmp[0];
        _make2PointPath(fvTmp);
    } else if (m_iDataNum < 3) {
        _make2PointPath(inpPath->m_pVec);
        FLOAT* pRoll = m_pRollData;
        pRoll[0] = inpPath->getPathRoll(0);
        pRoll[1] = inpPath->getPathRoll(1);
    } else {
        UNIT_DATA* pData = m_pPathData;
        FLOAT* pRoll = m_pRollData;
        const FVECTOR* pVect = inpPath->m_pVec;
        for (INT i = 0; i < m_iDataNum; i++) {
            pData->fvPoint = *pVect;
            *pRoll = inpPath->getPathRoll(i);
            pVect++;
            pData++;
            pRoll++;
        }
        recalcHermitePath(stParam.iFlags);
    }
    m_fDataSpeed = 0.0f;
    if (stParam.iFlags & 1) {
        if (stParam.fTime != m_fDataSpeed) {
            setSpeed(getHermiteDistanceAll() / (stParam.fTime * 60.0f));
            setDataSpeed();
        }
    } else if (stParam.iFlags & 2) {
        setSpeed(stParam.fSpeed * 0.0046296297f);
        setDataSpeed();
    }
    if (m_fSpeed >= 0.0f) {
        goStartPos();
    } else {
        goEndPos();
    }
}

void CHermitePath::_make2PointPath(const FVECTOR* inVect) {
    FVECTOR fvTmp;
    UNIT_DATA* pPath = m_pPathData;
    FLOAT* pRoll = m_pRollData;
    FVECTOR t0;
    FVECTOR t1;
    pPath[0].fvPoint = inVect[0];
    pPath[1].fvPoint = inVect[1];
    SubVectorXYZ(t0, pPath[0].fvPoint, pPath[1].fvPoint);
    AddVectorXYZ(t1, pPath[0].fvPoint, t0);
    pPath[0].fvPoint.w = Math3D::VectorLength(t1);
    pPath[1].fvPoint.w = Math3D::VectorLength(t0);
    SubVectorXYZ(t0, pPath[1].fvPoint, pPath[0].fvPoint);
    pPath[0].fvTangent = t0;
    pPath[1].fvTangent = t0;
    pPath[0].fvTangent.w = pPath[0].fvPoint.w;
    pPath[1].fvTangent.w = pPath[1].fvPoint.w;
    pRoll[0] = inVect[0].w;
    pRoll[1] = inVect[1].w;
    m_fAllDist = 0.0f;
    for (INT i = 0; i < m_iPathNum; i++) {
        m_fAllDist += m_pPathData[i].fvTangent.w;
    }
    m_uiStateFlag |= 4;
    m_iEndBlock = m_iPathNum;
    m_iStartBlock = 0;
}

void CHermitePath::recalcHermitePath(UINT uiOptionFlag) {
    BOOL bIsRound = (uiOptionFlag & 0x40000000) != 0;
    BOOL bIsLinear = uiOptionFlag >> 31;
    FVECTOR fvTmp;
    UNIT_DATA* pPath;
    FMATRIX mat;
    m_iStartBlock = 0;
    m_iEndBlock = m_iDataNum - 1;
    if (m_iDataNum < 3) {
        return;
    }
    if (bIsRound) {
        m_iPathNum = m_iDataNum;
        m_uiStateFlag |= 1;
    } else {
        m_iPathNum = m_iDataNum - 1;
        m_uiStateFlag &= ~1;
    }
    if (bIsLinear) {
        m_uiStateFlag |= 8;
    } else {
        m_uiStateFlag &= ~8;
    }
    m_iEndBlock = m_iPathNum;
    m_iStartBlock = 0;
    pPath = &m_pPathData[1];
    for (INT i = 1; i < m_iDataNum; i++) {
        SubVectorXYZ(fvTmp, pPath->fvPoint, pPath[-1].fvPoint);
        pPath->fvPoint.w = Math3D::VectorLength(fvTmp);
        pPath++;
    }
    if (m_uiStateFlag & 1) {
        SubVectorXYZ(fvTmp, m_pPathData[0].fvPoint, (m_pPathData + m_iDataNum - 1)->fvPoint);
        m_pPathData[0].fvPoint.w = Math3D::VectorLength(fvTmp);
    } else {
        m_pPathData[0].fvPoint.w = m_pPathData[1].fvPoint.w;
    }
    pPath = m_pPathData;
    for (INT i = 0; i < m_iDataNum; i++) {
        _makeTangent(pPath->fvTangent, i);
        pPath++;
    }
    pPath = m_pPathData;
    for (INT i = 0; i < m_iPathNum; i++) {
        FVECTOR fvOut;
        FVECTOR fvPrev;
        FLOAT fDistance;
        getHermiteMatrix(mat, i);
        fDistance = 0.0f;
        fvPrev = pPath->fvPoint;
        for (INT j = 1; j < 32; j++) {
            Math3D::HermiteInterpolation(fvOut, mat, j * 0.03125f);
            SubVectorXYZ(fvTmp, fvOut, fvPrev);
            fDistance += Math3D::VectorLength(fvTmp);
            fvPrev = fvOut;
        }
        pPath->fvTangent.w = fDistance;
        pPath++;
    }
    if (!(m_uiStateFlag & 1)) {
        pPath->fvTangent.w = pPath[-1].fvTangent.w;
    }
    _calcAllDist();
}

void CHermitePath::_makeTangent(FVECTOR& outVect, INT inIndex) {
    FVECTOR t0;
    FVECTOR t1;
    FVECTOR t2;
    t1 = m_pPathData[inIndex].fvPoint;
    if (m_uiStateFlag & 1) {
        if (inIndex <= 0) {
            t0 = m_pPathData[m_iDataNum - 1].fvPoint;
        } else {
            t0 = m_pPathData[inIndex - 1].fvPoint;
        }
        if (inIndex >= m_iDataNum - 1) {
            t2 = m_pPathData[0].fvPoint;
        } else {
            t2 = m_pPathData[inIndex + 1].fvPoint;
        }
    } else {
        if (inIndex > 0) {
            t0 = m_pPathData[inIndex - 1].fvPoint;
        }
        if (inIndex < m_iDataNum - 1) {
            t2 = m_pPathData[inIndex + 1].fvPoint;
        }
        if (inIndex <= 0) {
            SubVectorXYZ(t0, t1, t2);
            AddVectorXYZ(t0, t1, t0);
        }
        if (inIndex >= m_iDataNum - 1) {
            SubVectorXYZ(t2, t1, t0);
            AddVectorXYZ(t2, t1, t2);
        }
    }
    SubVectorXYZ(t1, t2, t0);
    ScaleVectorXYZ(outVect, t1, 0.5f);
    outVect.w = 0.0f;
}

void CHermitePath::_calcAllDist() {
    m_fAllDist = 0.0f;
    for (INT i = m_iStartBlock; i < m_iEndBlock; i++) {
        m_fAllDist += m_pPathData[i].fvTangent.w;
    }
}

#line 564
void CHermitePath::changePathPoint(const FVECTOR* inNewPoint, INT inIdx) {
    ASSERT((inIdx>=0)&&(inIdx<m_iBufSize));
    m_pPathData[inIdx].fvPoint.set(inNewPoint->x, inNewPoint->y, inNewPoint->z);
}

#line 575
void CHermitePath::changePathRoll(FLOAT inNewRoll, INT inIdx) {
    ASSERT((inIdx>=0)&&(inIdx<m_iBufSize));
    m_pRollData[inIdx] = inNewRoll;
}

#line 587
void CHermitePath::changePathNum(INT inNum) {
    ASSERT((inNum>=0)&&(inNum<=m_iBufSize));
    m_iDataNum = inNum;
}

#line 598
void CHermitePath::clearAllRollData(FLOAT infRoll) {
    ASSERT(m_pRollData);
    for (INT i = 0; i < m_iBufSize; i++) {
        m_pRollData[i] = infRoll;
    }
}

#line 608
void CHermitePath::getHermiteMatrix(FMATRIX& outMat, INT inIdx) {
    UNIT_DATA* p0;
    UNIT_DATA* p1;
    UNIT_DATA* p2;
    if (m_uiStateFlag & 1) {
        ASSERT((m_pPathData)&&(m_pRollData)&&(inIdx<m_iDataNum));
        p0 = &m_pPathData[inIdx % m_iDataNum];
        p1 = &m_pPathData[(inIdx + 1) % m_iDataNum];
        p2 = &m_pPathData[(inIdx + 2) % m_iDataNum];
        outMat.v[0] = p0->fvPoint;
        outMat.v[1] = p1->fvPoint;
        if (m_uiStateFlag & 8) {
            SubVectorXYZ(outMat.v[2], p1->fvPoint, p0->fvPoint);
            outMat.v[3] = outMat.v[2];
        } else {
            outMat.v[2] = p0->fvTangent;
            outMat.v[3] = p1->fvTangent;
        }
        if (m_iDataNum >= 3) {
            ScaleVectorXYZ(outMat.v[2], outMat.v[2], (p1->fvPoint.w + p1->fvPoint.w) / (p0->fvPoint.w + p1->fvPoint.w));
            ScaleVectorXYZ(outMat.v[3], outMat.v[3], (p1->fvPoint.w + p1->fvPoint.w) / (p1->fvPoint.w + p2->fvPoint.w));
        }
    } else {
#line 634
        ASSERT((m_pPathData)&&(m_pRollData)&&(inIdx<m_iPathNum));
        p0 = &m_pPathData[inIdx];
        outMat.v[0] = p0[0].fvPoint;
        outMat.v[1] = p0[1].fvPoint;
        if (m_uiStateFlag & 8) {
            SubVectorXYZ(outMat.v[2], p0[1].fvPoint, p0[0].fvPoint);
            outMat.v[3] = outMat.v[2];
        } else {
            outMat.v[2] = p0[0].fvTangent;
            outMat.v[3] = p0[1].fvTangent;
        }
        if (m_iDataNum >= 3) {
            ScaleVectorXYZ(outMat.v[2], outMat.v[2], (p0[1].fvPoint.w + p0[1].fvPoint.w) / (p0[0].fvPoint.w + p0[1].fvPoint.w));
            if (inIdx < m_iPathNum - 1) {
                ScaleVectorXYZ(outMat.v[3], outMat.v[3], (p0[1].fvPoint.w + p0[1].fvPoint.w) / (p0[1].fvPoint.w + p0[2].fvPoint.w));
            }
        }
    }
}

#line 664
FLOAT CHermitePath::getHermiteDistance(INT inIdx) {
    ASSERT((m_pPathData)&&((inIdx>=0)&&(inIdx<m_iPathNum)));
    return m_pPathData[inIdx].fvTangent.w;
}

#line 674
FLOAT CHermitePath::getHermiteDistanceBlockToBlock(INT inIdx0, INT inIdx1) {
    FLOAT fDistance;
    ASSERT((m_pPathData)&&((inIdx0>=0)&&(inIdx1<=m_iPathNum)));
    fDistance = 0.0f;
    for (INT i = inIdx0; i < inIdx1; i++) {
        fDistance += getHermiteDistance(i);
    }
    return fDistance;
}

FLOAT CHermitePath::getHermiteCoefficient(INT inIdx, FLOAT infPos) {
    FLOAT fDist = getHermiteDistance(inIdx);
    if (infPos <= 0.0f) {
        return 0.0f;
    }
    if (infPos > fDist) {
        return 1.0f;
    }
    return infPos / fDist;
}

FLOAT CHermitePath::getHermiteDistanceAll() {
    return m_fAllDist;
}

#line 712
FLOAT CHermitePath::getHermiteDistanceRest(INT inIdx, FLOAT inNowDist) {
    FLOAT fDist;
    ASSERT((m_pPathData)&&((inIdx>=0)&&(inIdx<m_iPathNum)));
    fDist = m_fAllDist - inNowDist;
    if (fDist < 0.0f) {
        fDist = 0.0f;
    }
    return fDist;
}

void CHermitePath::getStartPos(FVECTOR& outPos) {
    if (m_pPathData) {
        UNIT_DATA* p = &m_pPathData[m_iStartBlock];
        outPos.set(p->fvPoint.x, p->fvPoint.y, p->fvPoint.z);
    } else {
        outPos.init();
    }
}

void CHermitePath::getEndPos(FVECTOR& outPos) {
    if (m_pPathData) {
        UNIT_DATA* p = &m_pPathData[m_iEndBlock];
        outPos.set(p->fvPoint.x, p->fvPoint.y, p->fvPoint.z);
    } else {
        outPos.init();
    }
}

void CHermitePath::setSpeed(FLOAT infSpd) {
    if ((m_fSpeed < 0.0f && infSpd > 0.0f) || (m_fSpeed > 0.0f && infSpd < 0.0f)) {
        m_uiStateFlag &= ~2;
    }
    m_fSpeed = infSpd;
}

BOOL CHermitePath::isRoundData() {
    return m_uiStateFlag & 1;
}

BOOL CHermitePath::isLinearMoveData() {
    return m_uiStateFlag & 8;
}

BOOL CHermitePath::isEnd() {
    if (!m_pPathData) {
        return TRUE;
    }
    if (m_uiStateFlag & 1) {
        return FALSE;
    }
    if (m_uiStateFlag & 2) {
        return TRUE;
    }
    if (m_fSpeed < 0.0f) {
        if (m_fProgress <= 0.0f && m_iNowBlock <= m_iStartBlock) {
            m_uiStateFlag |= 2;
            return TRUE;
        }
    } else {
        if (m_iNowBlock >= m_iEndBlock - 1 && _fabsf(m_pPathData[m_iEndBlock - 1].fvTangent.w - m_fProgress) <= 1e-5f) {
            m_uiStateFlag |= 2;
            return TRUE;
        }
    }
    return FALSE;
}

BOOL CHermitePath::update(FLOAT inRate) {
    BOOL bRet;
    if (!m_pPathData) {
        return TRUE;
    }
    m_fProgress += m_fSpeed * inRate;
    bRet = _update(&m_iNowBlock, &m_fProgress, m_fSpeed);
    if (bRet) {
        m_uiStateFlag |= 2;
    }
    return bRet;
}

BOOL CHermitePath::_update(SHORT* pBlock, FLOAT* pfProgress, FLOAT inSpeed) {
    FLOAT fDist;
    BOOL bIsRound = m_uiStateFlag & 1;
    BOOL bRet = FALSE;
    m_uiStateFlag &= ~2;
    if (inSpeed >= 0.0f) {
        for (;;) {
            fDist = m_pPathData[*pBlock].fvTangent.w;
            if (*pfProgress <= fDist) {
                break;
            }
            if (*pBlock >= m_iEndBlock - 1) {
                if (!bIsRound) {
                    *pBlock = m_iEndBlock - 1;
                    bRet = TRUE;
                    *pfProgress = fDist;
                    break;
                }
                *pBlock = m_iStartBlock - 1;
            }
            *pfProgress -= fDist;
            (*pBlock)++;
        }
    } else {
        for (;;) {
            if (*pfProgress >= 0.0f) {
                break;
            }
            if (*pBlock <= m_iStartBlock) {
                if (!bIsRound) {
                    *pBlock = m_iStartBlock;
                    bRet = TRUE;
                    *pfProgress = 0.0f;
                    break;
                }
                *pBlock = m_iEndBlock;
            }
            (*pBlock)--;
            *pfProgress += m_pPathData[*pBlock].fvTangent.w;
        }
    }
    return bRet;
}

void CHermitePath::getNowPos(FVECTOR& inrefPos) {
    FMATRIX mat;
    FLOAT fT;
    if (m_pPathData) {
        if (m_iNowBlock < 0) {
            getStartPos(inrefPos);
        } else if (m_iNowBlock >= m_iEndBlock) {
            getEndPos(inrefPos);
        } else {
            fT = getHermiteCoefficient(m_iNowBlock, m_fProgress);
            getHermiteMatrix(mat, m_iNowBlock);
            Math3D::HermiteInterpolation(inrefPos, mat, fT);
            inrefPos.w = 1.0f;
        }
    }
}

void CHermitePath::getGapPos(FVECTOR& inrefPos, FLOAT inDist) {
    SHORT iBlock;
    FLOAT fProgress;
    FLOAT fSpd;
    if (m_pPathData) {
        fSpd = m_fSpeed;
        iBlock = m_iNowBlock;
        fProgress = m_fProgress;
        if (fSpd < 0.0f) {
            fProgress -= inDist;
            if (inDist < 0.0f) {
                fSpd = 1.0f;
            }
        } else {
            fProgress += inDist;
            if (inDist < 0.0f) {
                fSpd = -1.0f;
            }
        }
        _update(&iBlock, &fProgress, fSpd);
        FMATRIX mat;
        FLOAT fT;
        fT = getHermiteCoefficient(iBlock, fProgress);
        getHermiteMatrix(mat, iBlock);
        Math3D::HermiteInterpolation(inrefPos, mat, fT);
        inrefPos.w = 1.0f;
    }
}

void CHermitePath::getNowRot(FVECTOR& inrefRot, FLOAT inDist) {
    FMATRIX mat;
    FVECTOR fvPos1;
    FVECTOR fvPos2;
    FLOAT fT;
    if (m_pPathData) {
        fT = getHermiteCoefficient(m_iNowBlock, m_fProgress);
        getHermiteMatrix(mat, m_iNowBlock);
        Math3D::HermiteInterpolation(fvPos1, mat, fT);
        fvPos2 = fvPos1;
        getGapPos(fvPos2, inDist);
        SubVectorXYZ(fvPos1, fvPos2, fvPos1);
        if (Math3D::VectorLengthSq(fvPos1) <= 1e-5f) {
            return;
        }
        Normalize(fvPos1, fvPos1);
        MulVectorXYZ(fvPos2, fvPos1, fvPos1);
        inrefRot.x = -atan2f(fvPos1.y, _sqrtf(fvPos2.x + fvPos2.z));
        inrefRot.y = atan2f(fvPos1.x, fvPos1.z);
    }
}

FLOAT CHermitePath::getNowRoll() {
    FLOAT fT;
    FLOAT fRoll1;
    FLOAT fRoll2;
    FLOAT fDiff;
    if (!m_pRollData) {
        return 0.0f;
    }
    fT = getHermiteCoefficient(m_iNowBlock, m_fProgress);
    if (m_uiStateFlag & 1) {
        if (m_iPathNum == 0) {
            fRoll1 = m_pRollData[m_iNowBlock];
            fRoll2 = fRoll1;
        } else {
            fRoll1 = m_pRollData[m_iNowBlock];
            fRoll2 = m_pRollData[(m_iNowBlock + 1) % m_iPathNum];
        }
    } else {
        fRoll1 = m_pRollData[m_iNowBlock];
        fRoll2 = m_pRollData[m_iNowBlock + 1];
    }
    fDiff = Math3D::LimitPI(fRoll2 - fRoll1);
    return Math3D::LimitPI(fRoll1 + fT * fDiff);
}

void CHermitePath::goStartPos() {
    if (m_pPathData) {
        m_fProgress = 0.0f;
        m_iNowBlock = m_iStartBlock;
        if (m_fSpeed < m_fProgress) {
            m_uiStateFlag |= 2;
        }
    }
}

void CHermitePath::goEndPos() {
    if (m_pPathData) {
        m_iNowBlock = m_iEndBlock - 1;
        m_fProgress = getHermiteDistance(m_iNowBlock);
        if (m_fSpeed >= 0.0f) {
            m_uiStateFlag |= 2;
        }
    }
}

void CHermitePath::goBlockStartPos(INT inBlock) {
    if (m_pPathData) {
        m_iNowBlock = inBlock;
        if (m_iNowBlock < m_iStartBlock) {
            m_iNowBlock = m_iStartBlock;
        }
        if (m_iNowBlock > m_iEndBlock - 1) {
            m_iNowBlock = m_iEndBlock - 1;
        }
        m_fProgress = 0.0f;
        if (m_iNowBlock <= m_iStartBlock && m_fSpeed < m_fProgress) {
            m_uiStateFlag |= 2;
        }
    }
}

void CHermitePath::goBlockEndPos(INT inBlock) {
    if (m_pPathData) {
        m_iNowBlock = inBlock;
        if (m_iNowBlock < m_iStartBlock) {
            m_iNowBlock = m_iStartBlock;
        }
        if (m_iNowBlock > m_iEndBlock - 1) {
            m_iNowBlock = m_iEndBlock - 1;
        }
        m_fProgress = getHermiteDistance(m_iNowBlock);
        if (m_iNowBlock >= m_iEndBlock - 1 && m_fSpeed >= 0.0f) {
            m_uiStateFlag |= 2;
        }
    }
}

void CHermitePath::setStartEndBlock(INT inStartBlock, INT inEndBlock) {
    if (m_uiStateFlag & 1) {
        return;
    }
    if (inStartBlock > m_iDataNum - 1) {
        inStartBlock = m_iDataNum - 1;
    }
    if (inStartBlock < 0) {
        inStartBlock = 0;
    }
    if (inEndBlock > m_iDataNum - 1) {
        inEndBlock = m_iDataNum - 1;
    }
    if (inEndBlock < 0) {
        inEndBlock = 0;
    }
    if (inStartBlock < inEndBlock) {
        m_iStartBlock = inStartBlock;
        m_iEndBlock = inEndBlock;
        _calcAllDist();
    }
}
// SOURCE DATA: .rodata
