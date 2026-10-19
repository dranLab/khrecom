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
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/System/Coord", getRotTransPersMatrix__6CCoordR7FMATRIX);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/System/Coord", getRotTransPersMatrix_VF04_07__6CCoord);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/System/Coord", RotTransPers__6CCoordR7IVECTORRC7FVECTOR);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/System/Coord", RotTransPers__6CCoordR7FVECTORRC7FVECTOR);
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
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/System/Coord", ApplyMatrix33__FR7FVECTORRC7FMATRIXRC7FVECTOR);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/System/Coord", MulMatrix33__FR7FMATRIXRC7FMATRIXT1);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/System/Coord", AbsVector__FR7FVECTORRC7FVECTOR);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/System/Coord", InnerProduct4__FRC7FVECTORT0);
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
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/System/Coord", getMaxMinToVector8__FP7FVECTORRC7FVECTORT1);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/System/Coord", ViewClipMatrix__FR7FMATRIXfffff);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/System/Coord", ClipDisp__FRC7FMATRIXRC7FVECTOR);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/System/Coord", ClipDispBBox__FRC7FMATRIXPC7FVECTOR);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/System/Coord", ClipDispBBox__FPC7FVECTOR);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/System/Coord", ClipDispBBox__FRC7FMATRIXT0PC7FVECTOR);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/System/Coord", ClipDispBBox__FRC7FMATRIXN20PC7FVECTOR);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/System/Coord", ClipDispBBoxMaxMin__FPC7FVECTOR);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/System/Coord", ClipDispBBoxMaxMin__FRC7FMATRIXT0PC7FVECTOR);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/System/Coord", ApplyMatrix__FR7FVECTORRC7FMATRIXRC7FVECTOR);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/System/Coord", MulMatrix__FR7FMATRIXRC7FMATRIXT1);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/System/Coord", OuterProduct__FR7FVECTORRC7FVECTORT1);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/System/Coord", InnerProduct__FRC7FVECTORT0);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/System/Coord", InnerProduct__FRC7FVECTOR);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/System/Coord", InnerProductPW0__FRC7FVECTORT0);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/System/Coord", InnerProductPW1__FRC7FVECTORT0);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/System/Coord", Normalize__FR7FVECTORRC7FVECTOR);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/System/Coord", DivVector__FR7FVECTORRC7FVECTORf);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/System/Coord", DivVectorXYZ__FR7FVECTORRC7FVECTORf);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/System/Coord", AddVector__FR7FVECTORRC7FVECTORT1);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/System/Coord", SubVector__FR7FVECTORRC7FVECTORT1);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/System/Coord", MulVector__FR7FVECTORRC7FVECTORT1);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/System/Coord", ScaleVector__FR7FVECTORRC7FVECTORf);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/System/Coord", ScaleVectorXYZ__FR7FVECTORRC7FVECTORf);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/System/Coord", FTOI0Vector__FR7IVECTORRC7FVECTOR);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/System/Coord", FTOI4Vector__FR7IVECTORRC7FVECTOR);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/System/Coord", FTOI12Vector__FR7IVECTORRC7FVECTOR);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/System/Coord", FTOI15Vector__FR7IVECTORRC7FVECTOR);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/System/Coord", ITOF0Vector__FR7FVECTORRC7IVECTOR);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/System/Coord", ITOF4Vector__FR7FVECTORRC7IVECTOR);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/System/Coord", ITOF12Vector__FR7FVECTORRC7IVECTOR);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/System/Coord", ITOF15Vector__FR7FVECTORRC7IVECTOR);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/System/Coord", AddVectorXYZ__FR7FVECTORRC7FVECTORT1);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/System/Coord", SubVectorXYZ__FR7FVECTORRC7FVECTORT1);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/System/Coord", MulVectorXYZ__FR7FVECTORRC7FVECTORT1);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/System/Coord", ApplyMatrixXYZ__FR7FVECTORRC7FMATRIXRC7FVECTOR);
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
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/System/Coord", vu0SinfToCosf__Ff);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/System/Coord", sceVu0ApplyMatrix);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/System/Coord", sceVu0MulMatrix);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/System/Coord", sceVu0OuterProduct);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/System/Coord", sceVu0InnerProduct);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/System/Coord", sceVu0Normalize);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/System/Coord", sceVu0TransposeMatrix);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/System/Coord", sceVu0InversMatrix);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/System/Coord", sceVu0DivVector);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/System/Coord", sceVu0DivVectorXYZ);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/System/Coord", sceVu0InterVector);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/System/Coord", sceVu0AddVector);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/System/Coord", sceVu0SubVector);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/System/Coord", sceVu0MulVector);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/System/Coord", sceVu0ScaleVector);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/System/Coord", sceVu0TransMatrix);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/System/Coord", sceVu0CopyVector);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/System/Coord", sceVu0CopyMatrix);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/System/Coord", sceVu0FTOI4Vector);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/System/Coord", sceVu0FTOI0Vector);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/System/Coord", sceVu0ITOF4Vector);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/System/Coord", sceVu0ITOF0Vector);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/System/Coord", sceVu0UnitMatrix);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/System/Coord", _sceVu0ecossin);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/System/Coord", sceVu0RotMatrixZ);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/System/Coord", sceVu0RotMatrixX);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/System/Coord", sceVu0RotMatrixY);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/System/Coord", sceVu0RotMatrix);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/System/Coord", sceVu0ClampVector);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/System/Coord", sceVu0CameraMatrix);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/System/Coord", sceVu0NormalLightMatrix);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/System/Coord", sceVu0LightColorMatrix);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/System/Coord", sceVu0ViewScreenMatrix);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/System/Coord", sceVu0DropShadowMatrix);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/System/Coord", sceVu0RotTransPersN);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/System/Coord", sceVu0RotTransPers);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/System/Coord", sceVu0CopyVectorXYZ);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/System/Coord", sceVu0InterVectorXYZ);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/System/Coord", sceVu0ScaleVectorXYZ);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/System/Coord", sceVu0ClipScreen);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/System/Coord", sceVu0ClipScreen3);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/System/Coord", sceVu0ClipAll);
// SOURCE DATA: .rodata
