#ifndef SYSTEM_DEBUG_H
#define SYSTEM_DEBUG_H

#include "khtypes.h"

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
    YASUE = 13,
    KMIYAZAK = 14,
    KYOSHIOK = 15,
    DKAWABE = 16,
    FAYUMI = 17,
    TANADA = 18,
    MMATSUUR = 19,
    USR_GRA = 20,
    TFUJIMOT = 21,
    YAMASEI = 22,
    THAMA = 23,
    SOTANI = 24,
    KKONDO = 25,
    VMARK = 26,
    KIWASAKI = 27,
    TYAMAZAK = 28,
    ONISHI = 29,
    TSUGINO = 30,
    GKOBAYAS = 31,
    TTAKAKO = 32,
    HSUGIYAM = 33,
    SHINODA = 34,
    HATAKERU = 35,
    HAMADAK = 36,
    MIKI = 37,
    KO_INOUE = 38,
    ADACHIJ = 39,
    AIWATA = 40,
    MAKI = 41,
    YSHUHEI = 42,
    SHINICHI = 43,
    NSAKI = 44,
    UCHIMURA = 45,
    IMAKI = 46,
    FMIYUKI = 47,
    DCHIBA = 48,
    MWAKIMUR = 49,
    ITAI = 50,
    THONDA = 51,
    _GRA_RESERVE1 = 52,
    _GRA_RESERVE0 = 53,
    USR_SOUND = 54,
    _SOUND_RESERVE1 = 55,
    _SOUND_RESERVE0 = 56,
    USR_OTHER = 57,
    _OTHER_RESERVE1 = 58,
    _OTHER_RESERVE0 = 59,
    USR_TERMINATOR = 60
};

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

#ifndef __cplusplus /* MATCH: the C units (libppp) only call TRACE: USER and TtyColor are no C type names */
void (TRACE)(char* fmt, ...);
#else
extern char _szTraceBuf[1536];

extern "C" int dbgCheckUser(USER uid);
INT dbgButtonTrg(USER uid, UINT button, INT padId);
INT dbgButtonDown(USER uid, UINT button, INT padId);
INT dbgButtonRTrg(USER uid, UINT button, INT padId);
INT dbgButtonRpt(USER uid, UINT button, INT padId);
INT dbgAnalogRX(USER uid, INT padId);
INT dbgAnalogRY(USER uid, INT padId);
INT dbgAnalogLX(USER uid, INT padId);
INT dbgAnalogLY(USER uid, INT padId);
extern "C" void (TRACE)(char* fmt, ...);
extern "C" void (TRACE_COL)(TtyColor nCol, char* fmt, ...);
extern "C" void (TRACE_USER)(USER uid, char* fmt, ...);
extern "C" void (TRACE_USER_COL)(USER uid, TtyColor nCol, char* fmt, ...);
#endif /* __cplusplus */

#endif /* SYSTEM_DEBUG_H */
