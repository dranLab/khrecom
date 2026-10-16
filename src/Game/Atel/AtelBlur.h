// PARTIAL: added for Game/GameSub (only the singleton handling is known)
#ifndef GAME_ATEL_ATELBLUR_H
#define GAME_ATEL_ATELBLUR_H

#include "khtypes.h"
#include "System/Task.h"
#include "System/HeapMan.h"

namespace ATEL {

class CAtelBlur : public CTask {
private:
    UBYTE _m_0x40[0x60];
    static CAtelBlur* m_pInstance;

public:
    CAtelBlur(CTask* pParent);
    virtual ~CAtelBlur();

    /* the original's file is Game/ATEL/AtelBlur.h (upper-case directory) */
    static void initialize(CTask* pParent) {
        if (m_pInstance) return;
        TRACE("CAtelBlur::initialize\n");
#line 80 "c:/KHCOM/MainApp/Game/ATEL/AtelBlur.h"
        if (!(__null == m_pInstance)) AssertApp(__FILE__, __LINE__, "__null == m_pInstance", 0);
        m_pInstance = new (com_malloc_d(sizeof(CAtelBlur), __FILE__ "(81)", ({ UINT nRA; __asm__ volatile("daddu %0, $31, $0" : "=r"(nRA)); nRA; }))) CAtelBlur(pParent);
#line 82
        if (!(m_pInstance)) AssertApp(__FILE__, __LINE__, "m_pInstance", 0);
    }
    static void finalize() {
        CAtelBlur*& p = m_pInstance;
        if (p) delete p;
        p = __null;
    }
};

} // namespace ATEL

#endif /* GAME_ATEL_ATELBLUR_H */
