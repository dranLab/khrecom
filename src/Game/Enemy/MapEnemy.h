
#ifndef GAME_ENEMY_MAPENEMY_H
#define GAME_ENEMY_MAPENEMY_H

#include "khtypes.h"
/* MATCH: the .rodata of Game/Enemy/MapEnemyManager and Game/Enemy/CreateEnemy has no BodyCollision.h/Anim.h
 * strings: they define MAPENEMY_H_NO_COLL_ANIM before including to leave these two out. */
#ifndef MAPENEMY_H_NO_COLL_ANIM
#include "Game/BodyCollision.h"
#include "System/Graphic/Anim.h"
#endif
#include "Game/Entity.h"
#include "System/Vector.h"

class CState;
struct ENEMY_PARAM;

/* Game/Enemy/MapEnemy is the key unit: its inline functions are emitted there, in this order. */
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


#endif /* GAME_ENEMY_MAPENEMY_H */
