#ifndef GAME_PLAYER_PLAYERANIM_H
#define GAME_PLAYER_PLAYERANIM_H
// PARTIAL: added for Game/Player/Player (the tables themselves are still data from Game/Player/PlayerAnim's asm)

#include "khtypes.h"

#define ANIM_MAX 71

struct MOT_NAME_TBL {
    CHAR* pFileName;
    INT iIndex;
};

class PL_ANIM {
public:
    static MOT_NAME_TBL g_MotNameTbl_sora[72];
    static MOT_NAME_TBL* g_MotNameTbl[7];
    static char* g_pPlayerModelName[7];
};

#endif // GAME_PLAYER_PLAYERANIM_H
