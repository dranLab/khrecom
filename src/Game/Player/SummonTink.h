#ifndef GAME_PLAYER_SUMMONTINK_H
#define GAME_PLAYER_SUMMONTINK_H

#include "khtypes.h"
#include "Game/Player/Summon.h"
#include "Game/Player/PlayerCamera.h"

namespace Summon {

class CSummonTink : public CSummon {
private:
    FLOAT m_fRecover;     /* 0x340 */
    FLOAT m_fOldRecover;  /* 0x344 */
    FLOAT m_fNowRecover;  /* 0x348 */
    FLOAT m_fAddRecover;  /* 0x34c */
    FLOAT m_fMaxRecover;  /* 0x350 */
    HANDLE m_hEffRecover; /* 0x354 */
    TIMEQ m_tqEffRecover; /* 0x358 */

public:
    CSummonTink(CTask* inParent);
private:
    void initRecover();
    void fadeEffRecover(INT inFlag);
    void adjustPlayer();
public:
    virtual ~CSummonTink();
protected:
    virtual void beginStateAppear();
    virtual void stateAppear();
    virtual void beginStateAction1();
    virtual void stateAction1();
    virtual void beginStateExit();
    virtual void stateExit();
public:
    virtual INT setSkill(INT inID);
};

extern unsigned char g_TinkAttackID[4];
extern FVECTOR g_TinkOffset;
extern BCAM_PARAM g_TinkCamParam;

} // namespace Summon

#endif /* GAME_PLAYER_SUMMONTINK_H */
