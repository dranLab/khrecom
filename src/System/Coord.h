
#ifndef SYSTEM_COORD_H
#define SYSTEM_COORD_H

#include "khtypes.h"
// PARTIAL: FVECTOR/FMATRIX layouts shared by all units; the inline operators are added as units need them.
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
    void init0() { asm volatile("sqc2 $vf0, 0x0(%0)" : : "r"(this)); }
    void init() {
        _q = 0;
        w = 1.0f;
    }
#ifdef VECTOR_H_SET_CALL /* MATCH: setCall is defined before set, so it keeps a call (ModelMan calls FVECTOR::set out of line) */
    void setCall(FLOAT inX, FLOAT inY, FLOAT inZ, FLOAT inW) { set(inX, inY, inZ, inW); }
#endif
    void set(FLOAT inX, FLOAT inY, FLOAT inZ, FLOAT inW = 1.0f) {
        x = inX;
        y = inY;
        z = inZ;
        w = inW;
    }
    void setAll(FLOAT f) { x = y = z = w = f; }
    void setAllXYZ(FLOAT f) { x = y = z = f; }
    void zero(/* parameters unknown */);
    void addRot(const FVECTOR& vInSrc) {
        FVECTOR& vOut = *this;
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
                         : "=r"(nFlag) : "r"(this), "r"(&vInSrc), "r"(fPI) : "memory");
        if (nFlag & 0x3f) roundPI(vOut, nFlag);
    }
    BOOL operator==(const FVECTOR& inV) const { return _ul[0] == inV._ul[0] && _ul[1] == inV._ul[1]; }
    BOOL operator!=(const FVECTOR& inV) const { return _ul[0] != inV._ul[0] || _ul[1] != inV._ul[1]; }
    void operator+=(const FVECTOR& in) {
        const float(&inV)[4] = in.v; /* MATCH: through the array (System/Graphic/LightUnit's betaShade) */
        x += inV[0];
        y += inV[1];
        z += inV[2];
        w += inV[3];
    }
    void operator-=(const FVECTOR& inV) {
        x -= inV.x;
        y -= inV.y;
        z -= inV.z;
        w -= inV.w;
    }
    void operator*=(FLOAT f) {
        x *= f;
        y *= f;
        z *= f;
        w *= f;
    }
    FVECTOR operator+(const FVECTOR& inV) const {
        FVECTOR vec = *this;
        vec.x += inV.x;
        vec.y += inV.y;
        vec.z += inV.z;
        vec.w += inV.w;
        return vec;
    }
    FVECTOR operator-(const FVECTOR& inV) const {
        FVECTOR vec = *this;
        vec.x -= inV.x;
        vec.y -= inV.y;
        vec.z -= inV.z;
        vec.w -= inV.w;
        return vec;
    }
    FVECTOR operator-() const {
        FVECTOR vec = *this;
        vec.x = -vec.x;
        vec.y = -vec.y;
        vec.z = -vec.z;
        vec.w = -vec.w;
        return vec;
    }
    FVECTOR operator*(FLOAT f) const {
        FVECTOR vec = *this;
        vec.x *= f;
        vec.y *= f;
        vec.z *= f;
        vec.w *= f;
        return vec;
    }
    FVECTOR operator*(const FVECTOR& inV) const {
        FVECTOR vec = *this;
        vec.x *= inV.x;
        vec.y *= inV.y;
        vec.z *= inV.z;
        vec.w *= inV.w;
        return vec;
    }
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
    void set(INT _x, INT _y, INT _z, INT _w) {
        x = _x;
        y = _y;
        z = _z;
        w = _w;
    }
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
};

typedef float _FMATRIX[4][4] __attribute__((aligned(16)));
typedef float (*_FMATRIX_ROWP)[4];
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
    void init(/* parameters unknown */);
    void initWithScale(/* parameters unknown */);
    void initC3(/* parameters unknown */);
    void initTrans(/* parameters unknown */);
    FVECTOR& getTrans() { return v[3]; }
    FVECTOR& getTransRef(/* parameters unknown */);
    void setTrans(const FVECTOR& vecIn) { v[3] = vecIn; }
    void set(/* parameters unknown */);
    BOOL operator!=(const FMATRIX& inM) const { return v[0] != inM.v[0] || v[1] != inM.v[1] || v[2] != inM.v[2] || v[3] != inM.v[3]; }
    BOOL operator==(const FMATRIX& inM) const { return v[0] == inM.v[0] && v[1] == inM.v[1] && v[2] == inM.v[2] && v[3] == inM.v[3]; }
    // PARTIAL: the ref's conversions are to _FMATRIX* / float(*)[4][4] / _FMATRIX& (Game/Bullet/Bullet needs the row pointer for sceVu0)
    operator _FMATRIX_ROWP() { return m; }
};

/* The basic graphics types: in every original that has FVECTOR (274) they come right after the vector types and
 * before CCoord, in this order (the .mdebug type order; the ref spreads them over Fnt.h, Wnd.h, 2DSeqObj.h, Gprim.h
 * and VSME.h). */
union F_UL {
    FLOAT f;
    ULONG ul;
};

struct G_XY {
    FIX4 x;
    FIX4 y;
    void set(FIX4 _x, FIX4 _y) {
        x = _x;
        y = _y;
    }
};

struct G_XYZ {
    FIX4 x;
    FIX4 y;
    FIX4 z;
    void set(FIX4 _x, FIX4 _y, FIX4 _z) {
        x = _x;
        y = _y;
        z = _z;
    }
    void set(FIX4 _x, FIX4 _y) {
        x = _x;
        y = _y;
    }
    void bufferZ(/* parameters unknown */);
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

    void set(UBYTE _r, UBYTE _g, UBYTE _b, UBYTE _a) {
        r = _r;
        g = _g;
        b = _b;
        a = _a;
    }

    void set(UBYTE _r, UBYTE _g, UBYTE _b) {
        r = _r;
        g = _g;
        b = _b;
    }
};

struct G_UV {
    USHORT u;
    USHORT v;

    void set(USHORT _u, USHORT _v) {
        u = _u;
        v = _v;
    }
};

struct G_ST {
    FLOAT s;
    FLOAT t;
    void set(FLOAT _s, FLOAT _t) {
        s = _s;
        t = _t;
    }
};

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
    void setLocal(const FMATRIX& matIn) {
        if (m_Local != matIn) {
            m_Local = matIn;
            touch();
        }
    }
    void updateLocal(/* parameters unknown */);
    void setTrans(/* parameters unknown */);
    void updateTrans(/* parameters unknown */);
    void setParent(CCoord* pParent) { m_pParent = pParent; }
    void touch(); // defined below the class
    CCoord() {
        _q40 = 0;
        m_fScaleX = 1.0f;
        m_fScaleY = 1.0f;
        m_fScaleZ = 1.0f;
        m_hHandle = 0;
    }
    void fix(INT n = 0);
    TIMEQ getWorldTimestamp(/* parameters unknown */);
    FMATRIX& getWorldMatrix() { return m_World; }
    FMATRIX& getWorldRot33Matrix(/* parameters unknown */);
    FVECTOR getWorldPos() { return m_World.v[3]; } // PARTIAL: Effect/effect and EffectMan (original name unknown)
    FMATRIX& getLocalMatrix() { return m_Local; }
    CCoord* getParent() { return m_pParent; }
    void recalcLocalMatrix();
    void getScale(FVECTOR& outScale) {
        outScale.x = m_fScaleX;
        outScale.y = m_fScaleY;
        outScale.z = m_fScaleZ;
    }
    void recalcWorldMatrix();
    void getRotTransPersMatrix(FMATRIX& outMat);
    void getRotTransPersMatrix_VF04_07();
    void RotTransPers(IVECTOR& outV, const FVECTOR& inV);
    void RotTransPers(FVECTOR& outV, const FVECTOR& inV);
    void resetTimestamp() {
        m_FixTimestamp = -1;
        m_WorldTimestamp = -1;
    }
    HANDLE getHandle(/* parameters unknown */);
    void setHandle(HANDLE h) { m_hHandle = h; }
    void setScale(const FVECTOR& inScale) {
        m_fScaleX = inScale.x;
        m_fScaleY = inScale.y;
        m_fScaleZ = inScale.z;
        m_Flag = 1;
    }

private:
    void _mulParent(/* parameters unknown */);
    void _checkParent(/* parameters unknown */);
};

inline void CCoord::touch() { m_Flag = 1; }

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
        if (m_nCrdNum && m_pCrdArray) {
            return m_pCrdArray;
        }
        _coord_nullalart();
        return __null;
    }
    CCoord* Crd() const {
        if (m_nCrdNum && m_pCrdArray) {
            return m_pCrdArray;
        }
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
