#ifndef GAME_PLAYER_SUMMONSINBA_H
#define GAME_PLAYER_SUMMONSINBA_H

#include "khtypes.h"
#include "Game/Player/Summon.h"

namespace Summon {

class CSummonSinba : public CSummon {
public:
    CSummonSinba(CTask* inParent);
private:
    INT exeSinbaAttack();
public:
    virtual ~CSummonSinba();
protected:
    virtual void stateAppear();
    virtual void beginStateAction1();
    virtual void stateAction1();
    virtual void beginStateExit();
public:
    virtual INT setSkill(INT inID);
};

extern unsigned char g_ShibaAttackID[4];

} // namespace Summon

#endif /* GAME_PLAYER_SUMMONSINBA_H */
