
#ifndef GAME_ENEMY_HEARTLESS_H
#define GAME_ENEMY_HEARTLESS_H

#include "Game/Enemy/BattleEnemy.h"
#include "System/Vector.h"

class CHeartless : public CBattleEnemy {
protected:
    FVECTOR m_vAimPos;          /* 0x830 */
    INT m_iIdlingMotIndex;      /* 0x840 */
    INT m_iWalkMotIndex;        /* 0x844 */
    INT m_iWalkStartMotIndex;   /* 0x848 */
    INT m_iWalkEndMotIndex;     /* 0x84c */
    FLOAT m_fSpeed;             /* 0x850 */
    INT m_iWaitRate;            /* 0x854 */
    FLOAT m_fRandMin;           /* 0x858 */
    FLOAT m_fRandMax;           /* 0x85c */
    INT m_iStep;                /* 0x860 */
    INT m_iCount;               /* 0x864 */
    FLOAT m_fRot;               /* 0x868 */
    FLOAT m_fHomingDeg;         /* 0x86c */
private:
    UINT m_uiHeartlessFlag;     /* 0x870 */

public:
    CHeartless(CTask* inParent, CState* inDefaultState, CState* inMainState);
    virtual ~CHeartless();
    virtual BOOL initalize(INT inGroupID, INT inRsrcID, UINT iFlag);
    virtual BOOL initalize(INT inGroupID, INT inRsrcID) { return initalize(inGroupID, inRsrcID, 0); }
    virtual BOOL startCardBattle();
    void setHeartlessFlag(UINT inSet, UINT inMask) { m_uiHeartlessFlag = (m_uiHeartlessFlag | inSet) & ~inMask; }
    UINT isEnableRotMove() const { return m_uiHeartlessFlag & 1; }
    UINT isEnableEvade() const { return m_uiHeartlessFlag & 2; }
    UINT isEnableEvade2() const { return m_uiHeartlessFlag & 4; }
    UINT isEnableHitAway() const { return m_uiHeartlessFlag & 8; }
protected:
    virtual void aiCardBattle();
    virtual INT getDuelAttackState();
    void commonMoveInit(const CHAR* inIdling, const CHAR* inWalk, FLOAT inRand);
    void commonMove();
    void commonMoveEnd();
    void commonFlyInit(const CHAR* inIdling, const CHAR* inWalk, FLOAT inRandMin, FLOAT inRandMax);
    void commonFly();
    void commonFlyEnd();
    static void randAimPos(FVECTOR& outPos, FLOAT inRange);
    void randAimPosForFly();
    void computeRotMoveVelocity(FVECTOR& outVel, FLOAT inRot);
    BOOL checkBattleEnemyFlag(UINT inFlag);
    BOOL canRotMove();
    BOOL canEvade();
    BOOL canAttack(INT inIndex);
    BOOL canAttack();
    virtual INT randomBattleCardNumber() const;
    virtual INT getBattleCardNumber(INT inIndex) const;

private:
    void changeHeartlessMotion_(INT inIndex);
    void changeIdlingState_();
    void changeWalkState_();
    void changeRotMoveState_();
    INT getCardNumber_(INT inState) const;
};

#endif /* GAME_ENEMY_HEARTLESS_H */
