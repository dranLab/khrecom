#ifndef GAME_GIMMICK_GIMMICKTIMEBOMB_H
#define GAME_GIMMICK_GIMMICKTIMEBOMB_H

#include "khtypes.h"
#include "Game/Gimmick/BaseGimmick.h"

class CGimmickTimeBomb : public CBaseGimmick {
private:
    CHAR m_Step;         /* 0x220 */
    CHAR m_Stat;         /* 0x221 */
    CHAR m_Kill;         /* 0x222 */
    CHAR m_Prize;        /* 0x223 */
    FLOAT m_WaitCounter; /* 0x224 */

public:
    CGimmickTimeBomb(CTask* inParent);
    virtual ~CGimmickTimeBomb();
    virtual BOOL initalize(INT inGroupID, INT inRsrcID);
    virtual void AfterInit();
    virtual void beginStateMain();
    virtual void stateMain();
    virtual void endStateMain();
    void clockMain();
    void bombMain();
    virtual void beginStateHitAttack();
    virtual void stateHitAttack();
    virtual void endStateHitAttack();
};

#endif /* GAME_GIMMICK_GIMMICKTIMEBOMB_H */
