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
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Game/Player/PlayerDRiku", initTransform__Q22PL12CPlayerDRikui);
// initTransform__Q22PL12CPlayerDRikui: its .rodata, for its INCLUDE_ASM; remove with it.
asm(".section .rodata\n"
    ".align 3\n"
    "D_004A1BF8:\n"
    ".asciz \"CPlayerDRiku:: initTransform(%d)\\n\"\n"
    ".align 2\n"
    ".asciz \"\"\n"
    ".align 2\n"
    ".section .text");
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Game/Player/PlayerDRiku", exeTransform__Q22PL12CPlayerDRiku);
// exeTransform__Q22PL12CPlayerDRiku: its .rodata, for its INCLUDE_ASM; remove with it.
asm(".section .rodata\n"
    ".align 3\n"
    "D_004A1C20:\n"
    ".word 0x616C5043\n"
    ".word 0x44726579\n"
    ".word 0x756B6952\n"
    ".word 0x78653A3A\n"
    ".word 0x61725465\n"
    ".word 0x6F66736E\n"
    ".word 0x52206D72\n"
    ".word 0x52555445\n"
    ".word 0xA681204E\n"
    ".word 0x6C657544\n"
    ".word 0x2E2E2E20\n"
    ".word 0x6E614320\n"
    ".word 0x216C6563\n"
    ".word 0x0000000A\n"
    ".section .text");
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Game/Player/PlayerDRiku", transferControl__Q22PL12CPlayerDRiku);
// transferControl__Q22PL12CPlayerDRiku: its .rodata, for its INCLUDE_ASM; remove with it.
asm(".section .rodata\n"
    ".align 3\n"
    "D_004A1C58:\n"
    ".asciz \"CPlayerDRiku::transferControl (Riku->DRiku) uiBattleFlag = %x,\\n\"\n"
    ".align 2\n"
    ".align 3\n"
    "D_004A1C98:\n"
    ".asciz \"CPlayerDRiku::transferControl uiBattleFlag [CARD_DUEL] clear !!\\n\"\n"
    ".align 2\n"
    ".asciz \"\"\n"
    ".align 2\n"
    ".section .text");
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Game/Player/PlayerDRiku", exeAvoid__Q22PL12CPlayerDRiku);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Game/Player/PlayerDRiku", exeSpeedStep__Q22PL12CPlayerDRiku);
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
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Game/Player/PlayerDRiku", exeJump2nd__Q22PL12CPlayerDRiku);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Game/Player/PlayerDRiku", startStockSkillAction__Q22PL12CPlayerDRiku);
// startStockSkillAction__Q22PL12CPlayerDRiku: its .rodata, for its INCLUDE_ASM; remove with it.
asm(".section .rodata\n"
    ".align 4\n"
    "jtbl_004A1D00:\n"
    ".word .L0027EF24\n"
    ".word .L0027EF24\n"
    ".word .L0027EF24\n"
    ".word .L0027EF24\n"
    ".word .L0027EF2C\n"
    ".word .L0027EF64\n"
    ".word .L0027EF24\n"
    ".word .L0027EF3C\n"
    ".word .L0027EF3C\n"
    ".word .L0027EF3C\n"
    ".word .L0027EF3C\n"
    ".word .L0027EF3C\n"
    ".word .L0027EF3C\n"
    ".word 0\n"
    ".word 0\n"
    ".word 0\n"
    ".section .text");
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Game/Player/PlayerDRiku", initStockSkill__Q22PL12CPlayerDRikui);
// initStockSkill__Q22PL12CPlayerDRikui: its .rodata, for its INCLUDE_ASM; remove with it.
asm(".section .rodata\n"
    ".align 4\n"
    "jtbl_004A1D40:\n"
    ".word .L0027F078\n"
    ".word .L0027F08C\n"
    ".word .L0027F0A0\n"
    ".word .L0027F0C4\n"
    ".word .L0027F0C4\n"
    ".word .L0027F1C8\n"
    ".word .L0027F0E8\n"
    ".word .L0027F1C8\n"
    ".word .L0027F1C8\n"
    ".word .L0027F1C8\n"
    ".word .L0027F128\n"
    ".word .L0027F150\n"
    ".word .L0027F178\n"
    ".word 0\n"
    ".section .text");
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Game/Player/PlayerDRiku", exeStockSkill__Q22PL12CPlayerDRiku);
// exeStockSkill__Q22PL12CPlayerDRiku: its .rodata, for its INCLUDE_ASM; remove with it.
asm(".section .rodata\n"
    ".align 3\n"
    "D_004A1D78:\n"
    ".asciz \"exeStockSkill:  DARKBREAK  Entry NG...\\n\"\n"
    ".align 2\n"
    ".section .text");
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Game/Player/PlayerDRiku", setMagic__Q22PL12CPlayerDRikuiii);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Game/Player/PlayerDRiku", searchWarpPos__Q22PL12CPlayerDRikuf);
// searchWarpPos__Q22PL12CPlayerDRikuf: its .data, for its INCLUDE_ASM; remove with it.
asm(".section .data\n.align 4\ng_vDir.4590:\n"
    ".word 0, 0, 0xC1700000, 0x3F800000, 0, 0, 0x41700000, 0x3F800000\n.previous");
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Game/Player/PlayerDRiku", initFriend__Q22PL12CPlayerDRikui);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Game/Player/PlayerDRiku", exeFriend__Q22PL12CPlayerDRiku);

} // namespace PL
// SOURCE DATA: .data .rodata
