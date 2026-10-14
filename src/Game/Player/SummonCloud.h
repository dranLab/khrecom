#ifndef GAME_PLAYER_SUMMONCLOUD_H
#define GAME_PLAYER_SUMMONCLOUD_H

#include "khtypes.h"
#include "Game/Player/Summon.h"

namespace Summon {

class CSummonCloud : public CSummon {
private:
    HANDLE m_hEffect0;   /* 0x340 */
    HANDLE m_hEffect1;   /* 0x344 */
    FLOAT m_fAlpha;      /* 0x348 */
    FLOAT m_fWingsAlpha; /* 0x34c */

public:
    CSummonCloud(CTask* inParent);
private:
    void fadeWings_();
public:
    virtual ~CSummonCloud();
protected:
    virtual void updateBefore();
    virtual void beginStateAppear();
    virtual void stateAppear();
    virtual void beginStateAction1();
    virtual void stateAction1();
    virtual void beginStateAction2();
    virtual void stateAction2();
    void endAction2();
    virtual void beginStateExit();
    virtual void stateExit();
    HANDLE setAppearEffect(const FVECTOR* inPos);
public:
    virtual INT setSkill(INT inID);
};

extern unsigned char g_CloudAttackID[4];

} // namespace Summon

#endif /* GAME_PLAYER_SUMMONCLOUD_H */
