#ifndef GAME_GIMMICK_GIMMICKDOMINOPILLAR_H
#define GAME_GIMMICK_GIMMICKDOMINOPILLAR_H

#include "khtypes.h"
#include "Game/Gimmick/BaseGimmick.h"

class CGimmickDominoPillar : public CBaseGimmick {
private:
    CHAR m_Step;                     /* 0x220 */
    CHAR m_Stat;                     /* 0x221 */
    SHORT m_ID;                      /* 0x222 */
    BYTE m_OnceFlag;                 /* 0x224 */
    CGimmickDominoPillar* pChild[2]; /* 0x228 */

public:
    CGimmickDominoPillar(CTask* inParent, INT inRsrcID);
    virtual ~CGimmickDominoPillar();
    virtual BOOL initalize(INT inGroupID, INT inRsrcID);
    virtual INT getTypeID() const;
    void setStat(INT in_Stat) { m_Stat = in_Stat; }
    virtual void beginStateMain();
    virtual void stateMain();
    virtual void endStateMain();
    virtual void AfterInit();
    virtual void beginStateHitAttack();
    virtual void stateHitAttack();
    virtual void endStateHitAttack();
};

#endif /* GAME_GIMMICK_GIMMICKDOMINOPILLAR_H */
