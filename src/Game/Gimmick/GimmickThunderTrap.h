#ifndef GAME_GIMMICK_GIMMICKTHUNDERTRAP_H
#define GAME_GIMMICK_GIMMICKTHUNDERTRAP_H

#include "khtypes.h"
#include "Game/Gimmick/BaseGimmick.h"

class CCollAttack;

class CGimmickThunderTrap : public CBaseGimmick {
private:
    CHAR m_Step;          /* 0x220 */
    CHAR m_Stat;          /* 0x221 */
    FLOAT m_WaitCounter;  /* 0x224 */

public:
    CGimmickThunderTrap(CTask* inParent);
    virtual ~CGimmickThunderTrap();
    virtual void AfterInit();
    virtual BOOL initalize(INT inGroupID, INT inRsrcID);
    virtual void beginStateMain();
    virtual void stateMain();
    virtual void endStateMain();
    virtual void beginStateHitAttack();
    virtual void stateHitAttack();
    virtual void endStateHitAttack();
};

class CGimmickThunderLaser : public CBaseGimmick {
private:
    CHAR m_Step;           /* 0x220 */
    CHAR m_Stat;           /* 0x221 */
    FLOAT m_WaitCounter;   /* 0x224 */
    HANDLE m_hEfx;         /* 0x228 */
    CCollAttack* m_pAttack; /* 0x22c */

public:
    CGimmickThunderLaser(CTask* inParent);
    virtual ~CGimmickThunderLaser();
    virtual void AfterInit();
    virtual BOOL initalize(INT inGroupID, INT inRsrcID);
    virtual void beginStateMain();
    virtual void stateMain();
    virtual void endStateMain();
    virtual void beginStateHitAttack();
    virtual void stateHitAttack();
    virtual void endStateHitAttack();
};

#endif /* GAME_GIMMICK_GIMMICKTHUNDERTRAP_H */
