#define TASK_H_NO_SLC_OVERFLOW_MSG
#define GPRIM_H_PATH "System/Graphic/GPrim.h"
#include "System/Graphic/Anim.h"
#include "System/Graphic/Gprim.h"
#include "System/Graphic/2d/2DMan.h"
#include "Game/CardBattle.h"
#include "System/Graphic/Str.h"
#include "Game/Collision/CollUtil.h"

/* Placeholders: three empty strings follow CollUtil.h's strings in .rodata (as in FriendAladdin). */
inline void _emptyStringPlaceholder0() {
    TRACE("");
}
inline void _emptyStringPlaceholder1() {
    TRACE("");
}
inline void _emptyStringPlaceholder2() {
    TRACE("");
}
/* System/Task.h's container overflow message comes after them here. */
inline void _taskSLCOverflowMsg() {
    TRACE("\x83\x5e\x83\x58\x83\x4e\x97\x70\x83\x52\x83\x93\x83\x65\x83\x69\x82\xaa MAX \x82\xc9\x93\x9e\x92\x42\x82\xb5\x82\xdc\x82\xb5\x82\xbd\n");
}

#include "System/Graphic/Effect/effect.h"
#include "Game/Enemy/BattleEnemy.h"
#include "Game/Player/PlayerRiku.h"
#include "Game/Player/PlayerManager.h"
#include "Game/Player/PlayerCamera.h"
#include "Game/Player/PlayerGauge.h"
#include "Game/Player/Friend.h"
#include "Game/Player/PlayerAttack.h"
#include "Game/Player/PlayerWeapon.h"
#include "Game/Debug/DebugWork.h"
#include "Game/Movement.h"
#include "Game/Collision/CollMap.h"
#include "Game/Bullet/Bullet.h"
#include "Game/EntityFactory.h"
#include "System/Graphic/Effect/EffectMan.h"
#include "System/TaskMan.h"
#include "System/Debug.h"
#include "include_asm.h"

namespace PL {
class CPlayerDRiku : public CPlayer {
public:
    CPlayerDRiku(CTask* inParent, UINT inID, INT inEx);
    void startDMode(CPlayerRiku* inRiku);
};

extern UINT g_uiStartCondition;
extern FVECTOR g_vBGFadeColor[7];
extern BULLET_EFFECT_INFO g_EffTbl_Skill[67][4]; /* Game/Player/PlayerAttack.h */
extern FLOAT g_fGravity;
extern TIMEQ g_CardBreakTime[];

static LPCSTR g_szDpxTransform = "transform";

static LPCSTR g_szDpxFriend = "friend";
static LPCSTR g_szDpxDuel = "duel";
static LPCSTR g_szDpxSEAppear = "souleater";
static LPCSTR g_szDpxImpals = "impals";

PL_PARAM g_RikuParam = {
    0x21279, 0x2000259, 0.3f, 1.2f, 1.3f, 6.5f, 1.8f, 34.1f, 38.0f, 200, 2200, 2200,
};

BCAM_PARAM g_DuelCamParam = {
    { 55.0f, 0.05f, 0.2f, 0.7f, { { 0.0f, 0.9f, 4.0f, 0.0f } }, { { 0.0f, 0.1f, 0.0f, 0.0f } } },
    { 55.0f, 0.05f, 0.2f, 6.5f, { { 0.0f, -0.3f, 2.0f, 0.0f } }, { { 0.15f, -0.3f, 0.0f, 0.0f } } },
};
} // namespace PL

namespace Gauge {
class CPlayerGauge {
public:
    void setDP(INT inDP, INT inFlag);
    void setDMode(INT inP0, INT inP1, INT inP2);
};
class CEscapeGauge {
public:
    void destroy();
};
} // namespace Gauge

/* Stores/loads through a reference: gcc then can't tell them apart from the globals' and other members'
 * accesses, which keeps the original's order (the original sets these members through inline helpers). */
template <class T> static inline T& _ref(T& v) {
    return v;
}
static inline UINT _upKind(INT inType0, INT inType1) {
    UINT up = inType0 > 0 ? 1 << (inType0 - 1) : 0;
    if (inType1 > 0) up |= 1 << (inType1 - 1);
    return up;
}

static inline FLOAT _limitPI(FLOAT r) {
    if (r < -3.1415927f) r += 6.2831855f;
    else if (r > 3.1415927f) r -= 6.2831855f;
    return r;
}

static inline INT _getNowCamMode() {
    INT mode = 0;
    PlayerCamera::CPlayerCamera* cam = PlayerCamera::CPlayerCamera::getInstanceRef();
    if (cam) mode = cam->getCamMode();
    return mode;
}

class CRcf {
public:
    static BOOL isCompleteLoadRCF(INT nType, INT nID);
};

namespace Friend {
extern BCAM_PARAM g_HolyBurstCamParam;
}

void ScaleVectorXYZ(FVECTOR& outV, const FVECTOR& inV, FLOAT inScale);

/* Game/Collision/CollBody.h (not decompiled yet): CPlayer's m_collBody */
struct _CollBodyView {
    UBYTE _m_0x00[0x8];
    INT m_Enable;      /* 0x08 */
    UBYTE _m_0x0c[0x76];
    SHORT m_Damage;     /* 0x82 */
    SHORT m_enableBody; /* 0x84 */
    void off() { m_Enable = 0; }
    void clearDamage() { m_Damage = 0; }
    void offBody() {
        m_Enable = 0;
        m_enableBody = 0;
    }
};

namespace PL {

/* sets up the attack data of a stock skill's attack (inline in the original) */
static inline void _setAttack(CPlayerRiku* p, UINT inID) {
    p->m_uiAttackID = inID;
    p->m_pAtkData = &g_AttackDataList[inID];
    p->m_iAttackPower = p->m_iAP * CardBattle::g_pStockSkillList[p->m_iStockSkill].POWER[p->m_pAtkData->POWER] / 100;
    _ref(p->m_AtkKind) = (ATK_KIND)p->m_pAtkData->DAMAGE;
    _ref(p->m_AtkAttr) = (ATK_ATTR)CardBattle::g_pStockSkillList[p->m_iStockSkill].ATTR;
    UINT up = _upKind(CardBattle::g_pStockSkillList[p->m_iStockSkill].UP_TYPE0,
                      CardBattle::g_pStockSkillList[p->m_iStockSkill].UP_TYPE1);
    p->calcEnchant_AttackPoint(p->m_iAttackPower, p->m_AtkKind, p->m_AtkAttr, up);
}

CPlayerRiku::CPlayerRiku(CTask* inParent) : CPlayer(inParent, PLM::g_uiPlayerID, 0) {
    g_Param = g_RikuParam;
    m_pDark = NULL;
    m_hMagicCore = 0;
}

CPlayerRiku::~CPlayerRiku() {}

void CPlayerRiku::beginStateBattle() {
    TRACE("CPlayerRiku:: beginStateBattle()\n");
    m_iState = 1;
    m_uiStep = 0;
    m_uiActionFlag = (m_uiActionFlag | m_pParam->USE_BTL_ACTION) & ~0;
    initActionSkill();
    m_uiBattleFlag = (m_uiBattleFlag | 1) & ~0;
    setAnimForce(0x2a, -1, 0, 0, 0, 0);
    onWeapon(0);
    if (!(m_uiConditionFlag & 0x400000)) {
        m_pPLM->setReqLoadFirst();
        initStartWait();
        if (g_uiStartCondition & 8) {
            setConfuse(1, 0);
        }
        g_uiStartCondition = 0;
    } else {
        setAnimForce(0x2a, -1, 0, 0, 0, 0);
        setPlayerState(STAND);
        initStartWait();
    }
    m_uiConditionFlag = (m_uiConditionFlag | 0) & ~0x400000;
    if (!m_pDark && (m_uiActionFlag & 0x4000000)) {
        m_pDark = (CPlayerDRiku*)PLM::createPlayer(m_uiCharaID + 2, this, 1);
    }
    if (m_pCB) {
        m_pCB->m_uiFlag = (m_pCB->m_uiFlag | 0) & ~0x400000;
    }
}

void CPlayerRiku::stateBattle() {
    CPlayer::stateBattle();
    if (DebugWork::g_dw.player.iDP) {
        if (m_iDP != 30) {
            m_pGauge->setDP(30, 0);
        }
        m_iDP = 30;
    }
}

void CPlayerRiku::endStateBattle() {
    CPlayer::endStateBattle();
}

void CPlayerRiku::beginStateWait() {
    TRACE("CPlayerRiku:: beginStateWait()\n");
    m_iState = 2;
    if (!(m_uiPlayerFlag & 0x100)) {
        clearSpeed();
        m_WalkMode = 0;
        setEntityFlag(1, 0);
        setAnimForce(0x2a, 0, 0, 0, 0, 0);
        m_uiPlayerFlag = (m_uiPlayerFlag | 0x401001) & ~0;
        ((_CollBodyView*)m_collBody)->offBody();
    }
}

void CPlayerRiku::stateWait() {
    if (!(m_uiPlayerFlag & 0x100)) {
        m_uiPlayerFlag = (m_uiPlayerFlag | 0x400000) & ~0;
        if (m_pDark && m_PlayerState == TRANSFORM) {
            exeTransform();
        }
    }
}

void CPlayerRiku::endStateWait() {}

void CPlayerRiku::onWeapon(INT inEffect) {
    if (m_uiPlayerFlag & 2) {
        setPlayerFlag(0, 2);
        setDispOffFlag(0, 3);
        if (inEffect) {
            FVECTOR rot;
            FVECTOR tmp;
            tmp.x = 0.0f;
            tmp.y = 0.0f;
            tmp.z = -1.5707964f;
            tmp.w = 1.0f;
            rot = tmp;
            attachEffect(g_szDpxSEAppear, 0, &Crd(m_iBoneWeapon), NULL, &rot, 0.0f);
            setupFade(0.0f, 1.0f, 2000, 2);
        }
    }
}

void CPlayerRiku::offWeapon() {
    m_uiPlayerFlag = (m_uiPlayerFlag | 2) & ~0;
    setDispOffFlag(3, 0);
}

void CPlayerRiku::setStateSE(INT inState, INT inKind) {
    INT SEID = -1;
    switch (inState) {
    case 1:
        switch (m_uiAttackID) {
        case 0x98:
        case 0x99:
            SEID = 0x12e;
            break;
        case 0x9b:
            SEID = 0x12d;
            break;
        case 0x97:
        case 0x9c:
            SEID = 0x12c;
            break;
        case 0x9a:
        case 0x9d:
            SEID = 0x12f;
            break;
        case 0xae:
        case 0xaf:
        case 0xb0:
        case 0xb1:
        case 0xb2:
        case 0xb3:
        case 0xb4:
            SEID = 0x130;
            break;
        case 0xac:
        case 0xad:
            playSEPanAndVol(0, 0x15d, 0, NULL, 0);
            break;
        }
        break;
    case 2:
        SEID = 0x131;
        break;
    }
    if (SEID != -1) {
        playSEPanAndVol(1, SEID, 0, NULL, 0);
    }
}
INT CPlayerRiku::getStateSESeq(INT inState) {
    INT seqID = 0;
    switch (inState) {
    case 3:
    case 6:
        switch (m_uiAttackID) {
        case 0x5c:
        case 0x74:
        case 0x75:
        case 0x76:
            seqID = 1;
            break;
        }
        break;
    }
    return seqID;
}

void CPlayerRiku::initTransform(INT inFlag) {
    if (m_bIsDead) return;
    TRACE("CPlayerRiku:: initTransform(%d)\n", inFlag);
    setPlayerState(TRANSFORM);
    setConditionFlag(0, 0x2000000);
    setTrgFlag(0, 0x100000);
    setMDFooting(0);
    fadeEffAttack(0);
    if (!inFlag) {
        setConditionFlag(0, 0x200000);
        procSkillEnd(-1);
        return;
    }
    m_iSubState = 1;
    clearSpeed();
    offAttack();
    setBattleFlag(0, 8);
    setPlayerFlag(0, 0x40000);
    setConditionFlag(0x200000, 0);
    _ref(m_tqCardBreakTime) = g_CardBreakTime[7];
    m_fJumpGravity = g_fGravity;
    _ref(m_fWalkSpeed) = 0.0f;
    _ref(m_fMoveSpdXZ) = 0.0f;
    m_JumpState = 3;
}

void CPlayerRiku::exeTransform() {
    setBattleFlag(0, 8);
    setPlayerFlag(0x400000, 0);
    switch (m_iSubState) {
    case 0:
        return;
    case 1:
        if (isAir()) {
            setAnim(6, 800, 0);
            break;
        }
        m_iSubState = 2;
        setAnimForce(0x37, -1, 0, 0, 0, 0);
        m_tqStateTimer = 0;
        break;
    case 2:
        setPlayerFlag(0x400000, 0);
        if (!isMotionEnd()) break;
        m_pDark->startDMode(this);
        m_iSubState = 3;
        setPlayerFlag(0x1001, 0);
        ((_CollBodyView*)m_collBody)->offBody();
        offWeapon();
        m_pMarker->setFlag(FALSE);
        m_pCB->destroyPlayCard(1, 0);
        setBattleFlag(0, 0x10200000);
        m_pCB->setFlag(0xc0000000, 0);
        if (isBattleFlag(2)) {
            setBattleFlag(0, 2);
            m_tqEscape = 0;
            if (m_pEscape) m_pEscape->destroy();
            setBGFade(1.0f, 0.0f, 0);
        }
        changeState(2);
        break;
    case 3:
        setPlayerFlag(0x400000, 0);
        if (!m_pDark->isConditionFlag(0x200000)) return;
        m_pCam = NULL;
        m_pGauge = NULL;
        m_pMarker = NULL;
        m_pEscape = NULL;
        m_pCB = NULL;
        setPrizeHealing(0);
        m_iSubState = 0;
        clearSpeed();
        setBattleFlag(0, 0x443f90);
        setTrgFlag(0, 0x80bff);
        setBattleFlag(0, 0x80000);
        setBattleFlag(0, 0x10200000);
        setBattleFlag(0, 0x4060);
        setBattleFlag(0, 0x130000);
        setConditionFlag(0, 0x881803f);
        if (m_tqConfuse > 0) {
            m_tqConfuse = 0;
            setConfuse(0, 0);
        }
        return;
    case 4: {
        setPlayerFlag(0x400000, 0);
        m_iSubState = 5;
        clearSpeed();
        offAttack();
        setBattleFlag(0, 8);
        setPlayerFlag(0, 0x40000);
        _ref(m_tqCardBreakTime) = g_CardBreakTime[7];
        m_fJumpGravity = g_fGravity;
        m_fMoveSpdXZ = 0.0f;
        m_fWalkSpeed = 0.0f;
        m_JumpState = 3;
        FVECTOR pos = m_pDark->getPos();
        warp(pos);
        setRot(m_pDark->getRot());
        updateCoord();
        coordFix();
        pos.y += 0.5f;
        setOldPos(pos);
        setEntityFlag(0, 1);
        onWeapon(0);
        transferControl();
        CEffect* effect = CEffectMan::getEffect(attachEffect(g_szDpxTransform, 2, &Crd(0x1b), NULL, NULL, 0.0f));
        if (effect) effect->setAttachNoRot(TRUE);
        break;
    }
    case 5:
        setPlayerFlag(0x400000, 0);
        changeState(1);
        setPlayerFlag(0, 1);
        setPlayerFlag(0, 0x1000);
        m_pCam->changeNowPlayer(0, 0);
        m_pCam->changeCamMode(3, 0);
        enableControl();
        m_pMarker->setFlag(TRUE);
        m_iSubState = 0;
        setConditionFlag(0, 0x200000);
        break;
    }
    FVECTOR vel = m_pMove->getVel();
    m_fJumpGravity = g_fGravity;
    vel.y = m_fJumpSpdY += g_fGravity * m_fFrameRate;
    m_pMove->getVel() = vel;
}


void CPlayerRiku::endDMode(CPlayerDRiku* inRiku) {
    m_pDark = inRiku;
    m_iSubState = 4;
}

void CPlayerRiku::transferControl() {
    _ref(g_pPlayer) = this;
    _ref(m_pInstance) = this;
    m_iMaxHP = m_pDark->m_iMaxHP;
    m_iHP = m_pDark->m_iHP;
    m_iAP = m_pDark->m_iAP;
    m_iEXP = m_pDark->m_iEXP;
    m_iLV = m_pDark->m_iLV;
    m_iMP = m_pDark->m_iMP;
    m_iMaxDP = m_pDark->m_iMaxDP;
    setConditionFlag(0x400000, 0);
    m_iDP = 0;
    UINT flag = m_pDark->m_uiBattleFlag;
    setBattleFlag(flag, 0);
    TRACE("CPlayerRiku::transferControl (DRiku->Riku) uiBattleFlag = %x,\n", flag);
    if (isBattleFlag(0x8000)) {
        TRACE("CPlayerRiku::transferControl uiBattleFlag [CARD_DUEL] clear !!\n");
    }
    setBattleFlag(0, 8);
    setBattleFlag(0, 0x443f90);
    setBattleFlag(0, 0x80000);
    setBattleFlag(0, 0x10200000);
    setBattleFlag(0, 0x4060);
    setBattleFlag(0, 0x130000);
    setBattleFlag(0, 0x8000);
    m_bIsDead = m_pDark->m_bIsDead;
    m_pCam = m_pDark->m_pCam;
    m_pGauge = m_pDark->m_pGauge;
    m_pMarker = m_pDark->m_pMarker;
    m_pEscape = m_pDark->m_pEscape;
    m_pGauge->setDMode(0, m_iDP, 0);
    m_pCB = (CardBattle::CCardBattle*)CTaskMan::getSysTask(0x16);
    m_pCB->transferPlayerEnchant(m_pDark);
    m_pCB->setFlag(0, 0x400000);
    m_pRevolver = m_pDark->m_pRevolver;
    m_pRevolver->changeDarkMode(getHandle(), 0);
    setConditionFlag(0, 0x881803f);
    if (m_pDark->m_tqConfuse > 0) {
        setConfuse(1, m_pDark->m_tqConfuse);
    }
}


void CPlayerRiku::exeAvoid() {
    exeSpeedStep();
}

void CPlayerRiku::exeSpeedStep() {
    setBattleFlag(0, 8);
    FVECTOR vel = m_pMove->getVel();
    if (isAir()) {
        vel.y = m_fJumpSpdY += m_fJumpGravity * m_fFrameRate;
        m_pMove->getVel() = vel;
    } else {
        if (m_fJumpSpdY < 0.0f) m_fJumpSpdY = 0.0f;
        vel.y = m_fJumpSpdY;
        m_pMove->getVel() = vel;
    }
    if (getMotionCurrentTime() >= m_pParam->AVOID_TQ_MUTEKI) {
        setPlayerFlag(0, 1);
    }
    if (isMotionEnd()) {
        setPlayerFlag(0, 1);
        initIdle(-1, 1);
    }
}

INT CPlayerRiku::startStockSkillAction() {
    INT id;
    switch (m_iStockSkill) {
    case 0x62:
    case 0x64:
        id = m_iStockSkill;
        goto add;
    case 0x63:
        id = m_iStockSkill;
        id--;
    add:
        PLM::addStockBR(id);
    case 0x66:
    case 0x67:
    case 0x68:
    case 0x69:
    case 0x6a:
    case 0x6b:
        initStockSkill(0);
        m_iStepStockSkill = 10;
        return 1;
    }
    return 0;
}

void CPlayerRiku::initStockSkill(INT inNo) {
    INT type;
    setPlayerState(SKILL);
    setBattleFlag(0, 8);
    setBattleFlag(0, 0x80000);
    setPlayerFlag(0, 0x20000);
    m_iAttackNo = inNo;
    if (!inNo) m_uiPreAttackKind = 0;
    type = 0;
    m_bIsAttackHit = FALSE;
    m_bNextTrg = FALSE;
    m_iNextAttack = 0;
    m_bIsCanCombo = FALSE;
    m_bDmgMuteki = FALSE;
    m_tqCardBreakTime = g_CardBreakTime[7];
    clearSpeed();
    m_JumpState = 3;
    _ref(m_szEffectName) = NULL;
    _ref(m_uiAttackKind) = 8;
    m_fMoveSpdXZ = 0.0f;
    m_fWalkSpeed = 0.0f;
    m_iAerialAnimShift = 0;
    m_fJumpGravity = g_fGravity;
    setPlayerFlag(0, 0x2800000);
    procTargetMark();
    switch (m_iStockSkill) {
    case 0x62:
    case 0x63:
        m_uiAttackID = m_iStockSkill + 0x12;
        m_pPLM->setReqLoad(0x62, 0);
        break;
    case 0x64:
        m_fWallRadius = 1.0f;
        m_uiAttackID = 0x76;
        m_pPLM->setReqLoad(0x62, 0);
        type = 1;
        m_pPLM->setReqLoad(0x64, 0);
        break;
    case 0x66:
        m_uiAttackID = 0xae;
        m_pPLM->setReqLoad(0x66, 0);
        type = 1;
        m_pPLM->setReqLoad(0x93, 0);
        setBattleFlag(0x100000, 0);
        break;
    case 0x67:
        m_uiAttackID = 0xb0;
        m_pPLM->setReqLoad(0x67, 0);
        type = 1;
        m_pPLM->setReqLoad(0x94, 0);
        setBattleFlag(0x100000, 0);
        break;
    case 0x68:
        m_uiAttackID = 0xb2;
        m_pPLM->setReqLoad(0x68, 0);
        type = 1;
        m_pPLM->setReqLoad(0x95, 0);
        setBattleFlag(0x100000, 0);
        break;
    default:
        m_iSubState = 3;
        m_pAtkData = &g_AttackDataList[0];
        m_uiAttackID = 0;
        m_uiAttackKind = 0;
        return;
    }
    m_pAtkData = &g_AttackDataList[m_uiAttackID];
    if (type == 1) {
        m_iAttackPower = m_iAP * CardBattle::g_pStockSkillList[m_iStockSkill].POWER[m_pAtkData->POWER] / 100;
    } else {
        m_iAttackPower = 0;
    }
    _ref(m_AtkKind) = (ATK_KIND)m_pAtkData->DAMAGE;
    _ref(m_AtkAttr) = (ATK_ATTR)CardBattle::g_pStockSkillList[m_iStockSkill].ATTR;
    UINT up = _upKind(CardBattle::g_pStockSkillList[m_iStockSkill].UP_TYPE0,
                      CardBattle::g_pStockSkillList[m_iStockSkill].UP_TYPE1);
    calcEnchant_AttackPoint(m_iAttackPower, m_AtkKind, m_AtkAttr, up);
    m_fDistXZ = m_fDistY = 0.0f;
    m_iSubState = 1;
    m_uiPreAttackKind = m_uiAttackKind;
    setTrgFlag(0, 0x40000);
    m_hMagicCore = 0;
}

INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Game/Player/PlayerRiku", exeStockSkill__Q22PL11CPlayerRiku);

/* Best attempt (same structure): the CBattleEnemy flag tests get swapped registers, 0x6CD doesn't reload the flag, and 0.0f isn't rematerialized in f20 in 0x6CC. */
#if 0
void CPlayerRiku::exeStockSkill() {
    INT SESeq;
    FVECTOR pos;
    FVECTOR rot;
    FVECTOR vel;
    CBattleEnemy* pTarget;
    FVECTOR vec;
    TIMEQ frame = getMotionCurrentTime() / 50;
    setBattleFlag(0, 8);
    m_fDistXZ = m_fDistY = 0.0f;
    switch (m_iSubState) {
    case 1:
        if (!isPlayerFlag(0x800000) && isAir()) {
            setAnim(6, 800, 0);
            procGravity(frame, 0, 0);
            return;
        }
        SESeq = getStateSESeq(3);
        m_fJumpSpdY = 0.0f;
        m_JumpState = 3;
        clearSpeed();
        m_hOldLockon = m_hLockon;
        _ref(m_iOldCamMode) = _getNowCamMode();
        if (CardBattle::g_pStockSkillList[m_iStockSkill].CATEGORY == 5) {
            setBGFade(g_vBGFadeColor[0], 20.0f, 0);
            setAnimForce(0x2e, -1, 1, 0, SESeq, 0);
            setCastEffect(g_szDpxFriend, 0, &Crd(0), NULL);
            calcAppearPosFS_toAttackID();
            if ((m_pAtkData->FLAG & 0x20) && m_pPLM->isCompleteLoad(m_iStockSkill) != 1) {
                setObjStop(1);
                m_pCam->changeCamMode(5, 0);
            }
            m_iSubState = 11;
            return;
        }
        if (isPlayerFlag(0x800000)) {
            setPlayerFlag(0x2000000, 0);
            m_fJumpGravity = g_fGravity * 0.16f;
        }
        if (isBattleFlag(0x100000)) {
            setBGFade(g_vBGFadeColor[0], 20.0f, 0);
            setAnim(0x38, -1, 0);
            if (m_pPLM->isCompleteLoad(m_iStockSkill) != 1) {
                setObjStop(1);
                m_pCam->changeCamMode(5, 0);
            }
            m_iSubState = 0x4a1;
            return;
        }
        if (m_pAtkData->FLAG & 0x20) {
            setBGFade(g_vBGFadeColor[0], 20.0f, 0);
            if (m_pPLM->isCompleteLoad(m_iStockSkill) != 1) {
                setAnimForce(0x2e, -1, 1, 0, 0, 0);
                setObjStop(1);
                m_pCam->changeCamMode(5, 0);
                m_iSubState = 4;
                return;
            }
            setUseRsrcBlock(1);
        }
        m_iSubState = m_uiAttackID * 10;
        if (!(m_pAtkData->FLAG & 0x40)) {
            setAnimForce(m_pAtkData->ANIM_ID + m_iAerialAnimShift, 0, 1, m_pAtkData->ANIM_ESQ, SESeq, 0);
        } else {
            setAnimSkillEx(m_pAtkData->ANIM_ID + m_iAerialAnimShift, 0, 1, m_pAtkData->ANIM_ESQ, SESeq);
        }
        setPlayerFlag(0x100000, 0);
        m_tqStateTimer = 0;
        m_pMarker->holdTarget(8);
        procTargetMark();
        return;
    case 4:
        if (m_pPLM->isCompleteLoad(m_iStockSkill) != 1) return;
        if (!isMotionLoop()) return;
        setUseRsrcBlock(1);
        setObjStop(0);
        m_pCam->changeCamMode(-1, 0);
        SESeq = getStateSESeq(3);
        m_iSubState = m_uiAttackID * 10;
        if (!(m_pAtkData->FLAG & 0x40)) {
            setAnimForce(m_pAtkData->ANIM_ID + m_iAerialAnimShift, 0, 1, m_pAtkData->ANIM_ESQ, SESeq, 0);
        } else {
            setAnimSkillEx(m_pAtkData->ANIM_ID + m_iAerialAnimShift, 0, 1, m_pAtkData->ANIM_ESQ, SESeq);
        }
        setPlayerFlag(0x100000, 0);
        m_tqStateTimer = 0;
        m_pMarker->holdTarget(8);
        procTargetMark();
        return;
    case 11:
        if (m_iStockSkill == 0x64) {
            if (!CRcf::isCompleteLoadRCF(0, 0)) break;
            m_bNextTrg = FALSE;
            if (m_pPLM->isCompleteLoad(0x62) != 1) {
                m_pPLM->setReqLoad(0x62, 0);
                m_bNextTrg = TRUE;
            }
            if (m_pPLM->isCompleteLoad(0x64) != 1) {
                m_pPLM->setReqLoad(0x64, 0);
                m_bNextTrg = TRUE;
            }
            if (m_bNextTrg) break;
            if (!m_pPLM->isFriendReady()) {
                m_pPLM->createFriend(m_pPLM->getFriendID_toSkill(m_iStockSkill), this);
                break;
            }
            setUseRsrcBlock(1);
        } else {
            if (m_pPLM->isCompleteLoad(m_iStockSkill) != 1) break;
            if (!m_pPLM->isFriendReady()) {
                m_pPLM->createFriend(m_pPLM->getFriendID_toSkill(m_iStockSkill), NULL);
                break;
            }
        }
        if (!isMotionLoop()) break;
        setObjStop(0);
        m_pCam->changeCamMode(-1, 0);
        calcAppearPosFS_toAttackID();
        m_pPLM->getFriend()->setAppear();
        fadeEffect(m_hEffCast, 1);
        setEffect(g_szDpxFriend, 3, &getPos(), NULL, 0.0f);
        m_iSubState = m_uiAttackID * 10;
        setAnimForce(0x2a, -1, 0, 0, 0, 0);
        setPlayerFlag(0x100000, 0);
        m_tqStateTimer = 0;
        m_pMarker->holdTarget(8);
        procTargetMark();
        break;
    case 0x488:
    case 0x492:
        if (!m_pPLM->isFriendReady()) {
            m_iSubState = 0x4a0;
            break;
        }
        if (!m_bIsCanCombo) {
            m_bIsCanCombo = m_pPLM->getFriend()->setSkill(m_uiAttackID);
        }
        exeStandUp_inFriend();
        break;
    case 0x49c:
        setPlayerFlag(0x400000, 0);
        if (!m_pPLM->isFriendReady()) {
            m_iSubState = 0x4a0;
            break;
        }
        if (!m_bIsCanCombo) {
            m_bIsCanCombo = m_pPLM->getFriend()->setSkill(m_uiAttackID);
            if (!m_bIsCanCombo) break;
        }
        m_iSubState = 0x49d;
        setAnimSkillEx(0, 0, 0, 0, 0);
        setConditionFlag(0x80000, 0);
        break;
    case 0x49d:
        if (!m_pPLM->isFriendReady()) {
            m_iSubState = 0x4a0;
            break;
        }
        if (!isMotionEnd()) break;
        m_iSubState = 0x49e;
        setAnimSkillEx(1, 0, 0, 0, 0);
        setTimer(m_pAtkData->TRG_FRAME * 50);
        m_uiStep = 0;
        m_iCount = 0;
        PlayerCamera::CPlayerCamera::setBattleCamData(getHandle(), &Friend::g_HolyBurstCamParam);
        m_pCam->changeNowPlayer(getHandle(), 0);
        m_pCam->changeCamMode(3, 0);
        break;
    case 0x49e: {
        if (!m_pPLM->isFriendReady()) {
            m_iSubState = 0x4a0;
            break;
        }
        if (isMotionEnd()) {
            m_iCount = 0;
            if (++m_uiStep >= 4) {
                m_iSubState = 0x49f;
                setAnimSkillEx(2, -1, 1, 0, 0);
                setConditionFlag(0, 0x80000);
                m_fJumpSpdXZ = 0.0f;
                m_fWalkSpeed = 0.0f;
                m_fWallRadius = 0.5f;
                m_pMove->calcVelocityXZ(0.0f);
                clearSpeed();
                PlayerCamera::CPlayerCamera::setBattleCamData(0, NULL);
                m_pCam->changeNowPlayer(0, 0);
                m_pCam->changeCamMode(3, 0);
                break;
            }
        }
        FLOAT speed = 0.0f;
        if (m_fAnalogPower >= 0.3f) {
            FVECTOR v;
            speed = m_pAtkData->WORK2 * 0.1f;
            _limitPI(m_fAnalogViewDir - m_fDir);
            m_fDir = Movement::Homing(m_fDir, m_fAnalogViewDir, 0.2f, m_fFrameRate);
            FVECTOR tmp;
            tmp._q = 0;
            tmp.y = m_fDir;
            v = tmp;
            setRot(v);
        }
        m_fWalkSpeed = speed / 60.0f;
        m_fWalkSpeed = m_fJumpSpdXZ = Movement::Approach(m_fJumpSpdXZ, m_fWalkSpeed, 0.02f, m_fFrameRate);
        m_pMove->calcVelocityXZ(m_fJumpSpdXZ);
        if (getTimer() <= 0) {
            setMagic(0x162, m_uiStep, m_iStockSkill);
            m_iCount++;
            setTimer(m_pAtkData->TRG_FRAME * 50);
        }
        break;
    }
    case 0x49f:
        if (isMotionEnd()) setAnim(0x2a, -1, 0);
        if (m_pPLM->isFriendReady()) break;
        procSkillEnd(-1);
        break;
    case 0x4a0:
        procSkillEnd(-1);
        return;
    case 0x4a1:
        if (!CRcf::isCompleteLoadRCF(0, 0)) break;
        m_bNextTrg = FALSE;
        if (m_pPLM->isCompleteLoad(m_iStockSkill) != 1) {
            m_pPLM->setReqLoad(m_iStockSkill, 0);
            m_bNextTrg = TRUE;
        }
        switch (m_iStockSkill) {
        case 0x66:
            if (m_pPLM->isCompleteLoad(0x93) != 1) {
                m_pPLM->setReqLoad(0x93, 0);
                m_bNextTrg = TRUE;
            }
            break;
        case 0x67:
            if (m_pPLM->isCompleteLoad(0x94) != 1) {
                m_pPLM->setReqLoad(0x94, 0);
                m_bNextTrg = TRUE;
            }
            break;
        case 0x68:
            if (m_pPLM->isCompleteLoad(0x95) != 1) {
                m_pPLM->setReqLoad(0x95, 0);
                m_bNextTrg = TRUE;
            }
            break;
        }
        if (m_bNextTrg) break;
        setUseRsrcBlock(1);
        setObjStop(0);
        m_pCam->changeCamMode(-1, 0);
        m_iSubState = m_uiAttackID * 10;
        SESeq = getStateSESeq(3);
        switch (m_iSubState) {
        case 0x6cc:
            setAnimSkillEx(m_pAtkData->ANIM_ID, 0, 1, m_pAtkData->ANIM_ESQ, SESeq);
            break;
        case 0x6e0:
            _setAttack(this, 0xac);
            setAnimRsrcEx(0x514, 0, -1, 1, 0, 0, 0);
            break;
        case 0x6f4:
            _setAttack(this, 0xad);
            setAnimRsrcEx(0x514, 0, -1, 1, 0, 0, 0);
            break;
        }
        setPlayerFlag(0x100000, 0);
        m_tqStateTimer = 0;
        m_pMarker->holdTarget(8);
        procTargetMark();
        return;
    case 0x6cc:
        setPlayerFlag(0x400000, 0);
        procTargetMark();
        procAttackMove(frame, 0.0f, 0.0f, 2, 0.15f);
        if (!m_bNextTrg) {
            if (frame >= m_pAtkData->TRG_FRAME) {
                m_bNextTrg = TRUE;
                setMagic(0x20a, 0, m_iStockSkill);
            }
        } else {
            pTarget = NULL;
        if (m_hMark > 0) pTarget = (CBattleEnemy*)CTaskMan::getTask(m_hMark);
            if (pTarget && pTarget->isActiveTarget() && !pTarget->isBtlEnemyFlag(0x20000)) {
                pos = pTarget->getPos();
                vel = pTarget->getVel();
                if (0.0f < vel.y && 4.0f < pos.y - m_vJumpStartPos.y) {
                    pos.y = m_vJumpStartPos.y + 4.0f;
                    pTarget->setPos(pos);
                    ScaleVectorXYZ(vel, vel, 0.5f);
                    vel.y = 0.0f;
                    pTarget->setVel(vel);
                }
            }
        }
        if (!isMotionEnd()) break;
        m_bNextTrg = FALSE;
        m_bIsAttackHit = FALSE;
        if (m_fDistXZ > 6.0f) {
            m_iSubState = 0x6cd;
            _setAttack(this, 0xac);
            setAnimRsrcEx(0x514, 0, -1, 1, 0, 0, 0);
            setPlayerFlag(0x100000, 0);
            break;
        }
        m_iSubState = 0x6ce;
        _setAttack(this, 0xaf);
        setAnimSkillEx(m_pAtkData->ANIM_ID, 0, 1, 0, 0);
        setPlayerFlag(0x100000, 0);
        clearSpeed();
        return;
    case 0x6cd:
        pTarget = NULL;
        if (m_hMark > 0) pTarget = (CBattleEnemy*)CTaskMan::getTask(m_hMark);
        if (pTarget) {
            pTarget->getLockOnPos(m_vMarkPos);
            if (pTarget->isActiveTarget() && !pTarget->isBtlEnemyFlag(0x20000)) {
                vel = pTarget->getVel();
                if (vel.y < 0.0f) {
                    pos = pTarget->getOldPos();
                    pTarget->setPos(pos);
                    vel.y = 0.0f;
                    pTarget->setVel(vel);
                } else if (0.0f < vel.y) {
                    pos = pTarget->getPos();
                    if (4.0f < pos.y - m_vJumpStartPos.y) {
                        pos.y = m_vJumpStartPos.y + 4.0f;
                        pTarget->setPos(pos);
                        vel.y = 0.0f;
                        pTarget->setVel(vel);
                    }
                }
            }
        } else {
            m_hMark = m_hTarget;
        }
        if (m_pAnim->getAnimID()) {
            procAttackMove(frame, m_pAtkData->RADIUS * 0.1f, m_pAtkData->MAX_SPEED, 1, 1.0f);
        } else {
            procAttackMove(frame, 0.0f, 0.0f, 2, 0.15f);
            if (isMotionEnd()) {
                setAnimRsrcEx(0x514, 1, -1, 1, 0, 0, 0);
                onAttack();
            }
            setTimer(6000);
        }
        if (m_fDistXZ <= 6.0f || getTimer() <= 0) {
            offAttack();
            m_iSubState = 0x6ce;
            _setAttack(this, 0xaf);
            setAnimSkillEx(m_pAtkData->ANIM_ID, 0, 1, 0, 0);
            setPlayerFlag(0x100000, 0);
            clearSpeed();
            return;
        }
        break;
    case 0x6ce:
        if (procAttackCollision(frame)) return;
        pTarget = NULL;
        if (m_hMark > 0) pTarget = (CBattleEnemy*)CTaskMan::getTask(m_hMark);
        if (pTarget) {
            pTarget->getLockOnPos(m_vMarkPos);
            if (!m_bIsAttackHit && pTarget->isActiveTarget() && !pTarget->isBtlEnemyFlag(0x20000)) {
                vel = pTarget->getVel();
                if (vel.y < 0.0f) {
                    pos = pTarget->getOldPos();
                    pTarget->setPos(pos);
                    vel.y *= 0.5f;
                    pTarget->setVel(vel);
                } else if (0.0f < vel.y) {
                    pos = pTarget->getPos();
                    if (4.0f < pos.y - m_vJumpStartPos.y) {
                        pos.y = m_vJumpStartPos.y + 4.0f;
                        pTarget->setPos(pos);
                        vel.y = 0.0f;
                        pTarget->setVel(vel);
                    }
                }
            }
        } else {
            m_hMark = m_hTarget;
        }
        if (!m_bIsAttackHit && frame < m_pAtkData->COLL_END_FRAME) {
            procAttackMove(frame, m_pAtkData->RADIUS * 0.1f, m_pAtkData->MAX_SPEED, 1, 0.15f);
        } else {
            procAttackMove(frame, 0.0f, 0.0f, 0, 0.15f);
        }
        if (!m_bNextTrg && frame >= m_pAtkData->TRG_FRAME) {
            m_iSubState = 0x6cf;
            m_bNextTrg = TRUE;
            offAttack();
            setMagic(0x20a, 0, -1);
            setTimer(9000);
        }
        if (!m_bIsAttackHit) return;
        break;
    case 0x6cf:
        if (isMotionEnd()) setAnimForce(0x2a, -1, 0, 0, 0, 0);
        if (m_hMagicCore > 0 && !CTaskMan::getTask(m_hMagicCore)) m_hMagicCore = 0;
        if (getTimer() > 0 && m_hMagicCore) break;
        procSkillEnd(-1);
        return;
    case 0x6e0:
        pTarget = NULL;
        if (m_hMark > 0) pTarget = (CBattleEnemy*)CTaskMan::getTask(m_hMark);
        if (pTarget) {
            pTarget->getLockOnPos(m_vMarkPos);
        } else {
            m_hMark = 0;
        }
        if (m_pAnim->getAnimID()) {
            procAttackMove(frame, m_pAtkData->RADIUS * 0.1f, m_pAtkData->MAX_SPEED, 1, 1.0f);
        } else {
            procAttackMove(frame, 0.0f, 0.0f, 2, 0.15f);
            if (isMotionEnd()) {
                setAnimRsrcEx(0x514, 1, -1, 1, 0, 0, 0);
                onAttack();
            }
            setTimer(6000);
        }
        if (m_fDistXZ <= 6.0f || getTimer() <= 0) {
            offAttack();
            m_iSubState = 0x6e1;
            _setAttack(this, 0xb0);
            setAnimSkillEx(m_pAtkData->ANIM_ID, 0, 1, 0, 0);
            setPlayerFlag(0x100000, 0);
            clearSpeed();
            setTimer((m_pAtkData->COLL_START_FRAME + m_pAtkData->TRG_FRAME) * 50);
            m_bIsAttackHit = FALSE;
            return;
        }
        break;
    case 0x6e1:
        if (procAttackCollision(frame)) return;
        pTarget = NULL;
        if (m_hMark > 0) pTarget = (CBattleEnemy*)CTaskMan::getTask(m_hMark);
        if (pTarget) {
            m_hMark = m_hTarget;
            pTarget->getLockOnPos(m_vMarkPos);
        } else {
            m_hMark = m_hTarget;
        }
        if (frame < m_pAtkData->COLL_END_FRAME) {
            procAttackMove(frame, m_pAtkData->RADIUS * 0.1f, m_pAtkData->MAX_SPEED, 1, 0.15f);
            if (getTimer() <= 0) {
                m_pWeapon->updateAttackID();
                setTimer(m_pAtkData->TRG_FRAME * 50);
                m_bIsAttackHit = FALSE;
            }
        } else {
            procAttackMove(frame, 0.0f, 0.0f, 0, 0.15f);
        }
        if (!isMotionEnd()) break;
        m_bIsAttackHit = FALSE;
        offAttack();
        m_iSubState = 0x6e2;
        _setAttack(this, 0xb1);
        setAnimSkillEx(m_pAtkData->ANIM_ID, 0, 1, 0, 0);
        setPlayerFlag(0x100000, 0);
        clearSpeed();
        calcAppearPos(1.0f, 0.55f, NULL);
        return;
    case 0x6e2:
        if (procAttackCollision(frame)) return;
        if (!m_bIsAttackHit && frame < m_pAtkData->COLL_END_FRAME) {
            procAttackMove(frame, m_pAtkData->RADIUS * 0.1f, m_pAtkData->MAX_SPEED, 1, 0.15f);
        } else {
            procAttackMove(frame, 0.0f, 0.0f, 0, 0.15f);
        }
        if (frame >= m_pAtkData->TRG_FRAME && !m_bNextTrg) {
            m_bNextTrg = TRUE;
            setMagic(0x210, 0, -1);
        }
        if (!isMotionEnd()) return;
        procSkillEnd(-1);
        return;
    case 0x6f4:
        pTarget = NULL;
        if (m_hMark > 0) pTarget = (CBattleEnemy*)CTaskMan::getTask(m_hMark);
        if (pTarget) {
            pTarget->getLockOnPos(m_vMarkPos);
        } else {
            m_hMark = 0;
        }
        if (m_pAnim->getAnimID()) {
            procAttackMove(frame, m_pAtkData->RADIUS * 0.1f, m_pAtkData->MAX_SPEED, 1, 1.0f);
        } else {
            procAttackMove(frame, 0.0f, 0.0f, 2, 0.15f);
            if (isMotionEnd()) {
                setAnimRsrcEx(0x514, 1, -1, 1, 0, 0, 0);
                onAttack();
            }
            setTimer(6000);
        }
        if (m_fDistXZ <= m_pAtkData->RADIUS * 0.1f || getTimer() <= 0) {
            offAttack();
            m_iSubState = 0x6f5;
            _setAttack(this, 0xb2);
            setAnimSkillEx(m_pAtkData->ANIM_ID, 0, 1, 0, 0);
            setPlayerFlag(0x100000, 0);
            clearSpeed();
            m_bIsAttackHit = FALSE;
            return;
        }
        break;
    case 0x6f5:
        if (isPlayerFlag(0x100000)) procTargetMark();
        if (frame < m_pAtkData->COLL_END_FRAME) {
            procAttackMove(frame, m_pAtkData->RADIUS * 0.1f, m_pAtkData->MAX_SPEED, 1, 0.15f);
        } else {
            procAttackMove(frame, 0.0f, 0.0f, 0, 0.15f);
        }
        if (procAttackCollision(frame)) return;
        if (!isMotionEnd()) break;
        offAttack();
        m_iSubState = 0x6f6;
        _setAttack(this, 0xb3);
        setAnimSkillEx(m_pAtkData->ANIM_ID, 0, 0, 0, 0);
        setPlayerFlag(0x100000, 0);
        clearSpeed();
        m_bIsAttackHit = FALSE;
        procAttackMove(frame, 0.0f, 0.0f, 2, 1.0f);
        calcAppearPos(2.0f, 0.55f, NULL);
        m_vAppearPos.y = getPos().y;
        break;
    case 0x6f6:
        if (procAttackCollision(frame)) return;
        if (frame >= m_pAtkData->MOVE_START_FRAME) {
            if (!m_bNextTrg) {
                _ref(m_bNextTrg) = TRUE;
                m_fJumpSpdY = g_fAntiGravity * m_pAtkData->WORK1;
                vel = m_pMove->getVel();
                vel.y = m_fJumpSpdY;
                m_pMove->getVel() = vel;
            }
            procAttackMove(frame, 0.0f, 0.0f, 0, 0.15f);
        }
        if (!isMotionEnd()) break;
        offAttack();
        m_iSubState = 0x6f7;
        _setAttack(this, 0xb4);
        setAnimSkillEx(m_pAtkData->ANIM_ID, 0, 1, 0, 0);
        setPlayerFlag(0x100000, 0);
        clearSpeed();
        m_bIsAttackHit = FALSE;
        procAttackMove(frame, 0.0f, 0.0f, 2, 1.0f);
        break;
    case 0x6f7:
        if (frame < m_pAtkData->COLL_START_FRAME) {
            procTargetMark();
            procAttackMove(frame, 0.0f, 0.0f, 2, 0.15f);
        }
        if (frame >= m_pAtkData->TRG_FRAME && !m_bNextTrg) {
            m_bNextTrg = TRUE;
            m_pWeapon->updateAttackID();
        }
        if (procAttackCollision(frame)) return;
        if (!isMotionEnd()) return;
        offAttack();
        m_iSubState = 0x6f8;
        setAnimSkillEx(3, 0, 1, 0, 0);
        return;
    case 0x6f8:
        if (isMotionEnd()) {
            procSkillEnd(1);
            return;
        }
        m_fJumpSpdY *= 0.5f;
        break;
    case 3:
    case 0x6f9:
        if (!isMotionEnd()) break;
        procSkillEnd(-1);
        return;
    }
    procGravity(frame, m_pAtkData->MOVE_END_FRAME, 1);
}
#endif

void CPlayerRiku::initFriend(INT inNo) {
    if (m_pCB->getEnchantKind() == 0x1b) {
        addHP(m_iMaxHP / 4, 6000, 1);
    }
    setPlayerState(FRIEND);
    m_iSubState = 1;
    clearSpeed();
    offAttack();
    setBattleFlag(0, 8);
    setBattleFlag(0, 0x400);
    setBattleFlag(0, 0x80000);
    setTrgFlag(0, 0x80);
    setPlayerFlag(0, 0x20000);
    setPlayerFlag(0, 0x2800000);
    _ref(m_iAttackNo) = inNo;
    _ref(m_uiPreAttackKind) = 0;
    _ref(m_uiAttackKind) = 0;
    _ref(m_bIsAttackHit) = FALSE;
    _ref(m_bNextTrg) = FALSE;
    _ref(m_iNextAttack) = 0;
    _ref(m_bDmgMuteki) = FALSE;
    _ref(m_bIsCanCombo) = FALSE;
    _ref(m_fJumpGravity) = g_fGravity;
    _ref(m_JumpState) = 3;
    _ref(m_fMoveSpdXZ) = 0.0f;
    m_tqCardBreakTime = g_CardBreakTime[CardBattle::g_pBattleCardList[m_uiCardID].BREAK];
    m_fWalkSpeed = 0.0f;
    m_szEffectName = NULL;
    if (m_uiCardID != 0x38) {
        m_iSubState = 3;
        m_pAtkData = &g_AttackDataList[0];
        m_uiAttackID = 0;
        m_uiAttackKind = 0;
    } else {
        m_uiAttackID = 0x5c;
        m_pPLM->setReqLoad(0x62, 0);
        m_pAtkData = &g_AttackDataList[m_uiAttackID];
        m_uiPreAttackKind = m_uiAttackKind;
        m_fDistXZ = m_fDistY = 0.0f;
    }
}

void CPlayerRiku::exeFriend() {
    TIMEQ frame = getMotionCurrentTime() / 50;
    setBattleFlag(0, 8);
    procGravity(frame, m_pAtkData->MOVE_END_FRAME, 1);
    switch (m_iSubState) {
    case 1:
        if (isAir()) {
            setAnim(6, 800, 0);
            break;
        }
        setBGFade(g_vBGFadeColor[0], 20.0f, 0);
        setAnimForce(0x2e, -1, 1, 0, getStateSESeq(6), 0);
        setCastEffect(g_szDpxFriend, 0, &Crd(0), NULL);
        calcAppearPosFS_toAttackID();
        m_hOldLockon = m_hLockon;
        m_iOldCamMode = _getNowCamMode();
        if ((m_pAtkData->FLAG & 0x20) && m_pPLM->isCompleteLoadCard(m_uiCardID) != 1) {
            setObjStop(1);
            m_pCam->changeCamMode(5, 0);
        }
        m_iSubState = 4;
        break;
    case 4:
        if (m_pPLM->isCompleteLoadCard(m_uiCardID) != 1) break;
        if (!m_pPLM->isFriendReady()) {
            m_pPLM->createFriend(m_pPLM->getFriendID_toCard(m_uiCardID), NULL);
            break;
        }
        if (!isMotionLoop()) break;
        setObjStop(0);
        m_pCam->changeCamMode(-1, 0);
        calcAppearPosFS_toAttackID();
        m_pPLM->getFriend()->setAppear();
        fadeEffect(m_hEffCast, 1);
        setEffect(g_szDpxFriend, 3, &getPos(), NULL, 0.0f);
        m_iSubState = m_uiAttackID * 5;
        setAnimForce(0x2a, -1, 0, 0, 0, 0);
        setPlayerFlag(0x100000, 0);
        m_tqStateTimer = 0;
        m_pMarker->holdTarget(8);
        procTargetMark();
        break;
    case 0x1cc:
        if (!m_pPLM->isFriendReady()) {
            m_iSubState = 3;
            break;
        }
        if (!m_bIsCanCombo) {
            m_bIsCanCombo = m_pPLM->getFriend()->setSkill(m_uiAttackID);
        }
        exeStandUp_inFriend();
        break;
    case 3:
        if (isBattleFlag(0x1000) && exeUseStock(1)) {
            if (isPlayerFlag(0x80000000)) setBGFade(1.0f, 20.0f, 0);
            break;
        }
        procSkillEnd(-1);
        break;
    }
}

INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Game/Player/PlayerRiku", setMagic__Q22PL11CPlayerRikuiii);

/* Best attempt (same structure): BULLET_PARAM field stores and the attack-power loads are scheduled differently. */
#if 0
void CPlayerRiku::setMagic(INT inState, INT inCardID, INT inSkillID) {
    FVECTOR pos;
    FVECTOR rot;
    BULLET_PARAM param;
    CBullet* pBullet;
    if (!inCardID && !inSkillID) return;
    switch (inState) {
    case 0x20a:
        if (inSkillID > 0) {
            param.init(1, 3);
            param.CollKind = COLL_KIND_PSHOT;
            param.AtkKind = m_AtkKind;
            param.AtkAttr = m_AtkAttr;
            param.iAttackPoint = m_iAttackPower;
            param.fSpeed = 0.4f;
            param.fShiftY = 2.0f;
            param.fCollRadius = 2.0f;
            param.vPos = m_vMarkPos;
            m_pCollMap->setType(COLL_TYPE_SPHERE);
            m_pCollMap->enableSliding(0);
            m_pCollMap->setRadius(param.fCollRadius);
            m_pCollMap->setStartPos(param.vPos);
            param.vPos.y -= 8.0f;
            m_pCollMap->setEndPos(param.vPos);
            if (m_pCollMap->collideMap()) {
                param.vPos = *m_pCollMap->getOutPos();
            } else {
                param.vPos.y += 8.0f;
            }
            m_vJumpStartPos = param.vPos;
            m_hEffCast = setEffect(g_szDpxImpals, 2, &m_vJumpStartPos, NULL, 0.0f);
            param.vRot = getRot();
            param.vRot.x = -1.5707964f;
            param.uiFlag = 0x20003;
            param.pEffectTbl = g_EffTbl_Skill[52];
            param.tqFadeTime = 600;
            param.fScale = 1.0f;
            param.tqCollAtkTime = 600;
            param.tqHomingTime = 600;
            CEntityFactory::getInstance()->createBullet(param, this);
        } else {
            setQuake(10.0f, 0.25f, 50);
            m_uiAttackID = 0xaf;
            m_pAtkData = &g_AttackDataList[0xaf];
            m_iAttackPower = m_iAP * CardBattle::g_pStockSkillList[m_iStockSkill].POWER[3] / 100;
            _ref(m_AtkKind) = (ATK_KIND)3;
            _ref(m_AtkAttr) = (ATK_ATTR)CardBattle::g_pStockSkillList[m_iStockSkill].ATTR;
            UINT up = _upKind(CardBattle::g_pStockSkillList[m_iStockSkill].UP_TYPE0,
                              CardBattle::g_pStockSkillList[m_iStockSkill].UP_TYPE1);
            calcEnchant_AttackPoint(m_iAttackPower, m_AtkKind, m_AtkAttr, up);
            param.init(1, 3);
            param.AtkKind = m_AtkKind;
            param.AtkAttr = m_AtkAttr;
            param.CollKind = COLL_KIND_PSHOT;
            param.pEffectTbl = g_EffTbl_Skill[53];
            param.iAttackPoint = m_iAttackPower;
            param.uiFlag = 0x30002;
            param.CollType = COLL_TYPE_CIRCLE;
            param.tqCollAtkTime = 3000;
            param.fChangeRadius = 0.15f;
            param.fCollOffsetY = 0.4f;
            param.fSpeed = 0.0f;
            param.fCollRadius = 0.0f;
            getWeaponTopPos(param.vPos);
            m_pCollMap->setType(COLL_TYPE_SPHERE);
            m_pCollMap->enableSliding(0);
            m_pCollMap->setRadius(param.fCollRadius);
            m_pCollMap->setStartPos(param.vPos);
            param.vPos.y -= 4.0f;
            m_pCollMap->setEndPos(param.vPos);
            if (m_pCollMap->collideMap()) {
                param.vPos = *m_pCollMap->getOutPos();
            } else {
                param.vPos.y += 4.0f;
            }
            param.vRot = getRot();
            pBullet = (CBullet*)CEntityFactory::getInstance()->createBullet(param, this);
            if (pBullet) m_hMagicCore = pBullet->getHandle();
        }
        break;
    case 0x210:
        param.init(1, 3);
        param.CollKind = COLL_KIND_PSHOT;
        param.AtkKind = m_AtkKind;
        param.AtkAttr = m_AtkAttr;
        param.iAttackPoint = m_iAttackPower;
        param.fShiftY = 1.0f;
        param.fSpeed = 0.0f;
        param.fCollRadius = 1.0f;
        param.vPos = m_vAppearPos;
        m_pCollMap->setType(COLL_TYPE_SPHERE);
        m_pCollMap->enableSliding(0);
        m_pCollMap->setRadius(param.fCollRadius);
        m_pCollMap->setStartPos(param.vPos);
        param.vPos.y -= 4.0f;
        m_pCollMap->setEndPos(param.vPos);
        if (m_pCollMap->collideMap()) {
            param.vPos = *m_pCollMap->getOutPos();
        } else {
            param.vPos.y += 4.0f;
        }
        param.vRot = getRot();
        param.tqCollAtkTime = 1000;
        param.uiFlag = 0x20002;
        param.pEffectTbl = g_EffTbl_Skill[62];
        param.fScale = 1.5f;
        pBullet = (CBullet*)CEntityFactory::getInstance()->createBullet(param, this);
        if (pBullet) pBullet->setAttackID(m_pWeapon->getCollAttackID());
        break;
    case 0x162:
        getWeaponTopPos(pos);
        param.init(2, 3);
        param.CollKind = COLL_KIND_PSHOT;
        param.iAttackPoint = m_iAttackPower;
        param.AtkKind = m_AtkKind;
        param.AtkAttr = m_AtkAttr;
        param.uiFlag = 0x21008140;
        param.fSpeed = m_pAtkData->MAX_SPEED * 0.01f;
        param.tqHomingTime = 9000;
        param.fMaxDist = m_pAtkData->WORK0;
        param.fHomingRadius = m_pAtkData->RADIUS;
        param.vPos = pos;
        param.fHomingRate = m_pAtkData->WORK1;
        param.tqFadeTime = 1000;
        param.vRot = getRot();
        param.vRot.y = _limitPI(calcDirection(pos));
        param.pEffectTbl = g_EffTbl_Skill[m_pAtkData->EFFECT];
        param.fCollRadius = m_pAtkData->COLL_START_FRAME * 0.01f;
        pBullet = (CBullet*)CEntityFactory::getInstance()->createBullet(param, this);
        if (pBullet) {
            pBullet->setBodyRadius(0.1f, 0.0f, 0, COLL_KIND_NONE);
            if (!m_iCount) {
                m_pWeapon->setCollAttackID(pBullet->getAttackID());
            } else {
                pBullet->setAttackID(m_pWeapon->getCollAttackID());
            }
        }
        break;
    }
}
#endif

} // namespace PL

void CPlayer::initDuel(INT inFlag) {
    TRACE("CPlayer:: initDuel(%d)\n", inFlag);
    setPlayerState(DUEL);
    setBattleFlag(0, 0x8000);
    if (!inFlag) m_iSubState = 9000;
    setPlayerFlag(0, 0x20000);
    setPlayerFlag(1, 0);
    setBattleFlag(0, 8);
    ((_CollBodyView*)m_collBody)->off();
    m_bIsAttackHit = FALSE;
    m_bNextTrg = FALSE;
    m_iNextAttack = 0;
    setBattleFlag(0, 0x30000);
    ((_CollBodyView*)m_collBody)->clearDamage();
    offAttack();
    onWeapon(0);
    _ref(m_uiAttackKind) = 0;
    _ref(m_pAtkData) = &PL::g_AttackDataList[0];
    _ref(m_uiAttackID) = 0;
    m_fMoveSpdXZ = 0.0f;
    m_fJumpGravity = PL::g_fGravity;
    m_fWalkSpeed = 0.0f;
    m_JumpState = 3;
    setPlayerFlag(0, 0x2800000);
    setTrgFlag(0, 0x40000);
    m_pCam->enableRikuShiftY();
    m_hMark = m_hTarget = m_pCB->getDuelEnemy();
    m_pCam->changeNowPlayer(getHandle(), m_hTarget);
    m_pCam->changeCamMode(4, 0);
    setPlayerFlag(0x10000, 0);
    m_pMarker->setLockon(m_hTarget, 0);
    m_pMarker->onPush(7);
    setTimer(3000);
    TRACE("initDuel:  Duel Entry...\n");
    m_pCB->setDuelEntry(1);
    m_szEffectName = NULL;
}

void CPlayer::exeDuel() {
    BOOL bSpot = FALSE;
    TIMEQ frame = getMotionCurrentTime() / 50;
    setBattleFlag(0, 8);
    switch (m_iSubState) {
    case 9000:
        setPlayerFlag(0x400000, 0);
        if (isBattleFlag(4)) {
            setPushon(0);
            m_iSubState = 0;
            m_pCB->setDuelEntry(0);
            setBattleFlag(0, 0x8000);
            resetAction(4);
            m_pMarker->cancelLockon(m_hLockon, 0);
            initEnd();
            return;
        }
        if (isConditionFlag(0x2000000) || isTrgFlag(0x100000)) {
            TRACE("exeDuel:  Duel Entry... Cancel!! (DP=0)\n");
            setPushon(0);
            m_iSubState = 0;
            m_pCB->setDuelEntry(0);
            setBattleFlag(0, 0x8000);
            resetAction(4);
            m_pMarker->cancelLockon(m_hLockon, 0);
            TRACE("CPlayer:: exeDuel -> initTransform(0)\n");
            initTransform(0);
            return;
        }
        procTargetMark();
        procAttackMove(frame, 0.0f, 0.0f, 2, 0.15f);
        if (procFlowIdle(1)) {
            FVECTOR pos = getPos();
            pos.y += 0.5f;
            setOldPos(pos);
        }
        if (getTimer() <= 2500) {
            if (isTrgStock()) {
                TRACE("exeDuel:  Duel Entry TRG!\n");
                m_pMarker->setFlag(FALSE);
                setPushon(0);
                m_iSubState = 9002;
                clearSpeed();
                m_pCB->setDuelStart(0);
                setBGFade(PL::g_vBGFadeColor[6], 20.0f, 0);
                setEnemyStop(m_hTarget, 1);
                PlayerCamera::CPlayerCamera::setBattleCamData(getHandle(), &PL::g_DuelCamParam);
                m_pCam->changeNowPlayer(getHandle(), m_hMark);
                m_pCam->changeCamMode(4, 0);
                setTimer(3000);
            } else if (getTimer() <= 1500 && (getTimer() <= 0 || isTrgAttack())) {
                TRACE("exeDuel:  Duel Entry NG!\n");
                setPushon(0);
                m_iSubState = 0;
                m_pCB->setDuelEntry(0);
                setBattleFlag(0, 0x8000);
                resetAction(4);
                m_pMarker->cancelLockon(m_hLockon, 0);
                initStandUp(0);
            }
        }
        break;
    case 9002:
        setPlayerFlag(0x400000, 0);
        if (isAir()) {
            if (procFlowIdle(1)) {
                FVECTOR pos = getPos();
                pos.y += 0.5f;
                setOldPos(pos);
            }
        } else {
            setAnim(0x38, -1, 0);
            if (!m_hEffCast) {
                m_hEffCast = setEffect(PL::g_szDpxDuel, 3, &getPos(), NULL, 0.0f);
            }
            bSpot = TRUE;
        }
        procTargetMark();
        procAttackMove(frame, 0.0f, 0.0f, 2, 0.15f);
        if (getTimer() > 0) break;
        if (isAir()) setLand(5.0f, 1.0f, 1);
        TRACE("exeDuel:  Duel Start!\n");
        m_iSubState = 9003;
        setAnim(0x38, -1, 0);
        if (!m_hEffCast) {
            m_hEffCast = setEffect(PL::g_szDpxDuel, 3, &getPos(), NULL, 0.0f);
        }
        m_pCB->setDuelStart(1);
        break;
    case 9003:
        setPlayerFlag(0x400000, 0);
        bSpot = TRUE;
        if (isBattleFlag(0x10000)) {
            setBattleFlag(0, 0x10000);
            setEnemyStop(0, 0);
            setBattleFlag(0, 0x443f90);
            setPlayerFlag(0, 0x20c2000);
            setBattleFlag(0, 0x80000);
            setPlayerFlag(0, 0x400001);
            setBattleFlag(0, 0x10200000);
            setBattleFlag(0, 0x4060);
            setBattleFlag(0, 0x130000);
            setBattleFlag(0, 0x8000000);
            m_iSubState = 0;
            m_pMove->setFlag(1, FALSE);
            m_pMove->setFlag(0x40, TRUE);
            resetAction(0);
            m_pMarker->cancelLockon(m_hLockon, 0);
            m_pMarker->setFlag(TRUE);
            initStandUp(0);
            setConditionFlag(0x8000, 0);
            setFaceAnim(3, 0);
            initBadCondition();
            m_pCB->destroyPlayCard(1, 0);
            setBattleFlag(0, 0x10200000);
            break;
        }
        if (isBattleFlag(0x20000)) {
            setBattleFlag(0, 0x20000);
            setEnemyStop(0, 0);
            m_pRevolver->setPlayerDuelPlayStock();
            m_iSubState = 9004;
            fadeEffect(m_hEffCast, 1);
            setPlayerFlag(0, 0x10000);
            m_pMarker->setMode(1);
            m_pMarker->chkLockon(m_hTarget);
            m_hLockon = m_hTarget;
            m_pMarker->holdTarget(8);
            m_pMarker->setFlag(TRUE);
            PlayerCamera::CPlayerCamera::setBattleCamData(0, NULL);
            m_pCam->changeNowPlayer(0, m_hLockon);
            m_pCam->changeCamMode(4, 0);
            setBattleFlag(0x1000, 0);
            m_iStepStockSkill = 0;
            startStockSkillAction();
            m_PlayerState = DUEL;
            m_pCB->setFlag(0xc0000000, 0);
            break;
        }
        if (isTrgFlag(0x10) && chkChargeMove(0)) break;
        if (isTrgAttack()) m_pRevolver->setPlayerDuelCard(0);
        break;
    case 9005:
        break;
    case 9001:
    case 9004:
    default:
        exeStockSkill();
        if (m_PlayerState != DUEL) {
            setPlayerFlag(0, 1);
            m_pCB->setFlag(0, 0xc0000000);
            setBattleFlag(0, 0x8000000);
        }
        break;
    }
    if (bSpot) updateSpotLight(m_hEffCast);
}

// SOURCE DATA: .data .rodata
