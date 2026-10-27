#define TASK_H_NO_SLC_OVERFLOW_MSG
#define GPRIM_H_PATH "System/Graphic/GPrim.h"
#include "System/Graphic/Anim.h"
#include "System/Graphic/Gprim.h"
#include "System/Graphic/2d/2DMan.h"
#include "Game/CardBattle.h"
#include "System/Graphic/Str.h"
#include "Game/Collision/CollUtil.h"

/* Placeholder: an empty string follows CollUtil.h's strings in .rodata (as in PlayerDRiku). */
inline void _emptyStringPlaceholder() {
    TRACE("");
}

#include "System/Graphic/Effect/effect.h"

/* System/Task.h's container overflow message and two prize messages (from an unidentified header) come here. */
inline void _taskSLCOverflowMsg() {
    TRACE("\x83\x5e\x83\x58\x83\x4e\x97\x70\x83\x52\x83\x93\x83\x65\x83\x69\x82\xaa MAX \x82\xc9\x93\x9e\x92\x42\x82\xb5\x82\xdc\x82\xb5\x82\xbd\n");
}
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
#include "Game/Movement.h"
#include "include_asm.h"

namespace PL {
extern TIMEQ g_CardBreakTime[];
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

CPlayerSora::CPlayerSora(CTask* inParent) : CPlayer(inParent, PLM::g_uiPlayerID, 0) {
    g_Param = g_SoraParam;
    m_hMagicCore = 0;
    initMagicBullet();
    m_nHitCount = 0;
}
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Game/Player/PlayerSora", _$_Q22PL11CPlayerSora);
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
/* placeholder: .rodata of initStockSkill__Q22PL11CPlayerSorai (strings, jump tables) */
__asm__(".rdata\n.align 4\n.word .L002951E8\n.word .L002951F0\n.word .L00295204\n.word .L0029520C\n.word .L00295214\n.word .L0029521C\n.word .L00295230\n.word .L00295244\n.word .L00295258\n.word .L0029526C\n.word .L002952C4\n.word .L00295348\n.word .L002953C8\n.word .L00295420\n");
__asm__(".word .L00295478\n.word .L002954D0\n.word .L00295298\n.word .L002952F0\n.word .L00295374\n.word .L002953F4\n.word .L0029544C\n.word .L002954A4\n.word .L002954FC\n.word .L002953A0\n.word .L002953A8\n.word .L002953B0\n.word .L002953B8\n.word .L002953C0\n.word .L00295698\n.word .L002956AC\n");
__asm__(".word .L002956C0\n.word .L002956D4\n.word .L0029531C\n.word .L002956E8\n.word .L002956FC\n.word .L00295710\n.word .L00295724\n.word .L00295748\n.word .L0029575C\n.word .L00295528\n.word .L00295554\n.word .L00295588\n.word .L002955C0\n.word .L002955F8\n.word .L00295630\n.word .L00295770\n");
__asm__(".word .L00295654\n.word .L00295794\n.word .L002958D4\n.word .L002958D4\n.word .L002958FC\n.word .L002958FC\n.word .L00295928\n.word .L00295928\n.word .L00295928\n.word .L00295940\n.word .L00295940\n.word .L00295960\n.word .L00295960\n.word .L00295988\n.word .L00295988\n.word .L002959AC\n");
__asm__(".word .L002959AC\n.word .L002959AC\n.word .L002957B8\n.word .L002957B8\n.word .L002957B8\n.word .L002957D0\n.word .L002957D0\n.word .L002957D0\n.word .L002957E8\n.word .L00295844\n.word .L00295844\n.word .L0029585C\n.word .L0029585C\n.word .L00295874\n.word .L00295874\n.word .L0029588C\n");
__asm__(".word .L0029588C\n.word .L002958A4\n.word .L002958A4\n.word .L002958BC\n.word .L002958BC\n.word .L002959EC\n.word .L002959FC\n.word .L002959FC\n.text\n");
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Game/Player/PlayerSora", initStockSkill__Q22PL11CPlayerSorai);
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
/* placeholder: .rodata of initMagic__Q22PL11CPlayerSorai (strings, jump tables) */
__asm__(".rdata\n.align 4\n.word .L0029CB98\n.word .L0029CBB0\n.word .L0029CC0C\n.word .L0029CD68\n.word .L0029CDDC\n.word .L0029CF40\n.word .L0029D008\n.word .L0029D0B4\n.word .L0029D128\n.word func_0\n.word func_0\n.word func_0\n.text\n");
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Game/Player/PlayerSora", initMagic__Q22PL11CPlayerSorai);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Game/Player/PlayerSora", exeMagic__Q22PL11CPlayerSora);
/* placeholder: .rodata of setMagic__Q22PL11CPlayerSoraiii (strings, jump tables) */
__asm__(".rdata\n.align 4\n.word .L0029ECD8\n.word .L0029ECBC\n.word .L002A0328\n.word .L0029ECBC\n.word .L0029ECBC\n.word .L002A0328\n.word .L0029ECB0\n.word .L0029ECD8\n.text\n");
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Game/Player/PlayerSora", setMagic__Q22PL11CPlayerSoraiii);
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
// reads/writes members and globals as through a reference (the original reloads/stores them in this order)
template <class T> static inline T& _ref(T& v) { return v; }

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
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Game/Player/PlayerSora", exeFriend__Q22PL11CPlayerSora);
void SubVectorXYZ(FVECTOR& v0, const FVECTOR& v1, const FVECTOR& v2);
void Normalize(FVECTOR& outV, const FVECTOR& inV);
extern "C" float atan2f(float y, float x);

/* abs.s of the original (Math3D.h) */
static inline FLOAT _abs(FLOAT inValue) {
    FLOAT ret;
    __asm__ volatile("abs.s %0, %1" : "=f"(ret) : "f"(inValue));
    return ret;
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
static inline UINT _upKind(INT inType0, INT inType1) {
    UINT up = inType0 > 0 ? 1 << (inType0 - 1) : 0;
    if (inType1 > 0) up |= 1 << (inType1 - 1);
    return up;
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
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Game/Player/PlayerSora", exeSummon__Q22PL11CPlayerSora);
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
    m_pMove->setFlag(0x40, TRUE);
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
/* placeholder: .rodata of exePronounce__Q22PL11CPlayerSora (strings, jump tables) */
__asm__(".rdata\n.align 3\n.byte 0x43,0x50,0x6c,0x61,0x79,0x65,0x72,0x53,0x6f,0x72,0x61,0x3a,0x3a,0x20,0x65,0x78\n.byte 0x65,0x50,0x72,0x6f,0x6e,0x6f,0x75,0x6e,0x63,0x65,0x28,0x29,0x20,0x25,0x64,0x2c\n.byte 0x25,0x64,0x2c,0x25,0x64,0x20,0x81,0xa6,0x83,0x4d,0x83,0x8a,0x83,0x4d,0x83,0x8a\n.byte 0x8d,0xc5,0x8c,0xe3,0x82,0xcc,0x93,0xfc,0x97,0xcd,0x96,0xb3,0x8c,0xf8,0x0a,0x00\n.align 3\n.byte 0x43,0x50,0x6c,0x61,0x79,0x65,0x72,0x53,0x6f,0x72,0x61,0x3a,0x3a,0x20,0x65,0x78\n.byte 0x65,0x50,0x72,0x6f,0x6e,0x6f,0x75,0x6e,0x63,0x65,0x28,0x29,0x20,0x54,0x52,0x47\n.byte 0x21,0x0a,0x00,0x00,0x00,0x00,0x00,0x00\n.align 3\n.byte 0x43,0x50,0x6c,0x61,0x79,0x65,0x72,0x53,0x6f,0x72,0x61,0x3a,0x3a,0x20,0x65,0x78\n.byte 0x65,0x50,0x72,0x6f,0x6e,0x6f,0x75,0x6e,0x63,0x65,0x28,0x29,0x20,0x54,0x52,0x47\n.byte 0x2e,0x2e,0x2e,0x52,0x45,0x46,0x52,0x45,0x43,0x54,0x0a,0x00,0x00,0x00,0x00,0x00\n.byte 0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00\n.align 4\n");
__asm__(".word .L002A2D54\n.word .L002A2E14\n.word .L002A3100\n.word .L002A2EA4\n.word .L002A2EA4\n.word .L002A2EA4\n.word .L002A2EA4\n.word .L002A2EA4\n.word .L002A2EA4\n.word .L002A3074\n.word .L002A3100\n.word .L002A3100\n.text\n");
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Game/Player/PlayerSora", exePronounce__Q22PL11CPlayerSora);
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
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Game/Player/PlayerSora", initDuel__Q22PL11CPlayerSorai);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Game/Player/PlayerSora", exeDuel__Q22PL11CPlayerSora);
/* end of .rodata: the vtables (key function ~CPlayerSora still asm) and the deferred const g_vPronounceCountOfs */
__asm__(".rdata\n.align 3\n.globl _vt$Q22PL11CPlayerSora$4CFSM\n_vt$Q22PL11CPlayerSora$4CFSM:\n.word 0x0000FFC0\n.word func_0\n.word 0x0000FFC0\n.word _$_Q22PL11CPlayerSora\n.word func_0\n.word update__4CFSM\n.word func_0\n.word beginStateInitial__4CFSM\n.word 0x0000FFC0\n.word stateInitial__7CPlayer\n.word func_0\n.word endStateInitial__4CFSM\n.word 0x0000FFC0\n");
__asm__(".word endStateProc__7CEntity\n.word func_0\n.word func_0\n.globl _vt$Q22PL11CPlayerSora\n_vt$Q22PL11CPlayerSora:\n.word func_0\n.word func_0\n.word func_0\n.word _$_Q22PL11CPlayerSora\n.word func_0\n.word main__7CEntity\n.word func_0\n.word disp__7CEntity\n.word func_0\n.word after__5CTask\n.word func_0\n");
__asm__(".word onOut__5CTask\n.word func_0\n.word getTypeID__C7CEntity\n.word func_0\n.word initalize__7CEntityii\n.word func_0\n.word initalize__7CEntityiiUi\n.word func_0\n.word AfterInit__7CEntity\n.word func_0\n.word onMsg__7CPlayeriPv\n.word func_0\n.word getLockOnPos__7CPlayerR7FVECTOR\n.word func_0\n.word getHeadPos__C7CPlayerR7FVECTOR\n.word func_0\n");
__asm__(".word getUseCardPos__7CPlayerR7FVECTOR\n.word func_0\n.word getCameraTargetPos__7CPlayerR7FVECTOR\n.word func_0\n.word changeMotion__7CEntityiiiiii\n.word func_0\n.word updateBefore__7CPlayer\n.word func_0\n.word updateAfter__7CPlayer\n.word func_0\n.word updatePositionAfter__7CEntity\n.word func_0\n.word lastUpdate__7CPlayer\n.word func_0\n.word beginStateField__7CPlayer\n.word func_0\n");
__asm__(".word stateField__7CPlayer\n.word func_0\n.word endStateField__7CPlayer\n.word func_0\n.word beginStateBattle__7CPlayer\n.word func_0\n.word stateBattle__7CPlayer\n.word func_0\n.word endStateBattle__7CPlayer\n.word func_0\n.word beginStateWait__7CPlayer\n.word func_0\n.word stateWait__7CPlayer\n.word func_0\n.word endStateWait__7CPlayer\n.word func_0\n");
__asm__(".word beginStateHide__7CPlayer\n.word func_0\n.word stateHide__7CPlayer\n.word func_0\n.word endStateHide__7CPlayer\n.word func_0\n.word onWeapon__7CPlayeri\n.word func_0\n.word offWeapon__7CPlayer\n.word func_0\n.word setStateSE__Q22PL11CPlayerSoraii\n.word func_0\n.word getStateSESeq__Q22PL11CPlayerSorai\n.word func_0\n.word setAttack_inFriend__Q22PL11CPlayerSoraiii\n.word func_0\n");
__asm__(".word calcAppearPosFS_toAttackID__Q22PL11CPlayerSora\n.word func_0\n.word setAttack__7CPlayer\n.word func_0\n.word startStockSkillAction__Q22PL11CPlayerSora\n.word func_0\n.word initStockSkill__Q22PL11CPlayerSorai\n.word func_0\n.word exeStockSkill__Q22PL11CPlayerSora\n.word func_0\n.word initMagic__Q22PL11CPlayerSorai\n.word func_0\n.word exeMagic__Q22PL11CPlayerSora\n.word func_0\n.word setMagic__Q22PL11CPlayerSoraiii\n.word func_0\n");
__asm__(".word initFriend__Q22PL11CPlayerSorai\n.word func_0\n.word exeFriend__Q22PL11CPlayerSora\n.word func_0\n.word initSummon__Q22PL11CPlayerSorai\n.word func_0\n.word exeSummon__Q22PL11CPlayerSora\n.word func_0\n.word exeStandUp_inFriend__7CPlayer\n.word func_0\n.word exeAttack_inFriend__Q22PL11CPlayerSora\n.word func_0\n.word endAttack_inFriend__Q22PL11CPlayerSora\n.word func_0\n.word initTransform__7CPlayeri\n.word func_0\n");
__asm__(".word exeTransform__7CPlayer\n.word func_0\n.word initGlide__Q22PL11CPlayerSorai\n.word func_0\n.word exeGlide__Q22PL11CPlayerSora\n.word func_0\n.word initJump2nd__7CPlayer\n.word func_0\n.word exeJump2nd__7CPlayer\n.word func_0\n.word exeAvoid__Q22PL11CPlayerSora\n.word func_0\n.word initDuel__Q22PL11CPlayerSorai\n.word func_0\n.word exeDuel__Q22PL11CPlayerSora\n.word func_0\n");
__asm__(".word initPronounce__Q22PL11CPlayerSorai\n.word func_0\n.word exePronounce__Q22PL11CPlayerSora\n.word func_0\n.word func_0\n.globl _2PL$g_vPronounceCountOfs\n_2PL$g_vPronounceCountOfs:\n.byte 0x00,0x00,0x00,0x00,0x00,0x00,0xc0,0x3f,0x00,0x00,0x00,0x00,0x00,0x00,0x80,0x3f\n.text\n");
// SOURCE DATA: .data .rodata
