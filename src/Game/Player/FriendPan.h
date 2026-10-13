#ifndef GAME_PLAYER_FRIENDPAN_H
#define GAME_PLAYER_FRIENDPAN_H

#include "khtypes.h"
#include "Game/Player/Friend.h"

namespace Friend {

class CFriendPan : public CFriend {
private:
    INT m_nLogCount;    /* 0x340 */
    INT m_nLogCountMax; /* 0x344 */
    TIMEQ* m_pTrgLog;   /* 0x348 */

public:
    CFriendPan(CTask* inParent);
    virtual ~CFriendPan();
protected:
    virtual void beginStateAction1();
    virtual void stateAction1();
    virtual void beginStateAction2();
    virtual void stateAction2();
    virtual void beginStateAction3();
    virtual void stateAction3();
public:
    virtual INT setSkill(INT inID);
};

extern unsigned char g_PanAttackID[4];

} // namespace Friend

#endif /* GAME_PLAYER_FRIENDPAN_H */
