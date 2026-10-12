#include "System/Debug.h"
#include "System/MainApp.h"
#include "System/PadIO.h"
#include "Form/DebugMenu.h"

typedef char* va_list;
#define va_start(ap, last)                                    \
    (ap = ((va_list)__builtin_next_arg(last)                  \
           - (__builtin_args_info(2) >= 8 ? 0                 \
              : (8 - __builtin_args_info(2)) * 8)))
#define va_end(ap) ((void)0)

extern "C" {
int printf(const char* fmt, ...);
int vprintf(const char* fmt, va_list ap);
int vsprintf(char* buf, const char* fmt, va_list ap);
}

char _szTraceBuf[1536];

extern "C" int dbgCheckUser(USER uid) {
    USER mdb;

    if (uid == USR_SYSTEM) {
        return 1;
    }
    mdb = CDebugMenu::getDebugUser();
    switch (mdb) {
    case USR_SYSTEM:
        return 0;
    case USR_PROG:
        if (uid >= USR_PROG && uid < USR_PLAN) return 1;
        break;
    case USR_PLAN:
        if (uid >= USR_PLAN && uid < USR_GRA) return 1;
        break;
    case USR_GRA:
        if (uid >= USR_GRA && uid < USR_SOUND) return 1;
        break;
    case USR_SOUND:
        if (uid >= USR_SOUND && uid < USR_OTHER) return 1;
        break;
    default:
        switch (uid) {
        case USR_PROG:
            if (mdb >= USR_PROG && mdb < USR_PLAN) return 1;
            break;
        case USR_PLAN:
            if (mdb >= USR_PLAN && mdb < USR_GRA) return 1;
            break;
        case USR_GRA:
            if (mdb >= USR_GRA && mdb < USR_SOUND) return 1;
            break;
        case USR_SOUND:
            if (mdb >= USR_SOUND && mdb < USR_OTHER) return 1;
            break;
        default:
            if (mdb == uid) return 1;
            break;
        }
        break;
    }
    return 0;
}

INT dbgButtonTrg(USER uid, UINT button, INT padId) {
    if (!dbgCheckUser(uid)) {
        return 0;
    }
    return CPadIO::isButtonTrg(button, padId);
}

INT dbgButtonDown(USER uid, UINT button, INT padId) {
    if (!dbgCheckUser(uid)) {
        return 0;
    }
    return CPadIO::isButtonDown(button, padId);
}

INT dbgButtonRTrg(USER uid, UINT button, INT padId) {
    if (!dbgCheckUser(uid)) {
        return 0;
    }
    return CPadIO::isButtonRTrg(button, padId);
}

INT dbgButtonRpt(USER uid, UINT button, INT padId) {
    if (!dbgCheckUser(uid)) {
        return 0;
    }
    return CPadIO::isButtonRpt(button, padId);
}

INT dbgAnalogRX(USER uid, INT padId) {
    if (!dbgCheckUser(uid)) {
        return 0;
    }
    return CPadIO::getAnalogRX(padId);
}

INT dbgAnalogRY(USER uid, INT padId) {
    if (!dbgCheckUser(uid)) {
        return 0;
    }
    return CPadIO::getAnalogRY(padId);
}

INT dbgAnalogLX(USER uid, INT padId) {
    if (!dbgCheckUser(uid)) {
        return 0;
    }
    return CPadIO::getAnalogLX(padId);
}

INT dbgAnalogLY(USER uid, INT padId) {
    if (!dbgCheckUser(uid)) {
        return 0;
    }
    return CPadIO::getAnalogLY(padId);
}

extern "C" void TRACE(char* fmt, ...) {
    va_list args;

    va_start(args, fmt);
    if (g_bTraceOnScreen) {
        vsprintf(_szTraceBuf, fmt, args);
        addTraceString(_szTraceBuf);
    } else {
        vprintf(fmt, args);
    }
    va_end(args);
}

extern "C" void TRACE_COL(TtyColor nCol, char* fmt, ...) {
    va_list args;

    printf("\x1B[%dm", nCol);
    va_start(args, fmt);
    if (g_bTraceOnScreen) {
        vsprintf(_szTraceBuf, fmt, args);
        addTraceString(_szTraceBuf);
    } else {
        vprintf(fmt, args);
    }
    printf("\x1B[0m");
    va_end(args);
}

extern "C" void TRACE_USER(USER uid, char* fmt, ...) {
    va_list args;

    if (dbgCheckUser(uid)) {
        va_start(args, fmt);
        if (g_bTraceOnScreen) {
            vsprintf(_szTraceBuf, fmt, args);
            addTraceString(_szTraceBuf);
        } else {
            vprintf(fmt, args);
        }
        va_end(args);
    }
}

extern "C" void TRACE_USER_COL(USER uid, TtyColor nCol, char* fmt, ...) {
    va_list args;

    if (dbgCheckUser(uid)) {
        printf("\x1B[%dm", nCol);
        va_start(args, fmt);
        if (g_bTraceOnScreen) {
            vsprintf(_szTraceBuf, fmt, args);
            addTraceString(_szTraceBuf);
        } else {
            vprintf(fmt, args);
        }
        printf("\x1B[0m");
        va_end(args);
    }
}
// SOURCE DATA: .rodata .bss
