#ifndef GAME_PLAYER_PLAYERRIKU_H
#define GAME_PLAYER_PLAYERRIKU_H

#include "khtypes.h"
#include "Game/Player/Player.h"

namespace PL {

class CPlayerDRiku;

class CPlayerRiku : public CPlayer {
private:
    CPlayerDRiku* m_pDark;  /* 0x770 */
    HANDLE m_hMagicCore;    /* 0x774 */

public:
    CPlayerRiku(CTask* inParent);
    virtual ~CPlayerRiku();

protected:
    virtual void beginStateBattle();
    virtual void stateBattle();
    virtual void endStateBattle();
    virtual void beginStateWait();
    virtual void stateWait();
    virtual void endStateWait();

public:
    virtual void onWeapon(INT inEffect);
    virtual void offWeapon();
    virtual void setStateSE(INT inState, INT inKind);
    virtual INT getStateSESeq(INT inState);

private:
    virtual void initTransform(INT inFlag);
    virtual void exeTransform();
    void transferControl();
    virtual void exeAvoid();
    void exeSpeedStep();
    virtual INT startStockSkillAction();
    virtual void initStockSkill(INT inNo);
    virtual void exeStockSkill();
    virtual void initFriend(INT inNo);
    virtual void exeFriend();
    virtual void setMagic(INT inState, INT inCardID, INT inSkillID);

public:
    void endDMode(CPlayerDRiku* inRiku);
};

extern PL_PARAM g_RikuParam;

} // namespace PL

#endif /* GAME_PLAYER_PLAYERRIKU_H */
