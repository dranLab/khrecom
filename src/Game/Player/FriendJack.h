#ifndef GAME_PLAYER_FRIENDJACK_H
#define GAME_PLAYER_FRIENDJACK_H

#include "khtypes.h"
#include "Game/Player/Friend.h"

namespace Friend {

class CFriendJack : public CFriend {
public:
    CFriendJack(CTask* inParent);
    virtual ~CFriendJack();
private:
    INT exeJackMagic();
protected:
    virtual void beginStateAction1();
    virtual void stateAction1();
public:
    virtual INT setSkill(INT inID);
};

extern unsigned char g_JackMagicID[3][4];

} // namespace Friend

#endif /* GAME_PLAYER_FRIENDJACK_H */
