#define GAME_ENEMY_BATTLEENEMY_CPP
#include "khtypes.h"
#include "Game/Enemy/BattleEnemy.h"
#include "System/Debug.h"
#include "System/Fsm.h"
#include "Game/GameWork.h"
#include "Game/Enemy/EnemyParam.h"
#include "System/TaskMan.h"
#include "System/Graphic/Effect/EffectMan.h"
#include "System/Graphic/Effect/effect.h"
#include "Game/Bullet/BulletManager.h"
#include "Game/Debug/DebugWork.h"
#include "Game/Movement.h"
#include "Game/CardBattle.h"
#include "Game/Player/Player.h"
#include "Game/BodyCollision.h"
#include "System/Graphic/Anim.h"
#include "System/Coord.h"
#include "Game/CardKind.h"
#include "include_asm.h"

extern "C" char D_004AD680[];
extern "C" char D_004AD3E0[];
extern "C" char D_004ACBC0[];
extern "C" char D_004AD3C0[];
extern "C" char D_004AD3C8[];
FVECTOR getPlayerPos(INT inIndex);
namespace BattleEnemy {
FLOAT GameModeAp(FLOAT inAp);
INT GameModeHp(INT inHp);
extern FLOAT GameModeParamAp[];
}
namespace Gauge {
class CEnemyGauge { /* PARTIAL: Game/Player/PlayerGauge.h declares the class only for its own unit */
public:
    static void notifyHP(HANDLE inHandle, INT inHp, INT inHpMax);
};
}

INT BattleEnemy::GameModeHp(INT inHp) {
    return inHp * GameModeParamHp[CGameWork::getByte(0, 0x61)] / 100;
}
FLOAT BattleEnemy::GameModeAp(FLOAT inAp) {
    return inAp * GameModeParamAp[CGameWork::getByte(0, 0x61)];
}
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Game/Enemy/BattleEnemy", __12CBattleEnemyP5CTaskP6CStateT2);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Game/Enemy/BattleEnemy", _$_12CBattleEnemy);
INT CBattleEnemy::getTypeID() const {
    return 0x1004;
}
void CBattleEnemy::main() {
    if (!(DebugWork::g_dw.enemy.uiFlag & 0x20)) CEntity::main();
}
void CBattleEnemy::update() {
    if (DebugWork::g_dw.enemy.uiFlag & 0x400) clearSpeed();
    else CFSM::update();
}
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Game/Enemy/BattleEnemy", initalize__12CBattleEnemyiiUi);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Game/Enemy/BattleEnemy", onMsg__12CBattleEnemyiPv);
void CBattleEnemy::onOut() {
    if (!(m_ullBtlEnemyFlag & 0x40000000)) notifyDead();
}
INT CBattleEnemy::getHpMax() const {
    if (m_pBasicParam) return BattleEnemy::GameModeHp(m_pBasicParam->iHP);
    return 1;
}
INT CBattleEnemy::getSize() const {
    if (m_pEnemyParam) return m_pEnemyParam->uiSize;
    return 1;
}
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Game/Enemy/BattleEnemy", getNearestChild__12CBattleEnemyRfR7CEntity);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Game/Enemy/BattleEnemy", getExtraParam__12CBattleEnemyPCc);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Game/Enemy/BattleEnemy", getAttackPoint__12CBattleEnemyi8ATK_KINDUsP8ATK_KINDi);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Game/Enemy/BattleEnemy", getAttackAttr__C12CBattleEnemyi);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Game/Enemy/BattleEnemy", getAttackRate__C12CBattleEnemy);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Game/Enemy/BattleEnemy", getUseCardPos__12CBattleEnemyR7FVECTOR);
BOOL CBattleEnemy::isPlayingCardBattle() {
    if (CTaskMan::getSysTask(0x16) && CardBattle::CCardBattle::getPlayingHandle(2) == getHandle() &&
        CardBattle::CCardBattle::isPlayingEnemyCard()) {
        return TRUE;
    }
    return FALSE;
}
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Game/Enemy/BattleEnemy", isCurrentState__12CBattleEnemyi);
BOOL CBattleEnemy::isValidSynchro() {
    return (INT)getExtraParam(BattleEnemy::SynchroParamName) != 0;
}
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Game/Enemy/BattleEnemy", canCardBattle__12CBattleEnemy);
BOOL CBattleEnemy::canKill() {
    if (CardBattle::CCardBattle::getPlayingHandle(2) == getHandle() && CardBattle::CCardBattle::isPlayingEnemyCard()) return FALSE;
    return !isCurrentState(15);
}
void CBattleEnemy::changeMainState(INT inState) {
    if (m_ullBtlEnemyFlag & 4) return;
    if (m_pMainState) m_pNewState = &((CStateTemplate<CBattleEnemy>*)m_pMainState)[inState];
}
void CBattleEnemy::changeMotion(INT inIndex, TIMEQ inTq, BOOL inRootMove, TIMEQ inGr, INT inEffectNo, INT inSENo) {
    CEntity::changeMotion(inIndex, inTq, inRootMove, inGr, inEffectNo, inSENo);
    if (m_pAttack && m_pAttack->set(inIndex)) m_pAttack->setAttackParamForBattleEnemy();
}
BOOL CBattleEnemy::changeMotionIndex(INT inIndex, TIMEQ inTq, BOOL inRootMove, TIMEQ inGr, INT inEffectNo, INT inSENo) {
    if (!m_pAnim) return FALSE;
    if (m_pAnim->getAnimID() == inIndex) return FALSE;
    changeMotion(inIndex, inTq, inRootMove, inGr, inEffectNo, inSENo);
    return TRUE;
}
BOOL CBattleEnemy::startCardBattle() {
    m_ullBtlEnemyFlag &= ~0x802;
    return FALSE;
}
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Game/Enemy/BattleEnemy", endCardBattle__12CBattleEnemyP6CStatei);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Game/Enemy/BattleEnemy", checkHitPos__12CBattleEnemy);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Game/Enemy/BattleEnemy", playerHomingY__12CBattleEnemyf);
void CBattleEnemy::homingToPlayer(FLOAT inRate) {
    FVECTOR vPos;
    getPlayerPosition(vPos);
    m_pMove->homing(vPos, inRate);
}
void CBattleEnemy::homingToPlayerY(FLOAT inRate) {
    FVECTOR vPos;
    getPlayerPosition(vPos);
    m_pMove->homingY(vPos, inRate);
}
BOOL CBattleEnemy::turnToPlayer(FLOAT inLimit) {
    FVECTOR vPos;
    getPlayerPosition(vPos);
    return m_pMove->turnToTarget(vPos, inLimit);
}
BOOL CBattleEnemy::turnToPlayerY(FLOAT inLimit) {
    FVECTOR vPos;
    getPlayerPosition(vPos);
    return m_pMove->turnToTargetY(vPos, inLimit);
}
void CBattleEnemy::homingMoveToPlayer(FLOAT inRate, FLOAT inSpeed) {
    FVECTOR vPos;
    getPlayerPosition(vPos);
    m_pMove->homingMove(vPos, inRate, inSpeed);
}
void CBattleEnemy::homingMoveXZToPlayer(FLOAT inRate, FLOAT inSpeed) {
    FVECTOR pos;
    getPlayerPosition(pos);
    m_pMove->homingMoveXZ(pos, inRate, inSpeed);
}
void CBattleEnemy::getPlayerPosition(FVECTOR& outPos) {
    if (m_ullBtlEnemyFlag & 0x40000) outPos = m_vConfusePlayerPos;
    else outPos = getPlayerPos(0);
}
HANDLE CBattleEnemy::getPlayerHandle() {
    if (isBtlEnemyFlag(0x40000)) return 0;
    if (!g_pPlayer) AssertApp(D_004ACBC0, 0x611, D_004AD3C8, 0);
    return g_pPlayer->getHandle();
}
void CBattleEnemy::makeExpPrize() {
    if (m_pBodyColl->getHitCollBody()) {
        makeExpPrize(m_pBodyColl->getHitCollBody()->getDetectPos());
    } else {
        makeExpPrize(&Crd(1).m_World.getTrans());
    }
}
void CBattleEnemy::makePrize() {
    if (m_pBodyColl->getHitCollBody()) makePrize(m_pBodyColl->getHitCollBody()->getDetectPos());
}
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Game/Enemy/BattleEnemy", bossDeadDemoDispOff__12CBattleEnemy);
INT CBattleEnemy::getPlayerState() {
    if (g_pPlayer) return g_pPlayer->getPlayerState();
    return 2;
}
void CBattleEnemy::setBossKilledFlag() {
    CGameWork::setFlag(1, 2, 1);
}
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Game/Enemy/BattleEnemy", updateBefore__12CBattleEnemy);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Game/Enemy/BattleEnemy", updateAfter__12CBattleEnemy);
void CBattleEnemy::updatePositionAfter() {}
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Game/Enemy/BattleEnemy", lastUpdate__12CBattleEnemy);
void CBattleEnemy::changeDefaultState() {
    m_tqCancelTimer = 250;
    if (m_pDefaultState) {
        gotoState(*m_pDefaultState);
        m_ullBtlEnemyFlag |= 0x10;
    }
}
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Game/Enemy/BattleEnemy", beginStateAppear__12CBattleEnemy);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Game/Enemy/BattleEnemy", stateAppear__12CBattleEnemy);
void CBattleEnemy::endStateAppear() {}
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Game/Enemy/BattleEnemy", beginStateSmallDamage__12CBattleEnemy);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Game/Enemy/BattleEnemy", stateSmallDamage__12CBattleEnemy);
void CBattleEnemy::endStateSmallDamage() {
    m_ullBtlEnemyFlag &= ~0x10000000;
}
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Game/Enemy/BattleEnemy", beginStateBigDamage__12CBattleEnemy);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Game/Enemy/BattleEnemy", stateBigDamage__12CBattleEnemy);
void CBattleEnemy::endStateBigDamage() {
    if (!(m_ullBackupFlag & 4)) m_pMove->enableGravity(FALSE);
}
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Game/Enemy/BattleEnemy", beginStateCardBreak__12CBattleEnemy);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Game/Enemy/BattleEnemy", stateCardBreak__12CBattleEnemy);
void CBattleEnemy::endStateCardBreak() {
    m_ullBtlEnemyFlag &= ~0x10000000;
}
void CBattleEnemy::beginStateZero() {
    CBulletManager* p = (CBulletManager*)CTaskMan::getSysTask(0x1B);
    if (p) p->kill(getHandle());
}
void CBattleEnemy::stateZero() {
    if (isBtlEnemyFlag(1)) TRACE(D_004AD3E0);
    changeDefaultState();
}
void CBattleEnemy::endStateZero() {}
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Game/Enemy/BattleEnemy", beginStateDuel__12CBattleEnemy);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Game/Enemy/BattleEnemy", stateDuel__12CBattleEnemy);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Game/Enemy/BattleEnemy", endStateDuel__12CBattleEnemy);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Game/Enemy/BattleEnemy", beginStateDuelSuccess__12CBattleEnemy);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Game/Enemy/BattleEnemy", stateDuelSuccess__12CBattleEnemy);
void CBattleEnemy::endStateDuelSuccess() {}
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Game/Enemy/BattleEnemy", beginStateFaint__12CBattleEnemy);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Game/Enemy/BattleEnemy", stateFaint__12CBattleEnemy);
void CBattleEnemy::endStateFaint() {
    m_ullBtlEnemyFlag &= ~0x80000000000ULL;
    killBadStatusEffect();
}
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Game/Enemy/BattleEnemy", beginStateStop__12CBattleEnemy);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Game/Enemy/BattleEnemy", stateStop__12CBattleEnemy);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Game/Enemy/BattleEnemy", endStateStop__12CBattleEnemy);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Game/Enemy/BattleEnemy", beginStateTerror__12CBattleEnemy);
void CBattleEnemy::stateTerror() {
    if (getTimer() <= 0) changeDefaultState();
}
void CBattleEnemy::endStateTerror() {
    endTargetEvade();
    m_ullBtlEnemyFlag &= ~0x40000000000ULL;
    killBadStatusEffect();
}
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Game/Enemy/BattleEnemy", beginStateFreeze__12CBattleEnemy);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Game/Enemy/BattleEnemy", stateFreeze__12CBattleEnemy);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Game/Enemy/BattleEnemy", endStateFreeze__12CBattleEnemy);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Game/Enemy/BattleEnemy", beginStateShock__12CBattleEnemy);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Game/Enemy/BattleEnemy", stateShock__12CBattleEnemy);
void CBattleEnemy::endStateShock() {
    if (!(m_ullBackupFlag & 4)) m_pMove->enableGravity(FALSE);
}
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Game/Enemy/BattleEnemy", beginStateTornado__12CBattleEnemy);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Game/Enemy/BattleEnemy", stateTornado__12CBattleEnemy);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Game/Enemy/BattleEnemy", endStateTornado__12CBattleEnemy);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Game/Enemy/BattleEnemy", beginStateDegenerator__12CBattleEnemy);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Game/Enemy/BattleEnemy", stateDegenerator__12CBattleEnemy);
void CBattleEnemy::endStateDegenerator() {}
void CBattleEnemy::beginStateGravity() {
    m_iSysStep = 0;
    setTimer(250);
    bodyCollOff(-1);
    bodyCollOffBody(-1);
    m_pMove->enableGravity(TRUE);
    m_pMove->setMass(5.0f);
}
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Game/Enemy/BattleEnemy", stateGravity__12CBattleEnemy);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Game/Enemy/BattleEnemy", endStateGravity__12CBattleEnemy);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Game/Enemy/BattleEnemy", beginStateToss__12CBattleEnemy);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Game/Enemy/BattleEnemy", stateToss__12CBattleEnemy);
void CBattleEnemy::endStateToss() {
    if (m_ullBtlEnemyFlag & 0x100) m_pMove->enableGravity(FALSE);
    bodyCollOnBody(-1);
}
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Game/Enemy/BattleEnemy", beginStateBeat__12CBattleEnemy);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Game/Enemy/BattleEnemy", stateBeat__12CBattleEnemy);
void CBattleEnemy::endStateBeat() {
    if (m_ullBtlEnemyFlag & 0x100) m_pMove->enableGravity(FALSE);
}
void CBattleEnemy::beginStateMagnet() {
    m_pMove->enableGravity(FALSE);
    m_iSysStep = 0;
    setTimer(m_tqTime);
}
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Game/Enemy/BattleEnemy", stateMagnet__12CBattleEnemy);
void CBattleEnemy::endStateMagnet() {
    FVECTOR v;
    v.init();
    m_pMove->setAcc(v);
    if (!isBtlEnemyFlag(0x100)) m_pMove->enableGravity(TRUE);
}
void CBattleEnemy::beginStateSuspend() {
    m_pAnim->setSpeed(0.0f);
    m_ullBackupFlag = m_ullBtlEnemyFlag;
    setBtlEnemyFlag(0x84000);
}
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Game/Enemy/BattleEnemy", stateSuspend__12CBattleEnemy);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Game/Enemy/BattleEnemy", endStateSuspend__12CBattleEnemy);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Game/Enemy/BattleEnemy", beginStateVolley__12CBattleEnemy);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Game/Enemy/BattleEnemy", stateVolley__12CBattleEnemy);
void CBattleEnemy::endStateVolley() {
    if (m_ullBtlEnemyFlag & 0x100) m_pMove->enableGravity(FALSE);
    bodyCollOnBody(-1);
}
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Game/Enemy/BattleEnemy", beginStateDead__12CBattleEnemy);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Game/Enemy/BattleEnemy", stateDead__12CBattleEnemy);
void CBattleEnemy::endStateDead() {}
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Game/Enemy/BattleEnemy", getBattleEnemyManager__C12CBattleEnemy);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Game/Enemy/BattleEnemy", makePrize__12CBattleEnemyPC7FVECTOR);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Game/Enemy/BattleEnemy", makeExpPrize__12CBattleEnemyPC7FVECTOR);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Game/Enemy/BattleEnemy", makePremiumPrize__12CBattleEnemyPC7FVECTOR);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Game/Enemy/BattleEnemy", makeRoulettePrize__12CBattleEnemyPC7FVECTOR);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Game/Enemy/BattleEnemy", makeCard__12CBattleEnemyPC7FVECTOR);
void CBattleEnemy::setBossDeadEffect() {
    m_hDeath = setDeathEffect();
}
void CBattleEnemy::killBossDeadEffect() {
    CEffect* pEffect = CEffectMan::getEffect(m_hDeath);
    if (pEffect) pEffect->kill();
    m_hDeath = 0;
}
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Game/Enemy/BattleEnemy", recover__12CBattleEnemyiPCciRC7FVECTORPC6CCoord);
void CBattleEnemy::kill() {
    if (isState(*(CState*)&m_EnemyState[30 * 0x20])) return;
    m_iHp = 0;
    m_ullBtlEnemyFlag |= 0x200000000000ULL;
    destroyCard();
    changeBattleEnemyState(30);
}
/* Parked: registers only - after destroyCard() the original loads m_ullBtlEnemyFlag into v1 and the ~1 mask into
 * v0 (ours: v0/v1; in our RTL the -2 constant is set before the load, so it gets the other register); the same ld
 * v1/li v0 pattern follows destroyCard() in endCardBattle and hitTest_; tried: clearBtlEnemyFlag(1), &= ~1 / -2 /
 * ~1ULL / (INT)~1, reversed operands, a flag local, a reference local, setBattleEnemyFlag(0, 1) (the weak (on,
 * off) inline), if (TRUE), permuter 6 min (20513 candidates, no gain) */
#if 0
void CBattleEnemy::attackStateCardDestroy(BOOL inColl) {
    destroyCard();
    clearBtlEnemyFlag(1);
    if (inColl) bodyCollOn(-1);
}
#else
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Game/Enemy/BattleEnemy", attackStateCardDestroy__12CBattleEnemyi);
#endif
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Game/Enemy/BattleEnemy", onBio__12CBattleEnemyi);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Game/Enemy/BattleEnemy", onGift__12CBattleEnemyi);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Game/Enemy/BattleEnemy", setBadStatusEffect__12CBattleEnemyif);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Game/Enemy/BattleEnemy", killBadStatusEffect__12CBattleEnemy);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Game/Enemy/BattleEnemy", setupFlick__12CBattleEnemy);
void CBattleEnemy::commonBeginAttack() {
    bodyCollOff(-1);
}
void CBattleEnemy::commonEndAttack() {
    m_ullBtlEnemyFlag &= ~5;
    bodyCollOn(-1);
}
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Game/Enemy/BattleEnemy", notifyDead__12CBattleEnemy);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Game/Enemy/BattleEnemy", setupHitBack__12CBattleEnemy);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Game/Enemy/BattleEnemy", initEnemyParam__12CBattleEnemyiii);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Game/Enemy/BattleEnemy", playSmallDamageSE__12CBattleEnemy);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Game/Enemy/BattleEnemy", playBigDamageSE__12CBattleEnemy);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Game/Enemy/BattleEnemy", playDamageSE__12CBattleEnemy8ATK_KIND);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Game/Enemy/BattleEnemy", setupDuelCard__12CBattleEnemy);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Game/Enemy/BattleEnemy", destroyCard__12CBattleEnemy);
void CBattleEnemy::changeDamageMotion(INT inIndex, TIMEQ inTq, BOOL inRootMove, TIMEQ inGr) {
    INT identifyNo = 0;
    if (m_pBodyColl->getHitCollBody()) {
        identifyNo = m_sAttrDmgSeIdentifyNo[m_pBodyColl->getHitCollBody()->getDamageAttr()];
    }
    changeMotion(inIndex, inTq, inRootMove, inGr, 0, identifyNo);
}
void CBattleEnemy::beginStateAppear_() {
    beginStateAppear();
}
void CBattleEnemy::stateAppear_() {
    stateAppear();
}
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Game/Enemy/BattleEnemy", endStateAppear___12CBattleEnemy);
void CBattleEnemy::beginStateMain_() {
    beginStateMain();
}
void CBattleEnemy::stateMain_() {
    stateMain();
}
void CBattleEnemy::endStateMain_() {
    endStateMain();
}
void CBattleEnemy::beginStateAttack1_() {
    beginStateAttack1();
    commonBeginAttack();
}
void CBattleEnemy::stateAttack1_() {
    if (!(m_ullBtlEnemyFlag & 0x800000000ULL) && !isPlayingCardBattle()) TRACE(D_004AD680, getHandle(), getID());
    stateAttack1();
}
void CBattleEnemy::endStateAttack1_() {
    endStateAttack1();
    commonEndAttack();
}
void CBattleEnemy::beginStateAttack2_() {
    beginStateAttack2();
    commonBeginAttack();
}
void CBattleEnemy::stateAttack2_() {
    if (!(m_ullBtlEnemyFlag & 0x800000000ULL) && !isPlayingCardBattle()) TRACE(D_004AD680, getHandle(), getID());
    stateAttack2();
}
void CBattleEnemy::endStateAttack2_() {
    endStateAttack2();
    commonEndAttack();
}
void CBattleEnemy::beginStateAttack3_() {
    beginStateAttack3();
    commonBeginAttack();
}
void CBattleEnemy::stateAttack3_() {
    if (!(m_ullBtlEnemyFlag & 0x800000000ULL) && !isPlayingCardBattle()) TRACE(D_004AD680, getHandle(), getID());
    stateAttack3();
}
void CBattleEnemy::endStateAttack3_() {
    endStateAttack3();
    commonEndAttack();
}
void CBattleEnemy::beginStateAttack4_() {
    beginStateAttack4();
    commonBeginAttack();
}
void CBattleEnemy::stateAttack4_() {
    if (!(m_ullBtlEnemyFlag & 0x800000000ULL) && !isPlayingCardBattle()) TRACE(D_004AD680, getHandle(), getID());
    stateAttack4();
}
void CBattleEnemy::endStateAttack4_() {
    endStateAttack4();
    commonEndAttack();
}
void CBattleEnemy::beginStateAttack5_() {
    beginStateAttack5();
    commonBeginAttack();
}
void CBattleEnemy::stateAttack5_() {
    if (!(m_ullBtlEnemyFlag & 0x800000000ULL) && !isPlayingCardBattle()) TRACE(D_004AD680, getHandle(), getID());
    stateAttack5();
}
void CBattleEnemy::endStateAttack5_() {
    endStateAttack5();
    commonEndAttack();
}
void CBattleEnemy::beginStateAttack6_() {
    beginStateAttack6();
    commonBeginAttack();
}
void CBattleEnemy::stateAttack6_() {
    if (!(m_ullBtlEnemyFlag & 0x800000000ULL) && !isPlayingCardBattle()) TRACE(D_004AD680, getHandle(), getID());
    stateAttack6();
}
void CBattleEnemy::endStateAttack6_() {
    endStateAttack6();
    commonEndAttack();
}
void CBattleEnemy::beginStateAttack7_() {
    beginStateAttack7();
    commonBeginAttack();
}
void CBattleEnemy::stateAttack7_() {
    if (!(m_ullBtlEnemyFlag & 0x800000000ULL) && !isPlayingCardBattle()) TRACE(D_004AD680, getHandle(), getID());
    stateAttack7();
}
void CBattleEnemy::endStateAttack7_() {
    endStateAttack7();
    commonEndAttack();
}
void CBattleEnemy::beginStateAttack8_() {
    beginStateAttack8();
    commonBeginAttack();
}
void CBattleEnemy::stateAttack8_() {
    if (!(m_ullBtlEnemyFlag & 0x800000000ULL) && !isPlayingCardBattle()) TRACE(D_004AD680, getHandle(), getID());
    stateAttack8();
}
void CBattleEnemy::endStateAttack8_() {
    endStateAttack8();
    commonEndAttack();
}
void CBattleEnemy::beginStateItem_() {
    beginStateItem();
    commonBeginAttack();
}
void CBattleEnemy::stateItem_() {
    stateItem();
}
void CBattleEnemy::endStateItem_() {
    endStateItem();
    commonEndAttack();
}
void CBattleEnemy::beginStateSmallDamage_() {
    beginStateSmallDamage();
}
void CBattleEnemy::stateSmallDamage_() {
    stateSmallDamage();
}
void CBattleEnemy::endStateSmallDamage_() {
    endStateSmallDamage();
    m_ullBtlEnemyFlag &= ~0x1008;
}
void CBattleEnemy::beginStateBigDamage_() {
    beginStateBigDamage();
}
void CBattleEnemy::stateBigDamage_() {
    stateBigDamage();
}
void CBattleEnemy::endStateBigDamage_() {
    endStateBigDamage();
    m_ullBtlEnemyFlag &= ~8;
}
void CBattleEnemy::beginStateCardBreak_() {
    onAttackCancel();
    beginStateCardBreak();
}
void CBattleEnemy::stateCardBreak_() {
    stateCardBreak();
}
void CBattleEnemy::endStateCardBreak_() {
    endStateCardBreak();
}
void CBattleEnemy::beginStateZero_() {
    onAttackCancel();
    beginStateZero();
}
void CBattleEnemy::stateZero_() {
    stateZero();
}
void CBattleEnemy::endStateZero_() {
    endStateZero();
}
void CBattleEnemy::beginStateDuel_() {
    beginStateDuel();
}
void CBattleEnemy::stateDuel_() {
    stateDuel();
}
void CBattleEnemy::endStateDuel_() {
    endStateDuel();
    m_ullBtlEnemyFlag &= ~0x100000000ULL;
}
void CBattleEnemy::beginStateDuelSuccess_() {
    beginStateDuelSuccess();
}
void CBattleEnemy::stateDuelSuccess_() {
    stateDuelSuccess();
}
void CBattleEnemy::endStateDuelSuccess_() {
    endStateDuelSuccess();
}
void CBattleEnemy::beginStateFaint_() {
    beginStateFaint();
}
void CBattleEnemy::stateFaint_() {
    stateFaint();
}
void CBattleEnemy::endStateFaint_() {
    endStateFaint();
}
void CBattleEnemy::beginStateStop_() {
    beginStateStop();
}
void CBattleEnemy::stateStop_() {
    stateStop();
}
void CBattleEnemy::endStateStop_() {
    endStateStop();
}
void CBattleEnemy::beginStateTerror_() {
    beginStateTerror();
}
void CBattleEnemy::stateTerror_() {
    stateTerror();
}
void CBattleEnemy::endStateTerror_() {
    endStateTerror();
}
void CBattleEnemy::beginStateFreeze_() {
    beginStateFreeze();
}
void CBattleEnemy::stateFreeze_() {
    stateFreeze();
}
void CBattleEnemy::endStateFreeze_() {
    endStateFreeze();
}
void CBattleEnemy::beginStateShock_() {
    beginStateShock();
}
void CBattleEnemy::stateShock_() {
    stateShock();
}
void CBattleEnemy::endStateShock_() {
    endStateShock();
}
void CBattleEnemy::beginStateTornado_() {
    beginStateTornado();
}
void CBattleEnemy::stateTornado_() {
    stateTornado();
}
void CBattleEnemy::endStateTornado_() {
    endStateTornado();
}
void CBattleEnemy::beginStateDegenerator_() {
    beginStateDegenerator();
    addEnemyKillCount_();
}
void CBattleEnemy::stateDegenerator_() {
    stateDegenerator();
}
void CBattleEnemy::endStateDegenerator_() {
    endStateDegenerator();
}
void CBattleEnemy::beginStateGravity_() {
    beginStateGravity();
}
void CBattleEnemy::stateGravity_() {
    stateGravity();
}
void CBattleEnemy::endStateGravity_() {
    endStateGravity();
}
void CBattleEnemy::beginStateToss_() {
    beginStateToss();
}
void CBattleEnemy::stateToss_() {
    stateToss();
}
void CBattleEnemy::endStateToss_() {
    endStateToss();
}
void CBattleEnemy::beginStateBeat_() {
    beginStateBeat();
}
void CBattleEnemy::stateBeat_() {
    stateBeat();
}
void CBattleEnemy::endStateBeat_() {
    endStateBeat();
}
void CBattleEnemy::beginStateMagnet_() {
    beginStateMagnet();
}
void CBattleEnemy::stateMagnet_() {
    stateMagnet();
}
void CBattleEnemy::endStateMagnet_() {
    endStateMagnet();
}
void CBattleEnemy::beginStateSuspend_() {
    beginStateSuspend();
}
void CBattleEnemy::stateSuspend_() {
    stateSuspend();
}
void CBattleEnemy::endStateSuspend_() {
    endStateSuspend();
}
void CBattleEnemy::beginStateVolley_() {
    beginStateVolley();
}
void CBattleEnemy::stateVolley_() {
    stateVolley();
}
void CBattleEnemy::endStateVolley_() {
    endStateVolley();
}
void CBattleEnemy::beginStateDead_() {
    beginStateDead();
    addEnemyKillCount_();
}
void CBattleEnemy::stateDead_() {
    stateDead();
}
void CBattleEnemy::endStateDead_() {
    endStateDead();
}
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Game/Enemy/BattleEnemy", createFriendCard__12CBattleEnemy9CARD_KINDi);
void CBattleEnemy::notifyHP() {
    INT nHp = m_iHp;
    HANDLE hThis = getHandle();
    Gauge::CEnemyGauge::notifyHP(hThis, nHp, getHpMax());
}
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Game/Enemy/BattleEnemy", initFSM___12CBattleEnemy);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Game/Enemy/BattleEnemy", initRoom___12CBattleEnemy);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Game/Enemy/BattleEnemy", changeBattleEnemyState__12CBattleEnemyi);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Game/Enemy/BattleEnemy", hitTest___12CBattleEnemy);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Game/Enemy/BattleEnemy", computeDamage___12CBattleEnemyff8ATK_KIND);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Game/Enemy/BattleEnemy", computeDamage___12CBattleEnemyff);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Game/Enemy/BattleEnemy", changeDamageState___12CBattleEnemy8ATK_KIND);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Game/Enemy/BattleEnemy", canAttack___12CBattleEnemyPC9ENEMY_ACT);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Game/Enemy/BattleEnemy", selectDamageMotion___12CBattleEnemy);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Game/Enemy/BattleEnemy", enableBind___12CBattleEnemyi);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Game/Enemy/BattleEnemy", enableConfuse___12CBattleEnemyi);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Game/Enemy/BattleEnemy", setGuardEffect___12CBattleEnemyPC7FVECTOR);
FLOAT CBattleEnemy::getDamageRate_() const {
    return m_fDamageRate;
}
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Game/Enemy/BattleEnemy", isLastLeave___12CBattleEnemyi);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Game/Enemy/BattleEnemy", isReraise___12CBattleEnemy);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Game/Enemy/BattleEnemy", isQuickRecover___12CBattleEnemy);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Game/Enemy/BattleEnemy", enemyCardDash___12CBattleEnemy);
void CBattleEnemy::createFriendCard_(INT inRate) {
    CARD card;
    CardBattle::getDropFriendCard(card, inRate);
    if (card.m_nKind) createFriendCard((CARD_KIND)card.m_nKind, card.m_nNumber);
}
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Game/Enemy/BattleEnemy", setHitEffect___12CBattleEnemy);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Game/Enemy/BattleEnemy", cardBattleProc___12CBattleEnemy);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Game/Enemy/BattleEnemy", reviseObject___12CBattleEnemy);
void CBattleEnemy::updateBindTimer_() {
    if (m_tqBindTimer > 0) {
        m_tqBindTimer -= getFrameRate();
        if (m_tqBindTimer <= 0) enableBind_(FALSE);
    }
}
void CBattleEnemy::updateConfuseTimer_() {
    if (m_tqConfuseTimer > 0) {
        m_tqConfuseTimer -= getFrameRate();
        if (m_tqConfuseTimer <= 0) enableConfuse_(FALSE);
    }
}
void CBattleEnemy::updateHitBackTimer_() {
    if (m_tqHitBackTimer > 0) {
        m_tqHitBackTimer -= getFrameRate();
        if (m_tqHitBackTimer <= 0) {
            FVECTOR v;
            v.init();
            m_pMove->setHitBackVel(v);
        } else {
            m_pMove->setHitBackVel(m_vHitBackVel);
        }
    }
}
void CBattleEnemy::updateDamageTimer_() {
    if (m_tqDamageTimer > 0) {
        m_tqDamageTimer -= getFrameRate();
        if (m_tqDamageTimer <= 0) {
            m_iHitCount = 0;
            m_fDamageRate = 1.0f;
        }
    }
}
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Game/Enemy/BattleEnemy", isNowPlayDamageMotion___12CBattleEnemy);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Game/Enemy/BattleEnemy", addEnemyKillCount___12CBattleEnemy);
BOOL CBattleEnemy::changeSmallDamageState_(ATK_KIND inDmgKind) {
    if (!isBtlEnemyFlag(0x200000)) {
        changeBattleEnemyState(11);
        return TRUE;
    }
    playDamageSE(inDmgKind);
    return FALSE;
}
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Game/Enemy/BattleEnemy", updateSpotLight___12CBattleEnemy);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Game/Enemy/BattleEnemy", canCreateEnemyCard___12CBattleEnemy);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Game/Enemy/BattleEnemy", continuousDamageProc___12CBattleEnemy);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Game/Enemy/BattleEnemy", canBlackroomCard___12CBattleEnemy);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Game/Enemy/BattleEnemy", checkResist___12CBattleEnemy8ATK_KIND);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Game/Enemy/BattleEnemy", _$_t14CStateTemplate1Z12CBattleEnemy);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Game/Enemy/BattleEnemy", initalize__12CBattleEnemyii);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Game/Enemy/BattleEnemy", setBattleEnemyFlag__12CBattleEnemyUlUl);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Game/Enemy/BattleEnemy", setHp__12CBattleEnemyi);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Game/Enemy/BattleEnemy", setGroupNo__12CBattleEnemyi);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Game/Enemy/BattleEnemy", setDefaultState__12CBattleEnemyP6CState);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Game/Enemy/BattleEnemy", setFallMotionIndex__12CBattleEnemyi);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Game/Enemy/BattleEnemy", setLandMotionIndex__12CBattleEnemyi);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Game/Enemy/BattleEnemy", setParam__12CBattleEnemyi);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Game/Enemy/BattleEnemy", setBodyRviseCoefficient__12CBattleEnemyf);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Game/Enemy/BattleEnemy", setAttrDmgSeIdentifyNo__12CBattleEnemyii);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Game/Enemy/BattleEnemy", setCardBreakSeIdentifyNo__12CBattleEnemyi);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Game/Enemy/BattleEnemy", setExp__12CBattleEnemyi);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Game/Enemy/BattleEnemy", getBattleEnemyFlag__C12CBattleEnemy);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Game/Enemy/BattleEnemy", getHp__C12CBattleEnemy);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Game/Enemy/BattleEnemy", getGroupNo__C12CBattleEnemy);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Game/Enemy/BattleEnemy", getCardKind__C12CBattleEnemy);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Game/Enemy/BattleEnemy", getDuelCardKind__C12CBattleEnemy);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Game/Enemy/BattleEnemy", getParam__C12CBattleEnemy);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Game/Enemy/BattleEnemy", getEnemyParam__C12CBattleEnemy);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Game/Enemy/BattleEnemy", getHpPercent__C12CBattleEnemy);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Game/Enemy/BattleEnemy", getMesCardInfo__C12CBattleEnemy);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Game/Enemy/BattleEnemy", getStopTime__C12CBattleEnemy);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Game/Enemy/BattleEnemy", getExp__C12CBattleEnemy);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Game/Enemy/BattleEnemy", isCardBattle__C12CBattleEnemy);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Game/Enemy/BattleEnemy", isCardBattleFailed__C12CBattleEnemy);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Game/Enemy/BattleEnemy", isAttack__C12CBattleEnemy);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Game/Enemy/BattleEnemy", isStartFaint__C12CBattleEnemy);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Game/Enemy/BattleEnemy", isPermissionCardBattle__C12CBattleEnemy);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Game/Enemy/BattleEnemy", isNoCreateCard__C12CBattleEnemy);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Game/Enemy/BattleEnemy", isHitAttack__C12CBattleEnemy);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Game/Enemy/BattleEnemy", isReload__C12CBattleEnemy);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Game/Enemy/BattleEnemy", isFly__C12CBattleEnemy);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Game/Enemy/BattleEnemy", isPutCardReady__C12CBattleEnemy);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Game/Enemy/BattleEnemy", isNoHpSub__C12CBattleEnemy);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Game/Enemy/BattleEnemy", isStartCardBattle__C12CBattleEnemy);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Game/Enemy/BattleEnemy", isCounter__C12CBattleEnemy);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Game/Enemy/BattleEnemy", isInvincible__C12CBattleEnemy);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Game/Enemy/BattleEnemy", isMatchless__C12CBattleEnemy);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Game/Enemy/BattleEnemy", isNoHitBack__C12CBattleEnemy);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Game/Enemy/BattleEnemy", isNoPlayDamageMotion__C12CBattleEnemy);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Game/Enemy/BattleEnemy", isBind__C12CBattleEnemy);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Game/Enemy/BattleEnemy", isConfuse__C12CBattleEnemy);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Game/Enemy/BattleEnemy", isNoDmgResponse__C12CBattleEnemy);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Game/Enemy/BattleEnemy", isNoBigDamage__C12CBattleEnemy);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Game/Enemy/BattleEnemy", isNoSmallDmgResponse__C12CBattleEnemy);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Game/Enemy/BattleEnemy", isNoMakePrize__C12CBattleEnemy);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Game/Enemy/BattleEnemy", isHitBody__C12CBattleEnemy);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Game/Enemy/BattleEnemy", isHitPoints__C12CBattleEnemy);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Game/Enemy/BattleEnemy", isOnDamageEndCardBattle__C12CBattleEnemy);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Game/Enemy/BattleEnemy", isNoSetHitEffect__C12CBattleEnemy);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Game/Enemy/BattleEnemy", isAlwaysSetCard__C12CBattleEnemy);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Game/Enemy/BattleEnemy", isDamageAir__C12CBattleEnemy);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Game/Enemy/BattleEnemy", isStockAct__C12CBattleEnemy);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Game/Enemy/BattleEnemy", isNotifyDead__C12CBattleEnemy);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Game/Enemy/BattleEnemy", isRotMove__C12CBattleEnemy);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Game/Enemy/BattleEnemy", isStartDuel__C12CBattleEnemy);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Game/Enemy/BattleEnemy", isNoFlick__C12CBattleEnemy);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Game/Enemy/BattleEnemy", isNoPlayDamageSE__C12CBattleEnemy);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Game/Enemy/BattleEnemy", isNoCardBattle__C12CBattleEnemy);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Game/Enemy/BattleEnemy", isStartConfuse__C12CBattleEnemy);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Game/Enemy/BattleEnemy", isNoMakeFriendCard__C12CBattleEnemy);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Game/Enemy/BattleEnemy", isNoDuelPlayerTurn__C12CBattleEnemy);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Game/Enemy/BattleEnemy", isNoResponseTornado__C12CBattleEnemy);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Game/Enemy/BattleEnemy", isNoDmgResponseOnFreeze__C12CBattleEnemy);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Game/Enemy/BattleEnemy", isStartSleep__C12CBattleEnemy);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Game/Enemy/BattleEnemy", isTerror__C12CBattleEnemy);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Game/Enemy/BattleEnemy", isFaint__C12CBattleEnemy);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Game/Enemy/BattleEnemy", isNoDmgResponseOnMagnet__C12CBattleEnemy);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Game/Enemy/BattleEnemy", isNoKillCount__C12CBattleEnemy);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Game/Enemy/BattleEnemy", canFlick__C12CBattleEnemy);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Game/Enemy/BattleEnemy", changeAttackRate__12CBattleEnemyi);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Game/Enemy/BattleEnemy", bossDeadDemoCallback__12CBattleEnemy);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Game/Enemy/BattleEnemy", bossCardRevolverDestroy__12CBattleEnemy);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Game/Enemy/BattleEnemy", bossDeadDemoSpecial__12CBattleEnemy);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Game/Enemy/BattleEnemy", bossDeadDemoEnd__12CBattleEnemy);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Game/Enemy/BattleEnemy", beginStateMain__12CBattleEnemy);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Game/Enemy/BattleEnemy", stateMain__12CBattleEnemy);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Game/Enemy/BattleEnemy", endStateMain__12CBattleEnemy);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Game/Enemy/BattleEnemy", beginStateAttack1__12CBattleEnemy);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Game/Enemy/BattleEnemy", stateAttack1__12CBattleEnemy);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Game/Enemy/BattleEnemy", endStateAttack1__12CBattleEnemy);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Game/Enemy/BattleEnemy", beginStateAttack2__12CBattleEnemy);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Game/Enemy/BattleEnemy", stateAttack2__12CBattleEnemy);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Game/Enemy/BattleEnemy", endStateAttack2__12CBattleEnemy);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Game/Enemy/BattleEnemy", beginStateAttack3__12CBattleEnemy);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Game/Enemy/BattleEnemy", stateAttack3__12CBattleEnemy);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Game/Enemy/BattleEnemy", endStateAttack3__12CBattleEnemy);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Game/Enemy/BattleEnemy", beginStateAttack4__12CBattleEnemy);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Game/Enemy/BattleEnemy", stateAttack4__12CBattleEnemy);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Game/Enemy/BattleEnemy", endStateAttack4__12CBattleEnemy);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Game/Enemy/BattleEnemy", beginStateAttack5__12CBattleEnemy);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Game/Enemy/BattleEnemy", stateAttack5__12CBattleEnemy);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Game/Enemy/BattleEnemy", endStateAttack5__12CBattleEnemy);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Game/Enemy/BattleEnemy", beginStateAttack6__12CBattleEnemy);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Game/Enemy/BattleEnemy", stateAttack6__12CBattleEnemy);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Game/Enemy/BattleEnemy", endStateAttack6__12CBattleEnemy);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Game/Enemy/BattleEnemy", beginStateAttack7__12CBattleEnemy);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Game/Enemy/BattleEnemy", stateAttack7__12CBattleEnemy);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Game/Enemy/BattleEnemy", endStateAttack7__12CBattleEnemy);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Game/Enemy/BattleEnemy", beginStateAttack8__12CBattleEnemy);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Game/Enemy/BattleEnemy", stateAttack8__12CBattleEnemy);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Game/Enemy/BattleEnemy", endStateAttack8__12CBattleEnemy);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Game/Enemy/BattleEnemy", beginStateItem__12CBattleEnemy);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Game/Enemy/BattleEnemy", stateItem__12CBattleEnemy);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Game/Enemy/BattleEnemy", endStateItem__12CBattleEnemy);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Game/Enemy/BattleEnemy", aiCardBattle__12CBattleEnemy);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Game/Enemy/BattleEnemy", onGuard__12CBattleEnemy);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Game/Enemy/BattleEnemy", onDamage__12CBattleEnemy);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Game/Enemy/BattleEnemy", onAbsorb__12CBattleEnemy);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Game/Enemy/BattleEnemy", onInvalid__12CBattleEnemy);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Game/Enemy/BattleEnemy", onGimmickCard__12CBattleEnemy);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Game/Enemy/BattleEnemy", onCardBreak__12CBattleEnemy);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Game/Enemy/BattleEnemy", onReflect__12CBattleEnemy);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Game/Enemy/BattleEnemy", onAttackCancel__12CBattleEnemy);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Game/Enemy/BattleEnemy", onCardBattleFailed__12CBattleEnemy);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Game/Enemy/BattleEnemy", getDuelAttackState__12CBattleEnemy);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Game/Enemy/BattleEnemy", setAttackState__12CBattleEnemyi);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Game/Enemy/BattleEnemy", canAttack__12CBattleEnemyPC9ENEMY_ACT);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Game/Enemy/BattleEnemy", cancelTimeOver___C12CBattleEnemy);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Game/Enemy/BattleEnemy", executeBeginState__t14CStateTemplate1Z12CBattleEnemy);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Game/Enemy/BattleEnemy", executeState__t14CStateTemplate1Z12CBattleEnemy);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Game/Enemy/BattleEnemy", executeEndState__t14CStateTemplate1Z12CBattleEnemy);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Game/Enemy/BattleEnemy", func_002B8070);
typedef char _chk_be[sizeof(CBattleEnemy) == 0x830 ? 1 : -1];
