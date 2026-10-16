#ifndef GAME_ENEMY_ENEMYBLACKFUNGUS_H
#define GAME_ENEMY_ENEMYBLACKFUNGUS_H

#include "Game/Enemy/Heartless.h"

class CEnemyBlackfungus : public CHeartless {
private:
    BOOL m_bPoisonTouch;     /* 0x880 */
    BOOL m_bMetal;           /* 0x884 */
    INT m_iBlackfungusStep;  /* 0x888 */
    TIMEQ m_tqMetalTimer;    /* 0x88c */

public:
    CEnemyBlackfungus(CTask* inParent);
    virtual ~CEnemyBlackfungus();
    virtual BOOL initalize(INT inGroupID, INT inRsrcID);
protected:
    virtual void updateBefore();
    virtual void beginStateMain();
    virtual void stateMain();
    virtual void endStateMain();
    virtual void beginStateAttack1();
    virtual void stateAttack1();
    virtual void beginStateAttack2();
    virtual void stateAttack2();
};

#endif /* GAME_ENEMY_ENEMYBLACKFUNGUS_H */
