#define GPRIM_H_PATH "System/Graphic/GPrim.h" /* MATCH: this unit names Gprim.h so (header default "System/Graphic/Gprim.h") */

#include "System/Task.h"
#include "Game/Player/SummonCloud.h"
#include "System/Graphic/Gprim.h"
#include "System/Graphic/2d/2DMan.h"
#include "Game/CardBattle.h"
#include "System/Graphic/Str.h"
#include "Game/Collision/CollUtil.h"

/* Placeholders: three empty strings follow CollUtil.h's strings in .rodata (as in FriendAladdin). */
inline void _emptyStringPlaceholder0() {
    TRACE("");
}
inline void _emptyStringPlaceholder1() {
    TRACE("");
}
inline void _emptyStringPlaceholder2() {
    TRACE("");
}


#include "System/TaskMan.h"



#include "Game/BodyCollision.h"
#include "Game/Enemy/BattleEnemy.h"
#include "System/Graphic/Effect/effect.h"
#include "System/Graphic/Effect/EffectMan.h"
#include "Game/Player/Player.h"
#include "Game/Player/PlayerCamera.h"
#include "Game/Player/PlayerAttack.h"
#include "Game/Movement.h"
#include "Game/Collision/CollMap.h"
#include "Game/LockOnManager.h"
#include "System/TaskMan.h"
#include "System/Math3D.h"
#include "Game/Player/PlayerManager.h"
#include "System/Graphic/Camera.h"

extern "C" {
void sceVu0UnitMatrix(FMATRIX& m);
void sceVu0RotMatrixY(FMATRIX& m0, FMATRIX& m1, float ry);
}
void ApplyMatrixXYZ(FVECTOR& v0, const FMATRIX& m0, const FVECTOR& v1);
void AddVectorXYZ(FVECTOR& v0, const FVECTOR& v1, const FVECTOR& v2);
void SubVectorXYZ(FVECTOR& v0, const FVECTOR& v1, const FVECTOR& v2);

/* CEntity::m_vStartPos (Game/Entity.h has it as raw bytes). */


static inline FLOAT _abs(FLOAT inValue) {
    FLOAT ret;
    __asm__ volatile("abs.s %0, %1" : "=f"(ret) : "f"(inValue));
    return ret;
}

static inline void _unitMatrix(FMATRIX& m) {
    sceVu0UnitMatrix(m);
}
static inline void _rotMatrixY(FMATRIX& m, FLOAT r) {
    sceVu0RotMatrixY(m, m, r);
}

namespace Summon {

static LPCSTR g_szBoneWep0 = "wep0";
static LPCSTR g_szDpxCloudStEd = "cloud_st_ed";
static LPCSTR g_szDpxSummon = "summon";

unsigned char g_CloudAttackID[4] = { 128, 145, 147, 146 };

CSummonCloud::CSummonCloud(CTask* inParent) : CSummon(inParent, 7) {
    LPCSTR& name = g_szBoneWep0;
    m_iAtkBone = getBoneIndex(name, 0);
    m_vAtkOffset.set(1.78f, 0.0f, 0.0f);
    m_hEffect0 = 0;
    m_hEffect1 = 0;
    m_fAlpha = 0.0f;
    m_fWingsAlpha = 0.0f;
    setDispOffFlag(0x21, 0);
}

CSummonCloud::~CSummonCloud() {
    if (m_hEffect0 > 0) {
        CEffect* effect = CEffectMan::getEffect(m_hEffect0);
        if (effect) effect->kill();
        m_hEffect0 = 0;
    }
    if (m_hEffect1 > 0) {
        CEffect* effect = CEffectMan::getEffect(m_hEffect1);
        if (effect) effect->kill();
        m_hEffect1 = 0;
    }
}

void CSummonCloud::updateBefore() {
    fadeWings_();
    CSummon::updateBefore();
}

INT CSummonCloud::setSkill(INT inID) {
    if (m_iState != 1) return 0;
    switch (inID) {
    case 128:
        m_uiSkillID = 20;
        m_uiStartAnimID = 6;
        break;
    case 145:
        m_uiSkillID = 21;
        m_uiStartAnimID = 7;
        break;
    case 147:
        m_uiSkillID = 22;
        m_uiStartAnimID = 7;
        break;
    case 146:
        m_uiSkillID = 23;
        m_uiStartAnimID = 8;
        {
            INT level = 3;
            m_uiLevel = level;
            changeState(level);
        }
        return 1;
    default:
        setExit(0);
        return 1;
    }
    m_uiLevel = m_uiSkillID - 20;
    changeState(2);
    return 1;
}

void CSummonCloud::beginStateAppear() {
    m_iState = 0;
    setEntityFlag(0, 1);
    if (m_pPlayer->getAttackID() == 147) {
        m_vAimPos = m_pPlayer->getPos();
        homingPlayerTargetY(m_pPlayer->isNowMark(), 0.1f);
        CEntity* target = (CEntity*)CTaskMan::getTask(m_hTarget);
        if (target) {
            FMATRIX mat = target->Crd(0).getWorldMatrix();
            PLM::getWorldRotY(m_fDir, mat);
            FVECTOR rot = getRot();
            rot.y = m_fDir;
            setRot(rot);
            target->getLockOnPos(m_vAimPos);
            m_pPlayer->setDir(m_fDir);
            m_pPlayer->calcAppearPos(-1.5f, 0.75f, &m_vAimPos);
        } else {
            m_pPlayer->calcAppearPos(1.5f, 0.75f, &m_vAimPos);
        }
        m_vStartPos = m_pPlayer->getAppearPos();
        FVECTOR pos;
        CCollMap* coll;
        FVECTOR* dpos;
        FLOAT length;
        FLOAT len0;
        coll = m_pMove->getMapCollision();
        if (!coll) AssertApp(__FILE__, 189, "coll", 0); /* line 189 of the original SummonCloud.cpp */
        FLOAT radius = coll->getRadius();
        coll->setRadius(0.75f);
        coll->enableSliding(0);
        pos = m_pPlayer->getPos();
        FVECTOR start = { { 0.0f, 0.0f, -0.75f, 1.0f } };
        FMATRIX m;
        FVECTOR v;
        _unitMatrix(m);
        _rotMatrixY(m, m_pPlayer->getRot().y);
        ApplyMatrixXYZ(start, m, start);
        AddVectorXYZ(pos, pos, start);
        coll->setStartPos(pos);
        coll->setEndPos(m_vStartPos);
        SubVectorXYZ(v, m_vStartPos, pos);
        length = Math3D::VectorLength(v);
        if (coll->collideRide(0)) {
            dpos = coll->getDetectPos();
            SubVectorXYZ(v, *dpos, pos);
            len0 = Math3D::VectorLength(v);
            if (len0 < length) {
                m_vStartPos = *dpos;
                length = len0;
            }
        }
        if (coll->collideMap()) {
            dpos = coll->getDetectPos();
            SubVectorXYZ(v, *dpos, pos);
            len0 = Math3D::VectorLength(v);
            if (len0 < length) {
                m_vStartPos = *dpos;
                length = len0;
            }
        }
        start.x = m_vStartPos.x;
        start.y = 7.0f;
        start.z = m_vStartPos.z;
        pos.x = m_vStartPos.x;
        pos.y = -3.0f;
        pos.z = m_vStartPos.z;
        coll->setStartPos(start);
        coll->setEndPos(pos);
        length = 10.0f;
        if (coll->collideRide(0)) {
            dpos = coll->getDetectPos();
            len0 = start.y - dpos->y;
            if (len0 < length) {
                m_vStartPos.y = dpos->y;
                length = len0;
            }
        }
        if (coll->collideMap()) {
            dpos = coll->getDetectPos();
            len0 = start.y - dpos->y;
            if (len0 < length) {
                m_vStartPos.y = dpos->y;
                length = len0;
            }
        }
        warp(m_vStartPos);
        updateCoord();
        coordFix();
        coll->setRadius(radius);
        coll->enableSliding(1);
        m_uiSkillID = 22;
        m_tqSubTimer = 0;
    } else {
        setRot(m_pPlayer->getRot());
        m_fScale = 1.0f;
        FVECTOR scale = { { m_fScale, m_fScale, m_fScale, m_fScale } };
        setScale(scale);
        setupFade(0.0f, 1.0f, 1000, -1);
    }
    m_uiStartAnimID = 3;
    changeMotion(3, 0, 1, 0, 0, 0, 0);
    updateCoord();
    coordFix();
    if (m_pPlayer->getAttackID() != 147) {
        m_uiFlag |= 0x100000;
        setAppearEffect(0);
    }
    bodyCollOnBody(-1);
    m_pMove->enableGravity(1);
    clearSpeed();
}

void CSummonCloud::stateAppear() {
    FVECTOR rot;
    FVECTOR scale;
    if (m_uiSkillID == 22) {
        m_tqSubTimer += getFrameRate();
        if (m_tqSubTimer < 400) return;
        if (!(m_uiFlag & 0x100000)) {
            m_uiFlag |= 0x100000;
            m_vStartPos = getPos();
            setAppearEffect(&m_vStartPos);
        }
        rot = getRot();
        rot.y = calcDirection(m_vAimPos);
        setRot(rot);
        setRot(rot);
    } else {
        rot = m_pPlayer->getRot();
        setRot(rot);
    }
    if (m_fScale < 1.0f) {
        m_fScale += getFrameRateF() * 0.05f;
        if (m_fScale > 1.0f) m_fScale = 1.0f;
        scale = getScale();
        scale.z = m_fScale;
        scale.x = m_fScale;
        scale.y = m_fScale * 0.5f + 0.5f;
        setScale(scale);
    } else if (m_uiSkillID == 22 || isMotionEnd() || m_fScale != 1.0f) {
        m_tqSubTimer = 0;
        changeMotion(1, 0, 1, -1, 0, 0, 0);
        changeState(1);
    }
}

void CSummonCloud::beginStateAction1() {
    m_iAttackNo = 0;
    m_iState = 2;
    m_uiAttackID = g_CloudAttackID[m_uiLevel];
    m_pAtkData = &PL::g_AttackDataList[m_uiAttackID];
    m_iAttackPower = getPower(m_uiSkillID, 0) / 10;
    if (m_uiLevel == 0) {
        m_AtkAttr = (ATK_ATTR)CardBattle::g_pBattleCardList[41].ATTR;
    } else {
        m_AtkAttr = (ATK_ATTR)CardBattle::g_pStockSkillList[m_uiLevel + 64].ATTR;
    }
    CardBattle::CCardBattle::calcEnchant_AttackPoint(m_iAttackPower, m_AtkKind, m_AtkAttr, m_uiUpKind);
    changeMotion(m_uiStartAnimID, 0, 1, -1, 0, 0, 0);
    homingPlayerTargetY(m_hTarget, 0.1f);
    setLockon(m_hTarget);
    changeCamera(m_hTarget, 4, 0);
    m_tqSubTimer = 0;
    m_fMoveSpdXZ = 0.0f;
    setTimer(15000);
    m_pMove->enableGravity(1);
}

void CSummonCloud::stateAction1() {
    CEntity* pTarget = m_hTarget > 0 ? (CEntity*)CTaskMan::getTask(m_hTarget) : 0;
    FLOAT speed = 0.0f;
    INT frame = getMotionCurrentTime() / 50;
    if (m_pAtkData->MOVE_END_FRAME >= frame) {
        homingPlayerTargetY(m_hTarget, 0.3f);
        if (m_hTarget != m_hLockon) {
            setLockon(m_hTarget);
            changeCamera(m_hTarget, 4, 0);
        }
        if (pTarget) {
            if (calcDistanceXZ(pTarget) > m_pAtkData->RADIUS * 0.1f && m_uiSkillID == 22) {
                speed = m_pAtkData->MAX_SPEED / 60.0f;
            }
        }
    }
    if (!pTarget) {
        setLockon(0);
        m_pPlayer->getPlayerCamera()->changeCamMode(3, 0);
    }
    m_fMoveSpdXZ = Movement::Approach(m_fMoveSpdXZ, speed, 0.2f, getFrameRateF());
    m_pMove->calcVelocityXZ(m_fMoveSpdXZ);
    switch (m_iAttackNo) {
    case 0:
        if (m_pAtkData->WORK0 < frame) {
            m_iAttackNo = 1;
            m_iAttackPower = getPower(m_uiSkillID, 1) / 10;
            CardBattle::CCardBattle::calcEnchant_AttackPoint(m_iAttackPower, m_AtkKind, m_AtkAttr, m_uiUpKind);
            if (m_pAttack) m_pAttack->setAttackParam(m_iAttackPower, m_AtkAttr);
        }
        break;
    case 1:
        if (m_pAtkData->WORK1 < frame) {
            m_iAttackNo = 2;
            m_iAttackPower = getPower(m_uiSkillID, 2) / 10;
            CardBattle::CCardBattle::calcEnchant_AttackPoint(m_iAttackPower, m_AtkKind, m_AtkAttr, m_uiUpKind);
            if (m_pAttack) m_pAttack->setAttackParam(m_iAttackPower, m_AtkAttr);
        }
        break;
    case 2:
        break;
    }
    if (isMotionEnd()) {
        setLockon(0);
        m_pPlayer->getPlayerCamera()->changeCamMode(3, 0);
        changeMotion(1, 0, 1, -1, 0, 0, 0);
        setExit(0);
    }
}

void CSummonCloud::beginStateAction2() {
    m_iSubState = 0;
    m_iState = 3;
    m_uiAttackID = g_CloudAttackID[m_uiLevel];
    m_pAtkData = &PL::g_AttackDataList[m_uiAttackID];
    SS_PARAM*& ss = CardBattle::g_pStockSkillList;
    m_AtkAttr = (ATK_ATTR)ss[66].ATTR;
    m_iAttackNo = 0;
    m_uiKind = 0;
    m_iAttackPower = getPower(m_uiSkillID, 0) / 10;
    CardBattle::CCardBattle::calcEnchant_AttackPoint(m_iAttackPower, m_AtkKind, m_AtkAttr, m_uiUpKind);
    changeMotion(m_uiStartAnimID, 0, 1, -1, 0, 0, 0);
    homingPlayerTargetY(m_hTarget, 0.1f);
    setLockon(m_hTarget);
    changeCamera(m_hTarget, 4, 0);
    m_tqSubTimer = 0;
    m_pMove->enableGravity(1);
    m_pMove->setMass(0.75f);
    m_fAlpha = 0.01f;
    setDispOffFlag(0, 0x21);
    m_fDist = m_pAtkData->RADIUS * m_pAtkData->RADIUS * 0.01f;
    m_fMoveSpdXZ = m_pAtkData->MAX_SPEED * 0.01f;
    m_fRotDir = m_pAtkData->WORK1 * 0.01f;
}

void CSummonCloud::stateAction2() {
    HANDLE hTarget = m_hTarget;
    CEntity* pTarget = hTarget > 0 ? (CEntity*)CTaskMan::getTask(hTarget) : 0;
    FVECTOR aim;
    FVECTOR vel;
    aim.init();
    HANDLE hLockon;
    CEntity* pLockon;
    FVECTOR loc;
    loc.init();
    hLockon = m_hLockon;
    if (m_iSubState < 5) {
        if (!pTarget || !pTarget->isPermissionLockOn()) {
            m_hTarget = LockOnManager::CLockOnManager::getLockOnObject(*this);
            pTarget = (CEntity*)CTaskMan::getTask(m_hTarget);
        }
        if (pTarget) pTarget->getLockOnPos(aim);
        pLockon = hLockon > 0 ? (CEntity*)CTaskMan::getTask(hLockon) : 0;
        if (!pLockon || !pLockon->isPermissionLockOn()) {
            hLockon = LockOnManager::CLockOnManager::getRandomLockOnObject();
            pLockon = hLockon > 0 ? (CEntity*)CTaskMan::getTask(hLockon) : 0;
        }
        if (pLockon && pLockon->isPermissionLockOn()) pLockon->getLockOnPos(loc);
    }
    switch (m_iSubState) {
    case 0:
        m_hTarget = hTarget;
        m_vAimPos = aim;
        if (isMotionEnd()) {
            changeMotion(9, 0, 1, -1, 0, 0, 0);
            m_iSubState = 1;
            m_tqSubTimer = 0;
            vel = m_pMove->getVel();
            vel.y = m_pAtkData->WORK0 * 0.001f;
            m_pMove->setVel(vel);
            m_pMove->homingMoveXZ(m_vAimPos, m_fRotDir, m_fMoveSpdXZ);
            setTimer(m_pAtkData->MOVE_START_FRAME * 50);
        }
        break;
    case 1: {
        m_tqSubTimer += getFrameRate();
        if (m_pMove->getCollisionFlag() & 0xa) m_tqSubTimer += getFrameRate() * 2;
        if (m_uiFlag & 0x20) m_tqSubTimer += getFrameRate();
        m_hTarget = hTarget;
        m_vAimPos = aim;
        m_pMove->homingMoveXZ(m_vAimPos, m_fRotDir, m_fMoveSpdXZ);
        FVECTOR pos;
        getLockOnPos(pos);
        vel = getVel();
        if (_abs(m_vAimPos.y - pos.y) > 0.3f) {
            if (m_vAimPos.y < pos.y) {
                vel.y = -0.1f;
            } else if (pos.y < m_vAimPos.y) {
                vel.y = 0.1f;
            } else {
                vel.y = 0.0f;
            }
        }
        setVel(vel);
        if (m_pPlayer->isBattleFlag(4) || !pTarget || !pTarget->isPermissionLockOn()) goto finish;
        if (calcDistanceXZSq(m_vAimPos) <= m_fDist || m_tqSubTimer > 6000) {
            m_iAttackPower = getPower(m_uiSkillID, m_uiKind) / 10;
            CardBattle::CCardBattle::calcEnchant_AttackPoint(m_iAttackPower, m_AtkKind, m_AtkAttr, m_uiUpKind);
            changeMotion(m_uiKind + 10, 0, 1, -1, 0, 0, 0);
            m_iAttackNo++;
            m_uiKind = (m_iAttackNo < m_pAtkData->TRG_FRAME - 1) ? (m_iAttackNo & 1) : 2;
            m_iSubState = 2;
            setTimer(getMotionTotalTime() / m_pAtkData->MOVE_END_FRAME);
        }
        break;
    }
    case 2:
        if (getTimer() <= 0) {
            m_pMove->homingY(m_vAimPos, m_fRotDir);
            FVECTOR vel = m_pMove->getVel();
            vel.x = 0.0f;
            vel.z = 0.0f;
            m_pMove->setVel(vel);
            m_uiFlag |= 0x800;
        } else if (calcDistanceXZSq(m_vAimPos) > m_fDist) {
            m_pMove->homingMoveXZ(m_vAimPos, m_fRotDir, m_fMoveSpdXZ);
        } else {
            m_pMove->homingY(m_vAimPos, 0.5f);
        }
        if (m_iAttackNo < m_pAtkData->TRG_FRAME) {
            checkChainTrg();
            if (entryChainTrg(m_iAttackNo < m_pAtkData->TRG_FRAME - 1 ? 1 : 3)) {
                hLockon = m_hLockon = LockOnManager::CLockOnManager::getRandomLockOnObject();
                setLockon(hLockon);
                changeCamera(hLockon, 4, 0);
            }
            isSucceedChain();
            if (m_iAttackNo < m_pAtkData->TRG_FRAME && (m_uiFlag & 0x2000)) {
                m_hLockon = hLockon;
                changeCamera(hLockon, 4, 0);
                setLockon(m_hLockon);
                if (hLockon > 0) m_vAimPos = loc;
                if (isMotionEnd()) {
                    m_uiFlag &= 0xFFFF03FF;
                    m_hTarget = m_hLockon;
                    setLockon(m_hLockon);
                    if (calcDistanceXZSq(pTarget) > m_fDist) {
                        m_iSubState = 1;
                        m_tqSubTimer = 0;
                        changeMotion(9, 0, 1, -1, 0, 0, 0);
                        FVECTOR vel = m_pMove->getVel();
                        vel.y = m_pAtkData->WORK0 * 0.001f;
                        m_pMove->setVel(vel);
                        m_pMove->homingMoveXZ(m_vAimPos, m_fRotDir, m_fMoveSpdXZ);
                        setTimer(m_pAtkData->MOVE_START_FRAME * 50);
                    } else {
                        m_iSubState = 2;
                        m_iAttackPower = getPower(m_uiSkillID, m_uiKind) / 10;
                        CardBattle::CCardBattle::calcEnchant_AttackPoint(m_iAttackPower, m_AtkKind, m_AtkAttr, m_uiUpKind);
                        changeMotion(m_uiKind + 10, 0, 1, -1, 0, 0, 0);
                        m_iAttackNo++;
                        m_uiKind = (m_iAttackNo < m_pAtkData->TRG_FRAME - 1) ? (m_iAttackNo & 1) : 2;
                        setTimer(getMotionTotalTime() / m_pAtkData->MOVE_END_FRAME);
                    }
                }
                break;
            }
        }
        if (isMotionEnd()) {
        finish:
            changeMotion(13, 0, 1, -1, 0, 0, 0);
            m_iSubState = 5;
            m_fAlpha = -0.01f;
            setPushon(0);
            setLockon(0);
            clearSpeed();
        }
        break;
    case 5:
        if (isMotionEnd()) {
            m_pPlayer->getPlayerCamera()->changeCamMode(3, 0);
            changeMotion(1, 0, 1, -1, 0, 0, 0);
            setExit(0);
        }
        break;
    }
}

void CSummonCloud::endAction2() {
    m_fAlpha = -0.01f;
    m_pMove->setMass(1.0f);
}

void CSummonCloud::beginStateExit() {
    m_iState = 5;
    if (isGround() || !m_pPlayer->isBattleFlag(0x4000000)) {
        changeMotion(4, 0, 1, -1, 0, 0, 0);
    }
    bodyCollOffBody(-1);
    m_pMove->enableGravity(1);
    m_pMove->setMass(1.0f);
    m_iSubState = 0;
    TIMEQ t = (m_uiFlag & 0x40) ? 0 : 3000;
    m_tqTrgFrame = t;
    m_tqSubTimer = t;
    setTimer(t);
}

void CSummonCloud::stateExit() {
    if (m_iSubState > 0) {
        if (!(getFlag() & 0x40000000) && getTimer() <= 0) out();
    } else if (getTimer() <= 0 && m_iSubState == 0) {
        m_iSubState = 1;
        setDispOffFlag(0x21, 0);
        setupFade(1.0f, 0.0f, 1000, -1);
        resetLockon();
        setTimer(m_tqSubTimer);
        killChild(m_tqSubTimer * 0.02f);
        setAppearEffect(0);
        m_pPlayer->setPlayerFlag(0x8000000, 0);
        m_pMove->enableGravity(0);
        clearSpeed();
    }
}

HANDLE CSummonCloud::setAppearEffect(const FVECTOR* inPos) {
    if (!inPos) inPos = &getPos();
    FVECTOR pos = *inPos;
    CEffect* effect;
    m_hEffect0 = setEffect(g_szDpxCloudStEd, 0, &pos, 0, 0.0f);
    if (!m_hEffect0) return 0;
    effect = CEffectMan::getEffect(m_hEffect0);
    if (effect) {
        effect->scale().set(1.5f, 1.5f, 1.5f);
        effect->setZSortEntity(this, 2, 1);
    }
    m_hEffect1 = setEffect(g_szDpxSummon, 0, &pos, 0, 0.0f);
    if (!m_hEffect1) return 0;
    effect = CEffectMan::getEffect(m_hEffect1);
    if (effect) {
        effect->scale().set(1.5f, 1.5f, 1.5f);
        effect->setZSortEntity(this, 3, 1);
    }
    return m_hEffect0;
}

void CSummonCloud::fadeWings_() {
    if (m_fAlpha != 0.0f) {
        m_fWingsAlpha += m_fAlpha * getFrameRateF();
        if (m_fWingsAlpha > 1.0f) m_fWingsAlpha = 1.0f;
        if (m_fWingsAlpha < 0.0f) m_fWingsAlpha = 0.0f;
        setModelAlphaScale(m_fWingsAlpha, 0);
    }
}

} // namespace Summon
// SOURCE DATA: .data .rodata
