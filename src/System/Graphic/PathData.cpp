#define GPRIM_H_PATH "c:/KHCOM/MainApp/System/Graphic/Gprim.h"
#include "System/Graphic/PathData.h"
#include "System/Graphic/WorldEnv.h"
#include "System/Math3D.h"
#include "System/HeapMan.h"
#include <math.h>
#include <string.h>
#include "System/MainApp.h"
#include "System/TaskMan.h"
#include "include_asm.h"

static inline FLOAT _fabs(FLOAT inValue) {
    FLOAT ret;
    asm volatile("abs.s %0, %1" : "=f"(ret) : "f"(inValue));
    return ret;
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

static inline void _zeroVectorVu0(FVECTOR& v) { asm volatile("sqc2 $vf0, 0x0(%0)" : : "r"(&v) : "memory"); }

extern "C" void sceVu0RotMatrixX(FMATRIX& m0, const FMATRIX& m1, float rx);
extern "C" void sceVu0RotMatrixY(FMATRIX& m0, const FMATRIX& m1, float ry);
extern "C" void sceVu0RotMatrixZ(FMATRIX& m0, const FMATRIX& m1, float rz);
void ScaleVectorXYZ(FVECTOR& v0, const FVECTOR& v1, FLOAT f);
void SubVector(FVECTOR& v0, const FVECTOR& v1, const FVECTOR& v2);
void AddVector(FVECTOR& v0, const FVECTOR& v1, const FVECTOR& v2);
void ScaleVector(FVECTOR& v0, const FVECTOR& v1, FLOAT f);
void Normalize(FVECTOR& v0, const FVECTOR& v1);
void ApplyMatrix33(FVECTOR& v0, const FMATRIX& m0, const FVECTOR& v1);
void MulMatrix(FMATRIX& res, const FMATRIX& m0, const FMATRIX& m1);
void FTOI15Vector(IVECTOR& v0, const FVECTOR& v1);
void ITOF15Vector(FVECTOR& v0, const IVECTOR& v1);

inline void operator delete[](void* mem) { com_free(mem); }
#ifndef KH_PLACEMENT_NEW_DEFINED
#define KH_PLACEMENT_NEW_DEFINED
inline void* operator new(size_t, void* p) throw() { return p; }
#endif

/* strings of CPathVector::_init (INCLUDE_ASM) that CPathData::_init uses too */
extern "C" const char D_00462128[];
extern "C" const char D_00462178[];

// PARTIAL: System/RsrcDB.h would add RsrcData.h's string to .rodata
class CRsrcDB {
public:
    static BYTE* getRsrcPTD(const CHAR* pszName);
};

#define RET_ADDR() ({ UINT nRA; __asm__ volatile("daddu %0, $31, $0" : "=r"(nRA)); nRA; })
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
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/System/Graphic/PathData", _init__11CPathVectorPC7FVECTORii);
__asm__(".rdata\n.align 3\n.word 0x4B2F3A63\n.word 0x4D4F4348\n.word 0x69614D2F\n.word 0x7070416E\n.word 0x7379532F\n.word 0x2F6D6574\n.word 0x70617247\n.word 0x2F636968\n.word 0x68746150\n.word 0x61746144\n.word 0x7070632E\n.word 0x29343928\n.word 0x00000000\n.word 0x00000000\n.word 0x56705F6D\n.word 0x00006365\n.word 0x4B2F3A63\n.word 0x4D4F4348\n.word 0x69614D2F\n.word 0x7070416E\n.word 0x7379532F\n.word 0x2F6D6574\n.word 0x70617247\n.word 0x2F636968\n.word 0x68746150\n.word 0x61746144\n.word 0x7070632E\n.word 0x32323128\n.word 0x00000029\n.word 0x00000000\n.word 0x74615070\n.word 0x00000068\n.word 0x50692821\n.word 0x4D687461\n.word 0x3C207861\n.word 0x2930203D\n.word 0x00000000\n.word 0x00000000\n.word 0x4B2F3A63\n.word 0x4D4F4348\n.word 0x69614D2F\n.word 0x7070416E\n.word 0x7379532F\n.word 0x2F6D6574\n.word 0x70617247\n.word 0x2F636968\n.word 0x68746150\n.word 0x61746144\n.word 0x7070632E\n.word 0x37333128\n.word 0x00000029\n.word 0x00000000\n.word 0x62837D83\n.word 0xAA827683\n.word 0xE882A082\n.word 0xB982DC82\n.word 0x000AF182\n.word 0x00000000\n.text\n");
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/System/Graphic/PathData", makeVertexSpeedAll__11CPathVector);
__asm__(".rdata\n.align 3\n.word 0x20647053\n.word 0x252C6425\n.word 0x00000A66\n.word 0x00000000\n.word 0x2077654E\n.word 0x252C6425\n.word 0x00000A66\n.word 0x00000000\n.text\n");
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
/* placeholder: CVector<G_STRIP>'s assert strings come here (getNearPath, INCLUDE_ASM, uses operator[]) */
inline void _pathDataVectorStrings(G_STRIP_VECTOR& v) { v.back(); }

INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/System/Graphic/PathData", getNearPath__11CPathVectorR7FVECTORPfT2PiRC7FVECTORPC7FVECTORif);
__asm__(".rdata\n.word 0x7F000000\n.text\n");
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
            if (_fabs(fRoll[0] - fRoll[1]) > 3.1415927f) {
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
    FVECTOR* p = (FVECTOR*)com_malloc_d(iVecMemMax * sizeof(FVECTOR), __FILE__ "(1098)", RET_ADDR());
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
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/System/Graphic/PathData", addPath__11CPathVectori);
__asm__(".rdata\n.align 3\n.word 0x4B2F3A63\n.word 0x4D4F4348\n.word 0x69614D2F\n.word 0x7070416E\n.word 0x7379532F\n.word 0x2F6D6574\n.word 0x70617247\n.word 0x2F636968\n.word 0x68746150\n.word 0x61746144\n.word 0x7070632E\n.word 0x32313128\n.word 0x00002931\n.word 0x00000000\n.word 0x4B2F3A63\n.word 0x4D4F4348\n.word 0x69614D2F\n.word 0x7070416E\n.word 0x7379532F\n.word 0x2F6D6574\n.word 0x70617247\n.word 0x2F636968\n.word 0x68746150\n.word 0x61746144\n.word 0x7070632E\n.word 0x33313128\n.word 0x00002931\n.word 0x00000000\n.text\n");
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
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/System/Graphic/PathData", dispDebugLine__11CPathVectori);
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
        if (!(!(iPathMax <= 0))) AssertApp(__FILE__, __LINE__, (char*)D_00462128, 0);
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
        TRACE((char*)D_00462178);
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
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/System/Graphic/PathData", addPath__9CPathDataPC7FVECTORi);
__asm__(".rdata\n.align 3\n.word 0x4B2F3A63\n.word 0x4D4F4348\n.word 0x69614D2F\n.word 0x7070416E\n.word 0x7379532F\n.word 0x2F6D6574\n.word 0x70617247\n.word 0x2F636968\n.word 0x68746150\n.word 0x61746144\n.word 0x7070632E\n.word 0x30353128\n.word 0x00002933\n.word 0x00000000\n.text\n");
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
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/System/Graphic/PathData", dispDebugLine__9CPathData);
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
    m_gpPolyStrip.setIIP(FALSE);
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
    _unitMatrixVu0(m_Coord.m_Local);
    m_Coord.m_Flag = 1;
}

void CDbgCube::setCoord(FVECTOR* pvPos, FVECTOR* pvRot) {
    FVECTOR vPos;
    if (!pvPos) {
        _zeroVectorVu0(vPos);
    } else {
        vPos = *pvPos;
    }
    vPos.w = 1.0f;
    _unitMatrixVu0(m_Coord.m_Local);
    if (pvRot) {
        sceVu0RotMatrixX(m_Coord.m_Local, m_Coord.m_Local, pvRot->x);
        sceVu0RotMatrixY(m_Coord.m_Local, m_Coord.m_Local, pvRot->y);
        sceVu0RotMatrixZ(m_Coord.m_Local, m_Coord.m_Local, pvRot->z);
        ScaleVectorXYZ(m_Coord.m_Local.v[0], m_Coord.m_Local.v[0], pvRot->w);
        ScaleVectorXYZ(m_Coord.m_Local.v[1], m_Coord.m_Local.v[1], pvRot->w);
        ScaleVectorXYZ(m_Coord.m_Local.v[2], m_Coord.m_Local.v[2], pvRot->w);
    }
    m_Coord.m_Local.v[3] = vPos;
}

void CDbgCube::disp() {
    m_Coord.m_Flag = 1;
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
    m_gpPolyStrip.setIIP(FALSE);
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
    _unitMatrixVu0(m_Coord.m_Local);
    m_Coord.m_Flag = 1;
}
void CDbgArrow::setCoord(FVECTOR* pvPos, FVECTOR* pvRot) {
    FVECTOR vPos;
    FMATRIX m;
    QUATERNION q;
    FVECTOR vAxis;
    if (!pvPos) {
        _zeroVectorVu0(vPos);
    } else {
        vPos = *pvPos;
    }
    vPos.w = 1.0f;
    _unitMatrixVu0(m_Coord.m_Local);
    if (pvRot) {
        sceVu0RotMatrixX(m_Coord.m_Local, m_Coord.m_Local, pvRot->x);
        sceVu0RotMatrixY(m_Coord.m_Local, m_Coord.m_Local, pvRot->y);
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
    m_Coord.m_Local.v[3] = vPos;
}
void CDbgArrow::disp() {
    m_Coord.m_Flag = 1;
    m_Coord.fix(CMainApp::getTickCount());
    m_gpPolyStrip.CGPrim::disp();
}
// SOURCE DATA: .data .rodata
