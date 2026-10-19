#ifndef SYSTEM_GRAPHIC_PATHDATA_H
#define SYSTEM_GRAPHIC_PATHDATA_H

#include "khtypes.h"
#include "System/Vector.h"
#include "System/Graphic/Gprim.h"
#include "System/Graphic/Camera.h"

/* PARTIAL: added for System/Graphic/PathDataCamera (layouts from the debug info; members as needed) */

class CDbgArrow {
private:
    CCoord m_Coord;                /* 0x000 */
    UBYTE m_gpPolyStrip[0x50];     /* 0x0d0 C3DPolyStrip */

public:
    CDbgArrow();
    ~CDbgArrow();
    void disp();
    void setCoord(FVECTOR* pvPos, FVECTOR* pvRot);
    CCoord& getCoord() { return m_Coord; }
};

struct PATH_DATA_HEAD {
    INT iId;
    int iPad32[2];
    USHORT iVer;
    USHORT iPathNum;
};

struct PATH_VECTOR_PARAM {
    USHORT iVecNum;
    USHORT iVer;
    UINT iFlags;
    int iPad32[1];
    INT iNext;
};

struct PATH_VECTOR {
    PATH_VECTOR_PARAM sParam;
    FVECTOR vVec[1];
};

struct PATH_DATA {
    PATH_DATA_HEAD sHead;
    PATH_VECTOR sPath;
};

struct PATH_PARAM {
    UINT iFlags;
    FLOAT fTime;
    FLOAT fSpeed;
    FLOAT fTargetDistance;
    FLOAT fTargetOffsetY;
    INT iParentLink;
    FLOAT fProj;
    FLOAT fLookDelay;
    FLOAT fLearpIn;
    FLOAT fLearpOut;
    FLOAT fProj2;
} __attribute__((aligned(16))); /* 0x30 */

class CPathVector {
    friend class CHermitePath;
private:
    FVECTOR* m_pVec;               /* 0x000 */
    INT m_iVecMax;                 /* 0x004 */
    PATH_VECTOR* m_pPathData;      /* 0x008 */
    PATH_PARAM m_sParam;           /* 0x010 */
    INT m_iStatus;                 /* 0x040 */
    INT m_iVecMemMax;              /* 0x044 */
    INT m_iActive;                 /* 0x048 */
    FVECTOR m_vRot;                /* 0x050 */
    C3DLineStrip m_DDPathLines;    /* 0x060 */
    C3DLineStrip m_DDXYZLines;     /* 0x0a4 */
    CCoord m_DDXYZCoord;           /* 0x0f0 */
    class CDbgCube* m_pCube;       /* 0x1c0 */
    C3DPolyStrip m_DDPathPolys;    /* 0x1c4 */

public:
    // PARTIAL: added for System/Graphic/HermitePathCamera
    CPathVector(const FVECTOR* pPath, INT iPathMax) { init(pPath, iPathMax, 0); }
    ~CPathVector();
    void init(const FVECTOR* pPath, INT iPathMax, INT iMode);
    FLOAT getPathRoll(INT iIdx) const;
    float getTargetDistanceToPathStep(const FVECTOR& vTarget, float fStep);
    void getPathStepToPosition(FVECTOR& vPos, float fStep, BOOL bLoop);
    FLOAT getPathStepToRoll(float fStep);
    INT getPathVectorNum() { return m_iVecMax; }
    FVECTOR* getPathVector() { return m_pVec; }
    FVECTOR& getPathPosition(INT iIdx) { return m_pVec[iIdx]; }
    FLOAT getPathSpeed(INT iIdx) const;
    void getPathParam(PATH_PARAM& sParam) const;
    void dispDebugLine(INT iMode);
};

class CPathData {
private:
    CPathVector* m_pPath;          /* 0x00 */
    INT m_iPathMax;                /* 0x04 */
    PATH_DATA* m_pPathData;        /* 0x08 */
    INT m_iStatus;                 /* 0x0c */
    INT m_iPathMemMax;             /* 0x10 */
    INT m_iActive;                 /* 0x14 */
    C3DLineStrip m_DDLinkLines;    /* 0x18 */

public:
    CPathData(const CHAR* pszName) { init(pszName, 0); }
    CPathData(const PATH_DATA* pPathData) { init(pPathData, 0); }
    ~CPathData();
    void init(const CHAR* pszName, INT iMode);
    void init(const PATH_DATA* pPathData, INT iMode);
    INT getPathNum() { return m_iPathMax; }
    CPathVector* getPathVectorData(INT iPathNum) {
        if (iPathNum >= m_iPathMax) return NULL;
        return &m_pPath[iPathNum];
    }
    INT getPathVectorNum(INT iPathNum) { return m_pPath[iPathNum].getPathVectorNum(); }
    FVECTOR& getPathPosition(INT iPathNum, INT iIdx) { return m_pPath[iPathNum].getPathPosition(iIdx); }
    void getPathParam(INT iPathNum, PATH_PARAM& sParam) { m_pPath[iPathNum].getPathParam(sParam); }
    INT getLinkChild(INT iPathNum, INT iStart);
};

#endif /* SYSTEM_GRAPHIC_PATHDATA_H */
