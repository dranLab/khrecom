
#define GPRIM_H_PATH "System/Graphic/GPrim.h"
#include "Game/Gimmick/GimmickFountain.h"
#include "Game/BodyCollision.h"
#include "System/Graphic/Effect/effect.h"
#include "System/Graphic/Gprim.h"
#include "System/Graphic/2d/2DMan.h"
#include "Game/CardBattle.h"
#include "System/Graphic/Str.h"
#include "Game/Collision/CollUtil.h"

/* Placeholder: an empty string follows CollUtil.h's strings in .rodata (as in GimmickFallTomb); which
 * header it comes from is not known. */
inline void _emptyStringPlaceholder() {
    TRACE("");
}

#include "System/Graphic/Model.h"
#include "Game/Movement.h"
#include "Game/AttackMotion.h"
#include "Game/Gimmick/GimmickManager.h"
#include "Game/Player/Player.h"
#include "Game/Bullet/Bullet.h"
#include "Game/EntityFactory.h"
#include "System/Graphic/Camera.h"
#include "System/Debug.h"

extern "C" {
void sceVu0UnitMatrix(FMATRIX& m);
void sceVu0RotMatrixY(FMATRIX& m0, FMATRIX& m1, float ry);
float atan2f(float y, float x);
}
void Normalize(FVECTOR& v0, const FVECTOR& v1);
void ScaleVector(FVECTOR& v0, const FVECTOR& v1, FLOAT t);
FLOAT InnerProduct(const FVECTOR& v0, const FVECTOR& v1);
void ApplyMatrix(FVECTOR& v0, const FMATRIX& m0, const FVECTOR& v1);
void MulMatrix(FMATRIX& res, const FMATRIX& m0, const FMATRIX& m1);

static inline void _unitMatrix(FMATRIX& m) {
    sceVu0UnitMatrix(m);
}
static inline void _rotMatrixY(FMATRIX& m, FLOAT r) {
    sceVu0RotMatrixY(m, m, r);
}

static BULLET_EFFECT_INFO g_GimFountainEffTbl[4] = {
    {"rec_nm_common", 1, 0, 0},
    {"rec_nm_common", 3, 0, 0},
    {"rec_nm_common", 2, 0, 0},
    {"rec_nm_common", 0, 0, 0},
};

CGimmickFountain::CGimmickFountain(CTask* inParent) : CBaseGimmick(inParent) {
    m_Step = 0;
    m_Stat = 0;
    m_addRot = 0.0f;
    m_WaitCounter = 0.0f;
}

CGimmickFountain::~CGimmickFountain() {}

BOOL CGimmickFountain::initalize(INT inGroupID, INT inRsrcID) {
    if (!CBaseGimmick::initalize(inGroupID, inRsrcID)) return FALSE;
    m_pMove->enableGravity(FALSE);
    m_pMove->setFlag(1, 0);
    m_pMove->setCollisionFlag(0, 0xF);
    m_pMove->setFlag(8, 0);
    bodyCollOffBody(-1);
    rideModelEnableAllCollision(FALSE);
    changeMotion(1, 0, FALSE, -1, 0, 0);
    return TRUE;
}

void CGimmickFountain::AfterInit() {
    bodyCollOnBody(-1);
}

void CGimmickFountain::beginStateMain() {}

void CGimmickFountain::stateMain() {
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
        changeMotion(2, 0, FALSE, -1, 0, 0);
        m_Step = 40;
        m_WaitCounter = 35.0f;
        break;
    case 40:
        m_WaitCounter -= getFrameRateF();
        if (m_WaitCounter <= 0.0f) {
            FMATRIX m = Crd(getBoneIndex("pl_rot", 0)).getWorldMatrix();
            FMATRIX m2 = Crd(getBoneIndex("attack", 0)).getWorldMatrix();
            FVECTOR dir;
            FVECTOR v;
            GimParam* GimParam;
            INT atkPow;
            BULLET_PARAM param;
            dir.x = m.v[2].x;
            dir.y = m.v[2].y;
            dir.z = m.v[2].z;
            v.x = m2.v[3].x;
            v.y = m2.v[3].y;
            v.z = m2.v[3].z;
            ScaleVector(dir, dir, 5.0f);
            GimParam = getGimParam();
            if (GimParam != NULL) atkPow = GimParam->AtkPow;
            param.init(1, 3);
            param.AtkAttr = 1;
            param.AtkKind = 1;
            param.iAttackPoint = atkPow;
            param.vPos = v;
            param.fSpeed = 0.3f;
            param.vRot.x = 0.0f;
            param.vRot.z = 0.0f;
            param.vRot.y = atan2f(dir.x, dir.z);
            param.pEffectTbl = g_GimFountainEffTbl;
            param.fCollRadius = 0.4f;
            param.uiFlag = 0x1000108;
            CEntityFactory::getInstance()->createBullet(param, NULL);
            m_Step = 50;
            m_WaitCounter = 80.0f;
        }
        break;
    case 50:
        if (isMotionEnd()) {
            changeMotion(1, 0, FALSE, -1, 0, 0);
            m_Step = 10;
        }
        break;
    }
}

void CGimmickFountain::endStateMain() {}

BOOL CGimmickFountain::loockAtPlayer() {
    INT ret = FALSE;
    CTask* pTask = getSysTask(3);
    FMATRIX mPl = pTask->Crd(0).getWorldMatrix();
    FVECTOR dir;
    FVECTOR vec;
    if (!dbgCheckUser(KKIKUCHI) && calcDistanceXZ((CEntity*)pTask) > 8.0f) return FALSE;
    dir.x = mPl.v[2].x;
    dir.y = mPl.v[2].y;
    dir.z = mPl.v[2].z;
    vec = pTask->Crd(0).getWorldMatrix().v[3];
    {
        const FVECTOR& pos = getPos();
        vec.x -= pos.x;
        vec.y -= pos.y;
        vec.z -= pos.z;
        vec.w -= pos.w;
    }
    if (!dbgCheckUser(KKIKUCHI)) {
        if (vec.y > 0.5f) return FALSE;
        if (vec.y < -0.5f) return FALSE;
    }
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
        _unitMatrix(tm);
        _rotMatrixY(tm, 1.5707964f);
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

void CGimmickFountain::lastUpdate() {
    FMATRIX mMat;
    if (m_addRot < -3.14f) {
        m_addRot += 6.28f;
    } else if (m_addRot > 3.14f) {
        m_addRot -= 6.28f;
    }
    _unitMatrixVu0(mMat);
    sceVu0RotMatrixY(mMat, mMat, m_addRot);
    MulMatrix(Crd(getBoneIndex("pl_rot", 0)).getLocalMatrix(), Crd(getBoneIndex("pl_rot", 0)).getLocalMatrix(), mMat);
    Crd(0).m_Flag = 1;
}
// SOURCE DATA: .data .rodata
