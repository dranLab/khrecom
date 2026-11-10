
#define GPRIM_H_PATH "System/Graphic/GPrim.h" /* MATCH: this unit names Gprim.h so (header default "System/Graphic/Gprim.h") */
#include "Game/Gimmick/GimmickThunderTrap.h"
#include "Game/BodyCollision.h"
#include "System/Graphic/Gprim.h"
#include "System/Graphic/2d/2DMan.h"
#include "Game/CardBattle.h"
#include "System/Graphic/Str.h"
#include "Game/Collision/CollUtil.h"

/* MATCH: Placeholders: three empty strings follow CollUtil.h's strings in .rodata (as in GimmickJackBox/Bell); which
 * headers they come from is not known. */
inline void _emptyStringPlaceholder0() {
    TRACE("");
}
inline void _emptyStringPlaceholder1() { /* MATCH: placeholder: an inline function's strings, in the original .rodata order */
    TRACE("");
}
inline void _emptyStringPlaceholder2() { /* MATCH: placeholder: an inline function's strings, in the original .rodata order */
    TRACE("");
}

#include "System/Graphic/modeldata.h"
#include "System/Graphic/Effect/effect.h"
#include "Game/Movement.h"
#include "Game/AttackMotion.h"
#include "Game/Gimmick/GimmickManager.h"
#include "Game/LockOnManager.h"
#include "Game/Player/Player.h"
#include "Game/Bullet/Bullet.h"
#include "Game/EntityFactory.h"
#include "System/Graphic/Camera.h"

extern "C" {
float atan2f(float y, float x);
}

CGimmickThunderTrap::CGimmickThunderTrap(CTask* inParent) : CBaseGimmick(inParent) {
    m_Step = 0;
    m_WaitCounter = 0.0f;
}

CGimmickThunderTrap::~CGimmickThunderTrap() {
    ((CGimmickManager*)getSysTask(17))->setStorageGimmickState(getUniqueID(), m_Stat);
}

BOOL CGimmickThunderTrap::initalize(INT inGroupID, INT inRsrcID) {
    if (!CBaseGimmick::initalize(inGroupID, inRsrcID)) return FALSE;
    getSysTask(32);
    LockOnManager::CLockOnManager::registerLockOnObject(getHandle());
    m_pMove->enableGravity(FALSE);
    m_pMove->setFlag(1, 0);
    m_pMove->setCollisionFlag(0, 0xF);
    m_pMove->setFlag(8, 0);
    return TRUE;
}

void CGimmickThunderTrap::AfterInit() {
    m_Stat = ((CGimmickManager*)getSysTask(17))->getStorageGimmickState(getUniqueID());
    if (m_Stat == -1) m_Stat = 0;
    changeMotion(1, 0, FALSE, 0, 0, 0);
}

void CGimmickThunderTrap::beginStateMain() {}

void CGimmickThunderTrap::stateMain() {
    switch (m_Step) {
    case 0:
        break;
    case 10:
        changeMotion(2, 0, FALSE, 0, 0, 0);
        m_Step = 20;
        break;
    case 20:
        if (isMotionEnd()) {
            setGimmickFlag(0x10, TRUE);
            changeMotion(3, 0, FALSE, 0, 0, 0);
            m_pAttack->setCollKind(0);
            m_Step = 25;
            m_WaitCounter = 60.0f;
        }
        break;
    case 25:
        m_WaitCounter -= getFrameRateF();
        if (m_WaitCounter <= 30.0f) {
            setGimmickFlag(0x10, FALSE);
            m_Step = 30;
        }
        break;
    case 30:
        m_WaitCounter -= getFrameRateF();
        if (m_WaitCounter <= 0.0f) {
            m_WaitCounter = 0.0f;
            changeMotion(4, 0, FALSE, 0, 0, 0);
            m_Step = 40;
        }
        break;
    case 40:
        if (isMotionEnd()) {
            changeMotion(1, 0, FALSE, 0, 0, 0);
            m_Step = 0;
        }
        break;
    }
}

void CGimmickThunderTrap::endStateMain() {}

void CGimmickThunderTrap::beginStateHitAttack() {
    if (m_Step == 0) m_Step = 10;
    if (m_Stat == 0) createPrize(0, 0);
    m_Stat = 1;
    saveStat(1);
    changeGimmickState(0);
}

void CGimmickThunderTrap::stateHitAttack() {}

void CGimmickThunderTrap::endStateHitAttack() {}

static BULLET_EFFECT_INFO g_GimThunderLaserEffTbl[4] = {
    {NULL, 0, 0, 0},
    {"rec_pc_common1", 9, 0, 0},
    {"rec_pc_common1", 9, 0, 0},
    {"rec_pc_common1", 8, 0, 0},
};

CGimmickThunderLaser::CGimmickThunderLaser(CTask* inParent) : CBaseGimmick(inParent) {
    m_Step = 0;
    m_WaitCounter = 0.0f;
}

CGimmickThunderLaser::~CGimmickThunderLaser() {
    ((CGimmickManager*)getSysTask(17))->setStorageGimmickState(getUniqueID(), m_Stat);
}

BOOL CGimmickThunderLaser::initalize(INT inGroupID, INT inRsrcID) {
    if (!CBaseGimmick::initalize(inGroupID, inRsrcID)) return FALSE;
    getSysTask(32);
    LockOnManager::CLockOnManager::registerLockOnObject(getHandle());
    m_pMove->enableGravity(FALSE);
    m_pMove->setFlag(1, 0);
    m_pMove->setCollisionFlag(0, 0xF);
    m_pMove->setFlag(8, 0);
    return TRUE;
}

void CGimmickThunderLaser::AfterInit() {
    m_Stat = ((CGimmickManager*)getSysTask(17))->getStorageGimmickState(getUniqueID());
    if (m_Stat == -1) m_Stat = 0;
    if (m_Stat) {
        changeMotion(3, 0, FALSE, 0, -1, 0);
        setEntityFlag(0, 0x10000);
        bodyCollOffBody(-1);
        bodyCollOff(-1);
        rideModelEnableChipCollision(0, FALSE);
        rideModelEnableChipCollision(1, TRUE);
    } else {
        changeMotion(1, 0, FALSE, 0, 0, 0);
    }
    m_WaitCounter = 90.0f;
}

void CGimmickThunderLaser::beginStateMain() {}

void CGimmickThunderLaser::stateMain() {
    if (m_Stat) return;
    switch (m_Step) {
    case 10:
        if (calcDistanceXZ(g_pPlayer) <= 8.0f) {
            FLOAT y = g_pPlayer->getPos().y - getPos().y;
            if (y <= 0.5f && -0.5f <= y) {
                m_hEfx = attachEffect("rec_pc_common1.dpx", 7, &Crd(getBoneIndex("prize", 0)), NULL, NULL, 0.0f);
                m_WaitCounter = 30.0f;
                m_Step = 15;
            }
        }
        break;
    case 15:
        m_WaitCounter -= getFrameRateF();
        if (m_WaitCounter <= 0.0f) m_Step = 20;
        break;
    case 20: {
        FVECTOR v;
        FVECTOR pos; // unused (reference has it; keeps the 0x110 stack frame)
        BULLET_PARAM param;
        v = g_pPlayer->getPos();
        v.x -= getPos().x;
        v.z -= getPos().z;
        v.y = atan2f(v.x, v.z);
        param.init(1, 3);
        param.AtkAttr = (ATK_ATTR)5;
        param.AtkKind = (ATK_KIND)10;
        param.fSpeed = 0.15f;
        param.iAttackPoint = 0;
        param.vPos = Crd(getBoneIndex("prize", 0)).getWorldMatrix().v[3];
        param.vRot.y = v.y;
        param.uiFlag = 0x1100140;
        param.pEffectTbl = g_GimThunderLaserEffTbl;
        param.fCollRadius = 0.2f;
        param.tqFadeTime = 1000;
        param.fMaxDist = 50.0f;
        param.vRot.x = 0.0f;
        param.vRot.z = 0.0f;
        CEntityFactory::getInstance()->createBullet(param, NULL);
        m_Step = 30;
        m_WaitCounter = 150.0f;
        break;
    }
    case 30:
        m_WaitCounter -= getFrameRateF();
        if (m_WaitCounter <= 0.0f) m_Step = 40;
        break;
    case 0:
    case 40:
        m_WaitCounter -= getFrameRateF();
        if (m_WaitCounter <= 0.0f) m_Step = 10;
        break;
    }
}

void CGimmickThunderLaser::endStateMain() {}

void CGimmickThunderLaser::beginStateHitAttack() {
    if (m_Stat == 0) {
        createPrize(0, 0);
        m_Stat = 1;
        saveStat(1);
        setEntityFlag(0, 0x10000);
        g_pPlayer->cancelLockon();
        changeMotion(3, 0, FALSE, 0, 0, 0);
        bodyCollOffBody(-1);
        bodyCollOff(-1);
        rideModelEnableChipCollision(0, FALSE);
        rideModelEnableChipCollision(1, TRUE);
    }
    changeGimmickState(0);
}

void CGimmickThunderLaser::stateHitAttack() {}

void CGimmickThunderLaser::endStateHitAttack() {}
// SOURCE DATA: .data .rodata
// SOURCE DATA SLUS_217.99_retail: .data .rodata
