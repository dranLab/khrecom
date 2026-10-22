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
#include "System/Graphic/Effect/effect.h"
#include "System/Coord.h"
#include "Game/Player/PlayerAnim.h"
#include "Game/GameWork.h"
#include "Form/Menu/WorldSelect.h"
#include "Game/Player/PlayerCamera.h"
#include "Game/SceneInfo.h"
#include "Game/Gimmick/BaseGimmick.h"
#include "Game/Collision/CollMap.h"
#include "System/HeapMan.h"
#include "Game/Enemy/BattleEnemyManager.h"
#include "Game/CardKind.h"
#include "System/Graphic/Camera.h"
#include "System/Graphic/WorldEnv.h"
#include "Game/Game.h"
#include "Game/Bullet/Bullet.h"
#include "include_asm.h"

extern "C" char D_004997C0[];
extern "C" char D_00499828[];
extern "C" char D_00499A18[];
void ApplyMatrixXYZ(FVECTOR& v0, const FMATRIX& m0, const FVECTOR& v1);
void AddVectorXYZ(FVECTOR& v0, const FVECTOR& v1, const FVECTOR& v2);
extern "C" char D_0049B308[];
extern "C" char D_0049AFC0[]; /* "c:/KHCOM/MainApp/Game/Player/Player.cpp(12257)" */
extern "C" char D_0049B238[]; /* TRACE message (Shift-JIS) of notifyExterminate */
extern "C" char D_0049AEC0[]; /* TRACE message of initGimmickCard */
inline void* operator new(size_t, void* p) throw() { return p; }
/* System/RsrcDB.h parses only with RSRCDATA_H_FULL: local declarations of what makeAnimIndex needs */
enum NAME_CASE { NAME_CASE_SENSITIVE = 0, NAME_CASE_UPPER = 1, NAME_CASE_LOWER = 2 };
class CRsrcDB {
public:
    static INT getMotionIdxFromMotName(const CHAR* pszName, const CHAR* pszMotName, NAME_CASE nc);
};
namespace PL {
extern FVECTOR g_vDummyPlayerPos; // data from the asm until SOURCE DATA
extern FVECTOR g_vBanishPos;
extern BULLET_EFFECT_INFO g_EffTbl_Skill[67][4]; // PlayerAttack.h in the original
extern LPCSTR g_szDpxAlways; // static in the original; data from the asm until SOURCE DATA
extern LPCSTR g_szDpxReload; // static in the original
extern FLOAT g_fGimmickJumpPower[8];
}
namespace Gauge {
class CPlayerGauge { /* PARTIAL: Game/Player/PlayerGauge.h declares the class only for its own unit */
public:
    void setHP(INT inHP, INT inFlag);
};
class CPrizeGauge { /* PARTIAL: Game/Player/PlayerGauge.h declares the class only for its own unit */
public:
    static void setPrizeMp(INT inMP);
};
}
#define RET_ADDR() ({ UINT nRA; __asm__ volatile("daddu %0, $31, $0" : "=r"(nRA)); nRA; })

INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Game/Player/Player", DebugPlayerCamera__9DWTPlayerP12CDebugWindowii_0025F688);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Game/Player/Player", DebugPlayerAttack__9DWTPlayerP12CDebugWindowii_0025F798);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Game/Player/Player", DebugWinPlayer__9DWTPlayerP12CDebugWindowii_0025FA48);
FVECTOR getPlayerPos(BOOL flag) {
    if (g_pPlayer) {
        if (!flag && g_pPlayer->isConditionFlag(0x40)) return PL::g_vBanishPos;
        return g_pPlayer->getPos();
    }
    return PL::g_vDummyPlayerPos;
}
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
void CPlayer::updateBefore() {
    *(BOOL*)&m_bPreAir = m_bAir;
    m_fWeight = PL::g_fWeight;
    PL::g_fAntiGravity = PL::g_fWeight * 0.008166667f;
    if (CardBattle::CCardBattle::isLevitate()) m_fWeight *= 0.5f;
    PL::g_fGravity = m_fWeight * -0.008166667f;
}
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
INT CPlayer::importStatus() {
    if (!_instance()) return 0;
    _instance()->setMaxHP(CStatus::getStaus().HPMax);
    _instance()->setHP(CStatus::getStaus().HP);
    _instance()->setAP(CStatus::getStaus().AP);
    _instance()->setEXP(CStatus::getStaus().EXP);
    _instance()->setLV(CStatus::getStaus().LV);
    _instance()->setMP(CStatus::getStaus().MP);
    _instance()->setMaxDP(CStatus::getStaus().DPMax);
    return 1;
}
INT CPlayer::exportStatus() {
    if (!_instance()) return 0;
    CStatus::setHPMax(_instance()->m_iMaxHP);
    CStatus::setHP(_instance()->m_iHP);
    CStatus::setAP(_instance()->m_iAP);
    CStatus::setEXP(_instance()->m_iEXP);
    CStatus::setMP(_instance()->m_iMP);
    return 1;
}
void CPlayer::setPlayerState(PlayerState inState) {
    m_PrePlayerState = m_PlayerState;
    m_PlayerState = inState;
    m_iSubState = 0;
    m_tqStateTimer = 0;
}
void CPlayer::makeAnimIndex(INT inChara) {
    if (inChara) {
        MOT_NAME_TBL* tbl = PL_ANIM::g_MotNameTbl[inChara];
        LPCSTR file = PL_ANIM::g_pPlayerModelName[inChara];
        while (tbl->pFileName) {
            tbl->iIndex = CRsrcDB::getMotionIdxFromMotName(file, tbl->pFileName, NAME_CASE_SENSITIVE);
            tbl++;
        }
    }
}
INT CPlayer::getMotIndex(INT inChara, INT inIndex) {
    if (!(0 <= inIndex && inIndex < ANIM_MAX)) AssertApp(D_004997C0, 1361, D_00499A18, 0);
    return PL_ANIM::g_MotNameTbl[inChara][inIndex].iIndex;
}
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
void CPlayer::setAnimRsrcEx(INT inRcid, INT inAnimID, TIMEQ inChangeTime, BOOL inRootMove, INT inEffSeq, INT inSESeq, INT inWep) {
    m_iNowAnimID = -1;
    changeMotionEx(inRcid + 10000, inAnimID, 0, inRootMove, inChangeTime, inEffSeq, inSESeq);
    if (isPlayerFlag(0x40000000) && inWep > 0) {
        m_pWeapon->changeMotionEx(inRcid + inWep + 10000, inAnimID, 0, FALSE, inChangeTime, inEffSeq, inSESeq);
    }
    setAnimSpeed(1.0f);
}
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Game/Player/Player", isNowIdleAnim__7CPlayer);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Game/Player/Player", setFaceAnim__7CPlayerii);
void CPlayer::setTransOff(INT inFlag) {
    if (inFlag) {
        setEntityFlag(0x1000, 0);
        if (isPlayerFlag(0x40000000)) m_pWeapon->setEntityFlag(0x1000, 0);
    } else {
        setEntityFlag(0, 0x1000);
        if (isPlayerFlag(0x40000000)) m_pWeapon->setEntityFlag(0, 0x1000);
    }
}
void CPlayer::onWeapon(INT inEffect) {
    if (m_pWeapon) m_pWeapon->onWeapon(inEffect);
    m_uiPlayerFlag = (INT)m_uiPlayerFlag & ~2;
}
void CPlayer::offWeapon() {
    if (m_pWeapon) m_pWeapon->offWeapon();
    m_uiPlayerFlag |= 2;
}
void CPlayer::onAttack() {
    if (m_pWeapon) {
        m_pWeapon->onAttack(m_uiAttackID, m_iAttackPower, m_AtkKind, m_AtkAttr);
        onWeapon(1);
        if (m_uiAttackID < 8 || m_uiAttackID - 0x97 < 14) {
            if (isConditionFlag(0x80)) m_pWeapon->setWideAttack(1);
            if (isConditionFlag(0x100000)) m_pWeapon->setBackAttack(1);
            if (isConditionFlag(0x20000000)) m_pWeapon->setDoubleHit(1, NULL);
        }
    }
}
void CPlayer::offAttack() {
    if (m_pWeapon) m_pWeapon->offAttack();
}
void CPlayer::getWeaponTopPos(FVECTOR& inVec) {
    if (m_pWeapon) {
        FVECTOR ofs = PlayerWeapon::g_fWeaponTopOffset;
        FMATRIX m = m_pWeapon->getMatrixWep0();
        ApplyMatrixXYZ(inVec, m, ofs);
    }
}
void CPlayer::getCaskPos(FVECTOR& inVec) {
    if (m_pWeapon) {
        FVECTOR ofs = { { -0.06f, -0.27f, 0.03f, 1.0f } };
        if (DebugWork::g_dw.player.iTemp == 1) ofs = DebugWork::g_dw.player.vTemp;
        FMATRIX m = m_pWeapon->getMatrixWep0();
        ApplyMatrixXYZ(inVec, m, ofs);
    }
}
void CPlayer::updateBodyColl() {
    updateBodyCollPos();
    m_collBody.m_uiFlag = (INT)m_collBody.m_uiFlag & ~8;
    m_collBody.m_uiFlag = (INT)m_collBody.m_uiFlag & ~0x20;
    CCollBody& body = m_collBody;
    if (isPlayerFlag(1)) body.CCollBody::off();
    else body.CCollBody::on();
    if (isPlayerFlag(0x1000)) m_collBody.m_enableBody = 0;
    else m_collBody.m_enableBody = 1;
}
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
INT CPlayer::chkAtelHide(INT inFlag) {
    if (inFlag) {
        if (CGameWork::getFlag(1, 0)) {
            changeState(3);
            return TRUE;
        }
    } else {
        if (!CGameWork::getFlag(1, 0)) {
            if (!m_bIsBattleScene) changeState(0);
            else changeState(1);
            return TRUE;
        }
    }
    return FALSE;
}
void CPlayer::chkAtelControl() {
    if (CGameWork::getFlag(1, 5)) {
        if (isControl()) {
            m_uiPadFlag = (INT)m_uiPadFlag & ~1;
            m_pCam->setFlag(1, TRUE);
        }
    } else {
        if (!isControl()) {
            enableControl();
            m_pCam->setFlag(1, FALSE);
        }
    }
}
void CPlayer::getCameraTargetPos(FVECTOR& outPos) {
    if (m_PlayerState == PL::TRANSFORM) {
        outPos = Crd(m_iBoneCenter).getWorldMatrix().v[3];
    } else {
        outPos = Crd(0).getWorldMatrix().v[3];
        outPos.y += 0.5f;
    }
}
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Game/Player/Player", chkBodyRevise__7CPlayer);
void CPlayer::recoverCollMap() {
    if (m_pCollMap) {
        m_pCollMap->setType(COLL_TYPE_SPHERE);
        m_pCollMap->enableSliding(1);
        m_pMove->setFootRadius(m_fFootRadius);
        m_pMove->setWallRadius(m_fWallRadius);
    }
}
void CPlayer::setDownForce() {
    if (!isConditionFlag(0x10000000)) {
        if (!getJumpState()) {
            FVECTOR vel = m_pMove->getVel();
            vel.y = -0.05f;
            m_pMove->setVel(vel);
        }
    } else {
        FVECTOR vel = m_pMove->getVel();
        FVECTOR pos = m_pMove->getPos();
        pos.y += vel.y;
        if (pos.y <= m_fJumpLandLimit) {
            pos.y = m_fJumpLandLimit;
            if (vel.y < 0.0f) vel.y = 0.0f;
            m_pMove->getPos() = pos;
            m_pMove->setVel(vel);
            m_bAir = FALSE;
        }
    }
}
void CPlayer::setLand(FLOAT inDownLEngth, FLOAT inUpLength, BOOL inFirstHit) {
    recoverCollMap();
    land(inDownLEngth, inUpLength, inFirstHit);
}
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Game/Player/Player", beginStateField__7CPlayer);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Game/Player/Player", stateField__7CPlayer);
void CPlayer::endStateField() {
    setActionFlag(0, m_pParam->USE_MAP_ACTION);
    setBattleFlag(0, 1);
    if (CGameWork::getFlag(1, 0)) offWeapon();
    m_collBody.offBody();
    offAttack();
    m_uiAttackKind = 0;
    m_uiPreAttackKind = 0;
    m_uiAttackID = 0;
    m_pMarker->setFlag(FALSE);
}
void CPlayer::beginStateBattle() {
    m_iState = 1;
    m_uiStep = 0;
    setActionFlag(m_pParam->USE_BTL_ACTION, 0);
    initActionSkill();
    setBattleFlag(1, 0);
    setAnimForce(0x2A, -1, 0, 0, 0, 0);
    onWeapon(0);
    m_pPLM->setReqLoadFirst();
    initStartWait();
    if (PL::g_uiStartCondition & 8) setConfuse(1, 0);
    PL::g_uiStartCondition = 0;
    m_szEffectName = NULL;
}
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Game/Player/Player", stateBattle__7CPlayer);
void CPlayer::endStateBattle() {
    setActionFlag(0, m_pParam->USE_BTL_ACTION);
    m_collBody.offBody();
    offAttack();
    setBattleFlag(0, 1);
    offWeapon();
    m_uiAttackKind = 0;
    m_uiPreAttackKind = 0;
    m_uiAttackID = 0;
    m_pMarker->setFlag(FALSE);
}
void CPlayer::beginStateWait() {
    m_iState = 2;
}
void CPlayer::beginStateHide() {
    m_iState = 3;
    setPlayerState(PL::HIDE);
    clearSpeed();
    setEntityFlag(1, 0);
    m_WalkMode = 0;
    if (m_bIsBattleScene) setAnimForce(0x2A, 0, 0, 0, 0, 0);
    else setAnimForce(1, 0, 0, 0, 0, 0);
    setPlayerFlag(0x401001, 0);
    m_pCam->setFlag(1, TRUE);
}
void CPlayer::stateHide() {
    setPlayerFlag(0x400000, 0);
    if (dbgCheckUser(KKIKUCHI) && dbgButtonTrg(USR_SYSTEM, 0x10, 1)) {
        CGameWork::setInt(1, 0xE2, 0);
    }
    chkAtelHide(0);
}
void CPlayer::endStateHide() {
    setEntityFlag(0, 1);
    setPlayerFlag(0, 0x1001);
    setLand(5.0f, 0.5f, 1);
    clearSpeed();
    m_pCam->setFlag(1, FALSE); /* the partial CPlayerCamera keeps (flag, on/off): the original is setFlag(0, 1) */
}
BOOL CPlayer::isNowInAir() {
    m_bAirCheck = FALSE;
    return isAir();
}
BOOL CPlayer::isAir() {
    if (m_bAirCheck) return m_bAir;
    m_bAirCheck = TRUE;
    if (m_pMove->isGround()) {
        m_bAir = FALSE;
        return FALSE;
    }
    FVECTOR pos = m_pMove->getPos();
    if (pos.y <= m_fJumpLandLimit) {
        pos.y = m_fJumpLandLimit;
        m_pMove->getPos() = pos;
        m_bAir = FALSE;
        return FALSE;
    }
    m_bAir = TRUE;
    return TRUE;
}
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Game/Player/Player", getAirHeight__7CPlayerP7FVECTORfi);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Game/Player/Player", getFrontAirHeight__7CPlayerP7FVECTORfff);
void CPlayer::exeAttackTimer() {
    m_tqAttackTimer -= m_tqFrameRate;
    if (m_tqAttackTimer < 0) m_tqAttackTimer = 0;
}
void CPlayer::initStartWait() {
    setPlayerState(START_WAIT);
    setTimer(1500);
    setLand(2.0f, 0.5f, 1);
    m_bAirCheck = FALSE;
    m_bAir = FALSE;
    m_fJumpSpdY = 0.0f;
    m_uiPreAttackKind = 0;
    m_uiAttackKind = 0;
    m_uiAttackID = 0;
    m_tqAttackTimer = 0;
    setPlayerFlag(0x400001, 0);
    offAttack();
    m_WalkMode = 0;
    m_uiPadFlag = (INT)m_uiPadFlag & ~1;
    m_hTarget = 0;
    m_hLockon = 0;
    m_hGimmick = 0;
    m_pCam->setFlag(1, TRUE);
}
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Game/Player/Player", exeStartWait__7CPlayer);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Game/Player/Player", initStandUp__7CPlayeri);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Game/Player/Player", exeStandUp__7CPlayer);
INT CPlayer::chkWalkMode() {
    FLOAT aim_power = m_fAnalogPower;
    if (aim_power < 0.3f) {
        m_fWalkSpeed = 0.0f;
        return 0;
    }
    FLOAT dash = 1.0f;
    if (m_pCB && m_pCB->getEnchantKind() == 0x2D) dash = 1.7f;
    if (aim_power < 0.9f) {
        m_fWalkSpeed = m_pParam->MAX_WALK_SPEED * dash;
        return 1;
    }
    m_fWalkSpeed = aim_power * m_pParam->MAX_RUN_SPEED * dash;
    return 2;
}
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Game/Player/Player", exeWalk__7CPlayer);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Game/Player/Player", setWalkAnim__7CPlayer);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Game/Player/Player", chkBattleIdle__7CPlayer);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Game/Player/Player", procFlowIdle__7CPlayeri);
INT CPlayer::initIdle(INT inAir, INT inStand) {
    INT bAir = inAir;
    if (bAir < 0) bAir = isAir();
    if (!bAir) {
        initStandUp(inStand);
    } else {
        initFall();
    }
    return bAir;
}
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Game/Player/Player", chkWall__7CPlayeri);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Game/Player/Player", chkWallBottom__7CPlayer);
void CPlayer::initWall() {
    setPlayerState(PL::WALL);
    clearSpeed();
    offWeapon();
    m_iSubState = 0;
    if (CGame::isSoraGame()) setAnimRsrcEx(0xFD2, 0, -1, 0, 0, 0, 1);
    else setAnimRsrcEx(0xFD4, 0, -1, 0, 0, 0, 1);
    setPos(m_vWallPos);
    m_JumpState = 6;
    m_pMove->setFlag(0x40, FALSE); // partial CMovement: (flag, on)
    m_fFootRadius = 0.5f;
}
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
void CPlayer::initFall() {
    initJump(1);
    *(INT*)&m_JumpState = 3;
    m_fJumpGravity = PL::g_fGravity;
    setAnimForce(6, 800, 0, 0, 0, 0);
    offAttack();
    setPlayerFlag(0, 0x020C2000);
    setBattleFlag(0, 0x00443F90);
    setFaceAnim(0, 0);
    m_pMove->setFlag(0x40, TRUE); // partial CMovement: (flag, on)
}
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
void CPlayer::notifyGimmickJump(HANDLE inHandle, INT inPow) {
    if (!_instance()) return;
    _instance()->m_hGimmickJump = inHandle;
    if (inPow == -1) {
        _instance()->m_fGimmickJumpPow = PL::g_fGimmickJumpPower[((CBaseGimmick*)CTaskMan::getTask(inHandle))->getParam()];
    } else {
        _instance()->m_fGimmickJumpPow = PL::g_fGimmickJumpPower[inPow];
    }
}
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Game/Player/Player", chkGimmick__7CPlayerUi);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Game/Player/Player", initTreasureBox__7CPlayer);
void CPlayer::exeTreasureBox() {
    setPlayerFlag(0x400000, 0);
    if (isMotionEnd()) {
        m_bDmgMuteki = FALSE;
        setPlayerFlag(0, 1);
        m_collBody.on();
        initStandUp(0);
        m_pMarker->setFlag(TRUE);
        m_pMarker->holdTarget(0);
    }
}
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Game/Player/Player", initRoomCreation__7CPlayer);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Game/Player/Player", exeRoomCreation__7CPlayer);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Game/Player/Player", initWorldSelect__7CPlayer);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Game/Player/Player", exeWorldSelect__7CPlayer);
void CPlayer::initCarry() {
    setPlayerState(PL::CARRY);
    m_iSubState = 0;
    clearSpeed();
    offWeapon();
    setAnimForce(0x17, 0, 0, 0, 0, 0);
    m_pCam->setFlag(0x800, TRUE); /* the partial CPlayerCamera keeps (flag, on/off): the original is setFlag(0x800, 0) */
}
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
void CPlayer::setPushon(INT inFlag) {
    if (m_bIsBattleScene) {
        if (inFlag) {
            m_pMarker->onPush(inFlag);
        } else {
            m_pMarker->offPush();
            m_uiTrgFlag &= ~0x40000;
        }
    }
}
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
INT CPlayer::exeUseStock(INT inFlag) {
    if (isBattleFlag(4) || m_bIsDead) return FALSE;
    switch (m_iStepStockSkill) {
    case 0:
        if (m_iFirstSkill <= 0 && startStockSkillAction()) return TRUE;
        return startSingleAction();
    case 1:
        if (!inFlag) return FALSE;
        if (startStockSkillAction()) return TRUE;
        return startSingleAction();
    case 2:
        if (!inFlag) return FALSE;
        return startSingleAction();
    default:
        return FALSE;
    }
}
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Game/Player/Player", startSingleAction__7CPlayer);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Game/Player/Player", initAttack__7CPlayeri);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Game/Player/Player", setAttack__7CPlayer);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Game/Player/Player", exeAttack__7CPlayer);
void CPlayer::procTargetMark() {
    CEntity* pTarget = m_hTarget > 0 ? (CEntity*)CTaskMan::getTask(m_hTarget) : NULL;
    if (pTarget) {
        m_hMark = m_hTarget;
        pTarget->getLockOnPos(m_vMarkPos);
    } else {
        m_hMark = 0;
        m_vMarkPos.set(0.0f, 0.0f, 0.0f);
    }
}
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
void CPlayer::calcEnchant_DamagePoint(INT& inPoint, ATK_ATTR inAttr) {
    if (m_pCB) {
        CardBattle::ENCHANT_PARAM param;
        param.init(getHandle(), 1);
        param.uiState = 2;
        param.AtkAttr = inAttr;
        param.iAttackPoint = inPoint;
        m_pCB->calcEnchantParam(param);
        inPoint = param.iAttackPoint;
        if (param.iGuard > 0) Gauge::CCommonGauge::setDispSeq(getHandle(), 2);
    }
}
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
void CPlayer::setFlipForce(const FVECTOR* inPos) {
    FVECTOR pos;
    if (inPos) {
        pos = *inPos;
    } else {
        pos = m_vStartPos;
        pos.y = getPos().y + 0.0035f;
    }
    warp(pos);
    updateCoord();
    coordFix();
    clearSpeed();
    initFlip(0);
    CardBattle::CCardBattle::notifyCompleteGimmick();
    PLM::setVibration(3);
}
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
void CPlayer::initCharge() {
    setPlayerState(PL::CHARGE);
    clearSpeed();
    setTrgFlag(0, 0x10);
    setAnimForce(0x2E, -1, 0, 0, 0, 0);
    m_tqEffCharge = 1000;
    m_bDmgMuteki = FALSE;
    if (m_hEffCharge <= 0) {
        FVECTOR pos;
        pos._q = 0;
        pos.w = 1.0f;
        CCoord* crd = &Crd(0);
        m_hEffCharge = attachEffect(PL::g_szDpxReload, 0, crd, &pos, NULL, 0.0f);
        CEffect* effect = CEffectMan::getEffect(m_hEffCharge);
        if (effect) effect->m_iFlag |= 4;
    }
}
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Game/Player/Player", exeCharge__7CPlayer);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Game/Player/Player", chkChargeMove__7CPlayeri);
void CPlayer::fadeEffCharge() {
    if (m_hEffCharge > 0) {
        m_tqEffCharge -= getFrameRate();
        if (m_tqEffCharge <= 0) {
            m_tqEffCharge = 0;
            CEffect* effect = CEffectMan::getEffect(m_hEffCharge);
            if (effect) effect->fadeout(8.0f);
            m_hEffCharge = 0;
        }
    }
}
void CPlayer::setCure(INT inID, INT inPow) {
    FVECTOR pos;
    INT no = inID - 14;
    if (inID == 0x33) no = 0;
    pos.set(0.0f, 0.85f, 0.0f);
    CEffect* effect = CEffectMan::getEffect(attachEffect(PL::g_EffTbl_Skill[inID][1].pEffectName, no, &Crd(0), &pos, NULL, 0.0f));
    if (effect) effect->m_iFlag |= 4;
    addHP(inPow, 0, 1);
}
void CPlayer::setCastEffect(const CHAR* inName, INT inNo, CCoord* inCrd, FVECTOR* inOfs) {
    FVECTOR pos;
    if (inOfs) {
        pos = *inOfs;
    } else {
        pos._q = 0;
        pos.w = 1.0f;
    }
    m_hEffCast = attachEffect(inName, inNo, inCrd, &pos, NULL, 0.0f);
    CEffect* effect = CEffectMan::getEffect(m_hEffCast);
    if (effect) effect->m_iFlag |= 4;
}
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
void CPlayer::exeEscape() {
    setPlayerFlag(0x400000, 0);
    m_pCam->setFlag(0x1000, TRUE); /* the partial CPlayerCamera keeps (flag, on/off): the original is setFlag(0x1000, 0) */
    if (!m_iSubState) {
        m_tqEscape += m_tqFrameRate;
        if (m_tqEscape >= 2501) {
            if (CSceneInfo::reqExitBattle()) m_iSubState = 1;
        }
    }
}
void CPlayer::setBGFade(FLOAT inRate, FLOAT inTime, INT inFlag) {
    FVECTOR v;
    v.set(inRate, inRate, inRate, 1.0f);
    setBGFade(v, inTime, inFlag);
}
void CPlayer::setBGFade(FVECTOR& inColor, FLOAT inFrame, INT inFlag) {
    INT iObjType = 0x1808;
    if (inFlag & 1) iObjType = 0x187C;
    if (inColor.x + inColor.y + inColor.z == 3.0f) setPlayerFlag(0, 0x80000000);
    else setPlayerFlag(0x80000000, 0);
    CSceneEnv::setColor((ObjType::eObjBit)iObjType, inFrame, inColor, NULL, 0);
    if (inFlag & 2) CSceneEnv::setStop((ObjType::eObjBit)iObjType);
}
void CPlayer::setObjStop(UINT inFlag) {
    if (!inFlag) CSceneEnv::setStop((ObjType::eObjBit)0);
    else CSceneEnv::setStop((ObjType::eObjBit)0x7C);
}
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Game/Player/Player", exeWarp__7CPlayeri);
void CPlayer::setBanish(INT inFlag) {
    if (!inFlag) {
        setConditionFlag(0, 0x40);
        FVECTOR vColor;
        vColor.set(1.0f, 1.0f, 1.0f);
        CSceneEnv::setColor((ObjType::eObjBit)2, 30.0f, vColor, NULL, 2);
    } else {
        setConditionFlag(0x40, 0);
        m_tqBanish = 0;
        PL::g_vBanishPos = getPos();
    }
}
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Game/Player/Player", exeBanish__7CPlayer);
void CPlayer::setReflectGuard(INT inFlag) {
    if (!inFlag) {
        setConditionFlag(0, 0x200);
        m_collBody.m_GuardAttrFlag = 0;
        m_collBody.m_uiFlag = (INT)m_collBody.m_uiFlag & ~2;
    } else {
        setConditionFlag(0x200, 0);
        m_collBody.m_uiFlag |= 2;
        CCollBody& body = m_collBody;
        body.setGuardDir(1.5707964f);
        body.m_GuardAttrFlag |= 1;
    }
}
void CPlayer::setQuickBarrier(INT inFlag) {
    if (m_tqBarrier > 0) {
        m_collBody.m_uiFlag = (INT)m_collBody.m_uiFlag & ~2;
        m_tqBarrier = 0;
        m_collBody.m_GuardAttrFlag = 0;
    } else {
        m_tqBarrier = 6000;
        CCollBody& body = m_collBody;
        m_collBody.m_uiFlag |= 2;
        body.m_GuardAttrFlag |= 1;
        m_collBody.m_uiFlag |= 2;
        body.m_GuardAttrFlag |= 2;
        m_collBody.m_uiFlag |= 2;
        body.m_GuardAttrFlag |= 4;
        m_collBody.m_uiFlag |= 2;
        body.m_GuardAttrFlag |= 8;
        m_collBody.m_uiFlag |= 2;
        body.m_GuardAttrFlag |= 0x10;
        m_collBody.m_uiFlag |= 2;
        body.setGuardDir(3.1415927f);
        body.m_GuardAttrFlag |= 0x20;
    }
}
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
void CPlayer::exeAeroGuard() {
    if (isConditionFlag(0x80000000)) {
        CBullet* pBullet = (CBullet*)CTaskMan::getTask(m_hEffGuard);
        if (isConditionFlag(0x1000000)) {
            if (pBullet) {
                pBullet->out();
                m_hEffGuard = 0;
            }
        } else if (!pBullet) {
            setAeroGuard(1);
        } else {
            m_tqBarrier -= m_tqFrameRate;
            if (m_tqBarrier <= 0) {
                m_tqBarrier = 6000;
                pBullet->updateAttackID();
            }
            pBullet->warp(m_vCenterPos);
        }
    }
}
void CPlayer::setPrizeHealing(TIMEQ inTime) {
    if (!m_pInstance) return;
    if (!inTime) {
        if (m_hEffPrize > 0) {
            CEffect* effect = CEffectMan::getEffect(m_hEffPrize);
            if (effect) effect->loopEnd();
            m_hEffPrize = 0;
        }
        m_tqPrize = 0;
    } else {
        if (!m_tqPrize) {
            m_hEffPrize = attachEffect(PL::g_szDpxAlways, 0x16, &Crd(m_iBoneCenter), NULL, NULL, 0.0f);
            CEffect* effect = CEffectMan::getEffect(m_hEffPrize);
            if (effect) effect->m_iFlag |= 4;
        }
        m_tqPrize = inTime;
    }
}
void CPlayer::exePrizeHealing() {
    if (m_hEffPrize) {
        m_tqPrize -= m_tqFrameRate;
        if (m_tqPrize <= 0) {
            if (m_hEffPrize > 0) {
                CEffect* effect = CEffectMan::getEffect(m_hEffPrize);
                if (effect) effect->loopEnd();
                m_hEffPrize = 0;
            }
            m_tqPrize = 0;
        }
    }
}
void CPlayer::initGimmickCard(INT inNo) {
    TRACE(D_0049AEC0);
    setPlayerState(PL::GIMMICKCARD);
    m_iSubState = 1;
    clearSpeed();
    setPlayerFlag(1, 0);
    offAttack();
    setBattleFlag(0, 8);
    setBattleFlag(0, 0x200);
    setBattleFlag(0, 0x80000);
    setTrgFlag(0, 0x40);
    m_iAttackNo = inNo;
    m_uiPreAttackKind = 0;
    m_uiAttackKind = 0;
    m_bIsAttackHit = FALSE;
    m_bNextTrg = FALSE;
    m_iNextAttack = 0;
    m_bDmgMuteki = FALSE;
    m_bIsCanCombo = FALSE;
    m_fJumpGravity = *(FLOAT*)&PL::g_fGravity;
    m_JumpState = 3;
    m_fMoveSpdXZ = 0.0f;
    m_tqCardBreakTime = 0;
    m_fWalkSpeed = 0.0f;
}
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Game/Player/Player", exeGimmickCard__7CPlayer);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Game/Player/Player", setTrapDamage__7CPlayeriPCcUi);
void CPlayer::fadeEffect(INT& ioHandle, INT inLoop) {
    if (ioHandle > 0) {
        CEffect* effect = CEffectMan::getEffect(ioHandle);
        if (effect) {
            if (inLoop && effect->isLoop()) effect->loopEnd();
            else effect->fadeout(8.0f);
        }
        ioHandle = 0;
    }
}
void CPlayer::fadeEffAttack(INT inLoop) {
    INT i;
    for (i = 0; i < 4; i++) {
        if (m_hEffAttack[i] > 0) {
            CEffect* effect = CEffectMan::getEffect(m_hEffAttack[i]);
            if (effect) {
                if (inLoop && effect->isLoop()) effect->loopEnd();
                else effect->fadeout(8.0f);
            }
            m_hEffAttack[i] = 0;
        }
    }
}
void CPlayer::setEffAttack(INT inNum, const CHAR* inName, INT inNo, INT inBone, FVECTOR* inOfs) {
    FVECTOR pos;
    if (inOfs) {
        pos = *inOfs;
    } else {
        pos._q = 0;
        pos.w = 1.0f;
    }
    m_hEffAttack[inNum] = attachEffect(inName, inNo, &Crd(inBone), &pos, NULL, 0.0f);
    CEffect* effect = CEffectMan::getEffect(m_hEffAttack[inNum]);
    if (effect) effect->m_iFlag |= 4;
}
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Game/Player/Player", updateSpotLight__7CPlayeri);
void CPlayer::setQuake(FLOAT inCycle, FLOAT inPower, UINT inTime) {
    CCamera* cam = CWorldEnv::getActiveCamera();
    CameraQuakeParam cqp;
    cqp._fCycle = inCycle;
    cqp._nTime = inTime;
    cqp._fPower = inPower;
    new (com_malloc_d(sizeof(CCameraQuakeV), D_0049AFC0, RET_ADDR())) CCameraQuakeV(*cam, &cqp, inPower);
}
void CPlayer::notifyReturnCard(const CARD* inCard) {
    if (!m_pInstance) return;
    if (m_pRevolver) {
        if (!inCard) {
            m_pRevolver->resetCardOwner(2, 0);
            CBattleEnemyManager* pBEM = (CBattleEnemyManager*)CTaskMan::getSysTask(0x19);
            if (pBEM) pBEM->allGetPrize(2);
        } else if (inCard->m_nKind < 0xCB) {
            INT cat = inCard->m_nCategory0;
            if (cat && (cat < 7 || cat == 8)) m_pRevolver->addCard((CARD*)inCard, 1);
        }
    }
}
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Game/Player/Player", notifyGetPrize__7CPlayeriii);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Game/Player/Player", addEXP__7CPlayerii);
void CPlayer::addHP(INT inHP, TIMEQ inDispEFF, INT inDispREC) {
    if (_instance()) {
        if (inDispEFF > 0) _instance()->setPrizeHealing(inDispEFF);
        if (inDispREC > 0) Gauge::CCommonGauge::setDispSeq(_instance()->getHandle(), 0);
        _instance()->m_iHP += inHP;
        CPlayer* p = _instance();
        if (p->m_iHP > p->m_iMaxHP) p->m_iHP = p->m_iMaxHP;
        if (_instance()->m_pGauge) _instance()->m_pGauge->setHP(m_iHP, 0);
    } else {
        STATUS_INFO& st = CStatus::getStaus();
        INT hp = st.HP + inHP;
        if (hp > st.HPMax) hp = st.HPMax;
        st.HP = hp;
    }
}
void CPlayer::addMP(INT inMP) {
    if (_instance()) {
        _instance()->m_iMP += inMP;
        if (_instance()->m_iMP > 99999) _instance()->m_iMP = 99999;
    } else {
        INT mp = CStatus::getMP() + inMP;
        if (mp > 99999) mp = 99999;
        CStatus::setHP(mp);
    }
    Gauge::CPrizeGauge::setPrizeMp(inMP);
}
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Game/Player/Player", procDropMP__7CPlayer);
void CPlayer::notifyExterminate() {
    if (!_instance()) return;
    if (_instance()->m_bIsDead) {
        TRACE(D_0049B238);
        return;
    }
    if (_instance()->m_PlayerState == PL::ESCAPE) return;
    PLM::calcFavoriteStockCount();
    _instance()->setBattleFlag(4, 0);
    _instance()->setConditionFlag(0x2000, 0);
}
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
INT CPlayer::setMDFooting(HANDLE inFooting) {
    if (!inFooting) {
        setConditionFlag(0, 0x10000000);
        setTrgFlag(0, 0x400);
        m_fJumpLandLimit = -100.0f;
        m_hFooting = 0;
        m_pMove->setFlag(0x200, FALSE); // partial CMovement: (flag, on)
        return 0;
    }
    switch (m_PlayerState) {
    case PL::STAND:
    case PL::JUMP:
    case PL::FOOTING:
        break;
    default:
        return 0;
    }
    if (!isConditionFlag(0x10000000)) {
        setTrgFlag(0x400, 0);
        m_hFooting = inFooting;
        m_fJumpLandLimit = 2.0f;
        return 0;
    }
    m_pMove->setFlag(0x200, TRUE);
    return 1;
}
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Game/Player/Player", initFooting__7CPlayeri);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Game/Player/Player", exeFooting__7CPlayer);
void CPlayer::updateFooting() {
    if (isConditionFlag(0x10000000)) {
        CEntity* pEntity = m_hFooting > 0 ? (CEntity*)CTaskMan::getTask(m_hFooting) : NULL;
        if (pEntity) {
            FVECTOR pos = Crd(0).getWorldMatrix().v[3];
            pos.y = 2.0f;
            pEntity->warp(pos);
            FVECTOR rot = getRot();
            pEntity->setRot(rot);
            pEntity->updateCoord();
            pEntity->coordFix();
        }
    }
}
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
void CPlayer::exeWind() {
    if (dbgCheckUser(KKIKUCHI) && DebugWork::g_dw.player.iTemp == 5) {
        if (!m_pPLM) return;
        m_pPLM->addWindXYZ(&DebugWork::g_dw.player.vTemp);
    }
    if (m_pPLM && m_pPLM->isEnableWind() && m_PlayerState != PL::GLIDE) {
        FVECTOR vel = getVel();
        FVECTOR wind = m_pPLM->getWind();
        AddVectorXYZ(vel, vel, wind);
        setVel(vel);
    }
}
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
void CPlayer::cmdState_initWorldSelect(HANDLE inDoor) {
    m_hGimmick = inDoor;
    m_pMarker->holdTarget(8);
    initWorldSelect();
    new (com_malloc_d(sizeof(CWorldSelect), D_0049B308, RET_ADDR())) CWorldSelect(CTaskMan::getSysTask(1), m_hGimmick);
}
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
