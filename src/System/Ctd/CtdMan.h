// PARTIAL: added for Game/CardKind
#ifndef SYSTEM_CTD_CTDMAN_H
#define SYSTEM_CTD_CTDMAN_H

#include "khtypes.h"
#include "System/MainApp.h"
#include "System/HeapMan.h"

/* __FILE__ of this header in the original differs per includer ("System/CTD/CtdMan.h" in most, also
 * "System/Ctd/CtdMan.h" and "c:/KHCOM/MainApp/System/CTD/CtdMan.h"): define CTDMAN_H_PATH before
 * including it to pick another one. Line numbers below are the original ones (__LINE__ in messages). */
#ifndef CTDMAN_H_PATH
#define CTDMAN_H_PATH "System/CTD/CtdMan.h"
#endif

struct CtdBlock;

#line 20 CTDMAN_H_PATH
class CCtdMan {
public:
    static CCtdMan* m_pInstance;

    CCtdMan();
    virtual ~CCtdMan();

    static CCtdMan& initialize() {
        UINT nRA = 0;
#line 38
        if (m_pInstance != NULL) AssertApp(__FILE__, __LINE__, "__null == m_pInstance", 0);
        m_pInstance = (CCtdMan*)com_malloc_d(sizeof(CCtdMan), __FILE__ "(39)", nRA);
        return *m_pInstance;
    }

    static CHAR* getText(UINT ctdID) {
#line 50
        if (m_pInstance == NULL) AssertApp(__FILE__, __LINE__, "m_pInstance", 0);
        return m_pInstance->_getText(ctdID);
    }

private:
    CHAR* _getText(UINT ctdID);
};

#endif /* SYSTEM_CTD_CTDMAN_H */
