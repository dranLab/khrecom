
#define GPRIM_H_PATH "System/Graphic/GPrim.h" /* MATCH: this unit names Gprim.h so (header default "System/Graphic/Gprim.h") */
#include "Game/Gimmick/GimmickWindowLid.h"
#include "System/Graphic/Gprim.h"
#include "System/Graphic/2d/2DMan.h"
#include "Game/CardBattle.h"
#include "System/Graphic/Str.h"
#include "Game/Collision/CollUtil.h"

/* MATCH: Placeholders: three empty strings follow CollUtil.h's strings in .rodata; which headers they come from
 * is not known. */
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
#include "Game/Player/Player.h"

CGimmickWindowLid::CGimmickWindowLid(CTask* inParent) : CBaseGimmick(inParent) {
    m_Step = 0;
    m_WaitCounter = 0.0f;
}

CGimmickWindowLid::~CGimmickWindowLid() {}

BOOL CGimmickWindowLid::initalize(INT inGroupID, INT inRsrcID) {
    if (!CBaseGimmick::initalize(inGroupID, inRsrcID)) return FALSE;
    m_pMove->enableGravity(FALSE);
    m_pMove->setFlag(1, 0);
    m_pMove->setCollisionFlag(0, 0xF);
    m_pMove->setFlag(8, 0);
    return TRUE;
}

void CGimmickWindowLid::AfterInit() {
    rideModelEnableChipCollision(0, TRUE);
    rideModelEnableChipCollision(1, TRUE);
    changeMotion(1, 0, FALSE, 0, 0, 0);
}

void CGimmickWindowLid::beginStateMain() {}

void CGimmickWindowLid::stateMain() {
    rideModelSetChipLocalMatrix(0, (const FMATRIX*)&Crd(getBoneIndex("col", 0)));
    switch (m_Step) {
    case 0:
        if (isRideOnPlayer()) m_Step = 10;
        break;
    case 10:
        m_WaitCounter += (FLOAT)getFrameRate() * 0.0200000014f;
        if (m_WaitCounter > 120.0f) {
            m_WaitCounter = 0.0f;
            m_Step = 20;
            if (isHangOnPlayer()) g_pPlayer->setConditionFlag(0x40000, 0);
            changeMotion(2, 0, FALSE, 0, 0, 0);
        }
        break;
    case 20:
        m_WaitCounter += (FLOAT)getFrameRate() * 0.0200000014f;
        if (m_WaitCounter > 120.0f) {
            m_WaitCounter = 0.0f;
            m_Step = 30;
            setEntityFlag(0, 1);
            changeMotion(3, 0, FALSE, 0, 0, 0);
        }
        break;
    case 30:
        if (isMotionEnd()) {
            changeMotion(1, 0, FALSE, -1, 0, 0);
            m_Step = 0;
        }
        break;
    }
}

void CGimmickWindowLid::endStateMain() {}
// SOURCE DATA: .rodata
