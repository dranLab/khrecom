#define GAME_CARDREVOLVER_CPP
#include "khtypes.h"
#include "Game/CardRevolver.h"
#include "System/TaskMan.h"
#include "System/Sound/SoundMan.h"
#include "Game/GameWork.h"
#include "Game/Player/Player.h"
#include "System/Debug.h"
#include "System/HeapMan.h"
#include "include_asm.h"

using namespace CardBattle;

inline void* operator new(unsigned int, void* p) throw() { return p; }
#define RET_ADDR() ({ UINT nRA; __asm__ volatile("daddu %0, $31, $0" : "=r"(nRA)); nRA; })

extern LPCSTR g_szSeqCockpit __asm__("_10CardBattle$g_szSeqCockpit_0039B2E4");
template <class T> static inline T& _ref(T& v) { return v; }
static inline BOOL _isEqual(UINT a, UINT b) { return a == b; }
namespace CardBattle {
extern char* g_szStockSkillName[150];
}
extern "C" char D_0047B438[];
extern "C" char D_0047B678[];
extern "C" char D_0047B710[];
extern "C" char D_0047B740[];
extern "C" char D_0047B348[];
extern "C" char D_0047B378[];
extern "C" char D_0047B070[];
extern "C" char D_0047B1E0[];
extern "C" char D_0047B210[];
extern "C" const CARD_DATA cardDataTbl_0047C338[224];

static inline void _setCard(CARD& c, USHORT kind, BYTE num) {
    c.m_nId = 0;
    c.m_nKind2 = kind;
    c.m_nNumber2 = num;
    c.m_nCategory20 = cardDataTbl_0047C338[c.m_nKind].nCategory0;
    c.m_nCategory21 = cardDataTbl_0047C338[c.m_nKind].nCategory1;
}

void CardBattle::convBtlDeckToCard(BTL_DECK* inDeck) {
    for (INT i = 0; i < 100; i++) {
        if (inDeck[i].m_nKind <= 0 || inDeck[i].m_nKind > 202) break;
        _setCard(*(CARD*)&inDeck[i], (BYTE)inDeck[i].m_nKind, inDeck[i].m_nNumber);
    }
}
CCardRevolver::CCardRevolver(CTask* pParent, CARD* inDeck, INT inUser, INT inShortCut) : CTask(pParent, 0x4000) {
    m_uiUser = inUser;
    m_hOwner = pParent->getHandle();
    _init();
    _initialize(inDeck, inShortCut);
    _initCounter();
    CCardBattle* pBattle = (CCardBattle*)CTaskMan::getSysTask(0x16);
    if (pBattle) pBattle->setRevolver(m_uiUser, this);
}
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Game/CardRevolver", _$_Q210CardBattle13CCardRevolver);
void CCardRevolver::_init() {
    INT i;
    m_pCards = NULL;
    m_pCardsE = NULL;
    m_bActive = FALSE;
    m_pActiveCard = NULL;
    m_uiFlag = 0;
    m_iState = 0;
    m_iMode = 0;
    for (i = 0; i < 2; i++) {
        m_nTotal[i] = 0;
        m_nRemain[i] = 0;
        m_nActive[i] = 0;
    }
    m_iChargeCount = 1;
    m_tqChargeRate = 100;
    m_tqRollSpeed = 300;
    m_nShortCutID = -1;
    m_nLimit = 0;
    m_nExtra = 0;
    m_nEnd = 0;
    m_tqTimer = 0;
    m_iCharge = 0;
    m_tqCharge = 0;
    m_iStock = 0;
    m_iStockSum = 0;
    m_iStockSkill = 0;
    m_iExtraSkill = 0;
    m_iFirstSkill = 0;
    m_hPlaySE = 0;
    m_hChargeSE = 0;
    m_iReloadSEID = 0;
    m_pEmpty = NULL;
    m_iAction = 0;
    m_nReloadCard = 0;
    m_nReloadID = 0;
    m_nAdd = 0;
    m_nReturn = 0;
    for (i = 0; i < 39; i++) {
        _setCard(m_cardAdd[i], 0, 0);
    }
    for (i = 0; i < 7; i++) {
        m_pCardsS[i] = NULL;
        m_pDataS[i] = NULL;
    }
    initStock();
    m_hSnatch = 0;
    m_nSnatchCount = 0;
    m_tqSnatch = 0;
    m_tqDiscard = 0;
    m_nPlutoMark = 0;
    m_nReloadCount = 0;
    m_iMax = 0;
    m_pEmpty = NULL;
}
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Game/CardRevolver", _initialize__Q210CardBattle13CCardRevolverP4CARDi);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Game/CardRevolver", _initCounter__Q210CardBattle13CCardRevolver);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Game/CardRevolver", exeWait__Q210CardBattle13CCardRevolver);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Game/CardRevolver", main__Q210CardBattle13CCardRevolver);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Game/CardRevolver", exeMain__Q210CardBattle13CCardRevolver);
void CCardRevolver::destroy() {
    initHide();
}
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Game/CardRevolver", initHide__Q210CardBattle13CCardRevolver);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Game/CardRevolver", setDeck__Q210CardBattle13CCardRevolverP4CARDi);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Game/CardRevolver", rollDeck__Q210CardBattle13CCardRevolveriii);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Game/CardRevolver", changeMode__Q210CardBattle13CCardRevolverii);
void CCardRevolver::rollShortCut(INT inMode) {
    if (m_iMode != 0) return;
    if (CGameWork::getFlag(1, 9)) return;
    if (m_iState != 1) return;
    if (!m_bActive) return;
    if (m_nTotal[0] < 2) return;
    INT id;
    switch (inMode) {
    case 1:
        id = m_nReloadID;
        break;
    case 2:
        id = m_nShortCutID;
        break;
    default:
        return;
    }
    if (id == -1) return;
    CBattleCard* p = &m_pCards[id];
    INT n = p->getHand();
    if (n <= 0) return;
    CSound::playSE(14, -1, 64);
    rollDeck(n, 1, 300);
}
CBattleCard* CCardRevolver::getActiveCard() const {
    if (!m_bActive) return NULL;
    INT mode = m_iMode;
    INT total = m_nTotal[mode];
    INT active = m_nActive[mode];
    INT remain = m_nRemain[mode];
    CBattleCard* pCards;
    if (mode != 0) {
        pCards = m_pCardsE;
    } else {
        remain += m_nReloadCard;
        pCards = m_pCards;
    }
    if (!pCards) return NULL;
    if (!total) return NULL;
    if (!remain) return NULL;
    CBattleCard* pCard = &pCards[active];
    if (pCard == NULL || !pCard->isNowSelect()) return NULL;
    return pCard;
}
CARD* CCardRevolver::getActiveCardData() {
    CBattleCard* pCard = getActiveCard();
    if (pCard) return pCard->getCardData();
    return NULL;
}
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Game/CardRevolver", setPlayCard__Q210CardBattle13CCardRevolverii);
INT CCardRevolver::isEnableCharge() {
    if (m_uiFlag & 0x10000) return 0;
    if (m_uiFlag & 2) return m_uiFlag & 4;
    m_uiFlag |= 2;
    if (m_iState != 1) return 0;
    CBattleCard* pCard = getActiveCard();
    if (!pCard) return 0;
    if ((pCard->getCardData()->m_nId & 0xF0000000) != 0x80000000) return 0;
    m_uiFlag |= 4;
    return 1;
}
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Game/CardRevolver", chargeReloadCard__Q210CardBattle13CCardRevolveri);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Game/CardRevolver", initReload__Q210CardBattle13CCardRevolveri);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Game/CardRevolver", exeReload__Q210CardBattle13CCardRevolver);
INT CCardRevolver::getRemain(INT inMode, INT inKind, INT inCategory) {
    if (inMode < 0) return m_nRemain[m_iMode];
    if (inKind == 0 && inCategory == 0) return m_nRemain[inMode];
    INT total = inMode == 0 ? m_nLimit : m_nTotal[inMode];
    INT n = 0;
    CBattleCard* pCards = inMode == 0 ? m_pCards : m_pCardsE;
    INT i;
    if (inKind) {
        for (i = 0; i < total; i++) {
            CBattleCard* pCard = &pCards[i];
            if (!pCard) continue;
            if (pCard->getHand() == -1) continue;
            if (pCard->getStock() != -1) continue;
            if (pCard->getCardKind() == inKind) n++;
        }
    } else if (inCategory) {
        for (i = 0; i < total; i++) {
            CBattleCard* pCard = &pCards[i];
            if (!pCard) continue;
            if (pCard->getHand() == -1) continue;
            if (pCard->getStock() != -1) continue;
            if (pCard->getCardCategory() == inCategory) n++;
        }
    }
    return n;
}
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Game/CardRevolver", setStockCard__Q210CardBattle13CCardRevolver);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Game/CardRevolver", returnStockCard__Q210CardBattle13CCardRevolveri);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Game/CardRevolver", calcStockSkill__Q210CardBattle13CCardRevolver);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Game/CardRevolver", calcEnemyStockSkill__Q210CardBattle13CCardRevolver);
INT CCardRevolver::getStock(INT inFlag) {
    if (!inFlag) return m_iStock;
    INT n = 0;
    for (INT i = 0; i < 7; i++) {
        if (!m_pCardsS[i]) break;
        if (!m_pCardsS[i]->isNowStock()) break;
        n++;
    }
    return n;
}
INT CCardRevolver::setPlayStock(INT inFlag, INT inTest) {
    if (CGameWork::getFlag(1, 0x2f)) return 0;
    UINT uiEnd = CCardBattle::m_pInstance ? CCardBattle::m_pInstance->m_uiFlag & 0xC0000000 : 0;
    if (uiEnd) return 0;
    if (m_iState != 1) return 0;
    if (m_iStock <= 0) return 0;
    if (m_uiUser == 2 && getStock(0) != 3) return 0;
    if (!m_bActive) return 0;
    if (m_uiUser == 1 && getStock(1) != m_iStock) return 0;
    CCardBattle* pBattle = (CCardBattle*)CTaskMan::getSysTask(0x16);
    if (pBattle->isPlayingGimmickCard()) return 0;
    if (inFlag) {
        if (!pBattle->testBreakingCard(m_uiUser)) return 0;
        TRACE(D_0047B070);
        pBattle->resetNowPlaying(m_uiUser);
    }
    if (!pBattle->setPlayStock(m_hOwner, m_uiUser, m_pCardsS, m_iStockSkill, m_iExtraSkill, m_iFirstSkill, inTest)) return 0;
    for (INT i = 0; i < m_iStock; i++) {
        m_pCardsS[i] = NULL;
        m_pDataS[i] = NULL;
    }
    m_iStockSum = 0;
    m_uiFlag = (INT)m_uiFlag & ~0x200;
    m_iStockSkill = 0;
    m_iExtraSkill = 0;
    m_iFirstSkill = 0;
    m_iStock = 0;
    return 1;
}
INT CCardRevolver::getHave(INT inCategory, INT inPlay, INT inOwner, INT inReload, INT inNoGimmick) {
    INT n = 0;
    for (INT i = 0; i < m_nLimit; i++) {
        CBattleCard* pCard = &m_pCards[i];
        if (!pCard) continue;
        UINT uiFlag = pCard->getFlag();
        if (uiFlag & 0x20) continue;
        if (uiFlag & 0x200000) continue;
        if (!inOwner && pCard->getCardOwner() != m_uiUser) continue;
        INT cat = pCard->getCardCategory();
        if (inNoGimmick) {
            if (cat == 6) continue;
            if (cat == 14) continue;
        }
        if (m_uiFlag & 0x100) {
            if (uiFlag & 0x8000) continue;
        }
        if (inCategory && cat != inCategory) continue;
        if (!inPlay) {
            if (uiFlag & 0x40) continue;
            if (uiFlag & 4) continue;
        }
        if (!inReload && cat == 8) continue;
        n++;
    }
    return n;
}
INT CCardRevolver::checkBreakCard(INT inNum, INT inZero, BOOL inCheckGreater) {
    for (INT i = 0; i < m_nLimit; i++) {
        CBattleCard* pCard = &m_pCards[i];
        if (!pCard) continue;
        if (pCard->isReloadNG()) continue;
        INT num = pCard->getBaseCardNumber();
        if (inCheckGreater && inNum < num) continue;
        if (num >= inNum) return 1;
        if (num == 0 && inZero) return 1;
    }
    return 0;
}
void CCardRevolver::setAction(INT inID, INT inNum) {
    m_iAction = inID;
    if (m_iAction == 0) {
        CCardBattle* pBattle = (CCardBattle*)CTaskMan::getSysTask(0x16);
        m_iAction = pBattle->getPlayingCardID(m_uiUser, inNum);
    }
    if (m_iAction > 0) initAction();
}
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Game/CardRevolver", initAction__Q210CardBattle13CCardRevolver);
void CCardRevolver::exeAction() {
    m_tqTimer -= getFrameRate();
    if (m_tqTimer > 0) return;
    if (m_uiFlag & 8) {
        rollDeck(0, 1, 300);
        m_tqTimer = 0;
        m_iState = 1;
        TRACE(D_0047B1E0);
    } else {
        m_tqTimer = 200;
        m_uiFlag |= 8;
        rollDeck(0, 1, 300);
        m_tqTimer = 400;
        TRACE(D_0047B210);
    }
}
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Game/CardRevolver", addCard__Q210CardBattle13CCardRevolverP4CARDi);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Game/CardRevolver", exeAddCard__Q210CardBattle13CCardRevolver);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Game/CardRevolver", initCounter__Q210CardBattle13CCardRevolver);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Game/CardRevolver", exeCounter__Q210CardBattle13CCardRevolver);
void CCardRevolver::destroyCounter() {
    if (!m_pSeqCtrlCounter) return;
    m_uiCounterState = 3;
    m_pSeqCtrlCounter->replaceSeq(0, _ref(g_szSeqCockpit), 0x44);
    m_uiCounterFlag |= 1;
    m_pSeqCtrlCounter->enableDisp(2, FALSE);
    m_pSeqCtrlCounter_Num01->enableDisp(FALSE);
    m_pSeqCtrlCounter_Num10->enableDisp(FALSE);
    m_uiScutState = 4;
    m_pSeqCtrlCounter->replaceSeq(1, _ref(g_szSeqCockpit), 0x40);
    m_uiCounterFlag |= 2;
}
void CCardRevolver::deleteCounter() {
    delete m_pSeqCtrlCounter;
    m_pSeqCtrlCounter = NULL;
    delete m_pSeqCtrlCounter_Num01;
    m_pSeqCtrlCounter_Num01 = NULL;
    delete m_pSeqCtrlCounter_Num10;
    m_pSeqCtrlCounter_Num10 = NULL;
    delete m_pSeqCtrlNumEff;
    m_pSeqCtrlNumEff = NULL;
    delete m_pSeqCtrlTrap;
    m_pSeqCtrlTrap = NULL;
}
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Game/CardRevolver", setNumberEffect__Q210CardBattle13CCardRevolveri);
void CCardRevolver::_initNumEffect() {
    m_uiNumEffState = 0;
    m_pSeqCtrlNumEff = new (com_malloc_d(sizeof(D2::C2DSeqCtrl), D_0047B348, RET_ADDR())) D2::C2DSeqCtrl;
}
void CCardRevolver::exeNumEffect() {
    switch (m_uiNumEffState) {
    case 0:
        break;
    case 1:
        if (m_pSeqCtrlNumEff->isEnd(0)) {
            m_uiNumEffState = 3;
            m_pSeqCtrlNumEff->replaceSeq(0, _ref(g_szSeqCockpit), 0x79);
        }
        break;
    case 2:
        if (m_pSeqCtrlNumEff->isEnd(0)) {
            m_uiNumEffState = 3;
            m_pSeqCtrlNumEff->replaceSeq(0, _ref(g_szSeqCockpit), 0x77);
        }
        break;
    case 3:
        if (m_pSeqCtrlNumEff->isEnd(0)) {
            m_uiNumEffState = 0;
            m_pSeqCtrlNumEff->destroy();
        }
        break;
    }
}
void CCardRevolver::setTrapEffect() {
    if (!m_pSeqCtrlTrap) {
        m_pSeqCtrlTrap = new (com_malloc_d(sizeof(D2::C2DSeqCtrl), D_0047B378, RET_ADDR())) D2::C2DSeqCtrl;
    }
    m_pSeqCtrlTrap->setLayer(8);
    m_pSeqCtrlTrap->addSeq(g_szSeqCockpit, 0x96);
    CB_LAYOUT& lay = g_LayoutData[58];
    FVECTOR vPos = {{ (FLOAT)lay.x, (FLOAT)lay.y, 0.0f, 1.0f }};
    m_pSeqCtrlTrap->setPos(vPos);
}
INT CCardRevolver::exeTrapEffect(INT inFlag) {
    if (!m_pSeqCtrlTrap) return 0;
    if (!inFlag || m_pSeqCtrlTrap->isEnd(0)) {
        delete m_pSeqCtrlTrap;
        m_pSeqCtrlTrap = NULL;
        return 0;
    }
    return 1;
}
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Game/CardRevolver", initStock__Q210CardBattle13CCardRevolver);
void CCardRevolver::initStockSkillName(D2::C2DSeqCtrl* inSeq) {
    G_RGBA rgba;
    rgba.col = 0x80808080;
    m_p2DFntStockName = new (com_malloc_d(sizeof(D2::C2DFnt), D_0047B438, RET_ADDR()))
        D2::C2DFnt(0, 0, &rgba, 14, g_szStockSkillName[m_iStockSkill], 0x80);
    inSeq->addFntShift(g_szSeqCockpit, m_p2DFntStockName, -4.0f, 0.0f);
    inSeq->m_seqControl.fgColor = 1;
    inSeq->m_seqControl.fgFntScale = 1;
}
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Game/CardRevolver", exeStock__Q210CardBattle13CCardRevolver);
void CCardRevolver::deleteStock() {
    delete m_pSeqCtrlStock;
    m_pSeqCtrlStock = NULL;
    delete m_pSeqCtrlStock_Num01;
    m_pSeqCtrlStock_Num01 = NULL;
    delete m_pSeqCtrlStock_Num10;
    m_pSeqCtrlStock_Num10 = NULL;
}
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Game/CardRevolver", setStockInfo__Q210CardBattle13CCardRevolver);
void CCardRevolver::changeDarkMode(HANDLE inHandle, INT inFlag) {
    changeOwner(inHandle);
    if (inFlag) m_uiFlag |= 0x100;
    else m_uiFlag = (INT)m_uiFlag & ~0x100;
    calcStockSkill();
}
INT CCardRevolver::setPlayerDuelCard(INT inTest) {
    if (m_iState != 1) return 0;
    if (CGameWork::getFlag(1, 8)) return 0;
    if (CGameWork::getInt(1, 0xE2) & 2) return 0;
    CCardBattle* pBattle = (CCardBattle*)CTaskMan::getSysTask(0x16);
    if (!_isEqual(pBattle->m_uiDuelState, 5)) return 0;
    if (!_isEqual(pBattle->m_uiState[1], 10)) return 0;
    if (pBattle->isExecutingCard(1)) return 0;
    CBattleCard* pCard = getActiveCard();
    if (!pCard) return 0;
    INT cat = pCard->getCardData()->m_nId >> 28;
    if (cat >= 6) {
        if (cat < 9) return 0;
        if (cat == 14) return 0;
    }
    INT ret = pBattle->setPlayCard(m_hOwner, m_uiUser, pCard, NULL, inTest);
    if (!ret) return 0;
    m_nRemain[m_iMode]--;
    m_tqRollSpeed = 300;
    rollDeck(1, 1, 300);
    return ret;
}
void CCardRevolver::setPlayerDuelStock(CBattleCard* inCard, UINT inPri) {
    m_iStock = inPri;
    m_uiFlag |= 0x200;
    m_pCardsS[m_iStock] = inCard;
    m_pDataS[m_iStock] = inCard->getCardData();
    m_iStock++;
    CSound::playSE(15, -1, 64);
}
INT CCardRevolver::setPlayerDuelPlayStock() {
    CCardBattle* pBattle = (CCardBattle*)CTaskMan::getSysTask(0x16);
    pBattle->setDuelPlayStock(m_pCardsS, m_iStockSkill);
    for (INT i = 0; i < m_iStock; i++) {
        m_pCardsS[i] = NULL;
        m_pDataS[i] = NULL;
    }
    m_iStockSum = 0;
    m_uiFlag = (INT)m_uiFlag & ~0x200;
    m_iStockSkill = 0;
    m_iExtraSkill = 0;
    m_iFirstSkill = 0;
    m_iStock = 0;
    return 1;
}
void CCardRevolver::setSnatch(HANDLE inHandle) {
    m_hSnatch = inHandle;
    m_nSnatchCount++;
    if (!(m_uiFlag & 0x1000)) {
        m_tqSnatch = 0;
        if (m_iMode > 0) changeMode(0, 1);
        else m_pEmpty->enableDisp(FALSE);
    }
    if (m_tqSnatch > 0) m_tqSnatch = 100;
    m_uiFlag |= 0x1000;
}
INT CCardRevolver::exeSnatchCard() {
    if (!(m_uiFlag & 0x1000)) return 0;
    if (!(m_uiFlag & 0x20000)) return 0;
    TIMEQ tq = getFrameRate();
    m_tqSnatch -= tq;
    if (m_tqSnatch > 0) {
        m_tqTimer -= tq;
        if (m_tqTimer <= 0) {
            m_tqTimer = 200;
            rollDeck(-1, 0, 200);
        }
        return 0;
    }
    if (m_nSnatchCount > 0) {
        snatchCard();
        m_nSnatchCount--;
        return 0;
    }
    m_tqSnatch = 0;
    m_hSnatch = 0;
    m_uiFlag = (INT)m_uiFlag & ~0x1000;
    CPlayer* pPlayer = _ref(g_pPlayer);
    if (pPlayer) pPlayer->setCardSnatch(0);
    return 1;
}
void CCardRevolver::snatchCard() {
    if (getHave(1, 1, 0, 0, 0) < 7) return;
    m_tqSnatch = 1000;
    if (m_iMode > 0) changeMode(0, 1);
    else m_pEmpty->enableDisp(FALSE);
    for (INT i = 0; i < m_nLimit; i++) {
        CBattleCard* pCard = &m_pCards[i];
        if (!pCard) continue;
        if (pCard->getCardKind() != 0x12) continue;
        if (pCard->isNowPlaying()) continue;
        INT stock = pCard->getStock();
        if (stock != -1) continue;
        if (pCard->isReloadNG()) continue;
        if (pCard->getCardOwner() != 1) continue;
        INT hand = pCard->getHand();
        pCard->setSnatch(m_hSnatch);
        if (hand != stock) m_nRemain[m_iMode]--;
        break;
    }
    m_tqTimer = 200;
    rollDeck(-1, 1, 200);
}
INT CCardRevolver::returnSnatch(INT inNum) {
    return returnSnatchCard(inNum);
}
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Game/CardRevolver", returnSnatchCard__Q210CardBattle13CCardRevolveri);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Game/CardRevolver", exeReturnCard__Q210CardBattle13CCardRevolver);
INT CCardRevolver::canSnatch() {
    if (getHave(1, 1, 0, 0, 0) > 6) return TRUE;
    return FALSE;
}
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Game/CardRevolver", setPlayerPronounceCard__Q210CardBattle13CCardRevolveri);
void CCardRevolver::setDiscard() {
    if (isDiscard()) return;
    discard();
    m_uiFlag |= 0x8000;
}
INT CCardRevolver::exeDiscard() {
    if (!isDiscard()) return 0;
    m_tqDiscard -= getFrameRate();
    if (m_tqDiscard > 0) return 0;
    m_tqDiscard = 0;
    m_uiFlag &= ~0x8000;
    return 1;
}
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Game/CardRevolver", discard__Q210CardBattle13CCardRevolver);
void CCardRevolver::resetCardOwner(INT inOld, INT inNew) {
    for (INT i = 0; i < m_nLimit; i++) {
        CBattleCard* pCard = &m_pCards[i];
        if (!pCard) continue;
        if (pCard->getCardOwner() != inOld) continue;
        if (pCard->getCardCategory() == 8 && inNew == 0) continue;
        pCard->setCardOwner(inNew);
    }
}
INT CCardRevolver::calcPlutoMark() {
    m_nPlutoMark = 0;
    for (INT i = 0; i < m_nReloadID; i++) {
        CBattleCard* pCard = &m_pCards[i];
        if (!pCard) continue;
        BOOL bOn = FALSE;
        if (pCard->getCardOwner() == 1 && pCard->isReloadNG() && pCard->getHand() == -1 && !pCard->isNowPlaying() &&
            pCard->getStock() == -1 && pCard->getCardKind() != 0) {
            INT cat = pCard->getCardCategory();
            if (cat < 5 && cat != 0) {
                bOn = TRUE;
                pCard->setPlutoON();
                m_nPlutoMark++;
            }
        }
        if (!bOn) pCard->resetPlutoON();
    }
    return m_nPlutoMark;
}
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Game/CardRevolver", dropPlutoMarkCard__Q210CardBattle13CCardRevolverPC7FVECTOR);
void CCardRevolver::setPlayNG(INT inFlag, INT inCategory) {
    INT i;
    if (inFlag) {
        for (i = 0; i < m_nReloadID; i++) {
            CBattleCard* pCard = &m_pCards[i];
            if (!pCard) continue;
            if (!inCategory || pCard->getCardCategory() == inCategory) pCard->setPlayNG();
            else pCard->resetPlayNG();
        }
    } else {
        for (i = 0; i < m_nReloadID; i++) {
            CBattleCard* pCard = &m_pCards[i];
            if (!pCard) continue;
            if (inCategory && pCard->getCardCategory() != inCategory) continue;
            pCard->resetPlayNG();
        }
    }
}
CEnchantGauge::CEnchantGauge(CTask* pParent, INT inUser, CARD& inCard) : CTask(pParent, 0x4000) {
    m_uiUser = inUser;
    m_cardData = inCard;
    m_pSeqCtrls = NULL;
    m_pFntName = NULL;
    _initialize();
}
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Game/CardRevolver", _$_Q210CardBattle13CEnchantGauge);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Game/CardRevolver", _initialize__Q210CardBattle13CEnchantGauge);
void CEnchantGauge::setText() {
    if (!m_pSeqCtrls) return;
    if (!m_pFntName) return;
    if (m_cardData.m_nKind < 0x43) return;
    m_pFntName->setText(g_szEnemyCardEffName[m_cardData.m_nKind - 0x43]);
}
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Game/CardRevolver", main__Q210CardBattle13CEnchantGauge);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Game/CardRevolver", changeCount__Q210CardBattle13CEnchantGaugei);
void CEnchantGauge::destroy() {
    m_nNext = 0;
    CCardBattle* pBattle = _ref(CCardBattle::m_pInstance);
    UINT uiEnd = pBattle ? pBattle->m_uiFlag & 4 : 0;
    if (uiEnd) {
        out();
        return;
    }
    if (m_uiState == 3 || m_uiState == 4) return;
    m_uiState = 4;
    m_pSeqCtrls[0].replaceSeq(0, _ref(g_szSeqCockpit), 0x38);
    m_pSeqCtrls[0].enableDisp(1, FALSE);
    m_pSeqCtrls[2].enableDisp(0, FALSE);
    m_pSeqCtrls[1].enableDisp(0, FALSE);
}
CStockFolder::CStockFolder(CTask* pParent, INT inUser) : CTask(pParent, 0x4000) {
    m_uiUser = inUser;
    m_hOwner = pParent->getHandle();
    _init();
}
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Game/CardRevolver", _$_Q210CardBattle12CStockFolder);
void CStockFolder::_init() {
    m_iStock = 0;
    m_iStockSum = 0;
    m_iStockSkill = 0;
    m_iExtraSkill = 0;
    m_iFirstSkill = 0;
    for (INT i = 0; i < 7; i++) {
        m_pCards[i] = NULL;
        m_pData[i] = NULL;
    }
    initGauge();
}
void CStockFolder::main() {
    exeGauge();
}
void CStockFolder::destroy() {
    for (INT i = 0; i < m_iStock; i++) {
        m_pCards[i] = NULL;
        m_pData[i] = NULL;
    }
    m_iStock = 0;
    m_iStockSum = 0;
    m_iStockSkill = 0;
    m_iExtraSkill = 0;
    m_iFirstSkill = 0;
    m_uiState = 5;
    m_pSeqCtrl->enableDisp(FALSE);
    m_uiFlag = (INT)m_uiFlag & ~1;
    m_uiCategory = 0;
    m_pSeqCtrl_Num01->enableDisp(0, FALSE);
    m_pSeqCtrl_Num10->enableDisp(0, FALSE);
}
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Game/CardRevolver", initGauge__Q210CardBattle12CStockFolder);
void CStockFolder::initName(D2::C2DSeqCtrl* inSeq) {
    G_RGBA rgba;
    rgba.col = 0x80808080;
    m_p2DFntStockName = new (com_malloc_d(sizeof(D2::C2DFnt), D_0047B678, RET_ADDR()))
        D2::C2DFnt(0, 0, &rgba, 14, g_szStockSkillName[m_iStockSkill], 0x80);
    inSeq->addFntShift(g_szSeqCockpit, m_p2DFntStockName, -4.0f, 0.0f);
    inSeq->m_seqControl.fgColor = 1;
    inSeq->m_seqControl.fgFntScale = 1;
}
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Game/CardRevolver", exeGauge__Q210CardBattle12CStockFolder);
void CStockFolder::deleteGauge() {
    delete m_pSeqCtrl;
    m_pSeqCtrl = NULL;
    delete m_pSeqCtrl_Num01;
    m_pSeqCtrl_Num01 = NULL;
    delete m_pSeqCtrl_Num10;
    m_pSeqCtrl_Num10 = NULL;
}
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Game/CardRevolver", setStockInfo__Q210CardBattle12CStockFolder);
INT CStockFolder::setStockCard(INT inKind, INT inNumber) {
    if (CGameWork::getFlag(1, 10)) return 0;
    if (m_uiFlag & 2) return 0;
    CCardBattle* pBattle = (CCardBattle*)CTaskMan::getSysTask(0x16);
    if (!pBattle) return 0;
    if (pBattle->isPlayingCard(m_uiUser)) return 0;
    if (m_iStock == 3) return 0;
    m_pCards[m_iStock] = new (com_malloc_d(sizeof(CBattleCard), D_0047B710, RET_ADDR())) CBattleCard(this, inKind, inNumber, m_uiUser, 0);
    CBattleCard* pCard = m_pCards[m_iStock];
    pCard->setFolderON();
    pCard->setRoomEffect(NULL);
    m_pData[m_iStock] = pCard->getCardData();
    pCard->setStock(m_iStock);
    m_iStock++;
    CSound::playSE(15, -1, 64);
    return 1;
}
INT CStockFolder::calcEnemyStockSkill() {
    UINT uiKinds = 0;
    INT nSum = 0;
    m_iStockSkill = m_iStock;
    for (INT i = 0; i < m_iStock; i++) {
        uiKinds |= m_pData[i]->m_nKind << (i * 8);
        nSum += m_pData[i]->m_nNumber;
    }
    if (uiKinds == 0x989898) {
        if (nSum == 12) m_iStockSkill = 0x84;
        else if (nSum == 15) m_iStockSkill = 0x85;
        else if (nSum == 18) m_iStockSkill = 0x86;
        else if (nSum == 21) m_iStockSkill = 0x87;
        else if (nSum == 24) m_iStockSkill = 0x88;
        else m_iStockSkill = 0x6E;
    } else {
        m_iStockSkill = 0x6E;
    }
    m_iStockSum = nSum;
    if (m_iStock < 3) {
        m_iStockSkill = 0x6E;
        setStockInfo();
        m_iStockSkill = m_iStock;
    } else {
        setStockInfo();
    }
    return m_iStockSkill;
}
INT CStockFolder::returnStockCard() {
    if (m_iStock == 0) return 0;
    for (INT i = 0; i < m_iStock; i++) {
        if (m_pCards[i]) m_pCards[i]->destroy(1);
        m_pCards[i] = NULL;
        m_pData[i] = NULL;
    }
    m_iStock = 0;
    m_iStockSum = 0;
    m_iStockSkill = 0;
    m_iExtraSkill = 0;
    m_iFirstSkill = 0;
    return 1;
}
INT CStockFolder::getStock(INT inFlag) {
    if (!inFlag) return m_iStock;
    INT n = 0;
    for (INT i = 0; i < 7; i++) {
        if (!m_pCards[i]) break;
        if (!m_pCards[i]->isNowStock()) break;
        n++;
    }
    return n;
}
INT CStockFolder::setPlayStock(INT inFlag, INT inTest) {
    if (CGameWork::getFlag(1, 0x2f)) return 0;
    UINT uiEnd = CCardBattle::m_pInstance ? CCardBattle::m_pInstance->m_uiFlag & 0xC0000000 : 0;
    if (uiEnd) return 0;
    if (m_iStock <= 0) return 0;
    if (m_uiUser == 2 && getStock(1) != 3) return 0;
    CCardBattle* pBattle = (CCardBattle*)CTaskMan::getSysTask(0x16);
    if (pBattle->isPlayingGimmickCard()) return 0;
    if (inFlag) pBattle->resetNowPlaying(m_uiUser);
    INT ret = pBattle->setPlayStock(m_hOwner, m_uiUser, m_pCards, m_iStockSkill, m_iExtraSkill, m_iFirstSkill, inTest);
    if (!ret) return 0;
    for (INT i = 0; i < m_iStock; i++) {
        m_pCards[i] = NULL;
        m_pData[i] = NULL;
    }
    m_iStock = 0;
    m_iStockSum = 0;
    m_iStockSkill = 0;
    m_iExtraSkill = 0;
    m_iFirstSkill = 0;
    return ret;
}
INT CStockFolder::setPronounceCard(INT inKind, INT inNumber) {
    if (m_uiState >= 6) {
        CCardBattle* pBattle = (CCardBattle*)CTaskMan::getSysTask(0x16);
        if (!pBattle) return 0;
        if (!_isEqual(pBattle->m_uiDuelState, 11) && m_iStock != 6) goto ok;
    }
    return 0;
ok:
    m_pCards[m_iStock] = new (com_malloc_d(sizeof(CBattleCard), D_0047B740, RET_ADDR())) CBattleCard(this, inKind, inNumber, m_uiUser, 0);
    CBattleCard* pCard = m_pCards[m_iStock];
    pCard->setRoomEffect(NULL);
    m_pData[m_iStock] = pCard->getCardData();
    pCard->setPronounce(m_iStock);
    m_iStock++;
    return 1;
}
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Game/CardRevolver", setPlayPronounce__Q210CardBattle12CStockFolder);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Game/CardRevolver", getRollSpeed__CQ210CardBattle13CCardRevolver);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Game/CardRevolver", isActive__CQ210CardBattle13CCardRevolver);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Game/CardRevolver", setChargeRate__Q210CardBattle13CCardRevolveri);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Game/CardRevolver", setReloadNG__Q210CardBattle13CCardRevolver);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Game/CardRevolver", getMode__CQ210CardBattle13CCardRevolver);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Game/CardRevolver", isNowAddCount__CQ210CardBattle13CCardRevolver);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Game/CardRevolver", getReloadID__CQ210CardBattle13CCardRevolver);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Game/CardRevolver", getStockSkill__CQ210CardBattle13CCardRevolver);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Game/CardRevolver", changeOwner__Q210CardBattle13CCardRevolveri);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Game/CardRevolver", isDiscard__Q210CardBattle13CCardRevolver);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Game/CardRevolver", isEnableCount__Q210CardBattle13CEnchantGauge);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Game/CardRevolver", getStockSkill__CQ210CardBattle12CStockFolder);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Game/CardRevolver", getStockSum__CQ210CardBattle12CStockFolder);
