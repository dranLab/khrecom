/* .rodata comes from the original assembly: the split ends it at 0x4893B4, in the middle of CSphereCollision's
 * vtable (Game/Collision/CollUtilO's data holds the rest and CCollision's vtable). The code refers to the
 * original's strings by their labels instead. */
#define COLLOBJ_H_COLLISION_CLASSES
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
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Game/Collision/CollUtil", __detectMCInitInit__9CCollUtilP7MC_CHIP);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Game/Collision/CollUtil", __detectMCInitInit_Check__FP7MC_DATAf);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Game/Collision/CollUtil", detectMCInit__9CCollUtilP7MC_CHIPi);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Game/Collision/CollUtil", _detectMCD__9CCollUtilR10CCollision);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Game/Collision/CollUtil", detectMap__9CCollUtilP7FVECTORT1);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Game/Collision/CollUtil", detectPolygon__9CCollUtil);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Game/Collision/CollUtil", detectPLine__9CCollUtil);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Game/Collision/CollUtil", distanceLine__9CCollUtilP7FVECTORT1);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Game/Collision/CollUtil", detectPSphere__9CCollUtil);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Game/Collision/CollUtil", distanceSphere__9CCollUtilP7FVECTOR);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Game/Collision/CollUtil", slidingHosei__9CCollUtilR7FVECTORN21);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Game/Collision/CollUtil", _initDbg__9CCollUtil);
// SOURCE DATA: .data
