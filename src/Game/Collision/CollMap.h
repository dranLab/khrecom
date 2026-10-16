// PARTIAL: added for Game/Player/SummonSinba
#ifndef GAME_COLLISION_COLLMAP_H
#define GAME_COLLISION_COLLMAP_H

#include "khtypes.h"
#include "Game/Collision/CollObj.h"

class CCollMap : public CCollObj {
private:
    UBYTE _m_0x14[0x18]; /* 0x14 m_pLastRide .. 0x28 m_nTimeq */

public:
    CCollMap(); // PARTIAL: added for Game/Collision/CollMapCamera
    virtual ~CCollMap();
    virtual void init(COLL_KIND inKind, COLL_TYPE inType, INT inFlag, INT inHandle); // PARTIAL: signature completed for Game/Bullet/Bullet
    virtual INT collideMap();
    INT collideRide(INT inFlag);
};

#endif /* GAME_COLLISION_COLLMAP_H */
