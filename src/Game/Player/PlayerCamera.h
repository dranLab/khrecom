// PARTIAL: added for Game/Player/FriendJack
#ifndef GAME_PLAYER_PLAYERCAMERA_H
#define GAME_PLAYER_PLAYERCAMERA_H

#include "khtypes.h"

namespace PlayerCamera {

class CPlayerCamera {
public:
    void changeNowPlayer(HANDLE inPlayer, HANDLE inTarget);
    void changeCamMode(INT inMode, INT inReset);
};

} // namespace PlayerCamera

#endif /* GAME_PLAYER_PLAYERCAMERA_H */
