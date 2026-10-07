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

// PARTIAL: added for System/Graphic/2d/2DBase
enum TtyColor {
    _TTY_NORMAL = 0,
    _TTY_BLACK = 30,
    _TTY_RED = 31,
    _TTY_GREEN = 32,
    _TTY_YELLOW = 33,
    _TTY_BLUE = 34,
    _TTY_PURPLE = 35,
    _TTY_CYAN = 36,
    _TTY_WHITE = 37,
    _TTY_REV = 7,
    _TTY_REV_BLACK = 40,
    _TTY_REV_RED = 41,
    _TTY_REV_GREEN = 42,
    _TTY_REV_YELLOW = 43,
    _TTY_REV_BLUE = 44,
    _TTY_REV_PURPLE = 45,
    _TTY_REV_CYAN = 46,
    _TTY_REV_WHITE = 47
};

extern "C" void TRACE_COL(TtyColor nCol, char* fmt, ...);

#endif /* SYSTEM_DEBUG_H */
