#define GPRIM_H_PATH "System/Graphic/GPrim.h" /* MATCH: this unit names Gprim.h so (header default "System/Graphic/Gprim.h") */
#include "System/Task.h"
#include "System/TaskMan.h"
#include "Game/Player/SummonGenie.h"
#include "System/Graphic/Gprim.h"
#include "System/Graphic/2d/2DMan.h"
#include "Game/CardBattle.h"
#include "System/Graphic/Str.h"
#include "Game/Collision/CollUtil.h"

/* MATCH: Placeholders: three empty strings follow CollUtil.h's strings in .rodata (as in FriendAladdin). */
inline void _emptyStringPlaceholder0() {
    TRACE("");
}
inline void _emptyStringPlaceholder1() { /* MATCH: placeholder: an inline function's strings, in the original .rodata order */
    TRACE("");
}
inline void _emptyStringPlaceholder2() { /* MATCH: placeholder: an inline function's strings, in the original .rodata order */
    TRACE("");
}

#include "System/Graphic/Effect/effect.h"
#include "System/Graphic/Effect/EffectMan.h"
#include "Game/Player/Player.h"
#include "Game/Player/PlayerCamera.h"
#include "Game/Player/PlayerAttack.h"
#include "Game/Movement.h"
#include "Game/Bullet/Bullet.h"
#include "Game/EntityFactory.h"
#include "System/Rnd.h"
#include "include_asm.h"

namespace PL {
/* Game/Player/PlayerAttack.h. Declared const here: setMagic's schedule needs the effect-name loads to be
 * known not to alias the stack stores (read-only memory), as in the original. */
extern const BULLET_EFFECT_INFO g_EffTbl_Skill[67][4];
extern FVECTOR g_RotTbl_Magic[8];
}

namespace Summon {

static LPCSTR g_szDpxSummon = "summon";
static LPCSTR g_szDpxGenieStEd = "genie_st_ed";

unsigned char g_GenieMagicID[4] = { 140, 141, 142, 0 };

unsigned char g_GenieMagicSkillID[4] = { 22, 24, 25, 0 };

BCAM_PARAM g_GenieCamParam = {
    { 55.0f, 0.05f, 0.2f, 0.0f, { { 0.0f, 0.9f, 4.0f, 0.0f } }, { { 0.0f, 0.1f, 0.0f, 0.0f } } },
    { 55.0f, 0.05f, 0.5f, 16.0f, { { 1.0f, 0.5f, 4.2f, 0.0f } }, { { 0.0f, 0.9f, 1.0f, 0.0f } } },
};

CSummonGenie::CSummonGenie(CTask* inParent) : CSummon(inParent, 2) {
    m_fCamHeight = 4.0f;
}

CSummonGenie::~CSummonGenie() {
    PlayerCamera::CPlayerCamera::setBattleCamData(0, 0);
}

void CSummonGenie::beginStateAppear() {
    BULLET_PARAM param;
    FVECTOR pos;
    CEffect* effect;
    HANDLE handle;

    m_iState = 0;
    m_uiFlag |= 8;
    m_pMove->enableGravity(0);
    setPos(m_vStartPos);
    warp(m_vStartPos);
    updateCoord();
    coordFix();
    m_uiAttackID = g_GenieMagicID[0];
    setEntityFlag(0, 1);
    m_uiStartAnimID = 2;
    m_tqTrgFrame = 6000;
    changeMotion(2, 0, 1, 0, 0, 0, 0);
    pos = getPos();
    pos.y = pos.y + 0.5;
    handle = setEffect(g_szDpxGenieStEd, 0, &pos, &getRot(), 0.0f);
    if (handle) {
        effect = CEffectMan::getEffect(handle);
        if (effect) {
            effect->setAttachNoRot(1);
            effect->scale().set(1.7f, 1.7f, 1.7f);
            effect->setZSortEntity(this, 2, 1);
        }
    }
    handle = setEffect(g_szDpxSummon, 0, &getPos(), 0, 0.0f);
    if (handle) {
        effect = CEffectMan::getEffect(handle);
        if (effect) {
            effect->setAttachNoRot(1);
            effect->scale().set(1.5f, 1.5f, 1.5f);
            effect->setZSortEntity(this, 3, 1);
        }
    }
    bodyCollOnBody(-1);
    clearSpeed();
}

void CSummonGenie::stateAppear() {
    if (m_fScale < 1.0f) {
        m_fScale += getFrameRateF() * 0.05f;
        if (m_fScale > 1.0f) m_fScale = 1.0f;
        FVECTOR scale = getScale();
        scale.z = m_fScale;
        scale.x = m_fScale;
        scale.y = m_fScale * 0.5f + 0.5f;
        setScale(scale);
        PlayerCamera::CPlayerCamera::setBattleCamData(getHandle(), &g_GenieCamParam);
        homingPlayerTargetY(0, 0.1f);
        m_hLockon = m_hTarget;
        m_pPlayer->getPlayerCamera()->changeNowPlayer(m_hLockon, m_pPlayer->getHandle());
        m_pPlayer->getPlayerCamera()->changeCamMode(4, 1);
    } else if (getMotionCurrentTime() >= m_tqTrgFrame) {
        homingPlayerTargetY(0, 0.1f);
        changeState(1);
    }
}

INT CSummonGenie::setSkill(INT inID) {
    if (m_iState != 1) return 0;
    m_iAttackNo = 0;
    setTimer(0);
    switch (inID) {
    case 123:
        m_uiSkillID = 4;
        m_uiLevel = 0;
        changeState(2);
        break;
    case 138:
        m_uiLevel = 1;
        m_uiSkillID = 5;
        changeState(2);
        break;
    case 139:
        m_uiSkillID = 6;
        m_uiLevel = 2;
        changeState(2);
        break;
    default:
        setExit(0);
        break;
    }
    return 1;
}

void CSummonGenie::beginStateAction1() {
    m_iState = 2;
    INT r = RND::genrand_int31() % 3;
    m_uiKind = r;
    m_uiAttackID = g_GenieMagicID[m_uiKind];
    SS_PARAM*& ss = CardBattle::g_pStockSkillList;
    m_AtkAttr = (ATK_ATTR)ss[g_GenieMagicSkillID[m_uiKind]].ATTR;
    m_tqTrgFrame = 0;
    m_iSubState = m_uiKind + 1;
    m_uiAnimID = r + 4;
    homingPlayerTargetY(0, 0.1f);
}

void CSummonGenie::stateAction1() {
    if (exeGenieMagic()) setExit(0);
}

void CSummonGenie::stateExit() {
    if (m_iSubState > 0) {
        if (getTimer() <= 0) out();
        return;
    }
    if (m_fScale > 0.0f) {
        m_fScale -= getFrameRateF() * 0.05f;
        if (m_fScale <= 0.0f) {
            m_fScale = 0.0f;
            m_iSubState = 1;
            resetLockon();
            TIMEQ inTime = m_tqSubTimer;
            setTimer(inTime);
            killChild(inTime * 0.02f);
            setEntityFlag(1, 0);
            m_pPlayer->setPlayerFlag(0x8000000, 0);
        } else {
            FVECTOR scale = getScale();
            scale.z = m_fScale;
            scale.x = m_fScale;
            scale.y = m_fScale * 0.5f + 0.5f;
            setScale(scale);
        }
    } else {
        out();
    }
}

INT CSummonGenie::exeGenieMagic() {
    switch (m_iSubState) {
    case 1:
        changeMotion(4, 0, 1, -1, 0, 0, 0);
        m_iSubState = 6;
        setTimer(1000);
        homingPlayerTargetY(0, 0.1f);
        break;
    case 2:
        changeMotion(5, 0, 1, -1, 0, 0, 0);
        m_iSubState = 6;
        setTimer(1000);
        homingPlayerTargetY(0, 0.1f);
        break;
    case 3:
        changeMotion(6, 0, 1, -1, 0, 0, 0);
        m_iSubState = 6;
        setTimer(1000);
        homingPlayerTargetY(0, 0.1f);
        break;
    case 6:
        homingPlayerTargetY(0, 0.1f);
        if (getTimer() > 0) break;
        m_iSubState = 4;
        setMagic(m_uiAttackID);
        setTimer(1000);
        break;
    case 4:
        homingPlayerTargetY(0, 0.1f);
        if (getTimer() > 0) break;
        m_uiAnimID = 1;
        if (m_iAttackNo < m_uiLevel) {
            m_iAttackNo++;
            setTimer(1300);
            changeMotion(1, 0, 1, -1, 0, 0, 0);
            m_iSubState = 8;
            CEntity* pTarget = m_hTarget > 0 ? (CEntity*)CTaskMan::getTask(m_hTarget) : 0;
            if (!pTarget) {
                homingPlayerTargetY(0, 0.1f);
                m_hLockon = m_hTarget;
                m_pPlayer->getPlayerCamera()->changeNowPlayer(m_hLockon, m_pPlayer->getHandle());
                m_pPlayer->getPlayerCamera()->changeCamMode(4, 0);
            }
        } else {
            changeMotion(1, 0, 1, -1, 0, 0, 0);
            m_iSubState = 5;
            m_iAttackNo = 0;
            setTimer(2500);
        }
        break;
    case 5: {
        if (getTimer() > 0) break;
        changeMotion(3, 0, 1, -1, 0, 0, 0);
        setTimer(2000);
        FVECTOR pos = getPos();
        pos.y = pos.y + 0.5;
        HANDLE handle = setEffect(g_szDpxGenieStEd, 1, &pos, &getRot(), 10.0f);
        if (handle) {
            CEffect* effect = CEffectMan::getEffect(handle);
            if (effect) effect->scale().set(1.7f, 1.7f, 1.7f);
        }
        m_iSubState = 7;
        break;
    }
    case 7:
        if (getTimer() > 0) break;
        return 1;
    case 8:
        homingPlayerTargetY(0, 0.1f);
        if (getTimer() > 0) break;
        changeState(2);
        break;
    }
    return 0;
}

void CSummonGenie::setMagic(INT inID) {
    FVECTOR pos;
    FVECTOR rot;
    BULLET_PARAM param;
    PATK_DATA* pAtkData = &PL::g_AttackDataList[m_uiAttackID];
    CBullet* bullet;
    CEntity* pTarget = m_hTarget > 0 ? (CEntity*)CTaskMan::getTask(m_hTarget) : 0;

    switch (inID) {
    case 140:
        if (pTarget) {
            pTarget->getLockOnPos(pos);
            pos.y = getPos().y;
        } else {
            pos = getPos();
        }
        rot = getRot();
        setEffect(PL::g_EffTbl_Skill[pAtkData->EFFECT][1].pEffectName, 3, &pos, &rot, 0.0f);
        pos.y += 6.0f;
        param.init(1, 3);
        param.CollKind = 0x80000000;
        param.uiFlag = 0x47;
        param.AtkKind = pAtkData->DAMAGE;
        param.AtkAttr = m_AtkAttr;
        param.iAttackPoint = getPower(m_uiSkillID, m_uiKind) / 10;
        CardBattle::CCardBattle::calcEnchant_AttackPoint(param.iAttackPoint, (ATK_KIND&)param.AtkKind,
                                                         (ATK_ATTR)param.AtkAttr, m_uiUpKind);
        rot.x = 1.5707964f;
        param.fSpeed = pAtkData->MAX_SPEED * 0.01f;
        param.fMaxDist = pAtkData->WORK0;
        param.tqHomingTime = 800;
        param.vPos = pos;
        param.vRot = rot;
        param.pEffectTbl = (BULLET_EFFECT_INFO*)PL::g_EffTbl_Skill[pAtkData->EFFECT];
        param.fCollRadius = pAtkData->COLL_START_FRAME * 0.1f;
        CEntityFactory::getInstance()->createBullet(param, this);
        break;
    case 141:
        if (pTarget) {
            pTarget->getLockOnPos(pos);
            pos.y = getPos().y;
        } else {
            pos = getPos();
        }
        pos.y += 3.0f;
        rot = getRot();
        setEffect(PL::g_EffTbl_Skill[pAtkData->EFFECT][2].pEffectName, 8, &pos, &rot, 0.0f);
        param.init(2, 3);
        param.CollKind = 0x80000000;
        param.uiFlag = 0x41E1;
        param.AtkKind = pAtkData->DAMAGE;
        param.AtkAttr = m_AtkAttr;
        param.iAttackPoint = getPower(m_uiSkillID, m_uiKind);
        CardBattle::CCardBattle::calcEnchant_AttackPoint(param.iAttackPoint, (ATK_KIND&)param.AtkKind,
                                                         (ATK_ATTR)param.AtkAttr, m_uiUpKind);
        rot.x = 1.5707964f;
        param.fSpeed = pAtkData->MAX_SPEED * 0.01f;
        param.fMaxDist = pAtkData->WORK0;
        param.tqHomingTime = pAtkData->WORK2 * 100;
        param.vPos = pos;
        param.vRot = rot;
        param.pEffectTbl = (BULLET_EFFECT_INFO*)PL::g_EffTbl_Skill[pAtkData->EFFECT];
        param.pRotTbl = PL::g_RotTbl_Magic;
        param.fCollRadius = pAtkData->COLL_START_FRAME * 0.01f;
        param.vPos.w = pAtkData->COLL_END_FRAME * 0.01f;
        bullet = (CBullet*)CEntityFactory::getInstance()->createBullet(param, this);
        if (bullet) bullet->setBodyRadius(pAtkData->WORK1 * 0.01f, 0.0f, 0, COLL_KIND_NONE);
        break;
    case 142:
        if (pTarget) {
            pTarget->getLockOnPos(pos);
            pos.y = getPos().y + 1.0f;
        } else {
            pos = getPos();
            pos.y += 1.0f;
        }
        rot = getRot();
        param.init(1, 3);
        param.CollKind = 0x80000000;
        param.uiFlag = 0x47;
        param.AtkKind = pAtkData->DAMAGE;
        param.AtkAttr = m_AtkAttr;
        param.iAttackPoint = getPower(m_uiSkillID, m_uiKind);
        CardBattle::CCardBattle::calcEnchant_AttackPoint(param.iAttackPoint, (ATK_KIND&)param.AtkKind,
                                                         (ATK_ATTR)param.AtkAttr, m_uiUpKind);
        param.fSpeed = pAtkData->MAX_SPEED * 0.01f;
        param.fMaxDist = pAtkData->WORK0;
        param.tqHomingTime = 800;
        param.vPos = pos;
        param.vRot = rot;
        param.pEffectTbl = (BULLET_EFFECT_INFO*)PL::g_EffTbl_Skill[pAtkData->EFFECT];
        param.fCollRadius = pAtkData->COLL_START_FRAME * 0.1f;
        bullet = (CBullet*)CEntityFactory::getInstance()->createBullet(param, this);
        if (bullet) bullet->setBodyRadius(0.1f, 0.0f, 0, COLL_KIND_NONE);
        break;
    }
}

} // namespace Summon

// SOURCE DATA: .data .rodata
