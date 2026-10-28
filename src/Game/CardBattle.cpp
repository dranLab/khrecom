#include "khtypes.h"
#include "System/Task.h"
#include "Game/CardBattle.h"
#include "Game/BattleCard.h"
#include "Game/Entity.h"
#include "Game/Player/Player.h"
#include "Game/Player/PlayerManager.h"
#include "Game/Game.h"
#include "Game/SceneInfo.h"
#include "Game/Debug/DebugWork.h"
#include "Game/Enemy/BattleEnemy.h"
#include "Game/Enemy/BattleEnemyManager.h"
#include "Game/MessageManager.h"
#include "Game/GameWork.h"
#include "System/TaskMan.h"
#include "System/HeapMan.h"
#include "System/Graphic/2d/2DSeqCtrl.h"
#include "System/Sound/SoundMan.h"
#include "include_asm.h"

#define RET_ADDR() ({ UINT nRA; __asm__ volatile("daddu %0, $31, $0" : "=r"(nRA)); nRA; })
inline void* operator new[](size_t size, char* msg) throw() { return com_malloc_d(size, msg, RET_ADDR()); }
inline void* operator new(size_t, void* p) throw() { return p; }

using namespace CardBattle;

namespace CardBattle {
class CStockFolder {
    BYTE _m_0x00[0xb0];

public:
    CStockFolder(::CTask* pParent, INT inUser);
    void destroy();
    INT calcEnemyStockSkill();
    void setStockInfo();
    INT getStock(INT inFlag);
    void setPronounceCard(INT inKind, INT inNumber);
    INT setPlayPronounce();
};
class CEnchantGauge {
public:
    void changeCount(INT inCount);
    void destroy();
};
void calcFreezeDamage(INT& ioDamage);
INT getDropPlutoCard(CARD& outData);

class CDuelGauge : public ::CTask {
private:
    UINT m_uiState;        /* 0x40 */
    UINT m_uiTimerState;   /* 0x44 */
    UINT m_uiCountState;   /* 0x48 */
    UINT m_uiCount;        /* 0x4c */
    UINT m_uiCountMax;     /* 0x50 */
    UINT m_uiResult;       /* 0x54 */
    UINT m_uiFlag;         /* 0x58 */
    TIMEQ m_tqTimer;       /* 0x5c */
    TIMEQ m_tqLimit;       /* 0x60 */
    HANDLE m_hTimerSE;     /* 0x64 */
    D2::C2DSeqCtrl* m_pSeqCtrls; /* 0x68 */

public:
    CDuelGauge(::CTask* pParent);
    virtual ~CDuelGauge();
    virtual void main();

private:
    void _initialize();
    void setTimer(UINT inState);
    void exeTimer();
    void changeTimer(INT inTime);
    void setCount(UINT inState);
    void exeCount();

public:
    UINT getState() const { return m_uiState; }
    void setStart(TIMEQ inTime, UINT inCount);
    void setResult(UINT inResult);
    TIMEQ getTimer() const { return m_tqTimer; }
    void changeCount(UINT inCount, UINT inMax);
    void setDuelNG();
    void exeDuelNG();
};
}
/* the unit's own copy of the header static (data still from assembly) */
extern LPCSTR g_szSeqCockpit __asm__("_10CardBattle$g_szSeqCockpit_0039AF98");
extern "C" char D_00477690[]; /* "...CardBattle.cpp(4378)" */
extern "C" char D_00477338[]; /* "...CardBattle.cpp(NNNN)" */
extern "C" char D_004769E8[]; /* "...CardBattle.cpp(NNNN)" */
extern "C" char D_00477090[]; /* "inData" */
extern "C" char D_00477098[]; /* "...CardBattle.cpp(NNNN)" */
extern "C" char D_004772C0[]; /* "...CardBattle.cpp(NNNN)" */
extern "C" char D_004772F0[]; /* "...CardBattle.cpp(NNNN)" */
extern "C" char D_00477320[]; /* break effect name */
extern "C" char D_00477328[]; /* speed break effect name */
extern "C" char D_00476938[]; /* "m_pInstance" */
extern "C" char D_00477298[]; /* notifyCompleteGimmick's TRACE format */
namespace PL {
extern FLOAT g_fDefAttr[18][6];
}
/* the unit's copy of Game/CardKind's header static table (data still from assembly) */
extern const CARD_DATA cardDataTbl[224] __asm__("cardDataTbl_00478270");
/* CARD's inline constructor (Game/Card.cpp's _setCard) */
static inline void _setCard(CARD& c, USHORT nKind, BYTE nNumber) {
    c.m_nId = 0;
    c.m_nKind2 = nKind;
    c.m_nNumber2 = nNumber;
    c.m_nCategory20 = cardDataTbl[c.m_nKind].nCategory0;
    c.m_nCategory21 = cardDataTbl[c.m_nKind].nCategory1;
}
namespace D2 {
class C2DMan { /* PARTIAL: System/Graphic/2d/2DMan.h declares the classes only for its own unit */
public:
    static BOOL getLayPosition(const CHAR* pLadName, UINT inNo, UINT inIdx, FVECTOR& outPos, BOOL bLocal);
};
}
namespace CardBattle {
extern G_UV g_uvCRCounterNumTex[8][10];
extern INT g_DuelSeqIDX_TimerNumDmy[6][3];
extern G_UV g_uvDuelNumTex[4][10];
extern LPCSTR g_szLadDuel; /* the file's statics (data still from assembly) */
extern LPCSTR g_szSeqDuel;
}
extern "C" char D_00476980[]; /* __FILE__ */
extern "C" char D_00476730[];
extern "C" int abs(int);
template <class T> static inline T& _ref(T& v) { return v; }

void CardBattle::calcFreezeDamage(INT& ioDamage) {
    INT nPower = g_pStockSkillList[35].POWER[1];
    ioDamage += nPower / 10;
}
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Game/CardBattle", __Q210CardBattle11CCardBattle);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Game/CardBattle", _$_Q210CardBattle11CCardBattle);
CCardBattle* CCardBattle::getInstance() {
    if (!m_pInstance) {
        m_pInstance = new (com_malloc_d(sizeof(CCardBattle), D_004769E8, RET_ADDR())) CCardBattle;
        if (!(m_pInstance)) AssertApp(D_00476980, 163, D_00476938, 0);
    }
    return m_pInstance;
}
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Game/CardBattle", init__Q210CardBattle11CCardBattle);
void CCardBattle::main() {
    TIMEQ tqFrameRate = getFrameRate();
    UINT flag = 0x20000;
    if ((m_uiFlag & flag) && (CGameWork::getInt(0, 0x24) & 0x20)) {
        if (CGameWork::getFlag(1, 0x2A)) {
            if (m_uiDuelState == 1) m_uiDuelState = 0;
        } else {
            if (!m_uiDuelState) m_uiDuelState = 1;
        }
    }
    calcPlayingCard();
    exePlaying(1);
    exePlaying(2);
    exeRegeneration();
    exeBio();
    exeDuel();
    m_tqMainTimer += tqFrameRate;
    dispPlayingEnemy();
    dispCardBreak();
}
void CCardBattle::disp() {}
void CCardBattle::calcPlayingCard() {
    INT i;
    INT j;
    for (i = 0; i < 2; i++) {
        m_iCount[i] = 0;
        m_iPoint[i] = 0;
        for (j = 0; j < 7; j++) {
            if (m_cardData[i][j].m_nKind) {
                m_iCount[i]++;
                m_iPoint[i] += m_cardData[i][j].m_nNumber;
            }
            m_mesCardInfo[i].m_cardData[j] = m_cardData[i][j];
        }
        m_mesCardInfo[i].m_nStock = m_iStock[i];
        m_mesCardInfo[i].m_nExtra = m_iExtra[i];
        m_mesCardInfo[i].m_nFirst = m_iFirst[i];
    }
    UINT flag = 0x20000;
    if (m_uiFlag & flag) {
        if (m_iCount[1] > 0) {
            m_iPointBreak = calcPointBreak(m_iPoint[0], m_iPoint[1]);
        } else {
            m_iPointBreak = 0;
            m_iPointBonus = 0;
        }
        m_mesCardInfo[0].m_nBreak = 0;
        m_mesCardInfo[0].m_nBonus = 0;
    }
    m_uiFlag = (INT)m_uiFlag & ~0x300;
    m_uiFlag &= ~0x100000;
    for (i = 0; i < 2; i++) {
        switch (m_cardEnchant[i].m_nKind) {
        case 0x58:
            for (j = 0; j < m_iCount[i]; j++) {
                if (m_cardData[i][j].m_nCategory21 == 4) {
                    m_uiFlag |= 0x100 << i;
                    break;
                }
            }
            break;
        case 0x60:
            for (j = 0; j < m_iCount[i]; j++) {
                if (m_cardData[i][j].m_nKind == 0x1D) {
                    m_uiFlag |= 0x100 << i;
                    break;
                }
            }
            break;
        case 0x69:
            for (j = 0; j < m_iCount[i]; j++) {
                if (m_cardData[i][j].m_nCategory21 == 1) {
                    m_uiFlag |= 0x100 << i;
                    break;
                }
            }
            break;
        case 0x7B:
            if (i == 0) setFlag(0x100000, 0);
            break;
        }
        if (m_cardData[i][0].m_nCategory20 == 6) m_uiFlag |= 0x100 << i;
    }
}
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Game/CardBattle", exePlaying__Q210CardBattle11CCardBattlei);
void CCardBattle::setNumberBreak(UINT _PLAYER, UINT _ENEMY) {
    if (m_cardEnchant[_PLAYER].m_nKind == 0x65) {
        INT iCountE = m_iCount[_ENEMY];
        INT iPoint = m_iPoint[_PLAYER];
        for (INT i = 0; i < iCountE; i++) {
            CBattleCard* pCard = m_pCard[_ENEMY][i];
            if (pCard) {
                INT iPointE = pCard->getCardData()->m_nNumber;
                pCard->setNumberBreak(iPoint);
                iPoint -= iPointE;
                if (iPoint < 0) iPoint = 0;
                m_cardData[_ENEMY][i] = *m_pCard[_ENEMY][i]->getCardData();
            }
        }
    }
}
BOOL CCardBattle::checkNowPlaying(INT inSide, CBattleCard* pCard) {
    if (!(inSide)) AssertApp(D_00476980, 1425, D_00476730, 0);
    for (INT i = 0; i < 7; i++) {
        if (m_pCard[inSide - 1][i] == pCard) return TRUE;
    }
    return FALSE;
}
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Game/CardBattle", testPlayingCard__Q210CardBattle11CCardBattleUi);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Game/CardBattle", testBreakingCard__Q210CardBattle11CCardBattleUi);
INT CCardBattle::setPlayCard(HANDLE inHandle, INT inUser, CBattleCard* inCard, CARD* inData, INT inTest) {
    if (!(inUser)) AssertApp(D_00476980, 1660, D_00476730, 0);
    UINT _PLAYER = inUser - 1;
    if (m_uiState[_PLAYER]) return 0;
    m_hOwner[_PLAYER] = inHandle;
    m_uiState[_PLAYER] = 1;
    m_uiResult[_PLAYER] = 0;
    m_tqPlayTimer[_PLAYER] = 0;
    CBattleCard* pOld = m_pCard[_PLAYER][0];
    if (pOld && !pOld->getRevolver()) pOld->out();
    if (inCard) {
        m_pCard[_PLAYER][0] = inCard;
        inCard->setEnchantEffect(0x44, 0);
        m_pCard[_PLAYER][0]->setEnchantEffect(0x52, 0);
        m_pCard[_PLAYER][0]->setEnchantEffect(0x53, 0);
        m_cardData[_PLAYER][0] = *inCard->getCardData();
        checkNowPlaying(inUser, inCard);
    } else {
        if (!(inData)) AssertApp(D_00476980, 1708, D_00477090, 0);
        m_cardData[_PLAYER][0] = *inData;
        m_pCard[_PLAYER][0] = new (com_malloc_d(sizeof(CBattleCard), D_00477098, RET_ADDR()))
            CBattleCard(asTask(), inData->m_nKind, inData->m_nNumber, inUser, 0);
        if (m_pCard[_PLAYER][0]) m_pCard[_PLAYER][0]->setRoomEffect(NULL);
        m_cardData[_PLAYER][0] = *m_pCard[_PLAYER][0]->getCardData();
    }
    m_iStock[_PLAYER] = 0;
    m_iExtra[_PLAYER] = 0;
    m_iFirst[_PLAYER] = 0;
    m_pCard[_PLAYER][0]->setPlayCard();
    if (isNowDuel()) {
        m_uiState[_PLAYER] = 9;
        m_pCard[_PLAYER][0]->setFlagDuelPlay();
    }
    if (isNowPronounce()) {
        m_uiState[_PLAYER] = 14;
        m_pCard[_PLAYER][0]->setFlagPronouncePlay();
    }
    if (inTest) return testPlayingCard(_PLAYER);
    return 1;
}
/* Parked: induction variable - the original's copy loop runs on &m_pCard[_PLAYER][i] (cardData at -0x38), ours on
 * &m_cardData[_PLAYER][i] (m_pCard at +0x38); all else matches; tried: pointer/index forms of inCard (in-body and
 * for-header increments), the m_pCard store per branch, an m_pCard row pointer, call/copy order, permuter 4 min
 * (7957 candidates, no gain) */
#if 0
INT CCardBattle::setPlayStock(HANDLE inHandle, INT inUser, CBattleCard** inCard, INT inSkill, INT inExtra, INT inFirst,
                              INT inTest) {
    if (!(inUser)) AssertApp(D_00476980, 1754, D_00476730, 0);
    UINT _PLAYER = inUser - 1;
    if (m_uiState[_PLAYER]) return 0;
    m_hOwner[_PLAYER] = inHandle;
    m_uiState[_PLAYER] = 1;
    m_uiResult[_PLAYER] = 0;
    m_tqPlayTimer[_PLAYER] = 0;
    INT max = inSkill < 4 ? inSkill : 3;
    INT count = 0;
    INT i;
    for (i = 0; i < max; i++) {
        if (inCard[i]) count++;
    }
    for (i = 0; i < max; i++) {
        CBattleCard* pCard = *inCard;
        m_pCard[_PLAYER][i] = pCard;
        if (pCard) {
            m_cardData[_PLAYER][i] = *pCard->getCardData();
            pCard->setPlayStock(count);
        } else {
            _setCard(m_cardData[_PLAYER][i], 0, 0);
        }
        inCard++;
    }
    m_iStock[_PLAYER] = inSkill;
    m_iExtra[_PLAYER] = inExtra;
    m_iFirst[_PLAYER] = inFirst;
    if (!inTest) return 1;
    return testPlayingCard(_PLAYER);
}
#else
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Game/CardBattle", setPlayStock__Q210CardBattle11CCardBattleiiPPQ210CardBattle11CBattleCardiiii);
#endif
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Game/CardBattle", setEnchantCard__Q210CardBattle11CCardBattleiiPQ210CardBattle11CBattleCard);
/* Parked: address forms - the original computes &m_cardEnchant[_PLAYER] as (idx*4+0x17c)+base and loads the kind with
 * offset 0 (kept in two registers), and &m_nEnchantTime[_PLAYER] as (base+0x184)+idx*4; ours folds the offsets the
 * other way round; the reload of m_pInstance after the decrement matches via the INT* pTime pointer; tried: CARD view
 * struct / reference / pointer locals for the enchant, BYTE/INT/UINT kind, _ref forms, compare operand order,
 * permuter 4 min (8263 candidates, no gain) */
#if 0
void CCardBattle::decEnchantTime(INT inUser, INT inPart, INT inKind) {
    if (!(inUser)) AssertApp(D_00476980, 1977, D_00476730, 0);
    UINT _PLAYER = inUser - 1;
    INT kind = m_pInstance->m_cardEnchant[_PLAYER].m_nKind;
    if (!kind) return;
    if (inPart != g_pBattleCardList[kind].EFF_PART) return;
    if (inKind && inKind != kind - 0x43) return;
    CEnchantGauge* pGauge = m_pInstance->m_pEnchantGauge[_PLAYER];
    if (!pGauge) return;
    INT* pTime = &m_pInstance->m_nEnchantTime[_PLAYER];
    (*pTime)--;
    if (_ref(m_pInstance)->m_nEnchantTime[_PLAYER] <= 0) {
        pGauge->changeCount(0);
        m_pInstance->resetEnchantCard(inUser);
    } else {
        pGauge->changeCount(_ref(m_pInstance)->m_nEnchantTime[_PLAYER]);
    }
}
#else
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Game/CardBattle", decEnchantTime__Q210CardBattle11CCardBattleiii);
#endif
void CCardBattle::destroyPlayCard(INT inFlag, INT inP1) { resetNowPlaying(inFlag); }
void CCardBattle::resetNowPlaying(INT inUser) {
    if (!(inUser)) AssertApp(D_00476980, 2028, D_00476730, 0);
    UINT _PLAYER = inUser - 1;
    if (inUser == 1) {
        if (m_iStock[_PLAYER] > 0) {
            CGameWork::setFlag(1, 0x16, TRUE);
        } else {
            CGameWork::setFlag(1, 0x14, TRUE);
        }
        _setCard(m_cardGimmick, 0, 0);
        m_uiFlag &= ~0x8000;
    }
    if (m_iStock[_PLAYER] > 0) decEnchantTime(inUser, 0x2A, 0);
    m_uiResult[_PLAYER] = 0;
    m_uiState[_PLAYER] = 0;
    m_hOwner[_PLAYER] = 0;
    for (INT i = 0; i < 7; i++) {
        _setCard(m_cardData[_PLAYER][i], 0, 0);
        if (m_pCard[_PLAYER][i]) {
            m_pCard[_PLAYER][i]->setUseEnd();
            m_pCard[_PLAYER][i] = NULL;
        }
    }
    m_iStock[_PLAYER] = 0;
    m_iExtra[_PLAYER] = 0;
    m_iFirst[_PLAYER] = 0;
}
void CCardBattle::resetEnchantCard(INT inUser) {
    if (!(inUser)) AssertApp(D_00476980, 2069, D_00476730, 0);
    UINT _PLAYER = inUser - 1;
    if (!_PLAYER) {
        resetPlayerEnchant(m_cardEnchant[0].m_nKind);
        m_uiFlag &= ~0x80000;
        m_uiFlag = (INT)m_uiFlag & ~0x4000;
    }
    m_uiFlag &= ~(0x400 << _PLAYER);
    m_uiFlag &= ~(0x1000 << (_PLAYER ^ 1));
    _setCard(m_cardEnchant[_PLAYER], 0, 0);
    m_nEnchantTime[_PLAYER] = 0;
    if (m_pEnchantGauge[_PLAYER]) m_pEnchantGauge[_PLAYER]->destroy();
    m_pEnchantGauge[_PLAYER] = NULL;
    m_pEnchant[_PLAYER] = NULL;
}
HANDLE CCardBattle::getPlayingHandle(INT inSide) {
    if (!m_pInstance) return 0;
    if ((UINT)inSide < 2 && m_pInstance->m_uiState[0]) return m_pInstance->m_hOwner[0];
    if ((inSide == 0 || inSide == 2) && m_pInstance->m_uiState[1]) return m_pInstance->m_hOwner[1];
    return 0;
}
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Game/CardBattle", isStandingCard__Q210CardBattle11CCardBattlei);
BOOL CCardBattle::isPlayingCardStandNow(INT inSide) {
    if (!m_pInstance) return FALSE;
    if ((UINT)inSide < 2) {
        switch (m_pInstance->m_uiState[0]) {
        case 2:
        case 10:
        case 15:
            return TRUE;
        }
    }
    if (inSide == 0 || inSide == 2) {
        switch (m_pInstance->m_uiState[1]) {
        case 2:
        case 10:
        case 15:
            return TRUE;
        }
    }
    return FALSE;
}
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Game/CardBattle", isExecutingCard__Q210CardBattle11CCardBattlei);
BOOL CCardBattle::isPlayingCard(INT inSide) {
    if (!m_pInstance) return FALSE;
    if ((UINT)inSide < 2 && !m_pInstance->m_uiState[0]) return FALSE;
    if (inSide == 0 || inSide == 2) {
        if (!m_pInstance->m_uiState[1]) return FALSE;
    }
    return TRUE;
}
BOOL CCardBattle::isPlayingEnemyCard() {
    if (!m_pInstance) return FALSE;
    return m_pInstance->m_uiState[1] != 0;
}
INT CCardBattle::getPlayingCardPoint(INT inSide) {
    if (!m_pInstance) return -1;
    if ((UINT)inSide < 2 && m_pInstance->m_uiState[0]) return m_pInstance->m_iPoint[0];
    if ((inSide == 0 || inSide == 2) && m_pInstance->m_uiState[1]) return m_pInstance->m_iPoint[1];
    return -1;
}
INT CCardBattle::getPlayingCardID(INT inSide, INT inIdx) {
    if (!m_pInstance) return -1;
    if ((UINT)inSide < 2 && m_pInstance->m_uiState[0]) return m_pInstance->m_cardData[0][inIdx].m_nKind;
    if ((inSide == 0 || inSide == 2) && m_pInstance->m_uiState[1]) return m_pInstance->m_cardData[1][inIdx].m_nKind;
    return -1;
}
void CCardBattle::addGimmickCount(INT n) {
    if (m_pInstance) m_pInstance->m_iCountGimmick += n;
}
INT CCardBattle::getGimmickCount() {
    if (!m_pInstance) return -1;
    return m_pInstance->m_iCountGimmick;
}
void CCardBattle::addFriendCount(INT n) {
    if (m_pInstance) m_pInstance->m_iCountFriend += n;
}
INT CCardBattle::getFriendCount() {
    if (!m_pInstance) return -1;
    return m_pInstance->m_iCountFriend;
}
void CCardBattle::addPlutoCount(INT n) {
    if (m_pInstance) m_pInstance->m_iCountPluto += n;
}
INT CCardBattle::getPlutoCount() {
    if (!m_pInstance) return -1;
    return m_pInstance->m_iCountPluto;
}
void CCardBattle::setGimmickOwner(HANDLE h) {
    if (m_pInstance) m_pInstance->m_hGimmickOwner = h;
}
HANDLE CCardBattle::getGimmickOwner() {
    if (!m_pInstance) return 0;
    return m_pInstance->m_hGimmickOwner;
}
void CCardBattle::notifyMakeGimmick(CARD* inData, INT inFlag) {
    if (!m_pInstance) return;
    if (inData) m_pInstance->m_cardGimmick = *inData;
    _ref(m_pInstance)->m_uiFlag |= 0x8000;
    if (inFlag) return;
    CCardBattle* p = _ref(m_pInstance);
    if (!p->m_hGimmickOwner) return;
    MES_CARD_INFO mesInfo;
    mesInfo.m_nStock = 0;
    mesInfo.m_nExtra = 0;
    mesInfo.m_nFirst = 0;
    mesInfo.m_cardData[0] = p->m_cardGimmick;
    _setCard(mesInfo.m_cardData[1], 0, 0);
    _setCard(mesInfo.m_cardData[2], 0, 0);
    p->m_pMesage->sendMessage(0, p->getHandle(), p->m_hGimmickOwner, 6, &mesInfo);
}
void CCardBattle::notifyCompleteGimmick() {
    if (m_pInstance) {
        TRACE(D_00477298, m_pInstance->m_cardGimmick.m_nKind);
        _setCard(m_pInstance->m_cardGimmick, 0, 0);
        _ref(m_pInstance)->m_uiFlag &= ~0x8000;
    }
}
BOOL CCardBattle::isPlayingGimmickCard() {
    if (!m_pInstance) return FALSE;
    return m_pInstance->m_cardGimmick.m_nKind;
}
void CCardBattle::initPlayingEnemy(HANDLE inHandle) {
    m_uiFlag |= 1;
    m_pSeqCtrlEnemy->destroy();
    m_pSeqCtrlEnemy->setLayer(2);
    m_pSeqCtrlEnemy->addSeq(g_szSeqCockpit, 100);
    CB_LAYOUT* pLayout = &g_LayoutData[16];
    FVECTOR pos = {{pLayout->x, pLayout->y, 0.0f, 1.0f}};
    m_pSeqCtrlEnemy->setPos(pos);
}
void CCardBattle::dispPlayingEnemy() {
    UINT flag = 1;
    if (m_uiFlag & flag) {
        CEntity* pEntity;
        if (m_pSeqCtrlEnemy->isEnd(0) || CGameWork::getInt(1, 0xE2) || !m_hOwner[1] ||
            !(pEntity = (CEntity*)CTaskMan::getTask(m_hOwner[1]))) {
            m_uiFlag = (INT)m_uiFlag & ~1;
            m_pSeqCtrlEnemy->destroy();
        } else {
            FVECTOR pos;
            pEntity->getUseCardPos(pos);
            m_pSeqCtrlEnemy->set3DPos(pos);
        }
    }
}
/* Parked: registers/scheduling only (12 lines) - the m_pSeqCtrlBreak null test loads into v0 (ours a0, reused by the
 * else call), and in the speed branch the original stores m_hSpeed before loading m_pSeqCtrlSpeed for enableDisp
 * (jal delay slot); tried: _ref/== NULL test forms, m_hBreak store placement, permuter 4 min (379 candidates) */
#if 0
void CCardBattle::initCardBreak(HANDLE inHandle, UINT inUser) {
    m_vBreakPos.init();
    if (!inHandle) {
        m_hBreak = 0;
        if (!m_pSeqCtrlBreak) {
            m_pSeqCtrlBreak = new (com_malloc_d(sizeof(D2::C2DSeqCtrl), D_004772C0, RET_ADDR())) D2::C2DSeqCtrl(2);
            m_pSeqCtrlBreak->setLayer(7);
            m_pSeqCtrlBreak->addSeq(g_szSeqCockpit, 1);
            UINT flag = 0x20000;
            if (m_uiFlag & flag) {
                m_pSeqCtrlBreak->addSeq(g_szSeqCockpit, 2);
                m_pSeqCtrlBreak->enableColorCtrl(TRUE);
            }
            m_pSeqCtrlBreak->enableDisp(FALSE);
        } else {
            m_pSeqCtrlBreak->enableDisp(FALSE);
        }
        m_hSpeed = 0;
        if (!m_pSeqCtrlSpeed) {
            m_pSeqCtrlSpeed = new (com_malloc_d(sizeof(D2::C2DSeqCtrl), D_004772F0, RET_ADDR())) D2::C2DSeqCtrl(2);
            UINT flag = 0x20000;
            if (m_uiFlag & flag) {
                m_pSeqCtrlSpeed->setLayer(7);
                m_pSeqCtrlSpeed->addSeq(g_szSeqCockpit, 0x95);
                m_pSeqCtrlSpeed->addSeqShift(g_szSeqCockpit, 2, 60.0f, 0.0f);
                m_pSeqCtrlSpeed->addSeqShift(g_szSeqCockpit, 2, 80.0f, 0.0f);
                m_pSeqCtrlSpeed->enableColorCtrl(TRUE);
                m_pSeqCtrlSpeed->enableDisp(FALSE);
            }
        } else {
            m_pSeqCtrlSpeed->enableDisp(FALSE);
        }
        return;
    }
    CEntity* pEntity = (CEntity*)CTaskMan::getTask(inHandle);
    if (!pEntity) return;
    pEntity->getUseCardPos(m_vBreakPos);
    if (!m_iPointBonus || inUser == 1) {
        pEntity->setEffect(D_00477320, 0, &m_vBreakPos, NULL, 0.0f);
        pEntity->getHeadPos(m_vBreakPos);
        m_hBreak = inHandle;
        m_pSeqCtrlBreak->enableDisp(TRUE);
        m_pSeqCtrlBreak->changeTexture(1, 0, NULL, &g_uvCRCounterNumTex[0][m_iPointBreak],
                                       &g_uvCRCounterNumTex[1][m_iPointBreak]);
        m_pSeqCtrlBreak->restart(0, 0.0f);
    } else {
        pEntity->setEffect(D_00477328, 5, &m_vBreakPos, NULL, 0.0f);
        pEntity->getHeadPos(m_vBreakPos);
        m_hSpeed = inHandle;
        m_pSeqCtrlSpeed->enableDisp(TRUE);
        INT point = m_iPointBreak + m_iPointBonus;
        m_pSeqCtrlSpeed->changeTexture(1, 0, NULL, &g_uvCRCounterNumTex[0][point / 10],
                                       &g_uvCRCounterNumTex[1][point / 10]);
        m_pSeqCtrlSpeed->changeTexture(2, 0, NULL, &g_uvCRCounterNumTex[0][point % 10],
                                       &g_uvCRCounterNumTex[1][point % 10]);
        m_pSeqCtrlSpeed->restart(0, 0.0f);
        m_iPointBonus = 0;
    }
}
#else
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Game/CardBattle", initCardBreak__Q210CardBattle11CCardBattleiUi);
#endif
void CCardBattle::dispCardBreak() {
    if (m_hBreak > 0) {
        if (m_pSeqCtrlBreak->isEnd(0)) {
            m_pSeqCtrlBreak->enableDisp(FALSE);
            m_hBreak = 0;
        } else {
            m_pSeqCtrlBreak->enableDisp(TRUE);
            m_pSeqCtrlBreak->set3DPos(m_vBreakPos);
        }
    }
    if (m_hSpeed > 0) {
        if (m_pSeqCtrlSpeed->isEnd(0)) {
            m_pSeqCtrlSpeed->enableDisp(FALSE);
            m_hBreak = 0;
        } else {
            m_pSeqCtrlSpeed->enableDisp(TRUE);
            m_pSeqCtrlSpeed->set3DPos(m_vBreakPos);
        }
    }
}
INT CCardBattle::calcPointBreak(INT a, INT b) {
    INT d = abs(a - b);
    return d < 10 ? d : 9;
}
INT CCardBattle::setEnemyCard(HANDLE inEnemy, INT inKind, INT inNumber) {
    if (m_uiFlag & 0xC0000000) return 0;
    if (m_uiFlag & 0xC0000004) return 0;
    if (m_cardGimmick.m_nKind) return 0;
    if (m_uiDuelState >= 2) return 0;
    CARD card;
    _setCard(card, inKind, inNumber);
    INT result = setPlayCard(inEnemy, 2, NULL, &card, 0);
    if (result > 0) initPlayingEnemy(inEnemy);
    return result;
}
void CCardBattle::destroyEnemyCard(INT inHandle) {
    if (inHandle == m_hOwner[1]) destroyPlayCard(2, inHandle);
}
void CCardBattle::destroyRevolver(INT inSide) {
    if (!(inSide)) AssertApp(D_00476980, 2764, D_00476730, 0);
    UINT _PLAYER = inSide - 1;
    if (m_pRevolver[_PLAYER]) {
        m_pRevolver[_PLAYER]->destroy();
        m_pRevolver[_PLAYER] = NULL;
    }
}
void CCardBattle::setRevolver(INT inSide, CCardRevolver* pRevolver) {
    if (!(inSide)) AssertApp(D_00476980, 2774, D_00476730, 0);
    m_pRevolver[inSide - 1] = pRevolver;
}
CCardRevolver* CCardBattle::getRevolver(INT inSide) {
    if (!(inSide)) AssertApp(D_00476980, 2779, D_00476730, 0);
    return m_pRevolver[inSide - 1];
}
void CCardBattle::setBattleEnd() {
    if (m_pInstance) {
        _ref(m_pInstance->m_uiFlag) |= 0xC0000004;
        m_pInstance->initCardBreak(0, 0);
        m_pInstance->destroyPlayCard(1, 0);
        m_pInstance->resetEnchantCard(1);
        m_pInstance->destroyRevolver(1);
        m_pInstance->destroyPlayCard(2, 0);
        m_pInstance->resetEnchantCard(2);
        m_pInstance->destroyRevolver(2);
        m_pInstance->destroyStockFolder(1);
    }
}
BOOL CCardBattle::isBattleEnd() {
    if (!m_pInstance) return TRUE;
    return (m_pInstance->m_uiFlag & 0xc0000004) != 0;
}
void CCardBattle::setSuspend(BOOL b) {
    if (m_pInstance) {
        if (b) m_pInstance->m_uiFlag |= 0x10000;
        else m_pInstance->m_uiFlag &= ~0x10000;
    }
}
BOOL CCardBattle::isSuspend() {
    if (!m_pInstance) return FALSE;
    UINT uiMask = 0x10000;
    return (m_pInstance->m_uiFlag & uiMask) != 0;
}
BOOL CCardBattle::isLevitate() {
    if (!m_pInstance) return FALSE;
    UINT uiMask = 0x4000;
    return (m_pInstance->m_uiFlag & uiMask) != 0;
}
void CCardBattle::setBlack(BOOL b) {
    if (m_pInstance) {
        if (b) m_pInstance->m_uiFlag |= 0x40000;
        else m_pInstance->m_uiFlag &= ~0x40000;
    }
}
BOOL CCardBattle::isBlack() {
    if (!m_pInstance) return FALSE;
    UINT uiMask = 0x40000;
    return (m_pInstance->m_uiFlag & uiMask) != 0;
}
CStockFolder* CCardBattle::setStockFolder(::CTask* pParent, INT inUser) {
    if (!m_pInstance) return NULL;
    UINT _PLAYER = inUser - 1;
    if (!m_pInstance->m_pStockFolder[_PLAYER]) {
        m_pInstance->m_pStockFolder[_PLAYER] =
            new (com_malloc_d(sizeof(CStockFolder), D_00477338, RET_ADDR())) CStockFolder(pParent, inUser);
        _ref(m_pInstance)->m_hDuelEnemy = pParent->getHandle();
    }
    return m_pInstance->m_pStockFolder[_PLAYER];
}
void CCardBattle::destroyStockFolder(INT inSide) {
    if (!(inSide)) AssertApp(D_00476980, 2905, D_00476730, 0);
    UINT _PLAYER = inSide - 1;
    if (m_pStockFolder[_PLAYER]) {
        m_pStockFolder[_PLAYER]->destroy();
        m_pStockFolder[_PLAYER] = NULL;
    }
}
CStockFolder* CCardBattle::getStockFolder(INT inSide) {
    if (!m_pInstance) return NULL;
    return m_pInstance->m_pStockFolder[inSide - 1];
}
INT CCardBattle::getStockFolderStockSkill(INT inSide) {
    if (!m_pInstance) return 0;
    CStockFolder* p = m_pInstance->m_pStockFolder[inSide - 1];
    if (!p) return 0;
    return *(SHORT*)((BYTE*)p + 0x84);
}
INT CCardBattle::calcStockFolderEnemyStockSkill() {
    if (!m_pInstance) return 0;
    CStockFolder* p = m_pInstance->m_pStockFolder[1];
    if (!p) return 0;
    return p->calcEnemyStockSkill();
}
void CCardBattle::setStockFolderStockInfo(INT inSide) {
    if (m_pInstance) {
        CStockFolder* p = m_pInstance->m_pStockFolder[inSide - 1];
        if (p) p->setStockInfo();
    }
}
void ENCHANT_PARAM::init(INT inOwner, UINT inUser) {
    uiUser = inUser;
    hOwner = inOwner;
    AtkKind = (ATK_KIND)0;
    AtkAttr = (ATK_ATTR)0;
    uiState = 0;
    UpKind = 0;
    iAttackPoint = 0;
    iHp = 0;
    iHpMax = 0;
    iFinish = 0;
    iGuard = 0;
    iNormal = 0;
}
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Game/CardBattle", calcEnchantParam__Q210CardBattle11CCardBattleRQ210CardBattle13ENCHANT_PARAM);
void CCardBattle::calcEnchant_AttackPoint(INT& inPoint, ATK_KIND& inAtkKind, ATK_ATTR inAttr, UINT inUpKind) {
    if (m_pInstance && g_pPlayer) {
        ENCHANT_PARAM param;
        param.init(g_pPlayer->getHandle(), 1);
        param.uiState = 1;
        param.AtkKind = inAtkKind;
        param.UpKind = inUpKind;
        param.AtkAttr = inAttr;
        param.iAttackPoint = inPoint;
        param.iHp = g_pPlayer->getHP();
        param.iHpMax = g_pPlayer->getMaxHP();
        m_pInstance->calcEnchantParam(param);
        inPoint = param.iAttackPoint;
        inAtkKind = param.AtkKind;
    }
}
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Game/CardBattle", setPlayerEnchant__Q210CardBattle11CCardBattlei);
void CCardBattle::resetPlayerEnchant(INT inKind) {
    if (_ref(g_pPlayer)) {
        _ref(g_pPlayer)->setDefAttr(PL::g_fDefAttr[0]);
        _ref(g_pPlayer)->m_iMaxCombo = 3;
        { CPlayer* pPL = _ref(g_pPlayer); pPL->m_fPrizeDraw = 5.0f; pPL->m_fCardDraw = 5.0f; }
        _ref(g_pPlayer)->setBanish(FALSE);
        _ref(g_pPlayer)->setConditionFlag(0, 0x100000);
        _ref(g_pPlayer)->setConditionFlag(0, 0x80);
        _ref(g_pPlayer)->setConditionFlag(0, 0x100);
        _ref(g_pPlayer)->setConditionFlag(0, 0x20000000);
        _ref(g_pPlayer)->setConditionFlag(0, 0x40000000);
        _ref(g_pPlayer)->setConditionFlag(0, 0x1000);
        _ref(g_pPlayer)->setReflectGuard(FALSE);
        _ref(g_pPlayer)->setAeroGuard(FALSE);
        CCardRevolver* pRev = _ref(g_pPlayer)->getRevolver();
        if (pRev) {
            pRev->setChargeRate(100);
            pRev->setPlayNG(0, 0);
        }
    }
}
void CCardBattle::transferPlayerEnchant(CPlayer* inPL) {
    if (inPL && m_cardEnchant[0].m_nKind >= 0x44) {
        setPlayerEnchant(m_cardEnchant[0].m_nKind);
        inPL->setDefAttr(PL::g_fDefAttr[0]);
        inPL->m_iMaxCombo = 3;
        inPL->m_fPrizeDraw = 5.0f;
        inPL->m_fCardDraw = 5.0f;
        inPL->setBanish(FALSE);
        inPL->setConditionFlag(0, 0x100000);
        inPL->setConditionFlag(0, 0x80);
        inPL->setConditionFlag(0, 0x100);
        inPL->setConditionFlag(0, 0x20000000);
        inPL->setConditionFlag(0, 0x40000000);
        inPL->setConditionFlag(0, 0x1000);
        inPL->setReflectGuard(FALSE);
        inPL->setAeroGuard(FALSE);
    }
}
void CCardBattle::exeRegeneration() {
    UINT flag = 0x400;
    if (m_uiFlag & flag) {
        m_tqEnchantTimer[0] += getFrameRate();
        if (m_tqEnchantTimer[0] >= 6000) {
            if (g_pPlayer) {
                INT iHP = g_pPlayer->getHP();
                INT iMax = g_pPlayer->getMaxHP();
                if (!g_pPlayer->isConditionFlag(2) && iHP < iMax) {
                    INT iHeal = (iMax - iHP) / 8;
                    g_pPlayer->addHP(iHeal, 3000, 1);
                    decEnchantTime(1, 40, 39);
                    m_tqEnchantTimer[0] = 0;
                }
            }
        }
    }
    flag = 0x800;
    if (m_uiFlag & flag) {
        m_tqEnchantTimer[1] += getFrameRate();
        if (m_tqEnchantTimer[1] >= 6000) {
            decEnchantTime(2, 40, 39);
            m_tqEnchantTimer[1] = 0;
        }
    }
}
void CCardBattle::exeBio() {
    UINT flag = 0x1000;
    if (m_uiFlag & flag) {
        m_tqEnchantTimer[0] += getFrameRate();
        if (m_tqEnchantTimer[0] >= 1000) m_tqEnchantTimer[0] = 0;
    }
    flag = 0x2000;
    if (m_uiFlag & flag) {
        m_tqEnchantTimer[1] += getFrameRate();
        if (m_tqEnchantTimer[1] >= 1000) {
            CBattleEnemyManager* pBEM = (CBattleEnemyManager*)CTaskMan::getSysTask(0x19);
            if (pBEM) {
                for (SLContainerT<CTask>* container = pBEM->getFirstBattleEnemy(); container;
                     container = container->getNext()) {
                    CBattleEnemy* pEnemy = (CBattleEnemy*)container->getValue();
                    if (pEnemy && pEnemy->getTypeID() == 0x1004) pEnemy->onBio(1);
                }
            }
            m_tqEnchantTimer[1] = 0;
        }
    }
}
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Game/CardBattle", getDropFriendCard__10CardBattleR4CARDi);
INT CardBattle::getDropPlutoCard(CARD& outData) {
    if (CGame::isRikuGame()) return 0;
    if (CSceneInfo::isMinigameScene()) return 0;
    if (CCardBattle::getPlutoCount() >= 3) return 0;
    CCardRevolver* pRev = g_pPlayer ? g_pPlayer->getRevolver() : NULL;
    if (!pRev) return 0;
    BOOL bCheck2 = TRUE;
    INT r = RND::genrand_int31() % 1000;
    BOOL bCheck = TRUE;
    if (DebugWork::g_dw.player.iFriend > 0) {
        r = 0;
        bCheck = FALSE;
    }
    if (DebugWork::g_dw.player.iPluto > 0) {
        r = 0;
        bCheck = FALSE;
        bCheck2 = FALSE;
    }
    if (r >= 200) return 0;
    if (bCheck && !(CGameWork::getInt(0, 0x22) & 0x100)) return 0;
    if (bCheck2 && pRev->getHave(0, 1, 1, 0, 0) >= 10) return 0;
    _setCard(outData, 0x39, RND::genrand_int31() % 10);
    return 1;
}
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Game/CardBattle", exeDuel__Q210CardBattle11CCardBattle);
void CCardBattle::setDuelEntry(INT inFlag) {
    if (inFlag > 0) {
        if (m_uiDuelState == 1) m_uiDuelState = 2;
        return;
    }
    if (inFlag == -1) {
        if (!isDuelEntryingCard(0)) return;
    } else {
        if (m_uiDuelState != 2) return;
    }
    m_uiDuelState = 1;
    for (INT owner = 0; owner < 2; owner++) {
        for (INT i = 0; i < m_iCount[owner]; i++) {
            if (m_pCard[owner][i]) m_pCard[owner][i]->setDuelRefrect();
        }
        m_uiState[owner] = 5;
    }
    m_pMesage->sendMessage(0, getHandle(), m_hDuelEnemy, 9, &m_mesCardInfo[1]);
}
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Game/CardBattle", setDuelStart__Q210CardBattle11CCardBattlei);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Game/CardBattle", setDuelBreakCount__Q210CardBattle11CCardBattleii);
BOOL CCardBattle::setEnemyDuelCard(HANDLE inHandle, INT inIndex, INT inNumber) {
    if (m_uiDuelState != 5) return 0;
    if (m_uiState[0]) return 0;
    if (m_uiState[1]) return 0;
    CARD card;
    _setCard(card, inIndex, inNumber);
    INT result = setPlayCard(inHandle, 2, NULL, &card, 0);
    if (result > 0) initPlayingEnemy(inHandle);
    return result;
}
void CCardBattle::setDuelEnd() {
    if (!m_iDuelSkill) {
        m_uiDuelState = 1;
        CGameWork::setByte(1, 3, 2);
        m_pMesage->sendMessage(0, getHandle(), g_pPlayer->getHandle(), 11, &m_mesCardInfo[0]);
        m_pMesage->sendMessage(0, getHandle(), m_hDuelEnemy, 11, &m_mesCardInfo[1]);
        if (!CGameWork::getFlag(1, 7)) CGameWork::setFlag(1, 10, FALSE);
        if (m_pRevolver[0]) m_pRevolver[0]->returnStockCard(1);
        CGameWork::setFlag(1, 11, FALSE);
        CGameWork::setInt(1, 0xE2, CGameWork::getInt(1, 0xE2) ^ 0x14);
    } else {
        m_uiDuelState = 8;
        CGameWork::setByte(1, 3, 1);
        _ref(m_mesCardInfo[0].m_nStock) = m_iDuelSkill;
        m_pMesage->sendMessage(0, getHandle(), g_pPlayer->getHandle(), 12, &m_mesCardInfo[0]);
        m_pMesage->sendMessage(0, getHandle(), m_hDuelEnemy, 12, &m_mesCardInfo[1]);
        if (!CGameWork::getFlag(1, 7)) CGameWork::setFlag(1, 10, FALSE);
        CGameWork::setFlag(1, 11, FALSE);
        CGameWork::setInt(1, 0xE2, CGameWork::getInt(1, 0xE2) ^ 0x14);
        PLM::addIntBR(0x42);
    }
    PLM::calcDuelWin();
}
INT CCardBattle::setDuelPlayStock(CBattleCard** inCard, INT inSkill) {
    m_uiState[0] = 12;
    m_uiResult[0] = 0;
    for (INT i = 0; i < m_nDuelBreakCountMax; inCard++, i++) {
        CBattleCard* pCard = *inCard;
        m_pCard[0][i] = pCard;
        if (pCard) {
            m_cardData[0][i] = *pCard->getCardData();
            pCard->setPlayDuelStock(m_nDuelBreakCountMax);
        } else {
            _setCard(m_cardData[0][i], 0, 0);
        }
    }
    m_iStock[0] = inSkill;
    m_iExtra[0] = 0;
    m_iFirst[0] = 0;
    return 1;
}
BOOL CCardBattle::isDuelEntryingCard(INT inSide) {
    if (!m_pInstance) return FALSE;
    if ((UINT)inSide < 2) {
        UINT s = m_pInstance->m_uiState[0];
        if (s < 9) {
            if (s >= 6) return TRUE;
        }
    }
    if (inSide == 0 || inSide == 2) {
        UINT s = m_pInstance->m_uiState[1];
        if (s < 9) {
            if (s >= 6) return TRUE;
        }
    }
    return FALSE;
}
CDuelGauge::CDuelGauge(::CTask* pParent) : ::CTask(pParent, 0x4000) {
    m_pSeqCtrls = NULL;
    _initialize();
}
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Game/CardBattle", _$_Q210CardBattle10CDuelGauge);
void CDuelGauge::_initialize() {
    m_uiFlag = 0;
    m_uiState = 0;
    m_uiTimerState = 0;
    m_uiCountState = 0;
    m_uiCount = 0;
    m_uiCountMax = 0;
    m_uiResult = 0;
    m_tqTimer = 0;
    m_tqLimit = 0;
    m_hTimerSE = 0;
    m_pSeqCtrls = new (D_00477690) D2::C2DSeqCtrl[12];
}
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Game/CardBattle", main__Q210CardBattle10CDuelGauge);
/* Parked: one store slot - the original schedules the m_tqTimer store (0x5c) after the zero stores of m_uiCount and
 * m_uiResult, ours before them (and the setLayer sb one slot earlier); the statement order is the $LM line order;
 * tried: all 720 orders of the six stores (200 tested), chained/temp assignments for timer/limit, pSeq load forms
 * (_ref, m_pSeqCtrls, &[0]), permuter 4 min (13841 candidates, no gain) */
#if 0
void CDuelGauge::setStart(TIMEQ inTime, UINT inCount) {
    m_uiState = 1;
    m_uiCount = 0;
    m_uiCountMax = inCount;
    m_uiResult = 0;
    m_tqTimer = inTime;
    m_tqLimit = inTime;
    D2::C2DSeqCtrl* pSeq = &m_pSeqCtrls[0];
    pSeq->setLayer(8);
    pSeq->addSeq(g_szSeqDuel, 30);
    FVECTOR pos;
    D2::C2DMan::getLayPosition(g_szLadDuel, 1, 0, pos, FALSE);
    pSeq->setPos(pos);
    setTimer(1);
    setCount(1);
    CSound::playSE(0x14B, -1, 64);
    m_hTimerSE = CSound::playSE(0x14C, -1, 64);
}
#else
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Game/CardBattle", setStart__Q210CardBattle10CDuelGaugeiUi);
#endif
void CDuelGauge::setResult(UINT inResult) {
    m_uiResult = inResult;
    D2::C2DSeqCtrl* pSeq = &m_pSeqCtrls[0];
    m_uiState = 3;
    pSeq->setLayer(8);
    if (m_uiResult) {
        pSeq->addSeq(g_szSeqDuel, 31);
        CSound::playSE(0x14E, -1, 64);
    } else {
        pSeq->addSeq(g_szSeqDuel, 32);
        CSound::playSE(0x14F, -1, 64);
    }
    setTimer(3);
    setCount(3);
    if (m_hTimerSE > 0) {
        CSound::stopSE(m_hTimerSE, 0x14C);
        m_hTimerSE = 0;
    }
}
void CDuelGauge::setTimer(UINT inState) {
    D2::C2DSeqCtrl* pSeq;
    INT i;
    FVECTOR pos;
    switch (inState) {
    case 0:
        m_uiTimerState = inState;
        for (i = 1; i < 8; i++) {
            m_pSeqCtrls[i].destroy();
        }
        break;
    case 1:
        m_uiTimerState = 1;
        D2::C2DMan::getLayPosition(_ref(g_szLadDuel), 1, 0, pos, FALSE);
        pSeq = &m_pSeqCtrls[1];
        pSeq->destroy();
        pSeq->setLayer(2);
        pSeq->addSeq(g_szSeqDuel, 0x21);
        pSeq->setPos(pos);
        pSeq->addSeq(g_szSeqDuel, 0x24);
        pSeq->enableColorCtrl(TRUE);
        for (i = 0; i < 6; i++) {
            pSeq = &m_pSeqCtrls[i + 2];
            pSeq->destroy();
            pSeq->setLayer(2);
            pSeq->addSeq(g_szSeqDuel, g_DuelSeqIDX_TimerNumDmy[i][0]);
            pSeq->addSeq(g_szSeqDuel, 0x25);
            pSeq->enableColorCtrl(TRUE);
            pSeq->setPos(pos);
        }
        changeTimer(m_tqLimit);
        break;
    case 2:
        m_uiTimerState = inState;
        pSeq = &m_pSeqCtrls[1];
        pSeq->destroy();
        pSeq->setLayer(2);
        pSeq->addSeq(g_szSeqDuel, 0x22);
        pSeq->addSeq(g_szSeqDuel, 0x24);
        pSeq->enableColorCtrl(TRUE);
        for (i = 0; i < 6; i++) {
            pSeq = &m_pSeqCtrls[i + 2];
            pSeq->destroy();
            pSeq->setLayer(2);
            pSeq->addSeq(g_szSeqDuel, g_DuelSeqIDX_TimerNumDmy[i][1]);
            pSeq->addSeq(g_szSeqDuel, 0x25);
            pSeq->enableColorCtrl(TRUE);
        }
        changeTimer(m_tqTimer);
        break;
    case 3:
        m_uiTimerState = inState;
        pSeq = &m_pSeqCtrls[1];
        pSeq->destroy();
        pSeq->setLayer(2);
        pSeq->addSeq(g_szSeqDuel, 0x23);
        pSeq->addSeq(g_szSeqDuel, 0x24);
        pSeq->enableColorCtrl(TRUE);
        for (i = 0; i < 6; i++) {
            pSeq = &m_pSeqCtrls[i + 2];
            pSeq->destroy();
            pSeq->setLayer(2);
            pSeq->addSeq(g_szSeqDuel, g_DuelSeqIDX_TimerNumDmy[i][2]);
            pSeq->addSeq(g_szSeqDuel, 0x25);
            pSeq->enableColorCtrl(TRUE);
        }
        changeTimer(m_tqTimer);
        break;
    }
}
void CDuelGauge::changeTimer(TIMEQ inTime) {
    INT n = inTime * 100 / m_tqLimit;
    m_pSeqCtrls[1].changeTime(1, 100 - n);
    n = inTime % 3000 / 30 % 10;
    m_pSeqCtrls[2].changeTexture(1, 0, NULL, &g_uvDuelNumTex[0][n], &g_uvDuelNumTex[1][n]);
    n = inTime % 3000 / 300 % 10;
    m_pSeqCtrls[3].changeTexture(1, 0, NULL, &g_uvDuelNumTex[0][n], &g_uvDuelNumTex[1][n]);
    n = inTime / 3000 % 10;
    m_pSeqCtrls[4].changeTexture(1, 0, NULL, &g_uvDuelNumTex[0][n], &g_uvDuelNumTex[1][n]);
    n = inTime / 30000 % 10;
    m_pSeqCtrls[5].changeTexture(1, 0, NULL, &g_uvDuelNumTex[0][n], &g_uvDuelNumTex[1][n]);
}
void CDuelGauge::exeTimer() {
    D2::C2DSeqCtrl* pSeq = &m_pSeqCtrls[1];
    switch (m_uiTimerState) {
    case 1:
        if (pSeq->isEnd(0)) setTimer(2);
        break;
    case 0:
        break;
    case 2:
        if (m_tqTimer > 0) {
            m_tqTimer -= getFrameRate();
            if (m_tqTimer < 0) m_tqTimer = 0;
            changeTimer(m_tqTimer);
        }
        break;
    case 3:
        if (pSeq->isEnd(0)) setTimer(0);
        break;
    }
}
void CDuelGauge::setCount(UINT inState) {
    D2::C2DSeqCtrl* pSeq;
    FVECTOR pos;
    switch (inState) {
    case 0:
        m_uiCountState = inState;
        m_pSeqCtrls[8].destroy();
        m_pSeqCtrls[9].destroy();
        m_pSeqCtrls[10].destroy();
        break;
    case 1:
        m_uiCountState = inState;
        D2::C2DMan::getLayPosition(_ref(g_szLadDuel), 1, 0, pos, FALSE);
        pSeq = &m_pSeqCtrls[8];
        pSeq->destroy();
        pSeq->setLayer(2);
        pSeq->addSeq(g_szSeqDuel, 0);
        pSeq->setPos(pos);
        pSeq = &m_pSeqCtrls[9];
        pSeq->destroy();
        pSeq->setLayer(2);
        pSeq->addSeq(g_szSeqDuel, 6);
        pSeq->addSeq(g_szSeqDuel, m_uiCountMax + 20);
        pSeq->setPos(pos);
        pSeq->enableColorCtrl(TRUE);
        pSeq = &m_pSeqCtrls[10];
        pSeq->destroy();
        pSeq->setLayer(2);
        pSeq->addSeq(g_szSeqDuel, 3);
        pSeq->addSeq(g_szSeqDuel, m_uiCount + 9);
        pSeq->setPos(pos);
        pSeq->enableColorCtrl(TRUE);
        break;
    case 2:
        m_uiCountState = inState;
        pSeq = &m_pSeqCtrls[8];
        pSeq->destroy();
        pSeq->setLayer(2);
        pSeq->addSeq(g_szSeqDuel, 1);
        pSeq = &m_pSeqCtrls[9];
        pSeq->destroy();
        pSeq->setLayer(2);
        pSeq->addSeq(g_szSeqDuel, 7);
        pSeq->addSeq(g_szSeqDuel, m_uiCountMax + 20);
        pSeq->enableColorCtrl(TRUE);
        pSeq = &m_pSeqCtrls[10];
        pSeq->destroy();
        pSeq->setLayer(2);
        pSeq->addSeq(g_szSeqDuel, 4);
        if (m_uiCount) {
            pSeq->addSeq(g_szSeqDuel, m_uiCount + 9);
        } else {
            pSeq->addSeq(g_szSeqDuel, 9);
        }
        pSeq->enableColorCtrl(TRUE);
        break;
    case 3:
        m_uiCountState = inState;
        pSeq = &m_pSeqCtrls[8];
        pSeq->destroy();
        pSeq->setLayer(2);
        pSeq->addSeq(g_szSeqDuel, 2);
        pSeq = &m_pSeqCtrls[9];
        pSeq->destroy();
        pSeq->setLayer(2);
        pSeq->addSeq(g_szSeqDuel, 8);
        pSeq->addSeq(g_szSeqDuel, m_uiCountMax + 20);
        pSeq->enableColorCtrl(TRUE);
        pSeq = &m_pSeqCtrls[10];
        pSeq->destroy();
        pSeq->setLayer(2);
        pSeq->addSeq(g_szSeqDuel, 5);
        if (m_uiResult) {
            pSeq->addSeq(g_szSeqDuel, m_uiCount + 9);
        } else {
            pSeq->addSeq(g_szSeqDuel, 9);
            m_pSeqCtrls[10].changeTexture(1, 0, NULL, &g_uvDuelNumTex[2][m_uiCount], &g_uvDuelNumTex[3][m_uiCount]);
        }
        pSeq->enableColorCtrl(TRUE);
        break;
    }
}
void CDuelGauge::exeCount() {
    D2::C2DSeqCtrl* pSeq = &m_pSeqCtrls[8];
    switch (m_uiCountState) {
    case 1:
        if (pSeq->isEnd(0)) setCount(2);
        break;
    case 0:
    case 2:
        break;
    case 3:
        if (pSeq->isEnd(0)) setCount(0);
        break;
    }
}
void CDuelGauge::changeCount(UINT inCount, UINT inMax) {
    m_uiCount = inCount % 10;
    if (m_uiCount) {
        m_pSeqCtrls[10].replaceSeq(1, g_szSeqDuel, m_uiCount + 9);
    } else {
        m_pSeqCtrls[10].replaceSeq(1, g_szSeqDuel, 9);
    }
    if (inMax) {
        m_uiCountMax = inMax % 10;
        m_pSeqCtrls[9].changeTexture(1, 0, NULL, &g_uvDuelNumTex[2][m_uiCountMax], &g_uvDuelNumTex[3][m_uiCountMax]);
    }
}
void CDuelGauge::setDuelNG() {
    D2::C2DSeqCtrl* pSeq = &m_pSeqCtrls[11];
    if (pSeq) {
        m_uiFlag |= 1;
        pSeq->destroy();
        pSeq->setLayer(8);
        pSeq->addSeq(g_szSeqCockpit, 0x97);
        CB_LAYOUT* pLayout = &g_LayoutData[1];
        FVECTOR pos = {{pLayout->x, pLayout->y, 0.0f, 1.0f}};
        pSeq->setPos(pos);
    }
}
void CDuelGauge::exeDuelNG() {
    UINT flag = 1;
    if (m_uiFlag & flag) {
        D2::C2DSeqCtrl* pSeq = &m_pSeqCtrls[11];
        if (pSeq && pSeq->isEnd(0)) {
            pSeq->destroy();
            m_uiFlag &= ~flag;
        }
    }
}
INT CCardBattle::setPronounceStart(INT inFlag) {
    if (inFlag) {
        if (m_uiDuelState != 9) return 0;
        m_uiDuelState = 10;
        m_pMesage->sendMessage(0, getHandle(), m_hDuelEnemy, 13, &m_mesCardInfo[1]);
        return 1;
    }
    m_uiFlag |= 0xC0000000;
    m_uiDuelState = 9;
    for (INT i = 0; i < m_iCount[0]; i++) {
        if (m_pCard[0][i]) m_pCard[0][i]->setBreak(0);
    }
    m_uiState[0] = 5;
    CGameWork::setFlag(1, 10, TRUE);
    CCardRevolver* pRV = m_pRevolver[0];
    if (pRV) {
        pRV->returnStockCard(1);
        pRV->changeMode(0, 1);
    }
    CGameWork::setFlag(1, 11, TRUE);
    return 1;
}
INT CCardBattle::setPronouncePlay(INT inFlag) {
    CStockFolder* pSF = m_pStockFolder[1];
    if (!pSF) return 0;
    if (inFlag) {
        while (pSF->getStock(0) < 6) {
            pSF->setPronounceCard(0x98, RND::genrand_int31() % 8 + 1);
        }
    }
    m_uiDuelState = 11;
    setDuelBreakCount(0, 6);
    return pSF->setPlayPronounce();
}
INT CCardBattle::setEnemyPronounceCard(HANDLE inOwner, CBattleCard* inCard) {
    if (m_uiState[1]) return 0;
    m_hOwner[1] = inOwner;
    m_uiState[1] = 14;
    m_uiResult[1] = 0;
    CBattleCard* pOld = m_pCard[1][0];
    if (pOld && !pOld->getRevolver()) pOld->out();
    m_pCard[1][0] = inCard;
    if (inCard) {
        inCard->setEnchantEffect(0x44, 0);
        m_pCard[1][0]->setEnchantEffect(0x52, 0);
        m_pCard[1][0]->setEnchantEffect(0x53, 0);
    }
    m_cardData[1][0] = *inCard->getCardData();
    m_iStock[1] = 0;
    m_iExtra[1] = 0;
    m_iFirst[1] = 0;
    m_pCard[1][0]->setPlayPronounce();
    return 1;
}
void CCardBattle::setPronounceEnd(INT inCount) {
    if (m_uiDuelState < 10) return;
    m_uiDuelState = 0;
    disablePlayCard(0);
    if (inCount > 0) {
        CGameWork::setFlag(1, 10, FALSE);
        CGameWork::setFlag(1, 11, FALSE);
        m_pMesage->sendMessage(0, getHandle(), g_pPlayer->getHandle(), 15, &m_mesCardInfo[0]);
        m_pMesage->sendMessage(0, getHandle(), m_hDuelEnemy, 3, &m_mesCardInfo[1]);
    } else {
        resetEnchantCard(1);
        m_pMesage->sendMessage(0, getHandle(), g_pPlayer->getHandle(), 14, &m_mesCardInfo[0]);
        m_pMesage->sendMessage(0, getHandle(), m_hDuelEnemy, 14, &m_mesCardInfo[1]);
        if (g_pPlayer->getRevolver()) g_pPlayer->getRevolver()->rollDeck(0, 1, 300);
    }
}
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Game/CardBattle", disablePlayCard__Q210CardBattle11CCardBattleUi);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Game/CardBattle", isDisablePlayCard__Q210CardBattle11CCardBattle);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Game/CardBattle", isCardBattleEnd__Q210CardBattle11CCardBattle);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Game/CardBattle", disableDuelEntry__Q210CardBattle11CCardBattleUi);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Game/CardBattle", getDuelEnemy__CQ210CardBattle11CCardBattle);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Game/CardBattle", getDuelBreakCountMax__CQ210CardBattle11CCardBattle);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Game/CardBattle", isNowDuelEntry__CQ210CardBattle11CCardBattle);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Game/CardBattle", isNowDuel__CQ210CardBattle11CCardBattle);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Game/CardBattle", isNowSetEnemyDuelCard__CQ210CardBattle11CCardBattle);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Game/CardBattle", getDuelSkill__Q210CardBattle11CCardBattle);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Game/CardBattle", isPronounceMode__CQ210CardBattle11CCardBattle);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Game/CardBattle", isNowPronounce__CQ210CardBattle11CCardBattle);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Game/CardBattle", isNowSetEnemyPronounce__CQ210CardBattle11CCardBattle);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Game/CardBattle", setSubTimer__Q210CardBattle11CCardBattlei);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Game/CardBattle", getSubTimer__CQ210CardBattle11CCardBattle);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Game/CardBattle", getEnchantKind__CQ210CardBattle11CCardBattlei);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Game/CardBattle", getRoomCard__Q210CardBattle11CCardBattle);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Game/CardBattle", getEnchantCard__Q210CardBattle11CCardBattlei);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Game/CardBattle", getStockCount__Q210CardBattle11CCardBattlei);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Game/CardBattle", getState__CQ210CardBattle10CDuelGauge);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Game/CardBattle", getTimer__CQ210CardBattle10CDuelGauge);
