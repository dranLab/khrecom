// PARTIAL: added for Game/GameSub (only the singleton handling is known)
#ifndef GAME_ATEL_ATELCROSSFADE_H
#define GAME_ATEL_ATELCROSSFADE_H

#include "khtypes.h"
#include "System/Task.h"
#include "System/HeapMan.h"

namespace ATEL {

class CAtelCrossFade : public CTask {
private:
    UBYTE _m_0x40[0x90];
    static CAtelCrossFade* m_pInstance;

public:
    CAtelCrossFade(CTask* pParent);
    virtual ~CAtelCrossFade();

    /* the original's file is Game/ATEL/AtelCrossFade.h (upper-case directory) */
    static void initialize(CTask* pParent) {
        if (m_pInstance) return;
        TRACE("CAtelCrossFade::initialize\n");
#line 110 "c:/KHCOM/MainApp/Game/ATEL/AtelCrossFade.h"
        if (!(__null == m_pInstance)) AssertApp(__FILE__, __LINE__, "__null == m_pInstance", 0);
        m_pInstance = new (com_malloc_d(sizeof(CAtelCrossFade), __FILE__ "(111)", ({ UINT nRA; __asm__ volatile("daddu %0, $31, $0" : "=r"(nRA)); nRA; }))) CAtelCrossFade(pParent);
#line 112
        if (!(m_pInstance)) AssertApp(__FILE__, __LINE__, "m_pInstance", 0);
    }
    static void finalize() {
        CAtelCrossFade*& p = m_pInstance;
        if (p) delete p;
        p = __null;
    }
};

} // namespace ATEL

#endif /* GAME_ATEL_ATELCROSSFADE_H */
