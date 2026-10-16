#ifndef GAME_PLAYER_PLAYERDRIKU_H
#define GAME_PLAYER_PLAYERDRIKU_H

#include "khtypes.h"
#include "Game/Player/Player.h"

namespace PL {

class CPlayerRiku;

class CPlayerDRiku : public CPlayer {
public:
    CPlayerRiku* m_pRiku;    /* 0x770 */
    HANDLE m_hMagicCore;     /* 0x774 */
    INT m_nAttackCount;      /* 0x778 */
    INT m_iBoneLHand;        /* 0x77c */
    FVECTOR m_vWarpPos[2];   /* 0x780 */
    FVECTOR m_vWarpNormal[2]; /* 0x7a0 */

public:
    CPlayerDRiku(CTask* inParent, UINT inChara, INT inEx);
    virtual ~CPlayerDRiku();

protected:
    virtual void stateInitial();
    virtual void beginStateField() {}
    virtual void stateField() {}
    virtual void endStateField() {}
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
    void startDMode(CPlayerRiku* inRiku);

private:
    virtual void initTransform(INT inFlag);
    virtual void exeTransform();
    void transferControl();
    virtual void exeAvoid();
    void exeSpeedStep();
    virtual void setAttack();
    virtual void initJump2nd();
    virtual void exeJump2nd();
    virtual INT startStockSkillAction();
    virtual void initStockSkill(INT inNo);
    virtual void exeStockSkill();
    virtual void setMagic(INT inState, INT inCardID, INT inSkillID);
    virtual void initFriend(INT inNo);
    virtual void exeFriend();
    void searchWarpPos(FLOAT inDir);
};

extern PL_PARAM g_DRikuParam;

} // namespace PL

#endif /* GAME_PLAYER_PLAYERDRIKU_H */
