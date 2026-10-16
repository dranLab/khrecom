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
void AddVectorXYZ(FVECTOR& v0, const FVECTOR& v1, const FVECTOR& v2);
void ApplyMatrix33(FVECTOR& v0, const FMATRIX& m0, const FVECTOR& v1);
extern "C" void sceVu0RotMatrixY(FMATRIX& m0, FMATRIX& m1, float ry);

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

class CGameWork {
public:
    static BOOL getFlag(UINT nLv, UINT nIdx);
};

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
    m_pCB->setFlag(0, 0x400000);
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
        m_pCB->setFlag(0, 0x400000);
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
        m_pMarker->setFlag(0);
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
        m_pMarker->setFlag(TRUE);
        m_pCB->setFlag(0, 0xc0000000);
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
        m_pMarker->setFlag(FALSE);
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
            setAnimSpeed(getEndAlpha());
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
    m_pCB->setFlag(0, 0x400000);
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
        m_pMove->getVel() = vel;
    } else {
        if (m_fJumpSpdY < 0.0f) m_fJumpSpdY = 0.0f;
        vel.y = m_fJumpSpdY;
        m_pMove->getVel() = vel;
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
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Game/Player/PlayerDRiku", setAttack__Q22PL12CPlayerDRiku);
// setAttack__Q22PL12CPlayerDRiku: its .rodata, for its INCLUDE_ASM; remove with it.
asm(".section .rodata\n"
    ".align 4\n"
    "jtbl_004A1CE0:\n"
    ".word .L0027E098\n"
    ".word .L0027E098\n"
    ".word .L0027E0C8\n"
    ".word .L0027E0EC\n"
    ".word .L0027E0C8\n"
    ".word .L0027E098\n"
    ".word .L0027E0E4\n"
    ".word 0\n"
    ".section .text");
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Game/Player/PlayerDRiku", initJump2nd__Q22PL12CPlayerDRiku);

/* Best attempt: with a target, the m_vMarkPos/m_hMark copies get swapped registers and rot.y is stored before m_fDir/m_fJumpDir instead of in the branch delay slot (11 lines differ). */
#if 0
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
    m_pAtkData = &g_AttackDataList[0xa5];
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
        m_fDir = calcDirection(m_vMarkPos);
        m_fJumpDir = m_fDir;
        rot.y = m_fDir;
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
#endif
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Game/Player/PlayerDRiku", exeJump2nd__Q22PL12CPlayerDRiku);
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
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Game/Player/PlayerDRiku", exeStockSkill__Q22PL12CPlayerDRiku);
// exeStockSkill__Q22PL12CPlayerDRiku: its .rodata, for its INCLUDE_ASM; remove with it.
asm(".section .rodata\n"
    ".align 3\n"
    "D_004A1D78:\n"
    ".asciz \"exeStockSkill:  DARKBREAK  Entry NG...\\n\"\n"
    ".align 2\n"
    ".section .text");
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Game/Player/PlayerDRiku", setMagic__Q22PL12CPlayerDRikuiii);
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
