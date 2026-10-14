#ifndef GAME_PLAYER_SUMMONBAMBI_H
#define GAME_PLAYER_SUMMONBAMBI_H

#include "khtypes.h"
#include "Game/Player/Summon.h"

namespace Summon {

class CSummonBambi : public CSummon {
public:
    CSummonBambi(CTask* inParent);
private:
    void dropPrize();
public:
    virtual ~CSummonBambi();
protected:
    virtual void beginStateAppear();
    virtual void beginStateAction1();
    virtual void stateAction1();
    virtual void beginStateAction2();
    virtual void stateAction2();
    virtual void beginStateExit();
public:
    virtual INT setSkill(INT inID);
};

extern unsigned char g_BambiAttackID[4];

} // namespace Summon

#endif /* GAME_PLAYER_SUMMONBAMBI_H */
