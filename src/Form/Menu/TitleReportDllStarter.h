// PARTIAL: added for System/MainTaskFactory
#ifndef FORM_MENU_TITLEREPORTDLLSTARTER_H
#define FORM_MENU_TITLEREPORTDLLSTARTER_H

#include "khtypes.h"
#include "System/Task.h"
#include "System/MainApp.h"
#include "System/HeapMan.h"
#include "System/Debug.h"
#include "System/Fsm.h"

/* Line numbers are the original ones (__LINE__ in messages); only lines 55-57 (create) are known. */
/* PARTIAL: path override added for Form/Menu/TitleReportDllStarter ("c:/KHCOM/MainApp/...") */
#ifndef TITLEREPORTDLLSTARTER_H_PATH
#define TITLEREPORTDLLSTARTER_H_PATH "Form/Menu/TitleReportDllStarter.h"
#endif
#line 42 TITLEREPORTDLLSTARTER_H_PATH
class CTitleReportDllStarter : public CTask, public CFSM {
private:
    static CTitleReportDllStarter* m_pInstance;
    INT m_LaunchMode;   /* 0x6c */
    INT m_nParam;       /* 0x70 */
    HANDLE m_hChildTask; /* 0x74 */
    CStateTemplate<CTitleReportDllStarter> m_State[3]; /* 0x78 */

public:
    CTitleReportDllStarter(CTask* pParent, INT nArg1, INT nArg2);
    virtual ~CTitleReportDllStarter();
    virtual void main();
    virtual void disp();

#line 50
    static CTitleReportDllStarter* create(CTask* pParent, INT nArg1, INT nArg2) {
        if (m_pInstance != NULL) {
            return NULL;
        }
        TRACE("CTitleReportDllStarter::create\n");
        if (!(__null == m_pInstance)) AssertApp(__FILE__, __LINE__, "__null == m_pInstance", 0);
        m_pInstance = new (com_malloc_d(sizeof(CTitleReportDllStarter), __FILE__ "(56)", ({ UINT nRA; __asm__ volatile("daddu %0, $31, $0" : "=r"(nRA)); nRA; }))) CTitleReportDllStarter(pParent, nArg1, nArg2);
        if (!(m_pInstance)) AssertApp(__FILE__, __LINE__, "m_pInstance", 0);
        return m_pInstance;
    }

protected:
    enum { STATE_INIT = 0, STATE_DLL_LOAD = 1, STATE_MAIN = 2, STATE_MAX = 3 }; // names partly guessed
    void beginStateInit();
    void stateInit();
    void endStateInit();
    void beginStateDllLoad();
    void stateDllLoad();
    void endStateDllLoad();
    void beginStateMain();
    void stateMain();
    void endStateMain();
    void changeState(INT state);
};

#endif /* FORM_MENU_TITLEREPORTDLLSTARTER_H */
