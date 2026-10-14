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
    static void setFlag(UINT inKind, UINT inFlag, BOOL inOn); // PARTIAL: added for Game/Gimmick/GimmickSavePoint
    static INT getInt(UINT inKind, UINT inIndex); // PARTIAL: added for Game/Gimmick/GimmickWarpPoint
    static void setInt(UINT inKind, UINT inIndex, INT inValue); // PARTIAL: added for Form/Menu/GameOver
    static BOOL getFlag(UINT inKind, UINT inFlag); // PARTIAL: added for Form/Menu/Menu
    static BYTE getByte(UINT inKind, UINT inIndex); // PARTIAL: added for Game/Player/PlayerControl
};

#endif /* GAME_GAMEWORK_H */
