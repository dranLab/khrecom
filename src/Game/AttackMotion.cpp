
#define GPRIM_H_PATH "System/Graphic/GPrim.h" /* MATCH: this unit names Gprim.h so (header default "System/Graphic/Gprim.h") */
#define BATTLEENEMY_H_PATH "c:/KHCOM/MainApp/Game/Enemy/BattleEnemy.h" /* MATCH: this unit names BattleEnemy.h so (header default "Game/Enemy/BattleEnemy.h") */
#define COLLUTIL_H_PATH "c:/KHCOM/MainApp/Game/Collision/CollUtil.h" /* MATCH: this unit names CollUtil.h so (header default "Game/Collision/CollUtil.h") */
#include "System/Task.h"
#include "System/Graphic/Gprim.h"
#include "System/Graphic/2d/2DMan.h"
#include "Game/CardBattle.h"
#include "Game/Enemy/BattleEnemy.h"
#include "Game/Collision/CollUtil.h"
#include "Game/Collision/CollAttack.h"
#include "System/Graphic/Effect/effect.h"
#include "Game/AttackMotion.h"
#include "Game/CollParam.h"
#include "Game/Entity.h"
#include "System/Graphic/Camera.h"
#include "System/Graphic/Effect/EffectMan.h"
#include "System/Sound/SoundMan.h"
#include "System/Math3D.h"
#include "System/HeapMan.h"
#include <string.h>
#include "include_asm.h"

void ApplyMatrix(FVECTOR& v0, const FMATRIX& m0, const FVECTOR& v1);



#define ASSERT(e) KH_DEBUG(if (!(e)) AssertApp(__FILE__, __LINE__, #e, 0))

namespace AttackMotion {
COLL_TYPE g_CollType[5] = {COLL_TYPE_POINT, COLL_TYPE_SPHERE, COLL_TYPE_POLY3, COLL_TYPE_CAPSULE, COLL_TYPE_CIRCLE};
}

CAttackMotionCtrl::CAttackMotionCtrl() {
    m_iStep = 3;
    m_vEnemyPos.init();
    m_uiFlag = 0;
    m_pNowAtkCollParam = NULL;
    m_pCollAttack = NULL;
}

CAttackMotionCtrl::~CAttackMotionCtrl() {
    if (m_pCollAttack) {
        delete m_pCollAttack;
    }
    m_pCollAttack = NULL;
}

#line 59
BOOL CAttackMotionCtrl::initialize(CEntity* inOwner, COLL_KIND inCollKind) {
    ASSERT(inOwner);
#line 62
    m_pCollAttack = new KH_AT("(62)") CCollAttack;
    if (!m_pCollAttack) {
        return FALSE;
    }
    m_pOwner = inOwner;
    m_hEnemyHandle = inOwner->getHandle();
    m_CollKind = inCollKind;
    return TRUE;
}

void CAttackMotionCtrl::set(const ATK_COLL_PARAM* inAtkCollParam) {
    m_iStep = 0;
    m_pNowAtkCollParam = (ATK_COLL_PARAM*)inAtkCollParam;
    m_pCollAttack->updateAttackID();
    m_pCollAttack->setAttackKind((ATK_KIND)0);
}

#line 88
void CAttackMotionCtrl::update(TIMEQ inCurrentTime) {
    ASSERT(m_pCollAttack);
    if (!m_pNowAtkCollParam) {
        return;
    }
    switch (m_iStep) {
    case 0:
        if (inCurrentTime >= m_pNowAtkCollParam->tqStartTime) {
            FVECTOR v;
            v.init();
            m_pCollAttack->init((COLL_KIND)m_CollKind, AttackMotion::g_CollType[m_pNowAtkCollParam->iCollType],
                                m_hEnemyHandle, 0);
            m_pCollAttack->setStartPos(v);
            m_pCollAttack->setEndPos(v);
            m_pCollAttack->on();
            if (!m_pCollAttack->getAttackKind()) {
                m_pCollAttack->setAttackKind(m_pNowAtkCollParam->getAtkKind());
            }
            m_pCollAttack->setRadius(m_pNowAtkCollParam->fRadius);
            if (!m_CollKind) {
                m_pCollAttack->setHitEffectParam(m_pNowAtkCollParam->szHitEffectDpx, m_pNowAtkCollParam->iHitEffectNo,
                                                 (ObjType::eObjType)m_pOwner->getEntityType());
            }
            m_pCollAttack->updateAttackID();
            if (m_pNowAtkCollParam->tqEndTime < 0) {
                m_iStep = 2;
            } else {
                m_iStep = 1;
            }
        }
        break;
    case 1:
        if (inCurrentTime >= m_pNowAtkCollParam->tqEndTime) {
            if (m_pNowAtkCollParam->isNoLoop()) {
                m_iStep = 2;
            } else {
                m_iStep = 3;
            }
            m_pCollAttack->off();
        }
        break;
    case 2:
        break;
    default:
        if (m_pOwner->isMotionLoop()) {
            m_iStep = 0;
            m_pCollAttack->off();
        }
        break;
    }
    computeOffset_();
    if (isEnemyPos_()) {
        m_pCollAttack->setEnemyPos(m_vEnemyPos);
        m_uiFlag &= ~1;
    } else {
        m_pCollAttack->setEnemyPos(getParent_(m_pOwner)->Crd(0).m_World.getTransRef());
    }
}

#line 169
INT CAttackMotionCtrl::test() {
    ASSERT(m_pCollAttack);
    if (!m_pNowAtkCollParam) {
        return 0;
    }
    return m_pCollAttack->collideAttack();
}

#line 179
void CAttackMotionCtrl::release() {
    ASSERT(m_pCollAttack);
    m_iStep = 3;
    m_pCollAttack->off();
    m_pCollAttack->setAttackKind((ATK_KIND)0);
}

#line 189
void CAttackMotionCtrl::setAttackParam(INT inAttack, ATK_ATTR inAttr) {
    ASSERT(m_pCollAttack);
    m_pCollAttack->setAttack(inAttack, 0);
    m_pCollAttack->setAttackAttr(inAttr);
}

#line 198
void CAttackMotionCtrl::setAttackKind(ATK_KIND inAtkKind) {
    ASSERT(m_pCollAttack);
    m_pCollAttack->setAttackKind(inAtkKind);
}

#line 206
void CAttackMotionCtrl::setAttackID(UINT inAttackID) {
    ASSERT(m_pCollAttack);
    m_pCollAttack->setAttackID(inAttackID);
}

#line 214
void CAttackMotionCtrl::setRadius(FLOAT inRadius) {
    ASSERT(m_pCollAttack);
    m_pCollAttack->setRadius(inRadius);
}

#line 222
FVECTOR* CAttackMotionCtrl::getDetectPos() {
    ASSERT(m_pCollAttack);
    return m_pCollAttack->getDetectPos();
}

#line 230
UINT CAttackMotionCtrl::getAttackID() const {
    ASSERT(m_pCollAttack);
    return m_pCollAttack->getAttackID();
}

#line 238
ATK_KIND CAttackMotionCtrl::getAtkKind() const {
    ASSERT(m_pCollAttack);
    return m_pCollAttack->getAttackKind();
}

HANDLE CAttackMotionCtrl::setHitEffect() {
    if (strcmp(m_pNowAtkCollParam->szHitEffectDpx, "") == 0) {
        return 0;
    }
    if (!m_CollKind) {
        return 0;
    }
    FVECTOR rot = m_pOwner->getRot();
    rot.y = Math3D::LimitPI(rot.y + 3.1415927f);
    return CEffectMan::setEffect(m_pNowAtkCollParam->szHitEffectDpx, m_pNowAtkCollParam->iHitEffectNo,
                                 (ObjType::eObjType)m_pOwner->getEntityType(), m_pCollAttack->getDetectPos(), &rot,
                                 0.0f);
}

void CAttackMotionCtrl::setCollKind(INT inCollKind) {
    m_CollKind = (COLL_KIND)inCollKind;
}

HANDLE CAttackMotionCtrl::playHitSE() {
    if (m_pNowAtkCollParam->iHitSEFileID < 0) {
        return 0;
    }
    INT nID = m_pNowAtkCollParam->iHitSEFileID * 1000 + m_pNowAtkCollParam->iHitSESEID;
    return CSound::playSE(nID, *m_pCollAttack->getDetectPos(), -1);
}

void CAttackMotionCtrl::updateAttackID() {
    if (m_pCollAttack) {
        m_pCollAttack->updateAttackID();
    }
}

#line 294
void CAttackMotionCtrl::computeOffset_() {
    ASSERT(m_pCollAttack);
    switch (m_pCollAttack->getType()) {
    case COLL_TYPE_POINT:
    case COLL_TYPE_SPHERE: {
        FVECTOR vec0;
        FVECTOR vec1;
        if (!m_pCollAttack->getCollision()) {
            break;
        }
        *m_pCollAttack->getStartPos() = *m_pCollAttack->getEndPos();
        BONE_OFFSET& offset = m_pNowAtkCollParam->BoneOffset[0];
#line 309
        ASSERT(offset.iBoneIndex < m_pOwner->getCoordNum());
        vec0.set(offset.fOffsetX, offset.fOffsetY, offset.fOffsetZ);
        ApplyMatrix(vec1, m_pOwner->Crd(offset.iBoneIndex).m_World, vec0);
        m_pCollAttack->setEndPos(vec1);
        FVECTOR* start = m_pCollAttack->getStartPos();
        if (start->x == 0.0f && start->y == 0.0f && start->z == 0.0f && start->w == 1.0f) {
            m_pCollAttack->setStartPos(vec1);
        }
        break;
    }
    case COLL_TYPE_POLY3: {
        FVECTOR vec[2];
        FVECTOR tmp;
        if (!m_pCollAttack->getCollision()) {
            break;
        }
        *m_pCollAttack->getStartPos() = *m_pCollAttack->getEndPos();
        for (INT i = 0; i < 2; i++) {
            BONE_OFFSET& offset = m_pNowAtkCollParam->BoneOffset[i];
#line 333
            ASSERT(offset.iBoneIndex < m_pOwner->getCoordNum());
            tmp.set(offset.fOffsetX, offset.fOffsetY, offset.fOffsetZ);
            ApplyMatrix(vec[i], m_pOwner->Crd(offset.iBoneIndex).m_World, tmp);
        }
        m_pCollAttack->setEndPos(vec[0]);
        FVECTOR* start = m_pCollAttack->getStartPos();
        if (start->x == 0.0f && start->y == 0.0f && start->z == 0.0f && start->w == 1.0f) {
            m_pCollAttack->setStartPos(vec[0]);
        }
        m_pCollAttack->setBasePos(vec[1]);
        break;
    }
    case COLL_TYPE_CAPSULE: {
        FVECTOR vec[2];
        FVECTOR tmp;
        for (INT i = 0; i < 2; i++) {
            BONE_OFFSET& offset = m_pNowAtkCollParam->BoneOffset[i];
#line 354
            ASSERT(offset.iBoneIndex < m_pOwner->getCoordNum());
            tmp.set(offset.fOffsetX, offset.fOffsetY, offset.fOffsetZ);
            ApplyMatrix(vec[i], m_pOwner->Crd(offset.iBoneIndex).m_World, tmp);
        }
        m_pCollAttack->setStartPos(vec[0]);
        m_pCollAttack->setEndPos(vec[1]);
        break;
    }
    case COLL_TYPE_CIRCLE: {
        FVECTOR vec0;
        FVECTOR vec1;
        if (!m_pCollAttack->getCollision()) {
            break;
        }
        *m_pCollAttack->getStartPos() = *m_pCollAttack->getEndPos();
        BONE_OFFSET& offset = m_pNowAtkCollParam->BoneOffset[0];
#line 373
        ASSERT(offset.iBoneIndex < m_pOwner->getCoordNum());
        vec0.set(offset.fOffsetX, offset.fOffsetY, offset.fOffsetZ);
        ApplyMatrix(vec1, m_pOwner->Crd(offset.iBoneIndex).m_World, vec0);
        m_pCollAttack->setEndPos(vec1);
        FVECTOR* start = m_pCollAttack->getStartPos();
        if (start->x == 0.0f && start->y == 0.0f && start->z == 0.0f && start->w == 1.0f) {
            m_pCollAttack->setStartPos(vec1);
        }
        break;
    }
    }
}

CEntity* CAttackMotionCtrl::getParent_(CEntity* inChild) {
    CTask* parent = inChild->getParent();
    if (parent) {
        INT id = parent->getTypeID();
        if (id == 0x1000 || id == 0x1004) {
            return getParent_((CEntity*)parent);
        }
    }
    return inChild;
}

CAttackMotion::CAttackMotion(CEntity* inOwner) : CAttachMotion(inOwner) {
    m_iAtkMotCtrlCount = 0;
    m_iNowCtrlCount = 0;
    m_pAtkMotCtrl = NULL;
    m_iAtkIndex = 0;
    m_pAtkCollParam = NULL;
    m_uiAttackID = 0;
}

CAttackMotion::~CAttackMotion() {
    delete[] m_pAtkMotCtrl;
    m_pAtkMotCtrl = NULL;
}

#line 431
BOOL CAttackMotion::initialize(ATK_COLL_PARAM* inAtkCollParam, INT inCount) {
    ASSERT(inAtkCollParam);
    m_iAtkCollParamNum = inCount;
    m_pAtkCollParam = inAtkCollParam;
    ATK_COLL_PARAM* param = inAtkCollParam;
    for (INT i = 0; i < inCount;) {
        for (INT j = 0; j < 2; j++) {
            if (param->BoneOffset[j].iBoneIndex <= 0) {
                param->BoneOffset[j].iBoneIndex = m_pOwner->getBoneIndex(param->BoneOffset[j].szBoneName, 0);
#line 445
                ASSERT(param->BoneOffset[j].iBoneIndex != -1);
            }
        }
        if (param->iMotIndex <= 0) {
            param->iMotIndex = m_pOwner->getMotionIndex(param->szName);
        }
        i++;
        __asm__ volatile(""); /* MATCH: one more insn of life for the next param: i + 1 wins the tie and gets s5 */
        param++;
    }
    COLL_KIND coll_kind;
    switch (m_pOwner->getEntityType()) {
    case 1:
    case 0x11:
        coll_kind = COLL_KIND_PLAYER;
        break;
    case 2:
    case 3:
        coll_kind = COLL_KIND_ENEMY;
        break;
    default:
        coll_kind = COLL_KIND_ENEMY;
        break;
    }
    m_iAtkMotCtrlCount = 12;
#line 469
    m_pAtkMotCtrl = new KH_AT("(469)") CAttackMotionCtrl[12];
    if (!m_pAtkMotCtrl) {
        m_iAtkMotCtrlCount = 0;
        return FALSE;
    }
    for (INT i = 0; i < m_iAtkMotCtrlCount; i++) {
        if (!m_pAtkMotCtrl[i].initialize(m_pOwner, coll_kind)) {
            return FALSE;
        }
    }
    return TRUE;
}

BOOL CAttackMotion::set(INT inIndex) {
    if (!hasAtkCollParam()) {
        return FALSE;
    }
    INT count = 0;
    stop();
    INT n = m_iAtkCollParamNum;
    FVECTOR v;
    v.init();
    ATK_COLL_PARAM* param = m_pAtkCollParam;
    for (INT i = 0; i < n; i++, param++) {
        if (inIndex != param->iMotIndex) {
            continue;
        }
        if (!param->bEnable) {
            continue;
        }
        m_pAtkMotCtrl[count++].set(param);
        m_iAtkIndex = param->iAtkDataIndex;
        if (count >= m_iAtkMotCtrlCount) {
            break;
        }
    }
    m_iNowCtrlCount = count;
    return count > 0;
}

BOOL CAttackMotion::set(INT inIndex, INT inActNo) {
    if (!hasAtkCollParam()) {
        return FALSE;
    }
    INT count = 0;
    stop();
    INT n = m_iAtkCollParamNum;
    FVECTOR v;
    v.init();
    ATK_COLL_PARAM* param = m_pAtkCollParam;
    for (INT i = 0; i < n; i++, param++) {
        if (inIndex != param->iMotIndex) {
            continue;
        }
        if (inActNo != param->iAtkDataIndex) {
            continue;
        }
        if (!param->bEnable) {
            continue;
        }
        m_pAtkMotCtrl[count++].set(param);
        m_iAtkIndex = param->iAtkDataIndex;
        if (count >= m_iAtkMotCtrlCount) {
            break;
        }
    }
    m_iNowCtrlCount = count;
    return count > 0;
}

void CAttackMotion::setAttackParam(INT inValue, ATK_ATTR inAttr) {
    INT n = m_iNowCtrlCount;
    CAttackMotionCtrl* ctrl = m_pAtkMotCtrl;
    for (INT i = 0; i < n; i++, ctrl++) {
        ctrl->setAttackParam(inValue, inAttr);
    }
}

void CAttackMotion::setAttackParamForBattleEnemy() {
    if (m_pOwner->getTypeID() != 0x1004) {
        return;
    }
    CBattleEnemy* enemy = (CBattleEnemy*)m_pOwner;
    INT n = m_iNowCtrlCount;
    CAttackMotionCtrl* ctrl = m_pAtkMotCtrl;
    for (INT i = 0; i < n; i++, ctrl++) {
        ATK_COLL_PARAM* param = ctrl->getAtkCollParam();
#line 570
        ASSERT(param);
        ATK_KIND kind;
        INT ap = (INT)enemy->getAttackPoint(param->iAtkDataIndex, param->getAtkKind(), 0, &kind,
                                            param->isFinish() != 0);
        ctrl->setAttackParam(ap, enemy->getAttackAttr(param->iAtkDataIndex));
        ctrl->setAttackKind(kind);
    }
}

void CAttackMotion::setAttackParamForBattleEnemy(INT inActNo) {
    if (m_pOwner->getTypeID() != 0x1004) {
        return;
    }
    CBattleEnemy* enemy = (CBattleEnemy*)m_pOwner;
    INT n = m_iNowCtrlCount;
    CAttackMotionCtrl* ctrl = m_pAtkMotCtrl;
    for (INT i = 0; i < n; i++, ctrl++) {
        ATK_COLL_PARAM* param = ctrl->getAtkCollParam();
#line 596
        ASSERT(param);
        ATK_KIND kind;
        INT ap = (INT)enemy->getAttackPoint(inActNo, param->getAtkKind(), 0, &kind, param->isFinish() != 0);
        ctrl->setAttackParam(ap, enemy->getAttackAttr(inActNo));
        ctrl->setAttackKind(kind);
    }
}

void CAttackMotion::setAttackKind(ATK_KIND inAtkKind) {
    INT n = m_iNowCtrlCount;
    CAttackMotionCtrl* ctrl = m_pAtkMotCtrl;
    for (INT i = 0; i < n; i++, ctrl++) {
        ctrl->setAttackKind(inAtkKind);
    }
}

void CAttackMotion::setRadius(FLOAT inRadius) {
    INT n = m_iNowCtrlCount;
    CAttackMotionCtrl* ctrl = m_pAtkMotCtrl;
    for (INT i = 0; i < n; i++, ctrl++) {
        ctrl->setRadius(inRadius);
    }
}

void CAttackMotion::setCollKind(INT inCollKind) {
    for (INT i = 0; i < m_iAtkMotCtrlCount; i++) {
        m_pAtkMotCtrl[i].setCollKind(inCollKind);
    }
}

void CAttackMotion::setEnemyHandle(HANDLE inHandle) {
    for (INT i = 0; i < m_iAtkMotCtrlCount; i++) {
        m_pAtkMotCtrl[i].setEnemyHandle(inHandle);
    }
}

void CAttackMotion::setEnemyPos(const FVECTOR& inPos) {
    for (INT i = 0; i < m_iAtkMotCtrlCount; i++) {
        m_pAtkMotCtrl[i].setEnemyPos(inPos);
    }
}

void CAttackMotion::stop() {
    INT n = m_iNowCtrlCount;
    CAttackMotionCtrl* ctrl = m_pAtkMotCtrl;
    for (INT i = 0; i < n; i++, ctrl++) {
        ctrl->release();
    }
    m_iNowCtrlCount = 0;
}

void CAttackMotion::update() {
    TIMEQ current = m_pOwner->getMotionCurrentTime();
    INT n = m_iNowCtrlCount;
    CAttackMotionCtrl* ctrl = m_pAtkMotCtrl;
    for (INT i = 0; i < n; i++, ctrl++) {
        ctrl->update(current);
    }
}

INT CAttackMotion::test() {
    INT ret = 0;
    INT n = m_iNowCtrlCount;
    CAttackMotionCtrl* ctrl = m_pAtkMotCtrl;
    for (INT i = 0; i < n; i++, ctrl++) {
        INT result = ctrl->test();
        if (result && result != 3) {
            ctrl->setHitEffect();
        }
        ret |= result;
    }
    return ret;
}

void CAttackMotion::updateAttackID() {
    INT n = m_iAtkMotCtrlCount;
    CAttackMotionCtrl* ctrl = m_pAtkMotCtrl;
    for (INT i = 0; i < n; i++, ctrl++) {
        ctrl->updateAttackID();
    }
}

void CAttackMotion::uniformAttackID() {
    UINT id = m_pAtkMotCtrl->getAttackID();
    INT n = m_iAtkMotCtrlCount;
    CAttackMotionCtrl* ctrl = m_pAtkMotCtrl;
    for (INT i = 0; i < n; i++, ctrl++) {
        ctrl->setAttackID(id);
    }
}

#line 745
void CAttackMotion::enbaleAtkCollParam(INT inIndex, BOOL inSw) {
    ASSERT(0 <= inIndex && inIndex < m_iAtkCollParamNum);
    m_pAtkCollParam[inIndex].bEnable = inSw;
}

#line 753
void CAttackMotion::setAtkCollParamFlag(INT inIndex, UINT inSet, UINT inMask) {
    ASSERT(0 <= inIndex && inIndex < m_iAtkCollParamNum);
    m_pAtkCollParam[inIndex].uiFlag = (m_pAtkCollParam[inIndex].uiFlag | inSet) & ~inMask;
}

BOOL CAttackMotion::checkOnCollisionAtkKind(ATK_KIND inAtkKind) {
    INT n = m_iAtkMotCtrlCount;
    CAttackMotionCtrl* ctrl = m_pAtkMotCtrl;
    for (INT i = 0; i < n; i++, ctrl++) {
        if (ctrl->getAtkKind() == inAtkKind) {
            return TRUE;
        }
    }
    return FALSE;
}

// SOURCE DATA: .data .rodata
