#define GPRIM_H_PATH "System/Graphic/GPrim.h" // MATCH: the original spelled the include "GPrim.h"
#define COLLOBJ_H_ENUMS_ONLY /* MATCH: the collision classes come later (their vtables follow CHumanBoss's) */
#define BATTLEENEMY_H_PATH "c:/KHCOM/MainApp/Game/Enemy/BattleEnemy.h" // MATCH: the original included it by its full path
#include "System/TaskMan.h"
#include "System/Task.h"
#include "System/Graphic/Gprim.h"
#include "System/Graphic/2d/2DMan.h"
#include "Game/CardBattle.h"
#include "Game/Enemy/BattleEnemy.h"
#include "Game/Enemy/EnemyParam.h"
#include "Game/Enemy/HumanBoss.h"
#include "Game/Collision/CollUtilA.h"
#include "Game/Collision/CollUtil.h"
#include "System/Graphic/Anim.h"
#include "System/Graphic/Str.h"

/* MATCH: placeholder - .rodata has an empty string literal after Str.h's strings (from an unidentified header). */
inline void _humanBossEmptyString() {
    TRACE("");
}

#include "System/Graphic/Effect/effect.h"
#include "Game/BodyCollision.h"

#undef COLLOBJ_H_ENUMS_ONLY
#include "Game/Collision/CollObj.h"
#include "Game/CardKind.h"
#include "Game/Debug/DebugWork.h"
#include "System/Math3D.h"
#include "System/Rnd.h"
#include "include_asm.h"
#include <math.h>
#include "Game/Collision/CollBody.h"
/* Inline in the original's collision header: the weak copies of its destructors and collideMap end this
 * object. */






#include "Game/Enemy/BossDeadDemo.h"
#include "System/HeapMan.h"





#define ASSERT(e) if (!(e)) AssertApp(__FILE__, __LINE__, #e, 0)

void AddVector(FVECTOR& v0, const FVECTOR& v1, const FVECTOR& v2);

extern CEntity* g_pPlayer; /* a CPlayer (Game/Player/Player.h) */
/* CPlayer's HP: m_iHp (0x320), m_iHpMax (0x324) */
static inline INT _playerHp(CEntity* p) { return *(INT*)((UBYTE*)p + 0x320); }
static inline INT _playerHpMax(CEntity* p) { return *(INT*)((UBYTE*)p + 0x324); }

#line 286

CHumanBoss::CARD_DATA HumanBoss::g_CardDataTbl[12] = {
    { 101, { 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01 } },
    { 106, { 0x81, 0x82, 0x81, 0x81, 0x81, 0x81, 0x81, 0x81 } },
    { 107, { 0x83, 0x83, 0x83, 0x83, 0x84, 0x84, 0x84, 0x84 } },
    { 118, { 0x89, 0x8a, 0x89, 0x89, 0x89, 0x89, 0x89, 0x89 } },
    { 114, { 0x92, 0x92, 0x92, 0x92, 0x93, 0x93, 0x93, 0x93 } },
    { 116, { 0x8e, 0x8f, 0x90, 0x91, 0x8e, 0x8e, 0x8e, 0x8e } },
    { 117, { 0x8b, 0x8c, 0x8b, 0x8b, 0x8d, 0x8d, 0x8d, 0x8d } },
    { 112, { 0x85, 0x86, 0x87, 0x85, 0x88, 0x88, 0x88, 0x88 } },
    { 119, { 0x94, 0x95, 0x94, 0x94, 0x96, 0x96, 0x96, 0x96 } },
    { 121, { 0x99, 0x9a, 0x99, 0x99, 0x9b, 0x9b, 0x9b, 0x9b } },
    { 122, { 0x9c, 0x9c, 0x9c, 0x9c, 0x9d, 0x9d, 0x9d, 0x9d } },
    { 124, { 0x12, 0x12, 0x12, 0x12, 0x9e, 0x9e, 0x9e, 0x9e } },
};

INT HumanBoss::GetPlayerHpPercent() {
#line 214
    ASSERT(g_pPlayer);
    return _playerHp(g_pPlayer) * 100 / _playerHpMax(g_pPlayer);
}

CHumanBoss::CHumanBoss(CTask* inParent, CState* inDefaultState, CState* inMainState)
    : CBattleEnemy(inParent, inDefaultState, inMainState) {
    m_iNormalActNum = 0;
    m_pNormalActs = __null;
    m_iStockNum = 0;
    m_pStockData = __null;
    m_iStockCardCount = 0;
    m_pUseStock = __null;
    m_pCardData = __null;
    m_iActCount = 0;
    m_pDoBattleAct = __null;
    m_iNoReloadNum = 15;
    m_iRemainCardNum = 1;
    m_uiHumanBossFlag = 1;
    m_iStockRate = 2;
    m_iUseStockRate = 3;
    m_iTotalCardNum = 0;
    m_iStockTotalNumber = 0;
    m_iStockRollCount = 0;
}

CHumanBoss::~CHumanBoss() {
    if (m_pNormalActs) delete[] m_pNormalActs;
    m_pNormalActs = __null;
    if (m_pStockData) delete[] m_pStockData;
    m_pStockData = __null;
}

BOOL CHumanBoss::initalize(INT inGroupID, INT inRsrcID, UINT iFlag) {
    INT rate;

    if (!CBattleEnemy::initalize(inGroupID, inRsrcID, iFlag)) {
        return FALSE;
    }
    setupCardRevolver();
    setupAct();
    setTexAnmSeq(0, HumanBoss::NormalEyeSeqName);
    setTexAnmSeq(1, HumanBoss::NormalLipSeqName);
    setEntityFlag(0, 0x10000);
    /* MATCH: cast store = no struct-field alias with the global load below (it stays before that load) */
    *(INT*)&m_iDeadSeIdentifyNo = getParam() + 1;
    rate = getExtraParam(HumanBoss::UseItemCardParamName);
    m_iNoReloadNum = getAttackCardNum(3) * rate / 100;
    return TRUE;
}

/* MATCH: m_pEnemyParam read directly (friend in BattleEnemy.h), not through the getEnemyParam() inline (the ref lists no inlined this) */
INT CHumanBoss::getAttackCardNum(UINT inFlag) const {
    INT cardNum;
    INT i;
    ENEMY_CARD* card;
    INT j;
    cardNum = 0;
    for (i = 0; i < m_pEnemyParam->iCardCount; i++) {
        card = &m_pEnemyParam->pCards[i];
        for (j = 0; j < 10; j++) {
            if ((inFlag & 1) && (UINT)card->iCardKind < 4) {
                cardNum += card->bNumber[j];
            }
            if ((inFlag & 2) && (UINT)card->iCardKind - 4 < 4) {
                cardNum += card->bNumber[j];
            }
        }
    }
    return cardNum;
}

INT CHumanBoss::getRemainCard(UINT inFlag) {
    INT num = 0;
    if (inFlag & 1) {
        num = m_pCardRevolver->getRemain(0, 0, 1);
    }
    if (inFlag & 2) {
        num += m_pCardRevolver->getRemain(0, 0, 2);
    }
    return num;
}

INT CHumanBoss::getRemainEnemyCard(INT inKind) {
    return m_pCardRevolver->getRemain(1, inKind, 0);
}

INT CHumanBoss::getHaveCard(UINT inFlag) {
    INT num = 0;
    if (inFlag & 1) {
        num = m_pCardRevolver->getHave(1, 1, 1, 0, 0);
    }
    if (inFlag & 2) {
        num += m_pCardRevolver->getHave(2, 1, 1, 0, 0);
    }
    return num;
}

void CHumanBoss::updateAfter() {
    CBattleEnemy::updateAfter();
    if (!isCurrentState(0)) {
        playMyRemainHpVoice();
        playPlayerRemainHpVoice();
    }
}

void CHumanBoss::beginStateSmallDamage() {
    setTexAnmSeq(0, HumanBoss::DamageEyeSeqName);
    setTexAnmSeq(1, HumanBoss::DamageLipSeqName);
    CBattleEnemy::beginStateSmallDamage();
}

void CHumanBoss::endStateSmallDamage() {
    setTexAnmSeq(0, HumanBoss::NormalEyeSeqName);
    setTexAnmSeq(1, HumanBoss::NormalLipSeqName);
    CBattleEnemy::endStateSmallDamage();
}

void CHumanBoss::beginStateBigDamage() {
    setTexAnmSeq(0, HumanBoss::DamageEyeSeqName);
    setTexAnmSeq(1, HumanBoss::DamageLipSeqName);
    CBattleEnemy::beginStateBigDamage();
}

void CHumanBoss::endStateBigDamage() {
    setTexAnmSeq(0, HumanBoss::NormalEyeSeqName);
    setTexAnmSeq(1, HumanBoss::NormalLipSeqName);
    CBattleEnemy::endStateBigDamage();
}

void CHumanBoss::beginStateFaint() {
    setTexAnmSeq(0, HumanBoss::FaintEyeSeqName);
    setTexAnmSeq(1, HumanBoss::FaintLipSeqName);
    CBattleEnemy::beginStateFaint();
}

void CHumanBoss::endStateFaint() {
    setTexAnmSeq(0, HumanBoss::NormalEyeSeqName);
    setTexAnmSeq(1, HumanBoss::NormalLipSeqName);
    CBattleEnemy::endStateFaint();
}

void CHumanBoss::beginStateItem() {
    INT id = getExtraParam(HumanBoss::ItemMotionParamName);
    INT effectNo = 0;

    switch (m_iUseItem) {
    case 0x2a:
        effectNo = 0;
        break;
    case 0x2b:
        effectNo = 1;
        break;
    case 0x2c:
        effectNo = 2;
        break;
    case 0x2d:
        effectNo = 3;
        break;
    case 0x2e:
        effectNo = 4;
        break;
    case 0x2f:
        effectNo = 5;
        break;
    }
    changeMotion(id, 0, FALSE, -1, effectNo, 0);
}

void CHumanBoss::stateItem() {
    if (isMotionEnd()) {
        m_pCardRevolver->setAction(0, 0);
        endCardBattle(__null, 6000);
    }
}

void CHumanBoss::beginStateDead() {
    CBossDeadDemo* demo;
    CCollBody* pHit;

    bodyCollOff(-1);
    setBossDeadEffect();
    changeMotion(selectDamageMotion_(), 0, FALSE, -1, 0, m_iDeadSeIdentifyNo);
    pHit = m_pBodyColl->getHitCollBody();
    if (pHit) {
        if (pHit->getDamageKind() == 3) {
            playBigDamageSE();
        } else {
            playSmallDamageSE();
        }
    }
    setTexAnmSeq(0, HumanBoss::DeadEyeSeqName);
    setTexAnmSeq(1, HumanBoss::DeadLipSeqName);
    demo = new KH_AT("(483)") CBossDeadDemo(this, 0);
    if (demo) {
        demo->setup(getHandle(), __null, -1);
    }
}

void CHumanBoss::stateDead() {
}

void CHumanBoss::endStateDead() {
}

/* MATCH: m_pEnemyParam read directly (friend in BattleEnemy.h; the getEnemyParam() inline loses the register copy of the param pointer in the
   loops); the id % 10000 in a local, UINT search index, one INT i per loop, `cards = deck` before ASSERT(deck) */
#line 421
void CHumanBoss::setupCardRevolver() {
    CARD_DATA* cardData;
    INT cardNum;
    CARD* deck;
    CARD* cards;

    if (!m_pEnemyParam) return;
    UINT id = (UINT)getID() % 10000;
    cardData = __null;
    for (UINT n = 0; n < 12; n++) {
        if (HumanBoss::g_CardDataTbl[n].iID == id) {
            cardData = &HumanBoss::g_CardDataTbl[n];
            m_pCardData = cardData;
            break;
        }
    }
    if (!cardData) return;
    cardNum = 1;
    for (INT i = 0; i < m_pEnemyParam->iCardCount; i++) {
        ENEMY_CARD* card = &m_pEnemyParam->pCards[i];
        INT j;
        for (j = 0; j < 10; j++) {
            if (card->iCardKind != 14) cardNum += card->bNumber[j];
        }
    }
    cardNum += m_pEnemyParam->iEnemyCard;
    m_iTotalCardNum = cardNum;
#line 537
    deck = new KH_AT("(537)") CARD[cardNum];
    cards = deck;
#line 539
    ASSERT(deck);
    for (INT i = 0; i < m_pEnemyParam->iCardCount; i++) {
        ENEMY_CARD* card = &m_pEnemyParam->pCards[i];
        INT cardKind;
        INT j;
        if (card->iCardKind == 14) continue;
        switch (card->iCardKind) {
        case 0:
        case 1:
        case 2:
        case 3:
        case 4:
        case 5:
        case 6:
        case 7:
            cardKind = cardData->iCardKind[card->iCardKind];
            break;
        case 8:
            cardKind = 0x2A;
            break;
        case 9:
            cardKind = 0x2B;
            break;
        case 10:
            cardKind = 0x2C;
            break;
        case 11:
            cardKind = 0x2D;
            break;
        case 12:
            cardKind = 0x2E;
            break;
        case 13:
            cardKind = 0x2F;
            break;
        }
        for (j = 0; j < 10; j++) {
            INT k;
            for (k = 0; k < card->bNumber[j]; k++) {
                if (DebugWork::g_dw.enemy.uiFlag & 0x4000) {
                    cards->init(cardKind, DebugWork::g_dw.enemy.iCardNumber);
                } else {
                    cards->init(cardKind, j);
                }
                cards++;
            }
        }
    }
    for (INT i = 0; i < m_pEnemyParam->iEnemyCard; i++) {
        cards->init(m_pEnemyParam->byEnemyCards[i], 0xFF);
        cards++;
    }
    cards->init(0xFF, 0);
    for (INT i = cardNum - 2; i > 0; i--) {
        INT j = (INT)((FLOAT)(i + 1) * RND::genrand_real1());
        CARD v = deck[i];
        deck[i] = deck[j];
        deck[j] = v;
    }
#line 622
    m_pCardRevolver = new KH_AT("(622)") CardBattle::CCardRevolver(this, deck, 2, -1);
#line 623
    ASSERT(m_pCardRevolver);
    if (deck) delete[] deck;
    if (m_pEnemyParam->iEnemyCard > 0) setHumanBossFlag(0, 1);
    m_iRemainCardNum = getAttackCardNum(3);
}

/* MATCH: `none` is not in the ref (user decision 2026-11-10): the normal arm stores a non-constant -1, so
   loop.c (combine_movables) keeps it a separate movable from the compares' -1 (li s4,-1 in the preheader);
   cse2 then folds it and none gets no hard register, which is why the .mdebug does not list it. Same values:
   -1.0f converts exactly to the SHORT -1. */
#line 635
void CHumanBoss::setupAct()
{
    if (!m_pEnemyParam) return;

    ACT_DATA tmp[8];
    ACT_DATA tmp2[8];
    ENEMY_ACT* act; FLOAT none;
    act = m_pEnemyParam->pActs; none = -1;
    for (INT i = 0; i < m_pEnemyParam->iActCount; i++, act++)
    {
        if (act->iCard[0] == -1 && act->iCard[1] == -1 && act->iCard[2] == -1)
        {
            continue;
        }

        if (act->iCard[0] == -1 || act->iCard[1] == -1 || act->iCard[2] == -1)
        {

            tmp[m_iNormalActNum].iState = act->iActNo + 2;
            tmp[m_iNormalActNum].pAct = act;
            tmp[m_iNormalActNum].bEnable = TRUE;
            tmp[m_iNormalActNum].sMinNumber = none;
            tmp[m_iNormalActNum].sMaxNumber = none;
            m_iNormalActNum++;
            continue;
        }


        tmp2[m_iStockNum].iState = act->iActNo + 2;
        tmp2[m_iStockNum].pAct = act;
        tmp2[m_iStockNum].bEnable = TRUE;
        tmp2[m_iStockNum].sMinNumber = -1;
        tmp2[m_iStockNum].sMaxNumber = -1;
        m_iStockNum++;
        ASSERT(m_iStockNum <= 8);
    }

    if (m_iNormalActNum > 0)
    {
        m_pNormalActs = new KH_AT("(674)") ACT_DATA[m_iNormalActNum];
        ASSERT(m_pNormalActs);
        for (INT i = 0; i < m_iNormalActNum; i++)
        {
            m_pNormalActs[i] = tmp[i];
        }
    }

    if (m_iStockNum > 0)
    {
        m_pStockData = new KH_AT("(684)") ACT_DATA[m_iStockNum];
        ASSERT(m_pStockData);
        for (INT i = 0; i < m_iStockNum; i++)
        {
            m_pStockData[i] = tmp2[i];
        }

        m_pUseStock = &m_pStockData[getRand() % m_iStockNum];
    }
#line 743
}
#line 664

void CHumanBoss::aiCardBattle() {
    CARD* activeCard;

    if (!getEnemyParam()) {
        return;
    }
    if (isReturnStock()) {
        if (!returnStockAct(0)) {
            return;
        }
        setHumanBossFlag(0, 0x40);
    }
    if (isStopCardBattle()) {
        return;
    }
#line 762
    ASSERT(m_pCardData);
    setBattleEnemyFlag(0, 0x82);
    if (m_pCardRevolver->isEnableCharge()) {
        if (!isStartRollDeck()) {
            setHumanBossFlag(0x10, 0);
        } else {
            setHumanBossFlag(0xa0, 0);
        }
    }
    if (reloadCardProc()) {
        return;
    }
    if (RND::genrand_int31() % getAttackRate()) {
        return;
    }
    activeCard = m_pCardRevolver->getActiveCardData();
    if (!activeCard) {
        return;
    }
    if (enemyCardProc(activeCard)) {
        return;
    }
    if (itemCardProc(activeCard)) {
        return;
    }
    if (!isEndRollDeck()) {
        if (stockActProc(activeCard)) {
            return;
        }
    }
    normalActProc(activeCard);
}

INT CHumanBoss::getDuelAttackState() {
    ACT_DATA* tmp[8];
    INT count = 0;
    INT i;
    INT j;

    for (i = 0; i < m_iNormalActNum; i++) {
        if (!m_pNormalActs[i].bEnable) {
            continue;
        }
        for (j = 0; j < 2; j++) {
            if (m_pNormalActs[i].pAct->iCard[j] != -1) {
                tmp[count++] = &m_pNormalActs[i];
                break;
            }
        }
    }
    if (count <= 0) {
        return -1;
    }
    return tmp[RND::genrand_int31() % count]->iState;
}

void CHumanBoss::selectStockAct() {
    ACT_DATA* stock[16];
    INT stockCount = 0;
    INT count = 0;
    INT i;

    for (i = 0; i < m_iStockNum; i++) {
        if (m_pStockData[i].bEnable) {
            stockCount++;
            if (m_pUseStock != &m_pStockData[i]) {
                stock[count++] = &m_pStockData[i];
            }
        }
    }
    if (count > 0) {
        m_pUseStock = stock[RND::genrand_int31() % count];
    } else if (stockCount <= 0) {
        m_pUseStock = __null;
    }
}

BOOL CHumanBoss::testEdge() {
    FVECTOR v;
    FLOAT roty;
    FVECTOR s;
    FVECTOR e;

    roty = Math3D::LimitPI(getRot().y + 3.1415927f);
    v.x = sinf(roty);
    v.z = cosf(roty);
    v.y = 0.0f;
    s = getPos();
    s.y += 1.0f;
    AddVector(e, s, v);
    CPointCollision coll;
    coll.setStartPos(s);
    coll.setEndPos(e);
    if (coll.collideMap() && coll.m_outNormal.y < 0.64278764f) {
        return TRUE;
    }
    return FALSE;
}

CHumanBoss::ACT_DATA* CHumanBoss::getStockAct(INT inState) const {
    INT i;
    for (i = 0; i < m_iStockNum; i++) {
        if (m_pStockData[i].iState == inState) {
            return &m_pStockData[i];
        }
    }
    return __null;
}

void CHumanBoss::enableStockAct(INT inState, BOOL inSw) {
    INT i;
    for (i = 0; i < m_iStockNum; i++) {
        if (m_pStockData[i].iState == inState) {
            m_pStockData[i].bEnable = inSw;
        }
    }
}

void CHumanBoss::enableNormalAct(INT inState, BOOL inSw) {
    INT i;
    for (i = 0; i < m_iNormalActNum; i++) {
        if (m_pNormalActs[i].iState == inState) {
            m_pNormalActs[i].bEnable = inSw;
        }
    }
}

void CHumanBoss::setStockActMaxMinNumber(INT inState, SHORT inMax, SHORT inMin) {
    INT i;
    for (i = 0; i < m_iStockNum; i++) {
        if (m_pStockData[i].iState == inState) {
            m_pStockData[i].sMaxNumber = inMax;
            m_pStockData[i].sMinNumber = inMin;
        }
    }
}

void CHumanBoss::bossCardRevolverDestroy() {
#line 985
    ASSERT(m_pCardRevolver);
    m_pCardRevolver->destroy();
}

BOOL CHumanBoss::returnStockAct(INT inFlag) {
#line 993
    ASSERT(m_pCardRevolver);
    if (m_pCardRevolver->getStock(0) > 0 && !m_pCardRevolver->returnStockCard(inFlag)) {
        setHumanBossFlag(0x40, 0);
        TRACE("CHumanBoss::\x83X\x83g\x83" "b\x83N\x8bZ\x89\xf0\x8f\x9c\x8e\xb8\x94s\n");
        return FALSE;
    }
    TRACE("CHumanBoss::\x83X\x83g\x83" "b\x83N\x8bZ\x89\xf0\x8f\x9c\x90\xac\x8c\xf7\n");
    clearStockTotalNumber();
    return TRUE;
}

BOOL CHumanBoss::reloadCardProc() {
    INT num;

    if (!m_pCardRevolver->isEnableCharge()) {
        return FALSE;
    }
    if (m_pCardRevolver->getHave(1, 0, 1, 0, 0) + m_pCardRevolver->getHave(2, 0, 1, 0, 0) <= 0) {
        if (m_pCardRevolver->getStock(1) >= 3) {
            if (setPlayStock(m_pUseStock->iState)) {
                return TRUE;
            }
        } else if (m_pCardRevolver->getStock(1) > 0) {
            returnStockAct(0);
            return TRUE;
        }
    }
    if (!isEndRollDeck()) {
        INT remain = getRemainCard(3);
        if (remain * 100 / m_iRemainCardNum >= 26 || (remain > 0 && RND::genrand_int31() % 20 == 0)) {
            m_pCardRevolver->rollDeck(1, 0, 300);
            return TRUE;
        }
    }
    num = m_pCardRevolver->chargeReloadCard(0);
    if (num == 1) {
        playReloadCompleteVoice();
        setHumanBossFlag(0, 0xb0);
        m_iRemainCardNum = getHaveCard(3);
        if (m_iRemainCardNum <= 0) {
            m_iRemainCardNum = num;
        }
    }
    setBattleEnemyFlag(0x80, 0);
    return TRUE;
}

/* placeholder for enemyCardProc (INCLUDE_ASM): its strings */
__attribute__((section(".sdata"))) void _enemyCardProc_placeholder(void* cb) {
    TRACE("----------- change normal card mode.\n");
    KH_DEBUG(if (!cb) AssertApp(__FILE__, 0, "cb", 0);)
    TRACE("----------- change enemy card mode.\n");
}
BOOL CHumanBoss::enemyCardProc(const CARD* inActiveCard) {
    INT n;
    CardBattle::CCardBattle* cb;

    INT mode = m_pCardRevolver->getMode();
    if (mode == 1) {
        if (!canEnemyCard(inActiveCard)) {
            m_pCardRevolver->rollDeck(1, 0, 300);
            return FALSE;
        }
        n = m_pCardRevolver->getRemain(-1, 0, 0);
        if (n < 2 || (RND::genrand_int31() & 1) == 0) {
            if (n <= 0) {
                setHumanBossFlag(1, 0);
                m_pCardRevolver->changeMode(-1, 0);
            } else if (m_pCardRevolver->setPlayCard(0, 0)) {
                playUseEnemyCardVoice();
                if (n == mode) {
                    setHumanBossFlag(1, 0);
                }
            }
            TRACE("----------- change normal card mode.\n");
            return TRUE;
        }
        m_pCardRevolver->rollDeck(1, 0, 300);
        return TRUE;
    }
    if (isNoEnemyCard()) {
        return FALSE;
    }
    cb = (CardBattle::CCardBattle*)CTaskMan::getSysTask(0x16);
#line 1116
    ASSERT(cb);
    if (cb->getEnchantKind(1)) {
        return FALSE;
    }
    if (!canEnemyCard()) {
        return FALSE;
    }
    if (RND::genrand_int31() % 50) {
        return FALSE;
    }
    m_pCardRevolver->changeMode(-1, 0);
    TRACE("----------- change enemy card mode.\n");
    return TRUE;
}

BOOL CHumanBoss::itemCardProc(const CARD* inActiveCard) {
    INT noReloadNum;
    INT cardNum;
    INT haveCardNum;
    INT remainCard;
    INT rate;
    INT kind[16];
    INT count;
    INT i;
    ENEMY_CARD* card;
    INT cardKind;
    BOOL find;

    if ((DebugWork::g_dw.enemy.uiFlag & 8) && DebugWork::g_dw.enemy.sActNo + 2 == 10) {
        count = 0;
        for (i = 0; i < getEnemyParam()->iCardCount; i++) {
            card = &getEnemyParam()->pCards[i];
            cardKind = 0;
            switch (card->iCardKind) {
            case 8:
                cardKind = 0x2a;
                break;
            case 9:
                cardKind = 0x2b;
                break;
            case 10:
                cardKind = 0x2c;
                break;
            case 11:
                cardKind = 0x2d;
                break;
            case 12:
                cardKind = 0x2e;
                break;
            case 13:
                cardKind = 0x2f;
                break;
            }
            find = FALSE;
            {
                INT i;
                for (i = 0; i < count; i++) {
                    if (kind[i] == cardKind) {
                        find = TRUE;
                        break;
                    }
                }
            }
            if (!find) {
                kind[count++] = cardKind;
            }
        }
        if (count > 0) {
            m_iUseItem = kind[RND::genrand_int31() % count];
            setPlayCard(10);
            return TRUE;
        }
    }
    noReloadNum = -1;
    cardNum = 0;
    haveCardNum = 0;
    rate = getExtraParam(HumanBoss::UseItemCardParamName);
    remainCard = 0;
    switch (inActiveCard->m_nKind) {
    case 0x2a:
    case 0x2b:
    case 0x2c:
        cardNum = getAttackCardNum(1);
        haveCardNum = m_pCardRevolver->getHave(1, 1, 1, 0, 0);
        remainCard = getRemainCard(1);
        noReloadNum = cardNum * rate / 100;
        break;
    case 0x2d:
    case 0x2e:
        cardNum = getAttackCardNum(2);
        haveCardNum = m_pCardRevolver->getHave(2, 1, 1, 0, 0);
        remainCard = getRemainCard(2);
        noReloadNum = cardNum * rate / 100;
        break;
    case 0x2f:
        cardNum = getAttackCardNum(3);
        haveCardNum = m_pCardRevolver->getHave(1, 1, 1, 0, 0) + m_pCardRevolver->getHave(2, 1, 1, 0, 0);
        remainCard = getRemainCard(3);
        noReloadNum = cardNum * rate / 100;
        break;
    }
    if (noReloadNum < 0) {
        return FALSE;
    }
    if (isEndRollDeck()) {
    use:
        __asm__ volatile(""); /* MATCH: reorg cannot take the move a0,s1 for the branches into here */
        if (setPlayCard(10)) {
            m_iUseItem = inActiveCard->m_nKind;
        }
        return TRUE;
    }
    if (noReloadNum > 0 && cardNum - haveCardNum < noReloadNum) {
        __asm__ volatile("" : : : "$2"); /* MATCH: v0 is dead here: reorg fills the test's slot from the li v0,100 behind it */
    roll:
        m_pCardRevolver->rollDeck(1, 0, 300);
        __asm__ volatile(""); /* MATCH: keeps this tail: jump2 would cross-jump it into the last one */
        return TRUE;
    }
    if (remainCard * 100 / cardNum >= 26) goto roll;
    remainCard >>= 1;
    if (remainCard <= 0) {
        remainCard = 1;
    }
    if (RND::genrand_int31() % remainCard == 0) {
        goto use;
    }
    {
        register INT nCount asm("$5") = 1; /* MATCH: li a1 before the member load, for the mfhi test's slot */
        __asm__ volatile("" : : "r"(nCount));
        m_pCardRevolver->rollDeck(nCount, 0, 300);
    }
    return TRUE;
}

/* placeholder for stockActProc (INCLUDE_ASM): its strings */
__attribute__((section(".sdata"))) void _stockActProc_placeholder(INT n) {
    TRACE("CHumanBoss::\x83X\x83g\x83" "b\x83N\x82\xc5\x82\xab\x82\xc8\x82\xa2\n");
    TRACE("----------- stock boss card = 0x%08x\n", n);
}
BOOL CHumanBoss::stockActProc(const CARD* inActiveCard) {
    INT attackCardNum;
    INT magicCardNum;
    INT stockCount;
    ACT_DATA* stockData[16];
    INT count;
    INT i;
    INT state;
    INT cardKind;

    if (!m_pUseStock) {
        return FALSE;
    }
    attackCardNum = getAttackCardNum(1);
    magicCardNum = getAttackCardNum(2);
    if (attackCardNum > 0 && magicCardNum > 0) {
        if (m_pCardRevolver->getHave(1, 1, 1, 0, 0) < 3 && m_pCardRevolver->getHave(2, 1, 1, 0, 0) < 3) {
            return FALSE;
        }
    } else if (attackCardNum > 0) {
        if (m_pCardRevolver->getHave(1, 1, 1, 0, 0) < 3) {
            return FALSE;
        }
    } else if (magicCardNum > 0) {
        if (m_pCardRevolver->getHave(2, 1, 1, 0, 0) < 3) {
            return FALSE;
        }
    }
    stockCount = m_pCardRevolver->getStock(0);
    if (stockCount < 3 && !canStockAct(m_pUseStock)) {
        TRACE("CHumanBoss::\x83X\x83g\x83" "b\x83N\x82\xc5\x82\xab\x82\xc8\x82\xa2\n");
        count = 0;
        for (i = 0; i < m_iStockNum; i++) {
            if (m_pStockData[i].bEnable && m_pUseStock != &m_pStockData[i] && canStockAct(&m_pStockData[i])) {
                stockData[count++] = &m_pStockData[i];
            }
        }
        if (count > 0) {
            m_pUseStock = stockData[RND::genrand_int31() % count];
        }
        if (stockCount > 0) {
            returnStockAct(0);
            return TRUE;
        }
        return FALSE;
    }
    if (m_pCardRevolver->getStock(1) >= 3) {
        if (RND::genrand_int31() % m_iUseStockRate) {
            return FALSE;
        }
        if (!canAttack_(m_pUseStock->pAct)) {
            return FALSE;
        }
        if (DebugWork::g_dw.enemy.uiFlag & 8) {
            state = DebugWork::g_dw.enemy.sActNo + 2;
            {
                INT i;
                for (i = 0; i < m_iNormalActNum; i++) {
                    if (m_pNormalActs[i].iState == state) {
                        return setPlayStock(state) != 0;
                    }
                }
            }
            {
                INT i;
                for (i = 0; i < m_iStockNum; i++) {
                    if (m_pStockData[i].iState == state) {
                        return setPlayStock(state) != 0;
                    }
                }
            }
            return TRUE;
        }
        if (setPlayStock(m_pUseStock->iState)) {
            return TRUE;
        }
    } else {
        if (RND::genrand_int31() % m_iStockRate) {
            return FALSE;
        }
        if (stockCount >= 3) {
            return FALSE;
        }
        cardKind = m_pCardData->iCardKind[m_pUseStock->pAct->iCard[stockCount]];
        if (cardKind == inActiveCard->m_nKind) {
            if (inActiveCard->m_nNumber <= 0) {
                if (!isPerMissionStockZero() && m_pCardRevolver->getRemain(0, cardKind, 0) >= 2) {
                    if ((RND::genrand_int31() & 1) == 0) {
                        m_pCardRevolver->rollDeck(1, 0, 300);
                        return TRUE;
                    }
                    return FALSE;
                }
                setHumanBossFlag(0, 0x80);
            }
            if (setStockCard(inActiveCard)) {
                TRACE("----------- stock boss card = 0x%08x\n", inActiveCard->m_nKind);
                return TRUE;
            }
            return FALSE;
        }
        if ((RND::genrand_int31() & 1) == 0) {
            m_pCardRevolver->rollDeck(1, 0, 300);
            return TRUE;
        }
        return FALSE;
    }
    return FALSE;
}

BOOL CHumanBoss::normalActProc(const CARD* inActiveCard) {
    ACT_DATA* tmp[8];
    INT count = 0;
    ACT_DATA* act;
    INT i;
    INT j;
    INT card;
    register INT nNone asm("$22"); /* MATCH: the -1 in s6, &tmp[count] in s5 */

    for (i = 0; i < m_iNormalActNum; i++) {
        if (!m_pNormalActs[i].bEnable) {
            continue;
        }
        for (j = 0, nNone = -1; j < 2; j++) {
            card = m_pNormalActs[i].pAct->iCard[j];
            if (card == nNone) {
                continue;
            }
            if (m_pNormalActs[i].pAct->iCard[0] != nNone && m_pNormalActs[i].pAct->iCard[1] != nNone) {
                if (RND::genrand_int31() & 1) {
                    continue;
                }
            }
            if (((DebugWork::g_dw.enemy.uiFlag & 8) && canAttack_(m_pNormalActs[i].pAct)) ||
                (m_pCardData->iCardKind[card] == inActiveCard->m_nKind && canAttack_(m_pNormalActs[i].pAct))) {
                tmp[count++] = &m_pNormalActs[i];
                break;
            }
        }
    }
    if (count <= 0) {
        goto roll;
    }
    act = tmp[RND::genrand_int31() % count];
    if (getPlayerState() == 11) {
        if (CardBattle::CCardBattle::getPlayingCardPoint(1) > inActiveCard->m_nNumber && inActiveCard->m_nNumber != 0) {
            goto roll;
        }
        setDoBattleAct(act);
        useCard(act->iState);
        goto ret;
    roll:
        m_pCardRevolver->rollDeck(1, 0, 300);
        return FALSE;
    }
    setDoBattleAct(act);
    useCard(act->iState);
ret:
    return TRUE;
}

void CHumanBoss::setDoBattleAct(const ACT_DATA* inAct) {
#line 1634
    ASSERT(inAct);
    if (m_pDoBattleAct == inAct) {
        m_iActCount++;
    } else if (--m_iActCount < 0) {
        m_iActCount = 0;
    }
    m_pDoBattleAct = (ACT_DATA*)inAct;
}

void CHumanBoss::useCard(INT inState) {
    INT state;

    if (DebugWork::g_dw.enemy.uiFlag & 8) {
        state = DebugWork::g_dw.enemy.sActNo + 2;
        {
            INT i;
            for (i = 0; i < m_iNormalActNum; i++) {
                if (m_pNormalActs[i].iState == state) {
                    setPlayCard(state);
                    return;
                }
            }
        }
        {
            INT i;
            for (i = 0; i < m_iStockNum; i++) {
                if (m_pStockData[i].iState == state) {
                    setPlayCard(state);
                    return;
                }
            }
        }
        return;
    }
    setPlayCard(inState);
}

void CHumanBoss::playMyRemainHpVoice() {
    INT myHp;
    INT voiceID;
    INT id;

    if (isMyHpRemainVoice()) {
        return;
    }
    myHp = getExtraParam(HumanBoss::MyHpRemainParamName);
    if (getHp() * 100 / getHpMax() <= myHp) {
        voiceID = getExtraParam(HumanBoss::VoiceIDParamName);
        id = getExtraParam(HumanBoss::MyHpVoiceParamName);
        if (voiceID && id) {
            playVoice(voiceID, id, 1);
        }
        setHumanBossFlag(8, 0);
    }
}

void CHumanBoss::playPlayerRemainHpVoice() {
    INT playerHp;
    INT voiceID;
    INT id;

    if (isPlayerHpRemainVoice()) {
        return;
    }
    playerHp = getExtraParam(HumanBoss::PlHpRemainParamName);
    if (HumanBoss::GetPlayerHpPercent() <= playerHp) {
        voiceID = getExtraParam(HumanBoss::VoiceIDParamName);
        id = getExtraParam(HumanBoss::PlHpVoiceParamName);
        if (voiceID && id) {
            playVoice(voiceID, id, 1);
        }
        setHumanBossFlag(4, 0);
    }
}

void CHumanBoss::playReloadCompleteVoice() {
    INT voiceID = getExtraParam(HumanBoss::VoiceIDParamName);
    INT id = getExtraParam(HumanBoss::ReloadVoiceParamName);
    if (voiceID && id) {
        playVoice(voiceID, id, 1);
    }
}

void CHumanBoss::playUseEnemyCardVoice() {
    INT voiceID = getExtraParam(HumanBoss::VoiceIDParamName);
    INT id = getExtraParam(HumanBoss::ECardVoiceParamName);
    if (voiceID && id) {
        playVoice(voiceID, id, 1);
    }
}

BOOL CHumanBoss::canStockAct(const ACT_DATA* inAct) {
    INT i;

#line 1755
    ASSERT(__null != inAct);
    for (i = 0; i < 3; i++) {
        if (m_pCardRevolver->getRemain(0, m_pCardData->iCardKind[inAct->pAct->iCard[i]], 0) <= 0) {
            return FALSE;
        }
    }
    return TRUE;
}

/* MATCH: the roll >= 5 test is the else-if of the reset test, one shared return FALSE after both (separate returns
   cross-jump or change reorg's delay-slot fills) */
BOOL CHumanBoss::setStockCard(const CARD* inActiveCard) {
    SHORT totalNumber = m_iStockTotalNumber + inActiveCard->m_nNumber;

    if (m_pCardRevolver->getStock(0) == 2 && m_pUseStock->sMinNumber > 0) {
        if (totalNumber < m_pUseStock->sMinNumber || m_pUseStock->sMaxNumber < totalNumber) {
            if (m_iStockTotalNumber >= m_pUseStock->sMaxNumber || m_pUseStock->sMinNumber - m_iStockTotalNumber >= 10 ||
                ++m_iStockRollCount >= 10) {
                clearStockTotalNumber();
                returnStockAct(0);
                selectStockAct();
            } else if (m_iStockRollCount >= 5) {
                setHumanBossFlag(0x80, 0);
            }
            return FALSE;
        }
        TRACE_USER((USER)5, "getStock() = %d, TotalNumber = %d, CurrentNumber = %d, min = %d, max = %d\n",
                   m_pCardRevolver->getStock(0), totalNumber, m_iStockTotalNumber, m_pUseStock->sMinNumber,
                   m_pUseStock->sMaxNumber);
    }
    m_iStockTotalNumber = totalNumber;
    m_pCardRevolver->setStockCard();
    return TRUE;
}

BOOL CHumanBoss::setPlayCard(INT inState) {
    if (!m_pCardRevolver->setPlayCard(0, 0)) {
        return FALSE;
    }
    m_iAttackState = inState;
    setBattleEnemyFlag(0x201, 0);
    setHumanBossFlag(0, 0xb0);
    return TRUE;
}

BOOL CHumanBoss::setPlayStock(INT inState) {
    if (!m_pCardRevolver->setPlayStock(0, 0)) {
        return FALSE;
    }
    setBattleEnemyFlag(0x201, 0);
    m_iAttackState = inState;
    m_pDoBattleAct = m_pUseStock;
    clearStockTotalNumber();
    selectStockAct();
    return TRUE;
}
// SOURCE DATA: .data .rodata
