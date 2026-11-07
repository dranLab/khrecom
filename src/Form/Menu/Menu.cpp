
#define GPRIM_H_PATH "System/Graphic/GPrim.h" /* MATCH: this unit names Gprim.h so (header default "System/Graphic/Gprim.h") */
#include "System/Graphic/Gprim.h"
#include "System/Graphic/2d/2DMan.h"
#include "Game/Card.h"
#include "Game/GameWork.h"
#include "System/MainApp.h"
#include "System/HeapMan.h"
#include "Form/Menu/Menu.h"









#line 20
CMenu::CMenu(CTask* p, INT fgUseCardArray) : CTask(p, 0x4000) {
    m_nStep = 0;
    m_hLay = 0;
    m_nCardKindMax = 0;
    m_fgPadDisable = 0;
    if (fgUseCardArray >= 0) {
        m_nCardKind10 = (unsigned char(*)[10])com_malloc_d(298 * 10, __FILE__ "(27)", RET_ADDR());
        m_nCardKind = (USHORT*)com_malloc_d(298 * sizeof(USHORT), __FILE__ "(28)", RET_ADDR());
        initCardData(fgUseCardArray);
    } else {
        m_nCardKind = NULL;
        m_nCardKind10 = NULL;
    }
    m_bFpsHalf = CMainApp::isFpsHalf();
    CMainApp::setFpsFull();
}

CMenu::~CMenu() {
    D2::C2DMan::destroy(m_hLay);
    if (m_nCardKind) com_free(m_nCardKind);
    m_nCardKind = NULL;
    if (m_nCardKind10) com_free(m_nCardKind10);
    m_nCardKind10 = NULL;
    if (m_bFpsHalf) CMainApp::setFpsHalf();
}

void CMenu::initCardData(INT initKind) {
    INT index;
    INT i;
    INT j;

    for (i = 0; i < 298; i++) {
        m_nCardKind[i] = 0;
        for (j = 0; j < 10; j++) {
            m_nCardKind10[i][j] = 0;
        }
    }
    index = 0;
    if (initKind == 0) {
        INT i;
        INT j;
        INT k;
        INT n;
        short idTbl0[6][2] = {
            {0x001, 0x019}, {0x101, 0x119}, {0x01A, 0x02A}, {0x11A, 0x12A}, {0x02A, 0x031}, {0x044, 0x07F},
        };
        for (i = 0; i < 6; i++) {
            for (k = idTbl0[i][0]; k < idTbl0[i][1]; k++) {
                if (CCard::searchCardKind(k, -1)) {
                    m_nCardKind[index] = k;
                    for (j = 0; j < 10; j++) {
                        n = CCard::searchCard(k, j);
                        m_nCardKind10[index][j] = (n >= 0) ? n : 0;
                    }
                    index++;
                }
            }
        }
        m_nCardKindMax = index;
    } else {
        INT i;
        INT j;
        INT k;
        INT n;
        short idTbl1[2][2] = {
            {0x0A1, 0x0BA}, {0x0BA, 0x0BE},
        };
        for (i = 0; i < 2; i++) {
            for (k = idTbl1[i][0]; k < idTbl1[i][1]; k++) {
                if (CCard::searchCardKind(k, -1)) {
                    m_nCardKind[index] = k;
                    for (j = 0; j < 10; j++) {
                        n = CCard::searchCard(k, j);
                        m_nCardKind10[index][j] = (n >= 0) ? n : 0;
                    }
                    index++;
                }
            }
        }
        m_nCardKindMax = index;
    }
}

INT CMenu::_isButtonTrg(UINT b, UINT nInPad) {
    if (CGameWork::getFlag(1, 0x1C) || m_fgPadDisable) return 0;
    return CPadHandler::isButtonTrg(b, nInPad);
}

INT CMenu::_isButtonDown(UINT b, UINT nInPad) {
    if (CGameWork::getFlag(1, 0x1C) || m_fgPadDisable) return 0;
    return CPadHandler::isButtonDown(b, nInPad);
}

INT CMenu::_isButtonRTrg(UINT b, UINT nInPad) {
    if (CGameWork::getFlag(1, 0x1C) || m_fgPadDisable) return 0;
    return CPadHandler::isButtonRTrg(b, nInPad);
}

INT CMenu::_isButtonRpt(UINT b, UINT nInPad) {
    if (CGameWork::getFlag(1, 0x1C) || m_fgPadDisable) return 0;
    m_buttonTrg = CPadHandler::isButtonTrg(b, nInPad);
    return CPadHandler::isButtonRpt(b, nInPad);
}

INT CMenu::_getAnalogRX(UINT nInPad) {
    if (CGameWork::getFlag(1, 0x1C) || m_fgPadDisable) return 0;
    return CPadHandler::getAnalogRX(nInPad);
}

INT CMenu::_getAnalogRY(UINT nInPad) {
    if (CGameWork::getFlag(1, 0x1C) || m_fgPadDisable) return 0;
    return CPadHandler::getAnalogRY(nInPad);
}

INT CMenu::_getAnalogLX(UINT nInPad) {
    if (CGameWork::getFlag(1, 0x1C) || m_fgPadDisable) return 0;
    return CPadHandler::getAnalogLX(nInPad);
}

INT CMenu::_getAnalogLY(UINT nInPad) {
    if (CGameWork::getFlag(1, 0x1C) || m_fgPadDisable) return 0;
    return CPadHandler::getAnalogLY(nInPad);
}

BOOL CMenu::_isTrgOK(UINT nInPad) {
    if (CGameWork::getFlag(1, 0x1C) || m_fgPadDisable) return 0;
    return CPadHandler::isTrgOK(nInPad);
}

BOOL CMenu::_isTrgCANCEL(UINT nInPad) {
    if (CGameWork::getFlag(1, 0x1C) || m_fgPadDisable) return 0;
    return CPadHandler::isTrgCANCEL(nInPad);
}

BOOL CMenu::_isTrgACTION(UINT nInPad) {
    if (CGameWork::getFlag(1, 0x1C) || m_fgPadDisable) return 0;
    return CPadHandler::isTrgACTION(nInPad);
}

BOOL CMenu::_isTrgNEXT(UINT nInPad) {
    if (CGameWork::getFlag(1, 0x1C) || m_fgPadDisable) return 0;
    return CPadHandler::isTrgNEXT(nInPad);
}
// SOURCE DATA: .rodata
