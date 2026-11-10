// PARTIAL: added for Game/Gimmick/GimmickBreakable
#ifndef SYSTEM_GRAPHIC_STR_H
#define SYSTEM_GRAPHIC_STR_H

#include "khtypes.h"
#include "System/MainApp.h"

/* Placeholder for the original header's inline string class functions (255-character buffer): their
 * assert strings are in .rodata of every includer, in this order. Line numbers are not known. */
#line 1 "System/Graphic/Str.h"
inline void _strCheckLengths(BOOL inOk) {
    KH_DEBUG(if (!inOk) AssertApp(__FILE__, __LINE__, "strlen(inString.m_szBuff) <= 255", 0);)
    KH_DEBUG(if (!inOk) AssertApp(__FILE__, __LINE__, "strlen(inString) <= 255", 0);)
    KH_DEBUG(if (!inOk) AssertApp(__FILE__, __LINE__, "strlen(m_szBuff) + strlen(inString.m_szBuff) <= 255", 0);)
    KH_DEBUG(if (!inOk) AssertApp(__FILE__, __LINE__, "strlen(m_szBuff) + strlen(inString) <= 255", 0);)
}

#endif /* SYSTEM_GRAPHIC_STR_H */
