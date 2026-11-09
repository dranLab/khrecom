
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
union G_LAYER {
    INT _nInfo;
    struct {
        USHORT _nID;
        USHORT _nDepth;
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

typedef SHORT TDEG;
class CCoord;
class CCoords;
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

/* The classes of System/Graphic/Gprim. Their vtables and out-of-line inline functions come out in Gprim.o, the
 * object with their first non-inline virtual functions. From here on the line numbers are the original's (its
 * .mdebug: CGPrim::initAttribute at 76, CCPrim::create's allocation at 613, CTPrim's at 684, C3DTPrim's
 * constructor at 1906); the blank runs keep them. The types above (ALPHA, CDrawable, G_LINE, ...) sit elsewhere
 * in the original (the ref has CDrawable in DispList.h; no code of Gprim.h is below line 76), hence the #line. */
#line 72
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
    virtual ~CGPrim() {}
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
    CGTexPrim() { m_pTexInfo = NULL; }
    void setTPage(const TexInfo* pInInfo) { m_pTexInfo = (TexInfo*)pInInfo; }
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
    CGTexPrim2D() { m_pTexInfo = NULL; }
    void setTPage(const TexInfo* pInInfo) { m_pTexInfo = (TexInfo*)pInInfo; }
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

    void setRect(G_XY& scissoringStart, G_XY& scissoringEnd);
    void setDefault();
    virtual UINT getPacketSize() const { return 3; }
private:
    virtual void makePkt();
};








































class CZFill : public CGPrim {
public:
    INT m_nZ;

    CZFill() { m_nZ = 0; }
    virtual UINT getPacketSize() const { return 32; }
private:
    virtual void makePkt();
};









class CTile : public CGPrim {
public:
    G_XYZ m_P0;
    G_XYZ m_P1;
    G_RGBA m_C0;

    virtual UINT getPacketSize() const { return 3; }
private:
    virtual FIX4 calcZ() { return m_P0.z; }
    virtual void makePkt();
};











class CSprt : public CGTexPrim {
public:
    G_XYZ m_P0;
    G_XYZ m_P1;
    G_RGBA m_C0;
    G_UV m_T0;
    G_UV m_T1;

    CSprt() { m_C0.set(0x80, 0x80, 0x80, 0x80); }
    void setUVWH(USHORT u, USHORT v, USHORT w, USHORT h) {
        FIX4 _x = w << 4;
        FIX4 _y = h << 4;
        m_P1.x = m_P0.x + _x;
        m_P1.y = m_P0.y + _y;
        m_T0.set(u, v);
        m_T1.set(u + w, v + h);
    }
    virtual UINT getPacketSize() const { return CGTexPrim::getPacketSize() + 5; }
private:
    virtual FIX4 calcZ() { return m_P0.z; }
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

    CPolyGT4() {
        m_C0.set(0x80, 0x80, 0x80, 0x80);
        m_C3 = m_C2 = m_C1 = m_C0;
    }
    virtual UINT getPacketSize() const { return CGTexPrim::getPacketSize() + 8; }
private:
    virtual FIX4 calcZ() { return (UINT)(m_P0.z + m_P1.z + m_P2.z + m_P3.z) >> 2; }
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

    virtual UINT getPacketSize() const { return CGTexPrim2D::getPacketSize() + 12; }
private:
    virtual void makePkt();
};









class CLineF2 : public CGPrim {
public:
    G_XYZ m_P0;
    G_XYZ m_P1;
    G_RGBA m_C0;

    virtual UINT getPacketSize() const { return 3; }
private:
    virtual FIX4 calcZ() { return (UINT)(m_P0.z + m_P1.z) >> 1; }
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

    virtual UINT getPacketSize() const { return 6; }
private:
    virtual FIX4 calcZ() { return (UINT)(m_P0.z + m_P1.z + m_P2.z + m_P3.z) >> 2; }
    virtual void makePkt();
};








class CCircle : public CGPrim {
public:
    G_XYZ m_P0;
    G_RGBA m_C0;
    G_RGBA m_C1;
    FIX4 m_RX;
    FIX4 m_RY;

    virtual UINT getPacketSize() const { return 104; }
protected:
    virtual FIX4 calcZ() { return m_P0.z; }
    virtual void makePkt();
};






class CScreenColoredMask : public CTile {
public:
    CScreenColoredMask();
};














































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









class CRotSprt : public CSprt, public C2DRot {
public:
private:
    virtual FIX4 calcZ() { return m_Center.z; }
    virtual void makePkt();
public:
    virtual UINT getPacketSize() const { return CGTexPrim::getPacketSize() + 7; }
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

    CCPrim() { init(); }
    virtual ~CCPrim() { remove(); }
    void create(ULONGLONG iPrim, INT iVertexNum) {
        remove();
        m_iMax = iVertexNum;
        m_PRIM = iPrim;
#line 613 GPRIM_H_PATH
        m_V = (FVERTEX*)com_malloc_d(iVertexNum * sizeof(FVERTEX), __FILE__ "(613)", (UINT)-1);
        if (!m_V) AssertApp(__FILE__, __LINE__, "m_V", 0);
    }

    void remove() {
        if (m_V) com_free(m_V);
        m_V = NULL;
        init();
    }
    void setTEST(ULONGLONG TEST) { m_TEST = TEST; }
    void setALPHA(ULONGLONG ALPHA) { m_ALPHA = (ALPHA >> 16) | ALPHA; }
    void setCLAMP(ULONGLONG CLAMP) { m_CLAMP = CLAMP; }
    void setSCISSOR(ULONGLONG SCISSOR) { m_SCISSOR = SCISSOR; }
    BOOL isRect(INT i, const FVECTOR* vRect);
    virtual UINT getPacketSize() const { return (UINT)(m_iMax * 16 + 0xD0) >> 4; }
private:
    inline void init();
    inline virtual FIX4 calcZ();
    virtual void makePkt();
};
inline void CCPrim::init() {
    m_V = NULL;
    m_iMax = 0;
    m_PRIM = 0;
    m_TEST = (UINT)-1;
    m_ALPHA = (UINT)-1;
    m_CLAMP = (ULONGLONG)-1;
    m_SCISSOR = (ULONGLONG)-1;
}
inline FIX4 CCPrim::calcZ() {
    if (m_V && m_V->fP.z >= 0.0f) return (FIX4)m_V->fP.z << 4;
    return 0;
}




















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

    CTPrim() { init(); }
    virtual ~CTPrim() { remove(); }
    void create(ULONGLONG iPrim, INT iVertexNum) {
        remove();
        m_iMax = iVertexNum;
        m_PRIM = iPrim;
#line 684 GPRIM_H_PATH
        m_V = (FTVERTEX*)com_malloc_d(iVertexNum * sizeof(FTVERTEX), __FILE__ "(684)", (UINT)-1);
        if (!m_V) AssertApp(__FILE__, __LINE__, "m_V", 0);
    }

    void remove() {
        if (m_V) com_free(m_V);
        m_V = NULL;
        init();
    }
    void setTEST(ULONGLONG TEST) { m_TEST = TEST; }
    void setALPHA(ULONGLONG ALPHA) { m_ALPHA = (ALPHA >> 16) | ALPHA; }
    void setCLAMP(ULONGLONG CLAMP) { m_CLAMP = CLAMP; }
    void setSCISSOR(ULONGLONG SCISSOR) { m_SCISSOR = SCISSOR; }
    BOOL isRect(INT i, const FVECTOR* vRect);
    virtual UINT getPacketSize() const {
        UINT size = (UINT)(((m_iMax * 24 + 0x3F) & ~0xF) + 0xA0) >> 4;
        return size + CGTexPrim::getPacketSize();
    }
private:
    inline void init();
    inline virtual FIX4 calcZ();
    virtual void makePkt();
};
inline void CTPrim::init() {
    m_V = NULL;
    m_iMax = 0;
    m_PRIM = 0;
    m_TEST = (UINT)-1;
    m_ALPHA = (UINT)-1;
    m_CLAMP = (ULONGLONG)-1;
    m_SCISSOR = (ULONGLONG)-1;
}
inline FIX4 CTPrim::calcZ() {
    if (m_V && m_V->fP.z >= 0.0f) return (FIX4)m_V->fP.z << 4;
    return 0;
}




















































/* The original's colour inlines at 774-782 and 792-800 (.mdebug: the colour word loaded at 774, pextlb/pextlh at 780;
 * the qword loaded at 792, ppach/ppacb at 798; ref locals iCol/ivCol as here). Their names are ours. */
inline u_long128 _colToVec(UINT iCol) {
    u_long128 ivCol;




    __asm__ volatile("pextlb %0, $0, %1\n\tpextlh %0, $0, %0" : "=r"(ivCol) : "r"(iCol));
    return ivCol;
}









inline UINT _vecToCol(u_long128 iCol) {
    UINT ivCol;




    __asm__ volatile("ppach %0, $0, %1\n\tppacb %0, $0, %0" : "=r"(ivCol) : "r"(iCol));
    return ivCol;
}




































































/* The original's inline at 871-881 (.mdebug: the u_long128 inIVec loaded at 871, pexcw/pexch/dsra32/pextlw into
 * ULONGLONG outVec64 at 879, $t0 as scratch): x, y, z of an IVECTOR to a GS XYZ word. Its name is ours. */
inline ULONGLONG _vecToXYZ(u_long128 inIVec) {
    ULONGLONG outVec64;
    __asm__ volatile("pexcw %0, %1\n"
                     "pexch $8, %1\n"
                     "dsra32 %0, %0, 4\n"
                     "pextlw %0, %0, $8\n"
                     : "=&r"(outVec64)
                     : "r"(inIVec)
                     : "$8");
    return outVec64;
}


































/* The original's inline at 920 (.mdebug: lui 1.0f and two pextlw, all at 920): the qword (1, 1, 1, 1); name ours.
 * MATCH: the lui in the asm (a constant operand is scheduled to the top of CModel's constructor) */
inline u_long128 _oneVector() {
    u_long128 out128;
    __asm__ volatile("lui %0, 0x3f80\n\tpextlw %0, %0, %0\n\tpextlw %0, %0, %0" : "=r"(out128));
    return out128;
}
































/* The original's inline at 957-966 (.mdebug: INT x, y, z, w set up at 957, locals tmp and out128, the three pextlw
 * at 964): the qword (x, y, z, w). Its name is ours. */
inline u_long128 _packWord(INT x, INT y, INT z, INT w) {
    u_long128 out128;
    u_long128 tmp;



    __asm__ volatile("pextlw %0, %3, %2\n\tpextlw %1, %5, %4\n\tpextlw %0, %1, %0"
                     : "=&r"(out128), "=r"(tmp) : "r"(x), "r"(z), "r"(y), "r"(w));
    return out128;
}
/* The original's inline at 969-975 (.mdebug: x, z set up at 969, out128, pcpyld at 973; 371 uses in 11 units): x low, z
 * high doubleword (GIF tags, A+D). Name ours. MATCH: asm volatile (plain: equal tags merge in Gprim's makePkt) */
inline u_long128 _makeQW(ULONGLONG x, ULONGLONG z) {
    u_long128 out128;


    __asm__ volatile("pcpyld %0, %2, %1" : "=r"(out128) : "r"(x), "r"(z));
    return out128;
}
/* The original's element inlines at 978-1055 (.mdebug: the pcpyud at 982, sll at 992, dsra32 at 1001, sll at 1011,
 * dsra32/mtc1 at 1044, the por at 1055; locals x, out64, out32, outf, tmp). Their names are ours. */
inline ULONGLONG _getHi64(u_long128 x) {
    ULONGLONG out64;


    __asm__ volatile("pcpyud %0, %1, $0" : "=r"(out64) : "r"(x));
    return out64;
}



inline INT _getX(u_long128 x) {
    INT out32;


    __asm__ volatile("sll %0, %1, 0" : "=r"(out32) : "r"(x));
    return out32;
}


inline INT _getY(u_long128 x) {
    INT out32;


    __asm__ volatile("dsra32 %0, %1, 0" : "=r"(out32) : "r"(x));
    return out32;
}



inline INT _getZ(u_long128 x) {
    INT out32;


    __asm__ volatile("sll %0, %1, 0" : "=r"(out32) : "r"(_getHi64(x)));
    return out32;
}

























inline FLOAT _hiToFloat(u_long128 x) {
    FLOAT outf;
    INT tmp;


    __asm__ volatile("dsra32 %1, %2, 0\n\tmtc1 %1, %0" : "=f"(outf), "=&r"(tmp) : "r"(x));
    return outf;
}






inline FLOAT _getW(u_long128 x) {
    /* the 64-bit high half goes in as u_long128: a por, as in the original */
    return _hiToFloat(_getHi64(x));
}
















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

    C3DTile() {
        m_P0.set(-0.5f, -0.5f, 0.0f);
        m_P1.set(0.5f, 0.5f, 0.0f);
    }
    virtual UINT getPacketSize() const { return 3; }
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

    C3DSprt() {
        m_P0.set(-0.5f, -0.5f, 0.0f);
        m_P1.set(0.5f, 0.5f, 0.0f);
    }
    void setUVWH(USHORT u, USHORT v, USHORT w, USHORT h) {
        m_T0.set(u, v);
        m_T1.set(u + w, v + h);
    }
    virtual UINT getPacketSize() const { return CGTexPrim::getPacketSize() + 5; }
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

    C3DRotSprt() { m_Rot = 0; }
    virtual UINT getPacketSize() const { return CGTexPrim::getPacketSize() + 7; }
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

    C3DPolyFT4() {
        m_C0.col = 0x80808080;
        m_S0.set(0.0f, 0.0f);
        m_S1.set(1.0f, 0.0f);
        m_S2.set(0.0f, 1.0f);
        m_S3.set(1.0f, 1.0f);
    }
    virtual UINT getPacketSize() const { return CGTexPrim::getPacketSize() + 8; }
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

    C3DPolyGT4() {
        m_C0.col = 0x80808080;
        m_C1.col = 0x80808080;
        m_C2.col = 0x80808080;
        m_C3.col = 0x80808080;
        m_S0.set(0.0f, 0.0f);
        m_S1.set(1.0f, 0.0f);
        m_S2.set(0.0f, 1.0f);
        m_S3.set(1.0f, 1.0f);
    }
    virtual UINT getPacketSize() const { return CGTexPrim::getPacketSize() + 8; }
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

    C3DPolyFT3() {
        m_C0.col = 0x80808080;
        m_S0.set(0.0f, 0.0f);
        m_S1.set(1.0f, 0.0f);
        m_S2.set(0.0f, 1.0f);
    }
    virtual UINT getPacketSize() const { return CGTexPrim::getPacketSize() + 7; }
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

    C3DPolyGT3() {
        m_C0.col = 0x80808080;
        m_C1.col = 0x80808080;
        m_C2.col = 0x80808080;
        m_S0.set(0.0f, 0.0f);
        m_S1.set(1.0f, 0.0f);
        m_S2.set(0.0f, 1.0f);
    }
    virtual UINT getPacketSize() const { return CGTexPrim::getPacketSize() + 7; }
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

    /* MATCH: declared, unlike the other derived destructors (the original's is at line 1334, the class end: implicit);
     * implicit, ~CAvatar's inlined copies for lineX/Y/Z take other registers (its gcse fillers were tuned to this) */
    virtual ~C3DLine() {}
    virtual UINT getPacketSize() const { return 4; }
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

    virtual UINT getPacketSize() const { return 4; }
private:
    virtual FIX4 calcZ();
    virtual void makePkt();
};






class C3DLines : public CGPrim {
public:
    CCoord* m_pCrd;
    G_LINE_VECTOR m_vLINE;

    virtual UINT getPacketSize() const { return (UINT)(m_vLINE.size() * 40 + 0x1F) >> 4; }
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

    virtual UINT getPacketSize() const { return 6; }
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

    virtual UINT getPacketSize() const { return 5; }
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

    C3DCircle() {}
    virtual UINT getPacketSize() const { return 104; }
protected:
    virtual FIX4 calcZ();
    virtual void makePkt();
};










class C3DPointLight : public C3DCircle {
public:
    FLOAT m_R00;
    INT m_Active;

    virtual UINT getPacketSize() const { return m_Active ? 308 : 204; }
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

    virtual UINT getPacketSize() const { return m_Active ? 412 : 204; }
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
    C3DStripBase() {
        m_pCrd = NULL;
        m_pObj = NULL;
        m_fGeta = 0.0f;
    }
    virtual UINT getPacketSize() const { return (UINT)((m_vVERT.size() * 32 + 0x1F) & ~0xF) >> 4; }
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
    C3DTexStripBase() {
        m_pCrd = NULL;
        m_pObj = NULL;
        m_fGeta = 0.0f;
    }
    virtual UINT getPacketSize() const {
        UINT size = m_vVERT.size() * 48 + 48;
        return (size >> 4) + CGTexPrim::getPacketSize();
    }
private:
    virtual FIX4 calcZ();
    virtual void makePkt();
    virtual QWdata* makePktSub(QWdata* pQw) = 0;
};


















class C3DLineStrip : public C3DStripBase {
public:
    C3DLineStrip() { m_nType = 2; }
    void setCoord(CCoord* p) { m_pCrd = p; }
    CCoord* getCoord() { return m_pCrd; }

private:
    virtual QWdata* makePktSub(QWdata* pQw);
};






class C3DLineStripCoord : public C3DStripBase {
public:
    C3DLineStripCoord() { m_nType = 2; }
    void setObject(CCoords* p) { m_pObj = p; }
    CCoords* getObject() { return m_pObj; }

private:
    virtual QWdata* makePktSub(QWdata* pQw);
};






class C3DLineTStrip : public C3DTexStripBase {
public:
    C3DLineTStrip() { m_nType = 2; }
    void setCoord(CCoord* p) { m_pCrd = p; }
    CCoord* getCoord() { return m_pCrd; }

private:
    virtual QWdata* makePktSub(QWdata* pQw);
};






class C3DLineTStripCoord : public C3DTexStripBase {
public:
    C3DLineTStripCoord() { m_nType = 2; }
    void setObject(CCoords* p) { m_pObj = p; }
    CCoords* getObject() { return m_pObj; }

private:
    virtual QWdata* makePktSub(QWdata* pQw);
};






class C3DPolyStrip : public C3DStripBase {
public:
    C3DPolyStrip() { m_nType = 4; }
    void setCoord(CCoord* p) { m_pCrd = p; }
    CCoord* getCoord() { return m_pCrd; }

private:
    virtual QWdata* makePktSub(QWdata* pQw);
};






class C3DPolyStripCoord : public C3DStripBase {
public:
    C3DPolyStripCoord() { m_nType = 4; }
    void setObject(CCoords* p) { m_pObj = p; }
    CCoords* getObject() { return m_pObj; }

private:
    virtual QWdata* makePktSub(QWdata* pQw);
};






class C3DPolyTStrip : public C3DTexStripBase {
public:
    C3DPolyTStrip() { m_nType = 4; }
    void setCoord(CCoord* p) { m_pCrd = p; }
    CCoord* getCoord() { return m_pCrd; }

private:
    virtual QWdata* makePktSub(QWdata* pQw);
};






class C3DPolyTStripCoord : public C3DTexStripBase {
public:
    C3DPolyTStripCoord() { m_nType = 4; }
    void setObject(CCoords* p) { m_pObj = p; }
    CCoords* getObject() { return m_pObj; }

private:
    virtual QWdata* makePktSub(QWdata* pQw);
};






class C3DPolyFan : public C3DStripBase {
public:
    C3DPolyFan() { m_nType = 5; }
    void setCoord(CCoord* p) { m_pCrd = p; }
    CCoord* getCoord() { return m_pCrd; }

private:
    virtual QWdata* makePktSub(QWdata* pQw);
};






class C3DPolyFanCoord : public C3DStripBase {
public:
    C3DPolyFanCoord() { m_nType = 5; }
    void setObject(CCoords* p) { m_pObj = p; }
    CCoords* getObject() { return m_pObj; }

private:
    virtual QWdata* makePktSub(QWdata* pQw);
};






class C3DPolyTFan : public C3DTexStripBase {
public:
    C3DPolyTFan() { m_nType = 5; }
    void setCoord(CCoord* p) { m_pCrd = p; }
    CCoord* getCoord() { return m_pCrd; }

private:
    virtual QWdata* makePktSub(QWdata* pQw);
};






class C3DPolyTFanCoord : public C3DTexStripBase {
public:
    C3DPolyTFanCoord() { m_nType = 5; }
    void setObject(CCoords* p) { m_pObj = p; }
    CCoords* getObject() { return m_pObj; }

private:
    virtual QWdata* makePktSub(QWdata* pQw);
};



















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

    C3DCPrim() { _init(); }
    virtual ~C3DCPrim() { remove(); }
    void create(ULONGLONG iPrim, INT iVertexNum, CCoord* pCParent, INT iRep) {
        _create(iPrim, iVertexNum, pCParent, NULL, iRep);
    }
    void create(ULONGLONG iPrim, INT iVertexNum, CCoords* pOParent, INT iRep) {
        _create(iPrim, iVertexNum, NULL, pOParent, iRep);
    }
    void create(ULONGLONG iPrim, INT iVertexNum) { _create(iPrim, iVertexNum, NULL, NULL, 0); }
    void remove() {
        if (m_V) com_free(m_V);
        m_V = NULL;
        _init();
    }
    void setTEST(ULONGLONG TEST) { m_TEST = TEST; }
    void setALPHA(ULONGLONG ALPHA) { m_ALPHA = (ALPHA >> 16) | ALPHA; }
    void setCLAMP(ULONGLONG CLAMP) { m_CLAMP = CLAMP; }
    void setSCISSOR(ULONGLONG SCISSOR) { m_SCISSOR = SCISSOR; }
    void setZBUF(ULONGLONG ZBUF) { m_ZBUF = ((ZBUF >> 2) & 0xF0000000) | ZBUF; }
    CCoord* getCoordPtr() const { return m_pCrd; }
    CCoords* getObjPtr() const { return m_pObj; }
    void setCalcZ(INT iZ) { m_iZ = iZ; }
    virtual UINT getPacketSize() const {
        if (!m_PackReglist[m_PRIM & 7]) return (UINT)(((m_iMax * 32 + 0x1F) & ~0xF) + 0xA0) >> 4;
        return (UINT)(((m_iMax * 16 + 0x3F) & ~0xF) + 0xA0) >> 4;
    }
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

    C3DTPrim() { _init(); }
    virtual ~C3DTPrim() { remove(); }
    void create(ULONGLONG iPrim, INT iVertexNum, CCoord* pCParent, INT iRep) {
        _create(iPrim, iVertexNum, pCParent, NULL, iRep);
    }
    void create(ULONGLONG iPrim, INT iVertexNum, CCoords* pOParent, INT iRep) {
        _create(iPrim, iVertexNum, NULL, pOParent, iRep);
    }
    void create(ULONGLONG iPrim, INT iVertexNum) { _create(iPrim, iVertexNum, NULL, NULL, 0); }
    void remove() {
        if (m_V) com_free(m_V);
        m_V = NULL;
        _init();
    }
    void setTEST(ULONGLONG TEST) { m_TEST = TEST; }
    void setALPHA(ULONGLONG ALPHA) { m_ALPHA = (ALPHA >> 16) | ALPHA; }
    void setCLAMP(ULONGLONG CLAMP) { m_CLAMP = CLAMP; }
    void setSCISSOR(ULONGLONG SCISSOR) { m_SCISSOR = SCISSOR; }
    void setZBUF(ULONGLONG ZBUF) { m_ZBUF = ((ZBUF >> 2) & 0xF0000000) | ZBUF; }
    CCoord* getCoordPtr() const { return m_pCrd; }
    CCoords* getObjPtr() const { return m_pObj; }
    void setCalcZ(INT iZ) { m_iZ = iZ; }
    virtual UINT getPacketSize() const {
        UINT size;
        if (m_PackReglist[m_PRIM & 7]) {
            size = (UINT)(((m_iMax * 24 + 0x3F) & ~0xF) + 0xA0) >> 4;
        } else {
            size = (UINT)(((m_iMax * 48 + 0x3F) & ~0xF) + 0xA0) >> 4;
        }
        return size + CGTexPrim::getPacketSize();
    }
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

#endif /* SYSTEM_GRAPHIC_GPRIM_H */
