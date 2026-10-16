// PARTIAL: added for Game/Player/SummonSinba
#ifndef GAME_COLLISION_COLLOBJ_H
#define GAME_COLLISION_COLLOBJ_H

#include "khtypes.h"
#include "System/Vector.h"
#include "System/SJoint.h"

// PARTIAL: added for Game/Player/PlayerWeapon
enum COLL_TYPE {
    COLL_TYPE_NONE = 0,
    COLL_TYPE_POINT = 1,
    COLL_TYPE_SPHERE = 2,
    COLL_TYPE_CUBE = 3,
    COLL_TYPE_COLUMN = 4,
    COLL_TYPE_COLUMNR = 5,
    COLL_TYPE_POLY = 6,
    COLL_TYPE_POLY3 = 7,
    COLL_TYPE_CAPSULE = 8,
    COLL_TYPE_MODEL = 9,
    COLL_TYPE_ETC = 10,
    COLL_TYPE_CIRCLE = 11,
    COLL_TYPE_INFINITY = 12,
    COLL_TYPE_MODELW = 13
};

// PARTIAL: added for Game/Player/SummonGenie (moved here from Game/Bullet/Bullet.h for Game/Bullet/Bullet)
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

class CCollision {
public:
    FVECTOR m_inWS;      /* 0x00 */
    FVECTOR m_inWE;      /* 0x10 */
    FVECTOR m_outWE;     /* 0x20 */
    FVECTOR m_outNormal; /* 0x30 */
    FVECTOR m_outDetect; /* 0x40 */
    // PARTIAL: m_Type, m_fgStatus, m_Radius split out for Game/Player/PlayerWeapon
    INT m_Type;          /* 0x50 COLL_TYPE */
    // PARTIAL: m_Kind .. m_outInfoO split out for Game/Gimmick/GimmickSearchLaser
    INT m_Kind;          /* 0x54 COLL_KIND */
    INT m_outStatus;     /* 0x58 */
    INT m_fgStatus;      /* 0x5c */
    FLOAT m_Radius;      /* 0x60 */
    FLOAT m_shiftY;      /* 0x64 */
    INT m_outInfo;       /* 0x68 COLL_INFO_UNION */
    INT m_outInfoO;      /* 0x6c COLL_INFO_UNION */

    // PARTIAL: inline constructor and destructor (weak in the original) added for Game/Gimmick/GimmickSearchLaser
    // (as in Game/Collision/CollUtilF.cpp)
    CCollision() {
        m_fgStatus = 0;
        m_Kind = 0;
        m_outInfoO = 0;
        m_outInfo = 0;
        m_shiftY = 0.0f;
        m_Radius = 0.0f;
    }
    virtual ~CCollision() {} /* 0x70 */
    void setStartPos(const FVECTOR& inPos) { m_inWS = inPos; }
    void setEndPos(const FVECTOR& inPos) { m_inWE = inPos; }
};

class CCollObj : public CSJointT<CCollObj> {
protected:
    INT m_Enable;              /* 0x08 */
    CCollision* m_pCollision;  /* 0x0c */
    HANDLE m_hTask;            /* 0x10 */

public:
    CCollObj(); // PARTIAL: added for Game/Player/PlayerWeapon
    virtual ~CCollObj();
    // PARTIAL: inline bodies added for Game/Player/PlayerWeapon (weak in the original)
    virtual void off() { m_Enable = 0; }
    virtual void on() { m_Enable = 1; }
    virtual INT detectEtc();
    virtual void setKind(INT inKind);
    // PARTIAL: signature completed for Game/Bullet/Bullet (CCollAttack overrides it)
    virtual void init(COLL_KIND inKind, COLL_TYPE inType, INT inFlag, INT inHandle);

    void setStartPos(const FVECTOR& inPos) {
        if (m_pCollision) m_pCollision->setStartPos(inPos);
    }
    void setEndPos(const FVECTOR& inPos) {
        if (m_pCollision) m_pCollision->setEndPos(inPos);
    }
    // PARTIAL: added for Game/Bullet/Bullet
    INT isEnable() { return m_Enable; }
    FVECTOR* getOutPos() {
        if (m_pCollision) return &m_pCollision->m_outWE;
        return 0;
    }
    FVECTOR* getDetectPos() {
        if (m_pCollision) return &m_pCollision->m_outDetect;
        return 0;
    }
    // PARTIAL: added for Game/Player/PlayerWeapon
    void setRadius(FLOAT inRadius);
    void setShiftY(FLOAT inShiftY);
    void setType(COLL_TYPE inType) {
        if (m_pCollision) m_pCollision->m_Type = inType;
    }
    FVECTOR* getStartPos() { return (FVECTOR*)m_pCollision; }
    FVECTOR* getEndPos() {
        if (m_pCollision) return &m_pCollision->m_inWE;
        return 0;
    }
    FVECTOR* getDetectNormal() {
        if (m_pCollision) return &m_pCollision->m_outNormal;
        return 0;
    }
    // PARTIAL: added for Game/Enemy/EnemyBandit (original name unknown)
    INT getOutStatus() {
        if (m_pCollision) return m_pCollision->m_outStatus;
        return 0;
    }
    FLOAT getRadius() {
        if (m_pCollision) return m_pCollision->m_Radius;
        return 0.0f;
    }
    void enableSliding(INT inFlag) {
        if (m_pCollision) m_pCollision->m_fgStatus = (m_pCollision->m_fgStatus & ~1) | (inFlag != 0);
    }
};

#endif /* GAME_COLLISION_COLLOBJ_H */
