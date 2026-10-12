#ifndef GAME_PLAYER_FRIENDALADDIN_H
#define GAME_PLAYER_FRIENDALADDIN_H

#include "khtypes.h"
#include "Game/Player/Friend.h"

namespace Friend {

class CFriendAladdin : public CFriend {
public:
    CFriendAladdin(CTask* inParent);
    virtual ~CFriendAladdin();
protected:
    virtual void beginStateAction1();
    virtual void stateAction1();
public:
    virtual INT setSkill(INT inID);
};

extern unsigned char g_AladdinAttackID[4];

} // namespace Friend

#endif /* GAME_PLAYER_FRIENDALADDIN_H */
