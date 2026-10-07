// PARTIAL: added for Game/Gimmick/GimmickSavePoint
#ifndef GAME_PLAYER_PLAYER_H
#define GAME_PLAYER_PLAYER_H

#include "khtypes.h"
#include "Game/Entity.h"

enum PlayerState {
    INIT = 0,
    START_WAIT = 1,
    STAND = 2,
    WALK = 3,
    RUN = 4
    /* ... */
};

class CPlayer : public CEntity {
private:
    UBYTE _m_0x1d0[0x130];       /* 0x1d0 .. 0x2fc */
    PlayerState m_PlayerState;   /* 0x300 */
    UBYTE _m_0x304[0x1c];        /* 0x304 .. 0x31c */
    INT m_iHP;                   /* 0x320 */
    INT m_iMaxHP;                /* 0x324 */
    UBYTE _m_0x328[0x3f4];       /* 0x328 .. 0x718 */
    HANDLE m_hTarget;            /* 0x71c */
    HANDLE m_hLockon;            /* 0x720 */

public:
    CPlayer(CTask* inParent);
    INT getPlayerState() { return m_PlayerState; }
    INT getHP() { return m_iHP; }
    INT getMaxHP() { return m_iMaxHP; }
    void addHP(INT inHP, INT inFlag, INT inEffect);
    void cancelLockon();
    HANDLE getLockonHandle() {
        HANDLE handle = m_hLockon;
        if (handle == 0 || handle == -1) handle = m_hTarget;
        return handle;
    }
};

extern CPlayer* g_pPlayer;

#endif /* GAME_PLAYER_PLAYER_H */
