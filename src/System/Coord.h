
#ifndef SYSTEM_COORD_H
#define SYSTEM_COORD_H

#include "khtypes.h"
// The vector and matrix types (the ref: FVECTOR in types.h, IVECTOR and FMATRIX in System/Coord.h).
typedef float _FVECTOR[4] __attribute__((aligned(16)));
union FVECTOR;
void roundPI(FVECTOR& vInOut, INT nInClip); // System/Coord

union FVECTOR {
    _FVECTOR v;
    struct {
        FLOAT x;
        FLOAT y;
        FLOAT z;
        FLOAT w;
    };
    u_long128 _q;
    unsigned long _ul[2];
    unsigned int _ui[4];
    /* init0 is defined in the class on the original's lines (line markers: 87 the argument, 91 the sqc2); the
     * other inlines are defined after the classes, on the original's lines (652-904) */
#line 87
    void init0() {


        asm volatile("sqc2 $vf0, 0x0(%0)"
                     : : "r"(this));
    }
    void init(FLOAT inW = 1.0f); /* the parameter: Camera init clears w through it (line 729 stores 0) */
#ifdef VECTOR_H_SET_CALL /* MATCH: setCall is defined before set, so it keeps a call (ModelMan calls FVECTOR::set out of line) */
    void setCall(FLOAT inX, FLOAT inY, FLOAT inZ, FLOAT inW) { set(inX, inY, inZ, inW); }
#endif
    void set(FLOAT inX, FLOAT inY, FLOAT inZ, FLOAT inW = 1.0f);
    void set(const FVECTOR& inV);
    void set(const _FVECTOR& inV);
    void setAll(FLOAT f);
    void setAllXYZ(FLOAT f) { x = y = z = f; }
    void zero();
    void addRot(const FVECTOR& vInSrc);
    BOOL operator==(const FVECTOR& inV) const;
    BOOL operator!=(const FVECTOR& inV) const;
    void operator+=(const FVECTOR& inV);
    void operator+=(const _FVECTOR& inV);
    void operator-=(const FVECTOR& inV);
    void operator*=(const FVECTOR& inV);
    void operator*=(FLOAT f);
    FVECTOR operator+(const FVECTOR& inV) const;
    FVECTOR operator-(const FVECTOR& inV) const;
    FVECTOR operator-() const;
    FVECTOR operator*(const FVECTOR& inV) const;
    FVECTOR operator*(FLOAT f) const;
    operator _FVECTOR*();
    operator const _FVECTOR*() const;
    operator _FVECTOR&() const;
};

typedef int _IVECTOR[4] __attribute__((aligned(16)));
union IVECTOR {
    _IVECTOR v;
    struct {
        INT x;
        INT y;
        INT z;
        INT w;
    };
    u_long128 _q;
    unsigned long _ul[2];
    void init(/* parameters unknown */);
    void set(INT _x, INT _y, INT _z, INT _w);
    void set(const IVECTOR& inV);
    void setAll(/* parameters unknown */);
    void setAllXYZ(/* parameters unknown */);
    BOOL operator==(const IVECTOR& inV) const;
    BOOL operator!=(const IVECTOR& inV) const;
    void operator+=(const IVECTOR& inV);
    void operator-=(const IVECTOR& inV);
    void operator*=(INT n);
    IVECTOR operator+(const IVECTOR& inV) const;
    IVECTOR operator-(const IVECTOR& inV) const;
    IVECTOR operator*(INT n) const;
    operator _IVECTOR*() { return &v; }
    operator const _IVECTOR*() const { return &v; }
    operator _IVECTOR&() const { return *(_IVECTOR*)&v; }
};

typedef float _FMATRIX[4][4] __attribute__((aligned(16)));
union FMATRIX {
    _FMATRIX m;
    FVECTOR v[4];
    struct {
        FLOAT m00;
        FLOAT m10;
        FLOAT m20;
        FLOAT m30;
        FLOAT m01;
        FLOAT m11;
        FLOAT m21;
        FLOAT m31;
        FLOAT m02;
        FLOAT m12;
        FLOAT m22;
        FLOAT m32;
        FLOAT x;
        FLOAT y;
        FLOAT z;
        FLOAT w;
    };
    void init();
    void initWithScale(const FVECTOR& vScale);
    void initC3(/* parameters unknown */);
    void initTrans(/* parameters unknown */);
    /* on the original's lines (line markers: getTrans 158, getTransRef 159/160, setTrans 161) */
#line 158
    FVECTOR getTrans() const { return v[3]; }
    FVECTOR& getTransRef() { return v[3]; }
    const FVECTOR& getTransRef() const { return v[3]; }
    void setTrans(const FVECTOR& vecIn) { v[3] = vecIn; }
    void set(const _FMATRIX& inM);
    void set(/* parameters unknown */);
    BOOL operator==(const FMATRIX& inM) const;
    BOOL operator!=(const FMATRIX& inM) const;
    operator _FMATRIX*();
    operator const _FMATRIX*() const;
    operator _FMATRIX&() const;
};

union FVECTOR2 {
    float v[2];
    struct {
        FLOAT x;
        FLOAT y;
    };
    ULONGLONG _u64;
    LONGLONG _s64;
    /* on the original's lines (line markers: set 200, init0 201) */
#line 200
    void set(FLOAT fX, FLOAT fY) { x = fX; y = fY; }
    void init0() { _u64 = 0; }
};

/* The basic graphics types: in every original that has FVECTOR (274) they come right after the vector types and
 * before CCoord, in this order (the .mdebug type order; the ref spreads them over Fnt.h, Wnd.h, 2DSeqObj.h, Gprim.h
 * and VSME.h). Their set()s are one-liners on the original's lines (the line markers: 233, 245/246, 262/263, 274,
 * 285). */
union F_UL {
    FLOAT f;
    ULONG ul;
};

struct G_XY {
    FIX4 x;
    FIX4 y;
#line 233
    void set(FIX4 _x, FIX4 _y) { x = _x; y = _y; }
};

struct G_XYZ {
    FIX4 x;
    FIX4 y;
    FIX4 z;
#line 245
    void set(FIX4 _x, FIX4 _y, FIX4 _z) { x = _x; y = _y; z = _z; }
    void set(FIX4 _x, FIX4 _y) { x = _x; y = _y; }
    UINT bufferZ() { return z; }
};

struct G_RGBA {
    union {
        struct {
            UBYTE r;
            UBYTE g;
            UBYTE b;
            UBYTE a;
        };
        UINT col;
        UINT c;
        unsigned char v[4];
    };
#line 262
    void set(UBYTE _r, UBYTE _g, UBYTE _b, UBYTE _a) { r = _r; g = _g; b = _b; a = _a; }
    void set(UBYTE _r, UBYTE _g, UBYTE _b) { r = _r; g = _g; b = _b; }
};

struct G_UV {
    USHORT u;
    USHORT v;
#line 274
    void set(USHORT _u, USHORT _v) { u = _u; v = _v; }
};

struct G_ST {
    FLOAT s;
    FLOAT t;
#line 285
    void set(FLOAT _s, FLOAT _t) { s = _s; t = _t; }
};

/* The VU0 library (System/Coord defines it; the ref's Coord.h declares it, in this order). */
extern "C" {
void sceVu0ApplyMatrix(float *v0, float (*m0)[4], float *v1);
void sceVu0MulMatrix(float (*m0)[4], float (*m1)[4], float (*m2)[4]);
void sceVu0OuterProduct(float *v0, float *v1, float *v2);
float sceVu0InnerProduct(float *v0, float *v1);
void sceVu0Normalize(float *v0, float *v1);
void sceVu0TransposeMatrix(float (*m0)[4], float (*m1)[4]);
void sceVu0InversMatrix(float (*m0)[4], float (*m1)[4]);
void sceVu0DivVector(float *v0, float *v1, float q);
void sceVu0DivVectorXYZ(float *v0, float *v1, float q);
void sceVu0InterVector(float *v0, float *v1, float *v2, float t);
void sceVu0AddVector(float *v0, float *v1, float *v2);
void sceVu0SubVector(float *v0, float *v1, float *v2);
void sceVu0MulVector(float *v0, float *v1, float *v2);
void sceVu0ScaleVector(float *v0, float *v1, float t);
void sceVu0TransMatrix(float (*m0)[4], float (*m1)[4], float *tv);
void sceVu0CopyVector(float *v0, float *v1);
void sceVu0CopyMatrix(float (*m0)[4], float (*m1)[4]);
void sceVu0FTOI4Vector(int *v0, float *v1);
void sceVu0FTOI0Vector(int *v0, float *v1);
void sceVu0ITOF4Vector(float *v0, int *v1);
void sceVu0ITOF0Vector(float *v0, int *v1);
void sceVu0UnitMatrix(float (*m0)[4]);
void sceVu0RotMatrixZ(float (*m0)[4], float (*m1)[4], float rz);
void sceVu0RotMatrixX(float (*m0)[4], float (*m1)[4], float rx);
void sceVu0RotMatrixY(float (*m0)[4], float (*m1)[4], float ry);
void sceVu0RotMatrix(float (*m0)[4], float (*m1)[4], float *rot);
void sceVu0ClampVector(float *v0, float *v1, float min, float max);
void sceVu0CameraMatrix(float (*m)[4], float *p, float *zd, float *yd);
void sceVu0NormalLightMatrix(float (*m)[4], float *l0, float *l1, float *l2);
void sceVu0LightColorMatrix(float (*m)[4], float *c0, float *c1, float *c2, float *a);
void sceVu0ViewScreenMatrix(float (*m)[4], float scrz, float ax, float ay, float cx, float cy, float zmin, float zmax, float nearz, float farz);
void sceVu0DropShadowMatrix(float (*m)[4], float *lp, float a, float b, float c, int mode);
void sceVu0RotTransPersN(int (*v0)[4], float (*m0)[4], float (*v1)[4], int n, int mode);
void sceVu0RotTransPers(int *v0, float (*m0)[4], float *v1, int mode);
void sceVu0CopyVectorXYZ(float *v0, float *v1);
void sceVu0InterVectorXYZ(float *v0, float *v1, float *v2, float r);
void sceVu0ScaleVectorXYZ(float *v0, float *v1, float s);
int sceVu0ClipScreen(float *v0);
int sceVu0ClipScreen3(float *v0, float *v1, float *v2);
int sceVu0ClipAll(float *minv, float *maxv, float (*ms)[4], float (*vm)[4], int n);
}

void ApplyMatrix(FVECTOR& v0, const FMATRIX& m0, const FVECTOR& v1); // System/Coord
void MulMatrix(FMATRIX& res, const FMATRIX& m0, const FMATRIX& m1);   // System/Coord
#include "System/Graphic/VuCode/vu0.h"

/* The original's free inlines at 340-643 (its line markers). Names ours (the .mdebug has none for inlined
 * functions); parameter and local names from its inline blocks where they are visible: m1 (340, 613), m0 (613), v0
 * (344), matOut (354-370, 407, 429), matIn and fInRot (361-370), vecInTrans (396), vecPos/vecV/vecDir/matWork
 * (407-418), fInAx/fInAy (429), vecInB (477), vecOut (481), inT (477, 481), a and the result b (632-635), b
 * (642-643). The bodies pass the array members (.m/.v): through the conversion operators (one more inline call) cse
 * keeps the addresses in saved registers where the original recomputes them (FriendKing adjustPlayer). */
#line 340
inline void InversMatrix(FMATRIX& m0, const FMATRIX& m1) { sceVu0InversMatrix(m0.m, (float(*)[4])m1.m); }
#line 344
inline void ClampVector(FVECTOR& v0, const FVECTOR& v1, FLOAT fInMin, FLOAT fInMax) { sceVu0ClampVector(v0.v, (float*)v1.v, fInMin, fInMax); }
#line 354
inline void UnitMatrix(FMATRIX& matOut) { sceVu0UnitMatrix(matOut.m); }
#line 358
inline void RotMatrix(FMATRIX& matOut, const FMATRIX& matIn, const FVECTOR& vecInRot) { sceVu0RotMatrix(matOut.m, (float(*)[4])matIn.m, (float*)vecInRot.v); }
#line 361
inline void RotMatrixX(FMATRIX& matOut, const FMATRIX& matIn, FLOAT fInRot) {
    sceVu0RotMatrixX(matOut.m, (float(*)[4])matIn.m, fInRot);
}
#line 365
inline void RotMatrixY(FMATRIX& matOut, const FMATRIX& matIn, FLOAT fInRot) {
    sceVu0RotMatrixY(matOut.m, (float(*)[4])matIn.m, fInRot);
}
#line 369
inline void RotMatrixZ(FMATRIX& matOut, const FMATRIX& matIn, FLOAT fInRot) {
    sceVu0RotMatrixZ(matOut.m, (float(*)[4])matIn.m, fInRot);
}
#line 396
inline void TransMatrix(FMATRIX& matOut, const FMATRIX& matIn, const FVECTOR& vecInTrans) { sceVu0TransMatrix(matOut.m, (float(*)[4])matIn.m, (float*)vecInTrans.v); }
#line 407
inline void CameraMatrix(FMATRIX& matOut, const FVECTOR& vInPos, const FVECTOR& vInZd, const FVECTOR& vInYd, const FVECTOR& vInRot) {
    FMATRIX matWork;
    FVECTOR vecDir, vecV, vecPos;
    UnitMatrix(matWork);
    RotMatrixX(matWork, matWork, vInRot.x);
    RotMatrixY(matWork, matWork, vInRot.y);
    RotMatrixZ(matWork, matWork, vInRot.z);
    ApplyMatrix(vecDir, matWork, vInZd);
    ApplyMatrix(vecV, matWork, vInYd);
    ApplyMatrix(vecPos, matWork, vInPos);
    sceVu0CameraMatrix(matOut.m, vecPos.v, vecDir.v, vecV.v);
}
#line 429
inline void ViewScreenMatrix(FMATRIX& matOut, FLOAT fInScrz, FLOAT fInAx, FLOAT fInAy, FLOAT fInCx, FLOAT fInCy, FLOAT fInFar) {
    sceVu0ViewScreenMatrix(matOut.m, fInScrz, fInAx, fInAy, fInCx, fInCy, 1.0f, 16777000.0f, 0.1f, fInFar);
}
#line 477
inline void InterVector(FVECTOR& vecOut, const FVECTOR& vecInA, const FVECTOR& vecInB, FLOAT inT) {
    sceVu0InterVector(vecOut.v, (float*)vecInA.v, (float*)vecInB.v, inT);
}
#line 481
inline void InterVectorXYZ(FVECTOR& vecOut, const FVECTOR& vecInA, const FVECTOR& vecInB, FLOAT inT) {
    sceVu0InterVectorXYZ(vecOut.v, (float*)vecInA.v, (float*)vecInB.v, inT);
}
#line 498
/* the asm names the argument registers (System/Coord getMaxMinToVector8 is this inline alone) */
inline void GetMaxMinToVector8(FVECTOR* outV8, const FVECTOR& vMax, const FVECTOR& vMin) {
    asm volatile("lqc2 $vf4, 0x0($5)\n"
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
#line 563
inline void SetRotTransPersMatrix(const FMATRIX& m) {
    asm volatile("vsub.xyzw $vf3, $vf0, $vf0\n"
                 "lqc2 $vf4, 0x0(%0)\n"
                 "lqc2 $vf5, 0x10(%0)\n"
                 "lqc2 $vf6, 0x20(%0)\n"
                 "lqc2 $vf7, 0x30(%0)\n"
                 :
                 : "r"(&m));
}
#line 613
inline void InverseMatrix(FMATRIX& m0, const FMATRIX& m1) { sceVu0InversMatrix(m0.m, (float(*)[4])m1.m); }
#line 632
inline FVECTOR ApplyMatrix(const FMATRIX& m, const FVECTOR& a) {
    FVECTOR b;
    ApplyMatrix(b, m, a);
    return b;
}
#line 640
inline FMATRIX MulMatrix(const FMATRIX& m0, const FMATRIX& m1) {
    FMATRIX b;
    MulMatrix(b, m0, m1);
    return b;
}

/* The vector and matrix inlines with a place in the original's line markers, defined after the classes as there
 * (IVECTOR::set at 679 sits between FVECTOR's addRot 654-673 and init 729). The compound assignments open 9 lines
 * apart with their body 4 lines below (766/770, 775/779, 784/788, 802/806, 820/824), the binary operators 10 apart
 * (830: the copy at 835, the compound assignment at 836, the return at 837; 850, 870, 890). */
#line 652
inline void _addRot(FVECTOR& vOut, const FVECTOR& vInSrc)
{
    FLOAT fPI = 3.1415927f;
    INT nFlag;


    __asm__ volatile("qmtc2.ni %3, $vf3\n" /* MATCH: one asm statement; a separate qmtc2 asm flushes cse, so the argument address stays in a copy register (Avatar RotateAvatarMain) */
                     "vmr32.w $vf3, $vf3\n"
                     "lqc2 $vf1, 0x0(%1)\n"
                     "lqc2 $vf2, 0x0(%2)\n"
                     "vadd.xyz $vf1, $vf1, $vf2\n"
                     "vclipw.xyz $vf1, $vf3w\n"
                     "vnop\n"
                     "vnop\n"
                     "vnop\n"
                     "sqc2 $vf1, 0x0(%1)\n"
                     "vnop\n"
                     "cfc2.ni %0, $vi18\n"
                     : "=r"(nFlag) : "r"(&vOut), "r"(&vInSrc), "r"(fPI) : "memory");

    if (nFlag & 0x3f) roundPI(vOut, nFlag);
}
#line 679
inline void IVECTOR::set(INT _x, INT _y, INT _z, INT _w) { x = _x; y = _y; z = _z; w = _w; }
inline void IVECTOR::set(const IVECTOR& inV) { *this = inV; }
#line 728
inline void FVECTOR::zero() { _q = 0; }
inline void FVECTOR::init(FLOAT inW) { _q = 0; w = inW; }
inline void FVECTOR::set(FLOAT inX, FLOAT inY, FLOAT inZ, FLOAT inW) { x = inX; y = inY; z = inZ; w = inW; }
inline void FVECTOR::set(const FVECTOR& inV) { *this = inV; }
inline void FVECTOR::set(const _FVECTOR& inV) { *this = *(const FVECTOR*)inV; }
inline void FVECTOR::setAll(FLOAT f) { x = y = z = w = f; }
#line 736
inline void FVECTOR::addRot(const FVECTOR& vInSrc) { _addRot(*this, vInSrc); }
#line 746
inline BOOL FVECTOR::operator==(const FVECTOR& inV) const { return _ul[0] == inV._ul[0] && _ul[1] == inV._ul[1]; }
#line 754
inline BOOL FVECTOR::operator!=(const FVECTOR& inV) const {
    return _ul[0] != inV._ul[0] || _ul[1] != inV._ul[1];
}
#line 766
inline void FVECTOR::operator+=(const FVECTOR& inV) {



    x += inV.x; y += inV.y; z += inV.z; w += inV.w;
}
#line 775
inline void FVECTOR::operator+=(const _FVECTOR& inV) {



    x += inV[0]; y += inV[1]; z += inV[2]; w += inV[3];
}
#line 784
inline void FVECTOR::operator-=(const FVECTOR& inV) {



    x -= inV.x; y -= inV.y; z -= inV.z; w -= inV.w;
}
#line 802
inline void FVECTOR::operator*=(const FVECTOR& inV) {



    x *= inV.x; y *= inV.y; z *= inV.z; w *= inV.w;
}
#line 820
inline void FVECTOR::operator*=(FLOAT f) {



    x *= f; y *= f; z *= f; w *= f;
}
#line 830
inline FVECTOR FVECTOR::operator+(const FVECTOR& inV) const {




    FVECTOR vec = *this;
    vec += inV;
    return vec;
}
#line 850
inline FVECTOR FVECTOR::operator-(const FVECTOR& inV) const {




    FVECTOR vec = *this;
    vec -= inV;
    return vec;
}
#line 870
inline FVECTOR FVECTOR::operator*(const FVECTOR& inV) const {




    FVECTOR vec = *this;
    vec *= inV;
    return vec;
}
#line 890
inline FVECTOR FVECTOR::operator*(FLOAT f) const {




    FVECTOR vec = *this;
    vec *= f;
    return vec;
}
#line 900
inline FVECTOR operator*(FLOAT f, const FVECTOR& inV) { FVECTOR vec = inV; vec *= f; return vec; }
inline FVECTOR FVECTOR::operator-() const { return -1.0f * *this; }
#line 902
inline FVECTOR::operator _FVECTOR*() { return &v; }
inline FVECTOR::operator const _FVECTOR*() const { return &v; }
inline FVECTOR::operator _FVECTOR&() const { return *(_FVECTOR*)&v; }
#line 910
inline void FMATRIX::init() { vu0UnitMatrix(*this); }
#line 919
inline void FMATRIX::initWithScale(const FVECTOR& vScale) {
    /* MATCH: the scale diagonal as one asm on fixed registers with only input operands (outputs or C stores make
     * gcc reload the scale's owner afterwards: Entity updateCoord) */

    asm volatile("lq $5, 0x0(%0)\n"
                 "pextuw $2, $5, $0\n"
                 "pextlw $4, $0, $2\n"
                 "pextlw $2, $0, $5\n"
                 "pextuw $3, $2, $0\n"
                 "pcpyld $2, $0, $2\n"
                 "sq $2, 0x0(%1)\n"
                 "sq $3, 0x10(%1)\n"
                 "sq $4, 0x20(%1)\n"
                 "sqc2 $vf0, 0x30(%1)"
                 : : "r"(&vScale), "r"(this) : "$2", "$3", "$4", "$5");
}
#line 949
inline void FMATRIX::set(const _FMATRIX& inM) { v[0].set(inM[0]); v[1].set(inM[1]); v[2].set(inM[2]); v[3].set(inM[3]); }
#line 951
inline BOOL FMATRIX::operator==(const FMATRIX& inM) const { return v[0] == inM.v[0] && v[1] == inM.v[1] && v[2] == inM.v[2] && v[3] == inM.v[3]; }
inline BOOL FMATRIX::operator!=(const FMATRIX& inM) const { return v[0] != inM.v[0] || v[1] != inM.v[1] || v[2] != inM.v[2] || v[3] != inM.v[3]; }
inline FMATRIX::operator _FMATRIX*() { return &m; }
inline FMATRIX::operator const _FMATRIX*() const { return &m; }
inline FMATRIX::operator _FMATRIX&() const { return *(_FMATRIX*)&m; }

/* MATCH: CCoord is in System/Coord.h, not in Camera.h as the ref says: the original's line markers put its
 * constructor at Coord.h:1009-1016 and its accessors at Coord.h:1043-1072 (user decision 2026-11-10). */
class CCoord {
public:
    FMATRIX m_Local;
    // PARTIAL: the ref's fields from here are private (ours public)
    union {
        struct {
            union {
                struct {
                    TIMEQ m_FixTimestamp; /* 0x40 */
                    INT m_Flag;           /* 0x44 */
                };
                ULONGLONG _ulFixTimestamp;
            };
            TIMEQ m_WorldTimestamp; /* 0x48 */
            CCoord* m_pParent;      /* 0x4c */
        };
        u_long128 _q40; // MATCH: Game/Atel/EventTrap: the constructor clears 0x40..0x4f at once
    };
    union {
        FMATRIX m_Rot;              /* 0x50 */
        struct {
            int m_hHandlePad___[12];
            FLOAT m_fScaleX;        /* 0x80 */
            FLOAT m_fScaleY;        /* 0x84 */
            FLOAT m_fScaleZ;        /* 0x88 */
            INT m_hHandle;          /* 0x8c */
        };
    };
    FMATRIX m_World;     /* 0x90 */
    /* the inlines in the ref's order on the original's lines (line markers: the constructor 1009-1016, setLocal
     * 1024-1029, getParent 1043, setParent 1044, getScale 1046, setScale 1047, touch 1051, getWorldMatrix 1055,
     * resetTimestamp 1062, setHandle 1072) */
#line 1009
    CCoord() {
        _q40 = 0;
#line 1013
        m_fScaleX = 1.0f;
        m_fScaleY = 1.0f;
        m_fScaleZ = 1.0f;
        m_hHandle = 0;
    }
#line 1023
    void setLocal(const FMATRIX& matIn) {
        if (m_Local != matIn) {
#line 1026
            m_Local = matIn;
            touch();
        }
    }
    void updateLocal(/* parameters unknown */);
#line 1032
    void setTrans(const FVECTOR& vecIn) { m_Local.v[3] = vecIn; }
    void updateTrans(/* parameters unknown */);
#line 1043
    CCoord* getParent() { return m_pParent; }
    void setParent(CCoord* pParent) { m_pParent = pParent; }

    void getScale(FVECTOR& outScale) { outScale.x = m_fScaleX; outScale.y = m_fScaleY; outScale.z = m_fScaleZ; }
    void setScale(const FVECTOR& inScale) { m_fScaleX = inScale.x; m_fScaleY = inScale.y; m_fScaleZ = inScale.z; m_Flag = 1; }
    void fix(INT n = 0);
#line 1051
    void touch() { m_Flag = 1; }
    TIMEQ getWorldTimestamp(/* parameters unknown */);
    FMATRIX& getLocalMatrix() { return m_Local; }
#line 1055
    FMATRIX& getWorldMatrix() { return m_World; }
    FMATRIX& getWorldRot33Matrix(/* parameters unknown */);
    FVECTOR getWorldPos() { return m_World.v[3]; } // PARTIAL: Effect/effect and EffectMan (original name unknown)
    void getRotTransPersMatrix(FMATRIX& outMat);
    void getRotTransPersMatrix_VF04_07();
    void RotTransPers(IVECTOR& outV, const FVECTOR& inV);
    void RotTransPers(FVECTOR& outV, const FVECTOR& inV);
    void resetTimestamp() { m_FixTimestamp = -1; m_WorldTimestamp = -1; }
    void recalcWorldMatrix();
    void recalcLocalMatrix();
    HANDLE getHandle(/* parameters unknown */);
#line 1072
    void setHandle(HANDLE h) { m_hHandle = h; }

private:
    void _mulParent(/* parameters unknown */);
    void _checkParent(/* parameters unknown */);
};

class CCoords {
public:
    enum _PROC_PRIO {
        _PP_PRIMARY = 1,
        _PP_SECONDARY = 2,
        _PP_ALL = 3
    };

private:
    CCoord* m_pCrdArray;
    UINT m_nCrdNum;
    HANDLE m_hSysHandle;
    UINT m_iFlags;

public:
    CCoords(_PROC_PRIO pp);
    virtual ~CCoords();
    BOOL addCoord(UINT nNum, CCoord* pParent);
    void delCoord();
    HANDLE setSysFix(BOOL bSet, _PROC_PRIO pp);
    /* the inlines on the original's lines (line markers: getSysHandle 1117, getCoordNum 1120, Crd 1124-1155,
     * getLocalMatrix 1158, getWorldMatrix 1161, isSysUpdate 1176) */
#line 1117
    HANDLE getSysHandle() const { return m_hSysHandle; }


    INT getCoordNum() const { return m_nCrdNum; }



    CCoord& Crd(UINT nIndex) {
        if (m_nCrdNum && m_pCrdArray && nIndex < m_nCrdNum) {
            return m_pCrdArray[nIndex];
        }
        _coord_alart(nIndex);
        return getExceptCoord();
    }


    CCoord& Crd(UINT nIndex) const {
        if (m_nCrdNum && m_pCrdArray && nIndex < m_nCrdNum) {
            return m_pCrdArray[nIndex];
        }
        _coord_alart(nIndex);
        return getExceptCoord();
    }


    CCoord* Crd() {
        if (m_nCrdNum && m_pCrdArray)
            return m_pCrdArray;
        _coord_nullalart();
        return __null;
    }


    CCoord* Crd() const {
        if (m_nCrdNum && m_pCrdArray)
            return m_pCrdArray;
        _coord_nullalart();
        return __null;
    }


    FMATRIX& getLocalMatrix(UINT nIndex) { return Crd(nIndex).m_Local; }


    FMATRIX& getWorldMatrix(UINT nIndex) const { return Crd(nIndex).m_World; }
    void update(TIMEQ nTQ);
    void touch();
    void resetTimeStamp();

private:
    void _setCoordHandle(HANDLE h);
#line 1176
    BOOL isSysUpdate(_PROC_PRIO pp) { return m_iFlags & pp; }
    void setSysHandle(HANDLE h);
    CCoord& getExceptCoord();
    CCoord& getExceptCoord() const;
    void _coord_alart(UINT nIndex) const;
    void _coord_nullalart() const;

    friend class CCoordMan;
};

void ApplyMatrix33(FVECTOR& v0, const FMATRIX& m0, const FVECTOR& v1);
void ViewClipMatrix(FMATRIX& m, FLOAT scrw, FLOAT scrh, FLOAT scrz, FLOAT fNear, FLOAT fFar);
void ApplyMatrix(FVECTOR& v0, const FMATRIX& m0, const FVECTOR& v1);
void MulMatrix(FMATRIX& m0, const FMATRIX& m1, const FMATRIX& m2);
void OuterProduct(FVECTOR& v0, const FVECTOR& v1, const FVECTOR& v2);
FLOAT InnerProduct(const FVECTOR& v0, const FVECTOR& v1);
FLOAT InnerProduct(const FVECTOR& v0);
void Normalize(FVECTOR& v0, const FVECTOR& v1);
void DivVector(FVECTOR& v0, const FVECTOR& v1, FLOAT q);
void DivVectorXYZ(FVECTOR& v0, const FVECTOR& v1, FLOAT f);
void AddVector(FVECTOR& v0, const FVECTOR& v1, const FVECTOR& v2);
void SubVector(FVECTOR& v0, const FVECTOR& v1, const FVECTOR& v2);
void MulVector(FVECTOR& v0, const FVECTOR& v1, const FVECTOR& v2);
void ScaleVector(FVECTOR& v0, const FVECTOR& v1, FLOAT f);
void ScaleVectorXYZ(FVECTOR& v0, const FVECTOR& v1, FLOAT f);
void FTOI0Vector(IVECTOR& v0, const FVECTOR& v1);
void FTOI4Vector(IVECTOR& v0, const FVECTOR& v1);
void ITOF0Vector(FVECTOR& v0, const IVECTOR& v1);
void AddVectorXYZ(FVECTOR& v0, const FVECTOR& v1, const FVECTOR& v2);
void SubVectorXYZ(FVECTOR& v0, const FVECTOR& v1, const FVECTOR& v2);
void MulVectorXYZ(FVECTOR& v0, const FVECTOR& v1, const FVECTOR& v2);

#endif /* SYSTEM_COORD_H */
