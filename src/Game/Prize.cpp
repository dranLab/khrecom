#define COLLUTIL_H_PATH "c:/KHCOM/MainApp/Game/Collision/CollUtil.h" /* MATCH: this unit names CollUtil.h so (header default "Game/Collision/CollUtil.h") */
#define GPRIM_H_PATH "System/Graphic/GPrim.h" /* MATCH: this unit names Gprim.h so (header default "System/Graphic/Gprim.h") */
#define CARD_H_PATH "c:/KHCOM/MainApp/Game/Card.h" /* MATCH: this unit names Card.h so (header default "Game/Card.h") */
#define LINKEDBUF_H_PATH "System/LinkedBuf.h" /* MATCH: this unit names LinkedBuf.h by the relative path (LinkedBuf.h switches __FILE__ only when it is defined) */
#include "System/TaskMan.h"
#include "System/Task.h"
#include "Game/Collision/CollUtil.h"
#include "System/Graphic/Effect/effect.h"
#include "System/Graphic/Anim.h"
#include "System/Graphic/Gprim.h"
#include "System/Graphic/2d/2DMan.h"
#include "Game/CardBattle.h"
#include "System/Graphic/Str.h"

/* MATCH: Placeholder: an empty string follows Str.h's strings in .rodata (as in GimmickWallHole); which header it comes
 * from is not known. */
inline void _emptyStringPlaceholder() {
    TRACE("");
}

#include "System/Graphic/modeldata.h"
#include "System/Graphic/Model.h"
#include "Game/Card.h"
#include "System/HeapMan.h"
#include "Game/Prize.h"
#include "System/Graphic/LightUnit.h"
#include "System/LinkedBuf.h"
#include "Game/Movement.h"
#include "Game/Collision/CollMap.h"
#include "Game/GameWork.h"
#include "Game/SceneInfo.h"
#include "Game/Player/Player.h"
#include "System/Graphic/Effect/EffectMan.h"
#include "System/Graphic/Camera.h"
#include "System/Math3D.h"
#include "System/Sound/SoundMan.h"
#include "System/Rnd.h"
#include <string.h>
#include "include_asm.h"



#define ASSERT(e) KH_DEBUG(if (!(e)) AssertApp(__FILE__, __LINE__, #e, 0))

typedef int MODELTAG[4];

#include "System/RsrcDB.h"





extern "C" {
float sinf(float x);
float cosf(float x);
}
FVECTOR getPlayerPos(BOOL flag);
void ScaleVector(FVECTOR& v0, const FVECTOR& v1, FLOAT f);
void ScaleVectorXYZ(FVECTOR& v0, const FVECTOR& v1, FLOAT f);
void SubVector(FVECTOR& v0, const FVECTOR& v1, const FVECTOR& v2);
void SubVectorXYZ(FVECTOR& v0, const FVECTOR& v1, const FVECTOR& v2);
void AddVectorXYZ(FVECTOR& v0, const FVECTOR& v1, const FVECTOR& v2);
void Normalize(FVECTOR& v0, const FVECTOR& v1);
void ApplyMatrix(FVECTOR& v0, const FMATRIX& m0, const FVECTOR& v1);
void ApplyMatrix33(FVECTOR& v0, const FMATRIX& m0, const FVECTOR& v1);

#line 295

struct PRIZE_DATA {
    CHAR* pEffectName;
    INT iEffectNo;
    FLOAT fRadius;
};

namespace Prize {
static const PRIZE_DATA g_PrizeDataTbl[13] = {
    { "rec_prise", 8, 0.25f },  { "rec_prise", 7, 0.25f },  { "rec_prise", 0, 0.35f },  { "rec_prise", 10, 0.35f },
    { "rec_prise", 9, 0.35f },  { "rec_prise", 1, 0.425f }, { "rec_prise", 2, 0.25f },  { "rec_prise", 11, 0.25f },
    { "rec_prise", 3, 0.3f },   { "rec_prise", 12, 0.25f }, { "rec_prise", 4, 0.3f },   { "rec_prise", 5, 0.25f },
    { "rec_prise", 6, 0.25f },
};
static const int g_iCardModelIndex[14] = { 0, 0, 1, 2, 3, 4, 4, 5, 0, 6, 7, 8, 9, 10 };
static const G_ST g_CardST[4] = { { 0.f, 0.f }, { 0.25f, 0.f }, { 0.f, 0.25f }, { 0.25f, 0.25f } };
} // namespace Prize
/* the original's (local) table is referenced by another object's data, as splat symbolized it */
__asm__(".globl _5Prize$g_PrizeDataTbl");

static inline void _setNewState(CState*& p, CState& NewState) { p = &NewState; }














CPrize::CPrize(CTask* inParent)
    : CEntity(inParent, (ObjType::eObjType)6), m_hEffectHandle(0), m_bHoming(FALSE), m_bSECall(FALSE),
      m_bReturnCard(FALSE), m_bFlyHigh(FALSE), m_iBoundCounter(0), m_fPrizeRate(0.075f), m_pCardModel(NULL) {}

CPrize::~CPrize() {
    CEffect* effect = CEffectMan::getEffect(m_hEffectHandle);
    if (effect) effect->kill();
    if (m_pCardModel) {
        if (m_pCardModel->m_pData) DelayedDelete((MODELTAG*)m_pCardModel->m_pData, TRUE);
        else if (m_pCardModel->m_pSemiData) DelayedDelete((MODELTAG*)m_pCardModel->m_pSemiData, TRUE);
        DelayedDelete(m_pCardModel, FALSE);
    }
}

INT CPrize::getTypeID() const { return 0x1008; }

BOOL CPrize::setup(const PRIZE_PARAM& inParam) {
    m_PrizeKind = inParam.PrizeKind;
    m_iNumber = inParam.iNumber;
    m_fSpeed = inParam.fSpeed;
#line 157
    m_pMove = new KH_AT("(157)") CMovement(this);
    if (!m_pMove) return FALSE;
    addCoord(1);
    BOOL ret;
    if (inParam.PrizeKind >= 0xCB) {
        ret = initPrize_();
    } else {
        ret = initCard_();
    }
    if (!ret) return FALSE;
    if (inParam.uiPrizeFlag & 1) {
        warp(inParam.vPos);
    } else if (CSceneInfo::isFieldScene()) {
        CCollMap* coll = m_pMove->getMapCollision();
#line 199
        ASSERT(coll);
        FLOAT radius = coll->getRadius();
        coll->setRadius(0.0f);
        FVECTOR start = getPlayerPos(FALSE);
        FVECTOR end = inParam.vPos;
        FVECTOR pos = inParam.vPos;
        FVECTOR normal;
        start.y = end.y;
        coll->setStartPos(start);
        coll->setEndPos(end);
        if (coll->collideRide(FALSE)) {
            pos = *coll->getOutPos();
            coll->setEndPos(pos);
        }
        if (coll->collideMap()) pos = *coll->getOutPos();
        setOldPos(start);
        setPos(pos);
        FLOAT r = radius + 0.1f;
        m_pMove->setFlag(0x40, 0);
        m_pMove->setFootRadius(r * 0.5f);
        m_pMove->setWallRadius(r);
        if (inParam.PrizeKind >= 0xCB) {
            m_pMove->setShiftY(0.0f);
        } else {
            m_pMove->setShiftY(-0.2f);
        }
        m_pMove->update();
        m_pMove->setFlag(0, 0x40);
        m_pMove->setCollisionFlag(0, 0xF);
        m_pMove->setRadius(radius);
        if (inParam.PrizeKind >= 0xCB) {
            m_pMove->setShiftY(0.0f);
        } else {
            m_pMove->setShiftY(-0.2f);
        }
        warp(getPos());
        setOldPos(start);
        setPos(getPos());
        TRACE("\x8f" "C\x90\xb3\x91O = (%.5f, %.5f, %.5f), \x8f" "C\x90\xb3\x8c\xe3 = (%.5f, %.5f, %.5f)\n", inParam.vPos.x,
              inParam.vPos.y, inParam.vPos.z, getPos().x, getPos().y, getPos().z);
    } else {
        findPosition_(inParam);
    }
    setStartPos(inParam.vPos);
    setRot(inParam.vRot);
    updateCoord();
    coordFix();
    m_pMove->enableGravity(TRUE);
    return TRUE;
}

void CPrize::onOut() {
    if (m_pModels && (isFriendCard_() || isGimmickCard_())) {
        CGameWork::setFlag(1, 0x22, FALSE);
    }
}

void CPrize::move(const FVECTOR& inPos) {
    CCollMap* coll = m_pMove->getMapCollision();
#line 402
    ASSERT(coll);
    FVECTOR start = getPlayerPos(TRUE);
    FVECTOR end = inPos;
    coll->setStartPos(start);
    coll->setEndPos(end);
    if (coll->collideRide(FALSE)) end = *coll->getOutPos();
    coll->setEndPos(end);
    if (coll->collideMap()) end = *coll->getOutPos();
    warp(end);
}

void CPrize::enableMarluxia3Mode(BOOL inSw) {
    if (inSw) {
        for (INT i = 0; i < m_iModelCount; i++) {
            m_pModels[i].m_nFlag &= ~0x20;
            m_pModels[i].m_nFlag |= 0x1000;
        }
        m_State[1].set(this, &CPrize::beginCardMainSpecial_, &CPrize::cardMainSpecial_, &CPrize::endCardMainSpecial_);
        gotoState(m_State[1]);
    }
}

void CPrize::forceGetPrize() {
    playerPrizeGet_();
    out();
}

void CPrize::flyHigh(BOOL bAll) {
    if (m_PrizeKind >= 0xCB) return;
    if (m_bFlyHigh) return;
    FVECTOR rot;
    rot.set((-45.0f - getRandF() * 15.0f) * 0.017453292f, getRandF() * 360.0f * 0.017453292f, 0.0f, 1.0f);
    Math3D::LimitPIVector(rot);
    FVECTOR v;
    v.set(0.0f, 0.0f, 1.0f, 1.0f);
    FMATRIX m;
    m.init();
    RotMatrixX(m, m, rot.x);
    RotMatrixY(m, m, rot.y);
    RotMatrixZ(m, m, rot.z);
    ApplyMatrix33(v, m, v);
    if (m_PrizeKind != 0xA0 || bAll) {
        setupFade(1.0f, 0.0f, 3000, -1);
        ScaleVectorXYZ(v, v, 0.3f);
        m_pMove->setVel(v);
        m_iStep = 2;
        m_bFlyHigh = TRUE;
    } else {
        ScaleVectorXYZ(v, v, 0.3f);
        m_pMove->setVel(v);
    }
}

void CPrize::beginPrizeMain_() {
    setTimer(4500);
    m_pMove->calcVelocity(m_fSpeed);
    if (!CGameWork::getFlag(1, 0x21)) {
        m_iStep = 0;
    } else {
        m_iStep = 3;
    }
}

void CPrize::prizeMain_() {
    switch (m_iStep) {
    case 0:
        if (m_bHoming) {
            if (getTimer() <= 0) {
                m_iStep = 2;
                setTimer(1500);
                break;
            }
        } else if (getTimer() <= 0) {
            m_bHoming = TRUE;
            setTimer(30000);
        }
        if (m_pMove->isGround()) {
            FVECTOR v;
            v.set(0.0f, 1.0f, 0.0f, 1.0f);
            ScaleVector(v, v, m_pMove->getSpeed() * 0.7f);
            m_pMove->setVel(v);
            if (m_pMove->getSpeed() <= 0.05f) clearSpeed();
            m_iBoundCounter++;
            if (m_iBoundCounter < 3) playBoundSE_();
        } else if (m_pMove->isWall()) {
            FVECTOR v = m_pMove->getVel();
            FVECTOR n = m_pMove->getDetectNormal();
            Normalize(n, n);
            Math3D::CalcReflectionVector(v, m_pMove->getVel(), n);
            m_pMove->setVel(v);
        }
        if (isDraw_()) {
            ((FVECTOR&)getRot()).x = 0.0f;
            ((FVECTOR&)getRot()).y = g_pPlayer->calcDirection(this);
            m_pMove->enableGravity(FALSE);
            m_pMove->setFlag(1, 0);
            m_iStep = 1;
        }
        break;
    case 1:
        if (!g_pPlayer) {
            out();
            break;
        }
        {
            FVECTOR target;
            g_pPlayer->getLockOnPos(target);
            if (m_bSECall) {
                CEffect* effect = CEffectMan::getEffect(m_hEffectHandle);
                if (!effect) {
                    out();
                    break;
                }
                if (getTimer() <= 0) {
                    playerPrizeGet_();
                    out();
                    break;
                }
                FLOAT s = (FLOAT)getTimer() / 3000.0f;
                effect->scale().x = s;
                effect->scale().y = s;
                effect->scale().z = s;
                FVECTOR v;
                FMATRIX m;
                vu0UnitMatrix(m);
                RotMatrixY(m, m, 0.5934119f);
                ApplyMatrix(m_vGetVel, m, m_vGetVel);
                ScaleVectorXYZ(v, m_vGetVel, s);
                m_pMove->setVel(v);
                warp(target);
            } else {
                m_fSpeed = 0.3f;
                m_pMove->homingMove(target, 0.25f, m_fSpeed);
            }
            if (calcDistanceSq(target) <= 4.0f && !m_bSECall) {
                FVECTOR v;
                SubVector(v, getPos(), target);
                Normalize(v, v);
                FVECTOR v1 = v;
                v1.y = 0.0f;
                if (Math3D::VectorLength(v1) < v.y * 0.25f) v.set(0.0f, 1.0f, 0.0f, 1.0f);
                ScaleVector(v, v, 0.75f);
                m_pMove->setVel(v);
                m_vGetVel = v;
                warp(target);
                setTimer(3000);
                playGetSE_();
                m_bSECall = TRUE;
            }
        }
        break;
    case 2:
        if (getTimer() > 0) {
            CEffect* effect = CEffectMan::getEffect(m_hEffectHandle);
            if (effect) effect->color().w = (FLOAT)getTimer() / 1500.0f;
            break;
        }
        out();
        break;
    case 3:
        if (m_pMove->isGround()) {
            FVECTOR v;
            v.set(0.0f, 1.0f, 0.0f, 1.0f);
            ScaleVector(v, v, m_pMove->getSpeed() * 0.7f);
            m_pMove->setVel(v);
            if (m_pMove->getSpeed() <= 0.05f) clearSpeed();
            m_iBoundCounter++;
            if (m_iBoundCounter < 3) playBoundSE_();
        } else if (m_pMove->isWall()) {
            FVECTOR v = m_pMove->getVel();
            FVECTOR n = m_pMove->getDetectNormal();
            Normalize(n, n);
            Math3D::CalcReflectionVector(v, m_pMove->getVel(), n);
            m_pMove->setVel(v);
        }
        if (!CGameWork::getFlag(1, 0x21)) {
            ((FVECTOR&)getRot()).x = 0.0f;
            ((FVECTOR&)getRot()).y = g_pPlayer->calcDirection(this);
            m_pMove->enableGravity(FALSE);
            m_iStep = 1;
        }
        break;
    }
}

void CPrize::endPrizeMain_() {}
void CPrize::beginCardMain_() {
    m_pMove->calcVelocity(m_fSpeed);
    FVECTOR rot;
    rot.set(0.0f, 0.0f, 0.5235988f, 1.0f);
    setRot(rot);
    m_iStep = 0;
    m_pMove->setMass(0.5f);
}

void CPrize::cardMain_() {
    switch (m_iStep) {
    case 0:
        if (30000 - getTimer() >= 4500) m_bHoming = TRUE;
        cardRotation_();
        if (m_pMove->isGround() || m_pMove->isWall()) {
            FVECTOR v;
            FVECTOR n = m_pMove->getDetectNormal();
            Normalize(n, n);
            FVECTOR vel = m_pMove->getVel();
            Normalize(vel, vel);
            Math3D::CalcReflectionVector(v, vel, m_pMove->getDetectNormal());
            Normalize(v, v);
            ScaleVector(v, v, 0.125f);
            m_pMove->setVel(v);
        }
        if (isDraw_()) {
            m_pMove->setFlag(1, 0);
            m_iStep = 1;
            break;
        }
        if (CGameWork::getFlag(1, 0x23)) break;
        if ((isFriendCard_() || isGimmickCard_()) && getTimer() <= 0) {
            setupFade(1.0f, 0.0f, 1500, -1);
            m_iStep = 2;
        }
        break;
    case 1:
        if (!g_pPlayer) {
            out();
            break;
        }
        {
            FVECTOR target;
            g_pPlayer->getLockOnPos(target);
            if (m_bSECall) {
                if (getTimer() <= 0) {
                    playerPrizeGet_();
                    out();
                    break;
                }
                FLOAT s = (FLOAT)getTimer() / 3000.0f;
                FVECTOR scale = getScale();
                scale.x = s;
                scale.y = s;
                scale.z = s;
                setScale(scale);
                FVECTOR v;
                FMATRIX m;
                m.init();
                RotMatrixY(m, m, 0.5934119f);
                ApplyMatrix(m_vGetVel, m, m_vGetVel);
                ScaleVectorXYZ(v, m_vGetVel, s);
                m_pMove->setVel(v);
                warp(target);
            } else {
                m_fSpeed = 0.3f;
                FVECTOR v;
                SubVectorXYZ(v, target, getPos());
                Normalize(v, v);
                ScaleVectorXYZ(v, v, 0.3f);
                m_pMove->setVel(v);
            }
            if (calcDistanceSq(target) <= 4.0f && !m_bSECall) {
                FVECTOR v;
                SubVector(v, getPos(), target);
                Normalize(v, v);
                FVECTOR v1 = v;
                v1.y = 0.0f;
                if (Math3D::VectorLength(v1) < v.y * 0.25f) v.set(0.0f, 1.0f, 0.0f, 1.0f);
                ScaleVector(v, v, 0.75f);
                m_pMove->setVel(v);
                m_vGetVel = v;
                warp(target);
                setTimer(3000);
                playGetSE_();
                m_bSECall = TRUE;
            }
        }
        break;
    case 2:
        cardRotation_();
        if (m_pMove->isGround() || m_pMove->isWall()) {
            FVECTOR v;
            FVECTOR n = m_pMove->getDetectNormal();
            Normalize(n, n);
            Math3D::CalcReflectionVector(v, m_pMove->getVel(), m_pMove->getDetectNormal());
            Normalize(v, v);
            ScaleVector(v, v, 0.125f);
            m_pMove->setVel(v);
            m_bHoming = TRUE;
        }
        if (!isFade()) out();
        break;
    }
}

void CPrize::endCardMain_() {}

void CPrize::beginCardMainSpecial_() {
    m_pMove->calcVelocity(m_fSpeed);
    FVECTOR rot;
    rot.set(0.0f, 0.0f, 0.5235988f, 1.0f);
    setRot(rot);
    m_iStep = 0;
    m_pMove->setMass(0.5f);
}
void CPrize::cardMainSpecial_() {
    switch (m_iStep) {
    case 0: {
        cardRotation_();
        if (m_pMove->isGround()) {
            FVECTOR v;
            v.set(0.0f, 1.0f, 0.0f, 1.0f);
            ScaleVector(v, v, m_pMove->getSpeed() * 0.5f);
            m_pMove->setVel(v);
            if (m_pMove->getSpeed() <= 0.05f) clearSpeed();
            m_bHoming = TRUE;
        } else if (m_pMove->isWall()) {
            FVECTOR v = m_pMove->getVel();
            FVECTOR n = m_pMove->getDetectNormal();
            Normalize(n, n);
            Math3D::CalcReflectionVector(v, m_pMove->getVel(), n);
            m_pMove->setVel(v);
        }
        FLOAT dist;
        if (((CardBattle::CCardBattle*)CTaskMan::getSysTask(22))->getEnchantKind() == 9) {
            dist = g_pPlayer->getCardDrawRadius();
            dist *= dist;
        } else {
            dist = 2.25f;
        }
        if (m_bHoming && calcDistanceSq(g_pPlayer) <= dist) {
            m_pMove->setFlag(1, 0);
            m_iStep = 1;
        }
        break;
    }
    case 1:
        if (!g_pPlayer) {
            out();
            break;
        }
        {
            FVECTOR target;
            g_pPlayer->getLockOnPos(target);
            if (m_bSECall) {
                if (getTimer() <= 0) {
                    playerPrizeGet_();
                    out();
                    break;
                }
                FLOAT s = (FLOAT)getTimer() / 3000.0f;
                FVECTOR scale = getScale();
                scale.x = s;
                scale.y = s;
                scale.z = s;
                setScale(scale);
                FVECTOR v;
                FMATRIX m;
                m.init();
                RotMatrixY(m, m, 0.5934119f);
                ApplyMatrix(m_vGetVel, m, m_vGetVel);
                ScaleVectorXYZ(v, m_vGetVel, s);
                m_pMove->setVel(v);
                warp(target);
            } else {
                m_fSpeed = 0.3f;
                FVECTOR v;
                SubVectorXYZ(v, target, getPos());
                Normalize(v, v);
                ScaleVectorXYZ(v, v, 0.3f);
                m_pMove->setVel(v);
            }
            if (calcDistanceSq(target) <= 4.0f && !m_bSECall) {
                FVECTOR v;
                SubVector(v, getPos(), target);
                Normalize(v, v);
                FVECTOR v1 = v;
                v1.y = 0.0f;
                if (Math3D::VectorLength(v1) < v.y * 0.25f) v.set(0.0f, 1.0f, 0.0f, 1.0f);
                ScaleVector(v, v, 0.75f);
                m_pMove->setVel(v);
                m_vGetVel = v;
                warp(target);
                setTimer(3000);
                playGetSE_();
                m_bSECall = TRUE;
            }
        }
        break;
    case 2: {
        CEffect* effect = CEffectMan::getEffect(m_hEffectHandle);
        if (effect) effect->color().w = m_pModels->getAlphaScale();
        if (!isFade()) {
            CEffectMan::killEffect(m_hEffectHandle, 0.0f);
            m_hEffectHandle = 0;
            m_pMove->enableGravity(FALSE);
            m_pMove->setFlag(9, 0);
            m_iStep = 3;
        }
        break;
    }
    }
}

void CPrize::endCardMainSpecial_() {}

void CPrize::flashing_() {
    if (getFlag() & 0x800) {
        CEffect* effect = CEffectMan::getEffect(m_hEffectHandle);
        if (effect) {
            if (getFlashingCounter()) {
                effect->setDrawing(FALSE);
            } else {
                effect->setDrawing(TRUE);
            }
        }
    }
}

#line 825

void CPrize::playerPrizeGet_() {
    if (!m_bReturnCard) {
        CPlayer::notifyGetPrize(m_PrizeKind, m_iNumber, 1);
        return;
    }
    CARD card;
    card.init(m_PrizeKind, m_iNumber);
#line 1113
    ASSERT(g_pPlayer);
    g_pPlayer->notifyReturnCard(&card);
}
BOOL CPrize::initPrize_() {
    const PRIZE_DATA* data = &Prize::g_PrizeDataTbl[m_PrizeKind - 0xCB];
    m_pMove->setRadius(data->fRadius);
    m_pMove->setShiftY(0.0f);
    FVECTOR pos;
    pos.init();
    m_hEffectHandle = attachEffect(data->pEffectName, data->iEffectNo, &Crd(0), &pos, NULL, 0.0f);
    CEffect* effect = CEffectMan::getEffect(m_hEffectHandle);
    if (effect) effect->setAttachNoRot(TRUE);
    m_State[0].set(this, &CPrize::beginPrizeMain_, &CPrize::prizeMain_, &CPrize::endPrizeMain_);
    gotoState(m_State[0]);
    return TRUE;
}
BOOL CPrize::initCard_() {
    if (isFriendCard_()) goto flag;
    if (isGimmickCard_()) {
        CardBattle::CCardBattle* cb;
        cb = (CardBattle::CCardBattle*)CTaskMan::getSysTask(22);
#line 1169
        ASSERT(cb);
        if (cb->getGimmickCount() > 0) return FALSE;
    flag:
        if (CGameWork::getFlag(1, 0x22)) return FALSE;
        CGameWork::setFlag(1, 0x22, TRUE);
    } else if (m_PrizeKind == 0xA0) {
        return initReloadCard_();
    }
    INT groupID = 0xA028;
    INT count = CRsrcDB::getModelCount(groupID);
    if (count <= 0) return FALSE;
    CARD card;
    card.init(m_PrizeKind, m_iNumber);
    INT mdlIndex;
    if (m_PrizeKind != 0xB9) mdlIndex = Prize::g_iCardModelIndex[card.m_nCategory21];
    else mdlIndex = 10;
#line 1203
    ASSERT(0 <= mdlIndex && mdlIndex < count);
    FVECTOR texST;
    char texName[16];
    CCard::getCardTexInfo(card, texName, texST);
    G_ST st[4];
    st[0].set(texST.x, texST.y);

    st[1].s = texST.z;
    st[1].t = texST.y;
    st[2].s = texST.x;
    st[2].t = texST.w;
    st[3].s = texST.z;
    st[3].t = texST.w;
    MODELDATA* orgMdl = CRsrcDB::getRsrcMDL(groupID)[mdlIndex];
#line 1215
    ASSERT(orgMdl);
#line 1217
    MODELDATA* d = m_pCardModel = new KH_AT("(1217)") MODELDATA;
    if (!d) return FALSE;
    ((u_long128*)d)[0] = ((u_long128*)orgMdl)[0];
    ((u_long128*)d)[1] = ((u_long128*)orgMdl)[1];
    m_pCardModel->m_pData = NULL;
    m_pCardModel->m_pSemiData = NULL;
    INT dataSize1 = 0;
    INT dataSize2 = 0;
    if (orgMdl->m_pData) {
        MODELTAG* end = (MODELTAG*)(orgMdl->m_pSemiData ? orgMdl->m_pSemiData : orgMdl->m_pBBox);
        dataSize1 = (INT)end - (INT)orgMdl->m_pData;
        dataSize1 >>= 4;
    }
    if (orgMdl->m_pSemiData) {
        dataSize2 = (INT)orgMdl->m_pBBox - (INT)orgMdl->m_pSemiData;
        dataSize2 >>= 4;
    }
#line 1244
    MODELTAG* newMdl = new KH_AT("(1244)") MODELTAG[dataSize1 + dataSize2];
    if (!newMdl) return FALSE;
    memcpy(newMdl, orgMdl->m_pData, (dataSize1 + dataSize2) * sizeof(MODELTAG));
    if (orgMdl->m_pData) m_pCardModel->m_pData = newMdl;
    if (orgMdl->m_pSemiData) m_pCardModel->m_pSemiData = newMdl + dataSize1;
    MODELTAG* tag = (MODELTAG*)m_pCardModel->m_pData;
    if (tag) {
        INT index = 0;
        while (!(tag[index][0] == 0x60000000 && !tag[index][1] && !tag[index][2] && !tag[index][3])) {
            index++;
            INT num = (USHORT)tag[index][1];
            INT mode = (USHORT)tag[index][3];
            index += 2;
            for (INT i = 0; i < num; i++) {
                index += 2;
                if (MODELDATA::isNormal(mode)) index++;
                if (MODELDATA::isColorI(mode) || MODELDATA::isColorF(mode)) index++;
                if (MODELDATA::isTexture(mode)) {
                    FLOAT* s = (FLOAT*)&tag[index][0];
                    FLOAT* t = (FLOAT*)&tag[index][1];
                    for (INT j = 0; j < 4; j++) {
                        if (Prize::g_CardST[j].s == *s && Prize::g_CardST[j].t == *t) {
                            G_ST& r = st[j];
                            *s = r.s;
                            *t = r.t;
                            break;
                        }
                    }
                    index++;
                }
            }
            do { index++; } while (0); /* a loop construct (macro?): its loop note aligns the test label below */
        }
    }
    m_iModelCount = 1;
#line 1320
    m_pModels = new KH_AT("(1320)") CModel[m_iModelCount];
    if (!m_pModels) return FALSE;
#line 1324
    m_pvModelColor = new KH_AT("(1324)") FVECTOR[m_iModelCount];
    if (!m_pvModelColor) return FALSE;
    if (m_pCardModel->enableLight()) {
#line 1331
        m_pNLight = new KH_AT("(1331)") CLightUnit;
        if (!m_pNLight) return FALSE;
    }
    if (m_pCardModel->enablePointLight()) {
#line 1338
        m_pGLight = new KH_AT("(1338)") CGimmickLightUnit;
        if (!m_pGLight) return FALSE;
    }
    m_pModels->link(this, m_pCardModel);
    if (m_pCardModel->enablePointLight()) {
        m_pModels->setLightUnit(m_pGLight);
    } else if (m_pCardModel->enableLight()) {
        m_pModels->setLightUnit(m_pNLight);
    }
    m_pModels->changeTexture("dummy", texName);
    m_pvModelColor[0] = m_pModels->getColorScale();
    if (m_pNLight) m_pNLight->setAnchor(&Crd(0), 2);
    if (m_pGLight) m_pGLight->setAnchor(&Crd(0), 2);
    attachCardEffect_();
    m_iID = groupID;
    setEntityFlag(0x2000, 0);
    setTimer(30000);
    m_pMove->setRadius(0.4f);
    m_pMove->setShiftY(-0.2f);
    m_State[0].set(this, &CPrize::beginCardMain_, &CPrize::cardMain_, &CPrize::endCardMain_);
    gotoState(m_State[0]);
    return TRUE;
}
BOOL CPrize::initReloadCard_() {
    INT groupID = 0xA029;
    MODELDATA** pModelTbl;
    INT i;

    m_iModelCount = CRsrcDB::getModelCount(groupID);
    if (m_iModelCount <= 0) return FALSE;
    pModelTbl = CRsrcDB::getRsrcMDL(groupID);
    if (!pModelTbl) return FALSE;
#line 1402
    m_pModels = new KH_AT("(1402)") CModel[m_iModelCount];
    if (!m_pModels) return FALSE;
#line 1406
    m_pvModelColor = new KH_AT("(1406)") FVECTOR[m_iModelCount];
    if (!m_pvModelColor) return FALSE;
    for (i = 0; i < m_iModelCount; i++) {
        if (pModelTbl[i]->enableLight()) {
#line 1415
            m_pNLight = new KH_AT("(1415)") CLightUnit;
            if (!m_pNLight) return FALSE;
            break;
        }
    }
    for (i = 0; i < m_iModelCount; i++) {
        if (pModelTbl[i]->enablePointLight()) {
#line 1426
            m_pGLight = new KH_AT("(1426)") CGimmickLightUnit;
            if (!m_pGLight) return FALSE;
            break;
        }
    }
    for (i = 0; i < m_iModelCount; i++) {
        m_pModels[i].link(this, pModelTbl[i]);
        m_pModels[i].linkTexAnm(m_pTexAnm, m_nTexAnmNum);
        if (pModelTbl[i]->enablePointLight()) {
            m_pModels[i].setLightUnit(m_pGLight);
        } else if (pModelTbl[i]->enableLight()) {
            m_pModels[i].setLightUnit(m_pNLight);
        }
        m_pvModelColor[i] = m_pModels[i].getColorScale();
    }
    if (m_pNLight) m_pNLight->setAnchor(&Crd(0), 2);
    if (m_pGLight) m_pGLight->setAnchor(&Crd(0), 2);
    attachCardEffect_();
    m_iID = groupID;
    setEntityFlag(0x2000, 0);
    setTimer(30000);
    m_pMove->setRadius(0.4f);
    m_pMove->setShiftY(-0.2f);
    m_State[0].set(this, &CPrize::beginCardMain_, &CPrize::cardMain_, &CPrize::endCardMain_);
    gotoState(m_State[0]);
    return TRUE;
}
void CPrize::attachCardEffect_() {
    FVECTOR pos;
    pos.init();
    m_hEffectHandle = attachEffect("rec_card_work", 0, &Crd(0), &pos, NULL, 0.0f);
    CEffect* effect = CEffectMan::getEffect(m_hEffectHandle);
    if (effect) effect->setScale(5.0f, 5.0f, 5.0f);
}

void CPrize::playBoundSE_() {
    switch (m_PrizeKind) {
    case 0xCB:
    case 0xCC:
    case 0xCD:
    case 0xCE:
    case 0xCF:
    case 0xD0:
    case 0xD1:
        playSEPanAndVol(0, 0x2C, 0, NULL, 0);
        break;
    case 0xD2:
    case 0xD3:
        playSEPanAndVol(0, 0x2A, 0, NULL, 0);
        break;
    case 0xD4:
    case 0xD5:
        playSEPanAndVol(0, 0x2B, 0, NULL, 0);
        break;
    }
}

void CPrize::playGetSE_() {
    switch (m_PrizeKind) {
    case 0xCB:
    case 0xCC:
    case 0xCD:
    case 0xCE:
    case 0xCF:
    case 0xD0:
    case 0xD1:
        CSound::playSE(0x32, -1, 0x40);
        break;
    case 0xD2:
    case 0xD3:
        CSound::playSE(0x30, -1, 0x40);
        break;
    case 0xD4:
    case 0xD5:
        CSound::playSE(0x31, -1, 0x40);
        break;
    default:
        CSound::playSE(6, -1, 0x40);
        break;
    }
}

void CPrize::cardRotation_() {
    ((FVECTOR&)getRot()).y += getFrameRateF() * 0.06981317f;
    ((FVECTOR&)getRot()).y = Math3D::LimitPI(getRot().y);
}

BOOL CPrize::isGimmickCard_() {
    if (m_PrizeKind < 0x43) {
        if (m_PrizeKind >= 0x3A) return TRUE;
    }
    return FALSE;
}

BOOL CPrize::isFriendCard_() {
    if (m_PrizeKind < 0x3A) {
        if (m_PrizeKind >= 0x31) return TRUE;
    }
    return FALSE;
}

BOOL CPrize::isDraw_() {
    if (CGameWork::getFlag(1, 0x21)) return FALSE;
    if (getPos().y < -75.0f) return TRUE;
    if (!m_bHoming) return FALSE;
    FLOAT radius;
    if (g_pPlayer) {
        radius = g_pPlayer->getCardDrawRadius();
        radius *= radius;
    } else {
        radius = 25.0f;
    }
    if (calcDistanceSq(g_pPlayer) > radius) return FALSE;
    return TRUE;
}
void CPrize::findPosition_(const PRIZE_PARAM& inParam) {
    CCollMap* coll = m_pMove->getMapCollision();
#line 1638
    ASSERT(coll);
    FVECTOR start;
    FVECTOR end;
    FVECTOR pos = inParam.vPos;
    TRACE("================start ... (%.5f, %.5f, %.5f)\n", pos.x, pos.y, pos.z);
    FVECTOR v = getPlayerPos(FALSE);
    start = v;
    end = pos;
    start.y = end.y;
    if (Math3D::PointToPointDistanceSq(start, end) <= 25.0f) {
        FLOAT r = coll->getRadius() + 0.1f;
        SubVectorXYZ(v, start, end);
        Normalize(v, v);
        ScaleVectorXYZ(v, v, r);
        AddVectorXYZ(start, start, v);
        coll->setStartPos(start);
        coll->setEndPos(end);
        if (coll->collideRide(FALSE)) {
            pos = *coll->getOutPos();
            coll->setEndPos(pos);
        }
        if (coll->collideMap()) pos = *coll->getOutPos();
        TRACE("================step1 ... (%.5f, %.5f, %.5f)\n", pos.x, pos.y, pos.z);
    }
    start = pos;
    end = pos;
    start.y -= 2.5f;
    coll->setStartPos(start);
    coll->setEndPos(end);
    if (coll->collideRide(FALSE)) {
        pos = *coll->getOutPos();
        coll->setEndPos(pos);
    }
    if (coll->collideMap()) pos = *coll->getOutPos();
    TRACE("================step2 ... (%.5f, %.5f, %.5f)\n", pos.x, pos.y, pos.z);
    for (FLOAT deg = 0.0f; deg < 360.0f; deg += 45.0f) {
        FLOAT theta = Math3D::LimitPI(deg * 0.017453292f);
        start = pos;
        start.x += sinf(theta);
        start.z += cosf(theta);
        FLOAT d = Math3D::LimitPI(theta + 3.1415927f);
        end = pos;
        end.x += sinf(d);
        end.z += cosf(d);
        coll->setStartPos(start);
        coll->setEndPos(end);
        if (coll->collideRide(FALSE)) {
            pos = *coll->getOutPos();
            coll->setEndPos(pos);
        }
        if (coll->collideMap()) pos = *coll->getOutPos();
    }
    TRACE("================result ... (%.5f, %.5f, %.5f)\n", pos.x, pos.y, pos.z);
    warp(pos);
}

// SOURCE DATA: .rodata
