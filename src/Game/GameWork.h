// PARTIAL: added for Form/Menu/MoogleShop
#ifndef GAME_GAMEWORK_H
#define GAME_GAMEWORK_H

#include "khtypes.h"
#include "Game/StorageWork.h"

class CGameWork {
private:
    static STORAGE::MCWork* m_pMCWork;

public:
    static STORAGE::MCWork& progMCW() { return *m_pMCWork; }
};

#endif /* GAME_GAMEWORK_H */
