// PARTIAL: added for Game/ModelObj
#ifndef SYSTEM_GRAPHIC_LIGHTUNIT_H
#define SYSTEM_GRAPHIC_LIGHTUNIT_H

#include "khtypes.h"
#include "System/Vector.h"

class CCoord;
struct MODELDATA;
struct LIGHTDATA;
struct POINTLIGHTPACKED;

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
    void setAmbient(const FVECTOR& in_color);
    virtual POINTLIGHTPACKED* getPointLightData() const { return NULL; }

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
    void* m_Fixed[4];
    float m_FixedVolume[4];
    UBYTE m_Point[0xA0] __attribute__((aligned(16))); /* POINTLIGHTPACKED[1] */
    void* m_bbox;

public:
    CMapLightUnit() { m_Map = 1; }
    virtual ~CMapLightUnit() {}
    virtual void reCalc();
    virtual POINTLIGHTPACKED* getPointLightData() const;

protected:
    virtual void _calc();
};

class CGimmickLightUnit : public CMapLightUnit {
public:
    CGimmickLightUnit() {}
    virtual ~CGimmickLightUnit() {}
    virtual void reCalc();

protected:
    virtual void _calc();
};

#endif /* SYSTEM_GRAPHIC_LIGHTUNIT_H */
