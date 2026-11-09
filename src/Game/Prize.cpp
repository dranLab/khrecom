#define COLLUTIL_H_PATH "c:/KHCOM/MainApp/Game/Collision/CollUtil.h" /* MATCH: this unit names CollUtil.h so (header default "Game/Collision/CollUtil.h") */
#define GPRIM_H_PATH "System/Graphic/GPrim.h" /* MATCH: this unit names Gprim.h so (header default "System/Graphic/Gprim.h") */
#define CARD_H_PATH "c:/KHCOM/MainApp/Game/Card.h" /* MATCH: this unit names Card.h so (header default "Game/Card.h") */
#define LINKEDBUF_H_PATH "System/LinkedBuf.h" /* MATCH: this unit names LinkedBuf.h by the relative path (LinkedBuf.h switches __FILE__ only when it is defined) */
#include "System/TaskMan.h"
#include "System/Task.h"
#include "Game/Collision/CollUtil.h"
#include "System/Graphic/Effect/effect.h"
#include "System/Graphic/Anim.h"
#include "System/Graphic/Gprim.h"
#include "System/Graphic/2d/2DMan.h"
#include "Game/CardBattle.h"
#include "System/Graphic/Str.h"

/* MATCH: Placeholder: an empty string follows Str.h's strings in .rodata (as in GimmickWallHole); which header it comes
 * from is not known. */
inline void _emptyStringPlaceholder() {
    TRACE("");
}

#include "System/Graphic/modeldata.h"
#include "System/Graphic/Model.h"
#include "Game/Card.h"
#include "System/HeapMan.h"
#include "Game/Prize.h"
#include "System/Graphic/LightUnit.h"
#include "System/LinkedBuf.h"
#include "Game/Movement.h"
#include "Game/Collision/CollMap.h"
#include "Game/GameWork.h"
#include "Game/SceneInfo.h"
#include "Game/Player/Player.h"
#include "System/Graphic/Effect/EffectMan.h"
#include "System/Graphic/Camera.h"
#include "System/Math3D.h"
#include "System/Sound/SoundMan.h"
#include "System/Rnd.h"
#include <string.h>
#include "include_asm.h"



#define ASSERT(e) if (!(e)) AssertApp(__FILE__, __LINE__, #e, 0)

typedef int MODELTAG[4];

#include "System/RsrcDB.h"





extern "C" {
float sinf(float x);
float cosf(float x);
void sceVu0RotMatrixX(FMATRIX& m0, const FMATRIX& m1, float rx);
void sceVu0RotMatrixY(FMATRIX& m0, const FMATRIX& m1, float ry);
void sceVu0RotMatrixZ(FMATRIX& m0, const FMATRIX& m1, float rz);
}
FVECTOR getPlayerPos(BOOL flag);
void ScaleVector(FVECTOR& v0, const FVECTOR& v1, FLOAT f);
void ScaleVectorXYZ(FVECTOR& v0, const FVECTOR& v1, FLOAT f);
void SubVector(FVECTOR& v0, const FVECTOR& v1, const FVECTOR& v2);
void SubVectorXYZ(FVECTOR& v0, const FVECTOR& v1, const FVECTOR& v2);
void AddVectorXYZ(FVECTOR& v0, const FVECTOR& v1, const FVECTOR& v2);
void Normalize(FVECTOR& v0, const FVECTOR& v1);
void ApplyMatrix(FVECTOR& v0, const FMATRIX& m0, const FVECTOR& v1);
void ApplyMatrix33(FVECTOR& v0, const FMATRIX& m0, const FVECTOR& v1);

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

struct PRIZE_DATA {
    CHAR* pEffectName;
    INT iEffectNo;
    FLOAT fRadius;
};

namespace Prize {
static const PRIZE_DATA g_PrizeDataTbl[13] = {
    { "rec_prise", 8, 0.25f },  { "rec_prise", 7, 0.25f },  { "rec_prise", 0, 0.35f },  { "rec_prise", 10, 0.35f },
    { "rec_prise", 9, 0.35f },  { "rec_prise", 1, 0.425f }, { "rec_prise", 2, 0.25f },  { "rec_prise", 11, 0.25f },
    { "rec_prise", 3, 0.3f },   { "rec_prise", 12, 0.25f }, { "rec_prise", 4, 0.3f },   { "rec_prise", 5, 0.25f },
    { "rec_prise", 6, 0.25f },
};
static const int g_iCardModelIndex[14] = { 0, 0, 1, 2, 3, 4, 4, 5, 0, 6, 7, 8, 9, 10 };
static const G_ST g_CardST[4] = { { 0.f, 0.f }, { 0.25f, 0.f }, { 0.f, 0.25f }, { 0.25f, 0.25f } };
} // namespace Prize
/* the original's (local) table is referenced by another object's data, as splat symbolized it */
__asm__(".globl _5Prize$g_PrizeDataTbl");

static inline void _setNewState(CState*& p, CState& NewState) { p = &NewState; }

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

CPrize::CPrize(CTask* inParent)
    : CEntity(inParent, (ObjType::eObjType)6), m_hEffectHandle(0), m_bHoming(FALSE), m_bSECall(FALSE),
      m_bReturnCard(FALSE), m_bFlyHigh(FALSE), m_iBoundCounter(0), m_fPrizeRate(0.075f), m_pCardModel(NULL) {}

CPrize::~CPrize() {
    CEffect* effect = CEffectMan::getEffect(m_hEffectHandle);
    if (effect) effect->kill();
    if (m_pCardModel) {
        if (m_pCardModel->m_pData) DelayedDelete((MODELTAG*)m_pCardModel->m_pData, TRUE);
        else if (m_pCardModel->m_pSemiData) DelayedDelete((MODELTAG*)m_pCardModel->m_pSemiData, TRUE);
        DelayedDelete(m_pCardModel, FALSE);
    }
}

INT CPrize::getTypeID() const { return 0x1008; }

BOOL CPrize::setup(const PRIZE_PARAM& inParam) {
    m_PrizeKind = inParam.PrizeKind;
    m_iNumber = inParam.iNumber;
    m_fSpeed = inParam.fSpeed;
#line 157
    m_pMove = new (__FILE__ "(157)") CMovement(this);
    if (!m_pMove) return FALSE;
    addCoord(1);
    BOOL ret;
    if (inParam.PrizeKind >= 0xCB) {
        ret = initPrize_();
    } else {
        ret = initCard_();
    }
    if (!ret) return FALSE;
    if (inParam.uiPrizeFlag & 1) {
        warp(inParam.vPos);
    } else if (CSceneInfo::isFieldScene()) {
        CCollMap* coll = m_pMove->getMapCollision();
#line 199
        ASSERT(coll);
        FLOAT radius = coll->getRadius();
        coll->setRadius(0.0f);
        FVECTOR start = getPlayerPos(FALSE);
        FVECTOR end = inParam.vPos;
        FVECTOR pos = inParam.vPos;
        FVECTOR normal;
        start.y = end.y;
        coll->setStartPos(start);
        coll->setEndPos(end);
        if (coll->collideRide(FALSE)) {
            pos = *coll->getOutPos();
            coll->setEndPos(pos);
        }
        if (coll->collideMap()) pos = *coll->getOutPos();
        setOldPos(start);
        setPos(pos);
        FLOAT r = radius + 0.1f;
        m_pMove->setFlag(0x40, 0);
        m_pMove->setFootRadius(r * 0.5f);
        m_pMove->setWallRadius(r);
        if (inParam.PrizeKind >= 0xCB) {
            m_pMove->setShiftY(0.0f);
        } else {
            m_pMove->setShiftY(-0.2f);
        }
        m_pMove->update();
        m_pMove->setFlag(0, 0x40);
        m_pMove->setCollisionFlag(0, 0xF);
        m_pMove->setRadius(radius);
        if (inParam.PrizeKind >= 0xCB) {
            m_pMove->setShiftY(0.0f);
        } else {
            m_pMove->setShiftY(-0.2f);
        }
        warp(getPos());
        setOldPos(start);
        setPos(getPos());
        TRACE("\x8f" "C\x90\xb3\x91O = (%.5f, %.5f, %.5f), \x8f" "C\x90\xb3\x8c\xe3 = (%.5f, %.5f, %.5f)\n", inParam.vPos.x,
              inParam.vPos.y, inParam.vPos.z, getPos().x, getPos().y, getPos().z);
    } else {
        findPosition_(inParam);
    }
    setStartPos(inParam.vPos);
    setRot(inParam.vRot);
    updateCoord();
    coordFix();
    m_pMove->enableGravity(TRUE);
    return TRUE;
}

void CPrize::onOut() {
    if (m_pModels && (isFriendCard_() || isGimmickCard_())) {
        CGameWork::setFlag(1, 0x22, FALSE);
    }
}

void CPrize::move(const FVECTOR& inPos) {
    CCollMap* coll = m_pMove->getMapCollision();
#line 402
    ASSERT(coll);
    FVECTOR start = getPlayerPos(TRUE);
    FVECTOR end = inPos;
    coll->setStartPos(start);
    coll->setEndPos(end);
    if (coll->collideRide(FALSE)) end = *coll->getOutPos();
    coll->setEndPos(end);
    if (coll->collideMap()) end = *coll->getOutPos();
    warp(end);
}

void CPrize::enableMarluxia3Mode(BOOL inSw) {
    if (inSw) {
        for (INT i = 0; i < m_iModelCount; i++) {
            m_pModels[i].m_nFlag &= ~0x20;
            m_pModels[i].m_nFlag |= 0x1000;
        }
        m_State[1].set(this, &CPrize::beginCardMainSpecial_, &CPrize::cardMainSpecial_, &CPrize::endCardMainSpecial_);
        gotoState(m_State[1]);
    }
}

void CPrize::forceGetPrize() {
    playerPrizeGet_();
    out();
}

void CPrize::flyHigh(BOOL bAll) {
    if (m_PrizeKind >= 0xCB) return;
    if (m_bFlyHigh) return;
    FVECTOR rot;
    rot.set((-45.0f - getRandF() * 15.0f) * 0.017453292f, getRandF() * 360.0f * 0.017453292f, 0.0f, 1.0f);
    Math3D::LimitPIVector(rot);
    FVECTOR v;
    v.set(0.0f, 0.0f, 1.0f, 1.0f);
    FMATRIX m;
    _unitMatrixVu0(m);
    sceVu0RotMatrixX(m, m, rot.x);
    sceVu0RotMatrixY(m, m, rot.y);
    sceVu0RotMatrixZ(m, m, rot.z);
    ApplyMatrix33(v, m, v);
    if (m_PrizeKind != 0xA0 || bAll) {
        setupFade(1.0f, 0.0f, 3000, -1);
        ScaleVectorXYZ(v, v, 0.3f);
        m_pMove->setVel(v);
        m_iStep = 2;
        m_bFlyHigh = TRUE;
    } else {
        ScaleVectorXYZ(v, v, 0.3f);
        m_pMove->setVel(v);
    }
}

void CPrize::beginPrizeMain_() {
    setTimer(4500);
    m_pMove->calcVelocity(m_fSpeed);
    if (!CGameWork::getFlag(1, 0x21)) {
        m_iStep = 0;
    } else {
        m_iStep = 3;
    }
}

void CPrize::prizeMain_() {
    switch (m_iStep) {
    case 0:
        if (m_bHoming) {
            if (getTimer() <= 0) {
                m_iStep = 2;
                setTimer(1500);
                break;
            }
        } else if (getTimer() <= 0) {
            m_bHoming = TRUE;
            setTimer(30000);
        }
        if (m_pMove->isGround()) {
            FVECTOR v;
            v.set(0.0f, 1.0f, 0.0f, 1.0f);
            ScaleVector(v, v, m_pMove->getSpeed() * 0.7f);
            m_pMove->setVel(v);
            if (m_pMove->getSpeed() <= 0.05f) clearSpeed();
            m_iBoundCounter++;
            if (m_iBoundCounter < 3) playBoundSE_();
        } else if (m_pMove->isWall()) {
            FVECTOR v = m_pMove->getVel();
            FVECTOR n = m_pMove->getDetectNormal();
            Normalize(n, n);
            Math3D::CalcReflectionVector(v, m_pMove->getVel(), n);
            m_pMove->setVel(v);
        }
        if (isDraw_()) {
            ((FVECTOR&)getRot()).x = 0.0f;
            ((FVECTOR&)getRot()).y = g_pPlayer->calcDirection(this);
            m_pMove->enableGravity(FALSE);
            m_pMove->setFlag(1, 0);
            m_iStep = 1;
        }
        break;
    case 1:
        if (!g_pPlayer) {
            out();
            break;
        }
        {
            FVECTOR target;
            g_pPlayer->getLockOnPos(target);
            if (m_bSECall) {
                CEffect* effect = CEffectMan::getEffect(m_hEffectHandle);
                if (!effect) {
                    out();
                    break;
                }
                if (getTimer() <= 0) {
                    playerPrizeGet_();
                    out();
                    break;
                }
                FLOAT s = (FLOAT)getTimer() / 3000.0f;
                effect->scale().x = s;
                effect->scale().y = s;
                effect->scale().z = s;
                FVECTOR v;
                FMATRIX m;
                _unitMatrixVu0(m);
                sceVu0RotMatrixY(m, m, 0.5934119f);
                ApplyMatrix(m_vGetVel, m, m_vGetVel);
                ScaleVectorXYZ(v, m_vGetVel, s);
                m_pMove->setVel(v);
                warp(target);
            } else {
                m_fSpeed = 0.3f;
                m_pMove->homingMove(target, 0.25f, m_fSpeed);
            }
            if (calcDistanceSq(target) <= 4.0f && !m_bSECall) {
                FVECTOR v;
                SubVector(v, getPos(), target);
                Normalize(v, v);
                FVECTOR v1 = v;
                v1.y = 0.0f;
                if (Math3D::VectorLength(v1) < v.y * 0.25f) v.set(0.0f, 1.0f, 0.0f, 1.0f);
                ScaleVector(v, v, 0.75f);
                m_pMove->setVel(v);
                m_vGetVel = v;
                warp(target);
                setTimer(3000);
                playGetSE_();
                m_bSECall = TRUE;
            }
        }
        break;
    case 2:
        if (getTimer() > 0) {
            CEffect* effect = CEffectMan::getEffect(m_hEffectHandle);
            if (effect) effect->color().w = (FLOAT)getTimer() / 1500.0f;
            break;
        }
        out();
        break;
    case 3:
        if (m_pMove->isGround()) {
            FVECTOR v;
            v.set(0.0f, 1.0f, 0.0f, 1.0f);
            ScaleVector(v, v, m_pMove->getSpeed() * 0.7f);
            m_pMove->setVel(v);
            if (m_pMove->getSpeed() <= 0.05f) clearSpeed();
            m_iBoundCounter++;
            if (m_iBoundCounter < 3) playBoundSE_();
        } else if (m_pMove->isWall()) {
            FVECTOR v = m_pMove->getVel();
            FVECTOR n = m_pMove->getDetectNormal();
            Normalize(n, n);
            Math3D::CalcReflectionVector(v, m_pMove->getVel(), n);
            m_pMove->setVel(v);
        }
        if (!CGameWork::getFlag(1, 0x21)) {
            ((FVECTOR&)getRot()).x = 0.0f;
            ((FVECTOR&)getRot()).y = g_pPlayer->calcDirection(this);
            m_pMove->enableGravity(FALSE);
            m_iStep = 1;
        }
        break;
    }
}

void CPrize::endPrizeMain_() {}
void CPrize::beginCardMain_() {
    m_pMove->calcVelocity(m_fSpeed);
    FVECTOR rot;
    rot.set(0.0f, 0.0f, 0.5235988f, 1.0f);
    setRot(rot);
    m_iStep = 0;
    m_pMove->setMass(0.5f);
}

void CPrize::cardMain_() {
    switch (m_iStep) {
    case 0:
        if (30000 - getTimer() >= 4500) m_bHoming = TRUE;
        cardRotation_();
        if (m_pMove->isGround() || m_pMove->isWall()) {
            FVECTOR v;
            FVECTOR n = m_pMove->getDetectNormal();
            Normalize(n, n);
            FVECTOR vel = m_pMove->getVel();
            Normalize(vel, vel);
            Math3D::CalcReflectionVector(v, vel, m_pMove->getDetectNormal());
            Normalize(v, v);
            ScaleVector(v, v, 0.125f);
            m_pMove->setVel(v);
        }
        if (isDraw_()) {
            m_pMove->setFlag(1, 0);
            m_iStep = 1;
            break;
        }
        if (CGameWork::getFlag(1, 0x23)) break;
        if ((isFriendCard_() || isGimmickCard_()) && getTimer() <= 0) {
            setupFade(1.0f, 0.0f, 1500, -1);
            m_iStep = 2;
        }
        break;
    case 1:
        if (!g_pPlayer) {
            out();
            break;
        }
        {
            FVECTOR target;
            g_pPlayer->getLockOnPos(target);
            if (m_bSECall) {
                if (getTimer() <= 0) {
                    playerPrizeGet_();
                    out();
                    break;
                }
                FLOAT s = (FLOAT)getTimer() / 3000.0f;
                FVECTOR scale = getScale();
                scale.x = s;
                scale.y = s;
                scale.z = s;
                setScale(scale);
                FVECTOR v;
                FMATRIX m;
                _unitMatrixVu0(m);
                sceVu0RotMatrixY(m, m, 0.5934119f);
                ApplyMatrix(m_vGetVel, m, m_vGetVel);
                ScaleVectorXYZ(v, m_vGetVel, s);
                m_pMove->setVel(v);
                warp(target);
            } else {
                m_fSpeed = 0.3f;
                FVECTOR v;
                SubVectorXYZ(v, target, getPos());
                Normalize(v, v);
                ScaleVectorXYZ(v, v, 0.3f);
                m_pMove->setVel(v);
            }
            if (calcDistanceSq(target) <= 4.0f && !m_bSECall) {
                FVECTOR v;
                SubVector(v, getPos(), target);
                Normalize(v, v);
                FVECTOR v1 = v;
                v1.y = 0.0f;
                if (Math3D::VectorLength(v1) < v.y * 0.25f) v.set(0.0f, 1.0f, 0.0f, 1.0f);
                ScaleVector(v, v, 0.75f);
                m_pMove->setVel(v);
                m_vGetVel = v;
                warp(target);
                setTimer(3000);
                playGetSE_();
                m_bSECall = TRUE;
            }
        }
        break;
    case 2:
        cardRotation_();
        if (m_pMove->isGround() || m_pMove->isWall()) {
            FVECTOR v;
            FVECTOR n = m_pMove->getDetectNormal();
            Normalize(n, n);
            Math3D::CalcReflectionVector(v, m_pMove->getVel(), m_pMove->getDetectNormal());
            Normalize(v, v);
            ScaleVector(v, v, 0.125f);
            m_pMove->setVel(v);
            m_bHoming = TRUE;
        }
        if (!isFade()) out();
        break;
    }
}

void CPrize::endCardMain_() {}

void CPrize::beginCardMainSpecial_() {
    m_pMove->calcVelocity(m_fSpeed);
    FVECTOR rot;
    rot.set(0.0f, 0.0f, 0.5235988f, 1.0f);
    setRot(rot);
    m_iStep = 0;
    m_pMove->setMass(0.5f);
}
void CPrize::cardMainSpecial_() {
    switch (m_iStep) {
    case 0: {
        cardRotation_();
        if (m_pMove->isGround()) {
            FVECTOR v;
            v.set(0.0f, 1.0f, 0.0f, 1.0f);
            ScaleVector(v, v, m_pMove->getSpeed() * 0.5f);
            m_pMove->setVel(v);
            if (m_pMove->getSpeed() <= 0.05f) clearSpeed();
            m_bHoming = TRUE;
        } else if (m_pMove->isWall()) {
            FVECTOR v = m_pMove->getVel();
            FVECTOR n = m_pMove->getDetectNormal();
            Normalize(n, n);
            Math3D::CalcReflectionVector(v, m_pMove->getVel(), n);
            m_pMove->setVel(v);
        }
        FLOAT dist;
        if (((CardBattle::CCardBattle*)CTaskMan::getSysTask(22))->getEnchantKind() == 9) {
            dist = g_pPlayer->getCardDrawRadius();
            dist *= dist;
        } else {
            dist = 2.25f;
        }
        if (m_bHoming && calcDistanceSq(g_pPlayer) <= dist) {
            m_pMove->setFlag(1, 0);
            m_iStep = 1;
        }
        break;
    }
    case 1:
        if (!g_pPlayer) {
            out();
            break;
        }
        {
            FVECTOR target;
            g_pPlayer->getLockOnPos(target);
            if (m_bSECall) {
                if (getTimer() <= 0) {
                    playerPrizeGet_();
                    out();
                    break;
                }
                FLOAT s = (FLOAT)getTimer() / 3000.0f;
                FVECTOR scale = getScale();
                scale.x = s;
                scale.y = s;
                scale.z = s;
                setScale(scale);
                FVECTOR v;
                FMATRIX m;
                _unitMatrixVu0(m);
                sceVu0RotMatrixY(m, m, 0.5934119f);
                ApplyMatrix(m_vGetVel, m, m_vGetVel);
                ScaleVectorXYZ(v, m_vGetVel, s);
                m_pMove->setVel(v);
                warp(target);
            } else {
                m_fSpeed = 0.3f;
                FVECTOR v;
                SubVectorXYZ(v, target, getPos());
                Normalize(v, v);
                ScaleVectorXYZ(v, v, 0.3f);
                m_pMove->setVel(v);
            }
            if (calcDistanceSq(target) <= 4.0f && !m_bSECall) {
                FVECTOR v;
                SubVector(v, getPos(), target);
                Normalize(v, v);
                FVECTOR v1 = v;
                v1.y = 0.0f;
                if (Math3D::VectorLength(v1) < v.y * 0.25f) v.set(0.0f, 1.0f, 0.0f, 1.0f);
                ScaleVector(v, v, 0.75f);
                m_pMove->setVel(v);
                m_vGetVel = v;
                warp(target);
                setTimer(3000);
                playGetSE_();
                m_bSECall = TRUE;
            }
        }
        break;
    case 2: {
        CEffect* effect = CEffectMan::getEffect(m_hEffectHandle);
        if (effect) effect->color().w = m_pModels->getAlphaScale();
        if (!isFade()) {
            CEffectMan::killEffect(m_hEffectHandle, 0.0f);
            m_hEffectHandle = 0;
            m_pMove->enableGravity(FALSE);
            m_pMove->setFlag(9, 0);
            m_iStep = 3;
        }
        break;
    }
    }
}

void CPrize::endCardMainSpecial_() {}

void CPrize::flashing_() {
    if (getFlag() & 0x800) {
        CEffect* effect = CEffectMan::getEffect(m_hEffectHandle);
        if (effect) {
            if (getFlashingCounter()) {
                effect->setDrawing(FALSE);
            } else {
                effect->setDrawing(TRUE);
            }
        }
    }
}

static inline void _setCard(CARD& c, USHORT kind, BYTE num) {
    c.m_nId = 0;
    c.m_nKind2 = kind;
    c.m_nNumber2 = num;
    c.m_nCategory20 = cardDataTbl[c.m_nKind].nCategory0;
    c.m_nCategory21 = cardDataTbl[c.m_nKind].nCategory1;
}

void CPrize::playerPrizeGet_() {
    if (!m_bReturnCard) {
        CPlayer::notifyGetPrize(m_PrizeKind, m_iNumber, 1);
        return;
    }
    CARD card;
    _setCard(card, m_PrizeKind, m_iNumber);
#line 1113
    ASSERT(g_pPlayer);
    g_pPlayer->notifyReturnCard(&card);
}
BOOL CPrize::initPrize_() {
    const PRIZE_DATA* data = &Prize::g_PrizeDataTbl[m_PrizeKind - 0xCB];
    m_pMove->setRadius(data->fRadius);
    m_pMove->setShiftY(0.0f);
    FVECTOR pos;
    pos.init();
    m_hEffectHandle = attachEffect(data->pEffectName, data->iEffectNo, &Crd(0), &pos, NULL, 0.0f);
    CEffect* effect = CEffectMan::getEffect(m_hEffectHandle);
    if (effect) effect->setAttachNoRot(TRUE);
    m_State[0].set(this, &CPrize::beginPrizeMain_, &CPrize::prizeMain_, &CPrize::endPrizeMain_);
    gotoState(m_State[0]);
    return TRUE;
}
BOOL CPrize::initCard_() {
    if (isFriendCard_()) goto flag;
    if (isGimmickCard_()) {
        CardBattle::CCardBattle* cb;
        cb = (CardBattle::CCardBattle*)CTaskMan::getSysTask(22);
#line 1169
        ASSERT(cb);
        if (cb->getGimmickCount() > 0) return FALSE;
    flag:
        if (CGameWork::getFlag(1, 0x22)) return FALSE;
        CGameWork::setFlag(1, 0x22, TRUE);
    } else if (m_PrizeKind == 0xA0) {
        return initReloadCard_();
    }
    INT groupID = 0xA028;
    INT count = CRsrcDB::getModelCount(groupID);
    if (count <= 0) return FALSE;
    CARD card;
    _setCard(card, m_PrizeKind, m_iNumber);
    INT mdlIndex;
    if (m_PrizeKind != 0xB9) mdlIndex = Prize::g_iCardModelIndex[card.m_nCategory21];
    else mdlIndex = 10;
#line 1203
    ASSERT(0 <= mdlIndex && mdlIndex < count);
    FVECTOR texST;
    char texName[16];
    CCard::getCardTexInfo(card, texName, texST);
    G_ST st[4];
    st[0].s = texST.x;
    st[0].t = texST.y;
    st[1].s = texST.z;
    st[1].t = texST.y;
    st[2].s = texST.x;
    st[2].t = texST.w;
    st[3].s = texST.z;
    st[3].t = texST.w;
    MODELDATA* orgMdl = CRsrcDB::getRsrcMDL(groupID)[mdlIndex];
#line 1215
    ASSERT(orgMdl);
#line 1217
    MODELDATA* d = m_pCardModel = new (__FILE__ "(1217)") MODELDATA;
    if (!d) return FALSE;
    ((u_long128*)d)[0] = ((u_long128*)orgMdl)[0];
    ((u_long128*)d)[1] = ((u_long128*)orgMdl)[1];
    m_pCardModel->m_pData = NULL;
    m_pCardModel->m_pSemiData = NULL;
    INT dataSize1 = 0;
    INT dataSize2 = 0;
    if (orgMdl->m_pData) {
        MODELTAG* end = (MODELTAG*)(orgMdl->m_pSemiData ? orgMdl->m_pSemiData : orgMdl->m_pBBox);
        dataSize1 = (INT)end - (INT)orgMdl->m_pData;
        dataSize1 >>= 4;
    }
    if (orgMdl->m_pSemiData) {
        dataSize2 = (INT)orgMdl->m_pBBox - (INT)orgMdl->m_pSemiData;
        dataSize2 >>= 4;
    }
#line 1244
    MODELTAG* newMdl = new (__FILE__ "(1244)") MODELTAG[dataSize1 + dataSize2];
    if (!newMdl) return FALSE;
    memcpy(newMdl, orgMdl->m_pData, (dataSize1 + dataSize2) * sizeof(MODELTAG));
    if (orgMdl->m_pData) m_pCardModel->m_pData = newMdl;
    if (orgMdl->m_pSemiData) m_pCardModel->m_pSemiData = newMdl + dataSize1;
    MODELTAG* tag = (MODELTAG*)m_pCardModel->m_pData;
    if (tag) {
        INT index = 0;
        while (!(tag[index][0] == 0x60000000 && !tag[index][1] && !tag[index][2] && !tag[index][3])) {
            index++;
            INT num = (USHORT)tag[index][1];
            INT mode = (USHORT)tag[index][3];
            index += 2;
            for (INT i = 0; i < num; i++) {
                index += 2;
                if (MODELDATA::isNormal(mode)) index++;
                if (MODELDATA::isColorI(mode) || MODELDATA::isColorF(mode)) index++;
                if (MODELDATA::isTexture(mode)) {
                    FLOAT* s = (FLOAT*)&tag[index][0];
                    FLOAT* t = (FLOAT*)&tag[index][1];
                    for (INT j = 0; j < 4; j++) {
                        if (Prize::g_CardST[j].s == *s && Prize::g_CardST[j].t == *t) {
                            G_ST& r = st[j];
                            *s = r.s;
                            *t = r.t;
                            break;
                        }
                    }
                    index++;
                }
            }
            do { index++; } while (0); /* a loop construct (macro?): its loop note aligns the test label below */
        }
    }
    m_iModelCount = 1;
#line 1320
    m_pModels = new (__FILE__ "(1320)") CModel[m_iModelCount];
    if (!m_pModels) return FALSE;
#line 1324
    m_pvModelColor = new (__FILE__ "(1324)") FVECTOR[m_iModelCount];
    if (!m_pvModelColor) return FALSE;
    if (m_pCardModel->enableLight()) {
#line 1331
        m_pNLight = new (__FILE__ "(1331)") CLightUnit;
        if (!m_pNLight) return FALSE;
    }
    if (m_pCardModel->enablePointLight()) {
#line 1338
        m_pGLight = new (__FILE__ "(1338)") CGimmickLightUnit;
        if (!m_pGLight) return FALSE;
    }
    m_pModels->link(this, m_pCardModel);
    if (m_pCardModel->enablePointLight()) {
        m_pModels->setLightUnit(m_pGLight);
    } else if (m_pCardModel->enableLight()) {
        m_pModels->setLightUnit(m_pNLight);
    }
    m_pModels->changeTexture("dummy", texName);
    m_pvModelColor[0] = m_pModels->getColorScale();
    if (m_pNLight) m_pNLight->setAnchor(&Crd(0), 2);
    if (m_pGLight) m_pGLight->setAnchor(&Crd(0), 2);
    attachCardEffect_();
    m_iID = groupID;
    setEntityFlag(0x2000, 0);
    setTimer(30000);
    m_pMove->setRadius(0.4f);
    m_pMove->setShiftY(-0.2f);
    m_State[0].set(this, &CPrize::beginCardMain_, &CPrize::cardMain_, &CPrize::endCardMain_);
    gotoState(m_State[0]);
    return TRUE;
}
BOOL CPrize::initReloadCard_() {
    INT groupID = 0xA029;
    MODELDATA** pModelTbl;
    INT i;

    m_iModelCount = CRsrcDB::getModelCount(groupID);
    if (m_iModelCount <= 0) return FALSE;
    pModelTbl = CRsrcDB::getRsrcMDL(groupID);
    if (!pModelTbl) return FALSE;
#line 1402
    m_pModels = new (__FILE__ "(1402)") CModel[m_iModelCount];
    if (!m_pModels) return FALSE;
#line 1406
    m_pvModelColor = new (__FILE__ "(1406)") FVECTOR[m_iModelCount];
    if (!m_pvModelColor) return FALSE;
    for (i = 0; i < m_iModelCount; i++) {
        if (pModelTbl[i]->enableLight()) {
#line 1415
            m_pNLight = new (__FILE__ "(1415)") CLightUnit;
            if (!m_pNLight) return FALSE;
            break;
        }
    }
    for (i = 0; i < m_iModelCount; i++) {
        if (pModelTbl[i]->enablePointLight()) {
#line 1426
            m_pGLight = new (__FILE__ "(1426)") CGimmickLightUnit;
            if (!m_pGLight) return FALSE;
            break;
        }
    }
    for (i = 0; i < m_iModelCount; i++) {
        m_pModels[i].link(this, pModelTbl[i]);
        m_pModels[i].linkTexAnm(m_pTexAnm, m_nTexAnmNum);
        if (pModelTbl[i]->enablePointLight()) {
            m_pModels[i].setLightUnit(m_pGLight);
        } else if (pModelTbl[i]->enableLight()) {
            m_pModels[i].setLightUnit(m_pNLight);
        }
        m_pvModelColor[i] = m_pModels[i].getColorScale();
    }
    if (m_pNLight) m_pNLight->setAnchor(&Crd(0), 2);
    if (m_pGLight) m_pGLight->setAnchor(&Crd(0), 2);
    attachCardEffect_();
    m_iID = groupID;
    setEntityFlag(0x2000, 0);
    setTimer(30000);
    m_pMove->setRadius(0.4f);
    m_pMove->setShiftY(-0.2f);
    m_State[0].set(this, &CPrize::beginCardMain_, &CPrize::cardMain_, &CPrize::endCardMain_);
    gotoState(m_State[0]);
    return TRUE;
}
void CPrize::attachCardEffect_() {
    FVECTOR pos;
    pos.init();
    m_hEffectHandle = attachEffect("rec_card_work", 0, &Crd(0), &pos, NULL, 0.0f);
    CEffect* effect = CEffectMan::getEffect(m_hEffectHandle);
    if (effect) effect->setScale(5.0f, 5.0f, 5.0f);
}

void CPrize::playBoundSE_() {
    switch (m_PrizeKind) {
    case 0xCB:
    case 0xCC:
    case 0xCD:
    case 0xCE:
    case 0xCF:
    case 0xD0:
    case 0xD1:
        playSEPanAndVol(0, 0x2C, 0, NULL, 0);
        break;
    case 0xD2:
    case 0xD3:
        playSEPanAndVol(0, 0x2A, 0, NULL, 0);
        break;
    case 0xD4:
    case 0xD5:
        playSEPanAndVol(0, 0x2B, 0, NULL, 0);
        break;
    }
}

void CPrize::playGetSE_() {
    switch (m_PrizeKind) {
    case 0xCB:
    case 0xCC:
    case 0xCD:
    case 0xCE:
    case 0xCF:
    case 0xD0:
    case 0xD1:
        CSound::playSE(0x32, -1, 0x40);
        break;
    case 0xD2:
    case 0xD3:
        CSound::playSE(0x30, -1, 0x40);
        break;
    case 0xD4:
    case 0xD5:
        CSound::playSE(0x31, -1, 0x40);
        break;
    default:
        CSound::playSE(6, -1, 0x40);
        break;
    }
}

void CPrize::cardRotation_() {
    ((FVECTOR&)getRot()).y += getFrameRateF() * 0.06981317f;
    ((FVECTOR&)getRot()).y = Math3D::LimitPI(getRot().y);
}

BOOL CPrize::isGimmickCard_() {
    if (m_PrizeKind < 0x43) {
        if (m_PrizeKind >= 0x3A) return TRUE;
    }
    return FALSE;
}

BOOL CPrize::isFriendCard_() {
    if (m_PrizeKind < 0x3A) {
        if (m_PrizeKind >= 0x31) return TRUE;
    }
    return FALSE;
}

BOOL CPrize::isDraw_() {
    if (CGameWork::getFlag(1, 0x21)) return FALSE;
    if (getPos().y < -75.0f) return TRUE;
    if (!m_bHoming) return FALSE;
    FLOAT radius;
    if (g_pPlayer) {
        radius = g_pPlayer->getCardDrawRadius();
        radius *= radius;
    } else {
        radius = 25.0f;
    }
    if (calcDistanceSq(g_pPlayer) > radius) return FALSE;
    return TRUE;
}
void CPrize::findPosition_(const PRIZE_PARAM& inParam) {
    CCollMap* coll = m_pMove->getMapCollision();
#line 1638
    ASSERT(coll);
    FVECTOR start;
    FVECTOR end;
    FVECTOR pos = inParam.vPos;
    TRACE("================start ... (%.5f, %.5f, %.5f)\n", pos.x, pos.y, pos.z);
    FVECTOR v = getPlayerPos(FALSE);
    start = v;
    end = pos;
    start.y = end.y;
    if (Math3D::PointToPointDistanceSq(start, end) <= 25.0f) {
        FLOAT r = coll->getRadius() + 0.1f;
        SubVectorXYZ(v, start, end);
        Normalize(v, v);
        ScaleVectorXYZ(v, v, r);
        AddVectorXYZ(start, start, v);
        coll->setStartPos(start);
        coll->setEndPos(end);
        if (coll->collideRide(FALSE)) {
            pos = *coll->getOutPos();
            coll->setEndPos(pos);
        }
        if (coll->collideMap()) pos = *coll->getOutPos();
        TRACE("================step1 ... (%.5f, %.5f, %.5f)\n", pos.x, pos.y, pos.z);
    }
    start = pos;
    end = pos;
    start.y -= 2.5f;
    coll->setStartPos(start);
    coll->setEndPos(end);
    if (coll->collideRide(FALSE)) {
        pos = *coll->getOutPos();
        coll->setEndPos(pos);
    }
    if (coll->collideMap()) pos = *coll->getOutPos();
    TRACE("================step2 ... (%.5f, %.5f, %.5f)\n", pos.x, pos.y, pos.z);
    for (FLOAT deg = 0.0f; deg < 360.0f; deg += 45.0f) {
        FLOAT theta = Math3D::LimitPI(deg * 0.017453292f);
        start = pos;
        start.x += sinf(theta);
        start.z += cosf(theta);
        FLOAT d = Math3D::LimitPI(theta + 3.1415927f);
        end = pos;
        end.x += sinf(d);
        end.z += cosf(d);
        coll->setStartPos(start);
        coll->setEndPos(end);
        if (coll->collideRide(FALSE)) {
            pos = *coll->getOutPos();
            coll->setEndPos(pos);
        }
        if (coll->collideMap()) pos = *coll->getOutPos();
    }
    TRACE("================result ... (%.5f, %.5f, %.5f)\n", pos.x, pos.y, pos.z);
    warp(pos);
}

// SOURCE DATA: .rodata
