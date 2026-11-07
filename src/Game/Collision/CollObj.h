
/* PARTIAL: with COLLOBJ_H_ENUMS_ONLY defined, only the enums (Game/Enemy/HumanBoss: its includers get the
 * collision classes later, after their own classes, for the order of the vtables) */
#ifndef GAME_COLLISION_COLLOBJ_H_ENUMS
#define GAME_COLLISION_COLLOBJ_H_ENUMS

#include "khtypes.h"
#include "Game/Collision/CollInfo.h"


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

#endif /* GAME_COLLISION_COLLOBJ_H_ENUMS */

#if !defined(GAME_COLLISION_COLLOBJ_H) && !defined(COLLOBJ_H_ENUMS_ONLY)
#define GAME_COLLISION_COLLOBJ_H /* MATCH: Game/Collision/CollUtil.h tests it (collideMap, see below) */

#include "System/Vector.h"
#include "System/SJoint.h"


class CCollision {
public:
    FVECTOR m_inWS;      /* 0x00 */
    FVECTOR m_inWE;      /* 0x10 */
    FVECTOR m_outWE;     /* 0x20 */
    FVECTOR m_outNormal; /* 0x30 */
    FVECTOR m_outDetect; /* 0x40 */
    COLL_TYPE m_Type;    /* 0x50 */
    COLL_KIND m_Kind;    /* 0x54 */
    INT m_outStatus;     /* 0x58 */
    INT m_fgStatus;      /* 0x5c */
    FLOAT m_Radius;      /* 0x60 */
    FLOAT m_shiftY;      /* 0x64 */
    COLL_INFO_UNION m_outInfo;  /* 0x68 */
    COLL_INFO_UNION m_outInfoO; /* 0x6c */

    // inline constructor and destructor: weak in the original (as in Game/Collision/CollUtilF.cpp)
    CCollision() {
        m_fgStatus = 0;
        m_Kind = COLL_KIND_PLAYER;
        m_outInfoO.info = 0;
        m_outInfo.info = 0;
        m_Radius = m_shiftY = 0.0f;
    }
    virtual ~CCollision() {} /* 0x70 */
    void initForMap(/* parameters unknown */);
    void initForObj(/* parameters unknown */);
    void setStartPos(const FVECTOR& inPos) { m_inWS = inPos; }
    void setEndPos(const FVECTOR& inPos) { m_inWE = inPos; }
    FVECTOR& getOutPos(/* parameters unknown */);
};


class CPointCollision : public CCollision {
public:
    CPointCollision() { m_Type = COLL_TYPE_POINT; } virtual ~CPointCollision() {}
    virtual INT collideMap();
};
/* MATCH: collideMap inlines CCollUtil::instance (Game/Collision/CollUtil.h): defined here when CollUtil.h came
 * first (or the unit has its own accessor: COLLUTIL_H_LOCAL_ACCESSOR), else at the end of CollUtil.h
 * (Game/Gimmick/GimmickSearchLaser gets CollObj.h early via CardBattle.h) */
#if (defined(GAME_COLLISION_COLLUTIL_H) && defined(GAME_COLLISION_COLLUTILA_H)) || defined(COLLUTIL_H_LOCAL_ACCESSOR)
inline INT CPointCollision::collideMap() { return CCollUtil::instance()._detectMCD(*this); }
#endif

class CSphereCollision : public CCollision {
public:
    CSphereCollision() {
        m_Type = COLL_TYPE_SPHERE;
        m_Radius = m_shiftY = 0.5f;
    }
    void setRadius(FLOAT inRadius) {
        m_shiftY = inRadius;
        m_Radius = inRadius;
    }
    virtual INT collideMap();
};
#if (defined(GAME_COLLISION_COLLUTIL_H) && defined(GAME_COLLISION_COLLUTILA_H)) || defined(COLLUTIL_H_LOCAL_ACCESSOR)
inline INT CSphereCollision::collideMap() { return CCollUtil::instance()._detectMCD(*this); } // MATCH: see above
#endif

class CPoly3Collision : public CCollision {
public:
    FVECTOR m_BasePos;
    CPoly3Collision() { m_Type = COLL_TYPE_POLY3; }
};



class CColumnCollision : public CCollision {
public:
    CColumnCollision() {
        m_Type = COLL_TYPE_COLUMN;
        m_Radius = 0.5f;
    }
    void setRadius(/* parameters unknown */);
};


class CCircleCollision : public CCollision {
public:
    CCircleCollision() {
        m_Type = COLL_TYPE_CIRCLE;
        m_Radius = 0.5f;
    }
    void setRadius(/* parameters unknown */);
};

class CModelCollision : public CCollision {
public:
    CModelCollision() { m_Type = COLL_TYPE_MODEL; }
};

class CEtcCollision : public CCollision {
public:
    CEtcCollision() { m_Type = COLL_TYPE_ETC; }
};

class CInfinityCollision : public CCollision {
public:
    CInfinityCollision() { m_Type = COLL_TYPE_INFINITY; }
};


/* All inline members are emitted (weak) in Game/Collision/CollObj.cpp, in declaration order: keep the order. */
class CCollObj : public CSJointT<CCollObj> {
public:
    static class CCollisionList* m_pCollListP;
    static class CCollisionList* m_pCollListE;
    static class CCollisionList* m_pCollListI;
    static class CCollisionList* m_pCollListR;
    static class CCollisionList* m_pCollListR2;
    static class CCollisionList* m_pCollListRC;
    static class CCollisionList* m_pCollListA;
protected:
    INT m_Enable;              /* 0x08 */
    CCollision* m_pCollision;  /* 0x0c */
    HANDLE m_hTask;            /* 0x10 */

public:
    CCollObj();
    virtual ~CCollObj();
    static void collisionInit();
    static void collisionDelete();
    virtual void off() { m_Enable = 0; }
    virtual void on() { m_Enable = 1; }
    virtual INT detectEtc(CCollision& collision) { return 0; }
    INT isEnable() { return m_Enable; }
    void setStartPos(FVECTOR& inPos) {
        if (m_pCollision) m_pCollision->setStartPos(inPos);
    }
    void setEndPos(FVECTOR& inPos) {
        if (m_pCollision) m_pCollision->setEndPos(inPos);
    }
    void setBottomPos(FVECTOR& inPos) {
        if (m_pCollision) m_pCollision->setStartPos(inPos);
    }
    void setTopPos(FVECTOR& inPos) {
        if (m_pCollision) m_pCollision->setEndPos(inPos);
    }
    void setRadius(FLOAT inRadius);
    void setType(COLL_TYPE inType) {
        if (m_pCollision) m_pCollision->m_Type = inType;
    }
    virtual void setKind(COLL_KIND inKind) {
        if (m_pCollision) m_pCollision->m_Kind = inKind;
    }
    FVECTOR* getOutPos() { return m_pCollision ? &m_pCollision->m_outWE : 0; }
    FVECTOR* getDitectPos() { return m_pCollision ? &m_pCollision->m_outDetect : 0; }
    FVECTOR* getDetectPos() { return m_pCollision ? &m_pCollision->m_outDetect : 0; }
    FVECTOR* getStartPos() { return (FVECTOR*)m_pCollision; }
    FVECTOR* getEndPos() { return m_pCollision ? &m_pCollision->m_inWE : 0; }
    FVECTOR* getDetectNormal() { return m_pCollision ? &m_pCollision->m_outNormal : 0; }
    INT getType() { return m_pCollision ? m_pCollision->m_Type : 0; }
    INT getKind() { return m_pCollision ? m_pCollision->m_Kind : -1; }
    HANDLE getHandle() { return m_hTask; }
    void setHandle(HANDLE hTask) { m_hTask = hTask; }
    CCollision* getCollision() { return m_pCollision; }
    void setShiftY(FLOAT inShiftY);
    FLOAT getShiftY() { return m_pCollision ? m_pCollision->m_shiftY : 0.0f; }
    FLOAT getRadius() { return m_pCollision ? m_pCollision->m_Radius : 0.0f; }
#ifdef COLLOBJ_H_INFO_AS_INT /* MATCH: System/Graphic/TriangleIK was written against an INT result: with the union its else branch schedules the TRACE arguments differently */
    INT getDetectInfo() {
        if (m_pCollision) return m_pCollision->m_outInfo.info;
        return 0;
    }
    INT getDetectInfoO() {
        if (m_pCollision) return m_pCollision->m_outInfoO.info;
        return 0;
    }
#else
    COLL_INFO_UNION getDetectInfo() {
        COLL_INFO_UNION info;
        if (m_pCollision) {
            info.info = m_pCollision->m_outInfo.info;
            return info;
        }
        info.info = 0;
        return info;
    }
    COLL_INFO_UNION getDetectInfoO() {
        COLL_INFO_UNION info;
        if (m_pCollision) {
            info.info = m_pCollision->m_outInfoO.info;
            return info;
        }
        info.info = 0;
        return info;
    }
#endif
    INT getOutStatus() { return m_pCollision ? m_pCollision->m_outStatus : 0; }
    void enableSliding(INT inFlag) {
        if (m_pCollision) m_pCollision->m_fgStatus = (m_pCollision->m_fgStatus & ~1) | (inFlag != 0);
    }
    static class CCollisionList* getCollisionList(INT iType) {
        switch (iType) {
        case 0:
            return m_pCollListP;
        case 1:
            return m_pCollListE;
        case 2:
            return m_pCollListI;
        }
        return NULL;
    }
private:
    void link(HANDLE hTask);
    void unlink();
protected:
    virtual void init(COLL_KIND collKind, COLL_TYPE collType, HANDLE hTask, INT fgStatus);
};


class CCollisionList : public CSJointListT<CCollObj> {
public:
    ~CCollisionList() {
        for (CCollObj *pEntry = getHead(), *__pn = pEntry ? pEntry->getNext() : NULL; pEntry;
             pEntry = __pn, __pn = __pn ? __pn->getNext() : NULL) {
            delete pEntry;
        }
        resetParam();
    }
};



#endif /* GAME_COLLISION_COLLOBJ_H */
