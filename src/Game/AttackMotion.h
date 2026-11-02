// PARTIAL: added for Game/Gimmick/GimmickToldPipe
#ifndef GAME_ATTACKMOTION_H
#define GAME_ATTACKMOTION_H

#include "khtypes.h"
#include "System/Vector.h"           // PARTIAL: added for Game/AttackMotion (CAttackMotionCtrl)
#include "Game/CollParam.h"           // ATK_KIND
#include "Game/Collision/CollObj.h"   // COLL_KIND

union FVECTOR; // PARTIAL: added for Game/Player/FriendBeast
class CEntity; // PARTIAL: added for Game/Gimmick/BaseGimmick

// PARTIAL: added for Game/Gimmick/GimmickGuillotine
enum ATK_ATTR {
    ATK_ATTR_PHYSICAL = 0,
    ATK_ATTR_NONE = 1,
    ATK_ATTR_FIRE = 2,
    ATK_ATTR_ICE = 3,
    ATK_ATTR_THUNDER = 4,
    ATK_ATTR_SPECIAL = 5,
    ATK_ATTR_MAX = 6
};

// PARTIAL: CAttachMotion moved to Game/AttachMotion.h for Game/EffectMotion
#include "Game/AttachMotion.h"

class CCollAttack;

class CAttackMotionCtrl {
private:
    FVECTOR m_vEnemyPos;                /* 0x00 */
    UINT m_uiFlag;                      /* 0x10 */
    INT m_iStep;                        /* 0x14 */
    ATK_COLL_PARAM* m_pNowAtkCollParam; /* 0x18 */
    CCollAttack* m_pCollAttack;         /* 0x1c */
    CEntity* m_pOwner;                  /* 0x20 */
    HANDLE m_hEnemyHandle;              /* 0x24 */
    COLL_KIND m_CollKind;               /* 0x28 */

public:
    CAttackMotionCtrl();
    ~CAttackMotionCtrl();
    BOOL initialize(CEntity* inOwner, COLL_KIND inCollKind);
    void set(const ATK_COLL_PARAM* inAtkCollParam);
    void update(TIMEQ inCurrentTime);
    INT test();
    void release();
    void setAttackParam(INT inAttack, ATK_ATTR inAttr);
    void setAttackKind(ATK_KIND inAtkKind);
    void setAttackID(UINT inAttackID);
    void setRadius(FLOAT inRadius);
    void setCollKind(INT inCollKind);
    void setEnemyHandle(HANDLE inHandle) { m_hEnemyHandle = inHandle; }
    void setEnemyPos(const FVECTOR& inPos) {
        m_uiFlag |= 1;
        m_vEnemyPos = inPos;
    }
    FVECTOR* getDetectPos();
    UINT getAttackID() const;
    ATK_COLL_PARAM* getAtkCollParam() { return m_pNowAtkCollParam; }
    ATK_KIND getAtkKind() const;
    HANDLE setHitEffect();
    HANDLE playHitSE();
    void updateAttackID();

private:
    void computeOffset_();
    CEntity* getParent_(CEntity* inChild);
    UINT isEnemyPos_() { return m_uiFlag & 1; }
};

class CAttackMotion : public CAttachMotion {
private:
    INT m_iAtkMotCtrlCount;           /* 0x08 */
    INT m_iNowCtrlCount;              /* 0x0c */
    CAttackMotionCtrl* m_pAtkMotCtrl; /* 0x10 */
    INT m_iAtkIndex;                  /* 0x14 */
    INT m_iAtkCollParamNum;           /* 0x18 */
    ATK_COLL_PARAM* m_pAtkCollParam;  /* 0x1c */
    UINT m_uiAttackID;                /* 0x20 */

public:
    /* The inline members are emitted (weak) in Game/AttackMotion.cpp, in this order: keep it. */
    CAttackMotion(CEntity* inOwner);
    virtual ~CAttackMotion();
    BOOL initialize(ATK_COLL_PARAM* inAtkCollParam, INT inCount);
    virtual BOOL set(INT inIndex);
    BOOL set(INT inIndex, INT inActNo);
    void setAttackParam(INT inValue, ATK_ATTR inAttr);
    void setAttackParamForBattleEnemy();
    void setAttackParamForBattleEnemy(INT inActNo);
    void setAttackKind(ATK_KIND inAtkKind);
    void setRadius(FLOAT inRadius);
    void setCollKind(INT inCollKind);
    void setEnemyHandle(HANDLE inHandle);
    void setEnemyPos(const FVECTOR& inPos);
    virtual void stop();
    virtual void update();
    INT test();
    BOOL hasAtkCollParam() const {
        if (!m_pAtkMotCtrl) return FALSE;
        return m_pAtkCollParam != NULL;
    }
    INT getAtkIndex() const { return m_iAtkIndex; }
    void updateAttackID();
    void uniformAttackID();
    void enbaleAtkCollParam(INT inIndex, BOOL inSw);
    void setAtkCollParamFlag(INT inIndex, UINT inSet, UINT inMask);
    BOOL checkOnCollisionAtkKind(ATK_KIND inAtkKind);
};

#endif /* GAME_ATTACKMOTION_H */
