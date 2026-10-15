// PARTIAL: added for Game/Player/SummonSinba
#ifndef GAME_COLLISION_COLLMAP_H
#define GAME_COLLISION_COLLMAP_H

#include "khtypes.h"
#include "Game/Collision/CollObj.h"

class CCollMap : public CCollObj {
private:
    UBYTE _m_0x14[0x18]; /* 0x14 m_pLastRide .. 0x28 m_nTimeq */

public:
    virtual ~CCollMap();
    virtual void init(INT inKind, COLL_TYPE inType, INT inParam0, INT inParam1); // PARTIAL: see CCollObj::init
    virtual INT collideMap();
    INT collideRide(INT inFlag);
};

#endif /* GAME_COLLISION_COLLMAP_H */
