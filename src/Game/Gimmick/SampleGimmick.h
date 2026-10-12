#ifndef GAME_GIMMICK_SAMPLEGIMMICK_H
#define GAME_GIMMICK_SAMPLEGIMMICK_H

#include "khtypes.h"
#include "Game/Gimmick/BaseGimmick.h"

class CSampleGimmick : public CBaseGimmick {
public:
    CSampleGimmick(CTask* inParent);
    virtual ~CSampleGimmick();
    virtual BOOL initalize(INT inGroupID, INT inRsrcID);
    virtual void beginStateMain();
    virtual void stateMain();
    virtual void endStateMain();
    virtual void beginStateHitAttack();
    virtual void stateHitAttack();
    virtual void endStateHitAttack();
};

class CSampleGimmickForEffectID : public CBaseGimmick {
public:
    CSampleGimmickForEffectID(CTask* inParent, const ZONEPARTS& inParts);
    virtual ~CSampleGimmickForEffectID();
    virtual void beginStateMain();
    virtual void stateMain();
    virtual void endStateMain();
    virtual void beginStateHitAttack();
    virtual void stateHitAttack();
    virtual void endStateHitAttack();
};

#endif /* GAME_GIMMICK_SAMPLEGIMMICK_H */
