#ifndef GAME_PLAYER_SUMMONDUMBO_H
#define GAME_PLAYER_SUMMONDUMBO_H

#include "khtypes.h"
#include "Game/Player/Summon.h"

namespace Summon {

class CSummonDumbo : public CSummon {
public:
    CSummonDumbo(CTask* inParent);
    virtual ~CSummonDumbo();
protected:
    virtual void beginStateAppear();
    virtual void stateAppear();
    virtual void beginStateAction1();
    virtual void stateAction1();
    virtual void beginStateExit();
public:
    virtual INT setSkill(INT inID);
};

extern unsigned char g_DumboAttackID[4];

} // namespace Summon

#endif /* GAME_PLAYER_SUMMONDUMBO_H */
