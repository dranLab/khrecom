/* .rodata comes from the original assembly: the split ends it at 0x4893B4, in the middle of CSphereCollision's
 * vtable (Game/Collision/CollUtilO's data holds the rest and CCollision's vtable). The code refers to the
 * original's strings by their labels instead. */
#define COLLOBJ_H_COLLISION_CLASSES
#define COLLUTILA_H_INFO_MEMBERS
#include "khtypes.h"
union COLL_INFO_UNION {
    INT info;
    struct {
        BYTE infoID;
        BYTE infoATTR;
        BYTE infoPID;
        BYTE infoSTAT;
    };
};
#include "Game/Collision/CollUtilA.h"
#include "System/HeapMan.h"
#include "System/Debug.h"

extern char D_004891D0[]; /* "c:/KHCOM/MainApp/Game/Collision/CollUtil.h" */
extern char D_00489248[]; /* "m_pInstance" */

/* Game/Collision/CollUtil.h's accessor (line 194), with the original's strings */
inline CCollUtil* CCollUtil::getInstance() {
    CCollUtil*& instance = m_pInstance;
    if (!instance) AssertApp(D_004891D0, 194, D_00489248, 0);
    return instance;
}

#include "Game/Collision/CollObj.h"
#include "Game/Collision/Ride.h"
#include "include_asm.h"

extern char D_00489258[]; /* "c:/KHCOM/MainApp/Game/Collision/CollUtil.cpp(37)" */
extern char D_00489290[]; /* "CCollUtil:データフォーマットが異なっています\n" */
extern char D_004892C0[]; /* "c:/KHCOM/MainApp/Game/Collision/CollUtil.cpp(211)" */

#define RET_ADDR() ({ UINT nRA; __asm__ volatile("daddu %0, $31, $0" : "=r"(nRA)); nRA; })

inline void* operator new(size_t size, char* msg) throw() {
    return com_malloc_d(size, msg, RET_ADDR());
}
inline void* operator new[](size_t size, char* msg) throw() {
    return com_malloc_d(size, msg, RET_ADDR());
}
inline void operator delete[](void* mem) { com_free(mem); }

extern "C" void sceVu0UnitMatrix(FMATRIX& m);
extern "C" void sceVu0RotMatrixX(FMATRIX& m0, FMATRIX& m1, float rx);
extern "C" void sceVu0RotMatrixY(FMATRIX& m0, FMATRIX& m1, float ry);
extern "C" void sceVu0RotMatrixZ(FMATRIX& m0, FMATRIX& m1, float rz);
extern "C" void sceVu0TransMatrix(FMATRIX& m0, FMATRIX& m1, FVECTOR& tv);
extern "C" void sceVu0InversMatrix(FMATRIX& m0, FMATRIX& m1);
void MulMatrix(FMATRIX& m0, const FMATRIX& m1, const FMATRIX& m2);
static inline void _unitMatrix(FMATRIX& m) { sceVu0UnitMatrix(m); }
void SubVector(FVECTOR& v0, const FVECTOR& v1, const FVECTOR& v2);
void SubVectorXYZ(FVECTOR& v0, const FVECTOR& v1, const FVECTOR& v2);
FLOAT InnerProduct(const FVECTOR& v0);
FLOAT InnerProduct(const FVECTOR& v0, const FVECTOR& v1);
void ScaleVector(FVECTOR& v0, const FVECTOR& v1, FLOAT t);
void AddVector(FVECTOR& v0, const FVECTOR& v1, const FVECTOR& v2);
void Normalize(FVECTOR& v0, const FVECTOR& v1);
void OuterProduct(FVECTOR& v0, const FVECTOR& v1, const FVECTOR& v2);
void DivVector(FVECTOR& v0, const FVECTOR& v1, FLOAT q);

struct MCD_HEADER {
    unsigned char code[4];
    USHORT version;
    USHORT level;
    USHORT c_suu;
    USHORT m_suu;
    UINT total;
};

CCollUtil* CCollUtil::m_pInstance = __null;

CCollUtil::CCollUtil() {
    m_pCheckColl = new (D_00489258) CSphereCollision;
    m_pCheckColl->m_Kind = 0;
    m_pCheckColl->m_shiftY = 0.0f;
    m_pChipStatus = __null;
}

CCollUtil::~CCollUtil() {
    if (m_pCheckColl) delete m_pCheckColl;
    m_pCheckColl = __null;
    if (m_pChipStatus) delete[] m_pChipStatus;
    m_pChipStatus = __null;
}

void CCollUtil::setup() {
    m_pMCDInfo = __null;
    m_pCollDbg = __null;
}

void CCollUtil::dispose() {
    m_pMCDInfo = __null;
    m_pCollDbg = __null;
    if (m_pChipStatus) delete[] m_pChipStatus;
    m_pChipStatus = __null;
}

void CCollUtil::_initializeData(void* pDtHead, MCD_INFO* pMCDInfo) {
    BOOL fgIsObject;
    MC_CHIP* pChip;
    UINT ChipDataEnd;
    FVECTOR vec0;
    FVECTOR vec1;
    INT lv;
    INT i;

    if (!pDtHead) {
        return;
    }
    fgIsObject = FALSE;
    if (!pMCDInfo) {
        m_pMCDInfo = (MCD_INFO*)m_MCDInfo;
        pMCDInfo = (MCD_INFO*)m_MCDInfo;
    } else {
        fgIsObject = TRUE;
    }
    if (!fgIsObject) {
        if (m_pChipStatus) delete[] m_pChipStatus;
        m_pChipStatus = __null;
    }
    if (((MCD_HEADER*)pDtHead)->version == 3) {
        pMCDInfo->fMDVal = 3.0f;
        pMCDInfo->fMDVal2 = 10.0f;
        lv = 7;
        if (((MCD_HEADER*)pDtHead)->level < 8) {
            lv = ((MCD_HEADER*)pDtHead)->level;
        }
        if (lv < 4) {
            pMCDInfo->fMDVal = 3.0f;
            pMCDInfo->fMDVal2 = lv * 3.0f + 12.0f;
        } else {
            pMCDInfo->fMDVal = 4.0f;
            pMCDInfo->fMDVal2 = (lv - 4) * 4.0f + 16.0f;
        }
    } else {
        TRACE(D_00489290);
        return;
    }
    pMCDInfo->pMCData = (MC_DATA*)((BYTE*)pDtHead + sizeof(MCD_HEADER));
    pMCDInfo->pMCChip = (MC_CHIP*)(pMCDInfo->pMCData->total + (UINT)pMCDInfo->pMCData);
    pMCDInfo->iChipSuu = ((MCD_HEADER*)pDtHead)->c_suu;
    if (mappingData(pMCDInfo->pMCData)) {
        return;
    }
    pChip = pMCDInfo->pMCChip;
    ChipDataEnd = (UINT)(pChip + pMCDInfo->iChipSuu);
    for (i = 0; i < pMCDInfo->iChipSuu; i++, pChip++) {
        pChip->data = (BYTE*)((UINT)pChip->data + ChipDataEnd);
        if (pChip->flag) {
            mappingData((MC_DATA*)pChip->data);
        }
        recalcChipCollision(pChip);
    }
}

INT CCollUtil::mappingData(MC_DATA* pDtHead) {
    if ((UINT)pDtHead->tbl != 0x40) {
        return 1;
    }
    pDtHead->tbl = (USHORT(*)[2])((UINT)pDtHead->tbl + (UINT)pDtHead);
    pDtHead->d = (FLOAT*)((UINT)pDtHead->d + (UINT)pDtHead);
    pDtHead->norm = (FVECTOR*)((UINT)pDtHead->norm + (UINT)pDtHead);
    pDtHead->vert = (FVECTOR*)((UINT)pDtHead->vert + (UINT)pDtHead);
    pDtHead->prim = (USHORT*)((UINT)pDtHead->prim + (UINT)pDtHead);
    pDtHead->prim3 = (MC_PRIM*)((UINT)pDtHead->prim3 + (UINT)pDtHead);
    pDtHead->prim4 = (MC_PRIM*)((UINT)pDtHead->prim4 + (UINT)pDtHead);
    return 0;
}

void CCollUtil::recalcChipCollision(MC_CHIP* pChip) {
    FMATRIX mat0;
    FMATRIX mat1;

    _unitMatrix(mat0);
    _unitMatrix(mat1);
    mat0.m[0][0] = pChip->s.x;
    mat0.m[1][1] = pChip->s.y;
    mat0.m[2][2] = pChip->s.z;
    sceVu0RotMatrixX(mat0, mat0, pChip->r.x);
    sceVu0RotMatrixY(mat0, mat0, pChip->r.y);
    sceVu0RotMatrixZ(mat0, mat0, pChip->r.z);
    sceVu0TransMatrix(mat0, mat0, pChip->t);
    pChip->matLW = mat0;
    sceVu0InversMatrix(mat0, mat0);
    mat1.m[0][0] = 1.0f / (pChip->s.x * pChip->s.x);
    mat1.m[1][1] = 1.0f / (pChip->s.y * pChip->s.y);
    mat1.m[2][2] = 1.0f / (pChip->s.z * pChip->s.z);
    mat1.m[3][3] = 1.0f;
    MulMatrix(mat0, mat1, mat0);
    pChip->matWL = mat0;
}

void CCollUtil::_enableChipCollision(FVECTOR& vChipCenter, BOOL fg) {
    MC_CHIP* pChip;
    FVECTOR vec;
    INT i;

    if (!m_pMCDInfo) {
        return;
    }
    for (i = 0, pChip = m_pMCDInfo->pMCChip; i < m_pMCDInfo->iChipSuu; i++, pChip++) {
        SubVector(vec, pChip->t, vChipCenter);
        if (InnerProduct(vec) < 0.0001f) {
            pChip->id = fg ^ 1;
        }
    }
}

void CCollUtil::_backupChipStatus() {
    MC_CHIP* pChip;
    INT i;

    if (!m_pMCDInfo) {
        return;
    }
    if (m_pChipStatus) delete[] m_pChipStatus;
    m_pChipStatus = __null;
    m_pChipStatus = new (D_004892C0) UINT[m_pMCDInfo->iChipSuu];
    for (i = 0, pChip = m_pMCDInfo->pMCChip; i < m_pMCDInfo->iChipSuu; i++, pChip++) {
        m_pChipStatus[i] = pChip->id;
    }
}

void CCollUtil::_restoreChipStatus() {
    MC_CHIP* pChip;
    INT i;

    if (!m_pMCDInfo) {
        return;
    }
    if (m_pChipStatus) {
        for (i = 0, pChip = m_pMCDInfo->pMCChip; i < m_pMCDInfo->iChipSuu; i++, pChip++) {
            pChip->id = m_pChipStatus[i];
        }
        if (m_pChipStatus) delete[] m_pChipStatus;
        m_pChipStatus = __null;
    }
}

MC_CHIP* CCollUtil::_searchChipCollisionFirst(FVECTOR& vChipCenter, FLOAT fRange) {
    m_iChipSearchIndex = 0;
    m_vChipSearchPos = vChipCenter;
    m_vChipSearchPos.w = fRange;
    return _searchChipCollisionNext();
}

MC_CHIP* CCollUtil::_searchChipCollisionNext() {
    MC_CHIP* pChip;
    FLOAT rng;
    FVECTOR vec;

    if (!m_pMCDInfo) {
        return __null;
    }
    if (m_iChipSearchIndex >= m_pMCDInfo->iChipSuu) {
        return __null;
    }
    rng = m_vChipSearchPos.w * m_vChipSearchPos.w;
    pChip = &m_pMCDInfo->pMCChip[m_iChipSearchIndex];
    while (m_iChipSearchIndex < m_pMCDInfo->iChipSuu) {
        m_iChipSearchIndex++;
        SubVectorXYZ(vec, pChip->t, m_vChipSearchPos);
        if (InnerProduct(vec) < rng) {
            return pChip;
        }
        pChip++;
    }
    return __null;
}

INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Game/Collision/CollUtil", _detectOCD__9CCollUtilR10CCollisionP8MCD_INFOP8OCD_WORKi);
#if 0 /* the parked draft below is static, as in the original */
static INT __detectMCInitInit_Check(MC_DATA* pMCData, FLOAT fMDVal);
#else
INT __detectMCInitInit_Check(MC_DATA* pMCData, FLOAT fMDVal); /* MATCH: INCLUDE_ASM'd (parked): global label */
#endif
INT CCollUtil::__detectMCInitInit(MC_CHIP* pChip) {
    asm volatile("lqc2 $vf4, 0x30(%0)" : : "r"(this));
    asm volatile("lqc2 $vf8, 0x80(%0)\n"
                 "lqc2 $vf9, 0x90(%0)\n"
                 "lqc2 $vf10, 0xa0(%0)\n"
                 "lqc2 $vf11, 0xb0(%0)" : : "r"(pChip));
    asm volatile("vmulax.xyzw $ACC, $vf8, $vf4x\n"
                 "vmadday.xyzw $ACC, $vf9, $vf4y\n"
                 "vmaddaz.xyzw $ACC, $vf10, $vf4z\n"
                 "vmaddw.xyzw $vf4, $vf11, $vf4w");
    return __detectMCInitInit_Check((MC_DATA*)pChip->data, m_fMDVal);
}
static inline INT _vecX(u_long128 x) {
    INT out32;
    asm("sll %0, %1, 0" : "=r"(out32) : "r"(x));
    return out32;
}
static inline INT _vecY(u_long128 x) {
    INT out32;
    asm("dsra32 %0, %1, 0" : "=r"(out32) : "r"(x));
    return out32;
}
static inline INT _vecZ(u_long128 x) {
    ULONGLONG out64;
    INT out32;
    asm("pcpyud %0, %1, $0" : "=r"(out64) : "r"(x));
    asm("sll %0, %1, 0" : "=r"(out32) : "r"(out64));
    return out32;
}
/* Parked: 23 lines, registers only - the original keeps the VU0 flag and iRet in a2 and fMDVal/zoneVec in v1, ours
   v0/a0; iRet-as-flag, if/else and early-return forms and a 5 min permuter run (14.5k candidates) keep it. */
#if 0
static INT __detectMCInitInit_Check(MC_DATA* pMCData, FLOAT fMDVal) {
    INT iRet;
    u_long128 zoneVec;
    INT flag;
    asm volatile("lqc2 $vf5, 0x0(%0)\n"
                 "lqc2 $vf6, 0x10(%0)" : : "r"(pMCData));
    asm volatile("qmtc2.ni %0, $vf7" : : "r"(fMDVal));
    asm volatile("vmulax.xyzw $ACC, $vf4, $vf7x\n"
                 "vmsubw.xyz $vf4, $vf5, $vf0w\n"
                 "vnop\n"
                 "vnop\n"
                 "ctc2.ni $0, $vi16\n"
                 "vsub.xyz $vf0, $vf4, $vf0\n"
                 "vsub.xyz $vf0, $vf6, $vf4\n"
                 "vftoi0.xyzw $vf4, $vf4\n"
                 "vnop\n"
                 "vnop\n"
                 "vnop\n"
                 "vnop");
    asm volatile("cfc2.ni %0, $vi16" : "=r"(flag));
    iRet = -1;
    if (!(flag & 0xC0)) {
        asm volatile("qmfc2.ni %0, $vf4" : "=r"(zoneVec));
        iRet = (_vecZ(zoneVec) * pMCData->xw + _vecX(zoneVec)) * pMCData->yw + _vecY(zoneVec);
        if (pMCData->tbl[iRet][0] == 0xFFFF) iRet = -1;
    }
    return iRet;
}
#else
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Game/Collision/CollUtil", __detectMCInitInit_Check__FP7MC_DATAf);
#endif
extern char D_004892F8[]; /* PRIM_INFO overflow message */
extern char D_00489310[]; /* "c:/KHCOM/MainApp/Game/Collision/CollUtil.cpp" */
extern char D_00489340[]; /* "m_PrimInfoIdx < PRIM_INFO_MAX" */
extern char D_00489360[]; /* "m_PrimInfoIdx2 < PRIM_INFO2_MAX" */
static inline INT _dotN(FVECTOR& v) {
    u_long128 ret;
    INT iTmp;
    asm volatile("lqc2 $vf4, 0x0(%1)\n"
                 "vmulax.x $ACC, $vf21, $vf4x\n"
                 "vmadday.x $ACC, $vf22, $vf4y\n"
                 "vmaddz.x $vf4, $vf23, $vf4z\n"
                 "qmfc2.ni %0, $vf4" : "=r"(ret) : "r"(&v));
    asm("sll %0, %1, 0" : "=r"(iTmp) : "r"(ret));
    return iTmp;
}
static inline FLOAT _dotNf(FVECTOR& v) {
    INT iTmp;
    FLOAT ret;
    asm volatile("lqc2 $vf4, 0x0(%1)\n"
                 "vmulax.x $ACC, $vf21, $vf4x\n"
                 "vmadday.x $ACC, $vf22, $vf4y\n"
                 "vmaddz.x $vf4, $vf23, $vf4z\n"
                 "qmfc2.ni %0, $vf4" : "=r"(iTmp) : "r"(&v));
    asm("mtc1 %1, %0" : "=f"(ret) : "r"(iTmp));
    return ret;
}
/* Parked: 48 of 185 lines, 4 bytes long - structure, frame (unused primInfo/primInfo2/v0 locals) and VU0 blocks as the
   original; left: the t-register numbering of pMCData/iCnt/entryCnt/pPrimInfo2 (t0/t1/t2/a3 there) and one extra
   instruction. hill.py over the setup statements and a 5 min permuter run (14.5k candidates) gain 1 line. */
#if 0
void CCollUtil::detectMCInit(MC_CHIP* pChip, INT iArea) {
    FVECTOR vCk0;
    FVECTOR vCk1;
    FVECTOR vCkM;
    FVECTOR vCkVec;
    MC_DATA* pMCData;
    USHORT* pPrimIdx;
    INT iCnt;
    MC_PRIM* pPrim;
    PRIM_INFO primInfo;
    PRIM_INFO2 primInfo2;
    INT entryCnt;
    PRIM_INFO2* pPrimInfo2;
    FVECTOR& _vCkS0 = m_vCkS0;
    FVECTOR& _vCkE = m_vCkE;
    asm volatile("lqc2 $vf4, 0x0(%0)\n"
                 "lqc2 $vf5, 0x0(%1)\n"
                 "vmulax.xyzw $ACC, $vf8, $vf4x\n"
                 "vmadday.xyzw $ACC, $vf9, $vf4y\n"
                 "vmaddaz.xyzw $ACC, $vf10, $vf4z\n"
                 "vmaddw.xyzw $vf4, $vf11, $vf4w\n"
                 "vmulax.xyzw $ACC, $vf8, $vf5x\n"
                 "vmadday.xyzw $ACC, $vf9, $vf5y\n"
                 "vmaddaz.xyzw $ACC, $vf10, $vf5z\n"
                 "vmaddw.xyzw $vf6, $vf11, $vf5w\n"
                 "vmaddaw.xyzw $ACC, $vf11, $vf5w\n"
                 "vmsubw.xyzw $vf5, $vf4, $vf0w\n"
                 "sqc2 $vf4, 0x0(%2)\n"
                 "sqc2 $vf6, 0x0(%3)\n"
                 "sqc2 $vf5, 0x0(%4)" : : "r"(&_vCkS0), "r"(&_vCkE), "r"(&vCk0), "r"(&vCk1), "r"(&vCkVec) : "memory");
    pMCData = (MC_DATA*)pChip->data;
    pPrimInfo2 = &m_pPrimInfo2[m_PrimInfoIdx2];
    entryCnt = 0;
    pPrimIdx = &pMCData->prim[pMCData->tbl[iArea][0]];
    iCnt = pMCData->tbl[iArea][1];
    for (; iCnt; iCnt--, pPrimIdx++) {
        FVECTOR v0;
        FVECTOR v1;
        FVECTOR* pN0;
        FLOAT d0;
        pPrim = &pMCData->prim4[*pPrimIdx];
        if (!m_fgCamera && pPrim->info < 0) continue;
        pN0 = &pMCData->norm[pPrim->n0];
        asm volatile("lqc2 $vf21, 0x0(%0)\n"
                     "vaddy.x $vf22, $vf0, $vf21y\n"
                     "vaddz.x $vf23, $vf0, $vf21z" : : "r"(pN0));
        if (_dotN(vCkVec) >= 0) continue;
        if (m_SphereDetectFg) {
            asm volatile("lqc2 $vf5, 0x0(%0)\n"
                         "lqc2 $vf4, 0x0(%1)\n"
                         "qmtc2.ni %2, $vf6\n"
                         "vaddax.xyzw $ACC, $vf5, $vf0x\n"
                         "vmsubx.xyzw $vf4, $vf4, $vf6x\n"
                         "sqc2 $vf4, 0x0(%3)" : : "r"(&vCk1), "r"(pN0), "r"(m_Radius), "r"(&v1) : "memory");
        } else {
            v1 = vCk1;
        }
        d0 = -pN0->w;
        if (d0 < _dotNf(v1)) continue;
        if (_dotNf(vCk0) < d0) continue;
        entryCnt++;
        PRIM_INFO* pPrimInfo = &m_pPrimInfo[m_PrimInfoIdx];
        pPrimInfo->pPrim = pPrim;
        pPrimInfo->pInfo2 = pPrimInfo2;
        pPrimInfo->fgLine = 0;
        m_PrimInfoIdx++;
    }
    if (entryCnt) {
        m_PrimInfoIdx2++;
        pPrimInfo2->pMatLW = &pChip->matLW;
        pPrimInfo2->vLS = vCk0;
        pPrimInfo2->vLE = vCk1;
        pPrimInfo2->vLVec = vCkVec;
        pPrimInfo2->pNorm = pMCData->norm;
        pPrimInfo2->pMcChip = pChip;
        pPrimInfo2->pVert = pMCData->vert;
    }
    if (!(m_PrimInfoIdx < 0x240)) {
        TRACE(D_004892F8);
        if (!(m_PrimInfoIdx < 0x240)) AssertApp(D_00489310, 818, D_00489340, 0);
    }
    if (!(m_PrimInfoIdx2 < 0x40)) AssertApp(D_00489310, 819, D_00489360, 0);
}
#else
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Game/Collision/CollUtil", detectMCInit__9CCollUtilP7MC_CHIPi);
#endif
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Game/Collision/CollUtil", _detectMCD__9CCollUtilR10CCollision);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Game/Collision/CollUtil", detectMap__9CCollUtilP7FVECTORT1);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Game/Collision/CollUtil", detectPolygon__9CCollUtil);
INT CCollUtil::detectPLine() {
    PRIM_INFO2* pPrimInfo2 = m_pPrimInfoCurrent->pInfo2;
    MC_PRIM* pPrim = m_pPrimInfoCurrent->pPrim;
    FVECTOR* pVert = pPrimInfo2->pVert;
    INT fg = m_pPrimInfoCurrent->fgLine;
    do {
        if ((fg & 1) && distanceLine(&pVert[pPrim->v0], &pVert[pPrim->v1])) break;
        if ((fg & 2) && distanceLine(&pVert[pPrim->v1], &pVert[pPrim->v2])) break;
        if ((fg & 4) && distanceLine(&pVert[pPrim->v2], &pVert[pPrim->v3])) break;
        if (pPrim->kind && (fg & 8) && distanceLine(&pVert[pPrim->v3], &pVert[pPrim->v0])) break;
        return FALSE;
    } while (0);
    return TRUE;
}
static inline FLOAT _sqrtf(FLOAT x) {
    FLOAT ret;
    asm volatile("sqrt.s %0, %1" : "=f"(ret) : "f"(x));
    return ret;
}
static inline FLOAT _distance(FVECTOR& v, const FVECTOR& p0, const FVECTOR& p1) {
    SubVector(v, p0, p1);
    return _sqrtf(InnerProduct(v));
}
/* Parked: 215 of 506 lines, 52 bytes long - first draft, structure decoded (closest points of the two segments, two
   normal/offset branches, four range checks, slidingHosei). Left: the original keeps pP0/pP1 in stack slots, lays the
   denom if/else out with the test at the bottom (do-while(0) forms tried: worse) and keeps 0.0f in a temp. */
#if 0
INT CCollUtil::distanceLine(FVECTOR* pP0, FVECTOR* pP1) {
    FVECTOR p;
    FVECTOR v0;
    FVECTOR s0;
    FVECTOR s1;
    FVECTOR vec0;
    FLOAT pv0;
    FLOAT pv1;
    FLOAT v00;
    FLOAT v11;
    FLOAT v01;
    FLOAT denom;
    FLOAT s;
    FLOAT t;
    FVECTOR v;
    FVECTOR u;
    FVECTOR su;
    FVECTOR qs;
    FVECTOR pq;
    FLOAT f0;
    FLOAT f1;
    FLOAT m;
    FVECTOR nvec;
    SubVector(v0, *pP1, *pP0);
    SubVector(p, m_vCk0, *pP0);
    pv0 = InnerProduct(p, v0);
    pv1 = InnerProduct(p, m_vCkVec);
    v00 = InnerProduct(v0);
    if (v00 == 0.0f) return 0;
    v11 = m_CkVecl;
    v01 = InnerProduct(v0, m_vCkVec);
    denom = v00 * v11 - v01 * v01;
    if (denom < -1e-05f || 1e-05f <= denom) {
        s = (pv0 * v01 - pv1 * v00) / denom;
        if (s < 0.0f) return 0;
        t = (pv0 * v11 - pv1 * v01) / denom;
    } else {
        t = pv0 / v00;
        s = 0.0f;
    }
    ScaleVector(vec0, v0, t);
    AddVector(s0, *pP0, vec0);
    ScaleVector(vec0, m_vCkVec, s);
    AddVector(s1, m_vCk0, vec0);
    f0 = _distance(qs, s1, s0);
    if (m_RadiusL < f0) return 0;
    m = _sqrtf(m_Radius2 - f0 * f0);
    SubVector(pq, s1, s0);
    OuterProduct(qs, v0, pq);
    f1 = _sqrtf(InnerProduct(qs));
    DivVector(qs, qs, f1);
    if (f1 != 0.0f && 1e-05f <= f0) {
        if (0.0f < InnerProduct(qs, m_vCkVec)) ScaleVector(qs, qs, -1.0f);
        ScaleVector(qs, qs, m);
        Normalize(su, v0);
        if (0.0f < InnerProduct(su, m_vCkVec)) ScaleVector(su, su, -1.0f);
        f1 = _sqrtf(v00);
        if (1e-05f <= v01 || v01 < -1e-05f) {
            f1 = f1 * m_CkVecl2 / v01;
            f0 = f1 * f1 - 1.0f;
            if (1e-05f <= f0 || f0 < -1e-05f) {
                ScaleVector(su, su, m / _sqrtf(f0));
            } else {
                su._q = 0;
            }
        } else {
            su._q = 0;
        }
    } else {
        Normalize(v, v0);
        if (0.0f < InnerProduct(v, m_vCkVec)) ScaleVector(v, v, -1.0f);
        f1 = _sqrtf(v00);
        if (1e-05f <= v01 || v01 < -1e-05f) {
            f1 = f1 * m_CkVecl2 / v01;
            f0 = f1 * f1 - 1.0f;
            if (1e-05f <= f0 || f0 < -1e-05f) {
                m = m / _sqrtf(f0);
                ScaleVector(su, v, m);
            } else {
                su._q = 0;
            }
            m = m * f1;
        } else {
            m = m_RadiusL;
            su._q = 0;
        }
        Normalize(qs, m_vCkVec);
        ScaleVector(qs, qs, -m);
        SubVector(qs, qs, su);
    }
    AddVector(vec0, qs, su);
    AddVector(u, s1, vec0);
    SubVector(vec0, u, m_vCk0);
    if (InnerProduct(m_vCkVec, vec0) < 0.0f) return 0;
    SubVector(vec0, u, m_vCk1);
    if (0.0f < InnerProduct(m_vCkVec, vec0)) return 0;
    AddVector(v, s0, su);
    SubVector(vec0, v, *pP0);
    if (InnerProduct(v0, vec0) < 0.0f) return 0;
    SubVector(vec0, v, *pP1);
    if (0.0f < InnerProduct(v0, vec0)) return 0;
    if (!slidingHosei(v, u, nvec)) return 0;
    m_vNormal = nvec;
    m_vDetect = m_vCk2;
    return 1;
}
#else
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Game/Collision/CollUtil", distanceLine__9CCollUtilP7FVECTORT1);
#endif
INT CCollUtil::detectPSphere() {
    PRIM_INFO2* pPrimInfo2 = m_pPrimInfoCurrent->pInfo2;
    MC_PRIM* pPrim = m_pPrimInfoCurrent->pPrim;
    FVECTOR* pVert = pPrimInfo2->pVert;
    INT fg = m_pPrimInfoCurrent->fgLine;
    do {
        if ((fg & 0x10) && distanceSphere(&pVert[pPrim->v0])) break;
        if ((fg & 0x20) && distanceSphere(&pVert[pPrim->v1])) break;
        if ((fg & 0x40) && distanceSphere(&pVert[pPrim->v2])) break;
        if (pPrim->kind && (fg & 0x80) && distanceSphere(&pVert[pPrim->v3])) break;
        return FALSE;
    } while (0);
    return TRUE;
}
INT CCollUtil::distanceSphere(FVECTOR* pP0) {
    FVECTOR vc0;
    FVECTOR vc1;
    FLOAT u;
    FVECTOR p;
    FVECTOR s;
    FVECTOR vec0;
    FVECTOR nvec;
    SubVector(vc0, *pP0, m_vCk0);
    SubVector(vc1, *pP0, m_vCk1);
    u = InnerProduct(vc0, m_vCkVec) / m_CkVecl;
    if (u < 0.0f) return 0;
    ScaleVector(p, m_vCkVec, u);
    AddVector(p, m_vCk0, p);
    FLOAT l = _distance(s, *pP0, p);
    if (m_RadiusL < l) return 0;
    FLOAT len = _sqrtf(m_Radius2 - l * l);
    Normalize(s, m_vCkVec);
    ScaleVector(s, s, len);
    SubVector(s, p, s);
    SubVector(vec0, s, m_vCk0);
    if (InnerProduct(m_vCkVec, vec0) < 0.0f) return 0;
    SubVector(vec0, s, m_vCk1);
    if (0.0f < InnerProduct(m_vCkVec, vec0)) return 0;
    if (!slidingHosei(*pP0, s, nvec)) return 0;
    m_vNormal = nvec;
    m_vDetect = m_vCk2;
    return 1;
}
FLOAT InnerProduct4(const FVECTOR& v0, const FVECTOR& v1);
INT CCollUtil::slidingHosei(FVECTOR& vCenter, FVECTOR& vSurface, FVECTOR& vNormal) {
    FVECTOR vSurf;
    FVECTOR vTmp0;
    FVECTOR vTmp1;
    FVECTOR vTmp2;
    FVECTOR normalY;
    MC_PRIM* pPrim;
    BYTE info2Kind;
    FLOAT t1;
    register FLOAT t2 asm("$f0"); /* MATCH: the original keeps t2 in the call result register (f0), ours otherwise copies it to f1 */
    if (m_pPrimInfoCurrent) {
        pPrim = m_pPrimInfoCurrent->pPrim;
        m_iInfo.info = pPrim->info;
        info2Kind = m_iInfo.infoPID & 0x3F;
        if (m_iKind >= 0) {
            if (info2Kind >= 2) {
                if (info2Kind == 2) {
                    if (!m_iInfo2.infoPID) {
                        m_iInfo2.infoPID = m_iInfo.infoPID;
                        m_vDetectP = vSurface;
                        m_vNormalP = vNormal;
                        m_pDetectMatP = m_pPrimInfoCurrent->pInfo2->pMatLW;
                    }
                    m_pPrimInfoCurrent->fgLine = 0;
                    return 0;
                }
                if (m_iKind != 10) return 0;
            }
        } else if (m_fgCamera) {
            if (m_iInfo.infoPID) {
                if (info2Kind == 1 && m_iKind == (INT)0x8000000E) {
                    if (m_SemiTransMatIdx < 32) {
                        m_pSemiTransMat[m_SemiTransMatIdx++] = m_pPrimInfoCurrent->pInfo2->pMatLW;
                        m_iInfo2.infoPID = m_iInfo.infoPID;
                    }
                } else if (!m_iInfo2.infoPID) {
                    m_iInfo2.infoPID = m_iInfo.infoPID;
                }
                m_pPrimInfoCurrent->fgLine = 0;
                return 0;
            }
        } else if (m_iInfo.infoPID & 0x80) {
            m_pPrimInfoCurrent->fgLine = 0;
            return 0;
        }
    }
    SubVector(vNormal, vSurface, vCenter);
    Normalize(vNormal, vNormal);
    ScaleVector(vSurf, vNormal, m_Radius);
    AddVector(vSurf, vSurf, vCenter);
    if (!m_fgSliding) {
        m_vCk1 = vSurf;
    } else {
        vNormal.w = -InnerProduct(vSurf, vNormal);
        if (m_pPrimInfoCurrent) {
            PRIM_INFO2* pPrimInfo2 = m_pPrimInfoCurrent->pInfo2;
            normalY.x = pPrimInfo2->pMatLW->m[0][1];
            normalY.y = pPrimInfo2->pMatLW->m[1][1];
            normalY.z = pPrimInfo2->pMatLW->m[2][1];
        } else {
            FLOAT f = 0.0f;
            normalY.z = f;
            normalY.y = 1.0f;
            normalY.x = f;
        }
        m_vCk1.w = 1.0f;
        t1 = -InnerProduct4(vNormal, m_vCk1);
        t2 = InnerProduct(vNormal, normalY);
        if (0.64278764f < t2) {
            ScaleVector(vTmp0, normalY, t1 / t2);
            AddVector(vTmp0, m_vCk1, vTmp0);
            SubVector(vTmp2, vTmp0, vSurf);
            if (2.5000003e-07f <= InnerProduct(vTmp2)) {
                SubVector(vTmp1, m_vCk1, vSurf);
                t1 = _sqrtf(InnerProduct(vTmp1));
                SubVector(vTmp0, vTmp0, vSurf);
                Normalize(vTmp0, vTmp0);
                ScaleVector(vTmp0, vTmp0, t1);
                AddVector(m_vCk1, vSurf, vTmp0);
            } else {
                m_vCk1 = vSurf;
            }
        } else {
            ScaleVector(vTmp0, vNormal, t1);
            AddVector(m_vCk1, m_vCk1, vTmp0);
        }
    }
    ScaleVector(vTmp0, vNormal, 0.0035f);
    AddVector(m_vCk1, m_vCk1, vTmp0);
    AddVector(m_vCk2, vSurf, vTmp0);
    m_vCk2.w = 1.0f;
    m_vCk1.w = 1.0f;
    return 1;
}
/* MATCH: the original links no CCollDbg (debug build only) and calls address 0. Ps2EeAs makes undefined weak symbols
   global, so the method is a weak absolute 0, as in Game/Gimmick/CreateGimmick (the relocation names func_0 there) */
class CCollDbg {
public:
    void initEveryInt(INT nMax);
};
__asm__(".weak initEveryInt__8CCollDbgi\ninitEveryInt__8CCollDbgi = 0\n");
void CCollUtil::_initDbg() {
    if (m_pCollDbg) {
        ((CCollDbg*)m_pCollDbg)->initEveryInt(m_PrimInfoMax);
        m_PrimInfoMax = 0;
    }
}
// SOURCE DATA: .data
