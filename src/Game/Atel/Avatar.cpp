#include "khtypes.h"
#define GAME_ATEL_AVATAR_CPP
#define ENTITY_H_SETFLAG_DIRECT
#include "Game/Atel/Avatar.h"
#include "Game/Atel/Atel.h"
#include "Game/Movement.h"
#include "Game/BodyCollision.h"
#include "System/Graphic/Anim.h"
#include "System/Graphic/Model.h"
#include "System/Graphic/HermitePath.h"
#include "include_asm.h"

using namespace ATEL;

extern "C" float atan2f(float, float);
class CRsrcDB { /* PARTIAL: System/RsrcDB.h needs the full RsrcData.h classes */
public:
    static BYTE* getRsrcPTD(const CHAR* pszName);
};
extern "C" char D_0048AD68[]; /* __FILE__ */
extern "C" char D_0048ADC0[];
extern "C" char D_0048ADC8[];

INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Game/Atel/Avatar", __Q24ATEL7CAvatarP5CTask);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Game/Atel/Avatar", _$_Q24ATEL7CAvatar);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Game/Atel/Avatar", InitAvatar__Q24ATEL7CAvatari);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Game/Atel/Avatar", changeAvatarMainState__Q24ATEL7CAvatari);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Game/Atel/Avatar", beginStateMain__Q24ATEL7CAvatar);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Game/Atel/Avatar", stateMain__Q24ATEL7CAvatar);
void CAvatar::stateMainEnd() {}
void CAvatar::stateSkipMain() {}
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Game/Atel/Avatar", updateBefore__Q24ATEL7CAvatar);
void CAvatar::updateAfter() {}
void CAvatar::lastUpdate() {
    if (m_pBodyColl) m_pBodyColl->update();
}
CAtel& CAvatar::getAtel() {
    CAtel* pAtel = (CAtel*)getParent();
    if (!(pAtel)) AssertApp(D_0048AD68, 360, D_0048ADC0, 0);
    return *pAtel;
}
CVSM& CAvatar::getVSM() {
    CVSM* pVSM = getAtel().getVSM();
    if (!(pVSM)) AssertApp(D_0048AD68, 370, D_0048ADC8, 0);
    return *pVSM;
}
void CAvatar::getNowPosition(FVECTOR& vPos) const { vPos = getPos(); }
void CAvatar::getPrevPosition(FVECTOR& vPos) const { vPos = getOldPos(); }
void CAvatar::getMoveInfo(ETMoveInfo& info) const {}
void CAvatar::getRotation(FVECTOR& vRot) const { vRot = getRot(); }
void CAvatar::getScale(FVECTOR& vScale) const { vScale = CEntity::getScale(); }
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Game/Atel/Avatar", getFade__CQ24ATEL7CAvatariRf);
FLOAT CAvatar::getDirection() const { return 0.0f; }
INT CAvatar::getTypeID() const { return 0x4000; }
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Game/Atel/Avatar", createMoveStruct__Q24ATEL7CAvatar);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Game/Atel/Avatar", createRotateStruct__Q24ATEL7CAvatar);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Game/Atel/Avatar", createScaleStruct__Q24ATEL7CAvatar);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Game/Atel/Avatar", createFadeStruct__Q24ATEL7CAvatar);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Game/Atel/Avatar", onInit__Q24ATEL7CAvatariPvi);
INT CAvatar::onSetActive(INT nCmd, void* pParam, INT nID) { return -1; }
INT CAvatar::onSetVisible(INT nCmd, void* pParam, INT nID) {
    if (nID) setEntityFlag(1, 0x10000);
    else {
        setEntityFlag(0, 1);
        setEntityFlag(0x10000, 0);
    }
    return -1;
}
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Game/Atel/Avatar", onSetVisibleModel__Q24ATEL7CAvatariPvi);
INT CAvatar::onSetGravity(INT nCmd, void* pParam, INT nID) {
    if (nID) m_pMove->enableGravity(TRUE);
    else m_pMove->enableGravity(FALSE);
    return -1;
}
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Game/Atel/Avatar", onSetCollision__Q24ATEL7CAvatariPvi);
INT CAvatar::onSetMapCollision(INT nCmd, void* pParam, INT nID) {
    if (nID) m_pMove->setFlag(1, FALSE);
    else m_pMove->setFlag(1, TRUE);
    return -1;
}
INT CAvatar::onCreateLegIK(INT nCmd, void* pParam, INT nID) {
    createLegIK(-1.0f, -1.0f);
    return -1;
}
INT CAvatar::onSetIK(INT nCmd, void* pParam, INT nID) {
    if (nID) setGroundIK(TRUE);
    else setGroundIK(FALSE);
    return -1;
}
INT CAvatar::onSetLockOn(INT nCmd, void* pParam, INT nID) {
    if (nID) setEntityFlag(0x10000, 0);
    else setEntityFlag(0, 0x10000);
    return -1;
}
INT CAvatar::onSetNearClip(INT nCmd, void* pParam, INT nID) {
    if (nID) setEntityFlag(0, 0x1000);
    else setEntityFlag(0x1000, 0);
    return -1;
}
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Game/Atel/Avatar", onSetPosistion__Q24ATEL7CAvatariPvi);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Game/Atel/Avatar", onSetPosPathTop__Q24ATEL7CAvatariPvi);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Game/Atel/Avatar", onMove__Q24ATEL7CAvatariPvi);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Game/Atel/Avatar", calcAvatarMoveSpeed__Q24ATEL7CAvatarR7FVECTOR);
void CAvatar::setAvatarGravity(BOOL bOn) {
    if (bOn) m_pMove->enableGravity(TRUE);
    else m_pMove->enableGravity(FALSE);
}
INT CAvatar::onStopMove(INT nCmd, void* pParam, INT nID) { return -1; }
INT CAvatar::onContinueMove(INT nCmd, void* pParam, INT nID) { return -1; }
void CAvatar::setRotAhead() {
    FVECTOR& vVel = m_pMove->getVel();
    getRot().y = atan2f(vVel.x, vVel.z);
}
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Game/Atel/Avatar", isArrivingTarget__Q24ATEL7CAvatar);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Game/Atel/Avatar", isArrivingTargetFuture__Q24ATEL7CAvatar);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Game/Atel/Avatar", MoveAvatarMain__Q24ATEL7CAvatar);
BOOL CAvatar::updateAvatarPosOnPath(TIMEQ nTime) {
    BOOL bRet = TRUE;
    if (m_pMoveStruct) bRet = m_pMoveStruct->updatePosOnPath(nTime * 0.02f);
    return bRet;
}
void CAvatar::MoveAvatarEnd() { clearSpeed(); }
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Game/Atel/Avatar", onRotate__Q24ATEL7CAvatariPvi);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Game/Atel/Avatar", setAvatarRotTimeLimit__Q24ATEL7CAvatari);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Game/Atel/Avatar", RotateAvatarMain__Q24ATEL7CAvatar);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Game/Atel/Avatar", RotateAvatarEnd__Q24ATEL7CAvatar);
INT CAvatar::onSetDirection(INT nCmd, void* pParam, INT nID) { return -1; }
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Game/Atel/Avatar", onFadeing__Q24ATEL7CAvatariPvi);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Game/Atel/Avatar", FadeAvatarMain__Q24ATEL7CAvatar);
void CAvatar::FadeAvatarEnd() {}
void CAvatar::cancelFadeAvatar() {}
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Game/Atel/Avatar", onScaling__Q24ATEL7CAvatariPvi);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Game/Atel/Avatar", ScaleAvatarMain__Q24ATEL7CAvatar);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Game/Atel/Avatar", ScaleAvatarEnd__Q24ATEL7CAvatar);
void CAvatar::cancelScaleAvatar() { ScaleAvatarEnd(); }
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Game/Atel/Avatar", TexAnimTalk__Q24ATEL7CAvatari);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Game/Atel/Avatar", onPlayMotion__Q24ATEL7CAvatariPvi);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Game/Atel/Avatar", onPlayMotionDx__Q24ATEL7CAvatariPvi);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Game/Atel/Avatar", onPlayMotionEx__Q24ATEL7CAvatariPvi);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Game/Atel/Avatar", onPlayMotionEfx__Q24ATEL7CAvatariPvi);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Game/Atel/Avatar", onPlayMotionExDx__Q24ATEL7CAvatariPvi);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Game/Atel/Avatar", onSetMotionFrame__Q24ATEL7CAvatariPvi);
INT CAvatar::onSetMakeUpMotionInValid(INT nCmd, void* pParam, INT nID) {
    if (nID) m_nMotInValidFlag = 1;
    else m_nMotInValidFlag = 0;
    return -1;
}
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Game/Atel/Avatar", onRemoveAvatar__Q24ATEL7CAvatariPvi);
INT CAvatar::onAvatarStop(INT nCmd, void* pParam, INT nID) {
    changeAvatarMainState(1);
    return -1;
}
INT CAvatar::onAvatarStart(INT nCmd, void* pParam, INT nID) {
    changeAvatarMainState(0);
    return -1;
}
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Game/Atel/Avatar", onAvatarAttach__Q24ATEL7CAvatariPvi);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Game/Atel/Avatar", onAvatarAttachEffect__Q24ATEL7CAvatariPvi);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Game/Atel/Avatar", onAvatarSetLookPosition__Q24ATEL7CAvatariPvi);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Game/Atel/Avatar", onAvatarSetLookUpdatePosition__Q24ATEL7CAvatariPvi);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Game/Atel/Avatar", onAvatarSetLookAngle__Q24ATEL7CAvatariPvi);
INT CAvatar::onAvatarSetLookUpDateAngle(INT nCmd, void* pParam, INT nID) {
    m_pAnim->updateLookPosition(*(FVECTOR*)((BYTE*)pParam + 0x20), *(FLOAT*)((BYTE*)pParam + 0x30), 1);
    return -1;
}
INT CAvatar::onAvatarLookStart(INT nCmd, void* pParam, INT nID) {
    setAvatarState(0x10000, TRUE);
    return -1;
}
INT CAvatar::onAvatarLookStop(INT nCmd, void* pParam, INT nID) {
    setAvatarState(0x10000, FALSE);
    return -1;
}
INT CAvatar::onAvatarLookEnd(INT nCmd, void* pParam, INT nID) {
    m_pAnim->endLookPosition(*(FLOAT*)pParam);
    return -1;
}
INT CAvatar::onAvatarChangeTexture(INT nCmd, void* pParam, INT nID) {
    const CHAR** ppName = (const CHAR**)pParam;
    m_pModels->changeTexture(ppName[0], ppName[1]);
    return -1;
}
INT CAvatar::onSetTexAnimSeq(INT nCmd, void* pParam, INT nID) {
    setTexAnmSeq(nID, (const CHAR*)pParam);
    return -1;
}
INT CAvatar::onEnableNotSemiModel(INT nCmd, void* pParam, INT nID) {
    enableNotSemiModel(*(INT*)pParam, nID);
    return -1;
}
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Game/Atel/Avatar", getAvatarDirY2Target__Q24ATEL7CAvatarPC7FVECTORPC7FMATRIX);
FVECTOR* CAvatar::getPathStartPos(const CHAR* pszName) const {
    FVECTOR* pPos = NULL;
    BYTE* pPTD = CRsrcDB::getRsrcPTD(pszName);
    if (pPTD) {
        if (*(USHORT*)(pPTD + 0xe)) {
            if (*(USHORT*)(pPTD + 0x10)) pPos = (FVECTOR*)(pPTD + 0x20);
        }
    }
    return pPos;
}
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Game/Atel/Avatar", disp__Q24ATEL7CAvatar);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Game/Atel/Avatar", _$_Q24ATEL16AvatarMoveStruct);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Game/Atel/Avatar", setupPath__Q24ATEL16AvatarMoveStructPCcUs);
void AvatarMoveStruct::getPosOnPath(FVECTOR& vPos) {
    if (_pHermiteHelper) _pHermiteHelper->getNowPos(vPos);
}
BOOL AvatarMoveStruct::updatePosOnPath(FLOAT fTime) {
    BOOL bRet = TRUE;
    if (_pHermiteHelper) bRet = _pHermiteHelper->update(fTime);
    return bRet;
}
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Game/Atel/Avatar", func_0023A008);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Game/Atel/Avatar", _$_t14CStateTemplate1ZQ24ATEL7CAvatar);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Game/Atel/Avatar", setParentActorIdx__Q24ATEL7CAvatarUi);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Game/Atel/Avatar", setActorIdx__Q24ATEL7CAvatarUi);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Game/Atel/Avatar", getParentActorIdx__CQ24ATEL7CAvatar);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Game/Atel/Avatar", getActorIdx__CQ24ATEL7CAvatar);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Game/Atel/Avatar", getAvatarState__CQ24ATEL7CAvatar);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Game/Atel/Avatar", getMovement__CQ24ATEL7CAvatar);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Game/Atel/Avatar", setAvatarState__Q24ATEL7CAvatarii);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Game/Atel/Avatar", isAvatarBusy__CQ24ATEL7CAvatar);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Game/Atel/Avatar", isAvatarActive__CQ24ATEL7CAvatar);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Game/Atel/Avatar", isAvatarVisible__CQ24ATEL7CAvatar);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Game/Atel/Avatar", isAvatarMoving__CQ24ATEL7CAvatar);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Game/Atel/Avatar", isAvatarSuspendingMove__CQ24ATEL7CAvatar);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Game/Atel/Avatar", isAvatarRotating__CQ24ATEL7CAvatar);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Game/Atel/Avatar", isAvatarScaling__CQ24ATEL7CAvatar);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Game/Atel/Avatar", isAvatarAnimating__CQ24ATEL7CAvatar);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Game/Atel/Avatar", isAvatarFadeing__CQ24ATEL7CAvatar);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Game/Atel/Avatar", isAvatarMovingFuture__CQ24ATEL7CAvatar);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Game/Atel/Avatar", isAvatarSuspendingMoveFuture__CQ24ATEL7CAvatar);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Game/Atel/Avatar", isAvatarRotatingFuture__CQ24ATEL7CAvatar);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Game/Atel/Avatar", isAvatarScalingFuture__CQ24ATEL7CAvatar);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Game/Atel/Avatar", isAvatarAnimatingFuture__CQ24ATEL7CAvatar);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Game/Atel/Avatar", isAvatarFadeingFuture__CQ24ATEL7CAvatar);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Game/Atel/Avatar", reqCommand__Q24ATEL7CAvatariPvi);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Game/Atel/Avatar", secTQ__CQ24ATEL7CAvatarf);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Game/Atel/Avatar", getTimeCountRate__CQ24ATEL7CAvatarii);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Game/Atel/Avatar", setAvatarFadeTimeLimit__Q24ATEL7CAvatarPQ24ATEL16AvatarFadeStructi);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Game/Atel/Avatar", setAvatarFadeTimeLimit__Q24ATEL7CAvatarf);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Game/Atel/Avatar", setAvatarTargetFade__Q24ATEL7CAvatarPQ24ATEL16AvatarFadeStructRCf);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Game/Atel/Avatar", setAvatarFadeSpeed__Q24ATEL7CAvatarPQ24ATEL16AvatarFadeStructRCf);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Game/Atel/Avatar", getAvatarTargetFade__CQ24ATEL7CAvatarPQ24ATEL16AvatarFadeStruct);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Game/Atel/Avatar", getAvatarFadeSpeed__CQ24ATEL7CAvatarPQ24ATEL16AvatarFadeStruct);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Game/Atel/Avatar", getAvatarFadeTimeLimit__CQ24ATEL7CAvatarPQ24ATEL16AvatarFadeStruct);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Game/Atel/Avatar", getAvatarFadeTimeCount__CQ24ATEL7CAvatarPQ24ATEL16AvatarFadeStruct);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Game/Atel/Avatar", getAvatarFadeTimeCountRate__CQ24ATEL7CAvatarPQ24ATEL16AvatarFadeStruct);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Game/Atel/Avatar", isOverAvatarFadeTimeLimit__CQ24ATEL7CAvatarPQ24ATEL16AvatarFadeStruct);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Game/Atel/Avatar", isOverAvatarFadeTimeLimit__CQ24ATEL7CAvatarPQ24ATEL16AvatarFadeStructi);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Game/Atel/Avatar", clearAvatarFadeSpeed__Q24ATEL7CAvatarPQ24ATEL16AvatarFadeStruct);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Game/Atel/Avatar", clearAvatarFadeTimeCount__Q24ATEL7CAvatarPQ24ATEL16AvatarFadeStruct);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Game/Atel/Avatar", countAvatarFadeTime__Q24ATEL7CAvatarPQ24ATEL16AvatarFadeStructi);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Game/Atel/Avatar", setAvatarScaleTimeLimit__Q24ATEL7CAvatari);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Game/Atel/Avatar", setAvatarScaleTimeLimit__Q24ATEL7CAvatarf);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Game/Atel/Avatar", setAvatarTargetScale__Q24ATEL7CAvatarRC7FVECTOR);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Game/Atel/Avatar", setAvatarScaleSpeed__Q24ATEL7CAvatarRC7FVECTOR);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Game/Atel/Avatar", getAvatarTargetScale__CQ24ATEL7CAvatar);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Game/Atel/Avatar", getAvatarScaleSpeed__CQ24ATEL7CAvatar);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Game/Atel/Avatar", getAvatarScaleTimeLimit__CQ24ATEL7CAvatar);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Game/Atel/Avatar", getAvatarScaleTimeCount__CQ24ATEL7CAvatar);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Game/Atel/Avatar", getAvatarScaleTimeCountRate__CQ24ATEL7CAvatar);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Game/Atel/Avatar", isOverAvatarScaleTimeLimit__CQ24ATEL7CAvatar);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Game/Atel/Avatar", isOverAvatarScaleTimeLimit__CQ24ATEL7CAvatari);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Game/Atel/Avatar", clearAvatarScaleSpeed__Q24ATEL7CAvatar);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Game/Atel/Avatar", clearAvatarScaleTimeCount__Q24ATEL7CAvatar);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Game/Atel/Avatar", countAvatarScaleTime__Q24ATEL7CAvatari);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Game/Atel/Avatar", setAvatarRotTimeLimit__Q24ATEL7CAvatarf);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Game/Atel/Avatar", setAvatarStartRot__Q24ATEL7CAvatarRC7FVECTOR);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Game/Atel/Avatar", setAvatarTargetRot__Q24ATEL7CAvatarRC7FVECTOR);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Game/Atel/Avatar", setAvatarRotSpeed__Q24ATEL7CAvatarRC7FVECTOR);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Game/Atel/Avatar", getAvatarRotType__CQ24ATEL7CAvatar);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Game/Atel/Avatar", getAvatarRotTimeLimit__CQ24ATEL7CAvatar);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Game/Atel/Avatar", getAvatarTargetRot__CQ24ATEL7CAvatar);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Game/Atel/Avatar", getAvatarRideMatrix__CQ24ATEL7CAvatar);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Game/Atel/Avatar", getAvatarStartRot__CQ24ATEL7CAvatar);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Game/Atel/Avatar", getAvatarRotSpeed__CQ24ATEL7CAvatar);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Game/Atel/Avatar", isAvatarRotEternal__CQ24ATEL7CAvatar);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Game/Atel/Avatar", isOverAvatarRotTimeLimit__CQ24ATEL7CAvatar);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Game/Atel/Avatar", isOverAvatarRotTimeLimit__CQ24ATEL7CAvatari);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Game/Atel/Avatar", clearAvatarRotTimeCount__Q24ATEL7CAvatar);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Game/Atel/Avatar", clearAvatarRotSpeed__Q24ATEL7CAvatar);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Game/Atel/Avatar", countAvatarRotTime__Q24ATEL7CAvatari);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Game/Atel/Avatar", setAvatarStartPos__Q24ATEL7CAvatarRC7FVECTOR);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Game/Atel/Avatar", getAvatarStartPos__CQ24ATEL7CAvatar);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Game/Atel/Avatar", getAvatarTargetPos__CQ24ATEL7CAvatar);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Game/Atel/Avatar", getAvatarStartPosLocal__CQ24ATEL7CAvatar);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Game/Atel/Avatar", getAvatarTargetPosLocal__CQ24ATEL7CAvatar);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Game/Atel/Avatar", setAvatarTargetPos__Q24ATEL7CAvatarRC7FVECTOR);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Game/Atel/Avatar", clearAvatarMoveTimeCount__Q24ATEL7CAvatar);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Game/Atel/Avatar", countAvatarMoveTime__Q24ATEL7CAvatari);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Game/Atel/Avatar", setAvatarMoveTimeLimit__Q24ATEL7CAvatari);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Game/Atel/Avatar", setAvatarMoveTimeLimit__Q24ATEL7CAvatarf);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Game/Atel/Avatar", setAvatarSpeed__Q24ATEL7CAvatarf);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Game/Atel/Avatar", getAvatarMoveType__CQ24ATEL7CAvatar);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Game/Atel/Avatar", getAvatarMoveTimeLimit__CQ24ATEL7CAvatar);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Game/Atel/Avatar", getAvatarSpeed__CQ24ATEL7CAvatar);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Game/Atel/Avatar", isAvatarMoveWithGravity__CQ24ATEL7CAvatar);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Game/Atel/Avatar", isOverAvatarMoveTimeLimit__CQ24ATEL7CAvatar);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Game/Atel/Avatar", isOverAvatarMoveTimeLimit__CQ24ATEL7CAvatari);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Game/Atel/Avatar", getAvatarMoveMotion__CQ24ATEL7CAvatar);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Game/Atel/Avatar", getAvatarPosOnPath__Q24ATEL7CAvatarR7FVECTOR);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Game/Atel/Avatar", isAvatarRotAhead__CQ24ATEL7CAvatar);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Game/Atel/Avatar", executeBeginState__t14CStateTemplate1ZQ24ATEL7CAvatar);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Game/Atel/Avatar", executeState__t14CStateTemplate1ZQ24ATEL7CAvatar);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Game/Atel/Avatar", executeEndState__t14CStateTemplate1ZQ24ATEL7CAvatar);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Game/Atel/Avatar", func_0023BF40);
