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
    virtual void disp();
    virtual UINT getPacketSize() const;
private:
    virtual void makePkt();
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

/* PARTIAL: while the unit's weak inlines are still INCLUDE_ASM, every virtual destructor is declared out of line
 * (and first): it is then the classes' key function, never defined here, so defining makePkt/calcZ/... emits
 * no vtables and no weak copies after the INCLUDE_ASM'd ones. The original's destructors are inline. */

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

class CGPrim : public CDrawable {
protected:
    UINT m_nAttribute;

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
    virtual ~CGPrim();
    void setSemiTrans(ALPHA nInAlpha) {
        CDrawable::setSemiTrans(nInAlpha);
        if (nInAlpha != ALPHA_OFF) {
            m_nAttribute |= 1;
        } else {
            m_nAttribute &= ~1;
        }
    }
    void setShading(UBYTE nIIP) {
        if (nIIP) m_nAttribute |= 2;
        else m_nAttribute &= ~2;
    }
    void setBackClip(UBYTE nBackClip) {
        if (nBackClip) m_nAttribute |= 4;
        else m_nAttribute &= ~4;
    }
    void setTexNearest(UBYTE nNearest) {
        if (nNearest) m_nAttribute |= 8;
        else m_nAttribute &= ~8;
    }
    INT isSemiTrans() { return m_nAttribute & 1; }
    INT isShading() { return ((INT)m_nAttribute >> 1) & 1; }
    INT isBackClip() { return ((INT)m_nAttribute >> 2) & 1; }
    INT isTexNearest() { return ((INT)m_nAttribute >> 3) & 1; }
    virtual void disp();
    virtual FIX4 calcZ() { return 0; }
};

class CGTexPrim : public CGPrim {
protected:
    TexInfo* m_pTexInfo;
    static TexInfo* m_pTexCache;
    static INT m_nTmpTexArea;

public:
    virtual ~CGTexPrim();
    CGTexPrim() { m_pTexInfo = NULL; }
    void setTPage(TexInfo* pInInfo) { m_pTexInfo = pInInfo; }
    void setTPage(const CHAR* pTexName);
    TexInfo* getTPage() const { return m_pTexInfo; }
    virtual UINT getPacketSize() const;
protected:
    ULONGLONG setTexture() { return setTexture(m_pTexInfo, FALSE); }
    ULONGLONG setTexture(const TexInfo* pInInfo, BOOL bOnly2D);
public:
    static void clearTexCache() { m_pTexCache = NULL; }
    static BOOL updateTexCache(const TexInfo* pInInfo) {
        if (pInInfo == m_pTexCache) return FALSE;
        m_pTexCache = (TexInfo*)pInInfo;
        return TRUE;
    }
    static void setTmpTexAreaForAllPrims(INT nArea) { m_nTmpTexArea = nArea; }
};

class CGTexPrim2D : public CGPrim {
protected:
    TexInfo* m_pTexInfo;
    static TexInfo* m_pTexCache[28];
    static USHORT m_nCacheIdx;
    static USHORT m_nCacheIdxOld;

public:
    virtual ~CGTexPrim2D();
    CGTexPrim2D() { m_pTexInfo = NULL; }
    void setTPage(TexInfo* pInInfo) { m_pTexInfo = pInInfo; }
    void setTPage(const CHAR* pTexName);
    TexInfo* getTPage() const { return m_pTexInfo; }
    virtual UINT getPacketSize() const;
protected:
    ULONGLONG setTexture() { return setTexture(m_pTexInfo, FALSE); }
    ULONGLONG setTexture(const TexInfo* pInInfo, BOOL bOnly2D);
public:
    static void clearTexCache() {
        m_nCacheIdx = 0;
        for (INT i = 27; i >= 0; i--) {
            m_pTexCache[i] = NULL;
        }
    }
    static INT updateTexCache(const TexInfo* pInInfo) {
        INT size;
        for (INT i = 0; i < 28; i++) {
            if (pInInfo == m_pTexCache[i]) return i;
        }
        size = pInInfo->_TexInfo._nTransSize >> 6;
        if (size == 0) size = 1;
        m_nCacheIdxOld = m_nCacheIdx;
        m_nCacheIdx += size;
        if (m_nCacheIdx > 28) {
            m_nCacheIdx = size;
            m_nCacheIdxOld = 0;
        }
        m_pTexCache[m_nCacheIdxOld] = (TexInfo*)pInInfo;
        for (INT i = m_nCacheIdxOld + 1; i < m_nCacheIdx; i++) {
            m_pTexCache[i] = (TexInfo*)-1;
        }
        return -1;
    }
};

class CScissoring : public CGPrim {
public:
    G_XY m_P0;
    G_XY m_P1;

    CScissoring() {}
    virtual ~CScissoring();
    void setRect(G_XY& scissoringStart, G_XY& scissoringEnd);
    void setDefault();
    virtual UINT getPacketSize() const { return 3; }
private:
    virtual void makePkt();
};

class CZFill : public CGPrim {
public:
    INT m_nZ;

    virtual ~CZFill();
    CZFill();
    virtual UINT getPacketSize() const;
private:
    virtual void makePkt();
};

class CTile : public CGPrim {
public:
    G_XYZ m_P0;
    G_XYZ m_P1;
    G_RGBA m_C0;

    CTile() {}
    virtual ~CTile();
    virtual UINT getPacketSize() const;
private:
    virtual FIX4 calcZ();
    virtual void makePkt();
};

class CSprt : public CGTexPrim {
public:
    G_XYZ m_P0;
    G_XYZ m_P1;
    G_RGBA m_C0;
    G_UV m_T0;
    G_UV m_T1;

    virtual ~CSprt();
    CSprt() { m_C0.set(0x80, 0x80, 0x80, 0x80); }
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

class CPolyGT4 : public CGTexPrim {
public:
    G_XYZ m_P0;
    G_XYZ m_P1;
    G_XYZ m_P2;
    G_XYZ m_P3;
    G_RGBA m_C0;
    G_RGBA m_C1;
    G_RGBA m_C2;
    G_RGBA m_C3;
    G_UV m_T0;
    G_UV m_T1;
    G_UV m_T2;
    G_UV m_T3;

    virtual ~CPolyGT4();
    CPolyGT4();
    virtual UINT getPacketSize() const;
private:
    virtual FIX4 calcZ();
    virtual void makePkt();
};

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
    virtual ~CPolyGT4ST();
    virtual UINT getPacketSize() const;
private:
    virtual void makePkt();
};

class CLineF2 : public CGPrim {
public:
    G_XYZ m_P0;
    G_XYZ m_P1;
    G_RGBA m_C0;

    CLineF2() {}
    virtual ~CLineF2();
    virtual UINT getPacketSize() const;
private:
    virtual FIX4 calcZ();
    virtual void makePkt();
};

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
    virtual ~CPolyG4();
    virtual UINT getPacketSize() const;
private:
    virtual FIX4 calcZ();
    virtual void makePkt();
};

class CCircle : public CGPrim {
public:
    G_XYZ m_P0;
    G_RGBA m_C0;
    G_RGBA m_C1;
    FIX4 m_RX;
    FIX4 m_RY;

    CCircle() {}
    virtual ~CCircle();
    virtual UINT getPacketSize() const;
protected:
    virtual FIX4 calcZ();
    virtual void makePkt();
};

class CScreenColoredMask : public CTile {
public:
    CScreenColoredMask();
    virtual ~CScreenColoredMask();
};

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

class CRotSprt : public CSprt, public C2DRot {
public:
    CRotSprt() {}
    virtual ~CRotSprt();
private:
    virtual FIX4 calcZ();
    virtual void makePkt();
public:
    virtual UINT getPacketSize() const;
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

    CCPrim();
    virtual ~CCPrim();
    void create(ULONGLONG iPrim, INT iVertexNum) {
        remove();
        m_iMax = iVertexNum;
        m_PRIM = iPrim;
#line 613 GPRIM_H_PATH
        m_V = (FVERTEX*)com_malloc_d(iVertexNum * sizeof(FVERTEX), __FILE__ "(613)", (UINT)-1);
        if (!m_V) AssertApp(__FILE__, __LINE__, "m_V", 0);
    }
    void remove();
    void setTEST(ULONGLONG TEST);
    void setALPHA(ULONGLONG ALPHA);
    void setCLAMP(ULONGLONG CLAMP);
    void setSCISSOR(ULONGLONG SCISSOR);
    BOOL isRect(INT i, const FVECTOR* vRect);
    virtual UINT getPacketSize() const;
private:
    void init();
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

    CTPrim();
    virtual ~CTPrim();
    void create(ULONGLONG iPrim, INT iVertexNum) {
        remove();
        m_iMax = iVertexNum;
        m_PRIM = iPrim;
#line 684 GPRIM_H_PATH
        m_V = (FTVERTEX*)com_malloc_d(iVertexNum * sizeof(FTVERTEX), __FILE__ "(684)", (UINT)-1);
        if (!m_V) AssertApp(__FILE__, __LINE__, "m_V", 0);
    }
    void remove();
    void setTEST(ULONGLONG TEST);
    void setALPHA(ULONGLONG ALPHA);
    void setCLAMP(ULONGLONG CLAMP);
    void setSCISSOR(ULONGLONG SCISSOR);
    BOOL isRect(INT i, const FVECTOR* vRect);
    virtual UINT getPacketSize() const;
private:
    void init();
    virtual FIX4 calcZ();
    virtual void makePkt();
};

class C3DTile : public CGPrim {
protected:
    IVECTOR p0;
    IVECTOR p1;
public:
    FVECTOR m_Center;
    FVECTOR m_P0;
    FVECTOR m_P1;
    G_RGBA m_C0;
    CCoord* m_pCrd;

    virtual ~C3DTile();
    C3DTile();
    virtual UINT getPacketSize() const;
private:
    virtual FIX4 calcZ();
    virtual void makePkt();
};

class C3DSprt : public CGTexPrim {
protected:
    IVECTOR p0;
    IVECTOR p1;
public:
    FVECTOR m_Center;
    FVECTOR m_P0;
    FVECTOR m_P1;
    G_RGBA m_C0;
    G_UV m_T0;
    G_UV m_T1;
    CCoord* m_pCrd;

    virtual ~C3DSprt();
    C3DSprt();
    void setUVWH(USHORT u, USHORT v, USHORT w, USHORT h);
    virtual UINT getPacketSize() const;
private:
    virtual FIX4 calcZ();
    virtual void makePkt();
};

class C3DRotSprt : public C3DSprt {
protected:
    IVECTOR p10;
    IVECTOR p01;
public:
    TDEG m_Rot;

    virtual ~C3DRotSprt();
    C3DRotSprt();
    virtual UINT getPacketSize() const;
private:
    virtual FIX4 calcZ();
    virtual void makePkt();
};

class C3DPolyFT4 : public CGTexPrim {
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
    G_ST m_S0;
    G_ST m_S1;
    G_ST m_S2;
    G_ST m_S3;
    G_RGBA m_C0;
    CCoord* m_pCrd;

    virtual ~C3DPolyFT4();
    C3DPolyFT4();
    virtual UINT getPacketSize() const;
private:
    virtual FIX4 calcZ();
    virtual void makePkt();
};

class C3DPolyGT4 : public CGTexPrim {
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
    G_ST m_S0;
    G_ST m_S1;
    G_ST m_S2;
    G_ST m_S3;
    G_RGBA m_C0;
    G_RGBA m_C1;
    G_RGBA m_C2;
    G_RGBA m_C3;
    CCoord* m_pCrd;

    virtual ~C3DPolyGT4();
    C3DPolyGT4();
    virtual UINT getPacketSize() const;
private:
    virtual FIX4 calcZ();
    virtual void makePkt();
};

class C3DPolyFT3 : public CGTexPrim {
protected:
    IVECTOR p0;
    IVECTOR p1;
    IVECTOR p2;
public:
    FVECTOR m_P0;
    FVECTOR m_P1;
    FVECTOR m_P2;
    G_ST m_S0;
    G_ST m_S1;
    G_ST m_S2;
    G_RGBA m_C0;
    CCoord* m_pCrd;

    virtual ~C3DPolyFT3();
    C3DPolyFT3();
    virtual UINT getPacketSize() const;
private:
    virtual FIX4 calcZ();
    virtual void makePkt();
};

class C3DPolyGT3 : public CGTexPrim {
protected:
    IVECTOR p0;
    IVECTOR p1;
    IVECTOR p2;
public:
    FVECTOR m_P0;
    FVECTOR m_P1;
    FVECTOR m_P2;
    G_ST m_S0;
    G_ST m_S1;
    G_ST m_S2;
    G_RGBA m_C0;
    G_RGBA m_C1;
    G_RGBA m_C2;
    CCoord* m_pCrd;

    virtual ~C3DPolyGT3();
    C3DPolyGT3();
    virtual UINT getPacketSize() const;
private:
    virtual FIX4 calcZ();
    virtual void makePkt();
};

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

    C3DLine() {}
    virtual ~C3DLine();
    virtual UINT getPacketSize() const;
private:
    virtual FIX4 calcZ();
    virtual void makePkt();
};

class C3DLineCoord2 : public CGPrim {
protected:
    IVECTOR p0;
    IVECTOR p1;
public:
    FVECTOR m_P0;
    FVECTOR m_P1;
    G_RGBA m_C0;
    G_RGBA m_C1;
    CCoord* m_pCrd0;
    CCoord* m_pCrd1;

    C3DLineCoord2() {}
    virtual ~C3DLineCoord2();
    virtual UINT getPacketSize() const;
private:
    virtual FIX4 calcZ();
    virtual void makePkt();
};

class C3DLines : public CGPrim {
public:
    CCoord* m_pCrd;
    G_LINE_VECTOR m_vLINE;

    C3DLines() {}
    virtual ~C3DLines();
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

    C3DPolyG4() {}
    virtual ~C3DPolyG4();
    virtual UINT getPacketSize() const;
private:
    virtual FIX4 calcZ();
    virtual void makePkt();
};

class C3DPolyG3 : public CGPrim {
protected:
    IVECTOR p0;
    IVECTOR p1;
    IVECTOR p2;
public:
    FVECTOR m_P0;
    FVECTOR m_P1;
    FVECTOR m_P2;
    G_RGBA m_C0;
    G_RGBA m_C1;
    G_RGBA m_C2;
    CCoord* m_pCrd;

    C3DPolyG3() {}
    virtual ~C3DPolyG3();
    virtual UINT getPacketSize() const;
private:
    virtual FIX4 calcZ();
    virtual void makePkt();
};

class C3DCircle : public CGPrim {
    friend class C3DPointLight2;
protected:
    IVECTOR p0;
    IVECTOR p1;
public:
    FVECTOR m_Center;
    CCoord* m_pCrd;
    FLOAT m_R0;
    FLOAT m_Zoffset;
    G_RGBA m_C0;
    G_RGBA m_C1;

    virtual ~C3DCircle();
    C3DCircle();
    virtual UINT getPacketSize() const;
protected:
    virtual FIX4 calcZ();
    virtual void makePkt();
};

class C3DPointLight : public C3DCircle {
public:
    FLOAT m_R00;
    INT m_Active;

    C3DPointLight() {}
    virtual ~C3DPointLight();
    virtual UINT getPacketSize() const;
private:
    virtual void makePkt();
};

class C3DPointLight2 : public CGPrim {
protected:
    IVECTOR p00[2];
    IVECTOR p01[2];
public:
    C3DCircle m_Cl[2];
    FLOAT m_R00;
    INT m_Active;
    INT m_nType;

    C3DPointLight2() {}
    virtual ~C3DPointLight2();
    virtual UINT getPacketSize() const;
private:
    virtual FIX4 calcZ();
    virtual void makePkt();
    BOOL _makePktSub(INT nIdx);
};

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
    virtual ~C3DStripBase();
    C3DStripBase() {
        m_pCrd = NULL;
        m_pObj = NULL;
        m_fGeta = 0.0f;
    }
    virtual UINT getPacketSize() const;
private:
    virtual FIX4 calcZ();
    virtual void makePkt();
    virtual QWdata* makePktSub(QWdata* pQw) = 0;
};

class C3DTexStripBase : public CGTexPrim {
public:
    G_STRIP_VECTOR m_vVERT;
    FLOAT m_fGeta;
protected:
    FLOAT m_fGetaZ;
    CCoord* m_pCrd;
    CCoords* m_pObj;
    INT m_nType;

public:
    virtual ~C3DTexStripBase();
    C3DTexStripBase();
    virtual UINT getPacketSize() const;
private:
    virtual FIX4 calcZ();
    virtual void makePkt();
    virtual QWdata* makePktSub(QWdata* pQw) = 0;
};

#define GPRIM_STRIP_CLASS(NAME, BASE, TYPE, SET, GET, PTR)                                                         \
    class NAME : public BASE {                                                                                     \
    public:                                                                                                        \
        virtual ~NAME();                                                                                           \
        NAME() { m_nType = TYPE; }                                                                                 \
        void SET(PTR* p);                                                                                          \
        PTR* GET();                                                                                                \
    private:                                                                                                       \
        virtual QWdata* makePktSub(QWdata* pQw);                                                                   \
    }
GPRIM_STRIP_CLASS(C3DLineStrip, C3DStripBase, 2, setCoord, getCoord, CCoord);
GPRIM_STRIP_CLASS(C3DLineStripCoord, C3DStripBase, 2, setObject, getObject, CCoords);
GPRIM_STRIP_CLASS(C3DLineTStrip, C3DTexStripBase, 2, setCoord, getCoord, CCoord);
GPRIM_STRIP_CLASS(C3DLineTStripCoord, C3DTexStripBase, 2, setObject, getObject, CCoords);
GPRIM_STRIP_CLASS(C3DPolyStrip, C3DStripBase, 4, setCoord, getCoord, CCoord);
GPRIM_STRIP_CLASS(C3DPolyStripCoord, C3DStripBase, 4, setObject, getObject, CCoords);
GPRIM_STRIP_CLASS(C3DPolyTStrip, C3DTexStripBase, 4, setCoord, getCoord, CCoord);
GPRIM_STRIP_CLASS(C3DPolyTStripCoord, C3DTexStripBase, 4, setObject, getObject, CCoords);
GPRIM_STRIP_CLASS(C3DPolyFan, C3DStripBase, 5, setCoord, getCoord, CCoord);
GPRIM_STRIP_CLASS(C3DPolyFanCoord, C3DStripBase, 5, setObject, getObject, CCoords);
GPRIM_STRIP_CLASS(C3DPolyTFan, C3DTexStripBase, 5, setCoord, getCoord, CCoord);
GPRIM_STRIP_CLASS(C3DPolyTFanCoord, C3DTexStripBase, 5, setObject, getObject, CCoords);
#undef GPRIM_STRIP_CLASS

class C3DCPrim : public CGPrim {
private:
    static unsigned char m_PackReglist[8];
    UINT m_PRIM;
    UINT m_TEST;
    ULONGLONG m_CLAMP;
    ULONGLONG m_SCISSOR;
    UINT m_ALPHA;
    UINT m_ZBUF;
    CCoord* m_pCrd;
    CCoords* m_pObj;
    FLOAT m_fGetaZ;
    INT m_iZ;
public:
    FLOAT m_fGeta;
    USHORT m_iRep;
    USHORT m_iMax;
    G_STRIP* m_V;

    C3DCPrim();
    virtual ~C3DCPrim();
    void create(ULONGLONG iPrim, INT iVertexNum, CCoord* pCParent, INT iRep);
    void create(ULONGLONG iPrim, INT iVertexNum, CCoords* pOParent, INT iRep);
    void create(ULONGLONG iPrim, INT iVertexNum);
    void remove() {
        if (m_V) com_free(m_V);
        m_V = NULL;
        _init();
    }
    void setTEST(ULONGLONG TEST);
    void setALPHA(ULONGLONG ALPHA);
    void setCLAMP(ULONGLONG CLAMP);
    void setSCISSOR(ULONGLONG SCISSOR);
    void setZBUF(ULONGLONG ZBUF);
    CCoord* getCoordPtr() const;
    CCoords* getObjPtr() const;
    void setCalcZ(INT iZ);
    virtual UINT getPacketSize() const;
private:
    void _init();
    void _create(ULONGLONG iPrim, INT iVertexNum, CCoord* pCParent, CCoords* pOParent, INT iRep);
    virtual FIX4 calcZ();
    virtual void makePkt();
    QWdata* makePkt_None(QWdata* pQw);
    QWdata* makePkt_Line0(QWdata* pQw);
    QWdata* makePkt_LineStrip0(QWdata* pQw);
    QWdata* makePkt_Triangle0(QWdata* pQw);
    QWdata* makePkt_TriangleStrip0(QWdata* pQw);
    QWdata* makePkt_TriangleFan0(QWdata* pQw);
    QWdata* makePkt_Line1(QWdata* pQw);
    QWdata* makePkt_LineStrip1(QWdata* pQw);
    QWdata* makePkt_Triangle1(QWdata* pQw);
    QWdata* makePkt_TriangleStrip1(QWdata* pQw);
    QWdata* makePkt_TriangleFan1(QWdata* pQw);
};

class C3DTPrim : public CGTexPrim {
private:
    static unsigned char m_PackReglist[8];
    UINT m_PRIM;
    UINT m_TEST;
    ULONGLONG m_CLAMP;
    ULONGLONG m_SCISSOR;
    UINT m_ALPHA;
    UINT m_ZBUF;
    CCoord* m_pCrd;
    CCoords* m_pObj;
    FLOAT m_fGetaZ;
    INT m_iZ;
public:
    FLOAT m_fGeta;
    USHORT m_iRep;
    USHORT m_iMax;
    G_STRIP* m_V;

    C3DTPrim();
    virtual ~C3DTPrim();
    void create(ULONGLONG iPrim, INT iVertexNum, CCoord* pCParent, INT iRep);
    void create(ULONGLONG iPrim, INT iVertexNum, CCoords* pOParent, INT iRep);
    void create(ULONGLONG iPrim, INT iVertexNum);
    void remove() {
        if (m_V) com_free(m_V);
        m_V = NULL;
        _init();
    }
    void setTEST(ULONGLONG TEST);
    void setALPHA(ULONGLONG ALPHA);
    void setCLAMP(ULONGLONG CLAMP);
    void setSCISSOR(ULONGLONG SCISSOR);
    void setZBUF(ULONGLONG ZBUF);
    CCoord* getCoordPtr() const;
    CCoords* getObjPtr() const;
    void setCalcZ(INT iZ);
    virtual UINT getPacketSize() const;
private:
    void _init();
    void _create(ULONGLONG iPrim, INT iVertexNum, CCoord* pCParent, CCoords* pOParent, INT iRep);
    virtual FIX4 calcZ();
    virtual void makePkt();
    QWdata* makePkt_None(QWdata* pQw);
    QWdata* makePkt_Line0(QWdata* pQw);
    QWdata* makePkt_LineStrip0(QWdata* pQw);
    QWdata* makePkt_Triangle0(QWdata* pQw);
    QWdata* makePkt_TriangleStrip0(QWdata* pQw);
    QWdata* makePkt_TriangleFan0(QWdata* pQw);
    QWdata* makePkt_Line1(QWdata* pQw);
    QWdata* makePkt_LineStrip1(QWdata* pQw);
    QWdata* makePkt_Triangle1(QWdata* pQw);
    QWdata* makePkt_TriangleStrip1(QWdata* pQw);
    QWdata* makePkt_TriangleFan1(QWdata* pQw);
};
#endif /* SYSTEM_GRAPHIC_GPRIM_CPP */

#endif /* SYSTEM_GRAPHIC_GPRIM_H */
