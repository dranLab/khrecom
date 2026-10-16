#define TASK_H_NO_SLC_OVERFLOW_MSG
#define GPRIM_H_PATH "System/Graphic/GPrim.h"
#define BATTLEENEMY_H_PATH "c:/KHCOM/MainApp/Game/Enemy/BattleEnemy.h"
#include "System/Graphic/Gprim.h"
#include "System/Graphic/2d/2DMan.h"
#include "Game/CardBattle.h"
#include "Game/Enemy/EnemyWizard.h"
#include "System/Graphic/Anim.h"
#include "System/Graphic/Str.h"
#include "Game/Collision/CollUtil.h"

/* Placeholder: .rodata has an empty string literal after CollUtil.h's strings (from an unidentified header). */
inline void _enemyWizardEmptyString() {
    TRACE("");
}

#include "Game/BodyCollision.h"
#include "Game/Bullet/Bullet.h"
#include "Game/EntityFactory.h"
#include "Game/Movement.h"
#include "Game/SceneEnv.h"
#include "System/Graphic/Camera.h"
#include "Game/Collision/CollMap.h"
#include "System/Math3D.h"
#include "System/Rnd.h"
#include <math.h>

#define ASSERT(e) if (!(e)) AssertApp(__FILE__, __LINE__, #e, 0)

extern CEntity* g_pPlayer; /* a CPlayer (Game/Player/Player.h) */
FVECTOR getPlayerPos(BOOL flag);
void SubVector(FVECTOR& v0, const FVECTOR& v1, const FVECTOR& v2);
void Normalize(FVECTOR& v0, const FVECTOR& v1);
void ApplyMatrix(FVECTOR& v0, const FMATRIX& m0, const FVECTOR& v1);
extern "C" float atan2f(float y, float x);

static inline FLOAT _sqrtf(FLOAT x) {
    FLOAT ret;
    asm volatile("sqrt.s %0, %1" : "=f"(ret) : "f"(x));
    return ret;
}

namespace Wizard {
static CHAR* IdlingMotion = "e025_001";
static CHAR* SpeedFiraMotion = "e025_050";
static CHAR* RollingBlizzaraMotion = "e025_051";
static CHAR* RandomThundra1Motion = "e025_052";
static CHAR* RandomThundra2Motion = "e025_053";
static CHAR* RandomThundra3Motion = "e025_054";
static const FVECTOR WeaponOffset = {{0.55f, 0.0f, 0.0f, 1.0f}};
}

CEnemyWizard::CEnemyWizard(CTask* inParent) : CHeartless(inParent, 0, 0) {
    setEntityFlag(0x80, 0);
    setBtlEnemyFlag(0x100);
}

CEnemyWizard::~CEnemyWizard() {}

void CEnemyWizard::beginStateMain() {
    commonFlyInit(Wizard::IdlingMotion, Wizard::IdlingMotion, 0.75f, 1.75f);
}

void CEnemyWizard::stateMain() {
    commonFly();
}

void CEnemyWizard::endStateMain() {
    commonFlyEnd();
}

void CEnemyWizard::beginStateAttack1() {
    changeMotionFileName(Wizard::SpeedFiraMotion, 0, FALSE, -1, 0, 0);
    m_iStep = 0;
}

void CEnemyWizard::stateAttack1() {
    static const BULLET_EFFECT_INFO g_EffTbl[4] = {
        {NULL, 0, -1, 0}, {"_ex_2150", 11, -1, 0}, {"_ex_2150", 12, -1, 0}, {"_ex_2150", 10, -1, 0}};
    static const FVECTOR g_vOffset = {{-0.15f, 0.35f, 0.58f, 1.0f}};
    if (m_iStep == 0) {
        playerHomingY(0.25f);
        if (getMotionCurrentTime() >= 1200) {
            BULLET_PARAM param;
            FVECTOR pos;
            FVECTOR dir;
            FLOAT ret;
            FLOAT _y;
            param.init(1, 3);
            param.AtkAttr = getAttackAttr(0);
            param.iAttackPoint = (INT)getAttackPoint(0, (ATK_KIND)1, 0, NULL, 0);
            param.fSpeed = 0.4f;
            ApplyMatrix(param.vPos, Crd(0).getWorldMatrix(), g_vOffset);
            param.uiFlag = 0x8800;
            param.fCollRadius = 0.25f;
            param.pEffectTbl = (BULLET_EFFECT_INFO*)g_EffTbl;
            param.hHomingTarget = getPlayerHandle();
            param.fMaxDist = 900.0f;
            param.tqHomingTime2 = 750;
            param.fHomingRate = 2.5f;
            g_pPlayer->getLockOnPos(pos);
            SubVector(dir, pos, param.vPos);
            Normalize(dir, dir);
            ret = -atan2f(dir.y, _sqrtf(dir.z * dir.z + dir.x * dir.x));
            _y = getRot().y;
            param.vRot.set(ret, _y, 0.0f);
            CEntityFactory::getInstance()->createBullet(param, this);
            m_iStep++;
        }
    }
    if (isMotionEnd()) {
        endCardBattle(NULL, 6000);
    }
}

void CEnemyWizard::endStateAttack1() {}

void CEnemyWizard::beginStateAttack2() {
    changeMotionFileName(Wizard::RollingBlizzaraMotion, 0, FALSE, -1, 0, 0);
    m_iStep = 0;
}

void CEnemyWizard::stateAttack2() {
    static const BULLET_EFFECT_INFO g_EffTbl[4] = {
        {NULL, 0, -1, 0}, {"_ex_2150", 18, -1, 0}, {"_ex_2150", 19, -1, 0}, {"_ex_2150", 16, -1, 0}};
    if (m_iStep == 0) {
        playerHomingY(0.25f);
        if (getMotionCurrentTime() >= 2500) {
            BULLET_PARAM param;
            FVECTOR pos;
            FVECTOR dir;
            FLOAT ret;
            FLOAT _y;
            param.init(1, 3);
            param.AtkAttr = getAttackAttr(1);
            param.iAttackPoint = (INT)getAttackPoint(1, (ATK_KIND)1, 0, NULL, 0);
            param.fSpeed = 0.2f;
            ApplyMatrix(pos, *getBoneMatrix("wep0"), Wizard::WeaponOffset);
            param.vPos = pos;
            param.fMaxDist = 900.0f;
            param.pEffectTbl = (BULLET_EFFECT_INFO*)g_EffTbl;
            param.fCollRadius = 0.25f;
            g_pPlayer->getLockOnPos(pos);
            SubVector(dir, pos, param.vPos);
            Normalize(dir, dir);
            ret = -atan2f(dir.y, _sqrtf(dir.z * dir.z + dir.x * dir.x));
            _y = getRot().y;
            param.vRot.set(ret, _y, 0.0f);
            CEntityFactory::getInstance()->createBullet(param, this);
            m_iStep++;
        }
    }
    if (isMotionEnd()) {
        endCardBattle(NULL, 6000);
    }
}

void CEnemyWizard::endStateAttack2() {}

void CEnemyWizard::beginStateAttack3() {
    changeMotionFileName(Wizard::RandomThundra1Motion, 0, FALSE, -1, 0, 0);
    m_iStep = 0;
    FVECTOR vColor;
    vColor.set(0.5f, 0.5f, 0.5f);
    CSceneEnv::setColor((ObjType::eObjBit)0x1800, 30.0f, vColor, NULL, 0);
}

#line 210
void CEnemyWizard::stateAttack3() {
    static const BULLET_EFFECT_INFO g_EffTbl[4] = {
        {"_ex_2150", 22, -1, 0}, {"_ex_2150", 23, -1, 0}, {NULL, 0, -1, 0}, {NULL, 0, -1, 0}};
    switch (m_iStep) {
    case 0:
        if (isMotionEnd()) {
            changeMotionFileName(Wizard::RandomThundra2Motion, 0, FALSE, -1, 0, 0);
            setTimer(9000);
            m_iStep++;
        }
        break;
    case 1:
        if (getTimer() % 250 == 0) {
            FVECTOR pos = getPlayerPos(FALSE);
            FLOAT fLen = RND::genrand_real1() * 3.0f;
            FLOAT fRot = Math3D::LimitPI(RND::genrand_real1() * 360.0f * 0.017453292f);
            pos.x += fLen * sinf(fRot);
            pos.z += fLen * cosf(fRot);
            CCollMap* coll = m_pMove->getMapCollision();
            ASSERT(coll);
            coll->setStartPos(pos);
            pos.y -= 5.0f;
            coll->setStartPos(pos); /* (sic: the original sets the start position again) */
            if (coll->collideMap()) {
                pos = *coll->getOutPos();
            } else {
                pos.y += 5.0f;
            }
            BULLET_PARAM param;
            param.init(1, 3);
            param.AtkKind = 29;
            param.AtkAttr = getAttackAttr(2);
            param.iAttackPoint = (INT)getAttackPoint(2, (ATK_KIND)1, 0, NULL, 0);
            param.fSpeed = 5.0f;
            param.vPos = pos;
            param.pEffectTbl = (BULLET_EFFECT_INFO*)g_EffTbl;
            param.fCollRadius = 0.25f;
            param.vRot.init();
            CBullet* pBullet = (CBullet*)CEntityFactory::getInstance()->createBullet(param, this);
            if (pBullet) {
                param.vPos.y += 4.0f;
                pBullet->warp(param.vPos);
                pBullet->setCollAttackPos(param.vPos);
                ((FVECTOR&)pBullet->getRot()).x = 1.5707964f;
            }
        }
        if (getTimer() > 0) break;
        changeMotionFileName(Wizard::RandomThundra3Motion, 0, FALSE, -1, 0, 0);
        m_iStep++;
        break;
    case 2:
        if (isMotionEnd()) {
            endCardBattle(NULL, 6000);
        }
        break;
    }
}

void CEnemyWizard::endStateAttack3() {
    FVECTOR vColor;
    vColor.set(1.0f, 1.0f, 1.0f);
    CSceneEnv::setColor((ObjType::eObjBit)0x1800, 30.0f, vColor, NULL, 0);
}

#line 289
void CEnemyWizard::onAbsorb() {
    CCollBody* hit = m_pBodyColl->getHitCollBody();
    ASSERT(hit);
    switch (hit->getDamageAttr()) {
    case ATK_ATTR_FIRE:
        setEffect("_ex_2150", 4, hit->getDetectPos(), NULL, 0.0f);
        break;
    case ATK_ATTR_ICE:
        setEffect("_ex_2150", 5, hit->getDetectPos(), NULL, 0.0f);
        break;
    case ATK_ATTR_THUNDER:
        setEffect("_ex_2150", 6, hit->getDetectPos(), NULL, 0.0f);
        break;
    }
}

// SOURCE DATA: .data .rodata
