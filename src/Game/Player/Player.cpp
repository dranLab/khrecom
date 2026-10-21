#define GAME_PLAYER_PLAYER_CPP
#include "khtypes.h"
#include "Game/Collision/CollBody.h"
#include "Game/CardBattle.h"
#include "Game/Player/Player.h"
#include "Game/Player/PlayerManager.h"
#include "System/Debug.h"
#include "Game/Player/PlayerGauge.h"
#include "Game/Player/PlayerWeapon.h"
#include "Game/Debug/DebugWork.h"
#include "Game/Movement.h"
#include "Game/Status.h"
#include "Game/SceneEnv.h"
#include "Game/Bullet/BulletManager.h"
#include "System/TaskMan.h"
#include "Game/SEMotion.h"
#include "include_asm.h"

extern "C" char D_004997C0[];
extern "C" char D_00499828[];

INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Game/Player/Player", DebugPlayerCamera__9DWTPlayerP12CDebugWindowii_0025F688);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Game/Player/Player", DebugPlayerAttack__9DWTPlayerP12CDebugWindowii_0025F798);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Game/Player/Player", DebugWinPlayer__9DWTPlayerP12CDebugWindowii_0025FA48);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Game/Player/Player", getPlayerPos__Fi);
INT CPlayer::calcNextEXP(INT inLv) {
    INT nExp = 25;
    for (INT i = 3; i <= inLv + 1; i++) {
        nExp += i * i * 4;
    }
    return nExp;
}
INT CPlayer::getNowNextEXP() {
    INT next;
    if (m_pInstance) {
        next = calcNextEXP(m_pInstance->getLV());
        __asm__ __volatile__("" : : : "memory"); /* matching lever: the original re-reads m_pInstance after the const call */
        return next - m_pInstance->getEXP();
    } else {
        next = calcNextEXP(CStatus::getLV());
        return next - CStatus::getEXP();
    }
}
INT CPlayer::getNowTotalMP() {
    if (!m_pInstance) return CStatus::getMP();
    return m_pInstance->m_iMP;
}
void CPlayer::enablePadControl(INT inFlag) {
    if (m_pInstance) {
        UINT bit = 1;
        m_pInstance->m_uiPadFlag = (m_pInstance->m_uiPadFlag | inFlag) & ~(inFlag ^ bit);
    }
}
void CPlayer::enableDbgPadControl(INT inFlag) {
    if (m_pInstance) {
        UINT bit = 2;
        inFlag <<= 1;
        m_pInstance->m_uiPadFlag = (m_pInstance->m_uiPadFlag | inFlag) & ~(inFlag ^ bit);
    }
}
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Game/Player/Player", __7CPlayerP5CTaskUii);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Game/Player/Player", _$_7CPlayer);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Game/Player/Player", initFSM___7CPlayer);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Game/Player/Player", _init__7CPlayer);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Game/Player/Player", updateBefore__7CPlayer);
void CPlayer::updateAfter() {}
void CPlayer::lastUpdate() {
    updateBodyColl();
    m_hGimmickJump = 0;
    updateFooting();
}
void CPlayer::changeState(INT inState) {
    if (!((UINT)inState < 4)) AssertApp(D_004997C0, 941, D_00499828, 0);
    gotoState(*(CState*)&_m_0x21c[inState * 0x20]);
}
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Game/Player/Player", stateInitial__7CPlayer);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Game/Player/Player", initStartPos__7CPlayer);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Game/Player/Player", makeWeapon__7CPlayer);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Game/Player/Player", makeCockpit__7CPlayer);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Game/Player/Player", makeRevolver__7CPlayer);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Game/Player/Player", initActionSkill__7CPlayer);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Game/Player/Player", importStatus__7CPlayer);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Game/Player/Player", exportStatus__7CPlayer);
void CPlayer::setPlayerState(PlayerState inState) {
    m_PrePlayerState = m_PlayerState;
    m_PlayerState = inState;
    m_iSubState = 0;
    m_tqStateTimer = 0;
}
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Game/Player/Player", makeAnimIndex__7CPlayeri);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Game/Player/Player", getMotIndex__7CPlayerii);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Game/Player/Player", setAnimForce__7CPlayeriiiiii);
void CPlayer::setAnim(INT inID, INT inTq, INT inGr) {
    if (m_iNowAnimID != inID) setAnimForce(inID, inTq, inGr, 0, 0, 0);
}
void CPlayer::setAnimSpeed(FLOAT inSpeed) {
    if (m_fAnimSpeed != inSpeed) {
        m_fAnimSpeed = inSpeed;
        CEntity::setAnimSpeed(inSpeed);
        if (isPlayerFlag(0x40000000)) {
            m_pWeapon->setAnimSpeed(m_fAnimSpeed);
        }
    }
}
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Game/Player/Player", setAnimSkillEx__7CPlayeriiiii);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Game/Player/Player", setAnimRsrcEx__7CPlayeriiiiiii);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Game/Player/Player", isNowIdleAnim__7CPlayer);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Game/Player/Player", setFaceAnim__7CPlayerii);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Game/Player/Player", setTransOff__7CPlayeri);
void CPlayer::onWeapon(INT inEffect) {
    if (m_pWeapon) m_pWeapon->onWeapon(inEffect);
    m_uiPlayerFlag = (INT)m_uiPlayerFlag & ~2;
}
void CPlayer::offWeapon() {
    if (m_pWeapon) m_pWeapon->offWeapon();
    m_uiPlayerFlag |= 2;
}
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Game/Player/Player", onAttack__7CPlayer);
void CPlayer::offAttack() {
    if (m_pWeapon) m_pWeapon->offAttack();
}
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Game/Player/Player", getWeaponTopPos__7CPlayerR7FVECTOR);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Game/Player/Player", getCaskPos__7CPlayerR7FVECTOR);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Game/Player/Player", updateBodyColl__7CPlayer);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Game/Player/Player", updateBodyCollPos__7CPlayer);
BOOL CPlayer::isMuteki() const {
    if (m_uiPlayerFlag & 1) return TRUE;
    if (m_tqMutekiTimer > 0) return TRUE;
    return m_bDmgMuteki != 0;
}
void CPlayer::exeMutekiTimer() {
    m_tqMutekiTimer -= m_tqFrameRate;
    if (m_tqMutekiTimer < 0) m_tqMutekiTimer = 0;
}
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Game/Player/Player", setControl__7CPlayer);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Game/Player/Player", chkAtelHide__7CPlayeri);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Game/Player/Player", chkAtelControl__7CPlayer);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Game/Player/Player", getCameraTargetPos__7CPlayerR7FVECTOR);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Game/Player/Player", chkBodyRevise__7CPlayer);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Game/Player/Player", recoverCollMap__7CPlayer);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Game/Player/Player", setDownForce__7CPlayer);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Game/Player/Player", setLand__7CPlayerffi);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Game/Player/Player", beginStateField__7CPlayer);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Game/Player/Player", stateField__7CPlayer);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Game/Player/Player", endStateField__7CPlayer);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Game/Player/Player", beginStateBattle__7CPlayer);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Game/Player/Player", stateBattle__7CPlayer);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Game/Player/Player", endStateBattle__7CPlayer);
void CPlayer::beginStateWait() {
    m_iState = 2;
}
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Game/Player/Player", beginStateHide__7CPlayer);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Game/Player/Player", stateHide__7CPlayer);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Game/Player/Player", endStateHide__7CPlayer);
BOOL CPlayer::isNowInAir() {
    m_bAirCheck = FALSE;
    return isAir();
}
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Game/Player/Player", isAir__7CPlayer);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Game/Player/Player", getAirHeight__7CPlayerP7FVECTORfi);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Game/Player/Player", getFrontAirHeight__7CPlayerP7FVECTORfff);
void CPlayer::exeAttackTimer() {
    m_tqAttackTimer -= m_tqFrameRate;
    if (m_tqAttackTimer < 0) m_tqAttackTimer = 0;
}
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Game/Player/Player", initStartWait__7CPlayer);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Game/Player/Player", exeStartWait__7CPlayer);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Game/Player/Player", initStandUp__7CPlayeri);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Game/Player/Player", exeStandUp__7CPlayer);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Game/Player/Player", chkWalkMode__7CPlayer);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Game/Player/Player", exeWalk__7CPlayer);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Game/Player/Player", setWalkAnim__7CPlayer);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Game/Player/Player", chkBattleIdle__7CPlayer);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Game/Player/Player", procFlowIdle__7CPlayeri);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Game/Player/Player", initIdle__7CPlayerii);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Game/Player/Player", chkWall__7CPlayeri);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Game/Player/Player", chkWallBottom__7CPlayer);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Game/Player/Player", initWall__7CPlayer);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Game/Player/Player", exeWall__7CPlayer);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Game/Player/Player", chkLadderStepNow__7CPlayeri);
INT CPlayer::getLadderStepNow(FLOAT inHeight) {
    return (INT)((inHeight - m_vLadderPos.y + 0.0035f) / 0.375f);
}
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Game/Player/Player", initLadder__7CPlayeri);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Game/Player/Player", exeLadder__7CPlayer);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Game/Player/Player", isStartLadderJump__7CPlayer);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Game/Player/Player", getJumpState__C7CPlayer);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Game/Player/Player", initJump__7CPlayeri);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Game/Player/Player", initFall__7CPlayer);
void CPlayer::setFallFloat() {
    FVECTOR v = m_pMove->getVel();
    m_fJumpSpdY = 0.0f;
    v.y = m_fJumpSpdY;
    m_pMove->getVel() = v;
}
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Game/Player/Player", exeJump__7CPlayer);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Game/Player/Player", exeJumpLand__7CPlayer);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Game/Player/Player", chkHang__7CPlayer);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Game/Player/Player", initHang__7CPlayer);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Game/Player/Player", exeHang__7CPlayer);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Game/Player/Player", initAvoid__7CPlayer);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Game/Player/Player", exeGimmickRide__7CPlayer);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Game/Player/Player", notifyGimmickJump__7CPlayerii);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Game/Player/Player", chkGimmick__7CPlayerUi);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Game/Player/Player", initTreasureBox__7CPlayer);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Game/Player/Player", exeTreasureBox__7CPlayer);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Game/Player/Player", initRoomCreation__7CPlayer);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Game/Player/Player", exeRoomCreation__7CPlayer);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Game/Player/Player", initWorldSelect__7CPlayer);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Game/Player/Player", exeWorldSelect__7CPlayer);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Game/Player/Player", initCarry__7CPlayer);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Game/Player/Player", exeCarry__7CPlayer);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Game/Player/Player", resetAction__7CPlayeri);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Game/Player/Player", initAttackField__7CPlayeri);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Game/Player/Player", exeAttackField__7CPlayer);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Game/Player/Player", chkLockon__7CPlayer);
void CPlayer::cancelLockon() {
    m_pMarker->cancelLockon(m_hLockon, TRUE);
}
void CPlayer::autoLockon() {
    if (m_bIsBattleScene && !m_hLockon) m_pMarker->setMode(1);
}
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Game/Player/Player", setPushon__7CPlayeri);
void CPlayer::setTrgFlagRendaNG(const CHAR* inName) {
    if (!DebugWork::g_dw.player.iRenda) m_uiTrgFlag |= 0x40000;
}
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Game/Player/Player", chkPlayCard__7CPlayer);
/* Parked: the switch's jump table goes to .rodata, which is stripped until the unit's data comes from source
   (objcopy: symbol `.rodata' required but not present); tried: C switch over PL::ATTACK/ITEM/MAGIC/SKILL/
   GIMMICKCARD/FRIEND/SUMMON/DUEL (the code apart from the table is the original's shape) */
#if 0
INT CPlayer::isNowCardActionState() {
    switch (m_PlayerState) {
    case PL::ATTACK:
    case PL::ITEM:
    case PL::MAGIC:
    case PL::SKILL:
    case PL::GIMMICKCARD:
    case PL::FRIEND:
    case PL::SUMMON:
    case PL::DUEL:
        return 1;
    }
    return 0;
}
#else
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Game/Player/Player", isNowCardActionState__7CPlayer);
#endif
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Game/Player/Player", chkPlayCardCombo__7CPlayer);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Game/Player/Player", onMsg__7CPlayeriPv);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Game/Player/Player", exeUseStock__7CPlayeri);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Game/Player/Player", startSingleAction__7CPlayer);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Game/Player/Player", initAttack__7CPlayeri);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Game/Player/Player", setAttack__7CPlayer);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Game/Player/Player", exeAttack__7CPlayer);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Game/Player/Player", procTargetMark__7CPlayer);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Game/Player/Player", procAttackMove__7CPlayeriffif);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Game/Player/Player", procAttackCollision__7CPlayeri);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Game/Player/Player", procGravity__7CPlayeriii);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Game/Player/Player", procSkillEnd__7CPlayeri);
BOOL CPlayer::chkExtraSkill() {
    if (m_iExtraSkill) {
        m_iStepStockSkill = 2;
        m_iExtraSkill = 0;
        if (exeUseStock(1)) return TRUE;
    }
    return FALSE;
}
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Game/Player/Player", calcEnchant_AttackPoint__7CPlayerRiR8ATK_KIND8ATK_ATTRUi);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Game/Player/Player", calcEnchant_DamagePoint__7CPlayerRi8ATK_ATTR);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Game/Player/Player", setUseRsrcBlock__7CPlayeri);
void CPlayer::setDefAttr(FLOAT* inAttr) {
    for (INT i = 0; i < 6; i++) {
        m_fDefAttr[i] = inAttr[i];
    }
}
FLOAT CPlayer::getDefAttr(ATK_ATTR inAttr) const {
    if ((UINT)inAttr < 6) return m_fDefAttr[inAttr];
    return m_fDefAttr[5];
}
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Game/Player/Player", chkDamage__7CPlayer);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Game/Player/Player", initFlip__7CPlayeri);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Game/Player/Player", setFlipForce__7CPlayerPC7FVECTOR);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Game/Player/Player", exeDamage__7CPlayer);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Game/Player/Player", chkTrgAction__7CPlayeriUi);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Game/Player/Player", initDead__7CPlayer);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Game/Player/Player", exeDead__7CPlayer);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Game/Player/Player", notifyDead__7CPlayeri);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Game/Player/Player", initBadCondition__7CPlayer);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Game/Player/Player", exeBadCondition__7CPlayer);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Game/Player/Player", setConfuse__7CPlayerii);
void CPlayer::exeConfuse() {
    if (m_uiConditionFlag & 8) {
        m_tqConfuse -= m_tqFrameRate;
        if (m_tqConfuse <= 0) setConfuse(0, 0);
    }
}
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Game/Player/Player", initItem__7CPlayeri);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Game/Player/Player", exeItem__7CPlayer);
void CPlayer::exeStandUp_inFriend() {
    INT oldMode = m_WalkMode;
    INT mode = chkWalkMode();
    m_WalkMode = mode;
    if (!mode && oldMode && m_pSE) ((CSEMotion*)m_pSE)->playFootStopSE();
    exeWalk();
}
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Game/Player/Player", initCharge__7CPlayer);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Game/Player/Player", exeCharge__7CPlayer);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Game/Player/Player", chkChargeMove__7CPlayeri);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Game/Player/Player", fadeEffCharge__7CPlayer);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Game/Player/Player", setCure__7CPlayerii);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Game/Player/Player", setCastEffect__7CPlayerPCciP6CCoordP7FVECTOR);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Game/Player/Player", calcAppearPos__7CPlayerffP7FVECTOR);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Game/Player/Player", calcAppearPosFS__7CPlayerff);
void CPlayer::calcAppearPosFS_toAttackID() {
    if (m_uiAttackID < 0x78) {
        if (m_uiAttackID >= 0x76) {
            calcAppearPosFS(0.0f, m_fWallRadius);
            return;
        }
    }
    calcAppearPosFS(2.0f, m_fWallRadius);
}
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Game/Player/Player", chkEscape__7CPlayer);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Game/Player/Player", initEscape__7CPlayer);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Game/Player/Player", exeEscape__7CPlayer);
void CPlayer::setBGFade(FLOAT inRate, FLOAT inTime, INT inFlag) {
    FVECTOR v;
    v.set(inRate, inRate, inRate, 1.0f);
    setBGFade(v, inTime, inFlag);
}
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Game/Player/Player", setBGFade__7CPlayerR7FVECTORfi);
void CPlayer::setObjStop(UINT inFlag) {
    if (!inFlag) CSceneEnv::setStop((ObjType::eObjBit)0);
    else CSceneEnv::setStop((ObjType::eObjBit)0x7C);
}
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Game/Player/Player", exeWarp__7CPlayeri);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Game/Player/Player", setBanish__7CPlayeri);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Game/Player/Player", exeBanish__7CPlayer);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Game/Player/Player", setReflectGuard__7CPlayeri);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Game/Player/Player", setQuickBarrier__7CPlayeri);
void CPlayer::exeQuickBarrier() {
    if (m_tqBarrier) {
        m_tqBarrier -= m_tqFrameRate;
        if (m_tqBarrier <= 0) {
            m_collBody.m_uiFlag = (INT)m_collBody.m_uiFlag & ~2;
            m_tqBarrier = 0;
            m_collBody.m_GuardAttrFlag = 0;
        }
    }
}
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Game/Player/Player", setAeroGuard__7CPlayeri);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Game/Player/Player", exeAeroGuard__7CPlayer);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Game/Player/Player", setPrizeHealing__7CPlayeri);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Game/Player/Player", exePrizeHealing__7CPlayer);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Game/Player/Player", initGimmickCard__7CPlayeri);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Game/Player/Player", exeGimmickCard__7CPlayer);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Game/Player/Player", setTrapDamage__7CPlayeriPCcUi);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Game/Player/Player", fadeEffect__7CPlayerRii);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Game/Player/Player", fadeEffAttack__7CPlayeri);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Game/Player/Player", setEffAttack__7CPlayeriPCciiP7FVECTOR);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Game/Player/Player", updateSpotLight__7CPlayeri);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Game/Player/Player", setQuake__7CPlayerffUi);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Game/Player/Player", notifyReturnCard__7CPlayerPC4CARD);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Game/Player/Player", notifyGetPrize__7CPlayeriii);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Game/Player/Player", addEXP__7CPlayerii);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Game/Player/Player", addHP__7CPlayeriii);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Game/Player/Player", addMP__7CPlayeri);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Game/Player/Player", procDropMP__7CPlayer);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Game/Player/Player", notifyExterminate__7CPlayer);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Game/Player/Player", notifyBossDead__7CPlayeri);
HANDLE CPlayer::getLevelUpEffect() {
    if (!m_pInstance) return 0;
    return m_pInstance->m_hEffLevelUp;
}
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Game/Player/Player", initEnd__7CPlayer);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Game/Player/Player", exeEnd__7CPlayer);
BOOL CPlayer::isEnableBattleEnd() {
    if (!m_pInstance) return TRUE;
    if (m_pInstance->m_bIsDead) return FALSE;
    if (m_pInstance->m_PlayerState != 0x1C) return FALSE;
    return m_pInstance->m_iSubState >= 3;
}
BOOL CPlayer::isNowExterminate() {
    if (!m_pInstance) return TRUE;
    if (!m_pInstance->m_bIsDead) return FALSE;
    UINT m = 4;
    return (m_pInstance->m_uiBattleFlag & m) != 0;
}
void CPlayer::killBullet() {
    CBulletManager* p = (CBulletManager*)CTaskMan::getSysTask(0x1B);
    if (p) p->kill(getHandle());
}
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Game/Player/Player", setEnemyStop__7CPlayeriUi);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Game/Player/Player", setMDFooting__7CPlayeri);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Game/Player/Player", initFooting__7CPlayeri);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Game/Player/Player", exeFooting__7CPlayer);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Game/Player/Player", updateFooting__7CPlayer);
void CPlayer::setCardSnatch(INT inFlag) {
    if (m_pRevolver) {
        m_hSnatch = inFlag;
        if (inFlag > 0) {
            m_uiBattleFlag = (INT)m_uiBattleFlag & ~8;
            m_pRevolver->setSnatch(inFlag);
        }
    }
}
void CPlayer::addWind(FVECTOR* inVel) {
    if (m_pPLM) m_pPLM->addWindXYZ(inVel);
}
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Game/Player/Player", exeWind__7CPlayer);
BOOL CPlayer::cmdRevolver_rollDeck(INT inNum) {
    if (!m_pRevolver) return FALSE;
    m_pRevolver->rollDeck(inNum, 0, 300);
    return TRUE;
}
BOOL CPlayer::cmdRevolver_isEmpty() {
    if (!m_pRevolver) return TRUE;
    return m_pRevolver->getRemain(0, 0, 0) == 0;
}
INT CPlayer::cmdRevolver_isActive() {
    if (!m_pRevolver) return 0;
    return m_pRevolver->isActive();
}
void CPlayer::cmdMove_WaterSurfaceCheck(INT inFlag) {
    if (inFlag) m_pMove->setFlag(0x80, TRUE);
    else m_pMove->setFlag(0x80, FALSE);
}
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Game/Player/Player", cmdState_initWorldSelect__7CPlayeri);
void CPlayer::cmdState_setEncountWait() {
    setPlayerFlag(0x100, 0);
    changeState(2);
    setPlayerFlag(0x1001, 0);
    m_collBody.off();
    m_collBody.offBody();
}
void CPlayer::beginStateField_() {
    beginStateField();
}
void CPlayer::stateField_() {
    stateField();
}
void CPlayer::endStateField_() {
    endStateField();
}
void CPlayer::beginStateBattle_() {
    beginStateBattle();
}
void CPlayer::stateBattle_() {
    stateBattle();
}
void CPlayer::endStateBattle_() {
    endStateBattle();
}
void CPlayer::beginStateWait_() {
    beginStateWait();
}
void CPlayer::stateWait_() {
    stateWait();
}
void CPlayer::endStateWait_() {
    endStateWait();
}
void CPlayer::beginStateHide_() {
    beginStateHide();
}
void CPlayer::stateHide_() {
    stateHide();
}
void CPlayer::endStateHide_() {
    endStateHide();
}
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Game/Player/Player", __12CPlayerDebugP5CTask);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Game/Player/Player", _$_12CPlayerDebug);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Game/Player/Player", main__12CPlayerDebug);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Game/Player/Player", __static_initialization_and_destruction_0_00276330);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Game/Player/Player", _$_t14CStateTemplate1Z7CPlayer);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Game/Player/Player", getCurrentState__7CPlayer);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Game/Player/Player", stateWait__7CPlayer);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Game/Player/Player", endStateWait__7CPlayer);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Game/Player/Player", getCharaID__7CPlayer);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Game/Player/Player", getParam__7CPlayer);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Game/Player/Player", getPlayerCamera__C7CPlayer);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Game/Player/Player", getLockonMarker__C7CPlayer);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Game/Player/Player", getWeapon__C7CPlayer);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Game/Player/Player", getRevolver__C7CPlayer);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Game/Player/Player", getGauge__C7CPlayer);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Game/Player/Player", getPlayerState__C7CPlayer);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Game/Player/Player", getStateTQ__7CPlayer);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Game/Player/Player", clearStateTQ__7CPlayer);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Game/Player/Player", setSubState__7CPlayeri);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Game/Player/Player", getSubState__C7CPlayer);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Game/Player/Player", setPlayerFlag__7CPlayerUiUi);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Game/Player/Player", setActionFlag__7CPlayerUiUi);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Game/Player/Player", setConditionFlag__7CPlayerUiUi);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Game/Player/Player", setTrgFlag__7CPlayerUiUi);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Game/Player/Player", setBattleFlag__7CPlayerUiUi);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Game/Player/Player", isPlayerFlag__C7CPlayerUi);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Game/Player/Player", isActionFlag__C7CPlayerUi);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Game/Player/Player", isConditionFlag__C7CPlayerUi);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Game/Player/Player", isTrgFlag__C7CPlayerUi);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Game/Player/Player", isBattleFlag__C7CPlayerUi);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Game/Player/Player", setHP__7CPlayeri);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Game/Player/Player", setMaxHP__7CPlayeri);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Game/Player/Player", setCP__7CPlayeri);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Game/Player/Player", setMaxCP__7CPlayeri);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Game/Player/Player", getHP__7CPlayer);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Game/Player/Player", getMaxHP__7CPlayer);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Game/Player/Player", getCP__7CPlayer);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Game/Player/Player", getMaxCP__7CPlayer);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Game/Player/Player", setAP__7CPlayeri);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Game/Player/Player", getAP__7CPlayer);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Game/Player/Player", setLV__7CPlayeri);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Game/Player/Player", getLV__7CPlayer);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Game/Player/Player", setEXP__7CPlayeri);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Game/Player/Player", getEXP__7CPlayer);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Game/Player/Player", setMP__7CPlayeri);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Game/Player/Player", getMP__7CPlayer);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Game/Player/Player", setDP__7CPlayeri);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Game/Player/Player", setMaxDP__7CPlayeri);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Game/Player/Player", getDP__7CPlayer);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Game/Player/Player", getMaxDP__7CPlayer);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Game/Player/Player", getCount__C7CPlayer);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Game/Player/Player", getNowAnimID__7CPlayer);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Game/Player/Player", getAnimID__7CPlayer);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Game/Player/Player", setStartPos__7CPlayerfff);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Game/Player/Player", setStartDir__7CPlayerRCf);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Game/Player/Player", setDir__7CPlayerRCf);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Game/Player/Player", getStartPos__7CPlayer);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Game/Player/Player", getStartDir__7CPlayer);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Game/Player/Player", getDir__7CPlayer);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Game/Player/Player", getCenterPos__7CPlayer);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Game/Player/Player", getMovePower__7CPlayer);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Game/Player/Player", getWalkSpeed__7CPlayer);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Game/Player/Player", getLockOnPos__7CPlayerR7FVECTOR);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Game/Player/Player", getHeadPos__C7CPlayerR7FVECTOR);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Game/Player/Player", getUseCardPos__7CPlayerR7FVECTOR);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Game/Player/Player", getAppearPos__7CPlayer);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Game/Player/Player", setDrawRadius__7CPlayerff);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Game/Player/Player", getPrizeDrawRadius__7CPlayer);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Game/Player/Player", getCardDrawRadius__7CPlayer);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Game/Player/Player", getLadderPos__7CPlayer);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Game/Player/Player", getLadderStep__7CPlayer);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Game/Player/Player", getLadderStepMax__7CPlayer);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Game/Player/Player", getJumpSpdY__7CPlayer);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Game/Player/Player", getJumpTop__7CPlayer);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Game/Player/Player", getJumpStartPos__7CPlayer);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Game/Player/Player", getJumpLandLimit__C7CPlayer);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Game/Player/Player", getHangHandPos__7CPlayer);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Game/Player/Player", setAttackTimer__7CPlayeri);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Game/Player/Player", setMaxCombo__7CPlayeri);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Game/Player/Player", getAttackID__7CPlayer);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Game/Player/Player", setMutekiTimer__7CPlayeri);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Game/Player/Player", isDead__C7CPlayer);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Game/Player/Player", setBodyRviseCoefficient__7CPlayerf);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Game/Player/Player", getConfuse__C7CPlayer);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Game/Player/Player", setMarkPos__7CPlayerfff);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Game/Player/Player", getMarkPos__7CPlayer);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Game/Player/Player", isNowLockon__7CPlayer);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Game/Player/Player", getLockonPos__7CPlayer);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Game/Player/Player", isNowMark__7CPlayer);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Game/Player/Player", isNowTarget__7CPlayer);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Game/Player/Player", getOldLockon__7CPlayer);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Game/Player/Player", getOldCamMode__7CPlayer);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Game/Player/Player", setStateSE__7CPlayerii);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Game/Player/Player", getStateSESeq__7CPlayeri);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Game/Player/Player", setAttack_inFriend__7CPlayeriii);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Game/Player/Player", startStockSkillAction__7CPlayer);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Game/Player/Player", initStockSkill__7CPlayeri);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Game/Player/Player", exeStockSkill__7CPlayer);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Game/Player/Player", initMagic__7CPlayeri);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Game/Player/Player", exeMagic__7CPlayer);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Game/Player/Player", setMagic__7CPlayeriii);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Game/Player/Player", initFriend__7CPlayeri);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Game/Player/Player", exeFriend__7CPlayer);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Game/Player/Player", initSummon__7CPlayeri);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Game/Player/Player", exeSummon__7CPlayer);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Game/Player/Player", exeAttack_inFriend__7CPlayer);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Game/Player/Player", endAttack_inFriend__7CPlayer);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Game/Player/Player", initTransform__7CPlayeri);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Game/Player/Player", exeTransform__7CPlayer);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Game/Player/Player", initGlide__7CPlayeri);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Game/Player/Player", exeGlide__7CPlayer);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Game/Player/Player", initJump2nd__7CPlayer);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Game/Player/Player", exeJump2nd__7CPlayer);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Game/Player/Player", exeAvoid__7CPlayer);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Game/Player/Player", initPronounce__7CPlayeri);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Game/Player/Player", exePronounce__7CPlayer);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Game/Player/Player", executeBeginState__t14CStateTemplate1Z7CPlayer);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Game/Player/Player", executeState__t14CStateTemplate1Z7CPlayer);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Game/Player/Player", executeEndState__t14CStateTemplate1Z7CPlayer);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Game/Player/Player", func_00277080);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Game/Player/Player", _GLOBAL_$I$c__KHCOM_MainApp_Game_Player_Player.cppopbaaa);
