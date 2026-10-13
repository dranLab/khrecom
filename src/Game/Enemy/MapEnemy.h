// PARTIAL: added for Game/Enemy/MapEnemyShadow
#ifndef GAME_ENEMY_MAPENEMY_H
#define GAME_ENEMY_MAPENEMY_H

#include "khtypes.h"
#include "Game/BodyCollision.h"
#include "System/Graphic/Anim.h"
#include "Game/Entity.h"
#include "System/Vector.h"

class CState;
struct ENEMY_PARAM;

class CMapEnemy : public CEntity {
protected:
    FVECTOR m_vTarget;           /* 0x1d0 */
    ENEMY_PARAM* m_pEnemyParam;  /* 0x1e0 */
    FLOAT m_fWalkSpeed;          /* 0x1e4 */
    FLOAT m_fRunSpeed;           /* 0x1e8 */
    FLOAT m_fViewAngle;          /* 0x1ec */
    FLOAT m_fViewArea;           /* 0x1f0 */
    INT m_iAppearType;           /* 0x1f4 */
private:
    UINT m_uiFlag;               /* 0x1f8 */
    UBYTE _m_0x1fc[0x144];       /* 0x1fc m_iSysStep .. m_MapEnemyState[9] */

public:
    CMapEnemy(CTask* inParent, CState* inState = __null);
    virtual ~CMapEnemy();
    virtual INT getTypeID() const;
    virtual BOOL initalize(INT inGroupID, INT inRsrcID);
    virtual void onMsg(INT inMsg, void* inExtraInfo);
    void setMapEnemyFlag(UINT inFlag, BOOL inOn) {
        if (inOn) m_uiFlag |= inFlag;
        else m_uiFlag &= ~inFlag;
    }
    static FLOAT getAppearDistance();
    static FLOAT getDisappearDistance();

protected:
    void changeMapEnemyMotion(INT inMotion, INT inArg1, INT inArg2, INT inArg3);
    virtual void updateBefore();
    virtual void lastUpdate();
    void changeMapEnemyState(INT inState);
    BOOL checkGround();
    BOOL canAppear() const;
    BOOL canPlayerChase(FLOAT inRange, INT inArg);
    void getRandomTargetPos(FVECTOR& outPos, FLOAT inRange, INT inArg);
    virtual void beginStateAppear();
    virtual void stateAppear();
    virtual void endStateAppear();
    virtual void beginStateMain();
    virtual void stateMain();
    virtual void endStateMain();
    virtual void beginStateSleeping();
    virtual void stateSleeping();
    virtual void endStateSleeping();
    virtual void beginStateStanding();
    virtual void stateStanding();
    virtual void endStateStanding();
    virtual void beginStateDamage();
    virtual void stateDamage();
    virtual void endStateDamage();
    virtual void beginStateFaint();
    virtual void stateFaint();
    virtual void endStateFaint();
    virtual void beginStateStop();
    virtual void stateStop();
    virtual void endStateStop();
    virtual void beginStateConfuse();
    virtual void stateConfuse();
    virtual void endStateConfuse();
    virtual void beginStateDisappear();
    virtual void stateDisappear();
    virtual void endStateDisappear();
};

#endif /* GAME_ENEMY_MAPENEMY_H */
