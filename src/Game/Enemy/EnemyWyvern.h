#ifndef GAME_ENEMY_ENEMYWYVERN_H
#define GAME_ENEMY_ENEMYWYVERN_H

#include "Game/Enemy/Heartless.h"

class CEnemyWyvern : public CHeartless {
private:
    FVECTOR m_vTarget;  /* 0x880 */
    TIMEQ m_tqAtkTimer; /* 0x890 */
    FLOAT m_fRotDir;    /* 0x894 */
    INT m_iRotFlyStep;  /* 0x898 */

public:
    CEnemyWyvern(CTask* inParent);
    virtual ~CEnemyWyvern();
    virtual BOOL initalize(INT inGroupID, INT inRsrcID);
protected:
    virtual void beginStateMain();
    virtual void stateMain();
    virtual void endStateMain();
    virtual void beginStateAttack1();
    virtual void stateAttack1();
    virtual void endStateAttack1();
    virtual void beginStateAttack2();
    virtual void stateAttack2();
    virtual void endStateAttack2();
};

#endif /* GAME_ENEMY_ENEMYWYVERN_H */
