// PARTIAL: added for Game/Bullet/CreateBullet
#ifndef GAME_BULLET_BULLET_H
#define GAME_BULLET_BULLET_H

#include "khtypes.h"
#include "System/Graphic/Gprim.h" // PARTIAL: moved here from Game/Entity.h for Game/Gimmick/GimmickHitMore
#include "Game/Entity.h"

struct BULLET_PARAM {
    INT iBulletID;
    INT iGroupID;
    INT iMemberID;
    /* ... (rest not needed yet) */
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
