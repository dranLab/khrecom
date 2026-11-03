#define GPRIM_H_PATH "System/Graphic/GPrim.h" /* MATCH: this unit names Gprim.h so (header default "System/Graphic/Gprim.h") */
#include "System/Task.h"
#include "System/TaskMan.h"
#include "Game/Player/FriendGoofy.h"
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

#include "Game/BodyCollision.h"
#include "Game/Enemy/BattleEnemy.h"
#include "Game/Player/Player.h"
#include "Game/Player/PlayerCamera.h"
#include "Game/Player/PlayerAttack.h"
#include "Game/AttackMotion.h"
#include "Game/Movement.h"
#include "Game/LockOnManager.h"
#include "System/Rnd.h"

extern "C" {
void sceVu0UnitMatrix(FMATRIX& m);
void sceVu0RotMatrixY(FMATRIX& m0, FMATRIX& m1, float ry);
}
void ApplyMatrixXYZ(FVECTOR& v0, const FMATRIX& m0, const FVECTOR& v1);
void AddVectorXYZ(FVECTOR& v0, const FVECTOR& v1, const FVECTOR& v2);

/* Through inline wrappers (as in FriendKing): the original recomputes the matrix address for each call. */
static inline void _unitMatrix(FMATRIX& m) {
    sceVu0UnitMatrix(m);
}
static inline void _rotMatrixY(FMATRIX& m, FLOAT r) {
    sceVu0RotMatrixY(m, m, r);
}

namespace Friend {

static LPCSTR g_szBoneShield = "shield";

unsigned char g_GoofyAttackID[4] = { 86, 100, 101, 0 };

static inline BOOL _isCatchable(CBattleEnemy* pTarget) {
    if (pTarget->isNoDmgResponse()) return 0;
    if (pTarget->isNoBigDamage()) return 0;
    return 1;
}

static inline FLOAT _adjustRad(FLOAT r) {
    FLOAT ret = r;
    if (r < -3.1415927f) {
        ret = r + 6.2831855f;
    } else if (ret > 3.1415927f) {
        ret -= 6.2831855f;
    }
    return ret;
}

CFriendGoofy::CFriendGoofy(CTask* inParent, INT inEx) : CFriend(inParent, 2, inEx) {
    LPCSTR& name = g_szBoneShield;
    m_iAtkBone = getBoneIndex(name, 0);
    m_vAtkOffset.set(0.0f, 0.0f, 0.0f);
}

CFriendGoofy::~CFriendGoofy() {
}

void CFriendGoofy::beginStateAppear() {
    m_iState = 0;
    setEntityFlag(0, 1);
    m_uiStartAnimID = 3;
    m_uiGroundAnimID = 3;
    changeMotion(3, 0, 1, -1, 0, 0, 0);
    if (m_pPlayer->getAttackID() == 104) {
        adjustPlayer(0);
        m_uiFlag |= 8;
    }
    updateCoord();
    coordFix();
    setAppearEffect();
    bodyCollOnBody(-1);
    m_pMove->enableGravity(1);
    clearSpeed();
}

void CFriendGoofy::stateAppear() {
    if (m_pPlayer->getAttackID() == 104) {
        adjustPlayer(0);
    } else {
        m_pMove->homingY(m_pPlayer, 0.1f);
    }
    if (m_fScale < 1.0f) {
        m_fScale += getFrameRateF() * 0.05f;
        if (m_fScale > 1.0f) m_fScale = 1.0f;
        FVECTOR scale = getScale();
        scale.z = m_fScale;
        scale.x = m_fScale;
        scale.y = m_fScale * 0.5f + 0.5f;
        setScale(scale);
    } else if (m_uiAnimID == 1 || isMotionEnd()) {
        changeMotion(1, 0, 1, -1, 0, 0, 0);
        changeState(1);
    }
}

INT CFriendGoofy::setSkill(INT inID) {
    if (m_iState != 1) return 0;
    switch (inID) {
    case 86:
        m_uiSkillID = 5;
        m_uiLevel = 0;
        changeState(2);
        break;
    case 100:
        m_uiSkillID = 6;
        m_uiLevel = 1;
        changeState(2);
        break;
    case 101:
        m_uiSkillID = 7;
        m_uiLevel = 2;
        changeState(2);
        break;
    case 102:
        m_uiSkillID = 8;
        m_uiLevel = 0;
        changeState(3);
        break;
    case 104:
        m_uiSkillID = 9;
        m_uiLevel = 0;
        changeState(4);
        break;
    default:
        setExit(0);
        break;
    }
    return 1;
}

void CFriendGoofy::beginStateAction1() {
    m_iState = 2;
    m_iSubState = 1;
    m_uiAttackID = g_GoofyAttackID[m_uiLevel];
    m_pAtkData = &PL::g_AttackDataList[m_uiAttackID];
    m_iAttackPower = getPower(m_uiSkillID, 0) / 10;
    BC_PARAM*& bc = CardBattle::g_pBattleCardList;
    m_AtkAttr = bc[50].ATTR;
    changeMotion(6, 0, 1, -1, 0, 0, 0);
    m_pMove->enableGravity(1);
    m_pPlayer->getPlayerCamera()->changeNowPlayer(m_pPlayer->getHandle(), getHandle());
    m_pPlayer->getPlayerCamera()->changeCamMode(4, 0);
}

void CFriendGoofy::stateAction1() {
    if (exeGoofyTornado()) setExit(0);
    if (m_tqSubTimer > 0) m_tqSubTimer -= getFrameRate();
}

INT CFriendGoofy::exeGoofyTornado() {
    switch (m_iSubState) {
    case 1:
        if (m_uiAnimID == 6 && isMotionEnd()) {
            m_iSubState = 2;
            setTimer(getPower(m_uiSkillID, 3));
            changeMotion(7, 0, 1, -1, 0, 0, m_uiLevel);
            m_tqSubTimer = m_pAtkData->WORK2 * 100;
            m_vAimPos.init();
            m_hTarget = 0;
            FVECTOR pos = m_pPlayer->getPos();
            FLOAT r = calcDirection(pos) + 3.1415927f;
            FLOAT rot;
            if (r < -3.1415927f) {
                rot = r + 6.2831855f;
            } else {
                rot = r;
                if (rot > 3.1415927f) rot -= 6.2831855f;
            }
            m_fRotDir = rot;
            m_fDist = m_pAtkData->RADIUS * 0.1f;
        }
        break;
    case 2:
        if (m_hTarget == 0) {
            m_fRotDir = _adjustRad(m_fRotDir - m_pAtkData->WORK0 * 0.01f * getFrameRateF());
        } else {
            m_fRotDir = _adjustRad(m_fRotDir - m_pAtkData->WORK0 * 0.001f * getFrameRateF());
        }
        if (!isTargetMoveXZ() || m_tqSubTimer <= 0) {
            if (m_pAttack) m_pAttack->updateAttackID();
            m_tqSubTimer = m_pAtkData->WORK2 * 100;
            calcCirclePos(m_vAimPos, m_fDist, m_fRotDir);
            AddVectorXYZ(m_vAimPos, m_vAimPos, m_pPlayer->getPos());
            if (RND::genrand_int31() % 100 < m_pAtkData->TRG_FRAME) {
                m_hTarget = LockOnManager::CLockOnManager::getLockOnObject(*this);
                CEntity* pTarget = m_hTarget > 0 ? (CEntity*)CTaskMan::getTask(m_hTarget) : 0;
                if (pTarget && calcDistanceXZ(pTarget) <= m_fDist) m_vAimPos = pTarget->getPos();
            } else {
                m_hTarget = 0;
            }
            startTargetMoveXZ(m_vAimPos, m_pAtkData->MAX_SPEED * 0.01f, m_pAtkData->WORK1 * 0.01f, 1.0f);
        }
        if (getTimer() <= 0) {
            endTargetMoveXZ();
            m_iSubState = 8;
            return 1;
        }
        break;
    }
    return 0;
}

void CFriendGoofy::beginStateAction2() {
    m_iSubState = 1;
    m_iAttackNo = 0;
    m_iState = 3;
    m_uiAttackID = 102;
    m_pAtkData = &PL::g_AttackDataList[m_uiAttackID];
    m_iAttackPower = getPower(m_uiSkillID, 0) / 10;
    BC_PARAM*& bc = CardBattle::g_pBattleCardList;
    m_AtkAttr = bc[50].ATTR;
    changeMotion(8, 0, 1, -1, 0, 0, 0);
    homingPlayerTargetY(m_pPlayer->isNowMark(), 0.1f);
    setLockon(m_hTarget);
    changeCamera(m_hTarget, 4, 0);
    m_tqSubTimer = 0;
    m_fMoveSpdXZ = 0.0f;
    m_pMove->enableGravity(0);
    clearSpeed();
    m_fBodyRviseCoefficient = 0.2f;
}

void CFriendGoofy::stateAction2() {
    if (exeGoofySmash()) setExit(0);
}

INT CFriendGoofy::exeGoofySmash() {
    CBattleEnemy* pTarget = m_hTarget > 0 ? (CBattleEnemy*)CTaskMan::getTask(m_hTarget) : 0;
    INT frame = getMotionCurrentTime() / 50;
    switch (m_iSubState) {
    case 1:
        if (m_pAttack) m_pAttack->setEnemyPos(m_pPlayer->getPos());
        if (m_uiFlag & 0x800) {
            if (pTarget && _isCatchable(pTarget) && !pTarget->isBind()) {
                m_iSubState = 3;
                setTimer(3000);
                m_pPlayer->getPlayerCamera()->changeNowPlayer(m_pPlayer->getHandle(), m_hTarget);
                m_pPlayer->getPlayerCamera()->changeCamMode(4, 0);
                return 0;
            }
            m_uiFlag = (INT)m_uiFlag & ~0x800;
        }
        if (frame <= m_pAtkData->MOVE_START_FRAME) {
            homingPlayerTargetY(m_hTarget, 0.3f);
            if (m_hTarget != m_hLockon) {
                setLockon(m_hTarget);
                changeCamera(m_hTarget, 4, 0);
            }
            m_fMoveSpdXZ = 0.0f;
        } else if (frame <= m_pAtkData->MOVE_END_FRAME) {
            if (calcDistanceXZ(pTarget) > m_pAtkData->RADIUS * 0.1f) {
                m_fMoveSpdXZ = Movement::Approach(m_fMoveSpdXZ, m_pAtkData->MAX_SPEED / 60.0f, 0.2f, getFrameRateF());
            } else {
                m_fMoveSpdXZ = Movement::Approach(m_fMoveSpdXZ, 0.0f, 0.4f, getFrameRateF());
            }
            m_pMove->calcVelocity(m_fMoveSpdXZ);
        } else {
            FVECTOR vel = m_pMove->getVel();
            FLOAT fMoveSpdY = vel.y;
            m_fMoveSpdXZ = Movement::Approach(m_fMoveSpdXZ, 0.0f, 0.4f, getFrameRateF());
            m_pMove->calcVelocity(m_fMoveSpdXZ);
            if (!(m_uiFlag & 4)) {
                vel = m_pMove->getVel();
                vel.y = PL::g_fAntiGravity * m_pAtkData->WORK1;
                m_pMove->setVel(vel);
                m_pMove->enableGravity(1);
                m_uiFlag |= 4;
            } else {
                vel = m_pMove->getVel();
                vel.y = fMoveSpdY;
                m_pMove->setVel(vel);
            }
        }
        if (isMotionEnd()) return 1;
        break;
    case 3:
        if (pTarget) {
            pTarget->getLockOnPos(m_vAimPos);
            if (checkChainTrg()) {
                m_pPlayer->setAttack_inFriend(103, 1, 0);
                m_pPlayer->getPlayerCamera()->changeNowPlayer(m_pPlayer->getHandle(), m_hTarget);
                m_pPlayer->getPlayerCamera()->changeCamMode(3, 0);
            }
            if (pTarget->getFlag() & 0x10000) {
                if (pTarget->getVel().y > 0.0f && entryChainTrg(1)) setTimer(3000);
            }
            if (isSucceedChain()) {
                if (m_uiFlag & 0x8000) {
                    setPushon(0);
                    setLockon(0);
                    return 1;
                }
                if (m_uiFlag & 0x4000) {
                    m_iSubState = 4;
                    setLockon(m_hTarget);
                    changeCamera(m_hTarget, 4, 0);
                    m_uiFlag = (INT)m_uiFlag & ~4;
                    setTimer(9000);
                    m_iAttackPower = getPower(m_uiSkillID, 1) / 10;
                    changeMotion(9, 0, 1, -1, 0, 0, 0);
                    m_uiFlag &= ~0x1fc00;
                    return 0;
                }
            }
        } else {
            m_pPlayer->setPlayerFlag(0, 0x20000);
            setLockon(0);
            if (!(m_uiFlag & 0x400) && m_uiAnimID == 1) setTimer(0);
        }
        if (m_uiAnimID == 8) {
            FVECTOR vel = m_pMove->getVel();
            FLOAT fMoveSpdY = vel.y;
            m_fMoveSpdXZ = Movement::Approach(m_fMoveSpdXZ, 0.0f, 0.4f, getFrameRateF());
            m_pMove->calcVelocity(m_fMoveSpdXZ);
            if (!(m_uiFlag & 4)) {
                if (m_pAtkData->MOVE_END_FRAME < frame) {
                    vel = m_pMove->getVel();
                    vel.y = PL::g_fAntiGravity * m_pAtkData->WORK1;
                    m_pMove->setVel(vel);
                    m_pMove->enableGravity(1);
                    m_uiFlag |= 4;
                }
            } else {
                vel = m_pMove->getVel();
                vel.y = fMoveSpdY;
                m_pMove->setVel(vel);
            }
        } else {
            m_pMove->homingY(m_vAimPos, 0.1f);
        }
        if (isMotionEnd() && m_uiAnimID != 1) {
            changeMotion(1, 0, 1, -1, 0, 0, 0);
            clearSpeed();
            m_uiFlag = (INT)m_uiFlag & ~4;
        }
        if (getTimer() > 0) return 0;
        setPushon(0);
        setLockon(0);
        return 1;
    case 4: {
        if (!pTarget || getTimer() <= 0) {
            m_iSubState = 5;
            setLockon(0);
            return 0;
        }
        pTarget->getLockOnPos(m_vAimPos);
        m_pMove->homingY(m_vAimPos, 0.2f);
        FVECTOR pos;
        getLockOnPos(pos);
        FVECTOR vel = m_pMove->getVel();
        FLOAT fMoveSpdY = vel.y;
        if (calcDistanceXZ(pTarget) > m_pAtkData->RADIUS * 0.1f) {
            m_fMoveSpdXZ = Movement::Approach(m_fMoveSpdXZ, m_pAtkData->MAX_SPEED / 60.0f, 0.2f, getFrameRateF());
        } else {
            m_fMoveSpdXZ = Movement::Approach(m_fMoveSpdXZ, 0.0f, 0.4f, getFrameRateF());
        }
        m_pMove->calcVelocity(m_fMoveSpdXZ);
        if (!(m_uiFlag & 4)) {
            vel = m_pMove->getVel();
            FLOAT distY = m_vAimPos.y - getPos().y;
            vel.y = PL::g_fAntiGravity * (m_pAtkData->WORK2 + distY * 10.0f);
            m_pMove->setVel(vel);
            m_pMove->enableGravity(1);
            m_uiFlag |= 4;
        } else {
            vel = m_pMove->getVel();
            vel.y = fMoveSpdY;
            m_pMove->setVel(vel);
        }
        if (pos.y > m_vAimPos.y) {
            vel.y *= 0.85f;
            if (vel.y < 0.0f) vel.y = 0.0f;
            setVel(vel);
            if (!(m_uiFlag & 0x800)) {
                pos = pTarget->getOldPos();
                pTarget->setPos(pos);
                vel = pTarget->getVel();
                vel.y *= 0.5f;
                pTarget->setVel(vel);
                return 0;
            }
            m_iSubState = 5;
        }
        return 0;
    }
    case 5:
        if (m_uiFlag & 4) {
            if (m_uiAnimID == 9 && frame <= m_pAtkData->COLL_END_FRAME) clearSpeed();
            if (isGround()) {
                changeMotion(3, 0, 1, -1, 0, 0, 0);
                clearSpeed();
                m_uiFlag = (INT)m_uiFlag & ~4;
            } else if (m_uiAnimID != 2 && isMotionEnd()) {
                changeMotion(2, 0, 1, -1, 0, 0, 0);
            }
        } else if (isMotionEnd()) {
            m_iSubState = 8;
            setPushon(0);
            setLockon(0);
            if (m_uiAnimID != 1) changeMotion(1, 0, 1, -1, 0, 0, 0);
            return 1;
        }
        return 0;
    }
    return 0;
}

void CFriendGoofy::beginStateAction3() {
    m_iSubState = 1;
    m_iAttackNo = 0;
    m_iState = 4;
    m_uiAttackID = 104;
    m_pAtkData = &PL::g_AttackDataList[m_uiAttackID];
    m_iAttackPower = getPower(m_uiSkillID, 0) / 10;
    SS_PARAM*& ss = CardBattle::g_pStockSkillList;
    m_AtkAttr = ss[74].ATTR;
    m_uiFlag |= 8;
    setDispOffFlag(0xc, 0);
    m_pMove->enableGravity(0);
    clearSpeed();
}

void CFriendGoofy::stateAction3() {
    if (exeGoofyWildCrusher()) setExit(0);
}

INT CFriendGoofy::exeGoofyWildCrusher() {
    switch (m_iSubState) {
    case 1:
        adjustPlayer(1);
        if (m_pPlayer->isConditionFlag(0x80000)) {
            m_iSubState = 6;
            m_uiFlag |= 8;
            userBodyCollOff(-1);
            userBodyCollOffBody(-1);
            changeMotion(10, 0, 0, 0, 0, 0, 0);
        }
        break;
    case 6:
        adjustPlayer(1);
        if (isMotionEnd()) {
            m_iSubState = 7;
            changeMotion(11, 0, 0, -1, 0, 0, 0);
            setTimer(3000);
        }
        break;
    case 7:
        if (getTimer() <= 0) {
            m_pAttack->updateAttackID();
            setTimer(3000);
        }
        if (!m_pPlayer->isConditionFlag(0x80000)) {
            m_iSubState = 8;
            m_uiFlag = (INT)m_uiFlag & ~8;
            userBodyCollOn(-1);
            userBodyCollOnBody(-1);
            clearSpeed();
            adjustPlayer(0);
            changeMotion(12, 0, 1, -1, 0, 0, 0);
            m_uiFlag = (INT)m_uiFlag & ~8;
            setOldPos(m_pPlayer->getCenterPos());
        } else {
            adjustPlayer(1);
        }
        break;
    case 8:
        if (isMotionEnd()) return 1;
        break;
    }
    return 0;
}

void CFriendGoofy::adjustPlayer(INT inWarp) {
    FVECTOR pos = { { -0.25f, 0.0f, -1.1f, 1.0f } };
    FVECTOR rot;
    FMATRIX m;

    rot = m_pPlayer->getRot();
    setRot(rot);
    _unitMatrix(m);
    _rotMatrixY(m, rot.y);
    ApplyMatrixXYZ(pos, m, pos);
    AddVectorXYZ(pos, pos, m_pPlayer->getPos());
    if (inWarp) {
        warp(pos);
    } else {
        setPos(pos);
    }
}

} // namespace Friend

// SOURCE DATA: .data .rodata
