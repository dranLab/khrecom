#ifndef GAME_PLAYER_FRIENDGOOFY_H
#define GAME_PLAYER_FRIENDGOOFY_H

#include "khtypes.h"
#include "Game/Player/Friend.h"

namespace Friend {

class CFriendGoofy : public CFriend {
public:
    CFriendGoofy(CTask* inParent, INT inEx);
private:
    INT exeGoofyTornado();
    INT exeGoofySmash();
    INT exeGoofyWildCrusher();
    void adjustPlayer(INT inWarp);
public:
    virtual ~CFriendGoofy();
protected:
    virtual void beginStateAppear();
    virtual void stateAppear();
    virtual void beginStateAction1();
    virtual void stateAction1();
    virtual void beginStateAction2();
    virtual void stateAction2();
    virtual void beginStateAction3();
    virtual void stateAction3();
public:
    virtual INT setSkill(INT inID);
};

extern unsigned char g_GoofyAttackID[4];

} // namespace Friend

#endif /* GAME_PLAYER_FRIENDGOOFY_H */
