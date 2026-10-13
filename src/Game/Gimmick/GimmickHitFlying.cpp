#define TASK_H_NO_SLC_OVERFLOW_MSG
#define GPRIM_H_PATH "System/Graphic/GPrim.h"
#include "Game/Gimmick/GimmickHitFlying.h"
#include "Game/BodyCollision.h"
#include "System/Graphic/Effect/effect.h"
#include "System/Graphic/Gprim.h"
#include "System/Graphic/2d/2DMan.h"
#include "Game/CardBattle.h"
#include "System/Graphic/Str.h"
#include "Game/Collision/CollUtil.h"

/* Placeholders: three empty strings follow CollUtil.h's strings in .rodata (as in GimmickWindowLid/Bell); which
 * headers they come from is not known. */
inline void _emptyStringPlaceholder0() {
    TRACE("");
}
inline void _emptyStringPlaceholder1() {
    TRACE("");
}
inline void _emptyStringPlaceholder2() {
    TRACE("");
}

#include "Game/Movement.h"
#include "Game/AttackMotion.h"
#include "Game/Gimmick/GimmickManager.h"
#include "Game/LockOnManager.h"
#include "Game/Player/Player.h"
#include "System/Graphic/Camera.h"

void Normalize(FVECTOR& v0, const FVECTOR& v1);
void ScaleVector(FVECTOR& v0, const FVECTOR& v1, FLOAT t);

CGimmickHitFlying::CGimmickHitFlying(CTask* inParent) : CBaseGimmick(inParent) {
    m_Stat = 0;
    m_FlyDir.x = 0.0f;
    m_FlyDir.y = 0.0f;
    m_FlyDir.z = 0.0f;
    m_FlyDir.w = 1.0f;
    m_WaitCounter = 0.0f;
}

CGimmickHitFlying::~CGimmickHitFlying() {
    ((CGimmickManager*)getSysTask(17))->setStorageGimmickState(getUniqueID(), m_Stat);
}

BOOL CGimmickHitFlying::initalize(INT inGroupID, INT inRsrcID) {
    if (!CBaseGimmick::initalize(inGroupID, inRsrcID)) return FALSE;
    m_pMove->enableGravity(FALSE);
    m_pMove->setFlag(1, TRUE);
    m_pMove->setCollisionFlag(0xF, FALSE);
    bodyCollOffBody(-1);
    rideModelEnableAllCollision(FALSE);
    getSysTask(32);
    LockOnManager::CLockOnManager::registerLockOnObject(getHandle());
    return TRUE;
}

void CGimmickHitFlying::AfterInit() {
    m_Stat = ((CGimmickManager*)getSysTask(17))->getStorageGimmickState(getUniqueID());
    if (m_Stat == -1) m_Stat = 0;
    if (m_Stat == 0) {
        changeMotion(1, 0, FALSE, -1, 0, 0);
        bodyCollOnBody(-1);
        bodyCollOffBody(1);
    }
}

void CGimmickHitFlying::beginStateMain() {}

void CGimmickHitFlying::stateMain() {
    switch (m_Stat) {
    case 1:
        m_WaitCounter -= getFrameRateF();
        if (m_WaitCounter <= 0.0f) m_WaitCounter = 0.0f;
        if (moveFly()) {
            if (m_WaitCounter == 0.0f) {
                changeMotion(2, 0, FALSE, -1, 0, 0);
                m_pAttack->setCollKind(0);
                createPrize(0, 0);
                m_Stat = 2;
                setEntityFlag(1, 0);
            }
        }
        break;
    case 2:
        out();
        break;
    case 0:
        break;
    }
}

void CGimmickHitFlying::endStateMain() {}

void CGimmickHitFlying::beginStateHitAttack() {
    if (m_pBodyColl->getHitCollBody()->getDamageKind() == ATK_KIND_DMG_BIG) {
        FlyGimmick();
        m_Stat = 1;
        m_WaitCounter = 5.0f;
        saveStat(1);
    }
    changeGimmickState(0);
}

void CGimmickHitFlying::stateHitAttack() {}

void CGimmickHitFlying::endStateHitAttack() {}

void CGimmickHitFlying::FlyGimmick() {
    FMATRIX mat;
    FVECTOR vec;
    FVECTOR rot;

    mat = g_pPlayer->Crd(0).getWorldMatrix();
    vec.x = mat.v[2].x;
    vec.y = mat.v[2].y;
    vec.z = mat.v[2].z;
    Normalize(vec, vec);
    ScaleVector(vec, vec, 0.35f);
    vec.y += 0.1f;
    m_FlyDir = vec;
    m_pMove->enableGravity(TRUE);
    m_pMove->setFlag(1, FALSE);
    bodyCollOffBody(0);
    bodyCollOnBody(1);
    changeGimmickState(0);
}

BOOL CGimmickHitFlying::moveFly() {
    FVECTOR v;
    FVECTOR pos;

    if (m_WaitCounter == 0.0f &&
        ((m_pMove->getCollisionFlag() & 0xF) || m_pAttack->test() || (getGimmickFlag() & 1) || (getGimmickFlag() & 2)))
        return TRUE;
    getSysTask(17);
    pos = getPos();
    ScaleVector(v, m_FlyDir, getFrameRateF());
    pos.x += v.x;
    pos.y += v.y;
    pos.z += v.z;
    pos.w += v.w;
    setPos(pos);
    return FALSE;
}
// SOURCE DATA: .rodata
