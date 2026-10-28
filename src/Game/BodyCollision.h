#ifndef GAME_BODYCOLLISION_H
#define GAME_BODYCOLLISION_H

#include "khtypes.h"
#include "System/MainApp.h"
#include "Game/CollParam.h"

union FVECTOR; // PARTIAL: added for Game/Gimmick/BaseGimmick
class CEntity;

/* PARTIAL: includers without Game/Collision/CollBody.h (the full class) get this partial copy (layouts partial) */
#ifndef GAME_COLLISION_COLLBODY_H
class CCollBody {
private:
    UBYTE _m_0x00[0x0c];   /* 0x000 CCollObj: CSJointT, m_Enable */
    UBYTE* m_pCollision;   /* 0x00c CCollision* */ // PARTIAL: split out for Game/Enemy/EnemyWhitemushroom
    UBYTE _m_0x10[0x60];   /* 0x010 m_hTask, m_vEnemyPos .. m_vHitEffectRot */
    ATK_KIND m_DamageKind; /* 0x070 */
    INT m_DamageAttr;      /* 0x074 ATK_ATTR */ // PARTIAL: added for Game/Enemy/EnemyWhitemushroom
    UBYTE _m_0x78[0x10];   /* 0x078 .. */
    HANDLE m_hEnemyTask;   /* 0x088 */ // PARTIAL: added for Game/Enemy/BattleEnemy
    UBYTE _m_0x8c[0x74];   /* 0x08c .. */
    FLOAT m_fBadStatusParam; /* 0x100 */ // PARTIAL: added for Game/Enemy/EnemyCreapplant (name unknown)
public:
    FLOAT getBadStatusParam() { return m_fBadStatusParam; } // PARTIAL: added for Game/Enemy/EnemyCreapplant (name unknown)
    ATK_KIND getDamageKind() { return m_DamageKind; }
    HANDLE getEnemyHandle() { return m_hEnemyTask; } // PARTIAL: added for Game/Enemy/BattleEnemy
    const FVECTOR& getEnemyPos() const { return *(const FVECTOR*)&_m_0x10[0x10]; } // PARTIAL: added for Game/Enemy/BattleEnemy (m_vEnemyPos)
    FVECTOR* getStartPos() { return (FVECTOR*)m_pCollision; } // PARTIAL: added for Game/Enemy/BattleEnemy
    const CHAR* getHitEffectName() const { return (const CHAR*)&_m_0x8c[0xe8 - 0x8c]; } // PARTIAL: added for Game/Enemy/BattleEnemy
    INT getHitEffectNo() const { return *(const INT*)&_m_0x8c[0xf8 - 0x8c]; }           // PARTIAL: added for Game/Enemy/BattleEnemy
    INT getHitEffectObjType() const { return *(const INT*)&_m_0x8c[0xfc - 0x8c]; }      // PARTIAL: added for Game/Enemy/BattleEnemy (ObjType::eObjType)
    INT getDamageAttr() { return m_DamageAttr; } // PARTIAL: added for Game/Enemy/EnemyWhitemushroom (name unknown)
    /* CCollObj::getDetectPos (m_pCollision->m_vDetectPos at 0x40) */
    FVECTOR* getDetectPos() { return m_pCollision ? (FVECTOR*)(m_pCollision + 0x40) : NULL; }
    HANDLE setHitEffect(); // PARTIAL: added for Game/Gimmick/BaseGimmick
};
#endif

/* __FILE__ of the original's inline functions: "c:/KHCOM/MainApp/Game/BodyCollision.h" in Game/BodyCollision */
#ifndef BODYCOLLISION_H_PATH
#define BODYCOLLISION_H_PATH "Game/BodyCollision.h"
#endif
#line 18 BODYCOLLISION_H_PATH
/* The inline members are emitted (weak) in Game/BodyCollision.cpp, in this order: keep it. */
class CBodyCollision {
private:
    UINT m_uiFlag;                      /* 0x00 */
    UINT m_uiSysBodyFlag[2];            /* 0x04 */
    UINT m_uiUserBodyFlag[2];           /* 0x0c */
    CEntity* m_pOwner;                  /* 0x14 */
    BODY_COLL_PARAM* m_pBodyCollParam;  /* 0x18 */
    FLOAT m_fRadiusScale;               /* 0x1c */
    INT m_iLastDamage;                  /* 0x20 */
    INT m_iCollBodyCount;               /* 0x24 */
    CCollBody* m_pCollBody;             /* 0x28 */
    INT m_iHitIndex;                    /* 0x2c */
    CCollBody* m_pHit;                  /* 0x30 */

    void setFlag(UINT inSet, UINT inMask) { m_uiFlag = (m_uiFlag | inSet) & ~inMask; }
    void setSysBodyFlag(INT inKind, UINT inSet, UINT inMask) {
        if (!(0 <= inKind && inKind < 2)) AssertApp(__FILE__, __LINE__, "0 <= inKind && inKind < COLLISION_KIND_MAX", 0);
        m_uiSysBodyFlag[inKind] = (m_uiSysBodyFlag[inKind] | inSet) & ~inMask;
    }

    void setUserBodyFlag(INT inKind, UINT inSet, UINT inMask) {
        if (!(0 <= inKind && inKind < 2)) AssertApp(__FILE__, __LINE__, "0 <= inKind && inKind < COLLISION_KIND_MAX", 0);
        m_uiUserBodyFlag[inKind] = (m_uiUserBodyFlag[inKind] | inSet) & ~inMask;
    }
    void setHitCollBody(CCollBody* inHit) { m_pHit = inHit; }
    void setHitCollBodyIndex(INT inIndex) { m_iHitIndex = inIndex; }

public:
    CBodyCollision(CEntity* inOwner);
    virtual ~CBodyCollision();
    BOOL initalize(BODY_COLL_PARAM* inBcp, INT inCount);
    void setGuard(INT inAttr);
    void setGuardDir(FLOAT inDir);
    void setMuteki(INT inAttr);
    void setMutekiDir(FLOAT inDir);
    void setRadiusScale(FLOAT inValue);
    INT getDamage() const { return m_iLastDamage; }
    CCollBody* getHitCollBody() { return m_pHit; }
    INT getHitCollBodyIndex() { return m_iHitIndex; }
    CCollBody* getCollBody(INT inIndex);
    FLOAT getRadius();
    FLOAT getLargeRadius();
    INT isHit() const { return m_uiFlag & 4; }
    INT isGuard() const { return m_uiFlag & 0x10; }
    INT isHitMuteki() const { return m_uiFlag & 8; }
    void update();
    void detect();
    void on(INT inIndex);
    void off(INT inIndex);
    void onBody(INT inIndex);
    void offBody(INT inIndex);
    void userOn(INT inIndex);
    void userOff(INT inIndex);
    void userOnBody(INT inIndex);
    void userOffBody(INT inIndex);
    BOOL collideBody(FVECTOR* outPos, FVECTOR* outNormal, FVECTOR* outDetectPos);
    BOOL collideBody(INT inTargetType, FVECTOR* outPos, FVECTOR* outNormal, FVECTOR* outDetectPos);
    void setHp(INT inHp, INT inIndex);
    void setMaxHp(INT inHp, INT inIndex);
    INT getHp(INT inIndex) const;
    INT getMaxHp(INT inIndex) const;
    UINT isOn(INT inIndex) const {
        UINT s = m_uiSysBodyFlag[0];
        UINT u = m_uiUserBodyFlag[0];
        UINT bit = 1 << inIndex;
        return u & s & bit;
    }
    UINT isBodyOn(INT inIndex) const {
        UINT s = m_uiSysBodyFlag[1];
        UINT u = m_uiUserBodyFlag[1];
        UINT bit = 1 << inIndex;
        return u & s & bit;
    }
    void clearHitCollBody() { m_pHit = 0; }

private:
    void cleanup_();
    void updateOffset_();
    BOOL collideBody_(FVECTOR* outPos, FVECTOR* outNormal, FVECTOR* outDetectPos, INT inTargetType);
};

#endif /* GAME_BODYCOLLISION_H */
