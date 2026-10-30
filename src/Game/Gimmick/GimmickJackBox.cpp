#define TASK_H_NO_SLC_OVERFLOW_MSG
#define GPRIM_H_PATH "System/Graphic/GPrim.h"
#include "Game/Gimmick/GimmickJackBox.h"
#include "Game/BodyCollision.h"
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

#include "System/Graphic/modeldata.h"
#include "System/Graphic/Effect/effect.h"
#include "Game/Movement.h"
#include "Game/Gimmick/GimmickManager.h"
#include "Game/LockOnManager.h"
#include "System/Rnd.h"

CGimmickJackBox::CGimmickJackBox(CTask* inParent) : CBaseGimmick(inParent) {
    m_Step = 0;
    m_Stat = 0;
}

CGimmickJackBox::~CGimmickJackBox() {
    ((CGimmickManager*)getSysTask(17))->setStorageGimmickState(getUniqueID(), m_Stat);
}

BOOL CGimmickJackBox::initalize(INT inGroupID, INT inRsrcID) {
    if (!CBaseGimmick::initalize(inGroupID, inRsrcID)) return FALSE;
    getSysTask(32);
    LockOnManager::CLockOnManager::registerLockOnObject(getHandle());
    m_pMove->enableGravity(FALSE);
    m_pMove->setFlag(1, 0);
    m_pMove->setCollisionFlag(0, 0xF);
    m_pMove->setFlag(8, 0);
    return TRUE;
}

void CGimmickJackBox::AfterInit() {
    m_Stat = ((CGimmickManager*)getSysTask(17))->getStorageGimmickState(getUniqueID());
    if (m_Stat == -1) m_Stat = 0;
    if (m_Stat == 2) {
        changeMotion(3, 0, FALSE, 0, 0, 0);
        bodyCollOffBody(1);
    } else if (m_Stat == 1) {
        changeMotion(5, 0, FALSE, 0, 0, 0);
    } else {
        changeMotion(1, 0, FALSE, 0, 0, 0);
        bodyCollOffBody(1);
    }
}

void CGimmickJackBox::beginStateMain() {}

void CGimmickJackBox::stateMain() {
    switch (m_Step) {
    case 1:
        if (isMotionEnd()) {
            changeMotion(3, 0, FALSE, -1, 0, 0);
            m_Step = 0;
        }
        break;
    case 2:
        if (isMotionEnd()) {
            changeMotion(5, 0, FALSE, -1, 0, 0);
            m_Step = 0;
        }
        break;
    }
}

void CGimmickJackBox::endStateMain() {}

void CGimmickJackBox::beginStateHitAttack() {
    if (m_Stat == 0) {
        if (RND::genrand_int31() % 100 < 50) {
            m_Step = 1;
            m_Stat = 2;
            saveStat(2);
            changeMotion(2, 0, FALSE, -1, 0, 0);
            createPrize(0, 0);
        } else {
            m_Step = 2;
            m_Stat = 1;
            bodyCollOnBody(1);
            saveStat(m_Stat);
            changeMotion(4, 0, FALSE, -1, 0, 0);
        }
    }
    changeGimmickState(0);
}

void CGimmickJackBox::stateHitAttack() {}

void CGimmickJackBox::endStateHitAttack() {}
// SOURCE DATA: .rodata
