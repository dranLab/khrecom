#ifndef GAME_GIMMICK_GIMMICKSEARCHLASER_H
#define GAME_GIMMICK_GIMMICKSEARCHLASER_H

#include "khtypes.h"
#include "System/Vector.h"
#include "Game/Gimmick/BaseGimmick.h"

class CGimmickSearchLaser : public CBaseGimmick {
private:
    CHAR m_Step;     /* 0x220 */
    CHAR m_Stat;     /* 0x221 */
    FLOAT m_Wait;    /* 0x224 */
    int m_hChild[4]; /* 0x228 */

public:
    CGimmickSearchLaser(CTask* inParent);
    virtual ~CGimmickSearchLaser();
    virtual BOOL initalize(INT inGroupID, INT inRsrcID);
    virtual void AfterInit();
    virtual void beginStateMain();
    virtual void stateMain();
    virtual void endStateMain();
    virtual void beginStateHitAttack();
    virtual void stateHitAttack();
    virtual void endStateHitAttack();
};

class CGimmickSearchLaserCircle : public CBaseGimmick {
private:
    CHAR m_Step;         /* 0x220 */
    CHAR m_Stat;         /* 0x221 */
    BYTE m_KillFlag;     /* 0x222 */
    BYTE m_MoveStep;     /* 0x223 */
    BYTE m_PlTarget;     /* 0x224 */
    FLOAT m_Wait;        /* 0x228 */
    FLOAT m_NextWait;    /* 0x22c */
    FLOAT m_Speed;       /* 0x230 */
    FVECTOR m_ParentPos; /* 0x240 */
    HANDLE m_hEfx;       /* 0x250 */
    FVECTOR m_TargetPos; /* 0x260 */

public:
    CGimmickSearchLaserCircle(CTask* inParent);
    virtual ~CGimmickSearchLaserCircle();
    virtual BOOL initalize(INT inGroupID, INT inRsrcID);
    BOOL changeStepAttack();
    /* Inline in the original (its weak copy follows killLaser). Defined out of class for now, so that the
     * still-assembly copies of CSphereCollision's inline functions can follow it; CGimmickSearchLaser::AfterInit
     * writes the member itself (as the inlined call did). */
    void setPearentPos(const FVECTOR& in_pos) { m_ParentPos = in_pos; }
    friend class CGimmickSearchLaser;
    BOOL followPlayer();
    BOOL setMoveTargetPos();
    BOOL moveTargetPos();
    void killLaser();
    virtual void AfterInit();
    virtual void beginStateMain();
    virtual void stateMain();
    virtual void endStateMain();
};

#endif /* GAME_GIMMICK_GIMMICKSEARCHLASER_H */
