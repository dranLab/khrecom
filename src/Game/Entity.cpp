#define ANIM_H_GETFLAG_DECL_ONLY /* MATCH: the original calls CAnim::getFlag out of line */
#line 1
#define GAME_ENTITY_CPP /* MATCH: unit gate (its header gives this unit its own parts); without it the image differs (tested 2026-11-02) */
/* MATCH: __FILE__ of these headers as in the original .rodata */
#define BODYCOLLISION_H_PATH "c:/KHCOM/MainApp/Game/BodyCollision.h" /* MATCH: this unit names BodyCollision.h so (header default "Game/BodyCollision.h") */
#define COLLUTIL_H_PATH "c:/KHCOM/MainApp/Game/Collision/CollUtil.h" /* MATCH: this unit names CollUtil.h so (header default "Game/Collision/CollUtil.h") */
#define COLLDISP_H_PATH "c:/KHCOM/MainApp/Game/Collision/CollDisp.h" /* MATCH: this unit names CollDisp.h so (header default "Game/Collision/CollDisp.h") */
#define CARDBATTLE_H_PATH "c:/KHCOM/MainApp/Game/CardBattle.h" /* MATCH: this unit names CardBattle.h so (header default "Game/CardBattle.h") */
#include "khtypes.h"
#include "System/HeapMan.h"
#include "System/Graphic/LightUnit.h"
#include "System/TaskMan.h"
#include "System/Fsm.h"
#include "Game/Entity.h"
#include "System/Graphic/Gprim.h"
#include "System/Graphic/Model.h"
#include "System/RsrcData.h"
#include "System/Graphic/Anim.h"
#include "Game/BodyCollision.h"
#include "Game/Collision/CollUtil.h"
/* PARTIAL: an empty string between the CollUtil.h and CollDisp.h strings (owner unknown; the effect setters' strcmp
 * reuses it) */
inline void _entityEmptyStringPlaceholder() { TRACE(""); } /* MATCH: placeholder (see above) */
#include "Game/Collision/CollDisp.h"
#include "System/Coord.h"
#include "System/Graphic/Effect/effect.h"
#include "System/Graphic/2d/2DMan.h"
#include "Game/CardBattle.h"
#include "System/Graphic/Str.h"
#include "Game/Movement.h"
#include "Game/Collision/Ride.h"
#include "Game/EffectMotion.h"
#include "System/Graphic/TexAnm.h"
#include "System/MainApp.h"
#include "System/Sound/SoundMan.h"
#include "Game/SEMotion.h"
#include "System/Graphic/Camera.h"
#include "System/Math3D.h"
#include "Game/Collision/CollMap.h"
#include "Game/Debug/DebugWork.h"
#include "Game/AttackMotion.h"
#include "Game/SceneEnv.h"
#include "System/Graphic/Fnt.h"
#include "System/DebugProcInfo.h"
#include "System/Graphic/WorldEnv.h"

extern "C" unsigned int strlen(const char*);
extern "C" int sprintf(char*, const char*, ...);
void RotTransPersClipW(IVECTOR& iVec, const FVECTOR& fVec);
static inline void _loadRotTransPersMatrix(const FMATRIX& m) {
    asm volatile("vsub.xyzw $vf3, $vf0, $vf0\n"
                 "lqc2 $vf4, 0x0(%0)\n"
                 "lqc2 $vf5, 0x10(%0)\n"
                 "lqc2 $vf6, 0x20(%0)\n"
                 "lqc2 $vf7, 0x30(%0)\n"
                 :
                 : "r"(&m));
}
#include "System/RsrcDB.h"








extern "C" int strcmp(const char*, const char*);

void SubVector(FVECTOR& v0, const FVECTOR& v1, const FVECTOR& v2);
void SubVectorXYZ(FVECTOR& v0, const FVECTOR& v1, const FVECTOR& v2);
void MulVectorXYZ(FVECTOR& v0, const FVECTOR& v1, const FVECTOR& v2);
void AddVectorXYZ(FVECTOR& v0, const FVECTOR& v1, const FVECTOR& v2);
FLOAT InnerProduct(const FVECTOR& v0, const FVECTOR& v1);
void ApplyMatrix(FVECTOR& v0, const FMATRIX& m0, const FVECTOR& v1);
void ApplyMatrix33(FVECTOR& v0, const FMATRIX& m0, const FVECTOR& v1);
void Normalize(FVECTOR& v0, const FVECTOR& v1);
void ScaleVector(FVECTOR& v0, const FVECTOR& v1, FLOAT s);
void AddVector(FVECTOR& v0, const FVECTOR& v1, const FVECTOR& v2);
void MulVector(FVECTOR& v0, const FVECTOR& v1, const FVECTOR& v2);
extern "C" float atan2f(float, float);


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

CEntity::CEntity(CTask* inParent, ObjType::eObjType inType) : CTask(inParent, 0x4000) {
    m_pModels = NULL;
    m_pvModelColor = NULL;
    m_iModelCount = 0;
    m_pNLight = NULL;
    m_pGLight = NULL;
    m_pAnim = NULL;
    m_pTexAnm = NULL;
    m_nTexAnmNum = 0;
    m_pMove = NULL;
    m_pBodyColl = NULL;
    m_pAttack = NULL;
    m_pEffect = NULL;
    m_pEffectEx = NULL;
    m_pSE = NULL;
    m_pSEEx = NULL;
    m_iID = -1;
    setEntityType(inType);
    m_iFlashingCounter = 1;
    m_uiEntityFlag = 0x1000;
    m_uiDispOffFlag = 0;
    m_uiCheckEyeOffFlag = 0;
    m_tqTimer = 0;
    m_fNearTranslucence = 1.0f;
    m_hTarget = 0;
    m_fArriveRadius = 1.0f;
    m_fFadeAlpha = 1.0f;
    m_pCollParam = NULL;
    m_pRideModel = NULL;
    m_vStartPos.init();
    m_vRootMoveScale.set(1.0f, 1.0f, 1.0f);
    SE_MANAGED_DATA* data = m_SeManagedData;
    for (INT i = 0; i < 4; i++, data++) data->hHandle = 0;
    m_tqDamageDispTimer = 0;
    m_pDamageString = NULL;
}
CEntity::~CEntity() {
    delete m_pMove;
    m_pMove = NULL;
    delete m_pBodyColl;
    m_pBodyColl = NULL;
    delete m_pAttack;
    m_pAttack = NULL;
    delete m_pEffect;
    m_pEffect = NULL;
    delete m_pEffectEx;
    m_pEffectEx = NULL;
    delete m_pSE;
    m_pSE = NULL;
    delete m_pSEEx;
    m_pSEEx = NULL;
    delete[] m_pModels;
    m_pModels = NULL;
    delete[] m_pvModelColor;
    m_pvModelColor = NULL;
    delete m_pNLight;
    m_pNLight = NULL;
    delete m_pGLight;
    m_pGLight = NULL;
    delete m_pAnim;
    m_pAnim = NULL;
    delete[] m_pTexAnm;
    m_pTexAnm = NULL;
    delete m_pRideModel;
    m_pRideModel = NULL;
    delete m_pDamageString;
    m_pDamageString = NULL;
    TRACE("Delete entity ... ID = 0x%08x\n", getID());
}
INT CEntity::getTypeID() const { return 0x1000; }
BOOL CEntity::initalize(INT inGroupID, INT inRsrcID) { return initalize(inGroupID, inRsrcID, 0); }
BOOL CEntity::initalize(INT inGroupID, INT inRsrcID, UINT iFlag) {
    m_pMove = new (__FILE__ "(146)") CMovement(this);
    if (!m_pMove) return FALSE;
    INT group = inGroupID * 10000 + inRsrcID;
    if (!initalizeModel(group, !(iFlag & 1))) return FALSE;
    initCollision_(inGroupID, inRsrcID);
    initRideModel_(group);
    initEffect_(group);
    initSE_(group);
    m_pEffectEx = new (__FILE__ "(170)") CEffectMotion(this);
    if (!m_pEffectEx) return FALSE;
    m_pSEEx = new (__FILE__ "(174)") CSEMotion(this);
    if (!m_pSEEx) return FALSE;
    if (DebugWork::g_dw.sys.m_iDagameNumber) {
        m_pDamageString = new (__FILE__ "(181)") CFnt;
        if (m_pDamageString) {
            m_pDamageString->CFntM2::setStyle(0x8002);
            m_pDamageString->setSize(24);
            m_pDamageString->setLayerFG(1);
            m_pDamageString->setColor(255, 255, 255);
        }
    }
    return TRUE;
}
void CEntity::main() {
    if ((1 << m_EntityType) & CSceneEnv::getStopTypeBit()) return;
    if (m_uiEntityFlag & 0x80000000) return;
    storeModelColor_();
    if (m_pMove) {
        saveRideModelMatrix_();
        m_pMove->save();
        m_pMove->adjustRide();
        updateTimer_();
        updateBefore();
        update();
        updateAfter();
        chase_();
        evade_();
        intercept_();
        targetMove_();
        targetMoveXZ_();
        updateAnimation();
        if (CTaskMan::getSysTask(0x16) && CardBattle::CCardBattle::isLevitate() && !(m_uiEntityFlag & 0x200000)) m_pMove->setForce(0.5f);
        else m_pMove->setForce(1.0f);
        m_pMove->update();
        updatePositionAfter();
        m_pMove->notifyRide();
        updateCoord();
        coordFix();
        updateIK_();
        lastUpdate();
        updateEffect_();
        updateSE_();
        rideModelAdjustRiders_();
        updateFade_();
    } else {
        updateTimer_();
        updateBefore();
        update();
        updateAfter();
        lastUpdate();
    }
    loadModelColor_();
    if (m_tqDamageDispTimer > 0) m_tqDamageDispTimer -= getFrameRate();
}
void CEntity::disp() {
    translucence_();
    if (isDispOFF()) return;
    if (isFlashing()) {
        if ((m_iFlashingCounter ^= 1)) return;
    }
    if (m_pModels) {
        if (m_pNLight) m_pNLight->calc();
        if (m_pGLight) m_pGLight->calc();
        for (INT i = 0; i < m_iModelCount; i++) {
            if (!(m_uiDispOffFlag & (1 << i))) m_pModels[i].disp();
        }
    }
    if (DebugWork::g_dw.sys.m_iDagameNumber && m_pDamageString && m_tqDamageDispTimer > 0) m_pDamageString->disp();
}
void CEntity::setPos(const FVECTOR& inV) {
    if (!(m_pMove)) AssertApp(__FILE__, 369, "m_pMove", 0);
    *(FVECTOR*)((BYTE*)m_pMove + 0x20) = inV;
}
void CEntity::setOldPos(const FVECTOR& inV) {
    if (!(m_pMove)) AssertApp(__FILE__, 377, "m_pMove", 0);
    *(FVECTOR*)((BYTE*)m_pMove + 0x0) = inV;
}
void CEntity::setRot(const FVECTOR& inV) {
    if (!(m_pMove)) AssertApp(__FILE__, 385, "m_pMove", 0);
    *(FVECTOR*)((BYTE*)m_pMove + 0x40) = inV;
}
void CEntity::setQuat(const FVECTOR& inV) {
    if (!(m_pMove)) AssertApp(__FILE__, 393, "m_pMove", 0);
    *(FVECTOR*)((BYTE*)m_pMove + 0x50) = inV;
}
void CEntity::setScale(const FVECTOR& inV) {
    if (!(m_pMove)) AssertApp(__FILE__, 401, "m_pMove", 0);
    *(FVECTOR*)((BYTE*)m_pMove + 0x60) = inV;
}
void CEntity::setVel(const FVECTOR& inV) {
    if (!(m_pMove)) AssertApp(__FILE__, 409, "m_pMove", 0);
    *(FVECTOR*)((BYTE*)m_pMove + 0x70) = inV;
}
void CEntity::setHitBackVel(const FVECTOR& inV) {
    if (!(m_pMove)) AssertApp(__FILE__, 417, "m_pMove", 0);
    *(FVECTOR*)((BYTE*)m_pMove + 0x90) = inV;
}
void CEntity::setModelAlphaScale(FLOAT inAlpha, INT inIndex) {
    if (m_pModels) {
        if (inIndex < 0) {
            CModel* pModel = m_pModels;
            INT n = m_iModelCount;
            for (INT i = 0; i < n; i++) {
                pModel->setAlphaScale(inAlpha);
                pModel++;
            }
        } else {
            if (!(0 <= inIndex && inIndex < m_iModelCount)) AssertApp(__FILE__, 440, "0 <= inIndex && inIndex < m_iModelCount", 0);
            m_pModels[inIndex].setAlphaScale(inAlpha);
        }
    }
}
void CEntity::setModelColorScale(const FVECTOR& inColor, INT inIndex) {
    if (m_pModels) {
        if (inIndex < 0) {
            CModel* pModel = m_pModels;
            INT n = m_iModelCount;
            for (INT i = 0; i < n; i++) {
                pModel->setColorScale(inColor);
                pModel++;
            }
        } else {
            if (!(0 <= inIndex && inIndex < m_iModelCount)) AssertApp(__FILE__, 465, "0 <= inIndex && inIndex < m_iModelCount", 0);
            m_pModels[inIndex].setColorScale(inColor);
        }
    }
}
void CEntity::setAnimSpeed(FLOAT inSpeed) {
    if (m_pAnim) m_pAnim->setSpeed(inSpeed);
}
void CEntity::setMoveFlag(UINT inSet, UINT inMask) {
    if (!(m_pMove)) AssertApp(__FILE__, 487, "m_pMove", 0);
    UINT* pFlag = (UINT*)((BYTE*)m_pMove + 0x108);
    *pFlag = (*pFlag | inSet) & ~inMask;
}
const FVECTOR& CEntity::getPos() const {
    if (!(m_pMove)) AssertApp(__FILE__, 495, "m_pMove", 0);
    return *(FVECTOR*)((BYTE*)m_pMove + 0x20);
}
const FVECTOR& CEntity::getOldPos() const {
    if (!(m_pMove)) AssertApp(__FILE__, 503, "m_pMove", 0);
    return *(FVECTOR*)((BYTE*)m_pMove + 0x10);
}
const FVECTOR& CEntity::getRot() const {
    if (!(m_pMove)) AssertApp(__FILE__, 511, "m_pMove", 0);
    return *(FVECTOR*)((BYTE*)m_pMove + 0x40);
}
FVECTOR& CEntity::getRot() {
    if (!(m_pMove)) AssertApp(__FILE__, 516, "m_pMove", 0);
    return *(FVECTOR*)((BYTE*)m_pMove + 0x40);
}
const FVECTOR& CEntity::getQuat() const {
    if (!(m_pMove)) AssertApp(__FILE__, 524, "m_pMove", 0);
    return *(FVECTOR*)((BYTE*)m_pMove + 0x50);
}
FVECTOR& CEntity::getQuat() {
    if (!(m_pMove)) AssertApp(__FILE__, 529, "m_pMove", 0);
    return *(FVECTOR*)((BYTE*)m_pMove + 0x50);
}
const FVECTOR& CEntity::getScale() const {
    if (!(m_pMove)) AssertApp(__FILE__, 538, "m_pMove", 0);
    return *(FVECTOR*)((BYTE*)m_pMove + 0x60);
}
const FVECTOR& CEntity::getVel() const {
    if (!(m_pMove)) AssertApp(__FILE__, 546, "m_pMove", 0);
    return *(FVECTOR*)((BYTE*)m_pMove + 0x70);
}
INT CEntity::getBoneIndex(const CHAR* inBoneName, INT inModelIndex) const {
    if (!(inBoneName)) AssertApp(__FILE__, 554, "inBoneName", 0);
    if (m_pModels) return m_pModels[inModelIndex].getBoneIndex(inBoneName);
    return -1;
}
const CHAR* CEntity::getBoneName(INT inBone, INT inModel) const {
    if (m_pModels) return (const CHAR*)(*(BYTE**)(*(BYTE**)((BYTE*)&m_pModels[inModel] + 0x58) + 4) + inBone * 0x14);
    return NULL;
}
FMATRIX* CEntity::getBoneMatrix(const CHAR* inBoneName) const {
    if (!(inBoneName)) AssertApp(__FILE__, 581, "inBoneName", 0);
    INT nIdx = getBoneIndex(inBoneName, 0);
    if (nIdx >= 0) return (FMATRIX*)((BYTE*)&Crd(nIdx) + 0x90);
    return NULL;
}
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
BOOL CEntity::isRideOnPlayer() {
    if (!m_pRideModel) return FALSE;
    if (m_pRideModel->getRiderListSize() <= 0) return FALSE;
    return m_pRideModel->isRideOnPlayer();
}
BOOL CEntity::isRideOnEnemy() {
    if (!m_pRideModel) return FALSE;
    if (m_pRideModel->getRiderListSize() <= 0) return FALSE;
    return m_pRideModel->isRideOnEnemy();
}
BOOL CEntity::isHangOnPlayer() {
    if (!m_pRideModel) return FALSE;
    if (m_pRideModel->getRiderListSize() <= 0) return FALSE;
    return m_pRideModel->isHangOnPlayer();
}
BOOL CEntity::isHangOnEnemy() {
    if (!m_pRideModel) return FALSE;
    if (m_pRideModel->getRiderListSize() <= 0) return FALSE;
    return m_pRideModel->isHangOnEnemy();
}
BOOL CEntity::isGround() const { return m_pMove->isGround(); }
void CEntity::enableFade2Path(BOOL inSw, INT inIndex) {
    if (m_pModels) {
        if (inIndex < 0) {
            CModel* pModel = m_pModels;
            INT n = m_iModelCount;
            for (INT i = 0; i < n; i++) {
                pModel->fade2Path(inSw);
                pModel++;
            }
        } else {
            if (!(0 <= inIndex && inIndex < m_iModelCount)) AssertApp(__FILE__, 802, "0 <= inIndex && inIndex < m_iModelCount", 0);
            m_pModels[inIndex].fade2Path(inSw);
        }
    }
}
void CEntity::enableNotSemiModel(INT inModel, UINT inFlag) {
    if (m_pModels) {
        CModel* pModel = m_pModels;
        INT n = m_iModelCount;
        for (INT i = 0; i < n; i++) {
            if (inFlag & 1) {
                INT f = *(USHORT*)((BYTE*)pModel + 0x30);
                if (inModel) f |= 0x800;
                else f &= ~0x800;
                *(USHORT*)((BYTE*)pModel + 0x30) = f;
            }
            inFlag >>= 1;
            pModel++;
        }
    }
}
void CEntity::getLockOnPos(FVECTOR& outPos) {
    if (m_pCollParam) {
        BYTE* pLock = *(BYTE**)((BYTE*)m_pCollParam + 0x18);
        outPos.set(*(FLOAT*)(pLock + 0x14), *(FLOAT*)(pLock + 0x18), *(FLOAT*)(pLock + 0x1c));
        ApplyMatrix(outPos, *(FMATRIX*)((BYTE*)&Crd(*(INT*)(pLock + 0x10)) + 0x90), outPos);
    } else {
        outPos = *(FVECTOR*)((BYTE*)&Crd(0) + 0xc0);
    }
}
const CHAR* CEntity::getMotionName(INT inIndex) const {
    if (!m_pAnim) return NULL;
    return m_pAnim->getMotFilename(inIndex);
}
INT CEntity::getMotionIndex(const CHAR* inName) const {
    if (!(inName)) AssertApp(__FILE__, 865, "inName", 0);
    if (!m_pAnim) return -1;
    INT n = m_pAnim->getMotCount();
    INT start = 0;
    const CHAR* str = &inName[strlen(inName) - 1];
    if (*str >= '0' && *str <= '9') {
        INT value = 0;
        INT digit = 1;
        for (INT i = 0; i < 3; i++) {
            value += (*str-- - '0') * digit;
            digit *= 10;
        }
        if (value < n) {
            const CHAR* name = m_pAnim->getMotFilename(value);
            if (name && !strcmp(name, inName)) return value;
            start = value + 1;
            value--;
        } else {
            value = n - 1;
        }
        for (INT i = value; i >= 0; i--) {
            const CHAR* name = m_pAnim->getMotFilename(i);
            if (name && !strcmp(name, inName)) return i;
        }
    }
    for (INT i = start; i < n; i++) {
        const CHAR* name = m_pAnim->getMotFilename(i);
        if (name && !strcmp(name, inName)) return i;
    }
    TRACE("\x8ew\x92\xe8\x82\xb3\x82\xea\x82\xbd\x83\x82\x81[\x83V\x83\x87\x83\x93\x96\xbc\x82\xcd\x82\xa0\x82\xe8\x82\xdc\x82\xb9\x82\xf1 ... %s\n", inName);
    return -1;
}
FLOAT CEntity::getHeight() const {
    FLOAT heightRide = 1e+04f;
    FLOAT heightMap = 1e+04f;
    if (!(m_pMove)) AssertApp(__FILE__, 929, "m_pMove", 0);
    CCollMap* coll = m_pMove->getMapCollision();
    FVECTOR pos = getPos();
    coll->setStartPos(pos);
    pos.y -= 10.0f;
    coll->setEndPos(pos);
    if (coll->collideRide(FALSE)) {
        if (coll->getDetectNormal()->y >= 0.64278764f) heightRide = _fabsf(getPos().y - coll->getOutPos()->y);
    }
    if (coll->collideMap()) {
        if (coll->getDetectNormal()->y >= 0.64278764f) heightMap = _fabsf(getPos().y - coll->getOutPos()->y);
    }
    if (heightRide < heightMap) return heightRide;
    return heightMap;
}
void CEntity::getHeadPos(FVECTOR& outPos) const {
    COLL_PARAM_HEADER* coll = m_pCollParam;
    if (!coll) {
        outPos = *(FVECTOR*)((BYTE*)&Crd(0) + 0xc0);
        outPos.y += 1.0f;
    } else {
        OTHER_PARAM* other = coll->pOtherParam;
        if (!(other)) AssertApp(__FILE__, 979, "other", 0);
        outPos.set(other->BadStatusPos.fOffsetX, other->BadStatusPos.fOffsetY, other->BadStatusPos.fOffsetZ);
        ApplyMatrix(outPos, *(FMATRIX*)((BYTE*)&Crd(other->BadStatusPos.iBoneIndex) + 0x90), outPos);
    }
}
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
FLOAT CEntity::calcDistanceSq(const FVECTOR& inPos) const {
    FVECTOR v;
    if (!(m_pMove)) AssertApp(__FILE__, 1072, "m_pMove", 0);
    SubVector(v, m_pMove->getPos(), inPos);
    return InnerProduct(v, v);
}
FLOAT CEntity::calcDistanceXZSq(const CEntity& inEntity) const { return calcDistanceXZSq(inEntity.m_pMove->getPos()); }
FLOAT CEntity::calcDistanceXZSq(const CEntity* inEntity) const {
    if (inEntity) return calcDistanceXZSq(inEntity->m_pMove->getPos());
    return 1e+08f;
}
FLOAT CEntity::calcDistanceXZSq(const FVECTOR& inPos) const {
    if (!(m_pMove)) AssertApp(__FILE__, 1095, "m_pMove", 0);
    FVECTOR v0 = m_pMove->getPos();
    FVECTOR v1 = inPos;
    v0.y = v1.y = 0.0f;
    SubVector(v0, v0, v1);
    return InnerProduct(v0, v0);
}
FLOAT CEntity::calcDirection(const CEntity& inEntity) const { return calcDirection(inEntity.m_pMove->getPos()); }
FLOAT CEntity::calcDirection(const CEntity* inEntity) const {
    if (inEntity) return calcDirection(inEntity->m_pMove->getPos());
    return 0.0f;
}
FLOAT CEntity::calcDirection(const FVECTOR& inPos) const {
    FVECTOR v;
    if (!(m_pMove)) AssertApp(__FILE__, 1123, "m_pMove", 0);
    SubVector(v, inPos, m_pMove->getPos());
    return atan2f(v.x, v.z);
}
void CEntity::calcDirection(FVECTOR& outRot, const CEntity& inEntity) const { calcDirection(outRot, inEntity.m_pMove->getPos()); }
void CEntity::calcDirection(FVECTOR& outRot, const CEntity* inEntity) const {
    if (inEntity) calcDirection(outRot, inEntity->m_pMove->getPos());
    else outRot = inEntity->getRot();
}
void CEntity::calcDirection(FVECTOR& outRot, const FVECTOR& inPos) const {
    FVECTOR v;
    if (!(m_pMove)) AssertApp(__FILE__, 1147, "m_pMove", 0);
    SubVector(v, inPos, m_pMove->getPos());
    Normalize(v, v);
    outRot.x = -atan2f(v.y, _sqrtf(v.z * v.z + v.x * v.x));
    outRot.y = atan2f(v.x, v.z);
    outRot.z = 0.0f;
}
BOOL CEntity::testFront(const CEntity& inEntity) const { return testFront(inEntity.getPos()); }
BOOL CEntity::testFront(const CEntity* inEntity) const {
    if (inEntity) return testFront(inEntity->getPos());
    return FALSE;
}
BOOL CEntity::testFront(const FVECTOR& inTarget) const {
    FVECTOR n = {{0.0f, 0.0f, 1.0f, 1.0f}};
    ApplyMatrix33(n, *(FMATRIX*)((BYTE*)&Crd(0) + 0x90), n);
    n.w = InnerProduct(n, *(FVECTOR*)((BYTE*)&Crd(0) + 0xc0));
    return InnerProduct(n, inTarget) - n.w > 0.0f;
}
BOOL CEntity::entityVisible(const CEntity& inTarget, FLOAT inDegX, FLOAT inDegY) {
    FVECTOR dir;
    calcDirection(dir, inTarget);
    FLOAT dir0 = dir.y;
    getRot();
    FLOAT fDiff = _fabsf(Math3D::LimitPI(dir0 - getRot().y));
    if (inDegY * 0.5f * 0.017453292f < fDiff) return FALSE;
    dir0 = dir.x;
    getRot();
    fDiff = _fabsf(Math3D::LimitPI(dir0 - getRot().x));
    if (inDegX * 0.5f * 0.017453292f < fDiff) return FALSE;
    return TRUE;
}
BOOL CEntity::entityVisible(const CEntity* inEntity, FLOAT inAngle, FLOAT inDist) {
    if (inEntity) return entityVisible(*inEntity, inAngle, inDist);
    return FALSE;
}
BOOL CEntity::entityVisibleAxisX(const CEntity& inTarget, FLOAT inAngle) {
    FVECTOR dir;
    SubVector(dir, inTarget.getPos(), m_pMove->getPos());
    Normalize(dir, dir);
    FLOAT dir0 = -atan2f(dir.y, _sqrtf(dir.z * dir.z + dir.x * dir.x));
    getRot();
    FLOAT fDiff = _fabsf(Math3D::LimitPI(dir0 - getRot().x));
    if (inAngle * 0.5f * 0.017453292f < fDiff) return FALSE;
    return TRUE;
}
BOOL CEntity::entityVisibleAxisX(const CEntity* inEntity, FLOAT inAngle) {
    if (inEntity) return entityVisibleAxisX(*inEntity, inAngle);
    return FALSE;
}
BOOL CEntity::entityVisibleAxisY(const CEntity& inEntity, FLOAT inAngle) {
    FLOAT fDir = calcDirection(inEntity);
    getRot();
    FLOAT fDiff = _fabsf(Math3D::LimitPI(fDir - getRot().y));
    if (inAngle * 0.5f * 0.017453292f < fDiff) return FALSE;
    return TRUE;
}
BOOL CEntity::entityVisibleAxisY(const CEntity* inEntity, FLOAT inAngle) {
    if (inEntity) return entityVisibleAxisY(*inEntity, inAngle);
    return FALSE;
}
void CEntity::positionFix(const FVECTOR& inPos, FLOAT inDir) {
    setPos(inPos);
    setOldPos(inPos);
    FVECTOR vRot;
    vRot.set(0.0f, inDir, 0.0f);
    setRot(vRot);
    updateCoord();
    coordFix();
    if (m_pBodyColl) m_pBodyColl->update();
}
void CEntity::positionFix(const FVECTOR& inPos, const FVECTOR& inRot) {
    setPos(inPos);
    setOldPos(inPos);
    setRot(inRot);
    updateCoord();
    coordFix();
    if (m_pBodyColl) m_pBodyColl->update();
}
void CEntity::updateCoord() {
    if (!(m_pMove)) AssertApp(__FILE__, 1286, "m_pMove", 0);
    FMATRIX& m = Crd(0).m_Local;
    /* MATCH: the scale diagonal as one asm on fixed registers with only input operands (outputs or C stores make
     * gcc reload m_pMove afterwards) */
    asm volatile("lq $5, 0x0(%0)\n"
                 "pextuw $2, $5, $0\n"
                 "pextlw $4, $0, $2\n"
                 "pextlw $2, $0, $5\n"
                 "pextuw $3, $2, $0\n"
                 "pcpyld $2, $0, $2\n"
                 "sq $2, 0x0(%1)\n"
                 "sq $3, 0x10(%1)\n"
                 "sq $4, 0x20(%1)\n"
                 "sqc2 $vf0, 0x30(%1)"
                 : : "r"(&m_pMove->getScl()), "r"(&m) : "$2", "$3", "$4", "$5");
    if (m_pMove->isQuaternion()) {
        FMATRIX mat;
        Quaternion::QuaternionToMatrix(mat, m_pMove->getQuaternion());
        asm volatile("lqc2 $vf2, 0x0(%0)\n"
                     "lqc2 $vf3, 0x10(%0)\n"
                     "lqc2 $vf4, 0x20(%0)\n"
                     "lqc2 $vf5, 0x30(%0)\n"
                     "lqc2 $vf6, 0x0(%1)\n"
                     "lqc2 $vf7, 0x10(%1)\n"
                     "lqc2 $vf8, 0x20(%1)\n"
                     "lqc2 $vf9, 0x30(%1)\n"
                     "vmulax.xyzw $ACC, $vf2, $vf6x\n"
                     "vmadday.xyzw $ACC, $vf3, $vf6y\n"
                     "vmaddaz.xyzw $ACC, $vf4, $vf6z\n"
                     "vmaddw.xyzw $vf6, $vf5, $vf6w\n"
                     "vmulax.xyzw $ACC, $vf2, $vf7x\n"
                     "vmadday.xyzw $ACC, $vf3, $vf7y\n"
                     "vmaddaz.xyzw $ACC, $vf4, $vf7z\n"
                     "vmaddw.xyzw $vf7, $vf5, $vf7w\n"
                     "vmulax.xyzw $ACC, $vf2, $vf8x\n"
                     "vmadday.xyzw $ACC, $vf3, $vf8y\n"
                     "vmaddaz.xyzw $ACC, $vf4, $vf8z\n"
                     "vmaddw.xyzw $vf8, $vf5, $vf8w\n"
                     "vmulax.xyzw $ACC, $vf2, $vf9x\n"
                     "vmadday.xyzw $ACC, $vf3, $vf9y\n"
                     "vmaddaz.xyzw $ACC, $vf4, $vf9z\n"
                     "vmaddw.xyzw $vf9, $vf5, $vf9w\n"
                     "sqc2 $vf6, 0x0(%0)\n"
                     "sqc2 $vf7, 0x10(%0)\n"
                     "sqc2 $vf8, 0x20(%0)\n"
                     "sqc2 $vf9, 0x30(%0)"
                     : : "r"(&m), "r"(&mat) : "memory");
    } else if (m_uiEntityFlag & 0x2000) {
        if (!(m_uiEntityFlag & 0x200)) sceVu0RotMatrixZ(m.m, m.m, m_pMove->getRot().z);
        if (!(m_uiEntityFlag & 0x100)) sceVu0RotMatrixY(m.m, m.m, m_pMove->getRot().y);
        if (!(m_uiEntityFlag & 0x80)) sceVu0RotMatrixX(m.m, m.m, m_pMove->getRot().x);
    } else {
        if (!(m_uiEntityFlag & 0x80)) sceVu0RotMatrixX(m.m, m.m, m_pMove->getRot().x);
        if (!(m_uiEntityFlag & 0x100)) sceVu0RotMatrixY(m.m, m.m, m_pMove->getRot().y);
        if (!(m_uiEntityFlag & 0x200)) sceVu0RotMatrixZ(m.m, m.m, m_pMove->getRot().z);
    }
    FVECTOR pos = getPos();
    pos.w = 1.0f;
    m.v[3] = pos;
    Crd(0).m_Flag = 1;
}
void CEntity::coordFix() {
    INT n = getCoordNum();
    TIMEQ tick = CMainApp::getTickCount();
    for (INT i = 0; i < n; i++) Crd(i).fix(tick);
}
void CEntity::updateRootMove() {
    if (!m_pAnim) return;
    if (!m_pAnim->isRootMove()) return;
    if (isRootMoveOff()) return;
    FVECTOR trans;
    FLOAT rot_y;
    m_pAnim->getRootMove(trans, rot_y);
    if (!(m_pMove)) AssertApp(__FILE__, 1351, "m_pMove", 0);
    FVECTOR diff;
    SubVectorXYZ(diff, trans, getPos());
    MulVectorXYZ(diff, diff, m_vRootMoveScale);
    AddVectorXYZ(trans, getPos(), diff);
    m_pMove->setPos(trans);
    FVECTOR rot = m_pMove->getRot();
    rot.y = rot_y;
    m_pMove->setRot(rot);
}
void CEntity::changeMotionFileName(const CHAR* inName, TIMEQ inTq, BOOL inRootMove, TIMEQ inGr, INT inEffectNo, INT inSENo) {
    if (!(inName)) AssertApp(__FILE__, 1370, "inName", 0);
    INT nIndex = getMotionIndex(inName);
    if (nIndex != -1) changeMotion(nIndex, inTq, inRootMove, inGr, inEffectNo, inSENo);
}
void CEntity::changeMotion(INT inIndex, TIMEQ inTq, BOOL inRootMove, TIMEQ inGr, INT inEffectNo, INT inSENo) {
    if (m_pAnim) {
        if (m_pAnim->isNowExAnim()) {
            if (m_pEffectEx) m_pEffectEx->stop();
            if (m_pSEEx) m_pSEEx->stop();
        }
        if (inGr < 0) m_pAnim->start(inIndex, inTq);
        else m_pAnim->change(inIndex, inTq, inGr);
        m_pAnim->setRootMove(inRootMove, FALSE);
        m_pAnim->clearFlag();
        if (m_pEffect && !(m_uiEntityFlag & 0x400)) m_pEffect->set(inIndex, inEffectNo, TRUE);
        if (m_pSE && !(m_uiEntityFlag & 0x4000)) ((CSEMotion*)m_pSE)->set(inIndex, inSENo, TRUE);
    }
}
void CEntity::changeMotionFileNameWithSameCheck(const CHAR* inName, TIMEQ inTq, BOOL inRootMove, TIMEQ inGr, INT inEffectNo, INT inSENo) {
    if (!(inName)) AssertApp(__FILE__, 1433, "inName", 0);
    INT nIndex = getMotionIndex(inName);
    if (nIndex != -1) changeMotionWithSameCheck(nIndex, inTq, inRootMove, inGr, inEffectNo, inSENo);
}
void CEntity::changeMotionWithSameCheck(INT inIndex, TIMEQ inTq, BOOL inRootMove, TIMEQ inGr, INT inEffectNo, INT inSENo) {
    if (!(m_pAnim)) AssertApp(__FILE__, 1445, "m_pAnim", 0);
    if (m_pAnim->getAnimID() != inIndex) changeMotion(inIndex, inTq, inRootMove, inGr, inEffectNo, inSENo);
}
void CEntity::changeMotionEx(INT inRsrcID, INT inIndex, TIMEQ inTq, BOOL inRootMove, TIMEQ inGr, INT inEffectNo, INT inSENo) {
    if (m_pAnim) {
        if (!m_pAnim->isNowExAnim()) {
            if (m_pEffect) m_pEffect->stop();
            if (m_pSE) m_pSE->stop();
        }
        if (inGr < 0) m_pAnim->startEx(inRsrcID, inIndex, inTq);
        else m_pAnim->changeEx(inRsrcID, inIndex, inTq, inGr);
        m_pAnim->setRootMove(inRootMove, FALSE);
        m_pAnim->clearFlag();
        if (m_pEffectEx) {
            m_pEffectEx->initalize(inRsrcID);
            if (m_pEffectEx && !(m_uiEntityFlag & 0x400)) m_pEffectEx->set(inIndex, inEffectNo, TRUE);
        }
        if (m_pSEEx) {
            ((CSEMotion*)m_pSEEx)->initalize(inRsrcID);
            if (m_pSEEx && !(m_uiEntityFlag & 0x4000)) ((CSEMotion*)m_pSEEx)->set(inIndex, inSENo, TRUE);
        }
    }
}
void CEntity::changeMotionEx(const CHAR* inRsrcName, INT inIndex, TIMEQ inTq, BOOL inRootMove, TIMEQ inGr, INT inEffectNo, INT inSENo) {
    if (!(inRsrcName)) AssertApp(__FILE__, 1515, "inRsrcName", 0);
    if (m_pAnim) {
        if (inGr < 0) m_pAnim->startEx(inRsrcName, inIndex, inTq);
        else m_pAnim->changeEx(inRsrcName, inIndex, inTq, inGr);
        m_pAnim->setRootMove(inRootMove, FALSE);
        m_pAnim->clearFlag();
        if (m_pEffectEx && !(m_uiEntityFlag & 0x400)) m_pEffectEx->set(inIndex, inEffectNo, TRUE);
    }
}
void CEntity::setMotion(TIMEQ inTq) {
    if (m_pAnim) m_pAnim->setMotion(inTq, 0, FALSE);
}
INT CEntity::setTexAnmSeq(INT nAnmNo, INT nSeqNo) {
    if (m_pTexAnm) {
        if (nAnmNo < 0) return -1;
        if (nAnmNo < m_nTexAnmNum) return m_pTexAnm[nAnmNo].setSeq(nSeqNo);
    }
    return -1;
}
INT CEntity::setTexAnmSeq(INT nAnmNo, const CHAR* pszSeqName) {
    if (m_pTexAnm) {
        if (nAnmNo < 0) return -1;
        if (nAnmNo < m_nTexAnmNum) return m_pTexAnm[nAnmNo].setSeq(pszSeqName);
    }
    return -1;
}
BOOL CEntity::createLegIK(FLOAT inP0, FLOAT inP1) {
    if (m_pAnim) return m_pAnim->createLegIK(inP0, inP1);
    return FALSE;
}
BOOL CEntity::createArmIK() {
    if (m_pAnim) return m_pAnim->createArmIK();
    return FALSE;
}
void CEntity::setGroundIK(INT inFlag) {
    if (m_pAnim) {
        m_pAnim->setGroundIK(inFlag);

    }
}
void CEntity::setFootEffectIK(const CHAR* pszEffName, INT inFlag) {
    if (!(pszEffName)) AssertApp(__FILE__, 1621, "pszEffName", 0);
    if (m_pAnim) {
        BYTE* pIK = *(BYTE**)((BYTE*)m_pAnim + 0x100);
        if (pIK) {
            *(const CHAR**)(pIK + 0x28) = pszEffName;
            *(INT*)(pIK + 0x2c) = inFlag;
        }
    }
}
INT CEntity::isStandsGroundIK() {
    if (m_pAnim) {
        BYTE* pIK = *(BYTE**)((BYTE*)m_pAnim + 0x100);
        if (pIK) return *(UINT*)(pIK + 0x24) & 3;
    }
    return 0;
}
INT CEntity::isFirstStandsGroundIK() {
    if (m_pAnim) {
        BYTE* pIK = *(BYTE**)((BYTE*)m_pAnim + 0x100);
        if (pIK) {
            INT f = *(INT*)(pIK + 0x24);
            BOOL b = f & 4;
            INT n = b != 0;
            if (f & 8) n |= 2;
            return n;
        }
    }
    return 0;
}
void CEntity::warp(const FVECTOR& inPos) {
    if (!(m_pMove)) AssertApp(__FILE__, 1679, "m_pMove", 0);
    if (*(UINT*)((BYTE*)m_pMove + 0x110) & 4) {
        CCollMap* coll = *(CCollMap**)((BYTE*)m_pMove + 0x144);
        if (!(coll)) AssertApp(__FILE__, 1683, "coll", 0);
        coll->removeRide();
        *(UINT*)((BYTE*)m_pMove + 0x110) = (INT)*(UINT*)((BYTE*)m_pMove + 0x110) & ~4;
    }
    *(FVECTOR*)((BYTE*)m_pMove + 0x20) = inPos;
    *(FVECTOR*)((BYTE*)m_pMove + 0x00) = inPos;
}
void CEntity::land(FLOAT inDownLength, FLOAT inUpLength, BOOL inFirstHit) {
    if (!(m_pMove)) AssertApp(__FILE__, 1696, "m_pMove", 0);
    m_pMove->land(inDownLength, inUpLength, inFirstHit);
}
BOOL CEntity::initalizeModel(INT inGroup, BOOL bLight) {
    if ((m_iModelCount = CRsrcDB::getModelCount(inGroup)) <= 0) return FALSE;
    MODELDATA** pModelTbl = CRsrcDB::getRsrcMDL(inGroup);
    if (!pModelTbl) return FALSE;
    m_pModels = new (__FILE__ "(1714)") CModel[m_iModelCount];
    if (!m_pModels) return FALSE;
    m_pvModelColor = new (__FILE__ "(1718)") FVECTOR[m_iModelCount];
    if (!m_pvModelColor) return FALSE;
    if (bLight) {
        for (INT i = 0; i < m_iModelCount; i++) {
            if (pModelTbl[i]->enableLight()) {
                m_pNLight = new (__FILE__ "(1728)") CLightUnit;
                if (!m_pNLight) return FALSE;
                break;
            }
        }
        for (INT i = 0; i < m_iModelCount; i++) {
            if (pModelTbl[i]->enablePointLight()) {
                m_pGLight = new (__FILE__ "(1739)") CGimmickLightUnit;
                if (!m_pGLight) return FALSE;
                break;
            }
        }
    }
    TEXANM_DATA* pTXAData = CRsrcDB::getRsrcTXA(inGroup);
    if (pTXAData) {
        if ((m_nTexAnmNum = pTXAData->getDataNum()) > 0) {
            m_pTexAnm = new (__FILE__ "(1752)") CTexAnm[m_nTexAnmNum];
            if (m_pTexAnm) {
                for (INT i = 0; i < m_nTexAnmNum; i++) m_pTexAnm[i].setup(*pTXAData, i);
            }
        }
    }
    for (INT i = 0; i < m_iModelCount; i++) {
        m_pModels[i].link(this, pModelTbl[i]);
        m_pModels[i].linkTexAnm(m_pTexAnm, m_nTexAnmNum);
        if (bLight) {
            if (pModelTbl[i]->enablePointLight()) m_pModels[i].setLightUnit(m_pGLight);
            else if (pModelTbl[i]->enableLight()) m_pModels[i].setLightUnit(m_pNLight);
        }
        m_pvModelColor[i] = m_pModels[i].getColorScale();
        if ((UINT)inGroup / 10000 == 11) m_pModels[i].m_nFlag |= 0x40;
    }
    enableFade2Path(TRUE, -1);
    m_pAnim = new (__FILE__ "(1788)") CAnim;
    if (!m_pAnim) return FALSE;
    m_pAnim->link(this, CRsrcDB::getRsrcANN(inGroup), pModelTbl[0]);
    m_pAnim->init(0, 0);
    m_pAnim->setClipCheckFg(FALSE);
    INT kind = (m_EntityType == 1 || m_EntityType == 0x11) ? 1 : 2;
    if (m_pNLight) m_pNLight->setAnchor(&Crd(0), kind);
    if (m_pGLight) m_pGLight->setAnchor(&Crd(0), kind);
    m_iID = inGroup;
    return TRUE;
}

void CEntity::updateAnimation() {
    if (m_pAnim) {
        if (m_pAnim->isRootMove()) updateCoord();
        m_pAnim->play(getFrameRate(), isClippedLastTime());
        updateRootMove();
    }
    if (m_pTexAnm) {
        for (INT i = 0; i < m_nTexAnmNum; i++) m_pTexAnm[i].update(getFrameRateF());
    }
}
void CEntity::updateBadStatusEffect(HANDLE inHandle) {
    CEffect* effect = CEffectMan::getEffect(inHandle);
    if (!effect) return;
    if (!m_pCollParam) return;
    OTHER_PARAM* other = m_pCollParam->pOtherParam;
    if (!(other)) AssertApp(__FILE__, 1860, "other", 0);
    FVECTOR ofs;
    ofs.set(other->BadStatusPos.fOffsetX, other->BadStatusPos.fOffsetY, other->BadStatusPos.fOffsetZ);
    ApplyMatrix(ofs, *(FMATRIX*)((BYTE*)&Crd(other->BadStatusPos.iBoneIndex) + 0x90), ofs);
    FVECTOR pos = *(FVECTOR*)((BYTE*)&Crd(other->BadStatusPos.iBoneIndex) + 0xc0);
    pos.y += Math3D::PointToPointDistance(ofs, pos);
    effect->Crd(0).m_Local.v[3] = pos;
    effect->Crd(0).m_Flag = 1;
}
void CEntity::setDamageNumber(const FVECTOR& inPos, INT inDamage) {
    if (!DebugWork::g_dw.sys.m_iDagameNumber) return;
    if (!m_pDamageString) return;
    IVECTOR ivec;
    _loadRotTransPersMatrix(CWorldEnv::getWorldScreenMatrix());
    RotTransPersClipW(ivec, inPos);
    if (ivec.w) return;
    sprintf(m_szNumberString, "%d", inDamage);
    m_pDamageString->setPosition((ivec.x >> 4) - 0x800, (ivec.y >> 4) - 0x800, 0xFFFF28);
    m_pDamageString->setText(m_szNumberString);
    m_tqDamageDispTimer = 6000;
}
BOOL CEntity::startTargetChase(CEntity* inTarget, FLOAT inSpeed, FLOAT inRate) {
    if (m_uiEntityFlag & 0x78) return FALSE;
    if (!inTarget) return FALSE;
    m_fSpeed = inSpeed;
    m_uiEntityFlag |= 4;
    m_fRate = inRate;
    m_hTarget = inTarget->getHandle();
    return TRUE;
}
void CEntity::endTargetChase() {
    m_uiEntityFlag = (INT)m_uiEntityFlag & ~4;
    clearSpeed();
}
BOOL CEntity::startTargetEvade(CEntity* inTarget, FLOAT inSpeed, FLOAT inRate) {
    if (m_uiEntityFlag & 0x74) return FALSE;
    if (!inTarget) return FALSE;
    m_fSpeed = inSpeed;
    m_uiEntityFlag |= 8;
    m_fRate = inRate;
    m_hTarget = inTarget->getHandle();
    return TRUE;
}
void CEntity::endTargetEvade() {
    m_uiEntityFlag = (INT)m_uiEntityFlag & ~8;
    clearSpeed();
}
BOOL CEntity::startTargetIntercept(CEntity* inTarget, FLOAT inSpeed, FLOAT inRate) {
    if (m_uiEntityFlag & 0x6c) return FALSE;
    if (!inTarget) return FALSE;
    m_fSpeed = inSpeed;
    m_uiEntityFlag |= 0x10;
    m_fRate = inRate;
    m_hTarget = inTarget->getHandle();
    return TRUE;
}
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
BOOL CEntity::arriveTarget(const FVECTOR& inTarget, FLOAT inRadius) {
    FLOAT r2 = inRadius * inRadius;
    if (calcDistanceSq(inTarget) <= r2) return TRUE;
    FVECTOR p = getOldPos();
    FVECTOR d;
    SubVector(d, getPos(), p);
    Normalize(d, d);
    FVECTOR m;
    SubVector(m, p, inTarget);
    FLOAT b = InnerProduct(m, d);
    FLOAT c = InnerProduct(m, m) - r2;
    if (c > 0.0f && b > 0.0f) return FALSE;
    FLOAT discr = b * b - c;
    if (discr < 0.0f) return FALSE;
    FLOAT t = -b - _sqrtf(discr);
    if (t < 0.0f) t = 0.0f;
    FVECTOR q;
    ScaleVector(d, d, t);
    AddVector(q, p, d);
    const FVECTOR& p0 = getOldPos();
    const FVECTOR& p1 = getPos();
    if ((p0.x <= q.x && q.x <= p1.x && p0.y <= q.y && q.y <= p1.y && p0.z <= q.z && q.z <= p1.z) || (p1.x <= q.x && q.x <= p0.x && p1.y <= q.y && q.y <= p0.y && p1.z <= q.z && q.z <= p0.z)) return TRUE;
    return FALSE;
}
BOOL CEntity::arriveTargetXZ(const FVECTOR& inTarget, FLOAT inRadius) {
    FLOAT r2 = inRadius * inRadius;
    if (calcDistanceXZSq(inTarget) <= r2) return TRUE;
    FVECTOR p = getOldPos();
    FVECTOR d;
    SubVector(d, getPos(), p);
    d.y = 0.0f;
    Normalize(d, d);
    FVECTOR m;
    SubVector(m, p, inTarget);
    m.y = 0.0f;
    FLOAT b = InnerProduct(m, d);
    FLOAT c = InnerProduct(m, m) - r2;
    if (c > 0.0f && b > 0.0f) return FALSE;
    FLOAT discr = b * b - c;
    if (discr < 0.0f) return FALSE;
    FLOAT t = -b - _sqrtf(discr);
    if (t < 0.0f) t = 0.0f;
    FVECTOR q;
    ScaleVector(d, d, t);
    AddVector(q, p, d);
    const FVECTOR& p0 = getOldPos();
    const FVECTOR& p1 = getPos();
    if ((p0.x <= q.x && q.x <= p1.x && p0.z <= q.z && q.z <= p1.z) || (p1.x <= q.x && q.x <= p0.x && p1.z <= q.z && q.z <= p0.z)) return TRUE;
    return FALSE;
}
void CEntity::rideModelSetAttachMatrix(const FMATRIX* inpMat) {
    if (!(inpMat)) AssertApp(__FILE__, 2207, "inpMat", 0);
    if (m_pRideModel) m_pRideModel->setAttachMatrix(inpMat);
}
void CEntity::rideModelSetChipLocalMatrix(INT inIndex, const FMATRIX* in_pMatrix) {
    if (!(in_pMatrix)) AssertApp(__FILE__, 2218, "in_pMatrix", 0);
    if (m_pRideModel) m_pRideModel->setChipLocalMatrix(inIndex, in_pMatrix);
}
void CEntity::rideModelEnableAllCollision(BOOL inEnable) {
    if (m_pRideModel) {
        if (inEnable) m_pRideModel->on();
        else m_pRideModel->off();
    }
}
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
HANDLE CEntity::playSEPanAndVol(INT inFile, INT inID, INT inBoneIndex, const FVECTOR* inOffset, INT inType) {
    if (((m_uiEntityFlag & 0x80000) && !inType) || ((m_uiEntityFlag & 0x100000) && inType)) {
        return playSEPan(inFile, inID, -1, inBoneIndex, inOffset, inType);
    }
    FVECTOR pos;
    if (!(0 <= inBoneIndex && inBoneIndex < getCoordNum())) AssertApp(__FILE__, 2283, "0 <= inBoneIndex && inBoneIndex < getCoordNum()", 0);
    if (inOffset) ApplyMatrix(pos, *(FMATRIX*)((BYTE*)&Crd(inBoneIndex) + 0x90), *inOffset);
    else pos = *(FVECTOR*)((BYTE*)&Crd(inBoneIndex) + 0xc0);
    INT vol = -1;
    INT pan = CSound::getPanAndVol(pos, vol);
    INT id;
    if (!inType) id = inFile * 1000 + inID;
    else id = (inFile * 1000 + inID) | 0x10000000;
    return CSound::playSE(id, vol, pan);
}
HANDLE CEntity::playSEPan(INT inFile, INT inID, INT inVol, INT inBoneIndex, const FVECTOR* inOffset, INT inType) {
    FVECTOR pos;
    if (!(0 <= inBoneIndex && inBoneIndex < getCoordNum())) AssertApp(__FILE__, 2313, "0 <= inBoneIndex && inBoneIndex < getCoordNum()", 0);
    if (inOffset) ApplyMatrix(pos, *(FMATRIX*)((BYTE*)&Crd(inBoneIndex) + 0x90), *inOffset);
    else pos = *(FVECTOR*)((BYTE*)&Crd(inBoneIndex) + 0xc0);
    INT pan = CSound::getPan(pos);
    INT id;
    if (!inType) id = inFile * 1000 + inID;
    else id = (inFile * 1000 + inID) | 0x10000000;
    return CSound::playSE(id, inVol, pan);
}
void CEntity::stopSE(HANDLE inHandle, INT inFile, INT inID, INT inFlag) {
    INT nSeq;
    if (!inFlag) nSeq = inFile * 1000 + inID;
    else nSeq = (inFile * 1000 + inID) | 0x10000000;
    CSound::stopSE(inHandle, nSeq);
}
void CEntity::fadeoutSE(HANDLE inHandle, INT inFile, INT inID, INT inTime, INT inFlag) {
    INT nSeq;
    if (!inFlag) nSeq = inFile * 1000 + inID;
    else nSeq = (inFile * 1000 + inID) | 0x10000000;
    CSound::fadeoutSE(inHandle, nSeq, inTime);
}
void CEntity::setSEPanAndVol(HANDLE inHandle, INT inFile, INT inID, INT inBoneIndex, const FVECTOR* inOffset, INT inType) {
    if (((m_uiEntityFlag & 0x80000) && !inType) || ((m_uiEntityFlag & 0x100000) && inType)) {
        setSEPan(inHandle, inFile, inID, -1, inBoneIndex, inOffset, inType);
        return;
    }
    FVECTOR pos;
    if (!(0 <= inBoneIndex && inBoneIndex < getCoordNum())) AssertApp(__FILE__, 2380, "0 <= inBoneIndex && inBoneIndex < getCoordNum()", 0);
    if (inOffset) ApplyMatrix(pos, *(FMATRIX*)((BYTE*)&Crd(inBoneIndex) + 0x90), *inOffset);
    else pos = *(FVECTOR*)((BYTE*)&Crd(inBoneIndex) + 0xc0);
    INT vol = -1;
    INT pan = CSound::getPanAndVol(pos, vol);
    INT id;
    if (!inType) id = inFile * 1000 + inID;
    else id = (inFile * 1000 + inID) | 0x10000000;
    if (pan < 0) CSound::setSEVolume(inHandle, id, 0, 1);
    else CSound::setSEPanAndVol(inHandle, id, pan, vol, 1);
}
void CEntity::setSEPan(HANDLE inHandle, INT inFile, INT inID, INT inVol, INT inBoneIndex, const FVECTOR* inOffset, INT inType) {
    FVECTOR pos;
    if (!(0 <= inBoneIndex && inBoneIndex < getCoordNum())) AssertApp(__FILE__, 2417, "0 <= inBoneIndex && inBoneIndex < getCoordNum()", 0);
    if (inOffset) ApplyMatrix(pos, *(FMATRIX*)((BYTE*)&Crd(inBoneIndex) + 0x90), *inOffset);
    else pos = *(FVECTOR*)((BYTE*)&Crd(inBoneIndex) + 0xc0);
    INT pan = CSound::getPan(pos);
    INT id;
    if (!inType) id = inFile * 1000 + inID;
    else id = (inFile * 1000 + inID) | 0x10000000;
    CSound::setSEPan(inHandle, id, pan, 1);
    if (inVol >= 0) CSound::setSEVolume(inHandle, id, inVol, 1);
}
void CEntity::playVoice(INT inFileID, INT inID, INT inPriority) {
    if (m_pSE) ((CSEMotion*)m_pSE)->playVoice(inFileID, inID, inPriority);
}
void CEntity::playSEManaged(INT inFileID, INT inID, INT inBoneIndex, const FVECTOR* inOffset) {
    SE_MANAGED_DATA* data = m_SeManagedData;
    for (INT i = 0; i < 4; i++, data++) {
        if (!data->hHandle) {
            data->hHandle = playSEPanAndVol(inFileID, inID, inBoneIndex, inOffset, 0);
            data->iFileID = inFileID;
            data->iSeID = inID;
            data->iBoneIndex = inBoneIndex;
            if (inOffset) data->vOffset = *inOffset;
            else data->vOffset.init();
            break;
        }
    }
}
FLOAT CEntity::rndArea(FLOAT inMin, FLOAT inMax) { return inMin + getRandF() * _fabsf(inMax - inMin); }
BOOL CEntity::initCollision_(INT inGroupID, INT inRsrcID) {
    COLL_PARAM_HEADER* header = (COLL_PARAM_HEADER*)CRsrcDB::getFirstData(0x24, inGroupID * 10000 + inRsrcID, FALSE);
    if (!header) return FALSE;
    if (header->addrMapping()) {
        if (header->iBodyCollParamNum > 0) {
            m_pBodyColl = new (__FILE__ "(2525)") CBodyCollision(this);
            if (!m_pBodyColl) return FALSE;
            m_pBodyColl->initalize(header->pBodyCollParam, header->iBodyCollParamNum);
        }
        if (header->iAtkCollParamNum > 0) {
            m_pAttack = new (__FILE__ "(2535)") CAttackMotion(this);
            if (!m_pAttack) return FALSE;
            m_pAttack->initialize(header->pAtkCollParam, header->iAtkCollParamNum);
        }
        m_pCollParam = header;
        return TRUE;
    }
    return FALSE;
}
BOOL CEntity::initEffect_(INT inID) {
    m_pEffect = new (__FILE__ "(2563)") CEffectMotion(this);
    if (!m_pEffect) return FALSE;
    return m_pEffect->initalize(inID);
}
BOOL CEntity::initEffectEx_(INT inID) {
    m_pEffectEx = new (__FILE__ "(2574)") CEffectMotion(this);
    if (!m_pEffectEx) return FALSE;
    return m_pEffectEx->initalize(inID);
}
BOOL CEntity::initEffectEx_(const CHAR* inRsrcName) {
    m_pEffectEx = new (__FILE__ "(2585)") CEffectMotion(this);
    if (!m_pEffectEx) return FALSE;
    return m_pEffectEx->initalize(inRsrcName);
}
BOOL CEntity::initSE_(INT inID) {
    CSEMotion* pSE = new (__FILE__ "(2596)") CSEMotion(this);
    m_pSE = pSE;
    if (!pSE) return FALSE;
    return pSE->initalize(inID);
}
BOOL CEntity::initSEEx_(INT inID) {
    CSEMotion* pSE = new (__FILE__ "(2607)") CSEMotion(this);
    m_pSEEx = pSE;
    if (!pSE) return FALSE;
    return pSE->initalize(inID);
}
BOOL CEntity::initRideModel_(INT inGroup) {
    void* ocd = CRsrcDB::getRsrcOCD(inGroup);
    if (!ocd) return FALSE;
    m_pRideModel = new (__FILE__ "(2622)") CRideModel;
    if (!m_pRideModel) return FALSE;
    m_pRideModel->init(ocd, this);
    m_pRideModel->on();
    if (DebugWork::g_dw.collision.uiFlag & 1) new (__FILE__ "(2632)") CCollRideModelDisp(this, m_pRideModel, TRUE);
    return TRUE;
}
void CEntity::updateTimer_() {
    if (m_tqTimer > 0) m_tqTimer -= getFrameRate();
}
void CEntity::updateIK_() {
    if (m_pAnim && !isDispOFF()) m_pAnim->updateIK(0, 1, getFrameRate() * 0.020000001f);
}
void CEntity::updateEffect_() {
    if (m_pEffect) m_pEffect->update();
    if (m_pEffectEx) m_pEffectEx->update();
}
void CEntity::updateSE_() {
    if (m_pSE) m_pSE->update();
    if (m_pSEEx) m_pSEEx->update();
    SE_MANAGED_DATA* data = m_SeManagedData;
    for (INT i = 0; i < 4; i++, data++) {
        if (data->hHandle) {
            INT nFile = data->iFileID;
            INT nID = data->iSeID;
            if (!CSound::isSEPlaying(data->hHandle, nFile * 1000 + nID)) data->hHandle = 0;
            else setSEPanAndVol(data->hHandle, data->iFileID, data->iSeID, data->iBoneIndex, &data->vOffset, 0);
        }
    }
}
void CEntity::updateFade_() {
    if (!isFade()) return;
    m_tqFadeTimer -= getFrameRate();
    if (m_tqFadeTimer < 0) {
        m_tqFadeTimer = 0;
        setEntityFlag(0, 0x40000000);
    }
    FLOAT t = (FLOAT)m_tqFadeTimer / (FLOAT)m_tqFadeTime;
    m_fFadeAlpha = t * m_fStartAlpha + (1.0f - t) * m_fEndAlpha;
    if (m_iFadeModel == -1) setModelAlphaScale(m_fFadeAlpha, -1);
    else {
        for (INT i = 0; i < m_iModelCount; i++) {
            if ((m_iFadeModel >> i) & 1) setModelAlphaScale(m_fFadeAlpha, i);
        }
    }
}
void CEntity::saveRideModelMatrix_() {
    if (m_pRideModel) m_pRideModel->setOldMatrix();
}
void CEntity::rideModelAdjustRiders_() {
    if (m_pRideModel) {
        FVECTOR v;
        if (!(m_pMove)) AssertApp(__FILE__, 2754, "m_pMove", 0);
        SubVector(v, *(FVECTOR*)((BYTE*)m_pMove + 0x40), *(FVECTOR*)((BYTE*)m_pMove + 0x30));
        m_pRideModel->hoseiRiders(v);
    }
}
void CEntity::chase_() {
    if (m_uiEntityFlag & 4) {
        CEntity* pTarget = (CEntity*)CTaskMan::getTask(m_hTarget);
        if (pTarget) m_pMove->chase(*pTarget, m_fRate, m_fSpeed);
        else endTargetChase();
    }
}
void CEntity::evade_() {
    if (m_uiEntityFlag & 8) {
        CEntity* pTarget = (CEntity*)CTaskMan::getTask(m_hTarget);
        if (pTarget) m_pMove->evade(*pTarget, m_fRate, m_fSpeed);
        else endTargetEvade();
    }
}
void CEntity::intercept_() {
    if (m_uiEntityFlag & 0x10) {
        CEntity* pTarget = (CEntity*)CTaskMan::getTask(m_hTarget);
        if (pTarget) m_pMove->intercept(*pTarget, m_fRate, m_fSpeed);
        else endTargetIntercept();
    }
}
void CEntity::targetMove_() {
    if (m_uiEntityFlag & 0x20) {
        m_pMove->homingMove(m_vTargetPos, m_fRate, m_fSpeed);
        if (arriveTarget(m_vTargetPos, m_fArriveRadius)) endTargetMove();
    }
}
void CEntity::targetMoveXZ_() {
    if (m_uiEntityFlag & 0x40) {
        m_pMove->homingMoveXZ(m_vTargetPos, m_fRate, m_fSpeed);
        if (arriveTargetXZ(m_vTargetPos, m_fArriveRadius)) endTargetMoveXZ();
    }
}
void CEntity::loadModelColor_() {
    if (m_pvModelColor) {
        FVECTOR* pColor = m_pvModelColor;
        INT n = m_iModelCount;
        CModel* pModel = m_pModels;
        for (INT i = 0; i < n; i++) {
            *pColor = pModel->getColorScale();
            pModel++;
            pColor++;
        }
    }
}
void CEntity::storeModelColor_() {
    if (m_pvModelColor) {
        FVECTOR* pColor = m_pvModelColor;
        INT n = m_iModelCount;
        CModel* pModel = m_pModels;
        for (INT i = 0; i < n; i++, pModel++, pColor++) {
            pModel->setColorScale(*pColor);
        }
    }
}
void CEntity::translucence_() {
    if (!m_pvModelColor) return;
    INT n = m_iModelCount;
    CModel* model;
    FLOAT alpha = 1.0f;
    if (!(m_uiEntityFlag & 0x1000)) {
        BOOL trans = FALSE;
        model = m_pModels;
        if (m_pRideModel) {
            trans = (*(INT*)((BYTE*)m_pRideModel + 0x18) != 0);
        } else {
            for (INT i = 0; i < n; i++, model++) {
                if (m_uiDispOffFlag & (1 << i)) continue;
                if (model->getBBoxFlag() & 1) {
                    trans = TRUE;
                    break;
                }
            }
        }
        FLOAT limitAlpha = 1.0f;
        if (trans) {
            CCamera* camera = CWorldEnv::getActiveCamera();
            if (camera && camera->getTypeID() == 0x8000) camera = NULL;
            if (camera) {
                FVECTOR maxVec, minVec;
                FVECTOR v0, v1;
                limitAlpha = 0.4f;
                v0.set(-1.70141183e+38f, -1.70141183e+38f, -1.70141183e+38f);
                v1.set(1.70141183e+38f, 1.70141183e+38f, 1.70141183e+38f);
                model = m_pModels;
                for (INT i = 0; i < n; i++, model++) {
                    if (model->isShadow()) continue;
                    model->getWorldBBox(maxVec, minVec);
                    Math3D::VectorMaxMinXYZ(v0, v1, maxVec, minVec);
                }
                FVECTOR camVec = *(FVECTOR*)((BYTE*)&camera->Crd(0) + 0xc0);
                if (v1.x <= camVec.x && camVec.x <= v0.x && v1.y <= camVec.y && camVec.y <= v0.y && v1.z <= camVec.z && camVec.z <= v0.z) limitAlpha = 0.0f;
            }
        }
        FLOAT fAdd = getFrameRateF() * 0.033333335f;
        if (limitAlpha < m_fNearTranslucence) {
            m_fNearTranslucence -= fAdd;
            if (m_fNearTranslucence < limitAlpha) m_fNearTranslucence = limitAlpha;
        } else {
            m_fNearTranslucence += fAdd;
            if (limitAlpha < m_fNearTranslucence) m_fNearTranslucence = limitAlpha;
        }
        alpha = m_fNearTranslucence;
    }
    FVECTOR* pSceneColor = CSceneEnv::getTypeColor((ObjType::eObjType)m_EntityType);
    if (alpha < 1.0f || pSceneColor) {
        FVECTOR* modelColor = m_pvModelColor;
        model = m_pModels;
        for (INT i = 0; i < n; i++, model++, modelColor++) {
            FVECTOR result;
            if (pSceneColor) MulVector(result, *modelColor, *pSceneColor);
            else result = *modelColor;
            if (!(m_uiCheckEyeOffFlag & (1 << i)) || !m_pRideModel) result.w *= alpha;
            model->setColorScale(result);
        }
        if ((UINT)m_iID / 10000 == 11) {
            if (m_pEffect) m_pEffect->setAlpha(alpha);
            if (m_pEffectEx) m_pEffectEx->setAlpha(alpha);
        }
    }
    if ((m_uiEntityFlag & 0x400000) && m_pModels) {
        alpha = m_pModels->getAlphaScale();
        if (m_pEffect) m_pEffect->setAlpha(alpha);
        if (m_pEffectEx) m_pEffectEx->setAlpha(alpha);
    }
}
HANDLE CEntity::setEffect(const CHAR* inName, UINT inFlag, const FVECTOR* inPos, const FVECTOR* inRot, FLOAT inScale) {
    return CEffectMan::setEffect(inName, inFlag, (ObjType::eObjType)getEntityType(), inPos, inRot, inScale);
}
HANDLE CEntity::setEffect(const EFFECT_DATA& inData, UINT inNo, const FVECTOR* inPos, const FVECTOR* inRot, FLOAT inWait) {
    return CEffectMan::setEffect(inData, inNo, (ObjType::eObjType)getEntityType(), inPos, inRot, inWait, NULL);
}
HANDLE CEntity::setEffect(const CHAR* inName, UINT inFlag, const FMATRIX* inMat, FLOAT inScale) {
    return CEffectMan::setEffect(inName, inFlag, (ObjType::eObjType)getEntityType(), inMat, inScale);
}
HANDLE CEntity::setEffect(const EFFECT_DATA& inData, UINT inNo, const FMATRIX* inMat, FLOAT inWait) {
    return CEffectMan::setEffect(inData, inNo, (ObjType::eObjType)getEntityType(), inMat, inWait, NULL);
}
HANDLE CEntity::attachEffect(const CHAR* inName, UINT inNo, const CCoord* inParent, const FVECTOR* inPos, const FVECTOR* inRot, FLOAT inWait) {
    return CEffectMan::attachEffect(inName, inNo, (ObjType::eObjType)getEntityType(), inParent, inPos, inRot, inWait);
}
HANDLE CEntity::attachEffect(const EFFECT_DATA& inData, UINT inNo, const CCoord* inParent, const FVECTOR* inPos, const FVECTOR* inRot, FLOAT inWait) {
    return CEffectMan::attachEffect(inData, inNo, (ObjType::eObjType)getEntityType(), inParent, inPos, inRot, inWait, NULL);
}
HANDLE CEntity::attachEffect(const CHAR* inName, UINT inNo, const CCoord* inParent, const FMATRIX* inMat, FLOAT inWait) {
    return CEffectMan::attachEffect(inName, inNo, (ObjType::eObjType)getEntityType(), inParent, inMat, inWait);
}
HANDLE CEntity::attachEffect(const EFFECT_DATA& inData, UINT inNo, const CCoord* inParent, const FMATRIX* inMat, FLOAT inWait) {
    return CEffectMan::attachEffect(inData, inNo, (ObjType::eObjType)getEntityType(), inParent, inMat, inWait, NULL);
}
HANDLE CEntity::attachEffect2(const CHAR* inName, UINT inNo, const CCoord* inParent1, const CCoord* inParent2, const FVECTOR* inPos1, const FVECTOR* inPos2, FLOAT inWait) {
    return CEffectMan::attachEffect2(inName, inNo, (ObjType::eObjType)getEntityType(), inParent1, inParent2, inPos1, inPos2, inWait);
}
HANDLE CEntity::attachEffect2(const EFFECT_DATA& inData, UINT inNo, const CCoord* inParent1, const CCoord* inParent2, const FVECTOR* inPos1, const FVECTOR* inPos2, FLOAT inWait) {
    return CEffectMan::attachEffect2(inData, inNo, (ObjType::eObjType)getEntityType(), inParent1, inParent2, inPos1, inPos2, inWait, NULL);
}
HANDLE CEntity::attachEffect2(const CHAR* inName, UINT inNo, const CCoord* inParent1, const CCoord* inParent2, const FMATRIX* inMat1, const FMATRIX* inMat2, FLOAT inWait) {
    return CEffectMan::attachEffect2(inName, inNo, (ObjType::eObjType)getEntityType(), inParent1, inParent2, inMat1, inMat2, inWait);
}
HANDLE CEntity::attachEffect2(const EFFECT_DATA& inData, UINT inNo, const CCoord* inParent1, const CCoord* inParent2, const FMATRIX* inMat1, const FMATRIX* inMat2, FLOAT inWait) {
    return CEffectMan::attachEffect2(inData, inNo, (ObjType::eObjType)getEntityType(), inParent1, inParent2, inMat1, inMat2, inWait, NULL);
}
HANDLE CEntity::attachEffect3(const CHAR* inName, UINT inNo, const CCoord* inParent1, const CCoord* inParent2, const CCoord* inParent3, const FVECTOR* inPos1, const FVECTOR* inPos2, const FVECTOR* inPos3, FLOAT inWait) {
    return CEffectMan::attachEffect3(inName, inNo, (ObjType::eObjType)getEntityType(), inParent1, inParent2, inParent3, inPos1, inPos2, inPos3, inWait);
}
HANDLE CEntity::attachEffect3(const EFFECT_DATA& inData, UINT inNo, const CCoord* inParent1, const CCoord* inParent2, const CCoord* inParent3, const FVECTOR* inPos1, const FVECTOR* inPos2, const FVECTOR* inPos3, FLOAT inWait) {
    return CEffectMan::attachEffect3(inData, inNo, (ObjType::eObjType)getEntityType(), inParent1, inParent2, inParent3, inPos1, inPos2, inPos3, inWait, NULL);
}
HANDLE CEntity::attachEffect3(const CHAR* inName, UINT inNo, const CCoord* inParent1, const CCoord* inParent2, const CCoord* inParent3, const FMATRIX* inMat1, const FMATRIX* inMat2, const FMATRIX* inMat3, FLOAT inWait) {
    return CEffectMan::attachEffect3(inName, inNo, (ObjType::eObjType)getEntityType(), inParent1, inParent2, inParent3, inMat1, inMat2, inMat3, inWait);
}
HANDLE CEntity::attachEffect3(const EFFECT_DATA& inData, UINT inNo, const CCoord* inParent1, const CCoord* inParent2, const CCoord* inParent3, const FMATRIX* inMat1, const FMATRIX* inMat2, const FMATRIX* inMat3, FLOAT inWait) {
    return CEffectMan::attachEffect3(inData, inNo, (ObjType::eObjType)getEntityType(), inParent1, inParent2, inParent3, inMat1, inMat2, inMat3, inWait, NULL);
}
void CEntity::killLoopEffect(INT inNo) {
    if (m_pEffect) m_pEffect->killLoopEffect(inNo, 0);
    if (m_pEffectEx) m_pEffectEx->killLoopEffect(inNo, 0);
}
HANDLE CEntity::setAppearEffect() {
    COLL_PARAM_HEADER* coll = m_pCollParam;
    if (!coll) return 0;
    OTHER_PARAM* other = coll->pOtherParam;
    if (!(other)) AssertApp(__FILE__, 3183, "other", 0);
    if (strcmp(other->szAppearEffectDpx, "")) {
        FVECTOR pos;
        pos.set(other->AppearPos.fOffsetX, other->AppearPos.fOffsetY, other->AppearPos.fOffsetZ);
        ApplyMatrix(pos, *(FMATRIX*)((BYTE*)&Crd(other->AppearPos.iBoneIndex) + 0x90), pos);
        return setEffect(other->szAppearEffectDpx, other->iAppearEffectNo, &pos, NULL, 0.0f);
    }
    return 0;
}
HANDLE CEntity::setDisappearEffect() {
    COLL_PARAM_HEADER* coll = m_pCollParam;
    if (!coll) return 0;
    OTHER_PARAM* other = coll->pOtherParam;
    if (!(other)) AssertApp(__FILE__, 3210, "other", 0);
    if (strcmp(other->szAppearEffectDpx, "")) {
        FVECTOR pos;
        pos.set(other->AppearPos.fOffsetX, other->AppearPos.fOffsetY, other->AppearPos.fOffsetZ);
        ApplyMatrix(pos, *(FMATRIX*)((BYTE*)&Crd(other->AppearPos.iBoneIndex) + 0x90), pos);
        return setEffect(other->szAppearEffectDpx, other->iDisappearEffectNo, &pos, NULL, 0.0f);
    }
    return 0;
}
void CEntity::setDeathEffect(const FVECTOR* inPos) {
    if (m_pCollParam) {
        OTHER_PARAM* other = m_pCollParam->pOtherParam;
        if (!(other)) AssertApp(__FILE__, 3235, "other", 0);
        if (strcmp(other->szDeathEffectDpx, "")) {
            if (other->iDeathEffectNo1 >= 0) setEffect(other->szDeathEffectDpx, other->iDeathEffectNo1, inPos, &getRot(), 0.0f);
            if (other->iDeathEffectNo2 >= 0) setEffect(other->szDeathEffectDpx, other->iDeathEffectNo2, inPos, &getRot(), 0.0f);
        }
    }
}
HANDLE CEntity::setDeathEffect() {
    COLL_PARAM_HEADER* coll = m_pCollParam;
    if (!coll) return 0;
    OTHER_PARAM* other = coll->pOtherParam;
    if (!(other)) AssertApp(__FILE__, 3257, "other", 0);
    if (!strcmp(other->szDeathEffectDpx, "")) return 0;
    if (other->iDeathEffectNo1 >= 0) {
        FVECTOR pos;
        pos.set(other->DeathPos.fOffsetX, other->DeathPos.fOffsetY, other->DeathPos.fOffsetZ);
        return attachEffect(other->szDeathEffectDpx, other->iDeathEffectNo1, &Crd(other->DeathPos.iBoneIndex), &pos, NULL, 0.0f);
    }
    return 0;
}
HANDLE CEntity::setBadStatusEffect(const CHAR* inEffectName, INT inBadStatus, FLOAT inWait) {
    COLL_PARAM_HEADER* coll = m_pCollParam;
    if (!coll) return 0;
    OTHER_PARAM* other = coll->pOtherParam;
    if (!(other)) AssertApp(__FILE__, 3290, "other", 0);
    FVECTOR pos;
    pos.set(other->BadStatusPos.fOffsetX, other->BadStatusPos.fOffsetY, other->BadStatusPos.fOffsetZ);
    ApplyMatrix(pos, *(FMATRIX*)((BYTE*)&Crd(other->BadStatusPos.iBoneIndex) + 0x90), pos);
    HANDLE handle = setEffect(inEffectName, inBadStatus, &pos, NULL, 0.0f);
    CEffect* effect = CEffectMan::getEffect(handle);
    if (effect) effect->setScale(other->fBadStatusScale, other->fBadStatusScale, other->fBadStatusScale);
    return handle;
}
HANDLE CEntity::setRecoverEffect(const CHAR* inName, INT inNo) {
    if (!m_pCollParam) return 0;
    BYTE* pOther = *(BYTE**)((BYTE*)m_pCollParam + 0x18);
    if (!(pOther)) AssertApp(__FILE__, 3328, "other", 0);
    FVECTOR v;
    v.set(*(FLOAT*)(pOther + 0x34), *(FLOAT*)(pOther + 0x38), *(FLOAT*)(pOther + 0x3c));
    return attachEffect(inName, 0, &Crd(*(INT*)(pOther + 0x30)), &v, NULL, 0.0f);
}
// SOURCE DATA: .rodata
