// PARTIAL: FVECTOR/FMATRIX layouts shared by all units; the inline operators are added as units need them.
// The original header's inline functions leave "System/Vector.h(252)" in only a few objects' .rodata, so it
// must not get string literals (asserts) until a unit that has them reproduces them.
#ifndef SYSTEM_VECTOR_H
#define SYSTEM_VECTOR_H

#include "khtypes.h"

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

// PARTIAL: added for System/MainTaskFactory. Only __alloc/__getSize are known (out-of-class template members,
// instantiated wherever an inline function uses push_back); the other members are guesses.
#include <string.h>
extern "C" void* com_malloc_d(size_t bytes, char* msg, unsigned int ra);
extern "C" void com_free(void* mem);
extern "C" void AssertApp(char* pszFile, int nLine, char* pszMsg, char* pszExp);

/* __FILE__ as in the original (the header is found through "-I src" here). Line numbers are not known. */
/* MATCH: path override for System/TaskMan ("c:/KHCOM/MainApp/System/Vector.h") */
#ifndef VECTOR_H_PATH
#define VECTOR_H_PATH "System/Vector.h"
#endif
#line 200 VECTOR_H_PATH
template <class T>
class CVector {
private:
    T* m_pObj;
    INT m_iMax;
    INT m_iIdx;
    INT m_iStatus;

public:
    CVector() : m_pObj(NULL), m_iMax(0), m_iIdx(0), m_iStatus(0) {}
    virtual ~CVector() { __cleanup(); }
    // PARTIAL: reserve added for System/TaskMan
    void reserve(INT iNum) {
        if (m_iMax < iNum) {
            __alloc(iNum);
        }
    }
    // PARTIAL: push_back/clear as in Game/Atel/EventTrap (BOOL result and clear's __init for System/TaskMan)
    BOOL push_back(const T& x) {
        if (m_iIdx >= m_iMax) {
            __alloc(__getSize(m_iMax));
        }
        if (m_iIdx < m_iMax) {
            m_pObj[m_iIdx] = x;
            m_iIdx++;
            return TRUE;
        }
        return FALSE;
    }
    void clear() {
        __cleanup();
        __init();
    }
    INT size() const { return m_iIdx; }
    T& operator[](INT i) {
        if (!(!((m_pObj == __null)))) AssertApp(__FILE__, __LINE__, "!((m_pObj==__null))", 0);
        return m_pObj[i];
    }
    /* PARTIAL: added for Form/DebugWindow (names unknown; the original line of back()'s assert is 162) */
    T* getBuffer() { return m_pObj; }
    T& back() {
#line 162
        if (!(!((m_pObj == __null)))) AssertApp(__FILE__, __LINE__, "!((m_pObj==__null))", 0);
#line 233
        return *(m_pObj + m_iIdx - 1);
    }

    /* PARTIAL: added for System/TaskMan (lines of the asserts from the original) */
#line 150
    T* begin() {
        if (!(!((m_pObj == __null)))) AssertApp(__FILE__, __LINE__, "!((m_pObj==__null))", 0);
        return m_pObj;
    }
#line 161
    T* end() {
        if (!(!((m_pObj == __null)))) AssertApp(__FILE__, __LINE__, "!((m_pObj==__null))", 0);
        return m_pObj + m_iIdx;
    }

private:
    void __init() {
        m_pObj = NULL;
        m_iMax = 0;
        m_iIdx = 0;
        m_iStatus = 0;
    }
    void __cleanup() {
        for (INT i = 0; i < m_iIdx; i++) {
            m_pObj[i].~T();
        }
        __cleanmem();
    }
    void __cleanmem() {
        if (m_pObj != NULL) {
            /* PARTIAL: delete, not com_free (Game/Atel/EventTrap): same code, but the larger RTL makes a synthesized
             * destructor containing it too big to inline, which changes where gcc emits it */
            delete m_pObj;
        }
        m_pObj = NULL;
    }
    T* __alloc(INT iNum);
    INT __getSize(INT iNum);
};

template <class T>
T* CVector<T>::__alloc(INT iNum) {
    if (m_iStatus & 1) {
        return NULL;
    }
    if (iNum < 4) {
        iNum = 4;
    }
    T* p = (T*)com_malloc_d(iNum * sizeof(T), __FILE__ "(252)", (unsigned int)-1);
    if (p != NULL) {
        if (m_pObj != NULL) {
            if (m_iIdx > 0) {
                memcpy(p, m_pObj, sizeof(T) * m_iIdx);
                __cleanmem();
            }
        }
        m_iMax = iNum;
        m_pObj = p;
    } else {
        m_iStatus |= 1;
    }
    return p;
}

template <class T>
INT CVector<T>::__getSize(INT iNum) {
    if (iNum < 256) {
        iNum *= 2;
    } else {
        iNum += 256;
    }
    return iNum;
}

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

#endif /* SYSTEM_VECTOR_H */
