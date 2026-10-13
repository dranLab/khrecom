#ifndef FORM_SYSTEMMESSAGE_H
#define FORM_SYSTEMMESSAGE_H

#include "khtypes.h"

enum SYSMSG_ID {
    SYSMSG_ID_OPENTRAY = 1,
    SYSMSG_ID_NO_PAD = 2
};

class CSystemMessage {
public:
    static void initialize();
    static void finalize();
    static void setupRsrc();
    static BOOL isRsrcSetupComplete();
    static void disposeRsrc();
    static void setEndHandler(void (*pExtEndHandler)());
    static void open(const CHAR* pszMsg);
    static void open(SYSMSG_ID id);
    static void close();
    static BOOL isOpened();
    static BOOL isClosed();
};

#endif /* FORM_SYSTEMMESSAGE_H */
