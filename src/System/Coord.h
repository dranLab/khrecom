
#ifndef SYSTEM_COORD_H
#define SYSTEM_COORD_H

#include "khtypes.h"
#include "System/Graphic/Camera.h"

class CCoords {
public:
    enum _PROC_PRIO {
        _PP_PRIMARY = 1,
        _PP_SECONDARY = 2,
        _PP_ALL = 3
    };

private:
    CCoord* m_pCrdArray;
    UINT m_nCrdNum;
    HANDLE m_hSysHandle;
    UINT m_iFlags;

public:
    CCoords(_PROC_PRIO pp);
    virtual ~CCoords();
    BOOL addCoord(UINT nNum, CCoord* pParent);
    void delCoord();
    HANDLE setSysFix(BOOL bSet, _PROC_PRIO pp);
    HANDLE getSysHandle() const { return m_hSysHandle; }
    INT getCoordNum() const { return m_nCrdNum; }
    CCoord& Crd(UINT nIndex) {
        if (m_nCrdNum && m_pCrdArray && nIndex < m_nCrdNum) {
            return m_pCrdArray[nIndex];
        }
        _coord_alart(nIndex);
        return getExceptCoord();
    }
    CCoord& Crd(UINT nIndex) const {
        if (m_nCrdNum && m_pCrdArray && nIndex < m_nCrdNum) {
            return m_pCrdArray[nIndex];
        }
        _coord_alart(nIndex);
        return getExceptCoord();
    }
    CCoord* Crd() {
        if (m_nCrdNum && m_pCrdArray) {
            return m_pCrdArray;
        }
        _coord_nullalart();
        return __null;
    }
    CCoord* Crd() const {
        if (m_nCrdNum && m_pCrdArray) {
            return m_pCrdArray;
        }
        _coord_nullalart();
        return __null;
    }
    FMATRIX& getLocalMatrix(UINT nIndex) { return Crd(nIndex).m_Local; }
    FMATRIX& getWorldMatrix(UINT nIndex) const { return Crd(nIndex).m_World; }
    void update(TIMEQ nTQ);
    void touch();
    void resetTimeStamp();

private:
    void _setCoordHandle(HANDLE h);
    BOOL isSysUpdate(_PROC_PRIO pp) { return m_iFlags & pp; }
    void setSysHandle(HANDLE h);
    CCoord& getExceptCoord();
    CCoord& getExceptCoord() const;
    void _coord_alart(UINT nIndex) const;
    void _coord_nullalart() const;

    friend class CCoordMan;
};

void ApplyMatrix33(FVECTOR& v0, const FMATRIX& m0, const FVECTOR& v1);
void ViewClipMatrix(FMATRIX& m, FLOAT scrw, FLOAT scrh, FLOAT scrz, FLOAT fNear, FLOAT fFar);
void ApplyMatrix(FVECTOR& v0, const FMATRIX& m0, const FVECTOR& v1);
void MulMatrix(FMATRIX& m0, const FMATRIX& m1, const FMATRIX& m2);
void OuterProduct(FVECTOR& v0, const FVECTOR& v1, const FVECTOR& v2);
FLOAT InnerProduct(const FVECTOR& v0, const FVECTOR& v1);
FLOAT InnerProduct(const FVECTOR& v0);
void Normalize(FVECTOR& v0, const FVECTOR& v1);
void DivVector(FVECTOR& v0, const FVECTOR& v1, FLOAT q);
void DivVectorXYZ(FVECTOR& v0, const FVECTOR& v1, FLOAT f);
void AddVector(FVECTOR& v0, const FVECTOR& v1, const FVECTOR& v2);
void SubVector(FVECTOR& v0, const FVECTOR& v1, const FVECTOR& v2);
void MulVector(FVECTOR& v0, const FVECTOR& v1, const FVECTOR& v2);
void ScaleVector(FVECTOR& v0, const FVECTOR& v1, FLOAT f);
void ScaleVectorXYZ(FVECTOR& v0, const FVECTOR& v1, FLOAT f);
void FTOI0Vector(IVECTOR& v0, const FVECTOR& v1);
void FTOI4Vector(IVECTOR& v0, const FVECTOR& v1);
void ITOF0Vector(FVECTOR& v0, const IVECTOR& v1);
void AddVectorXYZ(FVECTOR& v0, const FVECTOR& v1, const FVECTOR& v2);
void SubVectorXYZ(FVECTOR& v0, const FVECTOR& v1, const FVECTOR& v2);
void MulVectorXYZ(FVECTOR& v0, const FVECTOR& v1, const FVECTOR& v2);

#endif /* SYSTEM_COORD_H */
