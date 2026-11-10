#ifndef GAME_COLLISION_COLLATTACK_H
#define GAME_COLLISION_COLLATTACK_H

#include "khtypes.h"
#include <string.h>
#include "Game/Collision/CollObj.h"
#include "Game/CollParam.h"                  // ATK_KIND
#include "Game/AttackMotion.h"               // ATK_ATTR
#include "System/Graphic/Effect/EffectMan.h" // ObjType::eObjType

class CCollBody;

namespace CollAttack {
extern FLOAT DelayHitEffectFrame;
}

/* All inline members are emitted (weak) in CollAttack.cpp, in declaration order: keep the order. */
class CCollAttack : public CCollObj {
private:
    char m_strHitEffectName[16];            /* 0x14 */
    INT m_iHitEffectNo;                     /* 0x24 */
    ObjType::eObjType m_HitEffectObjType;   /* 0x28 */
    FLOAT m_fHitEffectWait;                 /* 0x2c */
    ATK_KIND m_AttackKind;                  /* 0x30 */
    SHORT m_Attack;                         /* 0x34 */
    SHORT m_Critical;                       /* 0x36 */
    SHORT m_BackAttack;                     /* 0x38 */
    SHORT dum0;                             /* 0x3a */
    ATK_ATTR m_AttackAttr;                  /* 0x3c */
    HANDLE m_hEnemyTask;                    /* 0x40 */
    FVECTOR m_vEnemyPos;                    /* 0x50 */
    UINT m_uiAttackID;                      /* 0x60 */
    static UINT m_uiID;

public:
    CCollAttack() {
        m_iHitEffectNo = -1;
        m_Attack = 1;
        m_fHitEffectWait = 0.0f;
        m_AttackKind = (ATK_KIND)0;
        m_Critical = 0;
        m_BackAttack = 0;
        m_uiAttackID = 1;
        strcpy(m_strHitEffectName, "");
    }
    virtual ~CCollAttack();
    virtual INT collideAttack();
    virtual BOOL collideAttackFunc(CCollBody* pEntry);
    virtual void setKind(COLL_KIND collKind) {
        collKind = (COLL_KIND)(collKind | 1);
        if (m_pCollision) m_pCollision->m_Kind = collKind;
    }
    virtual void init(COLL_KIND collKind, COLL_TYPE collType, HANDLE hTask, INT fgStatus);
    INT attackSub(class CCollisionList* pList);
    void setAttackKind(ATK_KIND attackKind) { m_AttackKind = attackKind; }
    void setAttackAttr(ATK_ATTR attackAttr) { m_AttackAttr = attackAttr; }
    void setAttack(INT attack, INT critical) {
        m_Attack = attack;
        m_Critical = critical;
    }
    void setAttackID(UINT attackID) { m_uiAttackID = attackID; }
    INT getAttack() { return m_Attack; }
    void setCritical(INT critical) { m_Critical = critical; }
    void setBackAttack(INT backattack) { m_BackAttack = backattack; }
    ATK_KIND getAttackKind() { return m_AttackKind; }
    void setBasePos(FVECTOR& vBasePos);
    FVECTOR* getBasePos();
    BOOL chkGuardSuccess(CCollBody* pBody);
    BOOL chkMutekiSuccess(CCollBody* pBody);
    INT chkBackAttack(CCollBody* pBody);
    void setEnemyPos(const FVECTOR& inPos) { m_vEnemyPos = inPos; }
    const FVECTOR& getEnemyPos() const { return m_vEnemyPos; }
    HANDLE getEnemyHandle() { return m_hEnemyTask; }
    void setEnemyHandle(HANDLE hEnemyTask) { m_hEnemyTask = hEnemyTask; }
    void initEnemyPos() {
        m_vEnemyPos.init0();
        m_vEnemyPos.w = 1.0f;
    }
    void updateAttackID();
    UINT getAttackID() const { return m_uiAttackID; }
    ATK_ATTR getAttackAttr() const { return m_AttackAttr; }
    void setHitEffectParam(const CHAR* inName, INT inNo, ObjType::eObjType inType);
    const CHAR* getHitEffectName() const { return m_strHitEffectName; }
    INT getHitEffectNo() const { return m_iHitEffectNo; }
    ObjType::eObjType getHitEffectObjType() const { return m_HitEffectObjType; }
    FLOAT getHitEffectWait() const { return m_fHitEffectWait; }
};

#endif /* GAME_COLLISION_COLLATTACK_H */
