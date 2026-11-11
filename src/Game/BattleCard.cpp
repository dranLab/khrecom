#define GAME_BATTLECARD_CPP
#include "khtypes.h"
#define GPRIM_H_PATH "System/Graphic/GPrim.h" /* MATCH: the original includes it under this name */
#define CARDBATTLE_H_PATH "c:/KHCOM/MainApp/Game/CardBattle.h" /* MATCH: the original includes it under this name */
#include "System/TaskMan.h"
#include "System/Task.h"
#include "System/Graphic/Gprim.h"
#include "System/Graphic/2d/2DMan.h"
#include "Game/CardBattle.h"
#include "System/Graphic/Anim.h"
#include "System/Graphic/Str.h"
#include "Game/Collision/CollUtil.h"
/* MATCH: placeholder: an empty string follows CollUtil.h's strings in .rodata (as in Game/CardRevolver) */
inline void _emptyStringPlaceholderBattleCard() { TRACE(""); }
#include "Game/BattleCard.h"
#include "Game/GameWork.h"
#include "Game/Debug/DebugWork.h"
#include "System/Rnd.h"
#include "System/Com.h"
#include "System/Graphic/WorldEnv.h"
#include "Game/Entity.h"
#include "include_asm.h"
#include "System/HeapMan.h"

extern "C" int abs(int);
extern "C" float sinf(float);

using namespace CardBattle;

extern INT SCREEN_HEIGHT;






extern INT SCREEN_HEIGHT;
namespace CardBattle {
static LPCSTR g_szDpxZexion2 = "zexion2";
static LPCSTR g_szLadLayoutCard = "layout_card.lad";
unsigned char g_Seq_toLayout[26] = {
    11, 9, 8, 7, 20, 17, 18, 16, 29, 28, 27, 26, 25, 24, 33, 34, 35, 30, 31, 32, 6, 5, 4, 3, 2, 1,
};
static char* g_pBC_DbgStateName[9] = {
    "STATE_USE", "STATE_STOCK", "STATE_HAND", "STATE_DESTROY", "STATE_DUEL", "STATE_SNATCH", "STATE_PRONOUNCE",
    "STATE_MAX", "STATE_NONE",
};
static char* g_pBC_DbgModeName[21] = {
    "MODE_NULL", "MODE_START", "MODE_EXEC", "MODE_END", "MODE_REFRECT_START", "MODE_REFRECT", "MODE_BREAK",
    "MODE_ENCHANT", "MODE_STOCK_START", "MODE_STOCK", "MODE_HAND_NONE", "MODE_HAND_ACTIVE", "MODE_HAND_ROLL",
    "MODE_HAND_HIDE", "MODE_HAND_OUT", "MODE_HAND_RELOAD", "MODE_DUEL_ENTRY", "MODE_SNATCH_START", "MODE_SNATCH",
    "MODE_TRAP_START", "MODE_DISCARD",
};
CB_LAYOUT g_LayoutData[65] = {
    {0, 0, 0, 0}, // 0
    {0, SCREEN_HEIGHT - 96, 4, 100}, // 1
    {-16, SCREEN_HEIGHT - 68, 4, 80}, // 2
    {0, SCREEN_HEIGHT - 96, 3, 80}, // 3
    {-16, SCREEN_HEIGHT - 68, 4, 80}, // 4
    {0, SCREEN_HEIGHT - 96, 3, 80}, // 5
    {16, SCREEN_HEIGHT - 60, 2, 80}, // 6
    {64, SCREEN_HEIGHT - 96, 5, 100}, // 7
    {84, SCREEN_HEIGHT - 64, 4, 100}, // 8
    {94, SCREEN_HEIGHT - 12, 3, 100}, // 9
    {94, SCREEN_HEIGHT + 48, 2, 100}, // 10
    {9, SCREEN_HEIGHT - 120, 4, 100}, // 11
    {-32, SCREEN_HEIGHT - 120, 3, 100}, // 12
    {-64, SCREEN_HEIGHT + 96, 2, 100}, // 13
    {-44, SCREEN_HEIGHT - 64, 6, 100}, // 14
    {72, SCREEN_HEIGHT - 24, 2, 40}, // 15
    {448, SCREEN_HEIGHT - 96, 5, 100}, // 16
    {473, SCREEN_HEIGHT - 60, 4, 80}, // 17
    {468, SCREEN_HEIGHT - 12, 3, 80}, // 18
    {463, SCREEN_HEIGHT + 48, 2, 80}, // 19
    {488, SCREEN_HEIGHT - 72, 3, 80}, // 20
    {576, SCREEN_HEIGHT + 96, 2, 80}, // 21
    {512, SCREEN_HEIGHT - 120, 6, 100}, // 22
    {492, SCREEN_HEIGHT, 2, 40}, // 23
    {125, 65, 5, 50}, // 24
    {150, 65, 4, 50}, // 25
    {175, 65, 3, 50}, // 26
    {125, 85, 5, 50}, // 27
    {150, 85, 4, 50}, // 28
    {175, 85, 3, 50}, // 29
    {440, 70, 3, 50}, // 30
    {415, 70, 4, 50}, // 31
    {390, 70, 5, 50}, // 32
    {440, 90, 3, 50}, // 33
    {415, 90, 4, 50}, // 34
    {390, 90, 5, 50}, // 35
    {124, 63, 6, 50}, // 36
    {149, 63, 5, 50}, // 37
    {174, 63, 4, 50}, // 38
    {199, 63, 3, 50}, // 39
    {224, 63, 2, 50}, // 40
    {249, 63, 2, 50}, // 41
    {274, 63, 2, 50}, // 42
    {235, 339, 4, 80}, // 43
    {257, 340, 3, 80}, // 44
    {278, 342, 2, 80}, // 45
    {235, 338, 6, 80}, // 46
    {246, 339, 5, 80}, // 47
    {257, 340, 4, 80}, // 48
    {268, 341, 3, 80}, // 49
    {278, 342, 2, 80}, // 50
    {235, 337, 8, 80}, // 51
    {243, 338, 7, 80}, // 52
    {250, 339, 6, 80}, // 53
    {257, 340, 5, 80}, // 54
    {264, 341, 4, 80}, // 55
    {271, 342, 3, 80}, // 56
    {278, 343, 2, 80}, // 57
    {84, SCREEN_HEIGHT - 120, 6, 100}, // 58
    {256, 336, 7, 100}, // 59
    {276, 336, 6, 100}, // 60
    {296, 336, 5, 100}, // 61
    {316, 336, 4, 100}, // 62
    {336, 336, 3, 100}, // 63
    {356, 336, 2, 100}, // 64
};
}
#line 355
namespace CardBattle {
}
FLOAT RotTransPersClipW(IVECTOR& out_v, const FVECTOR& in_v);
static inline FLOAT _adjustRad(FLOAT r) {
    return (r < -3.1415927f) ? r + 6.2831855f : (r > 3.1415927f) ? r - 6.2831855f : r; // MATCH: one ternary (stateSnatch's wrap keeps the sum in the m_vRot.y register)
}

void CardBattle::initLayoutData() {
    FVECTOR vPos;
    for (INT i = 0; i < 26; i++) {
        D2::C2DMan::getLayPosition(g_szLadLayoutCard, 0, i, vPos, FALSE);
        CB_LAYOUT& lay = g_LayoutData[g_Seq_toLayout[i]];
        lay.x = (INT)vPos.x;
        lay.y = (INT)vPos.y;
    }
}

CardBattle::CBattleCard::CBattleCard(CTask* pParent, INT inIndex, INT inNumber, INT inUser, INT inState)
    : CTask(pParent, 0x4000) {
    m_uiFlag = 0;
    m_uiUser = inUser;
    m_iStock = -1;
    m_iHandMax = 0;
    m_tqTimer = 0;
    m_pCard = NULL;
    m_pRevolver = NULL;
    m_iHand = -1;
    m_uiOwner = inUser;
    m_State[0].set(this, &CBattleCard::preStateUse, &CBattleCard::stateUse, NULL);
    m_State[1].set(this, &CBattleCard::preStateStock, &CBattleCard::stateStock, NULL);
    m_State[2].set(this, &CBattleCard::preStateHand, &CBattleCard::stateHand, NULL);
    m_State[3].set(this, &CBattleCard::preStateDestroy, &CBattleCard::stateDestroy, NULL);
    m_State[4].set(this, &CBattleCard::preStateDuel, &CBattleCard::stateDuel, NULL);
    m_State[5].set(this, &CBattleCard::preStateSnatch, &CBattleCard::stateSnatch, NULL);
    m_State[6].set(this, &CBattleCard::preStatePronounce, &CBattleCard::statePronounce, NULL);
    m_cardData.init(inIndex, inNumber);
    m_nNumberR = -1;
    m_nNumberE = -1;
    m_nNumber = inNumber;
    m_pCard = new KH_AT("(246)") C2DCard;
    m_iStockCount = 0;
    m_hTarget = 0;
    if (inState == 2) {
        // MATCH: stores through plain pointers (not in-struct MEMs) are ordered before the SCREEN_HEIGHT load, which
        // gives them the scheduling priority to come first
        *(CCardRevolver**)&m_pRevolver = (CCardRevolver*)pParent;
        *(INT*)&m_iState = inState;
        m_uiMode = 10;
        m_Pos.set(0, SCREEN_HEIGHT + 0x60);
        m_vRot.set(0.0f, 0.0f, 0.0f, 1.0f);
        m_fBright = 1.0f;
        m_uiFlag |= 0x20;
        changeState(2);
    } else {
        _initialize();
    }
    {
        struct _bits { u_long b0 : 28; u_long cat1 : 4; };
        if (((_bits*)&m_cardData)->cat1 == 4) m_uiFlag |= 0x8000;
    }
}
CBattleCard::~CBattleCard() {
    if (m_pCard) delete m_pCard;
    m_pCard = NULL;
}
void CBattleCard::_initialize() {
    m_iState = 8;
    changeState(0);
    m_uiMode = 0;
    m_Pos.set(0, SCREEN_HEIGHT + 96);
    m_vRot.set(0.0f, 0.0f, 0.0f, 1.0f);
    m_iLayer = 3;
    m_fBright = 1.0f;
    m_pCard->setLayer(3);
    m_pCard->create(m_cardData);
    FVECTOR pos = {{m_Pos.x, m_Pos.y, 0.0f, 1.0f}};
    m_pCard->setPos(pos);
    m_uiFlag = (INT)m_uiFlag & ~0x80;
    m_pCard->enableDisp(FALSE);
}
void CBattleCard::changeData(CARD& inData) {
    if (m_pCard) {
        m_cardData = inData;
        m_pCard->destroy();
        if (!m_cardData.m_nKind) {
            m_uiFlag = (INT)m_uiFlag & ~0x80;
        } else {
            m_pCard->create(m_cardData);
            FVECTOR pos = {{m_Pos.x, m_Pos.y, 0.0f, 1.0f}};
            m_pCard->setPos(pos);
            m_uiFlag = (INT)m_uiFlag & ~0x20;
            setRoomEffect(NULL);
        }
    }
}
void CBattleCard::main() {
    ((CFSM*)this)->update();
    /* MATCH: the category is read through a 64-bit bit-field over m_cardData (see updateNumberEffect) */
    struct _bits { u_long b0 : 28; u_long cat1 : 4; };
    if (m_uiUser == 1 && ((_bits*)&m_cardData)->cat1 != 8) {
        if (CCardBattle::isBlack()) {
            if (m_fBright > 0.0f) {
                m_fBright -= getFrameRateF() * 0.05f;
                if (m_fBright < 0.0f) m_fBright = 0.0f;
                m_uiFlag |= 0x4000;
            }
        } else if ((m_uiFlag & 0x80000) && m_iState == 2) {
            if (m_fBright > 0.4f) {
                m_fBright -= getFrameRateF() * 0.05f;
                if (m_fBright < 0.4f) m_fBright = 0.4f;
                m_uiFlag |= 0x4000;
            }
        } else {
            if (m_fBright < 1.0f) {
                m_fBright += getFrameRateF() * 0.05f;
                if (m_fBright > 1.0f) m_fBright = 1.0f;
                m_uiFlag |= 0x4000;
            }
        }
    }
    dispCard();
}
void CBattleCard::dispCard() {
    BOOL disp = TRUE;
    UINT shift;
    UINT flag;
    CGameWork::getInt(1, 0xe2);
    shift = m_uiUser != 1;
    flag = 0x20 << shift;
    if (CGameWork::getInt(1, 0xe2) & flag) {
        disp = FALSE;
        goto end;
    }
    flag = 2 << shift;
    if ((CGameWork::getInt(1, 0xe2) & flag) && m_iState == 2) {
        disp = FALSE;
    } else if (!isDisp()) {
        disp = FALSE;
    } else {
        FVECTOR pos = {{m_Pos.x, m_Pos.y, 0.0f, 1.0f}};
        m_pCard->setPos(pos);
        if (m_uiFlag & 0x8) {
            FVECTOR rot = {{m_vRot.x, m_vRot.y, m_vRot.z, 1.0f}};
            m_uiFlag = (INT)m_uiFlag & ~0x8;
            m_pCard->setRot(rot);
            m_pCard->setScl(m_vRot.w);
        }
        if (m_uiFlag & 0x2000) {
            m_uiFlag = (INT)m_uiFlag & ~0x2000;
            m_pCard->setScl(m_vRot.w);
        }
        if (m_uiFlag & 0x4000) {
            FVECTOR col = {{m_fBright, m_fBright, m_fBright, 1.0f}};
            m_uiFlag = (INT)m_uiFlag & ~0x4000;
            m_pCard->enableColorCtrl(TRUE);
            m_pCard->setCol(col);
        }
    }
end:
    if (!disp) {
        if (m_uiFlag & 0x80) {
            m_uiFlag = (INT)m_uiFlag & ~0x80;
            m_pCard->enableDisp(FALSE);
        }
    } else {
        if (!(m_uiFlag & 0x80)) {
            m_uiFlag |= 0x80;
            m_pCard->enableDisp(TRUE);
        }
    }
}
void CBattleCard::testDisp() {
    if (m_pCard) {
        if (CGameWork::getInt(1, 0xe2)) {
            m_uiFlag = (INT)m_uiFlag & ~0x80;
            if (m_cardData.m_nKind) m_pCard->enableDisp(FALSE);
        } else {
            m_uiFlag |= 0x2008;
            if (m_uiFlag & 0x1) {
                m_uiFlag |= 0x80;
                if (m_cardData.m_nKind) m_pCard->enableDisp(TRUE);
            } else {
                m_uiFlag = (INT)m_uiFlag & ~0x80;
                if (m_cardData.m_nKind) m_pCard->enableDisp(FALSE);
            }
        }
    }
}
void CBattleCard::changeState(INT inState) {
    KH_DEBUG(if (!(inState >= 0 && inState < 7)) AssertApp(__FILE__, 492, "0 <= inState && inState < STATE_MAX", 0);)
    gotoState(m_State[inState]);
}
void CBattleCard::setPlayCard() {
    changeState(0);
    m_uiMode = 1;
    m_iHand = -1;
    m_uiFlag = (INT)(m_uiFlag | 0x40) & ~0x2;
}
void CBattleCard::setUseEnd() { setPlayEnd(); }
void CBattleCard::setPlayEnd() {
    switch (m_iState) {
    case 0:
    case 1:
    case 4:
    case 6:
        changeState(3);
        m_uiMode = 3;
        m_tqTimer = 400;
        m_uiFlag = (INT)m_uiFlag & ~0x2;
        break;
    }
}
void CBattleCard::preStateUse() {
    CB_LAYOUT* pLayout;
    m_iState = 0;
    m_uiFlag |= 0x1;
    switch (m_uiUser) {
    case 1: pLayout = &g_LayoutData[7]; break;
    case 2: pLayout = &g_LayoutData[16]; break;
    }
    m_StartPos.set(pLayout->x, pLayout->y);
    m_fStartScale = pLayout->scale * 0.01f;
    if (getCardCategory() == 14) pLayout = &g_LayoutData[58];
    else pLayout = &g_LayoutData[1];
    m_EndPos.set(pLayout->x, pLayout->y);
    m_fEndScale = pLayout->scale * 0.01f;
    m_tqTimer = 400;
    m_Pos = m_StartPos;
    m_vRot.w = m_fStartScale;
    m_iLayer = pLayout->layer;
    m_pCard->changeLayer(m_iLayer);
}
void CBattleCard::stateUse() {
    switch (m_uiMode) {
    case 1: {
        ((CCardBattle*)CTaskMan::getSysTask(0x16))->checkNowPlaying(m_uiUser, this);
        m_tqTimer -= getFrameRate();
        if (m_tqTimer <= 0) m_tqTimer = 0;
        INT dx = (m_EndPos.x - m_StartPos.x) * m_tqTimer / 400;
        INT dy = (m_EndPos.y - m_StartPos.y) * m_tqTimer / 400;
        FLOAT sc = m_fEndScale - (m_fEndScale - m_fStartScale) * m_tqTimer / 400.0f;
        m_vRot.w = sc;
        m_Pos.x = m_EndPos.x - dx;
        m_Pos.y = m_EndPos.y - dy;
        if (!isSet()) {
            if (abs(dx) <= 64) m_uiFlag |= 0x2;
        } else if (!m_tqTimer) {
            m_uiMode = 2;
            struct _bits { u_long b0 : 28; u_long cat1 : 4; };
            if (((_bits*)&m_cardData)->cat1 == 0xe) {
                if (m_pRevolver) m_pRevolver->setTrapEffect();
                m_uiMode = 0x13;
                m_tqTimer = 800;
            }
        }
        ((CCardBattle*)CTaskMan::getSysTask(0x16))->checkNowPlaying(m_uiUser, this);
        break;
    }
    case 2:
        if (!((CCardBattle*)CTaskMan::getSysTask(0x16))->checkNowPlaying(m_uiUser, this)) destroy(1);
        break;
    case 0x13:
        m_tqTimer -= getFrameRate();
        if (m_tqTimer <= 0) {
            m_tqTimer = 0;
            m_cardData.init(0x43, 0);
            m_pCard->destroy();
            m_pCard->create(m_cardData);
            m_iLayer = g_LayoutData[58].layer;
            m_pCard->changeLayer(m_iLayer);
            m_uiMode = 2;
        }
        break;
    }
}
void CBattleCard::setRefrect() {
    changeState(3);
    if (m_uiMode == 16) m_iState = 8;
    m_uiMode = 4;
    m_uiFlag = (INT)m_uiFlag & ~0x2;
    if (m_iStock && m_cardData.m_nCategory1 == 4) m_uiFlag = (INT)m_uiFlag & ~0x20;
}
void CBattleCard::setBreak(INT inFlag) {
    changeState(3);
    m_uiMode = 6;
    m_tqTimer = 1200;
    m_uiFlag = (INT)m_uiFlag & ~0x2;
    if (inFlag > 0) m_uiFlag |= 0x20;
}
void CBattleCard::destroy(INT inFlag) {
    if (m_uiMode < 8) {
        if (m_uiMode >= 3) return;
    }
    if (m_pRevolver) {
        TRACE("BC::destroy(); handle=%d\n", getHandle());
        m_uiFlag = (INT)m_uiFlag & ~0x1;
        m_uiFlag = (INT)m_uiFlag & ~0x2;
        m_uiFlag = (INT)m_uiFlag & ~0x4;
        m_uiFlag = (INT)m_uiFlag & ~0x40;
        m_uiFlag &= ~0x10000;
        m_uiFlag &= ~0x200000;
        m_uiFlag = (INT)m_uiFlag & ~0x10;
        m_uiMode = 0xe;
        m_iHand = -1;
        m_iStock = -1;
        m_pCard->enableColorCtrl(FALSE);
        m_vRot.set(0.0f, 0.0f, 0.0f, 1.0f);
        m_uiFlag = (INT)m_uiFlag & ~0x2008;
        changeState(2);
        resetEnchantEffect(0);
        if (KH_DW(DebugWork::g_dw.player.iCardSpend)) return;
        if (!inFlag) return;
        switch (getCardCategory()) {
        case 0xe:
            m_pRevolver->exeTrapEffect(0);
            CCardBattle::addGimmickCount(-1);
            __asm__ volatile(""); /* MATCH: keeps this copy: jump2 would cross-jump it into case 6's */
            break;
        case 6:
            CCardBattle::addGimmickCount(-1);
            break;
        case 5:
            if (m_cardData.m_nKind == 0x39) CCardBattle::addPlutoCount(-1);
            break;
        default:
            return;
        }
        m_uiFlag |= 0x20;
    } else {
        out();
    }
}
void CBattleCard::setEnchant() {
    changeState(3);
    m_uiMode = 7;
    m_tqTimer = 1200;
    m_iHand = -1;
}
void CBattleCard::setStock(UINT inPri) {
    changeState(1);
    m_iStock = inPri;
    m_uiMode = 8;
    m_iHand = -1;
    m_uiFlag = (INT)m_uiFlag & ~0x2;
    setEnchantEffect(0x44, 0);
    setEnchantEffect(0x52, 0);
    setEnchantEffect(0x53, 0);
}
void CBattleCard::setPlayStock(INT inCount) {
    CB_LAYOUT* pLayout;
    m_uiMode = 1;
    m_iStockCount = inCount;
    switch (m_uiUser) {
    case 1: pLayout = &g_LayoutData[27 + m_iStock]; break;
    case 2: pLayout = &g_LayoutData[33 + m_iStock]; break;
    }
    m_StartPos.set(pLayout->x, pLayout->y);
    m_fStartScale = pLayout->scale * 0.01f;
    switch (inCount) {
    case 0:
    case 1: pLayout = &g_LayoutData[1]; break;
    case 2: pLayout = &g_LayoutData[2 + m_iStock]; break;
    case 3: pLayout = &g_LayoutData[4 + m_iStock]; break;
    }
    m_EndPos.set(pLayout->x, pLayout->y);
    m_fEndScale = pLayout->scale * 0.01f;
    m_tqTimer = 400;
    m_Pos = m_StartPos;
    m_vRot.w = m_fStartScale;
    m_iLayer = pLayout->layer;
    m_pCard->changeLayer(m_iLayer);
    m_uiFlag = (INT)(m_uiFlag | 0x40) & ~0x2;
    if (m_uiFlag & 0x10) {
        m_uiFlag = (INT)m_uiFlag & ~0x10;
        m_pCard->enableColorCtrl(FALSE);
    }
    if (m_pRevolver) {
        struct _bits { u_long b0 : 28; u_long cat1 : 4; };
        if (!m_iStock) {
            CCardBattle* pCB = (CCardBattle*)CTaskMan::getSysTask(0x16);
            if (KH_DW(DebugWork::g_dw.player.iCardSpend)) return;
            INT enchant = pCB->getEnchantKind(m_uiUser);
            if (enchant == 0x2b) {
                if (getCardCategory() < 4 && getCardCategory()) return;
            } else if (((_bits*)&m_cardData)->cat1 == 1) {
                if (m_pRevolver->getHave(1, 1, 1, 0, 0) == 1) return;
            }
        } else {
            if (KH_DW(DebugWork::g_dw.player.iCardSpend)) return;
            if (getCardCategory() >= 6) return;
            if (getCardCategory() < 4) return;
        }
    }
    m_uiFlag |= 0x20;
}
void CBattleCard::preStateStock() {
    CB_LAYOUT* pLayout;
    m_iState = 1;
    m_uiFlag |= 0x1;
    switch (m_uiUser) {
    case 1:
        pLayout = &g_LayoutData[7];
        m_StartPos.set(pLayout->x, pLayout->y);
        m_fStartScale = pLayout->scale * 0.01f;
        pLayout = &g_LayoutData[24 + m_iStock];
        if (m_pRevolver->getStockSkill() >= 4) pLayout = &g_LayoutData[27 + m_iStock];
        m_EndPos.set(pLayout->x, pLayout->y);
        m_fEndScale = pLayout->scale * 0.01f;
        break;
    case 2:
        pLayout = &g_LayoutData[16];
        m_StartPos.set(pLayout->x, pLayout->y);
        m_fStartScale = pLayout->scale * 0.01f;
        pLayout = &g_LayoutData[30 + m_iStock];
        if (m_pRevolver && m_pRevolver->getStockSkill() >= 0x6f) pLayout = &g_LayoutData[33 + m_iStock];
        else if (CCardBattle::getStockFolderStockSkill(2) >= 0x6f) pLayout = &g_LayoutData[33 + m_iStock];
        m_EndPos.set(pLayout->x, pLayout->y);
        m_fEndScale = pLayout->scale * 0.01f;
        break;
    }
    m_tqTimer = 400;
    m_Pos = m_StartPos;
    m_vRot.w = m_fStartScale;
    m_iLayer = pLayout->layer;
    m_pCard->changeLayer(m_iLayer);
}
void CBattleCard::stateStock() {
    CB_LAYOUT* pLayout;
    switch (m_uiMode) {
    case 2:
        break;
    case 8:
        m_uiFlag |= 0x2008;
        m_tqTimer -= getFrameRate();
        if (m_tqTimer <= 0) {
            m_uiMode = 9;
            m_uiFlag |= 0x4;
            m_tqTimer = 0;
            m_Pos = m_EndPos;
            m_vRot.w = m_fEndScale;
            if (m_pRevolver) m_pRevolver->calcStockSkill();
            else CCardBattle::calcStockFolderEnemyStockSkill();
        } else {
            INT dx = (m_EndPos.x - m_StartPos.x) * m_tqTimer / 400;
            INT dy = (m_EndPos.y - m_StartPos.y) * m_tqTimer / 400;
            FLOAT sc = m_fEndScale - (m_fEndScale - m_fStartScale) * m_tqTimer / 400.0f;
            m_vRot.w = sc;
            m_Pos.x = m_EndPos.x - dx;
            m_Pos.y = m_EndPos.y - dy;
        }
        break;
    case 9:
        m_tqTimer += getFrameRate();
        {
            FLOAT r = m_tqTimer * 0.1f * 3.1415927f / 180.0f;
            m_Pos.y = (INT)(sinf(r) * 4.0f) + m_EndPos.y;
        }
        if (m_uiUser == 2) {
            CCardBattle* pCB = (CCardBattle*)CTaskMan::getSysTask(0x16);
            INT enchant = pCB->getEnchantKind(m_uiUser);
            if (enchant == 0x31) {
                if (!(m_uiFlag & 0x10)) {
                    m_uiFlag |= 0x10;
                    if (m_pRevolver) m_pRevolver->setStockInfo();
                    else CCardBattle::setStockFolderStockInfo(2);
                    FVECTOR col;
                    col._q = 0;
                    col.w = 1.0f;
                    m_pCard->enableColorCtrl(TRUE);
                    m_pCard->setCol(col);
                }
            } else if (m_uiFlag & 0x10) {
                m_uiFlag = (INT)m_uiFlag & ~0x10;
                if (m_pRevolver) m_pRevolver->setStockInfo();
                else CCardBattle::setStockFolderStockInfo(2);
                m_pCard->enableColorCtrl(FALSE);
            }
            if (m_pRevolver && m_pRevolver->getStockSkill() >= 0x6f) {
                pLayout = &g_LayoutData[33 + m_iStock];
                m_EndPos.set(pLayout->x, pLayout->y);
            } else if (CCardBattle::getStockFolderStockSkill(2) >= 0x6f) {
                pLayout = &g_LayoutData[33 + m_iStock];
                m_EndPos.set(pLayout->x, pLayout->y);
            }
        } else {
            if (m_pRevolver->getStockSkill() >= 4) {
                pLayout = &g_LayoutData[27 + m_iStock];
                m_EndPos.set(pLayout->x, pLayout->y);
            }
        }
        break;
    case 1: {
        m_uiFlag |= 0x2008;
        m_tqTimer -= getFrameRate();
        if (m_tqTimer <= 0) m_tqTimer = 0;
        INT dx = (m_EndPos.x - m_StartPos.x) * m_tqTimer / 400;
        INT dy = (m_EndPos.y - m_StartPos.y) * m_tqTimer / 400;
        FLOAT sc = m_fEndScale - (m_fEndScale - m_fStartScale) * m_tqTimer / 400.0f;
        m_vRot.w = sc;
        m_Pos.y = m_EndPos.y - dy;
        m_Pos.x = m_EndPos.x - dx;
        if (!isSet()) {
            if (abs(dx) <= 64 && abs(dy) <= 64) m_uiFlag |= 0x2;
        } else if (!m_tqTimer) {
            m_uiMode = 2;
        }
        break;
    }
    }
}
void CBattleCard::preStateHand() { m_iState = 2; }
void CBattleCard::stateHand() {
    switch (m_uiMode) {
    case 10:
        break;
    case 11: {
        m_tqTimer += getFrameRate();
        FLOAT r = m_tqTimer * 0.1f * 3.1415927f / 180.0f;
        m_Pos.y = (INT)(sinf(r) * 4.0f) + m_EndPos.y;
        break;
    }
    case 12:
        m_uiFlag |= 0x2008;
        m_tqTimer -= getFrameRate();
        if (m_tqTimer <= 0) {
            m_tqTimer = (RND::genrand_int31() % 100) * 100;
            m_uiMode = 11;
            if (m_iHand < 3) {
                if (m_iHand >= 0) goto hand_ok1;
            }
            if (m_iHand != m_iHandMax - 1) {
                m_tqTimer = 0;
                m_uiMode = 10;
                if (!KH_DW(DebugWork::g_dw.player.iDeckDbgDisp)) m_uiFlag = (INT)m_uiFlag & ~0x1;
            }
        hand_ok1:
            m_Pos = m_EndPos;
            m_vRot.set(0.0f, m_fEndDir, 0.0f, m_fEndScale);
        } else if (m_pRevolver) {
            moveCard(m_pRevolver->getRollSpeed(), m_pRevolver->getRollSpeed());
        }
        break;
    case 13:
        m_tqTimer -= getFrameRate();
        if (m_tqTimer <= 0) {
            m_tqTimer = 0;
            m_uiFlag = (INT)m_uiFlag & ~0x1;
            m_uiMode = 10;
            m_Pos = m_EndPos;
            m_vRot.set(0.0f, m_fEndDir, 0.0f, m_fEndScale);
            m_uiFlag |= 0x2008;
        } else {
            moveCard(400, 0);
        }
        break;
    case 14:
        break;
    case 15:
        if (m_iHand < 4) {
            if (m_iHand >= 0) {
                m_uiFlag |= 0x2008;
                goto timer;
            }
        }
        m_uiFlag |= 0x2008;
        if (m_iHand != m_iHandMax - 2 && m_iHand != m_iHandMax - 1 && !(m_uiFlag & 0x100000)) {
            m_uiFlag |= 0x100000;
            m_uiMode = 10;
            m_tqTimer = 0;
            if (!KH_DW(DebugWork::g_dw.player.iDeckDbgDisp)) {
                m_uiFlag = (INT)m_uiFlag & ~0x1;
                m_uiFlag = (INT)m_uiFlag & ~0x2008;
            }
        }
    timer:
        m_tqTimer -= getFrameRate();
        if (m_tqTimer <= 0) {
            m_iStep++;
            if (m_iHand < m_iStep) {
                m_tqTimer = (RND::genrand_int31() % 100) * 100;
                m_uiMode = 11;
                if (m_iHand < 3) {
                    if (m_iHand >= 0) goto hand_ok2;
                }
                if (m_iHand != m_iHandMax - 1) {
                    m_tqTimer = 0;
                    m_uiMode = 10;
                    if (!KH_DW(DebugWork::g_dw.player.iDeckDbgDisp)) m_uiFlag = (INT)m_uiFlag & ~0x1;
                }
        hand_ok2:
                m_Pos = m_EndPos;
                m_vRot.set(0.0f, m_fEndDir, 0.0f, m_fEndScale);
            } else {
                m_StartPos = m_EndPos;
                setHandLocate(m_iStep, m_iHandMax);
                m_Pos = m_StartPos;
                m_uiMode = 15;
                m_tqTimer = 200;
                m_vRot.set(0.0f, m_fEndDir, 0.0f, m_fEndScale);
                if (m_uiUser == 2) {
                    if (m_iStep == 1) {
                        if (m_iHand > 0) {
                            m_vRot.y = 0.0f;
                            m_fEndDir = 3.1415927f;
                            m_fStartDir = 0.0f;
                        }
                    }
                }
            }
        } else {
            moveCard(200, 300);
        }
        break;
    }
}
void CBattleCard::preStateDestroy() {
    CB_LAYOUT* pLayout;
    if (m_iState == 8 || m_iState == 2) {
        if (m_iStock == -1) {
            switch (m_uiUser) {
            case 1: pLayout = &g_LayoutData[7]; break;
            case 2: pLayout = &g_LayoutData[16]; break;
            }
            m_StartPos.set(pLayout->x, pLayout->y);
            m_fStartScale = pLayout->scale * 0.01f;
            pLayout = &g_LayoutData[1];
            m_EndPos.set(pLayout->x, pLayout->y);
            m_fEndScale = pLayout->scale * 0.01f;
        } else {
            switch (m_uiUser) {
            case 1: pLayout = &g_LayoutData[27 + m_iStock]; break;
            case 2: pLayout = &g_LayoutData[33 + m_iStock]; break;
            }
            m_StartPos.set(pLayout->x, pLayout->y);
            m_fStartScale = pLayout->scale * 0.01f;
            switch (m_iStockCount) {
            case 0:
            case 1: pLayout = &g_LayoutData[1]; break;
            case 2: pLayout = &g_LayoutData[2 + m_iStock]; break;
            case 3: pLayout = &g_LayoutData[4 + m_iStock]; break;
            }
            m_EndPos.set(pLayout->x, pLayout->y);
            m_fEndScale = pLayout->scale * 0.01f;
        }
        if (m_uiMode != 7) m_tqTimer = 400;
        m_Pos = m_StartPos;
        m_vRot.w = m_fStartScale;
        m_iLayer = pLayout->layer;
        m_pCard->changeLayer(m_iLayer);
    }
    m_iState = 3;
    m_uiFlag |= 0x1;
    m_iStock = -1;
    m_iStep = RND::genrand_int31() % 100;
}
void CBattleCard::stateDestroy() {
    switch (m_uiMode) {
    case 3:
        m_uiFlag |= 0x2008;
        m_tqTimer -= getFrameRate();
        if (m_tqTimer <= 0) {
            m_tqTimer = 0;
            m_uiMode = 0;
            destroy(1);
            return;
        }
        m_vRot.z = _adjustRad(m_vRot.z + getFrameRateF() * 5.0f * 3.1415927f / 180.0f);
        m_vRot.w = m_tqTimer / 400.0f;
        break;
    case 4: {
        m_uiFlag |= 0x2008;
        m_tqTimer -= getFrameRate();
        if (m_tqTimer <= 0) m_tqTimer = 0;
        INT dx = (m_EndPos.x - m_StartPos.x) * m_tqTimer / 400;
        INT dy = (m_EndPos.y - m_StartPos.y) * m_tqTimer / 400;
        m_Pos.x = m_EndPos.x - dx;
        m_Pos.y = m_EndPos.y - dy;
        if (m_vRot.w < 1.0f) m_vRot.w += getFrameRate() * 0.5f / 400.0f;
        if (m_vRot.w > 1.0f) m_vRot.w = 1.0f;
        if (abs(dx) <= 64 && abs(dy) <= 64) {
            m_tqTimer = 1000;
            m_uiMode = 5;
        }
        break;
    }
    case 5:
        m_uiFlag |= 0x2008;
        m_tqTimer -= getFrameRate();
        if (m_tqTimer <= 0) {
            m_tqTimer = 0;
            m_uiMode = 0;
            destroy(1);
            return;
        }
        m_Pos.x -= m_tqTimer / 200;
        m_Pos.y += (750 - m_tqTimer) * 60 / 1000;
        m_vRot.y = _adjustRad(m_vRot.y - getFrameRateF() * 5.0f * 3.1415927f / 180.0f);
        m_vRot.z = _adjustRad(m_vRot.z - getFrameRateF() * 2.0f * 3.1415927f / 180.0f);
        break;
    case 6:
        m_uiFlag |= 0x2008;
        m_tqTimer -= getFrameRate();
        if (m_tqTimer <= 0) {
            m_tqTimer = 0;
            m_uiMode = 0;
            destroy(1);
            return;
        }
        m_Pos.x -= ((m_iStep & 0xf) + 15) * m_tqTimer / 1200 * (1 - (m_uiUser & 0xfe));
        m_Pos.y += (m_iStep + 80) * (700 - m_tqTimer) / 1200;
        m_vRot.y = _adjustRad(m_vRot.y - getFrameRateF() * 3.0f * 3.1415927f / 180.0f);
        m_vRot.z = _adjustRad(m_vRot.z + getFrameRateF() * 30.0f * 3.1415927f / 180.0f);
        break;
    case 7:
        m_uiFlag |= 0x2008;
        m_tqTimer -= getFrameRate();
        if (m_tqTimer <= 0) {
            m_vRot.w = 0.0f;
            m_tqTimer = 0;
            m_uiMode = 0;
            destroy(1);
            return;
        }
        if (m_tqTimer < 600) {
            m_vRot.w = m_tqTimer * 2.0f / 1200.0f;
        } else {
            m_Pos.x += m_tqTimer / 240 * (1 - (m_uiUser & 0xfe));
            m_Pos.y -= m_tqTimer / 60;
        }
        break;
    }
}
void CBattleCard::moveCard(TIMEQ inTime, TIMEQ inRoll) {
    FLOAT sc;
    if (!inTime) return;
    INT x = m_EndPos.x - (m_EndPos.x - m_StartPos.x) * m_tqTimer / inTime;
    INT y = m_EndPos.y - (m_EndPos.y - m_StartPos.y) * m_tqTimer / inTime;
    sc = m_fEndScale - (m_fEndScale - m_fStartScale) * m_tqTimer / inTime;
    m_vRot.w = sc;
    m_Pos.x = x;
    m_Pos.y = y;
    if (!inRoll) return;
    m_vRot.y = m_fEndDir - (m_fEndDir - m_fStartDir) * m_tqTimer / inRoll;
}
INT CBattleCard::isNowSelect() {
    if (m_uiMode != 11) return FALSE;
    return m_iHand == 0;
}
void CBattleCard::setHand(const CARD& inData, INT inNum, INT inMax, INT inMode) {
    if (inMode) {
        m_StartPos = m_Pos;
    } else {
        m_pCard->destroy();
        if (!inData.m_nKind) {
            m_uiFlag |= 0x20;
            m_pCard->enableDisp(FALSE);
            m_cardData = inData;
            m_iHandMax = 0;
            m_iHand = -1;
            m_uiFlag = (INT)m_uiFlag & ~0x1;
            m_uiMode = 10;
            return;
        }
        m_pCard->create(inData);
        FVECTOR pos = {{m_Pos.x, m_Pos.y, 0.0f, 1.0f}};
        m_pCard->setPos(pos);
        m_pCard->enableDisp(FALSE);
        m_uiFlag = (INT)m_uiFlag & ~0x20;
        if (inData.m_nKind == 0xa0) m_pCard->changeTime(0);
    }
    m_cardData = inData;
    if (!inMode && !(m_uiFlag & 0x1e00)) m_nNumber = m_cardData.m_nNumber;
    m_iHand = inNum;
    m_iHandMax = inMax;
    m_uiFlag |= 0x1;
    m_uiMode = 0xb;
    setHandLocate(m_iHand, m_iHandMax);
    if (inMode) {
        m_Pos = m_StartPos;
        m_uiMode = 0xc;
        if (m_pRevolver) m_tqTimer = m_pRevolver->getRollSpeed();
    }
    setRoomEffect(NULL);
    {
        CARD* pEnchant = CCardBattle::getEnchantCard(m_uiUser);
        if (!pEnchant || pEnchant->m_nKind != 0x4b) {
            setEnchantEffect(0x4b, 0);
            setEnchantEffect(0x57, 0);
        } else {
            setEnchantEffect(0x57, 0);
            setEnchantEffect(0x4b, 0);
        }
    }
    {
        /* MATCH: 64-bit bit-field category read, see updateNumberEffect */
        struct _bits { u_long b0 : 28; u_long cat1 : 4; };
        if (((_bits*)&m_cardData)->cat1 == 4) m_uiFlag |= 0x8000;
    }
}
void CBattleCard::setHandLocate(INT inNum, INT inMax) {
    CB_LAYOUT* pLayout;
    INT n = 0;
    if (m_uiUser == 1) {
        if ((inMax >= 3 && inNum == inMax - 1) || inNum == -1) {
            pLayout = &g_LayoutData[11];
        } else {
            if (inNum < 4) {
                if (inNum >= 0) {
                    pLayout = &g_LayoutData[7 + inNum];
                    goto set1;
                }
            }
            m_tqTimer = 0;
            m_uiMode = 10;
            if (KH_DW(DebugWork::g_dw.player.iDeckDbgDisp)) {
                n = inNum * 25;
                pLayout = &g_LayoutData[15];
            } else {
                m_uiFlag = (INT)m_uiFlag & ~0x1;
                if (inNum == inMax - 2) pLayout = &g_LayoutData[12];
                else pLayout = &g_LayoutData[13];
            }
        }
    set1:
        m_Pos.set(pLayout->x, pLayout->y);
        m_iLayer = pLayout->layer;
        m_Pos.x += n;
        m_vRot.set(0.0f, 0.0f, 0.0f, pLayout->scale * 0.01f);
        m_fStartDir = 0.0f;
        m_fEndDir = 0.0f;
        m_fEndScale = m_fStartScale = m_vRot.w;
    } else {
        if (inNum == 0) {
            pLayout = &g_LayoutData[16];
            m_fEndDir = 0.0f;
            m_vRot.y = 3.1415927f;
            m_fStartDir = 3.1415927f;
        } else if (inNum >= 0) {
            if (inNum < 4) {
                pLayout = &g_LayoutData[16 + inNum];
                m_vRot.y = 3.1415927f;
                m_fStartDir = 3.1415927f;
                m_fEndDir = 3.1415927f;
            } else goto other;
        } else other: if (inNum == inMax - 1) {
            pLayout = &g_LayoutData[20];
            m_vRot.y = 3.1415927f;
            m_fStartDir = 3.1415927f;
            m_fEndDir = 3.1415927f;
        } else {
            m_tqTimer = 0;
            m_uiMode = 10;
            if (KH_DW(DebugWork::g_dw.player.iDeckDbgDisp)) {
                n = inNum * 35;
                pLayout = &g_LayoutData[23];
            } else {
                m_uiFlag = (INT)m_uiFlag & ~0x1;
                pLayout = &g_LayoutData[21];
            }
            m_vRot.y = 0.0f;
            m_fStartDir = 0.0f;
            m_fEndDir = 0.0f;
        }
        m_Pos.set(pLayout->x, pLayout->y);
        m_iLayer = pLayout->layer;
        m_vRot.x = m_vRot.z = 0.0f;
        m_fEndScale = m_fStartScale = m_vRot.w = pLayout->scale * 0.01f;
        m_Pos.y -= n;
    }
    m_EndPos = m_Pos;
    m_pCard->changeLayer(m_iLayer);
    m_uiFlag |= 0x2008;
}
void CBattleCard::hideHand() {
    m_uiMode = 13;
    m_tqTimer = 400;
    m_StartPos = m_Pos;
    m_fStartScale = m_vRot.w;
    CB_LAYOUT* pLay = m_uiUser == 1 ? &g_LayoutData[13] : &g_LayoutData[21];
    m_EndPos.set(pLay->x, pLay->y);
    m_fEndScale = pLay->scale * 0.01f;
}
void CBattleCard::outHand() {
    m_uiMode = 14;
    m_iHand = -1;
    m_uiFlag = (INT)m_uiFlag & ~0x1;
    m_uiFlag = (INT)m_uiFlag & ~0x2008;
}
void CBattleCard::updateReload(INT inNum) {
    if (m_cardData.m_nKind == 0xa0) m_pCard->changeTime(inNum);
}
void CBattleCard::resetHand(INT inNum, INT inMax) {
    CB_LAYOUT* pLayout;
    changeState(2);
    m_iHand = inNum;
    m_iHandMax = inMax;
    m_iStock = -1;
    if (m_uiUser == 1) {
        pLayout = &g_LayoutData[14];
        m_StartPos.set(pLayout->x, pLayout->y);
        m_fStartScale = pLayout->scale * 0.01f;
        pLayout = &g_LayoutData[7];
        m_EndPos.set(pLayout->x, pLayout->y);
        m_fEndScale = pLayout->scale * 0.01f;
    } else {
        pLayout = &g_LayoutData[22];
        m_StartPos.set(pLayout->x, pLayout->y);
        m_fStartScale = pLayout->scale * 0.01f;
        pLayout = &g_LayoutData[16];
        m_EndPos.set(pLayout->x, pLayout->y);
        m_fEndScale = pLayout->scale * 0.01f;
    }
    m_tqTimer = 200;
    m_Pos = m_StartPos;
    m_vRot.w = m_fStartScale;
    m_iLayer = pLayout->layer;
    m_pCard->changeLayer(m_iLayer);
    m_uiMode = 0xf;
    m_iStep = 0;
    m_uiFlag = (m_uiFlag | 1) & ~0x210044;
    testDisp();
    m_uiFlag = ((INT)m_uiFlag | 0x4000) & ~0x10;
    m_pCard->enableColorCtrl(FALSE);
    resetEnchantEffect(0);
}
void CBattleCard::setSelect(INT inFlag) {
    if (m_pCard) {
        if (inFlag) {
            if (isPlayNG()) m_pCard->select(TRUE);
            else m_pCard->select(FALSE);
        } else {
            m_pCard->unselect();
        }
    }
}
void CBattleCard::setRoomEffect(const CARD* inData) {
    const CARD* pRoom;
    struct _bits { u_long b0 : 24; u_long cat0 : 4; };
    INT cat = m_cardData.m_nCategory0;
    if (!cat) return;
    if (cat >= 6) {
        if (cat != 7) return;
        if (m_cardData.m_nNumber == -1) return;
    }
    if (m_uiFlag & 0x100) return;
    if (!(m_uiFlag & 0x1e00)) m_nNumber = m_cardData.m_nNumber;
    m_nNumberR = -1;
    pRoom = inData;
    if (!pRoom) {
        pRoom = CCardBattle::getRoomCard();
        if (!pRoom) return;
    }
    switch (pRoom->m_nKind) {
    case 0xa3:
        if (m_uiUser == 2) {
            m_nNumberR = m_nNumber - 2;
            if (m_nNumberR <= 0) m_nNumberR = 1;
        }
        break;
    case 0xa4:
        if (m_uiUser == 2) {
            m_nNumberR = m_nNumber + 2;
            if (m_nNumberR >= 10) m_nNumberR = 9;
        }
        break;
    case 0xac:
        if (m_uiUser == 1 && ((_bits*)&m_cardData)->cat0 == 1) {
            m_nNumberR = m_nNumber + 2;
            if (m_nNumberR >= 10) m_nNumberR = 9;
        }
        break;
    case 0xad:
        if (m_uiUser == 1 && (((_bits*)&m_cardData)->cat0 == 2 || ((_bits*)&m_cardData)->cat0 == 3)) {
            m_nNumberR = m_nNumber + 2;
            if (m_nNumberR >= 10) m_nNumberR = 9;
        }
        break;
    case 0xae:
        if (m_uiUser == 1 && ((_bits*)&m_cardData)->cat0 == 4) {
            m_nNumberR = m_nNumber + 2;
            if (m_nNumberR >= 10) m_nNumberR = 9;
        }
        break;
    default:
        return;
    }
    if (m_nNumberR != -1) {
        m_cardData.m_nNumber = m_nNumberR;
        m_pCard->replaceNumberBlue(m_nNumberR);
        m_uiFlag |= 0x100;
    }
}
void CBattleCard::setEnchantEffect(INT inKind, UINT inFlag) {
    INT cat = m_cardData.m_nCategory0;
    if (!cat) return;
    if (cat >= 6) {
        if (cat != 7) return;
        if (m_cardData.m_nNumber < 0) return;
    }
    {
        CARD* pEnchant = CCardBattle::getEnchantCard(m_uiUser);
        if (!pEnchant || pEnchant->m_nKind != inKind) {
            if (inKind == 0x4b || inKind == 0x57) resetEnchantEffect(inKind);
            return;
        }
    }
    switch (inKind) {
    case 0x44:
        if (m_cardData.m_nNumber < 9) m_cardData.m_nNumber++;
        if (m_pRevolver) m_pRevolver->setNumberEffect(0x44);
        m_nNumberE = m_cardData.m_nNumber;
        m_pCard->replaceNumberBlue(m_nNumberE);
        m_uiFlag |= 0x200;
        break;
    case 0x53:
        if (m_cardData.m_nNumber > 0) m_cardData.m_nNumber--;
        if (m_pRevolver) m_pRevolver->setNumberEffect(0x53);
        m_nNumberE = m_cardData.m_nNumber;
        m_pCard->replaceNumberBlue(m_nNumberE);
        m_uiFlag |= 0x200;
        break;
    case 0x52:
        if (!inFlag) {
            m_nNumberE = RND::genrand_int31() % 10;
            m_cardData.m_nNumber = m_nNumberE;
            m_pCard->replaceNumberBlue(m_nNumberE);
            m_uiFlag = ((INT)m_uiFlag & ~0x400) | 0x200;
        } else {
            if (m_uiFlag & 0x400) break;
            m_pCard->replaceNumberRoulette();
            m_uiFlag |= 0x400;
        }
        break;
    case 0x4b: {
        if (m_uiFlag & 0x800) break;
        m_nNumberE = (10 - m_nNumber) % 10;
        m_cardData.m_nNumber = m_nNumberE;
        m_pCard->replaceNumberBlue(m_nNumberE);
        m_uiFlag |= 0x800;
        break;
    }
    case 0x57:
        if (m_uiFlag & 0x1000) break;
        m_cardData.m_nNumber = 0;
        m_nNumberE = 0;
        m_pCard->replaceNumberBlue(0);
        m_uiFlag |= 0x1000;
        break;        break;
    case 0x65:
        break;
    }
}
void CBattleCard::resetEnchantEffect(INT inKind) {
    UINT uiMask;
    switch (inKind) {
    case 0x52: uiMask = 0x400; break;
    case 0x4b: uiMask = 0x800; break;
    case 0x57: uiMask = 0x1000; break;
    case 0: uiMask = 0x1e00; break;
    default: uiMask = 0x200; break;
    }
    if (m_uiFlag & uiMask) {
        if (!(m_uiFlag & 0x100)) {
            m_cardData.m_nNumber = m_nNumber;
            m_pCard->replaceNumberNormal(m_nNumber);
        } else {
            m_cardData.m_nNumber = m_nNumberR;
            m_pCard->replaceNumberBlue(m_nNumberR);
        }
        m_uiFlag &= ~uiMask;
    }
}
void CBattleCard::setNumberBreak(INT inNum) {
    m_cardData.m_nNumber -= inNum;
    if (m_cardData.m_nNumber < 0) m_cardData.m_nNumber = 0;
    m_nNumberE = m_cardData.m_nNumber;
    m_pCard->replaceNumberBlue(m_nNumberE);
    m_uiFlag |= 0x200;
}
void CBattleCard::updateNumberEffect() {
    if (!(m_uiFlag & 0x100)) {
        m_cardData.m_nNumber = m_nNumber;
        m_pCard->replaceNumberNormal(m_nNumber);
    } else {
        m_cardData.m_nNumber = m_nNumberR;
        m_pCard->replaceNumberBlue(m_nNumberR);
    }
    resetEnchantEffect(0);
    {
        CARD* pEnchant = CCardBattle::getEnchantCard(m_uiUser);
        if (!pEnchant || pEnchant->m_nKind != 0x4b) {
            setEnchantEffect(0x4b, 0);
            setEnchantEffect(0x57, 0);
        } else {
            setEnchantEffect(0x57, 0);
            setEnchantEffect(0x4b, 0);
        }
    }
    {
        /* MATCH: the category is read through a 64-bit bit-field over m_cardData (ld + mask); getCardCategory()'s
           BYTE bit-field gives lbu, and the header form changes Game/CardRevolver */
        struct _bits { u_long b0 : 28; u_long cat1 : 4; };
        if (((_bits*)&m_cardData)->cat1 == 4) m_uiFlag |= 0x8000;
    }
}
void CBattleCard::setDuel(INT inMode) {
    changeState(4);
    if (!inMode) m_uiMode = 16;
}
void CBattleCard::setDuelRefrect() {
    changeState(3);
    m_tqTimer = 1000;
    m_uiMode = 5;
    m_uiFlag = (INT)m_uiFlag & ~0x2;
    if (m_iStock && m_cardData.m_nCategory1 == 4) m_uiFlag = (INT)m_uiFlag & ~0x20;
}
void CBattleCard::setDuelStock(UINT inPri) {
    if (m_pRevolver) {
        changeState(4);
        m_iStock = inPri;
        m_uiMode = 8;
        m_iHand = -1;
        m_uiFlag = (INT)(m_uiFlag | 0x200000) & ~0x2;
        m_pRevolver->setPlayerDuelStock(this, m_iStock);
    }
}
void CBattleCard::setPlayDuelStock(INT inCount) {
    CB_LAYOUT* pLayout;
    m_uiMode = 1;
    m_iStockCount = inCount;
    pLayout = &g_LayoutData[36 + m_iStock];
    m_StartPos.set(pLayout->x, pLayout->y);
    m_fStartScale = pLayout->scale * 0.01f;
    switch (inCount) {
    case 0: pLayout = &g_LayoutData[1]; break;
    case 3: pLayout = &g_LayoutData[43 + m_iStock]; break;
    case 5: pLayout = &g_LayoutData[46 + m_iStock]; break;
    case 7: pLayout = &g_LayoutData[51 + m_iStock]; break;
    }
    m_EndPos.set(pLayout->x, pLayout->y);
    m_fEndScale = pLayout->scale * 0.01f;
    m_tqTimer = 400;
    m_Pos = m_StartPos;
    m_vRot.w = m_fStartScale;
    m_iLayer = pLayout->layer;
    m_pCard->changeLayer(m_iLayer);
    m_uiFlag = (INT)(m_uiFlag | 0x40) & ~0x2;
    if (m_uiFlag & 0x10) {
        m_uiFlag = (INT)m_uiFlag & ~0x10;
        m_pCard->enableColorCtrl(FALSE);
    }
    m_uiFlag &= ~0x200000;
}
void CBattleCard::preStateDuel() {
    CB_LAYOUT* pLayout;
    switch (m_uiMode) {
    case 0x10:
        if (m_iStock == -1) {
            switch (m_uiUser) {
            case 1: pLayout = &g_LayoutData[7]; break;
            case 2: pLayout = &g_LayoutData[16]; break;
            }
            m_StartPos.set(pLayout->x, pLayout->y);
            pLayout = &g_LayoutData[1];
            m_EndPos.set(pLayout->x, pLayout->y);
            m_fEndScale = pLayout->scale * 0.01f;
        } else {
            switch (m_uiUser) {
            case 1: pLayout = &g_LayoutData[27 + m_iStock]; break;
            case 2: pLayout = &g_LayoutData[33 + m_iStock]; break;
            }
            m_StartPos.set(pLayout->x, pLayout->y);
            switch (m_iStockCount) {
            case 0:
            case 1: pLayout = &g_LayoutData[1]; break;
            case 2: pLayout = &g_LayoutData[2 + m_iStock]; break;
            case 3: pLayout = &g_LayoutData[4 + m_iStock]; break;
            }
            m_EndPos.set(pLayout->x, pLayout->y);
            m_fEndScale = pLayout->scale * 0.01f;
        }
        m_fStartScale = m_fEndScale;
        m_StartPos.x = m_EndPos.x - (m_EndPos.x - m_StartPos.x) / 3;
        m_StartPos.y = m_EndPos.y - (m_EndPos.y - m_StartPos.y) / 3;
        m_tqTimer = 3000;
        m_Pos = m_StartPos;
        m_vRot.w = m_fEndScale;
        m_iLayer = pLayout->layer;
        m_pCard->changeLayer(m_iLayer);
        m_iStock = -1;
        m_uiFlag |= 0x40;
        break;
    case 8:
        m_uiFlag |= 0x1;
        pLayout = &g_LayoutData[1];
        m_StartPos.set(pLayout->x, pLayout->y);
        m_fStartScale = pLayout->scale * 0.01f;
        pLayout = &g_LayoutData[36 + m_iStock];
        m_EndPos.set(pLayout->x, pLayout->y);
        m_fEndScale = pLayout->scale * 0.01f;
        m_tqTimer = 400;
        m_Pos = m_StartPos;
        m_vRot.w = m_fStartScale;
        m_iLayer = pLayout->layer;
        m_pCard->changeLayer(m_iLayer);
        break;
    }
    m_iState = 4;
    m_uiFlag |= 0x1;
    if (m_uiFlag & 0x10) {
        m_uiFlag = (INT)m_uiFlag & ~0x10;
        m_pCard->enableColorCtrl(FALSE);
    }
}
void CBattleCard::stateDuel() {
    switch (m_uiMode) {
    case 16: {
        m_uiFlag |= 0x2008;
        m_tqTimer -= getFrameRate();
        if (m_tqTimer <= 0) m_tqTimer = 3000;
        INT tq = abs(m_tqTimer - 1500);
        INT x = (m_EndPos.x - m_StartPos.x) * tq / 1500;
        INT y = (m_EndPos.y - m_StartPos.y) * tq / 1500;
        m_Pos.x = m_EndPos.x - x;
        m_Pos.y = m_EndPos.y - y;
        if (abs(x) <= 32 && abs(y) <= 32) m_tqTimer >>= 1;
        break;
    }
    case 8:
        m_uiFlag |= 0x2008;
        m_tqTimer -= getFrameRate();
        if (m_tqTimer <= 0) {
            m_uiMode = 9;
            m_uiFlag |= 0x4;
            m_tqTimer = 0;
            m_Pos = m_EndPos;
            m_vRot.w = m_fEndScale;
        } else {
            INT dx = (m_EndPos.x - m_StartPos.x) * m_tqTimer / 400;
            INT dy = (m_EndPos.y - m_StartPos.y) * m_tqTimer / 400;
            FLOAT sc = m_fEndScale - (m_fEndScale - m_fStartScale) * m_tqTimer / 400.0f;
            m_vRot.w = sc;
            m_Pos.x = m_EndPos.x - dx;
            m_Pos.y = m_EndPos.y - dy;
        }
        break;
    case 9: {
        m_tqTimer += getFrameRate();
        FLOAT r = m_tqTimer * 0.1f * 3.1415927f / 180.0f;
        m_Pos.y = (INT)(sinf(r) * 4.0f) + m_EndPos.y;
        break;
    }
    case 1: {
        m_uiFlag |= 0x2008;
        m_tqTimer -= getFrameRate();
        if (m_tqTimer <= 0) m_tqTimer = 0;
        INT dx = (m_EndPos.x - m_StartPos.x) * m_tqTimer / 400;
        INT dy = (m_EndPos.y - m_StartPos.y) * m_tqTimer / 400;
        FLOAT sc = m_fEndScale - (m_fEndScale - m_fStartScale) * m_tqTimer / 400.0f;
        m_vRot.w = sc;
        m_Pos.y = m_EndPos.y - dy;
        m_Pos.x = m_EndPos.x - dx;
        if (!isSet()) {
            if (abs(dx) <= 64 && abs(dy) <= 64) m_uiFlag |= 0x2;
        } else if (!m_tqTimer) {
            m_uiMode = 2;
        }
        break;
    }
    case 2:
        break;
    }
}
void CBattleCard::setSnatch(HANDLE inHandle) {
    changeState(5);
    m_uiMode = 17;
    m_hTarget = inHandle;
    m_tqTimer = 800;
    m_iHand = -1;
    m_uiFlag |= 0x20;
}
void CBattleCard::resetSnatch() {
    m_uiOwner = 2;
    m_cardData.init(0, 0xff);
    m_uiFlag |= 0x20;
    m_uiFlag = (INT)m_uiFlag & ~0x1;
    m_uiFlag = (INT)m_uiFlag & ~0x2;
    m_uiFlag = (INT)m_uiFlag & ~0x4;
    m_uiFlag = (INT)m_uiFlag & ~0x40;
    m_iHand = -1;
    m_uiMode = 0xe;
    m_iStock = -1;
    m_uiFlag &= ~0x10000;
    m_iHandMax = 0;
    m_vRot.set(0.0f, 0.0f, 0.0f, 1.0f);
    m_uiFlag = (INT)m_uiFlag & ~0x2008;
    changeState(2);
    resetEnchantEffect(0);
}
void CBattleCard::preStateSnatch() {
    m_iState = 5;
    m_uiFlag |= 0x1;
    CB_LAYOUT& lay = g_LayoutData[7];
    m_StartPos.set(lay.x, lay.y);
    m_fStartScale = lay.scale * 0.01f;
    m_EndPos.set(lay.x + 50, lay.y - 100);
    m_fEndScale = m_fStartScale;
    m_tqTimer = 800;
    m_Pos = m_StartPos;
    m_vRot.w = m_fStartScale;
    m_iLayer = lay.layer;
    m_pCard->changeLayer(m_iLayer);
}
void CBattleCard::stateSnatch() {
    switch (m_uiMode) {
    case 0x11:
        m_uiFlag |= 0x2008;
        m_tqTimer -= getFrameRate();
        if (m_tqTimer <= 0) {
            m_tqTimer = 1600;
            m_uiMode = 0x12;
            m_Pos = m_EndPos;
            m_StartPos = m_Pos;
            m_vRot.w = m_fEndScale;
        } else {
            INT dx = (m_EndPos.x - m_StartPos.x) * m_tqTimer / 800;
            INT dy = (m_EndPos.y - m_StartPos.y) * m_tqTimer / 800;
            m_Pos.x = m_EndPos.x - dx;
            m_Pos.y = m_EndPos.y - dy;
        }
        break;
    case 0x12: {
        m_uiFlag |= 0x2008;
        m_tqTimer -= getFrameRate();
        CEntity* pEntity = (CEntity*)CTaskMan::getTask(m_hTarget);
        if (pEntity) {
            FVECTOR pos;
            IVECTOR out;
            pEntity->getLockOnPos(pos);
            SetRotTransPersMatrix(CWorldEnv::getWorldScreenMatrix());
            RotTransPersClipW(out, pos);
            if (!out.w && out.x >= 0x5000 && out.x <= 0xb000 && out.y >= 0x5000 && out.y <= 0xb000) {
                m_EndPos.x = (out.x >> 4) - 0x700;
                m_EndPos.y = (out.y >> 4) - 0x740;
            }
            INT dx = (m_EndPos.x - m_StartPos.x) * m_tqTimer / 1600;
            INT dy = (m_EndPos.y - m_StartPos.y) * m_tqTimer / 1600;
            m_Pos.x = m_EndPos.x - dx;
            m_Pos.y = m_EndPos.y - dy;
            if (m_tqTimer <= 0) {
                INT count = CCardBattle::getStockCount(2);
                if (count == 3) pEntity->attachEffect(g_szDpxZexion2, 10, &pEntity->Crd(1), (const FVECTOR*)NULL, (const FVECTOR*)NULL, 0.0f);
                else pEntity->attachEffect(g_szDpxZexion2, 11, &pEntity->Crd(1), (const FVECTOR*)NULL, (const FVECTOR*)NULL, 0.0f);
            }
        }
        m_vRot.y = _adjustRad(m_vRot.y + getFrameRateF() * DEG2RAD((1600 - m_tqTimer) * 0.01f));
        m_vRot.z = _adjustRad(m_vRot.z + getFrameRateF() * DEG2RAD(m_cardData.m_nNumber * 0.5f));
        m_vRot.w = m_tqTimer / 1600.0f;
        if (m_tqTimer <= 0) {
            m_tqTimer = 0;
            TRACE("PLCR:: snatchCard =\n");
            TRACE("  [Card%03d:%d] iStock=%d, iHand=%d/%d, uiFlag=%08x, hEnemy=%d,\n", m_cardData.m_nKind, m_cardData.m_nNumber, m_iStock, m_iHand, m_iHandMax, m_uiFlag, m_hTarget);
            CCardBattle* pCB = (CCardBattle*)CTaskMan::getSysTask(0x16);
            if (pCB) {
                CCardRevolver* pRV = pCB->getRevolver(2);
                if (pRV) pRV->addCard(&m_cardData, 0);
            }
            m_uiOwner = 2;
            destroy(1);
        }
        break;
    }
    }
}
void CBattleCard::setPronounce(UINT inPri) {
    changeState(6);
    m_uiMode = 8;
    m_iStock = inPri;
    m_iHand = -1;
    m_uiFlag = (INT)m_uiFlag & ~0x2;
}
void CBattleCard::setPlayPronounce() {
    m_uiMode = 1;
    m_StartPos = m_Pos;
    m_fStartScale = m_vRot.w;
    CB_LAYOUT& lay = g_LayoutData[1];
    m_EndPos.set(lay.x, lay.y);
    m_fEndScale = lay.scale * 0.01f;
    m_tqTimer = 400;
    m_uiFlag = (INT)(m_uiFlag | 0x40) & ~0x2;
    if (m_uiFlag & 0x10) {
        m_uiFlag = (INT)m_uiFlag & ~0x10;
        m_pCard->m_seqControl.fgColor = 0;
    }
    m_uiFlag |= 0x20020;
}
void CBattleCard::preStatePronounce() {
    CB_LAYOUT* pLayout;
    m_iState = 6;
    m_uiFlag |= 0x1;
    pLayout = &g_LayoutData[16];
    m_StartPos.set(pLayout->x, pLayout->y);
    m_fStartScale = pLayout->scale * 0.01f;
    pLayout = &g_LayoutData[59 + m_iStock];
    m_EndPos.set(pLayout->x, pLayout->y);
    m_fEndScale = pLayout->scale * 0.01f;
    m_tqTimer = 400;
    m_Pos = m_StartPos;
    m_vRot.w = m_fStartScale;
    m_iLayer = pLayout->layer;
    m_pCard->changeLayer(m_iLayer);
    if (m_iStock > 0) m_vRot.y = 3.1415927f;
    m_uiFlag |= 0x2008;
    if (m_uiFlag & 0x10) {
        m_uiFlag = (INT)m_uiFlag & ~0x10;
        m_pCard->enableColorCtrl(FALSE);
    }
}
void CBattleCard::statePronounce() {
    switch (m_uiMode) {
    case 2:
        break;
    case 8:
        m_uiFlag |= 0x2008;
        m_tqTimer -= getFrameRate();
        if (m_tqTimer <= 0) {
            m_uiMode = 9;
            m_uiFlag |= 0x4;
            m_tqTimer = 0;
            m_Pos = m_EndPos;
            m_vRot.w = m_fEndScale;
        } else {
            INT dx = (m_EndPos.x - m_StartPos.x) * m_tqTimer / 400;
            INT dy = (m_EndPos.y - m_StartPos.y) * m_tqTimer / 400;
            FLOAT sc = m_fEndScale - (m_fEndScale - m_fStartScale) * m_tqTimer / 400.0f;
            m_vRot.w = sc;
            m_Pos.x = m_EndPos.x - dx;
            m_Pos.y = m_EndPos.y - dy;
        }
        break;
    case 1: {
        m_uiFlag |= 0x2008;
        m_tqTimer -= getFrameRate();
        if (m_tqTimer <= 0) m_tqTimer = 0;
        INT dx = (m_EndPos.x - m_StartPos.x) * m_tqTimer / 400;
        INT dy = (m_EndPos.y - m_StartPos.y) * m_tqTimer / 400;
        FLOAT sc = m_fEndScale - (m_fEndScale - m_fStartScale) * m_tqTimer / 400.0f;
        m_vRot.w = sc;
        m_Pos.y = m_EndPos.y - dy;
        m_Pos.x = m_EndPos.x - dx;
        if (m_iStock > 0) m_vRot.y = 3.1415927f - 3.1415927f * m_tqTimer / 400.0f;
        if (!isSet()) {
            if (abs(dx) <= 64 && abs(dy) <= 64) m_uiFlag |= 0x2;
        } else if (!m_tqTimer) {
            m_uiMode = 2;
            m_vRot.y = 0.0f;
        }
        break;
    }
    }
}
void CBattleCard::setDiscard(INT inFlag) {
    m_uiMode = 20;
    if (inFlag) {
        m_uiOwner = 2;
        m_uiFlag |= 0x20;
        destroy(0);
    } else {
        outHand();
    }
}
// SOURCE DATA: .data .rodata .bss
