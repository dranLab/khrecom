#define GAME_PLAYER_PLAYER_CPP /* MATCH: unit gate (its header gives this unit its own parts); without it the unit compiles only against its full header (tested 2026-11-02) */

#include "khtypes.h"
#include "Game/Collision/CollInfo.h"









/* .rodata starts with the strings of the included headers' inline functions, in this order. */
#include "System/Graphic/Gprim.h"
#include "System/Graphic/Model.h"
#include "System/Graphic/Anim.h"
/* System/TaskMan.h's container overflow message comes after Anim.h's strings here. */
#include "System/TaskMan.h"


#include "System/Graphic/2d/2DMan.h"
#include "Game/CardBattle.h"
#include "System/RsrcData.h"
#include "Game/Collision/CollUtil.h"
/* one empty string follows CollUtil.h's strings */
inline void _emptyStringPlaceholder0() { /* MATCH: placeholder: an inline function's strings, in the original .rodata order */
    TRACE("");
}
#include "System/Graphic/Str.h"
#include "Game/Enemy/BattleEnemyManager.h"
#include "System/Graphic/Effect/effect.h"
#include "Game/Card.h"
#include "Game/Status.h"
#include "Game/Collision/CollBody.h"
#include "System/Fsm.h"
#include "Game/Player/Player.h"
#include "Game/Player/PlayerManager.h"
#include "System/Debug.h"
#include "Game/Player/PlayerGauge.h"
#include "Game/Player/PlayerWeapon.h"
#include "Game/Debug/DebugWork.h"
#include "Game/Movement.h"
#include "Game/Status.h"
#include "Game/SceneEnv.h"
#include "Game/Bullet/BulletManager.h"
#include "System/TaskMan.h"
#include "Game/SEMotion.h"
#include "System/Graphic/Effect/effect.h"
#include "System/Coord.h"
#include "Game/Player/PlayerAnim.h"
#include "Game/GameWork.h"
#include "Form/Menu/WorldSelect.h"
#include "Game/Player/PlayerCamera.h"
#include "Game/SceneInfo.h"
#include "Game/Gimmick/BaseGimmick.h"
#include "Game/Collision/CollMap.h"
#include "System/HeapMan.h"
#include "Game/Enemy/BattleEnemyManager.h"
#include "Game/CardKind.h"
#include "System/Graphic/Camera.h"
#include "System/Graphic/WorldEnv.h"
#include "Game/Game.h"
#include "Game/Bullet/Bullet.h"
#include "Game/EntityFactory.h"
#include "System/Math3D.h"
#include "System/Rnd.h"
#include "Game/Player/PlayerAttack.h"
#include "System/Graphic/Anim.h"
#include "Game/AttachMotion.h"
#include "Game/LockOnManager.h"
#include "System/Graphic/Model.h"
#include "Game/Gimmick/GimmickCarry.h"
#include "Game/Gimmick/GimmickTreasureBox.h"
#include "Game/Gimmick/GimmickDoor.h"
#include "Form/Menu/RoomCreation.h"
#include "Form/DebugWindow.h"
#include "Game/Player/DWTPlayer.h"
#include "include_asm.h"

void ApplyMatrixXYZ(FVECTOR& v0, const FMATRIX& m0, const FVECTOR& v1); void ApplyMatrix(FVECTOR& v0, const FMATRIX& m0, const FVECTOR& v1);
void AddVectorXYZ(FVECTOR& v0, const FVECTOR& v1, const FVECTOR& v2);
void SubVectorXYZ(FVECTOR& v0, const FVECTOR& v1, const FVECTOR& v2);
void Normalize(FVECTOR& outV, const FVECTOR& inV);
FLOAT InnerProduct(const FVECTOR& inV0, const FVECTOR& inV1);
template <class T> static inline T& _ref(T& v) { return v; } static inline UINT _upKind(INT inType0, INT inType1) { UINT up = inType0 > 0 ? 1 << (inType0 - 1) : 0; if (inType1 > 0) up |= 1 << (inType1 - 1); return up; }
static inline FLOAT _abs(FLOAT inValue) {
    FLOAT ret;
    __asm__ volatile("abs.s %0, %1" : "=f"(ret) : "f"(inValue));
    return ret;
}
static inline FLOAT _sqrtf(FLOAT x) {
    FLOAT ret;
    asm volatile("sqrt.s %0, %1" : "=f"(ret) : "f"(x));
    return ret;
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
extern "C" float atan2f(float y, float x); extern "C" int rand();
static inline FLOAT _limitPI(FLOAT r) {
    if (r < -3.1415927f) r += 6.2831855f;
    else if (r > 3.1415927f) r -= 6.2831855f;
    return r;
}
/* the same wrap with a separate result variable (the original's mov.s copy into a second register) */
static inline FLOAT _limitPI2(FLOAT d) {
    FLOAT r;
    if (d < -3.1415927f) r = d + 6.2831855f;
    else {
        r = d;
        if (r > 3.1415927f) r -= 6.2831855f;
    }
    return r;
}
void ScaleVectorXYZ(FVECTOR& v0, const FVECTOR& v1, FLOAT f);
static inline void _addAssign(FVECTOR& a, const FVECTOR& b) {
    a.x += b.x;
    a.y += b.y;
    a.z += b.z;
    a.w += b.w;
}


#include "System/RsrcDB.h"



namespace PL {
extern BULLET_EFFECT_INFO g_EffTbl_Skill[67][4]; // PlayerAttack.h in the original
extern INT g_CardBreakTime[8]; // PlayerAttack.h in the original
}
class CPlayerDebug : public CTask { /* main() is still INCLUDE_ASM; the members are those the constructor touches */
    CDebugWindow* m_pWin; PCAM_DATA* m_pCamData; PATK_DATA* m_pAtkData; INT m_iPrevState; INT m_iPrevMuteki; INT m_iPrevHP; CHAR m_szBuf[6][0x20];
public: CPlayerDebug(CTask* inParent); virtual ~CPlayerDebug();
    virtual void main();
};
#include "Form/Menu/JMemoFlag.h"
#include "Game/Player/Friend.h"
#include "Game/Player/Summon.h"
#include "Game/EffectMotion.h"
#include "Game/MinigameManager.h"
#include "System/Sound/SoundMan.h"
#line 188

CPlayer* g_pPlayer = NULL;
CPlayer* CPlayer::m_pInstance = NULL;
namespace PL {
static LPCSTR g_szDpxAsiato = "asiato";
static LPCSTR g_szDpxSpeedBreak = "speed_break";
static LPCSTR g_szDpxEX1580 = "ex_1580";
static LPCSTR g_szDpxVexen = "vexen";
static LPCSTR g_szDpxAbnormal = "abnormal";
static LPCSTR g_szDpxAlways = "always";
static LPCSTR g_szDpxItemplus = "itemplus";
static LPCSTR g_szDpxReload = "reload";
static LPCSTR g_szDpxZexion = "zexion";
static LPCSTR g_szDpxWarpOut = "warp_out";
static LPCSTR g_szDpxCureAll = "cure_all";
static LPCSTR g_szDpxBreak = "break";
static LPCSTR g_szDpxHitWall = "rec_wall";
static LPCSTR g_szBoneWep0 = "wep0";
static LPCSTR g_szBoneEx1580sk = "ex_1580_sk";
static LPCSTR g_szBoneEx1010sk = "ex_1010_sk";
static LPCSTR g_szBoneEx0010sk = "ex_0010_sk";
static LPCSTR g_szBoneChest1 = "chest1";
static LPCSTR g_szBoneWeaponR = "WeaponR";
static LPCSTR g_szBoneHead = "head";
static LPCSTR g_szBoneLToes = "l_toes";
static LPCSTR g_szBoneRToes = "r_toes";
static LPCSTR g_szBoneLHand = "l_hand";
static LPCSTR g_szTexEyeDamage = "EYE_DAMAGE";
static LPCSTR g_szTexEyeShut = "EYE_SHUT";
static LPCSTR g_szTexEyeKizetsu = "EYE_KIZETSU";
static LPCSTR g_szTexLipDamage = "LIP_DAMAGE";
static LPCSTR g_szTexLipDefault = "LIP_DEFAULT";
FVECTOR g_vFieldPlayerPos = {{ 0.0f, 0.0035f, 0.0f, 180.0f }};
FVECTOR g_vBattlePlayerPos = {{ 0.0f, 0.0035f, 8.0f, 180.0f }};
FVECTOR g_vDummyPlayerPos = {{ 0.0f, -1000.0f, 0.0f, 1.0f }};
FVECTOR g_vBanishPos = {{ 0.0f, -1000.0f, 0.0f, 1.0f }};
UINT g_uiStartCondition = 0;
FLOAT g_fWeight = 0.6f;
FLOAT g_fJumpStopRate = 0.43f;
FLOAT g_fGlideWeight = 0.01f;
FLOAT g_fGlideSpeedLow = 5.0f;
FLOAT g_fGlideSpeedHigh = 8.0f;
FLOAT g_fGlideTurnRate = 0.02f;
PL_PARAM g_Param = { 135801, 601, 0.25f, 1.0f, 1.3f, 6.2f, 1.8f, 28.0f, 32.5f, 100, 600, 1400 };
INT g_AttackResponse[6] = { 0, 400, 500, 600, 750, 900 };
FLOAT g_fDefAttr[18][6] = {
    { 100.0f, 100.0f, 100.0f, 100.0f, 100.0f, 100.0f },
    { 100.0f, 100.0f, 1000.0f, 100.0f, 50.0f, 100.0f },
    { 100.0f, 100.0f, 50.0f, 1000.0f, 100.0f, 100.0f },
    { 100.0f, 100.0f, 50.0f, 50.0f, 50.0f, 100.0f },
    { 100.0f, 100.0f, 0.0f, 1000.0f, 100.0f, 100.0f },
    { 100.0f, 150.0f, 100.0f, 100.0f, 0.0f, 100.0f },
    { 100.0f, 100.0f, 1000.0f, 0.0f, 100.0f, 100.0f },
    { 150.0f, 50.0f, 50.0f, 50.0f, 50.0f, 100.0f },
    { 100.0f, 100.0f, 50.0f, 50.0f, 50.0f, 100.0f },
    { 50.0f, 150.0f, 100.0f, 0.0f, 100.0f, 100.0f },
    { 100.0f, 50.0f, 50.0f, 50.0f, 50.0f, 100.0f },
    { 100.0f, 50.0f, 50.0f, 50.0f, 50.0f, 100.0f },
    { 100.0f, 50.0f, 50.0f, 50.0f, 50.0f, 100.0f },
    { 100.0f, 50.0f, 50.0f, 50.0f, 50.0f, 100.0f },
    { 100.0f, 50.0f, 50.0f, 50.0f, 50.0f, 100.0f },
    { 100.0f, 50.0f, 100.0f, 0.0f, 50.0f, 100.0f },
    { 100.0f, 50.0f, 50.0f, 50.0f, 50.0f, 100.0f },
    { 100.0f, 100.0f, 100.0f, 100.0f, 100.0f, 100.0f }
};
FVECTOR g_vBGFadeColor[7] = {
    {{ 0.5f, 0.5f, 0.5f, 1.0f }},
    {{ 0.5f, 0.5f, 0.75f, 1.0f }},
    {{ 0.75f, 0.5f, 0.6f, 1.0f }},
    {{ 0.6f, 0.5f, 0.75f, 1.0f }},
    {{ 0.75f, 0.75f, 0.6f, 1.0f }},
    {{ 0.5f, 0.75f, 0.5f, 1.0f }},
    {{ 0.3f, 0.25f, 0.375f, 1.0f }}
};
}
void (CPlayer::*CPlayer::m_pBeginStateTbl[4])() = { &CPlayer::beginStateField_, &CPlayer::beginStateBattle_, &CPlayer::beginStateWait_, &CPlayer::beginStateHide_ };
void (CPlayer::*CPlayer::m_pStateTbl[4])() = { &CPlayer::stateField_, &CPlayer::stateBattle_, &CPlayer::stateWait_, &CPlayer::stateHide_ };
void (CPlayer::*CPlayer::m_pEndStateTbl[4])() = { &CPlayer::endStateField_, &CPlayer::endStateBattle_, &CPlayer::endStateWait_, &CPlayer::endStateHide_ };
static const CARD_DATA cardDataTbl[224] = {
    { { 0x00 }, 0, 0, 0, "" }, // 0
    { { 0x11 }, 2, 0, 3, "Kingdom Key" }, // 1
    { { 0x11 }, 2, 1, 3, "Three Wishes" }, // 2
    { { 0x11 }, 2, 2, 3, "Crabclaw" }, // 3
    { { 0x11 }, 2, 3, 3, "Pumpkinhead" }, // 4
    { { 0x11 }, 2, 4, 3, "Fairy Harp" }, // 5
    { { 0x11 }, 2, 5, 3, "Wishing Star" }, // 6
    { { 0x11 }, 2, 6, 3, "Spellbinder" }, // 7
    { { 0x11 }, 2, 7, 3, "Metal Chocobo" }, // 8
    { { 0x11 }, 3, 0, 3, "Olympia" }, // 9
    { { 0x11 }, 3, 1, 3, "Lionheart" }, // 10
    { { 0x11 }, 3, 2, 3, "Lady Luck" }, // 11
    { { 0x11 }, 3, 3, 3, "Divine Rose" }, // 12
    { { 0x11 }, 3, 4, 3, "Oathkeeper" }, // 13
    { { 0x11 }, 3, 5, 3, "Oblivion" }, // 14
    { { 0x11 }, 3, 6, 3, "Ultima Weapon" }, // 15
    { { 0x11 }, 3, 7, 3, "Diamond Dust" }, // 16
    { { 0x11 }, 4, 0, 3, "One-Winged Angel" }, // 17
    { { 0x11 }, 4, 1, 4, "Soul Eater" }, // 18
    { { 0x11 }, 4, 2, 3, "Star Seeker" }, // 19
    { { 0x11 }, 4, 3, 3, "Monochrome" }, // 20
    { { 0x11 }, 4, 4, 3, "Follow the Wind" }, // 21
    { { 0x11 }, 4, 5, 3, "Hidden Dragon" }, // 22
    { { 0x11 }, 4, 6, 3, "Photon Debugger" }, // 23
    { { 0x11 }, 4, 7, 3, "Bond of Flame" }, // 24
    { { 0x11 }, 26, 0, 0, "-" }, // 25
    { { 0x22 }, 5, 0, 3, "Fire" }, // 26
    { { 0x22 }, 5, 1, 3, "Blizzard" }, // 27
    { { 0x22 }, 5, 2, 3, "Thunder" }, // 28
    { { 0x22 }, 5, 3, 3, "Cure" }, // 29
    { { 0x22 }, 5, 4, 3, "Gravity" }, // 30
    { { 0x22 }, 5, 5, 3, "Stop" }, // 31
    { { 0x22 }, 5, 6, 3, "Aero" }, // 32
    { { 0x22 }, 26, 2, 0, "-" }, // 33
    { { 0x22 }, 26, 1, 0, "-" }, // 34
    { { 0x33 }, 5, 7, 3, "Simba" }, // 35
    { { 0x33 }, 6, 0, 3, "Genie" }, // 36
    { { 0x33 }, 6, 1, 3, "Bambi" }, // 37
    { { 0x33 }, 6, 2, 3, "Dumbo" }, // 38
    { { 0x33 }, 6, 3, 3, "Tinker Bell" }, // 39
    { { 0x33 }, 6, 4, 3, "Mushu" }, // 40
    { { 0x33 }, 6, 5, 3, "Cloud" }, // 41
    { { 0x44 }, 6, 6, 6, "Potion" }, // 42
    { { 0x44 }, 6, 7, 6, "Hi-Potion" }, // 43
    { { 0x44 }, 7, 0, 2, "Mega-Potion" }, // 44
    { { 0x44 }, 7, 1, 2, "Ether" }, // 45
    { { 0x44 }, 7, 2, 2, "Mega-Ether" }, // 46
    { { 0x44 }, 7, 3, 2, "Elixir" }, // 47
    { { 0x44 }, 7, 4, 2, "Megalixir" }, // 48
    { { 0x55 }, 7, 5, 2, "Donald Duck" }, // 49
    { { 0x55 }, 7, 6, 2, "Goofy" }, // 50
    { { 0x55 }, 7, 7, 2, "Aladdin" }, // 51
    { { 0x55 }, 8, 0, 2, "Ariel" }, // 52
    { { 0x55 }, 8, 1, 2, "Jack" }, // 53
    { { 0x55 }, 8, 2, 2, "Peter Pan" }, // 54
    { { 0x55 }, 8, 3, 2, "The Beast" }, // 55
    { { 0x55 }, 8, 4, 4, "The King" }, // 56
    { { 0x55 }, 8, 5, 2, "Pluto" }, // 57
    { { 0x66 }, 8, 6, 6, "Guard Armor" }, // 58
    { { 0x66 }, 8, 6, 6, "Trickmaster" }, // 59
    { { 0x66 }, 8, 6, 6, "Jafar" }, // 60
    { { 0x66 }, 8, 6, 6, "Oogie Boogie" }, // 61
    { { 0x66 }, 8, 6, 6, "Parasite Cage" }, // 62
    { { 0x66 }, 8, 6, 6, "Ursula" }, // 63
    { { 0x66 }, 8, 6, 6, "Dragon Maleficent" }, // 64
    { { 0x66 }, 8, 6, 6, "Darkside" }, // 65
    { { 0xe6 }, 8, 7, 4, "-" }, // 66
    { { 0xe6 }, 9, 0, 0, "-" }, // 67
    { { 0x77 }, 9, 1, 6, "Shadow" }, // 68
    { { 0x77 }, 9, 2, 2, "Soldier" }, // 69
    { { 0x77 }, 9, 3, 6, "Large Body" }, // 70
    { { 0x77 }, 9, 4, 2, "Red Nocturne" }, // 71
    { { 0x77 }, 9, 5, 2, "Blue Rhapsody" }, // 72
    { { 0x77 }, 9, 6, 2, "Yellow Opera" }, // 73
    { { 0x77 }, 9, 7, 2, "Green Requiem" }, // 74
    { { 0x77 }, 10, 0, 6, "Powerwild" }, // 75
    { { 0x77 }, 10, 1, 2, "Bouncywild" }, // 76
    { { 0x77 }, 10, 2, 2, "Air Soldier" }, // 77
    { { 0x77 }, 10, 3, 2, "Bandit" }, // 78
    { { 0x77 }, 10, 4, 6, "Fat Bandit" }, // 79
    { { 0x77 }, 10, 5, 2, "Barrel Spider" }, // 80
    { { 0x77 }, 10, 6, 6, "Search Ghost" }, // 81
    { { 0x77 }, 10, 7, 6, "Sea Neon" }, // 82
    { { 0x77 }, 11, 0, 2, "Screwdiver" }, // 83
    { { 0x77 }, 11, 1, 2, "Aquatank" }, // 84
    { { 0x77 }, 11, 2, 6, "Wight Knight" }, // 85
    { { 0x77 }, 11, 3, 2, "Gargoyle" }, // 86
    { { 0x77 }, 11, 4, 6, "Pirate" }, // 87
    { { 0x77 }, 11, 5, 2, "Air Pirate" }, // 88
    { { 0x77 }, 11, 6, 4, "Darkball" }, // 89
    { { 0x77 }, 11, 7, 6, "Defender" }, // 90
    { { 0x77 }, 12, 0, 2, "Wyvern" }, // 91
    { { 0x77 }, 12, 1, 2, "Wizard" }, // 92
    { { 0x77 }, 12, 2, 2, "Neoshadow" }, // 93
    { { 0x77 }, 12, 3, 2, "White Mushroom" }, // 94
    { { 0x77 }, 12, 4, 2, "Black Fungus" }, // 95
    { { 0x77 }, 12, 5, 2, "Creeper Plant" }, // 96
    { { 0x77 }, 12, 6, 2, "Tornado Step" }, // 97
    { { 0x77 }, 12, 7, 2, "Crescendo" }, // 98
    { { 0x77 }, 13, 0, 6, "Guard Armor" }, // 99
    { { 0x77 }, 13, 1, 6, "Parasite Cage" }, // 100
    { { 0x77 }, 13, 2, 6, "Trickmaster" }, // 101
    { { 0x77 }, 13, 3, 6, "Darkside" }, // 102
    { { 0x77 }, 13, 4, 2, "Card Soldier" }, // 103
    { { 0x77 }, 13, 5, 6, "Hades" }, // 104
    { { 0x77 }, 13, 6, 6, "Jafar" }, // 105
    { { 0x77 }, 13, 7, 6, "Oogie Boogie" }, // 106
    { { 0x77 }, 14, 0, 6, "Ursula" }, // 107
    { { 0x77 }, 14, 1, 6, "Hook" }, // 108
    { { 0x77 }, 14, 2, 6, "Dragon Maleficent" }, // 109
    { { 0x77 }, 14, 3, 2, "Riku" }, // 110
    { { 0x77 }, 14, 4, 2, "Axel" }, // 111
    { { 0x77 }, 14, 5, 2, "Larxene" }, // 112
    { { 0x77 }, 14, 6, 2, "Vexen" }, // 113
    { { 0x77 }, 14, 7, 2, "Marluxia" }, // 114
    { { 0x77 }, 15, 0, 6, "Lexaeus" }, // 115
    { { 0x77 }, 15, 1, 2, "Ansem" }, // 116
    { { 0x77 }, 15, 3, 6, "Zexion" }, // 117
    { { 0x77 }, 15, 4, 2, "Xemnas" }, // 118
    { { 0x77 }, 15, 5, 2, "Xigbar" }, // 119
    { { 0x77 }, 15, 6, 2, "Xaldin" }, // 120
    { { 0x77 }, 15, 7, 2, "Sa\203\307x" }, // 121
    { { 0x77 }, 16, 0, 2, "Demyx" }, // 122
    { { 0x77 }, 16, 1, 2, "Luxord" }, // 123
    { { 0x77 }, 16, 2, 2, "Roxas" }, // 124
    { { 0x77 }, 21, 6, 2, "Gold Card" }, // 125
    { { 0x77 }, 21, 7, 2, "Platinum Card" }, // 126
    { { 0x77 }, 15, 2, 0, "Card Soldier Spade" }, // 127
    { { 0x77 }, 13, 4, 0, "Card Soldier Heart" }, // 128
    { { 0x17 }, 22, 0, 0, "\203N\203\211\203E\203hA" }, // 129
    { { 0x17 }, 22, 1, 0, "\203N\203\211\203E\203hB" }, // 130
    { { 0x17 }, 22, 2, 0, "\203n\203f\203XA" }, // 131
    { { 0x27 }, 22, 3, 0, "\203n\203f\203XB" }, // 132
    { { 0x17 }, 22, 4, 0, "\203t\203b\203NA" }, // 133
    { { 0x17 }, 22, 5, 0, "\203t\203b\203NB" }, // 134
    { { 0x17 }, 22, 6, 0, "\203t\203b\203NC" }, // 135
    { { 0x27 }, 22, 7, 0, "\203t\203b\203ND" }, // 136
    { { 0x17 }, 23, 0, 0, "\203A\203N\203Z\203\213A" }, // 137
    { { 0x17 }, 23, 1, 0, "\203A\203N\203Z\203\213B" }, // 138
    { { 0x17 }, 23, 2, 0, "\203\211\203N\203V\201[\203kA" }, // 139
    { { 0x17 }, 23, 3, 0, "\203\211\203N\203V\201[\203kB" }, // 140
    { { 0x27 }, 23, 4, 0, "\203\211\203N\203V\201[\203kC" }, // 141
    { { 0x17 }, 23, 5, 0, "\203\212\203NA" }, // 142
    { { 0x17 }, 23, 6, 0, "\203\212\203NB" }, // 143
    { { 0x17 }, 23, 7, 0, "\203\212\203NC" }, // 144
    { { 0x17 }, 24, 0, 0, "\203\212\203ND" }, // 145
    { { 0x17 }, 24, 1, 0, "\203\224\203B\203N\203Z\203\223A" }, // 146
    { { 0x27 }, 24, 2, 0, "\203\224\203B\203N\203Z\203\223B" }, // 147
    { { 0x17 }, 24, 3, 0, "\203}\201[\203\213\201[\203V\203\203A" }, // 148
    { { 0x17 }, 24, 4, 0, "\203}\201[\203\213\201[\203V\203\203B" }, // 149
    { { 0x27 }, 24, 5, 0, "\203}\201[\203\213\201[\203V\203\203C" }, // 150
    { { 0x77 }, 24, 6, 0, "\203}\201[\203\213\201[\203V\203\2032" }, // 151
    { { 0x77 }, 24, 7, 0, "\203}\201[\203\213\201[\203V\203\2033" }, // 152
    { { 0x17 }, 25, 0, 0, "\203\214\203N\203Z\203E\203XA" }, // 153
    { { 0x17 }, 25, 1, 0, "\203\214\203N\203Z\203E\203XB" }, // 154
    { { 0x27 }, 25, 2, 0, "\203\214\203N\203Z\203E\203XC" }, // 155
    { { 0x17 }, 25, 3, 0, "\203A\203\223\203Z\203\200A" }, // 156
    { { 0x27 }, 25, 4, 0, "\203A\203\223\203Z\203\200B" }, // 157
    { { 0x27 }, 25, 5, 0, "\203[\203N\203V\203I\203\223A" }, // 158
    { { 0x77 }, 26, 3, 0, "-" }, // 159
    { { 0x88 }, 0, 0, 6, "\203\212\203\215\201[\203h" }, // 160
    { { 0x99 }, 16, 3, 6, "Tranquil Darkness" }, // 161
    { { 0x99 }, 16, 4, 6, "Teeming Darkness" }, // 162
    { { 0x99 }, 16, 5, 6, "Feeble Darkness" }, // 163
    { { 0x99 }, 16, 6, 6, "Almighty Darkness" }, // 164
    { { 0x99 }, 16, 7, 6, "Sleeping Darkness" }, // 165
    { { 0x99 }, 17, 0, 6, "Looming Darkness" }, // 166
    { { 0x99 }, 17, 1, 2, "Premium Room" }, // 167
    { { 0x99 }, 17, 2, 2, "White Room" }, // 168
    { { 0x99 }, 17, 3, 6, "Black Room" }, // 169
    { { 0x99 }, 17, 4, 6, "Bottomless Darkness" }, // 170
    { { 0x99 }, 17, 5, 6, "Roulette Room" }, // 171
    { { 0xa9 }, 17, 6, 6, "Martial Waking" }, // 172
    { { 0xa9 }, 17, 7, 2, "Sorcerous Waking" }, // 173
    { { 0xa9 }, 18, 0, 2, "Alchemic Waking" }, // 174
    { { 0xa9 }, 18, 1, 6, "Meeting Ground" }, // 175
    { { 0xa9 }, 18, 2, 6, "Stagnant Space" }, // 176
    { { 0xa9 }, 18, 3, 6, "Strong Initiative" }, // 177
    { { 0xa9 }, 18, 4, 6, "Lasting Daze" }, // 178
    { { 0xb9 }, 18, 5, 2, "Calm Bounty" }, // 179
    { { 0xb9 }, 18, 6, 2, "Guarded Trove" }, // 180
    { { 0xb9 }, 18, 7, 2, "False Bounty" }, // 181
    { { 0xb9 }, 19, 0, 6, "Moment's Reprieve" }, // 182
    { { 0xb9 }, 19, 1, 6, "Mingling Worlds" }, // 183
    { { 0xb9 }, 19, 2, 2, "Moogle Room" }, // 184
    { { 0xb9 }, 19, 3, 6, "Random Joker" }, // 185
    { { 0xca }, 19, 4, 6, "Key of Beginnings" }, // 186
    { { 0xca }, 19, 5, 6, "Key of Guidance" }, // 187
    { { 0xca }, 19, 6, 6, "Key to Truth" }, // 188
    { { 0xca }, 19, 7, 2, "Key to Rewards" }, // 189
    { { 0xdb }, 20, 0, 6, "Traverse Town" }, // 190
    { { 0xdb }, 20, 3, 6, "Agrabah" }, // 191
    { { 0xdb }, 20, 2, 6, "Olympus Coliseum" }, // 192
    { { 0xdb }, 20, 1, 6, "Wonderland" }, // 193
    { { 0xdb }, 20, 5, 6, "Monstro" }, // 194
    { { 0xdb }, 20, 4, 6, "Halloween Town" }, // 195
    { { 0xdb }, 20, 6, 6, "Atlantica" }, // 196
    { { 0xdb }, 20, 7, 6, "Neverland" }, // 197
    { { 0xdb }, 21, 0, 6, "Hollow Bastion" }, // 198
    { { 0xdb }, 21, 1, 2, "100 Acre Woods" }, // 199
    { { 0xdb }, 21, 2, 6, "Twilight Town" }, // 200
    { { 0xdb }, 21, 3, 6, "Destiny Islands" }, // 201
    { { 0xdb }, 21, 4, 4, "Castle Oblivion" }, // 202
    { { 0x0d }, 0, 0, 0, "EXP\220\302\217\254" }, // 203
    { { 0x0d }, 0, 1, 0, "EXP\220\302\222\206" }, // 204
    { { 0x0d }, 0, 2, 0, "EXP\220\302\221\345" }, // 205
    { { 0x0d }, 0, 3, 0, "EXP\220\324\217\254" }, // 206
    { { 0x0d }, 0, 4, 0, "EXP\220\324\222\206" }, // 207
    { { 0x0d }, 0, 5, 0, "EXP\220\324\221\345" }, // 208
    { { 0x0d }, 0, 6, 0, "EXP\213\342" }, // 209
    { { 0x0d }, 1, 0, 0, "HP\217\254" }, // 210
    { { 0x0d }, 1, 1, 0, "HP\221\345" }, // 211
    { { 0x0d }, 2, 0, 0, "MP\217\254" }, // 212
    { { 0x0d }, 2, 1, 0, "MP\221\345" }, // 213
    { { 0x0d }, 3, 0, 0, "P" }, // 214
    { { 0x0d }, 4, 0, 0, "R" }, // 215
    { { 0x0c }, 0, 0, 0, "Unknown Room" }, // 216
    { { 0x0c }, 0, 0, 0, "Conqueror's Respite" }, // 217
    { { 0x0c }, 0, 0, 0, "Room of Rewards" }, // 218
    { { 0x0c }, 0, 0, 0, "" }, // 219
    { { 0x0c }, 0, 0, 0, "" }, // 220
    { { 0x00 }, 0, 0, 0, "" }, // 221
    { { 0x00 }, 0, 0, 0, "" }, // 222
    { { 0x00 }, 0, 0, 0, "" }, // 223
};

namespace PL {
static const FLOAT g_fGimmickJumpPower[8] = { 39.0f, 45.9f, 52.2f, 57.6f, 62.7f, 67.3f, 71.6f, 75.8f };
static const FVECTOR LADDER_LOCAL_OFS = {{ 0.0f, 0.0f, 0.84000003f, 1.0f }};
static const FVECTOR HANG_HAND_LOCAL_OFS = {{ 0.0f, 1.2f, 0.11f, 1.0f }};
FLOAT g_fGravity = g_fWeight * -0.008166667f;
FLOAT g_fAntiGravity = g_fWeight * 0.008166667f;
}
char* szAtkAnimID[71];
/* instantiates CStateTemplate<CPlayer> (the original's CPlayer constructor does): vtable and weak functions at the end */
inline void _stateTemplateInstance() {
    CStateTemplate<CPlayer> t;
}
FVECTOR getPlayerPos(BOOL flag) {
    if (g_pPlayer) {
        if (!flag && g_pPlayer->isConditionFlag(0x40)) return PL::g_vBanishPos;
        return g_pPlayer->getPos();
    }
    return PL::g_vDummyPlayerPos;
}
INT CPlayer::calcNextEXP(INT inLv) {
    INT nExp = 25;
    for (INT i = 3; i <= inLv + 1; i++) {
        nExp += i * i * 4;
    }
    return nExp;
}
INT CPlayer::getNowNextEXP() {
    INT next;
    if (m_pInstance) {
        next = calcNextEXP(m_pInstance->getLV());
        __asm__ __volatile__("" : : : "memory"); /* matching lever: the original re-reads m_pInstance after the const call */
        return next - m_pInstance->getEXP();
    } else {
        next = calcNextEXP(CStatus::getLV());
        return next - CStatus::getEXP();
    }
}
INT CPlayer::getNowTotalMP() {
    if (!m_pInstance) return CStatus::getMP();
    return m_pInstance->m_iMP;
}
void CPlayer::enablePadControl(INT inFlag) {
    if (m_pInstance) {
        UINT bit = 1;
        m_pInstance->m_uiPadFlag = (m_pInstance->m_uiPadFlag | inFlag) & ~(inFlag ^ bit);
    }
}
void CPlayer::enableDbgPadControl(INT inFlag) {
    if (m_pInstance) {
        UINT bit = 2;
        inFlag <<= 1;
        m_pInstance->m_uiPadFlag = (m_pInstance->m_uiPadFlag | inFlag) & ~(inFlag ^ bit);
    }
}
__asm__(".rdata\n");
CPlayer::CPlayer(CTask* inParent, UINT inChara, INT inEx) : CEntity(inParent, (ObjType::eObjType)1), m_uiCharaID(inChara), m_iState(0) {
    m_pDebug = NULL;
    declareSysTask(3);
    _init();
    m_uiRCID = PLM::g_uiPlayerRcid[m_uiCharaID];
    m_pParam = &PL::g_Param;
    if (!m_pParam) AssertApp(__FILE__, 0x240, "m_pParam", 0);
    if (inEx) {
        m_uiPlayerFlag |= 0x20000000;
    } else {
        g_pPlayer = this;
        m_pInstance = this;
    }
    initalize(1, m_uiRCID);
    initFSM_();
    m_pPLM = (PLM::CPlayerManager*)CTaskMan::getSysTask(0x1F);
    if (m_pPLM) m_pPLM->notifyPlayer(this, inEx);
    m_bIsBattleScene = CSceneInfo::isBattleScene();
    m_bIsMinigameScene = CSceneInfo::isMinigameScene();
    m_bIsBattleScene |= m_bIsMinigameScene;
    if (CSceneInfo::getCurrentWorldID(0) == 10) m_pMove->setFlag(0x80, 0);
}
CPlayer::~CPlayer() {
    if (this == *(CPlayer**)&g_pPlayer) exportStatus();
    fadeEffect(m_hEffPrize, 0);
    fadeEffect(m_hEffCharge, 0);
    fadeEffect(m_hEffCast, 0);
    fadeEffect(m_hEffBad, 0);
    fadeEffAttack(0);
    if (!m_bIsBattleScene) {
        FVECTOR pos = getPos();
        switch (m_PlayerState) {
        case HANG:
        case WALL:
        case LADDER:
            pos = m_vOldSafeField;
            break;
        default:
            break;
        }
        PL::g_vFieldPlayerPos.x = pos.x;
        PL::g_vFieldPlayerPos.y = pos.y;
        PL::g_vFieldPlayerPos.z = pos.z;
        PL::g_vFieldPlayerPos.w = m_fDir / 3.1415927f * 180.0f;
        PL::g_uiStartCondition = m_uiConditionFlag & 8;
    } else {
        if (m_pGauge) m_pGauge->setBattleEnd();
        if (isBattleFlag(2)) {
            setBattleFlag(0, 2);
            m_tqEscape = 0;
            setBGFade(1.0f, 0.0f, 3);
            CSceneEnv::setStop((ObjType::eObjBit)0);
            CGameWork::setInt(1, 0xE2, 0);
        }
        PL::g_uiStartCondition = 0;
    }
    if ((INT)m_uiPlayerFlag < 0) setBGFade(1.0f, 0.0f, 0);
    if (this == m_pInstance) m_pInstance = NULL;
    if (this == g_pPlayer) g_pPlayer = NULL;
}
void CPlayer::initFSM_() {
    for (INT i = 0; i < 4; i++) {
        m_State[i].set(this, m_pBeginStateTbl[i], m_pStateTbl[i], m_pEndStateTbl[i]);
    }
    m_iState = -1;
}
static inline void _setCard(CARD& c, USHORT kind, BYTE num) {
    c.m_nId = 0;
    c.m_nKind2 = kind;
    c.m_nNumber2 = num;
    c.m_nCategory20 = cardDataTbl[c.m_nKind].nCategory0;
    c.m_nCategory21 = cardDataTbl[c.m_nKind].nCategory1;
}
static inline void _setCardNK(CARD& c, USHORT kind, BYTE num) {
    c.m_nId = 0;
    c.m_nNumber2 = num;
    c.m_nKind2 = kind;
    c.m_nCategory20 = cardDataTbl[c.m_nKind].nCategory0;
    c.m_nCategory21 = cardDataTbl[c.m_nKind].nCategory1;
}
void CPlayer::_init() {
    INT i;
    m_fAnalogPower = 0.0f;
    m_fAnalogDir = 0.0f;
    m_fAnalogViewDir = 0.0f;
    m_iMaxHP = 300;
    m_iHP = 300;
    m_iMaxCP = 275;
    m_iCP = 0;
    m_iAP = 10;
    m_iEXP = 0;
    m_iLV = 1;
    m_iMP = 0;
    m_iMaxDP = 8;
    m_iDP = 0;
    m_bIsBattleScene = FALSE;
    m_iBoneWeapon = 0;
    m_iBoneCenter = 0;
    m_iBoneChest = 0;
    m_iBoneHead = 0;
    m_iBoneLFoot = 0;
    m_iBoneRFoot = 0;
    m_pPLM = NULL;
    m_pAtkData = NULL;
    m_pWeapon = NULL;
    m_pCam = NULL;
    m_pGauge = NULL;
    m_pCB = NULL;
    m_pRevolver = NULL;
    m_pMarker = NULL;
    m_pEscape = NULL;
    m_PrePlayerState = INIT;
    m_PlayerState = INIT;
    m_tqStateTimer = 0;
    m_iSubState = 0;
    m_uiPlayerFlag = 0;
    m_uiActionFlag = 0;
    m_uiConditionFlag = 0;
    m_uiTrgFlag = 0;
    m_uiBattleFlag = 0;
    m_uiStep = 0;
    m_iCount = 0;
    m_tqTime = 0;
    m_tqFrameRate = getFrameRate();
    m_fFrameRate = getFrameRateF();
    m_fWeight = *(FLOAT*)&PL::g_fWeight;
    m_fGravity = 0.0f;
    m_iPreAnimID = 0;
    m_iNowAnimID = 0;
    m_iAerialAnimShift = 0;
    m_fAnimSpeed = 1.0f;
    m_iNowFaceID = 0;
    m_nVoiceID = 0;
    i = 3;
    do {
        m_hEffAttack[i] = 0;
    } while (--i >= 0);
    m_szEffectName = NULL;
    m_vWeaponTopPos.init();
    m_vCenterPos.init();
    m_vStartPos.init();
    m_fStartDir = 0.0f;
    m_fDir = 0.0f;
    m_fScale = 1.0f;
    m_fWalkSpeed = 0.0f;
    m_fMoveSpdXZ = 0.0f;
    m_fMovePower = 0.0f;
    m_vOldSafeField.init();
    m_vAppearPos.init();
    m_vHeadPos.init();
    m_vEnemyPos.init();
    m_pCollMap = NULL;
    m_bAirCheck = FALSE;
    m_bAir = FALSE;
    m_bPreAir = FALSE;
    m_fWallRadius = 0.5f;
    m_fFootRadius = 0.25f;
    m_fBodyRviseCoefficient = 0.5f;
    m_fPrizeDraw = 0.0f;
    m_fCardDraw = 0.0f;
    m_hEffPrize = 0;
    m_hEffLevelUp = 0;
    m_tqPrize = 0;
    m_WalkMode = WALKMODE_STAND;
    m_tqEscape = 0;
    m_vWallPos.init();
    m_fWallDir = 0.0f;
    m_pLadderPos = NULL;
    m_pLadderNormal = NULL;
    m_vLadderPos.init();
    m_fLadderDir = 0.0f;
    m_iLadderStep = 0;
    m_iLadderStepMax = 0;
    m_fLadderSpdY = 0.0f;
    m_fJumpDir = 0.0f;
    m_fJumpSpdXZ = 0.0f;
    m_fJumpPow = 0.0f;
    m_fJumpSpdY = 0.0f;
    m_fJumpGravity = 0.0f;
    m_tqJumpTimer = 0;
    m_vJumpStartPos.init();
    m_fJumpHeight = 0.0f;
    m_fJumpTop = 0.0f;
    m_fGimmickJumpPow = 0.0f;
    m_hGimmickJump = 0;
    m_fJumpLandLimit = -100.0f;
    m_hFooting = 0;
    m_hSnatch = 0;
    m_vHangHandPos.init();
    m_vHangPos.init();
    m_uiAttackKind = 0;
    m_uiPreAttackKind = 0;
    m_uiAttackID = 0;
    m_iAttackNo = 0;
    m_iMaxCombo = 3;
    m_bIsAttackHit = FALSE;
    m_bNextTrg = FALSE;
    m_iNextAttack = 0;
    m_bIsCanCombo = FALSE;
    m_tqAttackTimer = 0;
    m_tqAttackResponse = 0;
    m_tqCardBreakTime = 0;
    m_iAttackPower = 0;
    m_iComboBoost = 0;
    m_AtkKind = (ATK_KIND)0;
    m_AtkAttr = (ATK_ATTR)0;
    m_fDistXZ = 0.0f;
    m_fDistY = 0.0f;
    m_hGimmick = 0;
    m_bIsDead = FALSE;
    m_bDmgMuteki = FALSE;
    m_bDmgInAir = FALSE;
    m_tqMutekiTimer = 0;
    m_tqBanish = 0;
    m_tqConfuse = 0;
    m_tqBarrier = 0;
    m_tqGuard = 0;
    m_hEffBad = 0;
    m_hEffGuard = 0;
    m_uiCardID = 0;
    m_iCardNum = -1;
    _setCardNK(((CARD*)m_recvCardInfo)[0], 0, 0);
    _setCardNK(((CARD*)m_recvCardInfo)[1], 0, 0);
    _setCardNK(((CARD*)m_recvCardInfo)[2], 0, 0);
    *(SHORT*)(m_recvCardInfo + 0x1c) = 0;
    m_recvCardInfo[0x1e] = 0;
    m_recvCardInfo[0x1f] = 0;
    m_recvCardInfo[0x20] = 0;
    m_recvCardInfo[0x21] = 0;
    m_recvCardID = 0;
    m_recvCardNum = -1;
    m_iStockSkill = 0;
    m_iExtraSkill = 0;
    m_iFirstSkill = 0;
    m_iStepStockSkill = 0;
    m_tqRevolverRoll = 0;
    m_tqRevolverSpeed = 0;
    m_hEffCharge = 0;
    m_tqEffCharge = 0;
    m_hTarget = 0;
    m_hLockon = 0;
    m_hMark = 0;
    m_vMarkPos.init();
    m_vLockonPos.init();
    m_hOldLockon = 0;
    m_iOldCamMode = 0;
    m_hEffCast = 0;
    m_hFriend = 0;
    m_hSummon = 0;
}
void CPlayer::updateBefore() {
    *(BOOL*)&m_bPreAir = m_bAir;
    m_fWeight = PL::g_fWeight;
    PL::g_fAntiGravity = PL::g_fWeight * 0.008166667f;
    if (CardBattle::CCardBattle::isLevitate()) m_fWeight *= 0.5f;
    PL::g_fGravity = m_fWeight * -0.008166667f;
}
void CPlayer::updateAfter() {}
void CPlayer::lastUpdate() {
    updateBodyColl();
    m_hGimmickJump = 0;
    updateFooting();
}
__asm__(".rdata\n");
void CPlayer::changeState(INT inState) {
    if (!((UINT)inState < 4)) AssertApp(__FILE__, 941, "0 <= inState && inState < STATE_MAX", 0);
    gotoState(m_State[inState]);
}
void CPlayer::stateInitial() {
    setPlayerState(PL::INIT);
    m_uiStep = 0;
    importStatus();
    setDefAttr(PL::g_fDefAttr[0]);
    m_fPrizeDraw = 5.0f;
    m_fCardDraw = 5.0f;
    createLegIK(-1.0f, -1.0f);
    setFootEffectIK(PL::g_szDpxAsiato, 0);
    setGroundIK(1);
    makeAnimIndex(m_uiCharaID);
    initStartPos();
    makeWeapon();
    if (!isPlayerFlag(0x20000000)) makeCockpit();
    if (!m_bIsBattleScene) {
        setAnimForce(1, 0, 0, 0, 0, 0);
    } else {
        setAnimForce(0x2A, 0, 0, 0, 0, 0);
        if (!isPlayerFlag(0x20000000)) makeRevolver();
    }
    initActionSkill();
    if (CGameWork::getFlag(1, 0)) changeState(3);
}
void CPlayer::initStartPos() {
    FVECTOR v;
    if (!m_bIsBattleScene) {
        if (m_pPLM) m_pPLM->calcFieldStartPos();
        m_vStartPos.set(PL::g_vFieldPlayerPos.x, PL::g_vFieldPlayerPos.y, PL::g_vFieldPlayerPos.z);
        m_fStartDir = PL::g_vFieldPlayerPos.w * 0.017453292f;
        setOldPos(m_vStartPos);
        setPos(m_vStartPos);
        m_fDir = m_fStartDir;
        changeState(0);
        m_pMove->setFlag(2, 0);
    } else {
        if (m_bIsMinigameScene) {
            m_vStartPos.set(-19.54f, 3.01f, -8.96f);
            m_fStartDir = -1.2217305f;
        } else {
            m_vStartPos.set(PL::g_vBattlePlayerPos.x, PL::g_vBattlePlayerPos.y, PL::g_vBattlePlayerPos.z);
            m_fStartDir = PL::g_vBattlePlayerPos.w * 0.017453292f;
        }
        setOldPos(m_vStartPos);
        setPos(m_vStartPos);
        m_fDir = m_fStartDir;
        changeState(1);
        setBattleFlag(1, 0);
    }
    {
        FVECTOR r;
        r._q = 0;
        r.y = m_fDir;
        v = r;
    }
    m_pMove->getRot() = v;
    updateCoord();
    coordFix();
    m_pMove->setFlag(0x40, 0);
    m_pMove->setFlag(0x10, 0);
    m_pMove->setFootRadius(0.25f);
    m_pMove->setWallRadius(0.5f);
    m_pMove->land(5.0f, 1.0f, 0);
    m_vCenterPos = Crd(m_iBoneCenter).getWorldMatrix().v[3];
    m_vHeadPos = Crd(m_iBoneHead).getWorldMatrix().v[3];
    m_vJumpStartPos = getPos();
    m_pCollMap = m_pMove->getMapCollision();
    m_collBody.init((COLL_KIND)0, (COLL_TYPE)4, getHandle(), 0);
    m_collBody.setRadius(m_pParam->BODY_COLL_RADIUS);
    m_collBody.off();
}
void CPlayer::makeWeapon() {
    switch (m_uiCharaID) {
    case 5:
    case 6:
        setPlayerFlag(0, 0x40000000);
        m_iBoneWeapon = getBoneIndex(PL::g_szBoneWep0, 0);
        m_iBoneCenter = getBoneIndex(*(LPCSTR*)&PL::g_szBoneEx1580sk, 0);
        m_iBoneChest = getBoneIndex(*(LPCSTR*)&PL::g_szBoneChest1, 0);
        break;
    case 3:
    case 4:
        setPlayerFlag(0, 0x40000000);
        m_iBoneWeapon = getBoneIndex(PL::g_szBoneWep0, 0);
        m_iBoneCenter = getBoneIndex(*(LPCSTR*)&PL::g_szBoneEx1010sk, 0);
        m_iBoneChest = getBoneIndex(*(LPCSTR*)&PL::g_szBoneChest1, 0);
        break;
    case 1:
    case 2:
    default:
        setPlayerFlag(0x40000000, 0);
        m_iBoneWeapon = getBoneIndex(PL::g_szBoneWeaponR, 0);
        m_iBoneCenter = getBoneIndex(*(LPCSTR*)&PL::g_szBoneEx0010sk, 0);
        m_iBoneChest = getBoneIndex(*(LPCSTR*)&PL::g_szBoneChest1, 0);
        break;
    }
    m_iBoneHead = getBoneIndex(*(LPCSTR*)&PL::g_szBoneHead, 0);
    m_iBoneLFoot = getBoneIndex(*(LPCSTR*)&PL::g_szBoneLToes, 0);
    m_iBoneRFoot = getBoneIndex(*(LPCSTR*)&PL::g_szBoneRToes, 0);
    m_pWeapon = new (__FILE__ "(1094)") PlayerWeapon::CPlayerWeapon(this, m_uiCharaID, m_iBoneWeapon);
    offWeapon();
}
__asm__(".rdata\n");
void CPlayer::makeCockpit() {
    m_pCam = new (__FILE__ "(1103)") PlayerCamera::CPlayerCamera(this);
    m_pMarker = new (__FILE__ "(1106)") Gauge::CLockonMarker(this, FALSE);
    if (m_bIsBattleScene) {
        m_pEscape = new (__FILE__ "(1112)") Gauge::CEscapeGauge(this);
    }
    m_pGauge = new (__FILE__ "(1117)") Gauge::CPlayerGauge(this);
    m_pGauge->setMaxHP(m_iMaxHP);
    m_pGauge->setHP(m_iHP, 1);
    m_pGauge->setDP(m_iDP, 30);
    m_pDebug = new (__FILE__ "(1124)") CPlayerDebug(this);
}
__asm__(".rdata\n");
void CPlayer::makeRevolver() {
    if (!m_pRevolver) {
        CARD* pDeck = NULL;
        CARD* deck = CCard::getCurrentDeck();
        if (!DebugWork::g_dw.player.iUseDbgDeck) pDeck = deck;
        if (CGameWork::getFlag(1, 0x2C)) pDeck = NULL;
        if (m_bIsMinigameScene) pDeck = NULL;
        if (!pDeck) {
            m_pRevolver = new (__FILE__ "(1156)") CardBattle::CCardRevolver(this, NULL, 1, -1);
        } else {
            INT max = CCard::getCurrentDeckMax();
            INT sc = CCard::getCurrentShortCut();
            if (dbgCheckUser(KKIKUCHI) && sc == -1) sc = DebugWork::g_dw.player.iTemp;
            deck = new (__FILE__ "(1172)") CARD[max + 1];
            for (INT i = 0; i < max; i++) {
                deck[i] = pDeck[i];
                if (deck[i].m_nCategory1 == 7) deck[i].m_nNumber = -1;
            }
            _setCard(deck[max], 0xFF, 0);
            m_pRevolver = new (__FILE__ "(1181)") CardBattle::CCardRevolver(this, deck, 1, sc);
            if (deck) com_free(deck);
        }
    }
    m_pCB = (CardBattle::CCardBattle*)CTaskMan::getSysTask(0x16);
}
void CPlayer::initActionSkill() {
    UINT uiActionSkill = CGameWork::getInt(0, 0x24);
    if (uiActionSkill & 1) setActionFlag(0x200000, 0);
    else setActionFlag(0, 0x200000);
    DebugWork::g_dw.player.iHiJump = isActionFlag(0x200000) != 0;
    if (CGame::isSoraGame()) {
        if (uiActionSkill & 4) setActionFlag(0x800000, 0);
        else setActionFlag(0, 0x800000);
        if (uiActionSkill & 2) setActionFlag(0x400000, 0);
        else setActionFlag(0, 0x400000);
        DebugWork::g_dw.player.iGlide = isActionFlag(0x400000) != 0;
        DebugWork::g_dw.player.iGlideSP = isActionFlag(0x800000) != 0;
    }
    setActionFlag(0, 0x4000000);
    if (CGame::isRikuGame() && m_bIsBattleScene) {
        if ((CGameWork::getInt(0, 0x24) & 8) && !m_pPLM->isZexionBattle()) setActionFlag(0x4000000, 0);
    }
    if (!m_bIsBattleScene) {
        if (m_pPLM->isTraverseTown()) setActionFlag(0x10000, 0);
        else setActionFlag(0, 0x10000);
    }
    if (CSceneInfo::getCurrentWorldID(0) == 10) {
        setActionFlag(0, 0x20);
    } else if (m_bIsBattleScene && CGameWork::getFlag(1, 0x31)) {
        setActionFlag(0x20, 0);
    }
}
INT CPlayer::importStatus() {
    if (!_instance()) return 0;
    _instance()->setMaxHP(CStatus::getStaus().HPMax);
    _instance()->setHP(CStatus::getStaus().HP);
    _instance()->setAP(CStatus::getStaus().AP);
    _instance()->setEXP(CStatus::getStaus().EXP);
    _instance()->setLV(CStatus::getStaus().LV);
    _instance()->setMP(CStatus::getStaus().MP);
    _instance()->setMaxDP(CStatus::getStaus().DPMax);
    return 1;
}
INT CPlayer::exportStatus() {
    if (!_instance()) return 0;
    CStatus::setHPMax(_instance()->m_iMaxHP);
    CStatus::setHP(_instance()->m_iHP);
    CStatus::setAP(_instance()->m_iAP);
    CStatus::setEXP(_instance()->m_iEXP);
    CStatus::setMP(_instance()->m_iMP);
    return 1;
}
void CPlayer::setPlayerState(PlayerState inState) {
    m_PrePlayerState = m_PlayerState;
    m_PlayerState = inState;
    m_iSubState = 0;
    m_tqStateTimer = 0;
}
void CPlayer::makeAnimIndex(INT inChara) {
    if (inChara) {
        MOT_NAME_TBL* tbl = PL_ANIM::g_MotNameTbl[inChara];
        LPCSTR file = PL_ANIM::g_pPlayerModelName[inChara];
        while (tbl->pFileName) {
            tbl->iIndex = CRsrcDB::getMotionIdxFromMotName(file, tbl->pFileName, NAME_CASE_SENSITIVE);
            tbl++;
        }
    }
}
__asm__(".rdata\n");
INT CPlayer::getMotIndex(INT inChara, INT inIndex) {
    if (!(0 <= inIndex && inIndex < ANIM_MAX)) AssertApp(__FILE__, 1361, "0<=inIndex && inIndex<ANIM_MAX", 0);
    return PL_ANIM::g_MotNameTbl[inChara][inIndex].iIndex;
}
void CPlayer::setAnimForce(INT inAnimID, TIMEQ inChangeTime, BOOL inRootMove, INT inEffSeq, INT inSESeq, TIMEQ inStart) {
    m_iPreAnimID = m_iNowAnimID;
    m_iNowAnimID = inAnimID;
    INT animID = getMotIndex(m_uiCharaID, inAnimID);
    changeMotion(animID, inStart, inRootMove, inChangeTime, inEffSeq, inSESeq);
    if (isPlayerFlag(0x40000000)) {
        m_pWeapon->changeMotion(animID, inStart, FALSE, inChangeTime, inEffSeq, inSESeq);
    }
    setAnimSpeed(1.0f);
}
void CPlayer::setAnim(INT inID, INT inTq, INT inGr) {
    if (m_iNowAnimID != inID) setAnimForce(inID, inTq, inGr, 0, 0, 0);
}
void CPlayer::setAnimSpeed(FLOAT inSpeed) {
    if (m_fAnimSpeed != inSpeed) {
        m_fAnimSpeed = inSpeed;
        CEntity::setAnimSpeed(inSpeed);
        if (isPlayerFlag(0x40000000)) {
            m_pWeapon->setAnimSpeed(m_fAnimSpeed);
        }
    }
}
void CPlayer::setAnimSkillEx(INT inAnimID, TIMEQ inChangeTime, BOOL inRootMove, INT inEffSeq, INT inSESeq) {
    UINT rcid = m_pPLM->getRCID_toSkill(m_iStockSkill);
    if (rcid) {
        m_iNowAnimID = -1;
        changeMotionEx(rcid + 10000, inAnimID, 0, inRootMove, inChangeTime, inEffSeq, inSESeq);
        if (isPlayerFlag(0x40000000)) {
            m_pWeapon->changeMotionEx(rcid + 11000, inAnimID, 0, FALSE, inChangeTime, inEffSeq, inSESeq);
        }
        setAnimSpeed(1.0f);
    }
}
void CPlayer::setAnimRsrcEx(INT inRcid, INT inAnimID, TIMEQ inChangeTime, BOOL inRootMove, INT inEffSeq, INT inSESeq, INT inWep) {
    m_iNowAnimID = -1;
    changeMotionEx(inRcid + 10000, inAnimID, 0, inRootMove, inChangeTime, inEffSeq, inSESeq);
    if (isPlayerFlag(0x40000000) && inWep > 0) {
        m_pWeapon->changeMotionEx(inRcid + inWep + 10000, inAnimID, 0, FALSE, inChangeTime, inEffSeq, inSESeq);
    }
    setAnimSpeed(1.0f);
}
BOOL CPlayer::isNowIdleAnim() {
    if (m_pAnim->isNowExAnim()) return FALSE;
    switch (m_iNowAnimID) {
    case 1:
    case 8:
    case 0xE:
    case 0x18:
    case 0x2A:
        return TRUE;
    }
    if (m_pPLM->getFriend() || m_pPLM->getSummon()) return TRUE;
    if (m_PlayerState == 5) {
        if (m_iSubState == 1) return TRUE;
    }
    return FALSE;
}
void CPlayer::setFaceAnim(INT inID, INT inFlag) {
    if (!inFlag && m_iNowFaceID == inID) return;
    m_iNowFaceID = inID;
    switch (inID) {
    case 1:
        setTexAnmSeq(0, PL::g_szTexEyeDamage);
        setTexAnmSeq(1, PL::g_szTexLipDamage);
        break;
    case 2:
        setTexAnmSeq(0, PL::g_szTexEyeShut);
        setTexAnmSeq(1, PL::g_szTexLipDamage);
        break;
    case 3:
        setTexAnmSeq(0, PL::g_szTexEyeKizetsu);
        setTexAnmSeq(1, PL::g_szTexLipDefault);
        break;
    case 0:
    default:
        m_iNowFaceID = 0;
        setTexAnmSeq(0, -1);
        setTexAnmSeq(1, -1);
        break;
    }
}
void CPlayer::setTransOff(INT inFlag) {
    if (inFlag) {
        setEntityFlag(0x1000, 0);
        if (isPlayerFlag(0x40000000)) m_pWeapon->setEntityFlag(0x1000, 0);
    } else {
        setEntityFlag(0, 0x1000);
        if (isPlayerFlag(0x40000000)) m_pWeapon->setEntityFlag(0, 0x1000);
    }
}
void CPlayer::onWeapon(INT inEffect) {
    if (m_pWeapon) m_pWeapon->onWeapon(inEffect);
    m_uiPlayerFlag = (INT)m_uiPlayerFlag & ~2;
}
void CPlayer::offWeapon() {
    if (m_pWeapon) m_pWeapon->offWeapon();
    m_uiPlayerFlag |= 2;
}
void CPlayer::onAttack() {
    if (m_pWeapon) {
        m_pWeapon->onAttack(m_uiAttackID, m_iAttackPower, m_AtkKind, m_AtkAttr);
        onWeapon(1);
        if (m_uiAttackID < 8 || m_uiAttackID - 0x97 < 14) {
            if (isConditionFlag(0x80)) m_pWeapon->setWideAttack(1);
            if (isConditionFlag(0x100000)) m_pWeapon->setBackAttack(1);
            if (isConditionFlag(0x20000000)) m_pWeapon->setDoubleHit(1, NULL);
        }
    }
}
void CPlayer::offAttack() {
    if (m_pWeapon) m_pWeapon->offAttack();
}
void CPlayer::getWeaponTopPos(FVECTOR& inVec) {
    if (m_pWeapon) {
        FVECTOR ofs = PlayerWeapon::g_fWeaponTopOffset;
        FMATRIX m = m_pWeapon->getMatrixWep0();
        ApplyMatrixXYZ(inVec, m, ofs);
    }
}
void CPlayer::getCaskPos(FVECTOR& inVec) {
    if (m_pWeapon) {
        FVECTOR ofs = { { -0.06f, -0.27f, 0.03f, 1.0f } };
        if (DebugWork::g_dw.player.iTemp == 1) ofs = DebugWork::g_dw.player.vTemp;
        FMATRIX m = m_pWeapon->getMatrixWep0();
        ApplyMatrixXYZ(inVec, m, ofs);
    }
}
void CPlayer::updateBodyColl() {
    updateBodyCollPos();
    m_collBody.m_uiFlag = (INT)m_collBody.m_uiFlag & ~8;
    m_collBody.m_uiFlag = (INT)m_collBody.m_uiFlag & ~0x20;
    CCollBody& body = m_collBody;
    if (isPlayerFlag(1)) body.CCollBody::off();
    else body.CCollBody::on();
    if (isPlayerFlag(0x1000)) m_collBody.m_enableBody = 0;
    else m_collBody.m_enableBody = 1;
}
void CPlayer::updateBodyCollPos() {
    FVECTOR top;
    FVECTOR bottom;
    m_vCenterPos = Crd(m_iBoneCenter).getWorldMatrix().v[3];
    m_vHeadPos = Crd(m_iBoneHead).getWorldMatrix().v[3];
    FLOAT _x = m_vCenterPos.x;
    FLOAT _y = m_vHeadPos.y + 0.2f;
    FLOAT _z = m_vCenterPos.z;
    m_vHeadPos.x = _x;
    m_vHeadPos.y = _y;
    m_vHeadPos.z = _z;
    top.set(_x, _y, _z);
    FLOAT y = (Crd(m_iBoneLFoot).getWorldMatrix().getTransRef().y + Crd(m_iBoneRFoot).getWorldMatrix().getTransRef().y) * 0.5f;
    bottom.set(m_vCenterPos.x, y, m_vCenterPos.z);
    m_collBody.setTopPos(top);
    m_collBody.setBottomPos(bottom);
    m_collBody.setRadius(m_pParam->BODY_COLL_RADIUS);
    m_collBody.setDir(m_fDir);
}
BOOL CPlayer::isMuteki() const {
    if (m_uiPlayerFlag & 1) return TRUE;
    if (m_tqMutekiTimer > 0) return TRUE;
    return m_bDmgMuteki != 0;
}
void CPlayer::exeMutekiTimer() {
    m_tqMutekiTimer -= m_tqFrameRate;
    if (m_tqMutekiTimer < 0) m_tqMutekiTimer = 0;
}
__asm__(".rdata\n");
void CPlayer::setControl() {
    INT iRev;
    m_fDir = m_pMove->getRot().y;
    m_uiTrgFlag &= 0xFFF7F400;
    m_fAnalogPower = getAnalogPower();
    m_fAnalogDir = getAnalogDir();
    m_fAnalogViewDir = getAnalogViewDir();
    if (!m_bIsBattleScene) {
        if (isTrgAttack() && isActionFlag(0x40) && !m_tqAttackTimer) setTrgFlag(4, 0);
    } else {
        if (isPlayerFlag(0x40000)) {
            initTransform(1);
            return;
        }
        if (m_bIsDead) return;
        if (isBattleFlag(4)) return;
        if (isConditionFlag(0x2000000) || isTrgFlag(0x100000)) {
            TRACE("CPlayer::setControl: DMODE End... Cancel!\n");
            return;
        }
        if (m_pRevolver) {
            iRev = 0;
            if (m_hSnatch > 0) {
                setBattleFlag(0, 8);
            } else if (!isTrgStockSkill()) {
                if (!CardBattle::CCardBattle::isExecutingCard(1) && !isBattleFlag(0x8000000)) {
                    iRev = isDownReloadDeck();
                    if (iRev) {
                        if (m_pRevolver->isEnableCharge()) {
                            if (!CGame::isRikuGame() || isTrgReloadDeck()) setTrgFlag(0x10, 0);
                        }
                    } else {
                        iRev = isTrgChangeDeck();
                        if (iRev) {
                            m_pRevolver->changeMode(-1, 0);
                        } else {
                            iRev = isTrgCancelStock();
                            if (iRev) {
                                if (!CGameWork::getFlag(1, 0x30)) m_pRevolver->returnStockCard(0);
                            }
                        }
                    }
                }
                if (!iRev) {
                    iRev = 1;
                    if (isTrgShortCutReload()) {
                        iRev = 0;
                        m_pRevolver->rollShortCut(1);
                    } else if (isTrgShortCutCard()) {
                        iRev = 0;
                        m_pRevolver->rollShortCut(2);
                    } else if (isDownRollDeckL() && isDownRollDeckR()) {
                    } else if (isDownRollDeckL()) {
                        iRev = 0;
                        if (m_tqRevolverRoll > 0) m_tqRevolverRoll = 0;
                        m_tqRevolverRoll -= m_tqFrameRate;
                        if (isTrgRollDeckL()) {
                            m_tqRevolverRoll = 0;
                            m_tqRevolverSpeed = 300;
                            m_pRevolver->rollDeck(-1, 0, 300);
                        } else if (m_tqRevolverRoll < -4000) {
                            m_tqRevolverSpeed = 100;
                            m_pRevolver->rollDeck(-1, 0, 100);
                        } else if (m_tqRevolverRoll < -2000) {
                            m_tqRevolverSpeed = 200;
                            m_pRevolver->rollDeck(-1, 0, 200);
                        }
                    } else if (isDownRollDeckR()) {
                        iRev = 0;
                        if (m_tqRevolverRoll < 0) m_tqRevolverRoll = 0;
                        m_tqRevolverRoll += m_tqFrameRate;
                        if (isTrgRollDeckR()) {
                            m_tqRevolverRoll = 0;
                            m_tqRevolverSpeed = 300;
                            m_pRevolver->rollDeck(1, 0, 300);
                        } else if (m_tqRevolverRoll > 4000) {
                            m_tqRevolverSpeed = 100;
                            m_pRevolver->rollDeck(1, 0, 100);
                        } else if (m_tqRevolverRoll > 2000) {
                            m_tqRevolverSpeed = 200;
                            m_pRevolver->rollDeck(1, 0, 200);
                        }
                    }
                }
            }
            if (iRev) {
                m_tqRevolverRoll = 0;
                m_tqRevolverSpeed = 300;
            }
        }
        if (isBattleFlag(0x1000)) {
            if (isConditionFlag(0x8810000)) {
                if (isConditionFlag(0x8000000)) {
                    TRACE("CPlayer::setControl(); DARKPUNISHER -> STOCK_SKILL ...enableDuelEntry\n");
                    m_pCB->disableDuelEntry(0);
                }
                setConditionFlag(0, 0x8810000);
                m_pMove->setFlag(0, 1);
                setOldPos(m_vCenterPos);
            }
            exeUseStock(0);
            return;
        }
        if (isBattleFlag(0x10)) setTrgFlag(4, 0);
        if (isBattleFlag(0x80)) setTrgFlag(8, 0);
        if (isBattleFlag(0x100)) setTrgFlag(0x20, 0);
        if (isBattleFlag(0x200)) setTrgFlag(0x40, 0);
        if (isBattleFlag(0x400)) setTrgFlag(0x80, 0);
        if (isBattleFlag(0x800)) setTrgFlag(0x100, 0);
    }
    if (isTrgDefense() && isActionFlag(8)) {
        if (!m_pCB || !CardBattle::CCardBattle::isPlayingCard(1) || !isBattleFlag(0x10200000)) setTrgFlag(2, 0);
    }
    if (isTrgJump() && isActionFlag(0x10)) setTrgFlag(1, 0);
    if (isTrgStock()) setTrgFlag(0x200, 0);
}
INT CPlayer::chkAtelHide(INT inFlag) {
    if (inFlag) {
        if (CGameWork::getFlag(1, 0)) {
            changeState(3);
            return TRUE;
        }
    } else {
        if (!CGameWork::getFlag(1, 0)) {
            if (!m_bIsBattleScene) changeState(0);
            else changeState(1);
            return TRUE;
        }
    }
    return FALSE;
}
void CPlayer::chkAtelControl() {
    if (CGameWork::getFlag(1, 5)) {
        if (isControl()) {
            m_uiPadFlag = (INT)m_uiPadFlag & ~1;
            m_pCam->setFlag(1, 0);
        }
    } else {
        if (!isControl()) {
            enableControl();
            m_pCam->setFlag(0, 1);
        }
    }
}
void CPlayer::getCameraTargetPos(FVECTOR& outPos) {
    if (m_PlayerState == PL::TRANSFORM) {
        outPos = Crd(m_iBoneCenter).getWorldMatrix().v[3];
    } else {
        outPos = Crd(0).getWorldMatrix().v[3];
        outPos.y += 0.5f;
    }
}
void CPlayer::chkBodyRevise() {
    setPlayerFlag(0, 0x200000);
    if (!isPlayerFlag(0x9000) && m_collBody.collideBody(0) && !isPlayerFlag(0x400000)) {
        FVECTOR pos = getPos();
        setOldPos(pos);
        pos.x += (m_collBody.getOutPos()->x - m_vCenterPos.x) * 0.5f;
        pos.z += (m_collBody.getOutPos()->z - m_vCenterPos.z) * 0.5f;
        setPos(pos);
        setPlayerFlag(0x200000, 0);
    }
    setPlayerFlag(0, 0x400000);
}
void CPlayer::recoverCollMap() {
    if (m_pCollMap) {
        m_pCollMap->setType(COLL_TYPE_SPHERE);
        m_pCollMap->enableSliding(1);
        m_pMove->setFootRadius(m_fFootRadius);
        m_pMove->setWallRadius(m_fWallRadius);
    }
}
void CPlayer::setDownForce() {
    if (!isConditionFlag(0x10000000)) {
        if (!getJumpState()) {
            FVECTOR vel = m_pMove->getVel();
            vel.y = -0.05f;
            m_pMove->setVel(vel);
        }
    } else {
        FVECTOR vel = m_pMove->getVel();
        FVECTOR pos = m_pMove->getPos();
        pos.y += vel.y;
        if (pos.y <= m_fJumpLandLimit) {
            pos.y = m_fJumpLandLimit;
            if (vel.y < 0.0f) vel.y = 0.0f;
            m_pMove->setPos(pos);
            m_pMove->setVel(vel);
            m_bAir = FALSE;
        }
    }
}
void CPlayer::setLand(FLOAT inDownLEngth, FLOAT inUpLength, BOOL inFirstHit) {
    recoverCollMap();
    land(inDownLEngth, inUpLength, inFirstHit);
}
void CPlayer::beginStateField() {
    m_iState = 0;
    m_uiStep = 0;
    setActionFlag(m_pParam->USE_MAP_ACTION, 0);
    initActionSkill();
    setBattleFlag(0, 1);
    setAnimForce(1, -1, 0, 0, 0, 0);
    initStartWait();
    if (CGameWork::getFlag(1, 0x380)) {
        setEffect(PL::g_szDpxWarpOut, 0, &getPos(), NULL, 0.0f);
        CGameWork::setFlag(1, 0x380, 0);
    }
}
void CPlayer::stateField() {
    if (chkAtelHide(1)) return;
    chkAtelControl();
    m_tqFrameRate = getFrameRate();
    m_fFrameRate = (FLOAT)m_tqFrameRate * 0.020000001f;
    m_fGravity = *(FLOAT*)&PL::g_fGravity * m_fFrameRate;
    m_bAirCheck = FALSE;
    chkBodyRevise();
    m_tqStateTimer += m_tqFrameRate;
    getPadInfo();
    setControl();
    if (m_iHP >= 2 && DebugWork::g_dw.player.iHP1) {
        m_iHP = 1;
        if (m_pGauge) m_pGauge->setDamage(1, 0);
    }
    if (dbgButtonTrg(USR_SYSTEM, 0x200, 1)) {
        m_pMove->setPos(m_vStartPos);
        m_pMove->setOldPos(m_vStartPos);
        return;
    }
    if (dbgButtonDown(USR_SYSTEM, 1, 1) && dbgButtonDown(USR_SYSTEM, 2, 1)) {
        FVECTOR pos;
        FVECTOR vec;
        FMATRIX mat;
        FLOAT x;
        FLOAT y;
        FLOAT dir;
        FLOAT power;
        DebugWork::g_dw.player.iPlayerInfo = 1;
        pos = m_pMove->getPos();
        pos.y -= (FLOAT)dbgAnalogRY(USR_SYSTEM, 1) * 0.00024414062f;
        x = (FLOAT)CPadHandler::getAnalogLX(1) * 0.00024414062f;
        y = (FLOAT)CPadHandler::getAnalogLY(1) * 0.00024414062f;
        dir = atan2f(x, y);
        power = _sqrtf(x * x + y * y);
        if (power > 1.0f) power = 1.0f;
        vec.x = 0.0f;
        vec.y = 0.0f;
        vec.z = power * 0.5f;
        vec.w = 1.0f;
        UnitMatrix(mat);
        RotMatrixY(mat, mat, dir);
        ApplyMatrix(vec, mat, vec);
        pos.x = pos.x + vec.x;
        pos.y = pos.y + vec.y;
        pos.z = pos.z + vec.z;
        pos.w = pos.w + vec.w;
        m_pMove->setPos(pos);
        m_pMove->setOldPos(pos);
        vec.init();
        m_pMove->setVel(vec);
        m_fJumpSpdY = 0.0f;
        m_pMove->setAcc(vec);
        return;
    }
    if (isTrgBattleChange()) {
        BOOL result;
        if (CSceneInfo::isMinigameScene()) result = CSceneInfo::reqExitMinigame(0);
        else result = CSceneInfo::reqEnterBattle();
        if (result) {
            changeState(2);
            setPlayerState(DBGWAIT);
            return;
        }
    }
    if (dbgCheckUser(KKIKUCHI)) {
        if (dbgButtonTrg(USR_SYSTEM, 0x10, 1)) {
            if (CGameWork::getInt(1, 0xE2)) CGameWork::setInt(1, 0xE2, 0);
            else CGameWork::setInt(1, 0xE2, -1);
        }
        if (dbgButtonTrg(USR_SYSTEM, 0x80, 1)) {
            m_iHP = 1;
            if (m_pGauge) m_pGauge->setDamage(1, 0);
        }
        if (dbgButtonTrg(USR_SYSTEM, 8, 1)) {
            PRIZE_PARAM param;
            param.vPos = m_vCenterPos;
            param.PrizeKind = 1;
            param.fSpeed = 0.25f;
            param.uiFlag = 0;
            param.uiPrizeFlag = 0;
            param.iNumber = rand() % 10;
            param.vRot.set((-70.0f - RND::genrand_real1() * 10.0f) * 0.017453292f, RND::genrand_real1() * 360.0f * 0.017453292f, 0.0f);
            Math3D::LimitPIVector(param.vRot);
            CEntityFactory::getInstance()->createPrize(param, NULL);
        }
    }
    if (m_PlayerState != DEAD) {
        if (chkDamage()) return;
    }
    chkLockon();
    setGroundIK(0);
    switch (m_PlayerState) {
    case START_WAIT:
        exeStartWait();
        setGroundIK(1);
        break;
    case STAND:
        exeGimmickRide();
    case WALK:
    case RUN:
    case TALK:
        exeStandUp();
        setGroundIK(1);
        break;
    case WALL:
        exeWall();
        break;
    case LADDER:
        exeLadder();
        break;
    case JUMP:
        exeJump();
        break;
    case GLIDE:
        exeGlide();
        break;
    case HANG:
        exeHang();
        break;
    case AVOID:
        exeAvoid();
        break;
    case ATTACK_FLD:
        exeAttackField();
        break;
    case TREASUREBOX:
        exeTreasureBox();
        break;
    case ROOMCREATION:
        exeRoomCreation();
        break;
    case WORLDSELECT:
        exeWorldSelect();
        break;
    case CARRY:
        exeCarry();
        break;
    case DAMAGE:
    case FLICKS:
        exeDamage();
        break;
    case DEAD:
        exeDead();
        break;
    case BADCONDITION:
        exeBadCondition();
        break;
    }
    exeMutekiTimer();
    exeAttackTimer();
    exePrizeHealing();
    setDownForce();
    exeConfuse();
    recoverCollMap();
}
void CPlayer::endStateField() {
    setActionFlag(0, m_pParam->USE_MAP_ACTION);
    setBattleFlag(0, 1);
    if (CGameWork::getFlag(1, 0)) offWeapon();
    m_collBody.offBody();
    offAttack();
    m_uiAttackKind = 0;
    m_uiPreAttackKind = 0;
    m_uiAttackID = 0;
    m_pMarker->off();
}
void CPlayer::beginStateBattle() {
    m_iState = 1;
    m_uiStep = 0;
    setActionFlag(m_pParam->USE_BTL_ACTION, 0);
    initActionSkill();
    setBattleFlag(1, 0);
    setAnimForce(0x2A, -1, 0, 0, 0, 0);
    onWeapon(0);
    m_pPLM->setReqLoadFirst();
    initStartWait();
    if (PL::g_uiStartCondition & 8) setConfuse(1, 0);
    PL::g_uiStartCondition = 0;
    m_szEffectName = NULL;
}
void CPlayer::stateBattle() {
    if (chkAtelHide(1)) return;
    chkAtelControl();
    m_tqFrameRate = getFrameRate();
    m_fFrameRate = (FLOAT)m_tqFrameRate * 0.020000001f;
    m_fGravity = *(FLOAT*)&PL::g_fGravity * m_fFrameRate;
    m_bAirCheck = FALSE;
    chkBodyRevise();
    m_tqStateTimer += m_tqFrameRate;
    getPadInfo();
    chkPlayCard();
    setControl();
    if (m_iHP >= 2 && DebugWork::g_dw.player.iHP1) {
        m_iHP = 1;
        if (m_pGauge) m_pGauge->setDamage(1, 0);
    }
    if (dbgButtonTrg(USR_SYSTEM, 0x200, 1)) {
        m_pMove->setPos(m_vStartPos);
        m_pMove->setOldPos(m_vStartPos);
        return;
    }
    if (dbgButtonDown(USR_SYSTEM, 1, 1) && dbgButtonDown(USR_SYSTEM, 2, 1)) {
        FVECTOR pos;
        FVECTOR vec;
        FMATRIX mat;
        FLOAT x;
        FLOAT y;
        FLOAT dir;
        FLOAT power;
        DebugWork::g_dw.player.iPlayerInfo = 1;
        pos = m_pMove->getPos();
        pos.y -= (FLOAT)dbgAnalogRY(USR_SYSTEM, 1) * 0.00024414062f;
        x = (FLOAT)CPadHandler::getAnalogLX(1) * 0.00024414062f;
        y = (FLOAT)CPadHandler::getAnalogLY(1) * 0.00024414062f;
        dir = atan2f(x, y);
        power = _sqrtf(x * x + y * y);
        if (power > 1.0f) power = 1.0f;
        vec.x = 0.0f;
        vec.y = 0.0f;
        vec.z = power * 0.5f;
        vec.w = 1.0f;
        UnitMatrix(mat);
        RotMatrixY(mat, mat, dir);
        ApplyMatrix(vec, mat, vec);
        pos.x = pos.x + vec.x;
        pos.y = pos.y + vec.y;
        pos.z = pos.z + vec.z;
        pos.w = pos.w + vec.w;
        m_pMove->setPos(pos);
        m_pMove->setOldPos(pos);
        vec.init();
        m_pMove->setVel(vec);
        m_fJumpSpdY = 0.0f;
        m_pMove->setAcc(vec);
        return;
    }
    if (isTrgBattleChange()) {
        if (CSceneInfo::reqExitBattle()) {
            resetAction(3);
            CardBattle::CCardBattle::setBattleEnd();
            m_pRevolver = NULL;
            changeState(2);
            setPlayerState(DBGWAIT);
            return;
        }
    }
    if (dbgCheckUser(KKIKUCHI)) {
        if (dbgButtonTrg(USR_SYSTEM, 0x10, 1)) {
            if (CGameWork::getInt(1, 0xE2)) CGameWork::setInt(1, 0xE2, 0);
            else CGameWork::setInt(1, 0xE2, -1);
        }
        if (dbgButtonTrg(USR_SYSTEM, 0x80, 1)) {
            m_iHP = 1;
            if (m_pGauge) m_pGauge->setDamage(1, 0);
        }
        if (dbgButtonDown(USR_SYSTEM, 2, 1)) {
            if (dbgButtonTrg(USR_SYSTEM, 0x8000, 1)) addMP(10);
            if (dbgButtonTrg(USR_SYSTEM, 0x2000, 1)) addMP(4);
            if (dbgButtonTrg(USR_SYSTEM, 0x1000, 1)) addMP(1000);
            if (dbgButtonTrg(USR_SYSTEM, 0x4000, 1)) addMP(100);
        }
        if (dbgButtonDown(USR_SYSTEM, 0x400, 1)) setFlipForce(NULL);
    }
    switch (m_PlayerState) {
    case ESCAPE:
    case END_BTL:
    case DEAD:
        break;
    case TRANSFORM:
        if (!isConditionFlag(0x200000)) break;
    default:
        if (chkDamage()) return;
        if (chkEscape()) return;
        break;
    }
    chkLockon();
    setGroundIK(0);
    switch (m_PlayerState) {
    case START_WAIT:
        exeStartWait();
        break;
    case STAND:
    case WALK:
    case RUN:
        exeStandUp();
        break;
    case JUMP:
        exeJump();
        break;
    case GLIDE:
        exeGlide();
        break;
    case JUMP_2ND:
        exeJump2nd();
        break;
    case HANG:
        exeHang();
        break;
    case AVOID:
        exeAvoid();
        break;
    case ATTACK:
        exeAttack();
        break;
    case DAMAGE:
    case FLICKS:
        exeDamage();
        break;
    case BADCONDITION:
        exeBadCondition();
        break;
    case ITEM:
        exeItem();
        break;
    case CHARGE:
        exeCharge();
        break;
    case MAGIC:
        exeMagic();
        break;
    case SKILL:
        exeStockSkill();
        break;
    case GIMMICKCARD:
        exeGimmickCard();
        break;
    case FRIEND:
        exeFriend();
        break;
    case SUMMON:
        exeSummon();
        break;
    case ESCAPE:
        exeEscape();
        break;
    case END_BTL:
        exeEnd();
        break;
    case DEAD:
        exeDead();
        break;
    case TRANSFORM:
        exeTransform();
        break;
    case DUEL:
        exeDuel();
        break;
    case FOOTING:
        exeFooting();
        break;
    case PRONOUNCE:
        exePronounce();
        break;
    }
    exeMutekiTimer();
    exeAttackTimer();
    exePrizeHealing();
    setDownForce();
    exeConfuse();
    exeBanish();
    exeQuickBarrier();
    exeAeroGuard();
    fadeEffCharge();
    recoverCollMap();
    exeWind();
}
void CPlayer::endStateBattle() {
    setActionFlag(0, m_pParam->USE_BTL_ACTION);
    m_collBody.offBody();
    offAttack();
    setBattleFlag(0, 1);
    offWeapon();
    m_uiAttackKind = 0;
    m_uiPreAttackKind = 0;
    m_uiAttackID = 0;
    m_pMarker->off();
}
void CPlayer::beginStateWait() {
    m_iState = 2;
}
void CPlayer::beginStateHide() {
    m_iState = 3;
    setPlayerState(PL::HIDE);
    clearSpeed();
    setEntityFlag(1, 0);
    m_WalkMode = WALKMODE_STAND;
    if (m_bIsBattleScene) setAnimForce(0x2A, 0, 0, 0, 0, 0);
    else setAnimForce(1, 0, 0, 0, 0, 0);
    setPlayerFlag(0x401001, 0);
    m_pCam->setFlag(1, 0);
}
void CPlayer::stateHide() {
    setPlayerFlag(0x400000, 0);
    if (dbgCheckUser(KKIKUCHI) && dbgButtonTrg(USR_SYSTEM, 0x10, 1)) {
        CGameWork::setInt(1, 0xE2, 0);
    }
    chkAtelHide(0);
}
void CPlayer::endStateHide() {
    setEntityFlag(0, 1);
    setPlayerFlag(0, 0x1001);
    setLand(5.0f, 0.5f, 1);
    clearSpeed();
    m_pCam->setFlag(0, 1);
}
BOOL CPlayer::isNowInAir() {
    m_bAirCheck = FALSE;
    return isAir();
}
BOOL CPlayer::isAir() {
    if (m_bAirCheck) return m_bAir;
    m_bAirCheck = TRUE;
    if (m_pMove->isGround()) {
        m_bAir = FALSE;
        return FALSE;
    }
    FVECTOR pos = m_pMove->getPos();
    if (pos.y <= m_fJumpLandLimit) {
        pos.y = m_fJumpLandLimit;
        m_pMove->setPos(pos);
        m_bAir = FALSE;
        return FALSE;
    }
    m_bAir = TRUE;
    return TRUE;
}
FLOAT CPlayer::getAirHeight(FVECTOR* inPos, FLOAT inHeight, INT inLimit) {
    FVECTOR pos;
    if (inPos) pos = *inPos;
    else pos = getPos();
    FLOAT fHeight = inHeight;
    FVECTOR vStart = {{ pos.x, pos.y, pos.z, 1.0f }};
    FVECTOR vEnd = {{ pos.x, pos.y - fHeight, pos.z, 1.0f }};
    m_pCollMap->setType(COLL_TYPE_POINT);
    m_pCollMap->enableSliding(0);
    m_pCollMap->setShiftY(0.0f);
    m_pCollMap->setStartPos(vStart);
    m_pCollMap->setEndPos(vEnd);
    FLOAT fRideH = fHeight;
    if (m_pCollMap->collideRide(0)) fRideH = pos.y - m_pCollMap->getDetectPos()->y;
    if (m_pCollMap->collideMap()) fHeight = pos.y - m_pCollMap->getDetectPos()->y;
    if (fHeight < fRideH) fHeight = fRideH;
    if (inLimit && pos.y + fHeight <= m_fJumpLandLimit) fHeight = pos.y - (FLOAT)inLimit;
    return fHeight;
}
FLOAT CPlayer::getFrontAirHeight(FVECTOR* inPos, FLOAT inLength, FLOAT inHeight, FLOAT inDir) {
    FVECTOR pos;
    if (inPos) pos = *inPos;
    else {
        pos = m_vCenterPos;
        pos.y = getPos().y;
    }
    FLOAT fHeight = inHeight;
    FVECTOR ofs = {{ 0.0f, 0.0f, inLength, 1.0f }};
    FLOAT d = m_fDir + inDir;
    FLOAT r;
    if (d < -3.1415927f) r = d + 6.2831855f;
    else {
        r = d;
        if (r > 3.1415927f) r -= 6.2831855f;
    }
    FMATRIX m;
    UnitMatrix(m);
    RotMatrixY(m, m, r);
    ApplyMatrixXYZ(ofs, m, ofs);
    FVECTOR vStart = {{ pos.x, pos.y, pos.z, 1.0f }};
    FVECTOR vEnd = {{ pos.x + ofs.x, pos.y + ofs.y, pos.z + ofs.z, 1.0f }};
    m_pCollMap->setType(COLL_TYPE_POINT);
    m_pCollMap->enableSliding(0);
    m_pCollMap->setShiftY(0.0f);
    m_pCollMap->setStartPos(vStart);
    m_pCollMap->setEndPos(vEnd);
    if (m_pCollMap->collideRide(0)) return 0.0f;
    if (m_pCollMap->collideMap()) return 0.0f;
    vStart = vEnd;
    vEnd.y -= inHeight;
    m_pCollMap->setStartPos(vStart);
    m_pCollMap->setEndPos(vEnd);
    FLOAT fRideH = fHeight;
    if (m_pCollMap->collideRide(0)) fRideH = pos.y - m_pCollMap->getDetectPos()->y;
    if (m_pCollMap->collideMap()) fHeight = pos.y - m_pCollMap->getDetectPos()->y;
    if (fRideH < fHeight) fHeight = fRideH;
    if (pos.y + fHeight <= m_fJumpLandLimit) fHeight = pos.y - m_fJumpLandLimit;
    return fHeight;
}
void CPlayer::exeAttackTimer() {
    m_tqAttackTimer -= m_tqFrameRate;
    if (m_tqAttackTimer < 0) m_tqAttackTimer = 0;
}
void CPlayer::initStartWait() {
    setPlayerState(START_WAIT);
    setTimer(1500);
    setLand(2.0f, 0.5f, 1);
    m_bAirCheck = FALSE;
    m_bAir = FALSE;
    m_fJumpSpdY = 0.0f;
    m_uiPreAttackKind = 0;
    m_uiAttackKind = 0;
    m_uiAttackID = 0;
    m_tqAttackTimer = 0;
    setPlayerFlag(0x400001, 0);
    offAttack();
    m_WalkMode = WALKMODE_STAND;
    m_uiPadFlag = (INT)m_uiPadFlag & ~1;
    m_hTarget = 0;
    m_hLockon = 0;
    m_hGimmick = 0;
    m_pCam->setFlag(1, 0);
}
void CPlayer::exeStartWait() {
    if (m_bIsBattleScene) {
        setPlayerFlag(0x400000, 0);
    } else if (getTimer() < 1100) {
        if (isAir()) setAnim(6, -1, 0);
        else setAnim(1, -1, 0);
    }
    if (m_pRevolver) setBattleFlag(0, 8);
    if (getTimer() > 0) return;
    if (CGameWork::getFlag(1, 5)) return;
    enableControl();
    m_pCam->setFlag(0, 1);
    setPlayerFlag(0, 1);
    m_pMarker->on();
    if (!isAir()) initStandUp(0);
    else initFall();
    if (isBattleFlag(1) && !isBattleFlag(0x80000000)) {
        m_vStartPos = getPos();
        setBattleFlag(0x80000000, 0);
    }
}
void CPlayer::initStandUp(INT inFlag) {
    if (isConditionFlag(0x10000000)) {
        initFooting(0);
        return;
    }
    if (!inFlag) {
        setPlayerState(PL::STAND);
        setAnimForce(chkBattleIdle() ? 0x2A : 1, -1, 0, 0, 0, 0);
    } else {
        m_WalkMode = chkWalkMode();
        switch (m_WalkMode) {
        case WALKMODE_STAND:
            setPlayerState(PL::STAND);
            break;
        case WALKMODE_WALK:
            setPlayerState(PL::WALK);
            break;
        case WALKMODE_RUN:
            setPlayerState(PL::RUN);
            break;
        }
        setWalkAnim();
        m_fMovePower = 0.5f;
        m_fWalkSpeed = m_fWalkSpeed / 60.0f * m_fMovePower;
    }
    offAttack();
    setPlayerFlag(0, 0x024C2000);
    m_JumpState = 0;
    m_fJumpSpdY = 0.0f;
    setConditionFlag(0, 0x08810023);
    setBattleFlag(0, 0x00443F90);
    setFaceAnim(0, 0);
    m_pMove->setFlag(0x40, 0); // partial CMovement: (flag, on)
}
__asm__(".rdata\n");
void CPlayer::exeStandUp() {
    WalkMode PreWalkMode;
    if (isTrgFlag(0x100000) || isConditionFlag(0x2000000)) {
        TRACE("CPlayer:: exeStandUp -> initTransform(0)\n");
        initTransform(0);
        return;
    }
    if (isTrgFlag(0x10)) {
        if (!chkChargeMove(0)) {
            initCharge();
            return;
        }
    }
    if (m_bIsBattleScene) {
        if (isBattleFlag(4)) {
            initEnd();
            return;
        }
        if (isTrgFlag(0x400)) {
            initFooting(1);
            return;
        }
        if (isTrgFlag(8)) {
            initItem(0);
            return;
        }
        if (isTrgFlag(0x40)) {
            initGimmickCard(0);
            return;
        }
        if (isTrgFlag(0x20)) initMagic(0);
        else if (isTrgFlag(0x80)) initFriend(0);
        else if (isTrgFlag(0x100)) initSummon(0);
        else if (isTrgFlag(4)) initAttack(0);
        else goto tail;
        return;
    } else {
        if (isTrgFlag(0x200)) {
            switch (chkGimmick(0)) {
            case 0x11:
                initTreasureBox();
                return;
            case 0x14:
                initRoomCreation();
                return;
            case 0x15:
                initWorldSelect();
                return;
            case 0x1E:
                initCarry();
                return;
            case 0x1F:
                initStandUp(0);
                return;
            }
        }
        if (isTrgFlag(4)) {
            initAttackField(0);
            return;
        }
    }
tail:
    if (isTrgFlag(2)) {
        initAvoid();
        return;
    }
    if (isTrgFlag(1)) {
        initJump(0);
        return;
    }
    PreWalkMode = m_WalkMode;
    m_WalkMode = chkWalkMode();
    switch (m_WalkMode) {
    case WALKMODE_STAND:
        if (PreWalkMode != WALKMODE_STAND && m_pSE) ((CSEMotion*)m_pSE)->playFootStopSE();
        setPlayerState(STAND);
        break;
    case WALKMODE_WALK:
        setPlayerState(WALK);
        break;
    case WALKMODE_RUN:
        setPlayerState(RUN);
        break;
    }
    exeWalk();
    if (isAir()) {
        initFall();
        setFallFloat();
        return;
    }
    if (!chkLadderStepNow(0)) {
        initLadder(0);
        return;
    }
    if (chkWall(0)) initWall();
}
WalkMode CPlayer::chkWalkMode() {
    FLOAT aim_power = m_fAnalogPower;
    if (aim_power < 0.3f) {
        m_fWalkSpeed = 0.0f;
        return WALKMODE_STAND;
    }
    FLOAT dash = 1.0f;
    if (m_pCB && m_pCB->getEnchantKind() == 0x2D) dash = 1.7f;
    if (aim_power < 0.9f) {
        m_fWalkSpeed = m_pParam->MAX_WALK_SPEED * dash;
        return WALKMODE_WALK;
    }
    m_fWalkSpeed = aim_power * m_pParam->MAX_RUN_SPEED * dash;
    return WALKMODE_RUN;
}
void CPlayer::exeWalk() {
    FLOAT aim_dir = m_fAnalogViewDir;
    FLOAT d0 = Movement::Homing(m_fDir, aim_dir, 0.2f, m_fFrameRate);
    if (m_WalkMode != WALKMODE_STAND) m_fDir = d0;
    setWalkAnim();
    FLOAT dirY = getDir();
    FVECTOR v = { 0.0f, aim_dir, 0.0f, 0.0f };
    m_pMove->setRot(v);
    m_fMovePower = 1.0f - _abs(_limitPI(aim_dir - dirY)) / 3.1415927f;
    m_fWalkSpeed = m_fWalkSpeed / 60.0f * m_fMovePower;
    m_pMove->calcVelocityXZ(m_fWalkSpeed);
     /* keeps aim_dir live past the wrap: matching lever */
    v.y = dirY;
    m_pMove->setRot(v);
}
void CPlayer::setWalkAnim() {
    INT anim;
    switch (m_PlayerState) {
    case STAND:
    case TALK:
        anim = chkBattleIdle() ? 0x2A : 1;
        break;
    case WALK:
        if (isBattleFlag(1)) anim = 0x2B;
        else anim = isPlayerFlag(2) ? 2 : 0x12;
        break;
    case RUN:
        if (!isBattleFlag(1)) anim = isPlayerFlag(2) ? 3 : 0x13;
        else anim = 0x2C;
        break;
    case AVOID:
    case JUMP:
        switch (m_WalkMode) {
        case WALKMODE_STAND:
            anim = isBattleFlag(1) ? 0x2A : 1;
            break;
        case WALKMODE_WALK:
            if (isBattleFlag(1)) anim = 0x2B;
            else anim = isPlayerFlag(2) ? 2 : 0x12;
            break;
        case WALKMODE_RUN:
            if (!isBattleFlag(1)) anim = isPlayerFlag(2) ? 3 : 0x13;
            else anim = 0x2C;
            break;
        }
        break;
    case CARRY:
        switch (m_WalkMode) {
        case WALKMODE_STAND:
            anim = 0x18;
            break;
        case WALKMODE_WALK:
            anim = 0x19;
            break;
        case WALKMODE_RUN:
            anim = 0x19;
            break;
        }
        break;
    default:
        switch (m_WalkMode) {
        case WALKMODE_STAND:
            anim = 0x2A;
            break;
        case WALKMODE_WALK:
            anim = 0x2B;
            break;
        case WALKMODE_RUN:
            anim = 0x2C;
            break;
        }
        break;
    }
    setAnim(anim, -1, 0);
}
INT CPlayer::chkBattleIdle() {
    if (!isBattleFlag(1)) goto check;
ret_true:
    return TRUE;
check:
    if (m_iPreAnimID >= 0x24) {
        if (m_iPreAnimID < 0x26) goto ret_true;
        if (m_iPreAnimID < 0x34) {
            if (m_iPreAnimID >= 0x30) goto ret_true;
        }
    }
    switch (m_iNowAnimID) {
    case 0x24:
    case 0x25:
    case 0x2A:
    case 0x30:
    case 0x31:
    case 0x32:
    case 0x33:
        goto ret_true;
    }
    return FALSE;
}
INT CPlayer::procFlowIdle(INT inFlag) {
    if (isAir()) {
        setAnim(6, 800, 0);
        setPlayerFlag(0x800000, 0);
        FVECTOR vel = m_pMove->getVel();
        m_fJumpSpdY += m_fGravity * m_fFrameRate;
        vel.y = m_fJumpSpdY;
        m_pMove->setVel(vel);
        return FALSE;
    }
    if (isPlayerFlag(0x800000)) {
        setPlayerFlag(0, 0x800000);
        setAnim(7, -1, 0);
        return FALSE;
    }
    if (m_iNowAnimID == 7 && !isMotionEnd()) return FALSE;
    setAnim(inFlag ? 0x2A : 1, -1, 0);
    clearSpeed();
    return TRUE;
}
INT CPlayer::initIdle(INT inAir, INT inStand) {
    INT bAir = inAir;
    if (bAir < 0) bAir = isAir();
    if (!bAir) {
        initStandUp(inStand);
    } else {
        initFall();
    }
    return bAir;
}
INT CPlayer::chkWall(INT inStep) {
    if (!isActionFlag(0x10000)) return FALSE;
    FVECTOR ofs = {{0.0f, 0.07f, 0.5f, 1.0f}};
    FMATRIX m;
    FVECTOR old;
    FVECTOR pos;
    FLOAT dir;
    BOOL bHit;
    dir = m_fDir;
    UnitMatrix(m);
    RotMatrixY(m, m, dir);
    ApplyMatrixXYZ(ofs, m, ofs);
    old = getOldPos();
    updateCoord();
    coordFix();
    pos = getPos();
    FVECTOR vStart = {{ pos.x - ofs.x, pos.y + ofs.y, pos.z - ofs.z, 1.0f }};
    FVECTOR vEnd = {{ pos.x + ofs.x, pos.y + ofs.y, pos.z + ofs.z, 1.0f }};
    FVECTOR vNormal;
    m_pCollMap->setType(COLL_TYPE_SPHERE);
    m_pCollMap->enableSliding(0);
    m_pCollMap->setRadius(0.25f);
    if (inStep > 0) {
        m_pCollMap->setStartPos(vEnd);
        m_pCollMap->setEndPos(vStart);
        if (!m_pCollMap->collideMap()) return FALSE;
        vNormal = *m_pCollMap->getDetectNormal();
        if (0.64278764f <= vNormal.y) return FALSE;
        m_fWallDir = _limitPI2(atan2f(vNormal.x, vNormal.z));
        m_vWallPos = *m_pCollMap->getDetectPos();
        if (!chkWallBottom()) return FALSE;
    } else {
        bHit = FALSE;
        m_pCollMap->setStartPos(vStart);
        m_pCollMap->setEndPos(vEnd);
        if (m_pCollMap->collideMap()) {
            vNormal = *m_pCollMap->getDetectNormal();
            if (vNormal.y < 0.64278764f) {
                m_fWallDir = _limitPI2(atan2f(vNormal.x, vNormal.z));
                m_vWallPos = *m_pCollMap->getDetectPos();
                bHit = chkWallBottom() != 0;
            }
        }
        if (!bHit) {
            m_pCollMap->setStartPos(vEnd);
            m_pCollMap->setEndPos(vStart);
            if (m_pCollMap->collideMap()) {
                vNormal = *m_pCollMap->getDetectNormal();
                if (vNormal.y < 0.64278764f) {
                    m_fWallDir = _limitPI2(atan2f(vNormal.x, vNormal.z));
                    m_vWallPos = *m_pCollMap->getDetectPos();
                    if (chkWallBottom()) bHit = TRUE;
                }
            }
        }
        if (!bHit) {
            ofs.y = 0.05f;
            ofs.z = 0.6f;
            ofs.x = 0.0f;
            ofs.w = 1.0f;
            RotMatrixY(m, m, 1.5707964f);
            ApplyMatrixXYZ(ofs, m, ofs);
            vStart.set(pos.x - ofs.x, pos.y + ofs.y, pos.z - ofs.z);
            vEnd.set(pos.x + ofs.x, pos.y + ofs.y, pos.z + ofs.z);
            m_pCollMap->setStartPos(vStart);
            m_pCollMap->setEndPos(vEnd);
            if (m_pCollMap->collideMap()) {
                vNormal = *m_pCollMap->getDetectNormal();
                if (vNormal.y < 0.64278764f) {
                    m_fWallDir = _limitPI2(atan2f(vNormal.x, vNormal.z));
                    m_vWallPos = *m_pCollMap->getDetectPos();
                    if (chkWallBottom()) bHit = TRUE;
                }
            }
        }
        if (!bHit) {
            ofs.y = 0.05f;
            ofs.z = 0.6f;
            ofs.x = 0.0f;
            ofs.w = 1.0f;
            RotMatrixY(m, m, 3.1415927f);
            ApplyMatrixXYZ(ofs, m, ofs);
            vStart.set(pos.x - ofs.x, pos.y + ofs.y, pos.z - ofs.z);
            vEnd.set(pos.x + ofs.x, pos.y + ofs.y, pos.z + ofs.z);
            m_pCollMap->setStartPos(vStart);
            m_pCollMap->setEndPos(vEnd);
            if (m_pCollMap->collideMap()) {
                vNormal = *m_pCollMap->getDetectNormal();
                if (vNormal.y < 0.64278764f) {
                    m_fWallDir = _limitPI2(atan2f(vNormal.x, vNormal.z));
                    m_vWallPos = *m_pCollMap->getDetectPos();
                    if (chkWallBottom()) bHit = TRUE;
                }
            }
        }
        if (!bHit) return FALSE;
    }
    if (inStep) return TRUE;
    pos = m_vWallPos;
    m_vOldSafeField = old;
    ofs.x = 0.25f;
    ofs.y = 0.0f;
    ofs.z = 0.0f;
    ofs.w = 1.0f;
    UnitMatrix(m);
    RotMatrixY(m, m, m_fWallDir);
    ApplyMatrixXYZ(ofs, m, ofs);
    vStart.set(pos.x + ofs.x, pos.y, pos.z + ofs.z);
    vEnd.set(pos.x - ofs.x, pos.y, pos.z - ofs.z);
    m_pCollMap->setRadius(0.5f);
    m_pCollMap->setStartPos(vStart);
    m_pCollMap->setEndPos(vEnd);
    if (m_pCollMap->collideMap()) {
        vNormal = *m_pCollMap->getDetectNormal();
        if (vNormal.y < 0.64278764f) pos = *m_pCollMap->getDetectPos();
    }
    m_pCollMap->setStartPos(vEnd);
    m_pCollMap->setEndPos(vStart);
    if (m_pCollMap->collideMap()) {
        vNormal = *m_pCollMap->getDetectNormal();
        if (vNormal.y < 0.64278764f) pos = *m_pCollMap->getDetectPos();
        m_vWallPos.x = pos.x;
        m_vWallPos.z = pos.z;
    } else {
        m_vWallPos.x = pos.x;
        m_vWallPos.z = pos.z;
    }
    return TRUE;
}
INT CPlayer::chkWallBottom() {
    FVECTOR vStart;
    FVECTOR vEnd;
    FVECTOR vNormal;
    FVECTOR ofs;
    FMATRIX m;
    FLOAT _x = m_vWallPos.x;
    FLOAT _z = m_vWallPos.z;
    vStart.set(_x, m_vWallPos.y + 0.25f, _z);
    vEnd.set(_x, m_vWallPos.y - 0.25f, _z);
    m_pCollMap->setStartPos(vStart);
    m_pCollMap->setEndPos(vEnd);
    if (!m_pCollMap->collideMap()) return FALSE;
    vNormal = *m_pCollMap->getDetectNormal();
    if (vNormal.y < 0.64278764f) return FALSE;
    m_vWallPos.y = m_pCollMap->getDetectPos()->y + 0.0035f;
    ofs.set(0.0f, 0.0f, 0.375f);
    UnitMatrix(m);
    RotMatrixY(m, m, m_fWallDir);
    ApplyMatrixXYZ(ofs, m, ofs);
    vStart.set(m_vWallPos.x + ofs.x, m_vWallPos.y + 0.25f, m_vWallPos.z + ofs.z);
    vEnd.set(m_vWallPos.x + ofs.x, m_vWallPos.y - 0.25f, m_vWallPos.z + ofs.z);
    m_pCollMap->setStartPos(vStart);
    m_pCollMap->setEndPos(vEnd);
    return !m_pCollMap->collideMap();
}
void CPlayer::initWall() {
    setPlayerState(PL::WALL);
    clearSpeed();
    offWeapon();
    m_iSubState = 0;
    if (CGame::isSoraGame()) setAnimRsrcEx(0xFD2, 0, -1, 0, 0, 0, 1);
    else setAnimRsrcEx(0xFD4, 0, -1, 0, 0, 0, 1);
    setPos(m_vWallPos);
    m_JumpState = 6;
    m_pMove->setFlag(0, 0x40); // partial CMovement: (flag, on)
    m_fFootRadius = 0.5f;
}
void CPlayer::exeWall() {
    FVECTOR pos;
    FVECTOR rot;
    FLOAT dir;
    Houkou houkou;
    FLOAT cancel_power;
    FLOAT cancel_speed;
    FLOAT r;
    setPlayerFlag(0x400000, 0);
    pos = getPos();
    rot = getRot();
    FVECTOR vec = { 1.0f, 0.0f, 0.0f, 1.0f };
    FMATRIX m;
    cancel_power = vec.y;

    if (!getSubState()) {
        FLOAT d0 = Movement::Homing(m_fDir, m_fWallDir, 0.15f, getFrameRateF());
        m_fDir = d0;
        rot.y = d0;
        setRot(rot);
        r = _limitPI(m_fWallDir - m_fDir);
        if (_abs(r) < 0.03141593f) {
            m_iSubState = 1;
            m_fDir = m_fWallDir;
        }
        warp(m_vWallPos);
        updateCoord();
        coordFix();
        return;
    }
    if (isTrgFlag(4)) {
        m_pMove->setFlag(0x40, 0);
        m_bAirCheck = TRUE;
        m_bAir = TRUE;
        initAttackField(0);
        cancel_power = 2.5f;
        cancel_speed = 2.0f;
    } else if (isTrgFlag(1)) {
        m_pMove->setFlag(0x40, 0);
        initJump(0);
        cancel_speed = 1.3f;
    } else if (!chkWall(1)) {
        m_pMove->setFlag(0x40, 0);
        initJump(0);
        cancel_power = 2.5f;
        cancel_speed = 1.3f;
    } else {
        warp(m_vWallPos);
        updateCoord();
        coordFix();
        dir = _limitPI2(m_fAnalogViewDir - getDir() + 3.1415927f);
        if (m_Houkou != HOUKOU_NUTRAL) {
            if (dir < 0.0f) houkou = HOUKOU_LEFT;
            else houkou = HOUKOU_RIGHT;
        } else {
            houkou = HOUKOU_NUTRAL;
        }
        switch (houkou) {
        case HOUKOU_LEFT:
            setSubState(2);
            if (m_pAnim->getAnimID() != 2) {
                if (CGame::isSoraGame()) setAnimRsrcEx(0xFD2, 2, -1, 0, 0, 0, 1);
                else setAnimRsrcEx(0xFD4, 2, -1, 0, 0, 0, 1);
            }
            m_fWalkSpeed = 0.904f;
            break;
        case HOUKOU_RIGHT:
            setSubState(3);
            if (m_pAnim->getAnimID() != 1) {
                if (CGame::isSoraGame()) setAnimRsrcEx(0xFD2, 1, -1, 0, 0, 0, 1);
                else setAnimRsrcEx(0xFD4, 1, -1, 0, 0, 0, 1);
            }
            m_fWalkSpeed = -0.904f;
            break;
        default:
            setSubState(1);
            if (m_pAnim->getAnimID() != 0) {
                if (CGame::isSoraGame()) setAnimRsrcEx(0xFD2, 0, -1, 0, 0, 0, 1);
                else setAnimRsrcEx(0xFD4, 0, -1, 0, 0, 0, 1);
            }
            m_fWalkSpeed = 0.0f;
            break;
        }
        m_fDir = m_fWallDir;
        rot.y = m_fWallDir;
        setRot(rot);
        m_fWalkSpeed = m_fWalkSpeed / 60.0f;
        m_fMovePower = 1.0f;
        m_pMove->calcVelocityXZ(m_fWalkSpeed, vec);
        return;
    }
    m_fFootRadius = 0.25f;
    dir = getDir();
    pos = getPos();
    FVECTOR ofs = {{0.0f, 0.0f, 0.6f, 1.0f}};
    UnitMatrix(m);
    RotMatrixY(m, m, dir);
    ApplyMatrixXYZ(ofs, m, ofs);
    AddVectorXYZ(pos, pos, ofs);
    ofs.set(0.5f, 0.0f, 0.0f);
    ApplyMatrixXYZ(ofs, m, ofs);
    FVECTOR vStart = {{ pos.x + ofs.x, pos.y, pos.z + ofs.z, 1.0f }};
    FVECTOR vEnd = {{ pos.x - ofs.x, pos.y, pos.z - ofs.z, 1.0f }};
    m_pCollMap->setType(COLL_TYPE_SPHERE);
    m_pCollMap->enableSliding(0);
    m_pCollMap->setRadius(0.5f);
    m_pCollMap->setStartPos(vStart);
    m_pCollMap->setEndPos(vEnd);
    if (m_pCollMap->collideRide(0) || m_pCollMap->collideMap()) pos = *m_pCollMap->getDetectPos();
    m_pCollMap->setStartPos(vEnd);
    m_pCollMap->setEndPos(vStart);
    if (m_pCollMap->collideRide(0) || m_pCollMap->collideMap()) pos = *m_pCollMap->getDetectPos();
    m_pCollMap->setStartPos(pos);
    vEnd = getPos();
    m_pCollMap->setEndPos(vEnd);
    if (m_pCollMap->collideRide(0) || m_pCollMap->collideMap()) pos = *m_pCollMap->getDetectPos();
    setPos(pos);
    m_fWalkSpeed = cancel_speed / 60.0f;
    m_pMove->calcVelocityXZ(m_fWalkSpeed);
    m_fJumpSpdXZ = m_fWalkSpeed;
    m_fMoveSpdXZ = m_fWalkSpeed;
    FVECTOR vel = m_pMove->getVel();
    vel.y = *(FLOAT*)&PL::g_fAntiGravity * cancel_power;
    m_fJumpSpdY = vel.y;
    m_pMove->setVel(vel);
}
/* FVECTOR::operator-(const FVECTOR&) of the original (ref locals: FVECTOR* this, FVECTOR vec). */
static inline FVECTOR _subVec(const FVECTOR& a, const FVECTOR& inV) {
    FVECTOR vec = a;
    vec.x -= inV.x;
    vec.y -= inV.y;
    vec.z -= inV.z;
    vec.w -= inV.w;
    return vec;
}
INT CPlayer::chkLadderStepNow(BOOL inAir) {
    const FVECTOR* pos;
    const FVECTOR* nor;
    FVECTOR dif;
    INT step;
    INT max;
    if (!isActionFlag(0x1000)) return -1;
    if (!inAir && m_fAnalogPower < 0.3f) return -1;
    pos = m_pMove->getLadderPos();
    nor = m_pMove->getLadderNormal();
    if (!pos || !nor) return -1;
    dif = _subVec(*pos, getPos());
    dif.y = 0.0f;
    dif.w = 1.0f;
    if (Math3D::VectorLength(dif) > 0.75f) return -1;
    setPlayerFlag(0x2000, 0);
    FLOAT r = atan2f(nor->x, nor->z) - m_fDir;
    if (r < -3.1415927f) r += 6.2831855f;
    else if (r > 3.1415927f) r -= 6.2831855f;
    if (getHoukouFromAngle(r) != 1) return -1;
    step = (INT)((getPos().y - pos->y) / 0.375f);
    max = (INT)(m_pMove->getLadderHeight() / 0.375f) - 4;
    if (step >= max) return -1;
    dbgCheckUser(KKIKUCHI);
    return step;
}
INT CPlayer::getLadderStepNow(FLOAT inHeight) {
    return (INT)((inHeight - m_vLadderPos.y + 0.0035f) / 0.375f);
}
void CPlayer::initLadder(INT inStep) {
    FVECTOR v;
    FVECTOR ofs;
    FMATRIX m;
    FVECTOR pos;
    m_vOldSafeField = getPos();
    setPlayerState(LADDER);
    offWeapon();
    m_pLadderPos = (FVECTOR*)m_pMove->getLadderPos();
    m_pLadderNormal = (FVECTOR*)m_pMove->getLadderNormal();
    m_iLadderStep = inStep;
    FLOAT d = atan2f(m_pLadderNormal->x, m_pLadderNormal->z) + 3.1415927f;
    FLOAT r;
    if (d < -3.1415927f) r = d + 6.2831855f;
    else {
        r = d;
        if (r > 3.1415927f) r -= 6.2831855f;
    }
    m_fLadderDir = r;
    m_fDir = r;
    v = getRot();
    v.y = m_fLadderDir;
    setRot(v);
    UnitMatrix(m);
    RotMatrixY(m, m, m_fLadderDir);
    ApplyMatrixXYZ(ofs, m, PL::LADDER_LOCAL_OFS);
    SubVectorXYZ(m_vLadderPos, *m_pLadderPos, ofs);
    pos = m_vLadderPos;
    if (!m_iLadderStep) {
        m_iSubState = 0;
        setAnimForce(9, 0, 0, 0, 0, 0);
        clearSpeed();
        m_fLadderSpdY = 0.375f / (FLOAT)(m_pAnim->getTotalTqCrd() / 25);
        m_pCam->initCamera(1, 0, NULL);
    } else {
        m_iSubState = 0;
        setAnimForce(10, 0, 0, 0, 0, 0);
        clearSpeed();
        m_fLadderSpdY = 0.0f;
        pos.y += (FLOAT)(m_iLadderStep - 2) * 0.375f;
    }
    setPos(pos);
    setOldPos(pos);
    m_iLadderStepMax = getLadderStepNow(m_vLadderPos.y + m_pMove->getLadderHeight()) - 4;
    m_JumpState = 6;
    m_pMove->setFlag(0, 0x40);
    m_pCam->setFlag(8, 0);
}
void CPlayer::exeLadder() {
    setPlayerFlag(0x400000, 0);
    m_iLadderStep = getLadderStepNow(getPos().y);
    switch (m_iSubState) {
    case 0:
        if (isMotionEnd()) {
            m_iSubState = 1;
            setAnimForce(8, 0, 0, 0, 0, 0);
            m_fLadderSpdY = 0.0f;
        }
        break;
    case 1:
        if (isStartLadderJump()) return;
        if (getAnalogHoukou() == HOUKOU_UP) {
            m_iSubState = 2;
            setAnimForce(0xB, 0, 0, 0, 0, 0);
            m_fLadderSpdY = 0.75f / (FLOAT)(m_pAnim->getTotalTqCrd() / 25);
        } else if (getAnalogHoukou() == HOUKOU_DOWN) {
            m_iSubState = 3;
            setAnimForce(0xC, 0, 0, 0, 0, 0);
            m_fLadderSpdY = -0.75f / (FLOAT)(m_pAnim->getTotalTqCrd() / 25);
        }
        break;
    case 2:
        if (isStartLadderJump()) return;
        if (m_iLadderStep >= m_iLadderStepMax) {
            m_fLadderSpdY = 0.0f;
            m_iSubState = 4;
            setAnimForce(0xD, 0, 1, 0, 0, 0);
            return;
        }
        if (isMotionEnd()) {
            if (getAnalogHoukou() != HOUKOU_UP) {
                m_iSubState = 1;
                setAnimForce(8, 0, 0, 0, 0, 0);
                m_fLadderSpdY = 0.0f;
            }
        }
        break;
    case 4: {
        FVECTOR pos;
        FMATRIX m;
        pos = m_vCenterPos;
        SubVectorXYZ(m_vHangHandPos, pos, getPos());
        UnitMatrix(m);
        RotMatrixY(m, m, m_fDir);
        InversMatrix(m, m);
        ApplyMatrixXYZ(m_vHangHandPos, m, m_vHangHandPos);
    }
        if (isMotionEnd()) {
            FLOAT dir;
            FVECTOR pos;
            dir = m_fDir;
            pos = getPos();
            FVECTOR ofs = {{0.0f, 1.8f, 1.3f, 1.0f}};
            FMATRIX m;
            UnitMatrix(m);
            RotMatrixY(m, m, dir);
            ApplyMatrixXYZ(ofs, m, ofs);
            AddVectorXYZ(pos, pos, ofs);
            setOldPos(pos);
            setPos(pos);
            setAnimForce(6, 0, 0, 0, 0, 0);
            initFall();
            return;
        }
        break;
    case 3:
        if (isStartLadderJump()) return;
        if (m_iLadderStep < 0 || getPos().y <= m_vLadderPos.y + 0.0035f) {
            m_iSubState = 5;
            setAnimForce(1, 400, 0, 0, 0, 0);
            break;
        }
        if (isMotionEnd()) {
            if (getAnalogHoukou() != HOUKOU_DOWN) {
                m_iSubState = 1;
                setAnimForce(8, 0, 0, 0, 0, 0);
                m_fLadderSpdY = 0.0f;
            }
        }
        break;
    case 5:
        if (getPos().y <= m_vLadderPos.y + 0.0035f) {
            m_fLadderSpdY = 0.0f;
            initStandUp(0);
            return;
        }
        break;
    }
    {
        FVECTOR vel = m_pMove->getVel();
        vel.y = m_fLadderSpdY * m_fFrameRate;
        m_pMove->setVel(vel);
    }
}
INT CPlayer::isStartLadderJump() {
    FLOAT dir;
    FVECTOR pos;
    if (!isTrgFlag(1)) return FALSE;
    dir = m_fDir;
    pos = getPos();
    FVECTOR ofs = { 0.0f, 0.0f, PL::LADDER_LOCAL_OFS.z, 1.0f };
    FMATRIX m;

    UnitMatrix(m);
    RotMatrixY(m, m, dir);
    ApplyMatrixXYZ(ofs, m, ofs);
    AddVectorXYZ(pos, pos, ofs);
    setPos(pos);
    {
        FVECTOR rot = getRot();
        FLOAT r = dir - 3.1415927f;
        if (r < -3.1415927f) r += 6.2831855f;
        else if (r > 3.1415927f) r -= 6.2831855f;
        dir = r;
        m_fDir = dir;
        rot.y = dir;
        setRot(rot);
    }
    setAnimForce(5, 0, 0, 0, 0, 0);
    initFall();
    m_fWalkSpeed = m_pParam->MAX_WALK_SPEED / 60.0f;
    m_pMove->calcVelocityXZ(m_fWalkSpeed);
    {
        FVECTOR vel = m_pMove->getVel();
        vel.y = PL::g_fAntiGravity * 18.0f;
        m_fJumpSpdY = vel.y;
        m_pMove->setVel(vel);
    }
    m_pMove->setFlag(0x40, 0); // partial CMovement: (flag, on)
    return TRUE;
}
INT CPlayer::getJumpState() const {
    switch (m_PlayerState) {
    case 6:
    case 7:
    case 8:
    case 10:
    case 11:
    case 12:
    case 13:
    case 14:
    case 0x13:
    case 0x16:
    case 0x18:
    case 0x19:
    case 0x1A:
    case 0x1B:
    case 0x1F:
    case 0x20:
    case 0x21:
    case 0x22:
    case 0x23:
    case 0x24:
        switch (m_JumpState) {
        case 0:
        default:
            return 0;
        case 1:
        case 2:
        case 5:
            return 1;
        case 3:
        case 4:
            return 2;
        case 6:
            return 3;
        }
    case 5:
    case 9:
    case 0x10:
    case 0x11:
    case 0x14:
    case 0x15:
    case 0x17:
    case 0x1C:
    case 0x1D:
        return 3;
    default:
        return 0;
    }
}
void CPlayer::initJump(INT inFlag) {
    setPlayerState(PL::JUMP);
    if (DebugWork::g_dw.player.iHiJump) setActionFlag(0x200000, 0);
    else setActionFlag(0, 0x200000);
    if (CGame::isSoraGame()) {
        UINT uiActionSkill = CGameWork::getInt(0, 0x24);
        if (uiActionSkill & 4) setActionFlag(0x800000, 0);
        else setActionFlag(0, 0x800000);
        if (uiActionSkill & 2) setActionFlag(0x400000, 0);
        else setActionFlag(0, 0x400000);
        if (DebugWork::g_dw.player.iGlide) setActionFlag(0x400000, 0);
        if (DebugWork::g_dw.player.iGlideSP) setActionFlag(0x800000, 0);
    }
    if (m_fAnalogPower >= 0.3f) {
        m_fDir = m_fAnalogViewDir;
        FVECTOR rot = getRot();
        rot.y = m_fDir;
        setRot(rot);
    }
    setPlayerFlag(0, 0x1000000);
    if (!inFlag) {
        if (isActionFlag(0x20000) && chkGimmick(0x2002) == 7) {
            setPlayerFlag(0x1000000, 0);
            setAnimForce(0x14, -1, 0, 0, 0, 0);
            m_fJumpPow = m_fGimmickJumpPow;
        } else if (isActionFlag(0x200000)) {
            setAnimForce(0x14, -1, 0, 0, 0, 0);
            m_fJumpPow = m_pParam->HIJUMP_POWER;
        } else {
            setAnimForce(4, -1, 0, 0, 0, 0);
            m_fJumpPow = m_pParam->JUMP_POWER;
        }
        m_vJumpStartPos = getPos();
        m_fJumpSpdXZ = m_fWalkSpeed * 0.8f;
        m_fJumpTop = 0.0f;
        m_fJumpSpdY = 0.0f;
    } else {
        if (m_fJumpSpdY > 0.0f) m_fJumpSpdY = 0.0f;
        m_fJumpPow = 0.0f;
        m_fJumpSpdXZ = m_fWalkSpeed;
    }
    m_fJumpDir = m_fDir;
    m_JumpState = 1;
    m_fJumpGravity = 0.0f;
    m_tqJumpTimer = 0;
    m_fJumpHeight = 0.0f;
}
void CPlayer::initFall() {
    initJump(1);
    *(INT*)&m_JumpState = 3;
    m_fJumpGravity = PL::g_fGravity;
    setAnimForce(6, 800, 0, 0, 0, 0);
    offAttack();
    setPlayerFlag(0, 0x020C2000);
    setBattleFlag(0, 0x00443F90);
    setFaceAnim(0, 0);
    m_pMove->setFlag(0x40, 0); // partial CMovement: (flag, on)
}
void CPlayer::setFallFloat() {
    FVECTOR v = m_pMove->getVel();
    m_fJumpSpdY = 0.0f;
    v.y = m_fJumpSpdY;
    m_pMove->setVel(v);
}
void CPlayer::exeJump() {
    FVECTOR vel = m_pMove->getVel();
    INT step;
    FLOAT aim_dir;
    FLOAT d0;
    FVECTOR v;
    FLOAT r;
    switch (m_JumpState) {
    case 1:
        clearSpeed();
        if (m_tqJumpTimer < m_pParam->JUMP_TQ_START) goto timer_end;
        m_JumpState = 2;
        m_fJumpGravity = *(FLOAT*)&PL::g_fGravity;
        vel.y = *(FLOAT*)&PL::g_fAntiGravity * m_fJumpPow;
        m_fJumpSpdY = vel.y;
        m_pMove->setVel(vel);
        goto timer_end;
    case 2:
        m_fJumpSpdY += m_fJumpGravity * m_fFrameRate;
        switch (m_iNowAnimID) {
        case 4:
            if (!isDownJump() && m_tqJumpTimer >= m_pParam->JUMP_TQ_START + 600) {
                m_JumpState = 3;
                m_fJumpSpdY *= *(FLOAT*)&PL::g_fJumpStopRate;
                setAnim(6, 800, 0);
            } else if (m_fJumpSpdY < 0.0f) {
                m_fJumpSpdY = 0.0f;
                setAnim(5, -1, 0);
            } else if (isMotionEnd()) {
                setAnim(5, -1, 0);
            }
            break;
        case 0x14:
            if (isPlayerFlag(0x1000000)) {
                if (m_fJumpSpdY < 0.0f) {
                    m_fJumpSpdY = 0.0f;
                    m_JumpState = 3;
                    setAnim(0x15, -1, 0);
                }
            } else if (!isDownJump() && m_tqJumpTimer >= m_pParam->JUMP_TQ_START + 600) {
                m_JumpState = 3;
                m_fJumpSpdY *= *(FLOAT*)&PL::g_fJumpStopRate;
            } else if (m_fJumpSpdY < 0.0f) {
                m_fJumpSpdY = 0.0f;
                m_JumpState = 3;
                setAnim(0x15, -1, 0);
            } else if (isMotionEnd()) {
                setAnim(0x15, -1, 0);
            }
            break;
        case 5:
        case 0x15:
            if (isMotionEnd()) {
                m_JumpState = 3;
                setAnim(6, -1, 0);
            }
            setGroundIK(1);
            break;
        }
        break;
    case 3:
        m_fJumpSpdY += m_fJumpGravity * m_fFrameRate;
        if (isMotionEnd()) setAnim(6, -1, 0);
        if (m_iNowAnimID == 4 && m_fJumpSpdY < 0.0f) m_fJumpSpdY = 0.0f;
        setGroundIK(1);
        break;
    case 6:
        exeJumpLand();
        setGroundIK(1);
        return;
    default:
        AssertApp(__FILE__, 0x1248, "0", 0);
        break;
    }
    if (isActionFlag(0x2000000)) {
        if (isTrgFlag(0x10)) chkChargeMove(0);
    }
    if (isTrgFlag(0x20)) {
        if (!m_bIsBattleScene) return;
        initMagic(0);
        return;
    } else if (isTrgFlag(4)) {
        if (!m_bIsBattleScene) initAttackField(0);
        else initAttack(0);
        return;
    } else if (m_JumpState < 4) {
        if (m_JumpState >= 2) {
            if (isActionFlag(0x8000000) && isTrgFlag(1)) {
                initJump2nd();
                return;
            }
            if (isActionFlag(0xC00000) && m_tqStateTimer >= 901 && isTrgFlag(1)) {
                initGlide(0);
                return;
            }
        }
    }
    vel.y = m_fJumpSpdY;
    m_pMove->setVel(vel);
    m_WalkMode = chkWalkMode();
    if (m_WalkMode != WALKMODE_STAND) {
        aim_dir = m_fAnalogViewDir;
        d0 = Movement::Homing(m_fDir, aim_dir, 0.1f, m_fFrameRate);
        v._q = 0;
        v.y = aim_dir;
        m_fDir = d0;
        m_pMove->getRot() = v;
        r = _limitPI2(aim_dir - d0);
        m_fMovePower = 1.0f - _abs(r) / 3.1415927f;
        m_fWalkSpeed = m_fWalkSpeed / 60.0f * m_fMovePower * 0.8f;
        if (m_fJumpSpdXZ != m_fWalkSpeed) {
            m_fJumpSpdXZ = m_fJumpSpdXZ + (m_fWalkSpeed - m_fJumpSpdXZ) * (m_fFrameRate * 0.05f);
            if (m_fWalkSpeed < m_fJumpSpdXZ) m_fJumpSpdXZ *= m_fMovePower;
        }
        m_fWalkSpeed = m_fJumpSpdXZ;
        m_pMove->calcVelocityXZ(m_fWalkSpeed);
        v.y = d0;
        m_pMove->getRot() = v;
    } else {
        m_fMovePower = 0.5f;
        if (m_fJumpSpdXZ != m_fWalkSpeed) {
            m_fJumpSpdXZ = m_fJumpSpdXZ + (m_fWalkSpeed - m_fJumpSpdXZ) * (m_fFrameRate * 0.05f);
        }
        m_fWalkSpeed = m_fJumpSpdXZ;
        m_pMove->calcVelocityXZ(m_fWalkSpeed);
    }
    if (getJumpState() == 2 && !isAir()) {
        m_JumpState = 6;
        setAnimForce(7, -1, 0, 0, 0, 0);
        m_fJumpSpdXZ = 0.0f;
        clearSpeed();
        setBattleFlag(0, 8);
        m_fJumpGravity = 0.0f;
        m_fJumpSpdY = 0.0f;
        return;
    }
    m_fJumpHeight = getPos().y - m_vJumpStartPos.y;
    if (m_fJumpTop < m_fJumpHeight) m_fJumpTop = m_fJumpHeight;
    step = chkLadderStepNow(1);
    if (step >= 2) {
        initLadder(step);
        return;
    }
    if (chkHang()) {
        initHang();
        return;
    }
timer_end:
    m_tqJumpTimer += m_tqFrameRate;
}
void CPlayer::exeJumpLand() {
    FVECTOR vel;
    if (m_bIsBattleScene) {
        if (isTrgFlag(0x400)) {
            initFooting(1);
            return;
        }
        if (isTrgFlag(8)) {
            initItem(0);
            return;
        }
        if (isTrgFlag(0x40)) {
            initGimmickCard(0);
            return;
        }
        if (isTrgFlag(0x80)) {
            initFriend(0);
            return;
        }
        if (isTrgFlag(0x100)) {
            initSummon(0);
            return;
        }
    }
    setBattleFlag(0, 8);
    if (isMotionEnd()) {
        initStandUp(0);
        return;
    }
    if (getMotionCurrentTime() >= 700 && m_fAnalogPower >= 0.3f) {
        if (isConditionFlag(0x10000000)) {
            initFooting(0);
            return;
        }
        m_WalkMode = chkWalkMode();
        setWalkAnim();
        switch (m_WalkMode) {
        case WALKMODE_STAND:
            setPlayerState(PL::STAND);
            break;
        case WALKMODE_WALK:
            setPlayerState(PL::WALK);
            break;
        case WALKMODE_RUN:
            setPlayerState(PL::RUN);
            break;
        }
        setPlayerFlag(0, 0x024C2000);
        setConditionFlag(0, 0x08810023);
        setBattleFlag(0, 0x00443F90);
        m_fMovePower = 0.5f;
        m_JumpState = 0;
        m_fWalkSpeed = m_fWalkSpeed / 60.0f * m_fMovePower;
        m_fJumpSpdY = 0.0f;
        return;
    }
    if (isTrgFlag(0x40)) {
        if (!m_bIsBattleScene) return;
        initGimmickCard(0);
    } else if (isTrgFlag(0x20)) {
        if (!m_bIsBattleScene) return;
        initMagic(0);
    } else if (isTrgFlag(4)) {
        if (!m_bIsBattleScene) initAttackField(0);
        else initAttack(0);
    } else {
        if (getMotionCurrentTime() >= 200 && isTrgFlag(1)) {
            chkWalkMode();
            FLOAT spd = m_fWalkSpeed / 60.0f;
            m_fMovePower = 1.0f;
            m_fJumpSpdXZ = spd;
            m_fWalkSpeed = spd;
            initJump(0);
            return;
        }
        m_fMovePower = 0.0f;
        m_fWalkSpeed = m_fJumpSpdXZ = Movement::Approach(m_fJumpSpdXZ, m_fMovePower, 0.1f, m_fFrameRate);
        m_pMove->calcVelocityXZ(m_fWalkSpeed);
        if (isAir()) {
            vel = m_pMove->getVel();
            vel.y = -0.05f;
            m_pMove->setVel(vel);
        } else {
            exeGimmickRide();
        }
    }
}
BOOL CPlayer::chkHang() {
    FVECTOR vel;
    FLOAT dir;
    FVECTOR ofs;
    FMATRIX m;
    FVECTOR pos;
    if (isBattleFlag(0x200000)) return FALSE;
    if (!isActionFlag(0x20)) return FALSE;
    if (isPlayerFlag(0x2000)) return FALSE;
    setPlayerFlag(0, 0x8000);
    vel = getVel();
    if (m_JumpState == 2) {
        if (!(vel.y < 0.0f)) return FALSE;
    } else if (m_JumpState != 3) {
        return FALSE;
    }
    if (!isAir()) return FALSE;
    ofs.z = 0.5f;
    ofs.x = 0.0f;
    ofs.y = 0.0f;
    ofs.w = 1.0f;
    dir = m_fDir;
    UnitMatrix(m);
    RotMatrixY(m, m, dir);
    ApplyMatrixXYZ(ofs, m, ofs);
    ApplyMatrixXYZ(pos, m, PL::HANG_HAND_LOCAL_OFS);
    _addAssign(pos, getPos());
    pos.y = m_vCenterPos.y + 0.05f + vel.y;
    FVECTOR vStart = {{ pos.x, pos.y, pos.z, 1.0f }};
    FVECTOR vEnd = {{ pos.x + ofs.x, pos.y + ofs.y, pos.z + ofs.z, 1.0f }};
    m_pCollMap->setType(COLL_TYPE_POINT);
    m_pCollMap->enableSliding(0);
    m_pCollMap->setShiftY(0.0f);
    m_pCollMap->setStartPos(vStart);
    m_pCollMap->setEndPos(vEnd);
    FVECTOR vNormal;
    FVECTOR vec;
    FVECTOR vDetect;
    COLL_INFO_UNION info;
    INT bRide = 0;
    if (!m_pCollMap->collideRide(0)) {
        if (!m_pCollMap->collideMap()) return FALSE;
        info = m_pCollMap->getDetectInfo();
    } else {
        bRide = 1;
        info = m_pCollMap->getDetectInfoO();
    }
    if (info.infoATTR == 1) return FALSE;
    vNormal = *m_pCollMap->getDetectNormal();
    if (vNormal.y >= 0.64278764f) return FALSE;
    FLOAT fInRot = _limitPI2(atan2f(vNormal.x, vNormal.z) + 3.1415927f);
    vec.x = 0.0f;
    vec.y = 0.0f;
    vec.z = 1.0f;
    vec.w = 1.0f;
    UnitMatrix(m);
    RotMatrixY(m, m, fInRot);
    ApplyMatrixXYZ(vec, m, vec);
    vDetect = *m_pCollMap->getDetectPos();
    ScaleVectorXYZ(ofs, vec, 0.1f);
    AddVectorXYZ(vDetect, vDetect, ofs);
    vStart.set(vDetect.x, vDetect.y + 0.5f, vDetect.z);
    vEnd.set(vDetect.x, vDetect.y, vDetect.z);
    m_pCollMap->setStartPos(vStart);
    m_pCollMap->setEndPos(vEnd);
    if (bRide) {
        if (!m_pCollMap->collideRide(0)) return FALSE;
        info = m_pCollMap->getDetectInfoO();
    } else {
        if (!m_pCollMap->collideMap()) return FALSE;
    }
    if (info.infoATTR == 1) return FALSE;
    vNormal = *m_pCollMap->getDetectNormal();
    if (vNormal.y < 0.64278764f) return FALSE;
    m_vHangPos = *m_pCollMap->getDetectPos();
    SubVectorXYZ(m_vHangPos, m_vHangPos, ofs);
    vStart.set(m_vHangPos.x - ofs.x, m_vHangPos.y + 0.1f, m_vHangPos.z - ofs.z);
    vEnd.set(m_vHangPos.x + ofs.x, m_vHangPos.y + 0.1f, m_vHangPos.z + ofs.z);
    m_pCollMap->setStartPos(vStart);
    m_pCollMap->setEndPos(vEnd);
    if (m_pCollMap->collideRide(0) || m_pCollMap->collideMap()) return FALSE;
    ofs.set(0.0f, 0.0f, -0.2f);
    ApplyMatrixXYZ(ofs, m, ofs);
    vStart.set(m_vHangPos.x + ofs.x, m_vHangPos.y, m_vHangPos.z + ofs.z);
    ofs.set(0.35f, 0.0f, 0.0f);
    ApplyMatrixXYZ(ofs, m, ofs);
    vEnd.set(vStart.x + ofs.x, vStart.y, vStart.z + ofs.z);
    m_pCollMap->setStartPos(vStart);
    m_pCollMap->setEndPos(vEnd);
    if (m_pCollMap->collideRide(0) || m_pCollMap->collideMap()) return FALSE;
    vEnd.set(vStart.x - ofs.x, vStart.y, vStart.z - ofs.z);
    m_pCollMap->setEndPos(vEnd);
    if (m_pCollMap->collideRide(0) || m_pCollMap->collideMap()) return FALSE;
    vEnd.y = vStart.y - 2.1f;
    m_pCollMap->setEndPos(vEnd);
    if (m_pCollMap->collideRide(0) || m_pCollMap->collideMap()) return FALSE;
    vEnd.x = vStart.x + ofs.x;
    vEnd.z = vStart.z + ofs.z;
    m_pCollMap->setEndPos(vEnd);
    if (m_pCollMap->collideRide(0) || m_pCollMap->collideMap()) return FALSE;
    if (bRide) setPlayerFlag(0x8000, 0);
    m_fDir = fInRot;
    return TRUE;
}
void CPlayer::initHang() {
    FVECTOR rot;
    FVECTOR ofs;
    FMATRIX m;
    m_vOldSafeField = getPos();
    setPlayerState(PL::HANG);
    m_iSubState = 0;
    setPlayerFlag(0x400000, 0);
    setBattleFlag(0, 8);
    rot = getRot();
    FLOAT dir = m_fDir;
    rot.y = dir;
    setRot(rot);
    UnitMatrix(m);
    RotMatrixY(m, m, dir);
    ApplyMatrixXYZ(ofs, m, PL::HANG_HAND_LOCAL_OFS);
    SubVectorXYZ(m_vHangPos, m_vHangPos, ofs);
    m_vHangPos.w = 1.0f;
    if (isPlayerFlag(0x8000)) {
        setPos(m_vHangPos);
        setConditionFlag(0, 0x40000);
        m_pCollMap->registerLastHang(getHandle());
        m_pCollMap->setHangPosition(rot, m_vHangPos);
    } else {
        warp(m_vHangPos);
    }
    setAnimForce(0xF, 0, 0, 0, 0, 100);
    updateCoord();
    coordFix();
    m_pMove->setFlag(0, 0x40); // partial CMovement: (flag, on)
    m_pCam->setFlag(2, 0);
    m_JumpState = 6;
    clearSpeed();
    m_fJumpSpdY = 0.0f;
    if (m_pMarker) m_pMarker->cancelLockon(m_hLockon, 0);
}
void CPlayer::exeHang() {
    FLOAT dir;
    Houkou houkou;
    FVECTOR pos;
    FVECTOR rot;
    FVECTOR ofs;
    FMATRIX m;
    FLOAT r;
    setPlayerFlag(0x400000, 0);
    setBattleFlag(0, 8);
    switch (m_iSubState) {
    case 0:
    case 1:
        if (isPlayerFlag(0x8000)) {
            if (isConditionFlag(0x40000) || m_pMove->isGround() || m_pMove->isWall()) {
                setConditionFlag(0, 0x40000);
                m_pCollMap->removeHang();
                setPlayerFlag(0, 0x8000);
                setTrgFlag(1, 0);
                if (m_tqStateTimer < 1500) m_tqStateTimer = 1500;
            } else if (m_pCollMap->getHangPosition(rot, pos)) {
                setPos(pos);
                setRot(rot);
            }
        }
        if (m_iNowAnimID == 0xF) {
            if (m_tqStateTimer < 1500) break;
            if (isMotionEnd()) {
                pos = Crd(m_pModels->getBoneIndex(PL::g_szBoneLHand)).getWorldMatrix().v[3];
                SubVectorXYZ(m_vHangHandPos, pos, getPos());
                UnitMatrix(m);
                RotMatrixY(m, m, m_fDir);
                InversMatrix(m, m);
                ApplyMatrixXYZ(m_vHangHandPos, m, m_vHangHandPos);
                m_iSubState = 1;
                setAnimForce(0xE, 400, 0, 0, 0, 0);
            }
        }
        r = _limitPI2(m_fAnalogViewDir - getDir() + 3.1415927f);
        if (getAnalogHoukou()) houkou = getHoukouFromAngle(r);
        else houkou = HOUKOU_NUTRAL;
        if (isTrgFlag(1) || getAnalogHoukou() == HOUKOU_DOWN || houkou == HOUKOU_DOWN) {
            r = getDir(); dir = r;
            pos = getPos();
            ofs.set(0.0f, 0.0f, -0.6f);
            UnitMatrix(m);
            RotMatrixY(m, m, dir);
            ApplyMatrixXYZ(ofs, m, ofs);
            AddVectorXYZ(pos, pos, ofs);
            ofs.set(0.5f, 0.0f, 0.0f);
            ApplyMatrixXYZ(ofs, m, ofs);
            FVECTOR vStart = {{ pos.x + ofs.x, pos.y, pos.z + ofs.z, 1.0f }};
            FVECTOR vEnd = {{ pos.x - ofs.x, pos.y, pos.z - ofs.z, 1.0f }};
            m_pCollMap->setType(COLL_TYPE_SPHERE);
            m_pCollMap->enableSliding(0);
            m_pCollMap->setRadius(0.5f);
            m_pCollMap->setStartPos(vStart);
            m_pCollMap->setEndPos(vEnd);
            if (m_pCollMap->collideRide(0) || m_pCollMap->collideMap()) pos = *m_pCollMap->getDetectPos();
            m_pCollMap->setStartPos(vEnd);
            m_pCollMap->setEndPos(vStart);
            if (m_pCollMap->collideRide(0) || m_pCollMap->collideMap()) pos = *m_pCollMap->getDetectPos();
            m_pCollMap->setStartPos(pos);
            vEnd = getPos();
            m_pCollMap->setEndPos(vEnd);
            if (m_pCollMap->collideRide(0) || m_pCollMap->collideMap()) pos = *m_pCollMap->getDetectPos();
            setPos(pos);
            rot = getRot();
            r = _limitPI2(dir - 3.1415927f);
            setDir(r);
            rot.y = r;
            setRot(rot);
            setAnimForce(5, 0, 0, 0, 0, 0);
            initFall();
            m_fWalkSpeed = m_pParam->MAX_WALK_SPEED / 60.0f;
            m_pMove->calcVelocityXZ(m_fWalkSpeed);
            FVECTOR vel = m_pMove->getVel();
            vel.y = PL::g_fAntiGravity * 12.0f;
            m_fJumpSpdY = vel.y;
            m_pMove->setVel(vel);
            if (isPlayerFlag(0x8000)) {
                m_pCollMap->removeHang();
                setPlayerFlag(0, 0x8000);
            }
            return;
        }
        if (getAnalogHoukou() == HOUKOU_UP || houkou == HOUKOU_UP) {
            m_iSubState = 2;
            setAnimForce(0x10, 0, 0, 0, 0, 0);
            m_pAnim->setRootMove(1, 1);
            if (isPlayerFlag(0x8000)) {
                m_pCollMap->removeHang();
                setPlayerFlag(0, 0x8000);
            }
            m_vHangPos = getPos();
        }
        break;
    case 2:
        pos = getPos();
        UnitMatrix(m);
        RotMatrixY(m, m, m_fDir);
        ofs.set(0.0f, 2.564f, -1.0f);
        ApplyMatrixXYZ(ofs, m, ofs);
        AddVectorXYZ(ofs, m_vHangPos, ofs);
        setOldPos(ofs);
        if (isMotionEnd()) {
            ofs.set(0.0f, 1.7f, 0.05f);
            ApplyMatrixXYZ(ofs, m, ofs);
            AddVectorXYZ(pos, pos, ofs);
            setPos(pos);
            setAnimForce(6, 0, 0, 0, 0, 0);
            initFall();
            setPlayerFlag(0, 0x8000);
            return;
        }
        break;
    }
    if (isPlayerFlag(0x8000)) {
        pos = getPos();
        rot = getRot();
        m_pCollMap->setHangPosition(rot, pos);
    }
}
void CPlayer::initAvoid() {
    if (isConditionFlag(0x10000000) && !isConditionFlag(0x400000)) {
        initFooting(2);
        return;
    }
    setPlayerState(PL::AVOID);
    setBattleFlag(0, 8);
    setPlayerFlag(1, 0);
    clearSpeed();
    if (m_fAnalogPower >= 0.3f) {
        m_fDir = m_fAnalogViewDir;
        FVECTOR rot = getRot();
        rot.y = m_fDir;
        setRot(rot);
    }
    updateCoord();
    setAnimForce(0x16, -1, TRUE, 0, 0, 0);
    m_fJumpSpdY = 0.0f;
    m_JumpState = 3;
    m_fJumpGravity = *(FLOAT*)&PL::g_fGravity;
}
void CPlayer::exeGimmickRide() {
    if (m_pMove->isObjRide() && chkGimmick(0x2002) == 7 && !CGameWork::getFlag(1, 0x27)) {
        CEntity* pEntity = (CEntity*)CTaskMan::getTask(m_hGimmick);
        if (pEntity) {
            FVECTOR target = pEntity->getPos();
            FVECTOR pos = getPos();
            if (calcDistanceXZSq(target) < 0.010000001f) {
                pos.x = target.x;
                pos.z = target.z;
            } else {
                pos.x = Movement::Approach(pos.x, target.x, 0.1f, m_fFrameRate);
                pos.z = Movement::Approach(pos.z, target.z, 0.1f, m_fFrameRate);
            }
            setPos(pos);
        }
    }
}
void CPlayer::notifyGimmickJump(HANDLE inHandle, INT inPow) {
    if (!_instance()) return;
    _instance()->m_hGimmickJump = inHandle;
    if (inPow == -1) {
        _instance()->m_fGimmickJumpPow = PL::g_fGimmickJumpPower[((CBaseGimmick*)CTaskMan::getTask(inHandle))->getParam()];
    } else {
        _instance()->m_fGimmickJumpPow = PL::g_fGimmickJumpPower[inPow];
    }
}
__asm__(".rdata\n");
INT CPlayer::chkGimmick(UINT inID) {
    INT result;
    HANDLE handle = m_hGimmickJump;
    CEntity* pEntity;
    FLOAT dir;
    if (handle <= 0) {
        handle = m_hTarget;
        if (handle <= 0) return 0;
    }
    pEntity = (CEntity*)CTaskMan::getTask(handle);
    if (!pEntity) return 0;
    if (inID && pEntity->getTypeID() != inID) return 0;
    switch (pEntity->getTypeID()) {
    case 0x2002:
        result = m_pMove->isObjRide() ? 7 : 0;
        break;
    case 0x4000:
        result = 0x1F;
        break;
    case 0x2003:
        dir = calcDirection(pEntity->getPos());
        if (_abs(_limitPI(dir - m_fDir)) * 57.295776f > 75.0f) return 0;
        if (!((CGimmickCarry*)pEntity)->checkCarryPermission()) return 0;
        procTargetMark();
        result = 0x1E;
        break;
    case 0x2000:
        if (((CGimmickTreasureBox*)pEntity)->isOpenTresureBox()) return 0;
        if (_abs(_limitPI(pEntity->getRot().y - m_fDir)) < 1.5707964f) return 0;
        dir = pEntity->calcDirection(m_vCenterPos);
        if (_abs(_limitPI2(pEntity->getRot().y - dir)) * 57.295776f > 75.0f) return 0;
        if (!((CGimmickTreasureBox*)pEntity)->checkOpenTresureBoxPermission()) return 0;
        ((CGimmickTreasureBox*)pEntity)->OpenTreasureBox();
        m_pMarker->holdTarget(8);
        result = 0x11;
        break;
    case 0x1006:
        if (CSceneInfo::getRoomInfo(-1)->_doors[((CGimmickDoor*)pEntity)->getDoorID()].canSelectWorld()) {
            FLOAT r = _limitPI(pEntity->getRot().y - m_fDir);
            if (getHoukouFromAngle(r) == 4) return 0;
            new (__FILE__ "(5804)") CWorldSelect(CTaskMan::getSysTask(1), handle);
            m_pMarker->holdTarget(8);
            result = 0x15;
        } else {
            if (!CSceneInfo::getRoomInfo(-1)->_doors[((CGimmickDoor*)pEntity)->getDoorID()].canCreateRoom()) return 0;
            FLOAT r = _limitPI(pEntity->getRot().y - m_fDir);
            if (getHoukouFromAngle(r) == 4) return 0;
            new (__FILE__ "(5819)") CRoomCreation(CTaskMan::getSysTask(1), handle);
            m_pMarker->holdTarget(8);
            result = 0x14;
        }
        break;
    default:
        return 0;
    }
    m_hGimmick = handle;
    return result;
}
void CPlayer::initTreasureBox() {
    CEntity* pTarget;
    FVECTOR rot;
    FVECTOR pos;
    setPlayerState(TREASUREBOX);
    setPlayerFlag(0x400001, 0);
    clearSpeed();
    m_collBody.off();
    onWeapon(1);
    m_pMarker->off();
    setAnimRsrcEx(0xFDC, 0, 0, 0, 0, 0, 1);
    m_bDmgMuteki = TRUE;
    pTarget = (CEntity*)CTaskMan::getTask(m_hGimmick);
    FLOAT d = pTarget->getRot().y + 3.1415927f;
    FLOAT r;
    if (d < -3.1415927f) r = d + 6.2831855f;
    else {
        r = d;
        if (r > 3.1415927f) r -= 6.2831855f;
    }
    m_fDir = r;
    rot = getRot();
    rot.y = m_fDir;
    setRot(rot);
    pos = pTarget->getPos();
    FVECTOR ofs = {{0.0f, 0.0f, -1.457f, 1.0f}};
    FMATRIX m;
    UnitMatrix(m);
    RotMatrixY(m, m, m_fDir);
    ApplyMatrixXYZ(ofs, m, ofs);
    AddVectorXYZ(pos, pos, ofs);
    pos.y += 0.0035f;
    setPos(pos);
    setOldPos(pos);
}
void CPlayer::exeTreasureBox() {
    setPlayerFlag(0x400000, 0);
    if (isMotionEnd()) {
        m_bDmgMuteki = FALSE;
        setPlayerFlag(0, 1);
        m_collBody.on();
        initStandUp(0);
        m_pMarker->on();
        m_pMarker->holdTarget(0);
    }
}
void CPlayer::initRoomCreation() {
    CEntity* pTarget;
    FVECTOR rot;
    FVECTOR pos;
    setTrgFlag(0, 0x18000);
    setPlayerState(ROOMCREATION);
    m_iSubState = 0;
    clearSpeed();
    m_collBody.off();
    m_bDmgMuteki = TRUE;
    offWeapon();
    m_pMarker->off();
    setAnimForce(1, -1, 0, 0, 0, 0);
    pTarget = (CEntity*)CTaskMan::getTask(m_hGimmick);
    FLOAT d = pTarget->getRot().y + 3.1415927f;
    FLOAT r;
    if (d < -3.1415927f) r = d + 6.2831855f;
    else {
        r = d;
        if (r > 3.1415927f) r -= 6.2831855f;
    }
    m_fDir = r;
    rot = getRot();
    rot.y = m_fDir;
    setRot(rot);
    pos = pTarget->getPos();
    FVECTOR ofs = {{0.0f, 0.0f, -2.0f, 1.0f}};
    FMATRIX m;
    UnitMatrix(m);
    RotMatrixY(m, m, m_fDir);
    ApplyMatrixXYZ(ofs, m, ofs);
    AddVectorXYZ(pos, pos, ofs);
    pos.y += 0.0035f;
    setOldPos(pos);
    setPos(pos);
    setLand(1.0f, 0.5f, 1);
    m_pCam->initCamera(1, 1, NULL);
    m_pCam->setFlag(1, 0);
}
void CPlayer::exeRoomCreation() {
    setPlayerFlag(0x400000, 0);
    switch (m_iSubState) {
    case 0:
        m_iSubState = 1;
    case 1:
        if (isTrgFlag(0x10000)) {
            m_iSubState = 2;
            setTrgFlag(0, 0x10000);
            if (CGame::isSoraGame()) setAnimRsrcEx(0xFE6, 0, 0, 1, 0, 0, 1);
            else setAnimRsrcEx(0xFE8, 0, 0, 1, 0, 0, 1);
        } else if (isTrgFlag(0x8000)) {
            m_iSubState = 4;
            setTrgFlag(0, 0x8000);
            m_WalkMode = WALKMODE_STAND;
            m_pCam->setFlag(0, 1);
            m_bDmgMuteki = FALSE;
            m_collBody.on();
            initStandUp(0);
            m_pMarker->on();
            m_pMarker->holdTarget(0);
        }
        break;
    case 2:
        if (isMotionEnd()) {
            m_iSubState = 3;
            m_pMove->setFlag(1, 0);
            setPlayerFlag(1, 0);
            setAnim(1, -1, 0);
            m_tqStateTimer = 0;
            m_fMovePower = 0.0f;
        }
        break;
    case 3:
        if (m_tqStateTimer > 3000) {
            setAnim(2, -1, 0);
            m_WalkMode = WALKMODE_WALK;
            m_fMovePower = 1.0f;
            m_fWalkSpeed = m_pParam->MAX_WALK_SPEED / 60.0f;
            m_pMove->calcVelocityXZ(m_fWalkSpeed);
        }
        break;
    case 4:
        break;
    }
}
void CPlayer::initWorldSelect() {
    CEntity* pTarget;
    FVECTOR rot;
    FVECTOR pos;
    setTrgFlag(0, 0x18000);
    setPlayerState(WORLDSELECT);
    m_iSubState = 0;
    clearSpeed();
    m_collBody.off();
    m_bDmgMuteki = TRUE;
    offWeapon();
    m_pMarker->off();
    setAnimForce(1, -1, 0, 0, 0, 0);
    pTarget = (CEntity*)CTaskMan::getTask(m_hGimmick);
    FLOAT d = pTarget->getRot().y + 3.1415927f;
    FLOAT r;
    if (d < -3.1415927f) r = d + 6.2831855f;
    else {
        r = d;
        if (r > 3.1415927f) r -= 6.2831855f;
    }
    m_fDir = r;
    rot = getRot();
    rot.y = m_fDir;
    setRot(rot);
    pos = pTarget->getPos();
    FVECTOR ofs = {{0.0f, 0.0f, -2.0f, 1.0f}};
    FMATRIX m;
    UnitMatrix(m);
    RotMatrixY(m, m, m_fDir);
    ApplyMatrixXYZ(ofs, m, ofs);
    AddVectorXYZ(pos, pos, ofs);
    pos.y += 0.0035f;
    setPos(pos);
    setOldPos(pos);
    m_pCam->initCamera(1, 1, NULL);
    m_pCam->setFlag(1, 0);
}
void CPlayer::exeWorldSelect() {
    setPlayerFlag(0x400000, 0);
    switch (m_iSubState) {
    case 0:
        m_iSubState = 1;
    case 1:
        if (isTrgFlag(0x10000)) {
            m_iSubState = 2;
            setTrgFlag(0, 0x10000);
            if (CGame::isSoraGame()) setAnimRsrcEx(0xFBE, 0, 0, 1, 0, 0, 1);
            else setAnimRsrcEx(0xFC0, 0, 0, 1, 0, 0, 1);
        } else if (isTrgFlag(0x8000)) {
            m_iSubState = 4;
            setTrgFlag(0, 0x8000);
            m_WalkMode = WALKMODE_STAND;
            m_pCam->setFlag(0, 1);
            m_bDmgMuteki = FALSE;
            m_collBody.on();
            initStandUp(0);
            m_pMarker->on();
            m_pMarker->holdTarget(0);
        }
        break;
    case 2:
        if (isMotionEnd()) {
            m_iSubState = 3;
            m_pMove->setFlag(1, 0);
            setPlayerFlag(1, 0);
            setAnim(1, -1, 0);
            m_tqStateTimer = 0;
            m_fMovePower = 0.0f;
        }
        break;
    case 3:
        if (m_tqStateTimer > 3000) {
            setAnim(2, -1, 0);
            m_WalkMode = WALKMODE_WALK;
            m_fMovePower = 1.0f;
            m_fWalkSpeed = m_pParam->MAX_WALK_SPEED / 60.0f;
            m_pMove->calcVelocityXZ(m_fWalkSpeed);
        }
        break;
    case 4:
        break;
    }
}
void CPlayer::initCarry() {
    setPlayerState(PL::CARRY);
    m_iSubState = 0;
    clearSpeed();
    offWeapon();
    setAnimForce(0x17, 0, 0, 0, 0, 0);
    m_pCam->setFlag(0x800, 0);
}
void CPlayer::exeCarry() {
    TIMEQ frameTQ;
    CGimmickCarry* pGimmick;
    FVECTOR pos;
    FVECTOR rot;
    WalkMode PreWalkMode;
    FLOAT aim_power;
    INT frame;
    frameTQ = getMotionCurrentTime();
    frame = frameTQ / 50;
    rot = getRot();
    if (m_pMarker) m_pMarker->m_uiFlag |= 0x80;
    if (m_hGimmick > 0) {
        pGimmick = (CGimmickCarry*)CTaskMan::getTask(m_hGimmick);
        if (!pGimmick || pGimmick->getTypeID() != 0x2003) {
            m_hGimmick = 0;
            m_iSubState = 4;
            initStandUp(0);
            return;
        }
    }
    switch (m_iSubState) {
    case 0:
        m_pMove->homingY(m_vMarkPos, 0.1f);
        if (calcDistanceXZ(m_vMarkPos) > 0.9f) {
            m_fWalkSpeed = m_pParam->MAX_RUN_SPEED / 60.0f;
            m_pMove->calcVelocityXZ(m_fWalkSpeed);
        } else {
            m_fWalkSpeed = 0.0f;
            m_pMove->calcVelocityXZ(m_fWalkSpeed);
        }
        if (frameTQ >= 700) {
            m_pMove->homingY(m_vMarkPos, 1.0f);
            clearSpeed();
            m_fWalkSpeed = 0.0f;
            m_iSubState = 1;
            pGimmick->lift();
        }
        break;
    case 1:
        if (isMotionEnd()) {
            m_iSubState = 2;
            setAnimForce(0x18, 0, 0, 0, 0, 0);
            pGimmick->CarryStand();
        }
        break;
    case 2:
        if (isAir()) {
            pGimmick->CancelCarry();
            m_hGimmick = 0;
            initFall();
            break;
        }
        if (isTrgFlag(0x204)) {
            m_iSubState = 3;
            clearSpeed();
            m_pMarker->holdTarget(8);
            setAnimForce(0x1A, 0, 0, 0, 0, 0);
            break;
        }
        PreWalkMode = m_WalkMode;
        aim_power = m_fAnalogPower;
        if (aim_power < 0.3f) {
            m_fWalkSpeed = 0.0f;
            m_WalkMode = WALKMODE_STAND;
        } else {
            m_fWalkSpeed = m_pParam->CARRY_MOVE_SPEED;
            m_WalkMode = WALKMODE_WALK;
        }
        if (PreWalkMode != m_WalkMode) {
            if (m_WalkMode == WALKMODE_STAND) {
                if (m_pSE) ((CSEMotion*)m_pSE)->playFootStopSE();
                pGimmick->CarryStand();
            } else {
                pGimmick->CarryWalk();
            }
        }
        exeWalk();
        break;
    case 3:
        if (m_hGimmick > 0) {
            procTargetMark();
            if (m_hTarget > 0) m_pMove->homingY(m_vMarkPos, 1.0f);
            m_fWalkSpeed = 0.0f;
            m_pMove->calcVelocityXZ(m_fWalkSpeed);
            if (m_hTarget > 0) {
                pGimmick->ThrowGimmick(m_vMarkPos);
            } else {
                calcAppearPos(5.0f, 0.5f, NULL);
                pGimmick->ThrowGimmick(m_vAppearPos);
            }
            m_hGimmick = 0;
            m_pMarker->holdTarget(0);
        } else {
            m_pCam->setFlag(0x800, 0);
            if (isMotionEnd()) {
                m_hGimmick = 0;
                m_iSubState = 4;
                initStandUp(0);
            }
        }
        break;
    case 4:
        m_hGimmick = 0;
        initStandUp(0);
        break;
    }
}
void CPlayer::resetAction(INT inMode) {
    setConditionFlag(0, 0x1000000);
    if (m_fScale != 1.0f) {
        FVECTOR scale;
        m_fScale = 1.0f;
        scale.x = 1.0f;
        scale.y = 1.0f;
        scale.z = 1.0f;
        scale.w = 1.0f;
        setScale(scale);
    }
    m_fWallRadius = 0.5f;
    m_fFootRadius = 0.25f;
    if (m_pPLM->getFriend()) m_pPLM->getFriend()->setExit(0);
    if (m_pPLM->getSummon()) m_pPLM->getSummon()->setExit(1);
    if (m_pPLM->getFriendEx()) m_pPLM->getFriendEx()->setExit(1);
    if (isPlayerFlag(0x80000000)) setBGFade(1.0f, 0.0f, 0);
    setupFade(0.0f, 1.0f, 50, -1);
    setModelAlphaScale(1.0f, -1);
    fadeEffect(m_hEffCharge, 0);
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
    killBullet();
    if (m_szEffectName) {
        CEffectMan::killAll(m_szEffectName, 10.0f);
        m_szEffectName = NULL;
    }
    if (isPlayerFlag(0x8000)) {
        m_pCollMap->removeHang();
        setPlayerFlag(0, 0x8000);
    }
    setConditionFlag(0, 0x40000);
    setConditionFlag(0, 0x80000);
    PlayerCamera::CPlayerCamera::setBattleCamData(0, NULL);
    m_pCam->changeNowPlayer(0, 0);
    m_pCam->changeCamMode(-1, 0);
    setPlayerFlag(0, 0x10000);
    setBattleFlag(0, 0x10200000);
    setUseRsrcBlock(0);
    switch (inMode) {
    case 6:
        m_iAttackNo = 0;
    case 1:
        setObjStop(0);
        break;
    case 3:
    case 4:
    case 5:
        setObjStop(0);
        m_uiAttackKind = 0;
        m_uiAttackID = 0;
        m_iAttackNo = 0;
        m_bIsAttackHit = FALSE;
        m_bNextTrg = FALSE;
        m_iNextAttack = 0;
        m_collBody.setDamage(0);
        offAttack();
        onWeapon(0);
        setBattleFlag(0, 8);
        setBattleFlag(0, 0x00443F90);
        setBattleFlag(0, 0x80000);
        setBattleFlag(0, 0x10200000);
        setPlayerFlag(0, 0x020C2000);
        setBattleFlag(0, 0x00130000);
        setPlayerFlag(0, 0x00401001);
        setBattleFlag(0, 0x03000000);
        setEntityFlag(0, 1);
        setPushon(0);
        m_pMarker->holdTarget(0);
        m_pMove->setFlag(0, 1);
        m_pMove->setFlag(0x40, 0);
        setConditionFlag(0, 0x200000);
        if (m_bIsMinigameScene) MG_SKILL::killMGSkill();
        switch (m_iStockSkill) {
        case 0x61: {
            FVECTOR pos;
            warp(m_vStartPos);
            pos = m_vStartPos;
            pos.y = 7.0f;
            setOldPos(pos);
            break;
        }
        case 0x27:
            setEnemyStop(0, 0);
            break;
        }
        break;
    case 7:
        m_uiAttackKind = 0;
        m_uiAttackID = 0;
        m_bIsAttackHit = FALSE;
        m_bNextTrg = FALSE;
        m_iNextAttack = 0;
        m_bIsCanCombo = FALSE;
        offAttack();
        m_pMarker->cancelLockon(m_hLockon, 0);
        m_pMarker->off();
        if (m_bIsMinigameScene) MG_SKILL::killMGSkill();
        break;
    case 8:
    case 9:
        setPlayerFlag(0, 0x020C2000);
        setBattleFlag(0, 8);
        setBattleFlag(0, 0x00443F90);
        setBattleFlag(0, 0x4060);
        setBattleFlag(0, 0x00130000);
        setBattleFlag(0, 0x03000000);
        m_pMarker->cancelLockon(m_hLockon, 0);
        m_pMarker->off();
        offAttack();
        setPlayerFlag(1, 0x00401000);
        setPushon(0);
        m_pMarker->holdTarget(0);
        m_pMove->setFlag(0, 1);
        if (m_bIsMinigameScene) MG_SKILL::killMGSkill();
        if (m_iStockSkill == 0x61) warp(m_vAppearPos);
        break;
    case 0:
    case 2:
        break;
    }
}
void CPlayer::initAttackField(INT inNo) {
    setPlayerState(PL::ATTACK_FLD);
    m_iAerialAnimShift = 0;
    setTrgFlag(0, 4);
    setPlayerFlag(0x100000, 0);
    m_pMarker->holdTarget(8);
    m_uiCardID = 0;
    m_iAttackNo = inNo;
    if (!inNo) m_uiPreAttackKind = 0;
    m_bIsAttackHit = FALSE;
    m_bNextTrg = FALSE;
    m_iNextAttack = 0;
    *(FLOAT*)&m_fMoveSpdXZ = isAir() ? m_fJumpSpdXZ : m_fWalkSpeed;
    /* MATCH: the empty barriers and pBreak in v1 hold the load, the store and the %hi in the original's issue order;
       the original's $LM lines have gravity first, then JumpState, CardBreakTime, SpdY. */
    FLOAT fScale = 0.16f;
    FLOAT fGravity = PL::g_fGravity;
    __asm__ volatile("");
    m_JumpState = 3;
    __asm__ volatile("");
    register TIMEQ* pBreak asm("$3") = PL::g_CardBreakTime;
    __asm__ volatile("");
    m_fJumpGravity = fGravity * fScale;
    m_tqCardBreakTime = pBreak[7];
    m_fJumpSpdY = 0.0f;
    setAttack();
    m_fDistXZ = m_fDistY = 0.0f;
    if (m_pAtkData->FLAG & 8) {
        setPlayerFlag(0x2800000, 0);
    } else {
        setPlayerFlag(0, 0x2800000);
        m_fJumpGravity = PL::g_fGravity;
    }
    onWeapon(1);
}
void CPlayer::exeAttackField() {
    TIMEQ frameTQ = getMotionCurrentTime();
    TIMEQ frame = frameTQ / 50;
    procGravity(frame, m_pAtkData->MOVE_END_FRAME, 1);
    if (frame <= m_pAtkData->MOVE_START_FRAME) {
        procTargetMark();
        procAttackMove(frame, 1.0f, 4.0f, 2, 1.0f);
    } else {
        FLOAT rate = 0.0f;
        if (frame < 6 && (m_pAtkData->FLAG & 8)) {
            procTargetMark();
            rate = 1.0f;
        }
        procAttackMove(frame, m_pAtkData->RADIUS * 0.1f, m_pAtkData->MAX_SPEED, !m_bIsAttackHit, rate);
    }
    if (frame > m_pAtkData->COLL_START_FRAME && frameTQ > 500) {
        if (m_iNextAttack) {
            offAttack();
            initAttackField(m_iAttackNo);
            return;
        }
        if (m_bIsCanCombo && m_bIsAttackHit && m_iAttackNo < m_iMaxCombo && isTrgAttack()) {
            m_iNextAttack = 1;
            return;
        }
    }
    if (procAttackCollision(frame)) return;
    if (isPlayerFlag(0x800000) && !isAir() && frame > m_pAtkData->COLL_END_FRAME) {
        procSkillEnd(1);
        return;
    }
    if (isMotionEnd()) procSkillEnd(-1);
}
void CPlayer::chkLockon() {
    HANDLE near;
    INT bCheck;
    if (!m_pMarker->m_bFlag) return;
    if (isPlayerFlag(0x10000)) return;
    if (m_PlayerState == 0x1E) near = LockOnManager::CLockOnManager::getLockOnMapEnemy(*this);
    else near = LockOnManager::CLockOnManager::getLockOnObject(*this);
    if (near == -1) near = 0;
    m_hTarget = near;
    bCheck = FALSE;
    if (m_bIsBattleScene && m_PlayerState != HANG && !CGameWork::getFlag(1, 0xC)) {
        if (!m_pMarker->m_iMode) {
            if (isTrgLockon()) {
                m_pMarker->changeMode();
                bCheck = TRUE;
                m_pMarker->chkLockon(m_hTarget);
                if (m_hTarget > 0) CSound::playSE(7, -1, 0x40);
                setTrgFlag(0, 0x1000);
            }
        } else if (isRTrgLockon() && (m_uiTrgFlag & 0x1000)) {
            m_pMarker->changeMode();
            bCheck = TRUE;
            m_pMarker->chkLockon(m_hTarget);
            if (m_hTarget > 0) CSound::playSE(8, -1, 0x40);
        } else if (isDownLockon()) {
            if (isTrgLockon()) setTrgFlag(0x1000, 0);
            if (isChangeHoukouR()) {
                HANDLE hChange = LockOnManager::CLockOnManager::changeLockOnObject(m_hLockon);
                if (hChange > 0) {
                    m_hTarget = hChange;
                    m_pMarker->setMode(1);
                    bCheck = TRUE;

                    m_pMarker->chkLockon(m_hTarget);
                    CSound::playSE(7, -1, 0x40);
                }
            }
            setTrgFlag(0x800, 0);
        } else {
            setTrgFlag(0, 0x1000);
        }
    }
    if (!bCheck) m_pMarker->chkLockon(m_hTarget);
    m_hLockon = m_pMarker->m_hLockon;
    m_vLockonPos = m_pMarker->m_vCameraTargetPos;
    if (m_hLockon > 0) m_hTarget = m_hLockon;
}
void CPlayer::cancelLockon() {
    m_pMarker->cancelLockon(m_hLockon, TRUE);
}
void CPlayer::autoLockon() {
    if (m_bIsBattleScene && !m_hLockon) m_pMarker->setMode(1);
}
void CPlayer::setPushon(INT inFlag) {
    if (m_bIsBattleScene) {
        if (inFlag) {
            m_pMarker->onPush(inFlag);
        } else {
            m_pMarker->offPush();
            m_uiTrgFlag &= ~0x40000;
        }
    }
}
void CPlayer::setTrgFlagRendaNG(const CHAR* inName) {
    if (!DebugWork::g_dw.player.iRenda) m_uiTrgFlag |= 0x40000;
}
__asm__(".rdata\n");
void CPlayer::chkPlayCard() {
    if (m_bIsDead) return;
    if (!m_pRevolver) return;
    BOOL bExeSkill = FALSE;
    INT iStock = m_pRevolver->getStock(1);
    if (isTrgStock()) {
        if (iStock < 3) m_pRevolver->setStockCard();
        else bExeSkill = TRUE;
    }
    if (!isBattleFlag(8)) {
        setBattleFlag(8, 0);
        return;
    }
    if (bExeSkill || (isTrgStockSkill() && iStock > 0)) {
        INT result = m_pRevolver->setPlayStock(0, 1);
        if (result) {
            TRACE("CPlayer::chkPlayCard(); Stock TRG!\n");
            setBattleFlag(0, 8);
            if (result == 1) {
                setBattleFlag(0x10200000, 0x20);
            } else {
                TRACE("REFRECT\n");
                setBattleFlag(0, 0x10200000); setBattleFlag(0x20, 0);
            }
            return;
        }
    }
    if (!isTrgAttack()) return;
    if (isBattleFlag(0x10)) return;
    if (isBattleFlag(0x100)) return;
    if (isBattleFlag(0x80)) return;
    if (isBattleFlag(0x200)) return;
    if (isBattleFlag(0x400)) return;
    if (isBattleFlag(0x800)) return;
    if (m_tqAttackTimer > 0) return;
    if (!isActionFlag(0x40)) return;
    INT mode = m_pRevolver->getMode();
    INT result = m_pRevolver->setPlayCard(0, 1);
    if (!result) return;
    setBattleFlag(0, 8);
    if (mode) return;
    if (result == 1) {
        TRACE("CPlayer::chkPlayCard(); TRG!\n");
        setBattleFlag(0x10200000, 0x20);
    } else {
        TRACE("CPlayer::chkPlayCard(); TRG...REFRECT\n");
        setBattleFlag(0, 0x10200000); setBattleFlag(0x20, 0);
    }
}
INT CPlayer::isNowCardActionState() {
    switch (m_PlayerState) {
    case PL::ATTACK:
    case PL::ITEM:
    case PL::MAGIC:
    case PL::SKILL:
    case PL::GIMMICKCARD:
    case PL::FRIEND:
    case PL::SUMMON:
    case PL::DUEL:
        return 1;
    }
    return 0;
}
__asm__(".rdata\n");
INT CPlayer::chkPlayCardCombo() {
    if (!m_pRevolver) return FALSE;
    if (m_bIsDead) return FALSE;
    if (isBattleFlag(4)) return FALSE;
    if (isConditionFlag(0x2000000)) return FALSE;
    if (isTrgFlag(0x100000)) return FALSE;
    BOOL bExeSkill = FALSE;
    INT stock = m_pRevolver->getStock(1);
    if (stock > 0) {
        if (isTrgStock() && stock == 3) bExeSkill = TRUE;
        else bExeSkill = isTrgStockSkill() != 0;
    }
    if (!bExeSkill && !isTrgAttack()) return FALSE;
    if (!isActionFlag(0x40)) return FALSE;
    if (isBattleFlag(0x4C1F90)) return FALSE;
    CARD* card = m_pRevolver->getActiveCardData();
    if (!card) return FALSE;
    if (bExeSkill && m_bIsCanCombo && !m_iNextAttack && m_bIsAttackHit && m_iAttackNo < m_iMaxCombo) {
        TRACE("CPlayer::chkPlayCardCombo(); Combo! > Stock!\n");
        INT result = m_pRevolver->setPlayStock(1, 1);
        if (result) {
            setBattleFlag(0, 8);
            if (result == 1) {
                setBattleFlag(0x80000, 0);
                setBattleFlag(0x10200000, 0);
                m_iNextAttack = 8;
            } else {
                TRACE("REFRECT\n");
                setBattleFlag(0, 0x80000);
                setBattleFlag(0, 0x10200000);
                m_iNextAttack = 0;
            }
            return TRUE;
        }
        TRACE("NG!!\n");
    }
    INT cat = card->m_nCategory1;
    if (cat && (cat < 7 || cat == 14)) {
        if (m_bIsCanCombo && !m_iNextAttack && m_bIsAttackHit && m_iAttackNo < m_iMaxCombo) {
            TRACE("CPlayer::chkPlayCardCombo(); Combo!\n");
            INT result = m_pRevolver->setPlayCard(1, 1);
            if (result) {
                setBattleFlag(0, 8);
                if (result == 1) {
                    setBattleFlag(0x80000, 0);
                    setBattleFlag(0x10200000, 0x20);
                } else {
                    TRACE("REFRECT\n");
                    setBattleFlag(0, 0x80000);
                    setBattleFlag(0, 0x10200000);
                    setBattleFlag(0x20, 0);
                }
                return TRUE;
            }
            TRACE("NG!!\n");
        }
    }
    return FALSE;
}
#define INFO (*(MES_CARD_INFO*)m_recvCardInfo)
#define CARD0 (*pCard)
void CPlayer::onMsg(INT inMsg, void* inExtraInfo) {
    CARD* pCard;
    m_recvMsg = inMsg;
    if (!inMsg) return;
    if (!inExtraInfo) {
        TRACE("CPlayer::onMsg *** warning!! *** %d, inExtraInfo=NULL\n", inMsg);
        return;
    }
    if (isBattleFlag(4) || m_bIsDead) {
        TRACE("CPlayer::onMsg *** warning!! *** %d, BATTLE_FLAG_ENEMY_EXTERMINATE\n", inMsg);
        setBattleFlag(0, 0x480F90);
        m_iNextAttack = 0;
        setBattleFlag(0, 8);
        setBattleFlag(0, 0x10080000);
        return;
    }
    pCard = INFO.m_cardData;
    setBattleFlag(0, 0x10004060);
    INFO = *(MES_CARD_INFO*)inExtraInfo;
    m_iStockSkill = INFO.m_nStock;
    m_iExtraSkill = INFO.m_nExtra;
    m_iFirstSkill = INFO.m_nFirst;
    if (getPlayerState() == 0x24) {
        switch (m_recvMsg) {
        case 1:
            break;
        case 3:
        case 4:
        case 7:
        case 13:
            setBattleFlag(0, 0x480F90);
            break;
        case 14:
            TRACE("CPlayer::onMsg = CB_MES_PRONOUNCE ---- DEATH\n");
            setBattleFlag(0x1000000, 0x480F90);
            m_iNextAttack = 0;
            setBattleFlag(0, 8);
            setBattleFlag(0x20, 0);
            break;
        case 15:
            TRACE("CPlayer::onMsg = CB_MES_PRONOUNCE ---- CLEAR\n");
            setBattleFlag(0x2000000, 0x480F90);
            break;
        }
    } else if (getPlayerState() == 0x22) {
        switch (m_recvMsg) {
        case 1:
            setConditionFlag(0, 0x4000000);
            break;
        case 3:
        case 4:
        case 7:
        case 8:
            setBattleFlag(0, 0x480F90);
            break;
        case 11:
            TRACE("CPlayer::onMsg = CB_MES_DUEL ---- TOOSLOW\n");
            setBattleFlag(0x10000, 0x480F90);
            setBattleFlag(0, 8);
            break;
        case 12:
            TRACE("CPlayer::onMsg = CB_MES_DUEL ---- SUCCESS ---- %d\n", m_iStockSkill);
            setBattleFlag(0x20000, 0);
            break;
        }
    } else if (!m_iStockSkill) {
        m_iStepStockSkill = 0;
        if (DebugWork::g_dw.player.iCardLock > 0) {
            switch (CARD0.m_nCategory21) {
            case 1:
            case 2:
            case 3:
            case 4:
            case 5:
                _setCardNK(CARD0, DebugWork::g_dw.player.iCardLock, CARD0.m_nNumber);
                break;
            case 6:
            case 14:
                _setCardNK(CARD0, DebugWork::g_dw.player.iCardLock, 0);
                break;
            case 7:
            case 8:
            case 9:
            case 10:
            case 11:
            case 12:
            case 13:
                break;
            }
        }
        switch (m_recvMsg) {
        case 1:
            m_uiCardID = CARD0.m_nKind;
            setConditionFlag(0, 0x4000000);
            if (isActionFlag(0x4000000) && !isConditionFlag(0x400000)) {
                TRACE("CPlayer::onMsg = CB_MES_START ---- PLAY CARD ---- card = id:%d, no:%d,", CARD0.m_nKind, CARD0.m_nNumber);
                if (INFO.m_nBreak) {
                    m_iDP += INFO.m_nBreak;
                    m_iDP += INFO.m_nBonus;
                    if (m_iDP > 30) m_iDP = 30;
                    TRACE("  break!!=%d, DP:%d/%d,", INFO.m_nBreak, m_iDP, 30);
                    m_pGauge->setDP(m_iDP, 30);
                    if (INFO.m_nBonus) {
                        setConditionFlag(0x4000000, 0);
                        TRACE("  speed break!!=%d, DP:%d/%d,", INFO.m_nBonus, m_iDP, 30);
                    }
                }
                TRACE("\n");
                if (m_iDP >= 30) {
                    INT cat = CARD0.m_nCategory21;
                    if (cat < 6) {
                        if (cat != 0) setPlayerFlag(0x40000, 0);
                    }
                    if (isPlayerFlag(0x40000)) return;
                }
            } else {
                TRACE("CPlayer::onMsg = CB_MES_START ---- PLAY CARD ---- card = id:%d, no:%d,\n", CARD0.m_nKind, CARD0.m_nNumber);
                if (INFO.m_nBonus) {
                    setConditionFlag(0x4000000, 0);
                    TRACE("  speed break!!\n");
                }
            }
            setBattleFlag(0, 0x480F90);
            switch (CARD0.m_nCategory21) {
            case 1: setBattleFlag(0x10, 0); break;
            case 2: setBattleFlag(0x100, 0); break;
            case 3: setBattleFlag(0x800, 0); break;
            case 4: setBattleFlag(0x80, 0); break;
            case 5: setBattleFlag(0x400, 0); break;
            case 14: setBattleFlag(0x400000, 0);
            case 6: setBattleFlag(0x200, 0); break;
            case 7:
                if (CARD0.m_nNumber != -1) {
                    m_pCB->destroyPlayCard(1, 0);
                }
                break;
            case 8:
                break;
            default:
                TRACE("CPlayer::onMsg *** error!! *** card category = ???\n");
                break;
            }
            break;
        case 2:
            TRACE("CPlayer::onMsg = CB_MES_SET ---- PLAY CARD ---- card = id:%d, no:%d,", CARD0.m_nKind, CARD0.m_nNumber);
            if (!isConditionFlag(0x400000)) {
                if (INFO.m_nBreak) TRACE("  break!!=%d,", INFO.m_nBreak);
            }
            TRACE("\n");
            break;
        case 3:
            setBattleFlag(0x40, 0);
            if (isActionFlag(0x4000000)) {
                TRACE("CPlayer::onMsg = CB_MES_BREAK ---- PLAY CARD ----  break!!=%d,", INFO.m_nBreak);
                if (INFO.m_nBreak) {
                    m_iDP -= INFO.m_nBreak;
                    if (isConditionFlag(0x400000)) {
                        if (m_iDP <= 0) {
                            setTrgFlag(0x100000, 0);
                            m_iDP = 0;
                            m_pCB->disableDuelEntry(0x400000);
                        }
                        TRACE(" DP:%d/%d, in DMODE\n", m_iDP, m_iMaxDP);
                        m_pGauge->setDP(m_iDP, m_iMaxDP);
                    } else {
                        if (m_iDP <= 0) m_iDP = 0;
                        TRACE(" DP:%d/%d,\n", m_iDP, 30);
                        m_pGauge->setDP(m_iDP, 30);
                    }
                }
                TRACE("\n");
            }
            setBattleFlag(0, 8);
            m_iNextAttack = 0;
            setBattleFlag(0, 0x480F90);
            break;
        case 8:
            TRACE("CPlayer::onMsg = CB_MES_DUEL ---- PLAY CARD ----\n");
            setBattleFlag(0x8000, 0);
            break;
        case 7:
            TRACE("CPlayer::onMsg = CB_MES_ZERO\n");
            setBattleFlag(0x4000, 8);
            m_iNextAttack = 0;
            setBattleFlag(0, 0x480F90);
            break;
        case 4:
            TRACE("CPlayer::onMsg = CB_MES_REFRECT\n");
            setBattleFlag(0x20, 8);
            m_iNextAttack = 0;
            setBattleFlag(0, 0x480F90);
            break;
        default:
            TRACE("CPlayer::onMsg = CB_MES_????:%d\n", m_recvMsg);
            return;
        }
    } else {
        switch (m_recvMsg) {
        case 1:
            setConditionFlag(0, 0x4000000);
            if (isActionFlag(0x4000000) && !isConditionFlag(0x400000)) {
                TRACE("CPlayer::onMsg = CB_MES_START ---- STOCK SKILL ----");
                if (INFO.m_nBreak) {
                    m_iDP += INFO.m_nBreak;
                    m_iDP += INFO.m_nBonus;
                    if (m_iDP > 30) m_iDP = 30;
                    TRACE("  break!!=%d, DP:%d/%d,", INFO.m_nBreak, m_iDP, 30);
                    m_pGauge->setDP(m_iDP, 30);
                    if (INFO.m_nBonus) {
                        setConditionFlag(0x4000000, 0);
                        TRACE("  speed break!!=%d, DP:%d/%d,", INFO.m_nBonus, m_iDP, 30);
                    }
                }
                TRACE("\n");
                if (m_iDP >= 30) {
                    setPlayerFlag(0x40000, 0);
                    break;
                }
            } else {
                TRACE("CPlayer::onMsg = CB_MES_START ---- STOCK SKILL ----\n");
                if (INFO.m_nBonus) {
                    setConditionFlag(0x4000000, 0);
                    TRACE("  speed break!!\n");
                }
            }
            setBattleFlag(0x40000, 0);
            m_iStepStockSkill = 99;
            break;
        case 2:
            if (isActionFlag(0x4000000) && !isConditionFlag(0x400000)) {
                if (isConditionFlag(0x200000)) {
                    TRACE("CPlayer::onMsg = CB_MES_SET ---- STOCK SKILL ----  break!!=%d, DP:%d/%d,\n", INFO.m_nBreak, m_iDP, 30);
                    return;
                }
            }
            setBattleFlag(0x1000, 0);
            if (m_pCB->getEnchantKind() == 0x2F) setBattleFlag(0x2000, 0);
            TRACE("CPlayer::onMsg = CB_MES_SET ---- STOCK SKILL ----");
            if (!isConditionFlag(0x400000)) {
                if (INFO.m_nBreak) TRACE("  break!!=%d,", INFO.m_nBreak);
            }
            TRACE("\n");
            TRACE("card[0]= id:%d, no:%d, sk:%d, ex:%d, fi:%d,\n", pCard[0].m_nKind, pCard[0].m_nNumber, INFO.m_nStock, INFO.m_nExtra, INFO.m_nFirst);
            TRACE("card[1]= id:%d, no:%d,\n", pCard[1].m_nKind, pCard[1].m_nNumber);
            TRACE("card[2]= id:%d, no:%d,\n", pCard[2].m_nKind, pCard[2].m_nNumber);
            m_iStepStockSkill = 0;
            break;
        case 3:
            setBattleFlag(0x40, 0);
            if (isActionFlag(0x4000000) && isConditionFlag(0x400000)) {
                TRACE("CPlayer::onMsg = CB_MES_BREAK ---- STOCK SKILL ----  break!!=%d\n", INFO.m_nBreak);
                if (INFO.m_nBreak) {
                    m_iDP -= INFO.m_nBreak;
                    if (isConditionFlag(0x400000)) {
                        if (m_iDP <= 0) {
                            setTrgFlag(0x100000, 0);
                            m_pCB->disableDuelEntry(0x400000);
                            m_iDP = 0;
                        }
                        TRACE(" DP:%d/%d, in DMODE\n", m_iDP, m_iMaxDP);
                        m_pGauge->setDP(m_iDP, m_iMaxDP);
                    } else {
                        if (m_iDP <= 0) m_iDP = 0;
                        TRACE(" DP:%d/%d, \n", m_iDP, 30);
                        m_pGauge->setDP(m_iDP, 30);
                    }
                }
                TRACE("\n");
            }
            setBattleFlag(0, 8);
            m_iNextAttack = 0;
            setBattleFlag(0, 0x480F90);
            break;
        case 8:
            TRACE("CPlayer::onMsg = CB_MES_DUEL ---- STOCK SKILL ----\n");
            setBattleFlag(0x8000, 0);
            break;
        case 7:
            TRACE("CPlayer::onMsg = CB_MES_ZERO\n");
            setBattleFlag(0x4000, 8);
            m_iNextAttack = 0;
            setBattleFlag(0, 0x480F90);
            break;
        case 4:
            TRACE("CPlayer::onMsg = CB_MES_REFRECT\n");
            setBattleFlag(0x20, 8);
            m_iNextAttack = 0;
            setBattleFlag(0, 0x480F90);
            break;
        default:
            TRACE("CPlayer::onMsg = CB_MES_????:%d\n", m_recvMsg);
            break;
        }
    }
}
#undef CARD0
#undef INFO
INT CPlayer::exeUseStock(INT inFlag) {
    if (isBattleFlag(4) || m_bIsDead) return FALSE;
    switch (m_iStepStockSkill) {
    case 0:
        if (m_iFirstSkill <= 0 && startStockSkillAction()) return TRUE;
        return startSingleAction();
    case 1:
        if (!inFlag) return FALSE;
        if (startStockSkillAction()) return TRUE;
        return startSingleAction();
    case 2:
        if (!inFlag) return FALSE;
        return startSingleAction();
    default:
        return FALSE;
    }
}
__asm__(".rdata\n");
INT CPlayer::startSingleAction() {
    offAttack();
    UBYTE* pc = (UBYTE*)this + m_iStepStockSkill * 4; /* m_recvCardInfo.m_cardData[step]: id byte, category in the top nibble (CARD is not in the tree yet) */
    m_uiCardID = pc[0x6D8];
    switch (*(UINT*)(pc + 0x6D8) >> 28) {
    case 1:
        initAttack(m_iAttackNo);
        break;
    case 4:
        initItem(0);
        break;
    case 2:
        initMagic(0);
        break;
    case 5:
        initFriend(0);
        break;
    case 3:
        initSummon(0);
        break;
    default:
        setPushon(0);
        m_pMarker->holdTarget(0);
        TRACE("CPlayer:: startSingleAction :holdTarget(0); \x81\xa6\x81\xa6\x81\xa6 No.2993\x91\xce\x89\x9e\x8e\xc0\x8ds \x81\xa6\x81\xa6\x81\xa6\n");
        if (isPlayerFlag(0x80000000)) setBGFade(1.0f, 20.0f, 0);
        fadeEffAttack(0);
        m_fWalkSpeed = m_fJumpSpdXZ = m_fMoveSpdXZ;
        setUseRsrcBlock(0);
        if (isBattleFlag(0x2000)) {
            TRACE("CPlayer:: startSingleAction W-STOCK !!\n");
            initIdle(isAir(), 0);
            setBattleFlag(0, 0x00480F90);
            setBattleFlag(0, 0x00041000);
            setBattleFlag(0, 0x10200000);
            setBattleFlag(0, 0x00130000);
            setBattleFlag(0, 0x03000000);
            setBattleFlag(0x41000, 0);
            setBattleFlag(0, 0x2000);
            m_iStepStockSkill = 0;
            return TRUE;
        }
        if (m_pCB) m_pCB->destroyPlayCard(1, 0);
        if (!m_iNextAttack) {
            TRACE("CPlayer:: startSingleAction END.\n");
            setBattleFlag(0, 0x00480F90);
            setBattleFlag(0, 0x00041000);
            setBattleFlag(0, 0x10200000);
            setBattleFlag(0, 0x00130000);
            setBattleFlag(0, 0x03000000);
            m_iAttackNo = 0;
            m_bIsAttackHit = FALSE;
            m_bNextTrg = FALSE;
            m_iNextAttack = 0;
            m_bIsCanCombo = FALSE;
            initIdle(isAir(), 0);
            m_uiAttackKind = 0;
            m_uiAttackID = 0;
            return TRUE;
        }
        TRACE("CPlayer:: startSingleAction *** Chain? ***\n");
        return FALSE;
    }
    m_iStepStockSkill++;
    return TRUE;
}
void CPlayer::initAttack(INT inNo) {
    setPlayerState(PL::ATTACK);
    setBattleFlag(0, 0x10);
    setBattleFlag(0, 0x80000);
    setBattleFlag(0, 8);
    setTrgFlag(0, 4);
    setPlayerFlag(0x100000, 0);
    fadeEffAttack(0);
    m_iAerialAnimShift = 0;
    m_pMarker->holdTarget(8);
    m_iAttackNo = inNo;
    if (!inNo) m_uiPreAttackKind = 0;
    m_bIsAttackHit = FALSE;
    m_bNextTrg = FALSE;
    m_iNextAttack = 0;
    if (isAir())
        m_fMoveSpdXZ = m_fJumpSpdXZ;
    else
        m_fMoveSpdXZ = m_fWalkSpeed;
    /* MATCH: the two stores through _ref() keep the g_pBattleCardList load after them (a plain member store never
       conflicts with a global scalar load); that sched1 order gives the original's v0/v1/a0 allocation. */
    _ref(m_fJumpGravity) = PL::g_fGravity * 0.16f;
    _ref(m_JumpState) = 3;
    m_tqCardBreakTime = PL::g_CardBreakTime[CardBattle::g_pBattleCardList[m_uiCardID].BREAK];
    m_iComboBoost = isConditionFlag(0x40000000) ? m_iComboBoost + 20 : 0;
    setAttack();
    m_fDistXZ = m_fDistY = 0.0f;
    if (m_pAtkData->FLAG & 8) {
        setPlayerFlag(0x2800000, 0);
    } else {
        setPlayerFlag(0, 0x2800000);
        m_fJumpGravity = PL::g_fGravity;
    }
}
void CPlayer::setAttack() {
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
    if (m_iAttackNo >= m_iMaxCombo) m_iAttackNo = 0;
    if (!(m_iAttackNo < m_iMaxCombo - 1)) {
        m_uiAttackKind = isAir() ? 7 : 4;
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
        default:
            if (isAir()) m_uiAttackKind = 6;
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
    if (CGame::isSoraGame()) m_uiAttackID = PL::g_AttackList[m_uiAttackKind];
    else if (!isConditionFlag(0x400000)) m_uiAttackID = PL::g_RikuAttackList[m_uiAttackKind];
    else m_uiAttackID = PL::g_DRikuAttackList[m_uiAttackKind];
    if (m_iAttackNo == 0) {
        if (CGame::isSoraGame()) {
            if ((UINT)((USHORT)DebugWork::g_dw.player.iAttackID - 1) < 7) m_uiAttackID = DebugWork::g_dw.player.iAttackID;
        } else {
            if ((UINT)((USHORT)DebugWork::g_dw.player.iAttackID - 0x97) < 7) m_uiAttackID = DebugWork::g_dw.player.iAttackID;
        }
    }
    m_pAtkData = &PL::g_AttackDataList[m_uiAttackID];
    switch (m_uiAttackID) {
    case 5:
    case 0x9B:
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
    case 6:
    case 7:
    case 0x9C:
    case 0x9D:
        if (!isAir()) m_fJumpSpdY = PL::g_fAntiGravity * (FLOAT)m_pAtkData->WORK1;
        procGravity(0, 1, 1);
        break;
    }
    setAnimForce(m_pAtkData->ANIM_ID, 0, 1, 0, 0, 0);
    if (isConditionFlag(0x4000000)) {
        setAnimSpeed(1.5f);
        if (!m_hEffAttack[0]) {
            m_hEffAttack[0] = attachEffect(PL::g_szDpxSpeedBreak, 1, &Crd(m_iBoneChest), NULL, NULL, 0.0f);
            effect = CEffectMan::getEffect(m_hEffAttack[0]);
            if (effect) effect->m_iFlag |= 4;
        }
        if (!m_hEffAttack[1]) {
            m_hEffAttack[1] = attachEffect(PL::g_szDpxEX1580, 0x2D, &Crd(m_iBoneChest), NULL, NULL, 0.0f);
            effect = CEffectMan::getEffect(m_hEffAttack[1]);
            if (effect) effect->m_iFlag |= 4;
        }
        if (!m_hEffAttack[2]) {
            m_hEffAttack[2] = attachEffect(PL::g_szDpxSpeedBreak, 3, &Crd(m_iBoneChest), NULL, NULL, 0.0f);
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
        _ref(m_AtkKind) = (ATK_KIND)m_pAtkData->DAMAGE;
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
void CPlayer::exeAttack() {
    TIMEQ frameTQ = getMotionCurrentTime();
    TIMEQ frame = frameTQ / 50;
    setBattleFlag(0, 8);
    if (m_iNowAnimID != 7) {
        procGravity(frame, m_pAtkData->MOVE_END_FRAME, 1);
        if (frame <= m_pAtkData->MOVE_START_FRAME) {
            procTargetMark();
            procAttackMove(frame, 1.0f, 4.0f, 2, 1.0f);
        } else {
            FLOAT rate = 0.0f;
            if (frame < 6 && (m_pAtkData->FLAG & 8)) {
                procTargetMark();
                rate = 1.0f;
            }
            procAttackMove(frame, m_pAtkData->RADIUS * 0.1f, m_pAtkData->MAX_SPEED, !m_bIsAttackHit, rate);
        }
    }
    if (!isBattleFlag(0x1000)) {
        if (isBattleFlag(0x10)) m_iNextAttack = 1;
        else if (isBattleFlag(0x100)) m_iNextAttack = 2;
        else if (isBattleFlag(0x80)) m_iNextAttack = 3;
        else if (isBattleFlag(0x400)) m_iNextAttack = 4;
        else if (isBattleFlag(0x800)) m_iNextAttack = 5;
        else if (isBattleFlag(0x200)) m_iNextAttack = 6;
        else if (isBattleFlag(0x40000)) m_iNextAttack = 7;
        if (frame > m_pAtkData->COLL_START_FRAME && frameTQ > 500) {
            if (m_iNextAttack == 1) {
                offAttack();
                initAttack(m_iAttackNo);
                return;
            } else if (m_iNextAttack == 2) {
                offAttack();
                initMagic(m_iAttackNo);
                setBattleFlag(0, 0x80000);
                fadeEffAttack(0);
                return;
            } else if (m_iNextAttack == 3) {
                offAttack();
                initItem(m_iAttackNo);
                setBattleFlag(0, 0x80000);
                fadeEffAttack(0);
                return;
            } else if (m_iNextAttack == 4) {
                offAttack();
                initFriend(m_iAttackNo);
                setBattleFlag(0, 0x80000);
                fadeEffAttack(0);
                return;
            } else if (m_iNextAttack == 5) {
                offAttack();
                initSummon(m_iAttackNo);
                setBattleFlag(0, 0x80000);
                fadeEffAttack(0);
                return;
            } else if (m_iNextAttack == 6) {
                offAttack();
                initGimmickCard(m_iAttackNo);
                setBattleFlag(0, 0x80000);
                fadeEffAttack(0);
                return;
            } else if (m_iNextAttack == 7) {
                offAttack();
                exeUseStock(0);
                fadeEffAttack(0);
                return;
            } else {
                if (chkPlayCardCombo()) return;
            }
        }
    } else {
        if ((m_iStepStockSkill == 3 || m_iStockSkill == m_iStepStockSkill) && !isBattleFlag(0x2000) && frame > m_pAtkData->COLL_START_FRAME && frameTQ > 500) {
            if (chkPlayCardCombo()) {
                setBattleFlag(0, 0x41000);
                return;
            }
        }
        if (frame >= m_pAtkData->COLL_END_FRAME) {
            if (exeUseStock(1)) return;
        }
    }
    if (m_iNowAnimID == 7) {
        if (isMotionEnd()) {
            procSkillEnd(-1);
            fadeEffAttack(0);
        }
        return;
    }
    if (procAttackCollision(frame)) {
        fadeEffAttack(0);
        return;
    }
    if (isPlayerFlag(0x800000) && !isAir() && frame > m_pAtkData->COLL_END_FRAME) {
        if (!isBattleFlag(0x80000)) {
            procSkillEnd(1);
        } else {
            setPlayerFlag(0, 0x800000);
            setAnim(7, -1, 0);
        }
        fadeEffAttack(0);
        return;
    }
    if (isMotionEnd()) {
        if (!isBattleFlag(0x80000)) {
            procSkillEnd(-1);
            fadeEffAttack(0);
        } else if (isBattleFlag(4)) {
            setBattleFlag(0, 0x80000);
            setBattleFlag(0, 0x200000);
            setBattleFlag(0, 0x10000000);
            fadeEffAttack(0);
        }
    }
}
void CPlayer::procTargetMark() {
    CEntity* pTarget = m_hTarget > 0 ? (CEntity*)CTaskMan::getTask(m_hTarget) : NULL;
    if (pTarget) {
        m_hMark = m_hTarget;
        pTarget->getLockOnPos(m_vMarkPos);
    } else {
        m_hMark = 0;
        m_vMarkPos.set(0.0f, 0.0f, 0.0f);
    }
}
void CPlayer::procAttackMove(TIMEQ inNowFrame, FLOAT inRadius, FLOAT inMaxSpeed, INT inHomingFlag, FLOAT inRate) {
    FLOAT speed = 0.0f;
    FVECTOR vec;
    FVECTOR dir;
    if (inHomingFlag && m_hMark > 0) {
        SubVectorXYZ(vec, m_vMarkPos, m_vCenterPos);
        Normalize(dir, vec);
        vec.y = 0.0f;
        m_fDistXZ = _sqrtf(InnerProduct(vec, vec));
        FLOAT d = Movement::Homing(m_fDir, atan2f(dir.x, dir.z), inRate, m_fFrameRate);
        vec.y = d; /* the original: FVECTOR::set (Coord.h:730); set() here gives other store order */
        vec.x = 0.0f;
        vec.z = 0.0f;
        vec.w = 1.0f;
        m_fDir = d;
        m_pMove->getRot() = vec;
        if (inHomingFlag != 2 && inNowFrame >= m_pAtkData->MOVE_START_FRAME && inNowFrame < m_pAtkData->MOVE_END_FRAME) {
            if (inRadius < m_fDistXZ) speed = inMaxSpeed / 60.0f;
            else if (m_pAtkData->FLAG & 4) m_hMark = 0;
        }
    }
    if ((m_pAtkData->FLAG & 0x10) && inNowFrame < m_pAtkData->MOVE_END_FRAME) {
        speed += (SBYTE)m_pAtkData->WORK0 / 60.0f;
    }
    if (isPlayerFlag(0x800000)) {
        if (speed > 0.0f) m_fMoveSpdXZ = Movement::Approach(m_fMoveSpdXZ, speed, 0.2f, m_fFrameRate);
        else m_fMoveSpdXZ = Movement::Approach(m_fMoveSpdXZ, speed, 0.1f, m_fFrameRate);
    } else if (m_pAtkData->FLAG & 4) {
        if (speed > 0.0f) m_fMoveSpdXZ = Movement::Approach(m_fMoveSpdXZ, speed, 0.2f, m_fFrameRate);
        else m_fMoveSpdXZ = Movement::Approach(m_fMoveSpdXZ, speed, 0.05f, m_fFrameRate);
    } else {
        if (speed > 0.0f) m_fMoveSpdXZ = Movement::Approach(m_fMoveSpdXZ, speed, 0.2f, m_fFrameRate);
        else m_fMoveSpdXZ = Movement::Approach(m_fMoveSpdXZ, speed, 0.4f, m_fFrameRate);
    }
    m_pMove->calcVelocityXZ(m_fMoveSpdXZ);
}
INT CPlayer::procAttackCollision(TIMEQ inNowFrame) {
    if (inNowFrame >= m_pAtkData->COLL_START_FRAME) {
        if (inNowFrame < m_pAtkData->COLL_END_FRAME) {
            if (!m_pWeapon->isOnAttack()) onAttack();
            switch (m_pWeapon->isHit()) {
            case 1:
            case 2:
                m_bIsAttackHit = TRUE;
                setPlayerFlag(0, 0x100000);
                break;
            case 3:
                if (m_pCB) {
                    m_pCB->destroyPlayCard(1, 0);
                    setBattleFlag(0, 0x10200000);
                }
                initFlip(0);
                return TRUE;
            }
        } else {
            offAttack();
            setPlayerFlag(0, 0x100000);
        }
    }
    return FALSE;
}
void CPlayer::procGravity(TIMEQ inNowFrame, TIMEQ inEndFrame, INT inFlag) {
    FVECTOR vel = m_pMove->getVel();
    m_fJumpSpdY += m_fJumpGravity * m_fFrameRate;
    if (inFlag && isPlayerFlag(0x2000000)) m_fJumpGravity = PL::g_fGravity * 0.16f;
    else m_fJumpGravity = PL::g_fGravity;
    if (isPlayerFlag(0x800000) && inNowFrame < inEndFrame) {
        BOOL bFloat = TRUE;
        if (m_hMark > 0) {
            FVECTOR vec;
            SubVectorXYZ(vec, m_vMarkPos, m_vCenterPos);
            m_fDistY = _abs(vec.y);
            if (vec.y < 0.0f) {
                if (m_fJumpSpdY > 0.0f) {
                    if (m_fDistY < 0.5f) m_fJumpSpdY *= PL::g_fJumpStopRate;
                    else m_fJumpSpdY = 0.0f;
                }
                bFloat = FALSE;
            }
        }
        if (bFloat && m_fJumpSpdY < 0.0f) m_fJumpSpdY = 0.0f;
    }
    if (!(inNowFrame < inEndFrame) && m_fJumpSpdY > 0.0f) m_fJumpSpdY = 0.0f;
    vel.y = m_fJumpSpdY;
    m_pMove->setVel(vel);
}
__asm__(".rdata\n");
void CPlayer::procSkillEnd(INT inAir) {
    INT bAir = inAir;
    if (bAir < 0) bAir = isAir();
    offAttack();
    setPushon(0);
    m_pMarker->holdTarget(0);
    if (isPlayerFlag(0x80000000)) setBGFade(1.0f, 20.0f, 0);
    fadeEffAttack(0);
    setBattleFlag(0, 0x00480F90);
    setBattleFlag(0, 0x00041000);
    setBattleFlag(0, 0x10200000);
    setBattleFlag(0, 0x00130000);
    setBattleFlag(0, 0x03000000);
    m_fWalkSpeed = m_fJumpSpdXZ = m_fMoveSpdXZ;
    if (m_PlayerState == 0x22) setPlayerFlag(0, 1);
    setUseRsrcBlock(0);
    if (isBattleFlag(4)) {
        initEnd();
    } else if (chkExtraSkill()) {
    } else if (isBattleFlag(0x2000)) {
        initIdle(bAir, 0);
        setBattleFlag(0x41000, 0x2000);
        m_iStepStockSkill = 0;
        TRACE("CPlayer::procSkillEnd  W-STOCK !!\n");
    } else {
        if (m_pCB) {
            m_pCB->destroyPlayCard(1, 0);
            setBattleFlag(0, 0x10200000);
        }
        initIdle(bAir, 0);
        m_uiAttackKind = 0;
        m_uiAttackID = 0;
        if (!m_iNextAttack) m_iAttackNo = 0;
        m_bIsAttackHit = FALSE;
        m_bNextTrg = FALSE;
        m_iNextAttack = 0;
        m_bIsCanCombo = FALSE;
    }
}
BOOL CPlayer::chkExtraSkill() {
    if (m_iExtraSkill) {
        m_iStepStockSkill = 2;
        m_iExtraSkill = 0;
        if (exeUseStock(1)) return TRUE;
    }
    return FALSE;
}
void CPlayer::calcEnchant_AttackPoint(INT& inPoint, ATK_KIND& inAtkKind, ATK_ATTR inAttr, UINT inUpKind) {
    if (m_pCB) {
        CardBattle::ENCHANT_PARAM param;
        param.init(getHandle(), 1);
        param.uiState = 1;
        param.AtkKind = inAtkKind;
        param.UpKind = inUpKind;
        param.AtkAttr = inAttr;
        param.iAttackPoint = inPoint;
        param.iHp = m_iHP;
        param.iHpMax = m_iMaxHP;
        switch (m_uiAttackKind) {
        case 1:
        case 2:
        case 3:
        case 5:
        case 6:
            param.iNormal = 1;
            break;
        case 4:
        case 7:
            param.iNormal = 1;
            param.iFinish = 1;
            break;
        }
        m_pCB->calcEnchantParam(param);
        inPoint = param.iAttackPoint;
        inAtkKind = param.AtkKind;
    }
}
void CPlayer::calcEnchant_DamagePoint(INT& inPoint, ATK_ATTR inAttr) {
    if (m_pCB) {
        CardBattle::ENCHANT_PARAM param;
        param.init(getHandle(), 1);
        param.uiState = 2;
        param.AtkAttr = inAttr;
        param.iAttackPoint = inPoint;
        m_pCB->calcEnchantParam(param);
        inPoint = param.iAttackPoint;
        if (param.iGuard > 0) Gauge::CCommonGauge::setDispSeq(getHandle(), 2);
    }
}
void CPlayer::setUseRsrcBlock(INT inFlag) {
    if (inFlag) {
        setPlayerFlag(0x10000000, 0);
        m_pPLM->setUseRsrcBlockSkill(m_iStockSkill, 1);
        switch (m_iStockSkill) {
        case 0x66:
            m_pPLM->setUseRsrcBlockSkill(0x93, 1);
            break;
        case 0x67:
            m_pPLM->setUseRsrcBlockSkill(0x94, 1);
            break;
        case 0x68:
            m_pPLM->setUseRsrcBlockSkill(0x95, 1);
            break;
        case 0x69:
            m_pPLM->setUseRsrcBlockSkill(0x93, 1);
            break;
        case 0x6A:
            m_pPLM->setUseRsrcBlockSkill(0x94, 1);
            m_pPLM->setUseRsrcBlockSkill(0x92, 1);
            break;
        case 0x6B:
            m_pPLM->setUseRsrcBlockSkill(0x95, 1);
            m_pPLM->setUseRsrcBlockSkill(0x92, 1);
            break;
        }
    } else if (isPlayerFlag(0x10000000)) {
        m_pPLM->setUseRsrcBlockSkill(m_iStockSkill, 0);
        switch (m_iStockSkill) {
        case 0x66:
            m_pPLM->setUseRsrcBlockSkill(0x93, 0);
            break;
        case 0x67:
            m_pPLM->setUseRsrcBlockSkill(0x94, 0);
            break;
        case 0x68:
            m_pPLM->setUseRsrcBlockSkill(0x95, 0);
            break;
        case 0x69:
            m_pPLM->setUseRsrcBlockSkill(0x93, 0);
            break;
        case 0x6A:
            m_pPLM->setUseRsrcBlockSkill(0x94, 0);
            m_pPLM->setUseRsrcBlockSkill(0x92, 0);
            break;
        case 0x6B:
            m_pPLM->setUseRsrcBlockSkill(0x95, 0);
            m_pPLM->setUseRsrcBlockSkill(0x92, 0);
            break;
        }
        setPlayerFlag(0, 0x10000000);
    }
}
void CPlayer::setDefAttr(FLOAT* inAttr) {
    for (INT i = 0; i < 6; i++) {
        m_fDefAttr[i] = inAttr[i];
    }
}
FLOAT CPlayer::getDefAttr(ATK_ATTR inAttr) const {
    if ((UINT)inAttr < 6) return m_fDefAttr[inAttr];
    return m_fDefAttr[5];
}
BOOL CPlayer::chkDamage() {
    INT face;
    FVECTOR dir;
    FVECTOR dpos;
    FLOAT dir0;
    BOOL bIsBackHit;
    UINT old_condition;
    INT dmg0;
    ATK_KIND kind;
    INT def_attr;
    INT anim;
    if (m_bIsDead) {
        setBattleFlag(0, 8);
        return FALSE;
    }
    if (isBattleFlag(0x20)) {
        if (isNowCardActionState()) {
            resetAction(1);
            initIdle(-1, 0);
            setBattleFlag(0, 8);
        }
        setBattleFlag(0, 0x10200000);
    }
    if (isBattleFlag(0x8000)) {
        resetAction(4);
        initDuel(0);
    } else if (isBattleFlag(0x4000)) {
        if (isConditionFlag(0x8810000)) {
            setBattleFlag(0, 0x4000);
            setBattleFlag(0, 0x10200000);
        } else {
            resetAction(3);
            initIdle(-1, 0);
            setBattleFlag(0x20, 0x4000);
            setBattleFlag(0, 0x10200000);
        }
        setBattleFlag(0, 8);
    } else if (isBattleFlag(0x40)) {
        initFlip(1);
    }
    if (!isActionFlag(0x200)) return FALSE;
    if (isMuteki()) {
        m_collBody.setDamage(0);
        return FALSE;
    }
    if (m_collBody.isGuardTrg()) {
        Gauge::CCommonGauge::setDispSeq(getHandle(), 2);
        return FALSE;
    }
    if (!m_collBody.isHit()) return FALSE;
    TRACE("CPlayer:: (initDamage)\n");
    face = 1;
    setBattleFlag(0, 8);
    m_bDmgInAir = isAir();
    dir = *m_collBody.getDetectPos();
    m_vEnemyPos = m_collBody.getEnemyPos();
    setConditionFlag(0, 0x20000);
    if (m_collBody.getEnemyHandle() > 0) {
        CEntity* pEntity = (CEntity*)CTaskMan::getTask(m_collBody.getEnemyHandle());
        if (pEntity && PLM::isEntityTypeID(pEntity->getTypeID())) {
            if (pEntity->isHeartless()) setConditionFlag(0x20000, 0);
            pEntity->getLockOnPos(dir);
        }
    }
    SubVectorXYZ(dpos, dir, getPos());
    dir0 = atan2f(dpos.x, dpos.z);
    bIsBackHit = _abs(_limitPI(dir0 - m_fDir)) < 1.5707964f ? FALSE : TRUE;
    {
        register UINT uiFlag asm("$3") = m_uiConditionFlag; /* MATCH: isConditionFlag with the word in v1, the mask in v0 */
        old_condition = uiFlag & 0x881803F;
    }
    dmg0 = m_collBody.getDamage();
    kind = m_collBody.getDamageKind();
    def_attr = m_collBody.getDamageAttr();
    calcEnchant_DamagePoint(dmg0, (ATK_ATTR)def_attr);
    def_attr = (INT)getDefAttr((ATK_ATTR)def_attr);
    if (def_attr >= 1000) {
        def_attr = 100;
        face = 3;
        setConditionFlag(1, 0);
    } else if (def_attr == 0) {
        Gauge::CCommonGauge::setDispSeq(getHandle(), 5);
        return FALSE;
    }
    m_collBody.setHitEffect();
    if (old_condition & 1) setConditionFlag(0, 1);
    dmg0 = dmg0 * def_attr / 100;
    INT hp0 = getHP();
    if (kind == 0x22) dmg0 = hp0;
    if (kind != 8) {
        hp0 -= dmg0;
        if (m_pCB && m_pCB->getEnchantKind() == 0x3B) {
            hp0 += dmg0;
        } else {
            hp0 += (DebugWork::g_dw.player.iDamage0 > 0) * dmg0;
            setDamageNumber(m_vHeadPos, dmg0);
        }
    }
    if (m_pCB) {
        if (hp0 <= 0 && getHP() >= 2 && m_pCB->getEnchantKind() == 0x29) {
            hp0 = 1;
            CardBattle::CCardBattle::decEnchantTime(1, 0x28, 0x29);
        }
    }
    if (hp0 < 0) hp0 = 0;
    m_iHP = hp0;
    TRACE("-------- Damege Info ------------------\n");
    if (CGame::isSoraGame()) {
        TRACE("Sora : HP = %d/%d\n", m_iHP, m_iMaxHP);
    } else if (isActionFlag(0x4000000)) {
        if (!isConditionFlag(0x400000)) {
            m_iDP++;
            if (m_iDP > 30) m_iDP = 30;
            TRACE("Riku : HP = %d/%d, DP = %d/%d,\n", m_iHP, m_iMaxHP, m_iDP, 30);
            m_pGauge->setDP(m_iDP, 30);
        } else {
            m_iDP -= 5;
            if (m_iDP <= 0) {
                setTrgFlag(0x100000, 0);
                m_iDP = 0;
                m_pCB->disableDuelEntry(0x400000);
                setBattleFlag(0, 8);
            }
            TRACE("Riku : HP = %d/%d, DP = %d/%d,\n", m_iHP, m_iMaxHP, m_iDP, m_iMaxDP);
            m_pGauge->setDP(m_iDP, m_iMaxDP);
        }
    } else {
        TRACE("Riku : HP = %d/%d\n", m_iHP, m_iMaxHP);
    }
    TRACE("Flag PL  : 0x%08x\n", m_uiPlayerFlag);
    TRACE("Flag CON : 0x%08x\n", m_uiConditionFlag);
    TRACE("Flag TRG : 0x%08x\n", m_uiTrgFlag);
    TRACE("Flag BTL : 0x%08x\n", m_uiBattleFlag);
    TRACE("---------------------------------------\n");
    if (hp0 <= 0) {
        TRACE("CPlayer::chkDamage(); ++++ DEAD ++++\n");
        m_bIsDead = TRUE;
        m_pPLM->notifyDeadSE(CGame::isSoraGame() ? CSound::playSE(0x4B, -1, 0x40) : CSound::playSE(0x4C, -1, 0x40));
        CSceneInfo::cancelBattleRequest();
        face = 2;
        CGameWork::setFlag(1, 6, TRUE);
        setPlayerFlag(0x1001, 0);
    }
    if (m_pGauge) m_pGauge->setDamage(m_iHP, kind);
    if (m_pCB) {
        if (m_pCB->getEnchantKind() == 0x2C) setConditionFlag(0x4000, 0);
        else setConditionFlag(0, 0x4000);
        CardBattle::CCardBattle::decEnchantTime(1, 0x27, 0);
    }
    if (isConditionFlag(0x1000)) setQuickBarrier(1);
    if (kind == 0x1A && !m_bIsDead && getPlayerState() != 0xD) return FALSE;
    if ((old_condition & 0x8010002) && !m_bIsDead && getPlayerState() != 0xD) return FALSE;
    if (m_pCB) {
        if (!m_bIsDead && !isBattleFlag(0x20)) {
            if (isBattleFlag(0x10000000)) {
                TRACE("CPlayer::chkDamage(); NG = Now Card Play TRG!!\n");
                return FALSE;
            }
            if (CardBattle::CCardBattle::isStandingCard(1)) {
                TRACE("CPlayer::chkDamage(); NG = Now Card Play!\n");
                return FALSE;
            }
        }
        if (isBattleFlag(0x20)) {
            if (isBattleFlag(0x10000000)) TRACE("CPlayer::chkDamage(); [REFRECT]  x(Now Card Play TRG)\n");
            if (CardBattle::CCardBattle::isStandingCard(1)) TRACE("CPlayer::chkDamage(); [REFRECT]  x(Now Card Play)\n");
        }
    }
    if (!m_bIsDead && isBattleFlag(4)) {
        initEnd();
        return FALSE;
    }
    resetAction(6);
    if (old_condition & 0x20) {
        setConditionFlag(0, 0x20);
        fadeEffect(m_hEffBad, 1);
        m_hEffBad = setEffect(PL::g_szDpxVexen, 4, &getPos(), NULL, 0.0f);
    }
    if (old_condition & 0x800000) {
        setConditionFlag(0, 0x800000);
        m_pMove->setFlag(0, 1);
        setOldPos(m_vCenterPos);
    }
    if (getPlayerState() == 0x1E) {
        CTask* pTask = CTaskMan::getTask(m_hGimmick);
        if (pTask && pTask->getTypeID() == 0x2003) {
            ((CGimmickCarry*)pTask)->CancelCarry();
            m_hGimmick = 0;
        }
    }
    setFaceAnim(face, 1);
    setPlayerState((PL::PlayerState)0xC);
    m_bNextTrg = FALSE;
    m_iNextAttack = 0;
    m_bDmgMuteki = FALSE;
    if (m_bIsDead) kind = (ATK_KIND)3;
    switch (kind) {
    case 39:
        setConfuse(1, 0);
    case 24:
        setSubState(6);
        m_fJumpSpdXZ = 0.0f;
        m_bDmgInAir = FALSE;
        anim = 0x29;
        procDropMP();
        bIsBackHit = FALSE;
        PLM::setVibration(2);
        break;
    case 35:
        if (m_pRevolver) m_pRevolver->setDiscard();
    case 34:
        fadeEffect(m_hEffBad, 1);
        setSubState(5);
        m_tqCardBreakTime = PL::g_CardBreakTime[5];
        anim = 0x22;
        m_fJumpSpdXZ = -0.13333334f;
        PLM::setVibration(3);
        break;
    case 31:
        setConditionFlag(1, 0);
        setFaceAnim(3, 0);
    case 3:
    case 30:
        setSubState(5);
        m_tqCardBreakTime = PL::g_CardBreakTime[5];
        anim = 0x22;
        m_fJumpSpdXZ = -0.13333334f;
        PLM::setVibration(2);
        break;
    case 2:
    case 29:
        setSubState(5);
        m_tqCardBreakTime = PL::g_CardBreakTime[4];
        anim = 0x22;
        m_fJumpSpdXZ = -0.13333334f;
        PLM::setVibration(2);
        break;
    case 6: {
        SubVectorXYZ(dpos, getPos(), m_vEnemyPos);
        dir0 = atan2f(dpos.x, dpos.z);
        setSubState(7);
        m_tqCardBreakTime = PL::g_CardBreakTime[5];
        setAnimForce(0x22, 0, 0, 0, m_bIsDead, 0);
        m_bDmgInAir = TRUE;
        m_JumpState = 3;
        m_fJumpGravity = *(FLOAT*)&PL::g_fGravity;
        m_fJumpSpdY = 0.0f;
        m_fDir = dir0;
        FVECTOR vec;
        FVECTOR vel = { 0.0f, dir0, 0.0f, 0.0f };
        vec = vel;
        m_pMove->setRot(vec);
        vec.x = dpos.x;
        vec.y = 0.0f;
        vec.z = dpos.z;
        FLOAT power = InnerProduct(vec, vec);
        m_fJumpSpdXZ = -0.26666668f;
        m_pMove->calcVelocityXZ(m_fJumpSpdXZ);
        power = dpos.y * 10.0f + 14.0f;
        if (power > 40.0f) power = 40.0f;
        else if (power < 0.0f) power = 0.0f;
        m_fJumpSpdY = PL::g_fAntiGravity * power;
        vel = getVel();
        vel.y = m_fJumpSpdY;
        m_pMove->setVel(vel);
        PLM::setVibration(2);
        if (m_bIsDead) initDead();
        return TRUE;
    }
    case 40:
        procDropMP();
    case 10:
        setConfuse(1, 0);
    case 1:
    case 28:
        setSubState(3);
        m_tqCardBreakTime = PL::g_CardBreakTime[3];
        anim = 0x1F;
        m_fJumpSpdXZ = -0.06666667f;
        PLM::setVibration(1);
        break;
    case 7:
        setConditionFlag(1, 0);
        setFaceAnim(3, 0);
        setSubState(3);
        m_tqCardBreakTime = PL::g_CardBreakTime[3];
        anim = 0x1F;
        m_fJumpSpdXZ = -0.06666667f;
        PLM::setVibration(1);
        break;
    case 12:
        setConditionFlag(0x20, 0);
        initBadCondition();
        PLM::setVibration(2);
        return TRUE;
    case 8:
        setConditionFlag(2, 0);
        setTimer(dmg0);
        initBadCondition();
        PLM::setVibration(2);
        return TRUE;
    case 27:
        setConditionFlag(0x10000, 0);
        initBadCondition();
        PLM::setVibration(2);
        return TRUE;
    case 17:
        setConditionFlag(0x800000, 0);
        initBadCondition();
        PLM::setVibration(2);
        return TRUE;
    case 32:
        setConditionFlag(0x8000000, 0);
        initBadCondition();
        PLM::setVibration(2);
        return TRUE;
    case 33:
        PLM::setVibration(2);
        if (m_bIsDead) {
            initDead();
            return TRUE;
        }
        m_fDir = Movement::Homing(m_fDir, dir0, 1.0f, m_fFrameRate);
        __asm__ volatile(""); /* MATCH: the m_fDir store, then a1, then the vptr load */
        {
            register INT nPronounce asm("$5") = 1;
            __asm__ volatile("" : : "r"(nPronounce));
            initPronounce(nPronounce);
        }
        return TRUE;
    default:
        TRACE("PL::chkDamage(); kind=%d, \x81\xa6\x96\xa2\x8d\xec\x90\xac\x81i\x82\xc6\x82\xe8\x82\xa0\x82\xa6\x82\xb8\x8f\xac\x83_\x83\x81\x81[\x83W\x88\xb5\x82\xa2\x81j\n", kind);
        anim = 0x1F;
        m_fJumpSpdXZ = -0.06666667f;
        PLM::setVibration(1);
        break;
    }
    if (m_bDmgInAir) {
        setSubState(4);
        m_tqCardBreakTime = PL::g_CardBreakTime[3];
        anim = 0x21;
        m_JumpState = 3;
        bIsBackHit = FALSE;
    }
    m_fJumpGravity = PL::g_fGravity;
    m_fJumpSpdY = 0.0f;
    if (!bIsBackHit) {
        FVECTOR vec;
        FLOAT d = Movement::Homing(m_fDir, dir0, 0.5f, m_fFrameRate);
        FVECTOR vel = { 0.0f, d, 0.0f, 0.0f };
        m_fDir = d;
        vec = vel;
        m_pMove->setRot(vec);
    } else {
        anim++;
        m_fJumpSpdXZ = -m_fJumpSpdXZ;
    }
    setAnimForce(anim, 0, 1, 0, m_bIsDead, 0);
    m_pMove->calcVelocityXZ(m_fJumpSpdXZ);
    if (m_bIsDead) initDead();
    if (old_condition & 0x8000) {
        setConditionFlag(0, 0x8000);
        m_tqCardBreakTime = PL::g_CardBreakTime[7];
    }
    return TRUE;
}
__asm__(".rdata\n");
void CPlayer::initFlip(INT inBreak) {
    FLOAT speed;
    INT animID;
    setBattleFlag(0, 8);
    setPlayerState(FLICKS);
    if (inBreak) {
        TRACE("CPlayer:: initFlip(BREAK!!)\n");
        speed = -8.0f;
        m_iComboBoost = 0;
        m_iSubState = 2;
    } else {
        TRACE("CPlayer:: initFlip()\n");
        speed = -4.0f;
        m_iSubState = 1;
        m_tqCardBreakTime = PL::g_CardBreakTime[5];
    }
    setBattleFlag(0, 0x40);
    PLM::setVibration(3);
    resetAction(5);
    m_fWalkSpeed = 0.0f;
    m_bDmgInAir = isAir();
    if (m_bDmgInAir) {
        if (getAirHeight(NULL, 10.0f, 1) < 0.7f) {
            setLand(1.0f, 0.5f, 1);
            animID = inBreak ? 0x25 : 0x24;
            m_bDmgInAir = FALSE;
        } else {
            animID = 0x26;
        }
    } else {
        animID = inBreak ? 0x25 : 0x24;
    }
    switch (m_uiAttackID) {
    case 0xD:
    case 0x30:
    case 0x31:
    case 0x32:
    case 0x33:
    case 0x34:
        setAnimForce(0x42, 0, 0, 0, 0, 0);
        speed = 0.0f;
        break;
    default:
        setAnimForce(animID, 0, 1, 0, 0, 0);
        break;
    }
    m_fJumpSpdY = 0.0f;
    m_fJumpSpdXZ = speed / 60.0f;
    m_fJumpGravity = *(FLOAT*)&PL::g_fGravity;
    m_pMove->calcVelocityXZ(m_fJumpSpdXZ);
    m_JumpState = 3;
}
void CPlayer::setFlipForce(const FVECTOR* inPos) {
    FVECTOR pos;
    if (inPos) {
        pos = *inPos;
    } else {
        pos = m_vStartPos;
        pos.y = getPos().y + 0.0035f;
    }
    warp(pos);
    updateCoord();
    coordFix();
    clearSpeed();
    initFlip(0);
    CardBattle::CCardBattle::notifyCompleteGimmick();
    PLM::setVibration(3);
}
void CPlayer::exeDamage() {
    BOOL bEndFlag;
    BOOL bActionFlag;
    setBattleFlag(0, 8);
    setGroundIK(1);
    if (m_iSubState != 7 || calcDistanceXZ(m_vEnemyPos) < 2.0f) {
        m_fJumpSpdXZ = Movement::Approach(m_fJumpSpdXZ, 0.0f, 0.1f, m_fFrameRate);
    }
    m_pMove->calcVelocityXZ(m_fJumpSpdXZ);
    if (isAir()) {
        m_JumpState = 3;
        FVECTOR vel = m_pMove->getVel();
        m_fJumpSpdY += m_fJumpGravity * m_fFrameRate;
        vel.y = m_fJumpSpdY;
        m_pMove->setVel(vel);
    }
    bEndFlag = FALSE;
    bActionFlag = FALSE;
    switch (m_iSubState) {
    case 1:
        setGroundIK(1);
        if (!isAir()) {
            if (m_iNowAnimID == 6 || m_iNowAnimID == 0x26) {
                setAnimForce(7, -1, 0, 0, 0, 0);
                m_JumpState = 6;
                m_fJumpSpdXZ = 0.0f;
                m_fJumpSpdY = 0.0f;
                m_fJumpGravity = 0.0f;
                clearSpeed();
            }
            m_bDmgInAir = FALSE;
            m_iPreAnimID = 0x24;
        }
        if (isMotionEnd()) {
            setAnimForce(isAir() ? 6 : 0x2A, -1, 0, 0, 0, 0);
        }
        bEndFlag = !(m_tqStateTimer < m_tqCardBreakTime);
        break;
    case 2:
        setGroundIK(1);
        if (!isAir()) {
            switch (m_iNowAnimID) {
            case 0x26:
                setAnimForce(0x25, -1, 0, 0, 0, getMotionCurrentTime() + m_tqFrameRate);
                m_pAnim->setRootMove(1, 1);
                m_JumpState = 6;
                m_fJumpSpdXZ = 0.0f;
                m_fJumpSpdY = 0.0f;
                m_fJumpGravity = 0.0f;
                clearSpeed();
                break;
            case 6:
                setAnimForce(7, -1, 0, 0, 0, 0);
                m_JumpState = 6;
                m_fJumpSpdXZ = 0.0f;
                m_fJumpSpdY = 0.0f;
                m_fJumpGravity = 0.0f;
                clearSpeed();
                break;
            }
            m_bDmgInAir = FALSE;
            m_iPreAnimID = 0x25;
        }
        if (isMotionEnd()) {
            setAnimForce(isAir() ? 6 : 0x2A, -1, 0, 0, 0, 0);
        }
        bEndFlag = !(m_tqStateTimer < m_tqCardBreakTime);
        break;
    case 3:
    case 5:
    case 7:
        if (isMotionEnd()) {
            if (isAir()) setAnimForce(6, -1, 0, 0, 0, 0);
            else setAnimForce(chkBattleIdle() ? 0x2A : 1, -1, 0, 0, 0, 0);
        }
        if (!(m_tqStateTimer < m_tqCardBreakTime)) {
            bEndFlag = TRUE;
            bActionFlag = !m_bIsDead;
        }
        break;
    case 4:
        if (!isAir()) {
            if (m_iNowAnimID == 6 || m_iNowAnimID == 0x21) {
                setAnimForce(7, -1, 0, 0, 0, 0);
            }
            m_bDmgInAir = FALSE;
        }
        if (isMotionEnd()) {
            if (isAir()) setAnimForce(6, -1, 0, 0, 0, 0);
            else setAnimForce(chkBattleIdle() ? 0x2A : 1, -1, 0, 0, 0, 0);
        }
        if (!(m_tqStateTimer < m_tqCardBreakTime)) {
            bEndFlag = TRUE;
            bActionFlag = !m_bIsDead;
        }
        break;
    case 6:
    default:
        bEndFlag = isMotionEnd() != 0;
        break;
    }
    if (isTrgFlag(0x40) && !isTrgFlag(0x100000) && !isConditionFlag(0x2000000)) {
        TRACE("PL::exeDamage(); \x81\xa6\x83_\x83\x81\x81[\x83W\x92\x86\x82\xbe\x82\xaf\x82\xc7\x83M\x83~\x83" "b\x83N\x94\xad\x93\xae\x81I\n");
        initGimmickCard(0);
        return;
    }
    if (bEndFlag) {
        if (m_bIsDead) {
            initDead();
            return;
        }
        if (isTrgFlag(0x100000) || isConditionFlag(0x2000000)) {
            TRACE("CPlayer:: exeDamage -> initTransform(0)\n");
            initTransform(0);
            return;
        }
        if (m_pCB) {
            if (CardBattle::CCardBattle::isStandingCard(1)) {
                TRACE("PL::exeDamage(); \x81\xa6\x83_\x83\x81\x81[\x83W\x92\x86\x82\xc8\x82\xcc\x82\xc9\x83K\x81[\x83h\x82\xf0\x8eg\x97p\x92\x86\x82\xc5\x82\xb7\x81I\n");
                m_pCB->destroyPlayCard(1, 0);
                setBattleFlag(0, 0x10200000);
            }
        }
        if (isConditionFlag(0x08810023)) {
            initBadCondition();
            return;
        }
        m_bDmgMuteki = FALSE;
        initIdle(-1, 0);
        setBattleFlag(0, 0x80000);
        setBattleFlag(0, 0x10200000);
        setBattleFlag(0, 0x4060);
        setBattleFlag(0, 0x00130000);
        setBattleFlag(0, 0x03000000);
    } else {
        if (isConditionFlag(0x4000)) bActionFlag = TRUE;
        if (bActionFlag) {
            if (isConditionFlag(0x08810023)) {
                initBadCondition();
                return;
            }
            if (chkTrgAction(1, 0)) {
                TRACE("PL::exeDamage(); chkTrgAction=OK!\n");
            }
        }
    }
}
INT CPlayer::chkTrgAction(INT inAction, UINT inFlag) {
    if (m_bIsBattleScene) {
        setBattleFlag(8, 0);
        if (isTrgFlag(0x10)) {
            if (!chkChargeMove(0) && inAction) {
                initCharge();
                return TRUE;
            }
        }
        if (isTrgFlag(8)) {
            initItem(0);
            return TRUE;
        }
        if (isTrgFlag(0x40)) {
            initGimmickCard(0);
            return TRUE;
        }
        if (isTrgFlag(0x20)) initMagic(0);
        else if (isTrgFlag(0x80)) initFriend(0);
        else if (isTrgFlag(0x100)) initSummon(0);
        else if (isTrgFlag(4)) initAttack(0);
        else goto tail;
        return TRUE;
    } else {
        if (isTrgFlag(0x200) && !isAir()) {
            switch (chkGimmick(0)) {
            case 0x11:
                initTreasureBox();
                return TRUE;
            case 0x14:
                initRoomCreation();
                return TRUE;
            case 0x15:
                initWorldSelect();
                return TRUE;
            case 0x1E:
                initCarry();
                return TRUE;
            case 0x1F:
                goto stand;
            }
        }
        if (isTrgFlag(4)) {
            initAttackField(0);
            return TRUE;
        }
    }
tail:
    if (!inAction) return FALSE;
    if (isAir()) return FALSE;
    if (m_fAnalogPower >= 0.3f) {
        if (isTrgFlag(2)) {
            initAvoid();
            return TRUE;
        }
        m_bDmgMuteki = FALSE;
    stand:
        initStandUp(0);
        return TRUE;
    }
    if (!isTrgFlag(1)) return FALSE;
    initJump(0);
    return TRUE;
}
void CPlayer::initDead() {
    FVECTOR vScale;
    setPlayerState(DEAD);
    setTrgFlag(0, 0x10000);
    m_bDmgMuteki = TRUE;
    setPlayerFlag(0x1001, 0);
    setBattleFlag(0, 8);
    m_iSubState = 0;
    if (m_pMarker) {
        m_pMarker->cancelLockon(m_hLockon, 0);
        m_pMarker->off();
    }
    m_tqEffCharge = 0;
    fadeEffect(m_hEffCharge, 0);
    fadeEffect(m_hEffCast, 0);
    setConfuse(0, 0);
    if (isConditionFlag(0x20)) {
        setConditionFlag(0, 0x20);
        fadeEffect(m_hEffBad, 0);
    }
    setConditionFlag(0, 0x0881803F);
    m_pMove->setFlag(0, 1);
    setOldPos(m_vCenterPos);
    if (isConditionFlag(0x1000000)) {
        setEntityFlag(0, 1);
        m_fScale = 1.0f;
        vScale = getScale();
        vScale.x = m_fScale;
        vScale.y = 2.0f - m_fScale;
        vScale.z = m_fScale;
        setScale(vScale);
        onWeapon(1);
        setConditionFlag(0, 0x1000000);
    }
    if (m_PrePlayerState == 0x22) {
        setPushon(0);
        if (m_pCB) m_pCB->setDuelEntry(0);
        setPlayerFlag(0, 0x10000);
    } else {
        m_pPLM->notifyPlayerDead(1);
        if (isBattleFlag(2)) {
            setBattleFlag(0, 2);
            m_tqEscape = 0;
            if (m_pEscape) m_pEscape->destroy();
            setBGFade(1.0f, 0.0f, 0);
        }
        CardBattle::CCardBattle* pCB = (CardBattle::CCardBattle*)CTaskMan::getSysTask(0x16);
        if (pCB) {
            if (pCB->getEnchantKind() != 0x2E) {
                setPlayerFlag(0, 0x020C2000);
                CardBattle::CCardBattle::setBattleEnd();
                setBattleFlag(0, 8);
                setBattleFlag(0, 0x00443F90);
                setBattleFlag(0, 0x10200000);
                setBattleFlag(0, 0x4060);
                setBattleFlag(0, 0x00130000);
                setBattleFlag(0, 0x03000000);
                m_bDmgMuteki = TRUE;
                setPlayerFlag(1, 0);
                m_iNextAttack = 0;
                m_pRevolver = NULL;
            }
        }
    }
}
void CPlayer::exeDead() {
    FLOAT f;
    FLOAT t;
    setBattleFlag(0, 8);
    switch (m_iSubState) {
    case 0:
        f = m_fJumpSpdXZ;
        if (f != 0.0f) {
            t = m_fFrameRate * 0.1f;
            m_fJumpSpdXZ = f + -f * t;
        }
        m_pMove->calcVelocityXZ(m_fJumpSpdXZ);
        if (m_tqStateTimer < 501) m_fJumpGravity = PL::g_fGravity * 0.16f;
        else m_fJumpGravity = PL::g_fGravity;
        if (isAir()) {
            FVECTOR vel = m_pMove->getVel();
            m_fJumpSpdY += m_fJumpGravity * m_fFrameRate;
            vel.y = m_fJumpSpdY;
            m_pMove->setVel(vel);
        }
        break;
    case 1: {
        FVECTOR pos;
        FVECTOR rot;
        setPlayerFlag(1, 0);
        clearSpeed();
        pos.init();
        warp(pos);
        rot = getRot();
        rot.y = -1.5707964f;
        setRot(rot);
        m_fDir = rot.y;
        break;
    }
    case 2:
        break;
    case 3:
        setPlayerFlag(0x1001, 0);
        m_iSubState = 4;
        addHP(m_iMaxHP / 4, 0x1770, 1);
        m_pCam->setFlag(0, 1);
        if (m_pCB) CardBattle::CCardBattle::decEnchantTime(1, 0x28, 0x2E);
        CSound::playSE(0x5C, -1, 0x40);
        setAnimForce(0x28, -1, 0, 0, 0, 0);
        clearSpeed();
        break;
    case 4:
        if (isMotionEnd()) {
            CSceneEnv::setStop((ObjType::eObjBit)0);
            m_iSubState = 2;
            m_bIsDead = FALSE;
            m_pMarker->on();
            initStandUp(0);
            m_bDmgMuteki = FALSE;
            m_tqMutekiTimer = 3000;
            setPlayerFlag(0, 0x1001);
        }
        break;
    }
}
void CPlayer::notifyDead(INT inCmd) {
    switch (inCmd) {
    case 0:
        clearSpeed();
        setBattleFlag(0, 0x00443F90);
        setBattleFlag(0, 0x10200000);
        setBattleFlag(0, 0x4060);
        setBattleFlag(0, 0x00130000);
        setPlayerFlag(0, 0x020C2000);
        setBattleFlag(0, 0x03000000);
        CEffectMan::killAll((ObjType::eObjBit)2, 0.0f);
        m_pMove->setFlag(0x10, 0);
        CEffectMan::killAll(*(LPCSTR*)&PL::g_szDpxBreak, 0.0f);

        CEffectMan::killAll(PL::g_szDpxSpeedBreak, 0.0f);
        CEffectMan::killAll(PL::g_szDpxHitWall, 0.0f);
        if (isConditionFlag(0x20000)) setAnimForce(0x11, 0, 0, 0, 0, 0);
        else setAnimForce(0x11, 0, 0, 1, 0, 0);
        importStatus();
        m_iHP = m_iMaxHP;
        if (m_pCB) {
            CardBattle::CCardBattle::setBattleEnd();
            m_pRevolver = NULL;
        }
        m_iSubState = 1;
        m_bDmgMuteki = TRUE;
        m_pMove->setFlag(1, 0);
        setPlayerFlag(1, 0);
        m_collBody.off();
        m_collBody.offBody();
        break;
    case 1:
        m_iSubState = 3;
        setAnimForce(0x27, -1, 0, 0, 0, 0);
        setPlayerFlag(1, 0);
        m_collBody.off();
        m_pCB->destroyPlayCard(1, 0);
        setBattleFlag(0, 0x10200000);
        onWeapon(0);
        setLand(5.0f, 0.5f, 1);
        CSceneEnv::setStop((ObjType::eObjBit)4);
        break;
    }
}
void CPlayer::initBadCondition() {
    setPlayerState(BADCONDITION);
    clearSpeed();
    if (isConditionFlag(1)) {
        m_iSubState = 1;
        setTimer(9000);
        setAnimForce(0x27, -1, 1, 1, 0, 0);
        m_JumpState = 3;
        m_fJumpGravity = *(FLOAT*)&PL::g_fGravity * 5.0f;
        m_fJumpSpdY = 0.0f;
    } else if (isConditionFlag(0x20)) {
        m_iSubState = 6;
        setTimer(15000);
        setAnimSpeed(0.0f);
        fadeEffect(m_hEffBad, 1);
        {
            FVECTOR pos;
            pos._q = 0;
            pos.w = 1.0f;
            m_hEffBad = attachEffect(PL::g_szDpxVexen, 2, &Crd(0), &pos, NULL, 0.0f);
            CEffect* effect = CEffectMan::getEffect(m_hEffBad);
            if (effect) effect->m_iFlag |= 4;
        }
        m_JumpState = 3;
        m_fJumpGravity = *(FLOAT*)&PL::g_fGravity * 5.0f;
        m_fJumpSpdY = 0.0f;
    } else if (isConditionFlag(2)) {
        m_iSubState = 2;
        setAnimSpeed(0.0f);
        fadeEffect(m_hEffBad, 1);
        {
            FVECTOR pos = {{ 0.0f, 0.65f, 0.0f, 1.0f }};
            m_hEffBad = attachEffect(PL::g_szDpxAbnormal, 3, &Crd(m_iBoneCenter), &pos, NULL, 0.0f);
            CEffect* effect = CEffectMan::getEffect(m_hEffBad);
            if (effect) effect->m_iFlag |= 4;
        }
        m_JumpState = 3;
        m_fJumpGravity = *(FLOAT*)&PL::g_fGravity * 5.0f;
        m_fJumpSpdY = 0.0f;
    } else if (isConditionFlag(0x10000)) {
        m_iSubState = 7;
        fadeEffect(m_hEffBad, 1);
        setAnimForce(0x20, 0, 1, 0, 0, 0);
        m_fJumpGravity = 0.0f;
        m_JumpState = 3;
        m_fJumpSpdY = 0.0f;
        m_pMove->setFlag(1, 0);
        setOldPos(m_vCenterPos);
        setTrgFlag(0, 0x8000);
    } else if (isConditionFlag(0x800000)) {
        m_iSubState = 8;
        fadeEffect(m_hEffBad, 1);
        setFaceAnim(3, 0);
        setAnimForce(0x21, 0, 0, 0, 0, 0);
        m_fJumpGravity = 0.0f;
        m_JumpState = 3;
        m_fJumpSpdY = 0.0f;
        m_pMove->setFlag(1, 0);
        setOldPos(m_vCenterPos);
        setTrgFlag(0, 0x8000);
    } else if (isConditionFlag(0x8000000)) {
        m_iSubState = 9;
        fadeEffect(m_hEffBad, 1);
        setAnimRsrcEx(0xFCB, 0, 0, 1, 0, 0, 0);
        m_fJumpGravity = 0.0f;
        m_JumpState = 3;
        m_fJumpSpdY = 0.0f;
        m_pMove->setFlag(1, 0);
        setOldPos(m_vCenterPos);
        setTrgFlag(0, 0x8000);
        m_pCB->disableDuelEntry(0x400000);
    } else if (isConditionFlag(0x8000)) {
        m_iSubState = 0xB;
        setTimer(15000);
        setAnimForce(0x27, -1, 0, 0, 0, 0);
    } else {
        m_iSubState = 0xC;
        setAnimForce(0x28, -1, 0, 0, 0, 0);
    }
    setBattleFlag(0, 8);
}
void CPlayer::exeBadCondition() {
    setBattleFlag(0, 8);
    if (isAir()) {
        FVECTOR vel = m_pMove->getVel();
        m_fJumpSpdY += m_fJumpGravity * m_fFrameRate;
        vel.y = m_fJumpSpdY;
        m_pMove->setVel(vel);
    }
    switch (m_iSubState) {
    case 1:
        if (isTrgAnyPad()) setTimer(getTimer() - 100);
        if (getTimer() <= 0) {
            m_iSubState = 12;
            setAnimForce(0x28, -1, 0, 0, 0, 0);
        }
        break;
    case 2:
        if (isTrgAnyPad()) setTimer(getTimer() - 100);
        if (getTimer() <= 0) {
            m_iSubState = 12;
            setAnimForce(0x28, -1, 0, 0, 0, 0);
            fadeEffect(m_hEffBad, 1);
        }
        break;
    case 6:
        if (isTrgAnyPad()) setTimer(getTimer() - 100);
        if (getTimer() <= 0) {
            m_iSubState = 12;
            setAnimForce(0x28, -1, 0, 0, 0, 0);
            fadeEffect(m_hEffBad, 1);
            m_hEffBad = setEffect(PL::g_szDpxVexen, 4, &getPos(), NULL, 0.0f);
        }
        break;
    case 7:
        PLM::setVibration(4);
        if (getMotionCurrentTime() >= 400) setAnimForce(0x20, 0, 1, 99, 99, 200);
        if (isTrgFlag(0x8000)) {
            setConditionFlag(1, 0x10000);
            m_pMove->setFlag(0, 1);
            setOldPos(m_vCenterPos);
            setFaceAnim(3, 0);
            initBadCondition();
        } else if (chkTrgAction(0, 0)) {
            setConditionFlag(0, 0x10000);
            m_pMove->setFlag(0, 1);
            setOldPos(m_vCenterPos);
        }
        break;
    case 8:
        PLM::setVibration(4);
        if (isMotionEnd()) {
            setAnimForce(0x21, 800, 1, 99, 99, 100);
            setAnimSpeed(0.5f);
        }
        if (isTrgFlag(0x8000)) {
            setConditionFlag(1, 0x800000);
            m_pMove->setFlag(0, 1);
            setOldPos(m_vCenterPos);
            m_iSubState = 12;
            setAnimForce(0x28, -1, 0, 0, 0, 0);
            m_JumpState = 3;
            m_fJumpGravity = *(FLOAT*)&PL::g_fGravity * 5.0f;
            m_fJumpSpdY = 0.0f;
        } else if (chkTrgAction(0, 0)) {
            setConditionFlag(0, 0x800000);
            m_pMove->setFlag(0, 1);
            setOldPos(m_vCenterPos);
        }
        break;
    case 9:
        PLM::setVibration(4);
        if (m_pAnim->getAnimID() != 1) {
            if (isMotionEnd()) setAnimRsrcEx(0xFCB, 1, -1, 1, 0, 0, 0);
            break;
        }
        if (isTrgFlag(0x40)) {
            TRACE("CPlayer::exeBadCondition(); DARKPUNISHER : [TRG_GIMMICK]\n");
            setConditionFlag(0, 0x8000000);
            m_pMove->setFlag(0, 1);
            setOldPos(m_vCenterPos);
            setAnimRsrcEx(0xFCB, 2, -1, 1, 0, 0, 0);
            initGimmickCard(0);
        } else if (isTrgFlag(0x8000)) {
            TRACE("CPlayer::exeBadCondition(); DARKPUNISHER : [TRG_CANCEL]\n");
            setConditionFlag(0, 0x8000000);
            m_pMove->setFlag(0, 1);
            setOldPos(m_vCenterPos);
            m_iSubState = 12;
            setAnimRsrcEx(0xFCB, 2, -1, 1, 0, 0, 0);
        } else if (chkTrgAction(0, 0)) {
            TRACE("CPlayer::exeBadCondition(); DARKPUNISHER : chkTrgAction=OK! ...enableDuelEntry\n");
            setConditionFlag(0, 0x8000000);
            m_pMove->setFlag(0, 1);
            setOldPos(m_vCenterPos);
        } else {
            break;
        }
        m_pCB->disableDuelEntry(0);
        break;
    case 11:
        if (getTimer() <= 0) {
            m_iSubState = 12;
            setAnimForce(0x28, -1, 0, 0, 0, 0);
        }
        break;
    case 12:
        if (isMotionEnd()) initStandUp(0);
        break;
    case 3:
    case 4:
    case 5:
    case 10:
        break;
    }
}
void CPlayer::setConfuse(INT inFlag, TIMEQ inTime) {
    if (!inFlag) {
        setConditionFlag(0, 8);
        m_tqConfuse = 0;
        m_uiPadFlag = (INT)m_uiPadFlag & ~4;
        fadeEffect(m_hEffBad, 0);
    } else if (m_tqConfuse <= 0) {
        /* MATCH: the barriers and the two register locals keep the original's issue order and registers. */
        register UINT uiCond asm("$3") = m_uiConditionFlag;
        register TIMEQ tq asm("$2") = 21000;
        __asm__ volatile("");
        UINT uiPad = m_uiPadFlag;
        if (inTime) tq = inTime;
        __asm__ volatile("");
        m_tqConfuse = tq;
        m_uiConditionFlag = uiCond | 8;
        m_uiPadFlag = uiPad | 4;
        FVECTOR pos = {0.0f, 0.65f, 0.0f, 1.0f};

        m_hEffBad = attachEffect(PL::g_szDpxAbnormal, 2, &Crd(m_iBoneCenter), &pos, NULL, 0.0f);
        CEffect* effect = CEffectMan::getEffect(m_hEffBad);
        if (effect) effect->m_iFlag |= 4;
    }
}
void CPlayer::exeConfuse() {
    if (m_uiConditionFlag & 8) {
        m_tqConfuse -= m_tqFrameRate;
        if (m_tqConfuse <= 0) setConfuse(0, 0);
    }
}
void CPlayer::initItem(INT inNo) {
    setPlayerState(PL::ITEM);
    clearSpeed();
    setBattleFlag(0, 8);
    setBattleFlag(0, 0x80);
    setBattleFlag(0, 0x80000);
    setTrgFlag(0, 8);
    setPlayerFlag(0, 0x02800000);
    m_iAttackNo = inNo;
    m_uiPreAttackKind = 0;
    m_bIsAttackHit = FALSE;
    m_bNextTrg = FALSE;
    m_iNextAttack = 0;
    m_bDmgMuteki = FALSE;
    m_bIsCanCombo = FALSE;
    m_fJumpGravity = *(FLOAT*)&PL::g_fGravity;
    m_JumpState = 3;
    m_iSubState = 1;
    m_fMoveSpdXZ = 0.0f;
    m_tqCardBreakTime = PL::g_CardBreakTime[(*(BC_PARAM**)&CardBattle::g_pBattleCardList)[m_uiCardID].BREAK];
}
void CPlayer::exeItem() {
    HANDLE handle;
    CEffect* effect;
    INT frame = getMotionCurrentTime() / 50;
    setBattleFlag(0, 8);
    switch (m_iSubState) {
    case 1:
        if (isAir()) {
            setAnim(6, 800, 0);
            FVECTOR vel = m_pMove->getVel();
            m_fJumpSpdY += m_fJumpGravity * m_fFrameRate;
            vel.y = m_fJumpSpdY;
            m_pMove->setVel(vel);
        } else {
            m_iSubState = 2;
            setAnimForce(0x2D, -1, 0, 1, 0, 0);
            m_tqStateTimer = 0;
        }
        break;
    case 2:
        if (frame >= 56) {
            FVECTOR pos;
            pos.y = 0.85f;
            m_iSubState = 3;
            pos.x = 0.0f;
            pos.w = 1.0f;
            pos.z = 0.0f;
            if (CGame::isRikuGame()) pos.y = 1.2f;
            handle = 0;
            switch (m_uiCardID) {
            case 0x2A:
                handle = attachEffect(PL::g_szDpxAlways, 3, &Crd(0), &pos, NULL, 0.0f);
                break;
            case 0x2B:
                handle = attachEffect(PL::g_szDpxAlways, 4, &Crd(0), &pos, NULL, 0.0f);
                break;
            case 0x2C:
                handle = attachEffect(PL::g_szDpxItemplus, 0, &Crd(0), &pos, NULL, 0.0f);
                break;
            case 0x2D:
                handle = attachEffect(PL::g_szDpxAlways, 5, &Crd(0), &pos, NULL, 0.0f);
                break;
            case 0x2E:
                handle = attachEffect(PL::g_szDpxItemplus, 1, &Crd(0), &pos, NULL, 0.0f);
                break;
            case 0x2F:
                handle = attachEffect(PL::g_szDpxAlways, 6, &Crd(0), &pos, NULL, 0.0f);
                break;
            case 0x30:
                handle = attachEffect(PL::g_szDpxItemplus, 2, &Crd(0), &pos, NULL, 0.0f);
                break;
            }
            effect = CEffectMan::getEffect(handle);
            if (effect) effect->m_iFlag |= 4;
            if (m_pRevolver) {
                if (m_iStepStockSkill > 0) m_pRevolver->setAction(0, m_iStepStockSkill - 1);
                else m_pRevolver->setAction(0, 0);
            }
        }
        break;
    case 3:
        if (isBattleFlag(0x1000) && exeUseStock(1)) {
            if (isPlayerFlag(0x80000000)) setBGFade(1.0f, 20.0f, 0);
        } else if (isMotionEnd()) {
            procSkillEnd(-1);
        }
        break;
    }
}
void CPlayer::exeStandUp_inFriend() {
    WalkMode PreWalkMode = m_WalkMode;
    WalkMode mode = chkWalkMode();
    m_WalkMode = mode;
    if (mode == WALKMODE_STAND && PreWalkMode != WALKMODE_STAND && m_pSE) ((CSEMotion*)m_pSE)->playFootStopSE();
    exeWalk();
}
void CPlayer::initCharge() {
    setPlayerState(PL::CHARGE);
    clearSpeed();
    setTrgFlag(0, 0x10);
    setAnimForce(0x2E, -1, 0, 0, 0, 0);
    m_tqEffCharge = 1000;
    m_bDmgMuteki = FALSE;
    if (m_hEffCharge <= 0) {
        FVECTOR pos;
        pos._q = 0;
        pos.w = 1.0f;
        CCoord* crd = &Crd(0);
        m_hEffCharge = attachEffect(PL::g_szDpxReload, 0, crd, &pos, NULL, 0.0f);
        CEffect* effect = CEffectMan::getEffect(m_hEffCharge);
        if (effect) effect->m_iFlag |= 4;
    }
}
void CPlayer::exeCharge() {
    INT result;
    m_tqEffCharge = 1000;
    switch (m_iSubState) {
    case 0:
        if (isTrgFlag(0x10)) {
            if (!isActionFlag(0x2000000) && isAir()) {
                initFall();
                return;
            }
            result = m_pRevolver->chargeReloadCard(0);
            if (result == 1) {
                FVECTOR pos;
                pos._q = 0;
                pos.w = 1.0f;
                CCoord* crd = &Crd(0);
                CEffect* effect = CEffectMan::getEffect(attachEffect(PL::g_szDpxReload, 1, crd, &pos, NULL, 0.0f));
                if (effect) effect->m_iFlag |= 4;
                setAnimForce(0x2F, -1, 0, 0, 0, 0);
                m_iSubState = result;
                fadeEffect(m_hEffCharge, 1);
            } else if (result < 0) {
                initStandUp(0);
            }
        } else {
            initStandUp(0);
        }
        break;
    case 1:
        if (isMotionEnd()) initStandUp(0);
        break;
    }
}
INT CPlayer::chkChargeMove(INT inFlag) {
    CEffect* effect;
    if (CGameWork::getFlag(1, 8)) return TRUE;
    if (!inFlag && CGame::isSoraGame()) {
        if (!m_pCB) return FALSE;
        if (m_pCB->getEnchantKind() != 10) return FALSE;
    }
    setPlayerFlag(0, 0x10);
    FVECTOR pos = { 0.0f, 0.0f, 0.0f, 1.0f };
    if (m_pRevolver->chargeReloadCard(0) != 1) {
        m_tqEffCharge = 1000;
        if (m_hEffCharge) return TRUE;
        m_hEffCharge = attachEffect(PL::g_szDpxReload, 0, &Crd(0), &pos, NULL, 0.0f);
        effect = CEffectMan::getEffect(m_hEffCharge);
        if (effect) effect->m_iFlag |= 4;
    } else {
        fadeEffect(m_hEffCharge, 1);
        effect = CEffectMan::getEffect(attachEffect(PL::g_szDpxReload, 1, &Crd(0), &pos, NULL, 0.0f));
        if (effect) effect->m_iFlag |= 4;
    }
    return TRUE;
}
void CPlayer::fadeEffCharge() {
    if (m_hEffCharge > 0) {
        m_tqEffCharge -= getFrameRate();
        if (m_tqEffCharge <= 0) {
            m_tqEffCharge = 0;
            CEffect* effect = CEffectMan::getEffect(m_hEffCharge);
            if (effect) effect->fadeout(8.0f);
            m_hEffCharge = 0;
        }
    }
}
void CPlayer::setCure(INT inID, INT inPow) {
    FVECTOR pos;
    INT no = inID - 14;
    if (inID == 0x33) no = 0;
    pos.set(0.0f, 0.85f, 0.0f); /* the original: a brace init (stores on the declaration line); that form swaps $f0/$f1 here */
    CEffect* effect = CEffectMan::getEffect(attachEffect(PL::g_EffTbl_Skill[inID][1].pEffectName, no, &Crd(0), &pos, NULL, 0.0f));
    if (effect) effect->m_iFlag |= 4;
    addHP(inPow, 0, 1);
}
void CPlayer::setCastEffect(const CHAR* inName, INT inNo, CCoord* inCrd, FVECTOR* inOfs) {
    FVECTOR pos;
    if (inOfs) {
        pos = *inOfs;
    } else {
        pos._q = 0;
        pos.w = 1.0f;
    }
    m_hEffCast = attachEffect(inName, inNo, inCrd, &pos, NULL, 0.0f);
    CEffect* effect = CEffectMan::getEffect(m_hEffCast);
    if (effect) effect->m_iFlag |= 4;
}
INT CPlayer::calcAppearPos(FLOAT inDist, FLOAT inRadius, FVECTOR* inCenter) {
    FVECTOR pos = inCenter ? *inCenter : m_vCenterPos;
    FMATRIX m;
    FVECTOR mpos;
    UnitMatrix(m);
    RotMatrixY(m, m, m_fDir);
    m_pCollMap->setType(COLL_TYPE_SPHERE);
    m_pCollMap->enableSliding(0);
    pos.w = 1.0f;
    if (inRadius > 0.5f) {
        FLOAT _z = inRadius - 0.5f;
        FVECTOR base = {{ 0.0f, 0.0f, inRadius - _z, 1.0f }};
        ApplyMatrixXYZ(base, m, base);
        AddVectorXYZ(base, pos, base);
        m_pCollMap->setRadius(0.5f);
        m_pCollMap->setShiftY(0.0f);
        m_pCollMap->setStartPos(pos);
        m_pCollMap->setEndPos(base);
        if (m_pCollMap->collideRide(0) || m_pCollMap->collideMap()) base = pos;
        FVECTOR start = {{ 0.0f, 0.0f, 0.5f - inRadius, 1.0f }};
        ApplyMatrixXYZ(start, m, start);
        AddVectorXYZ(start, pos, start);
        m_pCollMap->setRadius(inRadius);
        m_pCollMap->setShiftY(0.0f);
        m_pCollMap->setStartPos(base);
        m_pCollMap->setEndPos(start);
        if (m_pCollMap->collideRide(0)) {
            start = *m_pCollMap->getDetectPos();
            if (m_pCollMap->collideMap()) {
                mpos = *m_pCollMap->getDetectPos();
                if (Math3D::NearVector(start, mpos, pos) > 0) start = mpos;
            }
        } else if (m_pCollMap->collideMap()) {
            start = *m_pCollMap->getDetectPos();
        }
        pos = start;
        pos.w = 1.0f;
    }
    m_vAppearPos.set(0.0f, 0.0f, inDist);
    ApplyMatrixXYZ(m_vAppearPos, m, m_vAppearPos);
    AddVectorXYZ(m_vAppearPos, m_vAppearPos, pos);
    m_pCollMap->setRadius(inRadius);
    m_pCollMap->setShiftY(0.0f);
    m_pCollMap->setStartPos(pos);
    m_pCollMap->setEndPos(m_vAppearPos);
    if (m_pCollMap->collideRide(0)) {
        m_vAppearPos = *m_pCollMap->getDetectPos();
        if (m_pCollMap->collideMap()) {
            mpos = *m_pCollMap->getDetectPos();
            if (Math3D::NearVector(m_vAppearPos, mpos, pos) > 0) m_vAppearPos = mpos;
        }
        return TRUE;
    }
    if (m_pCollMap->collideMap()) {
        m_vAppearPos = *m_pCollMap->getDetectPos();
        return TRUE;
    }
    return FALSE;
}
void CPlayer::calcAppearPosFS(FLOAT inDist, FLOAT inRadius) {
    FVECTOR pos;
    BOOL hit;
    FLOAT height;
    pos = getPos();
    FVECTOR zero = {{m_vStartPos.x, pos.y, m_vStartPos.z, 1.0f}};
    FLOAT dir0 = calcDirection(zero);
    FMATRIX m;
    UnitMatrix(m);
    RotMatrixY(m, m, dir0);
    FVECTOR start = {{0.0f, 0.0f, inDist, 1.0f}};
    ApplyMatrixXYZ(start, m, start);
    AddVectorXYZ(pos, pos, start);
    FVECTOR vStart = {{pos.x, pos.y + 0.5f, pos.z, 1.0f}};
    FVECTOR vEnd = {{pos.x, -2.0f, pos.z, 1.0f}};
    if (m_pPLM->isJaferBattle()) vStart.y = 7.0f;
    else if (m_pPLM->isTricksterBattle()) vStart.y = 3.0f;
    m_pCollMap->setType(COLL_TYPE_SPHERE);
    m_pCollMap->enableSliding(0);
    m_pCollMap->setRadius(inRadius);
    m_pCollMap->setStartPos(vStart);
    m_pCollMap->setEndPos(vEnd);
    hit = FALSE;
    height = vEnd.y;
    if (m_pCollMap->collideRide(0)) {
        FLOAT detectH = m_pCollMap->getDetectPos()->y;
        hit = TRUE;
        if (height < detectH) height = detectH;
    }
    if (m_pCollMap->collideMap()) {
        FLOAT detectH = m_pCollMap->getDetectPos()->y;
        hit = TRUE;
        if (height < detectH) height = detectH;
    }
    if (!hit) height = 0.0f;
    m_vAppearPos.set(pos.x, height + 0.0035f, pos.z);
}
void CPlayer::calcAppearPosFS_toAttackID() {
    if (m_uiAttackID < 0x78) {
        if (m_uiAttackID >= 0x76) {
            calcAppearPosFS(0.0f, m_fWallRadius);
            return;
        }
    }
    calcAppearPosFS(2.0f, m_fWallRadius);
}
INT CPlayer::chkEscape() {
    BOOL bPlay;
    FVECTOR vNormal;
    if (m_PlayerState == PL::ESCAPE) return FALSE;
    bPlay = FALSE;
    if (m_pCB && (CardBattle::CCardBattle::isPlayingCard(1) || isBattleFlag(0x10200000))) bPlay = TRUE;
    if (m_fAnalogPower < 0.3f) bPlay = TRUE;
    if (bPlay || isBattleFlag(4) || m_pMove->getWallCollInfo().infoATTR != 5 || m_PlayerState == 0x20) {
        if (isBattleFlag(2)) {
            m_tqEscape = 0;
            setBattleFlag(0, 2);
            if (m_pEscape) m_pEscape->destroy();
            setBGFade(1.0f, 0.0f, 0);
        }
        return FALSE;
    }
    if (!isBattleFlag(2)) {
        setBattleFlag(2, 0);
        m_tqEscape = 0;
        if (m_pEscape) m_pEscape->setCharge(0.0f);
        setBGFade(0.5f, 120.0f, 0);
    }
    vNormal = m_pMove->getEscapeNormal();
    if (_abs(vNormal.x) > _abs(vNormal.z)) vNormal.z = 0.0f;
    else vNormal.x = 0.0f;
    FLOAT d = atan2f(vNormal.x, vNormal.z) + 3.1415927f;
    FLOAT r;
    if (d < -3.1415927f) r = d + 6.2831855f;
    else {
        r = d;
        if (r > 3.1415927f) r -= 6.2831855f;
    }
    m_fWallDir = r;
    m_tqEscape += m_tqFrameRate;
    if (m_tqEscape < 6000) {
        FLOAT charge = m_tqEscape * 100.0f / 6000.0f;
        if (m_pEscape) m_pEscape->setCharge(charge);
        return FALSE;
    }
    m_tqEscape = 6000;
    if (m_pEscape) m_pEscape->setCharge(100.0f);
    if (isAir()) return FALSE;
    initEscape();
    return TRUE;
}
void CPlayer::initEscape() {
    FVECTOR rot;
    setPlayerState(PL::ESCAPE);
    setBattleFlag(0, 8);
    setBattleFlag(0, 0x00443F90);
    setBattleFlag(0, 0x10200000);
    setBattleFlag(0, 0x4060);
    setBattleFlag(0, 0x00130000);
    setPlayerFlag(0, 0x020C2000);
    setBattleFlag(0, 0x03000000);
    clearSpeed();
    m_collBody.setDamage(0);
    m_collBody.off();
    m_collBody.offBody();
    m_bDmgMuteki = TRUE;
    resetAction(7);
    m_fDir = m_fWallDir;
    rot = getRot();
    rot.y = m_fDir;
    setRot(rot);
    m_pMove->setFlag(1, 0); // partial CMovement: (flag, on)
    setPlayerFlag(0x400001, 0);
    setAnim(0x2C, -1, 0);
    m_WalkMode = WALKMODE_RUN;
    m_fMovePower = 1.0f;
    m_fWalkSpeed = m_pParam->MAX_RUN_SPEED / 60.0f;
    m_pMove->calcVelocityXZ(m_fWalkSpeed);
    m_tqEscape = 0;
    m_pEscape->destroy();
    CardBattle::CCardBattle::setBattleEnd();
    m_pRevolver = NULL;
    CGameWork::setInt(1, 0xE2, -1);
    setBGFade(0.0f, 60.0f, 1);
    m_pCam->setFlag(1, 0);
    m_pCam->setFlag(0x1000, 0);
    if (getPos().z < 0.0f) m_vStartPos.z = 0.0f;
}
void CPlayer::exeEscape() {
    setPlayerFlag(0x400000, 0);
    m_pCam->setFlag(0x1000, 0);
    if (!m_iSubState) {
        m_tqEscape += m_tqFrameRate;
        if (m_tqEscape >= 2501) {
            if (CSceneInfo::reqExitBattle()) m_iSubState = 1;
        }
    }
}
void CPlayer::setBGFade(FLOAT inRate, FLOAT inTime, INT inFlag) {
    FVECTOR v = {inRate, inRate, inRate, 1.0f};

    setBGFade(v, inTime, inFlag);
}
void CPlayer::setBGFade(FVECTOR& inColor, FLOAT inFrame, INT inFlag) {
    INT iObjType = 0x1808;
    if (inFlag & 1) iObjType = 0x187C;
    if (inColor.x + inColor.y + inColor.z == 3.0f) setPlayerFlag(0, 0x80000000);
    else setPlayerFlag(0x80000000, 0);
    CSceneEnv::setColor((ObjType::eObjBit)iObjType, inFrame, inColor, NULL, 0);
    if (inFlag & 2) CSceneEnv::setStop((ObjType::eObjBit)iObjType);
}
void CPlayer::setObjStop(UINT inFlag) {
    if (!inFlag) CSceneEnv::setStop((ObjType::eObjBit)0);
    else CSceneEnv::setStop((ObjType::eObjBit)0x7C);
}
INT CPlayer::exeWarp(INT inSet) {
    if (inSet) {
        if (isDispOFF()) return TRUE;
        if (!(m_fScale > 0.0f)) return TRUE;
        m_fScale -= getFrameRateF() * 0.05f;
        if (m_fScale <= 0.0f) {
            setEntityFlag(1, 0);
            m_fScale = 1.0f;
            offWeapon();
        }
        FVECTOR vScale = getScale();
        vScale.x = m_fScale;
        vScale.y = 2.0f - m_fScale;
        vScale.z = m_fScale;
        setScale(vScale);
        setConditionFlag(0x1000000, 0);
        setPlayerFlag(1, 0);
        return FALSE;
    }
    if (isDispOFF()) {
        m_fScale = 0.0f;
        setEntityFlag(0, 1);
        onWeapon(0);
    }
    if (m_fScale < 1.0f) {
        m_fScale += getFrameRateF() * 0.05f;
        if (m_fScale > 1.0f) m_fScale = 1.0f;
        FVECTOR vScale = getScale();
        vScale.x = m_fScale;
        vScale.y = 2.0f - m_fScale;
        vScale.z = m_fScale;
        setScale(vScale);
        return FALSE;
    }
    setConditionFlag(0, 0x1000000);
    setPlayerFlag(0, 1);
    return TRUE;
}
void CPlayer::setBanish(INT inFlag) {
    if (!inFlag) {
        setConditionFlag(0, 0x40);
        FVECTOR vColor = {1.0f, 1.0f, 1.0f, 1.0f};

        CSceneEnv::setColor((ObjType::eObjBit)2, 30.0f, vColor, NULL, 2);
    } else {
        setConditionFlag(0x40, 0);
        m_tqBanish = 0;
        PL::g_vBanishPos = getPos();
    }
}
void CPlayer::exeBanish() {
    if (isConditionFlag(0x40)) {
        FVECTOR vColor = {1.0f, 1.0f, 1.0f, 0.5f};

        vColor.w = (cosf((FLOAT)m_tqBanish * 0.0006f) + 1.0f) * 0.5f * 0.85f + 0.1f;
        CSceneEnv::setColor((ObjType::eObjBit)2, 0.0f, vColor, NULL, 2);
        m_tqBanish += m_tqFrameRate;
        if (vColor.w > 0.8f) {
            m_tqBanish += m_tqFrameRate;
            if (vColor.w > 0.9f) PL::g_vBanishPos = getPos();
        }
    }
}
void CPlayer::setReflectGuard(INT inFlag) {
    if (!inFlag) {
        setConditionFlag(0, 0x200);
        m_collBody.m_GuardAttrFlag = 0;
        m_collBody.m_uiFlag = (INT)m_collBody.m_uiFlag & ~2;
    } else {
        setConditionFlag(0x200, 0);
        m_collBody.m_uiFlag |= 2;
        CCollBody& body = m_collBody;
        body.setGuardDir(1.5707964f);
        body.m_GuardAttrFlag |= 1;
    }
}
void CPlayer::setQuickBarrier(INT inFlag) {
    if (m_tqBarrier > 0) {
        m_collBody.m_uiFlag = (INT)m_collBody.m_uiFlag & ~2;
        m_tqBarrier = 0;
        m_collBody.m_GuardAttrFlag = 0;
    } else {
        m_tqBarrier = 6000;
        CCollBody& body = m_collBody;
        m_collBody.m_uiFlag |= 2;
        body.m_GuardAttrFlag |= 1;
        m_collBody.m_uiFlag |= 2;
        body.m_GuardAttrFlag |= 2;
        m_collBody.m_uiFlag |= 2;
        body.m_GuardAttrFlag |= 4;
        m_collBody.m_uiFlag |= 2;
        body.m_GuardAttrFlag |= 8;
        m_collBody.m_uiFlag |= 2;
        body.m_GuardAttrFlag |= 0x10;
        m_collBody.m_uiFlag |= 2;
        body.setGuardDir(3.1415927f);
        body.m_GuardAttrFlag |= 0x20;
    }
}
void CPlayer::exeQuickBarrier() {
    if (m_tqBarrier) {
        m_tqBarrier -= m_tqFrameRate;
        if (m_tqBarrier <= 0) {
            m_collBody.m_uiFlag = (INT)m_collBody.m_uiFlag & ~2;
            m_tqBarrier = 0;
            m_collBody.m_GuardAttrFlag = 0;
        }
    }
}
void CPlayer::setAeroGuard(INT inFlag) {
    CBullet* pBullet;
    BULLET_PARAM param;
    if (!inFlag) {
        setConditionFlag(0, 0x80000000);
        m_tqGuard = 0;
        pBullet = (CBullet*)CTaskMan::getTask(m_hEffGuard);
        if (pBullet) pBullet->out();
    } else {
        setConditionFlag(0x80000000, 0);
        m_tqGuard = 6000;
        param.init(1, 3);
        param.fSpeed = 0.0f;
        param.CollKind = 0x80000000;
        param.uiFlag = 3;
        param.AtkKind = 1;
        param.AtkAttr = 1;
        param.iAttackPoint = m_iAP;
        param.fMaxDist = 50.0f;
        param.vPos = m_vCenterPos;
        param.fCollRadius = 0.8f;
        param.fShiftY = 0.0f;
        param.pEffectTbl = PL::g_EffTbl_Skill[66];
        pBullet = (CBullet*)CEntityFactory::getInstance()->createBullet(param, this);
        m_hEffGuard = pBullet->getHandle();
        pBullet->setBodyRadius(0.0f, 0.0f, 0, (COLL_KIND)0);
        pBullet->userBodyCollOffBody(-1);
        pBullet->userBodyCollOff(-1);
    }
}
void CPlayer::exeAeroGuard() {
    if (isConditionFlag(0x80000000)) {
        CBullet* pBullet = (CBullet*)CTaskMan::getTask(m_hEffGuard);
        if (isConditionFlag(0x1000000)) {
            if (pBullet) {
                pBullet->out();
                m_hEffGuard = 0;
            }
        } else if (!pBullet) {
            setAeroGuard(1);
        } else {
            m_tqBarrier -= m_tqFrameRate;
            if (m_tqBarrier <= 0) {
                m_tqBarrier = 6000;
                pBullet->updateAttackID();
            }
            pBullet->warp(m_vCenterPos);
        }
    }
}
void CPlayer::setPrizeHealing(TIMEQ inTime) {
    if (!m_pInstance) return;
    if (!inTime) {
        if (m_hEffPrize > 0) {
            CEffect* effect = CEffectMan::getEffect(m_hEffPrize);
            if (effect) effect->loopEnd();
            m_hEffPrize = 0;
        }
        m_tqPrize = 0;
    } else {
        if (!m_tqPrize) {
            m_hEffPrize = attachEffect(PL::g_szDpxAlways, 0x16, &Crd(m_iBoneCenter), NULL, NULL, 0.0f);
            CEffect* effect = CEffectMan::getEffect(m_hEffPrize);
            if (effect) effect->m_iFlag |= 4;
        }
        m_tqPrize = inTime;
    }
}
void CPlayer::exePrizeHealing() {
    if (m_hEffPrize) {
        m_tqPrize -= m_tqFrameRate;
        if (m_tqPrize <= 0) {
            if (m_hEffPrize > 0) {
                CEffect* effect = CEffectMan::getEffect(m_hEffPrize);
                if (effect) effect->loopEnd();
                m_hEffPrize = 0;
            }
            m_tqPrize = 0;
        }
    }
}
__asm__(".rdata\n");
void CPlayer::initGimmickCard(INT inNo) {
    TRACE("CPlayer:: initGimmickCard()\n");
    setPlayerState(PL::GIMMICKCARD);
    m_iSubState = 1;
    clearSpeed();
    setPlayerFlag(1, 0);
    offAttack();
    setBattleFlag(0, 8);
    setBattleFlag(0, 0x200);
    setBattleFlag(0, 0x80000);
    setTrgFlag(0, 0x40);
    m_iAttackNo = inNo;
    m_uiPreAttackKind = 0;
    m_uiAttackKind = 0;
    m_bIsAttackHit = FALSE;
    m_bNextTrg = FALSE;
    m_iNextAttack = 0;
    m_bDmgMuteki = FALSE;
    m_bIsCanCombo = FALSE;
    m_fJumpGravity = *(FLOAT*)&PL::g_fGravity;
    m_JumpState = 3;
    m_fMoveSpdXZ = 0.0f;
    m_tqCardBreakTime = 0;
    m_fWalkSpeed = 0.0f;
}
void CPlayer::exeGimmickCard() {
    TIMEQ frame;
    TIMEQ f;
    frame = getMotionCurrentTime() / 50;
    f = frame;
    setBattleFlag(0, 8);
    switch (m_iSubState) {
    case 1:
        if (isAir()) {
            setAnim(6, 800, 0);
            FVECTOR vel = m_pMove->getVel();
            m_fJumpSpdY += m_fJumpGravity * m_fFrameRate;
            vel.y = m_fJumpSpdY;
            m_pMove->setVel(vel);
        } else if (isBattleFlag(0x400000)) {
            m_iSubState = 4;
            setAnim(0x2A, -1, 0);
            m_tqStateTimer = 0;
        } else {
            m_iSubState = 2;
            if (CGame::isSoraGame()) setAnimRsrcEx(0xFF0, 0, -1, 0, 0, 0, 1);
            else if (!isConditionFlag(0x400000)) setAnimRsrcEx(0xFF2, 0, -1, 0, 0, 0, 1);
            else setAnimRsrcEx(0xFF3, 0, -1, 0, 0, 0, 1);
            m_tqStateTimer = 0;
        }
        break;
    case 2: {
        if (m_tqStateTimer > 5000) {
            TRACE("CPlayer:: exeGimmickCard() %d, \x81\xa6\x83G\x83\x89\x81[\x8f\x88\x97\x9d\n", frame);
            f = 50;
        }
        if (f >= 50) {
            TRACE("CPlayer:: exeGimmickCard() notifyMakeGimmick\n");
            CardBattle::CCardBattle::notifyMakeGimmick(NULL, 0);
            m_iSubState = 3;
        }
        break;
    }
    case 3:
        if (isMotionEnd()) setAnim(0x2A, -1, 0);
        if (CardBattle::CCardBattle::getGimmickOwner() > 0) {
            if (CardBattle::CCardBattle::isPlayingGimmickCard()) break;
        } else {
            TRACE("CPlayer:: exeGimmickCard() notifyCompleteGimmick, \x81\xa6\x83G\x83\x89\x81[\x8f\x88\x97\x9d GimmickOwner=0\n");
            CardBattle::CCardBattle::notifyCompleteGimmick();
        }
        if (!isMotionEnd() && m_iNowAnimID != 0x2A) break;
        setPlayerFlag(0, 1);
        m_pCB->destroyPlayCard(1, 0);
        setBattleFlag(0, 0x10200000);
        procSkillEnd(-1);
        break;
    case 4:
        if (frame >= 30) {
            m_iSubState = 5;
            setEffect(PL::g_szDpxZexion, 0x1C, &getPos(), NULL, 0.0f);
            setAnimForce(0x1F, 0, 1, 0, m_bIsDead, 0);
            CardBattle::CCardBattle::notifyMakeGimmick(NULL, 0);
            PLM::setVibration(3);
        }
        break;
    case 5:
        if (m_bIsDead) {
            initDead();
        } else if (!CardBattle::CCardBattle::isPlayingGimmickCard()) {
            setPlayerFlag(0, 1);
            m_pCB->destroyPlayCard(1, 0);
            setBattleFlag(0, 0x10200000);
            setBattleFlag(0, 0x400000);
            setConditionFlag(1, 0);
            setFaceAnim(3, 0);
            initBadCondition();
        }
        break;
    }
}
__asm__(".rdata\n");
void CPlayer::setTrapDamage(INT inPower, const CHAR* lpszInName, UINT nNo) {
    INT face;
    INT hp0;
    TRACE("PL:: setTrapDamage\n");
    if (DebugWork::g_dw.player.iMuteki) return;
    if (lpszInName) setEffect(lpszInName, nNo, &m_vCenterPos, NULL, 0.0f);
    face = 1;
    hp0 = m_iHP;
    hp0 -= inPower;
    if (m_pCB && m_pCB->getEnchantKind() == 0x3B) {
        hp0 += inPower;
    } else {
        hp0 += (DebugWork::g_dw.player.iDamage0 > 0) * inPower;
        setDamageNumber(m_vHeadPos, inPower);
    }
    if (m_pCB) {
        if (hp0 <= 0 && m_iHP >= 2 && m_pCB->getEnchantKind() == 0x29) {
            hp0 = 1;
            CardBattle::CCardBattle::decEnchantTime(1, 0x28, 0x29);
        }
        CardBattle::CCardBattle::decEnchantTime(1, 0x27, 0);
    }
    if (hp0 < 0) hp0 = 0;
    m_iHP = hp0;
    TRACE("-------- Damege Info ------------------\n");
    TRACE("Riku : HP = %d/%d\n", m_iHP, m_iMaxHP);
    TRACE("---------------------------------------\n");
    if (hp0 <= 0) {
        m_bIsDead = TRUE;
        face = 2;
        m_pPLM->notifyDeadSE(CSound::playSE(0x4C, -1, 0x40));
        CGameWork::setFlag(1, 6, TRUE);
    }
    if (m_pGauge) m_pGauge->setDamage(m_iHP, 0);
    setFaceAnim(face, 1);
}
void CPlayer::fadeEffect(INT& ioHandle, INT inLoop) {
    if (ioHandle > 0) {
        CEffect* effect = CEffectMan::getEffect(ioHandle);
        if (effect) {
            if (inLoop && effect->isLoop()) effect->loopEnd();
            else effect->fadeout(8.0f);
        }
        ioHandle = 0;
    }
}
void CPlayer::fadeEffAttack(INT inLoop) {
    INT i;
    for (i = 0; i < 4; i++) {
        if (m_hEffAttack[i] > 0) {
            CEffect* effect = CEffectMan::getEffect(m_hEffAttack[i]);
            if (effect) {
                if (inLoop && effect->isLoop()) effect->loopEnd();
                else effect->fadeout(8.0f);
            }
            m_hEffAttack[i] = 0;
        }
    }
}
void CPlayer::setEffAttack(INT inNum, const CHAR* inName, INT inNo, INT inBone, FVECTOR* inOfs) {
    FVECTOR pos;
    if (inOfs) {
        pos = *inOfs;
    } else {
        pos._q = 0;
        pos.w = 1.0f;
    }
    m_hEffAttack[inNum] = attachEffect(inName, inNo, &Crd(inBone), &pos, NULL, 0.0f);
    CEffect* effect = CEffectMan::getEffect(m_hEffAttack[inNum]);
    if (effect) effect->m_iFlag |= 4;
}
void CPlayer::updateSpotLight(HANDLE inHandle) {
    CEffect* effect = CEffectMan::getEffect(inHandle);
    if (effect) {
        FVECTOR n = m_pMove->getDetectNormal();
        FMATRIX m;
        _unitMatrixVu0(m);
        RotMatrixX(m, m, atan2f(_sqrtf(n.z * n.z + n.x * n.x), n.y));
        RotMatrixY(m, m, atan2f(n.x, n.z));
        FVECTOR pos = getPos();
        pos.w = 1.0f;
        m.v[3] = pos;
        effect->getCoords()->getLocalMatrix(0) = m;
        effect->getCoords()->Crd(0).m_Flag = 1;
    }
}
__asm__(".rdata\n");
void CPlayer::setQuake(FLOAT inCycle, FLOAT inPower, UINT inTime) {
    CCamera* cam = CWorldEnv::getActiveCamera();
    CameraQuakeParam cqp;
    cqp._fCycle = inCycle;
    cqp._nTime = inTime;
    cqp._fPower = inPower;
    new (__FILE__ "(12257)") CCameraQuakeV(*cam, &cqp, inPower);
}
void CPlayer::notifyReturnCard(const CARD* inCard) {
    if (!m_pInstance) return;
    if (m_pRevolver) {
        if (!inCard) {
            m_pRevolver->resetCardOwner(2, 0);
            CBattleEnemyManager* pBEM = (CBattleEnemyManager*)CTaskMan::getSysTask(0x19);
            if (pBEM) pBEM->allGetPrize(2);
        } else if (inCard->m_nKind < 0xCB) {
            INT cat = inCard->m_nCategory0;
            if (cat && (cat < 7 || cat == 8)) m_pRevolver->addCard((CARD*)inCard, 1);
        }
    }
}
void CPlayer::notifyGetPrize(INT inKind, INT inNumber, INT inState) {
    CBattleEnemyManager* pBEM;
    INT bLvUpDisp;
    CARD card;
    INT work;
    INT work2;
    if (inKind <= 0xCA) {
        _setCard(card, inKind, inNumber);
        switch (card.m_nCategory20) {
        case 7:
            _setCard(card, inKind, 0xFF);
        case 1:
        case 2:
        case 3:
        case 4:
        case 9:
        case 10:
        case 11:
            if (inState == 3) {
                if (_instance()) _instance()->m_pRevolver->addCard(&card, 1);
            } else {
                CCard::addCard(inKind, inNumber);
                if (_instance()) {
                    switch (inState) {
                    case 1:
                        Gauge::CPrizeGauge::setPrizeCard(card, _instance()->m_bIsBattleScene + 1, 0);
                        break;
                    case 2:
                        Gauge::CPrizeGauge::setPrizeCard(card, 2, 0);
                        break;
                    }
                } else if (inState > 0) {
                    Gauge::CPrizeGauge::setPrizeCard(card, inState, 0);
                }
                INT mode = CGame::getDbgGameMode();
                if (mode > 3) {
                    if (mode < 7) CCard::addDeck(card, 0);
                    else if (mode < 9) CCard::addDeck(card, 0);
                }
            }
            break;
        case 5:
        case 6:
            if (_instance()) {
                if (_instance()->m_pRevolver) {
                    if (inState == 3) {
                        _instance()->m_pRevolver->addCard(&card, 1);
                        break;
                    }
                    _instance()->m_pRevolver->addCard(&card, 0);
                }
                CJiminyMemoFlag::checkEvent(4, card.m_nKind, 0);
            }
            break;
        case 8:
            break;
        }
        CGameWork::setFlag(1, 0xF, TRUE);
        work = CGameWork::getInt(0, 0x21);
        work2 = CGameWork::getInt(0, 0x2D);
        switch (card.m_nKind) {
        case 0x7:
            CGameWork::setInt(0, 0x21, work | 0x1);
            break;
        case 0x8:
            CGameWork::setInt(0, 0x21, work | 0x2);
            break;
        case 0xA:
            CGameWork::setInt(0, 0x21, work | 0x4);
            break;
        case 0xD:
            CGameWork::setInt(0, 0x21, work | 0x8);
            break;
        case 0xE:
            CGameWork::setInt(0, 0x21, work | 0x10);
            break;
        case 0xF:
            CGameWork::setInt(0, 0x21, work | 0x20);
            break;
        case 0x1A:
            CGameWork::setInt(0, 0x21, work | 0x100);
            break;
        case 0x1C:
            CGameWork::setInt(0, 0x21, work | 0x200);
            break;
        case 0x1E:
            CGameWork::setInt(0, 0x21, work | 0x400);
            break;
        case 0x1F:
            CGameWork::setInt(0, 0x21, work | 0x800);
            break;
        case 0x20:
            CGameWork::setInt(0, 0x21, work | 0x1000);
            break;
        case 0x23:
            CGameWork::setInt(0, 0x21, work | 0x2000);
            break;
        case 0x24:
            CGameWork::setInt(0, 0x21, work | 0x4000);
            break;
        case 0x25:
            CGameWork::setInt(0, 0x21, work | 0x8000);
            break;
        case 0x26:
            CGameWork::setInt(0, 0x21, work | 0x10000);
            break;
        case 0x27:
            CGameWork::setInt(0, 0x21, work | 0x20000);
            break;
        case 0x28:
            CGameWork::setInt(0, 0x21, work | 0x40000);
            break;
        case 0x29:
            CGameWork::setInt(0, 0x21, work | 0x80000);
            break;
        case 0x2B:
            CGameWork::setInt(0, 0x21, work | 0x100000);
            break;
        case 0x2C:
            CGameWork::setInt(0, 0x21, work | 0x200000);
            break;
        case 0x2D:
            CGameWork::setInt(0, 0x21, work | 0x400000);
            break;
        case 0x2E:
            CGameWork::setInt(0, 0x21, work | 0x800000);
            break;
        case 0x2F:
            CGameWork::setInt(0, 0x21, work | 0x1000000);
            break;
        case 0x30:
            CGameWork::setInt(0, 0x21, work | 0x2000000);
            break;
        case 0x13:
            CGameWork::setInt(0, 0x2D, work2 | 0x80);
            break;
        case 0x14:
            CGameWork::setInt(0, 0x2D, work2 | 0x100);
            break;
        case 0x15:
            CGameWork::setInt(0, 0x2D, work2 | 0x200);
            break;
        case 0x16:
            CGameWork::setInt(0, 0x2D, work2 | 0x400);
            break;
        case 0x17:
            CGameWork::setInt(0, 0x2D, work2 | 0x800);
            break;
        case 0x18:
            CGameWork::setInt(0, 0x2D, work2 | 0x1000);
            break;
        case 0x7D:
            CGameWork::setInt(0, 0x2D, work2 | 0x2000);
            break;
        case 0x7E:
            CGameWork::setInt(0, 0x2D, work2 | 0x4000);
            break;
        }
    } else {
        bLvUpDisp = _instance() != NULL;
        switch (inKind) {
        case 0xCB:
            addEXP(1, bLvUpDisp);
            break;
        case 0xCC:
            addEXP(10, bLvUpDisp);
            break;
        case 0xCD:
            addEXP(60, bLvUpDisp);
            break;
        case 0xCE:
            addEXP(5, bLvUpDisp);
            break;
        case 0xCF:
            addEXP(30, bLvUpDisp);
            break;
        case 0xD0:
            addEXP(99, bLvUpDisp);
            break;
        case 0xD1:
            addEXP(1400, bLvUpDisp);
            break;
        case 0xD2:
            if (!_instance()->m_bIsBattleScene) _instance()->addHP(_instance()->m_iMaxHP * 3 / 100, 0x1770, 0);
            else _instance()->addHP(3, 0x1770, 0);
            break;
        case 0xD3:
            if (!_instance()->m_bIsBattleScene) _instance()->addHP(_instance()->m_iMaxHP / 20, 0x2328, 0);
            else _instance()->addHP(10, 0x2328, 0);
            break;
        case 0xD4:
            addMP(4);
            break;
        case 0xD5:
            addMP(10);
            break;
        case 0xD6:
            pBEM = (CBattleEnemyManager*)CTaskMan::getSysTask(0x19);
            if (pBEM) pBEM->notifyPremiumPrizeGet();
            break;
        case 0xD7:
            pBEM = (CBattleEnemyManager*)CTaskMan::getSysTask(0x19);
            if (pBEM) pBEM->notifyRoulettePrizeGet();
            break;
        }
    }
}
static inline BOOL _isLevelMax() {
    BOOL ret = FALSE;
    if (_instance()) {
        if (_instance()->m_iLV >= 99) {
            _instance()->m_iLV = 99;
            ret = TRUE;
        }
    } else {
        ret = CStatus::getLV() >= 99;
    }
    return ret;
}
static inline void _addEXP(INT inEXP) {
    if (_instance()) _instance()->m_iEXP += inEXP;
    else CStatus::getStaus().EXP += inEXP;
}
static inline void _addLV(INT inLV) {
    if (_instance()) _instance()->m_iLV += inLV;
    else CStatus::getStaus().LV += inLV;
}
void CPlayer::addEXP(INT inEXP, INT inMode) {
    BOOL bLevelMax = _isLevelMax();
    BOOL levelup = FALSE;
    if (!bLevelMax) {
        _addEXP(inEXP);
        while (getNowNextEXP() <= 0) {
            _addLV(1);
            levelup = TRUE;
        }
    }
    if (levelup) {
        CGameWork::setFlag(1, 0x2D, TRUE);
        if (inMode && _instance()) {
            Gauge::CCommonGauge::setDispSeq(_instance()->getHandle(), 4);
            CSound::playSE(0x36, -1, 0x40);
            FVECTOR pos = {{0.0f, 0.85f, 0.0f, 1.0f}};
            _instance()->m_hEffLevelUp = m_pInstance->attachEffect(PL::g_szDpxAlways, 0x15, &_instance()->Crd(0), &pos, NULL, 0.0f);
            CEffect* effect = CEffectMan::getEffect(_instance()->m_hEffLevelUp);
            if (effect) effect->m_iFlag |= 4;
        }
    }
    Gauge::CPrizeGauge::setPrizeExp(inEXP);
}
void CPlayer::addHP(INT inHP, TIMEQ inDispEFF, INT inDispREC) {
    if (_instance()) {
        if (inDispEFF > 0) _instance()->setPrizeHealing(inDispEFF);
        if (inDispREC > 0) Gauge::CCommonGauge::setDispSeq(_instance()->getHandle(), 0);
        _instance()->m_iHP += inHP;
        CPlayer* p = _instance();
        if (p->m_iHP > p->m_iMaxHP) p->m_iHP = p->m_iMaxHP;
        if (_instance()->m_pGauge) _instance()->m_pGauge->setHP(m_iHP, 0);
    } else {
        STATUS_INFO& st = CStatus::getStaus();
        INT hp = st.HP + inHP;
        if (hp > st.HPMax) hp = st.HPMax;
        st.HP = hp;
    }
}
void CPlayer::addMP(INT inMP) {
    if (_instance()) {
        _instance()->m_iMP += inMP;
        if (_instance()->m_iMP > 99999) _instance()->m_iMP = 99999;
    } else {
        INT mp = CStatus::getMP() + inMP;
        if (mp > 99999) mp = 99999;
        CStatus::setHP(mp);
    }
    Gauge::CPrizeGauge::setPrizeMp(inMP);
}
void CPlayer::procDropMP() {
    INT dropMP;
    PRIZE_PARAM param;
    INT mp;
    dropMP = m_iMP / 10;
    if (dropMP > 500) dropMP = 500;
    else if (dropMP < 4 && m_iMP >= 4) dropMP = 4;
    param.vPos = m_vCenterPos;
    param.fSpeed = 0.2f;
    param.uiFlag = 0;
    param.uiPrizeFlag = 0;
    for (mp = dropMP; mp >= 4;) {
        if (mp >= 10) {
            param.PrizeKind = 0xD5;
            mp -= 10;
        } else if (mp >= 4) {
            param.PrizeKind = 0xD4;
            mp -= 4;
        } else {
            param.PrizeKind = 0;
        }
        if (param.PrizeKind) {
            param.vRot.set((-50.0f - RND::genrand_real1() * 10.0f) * 0.017453292f, RND::genrand_real1() * 360.0f * 0.017453292f, 0.0f);
            Math3D::LimitPIVector(param.vRot);
            CEntityFactory::getInstance()->createPrize(param, NULL);
        }
    }
    m_iMP -= dropMP - mp;
    Gauge::CPrizeGauge::setDropMp(dropMP - mp);
}
__asm__(".rdata\n");
void CPlayer::notifyExterminate() {
    if (!_instance()) return;
    if (_instance()->m_bIsDead) {
        TRACE("\x81\xa6\x91S\x96\xc5\x92\xca\x92m\x81" "E\x81" "E\x81" "E\x83v\x83\x8c\x83" "C\x83\x84\x81[\x8e\x80\x96S\x82\xcc\x88\xd7\x96\xb3\x8e\x8b\x82\xb5\x82\xdc\x82\xb7\x81I\n");
        return;
    }
    if (_instance()->m_PlayerState == PL::ESCAPE) return;
    PLM::calcFavoriteStockCount();
    _instance()->setBattleFlag(4, 0);
    _instance()->setConditionFlag(0x2000, 0);
}
INT CPlayer::notifyBossDead(INT inCmd) {
    INT rcid;
    if (!_instance()) return FALSE;
    if (_instance()->m_bIsDead) {
        TRACE("\x81\xa6\x83{\x83X\x8e\x80\x96S\x92\xca\x92m\x81" "E\x81" "E\x81" "E\x83v\x83\x8c\x83" "C\x83\x84\x81[\x8e\x80\x96S\x82\xcc\x88\xd7\x96\xb3\x8e\x8b\x82\xb5\x82\xdc\x82\xb7\x81I\n");
        return FALSE;
    }
    rcid = 0xFA0;
    if (CGame::isRikuGame()) rcid = _instance()->isConditionFlag(0x400000) ? 0xFA3 : 0xFA2;
    switch (inCmd) {
    case 0:
        PLM::calcFavoriteStockCount();
        _instance()->setBattleFlag(0x04000004, 0);
        _instance()->initEnd();
        CSceneEnv::setStop((ObjType::eObjBit)0x20002);
        _instance()->m_pCam->setFlag(0x20, 0);
        break;
    case 1:
        CSceneEnv::setStop((ObjType::eObjBit)0);
        CardBattle::CCardBattle::setBattleEnd();
        _instance()->m_pRevolver = NULL;
        break;
    case 2:
        _instance()->setConditionFlag(0x2000, 0);
        break;
    case 3:
        _instance()->setAnimRsrcEx(rcid, 3, -1, 0, 0, 0, 1);
        return TRUE;
    case 4:
        _instance()->setAnimRsrcEx(rcid, 1, 0, 0, 0, 0, 1);
        _instance()->offWeapon();
        return TRUE;
    case 5:
        _instance()->setAnimRsrcEx(rcid, 2, -1, 0, 0, 0, 1);
        break;
    case 6:
        _instance()->setAnimRsrcEx(rcid, 4, -1, 0, 0, 0, 1);
        break;
    }
    return TRUE;
}
HANDLE CPlayer::getLevelUpEffect() {
    if (!m_pInstance) return 0;
    return m_pInstance->m_hEffLevelUp;
}
void CPlayer::initEnd() {
    setPlayerState(PL::END_BTL);
    if (isBattleFlag(0x4000000)) {
        resetAction(9);
        m_iSubState = 1;
    } else {
        resetAction(8);
        m_iSubState = 2;
        m_pAnim->setRootMove(FALSE, FALSE);
        setConditionFlag(0x2000, 0);
        CardBattle::CCardBattle::setBattleEnd();
        if (m_pRevolver) {
            m_pRevolver->destroy();
            m_pRevolver = NULL;
        }
    }
    setPlayerFlag(0, 0x02800000);
    clearSpeed();
    m_bDmgMuteki = TRUE;
    m_fJumpGravity = *(FLOAT*)&PL::g_fGravity;
    m_fMoveSpdXZ = 0.0f;
    m_JumpState = 3;
    if (m_fJumpSpdY > m_fMoveSpdXZ) m_fJumpSpdY = m_fMoveSpdXZ;
    if (isBattleFlag(2)) {
        setBattleFlag(0, 2);
        m_tqEscape = 0;
        if (m_pEscape) m_pEscape->destroy();
    }
}
void CPlayer::exeEnd() {
    setBattleFlag(0, 8);
    switch (m_iSubState) {
    case 0:
        m_iSubState = 2;
        break;
    case 1:
        m_iSubState = 2;
        break;
    case 2:
        if (procFlowIdle(1)) {
            if (isConditionFlag(0x2000)) {
                onWeapon(0);
                setEntityFlag(0, 1);
                setDrawRadius(50.0f, 50.0f);
                if (isBattleFlag(0x4000000)) {
                    m_iSubState = 3;
                } else {
                    m_iSubState = 4;
                }
            }
        }
        break;
    case 3:
    case 4:
        break;
    }
}
BOOL CPlayer::isEnableBattleEnd() {
    if (!m_pInstance) return TRUE;
    if (m_pInstance->m_bIsDead) return FALSE;
    if (m_pInstance->m_PlayerState != 0x1C) return FALSE;
    return m_pInstance->m_iSubState >= 3;
}
BOOL CPlayer::isNowExterminate() {
    if (!m_pInstance) return TRUE;
    if (!m_pInstance->m_bIsDead) return FALSE;
    UINT m = 4;
    return (m_pInstance->m_uiBattleFlag & m) != 0;
}
void CPlayer::killBullet() {
    CBulletManager* p = (CBulletManager*)CTaskMan::getSysTask(0x1B);
    if (p) p->kill(getHandle());
}
void CPlayer::setEnemyStop(HANDLE inEnemy, UINT inSet) {
    SLContainerT<CTask>* container;
    CTask* task;
    CEntity* pEntity;
    CBattleEnemyManager* pBEM = (CBattleEnemyManager*)CTaskMan::getSysTask(0x19);
    if (!pBEM) return;
    container = pBEM->getFirstBattleEnemy();
    if (inSet) {
        for (; container; container = container->getNext()) {
            task = container->getValue();
            if (task && task->getTypeID() == 0x1004) {
                if (task->getHandle() != inEnemy) {
                    pEntity = (CEntity*)task;
                    pEntity->setEntityFlag(0x80000000, 0);
                }
            }
        }
        CSceneEnv::setStop((ObjType::eObjBit)8);
    } else {
        for (; container; container = container->getNext()) {
            task = container->getValue();
            if (task && task->getTypeID() == 0x1004) {
                pEntity = (CEntity*)task;
                pEntity->setEntityFlag(0, 0x80000000);
            }
        }
        CSceneEnv::setStop((ObjType::eObjBit)0);
    }
}
INT CPlayer::setMDFooting(HANDLE inFooting) {
    if (!inFooting) {
        setConditionFlag(0, 0x10000000);
        setTrgFlag(0, 0x400);
        m_fJumpLandLimit = -100.0f;
        m_hFooting = 0;
        m_pMove->setFlag(0, 0x200); // partial CMovement: (flag, on)
        return 0;
    }
    switch (m_PlayerState) {
    case PL::STAND:
    case PL::JUMP:
    case PL::FOOTING:
        break;
    default:
        return 0;
    }
    if (!isConditionFlag(0x10000000)) {
        setTrgFlag(0x400, 0);
        m_hFooting = inFooting;
        m_fJumpLandLimit = 2.0f;
        return 0;
    }
    m_pMove->setFlag(0x200, 0);
    return 1;
}
void CPlayer::initFooting(INT inFlag) {
    setPlayerState(PL::FOOTING);
    setTrgFlag(0, 0x400);
    if (inFlag == 1) {
        clearSpeed();
        setBattleFlag(0, 8);
        setBattleFlag(0, 0x80000);
        setPlayerFlag(0, 0x02800000);
        m_iAttackNo = 0;
        m_uiPreAttackKind = 0;
        m_bIsAttackHit = FALSE;
        m_bNextTrg = FALSE;
        m_iNextAttack = 0;
        m_bDmgMuteki = FALSE;
        m_bIsCanCombo = FALSE;
        m_fJumpGravity = *(FLOAT*)&PL::g_fGravity;
        m_JumpState = 3;
        m_fMoveSpdXZ = 0.0f;
    } else if (inFlag == 2) {
        setBattleFlag(0, 8);
        m_iSubState = inFlag;
        setPlayerFlag(1, 0);
        clearSpeed();
        if (m_fAnalogPower >= 0.3f) {
            m_fDir = m_fAnalogViewDir;
            FVECTOR rot = getRot();
            rot.y = m_fDir;
            setRot(rot);
        }
        updateCoord();
        if (CGame::isSoraGame()) setAnimRsrcEx(0xFAA, 0, -1, 0, 0, 0, 1);
        else if (!isConditionFlag(0x400000)) setAnimRsrcEx(0xFAC, 0, -1, 0, 0, 0, 1);
        else setAnimRsrcEx(0xFAD, 0, -1, 0, 0, 0, 1);
        m_fJumpSpdY = 0.0f;
        m_JumpState = 3;
        m_fJumpGravity = *(FLOAT*)&PL::g_fGravity;
    } else {
        m_iSubState = 1;
        m_WalkMode = chkWalkMode();
        setAnim(0x2A, -1, 0);
        m_fMovePower = 0.5f;
        m_fWalkSpeed = m_fWalkSpeed / 60.0f * m_fMovePower;
        offAttack();
        setPlayerFlag(0, 0x024C2000);
        m_JumpState = 0;
        m_fJumpSpdY = 0.0f;
        setConditionFlag(0, 0x08810023);
        setBattleFlag(0, 0x00443F90);
        setFaceAnim(0, 0);
        m_pMove->setFlag(0x40, 0);
    }
}
void CPlayer::exeFooting() {
    FVECTOR target;
    FVECTOR pos;
    FVECTOR v;
    FLOAT speed;
    FLOAT d0;
    FLOAT r;
    CEntity* pEntity = m_hFooting > 0 ? (CEntity*)CTaskMan::getTask(m_hFooting) : NULL;
    switch (m_iSubState) {
    case 0:
        setBattleFlag(0, 8);
        if (pEntity) {
            target = pEntity->getPos();
            pos = getPos();
            if (calcDistanceXZSq(target) < 0.010000001f) {
                pos.x = target.x;
                pos.z = target.z;
            } else {
                pos.x = Movement::Approach(pos.x, target.x, 0.1f, m_fFrameRate);
                pos.z = Movement::Approach(pos.z, target.z, 0.1f, m_fFrameRate);
            }
            setPos(pos);
        }
        if (procFlowIdle(1)) {
            m_iSubState = 1;
            setConditionFlag(0x10000000, 0);
        }
        return;
    case 2:
        setBattleFlag(0, 8);
        m_fWalkSpeed = 0.3f;
        m_fWalkSpeed = m_fJumpSpdXZ = Movement::Approach(m_fJumpSpdXZ, m_fWalkSpeed, 0.2f, m_fFrameRate);
        m_pMove->calcVelocityXZ(m_fWalkSpeed);
        if (isMotionEnd()) {
            setPlayerFlag(0, 1);
            initFooting(0);
        }
        return;
    case 3:
        return;
    case 1:
    default:
        break;
    }
    if (!m_hFooting) {
        setBattleFlag(0, 8);
        setConditionFlag(0, 0x10000000);
        setTrgFlag(0, 0x400);
        m_fJumpLandLimit = -100.0f;
        m_pMove->setFlag(0, 0x200);
        initIdle(-1, 0);
        return;
    }
    if (isTrgFlag(0x100000)) {
        TRACE("CPlayer:: exeFooting -> initTransform(0)\n");
        initTransform(0);
        return;
    }
    if (isTrgFlag(0x10)) {
        if (!chkChargeMove(0)) {
            initCharge();
            return;
        }
    }
    if (isBattleFlag(4)) {
        setMDFooting(0);
        return;
    }
    if (isTrgFlag(8)) {
        initItem(0);
    } else if (isTrgFlag(0x40)) {
        initGimmickCard(0);
    } else if (isTrgFlag(0x20)) {
        initMagic(0);
    } else if (isTrgFlag(0x80)) {
        initFriend(0);
    } else if (isTrgFlag(0x100)) {
        initSummon(0);
    } else if (isTrgFlag(4)) {
        initAttack(0);
    } else if (isTrgFlag(2)) {
        initAvoid();
    } else if (isTrgFlag(1)) {
        initJump(0);
    } else {
        speed = 0.0f;
        if (m_fAnalogPower >= 0.3f) {
            speed = 8.0f;
            d0 = m_fDir;
            r = m_fAnalogViewDir - d0;
            if (r < -3.1415927f) r += 6.2831855f;
            else if (r > 3.1415927f) r -= 6.2831855f;
            d0 = m_fDir;
            FLOAT nd = Movement::Homing(d0, m_fAnalogViewDir, 0.1f, m_fFrameRate);
            {
                FVECTOR t = { 0.0f, nd, 0.0f, 0.0f };
                setDir(nd);
                v = t;
            }
            m_pMove->setRot(v);
        }
        setAnim(0x2A, -1, 0);
        m_fWalkSpeed = speed / 60.0f;
        m_fWalkSpeed = m_fJumpSpdXZ = Movement::Approach(m_fJumpSpdXZ, m_fWalkSpeed, 0.1f, m_fFrameRate);
        m_pMove->calcVelocityXZ(m_fWalkSpeed);
    }
}
void CPlayer::updateFooting() {
    if (isConditionFlag(0x10000000)) {
        CEntity* pEntity = m_hFooting > 0 ? (CEntity*)CTaskMan::getTask(m_hFooting) : NULL;
        if (pEntity) {
            FVECTOR pos = Crd(0).getWorldMatrix().v[3];
            pos.y = 2.0f;
            pEntity->warp(pos);
            FVECTOR rot = getRot();
            pEntity->setRot(rot);
            pEntity->updateCoord();
            pEntity->coordFix();
        }
    }
}
void CPlayer::setCardSnatch(INT inFlag) {
    if (m_pRevolver) {
        m_hSnatch = inFlag;
        if (inFlag > 0) {
            m_uiBattleFlag = (INT)m_uiBattleFlag & ~8;
            m_pRevolver->setSnatch(inFlag);
        }
    }
}
void CPlayer::addWind(FVECTOR* inVel) {
    if (m_pPLM) m_pPLM->addWindXYZ(inVel);
}
void CPlayer::exeWind() {
    if (dbgCheckUser(KKIKUCHI) && DebugWork::g_dw.player.iTemp == 5) {
        if (!m_pPLM) return;
        m_pPLM->addWindXYZ(&DebugWork::g_dw.player.vTemp);
    }
    if (m_pPLM && m_pPLM->isEnableWind() && m_PlayerState != PL::GLIDE) {
        FVECTOR vel = getVel();
        FVECTOR wind = m_pPLM->getWind();
        AddVectorXYZ(vel, vel, wind);
        setVel(vel);
    }
}
BOOL CPlayer::cmdRevolver_rollDeck(INT inNum) {
    if (!m_pRevolver) return FALSE;
    m_pRevolver->rollDeck(inNum, 0, 300);
    return TRUE;
}
BOOL CPlayer::cmdRevolver_isEmpty() {
    if (!m_pRevolver) return TRUE;
    return m_pRevolver->getRemain(0, 0, 0) == 0;
}
INT CPlayer::cmdRevolver_isActive() {
    if (!m_pRevolver) return 0;
    return m_pRevolver->isActive();
}
void CPlayer::cmdMove_WaterSurfaceCheck(INT inFlag) {
    if (inFlag) m_pMove->setFlag(0x80, 0);
    else m_pMove->setFlag(0, 0x80);
}
__asm__(".rdata\n");
void CPlayer::cmdState_initWorldSelect(HANDLE inDoor) {
    m_hGimmick = inDoor;
    m_pMarker->holdTarget(8);
    initWorldSelect();
    new (__FILE__ "(13487)") CWorldSelect(CTaskMan::getSysTask(1), m_hGimmick);
}
void CPlayer::cmdState_setEncountWait() {
    setPlayerFlag(0x100, 0);
    changeState(2);
    setPlayerFlag(0x1001, 0);
    m_collBody.off();
    m_collBody.offBody();
}
void CPlayer::beginStateField_() {
    beginStateField();
}
void CPlayer::stateField_() {
    stateField();
}
void CPlayer::endStateField_() {
    endStateField();
}
void CPlayer::beginStateBattle_() {
    beginStateBattle();
}
void CPlayer::stateBattle_() {
    stateBattle();
}
void CPlayer::endStateBattle_() {
    endStateBattle();
}
void CPlayer::beginStateWait_() {
    beginStateWait();
}
void CPlayer::stateWait_() {
    stateWait();
}
void CPlayer::endStateWait_() {
    endStateWait();
}
void CPlayer::beginStateHide_() {
    beginStateHide();
}
void CPlayer::stateHide_() {
    stateHide();
}
void CPlayer::endStateHide_() {
    endStateHide();
}
/* the debug window / state name tables (after the last function with strings, before CPlayerDebug) */
__asm__(".rdata\n");
char* szPlayerState[38] = {
    "INIT",
    "START WAIT",
    "STAND",
    "WALK",
    "RUN",
    "WALL",
    "AVOID",
    "JUMP",
    "GLIDE",
    "HANG",
    "ATTACK/FLD",
    "ATTACK",
    "DAMAGE",
    "FLICKS",
    "ITEM",
    "HIDE",
    "LADDER",
    "TREASURE",
    "CHARGE",
    "MAGIC",
    "ROOMCREAT",
    "WORLDSLCT",
    "SKILL",
    "ESCAPE",
    "BADCONDIT",
    "GIMMICK",
    "FRIEND",
    "SUMMON",
    "END/BTL",
    "DEAD",
    "CARRY",
    "TALK",
    "TRANSFORM",
    "JUMP_2ND",
    "DUEL",
    "FOOTING",
    "PRONOUNCE",
    "DBG-WAIT"
};
char* szPlayerJumpState[4] = {
    "JMP_STATE_NONE",
    "JMP_STATE_RISE",
    "JMP_STATE_FALL",
    "JMP_STATE_LAND"
};
char* szPlayerLadderState[6] = {
    "SST_LADDER_START",
    "SST_LADDER",
    "SST_LADDER_UP",
    "SST_LADDER_DOWN",
    "SST_LADDER_CLIMB",
    "SST_LADDER_END"
};
char* szPlayerDamageState[8] = {
    "SST_DAMAGE_NONE",
    "SST_DAMAGE_FLICKS",
    "SST_DAMAGE_BREAK",
    "SST_DAMAGE_NORMAL",
    "SST_DAMAGE_AERIAL",
    "SST_DAMAGE_BLOWN",
    "SST_DAMAGE_BANANA",
    "SST_DAMAGE_VOLLEY"
};
char* szPlayerEndState[5] = {
    "SST_END_START",
    "SST_END_STOP",
    "SST_END_WAIT",
    "SST_END_WIN",
    "SST_END_END"
};
char* szAttackData[188] = {
    "0:NONE",
    "1:VERTICAL",
    "2:HORIZONTAL",
    "3:STAB",
    "4:TURN",
    "5:RISE",
    "6:AIR_VERTICAL",
    "7:AIR_TURN",
    "8:SLIDEDASH",
    "9:FINALBREAK",
    "10:FINALBREAK_FINISH",
    "11:STUNIMPACT",
    "12:ZANTETSUKEN",
    "13:STRIKERAID",
    "14:SONICRAVE_START",
    "15:SONICRAVE",
    "16:SONICRAVE_FINISH",
    "17:LASTARCANUM",
    "18:LASTARCANUM_1",
    "19:LASTARCANUM_2",
    "20:LASTARCANUM_3",
    "21:LASTARCANUM_4",
    "22:LASTARCANUM_5",
    "23:LASTARCANUM_6",
    "24:LASTARCANUM_7",
    "25:LASTARCANUM_8",
    "26:LASTARCANUM_9",
    "27:LASTARCANUM_10",
    "28:LASTARCANUM_11",
    "29:LASTARCANUM_12",
    "30:LASTARCANUM_FINISH",
    "31:RAGNAROCK",
    "32:RAGNAROCK_LASER",
    "33:TRINITYLIMIT",
    "34:FIRE",
    "35:BLIZZARD",
    "36:THUNDER",
    "37:CURE",
    "38:GRAVITY",
    "39:STOP",
    "40:AERO",
    "41:FIRA",
    "42:FIRAGA",
    "43:BLIZZARA",
    "44:BLIZZAGA",
    "45:HOMINGBLIZZARA",
    "46:THUNDRA",
    "47:THUNDAGA",
    "48:FIRERAID",
    "49:BLIZZARDRAID",
    "50:THUNDERRAID",
    "51:JUDGEMENT",
    "52:REFRECTRAID",
    "53:REFRECTRAID_SUB",
    "54:CURA",
    "55:CURAGA",
    "56:GRAVIRA",
    "57:GRAVIGA",
    "58:STOPRA",
    "59:STOPGA",
    "60:AERA",
    "61:AEROGA",
    "62:DEGENERATOR",
    "63:DEGENERATE",
    "64:BIND",
    "65:CONFUSE",
    "66:TERROR",
    "67:SYNCHRO",
    "68:TELEPORT",
    "69:FIRAGABURST",
    "70:FIRAGABURST_FLAME",
    "71:RAGINGSTORM",
    "72:MEGAFLARE",
    "73:FREEZE",
    "74:AQUASPLASH",
    "75:MAGNESPIRAL",
    "76:MAGNESPIRAL_ATK",
    "77:LETHALFLAME",
    "78:LETHALFLAME_ATK",
    "79:LETHALFLAME_FIN",
    "80:SHOCKIMPACT",
    "81:TORNADO",
    "82:QUAKE",
    "83:MIRACLEGIFT",
    "84:HOLY",
    "85:DONALD",
    "86:GOOFY",
    "87:ALADDIN",
    "88:ARIEL",
    "89:JACK",
    "90:PAN",
    "91:BEAST",
    "92:KING",
    "93:PLUTO",
    "94:DONALD_1",
    "95:DONALD_2",
    "96:DONALD_3",
    "97:DONALD_3_ATK1",
    "98:DONALD_3_ATK2",
    "99:DONALD_3_ATK3",
    "100:GOOFY_1",
    "101:GOOFY_2",
    "102:GOOFY_3",
    "103:GOOFY_3_ATK",
    "104:FRIEND_1",
    "105:ALADDIN_1",
    "106:ALADDIN_2",
    "107:JACK_1",
    "108:JACK_2",
    "109:ARIEL_1",
    "110:ARIEL_2",
    "111:PAN_1",
    "112:PAN_2",
    "113:BEAST_1",
    "114:BEAST_2",
    "115:BEAST_ATK",
    "116:KING_1",
    "117:KING_2",
    "118:KING_3",
    "119:KING_3D",
    "120:PLUTO_1",
    "121:PLUTO_1",
    "122:SINBA",
    "123:GENIE",
    "124:BAMBI",
    "125:DUMBO",
    "126:TINK",
    "127:MUSHU",
    "128:CLOUD",
    "129:SINBA_1",
    "130:SINBA_2",
    "131:DUMBO_1",
    "132:DUMBO_2",
    "133:BAMBI_1",
    "134:BAMBI_2",
    "135:BAMBI_3",
    "136:MUSHU_1",
    "137:MUSHU_2",
    "138:GENIE_1",
    "139:GENIE_2",
    "140:GENIE_THUNDRA",
    "141:GENIE_GRAVIRA",
    "142:GENIE_STOPRA",
    "143:TINK_1",
    "144:TINK_2",
    "145:CLOUD_1",
    "146:CLOUD_2",
    "147:CLOUD_3",
    "148:MG_BLAST",
    "149:MG_HONEY",
    "150:MG_TORNADO",
    "151:RK_VERTICAL",
    "152:RK_HORIZONTAL",
    "153:RK_STAB",
    "154:RK_TURN",
    "155:RK_RISE",
    "156:RK_AIR_VERTICAL",
    "157:RK_AIR_TURN",
    "158:DRK_VERTICAL",
    "159:DRK_HORIZONTAL",
    "160:DRK_STAB",
    "161:DRK_TURN",
    "162:DRK_RISE",
    "163:DRK_AIR_VERTICAL",
    "164:DRK_AIR_TURN",
    "165:DRK_2ND_JUMP",
    "166:DRK_DARKBREAK",
    "167:DRK_DARKBREAK_FIN",
    "168:DRK_DARKFIRAGA",
    "169:DRK_DARKAURA",
    "170:DRK_DARKAURA_FIN",
    "171:DRK_DARKAURA_MAG",
    "172:DS_DASH",
    "173:DS_RUSH(BARRAGE)",
    "174:DS_IMPULSE",
    "175:DS_IMPULSE_FIN",
    "176:DS_MAELSTROM",
    "177:DS_MAELSTROM_FIN",
    "178:DS_BARRAGE",
    "179:DS_BARRAGE_2",
    "180:DS_BARRAGE_FIN",
    "181:DS_D_IMPULSE",
    "182:DS_D_IMPULSE_FIN",
    "183:DS_D_MAELSTROM",
    "184:DS_D_MAELSTROM_FIN",
    "185:DS_D_BARRAGE",
    "186:DS_D_BARRAGE_2",
    "187:DS_D_BARRAGE_FIN"
};
char* szPlayerSkill[110] = {
    "OFF",
    "1:OFF",
    "2:OFF",
    "3:OFF",
    "\x83X\x83\x89\x83" "C\x83h\x83_\x83" "b\x83V\x83\x85",
    "\x83t\x83@\x83" "C\x83i\x83\x8b\x83u\x83\x8c\x83" "C\x83N",
    "\x83X\x83^\x83\x93\x83" "C\x83\x93\x83p\x83N\x83g",
    "\x83U\x83\x93\x83" "e\x83" "c\x83P\x83\x93",
    "\x83X\x83g\x83\x89\x83" "C\x83N\x83\x8c\x83" "C\x83h",
    "\x83\\\x83j\x83" "b\x83N\x83\x8c\x83" "C\x83\x94",
    "\x83\x89\x83X\x83g\x83" "A\x83\x8b\x83J\x83i\x83\x80",
    "\x83\x89\x83O\x83i\x83\x8d\x83N",
    "\x83g\x83\x8a\x83j\x83" "e\x83" "B\x83\x8a\x83~\x83" "b\x83g",
    "\x83t\x83@\x83" "C\x83\x89",
    "\x83u\x83\x8a\x83U\x83\x89",
    "\x83T\x83\x93\x83_\x83\x89",
    "\x83P\x83" "A\x83\x8b\x83\x89",
    "\x83O\x83\x89\x83r\x83\x89",
    "\x83X\x83g\x83v\x83\x89",
    "\x83G\x83" "A\x83\x8d\x83\x89",
    "\x83t\x83@\x83" "C\x83K",
    "\x83u\x83\x8a\x83U\x83K",
    "\x83T\x83\x93\x83_\x83K",
    "\x83P\x83" "A\x83\x8b\x83K",
    "\x83O\x83\x89\x83r\x83K",
    "\x83X\x83g\x83v\x83K",
    "\x83G\x83" "A\x83\x8d\x83K",
    "\x83t\x83@\x83" "C\x83" "A\x83\x8c\x83" "C\x83h",
    "\x83u\x83\x8a\x83U\x83h\x83\x8c\x83" "C\x83h",
    "\x83T\x83\x93\x83_\x81[\x83\x8c\x83" "C\x83h",
    "\x83\x8a\x83t\x83\x8c\x83N\x83g\x83\x8c\x83" "C\x83h",
    "\x83W\x83\x83\x83" "b\x83W\x83\x81\x83\x93\x83g",
    "\x83t\x83@\x83" "C\x83K\x83o\x81[\x83X\x83g",
    "\x83\x8c\x83" "C\x83W\x83\x93\x83O\x83X\x83g\x81[\x83\x80",
    "\x83\x81\x83K\x83t\x83\x8c\x83" "A",
    "\x83t\x83\x8a\x81[\x83Y",
    "\x83z\x81[\x83~\x83\x93\x83O\x83u\x83\x8a\x83U\x83\x89",
    "\x83" "A\x83N\x83" "A\x83X\x83v\x83\x89\x83" "b\x83V\x83\x85",
    "\x83}\x83O\x83l\x83X\x83p\x83" "C\x83\x89\x83\x8b",
    "\x83\x8a\x81[\x83T\x83\x8b\x83t\x83\x8c\x81[\x83\x80",
    "\x83V\x83\x87\x83" "b\x83N\x83" "C\x83\x93\x83p\x83N\x83g",
    "\x83g\x83\x8b\x83l\x83h",
    "\x83N\x83G\x83" "C\x83N",
    "\x83" "f\x83W\x83\x87\x83l\x81[\x83^\x81[",
    "\x83" "f\x83W\x83\x87\x83\x93",
    "\x83o\x83" "C\x83\x93\x83h",
    "\x83R\x83\x93\x83t\x83\x85",
    "\x83" "e\x83\x89\x81[",
    "\x83V\x83\x93\x83N\x83\x8d",
    "\x83~\x83\x89\x83N\x83\x8b\x83M\x83t\x83g",
    "\x83" "e\x83\x8c\x83|",
    "\x83z\x81[\x83\x8a\x81[",
    "\x83v\x83\x89\x83" "E\x83h\x83\x8d\x83" "A Lv2",
    "\x83v\x83\x89\x83" "E\x83h\x83\x8d\x83" "A Lv3",
    "\x83X\x83v\x83\x89\x83" "b\x83V\x83\x85 Lv2",
    "\x83X\x83v\x83\x89\x83" "b\x83V\x83\x85 Lv3",
    "\x83p\x83\x89\x83_\x83" "C\x83X Lv2",
    "\x83p\x83\x89\x83_\x83" "C\x83X Lv3",
    "\x83p\x83\x89\x83_\x83" "C\x83X\x83X\x83^\x83\x93\x83v",
    "\x83t\x83\x8c\x83" "A\x83u\x83\x8c\x83X Lv2",
    "\x83t\x83\x8c\x83" "A\x83u\x83\x8c\x83X Lv3",
    "\x83V\x83\x87\x81[\x83^\x83" "C\x83\x80 Lv2",
    "\x83V\x83\x87\x81[\x83^\x83" "C\x83\x80 Lv3",
    "\x83" "c\x83" "C\x83\x93\x83N\x83\x8b Lv2",
    "\x83" "c\x83" "C\x83\x93\x83N\x83\x8b Lv3",
    "\x8b\xa5\x8e" "a\x82\xe8 Lv2",
    "\x92\xb4\x8b\x86\x95\x90\x90_\x94" "e\x8e" "a",
    "\x8b\xa5\x8e" "a\x82\xe8\x81" "E\x89\xfc",
    "\x83}\x83W\x83" "b\x83N Lv2",
    "\x83}\x83W\x83" "b\x83N Lv3",
    "\x83X\x83^\x81[\x83_\x83X\x83g\x83u...",
    "\x83O\x81[\x83t\x83" "B\x83g\x83\x8b...Lv2",
    "\x83O\x81[\x83t\x83" "B\x83g\x83\x8b...Lv3",
    "\x83O\x81[\x83t\x83" "B\x83X\x83}\x83" "b\x83V\x83\x85",
    "\x83\x8f\x83" "C\x83\x8b\x83h\x83N\x83\x89\x83" "b\x83V\x83\x83",
    "\x83X\x83g\x81[\x83\x80\x83\x8a\x81[...Lv2",
    "\x83X\x83g\x81[\x83\x80\x83\x8a\x81[...Lv3",
    "\x83T\x83v\x83\x89\x83" "C\x83YLv2",
    "\x83T\x83v\x83\x89\x83" "C\x83YLv3",
    "\x83X\x83p\x83" "C\x83\x89\x83\x8b\x83" "E...Lv2",
    "\x83X\x83p\x83" "C\x83\x89\x83\x8b\x83" "E...Lv3",
    "\x83X\x83\x89\x83X\x83g\x83X\x83\x89...Lv2",
    "\x83X\x83\x89\x83X\x83g\x83X\x83\x89...Lv3",
    "\x83t\x83\x85\x83\x8a\x83" "A\x83X\x83{...Lv2",
    "\x83t\x83\x85\x83\x8a\x83" "A\x83X\x83{...Lv3",
    "\x83\x89\x83" "b\x83L\x81[\x83o\x83" "E...Lv2",
    "\x83\x89\x83" "b\x83L\x81[\x83o\x83" "E...Lv3",
    "MG:\x83n\x83j\x81[\x83X\x83g\x81[\x83\x80",
    "MG:\x83n\x83j\x81[\x83|\x83" "b\x83gLv2",
    "MG:\x83n\x83j\x81[\x83|\x83" "b\x83gLv3",
    "90:NG -ACTION-",
    "91:NG -ACTION-",
    "92:NG -ACTION-",
    "93:NG -DUMMY-",
    "94:NG -DUMMY-",
    "\x83_\x81[\x83N\x83u\x83\x8c\x83" "C\x83N",
    "\x83_\x81[\x83N\x83t\x83@\x83" "C\x83K",
    "\x83_\x81[\x83N\x83I\x81[\x83\x89",
    "\x82l\x82l\x83~\x83\x89\x83N\x83\x8b Lv2",
    "\x82l\x82l\x83~\x83\x89\x83N\x83\x8b Lv3",
    "\x83z\x81[\x83\x8a\x81[\x83o\x81[\x83X\x83g",
    "\x83" "C\x83\x93\x83\x94\x81[\x83X\x83@\x83o\x81[\x83X\x83g",
    "DS:\x83" "C\x83\x93\x83p\x83\x8b\x83X",
    "DS:\x83\x81\x83" "C\x83\x8b\x83X\x83g\x83\x8d\x83\x80",
    "DS:\x83o\x83\x89\x81[\x83W\x83\x85",
    "DS:\x82" "c\x83" "C\x83\x93\x83p\x83\x8b\x83X",
    "DS:\x82" "c\x83\x81\x83" "C\x83\x8b\x83X\x83g\x83\x8d\x83\x80",
    "DS:\x82" "c\x83o\x83\x89\x81[\x83W\x83\x85",
    "108:NG -DUMMY-",
    "109:NG -ACTION-"
};
char* szPlayerCard[67] = {
    "OFF",
    "\x83L\x83\x93\x83O\x83_\x83\x80\x83`\x83" "F\x81[\x83\x93",
    "\x83" "f\x83U\x83" "C\x83" "A\x81[\x83\x89\x83\x93\x83v",
    "\x83g\x83\x8c\x83W\x83\x83\x81[\x83I\x83u\x83V\x81[",
    "\x83p\x83\x93\x83v\x83L\x83\x93\x83w\x83" "b\x83h",
    "\x83t\x83" "F\x83" "A\x83\x8a\x81[\x83n\x81[\x83v",
    "\x83" "E\x83" "B\x83" "b\x83V\x83\x85\x83X\x83^\x81[",
    "\x83G\x83O\x83U\x83~\x83l\x81[\x83V\x83\x87\x83\x93",
    "\x83\x81\x83^\x83\x8b\x83`\x83\x87\x83R\x83{",
    "\x83p\x83\x8f\x81[\x83I\x83u\x83q\x81[\x83\x8d\x81[",
    "\x83\x89\x83" "C\x83I\x83\x93\x83n\x81[\x83g",
    "\x83\x89\x83X\x83g\x83\x8a\x83]\x81[\x83g",
    "\x83\x89\x83\x94\x83" "B\x83" "A\x83\x93\x83\x8d\x81[\x83Y",
    "\x96\xf1\x91\xa9\x82\xcc\x82\xa8\x8e\xe7\x82\xe8",
    "\x89\xdf\x82\xac\x8b\x8e\x82\xe8\x82\xb5\x8ev\x82\xa2\x8fo",
    "\x83" "A\x83\x8b\x83" "e\x83}\x83" "E\x83" "F\x83|\x83\x93",
    "\x83_\x83" "C\x83\x84\x83\x82\x83\x93\x83h\x83_\x83X\x83g",
    "\x95\xd0\x97\x83\x82\xcc\x93V\x8eg",
    "\x83\\\x83" "E\x83\x8b\x83" "C\x81[\x83^\x81[",
    "\x83X\x83^\x81[\x83V\x81[\x83J\x81[",
    "\x83\x82\x83m\x83N\x83\x8d\x81[\x83\x80",
    "\x83t\x83H\x83\x8d\x81[\x83U\x83" "E\x83" "B\x83\x93\x83h",
    "\x83q\x83h\x83" "D\x83\x93\x83h\x83\x89\x83S\x83\x93",
    "\x83t\x83H\x83g\x83\x93\x83" "f\x83o\x83" "b\x83K\x81[",
    "\x83{\x83\x93\x83h\x83I\x83u\x83t\x83\x8c\x83" "C\x83\x80",
    "\x83n\x83`\x91\xde\x8e\xa1\x95\x90\x8a\xed",
    "\x83t\x83@\x83" "C\x83" "A",
    "\x83u\x83\x8a\x83U\x83h",
    "\x83T\x83\x93\x83_\x81[",
    "\x83P\x83" "A\x83\x8b",
    "\x83O\x83\x89\x83r\x83" "f",
    "\x83X\x83g\x83" "b\x83v",
    "\x83G\x83" "A\x83\x8d",
    "\x83n\x83`\x91\xde\x8e\xa1\x93\xcb\x95\x97",
    "\x83n\x83`\x91\xde\x8e\xa1\x83n\x83`\x83~\x83" "c",
    "\x83V\x83\x93\x83o",
    "\x83W\x81[\x83j\x81[",
    "\x83o\x83\x93\x83r",
    "\x83_\x83\x93\x83{",
    "\x83" "e\x83" "B\x83\x93\x83N",
    "\x83\x80\x81[\x83V\x83\x85\x81[",
    "\x83N\x83\x89\x83" "E\x83h",
    "\x83|\x81[\x83V\x83\x87\x83\x93",
    "\x83n\x83" "C\x83|\x81[\x83V\x83\x87\x83\x93",
    "\x83\x81\x83K\x83|\x81[\x83V\x83\x87\x83\x93",
    "\x83G\x81[\x83" "e\x83\x8b",
    "\x83\x81\x83K\x83G\x81[\x83" "e\x83\x8b",
    "\x83G\x83\x8a\x83N\x83T\x81[",
    "\x83\x89\x83X\x83g\x83G\x83\x8a\x83N\x83T\x81[",
    "\x83h\x83i\x83\x8b\x83h",
    "\x83O\x81[\x83t\x83" "B\x81[",
    "\x83" "A\x83\x89\x83W\x83\x93",
    "\x83" "A\x83\x8a\x83G\x83\x8b",
    "\x83W\x83\x83\x83" "b\x83N",
    "\x83s\x81[\x83^\x81[\x83p\x83\x93",
    "\x83r\x81[\x83X\x83g",
    "\x89\xa4\x97l",
    "\x83v\x83\x8b\x81[\x83g",
    "G:\x83K\x81[\x83h\x83" "A\x81[\x83}\x81[",
    "G:\x83g\x83\x8a\x83" "b\x83N\x83X\x83^\x81[",
    "G:\x83W\x83\x83\x83t\x83@\x81[",
    "G:\x83u\x83M\x81[",
    "G:\x83p\x83\x89\x83T\x83" "C\x83g\x83P\x83" "C\x83W",
    "G:\x83" "A\x81[\x83X\x83\x89",
    "G:\x83}\x83\x8c\x83t\x83" "B\x83Z\x83\x93\x83g...",
    "G:\x83_\x81[\x83N\x83T\x83" "C\x83h",
    "G:ZexionTrapA"
};
extern "C" int sprintf(char*, const char*, ...); extern "C" unsigned int strlen(const char*); extern "C" char* strncpy(char*, const char*, unsigned int);
CPlayerDebug::CPlayerDebug(CTask* pParent) : CTask(pParent, 0x4000) {
    INT i;
    m_pWin = NULL;
    sprintf(m_szBuf[0], " ");
    sprintf(m_szBuf[1], " ");
    sprintf(m_szBuf[2], " ");
    sprintf(m_szBuf[3], " ");
    sprintf(m_szBuf[4], " ");
    sprintf(m_szBuf[5], " ");
    DebugWork::g_dw.player.m_szState[0] = m_szBuf[0];
    DebugWork::g_dw.player.m_szState[1] = m_szBuf[1];
    DebugWork::g_dw.player.m_szState[2] = m_szBuf[2];
    DebugWork::g_dw.player.m_szState[3] = m_szBuf[3];
    DebugWork::g_dw.player.m_szAnim[0] = m_szBuf[4];
    DebugWork::g_dw.player.m_szAnim[1] = m_szBuf[5];
    m_iPrevState = 0;
    m_iPrevMuteki = 0;
    m_iPrevHP = 0;
#line 14052
    m_pWin = new (__FILE__ "(" "14052" ")") CDebugWindow(this, DWTPlayer::debugPlayerInfo);
    m_pWin->dispOff(0);
    for (i = 0; i < 71; i++) szAtkAnimID[i] = PL_ANIM::g_MotNameTbl[PLM::g_uiPlayerID][i].pFileName;
    m_pCamData = 0;
    m_pAtkData = 0;
}
CPlayerDebug::~CPlayerDebug() {}
void CPlayerDebug::main() {
    if (DebugWork::g_dw.player.iPlayerInfo) {
        if (g_pPlayer) {
            DebugWork::g_dw.player.vPos = g_pPlayer->getPos();
            DebugWork::g_dw.player.vPos.w = g_pPlayer->m_fDir * 180.0f / 3.1415927f;
            INT st = g_pPlayer->m_PlayerState;
            DebugWork::g_dw.player.iPlayerState = st;
            switch (g_pPlayer->m_PlayerState) {
            case 2:
            case 0x20:
                if (g_pPlayer->getAnim()->getAnimID() == 0x2A) {
                    sprintf(m_szBuf[0], "\xf9WFlagPLY: % 08X", g_pPlayer->m_uiPlayerFlag);
                    sprintf(m_szBuf[1], "\xf9WFlagCON: % 08X", g_pPlayer->m_uiConditionFlag);
                    sprintf(m_szBuf[2], "\xf9WFlagBTL: % 08X", g_pPlayer->m_uiBattleFlag);
                } else if (CGame::isSoraGame()) {
                    sprintf(m_szBuf[0], "\xf9WLEVEL: % 10d", g_pPlayer->m_iLV);
                    sprintf(m_szBuf[1], "\xf9WEXP: % 12d", g_pPlayer->m_iEXP);
                    { const char* f = "\xf9WNEXT: % 5d/% 5d"; INT n = CPlayer::getNowNextEXP(); INT e = g_pPlayer->m_iEXP; sprintf(m_szBuf[2], f, n, e + CPlayer::getNowNextEXP()); }
                } else {
                    sprintf(m_szBuf[0], "\xf9WHP: % 3d/% 3d", g_pPlayer->m_iHP, g_pPlayer->m_iMaxHP);
                    sprintf(m_szBuf[1], "\xf9WAP: % 6d", g_pPlayer->m_iAP);
                    sprintf(m_szBuf[2], "\xf9WDP: % 3d/% 3d", g_pPlayer->m_iDP, g_pPlayer->m_iMaxDP);
                }
                break;
            case 0xF:
                if (m_iPrevState != 2) {
                    if (strlen(m_szBuf[0]) > 2) strncpy(m_szBuf[0], "\xf9S", 2);
                    if (strlen(m_szBuf[1]) > 2) strncpy(m_szBuf[1], "\xf9S", 2);
                    if (strlen(m_szBuf[2]) > 2) strncpy(m_szBuf[2], "\xf9S", 2);
                }
                break;
            case 3:
            case 4:
                sprintf(m_szBuf[0], "\xf9W\x95" "b\x91\xac:   % 7.4fm/s", g_pPlayer->m_fWalkSpeed * 60.0f);
                sprintf(m_szBuf[1], "\xf9W\x93`\x93\xb1\x97\xa6: % 7.2f\x81\x93", g_pPlayer->m_fMovePower * 100.0f);
                sprintf(m_szBuf[2], " ");
                break;
            case 7:
            case 0x21:
                sprintf(m_szBuf[0], "\xf9W%s", szPlayerJumpState[g_pPlayer->getJumpState()]);
                sprintf(m_szBuf[1], "\xf9WSpdY:  % 8.4fm/s", g_pPlayer->m_fJumpSpdY * 60.0f);
                sprintf(m_szBuf[2], "\xf9W\x92\xb8\x93_:  % 8.4fm", g_pPlayer->m_fJumpTop);
                break;
            case 9:
                if (g_pPlayer->m_iSubState == 2) {
                    sprintf(m_szBuf[0], "\xf9W\x8d\x98x:    % 9.4f", g_pPlayer->m_vCenterPos.x);
                    sprintf(m_szBuf[1], "\xf9W\x8d\x98y:    % 9.4f", g_pPlayer->m_vCenterPos.y);
                    sprintf(m_szBuf[2], "\xf9W\x8d\x98z:    % 9.4f", g_pPlayer->m_vCenterPos.z);
                } else {
                    sprintf(m_szBuf[0], "\xf9W\x8d\xb6\x8e\xe8x:  % 9.4f", g_pPlayer->m_vHangHandPos.x);
                    sprintf(m_szBuf[1], "\xf9W\x8d\xb6\x8e\xe8y:  % 9.4f", g_pPlayer->m_vHangHandPos.y);
                    sprintf(m_szBuf[2], "\xf9W\x8d\xb6\x8e\xe8z:  % 9.4f", g_pPlayer->m_vHangHandPos.z);
                }
                break;
            case 0x10:
                sprintf(m_szBuf[0], "\xf9W%s", szPlayerLadderState[g_pPlayer->m_iSubState]);
                sprintf(m_szBuf[1], "\xf9WStep:     % 3d/% 3d", g_pPlayer->m_iLadderStep, g_pPlayer->m_iLadderStepMax);
                if (g_pPlayer->m_iSubState == 4) sprintf(m_szBuf[2], "\xf9W\x8d\x98yz:% 4.3f,% 4.3f", g_pPlayer->m_vHangHandPos.y, g_pPlayer->m_vHangHandPos.z);
                else sprintf(m_szBuf[2], " ");
                break;
            case 6:
            case 0xA:
            case 0xB:
            case 0xC:
            case 0xD:
            case 0xE:
            case 0x13:
            case 0x16:
            case 0x18:
            case 0x19:
            case 0x1A:
            case 0x1B:
            case 0x1D:
            case 0x22:
            case 0x23:
            case 0x24:
                sprintf(m_szBuf[0], "\xf9WFlagPLY: % 08X", g_pPlayer->m_uiPlayerFlag);
                sprintf(m_szBuf[1], "\xf9WFlagCON: % 08X", g_pPlayer->m_uiConditionFlag);
                sprintf(m_szBuf[2], "\xf9WFlagBTL: % 08X", g_pPlayer->m_uiBattleFlag);
                break;
            case 0x1C:
                sprintf(m_szBuf[0], "\xf9W%s", szPlayerEndState[g_pPlayer->m_iSubState]);
                sprintf(m_szBuf[1], "\xf9WFlagPLY: % 08X", g_pPlayer->m_uiPlayerFlag);
                sprintf(m_szBuf[2], "\xf9WFlagBTL: % 08X", g_pPlayer->m_uiBattleFlag);
                break;
            }
            sprintf(m_szBuf[4], "File: % 11s", g_pPlayer->getMotionName(g_pPlayer->getAnim()->getAnimID()));
            sprintf(m_szBuf[5], "TimeQ:% 5d/% 5d", g_pPlayer->getMotionCurrentTime(), g_pPlayer->getMotionTotalTime());
            m_iPrevState = g_pPlayer->m_PlayerState;
        }
        m_pWin->dispOn(0);
    } else {
        m_pWin->dispOff(0);
    }
    if (g_pPlayer) {
        if (DebugWork::g_dw.player.iMuteki) g_pPlayer->setPlayerFlag(1, 0);
        else if (m_iPrevMuteki) g_pPlayer->setPlayerFlag(0, 1);
        m_iPrevMuteki = DebugWork::g_dw.player.iMuteki;
        if (DebugWork::g_dw.player.iDbgWindow) {
            if (DebugWork::g_dw.player.iHP < 0) {
                DebugWork::g_dw.player.iHP = 0;
                m_iPrevHP = 1;
            }
            INT maxHp = g_pPlayer->m_iMaxHP;
            if (maxHp < DebugWork::g_dw.player.iHP) {
                DebugWork::g_dw.player.iHP = maxHp;
                m_iPrevHP = (SHORT)maxHp + 1;
            }
            if (m_iPrevHP != DebugWork::g_dw.player.iHP) {
                m_iPrevHP = DebugWork::g_dw.player.iHP;
                (*(CPlayer* volatile*)&g_pPlayer)->m_iHP = m_iPrevHP;
                (*(CPlayer* volatile*)&g_pPlayer)->m_pGauge->setHP(m_iPrevHP, 1);
            }
            if (m_iPrevHP != g_pPlayer->m_iHP) {
                m_iPrevHP = g_pPlayer->m_iHP;
                DebugWork::g_dw.player.iHP = m_iPrevHP;
            }
        }
    }
    m_pCamData = (PCAM_DATA*)DebugWork::g_dw.player.pCamData;
    if (m_pCamData) {
        if (m_pCamData->fViewAngle != DebugWork::g_dw.player.vCamEye.w) {
            if (PlayerCamera::CPlayerCamera::getNowCamMode() == DebugWork::g_dw.player.iCamMode) ((CCamera*)g_pPlayer->m_pCam)->setGakaku(DebugWork::g_dw.player.vCamEye.w);
        }
        if (DebugWork::g_dw.player.iCamMode) {
            if (DebugWork::g_dw.player.iCamMode == 2) {
                if (m_pCamData->vEye.x != DebugWork::g_dw.player.vCamEye.x) DebugWork::g_dw.player.vCamEye.z = DebugWork::g_dw.player.vCamEye.x;
                if (m_pCamData->vEye.z != DebugWork::g_dw.player.vCamEye.z) DebugWork::g_dw.player.vCamEye.x = DebugWork::g_dw.player.vCamEye.z;
            }
        }
        m_pCamData->vEye = DebugWork::g_dw.player.vCamEye;
        m_pCamData->fViewAngle = DebugWork::g_dw.player.vCamEye.w;
        m_pCamData->vAim = DebugWork::g_dw.player.vCamAim;
        m_pCamData->fAdjust = DebugWork::g_dw.player.vCamAim.w;
        m_pCamData->fDistance = DebugWork::g_dw.player.fCamDist;
        m_pCamData->fSize = DebugWork::g_dw.player.fCamSize;
    }
    m_pAtkData = (PATK_DATA*)DebugWork::g_dw.player.pAtkData;
    if (m_pAtkData) {
        m_pAtkData->ANIM_ID = DebugWork::g_dw.player.iAtkPrm[0];
        m_pAtkData->ANIM_ESQ = DebugWork::g_dw.player.iAtkPrm[1];
        m_pAtkData->EFFECT = DebugWork::g_dw.player.iAtkPrm[2];
        m_pAtkData->TRG_FRAME = DebugWork::g_dw.player.iAtkPrm[3];
        m_pAtkData->COLL_START_FRAME = DebugWork::g_dw.player.iAtkPrm[4];
        m_pAtkData->COLL_END_FRAME = DebugWork::g_dw.player.iAtkPrm[5];
        m_pAtkData->MOVE_START_FRAME = DebugWork::g_dw.player.iAtkPrm[6];
        m_pAtkData->MOVE_END_FRAME = DebugWork::g_dw.player.iAtkPrm[7];
        m_pAtkData->RADIUS = DebugWork::g_dw.player.iAtkPrm[8];
        m_pAtkData->MAX_SPEED = DebugWork::g_dw.player.iAtkPrm[9];
        m_pAtkData->POWER = DebugWork::g_dw.player.iAtkPrm[10];
        m_pAtkData->DAMAGE = DebugWork::g_dw.player.iAtkPrm[11];
        m_pAtkData->WORK0 = DebugWork::g_dw.player.iAtkPrm[12];
        m_pAtkData->WORK1 = DebugWork::g_dw.player.iAtkPrm[13];
        m_pAtkData->WORK2 = DebugWork::g_dw.player.iAtkPrm[14];
        m_pAtkData->FLAG = DebugWork::g_dw.player.iAtkFlag[0] | DebugWork::g_dw.player.iAtkFlag[1] << 1 | DebugWork::g_dw.player.iAtkFlag[2] << 2 | DebugWork::g_dw.player.iAtkFlag[3] << 3 | DebugWork::g_dw.player.iAtkFlag[4] << 4 | DebugWork::g_dw.player.iAtkFlag[5] << 5 | DebugWork::g_dw.player.iAtkFlag[6] << 6 | DebugWork::g_dw.player.iAtkFlag[7] << 7;
    }
}
/* the static initializer (DWT table titles) is generated after the last INCLUDE_ASM: gcc assumes .rdata for its strings */
__asm__(".rdata\n");
// SOURCE DATA: .data .rodata .bss
