#define TASK_H_NO_SLC_OVERFLOW_MSG
#define GPRIM_H_PATH "System/Graphic/GPrim.h"
#include "Game/Gimmick/GimmickWarpPoint.h"
#include "System/Graphic/Gprim.h"
#include "System/Graphic/2d/2DMan.h"
#include "Game/CardBattle.h"
#include "System/Graphic/Str.h"
#include "Game/Collision/CollUtil.h"

/* Placeholders: three empty strings and the task container overflow message (see System/Task.h) follow
 * CollUtil.h's strings in .rodata; which headers they come from is not known. */
inline void _emptyStringPlaceholder0() {
    TRACE("");
}
inline void _emptyStringPlaceholder1() {
    TRACE("");
}
inline void _emptyStringPlaceholder2() {
    TRACE("");
}
inline void _taskSLCOverflowPlaceholder() {
    TRACE("\x83\x5e\x83\x58\x83\x4e\x97\x70\x83\x52\x83\x93\x83\x65\x83\x69\x82\xaa MAX \x82\xc9\x93\x9e\x92\x42\x82\xb5\x82\xdc\x82\xb5\x82\xbd\n");
}

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

inline void* operator new(size_t, void* p) { return p; }

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
            char* msg = __FILE__ "(190)";
            UINT nRA;
            __asm__ volatile("daddu %0, $31, $0" : "=r"(nRA));
            CWorldWarp* p = (CWorldWarp*)com_malloc_d(sizeof(CWorldWarp), msg, nRA);
            if (p != NULL) p = new (p) CWorldWarp(CTaskMan::getSysTask(1), FALSE);
        }
    }
}

void CGimmickWarpPoint::endStateMain() {}

INT CGimmickWarpPoint::getTypeID() const {
    return 0x2005;
}
// SOURCE DATA: .rodata
