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




#define ASSERT(e) if (!(e)) AssertApp(__FILE__, __LINE__, #e, 0)


CPathVector::~CPathVector() {
    if (m_iStatus & 1) {
        if (m_pVec) com_free(m_pVec);
        m_pVec = NULL;
    }
    if (m_pCube) delete[] m_pCube;
    m_pCube = NULL;
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
    CTask* pMap;
    G_STRIP Line;
    m_iStatus = 2;
    m_pVec = NULL;
    m_iActive = 0;
    if (bAlloc) {
        getPathParam(m_sParam);
        m_iVecMax = iPathMax;
        if (iPathMax <= 0) m_iVecMax = 1;
        m_iVecMemMax = m_iVecMax + 8;
#line 94
        m_pVec = new (__FILE__ "(94)") FVECTOR[m_iVecMemMax];
        ASSERT(m_pVec);
        if (pPath) {
            memcpy(m_pVec, pPath, m_iVecMax * sizeof(FVECTOR));
            if (m_pPathData && m_pPathData->sParam.iVer < 2) {
                for (INT i = 0; i < m_iVecMax; i++) {
                    FLOAT fRoll = m_pVec[i].w;
                    m_pVec[i].w = 0.0f;
                    _setPathVectorConvWRoll(&m_pVec[i].w, fRoll);
                    _setPathVectorConvWSpeed(&m_pVec[i].w, 0.0f);
                }
            }
        } else {
            for (INT i = 0; i < m_iVecMax; i++) {
                m_pVec[i].set(0.0f, 1.0f, (FLOAT)i, 0.0f);
            }
        }
        m_iStatus |= 1;
#line 122
        m_pCube = new (__FILE__ "(122)") CDbgCube[m_iVecMemMax];
        for (INT i = 0; i < m_iVecMemMax; i++) {
            m_pCube[i].setCoord(NULL, NULL);
        }
    } else {
#line 131
        ASSERT(pPath);
        ASSERT(!(iPathMax <= 0));
        m_pVec = (FVECTOR*)pPath;
        m_iVecMax = iPathMax;
#line 137
        m_pCube = new (__FILE__ "(137)") CDbgCube[iPathMax];
        for (INT i = 0; i < m_iVecMax; i++) {
            m_pCube[i].setCoord(&m_pVec[i], NULL);
        }
    }
    m_vRot.set(0.0f, 0.0f, 0.0f, 2.0f);
    pMap = CTaskMan::getSysTask(2);
    if (pMap) {
        m_DDPathLines.setCoord(&pMap->Crd(0));
        m_DDPathPolys.setCoord(&pMap->Crd(0));
        m_DDPathPolys.setSemiTrans(ALPHA_BLEND);
    } else {
        TRACE("\x83\x7d\x83\x62\x83\x76\x82\xaa\x82\xa0\x82\xe8\x82\xdc\x82\xb9\x82\xf1\n");
    }
    CCoord* pCrd = &m_DDXYZCoord;
    vu0UnitMatrix(pCrd->m_Local);
    pCrd->touch();
    m_DDXYZLines.setCoord(&m_DDXYZCoord);
    Line.Term = 0;
    for (INT i = 0; i <= 40; i++) {
        Line.P.set(0.0f, (FLOAT)(i - 30), 0.0f);
        Line.C.set(8, i * 5 + 0x37, 8, 0xff);
        if (i >= 40) Line.Term = 1;
        m_DDXYZLines.m_vVERT.push_back(Line);
    }
    Line.P.set(-1.0f, 0.0f, 0.0f);
    Line.C.set(0x40, 8, 8, 0xff);
    Line.Term = 0;
    m_DDXYZLines.m_vVERT.push_back(Line);
    Line.P.set(0.0f, 0.0f, 0.0f);
    Line.C.set(0xa0, 8, 8, 0xff);
    Line.Term = 0;
    m_DDXYZLines.m_vVERT.push_back(Line);
    Line.P.set(1.0f, 0.0f, 0.0f);
    Line.C.set(0xff, 8, 8, 0xff);
    Line.Term = 1;
    m_DDXYZLines.m_vVERT.push_back(Line);
    Line.P.set(0.0f, 0.0f, -1.0f);
    Line.C.set(8, 8, 0x40, 0xff);
    Line.Term = 0;
    m_DDXYZLines.m_vVERT.push_back(Line);
    Line.P.set(0.0f, 0.0f, 0.0f);
    Line.C.set(8, 0x28, 0xa0, 0xff);
    Line.Term = 0;
    m_DDXYZLines.m_vVERT.push_back(Line);
    Line.P.set(0.0f, 0.0f, 1.0f);
    Line.C.set(8, 0x48, 0xff, 0xff);
    Line.Term = 1;
    m_DDXYZLines.m_vVERT.push_back(Line);
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
    FLOAT fRoll;
    if (iNum >= m_iVecMax) iNum = m_iVecMax - 1;
    if (iNum < 0) iNum = 0;
    if ((m_iStatus & 1) || !m_pPathData || m_pPathData->sParam.iVer >= 2) {
        fRoll = _getPathVectorConvWRoll(m_pVec[iNum].w);
    } else {
        fRoll = m_pVec[iNum].w;
    }
    return fRoll;
}
FLOAT CPathVector::getPathSpeed(INT iNum) const {
    FLOAT fSpeed;
    if (iNum >= m_iVecMax) iNum = m_iVecMax - 1;
    if (iNum < 0) iNum = 0;
    if ((m_iStatus & 1) || !m_pPathData || m_pPathData->sParam.iVer >= 2) {
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
    if ((m_iStatus & 1) && &m_sParam != &sParam) {
        sParam = m_sParam;
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
    m_DDPathPolys.m_vVERT.clear();
    iNearStep = 0;
    fNearT = 0.0f;
    fNearDis = 1.7014118e+38f;
    fChkNearDis = 1.7014118e+38f;
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
                G_STRIP Line;
                Line.P = pv[1];
                Line.Term = 0;
                Line.C.col = 0x400000FF;
                Line.P.w = 1.0f;
                m_DDPathPolys.m_vVERT.push_back(Line);
                Line.P = v1[0];
                Line.C.col = 0x40808080;
                Line.P.w = 1.0f;
                m_DDPathPolys.m_vVERT.push_back(Line);
                Line.P = pv[2];
                Line.C.col = 0x40FF0000;
                Line.P.w = 1.0f;
                m_DDPathPolys.m_vVERT.push_back(Line);
                Line.P = v1[1];
                Line.C.col = 0x40808080;
                Line.P.w = 1.0f;
                m_DDPathPolys.m_vVERT.push_back(Line);
                m_DDPathPolys.m_vVERT.back().Term = 1;
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
void CPathVector::setPathParam(PATH_PARAM& sParam) {
    if (m_iStatus & 1) m_sParam = sParam;
}
void CPathVector::__addPathAlloc(FVECTOR*& pVec, INT iVecMax, INT iVecMemMax) {
#line 1098
    FVECTOR* p = new (__FILE__ "(1098)") FVECTOR[iVecMemMax];
    memcpy(p, pVec, iVecMax * sizeof(FVECTOR));
    if (pVec) com_free(pVec);
    pVec = p;
}
void CPathVector::__addPathCopy(FVECTOR*& pVec, INT iVecMax, INT iNum) {
    iVecMax--;
    memmove(&pVec[iNum + 2], &pVec[iNum + 1], (iVecMax - iNum) * sizeof(FVECTOR));
}
void CPathVector::__delPathCopy(FVECTOR*& pVec, INT iVecMax, INT iNum) {
    iVecMax--;
    memcpy(&pVec[iNum], &pVec[iNum + 1], (iVecMax - iNum) * sizeof(FVECTOR));
}
BOOL CPathVector::addPath(INT iNum) {
    FVECTOR vTmp;
    if (m_iStatus & 1) {
        if (iNum < 0) return FALSE;
        if (iNum < m_iVecMax) {
            if (m_iVecMemMax < m_iVecMax + 1) {
                m_iVecMemMax += 8;
#line 1121
                FVECTOR* p = new (__FILE__ "(1121)") FVECTOR[m_iVecMemMax];
                memcpy(p, m_pVec, m_iVecMax * sizeof(FVECTOR));
                if (m_pVec) com_free(m_pVec);
                m_pVec = p;
                if (m_pCube) delete[] m_pCube;
                m_pCube = NULL;
#line 1131
                m_pCube = new (__FILE__ "(1131)") CDbgCube[m_iVecMemMax];
                for (INT i = 0; i < m_iVecMemMax; i++) {
                    m_pCube[i].setCoord(NULL, NULL);
                }
            }
            INT iVecMax = m_iVecMax;
            iVecMax--;
            memmove(&m_pVec[iNum + 2], &m_pVec[iNum + 1], (iVecMax - iNum) * sizeof(FVECTOR));
            m_iVecMax++;
            if (iNum + 2 >= m_iVecMax) {
                if (iNum - 1 < 0) {
                    vTmp = m_pVec[iNum];
                    vTmp.z += 2.0f;
                } else {
                    SubVector(vTmp, m_pVec[iNum], m_pVec[iNum - 1]);
                    Normalize(vTmp, vTmp);
                    ScaleVector(vTmp, vTmp, 2.0f);
                    AddVector(vTmp, vTmp, m_pVec[iNum]);
                    vTmp.y = m_pVec[iNum].y;
                }
            } else {
                vTmp = m_pVec[iNum + 2];
            }
            AddVectorXYZ(m_pVec[iNum + 1], vTmp, m_pVec[iNum]);
            ScaleVectorXYZ(m_pVec[iNum + 1], m_pVec[iNum + 1], 0.5f);
            m_pVec[iNum + 1].w = 0.0f;
            for (INT i = 0; i < m_iVecMax; i++) {
                m_pCube[i].setCoord(&m_pVec[i], NULL);
            }
            return TRUE;
        }
        return FALSE;
    }
    return FALSE;
}
BOOL CPathVector::delPath(INT iNum) {
    if (m_iStatus & 1) {
        if (iNum < 0) return FALSE;
        if (iNum < m_iVecMax) {
            INT iVecMax = m_iVecMax;
            if (iVecMax < 2) return FALSE;
            iVecMax--;
            memcpy(&m_pVec[iNum], &m_pVec[iNum + 1], (iVecMax - iNum) * sizeof(FVECTOR));
            m_iVecMax--;
            for (INT i = 0; i < m_iVecMax; i++) {
                m_pCube[i].setCoord(&m_pVec[i], NULL);
            }
            return TRUE;
        }
        return FALSE;
    }
    return FALSE;
}
void CPathVector::dispDebugLine(INT iFlg) {
    m_vRot.y = Math3D::LimitPI(m_vRot.y + 0.034906585f);
    for (INT i = 0; i < m_iVecMax; i++) {
        if (iFlg && m_iActive == i) {
            m_pCube[i].setCoord(&m_pVec[i], &m_vRot);
            m_DDXYZCoord.m_Local.setTrans(m_pCube[i].getCoord().m_Local.v[3]);
        } else {
            m_pCube[i].setCoord(&m_pVec[i], NULL);
        }
        m_pCube[i].disp();
    }
    if (iFlg) {
        m_DDXYZCoord.touch();
        m_DDXYZCoord.fix(CMainApp::getTickCount());
        m_DDXYZLines.setCoord(&m_DDXYZCoord);
        m_DDXYZLines.disp();
        m_DDPathPolys.disp();
    } else {
        m_DDPathPolys.m_vVERT.clear();
    }
    if (!m_pVec) return;
    if (m_iVecMax < 2) return;
    if (iFlg) {
        m_DDPathLines.setSemiTrans(ALPHA_OFF);
    } else {
        m_DDPathLines.setSemiTrans(ALPHA_BLEND);
    }
    if ((m_iStatus & 2) || iFlg) {
        INT i;
        INT j;
        INT ib;
        FLOAT t;
        FVECTOR vPos;
        FVECTOR* pPath[4];
        FMATRIX mHermite;
        FVECTOR vHermite[32];
        G_STRIP Line;
        PATH_PARAM sParam;
        INT iLoopNum;
        m_iStatus &= ~2;
        m_DDPathLines.m_vVERT.clear();
        getPathParam(sParam);
        Line.C.col = 0x200000FF;
        Line.Term = 0;
        for (i = 0; i < m_iVecMax; i++) {
            if (i == 0) {
                Line.P = m_pVec[i];
                Line.P.w = 1.0f;
                m_DDPathLines.m_vVERT.push_back(Line);
            } else {
                ib = i - 1;
                SubVectorXYZ(vPos, m_pVec[i], m_pVec[ib]);
                t = 0.125f;
                for (j = 0; j < 8; j++) {
                    ScaleVectorXYZ(Line.P, vPos, t);
                    AddVectorXYZ(Line.P, m_pVec[ib], Line.P);
                    Line.P.w = 1.0f;
                    m_DDPathLines.m_vVERT.push_back(Line);
                    t += 0.125f;
                }
            }
        }
        if (sParam.iFlags & 0x40000000) {
            ib = m_iVecMax - 1;
            SubVectorXYZ(vPos, m_pVec[0], m_pVec[ib]);
            t = 0.125f;
            for (j = 0; j < 8; j++) {
                ScaleVectorXYZ(Line.P, vPos, t);
                AddVectorXYZ(Line.P, m_pVec[ib], Line.P);
                Line.P.w = 1.0f;
                m_DDPathLines.m_vVERT.push_back(Line);
                t += 0.125f;
            }
        }
        m_DDPathLines.m_vVERT.back().Term = 1;
        t = 0.0f;
        for (i = 0; i < 16; i++) {
            Math3D::HermiteVector(vHermite[i], t);
            t += 1.0f / 15.0f;
        }
        Line.C.col = 0x20FFFFFF;
        Line.Term = 0;
        if (sParam.iFlags & 0x40000000) {
            iLoopNum = m_iVecMax;
        } else {
            iLoopNum = m_iVecMax - 1;
        }
        for (i = 0; i < iLoopNum; i++) {
            for (j = 0; j < 4; j++) {
                if (i - 1 + j < 0) {
                    if (sParam.iFlags & 0x40000000) pPath[j] = &m_pVec[i - 1 + j + m_iVecMax];
                    else pPath[j] = NULL;
                } else if (i - 1 + j >= m_iVecMax) {
                    if (sParam.iFlags & 0x40000000) pPath[j] = &m_pVec[i - 1 + j - m_iVecMax];
                    else pPath[j] = NULL;
                } else {
                    pPath[j] = &m_pVec[i - 1 + j];
                }
            }
            Math3D::HermiteTCBMatrix(mHermite, pPath[0], pPath[1], pPath[2], pPath[3], 0.5f);
            for (j = 0; j < 16; j++) {
                ApplyMatrix(vPos, mHermite, vHermite[j]);
                vPos.w = 1.0f;
                Line.P = vPos;
                m_DDPathLines.m_vVERT.push_back(Line);
            }
        }
        m_DDPathLines.m_vVERT.back().Term = 1;
    }
    m_DDPathLines.disp();
}
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
    m_iActive = 0;
    m_iStatus = 0;
    if (bAlloc) {
        if (iPathMax <= 0) iPathMax = 1;
        m_iStatus = 1;
        m_iPathMemMax = iPathMax;
    } else {
#line 1435
        ASSERT(pPathData);
        ASSERT(!(iPathMax <= 0));
    }
    m_iPathMax = iPathMax;
#line 1441
    m_pPath = (CPathVector*)com_malloc_d(iPathMax * sizeof(CPathVector), __FILE__ "(1441)", (UINT)-1);
    for (INT i = 0; i < m_iPathMax; i++) {
        new (&m_pPath[i]) CPathVector;
        m_pPath[i].init(pPath, bAlloc);
        if (pPath) pPath = (const PATH_VECTOR*)((BYTE*)pPathData + pPath->sParam.iNext);
    }
    CTask* pMap = CTaskMan::getSysTask(2);
    if (pMap) {
        m_DDLinkLines.setCoord(&pMap->Crd(0));
    } else {
        TRACE("\x83\x7d\x83\x62\x83\x76\x82\xaa\x82\xa0\x82\xe8\x82\xdc\x82\xb9\x82\xf1\n");
    }
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
BOOL CPathData::addPath(const FVECTOR* pVec, INT iNum) {
    FVECTOR vTmp;
    if (m_iStatus & 1) {
        if (m_iPathMemMax < m_iPathMax + 1) {
            m_iPathMemMax++;
#line 1503
            CPathVector* p = (CPathVector*)com_malloc_d(m_iPathMemMax * sizeof(CPathVector), __FILE__ "(1503)", (UINT)-1);
            memcpy(p, m_pPath, m_iPathMax * sizeof(CPathVector));
            com_free(m_pPath);
            m_pPath = p;
        }
        if (pVec && iNum > 0) {
            new (&m_pPath[m_iPathMax]) CPathVector;
            m_pPath[m_iPathMax].init(pVec, iNum, 1);
            for (INT i = 0; i < m_pPath[m_iPathMax].getPathVectorNum(); i++) {
                vTmp = m_pPath[m_iPathMax].getPathPosition(i);
                vTmp.x += 1.0f;
                m_pPath[m_iPathMax].getPathVector()[i] = vTmp;
            }
        } else {
            new (&m_pPath[m_iPathMax]) CPathVector;
            m_pPath[m_iPathMax].init(NULL, 1, 1);
        }
        m_iPathMax++;
        return TRUE;
    }
    return FALSE;
}
BOOL CPathData::delPath(INT iNum) {
    PATH_PARAM sParam;
    if (m_iStatus & 1) {
        if (iNum < 0) return FALSE;
        if (iNum < m_iPathMax) {
            if (m_iPathMax < 2) return FALSE;
            for (INT i = 0; i < m_iPathMax; i++) {
                if (i == iNum) continue;
                m_pPath[i].getPathParam(sParam);
                if (sParam.iFlags & 0x10) {
                    if (sParam.iParentLink == iNum) {
                        sParam.iFlags &= ~0x10;
                        sParam.iParentLink = -1;
                        m_pPath[i].setPathParam(sParam);
                    } else if (iNum < sParam.iParentLink) {
                        sParam.iParentLink--;
                        m_pPath[i].setPathParam(sParam);
                    }
                }
            }
            m_pPath[iNum].~CPathVector();
            INT iPathMax = m_iPathMax;
            iPathMax--;
            memcpy(&m_pPath[iNum], &m_pPath[iNum + 1], (iPathMax - iNum) * sizeof(CPathVector));
            m_iPathMax--;
            return TRUE;
        }
        return FALSE;
    }
    return FALSE;
}
static inline INT _max(INT a, INT b) { return a > b ? a : b; }

void CPathData::dispDebugLine() {
    PATH_PARAM sParam;
    G_STRIP Line[2];
    FVECTOR* pv[2];
    m_DDLinkLines.m_vVERT.clear();
    for (INT i = 0; i < m_iPathMax; i++) {
        if (i == m_iActive) {
            m_pPath[i].dispDebugLine(1);
        } else {
            m_pPath[i].dispDebugLine(0);
        }
        m_pPath[i].getPathParam(sParam);
        if (sParam.iFlags & 0x10) {
            Line[0].C.col = 0x40002020;
            Line[0].Term = 0;
            Line[1].C.col = 0x4000ffff;
            Line[1].Term = 1;
            INT n0 = m_pPath[i].getPathVectorNum();
            INT n1 = m_pPath[sParam.iParentLink].getPathVectorNum();
            pv[0] = m_pPath[i].getPathVector();
            pv[1] = m_pPath[sParam.iParentLink].getPathVector();
            INT n = _max(n1, n0);
            for (INT j = 0; j < n; j++) {
                INT k0 = j;
                if (k0 >= n0) k0 = n0 - 1;
                Line[0].P = pv[0][k0];
                Line[0].P.w = 1.0f;
                INT k1 = j;
                if (k1 >= n1) k1 = n1 - 1;
                Line[1].P = pv[1][k1];
                Line[1].P.w = 1.0f;
                m_DDLinkLines.m_vVERT.push_back(Line[0]);
                m_DDLinkLines.m_vVERT.push_back(Line[1]);
            }
        }
    }
    m_DDLinkLines.CGPrim::disp();
}
CDbgCube::CDbgCube() { init(NULL); }

CDbgCube::CDbgCube(CCoord* pCrd) { init(pCrd); }

CDbgCube::~CDbgCube() {}

void CDbgCube::init(CCoord* pCrd) {
    static FVECTOR vert[6][4] = {
        {{0.05f, -0.05f, 0.05f, 1.0f}, {-0.05f, -0.05f, 0.05f, 1.0f}, {0.05f, 0.05f, 0.05f, 1.0f}, {-0.05f, 0.05f, 0.05f, 1.0f}},
        {{-0.05f, -0.05f, 0.05f, 1.0f}, {-0.05f, -0.05f, -0.05f, 1.0f}, {-0.05f, 0.05f, 0.05f, 1.0f}, {-0.05f, 0.05f, -0.05f, 1.0f}},
        {{-0.05f, -0.05f, -0.05f, 1.0f}, {0.05f, -0.05f, -0.05f, 1.0f}, {-0.05f, 0.05f, -0.05f, 1.0f}, {0.05f, 0.05f, -0.05f, 1.0f}},
        {{0.05f, -0.05f, -0.05f, 1.0f}, {0.05f, -0.05f, 0.05f, 1.0f}, {0.05f, 0.05f, -0.05f, 1.0f}, {0.05f, 0.05f, 0.05f, 1.0f}},
        {{-0.05f, -0.05f, 0.05f, 1.0f}, {0.05f, -0.05f, 0.05f, 1.0f}, {-0.05f, -0.05f, -0.05f, 1.0f}, {0.05f, -0.05f, -0.05f, 1.0f}},
        {{-0.05f, 0.05f, -0.05f, 1.0f}, {0.05f, 0.05f, -0.05f, 1.0f}, {-0.05f, 0.05f, 0.05f, 1.0f}, {0.05f, 0.05f, 0.05f, 1.0f}},
    };
    static unsigned int col[6] = {0x80ff0000, 0x80000040, 0x80400000, 0x800000ff, 0x80004000, 0x8000ff00};
    G_STRIP Line;
    m_gpPolyStrip.setShading(FALSE);
    m_gpPolyStrip.setBackClip(TRUE);
    for (INT i = 0; i < 6; i++) {
        Line.C.col = col[i];
        Line.Term = 0;
        for (INT j = 0; j < 4; j++) {
            Line.P = vert[i][j];
            m_gpPolyStrip.m_vVERT.push_back(Line);
        }
        m_gpPolyStrip.m_vVERT.back().Term = 1;
    }
    if (pCrd) m_gpPolyStrip.setCoord(pCrd);
    else m_gpPolyStrip.setCoord(&m_Coord);
    vu0UnitMatrix(m_Coord.m_Local);
    m_Coord.touch();
}

void CDbgCube::setCoord(FVECTOR* pvPos, FVECTOR* pvRot) {
    FVECTOR vPos;
    if (!pvPos) {
        vPos.init0();
    } else {
        vPos = *pvPos;
    }
    vPos.w = 1.0f;
    vu0UnitMatrix(m_Coord.m_Local);
    if (pvRot) {
        RotMatrixX(m_Coord.m_Local, m_Coord.m_Local, pvRot->x);
        RotMatrixY(m_Coord.m_Local, m_Coord.m_Local, pvRot->y);
        RotMatrixZ(m_Coord.m_Local, m_Coord.m_Local, pvRot->z);
        ScaleVectorXYZ(m_Coord.m_Local.v[0], m_Coord.m_Local.v[0], pvRot->w);
        ScaleVectorXYZ(m_Coord.m_Local.v[1], m_Coord.m_Local.v[1], pvRot->w);
        ScaleVectorXYZ(m_Coord.m_Local.v[2], m_Coord.m_Local.v[2], pvRot->w);
    }
    m_Coord.m_Local.setTrans(vPos);
}

void CDbgCube::disp() {
    m_Coord.touch();
    m_Coord.fix(CMainApp::getTickCount());
    m_gpPolyStrip.CGPrim::disp();
}
CDbgArrow::CDbgArrow() { init(NULL); }

CDbgArrow::CDbgArrow(CCoord* pCrd) { init(pCrd); }

CDbgArrow::~CDbgArrow() {}

void CDbgArrow::init(CCoord* pCrd) {
    static FVECTOR vert[10] = {
        {0.0f, 0.0f, 0.2f, 1.0f}, {0.05f, 0.05f, 0.0f, 1.0f}, {0.05f, -0.05f, 0.0f, 1.0f}, {-0.05f, 0.05f, 0.0f, 1.0f},
        {-0.05f, -0.05f, 0.0f, 1.0f}, {0.0f, 0.0f, 0.2f, 1.0f}, {0.05f, -0.05f, 0.0f, 1.0f}, {-0.05f, 0.05f, 0.0f, 1.0f},
        {0.0f, 0.0f, 0.2f, 1.0f}, {0.05f, 0.05f, 0.0f, 1.0f},
    };
    static unsigned int col[10] = {0x80000000, 0x80000000, 0x80808080, 0x80404040, 0x80404040,
                                   0x80808080, 0x80101010, 0x80404040, 0x80404040, 0x80f0f0f0};
    G_STRIP Line;
    m_gpPolyStrip.setShading(FALSE);
    m_gpPolyStrip.setBackClip(TRUE);
    Line.Term = 0;
    for (INT i = 0; i < 10; i++) {
        Line.P = vert[i];
        Line.C.col = col[i];
        m_gpPolyStrip.m_vVERT.push_back(Line);
    }
    m_gpPolyStrip.m_vVERT.back().Term = 1;
    if (pCrd) m_gpPolyStrip.setCoord(pCrd);
    else m_gpPolyStrip.setCoord(&m_Coord);
    vu0UnitMatrix(m_Coord.m_Local);
    m_Coord.touch();
}
void CDbgArrow::setCoord(FVECTOR* pvPos, FVECTOR* pvRot) {
    FVECTOR vPos;
    FMATRIX m;
    QUATERNION q;
    FVECTOR vAxis;
    if (!pvPos) {
        vPos.init0();
    } else {
        vPos = *pvPos;
    }
    vPos.w = 1.0f;
    vu0UnitMatrix(m_Coord.m_Local);
    if (pvRot) {
        RotMatrixX(m_Coord.m_Local, m_Coord.m_Local, pvRot->x);
        RotMatrixY(m_Coord.m_Local, m_Coord.m_Local, pvRot->y);
        if (pvRot->z != 0.0f) {
            vAxis.set(0.0f, 0.0f, 1.0f);
            ApplyMatrix33(vAxis, m_Coord.m_Local, vAxis);
            Quaternion::QuaternionRotationAxis(q, vAxis, pvRot->z);
            Quaternion::QuaternionToMatrix(m, q);
            MulMatrix(m_Coord.m_Local, m, m_Coord.m_Local);
        }
        ScaleVectorXYZ(m_Coord.m_Local.v[0], m_Coord.m_Local.v[0], pvRot->w);
        ScaleVectorXYZ(m_Coord.m_Local.v[1], m_Coord.m_Local.v[1], pvRot->w);
        ScaleVectorXYZ(m_Coord.m_Local.v[2], m_Coord.m_Local.v[2], pvRot->w);
    }
    m_Coord.m_Local.setTrans(vPos);
}
void CDbgArrow::disp() {
    m_Coord.touch();
    m_Coord.fix(CMainApp::getTickCount());
    m_gpPolyStrip.CGPrim::disp();
}
// SOURCE DATA: .data .rodata
