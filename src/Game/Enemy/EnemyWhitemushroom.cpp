
#define GPRIM_H_PATH "System/Graphic/GPrim.h" /* MATCH: this unit names Gprim.h so (header default "System/Graphic/Gprim.h") */
#define BATTLEENEMY_H_PATH "c:/KHCOM/MainApp/Game/Enemy/BattleEnemy.h" /* MATCH: this unit names BattleEnemy.h so (header default "Game/Enemy/BattleEnemy.h") */
#include "System/Graphic/Gprim.h"
#include "System/Graphic/2d/2DMan.h"
#include "Game/CardBattle.h"
#include "Game/Enemy/EnemyWhitemushroom.h"
#include "Game/Collision/CollUtil.h"
#include "Game/BodyCollision.h"
#include "System/Graphic/Anim.h"
#include "System/Graphic/Str.h"
#include "Game/Collision/CollAttack.h"
#include "System/Graphic/Effect/effect.h"
#include "Game/Enemy/EnemyParam.h"
#include "Game/EntityFactory.h"
#include "Game/Movement.h"
#include "System/Math3D.h"
#include "System/Rnd.h"
#include "Game/Collision/CollBody.h"
#define ASSERT(e) if (!(e)) AssertApp(__FILE__, __LINE__, #e, 0)

extern CEntity* g_pPlayer; /* a CPlayer (Game/Player/Player.h) */

#include "Form/Menu/JMemoFlag.h"
#include "System/Com.h"



#include "Game/Player/PlayerGauge.h"

namespace Whitemushroom {
static CHAR* IdlingMotion = "e016_001";
static CHAR* MoveMotion = "e016_002";
static CHAR* GladMotion = "e016_050";
static CHAR* AngryMotion = "e016_051";
static CHAR* DisappearMotion = "e016_074";
static CHAR* Disappear2Motion = "e016_075";
static CHAR* g_pNeedMotTbl[3] = { "e016_052", "e016_054", "e016_056" };
static CHAR* g_pHappyMotTbl[3] = { "e016_053", "e016_055", "e016_057" };
CHAR* EffectName = "_ex_2350";
static const ATK_ATTR g_pHappyAttr[3] = { ATK_ATTR_FIRE, ATK_ATTR_ICE, ATK_ATTR_THUNDER };
}

CEnemyWhitemushroom::CEnemyWhitemushroom(CTask* inParent) : CHeartless(inParent, 0, 0) {
    m_iHappyCount = 0;
}

CEnemyWhitemushroom::~CEnemyWhitemushroom() {}

void CEnemyWhitemushroom::beginStateMain() {
    changeMotionFileName(Whitemushroom::IdlingMotion, 0, FALSE, -1, 0, 0);
    m_pMove->enableGravity(TRUE);
    m_iStep = 0;
}

void CEnemyWhitemushroom::stateMain() {
    switch (m_iStep) {
    case 0:
        m_pMove->homingY(g_pPlayer, 0.1f);
        if (RND::genrand_int31() % 60 == 0) {
            changeMotionFileName(Whitemushroom::g_pNeedMotTbl[RND::genrand_int31() % 3], 0, FALSE, -1, 0, 0);
            m_iStep = 1;
            m_iCounter = 2;
        }
        break;
    case 1:
        if (isMotionEnd()) {
            if (--m_iCounter <= 0) {
                changeMotionFileName(Whitemushroom::IdlingMotion, 0, FALSE, -1, 0, 0);
                m_iStep = 0;
            }
        }
        break;
    }
}

void CEnemyWhitemushroom::endStateMain() {}

#line 112
void CEnemyWhitemushroom::onInvalid() {
    CCollBody* hit = m_pBodyColl->getHitCollBody();
    UINT i;
    ASSERT(hit);
    if (!isNoDmgResponse()) {
        if (hit->getDamageKind() == 14) return;
        changeBattleEnemyState(11);
    }
    for (i = 0; i < 3; i++) {
        if (m_pAnim->getAnimID() == getMotionIndex(Whitemushroom::g_pNeedMotTbl[i]) &&
            hit->getDamageAttr() == Whitemushroom::g_pHappyAttr[i]) {
            return;
        }
    }
    setHp(getHp() - m_pBodyColl->getDamage());
    Gauge::CEnemyGauge::notifyHP(getHandle(), getHp(), getHpMax());
}

void CEnemyWhitemushroom::beginStateSmallDamage() {
    beginDamage_();
}

void CEnemyWhitemushroom::stateSmallDamage() {
    damage_();
}

void CEnemyWhitemushroom::endStateSmallDamage() {
    endDamage_();
}

void CEnemyWhitemushroom::beginStateBigDamage() {
    beginDamage_();
}

void CEnemyWhitemushroom::stateBigDamage() {
    damage_();
}

void CEnemyWhitemushroom::endStateBigDamage() {
    endDamage_();
}

void CEnemyWhitemushroom::beginStateDead() {
    setEntityFlag(0, 0x10000);
    changeMotionFileName(Whitemushroom::AngryMotion, 0, FALSE, -1, 0, 0);
    bodyCollOff(-1);
    m_iStep = 0;
}

void CEnemyWhitemushroom::stateDead() {
    switch (m_iStep) {
    case 0:
        if (isMotionEnd()) {
            changeMotionFileName(Whitemushroom::DisappearMotion, 0, FALSE, -1, 0, 0);
            setupDisappear_();
            m_iStep = 1;
        }
        break;
    case 1:
        if (!isFade()) {
            out();
        }
        break;
    }
}

#line 202
void CEnemyWhitemushroom::beginDamage_() {
    CCollBody* hit;
    UINT i;
    bodyCollOff(-1);
    playSmallDamageSE();
    hit = m_pBodyColl->getHitCollBody();
    ASSERT(hit);
    for (i = 0; i < 3; i++) {
        if (m_pAnim->getAnimID() == getMotionIndex(Whitemushroom::g_pNeedMotTbl[i]) &&
            hit->getDamageAttr() == Whitemushroom::g_pHappyAttr[i]) {
            changeMotionFileName(Whitemushroom::g_pHappyMotTbl[i], 0, FALSE, -1, 0, 0);
            setEffect(Whitemushroom::EffectName, 4, hit->getDetectPos(), NULL, 0.0f);
            if (++m_iHappyCount >= 3) {
                m_iStep = 2;
                bodyCollOff(-1);
                setEntityFlag(0, 0x10000);
            } else {
                m_iStep = 1;
            }
            return;
        }
    }
    setEntityFlag(0, 0x10000);
    changeMotionFileName(Whitemushroom::AngryMotion, 0, FALSE, -1, 0, 0);
    m_iStep = 0;
}

/* Shape from the original's .mdebug block tree: case 4 and 5 are if-blocks with pos and the static local
 * inside, i is the for-scope local, the loop calls getRandF/DEG2RAD. Case 0 repeats the setupDisappear_ tail
 * (jump2 cross-jumps it into case 4's copy after sched2): in one block with the makePrize call, the li 5 for
 * m_iStep gives the vptr load (v0) one more dependent, so sched2 issues it before the setupFade stores. */
void CEnemyWhitemushroom::damage_() {
    switch (m_iStep) {
    case 0:
        if (isMotionEnd()) {
            changeMotionFileName(Whitemushroom::DisappearMotion, 0, FALSE, -1, 0, 0);
            setupDisappear_();
            m_iStep = 5;
        }
        break;
    case 1:
        if (isMotionEnd()) {
            changeDefaultState();
        }
        break;
    case 2:
        if (isMotionEnd()) {
            changeMotionFileName(Whitemushroom::Disappear2Motion, 0, FALSE, -1, 0, 0);
            m_iStep = 3;
        }
        break;
    case 3:
        if (getMotionCurrentTime() >= 4500) {
            PRIZE_PARAM param;
            param.uiFlag = 0;
            param.uiPrizeFlag = 0;
            getLockOnPos(param.vPos);
            param.iNumber = 0;
            param.fSpeed = 0.2f;
            param.PrizeKind = 0xD3;
            for (INT i = 0; i < 10; i++) {
                param.vRot.set(DEG2RAD(-45.0f - getRandF() * 15.0f), DEG2RAD(getRandF() * 360.0f), 0.0f);
                Math3D::LimitPIVector(param.vRot);
                CEntityFactory::getInstance()->createPrize(param, NULL);
            }
            m_iStep = 4;
        }
    case 4:
        if (isMotionEnd()) {
            FVECTOR pos;
            setupFade(1.0f, 0.0f, getMotionTotalTime(), -1);
            getLockOnPos(pos);
            makePrize(&pos);
            setupDisappear_();
            m_iStep = 5;
        }
        break;
    case 5:
        if (!isFade()) {
            static CHAR* MemoIDParamName = "MemoID";
            CJiminyMemoFlag::checkEvent(3, (INT)getExtraParam(MemoIDParamName), 1);
            out();
        }
        break;
    }
}

void CEnemyWhitemushroom::endDamage_() {
    bodyCollOn(-1);
}

void CEnemyWhitemushroom::setupDisappear_() {
    FVECTOR pos;
    getLockOnPos(pos);
    CEffect* effect = CEffectMan::getEffect(setEffect(Whitemushroom::EffectName, 3, &pos, NULL, 0.0f));
    if (effect) {
        setupFade(1.0f, 0.0f, effect->getTotalFrame() * 25, -1);
    }
}

// SOURCE DATA: .data .rodata
