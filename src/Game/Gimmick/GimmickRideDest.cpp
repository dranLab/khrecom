
#define GPRIM_H_PATH "System/Graphic/GPrim.h"
#include "Game/Gimmick/GimmickRideDest.h"
#include "Game/BodyCollision.h"
#include "System/Graphic/Effect/effect.h"
#include "System/Graphic/Gprim.h"
#include "System/Graphic/2d/2DMan.h"
#include "Game/CardBattle.h"
#include "System/Graphic/Str.h"
#include "Game/Collision/CollUtil.h"
#include "Game/Collision/CollBody.h"
/* Placeholders: three empty strings follow CollUtil.h's strings in .rodata (as in GimmickRideOn/WindowLid/Bell);
 * which headers they come from is not known. */
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
#include "Game/Gimmick/GimmickManager.h"
#include "Game/LockOnManager.h"
#include "Game/Player/Player.h"
#include "Game/SceneInfo.h"
#include "System/Graphic/Camera.h"

#include "Game/Gimmick/GimmickManager.h"






CGimmickRideDest::CGimmickRideDest(CTask* inParent) : CBaseGimmick(inParent) {
    m_Step = 0;
}

CGimmickRideDest::~CGimmickRideDest() {
    SHORT scene = CSceneInfo::getCurrentSceneNum();
    if (!(scene >= 1000 && scene < 2000)) {
        ((CGimmickManager*)getSysTask(17))->setStorageGimmickState(getUniqueID(), m_Stat);
    }
}

BOOL CGimmickRideDest::initalize(INT inGroupID, INT inRsrcID) {
    if (!CBaseGimmick::initalize(inGroupID, inRsrcID)) return FALSE;
    m_pMove->enableGravity(FALSE);
    m_pMove->setFlag(1, 0);
    m_pMove->setCollisionFlag(0, 0xF);
    m_pMove->setFlag(8, 0);
    bodyCollOffBody(-1);
    rideModelEnableAllCollision(FALSE);
    SHORT scene = CSceneInfo::getCurrentSceneNum();
    if (!(scene >= 1000 && scene < 2000)) {
        getSysTask(32);
        LockOnManager::CLockOnManager::registerLockOnObject(getHandle());
    }
    return TRUE;
}

void CGimmickRideDest::AfterInit() {
    INT BoneIndex;
    SHORT scene = CSceneInfo::getCurrentSceneNum();
    if (!(scene >= 1000 && scene < 2000)) {
        m_Stat = ((CGimmickManager*)getSysTask(17))->getStorageGimmickState(getUniqueID());
        if (m_Stat == -1) m_Stat = 0;
        if (m_Stat == 0 || m_Stat == 1) {
            bodyCollOnBody(-1);
            rideModelEnableAllCollision(TRUE);
        }
    } else {
        bodyCollOff(-1);
        bodyCollOffBody(-1);
    }
    if (getParam() == 6) rideModelChangeListRC();
    BoneIndex = getBoneIndex("col", 0);
    if (BoneIndex != -1) rideModelSetAttachMatrix(&Crd(BoneIndex).getWorldMatrix());
    rideModelChangeListR2();
}

void CGimmickRideDest::beginStateMain() {
    if (m_Stat == 2) out();
}

void CGimmickRideDest::stateMain() {
    SHORT scene = CSceneInfo::getCurrentSceneNum();
    if (scene >= 1000 && scene < 2000) return;
    switch (m_Step) {
    case 0:
        if (isRideOnPlayer()) {
            if (g_pPlayer->getPlayerState() == HANG) return;
            if (!m_BodyFlag) m_BodyFlag = 1;
            if (getBoneIndex("prize2", 0) != -1) {
                m_Step = 10;
                changeMotion(3, 0, FALSE, -1, 0, 0);
                if (!m_Stat) {
                    createPrize(1, 0);
                    m_Stat = 1;
                }
            }
        } else if (m_BodyFlag) {
            bodyCollOn(-1);
            m_BodyFlag = 0;
        }
        break;
    case 10:
        if (isMotionEnd() && !isRideOnPlayer()) {
            changeMotion(1, 0, FALSE, -1, 0, 0);
            m_Step = 0;
        }
        break;
    }
}

void CGimmickRideDest::endStateMain() {}

void CGimmickRideDest::beginStateHitAttack() {
    SHORT scene = CSceneInfo::getCurrentSceneNum();
    if (!(scene >= 1000 && scene < 2000)) {
        ATK_KIND kind = m_pBodyColl->getHitCollBody()->getDamageKind();
        if (kind == ATK_KIND_DMG_BIG || kind == ATK_KIND_GM_BIG_DAMAGE) {
            DropPrize prize;
            prize.StockKind = 0;
            prize.Kind = CARD_KIND_None;
            prize.Num = 0;
            prize.No = 0;
            if (!m_Stat) createPrize(0, 0);
            rideModelEnableAllCollision(FALSE);
            changeMotion(2, 0, FALSE, -1, 0, 0);
            m_Stat = 2;
            setEntityFlag(1, 0);
            saveStat(2);
        } else {
            goto other;
        }
    }
    changeGimmickState(0);
    return;
other:
    changeMotion(3, 0, FALSE, -1, 0, 0);
}

void CGimmickRideDest::stateHitAttack() {
    if (isMotionEnd()) {
        changeMotion(1, 0, FALSE, -1, 0, 0);
        changeGimmickState(0);
    }
}

void CGimmickRideDest::endStateHitAttack() {}
// SOURCE DATA: .rodata
