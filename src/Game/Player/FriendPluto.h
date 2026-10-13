#ifndef GAME_PLAYER_FRIENDPLUTO_H
#define GAME_PLAYER_FRIENDPLUTO_H

#include "khtypes.h"
#include "Game/Player/Friend.h"

namespace Friend {

class CFriendPluto : public CFriend {
private:
    USHORT m_nTable;     /* 0x340 */
    USHORT m_nDiscovery; /* 0x342 */

public:
    CFriendPluto(CTask* inParent);
    virtual ~CFriendPluto();
private:
    void calcDiscovery();
    void dropDiscovery();
    void setDropBone();
protected:
    virtual void beginStateAction1();
    virtual void stateAction1();
    void endAction1();
public:
    virtual INT setSkill(INT inID);
};

extern unsigned char g_PlutoSkillID[4];
extern unsigned char g_PlutoItemTable[2][4];

} // namespace Friend

#endif /* GAME_PLAYER_FRIENDPLUTO_H */
