
#include "System/SJoint.h"
/* vtable order: CSJointT<CLinkedPtr> is completed before CCoords, CLinkedPtr after it */
class CLinkedPtr;
typedef char _instCSJointLinkedPtr[sizeof(CSJointT<CLinkedPtr>)];
#include "System/Coord.h"
#include "System/LinkedBuf.h"
#include "System/CoordMan.h"
#include "System/Debug.h"
#include "System/Tsin.h"
#include "System/HeapMan.h"

extern const FVECTOR ONE_VECTOR = { { 1.0f, 1.0f, 1.0f, 1.0f } };
extern const FMATRIX ONE_MATRIX = { { { 1.0f, 0.0f, 0.0f, 0.0f },
                                      { 0.0f, 1.0f, 0.0f, 0.0f },
                                      { 0.0f, 0.0f, 1.0f, 0.0f },
                                      { 0.0f, 0.0f, 0.0f, 1.0f } } };

void MulMatrix(FMATRIX& m0, const FMATRIX& m1, const FMATRIX& m2);
BOOL ClipDispBBox(const FVECTOR* v);
BOOL ClipDispBBoxMaxMin(const FVECTOR* v);
float vu0Sinf(float x);
void ApplyMatrix(FVECTOR& v0, const FMATRIX& m0, const FVECTOR& v1);
void ScaleVector(FVECTOR& v0, const FVECTOR& v1, FLOAT t);
void DivVector(FVECTOR& v0, const FVECTOR& v1, FLOAT q);
void MulMatrix33(FMATRIX& m0, const FMATRIX& m1, const FMATRIX& m2);
extern "C" float sinf(float x);
extern "C" float cosf(float x);
extern "C" float atan2f(float y, float x);

















void CCoords::_coord_alart(UINT nIndex) const {
    TRACE("*** CCoords:: \x94\xcd\x88\xcd\x8aO\x82\xcc Coord \x82\xaa\x8ew\x92\xe8\x82\xb3\x82\xea\x82\xc4\x82\xa2\x82\xdc\x82\xb7 this:%08x pCrdArray:%08x CrdNum:%d Index:%d ***\n",
          this, m_pCrdArray, m_nCrdNum, nIndex);
}
void CCoords::_coord_nullalart() const {
    TRACE("*** CCoords:: Coord \x82\xaa\x91\xb6\x8d\xdd\x82\xb5\x82\xdc\x82\xb9\x82\xf1 this:%08x pCrdArray:%08x CrdNum:%d ***\n", this,
          m_pCrdArray, m_nCrdNum);
}
void CCoord::fix(TIMEQ tq) {
    CCoord* pParent = m_pParent;

    if (pParent && pParent->m_hHandle != m_hHandle) {
        const CCoords* pCrds = CCoordMan::getCoords(pParent->m_hHandle);
        if (!pCrds || !pCrds->Crd()) {
            TRACE(" CCoord : \x83" "A\x83^\x83" "b\x83`\x90\xe6\x82\xcc\x90" "e\x82\xaa\x82\xa2\x82\xc8\x82\xad\x82\xc8\x82\xc1\x82\xc4\x82\xa2\x82\xdc\x82\xb7!! this:%08x Parent:%08x Ex:%08x ExP:%08x\n",
                  this, pParent, &CCoordMan::getExceptCoord(FALSE), CCoordMan::getExceptCoord(FALSE).m_pParent);
            m_pParent = __null;
            m_Local = m_World;
            m_Rot.v[0] = m_World.v[0];
            m_Rot.v[1] = m_World.v[1];
            m_Rot.v[2] = m_World.v[2];
        }
    }
    if (pParent) {
        if (pParent->m_FixTimestamp < tq) {
            pParent->fix(tq);
        }
        if (pParent->m_WorldTimestamp >= tq || m_Flag) {
            asm volatile(
                 "lqc2 $vf2, 0x50(%1)\n"
                 "lqc2 $vf3, 0x60(%1)\n"
                 "lqc2 $vf4, 0x70(%1)\n"
                 "lqc2 $vf6, 0x0(%0)\n"
                 "lqc2 $vf7, 0x10(%0)\n"
                 "lqc2 $vf8, 0x20(%0)\n"
                 "lqc2 $vf9, 0x30(%0)\n"
                 "vmulax.xyzw $ACC, $vf2, $vf6x\n"
                 "vmadday.xyzw $ACC, $vf3, $vf6y\n"
                 "vmaddz.xyzw $vf6, $vf4, $vf6z\n"
                 "vmulax.xyzw $ACC, $vf2, $vf7x\n"
                 "vmadday.xyzw $ACC, $vf3, $vf7y\n"
                 "vmaddz.xyzw $vf7, $vf4, $vf7z\n"
                 "vmulax.xyzw $ACC, $vf2, $vf8x\n"
                 "vmadday.xyzw $ACC, $vf3, $vf8y\n"
                 "vmaddz.xyzw $vf8, $vf4, $vf8z\n"
                 "lqc2 $vf5, 0x80(%0)\n"
                 "sqc2 $vf6, 0x50(%0)\n"
                 "sqc2 $vf7, 0x60(%0)\n"
                 "sqc2 $vf8, 0x70(%0)\n"
                 "vmulx.xyzw $vf6, $vf6, $vf5x\n"
                 "vmuly.xyzw $vf7, $vf7, $vf5y\n"
                 "vmulz.xyzw $vf8, $vf8, $vf5z\n"
                 "lqc2 $vf2, 0x90(%1)\n"
                 "lqc2 $vf3, 0xA0(%1)\n"
                 "lqc2 $vf4, 0xB0(%1)\n"
                 "lqc2 $vf5, 0xC0(%1)\n"
                 "vmulax.xyzw $ACC, $vf2, $vf9x\n"
                 "vmadday.xyzw $ACC, $vf3, $vf9y\n"
                 "vmaddaz.xyzw $ACC, $vf4, $vf9z\n"
                 "vmaddw.xyzw $vf9, $vf5, $vf9w\n"
                 "sqc2 $vf6, 0x90(%0)\n"
                 "sqc2 $vf7, 0xA0(%0)\n"
                 "sqc2 $vf8, 0xB0(%0)\n"
                 "sqc2 $vf9, 0xC0(%0)\n"
                         :
                         : "r"(this), "r"(pParent)
                         : "memory");
            m_WorldTimestamp = tq;
        }
    } else if (m_Flag) {
        m_Rot.v[0] = m_Local.v[0];
        m_Rot.v[1] = m_Local.v[1];
        m_Rot.v[2] = m_Local.v[2];
        m_World = m_Local;
        m_WorldTimestamp = tq;
    }
    _ulFixTimestamp = tq;
}
void CCoord::getRotTransPersMatrix(FMATRIX& outMat) {
    vu0LoadMatrixVF20(&m_World);
    vu0MulMatrixVF28VF20();
    vu0StoreMatrixVF16(&outMat);
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
void CCoord::recalcWorldMatrix() {
    CCoord* pParent = m_pParent;

    if (pParent && pParent->m_hHandle != m_hHandle) {
        const CCoords* pCrds = CCoordMan::getCoords(pParent->m_hHandle);
        if (!pCrds || !pCrds->Crd()) {
            TRACE(" CCoord : \x83" "A\x83^\x83" "b\x83`\x90\xe6\x82\xcc\x90" "e\x82\xaa\x82\xa2\x82\xc8\x82\xad\x82\xc8\x82\xc1\x82\xc4\x82\xa2\x82\xdc\x82\xb7!! this:%08x Parent:%08x Ex:%08x ExP:%08x\n",
                  this, pParent, &CCoordMan::getExceptCoord(FALSE), CCoordMan::getExceptCoord(FALSE).m_pParent);
            m_pParent = __null;
            m_Local = m_World;
            m_Rot.v[0] = m_World.v[0];
            m_Rot.v[1] = m_World.v[1];
            m_Rot.v[2] = m_World.v[2];
        }
    }
    if (pParent) {
        asm volatile(
                     "lqc2 $vf2, 0x50(%1)\n"
                     "lqc2 $vf3, 0x60(%1)\n"
                     "lqc2 $vf4, 0x70(%1)\n"
                     "lqc2 $vf6, 0x0(%0)\n"
                     "lqc2 $vf7, 0x10(%0)\n"
                     "lqc2 $vf8, 0x20(%0)\n"
                     "lqc2 $vf9, 0x30(%0)\n"
                     "vmulax.xyzw $ACC, $vf2, $vf6x\n"
                     "vmadday.xyzw $ACC, $vf3, $vf6y\n"
                     "vmaddz.xyzw $vf6, $vf4, $vf6z\n"
                     "vmulax.xyzw $ACC, $vf2, $vf7x\n"
                     "vmadday.xyzw $ACC, $vf3, $vf7y\n"
                     "vmaddz.xyzw $vf7, $vf4, $vf7z\n"
                     "vmulax.xyzw $ACC, $vf2, $vf8x\n"
                     "vmadday.xyzw $ACC, $vf3, $vf8y\n"
                     "vmaddz.xyzw $vf8, $vf4, $vf8z\n"
                     "lqc2 $vf5, 0x80(%0)\n"
                     "sqc2 $vf6, 0x50(%0)\n"
                     "sqc2 $vf7, 0x60(%0)\n"
                     "sqc2 $vf8, 0x70(%0)\n"
                     "vmulx.xyzw $vf6, $vf6, $vf5x\n"
                     "vmuly.xyzw $vf7, $vf7, $vf5y\n"
                     "vmulz.xyzw $vf8, $vf8, $vf5z\n"
                     "lqc2 $vf2, 0x90(%1)\n"
                     "lqc2 $vf3, 0xA0(%1)\n"
                     "lqc2 $vf4, 0xB0(%1)\n"
                     "lqc2 $vf5, 0xC0(%1)\n"
                     "vmulax.xyzw $ACC, $vf2, $vf9x\n"
                     "vmadday.xyzw $ACC, $vf3, $vf9y\n"
                     "vmaddaz.xyzw $ACC, $vf4, $vf9z\n"
                     "vmaddw.xyzw $vf9, $vf5, $vf9w\n"
                     "sqc2 $vf6, 0x90(%0)\n"
                     "sqc2 $vf7, 0xA0(%0)\n"
                     "sqc2 $vf8, 0xB0(%0)\n"
                     "sqc2 $vf9, 0xC0(%0)\n"
                     :
                     : "r"(this), "r"(pParent)
                     : "memory");
    } else {
        m_Rot.v[0] = m_Local.v[0];
        m_Rot.v[1] = m_Local.v[1];
        m_Rot.v[2] = m_Local.v[2];
        m_World = m_Local;
    }
    m_Flag = 0;
}
void CCoord::recalcLocalMatrix() {
    FMATRIX mMat;
    FVECTOR vScl;

    CCoord* pParent = m_pParent;

    if (pParent && pParent->m_hHandle != m_hHandle) {
        const CCoords* pCrds = CCoordMan::getCoords(pParent->m_hHandle);
        if (!pCrds || !pCrds->Crd()) {
            TRACE(" CCoord : \x83" "A\x83^\x83" "b\x83`\x90\xe6\x82\xcc\x90" "e\x82\xaa\x82\xa2\x82\xc8\x82\xad\x82\xc8\x82\xc1\x82\xc4\x82\xa2\x82\xdc\x82\xb7!! this:%08x Parent:%08x Ex:%08x ExP:%08x\n",
                  this, pParent, &CCoordMan::getExceptCoord(FALSE), CCoordMan::getExceptCoord(FALSE).m_pParent);
            m_pParent = __null;
            m_Local = m_World;
            m_Rot.v[0] = m_World.v[0];
            m_Rot.v[1] = m_World.v[1];
            m_Rot.v[2] = m_World.v[2];
        }
    }
    if (pParent) {
        DivVector(m_Rot.v[0], m_World.v[0], m_fScaleX);
        DivVector(m_Rot.v[1], m_World.v[1], m_fScaleY);
        DivVector(m_Rot.v[2], m_World.v[2], m_fScaleZ);
        InversMatrix(mMat, pParent->m_Rot);
        MulMatrix33(m_Local, mMat, m_Rot);
        InversMatrix(mMat, pParent->m_World);
        ApplyMatrix(m_Local.v[3], mMat, m_World.v[3]);
    } else {
        m_Local = m_World;
        m_Rot.v[0] = m_World.v[0];
        m_Rot.v[1] = m_World.v[1];
        m_Rot.v[2] = m_World.v[2];
    }
}
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
            m_pCrdArray[i].m_pParent = pParent;
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
            p->resetTimestamp();
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
void ApplyMatrix33(FVECTOR& vecOut, const FMATRIX& matIn, const FVECTOR& vecIn) { vu0ApplyMatrix33(vecOut, matIn, vecIn); }
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
void spinMatrixX(FMATRIX& matOut, const FMATRIX& matIn, FLOAT fInDeg) {
    FLOAT sn = sinf(fInDeg);
    FLOAT cs = cosf(fInDeg);
    FMATRIX t;

    vu0UnitMatrix(t);
    t.m[1][1] = cs;
    t.m[1][2] = sn;
    t.m[2][1] = -sn;
    t.m[2][2] = cs;
    MulMatrix(matOut, matIn, t);
}
void spinMatrixY(FMATRIX& matOut, const FMATRIX& matIn, FLOAT fInDeg) {
    FLOAT sn = sinf(fInDeg);
    FLOAT cs = cosf(fInDeg);
    FMATRIX t;

    vu0UnitMatrix(t);
    t.m[2][2] = cs;
    t.m[2][0] = sn;
    t.m[0][0] = cs;
    t.m[0][2] = -sn;
    MulMatrix(matOut, matIn, t);
}
void spinMatrixZ(FMATRIX& matOut, const FMATRIX& matIn, FLOAT fInDeg) {
    FLOAT sn = sinf(fInDeg);
    FLOAT cs = cosf(fInDeg);
    FMATRIX t;

    vu0UnitMatrix(t);
    t.m[0][0] = cs;
    t.m[0][1] = sn;
    t.m[1][0] = -sn;
    t.m[1][1] = cs;
    MulMatrix(matOut, matIn, t);
}
void spinMatrixX(FMATRIX& matOut, const FMATRIX& matIn, INT nInDeg) {
    FLOAT sn = tsin(nInDeg);
    FLOAT cs = tcos(nInDeg);
    FMATRIX t;

    vu0UnitMatrix(t);
    t.m[1][1] = cs;
    t.m[1][2] = sn;
    t.m[2][1] = -sn;
    t.m[2][2] = cs;
    MulMatrix(matOut, matIn, t);
}
void spinMatrixY(FMATRIX& matOut, const FMATRIX& matIn, INT nInDeg) {
    FLOAT sn = tsin(nInDeg);
    FLOAT cs = tcos(nInDeg);
    FMATRIX t;

    vu0UnitMatrix(t);
    t.m[2][2] = cs;
    t.m[2][0] = sn;
    t.m[0][0] = cs;
    t.m[0][2] = -sn;
    MulMatrix(matOut, matIn, t);
}
void spinMatrixZ(FMATRIX& matOut, const FMATRIX& matIn, INT nInDeg) {
    FLOAT sn = tsin(nInDeg);
    FLOAT cs = tcos(nInDeg);
    FMATRIX t;

    vu0UnitMatrix(t);
    t.m[0][0] = cs;
    t.m[0][1] = sn;
    t.m[1][0] = -sn;
    t.m[1][1] = cs;
    MulMatrix(matOut, matIn, t);
}
void rotMatrixX(FMATRIX& matOut, const FMATRIX& matIn, INT nInDeg) {
    FLOAT sn = tsin(nInDeg);
    FLOAT cs = tcos(nInDeg);
    FMATRIX t;

    vu0UnitMatrix(t);
    t.m[1][1] = cs;
    t.m[1][2] = sn;
    t.m[2][1] = -sn;
    t.m[2][2] = cs;
    MulMatrix(matOut, t, matIn);
}
void rotMatrixY(FMATRIX& matOut, const FMATRIX& matIn, INT nInDeg) {
    FLOAT sn = tsin(nInDeg);
    FLOAT cs = tcos(nInDeg);
    FMATRIX t;

    vu0UnitMatrix(t);
    t.m[2][2] = cs;
    t.m[2][0] = sn;
    t.m[0][0] = cs;
    t.m[0][2] = -sn;
    MulMatrix(matOut, t, matIn);
}
void rotMatrixZ(FMATRIX& matOut, const FMATRIX& matIn, INT nInDeg) {
    FLOAT sn = tsin(nInDeg);
    FLOAT cs = tcos(nInDeg);
    FMATRIX t;

    vu0UnitMatrix(t);
    t.m[0][0] = cs;
    t.m[0][1] = sn;
    t.m[1][0] = -sn;
    t.m[1][1] = cs;
    MulMatrix(matOut, t, matIn);
}
void scaleMatrixSelf(FMATRIX& self, FLOAT f) {
    self.v[0] *= f;
    self.v[1] *= f;
    self.v[2] *= f;
}
void scaleMatrixSelf(FMATRIX& self, FVECTOR& v) {
    ScaleVector(self.v[0], self.v[0], v.x);
    ScaleVector(self.v[1], self.v[1], v.y);
    ScaleVector(self.v[2], self.v[2], v.z);
}
void World2Local(FVECTOR& out_v, const FMATRIX& world, const FVECTOR& in_v) {
    FMATRIX t;

    InverseMatrix(t, world);
    ApplyMatrix(out_v, t, in_v);
}
TDEG cossin2deg(FLOAT cs, FLOAT sn) {
    return (TDEG)(FLOAT)(atan2f(sn, cs) * 1800.0f / 3.14159265358979323846);
}
void World2Eurar(TDEG& swivel, TDEG& tilt, const FMATRIX& world, const FVECTOR& in_v) {
    FVECTOR v;
    FVECTOR t;
    FMATRIX m;

    World2Local(v, world, in_v);
    swivel = cossin2deg(v.z, v.x);
    m.init();
    spinMatrixY(m, m, -swivel);
    ApplyMatrix(t, m, v);
    tilt = -cossin2deg(t.z, t.y);
}
void roundPI(FVECTOR& vInOut, INT nInClip) {
    if (nInClip & 0x3) {
        FLOAT& f = vInOut.x;
        if (nInClip & 0x1) {
            f = f - (INT)(f * 0.15915494f) * 6.2831855f;
            if (f > 3.1415927f) {
                f -= 6.2831855f;
            }
        } else {
            f = f - (INT)(f * 0.15915494f) * 6.2831855f;
            if (f < -3.1415927f) {
                f += 6.2831855f;
            }
        }
    }
    if (nInClip & 0xC) {
        FLOAT& f = vInOut.y;
        if (nInClip & 0x4) {
            f = f - (INT)(f * 0.15915494f) * 6.2831855f;
            if (f > 3.1415927f) {
                f -= 6.2831855f;
            }
        } else {
            f = f - (INT)(f * 0.15915494f) * 6.2831855f;
            if (f < -3.1415927f) {
                f += 6.2831855f;
            }
        }
    }
    if (nInClip & 0x30) {
        FLOAT& f = vInOut.z;
        if (nInClip & 0x10) {
            f = f - (INT)(f * 0.15915494f) * 6.2831855f;
            if (f > 3.1415927f) {
                f -= 6.2831855f;
            }
        } else {
            f = f - (INT)(f * 0.15915494f) * 6.2831855f;
            if (f < -3.1415927f) {
                f += 6.2831855f;
            }
        }
    }
}
void getMaxMinToVector8(FVECTOR* p0, const FVECTOR& p1, const FVECTOR& p2) {
    GetMaxMinToVector8(p0, p1, p2);
}
void ViewClipMatrix(FMATRIX& outVC, FLOAT scrw, FLOAT scrh, FLOAT scrz, FLOAT zmin, FLOAT zmax) {
    asm volatile("sq $0, 0x0(%0)\n"
                 "sq $0, 0x10(%0)\n"
                 "sqc2 $vf0, 0x20(%0)\n"
                 "sq $0, 0x30(%0)\n"
                 :
                 : "r"(&outVC)
                 : "memory");
    outVC.m[0][0] = (scrz + scrz) / scrw;
    outVC.m[1][1] = (scrz + scrz) / scrh;
    outVC.m[2][2] = (zmax + zmin) / (zmax - zmin);
    outVC.m[3][2] = (zmax * -2.0f * zmin) / (zmax - zmin);
}
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
BOOL ClipDispBBox(const FMATRIX& lc, const FVECTOR* v) {
    asm volatile(
                 "lqc2 $vf4, 0x0(%0)\n"
                 "lqc2 $vf5, 0x10(%0)\n"
                 "lqc2 $vf6, 0x20(%0)\n"
                 "lqc2 $vf7, 0x30(%0)\n"
                 :
                 : "r"(&lc));
    return ClipDispBBox(v);
}
BOOL ClipDispBBox(const FVECTOR* v) {
    asm volatile(
                 "lqc2 $vf8, 0x0($4)\n"
                 "vmulax.xyzw $ACC, $vf4, $vf8x\n"
                 "vmadday.xyzw $ACC, $vf5, $vf8y\n"
                 "vmaddaz.xyzw $ACC, $vf6, $vf8z\n"
                 "vmaddw.xyzw $vf9, $vf7, $vf0w\n"
                 "addiu $2, $0, 0x3F\n"
                 "addiu $8, $0, 0x8\n"
                 "0:\n"
                 ".word 0x4BC949FF\n"
                 "lqc2 $vf8, 0x10($4)\n"
                 "vmulax.xyzw $ACC, $vf4, $vf8x\n"
                 "vmadday.xyzw $ACC, $vf5, $vf8y\n"
                 "vmaddaz.xyzw $ACC, $vf6, $vf8z\n"
                 "vmaddw.xyzw $vf9, $vf7, $vf0w\n"
                 "cfc2.ni $3, $vi18\n"
                 "and $2, $2, $3\n"
                 ".set noreorder\n"
                 "beq $0, $2, 1f\n"
                 "addiu $8, $8, -0x1\n"
                 "bne $0, $8, 0b\n"
                 "addiu $4, $4, 0x10\n"
                 ".set reorder\n"
                 "1:\n"
                 :
                 :
                 : "$2", "$3", "$4", "$8");
}
BOOL ClipDispBBox(const FMATRIX& wc, const FMATRIX& lw, const FVECTOR* v) {
    asm volatile(
                 "lqc2 $vf8, 0x0(%0)\n"
                 "lqc2 $vf9, 0x10(%0)\n"
                 "lqc2 $vf10, 0x20(%0)\n"
                 "lqc2 $vf11, 0x30(%0)\n"
                 "lqc2 $vf4, 0x0(%1)\n"
                 "lqc2 $vf5, 0x10(%1)\n"
                 "lqc2 $vf6, 0x20(%1)\n"
                 "lqc2 $vf7, 0x30(%1)\n"
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
                 :
                 : "r"(&wc), "r"(&lw));
    return ClipDispBBox(v);
}
BOOL ClipDispBBox(const FMATRIX& vc, const FMATRIX& wv, const FMATRIX& lw, const FVECTOR* v) {
    asm volatile(
                 "lqc2 $vf8, 0x0(%1)\n"
                 "lqc2 $vf9, 0x10(%1)\n"
                 "lqc2 $vf10, 0x20(%1)\n"
                 "lqc2 $vf11, 0x30(%1)\n"
                 "lqc2 $vf4, 0x0(%2)\n"
                 "lqc2 $vf5, 0x10(%2)\n"
                 "lqc2 $vf6, 0x20(%2)\n"
                 "lqc2 $vf7, 0x30(%2)\n"
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
                 "lqc2 $vf8, 0x0(%0)\n"
                 "lqc2 $vf9, 0x10(%0)\n"
                 "lqc2 $vf10, 0x20(%0)\n"
                 "lqc2 $vf11, 0x30(%0)\n"
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
                 :
                 : "r"(&vc), "r"(&wv), "r"(&lw));
    return ClipDispBBox(v);
}
BOOL ClipDispBBoxMaxMin(const FVECTOR* v) {
    asm volatile(
                 "lqc2 $vf8, 0x0($4)\n"
                 "lqc2 $vf9, 0x10($4)\n"
                 "vmulax.xyzw $ACC, $vf4, $vf8x\n"
                 "vmaddaz.xyzw $ACC, $vf6, $vf8z\n"
                 "vmaddaw.xyzw $ACC, $vf7, $vf0w\n"
                 "vmaddy.xyzw $vf10, $vf5, $vf8y\n"
                 "vmaddy.xyzw $vf11, $vf5, $vf9y\n"
                 "addiu $2, $zero, 0x3F\n"
                 ".word   0x4BCA51FF\n"
                 ".word   0x4BCB59FF\n"
                 "vmulax.xyzw $ACC, $vf4, $vf9x\n"
                 "vmaddaz.xyzw $ACC, $vf6, $vf8z\n"
                 "vmaddaw.xyzw $ACC, $vf7, $vf0w\n"
                 "vmaddy.xyzw $vf10, $vf5, $vf8y\n"
                 "vmaddy.xyzw $vf11, $vf5, $vf9y\n"
                 "cfc2.ni $3, $vi18\n"
                 "dsrl $8, $3, 6\n"
                 "and $2, $2, $3\n"
                 "and $2, $2, $8\n"
                 ".set noreorder\n"
                 "beq $0, $2, 1f\n"
                 ".word   0x4BCA51FF\n"
                 ".set reorder\n"
                 ".word   0x4BCB59FF\n"
                 "vmulax.xyzw $ACC, $vf4, $vf8x\n"
                 "vmaddaz.xyzw $ACC, $vf6, $vf9z\n"
                 "vmaddaw.xyzw $ACC, $vf7, $vf0w\n"
                 "vmaddy.xyzw $vf10, $vf5, $vf8y\n"
                 "vmaddy.xyzw $vf11, $vf5, $vf9y\n"
                 "cfc2.ni $3, $vi18\n"
                 "dsrl $8, $3, 6\n"
                 "and $2, $2, $3\n"
                 "and $2, $2, $8\n"
                 ".set noreorder\n"
                 "beq $0, $2, 1f\n"
                 ".word   0x4BCA51FF\n"
                 ".set reorder\n"
                 ".word   0x4BCB59FF\n"
                 "vmulax.xyzw $ACC, $vf4, $vf9x\n"
                 "vmaddaz.xyzw $ACC, $vf6, $vf9z\n"
                 "vmaddaw.xyzw $ACC, $vf7, $vf0w\n"
                 "vmaddy.xyzw $vf10, $vf5, $vf8y\n"
                 "vmaddy.xyzw $vf11, $vf5, $vf9y\n"
                 "cfc2.ni $3, $vi18\n"
                 "dsrl $8, $3, 6\n"
                 "and $2, $2, $3\n"
                 "and $2, $2, $8\n"
                 ".set noreorder\n"
                 "beq $0, $2, 1f\n"
                 ".word   0x4BCA51FF\n"
                 ".set reorder\n"
                 ".word   0x4BCB59FF\n"
                 "vnop\n"
                 "vnop\n"
                 "vnop\n"
                 "vnop\n"
                 "vnop\n"
                 "cfc2.ni $3, $vi18\n"
                 "dsrl $8, $3, 6\n"
                 "and $2, $2, $3\n"
                 "and $2, $2, $8\n"
                 "1:\n"
                 :
                 :
                 : "$2", "$3", "$8");
}
BOOL ClipDispBBoxMaxMin(const FMATRIX& wc, const FMATRIX& lw, const FVECTOR* v) {
    asm volatile(
                 "lqc2 $vf8, 0x0(%0)\n"
                 "lqc2 $vf9, 0x10(%0)\n"
                 "lqc2 $vf10, 0x20(%0)\n"
                 "lqc2 $vf11, 0x30(%0)\n"
                 "lqc2 $vf4, 0x0(%1)\n"
                 "lqc2 $vf5, 0x10(%1)\n"
                 "lqc2 $vf6, 0x20(%1)\n"
                 "lqc2 $vf7, 0x30(%1)\n"
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
                 :
                 : "r"(&wc), "r"(&lw));
    return ClipDispBBoxMaxMin(v);
}
void ApplyMatrix(FVECTOR& v0, const FMATRIX& m0, const FVECTOR& v1) { vu0ApplyMatrix(v0, m0, v1); }
void MulMatrix(FMATRIX& res, const FMATRIX& m0, const FMATRIX& m1) { vu0MulMatrix(res, m0, m1); }
void OuterProduct(FVECTOR& v0, const FVECTOR& v1, const FVECTOR& v2) { vu0OuterProduct(v0, v1, v2); }
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
void Normalize(FVECTOR& v0, const FVECTOR& v1) { vu0Normalize(v0, v1); }
void DivVector(FVECTOR& v0, const FVECTOR& v1, FLOAT q) { vu0DivVector(v0, v1, q); }
void DivVectorXYZ(FVECTOR& v0, const FVECTOR& v1, FLOAT q) { vu0DivVectorXYZ(v0, v1, q); }
void AddVector(FVECTOR& v0, const FVECTOR& v1, const FVECTOR& v2) { vu0AddVector(v0, v1, v2); }
void SubVector(FVECTOR& v0, const FVECTOR& v1, const FVECTOR& v2) { vu0SubVector(v0, v1, v2); }
void MulVector(FVECTOR& v0, const FVECTOR& v1, const FVECTOR& v2) { vu0MulVector(v0, v1, v2); }
void ScaleVector(FVECTOR& v0, const FVECTOR& v1, FLOAT t) { vu0ScaleVector(v0, v1, t); }
void ScaleVectorXYZ(FVECTOR& v0, const FVECTOR& v1, FLOAT t) { vu0ScaleVectorXYZ(v0, v1, t); }
void FTOI0Vector(IVECTOR& v0, const FVECTOR& v1) { vu0FTOI0Vector(v0, v1); }
void FTOI4Vector(IVECTOR& v0, const FVECTOR& v1) { vu0FTOI4Vector(v0, v1); }
void FTOI12Vector(IVECTOR& v0, const FVECTOR& v1) { vu0FTOI12Vector(v0, v1); }
void FTOI15Vector(IVECTOR& v0, const FVECTOR& v1) { vu0FTOI15Vector(v0, v1); }
void ITOF0Vector(FVECTOR& v0, const IVECTOR& v1) { vu0ITOF0Vector(v0, v1); }
void ITOF4Vector(FVECTOR& v0, const IVECTOR& v1) { vu0ITOF4Vector(v0, v1); }
void ITOF12Vector(FVECTOR& v0, const IVECTOR& v1) { vu0ITOF12Vector(v0, v1); }
void ITOF15Vector(FVECTOR& v0, const IVECTOR& v1) { vu0ITOF15Vector(v0, v1); }
void AddVectorXYZ(FVECTOR& v0, const FVECTOR& v1, const FVECTOR& v2) { vu0AddVectorXYZ(v0, v1, v2); }
void SubVectorXYZ(FVECTOR& v0, const FVECTOR& v1, const FVECTOR& v2) { vu0SubVectorXYZ(v0, v1, v2); }
void MulVectorXYZ(FVECTOR& v0, const FVECTOR& v1, const FVECTOR& v2) { vu0MulVectorXYZ(v0, v1, v2); }
void ApplyMatrixXYZ(FVECTOR& v0, const FMATRIX& m, const FVECTOR& v1) { vu0ApplyMatrixXYZ(v0, m, v1); }
float vu0Cosf(float x) {
    x += 1.5707964f;
    if (x > 3.1415927f) {
        x -= 6.2831855f;
    }
    return vu0Sinf(x);
}
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
extern "C" void sceVu0ApplyMatrix(float *v0, float (*m0)[4], float *v1) {
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
extern "C" void sceVu0MulMatrix(float (*m0)[4], float (*m1)[4], float (*m2)[4]) {
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
extern "C" void sceVu0OuterProduct(float *v0, float *v1, float *v2) {
    asm volatile(
                 "lqc2 $vf4, 0x0($5)\n"
                 "lqc2 $vf5, 0x0($6)\n"
                 "vsub.w $vf6, $vf6, $vf6\n"
                 "vopmula.xyz $ACC, $vf4, $vf5\n"
                 "vopmsub.xyz $vf6, $vf5, $vf4\n"
                 "sqc2 $vf6, 0x0($4)\n");
}
extern "C" float sceVu0InnerProduct(float *v0, float *v1) {
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
extern "C" void sceVu0Normalize(float *v0, float *v1) {
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
extern "C" void sceVu0TransposeMatrix(float (*m0)[4], float (*m1)[4]) {
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
extern "C" void sceVu0InversMatrix(float (*m0)[4], float (*m1)[4]) {
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
extern "C" void sceVu0DivVector(float *v0, float *v1, float q) {
    asm volatile(
                 "lqc2 $vf4, 0x0($5)\n"
                 "mfc1 $8, $f12\n"
                 "qmtc2.ni $8, $vf5\n"
                 "vdiv $Q, $vf0w, $vf5x\n"
                 "vwaitq\n"
                 "vmulq.xyzw $vf4, $vf4, $Q\n"
                 "sqc2 $vf4, 0x0($4)\n");
}
extern "C" void sceVu0DivVectorXYZ(float *v0, float *v1, float q) {
    asm volatile(
                 "lqc2 $vf4, 0x0($5)\n"
                 "mfc1 $8, $f12\n"
                 "qmtc2.ni $8, $vf5\n"
                 "vdiv $Q, $vf0w, $vf5x\n"
                 "vwaitq\n"
                 "vmulq.xyz $vf4, $vf4, $Q\n"
                 "sqc2 $vf4, 0x0($4)\n");
}
extern "C" void sceVu0InterVector(float *v0, float *v1, float *v2, float t) {
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
extern "C" void sceVu0AddVector(float *v0, float *v1, float *v2) {
    asm volatile(
                 "lqc2 $vf4, 0x0($5)\n"
                 "lqc2 $vf5, 0x0($6)\n"
                 "vadd.xyzw $vf6, $vf4, $vf5\n"
                 "sqc2 $vf6, 0x0($4)\n");
}
extern "C" void sceVu0SubVector(float *v0, float *v1, float *v2) {
    asm volatile(
                 "lqc2 $vf4, 0x0($5)\n"
                 "lqc2 $vf5, 0x0($6)\n"
                 "vsub.xyzw $vf6, $vf4, $vf5\n"
                 "sqc2 $vf6, 0x0($4)\n");
}
extern "C" void sceVu0MulVector(float *v0, float *v1, float *v2) {
    asm volatile(
                 "lqc2 $vf4, 0x0($5)\n"
                 "lqc2 $vf5, 0x0($6)\n"
                 "vmul.xyzw $vf6, $vf4, $vf5\n"
                 "sqc2 $vf6, 0x0($4)\n");
}
extern "C" void sceVu0ScaleVector(float *v0, float *v1, float t) {
    asm volatile(
                 "lqc2 $vf4, 0x0($5)\n"
                 "mfc1 $8, $f12\n"
                 "qmtc2.ni $8, $vf5\n"
                 "vmulx.xyzw $vf6, $vf4, $vf5x\n"
                 "sqc2 $vf6, 0x0($4)\n");
}
extern "C" void sceVu0TransMatrix(float (*m0)[4], float (*m1)[4], float *tv) {
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
extern "C" void sceVu0CopyVector(float *v0, float *v1) {
    asm volatile(
                 "lq $6, 0x0($5)\n"
                 "sq $6, 0x0($4)\n");
}
extern "C" void sceVu0CopyMatrix(float (*m0)[4], float (*m1)[4]) {
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
extern "C" void sceVu0FTOI4Vector(int *v0, float *v1) {
    asm volatile(
                 "lqc2 $vf4, 0x0($5)\n"
                 "vftoi4.xyzw $vf5, $vf4\n"
                 "sqc2 $vf5, 0x0($4)\n");
}
extern "C" void sceVu0FTOI0Vector(int *v0, float *v1) {
    asm volatile(
                 "lqc2 $vf4, 0x0($5)\n"
                 "vftoi0.xyzw $vf5, $vf4\n"
                 "sqc2 $vf5, 0x0($4)\n");
}
extern "C" void sceVu0ITOF4Vector(float *v0, int *v1) {
    asm volatile(
                 "lqc2 $vf4, 0x0($5)\n"
                 "vitof4.xyzw $vf5, $vf4\n"
                 "sqc2 $vf5, 0x0($4)\n");
}
extern "C" void sceVu0ITOF0Vector(float *v0, int *v1) {
    asm volatile(
                 "lqc2 $vf4, 0x0($5)\n"
                 "vitof0.xyzw $vf5, $vf4\n"
                 "sqc2 $vf5, 0x0($4)\n");
}
extern "C" void sceVu0UnitMatrix(float (*m0)[4]) {
    asm volatile(
                 "vmr32.xyzw $vf5, $vf0\n"
                 "vmr32.xyzw $vf6, $vf5\n"
                 "vmr32.xyzw $vf7, $vf6\n"
                 "sqc2 $vf0, 0x30($4)\n"
                 "sqc2 $vf5, 0x20($4)\n"
                 "sqc2 $vf6, 0x10($4)\n"
                 "sqc2 $vf7, 0x0($4)\n");
}
/* the VU0 library's sin/cos coefficients (static in the SDK, referenced only from asm: kept global here) */
float S5432[4] __attribute__((aligned(16))) = { 2.60188699e-06f, -0.000198074136f, 0.0083330255f, -0.166666567f };

/* static in the SDK; called only from the asm below (non-static here, so that it is kept) */
extern "C" void _sceVu0ecossin(float t) {
    asm volatile(
                 "vmul.x $vf7, $vf6, $vf6\n"
                 "la $8, S5432\n"
                 "lqc2 $vf5, 0x0($8)\n"
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
                 "vaddaw.x $ACC, $vf0, $vf0w\n"
                 "vmsub.x $vf7, $vf4, $vf4\n"
                 ".word 0x4A0703BD\n"
                 "vaddx.xy $vf4, $vf0, $vf4x\n"
                 "bnez $7, 1f\n"
                 "vaddw.x $vf7, $vf0, $vf0w\n"
                 "b 2f\n"
                 "1:\n"
                 "vsubw.x $vf7, $vf0, $vf0w\n"
                 "2:\n"
                 "vwaitq\n"
                 "vmulq.x $vf4, $vf7, $Q\n"
                 :
                 :
                 : "$8");
}
extern "C" void sceVu0RotMatrixZ(float (*m0)[4], float (*m1)[4], float rz) {
    asm volatile(
                 "mtc1 $0, $f0\n"
                 "c.lt.s $f12, $f0\n"
                 "li.s $f0, 1.57079637050628662109e0\n"
                 "bc1f 1f\n"
                 "add.s $f12, $f0, $f12\n"
                 "li $7, 1\n"
                 "j 2f\n"
                 "1:\n"
                 "sub.s $f12, $f0, $f12\n"
                 "move $7, $0\n"
                 "2:\n"
                 "mfc1 $8, $f12\n"
                 "qmtc2.ni $8, $vf6\n"
                 "move $6, $31\n"
                 "jal _sceVu0ecossin\n"
                 "daddu $31, $6, $zero\n"
                 "lqc2 $vf7, 0x0($5)\n"
                 "lqc2 $vf8, 0x10($5)\n"
                 "vaddx.y $vf5, $vf0, $vf4x\n"
                 "vaddy.x $vf5, $vf0, $vf4y\n"
                 "vsubx.x $vf6, $vf0, $vf4x\n"
                 "vaddy.y $vf6, $vf0, $vf4y\n"
                 "lqc2 $vf9, 0x20($5)\n"
                 "lqc2 $vf10, 0x30($5)\n"
                 "vmulax.xy $ACC, $vf5, $vf7x\n"
                 "vmaddy.xy $vf7, $vf6, $vf7y\n"
                 "vmulax.xy $ACC, $vf5, $vf8x\n"
                 "vmaddy.xy $vf8, $vf6, $vf8y\n"
                 "vmulax.xy $ACC, $vf5, $vf9x\n"
                 "vmaddy.xy $vf9, $vf6, $vf9y\n"
                 "vmulax.xy $ACC, $vf5, $vf10x\n"
                 "vmaddy.xy $vf10, $vf6, $vf10y\n"
                 "sqc2 $vf7, 0x0($4)\n"
                 "sqc2 $vf8, 0x10($4)\n"
                 "sqc2 $vf9, 0x20($4)\n"
                 "sqc2 $vf10, 0x30($4)\n"
                 :
                 :
                 : "$6", "$7", "$8", "$f0");
}
extern "C" void sceVu0RotMatrixX(float (*m0)[4], float (*m1)[4], float rx) {
    asm volatile(
                 "mtc1 $0, $f0\n"
                 "c.lt.s $f12, $f0\n"
                 "li.s $f0, 1.57079637050628662109e0\n"
                 "bc1f 1f\n"
                 "add.s $f12, $f0, $f12\n"
                 "li $7, 1\n"
                 "j 2f\n"
                 "1:\n"
                 "sub.s $f12, $f0, $f12\n"
                 "move $7, $0\n"
                 "2:\n"
                 "mfc1 $8, $f12\n"
                 "qmtc2.ni $8, $vf6\n"
                 "move $6, $31\n"
                 "jal _sceVu0ecossin\n"
                 "daddu $31, $6, $zero\n"
                 "lqc2 $vf7, 0x0($5)\n"
                 "lqc2 $vf8, 0x10($5)\n"
                 "vaddx.z $vf5, $vf0, $vf4x\n"
                 "vaddy.y $vf5, $vf0, $vf4y\n"
                 "vsubx.y $vf6, $vf0, $vf4x\n"
                 "vaddy.z $vf6, $vf0, $vf4y\n"
                 "lqc2 $vf9, 0x20($5)\n"
                 "lqc2 $vf10, 0x30($5)\n"
                 "vmulay.yz $ACC, $vf5, $vf7y\n"
                 "vmaddz.yz $vf7, $vf6, $vf7z\n"
                 "vmulay.yz $ACC, $vf5, $vf8y\n"
                 "vmaddz.yz $vf8, $vf6, $vf8z\n"
                 "vmulay.yz $ACC, $vf5, $vf9y\n"
                 "vmaddz.yz $vf9, $vf6, $vf9z\n"
                 "vmulay.yz $ACC, $vf5, $vf10y\n"
                 "vmaddz.yz $vf10, $vf6, $vf10z\n"
                 "sqc2 $vf7, 0x0($4)\n"
                 "sqc2 $vf8, 0x10($4)\n"
                 "sqc2 $vf9, 0x20($4)\n"
                 "sqc2 $vf10, 0x30($4)\n"
                 :
                 :
                 : "$6", "$7", "$8", "$f0");
}
extern "C" void sceVu0RotMatrixY(float (*m0)[4], float (*m1)[4], float ry) {
    asm volatile(
                 "mtc1 $0, $f0\n"
                 "c.lt.s $f12, $f0\n"
                 "li.s $f0, 1.57079637050628662109e0\n"
                 "bc1f 1f\n"
                 "add.s $f12, $f0, $f12\n"
                 "li $7, 1\n"
                 "j 2f\n"
                 "1:\n"
                 "sub.s $f12, $f0, $f12\n"
                 "move $7, $0\n"
                 "2:\n"
                 "mfc1 $8, $f12\n"
                 "qmtc2.ni $8, $vf6\n"
                 "move $6, $31\n"
                 "jal _sceVu0ecossin\n"
                 "daddu $31, $6, $zero\n"
                 "lqc2 $vf7, 0x0($5)\n"
                 "lqc2 $vf8, 0x10($5)\n"
                 "vsubx.z $vf5, $vf0, $vf4x\n"
                 "vaddy.x $vf5, $vf0, $vf4y\n"
                 "vaddx.x $vf6, $vf0, $vf4x\n"
                 "vaddy.z $vf6, $vf0, $vf4y\n"
                 "lqc2 $vf9, 0x20($5)\n"
                 "lqc2 $vf10, 0x30($5)\n"
                 "vmulax.xz $ACC, $vf5, $vf7x\n"
                 "vmaddz.xz $vf7, $vf6, $vf7z\n"
                 "vmulax.xz $ACC, $vf5, $vf8x\n"
                 "vmaddz.xz $vf8, $vf6, $vf8z\n"
                 "vmulax.xz $ACC, $vf5, $vf9x\n"
                 "vmaddz.xz $vf9, $vf6, $vf9z\n"
                 "vmulax.xz $ACC, $vf5, $vf10x\n"
                 "vmaddz.xz $vf10, $vf6, $vf10z\n"
                 "sqc2 $vf7, 0x0($4)\n"
                 "sqc2 $vf8, 0x10($4)\n"
                 "sqc2 $vf9, 0x20($4)\n"
                 "sqc2 $vf10, 0x30($4)\n"
                 :
                 :
                 : "$6", "$7", "$8", "$f0");
}
extern "C" void sceVu0RotMatrix(float (*m0)[4], float (*m1)[4], float* rot) {
    sceVu0RotMatrixZ(m0, m1, rot[2]);
    sceVu0RotMatrixY(m0, m0, rot[1]);
    sceVu0RotMatrixX(m0, m0, rot[0]);
}
extern "C" void sceVu0ClampVector(float *v0, float *v1, float min, float max) {
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
extern "C" void sceVu0CameraMatrix(float (*m)[4], float* p, float* zd, float* yd) {
    float m0[4][4] __attribute__((aligned(16)));
    float xd[4] __attribute__((aligned(16)));

    sceVu0UnitMatrix(m0);
    sceVu0OuterProduct(xd, yd, zd);
    sceVu0Normalize(m0[0], xd);
    sceVu0Normalize(m0[2], zd);
    sceVu0OuterProduct(m0[1], m0[2], m0[0]);
    sceVu0TransMatrix(m0, m0, p);
    sceVu0InversMatrix(m, m0);
}
extern "C" void sceVu0NormalLightMatrix(float (*m)[4], float* l0, float* l1, float* l2) {
    float mone = -1.0f;
    float t[4] __attribute__((aligned(16)));

    sceVu0ScaleVector(t, l0, mone);
    sceVu0Normalize(m[0], t);
    sceVu0ScaleVector(t, l1, mone);
    sceVu0Normalize(m[1], t);
    sceVu0ScaleVector(t, l2, mone);
    sceVu0Normalize(m[2], t);
    m[3][0] = m[3][1] = m[3][2] = 0.0f;
    m[3][3] = 1.0f;
    sceVu0TransposeMatrix(m, m);
}
extern "C" void sceVu0LightColorMatrix(float (*m)[4], float* c0, float* c1, float* c2, float* a) {
    sceVu0CopyVector(m[0], c0);
    sceVu0CopyVector(m[1], c1);
    sceVu0CopyVector(m[2], c2);
    sceVu0CopyVector(m[3], a);
}
extern "C" void sceVu0ViewScreenMatrix(float (*m)[4], float scrz, float ax, float ay, float cx, float cy, float zmin,
                                       float zmax, float nearz, float farz) {
    float az, cz;
    float mt[4][4] __attribute__((aligned(16)));

    cz = (-zmax * nearz + zmin * farz) / (-nearz + farz);
    az = farz * nearz * (-zmin + zmax) / (-nearz + farz);

    sceVu0UnitMatrix(m);
    m[0][0] = scrz;
    m[1][1] = scrz;
    m[2][2] = 0;
    m[3][3] = 0;
    m[3][2] = 1;
    m[2][3] = 1;

    sceVu0UnitMatrix(mt);
    mt[0][0] = ax;
    mt[1][1] = ay;
    mt[2][2] = az;
    mt[3][0] = cx;
    mt[3][1] = cy;
    mt[3][2] = cz;

    sceVu0MulMatrix(m, mt, m);
}
extern "C" void sceVu0DropShadowMatrix(float (*m)[4], float* lp, float a, float b, float c, int mode) {
    if (mode) {
        float x = lp[0];
        float y = lp[1];
        float z = lp[2];
        float e = (float)1.0 - (a * x + b * y + c * z);

        m[0][0] = a * x + e;
        m[1][0] = b * x;
        m[2][0] = c * x;
        m[3][0] = -x;
        m[0][1] = a * y;
        m[1][1] = b * y + e;
        m[2][1] = c * y;
        m[3][1] = -y;
        m[0][2] = a * z;
        m[1][2] = b * z;
        m[2][2] = c * z + e;
        m[3][2] = -z;
        m[0][3] = a;
        m[1][3] = b;
        m[2][3] = c;
        m[3][3] = e - (float)1.0;
    } else {
        float x = lp[0];
        float y = lp[1];
        float z = lp[2];
        float d = a * x + b * y + c * z;
        float e = (float)-1.0 / d;

        m[0][0] = e * (a * x - d);
        m[1][0] = e * (b * x);
        m[2][0] = e * (c * x);
        m[3][0] = e * -x;
        m[0][1] = e * (a * y);
        m[1][1] = e * (b * y - d);
        m[2][1] = e * (c * y);
        m[3][1] = e * -y;
        m[0][2] = e * (a * z);
        m[1][2] = e * (b * z);
        m[2][2] = e * (c * z - d);
        m[3][2] = e * -z;
        m[0][3] = 0;
        m[1][3] = 0;
        m[2][3] = 0;
        m[3][3] = e * -d;
    }
}
extern "C" void sceVu0RotTransPersN(int (*v0)[4], float (*m0)[4], float (*v1)[4], int n, int mode) {
    asm volatile(
                 "lqc2 $vf4, 0x0($5)\n"
                 "lqc2 $vf5, 0x10($5)\n"
                 "lqc2 $vf6, 0x20($5)\n"
                 "lqc2 $vf7, 0x30($5)\n"
                 "0:\n"
                 "lqc2 $vf8, 0x0($6)\n"
                 "vmulax.xyzw $ACC, $vf4, $vf8x\n"
                 "vmadday.xyzw $ACC, $vf5, $vf8y\n"
                 "vmaddaz.xyzw $ACC, $vf6, $vf8z\n"
                 "vmaddw.xyzw $vf9, $vf7, $vf8w\n"
                 "vdiv $Q, $vf0w, $vf9w\n"
                 "vwaitq\n"
                 "vmulq.xyz $vf9, $vf9, $Q\n"
                 "vftoi4.xyzw $vf10, $vf9\n"
                 "beqz $8, 1f\n"
                 "vftoi0.zw $vf10, $vf9\n"
                 "1:\n"
                 "sqc2 $vf10, 0x0($4)\n"
                 "addi $7, $7, -0x1\n"
                 "addi $6, $6, 0x10\n"
                 "addi $4, $4, 0x10\n"
                 "bne $0, $7, 0b\n"
);
}
extern "C" void sceVu0RotTransPers(int* v0, float (*m0)[4], float* v1, int mode) {
    asm volatile("lqc2 $vf4, 0x0($5)\n"
                 "lqc2 $vf5, 0x10($5)\n"
                 "lqc2 $vf6, 0x20($5)\n"
                 "lqc2 $vf7, 0x30($5)\n"
                 "lqc2 $vf8, 0x0($6)\n"
                 "vmulax.xyzw $ACC, $vf4, $vf8x\n"
                 "vmadday.xyzw $ACC, $vf5, $vf8y\n"
                 "vmaddaz.xyzw $ACC, $vf6, $vf8z\n"
                 "vmaddw.xyzw $vf9, $vf7, $vf8w\n"
                 "vdiv $Q, $vf0w, $vf9w\n"
                 "vwaitq\n"
                 "vmulq.xyz $vf9, $vf9, $Q\n"
                 "vftoi4.xyzw $vf10, $vf9\n"
                 "beqz $7, 0f\n"
                 "vftoi0.zw $vf10, $vf9\n"
                 "0:\n"
                 "sqc2 $vf10, 0x0($4)\n");
}
extern "C" void sceVu0CopyVectorXYZ(float* v0, float* v1) {
    v0[0] = v1[0];
    v0[1] = v1[1];
    v0[2] = v1[2];
}
extern "C" void sceVu0InterVectorXYZ(float *v0, float *v1, float *v2, float r) {
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
extern "C" void sceVu0ScaleVectorXYZ(float *v0, float *v1, float s) {
    asm volatile(
                 "lqc2 $vf4, 0x0($5)\n"
                 "mfc1 $8, $f12\n"
                 "qmtc2.ni $8, $vf5\n"
                 "vmulx.xyz $vf4, $vf4, $vf5x\n"
                 "sqc2 $vf4, 0x0($4)\n");
}
extern "C" int sceVu0ClipScreen(float *v0) {
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
extern "C" int sceVu0ClipScreen3(float* v0, float* v1, float* v2) {
    asm volatile(
                 "vsub.xyzw $vf4, $vf0, $vf0\n"
                 "ori $2, $zero, 0x8B00\n"
                 "dsll32 $2, $2, 0\n"
                 "ori $2, $2, 0x8B00\n"
                 "dsll $2, $2, 15\n"
                 "lqc2 $vf6, 0x0($4)\n"
                 "lqc2 $vf8, 0x0($5)\n"
                 "lqc2 $vf9, 0x0($6)\n"
                 "qmtc2.ni $2, $vf7\n"
                 "ctc2.ni $zero, $vi16\n"
                 "vsub.xyzw $vf5, $vf6, $vf4\n"
                 "vsub.xy $vf5, $vf7, $vf6\n"
                 "vsub.xyzw $vf5, $vf8, $vf4\n"
                 "vsub.xy $vf5, $vf7, $vf8\n"
                 "vsub.xyzw $vf5, $vf9, $vf4\n"
                 "vsub.xy $vf5, $vf7, $vf9\n"
                 "vnop\n"
                 "vnop\n"
                 "vnop\n"
                 "vnop\n"
                 "vnop\n"
                 "cfc2.ni $2, $vi16\n"
                 "andi $2, $2, 0xC0\n"
                 :
                 :
                 : "$2");
}
extern "C" int sceVu0ClipAll(float* minv, float* maxv, float (*ms)[4], float (*vm)[4], int n) {
    asm volatile(
                 "lqc2 $vf8, 0x0($7)\n"
                 "lqc2 $vf4, 0x0($6)\n"
                 "lqc2 $vf5, 0x10($6)\n"
                 "lqc2 $vf6, 0x20($6)\n"
                 "lqc2 $vf7, 0x30($6)\n"
                 "lqc2 $vf9, 0x0($4)\n"
                 "lqc2 $vf10, 0x0($5)\n"
                 "lqc2 $vf11, 0x0($4)\n"
                 "lqc2 $vf12, 0x0($5)\n"
                 "0:\n"
                 "vmulax.xyzw $ACC, $vf4, $vf8x\n"
                 "vmadday.xyzw $ACC, $vf5, $vf8y\n"
                 "vmaddaz.xyzw $ACC, $vf6, $vf8z\n"
                 "vmaddw.xyzw $vf8, $vf7, $vf8w\n"
                 "vmulw.xyz $vf11, $vf9, $vf8w\n"
                 "vmulw.xyz $vf12, $vf10, $vf8w\n"
                 "vnop\n"
                 "vnop\n"
                 "ctc2.ni $0, $vi16\n"
                 "vsub.xyw $vf11, $vf8, $vf11\n"
                 "vsub.xyw $vf12, $vf12, $vf8\n"
                 "vmove.w $vf11, $vf9\n"
                 "vmove.w $vf12, $vf10\n"
                 "vnop\n"
                 "addi $7, $7, 0x10\n"
                 "lqc2 $vf8, 0x0($7)\n"
                 "addi $8, $8, -0x1\n"
                 "cfc2.ni $2, $vi16\n"
                 "andi $2, $2, 0xC0\n"
                 "beqz $2, 1f\n"
                 "bne $0, $8, 0b\n"
                 "addi $2, $0, 0x1\n"
                 "1:\n"
                 :
                 :
                 : "$2");
}
// SOURCE DATA: .data .rodata
