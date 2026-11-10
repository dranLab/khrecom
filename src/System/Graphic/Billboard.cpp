#include "Billboard.h"
#include "System/HeapMan.h"


static const float s_matEyeFix[4][4] = {
    {-1.f, 0.f, 0.f, 0.f},
    {0.f, -1.f, 0.f, 0.f},
    {0.f, 0.f, 1.f, 0.f},
    {0.f, 0.f, 0.f, 1.f},
};

static const FVECTOR s_vYd = {{0.f, 1.f, 0.f, 0.f}};
static const FVECTOR s_vXd = {{1.f, 0.f, 0.f, 0.f}};

// Normalizes the xyz of vf<v> (w = 0), using vf<t0>, vf<t1> as temporaries.
#define VU0_NORMALIZE(v, t0, t1) \
    "vaddy.x $vf" #t0 ", $vf0, $vf" #v "y\n" \
    "vaddz.x $vf" #t1 ", $vf0, $vf" #v "z\n" \
    "vmulax.x $ACC, $vf" #v ", $vf" #v "x\n" \
    "vmadday.x $ACC, $vf" #t0 ", $vf" #v "y\n" \
    "vmaddz.x $vf" #t0 ", $vf" #t1 ", $vf" #v "z\n" \
    "vrsqrt $Q, $vf0w, $vf" #t0 "x\n" \
    "vsub.w $vf" #v ", $vf0, $vf0\n" \
    "vwaitq\n" \
    "vmulq.xyz $vf" #v ", $vf" #v ", $Q\n"

// vf<d> = vf<a> x vf<b>
#define VU0_CROSS(d, a, b) \
    "vopmula.xyz $ACC, $vf" #a ", $vf" #b "\n" \
    "vopmsub.xyz $vf" #d ", $vf" #b ", $vf" #a "\n"

CBillboardHelper::CBillboardHelper() {
    m_matEye = *(const FMATRIX*)s_matEyeFix;
    m_vEyePos.set(0.f, 0.f, 0.f, 1.f);
    m_vEyeYd.set(0.f, 1.f, 0.f, 0.f);
    m_vEyeZd.set(0.f, 0.f, -1.f, 0.f);
}

CBillboardHelper::~CBillboardHelper() {}

void CBillboardHelper::setEyeMatrixImpl(FMATRIX& inMat, const FVECTOR& vInRot) {
    UnitMatrix(inMat);
    RotMatrixX(inMat, inMat, vInRot.x);
    RotMatrixY(inMat, inMat, vInRot.y);
    RotMatrixZ(inMat, inMat, vInRot.z);
}

void CBillboardHelper::calcBillboardMatrix(FMATRIX& inMat, const FVECTOR& vInPos, const FVECTOR& vInEyePos,
                                           const FVECTOR& vInEyeZd, INT nInType, const FVECTOR* pvInEyeRot) {
    FMATRIX matEyeTmp;
    const FMATRIX* pmatEye = (const FMATRIX*)s_matEyeFix;
    if (pvInEyeRot) {
        setEyeMatrixImpl(matEyeTmp, *pvInEyeRot);
        pmatEye = &matEyeTmp;
    }
    FVECTOR vEyePosTmp;
    asm volatile(
        "lqc2 $vf8, 0x0(%1)\n"
        "lqc2 $vf4, 0x0(%2)\n"
        "lqc2 $vf5, 0x10(%2)\n"
        "lqc2 $vf6, 0x20(%2)\n"
        "lqc2 $vf7, 0x30(%2)\n"
        "vmulax.xyzw $ACC, $vf4, $vf8x\n"
        "vmadday.xyzw $ACC, $vf5, $vf8y\n"
        "vmaddaz.xyzw $ACC, $vf6, $vf8z\n"
        "vmaddw.xyzw $vf8, $vf7, $vf8w\n"
        "sqc2 $vf8, 0x0(%0)\n"
        :
        : "r"(&vEyePosTmp), "r"(&vInEyePos), "r"(pmatEye)
        : "memory");
    calcBillboardMatrixImpl(inMat, vInPos, vEyePosTmp, s_vYd, vInEyeZd, nInType);
}

void CBillboardHelper::calcBillboardMatrixImpl(FMATRIX& inMat, const FVECTOR& vInPos, const FVECTOR& vInEyePos,
                                               const FVECTOR& vInEyeYd, const FVECTOR& vInEyeZd, INT nInType) {
    vu0CalcBillboardMatrix_vf02_05(vInPos, vInEyePos, vInEyeYd, vInEyeZd, nInType);
    asm volatile(
        "sqc2 $vf2, 0x0(%0)\n"
        "sqc2 $vf3, 0x10(%0)\n"
        "sqc2 $vf4, 0x20(%0)\n"
        "sqc2 $vf5, 0x30(%0)\n"
        :
        : "r"(&inMat)
        : "memory");
}

void CBillboardHelper::vu0CalcBillboardMatrix_vf02_05(const FVECTOR& vInPos, const FVECTOR& vInEyePos,
                                                      const FVECTOR& vInEyeYd, const FVECTOR& vInEyeZd,
                                                      INT nInType) {
    FVECTOR vRight;
    FVECTOR vUp;
    if (nInType & 1) {
        asm volatile("lqc2 $vf4, 0x0(%0)\n" : : "r"(&vInEyeZd));
    } else {
        asm volatile(
            "lqc2 $vf4, 0x0(%0)\n"
            "lqc2 $vf5, 0x0(%1)\n"
            "vsub.xyz $vf4, $vf4, $vf5\n"
            :
            : "r"(&vInEyePos), "r"(&vInPos));
    }
    asm volatile(VU0_NORMALIZE(4, 5, 3));
    if (nInType & 2) {
        asm volatile("lqc2 $vf2, 0x0(%0)\n" : : "r"(&s_vXd));
    } else {
        asm volatile(
            "lqc2 $vf2, 0x0(%0)\n"
            VU0_CROSS(2, 2, 4)
            VU0_NORMALIZE(2, 5, 3)
            :
            : "r"(&vInEyeYd));
    }
    if (nInType & 4) {
        asm volatile(
            "lqc2 $vf3, 0x0(%0)\n"
            VU0_CROSS(4, 2, 3)
            :
            : "r"(&s_vYd));
    } else {
        asm volatile(
            VU0_CROSS(3, 4, 2)
            "vsub.w $vf3, $vf0, $vf0\n");
    }
    if (nInType & 2) {
        asm volatile(
            VU0_NORMALIZE(3, 5, 4)
            VU0_CROSS(4, 2, 3));
    }
    asm volatile(
        "lqc2 $vf5, 0x0(%0)\n"
        "vmove.w $vf5, $vf0\n"
        :
        : "r"(&vInPos));
}

// SOURCE DATA: .rodata
