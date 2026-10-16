#ifndef GAME_COLLISION_COLLBODY_H
#define GAME_COLLISION_COLLBODY_H

#include "khtypes.h"
#include "System/Vector.h"
#include "Game/Collision/CollObj.h"
#include "Game/CollParam.h"
#include "Game/AttackMotion.h"
#include "System/Graphic/Effect/EffectMan.h"

/* stores the VU0's vf0 (0, 0, 0, 1) (original name unknown) */
static inline void _unitVector(FVECTOR& v) {
    asm volatile("sqc2 $vf0, 0x0(%0)" : : "r"(&v));
}

/* Game/BodyCollision.h has a partial copy of this class for its includers (PARTIAL). */
class CCollBody : public CCollObj {
private:
    FVECTOR m_vEnemyPos;          /* 0x020 */
    FVECTOR m_DamagePlane;        /* 0x030 */
    FVECTOR m_DamagePosS;         /* 0x040 */
    FVECTOR m_DamagePosE;         /* 0x050 */
    FVECTOR m_vHitEffectRot;      /* 0x060 */
    ATK_KIND m_DamageKind;        /* 0x070 */
    ATK_ATTR m_DamageAttr;        /* 0x074 */
    UINT m_GuardAttrFlag;         /* 0x078 */
    UINT m_MutekiAttrFlag;        /* 0x07c */
    SHORT m_fgHit;                /* 0x080 */
    SHORT m_Damage;               /* 0x082 */
    SHORT m_enableBody;           /* 0x084 */
    SHORT m_nPriority;            /* 0x086 */
    HANDLE m_hEnemyTask;          /* 0x088 */
    UINT m_uiFlag;                /* 0x08c */
    INT m_iHp;                    /* 0x090 */
    INT m_iMaxHp;                 /* 0x094 */
    FLOAT m_fDir;                 /* 0x098 */
    FLOAT m_fGuardDir;            /* 0x09c */
    FLOAT m_fMutekiDir;           /* 0x0a0 */
    INT m_iSaveAttackIDPtr;       /* 0x0a4 */
    UINT m_uiSaveAttackID[16];    /* 0x0a8 */
    CHAR m_strHitEffectName[16];  /* 0x0e8 */
    INT m_iHitEffectNo;           /* 0x0f8 */
    ObjType::eObjType m_HitEffectObjType; /* 0x0fc */
    FLOAT m_fHitEffectWait;       /* 0x100 */
    static CCollision m_collTmp;

public:
    CCollBody();
    virtual ~CCollBody();
    virtual void on() {
        m_Enable = 1;
        m_fgHit = 0;
        m_Damage = 0;
        setCritical(0);
    }
    void setDamage(INT damage) { m_Damage = damage; }
    INT getDamage();
    INT chkDamage() { return m_Damage; }
    void setDamageKind(ATK_KIND damageKind) { m_DamageKind = damageKind; }
    ATK_KIND getDamageKind() { return m_DamageKind; }
    void setDamageAttr(ATK_ATTR damageAttr) { m_DamageAttr = damageAttr; }
    ATK_ATTR getDamageAttr() { return m_DamageAttr; }
    void setDamagePlane(FVECTOR& inNorm) { m_DamagePlane = inNorm; }
    void setDamagePlane(FVECTOR& inPos0, FVECTOR& inPos1, FVECTOR& inPos2);
    void clearDamagePlane() { _unitVector(m_DamagePlane); }
    BOOL isDamagePlane() { return m_DamagePlane.x != 0.0f || m_DamagePlane.y != 0.0f || m_DamagePlane.z != 0.0f; }
    FVECTOR& getDamagePlane() { return m_DamagePlane; }
    void setDamagePos(FVECTOR& damegePosS, FVECTOR& damegePosE) {
        m_DamagePosS = damegePosS;
        m_DamagePosE = damegePosE;
    }
    FVECTOR& getDamagePosS() { return m_DamagePosS; }
    FVECTOR& getDamagePosE() { return m_DamagePosE; }
    INT collideBody(INT iTargetType);
    void setHit() { m_fgHit = 1; }
    INT isHit() {
        INT ret = m_fgHit;
        m_fgHit = 0;
        return ret;
    }
    INT checkDistancePlane();
    void onBody() { m_enableBody = 1; }
    void offBody() { m_enableBody = 0; }
    INT isEnableBody() { return m_enableBody; }
    void setPriority(SHORT priority) { m_nPriority = priority; }
    SHORT getPriority() { return m_nPriority; }
    HANDLE getEnemyHandle() { return m_hEnemyTask; }
    void setEnemyHandle(HANDLE hEnemyTask) { m_hEnemyTask = hEnemyTask; }
    void setFlag(UINT inSet, UINT inMask) { m_uiFlag = (m_uiFlag | inSet) & ~inMask; }
    INT isMuteki() { return m_uiFlag & 1; }
    INT isGuard() { return m_uiFlag & 2; }
    INT isFlip() { return m_uiFlag & 4; }
    void setEnemyPos(const FVECTOR& inPos) { m_vEnemyPos = inPos; }
    const FVECTOR& getEnemyPos() const { return m_vEnemyPos; }
    void initEnemyPos() {
        _unitVector(m_vEnemyPos);
        m_vEnemyPos.w = 1.0f;
    }
    void setDir(const FLOAT& inDir) { m_fDir = inDir; }
    FLOAT getDir() { return m_fDir; }
    void setGuardTrg(UINT inSet) { setFlag(inSet, inSet ^ 8); }
    INT isGuardTrg() { return m_uiFlag & 8; }
    void setCritical(UINT inSet) { setFlag(inSet, inSet ^ 0x10); }
    INT isCritical() { return m_uiFlag & 0x10; }
    void setMutekiTrg(UINT inSet) { setFlag(inSet, inSet ^ 0x20); }
    INT isMutekiTrg() { return m_uiFlag & 0x20; }
    void setAttackID(UINT inID);
    UINT getAttackID() const { return m_uiSaveAttackID[m_iSaveAttackIDPtr]; }
    BOOL checkAttackID(UINT inID);
    void setGuard(UINT inSet, INT inAttr) {
        setFlag(inSet, inSet ^ 2);
        m_GuardAttrFlag = (inAttr >= 0) ? (m_GuardAttrFlag | (1 << inAttr)) : 0;
    }
    UINT getGuardAttrFlag() const { return m_GuardAttrFlag; }
    void setGuardDir(FLOAT inDir) { m_fGuardDir = inDir; }
    FLOAT getGuardDir() const { return m_fGuardDir; }
    void setMuteki(UINT inSet, INT inAttr) {
        setFlag(inSet, inSet ^ 1);
        m_MutekiAttrFlag = (inAttr >= 0) ? (m_MutekiAttrFlag | (1 << inAttr)) : 0;
    }
    UINT getMutekiAttrFlag() const { return m_MutekiAttrFlag; }
    void setMutekiDir(FLOAT inDir) { m_fMutekiDir = inDir; }
    FLOAT getMutekiDir() const { return m_fMutekiDir; }
    void setHp(INT inHp) { m_iHp = inHp; }
    INT getHp() const { return m_iHp; }
    void setMaxHp(INT inHp) {
        m_iMaxHp = inHp;
        m_iHp = inHp;
    }
    INT getMaxHp() const { return m_iMaxHp; }
    virtual BOOL collideBodyFunc(CCollBody* pEntry);
    virtual void init(COLL_KIND collKind, COLL_TYPE collType, HANDLE hTask, INT fgStatus);
    void setHitEffectParam(const CHAR* inName, INT inNo, ObjType::eObjType inType, const FVECTOR& inRot, FLOAT inWait);
    HANDLE setHitEffect();
    const CHAR* getHitEffectName() const { return m_strHitEffectName; }
    INT getHitEffectNo() const { return m_iHitEffectNo; }
    ObjType::eObjType getHitEffectObjType() const { return m_HitEffectObjType; }
    FLOAT getHitEffectWait() const { return m_fHitEffectWait; }
};

#endif /* GAME_COLLISION_COLLBODY_H */
