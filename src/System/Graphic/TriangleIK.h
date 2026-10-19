#ifndef SYSTEM_GRAPHIC_TRIANGLEIK_H
#define SYSTEM_GRAPHIC_TRIANGLEIK_H

#include "khtypes.h"
#include "System/Vector.h"
#include "System/Coord.h"
#include "System/Graphic/modeldata.h"

class CTriangleIK {
private:
    union {
        unsigned char m_nIdx[3];
        INT m_nIdxAll;
    };
    float m_fLen[2];

public:
    virtual ~CTriangleIK() {}
    void calc(CCoords& Crds, BOOL bDirF, const FVECTOR& vPos);
    BOOL setBoneIdx(INT nIdx0, INT nIdx1, INT nIdx2);
    INT getBoneIdx(INT n) const { return m_nIdx[n] == 0xFF ? -1 : m_nIdx[n]; }
    void setBoneLength(FLOAT fLen0, FLOAT fLen1) {
        m_fLen[0] = fLen0;
        m_fLen[1] = fLen1;
    }
    FLOAT getBoneLength(INT n) const { return m_fLen[n]; }
    BOOL isActive() const { return getBoneIdx(0) >= 0; }
};

class CBaseIK {
protected:
    CTriangleIK m_cTIK[2];

public:
    virtual ~CBaseIK() {}
    BOOL init(CCoords& Crds, int (*nIdx)[3]);
    void update(CCoords& Crds, BOOL bDirF, const FVECTOR* pvPosL, const FVECTOR* pvPosR);
    CTriangleIK& getTriangleIK(INT n) { return m_cTIK[n]; }
};

class CLegIK : public CBaseIK {
private:
    UINT m_iStatus;
    char* m_pszEffName;
    INT m_nEffNum;
    float m_fFootLen[2];
    float m_fToesLen[2];
    short int m_nToesIdx[2];
    float m_fPosY[2];

public:
    virtual ~CLegIK() {}
    BOOL init(CCoords& Crds, const MODELDATA& Model, FLOAT fFootLen, FLOAT fToesLen);
    void update(CCoords& Crds, const FVECTOR* pvPosL, const FVECTOR* pvPosR);
    void getFootPos(FVECTOR* pvOutPosL, FVECTOR* pvOutPosR, CCoords& Crds, FLOAT fr);
    void clearOldFootPos();
    void updateFootEffect(CCoords& Crds);
    static BOOL isIK(const MODELDATA& Model);
};

class CArmIK : public CBaseIK {
public:
    virtual ~CArmIK() {}
    BOOL init(CCoords& Crds, const MODELDATA& Model);
    void update(CCoords& Crds, const FVECTOR* pvPosL, const FVECTOR* pvPosR);
    static BOOL isIK(const MODELDATA& Model);
};

#endif // SYSTEM_GRAPHIC_TRIANGLEIK_H
