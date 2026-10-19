// PARTIAL: added for Game/Debug/KisEnemy
#ifndef GAME_COLLISION_COLLDISP_H
#define GAME_COLLISION_COLLDISP_H

#include "khtypes.h"
#include "System/MainApp.h"
#include "System/HeapMan.h"

/* Placeholders for CCollDisp's inline singleton constructor/creation: their strings
 * ("Game/Collision/CollDisp.h", "__null == m_pInstance", "Game/Collision/CollDisp.h(49)", "m_pInstance")
 * are in .rodata of every includer. Only line 49 is known (it is part of the allocation message). */
/* PARTIAL: path override added for Game/GameSubField ("c:/KHCOM/MainApp/Game/Collision/CollDisp.h") */
#ifndef COLLDISP_H_PATH
#define COLLDISP_H_PATH "Game/Collision/CollDisp.h"
#endif
/* Game/Collision/CollDisp.cpp defines the full classes and the real inline functions itself. */
#ifndef COLLDISP_CPP
#line 1 COLLDISP_H_PATH
inline void _collDispCheckInstance(void* inInstance) {
    if (!(__null == inInstance)) AssertApp(__FILE__, __LINE__, "__null == m_pInstance", 0);
}
inline void* _collDispCreate() {
    void* p = com_malloc_d(4, __FILE__ "(49)", (UINT)-1);
    if (!p) AssertApp(__FILE__, __LINE__, "m_pInstance", 0);
    return p;
}

// PARTIAL: added for Game/Bullet/Bullet
class CTask;
class CCollDisp {
private:
    UBYTE _pad[0x60]; /* PARTIAL: the original derives from CTask (size 0x60) */
public:
    // PARTIAL: added for Game/GameSubField (create/destroy are inline in the original)
    CCollDisp(CTask* pParent, INT nDisp, INT nMapZoneDisp);
    static CCollDisp* m_pInstance;
    static BOOL m_fgDisp;
    // PARTIAL: added for Form/DebugWindowTBL
    static UINT m_fgMapZoneDisp;
    static SHORT m_bMapZoneColR;
    static SHORT m_bMapZoneColG;
    static SHORT m_bMapZoneColB;
};

#else /* COLLDISP_CPP */

class CCollObj;
class C3DCPrim;
struct MCD_INFO;
class CRideModel;


class CCollDispMapZone;

class CCollDisp : public CTaskSecondary {
    friend void _setBodyLines(CCollDisp* pThis, class CCollisionList* pList);
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
        if (!(__null == m_pInstance)) AssertApp(__FILE__, __LINE__, "__null == m_pInstance", 0);
        m_pInstance = new (__FILE__ "(49)") CCollDisp(pInParent, fgDisp, inMapZoneDisp);
        if (!(m_pInstance)) AssertApp(__FILE__, __LINE__, "m_pInstance", 0);
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
    UINT m_fgQuit;       /* 0x54 */

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


#endif /* COLLDISP_CPP */

#endif /* GAME_COLLISION_COLLDISP_H */
