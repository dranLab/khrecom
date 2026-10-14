#define TASK_H_NO_SLC_OVERFLOW_MSG
#include "System/Graphic/Gprim.h"
#include "System/Graphic/Model.h"
#include "System/Graphic/Anim.h"
#include "System/Graphic/2d/2DMan.h"
#include "Game/CardBattle.h"
#include "System/Graphic/Str.h"
#include "Game/Collision/CollUtil.h"
#include "System/Graphic/LightUnit.h"
#include "Game/Player/NaviMap.h"
#include "System/Graphic/Camera.h"
#include "System/Graphic/2d/2DSeqCtrl.h"
#include "System/Graphic/2d/2DFnt.h"
#include "System/Graphic/2d/2DCard.h"
#include "Game/Player/Player.h"
#include "Game/SceneInfo.h"
#include "Game/SceneEnv.h"
#include "Game/GameWork.h"
#include "Game/Game.h"
#include "Game/Debug/DebugWork.h"
#include "System/MainApp.h"
#include "System/HeapMan.h"
#include "include_asm.h"


#define RET_ADDR() ({ UINT nRA; __asm__ volatile("daddu %0, $31, $0" : "=r"(nRA)); nRA; })

inline void* operator new(size_t size, char* msg) throw() {
    return com_malloc_d(size, msg, RET_ADDR());
}
inline void* operator new[](size_t size, char* msg) throw() {
    return com_malloc_d(size, msg, RET_ADDR());
}
inline void operator delete[](void* mem) { com_free(mem); }

#define ASSERT(e) if (!(e)) AssertApp(__FILE__, __LINE__, #e, 0)

class CRsrcDB {
public:
    static INT getFirstRsrc(INT nType, INT nGroup, INT nNum);
    static INT getModelCount(INT nGroup);
    static MODELDATA** getRsrcMDL(INT nGroup);
};

namespace D2 {
class C2DMan {
public:
    static HANDLE create(const char* pName, UINT nIdx);
    static void destroy(HANDLE h);
    static void setLayer(INT nLayer);
    static BOOL isEnd(HANDLE h);
    static void enableDisp(HANDLE h, BOOL bEnable);
    static void getLayPosition(const char* pName, UINT nIdx, UINT nLay, FVECTOR& vPos, BOOL b);
};
}  // namespace D2

extern "C" {
void sceVu0RotMatrixX(FMATRIX& m0, FMATRIX& m1, float rx);
void sceVu0RotMatrixY(FMATRIX& m0, FMATRIX& m1, float ry);
void sceVu0InversMatrix(FMATRIX& m0, FMATRIX& m1);
void sceVu0ViewScreenMatrix(FMATRIX& m, float scrz, float ax, float ay, float cx, float cy, float zmin, float zmax,
                            float nearz, float farz);
}
void MulMatrix(FMATRIX& res, const FMATRIX& m0, const FMATRIX& m1);

extern FLOAT ASPECT_RATE_X;
extern FLOAT ASPECT_RATE_Y;

using namespace D2;

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

static inline void _setCard(CARD& c, USHORT kind, BYTE num) {
    c.m_nId = 0;
    c.m_nNumber2 = num;
    c.m_nKind2 = kind;
    c.m_nCategory20 = cardDataTbl[c.m_nKind].nCategory0;
    c.m_nCategory21 = cardDataTbl[c.m_nKind].nCategory1;
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

static inline void _unitVectorVu0(FVECTOR& v) {
    asm volatile("sqc2 $vf0, 0x0(%0)" : : "r"(&v) : "memory");
}

static inline void _viewScreenMatrix(FMATRIX& matOut, FLOAT fInAx, FLOAT fInAy) {
    sceVu0ViewScreenMatrix(matOut, 512.0f, ASPECT_RATE_X, ASPECT_RATE_Y, fInAx, fInAy, 1.0f, 16777000.0f, 0.1f, 100.0f);
}

static inline FLOAT _deg2rad(FLOAT r) { return r * 3.1415927f / 180.0f; }

static inline FLOAT _roundRot(FLOAT fInRot) {
    FLOAT r;
    if (fInRot < -3.1415927f) {
        r = fInRot + 6.2831855f;
    } else {
        r = fInRot;
        if (3.1415927f < r) {
            r -= 6.2831855f;
        }
    }
    return r;
}

namespace Gauge {

BOOL CNaviMap::m_bBattleEnd = 0;

static LPCSTR szNaviMap2DFile[31] = {
    "", "tr_navi.lad", "ag_navi.lad", "ol_navi.lad", "wo_navi.lad", "mo_navi.lad", "ha_navi.lad", "at_navi.lad",
    "ne_navi.lad", "ho_navi.lad", "", "tw_navi.lad", "de_navi.lad", "ca_navi.lad", "tr_navi.lad", "ag_navi.lad",
    "ol_navi.lad", "wo_navi.lad", "mo_navi.lad", "ha_navi.lad", "at_navi.lad", "ne_navi.lad", "ho_navi.lad", "",
    "tw_navi.lad", "de_navi.lad", "ca_navi.lad", "", "", "", "",
};

UINT g_uiFlag_WorldStart[27] = {
    0,       1,       2,       4,        8,        0x10,      0x20,      0x40,      0x80,
    0x100,   0x200,   0x400,   0x800,    0x1000,   0x10000,   0x20000,   0x40000,   0x80000,
    0x100000, 0x200000, 0x400000, 0x800000, 0x1000000, 0x2000000, 0x4000000, 0x8000000, 0x10000000,
};

CNaviMap::CNaviMap(CTask* pParent) : CTask(pParent, 0x4000) {
    _initialize();
}

CNaviMap::~CNaviMap() {
    if (m_pModels) {
        delete[] m_pModels;
    }
    m_pModels = NULL;
    if (m_pLight) {
        delete m_pLight;
    }
    m_pLight = NULL;
    if (m_hBack > 0) {
        C2DMan::destroy(m_hBack);
    }
    if (m_hFront > 0) {
        C2DMan::destroy(m_hFront);
    }
    if (m_pRoomName) {
        delete[] m_pRoomName;
    }
    m_pRoomName = NULL;
    if (m_pCard) {
        delete m_pCard;
    }
    m_pCard = NULL;
    m_bBattleEnd = FALSE;
}

void CNaviMap::_initialize() {
    m_uiState = 2;
    m_pModels = NULL;
    m_pLight = NULL;
    m_uiDispFlag = 0;
    m_hBack = 0;
    m_hFront = 0;
    m_pRoomName = NULL;
    _unitVectorVu0(m_vPos);
    m_fDeg = 0.0f;
    m_fRotOfs = 0.0f;
    INT worldID = CSceneInfo::getCurrentWorldID(0);
    INT sceneID = CSceneInfo::getCurrentSceneNum();
    m_pCard = NULL;
    m_uiDispFlag |= 0x1000000;
    if (worldID >= 14) {
        worldID = 13;
    }
    m_uiRoomState = 1;
    m_uiCardState = 1;
    INT roomID = CSceneInfo::getCurrentRoomID();
    if (roomID == 0 || roomID == 99) {
        worldID = 13;
    }
    m_nWorldID = worldID;
    if (CGame::isRikuGame()) {
        m_nWorldID += 13;
        if (worldID == 12 && sceneID == 30) {
            m_fRotOfs = 2.3561945f;
        }
    }
    LPCSTR file = szNaviMap2DFile[m_nWorldID];
    INT nID = worldID + 0x32C8;
    if (!CRsrcDB::getFirstRsrc(12, nID, 1)) {
        m_uiState = 0;
        m_uiDispFlag |= 0x800000;
    } else {
        m_uiState = 1;
        m_uiDispFlag |= 0x800000;
    }
    m_nModelCount = CRsrcDB::getModelCount(nID);
    if (m_nModelCount) {
        MODELDATA** pModelTbl = CRsrcDB::getRsrcMDL(nID);
        if (pModelTbl) {
            INT i;
#line 194
            m_pModels = new (__FILE__ "(194)") CModel[m_nModelCount];
            ASSERT(m_pModels);
            for (i = 0; i < m_nModelCount; i++) {
                if (pModelTbl[i]->enableLight()) {
#line 201
                    m_pLight = new (__FILE__ "(201)") CLightUnit;
                    ASSERT(m_pLight);
                    break;
                }
            }
            for (i = 0; i < m_nModelCount; i++) {
                m_pModels[i].link(this, pModelTbl[i]);
                if (pModelTbl[i]->enableLight()) {
                    m_pModels[i].setLightUnit(m_pLight);
                }
            }
            CCoord& crd = Crd(0);
            _unitMatrixVu0(crd.m_Local);
            sceVu0RotMatrixY(crd.m_Local, crd.m_Local, 3.1415927f);
            crd.m_Local.m[3][0] = m_vPos.x;
            crd.m_Local.m[3][1] = m_vPos.y;
            crd.m_Local.m[3][2] = m_vPos.z;
            if (m_pLight) {
                m_pLight->setAnchor(&crd, 0);
                FVECTOR vAmbient;
                vAmbient.x = 1.0f;
                vAmbient.y = 1.0f;
                vAmbient.z = 1.0f;
                vAmbient.w = 0.0f;
                m_pLight->setAmbient(vAmbient);
            }
            m_vPosWSC.set(0.0f, 0.0f, 0.4f, 1.0f);
            FVECTOR pos;
            C2DMan::getLayPosition(file, 1, 0, pos, 0);
            m_PosScreen.x = (INT)pos.x - 256;
            m_PosScreen.y = 192 - (INT)pos.y;
            changeLocalWS();
        }
    }
    calcDispFlag();
    _setCard(m_cardRoom, 0, 0);
    RoomInfo* pRoom = CSceneInfo::getRoomInfo(-1);
    if (pRoom) {
        _setCard(m_cardRoom, pRoom->getRoomNameCardID(), 0xFF);
#line 254
        m_pRoomName = new (__FILE__ "(254)") C2DSeqCtrl[2];
    }
    if (m_cardRoom.m_nCategory20 == 9) {
#line 257
        m_pCard = new (__FILE__ "(257)") C2DCard;
    }
}


}  // namespace Gauge

/* main's jump tables (the function is still assembly). */
asm(".rdata\n.align 4\njtbl_00497810:\n"
    ".word .L0025EB64\n.word .L0025EA5C\n.word .L0025EB0C\n.word .L0025EB64\n.word .L0025EB64\n.word .L0025EB64\n"
    ".align 4\njtbl_00497830:\n"
    ".word .L0025ECE4\n.word .L0025EB8C\n.word .L0025EC10\n.word .L0025ECA4\n.word .L0025ECE4\n.word .L0025ECE4\n"
    ".text");
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Game/Player/NaviMap", main__Q25Gauge8CNaviMap);

namespace Gauge {

// Not matching: in the debug-rotation block (g_dw.player.iTemp == 2) the original keeps PI in $f3 from before the
// branch and loads 180.0f/-PI in $f2/$f1 (100.0f and -PI share $f1); here 180.0f is hoisted too and the registers of
// 180.0f/-PI swap (9 lines). Everything else matches; the "deg" temporary only reproduces the PI hoisting.
#if 0
void CNaviMap::main() {
    if (CSceneEnv::isStopGauge()) {
        return;
    }
    if (m_uiState == 1) {
        if (CGameWork::getFlag(1, 0x37)) {
            m_uiDispFlag |= 0x2000000;
            CGameWork::setFlag(1, 0x37, 0);
        }
    } else if (m_uiState != 2) {
        if (CGameWork::getByte(0, 0x67) || (CGameWork::getInt(1, 0xE2) & 8)) {
            if (!(m_uiDispFlag & 0x800000)) {
                m_uiDispFlag |= 0x800000;
                if (m_hBack > 0) {
                    C2DMan::enableDisp(m_hBack, FALSE);
                }
            }
        } else {
            if (m_uiDispFlag & 0x800000) {
                m_uiDispFlag &= ~0x800000;
                if (m_hBack > 0) {
                    C2DMan::enableDisp(m_hBack, TRUE);
                }
            }
        }
    }
    if (m_cardRoom.m_nKind) {
        if (CGameWork::getInt(1, 0xE2) & 8) {
            if (!(m_uiDispFlag & 0x1000000)) {
                m_uiDispFlag |= 0x1000000;
                if (m_pRoomName) {
                    m_pRoomName->enableDisp(FALSE);
                    if (m_pCard) {
                        m_pRoomName[1].enableDisp(FALSE);
                        m_pCard->enableDisp(FALSE);
                    }
                }
            }
        } else {
            if (m_uiDispFlag & 0x1000000) {
                m_uiDispFlag &= ~0x1000000;
                if (m_pRoomName) {
                    m_pRoomName->enableDisp(TRUE);
                    if (m_pCard) {
                        m_pRoomName[1].enableDisp(TRUE);
                        m_pCard->enableDisp(TRUE);
                    }
                }
            }
        }
    }
    switch (m_uiState) {
    case 0:
        break;
    case 1:
        if (!CGameWork::getByte(0, 0x67) && !(CGameWork::getInt(1, 0xE2) & 8)) {
            LPCSTR file = szNaviMap2DFile[m_nWorldID];
            C2DMan::setLayer(2);
            if (m_uiDispFlag & 0x2000000) {
                m_uiState = 2;
                m_hBack = C2DMan::create(file, 0);
            } else {
                m_uiState = 3;
                m_hBack = C2DMan::create(file, 1);
            }
            m_uiDispFlag &= ~0x2000000;
            m_uiDispFlag &= ~0x800000;
        }
        break;
    case 2:
        if (m_hBack && C2DMan::isEnd(m_hBack)) {
            LPCSTR file = szNaviMap2DFile[m_nWorldID];
            C2DMan::destroy(m_hBack);
            C2DMan::setLayer(2);
            m_hBack = C2DMan::create(file, 1);
            m_uiState = 3;
        }
        break;
    case 3:
        break;
    case 4:
        break;
    case 5:
        break;
    }
    switch (m_uiRoomState) {
    case 0:
        break;
    case 1:
        if (!(m_uiDispFlag & 0x800000) || CGameWork::getByte(0, 0x67)) {
            m_uiDispFlag &= ~0x2000000;
            m_uiDispFlag &= ~0x1000000;
            if (m_cardRoom.m_nKind) {
                LPCSTR file = szNaviMap2DFile[m_nWorldID];
                m_uiRoomState = 2;
                m_uiCardState = 2;
                initRoomName(file, 4);
            }
        }
        break;
    case 2:
        if (!(m_uiDispFlag & 0x1000000) && m_cardRoom.m_nKind) {
            if (m_pRoomName && m_pRoomName->isEnd(0)) {
                LPCSTR file = szNaviMap2DFile[m_nWorldID];
                m_uiCardState = 3;
                m_uiRoomState = 3;
                initRoomName(file, 5);
            }
            if (m_uiCardState >= 2 && m_pCard) {
                m_pCard->checkBack();
            }
        }
        break;
    case 3:
        if (!(m_uiDispFlag & 0x1000000) && m_cardRoom.m_nKind && m_uiCardState == 3 && m_pCard) {
            m_pCard->checkBack();
        }
        break;
    case 4:
        break;
    case 5:
        break;
    }
    if (m_pModels) {
        CCoord& crd = Crd(0);
        _unitMatrixVu0(crd.m_Local);
        FVECTOR rot;
        FLOAT deg = 55.0f;
        rot.x = _deg2rad(deg);
        rot.y = 0.0f;
        if (2 == DebugWork::g_dw.player.iTemp) {
            rot = DebugWork::g_dw.player.vTemp;
            rot.x = _roundRot(_deg2rad(rot.x * 100.0f));
        }
        sceVu0RotMatrixY(crd.m_Local, crd.m_Local, rot.y);
        sceVu0RotMatrixX(crd.m_Local, crd.m_Local, rot.x);
        crd.m_Local.v[3] = m_vPos;
        crd.touch();
        if (m_nModelCount >= 22) {
            CCoord& crd1 = Crd(1);
            _unitMatrixVu0(crd1.m_Local);
            rot.y = _roundRot(g_pPlayer->getRot().y - 0.7853982f + m_fRotOfs);
            sceVu0RotMatrixY(crd1.m_Local, crd1.m_Local, rot.y);
            crd1.m_Local.v[3] = m_vPos;
            Crd(1).touch();
        }
    }
}

#endif

void CNaviMap::disp() {
    if (CGameWork::getByte(0, 0x67)) {
        return;
    }
    if (CGameWork::getInt(1, 0xE2) & 8) {
        return;
    }
    if (m_uiDispFlag & 0x800000) {
        return;
    }
    if (!DebugWork::g_dw.sys.m_iFukidasiDisp) {
        return;
    }
    if (!m_pModels) {
        return;
    }
    if (m_pLight) {
        m_pLight->calc();
    }
    for (INT i = 0; i < m_nModelCount; i++) {
        if (m_uiDispFlag & (1 << i)) {
            m_pModels[i].disp();
        }
    }
}

void CNaviMap::changeLocalWS() {
    FMATRIX mVS;
    FMATRIX mWV;
    _unitMatrixVu0(mWV);
    sceVu0RotMatrixY(mWV, mWV, 3.1415927f);
    mWV.m[3][2] = m_vPosWSC.z;
    sceVu0InversMatrix(mWV, mWV);
    mWV.m[0][0] = -mWV.m[0][0];
    mWV.m[1][0] = -mWV.m[1][0];
    mWV.m[2][0] = -mWV.m[2][0];
    mWV.m[3][0] = -mWV.m[3][0];
    mWV.m[0][1] = -mWV.m[0][1];
    mWV.m[1][1] = -mWV.m[1][1];
    mWV.m[2][1] = -mWV.m[2][1];
    mWV.m[3][1] = -mWV.m[3][1];
    _viewScreenMatrix(mVS, m_PosScreen.x + 0x800, 0x800 - m_PosScreen.y);
    MulMatrix(m_mWS, mVS, mWV);
    for (INT i = 0; i < m_nModelCount; i++) {
        m_pModels[i].setWorldScreenMatrix(&m_mWS);
        m_pModels[i].changeLayerExt(1, 2);
    }
}

void CNaviMap::calcDispFlag() {
    m_uiDispFlag = (m_uiDispFlag & 0x800000) | 1;
    if (CGame::isSoraGame()) {
        m_uiDispFlag |= 0x200000;
    } else if (CGame::isRikuGame()) {
        m_uiDispFlag |= 0x400000;
    }
    for (INT i = 0; i < 4; i++) {
        DoorInfo* pDoorInfo = CSceneInfo::getDoorInfo((DOOR_ID)i, -1);
        if (!pDoorInfo) {
            continue;
        }
        switch (pDoorInfo->getDoorState()) {
        case DOOR_STAT_CLOSED:
            if (pDoorInfo->isSpecialDoor()) {
                m_uiDispFlag |= 0x20000 << i;
            } else {
                m_uiDispFlag |= 2 << i;
            }
            break;
        case DOOR_STAT_OPENED:
            m_uiDispFlag |= 0x2000 << i;
            if (pDoorInfo->isSpecialDoor()) {
                m_uiDispFlag |= 0x20000 << i;
            } else if (pDoorInfo->isWorldEntrance()) {
                m_uiDispFlag |= 0x200 << i;
            } else {
                m_uiDispFlag |= 0x20 << i;
            }
            break;
        }
    }
}

void CNaviMap::initRoomName(const CHAR* inFile, UINT inIdxSeq) {
    if (!m_pRoomName) {
        return;
    }
    if (strlen(inFile) < 4) {
        return;
    }
    C2DSeqCtrl* pSeq = m_pRoomName;
    FVECTOR pos;
    pos.init();
    pSeq->destroy();
    pSeq->setLayer(2);
    pSeq->addSeq((LPCTSTR)inFile, 0, inIdxSeq);
    pSeq->setPos(pos);
    G_RGBA rgba;
    rgba.col = 0x80808080;
#line 689
    C2DFnt* p2DFnt = new (__FILE__ "(689)") C2DFnt(0, 0, &rgba, 13, m_cardRoom.getName(), 0x42);
    pSeq->addFnt((LPCTSTR)inFile, 0, p2DFnt);
    pSeq->enableColorCtrl(TRUE);
    if (!m_pCard) {
        return;
    }
    switch (m_uiCardState) {
    case 2:
        pSeq = &m_pRoomName[1];
        pSeq->setLayer(2);
        pSeq->addSeq((LPCTSTR)inFile, 0, inIdxSeq - 2);
        pSeq->setPos(pos);
        m_pCard->setLayer(3);
        m_pCard->create(m_cardRoom);
        m_pCard->setParent(pSeq);
        m_pCard->enableColorCtrl(TRUE);
        break;
    case 3:
        pSeq = &m_pRoomName[1];
        pSeq->destroy();
        pSeq->setLayer(2);
        pSeq->addSeq((LPCTSTR)inFile, 0, inIdxSeq - 2);
        pSeq->setPos(pos);
        m_pCard->destroy();
        m_pCard->setLayer(3);
        m_pCard->create(m_cardRoom);
        m_pCard->setParent(pSeq);
        m_pCard->enableColorCtrl(TRUE);
        break;
    }
}


}  // namespace Gauge

// SOURCE DATA: .data .rodata
