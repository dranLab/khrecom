#ifndef GAME_ENEMY_SAMPLEENEMY_H
#define GAME_ENEMY_SAMPLEENEMY_H

#include "Game/Enemy/Heartless.h"
#include "System/Fsm.h"

class CSampleEnemy : public CHeartless {
private:
    INT m_iStep;  /* 0x880 */
    FLOAT m_fDeg; /* 0x884 */

public:
    CSampleEnemy(CTask* inParent);
    virtual ~CSampleEnemy();
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

class CSampleEnemy2 : public CHeartless {
private:
    CStateTemplate<CSampleEnemy2> m_State[2]; /* 0x880 */

public:
    CSampleEnemy2(CTask* inParent);
    virtual ~CSampleEnemy2();
    virtual BOOL initalize(INT inGroupID, INT inRsrcID);
protected:
    void beginIdle();
    void idle();
    void endIdle();
    void beginMove();
    void move();
    void endMove();
    virtual void beginStateAttack1();
    virtual void stateAttack1();
    virtual void endStateAttack1();
    virtual void beginStateAttack2();
    virtual void stateAttack2();
    virtual void endStateAttack2();
};

#endif /* GAME_ENEMY_SAMPLEENEMY_H */
