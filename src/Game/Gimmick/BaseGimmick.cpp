/* BASEGIMMICK_SOURCE_RODATA 1 compiles every function and needs "// SOURCE DATA: .rodata" as the last line, plus
 * the unnamed copy of CState's vtable at the end of this object's .rodata named in
 * config/SLUS_217.99/symbol_addrs.txt ("_vt$6CState_004C97B8 = 0x004C97B8; // size:0x30"), so that
 * tools/ee_asm_compat.py keeps our compiled copy. With that, all 37 functions and the .rodata match (verified).
 * Without it (0), the functions that use .rodata (strings, jump table, member pointer constants) and the ones
 * that make the compiler emit the vtables and inline functions (the destructor is the key function) stay
 * INCLUDE_ASM. */
#define BASEGIMMICK_SOURCE_RODATA 0

#define GAME_GIMMICK_BASEGIMMICK_CPP
#define GPRIM_H_PATH "System/Graphic/GPrim.h"
#include "System/Task.h"
#include "Game/Collision/CollUtil.h"
#include "Game/BodyCollision.h"
#include "Game/Gimmick/BaseGimmick.h"
#include "System/Graphic/Gprim.h"
#include "System/Graphic/2d/2DMan.h"
#include "Game/CardBattle.h"
#include "System/Graphic/Str.h"

/* Placeholders: three empty strings follow Str.h's strings in .rodata (as in GimmickBell); which headers they
 * come from is not known. */
inline void _emptyStringPlaceholder0() {
    TRACE("");
}
inline void _emptyStringPlaceholder1() {
    TRACE("");
}
inline void _emptyStringPlaceholder2() {
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
#include "include_asm.h"

#define RET_ADDR() ({ UINT nRA; __asm__ volatile("daddu %0, $31, $0" : "=r"(nRA)); nRA; })

inline void* operator new(size_t size, char* msg) throw() {
    return com_malloc_d(size, msg, RET_ADDR());
}

#if BASEGIMMICK_SOURCE_RODATA
CBaseGimmick::CBaseGimmick(CTask* inParent)
    : CEntity(inParent, ObjType::OBJ_TYPE_GIMMICK), m_uiGimmickFlag(0), m_iUniqueID(0), m_hEffectHandle(0) {
    initFSM_();
    setEntityFlag(0, 0x1000);
}
#else
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Game/Gimmick/BaseGimmick", __12CBaseGimmickP5CTask);
#endif

#if BASEGIMMICK_SOURCE_RODATA
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
        m_pMove = new (__FILE__ "(148)") CMovement(this);
        if (!m_pMove) {
            out();
            return;
        }
        addCoord(1);
        makeCoord(inParts);
        initFSM_();
    }
}
#else
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Game/Gimmick/BaseGimmick", __12CBaseGimmickP5CTaskRC9ZONEPARTS);
#endif

#if BASEGIMMICK_SOURCE_RODATA
CBaseGimmick::~CBaseGimmick() {
    CEffect* effect = CEffectMan::getEffect(m_hEffectHandle);
    if (effect) effect->kill();
}
#else
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Game/Gimmick/BaseGimmick", _$_12CBaseGimmick);
#endif

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

#if BASEGIMMICK_SOURCE_RODATA
static inline void _setNewState(CState*& p, CState& NewState) { p = &NewState; }
#line 328
void CBaseGimmick::changeGimmickState(INT inState) {
    if (!(0 <= inState && inState < GM_STATE_MAX)) AssertApp(__FILE__, __LINE__, "0 <= inState && inState < GM_STATE_MAX", 0);
    _setNewState(m_pNewState, m_GimmickState[inState]);
}
#else
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Game/Gimmick/BaseGimmick", changeGimmickState__12CBaseGimmicki);
#endif

void CBaseGimmick::makeCoord(const ZONEPARTS& inParts) {
    FVECTOR& rot = (FVECTOR&)getRot();
    rot.x = inParts.rot.x * 0.1f * 0.017453292f;
    rot.y = inParts.rot.y * 0.1f * 0.017453292f;
    rot.z = inParts.rot.z * 0.1f * 0.017453292f;
    Math3D::LimitPIVector(rot);
    setScale(*(const FVECTOR*)inParts.scale);
    setPos(*(const FVECTOR*)inParts.trans);
    setOldPos(*(const FVECTOR*)inParts.trans);
    *(FVECTOR*)_m_0x70 = *(const FVECTOR*)inParts.trans;
    updateCoord();
    coordFix();
}

#if BASEGIMMICK_SOURCE_RODATA
void CBaseGimmick::initFSM_() {
    m_GimmickState[GM_STATE_MAIN].set(this, &CBaseGimmick::beginStateMain, &CBaseGimmick::stateMain,
                                      &CBaseGimmick::endStateMain);
    m_GimmickState[GM_STATE_HIT_ATTACK].set(this, &CBaseGimmick::beginStateHitAttack, &CBaseGimmick::stateHitAttack,
                                            &CBaseGimmick::endStateHitAttack);
}
#else
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Game/Gimmick/BaseGimmick", initFSM___12CBaseGimmick);
#endif

#if BASEGIMMICK_SOURCE_RODATA
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
#else
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Game/Gimmick/BaseGimmick", hitTest___12CBaseGimmick);
#endif

BOOL CBaseGimmick::initGimmickParam(INT inGroupID, INT inRsrcID) {
    return TRUE;
}

GimParam* CBaseGimmick::getGimParam() {
    return ((CGimmickManager*)getSysTask(17))->getGimParam(m_iID);
}

#if BASEGIMMICK_SOURCE_RODATA
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
#else
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Game/Gimmick/BaseGimmick", createPrize__12CBaseGimmickii);
#endif

INT CBaseGimmick::saveStat(INT in_Stat) {
    INT ret = 0;
    SHORT scene = CSceneInfo::getCurrentSceneNum();
    if (!(1000 <= scene && scene < 2000)) {
        ret = ((CGimmickManager*)getSysTask(17))->setStorageGimmickState(getUniqueID(), in_Stat);
    }
    return ret;
}

#if !BASEGIMMICK_SOURCE_RODATA
/* Weak (inline) functions: from the headers once the destructor (the key function) is compiled here. */
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Game/Gimmick/BaseGimmick", _$_t14CStateTemplate1Z12CBaseGimmick);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Game/Gimmick/BaseGimmick", setGimmickFlag__12CBaseGimmickUiUi);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Game/Gimmick/BaseGimmick", setParam__12CBaseGimmicki);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Game/Gimmick/BaseGimmick", setUniqueID__12CBaseGimmicki);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Game/Gimmick/BaseGimmick", getGimmickFlag__C12CBaseGimmick);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Game/Gimmick/BaseGimmick", getParam__C12CBaseGimmick);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Game/Gimmick/BaseGimmick", getUniqueID__C12CBaseGimmick);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Game/Gimmick/BaseGimmick", beginStateMain__12CBaseGimmick);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Game/Gimmick/BaseGimmick", stateMain__12CBaseGimmick);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Game/Gimmick/BaseGimmick", endStateMain__12CBaseGimmick);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Game/Gimmick/BaseGimmick", beginStateHitAttack__12CBaseGimmick);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Game/Gimmick/BaseGimmick", stateHitAttack__12CBaseGimmick);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Game/Gimmick/BaseGimmick", endStateHitAttack__12CBaseGimmick);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Game/Gimmick/BaseGimmick", executeBeginState__t14CStateTemplate1Z12CBaseGimmick);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Game/Gimmick/BaseGimmick", executeState__t14CStateTemplate1Z12CBaseGimmick);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Game/Gimmick/BaseGimmick", executeEndState__t14CStateTemplate1Z12CBaseGimmick);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Game/Gimmick/BaseGimmick", func_002D6ED8);
#endif
