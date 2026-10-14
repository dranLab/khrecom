// PARTIAL: added for System/RootTask
#ifndef GAME_ATEL_CHATMAN_H
#define GAME_ATEL_CHATMAN_H

#include "khtypes.h"
#include "System/MainApp.h"
#include "System/HeapMan.h"
#include "System/Task.h" // PARTIAL: added for Game/Gimmick/GimmickTreasureBox

/* Includers define the placement operator new before including this header. */

namespace ATEL {

// PARTIAL: added for Game/Gimmick/GimmickTreasureBox
class ICommandAccepter {
public:
    virtual INT reqCommand(INT nCmd, void* pParam, INT nID) = 0;
};

/* Only the singleton creation is real (its strings and line numbers are in .rodata of every includer).
 * PARTIAL: bases CTask, ICommandAccepter (0x40) added for Game/Gimmick/GimmickTreasureBox (sizeof 0x50). */
class CChatMan : public CTask, public ICommandAccepter {
private:
    static CChatMan* m_pInstance;

    CChatMan();
    virtual ~CChatMan();
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
    // PARTIAL: added for Game/Gimmick/GimmickTreasureBox
    /* PARTIAL: reference added for Form/Menu/WorldSelect (the load of m_pInstance stays after stores) */
#line 57 "Game/Atel/ChatMan.h"
    static CChatMan& instance() { CChatMan*& instance = m_pInstance;
        if (!(instance)) AssertApp(__FILE__, __LINE__, "m_pInstance", 0);
        return *instance;
    }
    virtual INT reqCommand(INT nCmd, void* pParam, INT nID);
};

} // namespace ATEL

#endif /* GAME_ATEL_CHATMAN_H */
