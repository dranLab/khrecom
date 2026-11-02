/* The classes of System/Graphic/Gprim (included by Gprim.h). Gprim.cpp has the matching
 * #pragma implementation: every class here gets its vtable and all its inline functions emitted there, as in the
 * original object (in class definition order). */
#pragma interface /* MATCH: the inline functions and vtables only in Gprim.o, as in the originals */
/* create()'s "#line N GPRIM_H_PATH" (the original's assert path and line) is followed by a #line back to this file:
 * gcc applies the interface/implementation pragmas by file name, so the classes after it would otherwise lose them. */
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
    virtual ~CGTexPrim() {}
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
    /* MATCH: implicit destructor, as ~CPolyGT4ST's (2DSprObj's destructor inlines them for its CPolyGT4ST members) */
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

    virtual ~CScissoring() {}
    void setRect(G_XY& scissoringStart, G_XY& scissoringEnd);
    void setDefault();
    virtual UINT getPacketSize() const { return 3; }
private:
    virtual void makePkt();
};

class CZFill : public CGPrim {
public:
    INT m_nZ;

    virtual ~CZFill() {}
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

    virtual ~CTile() {}
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

    virtual ~CSprt() {}
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

    virtual ~CPolyGT4() {}
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

    /* MATCH: implicit destructor (see ~CGTexPrim2D) */
    virtual UINT getPacketSize() const { return CGTexPrim2D::getPacketSize() + 12; }
private:
    virtual void makePkt();
};

class CLineF2 : public CGPrim {
public:
    G_XYZ m_P0;
    G_XYZ m_P1;
    G_RGBA m_C0;

    virtual ~CLineF2() {}
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

    virtual ~CPolyG4() {}
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

    virtual ~CCircle() {}
    virtual UINT getPacketSize() const { return 104; }
protected:
    virtual FIX4 calcZ() { return m_P0.z; }
    virtual void makePkt();
};

class CScreenColoredMask : public CTile {
public:
    CScreenColoredMask();
    virtual ~CScreenColoredMask() {}
};



class CRotSprt : public CSprt, public C2DRot {
public:
    virtual ~CRotSprt() {}
private:
    virtual FIX4 calcZ() { return m_Center.z; }
    virtual void makePkt();
public:
    virtual UINT getPacketSize() const { return CGTexPrim::getPacketSize() + 7; }
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
#line 313 "src/System/Graphic/GprimFull.h"
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
    if (m_V && m_V->f[2] >= 0.0f) return (FIX4)m_V->f[2] << 4;
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
#line 365 "src/System/Graphic/GprimFull.h"
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
    if (m_V && m_V->fP[2] >= 0.0f) return (FIX4)m_V->fP[2] << 4;
    return 0;
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

    virtual ~C3DTile() {}
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

    virtual ~C3DSprt() {}
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

    virtual ~C3DRotSprt() {}
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

    virtual ~C3DPolyFT4() {}
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

    virtual ~C3DPolyGT4() {}
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

    virtual ~C3DPolyFT3() {}
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

    virtual ~C3DPolyGT3() {}
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

    virtual ~C3DLineCoord2() {}
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

    virtual ~C3DPolyG4() {}
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

    virtual ~C3DPolyG3() {}
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

    virtual ~C3DCircle() {}
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

    virtual ~C3DPointLight() {}
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

#define GPRIM_STRIP_CLASS(NAME, BASE, TYPE, SET, GET, PTR, MEMBER)                                                 \
    class NAME : public BASE {                                                                                     \
    public:                                                                                                        \
        NAME() { m_nType = TYPE; }                                                                                 \
        void SET(PTR* p) { MEMBER = p; }                                                                           \
        PTR* GET() { return MEMBER; }                                                                              \
    private:                                                                                                       \
        virtual QWdata* makePktSub(QWdata* pQw);                                                                   \
    }
GPRIM_STRIP_CLASS(C3DLineStrip, C3DStripBase, 2, setCoord, getCoord, CCoord, m_pCrd);
GPRIM_STRIP_CLASS(C3DLineStripCoord, C3DStripBase, 2, setObject, getObject, CCoords, m_pObj);
GPRIM_STRIP_CLASS(C3DLineTStrip, C3DTexStripBase, 2, setCoord, getCoord, CCoord, m_pCrd);
GPRIM_STRIP_CLASS(C3DLineTStripCoord, C3DTexStripBase, 2, setObject, getObject, CCoords, m_pObj);
GPRIM_STRIP_CLASS(C3DPolyStrip, C3DStripBase, 4, setCoord, getCoord, CCoord, m_pCrd);
GPRIM_STRIP_CLASS(C3DPolyStripCoord, C3DStripBase, 4, setObject, getObject, CCoords, m_pObj);
GPRIM_STRIP_CLASS(C3DPolyTStrip, C3DTexStripBase, 4, setCoord, getCoord, CCoord, m_pCrd);
GPRIM_STRIP_CLASS(C3DPolyTStripCoord, C3DTexStripBase, 4, setObject, getObject, CCoords, m_pObj);
GPRIM_STRIP_CLASS(C3DPolyFan, C3DStripBase, 5, setCoord, getCoord, CCoord, m_pCrd);
GPRIM_STRIP_CLASS(C3DPolyFanCoord, C3DStripBase, 5, setObject, getObject, CCoords, m_pObj);
GPRIM_STRIP_CLASS(C3DPolyTFan, C3DTexStripBase, 5, setCoord, getCoord, CCoord, m_pCrd);
GPRIM_STRIP_CLASS(C3DPolyTFanCoord, C3DTexStripBase, 5, setObject, getObject, CCoords, m_pObj);
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
