// PARTIAL: added for Game/MapCardDef
#ifndef SYSTEM_DEBUG_H
#define SYSTEM_DEBUG_H

#include "khtypes.h"

extern "C" void TRACE(char* fmt, ...);

// PARTIAL: added for System/Movie/MovieFiles
enum USER {
    USR_SYSTEM = 0,
    USR_PROG = 1,
    SKISHIWA = 2,
    KKIKUCHI = 3,
    TGOTO = 4,
    YKOU = 5,
    NAKAMICH = 6,
    HMATSUOK = 7,
    KATAOKAK = 8,
    KAHARA = 9,
    MNAKASHI = 10,
    USR_PLAN = 11,
    YYOSHIMO = 12,
    YASUE = 13
};

extern "C" void TRACE_USER(USER uid, char* fmt);

#endif /* SYSTEM_DEBUG_H */
