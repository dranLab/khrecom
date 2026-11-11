#define GPRIM_H_PATH "System/Graphic/GPrim.h" /* MATCH: this unit names Gprim.h so (header default "System/Graphic/Gprim.h") */
#define BATTLEENEMY_H_PATH "c:/KHCOM/MainApp/Game/Enemy/BattleEnemy.h" /* MATCH: this unit names BattleEnemy.h so (header default "Game/Enemy/BattleEnemy.h") */
#include "System/Task.h"
#include "System/TaskMan.h"
#include "System/Graphic/Gprim.h"
#include "System/Graphic/2d/2DMan.h"
#include "Game/CardBattle.h"
#include "Game/Enemy/Heartless.h"
#include "System/Graphic/Anim.h"
#include "System/Graphic/Str.h"
#include "Game/Collision/CollUtil.h"

/* MATCH: Placeholder: .rodata has empty string literals after CollUtil.h's strings (from an unidentified header). */
inline void _heartlessEmptyString() {
    TRACE("");
}

#include "Game/Enemy/EnemyParam.h"

/* MATCH: Placeholders for an unknown header's inline functions (prize messages) that come next in .rodata. */
inline void _premiumPrizeMessage() {
    TRACE("\x83v\x83\x8c\x83~\x83" "A\x83\x80\x83v\x83\x89\x83" "C\x83Y\x93\xfc\x8e\xe8\x81I\x81I\n");
}
inline void _roulettePrizeMessage() { /* MATCH: placeholder: an inline function's strings, in the original .rodata order */
    TRACE("\x83\x8b\x81[\x83\x8c\x83" "b\x83g\x83v\x83\x89\x83" "C\x83Y\x93\xfc\x8e\xe8\x81I\x81I\n");
}

#include "Game/Movement.h"
#include "Game/Collision/CollMap.h"
#include "Game/Debug/DebugWork.h"
#include "System/Math3D.h"
#include "System/Rnd.h"
#include <math.h>
extern "C" float atan2f(float y, float x);

extern CEntity* g_pPlayer; /* a CPlayer (Game/Player/Player.h) */
FVECTOR getPlayerPos(BOOL flag);
void AddVectorXYZ(FVECTOR& v0, const FVECTOR& v1, const FVECTOR& v2);
void SubVector(FVECTOR& v0, const FVECTOR& v1, const FVECTOR& v2);
void Normalize(FVECTOR& v0, const FVECTOR& v1);
void ScaleVector(FVECTOR& v0, const FVECTOR& v1, FLOAT f);

#include "Game/Enemy/BattleEnemyManager.h"




template <class T> static inline T& _ref(T& v) { return v; }

#define ASSERT(e) KH_DEBUG(if (!(e)) AssertApp(__FILE__, __LINE__, #e, 0))
#define DEG2RAD 0.017453292f

namespace Heartless {
static CHAR* SpeedParamName = "Speed";
static CHAR* WalkRateParamName = "WalkRate";
}

CHeartless::CHeartless(CTask* inParent, CState* inDefaultState, CState* inMainState)
    : CBattleEnemy(inParent, inDefaultState, inMainState) {
    m_iWalkStartMotIndex = -1;
    m_iWalkEndMotIndex = -1;
    m_fHomingDeg = 5.0f;
    m_iStep = 0;
    m_uiHeartlessFlag = 0;
}

CHeartless::~CHeartless() {}

BOOL CHeartless::initalize(INT inGroupID, INT inRsrcID, UINT iFlag) {
    if (!CBattleEnemy::initalize(inGroupID, inRsrcID, iFlag)) return FALSE;
    if (!isFly()) m_pMove->enableGravity(TRUE);
    return TRUE;
}

BOOL CHeartless::startCardBattle() {
    setBattleEnemyFlag(0, 0x802);
    CardBattle::CCardBattle* cb = (CardBattle::CCardBattle*)CTaskMan::getSysTask(22);
#line 115
    ASSERT(cb);
    INT num = randomBattleCardNumber();
    if (!isAlwaysSetCard() && num < CardBattle::CCardBattle::getPlayingCardPoint(1) &&
        RND::genrand_int31() % 20) {
        setBattleEnemyFlag(2, 0);
        setBattleEnemyFlag(0, 0x200);
        return FALSE;
    }
    if (!cb->setEnemyCard(getHandle(), *(INT*)((BYTE*)this + KH_VER(0x240, 0x220)), num)) {
        setBattleEnemyFlag(2, 0);
        setBattleEnemyFlag(0, 0x200);
        onCardBattleFailed();
        return FALSE;
    }
    setBattleEnemyFlag(1, 0);
    return TRUE;
}

void CHeartless::aiCardBattle() {
    if (!getEnemyParam()) return;
    if (RND::genrand_int31() % getAttackRate()) return;
    INT act[8];
    INT count = 0;
    ENEMY_PARAM* param;
    for (INT i = 0; i < (param = getEnemyParam())->iActCount; i++) {
        ENEMY_ACT* pAct = (ENEMY_ACT*)((UINT)i * sizeof(ENEMY_ACT) + (UINT)param->pActs);
        INT state = pAct->iActNo + 2;
        if (2 <= state && state < 10 && canAttack_(pAct)) {
#line 181
            ASSERT(count < 8);
            act[count++] = state;
        }
    }
    if (count == 0) return;
    if (!canCardBattle()) return;
    if (KH_DW(DebugWork::g_dw.enemy.uiFlag) & 8) {
        for (INT i = 0; i < getEnemyParam()->iActCount; i++) {
            if (getEnemyParam()->pActs[i].iActNo == KH_DW(DebugWork::g_dw.enemy.sActNo)) {
                setBattleEnemyFlag(0x200, 0);
                m_iAttackState = KH_DW(DebugWork::g_dw.enemy.sActNo) + 2;
                return;
            }
        }
        return;
    }
    setBattleEnemyFlag(0x200, 0);
    setAttackState(act[RND::genrand_int31() % count]);
}

INT CHeartless::getDuelAttackState() {
    INT r = RND::genrand_int31();
    return getEnemyParam()->pActs[r % getEnemyParam()->iActCount].iActNo + 2;
}

void CHeartless::commonMoveInit(const CHAR* inIdling, const CHAR* inWalk, FLOAT inRand) {
    m_iStep = 0;
    m_iIdlingMotIndex = getMotionIndex(inIdling);
    m_iWalkMotIndex = getMotionIndex(inWalk);
    m_fSpeed = getExtraParam(_ref(Heartless::SpeedParamName));
    m_iWaitRate = (INT)getExtraParam(_ref(Heartless::WalkRateParamName));
    m_fRandMax = inRand;
    changeHeartlessMotion_(m_iIdlingMotIndex);
    m_pMove->enableGravity(TRUE);
}

void CHeartless::commonMove() {
    switch (m_iStep) {
    case 0:
        if (canRotMove()) {
            changeRotMoveState_();
            setBattleEnemyFlag(0x80000000, 0);
            m_fRot = 1.0f;
            break;
        }
        if (RND::genrand_int31() % m_iWaitRate) break;
        changeWalkState_();
        randAimPos(m_vAimPos, m_fRandMax);
        m_iCount = 0;
        break;
    case 1:
        if (isMotionEnd()) {
            changeHeartlessMotion_(m_iWalkMotIndex);
            setTimer(RND::genrand_int31() % 3 * 3000 + 9000);
            m_iStep = 2;
        }
        break;
    case 2:
        if (canRotMove()) {
            changeHeartlessMotion_(m_iWalkMotIndex);
            m_iStep = 5;
            m_fRot = 1.0f;
            setBattleEnemyFlag(0x80000000, 0);
            break;
        }
        if (m_pMove->getCollisionFlag() & 0xA) {
            if (m_iCount <= 0) {
                FLOAT rot = Math3D::LimitPI(m_pMove->getRot().y + 3.1415927f);
                FLOAT len = m_fRandMax * 2.0f + rndArea(-m_fRandMax, m_fRandMax);
                m_vAimPos.x = len * sinf(rot);
                m_vAimPos.y = 0.0f;
                m_vAimPos.z = len * cosf(rot);
                m_iCount = 60;
            } else {
                m_iCount--;
            }
        } else if (m_iCount > 0) {
            m_iCount--;
        }
        if (!(RND::genrand_int31() % 50)) randAimPos(m_vAimPos, m_fRandMax);
        if (getTimer() > 0) {
            FVECTOR pos = getPlayerPos(FALSE);
            CCollMap* coll = m_pMove->getMapCollision();
#line 317
            ASSERT(coll);
            coll->setStartPos(pos);
            AddVectorXYZ(pos, pos, m_vAimPos);
            coll->setEndPos(pos);
            if (coll->collideMap()) pos = *coll->getOutPos();
            if (arriveTargetXZ(pos, (m_fSpeed + 0.1f) * getFrameRateF())) {
                if (RND::genrand_int31() & 1) {
                    setTimer(RND::genrand_int31() % 3 * 3000 + 9000);
                    randAimPos(m_vAimPos, m_fRandMax);
                    break;
                }
                changeIdlingState_();
                break;
            }
            m_pMove->turnToTargetY(pos, m_fHomingDeg * DEG2RAD);
            m_pMove->calcVelocityXZ(m_fSpeed);
            break;
        }
        changeIdlingState_();
        break;
    case 3:
        if (isMotionEnd()) {
            changeHeartlessMotion_(m_iIdlingMotIndex);
            m_iStep = 0;
        }
        break;
    case 4:
        if (isMotionEnd()) {
            changeHeartlessMotion_(m_iWalkMotIndex);
            m_iStep = 5;
        }
        break;
    case 5:
        if ((m_pMove->getCollisionFlag() & 0xA) || !(RND::genrand_int31() % 100)) {
            clearSpeed();
            if (m_iWalkEndMotIndex == -1) {
                changeHeartlessMotion_(m_iIdlingMotIndex);
                setTimer(6000);
                m_iStep = 7;
            } else {
                changeHeartlessMotion_(m_iWalkEndMotIndex);
                m_iStep = 7;
            }
        } else {
            FVECTOR vel;
            computeRotMoveVelocity(vel, m_fRot);
            ((FVECTOR&)getRot()).y = atan2f(vel.x, vel.z);
            FVECTOR v = m_pMove->getVel();
            v.x = vel.x;
            v.z = vel.z;
            m_pMove->setVel(v);
            if (calcDistanceXZSq(_ref(g_pPlayer)) > 16.0f) {
                setBattleEnemyFlag(0, 0x80000000);
                changeIdlingState_();
            }
        }
        break;
    case 6:
        if (isMotionEnd()) {
            changeHeartlessMotion_(m_iIdlingMotIndex);
            setTimer(6000);
            m_iStep = 7;
        }
        break;
    case 7:
        m_pMove->homingY(g_pPlayer, 0.1f);
        if (getTimer() <= 0) {
            changeRotMoveState_();
            m_fRot = -m_fRot;
        }
        break;
    case 8:
        m_pMove->evade(g_pPlayer, 0.1f, m_fSpeed);
        if (calcDistanceXZSq(g_pPlayer) > 25.0f) changeIdlingState_();
        break;
    }
}

void CHeartless::commonMoveEnd() {
    setBattleEnemyFlag(0, 0x80000000);
}

void CHeartless::commonFlyInit(const CHAR* inIdling, const CHAR* inWalk, FLOAT inRandMin, FLOAT inRandMax) {
    m_iStep = 0;
    m_iIdlingMotIndex = getMotionIndex(inIdling);
    m_iWalkMotIndex = getMotionIndex(inWalk);
    m_fSpeed = getExtraParam(_ref(Heartless::SpeedParamName));
    m_iWaitRate = (INT)getExtraParam(_ref(Heartless::WalkRateParamName));
    m_fRandMin = inRandMin;
    m_fRandMax = inRandMax;
    changeHeartlessMotion_(m_iIdlingMotIndex);
}

void CHeartless::commonFly() {
    switch (m_iStep) {
    case 0:
        if (canRotMove() && getHeight() < 3.5f) {
            changeRotMoveState_();
            setBattleEnemyFlag(0x80000000, 0);
            m_fRot = 1.0f;
            break;
        }
        if (RND::genrand_int31() % m_iWaitRate) break;
        changeWalkState_();
        randAimPosForFly();
        break;
    case 1:
        if (isMotionEnd()) {
            changeHeartlessMotion_(m_iWalkMotIndex);
            setTimer(RND::genrand_int31() % 3 * 3000 + 9000);
            m_iStep = 2;
        }
        break;
    case 2:
        if (canRotMove()) {
            changeHeartlessMotion_(m_iWalkMotIndex);
            m_iStep = 5;
            m_fRot = 1.0f;
            setBattleEnemyFlag(0x80000000, 0);
            break;
        }
        if (getTimer() <= 0 || arriveTarget(m_vAimPos, (m_fSpeed + 0.1f) * getFrameRateF())) {
            if (RND::genrand_int31() & 1) {
                setTimer(RND::genrand_int31() % 3 * 3000 + 9000);
                randAimPosForFly();
                break;
            }
            changeIdlingState_();
            break;
        }
        m_pMove->turnToTarget(m_vAimPos, m_fHomingDeg * DEG2RAD);
        m_pMove->calcVelocity(m_fSpeed);
        break;
    case 3:
        if (isMotionEnd()) {
            changeHeartlessMotion_(m_iIdlingMotIndex);
            m_iStep = 0;
        }
        break;
    case 4:
        if (isMotionEnd()) {
            changeHeartlessMotion_(m_iWalkMotIndex);
            m_iStep = 5;
        }
        break;
    case 5:
        if (m_pMove->getCollisionFlag() & 0xA) {
            m_fRot = -m_fRot;
        } else if (!(RND::genrand_int31() % 100)) {
            changeHeartlessMotion_(m_iIdlingMotIndex);
            clearSpeed();
            setTimer(6000);
            m_iStep = 7;
            break;
        }
        {
            FVECTOR vel;
            computeRotMoveVelocity(vel, m_fRot);
            ((FVECTOR&)getRot()).y = atan2f(vel.x, vel.z);
            FVECTOR v = m_pMove->getVel();
            v.x = vel.x;
            v.z = vel.z;
            v.y = 0.0f;
            m_pMove->setVel(v);
            if (calcDistanceXZSq(_ref(g_pPlayer)) > 16.0f) {
                setBattleEnemyFlag(0, 0x80000000);
                changeIdlingState_();
            }
        }
        break;
    case 6:
        if (isMotionEnd()) {
            changeHeartlessMotion_(m_iIdlingMotIndex);
            setTimer(6000);
            m_iStep = 7;
        }
        break;
    case 7:
        m_pMove->homingY(g_pPlayer, 0.1f);
        if (getTimer() <= 0) {
            changeRotMoveState_();
            m_fRot = -m_fRot;
        }
        break;
    case 8:
        m_pMove->evade(g_pPlayer, 0.1f, m_fSpeed);
        if (calcDistanceXZSq(g_pPlayer) > 25.0f) changeIdlingState_();
        break;
    }
}

void CHeartless::commonFlyEnd() {
    setBattleEnemyFlag(0, 0x80000000);
}

void CHeartless::randAimPos(FVECTOR& outPos, FLOAT inRange) {
    FLOAT rot = Math3D::LimitPI(RND::genrand_real1() * 360.0f * DEG2RAD);
    FLOAT len = inRange * 2.0f + rndArea(-inRange, inRange);
    outPos.x = len * sinf(rot);
    outPos.y = 0.0f;
    outPos.z = len * cosf(rot);
}

void CHeartless::randAimPosForFly() {
    FLOAT rot = Math3D::LimitPI(RND::genrand_real1() * 360.0f * DEG2RAD);
    FLOAT len = rndArea(1.0f, 4.0f);
    m_vAimPos = getPlayerPos(FALSE);
    m_vAimPos.x += len * sinf(rot);
    m_vAimPos.z += len * cosf(rot);
    m_vAimPos.y += rndArea(m_fRandMin, m_fRandMax);
    CCollMap* coll = m_pMove->getMapCollision();
#line 625
    ASSERT(coll);
    FVECTOR pos = getPlayerPos(FALSE);
    pos.y = m_vAimPos.y;
    coll->setStartPos(pos);
    coll->setEndPos(m_vAimPos);
    if (coll->collideMap()) m_vAimPos = *coll->getOutPos();
}

void CHeartless::computeRotMoveVelocity(FVECTOR& outVel, FLOAT inRot) {
    if (!g_pPlayer) {
        outVel.init();
        return;
    }
    FLOAT dir = g_pPlayer->calcDirection(this);
    FLOAT newDir = Math3D::LimitPI(dir + inRot * DEG2RAD);
    FLOAT dist = calcDistanceXZ(g_pPlayer);
    FVECTOR v1;
    FVECTOR v2;
    v1.set(dist * sinf(dir), 0.0f, dist * cosf(dir), 1.0f);
    v2.set(dist * sinf(newDir), 0.0f, dist * cosf(newDir), 1.0f);
    SubVector(v2, v2, v1);
    Normalize(v2, v2);
    ScaleVector(outVel, v2, m_fSpeed);
}

BOOL CHeartless::checkBattleEnemyFlag(UINT inFlag) {
    CBattleEnemyManager* bem = (CBattleEnemyManager*)CTaskMan::getSysTask(25);
#line 663
    ASSERT(bem);
    return bem->checkBattleEnemyFlag(inFlag);
}

BOOL CHeartless::canRotMove() {
    if (!isEnableRotMove()) return FALSE;
    if (checkBattleEnemyFlag(0x80000000)) return FALSE;
    if (calcDistanceXZSq(g_pPlayer) > 16.0f) return FALSE;
    return TRUE;
}

BOOL CHeartless::canEvade() {
    INT n;
    if (isEnableEvade()) {
        n = RND::genrand_int31() % 20;
    } else {
        if (!isEnableEvade2()) return FALSE;
        if ((FLOAT)getHp() / (FLOAT)getHpMax() > 0.4f) return FALSE;
        n = RND::genrand_int31() % 33;
    }
    if (n == 0) {
        if (calcDistanceXZSq(g_pPlayer) > 9.0f) return FALSE;
        return TRUE;
    }
    return FALSE;
}

BOOL CHeartless::canAttack() {
    return canAttack_(&getEnemyParam()->pActs[m_iAttackState - 2]);
}

BOOL CHeartless::canAttack(INT inIndex) {
#line 720
    KH_DEBUG(if (!(0 <= inIndex && inIndex < getEnemyParam()->iActCount)) AssertApp(__FILE__, __LINE__, "0 <= inIndex && inIndex < m_pEnemyParam->iActCount", 0);)
    return canAttack_(&getEnemyParam()->pActs[inIndex]);
}

INT CHeartless::randomBattleCardNumber() const {
    return getCardNumber_(m_iAttackState);
}

INT CHeartless::getBattleCardNumber(INT inState) const {
    if (inState < 0) inState = m_iAttackState;
    return getCardNumber_(inState);
}

void CHeartless::changeHeartlessMotion_(INT inIndex) {
    if (getAnim() && getAnim()->getAnimID() != inIndex) changeMotion(inIndex, 0, FALSE, -1, 0, 0);
}

void CHeartless::changeIdlingState_() {
    clearSpeed();
    if (m_iWalkEndMotIndex == -1) {
        changeHeartlessMotion_(m_iIdlingMotIndex);
        m_iStep = 0;
    } else {
        changeHeartlessMotion_(m_iWalkEndMotIndex);
        m_iStep = 3;
    }
}

void CHeartless::changeWalkState_() {
    if (m_iWalkStartMotIndex == -1) {
        changeHeartlessMotion_(m_iWalkMotIndex);
        m_iStep = 2;
        setTimer(RND::genrand_int31() % 3 * 3000 + 9000);
    } else {
        changeHeartlessMotion_(m_iWalkStartMotIndex);
        m_iStep = 1;
    }
}

void CHeartless::changeRotMoveState_() {
    if (m_iWalkStartMotIndex == -1) {
        changeHeartlessMotion_(m_iWalkMotIndex);
        m_iStep = 5;
    } else {
        changeHeartlessMotion_(m_iWalkStartMotIndex);
        m_iStep = 4;
    }
}

static inline ENEMY_ACT* _findAct(ENEMY_PARAM* param, INT inActNo) {
    for (INT i = 0; i < param->iActCount; i++) {
        if (param->pActs[i].iActNo == inActNo) return &param->pActs[i];
    }
    return NULL;
}

#define PARAM (((CHeartless*)this)->getEnemyParam())
INT CHeartless::getCardNumber_(INT inState) const {
    if (KH_DW(DebugWork::g_dw.enemy.uiFlag) & 0x4000) return KH_DW(DebugWork::g_dw.enemy.iCardNumber);
    ENEMY_ACT* act = _findAct(PARAM, inState - 2);
#line 826
    ASSERT(act);
    ENEMY_CARD* card = NULL;
    INT num = 0;
    if (act->iCard[0] < 0) {
#line 834
        KH_DEBUG(if (!(PARAM->iCardCount > 0)) AssertApp(__FILE__, __LINE__, "m_pEnemyParam->iCardCount > 0", 0);)
        card = PARAM->pCards;
    } else {
        for (INT i = 0; i < PARAM->iCardCount; i++) {
            if (PARAM->pCards[i].iCardKind == act->iCard[0]) {
                card = &PARAM->pCards[i];
                break;
            }
        }
    }
    if (!card) {
        card = PARAM->pCards;
        TRACE("warning:\x8ew\x92\xe8\x82\xb3\x82\xea\x82\xbd\x83J\x81[\x83h\x83Z\x83" "b\x83g\x82\xaa\x82\xa0\x82\xe8\x82\xdc\x82\xb9\x82\xf1 ... CardKind = %d, ID = %d\n",
              act->iCard[0], (UINT)m_iID % 10000);
    }
    INT tbl[10];
    for (INT i = 0; i < 10; i++) {
        if (card->bNumber[i]) tbl[num++] = i;
    }
#line 863
    ASSERT(num > 0);
    return tbl[RND::genrand_int31() % num];
}
#undef PARAM
// SOURCE DATA: .data .rodata
// SOURCE DATA SLUS_217.99_retail: .data .rodata
