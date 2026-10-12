#ifndef SYSTEM_GRAPHIC_BILLBOARD_H
#define SYSTEM_GRAPHIC_BILLBOARD_H

#include "khtypes.h"
#include "System/Vector.h"

void ApplyMatrix(FVECTOR& v0, const FMATRIX& m0, const FVECTOR& v1);

/* Builds billboard matrices (facing the eye) for a given eye rotation/position. */
class CBillboardHelper {
private:
    FMATRIX m_matEye;
    FVECTOR m_vEyePos;
    FVECTOR m_vEyeYd;
    FVECTOR m_vEyeZd;
    FMATRIX m_matDefault;

public:
    CBillboardHelper();
    virtual ~CBillboardHelper();

    void setEyeRot(const FVECTOR& vInRot) { setEyeMatrix(vInRot); }
    void setEyePos(const FVECTOR& vInPos, const FVECTOR* pvInZd) {
        ApplyMatrix(m_vEyePos, m_matEye, vInPos);
        if (pvInZd) {
            ApplyMatrix(m_vEyeZd, m_matEye, *pvInZd);
            m_vEyeZd.set(-m_vEyeZd.x, -m_vEyeZd.y, -m_vEyeZd.z);
        }
        calcDefaultMatrix();
    }
    const FVECTOR& getEyePos() const { return m_vEyePos; }
    void updateEye(const FVECTOR& vInRot, const FVECTOR& vInPos, const FVECTOR* pvInZd) {
        setEyeMatrix(vInRot);
        setEyePos(vInPos, pvInZd);
    }
    void getBillboardMatrix(FMATRIX& inMat, const FVECTOR& vInPos, INT nInType) {
        if (nInType == 1) {
            asm volatile(
                "lqc2 $vf5, 0x0(%1)\n"
                "vmove.w $vf5, $vf0\n"
                "lqc2 $vf2, 0x0(%2)\n"
                "lqc2 $vf3, 0x10(%2)\n"
                "lqc2 $vf4, 0x20(%2)\n"
                "sqc2 $vf5, 0x30(%0)\n"
                "sqc2 $vf2, 0x0(%0)\n"
                "sqc2 $vf3, 0x10(%0)\n"
                "sqc2 $vf4, 0x20(%0)\n"
                :
                : "r"(&inMat), "r"(&vInPos), "r"(&m_matDefault)
                : "memory");
        } else {
            calcBillboardMatrixImpl(inMat, vInPos, m_vEyePos, m_vEyeYd, m_vEyeZd, nInType);
        }
    }
    void getBillboardMatrix_vf02_05(const FVECTOR& vInPos, INT nInType) {
        if (nInType == 1) {
            asm volatile(
                "lqc2 $vf2, 0x0(%1)\n"
                "lqc2 $vf3, 0x10(%1)\n"
                "lqc2 $vf4, 0x20(%1)\n"
                "lqc2 $vf5, 0x0(%0)\n"
                "vmove.w $vf5, $vf0\n"
                :
                : "r"(&vInPos), "r"(&m_matDefault));
        } else {
            vu0CalcBillboardMatrix_vf02_05(vInPos, m_vEyePos, m_vEyeYd, m_vEyeZd, nInType);
        }
    }
    static void calcBillboardMatrix(FMATRIX& inMat, const FVECTOR& vInPos, const FVECTOR& vInEyePos,
                                    const FVECTOR& vInEyeZd, INT nInType, const FVECTOR* pvInEyeRot);

protected:
    void setEyeMatrix(const FVECTOR& vInRot) { setEyeMatrixImpl(m_matEye, vInRot); }
    static void setEyeMatrixImpl(FMATRIX& inMat, const FVECTOR& vInRot);
    static void calcBillboardMatrixImpl(FMATRIX& inMat, const FVECTOR& vInPos, const FVECTOR& vInEyePos,
                                        const FVECTOR& vInEyeYd, const FVECTOR& vInEyeZd, INT nInType);
    static void vu0CalcBillboardMatrix_vf02_05(const FVECTOR& vInPos, const FVECTOR& vInEyePos,
                                               const FVECTOR& vInEyeYd, const FVECTOR& vInEyeZd, INT nInType);
    void calcDefaultMatrix() {
        FVECTOR vTmp;
        calcBillboardMatrixImpl(m_matDefault, vTmp, m_vEyePos, m_vEyeYd, m_vEyeZd, 1);
    }
};

#endif /* SYSTEM_GRAPHIC_BILLBOARD_H */
