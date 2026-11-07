
#define GPRIM_H_PATH "System/Graphic/GPrim.h" /* MATCH: this unit names Gprim.h so (header default "System/Graphic/Gprim.h") */
#include "System/Graphic/Anim.h"
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
/* System/TaskMan.h's container overflow message comes after them here. */
#include "System/TaskMan.h"



#include "System/Graphic/Effect/effect.h"
#include "Game/Enemy/BattleEnemy.h"
#include "Game/Player/PlayerDRiku.h"
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
extern UINT g_uiStartCondition;
extern BULLET_EFFECT_INFO g_EffTbl_Skill[67][4]; /* Game/Player/PlayerAttack.h */
extern const BULLET_EFFECT_INFO g_EffTbl_HitMark[27];
extern FVECTOR g_OffsetTbl_StockSkill[8];
extern FVECTOR g_RotTbl_Magic[8];
extern FVECTOR g_vBGFadeColor[7];
extern FLOAT g_fDefAttr[];
extern FLOAT g_fGravity;
extern TIMEQ g_CardBreakTime[];

#if 1
static LPCSTR g_szBoneLxRot = "lx_rot";
static LPCSTR g_szDpxAsiato = "asiato";
static LPCSTR g_szDpxEX1580 = "ex_1580";
static LPCSTR g_szDpxSpeedBreak = "speed_break";
static LPCSTR g_szDpxFriend = "friend";
static LPCSTR g_szDpxMaelstrom = "maelstrom";
static LPCSTR g_szDpxSEAppear = "souleater";
static LPCSTR g_szDpxImpals = "impals";

PL_PARAM g_DRikuParam = {
    0x20279, 0xa000259, 0.3f, 1.2f, 1.3f, 6.0f, 1.8f, 32.0f, 36.0f, 300, 0, 0,
};
#else
extern LPCSTR g_szBoneLxRot;
extern LPCSTR g_szDpxAsiato;
extern LPCSTR g_szDpxEX1580;
extern LPCSTR g_szDpxSpeedBreak;
extern LPCSTR g_szDpxFriend;
extern LPCSTR g_szDpxMaelstrom;
extern LPCSTR g_szDpxSEAppear;
extern LPCSTR g_szDpxImpals;
#endif
} // namespace PL


/* Stores/loads through a reference: gcc then can't tell them apart from the globals' and other members'
 * accesses, which keeps the original's order (the original sets these members through inline helpers). */
template <class T> static inline T& _ref(T& v) {
    return v;
}
static inline UINT _upKind(INT inType0, INT inType1) {
    return (inType0 > 0 ? 1 << (inType0 - 1) : 0) | (inType1 > 0 ? 1 << (inType1 - 1) : 0);
}

static inline FLOAT _limitPI(FLOAT r) {
    return (r < -3.1415927f) ? r + 6.2831855f : (r > 3.1415927f) ? r - 6.2831855f : r;
}

static inline FLOAT _abs(FLOAT inValue) {
    FLOAT ret;
    __asm__ volatile("abs.s %0, %1" : "=f"(ret) : "f"(inValue));
    return ret;
}
void SubVectorXYZ(FVECTOR& v0, const FVECTOR& v1, const FVECTOR& v2);
void Normalize(FVECTOR& v0, const FVECTOR& v1);
FLOAT InnerProduct(const FVECTOR& v0, const FVECTOR& v1);
void ScaleVector(FVECTOR& v0, const FVECTOR& v1, FLOAT s);
extern "C" float atan2f(float y, float x);
static inline FLOAT _sqrtf(FLOAT inValue) {
    FLOAT ret;
    asm volatile("sqrt.s %0, %1" : "=f"(ret) : "f"(inValue));
    return ret;
}
static inline INT _getNowCamMode() {
    return PlayerCamera::CPlayerCamera::getNowCamMode();
}

#include "System/Rcf.h"

namespace Friend {
extern BCAM_PARAM g_HolyBurstCamParam;
}

void ScaleVectorXYZ(FVECTOR& outV, const FVECTOR& inV, FLOAT inScale);
void AddVectorXYZ(FVECTOR& v0, const FVECTOR& v1, const FVECTOR& v2);
void ApplyMatrix33(FVECTOR& v0, const FMATRIX& m0, const FVECTOR& v1);
extern "C" void sceVu0RotMatrixY(FMATRIX& m0, FMATRIX& m1, float ry);
extern "C" void sceVu0RotMatrixX(FMATRIX& m0, FMATRIX& m1, float rx);
extern "C" void sceVu0UnitMatrix(FMATRIX& m);
extern "C" float sinf(float);
extern "C" float cosf(float);
void ApplyMatrixXYZ(FVECTOR& v0, const FMATRIX& m0, const FVECTOR& v1);
namespace Math3D {
FLOAT LimitPI(FLOAT pai);
}

/* Coord.h's matrix wrappers (setMagic): called directly, cse keeps the matrix address in a saved register
 * across the calls; the original recomputes it for each call. */
static inline void _unitMatrix(FMATRIX& m) {
    sceVu0UnitMatrix(m);
}
static inline void _rotMatrixX(FMATRIX& m, FLOAT r) {
    sceVu0RotMatrixX(m, m, r);
}
static inline void _rotMatrixY(FMATRIX& m, FLOAT r) {
    sceVu0RotMatrixY(m, m, r);
}

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

#include "Game/GameWork.h"

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

#line 197
CPlayerDRiku::CPlayerDRiku(CTask* inParent, UINT inChara, INT inEx) : CPlayer(inParent, inChara, inEx) {
    m_pParam = &g_DRikuParam; if (!m_pParam) AssertApp(__FILE__, __LINE__, "m_pParam", 0);
    _ref(m_pRiku) = NULL;
    _ref(m_hMagicCore) = 0;
    _ref(m_nAttackCount) = 0;
    m_iBoneLHand = getBoneIndex(g_szBoneLxRot, 0);
    for (INT i = 0; i < 2; i++) {
        m_vWarpPos[i].init();
        m_vWarpNormal[i].init();
    }
    if (m_uiPlayerFlag & 0x20000000) {
        setEntityFlag(1, 0);
    }
}

CPlayerDRiku::~CPlayerDRiku() {}

void CPlayerDRiku::stateInitial() {
    setPlayerState((PlayerState)0);
    m_uiStep = 0;
    importStatus();
    setDefAttr(g_fDefAttr);
    m_fCardDraw = m_fPrizeDraw = 5.0f;
    createLegIK(0.17f, 0.06f);
    setFootEffectIK(g_szDpxAsiato, 0);
    setGroundIK(0);
    makeAnimIndex(m_uiCharaID);
    initStartPos();
    makeWeapon();
    initActionSkill();
    setAnimForce(0x2a, 0, 0, 0, 0, 0);
    if (m_uiPlayerFlag & 0x20000000) {
        changeState(2);
        return;
    }
    m_iDP = m_iMaxDP;
    makeCockpit();
    m_pGauge->setDMode(1, m_iDP, m_iMaxDP);
    makeRevolver();
    m_pRevolver->changeDarkMode(getHandle(), 1);
    setPlayerFlag(0, 0x40000);
    setConditionFlag(0x200000, 0x2000000);
    setConditionFlag(0x400000, 0);
    setTrgFlag(0, 0x100000);
    m_pCB->disableDuelEntry(0);
    if (CGameWork::getFlag(1, 0)) {
        changeState(3);
    }
}

void CPlayerDRiku::beginStateBattle() {
    TRACE("CPlayerDRiku:: beginStateBattle()\n");
    m_iState = 1;
    m_uiStep = 0;
    setActionFlag(m_pParam->USE_BTL_ACTION, 0);
    initActionSkill();
    setBattleFlag(1, 0);
    setAnimForce(0x2a, -1, 0, 0, 0, 0);
    onWeapon(0);
    if (m_uiPlayerFlag & 0x20000000) {
        setPlayerState(STAND);
    } else {
        m_pPLM->setReqLoadFirst();
        initStartWait();
        if (g_uiStartCondition & 8) {
            setConfuse(1, 0);
        }
        g_uiStartCondition = 0;
    }
    if (m_pCB) {
        m_pCB->disableDuelEntry(0);
    }
}

void CPlayerDRiku::stateBattle() {
    CPlayer::stateBattle();
    if (DebugWork::g_dw.player.iDP1 && m_iDP >= 2) {
        m_pGauge->setDP(1, m_iMaxDP);
        m_iDP = 1;
    }
}

void CPlayerDRiku::endStateBattle() {
    setActionFlag(0, m_pParam->USE_BTL_ACTION);
    setBattleFlag(0, 1);
    setPlayerFlag(0x1000, 0);
    ((_CollBodyView*)m_collBody)->m_enableBody = 0;
    offAttack();
    offWeapon();
    m_uiAttackKind = 0;
    m_uiPreAttackKind = 0;
    m_uiAttackID = 0;
    if (m_pMarker) {
        m_pMarker->off();
    }
}

void CPlayerDRiku::beginStateWait() {
    TRACE("CPlayerDRiku:: beginStateWait()\n");
    m_iState = 2;
    if (m_uiPlayerFlag & 0x20000000) {
        clearSpeed();
        if (!m_pRiku || !(((CPlayer*)m_pRiku)->m_uiConditionFlag & 0x400000)) {
            setEntityFlag(1, 0);
            setAnimForce(0x2a, 0, 0, 0, 0, 0);
        }
        m_WalkMode = 0;
        setPlayerFlag(0x401001, 0);
        ((_CollBodyView*)m_collBody)->offBody();
    }
}

void CPlayerDRiku::stateWait() {
    if (m_uiPlayerFlag & 0x20000000) {
        setPlayerFlag(0x400000, 0);
        if (m_uiPlayerFlag & 0x40000) {
            initTransform(1);
        } else if (m_PlayerState == TRANSFORM) {
            exeTransform();
        }
    }
}

void CPlayerDRiku::endStateWait() {}
void CPlayerDRiku::onWeapon(INT inEffect) {
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

void CPlayerDRiku::offWeapon() {
    setPlayerFlag(2, 0);
    setDispOffFlag(3, 0);
}

void CPlayerDRiku::setStateSE(INT inState, INT inKind) {
    INT SEID = -1;
    switch (inState) {
    case 1:
        switch (m_uiAttackID) {
        case 0x9e:
            SEID = 0x132;
            break;
        case 0x9f:
            SEID = 0x134;
            break;
        case 0xa0:
            SEID = 0x133;
            break;
        case 0xa1:
        case 0xa2:
            SEID = 0x134;
            break;
        case 0xa3:
            SEID = 0x132;
            break;
        case 0xa4:
            SEID = 0x134;
            break;
        case 0xa6:
        case 0xa7:
            SEID = 0x132;
            break;
        case 0xa9:
        case 0xaa:
            SEID = 0x134;
            break;
        case 0xb5:
        case 0xb6:
        case 0xb7:
        case 0xb8:
        case 0xb9:
        case 0xba:
        case 0xbb:
            SEID = 0x134;
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

INT CPlayerDRiku::getStateSESeq(INT inState) {
    INT seqID = 0;
    switch (inState) {
    case 3:
    case 6:
        switch (m_uiAttackID) {
        case 0x5c:
        case 0x74:
        case 0x75:
        case 0x77:
            seqID = 1;
            break;
        }
        break;
    }
    return seqID;
}

void CPlayerDRiku::startDMode(CPlayerRiku* inRiku) {
    m_pRiku = inRiku;
    setPlayerFlag(0x40000, 0);
}
void CPlayerDRiku::initTransform(INT inFlag) {
    if (m_bIsDead) return;
    TRACE("CPlayerDRiku:: initTransform(%d)\n", inFlag);
    setPlayerState(TRANSFORM);
    clearSpeed();
    offAttack();
    setBattleFlag(0, 8);
    setPlayerFlag(0, 0x40000);
    setConditionFlag(0x200000, 0x2000000);
    _ref(m_tqCardBreakTime) = g_CardBreakTime[7];
    m_fJumpGravity = g_fGravity;
    _ref(m_fMoveSpdXZ) = 0.0f;
    _ref(m_fWalkSpeed) = 0.0f;
    m_JumpState = 3;
    setMDFooting(0);
    fadeEffAttack(0);
    if (m_uiTrgFlag & 0x100000) {
        setConditionFlag(0x2000000, 0);
    }
    setTrgFlag(0, 0x100000);
    if (!inFlag) {
        m_iSubState = 4;
        return;
    }
    m_iSubState = 1;
    FVECTOR pos = m_pRiku->getPos();
    warp(pos);
    setRot(m_pRiku->getRot());
    updateCoord();
    coordFix();
    pos.y += 0.5f;
    setOldPos(pos);
    setEntityFlag(0, 1);
    onWeapon(0);
    setAnimForce(0x37, -1, 0, 0, 0, 0);
    transferControl();
    m_pCam->changeNowPlayer(m_pRiku->getHandle(), getHandle());
    m_pCam->changeCamMode(4, 0);
    m_pRevolver->changeMode(0, 1);
    m_pRevolver->chargeReloadCard(1);
    setTimer(400);
    setupFade(0.0f, 1.0f, 100, -1);
    m_fScale = 1.0f;
    FVECTOR vScale = getScale();
    vScale.x = m_fScale;
    vScale.y = m_fScale;
    vScale.z = m_fScale;
    setScale(vScale);
    setPlayerFlag(0, 0x1000);
}
void CPlayerDRiku::exeTransform() {
    setPlayerFlag(0x400000, 0);
    setBattleFlag(0, 8);
    switch (m_iSubState) {
    case 1:
        if (getTimer() > 0) {
            m_pRevolver->chargeReloadCard(2);
        }
        if (!isMotionEnd()) break;
        setAnim(0x2a, -1, 0);
        changeState(1);
        setPlayerFlag(0, 1);
        m_pCam->changeNowPlayer(0, 0);
        m_pCam->changeCamMode(3, 0);
        enableControl();
        m_pMarker->on();
        m_pCB->disablePlayCard(0);
        break;
    case 4:
        if (m_pCB) {
            TRACE("CPlayerDRiku::exeTransform RETURN \x81\xa6" "Duel ... Cancel!\n");
            m_pCB->setDuelEntry(-1);
        }
        m_pRiku->endDMode(this);
        m_iSubState = 5;
        setPlayerFlag(0x1001, 0);
        ((_CollBodyView*)m_collBody)->offBody();
        offWeapon();
        m_pMarker->off();
        m_pCB->destroyPlayCard(1, 0);
        setBattleFlag(0, 0x10200000);
        if (isBattleFlag(2)) {
            setBattleFlag(0, 2);
            m_tqEscape = 0;
            if (m_pEscape) m_pEscape->destroy();
            setBGFade(1.0f, 0.0f, 0);
        }
        setConditionFlag(0, 0x2000000);
        changeState(2);
        return;
    case 5:
        if (((CPlayer*)m_pRiku)->isConditionFlag(0x400000)) {
            m_pCam = NULL;
            m_pGauge = NULL;
            m_pMarker = NULL;
            m_pEscape = NULL;
            m_pCB = NULL;
            setPrizeHealing(0);
            setupFade(1.0f, 0.0f, 1500, -1);
            setAnimSpeed(*(FLOAT*)((BYTE*)(CEntity*)this + 0x110)); /* PARTIAL: CEntity::m_fEndAlpha (private) */
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
        }
        if (m_fScale > 0.0f) {
            m_fScale += getFrameRateF() * 0.02f;
            if (m_fScale <= 0.0f) m_fScale = 0.0f;
            FVECTOR vScale = getScale();
            vScale.x = m_fScale;
            vScale.y = m_fScale;
            vScale.z = m_fScale;
            setScale(vScale);
        }
        if (!isFade() && !m_pCB) {
            m_iSubState = 0;
            setEntityFlag(1, 0);
            setAnimForce(0x2a, 0, 0, 0, 0, 0);
        }
        break;
    }
}
void CPlayerDRiku::transferControl() {
    _ref(g_pPlayer) = this;
    _ref(m_pInstance) = this;
    m_iMaxHP = m_pRiku->m_iMaxHP;
    m_iHP = m_pRiku->m_iHP;
    m_iAP = m_pRiku->m_iAP;
    m_iEXP = m_pRiku->m_iEXP;
    m_iLV = m_pRiku->m_iLV;
    m_iMP = m_pRiku->m_iMP;
    m_iMaxDP = m_pRiku->m_iMaxDP;
    m_iDP = m_iMaxDP;
    m_vStartPos = m_pRiku->m_vStartPos;
    setConditionFlag(0x400000, 0);
    UINT flag = m_pRiku->m_uiBattleFlag;
    TRACE("CPlayerDRiku::transferControl (Riku->DRiku) uiBattleFlag = %x,\n", flag);
    setBattleFlag(flag, 0);
    if (isBattleFlag(0x8000)) {
        TRACE("CPlayerDRiku::transferControl uiBattleFlag [CARD_DUEL] clear !!\n");
    }
    setBattleFlag(0, 8);
    setBattleFlag(0, 0x443f90);
    setBattleFlag(0, 0x80000);
    setBattleFlag(0, 0x10200000);
    setBattleFlag(0, 0x4060);
    setBattleFlag(0, 0x130000);
    setBattleFlag(0, 0x8000);
    m_bIsDead = m_pRiku->m_bIsDead;
    m_pCam = m_pRiku->m_pCam;
    m_pGauge = m_pRiku->m_pGauge;
    m_pMarker = m_pRiku->m_pMarker;
    m_pEscape = m_pRiku->m_pEscape;
    m_pGauge->setDMode(1, m_iDP, m_iMaxDP);
    m_pCB = (CardBattle::CCardBattle*)CTaskMan::getSysTask(0x16);
    m_pCB->transferPlayerEnchant(m_pRiku);
    m_pCB->disableDuelEntry(0);
    m_pRevolver = m_pRiku->m_pRevolver;
    m_pRevolver->changeDarkMode(getHandle(), 1);
    setConditionFlag(0, 0x881803f);
    if (m_pRiku->m_tqConfuse > 0) {
        setConfuse(1, m_pRiku->m_tqConfuse);
    }
    PLM::addIntBR(0x41);
}

void CPlayerDRiku::exeAvoid() {
    exeSpeedStep();
}

void CPlayerDRiku::exeSpeedStep() {
    setBattleFlag(0, 8);
    FVECTOR vel = m_pMove->getVel();
    if (isAir()) {
        vel.y = m_fJumpSpdY += m_fJumpGravity * m_fFrameRate;
        m_pMove->setVel(vel);
    } else {
        if (m_fJumpSpdY < 0.0f) m_fJumpSpdY = 0.0f;
        vel.y = m_fJumpSpdY;
        m_pMove->setVel(vel);
    }
    if (getMotionCurrentTime() >= m_pParam->AVOID_TQ_MUTEKI) {
        setPlayerFlag(0, 1);
    }
    if (getMotionCurrentTime() >= m_pParam->AVOID_TQ_CANCEL) {
        if (isActionFlag(0x2000000) && isTrgFlag(0x10)) {
            chkChargeMove(0);
        }
        if (chkTrgAction(0, 0)) {
            m_pMarker->holdTarget(0);
            return;
        }
    }
    if (isMotionEnd()) {
        initIdle(-1, 1);
    }
}
void CPlayerDRiku::setAttack() {
    CEntity* pTarget;
    FLOAT distY;
    FLOAT dist;
    FLOAT dir;
    FLOAT rate;
    FLOAT power;
    CEffect* effect;
    if (m_hTarget > 0) pTarget = (CEntity*)CTaskMan::getTask(m_hTarget);
    else pTarget = NULL;
    distY = 0.0f;
    dist = 0.0f;
    dir = 0.0f;
    if (pTarget) {
        pTarget->getLockOnPos(m_vMarkPos);
        distY = m_vMarkPos.y - m_vCenterPos.y;
        distY -= 1.0f;
        dist = calcDistance(*pTarget);
        dir = calcDirection(*pTarget) - m_fDir;
        dir = _abs(dir);
        m_hMark = m_hTarget;
    } else {
        m_iSubState = 3;
        m_vMarkPos.set(0.0f, 0.0f, 0.0f);
        m_hMark = 0;
    }
    m_fJumpSpdY = 0.0f;
    if (isPlayerFlag(0x80000)) {
        m_uiAttackKind = 6;
    } else if (!(m_iAttackNo < m_iMaxCombo - 1)) {
        if (m_uiPreAttackKind == 7 || isAir()) m_uiAttackKind = 7;
        else m_uiAttackKind = 4;
    } else {
        switch (m_uiPreAttackKind) {
        case 1:
        case 2:
        case 6:
            if (distY > 0.0f) m_uiAttackKind = 5;
            else if (isAir()) m_uiAttackKind = 5;
            else m_uiAttackKind = 3;
            break;
        case 3:
        case 5:
            m_uiAttackKind = isAir() ? 6 : 1;
            goto kindDone;
        case 7:
            m_uiAttackKind = 7;
            break;
        default:
            if (isAir()) m_uiAttackKind = 7;
            else if (!pTarget) m_uiAttackKind = 1;
            else if (distY > 0.0f) m_uiAttackKind = 5;
            else if (dist > 3.5f) m_uiAttackKind = 3;
            else if (dir > 0.7853982f) {
                if (dir < 2.3561945f) m_uiAttackKind = 2;
                else m_uiAttackKind = 1;
            } else m_uiAttackKind = 1;
            break;
        }
    kindDone:
        if (m_uiAttackKind == 1) {
            if (getFrontAirHeight(NULL, 1.0f, 2.0f, 0.0f) > 0.0035f) m_uiAttackKind = 6;
        }
    }
    m_uiAttackID = PL::g_DRikuAttackList[m_uiAttackKind];
    if (m_iAttackNo == 0) {
        if ((UINT)((USHORT)DebugWork::g_dw.player.iAttackID - 0x9E) < 7) m_uiAttackID = DebugWork::g_dw.player.iAttackID;
    }
    m_pAtkData = &PL::g_AttackDataList[m_uiAttackID];
    switch (m_uiAttackID) {
    case 0xA2:
        power = 0.0f;
        if (!isAir()) {
            if (distY < 2.0f) power = (FLOAT)m_pAtkData->WORK1;
            else power = distY * 10.0f + 24.0f;
        } else {
            if (power <= distY) power = distY * 10.0f + 4.0f;
        }
        if (power > 40.0f) power = 40.0f;
        m_fJumpSpdY = PL::g_fAntiGravity * power;
        procGravity(0, 1, 1);
        break;
    }
    setAnimForce(m_pAtkData->ANIM_ID, 0, 1, 0, 0, 0);
    if (isConditionFlag(0x4000000)) {
        FVECTOR rot;
        setAnimSpeed(1.5f);
        rot.set(1.5707964f, 0.0f, 0.0f, 1.0f);
        if (!m_hEffAttack[0]) {
            m_hEffAttack[0] = attachEffect(PL::g_szDpxEX1580, 0x2A, &Crd(m_iBoneChest), NULL, &rot, 0.0f);
            effect = CEffectMan::getEffect(m_hEffAttack[0]);
            if (effect) effect->m_iFlag |= 4;
        }
        if (!m_hEffAttack[1]) {
            m_hEffAttack[1] = attachEffect(PL::g_szDpxEX1580, 0x2D, &Crd(m_iBoneChest), NULL, NULL, 0.0f);
            effect = CEffectMan::getEffect(m_hEffAttack[1]);
            if (effect) effect->m_iFlag |= 4;
        }
        if (!m_hEffAttack[2]) {
            m_hEffAttack[2] = attachEffect(PL::g_szDpxSpeedBreak, 4, &Crd(m_iBoneChest), NULL, NULL, 0.0f);
            effect = CEffectMan::getEffect(m_hEffAttack[2]);
            if (effect) effect->m_iFlag |= 4;
        }
    }
    if (isConditionFlag(0x100)) setAnimSpeed(2.0f);
    m_bDmgMuteki = FALSE;
    if (m_uiCardID == 0) {
        m_iAttackPower = m_iAP;
        m_AtkKind = (ATK_KIND)m_pAtkData->DAMAGE;
        m_AtkAttr = (ATK_ATTR)0;
    } else {
        INT power2;
        power2 = (*(BC_PARAM**)&CardBattle::g_pBattleCardList)[m_uiCardID].POWER[m_pAtkData->POWER];
        power2 += m_iComboBoost;
        if (isConditionFlag(0x4000000)) power2 += power2 / 2;
        m_iAttackPower = m_iAP * power2 / 100;
        m_AtkKind = (ATK_KIND)m_pAtkData->DAMAGE;
        if (m_iAttackNo == 1) m_AtkKind = (ATK_KIND)7;
        _ref(m_AtkAttr) = (ATK_ATTR)(*(BC_PARAM**)&CardBattle::g_pBattleCardList)[m_uiCardID].ATTR;
        UINT up = _upKind((*(BC_PARAM**)&CardBattle::g_pBattleCardList)[m_uiCardID].UP_TYPE0, (*(BC_PARAM**)&CardBattle::g_pBattleCardList)[m_uiCardID].UP_TYPE1);
        calcEnchant_AttackPoint(m_iAttackPower, m_AtkKind, m_AtkAttr, up);
    }
    m_bIsCanCombo = m_pAtkData->FLAG & 1;
    m_uiPreAttackKind = m_uiAttackKind;
    rate = m_fAnimSpeed * 100.0f;
    m_tqAttackTimer = m_pAnim->getTotalTqCrd() * 100 / (INT)rate + 1000;
    m_iAttackNo++;
}
void CPlayerDRiku::initJump2nd() {
    if (isBattleFlag(0x200000)) return;
    setPlayerState(JUMP_2ND);
    setBattleFlag(0, 8);
    setPlayerFlag(0x80000, 0);
    setAnimForce(0x1b, -1, 0, 1, 0, 0);
    setPlayerFlag(0x2800000, 0);
    m_JumpState = 2;
    setPlayerFlag(0x100000, 0);
    _ref(m_fJumpPow) = m_pParam->JUMP_POWER;
    m_fMoveSpdXZ = m_fJumpSpdXZ;
    m_uiAttackID = 0xa5;
    m_pAtkData = &g_AttackDataList[m_uiAttackID];
    m_fJumpGravity = g_fGravity * 0.16f;
    m_pMarker->holdTarget(8);
    CEntity* pTarget = m_hTarget > 0 ? (CEntity*)CTaskMan::getTask(m_hTarget) : NULL;
    FVECTOR rot = getRot();
    if (pTarget) {
        m_fDir = pTarget->getRot().y;
        pTarget->getLockOnPos(m_vMarkPos);
        calcAppearPos(-2.0f, 0.5f, &m_vMarkPos);
        m_vMarkPos = m_vAppearPos;
        m_hMark = m_hTarget;
        m_fJumpDir = calcDirection(m_vMarkPos);
        rot.y = m_fJumpDir;
        m_fDir = m_fJumpDir;
    } else {
        calcAppearPos(2.0f, 0.5f, &m_vCenterPos);
        m_hMark = 0;
        m_vMarkPos = m_vAppearPos;
        m_fJumpDir = rot.y;
        FLOAT r = rot.y + 3.1415927f;
        FLOAT d;
        if (r < -3.1415927f) d = r + 6.2831855f;
        else {
            d = r;
            if (d > 3.1415927f) d -= 6.2831855f;
        }
        m_fDir = d;
    }
    setRot(rot);
    m_fDistXZ = m_fDistY = 0.0f;
    m_fJumpSpdY = g_fAntiGravity * m_fJumpPow;
}
void CPlayerDRiku::exeJump2nd() {
    TIMEQ frame = getMotionCurrentTime() / 50;
    FVECTOR vel = m_pMove->getVel();
    FVECTOR vec;
    setBattleFlag(0, 8);
    switch (m_JumpState) {
    case 2: {
        BOOL bFloat = TRUE;
        m_fJumpGravity = g_fGravity;
        m_fJumpSpdY += m_fJumpGravity * m_fFrameRate;
        if (m_hMark > 0) {
            SubVectorXYZ(vec, m_vMarkPos, getPos());
            m_fDistY = _abs(vec.y);
            if (vec.y < -1.0f) {
                if (m_fJumpSpdY > 0.0f) {
                    if (m_fDistY < 3.0f) m_fJumpSpdY *= g_fJumpStopRate;
                    else m_fJumpSpdY = 0.0f;
                }
                bFloat = FALSE;
            }
        }
        if (bFloat && m_fJumpSpdY < 0.0f) {
            if (frame < m_pAtkData->MOVE_END_FRAME) m_fJumpSpdY = 0.0f;
        }
        vel.y = m_fJumpSpdY;
        FLOAT rate = 0.0f;
        FVECTOR rot = getRot();
        FLOAT dirY = rot.y;
        CEntity* pTarget = m_hTarget > 0 ? (CEntity*)CTaskMan::getTask(m_hTarget) : NULL;
        if (pTarget) {
            if (frame < 6) {
                rate = 1.0f;
                m_fDir = pTarget->getRot().y;
                pTarget->getLockOnPos(m_vMarkPos);
                calcAppearPos(-2.0f, 0.5f, &m_vMarkPos);
                m_vMarkPos = m_vAppearPos;
                m_hMark = m_hTarget;
                m_fDir = dirY;
            }
            if (frame < m_pAtkData->MOVE_END_FRAME) m_fDir = calcDirection(pTarget);
        }
        FVECTOR dir;
        FLOAT speed = 0.0f;
        SubVectorXYZ(vec, m_vMarkPos, m_vCenterPos);
        Normalize(dir, vec);
        vec.y = 0.0f;
        m_fDistXZ = _sqrtf(InnerProduct(vec, vec));
        m_fJumpDir = Movement::Homing(m_fJumpDir, atan2f(dir.x, dir.z), rate, m_fFrameRate);
        vec.set(0.0f, m_fJumpDir, 0.0f, 1.0f);
        setRot(vec);
        if (frame < m_pAtkData->MOVE_END_FRAME && m_pAtkData->RADIUS * 0.1f < m_fDistXZ) speed = m_pAtkData->MAX_SPEED / 60.0f;
        if ((m_pAtkData->FLAG & 0x10) && frame < m_pAtkData->MOVE_END_FRAME) speed += m_pAtkData->WORK0 / 60.0f;
        if (speed > 0.0f) m_fMoveSpdXZ = Movement::Approach(m_fMoveSpdXZ, speed, 0.2f, m_fFrameRate);
        else m_fMoveSpdXZ = Movement::Approach(m_fMoveSpdXZ, speed, 0.4f, m_fFrameRate);
        FMATRIX m;
        _unitMatrixVu0(m);
        vec.set(0.0f, 0.0f, 1.0f, 1.0f);
        sceVu0RotMatrixY(m, m, m_fJumpDir);
        ApplyMatrix33(vec, m, vec);
        ScaleVector(vec, vec, m_fMoveSpdXZ);
        vel.x = vec.x;
        vel.z = vec.z;
        m_pMove->setVel(vel);
        rot.set(0.0f, m_fDir, 0.0f, 1.0f);
        setRot(rot);
        if (isMotionEnd()) {
            m_JumpState = 3;
            setAnimForce(6, -1, 0, 0, 0, 0);
            m_pMarker->holdTarget(0);
        }
        break;
    }
    case 3:
        m_fJumpSpdY += m_fJumpGravity * m_fFrameRate;
        vel.y = m_fJumpSpdY;
        m_pMove->setVel(vel);
        break;
    case 6:
        setGroundIK(1);
        exeJumpLand();
        return;
    }
    if (isActionFlag(0x2000000)) {
        if (isTrgFlag(0x10)) chkChargeMove(0);
    }
    if (chkTrgAction(0, 0)) {
        m_pMarker->holdTarget(0);
        return;
    }
    if (!isAir()) {
        m_JumpState = 6;
        setAnimForce(7, -1, 0, 0, 0, 0);
        m_fJumpSpdXZ = 0.0f;
        clearSpeed();
        setBattleFlag(0, 8);
        m_fJumpSpdY = 0.0f;
        m_fJumpGravity = 0.0f;
        m_pMarker->holdTarget(0);
        return;
    }
    m_fJumpHeight = getPos().y - m_vJumpStartPos.y;
    if (m_fJumpTop < m_fJumpHeight) m_fJumpTop = m_fJumpHeight;
    m_tqJumpTimer += m_tqFrameRate;
}
INT CPlayerDRiku::startStockSkillAction() {
    INT id;
    switch (m_iStockSkill) {
    case 0x5f:
    case 0x60:
    case 0x61:
    case 0x62:
    case 0x65:
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
void CPlayerDRiku::initStockSkill(INT inNo) {
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
    m_nAttackCount = 0;
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
    case 0x5f:
        m_uiAttackID = 0xa6;
        m_pPLM->setReqLoad(0x5f, 0);
        type = 1;
        break;
    case 0x60:
        m_uiAttackID = 0xa8;
        m_pPLM->setReqLoad(0x60, 0);
        type = 1;
        break;
    case 0x61:
        m_uiAttackID = 0xa9;
        m_pPLM->setReqLoad(0x61, 0);
        type = 1;
        break;
    case 0x62:
    case 0x63:
        m_uiAttackID = m_iStockSkill + 0x12;
        m_pPLM->setReqLoad(0x62, 0);
        break;
    case 0x65:
        m_fWallRadius = 1.0f;
        m_uiAttackID = 0x77;
        m_pPLM->setReqLoad(0x62, 0);
        type = 1;
        m_pPLM->setReqLoad(0x65, 0);
        break;
    case 0x69:
        m_uiAttackID = 0xb5;
        m_pPLM->setReqLoad(0x69, 0);
        type = 1;
        m_pPLM->setReqLoad(0x93, 0);
        setBattleFlag(0x100000, 0);
        break;
    case 0x6a:
        m_uiAttackID = 0xb7;
        m_pPLM->setReqLoad(0x6a, 0);
        type = 1;
        m_pPLM->setReqLoad(0x94, 0);
        m_pPLM->setReqLoad(0x92, 0);
        setBattleFlag(0x100000, 0);
        break;
    case 0x6b:
        m_uiAttackID = 0xb9;
        m_pPLM->setReqLoad(0x6b, 0);
        type = 1;
        m_pPLM->setReqLoad(0x95, 0);
        m_pPLM->setReqLoad(0x92, 0);
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
/* Parked: structurally complete (35 cases in the original order, Riku's 0x6cc-0x6f9 group renumbered to 0x712-0x73f), same frame, 8 bytes long, 173 of 3903 lines off: case 1's attack-ID switch keeps a compare for the empty case 0x690 in the original; cross-jumped tails differ (0x67c's flag-and-return tail, 0x683); pTarget gets s2 instead of s0 in 0x69c; f0/f1 choices in 0x69c and 0x4a8; the original keeps 50 in s2 for the whole function; 0x4a8 has a dead (m_fAnalogViewDir - m_fDir) < -PI compare. Worked: two block-local FVECTORs in 0x4a8 (frame size), 0x69b's arrive/else arm order, 0x73c's offWeapon tail, 0x73d's setMagic(0xb9) loop with onWeapon(1). Tried: switch forms for case 1. Same family as the parked CPlayerRiku/CPlayerSora exeStockSkill drafts. */
#if 0
static inline void _setAttack(CPlayerDRiku* p, UINT inID) {
    p->m_uiAttackID = inID;
    p->m_pAtkData = &g_AttackDataList[inID];
    p->m_iAttackPower = p->m_iAP * CardBattle::g_pStockSkillList[p->m_iStockSkill].POWER[p->m_pAtkData->POWER] / 100;
    _ref(p->m_AtkKind) = (ATK_KIND)p->m_pAtkData->DAMAGE;
    _ref(p->m_AtkAttr) = (ATK_ATTR)CardBattle::g_pStockSkillList[p->m_iStockSkill].ATTR;
    UINT up = _upKind(CardBattle::g_pStockSkillList[p->m_iStockSkill].UP_TYPE0,
                      CardBattle::g_pStockSkillList[p->m_iStockSkill].UP_TYPE1);
    p->calcEnchant_AttackPoint(p->m_iAttackPower, p->m_AtkKind, p->m_AtkAttr, up);
}
void CPlayerDRiku::exeStockSkill() {
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
            m_iSubState = 0x69f;
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
            m_iSubState = 0x4ab;
            return;
        }
        if (m_pAtkData->FLAG & 0x20) {
            switch (m_uiAttackID * 10) {
            case 0x690:
                break;
            case 0x67c:
                setCastEffect(g_szDpxEX1580, 0x13, &Crd(m_iBoneCenter), NULL);
                break;
            }
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
    case 0x67c:
        if (!m_bNextTrg) {
            m_bNextTrg = TRUE;
            m_fJumpSpdY = g_fAntiGravity * m_pAtkData->WORK1;
            vel = m_pMove->getVel();
            vel.y = m_fJumpSpdY;
            m_pMove->setVel(vel);
        }
        procTargetMark();
        procAttackMove(frame, m_pAtkData->RADIUS * 0.1f, m_pAtkData->MAX_SPEED, 1, 0.15f);
        if (!isMotionEnd()) return;
        m_iSubState = 0x67d;
        m_fJumpSpdY = g_fGravity * m_pAtkData->WORK2;
        vel = m_pMove->getVel();
        vel.y = m_fJumpSpdY;
        m_pMove->setVel(vel);
        setAnimSkillEx(1, 0, 0, 0, 0);
        m_bNextTrg = FALSE;
        setPlayerFlag(0x100000, 0);
        return;
    case 0x67d:
    case 0x67e:
    case 0x67f:
    case 0x680:
    case 0x681:
    case 0x682:
        procAttackMove(frame, m_pAtkData->RADIUS * 0.1f, m_pAtkData->MAX_SPEED, 1, 0.15f);
        if (!isTrgFlag(0x40000)) {
            if (isTrgStock()) setTrgFlagRendaNG("exeStockSkill:  DARKBREAK  Entry NG...\n");
        }
        if (procAttackCollision(frame)) return;
        if (m_bIsAttackHit) {
            offAttack();
            if (++m_nAttackCount < 6 && !isTrgFlag(0x40000)) {
                m_iSubState = 0x683;
                m_bIsCanCombo = FALSE;
                m_bNextTrg = TRUE;
                vel = m_pMove->getVel();
                if (m_nAttackCount < 5) {
                    setPushon(1);
                    m_fJumpSpdY = 0.0f;
                } else {
                    setPushon(3);
                    _setAttack(this, 0xa7);
                    m_fJumpSpdY = g_fAntiGravity * m_pAtkData->WORK1;
                }
                vel.y = m_fJumpSpdY;
                m_pMove->setVel(vel);
                setAnimSkillEx(2, -1, 1, 0, 0);
                setPlayerFlag(0x100000, 0);
                return;
            }
        }
        if (isMotionEnd()) {
            m_iSubState = 0x684;
            setAnimSkillEx(3, -1, 1, 0, 0);
            clearSpeed();
        }
        break;
    case 0x683:
        if (!isTrgFlag(0x40000) && m_bNextTrg && isTrgStock()) {
            m_bIsCanCombo = TRUE;
            setPushon(0);
            m_bNextTrg = FALSE;
        }
        procTargetMark();
        procAttackMove(frame, m_pAtkData->RADIUS * 0.1f, m_pAtkData->MAX_SPEED, 1, 0.15f);
        if (!isMotionEnd()) return;
        setPushon(0);
        if (m_bIsCanCombo) {
            m_bIsCanCombo = FALSE;
            m_iSubState = m_nAttackCount + 0x67d;
            m_bNextTrg = FALSE;
            m_bIsAttackHit = FALSE;
            m_fJumpSpdY = g_fGravity * m_pAtkData->WORK2;
            vel = m_pMove->getVel();
            vel.y = m_fJumpSpdY;
            m_pMove->setVel(vel);
            setAnimSkillEx(1, 0, 0, 0, 0);
            setPlayerFlag(0x100000, 0);
            return;
        }
        clearSpeed();
        procSkillEnd(-1);
        return;
    case 0x690:
        setPlayerFlag(0x400000, 0);
        procTargetMark();
        procAttackMove(frame, 0.0f, 0.0f, 2, 0.15f);
        if (frame < m_pAtkData->TRG_FRAME) break;
        m_iSubState = 0x691;
        setMagic(0xa8, 0, m_iStockSkill);
        m_pCam->changeNowPlayer(getHandle(), m_hMagicCore);
        m_pCam->changeCamMode(4, 0);
        break;
    case 0x691:
        setPlayerFlag(0x400000, 0);
        if (m_hMagicCore > 0 && !CTaskMan::getTask(m_hMagicCore)) m_hMagicCore = 0;
        if (!isMotionEnd()) break;
        m_iSubState = 0x692;
        setAnim(0x2a, -1, 0);
        setTimer(m_pAtkData->WORK2 * 100);
        break;
    case 0x692:
        setPlayerFlag(0x400000, 0);
        if (m_hMagicCore > 0 && !CTaskMan::getTask(m_hMagicCore)) m_hMagicCore = 0;
        if (getTimer() > 0 && m_hMagicCore > 0) break;
        m_hMagicCore = 0;
        PlayerCamera::CPlayerCamera::setBattleCamData(0, NULL);
        m_pCam->changeNowPlayer(0, 0);
        m_pCam->changeCamMode(-1, 0);
        procSkillEnd(-1);
        return;
    case 0x69a:
        m_fJumpSpdY = 0.0f;
        setPlayerFlag(0x400000, 0);
        procTargetMark();
        procAttackMove(frame, 0.0f, 0.0f, 2, 0.15f);
        if (isMotionEnd()) {
            m_vAppearPos = m_vCenterPos;
            clearSpeed();
            setAnimSkillEx(1, -1, 0, 0, 0);
            setTimer(m_pAtkData->WORK2 * 100);
            m_fJumpDir = calcDirection(m_vMarkPos);
            searchWarpPos(m_fJumpDir);
            m_iSubState = 0x69b;
            m_nAttackCount = 0;
        }
        if (m_bNextTrg) return;
        m_bNextTrg = TRUE;
        m_pCam->changeNowPlayer(getHandle(), getHandle());
        m_pCam->changeCamMode(4, 0);
        return;
    case 0x69b:
        setPlayerFlag(0x400000, 0);
        rot = getRot();
        m_pMove->homingMove(m_vWarpPos[1], 1.0f, m_pAtkData->MAX_SPEED * 0.01f);
        setRot(rot);
        if (!m_pWeapon->getCollAttackID()) onAttack();
        if (arriveTarget(m_vWarpPos[1], m_pAtkData->RADIUS * 0.1f) || getTimer() <= 0) {
            offAttack();
            clearSpeed();
            setupFade(1.0f, 0.0f, m_pAtkData->MOVE_START_FRAME * 50, -1);
            updateCoord();
            coordFix();
            m_vCenterPos = *(FVECTOR*)((UBYTE*)&Crd(m_iBoneCenter) + 0xc0); /* CCoord::m_World.v[3] */
            setEffect(g_EffTbl_HitMark[21].pEffectName, 0x27, &m_vCenterPos, NULL, 0.0f);
            m_iSubState = 0x69c;
        } else {
            if (m_pMove->isWall()) setTimer(getTimer() - m_tqFrameRate);
        }
        return;
    case 0x69c:
        setPlayerFlag(0x400000, 0);
        if (isFade()) return;
        pTarget = m_hTarget > 0 ? (CBattleEnemy*)CTaskMan::getTask(m_hMark) : NULL;
        if (++m_nAttackCount >= m_pAtkData->TRG_FRAME) {
            setAnimSkillEx(4, -1, 0, 0, 0);
            searchWarpPos(m_fJumpDir);
            m_vAppearPos = m_vEnemyPos;
            setPos(m_vEnemyPos);
            pos.x = m_vStartPos.x;
            if (!(m_pPLM->isJaferBattle())) pos.y = m_vEnemyPos.y + 5.0f;
            else pos.y = 7.0f;
            pos.z = m_vStartPos.z;
            setOldPos(pos);
            m_iSubState = 0x69d;
            _setAttack(this, 0xaa);
            m_pCam->changeNowPlayer(getHandle(), getHandle());
            m_pCam->changeCamMode(4, 0);
            m_bNextTrg = FALSE;
        } else {
            if (pTarget) {
                pTarget->getLockOnPos(m_vMarkPos);
            } else {
                pTarget = m_hTarget > 0 ? (CBattleEnemy*)CTaskMan::getTask(m_hTarget) : NULL;
                if (pTarget) {
                    m_hMark = m_hTarget;
                    pTarget->getLockOnPos(m_vMarkPos);
                }
            }
            setAnimSkillEx(m_nAttackCount % 3 + 1, -1, 0, 0, 0);
            setTimer(m_pAtkData->WORK2 * 100);
            m_fJumpDir = _limitPI(m_fJumpDir + 0.31415927f);
            searchWarpPos(m_fJumpDir);
            warp(m_vWarpPos[0]);
            m_iSubState = 0x69b;
            if (m_pPLM->isJaferBattle()) {
                pos.set(m_vWarpPos[0].x, 7.0f, m_vWarpPos[0].z, 1.0f);
                setOldPos(pos);
            }
            if (pTarget) {
                m_pCam->changeNowPlayer(m_hMark, 0);
                m_pCam->changeCamMode(3, 0);
            }
        }
        setupFade(0.0f, 1.0f, m_pAtkData->MOVE_START_FRAME * 50, -1);
        updateCoord();
        coordFix();
        m_vCenterPos = *(FVECTOR*)((UBYTE*)&Crd(m_iBoneCenter) + 0xc0); /* CCoord::m_World.v[3] */
        setEffect(g_EffTbl_HitMark[21].pEffectName, 0x26, &m_vCenterPos, NULL, 0.0f);
        return;
    case 0x69d:
        setPlayerFlag(0x400000, 0);
        if (frame >= m_pAtkData->COLL_START_FRAME && !m_pWeapon->getCollAttackID()) onAttack();
        if (frame >= m_pAtkData->WORK1 && !m_bNextTrg) {
            m_bNextTrg = TRUE;
            setMagic(0xab, -1, m_iStockSkill);
        }
        if (frame < m_pAtkData->WORK0) break;
        setMagic(0xab, 0, -1);
        m_nAttackCount = 0;
        m_iSubState = 0x69e;
        setTimer(0);
        break;
    case 0x69e:
        setPlayerFlag(0x400000, 0);
        if (frame >= m_pAtkData->WORK2) offAttack();
        if (getTimer() > 0) break;
        if (++m_nAttackCount >= m_pAtkData->TRG_FRAME) {
            if (!isMotionEnd()) break;
            m_pCam->changeNowPlayer(0, 0);
            m_pCam->changeCamMode(3, 0);
            procSkillEnd(-1);
            return;
        }
        if (m_nAttackCount == 1) {
            _setAttack(this, 0xab);
        }
        setMagic(0xab, 0, m_iStockSkill);
        setTimer(m_pAtkData->WORK0 * 50);
        break;
    case 0x69f:
        if (m_iStockSkill == 0x65) {
            if (!CRcf::isCompleteLoadRCF(0, 0)) break;
            m_bNextTrg = FALSE;
            if (m_pPLM->isCompleteLoad(0x62) != 1) {
                m_pPLM->setReqLoad(0x62, 0);
                m_bNextTrg = TRUE;
            }
            if (m_pPLM->isCompleteLoad(0x65) != 1) {
                m_pPLM->setReqLoad(0x65, 0);
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
            m_iSubState = 0x4aa;
            break;
        }
        if (!m_bIsCanCombo) {
            m_bIsCanCombo = m_pPLM->getFriend()->setSkill(m_uiAttackID);
        }
        exeStandUp_inFriend();
        break;
    case 0x4a6:
        setPlayerFlag(0x400000, 0);
        if (!m_pPLM->isFriendReady()) {
            m_iSubState = 0x4aa;
            break;
        }
        if (!m_bIsCanCombo) {
            m_bIsCanCombo = m_pPLM->getFriend()->setSkill(m_uiAttackID);
            if (!m_bIsCanCombo) break;
        }
        m_iSubState = 0x4a7;
        setAnimSkillEx(0, 0, 0, 0, 0);
        setConditionFlag(0x80000, 0);
        break;
    case 0x4a7:
        setPlayerFlag(0x400000, 0);
        if (!m_pPLM->isFriendReady()) {
            m_iSubState = 0x4aa;
            break;
        }
        if (!isMotionEnd()) break;
        m_iSubState = 0x4a8;
        setAnimSkillEx(1, 0, 0, 0, 0);
        m_uiStep = 0;
        m_iCount = 0;
        setTimer(m_pAtkData->TRG_FRAME * 50);
        PlayerCamera::CPlayerCamera::setBattleCamData(getHandle(), &Friend::g_HolyBurstCamParam);
        m_pCam->changeNowPlayer(getHandle(), 0);
        m_pCam->changeCamMode(3, 0);
        break;
    case 0x4a8:
        if (!m_pPLM->isFriendReady()) {
            m_iSubState = 0x4aa;
            break;
        }
        if (isMotionEnd()) {
            m_iCount = 0;
            if (++m_uiStep >= 4) {
                m_iSubState = 0x4a9;
                setAnimSkillEx(2, -1, 1, 0, 0);
                setConditionFlag(0, 0x80000);
                m_fWallRadius = 0.5f;
                m_fJumpSpdXZ = 0.0f;
                m_fWalkSpeed = 0.0f;
                m_pMove->calcVelocityXZ(0.0f);
                clearSpeed();
                PlayerCamera::CPlayerCamera::setBattleCamData(0, NULL);
                m_pCam->changeNowPlayer(0, 0);
                m_pCam->changeCamMode(3, 0);
                break;
            }
        }
        {
            FLOAT speed = 0.0f;
            if (m_fAnalogPower >= 0.3f) {
                _limitPI(m_fAnalogViewDir - m_fDir);
                speed = m_pAtkData->WORK2 * 0.1f;
                m_fDir = Movement::Homing(m_fDir, m_fAnalogViewDir, 0.2f, m_fFrameRate);
                FVECTOR r0;
                r0._q = 0;
                r0.y = m_fDir;
                FVECTOR r1 = r0;
                setRot(r1);
            }
            m_fWalkSpeed = speed / 60.0f;
            m_fJumpSpdXZ = Movement::Approach(m_fJumpSpdXZ, m_fWalkSpeed, 0.02f, m_fFrameRate);
            m_fWalkSpeed = m_fJumpSpdXZ;
            m_pMove->calcVelocityXZ(m_fJumpSpdXZ);
        }
        if (getTimer() > 0) break;
        setMagic(0x77, 0, m_iStockSkill);
        m_iCount++;
        setTimer(m_pAtkData->TRG_FRAME * 50);
        break;
    case 0x4a9:
        if (isMotionEnd()) setAnim(0x2a, -1, 0);
        if (m_pPLM->isFriendReady()) break;
        procSkillEnd(-1);
        break;
    case 0x4ab:
        if (!CRcf::isCompleteLoadRCF(0, 0)) break;
        m_bNextTrg = FALSE;
        if (m_pPLM->isCompleteLoad(m_iStockSkill) != 1) {
            m_pPLM->setReqLoad(m_iStockSkill, 0);
            m_bNextTrg = TRUE;
        }
        switch (m_iStockSkill) {
        case 0x69:
            if (m_pPLM->isCompleteLoad(0x93) != 1) {
                m_pPLM->setReqLoad(0x93, 0);
                m_bNextTrg = TRUE;
            }
            break;
        case 0x6a:
            if (m_pPLM->isCompleteLoad(0x94) != 1) {
                m_pPLM->setReqLoad(0x94, 0);
                m_bNextTrg = TRUE;
            }
            if (m_pPLM->isCompleteLoad(0x92) != 1) {
                m_pPLM->setReqLoad(0x92, 0);
                m_bNextTrg = TRUE;
            }
            break;
        case 0x6b:
            if (m_pPLM->isCompleteLoad(0x95) != 1) {
                m_pPLM->setReqLoad(0x95, 0);
                m_bNextTrg = TRUE;
            }
            if (m_pPLM->isCompleteLoad(0x92) != 1) {
                m_pPLM->setReqLoad(0x92, 0);
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
        case 0x712:
            setAnimSkillEx(m_pAtkData->ANIM_ID, 0, 1, m_pAtkData->ANIM_ESQ, SESeq);
            break;
        case 0x726: {
            _setAttack(this, 0xac);
            setAnimRsrcEx(0x51e, 0, -1, 1, 0, 0, 0);
            break;
        }
        case 0x73a: {
            _setAttack(this, 0xad);
            setAnimRsrcEx(0x51e, 0, -1, 1, 0, 0, 0);
            break;
        }
        }
        setPlayerFlag(0x100000, 0);
        m_tqStateTimer = 0;
        m_pMarker->holdTarget(8);
        procTargetMark();
        return;
    case 0x712:
        setPlayerFlag(0x400000, 0);
        procTargetMark();
        procAttackMove(frame, 0.0f, 0.0f, 2, 0.15f);
        if (!m_bNextTrg) {
            if (frame >= m_pAtkData->TRG_FRAME) {
                m_bNextTrg = TRUE;
                setMagic(0xb5, 0, m_iStockSkill);
            }
        } else {
            pTarget = NULL;
        if (m_hMark > 0) pTarget = (CBattleEnemy*)CTaskMan::getTask(m_hMark);
            if (pTarget && pTarget->canFlick() && !pTarget->isBind()) {
                pos = pTarget->getPos();
                vel = pTarget->getVel();
                if (0.0f < vel.y && 3.5f < pos.y - m_vJumpStartPos.y) {
                    pos.y = m_vJumpStartPos.y + 3.5f;
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
            m_iSubState = 0x713;
            _setAttack(this, 0xac);
            setAnimRsrcEx(0x51e, 0, -1, 1, 0, 0, 0);
            setPlayerFlag(0x100000, 0);
            break;
        }
        m_iSubState = 0x714;
        _setAttack(this, 0xb6);
        setAnimSkillEx(m_pAtkData->ANIM_ID, 0, 1, 0, 0);
        setPlayerFlag(0x100000, 0);
        clearSpeed();
        return;
    case 0x713:
        pTarget = NULL;
        if (m_hMark > 0) pTarget = (CBattleEnemy*)CTaskMan::getTask(m_hMark);
        if (pTarget) {
            pTarget->getLockOnPos(m_vMarkPos);
            if (pTarget->canFlick() && !pTarget->isBind()) {
                vel = pTarget->getVel();
                if (vel.y < 0.0f) {
                    pos = pTarget->getOldPos();
                    pTarget->setPos(pos);
                    vel.y = 0.0f;
                    pTarget->setVel(vel);
                } else if (0.0f < vel.y) {
                    pos = pTarget->getPos();
                    if (3.5f < pos.y - m_vJumpStartPos.y) {
                        pos.y = m_vJumpStartPos.y + 3.5f;
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
                setAnimRsrcEx(0x51e, 1, -1, 1, 0, 0, 0);
                onAttack();
            }
            setTimer(6000);
        }
        if (m_fDistXZ <= 6.0f || getTimer() <= 0) {
            offAttack();
            m_iSubState = 0x714;
            _setAttack(this, 0xb6);
            setAnimSkillEx(m_pAtkData->ANIM_ID, 0, 1, 0, 0);
            setPlayerFlag(0x100000, 0);
            clearSpeed();
            return;
        }
        break;
    case 0x714:
        if (procAttackCollision(frame)) return;
        pTarget = NULL;
        if (m_hMark > 0) pTarget = (CBattleEnemy*)CTaskMan::getTask(m_hMark);
        if (pTarget) {
            pTarget->getLockOnPos(m_vMarkPos);
            if (!m_bIsAttackHit && pTarget->canFlick() && !pTarget->isBind()) {
                vel = pTarget->getVel();
                if (vel.y < 0.0f) {
                    pos = pTarget->getOldPos();
                    pTarget->setPos(pos);
                    vel.y *= 0.5f;
                    pTarget->setVel(vel);
                } else if (0.0f < vel.y) {
                    pos = pTarget->getPos();
                    if (3.5f < pos.y - m_vJumpStartPos.y) {
                        pos.y = m_vJumpStartPos.y + 3.5f;
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
            m_iSubState = 0x715;
            m_bNextTrg = TRUE;
            offAttack();
            setMagic(0xb5, 0, -1);
            setTimer(9000);
        }
        if (!m_bIsAttackHit) return;
        break;
    case 0x715:
        if (isMotionEnd()) setAnimForce(0x2a, -1, 0, 0, 0, 0);
        if (m_hMagicCore > 0 && !CTaskMan::getTask(m_hMagicCore)) m_hMagicCore = 0;
        if (getTimer() > 0 && m_hMagicCore) break;
        procSkillEnd(-1);
        return;
    case 0x726:
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
                setAnimRsrcEx(0x51e, 1, -1, 1, 0, 0, 0);
                onAttack();
            }
            setTimer(6000);
        }
        if (m_fDistXZ <= 6.0f || getTimer() <= 0) {
            offAttack();
            m_iSubState = 0x727;
            _setAttack(this, 0xb7);
            setAnimSkillEx(m_pAtkData->ANIM_ID, 0, 1, 0, 0);
            setPlayerFlag(0x100000, 0);
            clearSpeed();
            setTimer((m_pAtkData->COLL_START_FRAME + m_pAtkData->TRG_FRAME) * 50);
            m_bIsAttackHit = FALSE;
            return;
        }
        break;
    case 0x727:
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
        m_iSubState = 0x728;
        _setAttack(this, 0xb8);
        setAnimSkillEx(m_pAtkData->ANIM_ID, 0, 1, 0, 0);
        setPlayerFlag(0x100000, 0);
        clearSpeed();
        calcAppearPos(1.0f, 0.55f, NULL);
        return;
    case 0x728:
        if (procAttackCollision(frame)) return;
        if (!m_bIsAttackHit && frame < m_pAtkData->COLL_END_FRAME) {
            procAttackMove(frame, m_pAtkData->RADIUS * 0.1f, m_pAtkData->MAX_SPEED, 1, 0.15f);
        } else {
            procAttackMove(frame, 0.0f, 0.0f, 0, 0.15f);
        }
        if (frame >= m_pAtkData->TRG_FRAME && !m_bNextTrg) {
            m_bNextTrg = TRUE;
            setMagic(0xb7, 0, -1);
        }
        if (!isMotionEnd()) return;
        procSkillEnd(-1);
        return;
    case 0x73a:
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
                setAnimRsrcEx(0x51e, 1, -1, 1, 0, 0, 0);
                onAttack();
            }
            setTimer(6000);
        }
        if (m_fDistXZ <= m_pAtkData->RADIUS * 0.1f || getTimer() <= 0) {
            offAttack();
            m_iSubState = 0x73b;
            _setAttack(this, 0xb9);
            setAnimSkillEx(m_pAtkData->ANIM_ID, 0, 1, 0, 0);
            setPlayerFlag(0x100000, 0);
            clearSpeed();
            m_bIsAttackHit = FALSE;
            return;
        }
        break;
    case 0x73b:
        if (isPlayerFlag(0x100000)) procTargetMark();
        if (frame < m_pAtkData->COLL_END_FRAME) {
            procAttackMove(frame, m_pAtkData->RADIUS * 0.1f, m_pAtkData->MAX_SPEED, 1, 0.15f);
        } else {
            procAttackMove(frame, 0.0f, 0.0f, 0, 0.15f);
        }
        if (procAttackCollision(frame)) return;
        if (!isMotionEnd()) break;
        offAttack();
        m_iSubState = 0x73c;
        _setAttack(this, 0xba);
        setAnimSkillEx(m_pAtkData->ANIM_ID, 0, 0, 0, 0);
        setPlayerFlag(0x100000, 0);
        clearSpeed();
        m_bIsAttackHit = FALSE;
        procAttackMove(frame, 0.0f, 0.0f, 2, 1.0f);
        calcAppearPos(2.0f, 0.55f, NULL);
        m_vAppearPos.y = getPos().y;
        break;
    case 0x73c:
        if (procAttackCollision(frame)) return;
        if (frame >= m_pAtkData->MOVE_START_FRAME) {
            if (!m_bNextTrg) {
                _ref(m_bNextTrg) = TRUE;
                m_fJumpSpdY = g_fAntiGravity * m_pAtkData->WORK1;
                vel = m_pMove->getVel();
                vel.y = m_fJumpSpdY;
                m_pMove->setVel(vel);
            }
            procAttackMove(frame, 0.0f, 0.0f, 0, 0.15f);
        }
        if (!isMotionEnd()) break;
        offAttack();
        m_iSubState = 0x73d;
        _setAttack(this, 0xbb);
        setAnimSkillEx(m_pAtkData->ANIM_ID, 0, 1, 0, 0);
        setPlayerFlag(0x100000, 0);
        clearSpeed();
        m_bIsAttackHit = FALSE;
        procAttackMove(frame, 0.0f, 0.0f, 2, 1.0f);
        offWeapon();
        m_vWarpPos[0] = m_vCenterPos;
        setTimer(0);
        break;
    case 0x73d:
        if (frame < m_pAtkData->COLL_START_FRAME) {
            procTargetMark();
            procAttackMove(frame, 0.0f, 0.0f, 2, 0.15f);
        }
        if (m_nAttackCount < 7 && getTimer() <= 0) {
            m_nAttackCount++;
            setMagic(0xb9, m_nAttackCount, m_iStockSkill);
            setTimer(m_pAtkData->TRG_FRAME * 50);
        }
        if (!isMotionEnd()) return;
        m_iSubState = 0x73e;
        setAnimSkillEx(3, 0, 1, 0, 0);
        onWeapon(1);
        return;
    case 0x73e:
        if (isMotionEnd()) {
            procSkillEnd(1);
            return;
        }
        m_fJumpSpdY *= 0.5f;
        break;
    case 0x4aa:
        procSkillEnd(-1);
        return;
    case 3:
    case 0x684:
    case 0x73f:
        if (!isMotionEnd()) break;
        procSkillEnd(-1);
        return;
    }
    procGravity(frame, m_pAtkData->MOVE_END_FRAME, 1);
}
#else
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Game/Player/PlayerDRiku", exeStockSkill__Q22PL12CPlayerDRiku);
#endif

// exeStockSkill__Q22PL12CPlayerDRiku: its .rodata, for its INCLUDE_ASM; remove with it.
asm(".section .rodata\n"
    ".align 3\n"
    "D_004A1D78:\n"
    ".asciz \"exeStockSkill:  DARKBREAK  Entry NG...\\n\"\n"
    ".align 2\n"
    ".section .text");
void CPlayerDRiku::setMagic(INT inState, INT inCardID, INT inSkillID) {
    FVECTOR pos;
    FVECTOR rot;
    BULLET_PARAM param;
    CBullet* pBullet;
    if (!inCardID && !inSkillID) return;
    switch (inState) {
    case 0xa8:
        param.init(2, 3);
        param.uiFlag = 0x1009100;
        param.CollKind = COLL_KIND_PSHOT;
        param.AtkKind = m_AtkKind;
        param.AtkAttr = m_AtkAttr;
        param.iAttackPoint = m_iAttackPower;
        param.fSpeed = m_pAtkData->MAX_SPEED * 0.01f;
        param.fMaxDist = m_pAtkData->WORK0;
        param.tqHomingTime = 9000;
        param.hHomingTarget = m_hTarget;
        param.fHomingRadius = m_pAtkData->RADIUS;
        param.fHomingRate = m_pAtkData->WORK1;
        param.vPos = *(FVECTOR*)((UBYTE*)&Crd(m_iBoneLHand) + 0xc0); /* CCoord::m_World.v[3] */
        param.vPos.w = 0.5f;
        param.vRot = getRot();
        param.pEffectTbl = g_EffTbl_Skill[m_pAtkData->EFFECT];
        param.fCollRadius = m_pAtkData->COLL_START_FRAME * 0.01f;
        pBullet = (CBullet*)CEntityFactory::getInstance()->createBullet(param, this);
        if (pBullet) {
            m_hMagicCore = pBullet->getHandle();
            pBullet->setBodyRadius(0.1f, 0.0f, 0, COLL_KIND_NONE);
        }
        break;
    case 0xab:
        if (inSkillID == -1) {
            param.init(1, 3);
            param.AtkKind = m_AtkKind;
            param.CollKind = COLL_KIND_PSHOT;
            param.AtkAttr = m_AtkAttr;
            param.iAttackPoint = m_iAttackPower;
            param.pEffectTbl = g_EffTbl_Skill[49];
            param.uiFlag = 0x30002;
            param.CollType = COLL_TYPE_CIRCLE;
            param.tqCollAtkTime = 2000;
            param.fChangeRadius = 0.075f;
            param.fCollOffsetY = 0.4f;
            param.fSpeed = 0.0f;
            param.fCollRadius = 0.0f;
            param.vPos = getPos();
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
        } else if (inCardID == -1) {
            PATK_DATA* pAtkData = &g_AttackDataList[0xab];
            UINT up = _upKind(CardBattle::g_pStockSkillList[m_iStockSkill].UP_TYPE0,
                              CardBattle::g_pStockSkillList[m_iStockSkill].UP_TYPE1);
            param.init(1, 3);
            param.CollKind = COLL_KIND_PSHOT;
            param.AtkAttr = m_AtkAttr;
            param.iAttackPoint = m_iAP * CardBattle::g_pStockSkillList[m_iStockSkill].POWER[pAtkData->POWER] / 100;
            param.AtkKind = pAtkData->DAMAGE;
            calcEnchant_AttackPoint(param.iAttackPoint, *(ATK_KIND*)&param.AtkKind, (ATK_ATTR)param.AtkAttr, up);
            param.fSpeed = 0.0f;
            param.fCollRadius = pAtkData->RADIUS * 0.01f;
            param.fShiftY = param.fCollRadius;
            param.vPos = getPos();
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
            param.tqCollAtkTime = pAtkData->COLL_END_FRAME * 100;
            param.pEffectTbl = g_EffTbl_Skill[pAtkData->EFFECT];
            param.fScale = pAtkData->MAX_SPEED * 0.1f;
            param.uiFlag = 0x20002;
        } else {
            FLOAT r;
            FLOAT theta;
            param.init(1, 3);
            param.CollKind = COLL_KIND_PSHOT;
            param.AtkKind = m_AtkKind;
            param.AtkAttr = m_AtkAttr;
            param.iAttackPoint = m_iAttackPower;
            param.fSpeed = 0.0f;
            param.fCollRadius = m_pAtkData->RADIUS * 0.01f;
            param.fShiftY = param.fCollRadius;
            r = RND::genrand_real1() * (m_pAtkData->COLL_START_FRAME * 0.1f);
            theta = Math3D::LimitPI(RND::genrand_real1() * 360.0f * 0.017453292f);
            param.vPos = getPos();
            m_pCollMap->setType(COLL_TYPE_SPHERE);
            m_pCollMap->enableSliding(0);
            m_pCollMap->setRadius(param.fCollRadius);
            m_pCollMap->setStartPos(param.vPos);
            param.vPos.x += r * sinf(theta);
            param.vPos.z += r * cosf(theta);
            m_pCollMap->setEndPos(param.vPos);
            if (m_pCollMap->collideMap()) param.vPos = *m_pCollMap->getOutPos();
            param.vRot = getRot();
            param.pEffectTbl = g_EffTbl_Skill[m_pAtkData->EFFECT];
            param.tqCollAtkTime = m_pAtkData->COLL_END_FRAME * 100;
            param.fScale = m_pAtkData->MAX_SPEED * 0.1f;
            param.uiFlag = 0x20002;
        }
        CEntityFactory::getInstance()->createBullet(param, this);
        break;
    case 0xb5:
        if (inSkillID > 0) {
            param.init(1, 3);
            param.CollKind = COLL_KIND_PSHOT;
            param.AtkKind = m_AtkKind;
            param.AtkAttr = m_AtkAttr;
            param.iAttackPoint = m_iAttackPower;
            param.fSpeed = 0.4f;
            param.fCollRadius = 2.0f;
            param.fShiftY = 2.0f;
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
            m_hEffCast = setEffect(_ref(g_szDpxImpals), 6, &m_vJumpStartPos, NULL, 0.0f);
            param.vRot = getRot();
            param.vRot.x = -1.5707964f;
            param.pEffectTbl = g_EffTbl_Skill[54];
            param.tqCollAtkTime = 600;
            param.tqHomingTime = 600;
            param.tqFadeTime = 600;
            param.fScale = 1.0f;
            param.uiFlag = 0x20003;
            CEntityFactory::getInstance()->createBullet(param, this);
        } else {
            setQuake(10.0f, 0.25f, 50);
            _ref(m_uiAttackID) = 0xb6;
            _ref(m_pAtkData) = &g_AttackDataList[m_uiAttackID];
            _ref(m_iAttackPower) = getAP() * CardBattle::g_pStockSkillList[m_iStockSkill].POWER[3] / 100;
            _ref(m_AtkKind) = (ATK_KIND)3;
            _ref(m_AtkAttr) = (ATK_ATTR)CardBattle::g_pStockSkillList[m_iStockSkill].ATTR;
            UINT up = _upKind(CardBattle::g_pStockSkillList[m_iStockSkill].UP_TYPE0,
                              CardBattle::g_pStockSkillList[m_iStockSkill].UP_TYPE1);
            calcEnchant_AttackPoint(m_iAttackPower, m_AtkKind, m_AtkAttr, up);
            param.init(1, 3);
            param.CollKind = COLL_KIND_PSHOT;
            param.AtkKind = m_AtkKind;
            param.AtkAttr = m_AtkAttr;
            param.iAttackPoint = m_iAttackPower;
            param.fSpeed = 0.0f;
            param.pEffectTbl = g_EffTbl_Skill[55];
            param.uiFlag = 0x30002;
            param.CollType = COLL_TYPE_CIRCLE;
            param.tqCollAtkTime = 3000;
            param.fChangeRadius = 0.15f;
            param.fCollOffsetY = 0.4f;
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
    case 0xb9:
        if (inCardID > 0) {
            param.init(1, 3);
            param.CollKind = COLL_KIND_PSHOT;
            param.AtkKind = m_AtkKind;
            param.AtkAttr = m_AtkAttr;
            param.iAttackPoint = m_iAttackPower;
            param.fSpeed = m_pAtkData->MAX_SPEED * 0.01f;
            param.fCollRadius = m_pAtkData->RADIUS * 0.01f;
            param.fShiftY = 0.0f;
            param.fMaxDist = 10.0f;
            param.tqHomingTime = 3000;
            param.pEffectTbl = g_EffTbl_Skill[60];
            param.pOffsetTbl = &g_OffsetTbl_StockSkill[4];
            param.pRotTbl = &g_RotTbl_Magic[4];
            param.fScale = 1.0f;
            param.iExtraEffectNum = 1;
            param.pExtraEffectTbl = g_EffTbl_Skill[61];
            param.tqFadeTime = 300;
            param.uiFlag = 0x19000144;
            FVECTOR ofs = {{0.0f, 0.0f, 1.2f, 1.0f}};
            FMATRIX m;
            FLOAT r = _limitPI(inCardID * -6.2831855f / 7.0f);
            _unitMatrix(m);
            _rotMatrixY(m, r);
            _rotMatrixX(m, -0.7853982f);
            ApplyMatrixXYZ(ofs, m, ofs);
            _unitMatrix(m);
            _rotMatrixY(m, m_fDir);
            ApplyMatrixXYZ(ofs, m, ofs);
            AddVectorXYZ(param.vPos, m_vWarpPos[0], ofs);
            calcDirection(param.vRot, m_vAppearPos);
            CEntityFactory::getInstance()->createBullet(param, this);
        }
        break;
    case 0xb7:
        param.init(1, 3);
        param.CollKind = COLL_KIND_PSHOT;
        param.AtkKind = m_AtkKind;
        param.AtkAttr = m_AtkAttr;
        param.iAttackPoint = m_iAttackPower;
        param.fSpeed = 0.0f;
        param.fCollRadius = 3.0f;
        param.fShiftY = 3.0f;
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
        param.pEffectTbl = g_EffTbl_Skill[62];
        param.tqCollAtkTime = 1000;
        param.fScale = 1.5f;
        param.uiFlag = 0x20002;
        pBullet = (CBullet*)CEntityFactory::getInstance()->createBullet(param, this);
        if (pBullet) pBullet->setAttackID(m_pWeapon->getCollAttackID());
        setEffect(g_szDpxMaelstrom, 7, &param.vPos, NULL, 0.0f);
        break;
    case 0x77:
        getWeaponTopPos(pos);
        param.init(2, 3);
        param.CollKind = COLL_KIND_PSHOT;
        param.uiFlag = 0x21008140;
        param.AtkKind = m_AtkKind;
        param.AtkAttr = m_AtkAttr;
        param.iAttackPoint = m_iAttackPower;
        param.fSpeed = m_pAtkData->MAX_SPEED * 0.01f;
        param.fMaxDist = m_pAtkData->WORK0;
        param.tqHomingTime = 9000;
        param.fHomingRadius = m_pAtkData->RADIUS;
        param.fHomingRate = m_pAtkData->WORK1;
        param.tqFadeTime = 1000;
        param.vPos = pos;
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
void CPlayerDRiku::searchWarpPos(FLOAT inDir) {
    static FVECTOR g_vDir[2] = {
        { { 0.0f, 0.0f, -15.0f, 1.0f } },
        { { 0.0f, 0.0f, 15.0f, 1.0f } },
    };
    FVECTOR pos0;
    FVECTOR pos;
    FVECTOR vel;
    FLOAT shiftY;
    FVECTOR pos1;
    FMATRIX m;
    FVECTOR dir;
    CEntity* pTarget = m_hMark > 0 ? (CEntity*)CTaskMan::getTask(m_hMark) : NULL;
    if (pTarget) {
        pTarget->getLockOnPos(pos);
        vel = pTarget->getVel();
        pos0 = *(FVECTOR*)((UBYTE*)&pTarget->Crd(0) + 0xc0); /* CCoord::m_World.v[3] */
        shiftY = pos.y - pos0.y;
        if (shiftY > 0.5f) shiftY = 0.5f;
    } else {
        pos = m_vCenterPos;
        vel.init();
        shiftY = pos.y - getPos().y;
    }
    pos0.x = 0.0f;
    pos0.y = pos.y - shiftY;
    pos0.z = 0.0f;
    pos0.w = 1.0f;
    ScaleVectorXYZ(vel, vel, 10.0f);
    vel.y = 0.0f;
    AddVectorXYZ(pos1, pos, vel);
    m_pCollMap->setType((COLL_TYPE)2);
    m_pCollMap->enableSliding(0);
    m_pCollMap->setRadius(0.4f);
    m_pCollMap->setStartPos(pos0);
    m_pCollMap->setEndPos(pos1);
    INT ret;
    if (m_pPLM->isHookBattle()) {
        ret = m_pCollMap->collideRide(0);
    } else {
        ret = m_pCollMap->collideMap();
    }
    if (ret) pos1 = *m_pCollMap->getDetectPos();
    pos = pos1;
    pos.y -= shiftY;
    if (pos.y < 0.1f) pos.y = 0.1f;
    m_vEnemyPos = pos;
    _unitMatrixVu0(m);
    sceVu0RotMatrixY(m, m, inDir);
    for (INT i = 0; i < 2; i++) {
        ApplyMatrix33(dir, m, g_vDir[i]);
        AddVectorXYZ(dir, pos, dir);
        m_pCollMap->setStartPos(pos);
        m_pCollMap->setEndPos(dir);
        if (m_pPLM->isHookBattle()) {
            if (m_pCollMap->collideRide(0)) {
                m_vWarpPos[i] = *m_pCollMap->getDetectPos();
                m_vWarpNormal[i] = *m_pCollMap->getDetectNormal();
            }
        } else if (m_pCollMap->collideMap()) {
            m_vWarpPos[i] = *m_pCollMap->getDetectPos();
            m_vWarpNormal[i] = *m_pCollMap->getDetectNormal();
        } else {
            m_vWarpPos[i] = dir;
            dir.set(0.0f, 0.0f, -1.0f, 1.0f);
            ApplyMatrix33(dir, m, dir);
            m_vWarpNormal[i] = dir;
        }
        m_vWarpPos[i].w = 1.0f;
        m_vWarpNormal[i].w = 1.0f;
    }
    m_pCollMap->enableSliding(1);
}
void CPlayerDRiku::initFriend(INT inNo) {
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

void CPlayerDRiku::exeFriend() {
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

} // namespace PL
// SOURCE DATA: .data .rodata
