#include "khtypes.h"
#include "Game/BattleCard.h"
#include "Game/GameWork.h"
#include "include_asm.h"

using namespace CardBattle;

extern INT SCREEN_HEIGHT;
namespace D2 {
class C2DMan { /* PARTIAL: System/Graphic/2d/2DMan.h declares the class only for its own unit */
public:
    static BOOL getLayPosition(const CHAR* pLadName, UINT inNo, UINT inIdx, FVECTOR& outPos, BOOL bLocal);
};
}
namespace CardBattle {
extern LPCSTR g_szLadLayoutCard;
}
extern "C" char D_00473530[]; /* __FILE__ */
extern "C" char D_00473558[];

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
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Game/BattleCard", main__Q210CardBattle11CBattleCard);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Game/BattleCard", dispCard__Q210CardBattle11CBattleCard);
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
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Game/BattleCard", preStateUse__Q210CardBattle11CBattleCard);
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
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Game/BattleCard", destroy__Q210CardBattle11CBattleCardi);
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
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Game/BattleCard", preStateStock__Q210CardBattle11CBattleCard);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Game/BattleCard", stateStock__Q210CardBattle11CBattleCard);
void CBattleCard::preStateHand() { m_iState = 2; }
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Game/BattleCard", stateHand__Q210CardBattle11CBattleCard);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Game/BattleCard", preStateDestroy__Q210CardBattle11CBattleCard);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Game/BattleCard", stateDestroy__Q210CardBattle11CBattleCard);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Game/BattleCard", moveCard__Q210CardBattle11CBattleCardii);
INT CBattleCard::isNowSelect() {
    if (m_uiMode != 11) return FALSE;
    return m_iHand == 0;
}
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Game/BattleCard", setHand__Q210CardBattle11CBattleCardRC4CARDiii);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Game/BattleCard", setHandLocate__Q210CardBattle11CBattleCardii);
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
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Game/BattleCard", resetHand__Q210CardBattle11CBattleCardii);
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
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Game/BattleCard", updateNumberEffect__Q210CardBattle11CBattleCard);
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
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Game/BattleCard", setPlayDuelStock__Q210CardBattle11CBattleCardi);
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
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Game/BattleCard", resetSnatch__Q210CardBattle11CBattleCard);
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
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Game/BattleCard", preStatePronounce__Q210CardBattle11CBattleCard);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Game/BattleCard", statePronounce__Q210CardBattle11CBattleCard);
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
