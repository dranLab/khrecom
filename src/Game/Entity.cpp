#include "khtypes.h"
#include "Game/Entity.h"
#include "Game/Movement.h"
#include "Game/BodyCollision.h"
#include "Game/Collision/Ride.h"
#include "System/Graphic/Anim.h"
#include "System/Graphic/Model.h"
#include "include_asm.h"

class CSEMotion {
public:
    void playVoice(INT inFileID, INT inID, INT inPriority);
};

static inline FLOAT _sqrtf(FLOAT x) {
    FLOAT ret;
    asm volatile("sqrt.s %0, %1" : "=f"(ret) : "f"(x));
    return ret;
}
static inline FLOAT _fabsf(FLOAT x) {
    FLOAT ret;
    asm volatile("abs.s %0, %1" : "=f"(ret) : "f"(x));
    return ret;
}

INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Game/Entity", __7CEntityP5CTaskQ27ObjType8eObjType);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Game/Entity", _$_7CEntity);
INT CEntity::getTypeID() const { return 0x1000; }
BOOL CEntity::initalize(INT inGroupID, INT inRsrcID) { return initalize(inGroupID, inRsrcID, 0); }
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Game/Entity", initalize__7CEntityiiUi);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Game/Entity", main__7CEntity);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Game/Entity", disp__7CEntity);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Game/Entity", setPos__7CEntityRC7FVECTOR);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Game/Entity", setOldPos__7CEntityRC7FVECTOR);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Game/Entity", setRot__7CEntityRC7FVECTOR);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Game/Entity", setQuat__7CEntityRC7FVECTOR);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Game/Entity", setScale__7CEntityRC7FVECTOR);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Game/Entity", setVel__7CEntityRC7FVECTOR);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Game/Entity", setHitBackVel__7CEntityRC7FVECTOR);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Game/Entity", setModelAlphaScale__7CEntityfi);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Game/Entity", setModelColorScale__7CEntityRC7FVECTORi);
void CEntity::setAnimSpeed(FLOAT inSpeed) {
    if (m_pAnim) m_pAnim->setSpeed(inSpeed);
}
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Game/Entity", setMoveFlag__7CEntityUiUi);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Game/Entity", getPos__C7CEntity);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Game/Entity", getOldPos__C7CEntity);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Game/Entity", getRot__C7CEntity);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Game/Entity", getRot__7CEntity);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Game/Entity", getQuat__C7CEntity);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Game/Entity", getQuat__7CEntity);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Game/Entity", getScale__C7CEntity);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Game/Entity", getVel__C7CEntity);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Game/Entity", getBoneIndex__C7CEntityPCci);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Game/Entity", getBoneName__C7CEntityii);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Game/Entity", getBoneMatrix__C7CEntityPCc);
TIMEQ CEntity::getMotionCurrentTime() const {
    if (m_pAnim) return m_pAnim->getCurrentTqCrd();
    return 0;
}
TIMEQ CEntity::getMotionTotalTime() const {
    if (m_pAnim) return m_pAnim->getTotalTqCrd();
    return 0;
}
INT CEntity::getMotionCurrentID() const {
    if (m_pAnim) return m_pAnim->getAnimID();
    return 0;
}
INT CEntity::getMotionCount() const {
    if (m_pAnim) return m_pAnim->getMotCount();
    return 0;
}
INT CEntity::getRideCollisionNum() const {
    if (!m_pRideModel) return 0;
    return m_pRideModel->getCollisionNum();
}
BOOL CEntity::isMotionEnd() const {
    if (m_pAnim) return m_pAnim->getFlag(0) & 3;
    return FALSE;
}
BOOL CEntity::isMotionLoop() const {
    if (m_pAnim) return m_pAnim->getFlag(0) & 1;
    return FALSE;
}
BOOL CEntity::isMotionLooped() const {
    if (m_pAnim) return m_pAnim->getFlag(0) & 0x80;
    return FALSE;
}
BOOL CEntity::isMotionLoopData(INT inIndex) const {
    if (m_pAnim) return m_pAnim->isLoop(inIndex);
    return FALSE;
}
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Game/Entity", isRideOnPlayer__7CEntity);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Game/Entity", isRideOnEnemy__7CEntity);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Game/Entity", isHangOnPlayer__7CEntity);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Game/Entity", isHangOnEnemy__7CEntity);
BOOL CEntity::isGround() const { return m_pMove->isGround(); }
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Game/Entity", enableFade2Path__7CEntityii);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Game/Entity", enableNotSemiModel__7CEntityiUi);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Game/Entity", getLockOnPos__7CEntityR7FVECTOR);
const CHAR* CEntity::getMotionName(INT inIndex) const {
    if (!m_pAnim) return NULL;
    return m_pAnim->getMotFilename(inIndex);
}
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Game/Entity", getMotionIndex__C7CEntityPCc);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Game/Entity", getHeight__C7CEntity);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Game/Entity", getHeadPos__C7CEntityR7FVECTOR);
void CEntity::getUseCardPos(FVECTOR& outPos) { outPos = *(FVECTOR*)((BYTE*)&Crd(0) + 0xc0); }
void CEntity::endStateProc() {
    if (!(m_uiEntityFlag & 2)) clearSpeed();
}
void CEntity::clearSpeed() {
    if (m_pMove) {
        FVECTOR v;
        v.init();
        m_pMove->setVel(v);
    }
}
FLOAT CEntity::calcDistance(const CEntity& inEntity) const { return _sqrtf(calcDistanceSq(inEntity)); }
FLOAT CEntity::calcDistance(const CEntity* inEntity) const {
    return inEntity ? _sqrtf(calcDistanceSq(*inEntity)) : 1e+04f;
}
FLOAT CEntity::calcDistance(const FVECTOR& inPos) const { return _sqrtf(calcDistanceSq(inPos)); }
FLOAT CEntity::calcDistanceXZ(const CEntity& inEntity) const { return _sqrtf(calcDistanceXZSq(inEntity)); }
FLOAT CEntity::calcDistanceXZ(const CEntity* inEntity) const {
    return inEntity ? _sqrtf(calcDistanceXZSq(*inEntity)) : 1e+08f;
}
FLOAT CEntity::calcDistanceXZ(const FVECTOR& inPos) const { return _sqrtf(calcDistanceXZSq(inPos)); }
FLOAT CEntity::calcDistanceSq(const CEntity& inEntity) const { return calcDistanceSq(inEntity.m_pMove->getPos()); }
FLOAT CEntity::calcDistanceSq(const CEntity* inEntity) const {
    if (inEntity) return calcDistanceSq(inEntity->m_pMove->getPos());
    return 1e+08f;
}
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Game/Entity", calcDistanceSq__C7CEntityRC7FVECTOR);
FLOAT CEntity::calcDistanceXZSq(const CEntity& inEntity) const { return calcDistanceXZSq(inEntity.m_pMove->getPos()); }
FLOAT CEntity::calcDistanceXZSq(const CEntity* inEntity) const {
    if (inEntity) return calcDistanceXZSq(inEntity->m_pMove->getPos());
    return 1e+08f;
}
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Game/Entity", calcDistanceXZSq__C7CEntityRC7FVECTOR);
FLOAT CEntity::calcDirection(const CEntity& inEntity) const { return calcDirection(inEntity.m_pMove->getPos()); }
FLOAT CEntity::calcDirection(const CEntity* inEntity) const {
    if (inEntity) return calcDirection(inEntity->m_pMove->getPos());
    return 0.0f;
}
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Game/Entity", calcDirection__C7CEntityRC7FVECTOR);
void CEntity::calcDirection(FVECTOR& outRot, const CEntity& inEntity) const { calcDirection(outRot, inEntity.m_pMove->getPos()); }
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Game/Entity", calcDirection__C7CEntityR7FVECTORPC7CEntity);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Game/Entity", calcDirection__C7CEntityR7FVECTORRC7FVECTOR);
BOOL CEntity::testFront(const CEntity& inEntity) const { return testFront(inEntity.getPos()); }
BOOL CEntity::testFront(const CEntity* inEntity) const {
    if (inEntity) return testFront(inEntity->getPos());
    return FALSE;
}
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Game/Entity", testFront__C7CEntityRC7FVECTOR);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Game/Entity", entityVisible__7CEntityRC7CEntityff);
BOOL CEntity::entityVisible(const CEntity* inEntity, FLOAT inAngle, FLOAT inDist) {
    if (inEntity) return entityVisible(*inEntity, inAngle, inDist);
    return FALSE;
}
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Game/Entity", entityVisibleAxisX__7CEntityRC7CEntityf);
BOOL CEntity::entityVisibleAxisX(const CEntity* inEntity, FLOAT inAngle) {
    if (inEntity) return entityVisibleAxisX(*inEntity, inAngle);
    return FALSE;
}
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Game/Entity", entityVisibleAxisY__7CEntityRC7CEntityf);
BOOL CEntity::entityVisibleAxisY(const CEntity* inEntity, FLOAT inAngle) {
    if (inEntity) return entityVisibleAxisY(*inEntity, inAngle);
    return FALSE;
}
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Game/Entity", positionFix__7CEntityRC7FVECTORf);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Game/Entity", positionFix__7CEntityRC7FVECTORT1);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Game/Entity", updateCoord__7CEntity);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Game/Entity", coordFix__7CEntity);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Game/Entity", updateRootMove__7CEntity);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Game/Entity", changeMotionFileName__7CEntityPCciiiii);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Game/Entity", changeMotion__7CEntityiiiiii);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Game/Entity", changeMotionFileNameWithSameCheck__7CEntityPCciiiii);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Game/Entity", changeMotionWithSameCheck__7CEntityiiiiii);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Game/Entity", changeMotionEx__7CEntityiiiiiii);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Game/Entity", changeMotionEx__7CEntityPCciiiiii);
void CEntity::setMotion(TIMEQ inTq) {
    if (m_pAnim) m_pAnim->setMotion(inTq, 0, FALSE);
}
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Game/Entity", setTexAnmSeq__7CEntityii);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Game/Entity", setTexAnmSeq__7CEntityiPCc);
BOOL CEntity::createLegIK(FLOAT inP0, FLOAT inP1) {
    if (m_pAnim) return m_pAnim->createLegIK(inP0, inP1);
    return FALSE;
}
BOOL CEntity::createArmIK() {
    if (m_pAnim) return m_pAnim->createArmIK();
    return FALSE;
}
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Game/Entity", setGroundIK__7CEntityi);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Game/Entity", setFootEffectIK__7CEntityPCci);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Game/Entity", isStandsGroundIK__7CEntity);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Game/Entity", isFirstStandsGroundIK__7CEntity);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Game/Entity", warp__7CEntityRC7FVECTOR);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Game/Entity", land__7CEntityffi);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Game/Entity", initalizeModel__7CEntityii);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Game/Entity", updateAnimation__7CEntity);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Game/Entity", updateBadStatusEffect__7CEntityi);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Game/Entity", setDamageNumber__7CEntityRC7FVECTORi);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Game/Entity", startTargetChase__7CEntityP7CEntityff);
void CEntity::endTargetChase() {
    m_uiEntityFlag = (INT)m_uiEntityFlag & ~4;
    clearSpeed();
}
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Game/Entity", startTargetEvade__7CEntityP7CEntityff);
void CEntity::endTargetEvade() {
    m_uiEntityFlag = (INT)m_uiEntityFlag & ~8;
    clearSpeed();
}
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Game/Entity", startTargetIntercept__7CEntityP7CEntityff);
void CEntity::endTargetIntercept() {
    m_uiEntityFlag = (INT)m_uiEntityFlag & ~0x10;
    clearSpeed();
}
BOOL CEntity::startTargetMove(const FVECTOR& inPos, FLOAT inSpeed, FLOAT inAccel, FLOAT inRange) {
    if (m_uiEntityFlag & 0x5c) return FALSE;
    m_fSpeed = inSpeed;
    m_uiEntityFlag |= 0x20;
    m_fRate = inAccel;
    m_fArriveRadius = inRange;
    m_vTargetPos = inPos;
    return TRUE;
}
void CEntity::endTargetMove() {
    m_uiEntityFlag = (INT)m_uiEntityFlag & ~0x20;
    clearSpeed();
}
BOOL CEntity::startTargetMoveXZ(const FVECTOR& inPos, FLOAT inSpeed, FLOAT inAccel, FLOAT inRange) {
    if (m_uiEntityFlag & 0x3c) return FALSE;
    m_fSpeed = inSpeed;
    m_uiEntityFlag |= 0x40;
    m_fRate = inAccel;
    m_fArriveRadius = inRange;
    m_vTargetPos = inPos;
    return TRUE;
}
void CEntity::endTargetMoveXZ() {
    m_uiEntityFlag = (INT)m_uiEntityFlag & ~0x40;
    clearSpeed();
}
void CEntity::bodyCollOn(INT inKind) {
    if (m_pBodyColl) m_pBodyColl->on(inKind);
}
void CEntity::bodyCollOnBody(INT inKind) {
    if (m_pBodyColl) m_pBodyColl->onBody(inKind);
}
void CEntity::bodyCollOff(INT inKind) {
    if (m_pBodyColl) m_pBodyColl->off(inKind);
}
void CEntity::bodyCollOffBody(INT inKind) {
    if (m_pBodyColl) m_pBodyColl->offBody(inKind);
}
void CEntity::userBodyCollOn(INT inIndex) {
    if (m_pBodyColl) m_pBodyColl->userOn(inIndex);
}
void CEntity::userBodyCollOnBody(INT inIndex) {
    if (m_pBodyColl) m_pBodyColl->userOnBody(inIndex);
}
void CEntity::userBodyCollOff(INT inIndex) {
    if (m_pBodyColl) m_pBodyColl->userOff(inIndex);
}
void CEntity::userBodyCollOffBody(INT inIndex) {
    if (m_pBodyColl) m_pBodyColl->userOffBody(inIndex);
}
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Game/Entity", arriveTarget__7CEntityRC7FVECTORf);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Game/Entity", arriveTargetXZ__7CEntityRC7FVECTORf);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Game/Entity", rideModelSetAttachMatrix__7CEntityPC7FMATRIX);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Game/Entity", rideModelSetChipLocalMatrix__7CEntityiPC7FMATRIX);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Game/Entity", rideModelEnableAllCollision__7CEntityi);
void CEntity::rideModelEnableChipCollision(INT inIdx, BOOL inSw) {
    if (m_pRideModel) m_pRideModel->enableChipCollision(inIdx, inSw);
}
void CEntity::rideModelChangeListR2() {
    if (m_pRideModel) m_pRideModel->changeListR2();
}
void CEntity::rideModelChangeListR() {
    if (m_pRideModel) m_pRideModel->changeListR();
}
void CEntity::rideModelChangeListRC() {
    if (m_pRideModel) m_pRideModel->changeListRC();
}
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Game/Entity", playSEPanAndVol__7CEntityiiiPC7FVECTORi);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Game/Entity", playSEPan__7CEntityiiiiPC7FVECTORi);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Game/Entity", stopSE__7CEntityiiii);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Game/Entity", fadeoutSE__7CEntityiiiii);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Game/Entity", setSEPanAndVol__7CEntityiiiiPC7FVECTORi);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Game/Entity", setSEPan__7CEntityiiiiiPC7FVECTORi);
void CEntity::playVoice(INT inFileID, INT inID, INT inPriority) {
    if (m_pSE) ((CSEMotion*)m_pSE)->playVoice(inFileID, inID, inPriority);
}
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Game/Entity", playSEManaged__7CEntityiiiPC7FVECTOR);
FLOAT CEntity::rndArea(FLOAT inMin, FLOAT inMax) { return inMin + getRandF() * _fabsf(inMax - inMin); }
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Game/Entity", initCollision___7CEntityii);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Game/Entity", initEffect___7CEntityi);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Game/Entity", initEffectEx___7CEntityi);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Game/Entity", initEffectEx___7CEntityPCc);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Game/Entity", initSE___7CEntityi);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Game/Entity", initSEEx___7CEntityi);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Game/Entity", initRideModel___7CEntityi);
void CEntity::updateTimer_() {
    if (m_tqTimer > 0) m_tqTimer -= getFrameRate();
}
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Game/Entity", updateIK___7CEntity);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Game/Entity", updateEffect___7CEntity);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Game/Entity", updateSE___7CEntity);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Game/Entity", updateFade___7CEntity);
void CEntity::saveRideModelMatrix_() {
    if (m_pRideModel) m_pRideModel->setOldMatrix();
}
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Game/Entity", rideModelAdjustRiders___7CEntity);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Game/Entity", chase___7CEntity);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Game/Entity", evade___7CEntity);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Game/Entity", intercept___7CEntity);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Game/Entity", targetMove___7CEntity);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Game/Entity", targetMoveXZ___7CEntity);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Game/Entity", loadModelColor___7CEntity);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Game/Entity", storeModelColor___7CEntity);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Game/Entity", translucence___7CEntity);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Game/Entity", setEffect__7CEntityPCcUiPC7FVECTORT3f);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Game/Entity", setEffect__7CEntityRC11EFFECT_DATAUiPC7FVECTORT3f);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Game/Entity", setEffect__7CEntityPCcUiPC7FMATRIXf);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Game/Entity", setEffect__7CEntityRC11EFFECT_DATAUiPC7FMATRIXf);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Game/Entity", attachEffect__7CEntityPCcUiPC6CCoordPC7FVECTORT4f);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Game/Entity", attachEffect__7CEntityRC11EFFECT_DATAUiPC6CCoordPC7FVECTORT4f);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Game/Entity", attachEffect__7CEntityPCcUiPC6CCoordPC7FMATRIXf);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Game/Entity", attachEffect__7CEntityRC11EFFECT_DATAUiPC6CCoordPC7FMATRIXf);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Game/Entity", attachEffect2__7CEntityPCcUiPC6CCoordT3PC7FVECTORT5f);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Game/Entity", attachEffect2__7CEntityRC11EFFECT_DATAUiPC6CCoordT3PC7FVECTORT5f);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Game/Entity", attachEffect2__7CEntityPCcUiPC6CCoordT3PC7FMATRIXT5f);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Game/Entity", attachEffect2__7CEntityRC11EFFECT_DATAUiPC6CCoordT3PC7FMATRIXT5f);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Game/Entity", attachEffect3__7CEntityPCcUiPC6CCoordN23PC7FVECTORN26f);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Game/Entity", attachEffect3__7CEntityRC11EFFECT_DATAUiPC6CCoordN23PC7FVECTORN26f);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Game/Entity", attachEffect3__7CEntityPCcUiPC6CCoordN23PC7FMATRIXN26f);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Game/Entity", attachEffect3__7CEntityRC11EFFECT_DATAUiPC6CCoordN23PC7FMATRIXN26f);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Game/Entity", killLoopEffect__7CEntityi);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Game/Entity", setAppearEffect__7CEntity);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Game/Entity", setDisappearEffect__7CEntity);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Game/Entity", setDeathEffect__7CEntityPC7FVECTOR);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Game/Entity", setDeathEffect__7CEntity);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Game/Entity", setBadStatusEffect__7CEntityPCcif);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Game/Entity", setRecoverEffect__7CEntityPCci);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Game/Entity", func_001F3E00);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Game/Entity", func_001F3E30);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Game/Entity", func_001F3E50);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Game/Entity", func_001F3E90);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Game/Entity", func_001F3E98);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Game/Entity", func_001F3ED0);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Game/Entity", AfterInit__7CEntity);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Game/Entity", onMsg__7CEntityiPv);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Game/Entity", setEntityType__7CEntityQ27ObjType8eObjType);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Game/Entity", setStartPos__7CEntityRC7FVECTOR);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Game/Entity", setEntityFlag__7CEntityUiUi);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Game/Entity", setTimer__7CEntityi);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Game/Entity", setTimerFrame__7CEntityi);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Game/Entity", setTimerSecond__7CEntityf);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Game/Entity", setRate__7CEntityf);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Game/Entity", setSpeed__7CEntityf);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Game/Entity", setArriveRadius__7CEntityf);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Game/Entity", setTargetPos__7CEntityRC7FVECTOR);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Game/Entity", setDispOffFlag__7CEntityUiUi);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Game/Entity", setCheckEyeOffFlag__7CEntityUiUi);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Game/Entity", setNearTranslucence__7CEntityf);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Game/Entity", setRootMoveScale__7CEntityRC7FVECTOR);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Game/Entity", getStartPos__C7CEntity);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Game/Entity", getID__C7CEntity);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Game/Entity", getEntityType__C7CEntity);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Game/Entity", getFlag__C7CEntity);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Game/Entity", getFlashingCounter__C7CEntity);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Game/Entity", getTimer__C7CEntity);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Game/Entity", getRate__C7CEntity);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Game/Entity", getSpeed__C7CEntity);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Game/Entity", getArriveRadius__C7CEntity);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Game/Entity", getTargetPos__C7CEntity);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Game/Entity", getCollParam__C7CEntity);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Game/Entity", getFadeAlpha__C7CEntity);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Game/Entity", getNearTranslucence__C7CEntity);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Game/Entity", getCameraTargetPos__7CEntityR7FVECTOR);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Game/Entity", getRootMoveScale__C7CEntity);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Game/Entity", getModel__7CEntity);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Game/Entity", getModel__C7CEntity);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Game/Entity", getModelCount__C7CEntity);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Game/Entity", getAnim__7CEntity);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Game/Entity", getAnim__C7CEntity);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Game/Entity", isDispOFF__C7CEntity);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Game/Entity", isDisableClearSpeed__C7CEntity);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Game/Entity", isTargetChase__C7CEntity);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Game/Entity", isTargetEvade__C7CEntity);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Game/Entity", isTargetIntercept__C7CEntity);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Game/Entity", isTargetMove__C7CEntity);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Game/Entity", isTargetMoveXZ__C7CEntity);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Game/Entity", isRotXOff__C7CEntity);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Game/Entity", isRotYOff__C7CEntity);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Game/Entity", isRotZOff__C7CEntity);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Game/Entity", isEffectOff__C7CEntity);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Game/Entity", isFlashing__C7CEntity);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Game/Entity", isTransOff__C7CEntity);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Game/Entity", isRotZYX__C7CEntity);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Game/Entity", isSeOff__C7CEntity);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Game/Entity", isRootMoveOff__C7CEntity);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Game/Entity", isPermissionLockOn__C7CEntity);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Game/Entity", isHeartless__C7CEntity);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Game/Entity", isSetDataCreate__C7CEntity);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Game/Entity", isFade__C7CEntity);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Game/Entity", isStop__C7CEntity);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Game/Entity", isNoLevitate__C7CEntity);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Game/Entity", isSetAlphaAttachEffect__C7CEntity);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Game/Entity", timeOver__C7CEntity);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Game/Entity", setupFade__7CEntityffii);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Game/Entity", loadModelColor__7CEntity);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Game/Entity", random__7CEntity);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Game/Entity", rnd__7CEntity);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Game/Entity", setID__7CEntityi);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Game/Entity", updateBefore__7CEntity);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Game/Entity", updateAfter__7CEntity);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Game/Entity", updatePositionAfter__7CEntity);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Game/Entity", lastUpdate__7CEntity);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Game/Entity", func_001F42E0);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Game/Entity", func_001F4308);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Game/Entity", func_001F4318);
