
#define GPRIM_H_PATH "System/Graphic/GPrim.h" /* MATCH: this unit names Gprim.h so (header default "System/Graphic/Gprim.h") */
#include "Game/Gimmick/GimmickSavePoint.h"
#include "System/Graphic/Gprim.h"
#include "System/Graphic/2d/2DMan.h"
#include "Game/CardBattle.h"
#include "System/Graphic/Str.h"
#include "Game/Collision/CollUtil.h"

/* MATCH: Placeholders: three empty strings, then System/TaskMan.h's container overflow message, follow
 * CollUtil.h's strings in .rodata; which headers they come from is not known. */
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



#include "System/Vector.h"

#include "Game/Movement.h"
#include "Game/LockOnManager.h"
#include "Game/Player/Player.h"
#include "Game/GameWork.h"
#include "Game/Game.h"
#include "System/TaskMan.h"
#include "System/PadHandler.h"

CGimmickSavePoint::CGimmickSavePoint(CTask* inParent) : CBaseGimmick(inParent) {
    m_Step = 0;
    m_EfxFlag = 0;
    m_reqFlag = 0;
}

CGimmickSavePoint::~CGimmickSavePoint() {}

BOOL CGimmickSavePoint::initalize(INT inGroupID, INT inRsrcID) {
    if (!CBaseGimmick::initalize(inGroupID, inRsrcID)) return FALSE;
    m_pMove->enableGravity(FALSE);
    m_pMove->setFlag(1, 0);
    m_pMove->setCollisionFlag(0, 0xF);
    m_pMove->setFlag(8, 0);
    rideModelEnableAllCollision(FALSE);
    setEntityFlag(1, 0);
    changeMotion(1, 0, FALSE, -1, 0, 0);
    return TRUE;
}

void CGimmickSavePoint::beginStateMain() {}

void CGimmickSavePoint::stateMain() {
    INT handle;
    CTask* lockTaget;

    if (m_reqFlag == 1) {
        m_reqFlag = 0;
        CGameWork::setFlag(1, 5, FALSE);
    }
    if (calcDistanceXZ(g_pPlayer) < 0.8f && g_pPlayer->getPos().y - getPos().y < 0.2f) {
        if (m_Step == 0) {
            getSysTask(32);
            LockOnManager::CLockOnManager::registerLockOnObject(getHandle());
            if (g_pPlayer->getHP() != g_pPlayer->getMaxHP() && m_EfxFlag) {
                setEffect("rec_svpt.dpx", 1, &getPos(), __null, 0.0f);
            }
            g_pPlayer->addHP(g_pPlayer->getMaxHP(), 0, 0);
            if (!m_EfxFlag) {
                setEffect("rec_svpt.dpx", 1, &getPos(), __null, 0.0f);
                m_EfxFlag = 1;
            }
            m_Step = 1;
        }
        handle = g_pPlayer->getLockonHandle();
        lockTaget = CTaskMan::getTask(handle);
        if (lockTaget && lockTaget->getTypeID() == 0x2004 && CPadHandler::isTrgTalk()) {
            if (g_pPlayer->getPlayerState() == START_WAIT || g_pPlayer->getPlayerState() == STAND ||
                g_pPlayer->getPlayerState() == WALK || g_pPlayer->getPlayerState() == RUN) {
                m_reqFlag = 1;
                CGameWork::setFlag(1, 5, TRUE);
                CGame::reqSave();
            }
        }
    } else if (m_Step == 1) {
        setEntityFlag(0, 0x10000);
        g_pPlayer->cancelLockon();
        m_Step = 0;
    }
}

void CGimmickSavePoint::endStateMain() {}

BOOL CGimmickSavePoint::isRideOnSavePoint() {
    return isRideOnPlayer() != FALSE;
}

INT CGimmickSavePoint::getTypeID() const {
    return 0x2004;
}
// SOURCE DATA: .rodata
