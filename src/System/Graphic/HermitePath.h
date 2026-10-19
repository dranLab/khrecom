#ifndef SYSTEM_GRAPHIC_HERMITEPATH_H
#define SYSTEM_GRAPHIC_HERMITEPATH_H

#include "khtypes.h"
#include "System/Vector.h"

/* PARTIAL: added for System/Graphic/PathDataCamera (layout from the debug info; members as needed) */

class CPathVector;
struct UNIT_DATA;

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
    CHermitePath();
    ~CHermitePath();
    void makePathFromPathVector(const CPathVector* pPath);
    FLOAT getHermiteCoefficient(INT iBlock, FLOAT fProgress);
    void getNowPos(FVECTOR& vPos);
    void getNowRot(FVECTOR& vRot, FLOAT fRate);
    FLOAT getNowRoll();
    void setSpeed(FLOAT fSpeed);
    void update(FLOAT fr);
    // PARTIAL: added for System/Graphic/HermitePathCamera
    FLOAT getHermiteDistanceAll();
    void goStartPos();
    void goEndPos();
    BOOL isEnd();
    FLOAT getSpeed() { return m_fSpeed; }
    void setDataSpeed() { m_fDataSpeed = m_fSpeed; }
    INT getNowBlock() { return m_iNowBlock; }
    FLOAT getProgress() { return m_fProgress; }
    CPathVector* getPathVector() { return m_pPathVect; }
};

#endif /* SYSTEM_GRAPHIC_HERMITEPATH_H */
