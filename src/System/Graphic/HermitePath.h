#ifndef SYSTEM_GRAPHIC_HERMITEPATH_H
#define SYSTEM_GRAPHIC_HERMITEPATH_H

#include "khtypes.h"
#include "System/Vector.h"

class CPathVector;

struct FVECTOR3 {
    FLOAT x;
    FLOAT y;
    FLOAT z;
};

struct UNIT_DATA {
    FVECTOR fvPoint;
    FVECTOR fvTangent;
};

class CHermitePath {
private:
    UNIT_DATA* m_pPathData;    /* 0x00 */
    FLOAT* m_pRollData;        /* 0x04 */
    INT m_iBufSize;            /* 0x08 */
    UINT m_uiStateFlag;        /* 0x0c */
    SHORT m_iDataNum;          /* 0x10 */
    SHORT m_iPathNum;          /* 0x12 */
    SHORT m_iNowBlock;         /* 0x14 */
    SHORT m_iStartBlock;       /* 0x16 */
    SHORT m_iEndBlock;         /* 0x18 */
    SHORT pad;                 /* 0x1a */
    FLOAT m_fAllDist;          /* 0x1c */
    FLOAT m_fProgress;         /* 0x20 */
    FLOAT m_fSpeed;            /* 0x24 */
    FLOAT m_fDataSpeed;        /* 0x28 */
    CPathVector* m_pPathVect;  /* 0x2c */

public:
    CHermitePath(INT inBufSize);
    CHermitePath();
    ~CHermitePath();
    BOOL reserveBuf(INT inBufSize);
    void makePath(const FVECTOR3* inVect, INT inNum, UINT uiOptionFlag);
    void makePathFromFVector(const FVECTOR* inVect, INT inNum, UINT uiOptionFlag);
    void makePathFromPathVector(const CPathVector* inpPath);
    INT getBufSize() { return m_iBufSize; }
    INT getDataNum() { return m_iDataNum; }
    INT getPathNum() { return m_iPathNum; }
    void getHermiteMatrix(FMATRIX& outMat, INT inIdx);
    FLOAT getHermiteCoefficient(INT inIdx, FLOAT infPos);
    FLOAT getHermiteDistance(INT inIdx);
    FLOAT getHermiteDistanceBlockToBlock(INT inIdx0, INT inIdx1);
    FLOAT getHermiteDistanceAll();
    FLOAT getHermiteDistanceRest(INT inIdx, FLOAT inNowDist);
    void getStartPos(FVECTOR& outPos);
    void getEndPos(FVECTOR& outPos);
    void getNowPos(FVECTOR& inrefPos);
    void getGapPos(FVECTOR& inrefPos, FLOAT inDist);
    void getNowRot(FVECTOR& inrefRot, FLOAT inDist);
    FLOAT getNowRoll();
    void changePathPoint(const FVECTOR* inNewPoint, INT inIdx);
    void changePathRoll(FLOAT inNewRoll, INT inIdx);
    void changePathNum(INT inNum);
    void clearAllRollData(FLOAT infRoll);
    void recalcHermitePath(UINT uiOptionFlag);
    void setSpeed(FLOAT infSpd);
    FLOAT getSpeed() { return m_fSpeed; }
    void setDataSpeed() { m_fDataSpeed = m_fSpeed; }
    FLOAT getDataSpeed() { return m_fDataSpeed; }
    INT getPathBlock() { return m_iNowBlock; }
    FLOAT getPathProgress() { return m_fProgress; }
    INT getNowBlock() { return m_iNowBlock; }   // PARTIAL: name used by System/Graphic/HermitePathCamera
    FLOAT getProgress() { return m_fProgress; } // PARTIAL: name used by System/Graphic/HermitePathCamera
    CPathVector* getPathVector() { return m_pPathVect; }
    BOOL update(FLOAT inRate);
    BOOL isRoundData();
    BOOL isLinearMoveData();
    BOOL isEnd();
    void goStartPos();
    void goEndPos();
    void goBlockStartPos(INT inBlock);
    void goBlockEndPos(INT inBlock);
    void setStartEndBlock(INT inStartBlock, INT inEndBlock);

private:
    BOOL _allocateMem(INT inNum);
    void _deleteMem();
    void _make2PointPath(const FVECTOR* inVect);
    void _makeTangent(FVECTOR& outVect, INT inIndex);
    BOOL _update(SHORT* pBlock, FLOAT* pfProgress, FLOAT inSpeed);
    void _calcAllDist();
};

#endif /* SYSTEM_GRAPHIC_HERMITEPATH_H */
