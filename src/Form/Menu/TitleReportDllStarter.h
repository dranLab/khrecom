// PARTIAL: added for System/MainTaskFactory
#ifndef FORM_MENU_TITLEREPORTDLLSTARTER_H
#define FORM_MENU_TITLEREPORTDLLSTARTER_H

#include "khtypes.h"
#include "System/Task.h"
#include "System/MainApp.h"
#include "System/HeapMan.h"
#include "System/Debug.h"

/* Line numbers are the original ones (__LINE__ in messages); only lines 55-57 (create) are known. */
#line 42 "Form/Menu/TitleReportDllStarter.h"
class CTitleReportDllStarter : public CTask {
private:
    static CTitleReportDllStarter* m_pInstance;
    BYTE _pad[0xE0 - sizeof(CTask)]; // members not known yet

public:
    CTitleReportDllStarter(CTask* pParent, INT nArg1, INT nArg2);

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
};

#endif /* FORM_MENU_TITLEREPORTDLLSTARTER_H */
