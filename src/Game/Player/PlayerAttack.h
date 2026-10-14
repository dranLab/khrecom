// PARTIAL: added for Game/Player/FriendAladdin
#ifndef GAME_PLAYER_PLAYERATTACK_H
#define GAME_PLAYER_PLAYERATTACK_H

#include "khtypes.h"

struct PATK_DATA {
    BYTE ANIM_ID;
    BYTE ANIM_ESQ;
    BYTE EFFECT;
    BYTE TRG_FRAME;
    BYTE COLL_START_FRAME;
    BYTE COLL_END_FRAME;
    BYTE MOVE_START_FRAME;
    BYTE MOVE_END_FRAME;
    BYTE RADIUS;
    BYTE MAX_SPEED;
    BYTE POWER;
    BYTE DAMAGE;
    BYTE WORK0;
    BYTE WORK1;
    BYTE WORK2;
    BYTE FLAG;
};

namespace PL {
extern PATK_DATA g_AttackDataList[188];
extern FLOAT g_fAntiGravity; // PARTIAL: added for Game/Player/SummonBambi
}

#endif /* GAME_PLAYER_PLAYERATTACK_H */
