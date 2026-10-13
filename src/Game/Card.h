// PARTIAL: added for Form/Menu/Menu
#ifndef GAME_CARD_H
#define GAME_CARD_H

#include "khtypes.h"
#include "System/MainApp.h"
#include "System/HeapMan.h"
#include "System/Debug.h"

class CCard {
private:
    static CCard* m_pInstance;

    CCard();

public:
    /* Placeholder for the original singleton creation (CCard::initialize): its strings ("Game/Card.h",
     * "__null == m_pInstance", "Game/Card.h(66)") are in .rodata of every includer. Only line 66 is
     * known (it is part of the allocation message). */
#line 60 "Game/Card.h"
    static void _initializeCheck() {
        if (!(__null == m_pInstance)) AssertApp(__FILE__, __LINE__, "__null == m_pInstance", 0);
        com_malloc_d(4, __FILE__ "(66)", (UINT)-1);
    }

    static INT searchCard(USHORT inID, SHORT inNumber);
    static INT searchCardKind(USHORT inID, SHORT inNumber);
};

#endif /* GAME_CARD_H */
