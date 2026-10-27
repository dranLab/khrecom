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
#include "Game/Player/PlayerCamera.h"
#include "include_asm.h"

namespace PL {
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
/* placeholder: .rodata of startStockSkillAction__Q22PL11CPlayerSora (strings, jump tables) */
__asm__(".rdata\n.align 4\n.word .L00295084\n.word .L00295084\n.word .L00295084\n.word .L00295084\n.word .L00295084\n.word .L00295084\n.word .L00295084\n.word .L00295084\n.word .L00295084\n.word .L00295084\n.word .L00295084\n.word .L00295084\n.word .L00295084\n.word .L00295084\n");
__asm__(".word .L00295084\n.word .L00295084\n.word .L00295084\n.word .L00295084\n.word .L00295084\n.word .L00295084\n.word .L00295084\n.word .L00295084\n.word .L00295084\n.word .L00295084\n.word .L00295084\n.word .L00295084\n.word .L00295084\n.word .L00295084\n.word .L00295084\n.word .L00295084\n");
__asm__(".word .L00295084\n.word .L00295084\n.word .L00295084\n.word .L00295084\n.word .L00295084\n.word .L00295084\n.word .L00295084\n.word .L00295084\n.word .L00295084\n.word .L00295084\n.word .L00295084\n.word .L00295084\n.word .L00295084\n.word .L00295084\n.word .L00295084\n.word .L00295084\n");
__asm__(".word .L00295084\n.word .L00295084\n.word .L00295084\n.word .L0029508C\n.word .L00295084\n.word .L0029508C\n.word .L00295084\n.word .L0029508C\n.word .L00295084\n.word .L00295084\n.word .L0029508C\n.word .L00295084\n.word .L0029508C\n.word .L00295084\n.word .L0029508C\n.word .L00295084\n");
__asm__(".word .L00295084\n.word .L00295084\n.word .L00295084\n.word .L0029508C\n.word .L00295084\n.word .L00295084\n.word .L0029508C\n.word .L00295084\n.word .L00295084\n.word .L00295084\n.word .L0029508C\n.word .L00295084\n.word .L0029508C\n.word .L00295084\n.word .L0029508C\n.word .L00295084\n");
__asm__(".word .L0029508C\n.word .L00295084\n.word .L0029508C\n.word .L00295084\n.word .L0029508C\n.word .L0029509C\n.word .L0029509C\n.word .L0029509C\n.word func_0\n.word func_0\n.text\n");
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Game/Player/PlayerSora", startStockSkillAction__Q22PL11CPlayerSora);
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
/* placeholder: .rodata of initFriend__Q22PL11CPlayerSorai (strings, jump tables) */
__asm__(".rdata\n.align 4\n.word .L002A05A4\n.word .L002A05B8\n.word .L002A05CC\n.word .L002A05E0\n.word .L002A05F4\n.word .L002A0608\n.word .L002A061C\n.word .L002A0630\n.word .L002A0644\n.word func_0\n.word func_0\n.word func_0\n.text\n");
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Game/Player/PlayerSora", initFriend__Q22PL11CPlayerSorai);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Game/Player/PlayerSora", exeFriend__Q22PL11CPlayerSora);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Game/Player/PlayerSora", watchTarget__Q22PL11CPlayerSorai);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Game/Player/PlayerSora", setAttack_inFriend__Q22PL11CPlayerSoraiii);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Game/Player/PlayerSora", exeAttack_inFriend__Q22PL11CPlayerSora);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Game/Player/PlayerSora", endAttack_inFriend__Q22PL11CPlayerSora);
/* placeholder: .rodata of initSummon__Q22PL11CPlayerSorai (strings, jump tables) */
__asm__(".rdata\n.align 4\n.word .L002A17DC\n.word .L002A1800\n.word .L002A1824\n.word .L002A1838\n.word .L002A1860\n.word .L002A1880\n.word .L002A18A8\n.word func_0\n.text\n");
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Game/Player/PlayerSora", initSummon__Q22PL11CPlayerSorai);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Game/Player/PlayerSora", exeSummon__Q22PL11CPlayerSora);
/* placeholder: .rodata of setStateSE__Q22PL11CPlayerSoraii (strings, jump tables) */
__asm__(".rdata\n.align 4\n.word .L002A2168\n.word .L002A2158\n.word .L002A2150\n.word .L002A2160\n.word .L002A2148\n.word .L002A2128\n.word .L002A2130\n.word .L002A2138\n.word .L002A2140\n.word .L002A21A0\n.word .L002A21A0\n.word .L002A21A0\n.word .L002A21A0\n.word .L002A21A0\n");
__asm__(".word .L002A2140\n.word .L002A21A0\n.word .L002A21A0\n.word .L002A2148\n.word .L002A2168\n.word .L002A2158\n.word .L002A2160\n.word .L002A2170\n.word .L002A2148\n.word .L002A21A0\n.word .L002A2150\n.word .L002A2158\n.word .L002A2160\n.word .L002A2168\n.word .L002A2170\n.word .L002A21A0\n");
__asm__(".word .L002A21A0\n.word .L002A21A0\n.word .L002A21A0\n.word .L002A21A0\n.word .L002A21A0\n.word .L002A21A0\n.word .L002A21A0\n.word .L002A21A0\n.word .L002A21A0\n.word .L002A21A0\n.word .L002A21A0\n.word .L002A21A0\n.word .L002A21A0\n.word .L002A21A0\n.word .L002A21A0\n.word .L002A21A0\n");
__asm__(".word .L002A21A0\n.word .L002A21A0\n.word .L002A21A0\n.word .L002A21A0\n.word .L002A21A0\n.word .L002A21A0\n.word .L002A21A0\n.word .L002A21A0\n.word .L002A21A0\n.word .L002A21A0\n.word .L002A21A0\n.word .L002A21A0\n.word .L002A21A0\n.word .L002A21A0\n.word .L002A21A0\n.word .L002A21A0\n");
__asm__(".word .L002A21A0\n.word .L002A21A0\n.word .L002A21A0\n.word .L002A21A0\n.word .L002A21A0\n.word .L002A21A0\n.word .L002A21A0\n.word .L002A21A0\n.word .L002A21A0\n.word .L002A21A0\n.word .L002A21A0\n.word .L002A21A0\n.word .L002A21A0\n.word .L002A21A0\n.word .L002A21A0\n.word .L002A2178\n");
__asm__(".word .L002A218C\n.word .L002A21A0\n.word .L002A21A0\n.word .L002A21A0\n.word .L002A21A0\n.word .L002A21A0\n.word .L002A21A0\n.word .L002A21A0\n.word .L002A21A0\n.word .L002A21A0\n.word .L002A21A0\n.word .L002A21A0\n.word .L002A21A0\n.word .L002A21A0\n.word .L002A21A0\n.word .L002A21A0\n");
__asm__(".word .L002A21A0\n.word .L002A21A0\n.word .L002A21A0\n.word .L002A21A0\n.word .L002A21A0\n.word .L002A21A0\n.word .L002A21A0\n.word .L002A21A0\n.word .L002A2194\n.word .L002A21A0\n.word .L002A21A0\n.word .L002A21A0\n.word .L002A21A0\n.word .L002A21A0\n.word .L002A21A0\n.word .L002A21A0\n");
__asm__(".word .L002A21A0\n.word .L002A21A0\n.word .L002A21A0\n.word .L002A21A0\n.word .L002A2194\n.word func_0\n.text\n");
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Game/Player/PlayerSora", setStateSE__Q22PL11CPlayerSoraii);
/* placeholder: .rodata of getStateSESeq__Q22PL11CPlayerSorai (strings, jump tables) */
__asm__(".rdata\n.align 4\n.word .L002A2274\n.word .L002A2228\n.word .L002A2228\n.word .L002A2268\n.word .L002A2268\n.word .L002A2240\n.word .L002A2260\n.word .L002A2274\n.word .L002A2274\n.word .L002A2228\n.word .L002A2228\n.word .L002A2228\n.word .L002A2228\n.word .L002A2228\n");
__asm__(".word .L002A2274\n.word .L002A2274\n.word .L002A2274\n.word .L002A2274\n.word .L002A2274\n.word .L002A2274\n.word .L002A2268\n.word .L002A2268\n.word .L002A2268\n.word .L002A2268\n.word .L002A2240\n.word .L002A2240\n.word .L002A2260\n.word .L002A2260\n.word .L002A2248\n.word .L002A2248\n");
__asm__(".word .L002A2248\n.word .L002A2248\n.word .L002A2248\n.word .L002A2248\n.word .L002A2274\n.word .L002A2274\n.word .L002A2274\n.word .L002A2274\n.word .L002A2274\n.word .L002A2274\n.word .L002A2274\n.word .L002A2274\n.word .L002A2274\n.word .L002A2274\n.word .L002A2274\n.word .L002A2274\n");
__asm__(".word .L002A2274\n.word .L002A2274\n.word .L002A2274\n.word .L002A2230\n.word .L002A2274\n.word .L002A2268\n.word .L002A2238\n.word .L002A2240\n.word .L002A2260\n.word .L002A2258\n.word .L002A2250\n.word .L002A2248\n.word .L002A2274\n.word .L002A2270\n.word .L002A2268\n.word .L002A2268\n");
__asm__(".word .L002A2268\n.word .L002A2274\n.word .L002A2274\n.word .L002A2274\n.word .L002A2238\n.word .L002A2238\n.word .L002A2238\n.word .L002A2274\n.word .L002A2270\n.word .L002A2240\n.word .L002A2240\n.word .L002A2258\n.word .L002A2258\n.word .L002A2260\n.word .L002A2260\n.word .L002A2250\n");
__asm__(".word .L002A2250\n.word .L002A2248\n.word .L002A2248\n.word .L002A2274\n.word .L002A2274\n.word .L002A2274\n.word .L002A2274\n.word .L002A2274\n.word .L002A2270\n.word .L002A2270\n.word .L002A2270\n.word .L002A2270\n.word .L002A2270\n.word .L002A2270\n.word .L002A2270\n.word .L002A2270\n");
__asm__(".word .L002A2270\n.word .L002A2270\n.word .L002A2270\n.word .L002A2270\n.word .L002A2270\n.word .L002A2270\n.word .L002A2270\n.word .L002A2270\n.word .L002A2270\n.word .L002A2270\n.word .L002A2270\n.word .L002A2270\n.word .L002A2274\n.word .L002A2274\n.word .L002A2274\n.word .L002A2270\n");
__asm__(".word .L002A2270\n.word .L002A2270\n.word .L002A2270\n.word .L002A2270\n.word .L002A2260\n.word .L002A2268\n.word .L002A2270\n.word func_0\n.text\n");
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Game/Player/PlayerSora", getStateSESeq__Q22PL11CPlayerSorai);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Game/Player/PlayerSora", initGlide__Q22PL11CPlayerSorai);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Game/Player/PlayerSora", exeGlide__Q22PL11CPlayerSora);
void CPlayerSora::exeAvoid() {
    exeDogeRoll();
}
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Game/Player/PlayerSora", exeDogeRoll__Q22PL11CPlayerSora);
/* placeholder: .rodata of initPronounce__Q22PL11CPlayerSorai (strings, jump tables) */
__asm__(".rdata\n.align 3\n.byte 0x43,0x50,0x6c,0x61,0x79,0x65,0x72,0x53,0x6f,0x72,0x61,0x3a,0x3a,0x20,0x69,0x6e\n.byte 0x69,0x74,0x50,0x72,0x6f,0x6e,0x6f,0x75,0x6e,0x63,0x65,0x28,0x25,0x64,0x29,0x0a\n.byte 0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00\n.text\n");
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Game/Player/PlayerSora", initPronounce__Q22PL11CPlayerSorai);
/* placeholder: .rodata of exePronounce__Q22PL11CPlayerSora (strings, jump tables) */
__asm__(".rdata\n.align 2\n.byte 0x43,0x50,0x6c,0x61,0x79,0x65,0x72,0x53,0x6f,0x72,0x61,0x3a,0x3a,0x20,0x65,0x78\n.byte 0x65,0x50,0x72,0x6f,0x6e,0x6f,0x75,0x6e,0x63,0x65,0x28,0x29,0x20,0x25,0x64,0x2c\n.byte 0x25,0x64,0x2c,0x25,0x64,0x20,0x81,0xa6,0x83,0x4d,0x83,0x8a,0x83,0x4d,0x83,0x8a\n.byte 0x8d,0xc5,0x8c,0xe3,0x82,0xcc,0x93,0xfc,0x97,0xcd,0x96,0xb3,0x8c,0xf8,0x0a,0x00\n.align 3\n.byte 0x43,0x50,0x6c,0x61,0x79,0x65,0x72,0x53,0x6f,0x72,0x61,0x3a,0x3a,0x20,0x65,0x78\n.byte 0x65,0x50,0x72,0x6f,0x6e,0x6f,0x75,0x6e,0x63,0x65,0x28,0x29,0x20,0x54,0x52,0x47\n.byte 0x21,0x0a,0x00,0x00,0x00,0x00,0x00,0x00\n.align 3\n.byte 0x43,0x50,0x6c,0x61,0x79,0x65,0x72,0x53,0x6f,0x72,0x61,0x3a,0x3a,0x20,0x65,0x78\n.byte 0x65,0x50,0x72,0x6f,0x6e,0x6f,0x75,0x6e,0x63,0x65,0x28,0x29,0x20,0x54,0x52,0x47\n.byte 0x2e,0x2e,0x2e,0x52,0x45,0x46,0x52,0x45,0x43,0x54,0x0a,0x00,0x00,0x00,0x00,0x00\n.byte 0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00\n.align 4\n");
__asm__(".word .L002A2D54\n.word .L002A2E14\n.word .L002A3100\n.word .L002A2EA4\n.word .L002A2EA4\n.word .L002A2EA4\n.word .L002A2EA4\n.word .L002A2EA4\n.word .L002A2EA4\n.word .L002A3074\n.word .L002A3100\n.word .L002A3100\n.text\n");
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Game/Player/PlayerSora", exePronounce__Q22PL11CPlayerSora);
/* placeholder: .rodata of calcAppearPosFS_toAttackID__Q22PL11CPlayerSora (strings, jump tables) */
__asm__(".rdata\n.align 4\n.word .L002A316C\n.word .L002A316C\n.word .L002A316C\n.word .L002A316C\n.word .L002A316C\n.word .L002A316C\n.word .L002A316C\n.word .L002A31AC\n.word .L002A316C\n.word .L002A316C\n.word .L002A316C\n.word .L002A316C\n.word .L002A31AC\n.word .L002A31AC\n");
__asm__(".word .L002A31AC\n.word .L002A316C\n.word .L002A316C\n.word .L002A316C\n.word .L002A31AC\n.word .L002A3154\n.word .L002A316C\n.word .L002A316C\n.word .L002A316C\n.word .L002A316C\n.word .L002A316C\n.word .L002A316C\n.word .L002A316C\n.word .L002A316C\n.word .L002A316C\n.word .L002A316C\n");
__asm__(".word .L002A31AC\n.word .L002A31AC\n.word .L002A31AC\n.word .L002A31AC\n.word .L002A31AC\n.word .L002A316C\n.word .L002A316C\n.word .L002A31AC\n.word .L002A31AC\n.word .L002A3190\n.word .L002A31AC\n.word .L002A31AC\n.word .L002A31AC\n.word .L002A31AC\n.word .L002A31AC\n.word .L002A31AC\n");
__asm__(".word .L002A31AC\n.word .L002A31AC\n.word .L002A3190\n.word .L002A3190\n.word .L002A3190\n.word func_0\n.text\n");
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Game/Player/PlayerSora", calcAppearPosFS_toAttackID__Q22PL11CPlayerSora);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Game/Player/PlayerSora", initDuel__Q22PL11CPlayerSorai);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Game/Player/PlayerSora", exeDuel__Q22PL11CPlayerSora);
/* end of .rodata: the vtables (key function ~CPlayerSora still asm) and the deferred const g_vPronounceCountOfs */
__asm__(".rdata\n.globl _vt$Q22PL11CPlayerSora$4CFSM\n_vt$Q22PL11CPlayerSora$4CFSM:\n.word 0x0000FFC0\n.word func_0\n.word 0x0000FFC0\n.word _$_Q22PL11CPlayerSora\n.word func_0\n.word update__4CFSM\n.word func_0\n.word beginStateInitial__4CFSM\n.word 0x0000FFC0\n.word stateInitial__7CPlayer\n.word func_0\n.word endStateInitial__4CFSM\n.word 0x0000FFC0\n");
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
