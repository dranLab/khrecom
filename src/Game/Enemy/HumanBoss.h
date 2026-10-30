#ifndef GAME_ENEMY_HUMANBOSS_H
#define GAME_ENEMY_HUMANBOSS_H

#include "khtypes.h"
#include "Game/Enemy/BattleEnemy.h"

/* Sequence/parameter names: file-local statics of the header (every includer has its own copy in .data). */
namespace HumanBoss {
static CHAR* NormalEyeSeqName = "EYE_BLINK";
static CHAR* NormalLipSeqName = "LIP_DEFAULT";
static CHAR* DamageEyeSeqName = "EYE_DAMAGE";
static CHAR* DamageLipSeqName = "LIP_DAMAGE";
static CHAR* FaintEyeSeqName = "EYE_DAMAGE";
static CHAR* FaintLipSeqName = "LIP_DAMAGE";
static CHAR* DeadEyeSeqName = "EYE_DAMAGE";
static CHAR* DeadLipSeqName = "LIP_DAMAGE";
static CHAR* ItemMotionParamName = "ItemMot";
static CHAR* VoiceIDParamName = "VoiceID";
static CHAR* MyHpRemainParamName = "MyHpRemain";
static CHAR* MyHpVoiceParamName = "MyHpVoice";
static CHAR* PlHpRemainParamName = "PlHpRemain";
static CHAR* PlHpVoiceParamName = "PlHpVoice";
static CHAR* ReloadVoiceParamName = "ReloadVoice";
static CHAR* ECardVoiceParamName = "ECardVoice";
static CHAR* UseItemCardParamName = "UseItemCard";

INT GetPlayerHpPercent();
}

struct CARD;
namespace CardBattle {
class CCardRevolver;
}

/* Base of the human bosses: a card deck (CCardRevolver) driven by normal acts and stock (sleight) acts. */
class CHumanBoss : public CBattleEnemy {
public:
    struct CARD_DATA {
        INT iID;
        INT iCardKind[8]; /* CARD_KIND */
    };

    struct ACT_DATA {
        INT iState;
        ENEMY_ACT* pAct;
        BOOL bEnable;
        SHORT sMinNumber;
        SHORT sMaxNumber;
    };

protected:
    INT m_iStep;                              /* 0x830 */
    INT m_iUseItem;                           /* 0x834 */
    CardBattle::CCardRevolver* m_pCardRevolver; /* 0x838 */
    INT m_iNormalActNum;                      /* 0x83c */
    ACT_DATA* m_pNormalActs;                  /* 0x840 */
    INT m_iStockNum;                          /* 0x844 */
    ACT_DATA* m_pStockData;                   /* 0x848 */
    INT m_iStockCardCount;                    /* 0x84c */
    ACT_DATA* m_pUseStock;                    /* 0x850 */
    CARD_DATA* m_pCardData;                   /* 0x854 */
    INT m_iActCount;                          /* 0x858 */
    ACT_DATA* m_pDoBattleAct;                 /* 0x85c */
    INT m_iDeadSeIdentifyNo;                  /* 0x860 */
    INT m_iNoReloadNum;                       /* 0x864 */
    INT m_iRemainCardNum;                     /* 0x868 */
private:
    UINT m_uiHumanBossFlag;                   /* 0x86c */
    INT m_iStockRate;                         /* 0x870 */
    INT m_iUseStockRate;                      /* 0x874 */
    INT m_iTotalCardNum;                      /* 0x878 */
    INT m_iStockTotalNumber;                  /* 0x87c */
    INT m_iStockRollCount;                    /* 0x880 */

public:
    CHumanBoss(CTask* inParent, CState* inDefaultState, CState* inMainState);
    virtual ~CHumanBoss();
    virtual BOOL initalize(INT inGroupID, INT inRsrcID);
    virtual BOOL initalize(INT inGroupID, INT inRsrcID, UINT iFlag);
    void setHumanBossFlag(UINT inSet, UINT inMask);
    INT getTotalCardNum() const;
    INT getAttackCardNum(UINT inFlag) const;
    INT getRemainCard(UINT inFlag);
    INT getRemainEnemyCard(INT inKind);
    INT getHaveCard(UINT inFlag);
    UINT isNoEnemyCard() const;
    UINT isStopCardBattle() const;
    UINT isPlayerHpRemainVoice() const;
    UINT isMyHpRemainVoice() const;
    UINT isStartRollDeck() const;
    UINT isEndRollDeck() const;
    UINT isReturnStock() const;
    UINT isPerMissionStockZero() const;

protected:
    virtual void updateAfter();
    virtual void beginStateItem();
    virtual void stateItem();
    virtual void beginStateSmallDamage();
    virtual void endStateSmallDamage();
    virtual void beginStateBigDamage();
    virtual void endStateBigDamage();
    virtual void beginStateFaint();
    virtual void endStateFaint();
    virtual void beginStateDead();
    virtual void stateDead();
    virtual void endStateDead();
    virtual void setupCardRevolver();
    virtual void setupAct();
    virtual void aiCardBattle();
    virtual INT getDuelAttackState();
    virtual void selectStockAct();
    BOOL testEdge();
    void setStockRate(INT inRate);
    void setUseStockRate(INT inRate);
    void setNoReloadNum(INT inNum);
    ACT_DATA* getStockAct(INT inState) const;
    void enableStockAct(INT inState, BOOL inSw);
    void enableNormalAct(INT inState, BOOL inSw);
    void setStockActMaxMinNumber(INT inState, SHORT inMax, SHORT inMin);
    virtual void bossCardRevolverDestroy();
    BOOL returnStockAct(INT inFlag);
    virtual BOOL reloadCardProc();
    virtual BOOL enemyCardProc(const CARD* inActiveCard);
    virtual BOOL itemCardProc(const CARD* inActiveCard);
    virtual BOOL stockActProc(const CARD* inActiveCard);
    virtual BOOL normalActProc(const CARD* inActiveCard);
    void setDoBattleAct(const ACT_DATA* inAct);
    void useCard(INT inState);
    virtual void playMyRemainHpVoice();
    virtual void playPlayerRemainHpVoice();
    virtual void playReloadCompleteVoice();
    virtual void playUseEnemyCardVoice();
    virtual BOOL canEnemyCard();
    virtual BOOL canEnemyCard(const CARD* inActiveCard);
    BOOL canStockAct(const ACT_DATA* inAct);
    BOOL setStockCard(const CARD* inActiveCard);
    BOOL setPlayCard(INT inState);
    BOOL setPlayStock(INT inState);
    INT selectDamageMotion();
    void clearStockTotalNumber();
};

namespace HumanBoss {
extern CHumanBoss::CARD_DATA g_CardDataTbl[12];
}

/* The inline members, in the order of their weak copies in Game/Enemy/HumanBoss. */
inline BOOL CHumanBoss::initalize(INT inGroupID, INT inRsrcID) { return initalize(inGroupID, inRsrcID, 0); }
inline void CHumanBoss::setHumanBossFlag(UINT inSet, UINT inMask) {
    m_uiHumanBossFlag = (m_uiHumanBossFlag | inSet) & ~inMask;
}
inline INT CHumanBoss::getTotalCardNum() const { return m_iTotalCardNum; }
inline UINT CHumanBoss::isNoEnemyCard() const { return m_uiHumanBossFlag & 0x1; }
inline UINT CHumanBoss::isStopCardBattle() const { return m_uiHumanBossFlag & 0x2; }
inline UINT CHumanBoss::isPlayerHpRemainVoice() const { return m_uiHumanBossFlag & 0x4; }
inline UINT CHumanBoss::isMyHpRemainVoice() const { return m_uiHumanBossFlag & 0x8; }
inline UINT CHumanBoss::isStartRollDeck() const { return m_uiHumanBossFlag & 0x10; }
inline UINT CHumanBoss::isEndRollDeck() const { return m_uiHumanBossFlag & 0x20; }
inline UINT CHumanBoss::isReturnStock() const { return m_uiHumanBossFlag & 0x40; }
inline UINT CHumanBoss::isPerMissionStockZero() const { return m_uiHumanBossFlag & 0x80; }
inline void CHumanBoss::setStockRate(INT inRate) { m_iStockRate = inRate; }
inline void CHumanBoss::setUseStockRate(INT inRate) { m_iUseStockRate = inRate; }
inline void CHumanBoss::setNoReloadNum(INT inNum) { m_iNoReloadNum = inNum; }
inline BOOL CHumanBoss::canEnemyCard() { return TRUE; }
inline BOOL CHumanBoss::canEnemyCard(const CARD* inActiveCard) { return TRUE; }
inline INT CHumanBoss::selectDamageMotion() { return selectDamageMotion_(); }
inline void CHumanBoss::clearStockTotalNumber() {
    m_iStockTotalNumber = 0;
    m_iStockRollCount = 0;
}

#endif /* GAME_ENEMY_HUMANBOSS_H */
