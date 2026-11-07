#define GAME_BATTLECARD_CPP
#include "khtypes.h"
#define GPRIM_H_PATH "System/Graphic/GPrim.h" /* MATCH: the original includes it under this name */
#define CARDBATTLE_H_PATH "c:/KHCOM/MainApp/Game/CardBattle.h" /* MATCH: the original includes it under this name */
#include "System/TaskMan.h"
#include "System/Task.h"
#include "System/Graphic/Gprim.h"
#include "System/Graphic/2d/2DMan.h"
#include "Game/CardBattle.h"
#include "System/Graphic/Anim.h"
#include "System/Graphic/Str.h"
#include "Game/Collision/CollUtil.h"
/* MATCH: placeholder: an empty string follows CollUtil.h's strings in .rodata (as in Game/CardRevolver) */
inline void _emptyStringPlaceholderBattleCard() { TRACE(""); }
#include "Game/BattleCard.h"
#include "Game/GameWork.h"
#include "Game/Debug/DebugWork.h"
#include "System/Rnd.h"
#include "System/Com.h"
#include "System/Graphic/WorldEnv.h"
#include "Game/Entity.h"
#include "include_asm.h"
#include "System/HeapMan.h"

extern "C" int abs(int);
extern "C" float sinf(float);

using namespace CardBattle;

extern INT SCREEN_HEIGHT;






extern INT SCREEN_HEIGHT;
namespace CardBattle {
static LPCSTR g_szDpxZexion2 = "zexion2";
static LPCSTR g_szLadLayoutCard = "layout_card.lad";
unsigned char g_Seq_toLayout[26] = {
    11, 9, 8, 7, 20, 17, 18, 16, 29, 28, 27, 26, 25, 24, 33, 34, 35, 30, 31, 32, 6, 5, 4, 3, 2, 1,
};
static char* g_pBC_DbgStateName[9] = {
    "STATE_USE", "STATE_STOCK", "STATE_HAND", "STATE_DESTROY", "STATE_DUEL", "STATE_SNATCH", "STATE_PRONOUNCE",
    "STATE_MAX", "STATE_NONE",
};
static char* g_pBC_DbgModeName[21] = {
    "MODE_NULL", "MODE_START", "MODE_EXEC", "MODE_END", "MODE_REFRECT_START", "MODE_REFRECT", "MODE_BREAK",
    "MODE_ENCHANT", "MODE_STOCK_START", "MODE_STOCK", "MODE_HAND_NONE", "MODE_HAND_ACTIVE", "MODE_HAND_ROLL",
    "MODE_HAND_HIDE", "MODE_HAND_OUT", "MODE_HAND_RELOAD", "MODE_DUEL_ENTRY", "MODE_SNATCH_START", "MODE_SNATCH",
    "MODE_TRAP_START", "MODE_DISCARD",
};
CB_LAYOUT g_LayoutData[65] = {
    {0, 0, 0, 0}, // 0
    {0, SCREEN_HEIGHT - 96, 4, 100}, // 1
    {-16, SCREEN_HEIGHT - 68, 4, 80}, // 2
    {0, SCREEN_HEIGHT - 96, 3, 80}, // 3
    {-16, SCREEN_HEIGHT - 68, 4, 80}, // 4
    {0, SCREEN_HEIGHT - 96, 3, 80}, // 5
    {16, SCREEN_HEIGHT - 60, 2, 80}, // 6
    {64, SCREEN_HEIGHT - 96, 5, 100}, // 7
    {84, SCREEN_HEIGHT - 64, 4, 100}, // 8
    {94, SCREEN_HEIGHT - 12, 3, 100}, // 9
    {94, SCREEN_HEIGHT + 48, 2, 100}, // 10
    {9, SCREEN_HEIGHT - 120, 4, 100}, // 11
    {-32, SCREEN_HEIGHT - 120, 3, 100}, // 12
    {-64, SCREEN_HEIGHT + 96, 2, 100}, // 13
    {-44, SCREEN_HEIGHT - 64, 6, 100}, // 14
    {72, SCREEN_HEIGHT - 24, 2, 40}, // 15
    {448, SCREEN_HEIGHT - 96, 5, 100}, // 16
    {473, SCREEN_HEIGHT - 60, 4, 80}, // 17
    {468, SCREEN_HEIGHT - 12, 3, 80}, // 18
    {463, SCREEN_HEIGHT + 48, 2, 80}, // 19
    {488, SCREEN_HEIGHT - 72, 3, 80}, // 20
    {576, SCREEN_HEIGHT + 96, 2, 80}, // 21
    {512, SCREEN_HEIGHT - 120, 6, 100}, // 22
    {492, SCREEN_HEIGHT, 2, 40}, // 23
    {125, 65, 5, 50}, // 24
    {150, 65, 4, 50}, // 25
    {175, 65, 3, 50}, // 26
    {125, 85, 5, 50}, // 27
    {150, 85, 4, 50}, // 28
    {175, 85, 3, 50}, // 29
    {440, 70, 3, 50}, // 30
    {415, 70, 4, 50}, // 31
    {390, 70, 5, 50}, // 32
    {440, 90, 3, 50}, // 33
    {415, 90, 4, 50}, // 34
    {390, 90, 5, 50}, // 35
    {124, 63, 6, 50}, // 36
    {149, 63, 5, 50}, // 37
    {174, 63, 4, 50}, // 38
    {199, 63, 3, 50}, // 39
    {224, 63, 2, 50}, // 40
    {249, 63, 2, 50}, // 41
    {274, 63, 2, 50}, // 42
    {235, 339, 4, 80}, // 43
    {257, 340, 3, 80}, // 44
    {278, 342, 2, 80}, // 45
    {235, 338, 6, 80}, // 46
    {246, 339, 5, 80}, // 47
    {257, 340, 4, 80}, // 48
    {268, 341, 3, 80}, // 49
    {278, 342, 2, 80}, // 50
    {235, 337, 8, 80}, // 51
    {243, 338, 7, 80}, // 52
    {250, 339, 6, 80}, // 53
    {257, 340, 5, 80}, // 54
    {264, 341, 4, 80}, // 55
    {271, 342, 3, 80}, // 56
    {278, 343, 2, 80}, // 57
    {84, SCREEN_HEIGHT - 120, 6, 100}, // 58
    {256, 336, 7, 100}, // 59
    {276, 336, 6, 100}, // 60
    {296, 336, 5, 100}, // 61
    {316, 336, 4, 100}, // 62
    {336, 336, 3, 100}, // 63
    {356, 336, 2, 100}, // 64
};
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
static inline void _setCard(CARD& c, USHORT inKind, BYTE num) {
    c.m_nId = 0;
    c.m_nKind2 = inKind;
    c.m_nNumber2 = num;
    c.m_nCategory20 = cardDataTbl[c.m_nKind].nCategory0;
    c.m_nCategory21 = cardDataTbl[c.m_nKind].nCategory1;
}
namespace CardBattle {
}
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
static inline FLOAT _adjustRad(FLOAT r) {
    return (r < -3.1415927f) ? r + 6.2831855f : (r > 3.1415927f) ? r - 6.2831855f : r; // MATCH: one ternary (stateSnatch's wrap keeps the sum in the m_vRot.y register)
}

void CardBattle::initLayoutData() {
    FVECTOR vPos;
    for (INT i = 0; i < 26; i++) {
        D2::C2DMan::getLayPosition(g_szLadLayoutCard, 0, i, vPos, FALSE);
        CB_LAYOUT& lay = g_LayoutData[g_Seq_toLayout[i]];
        lay.x = (INT)vPos.x;
        lay.y = (INT)vPos.y;
    }
}

CardBattle::CBattleCard::CBattleCard(CTask* pParent, INT inIndex, INT inNumber, INT inUser, INT inState)
    : CTask(pParent, 0x4000) {
    m_uiFlag = 0;
    m_uiUser = inUser;
    m_iStock = -1;
    m_iHandMax = 0;
    m_tqTimer = 0;
    m_pCard = NULL;
    m_pRevolver = NULL;
    m_iHand = -1;
    m_uiOwner = inUser;
    m_State[0].set(this, &CBattleCard::preStateUse, &CBattleCard::stateUse, NULL);
    m_State[1].set(this, &CBattleCard::preStateStock, &CBattleCard::stateStock, NULL);
    m_State[2].set(this, &CBattleCard::preStateHand, &CBattleCard::stateHand, NULL);
    m_State[3].set(this, &CBattleCard::preStateDestroy, &CBattleCard::stateDestroy, NULL);
    m_State[4].set(this, &CBattleCard::preStateDuel, &CBattleCard::stateDuel, NULL);
    m_State[5].set(this, &CBattleCard::preStateSnatch, &CBattleCard::stateSnatch, NULL);
    m_State[6].set(this, &CBattleCard::preStatePronounce, &CBattleCard::statePronounce, NULL);
    _setCard(m_cardData, inIndex, inNumber);
    m_nNumberR = -1;
    m_nNumberE = -1;
    m_nNumber = inNumber;
    m_pCard = new (__FILE__ "(246)") C2DCard;
    m_iStockCount = 0;
    m_hTarget = 0;
    if (inState == 2) {
        // MATCH: stores through plain pointers (not in-struct MEMs) are ordered before the SCREEN_HEIGHT load, which
        // gives them the scheduling priority to come first
        *(CCardRevolver**)&m_pRevolver = (CCardRevolver*)pParent;
        *(INT*)&m_iState = inState;
        m_uiMode = 10;
        m_Pos.set(0, SCREEN_HEIGHT + 0x60);
        m_vRot.set(0.0f, 0.0f, 0.0f, 1.0f);
        m_fBright = 1.0f;
        m_uiFlag |= 0x20;
        changeState(2);
    } else {
        _initialize();
    }
    {
        struct _bits { u_long b0 : 28; u_long cat1 : 4; };
        if (((_bits*)&m_cardData)->cat1 == 4) m_uiFlag |= 0x8000;
    }
}
CBattleCard::~CBattleCard() {
    if (m_pCard) delete m_pCard;
    m_pCard = NULL;
}
void CBattleCard::_initialize() {
    m_iState = 8;
    changeState(0);
    m_uiMode = 0;
    m_Pos.set(0, SCREEN_HEIGHT + 96);
    m_vRot.set(0.0f, 0.0f, 0.0f, 1.0f);
    m_iLayer = 3;
    m_fBright = 1.0f;
    m_pCard->setLayer(3);
    m_pCard->create(m_cardData);
    FVECTOR pos = {{m_Pos.x, m_Pos.y, 0.0f, 1.0f}};
    m_pCard->setPos(pos);
    m_uiFlag = (INT)m_uiFlag & ~0x80;
    m_pCard->enableDisp(FALSE);
}
void CBattleCard::changeData(CARD& inData) {
    if (m_pCard) {
        m_cardData = inData;
        m_pCard->destroy();
        if (!m_cardData.m_nKind) {
            m_uiFlag = (INT)m_uiFlag & ~0x80;
        } else {
            m_pCard->create(m_cardData);
            FVECTOR pos = {{m_Pos.x, m_Pos.y, 0.0f, 1.0f}};
            m_pCard->setPos(pos);
            m_uiFlag = (INT)m_uiFlag & ~0x20;
            setRoomEffect(NULL);
        }
    }
}
void CBattleCard::main() {
    ((CFSM*)this)->update();
    /* MATCH: the category is read through a 64-bit bit-field over m_cardData (see updateNumberEffect) */
    struct _bits { u_long b0 : 28; u_long cat1 : 4; };
    if (m_uiUser == 1 && ((_bits*)&m_cardData)->cat1 != 8) {
        if (CCardBattle::isBlack()) {
            if (m_fBright > 0.0f) {
                m_fBright -= getFrameRateF() * 0.05f;
                if (m_fBright < 0.0f) m_fBright = 0.0f;
                m_uiFlag |= 0x4000;
            }
        } else if ((m_uiFlag & 0x80000) && m_iState == 2) {
            if (m_fBright > 0.4f) {
                m_fBright -= getFrameRateF() * 0.05f;
                if (m_fBright < 0.4f) m_fBright = 0.4f;
                m_uiFlag |= 0x4000;
            }
        } else {
            if (m_fBright < 1.0f) {
                m_fBright += getFrameRateF() * 0.05f;
                if (m_fBright > 1.0f) m_fBright = 1.0f;
                m_uiFlag |= 0x4000;
            }
        }
    }
    dispCard();
}
void CBattleCard::dispCard() {
    BOOL disp = TRUE;
    UINT shift;
    UINT flag;
    CGameWork::getInt(1, 0xe2);
    shift = m_uiUser != 1;
    flag = 0x20 << shift;
    if (CGameWork::getInt(1, 0xe2) & flag) {
        disp = FALSE;
        goto end;
    }
    flag = 2 << shift;
    if ((CGameWork::getInt(1, 0xe2) & flag) && m_iState == 2) {
        disp = FALSE;
    } else if (!isDisp()) {
        disp = FALSE;
    } else {
        FVECTOR pos = {{m_Pos.x, m_Pos.y, 0.0f, 1.0f}};
        m_pCard->setPos(pos);
        if (m_uiFlag & 0x8) {
            FVECTOR rot = {{m_vRot.x, m_vRot.y, m_vRot.z, 1.0f}};
            m_uiFlag = (INT)m_uiFlag & ~0x8;
            m_pCard->setRot(rot);
            m_pCard->setScl(m_vRot.w);
        }
        if (m_uiFlag & 0x2000) {
            m_uiFlag = (INT)m_uiFlag & ~0x2000;
            m_pCard->setScl(m_vRot.w);
        }
        if (m_uiFlag & 0x4000) {
            FVECTOR col = {{m_fBright, m_fBright, m_fBright, 1.0f}};
            m_uiFlag = (INT)m_uiFlag & ~0x4000;
            m_pCard->enableColorCtrl(TRUE);
            m_pCard->setCol(col);
        }
    }
end:
    if (!disp) {
        if (m_uiFlag & 0x80) {
            m_uiFlag = (INT)m_uiFlag & ~0x80;
            m_pCard->enableDisp(FALSE);
        }
    } else {
        if (!(m_uiFlag & 0x80)) {
            m_uiFlag |= 0x80;
            m_pCard->enableDisp(TRUE);
        }
    }
}
void CBattleCard::testDisp() {
    if (m_pCard) {
        if (CGameWork::getInt(1, 0xe2)) {
            m_uiFlag = (INT)m_uiFlag & ~0x80;
            if (m_cardData.m_nKind) m_pCard->enableDisp(FALSE);
        } else {
            m_uiFlag |= 0x2008;
            if (m_uiFlag & 0x1) {
                m_uiFlag |= 0x80;
                if (m_cardData.m_nKind) m_pCard->enableDisp(TRUE);
            } else {
                m_uiFlag = (INT)m_uiFlag & ~0x80;
                if (m_cardData.m_nKind) m_pCard->enableDisp(FALSE);
            }
        }
    }
}
void CBattleCard::changeState(INT inState) {
    if (!(inState >= 0 && inState < 7)) AssertApp(__FILE__, 492, "0 <= inState && inState < STATE_MAX", 0);
    gotoState(m_State[inState]);
}
void CBattleCard::setPlayCard() {
    changeState(0);
    m_uiMode = 1;
    m_iHand = -1;
    m_uiFlag = (INT)(m_uiFlag | 0x40) & ~0x2;
}
void CBattleCard::setUseEnd() { setPlayEnd(); }
void CBattleCard::setPlayEnd() {
    switch (m_iState) {
    case 0:
    case 1:
    case 4:
    case 6:
        changeState(3);
        m_uiMode = 3;
        m_tqTimer = 400;
        m_uiFlag = (INT)m_uiFlag & ~0x2;
        break;
    }
}
void CBattleCard::preStateUse() {
    CB_LAYOUT* pLayout;
    m_iState = 0;
    m_uiFlag |= 0x1;
    switch (m_uiUser) {
    case 1: pLayout = &g_LayoutData[7]; break;
    case 2: pLayout = &g_LayoutData[16]; break;
    }
    m_StartPos.set(pLayout->x, pLayout->y);
    m_fStartScale = pLayout->scale * 0.01f;
    if (getCardCategory() == 14) pLayout = &g_LayoutData[58];
    else pLayout = &g_LayoutData[1];
    m_EndPos.set(pLayout->x, pLayout->y);
    m_fEndScale = pLayout->scale * 0.01f;
    m_tqTimer = 400;
    m_Pos = m_StartPos;
    m_vRot.w = m_fStartScale;
    m_iLayer = pLayout->layer;
    m_pCard->changeLayer(m_iLayer);
}
void CBattleCard::stateUse() {
    switch (m_uiMode) {
    case 1: {
        ((CCardBattle*)CTaskMan::getSysTask(0x16))->checkNowPlaying(m_uiUser, this);
        m_tqTimer -= getFrameRate();
        if (m_tqTimer <= 0) m_tqTimer = 0;
        INT dx = (m_EndPos.x - m_StartPos.x) * m_tqTimer / 400;
        INT dy = (m_EndPos.y - m_StartPos.y) * m_tqTimer / 400;
        FLOAT sc = m_fEndScale - (m_fEndScale - m_fStartScale) * m_tqTimer / 400.0f;
        m_vRot.w = sc;
        m_Pos.x = m_EndPos.x - dx;
        m_Pos.y = m_EndPos.y - dy;
        if (!isSet()) {
            if (abs(dx) <= 64) m_uiFlag |= 0x2;
        } else if (!m_tqTimer) {
            m_uiMode = 2;
            struct _bits { u_long b0 : 28; u_long cat1 : 4; };
            if (((_bits*)&m_cardData)->cat1 == 0xe) {
                if (m_pRevolver) m_pRevolver->setTrapEffect();
                m_uiMode = 0x13;
                m_tqTimer = 800;
            }
        }
        ((CCardBattle*)CTaskMan::getSysTask(0x16))->checkNowPlaying(m_uiUser, this);
        break;
    }
    case 2:
        if (!((CCardBattle*)CTaskMan::getSysTask(0x16))->checkNowPlaying(m_uiUser, this)) destroy(1);
        break;
    case 0x13:
        m_tqTimer -= getFrameRate();
        if (m_tqTimer <= 0) {
            m_tqTimer = 0;
            _setCard(m_cardData, 0x43, 0);
            m_pCard->destroy();
            m_pCard->create(m_cardData);
            m_iLayer = g_LayoutData[58].layer;
            m_pCard->changeLayer(m_iLayer);
            m_uiMode = 2;
        }
        break;
    }
}
void CBattleCard::setRefrect() {
    changeState(3);
    if (m_uiMode == 16) m_iState = 8;
    m_uiMode = 4;
    m_uiFlag = (INT)m_uiFlag & ~0x2;
    if (m_iStock && m_cardData.m_nCategory1 == 4) m_uiFlag = (INT)m_uiFlag & ~0x20;
}
void CBattleCard::setBreak(INT inFlag) {
    changeState(3);
    m_uiMode = 6;
    m_tqTimer = 1200;
    m_uiFlag = (INT)m_uiFlag & ~0x2;
    if (inFlag > 0) m_uiFlag |= 0x20;
}
void CBattleCard::destroy(INT inFlag) {
    if (m_uiMode < 8) {
        if (m_uiMode >= 3) return;
    }
    if (m_pRevolver) {
        TRACE("BC::destroy(); handle=%d\n", getHandle());
        m_uiFlag = (INT)m_uiFlag & ~0x1;
        m_uiFlag = (INT)m_uiFlag & ~0x2;
        m_uiFlag = (INT)m_uiFlag & ~0x4;
        m_uiFlag = (INT)m_uiFlag & ~0x40;
        m_uiFlag &= ~0x10000;
        m_uiFlag &= ~0x200000;
        m_uiFlag = (INT)m_uiFlag & ~0x10;
        m_uiMode = 0xe;
        m_iHand = -1;
        m_iStock = -1;
        m_pCard->enableColorCtrl(FALSE);
        m_vRot.set(0.0f, 0.0f, 0.0f, 1.0f);
        m_uiFlag = (INT)m_uiFlag & ~0x2008;
        changeState(2);
        resetEnchantEffect(0);
        if (DebugWork::g_dw.player.iCardSpend) return;
        if (!inFlag) return;
        switch (getCardCategory()) {
        case 0xe:
            m_pRevolver->exeTrapEffect(0);
            CCardBattle::addGimmickCount(-1);
            __asm__ volatile(""); /* MATCH: keeps this copy: jump2 would cross-jump it into case 6's */
            break;
        case 6:
            CCardBattle::addGimmickCount(-1);
            break;
        case 5:
            if (m_cardData.m_nKind == 0x39) CCardBattle::addPlutoCount(-1);
            break;
        default:
            return;
        }
        m_uiFlag |= 0x20;
    } else {
        out();
    }
}
void CBattleCard::setEnchant() {
    changeState(3);
    m_uiMode = 7;
    m_tqTimer = 1200;
    m_iHand = -1;
}
void CBattleCard::setStock(UINT inPri) {
    changeState(1);
    m_iStock = inPri;
    m_uiMode = 8;
    m_iHand = -1;
    m_uiFlag = (INT)m_uiFlag & ~0x2;
    setEnchantEffect(0x44, 0);
    setEnchantEffect(0x52, 0);
    setEnchantEffect(0x53, 0);
}
void CBattleCard::setPlayStock(INT inCount) {
    CB_LAYOUT* pLayout;
    m_uiMode = 1;
    m_iStockCount = inCount;
    switch (m_uiUser) {
    case 1: pLayout = &g_LayoutData[27 + m_iStock]; break;
    case 2: pLayout = &g_LayoutData[33 + m_iStock]; break;
    }
    m_StartPos.set(pLayout->x, pLayout->y);
    m_fStartScale = pLayout->scale * 0.01f;
    switch (inCount) {
    case 0:
    case 1: pLayout = &g_LayoutData[1]; break;
    case 2: pLayout = &g_LayoutData[2 + m_iStock]; break;
    case 3: pLayout = &g_LayoutData[4 + m_iStock]; break;
    }
    m_EndPos.set(pLayout->x, pLayout->y);
    m_fEndScale = pLayout->scale * 0.01f;
    m_tqTimer = 400;
    m_Pos = m_StartPos;
    m_vRot.w = m_fStartScale;
    m_iLayer = pLayout->layer;
    m_pCard->changeLayer(m_iLayer);
    m_uiFlag = (INT)(m_uiFlag | 0x40) & ~0x2;
    if (m_uiFlag & 0x10) {
        m_uiFlag = (INT)m_uiFlag & ~0x10;
        m_pCard->enableColorCtrl(FALSE);
    }
    if (m_pRevolver) {
        struct _bits { u_long b0 : 28; u_long cat1 : 4; };
        if (!m_iStock) {
            CCardBattle* pCB = (CCardBattle*)CTaskMan::getSysTask(0x16);
            if (DebugWork::g_dw.player.iCardSpend) return;
            INT enchant = pCB->getEnchantKind(m_uiUser);
            if (enchant == 0x2b) {
                if (getCardCategory() < 4 && getCardCategory()) return;
            } else if (((_bits*)&m_cardData)->cat1 == 1) {
                if (m_pRevolver->getHave(1, 1, 1, 0, 0) == 1) return;
            }
        } else {
            if (DebugWork::g_dw.player.iCardSpend) return;
            if (getCardCategory() >= 6) return;
            if (getCardCategory() < 4) return;
        }
    }
    m_uiFlag |= 0x20;
}
void CBattleCard::preStateStock() {
    CB_LAYOUT* pLayout;
    m_iState = 1;
    m_uiFlag |= 0x1;
    switch (m_uiUser) {
    case 1:
        pLayout = &g_LayoutData[7];
        m_StartPos.set(pLayout->x, pLayout->y);
        m_fStartScale = pLayout->scale * 0.01f;
        pLayout = &g_LayoutData[24 + m_iStock];
        if (m_pRevolver->getStockSkill() >= 4) pLayout = &g_LayoutData[27 + m_iStock];
        m_EndPos.set(pLayout->x, pLayout->y);
        m_fEndScale = pLayout->scale * 0.01f;
        break;
    case 2:
        pLayout = &g_LayoutData[16];
        m_StartPos.set(pLayout->x, pLayout->y);
        m_fStartScale = pLayout->scale * 0.01f;
        pLayout = &g_LayoutData[30 + m_iStock];
        if (m_pRevolver && m_pRevolver->getStockSkill() >= 0x6f) pLayout = &g_LayoutData[33 + m_iStock];
        else if (CCardBattle::getStockFolderStockSkill(2) >= 0x6f) pLayout = &g_LayoutData[33 + m_iStock];
        m_EndPos.set(pLayout->x, pLayout->y);
        m_fEndScale = pLayout->scale * 0.01f;
        break;
    }
    m_tqTimer = 400;
    m_Pos = m_StartPos;
    m_vRot.w = m_fStartScale;
    m_iLayer = pLayout->layer;
    m_pCard->changeLayer(m_iLayer);
}
void CBattleCard::stateStock() {
    CB_LAYOUT* pLayout;
    switch (m_uiMode) {
    case 2:
        break;
    case 8:
        m_uiFlag |= 0x2008;
        m_tqTimer -= getFrameRate();
        if (m_tqTimer <= 0) {
            m_uiMode = 9;
            m_uiFlag |= 0x4;
            m_tqTimer = 0;
            m_Pos = m_EndPos;
            m_vRot.w = m_fEndScale;
            if (m_pRevolver) m_pRevolver->calcStockSkill();
            else CCardBattle::calcStockFolderEnemyStockSkill();
        } else {
            INT dx = (m_EndPos.x - m_StartPos.x) * m_tqTimer / 400;
            INT dy = (m_EndPos.y - m_StartPos.y) * m_tqTimer / 400;
            FLOAT sc = m_fEndScale - (m_fEndScale - m_fStartScale) * m_tqTimer / 400.0f;
            m_vRot.w = sc;
            m_Pos.x = m_EndPos.x - dx;
            m_Pos.y = m_EndPos.y - dy;
        }
        break;
    case 9:
        m_tqTimer += getFrameRate();
        {
            FLOAT r = m_tqTimer * 0.1f * 3.1415927f / 180.0f;
            m_Pos.y = (INT)(sinf(r) * 4.0f) + m_EndPos.y;
        }
        if (m_uiUser == 2) {
            CCardBattle* pCB = (CCardBattle*)CTaskMan::getSysTask(0x16);
            INT enchant = pCB->getEnchantKind(m_uiUser);
            if (enchant == 0x31) {
                if (!(m_uiFlag & 0x10)) {
                    m_uiFlag |= 0x10;
                    if (m_pRevolver) m_pRevolver->setStockInfo();
                    else CCardBattle::setStockFolderStockInfo(2);
                    FVECTOR col;
                    col._q = 0;
                    col.w = 1.0f;
                    m_pCard->enableColorCtrl(TRUE);
                    m_pCard->setCol(col);
                }
            } else if (m_uiFlag & 0x10) {
                m_uiFlag = (INT)m_uiFlag & ~0x10;
                if (m_pRevolver) m_pRevolver->setStockInfo();
                else CCardBattle::setStockFolderStockInfo(2);
                m_pCard->enableColorCtrl(FALSE);
            }
            if (m_pRevolver && m_pRevolver->getStockSkill() >= 0x6f) {
                pLayout = &g_LayoutData[33 + m_iStock];
                m_EndPos.set(pLayout->x, pLayout->y);
            } else if (CCardBattle::getStockFolderStockSkill(2) >= 0x6f) {
                pLayout = &g_LayoutData[33 + m_iStock];
                m_EndPos.set(pLayout->x, pLayout->y);
            }
        } else {
            if (m_pRevolver->getStockSkill() >= 4) {
                pLayout = &g_LayoutData[27 + m_iStock];
                m_EndPos.set(pLayout->x, pLayout->y);
            }
        }
        break;
    case 1: {
        m_uiFlag |= 0x2008;
        m_tqTimer -= getFrameRate();
        if (m_tqTimer <= 0) m_tqTimer = 0;
        INT dx = (m_EndPos.x - m_StartPos.x) * m_tqTimer / 400;
        INT dy = (m_EndPos.y - m_StartPos.y) * m_tqTimer / 400;
        FLOAT sc = m_fEndScale - (m_fEndScale - m_fStartScale) * m_tqTimer / 400.0f;
        m_vRot.w = sc;
        m_Pos.y = m_EndPos.y - dy;
        m_Pos.x = m_EndPos.x - dx;
        if (!isSet()) {
            if (abs(dx) <= 64 && abs(dy) <= 64) m_uiFlag |= 0x2;
        } else if (!m_tqTimer) {
            m_uiMode = 2;
        }
        break;
    }
    }
}
void CBattleCard::preStateHand() { m_iState = 2; }
void CBattleCard::stateHand() {
    switch (m_uiMode) {
    case 10:
        break;
    case 11: {
        m_tqTimer += getFrameRate();
        FLOAT r = m_tqTimer * 0.1f * 3.1415927f / 180.0f;
        m_Pos.y = (INT)(sinf(r) * 4.0f) + m_EndPos.y;
        break;
    }
    case 12:
        m_uiFlag |= 0x2008;
        m_tqTimer -= getFrameRate();
        if (m_tqTimer <= 0) {
            m_tqTimer = (RND::genrand_int31() % 100) * 100;
            m_uiMode = 11;
            if (m_iHand < 3) {
                if (m_iHand >= 0) goto hand_ok1;
            }
            if (m_iHand != m_iHandMax - 1) {
                m_tqTimer = 0;
                m_uiMode = 10;
                if (!DebugWork::g_dw.player.iDeckDbgDisp) m_uiFlag = (INT)m_uiFlag & ~0x1;
            }
        hand_ok1:
            m_Pos = m_EndPos;
            m_vRot.set(0.0f, m_fEndDir, 0.0f, m_fEndScale);
        } else if (m_pRevolver) {
            moveCard(m_pRevolver->getRollSpeed(), m_pRevolver->getRollSpeed());
        }
        break;
    case 13:
        m_tqTimer -= getFrameRate();
        if (m_tqTimer <= 0) {
            m_tqTimer = 0;
            m_uiFlag = (INT)m_uiFlag & ~0x1;
            m_uiMode = 10;
            m_Pos = m_EndPos;
            m_vRot.set(0.0f, m_fEndDir, 0.0f, m_fEndScale);
            m_uiFlag |= 0x2008;
        } else {
            moveCard(400, 0);
        }
        break;
    case 14:
        break;
    case 15:
        if (m_iHand < 4) {
            if (m_iHand >= 0) {
                m_uiFlag |= 0x2008;
                goto timer;
            }
        }
        m_uiFlag |= 0x2008;
        if (m_iHand != m_iHandMax - 2 && m_iHand != m_iHandMax - 1 && !(m_uiFlag & 0x100000)) {
            m_uiFlag |= 0x100000;
            m_uiMode = 10;
            m_tqTimer = 0;
            if (!DebugWork::g_dw.player.iDeckDbgDisp) {
                m_uiFlag = (INT)m_uiFlag & ~0x1;
                m_uiFlag = (INT)m_uiFlag & ~0x2008;
            }
        }
    timer:
        m_tqTimer -= getFrameRate();
        if (m_tqTimer <= 0) {
            m_iStep++;
            if (m_iHand < m_iStep) {
                m_tqTimer = (RND::genrand_int31() % 100) * 100;
                m_uiMode = 11;
                if (m_iHand < 3) {
                    if (m_iHand >= 0) goto hand_ok2;
                }
                if (m_iHand != m_iHandMax - 1) {
                    m_tqTimer = 0;
                    m_uiMode = 10;
                    if (!DebugWork::g_dw.player.iDeckDbgDisp) m_uiFlag = (INT)m_uiFlag & ~0x1;
                }
        hand_ok2:
                m_Pos = m_EndPos;
                m_vRot.set(0.0f, m_fEndDir, 0.0f, m_fEndScale);
            } else {
                m_StartPos = m_EndPos;
                setHandLocate(m_iStep, m_iHandMax);
                m_Pos = m_StartPos;
                m_uiMode = 15;
                m_tqTimer = 200;
                m_vRot.set(0.0f, m_fEndDir, 0.0f, m_fEndScale);
                if (m_uiUser == 2) {
                    if (m_iStep == 1) {
                        if (m_iHand > 0) {
                            m_vRot.y = 0.0f;
                            m_fEndDir = 3.1415927f;
                            m_fStartDir = 0.0f;
                        }
                    }
                }
            }
        } else {
            moveCard(200, 300);
        }
        break;
    }
}
void CBattleCard::preStateDestroy() {
    CB_LAYOUT* pLayout;
    if (m_iState == 8 || m_iState == 2) {
        if (m_iStock == -1) {
            switch (m_uiUser) {
            case 1: pLayout = &g_LayoutData[7]; break;
            case 2: pLayout = &g_LayoutData[16]; break;
            }
            m_StartPos.set(pLayout->x, pLayout->y);
            m_fStartScale = pLayout->scale * 0.01f;
            pLayout = &g_LayoutData[1];
            m_EndPos.set(pLayout->x, pLayout->y);
            m_fEndScale = pLayout->scale * 0.01f;
        } else {
            switch (m_uiUser) {
            case 1: pLayout = &g_LayoutData[27 + m_iStock]; break;
            case 2: pLayout = &g_LayoutData[33 + m_iStock]; break;
            }
            m_StartPos.set(pLayout->x, pLayout->y);
            m_fStartScale = pLayout->scale * 0.01f;
            switch (m_iStockCount) {
            case 0:
            case 1: pLayout = &g_LayoutData[1]; break;
            case 2: pLayout = &g_LayoutData[2 + m_iStock]; break;
            case 3: pLayout = &g_LayoutData[4 + m_iStock]; break;
            }
            m_EndPos.set(pLayout->x, pLayout->y);
            m_fEndScale = pLayout->scale * 0.01f;
        }
        if (m_uiMode != 7) m_tqTimer = 400;
        m_Pos = m_StartPos;
        m_vRot.w = m_fStartScale;
        m_iLayer = pLayout->layer;
        m_pCard->changeLayer(m_iLayer);
    }
    m_iState = 3;
    m_uiFlag |= 0x1;
    m_iStock = -1;
    m_iStep = RND::genrand_int31() % 100;
}
void CBattleCard::stateDestroy() {
    switch (m_uiMode) {
    case 3:
        m_uiFlag |= 0x2008;
        m_tqTimer -= getFrameRate();
        if (m_tqTimer <= 0) {
            m_tqTimer = 0;
            m_uiMode = 0;
            destroy(1);
            return;
        }
        m_vRot.z = _adjustRad(m_vRot.z + getFrameRateF() * 5.0f * 3.1415927f / 180.0f);
        m_vRot.w = m_tqTimer / 400.0f;
        break;
    case 4: {
        m_uiFlag |= 0x2008;
        m_tqTimer -= getFrameRate();
        if (m_tqTimer <= 0) m_tqTimer = 0;
        INT dx = (m_EndPos.x - m_StartPos.x) * m_tqTimer / 400;
        INT dy = (m_EndPos.y - m_StartPos.y) * m_tqTimer / 400;
        m_Pos.x = m_EndPos.x - dx;
        m_Pos.y = m_EndPos.y - dy;
        if (m_vRot.w < 1.0f) m_vRot.w += getFrameRate() * 0.5f / 400.0f;
        if (m_vRot.w > 1.0f) m_vRot.w = 1.0f;
        if (abs(dx) <= 64 && abs(dy) <= 64) {
            m_tqTimer = 1000;
            m_uiMode = 5;
        }
        break;
    }
    case 5:
        m_uiFlag |= 0x2008;
        m_tqTimer -= getFrameRate();
        if (m_tqTimer <= 0) {
            m_tqTimer = 0;
            m_uiMode = 0;
            destroy(1);
            return;
        }
        m_Pos.x -= m_tqTimer / 200;
        m_Pos.y += (750 - m_tqTimer) * 60 / 1000;
        m_vRot.y = _adjustRad(m_vRot.y - getFrameRateF() * 5.0f * 3.1415927f / 180.0f);
        m_vRot.z = _adjustRad(m_vRot.z - getFrameRateF() * 2.0f * 3.1415927f / 180.0f);
        break;
    case 6:
        m_uiFlag |= 0x2008;
        m_tqTimer -= getFrameRate();
        if (m_tqTimer <= 0) {
            m_tqTimer = 0;
            m_uiMode = 0;
            destroy(1);
            return;
        }
        m_Pos.x -= ((m_iStep & 0xf) + 15) * m_tqTimer / 1200 * (1 - (m_uiUser & 0xfe));
        m_Pos.y += (m_iStep + 80) * (700 - m_tqTimer) / 1200;
        m_vRot.y = _adjustRad(m_vRot.y - getFrameRateF() * 3.0f * 3.1415927f / 180.0f);
        m_vRot.z = _adjustRad(m_vRot.z + getFrameRateF() * 30.0f * 3.1415927f / 180.0f);
        break;
    case 7:
        m_uiFlag |= 0x2008;
        m_tqTimer -= getFrameRate();
        if (m_tqTimer <= 0) {
            m_vRot.w = 0.0f;
            m_tqTimer = 0;
            m_uiMode = 0;
            destroy(1);
            return;
        }
        if (m_tqTimer < 600) {
            m_vRot.w = m_tqTimer * 2.0f / 1200.0f;
        } else {
            m_Pos.x += m_tqTimer / 240 * (1 - (m_uiUser & 0xfe));
            m_Pos.y -= m_tqTimer / 60;
        }
        break;
    }
}
void CBattleCard::moveCard(TIMEQ inTime, TIMEQ inRoll) {
    FLOAT sc;
    if (!inTime) return;
    INT x = m_EndPos.x - (m_EndPos.x - m_StartPos.x) * m_tqTimer / inTime;
    INT y = m_EndPos.y - (m_EndPos.y - m_StartPos.y) * m_tqTimer / inTime;
    sc = m_fEndScale - (m_fEndScale - m_fStartScale) * m_tqTimer / inTime;
    m_vRot.w = sc;
    m_Pos.x = x;
    m_Pos.y = y;
    if (!inRoll) return;
    m_vRot.y = m_fEndDir - (m_fEndDir - m_fStartDir) * m_tqTimer / inRoll;
}
INT CBattleCard::isNowSelect() {
    if (m_uiMode != 11) return FALSE;
    return m_iHand == 0;
}
void CBattleCard::setHand(const CARD& inData, INT inNum, INT inMax, INT inMode) {
    if (inMode) {
        m_StartPos = m_Pos;
    } else {
        m_pCard->destroy();
        if (!inData.m_nKind) {
            m_uiFlag |= 0x20;
            m_pCard->enableDisp(FALSE);
            m_cardData = inData;
            m_iHandMax = 0;
            m_iHand = -1;
            m_uiFlag = (INT)m_uiFlag & ~0x1;
            m_uiMode = 10;
            return;
        }
        m_pCard->create(inData);
        FVECTOR pos = {{m_Pos.x, m_Pos.y, 0.0f, 1.0f}};
        m_pCard->setPos(pos);
        m_pCard->enableDisp(FALSE);
        m_uiFlag = (INT)m_uiFlag & ~0x20;
        if (inData.m_nKind == 0xa0) m_pCard->changeTime(0);
    }
    m_cardData = inData;
    if (!inMode && !(m_uiFlag & 0x1e00)) m_nNumber = m_cardData.m_nNumber;
    m_iHand = inNum;
    m_iHandMax = inMax;
    m_uiFlag |= 0x1;
    m_uiMode = 0xb;
    setHandLocate(m_iHand, m_iHandMax);
    if (inMode) {
        m_Pos = m_StartPos;
        m_uiMode = 0xc;
        if (m_pRevolver) m_tqTimer = m_pRevolver->getRollSpeed();
    }
    setRoomEffect(NULL);
    {
        CARD* pEnchant = CCardBattle::getEnchantCard(m_uiUser);
        if (!pEnchant || pEnchant->m_nKind != 0x4b) {
            setEnchantEffect(0x4b, 0);
            setEnchantEffect(0x57, 0);
        } else {
            setEnchantEffect(0x57, 0);
            setEnchantEffect(0x4b, 0);
        }
    }
    {
        /* MATCH: 64-bit bit-field category read, see updateNumberEffect */
        struct _bits { u_long b0 : 28; u_long cat1 : 4; };
        if (((_bits*)&m_cardData)->cat1 == 4) m_uiFlag |= 0x8000;
    }
}
void CBattleCard::setHandLocate(INT inNum, INT inMax) {
    CB_LAYOUT* pLayout;
    INT n = 0;
    if (m_uiUser == 1) {
        if ((inMax >= 3 && inNum == inMax - 1) || inNum == -1) {
            pLayout = &g_LayoutData[11];
        } else {
            if (inNum < 4) {
                if (inNum >= 0) {
                    pLayout = &g_LayoutData[7 + inNum];
                    goto set1;
                }
            }
            m_tqTimer = 0;
            m_uiMode = 10;
            if (DebugWork::g_dw.player.iDeckDbgDisp) {
                n = inNum * 25;
                pLayout = &g_LayoutData[15];
            } else {
                m_uiFlag = (INT)m_uiFlag & ~0x1;
                if (inNum == inMax - 2) pLayout = &g_LayoutData[12];
                else pLayout = &g_LayoutData[13];
            }
        }
    set1:
        m_Pos.set(pLayout->x, pLayout->y);
        m_iLayer = pLayout->layer;
        m_Pos.x += n;
        m_vRot.set(0.0f, 0.0f, 0.0f, pLayout->scale * 0.01f);
        m_fStartDir = 0.0f;
        m_fEndDir = 0.0f;
        m_fEndScale = m_fStartScale = m_vRot.w;
    } else {
        if (inNum == 0) {
            pLayout = &g_LayoutData[16];
            m_fEndDir = 0.0f;
            m_vRot.y = 3.1415927f;
            m_fStartDir = 3.1415927f;
        } else if (inNum >= 0) {
            if (inNum < 4) {
                pLayout = &g_LayoutData[16 + inNum];
                m_vRot.y = 3.1415927f;
                m_fStartDir = 3.1415927f;
                m_fEndDir = 3.1415927f;
            } else goto other;
        } else other: if (inNum == inMax - 1) {
            pLayout = &g_LayoutData[20];
            m_vRot.y = 3.1415927f;
            m_fStartDir = 3.1415927f;
            m_fEndDir = 3.1415927f;
        } else {
            m_tqTimer = 0;
            m_uiMode = 10;
            if (DebugWork::g_dw.player.iDeckDbgDisp) {
                n = inNum * 35;
                pLayout = &g_LayoutData[23];
            } else {
                m_uiFlag = (INT)m_uiFlag & ~0x1;
                pLayout = &g_LayoutData[21];
            }
            m_vRot.y = 0.0f;
            m_fStartDir = 0.0f;
            m_fEndDir = 0.0f;
        }
        m_Pos.set(pLayout->x, pLayout->y);
        m_iLayer = pLayout->layer;
        m_vRot.x = m_vRot.z = 0.0f;
        m_fEndScale = m_fStartScale = m_vRot.w = pLayout->scale * 0.01f;
        m_Pos.y -= n;
    }
    m_EndPos = m_Pos;
    m_pCard->changeLayer(m_iLayer);
    m_uiFlag |= 0x2008;
}
void CBattleCard::hideHand() {
    m_uiMode = 13;
    m_tqTimer = 400;
    m_StartPos = m_Pos;
    m_fStartScale = m_vRot.w;
    CB_LAYOUT* pLay = m_uiUser == 1 ? &g_LayoutData[13] : &g_LayoutData[21];
    m_EndPos.set(pLay->x, pLay->y);
    m_fEndScale = pLay->scale * 0.01f;
}
void CBattleCard::outHand() {
    m_uiMode = 14;
    m_iHand = -1;
    m_uiFlag = (INT)m_uiFlag & ~0x1;
    m_uiFlag = (INT)m_uiFlag & ~0x2008;
}
void CBattleCard::updateReload(INT inNum) {
    if (m_cardData.m_nKind == 0xa0) m_pCard->changeTime(inNum);
}
void CBattleCard::resetHand(INT inNum, INT inMax) {
    CB_LAYOUT* pLayout;
    changeState(2);
    m_iHand = inNum;
    m_iHandMax = inMax;
    m_iStock = -1;
    if (m_uiUser == 1) {
        pLayout = &g_LayoutData[14];
        m_StartPos.set(pLayout->x, pLayout->y);
        m_fStartScale = pLayout->scale * 0.01f;
        pLayout = &g_LayoutData[7];
        m_EndPos.set(pLayout->x, pLayout->y);
        m_fEndScale = pLayout->scale * 0.01f;
    } else {
        pLayout = &g_LayoutData[22];
        m_StartPos.set(pLayout->x, pLayout->y);
        m_fStartScale = pLayout->scale * 0.01f;
        pLayout = &g_LayoutData[16];
        m_EndPos.set(pLayout->x, pLayout->y);
        m_fEndScale = pLayout->scale * 0.01f;
    }
    m_tqTimer = 200;
    m_Pos = m_StartPos;
    m_vRot.w = m_fStartScale;
    m_iLayer = pLayout->layer;
    m_pCard->changeLayer(m_iLayer);
    m_uiMode = 0xf;
    m_iStep = 0;
    m_uiFlag = (m_uiFlag | 1) & ~0x210044;
    testDisp();
    m_uiFlag = ((INT)m_uiFlag | 0x4000) & ~0x10;
    m_pCard->enableColorCtrl(FALSE);
    resetEnchantEffect(0);
}
void CBattleCard::setSelect(INT inFlag) {
    if (m_pCard) {
        if (inFlag) {
            if (isPlayNG()) m_pCard->select(TRUE);
            else m_pCard->select(FALSE);
        } else {
            m_pCard->unselect();
        }
    }
}
void CBattleCard::setRoomEffect(const CARD* inData) {
    const CARD* pRoom;
    struct _bits { u_long b0 : 24; u_long cat0 : 4; };
    INT cat = m_cardData.m_nCategory0;
    if (!cat) return;
    if (cat >= 6) {
        if (cat != 7) return;
        if (m_cardData.m_nNumber == -1) return;
    }
    if (m_uiFlag & 0x100) return;
    if (!(m_uiFlag & 0x1e00)) m_nNumber = m_cardData.m_nNumber;
    m_nNumberR = -1;
    pRoom = inData;
    if (!pRoom) {
        pRoom = CCardBattle::getRoomCard();
        if (!pRoom) return;
    }
    switch (pRoom->m_nKind) {
    case 0xa3:
        if (m_uiUser == 2) {
            m_nNumberR = m_nNumber - 2;
            if (m_nNumberR <= 0) m_nNumberR = 1;
        }
        break;
    case 0xa4:
        if (m_uiUser == 2) {
            m_nNumberR = m_nNumber + 2;
            if (m_nNumberR >= 10) m_nNumberR = 9;
        }
        break;
    case 0xac:
        if (m_uiUser == 1 && ((_bits*)&m_cardData)->cat0 == 1) {
            m_nNumberR = m_nNumber + 2;
            if (m_nNumberR >= 10) m_nNumberR = 9;
        }
        break;
    case 0xad:
        if (m_uiUser == 1 && (((_bits*)&m_cardData)->cat0 == 2 || ((_bits*)&m_cardData)->cat0 == 3)) {
            m_nNumberR = m_nNumber + 2;
            if (m_nNumberR >= 10) m_nNumberR = 9;
        }
        break;
    case 0xae:
        if (m_uiUser == 1 && ((_bits*)&m_cardData)->cat0 == 4) {
            m_nNumberR = m_nNumber + 2;
            if (m_nNumberR >= 10) m_nNumberR = 9;
        }
        break;
    default:
        return;
    }
    if (m_nNumberR != -1) {
        m_cardData.m_nNumber = m_nNumberR;
        m_pCard->replaceNumberBlue(m_nNumberR);
        m_uiFlag |= 0x100;
    }
}
void CBattleCard::setEnchantEffect(INT inKind, UINT inFlag) {
    INT cat = m_cardData.m_nCategory0;
    if (!cat) return;
    if (cat >= 6) {
        if (cat != 7) return;
        if (m_cardData.m_nNumber < 0) return;
    }
    {
        CARD* pEnchant = CCardBattle::getEnchantCard(m_uiUser);
        if (!pEnchant || pEnchant->m_nKind != inKind) {
            if (inKind == 0x4b || inKind == 0x57) resetEnchantEffect(inKind);
            return;
        }
    }
    switch (inKind) {
    case 0x44:
        if (m_cardData.m_nNumber < 9) m_cardData.m_nNumber++;
        if (m_pRevolver) m_pRevolver->setNumberEffect(0x44);
        m_nNumberE = m_cardData.m_nNumber;
        m_pCard->replaceNumberBlue(m_nNumberE);
        m_uiFlag |= 0x200;
        break;
    case 0x53:
        if (m_cardData.m_nNumber > 0) m_cardData.m_nNumber--;
        if (m_pRevolver) m_pRevolver->setNumberEffect(0x53);
        m_nNumberE = m_cardData.m_nNumber;
        m_pCard->replaceNumberBlue(m_nNumberE);
        m_uiFlag |= 0x200;
        break;
    case 0x52:
        if (!inFlag) {
            m_nNumberE = RND::genrand_int31() % 10;
            m_cardData.m_nNumber = m_nNumberE;
            m_pCard->replaceNumberBlue(m_nNumberE);
            m_uiFlag = ((INT)m_uiFlag & ~0x400) | 0x200;
        } else {
            if (m_uiFlag & 0x400) break;
            m_pCard->replaceNumberRoulette();
            m_uiFlag |= 0x400;
        }
        break;
    case 0x4b: {
        if (m_uiFlag & 0x800) break;
        m_nNumberE = (10 - m_nNumber) % 10;
        m_cardData.m_nNumber = m_nNumberE;
        m_pCard->replaceNumberBlue(m_nNumberE);
        m_uiFlag |= 0x800;
        break;
    }
    case 0x57:
        if (m_uiFlag & 0x1000) break;
        m_cardData.m_nNumber = 0;
        m_nNumberE = 0;
        m_pCard->replaceNumberBlue(0);
        m_uiFlag |= 0x1000;
        break;        break;
    case 0x65:
        break;
    }
}
void CBattleCard::resetEnchantEffect(INT inKind) {
    UINT uiMask;
    switch (inKind) {
    case 0x52: uiMask = 0x400; break;
    case 0x4b: uiMask = 0x800; break;
    case 0x57: uiMask = 0x1000; break;
    case 0: uiMask = 0x1e00; break;
    default: uiMask = 0x200; break;
    }
    if (m_uiFlag & uiMask) {
        if (!(m_uiFlag & 0x100)) {
            m_cardData.m_nNumber = m_nNumber;
            m_pCard->replaceNumberNormal(m_nNumber);
        } else {
            m_cardData.m_nNumber = m_nNumberR;
            m_pCard->replaceNumberBlue(m_nNumberR);
        }
        m_uiFlag &= ~uiMask;
    }
}
void CBattleCard::setNumberBreak(INT inNum) {
    m_cardData.m_nNumber -= inNum;
    if (m_cardData.m_nNumber < 0) m_cardData.m_nNumber = 0;
    m_nNumberE = m_cardData.m_nNumber;
    m_pCard->replaceNumberBlue(m_nNumberE);
    m_uiFlag |= 0x200;
}
void CBattleCard::updateNumberEffect() {
    if (!(m_uiFlag & 0x100)) {
        m_cardData.m_nNumber = m_nNumber;
        m_pCard->replaceNumberNormal(m_nNumber);
    } else {
        m_cardData.m_nNumber = m_nNumberR;
        m_pCard->replaceNumberBlue(m_nNumberR);
    }
    resetEnchantEffect(0);
    {
        CARD* pEnchant = CCardBattle::getEnchantCard(m_uiUser);
        if (!pEnchant || pEnchant->m_nKind != 0x4b) {
            setEnchantEffect(0x4b, 0);
            setEnchantEffect(0x57, 0);
        } else {
            setEnchantEffect(0x57, 0);
            setEnchantEffect(0x4b, 0);
        }
    }
    {
        /* MATCH: the category is read through a 64-bit bit-field over m_cardData (ld + mask); getCardCategory()'s
           BYTE bit-field gives lbu, and the header form changes Game/CardRevolver */
        struct _bits { u_long b0 : 28; u_long cat1 : 4; };
        if (((_bits*)&m_cardData)->cat1 == 4) m_uiFlag |= 0x8000;
    }
}
void CBattleCard::setDuel(INT inMode) {
    changeState(4);
    if (!inMode) m_uiMode = 16;
}
void CBattleCard::setDuelRefrect() {
    changeState(3);
    m_tqTimer = 1000;
    m_uiMode = 5;
    m_uiFlag = (INT)m_uiFlag & ~0x2;
    if (m_iStock && m_cardData.m_nCategory1 == 4) m_uiFlag = (INT)m_uiFlag & ~0x20;
}
void CBattleCard::setDuelStock(UINT inPri) {
    if (m_pRevolver) {
        changeState(4);
        m_iStock = inPri;
        m_uiMode = 8;
        m_iHand = -1;
        m_uiFlag = (INT)(m_uiFlag | 0x200000) & ~0x2;
        m_pRevolver->setPlayerDuelStock(this, m_iStock);
    }
}
void CBattleCard::setPlayDuelStock(INT inCount) {
    CB_LAYOUT* pLayout;
    m_uiMode = 1;
    m_iStockCount = inCount;
    pLayout = &g_LayoutData[36 + m_iStock];
    m_StartPos.set(pLayout->x, pLayout->y);
    m_fStartScale = pLayout->scale * 0.01f;
    switch (inCount) {
    case 0: pLayout = &g_LayoutData[1]; break;
    case 3: pLayout = &g_LayoutData[43 + m_iStock]; break;
    case 5: pLayout = &g_LayoutData[46 + m_iStock]; break;
    case 7: pLayout = &g_LayoutData[51 + m_iStock]; break;
    }
    m_EndPos.set(pLayout->x, pLayout->y);
    m_fEndScale = pLayout->scale * 0.01f;
    m_tqTimer = 400;
    m_Pos = m_StartPos;
    m_vRot.w = m_fStartScale;
    m_iLayer = pLayout->layer;
    m_pCard->changeLayer(m_iLayer);
    m_uiFlag = (INT)(m_uiFlag | 0x40) & ~0x2;
    if (m_uiFlag & 0x10) {
        m_uiFlag = (INT)m_uiFlag & ~0x10;
        m_pCard->enableColorCtrl(FALSE);
    }
    m_uiFlag &= ~0x200000;
}
void CBattleCard::preStateDuel() {
    CB_LAYOUT* pLayout;
    switch (m_uiMode) {
    case 0x10:
        if (m_iStock == -1) {
            switch (m_uiUser) {
            case 1: pLayout = &g_LayoutData[7]; break;
            case 2: pLayout = &g_LayoutData[16]; break;
            }
            m_StartPos.set(pLayout->x, pLayout->y);
            pLayout = &g_LayoutData[1];
            m_EndPos.set(pLayout->x, pLayout->y);
            m_fEndScale = pLayout->scale * 0.01f;
        } else {
            switch (m_uiUser) {
            case 1: pLayout = &g_LayoutData[27 + m_iStock]; break;
            case 2: pLayout = &g_LayoutData[33 + m_iStock]; break;
            }
            m_StartPos.set(pLayout->x, pLayout->y);
            switch (m_iStockCount) {
            case 0:
            case 1: pLayout = &g_LayoutData[1]; break;
            case 2: pLayout = &g_LayoutData[2 + m_iStock]; break;
            case 3: pLayout = &g_LayoutData[4 + m_iStock]; break;
            }
            m_EndPos.set(pLayout->x, pLayout->y);
            m_fEndScale = pLayout->scale * 0.01f;
        }
        m_fStartScale = m_fEndScale;
        m_StartPos.x = m_EndPos.x - (m_EndPos.x - m_StartPos.x) / 3;
        m_StartPos.y = m_EndPos.y - (m_EndPos.y - m_StartPos.y) / 3;
        m_tqTimer = 3000;
        m_Pos = m_StartPos;
        m_vRot.w = m_fEndScale;
        m_iLayer = pLayout->layer;
        m_pCard->changeLayer(m_iLayer);
        m_iStock = -1;
        m_uiFlag |= 0x40;
        break;
    case 8:
        m_uiFlag |= 0x1;
        pLayout = &g_LayoutData[1];
        m_StartPos.set(pLayout->x, pLayout->y);
        m_fStartScale = pLayout->scale * 0.01f;
        pLayout = &g_LayoutData[36 + m_iStock];
        m_EndPos.set(pLayout->x, pLayout->y);
        m_fEndScale = pLayout->scale * 0.01f;
        m_tqTimer = 400;
        m_Pos = m_StartPos;
        m_vRot.w = m_fStartScale;
        m_iLayer = pLayout->layer;
        m_pCard->changeLayer(m_iLayer);
        break;
    }
    m_iState = 4;
    m_uiFlag |= 0x1;
    if (m_uiFlag & 0x10) {
        m_uiFlag = (INT)m_uiFlag & ~0x10;
        m_pCard->enableColorCtrl(FALSE);
    }
}
void CBattleCard::stateDuel() {
    switch (m_uiMode) {
    case 16: {
        m_uiFlag |= 0x2008;
        m_tqTimer -= getFrameRate();
        if (m_tqTimer <= 0) m_tqTimer = 3000;
        INT tq = abs(m_tqTimer - 1500);
        INT x = (m_EndPos.x - m_StartPos.x) * tq / 1500;
        INT y = (m_EndPos.y - m_StartPos.y) * tq / 1500;
        m_Pos.x = m_EndPos.x - x;
        m_Pos.y = m_EndPos.y - y;
        if (abs(x) <= 32 && abs(y) <= 32) m_tqTimer >>= 1;
        break;
    }
    case 8:
        m_uiFlag |= 0x2008;
        m_tqTimer -= getFrameRate();
        if (m_tqTimer <= 0) {
            m_uiMode = 9;
            m_uiFlag |= 0x4;
            m_tqTimer = 0;
            m_Pos = m_EndPos;
            m_vRot.w = m_fEndScale;
        } else {
            INT dx = (m_EndPos.x - m_StartPos.x) * m_tqTimer / 400;
            INT dy = (m_EndPos.y - m_StartPos.y) * m_tqTimer / 400;
            FLOAT sc = m_fEndScale - (m_fEndScale - m_fStartScale) * m_tqTimer / 400.0f;
            m_vRot.w = sc;
            m_Pos.x = m_EndPos.x - dx;
            m_Pos.y = m_EndPos.y - dy;
        }
        break;
    case 9: {
        m_tqTimer += getFrameRate();
        FLOAT r = m_tqTimer * 0.1f * 3.1415927f / 180.0f;
        m_Pos.y = (INT)(sinf(r) * 4.0f) + m_EndPos.y;
        break;
    }
    case 1: {
        m_uiFlag |= 0x2008;
        m_tqTimer -= getFrameRate();
        if (m_tqTimer <= 0) m_tqTimer = 0;
        INT dx = (m_EndPos.x - m_StartPos.x) * m_tqTimer / 400;
        INT dy = (m_EndPos.y - m_StartPos.y) * m_tqTimer / 400;
        FLOAT sc = m_fEndScale - (m_fEndScale - m_fStartScale) * m_tqTimer / 400.0f;
        m_vRot.w = sc;
        m_Pos.y = m_EndPos.y - dy;
        m_Pos.x = m_EndPos.x - dx;
        if (!isSet()) {
            if (abs(dx) <= 64 && abs(dy) <= 64) m_uiFlag |= 0x2;
        } else if (!m_tqTimer) {
            m_uiMode = 2;
        }
        break;
    }
    case 2:
        break;
    }
}
void CBattleCard::setSnatch(HANDLE inHandle) {
    changeState(5);
    m_uiMode = 17;
    m_hTarget = inHandle;
    m_tqTimer = 800;
    m_iHand = -1;
    m_uiFlag |= 0x20;
}
void CBattleCard::resetSnatch() {
    m_uiOwner = 2;
    _setCard(m_cardData, 0, 0xff);
    m_uiFlag |= 0x20;
    m_uiFlag = (INT)m_uiFlag & ~0x1;
    m_uiFlag = (INT)m_uiFlag & ~0x2;
    m_uiFlag = (INT)m_uiFlag & ~0x4;
    m_uiFlag = (INT)m_uiFlag & ~0x40;
    m_iHand = -1;
    m_uiMode = 0xe;
    m_iStock = -1;
    m_uiFlag &= ~0x10000;
    m_iHandMax = 0;
    m_vRot.set(0.0f, 0.0f, 0.0f, 1.0f);
    m_uiFlag = (INT)m_uiFlag & ~0x2008;
    changeState(2);
    resetEnchantEffect(0);
}
void CBattleCard::preStateSnatch() {
    m_iState = 5;
    m_uiFlag |= 0x1;
    CB_LAYOUT& lay = g_LayoutData[7];
    m_StartPos.set(lay.x, lay.y);
    m_fStartScale = lay.scale * 0.01f;
    m_EndPos.set(lay.x + 50, lay.y - 100);
    m_fEndScale = m_fStartScale;
    m_tqTimer = 800;
    m_Pos = m_StartPos;
    m_vRot.w = m_fStartScale;
    m_iLayer = lay.layer;
    m_pCard->changeLayer(m_iLayer);
}
void CBattleCard::stateSnatch() {
    switch (m_uiMode) {
    case 0x11:
        m_uiFlag |= 0x2008;
        m_tqTimer -= getFrameRate();
        if (m_tqTimer <= 0) {
            m_tqTimer = 1600;
            m_uiMode = 0x12;
            m_Pos = m_EndPos;
            m_StartPos = m_Pos;
            m_vRot.w = m_fEndScale;
        } else {
            INT dx = (m_EndPos.x - m_StartPos.x) * m_tqTimer / 800;
            INT dy = (m_EndPos.y - m_StartPos.y) * m_tqTimer / 800;
            m_Pos.x = m_EndPos.x - dx;
            m_Pos.y = m_EndPos.y - dy;
        }
        break;
    case 0x12: {
        m_uiFlag |= 0x2008;
        m_tqTimer -= getFrameRate();
        CEntity* pEntity = (CEntity*)CTaskMan::getTask(m_hTarget);
        if (pEntity) {
            FVECTOR pos;
            IVECTOR out;
            pEntity->getLockOnPos(pos);
            _setRotTransPersMatrix(CWorldEnv::getWorldScreenMatrix());
            RotTransPersClipW(out, pos);
            if (!out.w && out.x >= 0x5000 && out.x <= 0xb000 && out.y >= 0x5000 && out.y <= 0xb000) {
                m_EndPos.x = (out.x >> 4) - 0x700;
                m_EndPos.y = (out.y >> 4) - 0x740;
            }
            INT dx = (m_EndPos.x - m_StartPos.x) * m_tqTimer / 1600;
            INT dy = (m_EndPos.y - m_StartPos.y) * m_tqTimer / 1600;
            m_Pos.x = m_EndPos.x - dx;
            m_Pos.y = m_EndPos.y - dy;
            if (m_tqTimer <= 0) {
                INT count = CCardBattle::getStockCount(2);
                if (count == 3) pEntity->attachEffect(g_szDpxZexion2, 10, &pEntity->Crd(1), (const FVECTOR*)NULL, (const FVECTOR*)NULL, 0.0f);
                else pEntity->attachEffect(g_szDpxZexion2, 11, &pEntity->Crd(1), (const FVECTOR*)NULL, (const FVECTOR*)NULL, 0.0f);
            }
        }
        m_vRot.y = _adjustRad(m_vRot.y + getFrameRateF() * DEG2RAD((1600 - m_tqTimer) * 0.01f));
        m_vRot.z = _adjustRad(m_vRot.z + getFrameRateF() * DEG2RAD(m_cardData.m_nNumber * 0.5f));
        m_vRot.w = m_tqTimer / 1600.0f;
        if (m_tqTimer <= 0) {
            m_tqTimer = 0;
            TRACE("PLCR:: snatchCard =\n");
            TRACE("  [Card%03d:%d] iStock=%d, iHand=%d/%d, uiFlag=%08x, hEnemy=%d,\n", m_cardData.m_nKind, m_cardData.m_nNumber, m_iStock, m_iHand, m_iHandMax, m_uiFlag, m_hTarget);
            CCardBattle* pCB = (CCardBattle*)CTaskMan::getSysTask(0x16);
            if (pCB) {
                CCardRevolver* pRV = pCB->getRevolver(2);
                if (pRV) pRV->addCard(&m_cardData, 0);
            }
            m_uiOwner = 2;
            destroy(1);
        }
        break;
    }
    }
}
void CBattleCard::setPronounce(UINT inPri) {
    changeState(6);
    m_uiMode = 8;
    m_iStock = inPri;
    m_iHand = -1;
    m_uiFlag = (INT)m_uiFlag & ~0x2;
}
void CBattleCard::setPlayPronounce() {
    m_uiMode = 1;
    m_StartPos = m_Pos;
    m_fStartScale = m_vRot.w;
    CB_LAYOUT& lay = g_LayoutData[1];
    m_EndPos.set(lay.x, lay.y);
    m_fEndScale = lay.scale * 0.01f;
    m_tqTimer = 400;
    m_uiFlag = (INT)(m_uiFlag | 0x40) & ~0x2;
    if (m_uiFlag & 0x10) {
        m_uiFlag = (INT)m_uiFlag & ~0x10;
        m_pCard->m_seqControl.fgColor = 0;
    }
    m_uiFlag |= 0x20020;
}
void CBattleCard::preStatePronounce() {
    CB_LAYOUT* pLayout;
    m_iState = 6;
    m_uiFlag |= 0x1;
    pLayout = &g_LayoutData[16];
    m_StartPos.set(pLayout->x, pLayout->y);
    m_fStartScale = pLayout->scale * 0.01f;
    pLayout = &g_LayoutData[59 + m_iStock];
    m_EndPos.set(pLayout->x, pLayout->y);
    m_fEndScale = pLayout->scale * 0.01f;
    m_tqTimer = 400;
    m_Pos = m_StartPos;
    m_vRot.w = m_fStartScale;
    m_iLayer = pLayout->layer;
    m_pCard->changeLayer(m_iLayer);
    if (m_iStock > 0) m_vRot.y = 3.1415927f;
    m_uiFlag |= 0x2008;
    if (m_uiFlag & 0x10) {
        m_uiFlag = (INT)m_uiFlag & ~0x10;
        m_pCard->enableColorCtrl(FALSE);
    }
}
void CBattleCard::statePronounce() {
    switch (m_uiMode) {
    case 2:
        break;
    case 8:
        m_uiFlag |= 0x2008;
        m_tqTimer -= getFrameRate();
        if (m_tqTimer <= 0) {
            m_uiMode = 9;
            m_uiFlag |= 0x4;
            m_tqTimer = 0;
            m_Pos = m_EndPos;
            m_vRot.w = m_fEndScale;
        } else {
            INT dx = (m_EndPos.x - m_StartPos.x) * m_tqTimer / 400;
            INT dy = (m_EndPos.y - m_StartPos.y) * m_tqTimer / 400;
            FLOAT sc = m_fEndScale - (m_fEndScale - m_fStartScale) * m_tqTimer / 400.0f;
            m_vRot.w = sc;
            m_Pos.x = m_EndPos.x - dx;
            m_Pos.y = m_EndPos.y - dy;
        }
        break;
    case 1: {
        m_uiFlag |= 0x2008;
        m_tqTimer -= getFrameRate();
        if (m_tqTimer <= 0) m_tqTimer = 0;
        INT dx = (m_EndPos.x - m_StartPos.x) * m_tqTimer / 400;
        INT dy = (m_EndPos.y - m_StartPos.y) * m_tqTimer / 400;
        FLOAT sc = m_fEndScale - (m_fEndScale - m_fStartScale) * m_tqTimer / 400.0f;
        m_vRot.w = sc;
        m_Pos.y = m_EndPos.y - dy;
        m_Pos.x = m_EndPos.x - dx;
        if (m_iStock > 0) m_vRot.y = 3.1415927f - 3.1415927f * m_tqTimer / 400.0f;
        if (!isSet()) {
            if (abs(dx) <= 64 && abs(dy) <= 64) m_uiFlag |= 0x2;
        } else if (!m_tqTimer) {
            m_uiMode = 2;
            m_vRot.y = 0.0f;
        }
        break;
    }
    }
}
void CBattleCard::setDiscard(INT inFlag) {
    m_uiMode = 20;
    if (inFlag) {
        m_uiOwner = 2;
        m_uiFlag |= 0x20;
        destroy(0);
    } else {
        outHand();
    }
}
// SOURCE DATA: .data .rodata .bss
