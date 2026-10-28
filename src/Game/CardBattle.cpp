#include "khtypes.h"
#include "System/Task.h"
#include "Game/CardBattle.h"
#include "Game/BattleCard.h"
#include "Game/Entity.h"
#include "Game/Player/Player.h"
#include "Game/Enemy/BattleEnemy.h"
#include "Game/Enemy/BattleEnemyManager.h"
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
void calcFreezeDamage(INT& ioDamage);

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
extern "C" char D_00477298[]; /* notifyCompleteGimmick's TRACE format */
namespace PL {
extern FLOAT g_fDefAttr[18][6];
}
/* the unit's copy of Game/CardKind's header static table (data still from assembly) */
extern const CARD_DATA cardDataTbl[224] __asm__("cardDataTbl_00478270");
/* CARD's inline constructor (Game/Card.cpp's _setCard) */
static inline void _setCard(CARD& c, USHORT nKind, CHAR nNumber) {
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
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Game/CardBattle", getInstance__Q210CardBattle11CCardBattle);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Game/CardBattle", init__Q210CardBattle11CCardBattle);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Game/CardBattle", main__Q210CardBattle11CCardBattle);
void CCardBattle::disp() {}
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Game/CardBattle", calcPlayingCard__Q210CardBattle11CCardBattle);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Game/CardBattle", exePlaying__Q210CardBattle11CCardBattlei);
void CCardBattle::setNumberBreak(UINT _PLAYER, UINT _ENEMY) {
    if (m_cardEnchant[_PLAYER][0] == 0x65) {
        INT iCountE = m_iCount[_ENEMY];
        INT iPoint = m_iPoint[_PLAYER];
        for (INT i = 0; i < iCountE; i++) {
            CBattleCard* pCard = m_pCard[_ENEMY][i];
            if (pCard) {
                INT iPointE = pCard->getCardData()->m_nNumber;
                pCard->setNumberBreak(iPoint);
                iPoint -= iPointE;
                if (iPoint < 0) iPoint = 0;
                *(CARD*)m_cardData[_ENEMY][i] = *m_pCard[_ENEMY][i]->getCardData();
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
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Game/CardBattle", setPlayCard__Q210CardBattle11CCardBattleiiPQ210CardBattle11CBattleCardP4CARDi);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Game/CardBattle", setPlayStock__Q210CardBattle11CCardBattleiiPPQ210CardBattle11CBattleCardiiii);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Game/CardBattle", setEnchantCard__Q210CardBattle11CCardBattleiiPQ210CardBattle11CBattleCard);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Game/CardBattle", decEnchantTime__Q210CardBattle11CCardBattleiii);
void CCardBattle::destroyPlayCard(INT inFlag, INT inP1) { resetNowPlaying(inFlag); }
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Game/CardBattle", resetNowPlaying__Q210CardBattle11CCardBattlei);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Game/CardBattle", resetEnchantCard__Q210CardBattle11CCardBattlei);
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
    if ((UINT)inSide < 2 && m_pInstance->m_uiState[0]) return m_pInstance->m_cardData[0][inIdx][0];
    if ((inSide == 0 || inSide == 2) && m_pInstance->m_uiState[1]) return m_pInstance->m_cardData[1][inIdx][0];
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
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Game/CardBattle", notifyMakeGimmick__Q210CardBattle11CCardBattleP4CARDi);
void CCardBattle::notifyCompleteGimmick() {
    if (m_pInstance) {
        TRACE(D_00477298, ((CARD*)m_pInstance->m_cardGimmick)->m_nKind);
        _setCard(*(CARD*)m_pInstance->m_cardGimmick, 0, 0);
        _ref(m_pInstance)->m_uiFlag &= ~0x8000;
    }
}
BOOL CCardBattle::isPlayingGimmickCard() {
    if (!m_pInstance) return FALSE;
    return m_pInstance->m_cardGimmick[0];
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
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Game/CardBattle", initCardBreak__Q210CardBattle11CCardBattleiUi);
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
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Game/CardBattle", setEnemyCard__Q210CardBattle11CCardBattleiii);
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
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Game/CardBattle", resetPlayerEnchant__Q210CardBattle11CCardBattlei);
void CCardBattle::transferPlayerEnchant(CPlayer* inPL) {
    if (inPL && m_cardEnchant[0][0] >= 0x44) {
        setPlayerEnchant(m_cardEnchant[0][0]);
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
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Game/CardBattle", getDropPlutoCard__10CardBattleR4CARD);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Game/CardBattle", exeDuel__Q210CardBattle11CCardBattle);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Game/CardBattle", setDuelEntry__Q210CardBattle11CCardBattlei);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Game/CardBattle", setDuelStart__Q210CardBattle11CCardBattlei);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Game/CardBattle", setDuelBreakCount__Q210CardBattle11CCardBattleii);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Game/CardBattle", setEnemyDuelCard__Q210CardBattle11CCardBattleiii);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Game/CardBattle", setDuelEnd__Q210CardBattle11CCardBattle);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Game/CardBattle", setDuelPlayStock__Q210CardBattle11CCardBattlePPQ210CardBattle11CBattleCardi);
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
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Game/CardBattle", setPronounceStart__Q210CardBattle11CCardBattlei);
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
    *(CARD*)m_cardData[1][0] = *inCard->getCardData();
    m_iStock[1] = 0;
    m_iExtra[1] = 0;
    m_iFirst[1] = 0;
    m_pCard[1][0]->setPlayPronounce();
    return 1;
}
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Game/CardBattle", setPronounceEnd__Q210CardBattle11CCardBattlei);
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
