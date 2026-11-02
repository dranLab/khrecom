
#define GPRIM_H_PATH "System/Graphic/GPrim.h" /* MATCH: this unit names Gprim.h so (header default "System/Graphic/Gprim.h") */
#include "System/Graphic/Anim.h"
#include "System/Graphic/Gprim.h"
#include "System/Graphic/2d/2DMan.h"
#include "Game/CardBattle.h"
#include "System/Graphic/Str.h"
#include "Game/Collision/CollUtil.h"

/* MATCH: Placeholder: an empty string follows CollUtil.h's strings in .rodata (as in PlayerDRiku). */
inline void _emptyStringPlaceholder() {
    TRACE("");
}

#include "System/Graphic/Effect/effect.h"

/* System/TaskMan.h's container overflow message and two prize messages (from an unidentified header) come here. */
#include "System/TaskMan.h"


inline void _premiumPrizeMessage() {
    TRACE("\x83v\x83\x8c\x83~\x83" "A\x83\x80\x83v\x83\x89\x83" "C\x83Y\x93\xfc\x8e\xe8\x81I\x81I\n");
}
inline void _roulettePrizeMessage() {
    TRACE("\x83\x8b\x81[\x83\x8c\x83" "b\x83g\x83v\x83\x89\x83" "C\x83Y\x93\xfc\x8e\xe8\x81I\x81I\n");
}

#include "Game/Enemy/BattleEnemy.h"
#include "Game/Player/PlayerSora.h"
#include "Game/Player/PlayerManager.h"
#include "Game/Player/Friend.h"
#include "Game/Player/PlayerAttack.h"
#include "Game/Player/PlayerWeapon.h"
#include "Game/Player/Summon.h"
#include "System/Com.h"
#include "System/TaskMan.h"
#include "Game/Player/PlayerCamera.h"
#include "Game/Player/PlayerGauge.h"
#include "Game/Movement.h"
#include "System/Graphic/Effect/EffectMan.h"
#include "Game/EntityFactory.h"
#include "Game/LockOnManager.h"
#include "Game/Enemy/BattleEnemyManager.h"
#include "Game/Game.h"
#include "Game/EffectMotion.h"
#include "Game/SEMotion.h"
#include "Game/Collision/CollMap.h"
#include "System/Graphic/Camera.h"
#include "System/Rcf.h"
#include "include_asm.h"

namespace PL {
extern TIMEQ g_CardBreakTime[];
extern FVECTOR g_vBGFadeColor[7];
extern const BULLET_EFFECT_INFO g_EffTbl_Skill[67][4]; /* Game/Player/PlayerAttack.h (.rodata) */
extern const BULLET_EFFECT_INFO g_EffTbl_HitMark[27]; /* Game/Player/PlayerAttack.h (.rodata) */
extern FVECTOR g_RotTbl_Magic[8]; /* Game/Player/PlayerAttack.h */
extern FVECTOR g_OffsetTbl_StockSkill[8]; /* Game/Player/PlayerAttack.h */
extern const BULLET_EFFECT_INFO g_EffTbl_MagicCast[2]; /* Game/Player/PlayerAttack.h (.rodata) */
static const FVECTOR g_vPronounceCountOfs = {{0.0f, 1.5f, 0.0f, 1.0f}};
static LPCSTR g_szDpxFriend = "friend";
static LPCSTR g_szDpxSummon = "summon";
static LPCSTR g_szDpxLimit1 = "limit1";
static LPCSTR g_szDpxLimit2 = "limit2";
static LPCSTR g_szDpxLimit3 = "limit3";
static LPCSTR g_szDpxStart = "start";
static LPCSTR g_szDpxMarluxia3 = "marluxia3";
static LPCSTR g_szDpxCloud = "cloud_ed_lt";
static LPCSTR g_szDpxBind = "bind";

PL_PARAM g_SoraParam = {
    0x21279, 0x259, 0.25f, 1.0f, 1.3f, 6.2f, 1.8f, 28.0f, 32.5f, 100, 600, 1400,
};
PlayerCamera::BCAM_PARAM g_FiragaBurstCamParam = {
    { 55.0f, 0.05f, 0.2f, 0.6f, { { 0.0f, 0.9f, 4.0f, 0.0f } }, { { 0.0f, 0.1f, 0.0f, 0.0f } } },
    { 55.0f, 0.05f, 0.5f, 8.5f, { { 0.0f, 2.5f, 4.0f, 0.0f } }, { { 0.0f, 0.0f, 0.0f, 0.0f } } },
};
PlayerCamera::BCAM_PARAM g_TornadoCamParam = {
    { 55.0f, 0.05f, 0.2f, 0.6f, { { 0.0f, 1.4f, 4.0f, 0.0f } }, { { 0.0f, 0.6f, 0.0f, 0.0f } } },
    { 55.0f, 0.05f, 0.2f, 5.0f, { { 0.0f, 1.5f, 4.0f, 0.0f } }, { { 0.0f, 0.5f, 0.0f, 0.0f } } },
};
PlayerCamera::BCAM_PARAM g_PronounceCamParam = {
    { 60.0f, 0.05f, 0.2f, 0.0f, { { 0.0f, 0.0f, 4.0f, 0.0f } }, { { 0.0f, 0.7f, 0.0f, 0.0f } } },
    { 60.0f, 0.1f, 0.2f, 5.0f, { { 0.0f, -0.1f, 3.0f, 0.0f } }, { { 0.15f, -0.1f, 0.0f, 0.0f } } },
};
} // namespace PL

using namespace PL;

// reads/writes members and globals as through a reference (the original reloads/stores them in this order)
template <class T> static inline T& _ref(T& v) { return v; }

static inline UINT _upKind(INT inType0, INT inType1) {
    UINT up = inType0 > 0 ? 1 << (inType0 - 1) : 0;
    if (inType1 > 0) up |= 1 << (inType1 - 1);
    return up;
}

void SubVectorXYZ(FVECTOR& v0, const FVECTOR& v1, const FVECTOR& v2);
void Normalize(FVECTOR& outV, const FVECTOR& inV);
extern "C" float atan2f(float y, float x);
namespace Math3D { FLOAT VectorLength(const FVECTOR& inVector); }
namespace MG_SKILL {
HANDLE createMGSkill(INT inType, INT inLevel);
BOOL isMGSkillAlive();
}
FLOAT InnerProduct(const FVECTOR& inV0, const FVECTOR& inV1);
void ApplyMatrix(FVECTOR& v0, const FMATRIX& m0, const FVECTOR& v1);
void ApplyMatrixXYZ(FVECTOR& v0, const FMATRIX& m0, const FVECTOR& v1);
void ScaleVectorXYZ(FVECTOR& outV, const FVECTOR& inV, FLOAT inScale);
void AddVectorXYZ(FVECTOR& v0, const FVECTOR& v1, const FVECTOR& v2);
extern "C" {
void sceVu0UnitMatrix(FMATRIX& m);
void sceVu0RotMatrixY(FMATRIX& m0, FMATRIX& m1, float ry);
}
/* FMATRIX inlines of Coord.h */
static inline void _unitMatrix(FMATRIX& m) {
    sceVu0UnitMatrix(m);
}
static inline void _rotMatrixY(FMATRIX& m, FLOAT r) {
    sceVu0RotMatrixY(m, m, r);
}
extern "C" void* memset(void* s, int c, unsigned int n);
/* the inline unit matrix of vu0.h */
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

/* the angle wrap of Com.h, with a separate result variable (the original's mov.s copy) */
static inline FLOAT _limitPI2(FLOAT d) {
    FLOAT r;
    if (d < -3.1415927f) r = d + 6.2831855f;
    else {
        r = d;
        if (r > 3.1415927f) r -= 6.2831855f;
    }
    return r;
}
/* the angle difference of Com.h (to - from): in place */
static inline FLOAT _subAngle(FLOAT from, FLOAT to) {
    FLOAT r = to - from;
    if (r < -3.1415927f) r += 6.2831855f;
    else if (r > 3.1415927f) r -= 6.2831855f;
    return r;
}
/* the angle wrap of Com.h */
static inline FLOAT _limitPI(FLOAT r) {
    if (r < -3.1415927f) r += 6.2831855f;
    else if (r > 3.1415927f) r -= 6.2831855f;
    return r;
}
/* sqrt.s (Math3D.h) */
static inline FLOAT _sqrtf(FLOAT x) {
    FLOAT ret;
    asm volatile("sqrt.s %0, %1" : "=f"(ret) : "f"(x));
    return ret;
}


CPlayerSora::CPlayerSora(CTask* inParent) : CPlayer(inParent, PLM::g_uiPlayerID, 0) {
    g_Param = g_SoraParam;
    m_hMagicCore = 0;
    initMagicBullet();
    m_nHitCount = 0;
}
CPlayerSora::~CPlayerSora() {}
INT CPlayerSora::startStockSkillAction() {
    switch (m_iStockSkill) {
    case 4: case 5: case 6: case 7: case 8: case 9: case 10: case 11: case 12: case 13: case 14: case 15:
    case 16: case 17: case 18: case 19: case 20: case 21: case 22: case 23: case 24: case 25: case 26:
    case 27: case 28: case 29: case 30: case 31: case 32: case 33: case 34: case 35: case 36: case 37:
    case 38: case 39: case 40: case 41: case 42: case 43: case 44: case 45: case 46: case 47: case 48:
    case 49: case 50: case 51: case 52: case 54: case 56: case 58: case 59: case 61: case 63: case 65:
    case 66: case 67: case 68: case 70: case 71: case 73: case 74: case 75: case 77: case 79: case 81:
    case 83: case 85:
        PLM::addStockBR(m_iStockSkill);
        initStockSkill(0);
        m_iStepStockSkill = 10;
        return TRUE;
    case 53: case 55: case 57: case 60: case 62: case 64: case 69: case 72: case 76: case 78: case 80:
    case 82: case 84: case 86:
        PLM::addStockBR(m_iStockSkill - 1);
        initStockSkill(0);
        m_iStepStockSkill = 10;
        return TRUE;
    case 87: case 88: case 89:
        initStockSkill(0);
        m_iStepStockSkill = 10;
        return TRUE;
    }
    return FALSE;
}
void CPlayerSora::initStockSkill(INT inNo) {
    INT type;
    setPlayerState(SKILL);
    setBattleFlag(0, 8);
    setBattleFlag(0, 0x80000);
    setPlayerFlag(0, 0x20000);
    setPlayerFlag(0, 0x8000000);
    m_iAttackNo = inNo;
    if (!inNo) m_uiPreAttackKind = 0;
    m_bIsAttackHit = FALSE;
    m_bNextTrg = FALSE;
    m_iNextAttack = 0;
    m_bIsCanCombo = FALSE;
    m_bDmgMuteki = FALSE;
    m_tqCardBreakTime = g_CardBreakTime[7];
    m_uiStep = 0;
    m_szEffectName = NULL;
    clearSpeed();
    _ref(m_uiAttackKind) = 8;
    m_fJumpGravity = g_fGravity;
    m_JumpState = 3;
    m_fMoveSpdXZ = 0.0f;
    m_fWalkSpeed = 0.0f;
    m_iAerialAnimShift = 0;
    setPlayerFlag(0, 0x2800000);
    procTargetMark();
    type = 0;
    switch (m_iStockSkill) {
    case 4:
        m_uiAttackID = 0x8;
        type = 1;
        break;
    case 5:
        m_uiAttackID = 0x9;
        m_pPLM->setReqLoad(0x5, 0);
        type = 1;
        break;
    case 6:
        m_uiAttackID = 0xb;
        type = 1;
        break;
    case 7:
        m_uiAttackID = 0xc;
        type = 1;
        break;
    case 8:
        m_uiAttackID = 0xd;
        type = 1;
        break;
    case 9:
        m_uiAttackID = 0xe;
        m_pPLM->setReqLoad(0x9, 0);
        type = 1;
        break;
    case 10:
        m_uiAttackID = 0x11;
        m_pPLM->setReqLoad(0xa, 0);
        type = 1;
        break;
    case 11:
        m_uiAttackID = 0x1f;
        m_pPLM->setReqLoad(0xb, 0);
        type = 1;
        break;
    case 12:
        m_uiAttackID = 0x21;
        m_pPLM->setReqLoad(0xc, 0);
        type = 1;
        break;
    case 13:
        if (isAir()) {
            setPlayerFlag(0x800000, 0);
            m_iAerialAnimShift = 4;
        }
        m_uiAttackID = 0x29;
        type = 1;
        break;
    case 20:
        if (isAir()) {
            setPlayerFlag(0x800000, 0);
            m_iAerialAnimShift = 4;
        }
        m_uiAttackID = 0x2a;
        type = 1;
        break;
    case 14:
        if (isAir()) {
            setPlayerFlag(0x800000, 0);
            m_iAerialAnimShift = 4;
        }
        m_uiAttackID = 0x2b;
        type = 1;
        break;
    case 21:
        if (isAir()) {
            setPlayerFlag(0x800000, 0);
            m_iAerialAnimShift = 4;
        }
        m_uiAttackID = 0x2c;
        type = 1;
        break;
    case 36:
        if (isAir()) {
            setPlayerFlag(0x800000, 0);
            m_iAerialAnimShift = 4;
        }
        m_uiAttackID = 0x2d;
        type = 1;
        break;
    case 15:
        if (isAir()) {
            setPlayerFlag(0x800000, 0);
            m_iAerialAnimShift = 4;
        }
        m_uiAttackID = 0x2e;
        type = 1;
        break;
    case 22:
        if (isAir()) {
            setPlayerFlag(0x800000, 0);
            m_iAerialAnimShift = 4;
        }
        m_uiAttackID = 0x2f;
        type = 1;
        break;
    case 27:
        m_uiAttackID = 0x30;
        type = 1;
        break;
    case 28:
        m_uiAttackID = 0x31;
        type = 1;
        break;
    case 29:
        m_uiAttackID = 0x32;
        type = 1;
        break;
    case 30:
        m_uiAttackID = 0x34;
        type = 1;
        break;
    case 31:
        m_uiAttackID = 0x33;
        type = 1;
        break;
    case 16:
        if (isAir()) {
            setPlayerFlag(0x800000, 0);
            m_iAerialAnimShift = 4;
        }
        m_uiAttackID = 0x36;
        type = 2;
        break;
    case 23:
        if (isAir()) {
            setPlayerFlag(0x800000, 0);
            m_iAerialAnimShift = 4;
        }
        m_uiAttackID = 0x37;
        type = 2;
        break;
    case 17:
        if (isAir()) {
            setPlayerFlag(0x800000, 0);
            m_iAerialAnimShift = 4;
        }
        m_uiAttackID = 0x38;
        type = 3;
        break;
    case 24:
        if (isAir()) {
            setPlayerFlag(0x800000, 0);
            m_iAerialAnimShift = 4;
        }
        m_uiAttackID = 0x39;
        type = 3;
        break;
    case 18:
        if (isAir()) {
            setPlayerFlag(0x800000, 0);
            m_iAerialAnimShift = 4;
        }
        m_uiAttackID = 0x3a;
        type = 3;
        break;
    case 25:
        if (isAir()) {
            setPlayerFlag(0x800000, 0);
            m_iAerialAnimShift = 4;
        }
        m_uiAttackID = 0x3b;
        type = 3;
        break;
    case 19:
        if (isAir()) {
            setPlayerFlag(0x800000, 0);
            m_iAerialAnimShift = 4;
        }
        m_uiAttackID = 0x3c;
        type = 1;
        break;
    case 26:
        if (isAir()) {
            setPlayerFlag(0x800000, 0);
            m_iAerialAnimShift = 4;
        }
        m_uiAttackID = 0x3d;
        type = 1;
        break;
    case 43:
        if (isAir()) {
            setPlayerFlag(0x800000, 0);
            m_iAerialAnimShift = 4;
        }
        m_uiAttackID = 0x3e;
        type = 0;
        break;
    case 44:
        if (isAir()) {
            setPlayerFlag(0x800000, 0);
            m_iAerialAnimShift = 4;
        }
        m_uiAttackID = 0x3f;
        type = 0;
        break;
    case 45:
        if (isAir()) {
            setPlayerFlag(0x800000, 0);
            m_iAerialAnimShift = 4;
        }
        m_uiAttackID = 0x40;
        m_pPLM->setReqLoad(0x2d, 0);
        type = 1;
        break;
    case 46:
        if (isAir()) {
            setPlayerFlag(0x800000, 0);
            m_iAerialAnimShift = 4;
        }
        m_uiAttackID = 0x41;
        m_pPLM->setReqLoad(0x2e, 0);
        type = 1;
        break;
    case 47:
        if (isAir()) {
            setPlayerFlag(0x800000, 0);
            m_iAerialAnimShift = 4;
        }
        m_uiAttackID = 0x42;
        m_pPLM->setReqLoad(0x2f, 0);
        type = 1;
        break;
    case 48:
        m_uiAttackID = 0x43;
        m_pPLM->setReqLoad(0x30, 0);
        type = 0;
        break;
    case 50:
        m_fWallRadius = 0.6f;
        if (isAir()) {
            setPlayerFlag(0x800000, 0);
            m_iAerialAnimShift = -36;
        }
        m_uiAttackID = 0x44;
        type = 3;
        break;
    case 32:
        m_uiAttackID = 0x45;
        m_pPLM->setReqLoad(0x20, 0);
        type = 1;
        break;
    case 33:
        m_uiAttackID = 0x47;
        m_pPLM->setReqLoad(0x21, 0);
        type = 1;
        break;
    case 34:
        m_uiAttackID = 0x48;
        m_pPLM->setReqLoad(0x22, 0);
        type = 1;
        break;
    case 35:
        m_uiAttackID = 0x49;
        m_pPLM->setReqLoad(0x23, 0);
        type = 1;
        break;
    case 37:
        m_uiAttackID = 0x4a;
        m_pPLM->setReqLoad(0x25, 0);
        type = 1;
        break;
    case 38:
        m_uiAttackID = 0x4b;
        m_pPLM->setReqLoad(0x26, 0);
        type = 1;
        break;
    case 39:
        m_uiAttackID = 0x4d;
        m_pPLM->setReqLoad(0x27, 0);
        type = 3;
        break;
    case 40:
        m_uiAttackID = 0x50;
        m_pPLM->setReqLoad(0x28, 0);
        type = 3;
        break;
    case 41:
        m_uiAttackID = 0x51;
        m_pPLM->setReqLoad(0x29, 0);
        type = 1;
        break;
    case 42:
        m_uiAttackID = 0x52;
        m_pPLM->setReqLoad(0x2a, 0);
        type = 1;
        break;
    case 49:
        m_uiAttackID = 0x53;
        m_pPLM->setReqLoad(0x31, 0);
        type = 2;
        break;
    case 51:
        m_uiAttackID = 0x54;
        m_pPLM->setReqLoad(0x33, 0);
        type = 1;
        break;
    case 68:
    case 69:
    case 70:
        m_uiAttackID = m_iStockSkill + 0x1a;
        m_pPLM->setReqLoad(0x44, 0);
        break;
    case 71:
    case 72:
    case 73:
        m_uiAttackID = m_iStockSkill + 0x1d;
        m_pPLM->setReqLoad(0x47, 0);
        break;
    case 74:
        setMDFooting(0);
        m_fWallRadius = 1.0f;
        m_uiAttackID = m_iStockSkill + 0x1e;
        m_pPLM->setReqLoad(0x44, 0);
        m_pPLM->setReqLoad(0x47, 0);
        m_pPLM->setReqLoad(0x4a, 0);
        break;
    case 75:
    case 76:
        m_uiAttackID = m_iStockSkill + 0x1e;
        m_pPLM->setReqLoad(0x4b, 0);
        break;
    case 77:
    case 78:
        m_uiAttackID = m_iStockSkill + 0x1e;
        m_pPLM->setReqLoad(0x4d, 0);
        break;
    case 79:
    case 80:
        m_uiAttackID = m_iStockSkill + 0x1e;
        m_pPLM->setReqLoad(0x4f, 0);
        break;
    case 81:
    case 82:
        m_uiAttackID = m_iStockSkill + 0x1e;
        m_pPLM->setReqLoad(0x51, 0);
        break;
    case 83:
    case 84:
        m_uiAttackID = m_iStockSkill + 0x1e;
        m_pPLM->setReqLoad(0x53, 0);
        break;
    case 85:
    case 86:
        m_uiAttackID = m_iStockSkill + 0x23;
        m_pPLM->setReqLoad(0x55, 0);
        break;
    case 52:
    case 53:
        setMDFooting(0);
        m_fWallRadius = 1.0f;
        m_uiAttackID = m_iStockSkill + 0x4d;
        m_pPLM->setReqLoad(0x34, 0);
        break;
    case 54:
    case 55:
        setMDFooting(0);
        m_fWallRadius = 0.8f;
        m_uiAttackID = m_iStockSkill + 0x4d;
        m_pPLM->setReqLoad(0x36, 0);
        break;
    case 56:
    case 57:
    case 58:
        m_uiAttackID = m_iStockSkill + 0x4d;
        m_pPLM->setReqLoad(0x38, 0);
        break;
    case 59:
    case 60:
        m_vAppearPos = m_vHeadPos;
        m_uiAttackID = m_iStockSkill + 0x4d;
        m_pPLM->setReqLoad(0x3b, 0);
        break;
    case 61:
    case 62:
        setMDFooting(0);
        m_fWallRadius = 2.0f;
        m_uiAttackID = m_iStockSkill + 0x4d;
        m_pPLM->setReqLoad(0x3d, 0);
        break;
    case 63:
    case 64:
        m_uiAttackID = m_iStockSkill + 0x50;
        m_pPLM->setReqLoad(0x3f, 0);
        break;
    case 65:
    case 66:
    case 67:
        setMDFooting(0);
        m_fWallRadius = 0.8f;
        m_uiAttackID = m_iStockSkill + 0x50;
        m_pPLM->setReqLoad(0x41, 0);
        break;
    case 87:
        m_uiAttackID = 0x96;
        type = 1;
        break;
    case 88:
    case 89:
        m_uiAttackID = 0x95;
        type = 2;
        break;
    default:
        m_iSubState = 3;
        m_pAtkData = &g_AttackDataList[0];
        m_uiAttackID = 0;
        m_uiAttackKind = 0;
        return;
    }
    m_pAtkData = &g_AttackDataList[m_uiAttackID];
    switch (type) {
    case 1:
        m_iAttackPower = m_iAP * CardBattle::g_pStockSkillList[m_iStockSkill].POWER[m_pAtkData->POWER] / 100;
        break;
    case 2: {
        INT power = CardBattle::g_pStockSkillList[m_iStockSkill].POWER[m_pAtkData->POWER];
        m_iAttackPower = power / 10;
        break;
    }
    case 3:
        m_iAttackPower = CardBattle::g_pStockSkillList[m_iStockSkill].POWER[m_pAtkData->POWER];
        break;
    default:
        m_iAttackPower = 0;
        break;
    }
    _ref(m_AtkKind) = (ATK_KIND)m_pAtkData->DAMAGE;
    _ref(m_AtkAttr) = (ATK_ATTR)CardBattle::g_pStockSkillList[m_iStockSkill].ATTR;
    UINT up = _upKind(CardBattle::g_pStockSkillList[m_iStockSkill].UP_TYPE0,
                      CardBattle::g_pStockSkillList[m_iStockSkill].UP_TYPE1);
    calcEnchant_AttackPoint(m_iAttackPower, m_AtkKind, m_AtkAttr, up);
    m_iSubState = 1;
    m_uiPreAttackKind = m_uiAttackKind;
    m_fDistXZ = m_fDistY = 0.0f;
    setTrgFlag(0, 0x40000);
    if (isPlayerFlag(0x800000)) {
        setPlayerFlag(0x2000000, 0);
    }
}
/* Parked: 14 of 7930 lines differ, in three case groups (711/741/1042; all other jump-table groups match):
   the original loads m_pMove (lw 0xa0) between the two stores into the {0, d0, 0, 0} temp and stores m_fDir
   before copying the temp; ours (sched1) loads m_pMove after both temp stores and sched2 keeps it (post-reload
   sp vs s3 alias) - same open issue as exeGlide. tried: _ref/pointer/reference temp stores, compound literal and
   brace init (memset), m_pMove/setDir moved before the temp, register-asm and empty-asm barriers, two 30-min
   permuter runs (59->58, 36->35). Levers that matched the rest: separate early `case 1211:`/`case 1473:` copies
   of the procSkillEnd tail, const g_EffTbl_* (.rodata), function-scope effect/pHB/scale/pos/rot,
   _subAngle(getDir(), viewDir) + Homing(getDir(), ...) for the turning blocks, _ref(g_pStockSkillList)[..].POWER.
   The draft needs `void operator+=(const FVECTOR&)` in FVECTOR (Vector.h; clashes with LightUnit.cpp's free
   operator+=). */
#if 0
void CPlayerSora::exeStockSkill() {
    INT SESeq;
    FVECTOR pos;
    FVECTOR rot;
    INT i;
    CEffect* effect;
    CHomingBullet* pHB;
    FLOAT scale;
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
        _ref(m_hOldLockon) = m_hLockon;
        _ref(m_iOldCamMode) = PlayerCamera::CPlayerCamera::getNowCamMode();
        if (CardBattle::g_pStockSkillList[m_iStockSkill].CATEGORY == 5) {
            setBGFade(g_vBGFadeColor[0], 20.0f, 0);
            setAnimForce(0x2e, -1, 1, 0, SESeq, 0);
            setCastEffect(g_szDpxFriend, 0, &Crd(0), NULL);
            calcAppearPosFS_toAttackID();
            if ((m_pAtkData->FLAG & 0x20) && m_pPLM->isCompleteLoad(m_iStockSkill) != 1) {
                setObjStop(1);
                m_pCam->changeCamMode(5, 0);
            }
            m_iSubState = 0x34F;
            if (m_pCB->getEnchantKind() == 0x1b) {
                addHP(m_iMaxHP / 4, 6000, 1);
            }
            break;
        }
        if (CardBattle::g_pStockSkillList[m_iStockSkill].CATEGORY == 3) {
            setBGFade(g_vBGFadeColor[0], 20.0f, 0);
            setAnimForce(0x2e, -1, 1, 0, SESeq, 0);
            setCastEffect(g_szDpxSummon, 1, &Crd(0), NULL);
            calcAppearPosFS_toAttackID();
            if ((m_pAtkData->FLAG & 0x20) && m_pPLM->isCompleteLoad(m_iStockSkill) != 1) {
                setObjStop(1);
                m_pCam->changeCamMode(5, 0);
            }
            m_iSubState = 0x4BC;
            break;
        }
        if (isPlayerFlag(0x800000)) {
            setPlayerFlag(0x2000000, 0);
            m_fJumpGravity = g_fGravity * 0.16f;
        }
        m_uiStep = 0;
        if (m_pAtkData->FLAG & 0x20) {
            switch (m_uiAttackID * 10) {
            case 310:
                setBGFade(g_vBGFadeColor[5], 20.0f, 0);
                break;
            case 690:
            case 710:
            case 720:
                setBGFade(g_vBGFadeColor[2], 20.0f, 0);
                break;
            case 730:
            case 740:
                setBGFade(g_vBGFadeColor[1], 20.0f, 0);
                break;
            case 750:
            case 770:
                setBGFade(g_vBGFadeColor[3], 20.0f, 0);
                break;
            case 800:
            case 820:
                setBGFade(g_vBGFadeColor[4], 20.0f, 0);
                break;
            case 810:
                setBGFade(g_vBGFadeColor[5], 20.0f, 0);
                break;
            case 90:
                setBGFade(g_vBGFadeColor[4], 20.0f, 0);
                setCastEffect(g_szDpxStart, 2, &Crd(m_iBoneCenter), NULL);
                break;
            case 140:
                setBGFade(g_vBGFadeColor[1], 20.0f, 0);
                break;
            case 170:
                setBGFade(g_vBGFadeColor[2], 20.0f, 0);
                break;
            case 330:
                setBGFade(g_vBGFadeColor[0], 20.0f, 0);
                break;
            case 830:
            case 840:
            default:
                setBGFade(g_vBGFadeColor[0], 20.0f, 0);
                break;
            }
            if (m_pPLM->isCompleteLoad(m_iStockSkill) != 1) {
                setAnimForce(0x2e, -1, 1, 0, 0, 0);
                setObjStop(1);
                m_pCam->changeCamMode(5, 0);
                m_iSubState = 4;
                m_uiStep = 1;
                return;
            }
            setUseRsrcBlock(1);
        }
        m_iSubState = m_uiAttackID * 10;
        switch (m_iSubState) {
        case 310:
            setCastEffect(g_szDpxLimit3, 0, &Crd(m_iBoneCenter), NULL);
            break;
        case 140:
            setCastEffect(g_szDpxLimit1, 0, &Crd(m_iBoneCenter), NULL);
            break;
        case 170:
            setCastEffect(g_szDpxLimit2, 0, &Crd(m_iBoneCenter), NULL);
            break;
        case 480:
            setBGFade(g_vBGFadeColor[2], 20.0f, 0);
            break;
        case 490:
            setBGFade(g_vBGFadeColor[1], 20.0f, 0);
            break;
        case 500:
            setBGFade(g_vBGFadeColor[4], 20.0f, 0);
            break;
        case 130:
        case 520:
            setBGFade(g_vBGFadeColor[3], 20.0f, 0);
            break;
        case 510:
            setBGFade(g_vBGFadeColor[5], 20.0f, 0);
            break;
        }
        if (!(m_pAtkData->FLAG & 0x40)) {
            setAnimForce(m_pAtkData->ANIM_ID + m_iAerialAnimShift, 0, 1, m_pAtkData->ANIM_ESQ, SESeq, 0);
        } else {
            setAnimSkillEx(m_pAtkData->ANIM_ID + m_iAerialAnimShift, 0, 1, m_pAtkData->ANIM_ESQ, SESeq);
        }
        setPlayerFlag(0x100000, 0);
        m_tqStateTimer = 0;
        m_pMarker->holdTarget(8);
        procTargetMark();
        break;
    case 3:
        if (isMotionEnd()) {
            procSkillEnd(-1);
            return;
        }
        break;
    case 4:
        if (m_pPLM->isCompleteLoad(m_iStockSkill) != 1) break;
        if (m_uiStep == 1) {
            switch (m_uiAttackID * 10) {
            case 310:
                setCastEffect(g_szDpxLimit3, 0, &Crd(m_iBoneCenter), NULL);
                break;
            case 140:
                setCastEffect(g_szDpxLimit1, 0, &Crd(m_iBoneCenter), NULL);
                break;
            case 170:
                setCastEffect(g_szDpxLimit2, 0, &Crd(m_iBoneCenter), NULL);
                break;
            }
            m_uiStep = 0;
        }
        if (!isMotionLoop()) break;
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
        break;
    case 80:
        if (frame < m_pAtkData->MOVE_START_FRAME) {
            procTargetMark();
            procAttackMove(frame, 0.0f, 0.0f, 2, 0.15f);
            break;
        }
        procAttackMove(frame, m_pAtkData->RADIUS * 0.1f, m_pAtkData->MAX_SPEED, !m_bIsAttackHit, 0.15f);
        if (procAttackCollision(frame)) return;
        if (isMotionEnd()) {
            procSkillEnd(-1);
            return;
        }
        break;
    case 90:
        if (isTrgFlag(0x40000)) {
            setTrgFlag(0, 0x40000);
        }
    case 91:
        if (isPlayerFlag(0x100000)) {
            procTargetMark();
        }
        procAttackMove(frame, m_pAtkData->RADIUS * 0.1f, m_pAtkData->MAX_SPEED, 1, 0.15f);
        if (!isTrgFlag(0x40000)) {
            if (frame >= m_pAtkData->TRG_FRAME) {
                if (m_bNextTrg) {
                    if (isTrgStock()) {
                        m_bIsCanCombo = TRUE;
                    }
                }
                if (m_hTarget > 0 && !m_bNextTrg) {
                    FVECTOR vec;
                    SubVectorXYZ(vec, m_vMarkPos, m_vCenterPos);
                    vec.y = 0.0f;
                    FLOAT distSq = InnerProduct(vec, vec);
                    if (0.0f < distSq && distSq <= (FLOAT)(m_pAtkData->WORK0 * m_pAtkData->WORK0)) {
                        if (m_iSubState == 0x5B) {
                            setPushon(3);
                        } else {
                            setPushon(1);
                        }
                        m_bNextTrg = TRUE;
                        m_bIsCanCombo = FALSE;
                    }
                }
            } else if (isTrgStock()) {
                setTrgFlagRendaNG("exeStockSkill:  FINALBREAK  Entry NG...\n");
            }
        }
        if (frame >= m_pAtkData->COLL_END_FRAME && m_bIsCanCombo) {
            setPushon(0);
            offAttack();
            m_bIsCanCombo = FALSE;
            m_bNextTrg = FALSE;
            m_iSubState++;
            m_bIsAttackHit = FALSE;
            if (m_iSubState == 0x5C) {
                m_uiAttackID = 10;
                m_pAtkData = &g_AttackDataList[m_uiAttackID];
                m_iAttackPower = m_iAP * _ref(CardBattle::g_pStockSkillList)[m_iStockSkill].POWER[m_pAtkData->POWER] / 100;
                _ref(m_AtkKind) = (ATK_KIND)m_pAtkData->DAMAGE;
                _ref(m_AtkAttr) = (ATK_ATTR)CardBattle::g_pStockSkillList[m_iStockSkill].ATTR;
                UINT up = _upKind(CardBattle::g_pStockSkillList[m_iStockSkill].UP_TYPE0,
                                  CardBattle::g_pStockSkillList[m_iStockSkill].UP_TYPE1);
                calcEnchant_AttackPoint(m_iAttackPower, m_AtkKind, m_AtkAttr, up);
            }
            setAnimSkillEx(m_pAtkData->ANIM_ID, 0, 1, 0, 0);
            setPlayerFlag(0x100000, 0);
            break;
        }
        if (procAttackCollision(frame)) return;
        if (isMotionEnd()) {
            procSkillEnd(-1);
            return;
        }
        break;
    case 92:
        if (frame < m_pAtkData->TRG_FRAME) {
            if (frame >= m_pAtkData->MOVE_START_FRAME && m_fJumpSpdY <= 0.0f) {
                m_fJumpSpdY = g_fAntiGravity * m_pAtkData->WORK1;
            }
            procTargetMark();
            procAttackMove(frame, m_pAtkData->RADIUS * 0.1f, m_pAtkData->MAX_SPEED, 1, 0.15f);
        } else {
            if (g_fGravity * m_pParam->JUMP_POWER < m_fJumpSpdY) {
                m_fJumpSpdY = g_fGravity * m_pParam->JUMP_POWER;
                m_fMoveSpdXZ = 0.0f;
            }
            m_pAnim->setRootMove(0, 0);
            procAttackMove(frame, 0.0f, 0.0f, 0, 0.0f);
        }
        if (procAttackCollision(frame)) return;
        if (isMotionEnd()) {
            procSkillEnd(-1);
            return;
        }
        break;
    case 110:
        if (frame < m_pAtkData->MOVE_END_FRAME) {
            procTargetMark();
            procAttackMove(frame, 0.0f, 0.0f, 2, 0.15f);
        }
        if (frame >= m_pAtkData->TRG_FRAME && !m_bNextTrg) {
            BULLET_PARAM param;
            FMATRIX mat;
            m_bNextTrg = TRUE;
            param.init(1, 3);
            param.CollKind = COLL_KIND_PSHOT;
            param.uiFlag = 2;
            param.AtkKind = m_AtkKind;
            param.AtkAttr = m_AtkAttr;
            param.iAttackPoint = m_iAttackPower;
            param.fSpeed = 0.0f;
            param.fMaxDist = 0.0f;
            _unitMatrix(mat);
            _rotMatrixY(mat, getRot().y);
            param.vPos.set(0.0f, -g_OffsetTbl_StockSkill[3].y, g_OffsetTbl_StockSkill[3].z);
            ApplyMatrix(param.vPos, mat, param.vPos);
            param.vPos += getPos();
            param.vRot = getRot();
            param.pEffectTbl = (BULLET_EFFECT_INFO*)g_EffTbl_Skill[1];
            param.pOffsetTbl = g_OffsetTbl_StockSkill;
            param.fCollRadius = 2.0f;
            CEntityFactory::getInstance()->createBullet(param, this);
        }
        if (procAttackCollision(frame)) return;
        if (isMotionEnd()) {
            procSkillEnd(-1);
            return;
        }
        break;
    case 120:
        if (frame < m_pAtkData->MOVE_START_FRAME) {
            procTargetMark();
            procAttackMove(frame, 0.0f, 0.0f, 2, 0.15f);
            break;
        }
        procAttackMove(frame, m_pAtkData->RADIUS * 0.1f, m_pAtkData->MAX_SPEED, !m_bIsAttackHit, 0.15f);
        if (procAttackCollision(frame)) return;
        if (isMotionEnd()) {
            procSkillEnd(-1);
            return;
        }
        break;
    case 130:
    case 480:
    case 490:
    case 500:
    case 510:
    case 520:
        procTargetMark();
        procAttackMove(frame, 0.0f, 0.0f, 2, 0.15f);
        if (isMotionEnd()) {
            m_iSubState = 131;
            setAnimForce(0x40, 0, 0, m_pAtkData->ANIM_ESQ, 0, 0);
            setPlayerFlag(0x100000, 0);
        }
        break;
    case 131:
        if (isPlayerFlag(0x100000)) {
            procTargetMark();
        }
        if (frame < m_pAtkData->COLL_START_FRAME) {
            procAttackMove(frame, 0.0f, 0.0f, 2, 0.5f);
        }
        if (frame >= m_pAtkData->TRG_FRAME && !m_bNextTrg) {
            m_bNextTrg = TRUE;
            m_pWeapon->updateAttackID();
        }
        if (procAttackCollision(frame)) return;
        if (m_bIsAttackHit) {
            if (m_hTarget <= 0 || m_hTarget == m_pWeapon->getEnemyHandle()) {
                if (m_uiAttackID == 0x33) {
                    m_iSubState = 0x1FF;
                    m_pWeapon->setFreeMode(1, 0, NULL, &getRot());
                    setAnimForce(0x43, 0, 0, 0, 0, 0);
                    setTimer(1000);
                    break;
                } else if (m_uiAttackID == 0x34) {
                    FMATRIX mat = m_pWeapon->getMatrixWep0();
                    setEffect("recom", 0, &mat, 0.0f);
                    m_iSubState = 0x209;
                    m_uiAttackID = 0x35;
                    m_pAtkData = &g_AttackDataList[m_uiAttackID];
                    m_iAttackPower = m_iAP * _ref(CardBattle::g_pStockSkillList)[m_iStockSkill].POWER[m_pAtkData->POWER] / 100;
                    _ref(m_AtkKind) = (ATK_KIND)m_pAtkData->DAMAGE;
                    _ref(m_AtkAttr) = (ATK_ATTR)CardBattle::g_pStockSkillList[m_iStockSkill].ATTR;
                    UINT up = _upKind(CardBattle::g_pStockSkillList[m_iStockSkill].UP_TYPE0,
                                      CardBattle::g_pStockSkillList[m_iStockSkill].UP_TYPE1);
                    calcEnchant_AttackPoint(m_iAttackPower, m_AtkKind, m_AtkAttr, up);
                    setAnimForce(0x43, 0, 0, 1, 0, 0);
                    offAttack();
                    offWeapon();
                    setTimer(9000);
                    initMagicBullet();
                    CBattleEnemyManager* man = (CBattleEnemyManager*)CTaskMan::getSysTask(0x19);
                    if (man) {
                        m_nMagicBulletMax = man->getEnemyCount();
                    }
                    if (m_nMagicBulletMax < 2) {
                        m_nMagicBulletMax = 2;
                    } else if (m_nMagicBulletMax > 16) {
                        m_nMagicBulletMax = 16;
                    }
                    TRACE("CPlayerSora:: SS_REFRECTRAID : Bullet=%d/%d,\n", m_nMagicBullet, m_nMagicBulletMax);
                    m_iCount = 0;
                    setMagic(0x208, 0, m_iStockSkill);
                    m_iCount = 1;
                    break;
                }
            }
        }
        if (isMotionEnd()) {
            offAttack();
            m_iSubState = 0x84;
            setAnimForce(0x41, 0, 0, 0, 0, 0);
            return;
        }
        break;
    case 132:
    case 512:
    case 522:
        if (isMotionEnd()) {
            procSkillEnd(-1);
            return;
        }
        break;
    case 521:
        if (!checkMagicBullet()) {
            onWeapon(0);
            offAttack();
            m_iSubState = 0x84;
            setAnimForce(0x41, 0, 0, 0, 0, 0);
            return;
        }
        setMagic(0x208, 0, -1);
        break;
    case 511:
        if (m_hTarget > 0) {
            CEntity* pTarget = (CEntity*)CTaskMan::getTask(m_hTarget);
            if (pTarget) {
                pTarget->getLockOnPos(pos);
                m_pWeapon->setFreePos(pos);
            }
        }
        if (getTimer() <= 0) {
            m_pWeapon->updateAttackID();
            setTimer(2000);
        }
        if (m_pWeapon->isMotionEnd()) {
            m_pWeapon->setFreeMode(0, 0, NULL, NULL);
            offAttack();
            m_iSubState = 0x84;
            setAnimForce(0x41, 0, 0, 0, 0, 0);
            return;
        }
        break;
    case 140:
        procTargetMark();
        procAttackMove(frame, 0.0f, 0.0f, 2, 0.15f);
        if (isMotionEnd()) {
            m_iSubState = 141;
            m_bNextTrg = FALSE;
            m_bIsAttackHit = FALSE;
            m_uiAttackID = 0xF;
            m_pAtkData = &g_AttackDataList[m_uiAttackID];
            m_iAttackPower = m_iAP * _ref(CardBattle::g_pStockSkillList)[m_iStockSkill].POWER[m_pAtkData->POWER] / 100;
            _ref(m_AtkKind) = (ATK_KIND)m_pAtkData->DAMAGE;
            _ref(m_AtkAttr) = (ATK_ATTR)CardBattle::g_pStockSkillList[m_iStockSkill].ATTR;
            UINT up = _upKind(CardBattle::g_pStockSkillList[m_iStockSkill].UP_TYPE0,
                              CardBattle::g_pStockSkillList[m_iStockSkill].UP_TYPE1);
            calcEnchant_AttackPoint(m_iAttackPower, m_AtkKind, m_AtkAttr, up);
            setAnimSkillEx(m_pAtkData->ANIM_ID, 0, 1, 0, 0);
            setPlayerFlag(0x100000, 0);
            m_uiStep = 0;
        }
        break;
    case 141:
    case 142:
    case 143:
    case 144:
    case 145:
    case 146:
        if (frame < m_pAtkData->MOVE_START_FRAME) {
            procTargetMark();
            if (!m_uiStep) {
                procAttackMove(frame, 0.0f, 0.0f, 2, 1.0f);
                m_uiStep = 1;
            }
        } else if (frame < m_pAtkData->MOVE_END_FRAME) {
            procAttackMove(frame, m_pAtkData->RADIUS * 0.1f, m_pAtkData->MAX_SPEED, 1, 0.5f);
        } else {
            procAttackMove(frame, m_pAtkData->RADIUS * 0.1f, m_pAtkData->MAX_SPEED, 0, 0.5f);
        }
        if (!isTrgFlag(0x40000)) {
            if (frame >= m_pAtkData->COLL_START_FRAME) {
                if (m_bNextTrg) {
                    if (isTrgStock()) {
                        m_bIsCanCombo = TRUE;
                    }
                }
                if (frame < m_pAtkData->COLL_END_FRAME) {
                    if (m_hTarget > 0 && m_bIsAttackHit && !m_bNextTrg) {
                        if (m_iSubState == 0x92) {
                            setPushon(3);
                        } else {
                            setPushon(1);
                        }
                        m_bNextTrg = TRUE;
                        m_bIsCanCombo = FALSE;
                    }
                } else if (m_bIsCanCombo) {
                    setPushon(0);
                    offAttack();
                    m_bIsCanCombo = FALSE;
                    m_iSubState++;
                    m_bNextTrg = FALSE;
                    m_bIsAttackHit = FALSE;
                    m_uiAttackID = m_iSubState == 0x93 ? 0x10 : 0xF;
                    m_pAtkData = &g_AttackDataList[m_uiAttackID];
                    m_iAttackPower = m_iAP * _ref(CardBattle::g_pStockSkillList)[m_iStockSkill].POWER[m_pAtkData->POWER] / 100;
                    _ref(m_AtkKind) = (ATK_KIND)m_pAtkData->DAMAGE;
                    _ref(m_AtkAttr) = (ATK_ATTR)CardBattle::g_pStockSkillList[m_iStockSkill].ATTR;
                    UINT up = _upKind(CardBattle::g_pStockSkillList[m_iStockSkill].UP_TYPE0,
                                      CardBattle::g_pStockSkillList[m_iStockSkill].UP_TYPE1);
                    calcEnchant_AttackPoint(m_iAttackPower, m_AtkKind, m_AtkAttr, up);
                    setAnimSkillEx(m_pAtkData->ANIM_ID, 0, 1, 0, 0);
                    setPlayerFlag(0x100000, 0);
                    m_uiStep = 0;
                    break;
                }
            } else if (isTrgStock()) {
                setTrgFlagRendaNG("exeStockSkill:  SONICRAVE  Entry NG...\n");
            }
        }
        if (procAttackCollision(frame)) return;
        if (isMotionEnd()) {
            procSkillEnd(-1);
            return;
        }
        break;
    case 147:
        if (frame < m_pAtkData->MOVE_START_FRAME) {
            procTargetMark();
            if (!m_uiStep) {
                procAttackMove(frame, 0.0f, 0.0f, 2, 1.0f);
                m_uiStep = 1;
            }
            break;
        }
        if (frame < m_pAtkData->MOVE_END_FRAME) {
            procAttackMove(frame, m_pAtkData->RADIUS * 0.1f, m_pAtkData->MAX_SPEED, 1, 0.15f);
        } else {
            procAttackMove(frame, m_pAtkData->RADIUS * 0.1f, m_pAtkData->MAX_SPEED, 0, 0.15f);
        }
        if (procAttackCollision(frame)) return;
        if (isMotionEnd()) {
            procSkillEnd(-1);
            return;
        }
        break;
    case 170:
        procTargetMark();
        procAttackMove(frame, 0.0f, 0.0f, 2, 0.15f);
        if (isMotionEnd()) {
            m_iSubState = 171;
            m_bIsAttackHit = FALSE;
            m_uiAttackID = 0x12;
            m_pAtkData = &g_AttackDataList[m_uiAttackID];
            m_iAttackPower = m_iAP * _ref(CardBattle::g_pStockSkillList)[m_iStockSkill].POWER[m_pAtkData->POWER] / 100;
            _ref(m_AtkKind) = (ATK_KIND)m_pAtkData->DAMAGE;
            _ref(m_AtkAttr) = (ATK_ATTR)CardBattle::g_pStockSkillList[m_iStockSkill].ATTR;
            UINT up = _upKind(CardBattle::g_pStockSkillList[m_iStockSkill].UP_TYPE0,
                              CardBattle::g_pStockSkillList[m_iStockSkill].UP_TYPE1);
            calcEnchant_AttackPoint(m_iAttackPower, m_AtkKind, m_AtkAttr, up);
            setAnimSkillEx(m_pAtkData->ANIM_ID, 0, 1, 0, 0);
            setPlayerFlag(0x100000, 0);
        }
        break;
    case 171:
    case 172:
    case 173:
    case 174:
    case 175:
    case 176:
    case 177:
        if (isPlayerFlag(0x100000)) {
            procTargetMark();
        }
        procAttackMove(frame, m_pAtkData->RADIUS * 0.1f, m_pAtkData->MAX_SPEED, 1, 0.15f);
        if (frame >= m_pAtkData->COLL_END_FRAME) {
            m_bNextTrg = TRUE;
        }
        if (procAttackCollision(frame)) return;
        if (!isTrgFlag(0x40000)) {
            if (isTrgStock()) {
                setTrgFlagRendaNG("exeStockSkill:  LASTARCANUM  Entry NG...\n");
            }
        }
        if (m_bNextTrg || isMotionEnd()) {
            offAttack();
            m_bNextTrg = FALSE;
            m_bIsAttackHit = FALSE;
            m_uiAttackID = m_iSubState - 0x98;
            m_iSubState++;
            m_pAtkData = &g_AttackDataList[m_uiAttackID];
            m_iAttackPower = m_iAP * _ref(CardBattle::g_pStockSkillList)[m_iStockSkill].POWER[m_pAtkData->POWER] / 100;
            _ref(m_AtkKind) = (ATK_KIND)m_pAtkData->DAMAGE;
            _ref(m_AtkAttr) = (ATK_ATTR)CardBattle::g_pStockSkillList[m_iStockSkill].ATTR;
            UINT up = _upKind(CardBattle::g_pStockSkillList[m_iStockSkill].UP_TYPE0,
                              CardBattle::g_pStockSkillList[m_iStockSkill].UP_TYPE1);
            calcEnchant_AttackPoint(m_iAttackPower, m_AtkKind, m_AtkAttr, up);
            setAnimSkillEx(m_pAtkData->ANIM_ID, 0, 1, 0, 0);
            setPlayerFlag(0x100000, 0);
            setTrgFlag(0, 0x40000);
        }
        break;
    case 178:
    case 179:
    case 180:
    case 181:
    case 182:
        if (isPlayerFlag(0x100000)) {
            procTargetMark();
        }
        procAttackMove(frame, m_pAtkData->RADIUS * 0.1f, m_pAtkData->MAX_SPEED, 1, 0.15f);
        if (!isTrgFlag(0x40000)) {
            if (frame >= m_pAtkData->COLL_START_FRAME) {
                if (m_hTarget > 0 && m_bIsAttackHit && !m_bNextTrg) {
                    if (m_iSubState == 0xB6) {
                        setPushon(3);
                    } else {
                        setPushon(1);
                    }
                    m_bNextTrg = TRUE;
                    m_bIsCanCombo = FALSE;
                }
                if (m_bNextTrg) {
                    if (isTrgStock()) {
                        m_bIsCanCombo = TRUE;
                    }
                }
            } else if (isTrgStock()) {
                setTrgFlagRendaNG("exeStockSkill:  LASTARCANUM  Finish NG...\n");
            }
        }
        if (frame >= m_pAtkData->COLL_END_FRAME && m_bIsCanCombo) {
            setPushon(0);
            offAttack();
            m_bIsCanCombo = FALSE;
            m_bNextTrg = FALSE;
            m_bIsAttackHit = FALSE;
            m_uiAttackID = m_iSubState - 0x98;
            m_iSubState++;
            m_pAtkData = &g_AttackDataList[m_uiAttackID];
            m_iAttackPower = m_iAP * _ref(CardBattle::g_pStockSkillList)[m_iStockSkill].POWER[m_pAtkData->POWER] / 100;
            _ref(m_AtkKind) = (ATK_KIND)m_pAtkData->DAMAGE;
            _ref(m_AtkAttr) = (ATK_ATTR)CardBattle::g_pStockSkillList[m_iStockSkill].ATTR;
            UINT up = _upKind(CardBattle::g_pStockSkillList[m_iStockSkill].UP_TYPE0,
                              CardBattle::g_pStockSkillList[m_iStockSkill].UP_TYPE1);
            calcEnchant_AttackPoint(m_iAttackPower, m_AtkKind, m_AtkAttr, up);
            setAnimSkillEx(m_pAtkData->ANIM_ID, 0, 1, 0, 0);
            setPlayerFlag(0x100000, 0);
            setTrgFlag(0, 0x40000);
            break;
        }
        if (procAttackCollision(frame)) return;
        if (isMotionEnd()) {
            procSkillEnd(-1);
            return;
        }
        break;
    case 183:
        if (isPlayerFlag(0x100000)) {
            procTargetMark();
        }
        procAttackMove(frame, m_pAtkData->RADIUS * 0.1f, m_pAtkData->MAX_SPEED, 1, 0.15f);
        if (procAttackCollision(frame)) return;
        if (isMotionEnd()) {
            procSkillEnd(-1);
            return;
        }
        break;
    case 310:
        procTargetMark();
        procAttackMove(frame, 0.0f, 0.0f, 2, 0.15f);
        if (isMotionEnd()) {
            m_iSubState = 311;
            setAnimSkillEx(1, 0, 1, 0, 0);
            setPlayerFlag(0x100000, 0);
            m_fJumpSpdY = g_fAntiGravity * m_pAtkData->MAX_SPEED;
            setPlayerFlag(0x2000000, 0);
            fadeEffAttack(0);
            pos.set(0.32f, -0.71f, 0.03f);
            m_hEffAttack[0] = attachEffect("limit4", 2, &Crd(m_iBoneWeapon), &pos, NULL, 0.0f);
            effect = CEffectMan::getEffect(m_hEffAttack[0]);
            if (effect) {
                effect->setScale(0.0f, 0.0f, 0.0f);
            }
            if (m_hMark > 0) {
                m_pCam->changeNowPlayer(getHandle(), m_hMark);
                m_pCam->changeCamMode(4, 0);
            }
        }
        break;
    case 311:
        if (m_fJumpSpdY < 0.0f) {
            m_fJumpSpdY = 0.0f;
        }
        procTargetMark();
        procAttackMove(frame, 0.0f, 0.0f, 2, 0.15f);
        if (isTrgAttack() && frame >= 21) {
            if (frame <= m_pAtkData->COLL_END_FRAME) {
                m_iSubState = 312;
                setAnimSkillEx(2, 0, 1, 0, 0);
                m_bNextTrg = TRUE;
            } else {
                m_iSubState = 313;
                setAnimSkillEx(3, 0, 1, 0, 0);
                m_bNextTrg = TRUE;
            }
        } else if (isMotionEnd()) {
            m_iSubState = 313;
            setAnimSkillEx(3, 0, 1, 0, 0);
            m_bNextTrg = TRUE;
        }
        if (m_bNextTrg) {
            m_fJumpSpdY = 0.0f;
            m_fWalkSpeed = m_fJumpSpdXZ = m_fJumpSpdY;
            m_pMove->calcVelocityXZ(m_fWalkSpeed);
            clearSpeed();
            fadeEffAttack(0);
            INT max;
            if (frame <= m_pAtkData->COLL_START_FRAME) {
                max = m_pAtkData->WORK0;
            } else if (frame <= m_pAtkData->COLL_END_FRAME) {
                max = m_pAtkData->WORK1;
            } else {
                max = m_pAtkData->WORK2;
            }
            m_uiAttackID = 0x20;
            m_pAtkData = &g_AttackDataList[m_uiAttackID];
            m_iAttackPower = m_iAP * _ref(CardBattle::g_pStockSkillList)[m_iStockSkill].POWER[m_pAtkData->POWER] / 100;
            _ref(m_AtkKind) = (ATK_KIND)m_pAtkData->DAMAGE;
            _ref(m_AtkAttr) = (ATK_ATTR)CardBattle::g_pStockSkillList[m_iStockSkill].ATTR;
            UINT up = _upKind(CardBattle::g_pStockSkillList[m_iStockSkill].UP_TYPE0,
                              CardBattle::g_pStockSkillList[m_iStockSkill].UP_TYPE1);
            calcEnchant_AttackPoint(m_iAttackPower, m_AtkKind, m_AtkAttr, up);
            getWeaponTopPos(pos);
            rot = getRot();
            BULLET_PARAM param;
            param.init(2, 3);
            param.uiFlag = 0xB004;
            param.CollKind = COLL_KIND_PSHOT;
            param.AtkKind = m_AtkKind;
            param.AtkAttr = m_AtkAttr;
            param.iAttackPoint = m_iAttackPower;
            param.fSpeed = m_pAtkData->MAX_SPEED * 0.01f;
            param.fMaxDist = m_pAtkData->WORK0;
            param.tqHomingTime = m_pAtkData->MOVE_END_FRAME * 100;
            param.tqHomingTime2 = 9000;
            param.hHomingTarget = m_hTarget;
            param.fHomingRadius = m_pAtkData->RADIUS;
            param.fHomingRate = m_pAtkData->WORK1;
            param.vPos = pos;
            param.vRot = rot;
            param.pEffectTbl = (BULLET_EFFECT_INFO*)g_EffTbl_Skill[m_pAtkData->EFFECT];
            param.fCollRadius = m_pAtkData->COLL_START_FRAME * 0.01f;
            param.tqExplodeTime = 0;
            param.vPos.w = m_pAtkData->COLL_END_FRAME * 0.1f;
            initMagicBullet();
            for (i = 0; i < max; i++) {
                FLOAT r = i * 6.2831855f / max;
                param.vRot.x = _limitPI2(rot.x + sinf(r));
                param.vRot.y = _limitPI2(rot.y + cosf(r));
                pHB = (CHomingBullet*)CEntityFactory::getInstance()->createBullet(param, this);
                if (pHB) {
                    m_hMagicBullet[i] = pHB->getHandle();
                    m_nMagicBullet++;
                    m_nMagicBulletMax++;
                }
            }
            setTimer(param.tqHomingTime);
        } else {
            if (frame <= 20.0f) {
                effect = CEffectMan::getEffect(m_hEffAttack[0]);
                if (effect) {
                    scale = frame / 20.0f * 0.5f;
                    effect->setScale(scale, scale, scale);
                }
            } else {
                effect = CEffectMan::getEffect(m_hEffAttack[0]);
                if (effect) {
                    scale = (frame - 20.0f) / 100.0f * 1.5f + 0.5f;
                    effect->setScale(scale, scale, scale);
                }
            }
            FLOAT speed = 0.0f;
            rot = getRot();
            if (m_fAnalogPower >= 0.3f) {
                speed = 3.0f;
                FVECTOR v;
                {
                    /* the original's brace-initialized temporary ({0, dir, 0, 0}, cleared as one quadword) */
                    FVECTOR t;
                    t._q = 0;
                    t.y = m_fAnalogViewDir;
                    v = t;
                }
                setRot(v);
            }
            m_fWalkSpeed = speed / 60.0f;
            m_fJumpSpdXZ = Movement::Approach(m_fJumpSpdXZ, m_fWalkSpeed, 0.02f, m_fFrameRate);
            m_fWalkSpeed = m_fJumpSpdXZ;
            m_pMove->calcVelocityXZ(m_fWalkSpeed);
            setRot(rot);
        }
        if (m_fJumpSpdY <= 0.0f) {
            m_fJumpSpdY = 0.0f;
            return;
        }
        break;
    case 312:
    case 313:
        m_fJumpSpdY = 0.0f;
        if (isMotionEnd()) {
            m_iSubState = 314;
            setAnim(6, 800, 0);
            m_fJumpGravity = g_fGravity;
            PlayerCamera::CPlayerCamera::setBattleCamData(0, NULL);
            m_pCam->changeNowPlayer(0, 0);
            m_pCam->changeCamMode(-1, 0);
        }
        return;
    case 314:
        setGroundIK(1);
        if (!isAir()) {
            if (m_iNowAnimID != 0x2A) {
                if (m_iNowAnimID != 7) {
                    setAnim(7, -1, 0);
                } else if (isMotionEnd()) {
                    setAnim(0x2A, -1, 0);
                }
            }
        } else {
            FVECTOR vel = m_pMove->getVel();
            m_fJumpSpdY += m_fJumpGravity * m_fFrameRate;
            vel.y = m_fJumpSpdY;
            m_pMove->setVel(vel);
        }
        m_nMagicBullet = 0;
        for (i = 0; i < m_nMagicBulletMax; i++) {
            if (m_hMagicBullet[i] > 0 && CTaskMan::getTask(m_hMagicBullet[i])) {
                m_nMagicBullet++;
            }
        }
        if (m_nMagicBullet && getTimer() > 0) return;
        m_iSubState = 315;
        initMagicBullet();
        killBullet();
        setTimer(3000);
        return;
    case 315:
        setGroundIK(1);
        if (!isAir()) {
            if (m_iNowAnimID != 0x2A) {
                if (m_iNowAnimID != 7) {
                    setAnim(7, -1, 0);
                } else if (isMotionEnd()) {
                    setAnim(0x2A, -1, 0);
                }
            }
        }
        if (getTimer() > 0) break;
        procSkillEnd(0);
        return;
    case 330:
        setPlayerFlag(0x400000, 0);
        if (frame >= m_pAtkData->TRG_FRAME * 2 && !m_bNextTrg) {
            m_bNextTrg = TRUE;
            BULLET_PARAM param;
            param.init(1, 3);
            param.CollKind = COLL_KIND_PSHOT;
            param.uiFlag = 6;
            param.AtkKind = m_AtkKind;
            param.AtkAttr = m_AtkAttr;
            param.iAttackPoint = m_iAttackPower;
            param.tqHomingTime = 3000;
            param.fSpeed = 0.0f;
            param.fMaxDist = 0.0f;
            param.vPos = m_vCenterPos;
            param.vRot = getRot();
            param.pEffectTbl = (BULLET_EFFECT_INFO*)g_EffTbl_Skill[0];
            param.fCollRadius = 30.0f;
            CEntityFactory::getInstance()->createBullet(param, this);
            CGame::reqWhiteFadeIn(m_pAtkData->WORK1);
        }
        if (isMotionEnd()) {
            setAnim(0x2A, -1, 0);
            m_iSubState = 331;
            return;
        }
        break;
    case 331:
        if (!CGame::isFadeDone()) return;
        setTimer(3000);
        m_pEffectEx->kill(10.0f);
        m_pSEEx->stop();
        killLoopEffect(0);
        killBullet();
        m_iSubState = 332;
        break;
    case 410:
    case 420:
        m_fJumpSpdY = 0.0f;
        if (!m_bNextTrg) {
            m_bNextTrg = TRUE;
        }
        procTargetMark();
        procAttackMove(frame, 0.0f, 0.0f, 2, 0.15f);
        if (isMotionEnd()) {
            m_iSubState = 0x34E;
            setAnimForce(m_iNowAnimID + 1, 0, 1, m_pAtkData->EFFECT - 2, 0, 0);
            setMagic(0x22, 0, m_iStockSkill);
            setTimer(4000);
        }
        break;
    case 430:
    case 440:
    case 450:
        m_fJumpSpdY = 0.0f;
        if (!m_bNextTrg) {
            m_bNextTrg = TRUE;
        }
        procTargetMark();
        procAttackMove(frame, 0.0f, 0.0f, 2, 0.15f);
        if (frame < m_pAtkData->TRG_FRAME) break;
        m_iSubState = 0x34E;
        setMagic(0x23, 0, m_iStockSkill);
        setTimer(4000);
        break;
    case 460:
    case 470:
        m_fJumpSpdY = 0.0f;
        if (!m_bNextTrg) {
            m_bNextTrg = TRUE;
            effect = CEffectMan::getEffect(attachEffect(g_EffTbl_Skill[m_pAtkData->EFFECT][1].pEffectName, 0,
                                                                 &m_pWeapon->Crd(2), &PlayerWeapon::g_fWeaponTopOffset, NULL, 0.0f));
            if (effect) effect->m_iFlag |= 4;
            setBGFade(g_vBGFadeColor[0], 20.0f, 0);
        }
        procTargetMark();
        procAttackMove(frame, 0.0f, 0.0f, 2, 0.15f);
        if (frame < m_pAtkData->TRG_FRAME) break;
        m_iSubState = 0x34E;
        calcAppearPos((FLOAT)m_pAtkData->WORK0, (FLOAT)m_pAtkData->COLL_START_FRAME * 0.1f, NULL);
        m_vAppearPos.y = getPos().y;
        if (isAir()) m_vAppearPos.y = m_vJumpStartPos.y;
        m_vAppearPos.y += 6.0f;
        setMagic(0x24, 0, m_iStockSkill);
        setTimer(4000);
        break;
    case 540:
    case 550:
        m_fJumpSpdY = 0.0f;
        if (!m_bNextTrg) {
            m_bNextTrg = TRUE;
            attachEffect(g_EffTbl_Skill[m_pAtkData->EFFECT][1].pEffectName, 0, &m_pWeapon->Crd(2),
                         &PlayerWeapon::g_fWeaponTopOffset, NULL, 0.0f);
        }
        if (frame < m_pAtkData->TRG_FRAME) break;
        m_iSubState = 0x34E;
        setMagic(0x25, 0, m_iStockSkill);
        setTimer(4000);
        break;
    case 560:
    case 570:
        if (!m_bNextTrg) {
            m_bNextTrg = TRUE;
            attachEffect(g_EffTbl_Skill[m_pAtkData->EFFECT][2].pEffectName, 0, &m_pWeapon->Crd(2),
                         &PlayerWeapon::g_fWeaponTopOffset, NULL, 0.0f);
            calcAppearPos((FLOAT)m_pAtkData->WORK0, (FLOAT)m_pAtkData->COLL_START_FRAME * 0.01f, NULL);
            m_vAppearPos.y = getPos().y;
            if (m_hTarget > 0) {
                m_vAppearPos.x = m_vMarkPos.x;
                m_vAppearPos.z = m_vMarkPos.z;
            }
            if (isAir()) m_vAppearPos.y = m_vJumpStartPos.y;
            m_vAppearPos.y += 3.0f;
            setEffect(g_EffTbl_Skill[m_pAtkData->EFFECT][2].pEffectName, m_uiAttackID - 0x30, &m_vAppearPos, &getRot(), 0.0f);
        }
        procTargetMark();
        procAttackMove(frame, 0.0f, 0.0f, 2, 0.15f);
        if (frame < m_pAtkData->TRG_FRAME) break;
        m_iSubState = 0x34E;
        setMagic(0x26, 0, m_iStockSkill);
        setTimer(4000);
        break;
    case 580:
    case 590:
        m_fJumpSpdY = 0.0f;
        if (!m_bNextTrg) {
            m_bNextTrg = TRUE;
            pos.set(0.0f, 0.85f, 0.0f);
            setCastEffect(g_EffTbl_Skill[m_pAtkData->EFFECT][1].pEffectName, m_pAtkData->EFFECT - 0x15, &Crd(0), &pos);
        }
        procTargetMark();
        procAttackMove(frame, 0.0f, 0.0f, 2, 0.15f);
        if (frame < m_pAtkData->TRG_FRAME) break;
        m_iSubState = 0x34E;
        setMagic(0x27, 0, m_iStockSkill);
        setTimer(4000);
        break;
    case 600:
    case 610:
        m_fJumpSpdY = 0.0f;
        if (!m_bNextTrg) {
            m_bNextTrg = TRUE;
            attachEffect(g_EffTbl_Skill[m_pAtkData->EFFECT][1].pEffectName, 0, &m_pWeapon->Crd(2),
                         &PlayerWeapon::g_fWeaponTopOffset, NULL, 0.0f);
        }
        procTargetMark();
        procAttackMove(frame, 0.0f, 0.0f, 2, 0.15f);
        if (frame < m_pAtkData->TRG_FRAME) break;
        m_iSubState = 0x34E;
        setMagic(0x28, 0, m_iStockSkill);
        setTimer(4000);
        break;
    case 620:
        m_fJumpSpdY = 0.0f;
        procTargetMark();
        procAttackMove(frame, 0.0f, 0.0f, 2, 0.15f);
        if (frame < m_pAtkData->TRG_FRAME) break;
        m_iSubState = 0x34E;
        setMagic(0x3E, 0, m_iStockSkill);
        setTimer(4000);
        break;
    case 630:
        m_fJumpSpdY = 0.0f;
        procTargetMark();
        procAttackMove(frame, 0.0f, 0.0f, 2, 0.15f);
        if (frame < m_pAtkData->TRG_FRAME) break;
        m_iSubState = 0x34E;
        setMagic(0x3F, 0, m_iStockSkill);
        setTimer(4000);
        break;
    case 640:
        m_fJumpSpdY = 0.0f;
        procTargetMark();
        procAttackMove(frame, 0.0f, 0.0f, 2, 0.15f);
        if (frame < m_pAtkData->TRG_FRAME) break;
        m_iSubState = 641;
        setMagic(0x40, 0, m_iStockSkill);
        setTimer(3000);
        m_szEffectName = g_szDpxBind;
        break;
    case 641:
        setGroundIK(1);
        if (isAir()) {
            if (isMotionEnd()) {
                setAnim(6, -1, 0);
                m_fJumpGravity = g_fGravity;
            }
        } else if (m_iNowAnimID != 0x2A) {
            if (m_iNowAnimID != 7 && isPlayerFlag(0x800000)) {
                setAnim(7, -1, 0);
            } else if (isMotionEnd()) {
                setAnim(0x2A, -1, 0);
            }
        }
        if (getTimer() > 0) break;
        if (CEffectMan::getEffectNameNum(m_szEffectName)) break;
        procSkillEnd(-1);
        return;
    case 650:
        m_fJumpSpdY = 0.0f;
        procTargetMark();
        procAttackMove(frame, 0.0f, 0.0f, 2, 0.15f);
        if (frame < m_pAtkData->TRG_FRAME) break;
        fadeEffect(m_hEffCast, 1);
        m_iSubState = 651;
        setMagic(0x41, 0, m_iStockSkill);
        m_szEffectName = g_EffTbl_Skill[m_pAtkData->EFFECT][1].pEffectName;
        setTimer(3000);
        break;
    case 651:
        setGroundIK(1);
        if (isAir()) {
            if (isMotionEnd()) {
                setAnim(6, -1, 0);
                m_fJumpGravity = g_fGravity;
            }
        } else if (m_iNowAnimID != 0x2A) {
            if (m_iNowAnimID != 7 && isPlayerFlag(0x800000)) {
                setAnim(7, -1, 0);
            } else if (isMotionEnd()) {
                setAnim(0x2A, -1, 0);
            }
        }
        if (getTimer() > 0) break;
        if (CEffectMan::getEffectNameNum(m_szEffectName)) break;
        procSkillEnd(-1);
        return;
    case 660:
        m_fJumpSpdY = 0.0f;
        procTargetMark();
        procAttackMove(frame, 0.0f, 0.0f, 2, 0.15f);
        if (frame < m_pAtkData->TRG_FRAME) break;
        fadeEffect(m_hEffCast, 1);
        m_iSubState = 661;
        setMagic(0x42, 0, m_iStockSkill);
        m_szEffectName = g_EffTbl_Skill[m_pAtkData->EFFECT][1].pEffectName;
        setTimer(3000);
        break;
    case 661:
        setGroundIK(1);
        if (isAir()) {
            if (isMotionEnd()) {
                setAnim(6, -1, 0);
                m_fJumpGravity = g_fGravity;
            }
        } else if (m_iNowAnimID != 0x2A) {
            if (m_iNowAnimID != 7 && isPlayerFlag(0x800000)) {
                setAnim(7, -1, 0);
            } else if (isMotionEnd()) {
                setAnim(0x2A, -1, 0);
            }
        }
        if (getTimer() > 0) break;
        if (CEffectMan::getEffectNameNum(m_szEffectName)) break;
        procSkillEnd(-1);
        return;
    case 670:
        m_fJumpSpdY = 0.0f;
        procTargetMark();
        procAttackMove(frame, 0.0f, 0.0f, 2, 0.15f);
        if (frame < m_pAtkData->TRG_FRAME) break;
        m_iSubState = 671;
        setMagic(0x43, 0, m_iStockSkill);
        m_szEffectName = g_EffTbl_Skill[m_pAtkData->EFFECT][1].pEffectName;
        setTimer(9000);
        break;
    case 671:
        if (isMotionEnd()) {
            setAnim(0x2A, -1, 0);
        }
        if (getTimer() <= 0) {
            pHB = (CHomingBullet*)CTaskMan::getTask(m_hMagicCore);
            if (pHB) {
                pHB->out();
            }
            m_hMagicCore = 0;
        }
        if (m_hMagicCore > 0) {
            if (!CTaskMan::getTask(m_hMagicCore)) {
                m_hMagicCore = 0;
            }
        }
        if (m_hMagicCore) break;
        m_iSubState = 672;
        setTimer(3000);
        break;
    case 672:
        if (isMotionEnd()) {
            setAnim(0x2A, -1, 0);
        }
        if (getTimer() > 0) break;
        if (CEffectMan::getEffectNameNum(m_szEffectName)) break;
        procSkillEnd(-1);
        return;
    case 680:
        if (!m_bNextTrg) {
            m_bNextTrg = TRUE;
            setCastEffect(g_EffTbl_MagicCast[m_pAtkData->WORK2].pEffectName, 0, &Crd(0), NULL);
        }
        exeWarp(1);
        procTargetMark();
        procAttackMove(frame, 0.0f, 0.0f, 2, 0.3f);
        if (frame >= m_pAtkData->TRG_FRAME) {
            m_iSubState = 681;
            setMagic(0x44, 0, m_iStockSkill);
        }
        m_fJumpSpdY = 0.0f;
        {
            FVECTOR vel = m_pMove->getVel();
            vel.y = m_fJumpSpdY;
            m_pMove->setVel(vel);
        }
        return;
    case 681: {
        m_fJumpSpdY = 0.0f;
        calcAppearPosFS(m_fJumpSpdY, m_fWallRadius);
        FMATRIX m;
        if (m_hTarget > 0) {
            CEntity* pTarget = (CEntity*)CTaskMan::getTask(m_hTarget);
            if (pTarget) {
                m = pTarget->Crd(0).getWorldMatrix();
                PLM::getWorldRotY(m_fDir, m);
                rot = getRot();
                rot.y = m_fDir;
                setRot(rot);
                pTarget->getLockOnPos(m_vMarkPos);
                calcAppearPos(-1.5f, 0.55f, &m_vMarkPos);
            } else {
                calcAppearPos(1.5f, 0.55f, &m_vMarkPos);
            }
        } else {
            calcAppearPos(1.5f, 0.55f, &m_vMarkPos);
        }
        m_pCollMap->setType((COLL_TYPE)2);
        m_pCollMap->enableSliding(0);
        m_pCollMap->setRadius(m_fWallRadius);
        pos = getPos();
        FVECTOR v = {{0.0f, 0.0f, -m_fWallRadius, 1.0f}};
        _unitMatrix(m);
        _rotMatrixY(m, m_fDir);
        ApplyMatrixXYZ(v, m, v);
        AddVectorXYZ(pos, pos, v);
        m_pCollMap->setStartPos(pos);
        m_pCollMap->setEndPos(m_vAppearPos);
        FVECTOR vel;
        SubVectorXYZ(vel, m_vAppearPos, pos);
        FLOAT len0 = Math3D::VectorLength(vel);
        if (m_pCollMap->collideRide(0)) {
            FVECTOR* dpos = m_pCollMap->getDetectPos();
            SubVectorXYZ(vel, *dpos, pos);
            FLOAT length = Math3D::VectorLength(vel);
            if (length < len0) {
                len0 = length;
                m_vAppearPos = *dpos;
            }
        }
        if (m_pCollMap->collideMap()) {
            FVECTOR* dpos = m_pCollMap->getDetectPos();
            SubVectorXYZ(vel, *dpos, pos);
            FLOAT length = Math3D::VectorLength(vel);
            if (length < len0) {
                m_vAppearPos = *dpos;
            }
        }
        pos = getPos();
        setPos(m_vAppearPos);
        calcAppearPosFS(0.0f, m_fWallRadius);
        updateCoord();
        coordFix();
        setCastEffect(g_EffTbl_MagicCast[m_pAtkData->WORK2].pEffectName, 1, &Crd(0), NULL);
        m_szEffectName = g_EffTbl_MagicCast[m_pAtkData->WORK2].pEffectName;
        m_iSubState = 682;
        m_bNextTrg = FALSE;
        return;
    }
    case 682:
        if (!m_bNextTrg) {
            m_bNextTrg = TRUE;
            if (!isAir()) {
                setAnimForce(m_pAtkData->ANIM_ID, 0, 0, 0, 0, 0);
            } else {
                setAnimForce(m_pAtkData->ANIM_ID + m_iAerialAnimShift, 0, 0, 0, 0, 0);
            }
        }
        procTargetMark();
        procAttackMove(frame, 0.0f, 0.0f, 2, 0.3f);
        if (!exeWarp(0)) {
            m_fJumpSpdY = 0.0f;
            FVECTOR vel = m_pMove->getVel();
            vel.y = m_fJumpSpdY;
            m_pMove->setVel(vel);
            return;
        }
        if (CEffectMan::getEffectNameNum(m_szEffectName)) break;
        procSkillEnd(-1);
        return;
    case 690:
        setPlayerFlag(0x400000, 0);
        procTargetMark();
        procAttackMove(frame, 0.0f, 0.0f, 2, 0.15f);
        if (isMotionEnd()) {
            m_iSubState = 691;
            setAnimSkillEx(1, -1, 1, 0, 0);
            calcAppearPos((FLOAT)m_pAtkData->WORK0, (FLOAT)m_pAtkData->COLL_START_FRAME * 0.01f, NULL);
            m_vAppearPos.y = getPos().y;
            if (m_hTarget > 0) {
                m_vAppearPos.x = m_vMarkPos.x;
                m_vAppearPos.z = m_vMarkPos.z;
            }
            m_vAppearPos.y += m_pAtkData->MOVE_END_FRAME * 0.1f;
            getWeaponTopPos(pos);
            m_hEffAttack[0] = setEffect(g_EffTbl_Skill[m_pAtkData->EFFECT][3].pEffectName,
                                        g_EffTbl_Skill[m_pAtkData->EFFECT][3].iNo, &pos, NULL, 0.0f);
            m_szEffectName = g_EffTbl_Skill[m_pAtkData->EFFECT][3].pEffectName;
        }
        break;
    case 691:
        setPlayerFlag(0x400000, 0);
        if (frame < m_pAtkData->TRG_FRAME) break;
        m_iSubState = 692;
        fadeEffAttack(0);
        setMagic(0x2B2, 0, -1);
        m_tqStateTimer = 0;
        PlayerCamera::CPlayerCamera::setBattleCamData(getHandle(), &g_FiragaBurstCamParam);
        m_pCam->changeNowPlayer(getHandle(), m_hMagicCore);
        m_pCam->changeCamMode(4, 0);
        m_uiAttackID = 0x46;
        m_pAtkData = &g_AttackDataList[m_uiAttackID];
        m_iAttackPower = m_iAP * _ref(CardBattle::g_pStockSkillList)[m_iStockSkill].POWER[m_pAtkData->POWER] / 100;
        _ref(m_AtkKind) = (ATK_KIND)m_pAtkData->DAMAGE;
        _ref(m_AtkAttr) = (ATK_ATTR)CardBattle::g_pStockSkillList[m_iStockSkill].ATTR;
        {
            UINT up = _upKind(CardBattle::g_pStockSkillList[m_iStockSkill].UP_TYPE0,
                              CardBattle::g_pStockSkillList[m_iStockSkill].UP_TYPE1);
            calcEnchant_AttackPoint(m_iAttackPower, m_AtkKind, m_AtkAttr, up);
        }
        setTimer(2500);
        break;
    case 692:
        setPlayerFlag(0x400000, 0);
        if (getTimer() > 0) break;
        m_iSubState = 693;
        setTimer(m_pAtkData->COLL_END_FRAME * 100);
        m_iCount = 0;
    case 693:
        setPlayerFlag(0x400000, 0);
        if (m_hMagicCore > 0 && m_iCount < 40 && getTimer() > 0) {
            setMagic(0x2B2, 0, m_iStockSkill);
            m_iCount++;
            break;
        }
        m_iSubState = 694;
        setAnimSkillEx(2, -1, 1, 0, 0);
        break;
    case 694:
        if (m_hMagicCore > 0) {
            if (!CTaskMan::getTask(m_hMagicCore)) {
                m_hMagicCore = 0;
            }
        }
        if (isMotionEnd()) {
            m_hMagicCore = 0;
            PlayerCamera::CPlayerCamera::setBattleCamData(0, NULL);
            m_pCam->changeNowPlayer(0, 0);
            m_pCam->changeCamMode(-1, 0);
            m_iSubState = 695;
            setAnim(0x2A, -1, 0);
            setTimer(1500);
            killBullet();
        }
        break;
    case 695:
        if (isMotionEnd()) {
            setAnim(0x2A, -1, 0);
        }
        if (getTimer() > 0) break;
        if (CEffectMan::getEffectNameNum(m_szEffectName)) break;
        procSkillEnd(-1);
        return;
    case 710:
        if (!m_uiStep && frame >= 98) {
            setEffAttack(0, g_EffTbl_Skill[m_pAtkData->EFFECT][1].pEffectName, 4, 0, NULL);
            m_uiStep++;
        }
        if (isMotionEnd()) {
            m_iSubState = 711;
            setAnimSkillEx(1, -1, 0, 0, 0);
            m_fJumpSpdY = g_fAntiGravity * m_pAtkData->WORK1;
            m_tqStateTimer = 0;
            setTimer(m_pAtkData->TRG_FRAME * 100);
            m_iCount = 0;
        }
        break;
    case 711: {
        if (m_tqStateTimer < m_pAtkData->COLL_END_FRAME * 100) {
            if (getTimer() <= 0) {
                setMagic(0x2C6, 0, m_iStockSkill);
                m_iCount++;
                if (m_iCount >= 8) {
                    m_iCount = 0;
                }
                setTimer(m_pAtkData->TRG_FRAME * 100);
            }
        }
        if (m_tqStateTimer >= m_pAtkData->MOVE_END_FRAME * 100) {
            m_iSubState = 712;
            setAnimSkillEx(3, -1, 0, 0, 0);
            fadeEffAttack(1);
            killLoopEffect(1);
            if (isPlayerFlag(0x40000000)) {
                m_pWeapon->killLoopEffect(1);
            }
            m_fWalkSpeed = m_fJumpSpdXZ = 0.0f;
            m_pMove->calcVelocityXZ(m_fWalkSpeed);
            clearSpeed();
            break;
        }
        FLOAT speed = 0.0f;
        if (m_fAnalogPower >= 0.3f) {
            speed = m_pAtkData->WORK2 * 0.1f;
            FLOAT r = _subAngle(getDir(), m_fAnalogViewDir);
            FLOAT d0 = Movement::Homing(getDir(), m_fAnalogViewDir, 0.05f, m_fFrameRate);
            FVECTOR v;
            {
                /* the original's brace-initialized temporary ({0, dir, 0, 0}, cleared as one quadword) */
                FVECTOR t;
                t._q = 0;
                t.y = d0;
                v = t;
            }
            setDir(d0);
            m_pMove->setRot(v);
            if (m_pAnim->getAnimID() != 2) {
                setAnimSkillEx(2, -1, 0, 0, 0);
            }
        } else if (m_pAnim->getAnimID() != 1) {
            setAnimSkillEx(1, -1, 0, 0, 0);
        }
        m_fWalkSpeed = speed / 60.0f;
        m_fJumpSpdXZ = Movement::Approach(m_fJumpSpdXZ, m_fWalkSpeed, 0.02f, m_fFrameRate);
        m_fWalkSpeed = m_fJumpSpdXZ;
        m_pMove->calcVelocityXZ(m_fWalkSpeed);
        if (m_fJumpSpdY <= 0.0f) {
            m_fJumpSpdY = 0.0f;
            FVECTOR vel = m_pMove->getVel();
            vel.y = m_fJumpSpdY;
            m_pMove->setVel(vel);
            return;
        }
        break;
    }
    case 712:
        if (isMotionEnd()) {
            procSkillEnd(-1);
            return;
        }
        break;
    case 720:
        setPlayerFlag(0x400000, 0);
        procTargetMark();
        procAttackMove(frame, 0.0f, 0.0f, 2, 0.15f);
        if (isMotionEnd()) {
            m_iSubState = 721;
            setAnimSkillEx(1, -1, 0, 0, 0);
        }
        break;
    case 721:
        setPlayerFlag(0x400000, 0);
        if (!m_bNextTrg) {
            m_bNextTrg = TRUE;
            setMagic(0x2D0, 0, m_iStockSkill);
            m_pCam->changeNowPlayer(getHandle(), m_hMagicCore);
            m_pCam->changeCamMode(4, 0);
            setTimer(m_pAtkData->WORK2 * 100);
        }
        if (isMotionEnd()) {
            m_iSubState = 722;
        }
        break;
    case 722:
        setPlayerFlag(0x400000, 0);
        if (m_hMagicCore > 0 && !CTaskMan::getTask(m_hMagicCore)) {
            m_hMagicCore = 0;
        }
        if (getTimer() > 0 && m_hMagicCore > 0) break;
        if (m_pAnim->getAnimID() != 2) {
            setAnimSkillEx(2, -1, 0, 0, 0);
        }
        if (isMotionEnd()) {
            m_hMagicCore = 0;
            PlayerCamera::CPlayerCamera::setBattleCamData(0, NULL);
            m_pCam->changeNowPlayer(0, 0);
            m_pCam->changeCamMode(-1, 0);
            procSkillEnd(-1);
            return;
        }
        break;
    case 730:
        setPlayerFlag(0x400000, 0);
        procTargetMark();
        procAttackMove(frame, 0.0f, 0.0f, 2, 0.15f);
        if (isMotionEnd()) {
            m_iSubState = 731;
            setAnimSkillEx(1, -1, 0, 0, 0);
        }
        break;
    case 731:
        setPlayerFlag(0x400000, 0);
        if (frame >= m_pAtkData->TRG_FRAME) {
            m_iSubState = 732;
            setMagic(0x2DA, 0, m_iStockSkill);
        }
        break;
    case 732:
        setPlayerFlag(0x400000, 0);
        if (isMotionEnd()) {
            procSkillEnd(-1);
            return;
        }
        break;
    case 740:
        procTargetMark();
        procAttackMove(frame, 0.0f, 0.0f, 2, 0.15f);
        if (m_pAnim->getAnimID() == 0) {
            if (isMotionEnd()) {
                setAnimSkillEx(1, -1, 0, 0, 0);
                m_fJumpSpdY = g_fAntiGravity * m_pAtkData->WORK1;
            }
            break;
        }
        if (isMotionEnd()) {
            m_iSubState = 741;
            setAnimSkillEx(2, -1, 0, 0, 0);
            m_tqStateTimer = 0;
            m_iCount = 0;
        }
        if (m_fJumpSpdY <= 0.0f) {
            m_fJumpSpdY = 0.0f;
            FVECTOR vel = m_pMove->getVel();
            vel.y = m_fJumpSpdY;
            m_pMove->setVel(vel);
            return;
        }
        break;
    case 741: {
        if (m_tqStateTimer < m_pAtkData->COLL_END_FRAME * 100) {
            if (getTimer() <= 0) {
                setMagic(0x2E4, 0, m_iStockSkill);
                setTimer(m_pAtkData->TRG_FRAME * 100);
                m_iCount++;
                if (m_iCount >= 5) {
                    m_iCount = 0;
                }
            }
        }
        if (m_tqStateTimer >= m_pAtkData->MOVE_END_FRAME * 100) {
            m_iSubState = 742;
            setAnimSkillEx(3, -1, 0, 0, 0);
            fadeEffAttack(0);
            m_fWalkSpeed = m_fJumpSpdXZ = 0.0f;
            m_pMove->calcVelocityXZ(m_fWalkSpeed);
            clearSpeed();
            break;
        }
        FLOAT speed = 0.0f;
        if (m_fAnalogPower >= 0.3f) {
            speed = m_pAtkData->WORK2 * 0.1f;
            FLOAT r = _subAngle(getDir(), m_fAnalogViewDir);
            FLOAT d0 = Movement::Homing(getDir(), m_fAnalogViewDir, 0.03f, m_fFrameRate);
            FVECTOR v;
            {
                /* the original's brace-initialized temporary ({0, dir, 0, 0}, cleared as one quadword) */
                FVECTOR t;
                t._q = 0;
                t.y = d0;
                v = t;
            }
            setDir(d0);
            m_pMove->setRot(v);
        }
        m_fWalkSpeed = speed / 60.0f;
        m_fJumpSpdXZ = Movement::Approach(m_fJumpSpdXZ, m_fWalkSpeed, 0.1f, m_fFrameRate);
        m_fWalkSpeed = m_fJumpSpdXZ;
        m_pMove->calcVelocityXZ(m_fWalkSpeed);
        if (m_fJumpSpdY <= 0.0f) {
            m_fJumpSpdY = 0.0f;
            FVECTOR vel = m_pMove->getVel();
            vel.y = m_fJumpSpdY;
            m_pMove->setVel(vel);
            return;
        }
        break;
    }
    case 742:
        if (isMotionEnd()) {
            procSkillEnd(-1);
            return;
        }
        if (m_fJumpSpdY <= 0.0f) {
            m_fJumpSpdY = 0.0f;
            FVECTOR vel = m_pMove->getVel();
            vel.y = m_fJumpSpdY;
            m_pMove->setVel(vel);
            return;
        }
        break;
    case 750:
        setPlayerFlag(0x400000, 0);
        if (!m_bNextTrg && frame >= m_pAtkData->COLL_START_FRAME) {
            m_bNextTrg = TRUE;
            fadeEffAttack(0);
            pos.set(0.0f, 0.95f, 0.0f, 1.0f);
            m_hEffAttack[0] = attachEffect("magnespiral", 3, &Crd(0), &pos, NULL, 0.0f);
        }
        if (m_bNextTrg && frame <= m_pAtkData->COLL_END_FRAME) {
            effect = CEffectMan::getEffect(m_hEffAttack[0]);
            if (effect) {
                scale = (frame - m_pAtkData->COLL_START_FRAME) * 0.0625f;
                if (scale > 1.0f) scale = 1.0f;
                effect->setScale(scale, scale, scale);
            }
        }
        if (frame >= m_pAtkData->TRG_FRAME) {
            m_iSubState = 751;
            setMagic(0x2EE, 0, m_iStockSkill);
            setTimer(m_pAtkData->WORK0 * 50);
        }
        break;
    case 751:
        setPlayerFlag(0x400000, 0);
        if (getTimer() > 0) break;
        m_iSubState = 752;
        setAnimSkillEx(1, -1, 0, 0, 0);
        m_bIsAttackHit = FALSE;
        m_uiAttackID = 0x4C;
        m_pAtkData = &g_AttackDataList[m_uiAttackID];
        m_iAttackPower = m_iAP * _ref(CardBattle::g_pStockSkillList)[m_iStockSkill].POWER[m_pAtkData->POWER] / 100;
        _ref(m_AtkKind) = (ATK_KIND)m_pAtkData->DAMAGE;
        _ref(m_AtkAttr) = (ATK_ATTR)CardBattle::g_pStockSkillList[m_iStockSkill].ATTR;
        {
            UINT up = _upKind(CardBattle::g_pStockSkillList[m_iStockSkill].UP_TYPE0,
                              CardBattle::g_pStockSkillList[m_iStockSkill].UP_TYPE1);
            calcEnchant_AttackPoint(m_iAttackPower, m_AtkKind, m_AtkAttr, up);
        }
        break;
    case 752:
        setPlayerFlag(0x400000, 0);
        if (procAttackCollision(frame)) return;
        if (isMotionEnd()) {
            procSkillEnd(-1);
            return;
        }
        if (frame <= 16.0f) {
            effect = CEffectMan::getEffect(m_hEffAttack[0]);
            if (!effect) break;
            if (m_bNextTrg) {
                pos = getPos();
                pos.y = 0.95f;
                setEffect("magnespiral", 4, &pos, NULL, 0.0f);
                m_bNextTrg = FALSE;
            }
            scale = (16.0f - frame) * 0.0625f;
            effect->setScale(scale, scale, scale);
            break;
        }
        fadeEffAttack(0);
        break;
    case 770:
        setPlayerFlag(0x400000, 0);
        procTargetMark();
        procAttackMove(frame, 0.0f, 0.0f, 2, 0.15f);
        if (frame >= m_pAtkData->TRG_FRAME) {
            if (!m_bNextTrg) {
                m_bNextTrg = TRUE;
                setBGFade(g_vBGFadeColor[6], 20.0f, 0);
                if (m_hMark > 0) {
                    pos.set(0.0f, 0.85f, 0.0f, 1.0f);
                    setCastEffect(g_EffTbl_HitMark[17].pEffectName, 0, &Crd(0), &pos);
                    setMagic(0x302, 0, m_iStockSkill);
                    setTrgFlag(0x80000, 0);
                } else {
                    m_iSubState = 772;
                }
            } else {
                setTrgFlag(0x80000, 0);
            }
        }
        if (isMotionEnd()) {
            m_iSubState = 771;
            setAnimSkillEx(1, -1, 0, 0, 0);
            m_bIsAttackHit = FALSE;
            m_uiAttackID = 0x4E;
            m_pAtkData = &g_AttackDataList[m_uiAttackID];
            m_iAttackPower = m_iAP * _ref(CardBattle::g_pStockSkillList)[m_iStockSkill].POWER[m_pAtkData->POWER] / 100;
            _ref(m_AtkKind) = (ATK_KIND)m_pAtkData->DAMAGE;
            _ref(m_AtkAttr) = (ATK_ATTR)CardBattle::g_pStockSkillList[m_iStockSkill].ATTR;
            UINT up = _upKind(CardBattle::g_pStockSkillList[m_iStockSkill].UP_TYPE0,
                              CardBattle::g_pStockSkillList[m_iStockSkill].UP_TYPE1);
            calcEnchant_AttackPoint(m_iAttackPower, m_AtkKind, m_AtkAttr, up);
            setPlayerFlag(0x100000, 0);
            setTimer(m_pAtkData->WORK0 * 50);
            m_bNextTrg = FALSE;
            m_nHitCount = 0;
        }
        break;
    case 771:
        if (isPlayerFlag(0x100000)) {
            procTargetMark();
        }
        procAttackMove(frame, m_pAtkData->RADIUS * 0.1f, m_pAtkData->MAX_SPEED, 1, 0.15f);
        if (frame < m_pAtkData->COLL_END_FRAME) {
            if (!m_bNextTrg && m_nHitCount == 5) {
                m_bNextTrg = TRUE;
                CEntity* pTask = (CEntity*)CTaskMan::getTask(m_hMark);
                if (pTask) {
                    pTask->getLockOnPos(m_vMarkPos);
                    setEffect(g_EffTbl_HitMark[17].pEffectName, 4, &m_vMarkPos, &getRot(), 0.0f);
                }
            }
            if (getTimer() <= 0 && m_nHitCount < 4) {
                if (m_nHitCount == 0) {
                    fadeEffAttack(0);
                }
                m_nHitCount++;
                if (m_nHitCount == 4) {
                    m_uiAttackID = 0x4F;
                    m_pAtkData = &g_AttackDataList[m_uiAttackID];
                    m_iAttackPower = m_iAP * _ref(CardBattle::g_pStockSkillList)[m_iStockSkill].POWER[m_pAtkData->POWER] / 100;
                    _ref(m_AtkKind) = (ATK_KIND)m_pAtkData->DAMAGE;
                    _ref(m_AtkAttr) = (ATK_ATTR)CardBattle::g_pStockSkillList[m_iStockSkill].ATTR;
                    UINT up = _upKind(CardBattle::g_pStockSkillList[m_iStockSkill].UP_TYPE0,
                                      CardBattle::g_pStockSkillList[m_iStockSkill].UP_TYPE1);
                    calcEnchant_AttackPoint(m_iAttackPower, m_AtkKind, m_AtkAttr, up);
                    onAttack();
                }
                m_pWeapon->updateAttackID();
                setTimer(m_pAtkData->WORK1 * 50);
            }
        }
        if (procAttackCollision(frame)) {
            setMagic(0x302, 0, -1);
            return;
        }
        if (frame >= m_pAtkData->TRG_FRAME && m_nHitCount > 0 && m_nHitCount < 5) {
            if (m_bIsAttackHit) {
                CEntity* pTask = (CEntity*)CTaskMan::getTask(m_hMark);
                if (pTask) {
                    pTask->getLockOnPos(m_vMarkPos);
                    setEffect(g_EffTbl_HitMark[17].pEffectName, 2, &m_vMarkPos, &getRot(), 0.0f);
                }
                m_nHitCount = 5;
            }
        }
        if (frame >= m_pAtkData->WORK2) {
            if (isPlayerFlag(0x80000000)) {
                setBGFade(1.0f, 20.0f, 0);
                setMagic(0x302, 0, -1);
            }
        } else {
            setTrgFlag(0x80000, 0);
        }
        if (isMotionEnd()) {
            m_iSubState = 772;
            setAnim(0x2A, -1, 0);
            setTimer(2000);
        }
        break;
    case 772:
        if (getTimer() > 0) break;
        CEffectMan::killAll((ObjType::eObjBit)2, 0.0f);
        fadeEffect(m_hEffCast, 0);
        fadeEffAttack(0);
        m_pEffect->kill(10.0f);
        m_pEffectEx->kill(10.0f);
        m_pSE->stop();
        m_pSEEx->stop();
        killLoopEffect(0);
        if (isPlayerFlag(0x40000000)) {
            m_pWeapon->killEffect(10.0f);
            m_pWeapon->killLoopEffect(0);
        }
        procSkillEnd(-1);
        return;
    case 800:
        setPlayerFlag(0x400000, 0);
        if (frame < m_pAtkData->MOVE_START_FRAME) {
            procTargetMark();
            procAttackMove(frame, 0.0f, 0.0f, 2, 0.15f);
        }
        if (frame < m_pAtkData->TRG_FRAME) break;
        m_iSubState = 802;
        setMagic(0x320, 0, m_iStockSkill);
        m_szEffectName = g_EffTbl_Skill[m_pAtkData->EFFECT][1].pEffectName;
        setTimer(3000);
        break;
    case 802:
        if (isMotionEnd()) {
            setAnim(0x2A, -1, 0);
        }
        if (getTimer() > 0) break;
        if (CEffectMan::getEffectNameNum(m_szEffectName)) break;
        procSkillEnd(-1);
        return;
    case 810:
        setPlayerFlag(0x400000, 0);
        procTargetMark();
        if (!m_uiStep) {
            procAttackMove(frame, 0.0f, 0.0f, 2, 1.0f);
            m_uiStep = 1;
        }
        if (frame < m_pAtkData->COLL_START_FRAME) break;
        m_iSubState = 811;
        setMagic(0x32A, 0, m_iStockSkill);
        m_szEffectName = g_EffTbl_Skill[m_pAtkData->EFFECT][1].pEffectName;
        setTimer(m_pAtkData->MOVE_END_FRAME * 100);
        PlayerCamera::CPlayerCamera::setBattleCamData(getHandle(), &g_TornadoCamParam);
        m_pCam->changeNowPlayer(m_hMagicCore, 0);
        m_pCam->changeCamMode(-1, 0);
        break;
    case 811:
        if (m_hMagicCore > 0) {
            pHB = (CHomingBullet*)CTaskMan::getTask(m_hMagicCore);
            if (pHB) {
                FVECTOR vel = pHB->getVel();
                if (m_fAnalogPower >= 0.3f) {
                    FVECTOR dir;
                    Normalize(dir, vel);
                    rot.x = -atan2f(dir.y, _sqrtf(dir.z * dir.z + dir.x * dir.x));
                    rot.y = atan2f(dir.x, dir.z);
                    rot.z = 0.0f;
                    rot.y = Movement::Homing(rot.y, m_fAnalogViewDir, m_pAtkData->WORK0 * 0.01f, m_fFrameRate);
                    FLOAT speed = m_pAtkData->MAX_SPEED * 0.1f / 60.0f;
                    FVECTOR v = {{0.0f, -0.1f, 1.0f, 1.0f}};
                    FMATRIX m;
                    _unitMatrixVu0(m);
                    _rotMatrixY(m, rot.y);
                    ApplyMatrixXYZ(v, m, v);
                    ScaleVectorXYZ(v, v, speed);
                    vel.x = v.x;
                    vel.y = v.y;
                    vel.z = v.z;
                    pHB->setVel(vel);
                }
                pos = pHB->getOldPos();
                pos.y += m_pAtkData->WORK1 * 0.1f;
                pHB->setOldPos(pos);
            }
        }
        if (getTimer() > 0) break;
        m_hMagicCore = 0;
        PlayerCamera::CPlayerCamera::setBattleCamData(0, NULL);
        m_pCam->changeNowPlayer(0, 0);
        m_pCam->changeCamMode(-1, 0);
        m_iSubState = 812;
        setAnimSkillEx(1, -1, 0, 0, 0);
        setTimer(3000);
        break;
    case 812:
        if (isMotionEnd()) {
            setAnim(0x2A, -1, 0);
        }
        if (getTimer() > 0) break;
        if (CEffectMan::getEffectNameNum(m_szEffectName)) break;
        procSkillEnd(-1);
        return;
    case 820:
        setPlayerFlag(0x400000, 0);
        if (frame < m_pAtkData->MOVE_END_FRAME) {
            procTargetMark();
            procAttackMove(frame, 0.0f, 0.0f, 2, 0.15f);
        }
        if (frame < m_pAtkData->TRG_FRAME) break;
        if (m_bNextTrg) break;
        m_bNextTrg = TRUE;
        {
            pos = getPos();
            pos.y += 0.1f;
            setEffect(g_EffTbl_Skill[m_pAtkData->EFFECT][1].pEffectName, 0, &pos, NULL, 0.0f);
        }
        setMagic(0x334, 0, m_iStockSkill);
        setQuake(10.0f, 0.25f, 0x32);
        m_iSubState = 822;
        m_szEffectName = g_EffTbl_Skill[m_pAtkData->EFFECT][1].pEffectName;
        setTimer(3000);
        break;
    case 822:
        if (isMotionEnd()) {
            setAnim(0x2A, -1, 0);
        }
        if (getTimer() > 0) break;
        if (CEffectMan::getEffectNameNum(m_szEffectName)) break;
        procSkillEnd(-1);
        return;
    case 830:
        setPlayerFlag(0x400000, 0);
        if (!m_bNextTrg) {
            m_tqStateTimer = 0;
            setCastEffect(g_EffTbl_MagicCast[m_pAtkData->WORK2].pEffectName, 0, &Crd(0), NULL);
            setEffAttack(0, g_EffTbl_MagicCast[m_pAtkData->WORK2].pEffectName, 1, 0, NULL);
            m_hEffAttack[1] = setEffect(g_EffTbl_MagicCast[m_pAtkData->WORK2].pEffectName, 2, &getPos(), NULL, 0.0f);
            m_bNextTrg = TRUE;
        }
        if (m_tqStateTimer >= m_pAtkData->TRG_FRAME * 100) {
            m_iSubState = 831;
            setEffect("itemplus", 1, &m_vCenterPos, NULL, 0.0f);
            m_pRevolver->setAction(0x30, 0);
        }
        if (m_bNextTrg) {
            updateSpotLight(m_hEffAttack[1]);
        }
        break;
    case 831:
        setPlayerFlag(0x400000, 0);
        if (m_tqStateTimer >= m_pAtkData->WORK0 * 100) {
            m_iSubState = 832;
            setMagic(0x25, 0, m_iStockSkill);
            setMagic(0x33E, 0, m_iStockSkill);
        }
        updateSpotLight(m_hEffAttack[1]);
        break;
    case 832:
        setPlayerFlag(0x400000, 0);
        if (m_tqStateTimer >= m_pAtkData->WORK1 * 100) {
            fadeEffect(m_hEffCast, 0);
            fadeEffAttack(0);
            procSkillEnd(-1);
            return;
        }
        updateSpotLight(m_hEffAttack[1]);
        break;
    case 840:
        setPlayerFlag(0x400000, 0);
        if (frame < m_pAtkData->MOVE_END_FRAME) {
            procTargetMark();
            procAttackMove(frame, 0.0f, 0.0f, 2, 0.15f);
        }
        if (isMotionEnd()) {
            m_iSubState = 841;
            setAnimSkillEx(1, -1, 1, 0, 0);
            m_pMarker->holdTarget(0);
            setTimer(m_pAtkData->COLL_START_FRAME * 100);
            m_szEffectName = g_EffTbl_Skill[m_pAtkData->EFFECT][1].pEffectName;
        }
        break;
    case 841:
    case 842:
    case 843:
    case 844: {
        setPlayerFlag(0x400000, 0);
        if (getTimer() > 0) break;
        procTargetMark();
        procAttackMove(frame, 0.0f, 0.0f, 2, 0.15f);
        if (m_iSubState == 841 || !m_hTarget) {
            m_vAppearPos.x = m_vCenterPos.x;
            m_vAppearPos.z = m_vCenterPos.z;
        } else {
            m_vAppearPos.x = m_vMarkPos.x;
            m_vAppearPos.z = m_vMarkPos.z;
        }
        m_vAppearPos.y = getPos().y;
        FLOAT h = getAirHeight(&m_vAppearPos, 10.0f, 0);
        if (h < 10.0f) {
            m_vAppearPos.y -= h - 0.0035f;
        }
        setEffect(g_EffTbl_Skill[m_pAtkData->EFFECT][1].pEffectName, 0, &m_vAppearPos, NULL, 0.0f);
        if (m_iSubState == 844) {
            setEffect(g_EffTbl_Skill[m_pAtkData->EFFECT][1].pEffectName, 5, &m_vAppearPos, NULL, 0.0f);
        }
        setMagic(0x348, 0, m_iStockSkill);
        m_iSubState++;
        if (m_iSubState != 845) {
            setTimer(m_pAtkData->TRG_FRAME * 100);
        } else {
            setAnimSkillEx(2, -1, 1, 0, 0);
            setTimer(3000);
        }
        break;
    }
    case 845:
        setPlayerFlag(0x400000, 0);
        if (isMotionEnd()) {
            setAnim(0x2A, -1, 0);
        }
        if (getTimer() > 0) break;
        if (CEffectMan::getEffectNameNum(m_szEffectName)) break;
        procSkillEnd(-1);
        return;
    case 846:
        setGroundIK(1);
        if (isPlayerFlag(0x800000)) {
            setPlayerFlag(0, 0x2000000);
            if (!isAir()) {
                if (m_iNowAnimID != 0x2A) {
                    if (m_iNowAnimID != 7) {
                        setAnim(7, -1, 0);
                    } else if (isMotionEnd()) {
                        setAnim(0x2A, -1, 0);
                    }
                }
            }
        }
        if (isMotionEnd()) {
            procSkillEnd(-1);
            return;
        }
        if (getTimer() > 0) break;
        procSkillEnd(-1);
        return;
    case 847:
        if (m_iStockSkill == 0x4A) {
            if (!CRcf::isCompleteLoadRCF(0, 0)) break;
            m_bNextTrg = FALSE;
            if (m_pPLM->isCompleteLoad(0x44) != 1) {
                m_pPLM->setReqLoad(0x44, 0);
                m_bNextTrg = TRUE;
            }
            if (m_pPLM->isCompleteLoad(0x47) != 1) {
                m_pPLM->setReqLoad(0x47, 0);
                m_bNextTrg = TRUE;
            }
            if (m_pPLM->isCompleteLoad(0x4A) != 1) {
                m_pPLM->setReqLoad(0x4A, 0);
                m_bNextTrg = TRUE;
            }
            if (m_bNextTrg) break;
            if (!m_pPLM->isFriendReady()) {
                m_pPLM->createFriend(m_pPLM->getFriendID_toSkill(m_iStockSkill), this);
                break;
            }
            if (!m_pPLM->isFriendExReady()) break;
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
        if (m_pPLM->isFriendExReady()) {
            m_pPLM->getFriendEx()->setAppear();
        }
        fadeEffect(m_hEffCast, 1);
        setEffect(g_szDpxFriend, 3, &getPos(), NULL, 0.0f);
        m_iSubState = m_uiAttackID * 10;
        setAnimForce(0x2a, -1, 0, 0, 0, 0);
        setPlayerFlag(0x100000, 0);
        m_tqStateTimer = 0;
        m_pMarker->holdTarget(8);
        procTargetMark();
        break;
    case 940:
    case 950:
    case 1000:
    case 1010:
    case 1050:
    case 1060:
    case 1070:
    case 1080:
    case 1200:
    case 1210:
        if (!m_pPLM->isFriendReady()) {
            m_iSubState = 1211;
            break;
        }
        if (!m_bIsCanCombo) {
            m_bIsCanCombo = m_pPLM->getFriend()->setSkill(m_uiAttackID);
        }
        exeStandUp_inFriend();
        break;
    case 1090:
    case 1100:
    case 1110:
    case 1120:
        if (!m_pPLM->isFriendReady()) {
            m_iSubState = 1211;
            break;
        }
        if (!m_bIsCanCombo) {
            m_bIsCanCombo = m_pPLM->getFriend()->setSkill(m_uiAttackID);
        }
        watchTarget(0);
        break;
    case 960:
    case 1020:
    case 1130:
    case 1140:
        if (!m_pPLM->isFriendReady()) {
            m_iSubState = 1211;
            setAnim(0x2A, -1, 0);
            break;
        }
        if (!m_bIsCanCombo) {
            m_bIsCanCombo = m_pPLM->getFriend()->setSkill(m_uiAttackID);
        }
        if (isPlayerFlag(0x20000)) {
            if (exeAttack_inFriend()) {
                setPlayerFlag(0, 0x20000);
            }
            break;
        }
        offAttack();
        watchTarget(0);
        procGravity(frame, 0, 0);
        return;
    case 1040:
        setPlayerFlag(0x400000, 0);
        if (!m_pPLM->isFriendReady() && !m_pPLM->isFriendExReady()) {
            m_iSubState = 1211;
            break;
        }
        if (!m_bIsCanCombo) {
            m_bIsCanCombo = m_pPLM->getFriend()->setSkill(m_uiAttackID);
        }
        if (!m_bNextTrg) {
            m_bNextTrg = m_pPLM->getFriendEx()->setSkill(m_uiAttackID);
        }
        if (m_bIsCanCombo && m_bNextTrg) {
            m_iSubState = 1041;
            offWeapon();
            setAnimSkillEx(0, 0, 0, 0, 0);
            setConditionFlag(0x80000, 0);
        }
        break;
    case 1041:
        setPlayerFlag(0x400000, 0);
        if (!m_pPLM->isFriendExReady()) {
            m_iSubState = 1211;
            break;
        }
        if (isMotionEnd()) {
            m_iSubState = 1042;
            setAnimSkillEx(1, 0, 0, 0, 0);
            setTimer(15000);
        }
        break;
    case 1042:
        if (!m_pPLM->isFriendExReady()) {
            m_iSubState = 1211;
            break;
        }
        if (m_fAnalogPower >= 0.3f) {
            FLOAT r = _subAngle(getDir(), m_fAnalogViewDir);
            FLOAT d0 = Movement::Homing(getDir(), m_fAnalogViewDir, m_pAtkData->WORK1 * 0.01f, m_fFrameRate);
            FVECTOR v;
            {
                /* the original's brace-initialized temporary ({0, dir, 0, 0}, cleared as one quadword) */
                FVECTOR t;
                t._q = 0;
                t.y = d0;
                v = t;
            }
            setDir(d0);
            m_pMove->setRot(v);
        }
        m_fWalkSpeed = m_pAtkData->MAX_SPEED / 60.0f;
        m_fWalkSpeed = m_fJumpSpdXZ = Movement::Approach(m_fJumpSpdXZ, m_fWalkSpeed, m_pAtkData->WORK0 * 0.01f, m_fFrameRate);
        if (getTimer() <= 0 || isBattleFlag(4)) {
            m_iSubState = 1043;
            setAnimSkillEx(2, -1, 1, 0, 0);
            setConditionFlag(0, 0x80000);
            m_fWalkSpeed = m_fJumpSpdXZ = 0.0f;
            m_fWallRadius = 0.5f;
        }
        m_pMove->calcVelocityXZ(m_fWalkSpeed);
        break;
    case 1043:
        if (isMotionEnd()) {
            setAnim(0x2A, -1, 0);
            onWeapon(0);
        }
        if (m_pPLM->isFriendExReady()) break;
        onWeapon(0);
        procSkillEnd(-1);
        break;
    case 1211:
        procSkillEnd(-1);
        return;
    case 1212:
        if (m_pPLM->isCompleteLoad(m_iStockSkill) != 1) break;
        if (!m_pPLM->isSummonReady()) {
            calcAppearPosFS_toAttackID();
            m_pPLM->createSummon(m_pPLM->getSummonID_toSkill(m_iStockSkill), NULL);
            break;
        }
        if (!isMotionLoop()) break;
        setObjStop(0);
        m_pCam->changeCamMode(-1, 0);
        m_pPLM->getSummon()->setAppear();
        fadeEffect(m_hEffCast, 1);
        {
            CEffect* effect = CEffectMan::getEffect(m_hEffCast);
            if (effect) {
                effect->setZSortEntity(m_pPLM->getSummon(), 1, 1);
            }
        }
        switch (m_uiAttackID) {
        case 0x7A:
        case 0x7B:
        case 0x7D:
        case 0x80:
        case 0x81:
        case 0x82:
        case 0x83:
        case 0x84:
        case 0x8A:
        case 0x8B:
        case 0x91:
        case 0x92:
        case 0x93:
            break;
        default:
            setEffect(g_szDpxSummon, 2, &getPos(), NULL, 0.0f);
            break;
        }
        m_iSubState = m_uiAttackID * 10;
        setAnimForce(0x2a, -1, 0, 0, 0, 0);
        setPlayerFlag(0x100000, 0);
        m_tqStateTimer = 0;
        m_pMarker->holdTarget(8);
        procTargetMark();
        break;
    case 1360:
    case 1370:
        if (!m_pPLM->isSummonReady() || isPlayerFlag(0x8000000)) {
            m_iSubState = 1471;
            setAnim(0x2A, -1, 0);
            break;
        }
        if (!m_bIsCanCombo) {
            m_bIsCanCombo = m_pPLM->getSummon()->setSkill(m_uiAttackID);
        } else {
            watchTarget(2);
        }
        break;
    case 1471:
        if (m_pPLM->isSummonReady()) return;
        m_iSubState = 1473;
        setPlayerFlag(0, 0x8000000);
        m_fWallRadius = 0.5f;
        return;
    case 1330:
    case 1340:
    case 1350:
    case 1430:
    case 1440:
        if (!m_pPLM->isSummonReady()) {
            m_iSubState = 1473;
            break;
        }
        if (!m_bIsCanCombo) {
            m_bIsCanCombo = m_pPLM->getSummon()->setSkill(m_uiAttackID);
        }
        exeStandUp_inFriend();
        break;
    case 1290:
    case 1300:
    case 1310:
    case 1320:
    case 1380:
    case 1390:
    case 1450:
    case 1460:
    case 1470:
        if (!m_pPLM->isSummonReady() || isPlayerFlag(0x8000000)) {
            setPlayerFlag(0x400000, 0x1000);
            m_iSubState = 1472;
            if (isAir()) {
                setLand(8.0f, 0.5f, 1);
                updateCoord();
                coordFix();
            }
            exeWarp(0);
            setCastEffect(g_szDpxSummon, 2, &Crd(0), NULL);
            setPlayerFlag(0, 0x8000000);
            switch (m_uiAttackID) {
            case 0x80:
            case 0x91:
            case 0x92:
            case 0x93:
                setEffect(g_szDpxCloud, 0, &getPos(), NULL, 0.0f);
                break;
            }
            break;
        }
        setPlayerFlag(0x400000, 0);
        if (!exeWarp(1)) break;
        if (!m_bIsCanCombo) {
            m_bIsCanCombo = m_pPLM->getSummon()->setSkill(m_uiAttackID);
            m_pCam->changeNowPlayer(m_pPLM->getSummon()->getHandle(), m_hTarget);
            m_pCam->changeCamMode(-1, 0);
            setPlayerFlag(0x1001, 0);
            setEntityFlag(1, 0);
            offWeapon();
        }
        warp(m_pPLM->getSummon()->getPos());
        setRot(m_pPLM->getSummon()->getRot());
        clearSpeed();
        break;
    case 1472:
        setPlayerFlag(0x400000, 0);
        onWeapon(0);
        if (!exeWarp(0)) break;
        if (m_pPLM->isSummonReady()) return;
        setPlayerFlag(0, 0x1001);
        m_iSubState = 1473;
        setPlayerFlag(0, 0x8000000);
        m_fWallRadius = 0.5f;
        return;
    case 1473:
        procSkillEnd(-1);
        return;
    case 1500:
        setPlayerFlag(0x400000, 0);
        procTargetMark();
        procAttackMove(frame, 0.0f, 0.0f, 2, 0.15f);
        if (frame < m_pAtkData->TRG_FRAME) break;
        m_iSubState = 1501;
        m_hMagicCore = MG_SKILL::createMGSkill(2, 0);
        setTimer(m_pAtkData->COLL_START_FRAME * 50);
        break;
    case 1501:
        setPlayerFlag(0x400000, 0);
        if (!m_bNextTrg) {
            if (m_iNowAnimID == 0x2E) {
                if (getTimer() <= 0) {
                    setAnimForce(0x39, -1, 1, 4, 5, 0);
                }
            } else if (isMotionEnd()) {
                m_bNextTrg = TRUE;
            }
        } else {
            exeStandUp_inFriend();
        }
        if (MG_SKILL::isMGSkillAlive() && m_hMagicCore > 0) break;
        m_iSubState = 1502;
        clearSpeed();
        setTimer(800);
        break;
    case 332:
    case 1502:
        if (getTimer() > 0) break;
        procSkillEnd(-1);
        return;
    case 1490:
    case 1491:
        m_fJumpSpdY = 0.0f;
        if (frame < m_pAtkData->TRG_FRAME) break;
        m_iSubState = 1492;
        m_hMagicCore = MG_SKILL::createMGSkill(1, m_iStockSkill - 0x57);
        break;
    case 1492:
        if (!m_bNextTrg) {
            if (isMotionEnd()) {
                m_bNextTrg = TRUE;
            }
        } else {
            exeStandUp_inFriend();
        }
        if (MG_SKILL::isMGSkillAlive() && m_hMagicCore > 0) break;
        m_iSubState = 1502;
        setTimer(800);
        clearSpeed();
        break;
    }
    if (!isConditionFlag(0x1000000)) {
        procGravity(frame, m_pAtkData->MOVE_END_FRAME, 1);
    }
}
#else
/* placeholder: .rodata of exeStockSkill__Q22PL11CPlayerSora (strings, jump tables) */
__asm__(".rdata\n.align 3\n.byte 0x65,0x78,0x65,0x53,0x74,0x6f,0x63,0x6b,0x53,0x6b,0x69,0x6c,0x6c,0x3a,0x20,0x20\n.byte 0x46,0x49,0x4e,0x41,0x4c,0x42,0x52,0x45,0x41,0x4b,0x20,0x20,0x45,0x6e,0x74,0x72\n.byte 0x79,0x20,0x4e,0x47,0x2e,0x2e,0x2e,0x0a,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00\n.align 3\n.byte 0x72,0x65,0x63,0x6f,0x6d,0x00,0x00,0x00\n.align 3\n.byte 0x43,0x50,0x6c,0x61,0x79,0x65,0x72,0x53,0x6f,0x72,0x61,0x3a,0x3a,0x20,0x53,0x53\n.byte 0x5f,0x52,0x45,0x46,0x52,0x45,0x43,0x54,0x52,0x41,0x49,0x44,0x20,0x3a,0x20,0x42\n.byte 0x75,0x6c,0x6c,0x65,0x74,0x3d,0x25,0x64,0x2f,0x25,0x64,0x2c,0x0a,0x00,0x00,0x00\n.align 3\n.byte 0x65,0x78,0x65,0x53,0x74,0x6f,0x63,0x6b,0x53,0x6b,0x69,0x6c,0x6c,0x3a,0x20,0x20\n.byte 0x53,0x4f,0x4e,0x49,0x43,0x52,0x41,0x56,0x45,0x20,0x20,0x45,0x6e,0x74,0x72,0x79\n.byte 0x20,0x4e,0x47,0x2e,0x2e,0x2e,0x0a,0x00\n.align 3\n");
__asm__(".byte 0x65,0x78,0x65,0x53,0x74,0x6f,0x63,0x6b,0x53,0x6b,0x69,0x6c,0x6c,0x3a,0x20,0x20\n.byte 0x4c,0x41,0x53,0x54,0x41,0x52,0x43,0x41,0x4e,0x55,0x4d,0x20,0x20,0x45,0x6e,0x74\n.byte 0x72,0x79,0x20,0x4e,0x47,0x2e,0x2e,0x2e,0x0a,0x00,0x00,0x00,0x00,0x00,0x00,0x00\n.align 3\n.byte 0x65,0x78,0x65,0x53,0x74,0x6f,0x63,0x6b,0x53,0x6b,0x69,0x6c,0x6c,0x3a,0x20,0x20\n.byte 0x4c,0x41,0x53,0x54,0x41,0x52,0x43,0x41,0x4e,0x55,0x4d,0x20,0x20,0x46,0x69,0x6e\n.byte 0x69,0x73,0x68,0x20,0x4e,0x47,0x2e,0x2e,0x2e,0x0a,0x00,0x00,0x00,0x00,0x00,0x00\n.align 3\n.byte 0x6c,0x69,0x6d,0x69,0x74,0x34,0x00,0x00\n.align 3\n.byte 0x6d,0x61,0x67,0x6e,0x65,0x73,0x70,0x69,0x72,0x61,0x6c,0x00,0x00,0x00,0x00,0x00\n.align 3\n.byte 0x69,0x74,0x65,0x6d,0x70,0x6c,0x75,0x73,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00\n.align 4\n.word .L00295CAC\n.word .L0029C9CC\n");
__asm__(".word .L002963E8\n.word .L00296400\n.word .L0029C9CC\n.word .L0029C9CC\n.word .L0029C9CC\n.word .L0029C9CC\n.word .L0029C9CC\n.word .L0029C9CC\n.word .L0029C9CC\n.word .L0029C9CC\n.word .L0029C9CC\n.word .L0029C9CC\n.word .L0029C9CC\n.word .L0029C9CC\n.word .L0029C9CC\n.word .L0029C9CC\n");
__asm__(".word .L0029C9CC\n.word .L0029C9CC\n.word .L0029C9CC\n.word .L0029C9CC\n.word .L0029C9CC\n.word .L0029C9CC\n.word .L0029C9CC\n.word .L0029C9CC\n.word .L0029C9CC\n.word .L0029C9CC\n.word .L0029C9CC\n.word .L0029C9CC\n.word .L0029C9CC\n.word .L0029C9CC\n.word .L0029C9CC\n.word .L0029C9CC\n");
__asm__(".word .L0029C9CC\n.word .L0029C9CC\n.word .L0029C9CC\n.word .L0029C9CC\n.word .L0029C9CC\n.word .L0029C9CC\n.word .L0029C9CC\n.word .L0029C9CC\n.word .L0029C9CC\n.word .L0029C9CC\n.word .L0029C9CC\n.word .L0029C9CC\n.word .L0029C9CC\n.word .L0029C9CC\n.word .L0029C9CC\n.word .L0029C9CC\n");
__asm__(".word .L0029C9CC\n.word .L0029C9CC\n.word .L0029C9CC\n.word .L0029C9CC\n.word .L0029C9CC\n.word .L0029C9CC\n.word .L0029C9CC\n.word .L0029C9CC\n.word .L0029C9CC\n.word .L0029C9CC\n.word .L0029C9CC\n.word .L0029C9CC\n.word .L0029C9CC\n.word .L0029C9CC\n.word .L0029C9CC\n.word .L0029C9CC\n");
__asm__(".word .L0029C9CC\n.word .L0029C9CC\n.word .L0029C9CC\n.word .L0029C9CC\n.word .L0029C9CC\n.word .L0029C9CC\n.word .L0029C9CC\n.word .L0029C9CC\n.word .L0029C9CC\n.word .L0029C9CC\n.word .L0029C9CC\n.word .L0029C9CC\n.word .L0029C9CC\n.word .L00296590\n.word .L0029C9CC\n.word .L0029C9CC\n");
__asm__(".word .L0029C9CC\n.word .L0029C9CC\n.word .L0029C9CC\n.word .L0029C9CC\n.word .L0029C9CC\n.word .L0029C9CC\n.word .L0029C9CC\n.word .L0029661C\n.word .L00296640\n.word .L00296908\n.word .L0029C9CC\n.word .L0029C9CC\n.word .L0029C9CC\n.word .L0029C9CC\n.word .L0029C9CC\n.word .L0029C9CC\n");
__asm__(".word .L0029C9CC\n.word .L0029C9CC\n.word .L0029C9CC\n.word .L0029C9CC\n.word .L0029C9CC\n.word .L0029C9CC\n.word .L0029C9CC\n.word .L0029C9CC\n.word .L0029C9CC\n.word .L0029C9CC\n.word .L0029C9CC\n.word .L00296A48\n.word .L0029C9CC\n.word .L0029C9CC\n.word .L0029C9CC\n.word .L0029C9CC\n");
__asm__(".word .L0029C9CC\n.word .L0029C9CC\n.word .L0029C9CC\n.word .L0029C9CC\n.word .L0029C9CC\n.word .L00296C04\n.word .L0029C9CC\n.word .L0029C9CC\n.word .L0029C9CC\n.word .L0029C9CC\n.word .L0029C9CC\n.word .L0029C9CC\n.word .L0029C9CC\n.word .L0029C9CC\n.word .L0029C9CC\n.word .L00296CC4\n");
__asm__(".word .L00296D30\n.word .L00297050\n.word .L0029C9CC\n.word .L0029C9CC\n.word .L0029C9CC\n.word .L0029C9CC\n.word .L0029C9CC\n.word .L0029C9CC\n.word .L0029C9CC\n.word .L00297170\n.word .L002972B4\n.word .L002972B4\n.word .L002972B4\n.word .L002972B4\n.word .L002972B4\n.word .L002972B4\n");
__asm__(".word .L002975DC\n.word .L0029C9CC\n.word .L0029C9CC\n.word .L0029C9CC\n.word .L0029C9CC\n.word .L0029C9CC\n.word .L0029C9CC\n.word .L0029C9CC\n.word .L0029C9CC\n.word .L0029C9CC\n.word .L0029C9CC\n.word .L0029C9CC\n.word .L0029C9CC\n.word .L0029C9CC\n.word .L0029C9CC\n.word .L0029C9CC\n");
__asm__(".word .L0029C9CC\n.word .L0029C9CC\n.word .L0029C9CC\n.word .L0029C9CC\n.word .L0029C9CC\n.word .L0029C9CC\n.word .L0029C9CC\n.word .L00297700\n.word .L00297844\n.word .L00297844\n.word .L00297844\n.word .L00297844\n.word .L00297844\n.word .L00297844\n.word .L00297844\n.word .L002979E4\n");
__asm__(".word .L002979E4\n.word .L002979E4\n.word .L002979E4\n.word .L002979E4\n.word .L00297C8C\n.word .L0029C9CC\n.word .L0029C9CC\n.word .L0029C9CC\n.word .L0029C9CC\n.word .L0029C9CC\n.word .L0029C9CC\n.word .L0029C9CC\n.word .L0029C9CC\n.word .L0029C9CC\n.word .L0029C9CC\n.word .L0029C9CC\n");
__asm__(".word .L0029C9CC\n.word .L0029C9CC\n.word .L0029C9CC\n.word .L0029C9CC\n.word .L0029C9CC\n.word .L0029C9CC\n.word .L0029C9CC\n.word .L0029C9CC\n.word .L0029C9CC\n.word .L0029C9CC\n.word .L0029C9CC\n.word .L0029C9CC\n.word .L0029C9CC\n.word .L0029C9CC\n.word .L0029C9CC\n.word .L0029C9CC\n");
__asm__(".word .L0029C9CC\n.word .L0029C9CC\n.word .L0029C9CC\n.word .L0029C9CC\n.word .L0029C9CC\n.word .L0029C9CC\n.word .L0029C9CC\n.word .L0029C9CC\n.word .L0029C9CC\n.word .L0029C9CC\n.word .L0029C9CC\n.word .L0029C9CC\n.word .L0029C9CC\n.word .L0029C9CC\n.word .L0029C9CC\n.word .L0029C9CC\n");
__asm__(".word .L0029C9CC\n.word .L0029C9CC\n.word .L0029C9CC\n.word .L0029C9CC\n.word .L0029C9CC\n.word .L0029C9CC\n.word .L0029C9CC\n.word .L0029C9CC\n.word .L0029C9CC\n.word .L0029C9CC\n.word .L0029C9CC\n.word .L0029C9CC\n.word .L0029C9CC\n.word .L0029C9CC\n.word .L0029C9CC\n.word .L0029C9CC\n");
__asm__(".word .L0029C9CC\n.word .L0029C9CC\n.word .L0029C9CC\n.word .L0029C9CC\n.word .L0029C9CC\n.word .L0029C9CC\n.word .L0029C9CC\n.word .L0029C9CC\n.word .L0029C9CC\n.word .L0029C9CC\n.word .L0029C9CC\n.word .L0029C9CC\n.word .L0029C9CC\n.word .L0029C9CC\n.word .L0029C9CC\n.word .L0029C9CC\n");
__asm__(".word .L0029C9CC\n.word .L0029C9CC\n.word .L0029C9CC\n.word .L0029C9CC\n.word .L0029C9CC\n.word .L0029C9CC\n.word .L0029C9CC\n.word .L0029C9CC\n.word .L0029C9CC\n.word .L0029C9CC\n.word .L0029C9CC\n.word .L0029C9CC\n.word .L0029C9CC\n.word .L0029C9CC\n.word .L0029C9CC\n.word .L0029C9CC\n");
__asm__(".word .L0029C9CC\n.word .L0029C9CC\n.word .L0029C9CC\n.word .L0029C9CC\n.word .L0029C9CC\n.word .L0029C9CC\n.word .L0029C9CC\n.word .L0029C9CC\n.word .L0029C9CC\n.word .L0029C9CC\n.word .L0029C9CC\n.word .L0029C9CC\n.word .L0029C9CC\n.word .L0029C9CC\n.word .L0029C9CC\n.word .L0029C9CC\n");
__asm__(".word .L0029C9CC\n.word .L0029C9CC\n.word .L0029C9CC\n.word .L0029C9CC\n.word .L0029C9CC\n.word .L0029C9CC\n.word .L0029C9CC\n.word .L0029C9CC\n.word .L0029C9CC\n.word .L0029C9CC\n.word .L0029C9CC\n.word .L0029C9CC\n.word .L0029C9CC\n.word .L0029C9CC\n.word .L0029C9CC\n.word .L0029C9CC\n");
__asm__(".word .L0029C9CC\n.word .L0029C9CC\n.word .L0029C9CC\n.word .L00297D24\n.word .L00297E8C\n.word .L00298488\n.word .L00298488\n.word .L002984F4\n.word .L00298610\n.word .L0029C9CC\n.word .L0029C9CC\n.word .L0029C9CC\n.word .L0029C9CC\n.word .L0029C9CC\n.word .L0029C9CC\n.word .L0029C9CC\n");
__asm__(".word .L0029C9CC\n.word .L0029C9CC\n.word .L0029C9CC\n.word .L0029C9CC\n.word .L0029C9CC\n.word .L0029C9CC\n.word .L0029C9CC\n.word .L0029869C\n.word .L0029879C\n.word .L0029C90C\n.word .L0029C9CC\n.word .L0029C9CC\n.word .L0029C9CC\n.word .L0029C9CC\n.word .L0029C9CC\n.word .L0029C9CC\n");
__asm__(".word .L0029C9CC\n.word .L0029C9CC\n.word .L0029C9CC\n.word .L0029C9CC\n.word .L0029C9CC\n.word .L0029C9CC\n.word .L0029C9CC\n.word .L0029C9CC\n.word .L0029C9CC\n.word .L0029C9CC\n.word .L0029C9CC\n.word .L0029C9CC\n.word .L0029C9CC\n.word .L0029C9CC\n.word .L0029C9CC\n.word .L0029C9CC\n");
__asm__(".word .L0029C9CC\n.word .L0029C9CC\n.word .L0029C9CC\n.word .L0029C9CC\n.word .L0029C9CC\n.word .L0029C9CC\n.word .L0029C9CC\n.word .L0029C9CC\n.word .L0029C9CC\n.word .L0029C9CC\n.word .L0029C9CC\n.word .L0029C9CC\n.word .L0029C9CC\n.word .L0029C9CC\n.word .L0029C9CC\n.word .L0029C9CC\n");
__asm__(".word .L0029C9CC\n.word .L0029C9CC\n.word .L0029C9CC\n.word .L0029C9CC\n.word .L0029C9CC\n.word .L0029C9CC\n.word .L0029C9CC\n.word .L0029C9CC\n.word .L0029C9CC\n.word .L0029C9CC\n.word .L0029C9CC\n.word .L0029C9CC\n.word .L0029C9CC\n.word .L0029C9CC\n.word .L0029C9CC\n.word .L0029C9CC\n");
__asm__(".word .L0029C9CC\n.word .L0029C9CC\n.word .L0029C9CC\n.word .L0029C9CC\n.word .L0029C9CC\n.word .L0029C9CC\n.word .L0029C9CC\n.word .L0029C9CC\n.word .L0029C9CC\n.word .L0029C9CC\n.word .L0029C9CC\n.word .L0029C9CC\n.word .L0029C9CC\n.word .L0029C9CC\n.word .L0029C9CC\n.word .L0029C9CC\n");
__asm__(".word .L0029C9CC\n.word .L0029C9CC\n.word .L0029C9CC\n.word .L0029C9CC\n.word .L0029C9CC\n.word .L0029C9CC\n.word .L0029C9CC\n.word .L002987F8\n.word .L0029C9CC\n.word .L0029C9CC\n.word .L0029C9CC\n.word .L0029C9CC\n.word .L0029C9CC\n.word .L0029C9CC\n.word .L0029C9CC\n.word .L0029C9CC\n");
__asm__(".word .L0029C9CC\n.word .L002987F8\n.word .L0029C9CC\n.word .L0029C9CC\n.word .L0029C9CC\n.word .L0029C9CC\n.word .L0029C9CC\n.word .L0029C9CC\n.word .L0029C9CC\n.word .L0029C9CC\n.word .L0029C9CC\n.word .L00298894\n.word .L0029C9CC\n.word .L0029C9CC\n.word .L0029C9CC\n.word .L0029C9CC\n");
__asm__(".word .L0029C9CC\n.word .L0029C9CC\n.word .L0029C9CC\n.word .L0029C9CC\n.word .L0029C9CC\n.word .L00298894\n.word .L0029C9CC\n.word .L0029C9CC\n.word .L0029C9CC\n.word .L0029C9CC\n.word .L0029C9CC\n.word .L0029C9CC\n.word .L0029C9CC\n.word .L0029C9CC\n.word .L0029C9CC\n.word .L00298894\n");
__asm__(".word .L0029C9CC\n.word .L0029C9CC\n.word .L0029C9CC\n.word .L0029C9CC\n.word .L0029C9CC\n.word .L0029C9CC\n.word .L0029C9CC\n.word .L0029C9CC\n.word .L0029C9CC\n.word .L002988FC\n.word .L0029C9CC\n.word .L0029C9CC\n.word .L0029C9CC\n.word .L0029C9CC\n.word .L0029C9CC\n.word .L0029C9CC\n");
__asm__(".word .L0029C9CC\n.word .L0029C9CC\n.word .L0029C9CC\n.word .L002988FC\n.word .L0029C9CC\n.word .L0029C9CC\n.word .L0029C9CC\n.word .L0029C9CC\n.word .L0029C9CC\n.word .L0029C9CC\n.word .L0029C9CC\n.word .L0029C9CC\n.word .L0029C9CC\n.word .L00296CC4\n.word .L0029C9CC\n.word .L0029C9CC\n");
__asm__(".word .L0029C9CC\n.word .L0029C9CC\n.word .L0029C9CC\n.word .L0029C9CC\n.word .L0029C9CC\n.word .L0029C9CC\n.word .L0029C9CC\n.word .L00296CC4\n.word .L0029C9CC\n.word .L0029C9CC\n.word .L0029C9CC\n.word .L0029C9CC\n.word .L0029C9CC\n.word .L0029C9CC\n.word .L0029C9CC\n.word .L0029C9CC\n");
__asm__(".word .L0029C9CC\n.word .L00296CC4\n.word .L0029C9CC\n.word .L0029C9CC\n.word .L0029C9CC\n.word .L0029C9CC\n.word .L0029C9CC\n.word .L0029C9CC\n.word .L0029C9CC\n.word .L0029C9CC\n.word .L0029C9CC\n.word .L00296CC4\n.word .L002970B8\n.word .L00297050\n.word .L0029C9CC\n.word .L0029C9CC\n");
__asm__(".word .L0029C9CC\n.word .L0029C9CC\n.word .L0029C9CC\n.word .L0029C9CC\n.word .L0029C9CC\n.word .L00296CC4\n.word .L00297068\n.word .L00297050\n.word .L0029C9CC\n.word .L0029C9CC\n.word .L0029C9CC\n.word .L0029C9CC\n.word .L0029C9CC\n.word .L0029C9CC\n.word .L0029C9CC\n.word .L0029C9CC\n");
__asm__(".word .L0029C9CC\n.word .L0029C9CC\n.word .L0029C9CC\n.word .L0029C9CC\n.word .L0029C9CC\n.word .L0029C9CC\n.word .L0029C9CC\n.word .L0029C9CC\n.word .L0029C9CC\n.word .L00298A70\n.word .L0029C9CC\n.word .L0029C9CC\n.word .L0029C9CC\n.word .L0029C9CC\n.word .L0029C9CC\n.word .L0029C9CC\n");
__asm__(".word .L0029C9CC\n.word .L0029C9CC\n.word .L0029C9CC\n.word .L00298A70\n.word .L0029C9CC\n.word .L0029C9CC\n.word .L0029C9CC\n.word .L0029C9CC\n.word .L0029C9CC\n.word .L0029C9CC\n.word .L0029C9CC\n.word .L0029C9CC\n.word .L0029C9CC\n.word .L00298AF8\n.word .L0029C9CC\n.word .L0029C9CC\n");
__asm__(".word .L0029C9CC\n.word .L0029C9CC\n.word .L0029C9CC\n.word .L0029C9CC\n.word .L0029C9CC\n.word .L0029C9CC\n.word .L0029C9CC\n.word .L00298AF8\n.word .L0029C9CC\n.word .L0029C9CC\n.word .L0029C9CC\n.word .L0029C9CC\n.word .L0029C9CC\n.word .L0029C9CC\n.word .L0029C9CC\n.word .L0029C9CC\n");
__asm__(".word .L0029C9CC\n.word .L00298C80\n.word .L0029C9CC\n.word .L0029C9CC\n.word .L0029C9CC\n.word .L0029C9CC\n.word .L0029C9CC\n.word .L0029C9CC\n.word .L0029C9CC\n.word .L0029C9CC\n.word .L0029C9CC\n.word .L00298C80\n.word .L0029C9CC\n.word .L0029C9CC\n.word .L0029C9CC\n.word .L0029C9CC\n");
__asm__(".word .L0029C9CC\n.word .L0029C9CC\n.word .L0029C9CC\n.word .L0029C9CC\n.word .L0029C9CC\n.word .L00298D50\n.word .L0029C9CC\n.word .L0029C9CC\n.word .L0029C9CC\n.word .L0029C9CC\n.word .L0029C9CC\n.word .L0029C9CC\n.word .L0029C9CC\n.word .L0029C9CC\n.word .L0029C9CC\n.word .L00298D50\n");
__asm__(".word .L0029C9CC\n.word .L0029C9CC\n.word .L0029C9CC\n.word .L0029C9CC\n.word .L0029C9CC\n.word .L0029C9CC\n.word .L0029C9CC\n.word .L0029C9CC\n.word .L0029C9CC\n.word .L00298E04\n.word .L0029C9CC\n.word .L0029C9CC\n.word .L0029C9CC\n.word .L0029C9CC\n.word .L0029C9CC\n.word .L0029C9CC\n");
__asm__(".word .L0029C9CC\n.word .L0029C9CC\n.word .L0029C9CC\n.word .L00298E5C\n.word .L0029C9CC\n.word .L0029C9CC\n.word .L0029C9CC\n.word .L0029C9CC\n.word .L0029C9CC\n.word .L0029C9CC\n.word .L0029C9CC\n.word .L0029C9CC\n.word .L0029C9CC\n.word .L00298ED4\n.word .L00298F58\n.word .L0029C9CC\n");
__asm__(".word .L0029C9CC\n.word .L0029C9CC\n.word .L0029C9CC\n.word .L0029C9CC\n.word .L0029C9CC\n.word .L0029C9CC\n.word .L0029C9CC\n.word .L00299024\n.word .L002990BC\n.word .L0029C9CC\n.word .L0029C9CC\n.word .L0029C9CC\n.word .L0029C9CC\n.word .L0029C9CC\n.word .L0029C9CC\n.word .L0029C9CC\n");
__asm__(".word .L0029C9CC\n.word .L00299188\n.word .L00299220\n.word .L0029C9CC\n.word .L0029C9CC\n.word .L0029C9CC\n.word .L0029C9CC\n.word .L0029C9CC\n.word .L0029C9CC\n.word .L0029C9CC\n.word .L0029C9CC\n.word .L002992EC\n.word .L00299374\n.word .L00299420\n.word .L0029C9CC\n.word .L0029C9CC\n");
__asm__(".word .L0029C9CC\n.word .L0029C9CC\n.word .L0029C9CC\n.word .L0029C9CC\n.word .L0029C9CC\n.word .L00299464\n.word .L00299544\n.word .L002998BC\n.word .L0029C9CC\n.word .L0029C9CC\n.word .L0029C9CC\n.word .L0029C9CC\n.word .L0029C9CC\n.word .L0029C9CC\n.word .L0029C9CC\n.word .L002999A0\n");
__asm__(".word .L00299B00\n.word .L00299C64\n.word .L00299C9C\n.word .L00299D18\n.word .L00299DA4\n.word .L0029C9CC\n.word .L0029C9CC\n.word .L0029C9CC\n.word .L0029C9CC\n.word .L0029C9CC\n.word .L0029C9CC\n.word .L0029C9CC\n.word .L0029C9CC\n.word .L0029C9CC\n.word .L0029C9CC\n.word .L0029C9CC\n");
__asm__(".word .L0029C9CC\n.word .L0029C9CC\n.word .L0029C9CC\n.word .L00299DE8\n.word .L00299EA0\n.word .L0029A0F4\n.word .L0029C9CC\n.word .L0029C9CC\n.word .L0029C9CC\n.word .L0029C9CC\n.word .L0029C9CC\n.word .L0029C9CC\n.word .L0029C9CC\n.word .L0029A10C\n.word .L0029A160\n.word .L0029A1EC\n");
__asm__(".word .L0029C9CC\n.word .L0029C9CC\n.word .L0029C9CC\n.word .L0029C9CC\n.word .L0029C9CC\n.word .L0029C9CC\n.word .L0029C9CC\n.word .L0029A29C\n.word .L0029A310\n.word .L0029A364\n.word .L0029C9CC\n.word .L0029C9CC\n.word .L0029C9CC\n.word .L0029C9CC\n.word .L0029C9CC\n.word .L0029C9CC\n");
__asm__(".word .L0029C9CC\n.word .L0029A38C\n.word .L0029A480\n.word .L0029A650\n.word .L0029C9CC\n.word .L0029C9CC\n.word .L0029C9CC\n.word .L0029C9CC\n.word .L0029C9CC\n.word .L0029C9CC\n.word .L0029C9CC\n.word .L0029A69C\n.word .L0029A800\n.word .L0029A914\n.word .L0029C9CC\n.word .L0029C9CC\n");
__asm__(".word .L0029C9CC\n.word .L0029C9CC\n.word .L0029C9CC\n.word .L0029C9CC\n.word .L0029C9CC\n.word .L0029C9CC\n.word .L0029C9CC\n.word .L0029C9CC\n.word .L0029C9CC\n.word .L0029C9CC\n.word .L0029C9CC\n.word .L0029C9CC\n.word .L0029C9CC\n.word .L0029C9CC\n.word .L0029C9CC\n.word .L0029AA10\n");
__asm__(".word .L0029AC5C\n.word .L0029AFF8\n.word .L0029C9CC\n.word .L0029C9CC\n.word .L0029C9CC\n.word .L0029C9CC\n.word .L0029C9CC\n.word .L0029C9CC\n.word .L0029C9CC\n.word .L0029C9CC\n.word .L0029C9CC\n.word .L0029C9CC\n.word .L0029C9CC\n.word .L0029C9CC\n.word .L0029C9CC\n.word .L0029C9CC\n");
__asm__(".word .L0029C9CC\n.word .L0029C9CC\n.word .L0029C9CC\n.word .L0029C9CC\n.word .L0029C9CC\n.word .L0029C9CC\n.word .L0029C9CC\n.word .L0029C9CC\n.word .L0029C9CC\n.word .L0029C9CC\n.word .L0029C9CC\n.word .L0029C9CC\n.word .L0029C9CC\n.word .L0029B0BC\n.word .L0029C9CC\n.word .L0029B164\n");
__asm__(".word .L0029C9CC\n.word .L0029C9CC\n.word .L0029C9CC\n.word .L0029C9CC\n.word .L0029C9CC\n.word .L0029C9CC\n.word .L0029C9CC\n.word .L0029B1A8\n.word .L0029B298\n.word .L0029B4E8\n.word .L0029C9CC\n.word .L0029C9CC\n.word .L0029C9CC\n.word .L0029C9CC\n.word .L0029C9CC\n.word .L0029C9CC\n");
__asm__(".word .L0029C9CC\n.word .L0029B52C\n.word .L0029C9CC\n.word .L0029B66C\n.word .L0029C9CC\n.word .L0029C9CC\n.word .L0029C9CC\n.word .L0029C9CC\n.word .L0029C9CC\n.word .L0029C9CC\n.word .L0029C9CC\n.word .L0029B6B0\n.word .L0029B7E0\n.word .L0029B860\n.word .L0029C9CC\n.word .L0029C9CC\n");
__asm__(".word .L0029C9CC\n.word .L0029C9CC\n.word .L0029C9CC\n.word .L0029C9CC\n.word .L0029C9CC\n.word .L0029B8C4\n.word .L0029B984\n.word .L0029B984\n.word .L0029B984\n.word .L0029B984\n.word .L0029BB40\n.word .L0029BB94\n.word .L0029BC34\n.word .L0029C9CC\n.word .L0029C9CC\n.word .L0029C9CC\n");
__asm__(".word .L0029C9CC\n.word .L0029C9CC\n.word .L0029C9CC\n.word .L0029C9CC\n.word .L0029C9CC\n.word .L0029C9CC\n.word .L0029C9CC\n.word .L0029C9CC\n.word .L0029C9CC\n.word .L0029C9CC\n.word .L0029C9CC\n.word .L0029C9CC\n.word .L0029C9CC\n.word .L0029C9CC\n.word .L0029C9CC\n.word .L0029C9CC\n");
__asm__(".word .L0029C9CC\n.word .L0029C9CC\n.word .L0029C9CC\n.word .L0029C9CC\n.word .L0029C9CC\n.word .L0029C9CC\n.word .L0029C9CC\n.word .L0029C9CC\n.word .L0029C9CC\n.word .L0029C9CC\n.word .L0029C9CC\n.word .L0029C9CC\n.word .L0029C9CC\n.word .L0029C9CC\n.word .L0029C9CC\n.word .L0029C9CC\n");
__asm__(".word .L0029C9CC\n.word .L0029C9CC\n.word .L0029C9CC\n.word .L0029C9CC\n.word .L0029C9CC\n.word .L0029C9CC\n.word .L0029C9CC\n.word .L0029C9CC\n.word .L0029C9CC\n.word .L0029C9CC\n.word .L0029C9CC\n.word .L0029C9CC\n.word .L0029C9CC\n.word .L0029C9CC\n.word .L0029C9CC\n.word .L0029C9CC\n");
__asm__(".word .L0029C9CC\n.word .L0029C9CC\n.word .L0029C9CC\n.word .L0029C9CC\n.word .L0029C9CC\n.word .L0029C9CC\n.word .L0029C9CC\n.word .L0029C9CC\n.word .L0029C9CC\n.word .L0029C9CC\n.word .L0029C9CC\n.word .L0029C9CC\n.word .L0029C9CC\n.word .L0029C9CC\n.word .L0029C9CC\n.word .L0029C9CC\n");
__asm__(".word .L0029C9CC\n.word .L0029C9CC\n.word .L0029C9CC\n.word .L0029C9CC\n.word .L0029C9CC\n.word .L0029C9CC\n.word .L0029C9CC\n.word .L0029C9CC\n.word .L0029C9CC\n.word .L0029C9CC\n.word .L0029C9CC\n.word .L0029C9CC\n.word .L0029C9CC\n.word .L0029C9CC\n.word .L0029C9CC\n.word .L0029C9CC\n");
__asm__(".word .L0029C9CC\n.word .L0029C9CC\n.word .L0029C9CC\n.word .L0029C9CC\n.word .L0029C9CC\n.word .L0029C9CC\n.word .L0029C9CC\n.word .L0029C9CC\n.word .L0029C9CC\n.word .L0029BE30\n.word .L0029C9CC\n.word .L0029C9CC\n.word .L0029C9CC\n.word .L0029C9CC\n.word .L0029C9CC\n.word .L0029C9CC\n");
__asm__(".word .L0029C9CC\n.word .L0029C9CC\n.word .L0029C9CC\n.word .L0029BE30\n.word .L0029C9CC\n.word .L0029C9CC\n.word .L0029C9CC\n.word .L0029C9CC\n.word .L0029C9CC\n.word .L0029C9CC\n.word .L0029C9CC\n.word .L0029C9CC\n.word .L0029C9CC\n.word .L0029BEAC\n.word .L0029C9CC\n.word .L0029C9CC\n");
__asm__(".word .L0029C9CC\n.word .L0029C9CC\n.word .L0029C9CC\n.word .L0029C9CC\n.word .L0029C9CC\n.word .L0029C9CC\n.word .L0029C9CC\n.word .L0029C9CC\n.word .L0029C9CC\n.word .L0029C9CC\n.word .L0029C9CC\n.word .L0029C9CC\n.word .L0029C9CC\n.word .L0029C9CC\n.word .L0029C9CC\n.word .L0029C9CC\n");
__asm__(".word .L0029C9CC\n.word .L0029C9CC\n.word .L0029C9CC\n.word .L0029C9CC\n.word .L0029C9CC\n.word .L0029C9CC\n.word .L0029C9CC\n.word .L0029C9CC\n.word .L0029C9CC\n.word .L0029C9CC\n.word .L0029C9CC\n.word .L0029C9CC\n.word .L0029C9CC\n.word .L0029C9CC\n.word .L0029C9CC\n.word .L0029C9CC\n");
__asm__(".word .L0029C9CC\n.word .L0029C9CC\n.word .L0029C9CC\n.word .L0029C9CC\n.word .L0029C9CC\n.word .L0029BE30\n.word .L0029C9CC\n.word .L0029C9CC\n.word .L0029C9CC\n.word .L0029C9CC\n.word .L0029C9CC\n.word .L0029C9CC\n.word .L0029C9CC\n.word .L0029C9CC\n.word .L0029C9CC\n.word .L0029BE30\n");
__asm__(".word .L0029C9CC\n.word .L0029C9CC\n.word .L0029C9CC\n.word .L0029C9CC\n.word .L0029C9CC\n.word .L0029C9CC\n.word .L0029C9CC\n.word .L0029C9CC\n.word .L0029C9CC\n.word .L0029BEAC\n.word .L0029C9CC\n.word .L0029C9CC\n.word .L0029C9CC\n.word .L0029C9CC\n.word .L0029C9CC\n.word .L0029C9CC\n");
__asm__(".word .L0029C9CC\n.word .L0029C9CC\n.word .L0029C9CC\n.word .L0029C9CC\n.word .L0029C9CC\n.word .L0029C9CC\n.word .L0029C9CC\n.word .L0029C9CC\n.word .L0029C9CC\n.word .L0029C9CC\n.word .L0029C9CC\n.word .L0029C9CC\n.word .L0029C9CC\n.word .L0029BF68\n.word .L0029C058\n.word .L0029C0B8\n");
__asm__(".word .L0029C240\n.word .L0029C9CC\n.word .L0029C9CC\n.word .L0029C9CC\n.word .L0029C9CC\n.word .L0029C9CC\n.word .L0029C9CC\n.word .L0029BE30\n.word .L0029C9CC\n.word .L0029C9CC\n.word .L0029C9CC\n.word .L0029C9CC\n.word .L0029C9CC\n.word .L0029C9CC\n.word .L0029C9CC\n.word .L0029C9CC\n");
__asm__(".word .L0029C9CC\n.word .L0029BE30\n.word .L0029C9CC\n.word .L0029C9CC\n.word .L0029C9CC\n.word .L0029C9CC\n.word .L0029C9CC\n.word .L0029C9CC\n.word .L0029C9CC\n.word .L0029C9CC\n.word .L0029C9CC\n.word .L0029BE30\n.word .L0029C9CC\n.word .L0029C9CC\n.word .L0029C9CC\n.word .L0029C9CC\n");
__asm__(".word .L0029C9CC\n.word .L0029C9CC\n.word .L0029C9CC\n.word .L0029C9CC\n.word .L0029C9CC\n.word .L0029BE30\n.word .L0029C9CC\n.word .L0029C9CC\n.word .L0029C9CC\n.word .L0029C9CC\n.word .L0029C9CC\n.word .L0029C9CC\n.word .L0029C9CC\n.word .L0029C9CC\n.word .L0029C9CC\n.word .L0029BE58\n");
__asm__(".word .L0029C9CC\n.word .L0029C9CC\n.word .L0029C9CC\n.word .L0029C9CC\n.word .L0029C9CC\n.word .L0029C9CC\n.word .L0029C9CC\n.word .L0029C9CC\n.word .L0029C9CC\n.word .L0029BE58\n.word .L0029C9CC\n.word .L0029C9CC\n.word .L0029C9CC\n.word .L0029C9CC\n.word .L0029C9CC\n.word .L0029C9CC\n");
__asm__(".word .L0029C9CC\n.word .L0029C9CC\n.word .L0029C9CC\n.word .L0029BE58\n.word .L0029C9CC\n.word .L0029C9CC\n.word .L0029C9CC\n.word .L0029C9CC\n.word .L0029C9CC\n.word .L0029C9CC\n.word .L0029C9CC\n.word .L0029C9CC\n.word .L0029C9CC\n.word .L0029BE58\n.word .L0029C9CC\n.word .L0029C9CC\n");
__asm__(".word .L0029C9CC\n.word .L0029C9CC\n.word .L0029C9CC\n.word .L0029C9CC\n.word .L0029C9CC\n.word .L0029C9CC\n.word .L0029C9CC\n.word .L0029BEAC\n.word .L0029C9CC\n.word .L0029C9CC\n.word .L0029C9CC\n.word .L0029C9CC\n.word .L0029C9CC\n.word .L0029C9CC\n.word .L0029C9CC\n.word .L0029C9CC\n");
__asm__(".word .L0029C9CC\n.word .L0029BEAC\n.word .L0029C9CC\n.word .L0029C9CC\n.word .L0029C9CC\n.word .L0029C9CC\n.word .L0029C9CC\n.word .L0029C9CC\n.word .L0029C9CC\n.word .L0029C9CC\n.word .L0029C9CC\n.word .L0029C9CC\n.word .L0029C9CC\n.word .L0029C9CC\n.word .L0029C9CC\n.word .L0029C9CC\n");
__asm__(".word .L0029C9CC\n.word .L0029C9CC\n.word .L0029C9CC\n.word .L0029C9CC\n.word .L0029C9CC\n.word .L0029C9CC\n.word .L0029C9CC\n.word .L0029C9CC\n.word .L0029C9CC\n.word .L0029C9CC\n.word .L0029C9CC\n.word .L0029C9CC\n.word .L0029C9CC\n.word .L0029C9CC\n.word .L0029C9CC\n.word .L0029C9CC\n");
__asm__(".word .L0029C9CC\n.word .L0029C9CC\n.word .L0029C9CC\n.word .L0029C9CC\n.word .L0029C9CC\n.word .L0029C9CC\n.word .L0029C9CC\n.word .L0029C9CC\n.word .L0029C9CC\n.word .L0029C9CC\n.word .L0029C9CC\n.word .L0029C9CC\n.word .L0029C9CC\n.word .L0029C9CC\n.word .L0029C9CC\n.word .L0029C9CC\n");
__asm__(".word .L0029C9CC\n.word .L0029C9CC\n.word .L0029C9CC\n.word .L0029C9CC\n.word .L0029C9CC\n.word .L0029C9CC\n.word .L0029C9CC\n.word .L0029C9CC\n.word .L0029C9CC\n.word .L0029C9CC\n.word .L0029C9CC\n.word .L0029C9CC\n.word .L0029C9CC\n.word .L0029BE30\n.word .L0029C9CC\n.word .L0029C9CC\n");
__asm__(".word .L0029C9CC\n.word .L0029C9CC\n.word .L0029C9CC\n.word .L0029C9CC\n.word .L0029C9CC\n.word .L0029C9CC\n.word .L0029C9CC\n.word .L0029BE30\n.word .L0029C918\n.word .L0029C2B8\n.word .L0029C9CC\n.word .L0029C9CC\n.word .L0029C9CC\n.word .L0029C9CC\n.word .L0029C9CC\n.word .L0029C9CC\n");
__asm__(".word .L0029C9CC\n.word .L0029C9CC\n.word .L0029C9CC\n.word .L0029C9CC\n.word .L0029C9CC\n.word .L0029C9CC\n.word .L0029C9CC\n.word .L0029C9CC\n.word .L0029C9CC\n.word .L0029C9CC\n.word .L0029C9CC\n.word .L0029C9CC\n.word .L0029C9CC\n.word .L0029C9CC\n.word .L0029C9CC\n.word .L0029C9CC\n");
__asm__(".word .L0029C9CC\n.word .L0029C9CC\n.word .L0029C9CC\n.word .L0029C9CC\n.word .L0029C9CC\n.word .L0029C9CC\n.word .L0029C9CC\n.word .L0029C9CC\n.word .L0029C9CC\n.word .L0029C9CC\n.word .L0029C9CC\n.word .L0029C9CC\n.word .L0029C9CC\n.word .L0029C9CC\n.word .L0029C9CC\n.word .L0029C9CC\n");
__asm__(".word .L0029C9CC\n.word .L0029C9CC\n.word .L0029C9CC\n.word .L0029C9CC\n.word .L0029C9CC\n.word .L0029C9CC\n.word .L0029C9CC\n.word .L0029C9CC\n.word .L0029C9CC\n.word .L0029C9CC\n.word .L0029C9CC\n.word .L0029C9CC\n.word .L0029C9CC\n.word .L0029C9CC\n.word .L0029C9CC\n.word .L0029C9CC\n");
__asm__(".word .L0029C9CC\n.word .L0029C9CC\n.word .L0029C9CC\n.word .L0029C9CC\n.word .L0029C9CC\n.word .L0029C9CC\n.word .L0029C9CC\n.word .L0029C9CC\n.word .L0029C9CC\n.word .L0029C9CC\n.word .L0029C9CC\n.word .L0029C9CC\n.word .L0029C9CC\n.word .L0029C9CC\n.word .L0029C9CC\n.word .L0029C9CC\n");
__asm__(".word .L0029C9CC\n.word .L0029C9CC\n.word .L0029C9CC\n.word .L0029C9CC\n.word .L0029C9CC\n.word .L0029C9CC\n.word .L0029C9CC\n.word .L0029C550\n.word .L0029C9CC\n.word .L0029C9CC\n.word .L0029C9CC\n.word .L0029C9CC\n.word .L0029C9CC\n.word .L0029C9CC\n.word .L0029C9CC\n.word .L0029C9CC\n");
__asm__(".word .L0029C9CC\n.word .L0029C550\n.word .L0029C9CC\n.word .L0029C9CC\n.word .L0029C9CC\n.word .L0029C9CC\n.word .L0029C9CC\n.word .L0029C9CC\n.word .L0029C9CC\n.word .L0029C9CC\n.word .L0029C9CC\n.word .L0029C550\n.word .L0029C9CC\n.word .L0029C9CC\n.word .L0029C9CC\n.word .L0029C9CC\n");
__asm__(".word .L0029C9CC\n.word .L0029C9CC\n.word .L0029C9CC\n.word .L0029C9CC\n.word .L0029C9CC\n.word .L0029C550\n.word .L0029C9CC\n.word .L0029C9CC\n.word .L0029C9CC\n.word .L0029C9CC\n.word .L0029C9CC\n.word .L0029C9CC\n.word .L0029C9CC\n.word .L0029C9CC\n.word .L0029C9CC\n.word .L0029C4EC\n");
__asm__(".word .L0029C9CC\n.word .L0029C9CC\n.word .L0029C9CC\n.word .L0029C9CC\n.word .L0029C9CC\n.word .L0029C9CC\n.word .L0029C9CC\n.word .L0029C9CC\n.word .L0029C9CC\n.word .L0029C4EC\n.word .L0029C9CC\n.word .L0029C9CC\n.word .L0029C9CC\n.word .L0029C9CC\n.word .L0029C9CC\n.word .L0029C9CC\n");
__asm__(".word .L0029C9CC\n.word .L0029C9CC\n.word .L0029C9CC\n.word .L0029C4EC\n.word .L0029C9CC\n.word .L0029C9CC\n.word .L0029C9CC\n.word .L0029C9CC\n.word .L0029C9CC\n.word .L0029C9CC\n.word .L0029C9CC\n.word .L0029C9CC\n.word .L0029C9CC\n.word .L0029C43C\n.word .L0029C9CC\n.word .L0029C9CC\n");
__asm__(".word .L0029C9CC\n.word .L0029C9CC\n.word .L0029C9CC\n.word .L0029C9CC\n.word .L0029C9CC\n.word .L0029C9CC\n.word .L0029C9CC\n.word .L0029C43C\n.word .L0029C9CC\n.word .L0029C9CC\n.word .L0029C9CC\n.word .L0029C9CC\n.word .L0029C9CC\n.word .L0029C9CC\n.word .L0029C9CC\n.word .L0029C9CC\n");
__asm__(".word .L0029C9CC\n.word .L0029C550\n.word .L0029C9CC\n.word .L0029C9CC\n.word .L0029C9CC\n.word .L0029C9CC\n.word .L0029C9CC\n.word .L0029C9CC\n.word .L0029C9CC\n.word .L0029C9CC\n.word .L0029C9CC\n.word .L0029C550\n.word .L0029C9CC\n.word .L0029C9CC\n.word .L0029C9CC\n.word .L0029C9CC\n");
__asm__(".word .L0029C9CC\n.word .L0029C9CC\n.word .L0029C9CC\n.word .L0029C9CC\n.word .L0029C9CC\n.word .L0029C9CC\n.word .L0029C9CC\n.word .L0029C9CC\n.word .L0029C9CC\n.word .L0029C9CC\n.word .L0029C9CC\n.word .L0029C9CC\n.word .L0029C9CC\n.word .L0029C9CC\n.word .L0029C9CC\n.word .L0029C9CC\n");
__asm__(".word .L0029C9CC\n.word .L0029C9CC\n.word .L0029C9CC\n.word .L0029C9CC\n.word .L0029C9CC\n.word .L0029C9CC\n.word .L0029C9CC\n.word .L0029C9CC\n.word .L0029C9CC\n.word .L0029C9CC\n.word .L0029C9CC\n.word .L0029C9CC\n.word .L0029C9CC\n.word .L0029C9CC\n.word .L0029C9CC\n.word .L0029C9CC\n");
__asm__(".word .L0029C9CC\n.word .L0029C9CC\n.word .L0029C9CC\n.word .L0029C4EC\n.word .L0029C9CC\n.word .L0029C9CC\n.word .L0029C9CC\n.word .L0029C9CC\n.word .L0029C9CC\n.word .L0029C9CC\n.word .L0029C9CC\n.word .L0029C9CC\n.word .L0029C9CC\n.word .L0029C4EC\n.word .L0029C9CC\n.word .L0029C9CC\n");
__asm__(".word .L0029C9CC\n.word .L0029C9CC\n.word .L0029C9CC\n.word .L0029C9CC\n.word .L0029C9CC\n.word .L0029C9CC\n.word .L0029C9CC\n.word .L0029C550\n.word .L0029C9CC\n.word .L0029C9CC\n.word .L0029C9CC\n.word .L0029C9CC\n.word .L0029C9CC\n.word .L0029C9CC\n.word .L0029C9CC\n.word .L0029C9CC\n");
__asm__(".word .L0029C9CC\n.word .L0029C550\n.word .L0029C9CC\n.word .L0029C9CC\n.word .L0029C9CC\n.word .L0029C9CC\n.word .L0029C9CC\n.word .L0029C9CC\n.word .L0029C9CC\n.word .L0029C9CC\n.word .L0029C9CC\n.word .L0029C550\n.word .L0029C4C0\n.word .L0029C74C\n.word .L0029C918\n.word .L0029C9CC\n");
__asm__(".word .L0029C9CC\n.word .L0029C9CC\n.word .L0029C9CC\n.word .L0029C9CC\n.word .L0029C9CC\n.word .L0029C9CC\n.word .L0029C9CC\n.word .L0029C9CC\n.word .L0029C9CC\n.word .L0029C9CC\n.word .L0029C9CC\n.word .L0029C9CC\n.word .L0029C9CC\n.word .L0029C9CC\n.word .L0029C9CC\n.word .L0029C92C\n");
__asm__(".word .L0029C92C\n.word .L0029C964\n.word .L0029C9CC\n.word .L0029C9CC\n.word .L0029C9CC\n.word .L0029C9CC\n.word .L0029C9CC\n.word .L0029C9CC\n.word .L0029C9CC\n.word .L0029C7D0\n.word .L0029C850\n.word .L0029C90C\n.word func_0\n.word func_0\n.align 4\n.word .L0029C3D8\n");
__asm__(".word .L0029C3D8\n.word .L0029C3B0\n.word .L0029C3D8\n.word .L0029C3B0\n.word .L0029C3B0\n.word .L0029C3D8\n.word .L0029C3D8\n.word .L0029C3D8\n.word .L0029C3D8\n.word .L0029C3D8\n.word .L0029C3B0\n.word .L0029C3B0\n.word .L0029C3B0\n.word .L0029C3B0\n.word .L0029C3B0\n.word .L0029C3D8\n");
__asm__(".word .L0029C3D8\n.word .L0029C3B0\n.word .L0029C3B0\n.word .L0029C3B0\n.word .L0029C3B0\n.word .L0029C3B0\n.word .L0029C3D8\n.word .L0029C3D8\n.word .L0029C3D8\n.word func_0\n.word func_0\n.text\n");
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Game/Player/PlayerSora", exeStockSkill__Q22PL11CPlayerSora);
#endif
void CPlayerSora::initMagic(INT inNo) {
    FVECTOR pos;
    FVECTOR rot;
    setPlayerState(MAGIC);
    setBattleFlag(0, 8);
    setBattleFlag(0, 0x100);
    setBattleFlag(0, 0x80000);
    setTrgFlag(0, 0x20);
    _ref(m_iAttackNo) = inNo;
    _ref(m_uiPreAttackKind) = 0;
    _ref(m_bIsAttackHit) = FALSE;
    _ref(m_bNextTrg) = FALSE;
    _ref(m_iNextAttack) = 0;
    _ref(m_bIsCanCombo) = FALSE;
    _ref(m_bDmgMuteki) = FALSE;
    clearSpeed();
    _ref(m_uiAttackKind) = 0;
    m_fJumpGravity = g_fGravity;
    m_JumpState = 3;
    m_fMoveSpdXZ = 0.0f;
    m_fJumpSpdY = 0.0f;
    m_fWalkSpeed = 0.0f;
    m_iAerialAnimShift = 0;
    setPlayerFlag(0, 0x2800000);
    m_pMarker->holdTarget(8);
    procTargetMark();
    m_tqCardBreakTime = g_CardBreakTime[CardBattle::g_pBattleCardList[m_uiCardID].BREAK];
    if (isAir()) {
        setPlayerFlag(0x2800000, 0);
        m_iAerialAnimShift = 4;
    }
    switch (m_uiCardID) {
    case 0x1A:
        m_uiAttackID = 0x22;
        m_pAtkData = &g_AttackDataList[m_uiAttackID];
        m_iAttackPower = m_iAP * _ref(CardBattle::g_pBattleCardList)[m_uiCardID].POWER[m_pAtkData->POWER] / 100;
        break;
    case 0x1B:
        m_uiAttackID = 0x23;
        m_pAtkData = &g_AttackDataList[m_uiAttackID];
        m_iAttackPower = m_iAP * _ref(CardBattle::g_pBattleCardList)[m_uiCardID].POWER[m_pAtkData->POWER] / 100;
        break;
    case 0x1C: {
        m_uiAttackID = 0x24;
        m_pAtkData = &g_AttackDataList[m_uiAttackID];
        m_iAttackPower = m_iAP * _ref(CardBattle::g_pBattleCardList)[m_uiCardID].POWER[m_pAtkData->POWER] / 100;
        calcAppearPos((FLOAT)m_pAtkData->WORK0, (FLOAT)m_pAtkData->COLL_START_FRAME * 0.1f, NULL);
        m_vAppearPos.y = getPos().y;
        if (isAir()) m_vAppearPos.y = m_vJumpStartPos.y;
        m_vAppearPos.y += 6.0f;
        CEffect* effect = CEffectMan::getEffect(attachEffect(g_EffTbl_Skill[m_pAtkData->EFFECT][1].pEffectName, 0,
                                                             &m_pWeapon->Crd(2), &PlayerWeapon::g_fWeaponTopOffset, NULL, 0.0f));
        if (effect) effect->m_iFlag |= 4;
        setBGFade(g_vBGFadeColor[0], 20.0f, 0);
        break;
    }
    case 0x1D:
        m_uiAttackID = 0x25;
        m_pAtkData = &g_AttackDataList[m_uiAttackID];
        {
            INT power = _ref(CardBattle::g_pBattleCardList)[m_uiCardID].POWER[m_pAtkData->POWER];
            m_iAttackPower = power / 10;
        }
        attachEffect(g_EffTbl_Skill[m_pAtkData->EFFECT][1].pEffectName, 0, &m_pWeapon->Crd(2),
                     &PlayerWeapon::g_fWeaponTopOffset, NULL, 0.0f);
        break;
    case 0x1E:
        m_uiAttackID = 0x26;
        m_pAtkData = &g_AttackDataList[m_uiAttackID];
        m_iAttackPower = _ref(CardBattle::g_pBattleCardList)[m_uiCardID].POWER[m_pAtkData->POWER];
        attachEffect(g_EffTbl_Skill[m_pAtkData->EFFECT][2].pEffectName, 0, &m_pWeapon->Crd(2),
                     &PlayerWeapon::g_fWeaponTopOffset, NULL, 0.0f);
        calcAppearPos((FLOAT)m_pAtkData->WORK0, (FLOAT)m_pAtkData->COLL_START_FRAME * 0.01f, NULL);
        m_vAppearPos.y = getPos().y;
        if (m_hTarget > 0) {
            m_vAppearPos.x = m_vMarkPos.x;
            m_vAppearPos.z = m_vMarkPos.z;
        }
        if (isAir()) m_vAppearPos.y = m_vJumpStartPos.y;
        m_vAppearPos.y += 3.0f;
        rot = getRot();
        setEffect(g_EffTbl_Skill[m_pAtkData->EFFECT][2].pEffectName, 2, &m_vAppearPos, &rot, 0.0f);
        break;
    case 0x1F: {
        m_uiAttackID = 0x27;
        m_pAtkData = &g_AttackDataList[m_uiAttackID];
        m_iAttackPower = _ref(CardBattle::g_pBattleCardList)[m_uiCardID].POWER[m_pAtkData->POWER];
        pos.set(0.0f, 0.85f, 0.0f);
        CEffect* effect = CEffectMan::getEffect(attachEffect(g_EffTbl_Skill[m_pAtkData->EFFECT][1].pEffectName, 0,
                                                             &Crd(0), &pos, NULL, 0.0f));
        if (effect) effect->m_iFlag |= 4;
        break;
    }
    case 0x20:
        m_uiAttackID = 0x28;
        m_pAtkData = &g_AttackDataList[m_uiAttackID];
        m_iAttackPower = m_iAP * _ref(CardBattle::g_pBattleCardList)[m_uiCardID].POWER[m_pAtkData->POWER] / 100;
        attachEffect(g_EffTbl_Skill[m_pAtkData->EFFECT][1].pEffectName, 0, &m_pWeapon->Crd(2),
                     &PlayerWeapon::g_fWeaponTopOffset, NULL, 0.0f);
        break;
    case 0x21:
        m_uiAttackID = 0x94;
        m_pAtkData = &g_AttackDataList[m_uiAttackID];
        m_iAttackPower = m_iAP * _ref(CardBattle::g_pBattleCardList)[m_uiCardID].POWER[m_pAtkData->POWER] / 100;
        setPlayerFlag(0, 0x2800000);
        m_iAerialAnimShift = 0;
        break;
    case 0x22:
        m_uiAttackID = 0x95;
        m_pAtkData = &g_AttackDataList[m_uiAttackID];
        {
            INT power = _ref(CardBattle::g_pBattleCardList)[m_uiCardID].POWER[m_pAtkData->POWER];
            m_iAttackPower = power / 10;
        }
        setPlayerFlag(0, 0x2800000);
        m_iAerialAnimShift = 0;
        break;
    default:
        m_iSubState = 0x209;
        m_pAtkData = &g_AttackDataList[0];
        m_uiAttackID = 0;
        m_uiAttackKind = 0;
        return;
    }
    _ref(m_AtkKind) = (ATK_KIND)m_pAtkData->DAMAGE;
    _ref(m_AtkAttr) = (ATK_ATTR)CardBattle::g_pBattleCardList[m_uiCardID].ATTR;
    UINT up = _upKind(CardBattle::g_pBattleCardList[m_uiCardID].UP_TYPE0,
                      CardBattle::g_pBattleCardList[m_uiCardID].UP_TYPE1);
    calcEnchant_AttackPoint(m_iAttackPower, m_AtkKind, m_AtkAttr, up);
    if (isPlayerFlag(0x800000)) {
        setPlayerFlag(0x2000000, 0);
        m_fJumpGravity = g_fGravity * 0.16f;
    }
    m_uiPreAttackKind = m_uiAttackKind;
    m_fDistXZ = m_fDistY = 0.0f;
    m_iSubState = m_uiAttackID;
    INT SESeq = getStateSESeq(4);
    setAnimForce(m_pAtkData->ANIM_ID + m_iAerialAnimShift, -1, 1, m_pAtkData->ANIM_ESQ, SESeq, 0);
    setPlayerFlag(0x100000, 0);
    m_tqStateTimer = 0;
}
/* Game/MinigameManager.h */
void CPlayerSora::exeMagic() {
    FVECTOR pos; /* unused locals of the original (stack frame) */
    FVECTOR rot;
    TIMEQ frameTQ = getMotionCurrentTime();
    TIMEQ frame = frameTQ / 50;
    setBattleFlag(0, 8);
    m_fDistXZ = m_fDistY = 0.0f;
    procGravity(frame, m_pAtkData->MOVE_END_FRAME, 1);
    switch (m_iSubState) {
    case 0x22:
        m_fJumpSpdY = 0.0f;
        procTargetMark();
        procAttackMove(frame, 0.0f, 0.0f, 2, 0.15f);
        if (isMotionEnd()) {
            m_iSubState = 0x209;
            setAnimForce(m_iNowAnimID + 1, 0, 1, m_pAtkData->ANIM_ESQ, 0, 0);
            setMagic(0x22, m_uiCardID, 0);
            setTimer(4000);
        }
        break;
    case 0x23:
        m_fJumpSpdY = 0.0f;
        procTargetMark();
        procAttackMove(frame, 0.0f, 0.0f, 2, 0.15f);
        if (frame < m_pAtkData->TRG_FRAME) break;
        m_iSubState = 0x209;
        setMagic(0x23, m_uiCardID, 0);
        setTimer(4000);
        break;
    case 0x24:
        m_fJumpSpdY = 0.0f;
        procTargetMark();
        procAttackMove(frame, 0.0f, 0.0f, 2, 0.15f);
        if (frame < m_pAtkData->TRG_FRAME) break;
        m_iSubState = 0x209;
        setMagic(0x24, m_uiCardID, 0);
        setTimer(4000);
        break;
    case 0x25:
        m_fJumpSpdY = 0.0f;
        if (frame < m_pAtkData->TRG_FRAME) break;
        m_iSubState = 0x209;
        setMagic(0x25, m_uiCardID, 0);
        setTimer(4000);
        break;
    case 0x26:
        m_fJumpSpdY = 0.0f;
        procTargetMark();
        procAttackMove(frame, 0.0f, 0.0f, 2, 0.15f);
        if (frame < m_pAtkData->TRG_FRAME) break;
        m_iSubState = 0x209;
        setMagic(0x26, m_uiCardID, 0);
        setTimer(4000);
        break;
    case 0x27:
        m_fJumpSpdY = 0.0f;
        procTargetMark();
        procAttackMove(frame, 0.0f, 0.0f, 2, 0.15f);
        if (frame < m_pAtkData->TRG_FRAME) break;
        m_iSubState = 0x209;
        setMagic(0x27, m_uiCardID, 0);
        setTimer(4000);
        break;
    case 0x28:
        m_fJumpSpdY = 0.0f;
        procTargetMark();
        procAttackMove(frame, 0.0f, 0.0f, 2, 0.15f);
        if (frame < m_pAtkData->TRG_FRAME) break;
        m_iSubState = 0x209;
        setMagic(0x28, m_uiCardID, 0);
        setTimer(4000);
        break;
    case 0x94:
        m_fJumpSpdY = 0.0f;
        procTargetMark();
        procAttackMove(frame, 0.0f, 0.0f, 2, 0.15f);
        if (frame < m_pAtkData->TRG_FRAME) break;
        m_iSubState = 0x96;
        m_hMagicCore = MG_SKILL::createMGSkill(0, 0);
        break;
    case 0x95:
        m_fJumpSpdY = 0.0f;
        if (frame < m_pAtkData->TRG_FRAME) break;
        m_iSubState = 0x96;
        m_hMagicCore = MG_SKILL::createMGSkill(1, 0);
        break;
    case 0x96:
        if (!m_bNextTrg) {
            if (isMotionEnd()) {
                m_bNextTrg = TRUE;
            }
        } else {
            exeStandUp_inFriend();
        }
        if (MG_SKILL::isMGSkillAlive() && m_hMagicCore > 0) break;
        m_iSubState = 0x209;
        clearSpeed();
        setTimer(1000);
        break;
    case 0x209:
        if (isBattleFlag(0x1000) && exeUseStock(1)) {
            if (isPlayerFlag(0x80000000)) setBGFade(1.0f, 20.0f, 0);
            break;
        }
        setGroundIK(1);
        if (isPlayerFlag(0x800000)) {
            setPlayerFlag(0, 0x2000000);
            if (!isAir()) {
                if (m_iNowAnimID != 0x2A) {
                    if (m_iNowAnimID != 7) {
                        setAnim(7, -1, 0);
                    } else if (isMotionEnd()) {
                        setAnim(0x2A, -1, 0);
                    }
                }
            }
        }
        if (isMotionEnd() || getTimer() <= 0) {
            procSkillEnd(-1);
        }
        break;
    }
}
void CPlayerSora::setMagic(INT inState, INT inCardID, INT inSkillID) {
    FVECTOR pos;
    FVECTOR rot;
    BULLET_PARAM param;
    CBullet* pBullet;
    INT i;
    if (!inCardID && !inSkillID) return;
    switch (inState) {
    case 0x22:
        getWeaponTopPos(pos);
        param.init(2, 3);
        param.CollKind = COLL_KIND_PSHOT;
        param.uiFlag = 0x9100;
        param.AtkKind = m_AtkKind;
        param.AtkAttr = m_AtkAttr;
        param.iAttackPoint = m_iAttackPower;
        param.fSpeed = m_pAtkData->MAX_SPEED * 0.01f;
        param.fMaxDist = m_pAtkData->WORK0;
        param.tqHomingTime = 12000;
        param.hHomingTarget = m_hTarget;
        param.fHomingRadius = m_pAtkData->RADIUS;
        param.fHomingRate = m_pAtkData->WORK1;
        param.vPos = pos;
        param.vPos.w = 0.5f;
        param.vRot = getRot();
        param.pEffectTbl = (BULLET_EFFECT_INFO*)g_EffTbl_Skill[m_pAtkData->EFFECT];
        param.fCollRadius = m_pAtkData->COLL_START_FRAME * 0.01f;
        pBullet = (CBullet*)CEntityFactory::getInstance()->createBullet(param, this);
        if (pBullet) {
            pBullet->setBodyRadius(0.1f, 0.0f, 0, (COLL_KIND)-1);
        }
        break;
    case 0x23:
        getWeaponTopPos(pos);
        rot = getRot();
        param.init(2, 3);
        param.CollKind = COLL_KIND_PSHOT;
        param.AtkKind = m_AtkKind;
        param.AtkAttr = m_AtkAttr;
        param.iAttackPoint = m_iAttackPower;
        if (inSkillID == 0x24) {
            param.uiFlag = 0x8000;
            param.tqHomingTime = 12000;
            param.hHomingTarget = m_hTarget;
            param.fHomingRadius = m_pAtkData->RADIUS;
            param.fHomingRate = m_pAtkData->WORK1;
        }
        param.uiFlag |= 0x4000000;
        param.fSpeed = m_pAtkData->MAX_SPEED * 0.01f;
        param.fMaxDist = m_pAtkData->WORK0;
        param.vPos = pos;
        param.vRot = rot;
        param.pEffectTbl = (BULLET_EFFECT_INFO*)g_EffTbl_Skill[m_pAtkData->EFFECT];
        param.fCollRadius = m_pAtkData->COLL_START_FRAME * 0.01f;
        param.tqExplodeTime = m_pAtkData->WORK2 * 100;
        param.vPos.w = m_pAtkData->COLL_END_FRAME * 0.01f;
        pBullet = (CBullet*)CEntityFactory::getInstance()->createBullet(param, this);
        if (pBullet) {
            param.uiAttackID = pBullet->getAttackID();
        }
        switch (inSkillID) {
        case 0:
            param.pEffectTbl = (BULLET_EFFECT_INFO*)g_EffTbl_Skill[m_pAtkData->EFFECT + 1];
            param.vRot.y = _limitPI2(rot.y - 0.17467256f);
            CEntityFactory::getInstance()->createBullet(param, this);
            param.vRot.y = _limitPI2(rot.y + 0.17467256f);
            CEntityFactory::getInstance()->createBullet(param, this);
            break;
        case 0xE:
            param.pEffectTbl = (BULLET_EFFECT_INFO*)g_EffTbl_Skill[m_pAtkData->EFFECT + 1];
            param.vRot.y = _limitPI2(rot.y - 0.21802653f);
            CEntityFactory::getInstance()->createBullet(param, this);
            param.vRot.y = _limitPI2(rot.y + 0.21802653f);
            CEntityFactory::getInstance()->createBullet(param, this);
            param.vRot.x = _limitPI2(rot.x - 0.1781283f);
            param.vRot.y = _limitPI2(rot.y - 0.10461504f);
            CEntityFactory::getInstance()->createBullet(param, this);
            param.vRot.y = _limitPI2(rot.y + 0.10461504f);
            CEntityFactory::getInstance()->createBullet(param, this);
            break;
        case 0x15:
            param.pEffectTbl = (BULLET_EFFECT_INFO*)g_EffTbl_Skill[m_pAtkData->EFFECT + 1];
            param.vRot.y = _limitPI2(rot.y - 0.17467256f);
            CEntityFactory::getInstance()->createBullet(param, this);
            param.vRot.y = _limitPI2(rot.y + 0.17467256f);
            CEntityFactory::getInstance()->createBullet(param, this);
            param.vRot.y = _limitPI2(rot.y - 0.29656637f);
            CEntityFactory::getInstance()->createBullet(param, this);
            param.vRot.y = _limitPI2(rot.y + 0.29656637f);
            CEntityFactory::getInstance()->createBullet(param, this);
            param.vRot.x = _limitPI2(rot.x - 0.1781283f);
            param.vRot.y = _limitPI2(rot.y - 0.122207955f);
            CEntityFactory::getInstance()->createBullet(param, this);
            param.vRot.y = _limitPI2(rot.y + 0.122207955f);
            CEntityFactory::getInstance()->createBullet(param, this);
            break;
        }
        break;
    case 0x24:
        getWeaponTopPos(pos);
        rot = getRot();
        setEffect(g_EffTbl_Skill[m_pAtkData->EFFECT][1].pEffectName, 1, &pos, &rot, 0.0f);
        pos = m_vAppearPos;
        if (m_hTarget > 0) {
            pos.x = m_vMarkPos.x;
            pos.z = m_vMarkPos.z;
        } else {
            pos.x = m_vCenterPos.x;
            pos.z = m_vCenterPos.z;
        }
        pos.y -= 6.0f;
        setEffect(g_EffTbl_Skill[m_pAtkData->EFFECT][1].pEffectName, m_pAtkData->EFFECT - 10, &pos, &rot, 0.0f);
        pos.y += 6.0f;
        param.init(1, 3);
        param.CollKind = COLL_KIND_PSHOT;
        param.uiFlag = 0x47;
        param.AtkKind = m_AtkKind;
        param.AtkAttr = m_AtkAttr;
        param.iAttackPoint = m_iAttackPower;
        param.fSpeed = m_pAtkData->MAX_SPEED * 0.01f;
        param.fMaxDist = m_pAtkData->WORK0;
        param.tqHomingTime = 800;
        param.vPos = pos;
        rot.x = _limitPI2(1.5707964f);
        param.vRot = rot;
        param.pEffectTbl = (BULLET_EFFECT_INFO*)g_EffTbl_Skill[m_pAtkData->EFFECT];
        param.fCollRadius = m_pAtkData->COLL_START_FRAME * 0.1f;
        pBullet = (CBullet*)CEntityFactory::getInstance()->createBullet(param, this);
        if (pBullet) {
            pBullet->setBodyRadius(0.1f, 0.0f, 0, (COLL_KIND)-1);
        }
        break;
    case 0x25: {
        pos.set(0.0f, 0.85f, 0.0f);
        CEffect* effect = CEffectMan::getEffect(attachEffect(g_EffTbl_Skill[m_pAtkData->EFFECT][1].pEffectName,
                                                             m_pAtkData->EFFECT - 0xE, &Crd(0), &pos, NULL, 0.0f));
        if (effect) effect->m_iFlag |= 4;
        addHP(m_iAttackPower, 0, 1);
        break;
    }
    case 0x26:
        getWeaponTopPos(pos);
        rot = getRot();
        setEffect(g_EffTbl_Skill[m_pAtkData->EFFECT][2].pEffectName, 1, &pos, &rot, 0.0f);
        pos = m_vAppearPos;
        if (m_hTarget > 0) {
            pos.x = m_vMarkPos.x;
            pos.z = m_vMarkPos.z;
        }
        param.init(2, 3);
        param.CollKind = COLL_KIND_PSHOT;
        param.uiFlag = 0x41E1;
        param.AtkKind = m_AtkKind;
        param.AtkAttr = m_AtkAttr;
        param.iAttackPoint = m_iAttackPower;
        param.fSpeed = m_pAtkData->MAX_SPEED * 0.01f;
        param.fMaxDist = m_pAtkData->WORK0;
        param.tqHomingTime = m_pAtkData->WORK2 * 100;
        param.vPos = pos;
        rot.x = _limitPI2(1.5707964f);
        param.vRot = rot;
        param.pEffectTbl = (BULLET_EFFECT_INFO*)g_EffTbl_Skill[m_pAtkData->EFFECT];
        param.pRotTbl = g_RotTbl_Magic;
        param.fCollRadius = m_pAtkData->COLL_START_FRAME * 0.01f;
        param.vPos.w = m_pAtkData->COLL_END_FRAME * 0.01f;
        pBullet = (CBullet*)CEntityFactory::getInstance()->createBullet(param, this);
        if (pBullet) {
            pBullet->setBodyRadius(m_pAtkData->WORK1 * 0.01f, 0.0f, 0, (COLL_KIND)-1);
        }
        break;
    case 0x27:
        rot = getRot();
        param.init(1, 3);
        param.CollKind = COLL_KIND_PSHOT;
        param.uiFlag = 5;
        if (m_hTarget > 0) {
            pos = m_vMarkPos;
            param.uiFlag = 0x25;
            param.fSpeed = 0.0f;
        } else {
            pos = m_vCenterPos;
            param.fSpeed = m_pAtkData->MAX_SPEED * 0.01f;
        }
        param.AtkKind = m_AtkKind;
        param.AtkAttr = m_AtkAttr;
        param.iAttackPoint = m_iAttackPower;
        param.fMaxDist = m_pAtkData->WORK0;
        param.tqHomingTime = m_pAtkData->WORK2 * 100;
        param.vPos = pos;
        param.vRot = rot;
        param.pEffectTbl = (BULLET_EFFECT_INFO*)g_EffTbl_Skill[m_pAtkData->EFFECT];
        param.fCollRadius = m_pAtkData->COLL_START_FRAME * 0.1f;
        pBullet = (CBullet*)CEntityFactory::getInstance()->createBullet(param, this);
        if (pBullet) {
            pBullet->setBodyRadius(0.1f, 0.0f, 0, (COLL_KIND)-1);
        }
        break;
    case 0x28:
        getWeaponTopPos(pos);
        rot = getRot();
        setEffect(g_EffTbl_Skill[m_pAtkData->EFFECT][1].pEffectName, 1, &pos, &rot, 0.0f);
        pos = getPos();
        if (!isAir()) {
            setEffect(g_EffTbl_Skill[m_pAtkData->EFFECT][1].pEffectName, m_pAtkData->EFFECT - 0x16, &pos, &rot, 0.0f);
        }
        setEffect(g_EffTbl_Skill[m_pAtkData->EFFECT][1].pEffectName, m_pAtkData->EFFECT - 0x11, &pos, &rot, 0.0f);
        param.init(1, 3);
        param.CollKind = COLL_KIND_PSHOT;
        param.uiFlag = 7;
        param.AtkKind = m_AtkKind;
        param.AtkAttr = m_AtkAttr;
        param.iAttackPoint = m_iAttackPower;
        param.fSpeed = m_pAtkData->MAX_SPEED * 0.01f;
        param.fMaxDist = m_pAtkData->WORK0;
        param.tqHomingTime = m_pAtkData->WORK2 * 100;
        param.vPos = pos;
        rot.x = _limitPI2(-1.5707964f);
        param.vRot = rot;
        param.pEffectTbl = (BULLET_EFFECT_INFO*)g_EffTbl_Skill[m_pAtkData->EFFECT];
        param.fCollRadius = m_pAtkData->COLL_START_FRAME * 0.1f;
        CEntityFactory::getInstance()->createBullet(param, this);
        break;
    case 0x3E:
    case 0x3F:
    case 0x41:
    case 0x42:
    case 0x44:
        param.init(1, 3);
        param.CollKind = COLL_KIND_PSHOT;
        param.uiFlag = 4;
        param.AtkKind = m_AtkKind;
        param.AtkAttr = m_AtkAttr;
        param.iAttackPoint = m_iAttackPower;
        param.tqHomingTime = 0;
        param.fSpeed = 0.0f;
        param.vPos = m_hTarget > 0 ? m_vMarkPos : m_vCenterPos;
        param.vRot = getRot();
        param.pEffectTbl = (BULLET_EFFECT_INFO*)g_EffTbl_Skill[m_pAtkData->EFFECT];
        param.fCollRadius = m_pAtkData->COLL_START_FRAME * 0.01f;
        switch (inSkillID) {
        case 0x31:
            param.iAttackPoint = -m_iAttackPower;
        case 0x2C:
        case 0x2E:
        case 0x2F:
            param.fCollRadius = 50.0f;
            CEntityFactory::getInstance()->createBullet(param, this);
            break;
        case 0x2B:
        case 0x32:
            CEntityFactory::getInstance()->createBullet(param, this);
            break;
        case 0x2D:
        case 0x30:
            break;
        }
        break;
    case 0x40:
        rot = getRot();
        pos = getPos();
        if (isAir()) pos.y = m_vJumpStartPos.y;
        setEffect("bind", 0, &pos, &rot, 0.0f);
        param.init(1, 3);
        param.CollKind = COLL_KIND_PSHOT;
        param.uiFlag = 4;
        param.AtkKind = m_AtkKind;
        param.AtkAttr = m_AtkAttr;
        param.iAttackPoint = m_iAttackPower;
        param.tqHomingTime = 0;
        param.fSpeed = 0.0f;
        param.vPos = m_hTarget > 0 ? m_vMarkPos : m_vCenterPos;
        param.vRot = rot;
        param.pEffectTbl = (BULLET_EFFECT_INFO*)g_EffTbl_Skill[m_pAtkData->EFFECT];
        param.fCollRadius = 50.0f;
        CEntityFactory::getInstance()->createBullet(param, this);
        break;
    case 0x43:
        getWeaponTopPos(pos);
        param.init(2, 3);
        param.CollKind = COLL_KIND_PSHOT;
        param.uiFlag = 0x8000;
        param.AtkKind = m_AtkKind;
        param.AtkAttr = m_AtkAttr;
        param.iAttackPoint = m_iAttackPower;
        param.tqHomingTime = 12000;
        param.hHomingTarget = m_hTarget;
        param.fHomingRadius = m_pAtkData->RADIUS;
        param.fHomingRate = m_pAtkData->WORK1;
        param.fSpeed = m_pAtkData->MAX_SPEED * 0.01f;
        param.fMaxDist = m_pAtkData->WORK0;
        param.vPos = pos;
        param.vRot = getRot();
        param.pEffectTbl = (BULLET_EFFECT_INFO*)g_EffTbl_Skill[m_pAtkData->EFFECT];
        param.fCollRadius = m_pAtkData->COLL_START_FRAME * 0.01f;
        pBullet = (CBullet*)CEntityFactory::getInstance()->createBullet(param, this);
        if (pBullet) {
            m_hMagicCore = pBullet->getHandle();
        }
        break;
    case 0x2B2:
        if (inSkillID == -1) {
            param.init(2, 3);
            param.CollKind = COLL_KIND_PSHOT;
            param.uiFlag = 0x1C8007;
            param.AtkKind = m_AtkKind;
            param.AtkAttr = m_AtkAttr;
            param.iAttackPoint = m_iAttackPower;
            param.fSpeed = m_pAtkData->MAX_SPEED * 0.01f;
            param.tqHomingTime = m_pAtkData->WORK2 * 100;
            param.fHomingRadius = m_pAtkData->RADIUS * 0.1f;
            param.fHomingRate = m_pAtkData->WORK1;
            getWeaponTopPos(pos);
            param.vPos = pos;
            rot = getRot();
            rot.x = DEG2RAD(-45.0f);
            param.vRot = rot;
            param.vHomingPos = m_vAppearPos;
            if (m_hTarget > 0) {
                param.vHomingPos.x = m_vMarkPos.x;
                param.vHomingPos.z = m_vMarkPos.z;
            }
            SubVectorXYZ(pos, param.vHomingPos, param.vPos);
            param.fMaxDist = Math3D::VectorLength(pos);
            param.pEffectTbl = (BULLET_EFFECT_INFO*)g_EffTbl_Skill[m_pAtkData->EFFECT];
            param.fCollRadius = m_pAtkData->COLL_START_FRAME * 0.01f;
            pBullet = (CBullet*)CEntityFactory::getInstance()->createBullet(param, this);
            if (pBullet) {
                m_hMagicCore = pBullet->getHandle();
            }
        } else {
            if (m_hMagicCore <= 0) break;
            pBullet = (CBullet*)CTaskMan::getTask(m_hMagicCore);
            if (!pBullet) {
                m_hMagicCore = 0;
                break;
            }
            pos = pBullet->getPos();
            rot.x = _limitPI2(DEG2RAD(getRandF() * 90.0f - 10.0f));
            rot.y = _limitPI2(DEG2RAD((m_iCount % 8) * 45.0f + getRandF() * 45.0f));
            rot.z = 0.0f;
            param.init(1, 3);
            param.CollKind = COLL_KIND_PSHOT;
            param.uiFlag = 8;
            param.AtkKind = m_AtkKind;
            param.AtkAttr = m_AtkAttr;
            param.iAttackPoint = m_iAttackPower;
            param.fSpeed = m_pAtkData->MAX_SPEED * 0.01f;
            param.fMaxDist = m_pAtkData->WORK0;
            param.tqHomingTime = 9000;
            param.vPos = pos;
            param.vRot = rot;
            param.tqFadeTime = 800;
            param.pEffectTbl = (BULLET_EFFECT_INFO*)g_EffTbl_Skill[m_pAtkData->EFFECT];
            param.pRotTbl = g_RotTbl_Magic;
            param.fCollRadius = m_pAtkData->RADIUS * 0.01f;
            pBullet = (CBullet*)CEntityFactory::getInstance()->createBullet(param, this);
            if (!pBullet) break;
            pBullet->setBodyRadius(0.15f, 0.0f, 0, (COLL_KIND)-1);
            if (!(m_iCount % 10)) {
                m_pWeapon->setCollAttackID(pBullet->getAttackID());
            } else {
                pBullet->setAttackID(m_pWeapon->getCollAttackID());
            }
        }
        break;
    case 0x2C6:
        pos = getPos();
        rot = getRot();
        param.init(1, 3);
        param.CollKind = COLL_KIND_PSHOT;
        param.uiFlag = 7;
        param.AtkKind = m_AtkKind;
        param.AtkAttr = m_AtkAttr;
        param.iAttackPoint = m_iAttackPower;
        param.fSpeed = m_pAtkData->MAX_SPEED * 0.01f;
        param.fMaxDist = m_pAtkData->WORK0;
        param.tqHomingTime = m_pAtkData->TRG_FRAME * 50;
        param.vPos = pos;
        rot.x = _limitPI2(-1.5707964f);
        param.vRot = rot;
        param.pEffectTbl = (BULLET_EFFECT_INFO*)g_EffTbl_Skill[m_pAtkData->EFFECT];
        param.fCollRadius = m_pAtkData->COLL_START_FRAME * 0.1f;
        pBullet = (CBullet*)CEntityFactory::getInstance()->createBullet(param, this);
        if (!pBullet) break;
        pBullet->setBodyRadius(0.1f, 0.0f, 0, (COLL_KIND)-1);
        if (!m_iCount) {
            m_pWeapon->setCollAttackID(pBullet->getAttackID());
        } else {
            pBullet->setAttackID(m_pWeapon->getCollAttackID());
        }
        break;
    case 0x2D0:
        param.init(2, 3);
        param.uiFlag = 0x800401;
        param.CollKind = COLL_KIND_PSHOT;
        param.AtkKind = m_AtkKind;
        param.AtkAttr = m_AtkAttr;
        param.iAttackPoint = m_iAttackPower;
        param.fSpeed = 0.01f;
        param.fMaxDist = 0.0f;
        param.tqHomingTime = 100;
        param.pEffectTbl = (BULLET_EFFECT_INFO*)g_EffTbl_Skill[m_pAtkData->EFFECT];
        param.pExplodeEffectTbl = (BULLET_EFFECT_INFO*)&g_EffTbl_HitMark[14];
        param.fCollRadius = m_pAtkData->COLL_START_FRAME * 0.01f;
        param.tqExplodeTime = m_pAtkData->WORK2 * 100;
        getWeaponTopPos(pos);
        calcAppearPos(m_pAtkData->WORK0 * 0.1f, param.fCollRadius, &pos);
        param.vPos = m_vAppearPos;
        param.vRot = getRot();
        param.vPos.w = m_pAtkData->COLL_END_FRAME * 0.01f;
        pBullet = (CBullet*)CEntityFactory::getInstance()->createBullet(param, this);
        if (pBullet) {
            m_hMagicCore = pBullet->getHandle();
        }
        break;
    case 0x2E4:
        getWeaponTopPos(pos);
        rot = getRot();
        param.init(1, 3);
        param.CollKind = COLL_KIND_PSHOT;
        param.uiFlag = 7;
        param.AtkKind = m_AtkKind;
        param.AtkAttr = m_AtkAttr;
        param.iAttackPoint = m_iAttackPower;
        param.fSpeed = m_pAtkData->MAX_SPEED * 0.01f;
        param.fMaxDist = m_pAtkData->WORK0;
        param.tqHomingTime = m_pAtkData->TRG_FRAME * 50;
        param.vPos = pos;
        param.vRot = rot;
        param.pEffectTbl = (BULLET_EFFECT_INFO*)g_EffTbl_Skill[m_pAtkData->EFFECT];
        param.fCollRadius = m_pAtkData->COLL_START_FRAME * 0.1f;
        pBullet = (CBullet*)CEntityFactory::getInstance()->createBullet(param, this);
        if (!pBullet) break;
        pBullet->setBodyRadius(0.1f, 0.0f, 0, (COLL_KIND)-1);
        if (!m_iCount) {
            m_pWeapon->setCollAttackID(pBullet->getAttackID());
        } else {
            pBullet->setAttackID(m_pWeapon->getCollAttackID());
        }
        break;
    case 0x2EE:
        param.init(1, 3);
        param.CollKind = COLL_KIND_PSHOT;
        param.uiFlag = 6;
        param.AtkKind = m_AtkKind;
        param.AtkAttr = m_AtkAttr;
        param.iAttackPoint = m_pAtkData->WORK0 * 50;
        param.tqHomingTime = m_pAtkData->WORK0 * 50;
        param.fSpeed = 0.0f;
        param.fMaxDist = 0.0f;
        param.vPos = m_vCenterPos;
        param.vRot = getRot();
        param.pEffectTbl = (BULLET_EFFECT_INFO*)g_EffTbl_Skill[m_pAtkData->EFFECT];
        param.fCollRadius = m_pAtkData->RADIUS * 0.1f;
        pBullet = (CBullet*)CEntityFactory::getInstance()->createBullet(param, this);
        if (pBullet) {
            pBullet->setBodyRadius(m_pAtkData->WORK1 * 0.1f, 0.0f, 0, (COLL_KIND)-1);
        }
        break;
    case 0x320:
        getWeaponTopPos(pos);
        rot = getRot();
        param.init(1, 3);
        param.CollKind = COLL_KIND_PSHOT;
        param.uiFlag = 0x25;
        param.fSpeed = m_pAtkData->MAX_SPEED * 0.01f;
        param.AtkKind = m_AtkKind;
        param.AtkAttr = m_AtkAttr;
        param.iAttackPoint = m_iAttackPower;
        param.fMaxDist = m_pAtkData->WORK0;
        param.tqHomingTime = m_pAtkData->WORK1 * 100;
        param.vPos = pos;
        param.vRot = rot;
        param.pEffectTbl = (BULLET_EFFECT_INFO*)g_EffTbl_Skill[m_pAtkData->EFFECT];
        param.fCollRadius = m_pAtkData->COLL_START_FRAME * 0.1f;
        pBullet = (CBullet*)CEntityFactory::getInstance()->createBullet(param, this);
        if (pBullet) {
            pBullet->setBodyRadius(0.1f, 0.0f, 0, (COLL_KIND)-1);
        }
        break;
    case 0x32A:
        param.init(2, 3);
        param.CollKind = COLL_KIND_PSHOT;
        param.uiFlag = 7;
        param.AtkKind = m_AtkKind;
        param.AtkAttr = m_AtkAttr;
        param.iAttackPoint = m_iAttackPower;
        param.fSpeed = m_pAtkData->MAX_SPEED * 0.1f / 60.0f;
        param.fMaxDist = 30.0f;
        param.tqHomingTime = m_pAtkData->MOVE_END_FRAME * 100;
        param.vPos = getPos();
        param.vRot = getRot();
        param.pEffectTbl = (BULLET_EFFECT_INFO*)g_EffTbl_Skill[m_pAtkData->EFFECT];
        param.fCollRadius = m_pAtkData->RADIUS * 0.1f;
        param.fShiftY = m_pAtkData->RADIUS * 0.1f;
        pBullet = (CBullet*)CEntityFactory::getInstance()->createBullet(param, this);
        m_hMagicCore = pBullet->getHandle();
        pBullet->setBodyRadius(m_pAtkData->RADIUS * 0.01f, m_pAtkData->RADIUS * 0.01f, 1, (COLL_KIND)0);
        break;
    case 0x334:
        param.init(1, 3);
        param.CollKind = COLL_KIND_PSHOT;
        param.uiFlag = 6;
        param.AtkKind = m_AtkKind;
        param.AtkAttr = m_AtkAttr;
        param.iAttackPoint = m_iAttackPower;
        param.tqHomingTime = m_pAtkData->WORK0 * 50;
        param.fSpeed = 0.0f;
        param.fMaxDist = 0.0f;
        param.vPos = getPos();
        param.vRot = getRot();
        param.pEffectTbl = (BULLET_EFFECT_INFO*)g_EffTbl_Skill[m_pAtkData->EFFECT];
        param.fCollRadius = m_pAtkData->RADIUS;
        CEntityFactory::getInstance()->createBullet(param, this);
        break;
    case 0x33E: {
        CBattleEnemyManager* man = (CBattleEnemyManager*)CTaskMan::getSysTask(0x19);
        if (!man) break;
        SLContainerT<CTask>* container = man->getFirstBattleEnemy();
        if (!container) break;
        do {
            CBattleEnemy* enemy = (CBattleEnemy*)container->getValue();
            if (enemy && enemy->getTypeID() == 0x1004) {
                enemy->onGift(m_iAttackPower);
            }
            container = container->getNext();
        } while (container);
        break;
    }
    case 0x348:
        pos = m_vAppearPos;
        pos.y += 6.0f;
        param.init(1, 3);
        param.CollKind = COLL_KIND_PSHOT;
        param.uiFlag = 0x65;
        param.AtkKind = m_AtkKind;
        param.AtkAttr = m_AtkAttr;
        param.iAttackPoint = m_iAttackPower;
        param.fSpeed = m_pAtkData->MAX_SPEED * 0.01f;
        param.fMaxDist = m_pAtkData->WORK0;
        param.tqHomingTime = 800;
        param.vPos = pos;
        rot.x = _limitPI2(1.5707964f);
        param.vRot = rot;
        param.pEffectTbl = (BULLET_EFFECT_INFO*)g_EffTbl_Skill[m_pAtkData->EFFECT];
        param.fCollRadius = m_pAtkData->RADIUS * 0.1f;
        pBullet = (CBullet*)CEntityFactory::getInstance()->createBullet(param, this);
        if (pBullet) {
            pBullet->setBodyRadius(0.1f, 0.0f, 0, (COLL_KIND)-1);
        }
        break;
    case 0x2DA:
        param.init(1, 3);
        param.CollKind = COLL_KIND_PSHOT;
        param.uiFlag = 6;
        param.AtkKind = m_AtkKind;
        param.AtkAttr = m_AtkAttr;
        param.iAttackPoint = m_iAttackPower;
        param.tqHomingTime = m_pAtkData->WORK0 * 100;
        param.fSpeed = 0.0f;
        param.fMaxDist = 0.0f;
        param.vPos = getPos();
        param.vRot = getRot();
        param.pEffectTbl = (BULLET_EFFECT_INFO*)g_EffTbl_Skill[m_pAtkData->EFFECT];
        param.fCollRadius = m_pAtkData->RADIUS * 0.1f;
        param.vPos.y += param.fCollRadius * 0.25f;
        CEntityFactory::getInstance()->createBullet(param, this);
        break;
    case 0x208:
        if (inSkillID == -1) {
            for (i = 0; i < 16; i++) {
                if (m_hMagicBullet[i] <= 0) continue;
                pBullet = (CBullet*)CTaskMan::getTask(m_hMagicBullet[i]);
                if (!pBullet) {
                    m_hMagicBullet[i] = 0;
                    continue;
                }
                if (pBullet->getHitEnemyHandle() <= 0) continue;
                m_hTarget = pBullet->getHitEnemyHandle();
                if (!CTaskMan::getTask(m_hTarget)) m_hTarget = m_hMagicBullet[i];
                setMagic(0x208, 0, m_iStockSkill);
                pBullet->out();
                m_hMagicBullet[i] = 0;
            }
        } else {
            if (m_nMagicBullet >= m_nMagicBulletMax) break;
            CEntity* pEntity = (CEntity*)CTaskMan::getTask(m_hTarget);
            if (!pEntity) pEntity = this;
            INT target[2];
            memset(target, 0, sizeof(target));
            target[0] = LockOnManager::CLockOnManager::getLockOnObject(*pEntity);
            param.init(1, 3);
            param.CollKind = COLL_KIND_PSHOT;
            param.uiFlag = 0x200020;
            param.AtkKind = m_AtkKind;
            param.AtkAttr = m_AtkAttr;
            param.iAttackPoint = m_iAttackPower;
            param.fSpeed = m_pAtkData->MAX_SPEED * 0.01f;
            param.fMaxDist = m_pAtkData->WORK0;
            param.pEffectTbl = (BULLET_EFFECT_INFO*)g_EffTbl_Skill[m_pAtkData->EFFECT];
            param.fCollRadius = m_pAtkData->RADIUS * 0.01f;
            param.tqCollAtkTime = m_pAtkData->COLL_START_FRAME * 100;
            if (pEntity != this) {
                pEntity->getLockOnPos(pos);
            } else {
                m_pWeapon->getEnemyPos(pos);
            }
            param.vPos = pos;
            INT nCollAttackID = m_pWeapon->getCollAttackID();
            for (i = 0; i < 2; i++) {
                if (target[i] > 0 && (pEntity = (CEntity*)CTaskMan::getTask(target[i])) != NULL) {
                    pEntity->getLockOnPos(pos);
                    SubVectorXYZ(rot, pos, param.vPos);
                    Normalize(rot, rot);
                    param.vRot.x = -atan2f(rot.y, _sqrtf(rot.z * rot.z + rot.x * rot.x));
                    param.vRot.y = atan2f(rot.x, rot.z);
                } else {
                    param.vRot.x = _limitPI2(rndArea(0.0f, 0.25f) * -3.1415927f);
                    if (i == 0) {
                        param.vRot.y = _limitPI2(rndArea(0.0f, 2.0f) * 3.1415927f);
                    } else {
                        param.vRot.y = _limitPI2(param.vRot.y + rndArea(0.4f, 0.6f) * 3.1415927f);
                    }
                }
                param.vRot.z = 0.0f;
                pBullet = (CBullet*)CEntityFactory::getInstance()->createBullet(param, this);
                if (!pBullet) continue;
                pBullet->setBodyRadius(0.1f, 0.0f, 0, (COLL_KIND)-1);
                if (!m_iCount || i > 0) {
                    pBullet->setAttackID(nCollAttackID);
                } else {
                    nCollAttackID = pBullet->getAttackID();
                }
                setMagicBullet(pBullet->getHandle());
                if (param.pEffectTbl == g_EffTbl_Skill[m_pAtkData->EFFECT]) {
                    param.pEffectTbl = (BULLET_EFFECT_INFO*)g_EffTbl_Skill[m_pAtkData->EFFECT + 1];
                }
            }
        }
        break;
    case 0x302:
        if (inSkillID != -1) {
            m_hEffAttack[0] = setEffect(g_EffTbl_HitMark[17].pEffectName, 3, &m_vMarkPos, NULL, 0.0f);
            param.init(1, 3);
            param.CollKind = COLL_KIND_PSHOT;
            param.uiFlag = 4;
            param.AtkKind = m_AtkKind;
            param.AtkAttr = m_AtkAttr;
            param.iAttackPoint = m_iAttackPower;
            param.tqHomingTime = 0;
            param.fSpeed = 0.0f;
            param.vPos = m_vMarkPos;
            param.vRot = getRot();
            param.fCollRadius = m_pAtkData->COLL_START_FRAME * 0.01f;
            CEntityFactory::getInstance()->createBullet(param, this);
            setEnemyStop(m_hMark, 1);
        } else {
            setEnemyStop(0, 0);
        }
        break;
    }
}
void CPlayerSora::initMagicBullet() {
    m_nMagicBullet = 0;
    m_nMagicBulletMax = 0;
    for (INT i = 15; i >= 0; i--) {
        m_hMagicBullet[i] = 0;
    }
}
INT CPlayerSora::checkMagicBullet() {
    INT n = 0;
    for (INT i = 0; i < 16; i++) {
        if (m_hMagicBullet[i] > 0) n++;
    }
    return n;
}
void CPlayerSora::setMagicBullet(HANDLE inBullet) {
    for (INT i = 0; i < 16; i++) {
        if (!m_hMagicBullet[i]) {
            m_hMagicBullet[i] = inBullet;
            m_nMagicBullet++;
            break;
        }
    }
}
void CPlayerSora::initFriend(INT inNo) {
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
    switch (m_uiCardID) {
    case 0x31:
        m_uiAttackID = 0x55;
        m_pPLM->setReqLoad(0x44, 0);
        break;
    case 0x32:
        m_uiAttackID = 0x56;
        m_pPLM->setReqLoad(0x47, 0);
        break;
    case 0x33:
        m_uiAttackID = 0x57;
        m_pPLM->setReqLoad(0x4b, 0);
        break;
    case 0x34:
        m_uiAttackID = 0x58;
        m_pPLM->setReqLoad(0x4f, 0);
        break;
    case 0x35:
        m_uiAttackID = 0x59;
        m_pPLM->setReqLoad(0x4d, 0);
        break;
    case 0x36:
        m_uiAttackID = 0x5a;
        m_pPLM->setReqLoad(0x51, 0);
        break;
    case 0x37:
        m_uiAttackID = 0x5b;
        m_pPLM->setReqLoad(0x53, 0);
        break;
    case 0x38:
        m_uiAttackID = 0x5c;
        m_pPLM->setReqLoad(0x62, 0);
        break;
    case 0x39:
        m_uiAttackID = 0x5d;
        m_pPLM->setReqLoad(0x55, 0);
        break;
    default:
        m_iSubState = 3;
        m_pAtkData = &g_AttackDataList[0];
        m_uiAttackID = 0;
        m_uiAttackKind = 0;
        return;
    }
    m_pAtkData = &g_AttackDataList[m_uiAttackID];
    m_uiPreAttackKind = m_uiAttackKind;
    m_fDistXZ = m_fDistY = 0.0f;
}
void CPlayerSora::exeFriend() {
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
        _ref(m_hOldLockon) = m_hLockon;
        _ref(m_iOldCamMode) = PlayerCamera::CPlayerCamera::getNowCamMode();
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
    case 0x1a9:
    case 0x1ae:
    case 0x1b3:
    case 0x1bd:
    case 0x1d1:
        if (!m_pPLM->isFriendReady()) {
            m_iSubState = 3;
            break;
        }
        if (!m_bIsCanCombo) {
            m_bIsCanCombo = m_pPLM->getFriend()->setSkill(m_uiAttackID);
        }
        exeStandUp_inFriend();
        break;
    case 0x1b8:
    case 0x1c2:
    case 0x1cc:
        if (!m_pPLM->isFriendReady()) {
            m_iSubState = 3;
            break;
        }
        if (!m_bIsCanCombo) {
            m_bIsCanCombo = m_pPLM->getFriend()->setSkill(m_uiAttackID);
        }
        watchTarget(0);
        break;
    case 0x1c7:
        if (!m_pPLM->isFriendReady()) {
            m_iSubState = 3;
            break;
        }
        if (!m_bIsCanCombo) {
            m_bIsCanCombo = m_pPLM->getFriend()->setSkill(m_uiAttackID);
        }
        if (isPlayerFlag(0x20000)) {
            if (exeAttack_inFriend()) {
                setPlayerFlag(0, 0x20000);
            }
            break;
        }
        watchTarget(0);
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

/* abs.s of the original (Math3D.h) */
static inline FLOAT _abs(FLOAT inValue) {
    FLOAT ret;
    __asm__ volatile("abs.s %0, %1" : "=f"(ret) : "f"(inValue));
    return ret;
}

void CPlayerSora::watchTarget(INT inMode) {
    CEntity* pTarget;
    FLOAT tmp_dir;
    FLOAT fTurn;
    if (inMode == 1) {
        if (isAir()) {
            setAnim(6, 800, 0);
            FVECTOR vel = m_pMove->getVel();
            m_fJumpSpdY += m_fJumpGravity * m_fFrameRate;
            vel.y = m_fJumpSpdY;
            m_pMove->setVel(vel);
        } else {
            setAnim(0x2A, -1, 0);
            clearSpeed();
        }
    }
    FVECTOR vec;
    switch (inMode) {
    case 0:
        pTarget = m_pPLM->getFriend()->getTarget();
        if (!pTarget) {
            pTarget = m_pPLM->getFriend();
        } else {
            m_hTarget = pTarget->getHandle();
        }
        break;
    case 1:
        pTarget = m_pPLM->getSummon()->getTarget();
        if (!pTarget) {
            pTarget = m_pPLM->getSummon();
        } else {
            m_hTarget = pTarget->getHandle();
        }
        break;
    case 2:
        pTarget = m_pPLM->getSummon();
        clearSpeed();
        break;
    default:
        return;
    }
    pTarget->getLockOnPos(vec);
    SubVectorXYZ(vec, vec, m_vCenterPos);
    Normalize(vec, vec);
    tmp_dir = atan2f(vec.x, vec.z);
    fTurn = RAD2DEG(_abs(_limitPI2(tmp_dir - getDir())));
    m_fDir = Movement::Homing(getDir(), tmp_dir, 0.15f, m_fFrameRate);
    vec.set(0.0f, m_fDir, 0.0f);
    m_pMove->setRot(vec);
    switch (inMode) {
    case 2: {
        FLOAT x = CPadHandler::getAnalogLX() * (1.0f / 4096.0f);
        FLOAT y = CPadHandler::getAnalogLY() * (1.0f / 4096.0f);
        if (_abs(x) > 0.25f) {
            setAnim(0x2B, -1, 0);
        } else {
            setAnim(0x2A, -1, 0);
        }
        break;
    }
    case 0:
        if (isAir()) {
            setAnim(6, 800, 0);
            FVECTOR vel = m_pMove->getVel();
            m_fJumpSpdY += m_fJumpGravity * m_fFrameRate;
            vel.y = m_fJumpSpdY;
            m_pMove->setVel(vel);
        } else {
            if (fTurn > 10.0f) {
                setAnim(0x2B, -1, 0);
            } else {
                setAnim(0x2A, -1, 0);
            }
            clearSpeed();
        }
        break;
    }
}
void CPlayerSora::setAttack_inFriend(INT inID, INT inStock, HANDLE inTarget) {
    CEntity* pTarget;
    FLOAT distY;
    FLOAT dir;
    if (isPlayerFlag(0x20000)) {
        return;
    }
    setPlayerFlag(0x20000, 0);
    _ref(m_bIsAttackHit) = FALSE;
    m_fJumpGravity = g_fGravity * 0.16f;
    m_JumpState = 3;
    m_fJumpSpdY = 0.0f;
    if (inTarget > 0) {
        m_hTarget = inTarget;
    }
    if (m_hTarget > 0) pTarget = (CEntity*)CTaskMan::getTask(m_hTarget);
    else pTarget = NULL;
    distY = 0.0f;
    if (pTarget) {
        pTarget->getLockOnPos(m_vMarkPos);
        distY = m_vMarkPos.y - m_vCenterPos.y;
        distY -= 1.0f;
        calcDistance(*pTarget);
        dir = _abs(calcDirection(*pTarget) - getDir());
        m_hMark = m_hTarget;
        setPlayerFlag(0x100000, 0);
    } else {
        setPlayerFlag(0, 0x100000);
    }
    m_uiAttackID = inID;
    m_pAtkData = &g_AttackDataList[inID];
    if (inID == 5 || inID == 0x67) {
        if (!isAir()) {
            if (distY < 2.0f) m_fJumpSpdY = g_fAntiGravity * (FLOAT)m_pAtkData->WORK1;
            else m_fJumpSpdY = g_fAntiGravity * (distY * 10.0f + 24.0f);
        } else {
            if (0.0f <= distY) m_fJumpSpdY = g_fAntiGravity * (distY * 10.0f + 4.0f);
        }
        procGravity(0, m_pAtkData->MOVE_END_FRAME, 1);
    }
    setAnimForce(m_pAtkData->ANIM_ID, 0, 1, 0, 0, 0);
    if (isConditionFlag(0x100)) {
        setAnimSpeed(2.0f);
    }
    if (!inStock) {
        m_iAttackPower = m_iAP * CardBattle::g_pBattleCardList[m_uiCardID].POWER[m_pAtkData->POWER] / 100;
        _ref(m_AtkKind) = (ATK_KIND)m_pAtkData->DAMAGE;
        _ref(m_AtkAttr) = (ATK_ATTR)CardBattle::g_pBattleCardList[m_uiCardID].ATTR;
        UINT up = _upKind(CardBattle::g_pBattleCardList[m_uiCardID].UP_TYPE0,
                          CardBattle::g_pBattleCardList[m_uiCardID].UP_TYPE1);
        calcEnchant_AttackPoint(m_iAttackPower, m_AtkKind, m_AtkAttr, up);
    } else {
        m_iAttackPower = m_iAP * CardBattle::g_pStockSkillList[m_iStockSkill].POWER[m_pAtkData->POWER] / 100;
        _ref(m_AtkKind) = (ATK_KIND)m_pAtkData->DAMAGE;
        _ref(m_AtkAttr) = (ATK_ATTR)CardBattle::g_pStockSkillList[m_iStockSkill].ATTR;
        UINT up = _upKind(CardBattle::g_pStockSkillList[m_iStockSkill].UP_TYPE0,
                          CardBattle::g_pStockSkillList[m_iStockSkill].UP_TYPE1);
        calcEnchant_AttackPoint(m_iAttackPower, m_AtkKind, m_AtkAttr, up);
    }
    m_fDistXZ = m_fDistY = 0.0f;
    if (m_pAtkData->FLAG & 8) {
        setPlayerFlag(0x2800000, 0);
    } else {
        setPlayerFlag(0, 0x2800000);
        m_fJumpGravity = g_fGravity;
    }
    onWeapon(1);
}
INT CPlayerSora::exeAttack_inFriend() {
    TIMEQ frame = getMotionCurrentTime() / 50;
    procGravity(frame, m_pAtkData->MOVE_END_FRAME, 1);
    if (frame <= m_pAtkData->MOVE_START_FRAME) {
        if (isPlayerFlag(0x100000)) {
            procTargetMark();
        }
        procAttackMove(frame, 1.0f, 4.0f, 2, 1.0f);
    } else {
        FLOAT rate = 0.0f;
        if (frame < 6 && (m_pAtkData->FLAG & 8)) {
            procTargetMark();
            rate = 1.0f;
        }
        procAttackMove(frame, m_pAtkData->RADIUS * 0.1f, m_pAtkData->MAX_SPEED, !m_bIsAttackHit, rate);
    }
    if (!m_bIsAttackHit) {
        if (frame >= m_pAtkData->COLL_START_FRAME) {
            if (frame < m_pAtkData->COLL_END_FRAME) {
                if (!m_pWeapon->isOnAttack()) {
                    onAttack();
                }
                if (m_pWeapon->isHit()) {
                    m_bIsAttackHit = TRUE;
                    setPlayerFlag(0, 0x100000);
                    m_pPLM->getFriend()->setSuccess();
                    offAttack();
                }
            } else {
                offAttack();
                setPlayerFlag(0, 0x100000);
            }
        }
    } else {
        setPlayerFlag(0, 0x2000000);
    }
    if (isPlayerFlag(0x800000) && !isAir() && frame > m_pAtkData->COLL_END_FRAME) {
        return endAttack_inFriend();
    }
    if (isMotionEnd()) {
        return endAttack_inFriend();
    }
    return 0;
}
INT CPlayerSora::endAttack_inFriend() {
    if (!isPlayerFlag(0x20000)) {
        return -1;
    }
    setPlayerFlag(0, 0x20000);
    offAttack();
    m_fWalkSpeed = m_fJumpSpdXZ = m_fMoveSpdXZ;
    if (isAir()) {
        setAnim(6, 0x320, 0);
    } else {
        setAnimForce(0x2A, -1, 0, 0, 0, 0);
    }
    m_uiAttackID = 0;
    if (m_bIsAttackHit) {
        m_bIsAttackHit = FALSE;
        return 1;
    }
    m_bIsAttackHit = FALSE;
    m_pPLM->getFriend()->setFailed();
    return -1;
}
void CPlayerSora::initSummon(INT inNo) {
    setPlayerState(SUMMON);
    m_iSubState = 1;
    clearSpeed();
    offAttack();
    setBattleFlag(0, 8);
    setBattleFlag(0, 0x800);
    setBattleFlag(0, 0x80000);
    setTrgFlag(0, 0x100);
    setPlayerFlag(0, 0x2800000);
    setPlayerFlag(0, 0x8000000);
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
    switch (m_uiCardID) {
    case 0x23:
        setMDFooting(0);
        m_fWallRadius = 1.0f;
        m_uiAttackID = 0x7a;
        m_pPLM->setReqLoad(0x34, 0);
        break;
    case 0x24:
        setMDFooting(0);
        m_fWallRadius = 2.0f;
        m_uiAttackID = 0x7b;
        m_pPLM->setReqLoad(0x3d, 0);
        break;
    case 0x25:
        m_uiAttackID = 0x7c;
        m_pPLM->setReqLoad(0x38, 0);
        break;
    case 0x26:
        setMDFooting(0);
        m_fWallRadius = 0.8f;
        m_uiAttackID = 0x7d;
        m_pPLM->setReqLoad(0x36, 0);
        break;
    case 0x27:
        m_uiAttackID = 0x7e;
        m_pPLM->setReqLoad(0x3f, 0);
        break;
    case 0x28:
        m_vAppearPos = m_vHeadPos;
        m_uiAttackID = 0x7f;
        m_pPLM->setReqLoad(0x3b, 0);
        break;
    case 0x29:
        setMDFooting(0);
        m_fWallRadius = 0.8f;
        m_uiAttackID = 0x80;
        m_pPLM->setReqLoad(0x41, 0);
        break;
    default:
        m_iSubState = 5;
        m_pAtkData = &g_AttackDataList[0];
        m_uiAttackID = 0;
        m_uiAttackKind = 0;
        return;
    }
    m_pAtkData = &g_AttackDataList[m_uiAttackID];
    m_uiPreAttackKind = m_uiAttackKind;
    m_fDistXZ = m_fDistY = 0.0f;
}
void CPlayerSora::exeSummon() {
    TIMEQ frame = getMotionCurrentTime() / 50;
    setBattleFlag(0, 8);
    if (!isConditionFlag(0x1000000)) {
        procGravity(frame, m_pAtkData->MOVE_END_FRAME, 1);
    }
    switch (m_iSubState) {
    case 1:
        if (isAir()) {
            setAnim(6, 800, 0);
            break;
        }
        setBGFade(g_vBGFadeColor[0], 20.0f, 0);
        setAnimForce(0x2e, -1, 1, 0, getStateSESeq(7), 0);
        setCastEffect(g_szDpxSummon, 1, &Crd(0), NULL);
        calcAppearPosFS_toAttackID();
        _ref(m_hOldLockon) = m_hLockon;
        _ref(m_iOldCamMode) = PlayerCamera::CPlayerCamera::getNowCamMode();
        if ((m_pAtkData->FLAG & 0x20) && m_pPLM->isCompleteLoadCard(m_uiCardID) != 1) {
            setObjStop(1);
            m_pCam->changeCamMode(5, 0);
        }
        m_iSubState = 6;
        break;
    case 6:
        if (m_pPLM->isCompleteLoadCard(m_uiCardID) != 1) break;
        if (!m_pPLM->isSummonReady()) {
            calcAppearPosFS_toAttackID();
            m_pPLM->createSummon(m_pPLM->getSummonID_toCard(m_uiCardID), NULL);
            break;
        }
        if (!isMotionLoop()) break;
        setObjStop(0);
        m_pCam->changeCamMode(-1, 0);
        m_pPLM->getSummon()->setAppear();
        fadeEffect(m_hEffCast, 1);
        {
            CEffect* effect = CEffectMan::getEffect(m_hEffCast);
            if (effect) {
                effect->setZSortEntity(m_pPLM->getSummon(), 1, 1);
            }
        }
        switch (m_uiAttackID) {
        case 0x7A:
        case 0x7B:
        case 0x7D:
        case 0x80:
            break;
        default:
            setEffect(g_szDpxSummon, 2, &getPos(), NULL, 0.0f);
            break;
        }
        m_iSubState = m_uiAttackID;
        setAnimForce(0x2a, -1, 0, 0, 0, 0);
        setPlayerFlag(0x100000, 0);
        m_tqStateTimer = 0;
        m_pMarker->holdTarget(8);
        procTargetMark();
        break;
    case 0x7C:
    case 0x7E:
        if (!m_pPLM->isSummonReady()) {
            m_iSubState = 5;
            break;
        }
        if (!m_bIsCanCombo) {
            m_bIsCanCombo = m_pPLM->getSummon()->setSkill(m_uiAttackID);
        }
        exeStandUp_inFriend();
        break;
    case 0x7F:
        if (!m_pPLM->isSummonReady() || isPlayerFlag(0x8000000)) {
            m_iSubState = 3;
            setAnim(0x2A, -1, 0);
            break;
        }
        if (!m_bIsCanCombo) {
            m_bIsCanCombo = m_pPLM->getSummon()->setSkill(m_uiAttackID);
        } else {
            watchTarget(2);
        }
        break;
    case 3:
        if (m_pPLM->isSummonReady()) break;
        m_iSubState = 5;
        setPlayerFlag(0, 0x8000000);
        m_fWallRadius = 0.5f;
        break;
    case 0x7A:
    case 0x7B:
    case 0x7D:
    case 0x80:
        if (!m_pPLM->isSummonReady() || isPlayerFlag(0x8000000)) {
            setPlayerFlag(0x400000, 0x1000);
            m_iSubState = 4;
            if (isAir()) {
                setLand(8.0f, 0.5f, 1);
                updateCoord();
                coordFix();
            }
            exeWarp(0);
            setCastEffect(g_szDpxSummon, 2, &Crd(0), NULL);
            setPlayerFlag(0, 0x8000000);
            if (m_uiAttackID == 0x80) {
                setEffect(g_szDpxCloud, 0, &getPos(), NULL, 0.0f);
            }
            break;
        }
        setPlayerFlag(0x400000, 0);
        if (!exeWarp(1)) break;
        if (!m_bIsCanCombo) {
            m_bIsCanCombo = m_pPLM->getSummon()->setSkill(m_uiAttackID);
            m_pCam->changeNowPlayer(m_pPLM->getSummon()->getHandle(), m_hTarget);
            m_pCam->changeCamMode(-1, 0);
            setPlayerFlag(0x1001, 0);
            setEntityFlag(1, 0);
            offWeapon();
        }
        warp(m_pPLM->getSummon()->getPos());
        setRot(m_pPLM->getSummon()->getRot());
        clearSpeed();
        break;
    case 4:
        setPlayerFlag(0x400000, 0);
        onWeapon(0);
        if (!exeWarp(0)) break;
        if (m_pPLM->isSummonReady()) break;
        setPlayerFlag(0, 0x1001);
        m_iSubState = 5;
        setPlayerFlag(0, 0x8000000);
        m_fWallRadius = 0.5f;
        break;
    case 5:
        if (isBattleFlag(0x1000) && exeUseStock(1)) {
            if (isPlayerFlag(0x80000000)) setBGFade(1.0f, 20.0f, 0);
            break;
        }
        procSkillEnd(-1);
        break;
    }
}
void CPlayerSora::setStateSE(INT inState, INT inKind) {
    INT SEID = -1;
    switch (inState) {
    case 1:
        switch (m_uiAttackID) {
        case 6:
            SEID = 0xcd;
            break;
        case 7:
            SEID = 0xd0;
            break;
        case 8:
            SEID = 0xd4;
            break;
        case 9:
        case 15:
            SEID = 0xd1;
            break;
        case 5:
        case 18:
        case 23:
            SEID = 0xce;
            break;
        case 3:
        case 25:
            SEID = 0xca;
            break;
        case 2:
        case 20:
        case 26:
            SEID = 0xc9;
            break;
        case 4:
        case 21:
        case 27:
            SEID = 0xcb;
            break;
        case 1:
        case 19:
        case 28:
            SEID = 0xc8;
            break;
        case 22:
        case 29:
            SEID = 0xcc;
            break;
        case 78:
            SEID = 0xd0 - (m_nHitCount & 1);
            break;
        case 79:
            SEID = 0xd6;
            break;
        case 103:
        case 115:
            SEID = 0xd3;
            break;
        }
        break;
    case 2:
        SEID = 0xd2;
        break;
    }
    if (SEID != -1) {
        playSEPanAndVol(1, SEID, 0, NULL, 0);
    }
}
INT CPlayerSora::getStateSESeq(INT inState) {
    INT seq = 0;
    switch (inState) {
    case 3:
    case 4:
    case 6:
    case 7:
        switch (m_uiAttackID) {
        case 34:
            break;
        case 35: case 36: case 43: case 44: case 45: case 46: case 47:
            seq = 0;
            break;
        case 83:
            seq = 9;
            break;
        case 86: case 100: case 101: case 102:
            seq = 2;
            break;
        case 39: case 58: case 59: case 87: case 105: case 106:
            seq = 3;
            break;
        case 62: case 63: case 64: case 65: case 66: case 67: case 91: case 113: case 114:
            seq = 5;
            break;
        case 90: case 111: case 112:
            seq = 6;
            break;
        case 89: case 107: case 108:
            seq = 7;
            break;
        case 40: case 60: case 61: case 88: case 109: case 110: case 148:
            seq = 4;
            break;
        case 37: case 38: case 54: case 55: case 56: case 57: case 85: case 94: case 95: case 96: case 149:
            seq = 1;
            break;
        case 93: case 104: case 120: case 121: case 122: case 123: case 124: case 125: case 126: case 127:
        case 128: case 129: case 130: case 131: case 132: case 133: case 134: case 135: case 136: case 137:
        case 138: case 139: case 143: case 144: case 145: case 146: case 147: case 150:
            seq = 8;
            break;
        }
        break;
    }
    return seq;
}
void CPlayerSora::initGlide(INT inFlag) {
    if (isBattleFlag(0x200000)) {
        return;
    }
    setPlayerState(PL::GLIDE);
    setBattleFlag(0, 0x8);
    setPlayerFlag(0, 0x1000000);
    if (isActionFlag(0x800000)) {
        setAnimForce(0x1C, -1, 0, 0, 0, 0);
    } else {
        setAnimForce(0x1B, -1, 0, 0, 0, 0);
    }
    m_JumpState = 4;
    m_fJumpSpdY = 0.0f;
    m_fJumpGravity = _ref(g_fGlideWeight) * -0.008166667f;
    m_fJumpSpdXZ = m_fWalkSpeed;
    m_fJumpPow = 0.0f;
    m_fJumpDir = getDir();
    offAttack();
    setBattleFlag(0, 0x00443F90);
    setBattleFlag(0, 0x80000);
    setBattleFlag(0, 0x10200000);
    setPlayerFlag(0, 0x020C2000);
    setBattleFlag(0, 0x4060);
    setBattleFlag(0, 0x3000000);
    setFaceAnim(0, 0);
    m_pMove->setFlag(0x40, 0);
}
/* Parked: only the analog-steering block after Homing differs: the original loads m_pMove (lw v1,0xa0) before
   the y store into the {0, dir, 0, 0} temporary at sp+0x20 and copies it (lq/sq to sp+0x10) after the m_fDir
   store; ours stores y and loads the temp first (its stack-temp accesses conflict with this-> accesses, the
   original's do not) - 4 lines. tried: decl scopes/stack-slot order (vel/vNormal/v), brace init (memset: FVECTOR
   union is BLKmode here), explicit temp block, _ref/pointer/array-element/scalar-pointer stores, d0 as Homing
   result vs wrapped angle, setDir before/after/inside the temp block, getRot() = v, early m_pMove local,
   -dS sched dump (dependence 496 -> 469/484 in ours); permuter 6 + 12 min, best 4 lines (start 8) */
#if 0
void CPlayerSora::exeGlide() {
    FLOAT speed;
    setBattleFlag(0, 8);
    if (m_JumpState == 6) {
        setGroundIK(1);
        exeJumpLand();
        return;
    }
    if (isPlayerFlag(0x200000)) {
        initFall();
        return;
    }
    if (m_pMove->getCollisionFlag() & 0xA) {
        FVECTOR vNormal = m_pMove->getWallNormal();
        m_fWallDir = _limitPI2(atan2f(vNormal.x, vNormal.z));
        if (_abs(_limitPI2(m_fWallDir - m_fDir)) > 1.5707964f) {
            initFall();
            return;
        }
    }
    if (!isDownJump()) {
        initFall();
        return;
    }
    speed = isActionFlag(0x800000) ? g_fGlideSpeedHigh : g_fGlideSpeedLow;
    FVECTOR vel = m_pMove->getVel();
    m_fJumpSpdY += m_fJumpGravity * m_fFrameRate;
    vel.y = m_fJumpSpdY;
    m_pMove->setVel(vel);
    if (m_fAnalogPower >= 0.3f) {
        FLOAT d0 = _limitPI2(m_fAnalogViewDir - getDir());
        setDir(Movement::Homing(getDir(), m_fAnalogViewDir, g_fGlideTurnRate, m_fFrameRate));
        FVECTOR v;
        {
            /* the original's brace-initialized temporary ({0, dir, 0, 0}, cleared as one quadword) */
            FVECTOR t;
            t._q = 0;
            t.y = getDir();
            v = t;
        }
        m_pMove->setRot(v);
        if (getHoukouFromAngle(d0) != 4) {
            if (d0 < 0.0f) {
                setAnim(0x1D, -1, 0);
            } else {
                setAnim(0x1E, -1, 0);
            }
        } else if (isActionFlag(0x800000)) {
            setAnim(0x1C, -1, 0);
        } else {
            setAnim(0x1B, -1, 0);
        }
    } else if (isActionFlag(0x800000)) {
        setAnim(0x1C, -1, 0);
    } else {
        setAnim(0x1B, -1, 0);
    }
    m_fWalkSpeed = speed / 60.0f;
    m_fJumpSpdXZ = Movement::Approach(m_fJumpSpdXZ, m_fWalkSpeed, 0.1f, m_fFrameRate);
    m_fWalkSpeed = m_fJumpSpdXZ;
    m_pMove->calcVelocityXZ(m_fWalkSpeed);
    if (!isAir()) {
        m_JumpState = 6;
        setAnimForce(7, -1, 0, 0, 0, 0);
        m_fJumpSpdXZ = 0.0f;
        clearSpeed();
        m_fJumpSpdY = 0.0f;
        m_fJumpGravity = 0.0f;
        return;
    }
    m_fJumpHeight = getPos().y - m_vJumpStartPos.y;
    if (m_fJumpTop < m_fJumpHeight) m_fJumpTop = m_fJumpHeight;
    m_tqJumpTimer += m_tqFrameRate;
}
#else
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Game/Player/PlayerSora", exeGlide__Q22PL11CPlayerSora);
#endif
void CPlayerSora::exeAvoid() {
    exeDogeRoll();
}
void CPlayerSora::exeDogeRoll() {
    setBattleFlag(0, 0x8);
    FVECTOR vel = m_pMove->getVel();
    if (isAir()) {
        m_fJumpSpdY += m_fJumpGravity * m_fFrameRate;
        vel.y = m_fJumpSpdY;
        m_pMove->setVel(vel);
    } else {
        if (m_fJumpSpdY < 0.0f) m_fJumpSpdY = 0.0f;
        vel.y = m_fJumpSpdY;
        m_pMove->setVel(vel);
    }
    if (getMotionCurrentTime() >= m_pParam->AVOID_TQ_MUTEKI) {
        setPlayerFlag(0, 1);
    }
    if (!isAir() && getMotionCurrentTime() >= m_pParam->AVOID_TQ_CANCEL && m_fAnalogPower >= 0.3f) {
        m_WalkMode = chkWalkMode();
        setWalkAnim();
        switch (m_WalkMode) {
        case 0:
            setPlayerState(PL::STAND);
            break;
        case 1:
            setPlayerState(PL::WALK);
            break;
        case 2:
            setPlayerState(PL::RUN);
            break;
        }
        m_fMovePower = 1.0f;
        m_fWalkSpeed = m_fWalkSpeed / 60.0f;
        setPlayerFlag(0, 1);
        offAttack();
        setBattleFlag(0, 0x00443F90);
        setBattleFlag(0, 0x80000);
        setBattleFlag(0, 0x10200000);
        setPlayerFlag(0, 0x024C2000);
        setConditionFlag(0, 0x08810023);
        setBattleFlag(0, 0x4060);
        setBattleFlag(0, 0x3000000);
        m_JumpState = 0;
        m_fJumpSpdY = 0.0f;
        setFaceAnim(0, 0);
        return;
    }
    if (isMotionEnd()) {
        initIdle(-1, 1);
    }
}
/* Game/Collision/CollBody.h (not decompiled yet): CPlayer's m_collBody */
struct _CollBodyView {
    UBYTE _m_0x00[0x8];
    INT m_Enable; /* 0x08 */
    void off() { m_Enable = 0; }
};

void CPlayerSora::initPronounce(INT inFlag) {
    TRACE("CPlayerSora:: initPronounce(%d)\n", inFlag);
    setBattleFlag(0, 0x8);
    clearSpeed();
    if (!inFlag) {
        setOldPos(m_vCenterPos);
        initStandUp(0);
        return;
    }
    setPlayerState((PL::PlayerState)0x24);
    m_iSubState = 1; // setSubState(1): defined in Player.cpp only
    fadeEffect(m_hEffBad, 1);
    setAnimRsrcEx(0xFFA, 0, 0, 1, 0, 0, 1);
    m_JumpState = 3;
    m_fJumpGravity = 0.0f;
    m_fJumpSpdY = 0.0f;
    setOldPos(m_vCenterPos);
    setTrgFlag(0, 0x8000);
    setLand(5.0f, 1.0f, 1);
    setBattleFlag(0, 0x8);
    setBattleFlag(0, 0x00443F90);
    setBattleFlag(0, 0x80000);
    setBattleFlag(0, 0x10200000);
    setPlayerFlag(0, 0x20000);
    setPlayerFlag(1, 0);
    setBattleFlag(0, 0x4060);
    setBattleFlag(0, 0x3000000);
    ((_CollBodyView*)m_collBody)->off();
    m_hMark = m_hTarget = m_pCB->getDuelEnemy();
    PlayerCamera::CPlayerCamera::setBattleCamData(getHandle(), &g_PronounceCamParam);
    m_pCam->changeNowPlayer(getHandle(), m_hMark);
    m_pCam->changeCamMode(4, 0);
    m_uiStep = 0;
    setPlayerFlag(0x10000, 0);
    setTimer(3000);
    m_iCount = 7;
    m_pCB->setPronounceStart(0);
}
void CPlayerSora::exePronounce() {
    TIMEQ time;
    setBattleFlag(0, 8);
    if (isMotionEnd()) {
        setAnim(0x2A, -1, 0);
    }
    if (isTrgAnyPad()) {
        if (m_pAnim->getAnimID()) {
            setAnimRsrcEx(0xFFA, 0, -1, 0, 0, 0, 1);
        }
    }
    switch (m_iSubState) {
    case 1: {
        setPos(m_vEnemyPos);
        m_fDir = Movement::Homing(m_fDir, m_fStartDir, 0.5f, getFrameRateF());
        FVECTOR rot = getRot();
        rot.y = m_fDir;
        setRot(rot);
        if (!m_hEffBad) {
            m_hEffBad = setEffect(g_szDpxMarluxia3, 0x14, &getPos(), NULL, 0.0f);
        }
        if (getTimer() > 0) break;
        if (m_pCB->setPronounceStart(1)) {
            m_iSubState = 2;
        }
        break;
    }
    case 2:
        if (!m_pCB->isNowPronounce()) break;
        m_iSubState = 4;
        m_iCount--;
        setCastEffect(g_szDpxMarluxia3, m_iCount + 0x15, &Crd(0), (FVECTOR*)&g_vPronounceCountOfs);
        time = m_pCB->getSubTimer();
        if (!time) {
            m_pCB->setSubTimer(15000);
            time = 15000;
        }
        setTimer(time - 1500);
        setPlayerFlag(0, 1);
        break;
    case 3:
        break;
    case 4:
    case 5:
    case 6:
    case 7:
    case 8:
    case 9:
        if (m_uiStep) {
            if (getTimer() <= 0) {
                m_iSubState++;
                m_iCount--;
                setCastEffect(g_szDpxMarluxia3, m_iCount + 0x15, &Crd(0), (FVECTOR*)&g_vPronounceCountOfs);
                setTimer(m_pCB->getSubTimer() - 1500);
                m_uiStep = 0;
                break;
            }
        } else if (getTimer() <= 0) {
            m_uiStep = 1;
            setTimer(1500);
            fadeEffect(m_hEffCast, 1);
        }
        if (isBattleFlag(0x2000000)) {
            initStandUp(0);
            fadeEffect(m_hEffBad, 1);
            playSEPanAndVol(0x10F, 0x2D, 0, NULL, 0);
            fadeEffect(m_hEffCast, 0);
            PlayerCamera::CPlayerCamera::setBattleCamData(0, NULL);
            m_pCam->changeNowPlayer(0, 0);
            m_pCam->changeCamMode(-1, 0);
            setPlayerFlag(0, 0x10000);
            break;
        }
        if (m_iCount < 2 && m_uiStep) {
            if (getTimer() < 600) {
                TRACE("CPlayerSora:: exePronounce() %d,%d,%d \x81\xa6\x83\x4d\x83\x8a\x83\x4d\x83\x8a\x8d\xc5\x8c\xe3\x82\xcc\x93\xfc\x97\xcd\x96\xb3\x8c\xf8\n", m_iCount, m_uiStep, getTimer());
                break;
            }
        }
        if (isTrgFlag(0x10)) {
            if (chkChargeMove(1)) break;
        }
        if (isBattleFlag(0x20)) {
            setBattleFlag(0, 0x20);
            break;
        }
        if (!isTrgAttack()) break;
        if (m_pRevolver->setPlayerPronounceCard(1) == 1) {
            TRACE("CPlayerSora:: exePronounce() TRG!\n");
            setBattleFlag(0, 0x20);
        } else {
            TRACE("CPlayerSora:: exePronounce() TRG...REFRECT\n");
            setBattleFlag(0, 0x10200000);
            setBattleFlag(0x20, 0);
        }
        break;
    case 10:
        if (!m_uiStep) {
            m_uiStep = 1;
            m_pCB->setPronounceEnd(m_iCount);
        }
        if (getTimer() > 0) break;
        m_iSubState = 11;
        fadeEffect(m_hEffCast, 1);
        m_iCount = -1;
        PlayerCamera::CPlayerCamera::setBattleCamData(0, NULL);
        m_pCam->changeNowPlayer(0, 0);
        m_pCam->changeCamMode(-1, 0);
        setPlayerFlag(0, 0x10000);
        break;
    case 11:
    case 12:
        break;
    }
}
void CPlayerSora::calcAppearPosFS_toAttackID() {
    switch (m_uiAttackID) {
    case 104:
        calcAppearPosFS(0.0f, m_fWallRadius);
        break;
    case 85: case 86: case 87: case 88: case 89: case 90: case 91: case 93: case 94: case 95: case 96:
    case 100: case 101: case 102: case 105: case 106: case 107: case 108: case 109: case 110: case 111:
    case 112: case 113: case 114: case 120: case 121:
        calcAppearPosFS(2.0f, 0.55f);
        break;
    case 124: case 133: case 134: case 135:
        calcAppearPosFS(1.0f, 0.55f);
    default:
        calcAppearPosFS(0.0f, m_fWallRadius);
        break;
    }
}
// SOURCE DATA: .data .rodata
