// PARTIAL: added for Game/Bullet/CreateBullet
#ifndef GAME_ENTITY_H
#define GAME_ENTITY_H

#include "khtypes.h"
#include "System/Task.h"
#include "System/Graphic/Gprim.h"

struct FVECTOR;
class CState;

enum eObjType {
    eObjType_dummy = 0 /* values not needed yet */
};

class CFSM {
protected:
    CState* m_pCurrentState;
    CState* m_pNewState;
    UBYTE m_StateInitial[0x20]; /* CStateTemplate<CFSM> */
public:
    virtual ~CFSM();
    virtual void update();
    virtual void beginStateInitial();
    virtual void stateInitial();
    virtual void endStateInitial();
    virtual void endStateProc();
};

class CEntity : public CTask, public CFSM {
protected:
    UBYTE _m_0x70[0x70] __attribute__((aligned(16))); /* 0x070 m_vStartPos .. 0x0d0 m_vRootMoveScale */
private:
    eObjType m_EntityType; /* 0x0e0 */
    UINT m_uiEntityFlag;   /* 0x0e4 */
    UBYTE _m_0xe8[0xe8];   /* 0x0e8 m_uiDispOffFlag .. 0x1c4 m_pDamageString */

public:
    CEntity(CTask* inParent);
    virtual ~CEntity();
    virtual INT getTypeID() const;
    virtual BOOL initalize(INT inGroupID, INT inRsrcID);
    virtual BOOL initalize(INT inGroupID, INT inRsrcID, UINT iFlag);
    virtual void AfterInit();
    virtual void main();
    virtual void disp();
    virtual void onMsg(INT inMsg, void* inExtraInfo);
    virtual void getLockOnPos(FVECTOR& outPos);
    virtual void getHeadPos(FVECTOR& outPos);
    virtual void getUseCardPos(FVECTOR& outPos);
    virtual void getCameraTargetPos(FVECTOR& outPos);
    virtual void endStateProc();
    virtual void changeMotion(INT inIndex, TIMEQ inTq, BOOL inRootMove, TIMEQ inGr, INT inEffectNo, INT inSENo);
    virtual void updateBefore();
    virtual void updateAfter();
    virtual void updatePositionAfter();
    virtual void lastUpdate();

    void setEntityType(eObjType inType) { m_EntityType = inType; }
    eObjType getEntityType() { return m_EntityType; }
    UINT getFlag() { return m_uiEntityFlag; }
    void setEntityFlag(UINT inFlag, BOOL inOn) { UINT b = inOn ? inFlag : 0; m_uiEntityFlag |= b; }
};

#endif /* GAME_ENTITY_H */
