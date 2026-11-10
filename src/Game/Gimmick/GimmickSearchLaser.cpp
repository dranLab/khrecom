
#define GPRIM_H_PATH "System/Graphic/GPrim.h" /* MATCH: this unit names Gprim.h so (header default "System/Graphic/Gprim.h") */
#include "Game/Gimmick/GimmickSearchLaser.h"
#include "Game/BodyCollision.h"
#include "System/Graphic/Effect/effect.h"
#include "System/Graphic/Gprim.h"
#include "System/Graphic/2d/2DMan.h"
#include "Game/CardBattle.h"
#include "System/Graphic/Str.h"
#include "Game/Collision/CollUtilA.h"
#include "Game/Collision/CollUtil.h"

/* MATCH: Placeholders: three empty strings, then System/TaskMan.h's container overflow message, follow
 * CollUtil.h's strings in .rodata (as in GimmickPlantParent); which headers they come from is not known. */
inline void _emptyStringPlaceholder0() {
    TRACE("");
}
inline void _emptyStringPlaceholder1() { /* MATCH: placeholder: an inline function's strings, in the original .rodata order */
    TRACE("");
}
inline void _emptyStringPlaceholder2() { /* MATCH: placeholder: an inline function's strings, in the original .rodata order */
    TRACE("");
}
#include "System/TaskMan.h"



#include "Game/Collision/CollObj.h"
#include "System/Graphic/Effect/EffectMan.h"
#include "Game/Movement.h"
#include "Game/AttackMotion.h"
#include "Game/Gimmick/GimmickManager.h"
#include "Game/LockOnManager.h"
#include "Game/EntityFactory.h"
#include "Game/Player/Player.h"
#include "System/TaskMan.h"
#include "System/Rnd.h"
#include "include_asm.h"
#include "Game/Collision/CollBody.h"













#include "Game/Gimmick/GimmickManager.h"






// System/Coord.h
void AddVector(FVECTOR& v0, const FVECTOR& v1, const FVECTOR& v2);
void SubVector(FVECTOR& v0, const FVECTOR& v1, const FVECTOR& v2);
void ScaleVector(FVECTOR& v0, const FVECTOR& v1, FLOAT f);
void Normalize(FVECTOR& v0, const FVECTOR& v1);
void OuterProduct(FVECTOR& v0, const FVECTOR& v1, const FVECTOR& v2);
FLOAT InnerProduct(const FVECTOR& v0, const FVECTOR& v1);
void ApplyMatrix(FVECTOR& v0, const FMATRIX& m0, const FVECTOR& v1);
void ApplyMatrixXYZ(FVECTOR& v0, const FMATRIX& m0, const FVECTOR& v1);

static inline void _unitMatrix(FMATRIX& m) {
    sceVu0UnitMatrix(m.m);
}
static inline void _rotMatrixY(FMATRIX& m0, FMATRIX& m1, float ry) {
    sceVu0RotMatrixY(m0.m, m1.m, ry);
}

static inline void _unitMatrixVu0(FMATRIX& m) {
    asm volatile("vmr32.xyzw $vf1, $vf0\n"
                 "sqc2 $vf0, 0x30(%0)\n"
                 "vmr32.xyzw $vf2, $vf1\n"
                 "sqc2 $vf1, 0x20(%0)\n"
                 "vmr32.xyzw $vf3, $vf2\n"
                 "sqc2 $vf2, 0x10(%0)\n"
                 "sqc2 $vf3, 0x0(%0)\n"
                 :
                 : "r"(&m)
                 : "memory");
}

CGimmickSearchLaser::CGimmickSearchLaser(CTask* inParent) : CBaseGimmick(inParent) {
    m_Step = 0;
    m_hChild[0] = -1;
    m_hChild[1] = -1;
    m_hChild[2] = -1;
    m_hChild[3] = -1;
}

CGimmickSearchLaser::~CGimmickSearchLaser() {
    ((CGimmickManager*)getSysTask(17))->setStorageGimmickState(getUniqueID(), m_Stat);
}

BOOL CGimmickSearchLaser::initalize(INT inGroupID, INT inRsrcID) {
    if (!CBaseGimmick::initalize(inGroupID, inRsrcID)) return FALSE;
    m_pMove->enableGravity(FALSE);
    m_pMove->setFlag(1, 0);
    m_pMove->setCollisionFlag(0, 0xF);
    m_pMove->setFlag(8, 0);
    bodyCollOffBody(-1);
    getSysTask(32);
    LockOnManager::CLockOnManager::registerLockOnObject(getHandle());
    return TRUE;
}

void CGimmickSearchLaser::AfterInit() {
    CEntity* entity;
    INT i;

    m_Stat = ((CGimmickManager*)getSysTask(17))->getStorageGimmickState(getUniqueID());
    if (m_Stat == -1) m_Stat = 0;
    if (m_Stat) return;
    changeMotion(1, 0, FALSE, 0, 0, 0);
    bodyCollOnBody(-1);
    getSysTask(17);
    for (i = 0; i < 4; i++) {
        entity = CEntityFactory::getInstance()->createEntity(11, 0x521, getSysTask(17), NULL);
        entity->warp(getPos());
        entity->setRot(getRot());
        m_hChild[i] = entity->getHandle();
        ((CGimmickSearchLaserCircle*)CTaskMan::getTask(m_hChild[i]))->m_ParentPos = getPos();
    }
}

void CGimmickSearchLaser::beginStateMain() {
    if (m_Stat) out();
}

void CGimmickSearchLaser::stateMain() {
    FLOAT y;
    INT i;

    switch (m_Step) {
    case 0:
        m_Wait = 90.0f;
        m_Step = 1;
        break;
    case 5:
        if (calcDistanceXZ(g_pPlayer) <= 8.0f) {
            y = g_pPlayer->getPos().y - getPos().y;
            if (y <= 2.0f && y >= -2.0f) {
                for (i = 0; i < 4; i++) {
                    CGimmickSearchLaserCircle* pCircle = (CGimmickSearchLaserCircle*)CTaskMan::getTask(m_hChild[i]);
                    if (pCircle && pCircle->changeStepAttack()) {
                        m_Wait = 100.0f;
                        m_Step = 10;
                        break;
                    }
                }
            }
        }
        break;
    case 1:
    case 10:
        m_Wait -= getFrameRateF();
        if (m_Wait <= 0.0f) m_Step = 5;
        break;
    case 15:
        break;
    }
}

void CGimmickSearchLaser::endStateMain() {}

void CGimmickSearchLaser::beginStateHitAttack() {
    INT i;

    if (m_pBodyColl->getHitCollBody()->getDamageKind() == 2) {
        DropPrize prize;
        prize.StockKind = 0;
        prize.Kind = CARD_KIND_None;
        prize.Num = 0;
        prize.No = 0;
        if (!m_Stat) createPrize(0, 0);
        changeMotion(2, 0, FALSE, -1, 0, 0);
        setEntityFlag(1, 0);
        for (i = 0; i < 4; i++) {
            ((CGimmickSearchLaserCircle*)CTaskMan::getTask(m_hChild[i]))->killLaser();
        }
        m_Stat = 1;
        saveStat(1);
    }
    changeGimmickState(0);
}

void CGimmickSearchLaser::stateHitAttack() {}

void CGimmickSearchLaser::endStateHitAttack() {}

CGimmickSearchLaserCircle::CGimmickSearchLaserCircle(CTask* inParent) : CBaseGimmick(inParent) {
    m_Step = 0;
}

CGimmickSearchLaserCircle::~CGimmickSearchLaserCircle() {}

BOOL CGimmickSearchLaserCircle::initalize(INT inGroupID, INT inRsrcID) {
    return CBaseGimmick::initalize(inGroupID, inRsrcID) != 0;
}

void CGimmickSearchLaserCircle::AfterInit() {
    setEntityFlag(1, 0);
    changeMotion(1, 0, FALSE, 0, 0, 0);
    m_pMove->setRadius(1.0f);
}

void CGimmickSearchLaserCircle::beginStateMain() {}

void CGimmickSearchLaserCircle::stateMain() {
    CEffect* efx;
    GimParam* param;

    switch (m_Step) {
    case 0:
        break;
    case 1: {
        {
            FVECTOR v;
            FVECTOR v2;
            FMATRIX m;
            v.x = 0.0f;
            v.y = 0.0f;
            v.z = 3.0f;
            v.w = 1.0f;
            _unitMatrix(m);
            _rotMatrixY(m, m, (RND::genrand_int31() % 360 - 180) * 0.017453292f);
            ApplyMatrix(v, m, v);
            AddVector(v, m_ParentPos, v);
            CSphereCollision cColl;
            cColl.setRadius(0.5f);
            cColl.setStartPos(v);
            cColl.setEndPos(m_ParentPos);
            if (cColl.collideMap()) break;
            warp(v);
            m_pMove->enableGravity(TRUE);
            m_pMove->land(5.0f, 1.0f, FALSE);
            HANDLE h = setEffect("rec_ca_common.dpx", 0, &getPos(), NULL, 0.0f);
            m_Step = 2;
            m_hEfx = h;
            m_Wait = 60.0f;
            m_Speed = 0.6f;
        }
        m_KillFlag = 0;
        m_MoveStep = 0;
        break;
    }
    case 2:
        if (!CEffectMan::getEffect(m_hEfx)) {
            m_hEfx = attachEffect("rec_ca_common.dpx", 1, &Crd(0), NULL, NULL, 0.0f);
            m_Step = 3;
            m_Wait = 30.0f;
        }
        break;
    case 3:
        if (m_KillFlag) goto kill;
        m_Wait -= getFrameRateF();
        if (m_Wait <= 0.0f) {
            m_PlTarget = setMoveTargetPos();
            m_Step = 5;
        }
        break;
    case 5:
        if (m_KillFlag) goto kill;
        if (moveTargetPos()) {
            if (m_PlTarget) {
                m_Step = 4;
            } else {
                m_Step = 3;
                m_Wait = 30.0f;
            }
        }
        break;
    case 4:
        if (calcDistanceXZ(g_pPlayer) <= 0.5f) {
            m_Step = 6;
            CEffectMan::getEffect(m_hEfx)->kill();
            m_hEfx = setEffect("rec_ca_common.dpx", 2, &getPos(), NULL, 0.0f);
            break;
        }
    kill:
        CEffectMan::getEffect(m_hEfx)->kill();
        m_hEfx = setEffect("rec_ca_common.dpx", 2, &getPos(), NULL, 0.0f);
        m_Step = 9;
        break;
    case 6:
        CEffectMan::getEffect(m_hEfx)->kill();
        m_hEfx = attachEffect("rec_ca_common.dpx", 3, &Crd(0), NULL, NULL, 0.0f);
        m_Wait = 15.0f;
        m_Step = 7;
        break;
    case 7:
        m_Wait -= getFrameRateF();
        if (m_Wait <= 0.0f) {
            changeMotion(2, 0, FALSE, 0, 0, 0);
            if (m_pAttack && (param = getGimParam())) {
                m_pAttack->setAttackParam(param->AtkPow, (ATK_ATTR)param->AtkAttr);
            }
            m_Step = 8;
            m_Wait = 18.0f;
        }
        break;
    case 8:
        m_Wait -= getFrameRateF();
        if (m_Wait <= 0.0f) m_Step = 9;
        break;
    case 9:
        efx = CEffectMan::getEffect(m_hEfx);
        changeMotion(1, 0, FALSE, 0, 0, 0);
        if (!efx) m_Step = 0;
        break;
    }
}

void CGimmickSearchLaserCircle::endStateMain() {}

BOOL CGimmickSearchLaserCircle::changeStepAttack() {
    if (m_Step == 0) {
        m_Step = 1;
        return TRUE;
    }
    return FALSE;
}

BOOL CGimmickSearchLaserCircle::followPlayer() {
    FVECTOR v = g_pPlayer->getPos();
    if (calcDistanceXZ(g_pPlayer) <= 0.13f) {
        v.y = getPos().y;
        setPos(v);
        return TRUE;
    }
    SubVector(v, v, getPos());
    v.y = 0.0f;
    Normalize(v, v);
    ScaleVector(v, v, getFrameRateF() * 0.13f);
    AddVector(v, getPos(), v);
    setPos(v);
    return FALSE;
}

BOOL CGimmickSearchLaserCircle::setMoveTargetPos() {
    switch (m_MoveStep) {
    case 0: {
        m_MoveStep = 10;
        FVECTOR v;
        FVECTOR v2;
        FVECTOR dirPl;
        FMATRIX m;
        _unitMatrixVu0(m);
        sceVu0RotMatrixY(m.m, m.m, g_pPlayer->getRot().y);
        v.x = 0.0f;
        v.y = 0.0f;
        v.z = 1.0f;
        v.w = 1.0f;
        SubVector(dirPl, g_pPlayer->getPos(), getPos());
        Normalize(dirPl, dirPl);
        ApplyMatrixXYZ(v, m, v);
        v2.set(0.0f, 1.0f, 0.0f);
        OuterProduct(v, v, v2);
        if (InnerProduct(v, dirPl) <= 0.0f) {
            ScaleVector(v, v, (RND::genrand_int31() % 2000) / 1000.0f + 2.0f);
        } else {
            ScaleVector(v, v, -((RND::genrand_int31() % 2000) / 1000.0f) - 2.0f);
        }
        v2.set(0.0f, 0.0f, (RND::genrand_int31() % 2000) / 1000.0f + 2.0f);
        ApplyMatrixXYZ(v2, m, v2);
        AddVector(v, v2, v);
        AddVector(m_TargetPos, v, g_pPlayer->getPos());
        break;
    }
    case 10: {
        m_MoveStep = 10;
        FVECTOR v;
        FVECTOR v2;
        FVECTOR dirPl;
        FMATRIX m;
        _unitMatrixVu0(m);
        sceVu0RotMatrixY(m.m, m.m, g_pPlayer->getRot().y);
        v.x = 0.0f;
        v.y = 0.0f;
        v.z = 1.0f;
        v.w = 1.0f;
        SubVector(dirPl, g_pPlayer->getPos(), getPos());
        Normalize(dirPl, dirPl);
        ApplyMatrixXYZ(v, m, v);
        v2.set(0.0f, 1.0f, 0.0f);
        OuterProduct(v, v, v2);
        if (InnerProduct(v, dirPl) <= 0.0f) {
            ScaleVector(v, v, -((RND::genrand_int31() % 2000) / 1000.0f) - 1.0f);
        } else {
            ScaleVector(v, v, (RND::genrand_int31() % 2000) / 1000.0f + 1.0f);
        }
        v2.set(0.0f, 0.0f, (RND::genrand_int31() % 2000) / 1000.0f + 2.0f);
        ApplyMatrixXYZ(v2, m, v2);
        AddVector(v, v2, v);
        AddVector(m_TargetPos, v, g_pPlayer->getPos());
        m_MoveStep = 20;
        break;
    }
    case 20: {
        FVECTOR v = g_pPlayer->getPos();
        v.y = getPos().y;
        m_TargetPos = v;
        m_MoveStep = 30;
        m_Speed = 0.8f;
        m_NextWait = 40.0f;
        return TRUE;
    }
    default:
        m_MoveStep = 0;
        return TRUE;
    }
    m_NextWait = 40.0f;
    return FALSE;
}

BOOL CGimmickSearchLaserCircle::moveTargetPos() {
    FVECTOR v = m_TargetPos;
    if (calcDistanceXZ(m_TargetPos) <= getFrameRateF() * m_Speed) {
        setPos(v);
        return TRUE;
    }
    SubVector(v, v, getPos());
    v.y = 0.0f;
    Normalize(v, v);
    ScaleVector(v, v, getFrameRateF() * m_Speed);
    AddVector(v, getPos(), v);
    setPos(v);
    m_NextWait -= getFrameRateF();
    if (m_NextWait <= 0.0f) return TRUE;
    return FALSE;
}

void CGimmickSearchLaserCircle::killLaser() {
    m_KillFlag = 1;
}

// SOURCE DATA: .rodata
