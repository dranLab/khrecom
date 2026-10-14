// PARTIAL: added for Game/Debug/KisEnemy
#ifndef GAME_ENEMY_BATTLEENEMY_H
#define GAME_ENEMY_BATTLEENEMY_H

#include "khtypes.h"
#include "System/MainApp.h"
#include "Game/Entity.h"

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
    UBYTE _m_0x218[0x618]; /* 0x218 m_iAttackState .. 0x450 m_EnemyState[31] */

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
    // PARTIAL: added for Game/Player/FriendBeast (original names of the flag tests unknown)
    ULONGLONG isBtlEnemyFlag(ULONGLONG inFlag) const { return m_ullBtlEnemyFlag & inFlag; }
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
};

#endif /* GAME_ENEMY_BATTLEENEMY_H */
