#ifndef SYSTEM_GRAPHIC_LIGHTUNIT_H
#define SYSTEM_GRAPHIC_LIGHTUNIT_H

#include "khtypes.h"
#include "System/Vector.h"

class CCoord;
struct MODELDATA;
struct LIGHTDATA;
struct BBOX;
class CEffectLightMan;

struct POINTLIGHT {
    FVECTOR pos;
    FVECTOR col;
    FLOAT start;
    FLOAT end;
    USHORT nType;
    USHORT nPad;
    INT inUse;
    FVECTOR pos1;
};

struct POINTLIGHTPACKED {
    float m_fX[4];
    float m_fY[4];
    float m_fZ[4];
    float m_fFar[4];
    FVECTOR m_vColor[4];
    float m_fNear[4];
    float m_fLength[4];
};

class CEffectLight : public POINTLIGHT {
    friend class CEffectLightMan;

private:
    CEffectLightMan* m_pManager;

public:
    CEffectLight() {
        m_pManager = NULL;
        inUse = -1;
    }
    ~CEffectLight() { close(); }
    void setPosition(/* parameters unknown */);
    void setColor(/* parameters unknown */);
    void setStartEnd(/* parameters unknown */);
    void setParam(/* parameters unknown */);
    void close();
    BOOL isClosed() { return inUse < 0; }
    INT getID() { return inUse; }

protected:
    void linkManager(CEffectLightMan& inMan) { m_pManager = &inMan; }
    void open(INT n) { inUse = n; }
};

class CEffectLightMan {
    friend class CEffectLight;

private:
    CEffectLight m_LightTbl[32];
    UINT m_nActiveLightInfo;
    UINT m_nLightIndex;
    UINT m_nLightSeeker;
    UINT m_nLightSeekMask;

public:
    CEffectLightMan();
    CEffectLight* getBlankLight();
    CEffectLight* getLight(UINT nIndex) {
        if (nIndex < 32) return &m_LightTbl[nIndex];
        return NULL;
    }
    void initSeekActiveLight() {
        m_nLightSeeker = 1;
        m_nLightIndex = 0;
        m_nLightSeekMask = -1;
    }
    CEffectLight* seekNextActiveLight();
    void reset();

protected:
    void closeLight(UINT n) {
        if (n < 32) {
            m_nActiveLightInfo &= ~(1 << n);
        }
    }
};

inline void CEffectLight::close() {
    if (inUse >= 0) {
        if (m_pManager) {
            m_pManager->closeLight(inUse);
        }
        inUse = -1;
    }
}

class CLightUnit {
protected:
    FMATRIX m_Light;
    FMATRIX m_Color;
    FVECTOR m_Ambient;
    INT m_Kind;
    INT m_Map;
    CCoord* m_pCrd;
    UINT m_iFlags;

public:
    CLightUnit() {
        m_Map = 0;
        m_iFlags = 1;
    }
    virtual ~CLightUnit() {}
    void setAnchor(const CCoord* p, INT kind);
    virtual void setAnchor(const CCoord* p, const MODELDATA& modeldata, INT kind) { setAnchor(p, kind); }
    virtual void reCalc() {
        FVECTOR ctr;
        calc();
        getCenter(ctr);
        _calc(ctr);
    }
    void calc() { m_iFlags &= ~1; }
    /* MATCH: types.h lists a second calc() without parameters: the BOOL is a guess, the body is what Model inlines */
    void calc(BOOL bInCheck) {
        if (!isCalc()) _calc();
    }
    BOOL isCalc() { return m_iFlags & 1; }
    void setAmbient(const FVECTOR& in_color);
    void setLight(INT idx, FVECTOR in_light, FVECTOR in_color);
    virtual POINTLIGHTPACKED* getPointLightData() const { return NULL; }
    void affectMapLight(/* parameters unknown */);
    void affectObjLight(/* parameters unknown */);
    void betaShade(FVECTOR& out, const FVECTOR& in);
    FMATRIX& getLightMatrix() { return m_Light; }
    FMATRIX& getColorMatrix() { return m_Color; }
    FVECTOR& getAmbientVector() { return m_Ambient; }

protected:
    void getCenter(FVECTOR& ctr);
    LIGHTDATA* getLightData() const;
    void _calc(const FVECTOR& ctr);
    virtual void _calc() {
        FVECTOR ctr;
        getCenter(ctr);
        _calc(ctr);
    }
};

class CMapLightUnit : public CLightUnit {
protected:
    FVECTOR ctr[2];
    POINTLIGHT* m_Fixed[4];
    float m_FixedVolume[4];
    POINTLIGHTPACKED m_Point[1];
    const BBOX* m_bbox;

public:
    CMapLightUnit() { m_Map = 1; }
    virtual ~CMapLightUnit() {}
    virtual void setAnchor(const CCoord* p, const MODELDATA& modeldata, INT kind);
    void setAnchor(const CCoord* p, const BBOX& bbox, INT kind);
    void init();
    virtual void reCalc() {
        calc();
        _calc();
    }
    virtual POINTLIGHTPACKED* getPointLightData() const { return (POINTLIGHTPACKED*)m_Point; }
    void setLight(INT idx, const POINTLIGHT* l);

protected:
    void calcCenter(const FMATRIX& m);
    virtual void _calc();
};

class CGimmickLightUnit : public CMapLightUnit {
public:
    CGimmickLightUnit() {}
    virtual ~CGimmickLightUnit() {}
    virtual void reCalc() {
        calc();
        _calc();
    }

protected:
    virtual void _calc();
};

#endif /* SYSTEM_GRAPHIC_LIGHTUNIT_H */
