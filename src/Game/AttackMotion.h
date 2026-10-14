// PARTIAL: added for Game/Gimmick/GimmickToldPipe
#ifndef GAME_ATTACKMOTION_H
#define GAME_ATTACKMOTION_H

#include "khtypes.h"

union FVECTOR; // PARTIAL: added for Game/Player/FriendBeast

// PARTIAL: added for Game/Gimmick/GimmickGuillotine
enum ATK_ATTR {
    ATK_ATTR_PHYSICAL = 0,
    ATK_ATTR_NONE = 1,
    ATK_ATTR_FIRE = 2,
    ATK_ATTR_ICE = 3,
    ATK_ATTR_THUNDER = 4,
    ATK_ATTR_SPECIAL = 5,
    ATK_ATTR_MAX = 6
};

class CAttackMotion {
public:
    void setCollKind(INT inKind);
    void setAttackParam(INT inAttack, ATK_ATTR inAttr); // PARTIAL: added for Game/Gimmick/GimmickGuillotine
    void updateAttackID(); // PARTIAL: added for Game/Gimmick/GimmickHitRepetition
    BOOL test(); // PARTIAL: added for Game/Gimmick/GimmickHitFlying
    void setEnemyPos(const FVECTOR& inPos); // PARTIAL: added for Game/Player/FriendBeast
};

#endif /* GAME_ATTACKMOTION_H */
