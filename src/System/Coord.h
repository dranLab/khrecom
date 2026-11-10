
#ifndef SYSTEM_COORD_H
#define SYSTEM_COORD_H

#include "khtypes.h"
#include "System/Vector.h"

/* MATCH: CCoord is in System/Coord.h, not in Camera.h as the ref says: the original's line markers put its
 * constructor at Coord.h:1009-1016 and its accessors at Coord.h:1043-1072 (user decision 2026-11-10). */
class CCoord {
public:
    FMATRIX m_Local;
    // PARTIAL: the ref's fields from here are private (ours public)
    union {
        struct {
            union {
                struct {
                    TIMEQ m_FixTimestamp; /* 0x40 */
                    INT m_Flag;           /* 0x44 */
                };
                ULONGLONG _ulFixTimestamp;
            };
            TIMEQ m_WorldTimestamp; /* 0x48 */
            CCoord* m_pParent;      /* 0x4c */
        };
        u_long128 _q40; // MATCH: Game/Atel/EventTrap: the constructor clears 0x40..0x4f at once
    };
    union {
        FMATRIX m_Rot;              /* 0x50 */
        struct {
            int m_hHandlePad___[12];
            FLOAT m_fScaleX;        /* 0x80 */
            FLOAT m_fScaleY;        /* 0x84 */
            FLOAT m_fScaleZ;        /* 0x88 */
            INT m_hHandle;          /* 0x8c */
        };
    };
    FMATRIX m_World;     /* 0x90 */
    void setLocal(const FMATRIX& matIn) {
        if (m_Local != matIn) {
            m_Local = matIn;
            touch();
        }
    }
    void updateLocal(/* parameters unknown */);
    void setTrans(/* parameters unknown */);
    void updateTrans(/* parameters unknown */);
    void setParent(CCoord* pParent) { m_pParent = pParent; }
    void touch(); // defined below the class
    CCoord() {
        _q40 = 0;
        m_fScaleX = 1.0f;
        m_fScaleY = 1.0f;
        m_fScaleZ = 1.0f;
        m_hHandle = 0;
    }
    void fix(INT n = 0);
    TIMEQ getWorldTimestamp(/* parameters unknown */);
    FMATRIX& getWorldMatrix() { return m_World; }
    FMATRIX& getWorldRot33Matrix(/* parameters unknown */);
    FVECTOR getWorldPos() { return m_World.v[3]; } // PARTIAL: Effect/effect and EffectMan (original name unknown)
    FMATRIX& getLocalMatrix() { return m_Local; }
    CCoord* getParent() { return m_pParent; }
    void recalcLocalMatrix();
    void getScale(FVECTOR& outScale) {
        outScale.x = m_fScaleX;
        outScale.y = m_fScaleY;
        outScale.z = m_fScaleZ;
    }
    void recalcWorldMatrix();
    void getRotTransPersMatrix(FMATRIX& outMat);
    void getRotTransPersMatrix_VF04_07();
    void RotTransPers(IVECTOR& outV, const FVECTOR& inV);
    void RotTransPers(FVECTOR& outV, const FVECTOR& inV);
    void resetTimestamp() {
        m_FixTimestamp = -1;
        m_WorldTimestamp = -1;
    }
    HANDLE getHandle(/* parameters unknown */);
    void setHandle(HANDLE h) { m_hHandle = h; }
    void setScale(const FVECTOR& inScale) {
        m_fScaleX = inScale.x;
        m_fScaleY = inScale.y;
        m_fScaleZ = inScale.z;
        m_Flag = 1;
    }

private:
    void _mulParent(/* parameters unknown */);
    void _checkParent(/* parameters unknown */);
};

inline void CCoord::touch() { m_Flag = 1; }

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
