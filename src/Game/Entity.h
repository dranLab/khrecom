
#ifndef GAME_ENTITY_H
#define GAME_ENTITY_H

#include "khtypes.h"
#include "System/Task.h"
#include "System/Vector.h"
#include "System/Graphic/Effect/EffectMan.h"
// MATCH: Gprim.h is included by Game/Bullet/Bullet.h for Game/Gimmick/GimmickHitMore (its strings are not in every includer)

union FVECTOR;
class CState;
class CMovement;
class CAttackMotion;
class CModel;
class CLightUnit;
union FMATRIX;
class CAnim;
class CBodyCollision;
class CEffectMotion;
class CAttachMotion;
class CTexAnm;





#include "System/Fsm.h"























struct SE_MANAGED_DATA {
    FVECTOR vOffset;
    INT iBoneIndex;
    INT iFileID;
    INT iSeID;
    HANDLE hHandle;
};

class CEntity : public CTask, public CFSM {
    friend class CSEMotionCtrl;
    friend class CSEMotion;
    friend class CMovement;
    friend class CMapEnemy;
protected:
    FVECTOR m_vStartPos;          /* 0x070 */
    CModel* m_pModels;            /* 0x080 */
    FVECTOR* m_pvModelColor;      /* 0x084 */
    INT m_iModelCount;            /* 0x088 */
    CLightUnit* m_pNLight;        /* 0x08c */
    CLightUnit* m_pGLight;        /* 0x090 */
    CAnim* m_pAnim;               /* 0x094 */
    CTexAnm* m_pTexAnm;           /* 0x098 */
    INT m_nTexAnmNum;             /* 0x09c */
    CMovement* m_pMove;  /* 0x0a0 */
    CBodyCollision* m_pBodyColl; /* 0x0a4 */
    CAttackMotion* m_pAttack; /* 0x0a8 */
    CEffectMotion* m_pEffect; /* 0x0ac */
    CEffectMotion* m_pEffectEx; /* 0x0b0 */
    CAttachMotion* m_pSE; /* 0x0b4 CSEMotion* */
    CAttachMotion* m_pSEEx; /* 0x0b8 CSEMotion* */
    INT m_iID;           /* 0x0bc */
    FVECTOR m_vTargetPos;     /* 0x0c0 */
    FVECTOR m_vRootMoveScale; /* 0x0d0 */
private:
    ObjType::eObjType m_EntityType; /* 0x0e0 */
    UINT m_uiEntityFlag;   /* 0x0e4 */
    UINT m_uiDispOffFlag;     /* 0x0e8 */
    UINT m_uiCheckEyeOffFlag; /* 0x0ec */
    TIMEQ m_tqTimer;       /* 0x0f0 */
    INT m_iFlashingCounter;    /* 0x0f4 */
    FLOAT m_fNearTranslucence; /* 0x0f8 */
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
    struct COLL_PARAM_HEADER* m_pCollParam; /* 0x124 */
    class CRideModel* m_pRideModel;         /* 0x128 */
    SE_MANAGED_DATA m_SeManagedData[4]; /* 0x130 */
    KH_DEBUG(TIMEQ m_tqDamageDispTimer;)          /* 0x1b0 (retail: no damage display) */
    KH_DEBUG(char m_szNumberString[16];)          /* 0x1b4 */
    KH_DEBUG(class CFnt* m_pDamageString;)        /* 0x1c4 */

public:
    CEntity(CTask* inParent);
    CEntity(CTask* inParent, ObjType::eObjType inType);
    virtual ~CEntity();
    virtual INT getTypeID() const;
    virtual BOOL initalize(INT inGroupID, INT inRsrcID);
    virtual BOOL initalize(INT inGroupID, INT inRsrcID, UINT iFlag);
    virtual void AfterInit();
    virtual void main();
    virtual void disp();
    virtual void onMsg(INT inMsg, void* inExtraInfo);
    virtual void getLockOnPos(FVECTOR& outPos);
    virtual void getHeadPos(FVECTOR& outPos) const;
    virtual void getUseCardPos(FVECTOR& outPos);
    virtual void getCameraTargetPos(FVECTOR& outPos);
    virtual void endStateProc();
    virtual void changeMotion(INT inIndex, TIMEQ inTq, BOOL inRootMove, TIMEQ inGr, INT inEffectNo, INT inSENo);
    virtual void updateBefore();
    virtual void updateAfter();
    virtual void updatePositionAfter();
    virtual void lastUpdate();

    void bodyCollOnBody(INT inKind);
    void bodyCollOffBody(INT inKind);
    void bodyCollOn(INT inKind);
    void bodyCollOff(INT inKind);
    void rideModelEnableAllCollision(BOOL inEnable);
    void rideModelChangeListR2();
    void rideModelChangeListRC();

    void setEntityType(ObjType::eObjType inType);
    ObjType::eObjType getEntityType() const;
    UINT getFlag() const;
    UINT isHeartless() const;
    INT getID() const;
    INT isFirstStandsGroundIK();
    void updateBadStatusEffect(HANDLE inHandle);
    BOOL testFront(const FVECTOR& inPos) const;
    BOOL entityVisibleAxisY(const CEntity* inEntity, FLOAT inAngle);
    HANDLE setBadStatusEffect(const CHAR* inName, INT inKind, FLOAT inScale);
    INT getModelCount() const;
    CModel* getModel();
    CModel* getModel() const;
    inline CModel* getModel(INT inIndex);
    // Entity.cpp also emits this out of line (setEntityFlag__7CEntityUiUi): or inSet, then clear inMask.
    // MATCH: changed for Game/Gimmick/GimmickSavePoint - the store goes through a reference (not a struct
    // member access), so the scheduler does not move a following global load above it.
    void setEntityFlag(UINT inSet, UINT inMask);
    void setCheckEyeOffFlag(UINT inSet, UINT inMask);
    const FVECTOR& getPos() const;
    void setPos(const FVECTOR& inPos);
    INT getMotionIndex(const CHAR* inName) const;
    void setOldPos(const FVECTOR& inPos);
    FLOAT calcDistanceXZ(const CEntity* inEntity) const;
    FLOAT calcDistanceXZ(const CEntity& inEntity) const;
    FLOAT calcDistance(const CEntity* inEntity) const;
    FLOAT calcDirection(const CEntity* inEntity) const;
    void calcDirection(FVECTOR& outRot, const FVECTOR& inPos) const;
    HANDLE setEffect(const CHAR* inName, UINT inFlag, const FVECTOR* inPos, const FVECTOR* inRot, FLOAT inScale); void setDamageNumber(const FVECTOR& inPos, INT inDamage);
    BOOL isRideOnPlayer();
    INT getBoneIndex(const CHAR* inBoneName, INT inModelIndex) const;
    BOOL isMotionEnd() const;
    BOOL isHangOnPlayer();
    void rideModelEnableChipCollision(INT inIdx, BOOL inSw);
    void rideModelSetChipLocalMatrix(INT inIndex, const FMATRIX* in_pMatrix);
    void setTimer(TIMEQ inTq);
    TIMEQ getTimer() const;
    HANDLE setDeathEffect(); void setDeathEffect(const FVECTOR* inPos);
    void rideModelSetAttachMatrix(const FMATRIX* inpMat);
    FLOAT calcDistanceSq(const CEntity* inEntity) const;
    FLOAT calcDistanceSq(const CEntity& inEntity) const;
    TIMEQ getMotionTotalTime() const;
    BOOL arriveTarget(const FVECTOR& inTarget, FLOAT inRange);
    BOOL arriveTargetXZ(const FVECTOR& inTarget, FLOAT inRange);
    void clearSpeed();
    void changeMotionFileName(const CHAR* inName, TIMEQ inTq, BOOL inRootMove, TIMEQ inGr, INT inEffectNo, INT inSENo);
    FMATRIX* getBoneMatrix(const CHAR* inBoneName) const;
    INT getMotionCurrentID(); /* inline in the original; defined where CAnim is complete (Game/Enemy/MapEnemyGround.cpp) */
    void endTargetChase();
    INT setTexAnmSeq(INT nAnmNo, INT nSeqNo);
    INT setTexAnmSeq(INT nAnmNo, const CHAR* pszSeqName);
    void enableNotSemiModel(INT inModel, UINT inFlag);
    void playVoice(INT inFileID, INT inID, INT inPriority);
    void loadModelColor_();
    FLOAT getHeight() const;
    void warp(const FVECTOR& inPos);
    void setRot(const FVECTOR& inRot);
    void setScale(const FVECTOR& inScale);
    FVECTOR& getRot();
    const FVECTOR& getRot() const;
    const FVECTOR& getScale() const;
    void updateCoord();
    void updateRootMove();
    void updateAnimation();
    BOOL initRideModel_(INT inGroup);
    BOOL initCollision_(INT inGroupID, INT inRsrcID);
    void updateSE_();
    void updateFade_();
    void translucence_();
    void positionFix(const FVECTOR& inPos, FLOAT inDir);
    void positionFix(const FVECTOR& inPos, const FVECTOR& inRot);
    void land(FLOAT inDownLength, FLOAT inUpLength, BOOL inFirstHit);
    void coordFix();
    BOOL createLegIK(FLOAT inP0, FLOAT inP1);
    void setFootEffectIK(const CHAR* inName, INT inFlag);
    void setGroundIK(INT inFlag);
    void setDispOffFlag(UINT inSet, UINT inMask);
    TIMEQ getMotionCurrentTime() const;
    void userBodyCollOn(INT inIndex);
    void userBodyCollOnBody(INT inIndex);
    void userBodyCollOff(INT inIndex);
    void userBodyCollOffBody(INT inIndex);
    HANDLE setAppearEffect();
    HANDLE setDisappearEffect();
    void setNearTranslucence(FLOAT inTrans);
    FLOAT getNearTranslucence() const;
    UINT isTargetMoveXZ() const;
    BOOL startTargetMoveXZ(const FVECTOR& inPos, FLOAT inSpeed, FLOAT inAccel, FLOAT inRange);
    void endTargetMoveXZ();
    FLOAT calcDirection(const FVECTOR& inPos) const;
    void setAnimSpeed(FLOAT inSpeed);
    const FVECTOR& getVel() const;
    void setVel(const FVECTOR& inVel);
    const FVECTOR& getOldPos() const;
    HANDLE attachEffect(const CHAR* inName, UINT inNo, const CCoord* inParent, const FVECTOR* inPos,
                        const FVECTOR* inRot, FLOAT inWait);
    FLOAT calcDistanceXZ(const FVECTOR& inPos) const;
    static FLOAT rndArea(FLOAT inMin, FLOAT inMax);
    BOOL isGround() const;
    FLOAT calcDistance(const FVECTOR& inPos) const;
    UINT isDispOFF() const;
    CAnim* getAnim();
    CAnim* getAnim() const;
    HANDLE setEffect(const CHAR* inName, UINT inFlag, const FMATRIX* inMat, FLOAT inScale);
    HANDLE attachEffect3(const CHAR* inName, UINT inNo, const CCoord* inParent1, const CCoord* inParent2,
                         const CCoord* inParent3, const FVECTOR* inPos1, const FVECTOR* inPos2, const FVECTOR* inPos3,
                         FLOAT inWait);
    BOOL isMotionLoop() const;
    void setModelAlphaScale(FLOAT inAlpha, INT inModel);
    void killLoopEffect(INT inNo);
    void changeMotionEx(INT inRsrcID, INT inIndex, TIMEQ inTq, BOOL inRootMove, TIMEQ inGr, INT inEffectNo, INT inSENo);
    void changeMotionEx(const CHAR* inRsrcName, INT inIndex, TIMEQ inTq, BOOL inRootMove, TIMEQ inGr, INT inEffectNo, INT inSENo);
    void enableFade2Path(BOOL inSw, INT inIndex);
    UINT isPermissionLockOn() const;
    FLOAT calcDistanceXZSq(const FVECTOR& inPos) const;
    FLOAT calcDistanceXZSq(const CEntity* inEntity) const;
    const FVECTOR& getStartPos() const;
    void setStartPos(const FVECTOR& inPos);
    void setRate(FLOAT inRate);
    FLOAT getRate() const;
    FLOAT getFadeAlpha() const;
    UINT isFade() const;
    UINT isRootMoveOff() const;
    UINT isFlashing() const;
    INT getFlashingCounter() const;
    FLOAT calcDistanceSq(const FVECTOR& inPos) const;
    HANDLE playSEPanAndVol(INT inFile, INT inID, INT inPan, const FVECTOR* inPos, INT inFlag);
    HANDLE playSEPan(INT inFile, INT inID, INT inVol, INT inBoneIndex, const FVECTOR* inOffset, INT inType);
    void playSEManaged(INT inFileID, INT inID, INT inBoneIndex, const FVECTOR* inOffset);
    void setSEPanAndVol(HANDLE inHandle, INT inFile, INT inID, INT inPan, const FVECTOR* inPos, INT inFlag);
    void setSEPan(HANDLE inHandle, INT inFile, INT inID, INT inVol, INT inBoneIndex, const FVECTOR* inOffset, INT inType);
    void fadeoutSE(HANDLE inHandle, INT inFile, INT inID, INT inTime, INT inFlag);
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
    BOOL initalizeModel(INT inGroup, BOOL bLight);
    BOOL initEffect_(INT inID);
    BOOL initSEEx_(INT inID);
    BOOL initSE_(INT inID);
    void rideModelAdjustRiders_();
    void setMoveFlag(UINT inSet, UINT inMask);
    void setModelColorScale(const FVECTOR& inColor, INT inIndex);
    void changeMotionFileNameWithSameCheck(const CHAR* inName, TIMEQ inTq, BOOL inRootMove, TIMEQ inGr, INT inEffectNo, INT inSENo);
    void changeMotionWithSameCheck(INT inIndex, TIMEQ inTq, BOOL inRootMove, TIMEQ inGr, INT inEffectNo, INT inSENo);
    HANDLE setRecoverEffect(const CHAR* inName, INT inNo);
    void setupFade(FLOAT inStart, FLOAT inEnd, TIMEQ inTime, INT inModel);
    void setTimerFrame(INT inFrame);
    void setTimerSecond(FLOAT inSecond);
    void setSpeed(FLOAT inSpeed);
    void setArriveRadius(FLOAT inRadius);
    void setTargetPos(const FVECTOR& inPos);
    void setRootMoveScale(const FVECTOR& inScale);
    FLOAT getSpeed() const;
    FLOAT getArriveRadius() const;
    const FVECTOR& getTargetPos() const;
    COLL_PARAM_HEADER* getCollParam() const;
    const FVECTOR& getRootMoveScale() const;
    UINT isDisableClearSpeed() const;
    UINT isTargetChase() const;
    UINT isTargetEvade() const;
    UINT isTargetIntercept() const;
    UINT isTargetMove() const;
    UINT isRotXOff() const;
    UINT isRotYOff() const;
    UINT isRotZOff() const;
    UINT isEffectOff() const;
    UINT isTransOff() const;
    UINT isRotZYX() const;
    UINT isSeOff() const;
    UINT isSetDataCreate() const;
    UINT isStop() const;
    UINT isNoLevitate() const;
    UINT isSetAlphaAttachEffect() const;
    BOOL timeOver() const;
    void loadModelColor();
    static INT random();
    static FLOAT rnd();
    void setID(INT inID);
};

/* PARTIAL: added for Game/Gimmick/GimmickManager: the original header's random helpers (names unknown; lines
 * from the debug info, their line notes decide the assembler's padding before a following division). */
#include "System/Rnd.h"

/* The inline members of CEntity, in the original's emission order (Game/Entity emits them all: its key function
 * ~CEntity is there). Defined after the class so the order does not depend on the declaration order. */
inline void CEntity::AfterInit() {}
inline void CEntity::onMsg(INT inMsg, void* inExtraInfo) {}
inline void CEntity::setEntityType(ObjType::eObjType inType) { m_EntityType = inType; }
inline void CEntity::setStartPos(const FVECTOR& inPos) { m_vStartPos = inPos; }
#ifdef ENTITY_H_SETFLAG_DIRECT // MATCH: the plain form, for Game/Atel/Atel (createAvatar) and Game/Atel/Avatar (ctor, InitAvatar)
inline void CEntity::setEntityFlag(UINT inSet, UINT inMask) { m_uiEntityFlag = (m_uiEntityFlag | inSet) & ~inMask; }
#else
// MATCH: changed for Game/Gimmick/GimmickSavePoint - the store goes through a reference (not a struct member
// access), so the scheduler does not move a following global load above it.
inline void CEntity::setEntityFlag(UINT inSet, UINT inMask) { UINT& f = m_uiEntityFlag; f = (f | inSet) & ~inMask; }
#endif
inline void CEntity::setTimer(TIMEQ inTq) { m_tqTimer = inTq; }
inline void CEntity::setTimerFrame(INT inFrame) { m_tqTimer = inFrame * 50; }
inline void CEntity::setTimerSecond(FLOAT inSecond) { m_tqTimer = (TIMEQ)(inSecond * 60.0f * 50.0f); }
inline void CEntity::setRate(FLOAT inRate) { m_fRate = inRate; }
inline void CEntity::setSpeed(FLOAT inSpeed) { m_fSpeed = inSpeed; }
inline void CEntity::setArriveRadius(FLOAT inRadius) { m_fArriveRadius = inRadius; }
inline void CEntity::setTargetPos(const FVECTOR& inPos) { m_vTargetPos = inPos; }
#ifdef ENTITY_H_SETFLAG_DIRECT // MATCH: the plain form, for Game/Atel/Atel (createAvatar) and Game/Atel/Avatar (ctor, InitAvatar)
inline void CEntity::setDispOffFlag(UINT inSet, UINT inMask) { m_uiDispOffFlag = (m_uiDispOffFlag | inSet) & ~inMask; }
#else
inline void CEntity::setDispOffFlag(UINT inSet, UINT inMask) { UINT& f = m_uiDispOffFlag; f = (f | inSet) & ~inMask; }
#endif
inline void CEntity::setCheckEyeOffFlag(UINT inSet, UINT inMask) { UINT& f = m_uiCheckEyeOffFlag; f = (f | inSet) & ~inMask; }
inline void CEntity::setNearTranslucence(FLOAT inTrans) { m_fNearTranslucence = inTrans; }
inline void CEntity::setRootMoveScale(const FVECTOR& inScale) { m_vRootMoveScale = inScale; }
inline const FVECTOR& CEntity::getStartPos() const { return m_vStartPos; }
inline INT CEntity::getID() const { return m_iID; }
inline ObjType::eObjType CEntity::getEntityType() const { return m_EntityType; }
inline UINT CEntity::getFlag() const { return m_uiEntityFlag; }
inline INT CEntity::getFlashingCounter() const { return m_iFlashingCounter; }
inline TIMEQ CEntity::getTimer() const { return m_tqTimer; }
inline FLOAT CEntity::getRate() const { return m_fRate; }
inline FLOAT CEntity::getSpeed() const { return m_fSpeed; }
inline FLOAT CEntity::getArriveRadius() const { return m_fArriveRadius; }
inline const FVECTOR& CEntity::getTargetPos() const { return m_vTargetPos; }
inline COLL_PARAM_HEADER* CEntity::getCollParam() const { return m_pCollParam; }
inline FLOAT CEntity::getFadeAlpha() const { return m_fFadeAlpha; }
inline FLOAT CEntity::getNearTranslucence() const { return m_fNearTranslucence; }
inline void CEntity::getCameraTargetPos(FVECTOR& outPos) { getUseCardPos(outPos); }
inline const FVECTOR& CEntity::getRootMoveScale() const { return m_vRootMoveScale; }
inline CModel* CEntity::getModel() { return m_pModels; }
inline CModel* CEntity::getModel() const { return m_pModels; }
inline INT CEntity::getModelCount() const { return m_iModelCount; }
inline CAnim* CEntity::getAnim() { return m_pAnim; }
inline CAnim* CEntity::getAnim() const { return m_pAnim; }
inline UINT CEntity::isDispOFF() const { return m_uiEntityFlag & 0x1; }
inline UINT CEntity::isDisableClearSpeed() const { return m_uiEntityFlag & 0x2; }
inline UINT CEntity::isTargetChase() const { return m_uiEntityFlag & 0x4; }
inline UINT CEntity::isTargetEvade() const { return m_uiEntityFlag & 0x8; }
inline UINT CEntity::isTargetIntercept() const { return m_uiEntityFlag & 0x10; }
inline UINT CEntity::isTargetMove() const { return m_uiEntityFlag & 0x20; }
inline UINT CEntity::isTargetMoveXZ() const { return m_uiEntityFlag & 0x40; }
inline UINT CEntity::isRotXOff() const { return m_uiEntityFlag & 0x80; }
inline UINT CEntity::isRotYOff() const { return m_uiEntityFlag & 0x100; }
inline UINT CEntity::isRotZOff() const { return m_uiEntityFlag & 0x200; }
inline UINT CEntity::isEffectOff() const { return m_uiEntityFlag & 0x400; }
inline UINT CEntity::isFlashing() const { return m_uiEntityFlag & 0x800; }
inline UINT CEntity::isTransOff() const { return m_uiEntityFlag & 0x1000; }
inline UINT CEntity::isRotZYX() const { return m_uiEntityFlag & 0x2000; }
inline UINT CEntity::isSeOff() const { return m_uiEntityFlag & 0x4000; }
inline UINT CEntity::isRootMoveOff() const { return m_uiEntityFlag & 0x8000; }
inline UINT CEntity::isPermissionLockOn() const { return m_uiEntityFlag & 0x10000; }
inline UINT CEntity::isHeartless() const { return m_uiEntityFlag & 0x20000; }
inline UINT CEntity::isSetDataCreate() const { return m_uiEntityFlag & 0x40000; }
inline UINT CEntity::isFade() const { return m_uiEntityFlag & 0x40000000; }
inline UINT CEntity::isStop() const { return m_uiEntityFlag & 0x80000000; }
inline UINT CEntity::isNoLevitate() const { return m_uiEntityFlag & 0x200000; }
inline UINT CEntity::isSetAlphaAttachEffect() const { return m_uiEntityFlag & 0x400000; }
inline BOOL CEntity::timeOver() const { return m_tqTimer <= 0; }
inline void CEntity::setupFade(FLOAT inStart, FLOAT inEnd, TIMEQ inTime, INT inModel) {
    setEntityFlag(0x40000000, 0);
    m_fStartAlpha = inStart;
    m_fEndAlpha = inEnd;
    m_tqFadeTimer = inTime;
    m_tqFadeTime = inTime;
    m_iFadeModel = inModel;
}
inline void CEntity::loadModelColor() { loadModelColor_(); }
inline INT CEntity::random() { return RND::genrand_int31(); }
inline FLOAT CEntity::rnd() { return RND::genrand_real1(); }
inline void CEntity::setID(INT inID) { m_iID = inID; }
inline void CEntity::updateBefore() {}
inline void CEntity::updateAfter() {}
inline void CEntity::updatePositionAfter() {}
inline void CEntity::lastUpdate() {}
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
