#ifndef GAME_PLAYER_FRIENDARIEL_H
#define GAME_PLAYER_FRIENDARIEL_H

#include "khtypes.h"
#include "Game/Player/Friend.h"

namespace Friend {

class CFriendAriel : public CFriend {
public:
    CFriendAriel(CTask* inParent);
    virtual ~CFriendAriel();
private:
    INT exeArielAttack();
protected:
    virtual void beginStateAction1();
    virtual void stateAction1();
public:
    virtual INT setSkill(INT inID);
};

extern unsigned char g_ArielAttackID[4];

} // namespace Friend

#endif /* GAME_PLAYER_FRIENDARIEL_H */
