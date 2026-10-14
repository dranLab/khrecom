#ifndef GAME_GIMMICK_GIMMICKTUTORIAL_H
#define GAME_GIMMICK_GIMMICKTUTORIAL_H

#include "khtypes.h"
#include "Game/Gimmick/BaseGimmick.h"
#include "Game/Gimmick/GimmickCarry.h"

class CGimmickBreakableTu : public CBaseGimmick {
private:
    CHAR m_Step;         /* 0x220 */
    CHAR m_Stat;         /* 0x221 */
    FLOAT m_WaitCounter; /* 0x224 */

public:
    CGimmickBreakableTu(CTask* inParent);
    virtual ~CGimmickBreakableTu();
    virtual BOOL initalize(INT inGroupID, INT inRsrcID);
    virtual void beginStateMain();
    virtual void stateMain();
    virtual void endStateMain();
    virtual void beginStateHitAttack();
    virtual void stateHitAttack();
    virtual void endStateHitAttack();
};

class CGimmickCarryTu : public CGimmickCarry {
private:
    CHAR m_Step; /* 0x260 */

public:
    CGimmickCarryTu(CTask* inParent);
    virtual ~CGimmickCarryTu();
    virtual BOOL initalize(INT inGroupID, INT inRsrcID);
    virtual BOOL checkCarryPermission();
    virtual void beginStateMain();
    virtual void stateMain();
};

class CGimmickShadowTu : public CBaseGimmick {
private:
    CHAR m_Step;         /* 0x220 */
    CHAR m_Atk;          /* 0x221 */
    FLOAT m_WaitCounter; /* 0x224 */

public:
    CGimmickShadowTu(CTask* inParent);
    virtual ~CGimmickShadowTu();
    virtual BOOL initalize(INT inGroupID, INT inRsrcID);
    virtual void beginStateMain();
    virtual void stateMain();
    virtual void endStateMain();
    virtual INT getTypeID() const;
    virtual void beginStateHitAttack();
    virtual void stateHitAttack();
    virtual void endStateHitAttack();
};

#endif /* GAME_GIMMICK_GIMMICKTUTORIAL_H */
