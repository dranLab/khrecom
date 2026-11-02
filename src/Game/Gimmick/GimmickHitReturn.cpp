
#define GPRIM_H_PATH "System/Graphic/GPrim.h" /* MATCH: this unit names Gprim.h so (header default "System/Graphic/Gprim.h") */
#include "Game/Gimmick/GimmickHitReturn.h"
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
#include "System/Graphic/Effect/EffectMan.h"
#include "Game/Movement.h"
#include "Game/AttackMotion.h"
#include "Game/Gimmick/GimmickManager.h"
#include "Game/LockOnManager.h"
#include "Game/Player/Player.h"
#include "Game/SceneInfo.h"
#include "System/Graphic/Camera.h"

CGimmickHitReturn::CGimmickHitReturn(CTask* inParent, INT in_GimID) : CBaseGimmick(inParent), m_ID(in_GimID) {
    m_Step = 0;
    m_RepeatFlag = 0;
    m_RideFlag = 0;
    m_Wait = 0.0f;
}

CGimmickHitReturn::~CGimmickHitReturn() {
    SHORT scene = CSceneInfo::getCurrentSceneNum();
    if (!(scene >= 1000 && scene < 2000)) {
        ((CGimmickManager*)getSysTask(17))->setStorageGimmickState(getUniqueID(), m_Stat);
    }
}

BOOL CGimmickHitReturn::initalize(INT inGroupID, INT inRsrcID) {
    if (!CBaseGimmick::initalize(inGroupID, inRsrcID)) return FALSE;
    SHORT scene = CSceneInfo::getCurrentSceneNum();
    if (!(scene >= 1000 && scene < 2000)) {
        getSysTask(32);
        LockOnManager::CLockOnManager::registerLockOnObject(getHandle());
    }
    m_pMove->enableGravity(FALSE);
    m_pMove->setFlag(1, 0);
    m_pMove->setCollisionFlag(0, 0xF);
    m_pMove->setFlag(8, 0);
    return TRUE;
}

void CGimmickHitReturn::AfterInit() {
    INT BoneIndex;
    SHORT scene = CSceneInfo::getCurrentSceneNum();
    if (!(scene >= 1000 && scene < 2000)) {
        m_Stat = ((CGimmickManager*)getSysTask(17))->getStorageGimmickState(getUniqueID());
        if (m_Stat == -1) m_Stat = 0;
    } else {
        m_Stat = 1;
    }
    changeMotion(1, 0, FALSE, 0, 0, 0);
    if (getParam() == 6) {
        rideModelChangeListRC();
    } else {
        BoneIndex = getBoneIndex("col", 0);
        if (BoneIndex != -1) rideModelSetAttachMatrix(&Crd(BoneIndex).getWorldMatrix());
    }
    switch (m_ID) {
    case 0x4BA:
    case 0x4BB:
    case 0x4BC:
    case 0x4C5:
    case 0x4C6:
    case 0x4C7:
        rideModelEnableChipCollision(1, TRUE);
        break;
    }
    if (m_pAttack != NULL) {
        GimParam* param = getGimParam();
        if (param != NULL) m_pAttack->setAttackParam(param->AtkPow, (ATK_ATTR)param->AtkAttr);
    }
}

void CGimmickHitReturn::beginStateMain() {}

void CGimmickHitReturn::stateMain() {
    switch (m_ID) {
    case 0x4BA:
    case 0x4BB:
    case 0x4BC:
    case 0x4C5:
    case 0x4C6:
    case 0x4C7:
        rideModelSetChipLocalMatrix(0, (const FMATRIX*)&Crd(getBoneIndex("col", 0)));
        break;
    }
    if (isRideOnPlayer()) {
        if (m_ID == 0x45C) {
            if (!m_Stat) {
                createPrize(0, 0);
                m_Stat = 1;
                saveStat(1);
            }
        } else {
            if (getBoneIndex("prize2", 0) == -1) return;
            if (g_pPlayer->getPlayerState() == HANG) return;
            switch (m_ID) {
            case 0x4BA:
            case 0x4BB:
            case 0x4BC:
            case 0x4C5:
            case 0x4C6:
            case 0x4C7:
                if (!m_Stat) {
                    if (((CGimmickManager*)getSysTask(17))
                            ->DecidePrize(0x4BC, &Crd(getBoneIndex("prize2", 0)).getWorldMatrix().v[3], NULL, 0)) {
                        CEffectMan::setEffect("rec_priz_app", 0, ObjType::OBJ_TYPE_GIMMICK,
                                              &Crd(getBoneIndex("prize2", 0)).getWorldMatrix(), 0.0f);
                    }
                }
                break;
            default:
                if (!m_Stat) createPrize(1, 0);
                if (!m_RideFlag) changeMotion(3, 0, FALSE, -1, 0, 0);
                break;
            }
            m_RideFlag = 1;
            m_Stat = 1;
            saveStat(1);
        }
    } else {
        m_RideFlag = 0;
        if (isMotionEnd() && m_pAnim->getAnimID() != 1) changeMotion(1, 0, FALSE, 0, 0, 0);
    }
    if (getGimmickFlag() & 1) {
        m_Wait = 30.0f;
        setGimmickFlag(1, FALSE);
    }
    if (m_Wait != 0.0f) {
        m_Wait -= (FLOAT)getFrameRate() * 0.0200000014f;
        if (m_Wait <= 0.0f) {
            m_Wait = 0.0f;
            m_pAttack->updateAttackID();
        }
    }
}

void CGimmickHitReturn::endStateMain() {}

void CGimmickHitReturn::beginStateHitAttack() {
    if (!m_Stat) {
        switch (m_ID) {
        case 0x4BA:
        case 0x4BB:
        case 0x4BC:
        case 0x4C5:
        case 0x4C6:
        case 0x4C7:
            if (((CGimmickManager*)getSysTask(17))
                    ->DecidePrize(0x4BB, &Crd(getBoneIndex("prize", 0)).getWorldMatrix().v[3], NULL, 0)) {
                CEffectMan::setEffect("rec_priz_app", 0, ObjType::OBJ_TYPE_GIMMICK,
                                      &Crd(getBoneIndex("prize", 0)).getWorldMatrix(), 0.0f);
            }
            break;
        default:
            createPrize(0, 0);
            break;
        }
    }
    m_Stat = 1;
    saveStat(1);
    if (m_Stat) {
        changeMotion(2, 0, FALSE, -1, 0, 0);
        if (m_ID == 0x6A) {
            GimParam* param = getGimParam();
            if (param != NULL) m_pAttack->setAttackParam(param->AtkPow, (ATK_ATTR)param->AtkAttr);
            m_pAttack->setCollKind(0);
        }
    }
    changeGimmickState(0);
}

void CGimmickHitReturn::stateHitAttack() {}

void CGimmickHitReturn::endStateHitAttack() {}
// SOURCE DATA: .rodata
