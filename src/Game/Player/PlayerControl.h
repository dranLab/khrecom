#ifndef GAME_PLAYER_PLAYERCONTROL_H
#define GAME_PLAYER_PLAYERCONTROL_H

#include "khtypes.h"
#include "System/PadHandler.h"

enum Houkou {
    HOUKOU_NUTRAL = 0,
    HOUKOU_UP = 1,
    HOUKOU_LEFT = 2,
    HOUKOU_RIGHT = 3,
    HOUKOU_DOWN = 4
};

class CPlayerPad : public CPadHandler {
private:
    UINT m_uiPadFlag;
    FLOAT m_fDir;
    FLOAT m_fPower;
    FLOAT m_fPreDir;
    FLOAT m_fPrePower;
    Houkou m_Houkou;
    Houkou m_PreHoukou;
    FLOAT m_fDirR;
    FLOAT m_fPowerR;
    FLOAT m_fPreDirR;
    FLOAT m_fPrePowerR;
    Houkou m_HoukouR;
    Houkou m_PreHoukouR;
    FLOAT m_fViewDir;
    UINT m_padLA;
    UINT m_padRA;
    UINT m_padLB;
    UINT m_padRB;

public:
    CPlayerPad();
    ~CPlayerPad();
    void getPadInfo();
    void clearPadInfo();
    FLOAT getAnalogDir() { return m_fDir; }
    FLOAT getAnalogPower() { return m_fPower; }
    Houkou getAnalogHoukou() { return m_Houkou; }
    FLOAT getAnalogDirR() { return m_fDirR; }
    FLOAT getAnalogPowerR() { return m_fPowerR; }
    Houkou getAnalogHoukouR() { return m_HoukouR; }
    BOOL isTrgJump();
    BOOL isTrgDefense();
    BOOL isTrgAttack();
    BOOL isTrgLockon();
    BOOL isRTrgLockon();
    BOOL isDownLockon();
    BOOL isTrgStock();
    BOOL isTrgStockSkill();
    BOOL isTrgSkill();
    BOOL isRTrgJump();
    BOOL isDownJump();
    BOOL isRTrgAvoid();
    FLOAT getAnalogViewDir() { return m_fViewDir; }
    Houkou getHoukouFromAngle(FLOAT inAngle);
    FLOAT convViewDir(FLOAT pad_dir);
    BOOL isTrgRollDeckR();
    BOOL isTrgRollDeckL();
    BOOL isTrgChangeDeck();
    BOOL isTrgReloadDeck();
    BOOL isDownReloadDeck();
    BOOL isDownRollDeckR();
    BOOL isDownRollDeckL();
    BOOL isDownGlide();
    BOOL isTrgShortCutReload();
    BOOL isTrgShortCutCard();
    BOOL isTrgCancelStock();
    BOOL isChangeHoukouR();
    BOOL dbgIsTrgCommonInput(UINT b, UINT pad);
    BOOL dbgIsDownCommonInput(UINT b, UINT pad);
    BOOL isTrgBattleChange();
    BOOL isTrgWarpStartPos();
    BOOL isDownDebugMode();
    void enableControl() { UINT& f = m_uiPadFlag; f |= 1; } // PARTIAL: inline body added for Game/Player/PlayerRiku
    BOOL isControl();
    BOOL isTrgAnyPad();
};

#endif // GAME_PLAYER_PLAYERCONTROL_H
