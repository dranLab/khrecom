#include "khtypes.h"
#include "Game/BattleCard.h"
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
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Game/BattleCard", _initialize__Q210CardBattle11CBattleCard);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Game/BattleCard", changeData__Q210CardBattle11CBattleCardR4CARD);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Game/BattleCard", main__Q210CardBattle11CBattleCard);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Game/BattleCard", dispCard__Q210CardBattle11CBattleCard);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Game/BattleCard", testDisp__Q210CardBattle11CBattleCard);
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
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Game/BattleCard", setRefrect__Q210CardBattle11CBattleCard);
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
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Game/BattleCard", setSelect__Q210CardBattle11CBattleCardi);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Game/BattleCard", setRoomEffect__Q210CardBattle11CBattleCardPC4CARD);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Game/BattleCard", setEnchantEffect__Q210CardBattle11CBattleCardiUi);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Game/BattleCard", resetEnchantEffect__Q210CardBattle11CBattleCardi);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Game/BattleCard", setNumberBreak__Q210CardBattle11CBattleCardi);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Game/BattleCard", updateNumberEffect__Q210CardBattle11CBattleCard);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Game/BattleCard", setDuel__Q210CardBattle11CBattleCardi);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Game/BattleCard", setDuelRefrect__Q210CardBattle11CBattleCard);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Game/BattleCard", setDuelStock__Q210CardBattle11CBattleCardUi);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Game/BattleCard", setPlayDuelStock__Q210CardBattle11CBattleCardi);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Game/BattleCard", preStateDuel__Q210CardBattle11CBattleCard);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Game/BattleCard", stateDuel__Q210CardBattle11CBattleCard);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Game/BattleCard", setSnatch__Q210CardBattle11CBattleCardi);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Game/BattleCard", resetSnatch__Q210CardBattle11CBattleCard);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Game/BattleCard", preStateSnatch__Q210CardBattle11CBattleCard);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Game/BattleCard", stateSnatch__Q210CardBattle11CBattleCard);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Game/BattleCard", setPronounce__Q210CardBattle11CBattleCardUi);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Game/BattleCard", setPlayPronounce__Q210CardBattle11CBattleCard);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Game/BattleCard", preStatePronounce__Q210CardBattle11CBattleCard);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Game/BattleCard", statePronounce__Q210CardBattle11CBattleCard);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Game/BattleCard", setDiscard__Q210CardBattle11CBattleCardi);
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
