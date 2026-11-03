#define GAME_PLAYER_FRIEND_CPP
#define GPRIM_H_PATH "System/Graphic/GPrim.h" /* MATCH: this unit names Gprim.h so (header default "System/Graphic/Gprim.h") */
#include "System/TaskMan.h"
#include "System/Task.h"
#include "Game/Player/Friend.h"
#include "System/Graphic/Gprim.h"
#include "System/Graphic/2d/2DMan.h"
#include "Game/CardBattle.h"
#include "System/Graphic/Str.h"
#include "Game/Collision/CollUtil.h"

/* MATCH: Placeholders: three empty strings follow CollUtil.h's strings in .rodata (as in Summon). */
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
#include "Game/AttackMotion.h"
#include "Game/Player/Player.h"
#include "Game/Player/PlayerCamera.h"
#include "Game/Player/PlayerGauge.h"
#include "Game/Player/PlayerManager.h"
#include "Game/Player/PlayerAttack.h"
#include "Game/Bullet/Bullet.h"
#include "Game/EntityFactory.h"
#include "Game/Movement.h"
#include "Game/LockOnManager.h"
#include "Game/EffectMotion.h"
#include "Game/Bullet/BulletManager.h"
#include "System/Math3D.h"
#include "System/Graphic/Camera.h"
#include "Game/Debug/DebugWork.h"
#include "System/Debug.h"
#include "include_asm.h"

extern "C" {
void sceVu0UnitMatrix(FMATRIX& m);
void sceVu0RotMatrixY(FMATRIX& m0, FMATRIX& m1, float ry);
}
/* Through inline wrappers (as in FriendKing): the original recomputes the matrix address for each call. */
static inline void _unitMatrix(FMATRIX& m) {
    sceVu0UnitMatrix(m);
}
static inline void _rotMatrixY(FMATRIX& m, FLOAT r) {
    sceVu0RotMatrixY(m, m, r);
}
void ApplyMatrix(FVECTOR& v0, const FMATRIX& m0, const FVECTOR& v1);
void ApplyMatrixXYZ(FVECTOR& v0, const FMATRIX& m0, const FVECTOR& v1);
void AddVectorXYZ(FVECTOR& v0, const FVECTOR& v1, const FVECTOR& v2);
void SubVectorXYZ(FVECTOR& v0, const FVECTOR& v1, const FVECTOR& v2);
void ScaleVectorXYZ(FVECTOR& v0, const FVECTOR& v1, FLOAT s);
void Normalize(FVECTOR& v0, const FVECTOR& v1);

namespace PL {
extern BULLET_EFFECT_INFO g_EffTbl_Skill[67][4];
extern FVECTOR g_RotTbl_Magic[8];
}

namespace Friend {

#define SET_FLAG(inSet, inMask) _setFlag(m_uiFlag, inSet, inMask)

void (CFriend::*CFriend::m_pBeginStateTbl[STATE_MAX])() = {
    &CFriend::beginStateAppear_,  &CFriend::beginStateMain_,    &CFriend::beginStateAction1_,
    &CFriend::beginStateAction2_, &CFriend::beginStateAction3_, &CFriend::beginStateExit_,
};
void (CFriend::*CFriend::m_pStateTbl[STATE_MAX])() = {
    &CFriend::stateAppear_,  &CFriend::stateMain_,    &CFriend::stateAction1_,
    &CFriend::stateAction2_, &CFriend::stateAction3_, &CFriend::stateExit_,
};
void (CFriend::*CFriend::m_pEndStateTbl[STATE_MAX])() = {
    &CFriend::endStateAppear_,  &CFriend::endStateMain_,    &CFriend::endStateAction1_,
    &CFriend::endStateAction2_, &CFriend::endStateAction3_, &CFriend::endStateExit_,
};

CFriend::CFriend(CTask* inParent, INT inID, INT inEx)
    : CEntity(inParent, ObjType::OBJ_TYPE_FRIEND), m_uiCharaID(inID), m_uiAnimID(0), m_pPlayer(NULL), m_uiFlag(0) {
    if (inEx) m_uiFlag |= 0x1000000;
    initalize();
}

CFriend::~CFriend() {
    if (PLM::CPlayerManager::getPLM()) {
        PLM::CPlayerManager::getPLM()->notifyFriend(this, 0, m_uiFlag & 0x1000000);
    }
    killChild(0.0f);
    resetLockon();
}

void CFriend::initFSM_() {
    for (INT i = 0; i < STATE_MAX; i++) {
        m_State[i].set(this, m_pBeginStateTbl[i], m_pStateTbl[i], m_pEndStateTbl[i]);
    }
    m_iState = -1;
}

static inline void _setNewState(CState*& p, CState& NewState) { p = &NewState; }
#line 143
void CFriend::changeState(INT inState) {
    if (!(0 <= inState && inState < STATE_MAX)) AssertApp(__FILE__, __LINE__, "0 <= inState && inState < STATE_MAX", 0);
    _setNewState(m_pNewState, m_State[inState]);
}

void CFriend::changeMotion(INT inIndex, TIMEQ inTq, BOOL inRootMove, TIMEQ inGr, INT inEffectNo, INT inSENo,
                           INT inActNo) {
    m_uiAnimID = inIndex;
    CEntity::changeMotion(inIndex, inTq, inRootMove, inGr, inEffectNo, inSENo);
    if (m_pAttack && m_pAttack->set(inIndex, inActNo)) m_pAttack->setAttackParam(m_iAttackPower, (ATK_ATTR)m_AtkAttr);
}

BOOL CFriend::initalize() {
    PLM::CPlayerManager* pPLM = PLM::CPlayerManager::getPLM();
    if (pPLM->isHalloweenTown()) {
        switch (m_uiCharaID) {
        case 1:
            m_uiCharaID = 11;
            break;
        case 2:
            m_uiCharaID = 12;
            break;
        case 8:
            m_uiCharaID = 13;
            break;
        }
    }
    m_uiRCID = PLM::g_uiFriendRcid[m_uiCharaID];
    if (!CEntity::initalize(1, m_uiRCID)) return FALSE;
    initFSM_();
    pPLM->notifyFriend(this, 1, m_uiFlag & 0x1000000);
    m_pPlayer = g_pPlayer;
    setEntityFlag(1, 0);
    FVECTOR scale = getScale();
    m_fScale = 0.0f;
    scale.x = m_fScale;
    scale.y = 0.5f;
    scale.z = m_fScale;
    setScale(scale);
    m_vStartPos = m_pPlayer->getAppearPos();
    m_fDir = calcDirection(m_pPlayer);
    warp(m_vStartPos);
    FVECTOR rot = getRot();
    rot.y = m_fDir;
    setRot(rot);
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
    m_vAtkOffset.init();
    m_fRotDir = 0.0f;
    bodyCollOffBody(-1);
    m_uiCollAttackID = 0;
    setBodyRviseCoefficient(0.5f);
    m_pMove->setFlag(0x40, 0);
    m_pMove->setFootRadius(0.25f);
    m_pMove->setWallRadius(0.5f);
    return TRUE;
}

void CFriend::updateBefore() {
    if (m_pAttack) {
        if (m_pAttack->test()) {
            SET_FLAG(0x802, 0);
        } else {
            SET_FLAG(0, 2);
        }
    }
}

void CFriend::updateAfter() {
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

void CFriend::lastUpdate() {
    if (m_pBodyColl) m_pBodyColl->update();
    if (m_pAttack) m_pAttack->update();
}

void CFriend::beginStateAppear() {
    m_iState = STATE_APPEAR;
    setEntityFlag(0, 1);
    m_uiStartAnimID = 1;
    m_uiGroundAnimID = 0;
    switch (m_uiCharaID) {
    case 2:
        m_uiStartAnimID = 3;
        m_uiGroundAnimID = 3;
        break;
    case 3:
        m_uiGroundAnimID = 1;
        break;
    case 7:
        m_uiStartAnimID = 1;
        m_uiGroundAnimID = 1;
        m_pMove->enableGravity(TRUE);
        clearSpeed();
        break;
    }
    changeMotion(m_uiStartAnimID, 0, TRUE, -1, 0, 0, 0);
    updateCoord();
    coordFix();
    setAppearEffect();
    bodyCollOnBody(-1);
}

void CFriend::stateAppear() {
    m_pMove->homingY(m_pPlayer, 0.1f);
    if (m_fScale < 1.0f) {
        m_fScale += getFrameRateF() * 0.05f;
        if (m_fScale > 1.0f) m_fScale = 1.0f;
        FVECTOR scale = getScale();
        scale.x = m_fScale;
        scale.y = m_fScale * 0.5f + 0.5f;
        scale.z = m_fScale;
        setScale(scale);
    } else if (m_uiAnimID == 1 || isMotionEnd()) {
        changeMotion(1, 0, TRUE, -1, 0, 0, 0);
        changeState(STATE_MAIN);
    }
}

void CFriend::endStateAppear() {}

void CFriend::setAppear() {
    changeState(STATE_APPEAR);
    m_uiFlag |= 0x80;
}

void CFriend::beginStateMain() {
    m_iState = STATE_MAIN;
}

void CFriend::stateMain() {}

void CFriend::endStateMain() {}

void CFriend::beginStateExit() {
    m_iState = STATE_EXIT;
    changeMotion(1, 0, TRUE, -1, 0, 0, 0);
    setDisappearEffect();
    bodyCollOffBody(-1);
    UINT flag = m_uiFlag;
    m_iSubState = 0;
    m_tqSubTimer = (flag & 0x40) ? 0 : 1000;
    m_uiFlag = flag | 8;
    m_pMove->enableGravity(FALSE);
    clearSpeed();
}

void CFriend::stateExit() {
    if (m_iSubState > 0) {
        if (getTimer() <= 0) out();
    } else if (m_fScale > 0.0f) {
        m_fScale -= getFrameRateF() * 0.05f;
        if (m_fScale <= 0.0f) {
            m_fScale = 0.0f;
            m_iSubState = 1;
            resetLockon();
            setTimer(m_tqSubTimer);
            killChild(m_tqSubTimer * 0.02f);
            setEntityFlag(1, 0);
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

void CFriend::endStateExit() {}

void CFriend::setExit(INT inFlag) {
    if (inFlag) m_uiFlag |= 0x40;
    if (m_iState == STATE_EXIT) {
        m_tqTrgFrame = 0;
        m_tqSubTimer = 0;
        setTimer(m_tqTrgFrame);
    } else {
        changeState(STATE_EXIT);
        m_iSubState = 0;
    }
}

INT CFriend::setSkill(INT inID) {
    if (m_iState == STATE_MAIN) {
        setExit(FALSE);
        return TRUE;
    }
    return FALSE;
}

CEntity* CFriend::getTarget() {
    if (m_hTarget <= 0) return NULL;
    return (CEntity*)CTaskMan::getTask(m_hTarget);
}

INT CFriend::getPower(INT inSkillID, INT inKind) {
    INT result = 0;
    switch (inSkillID) {
    case 1: result = CardBattle::g_pBattleCardList[49].POWER[inKind]; break;
    case 2: result = CardBattle::g_pStockSkillList[68].POWER[inKind]; break;
    case 3: result = CardBattle::g_pStockSkillList[69].POWER[inKind]; break;
    case 4: result = CardBattle::g_pStockSkillList[70].POWER[inKind]; break;
    case 9: result = CardBattle::g_pStockSkillList[74].POWER[inKind]; break;
    case 5: result = CardBattle::g_pBattleCardList[50].POWER[inKind]; break;
    case 6: result = CardBattle::g_pStockSkillList[71].POWER[inKind]; break;
    case 7: result = CardBattle::g_pStockSkillList[72].POWER[inKind]; break;
    case 8: result = CardBattle::g_pStockSkillList[73].POWER[inKind]; break;
    case 10: result = CardBattle::g_pBattleCardList[51].POWER[inKind]; break;
    case 11: result = CardBattle::g_pStockSkillList[75].POWER[inKind]; break;
    case 12: result = CardBattle::g_pStockSkillList[76].POWER[inKind]; break;
    case 13: result = CardBattle::g_pBattleCardList[53].POWER[inKind]; break;
    case 14: result = CardBattle::g_pStockSkillList[77].POWER[inKind]; break;
    case 15: result = CardBattle::g_pStockSkillList[78].POWER[inKind]; break;
    case 16: result = CardBattle::g_pBattleCardList[52].POWER[inKind]; break;
    case 17: result = CardBattle::g_pStockSkillList[79].POWER[inKind]; break;
    case 18: result = CardBattle::g_pStockSkillList[80].POWER[inKind]; break;
    case 19: result = CardBattle::g_pBattleCardList[54].POWER[inKind]; break;
    case 20: result = CardBattle::g_pStockSkillList[81].POWER[inKind]; break;
    case 21: result = CardBattle::g_pStockSkillList[82].POWER[inKind]; break;
    case 22: result = CardBattle::g_pBattleCardList[55].POWER[inKind]; break;
    case 23: result = CardBattle::g_pStockSkillList[83].POWER[inKind]; break;
    case 24: result = CardBattle::g_pStockSkillList[84].POWER[inKind]; break;
    case 25: result = CardBattle::g_pBattleCardList[56].POWER[inKind]; break;
    case 26: result = CardBattle::g_pStockSkillList[98].POWER[inKind]; break;
    case 27: result = CardBattle::g_pStockSkillList[99].POWER[inKind]; break;
    case 28: result = CardBattle::g_pStockSkillList[100].POWER[inKind]; break;
    case 29: result = CardBattle::g_pBattleCardList[57].POWER[inKind]; break;
    case 30: result = CardBattle::g_pStockSkillList[85].POWER[inKind]; break;
    case 31: result = CardBattle::g_pStockSkillList[86].POWER[inKind]; break;
    }
    return result;
}

static inline CEntity* _getEntity(HANDLE inHandle) {
    if (inHandle > 0) return (CEntity*)CTaskMan::getTask(inHandle);
    return NULL;
}

void CFriend::homingPlayerTargetY(HANDLE inTarget, FLOAT inRate) {
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

void CFriend::getWeaponTopPos(FVECTOR& inPos) {
    FMATRIX m = Crd(m_iAtkBone).getWorldMatrix();
    ApplyMatrixXYZ(inPos, m, m_vAtkOffset);
    inPos.w = 1.0f;
}

/* setMagic's jump table (the function is still assembly). */
asm(".rdata\n.align 4\njtbl_00494750:\n"
    ".word .L00254A80\n.word .L00254C3C\n.word .L002555AC\n.word .L002557DC\n.word .L0025581C\n.word .L00255D88\n"
    ".word .L00255D88\n.word .L00254A80\n.word .L00254A80\n.word .L00254C3C\n.word .L00254C3C\n.word .L00255D88\n"
    ".word .L002555AC\n.word .L002555AC\n.word .L00255D88\n.word .L00255D88\n.word .L00255D88\n.word .L00255D88\n"
    ".word .L00255D88\n.word .L00255D88\n.word .L002557DC\n.word .L002557DC\n.word .L0025581C\n.word .L0025581C\n"
    ".word .L00255D88\n.word .L00255D88\n.word .L00255D88\n.word .L00255D88\n.word .L00255D88\n.word .L00255D88\n"
    ".word .L00255D88\n.word .L00255D88\n.word .L00255D88\n.word .L00255D88\n.word .L00255D88\n.word .L00255D88\n"
    ".word .L00255D88\n.word .L00255D88\n.word .L00255D88\n.word .L00255D88\n.word .L00255D88\n.word .L00255D88\n"
    ".word .L00255D88\n.word .L00255D88\n.word .L00255D88\n.word .L00255D88\n.word .L00255D88\n.word .L00255D88\n"
    ".word .L00255D88\n.word .L00255D88\n.word .L00255D88\n.word .L00255D88\n.word .L00255D88\n.word .L00255D88\n"
    ".word .L00255D88\n.word .L00255D88\n.word .L00255D88\n.word .L00255D88\n.word .L00255AB0\n.word .L00255D88\n"
    ".word .L00255D88\n.word .L00255D88\n.word .L00255D88\n.word .L00255D88\n.word .L00255D88\n.word .L00255D88\n"
    ".word .L00255D88\n.word .L00255D88\n.word .L00255D88\n.word .L00255D88\n.word .L00255D88\n.word .L00255D88\n"
    ".word .L00255D88\n.word .L00255D88\n.word .L00255D88\n.word .L00255D88\n.word .L00255D88\n.word .L00255D88\n"
    ".word .L00255D88\n.word .L00255D88\n.word .L00255D88\n.word .L00255D88\n.word .L00255AB0\n.word .L00255AB0\n"
    ".word .L00255B6C\n.word .L00255B6C\n"
    ".text");
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Game/Player/Friend", setMagic__Q26Friend7CFriendi);

/* Not matching (7 lines): in case 36/46/47 (thunder), sched2 issues the copy of getRot() (lq) before the load of
 * pAtkData->EFFECT (lbu) for setEffect's arguments; the original loads EFFECT first. Everything else matches. */
#if 0
static inline FLOAT _limitPI(FLOAT r) {
    return r < -3.1415927f ? r + 6.2831855f : (r > 3.1415927f ? r - 6.2831855f : r);
}

static inline INT _getUpBit(INT inType) {
    return inType > 0 ? 1 << (inType - 1) : 0;
}
static inline INT _getUpKind(INT inType0, INT inType1) {
    INT kind = _getUpBit(inType0);
    if (inType1 > 0) kind |= 1 << (inType1 - 1);
    return kind;
}

static inline void _enchant(BULLET_PARAM& ioParam) {
    UINT kind = _getUpKind(CardBattle::g_pBattleCardList[53].UP_TYPE0, CardBattle::g_pBattleCardList[53].UP_TYPE1);
    CardBattle::CCardBattle::calcEnchant_AttackPoint(ioParam.iAttackPoint, (ATK_KIND&)ioParam.AtkKind,
                                                     (ATK_ATTR)ioParam.AtkAttr, kind);
}

void CFriend::setMagic(INT inID) {
    FVECTOR pos;
    FVECTOR rot;
    BULLET_PARAM param;
    PATK_DATA* pAtkData = &PL::g_AttackDataList[m_uiAttackID];
    CBullet* bullet;
    m_hTarget = m_pPlayer->isNowMark();
    CEntity* pTarget = _getEntity(m_hTarget);
    if (!pTarget || !pTarget->isPermissionLockOn()) {
        pTarget = _getEntity(m_hTarget = LockOnManager::CLockOnManager::getLockOnObject(*this));
    }
    switch (inID) {
    case 34:
    case 41:
    case 42:
        getWeaponTopPos(pos);
        param.init(2, 3);
        param.CollKind = 0x80000000;
        param.uiFlag = 0x8100;
        param.AtkKind = pAtkData->DAMAGE;
        param.AtkAttr = m_AtkAttr;
        param.iAttackPoint = getPower(m_uiSkillID, m_uiKind) / 10;
        if (m_uiCharaID == 5) {
            _enchant(param);
        }
        param.fSpeed = pAtkData->MAX_SPEED * 0.01f;
        param.fMaxDist = pAtkData->WORK0;
        param.tqHomingTime = 9000;
        param.hHomingTarget = m_hTarget;
        param.fHomingRadius = pAtkData->RADIUS;
        param.fHomingRate = pAtkData->WORK1;
        param.vPos = pos;
        param.vRot = getRot();
        param.fCollRadius = pAtkData->COLL_START_FRAME * 0.01f;
        param.pEffectTbl = PL::g_EffTbl_Skill[pAtkData->EFFECT];
        bullet = (CBullet*)CEntityFactory::getInstance()->createBullet(param, this);
        if (bullet) bullet->setBodyRadius(0.1f, 0.0f, 0, (COLL_KIND)-1);
        break;
    case 35:
    case 43:
    case 44:
        getWeaponTopPos(pos);
        rot = getRot();
        param.init(2, 3);
        param.uiFlag = 0x4000000;
        param.CollKind = 0x80000000;
        param.AtkKind = pAtkData->DAMAGE;
        param.AtkAttr = m_AtkAttr;
        param.iAttackPoint = getPower(m_uiSkillID, m_uiKind) / 10;
        if (m_uiCharaID == 5) {
            _enchant(param);
        }
        param.vPos = pos;
        param.fSpeed = pAtkData->MAX_SPEED * 0.01f;
        param.fMaxDist = pAtkData->WORK0;
        param.pEffectTbl = PL::g_EffTbl_Skill[pAtkData->EFFECT];
        param.fCollRadius = pAtkData->COLL_START_FRAME * 0.01f;
        param.vRot = rot;
        param.tqExplodeTime = pAtkData->WORK2 * 100;
        param.vPos.w = pAtkData->COLL_END_FRAME * 0.01f;
        CEntityFactory::getInstance()->createBullet(param, this);
        switch (m_uiLevel) {
        case 0:
            param.pEffectTbl = PL::g_EffTbl_Skill[pAtkData->EFFECT + 1];
            param.vRot.y = _limitPI(rot.y - 0.15707964f);
            CEntityFactory::getInstance()->createBullet(param, this);
            param.vRot.y = _limitPI(rot.y + 0.15707964f);
            CEntityFactory::getInstance()->createBullet(param, this);
            break;
        case 1:
            param.pEffectTbl = PL::g_EffTbl_Skill[pAtkData->EFFECT + 1];
            param.vRot.y = _limitPI(rot.y - 0.18849556f);
            CEntityFactory::getInstance()->createBullet(param, this);
            param.vRot.y = _limitPI(rot.y + 0.18849556f);
            CEntityFactory::getInstance()->createBullet(param, this);
            param.vRot.x = _limitPI(rot.x - 0.15707964f);
            param.vRot.y = _limitPI(rot.y - 0.09424778f);
            CEntityFactory::getInstance()->createBullet(param, this);
            param.vRot.y = _limitPI(rot.y + 0.09424778f);
            CEntityFactory::getInstance()->createBullet(param, this);
            break;
        case 2:
            param.pEffectTbl = PL::g_EffTbl_Skill[pAtkData->EFFECT + 1];
            param.vRot.y = _limitPI(rot.y - 0.15707964f);
            CEntityFactory::getInstance()->createBullet(param, this);
            param.vRot.y = _limitPI(rot.y + 0.15707964f);
            CEntityFactory::getInstance()->createBullet(param, this);
            param.vRot.y = _limitPI(rot.y - 0.28274336f);
            CEntityFactory::getInstance()->createBullet(param, this);
            param.vRot.y = _limitPI(rot.y + 0.28274336f);
            CEntityFactory::getInstance()->createBullet(param, this);
            param.vRot.x = _limitPI(rot.x - 0.15707964f);
            param.vRot.y = _limitPI(rot.y - 0.15707964f);
            CEntityFactory::getInstance()->createBullet(param, this);
            param.vRot.y = _limitPI(rot.y + 0.15707964f);
            CEntityFactory::getInstance()->createBullet(param, this);
            break;
        }
        break;
    case 36:
    case 46:
    case 47:
        if (pTarget) {
            pTarget->getLockOnPos(pos);
            pos.y = getPos().y;
        } else {
            pos = getPos();
        }
        rot = getRot();
        setEffect(PL::g_EffTbl_Skill[pAtkData->EFFECT][1].pEffectName, pAtkData->EFFECT - 10, &pos, &rot, 0.0f);
        pos.y += 6.0f;
        param.init(1, 3);
        param.CollKind = 0x80000000;
        param.uiFlag = 0x47;
        param.AtkKind = pAtkData->DAMAGE;
        param.AtkAttr = m_AtkAttr;
        param.iAttackPoint = getPower(m_uiSkillID, m_uiKind) / 10;
        if (m_uiCharaID == 5) {
            _enchant(param);
        }
        rot.x = 1.5707964f;
        param.fSpeed = pAtkData->MAX_SPEED * 0.01f;
        param.fMaxDist = pAtkData->WORK0;
        param.tqHomingTime = 800;
        param.vPos = pos;
        param.vRot = rot;
        param.pEffectTbl = PL::g_EffTbl_Skill[pAtkData->EFFECT];
        param.fCollRadius = pAtkData->COLL_START_FRAME * 0.1f;
        bullet = (CBullet*)CEntityFactory::getInstance()->createBullet(param, this);
        if (bullet) bullet->setBodyRadius(0.1f, 0.0f, 0, (COLL_KIND)-1);
        break;
    case 37:
    case 54:
    case 55:
        param.iAttackPoint = getPower(m_uiSkillID, m_uiKind) / 10;
        m_pPlayer->setCure(pAtkData->EFFECT, param.iAttackPoint);
        break;
    case 38:
    case 56:
    case 57:
        if (pTarget) {
            pTarget->getLockOnPos(pos);
            pos.y = getPos().y;
        } else {
            pos = getPos();
        }
        pos.y += 3.0f;
        rot = getRot();
        if (pAtkData->EFFECT == 18) {
            setEffect(PL::g_EffTbl_Skill[pAtkData->EFFECT][2].pEffectName, 2, &pos, &rot, 0.0f);
        } else {
            setEffect(PL::g_EffTbl_Skill[pAtkData->EFFECT][2].pEffectName, pAtkData->EFFECT - 11, &pos, &rot, 0.0f);
        }
        param.init(2, 3);
        param.CollKind = 0x80000000;
        param.uiFlag = 0x41E1;
        param.AtkKind = pAtkData->DAMAGE;
        param.AtkAttr = m_AtkAttr;
        param.iAttackPoint = getPower(m_uiSkillID, m_uiKind);
        if (m_uiCharaID == 5) {
            _enchant(param);
        }
        param.fSpeed = pAtkData->MAX_SPEED * 0.01f;
        rot.x = 1.5707964f;
        param.vPos = pos;
        param.fMaxDist = pAtkData->WORK0;
        param.pEffectTbl = PL::g_EffTbl_Skill[pAtkData->EFFECT];
        param.tqHomingTime = pAtkData->WORK2 * 100;
        param.vRot = rot;
        param.fCollRadius = pAtkData->COLL_START_FRAME * 0.01f;
        param.vPos.w = pAtkData->COLL_END_FRAME * 0.01f;
        param.pRotTbl = PL::g_RotTbl_Magic;
        bullet = (CBullet*)CEntityFactory::getInstance()->createBullet(param, this);
        if (bullet) bullet->setBodyRadius(pAtkData->WORK1 * 0.01f, 0.0f, 0, (COLL_KIND)-1);
        break;
    case 92:
    case 116:
    case 117:
        param.init(1, 3);
        param.CollKind = 0x80000000;
        param.uiFlag = 4;
        param.AtkKind = 7;
        param.AtkAttr = m_AtkAttr;
        param.iAttackPoint = getPower(m_uiSkillID, 0) / 10;
        param.tqHomingTime = 0;
        param.fSpeed = 0.0f;
        param.vPos = getPos();
        param.vRot = getRot();
        param.pEffectTbl = PL::g_EffTbl_Skill[51];
        param.fCollRadius = 50.0f;
        CEntityFactory::getInstance()->createBullet(param, this);
        break;
    case 118:
    case 119:
        getWeaponTopPos(pos);
        param.init(2, 3);
        param.CollKind = 0x80000000;
        param.uiFlag = 0x21008140;
        param.AtkKind = pAtkData->DAMAGE;
        param.AtkAttr = m_AtkAttr;
        param.iAttackPoint = getPower(m_uiSkillID, m_uiKind) / 10;
        param.fSpeed = pAtkData->MAX_SPEED * 0.01f;
        param.fMaxDist = pAtkData->WORK0;
        param.tqHomingTime = 9000;
        param.fHomingRadius = pAtkData->RADIUS;
        param.fHomingRate = pAtkData->WORK1;
        param.tqFadeTime = 1000;
        param.vPos = pos;
        g_pPlayer->getWeaponTopPos(pos);
        param.vRot = getRot();
        param.vRot.y = _limitPI(g_pPlayer->calcDirection(pos) + 3.1415927f);
        param.fCollRadius = pAtkData->COLL_START_FRAME * 0.01f;
        param.pEffectTbl = PL::g_EffTbl_Skill[pAtkData->EFFECT];
        bullet = (CBullet*)CEntityFactory::getInstance()->createBullet(param, this);
        if (bullet) {
            bullet->setBodyRadius(0.1f, 0.0f, 0, (COLL_KIND)-1);
            if (m_pPlayer->getCount() == 1) {
                m_uiCollAttackID = bullet->getAttackID();
            } else {
                bullet->setAttackID(m_uiCollAttackID);
            }
        }
        break;
    }
}
#endif

void CFriend::calcCirclePos(FVECTOR& outPos, FLOAT inRadius, FLOAT inDir) {
    FVECTOR ofs;
    FMATRIX mat;
    ofs.z = inRadius;
    ofs.w = 1.0f;
    ofs.x = 0.0f;
    ofs.y = 0.0f;
    _unitMatrix(mat);
    _rotMatrixY(mat, inDir);
    ApplyMatrix(outPos, mat, ofs);
}

void CFriend::changeCamera(HANDLE inTarget, INT inMode, INT inReset) {
    m_pPlayer->getPlayerCamera()->changeNowPlayer(getHandle(), inTarget);
    m_pPlayer->getPlayerCamera()->changeCamMode(inMode, inReset);
}

void CFriend::setLockon(HANDLE inHandle) {
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

void CFriend::resetLockon() {
    if (g_pPlayer && (m_uiFlag & 0x80)) {
        SET_FLAG(0, 0x80);
        Gauge::CLockonMarker* pMarker = m_pPlayer->getLockonMarker();
        m_pPlayer->setPlayerFlag(0, 0x10000);
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

void CFriend::killChild(FLOAT inFadeTime) {
    CEffectMan::killAll(ObjType::OBJ_BIT_FRIEND, inFadeTime);
    m_pEffect->kill(inFadeTime);
    m_pSE->stop();
    killLoopEffect(0);
    CBulletManager* pBM = (CBulletManager*)CTaskMan::getSysTask(0x1b);
    if (pBM) pBM->kill(getHandle());
}

void CFriend::setPushon(INT inFlag) {
    Gauge::CLockonMarker* pMarker = m_pPlayer->getLockonMarker();
    if (inFlag) {
        pMarker->onPush(inFlag);
    } else {
        pMarker->offPush();
    }
}

INT CFriend::entryChainTrg(INT inMode) {
    if (m_hTarget <= 0) return FALSE;
    UINT flag = m_uiFlag;
    if (!(flag & 0x800)) return FALSE;
    if (flag & 0x400) return FALSE;
    if (flag & 0x40000) return FALSE;
    setPushon(inMode);
    SET_FLAG(0x400, 0x1000);
    return TRUE;
}

INT CFriend::checkChainTrg() {
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

INT CFriend::isSucceedChain() {
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

void CFriend::beginStateAppear_() {
    beginStateAppear();
}
void CFriend::stateAppear_() {
    stateAppear();
}
void CFriend::endStateAppear_() {
    endStateAppear();
}
void CFriend::beginStateMain_() {
    beginStateMain();
}
void CFriend::stateMain_() {
    stateMain();
}
void CFriend::endStateMain_() {
    endStateMain();
}
void CFriend::beginStateAction1_() {
    beginStateAction1();
}
void CFriend::stateAction1_() {
    stateAction1();
}
void CFriend::endStateAction1_() {
    endStateAction1();
}
void CFriend::beginStateAction2_() {
    beginStateAction2();
}
void CFriend::stateAction2_() {
    stateAction2();
}
void CFriend::endStateAction2_() {
    endStateAction2();
}
void CFriend::beginStateAction3_() {
    beginStateAction3();
}
void CFriend::stateAction3_() {
    stateAction3();
}
void CFriend::endStateAction3_() {
    endStateAction3();
}
void CFriend::beginStateExit_() {
    beginStateExit();
}
void CFriend::stateExit_() {
    stateExit();
}
void CFriend::endStateExit_() {
    endStateExit();
}

void CFriend::getCameraTargetPos(FVECTOR& outPos) {
    outPos = Crd(0).getWorldMatrix().v[3];
    outPos.y += m_fCamHeight;
    if (dbgCheckUser(KKIKUCHI) && DebugWork::g_dw.player.iTemp == 6) {
        outPos.y += DebugWork::g_dw.player.vTemp.y;
    }
}

} // namespace Friend
// SOURCE DATA: .data .rodata
