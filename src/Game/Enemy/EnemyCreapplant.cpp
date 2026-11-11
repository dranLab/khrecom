
#define GPRIM_H_PATH "System/Graphic/GPrim.h" /* MATCH: this unit names Gprim.h so (header default "System/Graphic/Gprim.h") */
#define BATTLEENEMY_H_PATH "c:/KHCOM/MainApp/Game/Enemy/BattleEnemy.h" /* MATCH: this unit names BattleEnemy.h so (header default "Game/Enemy/BattleEnemy.h") */
#include "System/Graphic/Gprim.h"
#include "System/Graphic/2d/2DMan.h"
#include "Game/CardBattle.h"
#include "Game/Enemy/EnemyCreapplant.h"
#include "System/Graphic/Anim.h"
#include "System/Graphic/Str.h"
#include "Game/Collision/CollUtil.h"
#include "Game/Collision/CollAttack.h"
#include "System/Graphic/Effect/effect.h"
#include "Game/BodyCollision.h"
#include "Game/Bullet/Bullet.h"
#include "Game/EntityFactory.h"
#include "Game/Movement.h"
#include "System/Graphic/Camera.h"
#include "System/Math3D.h"
#include "Game/Collision/CollBody.h"
#include "System/HeapMan.h"
#define ASSERT(e) KH_DEBUG(if (!(e)) AssertApp(__FILE__, __LINE__, #e, 0))



extern CEntity* g_pPlayer; /* a CPlayer (Game/Player/Player.h) */
FVECTOR getPlayerPos(BOOL flag);
void ApplyMatrix(FVECTOR& v0, const FMATRIX& m0, const FVECTOR& v1);

class CCollAttack;

class CRootNeedle : public CEntity {
private:
    INT m_iStep;            /* 0x1d0 */
    HANDLE m_hNeedle;       /* 0x1d4 */
    HANDLE m_hBase;         /* 0x1d8 */
    BOOL m_bHit;            /* 0x1dc */
    CCollAttack* m_pAttack; /* 0x1e0 */

public:
    CRootNeedle(CTask* inParent);
    virtual ~CRootNeedle();
    virtual INT getTypeID() const { return 0x1009; }
    BOOL setup(const FVECTOR& inPos, INT inAttack, ATK_KIND inAtkKind, ATK_ATTR inAtkAttr);
    BOOL isHit() const { return m_bHit; }
    virtual void beginStateInitial();
    virtual void stateInitial();
    virtual void endStateInitial();
protected:
    virtual void lastUpdate();
};

namespace Creapplant {
static CHAR* IdlingMotion = "e031_001";
static CHAR* Attack1Motion = "e031_050";
static CHAR* Attack2Motion = "e031_052";
}

CRootNeedle::CRootNeedle(CTask* inParent) : CEntity(inParent, (ObjType::eObjType)5) {
    setEntityFlag(0x20000, 0);
    m_iStep = 0;
    m_hNeedle = 0;
    m_bHit = FALSE;
    m_pAttack = NULL;
}

CRootNeedle::~CRootNeedle() {
    if (m_pAttack) delete m_pAttack;
    m_pAttack = NULL;
    CEffect* effect = CEffectMan::getEffect(m_hNeedle);
    if (effect) effect->kill();
}

#line 88
BOOL CRootNeedle::setup(const FVECTOR& inPos, INT inAttack, ATK_KIND inAtkKind, ATK_ATTR inAtkAttr) {
    FVECTOR pos;
    m_pMove = new KH_AT("(91)") CMovement(this);
    if (!m_pMove) return FALSE;
    addCoord(1);
    m_pAttack = new KH_AT("(97)") CCollAttack;
    if (!m_pAttack) return FALSE;
    warp(inPos);
    setStartPos(inPos);
    updateCoord();
    coordFix();
    pos = inPos;
    m_hNeedle = setEffect("m_ex750", 8, &pos, NULL, 0.0f);
    setEffect("m_ex750", 0x11, &pos, NULL, 0.0f);
    m_pAttack->init((COLL_KIND)0x80000002, (COLL_TYPE)8, getHandle(), 0);
    m_pAttack->setAttackKind(inAtkKind);
    m_pAttack->setAttackAttr(inAtkAttr);
    m_pAttack->setAttack(inAttack, 0);
    m_pAttack->setRadius(0.2f);
    m_pAttack->setEnemyPos(pos);
    m_pAttack->setStartPos(pos);
    pos.y += 1.5f;
    m_pAttack->setEndPos(pos);
    m_pAttack->setHitEffectParam("m_ex750", 9, (ObjType::eObjType)5);
    m_pAttack->updateAttackID();
    m_pAttack->off();
    return TRUE;
}

void CRootNeedle::beginStateInitial() {}

void CRootNeedle::stateInitial() {
    CEffect* effect = CEffectMan::getEffect(m_hNeedle);
    if (!effect) out();
    switch (m_iStep) {
    case 0:
        if (effect->getNowFrame() >= 30.0f) {
            m_pAttack->on();
            m_iStep++;
        }
        break;
    case 1:
        if (effect->getNowFrame() >= 40.0f) {
            m_pAttack->off();
            m_iStep++;
        }
        break;
    }
}

void CRootNeedle::endStateInitial() {}

void CRootNeedle::lastUpdate() {
    if (m_pAttack->collideAttack()) {
        m_bHit = TRUE;
    }
}

CEnemyCreapplant::CEnemyCreapplant(CTask* inParent) : CHeartless(inParent, 0, 0) {
    setBattleEnemyFlag(0x8200108000ULL, 0);
}

CEnemyCreapplant::~CEnemyCreapplant() {}

void CEnemyCreapplant::beginStateMain() {
    changeMotionFileName(Creapplant::IdlingMotion, 0, FALSE, -1, 0, 0);
}

void CEnemyCreapplant::stateMain() {
    m_pMove->turnToTargetY(g_pPlayer, 0.034906585f);
}

void CEnemyCreapplant::endStateMain() {}

void CEnemyCreapplant::beginStateAttack1() {
    changeMotionFileName(Creapplant::Attack1Motion, 0, FALSE, -1, 0, 0);
    m_iStep = 0;
}

void CEnemyCreapplant::stateAttack1() {
    static const BULLET_EFFECT_INFO g_EffTbl[4] = {
        {NULL, 0, -1, 0}, {"m_ex750", 6, -1, 0}, {"m_ex750", 7, -1, 0}, {"m_ex750", 5, -1, 0}};
    static const FVECTOR g_vOffset = {{0.0f, 0.5f, 1.31f, 1.0f}};
    if (m_iStep == 0) {
        turnToPlayerY(0.06981317f);
        if (getMotionCurrentTime() >= 2750) {
            BULLET_PARAM param;
            param.init(1, 3);
            param.AtkAttr = getAttackAttr(0);
            param.iAttackPoint = (INT)getAttackPoint(0, (ATK_KIND)1, 0, NULL, 0);
            param.fSpeed = 0.3f;
            param.pEffectTbl = (BULLET_EFFECT_INFO*)g_EffTbl;
            param.fCollRadius = 0.2f;
            param.fMaxDist = 900.0f;
            ApplyMatrix(param.vPos, Crd(0).getWorldMatrix(), g_vOffset);
            param.vRot.set(0.0f, getRot().y, 0.0f);
            CEntityFactory::getInstance()->createBullet(param, this);
            param.vRot.set(0.0f, Math3D::LimitPI(getRot().y + 0.34906584f), 0.0f);
            CEntityFactory::getInstance()->createBullet(param, this);
            param.vRot.set(0.0f, Math3D::LimitPI(getRot().y - 0.34906584f), 0.0f);
            CEntityFactory::getInstance()->createBullet(param, this);
            m_iStep++;
        }
    }
    if (isMotionEnd()) {
        endCardBattle(NULL, 6000);
    }
}

void CEnemyCreapplant::endStateAttack1() {}

void CEnemyCreapplant::beginStateAttack2() {
    changeMotionFileName(Creapplant::Attack2Motion, 0, FALSE, -1, 0, 0);
    m_iStep = 0;
}

#line 275
void CEnemyCreapplant::stateAttack2() {
    if (m_iStep == 0) {
        turnToPlayerY(0.06981317f);
        if (getMotionCurrentTime() >= 5000) {
            CRootNeedle* pNeedle;
            pNeedle = new KH_AT("(284)") CRootNeedle(this);
            if (pNeedle) {
                FVECTOR pos = getPlayerPos(FALSE);
                pos.y = getPos().y;
                INT iAttack = (INT)getAttackPoint(1, (ATK_KIND)1, 0, NULL, 0);
                pNeedle->setup(pos, iAttack, (ATK_KIND)24, getAttackAttr(1));
            }
            m_iStep++;
        }
    }
    if (isMotionEnd()) {
        endCardBattle(NULL, 6000);
    }
}

void CEnemyCreapplant::endStateAttack2() {}

#line 308
void CEnemyCreapplant::beginStateTerror() {
    CCollBody* hitBody = m_pBodyColl->getHitCollBody();
    ASSERT(hitBody);
    setTimer(18000);
    setBadStatusEffect(3, hitBody->getHitEffectWait());
    changeMotionFileName(Creapplant::IdlingMotion, 0, FALSE, -1, 0, 0);
}

void CEnemyCreapplant::stateTerror() {
    if (getTimer() <= 0) {
        changeDefaultState();
    }
}

void CEnemyCreapplant::endStateTerror() {
    killBadStatusEffect();
}

void CEnemyCreapplant::onCardBreak() {
    for (SLContainerT<CTask>* p = getChildren().getHead(); p; p = p->getNext()) {
        CTask* pTask = p->getValue();
        if (pTask && pTask->getTypeID() == 0x1009) {
            pTask->out();
        }
    }
}

// SOURCE DATA: .data .rodata
// SOURCE DATA SLUS_217.99_retail: .data .rodata
