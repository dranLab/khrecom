
#define GPRIM_H_PATH "System/Graphic/GPrim.h" /* MATCH: this unit names Gprim.h so (header default "System/Graphic/Gprim.h") */
#include "Game/Gimmick/GimmickBell.h"
#include "Game/BodyCollision.h"
#include "System/Graphic/Gprim.h"
#include "System/Graphic/2d/2DMan.h"
#include "Game/CardBattle.h"
#include "System/Graphic/Str.h"
#include "Game/Collision/CollUtil.h"

/* Placeholders: three empty strings follow CollUtil.h's strings in .rodata (as in GimmickWindowLid); which
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

#include "System/Graphic/modeldata.h"
#include "System/Graphic/Effect/effect.h"
#include "Game/Movement.h"
#include "Game/Gimmick/GimmickManager.h"
#include "Game/LockOnManager.h"
#include "Game/Player/Player.h"
#include "System/Graphic/Camera.h"

void Normalize(FVECTOR& outV, const FVECTOR& inV);
FLOAT InnerProduct(const FVECTOR& inA, const FVECTOR& inB);

CGimmickBell::CGimmickBell(CTask* inParent) : CBaseGimmick(inParent) {
    m_Step = 0;
}

CGimmickBell::~CGimmickBell() {
    ((CGimmickManager*)getSysTask(17))->setStorageGimmickState(getUniqueID(), m_Stat);
}

BOOL CGimmickBell::initalize(INT inGroupID, INT inRsrcID) {
    if (!CBaseGimmick::initalize(inGroupID, inRsrcID)) return FALSE;
    getSysTask(32);
    LockOnManager::CLockOnManager::registerLockOnObject(getHandle());
    m_pMove->enableGravity(FALSE);
    m_pMove->setFlag(1, 0);
    m_pMove->setCollisionFlag(0, 0xF);
    m_pMove->setFlag(8, 0);
    return TRUE;
}

void CGimmickBell::AfterInit() {
    m_Stat = ((CGimmickManager*)getSysTask(17))->getStorageGimmickState(getUniqueID());
    if (m_Stat == -1) m_Stat = 0;
    changeMotion(1, getParam() * 50, FALSE, -1, 0, 0);
}

void CGimmickBell::beginStateMain() {}

void CGimmickBell::stateMain() {}

void CGimmickBell::endStateMain() {}

void CGimmickBell::beginStateHitAttack() {
    FVECTOR dir;
    FVECTOR v;
    FMATRIX m;

    if (m_Stat == 0) {
        createPrize(0, 0);
        m_Stat = 1;
        saveStat(1);
    }
    m = Crd(0).getWorldMatrix();
    v.x = m.v[2].x;
    v.y = m.v[2].y;
    v.z = m.v[2].z;
    v.w = 0.0f;
    Normalize(v, v);
    dir = g_pPlayer->Crd(0).getWorldMatrix().v[3];
    dir.x -= getPos().x;
    dir.z -= getPos().z;
    dir.y = 0.0f;
    dir.w = 0.0f;
    Normalize(dir, dir);
    if (InnerProduct(v, dir) <= 0.0f) {
        changeMotion(2, 0, FALSE, 0, 0, 0);
    } else {
        changeMotion(3, 0, FALSE, 0, 0, 0);
    }
}

void CGimmickBell::stateHitAttack() {
    if (isMotionEnd()) {
        changeMotion(1, 0, FALSE, 0, 0, 0);
        changeGimmickState(0);
    }
}

void CGimmickBell::endStateHitAttack() {}
// SOURCE DATA: .rodata
