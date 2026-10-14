#ifndef GAME_PLAYER_FRIENDBEAST_H
#define GAME_PLAYER_FRIENDBEAST_H

#include "khtypes.h"
#include "Game/Player/Friend.h"

namespace Friend {

class CFriendBeast : public CFriend {
public:
    CFriendBeast(CTask* inParent);
    virtual ~CFriendBeast();
protected:
    virtual void beginStateAction1();
    virtual void stateAction1();
    virtual void beginStateAction2();
    virtual void stateAction2();
    virtual void beginStateAction3();
    virtual void stateAction3();
public:
    virtual INT setSkill(INT inID);
};

extern unsigned char g_BeastAttackID[4];

} // namespace Friend

#endif /* GAME_PLAYER_FRIENDBEAST_H */
