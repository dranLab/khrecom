#define TASK_H_NO_SLC_OVERFLOW_MSG
#include "System/SJoint.h"
/* vtable order: CSJointT<CLinkedPtr> is completed before CCoords, CLinkedPtr after it */
class CLinkedPtr;
typedef char _instCSJointLinkedPtr[sizeof(CSJointT<CLinkedPtr>)];
#include "System/Coord.h"
#include "System/LinkedBuf.h"
#include "System/CoordMan.h"
#include "System/Debug.h"
#include "include_asm.h"

/* .data (the VU0 library's sin/cos table) still comes from the original assembly. */
extern const FVECTOR ONE_VECTOR = { { 1.0f, 1.0f, 1.0f, 1.0f } };
extern const FMATRIX ONE_MATRIX = { { { 1.0f, 0.0f, 0.0f, 0.0f },
                                      { 0.0f, 1.0f, 0.0f, 0.0f },
                                      { 0.0f, 0.0f, 1.0f, 0.0f },
                                      { 0.0f, 0.0f, 0.0f, 1.0f } } };

#define RET_ADDR() ({ UINT nRA; __asm__ volatile("daddu %0, $31, $0" : "=r"(nRA)); nRA; })

inline void* operator new[](size_t size, char* msg) throw() {
    return com_malloc_d(size, msg, RET_ADDR());
}
inline void operator delete[](void* mem) { com_free(mem); }

void CCoords::_coord_alart(UINT nIndex) const {
    TRACE("*** CCoords:: \x94\xcd\x88\xcd\x8aO\x82\xcc Coord \x82\xaa\x8ew\x92\xe8\x82\xb3\x82\xea\x82\xc4\x82\xa2\x82\xdc\x82\xb7 this:%08x pCrdArray:%08x CrdNum:%d Index:%d ***\n",
          this, m_pCrdArray, m_nCrdNum, nIndex);
}
void CCoords::_coord_nullalart() const {
    TRACE("*** CCoords:: Coord \x82\xaa\x91\xb6\x8d\xdd\x82\xb5\x82\xdc\x82\xb9\x82\xf1 this:%08x pCrdArray:%08x CrdNum:%d ***\n", this,
          m_pCrdArray, m_nCrdNum);
}
/* Placeholder (in .sdata, stripped) for the string of the INCLUDE_ASM functions below */
__asm__(".section .sdata\n");
__attribute__((section(".sdata"))) void _fix_placeholder(INT n) {
    TRACE(" CCoord : \x83" "A\x83^\x83" "b\x83`\x90\xe6\x82\xcc\x90" "e\x82\xaa\x82\xa2\x82\xc8\x82\xad\x82\xc8\x82\xc1\x82\xc4\x82\xa2\x82\xdc\x82\xb7!! this:%08x Parent:%08x Ex:%08x ExP:%08x\n");
}
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/System/Coord", fix__6CCoordi);
void CCoord::getRotTransPersMatrix(FMATRIX& p0) {
    asm volatile(
                 "addiu $4, $4, 0x90\n"
                 "lqc2 $vf20, 0x0($4)\n"
                 "lqc2 $vf21, 0x10($4)\n"
                 "lqc2 $vf22, 0x20($4)\n"
                 "lqc2 $vf23, 0x30($4)\n"
                 "vmulax.xyzw $ACC, $vf28, $vf20x\n"
                 "vmadday.xyzw $ACC, $vf29, $vf20y\n"
                 "vmaddaz.xyzw $ACC, $vf30, $vf20z\n"
                 "vmaddw.xyzw $vf16, $vf31, $vf20w\n"
                 "vmulax.xyzw $ACC, $vf28, $vf21x\n"
                 "vmadday.xyzw $ACC, $vf29, $vf21y\n"
                 "vmaddaz.xyzw $ACC, $vf30, $vf21z\n"
                 "vmaddw.xyzw $vf17, $vf31, $vf21w\n"
                 "vmulax.xyzw $ACC, $vf28, $vf22x\n"
                 "vmadday.xyzw $ACC, $vf29, $vf22y\n"
                 "vmaddaz.xyzw $ACC, $vf30, $vf22z\n"
                 "vmaddw.xyzw $vf18, $vf31, $vf22w\n"
                 "vmulax.xyzw $ACC, $vf28, $vf23x\n"
                 "vmadday.xyzw $ACC, $vf29, $vf23y\n"
                 "vmaddaz.xyzw $ACC, $vf30, $vf23z\n"
                 "vmaddw.xyzw $vf19, $vf31, $vf23w\n"
                 "sqc2 $vf16, 0x0($5)\n"
                 "sqc2 $vf17, 0x10($5)\n"
                 "sqc2 $vf18, 0x20($5)\n"
                 "sqc2 $vf19, 0x30($5)\n");
}
void CCoord::getRotTransPersMatrix_VF04_07() {
    asm volatile(
                 "addiu $4, $4, 0x90\n"
                 "lqc2 $vf4, 0x0($4)\n"
                 "lqc2 $vf5, 0x10($4)\n"
                 "lqc2 $vf6, 0x20($4)\n"
                 "lqc2 $vf7, 0x30($4)\n"
                 "vmulax.xyzw $ACC, $vf28, $vf4x\n"
                 "vmadday.xyzw $ACC, $vf29, $vf4y\n"
                 "vmaddaz.xyzw $ACC, $vf30, $vf4z\n"
                 "vmaddw.xyzw $vf4, $vf31, $vf4w\n"
                 "vmulax.xyzw $ACC, $vf28, $vf5x\n"
                 "vmadday.xyzw $ACC, $vf29, $vf5y\n"
                 "vmaddaz.xyzw $ACC, $vf30, $vf5z\n"
                 "vmaddw.xyzw $vf5, $vf31, $vf5w\n"
                 "vmulax.xyzw $ACC, $vf28, $vf6x\n"
                 "vmadday.xyzw $ACC, $vf29, $vf6y\n"
                 "vmaddaz.xyzw $ACC, $vf30, $vf6z\n"
                 "vmaddw.xyzw $vf6, $vf31, $vf6w\n"
                 "vmulax.xyzw $ACC, $vf28, $vf7x\n"
                 "vmadday.xyzw $ACC, $vf29, $vf7y\n"
                 "vmaddaz.xyzw $ACC, $vf30, $vf7z\n"
                 "vmaddw.xyzw $vf7, $vf31, $vf7w\n");
}
void CCoord::RotTransPers(IVECTOR& p0, const FVECTOR& p1) {
    asm volatile(
                 "addiu $4, $4, 0x90\n"
                 "lqc2 $vf4, 0x0($4)\n"
                 "lqc2 $vf5, 0x10($4)\n"
                 "lqc2 $vf6, 0x20($4)\n"
                 "lqc2 $vf7, 0x30($4)\n"
                 "lqc2 $vf8, 0x0($6)\n"
                 "vmulax.xyzw $ACC, $vf28, $vf4x\n"
                 "vmadday.xyzw $ACC, $vf29, $vf4y\n"
                 "vmaddaz.xyzw $ACC, $vf30, $vf4z\n"
                 "vmaddw.xyzw $vf4, $vf31, $vf4w\n"
                 "vmulax.xyzw $ACC, $vf28, $vf5x\n"
                 "vmadday.xyzw $ACC, $vf29, $vf5y\n"
                 "vmaddaz.xyzw $ACC, $vf30, $vf5z\n"
                 "vmaddw.xyzw $vf5, $vf31, $vf5w\n"
                 "vmulax.xyzw $ACC, $vf28, $vf6x\n"
                 "vmadday.xyzw $ACC, $vf29, $vf6y\n"
                 "vmaddaz.xyzw $ACC, $vf30, $vf6z\n"
                 "vmaddw.xyzw $vf6, $vf31, $vf6w\n"
                 "vmulax.xyzw $ACC, $vf28, $vf7x\n"
                 "vmadday.xyzw $ACC, $vf29, $vf7y\n"
                 "vmaddaz.xyzw $ACC, $vf30, $vf7z\n"
                 "vmaddw.xyzw $vf7, $vf31, $vf7w\n"
                 "vmulax.xyzw $ACC, $vf4, $vf8x\n"
                 "vmadday.xyzw $ACC, $vf5, $vf8y\n"
                 "vmaddaz.xyzw $ACC, $vf6, $vf8z\n"
                 "vmaddw.xyzw $vf8, $vf7, $vf8w\n"
                 "vdiv $Q, $vf0w, $vf8w\n"
                 "vmax.w $vf8, $vf0, $vf0\n"
                 "vwaitq\n"
                 "vmulq.xyzw $vf8, $vf8, $Q\n"
                 "vftoi4.xyz $vf8, $vf8\n"
                 "sqc2 $vf8, 0x0($5)\n");
}
void CCoord::RotTransPers(FVECTOR& p0, const FVECTOR& p1) {
    asm volatile(
                 "addiu $4, $4, 0x90\n"
                 "lqc2 $vf4, 0x0($4)\n"
                 "lqc2 $vf5, 0x10($4)\n"
                 "lqc2 $vf6, 0x20($4)\n"
                 "lqc2 $vf7, 0x30($4)\n"
                 "lqc2 $vf8, 0x0($6)\n"
                 "vmulax.xyzw $ACC, $vf28, $vf4x\n"
                 "vmadday.xyzw $ACC, $vf29, $vf4y\n"
                 "vmaddaz.xyzw $ACC, $vf30, $vf4z\n"
                 "vmaddw.xyzw $vf4, $vf31, $vf4w\n"
                 "vmulax.xyzw $ACC, $vf28, $vf5x\n"
                 "vmadday.xyzw $ACC, $vf29, $vf5y\n"
                 "vmaddaz.xyzw $ACC, $vf30, $vf5z\n"
                 "vmaddw.xyzw $vf5, $vf31, $vf5w\n"
                 "vmulax.xyzw $ACC, $vf28, $vf6x\n"
                 "vmadday.xyzw $ACC, $vf29, $vf6y\n"
                 "vmaddaz.xyzw $ACC, $vf30, $vf6z\n"
                 "vmaddw.xyzw $vf6, $vf31, $vf6w\n"
                 "vmulax.xyzw $ACC, $vf28, $vf7x\n"
                 "vmadday.xyzw $ACC, $vf29, $vf7y\n"
                 "vmaddaz.xyzw $ACC, $vf30, $vf7z\n"
                 "vmaddw.xyzw $vf7, $vf31, $vf7w\n"
                 "vmulax.xyzw $ACC, $vf4, $vf8x\n"
                 "vmadday.xyzw $ACC, $vf5, $vf8y\n"
                 "vmaddaz.xyzw $ACC, $vf6, $vf8z\n"
                 "vmaddw.xyzw $vf8, $vf7, $vf8w\n"
                 "vdiv $Q, $vf0w, $vf8w\n"
                 "vmax.w $vf8, $vf0, $vf0\n"
                 "vwaitq\n"
                 "vmulq.xyzw $vf8, $vf8, $Q\n"
                 "sqc2 $vf8, 0x0($5)\n");
}
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/System/Coord", recalcWorldMatrix__6CCoord);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/System/Coord", recalcLocalMatrix__6CCoord);
CCoords::CCoords(_PROC_PRIO pp) {
    m_pCrdArray = __null;
    m_nCrdNum = 0;
    m_hSysHandle = 0;
    m_iFlags = pp << 2;
    CCoordMan::add(*this, pp);
}
CCoords::~CCoords() {
    if (m_hSysHandle) {
        CCoordMan::del(*this);
    }
    delCoord();
}
BOOL CCoords::addCoord(UINT nNum, CCoord* pParent) {
    UINT i;

    if (!nNum) {
        nNum = 1;
    }
    if (m_nCrdNum) {
        TRACE("\x8cx\x8d\x90\x81" "FCoord\x82\xcc\x8d\xc4\x8am\x95\xdb\x82\xaa\x8ds\x82\xed\x82\xea\x82\xdc\x82\xb5\x82\xbd\x81" "B\x96\xb3\x8e\x8b\x82\xb5\x82\xdc\x82\xb7\n");
        return FALSE;
    }
#line 480
    m_pCrdArray = new (__FILE__ "(480)") CCoord[nNum];
    if (!m_pCrdArray) {
        TRACE("\x8cx\x8d\x90\x81" "FCoord\x82\xcc\x83\x81\x83\x82\x83\x8a\x8am\x95\xdb\x82\xaa\x8ds\x82\xa6\x82\xdc\x82\xb9\x82\xf1!!!!!!!!!!\n");
        return FALSE;
    }
    m_nCrdNum = nNum;
    if (pParent) {
        for (i = 0; i < nNum; i++) {
            m_pCrdArray[i].setParent(pParent);
        }
    }
    _setCoordHandle(m_hSysHandle);
    return TRUE;
}
void CCoords::delCoord() {
    if (m_pCrdArray) {
        _setCoordHandle(0);
        DelayedDelete(m_pCrdArray, TRUE);
        m_pCrdArray = __null;
        m_nCrdNum = 0;
    }
}
HANDLE CCoords::setSysFix(BOOL bSet, _PROC_PRIO pp) {
    if (bSet) {
        pp = (_PROC_PRIO)(pp & _PP_ALL);
        if (pp == _PP_ALL) {
            pp = _PP_PRIMARY;
        }
        m_iFlags = (m_iFlags & ~3) | pp;
        if (!(m_iFlags & (pp << 2))) {
            CCoordMan::del(*this);
            CCoordMan::add(*this, pp);
            m_iFlags = (m_iFlags & ~0xC) | (pp << 2);
        }
    } else {
        m_iFlags &= ~3;
    }
    return m_hSysHandle;
}
void CCoords::update(TIMEQ nTQ) {
    CCoord* p = Crd();
    if (p) {
        for (INT i = 0; i < m_nCrdNum; i++, p++) {
            p->fix(nTQ);
        }
    }
}
void CCoords::touch() {
    CCoord* p = Crd();
    if (p) {
        for (INT i = 0; i < m_nCrdNum; i++, p++) {
            p->touch();
        }
    }
}
void CCoords::resetTimeStamp() {
    CCoord* p = Crd();
    if (p) {
        for (INT i = 0; i < m_nCrdNum; i++, p++) {
            p->resetTimeStamp();
        }
    }
}
void CCoords::setSysHandle(HANDLE h) {
    m_hSysHandle = h;
    _setCoordHandle(h);
}
void CCoords::_setCoordHandle(HANDLE h) {
    CCoord* p = m_pCrdArray;
    if (p) {
        for (INT i = 0; i < m_nCrdNum; i++, p++) {
            p->setHandle(h);
        }
    }
}
CCoord& CCoords::getExceptCoord() {
    return CCoordMan::getExceptCoord(TRUE);
}
CCoord& CCoords::getExceptCoord() const {
    return CCoordMan::getExceptCoord(TRUE);
}
void ApplyMatrix33(FVECTOR& p0, const FMATRIX& p1, const FVECTOR& p2) {
    asm volatile(
                 "lqc2 $vf8, 0x0($6)\n"
                 "lqc2 $vf4, 0x0($5)\n"
                 "lqc2 $vf5, 0x10($5)\n"
                 "lqc2 $vf6, 0x20($5)\n"
                 "vmulax.xyz $ACC, $vf4, $vf8x\n"
                 "vmadday.xyz $ACC, $vf5, $vf8y\n"
                 "vmaddz.xyz $vf8, $vf6, $vf8z\n"
                 "sqc2 $vf8, 0x0($4)\n");
}
void MulMatrix33(FMATRIX& p0, const FMATRIX& p1, const FMATRIX& p2) {
    asm volatile(
                 "lqc2 $vf2, 0x0($5)\n"
                 "lqc2 $vf3, 0x10($5)\n"
                 "lqc2 $vf4, 0x20($5)\n"
                 "lqc2 $vf6, 0x0($6)\n"
                 "lqc2 $vf7, 0x10($6)\n"
                 "lqc2 $vf8, 0x20($6)\n"
                 "vmulax.xyz $ACC, $vf2, $vf6x\n"
                 "vmadday.xyz $ACC, $vf3, $vf6y\n"
                 "vmaddz.xyz $vf6, $vf4, $vf6z\n"
                 "vmulax.xyz $ACC, $vf2, $vf7x\n"
                 "vmadday.xyz $ACC, $vf3, $vf7y\n"
                 "vmaddz.xyz $vf7, $vf4, $vf7z\n"
                 "vmulax.xyz $ACC, $vf2, $vf8x\n"
                 "vmadday.xyz $ACC, $vf3, $vf8y\n"
                 "vmaddz.xyz $vf8, $vf4, $vf8z\n"
                 "sqc2 $vf6, 0x0($4)\n"
                 "sqc2 $vf7, 0x10($4)\n"
                 "sqc2 $vf8, 0x20($4)\n");
}
void AbsVector(FVECTOR& p0, const FVECTOR& p1) {
    asm volatile(
                 "lqc2 $vf4, 0x0($5)\n"
                 "vabs.xyzw $vf4, $vf4\n"
                 "sqc2 $vf4, 0x0($4)\n");
}
FLOAT InnerProduct4(const FVECTOR& p0, const FVECTOR& p1) {
    asm volatile(
                 "lqc2 $vf4, 0x0($4)\n"
                 "lqc2 $vf5, 0x0($5)\n"
                 "vmul.xyzw $vf5, $vf4, $vf5\n"
                 "vaddy.x $vf5, $vf5, $vf5y\n"
                 "vaddz.x $vf5, $vf5, $vf5z\n"
                 "vaddw.x $vf5, $vf5, $vf5w\n"
                 "qmfc2.ni $2, $vf5\n"
                 "mtc1 $2, $f0\n");
}
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/System/Coord", spinMatrixX__FR7FMATRIXRC7FMATRIXf);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/System/Coord", spinMatrixY__FR7FMATRIXRC7FMATRIXf);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/System/Coord", spinMatrixZ__FR7FMATRIXRC7FMATRIXf);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/System/Coord", spinMatrixX__FR7FMATRIXRC7FMATRIXi);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/System/Coord", spinMatrixY__FR7FMATRIXRC7FMATRIXi);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/System/Coord", spinMatrixZ__FR7FMATRIXRC7FMATRIXi);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/System/Coord", rotMatrixX__FR7FMATRIXRC7FMATRIXi);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/System/Coord", rotMatrixY__FR7FMATRIXRC7FMATRIXi);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/System/Coord", rotMatrixZ__FR7FMATRIXRC7FMATRIXi);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/System/Coord", scaleMatrixSelf__FR7FMATRIXf);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/System/Coord", scaleMatrixSelf__FR7FMATRIXR7FVECTOR);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/System/Coord", World2Local__FR7FVECTORRC7FMATRIXRC7FVECTOR);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/System/Coord", cossin2deg__Fff);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/System/Coord", World2Eurar__FRsT0RC7FMATRIXRC7FVECTOR);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/System/Coord", roundPI__FR7FVECTORi);
void getMaxMinToVector8(FVECTOR* p0, const FVECTOR& p1, const FVECTOR& p2) {
    asm volatile(
                 "lqc2 $vf4, 0x0($5)\n"
                 "lqc2 $vf5, 0x0($6)\n"
                 "vadd.yz $vf6, $vf4, $vf0\n"
                 "vadd.y $vf7, $vf4, $vf0\n"
                 "vadd.xy $vf8, $vf4, $vf0\n"
                 "vadd.xz $vf9, $vf4, $vf0\n"
                 "vadd.z $vf10, $vf4, $vf0\n"
                 "vadd.x $vf11, $vf4, $vf0\n"
                 "vmulw.xw $vf6, $vf5, $vf0w\n"
                 "vmulw.xzw $vf7, $vf5, $vf0w\n"
                 "vmulw.zw $vf8, $vf5, $vf0w\n"
                 "vmulw.yw $vf9, $vf5, $vf0w\n"
                 "vmulw.xyw $vf10, $vf5, $vf0w\n"
                 "vmulw.yzw $vf11, $vf5, $vf0w\n"
                 "sqc2 $vf4, 0x0($4)\n"
                 "sqc2 $vf6, 0x10($4)\n"
                 "sqc2 $vf7, 0x20($4)\n"
                 "sqc2 $vf8, 0x30($4)\n"
                 "sqc2 $vf9, 0x40($4)\n"
                 "sqc2 $vf10, 0x50($4)\n"
                 "sqc2 $vf5, 0x60($4)\n"
                 "sqc2 $vf11, 0x70($4)\n");
}
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/System/Coord", ViewClipMatrix__FR7FMATRIXfffff);
INT ClipDisp(const FMATRIX& p0, const FVECTOR& p1) {
    asm volatile(
                 "lqc2 $vf8, 0x0($5)\n"
                 "lqc2 $vf4, 0x0($4)\n"
                 "lqc2 $vf5, 0x10($4)\n"
                 "lqc2 $vf6, 0x20($4)\n"
                 "lqc2 $vf7, 0x30($4)\n"
                 "vmulax.xyzw $ACC, $vf4, $vf8x\n"
                 "vmadday.xyzw $ACC, $vf5, $vf8y\n"
                 "vmaddaz.xyzw $ACC, $vf6, $vf8z\n"
                 "vmaddw.xyzw $vf9, $vf7, $vf0w\n"
                 ".word   0x4BC949FF\n"
                 "vnop\n"
                 "vnop\n"
                 "vnop\n"
                 "vnop\n"
                 "vnop\n"
                 "cfc2.ni $2, $vi18\n");
}
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/System/Coord", ClipDispBBox__FRC7FMATRIXPC7FVECTOR);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/System/Coord", ClipDispBBox__FPC7FVECTOR);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/System/Coord", ClipDispBBox__FRC7FMATRIXT0PC7FVECTOR);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/System/Coord", ClipDispBBox__FRC7FMATRIXN20PC7FVECTOR);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/System/Coord", ClipDispBBoxMaxMin__FPC7FVECTOR);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/System/Coord", ClipDispBBoxMaxMin__FRC7FMATRIXT0PC7FVECTOR);
void ApplyMatrix(FVECTOR& p0, const FMATRIX& p1, const FVECTOR& p2) {
    asm volatile(
                 "lqc2 $vf8, 0x0($6)\n"
                 "lqc2 $vf4, 0x0($5)\n"
                 "lqc2 $vf5, 0x10($5)\n"
                 "lqc2 $vf6, 0x20($5)\n"
                 "lqc2 $vf7, 0x30($5)\n"
                 "vmulax.xyzw $ACC, $vf4, $vf8x\n"
                 "vmadday.xyzw $ACC, $vf5, $vf8y\n"
                 "vmaddaz.xyzw $ACC, $vf6, $vf8z\n"
                 "vmaddw.xyzw $vf8, $vf7, $vf8w\n"
                 "sqc2 $vf8, 0x0($4)\n");
}
void MulMatrix(FMATRIX& p0, const FMATRIX& p1, const FMATRIX& p2) {
    asm volatile(
                 "lqc2 $vf2, 0x0($5)\n"
                 "lqc2 $vf3, 0x10($5)\n"
                 "lqc2 $vf4, 0x20($5)\n"
                 "lqc2 $vf5, 0x30($5)\n"
                 "lqc2 $vf6, 0x0($6)\n"
                 "lqc2 $vf7, 0x10($6)\n"
                 "lqc2 $vf8, 0x20($6)\n"
                 "lqc2 $vf9, 0x30($6)\n"
                 "vmulax.xyzw $ACC, $vf2, $vf6x\n"
                 "vmadday.xyzw $ACC, $vf3, $vf6y\n"
                 "vmaddaz.xyzw $ACC, $vf4, $vf6z\n"
                 "vmaddw.xyzw $vf6, $vf5, $vf6w\n"
                 "vmulax.xyzw $ACC, $vf2, $vf7x\n"
                 "vmadday.xyzw $ACC, $vf3, $vf7y\n"
                 "vmaddaz.xyzw $ACC, $vf4, $vf7z\n"
                 "vmaddw.xyzw $vf7, $vf5, $vf7w\n"
                 "vmulax.xyzw $ACC, $vf2, $vf8x\n"
                 "vmadday.xyzw $ACC, $vf3, $vf8y\n"
                 "vmaddaz.xyzw $ACC, $vf4, $vf8z\n"
                 "vmaddw.xyzw $vf8, $vf5, $vf8w\n"
                 "vmulax.xyzw $ACC, $vf2, $vf9x\n"
                 "vmadday.xyzw $ACC, $vf3, $vf9y\n"
                 "vmaddaz.xyzw $ACC, $vf4, $vf9z\n"
                 "vmaddw.xyzw $vf9, $vf5, $vf9w\n"
                 "sqc2 $vf6, 0x0($4)\n"
                 "sqc2 $vf7, 0x10($4)\n"
                 "sqc2 $vf8, 0x20($4)\n"
                 "sqc2 $vf9, 0x30($4)\n");
}
void OuterProduct(FVECTOR& p0, const FVECTOR& p1, const FVECTOR& p2) {
    asm volatile(
                 "lqc2 $vf4, 0x0($5)\n"
                 "lqc2 $vf5, 0x0($6)\n"
                 "vsub.w $vf6, $vf6, $vf6\n"
                 "vopmula.xyz $ACC, $vf4, $vf5\n"
                 "vopmsub.xyz $vf6, $vf5, $vf4\n"
                 "sqc2 $vf6, 0x0($4)\n");
}
FLOAT InnerProduct(const FVECTOR& p0, const FVECTOR& p1) {
    asm volatile(
                 "lqc2 $vf4, 0x0($4)\n"
                 "lqc2 $vf5, 0x0($5)\n"
                 "vaddy.x $vf6, $vf0, $vf4y\n"
                 "vaddz.x $vf7, $vf0, $vf4z\n"
                 "vmulax.x $ACC, $vf4, $vf5x\n"
                 "vmadday.x $ACC, $vf6, $vf5y\n"
                 "vmaddz.x $vf5, $vf7, $vf5z\n"
                 "qmfc2.ni $2, $vf5\n"
                 "mtc1 $2, $f0\n");
}
FLOAT InnerProduct(const FVECTOR& p0) {
    asm volatile(
                 "lqc2 $vf4, 0x0($4)\n"
                 "vaddy.x $vf6, $vf0, $vf4y\n"
                 "vaddz.x $vf7, $vf0, $vf4z\n"
                 "vmulax.x $ACC, $vf4, $vf4x\n"
                 "vmadday.x $ACC, $vf6, $vf4y\n"
                 "vmaddz.x $vf5, $vf7, $vf4z\n"
                 "qmfc2.ni $2, $vf5\n"
                 "mtc1 $2, $f0\n");
}
FLOAT InnerProductPW0(const FVECTOR& p0, const FVECTOR& p1) {
    asm volatile(
                 "lqc2 $vf4, 0x0($4)\n"
                 "lqc2 $vf5, 0x0($5)\n"
                 "vaddy.x $vf6, $vf0, $vf4y\n"
                 "vaddz.x $vf7, $vf0, $vf4z\n"
                 "vaddaw.x $ACC, $vf0, $vf4w\n"
                 "vmaddax.x $ACC, $vf4, $vf5x\n"
                 "vmadday.x $ACC, $vf6, $vf5y\n"
                 "vmaddz.x $vf5, $vf7, $vf5z\n"
                 "qmfc2.ni $2, $vf5\n"
                 "mtc1 $2, $f0\n");
}
FLOAT InnerProductPW1(const FVECTOR& p0, const FVECTOR& p1) {
    asm volatile(
                 "lqc2 $vf4, 0x0($4)\n"
                 "lqc2 $vf5, 0x0($5)\n"
                 "vaddy.x $vf6, $vf0, $vf4y\n"
                 "vaddz.x $vf7, $vf0, $vf4z\n"
                 "vaddaw.x $ACC, $vf0, $vf5w\n"
                 "vmaddax.x $ACC, $vf4, $vf5x\n"
                 "vmadday.x $ACC, $vf6, $vf5y\n"
                 "vmaddz.x $vf5, $vf7, $vf5z\n"
                 "qmfc2.ni $2, $vf5\n"
                 "mtc1 $2, $f0\n");
}
void Normalize(FVECTOR& p0, const FVECTOR& p1) {
    asm volatile(
                 "lqc2 $vf4, 0x0($5)\n"
                 "vaddy.x $vf5, $vf0, $vf4y\n"
                 "vaddz.x $vf6, $vf0, $vf4z\n"
                 "vmulax.x $ACC, $vf4, $vf4x\n"
                 "vmadday.x $ACC, $vf5, $vf4y\n"
                 "vmaddz.x $vf5, $vf6, $vf4z\n"
                 "vrsqrt $Q, $vf0w, $vf5x\n"
                 "vsub.xyzw $vf6, $vf0, $vf0\n"
                 "vwaitq\n"
                 "vmulq.xyz $vf6, $vf4, $Q\n"
                 "sqc2 $vf6, 0x0($4)\n");
}
void DivVector(FVECTOR& p0, const FVECTOR& p1, float p2) {
    asm volatile(
                 "lqc2 $vf4, 0x0($5)\n"
                 "mfc1 $8, $f12\n"
                 "qmtc2.ni $8, $vf5\n"
                 "vdiv $Q, $vf0w, $vf5x\n"
                 "vwaitq\n"
                 "vmulq.xyzw $vf4, $vf4, $Q\n"
                 "sqc2 $vf4, 0x0($4)\n");
}
void DivVectorXYZ(FVECTOR& p0, const FVECTOR& p1, float p2) {
    asm volatile(
                 "lqc2 $vf4, 0x0($5)\n"
                 "mfc1 $8, $f12\n"
                 "qmtc2.ni $8, $vf5\n"
                 "vdiv $Q, $vf0w, $vf5x\n"
                 "vwaitq\n"
                 "vmulq.xyz $vf4, $vf4, $Q\n"
                 "sqc2 $vf4, 0x0($4)\n");
}
void AddVector(FVECTOR& p0, const FVECTOR& p1, const FVECTOR& p2) {
    asm volatile(
                 "lqc2 $vf4, 0x0($5)\n"
                 "lqc2 $vf5, 0x0($6)\n"
                 "vadd.xyzw $vf4, $vf4, $vf5\n"
                 "sqc2 $vf4, 0x0($4)\n");
}
void SubVector(FVECTOR& p0, const FVECTOR& p1, const FVECTOR& p2) {
    asm volatile(
                 "lqc2 $vf4, 0x0($5)\n"
                 "lqc2 $vf5, 0x0($6)\n"
                 "vsub.xyzw $vf4, $vf4, $vf5\n"
                 "sqc2 $vf4, 0x0($4)\n");
}
void MulVector(FVECTOR& p0, const FVECTOR& p1, const FVECTOR& p2) {
    asm volatile(
                 "lqc2 $vf4, 0x0($5)\n"
                 "lqc2 $vf5, 0x0($6)\n"
                 "vmul.xyzw $vf4, $vf4, $vf5\n"
                 "sqc2 $vf4, 0x0($4)\n");
}
void ScaleVector(FVECTOR& p0, const FVECTOR& p1, float p2) {
    asm volatile(
                 "lqc2 $vf4, 0x0($5)\n"
                 "mfc1 $3, $f12\n"
                 "qmtc2.ni $3, $vf5\n"
                 "vmulx.xyzw $vf4, $vf4, $vf5x\n"
                 "sqc2 $vf4, 0x0($4)\n");
}
void ScaleVectorXYZ(FVECTOR& p0, const FVECTOR& p1, float p2) {
    asm volatile(
                 "lqc2 $vf4, 0x0($5)\n"
                 "mfc1 $3, $f12\n"
                 "qmtc2.ni $3, $vf5\n"
                 "vmulx.xyz $vf4, $vf4, $vf5x\n"
                 "sqc2 $vf4, 0x0($4)\n");
}
void FTOI0Vector(IVECTOR& p0, const FVECTOR& p1) {
    asm volatile(
                 "lqc2 $vf4, 0x0($5)\n"
                 "vftoi0.xyzw $vf4, $vf4\n"
                 "sqc2 $vf4, 0x0($4)\n");
}
void FTOI4Vector(IVECTOR& p0, const FVECTOR& p1) {
    asm volatile(
                 "lqc2 $vf4, 0x0($5)\n"
                 "vftoi4.xyzw $vf4, $vf4\n"
                 "sqc2 $vf4, 0x0($4)\n");
}
void FTOI12Vector(IVECTOR& p0, const FVECTOR& p1) {
    asm volatile(
                 "lqc2 $vf4, 0x0($5)\n"
                 "vftoi12.xyzw $vf4, $vf4\n"
                 "sqc2 $vf4, 0x0($4)\n");
}
void FTOI15Vector(IVECTOR& p0, const FVECTOR& p1) {
    asm volatile(
                 "lqc2 $vf4, 0x0($5)\n"
                 "vftoi15.xyzw $vf4, $vf4\n"
                 "sqc2 $vf4, 0x0($4)\n");
}
void ITOF0Vector(FVECTOR& p0, const IVECTOR& p1) {
    asm volatile(
                 "lqc2 $vf4, 0x0($5)\n"
                 "vitof0.xyzw $vf4, $vf4\n"
                 "sqc2 $vf4, 0x0($4)\n");
}
void ITOF4Vector(FVECTOR& p0, const IVECTOR& p1) {
    asm volatile(
                 "lqc2 $vf4, 0x0($5)\n"
                 "vitof4.xyzw $vf4, $vf4\n"
                 "sqc2 $vf4, 0x0($4)\n");
}
void ITOF12Vector(FVECTOR& p0, const IVECTOR& p1) {
    asm volatile(
                 "lqc2 $vf4, 0x0($5)\n"
                 "vitof12.xyzw $vf4, $vf4\n"
                 "sqc2 $vf4, 0x0($4)\n");
}
void ITOF15Vector(FVECTOR& p0, const IVECTOR& p1) {
    asm volatile(
                 "lqc2 $vf4, 0x0($5)\n"
                 "vitof15.xyzw $vf4, $vf4\n"
                 "sqc2 $vf4, 0x0($4)\n");
}
void AddVectorXYZ(FVECTOR& p0, const FVECTOR& p1, const FVECTOR& p2) {
    asm volatile(
                 "lqc2 $vf4, 0x0($5)\n"
                 "lqc2 $vf5, 0x0($6)\n"
                 "vadd.xyz $vf4, $vf4, $vf5\n"
                 "sqc2 $vf4, 0x0($4)\n");
}
void SubVectorXYZ(FVECTOR& p0, const FVECTOR& p1, const FVECTOR& p2) {
    asm volatile(
                 "lqc2 $vf4, 0x0($5)\n"
                 "lqc2 $vf5, 0x0($6)\n"
                 "vsub.xyz $vf4, $vf4, $vf5\n"
                 "sqc2 $vf4, 0x0($4)\n");
}
void MulVectorXYZ(FVECTOR& p0, const FVECTOR& p1, const FVECTOR& p2) {
    asm volatile(
                 "lqc2 $vf4, 0x0($5)\n"
                 "lqc2 $vf5, 0x0($6)\n"
                 "vmul.xyz $vf4, $vf4, $vf5\n"
                 "sqc2 $vf4, 0x0($4)\n");
}
void ApplyMatrixXYZ(FVECTOR& p0, const FMATRIX& p1, const FVECTOR& p2) {
    asm volatile(
                 "lqc2 $vf4, 0x0($6)\n"
                 "lqc2 $vf5, 0x0($5)\n"
                 "lqc2 $vf6, 0x10($5)\n"
                 "lqc2 $vf7, 0x20($5)\n"
                 "lqc2 $vf8, 0x30($5)\n"
                 "vmulax.xyzw $ACC, $vf5, $vf4x\n"
                 "vmadday.xyzw $ACC, $vf6, $vf4y\n"
                 "vmaddaz.xyzw $ACC, $vf7, $vf4z\n"
                 "vmaddw.xyz $vf4, $vf8, $vf0w\n"
                 "sqc2 $vf4, 0x0($4)\n");
}
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/System/Coord", vu0Cosf__Ff);
float vu0Sinf(float x) {
    static const float r[4] __attribute__((aligned(16))) = { 2.60188699e-06f, -0.000198074136f, 0.0083330255f,
                                                             -0.166666567f };
    float ret;

    asm volatile("lqc2 $vf5, 0x0(%0)" : : "r"(r));
    if (x > 1.5707964f) {
        x = 3.1415927f - x;
    } else if (x < -1.5707964f) {
        x = -3.1415927f - x;
    }
    asm volatile("mfc1 $8, %1\n"
                 "qmtc2.ni $8, $vf6\n"
                 "vmul.x $vf7, $vf6, $vf6\n"
                 "vmul.x $vf8, $vf7, $vf7\n"
                 "vadda.x $ACC, $vf6, $vf0\n"
                 "vmul.x $vf9, $vf7, $vf8\n"
                 "vmul.x $vf10, $vf8, $vf8\n"
                 "vmul.x $vf7, $vf7, $vf6\n"
                 "vmul.x $vf8, $vf8, $vf6\n"
                 "vmul.x $vf9, $vf9, $vf6\n"
                 "vmul.x $vf10, $vf10, $vf6\n"
                 "vmaddaw.x $ACC, $vf7, $vf5w\n"
                 "vmaddaz.x $ACC, $vf8, $vf5z\n"
                 "vmadday.x $ACC, $vf9, $vf5y\n"
                 "vmaddx.x $vf4, $vf10, $vf5x\n"
                 "qmfc2.ni $8, $vf4\n"
                 "mtc1 $8, %0\n"
                 : "=f"(ret)
                 : "f"(x)
                 : "$8");
    return ret;
}
float vu0SinfToCosf(float p0) {
    asm volatile(
                 "mfc1 $8, $f12\n"
                 "qmtc2.ni $8, $vf4\n"
                 "vaddaw.x $ACC, $vf0, $vf0w\n"
                 "vmsub.x $vf7, $vf4, $vf4\n"
                 ".word   0x4A0703BD\n"
                 "vwaitq\n"
                 "cfc2.ni $8, $vi22\n"
                 "mtc1 $8, $f0\n");
}
extern "C" void sceVu0ApplyMatrix(void* p0, void* p1, void* p2) {
    asm volatile(
                 "lqc2 $vf8, 0x0($6)\n"
                 "lqc2 $vf4, 0x0($5)\n"
                 "lqc2 $vf5, 0x10($5)\n"
                 "lqc2 $vf6, 0x20($5)\n"
                 "lqc2 $vf7, 0x30($5)\n"
                 "vmulax.xyzw $ACC, $vf4, $vf8x\n"
                 "vmadday.xyzw $ACC, $vf5, $vf8y\n"
                 "vmaddaz.xyzw $ACC, $vf6, $vf8z\n"
                 "vmaddw.xyzw $vf9, $vf7, $vf8w\n"
                 "sqc2 $vf9, 0x0($4)\n");
}
extern "C" void sceVu0MulMatrix(void* p0, void* p1, void* p2) {
    asm volatile(
                 "lqc2 $vf8, 0x0($5)\n"
                 "lqc2 $vf9, 0x10($5)\n"
                 "lqc2 $vf10, 0x20($5)\n"
                 "lqc2 $vf11, 0x30($5)\n"
                 "lqc2 $vf4, 0x0($6)\n"
                 "lqc2 $vf5, 0x10($6)\n"
                 "lqc2 $vf6, 0x20($6)\n"
                 "lqc2 $vf7, 0x30($6)\n"
                 "vmulax.xyzw $ACC, $vf8, $vf4x\n"
                 "vmadday.xyzw $ACC, $vf9, $vf4y\n"
                 "vmaddaz.xyzw $ACC, $vf10, $vf4z\n"
                 "vmaddw.xyzw $vf4, $vf11, $vf4w\n"
                 "vmulax.xyzw $ACC, $vf8, $vf5x\n"
                 "vmadday.xyzw $ACC, $vf9, $vf5y\n"
                 "vmaddaz.xyzw $ACC, $vf10, $vf5z\n"
                 "vmaddw.xyzw $vf5, $vf11, $vf5w\n"
                 "vmulax.xyzw $ACC, $vf8, $vf6x\n"
                 "vmadday.xyzw $ACC, $vf9, $vf6y\n"
                 "vmaddaz.xyzw $ACC, $vf10, $vf6z\n"
                 "vmaddw.xyzw $vf6, $vf11, $vf6w\n"
                 "vmulax.xyzw $ACC, $vf8, $vf7x\n"
                 "vmadday.xyzw $ACC, $vf9, $vf7y\n"
                 "vmaddaz.xyzw $ACC, $vf10, $vf7z\n"
                 "vmaddw.xyzw $vf7, $vf11, $vf7w\n"
                 "sqc2 $vf4, 0x0($4)\n"
                 "sqc2 $vf5, 0x10($4)\n"
                 "sqc2 $vf6, 0x20($4)\n"
                 "sqc2 $vf7, 0x30($4)\n");
}
extern "C" void sceVu0OuterProduct(void* p0, void* p1, void* p2) {
    asm volatile(
                 "lqc2 $vf4, 0x0($5)\n"
                 "lqc2 $vf5, 0x0($6)\n"
                 "vsub.w $vf6, $vf6, $vf6\n"
                 "vopmula.xyz $ACC, $vf4, $vf5\n"
                 "vopmsub.xyz $vf6, $vf5, $vf4\n"
                 "sqc2 $vf6, 0x0($4)\n");
}
extern "C" float sceVu0InnerProduct(void* p0, void* p1, void* p2) {
    asm volatile(
                 "lqc2 $vf4, 0x0($4)\n"
                 "lqc2 $vf5, 0x0($5)\n"
                 "vmaxw.xyzw $vf6, $vf0, $vf0w\n"
                 "vmul.xyz $vf5, $vf4, $vf5\n"
                 "vadday.x $ACC, $vf5, $vf5y\n"
                 "vmaddz.x $vf5, $vf6, $vf5z\n"
                 "qmfc2.ni $2, $vf5\n"
                 "mtc1 $2, $f0\n");
}
extern "C" void sceVu0Normalize(void* p0, void* p1, void* p2) {
    asm volatile(
                 "lqc2 $vf4, 0x0($5)\n"
                 "vmaxw.xyzw $vf6, $vf0, $vf0w\n"
                 "vmul.xyz $vf5, $vf4, $vf4\n"
                 "vadday.x $ACC, $vf5, $vf5y\n"
                 "vmaddz.x $vf5, $vf6, $vf5z\n"
                 ".word   0x4A0503BD\n"
                 "vwaitq\n"
                 "vaddq.x $vf5, $vf0, $Q\n"
                 "vnop\n"
                 "vnop\n"
                 "vdiv $Q, $vf0w, $vf5x\n"
                 "vsub.xyzw $vf6, $vf0, $vf0\n"
                 "vwaitq\n"
                 "vmulq.xyz $vf6, $vf4, $Q\n"
                 "sqc2 $vf6, 0x0($4)\n");
}
extern "C" void sceVu0TransposeMatrix(void* p0, void* p1, void* p2) {
    asm volatile(
                 "lq $8, 0x0($5)\n"
                 "lq $9, 0x10($5)\n"
                 "lq $10, 0x20($5)\n"
                 "lq $11, 0x30($5)\n"
                 "pextlw $12, $9, $8\n"
                 "pextuw $13, $9, $8\n"
                 "pextlw $14, $11, $10\n"
                 "pextuw $15, $11, $10\n"
                 "pcpyld $8, $14, $12\n"
                 "pcpyud $9, $12, $14\n"
                 "pcpyld $10, $15, $13\n"
                 "pcpyud $11, $13, $15\n"
                 "sq $8, 0x0($4)\n"
                 "sq $9, 0x10($4)\n"
                 "sq $10, 0x20($4)\n"
                 "sq $11, 0x30($4)\n");
}
extern "C" void sceVu0InversMatrix(void* p0, void* p1, void* p2) {
    asm volatile(
                 "lq $8, 0x0($5)\n"
                 "lq $9, 0x10($5)\n"
                 "lq $10, 0x20($5)\n"
                 "lqc2 $vf4, 0x30($5)\n"
                 "vmove.xyzw $vf5, $vf4\n"
                 "vsub.xyz $vf4, $vf4, $vf4\n"
                 "vmove.xyzw $vf9, $vf4\n"
                 "qmfc2.ni $11, $vf4\n"
                 "pextlw $12, $9, $8\n"
                 "pextuw $13, $9, $8\n"
                 "pextlw $14, $11, $10\n"
                 "pextuw $15, $11, $10\n"
                 "pcpyld $8, $14, $12\n"
                 "pcpyud $9, $12, $14\n"
                 "pcpyld $10, $15, $13\n"
                 "qmtc2.ni $8, $vf6\n"
                 "qmtc2.ni $9, $vf7\n"
                 "qmtc2.ni $10, $vf8\n"
                 "vmulax.xyz $ACC, $vf6, $vf5x\n"
                 "vmadday.xyz $ACC, $vf7, $vf5y\n"
                 "vmaddz.xyz $vf4, $vf8, $vf5z\n"
                 "vsub.xyz $vf4, $vf9, $vf4\n"
                 "sq $8, 0x0($4)\n"
                 "sq $9, 0x10($4)\n"
                 "sq $10, 0x20($4)\n"
                 "sqc2 $vf4, 0x30($4)\n");
}
extern "C" void sceVu0DivVector(void* p0, void* p1, void* p2) {
    asm volatile(
                 "lqc2 $vf4, 0x0($5)\n"
                 "mfc1 $8, $f12\n"
                 "qmtc2.ni $8, $vf5\n"
                 "vdiv $Q, $vf0w, $vf5x\n"
                 "vwaitq\n"
                 "vmulq.xyzw $vf4, $vf4, $Q\n"
                 "sqc2 $vf4, 0x0($4)\n");
}
extern "C" void sceVu0DivVectorXYZ(void* p0, void* p1, void* p2) {
    asm volatile(
                 "lqc2 $vf4, 0x0($5)\n"
                 "mfc1 $8, $f12\n"
                 "qmtc2.ni $8, $vf5\n"
                 "vdiv $Q, $vf0w, $vf5x\n"
                 "vwaitq\n"
                 "vmulq.xyz $vf4, $vf4, $Q\n"
                 "sqc2 $vf4, 0x0($4)\n");
}
extern "C" void sceVu0InterVector(void* p0, void* p1, void* p2) {
    asm volatile(
                 "lqc2 $vf4, 0x0($5)\n"
                 "lqc2 $vf5, 0x0($6)\n"
                 "mfc1 $8, $f12\n"
                 "qmtc2.ni $8, $vf6\n"
                 "vsub.xyzw $vf4, $vf4, $vf5\n"
                 "vaddax.xyzw $ACC, $vf5, $vf0x\n"
                 "vmaddx.xyzw $vf4, $vf4, $vf6x\n"
                 "sqc2 $vf4, 0x0($4)\n");
}
extern "C" void sceVu0AddVector(void* p0, void* p1, void* p2) {
    asm volatile(
                 "lqc2 $vf4, 0x0($5)\n"
                 "lqc2 $vf5, 0x0($6)\n"
                 "vadd.xyzw $vf6, $vf4, $vf5\n"
                 "sqc2 $vf6, 0x0($4)\n");
}
extern "C" void sceVu0SubVector(void* p0, void* p1, void* p2) {
    asm volatile(
                 "lqc2 $vf4, 0x0($5)\n"
                 "lqc2 $vf5, 0x0($6)\n"
                 "vsub.xyzw $vf6, $vf4, $vf5\n"
                 "sqc2 $vf6, 0x0($4)\n");
}
extern "C" void sceVu0MulVector(void* p0, void* p1, void* p2) {
    asm volatile(
                 "lqc2 $vf4, 0x0($5)\n"
                 "lqc2 $vf5, 0x0($6)\n"
                 "vmul.xyzw $vf6, $vf4, $vf5\n"
                 "sqc2 $vf6, 0x0($4)\n");
}
extern "C" void sceVu0ScaleVector(void* p0, void* p1, void* p2) {
    asm volatile(
                 "lqc2 $vf4, 0x0($5)\n"
                 "mfc1 $8, $f12\n"
                 "qmtc2.ni $8, $vf5\n"
                 "vmulx.xyzw $vf6, $vf4, $vf5x\n"
                 "sqc2 $vf6, 0x0($4)\n");
}
extern "C" void sceVu0TransMatrix(void* p0, void* p1, void* p2) {
    asm volatile(
                 "lqc2 $vf4, 0x0($6)\n"
                 "lqc2 $vf5, 0x30($5)\n"
                 "lq $7, 0x0($5)\n"
                 "lq $8, 0x10($5)\n"
                 "lq $9, 0x20($5)\n"
                 "vadd.xyz $vf5, $vf5, $vf4\n"
                 "sq $7, 0x0($4)\n"
                 "sq $8, 0x10($4)\n"
                 "sq $9, 0x20($4)\n"
                 "sqc2 $vf5, 0x30($4)\n");
}
extern "C" void sceVu0CopyVector(void* p0, void* p1, void* p2) {
    asm volatile(
                 "lq $6, 0x0($5)\n"
                 "sq $6, 0x0($4)\n");
}
extern "C" void sceVu0CopyMatrix(void* p0, void* p1, void* p2) {
    asm volatile(
                 "lq $6, 0x0($5)\n"
                 "lq $7, 0x10($5)\n"
                 "lq $8, 0x20($5)\n"
                 "lq $9, 0x30($5)\n"
                 "sq $6, 0x0($4)\n"
                 "sq $7, 0x10($4)\n"
                 "sq $8, 0x20($4)\n"
                 "sq $9, 0x30($4)\n");
}
extern "C" void sceVu0FTOI4Vector(void* p0, void* p1, void* p2) {
    asm volatile(
                 "lqc2 $vf4, 0x0($5)\n"
                 "vftoi4.xyzw $vf5, $vf4\n"
                 "sqc2 $vf5, 0x0($4)\n");
}
extern "C" void sceVu0FTOI0Vector(void* p0, void* p1, void* p2) {
    asm volatile(
                 "lqc2 $vf4, 0x0($5)\n"
                 "vftoi0.xyzw $vf5, $vf4\n"
                 "sqc2 $vf5, 0x0($4)\n");
}
extern "C" void sceVu0ITOF4Vector(void* p0, void* p1, void* p2) {
    asm volatile(
                 "lqc2 $vf4, 0x0($5)\n"
                 "vitof4.xyzw $vf5, $vf4\n"
                 "sqc2 $vf5, 0x0($4)\n");
}
extern "C" void sceVu0ITOF0Vector(void* p0, void* p1, void* p2) {
    asm volatile(
                 "lqc2 $vf4, 0x0($5)\n"
                 "vitof0.xyzw $vf5, $vf4\n"
                 "sqc2 $vf5, 0x0($4)\n");
}
extern "C" void sceVu0UnitMatrix(void* p0, void* p1, void* p2) {
    asm volatile(
                 "vmr32.xyzw $vf5, $vf0\n"
                 "vmr32.xyzw $vf6, $vf5\n"
                 "vmr32.xyzw $vf7, $vf6\n"
                 "sqc2 $vf0, 0x30($4)\n"
                 "sqc2 $vf5, 0x20($4)\n"
                 "sqc2 $vf6, 0x10($4)\n"
                 "sqc2 $vf7, 0x0($4)\n");
}
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/System/Coord", _sceVu0ecossin);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/System/Coord", sceVu0RotMatrixZ);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/System/Coord", sceVu0RotMatrixX);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/System/Coord", sceVu0RotMatrixY);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/System/Coord", sceVu0RotMatrix);
extern "C" void sceVu0ClampVector(void* p0, void* p1, void* p2) {
    asm volatile(
                 "mfc1 $8, $f12\n"
                 "mfc1 $9, $f13\n"
                 "lqc2 $vf6, 0x0($5)\n"
                 "qmtc2.ni $8, $vf4\n"
                 "qmtc2.ni $9, $vf5\n"
                 "vmaxx.xyzw $vf6, $vf6, $vf4x\n"
                 "vminix.xyzw $vf6, $vf6, $vf5x\n"
                 "sqc2 $vf6, 0x0($4)\n");
}
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/System/Coord", sceVu0CameraMatrix);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/System/Coord", sceVu0NormalLightMatrix);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/System/Coord", sceVu0LightColorMatrix);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/System/Coord", sceVu0ViewScreenMatrix);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/System/Coord", sceVu0DropShadowMatrix);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/System/Coord", sceVu0RotTransPersN);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/System/Coord", sceVu0RotTransPers);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/System/Coord", sceVu0CopyVectorXYZ);
extern "C" void sceVu0InterVectorXYZ(void* p0, void* p1, void* p2) {
    asm volatile(
                 "lqc2 $vf4, 0x0($5)\n"
                 "lqc2 $vf5, 0x0($6)\n"
                 "mfc1 $8, $f12\n"
                 "qmtc2.ni $8, $vf6\n"
                 "vsub.xyz $vf4, $vf4, $vf5\n"
                 "vaddax.xyz $ACC, $vf5, $vf0x\n"
                 "vmaddx.xyz $vf4, $vf4, $vf6x\n"
                 "sqc2 $vf4, 0x0($4)\n");
}
extern "C" void sceVu0ScaleVectorXYZ(void* p0, void* p1, void* p2) {
    asm volatile(
                 "lqc2 $vf4, 0x0($5)\n"
                 "mfc1 $8, $f12\n"
                 "qmtc2.ni $8, $vf5\n"
                 "vmulx.xyz $vf4, $vf4, $vf5x\n"
                 "sqc2 $vf4, 0x0($4)\n");
}
extern "C" int sceVu0ClipScreen(void* p0, void* p1, void* p2) {
    asm volatile(
                 "vsub.xyzw $vf4, $vf0, $vf0\n"
                 "ori $2, $zero, 0x8B00\n"
                 "dsll32 $2, $2, 0\n"
                 "ori $2, $2, 0x8B00\n"
                 "dsll $2, $2, 15\n"
                 "lqc2 $vf7, 0x0($4)\n"
                 "qmtc2.ni $2, $vf6\n"
                 "ctc2.ni $zero, $vi16\n"
                 "vsub.xyzw $vf5, $vf7, $vf4\n"
                 "vsub.xy $vf5, $vf6, $vf7\n"
                 "vnop\n"
                 "vnop\n"
                 "vnop\n"
                 "vnop\n"
                 "vnop\n"
                 "cfc2.ni $2, $vi16\n"
                 "andi $2, $2, 0xC0\n");
}
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/System/Coord", sceVu0ClipScreen3);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/System/Coord", sceVu0ClipAll);
// SOURCE DATA: .rodata
