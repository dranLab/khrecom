// PARTIAL: added for Game/Player/PlayerWeapon
#ifndef GAME_PLAYER_FRIENDDONALD_H
#define GAME_PLAYER_FRIENDDONALD_H

#include "khtypes.h"
#include "Game/Player/Friend.h"

namespace Friend {

class CFriendDonald : public CFriend {
public:
    CFriendDonald(CTask* inParent);
    virtual ~CFriendDonald();
    INT checkBlitzHit(INT inFlag);
};

} // namespace Friend

#endif /* GAME_PLAYER_FRIENDDONALD_H */
