#include "khtypes.h"
#define GAME_PLAYER_PLAYERGAUGE_CPP /* MATCH: unit gate (its header gives this unit its own parts); without it the link fails (inlines this unit must emit) (tested 2026-11-02) */
/* .rodata starts with the strings of the included headers' inline functions, in this order. */
#include "System/Graphic/Gprim.h"
#include "System/Graphic/Model.h"
#include "System/RsrcData.h"
#include "System/Graphic/Anim.h"
#include "System/TaskMan.h"
#include "System/Task.h"
#include "System/Graphic/2d/2DMan.h"
#include "Game/CardBattle.h"
#include "Game/Enemy/BattleEnemy.h"
#include "System/Graphic/Str.h"
#include "Game/Collision/CollUtil.h"
/* MATCH: placeholder: an empty string follows CollUtil.h's strings in .rodata (as in PlayerManager and CardRevolver) */
inline void _emptyStringPlaceholderPlayerGauge() { TRACE(""); }
#include "Game/Player/DWTPlayer.h"
#include "Game/Player/PlayerGauge.h"
#include "System/Graphic/2d/2DSeqCtrl.h"
#include "Game/GameWork.h"
#include "Game/SceneEnv.h"
#include "Game/Game.h"
#include "Game/Entity.h"
#include "Game/Player/PlayerCamera.h"
#include "Game/CardKind.h"
#include "Game/Player/Player.h"
#include "System/Graphic/WorldEnv.h"
#include "Game/SceneInfo.h"
#include "Game/Player/PlayerManager.h"
#include "Game/Player/NaviMap.h"
#include "System/Graphic/2d/2DCard.h"
#include "System/Sound/SoundMan.h"
#include "System/Graphic/2d/2DFnt.h"
#include "include_asm.h"

using namespace Gauge;

#define RET_ADDR() ({ UINT nRA; __asm__ volatile("daddu %0, $31, $0" : "=r"(nRA)); nRA; })
inline void* operator new(unsigned int, void* p) throw() { return p; }
namespace Gauge {
LPCSTR g_szLadGauge = "gauge.lad";
LPCSTR g_szSeqFace = "gauge:00";
LPCSTR g_szSeqGauge = "gauge:01";
LPCSTR g_szSeqDFace = "gauge:02";
LPCSTR g_szLadCockpit = "Cockpit.lad";
LPCSTR g_szSeqCockpit = "Cockpit:00";
LPCSTR g_szLadGetCard = "get_card.lad";
LPCSTR g_szSeqGetCard = "get_card:00";
G_UV g_uvNumberTex[2][10] = {
    {{98, 53}, {115, 53}, {128, 54}, {144, 53}, {161, 53}, {176, 53}, {192, 53}, {208, 53}, {224, 53}, {240, 53}},
    {{112, 67}, {125, 67}, {143, 67}, {159, 67}, {174, 67}, {191, 67}, {207, 67}, {222, 67}, {238, 67}, {253, 67}},
};
int g_RikuSeqIDData[2][3] = {{40, 41, 30}, {17, 18, 19}};
}
CEnemyGauge* CEnemyGauge::m_pInstance = NULL;
CCommonGauge* CCommonGauge::m_pInstance = NULL;
CPrizeGauge* CPrizeGauge::m_pInstance = NULL;
namespace CardBattle {
extern G_UV g_uvCRCounterNumTex[8][10];
}
FVECTOR getPlayerPos(BOOL flag);






void SubVectorXYZ(FVECTOR& v0, const FVECTOR& v1, const FVECTOR& v2);
FLOAT InnerProduct(const FVECTOR& inV0, const FVECTOR& inV1);
FLOAT RotTransPersClipW(IVECTOR& out_v, const FVECTOR& in_v);
static inline void _setRotTransPersMatrix(FMATRIX& m) {
    __asm__ volatile(
        "vsub.xyzw $vf3, $vf0, $vf0\n"
        "lqc2 $vf4, 0x0(%0)\n"
        "lqc2 $vf5, 0x10(%0)\n"
        "lqc2 $vf6, 0x20(%0)\n"
        "lqc2 $vf7, 0x30(%0)\n"
        :
        : "r"(&m));
}
static inline FLOAT _sqrtf(FLOAT x) {
    FLOAT ret;
    asm volatile("sqrt.s %0, %1" : "=f"(ret) : "f"(x));
    return ret;
}
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
static inline void _setCard(CARD& c, USHORT kind, BYTE num) {
    c.m_nId = 0;
    c.m_nKind2 = kind;
    c.m_nNumber2 = num;
    c.m_nCategory20 = cardDataTbl[c.m_nKind].nCategory0;
    c.m_nCategory21 = cardDataTbl[c.m_nKind].nCategory1;
}
/* MATCH: this CARD init stores the number before the kind (as CardRevolver's debug cards) */
static inline void _setCardNum(CARD& c, USHORT kind, BYTE num) {
    c.m_nId = 0;
    c.m_nNumber2 = num;
    c.m_nKind2 = kind;
    c.m_nCategory20 = cardDataTbl[c.m_nKind].nCategory0;
    c.m_nCategory21 = cardDataTbl[c.m_nKind].nCategory1;
}
inline void* operator new(size_t size, char* msg) throw() { return com_malloc_d(size, msg, RET_ADDR()); }
inline void operator delete[](void* mem) { com_free(mem); }
inline void* operator new[](size_t size, char* msg) throw() { return com_malloc_d(size, msg, RET_ADDR()); }
template <class T> static inline T& _ref(T& v) { return v; }

CPlayerGauge::CPlayerGauge(CTask* p) : CTask(p, 0x4000), m_pNaviMap(NULL), m_pFriend(NULL) { _initialize(); }
CPlayerGauge::~CPlayerGauge() {
    delete m_pNaviMap;
    m_pNaviMap = NULL;
    delete m_pFriend;
    m_pFriend = NULL;
    delete m_pCommon;
    m_pCommon = NULL;
    delete m_pPrize;
    m_pPrize = NULL;
    delete m_pSeqCtrl;
    m_pSeqCtrl = NULL;
    delete[] m_pSeqCtrlDP;
    m_pSeqCtrlDP = NULL;
    if (m_hAlertSE > 0) {
        CSound::stopSE(m_hAlertSE, 9);
        m_hAlertSE = 0;
    }
}
void CPlayerGauge::_initialize() {
    FVECTOR pos;
    m_nMaxHP = 300;
    m_nRedHP = 300;
    m_nHP = 300;
    m_iState = 1;
    m_uiFlag = 0;
    m_hAlertSE = 0;
    m_tqTimer = 0;
    m_tqRed = 0;
    m_szSeqFace = _ref(g_szSeqFace); // MATCH: global load kept after the field stores
    m_pSeqCtrl = NULL;
    m_pSeqCtrl = new (__FILE__ "(134)") D2::C2DSeqCtrl;
    m_pSeqCtrl->setLayer(2);
    m_pSeqCtrl->addSeq(g_szSeqGauge, 0xE);
    m_pSeqCtrl->addSeq(g_szSeqGauge, 7);
    m_pSeqCtrl->addSeq(g_szSeqGauge, 8);
    m_pSeqCtrl->addSeq(g_szSeqGauge, 9);
    m_pSeqCtrl->addSeq(m_szSeqFace, 3);
    changeTimeHP(m_pSeqCtrl, 1, m_nMaxHP);
    changeTimeHP(m_pSeqCtrl, 2, m_nMaxHP);
    changeTimeHP(m_pSeqCtrl, 3, m_nMaxHP);
    D2::C2DMan::getLayPosition(g_szLadGauge, 1, 0, pos, FALSE);
    m_pSeqCtrl->setPos(pos);
    m_pCommon = new (__FILE__ "(161)") CCommonGauge(this);
    m_pPrize = new (__FILE__ "(162)") CPrizeGauge(this);
    if (!CSceneInfo::isBattleScene() && !CSceneInfo::isMinigameScene()) {
        m_pNaviMap = new (__FILE__ "(166)") CNaviMap(this);
        INT mode = CGame::getDbgGameMode();
        if (mode >= 7) m_pFriend = new (__FILE__ "(178)") CFriendGauge(this);
        else if (mode < 4) m_pFriend = new (__FILE__ "(178)") CFriendGauge(this);
    }
    if (CGameWork::getInt(1, 0xE2) & 1) {
        m_iState = 7;
        m_pSeqCtrl->enableDisp(0, FALSE);
        m_pSeqCtrl->enableDisp(1, FALSE);
        m_pSeqCtrl->enableDisp(2, FALSE);
        m_pSeqCtrl->enableDisp(3, FALSE);
        m_pSeqCtrl->enableDisp(4, FALSE);
    } else if (m_pFriend) {
        m_pFriend->setGauge(1);
    }
    m_pSeqCtrlDP = NULL;
    m_nMaxDP = 0;
    m_nDP = 0;
    m_nStateDP = 0;
    if (CGame::isRikuGame() && CSceneInfo::isBattleScene()) {
        m_pSeqCtrlDP = new (__FILE__ "(206)") D2::C2DSeqCtrl[3];
        m_nMaxDP = 30;
        if (CGameWork::getInt(0, 0x24) & 8) m_uiFlag |= 4;
    }
    setDPGauge(m_iState);
}
/* MATCH: signed /4 of the USHORT max HP (as the INT max locals of setHP/setDMode) */
static inline INT _max(INT v) { return v; }
void CPlayerGauge::main() {
    if (CSceneEnv::isStopGauge()) return;
    switch (m_iState) {
    case 0:
        break;
    case 1:
    case 2:
    case 3:
    case 4:
    case 5:
        if (CGameWork::getInt(1, 0xE2) & 1) {
            *(USHORT*)&m_iState = 7; // MATCH: not a struct-field store (register of the constant)
            m_pSeqCtrl->replaceSeq(0, g_szSeqGauge, 0x10);
            m_nRedHP = m_nHP;
            m_pSeqCtrl->enableDisp(1, FALSE);
            m_pSeqCtrl->enableDisp(2, FALSE);
            m_pSeqCtrl->enableDisp(3, FALSE);
            m_pSeqCtrl->enableDisp(4, FALSE);
            if (m_hAlertSE > 0) {
                CSound::stopSE(m_hAlertSE, 9);
                m_hAlertSE = 0;
            }
            if (m_pFriend) m_pFriend->setGauge(2);
            setDPGauge(m_iState);
        }
        break;
    case 6:
        m_nRedHP = m_nHP;
        break;
    case 7:
        if (!(CGameWork::getInt(1, 0xE2) & 1)) {
            *(USHORT*)&m_iState = 1; // MATCH: as above
            m_pSeqCtrl->replaceSeq(0, g_szSeqGauge, 0xE);
            m_uiFlag = (INT)m_uiFlag & ~1;
            m_pSeqCtrl->enableDisp(0, TRUE);
            m_pSeqCtrl->enableDisp(1, TRUE);
            m_pSeqCtrl->enableDisp(3, TRUE);
            m_pSeqCtrl->enableDisp(4, TRUE);
            if (m_pFriend) m_pFriend->setGauge(1);
            setDPGauge(m_iState);
        }
        break;
    }
    if (m_nRedHP != m_nHP && m_iState != 7) {
        if (!(m_uiFlag & 2)) {
            m_uiFlag |= 2;
            m_pSeqCtrl->enableDisp(2, TRUE);
        }
        if (m_nRedHP < m_nHP) {
            m_nRedHP = m_nHP;
        } else if (m_tqRed <= 0) {
            m_tqRed = 0;
            m_nRedHP -= (m_nRedHP - m_nHP) / 10 + 1;
        }
        changeTimeHP(m_pSeqCtrl, 2, m_nRedHP);
    } else if (m_uiFlag & 2) {
        m_uiFlag = (INT)m_uiFlag & ~2;
        m_pSeqCtrl->enableDisp(2, FALSE);
    }
    m_tqRed -= getFrameRate();
    switch (m_iState) {
    case 1:
        if (m_pSeqCtrl->isEnd(0)) {
            m_tqTimer = 0;
            if (m_nHP == 0) {
                m_pSeqCtrl->replaceSeq(0, g_szSeqGauge, 0xF);
                m_pSeqCtrl->replaceSeq(4, m_szSeqFace, 2);
                m_iState = 5;
            } else if (m_nHP <= _max(m_nMaxHP) / 4) {
                m_pSeqCtrl->replaceSeq(0, g_szSeqGauge, 0xA);
                m_pSeqCtrl->replaceSeq(4, m_szSeqFace, 0);
                m_iState = 4;
            } else {
                m_pSeqCtrl->replaceSeq(0, g_szSeqGauge, 0xF);
                m_pSeqCtrl->replaceSeq(4, m_szSeqFace, 3);
                m_iState = 2;
                if (m_hAlertSE > 0) {
                    CSound::stopSE(m_hAlertSE, 9);
                    m_hAlertSE = 0;
                }
            }
        }
        break;
    case 4:
        if (_max(m_nMaxHP) / 4 < m_nHP) {
            m_pSeqCtrl->replaceSeq(0, g_szSeqGauge, 0xF);
            m_pSeqCtrl->replaceSeq(4, m_szSeqFace, 3);
            m_iState = 2;
            if (m_hAlertSE > 0) {
                CSound::stopSE(m_hAlertSE, 9);
                m_hAlertSE = 0;
            }
        } else if (m_hAlertSE == 0) {
            m_hAlertSE = CSound::playSE(9, -1, 64);
        }
        break;
    case 5:
        if (m_hAlertSE > 0) {
            CSound::stopSE(m_hAlertSE, 9);
            m_hAlertSE = 0;
        }
        break;
    case 3:
        m_tqTimer += getFrameRate();
        if (m_tqTimer >= 3000) {
            m_tqTimer = 0;
            if (m_nHP == 0) {
                m_pSeqCtrl->replaceSeq(0, g_szSeqGauge, 0xF);
                m_pSeqCtrl->replaceSeq(4, m_szSeqFace, 2);
                m_iState = 5;
            } else if (m_nHP <= _max(m_nMaxHP) / 4) {
                m_pSeqCtrl->replaceSeq(0, g_szSeqGauge, 0xA);
                m_pSeqCtrl->replaceSeq(4, m_szSeqFace, 0);
                m_iState = 4;
            } else {
                m_pSeqCtrl->replaceSeq(0, g_szSeqGauge, 0xF);
                m_pSeqCtrl->replaceSeq(4, m_szSeqFace, 3);
                m_iState = 2;
                if (m_hAlertSE > 0) {
                    CSound::stopSE(m_hAlertSE, 9);
                    m_hAlertSE = 0;
                }
            }
        }
        break;
    case 6:
        if (m_pSeqCtrl->isEnd(0)) {
            m_iState = 0;
            m_pSeqCtrl->enableDispEach(FALSE);
        }
        break;
    case 7:
        if ((m_uiFlag & 1) == 0 && m_pSeqCtrl->isEnd(0)) {
            m_uiFlag |= 1;
            m_pSeqCtrl->enableDispEach(FALSE);
        }
        break;
    }
    exeDPGauge();
}
void CPlayerGauge::changeTimeHP(D2::C2DSeqCtrl* p, UINT nIdx, UINT nHP) {
    p->changeTime(nIdx, (UINT)((FLOAT)nHP / 560.0f * 470.0f));
}
void CPlayerGauge::setMaxHP(INT n) {
    m_nMaxHP = n > -1 ? n : 0;
    if (m_nMaxHP > 560) m_nMaxHP = 560;
    changeTimeHP(m_pSeqCtrl, 1, n);
}
void CPlayerGauge::setHP(INT inHP, INT inFlag) {
    if (m_nHP == inHP) return;
    m_nHP = inHP > -1 ? inHP : 0;
    if (m_nHP > 560) m_nHP = 560;
    changeTimeHP(m_pSeqCtrl, 3, inHP);
    m_tqRed = 1500;
    if (inFlag || m_nRedHP < m_nHP) {
        m_nRedHP = m_nHP;
        changeTimeHP(m_pSeqCtrl, 2, m_nRedHP);
        m_tqRed = 0;
    }
    if (inFlag) {
        INT max = m_nMaxHP; // MATCH: signed division (USHORT / 4 is shortened to unsigned)
        if (m_nHP <= max / 4) {
            m_pSeqCtrl->replaceSeq(0, g_szSeqGauge, 0xA);
            m_pSeqCtrl->replaceSeq(4, m_szSeqFace, 0);
        }
    }
}
void CPlayerGauge::setDamage(INT inHP, INT inKind) {
    setHP(inHP, 0);
    if (CGameWork::getInt(1, 0xE2) & 1) return;
    m_iState = 3;
    m_tqTimer = 0;
    switch (inKind) {
    case 1:
    case 40:
    default:
        m_pSeqCtrl->replaceSeq(0, g_szSeqGauge, 0xD);
        break;
    case 2:
    case 4:
    case 5:
    case 6:
        m_pSeqCtrl->replaceSeq(0, g_szSeqGauge, 0xC);
        break;
    case 3:
    case 12:
    case 17:
    case 24:
    case 27:
    case 31:
    case 32:
    case 33:
    case 34:
    case 35:
    case 39:
        m_pSeqCtrl->replaceSeq(0, g_szSeqGauge, 0xB);
        break;
    }
    m_pSeqCtrl->replaceSeq(4, m_szSeqFace, 1);
}
void CPlayerGauge::setBattleEnd() { CNaviMap::m_bBattleEnd = TRUE; }
void CPlayerGauge::setDPGauge(INT inState) {
    INT i;
    INT dark;
    D2::C2DSeqCtrl* pSeq;
    INT gauge;
    if (!m_pSeqCtrlDP) return;
    if (!(m_uiFlag & 4)) return;
    if (inState == m_nStateDP) return;
    dark = FALSE;
    if (m_uiFlag & 8) dark = TRUE;
    switch (inState) {
    case 0:
    case 2:
    case 3:
    case 4:
    case 5:
    case 6:
        break;
    default:
        for (i = 0; i < 3; i++) m_pSeqCtrlDP[i].destroy();
        break;
    }
    m_uiFlag = (INT)m_uiFlag & ~0x10;
    if (m_nMaxDP) gauge = m_nDP * 100 / m_nMaxDP;
    else gauge = 0;
    if (gauge > 100) gauge = 100;
    switch (inState) {
    case 2:
    case 3:
    case 4:
    case 5:
    case 6:
    case 7:
        break;
    case 1:
    case 8:
    case 9:
        pSeq = m_pSeqCtrlDP;
        pSeq->setLayer(2);
        pSeq->addSeq(g_szSeqGauge, g_RikuSeqIDData[dark][0]);
        pSeq->addSeq(g_szSeqGauge, g_RikuSeqIDData[dark][1]);
        if (gauge == 100 && !dark && inState != 9) {
            pSeq->addSeq(g_szSeqGauge, 0x1D);
            m_uiFlag |= 0x10;
        }
        pSeq->setParent(m_pSeqCtrl);
        pSeq->enableColorCtrl(TRUE);
        pSeq->changeTime(1, gauge);
        pSeq = &m_pSeqCtrlDP[1];
        pSeq->setLayer(2);
        pSeq->addSeq(g_szSeqGauge, 0x2A);
        pSeq->addSeq(g_szSeqGauge, g_RikuSeqIDData[dark][2]);
        pSeq->setParent(m_pSeqCtrlDP);
        pSeq->enableColorCtrl(TRUE);
        i = m_nDP % 10; // MATCH: i reused for the digit (as gauge in setDP)
        pSeq->changeTexture(1, 0, NULL, &g_uvNumberTex[0][i], &g_uvNumberTex[1][i]);
        pSeq = &m_pSeqCtrlDP[2];
        pSeq->setLayer(2);
        pSeq->addSeq(g_szSeqGauge, 0x2B);
        pSeq->addSeq(g_szSeqGauge, g_RikuSeqIDData[dark][2]);
        pSeq->setParent(m_pSeqCtrlDP);
        pSeq->enableColorCtrl(TRUE);
        i = m_nDP / 10 % 10;
        pSeq->changeTexture(1, 0, NULL, &g_uvNumberTex[0][i], &g_uvNumberTex[1][i]);
        break;
    }
    m_nStateDP = inState;
}
void CPlayerGauge::setDP(INT inDP, INT inMax) {
    INT gauge;
    D2::C2DSeqCtrl* pSeq;
    if (!m_pSeqCtrlDP) return;
    if (!(m_uiFlag & 4)) return;
    if (m_nDP == inDP && m_nMaxDP == inMax) return;
    m_nMaxDP = inMax;
    if (m_nMaxDP > 99) m_nMaxDP = 99;
    m_nDP = inDP > -1 ? inDP : 0;
    if (m_nDP > m_nMaxDP) m_nDP = m_nMaxDP;
    if (m_nMaxDP) gauge = m_nDP * 100 / m_nMaxDP;
    else gauge = 0;
    if (gauge > 100) gauge = 100;
    pSeq = m_pSeqCtrlDP;
    pSeq->changeTime(1, gauge);
    if (gauge == 100 && !(m_uiFlag & 8)) {
        if (!(m_uiFlag & 0x10)) {
            pSeq->addSeq(g_szSeqGauge, 0x1D);
            m_uiFlag |= 0x10;
        }
    } else if (m_uiFlag & 0x10) {
        pSeq->destroyIndexToLast(2);
        m_uiFlag = (INT)m_uiFlag & ~0x10;
    }
    gauge = m_nDP % 10; // MATCH: gauge reused for the digits
    m_pSeqCtrlDP[1].changeTexture(1, 0, NULL, &g_uvNumberTex[0][gauge], &g_uvNumberTex[1][gauge]);
    gauge = m_nDP / 10 % 10;
    m_pSeqCtrlDP[2].changeTexture(1, 0, NULL, &g_uvNumberTex[0][gauge], &g_uvNumberTex[1][gauge]);
}
void CPlayerGauge::setDMode(INT inMode, INT inDP, INT inMax) {
    D2::C2DSeqCtrl* pSeqDP = m_pSeqCtrlDP;
    if (!pSeqDP) return;
    if (!(m_uiFlag & 4)) return;
    if (inMode) {
        *(UINT*)&m_uiFlag |= 8; // MATCH: not a struct-field store (stored before the face/HP loads)
        m_szSeqFace = g_szSeqDFace;
        INT max = m_nMaxHP; // MATCH: signed /4 (as setHP)
        if (m_nHP <= max / 4) {
            m_pSeqCtrl->replaceSeq(0, g_szSeqGauge, 0xA);
            m_pSeqCtrl->replaceSeq(4, m_szSeqFace, 0);
            m_iState = 4;
        } else {
            m_pSeqCtrl->replaceSeq(0, g_szSeqGauge, 0xF);
            m_pSeqCtrl->replaceSeq(4, m_szSeqFace, 3);
            m_iState = 2;
        }
        if (!(CGameWork::getInt(1, 0xE2) & 1)) m_pSeqCtrl->addSeq(g_szSeqGauge, 0x2E);
        setDPGauge(8);
        setDP(inDP, inMax);
        if (!(CGameWork::getInt(1, 0xE2) & 1)) pSeqDP->addSeq(g_szSeqGauge, 0x2C);
    } else {
        *(INT*)&m_uiFlag = (INT)m_uiFlag & ~8;
        m_szSeqFace = g_szSeqFace;
        INT max = m_nMaxHP;
        if (m_nHP <= max / 4) {
            m_pSeqCtrl->replaceSeq(0, g_szSeqGauge, 0xA);
            m_pSeqCtrl->replaceSeq(4, m_szSeqFace, 0);
            m_iState = 4;
        } else {
            m_pSeqCtrl->replaceSeq(0, g_szSeqGauge, 0xF);
            m_pSeqCtrl->replaceSeq(4, m_szSeqFace, 3);
            m_iState = 2;
        }
        if (!(CGameWork::getInt(1, 0xE2) & 1)) m_pSeqCtrl->replaceSeq(5, g_szSeqGauge, 0x2E);
        setDPGauge(9);
        setDP(inDP, inMax);
        if (!(CGameWork::getInt(1, 0xE2) & 1)) pSeqDP->addSeq(g_szSeqGauge, 0x2D);
    }
}
void CPlayerGauge::exeDPGauge() {
    if (!m_pSeqCtrlDP) return;
    if (!(m_uiFlag & 4)) {
        if (CGame::isRikuGame() && (CGameWork::getInt(0, 0x24) & 8)) m_uiFlag |= 4;
    } else {
        INT n = m_nStateDP;
        if (n >= 10) return;
        if (n >= 8 && m_pSeqCtrl->isEnd(5)) {
            m_pSeqCtrl->destroyIndexToLast(5);
            m_pSeqCtrlDP->destroyIndexToLast(2);
            setDPGauge(2);
        }
    }
}
CLockonMarker::CLockonMarker(CTask* inParent, BOOL inFlag) : CTask(inParent, 0x4000), m_pGauge(NULL), m_bFlag(inFlag) { _initialize(); }
CLockonMarker::~CLockonMarker() {
    delete m_pSeqCtrlPush;
    m_pSeqCtrlPush = NULL;
    delete m_pSeqCtrl;
    m_pSeqCtrl = NULL;
    delete m_pGauge;
    m_pGauge = NULL;
}
void CLockonMarker::_initialize() {
    m_nPreSeqNo = 99;
    m_nSeqNo = 99;
    m_uiFlag = 0;
    m_hTarget = 0;
    m_hLockon = 0;
    m_hHoldTarget = 0;
    m_iDist = 0;
    m_iMode = 0;
    m_bTarget = FALSE;
    m_bLockon = FALSE;
    m_bDisp = FALSE;
    m_bTrg = FALSE;
    m_fDist = 0.0f;
    m_fScale = 1.0f;
    m_vPos.init();
    m_vUseCardPos.init();
    m_vCameraTargetPos.init();

    m_uiKind = 0;
    m_pSeqCtrl = NULL;
    m_pSeqCtrl = new (__FILE__ "(850)") D2::C2DSeqCtrl(2);
    m_pSeqCtrl->setLayer(2);
    m_pSeqCtrl->addSeq(g_szSeqCockpit, m_nSeqNo);
    m_pSeqCtrl->enableDisp(0, FALSE);
    m_pSeqCtrlPush = NULL;
    m_pSeqCtrlPush = new (__FILE__ "(856)") D2::C2DSeqCtrl(2);
    m_vDispPos.init();
    if (CSceneInfo::isBattleScene() || CSceneInfo::isMinigameScene()) {
        m_uiFlag |= 4;
        m_pGauge = new (__FILE__ "(865)") CEnemyGauge(this);
    }
}
void CLockonMarker::main() {
    CEntity* pEntity;
    if (CSceneEnv::isStopGauge()) return;
    if (!m_pSeqCtrl) return;
    pEntity = NULL;
    if (m_bFlag) {
        m_bDisp = FALSE;
        if (m_hTarget > 0) {
            pEntity = (CEntity*)CTaskMan::getTask(m_hTarget);
            if (pEntity) {
                pEntity->getLockOnPos(m_vPos);
                m_bDisp = TRUE;
            } else {
                m_hTarget = 0;
                m_uiFlag = (INT)m_uiFlag & ~0x10;
                m_bTarget = FALSE;
                m_bLockon = FALSE;
            }
        }
        if (!m_bDisp) {
            if (m_pGauge) CEnemyGauge::resetHP();
        } else {
            if (m_bLockon) m_nSeqNo = 0x62;
            else if (m_bTarget) m_nSeqNo = (m_uiFlag & 0x10) ? 0x92 : 0x63;
            else goto hide;
            if (m_uiFlag & 1) {
                m_uiFlag = (INT)m_uiFlag & ~1;
                m_pSeqCtrl->enableDisp(0, TRUE);
                m_pSeqCtrl->restart(0, 0.0f);
            }
            if (m_nSeqNo != m_nPreSeqNo) m_pSeqCtrl->replaceSeq(0, g_szSeqCockpit, m_nSeqNo);
            m_pSeqCtrl->set3DPos(m_vPos);
            calcMarkerScale();
            if (m_pGauge && pEntity->getTypeID() == 0x1004) {
                CBattleEnemy* pEnemy = (CBattleEnemy*)CTaskMan::getTask(m_hTarget);
                CEnemyGauge::setHP(m_hTarget, pEnemy->getHp(), pEnemy->getHpMax());
            }
            m_nPreSeqNo = m_nSeqNo;
            if (m_uiKind != 6) goto push;
        }
    }
hide:
    if ((m_uiFlag & 1) == 0) {
        m_uiFlag |= 1;
        m_pSeqCtrl->enableDisp(0, FALSE);
    }
push:
    if (m_uiFlag & 2) m_pSeqCtrlPush->set3DPos(m_vPos);
    if (m_uiFlag & 0x20) {
        if (m_uiFlag & 0x100) {
            m_uiFlag = (INT)m_uiFlag & ~0x100;
            m_pSeqCtrlPush->addSeq(_ref(g_szSeqCockpit), 0x91);
        }
        if (m_pSeqCtrlPush->isEnd(0)) {
            m_uiFlag = (INT)m_uiFlag & ~0x20;
            m_pSeqCtrlPush->destroy();
        }
    }
}
void CLockonMarker::calcMarkerScale() {
    FVECTOR pos = getPlayerPos(FALSE);
    FLOAT dist;
    FLOAT old_sc;
    FVECTOR v;
    dist = m_fDist;
    if (PlayerCamera::CPlayerCamera::getPlayerCameraPos(pos)) {
        SubVectorXYZ(v, pos, m_vPos);
        dist = _sqrtf(InnerProduct(v, v));
    }
    old_sc = m_fScale;
    if (dist < 3.5f) m_fScale = 1.0f;
    else if (dist < 14.0f) m_fScale = (14.0f - dist) / 10.5f * 0.5f + 0.5f;
    else m_fScale = 0.5f;
    if (m_fScale != old_sc) {
        m_pSeqCtrl->setScl(m_fScale);
        if (m_uiFlag & 2) m_pSeqCtrlPush->setScl(m_fScale);
    }
}
void CLockonMarker::setTarget(HANDLE inTarget) {
    m_hTarget = inTarget;
    m_bLockon = FALSE;
    m_bTarget = FALSE;
    m_iDist = 0;
    if (!inTarget && (m_uiFlag & 1)) {
        m_uiFlag = (INT)m_uiFlag & ~1;
        m_pSeqCtrl->enableDisp(0, TRUE);
        m_pSeqCtrl->restart(0, 0.0f);
    }
    m_uiFlag = (INT)m_uiFlag & ~0x10;
    if (m_hTarget > 0) {
        CEntity* pTarget = (CEntity*)CTaskMan::getTask(m_hTarget);
        if (pTarget) {
            switch (pTarget->getTypeID()) {
            case 0x1006:
            case 0x2000:
            case 0x2003:
            case 0x2004:
            case 0x2005:
            case 0x4000:
                if (!(m_uiFlag & 0x80)) m_uiFlag |= 0x10;
                break;
            }
            m_bTarget = TRUE;
        }
    }
}
void CLockonMarker::setLockon(HANDLE inTarget, INT inDist) {
    m_iDist = inDist;
    m_hTarget = inTarget;
    m_uiFlag = (INT)m_uiFlag & ~0x10;
    m_bTarget = FALSE;
    m_bLockon = FALSE;
    if (inTarget > 0 && CTaskMan::getTask(inTarget)) m_bLockon = TRUE;
}
void CLockonMarker::chkLockon(HANDLE& inTarget) {
    CEntity* pTargetEntity;
    FVECTOR v;
    FLOAT dist;
    pTargetEntity = NULL;
    if (m_hHoldTarget > 0) {
        pTargetEntity = (CEntity*)CTaskMan::getTask(m_hHoldTarget);
        if (pTargetEntity && pTargetEntity->isPermissionLockOn()) {
            inTarget = m_hHoldTarget;
        } else {
            m_hHoldTarget = 0;
            pTargetEntity = NULL;
        }
    }
    if (m_iMode == 0) {
        if (!pTargetEntity) {
            if (inTarget > 0) pTargetEntity = (CEntity*)CTaskMan::getTask(inTarget);
            if (m_uiFlag & 8) m_hHoldTarget = inTarget;
            if (!pTargetEntity) goto reset;
        }
        pTargetEntity->getLockOnPos(m_vPos);
        SubVectorXYZ(v, getPlayerPos(FALSE), m_vPos);
        m_fDist = _sqrtf(InnerProduct(v, v));
        if (m_uiFlag & 4) dist = 14.0f;
        else if (m_uiFlag & 0x80) dist = 14.0f;
        else dist = 3.0f;
        if (m_fDist > dist) goto reset;
        if (!pTargetEntity->isPermissionLockOn()) goto reset;
        setTarget(inTarget);
    } else {
        if (m_hLockon > 0) {
            pTargetEntity = (CEntity*)CTaskMan::getTask(m_hLockon);
            if (m_uiFlag & 8) m_hHoldTarget = m_hLockon;
            if (!pTargetEntity || !pTargetEntity->isPermissionLockOn()) {
                if (m_uiFlag & 2) {
                    m_hLockon = 0;
                } else {
                    setTarget(0);
                    inTarget = 0;
                    m_hLockon = 0;
                    m_iMode = 0;
                    m_hHoldTarget = 0;
                }
            }
        } else {
            m_hLockon = inTarget;
            if (!pTargetEntity) {
                if (m_hLockon > 0) pTargetEntity = (CEntity*)CTaskMan::getTask(m_hLockon);
                if (m_uiFlag & 8) m_hHoldTarget = m_hLockon;
            }
            setLockon(m_hLockon, 0);
        }
        if (pTargetEntity) {
            pTargetEntity->getLockOnPos(m_vPos);
            pTargetEntity->getUseCardPos(m_vUseCardPos);
            pTargetEntity->getCameraTargetPos(m_vCameraTargetPos);
            SubVectorXYZ(v, getPlayerPos(FALSE), m_vPos);
            dist = _sqrtf(InnerProduct(v, v));
            m_fDist = dist;
            if (dist > ((m_uiFlag & 4) ? 30.0f : 3.0f) || (dist > 14.0f && m_bTrg)) {
            reset:
                setTarget(0);
                inTarget = 0;
                m_hLockon = 0;
                m_iMode = 0;
                m_hHoldTarget = 0;
            } else if (m_fDist > 7.0f) {
                m_iDist = 2;
            } else if (m_fDist > 3.5f) {
                m_iDist = 1;
            } else {
                m_iDist = 0;
            }
        }
    }
    m_bTrg = FALSE;
    m_uiFlag = (INT)m_uiFlag & ~0x80;
}
void CLockonMarker::cancelLockon(HANDLE& ioHandle, INT inFlag) {
    if (!inFlag) {
        m_hTarget = 0;
        m_bLockon = FALSE;
        m_bTarget = FALSE;
        m_uiFlag = (INT)m_uiFlag & ~0x10;
        m_uiFlag = (INT)m_uiFlag & ~1;
        m_iDist = 0;
        ioHandle = 0;
        m_hLockon = 0;
        m_iMode = 0;
        m_bTrg = FALSE;
        if (m_pGauge) CEnemyGauge::resetHP();
        holdTarget(0);
        offPush();
    } else if (m_uiKind != 6) {
        m_hTarget = 0;
        m_bLockon = FALSE;
        m_bTarget = FALSE;
        m_uiFlag = (INT)m_uiFlag & ~0x10;
        m_uiFlag = (INT)m_uiFlag & ~1;
        m_iDist = 0;
        ioHandle = 0;
        m_hLockon = 0;
        m_iMode = 0;
        m_bTrg = FALSE;
        if (m_pGauge) CEnemyGauge::resetHP();
        holdTarget(0);
        offPush();
    }
}
void CLockonMarker::onPush(INT inKind) {
    if (m_uiFlag & 2) return;
    if (m_iMode != 1) changeMode();
    m_uiKind = inKind;
    m_uiFlag |= 2;
    switch (inKind) {
    case 1:
        m_pSeqCtrlPush->addSeq(g_szSeqCockpit, 0x65);
        break;
    case 2:
        m_pSeqCtrlPush->addSeq(g_szSeqCockpit, 0x7B);
        break;
    case 3:
        m_pSeqCtrlPush->addSeq(g_szSeqCockpit, 0x7A);
        break;
    case 4:
        m_pSeqCtrlPush->addSeq(g_szSeqCockpit, 0x92);
        break;
    case 5:
        break;
    case 6:
        m_pSeqCtrlPush->addSeq(g_szSeqCockpit, 0x90);
        break;
    case 7:
        m_pSeqCtrlPush->addSeq(g_szSeqCockpit, 0x94);
        break;
    }
}
void CLockonMarker::offPush() {
    if (m_uiFlag & 2) {
        UINT f = m_uiFlag;
        m_uiFlag = (INT)f & ~2;
        if (!(f & 0x20)) {
            m_pSeqCtrlPush->destroy();
            if (m_uiKind == 6) m_uiFlag |= 0x120;
        }
        m_uiKind = 0;
    }
}
void CLockonMarker::holdTarget(INT inFlag) {
    if (inFlag) {
        m_uiFlag |= 8;
        m_hHoldTarget = m_hTarget;
    } else {
        m_hHoldTarget = 0;
        m_uiFlag = (INT)m_uiFlag & ~8;
    }
}
CEnemyGauge::CEnemyGauge(CTask* p) : CTask(p, 0x4000) {
    m_pInstance = this;
    _initialize();
}
CEnemyGauge::~CEnemyGauge() {
    delete m_pSeqCtrl;
    m_pSeqCtrl = NULL;
    m_pInstance = NULL;
}
void CEnemyGauge::_initialize() {
    m_nMaxHP = 200;
    m_nRedHP = 200;
    m_nHP = 200;
    m_nMaxHPS = 15;
    m_nRedHPS = 15;
    m_nHPS = 15;
    m_uiFlag = 0;
    m_hTarget = 0;
    m_tqRed = 0;
    m_pSeqCtrl = NULL;
    m_pSeqCtrl = new (__FILE__ "(1484)") D2::C2DSeqCtrl(1);
    m_pSeqCtrl->setLayer(2);
    m_pSeqCtrl->addSeq(g_szLadGauge, 1, 2);
    m_pSeqCtrl->addSeq(g_szLadGauge, 1, 0);
    m_pSeqCtrl->addSeq(g_szLadGauge, 1, 1);
    m_pSeqCtrl->addSeq(g_szLadGauge, 1, 3);
    m_pSeqCtrl->addSeq(g_szLadGauge, 1, 4);
    m_pSeqCtrl->addSeq(g_szLadGauge, 1, 5);
    m_pSeqCtrl->addSeq(g_szLadGauge, 1, 6);
    m_pSeqCtrl->changeTime(1, m_nMaxHP);
    m_pSeqCtrl->changeTime(2, m_nMaxHP);
    m_pSeqCtrl->changeTime(3, m_nMaxHP);
    m_pSeqCtrl->changeTime(4, m_nMaxHPS);
    m_pSeqCtrl->changeTime(5, m_nMaxHPS);
    m_pSeqCtrl->changeTime(6, m_nMaxHPS);
    m_pSeqCtrl->enableDispEach(0);
    m_vPos.set(240.0f, -40.0f, 0.0f);
}
void CEnemyGauge::main() {
    if (CSceneEnv::isStopGauge()) return;
    if ((CGameWork::getInt(1, 0xE2) & 0x10) || m_hTarget <= 0) {
        resetHP();
        return;
    }
    if (m_uiFlag & 1) {
        m_uiFlag = (INT)m_uiFlag & ~1;
        m_pSeqCtrl->enableDisp(1, TRUE);
        m_pSeqCtrl->enableDisp(3, TRUE);
        if (m_uiFlag & 4) {
            m_pSeqCtrl->enableDisp(4, TRUE);
            m_pSeqCtrl->enableDisp(6, TRUE);
        }
        if (m_uiFlag & 2) m_pSeqCtrl->enableDisp(0, TRUE);
    }
    if (!(m_uiFlag & 0x20)) {
        INT flag = 0;
        m_vPos.x -= getFrameRateF() * 8.0f;
        if (m_vPos.x < 0.0f) {
            m_vPos.x = 0.0f;
            flag = 1;
        }
        m_vPos.y += getFrameRateF() * 8.0f;
        if (0.0f < m_vPos.y) {
            m_vPos.y = 0.0f;
            flag++;
        }
        m_pSeqCtrl->setPos(m_vPos);
        if (flag == 2) m_uiFlag |= 0x20;
    }
    if (m_nRedHP != m_nHP) {
        if (!(m_uiFlag & 8)) {
            m_uiFlag |= 8;
            m_pSeqCtrl->enableDisp(2, TRUE);
        }
        if (m_nRedHP < m_nHP) {
            m_nRedHP = m_nHP;
        } else if (m_tqRed <= 0) {
            m_tqRed = 0;
            m_nRedHP -= (m_nRedHP - m_nHP) / 10 + 1;
        }
        if (m_nMaxHPS > 0) {
            if (!(m_uiFlag & 0x10)) {
                m_uiFlag |= 0x10;
                m_pSeqCtrl->enableDisp(5, TRUE);
            }
            m_nRedHPS = (m_nRedHP - 1) / 200;
            if (m_nRedHPS < 0) m_nRedHPS = 0;
            if (m_nRedHPS > m_nHPS) m_pSeqCtrl->changeTime(2, 200);
            else m_pSeqCtrl->changeTime(2, m_nRedHP - m_nRedHPS * 200);
            m_pSeqCtrl->changeTime(5, m_nRedHPS);
        } else {
            m_pSeqCtrl->changeTime(2, m_nRedHP);
        }
    } else {
        if (m_uiFlag & 8) {
            m_uiFlag = (INT)m_uiFlag & ~8;
            m_pSeqCtrl->enableDisp(2, FALSE);
        }
        if (m_uiFlag & 0x10) {
            m_uiFlag = (INT)m_uiFlag & ~0x10;
            m_pSeqCtrl->enableDisp(5, FALSE);
        }
    }
    m_tqRed -= getFrameRate();
}
void CEnemyGauge::resetHP() {
    if (!m_pInstance) return;
    if ((m_pInstance->m_uiFlag & 1) == 0) {
        *(UINT*)&m_pInstance->m_uiFlag = (INT)(m_pInstance->m_uiFlag | 1) & ~0x1E; // MATCH: not a struct-field store, so m_pInstance is reloaded after it
        m_pInstance->m_pSeqCtrl->enableDispEach(0);
    }
    m_pInstance->m_hTarget = 0;
}
/* MATCH: _ref(m_pInstance) - the original reloads m_pInstance after every store */
void CEnemyGauge::setHP(HANDLE inHandle, INT inHP, INT inMaxHP) {
    if (!m_pInstance) return;
    if (CGameWork::getInt(1, 0xE2) & 0x10) return;
    if (_ref(m_pInstance)->m_hTarget == inHandle) return;
    _ref(m_pInstance)->m_hTarget = inHandle;
    _ref(m_pInstance)->m_nMaxHP = inMaxHP;
    _ref(m_pInstance)->m_nRedHP = _ref(m_pInstance)->m_nHP = inHP > -1 ? inHP : 0;
    if (_ref(m_pInstance)->m_nHP > _ref(m_pInstance)->m_nMaxHP) _ref(m_pInstance)->m_nRedHP = _ref(m_pInstance)->m_nHP = _ref(m_pInstance)->m_nMaxHP;
    _ref(m_pInstance)->m_nMaxHPS = (_ref(m_pInstance)->m_nMaxHP - 1) / 200;
    if (_ref(m_pInstance)->m_nMaxHPS < 0) _ref(m_pInstance)->m_nMaxHPS = 0;
    _ref(m_pInstance)->m_nHPS = (_ref(m_pInstance)->m_nHP - 1) / 200;
    if (_ref(m_pInstance)->m_nHPS < 0) _ref(m_pInstance)->m_nHPS = 0;
    _ref(m_pInstance)->m_nRedHPS = _ref(m_pInstance)->m_nHPS;
    _ref(m_pInstance)->m_uiFlag = (INT)_ref(m_pInstance)->m_uiFlag & ~2;
    _ref(m_pInstance)->m_uiFlag = (INT)_ref(m_pInstance)->m_uiFlag & ~4;
    if (_ref(m_pInstance)->m_nMaxHPS > 0) {
        _ref(m_pInstance)->m_uiFlag |= 4;
        if (_ref(m_pInstance)->m_nMaxHPS > 15) _ref(m_pInstance)->m_nMaxHPS = 15;
        if (_ref(m_pInstance)->m_nHPS > 15) _ref(m_pInstance)->m_nHPS = 15;
        _ref(m_pInstance)->m_pSeqCtrl->changeTime(4, _ref(m_pInstance)->m_nMaxHPS);
        _ref(m_pInstance)->m_pSeqCtrl->changeTime(5, _ref(m_pInstance)->m_nRedHPS);
        _ref(m_pInstance)->m_pSeqCtrl->changeTime(6, _ref(m_pInstance)->m_nHPS);
        _ref(m_pInstance)->m_pSeqCtrl->enableDisp(4, TRUE);
        _ref(m_pInstance)->m_pSeqCtrl->enableDisp(5, TRUE);
        _ref(m_pInstance)->m_pSeqCtrl->enableDisp(6, TRUE);
        if (_ref(m_pInstance)->m_nHPS == _ref(m_pInstance)->m_nMaxHPS) {
            _ref(m_pInstance)->m_uiFlag |= 2;
            _ref(m_pInstance)->m_pSeqCtrl->enableDisp(0, TRUE);
            _ref(m_pInstance)->m_pSeqCtrl->changeTime(1, _ref(m_pInstance)->m_nMaxHP - _ref(m_pInstance)->m_nMaxHPS * 200);
            _ref(m_pInstance)->m_pSeqCtrl->enableDisp(1, TRUE);
        } else {
            _ref(m_pInstance)->m_pSeqCtrl->enableDisp(0, FALSE);
            _ref(m_pInstance)->m_pSeqCtrl->changeTime(1, 200);
            _ref(m_pInstance)->m_pSeqCtrl->enableDisp(1, TRUE);
        }
        _ref(m_pInstance)->m_pSeqCtrl->changeTime(2, _ref(m_pInstance)->m_nRedHP - _ref(m_pInstance)->m_nRedHPS * 200);
        _ref(m_pInstance)->m_pSeqCtrl->enableDisp(2, FALSE);
        _ref(m_pInstance)->m_pSeqCtrl->changeTime(3, _ref(m_pInstance)->m_nHP - _ref(m_pInstance)->m_nHPS * 200);
        _ref(m_pInstance)->m_pSeqCtrl->enableDisp(3, TRUE);
    } else {
        _ref(m_pInstance)->m_pSeqCtrl->changeTime(1, _ref(m_pInstance)->m_nMaxHP);
        _ref(m_pInstance)->m_pSeqCtrl->changeTime(2, _ref(m_pInstance)->m_nRedHP);
        _ref(m_pInstance)->m_pSeqCtrl->changeTime(3, _ref(m_pInstance)->m_nHP);
        _ref(m_pInstance)->m_pSeqCtrl->enableDisp(1, TRUE);
        _ref(m_pInstance)->m_pSeqCtrl->enableDisp(2, FALSE);
        _ref(m_pInstance)->m_pSeqCtrl->enableDisp(3, TRUE);
        _ref(m_pInstance)->m_pSeqCtrl->enableDisp(0, FALSE);
        _ref(m_pInstance)->m_pSeqCtrl->enableDisp(4, FALSE);
        _ref(m_pInstance)->m_pSeqCtrl->enableDisp(5, FALSE);
        _ref(m_pInstance)->m_pSeqCtrl->enableDisp(6, FALSE);
    }
}
/* MATCH: _ref(m_pInstance) as setHP */
void CEnemyGauge::notifyHP(HANDLE inHandle, INT inHP, INT inMaxHP) {
    D2::C2DSeqCtrl* pSeqCtrl;
    if (!m_pInstance) return;
    if (_ref(m_pInstance)->m_hTarget != inHandle) return;
    if (CGameWork::getInt(1, 0xE2) & 0x10) return;
    _ref(m_pInstance)->m_uiFlag = (INT)_ref(m_pInstance)->m_uiFlag & ~2;
    _ref(m_pInstance)->m_uiFlag = (INT)_ref(m_pInstance)->m_uiFlag & ~4;
    _ref(m_pInstance)->m_uiFlag = (INT)_ref(m_pInstance)->m_uiFlag & ~8;
    pSeqCtrl = _ref(m_pInstance)->m_pSeqCtrl;
    pSeqCtrl->enableDisp(0, FALSE);
    pSeqCtrl->enableDisp(1, FALSE);
    pSeqCtrl->enableDisp(2, FALSE);
    pSeqCtrl->enableDisp(3, FALSE);
    pSeqCtrl->enableDisp(4, FALSE);
    pSeqCtrl->enableDisp(5, FALSE);
    pSeqCtrl->enableDisp(6, FALSE);
    _ref(m_pInstance)->m_nHP = inHP > -1 ? inHP : 0;
    if (_ref(m_pInstance)->m_nHP > _ref(m_pInstance)->m_nMaxHP)
        _ref(m_pInstance)->m_nRedHP = _ref(m_pInstance)->m_nHP = _ref(m_pInstance)->m_nMaxHP;
    _ref(m_pInstance)->m_nHPS = (_ref(m_pInstance)->m_nHP - 1) / 200;
    if (_ref(m_pInstance)->m_nHPS < 0) _ref(m_pInstance)->m_nHPS = 0;
    pSeqCtrl->changeTime(3, _ref(m_pInstance)->m_nHP - _ref(m_pInstance)->m_nHPS * 200);
    pSeqCtrl->enableDisp(3, TRUE);
    _ref(m_pInstance)->m_tqRed = 1500;
    if (_ref(m_pInstance)->m_nRedHP < _ref(m_pInstance)->m_nHP) {
        _ref(m_pInstance)->m_nRedHP = _ref(m_pInstance)->m_nHP;
        _ref(m_pInstance)->m_nRedHPS = (_ref(m_pInstance)->m_nRedHP - 1) / 200;
        if (_ref(m_pInstance)->m_nRedHPS < 0) _ref(m_pInstance)->m_nRedHPS = 0;
        pSeqCtrl->changeTime(2, _ref(m_pInstance)->m_nRedHP - _ref(m_pInstance)->m_nRedHPS * 200);
        _ref(m_pInstance)->m_tqRed = 0;
    }
    if (_ref(m_pInstance)->m_nMaxHPS > 0) {
        _ref(m_pInstance)->m_uiFlag |= 4;
        pSeqCtrl->changeTime(6, _ref(m_pInstance)->m_nHPS);
        pSeqCtrl->enableDisp(4, TRUE);
        pSeqCtrl->enableDisp(5, TRUE);
        pSeqCtrl->enableDisp(6, TRUE);
        if (_ref(m_pInstance)->m_nHPS == _ref(m_pInstance)->m_nMaxHPS) {
            _ref(m_pInstance)->m_uiFlag |= 2;
            pSeqCtrl->enableDisp(0, TRUE);
            pSeqCtrl->changeTime(1, _ref(m_pInstance)->m_nMaxHP - _ref(m_pInstance)->m_nMaxHPS * 200);
            pSeqCtrl->enableDisp(1, TRUE);
        } else {
            pSeqCtrl->enableDisp(0, FALSE);
            pSeqCtrl->changeTime(1, 200);
            pSeqCtrl->enableDisp(1, TRUE);
        }
        if (_ref(m_pInstance)->m_nRedHPS > _ref(m_pInstance)->m_nHPS) {
            pSeqCtrl->changeTime(2, 200);
            pSeqCtrl->enableDisp(2, TRUE);
        } else {
            pSeqCtrl->changeTime(2, _ref(m_pInstance)->m_nRedHP - _ref(m_pInstance)->m_nRedHPS * 200);
        }
    } else {
        pSeqCtrl->enableDisp(1, TRUE);
        pSeqCtrl->enableDisp(3, TRUE);
    }
}
HANDLE CEnemyGauge::getNowTarget() {
    if (!m_pInstance) return 0;
    if (CGameWork::getInt(1, 0xe2) & 0x10) return 0;
    return m_pInstance->m_hTarget;
}
CCommonGauge::CCommonGauge(CTask* p) : CTask(p, 0x4000) {
    m_pInstance = this;
    _initialize();
}
CCommonGauge::~CCommonGauge() {
    delete[] m_pSeqCtrls;
    m_pSeqCtrls = NULL;
    m_pInstance = NULL;
}
void CCommonGauge::_initialize() {
    INT i;
    m_uiFlag = 0;
    m_pSeqCtrls = NULL;
    m_pSeqCtrls = new (__FILE__ "(1901)") D2::C2DSeqCtrl[7];
    m_pSeqCtrls[0].addSeq(_ref(g_szSeqCockpit), 0xD);
    m_pSeqCtrls[1].addSeq(g_szSeqCockpit, 0xD);
    m_pSeqCtrls[2].addSeq(g_szSeqCockpit, 0xC);
    m_pSeqCtrls[3].addSeq(g_szSeqCockpit, 0xC);
    m_pSeqCtrls[4].addSeq(g_szSeqCockpit, 0x3D);
    m_pSeqCtrls[5].addSeq(g_szSeqCockpit, 0x8F);
    m_pSeqCtrls[6].addSeq(g_szSeqCockpit, 0x8F);
    for (i = 0; i < 7; i++) {
        m_pSeqCtrls[i].changeLayer(2);
        m_pSeqCtrls[i].enableDisp(0, FALSE);
        m_vPos[i].init();
    }
}
void CCommonGauge::main() {
    INT i;
    if (CSceneEnv::isStopGauge()) return;
    if (CPlayer::isNowExterminate()) {
        if (m_uiFlag & 0xCF6F) {
            for (i = 0; i < 7; i++) {
                if (i == 4) continue;
                if (m_uiFlag & (1 << i)) {
                    m_pSeqCtrls[i].restart(0, 0.0f);
                    m_pSeqCtrls[i].enableDisp(0, FALSE);
                }
            }
            m_uiFlag = (m_uiFlag | 0x10000) & ~0xCF6F;
        }
        dispSeqCtrl(4, 0x10, 0x1000);
        return;
    }
    m_uiFlag &= ~0x10000;
    dispSeqCtrl(0, 1, 0x100);
    dispSeqCtrl(1, 2, 0x200);
    dispSeqCtrl(2, 4, 0x400);
    dispSeqCtrl(3, 8, 0x800);
    dispSeqCtrl(4, 0x10, 0x1000);
    dispSeqCtrl(5, 0x20, 0x4000);
    dispSeqCtrl(6, 0x40, 0x8000);
}
void CCommonGauge::dispSeqCtrl(INT inIndex, INT inFlag, INT inDisp) {
    D2::C2DSeqCtrl* pSeqCtrl;
    IVECTOR out;
    FVECTOR pos;
    if (!(m_uiFlag & inFlag)) return;
    pSeqCtrl = &m_pSeqCtrls[inIndex];
    if (pSeqCtrl->isEnd(0)) {
        m_uiFlag &= ~(inFlag | inDisp);
        pSeqCtrl->restart(0, 0.0f);
        pSeqCtrl->enableDisp(0, FALSE);
        return;
    }
    if (inFlag == 0x10 && !(m_uiFlag & 0x2000)) return;
    _setRotTransPersMatrix(CWorldEnv::getWorldScreenMatrix());
    RotTransPersClipW(out, m_vPos[inIndex]);
    if (out.w) {
        if (m_uiFlag & inDisp) {
            m_uiFlag &= ~inDisp;
            pSeqCtrl->enableDisp(0, FALSE);
        }
        return;
    }
    if (!(m_uiFlag & inDisp)) {
        m_uiFlag |= inDisp;
        pSeqCtrl->enableDisp(0, TRUE);
    }
    pos.set((out.x >> 4) - 0x700, (out.y >> 4) - 0x740, 0.0f, 1.0f);
    pSeqCtrl->setPos(pos);
    if (inFlag == 0x10) m_uiFlag = (INT)m_uiFlag & ~0x2000;
}
void CCommonGauge::setDispSeq(HANDLE inHandle, INT inIndex) {
    CEntity* pEntity;
    D2::C2DSeqCtrl* pSeqCtrl;
    HANDLE target;
    if (!m_pInstance) return;
    if (inIndex != 4 && CPlayer::isNowExterminate()) return;
    pEntity = (CEntity*)CTaskMan::getTask(inHandle);
    if (!pEntity) return;
    if (!PLM::isEntityTypeID(pEntity->getTypeID())) return;
    switch (inIndex) {
    case 1:
    case 3:
    case 6:
        target = CEnemyGauge::getNowTarget();
        if (target > 0 && target != inHandle) return;
        break;
    }
    switch (inIndex) {
    case 0:
        _ref(m_pInstance)->m_uiFlag = (INT)(_ref(m_pInstance)->m_uiFlag | 0x1) & ~0x100;
        break;
    case 2:
        _ref(m_pInstance)->m_uiFlag = (INT)(_ref(m_pInstance)->m_uiFlag | 0x4) & ~0x400;
        break;
    case 4:
        _ref(m_pInstance)->m_uiFlag = (INT)(_ref(m_pInstance)->m_uiFlag | 0x2010) & ~0x1000;
        break;
    case 5:
        _ref(m_pInstance)->m_uiFlag = (INT)(_ref(m_pInstance)->m_uiFlag | 0x20) & ~0x4000;
        break;
    case 1:
        _ref(m_pInstance)->m_uiFlag = (INT)(_ref(m_pInstance)->m_uiFlag | 0x2) & ~0x200;
        break;
    case 3:
        _ref(m_pInstance)->m_uiFlag = (INT)(_ref(m_pInstance)->m_uiFlag | 0x8) & ~0x800;
        break;
    case 6:
        _ref(m_pInstance)->m_uiFlag = (INT)(_ref(m_pInstance)->m_uiFlag | 0x40) & ~0x8000;
        break;
    default:
        return;
    }
    _ref(m_pInstance)->m_hOwner[inIndex] = inHandle;
    pEntity->getHeadPos(_ref(m_pInstance)->m_vPos[inIndex]);
    pSeqCtrl = &_ref(m_pInstance)->m_pSeqCtrls[inIndex];
    pSeqCtrl->restart(0, 0.0f);
    pSeqCtrl->enableDisp(0, FALSE);
}
CPrizeGauge::CPrizeGauge(CTask* p) : CTask(p ? p : getSysTask(1), 0x4000) {
    m_pInstance = this;
    if (p) m_uiMode = 1;
    else m_uiMode = 0;
    m_tqTimer = 0;
    _initialize();
}
CPrizeGauge::~CPrizeGauge() {
    delete[] m_pSeqCtrls;
    m_pSeqCtrls = NULL;
    delete[] m_pMogSeqCtrls;
    m_pMogSeqCtrls = NULL;
    delete[] m_pCardSeqCtrls;
    m_pCardSeqCtrls = NULL;
    delete m_pCard;
    m_pCard = NULL;
    m_pInstance = NULL;
}
void CPrizeGauge::_initialize() {
    INT i;
    FVECTOR pos;
    INT size;
    m_iFigure = 1;
    m_uiState = 0;
    m_iExp = 0;
    m_iOldExp = 0;
    m_pSeqCtrls = NULL;
    m_pSeqCtrls = new (__FILE__ "(2119)") D2::C2DSeqCtrl[6];
    size = 16;
    m_pSeqCtrls->setLayer(8);
    m_pSeqCtrls->addSeq(g_szSeqCockpit, 0x51);
    D2::C2DMan::getLayPosition(g_szLadCockpit, 0xA, 0, pos, FALSE);
    m_pSeqCtrls->setPos(pos);
    m_pSeqCtrls->enableDisp(0, FALSE);
    pos.x += 114.0f;
    pos.y += 20.0f;
    for (i = 1; i < 6; i++) {
        m_pSeqCtrls[i].setLayer(8);
        m_pSeqCtrls[i].addSeq(g_szSeqCockpit, 0x57);
        pos.x -= size;
        m_pSeqCtrls[i].setPos(pos);
        m_pSeqCtrls[i].enableDisp(0, FALSE);
    }
    m_iMpFigure = 1;
    m_uiMogState = 0;
    m_iMp = 0;
    m_iOldMp = 0;
    m_iDropMp = 0;
    m_pMogSeqCtrls = NULL;
    m_pMogSeqCtrls = new (__FILE__ "(2148)") D2::C2DSeqCtrl[6];
    m_pMogSeqCtrls->setLayer(8);
    m_pMogSeqCtrls->addSeq(g_szSeqCockpit, 0x66);
    D2::C2DMan::getLayPosition(g_szLadCockpit, 0x10, 0, pos, FALSE);
    m_pMogSeqCtrls->setPos(pos);
    m_pMogSeqCtrls->enableDisp(0, FALSE);
    pos.x += size * 4 + 50;
    pos.y += 20.0f;
    for (i = 1; i < 6; i++) {
        m_pMogSeqCtrls[i].setLayer(8);
        m_pMogSeqCtrls[i].addSeq(g_szSeqCockpit, 0x57);
        pos.x -= size;
        m_pMogSeqCtrls[i].setPos(pos);
        m_pMogSeqCtrls[i].enableDisp(0, FALSE);
    }
    m_uiCardState = 0;
    _setCardNum(m_cardData, 0, 0);
    m_pCardSeqCtrls = NULL;
    m_pCardSeqCtrls = new (__FILE__ "(2173)") D2::C2DSeqCtrl[2];
    m_pCard = new (__FILE__ "(2174)") C2DCard;
    for (i = 0; i < 4; i++) _setCard(m_cardStack[i], 0, 0);
    m_nStack = 0;
    m_nTexID = 0;
    if (PLM::CPlayerManager::getPLM() && PLM::CPlayerManager::getPLM()->is100AcreWood()) m_nTexID = 4;
}
void CPrizeGauge::main() {
    INT i;

    if (CSceneEnv::isStopGauge()) return;
    if (exePrizeCard()) return;
    if (CGameWork::getInt(1, 0xE2)) {
        if (m_uiState) {
            m_uiState = 0;
            m_pSeqCtrls->replaceSeq(0, g_szSeqCockpit, 0x51);
            for (i = 0; i < 6; i++) m_pSeqCtrls[i].enableDisp(0, FALSE);
            m_iExp = 0;
            m_iOldExp = 0;
        }
        if (m_uiMogState) {
            m_uiMogState = 0;
            m_pMogSeqCtrls->replaceSeq(0, g_szSeqCockpit, 0x66);
            m_pMogSeqCtrls->enableDisp(0, FALSE);
            for (i = 1; i < 6; i++) {
                m_pMogSeqCtrls[i].replaceSeq(0, g_szSeqCockpit, 0x57);
                m_pMogSeqCtrls[i].enableDisp(0, FALSE);
            }
            m_iMp = 0;
            m_iOldMp = 0;
            m_iDropMp = 0;
        }
        return;
    }
    switch (m_uiState) {
    case 0:
        break;
    case 1:
        if (m_pSeqCtrls->isEnd(0)) {
            *(UINT*)&m_uiState = 2; // MATCH: not struct-field stores (register of the state constants), all states below
            m_pSeqCtrls->replaceSeq(0, g_szSeqCockpit, 0x52);
            for (i = 1; i <= m_iFigure; i++) m_pSeqCtrls[i].enableDisp(0, TRUE);
        }
        break;
    case 2:
        if (m_pSeqCtrls->isEnd(0)) {
            *(UINT*)&m_uiState = 3;
            m_pSeqCtrls->replaceSeq(0, g_szSeqCockpit, 0x53);
            for (i = 1; i < 6; i++) m_pSeqCtrls[i].enableDisp(0, FALSE);
        }
        break;
    case 3:
        if (m_pSeqCtrls->isEnd(0)) {
            *(UINT*)&m_uiState = 4;
            m_pSeqCtrls->replaceSeq(0, g_szSeqCockpit, 0x54);
            setPrizeExp(0);
            m_iOldExp = 0;
        }
        break;
    case 4:
        if (m_pSeqCtrls->isEnd(0)) {
            *(UINT*)&m_uiState = 5;
            m_pSeqCtrls->replaceSeq(0, g_szSeqCockpit, 0x55);
            for (i = 1; i <= m_iFigure; i++) m_pSeqCtrls[i].enableDisp(0, TRUE);
        }
        break;
    case 5:
        if (m_pSeqCtrls->isEnd(0)) {
            *(UINT*)&m_uiState = 6;
            m_pSeqCtrls->replaceSeq(0, g_szSeqCockpit, 0x56);
            for (i = 1; i < 6; i++) m_pSeqCtrls[i].enableDisp(0, FALSE);
        }
        break;
    case 6:
        if (m_pSeqCtrls->isEnd(0)) {
            m_uiState = 0;
            m_pSeqCtrls->replaceSeq(0, g_szSeqCockpit, 0x51);
            m_pSeqCtrls->enableDisp(0, FALSE);
            m_iExp = 0;
            m_iOldExp = 0;
        }
        break;
    }
    m_iOldExp = m_iExp;
    switch (m_uiMogState) {
    case 0:
        break;
    case 1:
        if (m_pMogSeqCtrls->isEnd(0)) {
            *(UINT*)&m_uiMogState = 2;
            m_pMogSeqCtrls->replaceSeq(0, g_szSeqCockpit, 0x67);
            for (i = 1; i <= m_iMpFigure; i++) m_pMogSeqCtrls[i].enableDisp(0, TRUE);
        }
        break;
    case 2:
        if (m_pMogSeqCtrls->isEnd(0)) {
            *(UINT*)&m_uiMogState = 3;
            m_pMogSeqCtrls->replaceSeq(0, g_szSeqCockpit, 0x68);
            for (i = 1; i < 6; i++) m_pMogSeqCtrls[i].enableDisp(0, FALSE);
        }
        break;
    case 3:
        if (m_pMogSeqCtrls->isEnd(0)) {
            *(UINT*)&m_uiMogState = 4;
            m_pMogSeqCtrls->replaceSeq(0, g_szSeqCockpit, 0x69);
            setPrizeMp(0);
            m_iOldMp = 0;
        }
        break;
    case 4:
        if (m_pMogSeqCtrls->isEnd(0)) {
            *(UINT*)&m_uiMogState = 5;
            m_pMogSeqCtrls->replaceSeq(0, g_szSeqCockpit, 0x6A);
            for (i = 1; i <= m_iMpFigure; i++) m_pMogSeqCtrls[i].enableDisp(0, TRUE);
        }
        break;
    case 5:
        if (m_pMogSeqCtrls->isEnd(0)) {
            *(UINT*)&m_uiMogState = 6;
            m_pMogSeqCtrls->replaceSeq(0, g_szSeqCockpit, 0x6B);
            for (i = 1; i < 6; i++) m_pMogSeqCtrls[i].enableDisp(0, FALSE);
        }
        break;
    case 6:
        if (m_pMogSeqCtrls->isEnd(0)) {
            m_uiMogState = 0;
            m_pMogSeqCtrls->replaceSeq(0, g_szSeqCockpit, 0x66);
            m_pMogSeqCtrls->enableDisp(0, FALSE);
            m_iMp = 0;
            m_iOldMp = 0;
        }
        break;
    case 7:
        if (m_pMogSeqCtrls->isEnd(0)) {
            *(UINT*)&m_uiMogState = 8;
            m_pMogSeqCtrls->replaceSeq(0, g_szSeqCockpit, 0x8D);
            for (i = 1; i <= m_iMpFigure; i++) m_pMogSeqCtrls[i].enableDisp(0, TRUE);
        }
        break;
    case 8:
        if (m_pMogSeqCtrls->isEnd(0)) {
            *(UINT*)&m_uiMogState = 9;
            m_pMogSeqCtrls->replaceSeq(0, g_szSeqCockpit, 0x8E);
            for (i = 1; i < 6; i++) m_pMogSeqCtrls[i].enableDisp(0, FALSE);
        }
        break;
    case 9:
        if (m_pMogSeqCtrls->isEnd(0)) {
            m_uiMogState = 0;
            m_pMogSeqCtrls->replaceSeq(0, g_szSeqCockpit, 0x66);
            m_pMogSeqCtrls->enableDisp(0, FALSE);
            for (i = 1; i < 6; i++) {
                m_pMogSeqCtrls[i].replaceSeq(0, g_szSeqCockpit, 0x57);
                m_pMogSeqCtrls[i].enableDisp(0, FALSE);
            }
            m_iDropMp = 0;
        }
        break;
    }
    m_iOldMp = m_iMp;
}
void CPrizeGauge::setDispSeq() {
    INT i;
    INT pow;
    FVECTOR pos;
    pow = 10000;
    m_iFigure = 0;
    for (i = 5; i > 0; i--) {
        INT n = m_iExp / pow % 10;
        if (n > 0 && m_iFigure == 0) m_iFigure = i;
        m_pSeqCtrls[i].changeTexture(0, 0, NULL, &CardBattle::g_uvCRCounterNumTex[m_nTexID][n], &CardBattle::g_uvCRCounterNumTex[m_nTexID + 1][n]);
        pow /= 10;
    }
    D2::C2DMan::getLayPosition(g_szLadCockpit, 10, 0, pos, FALSE);
    pos.x += m_iFigure * 15 + 35;
    pos.y += 20.0f;
    for (i = 1; i <= m_iFigure; i++) {
        pos.x -= 15.0f;
        m_pSeqCtrls[i].setPos(pos);
    }
    switch (m_uiState) {
    case 0:
    case 1:
    case 3:
    case 4:
    case 6:
        break;
    case 2:
    case 5:
    default:
        for (i = 1; i < 6; i++) {
            if (i <= m_iFigure) m_pSeqCtrls[i].enableDisp(0, TRUE);
            else m_pSeqCtrls[i].enableDisp(0, FALSE);
        }
        break;
    }
}
void CPrizeGauge::setDispMogSeq() {
    INT mp;
    INT i;
    INT pow;
    FVECTOR pos;
    mp = m_iDropMp;
    if (mp <= 0) mp = m_iMp;
    if (mp > 99999) mp = 99999;
    m_iMpFigure = 0;
    pow = 10000;
    for (i = 5; i > 0; i--) {
        INT n = mp / pow % 10;
        if (n > 0 && m_iMpFigure == 0) m_iMpFigure = i;
        m_pMogSeqCtrls[i].changeTexture(0, 0, NULL, &CardBattle::g_uvCRCounterNumTex[m_nTexID][n], &CardBattle::g_uvCRCounterNumTex[m_nTexID + 1][n]);
        pow /= 10;
    }
    D2::C2DMan::getLayPosition(g_szLadCockpit, 0x10, 0, pos, FALSE);
    pos.x += m_iMpFigure * 15 + 35;
    pos.y += 20.0f;
    for (i = 1; i <= m_iMpFigure; i++) {
        pos.x -= 15.0f;
        m_pMogSeqCtrls[i].setPos(pos);
    }
    switch (m_uiMogState) {
    case 0:
    case 1:
    case 3:
    case 4:
    case 6:
    case 7:
    case 9:
        break;
    case 2:
    case 5:
    case 8:
    default:
        for (i = 1; i < 6; i++) {
            if (i <= m_iMpFigure) m_pMogSeqCtrls[i].enableDisp(0, TRUE);
            else m_pMogSeqCtrls[i].enableDisp(0, FALSE);
        }
        break;
    }
}
void CPrizeGauge::setPrizeExp(INT inExp) {
    if (!m_pInstance) return;
    if (CGameWork::getInt(1, 0xE2)) return;
    switch (_ref(m_pInstance)->m_uiState) {
    case 0:
        _ref(m_pInstance)->m_iExp = inExp;
        _ref(m_pInstance)->m_uiState = 1;
        _ref(m_pInstance)->m_pSeqCtrls->enableDisp(0, TRUE);
        break;
    case 1:
    case 2:
    case 3:
        _ref(m_pInstance)->m_iExp += inExp;
        break;
    case 4:
    case 5:
    case 6:
        if (g_pPlayer && g_pPlayer->getLV() >= 99) {
            _ref(m_pInstance)->m_iExp = 0;
            _ref(m_pInstance)->m_uiState = 0;
            _ref(m_pInstance)->m_pSeqCtrls->replaceSeq(0, _ref(g_szSeqCockpit), 0x51);
            _ref(m_pInstance)->m_pSeqCtrls->enableDisp(0, FALSE);
            _ref(m_pInstance)->m_iOldExp = 0;
            return;
        }
        _ref(m_pInstance)->m_iExp = CPlayer::getNowNextEXP();
        break;
    }
    _ref(m_pInstance)->setDispSeq();
}
void CPrizeGauge::setPrizeMp(INT inMp) {
    if (!m_pInstance) return;
    if (CGameWork::getInt(1, 0xE2)) return;
    switch (_ref(m_pInstance)->m_uiMogState) {
    case 0:
        _ref(m_pInstance)->m_iMp = inMp;
        _ref(m_pInstance)->m_uiMogState = 1;
        _ref(m_pInstance)->m_pMogSeqCtrls->enableDisp(0, TRUE);
        break;
    case 1:
    case 2:
    case 3:
        _ref(m_pInstance)->m_iMp += inMp;
        break;
    case 4:
    case 5:
    case 6:
    case 7:
    case 8:
    case 9:
        _ref(m_pInstance)->m_iMp = CPlayer::getNowTotalMP();
        break;
    }
    _ref(m_pInstance)->setDispMogSeq();
}
void CPrizeGauge::setDropMp(INT inMp) {
    INT i;
    if (!m_pInstance) return;
    if (inMp <= 0) return;
    if (CGameWork::getInt(1, 0xE2)) return;
    if (m_pInstance->m_uiMogState < 7) {
        *(UINT*)&m_pInstance->m_uiMogState = 7; // MATCH: as resetHP (m_pInstance reloaded after it)
        m_pInstance->m_pMogSeqCtrls->replaceSeq(0, g_szSeqCockpit, 0x8C);
        m_pInstance->m_pMogSeqCtrls->enableDisp(0, TRUE);
        for (i = 1; i < 6; i++) {
            m_pInstance->m_pMogSeqCtrls[i].replaceSeq(0, g_szSeqCockpit, 0x82);
            m_pInstance->m_pMogSeqCtrls[i].enableDisp(0, FALSE);
        }
    }
    *(INT*)&m_pInstance->m_iDropMp = inMp; // MATCH: as resetHP
    m_pInstance->m_iMp = 0;
    *(INT*)&m_pInstance->m_iOldMp = 0; // MATCH: m_pInstance reloaded for the call after it
    m_pInstance->setDispMogSeq();
}
void CPrizeGauge::setPrizeCard(CARD& inData, INT inMode, INT inStack) {
    INT i;

    if (!m_pInstance) {
#line 2645
        new (com_malloc_d(sizeof(CPrizeGauge), __FILE__ "(2645)", RET_ADDR())) CPrizeGauge(NULL);
        setPrizeCard(inData, inMode, 0);
        return;
    }
    if (m_pInstance->m_uiCardState != 0 && inStack == 0) {
        if (CGameWork::getInt(1, 0xE2) && inMode == 1) return;
        if (_ref(m_pInstance)->m_nStack == 4) {
            _ref(m_pInstance)->m_pCard->destroy();
            _ref(m_pInstance)->m_pCardSeqCtrls[0].destroy();
            _ref(m_pInstance)->m_pCardSeqCtrls[1].destroy();
            setPrizeCard(_ref(m_pInstance)->m_cardStack[0], inMode, _ref(m_pInstance)->m_nStack);
        }
        _ref(m_pInstance)->m_cardStack[_ref(m_pInstance)->m_nStack] = inData;
        _ref(m_pInstance)->m_nStack++;
        return;
    }
    _ref(m_pInstance)->m_uiCardState = inMode;
    switch (_ref(m_pInstance)->m_uiCardState) {
    case 1:
        _ref(m_pInstance)->m_cardData = inData;
        if (!CGameWork::getInt(1, 0xE2)) {
            FVECTOR pos;
            D2::C2DSeqCtrl* pSeq0;
            G_RGBA rgba;
            D2::C2DFnt* p2DFnt;
            D2::C2DSeqCtrl* pSeq1;

            pSeq0 = &_ref(m_pInstance)->m_pCardSeqCtrls[0];
            pSeq0->setLayer(8);
            pSeq0->addSeq(g_szSeqGetCard, 5);
            D2::C2DMan::getLayPosition(g_szLadGetCard, 2, 0, pos, FALSE);
            pSeq0->setPos(pos);
            rgba.col = 0x80808080;
#line 2705
            p2DFnt = new (com_malloc_d(sizeof(D2::C2DFnt), __FILE__ "(2705)", RET_ADDR())) D2::C2DFnt(0, 0, &rgba, 0xE, inData.getName(), 0);
            pSeq0->addFnt(g_szSeqGetCard, p2DFnt);
            pSeq0->enableColorCtrl(TRUE);
            pSeq1 = &_ref(m_pInstance)->m_pCardSeqCtrls[1];
            pSeq1->setLayer(8);
            pSeq1->addSeq(g_szSeqGetCard, 4);
            D2::C2DMan::getLayPosition(g_szLadGetCard, 2, 0, pos, FALSE);
            pSeq1->setPos(pos);
            _ref(m_pInstance)->m_pCard->setLayer(8);
            _ref(m_pInstance)->m_pCard->create(inData);
            _ref(m_pInstance)->m_pCard->setParent(pSeq1);
            _ref(m_pInstance)->m_pCard->enableColorCtrl(TRUE);
            CSound::playSE(0x39, -1, 64);
        }
        break;
    case 2: {
        INT nShiftKH2;
        FVECTOR pos;
        D2::C2DSeqCtrl* pSeq0;
        G_RGBA rgba;
        D2::C2DFnt* p2DFnt;
        D2::C2DSeqCtrl* pSeq1;

        _ref(m_pInstance)->m_tqTimer = 3000;
        nShiftKH2 = 0;
        if ((UINT)(inData.m_nKind - 0x13) < 6 || inData.m_nKind == 0xF || (UINT)(inData.m_nKind - 0x73) < 12) nShiftKH2 = 5;
        _ref(m_pInstance)->m_cardData = inData;
        pSeq0 = &_ref(m_pInstance)->m_pCardSeqCtrls[0];
        pSeq0->setLayer(8);
        pSeq0->addSeq(g_szSeqGetCard, nShiftKH2 + 2);
        D2::C2DMan::getLayPosition(g_szLadGetCard, 0, 0, pos, FALSE);
        pSeq0->setPos(pos);
        rgba.col = 0x80808080;
#line 2746
        p2DFnt = new (com_malloc_d(sizeof(D2::C2DFnt), __FILE__ "(2746)", RET_ADDR())) D2::C2DFnt(0, 0, &rgba, 0x12, inData.getName(), 2);
        pSeq0->addFnt(g_szSeqGetCard, p2DFnt);
        pSeq0->enableColorCtrl(TRUE);
        pSeq1 = &_ref(m_pInstance)->m_pCardSeqCtrls[1];
        pSeq1->setLayer(8);
        pSeq1->addSeq(g_szSeqGetCard, 0);
        D2::C2DMan::getLayPosition(g_szLadGetCard, 0, 0, pos, FALSE);
        pSeq1->setPos(pos);
        _ref(m_pInstance)->m_pCard->setLayer(8);
        _ref(m_pInstance)->m_pCard->create(inData);
        _ref(m_pInstance)->m_pCard->setParent(pSeq1);
        _ref(m_pInstance)->m_pCard->enableColorCtrl(TRUE);
        CSound::playSE(0x3A, -1, 64);
        break;
    }
    }
    if (inStack > 0) {
        for (i = 0; i < 2; i++) _ref(m_pInstance)->m_cardStack[i] = _ref(m_pInstance)->m_cardStack[i + 1];
        _setCard(_ref(m_pInstance)->m_cardStack[3], 0, 0);
        _ref(m_pInstance)->m_nStack--;
    }
}
INT CPrizeGauge::exePrizeCard() {
    BOOL flag;

    switch (m_uiCardState) {
    case 0:
        break;
    case 2: { /* MATCH: case 2 has its own flag (a0); case 1/3 uses the outer one (v1) */
        BOOL flag;
        m_tqTimer -= getFrameRate();
        flag = m_tqTimer <= 0 && CPadHandler::isButtonTrg(0x40, 0);
        if (flag || m_pCardSeqCtrls->isEnd(0)) {
            INT nShiftKH2;
            FVECTOR pos;
            D2::C2DSeqCtrl* pSeq;
            G_RGBA rgba;
            D2::C2DFnt* p2DFnt;

            m_uiCardState = 3;
            m_pCardSeqCtrls[0].destroy();
            m_pCardSeqCtrls[1].destroy();
            nShiftKH2 = 0;
            if ((UINT)(m_cardData.m_nKind - 0x13) < 6 || m_cardData.m_nKind == 0xF || (UINT)(m_cardData.m_nKind - 0x73) < 12) nShiftKH2 = 5;
            pSeq = &m_pCardSeqCtrls[0];
            pSeq->setLayer(8);
            pSeq->addSeq(g_szSeqGetCard, nShiftKH2 + 3);
            D2::C2DMan::getLayPosition(g_szLadGetCard, 1, 0, pos, FALSE);
            pSeq->setPos(pos);
            rgba.col = 0x80808080;
#line 2816
            p2DFnt = new (com_malloc_d(sizeof(D2::C2DFnt), __FILE__ "(2816)", RET_ADDR())) D2::C2DFnt(0, 0, &rgba, 0x12, m_cardData.getName(), 2);
            pSeq->addFnt(g_szSeqGetCard, p2DFnt);
            pSeq->enableColorCtrl(TRUE);
            pSeq = &m_pCardSeqCtrls[1];
            pSeq->setLayer(8);
            pSeq->addSeq(g_szSeqGetCard, 1);
            D2::C2DMan::getLayPosition(g_szLadGetCard, 1, 0, pos, FALSE);
            pSeq->setPos(pos);
            m_pCard->setParent(pSeq);
        }
        break;
    }
    case 1:
    case 3:
        flag = m_uiCardState == 1 && CGameWork::getInt(1, 0xE2);
        if (flag || m_pCardSeqCtrls->isEnd(0)) {
            m_pCard->destroy();
            m_pCardSeqCtrls[0].destroy();
            m_pCardSeqCtrls[1].destroy();
            if (m_nStack > 0) {
                INT dispOff = m_uiCardState == 3 ? 2 : 1;
                setPrizeCard(m_cardStack[0], dispOff, m_nStack);
                break;
            }
            m_uiCardState = 0;
            if (m_uiMode == 0) {
                out();
                return 1;
            }
        }
        break;
    }
    return 0;
}
INT CPrizeGauge::isNowDispCard() {
    if (!m_pInstance) return FALSE;
    return m_pInstance->m_uiCardState != 0;
}
CEscapeGauge::CEscapeGauge(CTask* p) : CTask(p, 0x4000) { _initialize(); }
CEscapeGauge::~CEscapeGauge() {
    delete m_pSeqCtrl;
    m_pSeqCtrl = NULL;
}
void CEscapeGauge::_initialize() {
    m_uiState = 0;
    m_uiFlag = 0;
    m_pSeqCtrl = new (com_malloc_d(sizeof(D2::C2DSeqCtrl), __FILE__ "(2903)", RET_ADDR())) D2::C2DSeqCtrl;
    m_pSeqCtrl->setLayer(8);
}
void CEscapeGauge::main() {
    if (CSceneEnv::isStopGauge()) return;
    switch (m_uiState) {
    case 1:
        if (m_pSeqCtrl->isEnd(0)) {
            m_uiState = 2;
            m_pSeqCtrl->replaceSeq(0, _ref(g_szSeqCockpit), 0x3A);
        }
        break;
    case 0:
    case 2:
        break;
    case 3:
        if (m_pSeqCtrl->isEnd(0)) {
            m_uiState = 0;
            m_pSeqCtrl->destroy();
        }
        break;
    }
}
void CEscapeGauge::setCharge(FLOAT inRate) {
    FVECTOR pos;
    switch (m_uiState) {
    case 0:
        m_uiState = 1;
        m_pSeqCtrl->addSeq(g_szSeqCockpit, 0x39);
        m_pSeqCtrl->addSeq(g_szSeqCockpit, 0x3C);
        D2::C2DMan::getLayPosition(g_szLadCockpit, 7, 0, pos, FALSE);
        m_pSeqCtrl->setPos(pos);
        m_pSeqCtrl->changeTime(1, (UINT)inRate);
        break;
    case 1:
        m_pSeqCtrl->changeTime(1, (UINT)inRate);
        break;
    case 2:
        m_pSeqCtrl->changeTime(1, (UINT)inRate);
        break;
    case 3:
        m_uiState = 2;
        m_pSeqCtrl->replaceSeq(0, g_szSeqCockpit, 0x3A);
        m_pSeqCtrl->changeTime(1, (UINT)inRate);
        break;
    }
}
void CEscapeGauge::destroy() {
    switch (m_uiState) {
    case 1:
    case 2:
        m_uiState = 3;
        m_pSeqCtrl->replaceSeq(0, _ref(g_szSeqCockpit), 0x3B);
        m_pSeqCtrl->changeTime(1, 0);
        break;
    }
}
CFriendGauge::CFriendGauge(CTask* p) : CTask(p, 0x4000) { _initialize(); }
CFriendGauge::~CFriendGauge() {
    delete[] m_pSeqCtrls;
    m_pSeqCtrls = NULL;
    delete[] m_pCards;
    m_pCards = NULL;
}
void CFriendGauge::_initialize() {
    INT i;
    INT kind;
    PLM::CPlayerManager* pPLM;
    m_uiState = 0;
    m_uiCount = 0;
    pPLM = PLM::CPlayerManager::getPLM();
    for (i = 0; i < 3; i++) _setCard(m_cardFriend[i], 0, 0);
    for (i = 0; i < 3; i++) {
        kind = pPLM->getAppearFriendCard(i, 1);
        if (kind) {
            _setCard(m_cardFriend[m_uiCount], kind, 0xFF);
            m_uiCount++;
        }
    }
    if (m_uiCount) m_pCards = new (__FILE__ "(3041)") C2DCard[m_uiCount];
    else m_pCards = NULL;
    m_pSeqCtrls = new (__FILE__ "(3045)") D2::C2DSeqCtrl[3];
}
void CFriendGauge::main() {
    if (CSceneEnv::isStopGauge()) return;
    if (m_uiState < 2) return;
    if (m_uiState != 2) return;
    if (m_pSeqCtrls->isEnd(0)) setGauge(0);
}
void CFriendGauge::setGauge(INT inState) {
    INT i;
    D2::C2DSeqCtrl* pSeq;
    FVECTOR pos;
    if (!m_uiCount) return;
    if (inState == m_uiState) return;
    if (m_uiState) {
        for (i = 0; i < m_uiCount; i++) m_pCards[i].destroy();
        for (i = 0; i < 3; i++) m_pSeqCtrls[i].destroy();
    }
    switch (inState) {
    case 1:
        pSeq = &m_pSeqCtrls[0];
        pSeq->setLayer(5);
        pSeq->addSeq(g_szSeqCockpit, 0x7C);
        D2::C2DMan::getLayPosition(g_szLadCockpit, 0x16, 0, pos, FALSE);
        pSeq->setPos(pos);
        m_pCards[0].setLayer(4);
        m_pCards[0].create(m_cardFriend[0]);
        m_pCards[0].setParent(pSeq);
        m_pCards[0].enableColorCtrl(TRUE);
        for (i = 1; i < m_uiCount; i++) {
            pSeq = &m_pSeqCtrls[i];
            pSeq->setLayer(4);
            pSeq->addSeq(g_szSeqCockpit, 0x7C + i * 2);
            pSeq->setPos(pos);
            m_pCards[i].setLayer(4);
            m_pCards[i].create(m_cardFriend[i]);
            m_pCards[i].setParent(pSeq);
            m_pCards[i].enableColorCtrl(TRUE);
        }
        break;
    case 2:
        if (m_uiState == 1) {
            pSeq = &m_pSeqCtrls[0];
            pSeq->setLayer(5);
            pSeq->addSeq(g_szSeqCockpit, 0x7D);
            D2::C2DMan::getLayPosition(g_szLadCockpit, 0x16, 0, pos, FALSE);
            pSeq->setPos(pos);
            m_pCards[0].setLayer(4);
            m_pCards[0].create(m_cardFriend[0]);
            m_pCards[0].setParent(pSeq);
            m_pCards[0].enableColorCtrl(TRUE);
            for (i = 1; i < m_uiCount; i++) {
                pSeq = &m_pSeqCtrls[i];
                pSeq->setLayer(4);
                pSeq->addSeq(g_szSeqCockpit, 0x7D + i * 2);
                pSeq->setPos(pos);
                m_pCards[i].setLayer(4);
                m_pCards[i].create(m_cardFriend[i]);
                m_pCards[i].setParent(pSeq);
                m_pCards[i].enableColorCtrl(TRUE);
            }
        }
        break;
    }
    m_uiState = inState;
}
// SOURCE DATA: .data .rodata .bss
