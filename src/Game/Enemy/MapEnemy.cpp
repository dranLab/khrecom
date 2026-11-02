
#define COLLOBJ_H_ENUMS_ONLY /* MATCH: the collision classes come after CMapEnemy (their vtables precede its own) */
#define GPRIM_H_PATH "System/Graphic/GPrim.h"
#include "System/Task.h"
#include "System/TaskMan.h"
#include "Game/BodyCollision.h"
#include "Game/Collision/CollUtilA.h"
#include "Game/Collision/CollUtil.h"
#include "System/Graphic/Anim.h"
#include "System/Graphic/Gprim.h"
#include "System/Graphic/2d/2DMan.h"
#include "Game/CardBattle.h"
#include "System/Graphic/Str.h"

/* Placeholder: an empty string follows Str.h's strings in .rodata (as in Game/Prize). */
inline void _emptyStringPlaceholder() {
    TRACE("");
}

#include "System/Graphic/modeldata.h"
#include "System/Graphic/Model.h"
#include "System/RsrcData.h"
#include "Game/Enemy/EnemyParam.h"
#include "System/Graphic/Effect/effect.h"
#include "Game/Enemy/MapEnemy.h"
#define COLLOBJ_H_COLLISION_CLASSES /* MATCH: constructs CPointCollision */
#undef COLLOBJ_H_ENUMS_ONLY
#include "Game/Collision/CollObj.h"
#include "Game/Collision/CollMap.h"
#include "System/Graphic/Camera.h"
#include "System/Math3D.h"
#include "System/Rnd.h"
#include "Game/SceneInfo.h"
#include "Game/Player/Player.h"
#include "Game/Debug/DebugWork.h"
#include "Game/LockOnManager.h"
#include "System/Sound/SoundMan.h"
#include "Game/Enemy/MapEnemyManager.h"
#include "Game/Collision/CollInfo.h"
#include "Game/Collision/CollBody.h"








#include "Game/Movement.h"
#include "Game/MessageManager.h"
#include "System/Graphic/Effect/EffectMan.h"
#include "Game/SceneEnv.h"
#include "Game/GameWork.h"
#include <string.h>
#include "include_asm.h"

/* Inline in the original's collision header: defined here, after CMapEnemy, so that its vtable copies come first
 * in .rodata (as in Game/Gimmick/GimmickCarry). */






#define MAP_ENEMY_STATE_MAX 9


void Normalize(FVECTOR& v0, const FVECTOR& v1);
void ScaleVector(FVECTOR& v0, const FVECTOR& v1, FLOAT f);
void AddVector(FVECTOR& v0, const FVECTOR& v1, const FVECTOR& v2);
void ApplyMatrix(FVECTOR& v0, const FMATRIX& m, const FVECTOR& v1);
extern "C" float fabsf(float x);
extern "C" float sinf(float x);
extern "C" float cosf(float x);
extern "C" int rand();
FVECTOR getPlayerPos(BOOL flag);
void SubVectorXYZ(FVECTOR& v0, const FVECTOR& v1, const FVECTOR& v2);
void AddVectorXYZ(FVECTOR& v0, const FVECTOR& v1, const FVECTOR& v2);
void ScaleVectorXYZ(FVECTOR& v0, const FVECTOR& v1, FLOAT f);

extern "C" void sceVu0RotMatrixY(FMATRIX& m0, const FMATRIX& m1, float ry);

static inline void _unitMatrixVu0(FMATRIX& m) {
    asm volatile("vmr32.xyzw $vf1, $vf0\n"
                 "sqc2 $vf0, 0x30(%0)\n"
                 "vmr32.xyzw $vf2, $vf1\n"
                 "sqc2 $vf1, 0x20(%0)\n"
                 "vmr32.xyzw $vf3, $vf2\n"
                 "sqc2 $vf2, 0x10(%0)\n"
                 "sqc2 $vf3, 0x0(%0)\n"
                 :
                 : "r"(&m)
                 : "memory");
}

#include "System/RsrcDB.h"




/* ENEMY_PARAM's inline relocation (its "@ENP" signature check is EnemyParam.h's placeholder): an already
 * relocated file has the top bit of its version set. */
static inline BOOL _setupEnemyParam(ENEMY_PARAM* inParam) {
    INT* p = (INT*)inParam;
    if (p[1] < 0) return TRUE;
    if (strncmp((const char*)inParam, "@ENP", 4)) return FALSE;
    if (p[1] != 1) return FALSE;
    p[1] = 0x80000001;
    p[2] = (INT)inParam + p[2];
    p[0x48 / 4] = (INT)inParam + p[0x48 / 4];
    p[0x50 / 4] = (INT)inParam + p[0x50 / 4];
    p[0x94 / 4] = (INT)inParam + p[0x94 / 4];
    return TRUE;
}

#define ASSERT(e) if (!(e)) AssertApp(__FILE__, __LINE__, #e, 0)

namespace MapEnemy {
static CHAR* g_pIdlingMotion[37] = {
    NULL, "e001_001", "e002_001", "e003_001", "e003_001", "e003_001",
    "e003_001", "e007_001", "e008_001", "e009_001", "e010_001", "e011_001",
    "e012_001", "e013_001", "e014_001", "e015_001", "e016_001", "e017_001",
    "e018_001", "e019_001", "e020_001", "e021_001", "e022_001", "e023_001",
    "e024_001", "e025_001", "e026_001", "e027_001", "e028_003", "e029_001",
    "e030_001", "e031_001", "e001_001", "e014_001", "e016_001", "e017_001",
    "e031_001",
};

static CHAR* g_pWalkMotion[37] = {
    NULL, "e001_002", "e002_002", "e003_002", "e003_002", "e003_002",
    "e003_002", "e007_002", "e008_002", "e009_002", "e010_002", "e011_002",
    "e012_002", "e013_002", "e014_001", "e015_002", "e016_002", "e017_002",
    "e018_001", "e019_002", "e020_002", "e021_002", "e022_002", "e023_001",
    "e024_003", "e025_001", "e026_002", "e027_002", "e028_004", "e029_002",
    "e030_002", "e031_001", "e001_002", "e014_001", "e016_002", "e017_002",
    "e031_001",
};

static CHAR* g_pRunMotion[37] = {
    NULL, "e001_003", "e002_003", "e003_002", "e003_002", "e003_002",
    "e003_002", "e007_002", "e008_002", "e009_002", "e010_002", "e011_002",
    "e012_002", "e013_002", "e014_001", "e015_002", "e016_002", "e017_002",
    "e018_001", "e019_002", "e020_002", "e021_002", "e022_002", "e023_001",
    "e024_003", "e025_001", "e026_002", "e027_002", "e028_004", "e029_002",
    "e030_002", "e031_001", "e001_003", "e014_001", "e016_002", "e017_002",
    "e031_001",
};

static CHAR* MapSpeedParamName = "MapSpeed";
static CHAR* MapSpeed2ParamName = "MapSpeed2";
static CHAR* ViewAngleParamName = "ViewAngle";
static CHAR* ViewAngle2ParamName = "ViewAngle2";
static CHAR* ViewAreaParamName = "ViewArea";
static CHAR* ViewArea2ParamName = "ViewArea2";
static CHAR* FileIDParamName = "FileID";
static CHAR* BadStatusEffectName = "abnormal";

static const INT g_bGravity[37] = {
    1, 1, 1, 0, 0, 0, 0, 1, 1, 1, 1, 1,
    1, 0, 0, 0, 1, 1, 0, 0, 0, 1, 0, 0,
    0, 0, 1, 1, 1, 1, 1, 1, 1, 0, 1, 1,
    1,
};

static const FLOAT g_fCollRadius[37] = {
    0.5f, 0.5f, 0.5f, 0.5f, 0.5f, 0.5f, 0.5f, 0.5f,
    0.5f, 1.25f, 1.25f, 0.5f, 0.5f, 0.5f, 0.5f, 0.5f,
    0.5f, 0.5f, 0.5f, 0.5f, 2.0f, 0.5f, 0.5f, 0.5f,
    0.5f, 0.5f, 1.25f, 0.5f, 0.5f, 0.5f, 0.5f, 0.5f,
    0.5f, 0.5f, 0.5f, 0.5f, 0.5f,
};
} // namespace MapEnemy

void (CMapEnemy::*CMapEnemy::m_pBeginStateTbl[])() = {
    &CMapEnemy::beginStateAppear, &CMapEnemy::beginStateMain, &CMapEnemy::beginStateSleeping,
    &CMapEnemy::beginStateStanding, &CMapEnemy::beginStateDamage, &CMapEnemy::beginStateFaint,
    &CMapEnemy::beginStateStop, &CMapEnemy::beginStateConfuse, &CMapEnemy::beginStateDisappear,
};
void (CMapEnemy::*CMapEnemy::m_pStateTbl[])() = {
    &CMapEnemy::stateAppear, &CMapEnemy::stateMain, &CMapEnemy::stateSleeping,
    &CMapEnemy::stateStanding, &CMapEnemy::stateDamage, &CMapEnemy::stateFaint,
    &CMapEnemy::stateStop, &CMapEnemy::stateConfuse, &CMapEnemy::stateDisappear,
};
void (CMapEnemy::*CMapEnemy::m_pEndStateTbl[])() = {
    &CMapEnemy::endStateAppear, &CMapEnemy::endStateMain, &CMapEnemy::endStateSleeping,
    &CMapEnemy::endStateStanding, &CMapEnemy::endStateDamage, &CMapEnemy::endStateFaint,
    &CMapEnemy::endStateStop, &CMapEnemy::endStateConfuse, &CMapEnemy::endStateDisappear,
};

CMapEnemy::CMapEnemy(CTask* inParent, CState* inState)
    : CEntity(inParent, (ObjType::eObjType)2), m_pEnemyParam(NULL), m_iAppearType(2), m_uiFlag(0),
      m_fMotionSpeed(1.0f), m_hBadStatus(0), m_pState(inState) {
    setEntityFlag(0x80, 0);
    m_vTarget.init();
}

CMapEnemy::~CMapEnemy() {
    CEffectMan::killEffect(m_hBadStatus, 0.0f);
}

INT CMapEnemy::getTypeID() const { return 0x1002; }
BOOL CMapEnemy::initalize(INT inGroupID, INT inRsrcID) {
    if (!CEntity::initalize(inGroupID, inRsrcID)) return FALSE;
    initEnemyParam_(inGroupID, inRsrcID);
    for (INT i = 0; i < 9; i++) {
        m_MapEnemyState[i].set(this, m_pBeginStateTbl[i], m_pStateTbl[i], m_pEndStateTbl[i]);
    }
#line 350
    ASSERT(m_pEnemyParam);
    m_fWalkSpeed = m_pEnemyParam->getExtraParam(MapEnemy::MapSpeedParamName);
    m_fRunSpeed = m_pEnemyParam->getExtraParam(MapEnemy::MapSpeed2ParamName);
    m_fViewAngle = m_pEnemyParam->getExtraParam(MapEnemy::ViewAngleParamName);
    m_fViewArea = m_pEnemyParam->getExtraParam(MapEnemy::ViewAreaParamName);
    INT id = (UINT)getID() % 10000;
#line 358
    ASSERT(1 <= id && id < (sizeof(MapEnemy::g_bGravity)/sizeof((MapEnemy::g_bGravity)[0])));
    m_pMove->enableGravity(MapEnemy::g_bGravity[id]);
    FLOAT radius = MapEnemy::g_fCollRadius[id];
    if (1.0f < radius) {
        m_pMove->setFlag(0x40, 0);
        m_pMove->setFootRadius(0.5f);
        m_pMove->setWallRadius(radius);
    } else {
        m_pMove->setRadius(radius);
    }
    switch (id) {
    case 0x10:
        m_iAppearType = 1;
        break;
    case 0x11:
    case 0x23:
        m_uiDispOffFlag |= 2;
        break;
    case 0x22:
        m_iAppearType = 1;
        break;
    }
    setMapEnemyFlag(0x20000000, 0);
    RoomInfo* room = CSceneInfo::getRoomInfo(-1);
    if (room) {
        switch (room->getCardID()) {
        case 0xa5:
            setMapEnemyFlag(0x40000000, 0x20000000);
            changeMapEnemyState(2);
            break;
        case 0xa6:
            if (g_pPlayer) {
                PL_PARAM* plParam = g_pPlayer->m_pParam;
#line 407
                ASSERT(plParam);
                FLOAT speed = plParam->MAX_RUN_SPEED / 60.0f * 0.9f;
                if (m_fRunSpeed == 0.0f || m_fWalkSpeed == m_fRunSpeed) {
                    m_fMotionSpeed = speed / m_fWalkSpeed;
                    if (2.0f < m_fMotionSpeed) m_fMotionSpeed = 2.0f;
                    m_fRunSpeed = speed;
                    m_fWalkSpeed = speed;
                } else {
                    m_fMotionSpeed = speed / m_fRunSpeed;
                    if (2.0f < m_fMotionSpeed) m_fMotionSpeed = 2.0f;
                    m_fWalkSpeed = m_fWalkSpeed / m_fRunSpeed * speed;
                    m_fRunSpeed = speed;
                }
            }
            m_fViewAngle = m_pEnemyParam->getExtraParam(MapEnemy::ViewAngle2ParamName);
            if (m_fViewAngle == 0.0f) m_fViewAngle = m_pEnemyParam->getExtraParam(MapEnemy::ViewAngleParamName);
            m_fViewArea = m_pEnemyParam->getExtraParam(MapEnemy::ViewArea2ParamName);
            if (m_fViewArea == 0.0f) m_fViewArea = m_pEnemyParam->getExtraParam(MapEnemy::ViewAreaParamName);
            setMapEnemyFlag(8, 0);
            changeMapEnemyState(0);
            break;
        case 0xb4:
            if (m_iMapEnemyParam) {
                setMapEnemyFlag(0x80000000, 0x20000000);
                changeMapEnemyState(3);
            } else {
                changeMapEnemyState(0);
            }
            break;
        case 0xb0:
            m_fMotionSpeed = 0.5f;
            m_fWalkSpeed *= 0.5f;
            m_fRunSpeed *= 0.5f;
            changeMapEnemyState(0);
            break;
        default:
            changeMapEnemyState(0);
            break;
        }
    } else {
        changeMapEnemyState(0);
    }
    LockOnManager::CLockOnManager::registerLockOnObject(getHandle());
    setTimer(6000);
    setEntityFlag(1, 0x1000);
    bodyCollOff(-1);
    return TRUE;
}
void CMapEnemy::onMsg(INT inMsg, void* inExtraInfo) {
    if (inMsg == 0x10101010) setAppearEffect();
}

BOOL CMapEnemy::isCurrentState(INT inState) { return isState(m_MapEnemyState[inState]); }

INT CMapEnemy::getIdlingMotion() const {
    INT id = (UINT)getID() % 10000;
#line 501
    ASSERT(1 <= id && id < (sizeof(MapEnemy::g_pIdlingMotion)/sizeof((MapEnemy::g_pIdlingMotion)[0])));
    return getMotionIndex(MapEnemy::g_pIdlingMotion[id]);
}

INT CMapEnemy::getWalkMotion() const {
    INT id = (UINT)getID() % 10000;
#line 510
    ASSERT(1 <= id && id < (sizeof(MapEnemy::g_pWalkMotion)/sizeof((MapEnemy::g_pWalkMotion)[0])));
    return getMotionIndex(MapEnemy::g_pWalkMotion[id]);
}

INT CMapEnemy::getRunMotion() const {
    INT id = (UINT)getID() % 10000;
#line 519
    ASSERT(1 <= id && id < (sizeof(MapEnemy::g_pRunMotion)/sizeof((MapEnemy::g_pRunMotion)[0])));
    return getMotionIndex(MapEnemy::g_pRunMotion[id]);
}

void CMapEnemy::updateBodyColl() {
    if (m_pBodyColl) m_pBodyColl->update();
}

FLOAT CMapEnemy::getAppearDistance() {
    CMapEnemyManager* mem = (CMapEnemyManager*)CTaskMan::getSysTask(0x18);
#line 539
    ASSERT(mem);
    return mem->m_fAppearDistance;
}

FLOAT CMapEnemy::getDisappearDistance() {
    CMapEnemyManager* mem = (CMapEnemyManager*)CTaskMan::getSysTask(0x18);
#line 549
    ASSERT(mem);
    return mem->m_fDisappearDistance;
}

TIMEQ CMapEnemy::getFaintTime() {
    CMapEnemyManager* mem = (CMapEnemyManager*)CTaskMan::getSysTask(0x18);
#line 559
    ASSERT(mem);
    return mem->getFaintTime();
}

TIMEQ CMapEnemy::getStopTime() {
    CMapEnemyManager* mem = (CMapEnemyManager*)CTaskMan::getSysTask(0x18);
#line 569
    ASSERT(mem);
    return mem->getStopTime();
}

TIMEQ CMapEnemy::getConfuseTime() {
    CMapEnemyManager* mem = (CMapEnemyManager*)CTaskMan::getSysTask(0x18);
#line 579
    ASSERT(mem);
    return mem->getConfuseTime();
}

void CMapEnemy::changeMapEnemyMotion(INT inMotionType, TIMEQ inTq, BOOL inRootMove, TIMEQ inGr) {
    INT index;
    switch (inMotionType) {
    case 0:
        index = getIdlingMotion();
        setAnimSpeed(1.0f);
        break;
    case 1:
        index = getWalkMotion();
        setAnimSpeed(m_fMotionSpeed);
        break;
    case 2:
        index = getRunMotion();
        setAnimSpeed(m_fMotionSpeed);
        break;
    }
    changeMotion(index, inTq, inRootMove, inGr, 0, 0);
}

void CMapEnemy::updateBefore() {}

void CMapEnemy::lastUpdate() {
    updateBodyColl();
    updateBadStatusEffect(m_hBadStatus);
    m_pBodyColl->setRadiusScale(1.0f);
    if (hitTest_()) return;
    if (!m_pBodyColl) return;
    if (!(CSceneEnv::getStopAndRequestTypeBit() & 4)) {
        m_pBodyColl->setRadiusScale(0.7f);
        if (m_pBodyColl->collideBody(1, NULL, NULL, NULL) && checkEncont_()) {
            m_pBodyColl->off(-1);
            m_pBodyColl->offBody(-1);
        }
        m_pBodyColl->setRadiusScale(1.0f);
    } else {
        FVECTOR result;
        if (m_pBodyColl->collideBody(1, &result, NULL, NULL)) {
            FVECTOR pos = getPos();
            setOldPos(pos);
#line 714
            ASSERT(m_pMove);
            pos.x = result.x;
            pos.z = result.z;
            setPos(pos);
            updateCoord();
            coordFix();
        }
    }
    if (m_iMapEnemyParam == 0) {
        FVECTOR result;
        if (m_pBodyColl->collideBody(0, &result, NULL, NULL)) {
            FVECTOR pos = getPos();
            setOldPos(pos);
#line 734
            ASSERT(m_pMove);
            if (!m_pMove->isGround()) {
                AddVectorXYZ(pos, pos, result);
                ScaleVectorXYZ(pos, pos, 0.5f);
            } else {
                pos.x = (result.x + pos.x) * 0.5f;
                pos.z = (result.z + pos.z) * 0.5f;
            }
            setPos(pos);
            setMapEnemyFlag(2, 0);
        } else {
            setMapEnemyFlag(0, 2);
        }
    }
    m_pBodyColl->setRadiusScale(0.7f);
}

void CMapEnemy::changeState(INT inState) {
    CState* state = &((CStateTemplate<CMapEnemy>*)m_pState)[inState];
    gotoState(*state);
}

void CMapEnemy::changeMapEnemyState(INT inState) {
#line 777
    ASSERT(0 <= inState && inState < MAP_ENEMY_STATE_MAX);
    gotoState(m_MapEnemyState[inState]);
}
BOOL CMapEnemy::checkWall(FVECTOR& outRefVec) {
    CPointCollision coll;
    FVECTOR e;
    coll.setStartPos(getPos());
    Normalize(e, m_pMove->getVel());
    ScaleVector(e, e, 1.0f);
    AddVector(e, e, getPos());
    coll.setEndPos(e);
    if (coll.collideMap() && coll.m_outNormal.y < 0.64278764f) {
        Math3D::CalcReflectionVector(outRefVec, coll.m_outNormal, m_pMove->getVel());
        return TRUE;
    }
    return FALSE;
}

BOOL CMapEnemy::checkGround() {
    CCollMap* coll = m_pMove->getMapCollision();
#line 808
    ASSERT(coll);
    FLOAT r = m_pMove->getRadius();
    FVECTOR pos;
    pos.set(0.0f, 0.0f, r * 2.25f, 1.0f);
    ApplyMatrix(pos, Crd(0).m_World, pos);
    coll->setStartPos((FVECTOR&)getPos());
    coll->setEndPos(pos);
    if (coll->collideMap() && coll->getDetectNormal()->y < 0.64278764f) return FALSE;
    pos.y += r;
    coll->setStartPos(pos);
    pos.y -= r * 2.5f;
    coll->setEndPos(pos);
    if (coll->collideMap() && coll->getDetectNormal()->y >= 0.64278764f) return TRUE;
    return FALSE;
}
BOOL CMapEnemy::canAppear() const {
    CMapEnemyManager* mem = (CMapEnemyManager*)CTaskMan::getSysTask(0x18);
#line 844
    ASSERT(mem);
    return mem->canAppear();
}
BOOL CMapEnemy::canPlayerChase(FLOAT inHeight, BOOL inGround) {
    FVECTOR target = getPlayerPos(FALSE);
    if (g_pPlayer && g_pPlayer->isGround() && fabsf(target.y - getPos().y) > inHeight) {
        setMapEnemyFlag(0, 4);
        return FALSE;
    }
    if (m_fViewAngle == 0.0f && m_fViewArea == 0.0f) {
        FLOAT dist = calcDistanceXZSq(target);
        if (dist > getDisappearDistance()) {
            setMapEnemyFlag(0, 4);
            return FALSE;
        }
    } else {
        if (calcDistanceXZSq(target) > m_fViewArea * m_fViewArea) {
            setMapEnemyFlag(0, 4);
            return FALSE;
        }
        if (!entityVisibleAxisY(g_pPlayer, m_fViewAngle)) return FALSE;
    }
    if (isPlayerFind()) return TRUE;
    if (!inGround) return TRUE;
    CCollMap* coll = m_pMove->getMapCollision();
#line 894
    ASSERT(coll);
    FVECTOR playerPos = getPlayerPos(FALSE);
    FVECTOR pos = getPos();
    coll->setStartPos(pos);
    coll->setEndPos(playerPos);
    if (coll->collideRideForCheck(FALSE) && coll->getDetectNormal()->y < 0.64278764f) return FALSE;
    if (coll->collideMap() && coll->getDetectNormal()->y < 0.64278764f) return FALSE;
    FVECTOR dir;
    FVECTOR v;
    FLOAT len = calcDistance(playerPos) / 10.0f;
    SubVectorXYZ(dir, playerPos, pos);
    Normalize(dir, dir);
    ScaleVectorXYZ(v, dir, len);
    AddVector(pos, pos, v);
    for (INT i = 0; i < 10; i++) {
        FVECTOR start = pos;
        start.y += 0.1f;
        coll->setStartPos(start);
        FVECTOR downVec = pos;
        downVec.y -= len;
        coll->setEndPos(downVec);
        if (!coll->collideMap()) return FALSE;
        if (coll->getDetectNormal()->y < 0.64278764f) return FALSE;
        AddVector(pos, pos, v);
    }
    setMapEnemyFlag(4, 0);
    return TRUE;
}
/* near-match (one getOutPos() is if-converted; s3/s4 swapped): kept as INCLUDE_ASM */
#if 0
void CMapEnemy::getRandomTargetPos(FVECTOR& outTarget, FLOAT inDistance, BOOL inGround) {
    CCollMap* coll = m_pMove->getMapCollision();
#line 953
    ASSERT(coll);
    FLOAT rotDir = (RND::genrand_int31() & 1) ? 1.0f : -1.0f;
    FLOAT startRot = RND::genrand_real1() * 360.0f;
    FLOAT far = 2.0f;
    FVECTOR pos = getPos();
    pos.y += 0.1f;
    for (FLOAT deg = 0.0f; deg < 360.0f; deg += 30.0f) {
        FMATRIX m;
        FVECTOR target;
        FVECTOR dir;
        FLOAT dist;
        _unitMatrixVu0(m);
        sceVu0RotMatrixY(m, m, Math3D::LimitPI(startRot + rotDir * deg * 0.017453292f));
        dir.set(0.0f, 0.0f, 1.0f, 1.0f);
        ApplyMatrix(dir, m, dir);
        ScaleVectorXYZ(target, dir, inDistance);
        AddVector(target, target, pos);
        coll->setStartPos(pos);
        coll->setEndPos(target);
        if (coll->collideRideForCheck(FALSE)) continue;
        coll->setStartPos(pos);
        coll->setEndPos(target);
        if (coll->collideMap()) {
            if (coll->getDetectNormal()->y < 0.64278764f) continue;
            target = *coll->getOutPos();
        }
        coll->setStartPos(target);
        target.y -= 1.0f;
        coll->setEndPos(target);
        if (!coll->collideMap()) continue;
        target.y += 1.0f;
        dist = Math3D::PointToPointDistance(pos, target);
        if (dist < far) continue;
        far = dist;
        if (inGround) {
            FVECTOR start = pos;
            FVECTOR v;
            BOOL air = FALSE;
            ScaleVectorXYZ(v, dir, dist * 0.1f);
            AddVector(start, start, v);
            for (INT i = 0; i < 10; i++) {
                start.y += 0.25f;
                coll->setStartPos(start);
                FVECTOR downVec = start;
                downVec.y -= 1.25f;
                coll->setEndPos(downVec);
                if (!coll->collideMap() || coll->getDetectNormal()->y < 0.64278764f) {
                    air = TRUE;
                    break;
                }
                start = *coll->getOutPos();
                AddVector(start, start, v);
            }
            if (air) continue;
        }
        outTarget = target;
        return;
    }
    outTarget = getPos();
}
#else
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Game/Enemy/MapEnemy", getRandomTargetPos__9CMapEnemyR7FVECTORfi);
#endif

void CMapEnemy::getWarpPos(FVECTOR& outPos) {
    CCollMap* coll = m_pMove->getMapCollision();
#line 1078
    ASSERT(coll);
    FLOAT rotDir = (RND::genrand_int31() & 1) ? 1.0f : -1.0f;
    FVECTOR pos = getPos();
    for (FLOAT deg = 0.0f; deg < 360.0f; deg += 30.0f) {
        FMATRIX m;
        FVECTOR target;
        FVECTOR dir;
        _unitMatrixVu0(m);
        sceVu0RotMatrixY(m, m, Math3D::LimitPI(getRot().y + rotDir * deg * 0.017453292f));
        dir.set(0.0f, 0.0f, 1.0f, 1.0f);
        ApplyMatrix(dir, m, dir);
        ScaleVectorXYZ(target, dir, 10.0f);
        AddVector(target, target, getPos());
        coll->setStartPos(pos);
        coll->setEndPos(target);
        if (coll->collideRideForCheck(FALSE)) continue;
        coll->setStartPos(pos);
        coll->setEndPos(target);
        if (coll->collideMap()) {
            if (coll->getDetectNormal()->y < 0.64278764f) continue;
            target = *coll->getOutPos();
        }
        coll->setStartPos(target);
        target.y -= 10.0f;
        coll->setEndPos(target);
        if (coll->collideRideForCheck(FALSE)) continue;
        if (!coll->collideMap()) continue;
        outPos = *coll->getOutPos();
        return;
    }
    outPos = m_vStartPos;
}
void CMapEnemy::beginStateAppear() {
    m_iAppearType = 1;
    setMapEnemyFlag(0, 1);
    m_iSysStep = 0;
    setModelAlphaScale(0.0f, -1);
    bodyCollOff(-1);
    bodyCollOffBody(-1);
    setEntityFlag(0, 0x10001);
    m_pMove->setFlag(9, 0);
}

void CMapEnemy::stateAppear() {
    switch (m_iSysStep) {
    case 0:
        if (getTimer() > 0) break;
        if (calcDistanceSq(g_pPlayer) <= getAppearDistance() && canAppear()) {
            if (m_pEnemyParam && m_pEnemyParam->iAppearMotionIndex != -1) {
                changeMotion(m_pEnemyParam->iAppearMotionIndex, 0, 0, 0, 0, 0);
                m_iSysStep = 1;
            } else {
                m_iSysStep = 3;
            }
            setEntityFlag(0x40000000, 0);
            setupFade(0.0f, 1.0f, 3000, -1);
            Message::CMessageManager::getInstance().sendMessage(150, getHandle(), getHandle(), 0x10101010, NULL);
            setTimer(1500);
            m_pMove->setFlag(0, 9);
            getRot().y = calcDirection(g_pPlayer);
            setMapEnemyFlag(1, 0);
        }
        break;
    case 1:
        if (getTimer() <= 0) {
            bodyCollOnBody(-1);
            m_iSysStep = 2;
        }
    case 2:
        if (isMotionEnd()) changeMapEnemyState(1);
        break;
    case 3:
        if (!isFade()) changeMapEnemyState(1);
        break;
    }
}

void CMapEnemy::endStateAppear() {
    bodyCollOn(-1);
    bodyCollOnBody(-1);
    setEntityFlag(0x10000, 0);
}

void CMapEnemy::beginStateMain() {
    if (m_pMove->isGravity()) {
        changeMapEnemyMotion(0, 0, 0, -1);
    } else {
        changeMapEnemyMotion(0, 0, 0, -1);
    }
    m_iSysStep = 0;
}

void CMapEnemy::stateMain() {
    if (m_pMove->isGravity()) {
        groundEnemyProc_();
    } else {
        flyEnemyProc_();
    }
}

void CMapEnemy::endStateMain() {}

void CMapEnemy::beginStateSleeping() {
    m_iAppearType = 3;
    changeMotion(m_pEnemyParam->iFaintMotionIndex[1], 0, 0, -1, 0, 0);
    setEntityFlag(0, 1);
    setTimer(100);
    bodyCollOn(-1);
    m_hBadStatus = setBadStatusEffect(MapEnemy::BadStatusEffectName, 5, 0.0f);
    m_iSysStep = 0;
}

void CMapEnemy::stateSleeping() {}

void CMapEnemy::endStateSleeping() {
    CEffectMan::killEffect(m_hBadStatus, 0.0f);
    m_hBadStatus = 0;
    setAnimSpeed(1.0f);
}

void CMapEnemy::beginStateStanding() {
    m_iAppearType = 3;
    setEntityFlag(0, 1);
    changeMapEnemyMotion(0, 0, 0, -1);
    bodyCollOn(-1);
}
void CMapEnemy::stateStanding() {}

void CMapEnemy::endStateStanding() {}

void CMapEnemy::beginStateDamage() {
    if (m_pBodyColl->getHitCollBody()) {
        if (m_pEnemyParam) {
            if (testFront(*m_pBodyColl->getHitCollBody()->getDetectPos())) {
                changeMotion(m_pEnemyParam->iDamageMotionIndex[0], 0, 0, -1, 0, 0);
            } else {
                changeMotion(m_pEnemyParam->iDamageMotionIndex[2], 0, 0, -1, 0, 0);
            }
            setTimer(getMotionTotalTime() / 2);
        } else {
            setTimer(3000);
        }
    }
    setAnimSpeed(1.0f);
    if (m_pBodyColl->getHitCollBody()->getDamageKind() == 0x24) {
        m_iSysStep = 1;
    } else {
        m_EntityType = (ObjType::eObjType)3;
        CSceneEnv::setStop((ObjType::eObjBit)4);
        CGameWork::setFlag(1, 5, TRUE);
        bodyCollOff(-1);
        bodyCollOffBody(-1);
        m_iSysStep = 0;
    }
}

void CMapEnemy::stateDamage() {
    if (getTimer() > 0) return;
    switch (m_iSysStep) {
    case 0:
        setAnimSpeed(0.0f);
        setMapEnemyFlag(0x10, 0);
        checkEncont_();
        break;
    case 1:
        if (isSleeping()) {
            changeMapEnemyState(2);
        } else if (isStanding()) {
            changeMapEnemyState(3);
        } else {
            changeMapEnemyState(1);
        }
        break;
    }
}

void CMapEnemy::endStateDamage() {}

void CMapEnemy::beginStateFaint() {
    if (m_pEnemyParam) {
        CCollBody* hit = m_pBodyColl->getHitCollBody();
#line 1395
        ASSERT(hit);
        if (testFront(*hit->getDetectPos())) {
            changeMotion(m_pEnemyParam->iDamageMotionIndex[0], 0, 0, -1, 0, 0);
        } else {
            changeMotion(m_pEnemyParam->iDamageMotionIndex[2], 0, 0, -1, 0, 0);
        }
        INT kind = hit->getDamageKind();
        if (kind >= 0x1c && (kind < 0x1f || kind == 0x26)) setMapEnemyFlag(0x40, 0);
    } else {
        setTimer(getFaintTime());
    }
    m_iSysStep = 0;
    setMapEnemyFlag(0x10, 0);
}

void CMapEnemy::stateFaint() {
    if (m_pEnemyParam) {
        switch (m_iSysStep) {
        case 0:
            if (isMotionEnd()) {
                changeMotion(m_pEnemyParam->iFaintMotionIndex[0], 0, 0, -1, 0, 0);
                m_hBadStatus = setBadStatusEffect(MapEnemy::BadStatusEffectName, 1, 0.0f);
                m_iSysStep++;
            }
            break;
        case 1:
            if (isMotionEnd()) {
                setTimer(getFaintTime());
                changeMotion(m_pEnemyParam->iFaintMotionIndex[1], 0, 0, -1, 0, 0);
                m_iSysStep++;
            }
            break;
        case 2:
            if (getTimer() > 0) break;
            setMapEnemyFlag(0, 0x10);
            CEffectMan::killEffect(m_hBadStatus, 0.0f);
            m_hBadStatus = 0;
            changeMotion(m_pEnemyParam->iFaintMotionIndex[2], 0, 0, -1, 0, 0);
            m_iSysStep++;
            break;
        case 3:
            if (isMotionEnd()) {
                if (isSleeping()) {
                    changeMapEnemyState(2);
                } else if (isStanding()) {
                    changeMapEnemyState(3);
                } else {
                    changeMapEnemyState(1);
                }
            }
            break;
        }
    } else {
        if (getTimer() > 0) return;
        if (isSleeping()) {
            changeMapEnemyState(2);
        } else if (isStanding()) {
            changeMapEnemyState(3);
        } else {
            changeMapEnemyState(1);
        }
    }
}

void CMapEnemy::endStateFaint() {
    CEffectMan::killEffect(m_hBadStatus, 0.0f);
    m_hBadStatus = 0;
    setMapEnemyFlag(0, 0x10);
}
void CMapEnemy::beginStateStop() {
    setMapEnemyFlag(0x20, 0);
    m_hBadStatus = setBadStatusEffect(MapEnemy::BadStatusEffectName, 4, 0.0f);
    setAnimSpeed(0.0f);
    setTimer(getStopTime());
}

void CMapEnemy::stateStop() {
    if (getTimer() > 0) return;
    setMapEnemyFlag(0, 0x20);
    if (isSleeping()) {
        changeMapEnemyState(2);
    } else if (isStanding()) {
        changeMapEnemyState(3);
    } else {
        changeMapEnemyState(1);
    }
}

void CMapEnemy::endStateStop() {
    CEffectMan::killEffect(m_hBadStatus, 0.0f);
    m_hBadStatus = 0;
    setMapEnemyFlag(0, 0x20);
    setAnimSpeed(1.0f);
}

void CMapEnemy::beginStateConfuse() {
    setMapEnemyFlag(0x80, 0);
    m_hBadStatus = setBadStatusEffect(MapEnemy::BadStatusEffectName, 2, 0.0f);
    setTimer(getConfuseTime());
    changeMapEnemyMotion(0, 0, 0, -1);
}

void CMapEnemy::stateConfuse() {
    if (getTimer() > 0) return;
    if (isSleeping()) {
        changeMapEnemyState(2);
    } else if (isStanding()) {
        changeMapEnemyState(3);
    } else {
        changeMapEnemyState(1);
    }
}

void CMapEnemy::endStateConfuse() {
    CEffectMan::killEffect(m_hBadStatus, 0.0f);
    m_hBadStatus = 0;
    setMapEnemyFlag(0, 0x80);
}

void CMapEnemy::beginStateDisappear() {
    m_iSysStep = 0;
    bodyCollOff(-1);
    bodyCollOffBody(-1);
    setEntityFlag(0, 0x10000);
    if (m_pEnemyParam && m_pEnemyParam->iDisappearMotionIndex != -1) {
        changeMotion(m_pEnemyParam->iDisappearMotionIndex, 0, 0, -1, 0, 0);
        m_iSysStep = 0;
    } else {
        m_iSysStep = 1;
    }
    setEntityFlag(0x40000000, 0);
    setupFade(1.0f, 0.0f, 3000, -1);
    setDisappearEffect();
    setMapEnemyFlag(0, 0x40);
}

void CMapEnemy::stateDisappear() {
    switch (m_iSysStep) {
    case 0:
        if (isMotionEnd()) changeMapEnemyState(0);
        break;
    case 1:
        if (!isFade()) changeMapEnemyState(0);
        break;
    }
}

void CMapEnemy::endStateDisappear() {
    warp(m_vStartPos);
    updateCoord();
    coordFix();
}
void CMapEnemy::groundEnemyProc_() {
    if (calcDistanceSq(g_pPlayer) > getDisappearDistance()) {
        changeMapEnemyState(8);
        return;
    }
    switch (m_iSysStep) {
    case 0: {
        if (RND::genrand_int31() % 40) break;
        TIMEQ exeTime = (RND::genrand_int31() % 4 + 3) * 3000;
        if (canPlayerChase(1.5f, TRUE)) {
            changeMapEnemyMotion(2, 0, 0, -1);
            setTimer(exeTime);
            m_iSysStep = 2;
        } else {
            getRandomTargetPos(m_vTarget, RND::genrand_real1() * 5.0f + 5.0f, TRUE);
            changeMapEnemyMotion(1, 0, 0, -1);
            setTimer(exeTime);
            m_iSysStep = 1;
        }
        break;
    }
    case 1:
        m_pMove->homingMoveXZ(m_vTarget, 0.1f, m_fWalkSpeed);
        if (arriveTarget(m_vTarget, 1.5f) || !checkGround()) {
            changeMapEnemyMotion(0, 0, 0, -1);
            clearSpeed();
            m_iSysStep = 0;
        }
        break;
    case 2:
        m_pMove->homingMoveXZ(g_pPlayer, 0.1f, m_fRunSpeed);
        if (getTimer() <= 0) {
            changeMapEnemyMotion(0, 0, 0, -1);
            clearSpeed();
            m_iSysStep = 0;
        } else if (!canPlayerChase(1.5f, TRUE) || !checkGround()) {
            changeMapEnemyState(8);
        }
        break;
    }
}

void CMapEnemy::flyEnemyProc_() {
    if (!isLoomingDarkness()) {
        if (calcDistanceSq(g_pPlayer) > getDisappearDistance()) {
            changeMapEnemyState(8);
            return;
        }
        switch (m_iSysStep) {
        case 0: {
            if (RND::genrand_int31() % 40) break;
            TIMEQ exeTime = (RND::genrand_int31() % 4 + 3) * 3000;
            if (canPlayerChase(5.0f, FALSE) && !(RND::genrand_int31() % 10)) {
                changeMapEnemyMotion(2, 0, 0, -1);
                setTimer(exeTime);
                m_iSysStep = 2;
            } else {
                getRandomTargetPos(m_vTarget, RND::genrand_real1() * 5.0f + 5.0f, FALSE);
                changeMapEnemyMotion(1, 0, 0, -1);
                setTimer(exeTime);
                m_iSysStep = 1;
            }
            break;
        }
        case 1:
            m_pMove->homingMove(m_vTarget, 0.1f, m_fWalkSpeed);
            if (arriveTarget(m_vTarget, 1.5f)) {
                changeMapEnemyMotion(0, 0, 0, -1);
                clearSpeed();
                m_iSysStep = 0;
            }
            break;
        case 2:
            g_pPlayer->getLockOnPos(m_vTarget);
            m_pMove->homingMove(m_vTarget, 0.1f, m_fRunSpeed);
            if (getTimer() <= 0) {
                changeMapEnemyMotion(0, 0, 0, -1);
                clearSpeed();
                m_iSysStep = 0;
            } else if (calcDistanceSq(g_pPlayer) > getDisappearDistance()) {
                changeMapEnemyState(8);
            }
            break;
        }
    } else {
        switch (m_iSysStep) {
        case 0:
            g_pPlayer->getLockOnPos(m_vTarget);
        case 2:
            m_pMove->homingMove(m_vTarget, 0.1f, m_fWalkSpeed);
            if (arriveTarget(m_vTarget, 0.5f)) {
                if (!(RND::genrand_int31() % 10)) {
                    m_vTarget = getPlayerPos(FALSE);
                    m_iSysStep = 2;
                    m_vTarget.y += 1.0f;
                } else {
                    changeMapEnemyMotion(0, 0, 0, -1);
                    clearSpeed();
                    setTimer(9000);
                    m_iSysStep = 1;
                }
            }
            break;
        case 1:
            m_pMove->homingY(g_pPlayer, 0.1f);
            if (calcDistanceXZSq(g_pPlayer) > 12.25f) {
                changeMapEnemyMotion(2, 0, 0, -1);
                m_iSysStep = 0;
            } else if (getTimer() <= 0) {
                getRandomTargetPos(m_vTarget, RND::genrand_real1() + 2.0f, FALSE);
                FLOAT r = RND::genrand_real1();
                m_vTarget.y = m_vTarget.y + ((RND::genrand_int31() & 1) ? r : r * -1.0f);
                changeMapEnemyMotion(2, 0, 0, -1);
                m_iSysStep = 2;
            }
            break;
        }
        if (calcDistanceSq(g_pPlayer) > getDisappearDistance()) changeMapEnemyState(8);
    }
}
/* near-match (the vol store lands in a delay slot): kept as INCLUDE_ASM */
#if 0
BOOL CMapEnemy::hitTest_() {
    if (!m_pBodyColl) return FALSE;
    m_pBodyColl->detect();
    if (!m_pBodyColl->isHit()) return FALSE;
    if (isCurrentState(4)) return FALSE;
    if (isCurrentState(8)) return FALSE;
#line 1861
    ASSERT(m_pEnemyParam);
    INT no = RND::genrand_int31() % 2 + 1;
    CCollBody* hit = m_pBodyColl->getHitCollBody();
#line 1864
    ASSERT(hit);
    INT vol = -1;
    INT pan = CSound::getPanAndVol(*hit->getDetectPos(), vol);
    INT nID = (INT)m_pEnemyParam->getExtraParam(MapEnemy::FileIDParamName);
    CSound::playSE(nID * 1000 + no, vol, pan);
    hit->setHitEffect();
    switch (hit->getDamageKind()) {
    case 0x1c:
    case 0x1d:
    case 0x1e:
    case 0x24:
    case 0x25:
    case 0x26:
        changeMapEnemyState(5);
        return TRUE;
    case 7:
    case 0x2a:
        changeMapEnemyState(7);
        return TRUE;
    case 8:
        changeMapEnemyState(6);
        return TRUE;
    case 0x29:
        break;
    default:
        if (DebugWork::g_dw.enemy.uiFlag & 0x40) return TRUE;
        changeMapEnemyState(4);
        break;
    }
    return TRUE;
}
#else
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Game/Enemy/MapEnemy", hitTest___9CMapEnemy);
__asm__(
    ".rdata\n"
    ".align 4\n"
    ".word .L002D1820\n"
    ".word .L002D1834\n"
    ".word .L002D1848\n"
    ".word .L002D1848\n"
    ".word .L002D1848\n"
    ".word .L002D1848\n"
    ".word .L002D1848\n"
    ".word .L002D1848\n"
    ".word .L002D1848\n"
    ".word .L002D1848\n"
    ".word .L002D1848\n"
    ".word .L002D1848\n"
    ".word .L002D1848\n"
    ".word .L002D1848\n"
    ".word .L002D1848\n"
    ".word .L002D1848\n"
    ".word .L002D1848\n"
    ".word .L002D1848\n"
    ".word .L002D1848\n"
    ".word .L002D1848\n"
    ".word .L002D1848\n"
    ".word .L002D180C\n"
    ".word .L002D180C\n"
    ".word .L002D180C\n"
    ".word .L002D1848\n"
    ".word .L002D1848\n"
    ".word .L002D1848\n"
    ".word .L002D1848\n"
    ".word .L002D1848\n"
    ".word .L002D180C\n"
    ".word .L002D180C\n"
    ".word .L002D180C\n"
    ".word .L002D1848\n"
    ".word .L002D1848\n"
    ".word .L002D1868\n"
    ".word .L002D1820\n"
    ".text\n");
#endif

/* near-match (inRadius and the end angle in swapped FP registers): kept as INCLUDE_ASM */
#if 0
BOOL CMapEnemy::searchGround_(FLOAT inRadius, FVECTOR* outPos) {
    CPointCollision coll;
    FVECTOR pos;
    for (FLOAT deg = (FLOAT)(rand() % 360), d = deg + 360.0f; deg < d; deg += 45.0f) {
        FLOAT rad = Math3D::LimitPI(deg * 0.017453292f);
        pos = getPlayerPos(FALSE);
        pos.x += inRadius * cosf(rad);
        pos.y += 0.1f;
        pos.z += inRadius * sinf(rad);
        coll.setEndPos(pos);
        coll.setStartPos(pos);
        coll.m_inWE.y -= 1.5f;
        if (coll.collideMap()) {
            if (outPos) *outPos = coll.m_outWE;
            return TRUE;
        }
    }
    return FALSE;
}
#else
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Game/Enemy/MapEnemy", searchGround___9CMapEnemyfP7FVECTOR);
#endif
BOOL CMapEnemy::initEnemyParam_(INT inGroupID, INT inRsrcID) {
    ENEMY_PARAM* param = (ENEMY_PARAM*)CRsrcDB::getFirstData(0x15, inGroupID * 10000 + inRsrcID, 1);
    if (!param) return FALSE;
    if (!_setupEnemyParam(param)) return FALSE;
    m_pEnemyParam = param;
    return TRUE;
}

BOOL CMapEnemy::checkEncont_() {
    if (DebugWork::g_dw.enemy.uiFlag & 0x40) return FALSE;
    if (isFaint()) {
        if (isHpHalf()) return encount_(3);
        return encount_(2);
    }
    if (isStop()) {
        if (isHpHalf()) return encount_(5);
        return encount_(4);
    }
    if (isSleeping()) {
        if (isHpHalf()) return encount_(9);
        return encount_(8);
    }
    if (isConfuse()) {
        if (isHpHalf()) return encount_(7);
        return encount_(6);
    }
    if (isHpHalf()) return encount_(1);
    return encount_(0);
}

BOOL CMapEnemy::encount_(UBYTE inAttackType) {
    if (g_pPlayer->m_bIsDead) return FALSE;
    CMapEnemyManager::setEncountEnemy((UINT)getID() % 10000, getUniqueID(), inAttackType, m_iAppearType);
    if (CSceneInfo::reqEnterBattle()) {
        g_pPlayer->cmdState_setEncountWait();
        return TRUE;
    }
    return FALSE;
}
// SOURCE DATA: .data .rodata
