#include "System/Math3D.h"
#include "include_asm.h"

extern "C" float acosf(float x);
extern "C" float tanf(float x);
extern "C" float asinf(float x);
extern "C" float cosf(float x);
extern "C" float atan2f(float y, float x);

void ApplyMatrix(FVECTOR& v0, const FMATRIX& m0, const FVECTOR& v1);
void OuterProduct(FVECTOR& v0, const FVECTOR& v1, const FVECTOR& v2);
FLOAT InnerProduct(const FVECTOR& v0, const FVECTOR& v1);
FLOAT InnerProduct(const FVECTOR& v0);
FLOAT InnerProduct4(const FVECTOR& vecInA, const FVECTOR& vecInB);
void Normalize(FVECTOR& v0, const FVECTOR& v1);
void AddVector(FVECTOR& v0, const FVECTOR& v1, const FVECTOR& v2);
void SubVector(FVECTOR& v0, const FVECTOR& v1, const FVECTOR& v2);
void MulVector(FVECTOR& v0, const FVECTOR& v1, const FVECTOR& v2);
void ScaleVector(FVECTOR& v0, const FVECTOR& v1, FLOAT t);
void ScaleVectorXYZ(FVECTOR& v0, const FVECTOR& v1, FLOAT t);
void SubVectorXYZ(FVECTOR& v0, const FVECTOR& v1, const FVECTOR& v2);
float vu0Cosf(float x);
float vu0Sinf(float x);
extern "C" void sceVu0InterVector(float* v0, float* v1, float* v2, float t);
extern "C" void TRACE(char* fmt, ...);
extern "C" void AssertApp(char* pszFile, int nLine, char* pszMsg, char* pszExp);

#define ASSERT(e) if (!(e)) AssertApp(__FILE__, __LINE__, #e, 0)

class CWorldEnv {
public:
    static FLOAT getDispFar();
    static FMATRIX& getWorldViewMatrix();
};

static inline FLOAT _fabsf(FLOAT inValue) {
    return __builtin_fabsf(inValue);
}

static inline void operator-=(FVECTOR& v, const FVECTOR& inV) {
    v.x -= inV.x;
    v.y -= inV.y;
    v.z -= inV.z;
    v.w -= inV.w;
}

static inline FVECTOR operator+(const FVECTOR& v, const FVECTOR& inV) {
    FVECTOR vec = v;
    vec.x += inV.x;
    vec.y += inV.y;
    vec.z += inV.z;
    vec.w += inV.w;
    return vec;
}

static inline FVECTOR operator-(const FVECTOR& v, const FVECTOR& inV) {
    FVECTOR vec = v;
    vec.x -= inV.x;
    vec.y -= inV.y;
    vec.z -= inV.z;
    vec.w -= inV.w;
    return vec;
}

static inline FVECTOR operator-(const FVECTOR& inV) {
    FVECTOR vec = inV;
    vec.x = -vec.x;
    vec.y = -vec.y;
    vec.z = -vec.z;
    vec.w = -vec.w;
    return vec;
}

static inline FVECTOR operator*(const FVECTOR& v, FLOAT inF) {
    FVECTOR vec = v;
    vec.x *= inF;
    vec.y *= inF;
    vec.z *= inF;
    vec.w *= inF;
    return vec;
}

static inline void operator*=(FVECTOR& v, FLOAT inF) {
    v.x *= inF;
    v.y *= inF;
    v.z *= inF;
    v.w *= inF;
}

static inline void _init0(FVECTOR& v) {
    asm volatile("sqc2 $vf0, 0x0(%0)" : : "r"(&v) : "memory");
}

static inline void _initTrans(FMATRIX& m) {
    _init0(m.v[3]);
}

static inline void _unitMatrix(FMATRIX& m) {
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

static inline FLOAT _sqrtf(FLOAT x) {
    FLOAT ret;
    asm volatile("sqrt.s %0, %1" : "=f"(ret) : "f"(x));
    return ret;
}

void CFrustum::initialize(FLOAT inE, FLOAT inA, FLOAT inN, FLOAT inF, BOOL inInit) {
    FLOAT d;
    FLOAT leftRightX;
    FLOAT leftRightZ;
    FLOAT topBottomY;
    FLOAT topBottomZ;

    d = 1.0f / _sqrtf(inE * inE + 1.0f);
    leftRightX = inE * d;
    leftRightZ = d;
    d = 1.0f / _sqrtf(inE * inE + inA * inA);
    topBottomY = inE * d;
    topBottomZ = inA * d;
    m_vFrustum[0].set(0.0f, 0.0f, 1.0f, -inN);
    m_vFrustum[1].set(0.0f, 0.0f, -1.0f, inF);
    m_vFrustum[2].set(leftRightX, 0.0f, leftRightZ, 0.0f);
    m_vFrustum[3].set(-leftRightX, 0.0f, leftRightZ, 0.0f);
    m_vFrustum[4].set(0.0f, topBottomY, topBottomZ, 0.0f);
    m_vFrustum[5].set(0.0f, -topBottomY, topBottomZ, 0.0f);
    m_bInit = inInit;
}

BOOL CFrustum::sphereVisible(const FVECTOR& inCenter, FLOAT inRadius) {
    FVECTOR v;
    if (!isInit()) {
        return FALSE;
    }
    FVECTOR* frustum = m_vFrustum;
    for (INT i = 0; i < 6; i++, frustum++) {
        if (InnerProduct4(inCenter, *frustum) <= -inRadius) {
            return FALSE;
        }
    }
    return TRUE;
}

BOOL CFrustum::ellipsoidVisible(const FVECTOR& inCenter, const FVECTOR& inR, const FVECTOR& inS, const FVECTOR& inT) {
    if (!isInit()) {
        return FALSE;
    }
    for (INT i = 0; i < 6; i++) {
        FLOAT RN = InnerProduct(inR, m_vFrustum[i]);
        FLOAT SN = InnerProduct(inS, m_vFrustum[i]);
        FLOAT TN = InnerProduct(inT, m_vFrustum[i]);
        FVECTOR v;
        RN = _sqrtf(RN * RN + SN * SN + TN * TN);
        MulVector(v, inCenter, m_vFrustum[i]);
        if (v.x + v.y + v.z + v.w <= -RN) {
            return FALSE;
        }
    }
    return TRUE;
}

BOOL CFrustum::cylinderVisible(const FVECTOR& inP1, const FVECTOR& inP2, FLOAT inRadius) {
    if (!isInit()) {
        return FALSE;
    }
    FVECTOR P2P1 = inP2 - inP1;
    FVECTOR A;
    Normalize(A, P2P1);
    for (INT i = 0; i < 6; i++) {
        FLOAT Reff = InnerProduct(A, m_vFrustum[i]);
        Reff = inRadius * _sqrtf(1.0f - Reff * Reff);
        FLOAT LP1 = InnerProduct4(inP1, m_vFrustum[i]);
        if (LP1 <= -Reff) {
            return FALSE;
        }
        if (InnerProduct4(inP2, m_vFrustum[i]) <= -Reff) {
            return FALSE;
        }
        FVECTOR P1P2 = inP1 - inP2;
        FLOAT t = (Reff + LP1) / InnerProduct4(m_vFrustum[i], P1P2);
        P2P1 *= t;
        FVECTOR P3 = inP1 + P2P1;
        P3.w = 1.0f;
        if (InnerProduct4(P3, m_vFrustum[i]) <= -Reff) {
            return FALSE;
        }
    }
    return TRUE;
}

void Quaternion::QuaternionNormalize(QUATERNION& outQ, const QUATERNION& inQ) {
    asm volatile("lqc2 $vf4, 0x0(%1)\n"
                 "vmul.xyzw $vf5, $vf4, $vf4\n"
                 "vaddy.x $vf5, $vf5, $vf5y\n"
                 "vaddz.x $vf5, $vf5, $vf5z\n"
                 "vaddw.x $vf5, $vf5, $vf5w\n"
                 "vrsqrt $Q, $vf0w, $vf5x\n"
                 "vwaitq\n"
                 "vmulq.xyzw $vf6, $vf4, $Q\n"
                 "sqc2 $vf6, 0x0(%0)\n"
                 :
                 : "r"(&outQ), "r"(&inQ)
                 : "memory");
}

void Quaternion::QuaternionMultiply(QUATERNION& outQ, const QUATERNION& inQ0, const QUATERNION& inQ1) {
    asm volatile("lqc2 $vf4, 0x0(%1)\n"
                 "lqc2 $vf5, 0x0(%2)\n"
                 "vmul.xyzw $vf6, $vf4, $vf5\n"
                 "vaddy.x $vf6, $vf6, $vf6y\n"
                 "vaddz.x $vf6, $vf6, $vf6z\n"
                 "vsubx.w $vf7, $vf6, $vf6x\n"
                 "vopmula.xyz $ACC, $vf4, $vf5\n"
                 "vopmsub.xyz $vf7, $vf5, $vf4\n"
                 "vmulax.w $ACC, $vf0, $vf0x\n"
                 "vmulaw.xyz $ACC, $vf4, $vf5w\n"
                 "vmaddaw.xyz $ACC, $vf5, $vf4w\n"
                 "vmaddw.xyzw $vf7, $vf7, $vf0w\n"
                 "sqc2 $vf7, 0x0(%0)\n"
                 :
                 : "r"(&outQ), "r"(&inQ0), "r"(&inQ1)
                 : "memory");
}

void Quaternion::QuaternionRotationAxis(QUATERNION& outQuat, const FVECTOR& inAxis, FLOAT inRad) {
    FLOAT theta = inRad * 0.5f;
    asm volatile("lqc2 $vf4, 0x0(%1)\n"
                 "mfc1 $v1, %2\n"
                 "qmtc2 $v1, $vf5\n"
                 "vmulx.xyzw $vf4, $vf4, $vf5x\n"
                 "sqc2 $vf4, 0x0(%0)\n"
                 :
                 : "r"(&outQuat), "r"(&inAxis), "f"(vu0Sinf(theta))
                 : "memory", "$3");
    outQuat.w = vu0Cosf(theta);
}

void Quaternion::QuaternionToMatrix(FMATRIX& outMatrix, const QUATERNION& inQuat) {
    FVECTOR v0;
    FVECTOR v1;
    FVECTOR v2;
    asm volatile("lqc2 $vf4, 0x0(%3)\n"
                 "vmaxw.xyzw $vf12, $vf0, $vf0w\n"
                 "vmul.xyzw $vf5, $vf4, $vf4\n"
                 "vaddy.x $vf6, $vf5, $vf5y\n"
                 "vaddz.x $vf6, $vf6, $vf5z\n"
                 "vaddw.x $vf6, $vf6, $vf5w\n"
                 "vaddw.w $vf7, $vf0, $vf0w\n"
                 "vdiv $Q, $vf7w, $vf6x\n"
                 "vwaitq\n"
                 "vaddq.x $vf6, $vf0, $Q\n"
                 "vopmula.xyz $ACC, $vf4, $vf4\n"
                 "vmadd.xyz $vf8, $vf0, $vf0\n"
                 "vmulw.xyz $vf11, $vf4, $vf4w\n"
                 "vmulx.xyz $vf9, $vf5, $vf6x\n"
                 "vmulx.xyz $vf10, $vf8, $vf6x\n"
                 "vmulx.xyz $vf11, $vf11, $vf6x\n"
                 "vadd.xyz $vf4, $vf10, $vf11\n"
                 "vsub.xyz $vf5, $vf10, $vf11\n"
                 "vaddy.x $vf8, $vf9, $vf9y\n"
                 "vaddz.y $vf8, $vf9, $vf9z\n"
                 "vaddx.z $vf8, $vf9, $vf9x\n"
                 "vsub.xyz $vf6, $vf12, $vf8\n"
                 "sqc2 $vf6, 0x0(%0)\n"
                 "sqc2 $vf4, 0x0(%1)\n"
                 "sqc2 $vf5, 0x0(%2)\n"
                 :
                 : "r"(&v0), "r"(&v1), "r"(&v2), "r"(&inQuat)
                 : "memory");
    outMatrix.m[0][0] = v0.y;
    outMatrix.m[0][1] = v1.z;
    outMatrix.m[0][2] = v2.y;
    outMatrix.m[1][0] = v2.z;
    outMatrix.m[1][1] = v0.z;
    outMatrix.m[1][2] = v1.x;
    outMatrix.m[2][0] = v1.y;
    outMatrix.m[2][1] = v2.x;
    outMatrix.m[2][2] = v0.x;
    outMatrix.m[0][3] = 0.0f;
    outMatrix.m[1][3] = 0.0f;
    outMatrix.m[2][3] = 0.0f;
    _initTrans(outMatrix);
}

void Quaternion::QuaternionToMatrix(FMATRIX& outMatrix, const QUATERNION& inQuat, const FVECTOR& inTrs) {
    FVECTOR v0;
    FVECTOR v1;
    FVECTOR v2;
    asm volatile("lqc2 $vf4, 0x0(%3)\n"
                 "vmaxw.xyzw $vf12, $vf0, $vf0w\n"
                 "vmul.xyzw $vf5, $vf4, $vf4\n"
                 "vaddy.x $vf6, $vf5, $vf5y\n"
                 "vaddz.x $vf6, $vf6, $vf5z\n"
                 "vaddw.x $vf6, $vf6, $vf5w\n"
                 "vaddw.w $vf7, $vf0, $vf0w\n"
                 "vdiv $Q, $vf7w, $vf6x\n"
                 "vwaitq\n"
                 "vaddq.x $vf6, $vf0, $Q\n"
                 "vopmula.xyz $ACC, $vf4, $vf4\n"
                 "vmadd.xyz $vf8, $vf0, $vf0\n"
                 "vmulw.xyz $vf11, $vf4, $vf4w\n"
                 "vmulx.xyz $vf9, $vf5, $vf6x\n"
                 "vmulx.xyz $vf10, $vf8, $vf6x\n"
                 "vmulx.xyz $vf11, $vf11, $vf6x\n"
                 "vadd.xyz $vf4, $vf10, $vf11\n"
                 "vsub.xyz $vf5, $vf10, $vf11\n"
                 "vaddy.x $vf8, $vf9, $vf9y\n"
                 "vaddz.y $vf8, $vf9, $vf9z\n"
                 "vaddx.z $vf8, $vf9, $vf9x\n"
                 "vsub.xyz $vf6, $vf12, $vf8\n"
                 "sqc2 $vf6, 0x0(%0)\n"
                 "sqc2 $vf4, 0x0(%1)\n"
                 "sqc2 $vf5, 0x0(%2)\n"
                 :
                 : "r"(&v0), "r"(&v1), "r"(&v2), "r"(&inQuat)
                 : "memory");
    outMatrix.m[0][0] = v0.y;
    outMatrix.m[0][1] = v1.z;
    outMatrix.m[0][2] = v2.y;
    outMatrix.m[1][0] = v2.z;
    outMatrix.m[1][1] = v0.z;
    outMatrix.m[1][2] = v1.x;
    outMatrix.m[2][0] = v1.y;
    outMatrix.m[2][1] = v2.x;
    outMatrix.m[2][2] = v0.x;
    outMatrix.m[0][3] = 0.0f;
    outMatrix.m[1][3] = 0.0f;
    outMatrix.m[2][3] = 0.0f;
    outMatrix.setTrans(inTrs);
}

void Quaternion::MatrixToQuaternion(QUATERNION& outQuat, const FMATRIX& inMatrix) {
    FLOAT Tr;
    FLOAT fourD;
    INT i;

    Tr = inMatrix.m[0][0] + inMatrix.m[1][1] + inMatrix.m[2][2] + inMatrix.m[3][3];
    if (Tr >= 1.0f) {
        fourD = 2.0f * _sqrtf(Tr);
#line 811
        ASSERT(fourD != 0.0f);
        outQuat.w = fourD * 0.25f;
        outQuat.x = (inMatrix.m[1][2] - inMatrix.m[2][1]) / fourD;
        outQuat.y = (inMatrix.m[2][0] - inMatrix.m[0][2]) / fourD;
        outQuat.z = (inMatrix.m[0][1] - inMatrix.m[1][0]) / fourD;
    } else {
        float q[4];
        if (inMatrix.m[0][0] > inMatrix.m[1][1]) {
            i = 0;
        } else {
            i = 1;
        }
        if (inMatrix.m[2][2] > inMatrix.m[i][i]) {
            i = 2;
        }
        INT j = (i + 1) % 3;
        INT k = (j + 1) % 3;
        fourD = 2.0f * _sqrtf(inMatrix.m[i][i] - inMatrix.m[j][j] - inMatrix.m[k][k] + 1.0f);
#line 837
        ASSERT(fourD != 0.0f);
        q[i] = fourD * 0.25f;
        q[j] = (inMatrix.m[i][j] + inMatrix.m[j][i]) / fourD;
        q[k] = (inMatrix.m[i][k] + inMatrix.m[k][i]) / fourD;
        q[3] = (inMatrix.m[j][k] - inMatrix.m[k][j]) / fourD;
        outQuat.x = q[0];
        outQuat.y = q[1];
        outQuat.z = q[2];
        outQuat.w = q[3];
    }
}

void Quaternion::QuaternionLerp(QUATERNION& outResult, const QUATERNION& inQuat0, const QUATERNION& inQuat1, FLOAT inT) {
    QUATERNION q;
    sceVu0InterVector(q.v, (float*)inQuat1.v, (float*)inQuat0.v, inT);
    QuaternionNormalize(outResult, q);
}

void Quaternion::QuaternionSlerp(QUATERNION& outResult, const QUATERNION& inQuat0, const QUATERNION& inQuat1, FLOAT inT) {
    FLOAT dot = InnerProduct4(inQuat0, inQuat1);
    QUATERNION q;
    if (dot < 0.0f) {
        dot = -dot;
        q = -inQuat1;
    } else {
        q = inQuat1;
    }
    if (dot < 1.0f) {
        FLOAT angle = acosf(dot);
        FLOAT sina = 1.0f / vu0Sinf(angle);
        FLOAT sinat = vu0Sinf(angle * inT);
        FLOAT sinaomt = vu0Sinf(angle * (1.0f - inT));
        outResult = (inQuat0 * sinaomt + q * sinat) * sina;
    } else {
        QuaternionLerp(outResult, inQuat0, q, inT);
    }
}

void Quaternion::RotationArc(QUATERNION& outQuat, const FVECTOR& inV0, const FVECTOR& inV1) {
    FVECTOR v0;
    FVECTOR v1;
    FVECTOR c;
    Normalize(v0, inV0);
    Normalize(v1, inV1);
    OuterProduct(c, v0, v1);
    FLOAT s = _sqrtf((InnerProduct(v0, v1) + 1.0f) * 2.0f);
    outQuat.x = c.x / s;
    outQuat.y = c.y / s;
    outQuat.z = c.z / s;
    outQuat.w = s * 0.5f;
}

void Quaternion::MakeQuaternion(QUATERNION& outQuat, FLOAT rx, FLOAT ry, FLOAT rz) {
    QUATERNION q0;
    QUATERNION q1;
    QUATERNION q2;
    QUATERNION q3;
    FLOAT theta;

    theta = rz * 0.5f;
    q0.set(0.0f, 0.0f, vu0Sinf(theta), vu0Cosf(theta));
    theta = ry * 0.5f;
    q1.set(0.0f, vu0Sinf(theta), 0.0f, vu0Cosf(theta));
    theta = rx * 0.5f;
    q2.set(vu0Sinf(theta), 0.0f, 0.0f, vu0Cosf(theta));
    QuaternionMultiply(q3, q0, q1);
    QuaternionMultiply(outQuat, q3, q2);
}

void Quaternion::QuaternionConjugate(QUATERNION& outQuat, const QUATERNION& inQuat) {
    asm volatile("lqc2 $vf4, 0x0(%1)\n"
                 "vsubw.x $vf5, $vf0, $vf0w\n"
                 "vmulx.xyz $vf4, $vf4, $vf5x\n"
                 "sqc2 $vf4, 0x0(%0)\n"
                 :
                 : "r"(&outQuat), "r"(&inQuat)
                 : "memory");
}

void Quaternion::QuaternionMulVector(QUATERNION& outQuat, const QUATERNION& inQuat, const FVECTOR& inVect) {
    asm volatile("lqc2 $vf4, 0x0(%1)\n"
                 "lqc2 $vf5, 0x0(%2)\n"
                 "vsub.xyzw $vf6, $vf6, $vf6\n"
                 "vopmula.xyz $ACC, $vf4, $vf5\n"
                 "vmaddaw.xyz $ACC, $vf5, $vf4w\n"
                 "vopmsub.xyz $vf6, $vf5, $vf4\n"
                 "vmul.xyz $vf7, $vf4, $vf5\n"
                 "vaddy.x $vf7, $vf7, $vf7y\n"
                 "vaddz.x $vf7, $vf7, $vf7z\n"
                 "vsubx.x $vf7, $vf0, $vf7x\n"
                 "vaddx.w $vf6, $vf6, $vf7x\n"
                 "sqc2 $vf6, 0x0(%0)\n"
                 :
                 : "r"(&outQuat), "r"(&inQuat), "r"(&inVect)
                 : "memory");
}

FLOAT Math3D::VectorLength(const FVECTOR& inVector) {
    INT iTmp;
    FLOAT ret;
    asm volatile("lqc2 $vf4, 0x0(%1)\n"
                 "vaddy.x $vf6, $vf0, $vf4y\n"
                 "vaddz.x $vf7, $vf0, $vf4z\n"
                 "vmulax.x $ACC, $vf4, $vf4x\n"
                 "vmadday.x $ACC, $vf6, $vf4y\n"
                 "vmaddz.x $vf5, $vf7, $vf4z\n"
                 "qmfc2 $v0, $vf5\n"
                 "mtc1 $v0, %0\n"
                 : "=f"(ret)
                 : "r"(&inVector)
                 : "$2");
    return _sqrtf(ret);
}

FLOAT Math3D::VectorLengthSq(const FVECTOR& inVector) {
    INT iTmp;
    FLOAT ret;
    asm volatile("lqc2 $vf4, 0x0(%1)\n"
                 "vaddy.x $vf6, $vf0, $vf4y\n"
                 "vaddz.x $vf7, $vf0, $vf4z\n"
                 "vmulax.x $ACC, $vf4, $vf4x\n"
                 "vmadday.x $ACC, $vf6, $vf4y\n"
                 "vmaddz.x $vf5, $vf7, $vf4z\n"
                 "qmfc2 $v0, $vf5\n"
                 "mtc1 $v0, %0\n"
                 : "=f"(ret)
                 : "r"(&inVector)
                 : "$2");
    return ret;
}

FLOAT Math3D::VectorLengthXZ(const FVECTOR& inVector) {
    return _sqrtf(VectorLengthXZSq(inVector));
}

FLOAT Math3D::VectorLengthXZSq(const FVECTOR& inVector) {
    INT iTmp;
    FLOAT ret;
    asm volatile("lqc2 $vf4, 0x0(%1)\n"
                 "vmul.xz $vf4, $vf4, $vf4\n"
                 "vaddz.x $vf4, $vf4, $vf4z\n"
                 "qmfc2 $v0, $vf4\n"
                 "mtc1 $v0, %0\n"
                 : "=f"(ret)
                 : "r"(&inVector)
                 : "$2");
    return ret;
}

void Math3D::VectorLerp(FVECTOR& outResult, const FVECTOR& inP1, const FVECTOR& inP2, FLOAT inT) {
    asm volatile("lqc2 $vf4, 0x0(%1)\n"
                 "lqc2 $vf5, 0x0(%2)\n"
                 "mfc1 $v1, %3\n"
                 "qmtc2 $v1, $vf6\n"
                 "vsub.xyzw $vf5, $vf5, $vf4\n"
                 "vadda.xyzw $ACC, $vf4, $vf0\n"
                 "vmaddx.xyz $vf4, $vf5, $vf6x\n"
                 "sqc2 $vf4, 0x0(%0)\n"
                 :
                 : "r"(&outResult), "r"(&inP1), "r"(&inP2), "f"(inT)
                 : "memory", "$3");
}

FLOAT Math3D::PointToPointDistance(const FVECTOR& inP0, const FVECTOR& inP1) {
    return _sqrtf(PointToPointDistanceSq(inP0, inP1));
}

FLOAT Math3D::PointToPointDistanceSq(const FVECTOR& inP0, const FVECTOR& inP1) {
    FVECTOR v;
    INT iTmp;
    FLOAT ret;
    asm volatile("lqc2 $vf4, 0x0(%1)\n"
                 "lqc2 $vf5, 0x0(%2)\n"
                 "vsub.xyzw $vf4, $vf4, $vf5\n"
                 "sqc2 $vf4, 0x0(%0)\n"
                 :
                 : "r"(&v), "r"(&inP1), "r"(&inP0)
                 : "memory");
    asm volatile("lqc2 $vf4, 0x0(%1)\n"
                 "vaddy.x $vf6, $vf0, $vf4y\n"
                 "vaddz.x $vf7, $vf0, $vf4z\n"
                 "vmulax.x $ACC, $vf4, $vf4x\n"
                 "vmadday.x $ACC, $vf6, $vf4y\n"
                 "vmaddz.x $vf5, $vf7, $vf4z\n"
                 "qmfc2 $v0, $vf5\n"
                 "mtc1 $v0, %0\n"
                 : "=f"(ret)
                 : "r"(&v)
                 : "$2");
    return ret;
}

FLOAT Math3D::PointToPointDistanceSqXZ(const FVECTOR& inP0, const FVECTOR& inP1) {
    FVECTOR v;
    SubVectorXYZ(v, inP0, inP1);
    return VectorLengthXZSq(v);
}

void Math3D::VectorMaximize(FVECTOR& outVec, const FVECTOR& inVec0, const FVECTOR& inVec1) {
    asm volatile("lqc2 $vf4, 0x0(%1)\n"
                 "lqc2 $vf5, 0x0(%2)\n"
                 "vmax.xyzw $vf6, $vf4, $vf5\n"
                 "sqc2 $vf6, 0x0(%0)\n"
                 :
                 : "r"(&outVec), "r"(&inVec0), "r"(&inVec1)
                 : "memory");
}

void Math3D::VectorMinimize(FVECTOR& outVec, const FVECTOR& inVec0, const FVECTOR& inVec1) {
    asm volatile("lqc2 $vf4, 0x0(%1)\n"
                 "lqc2 $vf5, 0x0(%2)\n"
                 "vmini.xyzw $vf6, $vf4, $vf5\n"
                 "sqc2 $vf6, 0x0(%0)\n"
                 :
                 : "r"(&outVec), "r"(&inVec0), "r"(&inVec1)
                 : "memory");
}

void Math3D::VectorMaximizeXYZ(FVECTOR& outVec, const FVECTOR& inVec0, const FVECTOR& inVec1) {
    asm volatile("lqc2 $vf4, 0x0(%1)\n"
                 "lqc2 $vf5, 0x0(%2)\n"
                 "vmax.xyz $vf4, $vf4, $vf5\n"
                 "sqc2 $vf4, 0x0(%0)\n"
                 :
                 : "r"(&outVec), "r"(&inVec0), "r"(&inVec1)
                 : "memory");
}

void Math3D::VectorMinimizeXYZ(FVECTOR& outVec, const FVECTOR& inVec0, const FVECTOR& inVec1) {
    asm volatile("lqc2 $vf4, 0x0(%1)\n"
                 "lqc2 $vf5, 0x0(%2)\n"
                 "vmini.xyz $vf4, $vf4, $vf5\n"
                 "sqc2 $vf4, 0x0(%0)\n"
                 :
                 : "r"(&outVec), "r"(&inVec0), "r"(&inVec1)
                 : "memory");
}

void Math3D::VectorMaxMinXYZ(FVECTOR& inoutMax, FVECTOR& inoutMin, const FVECTOR& inVec) {
    asm volatile("lqc2 $vf4, 0x0(%0)\n"
                 "lqc2 $vf5, 0x0(%1)\n"
                 "lqc2 $vf6, 0x0(%2)\n"
                 "vmax.xyz $vf4, $vf4, $vf6\n"
                 "vmini.xyz $vf5, $vf5, $vf6\n"
                 "sqc2 $vf4, 0x0(%0)\n"
                 "sqc2 $vf5, 0x0(%1)\n"
                 :
                 : "r"(&inoutMax), "r"(&inoutMin), "r"(&inVec)
                 : "memory");
}

void Math3D::VectorMaxMinXYZ(FVECTOR& inoutMax, FVECTOR& inoutMin, const FVECTOR& inMax, const FVECTOR& inMin) {
    asm volatile("lqc2 $vf4, 0x0(%0)\n"
                 "lqc2 $vf5, 0x0(%1)\n"
                 "lqc2 $vf6, 0x0(%2)\n"
                 "lqc2 $vf7, 0x0(%3)\n"
                 "vmax.xyz $vf4, $vf4, $vf6\n"
                 "vmini.xyz $vf5, $vf5, $vf7\n"
                 "sqc2 $vf4, 0x0(%0)\n"
                 "sqc2 $vf5, 0x0(%1)\n"
                 :
                 : "r"(&inoutMax), "r"(&inoutMin), "r"(&inMax), "r"(&inMin)
                 : "memory");
}

void Math3D::CalcReflectionVector(FVECTOR& outVec, const FVECTOR& inL, const FVECTOR& inN) {
    FVECTOR invL;
    ScaleVector(invL, inL, -1.0f);
    ScaleVector(outVec, inN, InnerProduct(invL, inN) * 2.0f);
    outVec -= invL;
}

FLOAT Math3D::PointToPlaneDistance(const FVECTOR& inPoint, const FVECTOR& inPlane) {
    FVECTOR v0;
    FVECTOR v1;
    FLOAT a = InnerProduct(inPlane, inPlane);
    if (a == 0.0f) {
        return 999999.0f;
    }
    return _fabsf(InnerProduct(inPoint, inPlane) + inPlane.w) / _sqrtf(a);
}

BOOL Math3D::PlaneIntersectLine(FVECTOR& outIntersect, const FVECTOR& inPlane, const FVECTOR& inV0, const FVECTOR& inV1) {
    FVECTOR v = inV1 - inV0;
    FLOAT a = InnerProduct(inPlane, v) + inPlane.w;
    if (a == 0.0f) {
        return FALSE;
    }
    FLOAT b = InnerProduct(inPlane, inV0) + inPlane.w;
    ScaleVector(v, v, -b / a);
    outIntersect = inV0 + v;
    if (outIntersect.x < inV0.x || outIntersect.x > inV1.x || inV0.y > outIntersect.y || outIntersect.y > inV1.y ||
        inV0.z > outIntersect.z || outIntersect.z > inV1.z) {
        return FALSE;
    }
    return TRUE;
}

INT Math3D::SphereIntersectRay(FVECTOR* outIntersect, const FVECTOR& inSphere, const FVECTOR& inV, const FVECTOR& inQ) {
    FVECTOR Q = inQ - inSphere;
    FLOAT a = InnerProduct(inV, inV);
    FLOAT b = 2.0f * InnerProduct(inV, Q);
    FLOAT c = InnerProduct(Q, Q) - inSphere.w * inSphere.w;
    FLOAT D = b * b - 4.0f * a * c;
    if (D > 0.0f) {
        FLOAT t1 = _sqrtf(D);
        FLOAT t0 = (-b - t1) * 0.5f * a;
        t1 = (-b + t1) * 0.5f * a;
        outIntersect[0] = Q + inV * t0 + inSphere;
        outIntersect[1] = Q + inV * t1 + inSphere;
        return 2;
    }
    if (D == 0.0f) {
        FLOAT t = (-b - _sqrtf(D)) * 0.5f * a;
        outIntersect[0] = Q + inV * t + inSphere;
        return 1;
    }
    return 0;
}

void Math3D::CalcFrustum(FVECTOR* inFrustum) {
    FLOAT e = 1.0f / tanf(0.7853982f);
    FLOAT a = 0.94f;
    FLOAT r1 = _sqrtf(e * e + 1.0f);
    FLOAT r2 = _sqrtf(e * e + a * a);
    inFrustum[0].set(0.0f, 0.0f, 1.0f, 1.0f);
    inFrustum[1].set(0.0f, 0.0f, -1.0f, CWorldEnv::getDispFar());
    inFrustum[2].set(e / r1, 0.0f, 1.0f / r1, 0.0f);
    inFrustum[3].set(-(e / r1), 0.0f, 1.0f / r1, 0.0f);
    inFrustum[4].set(0.0f, e / r2, a / r2, 0.0f);
    inFrustum[5].set(0.0f, -(e / r2), a / r2, 0.0f);
}

BOOL Math3D::CheckVisibleBoundingSphere(BOUNDING_SPHERE& inBSphere, FVECTOR* inFrustum) {
    FVECTOR center;
    FVECTOR d;
    INT i;
    ApplyMatrix(center, CWorldEnv::getWorldViewMatrix(), inBSphere.vCenter);
    for (i = 0; i < 6; i++, inFrustum++) {
        if (InnerProduct4(center, *inFrustum) <= -inBSphere.fR) {
            return FALSE;
        }
    }
    return TRUE;
}

BOOL Math3D::CheckVisibleBoundingCylinder(BOUNDING_CYLINDER& inBCylinder, FVECTOR* inFrustum) {
    FVECTOR Q[2];
    ApplyMatrix(Q[0], CWorldEnv::getWorldViewMatrix(), inBCylinder.vQ2);
    ApplyMatrix(Q[1], CWorldEnv::getWorldViewMatrix(), inBCylinder.vQ2);
    FVECTOR v0 = Q[1] - Q[0];
    FVECTOR a;
    Normalize(a, v0);
    for (INT i = 0; i < 6; i++) {
        FLOAT Reff = InnerProduct(a, inFrustum[i]);
        Reff = inBCylinder.fR * _sqrtf(1.0f - Reff * Reff);
        FVECTOR d1;
        for (INT j = 0; j < 2; j++) {
            MulVector(d1, Q[j], inFrustum[i]);
            if (d1.x + d1.y + d1.z + d1.w <= -Reff) {
                return FALSE;
            }
        }
        FVECTOR d2;
        FVECTOR v2 = Q[0] - Q[1];
        MulVector(d2, inFrustum[i], v2);
        MulVector(d1, Q[0], inFrustum[i]);
        FVECTOR v1;
        FLOAT t = Reff + (d1.x + d1.y + d1.z + d1.w);
        FLOAT s = d2.x + d2.y + d2.z + d2.w;
        ScaleVector(v1, v0, t / s);
        FVECTOR Q3 = Q[0] + v1;
        Q3.w = 1.0f;
        MulVector(d1, Q3, inFrustum[i]);
        if (d1.x + d1.y + d1.z + d1.w <= -Reff) {
            return FALSE;
        }
    }
    return TRUE;
}

void Math3D::HermiteInterpolation(FVECTOR& vecOut, const FMATRIX& matIn, FLOAT fInT) {
    FVECTOR vecB;
    HermiteVector(vecB, fInT);
    ApplyMatrix(vecOut, matIn, vecB);
    vecOut.w = 1.0f;
}

INT Math3D::NearVector(const FVECTOR& v0, const FVECTOR& v1, const FVECTOR& v2) {
    FLOAT dot0;
    FLOAT dot1;
    asm volatile("lqc2 $vf4, 0x0(%2)\n"
                 "lqc2 $vf6, 0x0(%4)\n"
                 "lqc2 $vf5, 0x0(%3)\n"
                 "vsub.xyz $vf4, $vf4, $vf6\n"
                 "vsub.xyz $vf5, $vf5, $vf6\n"
                 "vaddy.x $vf6, $vf0, $vf4y\n"
                 "vaddz.x $vf7, $vf0, $vf4z\n"
                 "vaddy.x $vf8, $vf0, $vf5y\n"
                 "vaddz.x $vf9, $vf0, $vf5z\n"
                 "vmulax.x $ACC, $vf4, $vf4x\n"
                 "vmadday.x $ACC, $vf6, $vf4y\n"
                 "vmaddz.x $vf4, $vf7, $vf4z\n"
                 "vmulax.x $ACC, $vf5, $vf5x\n"
                 "vmadday.x $ACC, $vf8, $vf5y\n"
                 "vmaddz.x $vf5, $vf9, $vf5z\n"
                 "qmfc2 $v0, $vf4\n"
                 "qmfc2 $v1, $vf5\n"
                 "mtc1 $v0, %0\n"
                 "mtc1 $v1, %1\n"
                 : "=f"(dot0), "=f"(dot1)
                 : "r"(&v0), "r"(&v1), "r"(&v2)
                 : "$2", "$3");
    if (dot0 <= dot1) {
        return 0;
    }
    return 1;
}

void Math3D::HermiteMatrix(FMATRIX& outMat, const FVECTOR* v0, const FVECTOR* v1, const FVECTOR* v2, const FVECTOR* v3, FLOAT fAlpha) {
    FVECTOR vTmp[3];
    FVECTOR vVec[2];
    float fAlphas[2];
#line 1682
    ASSERT(v1);
    ASSERT(v2);
    fAlphas[0] = fAlpha;
    fAlphas[1] = fAlpha;
    if (v0 == NULL) {
        SubVector(vVec[0], *v1, *v2);
        AddVector(vVec[0], vVec[0], *v1);
        v0 = &vVec[0];
    }
    if (v3 == NULL) {
        SubVector(vVec[1], *v2, *v1);
        AddVector(vVec[1], vVec[1], *v2);
        v3 = &vVec[1];
    }
    outMat.v[0] = *v1;
    outMat.v[1] = *v2;
    SubVector(vTmp[0], *v1, *v0);
    SubVector(vTmp[1], *v2, *v1);
    SubVector(vTmp[2], *v3, *v2);
    AddVector(outMat.v[2], vTmp[0], vTmp[1]);
    ScaleVector(outMat.v[2], outMat.v[2], fAlphas[0]);
    AddVector(outMat.v[3], vTmp[1], vTmp[2]);
    ScaleVector(outMat.v[3], outMat.v[3], fAlphas[1]);
}

void Math3D::HermiteTCBMatrix(FMATRIX& outMat, const FVECTOR* v0, const FVECTOR* v1, const FVECTOR* v2, const FVECTOR* v3, FLOAT fAlpha) {
    FVECTOR vTmp[3];
    FVECTOR vVec[2];
    float fAlphas[2];
#line 1719
    ASSERT(v1);
    ASSERT(v2);
    fAlphas[0] = fAlpha;
    fAlphas[1] = fAlpha;
    if (v0 == NULL) {
        SubVector(vVec[0], *v1, *v2);
        AddVector(vVec[0], vVec[0], *v1);
        v0 = &vVec[0];
    }
    if (v3 == NULL) {
        SubVector(vVec[1], *v2, *v1);
        AddVector(vVec[1], vVec[1], *v2);
        v3 = &vVec[1];
    }
    outMat.v[0] = *v1;
    outMat.v[1] = *v2;
    SubVector(vTmp[0], *v1, *v0);
    SubVector(vTmp[1], *v2, *v1);
    SubVector(vTmp[2], *v3, *v2);
    AddVector(outMat.v[2], vTmp[0], vTmp[1]);
    ScaleVector(outMat.v[2], outMat.v[2], fAlphas[0]);
    AddVector(outMat.v[3], vTmp[1], vTmp[2]);
    ScaleVector(outMat.v[3], outMat.v[3], fAlphas[1]);
    vTmp[0].w = VectorLength(vTmp[0]);
    vTmp[1].w = VectorLength(vTmp[1]);
    vTmp[2].w = VectorLength(vTmp[2]);
    ScaleVectorXYZ(outMat.v[2], outMat.v[2], (vTmp[1].w + vTmp[1].w) / (vTmp[0].w + vTmp[1].w));
    ScaleVectorXYZ(outMat.v[3], outMat.v[3], (vTmp[1].w + vTmp[1].w) / (vTmp[1].w + vTmp[2].w));
}

void Math3D::HermiteVector(FVECTOR& outVec, FLOAT t) {
    FLOAT tt = t * t;
    FLOAT ttt = tt * t;
    FLOAT ttt2 = ttt + ttt;
    FLOAT tt3 = tt * 3.0f;
    outVec.x = ttt2 - tt3 + 1.0f;
    outVec.y = tt3 - ttt2;
    outVec.z = ttt - (tt + tt) + t;
    outVec.w = ttt - tt;
}

void Math3D::HermitePosition(FVECTOR& outVec, const FVECTOR* v0, const FVECTOR* v1, const FVECTOR* v2, const FVECTOR* v3, FLOAT t, FLOAT fAlpha) {
    FMATRIX mMat;
    FVECTOR vVec;
    HermiteMatrix(mMat, v0, v1, v2, v3, fAlpha);
    HermiteInterpolation(outVec, mMat, t);
}

void Math3D::HermiteTCBPosition(FVECTOR& outVec, const FVECTOR* v0, const FVECTOR* v1, const FVECTOR* v2, const FVECTOR* v3, FLOAT t, FLOAT fAlpha) {
    FMATRIX mMat;
    FVECTOR vVec;
    HermiteTCBMatrix(mMat, v0, v1, v2, v3, fAlpha);
    HermiteInterpolation(outVec, mMat, t);
}

INT Math3D::checkFloat(FLOAT pai) {
    INT iPai = *(INT*)&pai;
    INT iPaiE = (iPai >> 23) & 0xFF;
    INT iPaiF = iPai & 0x7FFFFF;
    if (iPaiE == 0xFF) {
        if (iPaiF != 0) {
            /* "FLOAT値の内容が非数です 0x%08x \n" (Shift-JIS) */
            TRACE("FLOAT\222l\202\314\223\340\227e\202\252\224\361\220\224\202\305\202\267 0x%08x \n", iPai);
        } else {
            /* "FLOAT値の内容が無限大です 0x%08x \n" */
            TRACE("FLOAT\222l\202\314\223\340\227e\202\252\226\263\214\300\221\345\202\305\202\267 0x%08x \n", iPai);
        }
#line 1882
        AssertApp(__FILE__, __LINE__, "iPai", 0);
    } else if (iPaiE == 0 && iPaiF != 0) {
        /* "FLOAT値の内容が非正規化数です 0x%08x \n" */
        TRACE("FLOAT\222l\202\314\223\340\227e\202\252\224\361\220\263\213K\211\273\220\224\202\305\202\267 0x%08x \n", iPai);
    }
    return iPaiE;
}

FLOAT Math3D::GetIntersectionTimeOfFrontPlaneAndLine(FVECTOR& vOutHitPos, const FVECTOR& vPlaneNormal, const FVECTOR& vPlanePos, const FVECTOR& vSLine, const FVECTOR& vELine) {
    float bottom;
    asm volatile("lqc2 $vf6, 0x0(%1)\n"
                 "lqc2 $vf7, 0x0(%2)\n"
                 "lqc2 $vf4, 0x0(%3)\n"
                 "vsub.xyz $vf6, $vf6, $vf7\n"
                 "vaddy.x $vf1, $vf0, $vf4y\n"
                 "vaddz.x $vf2, $vf0, $vf4z\n"
                 "vmulax.x $ACC, $vf4, $vf6x\n"
                 "vmadday.x $ACC, $vf1, $vf6y\n"
                 "vmaddz.x $vf8, $vf2, $vf6z\n"
                 "qmfc2 $v0, $vf8\n"
                 "mtc1 $v0, %0\n"
                 : "=f"(bottom)
                 : "r"(&vELine), "r"(&vSLine), "r"(&vPlaneNormal)
                 : "$2");
    if (bottom >= 0.0f) {
        return 1.7014118346046923e+38f;
    }
    asm volatile("lqc2 $vf5, 0x0(%2)\n"
                 "vsub.xyz $vf9, $vf5, $vf7\n"
                 "vmulax.x $ACC, $vf4, $vf9x\n"
                 "vmadday.x $ACC, $vf1, $vf9y\n"
                 "vmaddz.x $vf9, $vf2, $vf9z\n"
                 "vdiv $Q, $vf9x, $vf8x\n"
                 "vaddx.w $vf9, $vf0, $vf0x\n"
                 "vadda.xyz $ACC, $vf7, $vf0\n"
                 "vwaitq\n"
                 "vmaddq.xyz $vf9, $vf6, $Q\n"
                 "cfc2 $v0, $vi22\n"
                 "mtc1 $v0, %0\n"
                 "sqc2 $vf9, 0x0(%1)\n"
                 : "=f"(bottom)
                 : "r"(&vOutHitPos), "r"(&vPlanePos)
                 : "$2", "memory");
    return bottom;
}

FLOAT Math3D::GetIntersectionTimeOfPlaneAndLine(FVECTOR& vOutHitPos, const FVECTOR& vPlaneNormal, const FVECTOR& vPlanePos, const FVECTOR& vSLine, const FVECTOR& vELine) {
    float bottom;
    asm volatile("lqc2 $vf6, 0x0(%1)\n"
                 "lqc2 $vf7, 0x0(%2)\n"
                 "lqc2 $vf4, 0x0(%3)\n"
                 "vsub.xyz $vf6, $vf6, $vf7\n"
                 "vaddy.x $vf1, $vf0, $vf4y\n"
                 "vaddz.x $vf2, $vf0, $vf4z\n"
                 "vmulax.x $ACC, $vf4, $vf6x\n"
                 "vmadday.x $ACC, $vf1, $vf6y\n"
                 "vmaddz.x $vf8, $vf2, $vf6z\n"
                 "qmfc2 $v0, $vf8\n"
                 "mtc1 $v0, %0\n"
                 : "=f"(bottom)
                 : "r"(&vELine), "r"(&vSLine), "r"(&vPlaneNormal)
                 : "$2");
    if (bottom == 0.0f) {
        return 1.7014118346046923e+38f;
    }
    asm volatile("lqc2 $vf5, 0x0(%2)\n"
                 "vsub.xyz $vf9, $vf5, $vf7\n"
                 "vmulax.x $ACC, $vf4, $vf9x\n"
                 "vmadday.x $ACC, $vf1, $vf9y\n"
                 "vmaddz.x $vf9, $vf2, $vf9z\n"
                 "vdiv $Q, $vf9x, $vf8x\n"
                 "vaddx.w $vf9, $vf0, $vf0x\n"
                 "vadda.xyz $ACC, $vf7, $vf0\n"
                 "vwaitq\n"
                 "vmaddq.xyz $vf9, $vf6, $Q\n"
                 "cfc2 $v0, $vi22\n"
                 "mtc1 $v0, %0\n"
                 "sqc2 $vf9, 0x0(%1)\n"
                 : "=f"(bottom)
                 : "r"(&vOutHitPos), "r"(&vPlanePos)
                 : "$2", "memory");
    return bottom;
}

BOOL Math3D::CheckInsidePlane3(const FVECTOR& vHitPos, const FVECTOR& vPlaneNormal, const FVECTOR& vPlanePos0, const FVECTOR& vPlanePos1, const FVECTOR& vPlanePos2) {
    INT val;
    asm volatile("vmaxw.xyzw $vf6, $vf0, $vf0w\n"
                 "lqc2 $vf4, 0x0(%1)\n"
                 "lqc2 $vf5, 0x0(%2)\n"
                 "lqc2 $vf10, 0x0(%3)\n"
                 "lqc2 $vf11, 0x0(%4)\n"
                 "lqc2 $vf12, 0x0(%5)\n"
                 "vsub.xyz $vf7, $vf11, $vf10\n"
                 "vsub.xyz $vf8, $vf12, $vf10\n"
                 "vsub.xyz $vf9, $vf4, $vf10\n"
                 "vopmula.xyz $ACC, $vf5, $vf7\n"
                 "vopmsub.xyz $vf7, $vf7, $vf5\n"
                 "vmul.xyz $vf8, $vf7, $vf8\n"
                 "vmul.xyz $vf9, $vf7, $vf9\n"
                 "vadday.x $ACC, $vf8, $vf8y\n"
                 "vmaddz.x $vf8, $vf6, $vf8z\n"
                 "vadday.x $ACC, $vf9, $vf9y\n"
                 "vmaddz.x $vf9, $vf6, $vf9z\n"
                 "qmfc2 %0, $vf8\n"
                 "qmfc2 $v1, $vf9\n"
                 "xor %0, %0, $v1\n"
                 "sll %0, %0, 0\n"
                 : "=r"(val)
                 : "r"(&vHitPos), "r"(&vPlaneNormal), "r"(&vPlanePos0), "r"(&vPlanePos1), "r"(&vPlanePos2)
                 : "$3");
    if (val < 0) {
        return FALSE;
    }
    asm volatile("vsub.xyz $vf7, $vf12, $vf11\n"
                 "vsub.xyz $vf8, $vf10, $vf11\n"
                 "vsub.xyz $vf9, $vf4, $vf11\n"
                 "vopmula.xyz $ACC, $vf5, $vf7\n"
                 "vopmsub.xyz $vf7, $vf7, $vf5\n"
                 "vmul.xyz $vf8, $vf7, $vf8\n"
                 "vmul.xyz $vf9, $vf7, $vf9\n"
                 "vadday.x $ACC, $vf8, $vf8y\n"
                 "vmaddz.x $vf8, $vf6, $vf8z\n"
                 "vadday.x $ACC, $vf9, $vf9y\n"
                 "vmaddz.x $vf9, $vf6, $vf9z\n"
                 "qmfc2 %0, $vf8\n"
                 "qmfc2 $v1, $vf9\n"
                 "xor %0, %0, $v1\n"
                 "sll %0, %0, 0\n"
                 : "=r"(val)
                 :
                 : "$3");
    if (val < 0) {
        return FALSE;
    }
    asm volatile("vsub.xyz $vf7, $vf10, $vf12\n"
                 "vsub.xyz $vf8, $vf11, $vf12\n"
                 "vsub.xyz $vf9, $vf4, $vf12\n"
                 "vopmula.xyz $ACC, $vf5, $vf7\n"
                 "vopmsub.xyz $vf7, $vf7, $vf5\n"
                 "vmul.xyz $vf8, $vf7, $vf8\n"
                 "vmul.xyz $vf9, $vf7, $vf9\n"
                 "vadday.x $ACC, $vf8, $vf8y\n"
                 "vmaddz.x $vf8, $vf6, $vf8z\n"
                 "vadday.x $ACC, $vf9, $vf9y\n"
                 "vmaddz.x $vf9, $vf6, $vf9z\n"
                 "qmfc2 %0, $vf8\n"
                 "qmfc2 $v1, $vf9\n"
                 "xor %0, %0, $v1\n"
                 "sll %0, %0, 0\n"
                 : "=r"(val)
                 :
                 : "$3");
    if (val < 0) {
        return FALSE;
    } else {
        return TRUE;
    }
}

BOOL Math3D::CheckInsidePlane4(const FVECTOR& vHitPos, const FVECTOR& vPlaneNormal, const FVECTOR& vPlanePos0, const FVECTOR& vPlanePos1, const FVECTOR& vPlanePos2, const FVECTOR& vPlanePos3) {
    INT val;
    asm volatile("vmaxw.xyzw $vf6, $vf0, $vf0w\n"
                 "lqc2 $vf4, 0x0(%1)\n"
                 "lqc2 $vf5, 0x0(%2)\n"
                 "lqc2 $vf10, 0x0(%3)\n"
                 "lqc2 $vf11, 0x0(%4)\n"
                 "lqc2 $vf12, 0x0(%5)\n"
                 "lqc2 $vf13, 0x0(%6)\n"
                 "vsub.xyz $vf7, $vf11, $vf10\n"
                 "vsub.xyz $vf8, $vf12, $vf10\n"
                 "vsub.xyz $vf9, $vf4, $vf10\n"
                 "vopmula.xyz $ACC, $vf5, $vf7\n"
                 "vopmsub.xyz $vf7, $vf7, $vf5\n"
                 "vmul.xyz $vf8, $vf7, $vf8\n"
                 "vmul.xyz $vf9, $vf7, $vf9\n"
                 "vadday.x $ACC, $vf8, $vf8y\n"
                 "vmaddz.x $vf8, $vf6, $vf8z\n"
                 "vadday.x $ACC, $vf9, $vf9y\n"
                 "vmaddz.x $vf9, $vf6, $vf9z\n"
                 "qmfc2 %0, $vf8\n"
                 "qmfc2 $v1, $vf9\n"
                 "xor %0, %0, $v1\n"
                 "sll %0, %0, 0\n"
                 : "=r"(val)
                 : "r"(&vHitPos), "r"(&vPlaneNormal), "r"(&vPlanePos0), "r"(&vPlanePos1), "r"(&vPlanePos2), "r"(&vPlanePos3)
                 : "$3");
    if (val < 0) {
        return FALSE;
    }
    asm volatile("vsub.xyz $vf7, $vf12, $vf11\n"
                 "vsub.xyz $vf8, $vf13, $vf11\n"
                 "vsub.xyz $vf9, $vf4, $vf11\n"
                 "vopmula.xyz $ACC, $vf5, $vf7\n"
                 "vopmsub.xyz $vf7, $vf7, $vf5\n"
                 "vmul.xyz $vf8, $vf7, $vf8\n"
                 "vmul.xyz $vf9, $vf7, $vf9\n"
                 "vadday.x $ACC, $vf8, $vf8y\n"
                 "vmaddz.x $vf8, $vf6, $vf8z\n"
                 "vadday.x $ACC, $vf9, $vf9y\n"
                 "vmaddz.x $vf9, $vf6, $vf9z\n"
                 "qmfc2 %0, $vf8\n"
                 "qmfc2 $v1, $vf9\n"
                 "xor %0, %0, $v1\n"
                 "sll %0, %0, 0\n"
                 : "=r"(val)
                 :
                 : "$3");
    if (val < 0) {
        return FALSE;
    }
    asm volatile("vsub.xyz $vf7, $vf13, $vf12\n"
                 "vsub.xyz $vf8, $vf10, $vf12\n"
                 "vsub.xyz $vf9, $vf4, $vf12\n"
                 "vopmula.xyz $ACC, $vf5, $vf7\n"
                 "vopmsub.xyz $vf7, $vf7, $vf5\n"
                 "vmul.xyz $vf8, $vf7, $vf8\n"
                 "vmul.xyz $vf9, $vf7, $vf9\n"
                 "vadday.x $ACC, $vf8, $vf8y\n"
                 "vmaddz.x $vf8, $vf6, $vf8z\n"
                 "vadday.x $ACC, $vf9, $vf9y\n"
                 "vmaddz.x $vf9, $vf6, $vf9z\n"
                 "qmfc2 %0, $vf8\n"
                 "qmfc2 $v1, $vf9\n"
                 "xor %0, %0, $v1\n"
                 "sll %0, %0, 0\n"
                 : "=r"(val)
                 :
                 : "$3");
    if (val < 0) {
        return FALSE;
    }
    asm volatile("vsub.xyz $vf7, $vf10, $vf13\n"
                 "vsub.xyz $vf8, $vf11, $vf13\n"
                 "vsub.xyz $vf9, $vf4, $vf13\n"
                 "vopmula.xyz $ACC, $vf5, $vf7\n"
                 "vopmsub.xyz $vf7, $vf7, $vf5\n"
                 "vmul.xyz $vf8, $vf7, $vf8\n"
                 "vmul.xyz $vf9, $vf7, $vf9\n"
                 "vadday.x $ACC, $vf8, $vf8y\n"
                 "vmaddz.x $vf8, $vf6, $vf8z\n"
                 "vadday.x $ACC, $vf9, $vf9y\n"
                 "vmaddz.x $vf9, $vf6, $vf9z\n"
                 "qmfc2 %0, $vf8\n"
                 "qmfc2 $v1, $vf9\n"
                 "xor %0, %0, $v1\n"
                 "sll %0, %0, 0\n"
                 : "=r"(val)
                 :
                 : "$3");
    if (val < 0) {
        return FALSE;
    } else {
        return TRUE;
    }
}

BOOL Math3D::CheckInsidePlane4Neighborhood2(const FVECTOR& vHitPos, const FVECTOR& vPlaneNormal, const FVECTOR& vPlanePos0, const FVECTOR& vPlanePos1, const FVECTOR& vPlanePos2, const FVECTOR& vPlanePos3) {
    INT val;
    asm volatile("vmaxw.xyzw $vf6, $vf0, $vf0w\n"
                 "lqc2 $vf4, 0x0(%1)\n"
                 "lqc2 $vf5, 0x0(%2)\n"
                 "lqc2 $vf10, 0x0(%3)\n"
                 "lqc2 $vf11, 0x0(%4)\n"
                 "lqc2 $vf12, 0x0(%5)\n"
                 "lqc2 $vf13, 0x0(%6)\n"
                 "vsub.xyz $vf7, $vf11, $vf10\n"
                 "vsub.xyz $vf8, $vf12, $vf10\n"
                 "vsub.xyz $vf9, $vf4, $vf10\n"
                 "vopmula.xyz $ACC, $vf5, $vf7\n"
                 "vopmsub.xyz $vf7, $vf7, $vf5\n"
                 "vmul.xyz $vf8, $vf7, $vf8\n"
                 "vmul.xyz $vf9, $vf7, $vf9\n"
                 "vadday.x $ACC, $vf8, $vf8y\n"
                 "vmaddz.x $vf8, $vf6, $vf8z\n"
                 "vadday.x $ACC, $vf9, $vf9y\n"
                 "vmaddz.x $vf9, $vf6, $vf9z\n"
                 "qmfc2 %0, $vf8\n"
                 "qmfc2 $v1, $vf9\n"
                 "xor %0, %0, $v1\n"
                 "sll %0, %0, 0\n"
                 : "=r"(val)
                 : "r"(&vHitPos), "r"(&vPlaneNormal), "r"(&vPlanePos0), "r"(&vPlanePos1), "r"(&vPlanePos2), "r"(&vPlanePos3)
                 : "$3");
    if (val < 0) {
        return FALSE;
    }
    asm volatile("vsub.xyz $vf7, $vf13, $vf12\n"
                 "vsub.xyz $vf8, $vf10, $vf12\n"
                 "vsub.xyz $vf9, $vf4, $vf12\n"
                 "vopmula.xyz $ACC, $vf5, $vf7\n"
                 "vopmsub.xyz $vf7, $vf7, $vf5\n"
                 "vmul.xyz $vf8, $vf7, $vf8\n"
                 "vmul.xyz $vf9, $vf7, $vf9\n"
                 "vadday.x $ACC, $vf8, $vf8y\n"
                 "vmaddz.x $vf8, $vf6, $vf8z\n"
                 "vadday.x $ACC, $vf9, $vf9y\n"
                 "vmaddz.x $vf9, $vf6, $vf9z\n"
                 "qmfc2 %0, $vf8\n"
                 "qmfc2 $v1, $vf9\n"
                 "xor %0, %0, $v1\n"
                 "sll %0, %0, 0\n"
                 : "=r"(val)
                 :
                 : "$3");
    if (val < 0) {
        return FALSE;
    } else {
        return TRUE;
    }
}

BOOL Math3D::EqualVector(const FVECTOR& vec0, const FVECTOR& vec1) {
    register BOOL ret asm("$2");
    asm volatile("lqc2 $vf4, 0x0(%1)\n"
                 "lqc2 $vf5, 0x0(%2)\n"
                 "li $v1, 0x3A03126F\n"
                 "qmtc2 $v1, $vf6\n"
                 "vsub.xyz $vf4, $vf4, $vf5\n"
                 "vabs.xyz $vf4, $vf4\n"
                 "vsubx.xyz $vf0, $vf4, $vf6x\n"
                 "vnop\n"
                 "vnop\n"
                 "daddiu $v1, $zero, 0xE0\n"
                 "vnop\n"
                 "vnop\n"
                 "cfc2 %0, $vi17\n"
                 "sll $t4, %0, 4\n"
                 "or %0, %0, $t4\n"
                 "and %0, %0, $v1\n"
                 ".set noreorder\n"
                 "bnel %0, $v1, 1f\n"
                 "daddu %0, $zero, $zero\n"
                 "1:\n"
                 ".set reorder\n"
                 : "=r"(ret)
                 : "r"(&vec0), "r"(&vec1)
                 : "$3", "$12");
    return ret;
}

BOOL Math3D::EqualVector(const FVECTOR& vec0, const FVECTOR& vec1, FLOAT fParam) {
    register BOOL ret asm("$2");
    asm volatile("lqc2 $vf4, 0x0(%1)\n"
                 "lqc2 $vf5, 0x0(%2)\n"
                 "mfc1 $v1, %3\n"
                 "qmtc2 $v1, $vf6\n"
                 "vsub.xyz $vf4, $vf4, $vf5\n"
                 "vabs.xyz $vf4, $vf4\n"
                 "vsubx.xyz $vf0, $vf4, $vf6x\n"
                 "vnop\n"
                 "vnop\n"
                 "daddiu $v1, $zero, 0xE0\n"
                 "vnop\n"
                 "vnop\n"
                 "cfc2 %0, $vi17\n"
                 "sll $t4, %0, 4\n"
                 "or %0, %0, $t4\n"
                 "and %0, %0, $v1\n"
                 ".set noreorder\n"
                 "bnel %0, $v1, 1f\n"
                 "daddu %0, $zero, $zero\n"
                 "1:\n"
                 ".set reorder\n"
                 : "=r"(ret)
                 : "r"(&vec0), "r"(&vec1), "f"(fParam)
                 : "$3", "$12");
    return ret;
}

BOOL Math3D::EqualVector(const FVECTOR& vec0) {
    register BOOL ret asm("$2");
    asm volatile("lqc2 $vf4, 0x0(%1)\n"
                 "li $v1, 0x38D1B717\n"
                 "qmtc2 $v1, $vf6\n"
                 "vabs.xyz $vf4, $vf4\n"
                 "vsubx.xyz $vf0, $vf4, $vf6x\n"
                 "vnop\n"
                 "vnop\n"
                 "daddiu $v1, $zero, 0xE0\n"
                 "vnop\n"
                 "vnop\n"
                 "cfc2 %0, $vi17\n"
                 "sll $t4, %0, 4\n"
                 "or %0, %0, $t4\n"
                 "and %0, %0, $v1\n"
                 ".set noreorder\n"
                 "bnel %0, $v1, 1f\n"
                 "daddu %0, $zero, $zero\n"
                 "1:\n"
                 ".set reorder\n"
                 : "=r"(ret)
                 : "r"(&vec0)
                 : "$3", "$12");
    return ret;
}

BOOL Math3D::EqualVector(const FVECTOR& vec0, FLOAT fParam) {
    register BOOL ret asm("$2");
    asm volatile("lqc2 $vf4, 0x0(%1)\n"
                 "mfc1 $v1, %2\n"
                 "qmtc2 $v1, $vf6\n"
                 "vabs.xyz $vf4, $vf4\n"
                 "vsubx.xyz $vf0, $vf4, $vf6x\n"
                 "vnop\n"
                 "vnop\n"
                 "daddiu $v1, $zero, 0xE0\n"
                 "vnop\n"
                 "vnop\n"
                 "cfc2 %0, $vi17\n"
                 "sll $t4, %0, 4\n"
                 "or %0, %0, $t4\n"
                 "and %0, %0, $v1\n"
                 ".set noreorder\n"
                 "bnel %0, $v1, 1f\n"
                 "daddu %0, $zero, $zero\n"
                 "1:\n"
                 ".set reorder\n"
                 : "=r"(ret)
                 : "r"(&vec0), "f"(fParam)
                 : "$3", "$12");
    return ret;
}

FLOAT Math3D::GetDistance2OfLineAndPoint(const FVECTOR& p0, const FVECTOR& p1, const FVECTOR& q) {
    FVECTOR pv;
    FVECTOR qp;
    FVECTOR qn;
    FVECTOR nv;
    SubVector(pv, p1, p0);
    SubVector(qp, q, p0);
    FLOAT a = InnerProduct(qp, pv);
    FLOAT b = InnerProduct(pv);
    ScaleVector(qn, pv, a / b);
    SubVector(nv, qp, qn);
    return InnerProduct(nv);
}

FLOAT Math3D::GetDistance2OfLimitedLineAndPoint(const FVECTOR& p0, const FVECTOR& p1, const FVECTOR& q) {
    FVECTOR pv;
    FVECTOR qp;
    FVECTOR qn;
    FVECTOR nv;
    FLOAT t;
    SubVector(pv, p1, p0);
    SubVector(qp, q, p0);
    t = InnerProduct(qp, pv) / InnerProduct(pv);
    if (t < 0.0f) {
        SubVector(nv, p0, q);
    } else if (t > 1.0f) {
        SubVector(nv, p1, q);
    } else {
        ScaleVector(qn, pv, t);
        SubVector(nv, qp, qn);
    }
    return InnerProduct(nv);
}

FLOAT Math3D::GetIntersectionOfLineAndPoint(FVECTOR& vOutI, const FVECTOR& p0, const FVECTOR& p1, const FVECTOR& q) {
    FVECTOR pv;
    FVECTOR qp;
    FVECTOR qn;
    FVECTOR nv;
    FLOAT t;
    SubVector(pv, p1, p0);
    SubVector(qp, q, p0);
    t = InnerProduct(qp, pv) / InnerProduct(pv);
    ScaleVector(qn, pv, t);
    AddVector(vOutI, p0, qn);
    return t;
}

FLOAT Math3D::LimitPI(FLOAT pai) {
    INT e = checkFloat(pai) - 127;
    if (e > 32) {
        /* "FLOAT値のPIの指数部が大きすぎます 2^%d (0x%08x) (%f)\n" */
        TRACE("FLOAT\222l\202\314PI\202\314\216w\220\224\225\224\202\252\221\345\202\253\202\267\202\254\202\334\202\267 2^%d (0x%08x) (%f)\n", e,
              *(INT*)&pai, pai);
#line 3227
        AssertApp(__FILE__, __LINE__, "e", 0);
    }
    FLOAT fPI = 3.1415927f;
    if (pai > fPI) {
        FLOAT fPI8 = 25.132742f;
        FLOAT fPI2 = 6.2831855f;
        if (pai > fPI8) {
            pai -= (INT)(pai / fPI2) * fPI2;
            if (pai > fPI) {
                pai -= fPI2;
            }
        } else {
            do {
                pai -= fPI2;
            } while (pai > fPI);
        }
    } else if (pai < -fPI) {
        FLOAT fPI8 = -25.132742f;
        FLOAT fPI2 = 6.2831855f;
        if (pai < fPI8) {
            pai -= (INT)(pai / fPI2) * fPI2;
            if (pai < -fPI) {
                pai += fPI2;
            }
        } else {
            do {
                pai += fPI2;
            } while (pai < -fPI);
        }
    }
    return pai;
}

void Math3D::LimitPIVector(FVECTOR& pai) {
    pai.x = LimitPI(pai.x);
    pai.y = LimitPI(pai.y);
    pai.z = LimitPI(pai.z);
}

INT Math3D::GetMaxVectorElement(const FVECTOR& vNorm) {
    INT iVal;
    INT iAbsX;
    INT iAbsY;
    asm volatile("lqc2 $vf4, 0x0(%3)\n"
                 "vabs.xyz $vf4, $vf4\n"
                 "qmfc2 %1, $vf4\n"
                 "vmaxy.x $vf5, $vf4, $vf4y\n"
                 "prot3w %2, %1\n"
                 "sll %1, %1, 0\n"
                 "vmaxz.x $vf5, $vf5, $vf4z\n"
                 "sll %2, %2, 0\n"
                 "qmfc2 %0, $vf5\n"
                 "sll %0, %0, 0\n"
                 : "=r"(iVal), "=r"(iAbsX), "=r"(iAbsY)
                 : "r"(&vNorm));
    if (iVal == iAbsX) {
        return 0;
    }
    if (iVal == iAbsY) {
        return 1;
    }
    return 2;
}

BOOL Math3D::HitPointBall(const FVECTOR& p, const FVECTOR& c, FLOAT cr) {
    register FLOAT t asm("$f12");
    register FLOAT cr2 asm("$f0");
    asm volatile("lqc2 $vf4, 0x0(%0)\n"
                 "lqc2 $vf5, 0x0(%1)\n"
                 :
                 : "r"(&p), "r"(&c));
    cr2 = cr * cr;
    asm volatile("vsub.xyz $vf4, $vf4, $vf5\n"
                 "vaddy.x $vf5, $vf0, $vf4y\n"
                 "vaddz.x $vf6, $vf0, $vf4z\n"
                 "vmulax.x $ACC, $vf4, $vf4x\n"
                 "vmadday.x $ACC, $vf5, $vf4y\n"
                 "vmaddz.x $vf4, $vf6, $vf4z\n"
                 "qmfc2 $v1, $vf4\n"
                 "mtc1 $v1, %0\n"
                 : "=f"(t)
                 :
                 : "$3");
    if (t <= cr2) {
        return TRUE;
    }
    return FALSE;
}

BOOL Math3D::HitBallBall(const FVECTOR& c0, const FVECTOR& c1, FLOAT cr0, FLOAT cr1) {
    return HitPointBall(c0, c1, cr0 + cr1);
}

BOOL Math3D::HitPillarPoint(const FVECTOR& m, const FVECTOR& n, const FVECTOR& p, FLOAT mnr) {
    FLOAT t;
    FVECTOR h;
    asm volatile("lqc2 $vf4, 0x0(%1)\n"
                 "lqc2 $vf5, 0x0(%2)\n"
                 "lqc2 $vf6, 0x0(%3)\n"
                 "vsub.xyz $vf7, $vf4, $vf5\n"
                 "vaddy.x $vf1, $vf0, $vf7y\n"
                 "vaddz.x $vf2, $vf0, $vf7z\n"
                 "vmulax.x $ACC, $vf7, $vf7x\n"
                 "vmadday.x $ACC, $vf1, $vf7y\n"
                 "vmaddz.x $vf8, $vf2, $vf7z\n"
                 "qmfc2 $v1, $vf8\n"
                 "mtc1 $v1, %0\n"
                 : "=f"(t)
                 : "r"(&n), "r"(&m), "r"(&p)
                 : "$3");
    if (t == 0.0f) {
        return FALSE;
    }
    asm volatile("vdiv $Q, $vf0w, $vf8x\n"
                 "vsub.xyz $vf8, $vf6, $vf5\n"
                 "vmulax.x $ACC, $vf7, $vf8x\n"
                 "vmadday.x $ACC, $vf1, $vf8y\n"
                 "vmaddz.x $vf8, $vf2, $vf8z\n"
                 "vwaitq\n"
                 "vmulq.x $vf8, $vf8, $Q\n"
                 "qmfc2 $v1, $vf8\n"
                 "mtc1 $v1, %0\n"
                 : "=f"(t)
                 :
                 : "$3");
    if (t < 0.0f || t > 1.0f) {
        return FALSE;
    }
    asm volatile("vmulax.xyz $ACC, $vf7, $vf8x\n"
                 "vmaddw.xyz $vf8, $vf5, $vf0w\n"
                 "sqc2 $vf8, 0x0(%0)\n"
                 :
                 : "r"(&h)
                 : "memory");
    return HitPointBall(h, p, mnr);
}

BOOL Math3D::HitInfinityPillarPoint(const FVECTOR& m, const FVECTOR& n, const FVECTOR& p, FLOAT mnr) {
    FLOAT t;
    FVECTOR h;
    asm volatile("lqc2 $vf4, 0x0(%1)\n"
                 "lqc2 $vf5, 0x0(%2)\n"
                 "lqc2 $vf6, 0x0(%3)\n"
                 "vsub.xyz $vf7, $vf4, $vf5\n"
                 "vaddy.x $vf1, $vf0, $vf7y\n"
                 "vaddz.x $vf2, $vf0, $vf7z\n"
                 "vmulax.x $ACC, $vf7, $vf7x\n"
                 "vmadday.x $ACC, $vf1, $vf7y\n"
                 "vmaddz.x $vf8, $vf2, $vf7z\n"
                 "qmfc2 $v1, $vf8\n"
                 "mtc1 $v1, %0\n"
                 : "=f"(t)
                 : "r"(&n), "r"(&m), "r"(&p)
                 : "$3");
    if (t == 0.0f) {
        return FALSE;
    }
    asm volatile("vdiv $Q, $vf0w, $vf8x\n"
                 "vsub.xyz $vf8, $vf6, $vf5\n"
                 "vmulax.x $ACC, $vf7, $vf8x\n"
                 "vmadday.x $ACC, $vf1, $vf8y\n"
                 "vmaddz.x $vf8, $vf2, $vf8z\n"
                 "vwaitq\n"
                 "vmulq.x $vf8, $vf8, $Q\n"
                 "vmulax.xyz $ACC, $vf7, $vf8x\n"
                 "vmaddw.xyz $vf8, $vf5, $vf0w\n"
                 "sqc2 $vf8, 0x0(%0)\n"
                 :
                 : "r"(&h)
                 : "memory");
    return HitPointBall(h, p, mnr);
}

BOOL Math3D::HitLineBall(const FVECTOR& m, const FVECTOR& n, const FVECTOR& c, FLOAT cr) {
    if (HitPointBall(m, c, cr)) {
        return TRUE;
    }
    if (HitPointBall(n, c, cr)) {
        return TRUE;
    }
    return HitPillarPoint(m, n, c, cr);
}

BOOL Math3D::HitInfinityLineBall(const FVECTOR& m, const FVECTOR& n, const FVECTOR& c, FLOAT cr) {
    if (HitPointBall(m, c, cr)) {
        return TRUE;
    }
    if (HitPointBall(n, c, cr)) {
        return TRUE;
    }
    return HitInfinityPillarPoint(m, n, c, cr);
}

BOOL Math3D::HitPillarBall(const FVECTOR& m, const FVECTOR& n, const FVECTOR& c, FLOAT mnr, FLOAT cr) {
    return HitLineBall(m, n, c, mnr + cr);
}

INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/System/Math3D", InvMatrix__6Math3DR7FMATRIXRC7FMATRIX);
#if 0
/* Same code apart from register allocation: gcc hoists the 1e-8 constants and a "li 20" out of the outer loop
 * (the original keeps them inside), which moves &c into s1. */
void Math3D::InvMatrix(FMATRIX& outMat, const FMATRIX& inMat) {
    FMATRIX a;
    FMATRIX c;
    INT i;
    INT j;
    INT k;
    INT pibot;
    FLOAT fMax;
    FLOAT tmp;

    a = inMat;
    _unitMatrix(c);
    for (i = 0; i < 4; i++) {
        fMax = _fabsf(a.m[i][i]);
        pibot = i;
        for (j = i + 1; j < 4; j++) {
            if (_fabsf(a.m[j][i]) > fMax) {
                fMax = _fabsf(a.m[j][i]);
                pibot = j;
            }
        }
        if (pibot != i) {
            for (j = 0; j < 4; j++) {
                tmp = a.m[pibot][j];
                a.m[pibot][j] = a.m[i][j];
                a.m[i][j] = tmp;
                tmp = c.m[i][j];
                c.m[i][j] = c.m[pibot][j];
                c.m[pibot][j] = tmp;
            }
        }
        if (a.m[i][i] == 0.0f) {
            a.m[i][i] = 1e-08f;
        } else if (a.m[i][i] == -0.0f) {
            a.m[i][i] = -1e-08f;
        }
        for (j = i + 1; j < 4; j++) {
            a.m[j][i] /= a.m[i][i];
            for (k = i + 1; k < 4; k++) {
                a.m[j][k] -= a.m[i][k] * a.m[j][i];
            }
        }
    }
    for (k = 0; k < 4; k++) {
        for (INT i = 0; i < 4; i++) {
            for (j = i + 1; j < 4; j++) {
                c.m[j][k] -= c.m[i][k] * a.m[j][i];
            }
        }
        for (INT i = 3; i >= 0; i--) {
            for (j = i + 1; j < 4; j++) {
                c.m[i][k] -= a.m[i][j] * c.m[j][k];
            }
            c.m[i][k] /= a.m[i][i];
        }
    }
    outMat = c;
}

#endif
BOOL Math3D::checkMaxMinOutsideXYZ(const FVECTOR& v, const FVECTOR& vMax, const FVECTOR& vMin) {
    INT iRet;
    asm volatile("lqc2 $vf4, 0x0(%1)\n"
                 "lqc2 $vf5, 0x0(%2)\n"
                 "lqc2 $vf6, 0x0(%3)\n"
                 "ctc2 $zero, $vi16\n"
                 "vsub.xyz $vf0, $vf4, $vf5\n"
                 "vsub.xyz $vf0, $vf6, $vf4\n"
                 "vnop\n"
                 "vnop\n"
                 "vnop\n"
                 "vnop\n"
                 "vnop\n"
                 "cfc2 %0, $vi16\n"
                 : "=r"(iRet)
                 : "r"(&v), "r"(&vMin), "r"(&vMax));
    return iRet & 0xC0;
}

void Math3D::MatrixToAngleXYZ(FVECTOR& vOut, const FMATRIX& mIn) {
    float as;
    float cosbeta;
    FMATRIX m1 = mIn;
    as = m1.m[0][2];
    if (as > 1.0f) {
        as = 1.0f;
    } else if (as < -1.0f) {
        as = -1.0f;
    }
    vOut.y = -asinf(as);
    cosbeta = cosf(vOut.y);
    if (vOut.y >= -1.57f && vOut.y <= 1.57f) {
        vOut.x = atan2f(m1.m[1][2], m1.m[2][2]);
        as = m1.m[0][1] / cosbeta;
        if (as > 1.0f) {
            as = 1.0f;
        } else if (as < -1.0f) {
            as = -1.0f;
        }
        vOut.z = asinf(as);
        if (m1.m[0][0] < 0.0f) {
            vOut.z = 3.1415927f - vOut.z;
        }
    } else {
        vOut.z = atan2f(-m1.m[2][1], m1.m[1][1]);
        vOut.x = 0.0f;
        if (m1.m[0][2] < vOut.x) {
            vOut.z = -vOut.z;
        }
    }
    LimitPIVector(vOut);
}

void Math3D::MatrixToAngleZYX(FVECTOR& outvAngle, const FMATRIX& mMat) {
    FLOAT cosbeta;
    const FVECTOR* vec[3];
    vec[0] = &mMat.v[0];
    vec[1] = &mMat.v[1];
    vec[2] = &mMat.v[2];
    outvAngle.y = -asinf(vec[2]->x);
    cosbeta = cosf(outvAngle.y);
    if (outvAngle.y >= -1.57f && outvAngle.y <= 1.57f) {
        outvAngle.z = atan2f(vec[1]->x, vec[0]->x);
        outvAngle.x = asinf(vec[2]->y / cosbeta);
        if (vec[2]->z < 0.0f) {
            outvAngle.x = LimitPI(3.1415927f - outvAngle.x);
        }
    } else {
        outvAngle.z = -atan2f(-vec[0]->y, vec[1]->y);
        outvAngle.x = 0.0f;
    }
}

extern "C" float __ieee754_sqrtf(float x) {
    return _sqrtf(x);
}
/* InvMatrix is still INCLUDE_ASM: its -0.0f constant, the last item of the unit's .rodata, under the label it uses */
extern "C" const float D_0045C23C = -0.0f;
// SOURCE DATA: .rodata
