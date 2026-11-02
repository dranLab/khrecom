
#define GPRIM_H_PATH "System/Graphic/GPrim.h" /* MATCH: this unit names Gprim.h so (header default "System/Graphic/Gprim.h") */
#include "Game/Gimmick/GimmickWallHole.h"
#include "Game/BodyCollision.h"
#include "System/Graphic/Effect/effect.h"
#include "System/Graphic/Gprim.h"
#include "System/Graphic/2d/2DMan.h"
#include "Game/CardBattle.h"
#include "System/Graphic/Str.h"
#include "Game/Collision/CollUtil.h"
#include "Game/Collision/CollBody.h"
/* MATCH: Placeholder: an empty string follows CollUtil.h's strings in .rodata (as in GimmickPlantChild); which
 * header it comes from is not known. */
inline void _emptyStringPlaceholder() {
    TRACE("");
}

#include "System/Graphic/Model.h"
#include "Game/Movement.h"
#include "Game/AttackMotion.h"
#include "Game/Gimmick/GimmickManager.h"
#include "Game/LockOnManager.h"
#include "Game/Player/Player.h"

CGimmickWallHole::CGimmickWallHole(CTask* inParent) : CBaseGimmick(inParent) {
    m_Step = 0;
    m_AtkUpDateWait = 0.0f;
}

CGimmickWallHole::~CGimmickWallHole() {
    ((CGimmickManager*)getSysTask(17))->setStorageGimmickState(getUniqueID(), m_Stat);
}

BOOL CGimmickWallHole::initalize(INT inGroupID, INT inRsrcID) {
    if (!CBaseGimmick::initalize(inGroupID, inRsrcID)) return FALSE;
    getSysTask(32);
    LockOnManager::CLockOnManager::registerLockOnObject(getHandle());
    m_pMove->enableGravity(FALSE);
    m_pMove->setFlag(1, 0);
    m_pMove->setCollisionFlag(0, 0xF);
    m_pMove->setFlag(8, 0);
    bodyCollOffBody(-1);
    rideModelEnableAllCollision(FALSE);
    return TRUE;
}

void CGimmickWallHole::AfterInit() {
    m_Stat = ((CGimmickManager*)getSysTask(17))->getStorageGimmickState(getUniqueID());
    if (m_Stat == -1) m_Stat = 0;
    if (m_Stat == 0) {
        bodyCollOnBody(-1);
        rideModelEnableAllCollision(TRUE);
        switch ((UINT)getID() % 10000) {
        case 516:
            changeMotion(1, 0, FALSE, 0, 0, 0);
            setGimmickFlag(0x10, TRUE);
            m_Wait = 60.0f;
            break;
        case 517:
            changeMotion(1, 0, FALSE, 0, 0, 0);
            break;
        }
    } else {
        out();
    }
}

void CGimmickWallHole::beginStateMain() {}

void CGimmickWallHole::stateMain() {
    switch ((UINT)getID() % 10000) {
    case 516:
        HoleMain();
        break;
    case 517:
        WhipMain();
        break;
    }
}

void CGimmickWallHole::endStateMain() {}

void CGimmickWallHole::HoleMain() {
    if (getGimmickFlag() & 1) {
        m_AtkUpDateWait = 60.0f;
        setGimmickFlag(1, FALSE);
    }
    if (m_AtkUpDateWait != 0.0f) {
        m_AtkUpDateWait -= getFrameRateF();
        if (m_AtkUpDateWait <= 0.0f) {
            m_AtkUpDateWait = 0.0f;
            m_pAttack->updateAttackID();
        }
    }

    switch (m_Step) {
    case 0:
        m_Wait -= getFrameRateF();
        if (m_Wait <= 0.0f) m_Step = 10;
        break;
    case 10:
        changeMotion(2, 0, FALSE, 0, 0, 0);
        m_Step = 20;
        break;
    case 20:
        if (isMotionEnd()) {
            changeMotion(3, 0, FALSE, 0, 0, 0);
            if (m_pAttack != NULL) {
                GimParam* param = getGimParam();
                if (param != NULL) m_pAttack->setAttackParam(param->AtkPow, (ATK_ATTR)param->AtkAttr);
            }
            m_Wait = 250.0f;
            m_Step = 30;
        }
        break;
    case 30:
        m_Wait -= getFrameRateF();
        if (m_Wait <= 190.0f) setGimmickFlag(0x10, FALSE);
        if (m_Wait <= 0.0f) {
            setGimmickFlag(0x10, TRUE);
            changeMotion(4, 0, FALSE, 0, 0, 0);
            m_Step = 40;
        }
        break;
    case 40:
        if (isMotionEnd()) {
            changeMotion(1, 0, FALSE, 0, 0, 0);
            m_Step = 0;
            m_Wait = 60.0f;
        }
        break;
    case 100:
        m_Wait = 50.0f;
        m_Step = 105;
        break;
    case 105:
        m_Wait -= getFrameRateF();
        if (m_Wait <= 0.0f) m_Step = 110;
        break;
    case 110: {
        FLOAT al = getModel()->getAlphaScale();
        al -= getFrameRateF() * 0.1f;
        if (al <= 0.0f) {
            al = 0.0f;
            out();
        }
        getModel()->setAlphaScale(al);
        break;
    }
    }
}

void CGimmickWallHole::WhipMain() {
    switch (m_Step) {
    case 0:
        if (calcDistanceXZ(g_pPlayer) <= 6.0f && g_pPlayer->getPos().y - getPos().y <= 2.0f &&
            -2.0f <= g_pPlayer->getPos().y - getPos().y) {
            m_Step = 10;
        }
        break;
    case 10:
        changeMotion(2, 0, FALSE, -1, 0, 0);
        if (m_pAttack != NULL) {
            GimParam* param = getGimParam();
            if (param != NULL) m_pAttack->setAttackParam(param->AtkPow, (ATK_ATTR)param->AtkAttr);
        }
        m_Step = 20;
        break;
    case 20:
        if (isMotionEnd()) {
            changeMotion(3, 0, FALSE, -1, 0, 0);
            m_Step = 30;
        }
        break;
    case 30:
        if (isMotionEnd()) {
            changeMotion(1, 0, FALSE, -1, 0, 0);
            m_Step = 0;
        }
        break;
    case 100:
        m_Step = 105;
        m_Wait = 60.0f;
        break;
    case 105:
        m_Wait -= getFrameRateF();
        if (m_Wait <= 0.0f) m_Step = 110;
        break;
    case 110: {
        FLOAT al;
        INT i;
        for (i = 0; i < getModelCount(); i++) {
            al = getModel()[i].getAlphaScale();
            al -= getFrameRateF() * 0.05f;
            if (al <= 0.0f) {
                al = 0.0f;
                out();
            }
            getModel(i)->setAlphaScale(al);
        }
        break;
    }
    }
}

void CGimmickWallHole::beginStateHitAttack() {
    if (m_Stat == 0) {
        switch ((UINT)getID() % 10000) {
        case 516:
            if (m_Step < 100) {
                m_Step = 100;
                changeMotion(5, 0, FALSE, -1, 0, 0);
                rideModelEnableChipCollision(0, FALSE);
                createPrize(0, 0);
                m_Stat = 1;
                saveStat(m_Stat);
            }
            break;
        case 517:
            if (m_Step < 100) {
                ATK_KIND kind = m_pBodyColl->getHitCollBody()->getDamageKind();
                if (kind == 2 || kind == 0x26) {
                    m_Step = 100;
                    changeMotion(5, 0, FALSE, -1, 0, 0);
                    createPrize(0, 0);
                    m_Stat = 1;
                    saveStat(m_Stat);
                } else {
                    m_Step = 30;
                    changeMotion(4, 0, FALSE, -1, 0, 0);
                }
            }
            break;
        }
    }
    changeGimmickState(0);
}

void CGimmickWallHole::stateHitAttack() {}

void CGimmickWallHole::endStateHitAttack() {}
// SOURCE DATA: .rodata
