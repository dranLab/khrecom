#ifndef GAME_DEBUG_KISENEMY_H
#define GAME_DEBUG_KISENEMY_H

#include "Game/Enemy/BattleEnemy.h"

class CKisEnemy : public CBattleEnemy {
private:
    INT m_iStep;  /* 0x830 */
    FLOAT m_fDeg; /* 0x834 */

public:
    CKisEnemy(CTask* inParent);
    virtual ~CKisEnemy();
    virtual BOOL initalize(INT inGroupID, INT inRsrcID);

private:
    virtual void beginStateMain();
    virtual void stateMain();
    virtual void endStateMain();
};

#endif /* GAME_DEBUG_KISENEMY_H */
