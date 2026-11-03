#define GAME_PLAYER_SUMMON_CPP
#define GPRIM_H_PATH "System/Graphic/GPrim.h" /* MATCH: this unit names Gprim.h so (header default "System/Graphic/Gprim.h") */
#include "System/TaskMan.h"
#include "System/Task.h"
#include "Game/Player/Summon.h"
#include "System/Graphic/Gprim.h"
#include "System/Graphic/2d/2DMan.h"
#include "Game/CardBattle.h"
#include "System/Graphic/Str.h"
#include "Game/Collision/CollUtil.h"

/* MATCH: Placeholders: three empty strings follow CollUtil.h's strings in .rodata (as in FriendAladdin). */
inline void _emptyStringPlaceholder0() {
    TRACE("");
}
inline void _emptyStringPlaceholder1() { /* MATCH: placeholder: an inline function's strings, in the original .rodata order */
    TRACE("");
}
inline void _emptyStringPlaceholder2() { /* MATCH: placeholder: an inline function's strings, in the original .rodata order */
    TRACE("");
}

#include "System/Graphic/Effect/effect.h"
#include "Game/BodyCollision.h"
#include "Game/Player/Player.h"
#include "Game/Player/PlayerCamera.h"
#include "Game/Player/PlayerGauge.h"
#include "Game/Player/PlayerManager.h"
#include "Game/Movement.h"
#include "Game/LockOnManager.h"
#include "Game/EffectMotion.h"
#include "Game/Bullet/BulletManager.h"
#include "System/Math3D.h"
#include "System/Graphic/Camera.h"
#include "Game/Debug/DebugWork.h"
#include "System/Debug.h"
#include "include_asm.h"

void ApplyMatrixXYZ(FVECTOR& v0, const FMATRIX& m0, const FVECTOR& v1);
void AddVectorXYZ(FVECTOR& v0, const FVECTOR& v1, const FVECTOR& v2);
void SubVectorXYZ(FVECTOR& v0, const FVECTOR& v1, const FVECTOR& v2);
void ScaleVectorXYZ(FVECTOR& v0, const FVECTOR& v1, FLOAT s);
void Normalize(FVECTOR& v0, const FVECTOR& v1);

namespace Summon {

/* The original's flag updates fold ~inMask at the RTL level (li -3, not lui/ori 0xfffffffd): an inline function. */
static inline void _setFlag(UINT& ioFlag, UINT inSet, UINT inMask) {
    UINT flag = ioFlag;
    ioFlag = (flag | inSet) & ~inMask;
}
#define SET_FLAG(inSet, inMask) _setFlag(m_uiFlag, inSet, inMask)

void (CSummon::*CSummon::m_pBeginStateTbl[STATE_MAX])() = {
    &CSummon::beginStateAppear_,  &CSummon::beginStateMain_,    &CSummon::beginStateAction1_,
    &CSummon::beginStateAction2_, &CSummon::beginStateAction3_, &CSummon::beginStateExit_,
};
void (CSummon::*CSummon::m_pStateTbl[STATE_MAX])() = {
    &CSummon::stateAppear_,  &CSummon::stateMain_,    &CSummon::stateAction1_,
    &CSummon::stateAction2_, &CSummon::stateAction3_, &CSummon::stateExit_,
};
void (CSummon::*CSummon::m_pEndStateTbl[STATE_MAX])() = {
    &CSummon::endStateAppear_,  &CSummon::endStateMain_,    &CSummon::endStateAction1_,
    &CSummon::endStateAction2_, &CSummon::endStateAction3_, &CSummon::endStateExit_,
};

CSummon::CSummon(CTask* inParent, INT inID)
    : CEntity(inParent, ObjType::OBJ_TYPE_PLAYER), m_uiCharaID(inID), m_uiAnimID(0), m_pPlayer(NULL), m_uiFlag(0) {
    initalize();
}

CSummon::~CSummon() {
    if (PLM::CPlayerManager::getPLM()) PLM::CPlayerManager::getPLM()->notifySummon(this, 0);
    killChild(0.0f);
    resetLockon();
}

void CSummon::initFSM_() {
    for (INT i = 0; i < STATE_MAX; i++) {
        m_State[i].set(this, m_pBeginStateTbl[i], m_pStateTbl[i], m_pEndStateTbl[i]);
    }
    m_iState = -1;
}

static inline void _setNewState(CState*& p, CState& NewState) { p = &NewState; }
#line 126
void CSummon::changeState(INT inState) {
    if (!(0 <= inState && inState < STATE_MAX)) AssertApp(__FILE__, __LINE__, "0 <= inState && inState < STATE_MAX", 0);
    _setNewState(m_pNewState, m_State[inState]);
}

void CSummon::changeMotion(INT inIndex, TIMEQ inTq, BOOL inRootMove, TIMEQ inGr, INT inEffectNo, INT inSENo,
                           INT inActNo) {
    m_uiAnimID = inIndex;
    CEntity::changeMotion(inIndex, inTq, inRootMove, inGr, inEffectNo, inSENo);
    if (m_pAttack && m_pAttack->set(inIndex, inActNo)) m_pAttack->setAttackParam(m_iAttackPower, m_AtkAttr);
}

BOOL CSummon::initalize() {
    m_uiRCID = PLM::g_uiSummonRcid[m_uiCharaID];
    if (!CEntity::initalize(1, m_uiRCID)) return FALSE;
    initFSM_();
    PLM::CPlayerManager* plm = PLM::CPlayerManager::getPLM();
    plm->notifySummon(this, 1);
    m_pPlayer = plm->getPlayer();
    setEntityFlag(1, 0);
    FVECTOR scale = getScale();
    m_fScale = 0.0f;
    scale.x = m_fScale;
    scale.y = 0.5f;
    scale.z = m_fScale;
    setScale(scale);
    FVECTOR pos = m_pPlayer->getAppearPos();
    pos.y += 0.1f;
    m_vStartPos = pos;
    m_fDir = calcDirection(m_pPlayer);
    warp(pos);
    FVECTOR rot = getRot();
    rot.y = m_fDir;
    setRot(rot);
    m_pMove->land(5.0f, 1.0f, TRUE);
    m_fMoveSpdXZ = 0.0f;
    m_fCamHeight = 0.5f;
    updateCoord();
    coordFix();
    m_iSubState = 0;
    m_uiSkillID = 0;
    m_hTarget = 0;
    m_hLockon = 0;
    m_tqSubTimer = 0;
    m_vAimPos.init();
    m_iAttackNo = 0;
    m_iAtkBone = 0;
    m_iAttackPower = 0;
    m_AtkKind = (ATK_KIND)0;
    m_vAtkOffset.init();
    m_AtkAttr = ATK_ATTR_PHYSICAL;
    m_uiUpKind = 0;
    m_fRotDir = 0.0f;
    bodyCollOffBody(-1);
    setBodyRviseCoefficient(0.5f);
    m_pMove->setFlag(0x40, 0);
    m_pMove->setFootRadius(0.25f);
    m_pMove->setWallRadius(0.5f);
    return TRUE;
}

void CSummon::updateBefore() {
    if (m_pAttack) {
        if (m_pAttack->test()) {
            SET_FLAG(0x802, 0);
        } else {
            SET_FLAG(0, 2);
        }
    }
}

void CSummon::updateAfter() {
    SET_FLAG(0, 0x20);
    if (m_uiFlag & 0x10) return;
    if (!m_pBodyColl) return;
    FVECTOR result;
    if (m_pBodyColl->collideBody(&result, NULL, NULL)) {
        SET_FLAG(0x20, 0);
        if (!(m_uiFlag & 8)) {
            FVECTOR pos = getPos();
            setOldPos(pos);
            FVECTOR v;
            SubVectorXYZ(v, result, pos);
            v.y = 0.0f;
            FLOAT len = Math3D::VectorLength(v);
            Normalize(v, v);
            ScaleVectorXYZ(v, v, len * m_fBodyRviseCoefficient);
            AddVectorXYZ(pos, pos, v);
            setPos(pos);
        }
    }
}

void CSummon::lastUpdate() {
    if (m_pBodyColl) m_pBodyColl->update();
    if (m_pAttack) m_pAttack->update();
}

void CSummon::beginStateAppear() {
    m_uiStartAnimID = 1;
    setEntityFlag(0, 1);
    m_iState = STATE_APPEAR;
    if (m_uiCharaID == 1) {
        m_uiStartAnimID = 3;
        setTimer(4000);
    } else {
        setTimer(0);
    }
    changeMotion(m_uiStartAnimID, 0, TRUE, 0, 0, 0, 0);
    updateCoord();
    coordFix();
    setAppearEffect();
    bodyCollOnBody(-1);
}

void CSummon::stateAppear() {
    if (m_fScale < 1.0f) {
        m_fScale += getFrameRateF() * 0.05f;
        if (m_fScale > 1.0f) m_fScale = 1.0f;
        FVECTOR scale = getScale();
        scale.x = m_fScale;
        scale.y = m_fScale * 0.5f + 0.5f;
        scale.z = m_fScale;
        setScale(scale);
    } else if (isMotionEnd()) {
        changeMotion(1, 0, TRUE, -1, 0, 0, 0);
        changeState(STATE_MAIN);
    }
}

void CSummon::endStateAppear() {}

void CSummon::setAppear() {
    changeState(STATE_APPEAR);
    m_uiFlag |= 0x80;
}

void CSummon::beginStateMain() {
    m_iState = STATE_MAIN;
}

void CSummon::stateMain() {}

void CSummon::endStateMain() {}

void CSummon::beginStateExit() {
    m_iState = STATE_EXIT;
    m_tqTrgFrame = 0;
    switch (m_uiCharaID) {
    case 1:
        m_uiAnimID = 1;
        break;
    case 2:
        m_uiAnimID = 1;
        break;
    case 3:
        m_uiAnimID = 3;
        break;
    case 4:
        m_uiAnimID = 2;
        break;
    case 5:
        m_uiAnimID = 3;
        break;
    case 6:
        m_uiAnimID = 3;
        break;
    case 7:
        m_uiAnimID = 4;
        break;
    }
    changeMotion(m_uiAnimID, 0, TRUE, -1, 0, 0, 0);
    bodyCollOffBody(-1);
    UINT flag = m_uiFlag;
    m_iSubState = 0;
    m_tqSubTimer = (flag & 0x40) ? 0 : 3000;
    setTimer(m_tqTrgFrame);
    m_uiFlag = flag | 8;
    m_pMove->enableGravity(FALSE);
    clearSpeed();
}

void CSummon::stateExit() {
    if (m_iSubState > 0) {
        if (getTimer() <= 0) out();
    } else if (getTimer() <= 0) {
        if (m_fScale > 0.0f) {
            m_fScale -= getFrameRateF() * 0.05f;
            if (m_fScale <= 0.0f) {
                m_fScale = 0.0f;
                m_iSubState = 1;
                resetLockon();
                setTimer(m_tqSubTimer);
                killChild(m_tqSubTimer * 0.02f);
                setEntityFlag(1, 0);
                m_pPlayer->setPlayerFlag(0x08000000, 0);
                m_pMove->enableGravity(FALSE);
                clearSpeed();
            } else {
                FVECTOR scale = getScale();
                scale.x = m_fScale;
                scale.y = m_fScale * 0.5f + 0.5f;
                scale.z = m_fScale;
                setScale(scale);
            }
        } else {
            out();
        }
    }
}

void CSummon::endStateExit() {}

void CSummon::setExit(INT inFlag) {
    if (inFlag) m_uiFlag |= 0x40;
    if (m_iState == STATE_EXIT) {
        m_tqTrgFrame = 0;
        m_tqSubTimer = 0;
        setTimer(0);
    } else {
        changeState(STATE_EXIT);
        m_iSubState = 0;
    }
}

INT CSummon::setSkill(INT inID) {
    if (m_iState == STATE_MAIN) {
        setExit(FALSE);
        return TRUE;
    }
    return FALSE;
}

CEntity* CSummon::getTarget() {
    if (m_hTarget <= 0) return NULL;
    return (CEntity*)CTaskMan::getTask(m_hTarget);
}

static inline INT _getUpBit(INT inType) {
    return inType > 0 ? 1 << (inType - 1) : 0;
}
static inline INT _getUpKind(INT inType0, INT inType1) {
    INT kind = _getUpBit(inType0);
    if (inType1 > 0) kind |= 1 << (inType1 - 1);
    return kind;
}

INT CSummon::getPower(INT inSkillID, INT inKind) {
    INT result = 0;
    BC_PARAM* pBCP = NULL;
    SS_PARAM* pSSP = NULL;
    switch (inSkillID) {
    case 1: pBCP = &CardBattle::g_pBattleCardList[35]; break;
    case 2: pSSP = &CardBattle::g_pStockSkillList[52]; break;
    case 3: pSSP = &CardBattle::g_pStockSkillList[53]; break;
    case 4: pBCP = &CardBattle::g_pBattleCardList[36]; break;
    case 5: pSSP = &CardBattle::g_pStockSkillList[61]; break;
    case 6: pSSP = &CardBattle::g_pStockSkillList[62]; break;
    case 7: pBCP = &CardBattle::g_pBattleCardList[37]; break;
    case 8: pSSP = &CardBattle::g_pStockSkillList[56]; break;
    case 9: pSSP = &CardBattle::g_pStockSkillList[57]; break;
    case 10: pSSP = &CardBattle::g_pStockSkillList[58]; break;
    case 11: pBCP = &CardBattle::g_pBattleCardList[38]; break;
    case 12: pSSP = &CardBattle::g_pStockSkillList[54]; break;
    case 13: pSSP = &CardBattle::g_pStockSkillList[55]; break;
    case 14: pBCP = &CardBattle::g_pBattleCardList[39]; break;
    case 15: pSSP = &CardBattle::g_pStockSkillList[63]; break;
    case 16: pSSP = &CardBattle::g_pStockSkillList[64]; break;
    case 17: pBCP = &CardBattle::g_pBattleCardList[40]; break;
    case 18: pSSP = &CardBattle::g_pStockSkillList[59]; break;
    case 19: pSSP = &CardBattle::g_pStockSkillList[60]; break;
    case 20: pBCP = &CardBattle::g_pBattleCardList[41]; break;
    case 21: pSSP = &CardBattle::g_pStockSkillList[65]; break;
    case 22: pSSP = &CardBattle::g_pStockSkillList[67]; break;
    case 23: pSSP = &CardBattle::g_pStockSkillList[66]; break;
    }
    if (pBCP) {
        result = pBCP->POWER[inKind];
        m_uiUpKind = _getUpKind(pBCP->UP_TYPE0, pBCP->UP_TYPE1);
    } else if (pSSP) {
        result = pSSP->POWER[inKind];
        m_uiUpKind = _getUpKind(pSSP->UP_TYPE0, pSSP->UP_TYPE1);
    }
    return result;
}

static inline CEntity* _getEntity(HANDLE inHandle) {
    if (inHandle > 0) return (CEntity*)CTaskMan::getTask(inHandle);
    return NULL;
}

void CSummon::homingPlayerTargetY(HANDLE inTarget, FLOAT inRate) {
    if (inTarget <= 0) {
        m_hTarget = m_pPlayer->isNowMark();
    } else {
        m_hTarget = inTarget;
    }
    CEntity* pTarget = _getEntity(m_hTarget);
    if (!pTarget) pTarget = _getEntity(m_hTarget = LockOnManager::CLockOnManager::getLockOnObject(*this));
    if (pTarget) {
        FVECTOR pos;
        pTarget->getLockOnPos(pos);
        m_pMove->homingY(pos, inRate);
    } else {
        FVECTOR rot = getRot();
        m_fDir = Movement::Homing(rot.y, m_pPlayer->getDir(), inRate, getFrameRateF());
    }
}

void CSummon::homingPlayerTargetXZ(HANDLE inTarget, FLOAT inRate) {
    if (inTarget <= 0) {
        m_hTarget = m_pPlayer->isNowMark();
    } else {
        m_hTarget = inTarget;
    }
    CEntity* pTarget = _getEntity(m_hTarget);
    if (!pTarget) pTarget = _getEntity(m_hTarget = LockOnManager::CLockOnManager::getLockOnObject(*this));
    if (pTarget) {
        FVECTOR pos;
        pTarget->getLockOnPos(pos);
        m_pMove->homingMoveXZ(pos, inRate, 0.02f);
    }
}

void CSummon::turnPlayerTargetY(HANDLE inTarget, FLOAT inRate) {
    if (inTarget <= 0) {
        m_hTarget = m_pPlayer->isNowMark();
    } else {
        m_hTarget = inTarget;
    }
    CEntity* pTarget = _getEntity(m_hTarget);
    if (!pTarget) pTarget = _getEntity(m_hTarget = LockOnManager::CLockOnManager::getLockOnObject(*this));
    if (pTarget) {
        FVECTOR pos;
        pTarget->getLockOnPos(pos);
        m_pMove->turnToTargetY(pos, 0.2617994f);
    }
}

void CSummon::getWeaponTopPos(FVECTOR& inPos) {
    FMATRIX m = Crd(m_iAtkBone).getWorldMatrix();
    ApplyMatrixXYZ(inPos, m, m_vAtkOffset);
    inPos.w = 1.0f;
}

void CSummon::changeCamera(HANDLE inTarget, INT inMode, INT inReset) {
    m_pPlayer->getPlayerCamera()->changeNowPlayer(getHandle(), inTarget);
    m_pPlayer->getPlayerCamera()->changeCamMode(inMode, inReset);
}

void CSummon::changeCameraSummon(HANDLE sumTarget, INT inMode, INT inReset) {
    m_pPlayer->getPlayerCamera()->changeNowPlayer(sumTarget, m_pPlayer->getHandle());
    m_pPlayer->getPlayerCamera()->changeCamMode(inMode, inReset);
}

void CSummon::setLockon(HANDLE inHandle) {
    m_hLockon = inHandle;
    Gauge::CLockonMarker* pMarker = m_pPlayer->getLockonMarker();
    if (inHandle > 0) {
        m_pPlayer->setPlayerFlag(0x10000, 0);
        pMarker->changeMode();
        pMarker->setLockon(m_hLockon, 0);
        pMarker->holdTarget(8);
    } else {
        m_pPlayer->setPlayerFlag(0, 0x10000);
        pMarker->changeMode(0);
        pMarker->setTarget(m_hTarget);
        pMarker->holdTarget(0);
    }
}

void CSummon::resetLockon() {
    if (g_pPlayer && (m_uiFlag & 0x80)) {
        SET_FLAG(0, 0x80);
        m_pPlayer->setPlayerFlag(0, 0x10000);
        Gauge::CLockonMarker* pMarker = m_pPlayer->getLockonMarker();
        if (pMarker) {
            pMarker->offPush();
            pMarker->changeMode(0);
            pMarker->setTarget(m_hTarget);
            pMarker->holdTarget(0);
        }
        if (m_pPlayer->getPlayerCamera()) {
            PlayerCamera::CPlayerCamera::setBattleCamData(0, NULL);
            m_pPlayer->getPlayerCamera()->changeNowPlayer(0, 0);
            m_pPlayer->getPlayerCamera()->changeCamMode(-1, 0);
        }
    }
}

void CSummon::killChild(FLOAT inFadeTime) {
    CEffectMan::killAll(ObjType::OBJ_BIT_FRIEND, inFadeTime);
    m_pEffect->kill(inFadeTime);
    m_pSE->stop();
    killLoopEffect(0);
    CBulletManager* pBM = (CBulletManager*)CTaskMan::getSysTask(0x1b);
    if (pBM) pBM->kill(getHandle());
}

void CSummon::setPushon(INT inFlag) {
    Gauge::CLockonMarker* pMarker = m_pPlayer->getLockonMarker();
    if (inFlag) {
        pMarker->onPush(inFlag);
    } else {
        pMarker->offPush();
    }
}

INT CSummon::entryChainTrg(INT inMode) {
    if (m_hTarget < 0) return FALSE;
    UINT flag = m_uiFlag;
    if (!(flag & 0x800)) return FALSE;
    if (flag & 0x400) return FALSE;
    if (flag & 0x40000) return FALSE;
    setPushon(inMode);
    SET_FLAG(0x400, 0x1000);
    return TRUE;
}

INT CSummon::checkChainTrg() {
    UINT flag = m_uiFlag;
    if (flag & 0x40000) return FALSE;
    if (flag & 0x400) {
        if (flag & 0x1000) return FALSE;
        if (isButtonTrg(0x10, 0)) {
            m_uiFlag |= 0x1000;
            return TRUE;
        }
    } else {
        if (isButtonTrg(0x10, 0)) {
            if (DebugWork::g_dw.player.iRenda) return FALSE;
            m_uiFlag |= 0x40000;
        }
    }
    return FALSE;
}

INT CSummon::isSucceedChain() {
    UINT flag = m_uiFlag;
    if (flag & 0x1000) {
        if (!(flag & 0x2000)) {
            m_uiFlag = flag | 0x2000;
            setPushon(0);
        }
        return TRUE;
    }
    return FALSE;
}

void CSummon::beginStateAppear_() {
    beginStateAppear();
}
void CSummon::stateAppear_() {
    stateAppear();
}
void CSummon::endStateAppear_() {
    endStateAppear();
}
void CSummon::beginStateMain_() {
    beginStateMain();
}
void CSummon::stateMain_() {
    stateMain();
}
void CSummon::endStateMain_() {
    endStateMain();
}
void CSummon::beginStateAction1_() {
    beginStateAction1();
}
void CSummon::stateAction1_() {
    stateAction1();
}
void CSummon::endStateAction1_() {
    endStateAction1();
}
void CSummon::beginStateAction2_() {
    beginStateAction2();
}
void CSummon::stateAction2_() {
    stateAction2();
}
void CSummon::endStateAction2_() {
    endStateAction2();
}
void CSummon::beginStateAction3_() {
    beginStateAction3();
}
void CSummon::stateAction3_() {
    stateAction3();
}
void CSummon::endStateAction3_() {
    endStateAction3();
}
void CSummon::beginStateExit_() {
    beginStateExit();
}
void CSummon::stateExit_() {
    stateExit();
}
void CSummon::endStateExit_() {
    endStateExit();
}

void CSummon::getCameraTargetPos(FVECTOR& outPos) {
    outPos = Crd(0).getWorldMatrix().v[3];
    outPos.y += m_fCamHeight;
    if (dbgCheckUser(KKIKUCHI) && DebugWork::g_dw.player.iTemp == 6) {
        outPos.y += DebugWork::g_dw.player.vTemp.y;
    }
}

} // namespace Summon
// SOURCE DATA: .data .rodata
