
#define SYSTEM_GRAPHIC_ANIM_H /* MATCH: the original's includes don't bring in Anim.h (no "2 > idx" in .rodata) */
#include "System/Graphic/modeldata.h"
#include "Game/BodyCollision.h"
#include "Game/Gimmick/SampleGimmick.h"
#include "System/Graphic/MapUnit.h"

#include "Game/Movement.h"

MAPUNITTABLE g_DefaultGimmick = {"0000", NewClass<CSampleGimmickForEffectID>, 17};

CSampleGimmick::CSampleGimmick(CTask* inParent) : CBaseGimmick(inParent) {}

CSampleGimmick::~CSampleGimmick() {}

BOOL CSampleGimmick::initalize(INT inGroupID, INT inRsrcID) {
    if (!CBaseGimmick::initalize(inGroupID, inRsrcID)) return FALSE;
    m_pMove->enableGravity(FALSE);
    m_pMove->setFlag(1, 0);
    m_pMove->setCollisionFlag(0, 0xF);
    rideModelChangeListR2();
    return TRUE;
}

void CSampleGimmick::beginStateMain() {
    if (getParam()) {
        changeMotion(getParam(), 0, FALSE, 0, 0, 0);
    } else {
        changeMotion(1, 0, FALSE, 0, 0, 0);
    }
}

void CSampleGimmick::stateMain() {}

void CSampleGimmick::endStateMain() {}

void CSampleGimmick::beginStateHitAttack() {}

void CSampleGimmick::stateHitAttack() {}

void CSampleGimmick::endStateHitAttack() {}

CSampleGimmickForEffectID::CSampleGimmickForEffectID(CTask* inParent, const ZONEPARTS& inParts)
    : CBaseGimmick(inParent, inParts) {
    changeMotion(1, 0, FALSE, -1, 0, 0);
}

CSampleGimmickForEffectID::~CSampleGimmickForEffectID() {}

void CSampleGimmickForEffectID::beginStateMain() {}

void CSampleGimmickForEffectID::stateMain() {}

void CSampleGimmickForEffectID::endStateMain() {}

void CSampleGimmickForEffectID::beginStateHitAttack() {}

void CSampleGimmickForEffectID::stateHitAttack() {}

void CSampleGimmickForEffectID::endStateHitAttack() {}
// SOURCE DATA: .rodata .bss
// SOURCE DATA SLUS_217.99_retail: .bss .rodata
