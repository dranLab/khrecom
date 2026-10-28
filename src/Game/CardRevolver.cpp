#define GAME_CARDREVOLVER_CPP
#include "khtypes.h"
#include "Game/CardRevolver.h"
#include "System/TaskMan.h"
#include "System/Sound/SoundMan.h"
#include "Game/GameWork.h"
#include "Game/Player/Player.h"
#include "System/Debug.h"
#include "System/HeapMan.h"
#include "Game/Debug/DebugWork.h"
#include "include_asm.h"

using namespace CardBattle;

inline void* operator new(unsigned int, void* p) throw() { return p; }
#define RET_ADDR() ({ UINT nRA; __asm__ volatile("daddu %0, $31, $0" : "=r"(nRA)); nRA; })

extern LPCSTR g_szSeqCockpit __asm__("_10CardBattle$g_szSeqCockpit_0039B2E4");
template <class T> static inline T& _ref(T& v) { return v; }
extern "C" char D_00479D40[]; /* "Game/CardBattle.h" */
extern "C" char D_00479D68[]; /* "inUser" */
/* PARTIAL: stand-in for Game/CardBattle CCardBattle (m_cardEnchant, getEnchantKind): gated by GAME_CARDBATTLE_CPP */
struct CB_ENCHANT_VIEW {
    BYTE _pad[0x17C];
    CARD m_cardEnchant[2];
};
/* CCardBattle::getEnchantKind(INT inUser), an inline in Game/CardBattle.h */
inline INT CCardRevolver::_getEnchantKind(CCardBattle* p, INT inUser) {
    if (!inUser) AssertApp(D_00479D40, 641, D_00479D68, 0);
    return ((CB_ENCHANT_VIEW*)p)->m_cardEnchant[inUser - 1].m_nKind < 0x44 ? 0 : ((CB_ENCHANT_VIEW*)p)->m_cardEnchant[inUser - 1].m_nKind - 0x43;
}
static inline BOOL _isEqual(UINT a, UINT b) { return a == b; }
namespace CardBattle {
extern char* g_szStockSkillName[150];
extern LPCSTR g_szLadCockpit;
}
namespace D2 {
class C2DMan { /* PARTIAL: stand-in for System/Graphic/2d/2DMan C2DMan: gated by SYSTEM_GRAPHIC_2D_2DMAN_CPP */
public:
    static BOOL getLayPosition(const CHAR* pLadName, UINT inNo, UINT inIdx, FVECTOR& outPos, BOOL bLocal);
};
}
extern INT SCREEN_HEIGHT;
extern "C" char D_0047AAC0[];
extern "C" char D_0047B3A8[];
extern "C" char D_0047B3D8[];
extern "C" char D_0047B408[];
extern "C" char D_0047B5E8[];
extern "C" char D_0047B618[];
extern "C" char D_0047B648[];
extern "C" char D_0047B438[];
extern "C" char D_0047B678[];
extern "C" char D_0047B710[];
extern "C" char D_0047B740[];
extern "C" char D_0047B348[];
extern "C" char D_0047B378[];
extern "C" char D_0047AE78[];
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
void CCardRevolver::_initCounter() {
    initCounter();
    m_pEmpty = new (com_malloc_d(sizeof(C2DCard), D_0047AAC0, RET_ADDR())) C2DCard;
    m_pEmpty->setLayer(2);
    m_pEmpty->createEmpty();
    FVECTOR vPos;
    if (m_uiUser == 1) vPos.set(64.0f, (FLOAT)SCREEN_HEIGHT - 96.0f, 0.0f, 1.0f);
    else vPos.set(448.0f, (FLOAT)SCREEN_HEIGHT - 96.0f, 0.0f, 1.0f);
    m_pEmpty->setPos(vPos);
    m_pEmpty->enableDisp(FALSE);
}
/* Parked: 2 lines off: in the first loop's preheader the original hoists lui 0x80000000 (setReloadSet) before the
   index init (move s4,zero), ours after it. Worked: (INT) casts on both m_uiFlag ANDs, getCardData()->m_nCategory1,
   m_nReloadCount before m_nActive[0], m_iMax++ before setReloadSet. Tried: setReloadSet first, i/card at function
   or block scope, pointer-increment loop; permuter 6 min (7603 candidates, no gain). */
#if 0
void CCardRevolver::exeWait() {
    UINT flag = (m_uiUser == 1) ? 2 : 4;
    if (!(m_uiFlag & 0x40)) {
        INT i;
        CBattleCard* card;
        m_bActive = FALSE;
        m_tqTimer = 0;
        m_nRemain[0] = 0;
        m_iMax = 0;
        m_nReloadCount = 0;
        m_nActive[0] = 0;
        for (i = 0; i < m_nLimit; i++) {
            card = &m_pCards[i];
            if (!card) continue;
            if (card->isNowPlaying()) continue;
            if (card->isDuelStock()) continue;
            card->resetReloadSet();
            if (card->getHand() >= 0) {
                card->hideHand();
                card->outHand();
            }
            if (card->getHand() != -1) continue;
            if (card->getStock() != -1) continue;
            if (card->isReloadNG()) continue;
            if ((m_uiFlag & 0x100) && card->isDModeNG()) continue;
            m_iMax++;
            card->setReloadSet();
        }
        m_tqTimer = 400;
        m_uiFlag |= 0x40;
    }
    if (CGameWork::getInt(1, 0xE2) & flag) return;
    if (m_uiFlag & 0x40) {
        m_iState = 2;
        m_uiFlag = (INT)m_uiFlag & ~0x40;
        m_uiFlag = (INT)m_uiFlag & ~8;
        m_hPlaySE = CSound::playSE(m_iReloadSEID, -1, 0x40);
        INT j = m_nLimit - 1;
        for (INT i = 0; i < m_nLimit; i++, j--) {
            CBattleCard* card = &m_pCards[j];
            if (!card) continue;
            if (card->isReloadSet()) {
                card->resetReloadSet();
                m_nActive[0] = j;
                if (card->getCardData()->m_nCategory1 != 8) m_nRemain[0]++;
                card->resetHand(m_iMax - m_nRemain[0] - 1, m_iMax);
                m_nReloadCount++;
            } else {
                card->setStartIn();
            }
        }
    } else {
        m_iState = 1;
    }
}
#else
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Game/CardRevolver", exeWait__Q210CardBattle13CCardRevolver);
#endif
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Game/CardRevolver", main__Q210CardBattle13CCardRevolver);
void CCardRevolver::exeMain() {
    if (DebugWork::g_dw.player.iDeckDbgDisp) {
        if (!(m_uiFlag & 0x10)) rollDeck(0, 0, 300);
        m_uiFlag |= 0x10;
    } else {
        if (m_uiFlag & 0x10) rollDeck(0, 0, 300);
        m_uiFlag = (INT)m_uiFlag & ~0x10;
    }
    INT mode = m_iMode;
    INT nActive = m_nActive[mode];
    INT nRemain = m_nRemain[mode];
    CBattleCard* pCards;
    if (mode == 0) {
        nRemain += m_nReloadCard;
        pCards = m_pCards;
    } else {
        pCards = m_pCardsE;
    }
    if (pCards) {
        if (nRemain > 0) m_bActive = pCards[nActive].isNowSelect();
        else m_bActive = TRUE;
    } else {
        changeMode(0, 1);
        return;
    }
    if (m_uiUser == 1) {
        if (m_bActive) {
            CBattleCard* pCard = &pCards[nActive];
            pCard->setSelect(1);
            pCard->setEnchantEffect(0x52, 1);
            if (m_pActiveCard && m_pActiveCard != pCard) {
                m_pActiveCard->setSelect(0);
                m_pActiveCard->resetEnchantEffect(0x52);
            }
            m_pActiveCard = &pCards[nActive];
        } else {
            if (m_pActiveCard) {
                m_pActiveCard->setSelect(0);
                m_pActiveCard->resetEnchantEffect(0x52);
            }
            m_pActiveCard = NULL;
        }
    } else {
        if (m_bActive) {
            CBattleCard* pCard = &pCards[nActive];
            pCard->setEnchantEffect(0x52, 1);
            if (m_pActiveCard && m_pActiveCard != pCard) m_pActiveCard->resetEnchantEffect(0x52);
            m_pActiveCard = &pCards[nActive];
        } else {
            if (m_pActiveCard) m_pActiveCard->resetEnchantEffect(0x52);
            m_pActiveCard = NULL;
        }
    }
    if (!m_bActive) return;
    if (m_nReloadCard <= 0) return;
    if (DebugWork::g_dw.player.iAutoReload && m_iMode == 0 && nRemain == m_nReloadCard && m_iStock == 0) {
        initReload(0);
        return;
    }
    CCardBattle* pBattle = (CCardBattle*)CTaskMan::getSysTask(0x16);
    if (_getEnchantKind(pBattle, m_uiUser) != 0x11) return;
    if (pBattle->isExecutingCard(m_uiUser)) return;
    if (m_iMode != 0) return;
    if (nRemain != m_nReloadCard) return;
    initReload(0);
}
void CCardRevolver::destroy() {
    initHide();
}
void CCardRevolver::initHide() {
    if (m_iState == 4) return;
    m_iState = 4;
    CBattleCard* pCards = m_iMode == 0 ? m_pCards : m_pCardsE;
    INT n = m_iMode == 0 ? m_nLimit : m_nTotal[m_iMode];
    INT i;
    for (i = 0; i < n; i++) {
        CBattleCard* pCard = &pCards[i];
        if (!pCard) continue;
        if (pCard->getHand() != -1) pCard->hideHand();
        if (pCard->getStock() != -1) pCard->destroy(1);
    }
    for (i = 0; i < m_iStock; i++) {
        m_pCardsS[i] = NULL;
        m_pDataS[i] = NULL;
    }
    m_iStock = 0;
    m_iStockSum = 0;
    m_iStockSkill = 0;
    m_iExtraSkill = 0;
    m_iFirstSkill = 0;
    m_uiStockState = 0;
    m_uiStockCategory = 0;
    m_pSeqCtrlStock->enableDisp(FALSE);
    m_uiStockFlag = (INT)m_uiStockFlag & ~1;
    m_pSeqCtrlStock_Num01->enableDisp(0, FALSE);
    m_pSeqCtrlStock_Num10->enableDisp(0, FALSE);
    destroyCounter();
    if (m_pEmpty) m_pEmpty->enableDisp(FALSE);
    if (m_hPlaySE > 0) {
        CSound::stopSE(m_hPlaySE, m_iReloadSEID);
        m_hPlaySE = 0;
    }
    if (m_hChargeSE > 0) {
        CSound::stopSE(m_hChargeSE, 0x13);
        m_hChargeSE = 0;
    }
}
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
INT CCardRevolver::chargeReloadCard(INT inMode) {
    if (CGameWork::getFlag(1, 8)) return -1;
    if (m_uiFlag & 0x10000) return -1;
    if (!isEnableCharge() && !inMode) return -1;
    if (m_uiFlag & 0x80) {
        if (!inMode) CSound::playSE(0x15, -1, 0x40);
        initReload(inMode);
        return 1;
    }
    m_tqCharge += getFrameRate() * m_tqChargeRate / 100;
    CBattleCard* card = getActiveCard();
    INT charge = m_tqCharge / 30;
    if (charge - m_iCharge * 100 >= 100) {
        if (m_iCharge > 0) {
            m_iCharge--;
            m_tqCharge = m_iCharge * 3000;
            charge = m_iCharge * 100;
            CSound::playSE(0x14, -1, 0x40);
        } else {
            CCardBattle* pCB = (CCardBattle*)CTaskMan::getSysTask(0x16);
            INT kind = _getEnchantKind(pCB, m_uiUser);
            if (kind != 0x18) {
                if (m_iChargeCount < 3) m_iChargeCount++;
                if (kind == 0x1E) m_iChargeCount = 1;
            }
            m_iCharge = m_iChargeCount - 1;
            m_tqCharge = m_iCharge * 3000;
            card->updateReload(m_iCharge * 100);
            CSound::playSE(0x15, -1, 0x40);
            if (kind == 0xD) {
                setAction(0x50, 0);
                return 1;
            }
            initReload(0);
            return 1;
        }
    }
    card->updateReload(charge);
    m_uiFlag |= 0x20;
    if (!m_hChargeSE) m_hChargeSE = CSound::playSE(0x13, -1, 0x40);
    return 0;
}
void CCardRevolver::initReload(INT inMode) {
    TRACE(D_0047AE78, inMode);
    m_iState = 2;
    m_bActive = FALSE;
    m_tqTimer = 0;
    m_nRemain[0] = 0;
    m_iMax = 0;
    m_nActive[0] = 0;
    m_nReloadCount = 0;
    for (INT i = 0; i < m_nLimit; i++) {
        CBattleCard* pCard = &m_pCards[i];
        if (!pCard) continue;
        if (pCard->isNowPlaying()) continue;
        if (pCard->isDuelStock()) continue;
        pCard->resetReloadSet();
        if (pCard->getHand() >= 0) {
            pCard->hideHand();
            pCard->outHand();
        }
        if (pCard->getHand() != -1) continue;
        if (pCard->getStock() != -1) continue;
        if (pCard->isReloadNG()) continue;
        if ((m_uiFlag & 0x100) && pCard->isDModeNG()) continue;
        pCard->setReloadSet();
        m_iMax++;
    }
    m_tqTimer = 400;
    m_uiFlag = (INT)m_uiFlag & ~8;
    if (m_hPlaySE > 0) {
        CSound::stopSE(m_hPlaySE, m_iReloadSEID);
        m_hPlaySE = 0;
    }
    m_hPlaySE = CSound::playSE(m_iReloadSEID, -1, 64);
    if (inMode != 2) CCardBattle::decEnchantTime(m_uiUser, 0x29, 0);
    m_uiFlag &= ~0x20000;
}
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
INT CCardRevolver::calcEnemyStockSkill() {
    UINT uiKinds = 0;
    INT nSum = 0;
    m_iStockSkill = m_iStock;
    for (INT i = 0; i < m_iStock; i++) {
        uiKinds |= m_pDataS[i]->m_nKind << (i * 8);
        nSum += m_pDataS[i]->m_nNumber;
    }
    switch (uiKinds) {
    case 0x818181: m_iStockSkill = 0x6F; break;
    case 0x828282: m_iStockSkill = 0x70; break;
    case 0x838383: m_iStockSkill = 0x71; break;
    case 0x848484: m_iStockSkill = 0x72; break;
    case 0x878585: m_iStockSkill = 0x73; break;
    case 0x878686: m_iStockSkill = 0x74; break;
    case 0x898989: m_iStockSkill = 0x75; break;
    case 0x8A8A8A: m_iStockSkill = 0x76; break;
    case 0x8B8C8C: m_iStockSkill = 0x77; break;
    case 0x8D8D8C: m_iStockSkill = 0x78; break;
    case 0x8D8D8D: m_iStockSkill = 0x79; break;
    case 0x908E8E: m_iStockSkill = 0x7A; break;
    case 0x919191: m_iStockSkill = 0x7B; break;
    case 0x918F8F: m_iStockSkill = 0x7C; break;
    case 0x929392: m_iStockSkill = 0x7D; break;
    case 0x939393: m_iStockSkill = 0x7E; break;
    case 0x929393: m_iStockSkill = 0x7F; break;
    case 0x939392: m_iStockSkill = 0x80; break;
    case 0x959694: m_iStockSkill = 0x81; break;
    case 0x969496: m_iStockSkill = 0x82; break;
    case 0x959495: m_iStockSkill = 0x83; break;
    case 0x989898:
        if (nSum == 12) m_iStockSkill = 0x84;
        else if (nSum == 15) m_iStockSkill = 0x85;
        else if (nSum == 18) m_iStockSkill = 0x86;
        else if (nSum == 21) m_iStockSkill = 0x87;
        else if (nSum == 24) m_iStockSkill = 0x88;
        else m_iStockSkill = 0x6E;
        break;
    case 0x9B999A: m_iStockSkill = 0x89; break;
    case 0x999B9A: m_iStockSkill = 0x8A; break;
    case 0x9B9A99: m_iStockSkill = 0x8B; break;
    case 0x9C9D9D: m_iStockSkill = 0x8C; break;
    case 0x9C9C9C: m_iStockSkill = 0x8D; break;
    case 0x9D9C9D: m_iStockSkill = 0x8E; break;
    case 0x9E9E9E: m_iStockSkill = 0x8F; break;
    case 0x121212:
        if (nSum < 11) m_iStockSkill = 0x90;
        else m_iStockSkill = 0x91;
        break;
    default:
        m_iStockSkill = 0x6E;
        break;
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
void CCardRevolver::addCard(CARD* pData, INT inReturn) {
    INT i;
    if (m_uiUser == 1) {
        if (inReturn) {
            m_cardAdd[m_nReturn + 9] = *pData;
            m_nReturn++;
            m_uiFlag |= 0x4000;
            return;
        }
        switch ((INT)(pData->m_nId >> 28)) {
        case 6:
        case 14:
            if (CCardBattle::getGimmickCount() > 0) return;
            m_cardAdd[0] = *pData;
            CCardBattle::addGimmickCount(1);
            break;
        case 5:
            if (pData->m_nKind != 0x39) {
                i = CCardBattle::getFriendCount();
                if (i >= 5) return;
                m_cardAdd[i + 1] = *pData;
                CCardBattle::addFriendCount(1);
            } else {
                if (CCardBattle::getPlutoCount() >= 3) return;
                for (i = 6; i < 9; i++) {
                    CBattleCard* pCard = &m_pCards[m_nReloadID + 1 + i];
                    if (!pCard) continue;
                    if (!pCard->isReloadNG()) continue;
                    if (pCard->isNowPlaying()) continue;
                    m_cardAdd[i] = *pData;
                    CCardBattle::addPlutoCount(1);
                    break;
                }
            }
            break;
        }
        m_uiFlag |= 0x400;
    } else {
        if (!(m_uiFlag & 0x2000)) return;
        if (m_nAdd >= 24) return;
        m_cardAdd[m_nAdd] = *pData;
        m_nAdd++;
        m_uiFlag |= 0x400;
    }
}
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
void CCardRevolver::setNumberEffect(INT inKind) {
    FVECTOR vPos;
    switch (inKind) {
    case 0x44:
        m_pSeqCtrlNumEff->destroy();
        m_pSeqCtrlNumEff->setLayer(8);
        m_pSeqCtrlNumEff->addSeq(g_szSeqCockpit, 0x78);
        m_uiNumEffState = 1;
        D2::C2DMan::getLayPosition(_ref(g_szLadCockpit), 0x13, 0, vPos, FALSE);
        break;
    case 0x53:
        m_pSeqCtrlNumEff->destroy();
        m_pSeqCtrlNumEff->setLayer(8);
        m_pSeqCtrlNumEff->addSeq(g_szSeqCockpit, 0x76);
        m_uiNumEffState = 2;
        D2::C2DMan::getLayPosition(_ref(g_szLadCockpit), 0x12, 0, vPos, FALSE);
        break;
    default:
        return;
    }
    if (m_uiUser != 1) vPos.x += 320.0f;
    m_pSeqCtrlNumEff->setPos(vPos);
}
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
void CCardRevolver::initStock() {
    FVECTOR vPos;
    m_uiStockState = 0;
    m_uiStockFlag = 0;
    m_uiStockCategory = 0;
    m_pSeqCtrlStock = NULL;
    m_pSeqCtrlStock_Num01 = NULL;
    m_pSeqCtrlStock_Num10 = NULL;
    m_pSeqCtrlStock = new (com_malloc_d(sizeof(D2::C2DSeqCtrl), D_0047B3A8, RET_ADDR())) D2::C2DSeqCtrl;
    m_pSeqCtrlStock_Num10 = new (com_malloc_d(sizeof(D2::C2DSeqCtrl), D_0047B3D8, RET_ADDR())) D2::C2DSeqCtrl;
    m_pSeqCtrlStock_Num01 = new (com_malloc_d(sizeof(D2::C2DSeqCtrl), D_0047B408, RET_ADDR())) D2::C2DSeqCtrl;
    m_pSeqCtrlStock->setLayer(7);
    m_pSeqCtrlStock_Num01->setLayer(8);
    m_pSeqCtrlStock_Num10->setLayer(8);
    if (m_uiUser == 1) {
        m_pSeqCtrlStock->addSeq(g_szSeqCockpit, 0x1B);
        D2::C2DMan::getLayPosition(g_szLadCockpit, 4, 0, vPos, FALSE);
        FVECTOR vNum = vPos;
        m_pSeqCtrlStock_Num01->addSeq(g_szSeqCockpit, 0xE);
        vNum.x = 41.0f;
        vNum.y = -11.0f;
        m_pSeqCtrlStock_Num01->setPos(vNum);
        m_pSeqCtrlStock_Num01->enableDisp(0, FALSE);
        m_pSeqCtrlStock_Num10->addSeq(g_szSeqCockpit, 0xE);
        vNum.x = 26.0f;
        m_pSeqCtrlStock_Num10->setPos(vNum);
        m_pSeqCtrlStock_Num10->enableDisp(0, FALSE);
    } else {
        m_pSeqCtrlStock->addSeq(g_szSeqCockpit, 0x18);
        D2::C2DMan::getLayPosition(g_szLadCockpit, 3, 0, vPos, FALSE);
        FVECTOR vNum = vPos;
        m_pSeqCtrlStock_Num01->addSeq(g_szSeqCockpit, 0xE);
        vNum.x = -13.0f;
        vNum.y = -12.0f;
        m_pSeqCtrlStock_Num01->setPos(vNum);
        m_pSeqCtrlStock_Num01->enableDisp(0, FALSE);
        m_pSeqCtrlStock_Num10->addSeq(g_szSeqCockpit, 0xE);
        vNum.x = -28.0f;
        m_pSeqCtrlStock_Num10->setPos(vNum);
        m_pSeqCtrlStock_Num10->enableDisp(0, FALSE);
    }
    m_pSeqCtrlStock->setPos(vPos);
    initStockSkillName(m_pSeqCtrlStock);
    m_pSeqCtrlStock->enableDisp(FALSE);
}
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
void CStockFolder::initGauge() {
    FVECTOR vPos;
    m_uiState = 0;
    m_uiFlag = 0;
    m_uiCategory = 0;
    m_pSeqCtrl = NULL;
    m_pSeqCtrl_Num01 = NULL;
    m_pSeqCtrl_Num10 = NULL;
    m_nNumTexID = 0;
    m_pSeqCtrl = new (com_malloc_d(sizeof(D2::C2DSeqCtrl), D_0047B5E8, RET_ADDR())) D2::C2DSeqCtrl;
    m_pSeqCtrl_Num10 = new (com_malloc_d(sizeof(D2::C2DSeqCtrl), D_0047B618, RET_ADDR())) D2::C2DSeqCtrl;
    m_pSeqCtrl_Num01 = new (com_malloc_d(sizeof(D2::C2DSeqCtrl), D_0047B648, RET_ADDR())) D2::C2DSeqCtrl;
    m_pSeqCtrl->setLayer(7);
    m_pSeqCtrl_Num01->setLayer(8);
    m_pSeqCtrl_Num10->setLayer(8);
    m_pSeqCtrl->addSeq(g_szSeqCockpit, 0x18);
    D2::C2DMan::getLayPosition(g_szLadCockpit, 3, 0, vPos, FALSE);
    vPos.x += -8.0f;
    FVECTOR vNum = vPos;
    m_pSeqCtrl_Num01->addSeq(g_szSeqCockpit, 0xE);
    vNum.x = -13.0f;
    vNum.y = -12.0f;
    m_pSeqCtrl_Num01->setPos(vNum);
    m_pSeqCtrl_Num01->enableDisp(0, FALSE);
    m_pSeqCtrl_Num10->addSeq(g_szSeqCockpit, 0xE);
    vNum.x = -28.0f;
    m_pSeqCtrl_Num10->setPos(vNum);
    m_pSeqCtrl_Num10->enableDisp(0, FALSE);
    m_pSeqCtrl->setPos(vPos);
    initName(m_pSeqCtrl);
    m_pSeqCtrl->enableDisp(FALSE);
}
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
