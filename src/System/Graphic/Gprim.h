// PARTIAL: added for System/Graphic/2d/2DFnt
#ifndef SYSTEM_GRAPHIC_GPRIM_H
#define SYSTEM_GRAPHIC_GPRIM_H

#include "khtypes.h"
#include "System/MainApp.h"
#include "System/HeapMan.h"
#include "System/Vector.h" // PARTIAL: added for Game/Atel/EventTrap
#include "System/Graphic/TexInfo.h"

/* __FILE__ of this header in the original differs per includer ("System/Graphic/Gprim.h" in most, also
 * "c:/KHCOM/MainApp/System/Graphic/Gprim.h" and "System/../System/Graphic/Gprim.h"): define
 * GPRIM_H_PATH before including it to pick another one. Line numbers in messages are the original ones. */
#ifndef GPRIM_H_PATH
#define GPRIM_H_PATH "System/Graphic/Gprim.h"
#endif

/* PARTIAL: ALPHA as a union with constructors and operator=, added for Game/Atel/EventTrap: passed by value it
 * keeps the value in a register in CGPrim's constructor (array of C3DPolyG4), and the user-defined operator= keeps
 * the CGPrim vtable store in C3DLines' constructor. */
enum _ALPHA {
    ALPHA_OFF = 0,
    ALPHA_BLEND = 1,
    ALPHA_PLUS = 2,
    ALPHA_MINUS = 3,
    ALPHA_MUL = 4,
};

union ALPHA {
    USHORT _value;
    struct {
        BYTE alphaMode;
        BYTE alphaValue;
    };
    ALPHA() {}
    ALPHA(_ALPHA nMode) { _value = nMode; }
    ALPHA(INT nMode) { _value = nMode; }
    operator INT() const { return _value; }
    // PARTIAL: added for System/Graphic/ScreenMask (the result is materialized: xori + bnez)
    BOOL operator==(_ALPHA nMode) const { return _value == nMode; }
    BOOL operator!=(_ALPHA nMode) const { return _value != nMode; } // PARTIAL: added for System/Graphic/2d/2DSprObj
    ALPHA& operator=(const ALPHA& a) {
        _value = a._value;
        return *this;
    }
    BYTE getAlphaMode() const { return alphaMode; }   // PARTIAL: added for System/Graphic/DispList
    BYTE getAlphaValue() const { return alphaValue; } // PARTIAL: added for System/Graphic/DispList
};

// PARTIAL: added for Game/AtWater
union QWdata {
    u_long ul64[2];
    u_long128 ul128;
    u_int ui32[4];
    u_long u64[2];
    u_long128 u128;
    u_int u32[4];
    u_short u16[8];
    long s64[2];
    int s32[4];
    short s16[8];
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

// PARTIAL: added for Game/NowLoading
struct G_UV {
    USHORT u;
    USHORT v;

    void set(USHORT _u, USHORT _v) {
        u = _u;
        v = _v;
    }
};

union G_LAYER {
    INT _nInfo;
    struct {
        USHORT _nID;
        USHORT _nDepth;
    };
};

union FVERTEX {
    struct {
        int iPad[3];
        G_RGBA C;
    };
    float f[4];
};

struct FTVERTEX {
    float fT[4];
    union {
        struct {
            int iPad[3];
            G_RGBA C;
        };
        float fP[4];
    };
};

/* The original's CDrawable::getPacketSize/makePkt are pure (their vtable slots hold __pure_virtual). */
class CDrawable {
    friend class CDispListBase; // PARTIAL: System/Graphic/DispList links primitives
    friend class CDispList;           // PARTIAL: System/Graphic/DispList
    friend class CDispListSemi;
    friend class CDispListSemiBase;
    friend class CDispListSemiSingleSort;
    friend class CDispListWithoutZ;
    friend class CDispListShadowDraw;
    friend class CDispListShadowMap;
    friend struct OTtag;
    friend class CModelMan;     // PARTIAL: System/Graphic/ModelMan reads the layer and calls makePkt
private:
    CDrawable* m_pNext;
protected:
    FIX4 m_nZ;
    USHORT m_iFlag;
    ALPHA m_nAlphaMode;
    UINT m_nTestFlag;
    G_LAYER m_LayerInfo;
public:
    // PARTIAL: added for Game/NowLoading
    CDrawable() {}
    void setTestFlag(UINT nFlag) { m_nTestFlag = nFlag; }
    void setLayer(USHORT nID, USHORT nDepth) {
        m_LayerInfo._nID = nID;
        m_LayerInfo._nDepth = nDepth;
    }
    void setLayerFG(USHORT nDepth) { setLayer(3, nDepth); }
    void setLayerDepth(USHORT nInDepth) { m_LayerInfo._nDepth = nInDepth; } // PARTIAL: added for Game/Map
    void setSemiTrans(ALPHA nInAlpha) { m_nAlphaMode = nInAlpha; }
    USHORT getLayerDepth() const { return m_LayerInfo._nDepth; } // PARTIAL: added for System/Graphic/2d/2DSprObj
    void setZ(FIX4 nZ) { m_nZ = nZ; } // PARTIAL: added for System/Graphic/Model
    USHORT getLayerID() const { return m_LayerInfo._nID; }
    ULONGLONG getTestFlag() const { return m_nTestFlag; }
    ALPHA getSemiTrans() const { return m_nAlphaMode; }
    FIX4 getZ() const { return m_nZ; }
    virtual void disp();
    virtual UINT getPacketSize() const = 0;
private:
    virtual void makePkt() = 0;
};

/* The types below have no #pragma interface (their inline functions are not emitted in Gprim.o); the classes are in
 * GprimFull.h. */

typedef SHORT TDEG;
class CCoord;
class CCoords;

struct G_ST {
    FLOAT s;
    FLOAT t;
    void set(FLOAT _s, FLOAT _t) {
        s = _s;
        t = _t;
    }
};

struct G_LINE {
    FVECTOR P0;
    FVECTOR P1;
    G_RGBA C0;
    G_RGBA C1;
    IVECTOR p0;
    IVECTOR p1;
};

struct G_STRIP {
    FVECTOR P;
    G_ST S;
    G_RGBA C;
    USHORT Term;
    USHORT Bone;
};

typedef CVector<G_LINE> G_LINE_VECTOR;
typedef CVector<G_STRIP> G_STRIP_VECTOR;

extern "C" void sceVu0ITOF4Vector(FVECTOR& v0, IVECTOR& v1);
extern "C" void sceVu0FTOI4Vector(IVECTOR& v0, FVECTOR& v1);
void ApplyMatrix(FVECTOR& v0, const FMATRIX& m0, const FVECTOR& v1);
class C2DRot {
protected:
    FMATRIX m_Matrix;
    FLOAT m_fRot;
    G_XYZ m_Center;

public:
    void setCenter(/* parameters unknown */);
    G_XYZ& getCenter(/* parameters unknown */);
    void setRot(/* parameters unknown */);
    FLOAT getRot(/* parameters unknown */);
    void calcVert(IVECTOR& iv) {
        FVECTOR v;
        sceVu0ITOF4Vector(v, iv);
        ApplyMatrix(v, m_Matrix, v);
        sceVu0FTOI4Vector(iv, v);
    }
};

#include "System/Graphic/GprimFull.h"

#endif /* SYSTEM_GRAPHIC_GPRIM_H */
