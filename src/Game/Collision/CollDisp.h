#ifndef GAME_COLLISION_COLLDISP_H
#define GAME_COLLISION_COLLDISP_H

#include "khtypes.h"
#include "System/MainApp.h"
#include "System/HeapMan.h"

#ifndef COLLDISP_H_PATH /* MATCH: CollDisp, GameSubField, GameSubBattle and Entity name it "c:/KHCOM/MainApp/Game/Collision/CollDisp.h" */
#define COLLDISP_H_PATH "Game/Collision/CollDisp.h"
#endif
#include "System/Task.h"
#include "System/Vector.h"
class CCollisionList;
class CCoord;
/* MATCH: operator new for CCollDisp::initialize's allocation, declared only: the includers define it themselves */
inline void* operator new(size_t size, char* msg) throw();

class CCollObj;
class C3DCPrim;
struct MCD_INFO;
class CRideModel;


class CCollDispMapZone;

class CCollDisp : public CTaskSecondary {
    friend void _setBodyLines(CCollDisp* pThis, class CCollisionList* pList); /* MATCH: data placeholder of the parked setBodyLines */
public:
    static CCollDisp* m_pInstance;
    static INT m_fgDisp;
    static UINT m_fgMapZoneDisp;
    static SHORT m_bMapZoneColR;
    static SHORT m_bMapZoneColG;
    static SHORT m_bMapZoneColB;
    static SHORT m_bMapZoneColA;

private:
    FVECTOR m_fvLookPos;          /* 0x40 */
    C3DCPrim* m_pLines;           /* 0x50 */
    SHORT m_nDispAtkNum;          /* 0x54 */
    SHORT m_nDispBodyNum;         /* 0x56 */
    CCollDispMapZone* m_pMapZone; /* 0x58 */

public:
    CCollDisp(CTask* pInParent, INT fgDisp, INT inMapZoneDisp);
    virtual ~CCollDisp();
#line 47 COLLDISP_H_PATH
    static CCollDisp& initialize(CTask* pInParent, INT fgDisp, INT inMapZoneDisp) {
        KH_DEBUG(if (!(__null == m_pInstance)) AssertApp(__FILE__, __LINE__, "__null == m_pInstance", 0);)
        m_pInstance = new KH_AT("(49)") CCollDisp(pInParent, fgDisp, inMapZoneDisp);
        KH_DEBUG(if (!(m_pInstance)) AssertApp(__FILE__, __LINE__, "m_pInstance", 0);)
        return *m_pInstance;
    }
    static void finalize() {
        if (m_pInstance) delete m_pInstance;
        m_pInstance = __null;
    }
    virtual void main();
    virtual void disp();

private:
    void init();
    void setBodyLines(CCollisionList* pList);
    void setAttackLines(CCollisionList* pList);
    BOOL checkRange(const FVECTOR& refPos);
    void drawSphere(C3DCPrim& refPrim, const FVECTOR& refPos, FLOAT infShiftY, FLOAT infRadius, UINT inCol);
    void drawColumn(C3DCPrim& refPrim, const FVECTOR& refPos1, const FVECTOR& refPos2, FLOAT infShiftY, FLOAT infRadius,
                    UINT inCol);
    void drawCapsule(C3DCPrim& refPrim, const FVECTOR& refPos1, const FVECTOR& refPos2, FLOAT infShiftY,
                     FLOAT infRadius, UINT inCol);
    void setBBMatrix(CCoord* inpCrd, const FVECTOR& inrefPos, FLOAT infShiftY);
    FLOAT setColmMatrix(CCoord* inpCrd, const FVECTOR& refPos1, const FVECTOR& refPos2, FLOAT infShiftY);
};

class CCollDispMapZone : public CTaskSecondary {
private:
    C3DCPrim* m_pLine;  /* 0x40 */
    C3DCPrim* m_pPoly;   /* 0x44 */
    MCD_INFO* m_pInfo;   /* 0x48 */
    INT m_iChipNum;      /* 0x4c */
    UINT m_uiLineCol;    /* 0x50 */
    UINT m_uiQuitReq;    /* 0x54 */

public:
    CCollDispMapZone(CTask* inParentObj, MCD_INFO* inInfo);
    virtual ~CCollDispMapZone();
    void QuitReq();
    void changeLineColor(UINT inRGBA);
    UINT getLineColor() const { return m_uiLineCol; }
    virtual void main();
    virtual void disp();

private:
    void _makeModelPrim();
    void _updateModelPrim();
};

class CCollRideModelDisp : private CTaskSecondary {
private:
    C3DCPrim* m_pLine;         /* 0x40 */
    C3DCPrim* m_pPoly;         /* 0x44 */
    CRideModel* m_pRideModel;  /* 0x48 */
    UINT m_uiStateFlag;        /* 0x4c */
    MCD_INFO* m_pInfo;         /* 0x50 */
    INT m_iChipNum;            /* 0x54 */
    UINT m_uiLineCol;          /* 0x58 */

public:
    CCollRideModelDisp(CTask* inBo, CRideModel* inRideModel, BOOL inAutoUpdate);
    virtual ~CCollRideModelDisp();
    virtual void main();
    virtual void disp();
    void updateModelPrim();

private:
    void _makeModelPrim();
};


#endif /* GAME_COLLISION_COLLDISP_H */
