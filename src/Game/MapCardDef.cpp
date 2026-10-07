#include <string.h>

#include "Game/MapCardDef.h"
#include "System/Debug.h"

BOOL MAP_CARD_PARAM_HEADER::addrMapping() {
    if (uiVersion & 0x80000000) {
        return TRUE;
    }
    if (strncmp(szCode, "@MCP", 4) != 0) {
        return FALSE;
    }
    if ((uiVersion & 0x7FFFFFFF) != 1) {
        TRACE("****************** MCP\x82\xcc\x83o\x81[\x83W\x83\x87\x83\x93\x82\xaa\x8c\xc3\x82\xa2\x82\xc5\x82\xb7 ******************\n");
        return FALSE;
    }
    uiVersion |= 0x80000000;
    pParam = (MAP_CARD_PARAM*)((char*)this + (INT)pParam);
    return TRUE;
}

MAP_CARD_PARAM* MAP_CARD_PARAM_HEADER::getMapCardParam(BYTE inCardID) {
    static unsigned char g_byMapCardTbl[25] = {
        161, 162, 163, 164, 165, 166, 167, 168, 169, 171, 170, 172, 173,
        174, 175, 176, 177, 178, 179, 180, 181, 182, 183, 184, 185
    };
    UINT i;

    for (i = 0; i < 25; i++) {
        if (g_byMapCardTbl[i] == inCardID) {
            return &pParam[i];
        }
    }
    return &pParam[iParamNum - 1];
}
// SOURCE DATA: .data .rodata
