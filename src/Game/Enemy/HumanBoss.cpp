#define GPRIM_H_PATH "System/Graphic/GPrim.h" // MATCH: the original spelled the include "GPrim.h"
#define COLLOBJ_H_ENUMS_ONLY /* MATCH: the collision classes come later (their vtables follow CHumanBoss's) */
#define BATTLEENEMY_H_PATH "c:/KHCOM/MainApp/Game/Enemy/BattleEnemy.h" // MATCH: the original included it by its full path
#include "System/TaskMan.h"
#include "System/Task.h"
#include "System/Graphic/Gprim.h"
#include "System/Graphic/2d/2DMan.h"
#include "Game/CardBattle.h"
#include "Game/Enemy/BattleEnemy.h"
#include "Game/Enemy/EnemyParam.h"
#include "Game/Enemy/HumanBoss.h"
#include "Game/Collision/CollUtilA.h"
#include "Game/Collision/CollUtil.h"
#include "System/Graphic/Anim.h"
#include "System/Graphic/Str.h"

/* MATCH: placeholder - .rodata has an empty string literal after Str.h's strings (from an unidentified header). */
inline void _humanBossEmptyString() {
    TRACE("");
}

#include "System/Graphic/Effect/effect.h"
#include "Game/BodyCollision.h"

#undef COLLOBJ_H_ENUMS_ONLY
#include "Game/Collision/CollObj.h"
#include "Game/CardKind.h"
#include "Game/Debug/DebugWork.h"
#include "System/Math3D.h"
#include "System/Rnd.h"
#include "include_asm.h"
#include <math.h>
#include "Game/Collision/CollBody.h"
/* Inline in the original's collision header: the weak copies of its destructors and collideMap end this
 * object. */






#include "Game/Enemy/BossDeadDemo.h"

inline void* operator new(size_t, void* p) throw() { return p; }

#define RET_ADDR() ({ UINT nRA; __asm__ volatile("daddu %0, $31, $0" : "=r"(nRA)); nRA; })

inline void* operator new[](size_t size, char* msg) throw() {
    return com_malloc_d(size, msg, RET_ADDR());
}
inline void operator delete[](void* mem) { com_free(mem); }

#define ASSERT(e) if (!(e)) AssertApp(__FILE__, __LINE__, #e, 0)

void AddVector(FVECTOR& v0, const FVECTOR& v1, const FVECTOR& v2);

extern CEntity* g_pPlayer; /* a CPlayer (Game/Player/Player.h) */
/* CPlayer's HP: m_iHp (0x320), m_iHpMax (0x324) */
static inline INT _playerHp(CEntity* p) { return *(INT*)((UBYTE*)p + 0x320); }
static inline INT _playerHpMax(CEntity* p) { return *(INT*)((UBYTE*)p + 0x324); }

/* Game/CardKind's card table: a static in the original's header, emitted (with its strings) at the end of
 * .rodata. */
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

CHumanBoss::CARD_DATA HumanBoss::g_CardDataTbl[12] = {
    { 101, { 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01 } },
    { 106, { 0x81, 0x82, 0x81, 0x81, 0x81, 0x81, 0x81, 0x81 } },
    { 107, { 0x83, 0x83, 0x83, 0x83, 0x84, 0x84, 0x84, 0x84 } },
    { 118, { 0x89, 0x8a, 0x89, 0x89, 0x89, 0x89, 0x89, 0x89 } },
    { 114, { 0x92, 0x92, 0x92, 0x92, 0x93, 0x93, 0x93, 0x93 } },
    { 116, { 0x8e, 0x8f, 0x90, 0x91, 0x8e, 0x8e, 0x8e, 0x8e } },
    { 117, { 0x8b, 0x8c, 0x8b, 0x8b, 0x8d, 0x8d, 0x8d, 0x8d } },
    { 112, { 0x85, 0x86, 0x87, 0x85, 0x88, 0x88, 0x88, 0x88 } },
    { 119, { 0x94, 0x95, 0x94, 0x94, 0x96, 0x96, 0x96, 0x96 } },
    { 121, { 0x99, 0x9a, 0x99, 0x99, 0x9b, 0x9b, 0x9b, 0x9b } },
    { 122, { 0x9c, 0x9c, 0x9c, 0x9c, 0x9d, 0x9d, 0x9d, 0x9d } },
    { 124, { 0x12, 0x12, 0x12, 0x12, 0x9e, 0x9e, 0x9e, 0x9e } },
};

INT HumanBoss::GetPlayerHpPercent() {
#line 214
    ASSERT(g_pPlayer);
    return _playerHp(g_pPlayer) * 100 / _playerHpMax(g_pPlayer);
}

CHumanBoss::CHumanBoss(CTask* inParent, CState* inDefaultState, CState* inMainState)
    : CBattleEnemy(inParent, inDefaultState, inMainState) {
    m_iNormalActNum = 0;
    m_pNormalActs = __null;
    m_iStockNum = 0;
    m_pStockData = __null;
    m_iStockCardCount = 0;
    m_pUseStock = __null;
    m_pCardData = __null;
    m_iActCount = 0;
    m_pDoBattleAct = __null;
    m_iNoReloadNum = 15;
    m_iRemainCardNum = 1;
    m_uiHumanBossFlag = 1;
    m_iStockRate = 2;
    m_iUseStockRate = 3;
    m_iTotalCardNum = 0;
    m_iStockTotalNumber = 0;
    m_iStockRollCount = 0;
}

CHumanBoss::~CHumanBoss() {
    if (m_pNormalActs) delete[] m_pNormalActs;
    m_pNormalActs = __null;
    if (m_pStockData) delete[] m_pStockData;
    m_pStockData = __null;
}

BOOL CHumanBoss::initalize(INT inGroupID, INT inRsrcID, UINT iFlag) {
    INT rate;

    if (!CBattleEnemy::initalize(inGroupID, inRsrcID, iFlag)) {
        return FALSE;
    }
    setupCardRevolver();
    setupAct();
    setTexAnmSeq(0, HumanBoss::NormalEyeSeqName);
    setTexAnmSeq(1, HumanBoss::NormalLipSeqName);
    setEntityFlag(0, 0x10000);
    /* MATCH: cast store = no struct-field alias with the global load below (it stays before that load) */
    *(INT*)&m_iDeadSeIdentifyNo = getParam() + 1;
    rate = getExtraParam(HumanBoss::UseItemCardParamName);
    m_iNoReloadNum = getAttackCardNum(3) * rate / 100;
    return TRUE;
}

/* Parked: 20 lines (2026-11-05, was 27): the goto-built outer loop reloads iCardCount/pCards like the original (a for loop hoists them); left: the original hoists the m_pEnemyParam load once (t2), ours reloads it too. The ref lists card/j at function level and no getEnemyParam inline
   2026-11-07: a local for getEnemyParam() before the goto loop: 23 (the m_pEnemyParam reload goes, but iCardCount's load
   moves to the top of the body); for/do-while forms with or without it: 27.
   2026-11-07: a plain for loop reading m_pEnemyParam directly matches, but m_pEnemyParam is private in the ref
   (CHumanBoss is no friend): not the original source, reverted. Lead: the ref declares `ENEMY_PARAM* getEnemyParam();`
   non-const (ours is const) - but this method is const, so check how the original reached it. */
#if 0
INT CHumanBoss::getAttackCardNum(UINT inFlag) const {
    INT cardNum;
    INT i;
    ENEMY_CARD* card;
    INT j;
    cardNum = 0;
    i = 0;
    if (i < getEnemyParam()->iCardCount) {
    loop:
        card = &getEnemyParam()->pCards[i];
        for (j = 0; j < 10; j++) {
            if ((inFlag & 1) && (UINT)card->iCardKind < 4) {
                cardNum += card->bNumber[j];
            }
            if ((inFlag & 2) && (UINT)card->iCardKind - 4 < 4) {
                cardNum += card->bNumber[j];
            }
        }
        if (++i < getEnemyParam()->iCardCount) goto loop;
    }
    return cardNum;
}
#else
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Game/Enemy/HumanBoss", getAttackCardNum__C10CHumanBossUi);
#endif

INT CHumanBoss::getRemainCard(UINT inFlag) {
    INT num = 0;
    if (inFlag & 1) {
        num = m_pCardRevolver->getRemain(0, 0, 1);
    }
    if (inFlag & 2) {
        num += m_pCardRevolver->getRemain(0, 0, 2);
    }
    return num;
}

INT CHumanBoss::getRemainEnemyCard(INT inKind) {
    return m_pCardRevolver->getRemain(1, inKind, 0);
}

INT CHumanBoss::getHaveCard(UINT inFlag) {
    INT num = 0;
    if (inFlag & 1) {
        num = m_pCardRevolver->getHave(1, 1, 1, 0, 0);
    }
    if (inFlag & 2) {
        num += m_pCardRevolver->getHave(2, 1, 1, 0, 0);
    }
    return num;
}

void CHumanBoss::updateAfter() {
    CBattleEnemy::updateAfter();
    if (!isCurrentState(0)) {
        playMyRemainHpVoice();
        playPlayerRemainHpVoice();
    }
}

void CHumanBoss::beginStateSmallDamage() {
    setTexAnmSeq(0, HumanBoss::DamageEyeSeqName);
    setTexAnmSeq(1, HumanBoss::DamageLipSeqName);
    CBattleEnemy::beginStateSmallDamage();
}

void CHumanBoss::endStateSmallDamage() {
    setTexAnmSeq(0, HumanBoss::NormalEyeSeqName);
    setTexAnmSeq(1, HumanBoss::NormalLipSeqName);
    CBattleEnemy::endStateSmallDamage();
}

void CHumanBoss::beginStateBigDamage() {
    setTexAnmSeq(0, HumanBoss::DamageEyeSeqName);
    setTexAnmSeq(1, HumanBoss::DamageLipSeqName);
    CBattleEnemy::beginStateBigDamage();
}

void CHumanBoss::endStateBigDamage() {
    setTexAnmSeq(0, HumanBoss::NormalEyeSeqName);
    setTexAnmSeq(1, HumanBoss::NormalLipSeqName);
    CBattleEnemy::endStateBigDamage();
}

void CHumanBoss::beginStateFaint() {
    setTexAnmSeq(0, HumanBoss::FaintEyeSeqName);
    setTexAnmSeq(1, HumanBoss::FaintLipSeqName);
    CBattleEnemy::beginStateFaint();
}

void CHumanBoss::endStateFaint() {
    setTexAnmSeq(0, HumanBoss::NormalEyeSeqName);
    setTexAnmSeq(1, HumanBoss::NormalLipSeqName);
    CBattleEnemy::endStateFaint();
}

void CHumanBoss::beginStateItem() {
    INT id = getExtraParam(HumanBoss::ItemMotionParamName);
    INT effectNo = 0;

    switch (m_iUseItem) {
    case 0x2a:
        effectNo = 0;
        break;
    case 0x2b:
        effectNo = 1;
        break;
    case 0x2c:
        effectNo = 2;
        break;
    case 0x2d:
        effectNo = 3;
        break;
    case 0x2e:
        effectNo = 4;
        break;
    case 0x2f:
        effectNo = 5;
        break;
    }
    changeMotion(id, 0, FALSE, -1, effectNo, 0);
}

void CHumanBoss::stateItem() {
    if (isMotionEnd()) {
        m_pCardRevolver->setAction(0, 0);
        endCardBattle(__null, 6000);
    }
}

void CHumanBoss::beginStateDead() {
    CBossDeadDemo* demo;
    CCollBody* pHit;

    bodyCollOff(-1);
    setBossDeadEffect();
    changeMotion(selectDamageMotion_(), 0, FALSE, -1, 0, m_iDeadSeIdentifyNo);
    pHit = m_pBodyColl->getHitCollBody();
    if (pHit) {
        if (pHit->getDamageKind() == 3) {
            playBigDamageSE();
        } else {
            playSmallDamageSE();
        }
    }
    setTexAnmSeq(0, HumanBoss::DeadEyeSeqName);
    setTexAnmSeq(1, HumanBoss::DeadLipSeqName);
    demo = new (com_malloc_d(sizeof(CBossDeadDemo), __FILE__ "(483)", RET_ADDR())) CBossDeadDemo(this, 0);
    if (demo) {
        demo->setup(getHandle(), __null, -1);
    }
}

void CHumanBoss::stateDead() {
}

void CHumanBoss::endStateDead() {
}

/* Parked: 21 of 371 lines - the param pointer is carried in a register copy through the build and enemy-card loops
   (the original's `move a3,v1`; ours reloads getEnemyParam() in the enemy-card loop body). Levers that got it here:
   the id % 10000 in a local, UINT search index, one INT i per loop, `cards = deck` before ASSERT(deck) (callee-saved),
   setHumanBossFlag(0, 1), BYTE card number, and a volatile read of the first loop's iCardCount (kept pointer, reloaded
   count). Permuter 9 min: no gain. 2026-11-07: m_pEnemyParam read directly matches, but the member is private in the
   ref: not the original source, reverted; try the ref's non-const getEnemyParam() signature. */
#if 0
static inline void _setCard(CARD& c, USHORT kind, BYTE num) { c.m_nId = 0; c.m_nKind2 = kind; c.m_nNumber2 = num; c.m_nCategory20 = cardDataTbl[c.m_nKind].nCategory0; c.m_nCategory21 = cardDataTbl[c.m_nKind].nCategory1; }
void CHumanBoss::setupCardRevolver() {
    CARD_DATA* cardData;
    INT cardNum;
    CARD* deck;
    CARD* cards;

    if (!getEnemyParam()) return;
    UINT id = (UINT)getID() % 10000;
    cardData = __null;
    for (UINT n = 0; n < 12; n++) {
        if (HumanBoss::g_CardDataTbl[n].iID == id) {
            cardData = &HumanBoss::g_CardDataTbl[n];
            m_pCardData = cardData;
            break;
        }
    }
    if (!cardData) return;
    cardNum = 1;
    for (INT i = 0; i < *(volatile INT*)&getEnemyParam()->iCardCount; i++) {
        ENEMY_CARD* card = &getEnemyParam()->pCards[i];
        INT j;
        for (j = 0; j < 10; j++) {
            if (card->iCardKind != 14) cardNum += card->bNumber[j];
        }
    }
    cardNum += getEnemyParam()->iEnemyCard;
    m_iTotalCardNum = cardNum;
#line 537
    deck = new (__FILE__ "(537)") CARD[cardNum];
    cards = deck;
#line 539
    ASSERT(deck);
    for (INT i = 0; i < getEnemyParam()->iCardCount; i++) {
        ENEMY_CARD* card = &getEnemyParam()->pCards[i];
        INT cardKind;
        INT j;
        if (card->iCardKind == 14) continue;
        switch (card->iCardKind) {
        case 0:
        case 1:
        case 2:
        case 3:
        case 4:
        case 5:
        case 6:
        case 7:
            cardKind = cardData->iCardKind[card->iCardKind];
            break;
        case 8:
            cardKind = 0x2A;
            break;
        case 9:
            cardKind = 0x2B;
            break;
        case 10:
            cardKind = 0x2C;
            break;
        case 11:
            cardKind = 0x2D;
            break;
        case 12:
            cardKind = 0x2E;
            break;
        case 13:
            cardKind = 0x2F;
            break;
        }
        for (j = 0; j < 10; j++) {
            INT k;
            for (k = 0; k < card->bNumber[j]; k++) {
                if (DebugWork::g_dw.enemy.uiFlag & 0x4000) {
                    _setCard(*cards, cardKind, DebugWork::g_dw.enemy.iCardNumber);
                } else {
                    _setCard(*cards, cardKind, j);
                }
                cards++;
            }
        }
    }
    for (INT i = 0; i < getEnemyParam()->iEnemyCard; i++) {
        _setCard(*cards, getEnemyParam()->byEnemyCards[i], 0xFF);
        cards++;
    }
    _setCard(*cards, 0xFF, 0);
    for (INT i = cardNum - 2; i > 0; i--) {
        INT j = (INT)((FLOAT)(i + 1) * RND::genrand_real1());
        CARD v = deck[i];
        deck[i] = deck[j];
        deck[j] = v;
    }
#line 622
    m_pCardRevolver = new (com_malloc_d(sizeof(CardBattle::CCardRevolver), __FILE__ "(622)", RET_ADDR())) CardBattle::CCardRevolver(this, deck, 2, -1);
#line 623
    ASSERT(m_pCardRevolver);
    if (deck) delete[] deck;
    if (getEnemyParam()->iEnemyCard > 0) setHumanBossFlag(0, 1);
    m_iRemainCardNum = getAttackCardNum(3);
}
#else
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Game/Enemy/HumanBoss", setupCardRevolver__10CHumanBoss);
/* placeholder for setupCardRevolver (INCLUDE_ASM): its strings, then its jump table */
__asm__(".section .sdata\n");
__attribute__((section(".sdata"))) void _setupCardRevolver_placeholder(CHumanBoss* p, void* deck, void* rev) {
#line 537
    com_malloc_d(4, __FILE__ "(537)", (UINT)-1);
#line 539
    ASSERT(deck);
#line 622
    com_malloc_d(4, __FILE__ "(622)", (UINT)-1);
#line 623
    if (!rev) AssertApp(__FILE__, __LINE__, "m_pCardRevolver", 0);
    TRACE(cardDataTbl[0].pName); /* keeps the card table (used by the asm) */
}
asm(".rdata\n.align 4\njtbl_004C3B00:\n"
    ".word .L002CBD50\n.word .L002CBD50\n.word .L002CBD50\n.word .L002CBD50\n"
    ".word .L002CBD50\n.word .L002CBD50\n.word .L002CBD50\n.word .L002CBD50\n"
    ".word .L002CBD64\n.word .L002CBD6C\n.word .L002CBD74\n.word .L002CBD7C\n"
    ".word .L002CBD84\n.word .L002CBD8C\n.text\n");
#endif

INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Game/Enemy/HumanBoss", setupAct__10CHumanBoss);
/* placeholder for setupAct (INCLUDE_ASM): its strings */
__attribute__((section(".sdata"))) void _setupAct_placeholder(void* p, void* q, void* r) {
    if (!p) AssertApp(__FILE__, 0, "m_iStockNum <= 8", 0);
    com_malloc_d(4, __FILE__ "(674)", (UINT)-1);
    if (!q) AssertApp(__FILE__, 0, "m_pNormalActs", 0);
    com_malloc_d(4, __FILE__ "(684)", (UINT)-1);
    if (!r) AssertApp(__FILE__, 0, "m_pStockData", 0);
}
/* Parked: 18 of 205 lines (2026-11-07): the normal arm's -1 (sMin/sMax) is its own hoisted register in the original (li s4,-1 after li s5,1), ours shares the compares' -1 (s2): loop.c (-dL) combines the arm's HImode -1 movable with the SImode compare constant ("matches 84"); the rest follows (14/12 store order, the assert's __FILE__ lui hoisted too in ours, a loop-align nop); tried: chain orders, separate stores, read-back, (SHORT)/0xFFFF/-1L, INT/SHORT/long locals, m_pEnemyParam direct (all 18; private in the ref, so not used), duplicated normal arm per branch (18, the arms' -1 become subregs), inverted/nested/else-if condition shapes (18-88); permuter 15 min: no gain */
#if 0
void CHumanBoss::setupAct() {
    ACT_DATA tmp[8];
    ACT_DATA tmp2[8];
    ENEMY_ACT* act;
    INT i;

    if (!getEnemyParam()) {
        return;
    }
    act = getEnemyParam()->pActs;
    for (i = 0; i < getEnemyParam()->iActCount; i++, act++) {
        if (act->iCard[0] == -1 && act->iCard[1] == -1 && act->iCard[2] == -1) {
            continue;
        }
        if (act->iCard[0] == -1 || act->iCard[1] == -1 || act->iCard[2] == -1) {
            tmp[m_iNormalActNum].iState = act->iActNo + 2;
            tmp[m_iNormalActNum].pAct = act;
            tmp[m_iNormalActNum].bEnable = TRUE;
            tmp[m_iNormalActNum].sMinNumber = tmp[m_iNormalActNum].sMaxNumber = -1;
            m_iNormalActNum++;
        } else {
            tmp2[m_iStockNum].iState = act->iActNo + 2;
            tmp2[m_iStockNum].pAct = act;
            tmp2[m_iStockNum].bEnable = TRUE;
            tmp2[m_iStockNum].sMaxNumber = tmp2[m_iStockNum].sMinNumber = -1;
            m_iStockNum++;
#line 669
            ASSERT(m_iStockNum <= 8);
        }
    }
    if (m_iNormalActNum > 0) {
#line 674
        m_pNormalActs = new (__FILE__ "(674)") ACT_DATA[m_iNormalActNum];
        ASSERT(m_pNormalActs);
        {
            INT i;
            for (i = 0; i < m_iNormalActNum; i++) {
                m_pNormalActs[i] = tmp[i];
            }
        }
    }
    if (m_iStockNum > 0) {
#line 684
        m_pStockData = new (__FILE__ "(684)") ACT_DATA[m_iStockNum];
        ASSERT(m_pStockData);
        {
            INT i;
            for (i = 0; i < m_iStockNum; i++) {
                m_pStockData[i] = tmp2[i];
            }
        }
        m_pUseStock = &m_pStockData[RND::genrand_int31() % m_iStockNum];
    }
}
#endif

void CHumanBoss::aiCardBattle() {
    CARD* activeCard;

    if (!getEnemyParam()) {
        return;
    }
    if (isReturnStock()) {
        if (!returnStockAct(0)) {
            return;
        }
        setHumanBossFlag(0, 0x40);
    }
    if (isStopCardBattle()) {
        return;
    }
#line 762
    ASSERT(m_pCardData);
    setBattleEnemyFlag(0, 0x82);
    if (m_pCardRevolver->isEnableCharge()) {
        if (!isStartRollDeck()) {
            setHumanBossFlag(0x10, 0);
        } else {
            setHumanBossFlag(0xa0, 0);
        }
    }
    if (reloadCardProc()) {
        return;
    }
    if (RND::genrand_int31() % getAttackRate()) {
        return;
    }
    activeCard = m_pCardRevolver->getActiveCardData();
    if (!activeCard) {
        return;
    }
    if (enemyCardProc(activeCard)) {
        return;
    }
    if (itemCardProc(activeCard)) {
        return;
    }
    if (!isEndRollDeck()) {
        if (stockActProc(activeCard)) {
            return;
        }
    }
    normalActProc(activeCard);
}

INT CHumanBoss::getDuelAttackState() {
    ACT_DATA* tmp[8];
    INT count = 0;
    INT i;
    INT j;

    for (i = 0; i < m_iNormalActNum; i++) {
        if (!m_pNormalActs[i].bEnable) {
            continue;
        }
        for (j = 0; j < 2; j++) {
            if (m_pNormalActs[i].pAct->iCard[j] != -1) {
                tmp[count++] = &m_pNormalActs[i];
                break;
            }
        }
    }
    if (count <= 0) {
        return -1;
    }
    return tmp[RND::genrand_int31() % count]->iState;
}

void CHumanBoss::selectStockAct() {
    ACT_DATA* stock[16];
    INT stockCount = 0;
    INT count = 0;
    INT i;

    for (i = 0; i < m_iStockNum; i++) {
        if (m_pStockData[i].bEnable) {
            stockCount++;
            if (m_pUseStock != &m_pStockData[i]) {
                stock[count++] = &m_pStockData[i];
            }
        }
    }
    if (count > 0) {
        m_pUseStock = stock[RND::genrand_int31() % count];
    } else if (stockCount <= 0) {
        m_pUseStock = __null;
    }
}

BOOL CHumanBoss::testEdge() {
    FVECTOR v;
    FLOAT roty;
    FVECTOR s;
    FVECTOR e;

    roty = Math3D::LimitPI(getRot().y + 3.1415927f);
    v.x = sinf(roty);
    v.z = cosf(roty);
    v.y = 0.0f;
    s = getPos();
    s.y += 1.0f;
    AddVector(e, s, v);
    CPointCollision coll;
    coll.setStartPos(s);
    coll.setEndPos(e);
    if (coll.collideMap() && coll.m_outNormal.y < 0.64278764f) {
        return TRUE;
    }
    return FALSE;
}

CHumanBoss::ACT_DATA* CHumanBoss::getStockAct(INT inState) const {
    INT i;
    for (i = 0; i < m_iStockNum; i++) {
        if (m_pStockData[i].iState == inState) {
            return &m_pStockData[i];
        }
    }
    return __null;
}

void CHumanBoss::enableStockAct(INT inState, BOOL inSw) {
    INT i;
    for (i = 0; i < m_iStockNum; i++) {
        if (m_pStockData[i].iState == inState) {
            m_pStockData[i].bEnable = inSw;
        }
    }
}

void CHumanBoss::enableNormalAct(INT inState, BOOL inSw) {
    INT i;
    for (i = 0; i < m_iNormalActNum; i++) {
        if (m_pNormalActs[i].iState == inState) {
            m_pNormalActs[i].bEnable = inSw;
        }
    }
}

void CHumanBoss::setStockActMaxMinNumber(INT inState, SHORT inMax, SHORT inMin) {
    INT i;
    for (i = 0; i < m_iStockNum; i++) {
        if (m_pStockData[i].iState == inState) {
            m_pStockData[i].sMaxNumber = inMax;
            m_pStockData[i].sMinNumber = inMin;
        }
    }
}

void CHumanBoss::bossCardRevolverDestroy() {
#line 985
    ASSERT(m_pCardRevolver);
    m_pCardRevolver->destroy();
}

BOOL CHumanBoss::returnStockAct(INT inFlag) {
#line 993
    ASSERT(m_pCardRevolver);
    if (m_pCardRevolver->getStock(0) > 0 && !m_pCardRevolver->returnStockCard(inFlag)) {
        setHumanBossFlag(0x40, 0);
        TRACE("CHumanBoss::\x83X\x83g\x83" "b\x83N\x8bZ\x89\xf0\x8f\x9c\x8e\xb8\x94s\n");
        return FALSE;
    }
    TRACE("CHumanBoss::\x83X\x83g\x83" "b\x83N\x8bZ\x89\xf0\x8f\x9c\x90\xac\x8c\xf7\n");
    clearStockTotalNumber();
    return TRUE;
}

BOOL CHumanBoss::reloadCardProc() {
    INT num;

    if (!m_pCardRevolver->isEnableCharge()) {
        return FALSE;
    }
    if (m_pCardRevolver->getHave(1, 0, 1, 0, 0) + m_pCardRevolver->getHave(2, 0, 1, 0, 0) <= 0) {
        if (m_pCardRevolver->getStock(1) >= 3) {
            if (setPlayStock(m_pUseStock->iState)) {
                return TRUE;
            }
        } else if (m_pCardRevolver->getStock(1) > 0) {
            returnStockAct(0);
            return TRUE;
        }
    }
    if (!isEndRollDeck()) {
        INT remain = getRemainCard(3);
        if (remain * 100 / m_iRemainCardNum >= 26 || (remain > 0 && RND::genrand_int31() % 20 == 0)) {
            m_pCardRevolver->rollDeck(1, 0, 300);
            return TRUE;
        }
    }
    num = m_pCardRevolver->chargeReloadCard(0);
    if (num == 1) {
        playReloadCompleteVoice();
        setHumanBossFlag(0, 0xb0);
        m_iRemainCardNum = getHaveCard(3);
        if (m_iRemainCardNum <= 0) {
            m_iRemainCardNum = num;
        }
    }
    setBattleEnemyFlag(0x80, 0);
    return TRUE;
}

/* placeholder for enemyCardProc (INCLUDE_ASM): its strings */
__attribute__((section(".sdata"))) void _enemyCardProc_placeholder(void* cb) {
    TRACE("----------- change normal card mode.\n");
    if (!cb) AssertApp(__FILE__, 0, "cb", 0);
    TRACE("----------- change enemy card mode.\n");
}
BOOL CHumanBoss::enemyCardProc(const CARD* inActiveCard) {
    INT n;
    CardBattle::CCardBattle* cb;

    INT mode = m_pCardRevolver->getMode();
    if (mode == 1) {
        if (!canEnemyCard(inActiveCard)) {
            m_pCardRevolver->rollDeck(1, 0, 300);
            return FALSE;
        }
        n = m_pCardRevolver->getRemain(-1, 0, 0);
        if (n < 2 || (RND::genrand_int31() & 1) == 0) {
            if (n <= 0) {
                setHumanBossFlag(1, 0);
                m_pCardRevolver->changeMode(-1, 0);
            } else if (m_pCardRevolver->setPlayCard(0, 0)) {
                playUseEnemyCardVoice();
                if (n == mode) {
                    setHumanBossFlag(1, 0);
                }
            }
            TRACE("----------- change normal card mode.\n");
            return TRUE;
        }
        m_pCardRevolver->rollDeck(1, 0, 300);
        return TRUE;
    }
    if (isNoEnemyCard()) {
        return FALSE;
    }
    cb = (CardBattle::CCardBattle*)CTaskMan::getSysTask(0x16);
#line 1116
    ASSERT(cb);
    if (cb->getEnchantKind(1)) {
        return FALSE;
    }
    if (!canEnemyCard()) {
        return FALSE;
    }
    if (RND::genrand_int31() % 50) {
        return FALSE;
    }
    m_pCardRevolver->changeMode(-1, 0);
    TRACE("----------- change enemy card mode.\n");
    return TRUE;
}

BOOL CHumanBoss::itemCardProc(const CARD* inActiveCard) {
    INT noReloadNum;
    INT cardNum;
    INT haveCardNum;
    INT remainCard;
    INT rate;
    INT kind[16];
    INT count;
    INT i;
    ENEMY_CARD* card;
    INT cardKind;
    BOOL find;

    if ((DebugWork::g_dw.enemy.uiFlag & 8) && DebugWork::g_dw.enemy.sActNo + 2 == 10) {
        count = 0;
        for (i = 0; i < getEnemyParam()->iCardCount; i++) {
            card = &getEnemyParam()->pCards[i];
            cardKind = 0;
            switch (card->iCardKind) {
            case 8:
                cardKind = 0x2a;
                break;
            case 9:
                cardKind = 0x2b;
                break;
            case 10:
                cardKind = 0x2c;
                break;
            case 11:
                cardKind = 0x2d;
                break;
            case 12:
                cardKind = 0x2e;
                break;
            case 13:
                cardKind = 0x2f;
                break;
            }
            find = FALSE;
            {
                INT i;
                for (i = 0; i < count; i++) {
                    if (kind[i] == cardKind) {
                        find = TRUE;
                        break;
                    }
                }
            }
            if (!find) {
                kind[count++] = cardKind;
            }
        }
        if (count > 0) {
            m_iUseItem = kind[RND::genrand_int31() % count];
            setPlayCard(10);
            return TRUE;
        }
    }
    noReloadNum = -1;
    cardNum = 0;
    haveCardNum = 0;
    rate = getExtraParam(HumanBoss::UseItemCardParamName);
    remainCard = 0;
    switch (inActiveCard->m_nKind) {
    case 0x2a:
    case 0x2b:
    case 0x2c:
        cardNum = getAttackCardNum(1);
        haveCardNum = m_pCardRevolver->getHave(1, 1, 1, 0, 0);
        remainCard = getRemainCard(1);
        noReloadNum = cardNum * rate / 100;
        break;
    case 0x2d:
    case 0x2e:
        cardNum = getAttackCardNum(2);
        haveCardNum = m_pCardRevolver->getHave(2, 1, 1, 0, 0);
        remainCard = getRemainCard(2);
        noReloadNum = cardNum * rate / 100;
        break;
    case 0x2f:
        cardNum = getAttackCardNum(3);
        haveCardNum = m_pCardRevolver->getHave(1, 1, 1, 0, 0) + m_pCardRevolver->getHave(2, 1, 1, 0, 0);
        remainCard = getRemainCard(3);
        noReloadNum = cardNum * rate / 100;
        break;
    }
    if (noReloadNum < 0) {
        return FALSE;
    }
    if (isEndRollDeck()) {
    use:
        __asm__ volatile(""); /* MATCH: reorg cannot take the move a0,s1 for the branches into here */
        if (setPlayCard(10)) {
            m_iUseItem = inActiveCard->m_nKind;
        }
        return TRUE;
    }
    if (noReloadNum > 0 && cardNum - haveCardNum < noReloadNum) {
        __asm__ volatile("" : : : "$2"); /* MATCH: v0 is dead here: reorg fills the test's slot from the li v0,100 behind it */
    roll:
        m_pCardRevolver->rollDeck(1, 0, 300);
        __asm__ volatile(""); /* MATCH: keeps this tail: jump2 would cross-jump it into the last one */
        return TRUE;
    }
    if (remainCard * 100 / cardNum >= 26) goto roll;
    remainCard >>= 1;
    if (remainCard <= 0) {
        remainCard = 1;
    }
    if (RND::genrand_int31() % remainCard == 0) {
        goto use;
    }
    {
        register INT nCount asm("$5") = 1; /* MATCH: li a1 before the member load, for the mfhi test's slot */
        __asm__ volatile("" : : "r"(nCount));
        m_pCardRevolver->rollDeck(nCount, 0, 300);
    }
    return TRUE;
}

/* placeholder for stockActProc (INCLUDE_ASM): its strings */
__attribute__((section(".sdata"))) void _stockActProc_placeholder(INT n) {
    TRACE("CHumanBoss::\x83X\x83g\x83" "b\x83N\x82\xc5\x82\xab\x82\xc8\x82\xa2\n");
    TRACE("----------- stock boss card = 0x%08x\n", n);
}
BOOL CHumanBoss::stockActProc(const CARD* inActiveCard) {
    INT attackCardNum;
    INT magicCardNum;
    INT stockCount;
    ACT_DATA* stockData[16];
    INT count;
    INT i;
    INT state;
    INT cardKind;

    if (!m_pUseStock) {
        return FALSE;
    }
    attackCardNum = getAttackCardNum(1);
    magicCardNum = getAttackCardNum(2);
    if (attackCardNum > 0 && magicCardNum > 0) {
        if (m_pCardRevolver->getHave(1, 1, 1, 0, 0) < 3 && m_pCardRevolver->getHave(2, 1, 1, 0, 0) < 3) {
            return FALSE;
        }
    } else if (attackCardNum > 0) {
        if (m_pCardRevolver->getHave(1, 1, 1, 0, 0) < 3) {
            return FALSE;
        }
    } else if (magicCardNum > 0) {
        if (m_pCardRevolver->getHave(2, 1, 1, 0, 0) < 3) {
            return FALSE;
        }
    }
    stockCount = m_pCardRevolver->getStock(0);
    if (stockCount < 3 && !canStockAct(m_pUseStock)) {
        TRACE("CHumanBoss::\x83X\x83g\x83" "b\x83N\x82\xc5\x82\xab\x82\xc8\x82\xa2\n");
        count = 0;
        for (i = 0; i < m_iStockNum; i++) {
            if (m_pStockData[i].bEnable && m_pUseStock != &m_pStockData[i] && canStockAct(&m_pStockData[i])) {
                stockData[count++] = &m_pStockData[i];
            }
        }
        if (count > 0) {
            m_pUseStock = stockData[RND::genrand_int31() % count];
        }
        if (stockCount > 0) {
            returnStockAct(0);
            return TRUE;
        }
        return FALSE;
    }
    if (m_pCardRevolver->getStock(1) >= 3) {
        if (RND::genrand_int31() % m_iUseStockRate) {
            return FALSE;
        }
        if (!canAttack_(m_pUseStock->pAct)) {
            return FALSE;
        }
        if (DebugWork::g_dw.enemy.uiFlag & 8) {
            state = DebugWork::g_dw.enemy.sActNo + 2;
            {
                INT i;
                for (i = 0; i < m_iNormalActNum; i++) {
                    if (m_pNormalActs[i].iState == state) {
                        return setPlayStock(state) != 0;
                    }
                }
            }
            {
                INT i;
                for (i = 0; i < m_iStockNum; i++) {
                    if (m_pStockData[i].iState == state) {
                        return setPlayStock(state) != 0;
                    }
                }
            }
            return TRUE;
        }
        if (setPlayStock(m_pUseStock->iState)) {
            return TRUE;
        }
    } else {
        if (RND::genrand_int31() % m_iStockRate) {
            return FALSE;
        }
        if (stockCount >= 3) {
            return FALSE;
        }
        cardKind = m_pCardData->iCardKind[m_pUseStock->pAct->iCard[stockCount]];
        if (cardKind == inActiveCard->m_nKind) {
            if (inActiveCard->m_nNumber <= 0) {
                if (!isPerMissionStockZero() && m_pCardRevolver->getRemain(0, cardKind, 0) >= 2) {
                    if ((RND::genrand_int31() & 1) == 0) {
                        m_pCardRevolver->rollDeck(1, 0, 300);
                        return TRUE;
                    }
                    return FALSE;
                }
                setHumanBossFlag(0, 0x80);
            }
            if (setStockCard(inActiveCard)) {
                TRACE("----------- stock boss card = 0x%08x\n", inActiveCard->m_nKind);
                return TRUE;
            }
            return FALSE;
        }
        if ((RND::genrand_int31() & 1) == 0) {
            m_pCardRevolver->rollDeck(1, 0, 300);
            return TRUE;
        }
        return FALSE;
    }
    return FALSE;
}

BOOL CHumanBoss::normalActProc(const CARD* inActiveCard) {
    ACT_DATA* tmp[8];
    INT count = 0;
    ACT_DATA* act;
    INT i;
    INT j;
    INT card;
    register INT nNone asm("$22"); /* MATCH: the -1 in s6, &tmp[count] in s5 */

    for (i = 0; i < m_iNormalActNum; i++) {
        if (!m_pNormalActs[i].bEnable) {
            continue;
        }
        for (j = 0, nNone = -1; j < 2; j++) {
            card = m_pNormalActs[i].pAct->iCard[j];
            if (card == nNone) {
                continue;
            }
            if (m_pNormalActs[i].pAct->iCard[0] != nNone && m_pNormalActs[i].pAct->iCard[1] != nNone) {
                if (RND::genrand_int31() & 1) {
                    continue;
                }
            }
            if (((DebugWork::g_dw.enemy.uiFlag & 8) && canAttack_(m_pNormalActs[i].pAct)) ||
                (m_pCardData->iCardKind[card] == inActiveCard->m_nKind && canAttack_(m_pNormalActs[i].pAct))) {
                tmp[count++] = &m_pNormalActs[i];
                break;
            }
        }
    }
    if (count <= 0) {
        goto roll;
    }
    act = tmp[RND::genrand_int31() % count];
    if (getPlayerState() == 11) {
        if (CardBattle::CCardBattle::getPlayingCardPoint(1) > inActiveCard->m_nNumber && inActiveCard->m_nNumber != 0) {
            goto roll;
        }
        setDoBattleAct(act);
        useCard(act->iState);
        goto ret;
    roll:
        m_pCardRevolver->rollDeck(1, 0, 300);
        return FALSE;
    }
    setDoBattleAct(act);
    useCard(act->iState);
ret:
    return TRUE;
}

void CHumanBoss::setDoBattleAct(const ACT_DATA* inAct) {
#line 1634
    ASSERT(inAct);
    if (m_pDoBattleAct == inAct) {
        m_iActCount++;
    } else if (--m_iActCount < 0) {
        m_iActCount = 0;
    }
    m_pDoBattleAct = (ACT_DATA*)inAct;
}

void CHumanBoss::useCard(INT inState) {
    INT state;

    if (DebugWork::g_dw.enemy.uiFlag & 8) {
        state = DebugWork::g_dw.enemy.sActNo + 2;
        {
            INT i;
            for (i = 0; i < m_iNormalActNum; i++) {
                if (m_pNormalActs[i].iState == state) {
                    setPlayCard(state);
                    return;
                }
            }
        }
        {
            INT i;
            for (i = 0; i < m_iStockNum; i++) {
                if (m_pStockData[i].iState == state) {
                    setPlayCard(state);
                    return;
                }
            }
        }
        return;
    }
    setPlayCard(inState);
}

void CHumanBoss::playMyRemainHpVoice() {
    INT myHp;
    INT voiceID;
    INT id;

    if (isMyHpRemainVoice()) {
        return;
    }
    myHp = getExtraParam(HumanBoss::MyHpRemainParamName);
    if (getHp() * 100 / getHpMax() <= myHp) {
        voiceID = getExtraParam(HumanBoss::VoiceIDParamName);
        id = getExtraParam(HumanBoss::MyHpVoiceParamName);
        if (voiceID && id) {
            playVoice(voiceID, id, 1);
        }
        setHumanBossFlag(8, 0);
    }
}

void CHumanBoss::playPlayerRemainHpVoice() {
    INT playerHp;
    INT voiceID;
    INT id;

    if (isPlayerHpRemainVoice()) {
        return;
    }
    playerHp = getExtraParam(HumanBoss::PlHpRemainParamName);
    if (HumanBoss::GetPlayerHpPercent() <= playerHp) {
        voiceID = getExtraParam(HumanBoss::VoiceIDParamName);
        id = getExtraParam(HumanBoss::PlHpVoiceParamName);
        if (voiceID && id) {
            playVoice(voiceID, id, 1);
        }
        setHumanBossFlag(4, 0);
    }
}

void CHumanBoss::playReloadCompleteVoice() {
    INT voiceID = getExtraParam(HumanBoss::VoiceIDParamName);
    INT id = getExtraParam(HumanBoss::ReloadVoiceParamName);
    if (voiceID && id) {
        playVoice(voiceID, id, 1);
    }
}

void CHumanBoss::playUseEnemyCardVoice() {
    INT voiceID = getExtraParam(HumanBoss::VoiceIDParamName);
    INT id = getExtraParam(HumanBoss::ECardVoiceParamName);
    if (voiceID && id) {
        playVoice(voiceID, id, 1);
    }
}

BOOL CHumanBoss::canStockAct(const ACT_DATA* inAct) {
    INT i;

#line 1755
    ASSERT(__null != inAct);
    for (i = 0; i < 3; i++) {
        if (m_pCardRevolver->getRemain(0, m_pCardData->iCardKind[inAct->pAct->iCard[i]], 0) <= 0) {
            return FALSE;
        }
    }
    return TRUE;
}

/* MATCH: the roll >= 5 test is the else-if of the reset test, one shared return FALSE after both (separate returns
   cross-jump or change reorg's delay-slot fills) */
BOOL CHumanBoss::setStockCard(const CARD* inActiveCard) {
    SHORT totalNumber = m_iStockTotalNumber + inActiveCard->m_nNumber;

    if (m_pCardRevolver->getStock(0) == 2 && m_pUseStock->sMinNumber > 0) {
        if (totalNumber < m_pUseStock->sMinNumber || m_pUseStock->sMaxNumber < totalNumber) {
            if (m_iStockTotalNumber >= m_pUseStock->sMaxNumber || m_pUseStock->sMinNumber - m_iStockTotalNumber >= 10 ||
                ++m_iStockRollCount >= 10) {
                clearStockTotalNumber();
                returnStockAct(0);
                selectStockAct();
            } else if (m_iStockRollCount >= 5) {
                setHumanBossFlag(0x80, 0);
            }
            return FALSE;
        }
        TRACE_USER((USER)5, "getStock() = %d, TotalNumber = %d, CurrentNumber = %d, min = %d, max = %d\n",
                   m_pCardRevolver->getStock(0), totalNumber, m_iStockTotalNumber, m_pUseStock->sMinNumber,
                   m_pUseStock->sMaxNumber);
    }
    m_iStockTotalNumber = totalNumber;
    m_pCardRevolver->setStockCard();
    return TRUE;
}

BOOL CHumanBoss::setPlayCard(INT inState) {
    if (!m_pCardRevolver->setPlayCard(0, 0)) {
        return FALSE;
    }
    setAttackState_(inState);
    setBattleEnemyFlag(0x201, 0);
    setHumanBossFlag(0, 0xb0);
    return TRUE;
}

BOOL CHumanBoss::setPlayStock(INT inState) {
    if (!m_pCardRevolver->setPlayStock(0, 0)) {
        return FALSE;
    }
    setBattleEnemyFlag(0x201, 0);
    setAttackState_(inState);
    m_pDoBattleAct = m_pUseStock;
    clearStockTotalNumber();
    selectStockAct();
    return TRUE;
}
// SOURCE DATA: .data .rodata
