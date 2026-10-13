#ifndef GAME_PLAYER_SUMMONMUSHU_H
#define GAME_PLAYER_SUMMONMUSHU_H

#include "khtypes.h"
#include "Game/Player/Summon.h"
#include "Game/Player/PlayerCamera.h"

namespace Summon {

class CSummonMushu : public CSummon {
public:
    CSummonMushu(CTask* inParent);
private:
    void adjustPlayer();
    void setFlare();
public:
    virtual ~CSummonMushu();
protected:
    virtual void updateBefore();
    virtual void beginStateAppear();
    virtual void beginStateAction1();
    virtual void stateAction1();
    virtual void beginStateExit();
    virtual void getCameraTargetPos(FVECTOR& outPos);
public:
    virtual INT setSkill(INT inID);
};

extern unsigned char g_MushuAttackID[4];
extern FVECTOR g_MushuOffset;
extern BCAM_PARAM g_MushuCamParam;

} // namespace Summon

#endif /* GAME_PLAYER_SUMMONMUSHU_H */
