// PARTIAL: added for Game/Bullet/CreateBullet
#ifndef GAME_ENTITY_H
#define GAME_ENTITY_H

#include "khtypes.h"
#include "System/Task.h"
#include "System/Vector.h" // PARTIAL: added for Game/Bullet/Bullet (setStartPos)
#include "System/Graphic/Effect/EffectMan.h" // PARTIAL: added for Game/Gimmick/BaseGimmick (ObjType::eObjType)
// PARTIAL: Gprim.h include moved to Game/Bullet/Bullet.h for Game/Gimmick/GimmickHitMore (its strings are not in every includer)

union FVECTOR;
class CState;
class CMovement; // PARTIAL: added for Game/Gimmick/GimmickBreakable
class CAttackMotion; // PARTIAL: added for Game/Gimmick/GimmickToldPipe
class CModel;    // PARTIAL: added for Game/Gimmick/GimmickPlantChild
class CLightUnit;        // PARTIAL: added for Game/AttachCard
union FMATRIX;   // PARTIAL: added for Game/Gimmick/GimmickWindowLid
class CAnim;     // PARTIAL: added for Game/Gimmick/GimmickLotusLeaf
class CBodyCollision; // PARTIAL: added for Game/Gimmick/GimmickGuillotine
class CEffectMotion;  // PARTIAL: added for Game/Player/PlayerWeapon
class CAttachMotion; // PARTIAL: added for Game/Player/Summon
class CTexAnm; // PARTIAL: added for Game/Entity

enum eObjType {
    eObjType_dummy = 0 /* values not needed yet */
};

// PARTIAL: skipped for Game/Player/PlayerCamera when System/Fsm.h (the full CFSM) came first
#ifndef SYSTEM_FSM_H
class CFSM {
protected:
    CState* m_pCurrentState;
    CState* m_pNewState;
    UBYTE m_StateInitial[0x20]; /* CStateTemplate<CFSM> */
public:
    // PARTIAL: added for Game/Enemy/MapEnemy (as in System/Fsm.h)
    BOOL isState(CState& inState) { return m_pCurrentState == &inState; }
    // PARTIAL: added for Game/Prize (as in System/Fsm.h)
    BOOL gotoState(CState& inState) {
        m_pNewState = &inState;
        return TRUE;
    }
    virtual ~CFSM();
    virtual void update();
    virtual void beginStateInitial();
    virtual void stateInitial();
    virtual void endStateInitial();
    virtual void endStateProc();
};
#endif

struct SE_MANAGED_DATA {
    FVECTOR vOffset;
    INT iBoneIndex;
    INT iFileID;
    INT iSeID;
    HANDLE hHandle;
};

class CEntity : public CTask, public CFSM {
    friend class CSEMotionCtrl; // PARTIAL: added for Game/SEMotion
    friend class CSEMotion;
    friend class CMovement; // PARTIAL: added for Game/Movement
    friend class CMapEnemy; // PARTIAL: added for Game/Enemy/MapEnemy (sets m_EntityType)
protected:
    // PARTIAL: m_pMove split out for Game/Gimmick/GimmickBreakable
    // PARTIAL: m_pModels, m_iModelCount split out for Game/Gimmick/GimmickPlantChild
    UBYTE _m_0x70[0x10] __attribute__((aligned(16))); /* 0x070 m_vStartPos */
    CModel* m_pModels;            /* 0x080 */
    FVECTOR* m_pvModelColor;      /* 0x084 */
    INT m_iModelCount;            /* 0x088 */
    // PARTIAL: m_pAnim split out for Game/Gimmick/GimmickLotusLeaf
    // PARTIAL: m_pNLight, m_pGLight split out for Game/AttachCard
    CLightUnit* m_pNLight;        /* 0x08c */
    CLightUnit* m_pGLight;        /* 0x090 */
    CAnim* m_pAnim;               /* 0x094 */
    CTexAnm* m_pTexAnm;           /* 0x098 */ // PARTIAL: split out for Game/Entity
    INT m_nTexAnmNum;             /* 0x09c */
    CMovement* m_pMove;  /* 0x0a0 */
    // PARTIAL: m_pAttack, m_iID split out for Game/Gimmick/GimmickToldPipe
    CBodyCollision* m_pBodyColl; /* 0x0a4 */ // PARTIAL: split out for Game/Gimmick/GimmickGuillotine
    CAttackMotion* m_pAttack; /* 0x0a8 */
    // PARTIAL: m_pEffect split out for Game/Player/PlayerWeapon
    CEffectMotion* m_pEffect; /* 0x0ac */
    // PARTIAL: m_pSE split out for Game/Player/Summon
    CEffectMotion* m_pEffectEx; /* 0x0b0 */ // PARTIAL: split out for Game/Entity
    CAttachMotion* m_pSE; /* 0x0b4 CSEMotion* */
    CAttachMotion* m_pSEEx; /* 0x0b8 CSEMotion* */ // PARTIAL: split out for Game/Entity
    INT m_iID;           /* 0x0bc */
    FVECTOR m_vTargetPos;     /* 0x0c0 */ // PARTIAL: split out for Game/Entity
    FVECTOR m_vRootMoveScale; /* 0x0d0 */
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
    // PARTIAL: fade members split out for Game/Player/PlayerWeapon
    // PARTIAL: split out for Game/Bullet/Bullet
    FLOAT m_fSpeed;        /* 0x0fc */
    FLOAT m_fRate;         /* 0x100 */
    HANDLE m_hTarget;      /* 0x104 */
    FLOAT m_fArriveRadius; /* 0x108 */
    FLOAT m_fStartAlpha;   /* 0x10c */
    FLOAT m_fEndAlpha;     /* 0x110 */
    FLOAT m_fFadeAlpha;    /* 0x114 */
    TIMEQ m_tqFadeTimer;   /* 0x118 */
    TIMEQ m_tqFadeTime;    /* 0x11c */
    INT m_iFadeModel;      /* 0x120 */
    struct COLL_PARAM_HEADER* m_pCollParam; /* 0x124 */ // PARTIAL: split out for Game/Entity
    class CRideModel* m_pRideModel;         /* 0x128 */
    SE_MANAGED_DATA m_SeManagedData[4]; /* 0x130 */ // PARTIAL: split out for Game/Entity
    TIMEQ m_tqDamageDispTimer;          /* 0x1b0 */
    char m_szNumberString[16];          /* 0x1b4 */
    class CFntM2* m_pDamageString;      /* 0x1c4 (the ref types it CFnt*; the code calls CFntM2::setText) */

public:
    CEntity(CTask* inParent);
    CEntity(CTask* inParent, ObjType::eObjType inType); // PARTIAL: added for Game/Gimmick/BaseGimmick
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
    INT getID() const { return m_iID; } // PARTIAL: added for Game/Gimmick/GimmickToldPipe
    INT isFirstStandsGroundIK(); // PARTIAL: added for Game/Movement
    void updateBadStatusEffect(HANDLE inHandle); // PARTIAL: added for Game/Enemy/MapEnemy
    BOOL testFront(const FVECTOR& inPos) const; // PARTIAL: added for Game/Enemy/MapEnemy
    BOOL entityVisibleAxisY(const CEntity* inEntity, FLOAT inAngle); // PARTIAL: added for Game/Enemy/MapEnemy
    HANDLE setBadStatusEffect(const CHAR* inName, INT inKind, FLOAT inScale); // PARTIAL: added for Game/Enemy/MapEnemy (parameter names guessed)
    INT getModelCount() { return m_iModelCount; }
    inline CModel* getModel();
    inline CModel* getModel(INT inIndex);
    // Entity.cpp also emits this out of line (setEntityFlag__7CEntityUiUi): or inSet, then clear inMask.
    // PARTIAL: changed for Game/Gimmick/GimmickSavePoint - the store goes through a reference (not a struct
    // member access), so the scheduler does not move a following global load above it.
#ifdef ENTITY_H_SETFLAG_DIRECT // PARTIAL: the plain form, for Game/Atel/Avatar
    void setEntityFlag(UINT inSet, UINT inMask) { m_uiEntityFlag = (m_uiEntityFlag | inSet) & ~inMask; }
#else
    void setEntityFlag(UINT inSet, UINT inMask) { UINT& f = m_uiEntityFlag; f = (f | inSet) & ~inMask; }
#endif
    void clearEntityFlag(INT inFlag) { m_uiEntityFlag &= ~inFlag; } // PARTIAL: added for Game/Atel/Atel (name unknown)
    // PARTIAL: added for Game/Gimmick/GimmickStatue (also out of line: setCheckEyeOffFlag__7CEntityUiUi)
    void setCheckEyeOffFlag(UINT inSet, UINT inMask) { UINT& f = m_uiCheckEyeOffFlag; f = (f | inSet) & ~inMask; }
    // PARTIAL: added for Game/Gimmick/GimmickSavePoint
    const FVECTOR& getPos() const;
    void setPos(const FVECTOR& inPos); // PARTIAL: added for Game/Gimmick/GimmickHitFlying
    INT getMotionIndex(const CHAR* inName) const; // PARTIAL: added for Game/Enemy/EnemyWyvern
    void setOldPos(const FVECTOR& inPos); // PARTIAL: added for Game/Gimmick/BaseGimmick
    FLOAT calcDistanceXZ(const CEntity* inEntity) const;
    FLOAT calcDistanceXZ(const CEntity& inEntity) const; // PARTIAL: added for Game/Gimmick/GimmickDoor
    FLOAT calcDistance(const CEntity* inEntity) const; // PARTIAL: added for Game/Gimmick/GimmickDeceive
    FLOAT calcDirection(const CEntity* inEntity) const; // PARTIAL: added for Game/Gimmick/GimmickFallTomb
    void calcDirection(FVECTOR& outRot, const FVECTOR& inPos) const; // PARTIAL: added for Game/Player/SummonMushu
    HANDLE setEffect(const CHAR* inName, UINT inFlag, const FVECTOR* inPos, const FVECTOR* inRot, FLOAT inScale); void setDamageNumber(const FVECTOR& inPos, INT inDamage); // PARTIAL: setDamageNumber added for Game/Player/Player
    BOOL isRideOnPlayer();
    // PARTIAL: added for Game/Gimmick/GimmickWindowLid
    INT getBoneIndex(const CHAR* inBoneName, INT inModelIndex) const;
    BOOL isMotionEnd() const;
    BOOL isHangOnPlayer();
    void rideModelEnableChipCollision(INT inIdx, BOOL inSw);
    void rideModelSetChipLocalMatrix(INT inIndex, const FMATRIX* in_pMatrix);
    // PARTIAL: added for Game/Player/FriendJack
    void setTimer(TIMEQ inTq) { m_tqTimer = inTq; }
    TIMEQ getTimer() { return m_tqTimer; }
    HANDLE setDeathEffect(); void setDeathEffect(const FVECTOR* inPos); // PARTIAL: added for Game/Enemy/BattleEnemy (void: the caller keeps v0 free)
    void rideModelSetAttachMatrix(const FMATRIX* inpMat); // PARTIAL: added for Game/Gimmick/GimmickRideOn
    // PARTIAL: added for Game/Enemy/MapEnemyShadow
    FLOAT calcDistanceSq(const CEntity* inEntity) const;
    FLOAT calcDistanceSq(const CEntity& inEntity) const; // PARTIAL: added for Game/Gimmick/GimmickManager
    TIMEQ getMotionTotalTime() const;
    BOOL arriveTarget(const FVECTOR& inTarget, FLOAT inRange);
    BOOL arriveTargetXZ(const FVECTOR& inTarget, FLOAT inRange); // PARTIAL: added for Game/Enemy/MapEnemyMushroom
    void clearSpeed();
    // PARTIAL: added for Game/Enemy/EnemyDarkball
    void changeMotionFileName(const CHAR* inName, TIMEQ inTq, BOOL inRootMove, TIMEQ inGr, INT inEffectNo, INT inSENo);
    FMATRIX* getBoneMatrix(const CHAR* inBoneName) const; // PARTIAL: added for Game/Enemy/EnemyFatbandit
    // PARTIAL: added for Game/Enemy/MapEnemyGround
    INT getMotionCurrentID(); /* inline in the original; defined where CAnim is complete (Game/Enemy/MapEnemyGround.cpp) */
    void endTargetChase();
    INT setTexAnmSeq(INT nAnmNo, INT nSeqNo);
    INT setTexAnmSeq(INT nAnmNo, const CHAR* pszSeqName); // PARTIAL: added for Game/Enemy/BossDeadDemo
    void enableNotSemiModel(INT inModel, UINT inFlag); // PARTIAL: added for Game/Atel/Avatar (parameter names guessed)
    void playVoice(INT inFileID, INT inID, INT inPriority); // PARTIAL: added for Game/Enemy/HumanBoss
    void loadModelColor_(); // PARTIAL: added for Game/Enemy/EnemyHeartsoldier
    FLOAT getHeight() const; // PARTIAL: added for Game/Enemy/EnemyTornadostep
    void warp(const FVECTOR& inPos);
    // PARTIAL: added for Game/Player/SummonDumbo
    void setRot(const FVECTOR& inRot);
    void setScale(const FVECTOR& inScale);
    FVECTOR& getRot();
    const FVECTOR& getRot() const; // PARTIAL: added for Game/Gimmick/GimmickPlantParent
    const FVECTOR& getScale() const;
    void updateCoord();
    void updateRootMove(); // PARTIAL: added for Game/Entity
    void updateAnimation();
    BOOL initRideModel_(INT inGroup);
    BOOL initCollision_(INT inGroupID, INT inRsrcID);
    void updateSE_();
    void updateFade_();
    // PARTIAL: added for Game/EntityFactory
    void positionFix(const FVECTOR& inPos, FLOAT inDir);
    void positionFix(const FVECTOR& inPos, const FVECTOR& inRot);
    void land(FLOAT inDownLength, FLOAT inUpLength, BOOL inFirstHit);
    void coordFix();
    // PARTIAL: added for Game/Player/PlayerDRiku
    BOOL createLegIK(FLOAT inP0, FLOAT inP1);
    void setFootEffectIK(const CHAR* inName, INT inFlag);
    void setGroundIK(INT inFlag);
    // PARTIAL: added for Game/Player/FriendKing (also out of line: setDispOffFlag__7CEntityUiUi)
#ifdef ENTITY_H_SETFLAG_DIRECT // PARTIAL: the plain form, for Game/Atel/Avatar
    void setDispOffFlag(UINT inSet, UINT inMask) { m_uiDispOffFlag = (m_uiDispOffFlag | inSet) & ~inMask; }
#else
    void setDispOffFlag(UINT inSet, UINT inMask) { UINT& f = m_uiDispOffFlag; f = (f | inSet) & ~inMask; }
#endif
    TIMEQ getMotionCurrentTime() const;
    void userBodyCollOn(INT inIndex);
    void userBodyCollOnBody(INT inIndex);
    void userBodyCollOff(INT inIndex);
    void userBodyCollOffBody(INT inIndex);
    HANDLE setAppearEffect();
    HANDLE setDisappearEffect(); // PARTIAL: added for Game/Player/Friend
    // PARTIAL: added for Game/Gimmick/GimmickDominoPillar
    void setNearTranslucence(FLOAT inTrans) { m_fNearTranslucence = inTrans; }
    FLOAT getNearTranslucence() const { return m_fNearTranslucence; }
    // PARTIAL: added for Game/Player/FriendPluto
    UINT isTargetMoveXZ() const { return m_uiEntityFlag & 0x40; }
    BOOL startTargetMoveXZ(const FVECTOR& inPos, FLOAT inSpeed, FLOAT inAccel, FLOAT inRange);
    void endTargetMoveXZ();
    // PARTIAL: added for Game/Player/FriendPan
    FLOAT calcDirection(const FVECTOR& inPos) const;
    void setAnimSpeed(FLOAT inSpeed);
    // PARTIAL: added for Game/Player/FriendAriel
    const FVECTOR& getVel() const;
    void setVel(const FVECTOR& inVel);
    const FVECTOR& getOldPos() const; // PARTIAL: added for Game/Player/FriendGoofy
    // PARTIAL: added for Game/Gimmick/GimmickThunderTrap
    HANDLE attachEffect(const CHAR* inName, UINT inNo, const CCoord* inParent, const FVECTOR* inPos,
                        const FVECTOR* inRot, FLOAT inWait);
    // PARTIAL: added for Game/Player/SummonTink
    FLOAT calcDistanceXZ(const FVECTOR& inPos) const;
    static FLOAT rndArea(FLOAT inMin, FLOAT inMax);
    // PARTIAL: added for Game/Player/SummonBambi
    BOOL isGround() const;
    FLOAT calcDistance(const FVECTOR& inPos) const;
    // PARTIAL: added for Game/Player/PlayerWeapon (weak in the original)
    UINT isDispOFF() const { return m_uiEntityFlag & 1; }
    // PARTIAL: added for Game/EffectMotion
    CAnim* getAnim() { return m_pAnim; }
    HANDLE setEffect(const CHAR* inName, UINT inFlag, const FMATRIX* inMat, FLOAT inScale);
    HANDLE attachEffect3(const CHAR* inName, UINT inNo, const CCoord* inParent1, const CCoord* inParent2,
                         const CCoord* inParent3, const FVECTOR* inPos1, const FVECTOR* inPos2, const FVECTOR* inPos3,
                         FLOAT inWait);
    BOOL isMotionLoop() const;
    // PARTIAL: added for Game/Player/SummonCloud
    void setModelAlphaScale(FLOAT inAlpha, INT inModel);
    void killLoopEffect(INT inNo); // PARTIAL: added for Game/Player/Summon
    // PARTIAL: added for Game/Gimmick/GimmickCarry
    void changeMotionEx(INT inRsrcID, INT inIndex, TIMEQ inTq, BOOL inRootMove, TIMEQ inGr, INT inEffectNo, INT inSENo);
    void changeMotionEx(const CHAR* inRsrcName, INT inIndex, TIMEQ inTq, BOOL inRootMove, TIMEQ inGr, INT inEffectNo, INT inSENo); // PARTIAL: added for Game/Entity
    void enableFade2Path(BOOL inSw, INT inIndex);
    UINT isPermissionLockOn() const { return m_uiEntityFlag & 0x10000; }
    FLOAT calcDistanceXZSq(const FVECTOR& inPos) const;
    FLOAT calcDistanceXZSq(const CEntity* inEntity) const;
    // PARTIAL: added for Game/Bullet/Bullet
    const FVECTOR& getStartPos() const { return *(const FVECTOR*)_m_0x70; }
    void setStartPos(const FVECTOR& inPos) { *(FVECTOR*)_m_0x70 = inPos; }
    void setRate(FLOAT inRate) { m_fRate = inRate; }
    FLOAT getRate() const { return m_fRate; }
    FLOAT getFadeAlpha() const { return m_fFadeAlpha; }
    FLOAT getEndAlpha() const { return m_fEndAlpha; } // PARTIAL: added for Game/Player/PlayerDRiku (original name unknown)
    UINT isFade() const { return m_uiEntityFlag & 0x40000000; }
    UINT isRootMoveOff() const { return m_uiEntityFlag & 0x8000; } // PARTIAL: added for Game/Entity
    INT getFlashingCounter() const { return m_iFlashingCounter; } // PARTIAL: added for Game/Prize (name unknown)
    FLOAT calcDistanceSq(const FVECTOR& inPos) const;
    HANDLE playSEPanAndVol(INT inFile, INT inID, INT inPan, const FVECTOR* inPos, INT inFlag);
    HANDLE playSEPan(INT inFile, INT inID, INT inVol, INT inBoneIndex, const FVECTOR* inOffset, INT inType); // PARTIAL: added for Game/Entity
    void playSEManaged(INT inFileID, INT inID, INT inBoneIndex, const FVECTOR* inOffset); // PARTIAL: added for Game/Entity
    void setSEPanAndVol(HANDLE inHandle, INT inFile, INT inID, INT inPan, const FVECTOR* inPos, INT inFlag);
    void fadeoutSE(HANDLE inHandle, INT inFile, INT inID, INT inTime, INT inFlag);
    // PARTIAL: added for Game/Entity
    INT getMotionCurrentID() const;
    INT getMotionCount() const;
    INT getRideCollisionNum() const;
    BOOL isMotionLooped() const;
    BOOL isMotionLoopData(INT inIndex) const;
    const CHAR* getMotionName(INT inIndex) const;
    FLOAT calcDistance(const CEntity& inEntity) const;
    FLOAT calcDistanceXZSq(const CEntity& inEntity) const;
    FLOAT calcDirection(const CEntity& inEntity) const;
    void calcDirection(FVECTOR& outRot, const CEntity& inEntity) const;
    void calcDirection(FVECTOR& outRot, const CEntity* inEntity) const;
    BOOL testFront(const CEntity& inEntity) const;
    BOOL testFront(const CEntity* inEntity) const;
    BOOL entityVisible(const CEntity& inEntity, FLOAT inAngle, FLOAT inDist);
    BOOL entityVisible(const CEntity* inEntity, FLOAT inAngle, FLOAT inDist);
    BOOL entityVisibleAxisX(const CEntity& inEntity, FLOAT inAngle);
    BOOL entityVisibleAxisX(const CEntity* inEntity, FLOAT inAngle);
    BOOL entityVisibleAxisY(const CEntity& inEntity, FLOAT inAngle);
    void setMotion(TIMEQ inTq);
    BOOL createArmIK();
    void endTargetEvade();
    void endTargetIntercept();
    void endTargetMove();
    BOOL startTargetMove(const FVECTOR& inPos, FLOAT inSpeed, FLOAT inAccel, FLOAT inRange);
    void rideModelChangeListR();
    void updateTimer_();
    void saveRideModelMatrix_();
    const FVECTOR& getQuat() const;
    FVECTOR& getQuat();
    void setQuat(const FVECTOR& inQuat);
    void setHitBackVel(const FVECTOR& inVel);
    BOOL isRideOnEnemy();
    BOOL isHangOnEnemy();
    void updateIK_();
    void chase_();
    void evade_();
    void intercept_();
    void updateEffect_();
    HANDLE attachEffect2(const CHAR* inName, UINT inNo, const CCoord* inParent1, const CCoord* inParent2, const FMATRIX* inMat1, const FMATRIX* inMat2, FLOAT inWait);
    HANDLE attachEffect2(const CHAR* inName, UINT inNo, const CCoord* inParent1, const CCoord* inParent2, const FVECTOR* inPos1, const FVECTOR* inPos2, FLOAT inWait);
    HANDLE attachEffect(const CHAR* inName, UINT inNo, const CCoord* inParent, const FMATRIX* inMat, FLOAT inWait);
    HANDLE attachEffect2(const struct EFFECT_DATA& inData, UINT inNo, const CCoord* inParent1, const CCoord* inParent2, const FMATRIX* inMat1, const FMATRIX* inMat2, FLOAT inWait);
    HANDLE attachEffect2(const struct EFFECT_DATA& inData, UINT inNo, const CCoord* inParent1, const CCoord* inParent2, const FVECTOR* inPos1, const FVECTOR* inPos2, FLOAT inWait);
    HANDLE attachEffect3(const CHAR* inName, UINT inNo, const CCoord* inParent1, const CCoord* inParent2, const CCoord* inParent3, const FMATRIX* inMat1, const FMATRIX* inMat2, const FMATRIX* inMat3, FLOAT inWait);
    HANDLE attachEffect(const struct EFFECT_DATA& inData, UINT inNo, const CCoord* inParent, const FMATRIX* inMat, FLOAT inWait);
    HANDLE attachEffect(const struct EFFECT_DATA& inData, UINT inNo, const CCoord* inParent, const FVECTOR* inPos, const FVECTOR* inRot, FLOAT inWait);
    HANDLE setEffect(const struct EFFECT_DATA& inData, UINT inNo, const FMATRIX* inMat, FLOAT inWait);
    HANDLE setEffect(const struct EFFECT_DATA& inData, UINT inNo, const FVECTOR* inPos, const FVECTOR* inRot, FLOAT inWait);
    HANDLE attachEffect3(const struct EFFECT_DATA& inData, UINT inNo, const CCoord* inParent1, const CCoord* inParent2, const CCoord* inParent3, const FMATRIX* inMat1, const FMATRIX* inMat2, const FMATRIX* inMat3, FLOAT inWait);
    HANDLE attachEffect3(const struct EFFECT_DATA& inData, UINT inNo, const CCoord* inParent1, const CCoord* inParent2, const CCoord* inParent3, const FVECTOR* inPos1, const FVECTOR* inPos2, const FVECTOR* inPos3, FLOAT inWait);
    const CHAR* getBoneName(INT inBone, INT inModel) const;
    void stopSE(HANDLE inHandle, INT inFile, INT inID, INT inFlag);
    BOOL startTargetChase(CEntity* inTarget, FLOAT inSpeed, FLOAT inRate);
    BOOL startTargetEvade(CEntity* inTarget, FLOAT inSpeed, FLOAT inRate);
    BOOL startTargetIntercept(CEntity* inTarget, FLOAT inSpeed, FLOAT inRate);
    INT isStandsGroundIK();
    void storeModelColor_();
    void targetMoveXZ_();
    void targetMove_();
    BOOL initEffectEx_(const CHAR* inRsrcName);
    BOOL initEffectEx_(INT inID);
    BOOL initEffect_(INT inID);
    BOOL initSEEx_(INT inID);
    BOOL initSE_(INT inID);
    void rideModelAdjustRiders_();
    void setMoveFlag(UINT inSet, UINT inMask);
    void setModelColorScale(const FVECTOR& inColor, INT inIndex);
    void changeMotionFileNameWithSameCheck(const CHAR* inName, TIMEQ inTq, BOOL inRootMove, TIMEQ inGr, INT inEffectNo, INT inSENo);
    void changeMotionWithSameCheck(INT inIndex, TIMEQ inTq, BOOL inRootMove, TIMEQ inGr, INT inEffectNo, INT inSENo);
    HANDLE setRecoverEffect(const CHAR* inName, INT inNo);
    void setupFade(FLOAT inStart, FLOAT inEnd, TIMEQ inTime, INT inModel) {
        setEntityFlag(0x40000000, 0);
        m_fStartAlpha = inStart;
        m_fEndAlpha = inEnd;
        m_tqFadeTimer = inTime;
        m_tqFadeTime = inTime;
        m_iFadeModel = inModel;
    }
};

/* PARTIAL: added for Game/Gimmick/GimmickManager: the original header's random helpers (names unknown; lines
 * from the debug info, their line notes decide the assembler's padding before a following division). */
#include "System/Rnd.h"
#line 574
inline INT getRand() {
    return RND::genrand_int31();
}
#line 581
inline FLOAT getRandF() { return RND::genrand_real1(); }
/* PARTIAL: added for Game/Enemy/BattleEnemy (setupDuelCard): shuffles an array (name unknown, lines from the debug info) */
#line 588
inline void shuffleArray(INT* inArray, INT inNum) {
    INT i;
    INT* array = inArray;
    for (i = inNum - 1; i > 0; i--) {
        INT j;
        j = (INT)((i + 1) * getRandF());
        INT tmp = array[i];
        array[i] = array[j];
        array[j] = tmp;
    }
}

#endif /* GAME_ENTITY_H */
