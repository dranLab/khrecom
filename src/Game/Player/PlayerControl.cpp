#include "Game/Player/PlayerControl.h"
#include "System/HeapMan.h"
#include "System/Vector.h"
#include "Game/GameWork.h"
#include "Game/Debug/DebugWork.h"

extern "C" {
float atan2f(float y, float x);
}

#include "System/Coord.h"
#include "System/Graphic/WorldEnv.h"

static inline FLOAT _sqrtf(FLOAT x) {
    FLOAT ret;
    asm volatile("sqrt.s %0, %1" : "=f"(ret) : "f"(x));
    return ret;
}

CPlayerPad::CPlayerPad() : m_uiPadFlag(1) {
    m_fDir = 0.0f;
    m_fPower = 0.0f;
    m_fPreDir = 0.0f;
    m_fPrePower = 0.0f;
    m_Houkou = HOUKOU_NUTRAL;
    m_PreHoukou = HOUKOU_NUTRAL;
    m_fDirR = 0.0f;
    m_fPowerR = 0.0f;
    m_fPreDirR = 0.0f;
    m_fPrePowerR = 0.0f;
    m_HoukouR = HOUKOU_NUTRAL;
    m_PreHoukouR = HOUKOU_NUTRAL;
    m_fViewDir = 0.0f;
    m_uiPadFlag |= 2;
    if (CGameWork::getByte(0, 100) == 0) {
        m_padLA = 4;
        m_padRA = 8;
        m_padLB = 1;
        m_padRB = 2;
    } else {
        m_padLA = 1;
        m_padRA = 2;
        m_padLB = 4;
        m_padRB = 8;
    }
}

CPlayerPad::~CPlayerPad() {
}

void CPlayerPad::getPadInfo() {
    FLOAT x;
    FLOAT y;

    if (CGameWork::getByte(0, 100) == 0) {
        m_padLA = 4;
        m_padRA = 8;
        m_padLB = 1;
        m_padRB = 2;
    } else {
        m_padLA = 1;
        m_padRA = 2;
        m_padLB = 4;
        m_padRB = 8;
    }
    m_fPreDir = m_fDir;
    m_fPrePower = m_fPower;
    m_PreHoukou = m_Houkou;
    m_fPreDirR = m_fDirR;
    m_fPrePowerR = m_fPowerR;
    m_PreHoukouR = m_HoukouR;
    if ((m_uiPadFlag & 1) == 0) {
        clearPadInfo();
        return;
    }

    x = getAnalogLX(0) * (1.0f / 4096.0f);
    y = getAnalogLY(0) * (1.0f / 4096.0f);
    if (m_uiPadFlag & 4) {
        x = -x;
        y = -y;
    }
    m_fDir = atan2f(x, y);
    m_fPower = _sqrtf(x * x + y * y);
    if (m_fPower > 1.0f) {
        m_fPower = 1.0f;
    }
    if (m_fPower < 0.2f) {
        m_Houkou = HOUKOU_NUTRAL;
    } else {
        m_Houkou = getHoukouFromAngle(m_fDir);
    }
    m_fViewDir = convViewDir(m_fDir);

    x = getAnalogRX(0) * (1.0f / 4096.0f);
    y = getAnalogRY(0) * (1.0f / 4096.0f);
    m_fDirR = atan2f(x, y);
    m_fPowerR = _sqrtf(x * x + y * y);
    if (m_fPowerR > 1.0f) {
        m_fPowerR = 1.0f;
    }
    if (m_fPowerR < 0.2f) {
        m_HoukouR = HOUKOU_NUTRAL;
    } else {
        m_HoukouR = getHoukouFromAngle(m_fDirR);
    }
}

Houkou CPlayerPad::getHoukouFromAngle(FLOAT inAngle) {
    if (inAngle < -2.3561945f) {
        return HOUKOU_UP;
    }
    if (inAngle < -0.7853982f) {
        return HOUKOU_LEFT;
    }
    if (inAngle < 0.7853982f) {
        return HOUKOU_DOWN;
    }
    if (inAngle < 2.3561945f) {
        return HOUKOU_RIGHT;
    }
    return HOUKOU_UP;
}

FLOAT CPlayerPad::convViewDir(FLOAT pad_dir) {
    FVECTOR view_vec;
    FLOAT r;

    CWorldEnv::getViewVector(view_vec);
    Normalize(view_vec, view_vec);
    FLOAT x = atan2f(view_vec.x, view_vec.z) + pad_dir - 3.1415927f;
    /* separate result variable: the original copies x in the branch delay slot */
    if (x < -3.1415927f) {
        r = x + 6.2831855f;
    } else {
        r = x;
        if (r > 3.1415927f) {
            r -= 6.2831855f;
        }
    }
    return r;
}

void CPlayerPad::clearPadInfo() {
    m_fDir = 0.0f;
    m_fPower = 0.0f;
    m_Houkou = HOUKOU_NUTRAL;
    m_fDirR = 0.0f;
    m_fPowerR = 0.0f;
    m_HoukouR = HOUKOU_NUTRAL;
}

BOOL CPlayerPad::isTrgAnyPad() {
    if (m_uiPadFlag & 1) {
        if (isButtonTrg(0x80) || isButtonTrg(0x40) || isButtonTrg(0x20) || isButtonTrg(0x10)) {
            return TRUE;
        }
        if (m_fPower > 0.3f) {
            if (m_Houkou != m_PreHoukou) {
                return TRUE;
            }
            if (m_fPrePower <= 0.3f) {
                return TRUE;
            }
        }
    }
    return FALSE;
}

BOOL CPlayerPad::isTrgJump() {
    return isButtonTrg(0x20) * (m_uiPadFlag & 1);
}

BOOL CPlayerPad::isTrgDefense() {
    return isButtonTrg(0x80) * (m_uiPadFlag & 1);
}

BOOL CPlayerPad::isTrgAttack() {
    if (DebugWork::g_dw.player.iTrgAuto > 0) {
        return isButtonDown(0x40) * (m_uiPadFlag & 1);
    } else {
        return isButtonTrg(0x40) * (m_uiPadFlag & 1);
    }
}

BOOL CPlayerPad::isTrgLockon() {
    return isButtonTrg(m_padRB) * (m_uiPadFlag & 1);
}

BOOL CPlayerPad::isRTrgLockon() {
    INT ret = isButtonRTrg(m_padRB);
    UINT flag = m_uiPadFlag & 1;
    return ret | (flag == 0);
}

BOOL CPlayerPad::isDownLockon() {
    return isButtonDown(m_padRB) * (m_uiPadFlag & 1);
}

BOOL CPlayerPad::isTrgStock() {
    return isButtonTrg(0x10) * (m_uiPadFlag & 1);
}

BOOL CPlayerPad::isTrgStockSkill() {
    UINT flag = m_uiPadFlag & 1;
    return (isButtonDown(m_padLA) && isButtonDown(m_padRA)) * flag;
}

BOOL CPlayerPad::isRTrgJump() {
    INT ret = isButtonRTrg(0x20);
    UINT flag = m_uiPadFlag & 1;
    return ret | (flag == 0);
}

BOOL CPlayerPad::isDownJump() {
    return isButtonDown(0x20) * (m_uiPadFlag & 1);
}

BOOL CPlayerPad::isRTrgAvoid() {
    INT ret = isButtonRTrg(0x80);
    UINT flag = m_uiPadFlag & 1;
    return ret | (flag == 0);
}

BOOL CPlayerPad::isTrgRollDeckR() {
    return isButtonTrg(m_padLA | 0x8000) * (m_uiPadFlag & 1);
}

BOOL CPlayerPad::isTrgRollDeckL() {
    return isButtonTrg(m_padRA | 0x2000) * (m_uiPadFlag & 1);
}

BOOL CPlayerPad::isTrgChangeDeck() {
    return isButtonTrg(0x100) * (m_uiPadFlag & 1);
}

BOOL CPlayerPad::isDownReloadDeck() {
    return isButtonDown(0x40) * (m_uiPadFlag & 1);
}

BOOL CPlayerPad::isTrgReloadDeck() {
    return isButtonTrg(0x40) * (m_uiPadFlag & 1);
}

BOOL CPlayerPad::isDownRollDeckR() {
    return (isButtonDown(m_padLA) | isButtonDown(0x8000)) * (m_uiPadFlag & 1);
}

BOOL CPlayerPad::isDownRollDeckL() {
    return (isButtonDown(m_padRA) | isButtonDown(0x2000)) * (m_uiPadFlag & 1);
}

BOOL CPlayerPad::isTrgShortCutReload() {
    return isButtonTrg(0x1000) * (m_uiPadFlag & 1);
}

BOOL CPlayerPad::isTrgShortCutCard() {
    return isButtonTrg(0x4000) * (m_uiPadFlag & 1);
}

BOOL CPlayerPad::isTrgCancelStock() {
    return isButtonTrg(m_padLB) * (m_uiPadFlag & 1);
}

BOOL CPlayerPad::isChangeHoukouR() {
    if ((m_uiPadFlag & 1) == 0) {
        return FALSE;
    }
    if (m_fPowerR > 0.0f && m_HoukouR != m_PreHoukouR) {
        return TRUE;
    }
    return FALSE;
}

BOOL CPlayerPad::dbgIsTrgCommonInput(UINT b, UINT pad) {
    return isButtonTrg(b, pad) * (m_uiPadFlag & 2);
}

BOOL CPlayerPad::dbgIsDownCommonInput(UINT b, UINT pad) {
    return isButtonDown(b, pad) * (m_uiPadFlag & 2);
}

BOOL CPlayerPad::isTrgBattleChange() {
    return dbgIsTrgCommonInput(0x800, 1);
}

BOOL CPlayerPad::isTrgWarpStartPos() {
    return dbgIsTrgCommonInput(0x200, 1);
}

BOOL CPlayerPad::isDownDebugMode() {
    return dbgIsDownCommonInput(1, 0) && dbgIsDownCommonInput(2, 0);
}
