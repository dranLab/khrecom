#define TASK_H_NO_SLC_OVERFLOW_MSG
#define GPRIM_H_PATH "System/Graphic/GPrim.h"
#include "Game/Gimmick/GimmickFallTomb.h"
#include "Game/BodyCollision.h"
#include "System/Graphic/Effect/effect.h"
#include "System/Graphic/Gprim.h"
#include "System/Graphic/2d/2DMan.h"
#include "Game/CardBattle.h"
#include "System/Graphic/Str.h"
#include "Game/Collision/CollUtil.h"

/* Placeholder: an empty string follows CollUtil.h's strings in .rodata (as in GimmickPlantChild); which
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
#include "System/Graphic/Camera.h"
#include "System/Rnd.h"

CGimmickFallTomb::CGimmickFallTomb(CTask* inParent) : CBaseGimmick(inParent) {
    m_Step = 0;
    m_Stat = 0;
}

CGimmickFallTomb::~CGimmickFallTomb() {
    ((CGimmickManager*)getSysTask(17))->setStorageGimmickState(getUniqueID(), m_Stat);
}

BOOL CGimmickFallTomb::initalize(INT inGroupID, INT inRsrcID) {
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

void CGimmickFallTomb::AfterInit() {
    CGimmickManager* gim = (CGimmickManager*)getSysTask(17);
    m_Stat = gim->getStorageGimmickState(getUniqueID());
    if (m_Stat == -1) m_Stat = 0;
    if (m_Stat == 0) {
        bodyCollOnBody(-1);
        rideModelEnableAllCollision(TRUE);
        changeMotion(1, 0, FALSE, -1, 0, 0);
        switch ((UINT)getID() % 10000) {
        case 609:
            m_Distance = 6.0f;
            m_DistRot = 20.0f;
            break;
        case 610:
            m_Distance = 5.0f;
            m_DistRot = 45.0f;
            rideModelSetAttachMatrix(&Crd(getBoneIndex("col", 0)).getWorldMatrix());
            break;
        case 611:
            m_Distance = 4.0f;
            m_DistRot = 20.0f;
            break;
        default:
            TRACE("\x93\x7c\x82\xea\x82\xe9\x95\xe6\x90\xce\x83\x47\x83\x89\x81\x5b\x81\x46\x94\xcd\x88\xcd\x82\xaa\x90\xdd\x92\xe8\x82\xb3\x82\xea\x82\xc4\x82\xa2\x82\xdc\x82\xb9\x82\xf1\n");
            m_Distance = 2.0f;
            m_DistRot = 20.0f;
            break;
        }
    } else {
        m_Step = 10;
    }
    GimParam* param = gim->getGimParam((UINT)getID() % 10000);
    m_HP = param != NULL ? param->Hp : 3;
}

void CGimmickFallTomb::beginStateMain() {
    if (m_Step == 10 && m_Stat == 1) out();
}

void CGimmickFallTomb::stateMain() {
    if ((UINT)getID() % 10000 == 610 && calcDistanceXZ(g_pPlayer) <= m_Distance && m_Step == 0) {
        FLOAT rotY = calcDirection(g_pPlayer) - getRot().y;
        if (rotY < -3.14f) rotY += 6.28f;
        rotY *= 57.295776f;
        if (-m_DistRot <= rotY && rotY <= m_DistRot) m_Step = 10;
    }

    switch (m_Step) {
    case 0:
        break;
    case 5:
        if (isMotionEnd()) {
            changeMotion(1, 0, FALSE, -1, 0, 0);
            m_Step = 0;
        }
        break;
    case 10:
        m_Stat = 1;
        saveStat(1);
        changeMotion(3, 0, FALSE, -1, 0, 0);
        m_Step = 20;
        setEntityFlag(0, 0x10000);
        break;
    case 20:
        if (isMotionEnd()) {
            changeMotion(4, 0, FALSE, -1, 0, 0);
            bodyCollOn(-1);
            if (m_pAttack != NULL) {
                GimParam* param = getGimParam();
                if (param != NULL) m_pAttack->setAttackParam(param->AtkPow, (ATK_ATTR)param->AtkAttr);
            }
            m_Step = 30;
            break;
        }
    case 30:
        if (isMotionEnd()) {
            bodyCollOff(-1);
            m_Step = 40;
        }
        break;
    case 40: {
        FLOAT al;
        INT i;
        for (i = 0; i < getModelCount(); i++) {
            al = getModel()[i].getAlphaScale() - getFrameRateF() * 0.05;
            if (al <= 0.0f) {
                al = 0.0f;
                m_Step = 50;
            }
            getModel(i)->setAlphaScale(al);
        }
        break;
    }
    case 50:
        out();
        break;
    }
}

void CGimmickFallTomb::endStateMain() {}

void CGimmickFallTomb::beginStateHitAttack() {
    if (m_Stat == 0) {
        m_HP--;
        ATK_KIND kind = m_pBodyColl->getHitCollBody()->getDamageKind();
        if (kind == 2 || kind == 0x26) m_HP = 0;
        if (RND::genrand_int31() % 3 == 0) createPrize(0, 0);
        if (m_HP <= 0) {
            m_Step = 10;
        } else {
            changeMotion(2, 0, FALSE, -1, 0, 0);
            m_Step = 5;
        }
    }
    changeGimmickState(0);
}

void CGimmickFallTomb::stateHitAttack() {}

void CGimmickFallTomb::endStateHitAttack() {}
// SOURCE DATA: .rodata
