
#define GPRIM_H_PATH "System/Graphic/GPrim.h" /* MATCH: this unit names Gprim.h so (header default "System/Graphic/Gprim.h") */
#include "Game/Gimmick/GimmickWarpPoint.h"
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
#include "Game/SceneInfo.h"
#include "System/HeapMan.h"
#include "Form/Menu/WorldWarp.h"



CGimmickWarpPoint::CGimmickWarpPoint(CTask* inParent) : CBaseGimmick(inParent) {
    m_Step = 0;
    m_reqFlag = 0;
}

CGimmickWarpPoint::~CGimmickWarpPoint() {
    if (m_reqFlag) {
        CGameWork::setFlag(1, 5, FALSE);
    }
}

BOOL CGimmickWarpPoint::initalize(INT inGroupID, INT inRsrcID) {
    if (!CBaseGimmick::initalize(inGroupID, inRsrcID)) return FALSE;
    m_pMove->enableGravity(FALSE);
    m_pMove->setFlag(1, 0);
    m_pMove->setCollisionFlag(0, 0xF);
    m_pMove->setFlag(8, 0);
    changeMotion(2, 0, FALSE, 0, 0, 0);
    getSysTask(32);
    LockOnManager::CLockOnManager::registerLockOnObject(getHandle());
    return TRUE;
}

void CGimmickWarpPoint::AfterInit() {
    if (CGame::isSoraGame()) {
        if (!(CGameWork::getInt(0, 0xC2) & 8) && CSceneInfo::getCurrentFloorNumber() == 1) {
            out();
        }
    } else {
        if (CGameWork::getInt(0, 0x35) == -12) {
            out();
        }
    }
}

void CGimmickWarpPoint::beginStateMain() {
    if (CGame::isSoraGame() && !(CGameWork::getInt(0, 0xC2) & 8)) {
        setEntityFlag(0, 0x10000);
    }
}

void CGimmickWarpPoint::stateMain() {
    INT handle;
    CTask* lockTaget;

    if (m_reqFlag) {
        m_reqFlag = 0;
        CGameWork::setFlag(1, 5, FALSE);
    }
    if (!(getFlag() & 0x10000) && (CGameWork::getInt(0, 0xC2) & 8)) {
        setEntityFlag(0x10000, 0);
    }
    if (CSceneInfo::hasReqChangeScene()) return;
    handle = g_pPlayer->getLockonHandle();
    lockTaget = CTaskMan::getTask(handle);
    if (lockTaget && lockTaget->getTypeID() == 0x2005 && CPadHandler::isTrgTalk()) {
        if (g_pPlayer->getPlayerState() == START_WAIT || g_pPlayer->getPlayerState() == STAND ||
            g_pPlayer->getPlayerState() == WALK || g_pPlayer->getPlayerState() == RUN) {
            m_reqFlag = 1;
            CGameWork::setFlag(1, 5, TRUE);
            new (__FILE__ "(190)") CWorldWarp(CTaskMan::getSysTask(1), FALSE);
        }
    }
}

void CGimmickWarpPoint::endStateMain() {}

INT CGimmickWarpPoint::getTypeID() const {
    return 0x2005;
}
// SOURCE DATA: .rodata
