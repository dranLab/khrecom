#ifndef GAME_PLAYER_FRIENDDONALD_H
#define GAME_PLAYER_FRIENDDONALD_H

#include "khtypes.h"
#include "Game/Player/Friend.h"

namespace Friend {

class CFriendDonald : public CFriend {
public:
    CFriendDonald(CTask* inParent);
private:
    INT exeDonaldMagic();
    INT exeDonaldStardustBlitz();
    HANDLE setBlitz();
    void setStardust(INT inNo);
    INT exeDonaldWildCrusher();
    void adjustPlayer(INT inWarp);
public:
    virtual ~CFriendDonald();
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
    INT checkBlitzHit(INT inFlag);
};

extern unsigned char g_DonaldMagicID[3][4];
extern unsigned char g_DonaldAttackID[4];

} // namespace Friend

#endif /* GAME_PLAYER_FRIENDDONALD_H */
