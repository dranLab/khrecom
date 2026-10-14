// PARTIAL: added for Game/Bullet/CreateBullet
#ifndef GAME_BULLET_BULLET_H
#define GAME_BULLET_BULLET_H

#include "khtypes.h"
#include "System/Graphic/Gprim.h" // PARTIAL: moved here from Game/Entity.h for Game/Gimmick/GimmickHitMore
#include "Game/Entity.h"
#include "System/Vector.h" // PARTIAL: added for Game/Player/SummonMushu (BULLET_PARAM)

// PARTIAL: completed for Game/Player/SummonMushu
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
};

// PARTIAL: added for Game/Player/SummonGenie
enum COLL_KIND {
    COLL_KIND_SHOT = -2147483647 - 1,
    COLL_KIND_PLAYER = 0,
    COLL_KIND_ENEMY = 2,
    COLL_KIND_OBJECT = 4,
    COLL_KIND_ITEM = 6,
    COLL_KIND_PSHOT = -2147483647 - 1,
    COLL_KIND_ESHOT = -2147483646,
    COLL_KIND_OSHOT = -2147483644,
    COLL_KIND_SHADOW = -2147483640,
    COLL_KIND_FOOT = 10,
    COLL_KIND_CAMERA = -2147483636,
    COLL_KIND_CAMERAT = -2147483634,
    COLL_KIND_BODY = 0,
    COLL_KIND_ATTACK = 1,
    COLL_KIND_NONE = -1,
    COLL_KIND_CK_PE = 14,
    COLL_KIND_CK_BA = 1
};

class CBullet : public CEntity {
private:
    ULONGLONG m_uiFlag;      /* 0x1d0 */
    HANDLE m_hOwner;         /* 0x1d8 */
    UBYTE _m_0x1dc[0x84];    /* 0x1dc m_fSpeed .. 0x250 m_pCollLines */

public:
    CBullet(CTask* inParent);
    virtual ~CBullet();
    virtual INT getTypeID() const;
    virtual BOOL setup(const BULLET_PARAM& inParam);
    virtual void disp();

    HANDLE getOwner() const { return m_hOwner; } // PARTIAL: added for Game/Bullet/BulletManager
    void kill();                                 // PARTIAL: added for Game/Bullet/BulletManager
    void setBodyRadius(FLOAT inRadius, FLOAT inShiftY, INT inSliding, COLL_KIND inKind); // PARTIAL: added for Game/Player/SummonGenie

    void setOwner(CEntity* inOwner) {
        m_hOwner = inOwner->getHandle();
        setEntityType(inOwner->getEntityType());
        setEntityFlag(inOwner->isHeartless() ? 0x20000 : 0, 0);
    }
};

class CHomingBullet : public CBullet {
private:
    UBYTE _m_0x260[0x50]; /* 0x260 m_iStep .. 0x2a4 m_pExplodeEffectTbl */

public:
    CHomingBullet(CTask* inParent);
    virtual ~CHomingBullet();
    virtual BOOL setup(const BULLET_PARAM& inParam);
};

#endif /* GAME_BULLET_BULLET_H */
