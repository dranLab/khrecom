// PARTIAL: added for Game/Enemy/MapEnemyShadow
#ifndef GAME_ENEMY_MAPENEMY_H
#define GAME_ENEMY_MAPENEMY_H

#include "khtypes.h"
/* PARTIAL: added for Game/Enemy/MapEnemyManager - its .rodata has no BodyCollision.h/Anim.h strings: define
 * MAPENEMY_H_NO_COLL_ANIM before including to leave these two out. */
#ifndef MAPENEMY_H_NO_COLL_ANIM
#include "Game/BodyCollision.h"
#include "System/Graphic/Anim.h"
#endif
#include "Game/Entity.h"
#include "System/Vector.h"

class CState;
struct ENEMY_PARAM;

#ifdef MAPENEMY_H_FULL_CLASS
/* The full class, for Game/Enemy/MapEnemy (the key unit: its inline functions are emitted there, in this order). */
#include "System/Fsm.h"

class CMapEnemy : public CEntity {
protected:
    FVECTOR m_vTarget;           /* 0x1d0 */
    ENEMY_PARAM* m_pEnemyParam;  /* 0x1e0 */
    FLOAT m_fWalkSpeed;          /* 0x1e4 */
    FLOAT m_fRunSpeed;           /* 0x1e8 */
    FLOAT m_fViewAngle;          /* 0x1ec */
    FLOAT m_fViewArea;           /* 0x1f0 */
    INT m_iAppearType;           /* 0x1f4 */
private:
    UINT m_uiFlag;               /* 0x1f8 */
    INT m_iSysStep;              /* 0x1fc */
    INT m_iUniqueID;             /* 0x200 */
    INT m_iMapEnemyParam;        /* 0x204 */
    FLOAT m_fMotionSpeed;        /* 0x208 */
    HANDLE m_hBadStatus;         /* 0x20c */
    CState* m_pState;            /* 0x210 */
    CStateTemplate<CMapEnemy> m_MapEnemyState[9]; /* 0x214 */
    static void (CMapEnemy::*m_pBeginStateTbl[])();
    static void (CMapEnemy::*m_pStateTbl[])();
    static void (CMapEnemy::*m_pEndStateTbl[])();

public:
    CMapEnemy(CTask* inParent, CState* inState = __null);
    virtual ~CMapEnemy();
    virtual INT getTypeID() const;
    virtual BOOL initalize(INT inGroupID, INT inRsrcID);
    virtual void onMsg(INT inMsg, void* inExtraInfo);
    void setUniqueID(INT inID) { m_iUniqueID = inID; }
    void setMapEnemyFlag(UINT inSet, UINT inMask) {
        UINT& f = m_uiFlag;
        f = (f | inSet) & ~inMask;
    }
    void setMapEnemyParam(INT inParam) { m_iMapEnemyParam = inParam; }
    INT getUniqueID() const { return m_iUniqueID; }
    UINT isAppear() const { return m_uiFlag & 1; }
    UINT isObjTouch() const { return m_uiFlag & 2; }
    UINT isPlayerFind() const { return m_uiFlag & 4; }
    UINT isLoomingDarkness() const { return m_uiFlag & 8; }
    UINT isFaint() const { return m_uiFlag & 0x10; }
    UINT isStop() const { return m_uiFlag & 0x20; }
    UINT isHpHalf() const { return m_uiFlag & 0x40; }
    UINT isConfuse() const { return m_uiFlag & 0x80; }
    UINT isNormal() const { return m_uiFlag & 0x20000000; }
    UINT isSleeping() const { return m_uiFlag & 0x40000000; }
    UINT isStanding() const { return m_uiFlag & 0x80000000; }
    BOOL isCurrentState(INT inState);
    void updateBodyColl();
    static FLOAT getAppearDistance();
    static FLOAT getDisappearDistance();
    static TIMEQ getFaintTime();
    static TIMEQ getStopTime();
    static TIMEQ getConfuseTime();

protected:
    INT getIdlingMotion() const;
    INT getWalkMotion() const;
    INT getRunMotion() const;
    void changeMapEnemyMotion(INT inMotionType, TIMEQ inTq, BOOL inRootMove, TIMEQ inGr);
    virtual void updateBefore();
    virtual void lastUpdate();
    void changeState(INT inState);
    void changeMapEnemyState(INT inState);
    BOOL checkWall(FVECTOR& outRefVec);
    BOOL checkGround();
    BOOL canAppear() const;
    BOOL canPlayerChase(FLOAT inHeight, BOOL inGround);
    void getRandomTargetPos(FVECTOR& outTarget, FLOAT inDistance, BOOL inGround);
    void getWarpPos(FVECTOR& outPos);
    virtual void beginStateAppear();
    virtual void stateAppear();
    virtual void endStateAppear();
    virtual void beginStateMain();
    virtual void stateMain();
    virtual void endStateMain();
    virtual void beginStateSleeping();
    virtual void stateSleeping();
    virtual void endStateSleeping();
    virtual void beginStateStanding();
    virtual void stateStanding();
    virtual void endStateStanding();
    virtual void beginStateDamage();
    virtual void stateDamage();
    virtual void endStateDamage();
    virtual void beginStateFaint();
    virtual void stateFaint();
    virtual void endStateFaint();
    virtual void beginStateStop();
    virtual void stateStop();
    virtual void endStateStop();
    virtual void beginStateConfuse();
    virtual void stateConfuse();
    virtual void endStateConfuse();
    virtual void beginStateDisappear();
    virtual void stateDisappear();
    virtual void endStateDisappear();

private:
    void groundEnemyProc_();
    void flyEnemyProc_();
    BOOL hitTest_();
    BOOL searchGround_(FLOAT inHeight, FVECTOR* outPos);
    BOOL initEnemyParam_(INT inGroupID, INT inRsrcID);
    BOOL checkEncont_();
    BOOL encount_(UBYTE inKind);
};

#else
class CMapEnemy : public CEntity {
protected:
    FVECTOR m_vTarget;           /* 0x1d0 */
    ENEMY_PARAM* m_pEnemyParam;  /* 0x1e0 */
    FLOAT m_fWalkSpeed;          /* 0x1e4 */
    FLOAT m_fRunSpeed;           /* 0x1e8 */
    FLOAT m_fViewAngle;          /* 0x1ec */
    FLOAT m_fViewArea;           /* 0x1f0 */
    INT m_iAppearType;           /* 0x1f4 */
private:
    UINT m_uiFlag;               /* 0x1f8 */
    INT m_iSysStep;              /* 0x1fc */ // PARTIAL: m_iSysStep, m_iUniqueID split out for Game/Enemy/MapEnemyManager
    INT m_iUniqueID;             /* 0x200 */
    UBYTE _m_0x204[0x13c];       /* 0x204 m_iMapEnemyParam .. m_MapEnemyState[9] */

public:
    CMapEnemy(CTask* inParent, CState* inState = __null);
    virtual ~CMapEnemy();
    virtual INT getTypeID() const;
    virtual BOOL initalize(INT inGroupID, INT inRsrcID);
    virtual void onMsg(INT inMsg, void* inExtraInfo);
    /* (the original is setMapEnemyFlag(UINT inSet, UINT inMask), "or inSet, then clear inMask": see the full class
     * above; this form gives the same code for the constant arguments of its callers) */
    void setMapEnemyFlag(UINT inFlag, BOOL inOn) {
        if (inOn) m_uiFlag |= inFlag;
        else m_uiFlag &= ~inFlag;
    }
    void setUniqueID(INT inID) { m_iUniqueID = inID; } // PARTIAL: added for Game/Enemy/MapEnemyManager
    void setParam(INT inParam) { *(INT*)&_m_0x204[0] = inParam; } /* m_iMapEnemyParam 0x204 */ // PARTIAL: added for Game/Enemy/CreateEnemy
    void updateBodyColl(); // PARTIAL: added for Game/Enemy/CreateEnemy
    UINT isAppear() const { return m_uiFlag & 1; }      // PARTIAL: added for Game/Enemy/MapEnemyManager
    UINT isMapEnemyFlag(UINT inFlag) const { return m_uiFlag & inFlag; } // PARTIAL: added for Game/Enemy/MapEnemyAir (name unknown)
    static FLOAT getAppearDistance();
    static FLOAT getDisappearDistance();

protected:
    void changeMapEnemyMotion(INT inMotion, INT inArg1, INT inArg2, INT inArg3);
    virtual void updateBefore();
    virtual void lastUpdate();
    void changeMapEnemyState(INT inState);
    BOOL checkGround();
    BOOL canAppear() const;
    void getWarpPos(FVECTOR& outPos); // PARTIAL: added for Game/Enemy/MapEnemyMushroom
    BOOL canPlayerChase(FLOAT inRange, INT inArg);
    void getRandomTargetPos(FVECTOR& outPos, FLOAT inRange, INT inArg);
    INT getRunMotion() const; // PARTIAL: added for Game/Enemy/MapEnemyGround
    virtual void beginStateAppear();
    virtual void stateAppear();
    virtual void endStateAppear();
    virtual void beginStateMain();
    virtual void stateMain();
    virtual void endStateMain();
    virtual void beginStateSleeping();
    virtual void stateSleeping();
    virtual void endStateSleeping();
    virtual void beginStateStanding();
    virtual void stateStanding();
    virtual void endStateStanding();
    virtual void beginStateDamage();
    virtual void stateDamage();
    virtual void endStateDamage();
    virtual void beginStateFaint();
    virtual void stateFaint();
    virtual void endStateFaint();
    virtual void beginStateStop();
    virtual void stateStop();
    virtual void endStateStop();
    virtual void beginStateConfuse();
    virtual void stateConfuse();
    virtual void endStateConfuse();
    virtual void beginStateDisappear();
    virtual void stateDisappear();
    virtual void endStateDisappear();
};

#endif /* MAPENEMY_H_FULL_CLASS */

#endif /* GAME_ENEMY_MAPENEMY_H */
