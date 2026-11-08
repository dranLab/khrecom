// PARTIAL: added for Game/Debug/KisEnemy
#ifndef GAME_ENEMY_BATTLEENEMY_H
#define GAME_ENEMY_BATTLEENEMY_H

#include "khtypes.h"
#include "System/MainApp.h"
#include "Game/Entity.h"
#include "Game/CollParam.h"     // PARTIAL: added for Game/Enemy/EnemyBluerhapsody (ATK_KIND)
#include "Game/AttackMotion.h" // PARTIAL: added for Game/Enemy/EnemyBluerhapsody (ATK_ATTR)
#include "System/Fsm.h"
#include "System/Graphic/Effect/ObjType.h"

struct ENEMY_PARAM;
struct BASIC_ENEMY_PARAM;
struct ENEMY_ACT;
namespace BattleEnemy {
INT GameModeHp(INT inHp);
FLOAT GameModeAp(FLOAT inAp);
}

#include "Game/CardKindEnum.h"



/* Effect names: file-local statics of the header (every includer has its own copy in .data). */
namespace BattleEnemy {
static CHAR* BadStatusEffectName = "abnormal";
static CHAR* RecoverEffectName = "cure01";
}

/* Placeholder for the original header's inline functions checking indices: their strings are in .rodata
 * of every includer, in this order. Line numbers are not known. */
#ifndef BATTLEENEMY_H_PATH // MATCH: path override for Game/Gimmick/CreateGimmick ("../Enemy/BattleEnemy.h")
#define BATTLEENEMY_H_PATH "Game/Enemy/BattleEnemy.h"
#endif
#line 1 BATTLEENEMY_H_PATH
inline void _battleEnemyCheckIndex(INT inIndex, INT inState) {
    if (!(0 <= inIndex && inIndex < 6)) AssertApp(__FILE__, __LINE__, "0 <= inIndex && inIndex < ATK_ATTR_MAX", 0);
    if (!(0 <= inIndex && inIndex < 2)) AssertApp(__FILE__, __LINE__, "0 <= inIndex && inIndex < 2", 0);
    if (!(inState < 8)) AssertApp(__FILE__, __LINE__, "BTL_ENEMY_STATE_ATTACK1 <= inState && inState < BTL_ENEMY_STATE_ATTACK_MAX", 0);
}

class CBattleEnemyManager;

struct STOP_DAMAGE {
    FVECTOR vDetectPos;
    char szHitEffectName[16];
    SHORT sHitEffectNo;
    SHORT sHitIndex;
    ObjType::eObjType hitEffectObjType;
    ATK_KIND dmgKind;
    INT iDamage;
};

class CBattleEnemy : public CEntity {
private: friend class CHumanBoss; friend class CHeartless; friend class CEnemyHeartsoldier; friend class CEnemySpadesoldier; /* MATCH: they use private members (the ref can't show friends): CHumanBoss reads m_pEnemyParam (getAttackCardNum, setupCardRevolver: the inline getter changes PRE) and sets m_iAttackState (setPlayCard, setPlayStock), CHeartless reads m_iAttackState and calls canAttack_ (aiCardBattle, canAttack, randomBattleCardNumber), all without an inline scope in the original .mdebug; CEnemyHeartsoldier/CEnemySpadesoldier call selectDamageMotion_ (beginStateDead) */
    FVECTOR m_vHitBackVel;       /* 0x1d0 */
    FVECTOR m_vConfusePlayerPos; /* 0x1e0 */
    FVECTOR m_vBackupVel;        /* 0x1f0 */
    ULONGLONG m_ullBtlEnemyFlag; /* 0x200 */
    ULONGLONG m_ullBackupFlag;   /* 0x208 */
    INT m_iSysStep;              /* 0x210 */
    INT m_iHp;                   /* 0x214 */
    INT m_iAttackState;          /* 0x218 */
    INT m_iGroupNo;              /* 0x21c */
    INT m_iActiveAttackRate;     /* 0x220 */
    INT m_iDamageCounter;        /* 0x224 */
    INT m_iHitCount;             /* 0x228 */
    TIMEQ m_tqDamageTimer;       /* 0x22c */
    FLOAT m_fDamageRate;         /* 0x230 */
    INT m_iParam;                /* 0x234 */
    INT m_iFallMotionIndex;      /* 0x238 */
    INT m_iLandMotionIndex;      /* 0x23c */
    CARD_KIND m_CardKind;        /* 0x240 */
    CARD_KIND m_DuelCardKind;    /* 0x244 */
    HANDLE m_hBadStatus;         /* 0x248 */
    HANDLE m_hDision;            /* 0x24c */
    HANDLE m_hFreeze;            /* 0x250 */
    HANDLE m_hRecover;           /* 0x254 */
    HANDLE m_hDeath;             /* 0x258 */
    HANDLE m_hHitEffect;         /* 0x25c */
    int m_hSpotLight[2];         /* 0x260 */
    TIMEQ m_tqCancelTimer;       /* 0x268 */
    TIMEQ m_tqBindTimer;         /* 0x26c */
    TIMEQ m_tqConfuseTimer;      /* 0x270 */
    TIMEQ m_tqHitBackTimer;      /* 0x274 */
    TIMEQ m_tqTime;              /* 0x278 */
    FLOAT m_fDamage;             /* 0x27c */
    FLOAT m_fBodyRviseCoefficient; /* 0x280 */
    INT m_iExp;                  /* 0x284 */
    short m_sAttrDmgSeIdentifyNo[6]; /* 0x288 */
    INT m_iCardBreakSeIdentifyNo; /* 0x294 */
    CState* m_pDefaultState;     /* 0x298 */
    CState* m_pMainState;        /* 0x29c */
    ENEMY_PARAM* m_pEnemyParam;  /* 0x2a0 */
    BASIC_ENEMY_PARAM* m_pBasicParam; /* 0x2a4 */
    MES_CARD_INFO m_MesCardInfo; /* 0x2a8 */
protected:
    SHORT m_sStopDamageCount;    /* 0x2cc */
    SHORT m_sStopDamagePtr;      /* 0x2ce */
    STOP_DAMAGE m_StopDamage[8]; /* 0x2d0 */
private:
    CStateTemplate<CBattleEnemy> m_EnemyState[31]; /* 0x450 */
    static void (CBattleEnemy::*m_pBeginStateTbl[])();
    static void (CBattleEnemy::*m_pStateTbl[])();
    static void (CBattleEnemy::*m_pEndStateTbl[])();
    static INT m_iDuelCardNum;
    static INT m_iDuelCardPtr;
    static int m_iDuelCardArray[10];

public:
    CBattleEnemy(CTask* inParent, CState* inDefaultState, CState* inMainState);
    virtual ~CBattleEnemy();
    virtual INT getTypeID() const;
    virtual void main();
    virtual void update();
    virtual BOOL initalize(INT inGroupID, INT inRsrcID, UINT iFlag);
    virtual BOOL initalize(INT inGroupID, INT inRsrcID) { return initalize(inGroupID, inRsrcID, 0); }
    virtual void onMsg(INT inMsg, void* inExtraInfo);
    virtual void onOut();
    void setBattleEnemyFlag(ULONGLONG inOn, ULONGLONG inOff) { m_ullBtlEnemyFlag = (m_ullBtlEnemyFlag | inOn) & ~inOff; }
    void setHp(INT inHp) { m_iHp = inHp; }
    void setGroupNo(INT inNo) { m_iGroupNo = inNo; }
    void setDefaultState(CState* inState) { m_pDefaultState = inState; }
    void setFallMotionIndex(INT inIndex) { m_iFallMotionIndex = inIndex; }
    void setLandMotionIndex(INT inIndex) { m_iLandMotionIndex = inIndex; }
    void setParam(INT inParam) { m_iParam = inParam; }
    void setBodyRviseCoefficient(FLOAT inCoefficient) { m_fBodyRviseCoefficient = inCoefficient; }
#line 226 BATTLEENEMY_H_PATH
    void setAttrDmgSeIdentifyNo(INT inIndex, INT inNo) {
        if (!(0 <= inIndex && inIndex < 6)) AssertApp(__FILE__, __LINE__, "0 <= inIndex && inIndex < ATK_ATTR_MAX", 0);
        m_sAttrDmgSeIdentifyNo[inIndex] = inNo;
    }
    void setCardBreakSeIdentifyNo(INT inNo) { m_iCardBreakSeIdentifyNo = inNo; }
    void setExp(INT inExp) { m_iExp = inExp; }
    UINT getBattleEnemyFlag() const { return m_ullBtlEnemyFlag; }
    INT getHp() const { return m_iHp; }
    INT getGroupNo() const { return m_iGroupNo; }
    HANDLE getNearestChild(FLOAT& outDist, CEntity& inEntity);
    CARD_KIND getCardKind() const { return m_CardKind; }
    CARD_KIND getDuelCardKind() const { return m_DuelCardKind; }
    INT getParam() const { return m_iParam; }
    INT getHpMax() const;
    INT getSize() const;
    FLOAT getExtraParam(const CHAR* inParamName);
    FLOAT getAttackPoint(INT inIndex, ATK_KIND inAtkKind, USHORT inUpFlag, ATK_KIND* outAtkKind, INT inFinish);
    ATK_ATTR getAttackAttr(INT inIndex) const;
    INT getAttackRate() const;
    ENEMY_PARAM* getEnemyParam() const { return m_pEnemyParam; }
    INT getHpPercent() const { return getHp() * 100 / getHpMax(); }
    virtual void getUseCardPos(FVECTOR& outPos);
    const MES_CARD_INFO& getMesCardInfo() const { return m_MesCardInfo; }
    TIMEQ getStopTime() const { return m_tqTime; }
    INT getExp() const { return m_iExp; }
    ULONGLONG isCardBattle() const { return m_ullBtlEnemyFlag & 0x1ULL; }
    ULONGLONG isCardBattleFailed() const { return m_ullBtlEnemyFlag & 0x2ULL; }
    ULONGLONG isAttack() const { return m_ullBtlEnemyFlag & 0x4ULL; }
    ULONGLONG isStartFaint() const { return m_ullBtlEnemyFlag & 0x8ULL; }
    ULONGLONG isPermissionCardBattle() const { return m_ullBtlEnemyFlag & 0x10ULL; }
    ULONGLONG isNoCreateCard() const { return m_ullBtlEnemyFlag & 0x20ULL; }
    ULONGLONG isHitAttack() const { return m_ullBtlEnemyFlag & 0x40ULL; }
    LONGLONG isReload() const { return m_ullBtlEnemyFlag & 0x80ULL; }
    ULONGLONG isFly() const { return m_ullBtlEnemyFlag & 0x100ULL; }
    ULONGLONG isPutCardReady() const { return m_ullBtlEnemyFlag & 0x200ULL; }
    ULONGLONG isNoHpSub() const { return m_ullBtlEnemyFlag & 0x400ULL; }
    ULONGLONG isStartCardBattle() const { return m_ullBtlEnemyFlag & 0x800ULL; }
    ULONGLONG isCounter() const { return m_ullBtlEnemyFlag & 0x1000ULL; }
    ULONGLONG isInvincible() const { return m_ullBtlEnemyFlag & 0x2000ULL; }
    ULONGLONG isMatchless() const { return m_ullBtlEnemyFlag & 0x4000ULL; }
    ULONGLONG isNoHitBack() const { return m_ullBtlEnemyFlag & 0x8000ULL; }
    ULONGLONG isNoPlayDamageMotion() const { return m_ullBtlEnemyFlag & 0x10000ULL; }
    ULONGLONG isBind() const { return m_ullBtlEnemyFlag & 0x20000ULL; }
    ULONGLONG isConfuse() const { return m_ullBtlEnemyFlag & 0x40000ULL; }
    ULONGLONG isNoDmgResponse() const { return m_ullBtlEnemyFlag & 0x80000ULL; }
    ULONGLONG isNoBigDamage() const { return m_ullBtlEnemyFlag & 0x100000ULL; }
    ULONGLONG isNoSmallDmgResponse() const { return m_ullBtlEnemyFlag & 0x200000ULL; }
    ULONGLONG isNoMakePrize() const { return m_ullBtlEnemyFlag & 0x400000ULL; }
    ULONGLONG isHitBody() const { return m_ullBtlEnemyFlag & 0x800000ULL; }
    ULONGLONG isHitPoints() const { return m_ullBtlEnemyFlag & 0x1000000ULL; }
    ULONGLONG isOnDamageEndCardBattle() const { return m_ullBtlEnemyFlag & 0x2000000ULL; }
    ULONGLONG isNoSetHitEffect() const { return m_ullBtlEnemyFlag & 0x4000000ULL; }
    ULONGLONG isAlwaysSetCard() const { return m_ullBtlEnemyFlag & 0x8000000ULL; }
    ULONGLONG isDamageAir() const { return m_ullBtlEnemyFlag & 0x10000000ULL; }
    ULONGLONG isStockAct() const { return m_ullBtlEnemyFlag & 0x20000000ULL; }
    ULONGLONG isNotifyDead() const { return m_ullBtlEnemyFlag & 0x40000000ULL; }
    ULONGLONG isRotMove() const { return m_ullBtlEnemyFlag & 0x80000000ULL; }
    ULONGLONG isStartDuel() const { return m_ullBtlEnemyFlag & 0x100000000ULL; }
    ULONGLONG isNoFlick() const { return m_ullBtlEnemyFlag & 0x200000000ULL; }
    ULONGLONG isNoPlayDamageSE() const { return m_ullBtlEnemyFlag & 0x400000000ULL; }
    ULONGLONG isNoCardBattle() const { return m_ullBtlEnemyFlag & 0x800000000ULL; }
    ULONGLONG isStartConfuse() const { return m_ullBtlEnemyFlag & 0x1000000000ULL; }
    ULONGLONG isNoMakeFriendCard() const { return m_ullBtlEnemyFlag & 0x2000000000ULL; }
    ULONGLONG isNoDuelPlayerTurn() const { return m_ullBtlEnemyFlag & 0x4000000000ULL; }
    ULONGLONG isNoResponseTornado() const { return m_ullBtlEnemyFlag & 0x8000000000ULL; }
    ULONGLONG isNoDmgResponseOnFreeze() const { return m_ullBtlEnemyFlag & 0x10000000000ULL; }
    ULONGLONG isStartSleep() const { return m_ullBtlEnemyFlag & 0x20000000000ULL; }
    ULONGLONG isTerror() const { return m_ullBtlEnemyFlag & 0x40000000000ULL; }
    ULONGLONG isFaint() const { return m_ullBtlEnemyFlag & 0x80000000000ULL; }
    ULONGLONG isNoDmgResponseOnMagnet() const { return m_ullBtlEnemyFlag & 0x100000000000ULL; }
    ULONGLONG isNoKillCount() const { return m_ullBtlEnemyFlag & 0x200000000000ULL; }
    BOOL isPlayingCardBattle();
    BOOL isCurrentState(INT inState);
    BOOL isValidSynchro();
    BOOL canCardBattle();
    BOOL canFlick() const {
        if (isNoDmgResponse()) return FALSE;
        if (isNoBigDamage()) return FALSE;
        return TRUE;
    }
    BOOL canKill();
    void changeMainState(INT inState);
    virtual void changeMotion(INT inIndex, TIMEQ inTq, BOOL inRootMove, TIMEQ inGr, INT inEffectNo, INT inSENo);
    BOOL changeMotionIndex(INT inIndex, TIMEQ inTq, BOOL inRootMove, TIMEQ inGr, INT inEffectNo, INT inSENo);
#line 399 BATTLEENEMY_H_PATH
    void changeAttackRate(INT inIndex) {
        if (!(0 <= inIndex && inIndex < 2)) AssertApp(__FILE__, __LINE__, "0 <= inIndex && inIndex < 2", 0);
        m_iActiveAttackRate = inIndex;
    }
    virtual BOOL startCardBattle();
    void endCardBattle(CState* inState, TIMEQ inNextAttackWait);
    INT checkHitPos();
    void playerHomingY(FLOAT inRate);
    void homingToPlayer(FLOAT inRate);
    void homingToPlayerY(FLOAT inRate);
    BOOL turnToPlayer(FLOAT inLimit);
    BOOL turnToPlayerY(FLOAT inLimit);
    void homingMoveToPlayer(FLOAT inRate, FLOAT inSpeed);
    void homingMoveXZToPlayer(FLOAT inRate, FLOAT inSpeed);
    void getPlayerPosition(FVECTOR& outPos);
    HANDLE getPlayerHandle();
    void makeExpPrize();
    void makePrize();
    virtual void bossDeadDemoCallback() {}
    virtual void setBossDeadEffect();
    virtual void killBossDeadEffect();
    virtual void bossCardRevolverDestroy() {}
    virtual void bossDeadDemoDispOff();
    virtual BOOL bossDeadDemoSpecial() { return TRUE; }
    virtual void bossDeadDemoEnd() {}
    HANDLE recover(INT inPoint, const CHAR* inEffectName, INT inEffectNo, const FVECTOR& inPos, const CCoord* inParent);
    void kill();
    void attackStateCardDestroy(BOOL inColl);
    virtual void onBio(INT inParam);
    virtual void onGift(INT inParam);
    static INT getPlayerState();
    static void setBossKilledFlag();

protected:
    virtual void updateBefore();
    virtual void updateAfter();
    virtual void updatePositionAfter();
    virtual void lastUpdate();
    void changeDefaultState();
    virtual void beginStateAppear();
    virtual void stateAppear();
    virtual void endStateAppear();
    virtual void beginStateMain() {}
    virtual void stateMain() {}
    virtual void endStateMain() {}
    virtual void beginStateAttack1() {}
    virtual void stateAttack1() {}
    virtual void endStateAttack1() {}
    virtual void beginStateAttack2() {}
    virtual void stateAttack2() {}
    virtual void endStateAttack2() {}
    virtual void beginStateAttack3() {}
    virtual void stateAttack3() {}
    virtual void endStateAttack3() {}
    virtual void beginStateAttack4() {}
    virtual void stateAttack4() {}
    virtual void endStateAttack4() {}
    virtual void beginStateAttack5() {}
    virtual void stateAttack5() {}
    virtual void endStateAttack5() {}
    virtual void beginStateAttack6() {}
    virtual void stateAttack6() {}
    virtual void endStateAttack6() {}
    virtual void beginStateAttack7() {}
    virtual void stateAttack7() {}
    virtual void endStateAttack7() {}
    virtual void beginStateAttack8() {}
    virtual void stateAttack8() {}
    virtual void endStateAttack8() {}
    virtual void beginStateItem() {}
    virtual void stateItem() {}
    virtual void endStateItem() {}
    virtual void beginStateSmallDamage();
    virtual void stateSmallDamage();
    virtual void endStateSmallDamage();
    virtual void beginStateBigDamage();
    virtual void stateBigDamage();
    virtual void endStateBigDamage();
    virtual void beginStateCardBreak();
    virtual void stateCardBreak();
    virtual void endStateCardBreak();
    virtual void beginStateZero();
    virtual void stateZero();
    virtual void endStateZero();
    virtual void beginStateDuel();
    virtual void stateDuel();
    virtual void endStateDuel();
    virtual void beginStateDuelSuccess();
    virtual void stateDuelSuccess();
    virtual void endStateDuelSuccess();
    virtual void beginStateFaint();
    virtual void stateFaint();
    virtual void endStateFaint();
    virtual void beginStateStop();
    virtual void stateStop();
    virtual void endStateStop();
    virtual void beginStateTerror();
    virtual void stateTerror();
    virtual void endStateTerror();
    virtual void beginStateFreeze();
    virtual void stateFreeze();
    virtual void endStateFreeze();
    virtual void beginStateShock();
    virtual void stateShock();
    virtual void endStateShock();
    virtual void beginStateTornado();
    virtual void stateTornado();
    virtual void endStateTornado();
    virtual void beginStateDegenerator();
    virtual void stateDegenerator();
    virtual void endStateDegenerator();
    virtual void beginStateGravity();
    virtual void stateGravity();
    virtual void endStateGravity();
    virtual void beginStateToss();
    virtual void stateToss();
    virtual void endStateToss();
    virtual void beginStateBeat();
    virtual void stateBeat();
    virtual void endStateBeat();
    virtual void beginStateMagnet();
    virtual void stateMagnet();
    virtual void endStateMagnet();
    virtual void beginStateSuspend();
    virtual void stateSuspend();
    virtual void endStateSuspend();
    virtual void beginStateVolley();
    virtual void stateVolley();
    virtual void endStateVolley();
    virtual void beginStateDead();
    virtual void stateDead();
    virtual void endStateDead();
    virtual void aiCardBattle() {}
    virtual void onGuard() {}
    virtual void onDamage() {}
    virtual void onAbsorb() {}
    virtual void onInvalid() {}
    virtual void onGimmickCard() {}
    virtual void onCardBreak() {}
    virtual void onReflect() {}
    virtual void onAttackCancel() {}
    virtual void onCardBattleFailed() {}
    virtual INT getDuelAttackState() { return -1; }
    virtual CEntity* createFriendCard(CARD_KIND inKind, INT inParam);
    virtual void notifyHP();
    CBattleEnemyManager* getBattleEnemyManager() const;
#line 673 BATTLEENEMY_H_PATH
    void setAttackState(INT inState) {
        if (!(2 <= inState && inState < 10)) AssertApp(__FILE__, __LINE__, "BTL_ENEMY_STATE_ATTACK1 <= inState && inState < BTL_ENEMY_STATE_ATTACK_MAX", 0);
        m_iAttackState = inState;
    }
    void makePrize(const FVECTOR* inPos);
    void makeExpPrize(const FVECTOR* inPos);
    void makePremiumPrize(const FVECTOR* inPos);
    void makeRoulettePrize(const FVECTOR* inPos);
    void makeCard(const FVECTOR* inPos);
    void changeBattleEnemyState(INT inState);
    void setBadStatusEffect(INT inKind, FLOAT inParam);
    void killBadStatusEffect();
    void setupFlick();
    virtual void commonBeginAttack();
    virtual void commonEndAttack();
    void notifyDead();
    void setupHitBack();
    BOOL initEnemyParam(INT inGroupID, INT inRsrcID, INT inIndex);
    void playSmallDamageSE();
    void playBigDamageSE();
    void playDamageSE(ATK_KIND inDmgKind);
    BOOL setupDuelCard();
    BOOL destroyCard();
    BOOL canAttack(const ENEMY_ACT* inAct) { return canAttack_(inAct); }
    void changeDamageMotion(INT inIndex, TIMEQ inTq, BOOL inRootMove, TIMEQ inGr);
    BOOL checkResist_(ATK_KIND inDmgKind);

private:
    void beginStateAppear_();
    void stateAppear_();
    void endStateAppear_();
    void beginStateMain_();
    void stateMain_();
    void endStateMain_();
    void beginStateAttack1_();
    void stateAttack1_();
    void endStateAttack1_();
    void beginStateAttack2_();
    void stateAttack2_();
    void endStateAttack2_();
    void beginStateAttack3_();
    void stateAttack3_();
    void endStateAttack3_();
    void beginStateAttack4_();
    void stateAttack4_();
    void endStateAttack4_();
    void beginStateAttack5_();
    void stateAttack5_();
    void endStateAttack5_();
    void beginStateAttack6_();
    void stateAttack6_();
    void endStateAttack6_();
    void beginStateAttack7_();
    void stateAttack7_();
    void endStateAttack7_();
    void beginStateAttack8_();
    void stateAttack8_();
    void endStateAttack8_();
    void beginStateItem_();
    void stateItem_();
    void endStateItem_();
    void beginStateSmallDamage_();
    void stateSmallDamage_();
    void endStateSmallDamage_();
    void beginStateBigDamage_();
    void stateBigDamage_();
    void endStateBigDamage_();
    void beginStateCardBreak_();
    void stateCardBreak_();
    void endStateCardBreak_();
    void beginStateZero_();
    void stateZero_();
    void endStateZero_();
    void beginStateDuel_();
    void stateDuel_();
    void endStateDuel_();
    void beginStateDuelSuccess_();
    void stateDuelSuccess_();
    void endStateDuelSuccess_();
    void beginStateFaint_();
    void stateFaint_();
    void endStateFaint_();
    void beginStateStop_();
    void stateStop_();
    void endStateStop_();
    void beginStateTerror_();
    void stateTerror_();
    void endStateTerror_();
    void beginStateFreeze_();
    void stateFreeze_();
    void endStateFreeze_();
    void beginStateShock_();
    void stateShock_();
    void endStateShock_();
    void beginStateTornado_();
    void stateTornado_();
    void endStateTornado_();
    void beginStateDegenerator_();
    void stateDegenerator_();
    void endStateDegenerator_();
    void beginStateGravity_();
    void stateGravity_();
    void endStateGravity_();
    void beginStateToss_();
    void stateToss_();
    void endStateToss_();
    void beginStateBeat_();
    void stateBeat_();
    void endStateBeat_();
    void beginStateMagnet_();
    void stateMagnet_();
    void endStateMagnet_();
    void beginStateSuspend_();
    void stateSuspend_();
    void endStateSuspend_();
    void beginStateVolley_();
    void stateVolley_();
    void endStateVolley_();
    void beginStateDead_();
    void stateDead_();
    void endStateDead_();
    void initFSM_();
    void initRoom_();
    BOOL hitTest_();
    INT computeDamage_(FLOAT inDamage, FLOAT inAttr);
    INT computeDamage_(FLOAT inDamage, FLOAT inAttr, ATK_KIND inAtkKind);
    void changeDamageState_(ATK_KIND inDmgKind);
    BOOL cancelTimeOver_() const { return m_tqCancelTimer <= 0; }
    BOOL canAttack_(const ENEMY_ACT* inAct);
    INT selectDamageMotion_();
    void enableBind_(BOOL inEnable);
    void enableConfuse_(BOOL inEnable);
    void setGuardEffect_(const FVECTOR* inPos);
    FLOAT getDamageRate_() const;
    BOOL isLastLeave_(INT inResultHp);
    BOOL isReraise_();
    BOOL isQuickRecover_();
    void enemyCardDash_();
    void createFriendCard_(INT inRate);
    void setHitEffect_();
    void cardBattleProc_();
    void reviseObject_();
    void updateBindTimer_();
    void updateConfuseTimer_();
    void updateHitBackTimer_();
    void updateDamageTimer_();
    BOOL isNowPlayDamageMotion_();
    BOOL addEnemyKillCount_();
    BOOL changeSmallDamageState_(ATK_KIND inDmgKind);
    void updateSpotLight_();
    BOOL canCreateEnemyCard_();
    void continuousDamageProc_();
    BOOL canBlackroomCard_();
};

#endif /* GAME_ENEMY_BATTLEENEMY_H */
