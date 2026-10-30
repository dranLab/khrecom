#ifndef GAME_COLLISION_RIDE_H
#define GAME_COLLISION_RIDE_H

#include "khtypes.h"
#include "System/Vector.h"
#include "System/SJoint.h"
#include "Game/Collision/CollObj.h"
#include "Game/Collision/CollUtilA.h"

class CTask;

class CRider;
class CRiderList;

class CRide : public CCollObj {
public:
    CRiderList* m_pRiderList; /* 0x14 */
private:
    INT m_fgOnLook;           /* 0x18 */

public:
    CRide();
    virtual ~CRide();
    void addRiderList(CRider& Rider);
    void removeRiderList(CRider& Rider);
    INT getRiderListSize();
    BOOL isRideOn(COLL_KIND inCheckKind);
    BOOL isRideOnPlayer() { return isRideOn(COLL_KIND_PLAYER); }
    BOOL isRideOnEnemy() { return isRideOn(COLL_KIND_ENEMY); }
    BOOL isHangOn(COLL_KIND inCheckKind);
    BOOL isHangOnPlayer() { return isHangOn(COLL_KIND_PLAYER); }
    BOOL isHangOnEnemy() { return isHangOn(COLL_KIND_ENEMY); }
    INT isOnLook() { return m_fgOnLook; }
    void setFgOnLook(INT fgOnLook) { m_fgOnLook = fgOnLook; }
    FMATRIX* getAttachMatrix();
};

class CRideModel : public CRide {
public:
    MCD_INFO m_MCDInfo;    /* 0x1c */
    FMATRIX* m_pMatIOld;   /* 0x3c */
    OCD_WORK* m_pOCDWork;  /* 0x40 */
    SHORT m_fgMove;        /* 0x44 */
    SHORT m_fgAdjust;      /* 0x46 */
    FLOAT m_fBSRadius2;    /* 0x48 */

    CRideModel();
    virtual ~CRideModel();
    void init(void* pOCDData, CTask* pBFObj);
    void setAttachMatrix(const FMATRIX* inMat);
    void enableChipCollision(INT inIdx, BOOL inSw);
    void setChipLocalMatrix(INT inIdx, const FMATRIX* inMat);
    FMATRIX* getChipLocalMatrix(INT inIdx);
    void setOldMatrix();
    void hoseiRiders(FVECTOR& diffRot);
    INT getCollisionNum() const { return m_MCDInfo.iChipSuu; }
    void changeListR2();
    void changeListR();
    void changeListRC();
    void enableTransAdjust(BOOL fg) { m_fgAdjust = (m_fgAdjust & ~1) | fg; }
    void enableRotAdjust(BOOL fg) { m_fgAdjust = (m_fgAdjust & ~2) | (fg << 1); }
    void setBSRadius2(FLOAT fBSRadius2) { m_fBSRadius2 = fBSRadius2; }
    FMATRIX* getAttachMatrix();
};

class CRideEtc : public CRide {
public:
    CRideEtc() {}
    virtual ~CRideEtc() {}
    virtual INT detectEtc(CCollision& collision);
    void init();
    void hoseiRiders();
};

class CRider : public CSJointT<CRider> {
public:
    FVECTOR m_Rotate;   /* 0x10 */
    FVECTOR m_Position; /* 0x20 */
    FLOAT m_shiftY;     /* 0x30 */
    FLOAT m_Radius;     /* 0x34 */
    COLL_KIND m_Kind;   /* 0x38 */
    BOOL m_fgHang;      /* 0x3c */
private:
    HANDLE m_hTask;     /* 0x40 */

public:
    CRider(HANDLE hTask, FLOAT shiftY, FLOAT radius, COLL_KIND kind, BOOL fgHang);
    HANDLE getHandle() { return m_hTask; }
    BOOL isHang() { return m_fgHang; }
    // PARTIAL: added for Game/Collision/CollMap (original names unknown)
    void setPosition(FVECTOR& Rotate, FVECTOR& Position) {
        m_Rotate = Rotate;
        m_Position = Position;
    }
    void getPosition(FVECTOR& Rotate, FVECTOR& Position) {
        Rotate = m_Rotate;
        Position = m_Position;
    }
};

class CRiderList : public CSJointListT<CRider> {
public:
    ~CRiderList() {
        for (CRider *pEntry = getHead(), *__pn = pEntry ? pEntry->getNext() : NULL; pEntry;
             pEntry = __pn, __pn = __pn ? __pn->getNext() : NULL) {
            delete pEntry;
        }
        resetParam();
    }
};

#endif /* GAME_COLLISION_RIDE_H */
