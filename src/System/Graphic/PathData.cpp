#define GPRIM_H_PATH "c:/KHCOM/MainApp/System/Graphic/Gprim.h" /* MATCH: this unit names Gprim.h so (header default "System/Graphic/Gprim.h") */
#include "System/Graphic/PathData.h"
#include "System/Graphic/WorldEnv.h"
#include "System/Math3D.h"
#include "System/HeapMan.h"
#include <math.h>
#include <string.h>
#include "System/MainApp.h"
#include "System/TaskMan.h"
#include "System/Graphic/HermitePath.h"
#include "include_asm.h"






















void ScaleVectorXYZ(FVECTOR& v0, const FVECTOR& v1, FLOAT f);
void SubVector(FVECTOR& v0, const FVECTOR& v1, const FVECTOR& v2); void SubVectorXYZ(FVECTOR& v0, const FVECTOR& v1, const FVECTOR& v2);
void AddVector(FVECTOR& v0, const FVECTOR& v1, const FVECTOR& v2);
void ScaleVector(FVECTOR& v0, const FVECTOR& v1, FLOAT f);
void Normalize(FVECTOR& v0, const FVECTOR& v1); void OuterProduct(FVECTOR& v0, const FVECTOR& v1, const FVECTOR& v2); FLOAT InnerProduct(const FVECTOR& v0, const FVECTOR& v1);
void AddVectorXYZ(FVECTOR& v0, const FVECTOR& v1, const FVECTOR& v2);
void ApplyMatrix33(FVECTOR& v0, const FMATRIX& m0, const FVECTOR& v1);
void MulMatrix(FMATRIX& res, const FMATRIX& m0, const FMATRIX& m1);
void FTOI15Vector(IVECTOR& v0, const FVECTOR& v1);
void ITOF15Vector(FVECTOR& v0, const IVECTOR& v1);







// PARTIAL: System/RsrcDB.h would add RsrcData.h's string to .rodata
#include "System/RsrcDB.h"




#define ASSERT(e) KH_DEBUG(if (!(e)) AssertApp(__FILE__, __LINE__, #e, 0))


CPathVector::~CPathVector() {
    KH_DEBUG(if (m_iStatus & 1) {)
        KH_DEBUG(if (m_pVec) com_free(m_pVec);)
        KH_DEBUG(m_pVec = NULL;)
    KH_DEBUG(})
    KH_DEBUG(if (m_pCube) delete[] m_pCube;)
    KH_DEBUG(m_pCube = NULL;)
}
void CPathVector::init(const FVECTOR* pPath, INT iPathMax, INT iMode) {
    m_pPathData = NULL;
    _init(pPath, iPathMax, iMode);
}
void CPathVector::init(const PATH_VECTOR* pPathData, BOOL bAlloc) {
    const FVECTOR* pPath = NULL;
    INT iPathMax = 0;
    m_pPathData = (PATH_VECTOR*)pPathData;
    if (pPathData) {
        pPath = pPathData->vVec;
        iPathMax = pPathData->sParam.iVecNum;
    } else {
#line 76
        ASSERT(! (!bAlloc));
    }
    _init(pPath, iPathMax, bAlloc);
}
void CPathVector::_init(const FVECTOR* pPath, INT iPathMax, BOOL bAlloc) {
    KH_DEBUG(CTask* pMap;)
    KH_DEBUG(G_STRIP Line;)
    KH_DEBUG(m_iStatus = 2;)
    KH_DEBUG(m_pVec = NULL;)
    KH_DEBUG(m_iActive = 0;)
    KH_DEBUG(if (bAlloc) {)
        KH_DEBUG(getPathParam(m_sParam);)
        KH_DEBUG(m_iVecMax = iPathMax;)
        KH_DEBUG(if (iPathMax <= 0) m_iVecMax = 1;)
        KH_DEBUG(m_iVecMemMax = m_iVecMax + 8;)
#line 94
        KH_DEBUG(m_pVec = new KH_AT("(94)") FVECTOR[m_iVecMemMax];)
        KH_DEBUG(ASSERT(m_pVec);)
        KH_DEBUG(if (pPath) {)
            KH_DEBUG(memcpy(m_pVec, pPath, m_iVecMax * sizeof(FVECTOR));)
            KH_DEBUG(if (m_pPathData && m_pPathData->sParam.iVer < 2) {)
                KH_DEBUG(for (INT i = 0; i < m_iVecMax; i++) {)
                    KH_DEBUG(FLOAT fRoll = m_pVec[i].w;)
                    KH_DEBUG(m_pVec[i].w = 0.0f;)
                    KH_DEBUG(_setPathVectorConvWRoll(&m_pVec[i].w, fRoll);)
                    KH_DEBUG(_setPathVectorConvWSpeed(&m_pVec[i].w, 0.0f);)
                KH_DEBUG(})
            KH_DEBUG(})
        KH_DEBUG(} else {)
            KH_DEBUG(for (INT i = 0; i < m_iVecMax; i++) {)
                KH_DEBUG(m_pVec[i].set(0.0f, 1.0f, (FLOAT)i, 0.0f);)
            KH_DEBUG(})
        KH_DEBUG(})
        KH_DEBUG(m_iStatus |= 1;)
#line 122
        KH_DEBUG(m_pCube = new KH_AT("(122)") CDbgCube[m_iVecMemMax];)
        KH_DEBUG(for (INT i = 0; i < m_iVecMemMax; i++) {)
            KH_DEBUG(m_pCube[i].setCoord(NULL, NULL);)
        KH_DEBUG(})
    KH_DEBUG(} else {)
#line 131
        ASSERT(pPath);
        ASSERT(!(iPathMax <= 0));
        KH_DEBUG(m_pVec = (FVECTOR*)pPath;) KH_RELEASE(m_iVecMax = iPathMax;)
        KH_DEBUG(m_iVecMax = iPathMax;) KH_RELEASE(m_pVec = (FVECTOR*)pPath;)
#line 137
        KH_DEBUG(m_pCube = new KH_AT("(137)") CDbgCube[iPathMax];)
        KH_DEBUG(for (INT i = 0; i < m_iVecMax; i++) {)
            KH_DEBUG(m_pCube[i].setCoord(&m_pVec[i], NULL);)
        KH_DEBUG(})
    KH_DEBUG(})
    KH_DEBUG(m_vRot.set(0.0f, 0.0f, 0.0f, 2.0f);)
    KH_DEBUG(pMap = CTaskMan::getSysTask(2);)
    KH_DEBUG(if (pMap) {)
        KH_DEBUG(m_DDPathLines.setCoord(&pMap->Crd(0));)
        KH_DEBUG(m_DDPathPolys.setCoord(&pMap->Crd(0));)
        KH_DEBUG(m_DDPathPolys.setSemiTrans(ALPHA_BLEND);)
    KH_DEBUG(} else {)
        KH_DEBUG(TRACE("\x83\x7d\x83\x62\x83\x76\x82\xaa\x82\xa0\x82\xe8\x82\xdc\x82\xb9\x82\xf1\n");)
    KH_DEBUG(})
    KH_DEBUG(CCoord* pCrd = &m_DDXYZCoord;)
    KH_DEBUG(vu0UnitMatrix(pCrd->m_Local);)
    KH_DEBUG(pCrd->touch();)
    KH_DEBUG(m_DDXYZLines.setCoord(&m_DDXYZCoord);)
    KH_DEBUG(Line.Term = 0;)
    KH_DEBUG(for (INT i = 0; i <= 40; i++) {)
        KH_DEBUG(Line.P.set(0.0f, (FLOAT)(i - 30), 0.0f);)
        KH_DEBUG(Line.C.set(8, i * 5 + 0x37, 8, 0xff);)
        KH_DEBUG(if (i >= 40) Line.Term = 1;)
        KH_DEBUG(m_DDXYZLines.m_vVERT.push_back(Line);)
    KH_DEBUG(})
    KH_DEBUG(Line.P.set(-1.0f, 0.0f, 0.0f);)
    KH_DEBUG(Line.C.set(0x40, 8, 8, 0xff);)
    KH_DEBUG(Line.Term = 0;)
    KH_DEBUG(m_DDXYZLines.m_vVERT.push_back(Line);)
    KH_DEBUG(Line.P.set(0.0f, 0.0f, 0.0f);)
    KH_DEBUG(Line.C.set(0xa0, 8, 8, 0xff);)
    KH_DEBUG(Line.Term = 0;)
    KH_DEBUG(m_DDXYZLines.m_vVERT.push_back(Line);)
    KH_DEBUG(Line.P.set(1.0f, 0.0f, 0.0f);)
    KH_DEBUG(Line.C.set(0xff, 8, 8, 0xff);)
    KH_DEBUG(Line.Term = 1;)
    KH_DEBUG(m_DDXYZLines.m_vVERT.push_back(Line);)
    KH_DEBUG(Line.P.set(0.0f, 0.0f, -1.0f);)
    KH_DEBUG(Line.C.set(8, 8, 0x40, 0xff);)
    KH_DEBUG(Line.Term = 0;)
    KH_DEBUG(m_DDXYZLines.m_vVERT.push_back(Line);)
    KH_DEBUG(Line.P.set(0.0f, 0.0f, 0.0f);)
    KH_DEBUG(Line.C.set(8, 0x28, 0xa0, 0xff);)
    KH_DEBUG(Line.Term = 0;)
    KH_DEBUG(m_DDXYZLines.m_vVERT.push_back(Line);)
    KH_DEBUG(Line.P.set(0.0f, 0.0f, 1.0f);)
    KH_DEBUG(Line.C.set(8, 0x48, 0xff, 0xff);)
    KH_DEBUG(Line.Term = 1;)
    KH_DEBUG(m_DDXYZLines.m_vVERT.push_back(Line);)
}
void CPathVector::makeVertexSpeedAll() {
    INT i;
    INT j;
    INT iNCnt = 0;
    INT iUCnt = 0;
    INT iNum = m_iVecMax;
    float fVSpeed[iNum];
    if (iNum >= 2) {
        for (i = 0; i < iNum; i++) {
            fVSpeed[i] = getPathSpeed(i);
            if (fVSpeed[i] > 0.0f) iUCnt++;
            if (fVSpeed[i] <= 0.0f) iNCnt++;
            TRACE("Spd %d,%f\n", i, fVSpeed[i]);
        }
    }
    if (iUCnt > 0 && iNCnt > 0) {
        CHermitePath cHPath;
        cHPath.makePathFromPathVector(this);
        FLOAT fBeforeSpeed;
        INT iBefore = 0;
        TRACE("New %d,%f\n", 0, fVSpeed[0]);
        fBeforeSpeed = fVSpeed[0];
        for (i = 1; i < iNum; i++) {
            if (fVSpeed[i] <= 0.0f) {
                FLOAT fNextSpeed;
                INT iNext;
                FLOAT fNextDis;
                FLOAT t;
                for (j = i + 1; j < iNum; j++) {
                    if (fVSpeed[j] > 0.0f) {
                        fNextSpeed = fVSpeed[j];
                        iNext = j;
                        break;
                    }
                }
                if (j >= iNum) {
                    fNextSpeed = fBeforeSpeed;
                    iNext = iNum - 1;
                }
                fNextDis = cHPath.getHermiteDistanceBlockToBlock(i, iNext);
                t = cHPath.getHermiteDistanceBlockToBlock(iBefore, i);
                fNextDis += t;
                t = t / fNextDis;
                fVSpeed[i] = (fNextSpeed - fBeforeSpeed) * t + fBeforeSpeed;
                _setPathVectorConvWSpeed(&m_pVec[i].w, fVSpeed[i]);
            }
            TRACE("New %d,%f\n", i, fVSpeed[i]);
            iBefore = i;
            fBeforeSpeed = fVSpeed[i];
        }
    }
}
FLOAT CPathVector::getPathRoll(INT iNum) const {
    FLOAT fRoll; KH_RELEASE(if (!m_pPathData) return 0.0f;)
    if (iNum >= m_iVecMax) iNum = m_iVecMax - 1;
    if (iNum < 0) iNum = 0;
    if (KH_DEBUG((m_iStatus & 1) || !m_pPathData ||) m_pPathData->sParam.iVer >= 2) {
        fRoll = _getPathVectorConvWRoll(m_pVec[iNum].w);
    } else {
        fRoll = m_pVec[iNum].w;
    }
    return fRoll;
}
FLOAT CPathVector::getPathSpeed(INT iNum) const {
    FLOAT fSpeed; KH_RELEASE(if (!m_pPathData) return 0.0f;)
    if (iNum >= m_iVecMax) iNum = m_iVecMax - 1;
    if (iNum < 0) iNum = 0;
    if (KH_DEBUG((m_iStatus & 1) || !m_pPathData ||) m_pPathData->sParam.iVer >= 2) {
        fSpeed = _getPathVectorConvWSpeed(m_pVec[iNum].w);
    } else {
        fSpeed = 0.0f;
    }
    return fSpeed;
}
FLOAT _getPathVectorConvWRoll(FLOAT w) {
    INT iRoll = *(INT*)&w;
    iRoll &= 0xfff;
    if (iRoll & 0x800) iRoll |= 0xfffff000;
    return iRoll * 0.0015339808f;
}
void _setPathVectorConvWRoll(FLOAT* fW, FLOAT fRoll) {
    INT iRoll = (INT)(fRoll * 651.8986f);
    *(UINT*)fW = (*(UINT*)fW & 0xfffff000) | (iRoll & 0xfff);
}
FLOAT _getPathVectorConvWSpeed(FLOAT w) {
    UINT iSpeed = *(UINT*)&w;
    iSpeed >>= 16;
    if (!iSpeed) return 0.0f;
    return iSpeed * 0.015625f;
}
void _setPathVectorConvWSpeed(FLOAT* fW, FLOAT fSpeed) {
    INT iSpeed;
    if (fSpeed <= 0.001f) iSpeed = 0;
    else iSpeed = (INT)(fSpeed * 64.0f);
    *(UINT*)fW = *(USHORT*)fW | (iSpeed << 16);
}
void CPathVector::getPathParam(PATH_PARAM& sParam) const {
    static const PATH_PARAM sPathParamInit = {0, 10.0f, 10.0f, 5.0f, 1.0f, -1, 0.0f, 1.0f, 16.0f, 16.0f, 0.0f};
    if (KH_DEBUG((m_iStatus & 1) && &m_sParam != &sParam) KH_RELEASE(0)) {
        KH_DEBUG(sParam = m_sParam;)
    } else {
        sParam = sPathParamInit;
        sParam.fProj = CWorldEnv::getScreenZ();
        if (m_pPathData) {
            sParam.iFlags = m_pPathData->sParam.iFlags;
            INT* p = (INT*)&m_pPathData->vVec[m_pPathData->sParam.iVecNum];
            if (sParam.iFlags & 1) sParam.fTime = *(FLOAT*)p++;
            if (sParam.iFlags & 2) sParam.fSpeed = *(FLOAT*)p++;
            if (sParam.iFlags & 4) sParam.fTargetDistance = *(FLOAT*)p++;
            if (sParam.iFlags & 8) sParam.fTargetOffsetY = *(FLOAT*)p++;
            if (sParam.iFlags & 0x10) sParam.iParentLink = *p++;
            if (sParam.iFlags & 0x20) sParam.fProj = *(FLOAT*)p++;
            if (sParam.iFlags & 0x40) sParam.fLookDelay = *(FLOAT*)p++;
            if (sParam.iFlags & 0x80) sParam.fLearpIn = *(FLOAT*)p++;
            if (sParam.iFlags & 0x100) sParam.fLearpOut = *(FLOAT*)p++;
            if (sParam.iFlags & 0x200) sParam.fProj2 = *(FLOAT*)p++;
        }
    }
}

void CPathVector::getNearPath(FVECTOR& outvN, float* outfDis, float* outfT, int* outiStep, const FVECTOR& vTarget,
                              const FVECTOR* pathv, int path_max, float hashi) {
    FLOAT fNearDis;
    FLOAT fNearT;
    INT iNearStep;
    FVECTOR vNearNv;
    FLOAT fDis;
    FLOAT t;
    INT i;
    INT j;
    FLOAT fChkDis;
    FLOAT fChkNearDis;
    FLOAT fBottom;
    int step[4];
    INT dir;
    INT flg;
    FVECTOR pnv;
    FVECTOR pvec;
    FVECTOR fvTmp;
    FVECTOR pv[4];
    FVECTOR v0[4];
    FVECTOR v1[4];
    FVECTOR vCloss;
    KH_DEBUG(m_DDPathPolys.m_vVERT.clear();)
    fNearDis = 1.7014118e+38f; // MATCH: in the beta's marker order (515-518)
    fChkNearDis = 1.7014118e+38f;
    fNearT = 0.0f;
    iNearStep = 0;
    vNearNv.init0();
    /* MATCH: an asm with an operand and no clobber ends cse's equivalences (fNearT = 0.0f), so the hoisted 0.0f
       compare constant gets its own mtc1 zero instead of a copy of fNearT */
    __asm__ volatile("" : : "i"(0));
    dir = 0;
    for (i = -1; i <= path_max - 1; i++) {
        step[0] = i - 1;
        step[1] = i;
        step[2] = i + 1;
        step[3] = 2 + i;
        for (j = 0; j < 4; j++) {
            if (step[j] < 0) {
                SubVector(fvTmp, pathv[0], pathv[1]);
                Normalize(fvTmp, fvTmp);
                ScaleVector(fvTmp, fvTmp, hashi * (FLOAT)(-step[j]));
                AddVector(pv[j], pathv[0], fvTmp);
            } else if (step[j] >= path_max) {
                SubVector(fvTmp, pathv[path_max - 1], pathv[path_max - 2]);
                Normalize(fvTmp, fvTmp);
                ScaleVector(fvTmp, fvTmp, hashi * (FLOAT)(step[j] - (path_max - 1)));
                AddVector(pv[j], pathv[path_max - 1], fvTmp);
            } else {
                pv[j] = pathv[step[j]];
            }
        }
        fDis = Math3D::GetDistance2OfLineAndPoint(pv[1], pv[2], vTarget);
        if (fDis < fChkNearDis) {
            pv[3].y = pv[2].y = pv[1].y = pv[0].y = vTarget.y;
            flg = 0;
            Math3D::GetIntersectionOfLineAndPoint(vCloss, pv[1], pv[2], vTarget);
            fChkDis = Math3D::PointToPointDistance(vCloss, vTarget);
            SubVector(pnv, vTarget, vCloss);
            SubVector(pvec, pv[2], pv[1]);
            fvTmp.x = 0.0f;
            fvTmp.y = -1.0f;
            fvTmp.z = 0.0f;
            OuterProduct(pvec, pvec, fvTmp);
            if (InnerProduct(pnv, pvec) >= 0.0f) flg = 1;
            for (j = 1; j < 4; j++) {
                SubVector(v0[j], pv[j], pv[j - 1]);
                Normalize(v0[j], v0[j]);
                OuterProduct(v0[j], v0[j], fvTmp);
                ScaleVector(v0[j], v0[j], fChkDis);
                if (flg) AddVector(v0[j], pv[j], v0[j]);
                else SubVector(v0[j], pv[j], v0[j]);
            }
            for (j = 1; 3 > j; j++) {
                if (i + (j - 1) <= 0 || i + (j - 1) >= path_max - 1) {
                    fBottom = 0.0f;
                } else {
                    fBottom = (pv[j + 1].x - pv[j].x) * (pv[j].z - pv[j - 1].z) -
                              (pv[j].x - pv[j - 1].x) * (pv[j + 1].z - pv[j].z);
                }
                if (!(fBottom > -0.001f && fBottom < 0.001f)) {
                    t = ((pv[j + 1].x - pv[j].x) * (v0[j + 1].z - v0[j].z) -
                         (v0[j + 1].x - v0[j].x) * (pv[1 + j].z - pv[j].z)) / fBottom;
                } else {
                    t = 0.0f;
                }
                v1[j - 1].x = (pv[j].x - pv[j - 1].x) * t + v0[j].x;
                v1[j - 1].y = pv[j].y;
                v1[j - 1].z = (pv[j].z - pv[j - 1].z) * t + v0[j].z;
            }
            SubVector(v0[0], pv[1], pv[2]);
            SubVector(v0[1], v1[0], v1[1]);
            if (InnerProduct(v0[0], v0[1]) > 0.0f) {
                KH_DEBUG(G_STRIP Line;)
                KH_DEBUG(Line.P = pv[1];)
                KH_DEBUG(Line.Term = 0;)
                KH_DEBUG(Line.C.col = 0x400000FF;)
                KH_DEBUG(Line.P.w = 1.0f;)
                KH_DEBUG(m_DDPathPolys.m_vVERT.push_back(Line);)
                KH_DEBUG(Line.P = v1[0];)
                KH_DEBUG(Line.C.col = 0x40808080;)
                KH_DEBUG(Line.P.w = 1.0f;)
                KH_DEBUG(m_DDPathPolys.m_vVERT.push_back(Line);)
                KH_DEBUG(Line.P = pv[2];)
                KH_DEBUG(Line.C.col = 0x40FF0000;)
                KH_DEBUG(Line.P.w = 1.0f;)
                KH_DEBUG(m_DDPathPolys.m_vVERT.push_back(Line);)
                KH_DEBUG(Line.P = v1[1];)
                KH_DEBUG(Line.C.col = 0x40808080;)
                KH_DEBUG(Line.P.w = 1.0f;)
                KH_DEBUG(m_DDPathPolys.m_vVERT.push_back(Line);)
                KH_DEBUG(m_DDPathPolys.m_vVERT.back().Term = 1;)
                t = Math3D::GetIntersectionOfLineAndPoint(vCloss, v1[0], v1[1], vTarget);
                if (0.0f <= t && t <= 1.0f) {
                    if (i < path_max - 1 || !dir) {
                        if (0 <= i) dir++;
                        vNearNv = pnv;
                        if (!flg) fChkDis = -fChkDis;
                        fNearDis = fChkDis;
                        fNearT = t;
                        iNearStep = i;
                        fChkNearDis = fDis;
                    }
                }
            }
        }
    }
    Normalize(outvN, vNearNv);
    *outfDis = fNearDis;
    *outfT = fNearT;
    *outiStep = iNearStep;
}
float CPathVector::getTargetDistanceToPathStep(const FVECTOR& tpos, float distance) {
    int path_max = m_iVecMax;
    FVECTOR* pathv = m_pVec;
    int istep;
    float t;
    float dis;
    float hashi = 100.0f;
    FVECTOR ftmp;
    FVECTOR nv;
    getNearPath(nv, &dis, &t, &istep, tpos, pathv, path_max, hashi);
    if (istep >= path_max - 1) {
        SubVector(ftmp, pathv[path_max - 1], pathv[path_max - 2]);
        Normalize(ftmp, ftmp);
        ScaleVector(ftmp, ftmp, hashi);
        istep = path_max - 1;
        ScaleVector(ftmp, ftmp, t);
        AddVector(ftmp, ftmp, pathv[path_max - 1]);
    } else if (istep < 0) {
        SubVector(ftmp, pathv[0], pathv[1]);
        Normalize(ftmp, ftmp);
        ScaleVector(ftmp, ftmp, hashi);
        istep = -1;
        ScaleVector(ftmp, ftmp, 1.0f - t);
        AddVector(ftmp, ftmp, pathv[0]);
    } else {
        SubVector(ftmp, pathv[istep + 1], pathv[istep]);
        ScaleVector(ftmp, ftmp, t);
        AddVector(ftmp, ftmp, pathv[istep]);
    }
    if (distance < 0.0f) {
        if (istep >= path_max - 1) {
            istep = path_max - 2;
            t = 1.0f;
        } else {
            istep++;
            distance = -distance;
            while (1) {
                SubVector(ftmp, ftmp, pathv[istep]);
                dis = Math3D::VectorLengthXZ(ftmp);
                if (distance < dis) {
                    dis -= distance;
                    if (istep <= 0) {
                        istep = 0;
                        t = 0.0f;
                    } else {
                        istep--;
                        SubVector(ftmp, pathv[istep + 1], pathv[istep]);
                        t = 1.0f - dis / Math3D::VectorLengthXZ(ftmp);
                    }
                    break;
                }
                distance -= dis;
                ftmp = pathv[istep];
                istep++;
                if (istep > path_max - 1) {
                    istep = path_max - 2;
                    t = 1.0f;
                    break;
                }
            }
        }
    } else {
        if (istep < 0) {
            t = 0.0f;
            istep = 0;
        } else {
            while (1) {
                SubVector(ftmp, ftmp, pathv[istep]);
                dis = Math3D::VectorLengthXZ(ftmp);
                if (distance < dis) {
                    dis -= distance;
                    if (istep >= path_max - 1) {
                        istep = path_max - 2;
                        t = 1.0f;
                    } else {
                        SubVector(ftmp, pathv[istep + 1], pathv[istep]);
                        t = dis / Math3D::VectorLengthXZ(ftmp);
                    }
                    break;
                }
                distance -= dis;
                ftmp = pathv[istep];
                istep--;
                if (istep < 0) {
                    istep = 0;
                    t = 0.0f;
                    break;
                }
            }
        }
    }
    return istep + t;
}
float CPathVector::getTargetToPathStep(const FVECTOR& tpos) {
    FVECTOR vN;
    float fDis;
    float fT;
    int iStep;
    INT iMax = m_iVecMax;
    getNearPath(vN, &fDis, &fT, &iStep, tpos, m_pVec, iMax, 100.0f);
    if (iStep < 0) {
        fT = 0.0f;
        iStep = 0;
    } else if (iStep >= iMax - 1) {
        iStep = iMax - 2;
        fT = 1.0f;
    }
    return iStep + fT;
}
float CPathVector::getPathStepToVectorIdx4(int* pv, float fstep) {
    INT iMax = m_iVecMax;
    int step[4];
    FVECTOR vF;
    IVECTOR vI;
    PATH_PARAM sParam;
    getPathParam(sParam);
    vF.x = fstep;
    FTOI15Vector(vI, vF);
    INT iStep = vI.x >> 15;
    vI.x &= 0x7fff;
    ITOF15Vector(vF, vI);
    float t = vF.x;
    if (sParam.iFlags & 0x40000000) {
        if (iStep > iMax - 1) {
            t = 1.0f;
            iStep = iMax - 1;
        } else if (iStep < 0) {
            t = 0.0f;
            iStep = 0;
        }
        step[0] = iStep - 1;
        step[1] = iStep;
        step[2] = iStep + 1;
        step[3] = iStep + 2;
        for (INT i = 0; i < 4; i++) {
            if (step[i] < 0) pv[i] = step[i] + iMax;
            else if (step[i] > iMax - 1) pv[i] = step[i] - iMax;
            else pv[i] = step[i];
        }
    } else {
        if (iStep > iMax - 2) {
            t = 1.0f;
            iStep = iMax - 2;
        } else if (iStep < 0) {
            t = 0.0f;
            iStep = 0;
        }
        step[0] = iStep - 1;
        step[1] = iStep;
        step[2] = iStep + 1;
        step[3] = iStep + 2;
        for (INT i = 0; i < 4; i++) {
            if (step[i] < 0) pv[i] = -1;
            else if (step[i] > iMax - 1) pv[i] = -1;
            else pv[i] = step[i];
        }
    }
    return t;
}
float CPathVector::getPathStepToVector4(const FVECTOR** pv, float fstep) {
    int idx[4];
    FVECTOR* pVec = m_pVec;
    float fRet = getPathStepToVectorIdx4(idx, fstep);
    for (int i = 0; i < 4; i++) {
        if (idx[i] < 0) pv[i] = NULL;
        else pv[i] = &pVec[idx[i]];
    }
    return fRet;
}
void CPathVector::getPathStepToPosition(FVECTOR& mpos, float fstep, BOOL iTcb) {
    const FVECTOR* pv[4];
    FMATRIX m;
    float t = getPathStepToVector4(pv, fstep);
    if (!iTcb) {
        Math3D::HermiteMatrix(m, pv[0], pv[1], pv[2], pv[3], 0.5f);
    } else {
        Math3D::HermiteTCBMatrix(m, pv[0], pv[1], pv[2], pv[3], 0.5f);
    }
    Math3D::HermiteInterpolation(mpos, m, t);
}
FLOAT CPathVector::getPathStepToRoll(FLOAT fStep) {
    int idx[4];
    FLOAT fRoll[2];
    float t = getPathStepToVectorIdx4(idx, fStep);
    if (idx[1] >= 0) {
        if (idx[2] >= 0) {
            fRoll[0] = getPathRoll(idx[1]);
            fRoll[1] = getPathRoll(idx[2]);
            if (_fabsf(fRoll[0] - fRoll[1]) > 3.1415927f) {
                if (fRoll[1] < 0.0f) fRoll[1] += 6.2831855f;
                else fRoll[1] -= 6.2831855f;
            }
            return Math3D::LimitPI(fRoll[0] + (fRoll[1] - fRoll[0]) * t);
        }
        return getPathRoll(idx[1]);
    }
    if (idx[2] < 0) return 0.0f;
    return getPathRoll(idx[2]);
}
KH_DEBUG(void CPathVector::setPathParam(PATH_PARAM& sParam) {)
    KH_DEBUG(if (m_iStatus & 1) m_sParam = sParam;)
KH_DEBUG(})
KH_DEBUG(void CPathVector::__addPathAlloc(FVECTOR*& pVec, INT iVecMax, INT iVecMemMax) {)
#line 1098
    KH_DEBUG(FVECTOR* p = new KH_AT("(1098)") FVECTOR[iVecMemMax];)
    KH_DEBUG(memcpy(p, pVec, iVecMax * sizeof(FVECTOR));)
    KH_DEBUG(if (pVec) com_free(pVec);)
    KH_DEBUG(pVec = p;)
KH_DEBUG(})
KH_DEBUG(void CPathVector::__addPathCopy(FVECTOR*& pVec, INT iVecMax, INT iNum) {)
    KH_DEBUG(iVecMax--;)
    KH_DEBUG(memmove(&pVec[iNum + 2], &pVec[iNum + 1], (iVecMax - iNum) * sizeof(FVECTOR));)
KH_DEBUG(})
KH_DEBUG(void CPathVector::__delPathCopy(FVECTOR*& pVec, INT iVecMax, INT iNum) {)
    KH_DEBUG(iVecMax--;)
    KH_DEBUG(memcpy(&pVec[iNum], &pVec[iNum + 1], (iVecMax - iNum) * sizeof(FVECTOR));)
KH_DEBUG(})
KH_DEBUG(BOOL CPathVector::addPath(INT iNum) {)
    KH_DEBUG(FVECTOR vTmp;)
    KH_DEBUG(if (m_iStatus & 1) {)
        KH_DEBUG(if (iNum < 0) return FALSE;)
        KH_DEBUG(if (iNum < m_iVecMax) {)
            KH_DEBUG(if (m_iVecMemMax < m_iVecMax + 1) {)
                KH_DEBUG(m_iVecMemMax += 8;)
#line 1121
                KH_DEBUG(FVECTOR* p = new KH_AT("(1121)") FVECTOR[m_iVecMemMax];)
                KH_DEBUG(memcpy(p, m_pVec, m_iVecMax * sizeof(FVECTOR));)
                KH_DEBUG(if (m_pVec) com_free(m_pVec);)
                KH_DEBUG(m_pVec = p;)
                KH_DEBUG(if (m_pCube) delete[] m_pCube;)
                KH_DEBUG(m_pCube = NULL;)
#line 1131
                KH_DEBUG(m_pCube = new KH_AT("(1131)") CDbgCube[m_iVecMemMax];)
                KH_DEBUG(for (INT i = 0; i < m_iVecMemMax; i++) {)
                    KH_DEBUG(m_pCube[i].setCoord(NULL, NULL);)
                KH_DEBUG(})
            KH_DEBUG(})
            KH_DEBUG(INT iVecMax = m_iVecMax;)
            KH_DEBUG(iVecMax--;)
            KH_DEBUG(memmove(&m_pVec[iNum + 2], &m_pVec[iNum + 1], (iVecMax - iNum) * sizeof(FVECTOR));)
            KH_DEBUG(m_iVecMax++;)
            KH_DEBUG(if (iNum + 2 >= m_iVecMax) {)
                KH_DEBUG(if (iNum - 1 < 0) {)
                    KH_DEBUG(vTmp = m_pVec[iNum];)
                    KH_DEBUG(vTmp.z += 2.0f;)
                KH_DEBUG(} else {)
                    KH_DEBUG(SubVector(vTmp, m_pVec[iNum], m_pVec[iNum - 1]);)
                    KH_DEBUG(Normalize(vTmp, vTmp);)
                    KH_DEBUG(ScaleVector(vTmp, vTmp, 2.0f);)
                    KH_DEBUG(AddVector(vTmp, vTmp, m_pVec[iNum]);)
                    KH_DEBUG(vTmp.y = m_pVec[iNum].y;)
                KH_DEBUG(})
            KH_DEBUG(} else {)
                KH_DEBUG(vTmp = m_pVec[iNum + 2];)
            KH_DEBUG(})
            KH_DEBUG(AddVectorXYZ(m_pVec[iNum + 1], vTmp, m_pVec[iNum]);)
            KH_DEBUG(ScaleVectorXYZ(m_pVec[iNum + 1], m_pVec[iNum + 1], 0.5f);)
            KH_DEBUG(m_pVec[iNum + 1].w = 0.0f;)
            KH_DEBUG(for (INT i = 0; i < m_iVecMax; i++) {)
                KH_DEBUG(m_pCube[i].setCoord(&m_pVec[i], NULL);)
            KH_DEBUG(})
            KH_DEBUG(return TRUE;)
        KH_DEBUG(})
        KH_DEBUG(return FALSE;)
    KH_DEBUG(})
    KH_DEBUG(return FALSE;)
KH_DEBUG(})
KH_DEBUG(BOOL CPathVector::delPath(INT iNum) {)
    KH_DEBUG(if (m_iStatus & 1) {)
        KH_DEBUG(if (iNum < 0) return FALSE;)
        KH_DEBUG(if (iNum < m_iVecMax) {)
            KH_DEBUG(INT iVecMax = m_iVecMax;)
            KH_DEBUG(if (iVecMax < 2) return FALSE;)
            KH_DEBUG(iVecMax--;)
            KH_DEBUG(memcpy(&m_pVec[iNum], &m_pVec[iNum + 1], (iVecMax - iNum) * sizeof(FVECTOR));)
            KH_DEBUG(m_iVecMax--;)
            KH_DEBUG(for (INT i = 0; i < m_iVecMax; i++) {)
                KH_DEBUG(m_pCube[i].setCoord(&m_pVec[i], NULL);)
            KH_DEBUG(})
            KH_DEBUG(return TRUE;)
        KH_DEBUG(})
        KH_DEBUG(return FALSE;)
    KH_DEBUG(})
    KH_DEBUG(return FALSE;)
KH_DEBUG(})
KH_DEBUG(void CPathVector::dispDebugLine(INT iFlg) {)
    KH_DEBUG(m_vRot.y = Math3D::LimitPI(m_vRot.y + 0.034906585f);)
    KH_DEBUG(for (INT i = 0; i < m_iVecMax; i++) {)
        KH_DEBUG(if (iFlg && m_iActive == i) {)
            KH_DEBUG(m_pCube[i].setCoord(&m_pVec[i], &m_vRot);)
            KH_DEBUG(m_DDXYZCoord.m_Local.setTrans(m_pCube[i].getCoord().m_Local.v[3]);)
        KH_DEBUG(} else {)
            KH_DEBUG(m_pCube[i].setCoord(&m_pVec[i], NULL);)
        KH_DEBUG(})
        KH_DEBUG(m_pCube[i].disp();)
    KH_DEBUG(})
    KH_DEBUG(if (iFlg) {)
        KH_DEBUG(m_DDXYZCoord.touch();)
        KH_DEBUG(m_DDXYZCoord.fix(CMainApp::getTickCount());)
        KH_DEBUG(m_DDXYZLines.setCoord(&m_DDXYZCoord);)
        KH_DEBUG(m_DDXYZLines.disp();)
        KH_DEBUG(m_DDPathPolys.disp();)
    KH_DEBUG(} else {)
        KH_DEBUG(m_DDPathPolys.m_vVERT.clear();)
    KH_DEBUG(})
    KH_DEBUG(if (!m_pVec) return;)
    KH_DEBUG(if (m_iVecMax < 2) return;)
    KH_DEBUG(if (iFlg) {)
        KH_DEBUG(m_DDPathLines.setSemiTrans(ALPHA_OFF);)
    KH_DEBUG(} else {)
        KH_DEBUG(m_DDPathLines.setSemiTrans(ALPHA_BLEND);)
    KH_DEBUG(})
    KH_DEBUG(if ((m_iStatus & 2) || iFlg) {)
        KH_DEBUG(INT i;)
        KH_DEBUG(INT j;)
        KH_DEBUG(INT ib;)
        KH_DEBUG(FLOAT t;)
        KH_DEBUG(FVECTOR vPos;)
        KH_DEBUG(FVECTOR* pPath[4];)
        KH_DEBUG(FMATRIX mHermite;)
        KH_DEBUG(FVECTOR vHermite[32];)
        KH_DEBUG(G_STRIP Line;)
        KH_DEBUG(PATH_PARAM sParam;)
        KH_DEBUG(INT iLoopNum;)
        KH_DEBUG(m_iStatus &= ~2;)
        KH_DEBUG(m_DDPathLines.m_vVERT.clear();)
        KH_DEBUG(getPathParam(sParam);)
        KH_DEBUG(Line.C.col = 0x200000FF;)
        KH_DEBUG(Line.Term = 0;)
        KH_DEBUG(for (i = 0; i < m_iVecMax; i++) {)
            KH_DEBUG(if (i == 0) {)
                KH_DEBUG(Line.P = m_pVec[i];)
                KH_DEBUG(Line.P.w = 1.0f;)
                KH_DEBUG(m_DDPathLines.m_vVERT.push_back(Line);)
            KH_DEBUG(} else {)
                KH_DEBUG(ib = i - 1;)
                KH_DEBUG(SubVectorXYZ(vPos, m_pVec[i], m_pVec[ib]);)
                KH_DEBUG(t = 0.125f;)
                KH_DEBUG(for (j = 0; j < 8; j++) {)
                    KH_DEBUG(ScaleVectorXYZ(Line.P, vPos, t);)
                    KH_DEBUG(AddVectorXYZ(Line.P, m_pVec[ib], Line.P);)
                    KH_DEBUG(Line.P.w = 1.0f;)
                    KH_DEBUG(m_DDPathLines.m_vVERT.push_back(Line);)
                    KH_DEBUG(t += 0.125f;)
                KH_DEBUG(})
            KH_DEBUG(})
        KH_DEBUG(})
        KH_DEBUG(if (sParam.iFlags & 0x40000000) {)
            KH_DEBUG(ib = m_iVecMax - 1;)
            KH_DEBUG(SubVectorXYZ(vPos, m_pVec[0], m_pVec[ib]);)
            KH_DEBUG(t = 0.125f;)
            KH_DEBUG(for (j = 0; j < 8; j++) {)
                KH_DEBUG(ScaleVectorXYZ(Line.P, vPos, t);)
                KH_DEBUG(AddVectorXYZ(Line.P, m_pVec[ib], Line.P);)
                KH_DEBUG(Line.P.w = 1.0f;)
                KH_DEBUG(m_DDPathLines.m_vVERT.push_back(Line);)
                KH_DEBUG(t += 0.125f;)
            KH_DEBUG(})
        KH_DEBUG(})
        KH_DEBUG(m_DDPathLines.m_vVERT.back().Term = 1;)
        KH_DEBUG(t = 0.0f;)
        KH_DEBUG(for (i = 0; i < 16; i++) {)
            KH_DEBUG(Math3D::HermiteVector(vHermite[i], t);)
            KH_DEBUG(t += 1.0f / 15.0f;)
        KH_DEBUG(})
        KH_DEBUG(Line.C.col = 0x20FFFFFF;)
        KH_DEBUG(Line.Term = 0;)
        KH_DEBUG(if (sParam.iFlags & 0x40000000) {)
            KH_DEBUG(iLoopNum = m_iVecMax;)
        KH_DEBUG(} else {)
            KH_DEBUG(iLoopNum = m_iVecMax - 1;)
        KH_DEBUG(})
        KH_DEBUG(for (i = 0; i < iLoopNum; i++) {)
            KH_DEBUG(for (j = 0; j < 4; j++) {)
                KH_DEBUG(if (i - 1 + j < 0) {)
                    KH_DEBUG(if (sParam.iFlags & 0x40000000) pPath[j] = &m_pVec[i - 1 + j + m_iVecMax];)
                    KH_DEBUG(else pPath[j] = NULL;)
                KH_DEBUG(} else if (i - 1 + j >= m_iVecMax) {)
                    KH_DEBUG(if (sParam.iFlags & 0x40000000) pPath[j] = &m_pVec[i - 1 + j - m_iVecMax];)
                    KH_DEBUG(else pPath[j] = NULL;)
                KH_DEBUG(} else {)
                    KH_DEBUG(pPath[j] = &m_pVec[i - 1 + j];)
                KH_DEBUG(})
            KH_DEBUG(})
            KH_DEBUG(Math3D::HermiteTCBMatrix(mHermite, pPath[0], pPath[1], pPath[2], pPath[3], 0.5f);)
            KH_DEBUG(for (j = 0; j < 16; j++) {)
                KH_DEBUG(ApplyMatrix(vPos, mHermite, vHermite[j]);)
                KH_DEBUG(vPos.w = 1.0f;)
                KH_DEBUG(Line.P = vPos;)
                KH_DEBUG(m_DDPathLines.m_vVERT.push_back(Line);)
            KH_DEBUG(})
        KH_DEBUG(})
        KH_DEBUG(m_DDPathLines.m_vVERT.back().Term = 1;)
    KH_DEBUG(})
    KH_DEBUG(m_DDPathLines.disp();)
KH_DEBUG(})
CPathData::~CPathData() {
    if (m_pPath) {
        for (INT i = 0; i < m_iPathMax; i++) {
            m_pPath[i].~CPathVector();
        }
        com_free(m_pPath);
        m_pPath = NULL;
    }
}
void CPathData::init(const PATH_DATA* pPathData, INT iMode) { _init(pPathData, iMode); }
void CPathData::init(const CHAR* pszInName, INT iMode) {
#line 1402
    ASSERT(pszInName);
    _init((const PATH_DATA*)CRsrcDB::getRsrcPTD(pszInName), iMode);
}
void CPathData::_init(const PATH_DATA* pPathData, BOOL bAlloc) {
    INT iPathMax = 0;
    const PATH_VECTOR* pPath = NULL;
    m_pPathData = (PATH_DATA*)pPathData;
    if (pPathData) {
        if (pPathData->sHead.iId != 0x445450) {
            TRACE("\x83\x70\x83\x58\x83\x66\x81\x5b\x83\x5e\x82\xc5\x82\xcd\x97\x4c\x82\xe8\x82\xdc\x82\xb9\x82\xf1\n");
        } else {
            iPathMax = pPathData->sHead.iPathNum;
            pPath = &pPathData->sPath;
        }
    }
    KH_DEBUG(m_iActive = 0;)
    KH_DEBUG(m_iStatus = 0;)
    KH_DEBUG(if (bAlloc) {)
        KH_DEBUG(if (iPathMax <= 0) iPathMax = 1;)
        KH_DEBUG(m_iStatus = 1;)
        KH_DEBUG(m_iPathMemMax = iPathMax;)
    KH_DEBUG(} else {)
#line 1435
        ASSERT(pPathData);
        ASSERT(!(iPathMax <= 0));
    KH_DEBUG(})
    m_iPathMax = iPathMax;
#line 1441
    m_pPath = (CPathVector*)com_malloc_d(iPathMax * sizeof(CPathVector), __FILE__ "(1441)", (UINT)-1);
    for (INT i = 0; i < m_iPathMax; i++) {
        new (&m_pPath[i]) CPathVector;
        m_pPath[i].init(pPath, bAlloc);
        KH_DEBUG(if (pPath)) pPath = (const PATH_VECTOR*)((BYTE*)pPathData + pPath->sParam.iNext);
    }
    KH_DEBUG(CTask* pMap = CTaskMan::getSysTask(2);)
    KH_DEBUG(if (pMap) {)
        KH_DEBUG(m_DDLinkLines.setCoord(&pMap->Crd(0));)
    KH_DEBUG(} else {)
        KH_DEBUG(TRACE("\x83\x7d\x83\x62\x83\x76\x82\xaa\x82\xa0\x82\xe8\x82\xdc\x82\xb9\x82\xf1\n");)
    KH_DEBUG(})
}
INT CPathData::getLinkChild(INT iPathNum, INT iIdx) {
    PATH_PARAM sParam;
    INT i = iIdx + 1;
    if (i < 0) i = 0;
    for (; i < m_iPathMax; i++) {
        if (i == iPathNum) continue;
        m_pPath[i].getPathParam(sParam);
        if ((sParam.iFlags & 0x10) && sParam.iParentLink == iPathNum) break;
    }
    return i < m_iPathMax ? i : -1;
}
KH_DEBUG(BOOL CPathData::addPath(const FVECTOR* pVec, INT iNum) {)
    KH_DEBUG(FVECTOR vTmp;)
    KH_DEBUG(if (m_iStatus & 1) {)
        KH_DEBUG(if (m_iPathMemMax < m_iPathMax + 1) {)
            KH_DEBUG(m_iPathMemMax++;)
#line 1503
            KH_DEBUG(CPathVector* p = (CPathVector*)com_malloc_d(m_iPathMemMax * sizeof(CPathVector), __FILE__ "(1503)", (UINT)-1);)
            KH_DEBUG(memcpy(p, m_pPath, m_iPathMax * sizeof(CPathVector));)
            KH_DEBUG(com_free(m_pPath);)
            KH_DEBUG(m_pPath = p;)
        KH_DEBUG(})
        KH_DEBUG(if (pVec && iNum > 0) {)
            KH_DEBUG(new (&m_pPath[m_iPathMax]) CPathVector;)
            KH_DEBUG(m_pPath[m_iPathMax].init(pVec, iNum, 1);)
            KH_DEBUG(for (INT i = 0; i < m_pPath[m_iPathMax].getPathVectorNum(); i++) {)
                KH_DEBUG(vTmp = m_pPath[m_iPathMax].getPathPosition(i);)
                KH_DEBUG(vTmp.x += 1.0f;)
                KH_DEBUG(m_pPath[m_iPathMax].getPathVector()[i] = vTmp;)
            KH_DEBUG(})
        KH_DEBUG(} else {)
            KH_DEBUG(new (&m_pPath[m_iPathMax]) CPathVector;)
            KH_DEBUG(m_pPath[m_iPathMax].init(NULL, 1, 1);)
        KH_DEBUG(})
        KH_DEBUG(m_iPathMax++;)
        KH_DEBUG(return TRUE;)
    KH_DEBUG(})
    KH_DEBUG(return FALSE;)
KH_DEBUG(})
KH_DEBUG(BOOL CPathData::delPath(INT iNum) {)
    KH_DEBUG(PATH_PARAM sParam;)
    KH_DEBUG(if (m_iStatus & 1) {)
        KH_DEBUG(if (iNum < 0) return FALSE;)
        KH_DEBUG(if (iNum < m_iPathMax) {)
            KH_DEBUG(if (m_iPathMax < 2) return FALSE;)
            KH_DEBUG(for (INT i = 0; i < m_iPathMax; i++) {)
                KH_DEBUG(if (i == iNum) continue;)
                KH_DEBUG(m_pPath[i].getPathParam(sParam);)
                KH_DEBUG(if (sParam.iFlags & 0x10) {)
                    KH_DEBUG(if (sParam.iParentLink == iNum) {)
                        KH_DEBUG(sParam.iFlags &= ~0x10;)
                        KH_DEBUG(sParam.iParentLink = -1;)
                        KH_DEBUG(m_pPath[i].setPathParam(sParam);)
                    KH_DEBUG(} else if (iNum < sParam.iParentLink) {)
                        KH_DEBUG(sParam.iParentLink--;)
                        KH_DEBUG(m_pPath[i].setPathParam(sParam);)
                    KH_DEBUG(})
                KH_DEBUG(})
            KH_DEBUG(})
            KH_DEBUG(m_pPath[iNum].~CPathVector();)
            KH_DEBUG(INT iPathMax = m_iPathMax;)
            KH_DEBUG(iPathMax--;)
            KH_DEBUG(memcpy(&m_pPath[iNum], &m_pPath[iNum + 1], (iPathMax - iNum) * sizeof(CPathVector));)
            KH_DEBUG(m_iPathMax--;)
            KH_DEBUG(return TRUE;)
        KH_DEBUG(})
        KH_DEBUG(return FALSE;)
    KH_DEBUG(})
    KH_DEBUG(return FALSE;)
KH_DEBUG(})
static inline INT _max(INT a, INT b) { return a > b ? a : b; }

KH_DEBUG(void CPathData::dispDebugLine() {)
    KH_DEBUG(PATH_PARAM sParam;)
    KH_DEBUG(G_STRIP Line[2];)
    KH_DEBUG(FVECTOR* pv[2];)
    KH_DEBUG(m_DDLinkLines.m_vVERT.clear();)
    KH_DEBUG(for (INT i = 0; i < m_iPathMax; i++) {)
        KH_DEBUG(if (i == m_iActive) {)
            KH_DEBUG(m_pPath[i].dispDebugLine(1);)
        KH_DEBUG(} else {)
            KH_DEBUG(m_pPath[i].dispDebugLine(0);)
        KH_DEBUG(})
        KH_DEBUG(m_pPath[i].getPathParam(sParam);)
        KH_DEBUG(if (sParam.iFlags & 0x10) {)
            KH_DEBUG(Line[0].C.col = 0x40002020;)
            KH_DEBUG(Line[0].Term = 0;)
            KH_DEBUG(Line[1].C.col = 0x4000ffff;)
            KH_DEBUG(Line[1].Term = 1;)
            KH_DEBUG(INT n0 = m_pPath[i].getPathVectorNum();)
            KH_DEBUG(INT n1 = m_pPath[sParam.iParentLink].getPathVectorNum();)
            KH_DEBUG(pv[0] = m_pPath[i].getPathVector();)
            KH_DEBUG(pv[1] = m_pPath[sParam.iParentLink].getPathVector();)
            KH_DEBUG(INT n = _max(n1, n0);)
            KH_DEBUG(for (INT j = 0; j < n; j++) {)
                KH_DEBUG(INT k0 = j;)
                KH_DEBUG(if (k0 >= n0) k0 = n0 - 1;)
                KH_DEBUG(Line[0].P = pv[0][k0];)
                KH_DEBUG(Line[0].P.w = 1.0f;)
                KH_DEBUG(INT k1 = j;)
                KH_DEBUG(if (k1 >= n1) k1 = n1 - 1;)
                KH_DEBUG(Line[1].P = pv[1][k1];)
                KH_DEBUG(Line[1].P.w = 1.0f;)
                KH_DEBUG(m_DDLinkLines.m_vVERT.push_back(Line[0]);)
                KH_DEBUG(m_DDLinkLines.m_vVERT.push_back(Line[1]);)
            KH_DEBUG(})
        KH_DEBUG(})
    KH_DEBUG(})
    KH_DEBUG(m_DDLinkLines.CGPrim::disp();)
KH_DEBUG(})
KH_DEBUG(CDbgCube::CDbgCube() { init(NULL); })

KH_DEBUG(CDbgCube::CDbgCube(CCoord* pCrd) { init(pCrd); })

KH_DEBUG(CDbgCube::~CDbgCube() {})

KH_DEBUG(void CDbgCube::init(CCoord* pCrd) {)
    KH_DEBUG(static FVECTOR vert[6][4] = {)
        KH_DEBUG({{0.05f, -0.05f, 0.05f, 1.0f}, {-0.05f, -0.05f, 0.05f, 1.0f}, {0.05f, 0.05f, 0.05f, 1.0f}, {-0.05f, 0.05f, 0.05f, 1.0f}},)
        KH_DEBUG({{-0.05f, -0.05f, 0.05f, 1.0f}, {-0.05f, -0.05f, -0.05f, 1.0f}, {-0.05f, 0.05f, 0.05f, 1.0f}, {-0.05f, 0.05f, -0.05f, 1.0f}},)
        KH_DEBUG({{-0.05f, -0.05f, -0.05f, 1.0f}, {0.05f, -0.05f, -0.05f, 1.0f}, {-0.05f, 0.05f, -0.05f, 1.0f}, {0.05f, 0.05f, -0.05f, 1.0f}},)
        KH_DEBUG({{0.05f, -0.05f, -0.05f, 1.0f}, {0.05f, -0.05f, 0.05f, 1.0f}, {0.05f, 0.05f, -0.05f, 1.0f}, {0.05f, 0.05f, 0.05f, 1.0f}},)
        KH_DEBUG({{-0.05f, -0.05f, 0.05f, 1.0f}, {0.05f, -0.05f, 0.05f, 1.0f}, {-0.05f, -0.05f, -0.05f, 1.0f}, {0.05f, -0.05f, -0.05f, 1.0f}},)
        KH_DEBUG({{-0.05f, 0.05f, -0.05f, 1.0f}, {0.05f, 0.05f, -0.05f, 1.0f}, {-0.05f, 0.05f, 0.05f, 1.0f}, {0.05f, 0.05f, 0.05f, 1.0f}},)
    KH_DEBUG(};)
    KH_DEBUG(static unsigned int col[6] = {0x80ff0000, 0x80000040, 0x80400000, 0x800000ff, 0x80004000, 0x8000ff00};)
    KH_DEBUG(G_STRIP Line;)
    KH_DEBUG(m_gpPolyStrip.setShading(FALSE);)
    KH_DEBUG(m_gpPolyStrip.setBackClip(TRUE);)
    KH_DEBUG(for (INT i = 0; i < 6; i++) {)
        KH_DEBUG(Line.C.col = col[i];)
        KH_DEBUG(Line.Term = 0;)
        KH_DEBUG(for (INT j = 0; j < 4; j++) {)
            KH_DEBUG(Line.P = vert[i][j];)
            KH_DEBUG(m_gpPolyStrip.m_vVERT.push_back(Line);)
        KH_DEBUG(})
        KH_DEBUG(m_gpPolyStrip.m_vVERT.back().Term = 1;)
    KH_DEBUG(})
    KH_DEBUG(if (pCrd) m_gpPolyStrip.setCoord(pCrd);)
    KH_DEBUG(else m_gpPolyStrip.setCoord(&m_Coord);)
    KH_DEBUG(vu0UnitMatrix(m_Coord.m_Local);)
    KH_DEBUG(m_Coord.touch();)
KH_DEBUG(})

KH_DEBUG(void CDbgCube::setCoord(FVECTOR* pvPos, FVECTOR* pvRot) {)
    KH_DEBUG(FVECTOR vPos;)
    KH_DEBUG(if (!pvPos) {)
        KH_DEBUG(vPos.init0();)
    KH_DEBUG(} else {)
        KH_DEBUG(vPos = *pvPos;)
    KH_DEBUG(})
    KH_DEBUG(vPos.w = 1.0f;)
    KH_DEBUG(vu0UnitMatrix(m_Coord.m_Local);)
    KH_DEBUG(if (pvRot) {)
        KH_DEBUG(RotMatrixX(m_Coord.m_Local, m_Coord.m_Local, pvRot->x);)
        KH_DEBUG(RotMatrixY(m_Coord.m_Local, m_Coord.m_Local, pvRot->y);)
        KH_DEBUG(RotMatrixZ(m_Coord.m_Local, m_Coord.m_Local, pvRot->z);)
        KH_DEBUG(ScaleVectorXYZ(m_Coord.m_Local.v[0], m_Coord.m_Local.v[0], pvRot->w);)
        KH_DEBUG(ScaleVectorXYZ(m_Coord.m_Local.v[1], m_Coord.m_Local.v[1], pvRot->w);)
        KH_DEBUG(ScaleVectorXYZ(m_Coord.m_Local.v[2], m_Coord.m_Local.v[2], pvRot->w);)
    KH_DEBUG(})
    KH_DEBUG(m_Coord.m_Local.setTrans(vPos);)
KH_DEBUG(})

KH_DEBUG(void CDbgCube::disp() {)
    KH_DEBUG(m_Coord.touch();)
    KH_DEBUG(m_Coord.fix(CMainApp::getTickCount());)
    KH_DEBUG(m_gpPolyStrip.CGPrim::disp();)
KH_DEBUG(})
KH_DEBUG(CDbgArrow::CDbgArrow() { init(NULL); })

KH_DEBUG(CDbgArrow::CDbgArrow(CCoord* pCrd) { init(pCrd); })

KH_DEBUG(CDbgArrow::~CDbgArrow() {})

KH_DEBUG(void CDbgArrow::init(CCoord* pCrd) {)
    KH_DEBUG(static FVECTOR vert[10] = {)
        KH_DEBUG({0.0f, 0.0f, 0.2f, 1.0f}, {0.05f, 0.05f, 0.0f, 1.0f}, {0.05f, -0.05f, 0.0f, 1.0f}, {-0.05f, 0.05f, 0.0f, 1.0f},)
        KH_DEBUG({-0.05f, -0.05f, 0.0f, 1.0f}, {0.0f, 0.0f, 0.2f, 1.0f}, {0.05f, -0.05f, 0.0f, 1.0f}, {-0.05f, 0.05f, 0.0f, 1.0f},)
        KH_DEBUG({0.0f, 0.0f, 0.2f, 1.0f}, {0.05f, 0.05f, 0.0f, 1.0f},)
    KH_DEBUG(};)
    KH_DEBUG(static unsigned int col[10] = {0x80000000, 0x80000000, 0x80808080, 0x80404040, 0x80404040,)
                                   KH_DEBUG(0x80808080, 0x80101010, 0x80404040, 0x80404040, 0x80f0f0f0};)
    KH_DEBUG(G_STRIP Line;)
    KH_DEBUG(m_gpPolyStrip.setShading(FALSE);)
    KH_DEBUG(m_gpPolyStrip.setBackClip(TRUE);)
    KH_DEBUG(Line.Term = 0;)
    KH_DEBUG(for (INT i = 0; i < 10; i++) {)
        KH_DEBUG(Line.P = vert[i];)
        KH_DEBUG(Line.C.col = col[i];)
        KH_DEBUG(m_gpPolyStrip.m_vVERT.push_back(Line);)
    KH_DEBUG(})
    KH_DEBUG(m_gpPolyStrip.m_vVERT.back().Term = 1;)
    KH_DEBUG(if (pCrd) m_gpPolyStrip.setCoord(pCrd);)
    KH_DEBUG(else m_gpPolyStrip.setCoord(&m_Coord);)
    KH_DEBUG(vu0UnitMatrix(m_Coord.m_Local);)
    KH_DEBUG(m_Coord.touch();)
KH_DEBUG(})
KH_DEBUG(void CDbgArrow::setCoord(FVECTOR* pvPos, FVECTOR* pvRot) {)
    KH_DEBUG(FVECTOR vPos;)
    KH_DEBUG(FMATRIX m;)
    KH_DEBUG(QUATERNION q;)
    KH_DEBUG(FVECTOR vAxis;)
    KH_DEBUG(if (!pvPos) {)
        KH_DEBUG(vPos.init0();)
    KH_DEBUG(} else {)
        KH_DEBUG(vPos = *pvPos;)
    KH_DEBUG(})
    KH_DEBUG(vPos.w = 1.0f;)
    KH_DEBUG(vu0UnitMatrix(m_Coord.m_Local);)
    KH_DEBUG(if (pvRot) {)
        KH_DEBUG(RotMatrixX(m_Coord.m_Local, m_Coord.m_Local, pvRot->x);)
        KH_DEBUG(RotMatrixY(m_Coord.m_Local, m_Coord.m_Local, pvRot->y);)
        KH_DEBUG(if (pvRot->z != 0.0f) {)
            KH_DEBUG(vAxis.set(0.0f, 0.0f, 1.0f);)
            KH_DEBUG(ApplyMatrix33(vAxis, m_Coord.m_Local, vAxis);)
            KH_DEBUG(Quaternion::QuaternionRotationAxis(q, vAxis, pvRot->z);)
            KH_DEBUG(Quaternion::QuaternionToMatrix(m, q);)
            KH_DEBUG(MulMatrix(m_Coord.m_Local, m, m_Coord.m_Local);)
        KH_DEBUG(})
        KH_DEBUG(ScaleVectorXYZ(m_Coord.m_Local.v[0], m_Coord.m_Local.v[0], pvRot->w);)
        KH_DEBUG(ScaleVectorXYZ(m_Coord.m_Local.v[1], m_Coord.m_Local.v[1], pvRot->w);)
        KH_DEBUG(ScaleVectorXYZ(m_Coord.m_Local.v[2], m_Coord.m_Local.v[2], pvRot->w);)
    KH_DEBUG(})
    KH_DEBUG(m_Coord.m_Local.setTrans(vPos);)
KH_DEBUG(})
KH_DEBUG(void CDbgArrow::disp() {)
    KH_DEBUG(m_Coord.touch();)
    KH_DEBUG(m_Coord.fix(CMainApp::getTickCount());)
    KH_DEBUG(m_gpPolyStrip.CGPrim::disp();)
KH_DEBUG(})
// SOURCE DATA: .data .rodata
// SOURCE DATA SLUS_217.99_retail: .rodata
