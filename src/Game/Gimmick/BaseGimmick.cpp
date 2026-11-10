#define GAME_GIMMICK_BASEGIMMICK_CPP /* MATCH: unit gate (its header gives this unit its own parts); without it the image differs (tested 2026-11-02) */
#define GPRIM_H_PATH "System/Graphic/GPrim.h" /* MATCH: this unit names Gprim.h so (header default "System/Graphic/Gprim.h") */
#include "System/TaskMan.h"
#include "System/Task.h"
#include "Game/Collision/CollUtil.h"
#include "Game/BodyCollision.h"
#include "Game/Gimmick/BaseGimmick.h"
#include "System/Graphic/Gprim.h"
#include "System/Graphic/2d/2DMan.h"
#include "Game/CardBattle.h"
#include "System/Graphic/Str.h"
/* MATCH: Placeholders: three empty strings follow Str.h's strings in .rodata (as in GimmickBell); which headers they
 * come from is not known. */
inline void _emptyStringPlaceholder0() {
    TRACE("");
}
inline void _emptyStringPlaceholder1() { /* MATCH: placeholder: an inline function's strings, in the original .rodata order */
    TRACE("");
}
inline void _emptyStringPlaceholder2() { /* MATCH: placeholder: an inline function's strings, in the original .rodata order */
    TRACE("");
}

#include "System/Graphic/modeldata.h"
#include "System/Graphic/Effect/effect.h"
#include "System/Graphic/Model.h"
#include "System/RsrcData.h"
#include "System/Graphic/Effect/EffectMan.h"
#include "System/Graphic/Camera.h"
#include "System/Math3D.h"
#include "Game/Map.h"
#include "Game/MapUnitTable.h"
#include "Game/Movement.h"
#include "Game/AttackMotion.h"
#include "Game/Collision/CollObj.h"
#include "Game/Gimmick/GimmickManager.h"
#include "Game/SceneInfo.h"
#include "Game/Collision/CollBody.h"
#include "System/HeapMan.h"


CBaseGimmick::CBaseGimmick(CTask* inParent)
    : CEntity(inParent, ObjType::OBJ_TYPE_GIMMICK), m_uiGimmickFlag(0), m_iUniqueID(0), m_hEffectHandle(0) {
    initFSM_();
    setEntityFlag(0, 0x1000);
}

CBaseGimmick::CBaseGimmick(CTask* inParent, const ZONEPARTS& inParts)
    : CEntity(inParent, ObjType::OBJ_TYPE_ENEMY), m_uiGimmickFlag(0), m_iUniqueID(0), m_hEffectHandle(0) {
    INT id = effectIDtoI(inParts.effectId);
    if (id != -1) {
        if (!initalize(11, id)) {
            TRACE("Gimmick initalize failed ... %d\n", id);
            out();
            return;
        }
        makeCoord(inParts);
        initFSM_();
    } else {
#line 148
        m_pMove = new KH_AT("(148)") CMovement(this);
        if (!m_pMove) {
            out();
            return;
        }
        addCoord(1);
        makeCoord(inParts);
        initFSM_();
    }
}

CBaseGimmick::~CBaseGimmick() {
    CEffect* effect = CEffectMan::getEffect(m_hEffectHandle);
    if (effect) effect->kill();
}

INT CBaseGimmick::getTypeID() const {
    return 0x1005;
}

BOOL CBaseGimmick::initalize(INT inGroupID, INT inRsrcID) {
    if (!CEntity::initalize(inGroupID, inRsrcID)) return FALSE;
    initGimmickParam(inGroupID, inRsrcID);
    changeGimmickState(GM_STATE_MAIN);
    return TRUE;
}

BOOL CBaseGimmick::initalize(INT inGroupID, INT inRsrcID, UINT inFlag) {
    if (!CEntity::initalize(inGroupID, inRsrcID, inFlag)) return FALSE;
    initGimmickParam(inGroupID, inRsrcID);
    changeGimmickState(GM_STATE_MAIN);
    return TRUE;
}

void CBaseGimmick::changeMotion(INT inIndex, TIMEQ inTq, BOOL inRootMove, TIMEQ inGr, INT inEffectNo, INT inSENo) {
    CEntity::changeMotion(inIndex, inTq, inRootMove, inGr, inEffectNo, inSENo);
    if (m_pAttack) m_pAttack->set(inIndex);
}

BOOL CBaseGimmick::changeMotionIndex(INT inIndex, TIMEQ inTq, BOOL inRootMove, TIMEQ inGr, INT inEffectNo, INT inSENo) {
    if (m_pAnim && m_pAnim->getAnimID() != inIndex) {
        changeMotion(inIndex, inTq, inRootMove, inGr, inEffectNo, inSENo);
        return TRUE;
    }
    return FALSE;
}

INT CBaseGimmick::effectIDtoI(UINT inEffID) {
    MAPUNITID id;
    id.ui = inEffID;
    for (INT i = 0; i < 4; i++) {
        if (!('0' <= id.c[i] && id.c[i] <= '9')) return -1;
    }
    return (id.c[0] - '0') * 1000 + (id.c[1] - '0') * 100 + (id.c[2] - '0') * 10 + (id.c[3] - '0');
}

void CBaseGimmick::updateBefore() {
    setGimmickFlag(0, 1);
    if (m_pAttack && !(getGimmickFlag() & 0x10) && m_pAttack->test()) setGimmickFlag(1, 0);
    hitTest_();
    if (getGimmickFlag() & 8) return;
    if (!m_pBodyColl) return;
    FVECTOR result;
    if (m_pBodyColl->collideBody(&result, NULL, NULL)) {
        if (!(getGimmickFlag() & 4)) {
            FVECTOR pos = getPos();
            pos.x = result.x;
            pos.z = result.z;
            setPos(pos);
        }
        setGimmickFlag(2, 0);
    } else {
        setGimmickFlag(0, 2);
    }
}

void CBaseGimmick::updateAfter() {}

void CBaseGimmick::lastUpdate() {
    if (m_pBodyColl) m_pBodyColl->update();
    if (m_pAttack && !(getGimmickFlag() & 0x10)) m_pAttack->update();
}

static inline void _setNewState(CState*& p, CState& NewState) { p = &NewState; }
#line 328
void CBaseGimmick::changeGimmickState(INT inState) {
    if (!(0 <= inState && inState < GM_STATE_MAX)) AssertApp(__FILE__, __LINE__, "0 <= inState && inState < GM_STATE_MAX", 0);
    _setNewState(m_pNewState, m_GimmickState[inState]);
}

void CBaseGimmick::makeCoord(const ZONEPARTS& inParts) {
    FVECTOR& rot = (FVECTOR&)getRot();
    rot.x = inParts.rot.x * 0.1f * 0.017453292f;
    rot.y = inParts.rot.y * 0.1f * 0.017453292f;
    rot.z = inParts.rot.z * 0.1f * 0.017453292f;
    Math3D::LimitPIVector(rot);
    setScale(*(const FVECTOR*)inParts.scale);
    setPos(*(const FVECTOR*)inParts.trans);
    setOldPos(*(const FVECTOR*)inParts.trans);
    m_vStartPos = *(const FVECTOR*)inParts.trans;
    updateCoord();
    coordFix();
}

void CBaseGimmick::initFSM_() {
    m_GimmickState[GM_STATE_MAIN].set(this, &CBaseGimmick::beginStateMain, &CBaseGimmick::stateMain,
                                      &CBaseGimmick::endStateMain);
    m_GimmickState[GM_STATE_HIT_ATTACK].set(this, &CBaseGimmick::beginStateHitAttack, &CBaseGimmick::stateHitAttack,
                                            &CBaseGimmick::endStateHitAttack);
}

#line 375
BOOL CBaseGimmick::hitTest_() {
    if (!m_pBodyColl) return FALSE;
    m_pBodyColl->detect();
    if (!m_pBodyColl->isHit()) return FALSE;
    CCollBody* hit = m_pBodyColl->getHitCollBody();
#line 384
    if (!(hit)) AssertApp(__FILE__, __LINE__, "hit", 0);
    switch (hit->getDamageKind()) {
    case 7:
    case 8:
    case 9:
    case 10:
    case 28:
    case 37:
    case 41:
    case 42:
        return FALSE;
    }
    if (CSceneInfo::getCurrentWorldID() == 13 ||
        (CSceneInfo::getCurrentWorldID() == 23 && CSceneInfo::getCurrentSceneNum() == 13)) {
        FVECTOR v = *((CCollObj*)hit)->getDetectPos();
        setEffect("rec_ca_common.dpx", 6, &v, NULL, 0.0f);
    } else {
        hit->setHitEffect();
    }
    changeGimmickState(GM_STATE_HIT_ATTACK);
    return TRUE;
}

BOOL CBaseGimmick::initGimmickParam(INT inGroupID, INT inRsrcID) {
    return TRUE;
}

GimParam* CBaseGimmick::getGimParam() {
    return ((CGimmickManager*)getSysTask(17))->getGimParam(m_iID);
}

INT CBaseGimmick::createPrize(INT in_Flag, INT in_NotDropCardFlag) {
    CGimmickManager* pCGimMan = (CGimmickManager*)getSysTask(17);
    INT ret;
    if (!in_Flag) {
        ret = pCGimMan->DecidePrize(getID(), &Crd(getBoneIndex("prize", 0)).getWorldMatrix().v[3], &getRot(),
                                    in_NotDropCardFlag);
    } else {
        ret = pCGimMan->DecidePrize(getID(), &Crd(getBoneIndex("prize2", 0)).getWorldMatrix().v[3], &getRot(),
                                    in_NotDropCardFlag);
    }
    if (ret) {
        CEffectMan::setEffect("rec_priz_app", 0, ObjType::OBJ_TYPE_GIMMICK,
                              &Crd(getBoneIndex("prize", 0)).getWorldMatrix(), 0.0f);
    }
    return ret;
}

INT CBaseGimmick::saveStat(INT in_Stat) {
    INT ret = 0;
    SHORT scene = CSceneInfo::getCurrentSceneNum();
    if (!(1000 <= scene && scene < 2000)) {
        ret = ((CGimmickManager*)getSysTask(17))->setStorageGimmickState(getUniqueID(), in_Stat);
    }
    return ret;
}

// SOURCE DATA: .rodata
