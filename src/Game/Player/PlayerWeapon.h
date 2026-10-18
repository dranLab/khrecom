#ifndef GAME_PLAYER_PLAYERWEAPON_H
#define GAME_PLAYER_PLAYERWEAPON_H

#include "khtypes.h"
#include "System/Vector.h"
#include "Game/Entity.h"
#include "Game/Collision/CollAttack.h"
#include "Game/Bullet/Bullet.h" // BULLET_EFFECT_INFO

class CPlayer;
class CCollMap;

namespace PlayerWeapon {

class CPlayerWeapon : public CEntity {
private:
    CPlayer* m_pPlayer;        /* 0x1d0 */
    INT m_iBoneID;             /* 0x1d4 */
    UINT m_uiAttackID;         /* 0x1d8 */
    CCollAttack m_collAttack;  /* 0x1e0 */
    CCollMap* m_pCollMap;      /* 0x250 */
    INT m_iHit;                /* 0x254 */
    INT m_uiFlag;              /* 0x258 (UINT in the original; signed masks match) */
    UINT m_nHitEffWall;        /* 0x25c */
    FVECTOR m_vFreePos;        /* 0x260 */
    HANDLE m_hEnemyHandle;     /* 0x270 */
    UINT m_uiDoubleAttackID;   /* 0x274 */

public:
    CPlayerWeapon(CTask* inParent, INT inID, INT inBone);

private:
    void setCollMapPos_toAttackID();

public:
    virtual ~CPlayerWeapon();

protected:
    virtual void stateInitial();
    virtual void updateBefore();
    virtual void updateAfter();

public:
    FMATRIX& getMatrixWep0();
    void onAttack(INT inID, INT inAP, INT inKind, INT inAttr);
    void offAttack();
    INT isHit() { return m_iHit; }
    INT isOnAttack() { return m_uiAttackID; }
    void onWeapon(INT inEffect);
    void offWeapon();
    void updateAttackID();
    INT getCollAttackID();
    void setCollAttackID(UINT inID);
    void setWideAttack(INT inFlag);
    void setBackAttack(INT inFlag);
    void killEffect(FLOAT inFadeTime);
    HANDLE getEnemyHandle() { return m_hEnemyHandle; }
    void getEnemyPos(FVECTOR& outPos);
    void setFreePos(const FVECTOR& inPos) { m_vFreePos = inPos; }
    void setFreeMode(INT inFlag, INT inAnim, FVECTOR* inPos, FVECTOR* inRot);
    void setDoubleHit(INT inFlag, const FVECTOR* inPos);
};

extern FVECTOR g_fWeaponTopOffset;

} // namespace PlayerWeapon

#endif /* GAME_PLAYER_PLAYERWEAPON_H */
