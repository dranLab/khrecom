
#ifndef GAME_BULLET_BULLET_H
#define GAME_BULLET_BULLET_H

#include "khtypes.h"
#include "System/Graphic/Gprim.h" // MATCH: here, not in Game/Entity.h: Game/Gimmick/GimmickHitMore's original lacks Gprim.h's strings
#include "Game/Entity.h"
#include "System/Vector.h"
#include "Game/Collision/CollObj.h"
#include "System/Fsm.h"


struct BULLET_EFFECT_INFO {
    CHAR* pEffectName;
    INT iNo;
    INT iFile;
    INT iID;
};

struct BULLET_EFFECT_SHADOW_INFO {
    CHAR* pEffectName;
    INT iNo;
};

struct BULLET_PARAM {
    INT iBulletID;          /* 0x00 */
    INT iGroupID;           /* 0x04 */
    INT iMemberID;          /* 0x08 */
    INT CollKind;           /* 0x0c COLL_KIND */
    FVECTOR vPos;           /* 0x10 */
    FVECTOR vRot;           /* 0x20 */
    FLOAT fSpeed;           /* 0x30 */
    FLOAT fMaxDist;         /* 0x34 */
    TIMEQ tqExplodeTime;    /* 0x38 */
    TIMEQ tqHomingTime;     /* 0x3c */
    TIMEQ tqHomingTime2;    /* 0x40 */
    HANDLE hHomingTarget;   /* 0x44 */
    FLOAT fHomingRadius;    /* 0x48 */
    FLOAT fHomingRate;      /* 0x4c */
    FVECTOR vHomingPos;     /* 0x50 */
    ULONGLONG uiFlag;       /* 0x60 */
    INT AtkKind;            /* 0x68 ATK_KIND */
    INT AtkAttr;            /* 0x6c ATK_ATTR */
    INT iAttackPoint;       /* 0x70 */
    UINT uiAttackID;        /* 0x74 */
    FLOAT fCollRadius;      /* 0x78 */
    FLOAT fShiftY;          /* 0x7c */
    INT CollType;           /* 0x80 COLL_TYPE */
    FLOAT fChangeRadius;    /* 0x84 */
    FLOAT fCollOffsetY;     /* 0x88 */
    TIMEQ tqCollAtkTime;    /* 0x8c */
    FLOAT fScale;           /* 0x90 */
    TIMEQ tqFadeTime;       /* 0x94 */
    HANDLE hBoundTarget;    /* 0x98 */
    INT iBoundCount;        /* 0x9c */
    FLOAT fEffectBulletSkipFrame; /* 0xa0 */
    BULLET_EFFECT_INFO* pEffectTbl;        /* 0xa4 */
    FVECTOR* pOffsetTbl;                   /* 0xa8 */
    FVECTOR* pRotTbl;                      /* 0xac */
    INT iExtraEffectNum;                   /* 0xb0 */
    BULLET_EFFECT_INFO* pExtraEffectTbl;   /* 0xb4 */
    BULLET_EFFECT_INFO* pExplodeEffectTbl; /* 0xb8 */
    BULLET_EFFECT_SHADOW_INFO* pShadow;    /* 0xbc */

    void init(INT inBulletID, INT inGroupID);
    INT isAttachNoRot() const { return uiFlag & 1; }
};


class CCollAttack;
class C3DLines;
class CEffect;

class CBullet : public CEntity {
private:
    ULONGLONG m_uiFlag;                  /* 0x1d0 */
    HANDLE m_hOwner;                     /* 0x1d8 */
    FLOAT m_fSpeed;                      /* 0x1dc */
    FLOAT m_fMaxDist;                    /* 0x1e0 */
    HANDLE m_hEffectHandle;              /* 0x1e4 */
    INT m_iFileID;                       /* 0x1e8 */
    INT m_iSEID;                         /* 0x1ec */
    HANDLE m_hSEHandle;                  /* 0x1f0 */
    HANDLE m_hEffectShadow;              /* 0x1f4 */
    HANDLE m_hTarget;                    /* 0x1f8 */
    TIMEQ m_tqHomingTimer;               /* 0x1fc */
    FLOAT m_fChangeRadius;               /* 0x200 */
    FLOAT m_fCollOffsetY;                /* 0x204 */
    TIMEQ m_tqCollAtkTimer;              /* 0x208 */
    TIMEQ m_tqFadeTime;                  /* 0x20c */
    BULLET_EFFECT_INFO* m_pEffectTbl;    /* 0x210 */
    FVECTOR* m_pOffsetTbl;               /* 0x214 */
    FVECTOR* m_pRotTbl;                  /* 0x218 */
    INT m_iExtraEffectHandleNum;         /* 0x21c */
    HANDLE* m_pExtraEffectHandle;        /* 0x220 */
    HANDLE m_hHitEnemyHandle;            /* 0x224 */
    BULLET_EFFECT_INFO* m_pExtraEffectTbl; /* 0x228 */
    CCollAttack* m_pAttack;              /* 0x22c */
    CStateTemplate<CBullet> m_State;     /* 0x230 */
    C3DLines* m_pCollLines;              /* 0x250 */

public:
    CBullet(CTask* inParent);
    virtual ~CBullet();
    virtual INT getTypeID() const;
    virtual BOOL setup(const BULLET_PARAM& inParam);
    void setBodyRadius(FLOAT inRadius, FLOAT inShiftY, INT inSliding, COLL_KIND inKind);
    void setCollAttackPos(const FVECTOR& inPos);
    virtual void disp();
    void setOwner(HANDLE inOwner) { m_hOwner = inOwner; }
    // MATCH: Game/Bullet/CreateBullet calls setOwner with the owner entity (defined below, outside Bullet.cpp,
    // since an inline member here would be emitted in Bullet.o)
    void setOwner(CEntity* inOwner);
    void setAttackID(UINT inID);
    void updateAttackID();
    HANDLE getOwner() const { return m_hOwner; }
    CEffect* getAttachEffectBullet();
    UINT getAttackID() const;
    HANDLE getHitEnemyHandle() const { return m_hHitEnemyHandle; }
    void kill();
    ULONGLONG isAttachNoRot() const { return m_uiFlag & 0x1; }
    ULONGLONG isNoHitOut() const { return m_uiFlag & 0x2; }
    ULONGLONG isTimeOut() const { return m_uiFlag & 0x4; }
    ULONGLONG isGravity() const { return m_uiFlag & 0x8; }
    ULONGLONG isNoHitMapOut() const { return m_uiFlag & 0x10; }
    ULONGLONG isNoHitObjOut() const { return m_uiFlag & 0x20; }
    ULONGLONG isClearSpeedOnGround() const { return m_uiFlag & 0x40; }
    ULONGLONG isSpread() const { return m_uiFlag & 0x80; }
    ULONGLONG isAlwaysFadeOut() const { return m_uiFlag & 0x100; }
    ULONGLONG isMaxDistSetEffect() const { return m_uiFlag & 0x400; }
    ULONGLONG isHomingOn() const { return m_uiFlag & 0x800; }
    ULONGLONG isStartHomingOff() const { return m_uiFlag & 0x1000; }
    ULONGLONG isTwistMoveOn() const { return m_uiFlag & 0x2000; }
    ULONGLONG isHitExplodeOnly() const { return m_uiFlag & 0x4000; }
    ULONGLONG isDegHoming() const { return m_uiFlag & 0x8000; }
    ULONGLONG isChangeRadius() const { return m_uiFlag & 0x10000; }
    ULONGLONG isCollAtkTime() const { return m_uiFlag & 0x20000; }
    ULONGLONG isMaxDistClearSpeed() const { return m_uiFlag & 0x40000; }
    ULONGLONG isHomingPosOn() const { return m_uiFlag & 0x80000; }
    ULONGLONG isClearSpeedOnTarget() const { return m_uiFlag & 0x100000; }
    ULONGLONG isStartCollAtkOff() const { return m_uiFlag & 0x200000; }
    ULONGLONG isBoundOn() const { return m_uiFlag & 0x400000; }
    ULONGLONG isExplodeCollSpread() const { return m_uiFlag & 0x800000; }
    ULONGLONG isClearSpeedOnObj() const { return m_uiFlag & 0x1000000; }
    ULONGLONG isNoXRotHoming() const { return m_uiFlag & 0x2000000; }
    ULONGLONG isFadeKill() const { return m_uiFlag & 0x4000000; }
    ULONGLONG isExtraFullAdjust() const { return m_uiFlag & 0x8000000; }
    ULONGLONG isUnrevisedHitMapOut() const { return m_uiFlag & 0x10000000; }
    ULONGLONG isAutoLockOn() const { return m_uiFlag & 0x20000000; }
    ULONGLONG isClearSpeedTouchWall() const { return m_uiFlag & 0x40000000; }
    ULONGLONG isBulletFade() const { return m_uiFlag & 0x800000000000ULL; }
    ULONGLONG isTouchAny() const { return m_uiFlag & 0x1000000000000ULL; }

protected:
    virtual void beginMainState();
    virtual void mainState();
    virtual void endMainState();
    virtual void lastUpdate();
    virtual HANDLE makeEffect(INT inID);
    BOOL updateFade();
    void updateAttackCollision();
    void updateShadow();

private:
    HANDLE playBulletSE_(INT inID);

    friend class CHomingBullet;
};

#ifndef GAME_BULLET_BULLET_CPP /* MATCH: GAME_BULLET_BULLET_CPP gate, only for its own unit (tested 2026-11-02) */
inline void CBullet::setOwner(CEntity* inOwner) {
    m_hOwner = inOwner->getHandle();
    setEntityType(inOwner->getEntityType());
    setEntityFlag(inOwner->isHeartless() ? 0x20000 : 0, 0);
}
#endif

class CHomingBullet : public CBullet {
private:
    INT m_iStep;                             /* 0x260 */
    HANDLE m_hHomingTarget;                  /* 0x264 */
    FLOAT m_fHomingRadius;                   /* 0x268 */
    FVECTOR m_vHomingPos;                    /* 0x270 */
    FVECTOR m_vTwist;                        /* 0x280 */
    TIMEQ m_tqExplodeTime;                   /* 0x290 */
    FLOAT m_fExplodeRadius;                  /* 0x294 */
    FLOAT m_fSpreadScale;                    /* 0x298 */
    HANDLE m_hBoundTarget;                   /* 0x29c */
    INT m_iBoundCount;                       /* 0x2a0 */
    BULLET_EFFECT_INFO* m_pExplodeEffectTbl; /* 0x2a4 */

public:
    CHomingBullet(CTask* inParent);
    virtual ~CHomingBullet();
    virtual BOOL setup(const BULLET_PARAM& inParam);
    void setHomingPos(const FVECTOR& inPos) { m_vHomingPos = inPos; }
    INT isExplode() const { return m_iStep > 0; }

protected:
    virtual void mainState();
    virtual void lastUpdate();
    virtual HANDLE makeEffect(INT inID);

private:
    BOOL bound_();
    INT initExplode();
    void exeSpread();
    void exeTwist();
};

#endif /* GAME_BULLET_BULLET_H */
