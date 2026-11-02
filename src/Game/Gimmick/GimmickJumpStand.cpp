
#define GPRIM_H_PATH "System/Graphic/GPrim.h" /* MATCH: this unit names Gprim.h so (header default "System/Graphic/Gprim.h") */
#include "Game/Gimmick/GimmickJumpStand.h"
#include "System/Graphic/Gprim.h"
#include "System/Graphic/2d/2DMan.h"
#include "Game/CardBattle.h"
#include "System/Graphic/Str.h"
#include "Game/Collision/CollUtil.h"

/* Placeholders: empty strings follow CollUtil.h's strings in .rodata (as in GimmickLotusLeaf); which
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
#include "Game/Player/Player.h"
#include "System/Graphic/Camera.h"

CGimmickJumpStand::CGimmickJumpStand(CTask* inParent, INT in_GimID) : CBaseGimmick(inParent) {
    m_ID = in_GimID;
    m_Step = 0;
    m_WaitCounter = 0.0f;
    m_ColiFlag = 0;
    m_Wait = 0.0f;
}

CGimmickJumpStand::~CGimmickJumpStand() {}

INT CGimmickJumpStand::getTypeID() const {
    return 0x2002;
}

BOOL CGimmickJumpStand::initalize(INT inGroupID, INT inRsrcID) {
    if (!CBaseGimmick::initalize(inGroupID, inRsrcID)) return FALSE;
    changeMotion(1, 0, FALSE, -1, 0, 0);
    switch (m_ID) {
    case 103:
    case 403:
    case 503:
    case 703:
    case 803:
    case 855:
    case 903:
    case 926:
    case 1303:
    case 1314:
        rideModelSetAttachMatrix(&Crd(getBoneIndex("jump_col", 0)).getWorldMatrix());
        break;
    case 203:
    case 303:
    case 603:
    case 1203:
        rideModelChangeListR2();
        rideModelEnableChipCollision(0, FALSE);
        rideModelEnableChipCollision(1, TRUE);
        break;
    case 1103:
    case 1150:
        rideModelSetAttachMatrix(&Crd(getBoneIndex("jump_col", 0)).getWorldMatrix());
        break;
    }
    rideModelEnableAllCollision(TRUE);
    changeMotion(1, 0, FALSE, -1, 0, 0);
    m_pMove->enableGravity(FALSE);
    m_pMove->setFlag(1, 0);
    m_pMove->setCollisionFlag(0, 0xF);
    m_pMove->setFlag(8, 0);
    return TRUE;
}

void CGimmickJumpStand::beginStateMain() {}

void CGimmickJumpStand::stateMain() {
    switch (m_ID) {
    case 103:
    case 403:
    case 503:
    case 703:
    case 803:
    case 855:
    case 903:
    case 926:
    case 1303:
    case 1314:
        main_Tr();
        break;
    case 203:
    case 303:
    case 603:
    case 1203:
        main_Ol();
        break;
    case 1103:
    case 1150:
        main_Tw();
        break;
    }
}

void CGimmickJumpStand::endStateMain() {}

void CGimmickJumpStand::main_Tr() {
    CTask* pTask;
    if (isRideOnPlayer()) CPlayer::notifyGimmickJump(getHandle(), getJumpStandPower());
    switch (m_Step) {
    case 0:
        m_WaitCounter = 10.0f;
        m_Step = 1;
        break;
    case 1:
        m_WaitCounter -= getFrameRateF();
        if (m_WaitCounter <= 0.0f) m_Step = 5;
        if (isRideOnPlayer()) {
            changeMotion(2, 0, FALSE, 0, 0, -1);
            m_Step = 10;
        }
        break;
    case 5:
        pTask = getSysTask(3);
        if (isRideOnPlayer()) {
            changeMotion(2, 0, FALSE, -1, 0, 0);
            m_Step = 10;
        }
        break;
    case 10:
        pTask = getSysTask(3);
        if (!isRideOnPlayer()) {
            if (((CPlayer*)pTask)->getJumpState() == 1) {
                changeMotion(3, 0, FALSE, -1, 0, 0);
                m_Step = 20;
            } else {
                changeMotion(1, 0, FALSE, -1, 0, 0);
                m_Step = 5;
            }
        }
        break;
    case 20:
        if (isMotionEnd() || isRideOnPlayer()) {
            changeMotion(1, 0, FALSE, -1, 0, 0);
            m_Step = 5;
        }
        break;
    }
}

void CGimmickJumpStand::main_Ol() {
    if (m_ColiFlag == 0) rideModelSetChipLocalMatrix(0, &Crd(getBoneIndex("jump_col", 0)).m_Local);
    if (isRideOnPlayer()) CPlayer::notifyGimmickJump(getHandle(), getJumpStandPower());
    switch (m_Step) {
    case 0:
        m_WaitCounter = 10.0f;
        m_Step = 1;
        break;
    case 1:
        m_WaitCounter -= getFrameRateF();
        if (m_WaitCounter <= 0.0f) m_Step = 5;
        if (isRideOnPlayer()) {
            changeMotion(2, 0, FALSE, 0, 0, -1);
            m_Step = 10;
        }
        break;
    case 5:
        getSysTask(3);
        if (isRideOnPlayer()) {
            m_WaitCounter = 8.0f;
            changeMotion(2, 0, FALSE, -1, 0, 0);
            m_Step = 10;
        } else {
            rideModelEnableChipCollision(0, TRUE);
        }
        break;
    case 10: {
        CPlayer* pPlayer = (CPlayer*)getSysTask(3);
        if (!isRideOnPlayer()) {
            if (pPlayer->getJumpState() == 1) {
                changeMotion(3, 0, FALSE, -1, 0, 0);
                m_ColiFlag = 1;
                m_Wait = 20.0f;
                m_Step = 20;
            } else if (m_WaitCounter <= 0.0f) {
                changeMotion(1, 0, FALSE, -1, 0, 0);
                m_Step = 5;
            } else {
                m_WaitCounter -= getFrameRateF();
            }
        } else {
            m_WaitCounter = 12.0f;
            if (calcDistanceXZ(pPlayer) > 0.5f) {
                if (m_pAnim->getAnimID() != 1) changeMotion(1, 0, FALSE, -1, 0, 0);
            } else {
                if (m_pAnim->getAnimID() != 2) changeMotion(2, 0, FALSE, -1, 0, 0);
            }
        }
        break;
    }
    case 20:
        if (isMotionEnd() || isRideOnPlayer()) {
            changeMotion(1, 0, FALSE, -1, 0, 0);
            m_Step = 5;
        }
        break;
    }
    if (m_ColiFlag == 1) {
        m_Wait -= getFrameRateF();
        if (m_Wait <= 0.0f) m_ColiFlag = 0;
    }
}

void CGimmickJumpStand::main_Tw() {
    CTask* pTask;
    if (isRideOnPlayer()) CPlayer::notifyGimmickJump(getHandle(), getJumpStandPower());
    switch (m_Step) {
    case 0:
        m_WaitCounter = 10.0f;
        m_Step = 1;
        break;
    case 1:
        m_WaitCounter -= getFrameRateF();
        if (m_WaitCounter <= 0.0f) m_Step = 5;
        if (isRideOnPlayer()) {
            changeMotion(5, 0, FALSE, 0, 0, -1);
            m_Step = 10;
        }
        break;
    case 5:
        pTask = getSysTask(3);
        if (isRideOnPlayer()) {
            if (m_pAnim->getAnimID() == 3 || m_pAnim->getAnimID() == 4) changeMotion(2, 0, FALSE, -1, 0, 0);
            if (m_pAnim->getAnimID() == 1) changeMotion(5, 0, FALSE, -1, 0, 0);
            m_Step = 10;
        }
        break;
    case 10:
        pTask = getSysTask(3);
        if (!isRideOnPlayer()) {
            if (((CPlayer*)pTask)->getJumpState() == 1) {
                changeMotion(3, 0, FALSE, -1, 0, 0);
                m_Step = 20;
            } else {
                changeMotion(1, 0, FALSE, -1, 0, 0);
                m_Step = 5;
            }
        }
        break;
    case 20:
        if (isMotionEnd()) {
            changeMotion(4, 0, FALSE, -1, 0, 0);
            m_Step = 5;
        } else if (isRideOnPlayer()) {
            changeMotion(2, 0, FALSE, -1, 0, 0);
            m_Step = 5;
        }
        break;
    }
}
// SOURCE DATA: .rodata
