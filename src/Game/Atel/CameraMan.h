// PARTIAL: added for Game/GameSub (only the singleton handling is known)
#ifndef GAME_ATEL_CAMERAMAN_H
#define GAME_ATEL_CAMERAMAN_H

#include "khtypes.h"
#include "System/Task.h"
#include "System/HeapMan.h"

namespace ATEL {

class CCameraMan : public CTask {
private:
    UBYTE _m_0x40[0x30];
    static CCameraMan* m_pInstance;

public:
    CCameraMan(CTask* pParent);
    virtual ~CCameraMan();
    void setup();

    /* the original's file is Game/ATEL/CameraMan.h (upper-case directory) */
    static void initialize(CTask* pParent) {
        if (m_pInstance) return;
        TRACE("CCameraMan::initialize\n");
#line 67 "c:/KHCOM/MainApp/Game/ATEL/CameraMan.h"
        if (!(__null == m_pInstance)) AssertApp(__FILE__, __LINE__, "__null == m_pInstance", 0);
        m_pInstance = new (com_malloc_d(sizeof(CCameraMan), __FILE__ "(68)", ({ UINT nRA; __asm__ volatile("daddu %0, $31, $0" : "=r"(nRA)); nRA; }))) CCameraMan(pParent);
#line 69
        if (!(m_pInstance)) AssertApp(__FILE__, __LINE__, "m_pInstance", 0);
        m_pInstance->setup();
    }
    static void finalize() {
        CCameraMan*& p = m_pInstance;
        if (p) delete p;
        p = __null;
    }
};

} // namespace ATEL

#endif /* GAME_ATEL_CAMERAMAN_H */
