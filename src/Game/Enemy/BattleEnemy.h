// PARTIAL: added for Game/Debug/KisEnemy
#ifndef GAME_ENEMY_BATTLEENEMY_H
#define GAME_ENEMY_BATTLEENEMY_H

#include "khtypes.h"
#include "System/MainApp.h"
#include "Game/Entity.h"
#include "Game/CollParam.h"     // PARTIAL: added for Game/Enemy/EnemyBluerhapsody (ATK_KIND)
#include "Game/AttackMotion.h" // PARTIAL: added for Game/Enemy/EnemyBluerhapsody (ATK_ATTR)
#ifdef BATTLEENEMY_H_STATE_TEMPLATE // PARTIAL: Game/Enemy/BattleEnemy sees m_EnemyState as the real CStateTemplate array
#include "System/Fsm.h"
#endif

struct ENEMY_PARAM; // PARTIAL: added for Game/Enemy/EnemyAirpirate
struct ENEMY_ACT;   // PARTIAL: added for Game/Enemy/Heartless
namespace BattleEnemy {
INT GameModeHp(INT inHp); // PARTIAL: added for Game/Enemy/BattleEnemy
}

enum CARD_KIND {
    CARD_KIND_None = 0 /* values not needed yet */
};

/* Effect names: file-local statics of the header (every includer has its own copy in .data). */
namespace BattleEnemy {
static CHAR* BadStatusEffectName = "abnormal";
static CHAR* RecoverEffectName = "cure01";
}

/* Placeholder for the original header's inline functions checking indices: their strings are in .rodata
 * of every includer, in this order. Line numbers are not known. */
#ifndef BATTLEENEMY_H_PATH // PARTIAL: path override added for Game/Gimmick/CreateGimmick ("../Enemy/BattleEnemy.h")
#define BATTLEENEMY_H_PATH "Game/Enemy/BattleEnemy.h"
#endif
#line 1 BATTLEENEMY_H_PATH
inline void _battleEnemyCheckIndex(INT inIndex, INT inState) {
    if (!(0 <= inIndex && inIndex < 6)) AssertApp(__FILE__, __LINE__, "0 <= inIndex && inIndex < ATK_ATTR_MAX", 0);
    if (!(0 <= inIndex && inIndex < 2)) AssertApp(__FILE__, __LINE__, "0 <= inIndex && inIndex < 2", 0);
    if (!(inState < 8)) AssertApp(__FILE__, __LINE__, "BTL_ENEMY_STATE_ATTACK1 <= inState && inState < BTL_ENEMY_STATE_ATTACK_MAX", 0);
}

class CBattleEnemyManager;

class CBattleEnemy : public CEntity {
private:
    // PARTIAL: split into the original members for Game/Enemy/BattleEnemy (same layout)
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
    HANDLE m_hSpotLight[2];      /* 0x260 */
    TIMEQ m_tqCancelTimer;       /* 0x268 */
    TIMEQ m_tqBindTimer;         /* 0x26c */
    TIMEQ m_tqConfuseTimer;      /* 0x270 */
    TIMEQ m_tqHitBackTimer;      /* 0x274 */
    TIMEQ m_tqTime;              /* 0x278 */
    FLOAT m_fDamage;             /* 0x27c */
protected:
    FLOAT m_fBodyRviseCoefficient; /* 0x280 */
private:
    INT m_iExp;                  /* 0x284 */
    short m_sAttrDmgSeIdentifyNo[6]; /* 0x288 */
    INT m_iCardBreakSeIdentifyNo; /* 0x294 */
    CState* m_pDefaultState;     /* 0x298 */
    CState* m_pMainState;        /* 0x29c */
    ENEMY_PARAM* m_pEnemyParam;  /* 0x2a0 */
    struct BASIC_ENEMY_PARAM* m_pBasicParam; /* 0x2a4 */
    UBYTE m_MesCardInfo[0x24];   /* 0x2a8 MES_CARD_INFO */
    SHORT m_sStopDamageCount;    /* 0x2cc */
    SHORT m_sStopDamagePtr;      /* 0x2ce */
    UBYTE m_StopDamage[0x180];   /* 0x2d0 STOP_DAMAGE[8] */
#ifdef BATTLEENEMY_H_STATE_TEMPLATE
    CStateTemplate<CBattleEnemy> m_EnemyState[31]; /* 0x450 */
    static void (CBattleEnemy::*m_pBeginStateTbl[])();
    static void (CBattleEnemy::*m_pStateTbl[])();
    static void (CBattleEnemy::*m_pEndStateTbl[])();
#else
    UBYTE m_EnemyState[0x3e0];   /* 0x450 CStateTemplate<CBattleEnemy>[31] */
#endif

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
    void setHp(INT inHp) { m_iHp = inHp; }
    INT getHp() const { return m_iHp; } // PARTIAL: added for Game/Enemy/EnemyGreenrequiem
    void setFallMotionIndex(INT inIndex) { m_iFallMotionIndex = inIndex; } // PARTIAL: added for Game/Enemy/EnemyPirate
    void setLandMotionIndex(INT inIndex) { m_iLandMotionIndex = inIndex; }
    // PARTIAL: added for Game/Player/FriendBeast (original names of the flag tests unknown)
    ULONGLONG isBtlEnemyFlag(ULONGLONG inFlag) const { return m_ullBtlEnemyFlag & inFlag; }
    // PARTIAL: added for Game/Player/PlayerRiku (original name unknown)
    BOOL isActiveTarget() const { return !isBtlEnemyFlag(0x80000) && !isBtlEnemyFlag(0x100000); }
    // PARTIAL: added for Game/Enemy/EnemyDarkball (original name unknown)
    void setBtlEnemyFlag(ULONGLONG inFlag) { m_ullBtlEnemyFlag |= inFlag; }
    void setBattleEnemyFlag(ULONGLONG inOn, ULONGLONG inOff) { m_ullBtlEnemyFlag = (m_ullBtlEnemyFlag | inOn) & ~inOff; }
    void clearBtlEnemyFlag(ULONGLONG inFlag) { m_ullBtlEnemyFlag &= ~inFlag; } // PARTIAL: added for Game/Enemy/EnemyTornadostep
    void getPlayerPosition(FVECTOR& outPos);
    FLOAT getExtraParam(const CHAR* inParamName);
    void endCardBattle(CState* inState, TIMEQ inNextAttackWait);
    BOOL turnToPlayer(FLOAT inLimit);
    // PARTIAL: added for Game/Enemy/EnemySearchghost
    BOOL turnToPlayerY(FLOAT inLimit);
    void playerHomingY(FLOAT inRate); // PARTIAL: added for Game/Enemy/EnemySeaneon
    // PARTIAL: added for Game/Enemy/EnemyBluerhapsody
    FLOAT getAttackPoint(INT inIndex, ATK_KIND inAtkKind, USHORT inUpFlag, ATK_KIND* outAtkKind, INT inFinish);
    ATK_ATTR getAttackAttr(INT inIndex) const;
    HANDLE getPlayerHandle(); // PARTIAL: added for Game/Enemy/EnemyFatbandit
    void makePrize();
    void setupFlick();
    BOOL isQuickRecover_();
    void enemyCardDash_();
    void continuousDamageProc_();
    BOOL isNowPlayDamageMotion_();
    BOOL canBlackroomCard_();
    INT checkHitPos();
    BOOL isLastLeave_(INT inResultHp);
    BOOL isReraise_();
    BOOL setupDuelCard();
    void cardBattleProc_();
    void setGuardEffect_(const FVECTOR* inPos);
    void reviseObject_();
    void setHitEffect_();
    void updateSpotLight_();
    INT computeDamage_(FLOAT inDamage, FLOAT inAttr);
    BOOL initEnemyParam(INT inGroupID, INT inRsrcID, INT inIndex);
    void initFSM_();
    void setupHitBack();
    void makeRoulettePrize(const FVECTOR* inPos);
    BOOL checkResist_(ATK_KIND inDmgKind);
    CBattleEnemyManager* getBattleEnemyManager() const;
    void makeExpPrize(const FVECTOR* inPos);
    void makePremiumPrize(const FVECTOR* inPos);
    void makeCard(const FVECTOR* inPos);
    BOOL hitTest_();
    void homingMoveXZToPlayer(FLOAT inRate, FLOAT inSpeed);
    void createFriendCard_(INT inRate);
    void attackStateCardDestroy(BOOL inColl);
    void changeDamageMotion(INT inIndex, TIMEQ inTq, BOOL inRootMove, TIMEQ inGr);
    void updateHitBackTimer_();
    BOOL changeMotionIndex(INT inIndex, TIMEQ inTq, BOOL inRootMove, TIMEQ inGr, INT inEffectNo, INT inSENo);
    BOOL changeSmallDamageState_(ATK_KIND inDmgKind);
    void playDamageSE(ATK_KIND inDmgKind);
    // PARTIAL: added for Game/Enemy/EnemyHeartsoldier
    void changeDefaultState();
    void playSmallDamageSE();
    void playBigDamageSE();
    INT selectDamageMotion_();
    INT getParam() { return m_iParam; }
    void makeExpPrize();              // PARTIAL: added for Game/Enemy/BossDeadDemo
    static void setBossKilledFlag();
    static INT m_iDuelCardNum;        // PARTIAL: added for Game/Enemy/BattleEnemy
    static INT m_iDuelCardPtr;
    static int m_iDuelCardArray[10];
    void setParam(INT inParam) { m_iParam = inParam; } // PARTIAL: added for Game/Enemy/CreateEnemy
    // PARTIAL: added for Game/Enemy/BattleEnemyManager
    UINT getBattleEnemyFlag() const { return m_ullBtlEnemyFlag; }
    void setGroupNo(INT inNo) { m_iGroupNo = inNo; }
    INT getGroupNo() const { return m_iGroupNo; }
    HANDLE getNearestChild(FLOAT& outDist, CEntity& inEntity);
    ULONGLONG isPermissionCardBattle() const { return m_ullBtlEnemyFlag & 0x200; }
    INT getHpMax() const;
    BOOL isCurrentState(INT inState); // PARTIAL: added for Game/Enemy/HumanBoss
    INT getPlayerState();             // PARTIAL: added for Game/Enemy/HumanBoss
    // PARTIAL: added for Game/Enemy/EnemyAirpirate
    ENEMY_PARAM* getEnemyParam() { return m_pEnemyParam; }
    ENEMY_PARAM* getEnemyParam() const { return m_pEnemyParam; } // PARTIAL: added for Game/Enemy/HumanBoss
    INT getAttackRate() const;
    INT getAttackState() const { return m_iAttackState; } // PARTIAL: added for Game/Enemy/Heartless
    BOOL canAttack_(const ENEMY_ACT* inAct);
    BOOL destroyCard(); // PARTIAL: added for Game/Enemy/EnemyBarrelspider
    // PARTIAL: added for Game/Enemy/EnemyWhitemushroom
    void changeBattleEnemyState(INT inState);
    void makePrize(const FVECTOR* inPos);
    // PARTIAL: added for Game/Enemy/EnemyCreapplant
    void setBadStatusEffect(INT inKind, FLOAT inParam);
    void killBadStatusEffect();
    BOOL canCardBattle();
    HANDLE recover(INT inPoint, const CHAR* inEffectName, INT inEffectNo, const FVECTOR& inPos, const CCoord* inParent);
    virtual void getUseCardPos(FVECTOR& outPos);
    virtual void changeMotion(INT inIndex, TIMEQ inTq, BOOL inRootMove, TIMEQ inGr, INT inEffectNo, INT inSENo);
    virtual BOOL startCardBattle();
    virtual void bossDeadDemoCallback();
    virtual void setBossDeadEffect();
    virtual void killBossDeadEffect();
    virtual void bossCardRevolverDestroy();
    virtual void bossDeadDemoDispOff();
    virtual BOOL bossDeadDemoSpecial();
    virtual void bossDeadDemoEnd();
    virtual void onBio(INT inParam);
    virtual void onGift(INT inParam);

protected:
    virtual void updateBefore();
    virtual void updateAfter();
    virtual void updatePositionAfter();
    virtual void lastUpdate();
    virtual void beginStateAppear();
    virtual void stateAppear();
    virtual void endStateAppear();
    virtual void beginStateMain();
    virtual void stateMain();
    virtual void endStateMain();
    virtual void beginStateAttack1();
    virtual void stateAttack1();
    virtual void endStateAttack1();
    virtual void beginStateAttack2();
    virtual void stateAttack2();
    virtual void endStateAttack2();
    virtual void beginStateAttack3();
    virtual void stateAttack3();
    virtual void endStateAttack3();
    virtual void beginStateAttack4();
    virtual void stateAttack4();
    virtual void endStateAttack4();
    virtual void beginStateAttack5();
    virtual void stateAttack5();
    virtual void endStateAttack5();
    virtual void beginStateAttack6();
    virtual void stateAttack6();
    virtual void endStateAttack6();
    virtual void beginStateAttack7();
    virtual void stateAttack7();
    virtual void endStateAttack7();
    virtual void beginStateAttack8();
    virtual void stateAttack8();
    virtual void endStateAttack8();
    virtual void beginStateItem();
    virtual void stateItem();
    virtual void endStateItem();
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
    virtual void aiCardBattle();
    virtual void onGuard();
    virtual void onDamage();
    virtual void onAbsorb();
    virtual void onInvalid();
    virtual void onGimmickCard();
    virtual void onCardBreak();
    virtual void onReflect();
    virtual void onAttackCancel();
    virtual void onCardBattleFailed();
    virtual INT getDuelAttackState();
    virtual CEntity* createFriendCard(CARD_KIND inKind, INT inParam);
    virtual void notifyHP();
    virtual void commonBeginAttack();
    virtual void commonEndAttack();

public: // PARTIAL: state wrappers added for Game/Enemy/BattleEnemy
    BOOL isValidSynchro();
    BOOL canKill();
    void homingToPlayerY(FLOAT inRate);
    void homingMoveToPlayer(FLOAT inRate, FLOAT inSpeed);
    void kill();
    void notifyDead();
    void enableConfuse_(BOOL inEnable);
    void updateDamageTimer_();
    void updateBindTimer_();
    void updateConfuseTimer_();
    void enableBind_(BOOL inEnable);
    void homingToPlayer(FLOAT inRate);
    INT getSize() const;
    FLOAT getDamageRate_() const;
    void changeMainState(INT inState);
    BOOL isPlayingCardBattle();
    BOOL addEnemyKillCount_();
    BOOL canCreateEnemyCard_();
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
public:
    void setAttackState_(INT inState) { m_iAttackState = inState; } // PARTIAL: added for Game/Enemy/Heartless (no range check)
public:
    // PARTIAL: added for Game/Enemy/EnemyAirpirate (last in the class: it sets the original line number)
#line 673 BATTLEENEMY_H_PATH
    void setAttackState(INT inState) {
        if (!(2 <= inState && inState < 10)) AssertApp(__FILE__, __LINE__, "BTL_ENEMY_STATE_ATTACK1 <= inState && inState < BTL_ENEMY_STATE_ATTACK_MAX", 0);
        m_iAttackState = inState;
    }
};

#endif /* GAME_ENEMY_BATTLEENEMY_H */
