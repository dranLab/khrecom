
#define COLLUTIL_H_PATH "c:/KHCOM/MainApp/Game/Collision/CollUtil.h" /* MATCH: this unit names CollUtil.h so (header default "Game/Collision/CollUtil.h") */
#define BODYCOLLISION_H_PATH "c:/KHCOM/MainApp/Game/BodyCollision.h" /* MATCH: this unit names BodyCollision.h so (header default "Game/BodyCollision.h") */
#include "System/HeapMan.h"
#include "Game/Collision/CollUtil.h"
#include "Game/Collision/CollBody.h"
#include "Game/BodyCollision.h"
#include "Game/Entity.h"
#include "System/Graphic/Camera.h"

void ApplyMatrix(FVECTOR& v0, const FMATRIX& m0, const FVECTOR& v1);
void SubVectorXYZ(FVECTOR& v0, const FVECTOR& v1, const FVECTOR& v2);
void AddVectorXYZ(FVECTOR& v0, const FVECTOR& v1, const FVECTOR& v2);
void ScaleVectorXYZ(FVECTOR& v0, const FVECTOR& v1, FLOAT r);



#define ASSERT(e) KH_DEBUG(if (!(e)) AssertApp(__FILE__, __LINE__, #e, 0))

CBodyCollision::CBodyCollision(CEntity* inOwner) {
    m_pOwner = inOwner;
    m_fRadiusScale = 1.0f;
    m_iHitIndex = -1;
    m_uiFlag = 0;
    m_pBodyCollParam = 0;
    m_iCollBodyCount = 0;
    m_pCollBody = 0;
    m_pHit = 0;
    for (INT i = 0; i < 2; i++) {
        m_uiSysBodyFlag[i] = 0;
        m_uiUserBodyFlag[i] = 0;
    }
}

CBodyCollision::~CBodyCollision() {
    cleanup_();
}

#line 49
BOOL CBodyCollision::initalize(BODY_COLL_PARAM* inBcp, INT inCount) {
    static COLL_TYPE g_CollTypeTbl[3] = { COLL_TYPE_SPHERE, COLL_TYPE_COLUMN, COLL_TYPE_COLUMNR };
    COLL_KIND coll_kind;
    ASSERT(inBcp);
    ASSERT(inCount > 0);
    BODY_COLL_PARAM* bcp;
    CCollBody* body;
    INT index;
    cleanup_();
    m_pBodyCollParam = inBcp;
    m_iCollBodyCount = inCount;
    m_pCollBody = new ("c:/KHCOM/MainApp/Game/BodyCollision.cpp(60)") CCollBody[inCount];
    if (!m_pCollBody) {
        return FALSE;
    }
    switch ((INT)m_pOwner->getEntityType()) {
    case 1:
    case 17:
        coll_kind = COLL_KIND_PLAYER;
        break;
    case 2:
    case 3:
    default:
        coll_kind = COLL_KIND_ENEMY;
        break;
    }
    bcp = m_pBodyCollParam;
    body = m_pCollBody;
    for (index = 0; index < inCount; index++) {
        for (INT j = 0; j < 2; j++) {
            INT idx = bcp->BoneOffset[index].iBoneIndex;
            if (idx < 0) {
                idx = m_pOwner->getBoneIndex(bcp->BoneOffset[index].szBoneName, 0);
            }
            bcp->BoneOffset[index].iBoneIndex = (idx > -1) ? idx : 0;
        }
        body->init(coll_kind, g_CollTypeTbl[bcp->iCollType], m_pOwner->getHandle(), 0);
        bcp++;
        body++;
    }
    bcp = m_pBodyCollParam;
    body = m_pCollBody;
    /* the shift's 1 is loaded once before the loop, apart from onBody()'s (a separate register) */
    INT i = 0;
    if (i < inCount) {
    UINT one = 1;
    do {
        if (bcp->uiFlag & 1) {
            body->on();
            setSysBodyFlag(0, one << i, 0);
            setUserBodyFlag(0, one << i, 0);
        } else {
            body->off();
        }
        if (bcp->uiFlag & 2) {
            body->onBody();
            setSysBodyFlag(1, one << i, 0);
            setUserBodyFlag(1, one << i, 0);
        } else {
            body->offBody();
        }
        bcp++;
        body++;
    } while (++i < inCount);
    }
    return TRUE;
}

void CBodyCollision::setGuard(INT inAttr) {
    CCollBody* body = m_pCollBody;
    INT count = m_iCollBodyCount;
    for (INT i = 0; i < count; i++) {
        body->setGuard(2, inAttr);
        body++;
    }
}

void CBodyCollision::setGuardDir(FLOAT inDir) {
    CCollBody* body = m_pCollBody;
    INT count = m_iCollBodyCount;
    for (INT i = 0; i < count; i++) {
        body->setGuardDir(inDir);
        body++;
    }
}

void CBodyCollision::setMuteki(INT inAttr) {
    CCollBody* body = m_pCollBody;
    INT count = m_iCollBodyCount;
    for (INT i = 0; i < count; i++) {
        body->setMuteki(1, inAttr);
        body++;
    }
}

void CBodyCollision::setMutekiDir(FLOAT inDir) {
    CCollBody* body = m_pCollBody;
    INT count = m_iCollBodyCount;
    for (INT i = 0; i < count; i++) {
        body->setMutekiDir(inDir);
        body++;
    }
}

#line 179
void CBodyCollision::setHp(INT inHp, INT inIndex) {
    if (inIndex == -1) {
        CCollBody* body = m_pCollBody;
        INT count = m_iCollBodyCount;
        for (INT i = 0; i < count; i++) {
            body->setHp(inHp);
            body++;
        }
    } else {
        ASSERT(0 <= inIndex && inIndex < m_iCollBodyCount);
        m_pCollBody[inIndex].setHp(inHp);
    }
}

#line 199
void CBodyCollision::setMaxHp(INT inHp, INT inIndex) {
    if (inIndex == -1) {
        CCollBody* body = m_pCollBody;
        INT count = m_iCollBodyCount;
        for (INT i = 0; i < count; i++) {
            body->setMaxHp(inHp);
            body++;
        }
    } else {
        ASSERT(0 <= inIndex && inIndex < m_iCollBodyCount);
        m_pCollBody[inIndex].setMaxHp(inHp);
    }
}

#line 216
FLOAT CBodyCollision::getRadius() {
    ASSERT(m_pCollBody);
    return m_pCollBody->getRadius();
}

#line 223
FLOAT CBodyCollision::getLargeRadius() {
    FLOAT radius;
    ASSERT(m_pCollBody);
    CCollBody* body = m_pCollBody;
    INT count = m_iCollBodyCount;
    radius = 0.0f;
    for (INT i = 0; i < count; i++) {
        FLOAT r = body->getRadius();
        if (radius < r) {
            radius = r;
        }
        body++;
    }
    return radius;
}

#line 244
INT CBodyCollision::getHp(INT inIndex) const {
    ASSERT(0 <= inIndex && inIndex < m_iCollBodyCount);
    return m_pCollBody[inIndex].getHp();
}

#line 252
INT CBodyCollision::getMaxHp(INT inIndex) const {
    ASSERT(0 <= inIndex && inIndex < m_iCollBodyCount);
    return m_pCollBody[inIndex].getMaxHp();
}

void CBodyCollision::setRadiusScale(FLOAT inValue) {
    BODY_COLL_PARAM* bcp = m_pBodyCollParam;
    CCollBody* body = m_pCollBody;
    INT count = m_iCollBodyCount;
    for (INT i = 0; i < count; i++, bcp++, body++) {
        body->setRadius(bcp->fRadius * inValue);
        if (bcp->iCollType == 0) {
            body->setShiftY(0.0f);
        }
    }
    m_fRadiusScale = inValue;
}

#line 279
CCollBody* CBodyCollision::getCollBody(INT inIndex) {
    ASSERT(0 <= inIndex && inIndex < m_iCollBodyCount);
    return &m_pCollBody[inIndex];
}

void CBodyCollision::update() {
    updateOffset_();
}

void CBodyCollision::detect() {
    CCollBody* body;
    INT count;
    setHitCollBodyIndex(-1);
    setFlag(0, 0x1C);
    m_iLastDamage = 0;
    body = m_pCollBody;
    count = m_iCollBodyCount;
    for (INT i = 0; i < count; i++) {
        if (body->isGuardTrg()) {
            setFlag(0x10, 0);
            body->setGuardTrg(0);
            setHitCollBody(body);
            setHitCollBodyIndex(i);
            break;
        }
        if (body->isMutekiTrg()) {
            setFlag(8, 0);
            body->setMutekiTrg(0);
            setHitCollBody(body);
            setHitCollBodyIndex(i);
            break;
        }
        if (body->isHit()) {
            BODY_COLL_PARAM* bcp = &m_pBodyCollParam[i];
            INT damage = body->getDamage();
            if (bcp->uiFlag & 0x10) {
                switch (body->getDamageKind()) {
                case 8:
                case 0x10:
                case 0x14:
                case 0x17:
                    break;
                default:
                    damage = (INT)(damage * bcp->fCoefficient);
                    body->setDamage(damage);
                    break;
                }
            }
            m_iLastDamage = damage;
            setFlag(4, 0);
            setHitCollBody(body);
            setHitCollBodyIndex(i);
            break;
        }
        body++;
    }
    if (isHit() || isGuard() || isHitMuteki()) {
        UINT id = getHitCollBody()->getAttackID();
        body = m_pCollBody;
        for (INT i = 0; i < count; i++) {
            body->isHit();
            body->setAttackID(id);
            body->setGuardTrg(0);
            body->setMutekiTrg(0);
            body++;
        }
    }
}

#line 394
void CBodyCollision::on(INT inIndex) {
    if (inIndex < 0) {
        BODY_COLL_PARAM* bcp = m_pBodyCollParam;
        CCollBody* body = m_pCollBody;
        INT count = m_iCollBodyCount;
        for (INT i = 0; i < count; i++) {
            if (bcp->uiFlag & 1) {
                setSysBodyFlag(0, 1 << i, 0);
                if (isOn(i)) {
                    body->on();
                }
            }
            bcp++;
            body++;
        }
    } else {
        ASSERT(0 <= inIndex && inIndex < m_iCollBodyCount);
        if (m_pBodyCollParam[inIndex].uiFlag & 1) {
            setSysBodyFlag(0, 0, 1 << inIndex);
            if (isOn(inIndex)) {
                m_pCollBody[inIndex].on();
            }
        }
    }
}

#line 428
void CBodyCollision::off(INT inIndex) {
    if (inIndex < 0) {
        CCollBody* body = m_pCollBody;
        INT count = m_iCollBodyCount;
        for (INT i = 0; i < count; i++) {
            setSysBodyFlag(0, 0, 1 << i);
            body->off();
            body++;
        }
    } else {
        ASSERT(0 <= inIndex && inIndex < m_iCollBodyCount);
        setSysBodyFlag(0, 0, 1 << inIndex);
        m_pCollBody[inIndex].off();
    }
}

#line 451
void CBodyCollision::onBody(INT inIndex) {
    if (inIndex < 0) {
        BODY_COLL_PARAM* bcp = m_pBodyCollParam;
        CCollBody* body = m_pCollBody;
        INT count = m_iCollBodyCount;
        for (INT i = 0; i < count; i++) {
            if (bcp->uiFlag & 2) {
                setSysBodyFlag(1, 1 << i, 0);
                if (isBodyOn(i)) {
                    body->onBody();
                }
            }
            bcp++;
            body++;
        }
    } else {
        ASSERT(0 <= inIndex && inIndex < m_iCollBodyCount);
        if (m_pBodyCollParam[inIndex].uiFlag & 2) {
            setSysBodyFlag(1, 1 << inIndex, 0);
            if (isBodyOn(inIndex)) {
                m_pCollBody[inIndex].onBody();
            }
        }
    }
}

#line 485
void CBodyCollision::offBody(INT inIndex) {
    if (inIndex < 0) {
        CCollBody* body = m_pCollBody;
        INT count = m_iCollBodyCount;
        for (INT i = 0; i < count; i++) {
            setSysBodyFlag(1, 0, 1 << i);
            body->offBody();
            body++;
        }
    } else {
        ASSERT(0 <= inIndex && inIndex < m_iCollBodyCount);
        setSysBodyFlag(1, 0, 1 << inIndex);
        m_pCollBody[inIndex].offBody();
    }
}

#line 508
void CBodyCollision::userOn(INT inIndex) {
    if (inIndex < 0) {
        BODY_COLL_PARAM* bcp = m_pBodyCollParam;
        CCollBody* body = m_pCollBody;
        INT count = m_iCollBodyCount;
        for (INT i = 0; i < count; i++) {
            if (bcp->uiFlag & 1) {
                setUserBodyFlag(0, 1 << i, 0);
                if (isOn(i)) {
                    body->on();
                }
            }
            bcp++;
            body++;
        }
    } else {
        ASSERT(0 <= inIndex && inIndex < m_iCollBodyCount);
        if (m_pBodyCollParam[inIndex].uiFlag & 1) {
            setUserBodyFlag(0, 1 << inIndex, 0);
            if (isOn(inIndex)) {
                m_pCollBody[inIndex].on();
            }
        }
    }
}

#line 542
void CBodyCollision::userOff(INT inIndex) {
    if (inIndex < 0) {
        CCollBody* body = m_pCollBody;
        INT count = m_iCollBodyCount;
        for (INT i = 0; i < count; i++) {
            setUserBodyFlag(0, 0, 1 << i);
            body->off();
            body++;
        }
    } else {
        ASSERT(0 <= inIndex && inIndex < m_iCollBodyCount);
        setUserBodyFlag(0, 0, 1 << inIndex);
        m_pCollBody[inIndex].off();
    }
}

#line 565
void CBodyCollision::userOnBody(INT inIndex) {
    if (inIndex < 0) {
        BODY_COLL_PARAM* bcp = m_pBodyCollParam;
        CCollBody* body = m_pCollBody;
        INT count = m_iCollBodyCount;
        for (INT i = 0; i < count; i++) {
            if (bcp->uiFlag & 2) {
                setUserBodyFlag(1, 1 << i, 0);
                if (isBodyOn(i)) {
                    body->onBody();
                }
            }
            bcp++;
            body++;
        }
    } else {
        ASSERT(0 <= inIndex && inIndex < m_iCollBodyCount);
        if (m_pBodyCollParam[inIndex].uiFlag & 2) {
            setUserBodyFlag(1, 1 << inIndex, 0);
            if (isBodyOn(inIndex)) {
                m_pCollBody[inIndex].onBody();
            }
        }
    }
}

#line 599
void CBodyCollision::userOffBody(INT inIndex) {
    if (inIndex < 0) {
        CCollBody* body = m_pCollBody;
        INT count = m_iCollBodyCount;
        for (INT i = 0; i < count; i++) {
            setUserBodyFlag(1, 0, 1 << i);
            body->offBody();
            body++;
        }
    } else {
        ASSERT(0 <= inIndex && inIndex < m_iCollBodyCount);
        setUserBodyFlag(1, 0, 1 << inIndex);
        m_pCollBody[inIndex].offBody();
    }
}

BOOL CBodyCollision::collideBody(FVECTOR* outPos, FVECTOR* outNormal, FVECTOR* outDetectPos) {
    INT n;
    CCollBody* body;
    BODY_COLL_PARAM* bcp;
    INT targetType;
    FVECTOR diff;
    if (m_pBodyCollParam) {
        n = m_iCollBodyCount;
        bcp = m_pBodyCollParam;
        body = m_pCollBody;
        for (INT i = 0; i < n; i++, body++, bcp++) {
            if ((bcp->uiFlag & 0xC) == 0xC) {
                targetType = 2;
            } else if (bcp->uiFlag & 4) {
                targetType = 1;
            } else if (bcp->uiFlag & 8) {
                targetType = 0;
            } else {
                continue;
            }
            if (body->collideBody(targetType) & 1) {
                switch (body->getType()) {
                case COLL_TYPE_SPHERE:
                    diff = m_pOwner->Crd(bcp->BoneOffset[0].iBoneIndex).getWorldMatrix().getTransRef();
                    break;
                case COLL_TYPE_COLUMN:
                case COLL_TYPE_COLUMNR:
                    AddVectorXYZ(diff, m_pOwner->Crd(bcp->BoneOffset[0].iBoneIndex).getWorldMatrix().getTransRef(),
                                 m_pOwner->Crd(bcp->BoneOffset[1].iBoneIndex).getWorldMatrix().getTransRef());
                    ScaleVectorXYZ(diff, diff, 0.5f);
                    break;
                }
                if (outPos) {
                    SubVectorXYZ(diff, *body->getOutPos(), diff);
                    AddVectorXYZ(*outPos, m_pOwner->Crd(0).getWorldMatrix().getTransRef(), diff);
                }
                if (outNormal) {
                    *outNormal = *body->getDetectNormal();
                }
                if (outDetectPos) {
                    SubVectorXYZ(diff, *body->getDetectPos(), diff);
                    AddVectorXYZ(*outDetectPos, m_pOwner->Crd(0).getWorldMatrix().getTransRef(), diff);
                }
                return TRUE;
            }
        }
    }
    return FALSE;
}

BOOL CBodyCollision::collideBody(INT inTargetType, FVECTOR* outPos, FVECTOR* outNormal, FVECTOR* outDetectPos) {
    return collideBody_(outPos, outNormal, outDetectPos, inTargetType);
}

void CBodyCollision::cleanup_() {
    delete[] m_pCollBody;
    m_pCollBody = 0;
    m_iCollBodyCount = 0;
    m_pBodyCollParam = 0;
}

#line 748
void CBodyCollision::updateOffset_() {
    BODY_COLL_PARAM* bcp;
    CCollBody* body;
    INT count = m_iCollBodyCount;
    bcp = m_pBodyCollParam;
    body = m_pCollBody;
    for (INT i = 0; i < count; i++, bcp++, body++) {
        switch (bcp->iCollType) {
        case 0: {
            FVECTOR ofs;
            BONE_OFFSET& offset = bcp->BoneOffset[0];
            ASSERT(offset.iBoneIndex < m_pOwner->getCoordNum());
            ofs.set(offset.fOffsetX, offset.fOffsetY, offset.fOffsetZ);
            ApplyMatrix(ofs, m_pOwner->Crd(offset.iBoneIndex).getWorldMatrix(), ofs);
            body->setEndPos(ofs);
            body->setStartPos(ofs);
            body->setRadius(bcp->fRadius * m_fRadiusScale);
            body->setShiftY(0.0f);
            break;
        }
        case 1: {
            FVECTOR ofs[2];
            INT iBoneIndex = bcp->BoneOffset[0].iBoneIndex;
#line 774
            ASSERT(iBoneIndex < m_pOwner->getCoordNum());
            ofs[0].set(bcp->BoneOffset[0].fOffsetX, 0.0f, bcp->BoneOffset[0].fOffsetZ);
            ApplyMatrix(ofs[0], m_pOwner->Crd(iBoneIndex).getWorldMatrix(), ofs[0]);
            ofs[0].y += bcp->BoneOffset[0].fOffsetY;
            ofs[1].set(0.0f, bcp->BoneOffset[1].fOffsetY - bcp->BoneOffset[0].fOffsetY, 0.0f);
            AddVectorXYZ(ofs[1], ofs[1], ofs[0]);
            body->setEndPos(ofs[0]);
            body->setStartPos(ofs[1]);
            body->setRadius(bcp->fRadius * m_fRadiusScale);
            break;
        }
        case 2: {
            FVECTOR ofs[2];
            for (INT j = 0; j < 2; j++) {
                BONE_OFFSET& offset = bcp->BoneOffset[j];
#line 800
                ASSERT(offset.iBoneIndex < m_pOwner->getCoordNum());
                ofs[j].set(offset.fOffsetX, offset.fOffsetY, offset.fOffsetZ);
                ApplyMatrix(ofs[j], m_pOwner->Crd(offset.iBoneIndex).getWorldMatrix(), ofs[j]);
            }
            body->setEndPos(ofs[0]);
            body->setStartPos(ofs[1]);
            body->setRadius(bcp->fRadius * m_fRadiusScale);
            break;
        }
        }
    }
}

BOOL CBodyCollision::collideBody_(FVECTOR* outPos, FVECTOR* outNormal, FVECTOR* outDetectPos, INT inTargetType) {
    INT n;
    CCollBody* body;
    BODY_COLL_PARAM* bcp;
    FVECTOR diff;
    if (m_pBodyCollParam) {
        n = m_iCollBodyCount;
        bcp = m_pBodyCollParam;
        body = m_pCollBody;
        for (INT i = 0; i < n; i++, body++, bcp++) {
            if (body->collideBody(inTargetType) & 1) {
                BONE_OFFSET& offset = bcp->BoneOffset[0];
#line 829
                ASSERT(offset.iBoneIndex < m_pOwner->getCoordNum());
                diff.set(offset.fOffsetX, offset.fOffsetY, offset.fOffsetZ);
                ApplyMatrix(diff, m_pOwner->Crd(offset.iBoneIndex).getWorldMatrix(), diff);
                if (outPos) {
                    SubVectorXYZ(diff, *body->getOutPos(), diff);
                    AddVectorXYZ(*outPos, m_pOwner->Crd(0).getWorldMatrix().getTransRef(), diff);
                }
                if (outNormal) {
                    *outNormal = *body->getDetectNormal();
                }
                if (outDetectPos) {
                    SubVectorXYZ(diff, *body->getDetectPos(), diff);
                    AddVectorXYZ(*outDetectPos, m_pOwner->Crd(0).getWorldMatrix().getTransRef(), diff);
                }
                return TRUE;
            }
        }
    }
    return FALSE;
}
// SOURCE DATA: .data .rodata
