#ifndef FORM_MENU_TECHHELP_H
#define FORM_MENU_TECHHELP_H

#include "khtypes.h"
#include "System/Task.h"
#include "System/PadHandler.h"

class CTechHelp : public CTask, public CPadHandler {
private:
    INT m_nStep;       /* 0x50 */
    HANDLE m_hHelpLay; /* 0x54 */
    INT m_iRsecID;     /* 0x58 */
    INT m_fTimer;      /* 0x5c */
    static BOOL m_bEnd;

public:
    CTechHelp(CTask* p, USHORT nTeckKind);
    virtual ~CTechHelp();
    static BOOL isEnd() { return m_bEnd; }

private:
    virtual void main();
};

#endif /* FORM_MENU_TECHHELP_H */
