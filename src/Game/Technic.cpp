#include "System/Ctd/CtdMan.h"
#define GPRIM_H_PATH "System/Graphic/GPrim.h"
#define CARDBATTLE_H_PATH "c:/KHCOM/MainApp/Game/CardBattle.h"
#define TECHNIC_H_PATH "c:/KHCOM/MainApp/Game/Technic.h"
#define TASK_H_NO_SLC_OVERFLOW_MSG
#include "System/Graphic/Gprim.h"
#include "System/Graphic/2d/2DMan.h"
#include "Game/CardBattle.h"
#include "Game/Technic.h"
#include "Game/Game.h"
#include "Game/GameWork.h"
#include "System/Debug.h"
#include <string.h>

#define TECHNIC_BUFF_SIZE 28

CTechnic* CTechnic::m_pInstance = NULL;
TECHNIC_INFO* CTechnic::m_pTechnicInfo;

CTechnic::CTechnic() {
    m_pTechnicInfo = (TECHNIC_INFO*)CGameWork::progMCW()._technicInfo;
}

CTechnic::~CTechnic() {}

void CTechnic::clearAll() {
    memset(m_pTechnicInfo, 0, TECHNIC_BUFF_SIZE);
    TRACE("CTechnic::clearAll() \x82\xb7\x82\xd7\x82\xc4\x82\xcc\x83X\x83g\x83" "b\x83N\x8bZ\x82\xf0\x8d\xed\x8f\x9c\x82\xb5\x82\xdc\x82\xb5\x82\xbd\n");
}

void CTechnic::setAll() {
    INT i;
    clearAll();
    if (CGame::isSoraGame()) {
        for (i = 4; i < 87; i++) {
            addTechnic(i);
        }
        for (i = 90; i < 93; i++) {
            addTechnic(i);
        }
    } else {
        for (i = 95; i < 108; i++) {
            addTechnic(i);
        }
        for (i = 109; i < 110; i++) {
            addTechnic(i);
        }
    }
    TRACE("CTechnic::setAll() \x82\xb7\x82\xd7\x82\xc4\x82\xcc\x83X\x83g\x83" "b\x83N\x8bZ\x82\xf0\x8fK\x93\xbe\x82\xb5\x82\xdc\x82\xb5\x82\xbd\n");
}

BOOL CTechnic::addTechnic(USHORT inKind) {
    if (!isValid(inKind)) return FALSE;
    if (getAppearance(inKind)) return TRUE;
    if (dbgCheckUser((USER)8)) {
        if (getName(inKind)) {
            TRACE("CTechnic::addTechnic() \x83X\x83g\x83" "b\x83N\x8bZ\x82\xf0\x90V\x8bK\x93\xfc\x8e\xe8\x81I [%3d: %s]\n", inKind, getName(inKind));
        } else {
            TRACE("CTechnic::addTechnic() \x83X\x83g\x83" "b\x83N\x8bZ\x82\xf0\x90V\x8bK\x93\xfc\x8e\xe8\x81I [%3d: \x96\xbc\x91O\x83" "f\x81[\x83^\x82\xaa\x82\xa0\x82\xe8\x82\xdc\x82\xb9\x82\xf1]\n", inKind);
        }
    }
    setAppearance(inKind, 1);
    return TRUE;
}

BOOL CTechnic::canUse(USHORT inKind) {
    if (!isValid(inKind)) return FALSE;
    return getAppearance(inKind) != 0;
}

BOOL CTechnic::isNew(USHORT inKind) {
    if (!isValid(inKind)) return FALSE;
    return getAppearance(inKind) == 1;
}

BOOL CTechnic::setNewFinish(USHORT inKind) {
    if (!isValid(inKind)) return FALSE;
    if (getAppearance(inKind) != 1) return FALSE;
    setAppearance(inKind, 2);
    return TRUE;
}

BOOL CTechnic::isValid(USHORT inKind) {
    if (CGame::isSoraGame()) return isSoraValid(inKind);
    return isRikuValid(inKind);
}

BOOL CTechnic::isSoraValid(USHORT inKind) {
    if (4 <= inKind && inKind < 87) return TRUE;
    return 90 <= inKind && inKind < 93;
}

BOOL CTechnic::isRikuValid(USHORT inKind) {
    if (95 <= inKind && inKind < 108) return TRUE;
    return inKind == 109;
}

LPCTSTR CTechnic::getName(INT nTechKind) {
    LPCTSTR pTechName;
    UINT ctdID;
    if (nTechKind < 4 || nTechKind >= 110) {
        pTechName = CCtdMan::getText(0x80100);
    } else {
        ctdID = 0x80100 + nTechKind;
        pTechName = CCtdMan::getText(ctdID);
    }
    return pTechName;
}

#line 229
INT CTechnic::getAppearance(USHORT inKind) {
    USHORT idx = inKind >> 2;
    USHORT shift = (inKind & 3) << 1;
    BYTE* pTop = (BYTE*)m_pTechnicInfo;
    if (!(idx < TECHNIC_BUFF_SIZE)) AssertApp(__FILE__, __LINE__, "idx < TECHNIC_BUFF_SIZE", 0);
    return (pTop[idx] >> shift) & 3;
}

#line 248
void CTechnic::setAppearance(USHORT inKind, INT nAppear) {
    USHORT idx = inKind >> 2;
    USHORT shift = (inKind & 3) << 1;
    BYTE* pTop = (BYTE*)m_pTechnicInfo;
    BYTE value = nAppear & 3;
    if (!(idx < TECHNIC_BUFF_SIZE)) AssertApp(__FILE__, __LINE__, "idx < TECHNIC_BUFF_SIZE", 0);
    pTop[idx] = (pTop[idx] & ~(3 << shift)) | (value << shift);
}

// SOURCE DATA: .data .rodata
