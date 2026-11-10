#define GPRIM_H_PATH "System/Graphic/GPrim.h" /* MATCH: this unit names Gprim.h so (header default "System/Graphic/Gprim.h") */
#include "System/Task.h"
#include "System/TaskMan.h"
#include "Game/Player/FriendDonald.h"
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
#include "Game/Enemy/BattleEnemy.h"
#include "Game/Player/Player.h"
#include "Game/Player/PlayerCamera.h"
#include "Game/Player/PlayerAttack.h"
#include "Game/Player/PlayerWeapon.h"
#include "Game/Bullet/Bullet.h"
#include "Game/EntityFactory.h"
#include "Game/Movement.h"
#include "System/Graphic/Effect/EffectMan.h"
#include "System/Rnd.h"
#include "System/Graphic/Camera.h"

extern "C" {
float sinf(float);
float cosf(float);
}
void ApplyMatrixXYZ(FVECTOR& v0, const FMATRIX& m0, const FVECTOR& v1);
void AddVectorXYZ(FVECTOR& v0, const FVECTOR& v1, const FVECTOR& v2);
void ScaleVectorXYZ(FVECTOR& v0, const FVECTOR& v1, FLOAT s);

namespace PL {
extern BULLET_EFFECT_INFO g_EffTbl_HitMark[27]; /* Game/Player/PlayerAttack.h */
extern BULLET_EFFECT_INFO g_EffTbl_Skill[67][4]; /* Game/Player/PlayerAttack.h */
}

/* Through inline wrappers (as in FriendKing): the original recomputes the matrix address for each call. */
static inline void _unitMatrix(FMATRIX& m) {
    sceVu0UnitMatrix(m.m);
}
static inline void _rotMatrixY(FMATRIX& m, FLOAT r) {
    sceVu0RotMatrixY(m.m, m.m, r);
}

namespace Friend {

static LPCSTR g_szBoneWeaponR = "WeaponR";

/* MATCH: Placeholders: two empty strings follow "WeaponR" in .rodata. */
inline void _emptyStringPlaceholder3() {
    TRACE("");
}
inline void _emptyStringPlaceholder4() { /* MATCH: placeholder: an inline function's strings, in the original .rodata order */
    TRACE("");
}

unsigned char g_DonaldMagicID[3][4] = {
    { 34, 35, 36, 37 },
    { 41, 43, 46, 54 },
    { 42, 44, 47, 55 },
};

unsigned char g_DonaldAttackID[4] = { 97, 98, 99, 0 };

static inline FLOAT _adjustRad(FLOAT r) {
    FLOAT ret = r;
    if (r < -3.1415927f) {
        ret = r + 6.2831855f;
    } else if (ret > 3.1415927f) {
        ret -= 6.2831855f;
    }
    return ret;
}

CFriendDonald::CFriendDonald(CTask* inParent) : CFriend(inParent, 1, 0) {
    LPCSTR& name = g_szBoneWeaponR;
    m_iAtkBone = getBoneIndex(name, 0);
    m_vAtkOffset.set(0.3f, -0.45f, 0.01f);
}

CFriendDonald::~CFriendDonald() {
}

void CFriendDonald::beginStateAppear() {
    m_iState = 0;
    setEntityFlag(0, 1);
    m_uiStartAnimID = 1;
    m_uiGroundAnimID = 0;
    changeMotion(1, 0, 1, -1, 0, 0, 0);
    if (m_pPlayer->getAttackID() == 104) {
        adjustPlayer(0);
        m_uiFlag |= 8;
    }
    updateCoord();
    coordFix();
    setAppearEffect();
    bodyCollOnBody(-1);
}

void CFriendDonald::stateAppear() {
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

INT CFriendDonald::setSkill(INT inID) {
    if (m_iState != 1) return 0;
    switch (inID) {
    case 85:
        m_uiSkillID = 1;
        m_uiLevel = 0;
        changeState(2);
        break;
    case 94:
        m_uiSkillID = 2;
        m_uiLevel = 1;
        changeState(2);
        break;
    case 95:
        m_uiSkillID = 3;
        m_uiLevel = 2;
        changeState(2);
        break;
    case 96:
        m_uiSkillID = 4;
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

void CFriendDonald::beginStateAction1() {
    m_iState = 2;
    m_uiKind = RND::genrand_int31() % 4;
    m_uiAttackID = g_DonaldMagicID[m_uiLevel][m_uiKind];
    BC_PARAM*& bc = CardBattle::g_pBattleCardList;
    m_AtkAttr = bc[m_uiKind + 26].ATTR;
    m_iSubState = m_uiKind + 2;
    if (m_iSubState < 4) {
        m_tqTrgFrame = 0;
        changeMotion(5, 0, 1, -1, m_uiKind & 1, 0, 0);
    } else {
        m_tqTrgFrame = 3700;
        changeMotion(7, 0, 1, -1, m_uiKind & 1, 0, 0);
    }
    m_pPlayer->getPlayerCamera()->changeNowPlayer(m_pPlayer->getHandle(), getHandle());
    m_pPlayer->getPlayerCamera()->changeCamMode(4, 0);
    m_pMove->enableGravity(1);
}

void CFriendDonald::stateAction1() {
    if (exeDonaldMagic()) setExit(0);
}

INT CFriendDonald::exeDonaldMagic() {
    switch (m_iSubState) {
    case 2:
        homingPlayerTargetY(0, 0.1f);
        if (isMotionEnd()) {
            changeMotion(6, 0, 1, -1, m_uiLevel, 0, 0);
            m_iSubState = 9;
            setMagic(34);
            return 0;
        }
        break;
    case 3:
        homingPlayerTargetY(0, 0.1f);
        if (isMotionEnd()) {
            changeMotion(6, 0, 1, -1, 3, 0, 0);
            m_iSubState = 9;
            setMagic(35);
            return 0;
        }
        break;
    case 4:
        homingPlayerTargetY(0, 0.1f);
        if (getMotionCurrentTime() < m_tqTrgFrame) return 0;
        m_iSubState = 9;
        setMagic(36);
        return 0;
    case 5:
        m_pMove->homingY(m_pPlayer, 0.1f);
        if (getMotionCurrentTime() < m_tqTrgFrame) return 0;
        m_iSubState = 9;
        setMagic(37);
        return 0;
    case 9:
        if (!isMotionEnd()) return 0;
        if (m_iAttackNo == 0) {
            m_iAttackNo = 1;
            changeState(2);
            return 0;
        }
        changeMotion(1, 0, 1, -1, 0, 0, 0);
        m_iSubState = 10;
        setTimer(2000);
        break;
    case 10:
        if (getTimer() <= 0) return 1;
        break;
    }
    return 0;
}

void CFriendDonald::beginStateAction2() {
    m_iState = 3;
    m_iSubState = 1;
    m_uiAttackID = 96;
    m_pAtkData = &PL::g_AttackDataList[m_uiAttackID];
    m_iAttackPower = getPower(m_uiSkillID, m_iAttackNo) / 10;
    SS_PARAM*& ss = CardBattle::g_pStockSkillList;
    m_AtkAttr = ss[70].ATTR;
    changeMotion(8, 0, 1, -1, 0, 0, 0);
    m_tqSubTimer = 0;
    m_fMoveSpdXZ = 0.0f;
    m_tqTrgFrame = m_pAtkData->TRG_FRAME * 100;
    m_uiFlag &= ~0x1fc00;
    homingPlayerTargetY(0, 0.1f);
    if (m_iAttackNo == 0) {
        setLockon(m_hTarget);
        changeCamera(m_hTarget, 4, 0);
        setTimer(3000);
    } else {
        m_pPlayer->getPlayerCamera()->changeNowPlayer(m_pPlayer->getHandle(), getHandle());
        m_pPlayer->getPlayerCamera()->changeCamMode(4, 0);
        setTimer(500);
    }
    CEntity* pTarget = (CEntity*)CTaskMan::getTask(m_hTarget);
    FLOAT dir = m_pPlayer->calcDirection(pTarget);
    if (pTarget) {
        FLOAT r = _adjustRad(m_pPlayer->calcDirection(getPos()) - dir);
        if (r < 0.0f) {
            dir = _adjustRad(dir - 0.7853982f);
        } else {
            dir = _adjustRad(dir + 0.7853982f);
        }
    }
    m_fDist = m_pAtkData->RADIUS * 0.1f;
    calcCirclePos(m_vAimPos, m_fDist, dir);
    AddVectorXYZ(m_vAimPos, m_vAimPos, m_pPlayer->getPos());
    m_pMove->enableGravity(0);
    clearSpeed();
}

void CFriendDonald::stateAction2() {
    if (exeDonaldStardustBlitz()) setExit(0);
}

INT CFriendDonald::exeDonaldStardustBlitz() {
    CBullet* pBlitz = m_hLockon > 0 ? (CBullet*)CTaskMan::getTask(m_hLockon) : 0;
    INT no;
    switch (m_iSubState) {
    case 1:
        if (calcDistanceXZ(m_vAimPos) < m_pAtkData->RADIUS * 0.01f || getTimer() <= 0) {
            m_fMoveSpdXZ = Movement::Approach(m_fMoveSpdXZ, 0.0f, 0.4f, getFrameRateF());
        } else {
            m_fMoveSpdXZ = Movement::Approach(m_fMoveSpdXZ, m_pAtkData->MAX_SPEED / 60.0f, 0.2f, getFrameRateF());
        }
        if (m_fMoveSpdXZ > 0.0f) {
            m_pMove->homingY(m_pPlayer, 0.2f);
            FVECTOR rot = getRot();
            m_pMove->homingMoveXZ(m_vAimPos, 1.0f, m_fMoveSpdXZ);
            setRot(rot);
        } else {
            m_iSubState = 6;
            changeMotion(9, 0, 1, -1, 0, 0, 0);
            clearSpeed();
        }
        return 0;
    case 6:
        m_pMove->homingY(m_pPlayer, 0.1f);
        if (getMotionCurrentTime() < m_tqTrgFrame) return 0;
        m_iSubState = 9;
        m_hLockon = setBlitz();
        setLockon(m_hLockon);
        setTimer(15000);
        m_uiFlag = (m_uiFlag | 0x800) & ~0x40000;
        break;
    case 9:
        if (m_pPlayer->isBattleFlag(4)) {
            setPushon(0);
            setLockon(0);
            return 1;
        }
        if (getTimer() <= 0) {
            setPushon(0);
            setLockon(0);
            /* The original tests m_iAttackNo here and jumps into the shared tail below. */
            no = m_iAttackNo;
            if (no < 2) goto next;
            goto last;
        }
        if (pBlitz && !pBlitz->isGround()) {
            if (!(m_uiFlag & 0x40000)) {
                FVECTOR vel = pBlitz->getVel();
                FVECTOR ppos;
                FVECTOR pos = pBlitz->getPos();
                if (vel.y < 0.0f) {
                    m_pPlayer->getHeadPos(ppos);
                    if (pos.y < ppos.y) {
                        vel.init();
                        pBlitz->setVel(vel);
                        if (m_tqSubTimer == 0) {
                            setTimer(1500);
                            m_tqSubTimer = 1500;
                        }
                    }
                }
            }
            if (checkChainTrg()) {
                m_pPlayer->setPlayerFlag(0, 0x20000);
                m_pPlayer->setAttack_inFriend(g_DonaldAttackID[m_iAttackNo], 1, m_hTarget);
                m_pPlayer->getPlayerCamera()->changeNowPlayer(m_pPlayer->getHandle(), m_hTarget);
                m_pPlayer->getPlayerCamera()->changeCamMode(3, 0);
            }
            m_fDist = pBlitz->calcDistanceXZ(m_pPlayer->getCenterPos());
            if (m_fDist < m_iAttackNo * 0.15f + 3.7f && entryChainTrg(1)) {
                setTimer(3000);
                m_pPlayer->getPlayerCamera()->changeNowPlayer(m_pPlayer->getHandle(), m_hLockon);
                m_pPlayer->getPlayerCamera()->changeCamMode(4, 0);
            }
            if (isMotionEnd()) changeMotion(8, 0, 1, -1, 0, 0, 0);
            if (!isSucceedChain()) break;
            checkBlitzHit(1);
            if (!(m_uiFlag & 0x10000)) break;
            setLockon(0);
            m_vAimPos = pBlitz->getPos();
            pBlitz->out();
            setStardust(m_iAttackNo);
        } else {
            setPushon(0);
        }
        no = m_iAttackNo;
        if (no < 2) {
        next:
            m_iAttackNo = no + 1;
            changeState(3);
            return 0;
        }
    last:
        m_pPlayer->getPlayerCamera()->changeNowPlayer(m_pPlayer->getHandle(), m_hTarget);
        m_pPlayer->getPlayerCamera()->changeCamMode(4, 0);
        m_iSubState = 10;
        setTimer(3000);
        break;
    case 10:
        if (isMotionEnd()) changeMotion(8, 0, 1, -1, 0, 0, 0);
        if (getTimer() <= 0) return 1;
        break;
    }
    return 0;
}

HANDLE CFriendDonald::setBlitz() {
    BULLET_PARAM param;
    FVECTOR apos;
    FVECTOR rot;
    FVECTOR pos = { { 0.0f, 0.0f, m_pAtkData->WORK0 * 0.1f, 1.0f } };
    FMATRIX m;

    _unitMatrix(m);
    _rotMatrixY(m, m_pPlayer->getRot().y);
    ApplyMatrixXYZ(pos, m, pos);
    AddVectorXYZ(pos, pos, m_pPlayer->getPos());
    m_fDist = calcDistance(pos) - 1.0f;
    calcDirection(rot, pos);
    getWeaponTopPos(apos);
    param.init(2, 3);
    param.CollKind = COLL_KIND_PSHOT;
    param.uiFlag = 0x68;
    param.AtkKind = m_pAtkData->DAMAGE;
    param.AtkAttr = m_AtkAttr;
    param.iAttackPoint = m_iAttackPower;
    param.fSpeed = m_pMove->calcOrbitSpeedFromDistance(m_pAtkData->WORK1 * 0.017453292f, m_fDist);
    param.fMaxDist = 15.0f;
    param.vPos = apos;
    param.vRot.set(-m_pAtkData->WORK1 * 0.017453292f, rot.y, 0.0f);
    param.pEffectTbl = PL::g_EffTbl_Skill[m_pAtkData->EFFECT];
    param.fCollRadius = m_pAtkData->COLL_START_FRAME * 0.01f;
    param.tqExplodeTime = 0;
    CBullet* pBlitz = (CBullet*)CEntityFactory::getInstance()->createBullet(param, this);
    if (pBlitz) {
        FLOAT s = m_pAtkData->WORK2 * 0.1f;
        rot.x = s; rot.y = s; rot.z = s;
        pBlitz->setScale(rot);
    }
    return pBlitz->getHandle();
}

INT CFriendDonald::checkBlitzHit(INT inFlag) {
    CBullet* pBlitz = m_hLockon > 0 ? (CBullet*)CTaskMan::getTask(m_hLockon) : 0;
    if (!pBlitz) return 0;
    if (inFlag > 0) {
        if (m_uiFlag & 0x10000) return 1;
        if (pBlitz->calcDistance(m_pPlayer->getCenterPos()) > 1.25f) return 0;
    } else {
        if (pBlitz->isGround()) return 0;
        FVECTOR wpos;
        m_pPlayer->getWeaponTopPos(wpos);
        AddVectorXYZ(wpos, wpos, ((CTask*)m_pPlayer->getWeapon())->Crd(0).getWorldMatrix().v[3]);
        ScaleVectorXYZ(wpos, wpos, 0.5f);
        if (pBlitz->calcDistance(wpos) > 1.25f) return 0;
    }
    m_uiFlag |= 0x10802;
    return 1;
}

void CFriendDonald::setStardust(INT inNo) {
    FVECTOR pos = m_vAimPos;
    FVECTOR rot = m_pPlayer->getRot();
    BULLET_PARAM param;
    PATK_DATA* pAtkData;
    INT i;
    FVECTOR dir;
    FLOAT scale = m_pAtkData->WORK2 * 0.05f;
    CEffect* effect = CEffectMan::getEffect(setEffect(PL::g_EffTbl_HitMark[8].pEffectName, 5, &pos, &rot, 0.0f));
    if (effect) effect->setScale(scale, scale, scale);
    pAtkData = &PL::g_AttackDataList[m_uiAttackID + inNo + 1];
    param.init(2, 3);
    param.CollKind = COLL_KIND_PSHOT;
    param.AtkKind = m_pAtkData->DAMAGE;
    param.AtkAttr = m_AtkAttr;
    param.iAttackPoint = m_iAttackPower;
    param.vRot = rot;
    param.fMaxDist = 15.0f;
    param.fCollRadius = pAtkData->WORK1 * 0.01f;
    param.pEffectTbl = PL::g_EffTbl_Skill[3];
    param.fSpeed = pAtkData->WORK0 * 0.01f;
    param.vPos = pos;
    param.tqExplodeTime = 1000;
    param.vPos.w = param.fCollRadius * 2.0f;
    CBullet* pBullet = (CBullet*)CEntityFactory::getInstance()->createBullet(param, this);
    if (pBullet) param.uiAttackID = pBullet->getAttackID();
    switch (inNo) {
    case 0:
        for (i = 0; i < 3; i++) {
            dir.x = (2 - (i * 2) % 3) * pAtkData->WORK2 * 0.1f * 0.017453292f;
            dir.y = (pAtkData->WORK2 + i * pAtkData->WORK2 * 0.2f) * 0.017453292f;
            param.vRot.x = _adjustRad(rot.x - dir.x);
            param.vRot.y = _adjustRad(rot.y + dir.y);
            CEntityFactory::getInstance()->createBullet(param, this);
            param.vRot.y = _adjustRad(rot.y - dir.y);
            CEntityFactory::getInstance()->createBullet(param, this);
        }
        break;
    case 1:
        for (i = 0; i < 6; i++) {
            dir.w = i * 6.2831855f / 6.0f;
            FLOAT ofs = pAtkData->WORK2 * 0.5f * 0.017453292f;
            dir.x = sinf(dir.w) * pAtkData->WORK2 * 0.01f - ofs;
            dir.y = cosf(dir.w) * pAtkData->WORK2 * 0.01f;
            param.vRot.x = _adjustRad(rot.x + dir.x);
            param.vRot.y = _adjustRad(rot.y + dir.y);
            CEntityFactory::getInstance()->createBullet(param, this);
        }
        break;
    case 2:
        for (i = 0; i < 3; i++) {
            dir.y = ((FLOAT)pAtkData->WORK2 + i * pAtkData->WORK2) * 0.017453292f;
            param.vRot.y = _adjustRad(rot.y + dir.y);
            CEntityFactory::getInstance()->createBullet(param, this);
            param.vRot.y = _adjustRad(rot.y - dir.y);
            CEntityFactory::getInstance()->createBullet(param, this);
        }
        break;
    }
}

void CFriendDonald::beginStateAction3() {
    m_iState = 4;
    m_iSubState = 1;
    m_uiAttackID = 104;
    m_pAtkData = &PL::g_AttackDataList[m_uiAttackID];
    m_uiFlag |= 8;
    setDispOffFlag(0xa, 0);
    m_pMove->enableGravity(0);
    clearSpeed();
}

void CFriendDonald::stateAction3() {
    if (exeDonaldWildCrusher()) setExit(0);
}

INT CFriendDonald::exeDonaldWildCrusher() {
    switch (m_iSubState) {
    case 1:
        adjustPlayer(1);
        if (m_pPlayer->isConditionFlag(0x80000)) {
            m_iSubState = 7;
            changeMotion(10, 0, 0, 0, 0, 0, 0);
            m_uiFlag |= 8;
            userBodyCollOff(-1);
            userBodyCollOffBody(-1);
        }
        break;
    case 7:
        adjustPlayer(1);
        if (isMotionEnd()) {
            m_iSubState = 8;
            changeMotion(11, 0, 0, -1, 0, 0, 0);
        }
        break;
    case 8:
        if (!m_pPlayer->isConditionFlag(0x80000)) {
            m_iSubState = 10;
            m_uiFlag = (INT)m_uiFlag & ~8;
            userBodyCollOn(-1);
            userBodyCollOnBody(-1);
            clearSpeed();
            adjustPlayer(0);
            changeMotion(12, 0, 1, -1, 0, 0, 0);
            setOldPos(m_pPlayer->getCenterPos());
        } else {
            adjustPlayer(1);
        }
        break;
    case 10:
        if (isMotionEnd()) return 1;
        break;
    }
    return 0;
}

void CFriendDonald::adjustPlayer(INT inWarp) {
    FVECTOR pos = { { -0.8f, 0.0f, -0.7f, 1.0f } };
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
