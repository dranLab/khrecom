
#include "System/Graphic/Gprim.h"
#include "Game/Gimmick/GimmickAngelArrow.h"
#include "System/Graphic/Effect/effect.h"
#include "System/Graphic/2d/2DMan.h"
#include "Game/CardBattle.h"
#include "System/Graphic/Str.h"
#include "Game/Collision/CollUtil.h"

/* MATCH: Placeholders: three empty strings follow CollUtil.h's strings in .rodata; which headers they come
 * from is not known. */
inline void _emptyStringPlaceholder0() {
    TRACE("");
}
inline void _emptyStringPlaceholder1() { /* MATCH: placeholder: an inline function's strings, in the original .rodata order */
    TRACE("");
}
inline void _emptyStringPlaceholder2() { /* MATCH: placeholder: an inline function's strings, in the original .rodata order */
    TRACE("");
}

#include "Game/Movement.h"
#include "Game/Gimmick/GimmickManager.h"
#include "Game/LockOnManager.h"
#include "Game/Player/Player.h"
#include "Game/Bullet/Bullet.h"
#include "Game/EntityFactory.h"
#include "System/Graphic/Camera.h"
#include "System/Debug.h"

extern "C" {
float atan2f(float y, float x);
}
void Normalize(FVECTOR& v0, const FVECTOR& v1);
void ScaleVector(FVECTOR& v0, const FVECTOR& v1, FLOAT t);
FLOAT InnerProduct(const FVECTOR& v0, const FVECTOR& v1);
void ApplyMatrix(FVECTOR& v0, const FMATRIX& m0, const FVECTOR& v1);
void MulMatrix(FMATRIX& res, const FMATRIX& m0, const FMATRIX& m1);


static BULLET_EFFECT_INFO g_GimAngelArrowEffTbl[4] = {
    {"rec_he_common", 12, 0, 0},
    {"rec_he_common", 6, 0, 0},
    {"rec_he_common", 7, 0, 0},
    {"rec_he_common", 5, 0, 0},
};

CGimmickAngelArrow::CGimmickAngelArrow(CTask* inParent) : CBaseGimmick(inParent) {
    m_Stat = 0;
    m_Step = 0;
    m_WaitCounter = 0.0f;
    m_addRot = 0.0f;
}

CGimmickAngelArrow::~CGimmickAngelArrow() {
    ((CGimmickManager*)getSysTask(17))->setStorageGimmickState(getUniqueID(), m_Stat);
}

BOOL CGimmickAngelArrow::initalize(INT inGroupID, INT inRsrcID) {
    if (!CBaseGimmick::initalize(inGroupID, inRsrcID)) return FALSE;
    getSysTask(32);
    LockOnManager::CLockOnManager::registerLockOnObject(getHandle());
    m_pMove->enableGravity(FALSE);
    m_pMove->setFlag(1, 0);
    m_pMove->setCollisionFlag(0, 0xF);
    bodyCollOffBody(-1);
    changeMotion(1, 0, FALSE, -1, 0, 0);
    return TRUE;
}

void CGimmickAngelArrow::AfterInit() {
    m_Stat = ((CGimmickManager*)getSysTask(17))->getStorageGimmickState(getUniqueID());
    if (m_Stat == -1) m_Stat = 0;
    if (m_Stat == 0) bodyCollOnBody(-1);
}

void CGimmickAngelArrow::beginStateMain() {
    if (m_Stat == 1) out();
}

void CGimmickAngelArrow::stateMain() {
    switch (m_Step) {
    case 0:
        m_Step = 5;
        m_WaitCounter = 180.0f;
        break;
    case 5:
        if (m_WaitCounter > 0.0f) m_WaitCounter -= getFrameRateF();
        if (m_WaitCounter <= 0.0f) {
            m_WaitCounter = 0.0f;
            m_Step = 10;
        }
        break;
    case 10:
        if (loockAtPlayer()) m_Step = 20;
        break;
    case 20:
        m_Step = 30;
        m_WaitCounter = 20.0f;
        break;
    case 30:
        if (m_WaitCounter > 0.0f) m_WaitCounter -= getFrameRateF();
        if (m_WaitCounter <= 0.0f) {
            changeMotion(2, 0, FALSE, -1, 0, 0);
            m_WaitCounter = 0.0f;
            m_Step = 40;
        }
        break;
    case 40: {
        FMATRIX m = Crd(getBoneIndex("pl_rot", 0)).getWorldMatrix();
        FMATRIX m2 = Crd(getBoneIndex("attack", 0)).getWorldMatrix();
        FVECTOR dir;
        FVECTOR v;
        BULLET_PARAM param;
        dir.x = m.v[2].x;
        dir.y = m.v[2].y;
        dir.z = m.v[2].z;
        v.x = m2.v[3].x;
        v.y = m2.v[3].y;
        v.z = m2.v[3].z;
        ScaleVector(dir, dir, 5.0f);
        param.init(1, 3);
        param.iAttackPoint = 0;
        param.AtkAttr = 5;
        param.AtkKind = 10;
        param.fSpeed = 0.2f;
        param.vPos = v;
        param.vRot.x = 0.0f;
        param.vRot.z = 0.0f;
        param.vRot.y = atan2f(dir.x, dir.z);
        param.fCollRadius = 0.2f;
        param.pEffectTbl = g_GimAngelArrowEffTbl;
        param.uiFlag = 0x41108940;
        param.hHomingTarget = g_pPlayer->getHandle();
        param.tqFadeTime = 500;
        param.tqHomingTime2 = 750;
        param.fHomingRate = 0.125f;
        param.fMaxDist = 50.0f;
        CEntityFactory::getInstance()->createBullet(param, NULL);
        m_Step = 50;
        m_WaitCounter = 80.0f;
        break;
    }
    case 50:
        if (isMotionEnd()) {
            changeMotion(3, 0, FALSE, -1, 0, 0);
            m_Step = 60;
        }
        break;
    case 60:
        if (m_WaitCounter > 0.0f) m_WaitCounter -= getFrameRateF();
        if (m_WaitCounter <= 0.0f) {
            changeMotion(1, 0, FALSE, -1, 0, 0);
            m_Step = 10;
        }
        break;
    }
}

void CGimmickAngelArrow::endStateMain() {}

BOOL CGimmickAngelArrow::loockAtPlayer() {
    INT ret = FALSE;
    CTask* pTask = getSysTask(3);
    FMATRIX mPl = pTask->Crd(0).getWorldMatrix();
    FVECTOR dir;
    FVECTOR vec;
    if (calcDistanceXZ((CEntity*)pTask) > 15.0f) return FALSE;
    dir.x = mPl.v[2].x;
    dir.y = mPl.v[2].y;
    dir.z = mPl.v[2].z;
    vec = pTask->Crd(0).getWorldMatrix().v[3];
    {
        const FVECTOR& pos = getPos();
        vec -= pos;



    }
    if (vec.y > 0.5f) return FALSE;
    if (vec.y < -0.5f) return FALSE;
    {
        FVECTOR v;
        INT index = getBoneIndex("pl_rot", 0);
        FMATRIX m = Crd(index).getWorldMatrix();
        FMATRIX mParent = Crd(index).getParent()->getWorldMatrix();
        FLOAT errorcheck;
        FMATRIX tm;
        FLOAT rty;
        v.x = m.v[3].x;
        v.y = m.v[3].y;
        v.z = m.v[3].z;
        m.v[3].w = 1.0f;
        m.v[3].x = 0.0f;
        m.v[3].y = 0.0f;
        m.v[3].z = 0.0f;
        dir.x = m.v[2].x;
        dir.y = m.v[2].y;
        dir.z = m.v[2].z;
        dir.w = 0.0f;
        Normalize(dir, dir);
        vec = pTask->Crd(0).getWorldMatrix().v[3];
        vec.x -= getPos().x;
        vec.z -= getPos().z;
        vec.y = 0.0f;
        vec.w = 0.0f;
        Normalize(vec, vec);
        ScaleVector(dir, dir, 5.0f);
        ScaleVector(vec, vec, 5.0f);
        errorcheck = InnerProduct(dir, vec);
        UnitMatrix(tm);
        RotMatrixY(tm, tm, 1.5707964f);
        ApplyMatrix(dir, tm, dir);
        rty = InnerProduct(dir, vec);
        if (rty > 4.0f || (errorcheck < 0.0f && rty >= 0.0f)) {
            m_addRot += getFrameRateF() * 4.0f * 0.017453292f;
        } else if (rty < -4.0f || (errorcheck < 0.0f && rty < 0.0f)) {
            m_addRot += getFrameRateF() * -4.0f * 0.017453292f;
        } else {
            FVECTOR target;
            ret = TRUE;
            calcDirection(target, pTask->Crd(0).getWorldMatrix().v[3]);
            m_addRot = target.y - getRot().y;
        }
    }
    return ret;
}

static inline void _unitMatrixVu0(FMATRIX& m) {
    asm volatile("vmr32.xyzw $vf1, $vf0\n"
                 "sqc2 $vf0, 0x30(%0)\n"
                 "vmr32.xyzw $vf2, $vf1\n"
                 "sqc2 $vf1, 0x20(%0)\n"
                 "vmr32.xyzw $vf3, $vf2\n"
                 "sqc2 $vf2, 0x10(%0)\n"
                 "sqc2 $vf3, 0x0(%0)\n"
                 :
                 : "r"(&m)
                 : "memory");
}

void CGimmickAngelArrow::lastUpdate() {
    FMATRIX mMat;
    if (m_addRot < -3.14f) {
        m_addRot += 6.28f;
    } else if (m_addRot > 3.14f) {
        m_addRot -= 6.28f;
    }
    _unitMatrixVu0(mMat);
    RotMatrixY(mMat, mMat, m_addRot);
    MulMatrix(Crd(getBoneIndex("pl_rot", 0)).getLocalMatrix(), Crd(getBoneIndex("pl_rot", 0)).getLocalMatrix(), mMat);
    Crd(0).touch();
}

void CGimmickAngelArrow::beginStateHitAttack() {
    if (m_Stat == 0) createPrize(0, 0);
    changeMotion(1, 0, FALSE, -1, 1, 0);
    m_Stat = 1;
    setEntityFlag(1, 0);
    changeGimmickState(0);
}

void CGimmickAngelArrow::stateHitAttack() {}

void CGimmickAngelArrow::endStateHitAttack() {}
// SOURCE DATA: .data .rodata
