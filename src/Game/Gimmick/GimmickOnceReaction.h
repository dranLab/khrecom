#ifndef GAME_GIMMICK_GIMMICKONCEREACTION_H
#define GAME_GIMMICK_GIMMICKONCEREACTION_H

#include "khtypes.h"
#include "Game/Gimmick/BaseGimmick.h"

class CGimmickOnceReaction : public CBaseGimmick {
private:
    SHORT m_ID;      /* 0x220 */
    CHAR m_Step;     /* 0x222 */
    CHAR m_Stat;     /* 0x223 */
    BYTE m_OnceFlag; /* 0x224 */
    BYTE m_Drop;     /* 0x225 */

public:
    CGimmickOnceReaction(CTask* inParent, INT in_GimID);
    virtual ~CGimmickOnceReaction();
    virtual BOOL initalize(INT inGroupID, INT inRsrcID);
    virtual void AfterInit();
    virtual void beginStateMain();
    virtual void stateMain();
    virtual void endStateMain();
    void defaultMain();
    void flowerMain();
    void shellMain();
    void tuboMain();
    void GimColChangeMain();
    virtual void beginStateHitAttack();
    virtual void stateHitAttack();
    virtual void endStateHitAttack();
};

#endif /* GAME_GIMMICK_GIMMICKONCEREACTION_H */
