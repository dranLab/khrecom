// PARTIAL: added for Game/Debug/KisEnemy
#ifndef GAME_ENEMY_BATTLEENEMY_H
#define GAME_ENEMY_BATTLEENEMY_H

#include "khtypes.h"
#include "System/MainApp.h"
#include "Game/Entity.h"
#include "Game/CollParam.h"     // PARTIAL: added for Game/Enemy/EnemyBluerhapsody (ATK_KIND)
#include "Game/AttackMotion.h" // PARTIAL: added for Game/Enemy/EnemyBluerhapsody (ATK_ATTR)

struct ENEMY_PARAM; // PARTIAL: added for Game/Enemy/EnemyAirpirate
struct ENEMY_ACT;   // PARTIAL: added for Game/Enemy/Heartless

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

class CBattleEnemy : public CEntity {
private:
    UBYTE _m_0x1d0[0x30]; /* 0x1d0 m_vHitBackVel .. 0x1f0 m_vBackupVel */
    ULONGLONG m_ullBtlEnemyFlag; /* 0x200 */ // PARTIAL: split out for Game/Player/FriendBeast
    UBYTE _m_0x208[0xc];   /* 0x208 m_ullBackupFlag, 0x210 m_iSysStep */
    INT m_iHp;             /* 0x214 */
    UBYTE _m_0x218[0x20];  /* 0x218 m_iAttackState .. 0x234 m_iParam */
    INT m_iFallMotionIndex; /* 0x238 */ // PARTIAL: split out for Game/Enemy/EnemyPirate
    INT m_iLandMotionIndex; /* 0x23c */
    UBYTE _m_0x240[0x40];  /* 0x240 .. 0x27c m_fDamage */
protected: // PARTIAL: split out for Game/Enemy/EnemyLargebody
    FLOAT m_fBodyRviseCoefficient; /* 0x280 */
private:
    UBYTE _m_0x284[0x5ac]; /* 0x284 m_iExp .. 0x450 m_EnemyState[31] */

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
    // PARTIAL: added for Game/Enemy/EnemyHeartsoldier
    void changeDefaultState();
    void playSmallDamageSE();
    void playBigDamageSE();
    INT selectDamageMotion_();
    INT getParam() { return *(INT*)&_m_0x218[0x1c]; } /* m_iParam 0x234 */
    void makeExpPrize();              // PARTIAL: added for Game/Enemy/BossDeadDemo
    static void setBossKilledFlag();
    void setParam(INT inParam) { *(INT*)&_m_0x218[0x1c] = inParam; } // PARTIAL: added for Game/Enemy/CreateEnemy
    // PARTIAL: added for Game/Enemy/BattleEnemyManager
    UINT getBattleEnemyFlag() const { return m_ullBtlEnemyFlag; }
    void setGroupNo(INT inNo) { *(INT*)&_m_0x218[4] = inNo; } /* m_iGroupNo 0x21c */
    INT getGroupNo() const { return *(const INT*)&_m_0x218[4]; }
    HANDLE getNearestChild(FLOAT& outDist, CEntity& inEntity);
    ULONGLONG isPermissionCardBattle() const { return m_ullBtlEnemyFlag & 0x200; }
    INT getHpMax() const;
    // PARTIAL: added for Game/Enemy/EnemyAirpirate
    ENEMY_PARAM* getEnemyParam() { return *(ENEMY_PARAM**)&_m_0x284[0x1c]; } /* m_pEnemyParam 0x2a0 */
    INT getAttackRate() const;
    INT getAttackState() const { return *(const INT*)&_m_0x218[0]; } // PARTIAL: added for Game/Enemy/Heartless
    BOOL canAttack_(const ENEMY_ACT* inAct);
    void destroyCard(); // PARTIAL: added for Game/Enemy/EnemyBarrelspider
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

public:
    void setAttackState_(INT inState) { *(INT*)&_m_0x218[0] = inState; } // PARTIAL: added for Game/Enemy/Heartless (no range check)
public:
    // PARTIAL: added for Game/Enemy/EnemyAirpirate (last in the class: it sets the original line number)
#line 673 BATTLEENEMY_H_PATH
    void setAttackState(INT inState) {
        if (!(2 <= inState && inState < 10)) AssertApp(__FILE__, __LINE__, "BTL_ENEMY_STATE_ATTACK1 <= inState && inState < BTL_ENEMY_STATE_ATTACK_MAX", 0);
        *(INT*)&_m_0x218[0] = inState; /* m_iAttackState 0x218 */
    }
};

#endif /* GAME_ENEMY_BATTLEENEMY_H */
