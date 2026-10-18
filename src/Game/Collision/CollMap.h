#ifndef GAME_COLLISION_COLLMAP_H
#define GAME_COLLISION_COLLMAP_H

#include "khtypes.h"
#include "Game/Collision/CollObj.h"

class CRide;
class CRider;

/* All inline members are emitted (weak) in Game/Collision/CollMap.cpp, in declaration order: keep the order. */
class CCollMap : public CCollObj {
private:
    CRide* m_pLastRide;  /* 0x14 */
    CRide* m_pNowRide;   /* 0x18 */
    CRide* m_pNowHang;   /* 0x1c */
    CRider* m_pRider;    /* 0x20 */
    CRider* m_pHanger;   /* 0x24 */
    TIMEQ m_nTimeq;      /* 0x28 */
    static INT m_fgSameFrame;
    static BOOL m_bExceptBSCheck;

public:
    CCollMap();
    virtual ~CCollMap();
    virtual INT collideMap();
    virtual void init(COLL_KIND collKind, COLL_TYPE collType, INT fgStatus, HANDLE hTask);
    INT collideRide(BOOL fgExceptNowRide);
    INT collideRideForCheck(BOOL fgExceptNowRide);
    void registerLastRide(HANDLE hTask);
    void removeRide();
    void setRidePosition(FVECTOR& Rotate, FVECTOR& Position);
    BOOL getRidePosition(FVECTOR& Rotate, FVECTOR& Position);
    BOOL isOnRide() { return m_pNowRide != NULL; }
    CRide* getNowRide() { return m_pNowRide; }
    void registerLastHang(HANDLE hTask);
    void removeHang();
    void setHangPosition(FVECTOR& Rotate, FVECTOR& Position);
    BOOL getHangPosition(FVECTOR& Rotate, FVECTOR& Position);
    BOOL isOnHang() { return m_pNowHang != NULL; }
    CRide* getNowHang() { return m_pNowHang; }
    INT checkWaterSurface(FVECTOR& inPos, FLOAT inHeight);

private:
    INT collideRideR2(CCollision& coll, CRide** ppRideAtari, BOOL fgExceptNowRide);
};

#endif /* GAME_COLLISION_COLLMAP_H */
