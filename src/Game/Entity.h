// PARTIAL: added for Game/Bullet/CreateBullet
#ifndef GAME_ENTITY_H
#define GAME_ENTITY_H

#include "khtypes.h"
#include "System/Task.h"
// PARTIAL: Gprim.h include moved to Game/Bullet/Bullet.h for Game/Gimmick/GimmickHitMore (its strings are not in every includer)

union FVECTOR;
class CState;
class CMovement; // PARTIAL: added for Game/Gimmick/GimmickBreakable
class CAttackMotion; // PARTIAL: added for Game/Gimmick/GimmickToldPipe
class CModel;    // PARTIAL: added for Game/Gimmick/GimmickPlantChild
union FMATRIX;   // PARTIAL: added for Game/Gimmick/GimmickWindowLid
class CAnim;     // PARTIAL: added for Game/Gimmick/GimmickLotusLeaf
class CBodyCollision; // PARTIAL: added for Game/Gimmick/GimmickGuillotine

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
    // PARTIAL: m_pAnim split out for Game/Gimmick/GimmickLotusLeaf
    UBYTE _m_0x8c[0x8];           /* 0x08c m_pNLight, m_pGLight */
    CAnim* m_pAnim;               /* 0x094 */
    UBYTE _m_0x98[0x8];           /* 0x098 m_pTexAnm, m_nTexAnmNum */
    CMovement* m_pMove;  /* 0x0a0 */
    // PARTIAL: m_pAttack, m_iID split out for Game/Gimmick/GimmickToldPipe
    CBodyCollision* m_pBodyColl; /* 0x0a4 */ // PARTIAL: split out for Game/Gimmick/GimmickGuillotine
    CAttackMotion* m_pAttack; /* 0x0a8 */
    UBYTE _m_0xac[0x10]; /* 0x0ac m_pEffect .. 0x0b8 m_pSEEx */
    INT m_iID;           /* 0x0bc */
    UBYTE _m_0xc0[0x20]; /* 0x0c0 m_vTargetPos .. 0x0d0 m_vRootMoveScale */
private:
    eObjType m_EntityType; /* 0x0e0 */
    UINT m_uiEntityFlag;   /* 0x0e4 */
    // PARTIAL: m_tqTimer split out for Game/Player/FriendJack
    // PARTIAL: m_uiDispOffFlag/m_uiCheckEyeOffFlag split out for Game/Gimmick/GimmickStatue
    UINT m_uiDispOffFlag;     /* 0x0e8 */
    UINT m_uiCheckEyeOffFlag; /* 0x0ec */
    TIMEQ m_tqTimer;       /* 0x0f0 */
    // PARTIAL: m_iFlashingCounter/m_fNearTranslucence split out for Game/Gimmick/GimmickDominoPillar
    INT m_iFlashingCounter;    /* 0x0f4 */
    FLOAT m_fNearTranslucence; /* 0x0f8 */
    UBYTE _m_0xfc[0xd4];   /* 0x0fc m_fSpeed .. 0x1c4 m_pDamageString */

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
    void bodyCollOn(INT inKind); // PARTIAL: added for Game/Gimmick/GimmickGuillotine
    void bodyCollOff(INT inKind); // PARTIAL: added for Game/Gimmick/GimmickStatue
    void rideModelEnableAllCollision(BOOL inEnable);
    void rideModelChangeListR2();
    void rideModelChangeListRC(); // PARTIAL: added for Game/Gimmick/GimmickRideDest

    void setEntityType(eObjType inType) { m_EntityType = inType; }
    eObjType getEntityType() { return m_EntityType; }
    UINT getFlag() { return m_uiEntityFlag; }
    UINT isHeartless() const { return m_uiEntityFlag & 0x20000; } // PARTIAL: added for Game/Bullet/CreateBullet
    // PARTIAL: added for Game/Gimmick/GimmickPlantChild (getModel needs CModel's definition: System/Graphic/Model.h)
    INT getID() { return m_iID; } // PARTIAL: added for Game/Gimmick/GimmickToldPipe
    INT getModelCount() { return m_iModelCount; }
    inline CModel* getModel();
    inline CModel* getModel(INT inIndex);
    // Entity.cpp also emits this out of line (setEntityFlag__7CEntityUiUi): or inSet, then clear inMask.
    // PARTIAL: changed for Game/Gimmick/GimmickSavePoint - the store goes through a reference (not a struct
    // member access), so the scheduler does not move a following global load above it.
    void setEntityFlag(UINT inSet, UINT inMask) { UINT& f = m_uiEntityFlag; f = (f | inSet) & ~inMask; }
    // PARTIAL: added for Game/Gimmick/GimmickStatue (also out of line: setCheckEyeOffFlag__7CEntityUiUi)
    void setCheckEyeOffFlag(UINT inSet, UINT inMask) { UINT& f = m_uiCheckEyeOffFlag; f = (f | inSet) & ~inMask; }
    // PARTIAL: added for Game/Gimmick/GimmickSavePoint
    const FVECTOR& getPos() const;
    void setPos(const FVECTOR& inPos); // PARTIAL: added for Game/Gimmick/GimmickHitFlying
    FLOAT calcDistanceXZ(const CEntity* inEntity) const;
    FLOAT calcDistance(const CEntity* inEntity) const; // PARTIAL: added for Game/Gimmick/GimmickDeceive
    FLOAT calcDirection(const CEntity* inEntity) const; // PARTIAL: added for Game/Gimmick/GimmickFallTomb
    HANDLE setEffect(const CHAR* inName, UINT inFlag, const FVECTOR* inPos, const FVECTOR* inRot, FLOAT inScale);
    BOOL isRideOnPlayer();
    // PARTIAL: added for Game/Gimmick/GimmickWindowLid
    INT getBoneIndex(const CHAR* inBoneName, INT inModelIndex) const;
    BOOL isMotionEnd() const;
    BOOL isHangOnPlayer();
    void rideModelEnableChipCollision(INT inIdx, BOOL inSw);
    // PARTIAL: added for Game/Player/FriendJack
    void setTimer(TIMEQ inTq) { m_tqTimer = inTq; }
    TIMEQ getTimer() { return m_tqTimer; }
    void rideModelSetChipLocalMatrix(INT inIndex, const FMATRIX* in_pMatrix);
    void rideModelSetAttachMatrix(const FMATRIX* inpMat); // PARTIAL: added for Game/Gimmick/GimmickRideOn
    // PARTIAL: added for Game/Enemy/MapEnemyShadow
    FLOAT calcDistanceSq(const CEntity* inEntity) const;
    TIMEQ getMotionTotalTime() const;
    BOOL arriveTarget(const FVECTOR& inTarget, FLOAT inRange);
    void clearSpeed();
    void warp(const FVECTOR& inPos);
    // PARTIAL: added for Game/Player/SummonDumbo
    void setRot(const FVECTOR& inRot);
    void setScale(const FVECTOR& inScale);
    const FVECTOR& getRot();
    const FVECTOR& getRot() const; // PARTIAL: added for Game/Gimmick/GimmickPlantParent
    const FVECTOR& getScale() const;
    void updateCoord();
    void coordFix();
    // PARTIAL: added for Game/Player/FriendKing (also out of line: setDispOffFlag__7CEntityUiUi)
    void setDispOffFlag(UINT inSet, UINT inMask) { UINT& f = m_uiDispOffFlag; f = (f | inSet) & ~inMask; }
    TIMEQ getMotionCurrentTime() const;
    void userBodyCollOn(INT inIndex);
    void userBodyCollOnBody(INT inIndex);
    void userBodyCollOff(INT inIndex);
    void userBodyCollOffBody(INT inIndex);
    HANDLE setAppearEffect();
    // PARTIAL: added for Game/Gimmick/GimmickDominoPillar
    void setNearTranslucence(FLOAT inTrans) { m_fNearTranslucence = inTrans; }
    FLOAT getNearTranslucence() const { return m_fNearTranslucence; }
};

#endif /* GAME_ENTITY_H */
