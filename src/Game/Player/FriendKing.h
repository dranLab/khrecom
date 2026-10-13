#ifndef GAME_PLAYER_FRIENDKING_H
#define GAME_PLAYER_FRIENDKING_H

#include "khtypes.h"
#include "Game/Player/Friend.h"

namespace Friend {

class CFriendKing : public CFriend {
public:
    CFriendKing(CTask* inParent);
    virtual ~CFriendKing();
private:
    INT exeKingHolyBurst();
    void adjustPlayer(INT inWarp);
protected:
    virtual void beginStateAppear();
    virtual void stateAppear();
    virtual void beginStateAction1();
    virtual void stateAction1();
    virtual void endStateAction1();
    virtual void beginStateAction2();
    virtual void stateAction2();
public:
    virtual INT setSkill(INT inID);
};

extern unsigned char g_KingSkillID[4];

} // namespace Friend

#endif /* GAME_PLAYER_FRIENDKING_H */
