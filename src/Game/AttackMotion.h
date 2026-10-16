// PARTIAL: added for Game/Gimmick/GimmickToldPipe
#ifndef GAME_ATTACKMOTION_H
#define GAME_ATTACKMOTION_H

#include "khtypes.h"

union FVECTOR; // PARTIAL: added for Game/Player/FriendBeast
class CEntity; // PARTIAL: added for Game/Gimmick/BaseGimmick

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

// PARTIAL: CAttachMotion moved to Game/AttachMotion.h for Game/EffectMotion
#include "Game/AttachMotion.h"

class CAttackMotion : public CAttachMotion { // PARTIAL: base CAttachMotion added for Game/Gimmick/BaseGimmick
public:
    CAttackMotion(CEntity* inOwner); // PARTIAL: added for Game/EffectMotion (CAttachMotion has no default constructor)
    void setCollKind(INT inKind);
    // PARTIAL: added for Game/Player/Summon
    virtual BOOL set(INT inIndex);
    BOOL set(INT inIndex, INT inActNo);
    void setAttackParam(INT inAttack, ATK_ATTR inAttr); // PARTIAL: added for Game/Gimmick/GimmickGuillotine
    void updateAttackID(); // PARTIAL: added for Game/Gimmick/GimmickHitRepetition
    BOOL test(); // PARTIAL: added for Game/Gimmick/GimmickHitFlying
    void setEnemyPos(const FVECTOR& inPos); // PARTIAL: added for Game/Player/FriendBeast
    void setRadius(FLOAT inRadius); // PARTIAL: added for Game/Enemy/EnemyLargebody
};

#endif /* GAME_ATTACKMOTION_H */
