// PARTIAL: added for Game/Bullet/CreateBullet
#ifndef GAME_ENTITY_H
#define GAME_ENTITY_H

#include "khtypes.h"
#include "System/Task.h"
// PARTIAL: Gprim.h include moved to Game/Bullet/Bullet.h for Game/Gimmick/GimmickHitMore (its strings are not in every includer)

union FVECTOR;
class CState;
class CMovement; // PARTIAL: added for Game/Gimmick/GimmickBreakable
class CModel;    // PARTIAL: added for Game/Gimmick/GimmickPlantChild
union FMATRIX;   // PARTIAL: added for Game/Gimmick/GimmickWindowLid

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
    UINT isHeartless() const { return m_uiEntityFlag & 0x20000; } // PARTIAL: added for Game/Bullet/CreateBullet
    // PARTIAL: added for Game/Gimmick/GimmickPlantChild (getModel needs CModel's definition: System/Graphic/Model.h)
    INT getModelCount() { return m_iModelCount; }
    inline CModel* getModel();
    inline CModel* getModel(INT inIndex);
    // Entity.cpp also emits this out of line (setEntityFlag__7CEntityUiUi): or inSet, then clear inMask.
    // PARTIAL: changed for Game/Gimmick/GimmickSavePoint - the store goes through a reference (not a struct
    // member access), so the scheduler does not move a following global load above it.
    void setEntityFlag(UINT inSet, UINT inMask) { UINT& f = m_uiEntityFlag; f = (f | inSet) & ~inMask; }
    // PARTIAL: added for Game/Gimmick/GimmickSavePoint
    const FVECTOR& getPos() const;
    FLOAT calcDistanceXZ(const CEntity* inEntity) const;
    HANDLE setEffect(const CHAR* inName, UINT inFlag, const FVECTOR* inPos, const FVECTOR* inRot, FLOAT inScale);
    BOOL isRideOnPlayer();
    // PARTIAL: added for Game/Gimmick/GimmickWindowLid
    INT getBoneIndex(const CHAR* inBoneName, INT inModelIndex) const;
    BOOL isMotionEnd() const;
    BOOL isHangOnPlayer();
    void rideModelEnableChipCollision(INT inIdx, BOOL inSw);
    void rideModelSetChipLocalMatrix(INT inIndex, const FMATRIX* in_pMatrix);
};

#endif /* GAME_ENTITY_H */
