// PARTIAL: added for System/Graphic/2d/2DFnt
#ifndef SYSTEM_GRAPHIC_GPRIM_H
#define SYSTEM_GRAPHIC_GPRIM_H

#include "khtypes.h"
#include "System/MainApp.h"
#include "System/HeapMan.h"
#include "System/Vector.h" // PARTIAL: added for Game/Atel/EventTrap

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
    void set(FIX4 _x, FIX4 _y) { // PARTIAL: added for System/Wnd
        x = _x;
        y = _y;
    }
};

struct G_XYZ {
    FIX4 x;
    FIX4 y;
    FIX4 z;
    // PARTIAL: added for System/DebugProcInfo
    void set(FIX4 _x, FIX4 _y, FIX4 _z) {
        x = _x;
        y = _y;
        z = _z;
    }
    void set(FIX4 _x, FIX4 _y) {
        x = _x;
        y = _y;
    }
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
    // PARTIAL: added for Game/NowLoading (the original has constructors; a union member here can't)
    void set(UBYTE _r, UBYTE _g, UBYTE _b, UBYTE _a) {
        r = _r;
        g = _g;
        b = _b;
        a = _a;
    }
    // PARTIAL: added for System/Movie/MSVideo
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

/* The original's CDrawable::getPacketSize/makePkt are pure (their vtable slots hold __pure_virtual). Only Gprim.cpp's
 * full classes declare every override, so only its view declares them pure. */
#ifdef SYSTEM_GRAPHIC_GPRIM_CPP
#define GPRIM_DRAWABLE_PURE = 0
#else
#define GPRIM_DRAWABLE_PURE
#endif
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
    void setSemiTrans(ALPHA nInAlpha) { m_nAlphaMode = nInAlpha; }
    USHORT getLayerDepth() const { return m_LayerInfo._nDepth; } // PARTIAL: added for System/Graphic/2d/2DSprObj
    void setZ(FIX4 nZ) { m_nZ = nZ; } // PARTIAL: added for System/Graphic/Model
    USHORT getLayerID() const { return m_LayerInfo._nID; }
    ULONGLONG getTestFlag() const { return m_nTestFlag; }
    ALPHA getSemiTrans() const { return m_nAlphaMode; }
    FIX4 getZ() const { return m_nZ; }
    virtual void disp();
    virtual UINT getPacketSize() const GPRIM_DRAWABLE_PURE;
private:
    virtual void makePkt() GPRIM_DRAWABLE_PURE;
};

#ifndef SYSTEM_GRAPHIC_GPRIM_CPP /* the partial classes, as the other units see them */
class CGPrim : public CDrawable {
protected:
    UINT m_nAttribute;
    // PARTIAL: added for Game/NowLoading
    void initAttribute(UINT nABE, UINT nIIP, UINT nBackClip) {
        m_nAttribute = nABE | (nIIP << 1) | (nBackClip << 2);
    }
public:
    CGPrim() {
        m_iFlag = 0;
        m_nZ = 0;
        initAttribute(0, 1, 0);
        CDrawable::setSemiTrans(ALPHA_OFF);
        setTestFlag(0x5000D);
        setLayer(0, 0);
    }
    void setSemiTrans(ALPHA nInAlpha) {
        CDrawable::setSemiTrans(nInAlpha);
        if (nInAlpha != ALPHA_OFF) {
            m_nAttribute |= 1;
        } else {
            m_nAttribute &= ~1;
        }
    }
    // PARTIAL: added for System/Graphic/PathData (names unknown)
    void setIIP(BOOL b) {
        if (b) m_nAttribute |= 2;
        else m_nAttribute &= ~2;
    }
    void setBackClip(BOOL b) {
        if (b) m_nAttribute |= 4;
        else m_nAttribute &= ~4;
    }
    virtual ~CGPrim() {} // PARTIAL: inline (Game/AtWater: no out-of-line _$_6CGPrim exists)
    virtual void disp();
    virtual FIX4 calcZ();
};

class CGTexPrim : public CGPrim {
protected:
    struct TexInfo* m_pTexInfo;
public:
    CGTexPrim() { m_pTexInfo = 0; } // PARTIAL: added for Game/NowLoading
    // PARTIAL: added for System/Graphic/ModelMan
    static struct TexInfo* m_pTexCache;
    static INT m_nTmpTexArea;
    static void clearTexCache() { m_pTexCache = NULL; }
    void setTPage(struct TexInfo* pInInfo) { m_pTexInfo = pInInfo; }
    void setTPage(const CHAR* pTexName); // PARTIAL: added for System/Wnd
    struct TexInfo* getTexInfo() const { return m_pTexInfo; } // PARTIAL: added for System/Wnd (name unknown)
    virtual ~CGTexPrim();
    virtual UINT getPacketSize() const;
};

// PARTIAL: added for System/Graphic/ModelMan (the texture cache), class body for System/Graphic/2d/2DSprObj
class CGTexPrim2D : public CGPrim {
protected:
    struct TexInfo* m_pTexInfo;
public:
    CGTexPrim2D() { m_pTexInfo = NULL; }
    void setTPage(struct TexInfo* pInInfo) { m_pTexInfo = pInInfo; }
    struct TexInfo* getTPage() const { return m_pTexInfo; }
    void setTPage(const CHAR* pTexName); // PARTIAL: added for System/Graphic/Gprim
    virtual UINT getPacketSize() const;

    static struct TexInfo* m_pTexCache[28];
    static USHORT m_nCacheIdx;
    static void clearTexCache() {
        m_nCacheIdx = 0;
        for (INT i = 27; i >= 0; i--) {
            m_pTexCache[i] = NULL;
        }
    }
};

class CCPrim : public CGPrim {
private:
    UINT m_PRIM;
    UINT m_TEST;
    ULONGLONG m_CLAMP;
    ULONGLONG m_SCISSOR;
    UINT m_ALPHA;
public:
    INT m_iMax;
    FVERTEX* m_V;

    CCPrim() { init(); }                // PARTIAL: added for Game/Debug/DebugPathData
    virtual ~CCPrim() { remove(); }     // PARTIAL: added for Game/Debug/DebugPathData
    void create(ULONGLONG iPrim, INT iVertexNum) {
        remove();
        m_iMax = iVertexNum;
        m_PRIM = iPrim;
#line 613 GPRIM_H_PATH
        m_V = (FVERTEX*)com_malloc_d(iVertexNum * sizeof(FVERTEX), __FILE__ "(613)", (UINT)-1);
        if (!m_V) AssertApp(__FILE__, __LINE__, "m_V", 0);
    }
    void remove();
    BOOL isRect(INT i, const FVECTOR* vRect); // PARTIAL: added for System/Graphic/Gprim
    virtual UINT getPacketSize() const; // PARTIAL: added for Game/Debug/DebugPathData (the key function)
private:
    void init(); // PARTIAL: added for Game/Debug/DebugPathData
    virtual FIX4 calcZ();
    virtual void makePkt();
};

class CTPrim : public CGTexPrim {
private:
    UINT m_PRIM;
    UINT m_TEST;
    ULONGLONG m_CLAMP;
    ULONGLONG m_SCISSOR;
    UINT m_ALPHA;
public:
    INT m_iMax;
    FTVERTEX* m_V;

    void create(ULONGLONG iPrim, INT iVertexNum) {
        remove();
        m_iMax = iVertexNum;
        m_PRIM = iPrim;
#line 684
        m_V = (FTVERTEX*)com_malloc_d(iVertexNum * sizeof(FTVERTEX), __FILE__ "(684)", (UINT)-1);
        if (!m_V) AssertApp(__FILE__, __LINE__, "m_V", 0);
    }
    void remove();
    BOOL isRect(INT i, const FVECTOR* vRect); // PARTIAL: added for System/Graphic/Gprim
};

// PARTIAL: added for System/DebugProcInfo
class CTile : public CGPrim {
public:
    G_XYZ m_P0;
    G_XYZ m_P1;
    G_RGBA m_C0;

    CTile() {}
    virtual ~CTile() {}
    virtual UINT getPacketSize() const;
private:
    virtual FIX4 calcZ();
    virtual void makePkt();
};

// PARTIAL: added for Form/DebugMenu
class CPolyG4 : public CGPrim {
public:
    G_XYZ m_P0;
    G_XYZ m_P1;
    G_XYZ m_P2;
    G_XYZ m_P3;
    G_RGBA m_C0;
    G_RGBA m_C1;
    G_RGBA m_C2;
    G_RGBA m_C3;

    CPolyG4() {}
    virtual ~CPolyG4() {}
    virtual UINT getPacketSize() const;
private:
    virtual FIX4 calcZ();
    virtual void makePkt();
};

// PARTIAL: added for Game/NowLoading
class CSprt : public CGTexPrim {
public:
    G_XYZ m_P0;
    G_XYZ m_P1;
    G_RGBA m_C0;
    G_UV m_T0;
    G_UV m_T1;

    CSprt() { m_C0.set(0x80, 0x80, 0x80, 0x80); }
    virtual ~CSprt();
    void setXY(FIX4 x, FIX4 y) {
        m_P0.x = x;
        m_P0.y = y;
    }
    void setUVWH(USHORT u, USHORT v, USHORT w, USHORT h) {
        FIX4 _x = w << 4;
        FIX4 _y = h << 4;
        m_P1.x = m_P0.x + _x;
        m_P1.y = m_P0.y + _y;
        m_T0.set(u, v);
        m_T1.set(u + w, v + h);
    }
    virtual UINT getPacketSize() const;
private:
    virtual FIX4 calcZ();
    virtual void makePkt();
};

// PARTIAL: added for Game/Atel/EventTrap
class CCoord;

struct G_LINE {
    FVECTOR P0;
    FVECTOR P1;
    G_RGBA C0;
    G_RGBA C1;
    IVECTOR p0;
    IVECTOR p1;
};

typedef CVector<G_LINE> G_LINE_VECTOR;

// PARTIAL: added for Game/Atel/Avatar (the full class with its inlines is in GprimFull.h)
class C3DLine : public CGPrim {
protected:
    IVECTOR p0;
    IVECTOR p1;
public:
    FVECTOR m_P0;
    FVECTOR m_P1;
    G_RGBA m_C0;
    G_RGBA m_C1;
    CCoord* m_pCrd;

    virtual UINT getPacketSize() const;
private:
    virtual void makePkt();
public:
    virtual ~C3DLine();
private:
    virtual FIX4 calcZ();
};

class C3DLines : public CGPrim {
public:
    CCoord* m_pCrd;
    G_LINE_VECTOR m_vLINE;

    virtual UINT getPacketSize() const;
private:
    virtual FIX4 calcZ();
    virtual void makePkt();
};

class C3DPolyG4 : public CGPrim {
protected:
    IVECTOR p0;
    IVECTOR p1;
    IVECTOR p2;
    IVECTOR p3;
public:
    FVECTOR m_P0;
    FVECTOR m_P1;
    FVECTOR m_P2;
    FVECTOR m_P3;
    G_RGBA m_C0;
    G_RGBA m_C1;
    G_RGBA m_C2;
    G_RGBA m_C3;
    CCoord* m_pCrd;

    virtual UINT getPacketSize() const;
private:
    virtual FIX4 calcZ();
    virtual void makePkt();
};

// PARTIAL: added for System/Graphic/2d/2DSeqObj
class CScissoring : public CGPrim {
private:
    G_XY m_P0;
    G_XY m_P1;

public:
    CScissoring() {}
    virtual ~CScissoring();
    void setDefault();
    void setRect(G_XY& p0, G_XY& p1); // PARTIAL: added for System/Graphic/2d/2DSprObj
};

// PARTIAL: added for Form/DebugWindow
struct G_ST {
    FLOAT s;
    FLOAT t;
    void set(FLOAT _s, FLOAT _t) { // PARTIAL: added for System/Graphic/2d/2DSprObj
        s = _s;
        t = _t;
    }
};

// PARTIAL: added for System/Graphic/2d/2DSprObj
class CPolyGT4ST : public CGTexPrim2D {
public:
    G_XY m_P0;
    G_XY m_P1;
    G_XY m_P2;
    G_XY m_P3;
    G_RGBA m_C0;
    G_RGBA m_C1;
    G_RGBA m_C2;
    G_RGBA m_C3;
    G_ST m_T0;
    G_ST m_T1;
    G_ST m_T2;
    G_ST m_T3;

    CPolyGT4ST() {}
    virtual UINT getPacketSize() const;
private:
    virtual void makePkt();
};

struct G_STRIP {
    FVECTOR P;
    G_ST S;
    G_RGBA C;
    USHORT Term;
    USHORT Bone;
};

typedef CVector<G_STRIP> G_STRIP_VECTOR;

class CCoords;

class C3DStripBase : public CGPrim {
public:
    G_STRIP_VECTOR m_vVERT;
    FLOAT m_fGeta;
protected:
    FLOAT m_fGetaZ;
    CCoord* m_pCrd;
    CCoords* m_pObj;
    INT m_nType;

public:
    // PARTIAL: constructor added for System/Graphic/HermitePathCamera (the derived ones set m_nType)
    C3DStripBase() {
        m_pCrd = NULL;
        m_pObj = NULL;
        m_fGeta = 0.0f;
    }
    virtual UINT getPacketSize() const;
private:
    virtual FIX4 calcZ();
    virtual void makePkt();
    virtual QWdata* makePktSub();
};

class C3DLineStrip : public C3DStripBase {
public:
    C3DLineStrip() { m_nType = 2; }
    void setCoord(CCoord* pCrd) { m_pCrd = pCrd; }
private:
    virtual QWdata* makePktSub();
};

// PARTIAL: added for System/Graphic/HermitePathCamera
class C3DPolyStrip : public C3DStripBase {
public:
    C3DPolyStrip() { m_nType = 4; }
    void setCoord(CCoord* pCrd) { m_pCrd = pCrd; }
    CCoord* getCoord() { return m_pCrd; }
private:
    virtual QWdata* makePktSub();
};

#else /* SYSTEM_GRAPHIC_GPRIM_CPP: the full classes, for System/Graphic/Gprim itself */

/* The types below have no #pragma interface (their inline functions are not emitted in Gprim.o); the classes are in
 * GprimFull.h. */

typedef SHORT TDEG;
class CCoord;
class CCoords;

struct VTransInfo { /* PARTIAL: as in System/Graphic/modeldata.h */
    UINT _nDistPos;
    USHORT _nFormat;
    USHORT _nDistBufW;
    ULONGLONG* _pImage;
    UINT _qwImage;
    USHORT _nDistX;
    USHORT _nDistY;
    USHORT _nDistW;
    USHORT _nDistH;
    USHORT _nTw;
    USHORT _nTh;
    UINT _nTransSize;
};

struct TexInfo {
    ULONGLONG _nTPage;
    VTransInfo _TexInfo;
    VTransInfo _ClutInfo;
    TexInfo* _pExtendInfo0;
    TexInfo* _pExtendInfo1;
    TexInfo* _pExtendInfo2;
    UBYTE _nInfoType;
    UBYTE _nType;
    UBYTE _nParam;
    UBYTE _nFlag;
    float _fOfs[2];
    CHAR* _pName;
    void* _pTexAnm;
    INT _nTexSize;
    INT _nClutSize;

    BOOL isFixTex() const { return !_TexInfo._pImage || (_nFlag & 1); }
    BOOL isFixClut() const { return !_ClutInfo._pImage || (_nFlag & 2); }
    BOOL isFix() const { return isFixTex() && isFixClut(); }
    ULONGLONG getTPage() const { return _nTPage; }
    ULONGLONG getTPage(UINT nTexPos, UINT nClutPos) const {
        return _nTPage | nTexPos | ((ULONGLONG)nClutPos << 37);
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
    void calcVert(IVECTOR& iv) {
        FVECTOR v;
        sceVu0ITOF4Vector(v, iv);
        ApplyMatrix(v, m_Matrix, v);
        sceVu0FTOI4Vector(iv, v);
    }
};

#include "System/Graphic/GprimFull.h"
#endif /* SYSTEM_GRAPHIC_GPRIM_CPP */

#endif /* SYSTEM_GRAPHIC_GPRIM_H */
