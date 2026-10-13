// PARTIAL: added for System/RootTask
#ifndef GAME_ATEL_CHATMAN_H
#define GAME_ATEL_CHATMAN_H

#include "khtypes.h"
#include "System/MainApp.h"
#include "System/HeapMan.h"

/* Includers define the placement operator new before including this header. */

namespace ATEL {

/* Only the singleton creation is real (its strings and line numbers are in .rodata of every includer);
 * the layout is a placeholder of the original size (0x50). */
class CChatMan {
private:
    char _pad[0x50];
    static CChatMan* m_pInstance;

    CChatMan();
    void setup();

public:
#line 45 "Game/Atel/ChatMan.h"
    static CChatMan& initialize() {
        if (!(__null == m_pInstance)) AssertApp(__FILE__, __LINE__, "__null == m_pInstance", 0);
        m_pInstance = new (com_malloc_d(sizeof(CChatMan), __FILE__ "(47)", ({ UINT nRA; __asm__ volatile("daddu %0, $31, $0" : "=r"(nRA)); nRA; }))) CChatMan;
        if (!(m_pInstance)) AssertApp(__FILE__, __LINE__, "m_pInstance", 0);
        m_pInstance->setup();
        return *m_pInstance;
    }
};

} // namespace ATEL

#endif /* GAME_ATEL_CHATMAN_H */
