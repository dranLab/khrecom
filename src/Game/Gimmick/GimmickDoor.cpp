#define GPRIM_H_PATH "System/Graphic/GPrim.h"
#include "System/Task.h"
#include "System/Graphic/modeldata.h"
#include "Game/Gimmick/GimmickDoor.h"
#include "System/Graphic/Gprim.h"
#include "System/Graphic/2d/2DMan.h"
#include "Game/CardBattle.h"
#include "System/Graphic/Str.h"
#include "Game/Collision/CollUtil.h"

/* Placeholder: an empty string follows CollUtil.h's strings in .rodata (as in GimmickPlantParent); which header it
 * comes from is not known. */
inline void _emptyStringPlaceholder0() {
    TRACE("");
}

#include "System/Graphic/Effect/effect.h"
#include "System/Graphic/Effect/EffectMan.h"
#include "System/Graphic/MapUnit.h"
#include "Game/Map.h"
#include "Game/Movement.h"
#include "Game/GameWork.h"
#include "Game/LockOnManager.h"
#include "Game/Player/Player.h"
#include "System/TaskMan.h"

#define DOOR_WORLD(w)                                                                                        \
    {                                                                                                        \
        {#w "00", NewClass<CGimmickDoor>, 17}, {#w "01", NewClass<CGimmickDoor>, 17},                        \
            {#w "02", NewClass<CGimmickDoor>, 17}, {#w "40", NewClass<CGimmickDoor>, 17},                    \
            {#w "41", NewClass<CGimmickDoor>, 17}, {#w "42", NewClass<CGimmickDoor>, 17},                    \
            {#w "43", NewClass<CGimmickDoor>, 17}, {#w "44", NewClass<CGimmickDoor>, 17},                    \
            {#w "45", NewClass<CGimmickDoor>, 17}, {#w "46", NewClass<CGimmickDoor>, 17},                    \
            {#w "47", NewClass<CGimmickDoor>, 17}, {#w "48", NewClass<CGimmickDoor>, 17},                    \
    }

MAPUNITTABLE g_DoorWorld1[12] = DOOR_WORLD(01);
MAPUNITTABLE g_DoorWorld2[12] = DOOR_WORLD(02);
MAPUNITTABLE g_DoorWorld3[12] = DOOR_WORLD(03);
MAPUNITTABLE g_DoorWorld4[12] = DOOR_WORLD(04);
MAPUNITTABLE g_DoorWorld5[12] = DOOR_WORLD(05);
MAPUNITTABLE g_DoorWorld6[12] = DOOR_WORLD(06);
MAPUNITTABLE g_DoorWorld7[12] = DOOR_WORLD(07);
MAPUNITTABLE g_DoorWorld8[12] = DOOR_WORLD(08);
MAPUNITTABLE g_DoorWorld9[12] = DOOR_WORLD(09);
MAPUNITTABLE g_DoorWorld10[12] = DOOR_WORLD(10);
MAPUNITTABLE g_DoorWorld11[12] = DOOR_WORLD(11);
MAPUNITTABLE g_DoorWorld12[12] = DOOR_WORLD(12);
MAPUNITTABLE g_DoorWorld13[12] = DOOR_WORLD(13);
MAPUNITTABLE g_DoorWorld0[12] = DOOR_WORLD(14);

static inline void _unitVectorVu0(FVECTOR& v) {
    /* no "memory" clobber: a bare volatile asm makes cse2 flush its tables (keeps a hoisted lui) */
    asm volatile("sqc2 $vf0, 0x0(%0)" : : "r"(&v));
}

static inline BOOL isSpecialRoom(INT inRoomID) { return inRoomID == 0 || inRoomID == 99; }

static inline LockOnManager::CLockOnManager* getLockOnMan(CTask* inTask) {
    return (LockOnManager::CLockOnManager*)inTask->getSysTask(32);
}

CGimmickDoor::CGimmickDoor(CTask* inParent, const ZONEPARTS& inParts) : CBaseGimmick(inParent) {
    BOOL bCancel = FALSE;
    INT rsrcid;
    DoorInfo* info;

    setEntityType((ObjType::eObjType)7);
    m_iStep = 0;
    m_hEffect[1] = 0;
    m_hEffect[0] = 0;

    /* A single-pass loop: the original's (rotated) loop test sets bCancel inside the loop on every failed check,
     * which gives bCancel a register (more weighted references). */
#define PASS_OR_CANCEL(cond) ((cond) || (bCancel = TRUE, FALSE))
    while (PASS_OR_CANCEL(CSceneInfo::isFieldScene())
           && PASS_OR_CANCEL((rsrcid = effectIDtoI(inParts.effectId)) >= 0)
           && PASS_OR_CANCEL((info = CSceneInfo::getDoorInfo(m_DoorID = (DOOR_ID)inParts.effectParam, -1)) != NULL)
           && PASS_OR_CANCEL((m_DoorState = info->getDoorState()) != DOOR_STAT_NOTHING)) {
        switch (info->_id) {
        case DOOR_ID_RU: setEntityType((ObjType::eObjType)7); break;
        case DOOR_ID_RD: setEntityType((ObjType::eObjType)8); break;
        case DOOR_ID_LD: setEntityType((ObjType::eObjType)9); break;
        case DOOR_ID_LU: setEntityType((ObjType::eObjType)10); break;
        }
        if (info->_shape == 1) {
            switch (rsrcid % 100) {
            case 40: rsrcid = rsrcid / 100 * 100 + 42; break;
            case 41: rsrcid = rsrcid / 100 * 100 + 43; break;
            case 46: rsrcid = rsrcid / 100 * 100 + 47; break;
            default: rsrcid = rsrcid / 100 * 100 + 1; break;
            }
        } else if (info->_type == DOOR_TYPE_WORLD_ENTRANCE) {
            switch (rsrcid % 100) {
            case 40: rsrcid = rsrcid / 100 * 100 + 44; break;
            case 41: rsrcid = rsrcid / 100 * 100 + 45; break;
            case 46: rsrcid = rsrcid / 100 * 100 + 48; break;
            default: rsrcid = rsrcid / 100 * 100 + 2; break;
            }
        }
        if (!CBaseGimmick::initalize(11, rsrcid)) {
            TRACE("Gimmick initalize failed ... %d\n", rsrcid);
            bCancel = TRUE;
        }
        break;
    }
#undef PASS_OR_CANCEL

    if (bCancel) {
        out();
        return;
    }

    makeCoord(inParts);
    switch (m_DoorState) {
    case DOOR_STAT_CLOSED:
        changeMotion(1, 0, FALSE, -1, 0, 0);
        { LockOnManager::CLockOnManager* pLockOn = getLockOnMan(this); pLockOn->registerLockOnObject(getHandle()); }
        m_iStep = 1;
        break;
    case DOOR_STAT_OPENED:
        changeMotion(3, 0, FALSE, -1, 0, 0);
        if (!info || (!info->isEventDoor() && info->_type != DOOR_TYPE_TO_EXTRA)) {
            if (!isSpecialRoom(CSceneInfo::getCurrentRoomID())) {
                { LockOnManager::CLockOnManager* pLockOn = getLockOnMan(this); pLockOn->registerLockOnObject(getHandle()); }
            } else {
                rideModelEnableChipCollision(0, FALSE);
                rideModelEnableChipCollision(1, TRUE);
            }
        }
        break;
    case DOOR_STAT_ONE_WAY:
    case DOOR_STAT_UNAVAILABLE:
        setEntityFlag(1, 0);
        rideModelEnableAllCollision(FALSE);
        bodyCollOffBody(-1);
        m_iStep = 1;
        break;
    }

    if (info->_shape == 1) {
        if (m_DoorState == DOOR_STAT_CLOSED) {
            FVECTOR vRot[2];
            _unitVectorVu0(vRot[0]);
            vRot[1].set(0.0f, 3.1415927f, 0.0f, 1.0f);
            for (INT i = 0; i < 2; i++) {
                CEffect* pEff;
                m_hEffect[i] = CEffectMan::attachEffect("rec_mon_he.dpx", 4, (ObjType::eObjType)getEntityType(),
                                                        &Crd(0), NULL, &vRot[i], 0.0f);
                pEff = CEffectMan::getEffect(m_hEffect[i]);
                if (pEff) pEff->setZSortEntity(this, 1, TRUE);
            }
        }
        if (m_DoorState == DOOR_STAT_OPENED) {
            rideModelEnableChipCollision(0, FALSE);
            rideModelEnableChipCollision(1, TRUE);
        }
    }

    if (info->_shape == 4
        || (CSceneInfo::getCurrentRoomID() != 0 && CSceneInfo::getCurrentRoomID() != 99 && info->_shape == 3)
        || (CSceneInfo::getCurrentWorldID() == 13 && info->_id == 0 && CSceneInfo::getCurrentRoomID() == 19)) {
        setEntityFlag(0, 0x10000);
        rideModelEnableAllCollision(TRUE);
    }

    if (((CSceneInfo::getCurrentRoomID() == 0 || CSceneInfo::getCurrentRoomID() == 99
          || CSceneInfo::getCurrentWorldID() == 0)
         && rsrcid == 1400)
        || (CSceneInfo::getCurrentWorldID() == 10 && CSceneInfo::getCurrentRoomID() != 0
            && CSceneInfo::getCurrentRoomID() != 99)
        || (CSceneInfo::getCurrentWorldID() == 13 && info->_id == 2 && CSceneInfo::getCurrentSceneNum() == 90)) {
        setEntityFlag(0, 0x10000);
        setEntityFlag(1, 0);
        rideModelEnableAllCollision(FALSE);
        bodyCollOffBody(-1);
        m_iStep = 1;
    }

    if (CSceneInfo::getCurrentWorldID() == 13 && info->_id == 0 && CSceneInfo::getCurrentSceneNum() == 90) {
        setEntityFlag(0, 0x10000);
    }

    TRACE("\x83\x68\x83\x41\x81\x46\x83\x8a\x83\x5c\x81\x5b\x83\x58:%d\n", rsrcid);
    m_pMove->enableGravity(FALSE);
    m_pMove->setFlag(1, 0);
    m_pMove->setCollisionFlag(0, 0xF);
    m_pMove->setFlag(8, 0);
    enableFade2Path(TRUE, -1);
    changeGimmickState(GM_STATE_MAIN);
}

CGimmickDoor::~CGimmickDoor() {
    for (INT i = 0; i < 2; i++) {
        CEffect* pEff = CEffectMan::getEffect(m_hEffect[i]);
        if (pEff) pEff->kill();
    }
}

INT CGimmickDoor::getTypeID() const {
    return !CGameWork::getFlag(1, 0x24) ? 0x1006 : 0x1005;
}

void CGimmickDoor::beginStateMain() {}

void CGimmickDoor::stateMain() {
    if (CGameWork::getFlag(1, 0x20)) return;
    if (CSceneInfo::getCurrentWorldID() == 10) return;
    if (CSceneInfo::getCurrentRoomID() == 0) return;
    if (CSceneInfo::getCurrentRoomID() == 99) return;
    if (m_iStep != 0) return;
    if (CTaskMan::getSysTask(0x1D)) return;
    if (g_pPlayer && g_pPlayer->getPlayerState() != DEAD) {
        FLOAT y = g_pPlayer->getPos().y - getPos().y;
        if (y <= 3.0f && -0.5f <= y) {
            if (calcDistanceXZ(*g_pPlayer) <= 1.0f && CSceneInfo::reqEnterDoor(m_DoorID)) {
                m_iStep++;
            }
        }
    }
}

void CGimmickDoor::endStateMain() {}

void CGimmickDoor::beginStateHitAttack() {}

void CGimmickDoor::stateHitAttack() {}

void CGimmickDoor::endStateHitAttack() {}

void CGimmickDoor::killEffectCrown() {
    for (INT i = 0; i < 2; i++) {
        CEffect* eff = CEffectMan::getEffect(m_hEffect[i]);
        if (eff) eff->fadeout(8.0f);
        m_hEffect[i] = 0;
    }
}

void CGimmickDoor::setLockOnMarker(BOOL in_flag) {
    if (in_flag) setEntityFlag(0x10000, 0);
    else setEntityFlag(0, 0x10000);
}
// SOURCE DATA: .rodata .bss
