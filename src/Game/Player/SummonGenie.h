#ifndef GAME_PLAYER_SUMMONGENIE_H
#define GAME_PLAYER_SUMMONGENIE_H

#include "khtypes.h"
#include "Game/Player/Summon.h"

namespace PlayerCamera {
struct BCAM_PARAM;
}

namespace Summon {

class CSummonGenie : public CSummon {
protected:
    INT m_submagic; /* 0x340 */

public:
    CSummonGenie(CTask* inParent);
private:
    INT exeGenieMagic();
public:
    virtual ~CSummonGenie();
protected:
    virtual void beginStateAppear();
    virtual void stateAppear();
    virtual void stateExit();
    virtual void beginStateAction1();
    virtual void stateAction1();
    void setMagic(INT inID);
public:
    virtual INT setSkill(INT inID);
};

extern unsigned char g_GenieMagicID[4];
extern unsigned char g_GenieMagicSkillID[4];
extern PlayerCamera::BCAM_PARAM g_GenieCamParam;

} // namespace Summon

#endif /* GAME_PLAYER_SUMMONGENIE_H */
