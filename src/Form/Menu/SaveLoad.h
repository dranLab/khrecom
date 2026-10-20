#ifndef FORM_MENU_SAVELOAD_H
#define FORM_MENU_SAVELOAD_H

#include "khtypes.h"
#include "System/Task.h"

enum SAVELOAD {
    SLMODE_SAVE = 0,
    SLMODE_LOAD = 1,
    SLMODE_BOOT = 2,
    SLMODE_MESSAGE = 3
};

enum GAMECLEAR {
    GAMECLEAR_NONE = 0,
    GAMECLEAR_CLEAR = 1
};

enum SLMSG {
    SLMSG_GAMECLEAR_SAVE_OK = 0,
    SLMSG_GAMECLEAR_SAVE_CANCEL = 1
};

enum SLMSG_RESULT {
    SLMSG_RESULT_CANCEL = 0,
    SLMSG_RESULT_OK = 1,
    SLMSG_RESULT_NO = 2
};

class CSaveLoad : public CTask {
private:
    static HANDLE m_hThis;
    INT m_nFlag; /* 0x40 */

public:
    CSaveLoad(CTask* pParent, SAVELOAD mode, GAMECLEAR clear);
    CSaveLoad(CTask* pParent, SLMSG msg);
    virtual ~CSaveLoad();
    static BOOL isExited();
    static void disposeRsrc();
    static BOOL isSuccessLoad();
    static BOOL isSuccessSave();
    static SLMSG_RESULT getMsgResult();

private:
    void construct(SAVELOAD mode);
};

class CSystemDataChecker {
public:
    static void start();
    static void abort();
    static BOOL isExited();
};

#endif /* FORM_MENU_SAVELOAD_H */
