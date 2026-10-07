// PARTIAL: added for Game/Bullet/CreateBullet
#ifndef GAME_ENTITY_H
#define GAME_ENTITY_H

#include "khtypes.h"
#include "System/Task.h"
// PARTIAL: Gprim.h include moved to Game/Bullet/Bullet.h for Game/Gimmick/GimmickHitMore (its strings are not in every includer)

struct FVECTOR;
class CState;
class CMovement; // PARTIAL: added for Game/Gimmick/GimmickBreakable
class CModel;    // PARTIAL: added for Game/Gimmick/GimmickPlantChild

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
    // PARTIAL: m_pMove split out for Game/Gimmick/GimmickBreakable
    // PARTIAL: m_pModels, m_iModelCount split out for Game/Gimmick/GimmickPlantChild
    UBYTE _m_0x70[0x10] __attribute__((aligned(16))); /* 0x070 m_vStartPos */
    CModel* m_pModels;            /* 0x080 */
    FVECTOR* m_pvModelColor;      /* 0x084 */
    INT m_iModelCount;            /* 0x088 */
    UBYTE _m_0x8c[0x14];          /* 0x08c m_pNLight .. 0x09c m_nTexAnmNum */
    CMovement* m_pMove;  /* 0x0a0 */
    UBYTE _m_0xa4[0x3c]; /* 0x0a4 m_pBodyColl .. 0x0d0 m_vRootMoveScale */
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
    virtual void getHeadPos(FVECTOR& outPos) const; // PARTIAL: const added for Game/Gimmick/GimmickHitMore
    virtual void getUseCardPos(FVECTOR& outPos);
    virtual void getCameraTargetPos(FVECTOR& outPos);
    virtual void endStateProc();
    virtual void changeMotion(INT inIndex, TIMEQ inTq, BOOL inRootMove, TIMEQ inGr, INT inEffectNo, INT inSENo);
    virtual void updateBefore();
    virtual void updateAfter();
    virtual void updatePositionAfter();
    virtual void lastUpdate();

    // PARTIAL: added for Game/Gimmick/GimmickBreakable
    void bodyCollOnBody(INT inKind);
    void bodyCollOffBody(INT inKind);
    void rideModelEnableAllCollision(BOOL inEnable);
    void rideModelChangeListR2();

    void setEntityType(eObjType inType) { m_EntityType = inType; }
    eObjType getEntityType() { return m_EntityType; }
    UINT getFlag() { return m_uiEntityFlag; }
    // PARTIAL: added for Game/Gimmick/GimmickPlantChild (getModel needs CModel's definition: System/Graphic/Model.h)
    INT getModelCount() { return m_iModelCount; }
    inline CModel* getModel();
    inline CModel* getModel(INT inIndex);
    void setEntityFlag(UINT inFlag, BOOL inOn) { UINT b = inOn ? inFlag : 0; m_uiEntityFlag |= b; }
    // PARTIAL: added for Game/Gimmick/GimmickPlantChild. Placeholder: the original's only flag setter is
    // setEntityFlag(UINT inSet, UINT inMask) (Entity.cpp in ref), whose body is not known yet.
    void clearEntityFlag(UINT inFlag) { m_uiEntityFlag &= ~inFlag; }
};

#endif /* GAME_ENTITY_H */
