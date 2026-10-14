// PARTIAL: added for Game/Player/PlayerWeapon
#ifndef GAME_COLLISION_COLLATTACK_H
#define GAME_COLLISION_COLLATTACK_H

#include "khtypes.h"
#include <string.h>
#include "Game/Collision/CollObj.h"
#include "Game/Bullet/Bullet.h" // COLL_KIND
#include "System/Graphic/Effect/EffectMan.h" // ObjType::eObjType

class CCollAttack : public CCollObj {
private:
    char m_strHitEffectName[16];  /* 0x14 */
    INT m_iHitEffectNo;           /* 0x24 */
    INT m_HitEffectObjType;       /* 0x28 eObjType */
    FLOAT m_fHitEffectWait;       /* 0x2c */
    INT m_AttackKind;             /* 0x30 ATK_KIND */
    SHORT m_Attack;               /* 0x34 */
    SHORT m_Critical;             /* 0x36 */
    SHORT m_BackAttack;           /* 0x38 */
    SHORT dum0;                   /* 0x3a */
    INT m_AttackAttr;             /* 0x3c ATK_ATTR */
    HANDLE m_hEnemyTask;          /* 0x40 */
    FVECTOR m_vEnemyPos;          /* 0x50 */
    UINT m_uiAttackID;            /* 0x60 */

public:
    CCollAttack() {
        m_iHitEffectNo = -1;
        m_Attack = 1;
        m_fHitEffectWait = 0.0f;
        m_AttackKind = 0;
        m_Critical = 0;
        m_BackAttack = 0;
        m_uiAttackID = 1;
        strcpy(m_strHitEffectName, "");
    }
    virtual ~CCollAttack();
    virtual INT collideAttack();
    void init(COLL_KIND inKind, COLL_TYPE inType, INT inHandle, INT inFlag);
    void setBasePos(FVECTOR& inPos);
    FVECTOR* getBasePos();
    void updateAttackID();
    void setHitEffectParam(const CHAR* inName, INT inNo, ObjType::eObjType inType);

    void setAttack(INT inAttack, INT inCritical) {
        m_Attack = inAttack;
        m_Critical = inCritical;
    }
    INT getAttack() { return m_Attack; }
    void setBackAttack(INT inBackAttack) { m_BackAttack = inBackAttack; }
    INT getAttackKind() { return m_AttackKind; }
    INT getAttackAttr() const { return m_AttackAttr; }
    void setEnemyPos(const FVECTOR& inPos) { m_vEnemyPos = inPos; }
    HANDLE getEnemyHandle() { return m_hEnemyTask; }
    void setAttackID(UINT inID) { m_uiAttackID = inID; }
    UINT getAttackID() const { return m_uiAttackID; }
    void setAttackParam(INT attack, INT attackKind, INT attackAttr) {
        m_Attack = attack;
        m_AttackKind = attackKind;
        m_AttackAttr = attackAttr;
        m_Critical = 0;
        m_BackAttack = 0;
    }
};

#endif /* GAME_COLLISION_COLLATTACK_H */
