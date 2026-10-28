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
#include "Game/Enemy/BattleEnemyManager.h"
#include "Game/Card.h"
#include "Game/SceneInfo.h"
#include "Game/Game.h"
#include "Game/SceneEnv.h"
#include "Game/EffectMotion.h"
#include "include_asm.h"

extern "C" char D_004AD680[];
extern "C" char D_004AD3E0[];
extern "C" char D_004ACBC0[];
extern "C" char D_004AD3C0[];
extern "C" char D_004AD3C8[];
extern "C" char D_004AD3B8[];
extern "C" char D_004AD388[];
extern "C" char D_004AD578[];
/* PARTIAL: until SOURCE DATA, the unit's copy of the header static BattleEnemy::BadStatusEffectName (its .data
 * comes from the original assembly) */
template <class T> static inline T& _ref(T& x) { return x; }
namespace BattleEnemy {
extern CHAR* FreezeEffectName;
extern CHAR* DuelMotionParamName;
extern CHAR* DisionEffectName;
}
extern "C" char D_004AD928[];
extern "C" char D_004AD2A0[];
extern "C" char D_004AD568[];
extern "C" char D_004AD3D8[];
void ScaleVectorXYZ(FVECTOR& v0, const FVECTOR& v1, FLOAT t);
void AddVectorXYZ(FVECTOR& v0, const FVECTOR& v1, const FVECTOR& v2);
extern CHAR* g_BadStatusEffectName asm("_11BattleEnemy$BadStatusEffectName_003A85F0");
FVECTOR getPlayerPos(INT inIndex);
namespace BattleEnemy {
FLOAT GameModeAp(FLOAT inAp);
INT GameModeHp(INT inHp);
extern FLOAT GameModeParamAp[];
}
namespace Gauge {
class CCommonGauge { /* PARTIAL: Game/Player/PlayerGauge.h */
public:
    static void setDispSeq(HANDLE inOwner, INT inSeq);
};
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
FLOAT CBattleEnemy::getExtraParam(const CHAR* inParamName) {
    if (m_pEnemyParam) return m_pEnemyParam->getExtraParam((char*)inParamName);
    return 0.0f;
}
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Game/Enemy/BattleEnemy", getAttackPoint__12CBattleEnemyi8ATK_KINDUsP8ATK_KINDi);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Game/Enemy/BattleEnemy", getAttackAttr__C12CBattleEnemyi);
INT CBattleEnemy::getAttackRate() const {
    if (!m_pEnemyParam) return 10000;
    INT power = 1;
    if (m_tqConfuseTimer > 0) power = 10;
    if ((UINT)getID() % 10000 < 100) {
        CBattleEnemyManager* bem = (CBattleEnemyManager*)CTaskMan::getSysTask(0x19);
        if (bem) {
            power *= bem->getEnemyCount();
            if (power <= 0) power = 1;
        }
    }
    return m_pEnemyParam->iAttackRate[m_iActiveAttackRate] * power;
}
void CBattleEnemy::getUseCardPos(FVECTOR& outPos) {
    FLOAT offset = getExtraParam(BattleEnemy::CamTrgOfsParamName);
    outPos = Crd(0).m_World.getTrans();
    outPos.y += offset;
}
BOOL CBattleEnemy::isPlayingCardBattle() {
    if (CTaskMan::getSysTask(0x16) && CardBattle::CCardBattle::getPlayingHandle(2) == getHandle() &&
        CardBattle::CCardBattle::isPlayingEnemyCard()) {
        return TRUE;
    }
    return FALSE;
}
BOOL CBattleEnemy::isCurrentState(INT inState) {
    if (!((UINT)inState < 31)) AssertApp(D_004ACBC0, 0x4AF, D_004AD388, 0);
    CState& State = *(CState*)((inState * 0x20 + (INT)this) + 0x450);
    return isState(State);
}
BOOL CBattleEnemy::isValidSynchro() {
    return (INT)getExtraParam(BattleEnemy::SynchroParamName) != 0;
}
BOOL CBattleEnemy::canCardBattle() {
    if (CardBattle::CCardBattle::isPlayingEnemyCard()) return FALSE;
    CBattleEnemyManager* bem = (CBattleEnemyManager*)CTaskMan::getSysTask(0x19);
    if (!bem) AssertApp(D_004ACBC0, 0x4C8, D_004AD3B8, 0);
    return bem->canCardBattle(getHandle()) != FALSE;
}
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
void CBattleEnemy::playerHomingY(FLOAT inRate) {
    FVECTOR target;
    FVECTOR vel;
    if (!g_pPlayer) return;
    getPlayerPosition(target);
    ScaleVectorXYZ(vel, g_pPlayer->getVel(), 12.5f);
    vel.y = 0.0f;
    AddVectorXYZ(target, target, vel);
    m_pMove->homingY(target, inRate);
}
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
void CBattleEnemy::bossDeadDemoDispOff() {
    m_pEffect->kill(0.0f);
    m_pSE->stop();
    setEntityFlag(0, 0x80000000);
    setModelAlphaScale(0.0f, -1);
    bodyCollOffBody(-1);
    loadModelColor_();
    killLoopEffect(0);
    killBadStatusEffect();
}
INT CBattleEnemy::getPlayerState() {
    if (g_pPlayer) return g_pPlayer->getPlayerState();
    return 2;
}
void CBattleEnemy::setBossKilledFlag() {
    CGameWork::setFlag(1, 2, 1);
}
void CBattleEnemy::updateBefore() {
    enemyCardDash_();
    if (m_pAttack) {
        if (m_pAttack->test()) {
            m_ullBtlEnemyFlag |= 0x40;
        } else {
            m_ullBtlEnemyFlag &= ~0x40;
        }
    }
    hitTest_();
    if (isBtlEnemyFlag(0x20000)) m_pMove->setOldPos2(getPos());
}
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Game/Enemy/BattleEnemy", updateAfter__12CBattleEnemy);
void CBattleEnemy::updatePositionAfter() {}
void CBattleEnemy::lastUpdate() {
    if (m_pBodyColl) m_pBodyColl->update();
    if (m_pAttack) m_pAttack->update();
    if (isBtlEnemyFlag(0x20000)) {
        setVel(m_vBackupVel);
        m_pMove->updateVelocity();
        if (!isBtlEnemyFlag(0x100)) m_pMove->setCollisionFlag(1, TRUE);
    }
    updateBadStatusEffect(m_hBadStatus);
}
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
void CBattleEnemy::beginStateSmallDamage() {
    if (m_pEnemyParam) {
        if (!isBtlEnemyFlag(0x100) && !(m_pMove->getCollisionFlag() & 5)) m_ullBtlEnemyFlag |= 0x10000000;
        changeDamageMotion(selectDamageMotion_(), 0, 0, -1);
    } else {
        setTimer(3000);
    }
    setupHitBack();
    playSmallDamageSE();
    if (isQuickRecover_()) {
        m_iSysStep = 1;
    } else {
        m_iSysStep = 0;
    }
}
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Game/Enemy/BattleEnemy", stateSmallDamage__12CBattleEnemy);
void CBattleEnemy::endStateSmallDamage() {
    m_ullBtlEnemyFlag &= ~0x10000000;
}
void CBattleEnemy::beginStateBigDamage() {
    if (!isBtlEnemyFlag(0x200000000ULL)) {
        setupFlick();
        m_ullBackupFlag = m_pMove->getFlag();
        m_pMove->enableGravity(TRUE);
        m_iSysStep = 0;
    } else {
        if (checkHitPos() & 1) {
            changeDamageMotion(m_pEnemyParam->iDamageMotionIndex[6], 0, 0, -1);
        } else {
            changeDamageMotion(m_pEnemyParam->iDamageMotionIndex[8], 0, 0, -1);
        }
        m_iSysStep = 1;
    }
    playBigDamageSE();
}
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
void CBattleEnemy::endStateDuel() {
    for (INT i = 0; i < 2; i++) {
        CEffect* effect = CEffectMan::getEffect(m_hSpotLight[i]);
        if (effect) {
            effect->detachCoord();
            effect->loopEnd();
        }
        m_hSpotLight[i] = 0;
    }
    bodyCollOnBody(-1);
}
void CBattleEnemy::beginStateDuelSuccess() {
    INT no = (INT)getExtraParam(BattleEnemy::DuelMotionParamName);
    if (no <= 0) {
        changeMotion(1, 0, 0, -1, 0, 0);
    } else {
        changeMotion(no, 0, 0, -1, 0, 0);
    }
    m_iSysStep = 0;
    setTimer(15000);
}
void CBattleEnemy::stateDuelSuccess() {
    if (!isBtlEnemyFlag(0x4000000000ULL)) m_pMove->turnToTargetY(g_pPlayer, 0.17453292f);
    if (getTimer() <= 0) changeDefaultState();
}
void CBattleEnemy::endStateDuelSuccess() {}
void CBattleEnemy::beginStateFaint() {
    if (m_pEnemyParam) {
        changeMotion(m_pEnemyParam->iFaintMotionIndex[0], 0, 0, -1, 0, 0);
    } else {
        setTimer(15000);
    }
    m_iSysStep = 0;
    setBadStatusEffect(1, 0.0f);
    m_ullBtlEnemyFlag |= 0x80000000000ULL;
}
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
void CBattleEnemy::stateFreeze() {
    if (getTimer() > 0) return;
    CEffect* effect = CEffectMan::getEffect(m_hFreeze);
    if (effect) effect->loopEnd();
    m_hFreeze = 0;
    FVECTOR pos;
    getLockOnPos(pos);
    setEffect(BattleEnemy::FreezeEffectName, 4, &pos, NULL, 0.0f);
    changeDefaultState();
}
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Game/Enemy/BattleEnemy", endStateFreeze__12CBattleEnemy);
void CBattleEnemy::beginStateShock() {
    if (!isBtlEnemyFlag(0x200000000ULL)) {
        setupFlick();
        m_ullBackupFlag = m_pMove->getFlag();
        m_pMove->enableGravity(TRUE);
        m_iSysStep = 0;
    } else {
        changeDamageMotion(m_pEnemyParam->iDamageMotionIndex[0], 0, 0, -1);
        m_iSysStep = 1;
    }
}
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Game/Enemy/BattleEnemy", stateShock__12CBattleEnemy);
void CBattleEnemy::endStateShock() {
    if (!(m_ullBackupFlag & 4)) m_pMove->enableGravity(FALSE);
}
void CBattleEnemy::beginStateTornado() {
    if (!m_pEnemyParam) AssertApp(D_004ACBC0, 0xC5B, D_004AD2A0, 0);
    changeDamageMotion(m_pEnemyParam->iDamageMotionIndex[6], 0, 0, -1);
    m_pMove->enableGravity(FALSE);
    m_iSysStep = 0;
    bodyCollOff(-1);
    bodyCollOffBody(-1);
}
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Game/Enemy/BattleEnemy", stateTornado__12CBattleEnemy);
void CBattleEnemy::endStateTornado() {
    bodyCollOn(-1);
    bodyCollOnBody(-1);
    m_pAnim->setSpeed(1.0f);
    if (isBtlEnemyFlag(0x100)) {
        m_pMove->enableGravity(FALSE);
    } else {
        m_pMove->enableGravity(TRUE);
    }
}
void CBattleEnemy::beginStateDegenerator() {
    FVECTOR pos;
    bodyCollOff(-1);
    bodyCollOffBody(-1);
    getLockOnPos(pos);
    if (!m_pBodyColl->getHitCollBody()) AssertApp(D_004ACBC0, 0xCC3, D_004AD3D8, 0);
    m_hDision = setEffect(BattleEnemy::DisionEffectName, 0, &pos, NULL, 0.0f);
    m_iHp = 0;
    m_pMove->enableGravity(FALSE);
    m_iSysStep = 0;
}
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
void CBattleEnemy::endStateGravity() {
    bodyCollOn(-1);
    bodyCollOnBody(-1);
    if (isBtlEnemyFlag(0x100)) m_pMove->enableGravity(FALSE);
    m_pMove->setMass(1.0f);
}
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Game/Enemy/BattleEnemy", beginStateToss__12CBattleEnemy);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Game/Enemy/BattleEnemy", stateToss__12CBattleEnemy);
void CBattleEnemy::endStateToss() {
    if (m_ullBtlEnemyFlag & 0x100) m_pMove->enableGravity(FALSE);
    bodyCollOnBody(-1);
}
void CBattleEnemy::beginStateBeat() {
    FVECTOR vel;
    if (checkHitPos() & 1) {
        changeDamageMotion(m_pEnemyParam->iDamageMotionIndex[6], 0, 0, -1);
    } else {
        changeDamageMotion(m_pEnemyParam->iDamageMotionIndex[8], 0, 0, -1);
    }
    vel.x = 0.0f;
    vel.y = -0.2f;
    vel.z = 0.0f;
    vel.w = 1.0f;
    m_pMove->setVel(vel);
    m_pMove->enableGravity(TRUE);
    playBigDamageSE();
    m_iSysStep = 0;
}
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
void CBattleEnemy::stateSuspend() {
    CTask* cb = CTaskMan::getSysTask(0x16);
    if (!cb) AssertApp(D_004ACBC0, 0xE6B, D_004AD3C0, 0);
    if (!CardBattle::CCardBattle::isSuspend()) changeDefaultState();
}
void CBattleEnemy::endStateSuspend() {
    m_pAnim->setSpeed(1.0f);
    if (!(m_ullBackupFlag & 0x80000)) clearBtlEnemyFlag(0x80000);
    if (!(m_ullBackupFlag & 0x4000)) clearBtlEnemyFlag(0x4000);
}
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Game/Enemy/BattleEnemy", beginStateVolley__12CBattleEnemy);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Game/Enemy/BattleEnemy", stateVolley__12CBattleEnemy);
void CBattleEnemy::endStateVolley() {
    if (m_ullBtlEnemyFlag & 0x100) m_pMove->enableGravity(FALSE);
    bodyCollOnBody(-1);
}
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Game/Enemy/BattleEnemy", beginStateDead__12CBattleEnemy);
void CBattleEnemy::stateDead() {
    if (m_pEnemyParam && m_iSysStep == 0 && m_pMove->getVel().y < 0.0f && (m_pMove->getCollisionFlag() & 5)) {
        clearSpeed();
        m_iSysStep++;
    }
    if (getTimer() <= 0) out();
}
void CBattleEnemy::endStateDead() {}
CBattleEnemyManager* CBattleEnemy::getBattleEnemyManager() const {
    CTask* parent = (CTask*)getParent();
    while (parent) {
        if (parent->getTypeID() == 0x1003) return (CBattleEnemyManager*)parent;
        parent = parent->getParent();
    }
    return NULL;
}
void CBattleEnemy::makePrize(const FVECTOR* inPos) {
    if (!inPos) AssertApp(D_004ACBC0, 0xF4B, D_004AD568, 0);
    if (isBtlEnemyFlag(0x400000)) return;
    makeExpPrize(inPos);
    makePremiumPrize(inPos);
    makeRoulettePrize(inPos);
    makeCard(inPos);
}
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Game/Enemy/BattleEnemy", makeExpPrize__12CBattleEnemyPC7FVECTOR);
void CBattleEnemy::makePremiumPrize(const FVECTOR* inPos) {
    CBattleEnemyManager* bem = (CBattleEnemyManager*)CTaskMan::getSysTask(0x19);
    if (!bem) AssertApp(D_004ACBC0, 0xF9A, D_004AD3B8, 0);
    bem->createPremiumPrize(*inPos, (UINT)getID() % 10000);
}
void CBattleEnemy::makeRoulettePrize(const FVECTOR* inPos) {
    CBattleEnemyManager* bem = (CBattleEnemyManager*)CTaskMan::getSysTask(0x19);
    if (!bem) AssertApp(D_004ACBC0, 0xFA4, D_004AD3B8, 0);
    bem->createRoulettePrize(*inPos);
}
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
void CBattleEnemy::onBio(INT inSubHp) {
    if (m_iHp > 0) {
        if (CSceneEnv::getStopTypeBit() & (1 << getEntityType())) return;
        if (getFlag() & 0x80000000) return;
        if (isCurrentState(18)) return;
        m_iHp -= inSubHp;
        if (m_iHp <= 0) m_iHp = 1;
        notifyHP();
    }
}
void CBattleEnemy::onGift(INT inAddHp) {
    if (m_iHp > 0) {
        FVECTOR pos;
        getLockOnPos(pos);
        recover(inAddHp, D_004AD578, 0, pos, NULL);
    }
}
void CBattleEnemy::setBadStatusEffect(INT inNo, FLOAT inWait) {
    CEffectMan::killEffect(m_hBadStatus, 0.0f);
    m_hBadStatus = 0;
    m_hBadStatus = CEntity::setBadStatusEffect(_ref(g_BadStatusEffectName), inNo, inWait);
}
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Game/Enemy/BattleEnemy", killBadStatusEffect__12CBattleEnemy);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Game/Enemy/BattleEnemy", setupFlick__12CBattleEnemy);
void CBattleEnemy::commonBeginAttack() {
    bodyCollOff(-1);
}
void CBattleEnemy::commonEndAttack() {
    m_ullBtlEnemyFlag &= ~5;
    bodyCollOn(-1);
}
void CBattleEnemy::notifyDead() {
    if (isBtlEnemyFlag(0x40000000)) return;
    CBattleEnemyManager* bem = (CBattleEnemyManager*)CTaskMan::getSysTask(0x19);
    if (!bem) AssertApp(D_004ACBC0, 0x116A, D_004AD3B8, 0);
    bem->notifyDead(getHandle());
    m_ullBtlEnemyFlag |= 0x40000000;
}
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Game/Enemy/BattleEnemy", setupHitBack__12CBattleEnemy);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Game/Enemy/BattleEnemy", initEnemyParam__12CBattleEnemyiii);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Game/Enemy/BattleEnemy", playSmallDamageSE__12CBattleEnemy);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Game/Enemy/BattleEnemy", playBigDamageSE__12CBattleEnemy);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Game/Enemy/BattleEnemy", playDamageSE__12CBattleEnemy8ATK_KIND);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Game/Enemy/BattleEnemy", setupDuelCard__12CBattleEnemy);
BOOL CBattleEnemy::destroyCard() {
    CardBattle::CCardBattle* cb = (CardBattle::CCardBattle*)CTaskMan::getSysTask(0x16);
    if (!cb) AssertApp(D_004ACBC0, 0x12F9, D_004AD3C0, 0);
    if (CardBattle::CCardBattle::getPlayingHandle(2) == getHandle()) {
        cb->destroyEnemyCard(getHandle());
        return TRUE;
    }
    return FALSE;
}
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
void CBattleEnemy::changeBattleEnemyState(INT inState) {
    if (!((UINT)inState < 31)) AssertApp(D_004ACBC0, 0x162B, D_004AD388, 0);
    CState& NewState = *(CState*)((inState * 0x20 + (INT)this) + 0x450);
    clearBtlEnemyFlag(0x10);
    m_pNewState = &NewState;
}
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
struct CB_ENCHANT_VIEW {
    BYTE _pad[0x17C];
    CARD m_cardEnchant[2];
};
static inline INT _getEnchantKind(CardBattle::CCardBattle* p, INT inUser) {
    return ((CB_ENCHANT_VIEW*)p)->m_cardEnchant[inUser - 1].m_nKind < 0x44 ? 0 : ((CB_ENCHANT_VIEW*)p)->m_cardEnchant[inUser - 1].m_nKind - 0x43;
}
BOOL CBattleEnemy::isLastLeave_(INT inResultHp) {
    CardBattle::CCardBattle* cb = (CardBattle::CCardBattle*)CTaskMan::getSysTask(0x16);
    if (cb && _getEnchantKind(cb, 2) == 0x29 && m_iHp >= 2 && inResultHp <= 0) {
        m_iHp = 1;
        notifyHP();
        CardBattle::CCardBattle::decEnchantTime(2, 0x28, 0x29);
        return TRUE;
    }
    return FALSE;
}
BOOL CBattleEnemy::isReraise_() {
    CardBattle::CCardBattle* cb = (CardBattle::CCardBattle*)CTaskMan::getSysTask(0x16);
    if (cb && _getEnchantKind(cb, 2) == 0x2E) {
        m_iHp = getHpMax() / 4;
        notifyHP();
        Gauge::CCommonGauge::setDispSeq(getHandle(), 1);
        CardBattle::CCardBattle::decEnchantTime(2, 0x28, 0x2E);
        return TRUE;
    }
    return FALSE;
}
/* CCardBattle's m_cardEnchant as the original's CARD[2] (Game/CardBattle.h declares bytes), and its
 * getEnchantKind(INT inUser) inline (1-based user), as in Game/CardRevolver */
BOOL CBattleEnemy::isQuickRecover_() {
    CardBattle::CCardBattle* cb = (CardBattle::CCardBattle*)CTaskMan::getSysTask(0x16);
    if (cb && _getEnchantKind(cb, 2) == 0x2C) {
        setBattleEnemyFlag(0x10, 1);
        return TRUE;
    }
    return FALSE;
}
void CBattleEnemy::enemyCardDash_() {
    CardBattle::CCardBattle* cb = (CardBattle::CCardBattle*)CTaskMan::getSysTask(0x16);
    if (cb && _getEnchantKind(cb, 2) == 0x2D) {
        m_pMove->setPower(1.7f);
    } else {
        m_pMove->setPower(1.0f);
    }
}
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
BOOL CBattleEnemy::isNowPlayDamageMotion_() {
    if (!m_pAnim) AssertApp(D_004ACBC0, 0x1D28, D_004AD928, 0);
    return m_pEnemyParam->isDamageMotion(m_pAnim->getAnimID());
}
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
void CBattleEnemy::continuousDamageProc_() {
    if ((UINT)getID() % 10000 - 100 < 100) {
        if (++m_iHitCount >= 6) {
            m_fDamageRate -= 0.25f;
            if (m_fDamageRate < 0.05f) m_fDamageRate = 0.05f;
        }
        m_tqDamageTimer = 1500;
    }
}
BOOL CBattleEnemy::canBlackroomCard_() {
    if (CCard::isMapCardFull()) return FALSE;
    RoomInfo* room = CSceneInfo::getRoomInfo(-1);
    if (!room) return FALSE;
    if (room->getCardID() != 0xA9) return FALSE;
    UINT id = (UINT)getID() % 10000;
    if (id != 17 && id != 35) return FALSE;
    if (CGame::isSoraGame() && CSceneInfo::getCurrentWorldID() == 1) return FALSE;
    return TRUE;
}
BOOL CBattleEnemy::checkResist_(ATK_KIND inDmgKind) {
    BOOL resist = FALSE;
    switch (inDmgKind) {
    case 7:
        resist = !m_pEnemyParam->getSpecialEffect(0);
        break;
    case 10:
        resist = !m_pEnemyParam->getSpecialEffect(4);
        break;
    }
    if (resist) Gauge::CCommonGauge::setDispSeq(getHandle(), 6);
    return resist;
}
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
