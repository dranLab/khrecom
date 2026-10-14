#define TASK_H_NO_SLC_OVERFLOW_MSG
#define GPRIM_H_PATH "System/Graphic/GPrim.h"
#include "Game/Gimmick/GimmickSearchLaser.h"
#include "Game/BodyCollision.h"
#include "System/Graphic/Effect/effect.h"
#include "System/Graphic/Gprim.h"
#include "System/Graphic/2d/2DMan.h"
#include "Game/CardBattle.h"
#include "System/Graphic/Str.h"
#include "Game/Collision/CollUtilA.h"
/* Game/Collision/CollUtil.h's strings, labelled for the still-assembly stateMain (CSphereCollision::collideMap's
 * singleton assert). With stateMain compiled: #include "Game/Collision/CollUtil.h" instead. */
asm(".section .rodata\n.align 3\n.globl D_004D4A28\nD_004D4A28:\n.asciz \"Game/Collision/CollUtil.h\"\n"
    ".align 3\n.asciz \"Game/Collision/CollUtil.h(185)\"\n"
    ".align 3\n.globl D_004D4A68\nD_004D4A68:\n.asciz \"m_pInstance\"\n.previous");

/* Placeholders: three empty strings and the task container overflow message (see System/Task.h) follow
 * CollUtil.h's strings in .rodata (as in GimmickPlantParent); which headers they come from is not known. */
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

#include "Game/Collision/CollObj.h"
#include "System/Graphic/Effect/EffectMan.h"
#include "Game/Movement.h"
#include "Game/AttackMotion.h"
#include "Game/Gimmick/GimmickManager.h"
#include "Game/LockOnManager.h"
#include "Game/EntityFactory.h"
#include "Game/Player/Player.h"
#include "System/TaskMan.h"
#include "System/Rnd.h"
#include "include_asm.h"

/* Inline in the original's collision header: the weak copies of its destructors and collideMap end this
 * object (the vtables and the named copies are other objects'). */
class CSphereCollision : public CCollision {
public:
    CSphereCollision() { m_Type = COLL_TYPE_SPHERE; }
    virtual ~CSphereCollision() {}
    void setRadius(FLOAT inRadius) {
        m_shiftY = inRadius;
        m_Radius = inRadius;
    }
    virtual INT collideMap() { return CCollUtil::getInstance()->_detectMCD(*this); }
};

struct DropPrize {
    INT StockKind;
    INT Kind; /* CARD_KIND */
    SHORT Num;
    SHORT No;
};

// System/Coord.h
void AddVector(FVECTOR& v0, const FVECTOR& v1, const FVECTOR& v2);
void SubVector(FVECTOR& v0, const FVECTOR& v1, const FVECTOR& v2);
void ScaleVector(FVECTOR& v0, const FVECTOR& v1, FLOAT f);
void Normalize(FVECTOR& v0, const FVECTOR& v1);
void OuterProduct(FVECTOR& v0, const FVECTOR& v1, const FVECTOR& v2);
FLOAT InnerProduct(const FVECTOR& v0, const FVECTOR& v1);
void ApplyMatrix(FVECTOR& v0, const FMATRIX& m0, const FVECTOR& v1);
void ApplyMatrixXYZ(FVECTOR& v0, const FMATRIX& m0, const FVECTOR& v1);
extern "C" void sceVu0UnitMatrix(float m[4][4]);
extern "C" void sceVu0RotMatrixY(float m0[4][4], float m1[4][4], float ry);

static inline void _unitMatrix(FMATRIX& m) {
    sceVu0UnitMatrix(m.m);
}
static inline void _rotMatrixY(FMATRIX& m0, FMATRIX& m1, float ry) {
    sceVu0RotMatrixY(m0.m, m1.m, ry);
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

CGimmickSearchLaser::CGimmickSearchLaser(CTask* inParent) : CBaseGimmick(inParent) {
    m_Step = 0;
    m_hChild[0] = -1;
    m_hChild[1] = -1;
    m_hChild[2] = -1;
    m_hChild[3] = -1;
}

CGimmickSearchLaser::~CGimmickSearchLaser() {
    ((CGimmickManager*)getSysTask(17))->setStorageGimmickState(getUniqueID(), m_Stat);
}

BOOL CGimmickSearchLaser::initalize(INT inGroupID, INT inRsrcID) {
    if (!CBaseGimmick::initalize(inGroupID, inRsrcID)) return FALSE;
    m_pMove->enableGravity(FALSE);
    m_pMove->setFlag(1, TRUE);
    m_pMove->setCollisionFlag(0xF, FALSE);
    m_pMove->setFlag(8, TRUE);
    bodyCollOffBody(-1);
    getSysTask(32);
    LockOnManager::CLockOnManager::registerLockOnObject(getHandle());
    return TRUE;
}

void CGimmickSearchLaser::AfterInit() {
    CEntity* entity;
    INT i;

    m_Stat = ((CGimmickManager*)getSysTask(17))->getStorageGimmickState(getUniqueID());
    if (m_Stat == -1) m_Stat = 0;
    if (m_Stat) return;
    changeMotion(1, 0, FALSE, 0, 0, 0);
    bodyCollOnBody(-1);
    getSysTask(17);
    for (i = 0; i < 4; i++) {
        entity = CEntityFactory::getInstance()->createEntity(11, 0x521, getSysTask(17), NULL);
        entity->warp(getPos());
        entity->setRot(getRot());
        m_hChild[i] = entity->getHandle();
        ((CGimmickSearchLaserCircle*)CTaskMan::getTask(m_hChild[i]))->m_ParentPos = getPos();
    }
}

void CGimmickSearchLaser::beginStateMain() {
    if (m_Stat) out();
}

void CGimmickSearchLaser::stateMain() {
    FLOAT y;
    INT i;

    switch (m_Step) {
    case 0:
        m_Wait = 90.0f;
        m_Step = 1;
        break;
    case 5:
        if (calcDistanceXZ(g_pPlayer) <= 8.0f) {
            y = g_pPlayer->getPos().y - getPos().y;
            if (y <= 2.0f && y >= -2.0f) {
                for (i = 0; i < 4; i++) {
                    CGimmickSearchLaserCircle* pCircle = (CGimmickSearchLaserCircle*)CTaskMan::getTask(m_hChild[i]);
                    if (pCircle && pCircle->changeStepAttack()) {
                        m_Wait = 100.0f;
                        m_Step = 10;
                        break;
                    }
                }
            }
        }
        break;
    case 1:
    case 10:
        m_Wait -= getFrameRateF();
        if (m_Wait <= 0.0f) m_Step = 5;
        break;
    case 15:
        break;
    }
}

void CGimmickSearchLaser::endStateMain() {}

void CGimmickSearchLaser::beginStateHitAttack() {
    INT i;

    if (m_pBodyColl->getHitCollBody()->getDamageKind() == 2) {
        DropPrize prize;
        prize.StockKind = 0;
        prize.Kind = 0;
        prize.Num = 0;
        prize.No = 0;
        if (!m_Stat) createPrize(0, 0);
        changeMotion(2, 0, FALSE, -1, 0, 0);
        setEntityFlag(1, 0);
        for (i = 0; i < 4; i++) {
            ((CGimmickSearchLaserCircle*)CTaskMan::getTask(m_hChild[i]))->killLaser();
        }
        m_Stat = 1;
        saveStat(1);
    }
    changeGimmickState(0);
}

void CGimmickSearchLaser::stateHitAttack() {}

void CGimmickSearchLaser::endStateHitAttack() {}

CGimmickSearchLaserCircle::CGimmickSearchLaserCircle(CTask* inParent) : CBaseGimmick(inParent) {
    m_Step = 0;
}

CGimmickSearchLaserCircle::~CGimmickSearchLaserCircle() {}

BOOL CGimmickSearchLaserCircle::initalize(INT inGroupID, INT inRsrcID) {
    return CBaseGimmick::initalize(inGroupID, inRsrcID) != 0;
}

void CGimmickSearchLaserCircle::AfterInit() {
    setEntityFlag(1, 0);
    changeMotion(1, 0, FALSE, 0, 0, 0);
    m_pMove->setRadius(1.0f);
}

void CGimmickSearchLaserCircle::beginStateMain() {}

INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Game/Gimmick/GimmickSearchLaser", stateMain__25CGimmickSearchLaserCircle);
/* Its .rodata: the effect name, the jump table, and the unnamed copies of CSphereCollision's and CCollision's
 * vtables (constructed in this function; the named copies are other objects'). */
asm(".section .rodata\n.align 4\n.globl D_004D4AF0\nD_004D4AF0:\n.asciz \"rec_ca_common.dpx\"\n"
    ".align 4\n.globl jtbl_004D4B10\njtbl_004D4B10:\n"
    ".word .L002ED470\n"
    ".word .L002ECF7C\n"
    ".word .L002ED134\n"
    ".word .L002ED194\n"
    ".word .L002ED22C\n"
    ".word .L002ED1EC\n"
    ".word .L002ED2EC\n"
    ".word .L002ED34C\n"
    ".word .L002ED3F0\n"
    ".word .L002ED430\n"
    ".align 3\n.word 0, 0, 0, _$_16CSphereCollision, 0, collideMap__16CSphereCollision, 0, 0\n"
    ".word 0, 0, 0, _$_10CCollision, 0, 0\n"
    ".previous");
/* Not matching (only scheduling in case 1): the original loads CCollUtil::m_pInstance after the two lq of
 * setStartPos/setEndPos and stores m_inWS first (m_inWE in the delay slot); ours loads m_pInstance first and
 * stores m_inWE first. Everything else matches. With it compiled, the CSphereCollision/CCollision inline
 * functions are emitted from source (checked: they match func_002EDC48..func_002EDCA8). */
#if 0
void CGimmickSearchLaserCircle::stateMain() {
    CEffect* efx;
    GimParam* param;

    switch (m_Step) {
    case 0:
        break;
    case 1: {
        {
            FVECTOR v;
            FVECTOR v2;
            FMATRIX m;
            v.x = 0.0f;
            v.y = 0.0f;
            v.z = 3.0f;
            v.w = 1.0f;
            _unitMatrix(m);
            _rotMatrixY(m, m, (RND::genrand_int31() % 360 - 180) * 0.017453292f);
            ApplyMatrix(v, m, v);
            AddVector(v, m_ParentPos, v);
            CSphereCollision cColl;
            cColl.setRadius(0.5f);
            cColl.setStartPos(v);
            cColl.setEndPos(m_ParentPos);
            if (cColl.collideMap()) break;
            warp(v);
            m_pMove->enableGravity(TRUE);
            m_pMove->land(5.0f, 1.0f, FALSE);
            HANDLE h = setEffect("rec_ca_common.dpx", 0, &getPos(), NULL, 0.0f);
            m_Step = 2;
            m_hEfx = h;
            m_Wait = 60.0f;
            m_Speed = 0.6f;
        }
        m_KillFlag = 0;
        m_MoveStep = 0;
        break;
    }
    case 2:
        if (!CEffectMan::getEffect(m_hEfx)) {
            m_hEfx = attachEffect("rec_ca_common.dpx", 1, &Crd(0), NULL, NULL, 0.0f);
            m_Step = 3;
            m_Wait = 30.0f;
        }
        break;
    case 3:
        if (m_KillFlag) goto kill;
        m_Wait -= getFrameRateF();
        if (m_Wait <= 0.0f) {
            m_PlTarget = setMoveTargetPos();
            m_Step = 5;
        }
        break;
    case 5:
        if (m_KillFlag) goto kill;
        if (moveTargetPos()) {
            if (m_PlTarget) {
                m_Step = 4;
            } else {
                m_Step = 3;
                m_Wait = 30.0f;
            }
        }
        break;
    case 4:
        if (calcDistanceXZ(g_pPlayer) <= 0.5f) {
            m_Step = 6;
            CEffectMan::getEffect(m_hEfx)->kill();
            m_hEfx = setEffect("rec_ca_common.dpx", 2, &getPos(), NULL, 0.0f);
            break;
        }
    kill:
        CEffectMan::getEffect(m_hEfx)->kill();
        m_hEfx = setEffect("rec_ca_common.dpx", 2, &getPos(), NULL, 0.0f);
        m_Step = 9;
        break;
    case 6:
        CEffectMan::getEffect(m_hEfx)->kill();
        m_hEfx = attachEffect("rec_ca_common.dpx", 3, &Crd(0), NULL, NULL, 0.0f);
        m_Wait = 15.0f;
        m_Step = 7;
        break;
    case 7:
        m_Wait -= getFrameRateF();
        if (m_Wait <= 0.0f) {
            changeMotion(2, 0, FALSE, 0, 0, 0);
            if (m_pAttack && (param = getGimParam())) {
                m_pAttack->setAttackParam(param->AtkPow, (ATK_ATTR)param->AtkAttr);
            }
            m_Step = 8;
            m_Wait = 18.0f;
        }
        break;
    case 8:
        m_Wait -= getFrameRateF();
        if (m_Wait <= 0.0f) m_Step = 9;
        break;
    case 9:
        efx = CEffectMan::getEffect(m_hEfx);
        changeMotion(1, 0, FALSE, 0, 0, 0);
        if (!efx) m_Step = 0;
        break;
    }
}

#endif

void CGimmickSearchLaserCircle::endStateMain() {}

BOOL CGimmickSearchLaserCircle::changeStepAttack() {
    if (m_Step == 0) {
        m_Step = 1;
        return TRUE;
    }
    return FALSE;
}

BOOL CGimmickSearchLaserCircle::followPlayer() {
    FVECTOR v = g_pPlayer->getPos();
    if (calcDistanceXZ(g_pPlayer) <= 0.13f) {
        v.y = getPos().y;
        setPos(v);
        return TRUE;
    }
    SubVector(v, v, getPos());
    v.y = 0.0f;
    Normalize(v, v);
    ScaleVector(v, v, getFrameRateF() * 0.13f);
    AddVector(v, getPos(), v);
    setPos(v);
    return FALSE;
}

BOOL CGimmickSearchLaserCircle::setMoveTargetPos() {
    switch (m_MoveStep) {
    case 0: {
        m_MoveStep = 10;
        FVECTOR v;
        FVECTOR v2;
        FVECTOR dirPl;
        FMATRIX m;
        _unitMatrixVu0(m);
        sceVu0RotMatrixY(m.m, m.m, g_pPlayer->getRot().y);
        v.x = 0.0f;
        v.y = 0.0f;
        v.z = 1.0f;
        v.w = 1.0f;
        SubVector(dirPl, g_pPlayer->getPos(), getPos());
        Normalize(dirPl, dirPl);
        ApplyMatrixXYZ(v, m, v);
        v2.set(0.0f, 1.0f, 0.0f);
        OuterProduct(v, v, v2);
        if (InnerProduct(v, dirPl) <= 0.0f) {
            ScaleVector(v, v, (RND::genrand_int31() % 2000) / 1000.0f + 2.0f);
        } else {
            ScaleVector(v, v, -((RND::genrand_int31() % 2000) / 1000.0f) - 2.0f);
        }
        v2.set(0.0f, 0.0f, (RND::genrand_int31() % 2000) / 1000.0f + 2.0f);
        ApplyMatrixXYZ(v2, m, v2);
        AddVector(v, v2, v);
        AddVector(m_TargetPos, v, g_pPlayer->getPos());
        break;
    }
    case 10: {
        m_MoveStep = 10;
        FVECTOR v;
        FVECTOR v2;
        FVECTOR dirPl;
        FMATRIX m;
        _unitMatrixVu0(m);
        sceVu0RotMatrixY(m.m, m.m, g_pPlayer->getRot().y);
        v.x = 0.0f;
        v.y = 0.0f;
        v.z = 1.0f;
        v.w = 1.0f;
        SubVector(dirPl, g_pPlayer->getPos(), getPos());
        Normalize(dirPl, dirPl);
        ApplyMatrixXYZ(v, m, v);
        v2.set(0.0f, 1.0f, 0.0f);
        OuterProduct(v, v, v2);
        if (InnerProduct(v, dirPl) <= 0.0f) {
            ScaleVector(v, v, -((RND::genrand_int31() % 2000) / 1000.0f) - 1.0f);
        } else {
            ScaleVector(v, v, (RND::genrand_int31() % 2000) / 1000.0f + 1.0f);
        }
        v2.set(0.0f, 0.0f, (RND::genrand_int31() % 2000) / 1000.0f + 2.0f);
        ApplyMatrixXYZ(v2, m, v2);
        AddVector(v, v2, v);
        AddVector(m_TargetPos, v, g_pPlayer->getPos());
        m_MoveStep = 20;
        break;
    }
    case 20: {
        FVECTOR v = g_pPlayer->getPos();
        v.y = getPos().y;
        m_TargetPos = v;
        m_MoveStep = 30;
        m_Speed = 0.8f;
        m_NextWait = 40.0f;
        return TRUE;
    }
    default:
        m_MoveStep = 0;
        return TRUE;
    }
    m_NextWait = 40.0f;
    return FALSE;
}

BOOL CGimmickSearchLaserCircle::moveTargetPos() {
    FVECTOR v = m_TargetPos;
    if (calcDistanceXZ(m_TargetPos) <= getFrameRateF() * m_Speed) {
        setPos(v);
        return TRUE;
    }
    SubVector(v, v, getPos());
    v.y = 0.0f;
    Normalize(v, v);
    ScaleVector(v, v, getFrameRateF() * m_Speed);
    AddVector(v, getPos(), v);
    setPos(v);
    m_NextWait -= getFrameRateF();
    if (m_NextWait <= 0.0f) return TRUE;
    return FALSE;
}

void CGimmickSearchLaserCircle::killLaser() {
    m_KillFlag = 1;
}

/* Weak in the original (inline in the class; see the header). */
void CGimmickSearchLaserCircle::setPearentPos(const FVECTOR& in_pos) {
    m_ParentPos = in_pos;
}

/* Weak copies of CCollision's and CSphereCollision's destructors and CSphereCollision::collideMap: they come
 * from source once stateMain matches (it constructs the CSphereCollision); then setPearentPos can be defined in
 * the class again, CollUtil.h included instead of its strings and stateMain's .rodata asm dropped. */
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Game/Gimmick/GimmickSearchLaser", func_002EDC48);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Game/Gimmick/GimmickSearchLaser", func_002EDC78);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Game/Gimmick/GimmickSearchLaser", func_002EDCA8);

// SOURCE DATA: .rodata
