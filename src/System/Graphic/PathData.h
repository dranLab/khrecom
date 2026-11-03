#ifndef SYSTEM_GRAPHIC_PATHDATA_H
#define SYSTEM_GRAPHIC_PATHDATA_H

#include "khtypes.h"
#include "System/Vector.h"
#include "System/Graphic/Gprim.h"
#include "System/Graphic/Camera.h"



class CDbgCube {
private:
    CCoord m_Coord;                /* 0x000 */
    C3DPolyStrip m_gpPolyStrip;    /* 0x0d0 */

public:
    CDbgCube(CCoord* pCrd);
    CDbgCube();
    ~CDbgCube();
    void disp();
    void setCoord(FVECTOR* pvPos, FVECTOR* pvRot);
    CCoord& getCoord() { return m_Coord; }
private:
    void init(CCoord* pCrd);
};

class CDbgArrow {
private:
    CCoord m_Coord;                /* 0x000 */
    C3DPolyStrip m_gpPolyStrip;    /* 0x0d0 */

public:
    CDbgArrow(CCoord* pCrd);
    CDbgArrow();
    ~CDbgArrow();
    void disp();
    void setCoord(FVECTOR* pvPos, FVECTOR* pvRot);
    CCoord& getCoord() { return m_Coord; }
private:
    void init(CCoord* pCrd);
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
    CPathVector(const FVECTOR* pPath, INT iPathMax) { init(pPath, iPathMax, 0); }
    CPathVector() : m_pVec(NULL), m_iVecMax(0), m_pPathData(NULL) {}
    ~CPathVector();
    void init(const FVECTOR* pPath, INT iPathMax, INT iMode);
    void init(const PATH_VECTOR* pPathData, BOOL bAlloc);
    INT getPathVectorNum() { return m_iVecMax; }
    FVECTOR* getPathVector() { return m_pVec; }
    FVECTOR& getPathPosition(INT iIdx) { return m_pVec[iIdx]; }
    FLOAT getPathRoll(INT iNum) const;
    FLOAT getPathSpeed(INT iNum) const;
    void getPathParam(PATH_PARAM& sParam) const;
    void makeVertexSpeedAll();
    float getTargetDistanceToPathStep(const FVECTOR& tpos, float distance);
    float getTargetToPathStep(const FVECTOR& tpos);
    void getPathStepToPosition(FVECTOR& mpos, float fstep, BOOL iTcb);
    FLOAT getPathStepToRoll(float fStep);
    float getPathStepToVectorIdx4(int* pv, float fstep);
    float getPathStepToVector4(const FVECTOR** pv, float fstep);
    void setPathParam(PATH_PARAM& sParam);
    void setPathPosition(INT iNum, const FVECTOR& vPos) { m_pVec[iNum] = vPos; }
    void setActive(INT iNum) { *(INT*)&m_iActive = iNum; } // MATCH: not a struct-field store, so callers reload statics after it (gcc 2.95 alias heuristic; CDebugPathData::setActivePathVecNum)
    BOOL addPath(INT iNum);
    BOOL delPath(INT iNum);
    void dispDebugLine(INT iFlg);
private:
    void _init(const FVECTOR* pPath, INT iPathMax, BOOL bAlloc);
    void getNearPath(FVECTOR& outvN, float* outfDis, float* outfT, int* outiStep, const FVECTOR& vTarget,
                     const FVECTOR* pathv, int path_max, float hashi);
    void __addPathAlloc(FVECTOR*& pVec, INT iVecMax, INT iVecMemMax);
    void __addPathCopy(FVECTOR*& pVec, INT iVecMax, INT iNum);
    void __delPathCopy(FVECTOR*& pVec, INT iVecMax, INT iNum);
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
    CPathData(const CHAR* pszInName, BOOL bAlloc) { init(pszInName, bAlloc); }
    CPathData(const PATH_DATA* pPathData) { init(pPathData, 0); }
    CPathData(const PATH_DATA* pPathData, BOOL bAlloc) { init(pPathData, bAlloc); }
    ~CPathData();
    void init(const PATH_DATA* pPathData, INT iMode);
    void init(const CHAR* pszName, INT iMode);
    INT getPathNum() { return m_iPathMax; }
    CPathVector* getPathVectorData(INT iPathNum) {
        if (iPathNum >= m_iPathMax) return NULL;
        return &m_pPath[iPathNum];
    }
    INT getPathVectorNum(INT iPathNum) { return m_pPath[iPathNum].getPathVectorNum(); }
    FVECTOR* getPathVector(INT iPathNum) { return m_pPath[iPathNum].getPathVector(); }
    FVECTOR& getPathPosition(INT iPathNum, INT iIdx) { return m_pPath[iPathNum].getPathPosition(iIdx); }
    void getPathParam(INT iPathNum, PATH_PARAM& sParam) { m_pPath[iPathNum].getPathParam(sParam); }
    INT getLinkChild(INT iPathNum, INT iStart);
    void makeVertexSpeedAll() {
        for (INT i = 0; i < getPathNum(); i++) getPathVectorData(i)->makeVertexSpeedAll();
    }
    void setPathParam(INT iPathNum, PATH_PARAM& sParam) { m_pPath[iPathNum].setPathParam(sParam); }
    void setPathPosition(INT iPathNum, INT iNum, const FVECTOR& vPos) { m_pPath[iPathNum].setPathPosition(iNum, vPos); }
    void setActive(INT iPathNum, INT iNum) {
        *(INT*)&m_iActive = iPathNum; // MATCH: as CPathVector::setActive
        m_pPath[iPathNum].setActive(iNum);
    }
    BOOL addPath(const FVECTOR* pVec, INT iNum);
    BOOL delPath(INT iNum);
    BOOL addPathVector(INT iPathNum, INT iNum) { return m_pPath[iPathNum].addPath(iNum); }
    BOOL delPathVector(INT iPathNum, INT iNum) { return m_pPath[iPathNum].delPath(iNum); }
    void dispDebugLine();
private:
    void _init(const PATH_DATA* pPathData, BOOL bAlloc);
};

FLOAT _getPathVectorConvWRoll(FLOAT w);
void _setPathVectorConvWRoll(FLOAT* fW, FLOAT fRoll);
FLOAT _getPathVectorConvWSpeed(FLOAT w);
void _setPathVectorConvWSpeed(FLOAT* fW, FLOAT fSpeed);

#endif /* SYSTEM_GRAPHIC_PATHDATA_H */
