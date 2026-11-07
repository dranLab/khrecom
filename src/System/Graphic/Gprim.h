
#ifndef SYSTEM_GRAPHIC_GPRIM_H
#define SYSTEM_GRAPHIC_GPRIM_H

#include "khtypes.h"
#include "System/MainApp.h"
#include "System/HeapMan.h"
#include "System/Vector.h" /* FVECTOR and the basic graphics types (G_XY ... G_ST) */
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
    _ALPHA_MODE_MASK = 15,
    _USE_DST_FLAG = 128,
    ALPHA_DST_BLEND = 129,
    ALPHA_DST_PLUS = 130,
    ALPHA_DST_MINUS = 131,
    _USE_VALUE_FLAG = 64,
    ALPHA_VALUE_BLEND = 65,
    ALPHA_VALUE_PLUS = 66,
    ALPHA_VALUE_MINUS = 67
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
    operator int() const { return _value; }
    BOOL operator==(_ALPHA nMode) const { return _value == nMode; }
    BOOL operator!=(_ALPHA nMode) const { return _value != nMode; }
    ALPHA& operator=(const ALPHA& a) {
        _value = a._value;
        return *this;
    }
    BYTE getAlphaMode() const { return alphaMode; }
    void setAlphaMode(/* parameters unknown */);
    BYTE getAlphaValue() const { return alphaValue; }
    void setAlphaValue(/* parameters unknown */);
};
/* moved to System/Vector.h (QWdata: khtypes.h) */
#line 137
union G_LAYER {
    INT _nInfo;
    struct {
        USHORT _nID;
        USHORT _nDepth;
    };
};

union FVERTEX {
    FVECTOR fP;
    IVECTOR iP;
    struct {
        int iPad[3];
        G_RGBA C;
    };
};

struct FTVERTEX {
    union {
        FVECTOR fT;
        IVECTOR iT;
    };
    union {
        FVECTOR fP;
        IVECTOR iP;
        struct {
            int iPad[3];
            G_RGBA C;
        };
    };
};

/* The original's CDrawable::getPacketSize/makePkt are pure (their vtable slots hold __pure_virtual). */
class CDrawable {
    friend class CDispListBase; // MATCH: System/Graphic/DispList links primitives (friends are not in the debug info)
    friend class CDispList;
    friend class CDispListSemi;
    friend class CDispListSemiBase;
    friend class CDispListSemiSingleSort;
    friend class CDispListWithoutZ;
    friend class CDispListShadowDraw;
    friend class CDispListShadowMap;
    friend struct OTtag;
    friend class CModelMan;     // MATCH: System/Graphic/ModelMan reads the layer and calls makePkt
private:
    CDrawable* m_pNext;
protected:
    FIX4 m_nZ;
    USHORT m_iFlag;
    ALPHA m_nAlphaMode;
    UINT m_nTestFlag;
    G_LAYER m_LayerInfo;
public:
    CDrawable() {}
    virtual void disp();
    virtual UINT getPacketSize() const = 0;
private:
    virtual void makePkt() = 0;
    CDrawable* getNextDrawable(/* parameters unknown */);
    void setNextDrawable(/* parameters unknown */);
public:
    void setTestFlag(UINT nFlag) { m_nTestFlag = nFlag; }
    void setLayer(USHORT nID, USHORT nDepth) {
        m_LayerInfo._nID = nID;
        m_LayerInfo._nDepth = nDepth;
    }
    void setLayerDepth(USHORT nInDepth) { m_LayerInfo._nDepth = nInDepth; }
    void setLayerFG(USHORT nDepth) { setLayer(3, nDepth); }
    void setLayerBG(/* parameters unknown */);
    void setLayerDirect(/* parameters unknown */);
    USHORT getLayerID() const { return m_LayerInfo._nID; }
    USHORT getLayerDepth() const { return m_LayerInfo._nDepth; }
    G_LAYER getLayerInfo(/* parameters unknown */);
    void setAlphaTestEnable(/* parameters unknown */);
    void setAlphaTestDefault(/* parameters unknown */);
    void setDstAlphaTestEnable(/* parameters unknown */);
    void setDstAlphaMode(/* parameters unknown */);
    void setDepthTestEnable(/* parameters unknown */);
    void setDepthTestCmp(/* parameters unknown */);
    ULONGLONG getTestFlag() const { return m_nTestFlag; }
    void setSemiTrans(ALPHA nInAlpha) { m_nAlphaMode = nInAlpha; }
    ALPHA getSemiTrans() const { return m_nAlphaMode; }
    void setZ(FIX4 nZ) { m_nZ = nZ; }
    FIX4 getZ() const { return m_nZ; }
    void setFlag(/* parameters unknown */);
    void resetFlag(/* parameters unknown */);
    USHORT getFlag(/* parameters unknown */);
    void setSemiSort(/* parameters unknown */);
    BOOL isSemiSort(/* parameters unknown */);
    void copyAttribute(/* parameters unknown */);
};

/* The types below have no #pragma interface (their inline functions are not emitted in Gprim.o); the classes are in
 * GprimFull.h. */

typedef SHORT TDEG;
class CCoord;
class CCoords;
/* moved to System/Vector.h (QWdata: khtypes.h) */
#line 245
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
