#include "khtypes.h"
#include "Game/BattleCard.h"
#include "Game/GameWork.h"
#include "Game/Debug/DebugWork.h"
#include "System/Rnd.h"
#include "include_asm.h"

extern "C" int abs(int);

using namespace CardBattle;

extern INT SCREEN_HEIGHT;
namespace D2 {
class C2DMan { /* PARTIAL: stand-in for System/Graphic/2d/2DMan C2DMan: gated by SYSTEM_GRAPHIC_2D_2DMAN_CPP */
public:
    static BOOL getLayPosition(const CHAR* pLadName, UINT inNo, UINT inIdx, FVECTOR& outPos, BOOL bLocal);
};
}
namespace CardBattle {
extern LPCSTR g_szLadLayoutCard;
}
extern "C" char D_00473530[]; /* __FILE__ */
extern "C" const CARD_DATA cardDataTbl_00474480[]; /* CardKind.h's static table, this unit's copy (.rodata asm) */
static inline void _setCard(CARD& c, USHORT inKind, BYTE num) {
    c.m_nId = 0;
    c.m_nKind2 = inKind;
    c.m_nNumber2 = num;
    c.m_nCategory20 = cardDataTbl_00474480[c.m_nKind].nCategory0;
    c.m_nCategory21 = cardDataTbl_00474480[c.m_nKind].nCategory1;
}
extern "C" char D_00473558[];
extern "C" char D_00473580[]; /* "BC::destroy(); handle=%d\n" */
extern "C" char D_004730D8[]; /* "Game/CardBattle.h" */
extern "C" char D_00473100[]; /* "inUser" */

void CardBattle::initLayoutData() {
    FVECTOR vPos;
    for (INT i = 0; i < 26; i++) {
        D2::C2DMan::getLayPosition(g_szLadLayoutCard, 0, i, vPos, FALSE);
        CB_LAYOUT& lay = g_LayoutData[g_Seq_toLayout[i]];
        lay.x = (INT)vPos.x;
        lay.y = (INT)vPos.y;
    }
}
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Game/BattleCard", __Q210CardBattle11CBattleCardP5CTaskiiii);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Game/BattleCard", _$_Q210CardBattle11CBattleCard);
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
    if (!(inState >= 0 && inState < 7)) AssertApp(D_00473530, 492, D_00473558, 0);
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
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Game/BattleCard", stateUse__Q210CardBattle11CBattleCard);
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
/* Parked: 5 lines - the original keeps two copies of `addGimmickCount(-1); break;` (cases 0xe and 6); our jump2
   cross-jumps case 0xe's tail into case 6. Per-case flag sets, gotos, labels, argument spellings and block scopes
   all merge too (synthetic tests build/ps/cj/d*.cpp, e*.cpp, f1.cpp). */
#if 0
void CBattleCard::destroy(INT inFlag) {
    if (m_uiMode < 8) {
        if (m_uiMode >= 3) return;
    }
    if (m_pRevolver) {
        TRACE(D_00473580, getHandle());
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
        if (DebugWork::g_dw.player.iCardSpend) return;
        if (!inFlag) return;
        switch (getCardCategory()) {
        case 0xe:
            m_pRevolver->exeTrapEffect(0);
            CCardBattle::addGimmickCount(-1);
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
#else
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Game/BattleCard", destroy__Q210CardBattle11CBattleCardi);
#endif
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
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Game/BattleCard", setPlayStock__Q210CardBattle11CBattleCardi);
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
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Game/BattleCard", stateStock__Q210CardBattle11CBattleCard);
void CBattleCard::preStateHand() { m_iState = 2; }
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Game/BattleCard", stateHand__Q210CardBattle11CBattleCard);
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
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Game/BattleCard", stateDestroy__Q210CardBattle11CBattleCard);
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
        INT inUser = m_uiUser;
        if (!(inUser)) AssertApp(D_004730D8, 644, D_00473100, 0);
        CARD* pEnchant = CCardBattle::m_pInstance ? &CCardBattle::m_pInstance->m_cardEnchant[inUser - 1] : NULL;
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
            if (DebugWork::g_dw.player.iDeckDbgDisp) {
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
            if (DebugWork::g_dw.player.iDeckDbgDisp) {
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
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Game/BattleCard", setRoomEffect__Q210CardBattle11CBattleCardPC4CARD);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Game/BattleCard", setEnchantEffect__Q210CardBattle11CBattleCardiUi);
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
        INT inUser = m_uiUser;
        if (!(inUser)) AssertApp(D_004730D8, 644, D_00473100, 0);
        CARD* pEnchant = CCardBattle::m_pInstance ? &CCardBattle::m_pInstance->m_cardEnchant[inUser - 1] : NULL;
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
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Game/BattleCard", preStateDuel__Q210CardBattle11CBattleCard);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Game/BattleCard", stateDuel__Q210CardBattle11CBattleCard);
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
    _setCard(m_cardData, 0, 0xff);
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
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Game/BattleCard", stateSnatch__Q210CardBattle11CBattleCard);
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
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Game/BattleCard", __static_initialization_and_destruction_0_001D3370);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Game/BattleCard", func_001D3E70);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Game/BattleCard", _$_t14CStateTemplate1ZQ210CardBattle11CBattleCard);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Game/BattleCard", getCurrentState__Q210CardBattle11CBattleCard);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Game/BattleCard", getMode__Q210CardBattle11CBattleCard);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Game/BattleCard", getLayer__Q210CardBattle11CBattleCard);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Game/BattleCard", isDisp__Q210CardBattle11CBattleCard);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Game/BattleCard", isSet__Q210CardBattle11CBattleCard);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Game/BattleCard", isNowStock__Q210CardBattle11CBattleCard);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Game/BattleCard", getHand__Q210CardBattle11CBattleCard);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Game/BattleCard", getStock__Q210CardBattle11CBattleCard);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Game/BattleCard", getHandMax__Q210CardBattle11CBattleCard);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Game/BattleCard", getFlag__Q210CardBattle11CBattleCard);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Game/BattleCard", getCardData__Q210CardBattle11CBattleCard);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Game/BattleCard", getPos__Q210CardBattle11CBattleCard);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Game/BattleCard", isReloadNG__Q210CardBattle11CBattleCard);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Game/BattleCard", setReloadNG__Q210CardBattle11CBattleCard);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Game/BattleCard", resetReloadNG__Q210CardBattle11CBattleCard);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Game/BattleCard", isNowPlaying__Q210CardBattle11CBattleCard);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Game/BattleCard", isNowReloadNG__Q210CardBattle11CBattleCard);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Game/BattleCard", isDModeNG__Q210CardBattle11CBattleCard);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Game/BattleCard", isPlutoON__Q210CardBattle11CBattleCard);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Game/BattleCard", setPlutoON__Q210CardBattle11CBattleCard);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Game/BattleCard", resetPlutoON__Q210CardBattle11CBattleCard);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Game/BattleCard", isPlayNG__Q210CardBattle11CBattleCard);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Game/BattleCard", setPlayNG__Q210CardBattle11CBattleCard);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Game/BattleCard", resetPlayNG__Q210CardBattle11CBattleCard);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Game/BattleCard", isReloadSet__Q210CardBattle11CBattleCard);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Game/BattleCard", setReloadSet__Q210CardBattle11CBattleCard);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Game/BattleCard", resetReloadSet__Q210CardBattle11CBattleCard);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Game/BattleCard", setStartIn__Q210CardBattle11CBattleCard);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Game/BattleCard", isFolderON__Q210CardBattle11CBattleCard);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Game/BattleCard", setFolderON__Q210CardBattle11CBattleCard);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Game/BattleCard", isDuelStock__Q210CardBattle11CBattleCard);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Game/BattleCard", getRevolver__CQ210CardBattle11CBattleCard);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Game/BattleCard", getCardKind__CQ210CardBattle11CBattleCard);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Game/BattleCard", getCardCategory__CQ210CardBattle11CBattleCard);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Game/BattleCard", getBaseCardNumber__CQ210CardBattle11CBattleCard);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Game/BattleCard", setBaseCardNumber__Q210CardBattle11CBattleCard);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Game/BattleCard", setFlagDuelPlay__Q210CardBattle11CBattleCard);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Game/BattleCard", isFlagDuelPlay__Q210CardBattle11CBattleCard);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Game/BattleCard", setCardOwner__Q210CardBattle11CBattleCardi);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Game/BattleCard", getCardOwner__CQ210CardBattle11CBattleCard);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Game/BattleCard", setFlagPronouncePlay__Q210CardBattle11CBattleCard);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Game/BattleCard", isFlagPronouncePlay__Q210CardBattle11CBattleCard);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Game/BattleCard", executeBeginState__t14CStateTemplate1ZQ210CardBattle11CBattleCard);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Game/BattleCard", executeState__t14CStateTemplate1ZQ210CardBattle11CBattleCard);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Game/BattleCard", executeEndState__t14CStateTemplate1ZQ210CardBattle11CBattleCard);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Game/BattleCard", _GLOBAL_$I$c__KHCOM_MainApp_Game_BattleCard.cpp08aaaa);
