
#include "System/Graphic/Gprim.h"
#include "Game/Gimmick/GimmickPlantBullet.h"
#include "System/Graphic/Effect/effect.h"
#include "System/Graphic/2d/2DMan.h"
#include "Game/CardBattle.h"
#include "System/Graphic/Str.h"
#include "Game/Collision/CollUtil.h"

/* MATCH: Placeholder: an empty string follows CollUtil.h's strings in .rodata (as in GimmickPlantChild); which
 * header it comes from is not known. */
inline void _emptyStringPlaceholder() {
    TRACE("");
}

#include "System/Graphic/Model.h"
#include "Game/BodyCollision.h"
#include "Game/Movement.h"
#include "Game/Gimmick/GimmickManager.h"
#include "Game/LockOnManager.h"
#include "Game/Player/Player.h"
#include "Game/Bullet/Bullet.h"
#include "Game/EntityFactory.h"
#include "System/Graphic/Camera.h"
#include "System/Math3D.h"
#include "Game/Collision/CollBody.h"
static BULLET_EFFECT_INFO g_GimPlantBulletEffTbl[4] = {
    {NULL, 0, 0, 0},
    {"rec_aw_common", 3, 0, 0},
    {"rec_aw_common", 4, 0, 0},
    {"rec_aw_common", 2, 0, 0},
};

CGimmickPlantBullet::CGimmickPlantBullet(CTask* inParent) : CBaseGimmick(inParent) {
    m_Stat = 0;
    m_Step = 0;
    m_AtkStep = 0;
    m_RotStep = 0;
    m_WaitCounter = 0.0f;
}

CGimmickPlantBullet::~CGimmickPlantBullet() {
    ((CGimmickManager*)getSysTask(17))->setStorageGimmickState(getUniqueID(), m_Stat);
}

BOOL CGimmickPlantBullet::initalize(INT inGroupID, INT inRsrcID) {
    if (!CBaseGimmick::initalize(inGroupID, inRsrcID)) return FALSE;
    getSysTask(32);
    LockOnManager::CLockOnManager::registerLockOnObject(getHandle());
    m_pMove->enableGravity(FALSE);
    m_pMove->setFlag(1, 0);
    m_pMove->setCollisionFlag(0, 0xF);
    m_pMove->setFlag(8, 0);
    bodyCollOffBody(-1);
    changeMotion(1, 0, FALSE, -1, 0, 0);
    return TRUE;
}

void CGimmickPlantBullet::AfterInit() {
    m_Stat = ((CGimmickManager*)getSysTask(17))->getStorageGimmickState(getUniqueID());
    if (m_Stat == -1) m_Stat = 0;
    m_Dest = m_Stat;
    if (m_Stat == 0) bodyCollOnBody(-1);
}

void CGimmickPlantBullet::beginStateMain() {
    if (m_Dest == 1) out();
}

void CGimmickPlantBullet::stateMain() {
    FLOAT y;
    FLOAT rotY;
    FLOAT al;
    INT i;

    switch (m_Step) {
    case 0:
        m_Step = 10;
        m_WaitCounter = 120.0f;
        break;
    case 10:
        if (m_WaitCounter > 0.0f) m_WaitCounter -= getFrameRateF();
        if (m_WaitCounter <= 0.0f) {
            m_WaitCounter = 0.0f;
            m_Step = 20;
        }
        break;
    case 20:
        if (calcDistanceXZ(g_pPlayer) <= 12.0f) {
            y = g_pPlayer->getPos().y - getPos().y;
            if (y <= 0.5f && y >= -0.5f) {
                rotY = Math3D::LimitPI(calcDirection(g_pPlayer) - getRot().y) * 57.295776f;
                if (rotY < -5.0f || rotY > 5.0f) {
                    m_Step = 25;
                } else if (m_RotStep == 0) {
                    m_Step = 30;
                }
            }
        }
        break;
    case 25:
        loockAtPlayer();
        if (m_RotStep == 0) m_Step = 20;
        break;
    case 30:
        if (AtkPlayer()) m_Step = 20;
        break;
    case 50:
        for (i = 0; i < getModelCount(); i++) {
            al = getModel()[i].getAlphaScale() - getFrameRateF() * 0.01;
            if (al <= 0.0f) {
                al = 0.0f;
                m_Step = 60;
            }
            getModel(i)->setAlphaScale(al);
        }
        break;
    case 60:
        out();
        break;
    }
}

void CGimmickPlantBullet::endStateMain() {}

void CGimmickPlantBullet::loockAtPlayer() {
    FLOAT rotY = calcDirection(g_pPlayer);
    rotY = Math3D::LimitPI(rotY - getRot().y) * 57.295776f;

    switch (m_RotStep) {
    case 0:
        if (rotY > 5.0f) {
            changeMotion(3, 0, FALSE, 0, 0, 0);
            m_RotStep = 10;
        } else if (rotY < -5.0f) {
            changeMotion(2, 0, FALSE, 0, 0, 0);
            m_RotStep = 20;
        }
        break;
    case 10:
        if (rotY > 2.0f) {
            FVECTOR v = getRot();
            FLOAT r = (v.y * 57.295776f + getFrameRateF()) * 0.017453292f;
            v.y = r;
            if (r > 3.14f) v.y = r - 6.28f;
            setRot(v);
        }
        if (isMotionEnd()) {
            changeMotion(1, 0, FALSE, 0, 0, 0);
            m_RotStep = 0;
        }
        break;
    case 20:
        if (rotY < -2.0f) {
            FVECTOR v = getRot();
            FLOAT r = (v.y * 57.295776f - getFrameRateF()) * 0.017453292f;
            v.y = r;
            if (r < -3.14f) v.y = r + 6.28f;
            setRot(v);
        }
        if (isMotionEnd()) {
            changeMotion(1, 0, FALSE, 0, 0, 0);
            m_RotStep = 0;
        }
        break;
    }
}

BOOL CGimmickPlantBullet::AtkPlayer() {
    switch (m_AtkStep) {
    case 0:
        if (g_pPlayer->getPos().y - getPos().y < 0.5f) {
            changeMotion(4, 0, FALSE, 0, 0, 0);
            m_AtkStep = 5;
            m_WaitCounter = 40.0f;
        }
        break;
    case 5:
        m_WaitCounter -= getFrameRateF();
        if (m_WaitCounter <= 0.0f) m_AtkStep = 10;
        break;
    case 10: {
        FVECTOR v;
        FMATRIX m = Crd(getBoneIndex("hana3", 0)).getWorldMatrix();
        BULLET_PARAM param;
        v.x = m.v[3].x;
        v.y = m.v[3].y;
        v.z = m.v[3].z;
        param.init(1, 3);
        param.AtkAttr = 5;
        param.AtkKind = 10;
        param.fSpeed = 0.15f;
        param.vPos = v;
        param.iAttackPoint = 0;
        param.vRot.x = 0.0f;
        param.vRot.z = 0.0f;
        param.vRot.y = getRot().y;
        param.pEffectTbl = g_GimPlantBulletEffTbl;
        param.fCollRadius = 0.2f;
        param.uiFlag = 0x41108940;
        param.hHomingTarget = g_pPlayer->getHandle();
        param.tqFadeTime = 2000;
        param.tqHomingTime2 = 750;
        param.fHomingRate = 0.125f;
        param.fMaxDist = 50.0f;
        CEntityFactory::getInstance()->createBullet(param, NULL);
        m_AtkStep = 20;
        break;
    }
    case 20:
        if (isMotionEnd()) {
            changeMotion(1, 0, FALSE, 0, 0, 0);
            m_AtkStep = 30;
            m_WaitCounter = 120.0f;
        }
        break;
    case 30:
        m_WaitCounter -= getFrameRateF();
        if (m_WaitCounter <= 0.0f) {
            m_AtkStep = 0;
            return TRUE;
        }
        break;
    }
    return FALSE;
}

void CGimmickPlantBullet::beginStateHitAttack() {
    if (m_pBodyColl->getHitCollBody()->getDamageKind() == ATK_KIND_DMG_BIG) {
        createPrize(0, 0);
        changeMotion(6, 0, FALSE, -1, 0, 0);
        bodyCollOffBody(-1);
        m_Step = 50;
        setEntityFlag(0, 0x10000);
        m_Stat = 1;
        saveStat(m_Stat);
        changeGimmickState(0);
    } else {
        changeMotion(5, 0, FALSE, -1, 0, 0);
    }
    changeGimmickState(0);
}

void CGimmickPlantBullet::stateHitAttack() {}

void CGimmickPlantBullet::endStateHitAttack() {}
// SOURCE DATA: .data .rodata
