#include "khtypes.h"
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
#include "include_asm.h"

INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Game/Player/Player", DebugPlayerCamera__9DWTPlayerP12CDebugWindowii_0025F688);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Game/Player/Player", DebugPlayerAttack__9DWTPlayerP12CDebugWindowii_0025F798);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Game/Player/Player", DebugWinPlayer__9DWTPlayerP12CDebugWindowii_0025FA48);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Game/Player/Player", getPlayerPos__Fi);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Game/Player/Player", calcNextEXP__7CPlayeri);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Game/Player/Player", getNowNextEXP__7CPlayer);
INT CPlayer::getNowTotalMP() {
    if (!m_pInstance) return CStatus::getMP();
    return m_pInstance->m_iMP;
}
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Game/Player/Player", enablePadControl__7CPlayeri);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Game/Player/Player", enableDbgPadControl__7CPlayeri);
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
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Game/Player/Player", changeState__7CPlayeri);
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
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Game/Player/Player", setAnimSpeed__7CPlayerf);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Game/Player/Player", setAnimSkillEx__7CPlayeriiiii);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Game/Player/Player", setAnimRsrcEx__7CPlayeriiiiiii);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Game/Player/Player", isNowIdleAnim__7CPlayer);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Game/Player/Player", setFaceAnim__7CPlayerii);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Game/Player/Player", setTransOff__7CPlayeri);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Game/Player/Player", onWeapon__7CPlayeri);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Game/Player/Player", offWeapon__7CPlayer);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Game/Player/Player", onAttack__7CPlayer);
void CPlayer::offAttack() {
    if (m_pWeapon) m_pWeapon->offAttack();
}
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Game/Player/Player", getWeaponTopPos__7CPlayerR7FVECTOR);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Game/Player/Player", getCaskPos__7CPlayerR7FVECTOR);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Game/Player/Player", updateBodyColl__7CPlayer);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Game/Player/Player", updateBodyCollPos__7CPlayer);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Game/Player/Player", isMuteki__C7CPlayer);
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
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Game/Player/Player", getLadderStepNow__7CPlayerf);
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
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Game/Player/Player", autoLockon__7CPlayer);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Game/Player/Player", setPushon__7CPlayeri);
void CPlayer::setTrgFlagRendaNG(const CHAR* inName) {
    if (!DebugWork::g_dw.player.iRenda) m_uiTrgFlag |= 0x40000;
}
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Game/Player/Player", chkPlayCard__7CPlayer);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Game/Player/Player", isNowCardActionState__7CPlayer);
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
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Game/Player/Player", chkExtraSkill__7CPlayer);
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
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Game/Player/Player", exeConfuse__7CPlayer);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Game/Player/Player", initItem__7CPlayeri);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Game/Player/Player", exeItem__7CPlayer);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Game/Player/Player", exeStandUp_inFriend__7CPlayer);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Game/Player/Player", initCharge__7CPlayer);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Game/Player/Player", exeCharge__7CPlayer);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Game/Player/Player", chkChargeMove__7CPlayeri);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Game/Player/Player", fadeEffCharge__7CPlayer);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Game/Player/Player", setCure__7CPlayerii);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Game/Player/Player", setCastEffect__7CPlayerPCciP6CCoordP7FVECTOR);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Game/Player/Player", calcAppearPos__7CPlayerffP7FVECTOR);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Game/Player/Player", calcAppearPosFS__7CPlayerff);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Game/Player/Player", calcAppearPosFS_toAttackID__7CPlayer);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Game/Player/Player", chkEscape__7CPlayer);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Game/Player/Player", initEscape__7CPlayer);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Game/Player/Player", exeEscape__7CPlayer);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Game/Player/Player", setBGFade__7CPlayerffi);
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
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Game/Player/Player", exeQuickBarrier__7CPlayer);
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
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Game/Player/Player", isEnableBattleEnd__7CPlayer);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Game/Player/Player", isNowExterminate__7CPlayer);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Game/Player/Player", killBullet__7CPlayer);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Game/Player/Player", setEnemyStop__7CPlayeriUi);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Game/Player/Player", setMDFooting__7CPlayeri);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Game/Player/Player", initFooting__7CPlayeri);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Game/Player/Player", exeFooting__7CPlayer);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Game/Player/Player", updateFooting__7CPlayer);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Game/Player/Player", setCardSnatch__7CPlayeri);
void CPlayer::addWind(FVECTOR* inVel) {
    if (m_pPLM) m_pPLM->addWindXYZ(inVel);
}
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Game/Player/Player", exeWind__7CPlayer);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Game/Player/Player", cmdRevolver_rollDeck__7CPlayeri);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Game/Player/Player", cmdRevolver_isEmpty__7CPlayer);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Game/Player/Player", cmdRevolver_isActive__7CPlayer);
void CPlayer::cmdMove_WaterSurfaceCheck(INT inFlag) {
    if (inFlag) m_pMove->setFlag(0x80, TRUE);
    else m_pMove->setFlag(0x80, FALSE);
}
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Game/Player/Player", cmdState_initWorldSelect__7CPlayeri);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Game/Player/Player", cmdState_setEncountWait__7CPlayer);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Game/Player/Player", beginStateField___7CPlayer);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Game/Player/Player", stateField___7CPlayer);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Game/Player/Player", endStateField___7CPlayer);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Game/Player/Player", beginStateBattle___7CPlayer);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Game/Player/Player", stateBattle___7CPlayer);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Game/Player/Player", endStateBattle___7CPlayer);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Game/Player/Player", beginStateWait___7CPlayer);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Game/Player/Player", stateWait___7CPlayer);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Game/Player/Player", endStateWait___7CPlayer);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Game/Player/Player", beginStateHide___7CPlayer);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Game/Player/Player", stateHide___7CPlayer);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Game/Player/Player", endStateHide___7CPlayer);
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
