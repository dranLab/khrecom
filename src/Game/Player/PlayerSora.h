#ifndef GAME_PLAYER_PLAYERSORA_H
#define GAME_PLAYER_PLAYERSORA_H

#include "khtypes.h"
#include "Game/Player/Player.h"

namespace PL {

class CPlayerSora : public CPlayer {
private:
    HANDLE m_hMagicCore;      /* 0x770 */
    INT m_nHitCount;          /* 0x774 */
    INT m_nMagicBullet;       /* 0x778 */
    INT m_nMagicBulletMax;    /* 0x77c */
    HANDLE m_hMagicBullet[16]; /* 0x780 */

public:
    CPlayerSora(CTask* inParent);
    virtual ~CPlayerSora();

private:
    virtual INT startStockSkillAction();
    virtual void initStockSkill(INT inNo);
    virtual void exeStockSkill();
    virtual void initMagic(INT inNo);
    virtual void exeMagic();
    virtual void setMagic(INT inState, INT inCardID, INT inSkillID);
    virtual void initFriend(INT inNo);
    virtual void exeFriend();
    virtual void initSummon(INT inNo);
    virtual void exeSummon();
    virtual void initDuel(INT inFlag) {}
    virtual void exeDuel() {}
    void initMagicBullet();
    INT checkMagicBullet();
    void setMagicBullet(HANDLE inBullet);
    void watchTarget(INT inMode);
    virtual INT exeAttack_inFriend();
    virtual INT endAttack_inFriend();
    virtual void initGlide(INT inFlag);
    virtual void exeGlide();
    virtual void exeAvoid();
    void exeDogeRoll();
    virtual void initPronounce(INT inFlag);
    virtual void exePronounce();

public:
    virtual void setStateSE(INT inState, INT inKind);
    virtual INT getStateSESeq(INT inState);
};

extern PL_PARAM g_SoraParam;

} // namespace PL

#endif /* GAME_PLAYER_PLAYERSORA_H */
