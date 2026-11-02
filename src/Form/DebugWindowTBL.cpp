/* .rodata starts with the strings of the included headers' inline functions, in this order. */
#include "System/Graphic/Gprim.h"
#include "System/Graphic/Str.h"
#include "System/TaskMan.h"
#include "System/Task.h"
#define DEBUGPATHDATA_H_PATH "Game/Debug/DebugPathData.h" /* MATCH: the original includes it under this name */
#include "Game/Debug/DebugPathData.h"
#include "Game/Collision/CollUtil.h"
#include "Game/Collision/CollDisp.h"
#include "System/Graphic/Anim.h"
#include "System/Graphic/2d/2DMan.h"
#include "Game/CardBattle.h"

/* Placeholder: an empty string follows CardBattle.h's strings in .rodata (an unknown header's inline function). */
inline void _emptyStringPlaceholder() {
    TRACE("");
}

#include "System/Graphic/modeldata.h"
#include "System/Graphic/Model.h"
#include "Game/Technic.h"
#include "Game/Status.h"
#include "Game/Card.h"
#include "System/PadIO.h"
#include "System/Rnd.h"
#include "System/MainApp.h"
#include "System/Sound/SoundMan.h"
#include "System/Graphic/Camera.h"
#include "System/Graphic/DeFocus.h"
#include "System/Graphic/WorldEnv.h"
#include "Game/Game.h"
#include "Game/GameWork.h"
#include "Game/SceneEnv.h"
#include "Game/EntityFactory.h"
#include "Game/Gimmick/GimmickManager.h"
#include "Game/Player/Player.h"
#include "Game/Player/PlayerAttack.h"
#include "Game/Player/PlayerCamera.h"
#include "Game/Player/PlayerManager.h"
#include "Game/Debug/DebugWork.h"
#include "Form/Menu/JMemoFlag.h"
#include "System/DebugProcInfo.h"
#define FORM_DEBUGWINDOW_FULL /* MATCH: CNakCamera/CGlidWorld; the original has Gprim.h's strings */
#include "Form/DebugWindow.h"
#include "Form/DebugWindowTBL.h"
#include "include_asm.h"

extern "C" int sprintf(char* buf, const char* fmt, ...);
extern char _end[];

/* The heap bounds a common header defines in every includer (dynamically initialized, first in .bss). */
namespace EERAM {
static INT _HEAP_START = (((INT)_end - 1) / 64 + 1) * 64;
static INT _HEAP_SIZE = 0x2000000 - 0x40000 - _HEAP_START;
}

inline void* operator new(size_t n) throw() { return com_malloc(n); }
inline void* operator new(size_t, void* p) throw() { return p; }

#define RET_ADDR() ({ UINT nRA; __asm__ volatile("daddu %0, $31, $0" : "=r"(nRA)); nRA; })

static inline void _unitVectorVu0(FVECTOR& v) {
    asm volatile("sqc2 $vf0, 0x0(%0)" : : "r"(&v));
}

/* Other units' classes, as far as this file uses them (their headers don't exist yet). */
class CShadowModelMan {
public:
    static FLOAT m_fCDensity;
    static FLOAT m_fADensity;
};















/* CGimmickProbDebug: Game/Gimmick/GimmickManager.h */






#include "System/Graphic/PackMan.h"




class CRsrcData {
private:
    void* m_pDataSrc;
    char m_szName[24];

public:
    char* getName() { return m_szName; }
};

#include "System/RsrcDB.h"






#include "Game/Atel/VSME.h"






































#include "Game/Atel/Atel.h"







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


namespace DWT {

/* DWTPlayer.h declares SCREEN_HEIGHT inside the namespace it is included in: bind it to the global. */
extern INT SCREEN_HEIGHT asm("SCREEN_HEIGHT");

char* szOnOff[2] = {"ON ", "OFF"};

char* szOffOn[2] = {"OFF", "ON "};

char* szFalseTrue[2] = {"FALSE", "TRUE "};

char* szCamSpeed[4] = {"\x82\xa8\x82\xbb\x82\xa2", "\x82\xd3\x82\xc2\x82\xa4", "\x82\xcd\x82\xe2\x82\xa2", "\x83}\x83" "b\x83n"};

char* szPlayDemo[4] = {"OFF", "REC", "PLAY", "\x83" "f\x83\x82"};

char* szAfterEffect[3] = {"\x82\xc8\x82\xb5", "\x83U\x83\x93\x83]\x83" "E", "\x83J\x83N\x83_\x83" "C"};

char* szFps[2] = {"60 FPS", "30 FPS"};

char* szDefOnOff[3] = {"NORMAL", "ON ", "OFF"};

char* szCollDispRef[2] = {"\x83v\x83\x8c\x83" "C\x83\x84\x81[", "\x83J\x83\x81\x83\x89"};

static BOOL debugWinYesNoLineCall(CDebugWindow* pObj, INT iLineY, INT inParam) {
    if (inParam & 0x20) {
        if (iLineY == 0) {
            pObj->sendMessage(3, 1);
            pObj->sendMessage(2, 0);
        }
    }
    if ((inParam & 0x40) || ((inParam & 0x20) && iLineY == 1)) {
        pObj->sendMessage(3, 0);
        pObj->sendMessage(2, 0);
    }
    return TRUE;
}

static DEBUG_WINDOW_LINE debugWinYesNoLine[3] = {
    {"     \x82x\x82" "d\x82r", &CDebugWindow::none, 1, {0}, 0, 0.0f, 0.0f, 0.0f, NULL, 0, debugWinYesNoLineCall, NULL},
    {"      \x82m\x82n ", &CDebugWindow::none, 1, {0}, 0, 0.0f, 0.0f, 0.0f, NULL, 0, debugWinYesNoLineCall, NULL},
    {NULL, NULL, 0, {0}, 0, 0.0f, 0.0f, 0.0f, NULL, 0, NULL, NULL},
};

DEBUG_WINDOW_TBL debugWinYesNo = {"\x82\xe6\x82\xeb\x82\xb5\x82\xa2\x82\xc5\x82\xb7\x82\xa9\x81H", -48, -32, 8, 2, 12, 0, 6, debugWinYesNoLine, NULL, NULL};

static void _makeInputName() {
    CHAR* pC = DebugWork::g_dw.nakamich.szNameBuf;
    INT iFlg = 0;
    for (INT i = 0; i < 8; i++) {
        if (pC[i] == 0) {
            iFlg = 1;
        }
        if (iFlg) {
            pC[i] = '*';
        }
    }
    pC[8] = 0;
}

void _makeDispName();
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Form/DebugWindowTBL", _makeDispName__3DWTv);
#if 0
/* differs in the copy loop: the original keeps i + j unreduced (addu/slti each pass) and lays the loop out with the first pass peeled */
static void _makeDispName() {
    INT i;
    INT j;
    CHAR* pC = DebugWork::g_dw.nakamich.szNameBuf;
    for (i = 0; i < 8 && pC[i] == '*'; i++) {
    }
    for (j = 0; (i + j < 8) && (pC[i + j] != '*'); ++j) {
        pC[j] = pC[i + j];
    }
    pC[j] = 0;
}
#endif

static BOOL debugWinNameLineCall(CDebugWindow* pObj, INT iLineY, INT inParam) {
    if (inParam & 1) {
        pObj->initCurX(8, 8, 8, 8);
        _makeInputName();
    }
    if (inParam & 0x20) {
        _makeDispName();
    } else if (inParam & 0x800) {
        if (!pObj->isChildAnswer() || pObj->getChildAnswer() != 1 || *DebugWork::g_dw.nakamich.szNameBuf == 0) {
            _makeInputName();
        } else {
            pObj->sendMessage(3, (INT)&DebugWork::g_dw.nakamich.szNameBuf);
            pObj->sendMessage(2, 0);
        }
    } else {
        CHAR* pC = &DebugWork::g_dw.nakamich.szNameBuf[pObj->getCurX()];
        INT iPM = 0;
        if (inParam & 0x200) {
            iPM = 1;
        }
        if (inParam & 0x400) {
            iPM = -1;
        }
        CHAR c = *pC;
        c += iPM;
        if (c < '*' || c > '_') {
            if (iPM >= 0) {
                c = '*';
            } else {
                c = '_';
            }
        } else if (c > '*' && c < '0') {
            if (iPM >= 0) {
                c = '0';
            } else {
                c = '*';
            }
        } else if (c > '9' && c < 'A') {
            if (iPM >= 0) {
                c = 'A';
            } else {
                c = '9';
            }
        } else if (c > 'Z' && c < '_') {
            if (iPM >= 0) {
                c = '_';
            } else {
                c = 'Z';
            }
        }
        *pC = c;
    }
    return TRUE;
}

static char* szdebugWinNameStr[1] = {DebugWork::g_dw.nakamich.szNameBuf};

static DEBUG_WINDOW_LINE debugWinNameLine[2] = {
    {" %s ", &CDebugWindow::none, 1, {0}, 0x40, 0.0f, 0.0f, 0.0f, szdebugWinNameStr, 0, debugWinNameLineCall, &debugWinYesNo},
    {NULL, NULL, 0, {0}, 0, 0.0f, 0.0f, 0.0f, NULL, 0, NULL, NULL},
};

DEBUG_WINDOW_TBL debugWinName = {"\x96\xbc\x91O\x90\xdd\x92\xe8", -40, -72, 5, 1, 16, 0, 0, debugWinNameLine, NULL, NULL};

#include "Game/Player/DWTPlayer.h"

static void _stopSE() {
    if (DebugWork::g_dw.sound.iSE_Handle >= 0) {
        CSound::stopSE(DebugWork::g_dw.sound.iSE_Handle, DebugWork::g_dw.sound.iSE_Id);
        DebugWork::g_dw.sound.iSE_Handle = -1;
    }
}

static BOOL _stopSE(CDebugWindow* pObj, INT inY, INT inParam) {
    if (inParam & 0x20) {
        _stopSE();
    }
    return TRUE;
}

static void _fadeoutSE() {
    if (DebugWork::g_dw.sound.iSE_Handle >= 0) {
        CSound::fadeoutSE(DebugWork::g_dw.sound.iSE_Handle, DebugWork::g_dw.sound.iSE_Id, 30);
        DebugWork::g_dw.sound.iSE_Handle = -1;
    }
}

static BOOL _fadeoutSE(CDebugWindow* pObj, INT inY, INT inParam) {
    if (inParam & 0x20) {
        _fadeoutSE();
    }
    return TRUE;
}


BOOL debugWinSeTestLineCall(CDebugWindow* pObj, INT iLineY, INT inParam);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Form/DebugWindowTBL", debugWinSeTestLineCall__3DWTP12CDebugWindowii);
#if 0
/* differs only in the delay slot of the playSE call: the original loads iSE_Id last (li a2,64 first) */
static BOOL debugWinSeTestLineCall(CDebugWindow* pObj, INT iLineY, INT inParam) {
    if (inParam & 0x20) {
        _stopSE();
        if (DebugWork::g_dw.sound.bVoice) {
            DebugWork::g_dw.sound.iSE_Id = (DebugWork::g_dw.sound.iSE_FileId * 1000 + DebugWork::g_dw.sound.iSE_SeId) | 0x10000000;
        } else {
            DebugWork::g_dw.sound.iSE_Id = DebugWork::g_dw.sound.iSE_FileId * 1000 + DebugWork::g_dw.sound.iSE_SeId;
        }
        DebugWork::g_dw.sound.iSE_Handle = CSound::playSE(DebugWork::g_dw.sound.iSE_Id, -1, 0x40);
    }
    if (inParam & 2) {
        _stopSE();
    }
    return TRUE;
}
#endif

static DEBUG_WINDOW_LINE debugWinSeTestLine[15] = {
    {" +- FILE-ID:%4d -+", &DebugWork::g_dw.sound.iSE_FileId, 4, {0}, 2, 0.0f, 0.0f, 0.0f, NULL, 0, NULL, NULL},
    {" I    <+1000 >    I", &DebugWork::g_dw.sound.iSE_FileId, 4, {0}, 0, 0.0f, 9999.0f, 1000.0f, NULL, 0, debugWinSeTestLineCall, NULL},
    {" I    < +100 >    I", &DebugWork::g_dw.sound.iSE_FileId, 4, {0}, 0, 0.0f, 9999.0f, 100.0f, NULL, 0, debugWinSeTestLineCall, NULL},
    {" I    <  +10 >    I", &DebugWork::g_dw.sound.iSE_FileId, 4, {0}, 0, 0.0f, 9999.0f, 10.0f, NULL, 0, debugWinSeTestLineCall, NULL},
    {" I    <   +1 >    I", &DebugWork::g_dw.sound.iSE_FileId, 4, {0}, 0, 0.0f, 9999.0f, 1.0f, NULL, 0, debugWinSeTestLineCall, NULL},
    {" +----------------+", &DebugWork::g_dw.sound.iSE_FileId, 4, {0}, 2, 0.0f, 0.0f, 0.0f, NULL, 0, NULL, NULL},
    {" +-  SE-ID : %3d -+", &DebugWork::g_dw.sound.iSE_SeId, 4, {0}, 2, 0.0f, 0.0f, 0.0f, NULL, 0, NULL, NULL},
    {" I    < +100 >    I", &DebugWork::g_dw.sound.iSE_SeId, 4, {0}, 0, 0.0f, 511.0f, 100.0f, NULL, 0, debugWinSeTestLineCall, NULL},
    {" I    <  +10 >    I", &DebugWork::g_dw.sound.iSE_SeId, 4, {0}, 0, 0.0f, 511.0f, 10.0f, NULL, 0, debugWinSeTestLineCall, NULL},
    {" I    <   +1 >    I", &DebugWork::g_dw.sound.iSE_SeId, 4, {0}, 0, 0.0f, 511.0f, 1.0f, NULL, 0, debugWinSeTestLineCall, NULL},
    {" +----------------+", &DebugWork::g_dw.sound.iSE_SeId, 4, {0}, 2, 0.0f, 0.0f, 0.0f, NULL, 0, NULL, NULL},
    {" I VOICE-MODE %3s I", &DebugWork::g_dw.sound.bVoice, 4, {0}, 0, 0.0f, 1.0f, 1.0f, szOffOn, 0, debugWinSeTestLineCall, NULL},
    {" I  \x82r\x82" "d\x83X\x83g\x83" "b\x83v  I", &CDebugWindow::none, 1, {0}, 0, 0.0f, 0.0f, 0.0f, NULL, 0, _stopSE, NULL},
    {" I  \x82r\x82" "d FADEOUT  I", &CDebugWindow::none, 1, {0}, 0, 0.0f, 0.0f, 0.0f, NULL, 0, _fadeoutSE, NULL},
    {NULL, NULL, 0, {0}, 0, 0.0f, 0.0f, 0.0f, NULL, 0, NULL, NULL},
};

DEBUG_WINDOW_TBL debugWinSeTest = {"SE TEST", -60, -32, 10, 15, 12, 0, 0, debugWinSeTestLine, NULL, NULL};

static DEBUG_WINDOW_LINE debugWinCollisionDispLine[15] = {
    {"\x82n\x82" "a\x82i\x93\x96\x82\xbd\x82\xe8\x95\\\x8e\xa6 %s", &CCollDisp::m_fgDisp, 4, {0}, 0, 0.0f, 1.0f, 1.0f, szOffOn, 0, NULL, NULL},
    {"\x83}\x83" "b\x83v\x93\x96\x82\xbd\x82\xe8\x95\\\x8e\xa6 %s", &CCollDisp::m_fgMapZoneDisp, 4, {0}, 0, 0.0f, 1.0f, 1.0f, szOffOn, 0, NULL, NULL},
    {"\xf9W\x83}\x83" "b\x83v\x93\x96\x82\xbd\x82\xe8\x90" "F\xf9T\x82q\xf9" "A %3d", &CCollDisp::m_bMapZoneColR, 2, {0}, 0, 0.0f, 255.0f, 1.0f, NULL, 0, NULL, NULL},
    {"              \xf9V\x82" "f\xf9" "A %3d", &CCollDisp::m_bMapZoneColG, 2, {0}, 0, 0.0f, 255.0f, 1.0f, NULL, 0, NULL, NULL},
    {"              \xf9U\x82" "a\xf9" "A %3d", &CCollDisp::m_bMapZoneColB, 2, {0}, 0, 0.0f, 255.0f, 1.0f, NULL, 0, NULL, NULL},
    {"\x82\xb5\x82\xa9\x82\xaf\x93\x96\x82\xbd\x82\xe8\x95\\\x8e\xa6 %s", &DebugWork::g_dw.collision.uiFlag, 4, {0}, 0, 0.0f, 1.0f, 1.0f, szOffOn, 1, NULL, NULL},
    {"\xf9S-----------------------", &CDebugWindow::none, 1, {0}, 2, 0.0f, 0.0f, 0.0f, NULL, 0, NULL, NULL},
    {"\x8b\x97\x97\xa3\x83`\x83" "F\x83" "b\x83N     %s", &DebugWork::g_dw.collision.uiFlag, 4, {0}, 0, 0.0f, 1.0f, 1.0f, szOnOff, 2, NULL, NULL},
    {"\x8a\xee\x8f\x80\x93_\xf9S\x81" "F\xf9" "A\xf9V%s\xf9" "A", &DebugWork::g_dw.collision.uiFlag, 4, {0}, 0, 0.0f, 1.0f, 1.0f, szCollDispRef, 4, NULL, NULL},
    {"\xf9S-----------------------", &CDebugWindow::none, 1, {0}, 2, 0.0f, 0.0f, 0.0f, NULL, 0, NULL, NULL},
    {" \xf9T\x82\xb5\x82\xa9\x82\xaf\x93\x96\x82\xbd\x82\xe8\x95\\\x8e\xa6\x82\xcc", &CDebugWindow::none, 1, {0}, 2, 0.0f, 0.0f, 0.0f, NULL, 0, NULL, NULL},
    {" \xf9T\x90\xdd\x92\xe8\x95\xcf\x8dX\x82\xcd\x81" "A", &CDebugWindow::none, 1, {0}, 2, 0.0f, 0.0f, 0.0f, NULL, 0, NULL, NULL},
    {" \xf9T\x83V\x81[\x83\x93\x95\xcf\x8dX\x82\xaa\x95K\x97v\x82\xc5\x82\xb7\x81" "B", &CDebugWindow::none, 1, {0}, 2, 0.0f, 0.0f, 0.0f, NULL, 0, NULL, NULL},
    {"\xf9S-----------------------", &CDebugWindow::none, 1, {0}, 2, 0.0f, 0.0f, 0.0f, NULL, 0, NULL, NULL},
    {NULL, NULL, 0, {0}, 0, 0.0f, 0.0f, 0.0f, NULL, 0, NULL, NULL},
};

static DEBUG_WINDOW_TBL debugWinCollisionDisp = {"\x93\x96\x82\xbd\x82\xe8\x94\xbb\x92\xe8\x95\\\x8e\xa6\x90\xdd\x92\xe8", -208, -160, 11, 15, 12, 0, 0, debugWinCollisionDispLine, NULL, NULL};

static void __setDebugWorkFocusBlur(INT i) {
    CBlurPrim* p = CDeFocus::getBlur(i);
    if (p) {
        DebugWork::g_dw.sys.m_iBlurKind = p->getKind();
        DebugWork::g_dw.sys.m_iBlurRate = p->getRate();
        DebugWork::g_dw.sys.m_fBlurLen = p->getDistance();
        UINT iColor = p->getColor();
        DebugWork::g_dw.sys.m_iBlurCR = iColor & 0xFF;
        DebugWork::g_dw.sys.m_iBlurCG = (iColor >> 8) & 0xFF;
        DebugWork::g_dw.sys.m_iBlurCB = (iColor >> 16) & 0xFF;
        DebugWork::g_dw.sys.m_iBlurCA = iColor >> 24;
        DebugWork::g_dw.sys.m_iBlurAlpha = p->getAlphaModeINT();
    }
}

static BOOL debugWinFocusTestLineChCall(CDebugWindow* pObj, INT iLineY, INT inParam) {
    if (inParam & 0x10) {
        __setDebugWorkFocusBlur(DebugWork::g_dw.sys.m_iBlurCh);
    }
    return TRUE;
}

static BOOL DebugAfterImageOn(CDebugWindow* pObj, INT inY, INT inParam) {
    if (inParam & 0x180) {
        INT kind;
        DebugWork::g_dw.sys.m_fgAfterImageTrg = 1;
        switch (DebugWork::g_dw.sys.m_fgAfterImage) {
        case 0:
            kind = 0;
            break;
        case 1:
            kind = 1;
            break;
        case 2:
            kind = 2;
            break;
        }
        CPackMan::setFrameEffect(kind, 0, 0, 0);
    } else {
        DebugWork::g_dw.sys.m_fgAfterImageTrg = 0;
    }
    return TRUE;
}

static INT debugWinFocusTestAllCall(CDebugWindow* pObj, INT inParam) {
    if (inParam & 1) {
        __setDebugWorkFocusBlur(DebugWork::g_dw.sys.m_iBlurCh);
        DebugWork::g_dw.sys.m_fDeFocusNear = 0.0f;
        DebugWork::g_dw.sys.m_fDeFocusFar = 0.0f;
    }
    if (inParam & 2) {
        DebugWork::g_dw.sys.m_iBlurKind = 0;
        DebugWork::g_dw.sys.m_fDeFocusNear = 0.0f;
        DebugWork::g_dw.sys.m_fDeFocusFar = 0.0f;
    }
    return TRUE;
}

static DEBUG_WINDOW_LINE debugWinFocusTestLine[31] = {
    {"---------\x83O\x83\x8c\x83" "A---------", &CDebugWindow::none, 1, {0}, 2, 0.0f, 0.0f, 0.0f, NULL, 0, NULL, NULL},
    {"\x83O\x83\x8c\x83" "A              %s", &DebugWork::g_dw.sys.m_iGlare, 1, {0}, 0, 0.0f, 2.0f, 1.0f, szDefOnOff, 0, NULL, NULL},
    {" ->ON:\x83O\x83\x8c\x83" "A\x82\xc2\x82\xe6\x82\xb3 %4.2f", &DebugWork::g_dw.sys.m_fGlareMaxPer, 4, {0}, 1, 0.0f, 2.0f, 0.01f, NULL, 0, NULL, NULL},
    {"------\x83" "f\x83t\x83H\x81[\x83J\x83X------", &CDebugWindow::none, 1, {0}, 2, 0.0f, 0.0f, 0.0f, NULL, 0, NULL, NULL},
    {"\x94\xed\x8e\xca\x8a" "E\x90[\x93x          %s", &DebugWork::g_dw.sys.m_iDeFocus, 1, {0}, 0, 0.0f, 1.0f, 1.0f, szOnOff, 0, NULL, NULL},
    {"\x94\xed\x8e\xca\x8a" "E\x90[\x93x\x82\xc9\x90" "F\x82\xc2\x82\xaf  %s", &DebugWork::g_dw.sys.m_iDeFocusCol, 1, {0}, 0, 0.0f, 1.0f, 1.0f, szOffOn, 0, NULL, NULL},
    {"Manual Near       %5.1f", &DebugWork::g_dw.sys.m_fDeFocusNear, 4, {0}, 1, 0.0f, 100.0f, 0.1f, NULL, 0, NULL, NULL},
    {"Manual Far        %5.1f", &DebugWork::g_dw.sys.m_fDeFocusFar, 4, {0}, 1, 0.0f, 999.0f, 1.0f, NULL, 0, NULL, NULL},
    {"-------\x83u\x83\x89\x81[\x90\xdd\x92\xe8-------", &CDebugWindow::none, 1, {0}, 2, 0.0f, 0.0f, 0.0f, NULL, 0, NULL, NULL},
    {"\x89\xe6\x96\xca\x83{\x83P\x83`\x83\x83\x83\x93\x83l\x83\x8b  %2d", &DebugWork::g_dw.sys.m_iBlurCh, 1, {0}, 0, 0.0f, 1.0f, 1.0f, NULL, 0, debugWinFocusTestLineChCall, NULL},
    {"\x89\xe6\x96\xca\x83{\x83P\x8e\xed\x95\xca        %2d", &DebugWork::g_dw.sys.m_iBlurKind, 1, {0}, 0, 0.0f, 6.0f, 1.0f, NULL, 0, NULL, NULL},
    {"\x89\xe6\x96\xca\x83{\x83P\x82\xe8\x82\xc2        %2d", &DebugWork::g_dw.sys.m_iBlurRate, 1, {0}, 0, 0.0f, 99.0f, 1.0f, NULL, 0, NULL, NULL},
    {"\x89\xe6\x96\xca\x83{\x83PFar\x8b\x97\x97\xa3   %5.1f", &DebugWork::g_dw.sys.m_fBlurLen, 4, {0}, 1, 0.0f, 999.0f, 0.1f, NULL, 0, NULL, NULL},
    {"\x89\xe6\x96\xca\x83{\x83P\x90" "F             ", &CDebugWindow::none, 1, {0}, 2, 0.0f, 0.0f, 0.0f, NULL, 0, NULL, NULL},
    {"        \xf9TR          %3d", &DebugWork::g_dw.sys.m_iBlurCR, 2, {0}, 0, 0.0f, 255.0f, 1.0f, NULL, 0, NULL, NULL},
    {"        \xf9VG          %3d", &DebugWork::g_dw.sys.m_iBlurCG, 2, {0}, 0, 0.0f, 255.0f, 1.0f, NULL, 0, NULL, NULL},
    {"        \xf9UB          %3d", &DebugWork::g_dw.sys.m_iBlurCB, 2, {0}, 0, 0.0f, 255.0f, 1.0f, NULL, 0, NULL, NULL},
    {"        \xf9WA          %3d", &DebugWork::g_dw.sys.m_iBlurCA, 2, {0}, 0, 0.0f, 255.0f, 1.0f, NULL, 0, NULL, NULL},
    {"+- ALPHA (A-B)xC>>7+D -+", &CDebugWindow::none, 1, {0}, 2, 0.0f, 0.0f, 0.0f, NULL, 0, NULL, NULL},
    {"|\x94\xbc\x93\xa7\x96\xbe A:0 B:1 C:0 D:1|", &CDebugWindow::none, 1, {0}, 2, 0.0f, 0.0f, 0.0f, NULL, 0, NULL, NULL},
    {"|\x89\xc1\x82\xb3\x82\xf1 A:0 B:2 C:0 D:1|", &CDebugWindow::none, 1, {0}, 2, 0.0f, 0.0f, 0.0f, NULL, 0, NULL, NULL},
    {"|\x8c\xb8\x82\xb3\x82\xf1 A:1 B:0 C:0 D:2|", &CDebugWindow::none, 1, {0}, 2, 0.0f, 0.0f, 0.0f, NULL, 0, NULL, NULL},
    {"+----------------------+", &CDebugWindow::none, 1, {0}, 2, 0.0f, 0.0f, 0.0f, NULL, 0, NULL, NULL},
    {"     A (0:S 1:D 2:0) %d", &DebugWork::g_dw.sys.m_iBlurAlpha, 4, {0}, 0, 0.0f, 2.0f, 1.0f, NULL, 3, NULL, NULL},
    {"     B (0:S 1:D 2:0) %d", &DebugWork::g_dw.sys.m_iBlurAlpha, 4, {0}, 0, 0.0f, 2.0f, 1.0f, NULL, 0xC, NULL, NULL},
    {"     C (0:S 1:D 2:F) %d", &DebugWork::g_dw.sys.m_iBlurAlpha, 4, {0}, 0, 0.0f, 2.0f, 1.0f, NULL, 0x30, NULL, NULL},
    {"     D (0:S 1:D 2:0) %d", &DebugWork::g_dw.sys.m_iBlurAlpha, 4, {0}, 0, 0.0f, 2.0f, 1.0f, NULL, 0xC0, NULL, NULL},
    {"     FIX           %3d", &DebugWork::g_dw.sys.m_iBlurAlpha, 4, {0}, 0, 0.0f, 255.0f, 1.0f, NULL, 0xFF0000, NULL, NULL},
    {"----\x95\\\x8e\xa6\x89\xe6\x96\xca\x91S\x91\xcc\x8c\xf8\x89\xca----", &CDebugWindow::none, 1, {0}, 2, 0.0f, 0.0f, 0.0f, NULL, 0, NULL, NULL},
    {"\x8e" "c\x91\x9c\x83G\x83t\x83" "F\x83N\x83g      %s", &DebugWork::g_dw.sys.m_fgAfterImage, 1, {0}, 0, 0.0f, 2.0f, 1.0f, szAfterEffect, 0, DebugAfterImageOn, NULL},
    {NULL, NULL, 0, {0}, 0, 0.0f, 0.0f, 0.0f, NULL, 0, NULL, NULL},
};

DEBUG_WINDOW_TBL debugWinFocusTest = {"\x83t\x83H\x81[\x83J\x83X\x83" "e\x83X\x83g", -208, -168, 12, 24, 12, 0, 0, debugWinFocusTestLine, NULL, debugWinFocusTestAllCall};

static void __initDebugWorkShadow(INT i) {
    DebugWork::g_dw.shadow.p.fDensityColor = CShadowModelMan::m_fCDensity;
    DebugWork::g_dw.shadow.p.fDensityAlpha = CShadowModelMan::m_fADensity;
    DebugWork::g_dw.shadow.b = DebugWork::g_dw.shadow.p;
}

static void __setDebugWorkShadow(INT i) {
    DEBUG_SHADOW_TEST1* p = i == 0 ? &DebugWork::g_dw.shadow.b : &DebugWork::g_dw.shadow.p;
    FLOAT* pf = &p->fDensityColor;
    CShadowModelMan::m_fCDensity = pf[0];
    CShadowModelMan::m_fADensity = pf[1];
}

static BOOL debugWinDebugWorkShadowLineAddSubCall(CDebugWindow* pObj, INT inY, INT inParam) {
    if (inParam & 0x80) {
        pObj->addValue(-1, NULL, NULL, &DebugWork::g_dw.shadow.fValuePlus, NULL);
    } else if (inParam & 0x100) {
        pObj->addValue(1, NULL, NULL, &DebugWork::g_dw.shadow.fValuePlus, NULL);
    }
    return TRUE;
}

static INT debugWinShadowTestAllCall(CDebugWindow* pObj, INT inParam) {
    if (inParam & 1) {
        DebugWork::g_dw.shadow.fValuePlus = 0.1f;
        __initDebugWorkShadow(0);
    } else if (inParam & 2) {
        __setDebugWorkShadow(0);
    } else {
        __setDebugWorkShadow(1);
    }
    return TRUE;
}

static DEBUG_WINDOW_LINE debugWinShadowTestLine[6] = {
    {"\x89" "e\x82\xcc\x90" "F\x82\xcc\x82\xb1\x82\xb3         %3.2f", &DebugWork::g_dw.shadow.p.fDensityColor, 4, {0}, 1, 0.0f, 1.0f, 0.0f, NULL, 0, debugWinDebugWorkShadowLineAddSubCall, NULL},
    {"\x89" "e\x82\xcc\x83" "A\x83\x8b\x83t\x83@\x82\xcc\x82\xb1\x82\xb3   %3.2f", &DebugWork::g_dw.shadow.p.fDensityAlpha, 4, {0}, 1, 0.0f, 1.0f, 0.0f, NULL, 0, debugWinDebugWorkShadowLineAddSubCall, NULL},
    {"--------------------------", &CDebugWindow::none, 1, {0}, 2, 0.0f, 0.0f, 0.0f, NULL, 0, NULL, NULL},
    {"Param\x95\xcf\x8dX\x97p\x91\x9d\x8c\xb8\x92l: %6.2f\xf9" "A", &DebugWork::g_dw.shadow.fValuePlus, 4, {0}, 0x11, 0.01f, 100.0f, 10.0f, NULL, 0, NULL, NULL},
    {"\x83G\x83\x8a\x83" "A\x95\\\x8e\xa6           %s", &DebugWork::g_dw.shadow.iDispArea, 1, {0}, 0, 0.0f, 1.0f, 1.0f, szOffOn, 0, NULL, NULL},
    {NULL, NULL, 0, {0}, 0, 0.0f, 0.0f, 0.0f, NULL, 0, NULL, NULL},
};

DEBUG_WINDOW_TBL debugWinShadowTest = {"\x89" "e\x83" "e\x83X\x83g", -208, -168, 13, 24, 12, 0, 0, debugWinShadowTestLine, NULL, debugWinShadowTestAllCall};

namespace DWTEvent {

static char* szPlayMode[2] = {"\x83\\\x83\x89", "\x83\x8a\x83N"};

static char* szDifficultMode[4] = {"\x83r\x83M\x83i\x81[", "\x83X\x83^\x83\x93\x83_\x81[\x83h", "\x83v\x83\x89\x83" "E\x83h", "\x83G\x83N\x83X\x83g\x83\x8a\x81[\x83\x80"};

static char* szSoundMode[3] = {"\x83X\x83" "e\x83\x8c\x83I", "\x83v\x83\x8d\x83\x8d\x83W\x83" "b\x83N\x87U", "\x83\x82\x83m\x83\x89\x83\x8b"};

static char* szMenuKind[8] = {"NotOpen", "Camp", "RoomCreation_1", "RoomCreation_2", "RoomCreation_EV", "WorldSelect", "MogShop", "SaveMenu"};

static char* szMiniGameState[6] = {"\x82\xb5\x82\xe5\x82\xab\x82\xb6\x82\xe5\x82\xa4\x82\xbd\x82\xa2", "\x82P\x89\xf1\x96\xda\x82\xbf\x82\xe5\x82\xa4\x82\xb9\x82\xf1", "\x82P\x89\xf1\x96\xda\x83N\x83\x8a\x83" "A\x82\xb5\x82\xbd", "\x82Q\x89\xf1\x96\xda\x82\xbf\x82\xe5\x82\xa4\x82\xb9\x82\xf1", "\x82Q\x89\xf1\x96\xda\x83N\x83\x8a\x83" "A\x82\xb5\x82\xbd", "\x8d" "D\x82\xab\x82\xc9\x97V\x82\xd7"};

static char* szMiniGamePlayState[4] = {"\x82\xb5\x82\xe5\x82\xab\x82\xb6\x82\xe5\x82\xa4\x82\xbd\x82\xa2", "\x83Q\x81[\x83\x80\x8aJ\x8en", "\x83Q\x81[\x83\x80\x92\x86", "\x83Q\x81[\x83\x80\x8fI\x97\xb9"};

static char* szMiniGameKind[5] = {"\x96\xec\x8d\xd8\x83p\x83j\x83" "b\x83N", "\x83o\x83\x8b\x81[\x83\x93\x83O\x83\x89\x83" "C\x83_\x81[", "\x83" "e\x83" "B\x83K\x81[\x83W\x83\x83\x83\x93\x83v", "\x83\x8f\x83" "C\x83\x8b\x83h\x83_\x83" "C\x83u", "\x82\xcd\x82\xbf\x91\xde\x8e\xa1"};

static char* szStockKindName[77] = {"-----\x82O", "-----\x82P", "-----\x82Q", "-----\x82R", "\x83X\x83\x89\x83" "C\x83h\x83_\x83" "b\x83V\x83\x85", "\x83t\x83@\x83" "C\x83i\x83\x8b\x83u\x83\x8c\x83" "C\x83N", "\x83X\x83^\x83\x93\x83" "C\x83\x93\x83p\x83N\x83g", "\x83U\x83\x93\x83" "e\x83" "c\x83P\x83\x93", "\x83X\x83g\x83\x89\x83" "C\x83N\x83\x8c\x83" "C\x83h", "\x83\\\x83j\x83" "b\x83N\x83\x8c\x83" "C\x83\x94", "\x83\x89\x83X\x83g\x83" "A\x83\x8b\x83J\x83i\x83\x80", "\x83\x89\x83O\x83i\x83\x8d\x83N", "\x83\x8a\x83j\x83" "e\x83" "B\x83\x8a\x83~\x83" "b\x83g", "\x83t\x83@\x83" "C\x83\x89", "\x83u\x83\x8a\x83U\x83\x89", "\x83T\x83\x93\x83_\x83\x89", "\x83P\x83" "A\x83\x8b\x83\x89", "\x83O\x83\x89\x83r\x83\x89", "\x83X\x83g\x83v\x83\x89", "\x83G\x83" "A\x83\x8d\x83\x89", "\x83t\x83@\x83" "C\x83K", "\x83u\x83\x8a\x83U\x83K", "\x83T\x83\x93\x83_\x83K", "\x83P\x83" "A\x83\x8b\x83K", "\x83O\x83\x89\x83r\x83K", "\x83X\x83g\x83v\x83K", "\x83G\x83" "A\x83\x8d\x83K", "\x83t\x83@\x83" "C\x83" "A\x83\x8c\x83" "C\x83h", "\x83u\x83\x8a\x83U\x83h\x83\x8c\x83" "C\x83h", "\x83T\x83\x93\x83_\x81[\x83\x8c\x83" "C\x83h", "\x83\x8a\x83t\x83\x8c\x83N\x83g\x83\x8c\x83" "C\x83h", "\x83W\x83\x83\x83" "b\x83W\x83\x81\x83\x93\x83g", "\x83t\x83@\x83" "C\x83K\x83o\x81[\x83X\x83g", "\x83\x8c\x83" "C\x83W\x83\x93\x83O\x83X\x83g\x81[\x83\x80", "\x83\x81\x83K\x83t\x83\x8c\x83" "A", "\x83t\x83\x8a\x81[\x83Y", "\x83z\x81[\x83~\x83\x93\x83O\x83u\x83\x8a\x83U\x83\x89\x09", "\x83" "A\x83N\x83" "A\x83X\x83v\x83\x89\x83" "b\x83V\x83\x85", "\x83}\x83O\x83l\x83X\x83p\x83" "C\x83\x89\x83\x8b", "\x83\x8a\x81[\x83T\x83\x8b\x83t\x83\x8c\x81[\x83\x80", "\x83V\x83\x87\x83" "b\x83N\x83" "C\x83\x93\x83p\x83N\x83g", "\x83g\x83\x8b\x83l\x83h", "\x83N\x83G\x83" "C\x83N", "\x83" "f\x83W\x83\x87\x83l\x81[\x83^\x81[", "\x83" "f\x83W\x83\x87\x83\x93", "\x83o\x83" "C\x83\x93\x83h", "\x83R\x83\x93\x83t\x83\x85", "\x83" "e\x83\x89\x81[", "\x83V\x83\x93\x83N\x83\x8d", "\x83~\x83\x89\x83N\x83\x8b\x83M\x83t\x83g", "\x83" "e\x83\x8c\x83|", "\x83z\x81[\x83\x8a\x81[", "\x83v\x83\x89\x83" "E\x83h\x83\x8d\x83" "A", "\x83X\x83v\x83\x89\x83" "b\x83V\x83\x85", "\x83p\x83\x89\x83_\x83" "C\x83X", "\x83p\x83\x89\x83_\x83" "C\x83X\x83X\x83^\x83\x93\x83v", "\x83t\x83\x8c\x83" "A\x83u\x83\x8c\x83X", "\x83V\x83\x87\x81[\x83^\x83" "C\x83\x80", "\x83V\x83\x87\x81[\x83^\x83" "C\x83\x80", "\x83" "c\x83" "C\x83\x93\x83N\x83\x8b", "\x8b\xa5\x8e" "a\x82\xe8", "\x92\xb4\x8b\x86\x95\x90\x90_\x94" "e\x8e" "a", "\x8b\xa5\x8e" "a\x82\xe8\x81" "E\x89\xfc", "\x83}\x83W\x83" "b\x83N", "\x83X\x83^\x81[\x83_\x83X\x83g\x83u\x83\x8a\x83" "b\x83" "c", "\x83O\x81[\x83t\x83" "B\x81[\x83g\x83\x8b\x83l\x83h", "\x83O\x81[\x83t\x83" "B\x81[\x83X\x83}\x83" "b\x83V\x83\x85", "\x83\x8f\x83" "C\x83\x8b\x83h\x83N\x83\x89\x83" "b\x83V\x83\x83\x81[", "\x83X\x83g\x81[\x83\x80\x83\x8a\x81[\x83p\x81[", "\x83T\x83v\x83\x89\x83" "C\x83Y", "\x83X\x83p\x83" "C\x83\x89\x83\x8b\x83" "E\x83" "F\x83" "C\x83u", "\x83X\x83p\x83" "C\x83\x89\x83\x8b\x83" "E\x83" "F\x83" "C\x83u", "\x83X\x83\x89\x83X\x83g\x83\x89\x83" "b\x83V\x83\x85", "\x83t\x83\x85\x83\x8a\x83" "A\x83X\x83{\x83\x8c\x81[", "\x83t\x83\x85\x83\x8a\x83" "A\x83X\x83{\x83\x8c\x81[", "\x83\x89\x83" "b\x83L\x81[\x83o\x83" "E\x83\x93\x83" "e\x83" "B", "\x83\x89\x83" "b\x83L\x81[\x83o\x83" "E\x83\x93\x83" "e\x83" "B"};

static BOOL DebugSetNpcIDWindow(CDebugWindow* pObj, INT inY, INT inParam) {
    if (inParam & 0x20) {
        CActorDebug* p;
        DebugWork::g_dw.event.iNpcID = inY;
        if (DebugWork::g_dw.event.pClass) {
            p = (CActorDebug*)DebugWork::g_dw.event.pClass;
            delete p;
        }
        p = new CActorDebug(CTaskMan::getSysTask(1));
        DebugWork::g_dw.event.pClass = (INT*)p;
        DebugWork::g_dw.event.iNpcCoordDisp = 1;
    }
    return TRUE;
}

static INT debugWinNPCListAllCall(CDebugWindow* pObj, INT inParam) {
    if (inParam & 1) {
        CTask* pA = CTaskMan::getSysTask(0x1A);
        INT ret = 0;
        if (pA && ((CAtel*)pA)->getVSM()) {
            ret = ((CAtel*)pA)->getVSM()->getActorNum();
        }
        return ret;
    }
    return TRUE;
}

static INT debugWinNPCListDispCall(LPSTR pStr, INT iLineY, const void* pValue) {
    CTask* pA = CTaskMan::getSysTask(0x1A);
    if (pA && ((CAtel*)pA)->getVSM()) {
        sprintf(pStr, " %s", ((CAtel*)pA)->getVSM()->getAvatarName(iLineY));
    }
    return TRUE;
}

static DEBUG_WINDOW_LINE debugWinNPCListLine[2] = {
    {"", &CDebugWindow::none, 1, {0}, 0, 0.0f, 0.0f, 0.0f, NULL, 0, DWTEvent::DebugSetNpcIDWindow, NULL},
    {NULL, NULL, 0, {0}, 0, 0.0f, 0.0f, 0.0f, NULL, 0, NULL, NULL},
};

static DEBUG_WINDOW_TBL debugWinNpcList = {"ACTOR\x83\x8a\x83X\x83g", -208, -136, 10, 20, 14, 0, 8, DWTEvent::debugWinNPCListLine, DWTEvent::debugWinNPCListDispCall, DWTEvent::debugWinNPCListAllCall};

static BOOL DebugCreateNpcWindow(CDebugWindow* pObj, INT inY, INT inParam) {
    CTask* pA = CTaskMan::getSysTask(0x1A);
    if (pA && ((CAtel*)pA)->getVSM() && (inParam & 0x180)) {
        if (DebugWork::g_dw.event.iNpcCoordDisp == 1) {
            CActorDebug* p = new CActorDebug(CTaskMan::getSysTask(1));
            DebugWork::g_dw.event.pClass = (INT*)p;
        }
    }
    return TRUE;
}

static BOOL DebugCreateEventLineDisp(CDebugWindow* pObj, INT inY, INT inParam) {
    CTask* pA = CTaskMan::getSysTask(0x1A);
    if (pA && ((CAtel*)pA)->getVSM() && (inParam & 0x180)) {
        if (DebugWork::g_dw.event.iEventline == 1) {
            CEventLineDisp* p = new CEventLineDisp(CTaskMan::getSysTask(1));
            DebugWork::g_dw.event.pEventlineClass = (INT*)p;
        } else {
            CEventLineDisp* p = (CEventLineDisp*)DebugWork::g_dw.event.pEventlineClass;
            if (p) {
                p->out();
            }
            DebugWork::g_dw.event.pEventlineClass = NULL;
        }
    }
    return TRUE;
}

static INT DebugAtelWorkDef(CDebugWindow* pObj, INT inParam) {
    DebugWork::g_dw.atelwork.iWorldIntro = CGameWork::getInt(0, 0xC1);
    DebugWork::g_dw.atelwork.iTutorialEnd = CGameWork::getInt(0, 0xC2);
    DebugWork::g_dw.atelwork.iPossibleCard = CGameWork::getInt(0, 0x21);
    DebugWork::g_dw.atelwork.iPossibleFriend = CGameWork::getInt(0, 0x22);
    DebugWork::g_dw.atelwork.iPoohMiniGameIntro = CGameWork::getInt(0, 0x23);
    DebugWork::g_dw.atelwork.iPcStanderdSkill = CGameWork::getInt(0, 0x24);
    DebugWork::g_dw.atelwork.iEnSoraEventStep = CGameWork::getInt(0, 0x25);
    DebugWork::g_dw.atelwork.iEnRikuEventStep = CGameWork::getInt(0, 0x26);
    DebugWork::g_dw.atelwork.iSoraJiminyStep = CGameWork::getInt(0, 0x33);
    DebugWork::g_dw.atelwork.iRikuJiminyStep = CGameWork::getInt(0, 0x34);
    DebugWork::g_dw.atelwork.iKH2CardGet = CGameWork::getInt(0, 0x2D);
    DebugWork::g_dw.atelwork.iPoohHiScoreOwl = CGameWork::getInt(0, 0x2E);
    DebugWork::g_dw.atelwork.iPoohHiScoreRoo = CGameWork::getInt(0, 0x2F);
    DebugWork::g_dw.atelwork.iPoohHiScoreEeyore = CGameWork::getInt(0, 0x30);
    DebugWork::g_dw.atelwork.iPoohHiScoreTigger = CGameWork::getInt(0, 0x31);
    DebugWork::g_dw.atelwork.iPoohHiScoreRabbit = CGameWork::getInt(0, 0x32);
    DebugWork::g_dw.atelwork.iDistinctionClear = CGameWork::getFlag(0, 0x83);
    DebugWork::g_dw.atelwork.iPlayMode = CGameWork::getByte(0, 0x60);
    DebugWork::g_dw.atelwork.iPlayDifficultMode = CGameWork::getByte(0, 0x61);
    DebugWork::g_dw.atelwork.iSoundMode = CGameWork::getByte(0, 0x62);
    DebugWork::g_dw.atelwork.iVibrat = CGameWork::getByte(0, 0x63);
    DebugWork::g_dw.atelwork.iPoohGamePiglet = CGameWork::getByte(0, 0x3);
    DebugWork::g_dw.atelwork.iPoohGameOwl = CGameWork::getByte(0, 0x4);
    DebugWork::g_dw.atelwork.iPoohGameRoo = CGameWork::getByte(0, 0x5);
    DebugWork::g_dw.atelwork.iPoohGameEeyore = CGameWork::getByte(0, 0x6);
    DebugWork::g_dw.atelwork.iPoohGameTigger = CGameWork::getByte(0, 0x7);
    DebugWork::g_dw.atelwork.iPoohGameRabbit = CGameWork::getByte(0, 0x8);
    DebugWork::g_dw.atelwork.iPoohClearIntoroPiglet = CGameWork::getFlag(0, 0xA);
    DebugWork::g_dw.atelwork.iPoohClearIntoroOwl = CGameWork::getFlag(0, 0xB);
    DebugWork::g_dw.atelwork.iPoohClearIntoroRoo = CGameWork::getFlag(0, 0xC);
    DebugWork::g_dw.atelwork.iPoohClearIntoroEeyore = CGameWork::getFlag(0, 0xD);
    DebugWork::g_dw.atelwork.iPoohClearIntoroTigger = CGameWork::getFlag(0, 0xE);
    DebugWork::g_dw.atelwork.iPoohClearIntoroRabbit = CGameWork::getFlag(0, 0xF);
    DebugWork::g_dw.atelwork.iPoohClearIntoroResetPiglet = CGameWork::getFlag(0, 0x13);
    DebugWork::g_dw.atelwork.iPoohClearIntoroResetOwl = CGameWork::getFlag(0, 0x14);
    DebugWork::g_dw.atelwork.iPoohClearIntoroResetRoo = CGameWork::getFlag(0, 0x15);
    DebugWork::g_dw.atelwork.iPoohClearIntoroResetEeyore = CGameWork::getFlag(0, 0x16);
    DebugWork::g_dw.atelwork.iPoohClearIntoroResetTigger = CGameWork::getFlag(0, 0x17);
    DebugWork::g_dw.atelwork.iPoohClearIntoroRabbit = CGameWork::getFlag(0, 0x18);
    DebugWork::g_dw.atelwork.iGameKH2clear = CGameWork::getFlag(0, 0x11);
    DebugWork::g_dw.atelwork.iMahluciaKilled = CGameWork::getFlag(0, 0x82);
    DebugWork::g_dw.atelwork.iRikuClear = CGameWork::getFlag(0, 0x12);
    DebugWork::g_dw.atelwork.iSoraClear = CGameWork::getFlag(0, 0x9);
    DebugWork::g_dw.atelwork.iSoraMovie0 = CGameWork::getInt(0, 0x27);
    DebugWork::g_dw.atelwork.iSoraMovie1 = CGameWork::getInt(0, 0x28);
    DebugWork::g_dw.atelwork.iSoraMovie2 = CGameWork::getInt(0, 0x29);
    DebugWork::g_dw.atelwork.iRikuMovie0 = CGameWork::getInt(0, 0x2A);
    DebugWork::g_dw.atelwork.iRikuMovie1 = CGameWork::getInt(0, 0x2B);
    DebugWork::g_dw.atelwork.iRikuMovie2 = CGameWork::getInt(0, 0x2C);
    DebugWork::g_dw.atelwork.iBossEventStep = CGameWork::getInt(1, 0xE1);
    DebugWork::g_dw.atelwork.iCockpitDisp = CGameWork::getInt(1, 0xE2);
    DebugWork::g_dw.atelwork.iKindMenu = CGameWork::getByte(1, 0x140);
    DebugWork::g_dw.atelwork.iPoohMinigameState = CGameWork::getByte(1, 0x141);
    DebugWork::g_dw.atelwork.iPoohMinigameKind = CGameWork::getByte(1, 0x142);
    DebugWork::g_dw.atelwork.iPlayerDispOff = CGameWork::getFlag(1, 0x0);
    DebugWork::g_dw.atelwork.iBossKilled = CGameWork::getFlag(1, 0x2);
    DebugWork::g_dw.atelwork.iNotEventSkip = CGameWork::getFlag(1, 0x3);
    DebugWork::g_dw.atelwork.iIsAtelEvent = CGameWork::getFlag(1, 0x4);
    DebugWork::g_dw.atelwork.iNotPcControl = CGameWork::getFlag(1, 0x5);
    DebugWork::g_dw.atelwork.iNotPause = CGameWork::getFlag(1, 0x6);
    DebugWork::g_dw.atelwork.iIsTutorial = CGameWork::getFlag(1, 0x7);
    DebugWork::g_dw.atelwork.iNotRoomJump = CGameWork::getFlag(1, 0x20);
    DebugWork::g_dw.atelwork.iNotRoomCreation = CGameWork::getFlag(1, 0x24);
    DebugWork::g_dw.atelwork.iNotHartBtl = CGameWork::getFlag(1, 0x25);
    DebugWork::g_dw.atelwork.iIsBtlTutorial = CGameWork::getFlag(1, 0x2C);
    DebugWork::g_dw.atelwork.iIsWorldIntro = CGameWork::getFlag(1, 0x37);
    return TRUE;
}

static BOOL debugBtlReportCallBackParam(CDebugWindow* pObj, INT inY, INT inParam) {
    if (inParam & 0x180) {
        switch (inY) {
        case 4:
            CGameWork::setInt(0, 0xB3, DebugWork::g_dw.atelwork.KillEnemyCount);
            break;
        case 6:
            CGameWork::setInt(0, 0x3C, DebugWork::g_dw.atelwork.CardBreakCount);
            break;
        case 7:
            CGameWork::setInt(0, 0x3D, DebugWork::g_dw.atelwork.StockKind);
            break;
        case 8:
            CGameWork::setInt(0, 0x3E, DebugWork::g_dw.atelwork.StockCount);
            break;
        case 15:
            CGameWork::setInt(0, 0x3F, DebugWork::g_dw.atelwork.SpeedCount);
            break;
        case 16:
            CGameWork::setInt(0, 0xB3, DebugWork::g_dw.atelwork.KillEnemyCount);
            break;
        case 18:
            CGameWork::setInt(0, 0x41, DebugWork::g_dw.atelwork.DarkCount);
            break;
        }
    }
    return TRUE;
}

static INT debugBtlReportCallBack(CDebugWindow* pObj, INT inParam) {
    DebugWork::g_dw.atelwork.BtlMemo = CJiminyMemoFlag::getContentOpenPercentage(CGameWork::getByte(0, 0x60));
    DebugWork::g_dw.atelwork.KillEnemyCount = CGameWork::getInt(0, 0xB3);
    DebugWork::g_dw.atelwork.KeysTresur = 0;
    for (INT i = 0; i < 16; i++) {
        DebugWork::g_dw.atelwork.KeysTresur += CGameWork::progMCW()._KeyRoomsTresureBoxCounter[i];
    }
    DebugWork::g_dw.atelwork.CardBreakCount = CGameWork::getInt(0, 0x3C);
    DebugWork::g_dw.atelwork.StockKind = CGameWork::getInt(0, 0x3D);
    DebugWork::g_dw.atelwork.StockCount = CGameWork::getInt(0, 0x3E);
    UINT nTime = CMainApp::getPlayTime();
    DebugWork::g_dw.atelwork.Hour = nTime / 3600;
    DebugWork::g_dw.atelwork.minit = (nTime % 3600) / 60;
    if (DebugWork::g_dw.atelwork.Hour >= 1000) {
        DebugWork::g_dw.atelwork.Hour = 999;
        DebugWork::g_dw.atelwork.minit = 59;
    }
    INT nWin = CGameWork::getInt(0, 0x40);
    DebugWork::g_dw.atelwork.DuelWin = *(FLOAT*)&nWin * 100.0f;
    DebugWork::g_dw.atelwork.DarkCount = CGameWork::getInt(0, 0x41);
    DebugWork::g_dw.atelwork.SpeedCount = CGameWork::getInt(0, 0x3F);
    return TRUE;
}

static DEBUG_WINDOW_LINE debugBtlReportLine[22] = {
    {"\xf9S----------\xf9V\x83\\\x83\x89\xf9S---------", &CDebugWindow::none, 1, {0}, 2, 0.0f, 0.0f, 0.0f, NULL, 0, NULL, NULL},
    {"\x83Q\x81[\x83\x80\x83\x8c\x83x\x83\x8b       \x81@:%s", &DebugWork::g_dw.atelwork.iPlayDifficultMode, 1, {0}, 0, 0.0f, 0.0f, 0.0f, &szDifficultMode[DebugWork::g_dw.atelwork.iPlayDifficultMode], 0, NULL, NULL},
    {"\x83W\x83~\x83j\x81[\x92" "B\x90\xac\x97\xa6     \x81@:%d", &DebugWork::g_dw.atelwork.BtlMemo, 4, {0}, 0, 0.0f, 1.0f, 0.0f, NULL, 0, NULL, NULL},
    {"\x83R\x83\x8c\x83N\x83^\x81[\x83Y\x83\x89\x83\x93\x83N \x81@:--", &CDebugWindow::none, 1, {0}, 0, 0.0f, 1.0f, 0.0f, NULL, 0, NULL, NULL},
    {"\x93|\x82\xb5\x82\xbd\x93G\x82\xcc\x90\x94\x81@\x81@\x81@ \x81@:%d", &DebugWork::g_dw.atelwork.KillEnemyCount, 4, {0}, 0, 0.0f, 999999.0f, 1000.0f, NULL, 0, DWTEvent::debugBtlReportCallBackParam, NULL},
    {"\x96\xa2\x92m\x82\xc8\x82\xe9\x95\xf3\x82\xcc\x90\x94     \x81@:%d/24", &DebugWork::g_dw.atelwork.KeysTresur, 4, {0}, 0, 0.0f, 1.0f, 0.0f, NULL, 0, NULL, NULL},
    {"\x83J\x81[\x83h\x83u\x83\x8c\x83" "C\x83N\x89\xf1\x90\x94 \x81@:%d", &DebugWork::g_dw.atelwork.CardBreakCount, 4, {0}, 0, 0.0f, 99999.0f, 100.0f, NULL, 0, DWTEvent::debugBtlReportCallBackParam, NULL},
    {"\x8d\xc5\x82\xe0\x8eg\x82\xc1\x82\xbd\x83X\x83g\x83" "b\x83N\x8bZ :%s", &DebugWork::g_dw.atelwork.StockKind, 4, {0}, 0, 0.0f, 78.0f, 1.0f, &szStockKindName[DebugWork::g_dw.atelwork.StockKind], 0, DWTEvent::debugBtlReportCallBackParam, NULL},
    {"        \x81@\x81@\x81@\x81@\x89\xf1\x90\x94 :%d", &DebugWork::g_dw.atelwork.StockCount, 4, {0}, 0, 0.0f, 99999.0f, 100.0f, NULL, 0, DWTEvent::debugBtlReportCallBackParam, NULL},
    {"\x83N\x83\x8a\x83" "A\x8e\x9e\x8a\xd4\x81@\x81@\x81@     :%d", &DebugWork::g_dw.atelwork.Hour, 4, {0}, 0, 0.0f, 1.0f, 0.0f, NULL, 0, NULL, NULL},
    {"\x81@\x81@  \x81@\x95\xaa\x81@\x81@\x81@\x81@   :%d", &DebugWork::g_dw.atelwork.minit, 4, {0}, 0, 0.0f, 1.0f, 0.0f, NULL, 0, NULL, NULL},
    {"\xf9S----------\xf9V\x83\x8a\x83N\xf9S---------", &CDebugWindow::none, 1, {0}, 2, 0.0f, 0.0f, 0.0f, NULL, 0, NULL, NULL},
    {"\x83Q\x81[\x83\x80\x83\x8c\x83x\x83\x8b\x81@\x81@\x81@\x81@ :%s", &DebugWork::g_dw.atelwork.iPlayDifficultMode, 1, {0}, 0, 0.0f, 0.0f, 0.0f, &szDifficultMode[DebugWork::g_dw.atelwork.iPlayDifficultMode], 0, NULL, NULL},
    {"\x82" "c\x83\x8c\x83|\x81[\x83g\x92" "B\x90\xac\x97\xa6     :%d", &DebugWork::g_dw.atelwork.BtlMemo, 4, {0}, 0, 0.0f, 1.0f, 0.0f, NULL, 0, NULL, NULL},
    {"\x83R\x83\x8c\x83N\x83^\x81[\x83Y\x83\x89\x83\x93\x83N   :--", &CDebugWindow::none, 1, {0}, 0, 0.0f, 1.0f, 0.0f, NULL, 0, NULL, NULL},
    {"\x83X\x83s\x81[\x83h\x83u\x83\x8c\x83" "C\x83N     :%d", &DebugWork::g_dw.atelwork.SpeedCount, 4, {0}, 0, 0.0f, 99999.0f, 100.0f, NULL, 0, DWTEvent::debugBtlReportCallBackParam, NULL},
    {"\x93|\x82\xb5\x82\xbd\x93G\x82\xcc\x90\x94\x81@\x81@     :%d", &DebugWork::g_dw.atelwork.KillEnemyCount, 4, {0}, 0, 0.0f, 99999.0f, 100.0f, NULL, 0, DWTEvent::debugBtlReportCallBackParam, NULL},
    {"\x83" "f\x83\x85\x83G\x83\x8b\x82\xcc\x8f\x9f\x97\xa6       :%5.2f", &DebugWork::g_dw.atelwork.DuelWin, 4, {0}, 1, 0.0f, 0.0f, 0.0f, NULL, 0, NULL, NULL},
    {"\x82" "c\x83\x82\x81[\x83h\x94\xad\x93\xae\x89\xf1\x90\x94     :%d", &DebugWork::g_dw.atelwork.DarkCount, 4, {0}, 0, 0.0f, 99999.0f, 100.0f, NULL, 0, DWTEvent::debugBtlReportCallBackParam, NULL},
    {"\x83N\x83\x8a\x83" "A\x8e\x9e\x8a\xd4\x81@\x81@\x81@     :%d", &DebugWork::g_dw.atelwork.Hour, 4, {0}, 0, 0.0f, 1.0f, 0.0f, NULL, 0, NULL, NULL},
    {"\x81@\x81@  \x81@\x95\xaa\x81@\x81@\x81@\x81@   :%d", &DebugWork::g_dw.atelwork.minit, 4, {0}, 0, 0.0f, 1.0f, 0.0f, NULL, 0, NULL, NULL},
    {NULL, NULL, 0, {0}, 0, 0.0f, 0.0f, 0.0f, 0, 0, NULL, NULL},
};

static DEBUG_WINDOW_TBL debugBtlReport = {"\x83o\x83g\x83\x8b\x83\x8c\x83|\x81[\x83g", -106, -(SCREEN_HEIGHT >> 1) + 150, 20, 29, 12, 0, 0, DWTEvent::debugBtlReportLine, NULL, DWTEvent::debugBtlReportCallBack};

static INT debugKeyTresurCallBack(CDebugWindow* pObj, INT inParam) {
    DebugWork::g_dw.atelwork.TreTr = CGameWork::progMCW()._KeyRoomsTresureBoxCounter[1];
    DebugWork::g_dw.atelwork.TreAg = CGameWork::progMCW()._KeyRoomsTresureBoxCounter[2];
    DebugWork::g_dw.atelwork.TreOl = CGameWork::progMCW()._KeyRoomsTresureBoxCounter[3];
    DebugWork::g_dw.atelwork.TreWo = CGameWork::progMCW()._KeyRoomsTresureBoxCounter[4];
    DebugWork::g_dw.atelwork.TreMo = CGameWork::progMCW()._KeyRoomsTresureBoxCounter[5];
    DebugWork::g_dw.atelwork.TreHa = CGameWork::progMCW()._KeyRoomsTresureBoxCounter[6];
    DebugWork::g_dw.atelwork.TreAt = CGameWork::progMCW()._KeyRoomsTresureBoxCounter[7];
    DebugWork::g_dw.atelwork.TreNe = CGameWork::progMCW()._KeyRoomsTresureBoxCounter[8];
    DebugWork::g_dw.atelwork.TreHo = CGameWork::progMCW()._KeyRoomsTresureBoxCounter[9];
    DebugWork::g_dw.atelwork.TrePo = CGameWork::progMCW()._KeyRoomsTresureBoxCounter[10];
    DebugWork::g_dw.atelwork.TreTw = CGameWork::progMCW()._KeyRoomsTresureBoxCounter[11];
    DebugWork::g_dw.atelwork.TreDe = CGameWork::progMCW()._KeyRoomsTresureBoxCounter[12];
    DebugWork::g_dw.atelwork.TreCa = CGameWork::progMCW()._KeyRoomsTresureBoxCounter[13];
    return TRUE;
}

static DEBUG_WINDOW_LINE debugKeyTresurLine[14] = {
    {"\x83g\x83\x89\x83\x94\x83@\x81[\x83X       :%d", &DebugWork::g_dw.atelwork.TreTr, 1, {0}, 0, 0.0f, 1.0f, 0.0f, NULL, 0, NULL, NULL},
    {"\x83" "A\x83O\x83\x89\x83o           :%d", &DebugWork::g_dw.atelwork.TreAg, 1, {0}, 0, 0.0f, 1.0f, 0.0f, NULL, 0, NULL, NULL},
    {"\x83I\x83\x8a\x83\x93\x83|\x83X         :%d", &DebugWork::g_dw.atelwork.TreOl, 1, {0}, 0, 0.0f, 1.0f, 0.0f, NULL, 0, NULL, NULL},
    {"\x83\x8f\x83\x93\x83_\x81[\x83\x89\x83\x93\x83h     :%d", &DebugWork::g_dw.atelwork.TreWo, 1, {0}, 0, 0.0f, 1.0f, 0.0f, NULL, 0, NULL, NULL},
    {"\x83\x82\x83\x93\x83X\x83g\x83\x8d         :%d", &DebugWork::g_dw.atelwork.TreMo, 1, {0}, 0, 0.0f, 1.0f, 0.0f, NULL, 0, NULL, NULL},
    {"\x83n\x83\x8d\x83" "E\x83" "B\x83\x93         :%d", &DebugWork::g_dw.atelwork.TreHa, 1, {0}, 0, 0.0f, 1.0f, 0.0f, NULL, 0, NULL, NULL},
    {"\x83" "A\x83g\x83\x89\x83\x93\x83" "e\x83" "B\x83J     :%d", &DebugWork::g_dw.atelwork.TreAt, 1, {0}, 0, 0.0f, 1.0f, 0.0f, NULL, 0, NULL, NULL},
    {"\x83l\x83o\x81[\x83\x89\x83\x93\x83h       :%d", &DebugWork::g_dw.atelwork.TreNe, 1, {0}, 0, 0.0f, 1.0f, 0.0f, NULL, 0, NULL, NULL},
    {"\x83z\x83\x8d\x83" "E\x83o\x83X\x83" "e\x83" "B\x83I\x83\x93 :%d", &DebugWork::g_dw.atelwork.TreHo, 1, {0}, 0, 0.0f, 1.0f, 0.0f, NULL, 0, NULL, NULL},
    {"100\x83G\x81[\x83J\x81[        :%d", &DebugWork::g_dw.atelwork.TrePo, 1, {0}, 0, 0.0f, 1.0f, 0.0f, NULL, 0, NULL, NULL},
    {"\x83g\x83\x8f\x83" "C\x83\x89\x83" "C\x83g       :%d", &DebugWork::g_dw.atelwork.TreTw, 1, {0}, 0, 0.0f, 1.0f, 0.0f, NULL, 0, NULL, NULL},
    {"\x83" "f\x83X\x83" "e\x83j\x81[         :%d", &DebugWork::g_dw.atelwork.TreDe, 1, {0}, 0, 0.0f, 1.0f, 0.0f, NULL, 0, NULL, NULL},
    {"\x96Y\x8bp\x82\xcc\x8f\xe9           :%d", &DebugWork::g_dw.atelwork.TreCa, 1, {0}, 0, 0.0f, 1.0f, 0.0f, NULL, 0, NULL, NULL},
    {NULL, NULL, 0, {0}, 0, 0.0f, 0.0f, 0.0f, NULL, 0, NULL, NULL},
};

static DEBUG_WINDOW_TBL debugKeyTresur = {"\x96\xa2\x92m\x82\xc8\x82\xe9\x95\xf3\x82\xa0\x82\xaf\x82\xbd\x90\x94", -106, -(SCREEN_HEIGHT >> 1) + 150, 15, 29, 12, 0, 0, DWTEvent::debugKeyTresurLine, NULL, DWTEvent::debugKeyTresurCallBack};

static unsigned char SoraEnemyMemoIdTbl[35] = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20, 21, 22, 23, 24, 25, 26, 27, 28, 29, 30, 31, 100, 108, 103, 113};

static unsigned char RikuEnemyMemoIdTbl[34] = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 17, 18, 19, 20, 21, 22, 23, 24, 25, 26, 27, 28, 29, 30, 31, 100, 108, 103, 113};

static unsigned short SoraCardKindTbl[159] = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 19, 20, 21, 22, 23, 24, 26, 27, 28, 29, 30, 31, 32, 35, 36, 37, 38, 39, 40, 41, 42, 43, 44, 45, 46, 47, 48, 49, 50, 51, 52, 53, 54, 55, 57, 68, 69, 75, 76, 70, 79, 82, 78, 87, 71, 72, 73, 74, 92, 77, 80, 85, 88, 86, 81, 84, 83, 91, 90, 94, 95, 96, 97, 98, 93, 99, 104, 101, 105, 107, 106, 100, 108, 109, 102, 110, 103, 116, 118, 119, 120, 113, 115, 117, 121, 111, 122, 123, 114, 112, 124, 125, 126, 161, 162, 163, 164, 165, 166, 167, 168, 169, 170, 171, 172, 173, 174, 175, 176, 177, 178, 179, 180, 181, 182, 183, 184, 189, 186, 187, 188, 185, 190, 191, 192, 193, 194, 195, 196, 197, 198, 199, 200, 201, 58, 59, 60, 61, 62, 63, 64, 65};

static unsigned short RikuCardKindTbl[66] = {18, 42, 43, 56, 68, 75, 70, 79, 82, 87, 85, 81, 90, 89, 99, 104, 101, 105, 107, 106, 100, 108, 109, 102, 115, 117, 161, 162, 163, 164, 165, 166, 169, 170, 171, 172, 175, 176, 177, 178, 182, 183, 186, 187, 188, 185, 190, 191, 192, 193, 194, 195, 196, 197, 198, 200, 201, 202, 58, 59, 60, 61, 62, 63, 64, 65};

static BOOL DebugSetWorldTitleFlag(CDebugWindow* pObj, INT inY, INT inParam) {
    if (inParam & 0x180) {
        INT world = CGameWork::getInt(0, 0xC1);
        switch (inY) {
        case 1:
            world ^= 0x1;
            break;
        case 2:
            world ^= 0x2;
            break;
        case 3:
            world ^= 0x4;
            break;
        case 4:
            world ^= 0x8;
            break;
        case 5:
            world ^= 0x10;
            break;
        case 6:
            world ^= 0x20;
            break;
        case 7:
            world ^= 0x40;
            break;
        case 8:
            world ^= 0x80;
            break;
        case 9:
            world ^= 0x100;
            break;
        case 10:
            world ^= 0x200;
            break;
        case 11:
            world ^= 0x400;
            break;
        case 12:
            world ^= 0x800;
            break;
        case 13:
            world ^= 0x1000;
            break;
        // case 14 -> .L003034A8
        case 15:
            world ^= 0x10000;
            break;
        case 16:
            world ^= 0x20000;
            break;
        case 17:
            world ^= 0x40000;
            break;
        case 18:
            world ^= 0x80000;
            break;
        case 19:
            world ^= 0x100000;
            break;
        case 20:
            world ^= 0x200000;
            break;
        case 21:
            world ^= 0x400000;
            break;
        case 22:
            world ^= 0x800000;
            break;
        case 23:
            world ^= 0x1000000;
            break;
        case 24:
            world ^= 0x2000000;
            break;
        case 25:
            world ^= 0x4000000;
            break;
        case 26:
            world ^= 0x8000000;
            break;
        case 27:
            world ^= 0x10000000;
            break;
        }
        DebugWork::g_dw.atelwork.iWorldIntro = world;
        CGameWork::setInt(0, 0xC1, world);
    }
    return TRUE;
}

static BOOL DebugSetTutorialFlag(CDebugWindow* pObj, INT inY, INT inParam) {
    if (inParam & 0x180) {
        INT tuto = CGameWork::getInt(0, 0xC2);
        switch (inY) {
        case 0:
            tuto ^= 0x1;
            break;
        case 1:
            tuto ^= 0x2;
            break;
        case 2:
            tuto ^= 0x4;
            break;
        case 3:
            tuto ^= 0x8;
            break;
        case 4:
            tuto ^= 0x10;
            break;
        case 5:
            tuto ^= 0x20;
            break;
        case 6:
            tuto ^= 0x40;
            break;
        }
        DebugWork::g_dw.atelwork.iTutorialEnd = tuto;
        CGameWork::setInt(0, 0xC2, tuto);
    }
    return TRUE;
}

static BOOL DebugSetCardPossibleFlag(CDebugWindow* pObj, INT inY, INT inParam) {
    if (inParam & 0x180) {
        INT card = CGameWork::getInt(0, 0x21);
        switch (inY) {
        case 0:
            card ^= 0x1;
            break;
        case 1:
            card ^= 0x2;
            break;
        case 2:
            card ^= 0x4;
            break;
        case 3:
            card ^= 0x8;
            break;
        case 4:
            card ^= 0x10;
            break;
        case 5:
            card ^= 0x20;
            break;
        case 6:
            card ^= 0x40;
            break;
        case 7:
            card ^= 0x80;
            break;
        case 8:
            card ^= 0x100;
            break;
        case 9:
            card ^= 0x200;
            break;
        case 10:
            card ^= 0x400;
            break;
        case 11:
            card ^= 0x800;
            break;
        case 12:
            card ^= 0x1000;
            break;
        case 13:
            card ^= 0x2000;
            break;
        case 14:
            card ^= 0x4000;
            break;
        case 15:
            card ^= 0x8000;
            break;
        case 16:
            card ^= 0x10000;
            break;
        case 17:
            card ^= 0x20000;
            break;
        case 18:
            card ^= 0x40000;
            break;
        case 19:
            card ^= 0x80000;
            break;
        case 20:
            card ^= 0x100000;
            break;
        case 21:
            card ^= 0x200000;
            break;
        case 22:
            card ^= 0x400000;
            break;
        case 23:
            card ^= 0x800000;
            break;
        case 24:
            card ^= 0x1000000;
            break;
        case 25:
            card ^= 0x2000000;
            break;
        }
        DebugWork::g_dw.atelwork.iPossibleCard = card;
        CGameWork::setInt(0, 0x21, card);
    }
    return TRUE;
}

static BOOL DebugSetFriendPossibleFlag(CDebugWindow* pObj, INT inY, INT inParam) {
    if (inParam & 0x180) {
        INT fri = CGameWork::getInt(0, 0x22);
        switch (inY) {
        case 0:
            fri ^= 0x1;
            break;
        case 1:
            fri ^= 0x2;
            break;
        case 2:
            fri ^= 0x4;
            break;
        case 3:
            fri ^= 0x8;
            break;
        case 4:
            fri ^= 0x10;
            break;
        case 5:
            fri ^= 0x20;
            break;
        case 6:
            fri ^= 0x40;
            break;
        case 7:
            fri ^= 0x80;
            break;
        case 8:
            fri ^= 0x100;
            break;
        }
        DebugWork::g_dw.atelwork.iPossibleFriend = fri;
        CGameWork::setInt(0, 0x22, fri);
    }
    return TRUE;
}

static BOOL DebugSetPoohFlag(CDebugWindow* pObj, INT inY, INT inParam) {
    if (inParam & 0x180) {
        INT intro = CGameWork::getInt(0, 0x23);
        switch (inY) {
        case 4:
            intro ^= 0x1;
            break;
        case 5:
            intro ^= 0x2;
            break;
        case 6:
            intro ^= 0x4;
            break;
        case 7:
            intro ^= 0x8;
            break;
        case 8:
            intro ^= 0x10;
            break;
        case 9:
            intro ^= 0x20;
            break;
        case 10:
            intro ^= 0x40;
            break;
        case 11:
            intro ^= 0x80;
            break;
        }
        DebugWork::g_dw.atelwork.iPoohMiniGameIntro = intro;
        CGameWork::setInt(0, 0x23, intro);
        CGameWork::setInt(0, 0x2E, DebugWork::g_dw.atelwork.iPoohHiScoreOwl);
        CGameWork::setInt(0, 0x2F, DebugWork::g_dw.atelwork.iPoohHiScoreRoo);
        CGameWork::setInt(0, 0x30, DebugWork::g_dw.atelwork.iPoohHiScoreEeyore);
        CGameWork::setInt(0, 0x31, DebugWork::g_dw.atelwork.iPoohHiScoreTigger);
        CGameWork::setInt(0, 0x32, DebugWork::g_dw.atelwork.iPoohHiScoreRabbit);
        CGameWork::setByte(0, 3, DebugWork::g_dw.atelwork.iPoohGamePiglet);
        CGameWork::setByte(0, 4, DebugWork::g_dw.atelwork.iPoohGameOwl);
        CGameWork::setByte(0, 5, DebugWork::g_dw.atelwork.iPoohGameRoo);
        CGameWork::setByte(0, 6, DebugWork::g_dw.atelwork.iPoohGameEeyore);
        CGameWork::setByte(0, 7, DebugWork::g_dw.atelwork.iPoohGameTigger);
        CGameWork::setByte(0, 8, DebugWork::g_dw.atelwork.iPoohGameRabbit);
    }
    return TRUE;
}

static BOOL DebugSetPcSkillFlag(CDebugWindow* pObj, INT inY, INT inParam) {
    if (inParam & 0x180) {
        INT skill = CGameWork::getInt(0, 0x24);
        switch (inY) {
        case 0:
            skill ^= 0x1;
            break;
        case 1:
            skill ^= 0x2;
            break;
        case 2:
            skill ^= 0x4;
            break;
        case 3:
            skill ^= 0x8;
            break;
        case 4:
            skill ^= 0x10;
            break;
        case 5:
            skill ^= 0x20;
            break;
        }
        DebugWork::g_dw.atelwork.iPcStanderdSkill = skill;
        CGameWork::setInt(0, 0x24, skill);
    }
    return TRUE;
}

static BOOL DebugSetSoraMovieFlag(CDebugWindow* pObj, INT inY, INT inParam) {
    if (inParam & 0x180) {
        INT movie0 = CGameWork::getInt(0, 0x27);
        INT movie1 = CGameWork::getInt(0, 0x28);
        INT movie2 = CGameWork::getInt(0, 0x29);
        switch (inY) {
        case 0:
            movie0 ^= 0x1;
            break;
        case 1:
            movie0 ^= 0x2;
            break;
        case 2:
            movie0 ^= 0x4;
            break;
        case 3:
            movie0 ^= 0x8;
            break;
        case 4:
            movie0 ^= 0x10;
            break;
        case 5:
            movie0 ^= 0x20;
            break;
        case 6:
            movie0 ^= 0x40;
            break;
        case 7:
            movie0 ^= 0x80;
            break;
        case 8:
            movie0 ^= 0x100;
            break;
        case 9:
            movie0 ^= 0x200;
            break;
        case 10:
            movie0 ^= 0x400;
            break;
        case 11:
            movie0 ^= 0x800;
            break;
        case 12:
            movie0 ^= 0x1000;
            break;
        case 13:
            movie0 ^= 0x2000;
            break;
        case 14:
            movie0 ^= 0x4000;
            break;
        case 15:
            movie0 ^= 0x8000;
            break;
        case 16:
            movie0 ^= 0x10000;
            break;
        case 17:
            movie0 ^= 0x20000;
            break;
        case 18:
            movie0 ^= 0x40000;
            break;
        case 19:
            movie0 ^= 0x80000;
            break;
        case 20:
            movie0 ^= 0x100000;
            break;
        case 21:
            movie0 ^= 0x200000;
            break;
        case 22:
            movie0 ^= 0x400000;
            break;
        case 23:
            movie0 ^= 0x800000;
            break;
        case 24:
            movie0 ^= 0x1000000;
            break;
        case 25:
            movie0 ^= 0x2000000;
            break;
        case 26:
            movie0 ^= 0x4000000;
            break;
        case 27:
            movie0 ^= 0x8000000;
            break;
        case 28:
            movie0 ^= 0x10000000;
            break;
        case 29:
            movie0 ^= 0x20000000;
            break;
        case 30:
            movie0 ^= 0x40000000;
            break;
        case 31:
            movie0 ^= 0x80000000;
            break;
        case 32:
            movie1 ^= 0x1;
            break;
        case 33:
            movie1 ^= 0x2;
            break;
        case 34:
            movie1 ^= 0x4;
            break;
        case 35:
            movie1 ^= 0x8;
            break;
        case 36:
            movie1 ^= 0x10;
            break;
        case 37:
            movie1 ^= 0x20;
            break;
        case 38:
            movie1 ^= 0x40;
            break;
        case 39:
            movie1 ^= 0x80;
            break;
        case 40:
            movie1 ^= 0x100;
            break;
        case 41:
            movie1 ^= 0x200;
            break;
        case 42:
            movie1 ^= 0x400;
            break;
        case 43:
            movie1 ^= 0x800;
            break;
        case 44:
            movie1 ^= 0x1000;
            break;
        case 45:
            movie1 ^= 0x2000;
            break;
        case 46:
            movie1 ^= 0x4000;
            break;
        case 47:
            movie1 ^= 0x8000;
            break;
        case 48:
            movie1 ^= 0x10000;
            break;
        case 49:
            movie1 ^= 0x20000;
            break;
        case 50:
            movie1 ^= 0x40000;
            break;
        case 51:
            movie1 ^= 0x80000;
            break;
        case 52:
            movie1 ^= 0x100000;
            break;
        case 53:
            movie1 ^= 0x200000;
            break;
        case 54:
            movie1 ^= 0x400000;
            break;
        case 55:
            movie1 ^= 0x800000;
            break;
        case 56:
            movie1 ^= 0x1000000;
            break;
        case 57:
            movie1 ^= 0x2000000;
            break;
        case 58:
            movie1 ^= 0x4000000;
            break;
        }
        DebugWork::g_dw.atelwork.iSoraMovie0 = movie0;
        CGameWork::setInt(0, 0x27, movie0);
        DebugWork::g_dw.atelwork.iSoraMovie1 = movie1;
        CGameWork::setInt(0, 0x28, movie1);
        DebugWork::g_dw.atelwork.iSoraMovie2 = movie2;
        CGameWork::setInt(0, 0x29, movie2);
    }
    return TRUE;
}

static BOOL DebugSetRikuMovieFlag(CDebugWindow* pObj, INT inY, INT inParam) {
    if (inParam & 0x180) {
        INT movie0 = CGameWork::getInt(0, 0x2A);
        INT movie1 = CGameWork::getInt(0, 0x2B);
        INT movie2 = CGameWork::getInt(0, 0x2C);
        switch (inY) {
        case 0:
            movie0 ^= 0x1;
            break;
        case 1:
            movie0 ^= 0x2;
            break;
        case 2:
            movie0 ^= 0x4;
            break;
        case 3:
            movie0 ^= 0x8;
            break;
        case 4:
            movie0 ^= 0x10;
            break;
        case 5:
            movie0 ^= 0x20;
            break;
        case 6:
            movie0 ^= 0x40;
            break;
        case 7:
            movie0 ^= 0x80;
            break;
        case 8:
            movie0 ^= 0x100;
            break;
        case 9:
            movie0 ^= 0x200;
            break;
        case 10:
            movie0 ^= 0x400;
            break;
        case 11:
            movie0 ^= 0x800;
            break;
        case 12:
            movie0 ^= 0x1000;
            break;
        case 13:
            movie0 ^= 0x2000;
            break;
        case 14:
            movie0 ^= 0x4000;
            break;
        case 15:
            movie0 ^= 0x8000;
            break;
        case 16:
            movie0 ^= 0x10000;
            break;
        case 17:
            movie0 ^= 0x20000;
            break;
        case 18:
            movie0 ^= 0x40000;
            break;
        case 19:
            movie0 ^= 0x80000;
            break;
        case 20:
            movie0 ^= 0x100000;
            break;
        case 21:
            movie0 ^= 0x200000;
            break;
        case 22:
            movie0 ^= 0x400000;
            break;
        case 23:
            movie0 ^= 0x800000;
            break;
        case 24:
            movie0 ^= 0x1000000;
            break;
        case 25:
            movie0 ^= 0x2000000;
            break;
        case 26:
            movie0 ^= 0x4000000;
            break;
        case 27:
            movie0 ^= 0x8000000;
            break;
        case 28:
            movie0 ^= 0x10000000;
            break;
        case 29:
            movie0 ^= 0x20000000;
            break;
        case 30:
            movie0 ^= 0x40000000;
            break;
        case 31:
            movie0 ^= 0x80000000;
            break;
        case 32:
            movie1 ^= 0x1;
            break;
        case 33:
            movie1 ^= 0x2;
            break;
        case 34:
            movie1 ^= 0x4;
            break;
        case 35:
            movie1 ^= 0x8;
            break;
        case 36:
            movie1 ^= 0x10;
            break;
        case 37:
            movie1 ^= 0x20;
            break;
        case 38:
            movie1 ^= 0x40;
            break;
        case 39:
            movie1 ^= 0x80;
            break;
        case 40:
            movie1 ^= 0x100;
            break;
        }
        DebugWork::g_dw.atelwork.iRikuMovie0 = movie0;
        CGameWork::setInt(0, 0x2A, movie0);
        DebugWork::g_dw.atelwork.iRikuMovie1 = movie1;
        CGameWork::setInt(0, 0x2B, movie1);
        DebugWork::g_dw.atelwork.iRikuMovie2 = movie2;
        CGameWork::setInt(0, 0x2C, movie2);
    }
    return TRUE;
}

static BOOL DebugSetKH2CardGetFlag(CDebugWindow* pObj, INT inY, INT inParam) {
    if (inParam & 0x180) {
        INT card = CGameWork::getInt(0, 0x2D);
        switch (inY) {
        case 0:
            card ^= 0x1;
            break;
        case 1:
            card ^= 0x2;
            break;
        case 2:
            card ^= 0x4;
            break;
        case 3:
            card ^= 0x8;
            break;
        case 4:
            card ^= 0x10;
            break;
        case 5:
            card ^= 0x20;
            break;
        case 6:
            card ^= 0x40;
            break;
        case 7:
            card ^= 0x80;
            break;
        case 8:
            card ^= 0x100;
            break;
        case 9:
            card ^= 0x200;
            break;
        case 10:
            card ^= 0x400;
            break;
        case 11:
            card ^= 0x800;
            break;
        case 12:
            card ^= 0x1000;
            break;
        }
        DebugWork::g_dw.atelwork.iKH2CardGet = card;
        CGameWork::setInt(0, 0x2D, card);
    }
    return TRUE;
}

static BOOL DebugSetJiminyMemoFlag(CDebugWindow* pObj, INT inY, INT inParam) {
    if (inParam & 0x180) {
        INT SoraWorld = DebugWork::g_dw.jiminy.Sora_World;
        INT SoraFloor = DebugWork::g_dw.jiminy.Sora_Floor;
        INT RikuFloor = DebugWork::g_dw.jiminy.Riku_Floor;
        INT RikuWorld = DebugWork::g_dw.jiminy.Riku_World;
        INT SoraEtc = DebugWork::g_dw.jiminy.Sora_Etc;
        INT offsetY;
        INT shiftY;
        switch (inY) {
        case 1:
        case 2:
        case 3:
        case 4:
        case 5:
        case 6:
        case 7:
        case 8:
        case 9:
        case 10:
        case 11:
        case 12:
        case 13:
            shiftY = 1 << (inY - 1);
            SoraWorld ^= shiftY;
            CJiminyMemoFlag::debugEvent(SoraWorld & shiftY, 0, 1, inY, 0);
            break;
        case 14:
        case 15:
        case 16:
        case 17:
        case 18:
        case 19:
        case 20:
        case 21:
        case 22:
        case 23:
        case 24:
        case 25:
        case 26:
            shiftY = 1 << (inY - 14);
            SoraFloor ^= shiftY;
            CJiminyMemoFlag::debugEvent(SoraFloor & shiftY, 0, 2, inY - 13, 0);
            break;
        case 28:
        case 29:
        case 30:
        case 31:
        case 32:
        case 33:
        case 34:
        case 35:
        case 36:
        case 37:
        case 38:
        case 39:
            shiftY = 1 << (inY - 28);
            RikuWorld ^= shiftY;
            CJiminyMemoFlag::debugEvent(RikuWorld & shiftY, 1, 1, inY - 27, 0);
            break;
        case 40:
        case 41:
        case 42:
        case 43:
        case 44:
        case 45:
        case 46:
        case 47:
        case 48:
        case 49:
        case 50:
        case 51:
            shiftY = 1 << (inY - 40);
            RikuFloor ^= shiftY;
            CJiminyMemoFlag::debugEvent(RikuFloor & shiftY, 1, 2, inY - 52, 0);
            break;
        case 53:
            SoraEtc ^= 0x1;
            CJiminyMemoFlag::debugEvent(SoraEtc & 0x1, 0, 8, 0, 0);
            break;
        case 54:
            SoraEtc ^= 0x2;
            CJiminyMemoFlag::debugEvent(SoraEtc & 0x2, 0, 8, 1, 0);
            break;
        case 55:
            SoraEtc ^= 0x4;
            CJiminyMemoFlag::debugEvent(SoraEtc & 0x4, 0, 8, 2, 0);
            break;
        case 56:
            SoraEtc ^= 0x8;
            CJiminyMemoFlag::debugEvent(SoraEtc & 0x8, 0, 8, 3, 0);
            break;
        case 57:
            SoraEtc ^= 0x10;
            CJiminyMemoFlag::debugEvent(SoraEtc & 0x10, 0, 8, 4, 0);
            break;
        case 58:
            SoraEtc ^= 0x20;
            CJiminyMemoFlag::debugEvent(SoraEtc & 0x20, 0, 6, 0, 0);
            break;
        case 59:
            SoraEtc ^= 0x40;
            CJiminyMemoFlag::debugEvent(SoraEtc & 0x40, 0, 5, 0, 0);
            break;
        case 60:
            SoraEtc ^= 0x80;
            CJiminyMemoFlag::debugEvent(SoraEtc & 0x80, 0, 10, 11, 0);
            break;
        case 61:
            SoraEtc ^= 0x100;
            CJiminyMemoFlag::debugEvent(SoraEtc & 0x100, 0, 10, 12, 0);
            break;
        case 62:
            SoraEtc ^= 0x200;
            CJiminyMemoFlag::debugEvent(SoraEtc & 0x200, 0, 10, 13, 0);
            break;
        case 63:
            SoraEtc ^= 0x400;
            CJiminyMemoFlag::debugEvent(SoraEtc & 0x400, 0, 10, 14, 0);
            break;
        case 64:
            SoraEtc ^= 0x800;
            CJiminyMemoFlag::debugEvent(SoraEtc & 0x800, 0, 10, 15, 0);
            break;
        case 65:
            SoraEtc ^= 0x1000;
            CJiminyMemoFlag::debugEvent(SoraEtc & 0x1000, 0, 10, 16, 0);
            break;
        case 66:
            SoraEtc ^= 0x2000;
            CJiminyMemoFlag::debugEvent(SoraEtc & 0x2000, 0, 9, 0, 0);
            break;
        }
        DebugWork::g_dw.jiminy.Sora_World = SoraWorld;
        DebugWork::g_dw.jiminy.Sora_Floor = SoraFloor;
        DebugWork::g_dw.jiminy.Riku_World = RikuWorld;
        DebugWork::g_dw.jiminy.Riku_Floor = RikuFloor;
        DebugWork::g_dw.jiminy.Sora_Etc = SoraEtc;
    } else if (inParam & 0x20) {
        INT SoraFlag = DebugWork::g_dw.jiminy.Sora_Flag;
        INT SoraEKind = SoraEnemyMemoIdTbl[DebugWork::g_dw.jiminy.Sora_Enemy];
        INT SoraENum = DebugWork::g_dw.jiminy.Sora_ENum;
        INT SoraCard = DebugWork::g_dw.jiminy.Sora_Card;
        USHORT SoraCKind = SoraCardKindTbl[SoraCard];
        INT RikuFlag = DebugWork::g_dw.jiminy.Riku_Flag;
        INT RikuEKind = RikuEnemyMemoIdTbl[DebugWork::g_dw.jiminy.Riku_Enemy];
        INT RikuENum = DebugWork::g_dw.jiminy.Riku_ENum;
        INT RikuCard = DebugWork::g_dw.jiminy.Riku_Card;
        USHORT RikuCKind = RikuCardKindTbl[RikuCard];
        switch (inY) {
        case 68:
        case 69:
        case 70:
            SoraFlag |= 0x701;
            CJiminyMemoFlag::debugEvent(1, 0, 3, SoraEKind, SoraENum);
            break;
        case 72:
        case 73:
        case 74:
            RikuFlag |= 0x701;
            CJiminyMemoFlag::debugEvent(1, 1, 3, RikuEKind, RikuENum);
            break;
        case 76:
        case 77:
            SoraFlag |= 0x7002;
            CJiminyMemoFlag::debugEvent(1, 0, 4, SoraCKind, 0);
            break;
        case 79:
        case 80:
            RikuFlag |= 0x7002;
            CJiminyMemoFlag::debugEvent(1, 1, 4, RikuCKind, 0);
            break;
        }
        DebugWork::g_dw.jiminy.Sora_Flag = SoraFlag;
        DebugWork::g_dw.jiminy.Riku_Flag = RikuFlag;
    } else {
        INT SoraFlag = DebugWork::g_dw.jiminy.Sora_Flag;
        INT RikuFlag = DebugWork::g_dw.jiminy.Riku_Flag;
        INT n;
        for (INT i = 0; i < 2; i++) {
            if ((SoraFlag >> i) & 1) {
                n = (SoraFlag >> (i * 4 + 8)) & 0xF;
                n--;
                if (n == 0) {
                    SoraFlag &= ~(1 << i);
                }
                SoraFlag = (SoraFlag & ~(0xF << (i * 4 + 8))) | (n << (i * 4 + 8));
            }
        }
        for (INT i = 0; i < 2; i++) {
            if ((RikuFlag >> i) & 1) {
                n = (RikuFlag >> (i * 4 + 8)) & 0xF;
                n--;
                if (n == 0) {
                    RikuFlag &= ~(1 << i);
                }
                RikuFlag = (RikuFlag & ~(0xF << (i * 4 + 8))) | (n << (i * 4 + 8));
            }
        }
        DebugWork::g_dw.jiminy.Sora_Flag = SoraFlag;
        DebugWork::g_dw.jiminy.Riku_Flag = RikuFlag;
    }
    return TRUE;
}

static char* szJMSoraEnemyKind[35] = {"\x83V\x83\x83\x83h\x83" "E", "\x83\\\x83\x8b\x83W\x83\x83\x81[", "\x83\x8c\x83" "b\x83h\x83m\x83N\x83^\x81[\x83\x93", "\x83u\x83\x8b\x81[\x83\x89\x83v\x83\\\x83" "f\x83" "B", "\x83" "C\x83G\x83\x8d\x81[\x83I\x83y\x83\x89", "\x83O\x83\x8a\x81[\x83\x93\x83\x8c\x83N\x83" "C\x83G\x83\x80", "\x83p\x83\x8f\x81[\x83\x8f\x83" "C\x83\x8b\x83h", "\x83o\x83" "E\x83\x93\x83V\x81[\x83\x8f\x83" "C\x83\x8b\x83h", "\x83\x89\x81[\x83W\x83{\x83" "f\x83" "B", "\x83t\x83@\x83" "b\x83g\x83o\x83\x93\x83" "f\x83" "B\x83" "b\x83g", "\x83o\x83\x93\x83" "f\x83" "B\x83" "b\x83g", "\x83\x8f\x83" "C\x83g\x83i\x83" "C\x83g", "\x83K\x81[\x83S\x83" "C\x83\x8b", "\x83T\x81[\x83`\x83S\x81[\x83X\x83g", "\x83G\x83" "A\x83\\\x83\x8b\x83W\x83\x83\x81[", "\x83z\x83\x8f\x83" "C\x83g\x83}\x83" "b\x83V\x83\x85\x83\x8b\x81[\x83\x80", "\x83u\x83\x89\x83" "b\x83N\x83t\x83@\x83\x93\x83K\x83X", "\x83V\x81[\x83l\x83I\x83\x93", "\x83X\x83N\x83\x8a\x83\x85\x81[\x83_\x83" "C\x83o\x81[", "\x83" "A\x83N\x83" "A\x83^\x83\x93\x83N", "\x83p\x83" "C\x83\x8c\x81[\x83" "c", "\x83G\x83" "A\x83p\x83" "C\x83\x8c\x81[\x83" "c", "\x83_\x81[\x83N\x83{\x81[\x83\x8b", "\x83\x8f\x83" "C\x83o\x81[\x83\x93", "\x83" "E\x83" "B\x83U\x81[\x83h", "\x83" "f\x83" "B\x83t\x83" "F\x83\x93\x83_\x81[", "\x83l\x83I\x83V\x83\x83\x83h\x83" "E", "\x83o\x83\x8c\x83\x8b\x83X\x83p\x83" "C\x83_\x81[", "\x83g\x83\x8b\x83l\x81[\x83h\x83X\x83" "e\x83" "b\x83v", "\x83\x89\x83" "E\x83h\x83l\x83X", "\x83N\x83\x8a\x81[\x83v\x83v\x83\x89\x83\x93\x83g", "\x83K\x81[\x83h\x83" "A\x81[\x83}\x81[", "\x83p\x83\x89\x83T\x83" "C\x83g\x83P\x83" "C\x83W", "\x83g\x83\x8a\x83" "b\x83N\x83}\x83X\x83^\x81[", "\x83_\x81[\x83N\x83T\x83" "C\x83h"};

static char* szJMRikuEnemyKind[34] = {"\x83V\x83\x83\x83h\x83" "E", "\x83\\\x83\x8b\x83W\x83\x83\x81[", "\x83\x8c\x83" "b\x83h\x83m\x83N\x83^\x81[\x83\x93", "\x83u\x83\x8b\x81[\x83\x89\x83v\x83\\\x83" "f\x83" "B", "\x83" "C\x83G\x83\x8d\x81[\x83I\x83y\x83\x89", "\x83O\x83\x8a\x81[\x83\x93\x83\x8c\x83N\x83" "C\x83G\x83\x80", "\x83p\x83\x8f\x81[\x83\x8f\x83" "C\x83\x8b\x83h", "\x83o\x83" "E\x83\x93\x83V\x81[\x83\x8f\x83" "C\x83\x8b\x83h", "\x83\x89\x81[\x83W\x83{\x83" "f\x83" "B", "\x83t\x83@\x83" "b\x83g\x83o\x83\x93\x83" "f\x83" "B\x83" "b\x83g", "\x83o\x83\x93\x83" "f\x83" "B\x83" "b\x83g", "\x83\x8f\x83" "C\x83g\x83i\x83" "C\x83g", "\x83K\x81[\x83S\x83" "C\x83\x8b", "\x83T\x81[\x83`\x83S\x81[\x83X\x83g", "\x83G\x83" "A\x83\\\x83\x8b\x83W\x83\x83\x81[", "\x83u\x83\x89\x83" "b\x83N\x83t\x83@\x83\x93\x83K\x83X", "\x83V\x81[\x83l\x83I\x83\x93", "\x83X\x83N\x83\x8a\x83\x85\x81[\x83_\x83" "C\x83o\x81[", "\x83" "A\x83N\x83" "A\x83^\x83\x93\x83N", "\x83p\x83" "C\x83\x8c\x81[\x83" "c", "\x83G\x83" "A\x83p\x83" "C\x83\x8c\x81[\x83" "c", "\x83_\x81[\x83N\x83{\x81[\x83\x8b", "\x83\x8f\x83" "C\x83o\x81[\x83\x93", "\x83" "E\x83" "B\x83U\x81[\x83h", "\x83" "f\x83" "B\x83t\x83" "F\x83\x93\x83_\x81[", "\x83l\x83I\x83V\x83\x83\x83h\x83" "E", "\x83o\x83\x8c\x83\x8b\x83X\x83p\x83" "C\x83_\x81[", "\x83g\x83\x8b\x83l\x81[\x83h\x83X\x83" "e\x83" "b\x83v", "\x83\x89\x83" "E\x83h\x83l\x83X", "\x83N\x83\x8a\x81[\x83v\x83v\x83\x89\x83\x93\x83g", "\x83K\x81[\x83h\x83" "A\x81[\x83}\x81[", "\x83p\x83\x89\x83T\x83" "C\x83g\x83P\x83" "C\x83W", "\x83g\x83\x8a\x83" "b\x83N\x83}\x83X\x83^\x81[", "\x83_\x81[\x83N\x83T\x83" "C\x83h"};

static char* szJMSoraCardKindTbl[159] = {"\x83L\x83\x93\x83O\x83_\x83\x80\x83`\x83" "F\x81[\x83\x93", "\x83" "f\x83U\x83" "C\x83" "A\x81[\x83\x89\x83\x93\x83v", "\x83g\x83\x8c\x83W\x83\x83\x81[\x83I\x83u\x83V\x81[", "\x83p\x83\x93\x83v\x83L\x83\x93\x83w\x83" "b\x83h", "\x83t\x83" "F\x83" "A\x83\x8a\x81[\x83n\x81[\x83v", "\x83" "E\x83" "B\x83" "b\x83V\x83\x85\x83X\x83^\x81[", "\x83G\x83O\x83U\x83~\x83l\x81[\x83V\x83\x87\x83\x93", "\x83\x81\x83^\x83\x8b\x83`\x83\x87\x83R\x83{", "\x83p\x83\x8f\x81[\x83I\x83u\x83q\x81[\x83\x8d\x81[", "\x83\x89\x83" "C\x83I\x83\x93\x83n\x81[\x83g", "\x83\x89\x83X\x83g\x83\x8a\x83]\x81[\x83g", "\x83\x89\x83\x94\x83" "B\x83" "A\x83\x93\x83\x8d\x81[\x83Y", "\x96\xf1\x91\xa9\x82\xcc\x82\xa8\x8e\xe7\x82\xe8", "\x89\xdf\x82\xac\x8b\x8e\x82\xe8\x82\xb5\x8ev\x82\xa2\x8fo", "\x83" "A\x83\x8b\x83" "e\x83}\x83" "E\x83" "F\x83|\x83\x93", "\x83_\x83" "C\x83\x84\x83\x82\x83\x93\x83h\x83_\x83X\x83g", "\x95\xd0\x97\x83\x82\xcc\x93V\x8eg", "\x83X\x83^\x81[\x83V\x81[\x83J\x81[", "\x83\x82\x83m\x83N\x83\x8d\x81[\x83\x80", "\x83t\x83H\x83\x8d\x81[\x83U\x83" "E\x83" "B\x83\x93\x83h", "\x83q\x83h\x83" "D\x83\x93\x83h\x83\x89\x83S\x83\x93", "\x83t\x83H\x83g\x83\x93\x83" "f\x83o\x83" "b\x83K\x81[", "\x83{\x83\x93\x83h\x83I\x83u\x83t\x83\x8c\x83" "C\x83\x80", "\x83t\x83@\x83" "C\x83" "A", "\x83u\x83\x8a\x83U\x83h", "\x83T\x83\x93\x83_\x81[", "\x83P\x83" "A\x83\x8b", "\x83O\x83\x89\x83r\x83" "f", "\x83X\x83g\x83" "b\x83v", "\x83G\x83" "A\x83\x8d", "\x83V\x83\x93\x83o", "\x83W\x81[\x83j\x81[", "\x83o\x83\x93\x83r", "\x83_\x83\x93\x83{", "\x83" "e\x83" "B\x83\x93\x83N", "\x83\x80\x81[\x83V\x83\x85\x81[", "\x83N\x83\x89\x83" "E\x83h", "\x83|\x81[\x83V\x83\x87\x83\x93", "\x83n\x83" "C\x83|\x81[\x83V\x83\x87\x83\x93", "\x83\x81\x83K\x83|\x81[\x83V\x83\x87\x83\x93", "\x83G\x81[\x83" "e\x83\x8b", "\x83\x81\x83K\x83G\x81[\x83" "e\x83\x8b", "\x83G\x83\x8a\x83N\x83T\x81[", "\x83\x89\x83X\x83g\x83G\x83\x8a\x83N\x83T\x81[", "\x83h\x83i\x83\x8b\x83h", "\x83O\x81[\x83t\x83" "B\x81[", "\x83" "A\x83\x89\x83W\x83\x93", "\x83" "A\x83\x8a\x83G\x83\x8b", "\x83W\x83\x83\x83" "b\x83N", "\x83s\x81[\x83^\x81[\x83p\x83\x93", "\x83r\x81[\x83X\x83g", "\x83v\x83\x8b\x81[\x83g", "\x83V\x83\x83\x83h\x83" "E", "\x83\\\x83\x8b\x83W\x83\x83\x81[", "\x83p\x83\x8f\x81[\x83\x8f\x83" "C\x83\x8b\x83h", "\x83o\x83" "E\x83\x93\x83V\x81[\x83\x8f\x83" "C\x83\x8b\x83h", "\x83\x89\x81[\x83W\x83{\x83" "f\x83" "B", "\x83t\x83@\x83" "b\x83g\x83o\x83\x93\x83" "f\x83" "B\x83" "b\x83g", "\x83V\x81[\x83l\x83I\x83\x93", "\x83o\x83\x93\x83" "f\x83" "B\x83" "b\x83g", "\x83p\x83" "C\x83\x8c\x81[\x83" "c", "\x83\x8c\x83" "b\x83h\x83m\x83N\x83^\x81[\x83\x93", "\x83u\x83\x8b\x81[\x83\x89\x83v\x83\\\x83" "f\x83" "B", "\x83" "C\x83G\x83\x8d\x81[\x83I\x83y\x83\x89", "\x83O\x83\x8a\x81[\x83\x93\x83\x8c\x83N\x83" "C\x83G\x83\x80", "\x83" "E\x83" "B\x83U\x81[\x83h", "\x83G\x83" "A\x83\\\x83\x8b\x83W\x83\x83\x81[", "\x83o\x83\x8c\x83\x8b\x83X\x83p\x83" "C\x83_\x81[", "\x83\x8f\x83" "C\x83g\x83i\x83" "C\x83g", "\x83G\x83" "A\x83p\x83" "C\x83\x8c\x81[\x83" "c", "\x83K\x81[\x83S\x83" "C\x83\x8b", "\x83T\x81[\x83`\x83S\x81[\x83X\x83g", "\x83" "A\x83N\x83" "A\x83^\x83\x93\x83N", "\x83X\x83N\x83\x8a\x83\x85\x81[\x83_\x83" "C\x83o\x81[", "\x83\x8f\x83" "C\x83o\x81[\x83\x93", "\x83" "f\x83" "B\x83t\x83" "F\x83\x93\x83_\x81[", "\x83z\x83\x8f\x83" "C\x83g\x83}\x83" "b\x83V\x83\x85\x83\x8b\x81[\x83\x80", "\x83u\x83\x89\x83" "b\x83N\x83t\x83@\x83\x93\x83K\x83X", "\x83N\x83\x8a\x81[\x83v\x83v\x83\x89\x83\x93\x83g", "\x83g\x83\x8b\x83l\x81[\x83h\x83X\x83" "e\x83" "b\x83v", "\x83\x89\x83" "E\x83h\x83l\x83X", "\x83l\x83I\x83V\x83\x83\x83h\x83" "E", "\x83K\x81[\x83h\x83" "A\x81[\x83}\x81[", "\x83n\x83" "f\x83X", "\x83g\x83\x8a\x83" "b\x83N\x83}\x83X\x83^\x81[", "\x83W\x83\x83\x83t\x83@\x81[", "\x83" "A\x81[\x83X\x83\x89", "\x83u\x83M\x81[", "\x83p\x83\x89\x83T\x83" "C\x83g\x83P\x83" "C\x83W", "\x83t\x83" "b\x83N", "\x83}\x83\x8c\x83t\x83" "B\x83Z\x83\x93\x83g\x83h\x83\x89\x83S\x83\x93", "\x83_\x81[\x83N\x83T\x83" "C\x83h", "\x83\x8a\x83N", "\x83g\x83\x89\x83\x93\x83v\x95\xba", "\x83" "A\x83\x93\x83Z\x83\x80", "\x83[\x83\x80\x83i\x83X", "\x83V\x83O\x83o\x81[\x83\x8b", "\x83U\x83\x8b\x83" "f\x83" "B\x83\x93", "\x83\x94\x83" "B\x83N\x83Z\x83\x93", "\x83\x8c\x83N\x83Z\x83" "E\x83X", "\x83[\x83N\x83V\x83I\x83\x93", "\x83T\x83" "C\x83N\x83X", "\x83" "A\x83N\x83Z\x83\x8b", "\x83" "f\x83~\x83" "b\x83N\x83X", "\x83\x8b\x83N\x83\\\x81[\x83h", "\x83}\x81[\x83\x8b\x81[\x83V\x83\x83", "\x83\x89\x83N\x83V\x81[\x83k", "\x83\x8d\x83N\x83T\x83X", "\x83S\x81[\x83\x8b\x83h", "\x83v\x83\x89\x83`\x83i", "\x90\xc3\x82\xb6\x82\xe1\x82\xad\x82\xc8\x82\xe9\x88\xc5", "\x82\xd0\x82\xb5\x82\xdf\x82\xad\x88\xc5", "\x82\xbf\x82\xa9\x82\xe7\x82\xc8\x82\xab\x88\xc5", "\x82\xa8\x82\xa8\x82\xa2\x82\xc8\x82\xe9\x88\xc5", "\x96\xb0\x82\xe8\x82\xcc\x88\xc5", "\x82\xb9\x82\xdc\x82\xe8\x82\xad\x82\xe9\x88\xc5", "\x83v\x83\x8c\x83~\x83" "A\x83\x8b\x81[\x83\x80", "\x83z\x83\x8f\x83" "C\x83g\x83\x8b\x81[\x83\x80", "\x83u\x83\x89\x83" "b\x83N\x83\x8b\x81[\x83\x80", "\x92\xea\x92m\x82\xea\x82\xca\x88\xc5", "\x83\x8b\x81[\x83\x8c\x83" "b\x83g\x83\x8b\x81[\x83\x80", "\x95\x90\x97\xcd\x82\xd6\x82\xcc\x82\xdf\x82\xb4\x82\xdf", "\x96\x82\x97\xcd\x82\xd6\x82\xcc\x82\xdf\x82\xb4\x82\xdf", "\x98" "B\x8b\xe0\x82\xd6\x82\xcc\x82\xdf\x82\xb4\x82\xdf", "\x82\xdf\x82\xae\x82\xe8\x82\xa0\x82\xa2\x82\xcc\x92n", "\x82\xc4\x82\xa2\x82\xbd\x82\xa2\x82\xcc\x8a\xd4", "\x83_\x83\x81\x81[\x83W\x83" "A\x83" "b\x83v\x83\x8b\x81[\x83\x80", "\x83X\x83^\x83\x93\x83v\x83\x89\x83X\x83\x8b\x81[\x83\x80", "\x82\xe2\x82\xb7\x82\xe7\x82\xac\x82\xcc\x95\xf3", "\x8e\xe7\x82\xe7\x82\xea\x82\xb5\x95\xf3", "\x82\xa2\x82\xc2\x82\xed\x82\xe8\x82\xcc\x95\xf3", "\x88\xea\x8e\x9e\x82\xcc\x8bx\x91\xa7", "\x8c\xf0\x82\xb6\x82\xe8\x8d\x87\x82\xa4\x90\xa2\x8a" "E", "\x83\x82\x81[\x83O\x83\x8a\x83\x8b\x81[\x83\x80", "\x96\xa2\x92m\x82\xc8\x82\xe9\x95\xf3\x82\xcc\x83L\x81[\x83J\x81[\x83h", "\x82\xcd\x82\xb6\x82\xdc\x82\xe8\x82\xcc\x83L\x81[\x83J\x81[\x83h", "\x82\xdd\x82\xbf\x82\xd1\x82\xab\x82\xcc\x83L\x81[\x83J\x81[\x83h", "\x90^\x8e\xc0\x82\xd6\x82\xcc\x83L\x81[\x83J\x81[\x83h", "\x83W\x83\x87\x81[\x83J\x81[", "\x83g\x83\x89\x83o\x81[\x83X\x83^\x83" "E\x83\x93", "\x83" "A\x83O\x83\x89\x83o\x81[", "\x83I\x83\x8a\x83\x93\x83|\x83X\x81" "E\x83R\x83\x8d\x83V\x83" "A\x83\x80", "\x83\x8f\x83\x93\x83_\x81[\x83\x89\x83\x93\x83h", "\x83\x82\x83\x93\x83X\x83g\x83\x8d", "\x83n\x83\x8d\x83" "E\x83" "B\x83\x93\x83^\x83" "E\x83\x93", "\x83" "A\x83g\x83\x89\x83\x93\x83" "e\x83" "B\x83J", "\x83l\x83o\x81[\x83\x89\x83\x93\x83h", "\x83z\x83\x8d\x83" "E\x83o\x83X\x83" "e\x83" "B\x83I\x83\x93", "100\x83G\x81[\x83J\x81[\x82\xcc\x90X", "\x83g\x83\x8f\x83" "C\x83\x89\x83" "C\x83g\x83^\x83" "E\x83\x93", "\x83" "f\x83X\x83" "e\x83" "B\x83j\x81[\x83" "A\x83" "C\x83\x89\x83\x93\x83h", "\x83M\x83~\x83" "b\x83N-\x83K\x81[\x83h\x83" "A\x81[\x83}\x81[", "\x83M\x83~\x83" "b\x83N-\x83g\x83\x8a\x83" "b\x83N\x83X\x83^\x81[", "\x83M\x83~\x83" "b\x83N-\x83W\x83\x83\x83t\x83@\x81[", "\x83M\x83~\x83" "b\x83N-\x83u\x83M\x81[", "\x83M\x83~\x83" "b\x83N-\x83p\x83\x89\x83T\x83" "C\x83g\x83P\x83" "C\x83W", "\x83M\x83~\x83" "b\x83N-\x83" "A\x81[\x83X\x83\x89", "\x83M\x83~\x83" "b\x83N-\x83}\x83\x8c\x83t\x83" "B\x83Z\x83\x93\x83g\x83h\x83\x89\x83S\x83\x93", "\x83M\x83~\x83" "b\x83N-\x83_\x81[\x83N\x83T\x83" "C\x83h"};

static char* szJMRikuCardKindTbl[66] = {"\x83\\\x83" "E\x83\x8b\x83" "C\x81[\x83^\x81[", "\x83|\x81[\x83V\x83\x87\x83\x93", "\x83n\x83" "C\x83|\x81[\x83V\x83\x87\x83\x93", "\x89\xa4\x97l", "\x83V\x83\x83\x83h\x83" "E", "\x83p\x83\x8f\x81[\x83\x8f\x83" "C\x83\x8b\x83h", "\x83\x89\x81[\x83W\x83{\x83" "f\x83" "B", "\x83t\x83@\x83" "b\x83g\x83o\x83\x93\x83" "f\x83" "B\x83" "b\x83g", "\x83V\x81[\x83l\x83I\x83\x93", "\x83p\x83" "C\x83\x8c\x81[\x83" "c", "\x83\x8f\x83" "C\x83g\x83i\x83" "C\x83g", "\x83T\x81[\x83`\x83S\x81[\x83X\x83g", "\x83" "f\x83" "B\x83t\x83" "F\x83\x93\x83_\x81[", "\x83_\x81[\x83N\x83{\x81[\x83\x8b", "\x83K\x81[\x83h\x83" "A\x81[\x83}\x81[", "\x83n\x83" "f\x83X", "\x83g\x83\x8a\x83" "b\x83N\x83}\x83X\x83^\x81[", "\x83W\x83\x83\x83t\x83@\x81[", "\x83" "A\x81[\x83X\x83\x89", "\x83u\x83M\x81[", "\x83p\x83\x89\x83T\x83" "C\x83g\x83P\x83" "C\x83W", "\x83t\x83" "b\x83N", "\x83}\x83\x8c\x83t\x83" "B\x83Z\x83\x93\x83g\x83h\x83\x89\x83S\x83\x93", "\x83_\x81[\x83N\x83T\x83" "C\x83h", "\x83\x8c\x83N\x83Z\x83" "E\x83X", "\x83[\x83N\x83V\x83I\x83\x93", "\x90\xc3\x82\xb6\x82\xe1\x82\xad\x82\xc8\x82\xe9\x88\xc5", "\x82\xd0\x82\xb5\x82\xdf\x82\xad\x88\xc5", "\x82\xbf\x82\xa9\x82\xe7\x82\xc8\x82\xab\x88\xc5", "\x82\xa8\x82\xa8\x82\xa2\x82\xc8\x82\xe9\x88\xc5", "\x96\xb0\x82\xe8\x82\xcc\x88\xc5", "\x82\xb9\x82\xdc\x82\xe8\x82\xad\x82\xe9\x88\xc5", "\x83u\x83\x89\x83" "b\x83N\x83\x8b\x81[\x83\x80", "\x92\xea\x92m\x82\xea\x82\xca\x88\xc5", "\x83\x8b\x81[\x83\x8c\x83" "b\x83g\x83\x8b\x81[\x83\x80", "\x95\x90\x97\xcd\x82\xd6\x82\xcc\x82\xdf\x82\xb4\x82\xdf", "\x82\xdf\x82\xae\x82\xe8\x82\xa0\x82\xa2\x82\xcc\x92n", "\x82\xc4\x82\xa2\x82\xbd\x82\xa2\x82\xcc\x8a\xd4", "\x83_\x83\x81\x81[\x83W\x83" "A\x83" "b\x83v\x83\x8b\x81[\x83\x80", "\x83X\x83^\x83\x93\x83v\x83\x89\x83X\x83\x8b\x81[\x83\x80", "\x88\xea\x8e\x9e\x82\xcc\x8bx\x91\xa7", "\x8c\xf0\x82\xb6\x82\xe8\x8d\x87\x82\xa4\x90\xa2\x8a" "E", "\x82\xcd\x82\xb6\x82\xdc\x82\xe8\x82\xcc\x83L\x81[\x83J\x81[\x83h", "\x82\xdd\x82\xbf\x82\xd1\x82\xab\x82\xcc\x83L\x81[\x83J\x81[\x83h", "\x90^\x8e\xc0\x82\xd6\x82\xcc\x83L\x81[\x83J\x81[\x83h", "\x83W\x83\x87\x81[\x83J\x81[", "\x83g\x83\x89\x83o\x81[\x83X\x83^\x83" "E\x83\x93", "\x83" "A\x83O\x83\x89\x83o\x81[", "\x83I\x83\x8a\x83\x93\x83|\x83X\x81" "E\x83R\x83\x8d\x83V\x83" "A\x83\x80", "\x83\x8f\x83\x93\x83_\x81[\x83\x89\x83\x93\x83h", "\x83\x82\x83\x93\x83X\x83g\x83\x8d", "\x83n\x83\x8d\x83" "E\x83" "B\x83\x93\x83^\x83" "E\x83\x93", "\x83" "A\x83g\x83\x89\x83\x93\x83" "e\x83" "B\x83J", "\x83l\x83o\x81[\x83\x89\x83\x93\x83h", "\x83z\x83\x8d\x83" "E\x83o\x83X\x83" "e\x83" "B\x83I\x83\x93", "\x83g\x83\x8f\x83" "C\x83\x89\x83" "C\x83g\x83^\x83" "E\x83\x93", "\x83" "f\x83X\x83" "e\x83" "B\x83j\x81[\x83" "A\x83" "C\x83\x89\x83\x93\x83h", "\x96Y\x8bp\x82\xcc\x8f\xe9", "\x83M\x83~\x83" "b\x83N-\x83K\x81[\x83h\x83" "A\x81[\x83}\x81[", "\x83M\x83~\x83" "b\x83N-\x83g\x83\x8a\x83" "b\x83N\x83X\x83^\x81[", "\x83M\x83~\x83" "b\x83N-\x83W\x83\x83\x83t\x83@\x81[", "\x83M\x83~\x83" "b\x83N-\x83u\x83M\x81[", "\x83M\x83~\x83" "b\x83N-\x83p\x83\x89\x83T\x83" "C\x83g\x83P\x83" "C\x83W", "\x83M\x83~\x83" "b\x83N-\x83" "A\x81[\x83X\x83\x89", "\x83M\x83~\x83" "b\x83N-\x83}\x83\x8c\x83t\x83" "B\x83Z\x83\x93\x83g\x83h\x83\x89\x83S\x83\x93", "\x83M\x83~\x83" "b\x83N-\x83_\x81[\x83N\x83T\x83" "C\x83h"};

static char* szJMSwitchTbl[2] = {"", "\x8dX\x90V"};

static DEBUG_WINDOW_LINE debugWinJiminyLine[82] = {
    {"\xf9S----------\xf9V\x83\\\x83\x89\xf9S---------", &CDebugWindow::none, 1, {0}, 2, 0.0f, 0.0f, 0.0f, NULL, 0, NULL, NULL},
    {"\x83g\x83\x89\x83\x94\x83@\x81[\x83X       :%s", &DebugWork::g_dw.jiminy.Sora_World, 4, {0}, 0, 0.0f, 0.0f, 0.0f, szOffOn, 1, DWTEvent::DebugSetJiminyMemoFlag, NULL},
    {"\x83" "A\x83O\x83\x89\x83o\x81[         :%s", &DebugWork::g_dw.jiminy.Sora_World, 4, {0}, 0, 0.0f, 0.0f, 0.0f, szOffOn, 2, DWTEvent::DebugSetJiminyMemoFlag, NULL},
    {"\x83I\x83\x8a\x83\x93\x83|\x83X         :%s", &DebugWork::g_dw.jiminy.Sora_World, 4, {0}, 0, 0.0f, 0.0f, 0.0f, szOffOn, 4, DWTEvent::DebugSetJiminyMemoFlag, NULL},
    {"\x83\x8f\x83\x93\x83_\x81[\x83\x89\x83\x93\x83h     :%s", &DebugWork::g_dw.jiminy.Sora_World, 4, {0}, 0, 0.0f, 0.0f, 0.0f, szOffOn, 8, DWTEvent::DebugSetJiminyMemoFlag, NULL},
    {"\x83\x82\x83\x93\x83X\x83g\x83\x8d         :%s", &DebugWork::g_dw.jiminy.Sora_World, 4, {0}, 0, 0.0f, 0.0f, 0.0f, szOffOn, 0x10, DWTEvent::DebugSetJiminyMemoFlag, NULL},
    {"\x83n\x83\x8d\x83" "E\x83" "B\x83\x93\x83^\x83" "E\x83\x93   :%s", &DebugWork::g_dw.jiminy.Sora_World, 4, {0}, 0, 0.0f, 0.0f, 0.0f, szOffOn, 0x20, DWTEvent::DebugSetJiminyMemoFlag, NULL},
    {"\x83" "A\x83g\x83\x89\x83\x93\x83" "e\x83" "B\x83J     :%s", &DebugWork::g_dw.jiminy.Sora_World, 4, {0}, 0, 0.0f, 0.0f, 0.0f, szOffOn, 0x40, DWTEvent::DebugSetJiminyMemoFlag, NULL},
    {"\x83l\x83o\x81[\x83\x89\x83\x93\x83h       :%s", &DebugWork::g_dw.jiminy.Sora_World, 4, {0}, 0, 0.0f, 0.0f, 0.0f, szOffOn, 0x80, DWTEvent::DebugSetJiminyMemoFlag, NULL},
    {"\x83z\x83\x8d\x83" "E\x83o\x83X\x83" "e\x83" "B\x83I\x83\x93 :%s", &DebugWork::g_dw.jiminy.Sora_World, 4, {0}, 0, 0.0f, 0.0f, 0.0f, szOffOn, 0x100, DWTEvent::DebugSetJiminyMemoFlag, NULL},
    {"\x82P\x82O\x82O\x83G\x81[\x83J\x81[     :%s", &DebugWork::g_dw.jiminy.Sora_World, 4, {0}, 0, 0.0f, 0.0f, 0.0f, szOffOn, 0x200, DWTEvent::DebugSetJiminyMemoFlag, NULL},
    {"\x83g\x83\x8f\x83" "C\x83\x89\x83" "C\x83g       :%s", &DebugWork::g_dw.jiminy.Sora_World, 4, {0}, 0, 0.0f, 0.0f, 0.0f, szOffOn, 0x400, DWTEvent::DebugSetJiminyMemoFlag, NULL},
    {"\x83" "f\x83X\x83" "e\x83j\x81[         :%s", &DebugWork::g_dw.jiminy.Sora_World, 4, {0}, 0, 0.0f, 0.0f, 0.0f, szOffOn, 0x800, DWTEvent::DebugSetJiminyMemoFlag, NULL},
    {"\x82\xda\x82\xa4\x82\xab\x82\xe1\x82\xad\x82\xcc\x82\xb5\x82\xeb   :%s", &DebugWork::g_dw.jiminy.Sora_World, 4, {0}, 0, 0.0f, 0.0f, 0.0f, szOffOn, 0x1000, DWTEvent::DebugSetJiminyMemoFlag, NULL},
    {"\x81@\x82P\x82" "e             :%s", &DebugWork::g_dw.jiminy.Sora_Floor, 4, {0}, 0, 0.0f, 0.0f, 0.0f, szOffOn, 1, DWTEvent::DebugSetJiminyMemoFlag, NULL},
    {"\x81@\x82Q\x82" "e             :%s", &DebugWork::g_dw.jiminy.Sora_Floor, 4, {0}, 0, 0.0f, 0.0f, 0.0f, szOffOn, 2, DWTEvent::DebugSetJiminyMemoFlag, NULL},
    {"\x81@\x82R\x82" "e             :%s", &DebugWork::g_dw.jiminy.Sora_Floor, 4, {0}, 0, 0.0f, 0.0f, 0.0f, szOffOn, 4, DWTEvent::DebugSetJiminyMemoFlag, NULL},
    {"\x81@\x82S\x82" "e             :%s", &DebugWork::g_dw.jiminy.Sora_Floor, 4, {0}, 0, 0.0f, 0.0f, 0.0f, szOffOn, 8, DWTEvent::DebugSetJiminyMemoFlag, NULL},
    {"\x81@\x82T\x82" "e             :%s", &DebugWork::g_dw.jiminy.Sora_Floor, 4, {0}, 0, 0.0f, 0.0f, 0.0f, szOffOn, 0x10, DWTEvent::DebugSetJiminyMemoFlag, NULL},
    {"\x81@\x82U\x82" "e             :%s", &DebugWork::g_dw.jiminy.Sora_Floor, 4, {0}, 0, 0.0f, 0.0f, 0.0f, szOffOn, 0x20, DWTEvent::DebugSetJiminyMemoFlag, NULL},
    {"\x81@\x82V\x82" "e             :%s", &DebugWork::g_dw.jiminy.Sora_Floor, 4, {0}, 0, 0.0f, 0.0f, 0.0f, szOffOn, 0x40, DWTEvent::DebugSetJiminyMemoFlag, NULL},
    {"\x81@\x82W\x82" "e             :%s", &DebugWork::g_dw.jiminy.Sora_Floor, 4, {0}, 0, 0.0f, 0.0f, 0.0f, szOffOn, 0x80, DWTEvent::DebugSetJiminyMemoFlag, NULL},
    {"\x81@\x82X\x82" "e             :%s", &DebugWork::g_dw.jiminy.Sora_Floor, 4, {0}, 0, 0.0f, 0.0f, 0.0f, szOffOn, 0x100, DWTEvent::DebugSetJiminyMemoFlag, NULL},
    {"\x82P\x82O\x82" "e             :%s", &DebugWork::g_dw.jiminy.Sora_Floor, 4, {0}, 0, 0.0f, 0.0f, 0.0f, szOffOn, 0x200, DWTEvent::DebugSetJiminyMemoFlag, NULL},
    {"\x82P\x82P\x82" "e             :%s", &DebugWork::g_dw.jiminy.Sora_Floor, 4, {0}, 0, 0.0f, 0.0f, 0.0f, szOffOn, 0x400, DWTEvent::DebugSetJiminyMemoFlag, NULL},
    {"\x82P\x82Q\x82" "e             :%s", &DebugWork::g_dw.jiminy.Sora_Floor, 4, {0}, 0, 0.0f, 0.0f, 0.0f, szOffOn, 0x800, DWTEvent::DebugSetJiminyMemoFlag, NULL},
    {"\x82P\x82R\x82" "e             :%s", &DebugWork::g_dw.jiminy.Sora_Floor, 4, {0}, 0, 0.0f, 0.0f, 0.0f, szOffOn, 0x1000, DWTEvent::DebugSetJiminyMemoFlag, NULL},
    {"\xf9S----------\xf9V\x83\x8a\x83N\xf9S---------", &CDebugWindow::none, 1, {0}, 2, 0.0f, 0.0f, 0.0f, NULL, 0, NULL, NULL},
    {"\x83g\x83\x89\x83\x94\x83@\x81[\x83X       :%s", &DebugWork::g_dw.jiminy.Riku_World, 4, {0}, 0, 0.0f, 0.0f, 0.0f, szOffOn, 1, DWTEvent::DebugSetJiminyMemoFlag, NULL},
    {"\x83" "A\x83O\x83\x89\x83o\x81[         :%s", &DebugWork::g_dw.jiminy.Riku_World, 4, {0}, 0, 0.0f, 0.0f, 0.0f, szOffOn, 2, DWTEvent::DebugSetJiminyMemoFlag, NULL},
    {"\x83I\x83\x8a\x83\x93\x83|\x83X         :%s", &DebugWork::g_dw.jiminy.Riku_World, 4, {0}, 0, 0.0f, 0.0f, 0.0f, szOffOn, 4, DWTEvent::DebugSetJiminyMemoFlag, NULL},
    {"\x83\x8f\x83\x93\x83_\x81[\x83\x89\x83\x93\x83h     :%s", &DebugWork::g_dw.jiminy.Riku_World, 4, {0}, 0, 0.0f, 0.0f, 0.0f, szOffOn, 8, DWTEvent::DebugSetJiminyMemoFlag, NULL},
    {"\x83\x82\x83\x93\x83X\x83g\x83\x8d         :%s", &DebugWork::g_dw.jiminy.Riku_World, 4, {0}, 0, 0.0f, 0.0f, 0.0f, szOffOn, 0x10, DWTEvent::DebugSetJiminyMemoFlag, NULL},
    {"\x83n\x83\x8d\x83" "E\x83" "B\x83\x93\x83^\x83" "E\x83\x93   :%s", &DebugWork::g_dw.jiminy.Riku_World, 4, {0}, 0, 0.0f, 0.0f, 0.0f, szOffOn, 0x20, DWTEvent::DebugSetJiminyMemoFlag, NULL},
    {"\x83" "A\x83g\x83\x89\x83\x93\x83" "e\x83" "B\x83J     :%s", &DebugWork::g_dw.jiminy.Riku_World, 4, {0}, 0, 0.0f, 0.0f, 0.0f, szOffOn, 0x40, DWTEvent::DebugSetJiminyMemoFlag, NULL},
    {"\x83l\x83o\x81[\x83\x89\x83\x93\x83h       :%s", &DebugWork::g_dw.jiminy.Riku_World, 4, {0}, 0, 0.0f, 0.0f, 0.0f, szOffOn, 0x80, DWTEvent::DebugSetJiminyMemoFlag, NULL},
    {"\x83z\x83\x8d\x83" "E\x83o\x83X\x83" "e\x83" "B\x83I\x83\x93 :%s", &DebugWork::g_dw.jiminy.Riku_World, 4, {0}, 0, 0.0f, 0.0f, 0.0f, szOffOn, 0x100, DWTEvent::DebugSetJiminyMemoFlag, NULL},
    {"\x83g\x83\x8f\x83" "C\x83\x89\x83" "C\x83g       :%s", &DebugWork::g_dw.jiminy.Riku_World, 4, {0}, 0, 0.0f, 0.0f, 0.0f, szOffOn, 0x200, DWTEvent::DebugSetJiminyMemoFlag, NULL},
    {"\x83" "f\x83X\x83" "e\x83j\x81[         :%s", &DebugWork::g_dw.jiminy.Riku_World, 4, {0}, 0, 0.0f, 0.0f, 0.0f, szOffOn, 0x400, DWTEvent::DebugSetJiminyMemoFlag, NULL},
    {"\x82\xda\x82\xa4\x82\xab\x82\xe1\x82\xad\x82\xcc\x82\xb5\x82\xeb   :%s", &DebugWork::g_dw.jiminy.Riku_World, 4, {0}, 0, 0.0f, 0.0f, 0.0f, szOffOn, 0x800, DWTEvent::DebugSetJiminyMemoFlag, NULL},
    {"\x82" "a\x82P\x82Q\x82" "e           :%s", &DebugWork::g_dw.jiminy.Riku_Floor, 4, {0}, 0, 0.0f, 0.0f, 0.0f, szOffOn, 1, DWTEvent::DebugSetJiminyMemoFlag, NULL},
    {"\x82" "a\x82P\x82P\x82" "e           :%s", &DebugWork::g_dw.jiminy.Riku_Floor, 4, {0}, 0, 0.0f, 0.0f, 0.0f, szOffOn, 2, DWTEvent::DebugSetJiminyMemoFlag, NULL},
    {"\x82" "a\x82P\x82O\x82" "e           :%s", &DebugWork::g_dw.jiminy.Riku_Floor, 4, {0}, 0, 0.0f, 0.0f, 0.0f, szOffOn, 4, DWTEvent::DebugSetJiminyMemoFlag, NULL},
    {"\x82" "a\x81@\x82X\x82" "e           :%s", &DebugWork::g_dw.jiminy.Riku_Floor, 4, {0}, 0, 0.0f, 0.0f, 0.0f, szOffOn, 8, DWTEvent::DebugSetJiminyMemoFlag, NULL},
    {"\x82" "a\x81@\x82W\x82" "e           :%s", &DebugWork::g_dw.jiminy.Riku_Floor, 4, {0}, 0, 0.0f, 0.0f, 0.0f, szOffOn, 0x10, DWTEvent::DebugSetJiminyMemoFlag, NULL},
    {"\x82" "a\x81@\x82V\x82" "e           :%s", &DebugWork::g_dw.jiminy.Riku_Floor, 4, {0}, 0, 0.0f, 0.0f, 0.0f, szOffOn, 0x20, DWTEvent::DebugSetJiminyMemoFlag, NULL},
    {"\x82" "a\x81@\x82U\x82" "e           :%s", &DebugWork::g_dw.jiminy.Riku_Floor, 4, {0}, 0, 0.0f, 0.0f, 0.0f, szOffOn, 0x40, DWTEvent::DebugSetJiminyMemoFlag, NULL},
    {"\x82" "a\x81@\x82T\x82" "e           :%s", &DebugWork::g_dw.jiminy.Riku_Floor, 4, {0}, 0, 0.0f, 0.0f, 0.0f, szOffOn, 0x80, DWTEvent::DebugSetJiminyMemoFlag, NULL},
    {"\x82" "a\x81@\x82S\x82" "e           :%s", &DebugWork::g_dw.jiminy.Riku_Floor, 4, {0}, 0, 0.0f, 0.0f, 0.0f, szOffOn, 0x100, DWTEvent::DebugSetJiminyMemoFlag, NULL},
    {"\x82" "a\x81@\x82R\x82" "e           :%s", &DebugWork::g_dw.jiminy.Riku_Floor, 4, {0}, 0, 0.0f, 0.0f, 0.0f, szOffOn, 0x200, DWTEvent::DebugSetJiminyMemoFlag, NULL},
    {"\x82" "a\x81@\x82Q\x82" "e           :%s", &DebugWork::g_dw.jiminy.Riku_Floor, 4, {0}, 0, 0.0f, 0.0f, 0.0f, szOffOn, 0x400, DWTEvent::DebugSetJiminyMemoFlag, NULL},
    {"\x82" "a\x81@\x82P\x82" "e           :%s", &DebugWork::g_dw.jiminy.Riku_Floor, 4, {0}, 0, 0.0f, 0.0f, 0.0f, szOffOn, 0x800, DWTEvent::DebugSetJiminyMemoFlag, NULL},
    {"\xf9S------\xf9V\x83\\\x83\x89 \x82\xbb\x82\xcc\x91\xbc\xf9S------", &CDebugWindow::none, 1, {0}, 2, 0.0f, 0.0f, 0.0f, NULL, 0, NULL, NULL},
    {"\x83~\x83j\x83Q\x81[\x83\x80\x81" "E\x96\xec\x8d\xd8   :%s", &DebugWork::g_dw.jiminy.Sora_Etc, 4, {0}, 0, 0.0f, 0.0f, 0.0f, szOffOn, 1, DWTEvent::DebugSetJiminyMemoFlag, NULL},
    {"\x83~\x83j\x83Q\x81[\x83\x80\x81" "E\x83o\x83\x8b   :%s", &DebugWork::g_dw.jiminy.Sora_Etc, 4, {0}, 0, 0.0f, 0.0f, 0.0f, szOffOn, 2, DWTEvent::DebugSetJiminyMemoFlag, NULL},
    {"\x83~\x83j\x83Q\x81[\x83\x80\x81" "E\x83W\x83\x83   :%s", &DebugWork::g_dw.jiminy.Sora_Etc, 4, {0}, 0, 0.0f, 0.0f, 0.0f, szOffOn, 4, DWTEvent::DebugSetJiminyMemoFlag, NULL},
    {"\x83~\x83j\x83Q\x81[\x83\x80\x81" "E\x83\x8f\x83" "C   :%s", &DebugWork::g_dw.jiminy.Sora_Etc, 4, {0}, 0, 0.0f, 0.0f, 0.0f, szOffOn, 8, DWTEvent::DebugSetJiminyMemoFlag, NULL},
    {"\x83~\x83j\x83Q\x81[\x83\x80\x81" "E\x83n\x83`   :%s", &DebugWork::g_dw.jiminy.Sora_Etc, 4, {0}, 0, 0.0f, 0.0f, 0.0f, szOffOn, 0x10, DWTEvent::DebugSetJiminyMemoFlag, NULL},
    {"\x83\x82\x83\x93\x83X\x83g\x83\x8d\x82\xc5\x91\xe5\x96\\\x82\xea :%s", &DebugWork::g_dw.jiminy.Sora_Etc, 4, {0}, 0, 0.0f, 0.0f, 0.0f, szOffOn, 0x20, DWTEvent::DebugSetJiminyMemoFlag, NULL},
    {"\x83\x82\x81[\x83O\x83\x8a\x83V\x83\x87\x83" "b\x83v   :%s", &DebugWork::g_dw.jiminy.Sora_Etc, 4, {0}, 0, 0.0f, 0.0f, 0.0f, szOffOn, 0x40, DWTEvent::DebugSetJiminyMemoFlag, NULL},
    {"\x83s\x83O\x83\x8c\x83" "b\x83g\x82\xc6\x98" "b\x82\xb5\x82\xbd :%s", &DebugWork::g_dw.jiminy.Sora_Etc, 4, {0}, 0, 0.0f, 0.0f, 0.0f, szOffOn, 0x80, DWTEvent::DebugSetJiminyMemoFlag, NULL},
    {"\x83I\x83" "E\x83\x8b\x82\xc6\x98" "b\x82\xb5\x82\xbd     :%s", &DebugWork::g_dw.jiminy.Sora_Etc, 4, {0}, 0, 0.0f, 0.0f, 0.0f, szOffOn, 0x100, DWTEvent::DebugSetJiminyMemoFlag, NULL},
    {"\x83\x8b\x81[\x82\xc6\x98" "b\x82\xb5\x82\xbd       :%s", &DebugWork::g_dw.jiminy.Sora_Etc, 4, {0}, 0, 0.0f, 0.0f, 0.0f, szOffOn, 0x200, DWTEvent::DebugSetJiminyMemoFlag, NULL},
    {"\x83" "C\x81[\x83\x88\x81[\x82\xc6\x98" "b\x82\xb5\x82\xbd   :%s", &DebugWork::g_dw.jiminy.Sora_Etc, 4, {0}, 0, 0.0f, 0.0f, 0.0f, szOffOn, 0x400, DWTEvent::DebugSetJiminyMemoFlag, NULL},
    {"\x83" "e\x83" "B\x83K\x81[\x82\xc6\x98" "b\x82\xb5\x82\xbd   :%s", &DebugWork::g_dw.jiminy.Sora_Etc, 4, {0}, 0, 0.0f, 0.0f, 0.0f, szOffOn, 0x800, DWTEvent::DebugSetJiminyMemoFlag, NULL},
    {"\x83\x89\x83r\x83" "b\x83g\x82\xc6\x98" "b\x82\xb5\x82\xbd   :%s", &DebugWork::g_dw.jiminy.Sora_Etc, 4, {0}, 0, 0.0f, 0.0f, 0.0f, szOffOn, 0x1000, DWTEvent::DebugSetJiminyMemoFlag, NULL},
    {"100\x83G\x81[\x83J\x81[\x82\xcc\x90X\x8fo\x82\xbd:%s", &DebugWork::g_dw.jiminy.Sora_Etc, 4, {0}, 0, 0.0f, 0.0f, 0.0f, szOffOn, 0x2000, DWTEvent::DebugSetJiminyMemoFlag, NULL},
    {"\xf9S------\xf9V\x83\\\x83\x89 \x93G\x82\xcc\x90\x94\xf9S------", &CDebugWindow::none, 1, {0}, 2, 0.0f, 0.0f, 0.0f, NULL, 0, NULL, NULL},
    {"\x93G\x82\xcc\x8e\xed\x97\xde           :%s", &DebugWork::g_dw.jiminy.Sora_Enemy, 4, {0}, 0, 0.0f, 34.0f, 1.0f, DWTEvent::szJMSoraEnemyKind, 0, DWTEvent::DebugSetJiminyMemoFlag, NULL},
    {"\x93G\x82\xcc\x90\x94             :%5d", &DebugWork::g_dw.jiminy.Sora_ENum, 4, {0}, 0, 0.0f, 10000.0f, 100.0f, NULL, 0, DWTEvent::DebugSetJiminyMemoFlag, NULL},
    {"\x81\x9b\x83{\x83^\x83\x93\x82\xc5\x90\xdd\x92\xe8     :%s", &DebugWork::g_dw.jiminy.Sora_Flag, 4, {0}, 0, 0.0f, 0.0f, 0.0f, DWTEvent::szJMSwitchTbl, 1, DWTEvent::DebugSetJiminyMemoFlag, NULL},
    {"\xf9S------\xf9V\x83\x8a\x83N \x93G\x82\xcc\x90\x94\xf9S------", &CDebugWindow::none, 1, {0}, 2, 0.0f, 0.0f, 0.0f, NULL, 0, NULL, NULL},
    {"\x93G\x82\xcc\x8e\xed\x97\xde           :%s", &DebugWork::g_dw.jiminy.Riku_Enemy, 4, {0}, 0, 0.0f, 33.0f, 1.0f, DWTEvent::szJMRikuEnemyKind, 0, DWTEvent::DebugSetJiminyMemoFlag, NULL},
    {"\x93G\x82\xcc\x90\x94             :%5d", &DebugWork::g_dw.jiminy.Riku_ENum, 4, {0}, 0, 0.0f, 10000.0f, 100.0f, NULL, 0, DWTEvent::DebugSetJiminyMemoFlag, NULL},
    {"\x81\x9b\x83{\x83^\x83\x93\x82\xc5\x90\xdd\x92\xe8     :%s", &DebugWork::g_dw.jiminy.Riku_Flag, 4, {0}, 0, 0.0f, 0.0f, 0.0f, DWTEvent::szJMSwitchTbl, 1, DWTEvent::DebugSetJiminyMemoFlag, NULL},
    {"\xf9S------\xf9V\x83\\\x83\x89 \x83J\x81[\x83h\xf9S------", &CDebugWindow::none, 1, {0}, 2, 0.0f, 0.0f, 0.0f, NULL, 0, NULL, NULL},
    {"\x83J\x81[\x83h\x82\xcc\x8e\xed\x97\xde       :%s", &DebugWork::g_dw.jiminy.Sora_Card, 4, {0}, 0, 0.0f, 158.0f, 1.0f, DWTEvent::szJMSoraCardKindTbl, 0, DWTEvent::DebugSetJiminyMemoFlag, NULL},
    {"\x81\x9b\x83{\x83^\x83\x93\x82\xc5\x8e\xe6\x93\xbe     :%s", &DebugWork::g_dw.jiminy.Sora_Flag, 4, {0}, 0, 0.0f, 0.0f, 0.0f, DWTEvent::szJMSwitchTbl, 2, DWTEvent::DebugSetJiminyMemoFlag, NULL},
    {"\xf9S------\xf9V\x83\x8a\x83N \x83J\x81[\x83h\xf9S------", &CDebugWindow::none, 1, {0}, 2, 0.0f, 0.0f, 0.0f, NULL, 0, NULL, NULL},
    {"\x83J\x81[\x83h\x82\xcc\x8e\xed\x97\xde       :%s", &DebugWork::g_dw.jiminy.Riku_Card, 4, {0}, 0, 0.0f, 65.0f, 1.0f, DWTEvent::szJMRikuCardKindTbl, 0, DWTEvent::DebugSetJiminyMemoFlag, NULL},
    {"\x81\x9b\x83{\x83^\x83\x93\x82\xc5\x8e\xe6\x93\xbe     :%s", &DebugWork::g_dw.jiminy.Riku_Flag, 4, {0}, 0, 0.0f, 0.0f, 0.0f, DWTEvent::szJMSwitchTbl, 2, DWTEvent::DebugSetJiminyMemoFlag, NULL},
    {NULL, NULL, 0, {0}, 0, 0.0f, 0.0f, 0.0f, NULL, 0, NULL, NULL},
};

static DEBUG_WINDOW_TBL debugWinJiminy = {"\x83W\x83~\x83j\x81[\x83\x81\x83\x82", -106, -(SCREEN_HEIGHT >> 1) + 150, 15, 25, 12, 0, 0, DWTEvent::debugWinJiminyLine, NULL, NULL};

static DEBUG_WINDOW_LINE debugWinKH2CardGetLine[14] = {
    {"\x83[\x83\x80\x83i\x83X           :%s", &DebugWork::g_dw.atelwork.iKH2CardGet, 4, {0}, 0, 0.0f, 0.0f, 0.0f, szOffOn, 1, DWTEvent::DebugSetKH2CardGetFlag, NULL},
    {"\x83V\x83\x8b\x83o\x81[\x83\x8b         :%s", &DebugWork::g_dw.atelwork.iKH2CardGet, 4, {0}, 0, 0.0f, 0.0f, 0.0f, szOffOn, 2, DWTEvent::DebugSetKH2CardGetFlag, NULL},
    {"\x83U\x83\x8b\x83" "f\x83" "B\x83\x93         :%s", &DebugWork::g_dw.atelwork.iKH2CardGet, 4, {0}, 0, 0.0f, 0.0f, 0.0f, szOffOn, 4, DWTEvent::DebugSetKH2CardGetFlag, NULL},
    {"\x83T\x83" "C\x83N\x83X           :%s", &DebugWork::g_dw.atelwork.iKH2CardGet, 4, {0}, 0, 0.0f, 0.0f, 0.0f, szOffOn, 8, DWTEvent::DebugSetKH2CardGetFlag, NULL},
    {"\x83" "f\x83~\x83" "b\x83N\x83X         :%s", &DebugWork::g_dw.atelwork.iKH2CardGet, 4, {0}, 0, 0.0f, 0.0f, 0.0f, szOffOn, 0x10, DWTEvent::DebugSetKH2CardGetFlag, NULL},
    {"\x83\x8b\x83N\x83\\\x81[\x83h         :%s", &DebugWork::g_dw.atelwork.iKH2CardGet, 4, {0}, 0, 0.0f, 0.0f, 0.0f, szOffOn, 0x20, DWTEvent::DebugSetKH2CardGetFlag, NULL},
    {"\x83\x8d\x83N\x83T\x83X           :%s", &DebugWork::g_dw.atelwork.iKH2CardGet, 4, {0}, 0, 0.0f, 0.0f, 0.0f, szOffOn, 0x40, DWTEvent::DebugSetKH2CardGetFlag, NULL},
    {"\x83X\x83^\x81[\x83V\x81[\x83J\x81[     :%s", &DebugWork::g_dw.atelwork.iKH2CardGet, 4, {0}, 0, 0.0f, 0.0f, 0.0f, szOffOn, 0x80, DWTEvent::DebugSetKH2CardGetFlag, NULL},
    {"\x83\x82\x83m\x83N\x83\x8d\x81[\x83\x80\x09    :%s", &DebugWork::g_dw.atelwork.iKH2CardGet, 4, {0}, 0, 0.0f, 0.0f, 0.0f, szOffOn, 0x100, DWTEvent::DebugSetKH2CardGetFlag, NULL},
    {"\x83t\x83H\x83\x8d\x81[\x83U\x83" "E\x83" "B\x83\x93\x83h :%s", &DebugWork::g_dw.atelwork.iKH2CardGet, 4, {0}, 0, 0.0f, 0.0f, 0.0f, szOffOn, 0x200, DWTEvent::DebugSetKH2CardGetFlag, NULL},
    {"\x83q\x83h\x83" "D\x83\x93\x83h\x83\x89\x83S\x83\x93   :%s", &DebugWork::g_dw.atelwork.iKH2CardGet, 4, {0}, 0, 0.0f, 0.0f, 0.0f, szOffOn, 0x400, DWTEvent::DebugSetKH2CardGetFlag, NULL},
    {"\x83t\x83H\x83g\x83\x93\x83" "f\x83o\x83" "b\x83K\x81[ :%s", &DebugWork::g_dw.atelwork.iKH2CardGet, 4, {0}, 0, 0.0f, 0.0f, 0.0f, szOffOn, 0x800, DWTEvent::DebugSetKH2CardGetFlag, NULL},
    {"\x83{\x83\x93\x83h\x83I\x83u\x83t\x83\x8c\x83" "C\x83\x80 :%s", &DebugWork::g_dw.atelwork.iKH2CardGet, 4, {0}, 0, 0.0f, 0.0f, 0.0f, szOffOn, 0x1000, DWTEvent::DebugSetKH2CardGetFlag, NULL},
    {NULL, NULL, 0, {0}, 0, 0.0f, 0.0f, 0.0f, NULL, 0, NULL, NULL},
};

static DEBUG_WINDOW_TBL debugWinKH2CardGet = {"\x82j\x82g\x82Q\x83J\x81[\x83h\x93\xfc\x8e\xe8", -106, -(SCREEN_HEIGHT >> 1) + 150, 15, 13, 12, 0, 0, DWTEvent::debugWinKH2CardGetLine, NULL, DWTEvent::DebugAtelWorkDef};

static BOOL debugGameInfomationParam(CDebugWindow* pObj, INT inY, INT inParam) {
    if (inParam & 0x180) {
        switch (inY) {
        case 1:
            CGameWork::setFlag(0, 0x83, DebugWork::g_dw.atelwork.iDistinctionClear);
            break;
        case 2:
            CGameWork::setFlag(0, 0x11, DebugWork::g_dw.atelwork.iGameKH2clear);
            break;
        case 3:
            CGameWork::setFlag(0, 0x9, DebugWork::g_dw.atelwork.iSoraClear);
            break;
        case 4:
            CGameWork::setFlag(0, 0x12, DebugWork::g_dw.atelwork.iRikuClear);
            break;
        case 7:
            CGameWork::setFlag(0, 0x82, DebugWork::g_dw.atelwork.iMahluciaKilled);
            break;
        case 8:
            CGameWork::setByte(0, 0x61, DebugWork::g_dw.atelwork.iPlayDifficultMode);
            break;
        }
    }
    return TRUE;
}

static DEBUG_WINDOW_LINE debugGameInfomationLine[12] = {
    {"\x83v\x83\x8c\x83" "C\x83\x82\x81[\x83h         : %s", &DebugWork::g_dw.atelwork.iPlayMode, 1, {0}, 0, 0.0f, 0.0f, 0.0f, &szPlayMode[DebugWork::g_dw.atelwork.iPlayMode], 0, NULL, NULL},
    {"\x83R\x83x\x83" "c\x83Q\x81[\x83\x80\x83N\x83\x8a\x83" "A   : %s", &DebugWork::g_dw.atelwork.iDistinctionClear, 1, {0}, 0, 0.0f, 1.0f, 1.0f, szOffOn, 0, DWTEvent::debugGameInfomationParam, NULL},
    {"\x82j\x82g\x82Q\x83N\x83\x8a\x83" "A         : %s", &DebugWork::g_dw.atelwork.iGameKH2clear, 1, {0}, 0, 0.0f, 1.0f, 1.0f, szOffOn, 0, DWTEvent::debugGameInfomationParam, NULL},
    {"\x83\\\x83\x89\x83N\x83\x8a\x83" "A           : %s", &DebugWork::g_dw.atelwork.iSoraClear, 1, {0}, 0, 0.0f, 1.0f, 1.0f, szOffOn, 0, DWTEvent::debugGameInfomationParam, NULL},
    {"\x83\x8a\x83N\x83N\x83\x8a\x83" "A           : %s", &DebugWork::g_dw.atelwork.iRikuClear, 1, {0}, 0, 0.0f, 1.0f, 1.0f, szOffOn, 0, DWTEvent::debugGameInfomationParam, NULL},
    {"\x83\\\x83\x89" "EN\x83t\x83\x89\x83O         : %d", &DebugWork::g_dw.atelwork.iEnSoraEventStep, 4, {0}, 0, 0.0f, 0.0f, 0.0f, 0, 0, NULL, NULL},
    {"\x83\x8a\x83NEN\x83t\x83\x89\x83O         : %d", &DebugWork::g_dw.atelwork.iEnRikuEventStep, 4, {0}, 0, 0.0f, 0.0f, 0.0f, 0, 0, NULL, NULL},
    {"\x83}\x81[\x83\x8b\x81[\x83V\x83\x83KILL     : %s", &DebugWork::g_dw.atelwork.iMahluciaKilled, 1, {0}, 0, 0.0f, 1.0f, 1.0f, szOffOn, 0, DWTEvent::debugGameInfomationParam, NULL},
    {"\x83v\x83\x8c\x83" "C\x82\xc8\x82\xf1\x82\xa2\x82\xc7       : %s", &DebugWork::g_dw.atelwork.iPlayDifficultMode, 1, {0}, 0, 0.0f, 3.0f, 1.0f, &szDifficultMode[DebugWork::g_dw.atelwork.iPlayDifficultMode], 0, DWTEvent::debugGameInfomationParam, NULL},
    {"\x83T\x83" "E\x83\x93\x83h\x83\x82\x81[\x83h       : %s", &DebugWork::g_dw.atelwork.iSoundMode, 1, {0}, 0, 0.0f, 3.0f, 1.0f, &szSoundMode[DebugWork::g_dw.atelwork.iSoundMode], 0, NULL, NULL},
    {"\x83o\x83" "C\x83u\x83\x8c\x81[\x83V\x83\x87\x83\x93     : %s", &DebugWork::g_dw.atelwork.iVibrat, 1, {0}, 0, 0.0f, 3.0f, 1.0f, szOnOff, 0, NULL, NULL},
    {NULL, NULL, 0, {0}, 0, 0.0f, 0.0f, 0.0f, 0, 0, NULL, NULL},
};

static DEBUG_WINDOW_TBL debugGameInfomation = {"\x83Q\x81[\x83\x80\x8f\xee\x95\xf1", -106, -(SCREEN_HEIGHT >> 1) + 150, 20, 13, 12, 0, 0, DWTEvent::debugGameInfomationLine, NULL, DWTEvent::DebugAtelWorkDef};

static DEBUG_WINDOW_LINE debugWinTuEndLine[8] = {
    {"\x83L\x81[\x83\x8b\x81[\x83\x80        :%s", &DebugWork::g_dw.atelwork.iTutorialEnd, 4, {0}, 0, 0.0f, 0.0f, 0.0f, szOffOn, 1, DWTEvent::DebugSetTutorialFlag, NULL},
    {"\x83\x82\x81[\x83O\x83\x8a\x83V\x83\x87\x83" "b\x83v  :%s", &DebugWork::g_dw.atelwork.iTutorialEnd, 4, {0}, 0, 0.0f, 0.0f, 0.0f, szOffOn, 2, DWTEvent::DebugSetTutorialFlag, NULL},
    {"\x83t\x83\x8d\x83" "A\x88\xda\x93\xae        :%s", &DebugWork::g_dw.atelwork.iTutorialEnd, 4, {0}, 0, 0.0f, 0.0f, 0.0f, szOffOn, 4, DWTEvent::DebugSetTutorialFlag, NULL},
    {"\x83\x8f\x81[\x83v\x83|\x83" "C\x83\x93\x83g    :%s", &DebugWork::g_dw.atelwork.iTutorialEnd, 4, {0}, 0, 0.0f, 0.0f, 0.0f, szOffOn, 8, DWTEvent::DebugSetTutorialFlag, NULL},
    {"\x83Z\x81[\x83u\x83|\x83" "C\x83\x93\x83g    :%s", &DebugWork::g_dw.atelwork.iTutorialEnd, 4, {0}, 0, 0.0f, 0.0f, 0.0f, szOffOn, 0x10, DWTEvent::DebugSetTutorialFlag, NULL},
    {"\x83t\x83" "B\x81[\x83\x8b\x83h        :%s", &DebugWork::g_dw.atelwork.iTutorialEnd, 4, {0}, 0, 0.0f, 0.0f, 0.0f, szOffOn, 0x20, DWTEvent::DebugSetTutorialFlag, NULL},
    {"\x83\x8f\x81[\x83\x8b\x83h\x83Z\x83\x8c\x83N\x83g  :%s", &DebugWork::g_dw.atelwork.iTutorialEnd, 4, {0}, 0, 0.0f, 0.0f, 0.0f, szOffOn, 0x40, DWTEvent::DebugSetTutorialFlag, NULL},
    {NULL, NULL, 0, {0}, 0, 0.0f, 0.0f, 0.0f, NULL, 0, NULL, NULL},
};

static DEBUG_WINDOW_TBL debugWinTuEnd = {"\x83`\x83\x85\x81[\x83g\x83\x8a\x83" "A\x83\x8b\x8fI\x97\xb9", -106, -(SCREEN_HEIGHT >> 1) + 150, 15, 7, 12, 0, 0, DWTEvent::debugWinTuEndLine, NULL, DWTEvent::DebugAtelWorkDef};

static DEBUG_WINDOW_LINE debugWinMovieLineRiku[42] = {
    {"501   :%s", &DebugWork::g_dw.atelwork.iRikuMovie0, 4, {0}, 0, 0.0f, 0.0f, 0.0f, szOffOn, 1, DWTEvent::DebugSetRikuMovieFlag, NULL},
    {"502   :%s", &DebugWork::g_dw.atelwork.iRikuMovie0, 4, {0}, 0, 0.0f, 0.0f, 0.0f, szOffOn, 2, DWTEvent::DebugSetRikuMovieFlag, NULL},
    {"503   :%s", &DebugWork::g_dw.atelwork.iRikuMovie0, 4, {0}, 0, 0.0f, 0.0f, 0.0f, szOffOn, 4, DWTEvent::DebugSetRikuMovieFlag, NULL},
    {"504   :%s", &DebugWork::g_dw.atelwork.iRikuMovie0, 4, {0}, 0, 0.0f, 0.0f, 0.0f, szOffOn, 8, DWTEvent::DebugSetRikuMovieFlag, NULL},
    {"511   :%s", &DebugWork::g_dw.atelwork.iRikuMovie0, 4, {0}, 0, 0.0f, 0.0f, 0.0f, szOffOn, 0x10, DWTEvent::DebugSetRikuMovieFlag, NULL},
    {"512   :%s", &DebugWork::g_dw.atelwork.iRikuMovie0, 4, {0}, 0, 0.0f, 0.0f, 0.0f, szOffOn, 0x20, DWTEvent::DebugSetRikuMovieFlag, NULL},
    {"521   :%s", &DebugWork::g_dw.atelwork.iRikuMovie0, 4, {0}, 0, 0.0f, 0.0f, 0.0f, szOffOn, 0x40, DWTEvent::DebugSetRikuMovieFlag, NULL},
    {"522   :%s", &DebugWork::g_dw.atelwork.iRikuMovie0, 4, {0}, 0, 0.0f, 0.0f, 0.0f, szOffOn, 0x80, DWTEvent::DebugSetRikuMovieFlag, NULL},
    {"531   :%s", &DebugWork::g_dw.atelwork.iRikuMovie0, 4, {0}, 0, 0.0f, 0.0f, 0.0f, szOffOn, 0x100, DWTEvent::DebugSetRikuMovieFlag, NULL},
    {"541   :%s", &DebugWork::g_dw.atelwork.iRikuMovie0, 4, {0}, 0, 0.0f, 0.0f, 0.0f, szOffOn, 0x200, DWTEvent::DebugSetRikuMovieFlag, NULL},
    {"542   :%s", &DebugWork::g_dw.atelwork.iRikuMovie0, 4, {0}, 0, 0.0f, 0.0f, 0.0f, szOffOn, 0x400, DWTEvent::DebugSetRikuMovieFlag, NULL},
    {"543   :%s", &DebugWork::g_dw.atelwork.iRikuMovie0, 4, {0}, 0, 0.0f, 0.0f, 0.0f, szOffOn, 0x800, DWTEvent::DebugSetRikuMovieFlag, NULL},
    {"551   :%s", &DebugWork::g_dw.atelwork.iRikuMovie0, 4, {0}, 0, 0.0f, 0.0f, 0.0f, szOffOn, 0x1000, DWTEvent::DebugSetRikuMovieFlag, NULL},
    {"552   :%s", &DebugWork::g_dw.atelwork.iRikuMovie0, 4, {0}, 0, 0.0f, 0.0f, 0.0f, szOffOn, 0x2000, DWTEvent::DebugSetRikuMovieFlag, NULL},
    {"561   :%s", &DebugWork::g_dw.atelwork.iRikuMovie0, 4, {0}, 0, 0.0f, 0.0f, 0.0f, szOffOn, 0x4000, DWTEvent::DebugSetRikuMovieFlag, NULL},
    {"571   :%s", &DebugWork::g_dw.atelwork.iRikuMovie0, 4, {0}, 0, 0.0f, 0.0f, 0.0f, szOffOn, 0x8000, DWTEvent::DebugSetRikuMovieFlag, NULL},
    {"581   :%s", &DebugWork::g_dw.atelwork.iRikuMovie0, 4, {0}, 0, 0.0f, 0.0f, 0.0f, szOffOn, 0x10000, DWTEvent::DebugSetRikuMovieFlag, NULL},
    {"582   :%s", &DebugWork::g_dw.atelwork.iRikuMovie0, 4, {0}, 0, 0.0f, 0.0f, 0.0f, szOffOn, 0x20000, DWTEvent::DebugSetRikuMovieFlag, NULL},
    {"583   :%s", &DebugWork::g_dw.atelwork.iRikuMovie0, 4, {0}, 0, 0.0f, 0.0f, 0.0f, szOffOn, 0x40000, DWTEvent::DebugSetRikuMovieFlag, NULL},
    {"584   :%s", &DebugWork::g_dw.atelwork.iRikuMovie0, 4, {0}, 0, 0.0f, 0.0f, 0.0f, szOffOn, 0x80000, DWTEvent::DebugSetRikuMovieFlag, NULL},
    {"585   :%s", &DebugWork::g_dw.atelwork.iRikuMovie0, 4, {0}, 0, 0.0f, 0.0f, 0.0f, szOffOn, 0x100000, DWTEvent::DebugSetRikuMovieFlag, NULL},
    {"586   :%s", &DebugWork::g_dw.atelwork.iRikuMovie0, 4, {0}, 0, 0.0f, 0.0f, 0.0f, szOffOn, 0x200000, DWTEvent::DebugSetRikuMovieFlag, NULL},
    {"591   :%s", &DebugWork::g_dw.atelwork.iRikuMovie0, 4, {0}, 0, 0.0f, 0.0f, 0.0f, szOffOn, 0x400000, DWTEvent::DebugSetRikuMovieFlag, NULL},
    {"592   :%s", &DebugWork::g_dw.atelwork.iRikuMovie0, 4, {0}, 0, 0.0f, 0.0f, 0.0f, szOffOn, 0x800000, DWTEvent::DebugSetRikuMovieFlag, NULL},
    {"593   :%s", &DebugWork::g_dw.atelwork.iRikuMovie0, 4, {0}, 0, 0.0f, 0.0f, 0.0f, szOffOn, 0x1000000, DWTEvent::DebugSetRikuMovieFlag, NULL},
    {"594   :%s", &DebugWork::g_dw.atelwork.iRikuMovie0, 4, {0}, 0, 0.0f, 0.0f, 0.0f, szOffOn, 0x2000000, DWTEvent::DebugSetRikuMovieFlag, NULL},
    {"595   :%s", &DebugWork::g_dw.atelwork.iRikuMovie0, 4, {0}, 0, 0.0f, 0.0f, 0.0f, szOffOn, 0x4000000, DWTEvent::DebugSetRikuMovieFlag, NULL},
    {"596   :%s", &DebugWork::g_dw.atelwork.iRikuMovie0, 4, {0}, 0, 0.0f, 0.0f, 0.0f, szOffOn, 0x8000000, DWTEvent::DebugSetRikuMovieFlag, NULL},
    {"601   :%s", &DebugWork::g_dw.atelwork.iRikuMovie0, 4, {0}, 0, 0.0f, 0.0f, 0.0f, szOffOn, 0x10000000, DWTEvent::DebugSetRikuMovieFlag, NULL},
    {"602   :%s", &DebugWork::g_dw.atelwork.iRikuMovie0, 4, {0}, 0, 0.0f, 0.0f, 0.0f, szOffOn, 0x20000000, DWTEvent::DebugSetRikuMovieFlag, NULL},
    {"603   :%s", &DebugWork::g_dw.atelwork.iRikuMovie0, 4, {0}, 0, 0.0f, 0.0f, 0.0f, szOffOn, 0x40000000, DWTEvent::DebugSetRikuMovieFlag, NULL},
    {"604   :%s", &DebugWork::g_dw.atelwork.iRikuMovie0, 4, {0}, 0, 0.0f, 0.0f, 0.0f, szOffOn, 0x80000000, DWTEvent::DebugSetRikuMovieFlag, NULL},
    {"605   :%s", &DebugWork::g_dw.atelwork.iRikuMovie1, 4, {0}, 0, 0.0f, 0.0f, 0.0f, szOffOn, 1, DWTEvent::DebugSetRikuMovieFlag, NULL},
    {"606   :%s", &DebugWork::g_dw.atelwork.iRikuMovie1, 4, {0}, 0, 0.0f, 0.0f, 0.0f, szOffOn, 2, DWTEvent::DebugSetRikuMovieFlag, NULL},
    {"611   :%s", &DebugWork::g_dw.atelwork.iRikuMovie1, 4, {0}, 0, 0.0f, 0.0f, 0.0f, szOffOn, 4, DWTEvent::DebugSetRikuMovieFlag, NULL},
    {"612   :%s", &DebugWork::g_dw.atelwork.iRikuMovie1, 4, {0}, 0, 0.0f, 0.0f, 0.0f, szOffOn, 8, DWTEvent::DebugSetRikuMovieFlag, NULL},
    {"613   :%s", &DebugWork::g_dw.atelwork.iRikuMovie1, 4, {0}, 0, 0.0f, 0.0f, 0.0f, szOffOn, 0x10, DWTEvent::DebugSetRikuMovieFlag, NULL},
    {"614   :%s", &DebugWork::g_dw.atelwork.iRikuMovie1, 4, {0}, 0, 0.0f, 0.0f, 0.0f, szOffOn, 0x20, DWTEvent::DebugSetRikuMovieFlag, NULL},
    {"615   :%s", &DebugWork::g_dw.atelwork.iRikuMovie1, 4, {0}, 0, 0.0f, 0.0f, 0.0f, szOffOn, 0x40, DWTEvent::DebugSetRikuMovieFlag, NULL},
    {"621   :%s", &DebugWork::g_dw.atelwork.iRikuMovie1, 4, {0}, 0, 0.0f, 0.0f, 0.0f, szOffOn, 0x80, DWTEvent::DebugSetRikuMovieFlag, NULL},
    {"631   :%s", &DebugWork::g_dw.atelwork.iRikuMovie1, 4, {0}, 0, 0.0f, 0.0f, 0.0f, szOffOn, 0x100, DWTEvent::DebugSetRikuMovieFlag, NULL},
    {NULL, NULL, 0, {0}, 0, 0.0f, 0.0f, 0.0f, NULL, 0, NULL, NULL},
};

static DEBUG_WINDOW_TBL debugWinMovieRiku = {"\x83\x80\x81[\x83r\x81[", -106, -(SCREEN_HEIGHT >> 1) + 150, 15, 20, 12, 0, 0, DWTEvent::debugWinMovieLineRiku, NULL, DWTEvent::DebugAtelWorkDef};

static DEBUG_WINDOW_LINE debugWinMovieLine[60] = {
    {"001   :%s", &DebugWork::g_dw.atelwork.iSoraMovie0, 4, {0}, 0, 0.0f, 0.0f, 0.0f, szOffOn, 1, DWTEvent::DebugSetSoraMovieFlag, NULL},
    {"011   :%s", &DebugWork::g_dw.atelwork.iSoraMovie0, 4, {0}, 0, 0.0f, 0.0f, 0.0f, szOffOn, 2, DWTEvent::DebugSetSoraMovieFlag, NULL},
    {"012   :%s", &DebugWork::g_dw.atelwork.iSoraMovie0, 4, {0}, 0, 0.0f, 0.0f, 0.0f, szOffOn, 4, DWTEvent::DebugSetSoraMovieFlag, NULL},
    {"013   :%s", &DebugWork::g_dw.atelwork.iSoraMovie0, 4, {0}, 0, 0.0f, 0.0f, 0.0f, szOffOn, 8, DWTEvent::DebugSetSoraMovieFlag, NULL},
    {"021   :%s", &DebugWork::g_dw.atelwork.iSoraMovie0, 4, {0}, 0, 0.0f, 0.0f, 0.0f, szOffOn, 0x10, DWTEvent::DebugSetSoraMovieFlag, NULL},
    {"022   :%s", &DebugWork::g_dw.atelwork.iSoraMovie0, 4, {0}, 0, 0.0f, 0.0f, 0.0f, szOffOn, 0x20, DWTEvent::DebugSetSoraMovieFlag, NULL},
    {"023   :%s", &DebugWork::g_dw.atelwork.iSoraMovie0, 4, {0}, 0, 0.0f, 0.0f, 0.0f, szOffOn, 0x40, DWTEvent::DebugSetSoraMovieFlag, NULL},
    {"031   :%s", &DebugWork::g_dw.atelwork.iSoraMovie0, 4, {0}, 0, 0.0f, 0.0f, 0.0f, szOffOn, 0x80, DWTEvent::DebugSetSoraMovieFlag, NULL},
    {"032   :%s", &DebugWork::g_dw.atelwork.iSoraMovie0, 4, {0}, 0, 0.0f, 0.0f, 0.0f, szOffOn, 0x100, DWTEvent::DebugSetSoraMovieFlag, NULL},
    {"033   :%s", &DebugWork::g_dw.atelwork.iSoraMovie0, 4, {0}, 0, 0.0f, 0.0f, 0.0f, szOffOn, 0x200, DWTEvent::DebugSetSoraMovieFlag, NULL},
    {"041   :%s", &DebugWork::g_dw.atelwork.iSoraMovie0, 4, {0}, 0, 0.0f, 0.0f, 0.0f, szOffOn, 0x400, DWTEvent::DebugSetSoraMovieFlag, NULL},
    {"042   :%s", &DebugWork::g_dw.atelwork.iSoraMovie0, 4, {0}, 0, 0.0f, 0.0f, 0.0f, szOffOn, 0x800, DWTEvent::DebugSetSoraMovieFlag, NULL},
    {"043   :%s", &DebugWork::g_dw.atelwork.iSoraMovie0, 4, {0}, 0, 0.0f, 0.0f, 0.0f, szOffOn, 0x1000, DWTEvent::DebugSetSoraMovieFlag, NULL},
    {"051   :%s", &DebugWork::g_dw.atelwork.iSoraMovie0, 4, {0}, 0, 0.0f, 0.0f, 0.0f, szOffOn, 0x2000, DWTEvent::DebugSetSoraMovieFlag, NULL},
    {"052   :%s", &DebugWork::g_dw.atelwork.iSoraMovie0, 4, {0}, 0, 0.0f, 0.0f, 0.0f, szOffOn, 0x4000, DWTEvent::DebugSetSoraMovieFlag, NULL},
    {"053   :%s", &DebugWork::g_dw.atelwork.iSoraMovie0, 4, {0}, 0, 0.0f, 0.0f, 0.0f, szOffOn, 0x8000, DWTEvent::DebugSetSoraMovieFlag, NULL},
    {"061   :%s", &DebugWork::g_dw.atelwork.iSoraMovie0, 4, {0}, 0, 0.0f, 0.0f, 0.0f, szOffOn, 0x10000, DWTEvent::DebugSetSoraMovieFlag, NULL},
    {"062   :%s", &DebugWork::g_dw.atelwork.iSoraMovie0, 4, {0}, 0, 0.0f, 0.0f, 0.0f, szOffOn, 0x20000, DWTEvent::DebugSetSoraMovieFlag, NULL},
    {"063   :%s", &DebugWork::g_dw.atelwork.iSoraMovie0, 4, {0}, 0, 0.0f, 0.0f, 0.0f, szOffOn, 0x40000, DWTEvent::DebugSetSoraMovieFlag, NULL},
    {"064   :%s", &DebugWork::g_dw.atelwork.iSoraMovie0, 4, {0}, 0, 0.0f, 0.0f, 0.0f, szOffOn, 0x80000, DWTEvent::DebugSetSoraMovieFlag, NULL},
    {"071   :%s", &DebugWork::g_dw.atelwork.iSoraMovie0, 4, {0}, 0, 0.0f, 0.0f, 0.0f, szOffOn, 0x100000, DWTEvent::DebugSetSoraMovieFlag, NULL},
    {"072   :%s", &DebugWork::g_dw.atelwork.iSoraMovie0, 4, {0}, 0, 0.0f, 0.0f, 0.0f, szOffOn, 0x200000, DWTEvent::DebugSetSoraMovieFlag, NULL},
    {"073   :%s", &DebugWork::g_dw.atelwork.iSoraMovie0, 4, {0}, 0, 0.0f, 0.0f, 0.0f, szOffOn, 0x400000, DWTEvent::DebugSetSoraMovieFlag, NULL},
    {"081   :%s", &DebugWork::g_dw.atelwork.iSoraMovie0, 4, {0}, 0, 0.0f, 0.0f, 0.0f, szOffOn, 0x800000, DWTEvent::DebugSetSoraMovieFlag, NULL},
    {"082   :%s", &DebugWork::g_dw.atelwork.iSoraMovie0, 4, {0}, 0, 0.0f, 0.0f, 0.0f, szOffOn, 0x1000000, DWTEvent::DebugSetSoraMovieFlag, NULL},
    {"083   :%s", &DebugWork::g_dw.atelwork.iSoraMovie0, 4, {0}, 0, 0.0f, 0.0f, 0.0f, szOffOn, 0x2000000, DWTEvent::DebugSetSoraMovieFlag, NULL},
    {"084   :%s", &DebugWork::g_dw.atelwork.iSoraMovie0, 4, {0}, 0, 0.0f, 0.0f, 0.0f, szOffOn, 0x4000000, DWTEvent::DebugSetSoraMovieFlag, NULL},
    {"091   :%s", &DebugWork::g_dw.atelwork.iSoraMovie0, 4, {0}, 0, 0.0f, 0.0f, 0.0f, szOffOn, 0x8000000, DWTEvent::DebugSetSoraMovieFlag, NULL},
    {"092   :%s", &DebugWork::g_dw.atelwork.iSoraMovie0, 4, {0}, 0, 0.0f, 0.0f, 0.0f, szOffOn, 0x10000000, DWTEvent::DebugSetSoraMovieFlag, NULL},
    {"093   :%s", &DebugWork::g_dw.atelwork.iSoraMovie0, 4, {0}, 0, 0.0f, 0.0f, 0.0f, szOffOn, 0x20000000, DWTEvent::DebugSetSoraMovieFlag, NULL},
    {"101   :%s", &DebugWork::g_dw.atelwork.iSoraMovie0, 4, {0}, 0, 0.0f, 0.0f, 0.0f, szOffOn, 0x40000000, DWTEvent::DebugSetSoraMovieFlag, NULL},
    {"102   :%s", &DebugWork::g_dw.atelwork.iSoraMovie0, 4, {0}, 0, 0.0f, 0.0f, 0.0f, szOffOn, 0x80000000, DWTEvent::DebugSetSoraMovieFlag, NULL},
    {"103   :%s", &DebugWork::g_dw.atelwork.iSoraMovie1, 4, {0}, 0, 0.0f, 0.0f, 0.0f, szOffOn, 1, DWTEvent::DebugSetSoraMovieFlag, NULL},
    {"104   :%s", &DebugWork::g_dw.atelwork.iSoraMovie1, 4, {0}, 0, 0.0f, 0.0f, 0.0f, szOffOn, 2, DWTEvent::DebugSetSoraMovieFlag, NULL},
    {"111   :%s", &DebugWork::g_dw.atelwork.iSoraMovie1, 4, {0}, 0, 0.0f, 0.0f, 0.0f, szOffOn, 4, DWTEvent::DebugSetSoraMovieFlag, NULL},
    {"112   :%s", &DebugWork::g_dw.atelwork.iSoraMovie1, 4, {0}, 0, 0.0f, 0.0f, 0.0f, szOffOn, 8, DWTEvent::DebugSetSoraMovieFlag, NULL},
    {"113   :%s", &DebugWork::g_dw.atelwork.iSoraMovie1, 4, {0}, 0, 0.0f, 0.0f, 0.0f, szOffOn, 0x10, DWTEvent::DebugSetSoraMovieFlag, NULL},
    {"114   :%s", &DebugWork::g_dw.atelwork.iSoraMovie1, 4, {0}, 0, 0.0f, 0.0f, 0.0f, szOffOn, 0x20, DWTEvent::DebugSetSoraMovieFlag, NULL},
    {"115   :%s", &DebugWork::g_dw.atelwork.iSoraMovie1, 4, {0}, 0, 0.0f, 0.0f, 0.0f, szOffOn, 0x40, DWTEvent::DebugSetSoraMovieFlag, NULL},
    {"116   :%s", &DebugWork::g_dw.atelwork.iSoraMovie1, 4, {0}, 0, 0.0f, 0.0f, 0.0f, szOffOn, 0x80, DWTEvent::DebugSetSoraMovieFlag, NULL},
    {"117   :%s", &DebugWork::g_dw.atelwork.iSoraMovie1, 4, {0}, 0, 0.0f, 0.0f, 0.0f, szOffOn, 0x100, DWTEvent::DebugSetSoraMovieFlag, NULL},
    {"118   :%s", &DebugWork::g_dw.atelwork.iSoraMovie1, 4, {0}, 0, 0.0f, 0.0f, 0.0f, szOffOn, 0x200, DWTEvent::DebugSetSoraMovieFlag, NULL},
    {"121   :%s", &DebugWork::g_dw.atelwork.iSoraMovie1, 4, {0}, 0, 0.0f, 0.0f, 0.0f, szOffOn, 0x400, DWTEvent::DebugSetSoraMovieFlag, NULL},
    {"122   :%s", &DebugWork::g_dw.atelwork.iSoraMovie1, 4, {0}, 0, 0.0f, 0.0f, 0.0f, szOffOn, 0x800, DWTEvent::DebugSetSoraMovieFlag, NULL},
    {"123   :%s", &DebugWork::g_dw.atelwork.iSoraMovie1, 4, {0}, 0, 0.0f, 0.0f, 0.0f, szOffOn, 0x1000, DWTEvent::DebugSetSoraMovieFlag, NULL},
    {"124   :%s", &DebugWork::g_dw.atelwork.iSoraMovie1, 4, {0}, 0, 0.0f, 0.0f, 0.0f, szOffOn, 0x2000, DWTEvent::DebugSetSoraMovieFlag, NULL},
    {"125   :%s", &DebugWork::g_dw.atelwork.iSoraMovie1, 4, {0}, 0, 0.0f, 0.0f, 0.0f, szOffOn, 0x4000, DWTEvent::DebugSetSoraMovieFlag, NULL},
    {"126   :%s", &DebugWork::g_dw.atelwork.iSoraMovie1, 4, {0}, 0, 0.0f, 0.0f, 0.0f, szOffOn, 0x8000, DWTEvent::DebugSetSoraMovieFlag, NULL},
    {"131   :%s", &DebugWork::g_dw.atelwork.iSoraMovie1, 4, {0}, 0, 0.0f, 0.0f, 0.0f, szOffOn, 0x10000, DWTEvent::DebugSetSoraMovieFlag, NULL},
    {"132   :%s", &DebugWork::g_dw.atelwork.iSoraMovie1, 4, {0}, 0, 0.0f, 0.0f, 0.0f, szOffOn, 0x20000, DWTEvent::DebugSetSoraMovieFlag, NULL},
    {"133   :%s", &DebugWork::g_dw.atelwork.iSoraMovie1, 4, {0}, 0, 0.0f, 0.0f, 0.0f, szOffOn, 0x40000, DWTEvent::DebugSetSoraMovieFlag, NULL},
    {"134   :%s", &DebugWork::g_dw.atelwork.iSoraMovie1, 4, {0}, 0, 0.0f, 0.0f, 0.0f, szOffOn, 0x80000, DWTEvent::DebugSetSoraMovieFlag, NULL},
    {"135   :%s", &DebugWork::g_dw.atelwork.iSoraMovie1, 4, {0}, 0, 0.0f, 0.0f, 0.0f, szOffOn, 0x100000, DWTEvent::DebugSetSoraMovieFlag, NULL},
    {"136   :%s", &DebugWork::g_dw.atelwork.iSoraMovie1, 4, {0}, 0, 0.0f, 0.0f, 0.0f, szOffOn, 0x200000, DWTEvent::DebugSetSoraMovieFlag, NULL},
    {"137   :%s", &DebugWork::g_dw.atelwork.iSoraMovie1, 4, {0}, 0, 0.0f, 0.0f, 0.0f, szOffOn, 0x400000, DWTEvent::DebugSetSoraMovieFlag, NULL},
    {"138   :%s", &DebugWork::g_dw.atelwork.iSoraMovie1, 4, {0}, 0, 0.0f, 0.0f, 0.0f, szOffOn, 0x800000, DWTEvent::DebugSetSoraMovieFlag, NULL},
    {"139   :%s", &DebugWork::g_dw.atelwork.iSoraMovie1, 4, {0}, 0, 0.0f, 0.0f, 0.0f, szOffOn, 0x1000000, DWTEvent::DebugSetSoraMovieFlag, NULL},
    {"140   :%s", &DebugWork::g_dw.atelwork.iSoraMovie1, 4, {0}, 0, 0.0f, 0.0f, 0.0f, szOffOn, 0x2000000, DWTEvent::DebugSetSoraMovieFlag, NULL},
    {"151   :%s", &DebugWork::g_dw.atelwork.iSoraMovie1, 4, {0}, 0, 0.0f, 0.0f, 0.0f, szOffOn, 0x4000000, DWTEvent::DebugSetSoraMovieFlag, NULL},
    {NULL, NULL, 0, {0}, 0, 0.0f, 0.0f, 0.0f, NULL, 0, NULL, NULL},
};

static DEBUG_WINDOW_TBL debugWinMovie = {"\x83\x80\x81[\x83r\x81[", -106, -(SCREEN_HEIGHT >> 1) + 150, 15, 20, 12, 0, 0, DWTEvent::debugWinMovieLine, NULL, DWTEvent::DebugAtelWorkDef};

static DEBUG_WINDOW_LINE debugWinPcSkillLine[7] = {
    {"\x83n\x83" "C\x83W\x83\x83\x83\x93\x83v     :%s", &DebugWork::g_dw.atelwork.iPcStanderdSkill, 4, {0}, 0, 0.0f, 0.0f, 0.0f, szOffOn, 1, DWTEvent::DebugSetPcSkillFlag, NULL},
    {"\x83O\x83\x89\x83" "C\x83h         :%s", &DebugWork::g_dw.atelwork.iPcStanderdSkill, 4, {0}, 0, 0.0f, 0.0f, 0.0f, szOffOn, 2, DWTEvent::DebugSetPcSkillFlag, NULL},
    {"\x83X\x81[\x83p\x81[\x83O\x83\x89\x83" "C\x83h :%s", &DebugWork::g_dw.atelwork.iPcStanderdSkill, 4, {0}, 0, 0.0f, 0.0f, 0.0f, szOffOn, 4, DWTEvent::DebugSetPcSkillFlag, NULL},
    {"\x83_\x81[\x83N\x83\x8a\x83N\x89\xbb     :%s", &DebugWork::g_dw.atelwork.iPcStanderdSkill, 4, {0}, 0, 0.0f, 0.0f, 0.0f, szOffOn, 8, DWTEvent::DebugSetPcSkillFlag, NULL},
    {"\x83X\x83s\x81[\x83h\x83u\x83\x8c\x83" "C\x83N :%s", &DebugWork::g_dw.atelwork.iPcStanderdSkill, 4, {0}, 0, 0.0f, 0.0f, 0.0f, szOffOn, 0x10, DWTEvent::DebugSetPcSkillFlag, NULL},
    {"\x83" "f\x83\x85\x83G\x83\x8b         :%s", &DebugWork::g_dw.atelwork.iPcStanderdSkill, 4, {0}, 0, 0.0f, 0.0f, 0.0f, szOffOn, 0x20, DWTEvent::DebugSetPcSkillFlag, NULL},
    {NULL, NULL, 0, {0}, 0, 0.0f, 0.0f, 0.0f, NULL, 0, NULL, NULL},
};

static DEBUG_WINDOW_TBL debugWinPcSkill = {"\x83v\x83\x8c\x83" "C\x83\x84\x81[\x83X\x83L\x83\x8b", -106, -(SCREEN_HEIGHT >> 1) + 150, 15, 6, 12, 0, 0, DWTEvent::debugWinPcSkillLine, NULL, DWTEvent::DebugAtelWorkDef};

static DEBUG_WINDOW_LINE debugWinPoohLine[40] = {
    {"\xf9S----------\xf9V\x83Q\x81[\x83\x80\xf9S---------", &CDebugWindow::none, 1, {0}, 2, 0.0f, 0.0f, 0.0f, NULL, 0, NULL, NULL},
    {"\x82\xb6\x82\xe5\x82\xa4\x82\xbd\x82\xa2  :%s", &DebugWork::g_dw.atelwork.iPoohMinigameState, 1, {0}, 0, 0.0f, 0.0f, 0.0f, &szMiniGamePlayState[DebugWork::g_dw.atelwork.iPoohMinigameState], 0, NULL, NULL},
    {"\x82\xb5\x82\xe3\x82\xe9\x82\xa2    :%s", &DebugWork::g_dw.atelwork.iPoohMinigameKind, 1, {0}, 0, 0.0f, 0.0f, 0.0f, &szMiniGameKind[DebugWork::g_dw.atelwork.iPoohMinigameKind], 0, NULL, NULL},
    {"\xf9S----------\xf9V\x82\xc7\x82\xa4\x82\xc9\x82\xe3\x82\xa4\xf9S---------", &CDebugWindow::none, 1, {0}, 2, 0.0f, 0.0f, 0.0f, NULL, 0, NULL, NULL},
    {"\x83s\x83O\x83\x8c\x83" "b\x83g  :%s", &DebugWork::g_dw.atelwork.iPoohMiniGameIntro, 4, {0}, 0, 0.0f, 0.0f, 0.0f, szOffOn, 1, DWTEvent::DebugSetPoohFlag, NULL},
    {"\x83I\x83" "E\x83\x8b      :%s", &DebugWork::g_dw.atelwork.iPoohMiniGameIntro, 4, {0}, 0, 0.0f, 0.0f, 0.0f, szOffOn, 2, DWTEvent::DebugSetPoohFlag, NULL},
    {"\x83\x8b\x81[        :%s", &DebugWork::g_dw.atelwork.iPoohMiniGameIntro, 4, {0}, 0, 0.0f, 0.0f, 0.0f, szOffOn, 4, DWTEvent::DebugSetPoohFlag, NULL},
    {"\x83" "C\x81[\x83\x88\x81[    :%s", &DebugWork::g_dw.atelwork.iPoohMiniGameIntro, 4, {0}, 0, 0.0f, 0.0f, 0.0f, szOffOn, 8, DWTEvent::DebugSetPoohFlag, NULL},
    {"\x83" "e\x83" "B\x83K\x81[    :%s", &DebugWork::g_dw.atelwork.iPoohMiniGameIntro, 4, {0}, 0, 0.0f, 0.0f, 0.0f, szOffOn, 0x10, DWTEvent::DebugSetPoohFlag, NULL},
    {"\x83\x89\x83r\x83" "b\x83g    :%s", &DebugWork::g_dw.atelwork.iPoohMiniGameIntro, 4, {0}, 0, 0.0f, 0.0f, 0.0f, szOffOn, 0x20, DWTEvent::DebugSetPoohFlag, NULL},
    {"\x83S\x81[\x83\x8b\x82P    :%s", &DebugWork::g_dw.atelwork.iPoohMiniGameIntro, 4, {0}, 0, 0.0f, 0.0f, 0.0f, szOffOn, 0x40, DWTEvent::DebugSetPoohFlag, NULL},
    {"\x83S\x81[\x83\x8b\x82Q    :%s", &DebugWork::g_dw.atelwork.iPoohMiniGameIntro, 4, {0}, 0, 0.0f, 0.0f, 0.0f, szOffOn, 0x80, DWTEvent::DebugSetPoohFlag, NULL},
    {"\xf9S----------\xf9V\x82\xb6\x82\xe5\x82\xa4\x82\xbd\x82\xa2\xf9S---------", &CDebugWindow::none, 1, {0}, 2, 0.0f, 0.0f, 0.0f, NULL, 0, NULL, NULL},
    {"\x83s\x83O\x83\x8c\x83" "b\x83g  :%s", &DebugWork::g_dw.atelwork.iPoohGamePiglet, 1, {0}, 0, 0.0f, 3.0f, 1.0f, &szMiniGameState[DebugWork::g_dw.atelwork.iPoohGamePiglet], 0, DWTEvent::DebugSetPoohFlag, NULL},
    {"\x83I\x83" "E\x83\x8b      :%s", &DebugWork::g_dw.atelwork.iPoohGameOwl, 1, {0}, 0, 0.0f, 3.0f, 1.0f, &szMiniGameState[DebugWork::g_dw.atelwork.iPoohGameOwl], 0, DWTEvent::DebugSetPoohFlag, NULL},
    {"\x83\x8b\x81[        :%s", &DebugWork::g_dw.atelwork.iPoohGameRoo, 1, {0}, 0, 0.0f, 3.0f, 1.0f, &szMiniGameState[DebugWork::g_dw.atelwork.iPoohGameRoo], 0, DWTEvent::DebugSetPoohFlag, NULL},
    {"\x83" "C\x81[\x83\x88\x81[    :%s", &DebugWork::g_dw.atelwork.iPoohGameEeyore, 1, {0}, 0, 0.0f, 3.0f, 1.0f, &szMiniGameState[DebugWork::g_dw.atelwork.iPoohGameEeyore], 0, DWTEvent::DebugSetPoohFlag, NULL},
    {"\x83" "e\x83" "B\x83K\x81[    :%s", &DebugWork::g_dw.atelwork.iPoohGameTigger, 1, {0}, 0, 0.0f, 3.0f, 1.0f, &szMiniGameState[DebugWork::g_dw.atelwork.iPoohGameTigger], 0, DWTEvent::DebugSetPoohFlag, NULL},
    {"\x83\x89\x83r\x83" "b\x83g    :%s", &DebugWork::g_dw.atelwork.iPoohGameRabbit, 1, {0}, 0, 0.0f, 3.0f, 1.0f, &szMiniGameState[DebugWork::g_dw.atelwork.iPoohGameRabbit], 0, DWTEvent::DebugSetPoohFlag, NULL},
    {"\xf9S----------\xf9V\x83N\x83\x8a\x83" "A\x97p\xf9S---------", &CDebugWindow::none, 1, {0}, 2, 0.0f, 0.0f, 0.0f, NULL, 0, NULL, NULL},
    {"\x83s\x83O\x83\x8c\x83" "b\x83g  :%d", &DebugWork::g_dw.atelwork.iPoohClearIntoroPiglet, 1, {0}, 0, 0.0f, 0.0f, 0.0f, 0, 0, NULL, NULL},
    {"\x83I\x83" "E\x83\x8b      :%d", &DebugWork::g_dw.atelwork.iPoohClearIntoroOwl, 1, {0}, 0, 0.0f, 0.0f, 0.0f, 0, 0, NULL, NULL},
    {"\x83\x8b\x81[        :%d", &DebugWork::g_dw.atelwork.iPoohClearIntoroRoo, 1, {0}, 0, 0.0f, 0.0f, 0.0f, 0, 0, NULL, NULL},
    {"\x83" "C\x81[\x83\x88\x81[    :%d", &DebugWork::g_dw.atelwork.iPoohClearIntoroEeyore, 1, {0}, 0, 0.0f, 0.0f, 0.0f, 0, 0, NULL, NULL},
    {"\x83" "e\x83" "B\x83K\x81[    :%d", &DebugWork::g_dw.atelwork.iPoohClearIntoroTigger, 1, {0}, 0, 0.0f, 0.0f, 0.0f, 0, 0, NULL, NULL},
    {"\x83\x89\x83r\x83" "b\x83g    :%d", &DebugWork::g_dw.atelwork.iPoohClearIntoroRabbit, 1, {0}, 0, 0.0f, 0.0f, 0.0f, 0, 0, NULL, NULL},
    {"\xf9S----------\xf9V\x8eg\x82\xa2\x8e\xcc\x82\xc4\x83N\x83\x8a\x83" "A\x97p\xf9S---------", &CDebugWindow::none, 1, {0}, 2, 0.0f, 0.0f, 0.0f, NULL, 0, NULL, NULL},
    {"\x83s\x83O\x83\x8c\x83" "b\x83g  :%d", &DebugWork::g_dw.atelwork.iPoohClearIntoroResetPiglet, 1, {0}, 0, 0.0f, 0.0f, 0.0f, 0, 0, NULL, NULL},
    {"\x83I\x83" "E\x83\x8b      :%d", &DebugWork::g_dw.atelwork.iPoohClearIntoroResetOwl, 1, {0}, 0, 0.0f, 0.0f, 0.0f, 0, 0, NULL, NULL},
    {"\x83\x8b\x81[        :%d", &DebugWork::g_dw.atelwork.iPoohClearIntoroResetRoo, 1, {0}, 0, 0.0f, 0.0f, 0.0f, 0, 0, NULL, NULL},
    {"\x83" "C\x81[\x83\x88\x81[    :%d", &DebugWork::g_dw.atelwork.iPoohClearIntoroResetEeyore, 1, {0}, 0, 0.0f, 0.0f, 0.0f, 0, 0, NULL, NULL},
    {"\x83" "e\x83" "B\x83K\x81[    :%d", &DebugWork::g_dw.atelwork.iPoohClearIntoroResetTigger, 1, {0}, 0, 0.0f, 0.0f, 0.0f, 0, 0, NULL, NULL},
    {"\x83\x89\x83r\x83" "b\x83g    :%d", &DebugWork::g_dw.atelwork.iPoohClearIntoroResetRabbit, 1, {0}, 0, 0.0f, 0.0f, 0.0f, 0, 0, NULL, NULL},
    {"\xf9S----------\xf9V\x83n\x83" "C\x83X\x83R\x83" "A\xf9S---------", &CDebugWindow::none, 1, {0}, 2, 0.0f, 0.0f, 0.0f, NULL, 0, NULL, NULL},
    {"\x83I\x83" "E\x83\x8b      :%d", &DebugWork::g_dw.atelwork.iPoohHiScoreOwl, 4, {0}, 0, 0.0f, 10000.0f, 100.0f, NULL, 0, DWTEvent::DebugSetPoohFlag, NULL},
    {"\x83\x8b\x81[        :%d", &DebugWork::g_dw.atelwork.iPoohHiScoreRoo, 4, {0}, 0, 0.0f, 10000.0f, 100.0f, NULL, 0, DWTEvent::DebugSetPoohFlag, NULL},
    {"\x83" "C\x81[\x83\x88\x81[    :%d", &DebugWork::g_dw.atelwork.iPoohHiScoreEeyore, 4, {0}, 0, 0.0f, 10000.0f, 100.0f, NULL, 0, DWTEvent::DebugSetPoohFlag, NULL},
    {"\x83" "e\x83" "B\x83K\x81[    :%d", &DebugWork::g_dw.atelwork.iPoohHiScoreTigger, 4, {0}, 0, 0.0f, 10000.0f, 100.0f, NULL, 0, DWTEvent::DebugSetPoohFlag, NULL},
    {"\x83\x89\x83r\x83" "b\x83g    :%d", &DebugWork::g_dw.atelwork.iPoohHiScoreRabbit, 4, {0}, 0, 0.0f, 10000.0f, 100.0f, NULL, 0, DWTEvent::DebugSetPoohFlag, NULL},
    {NULL, NULL, 0, {0}, 0, 0.0f, 0.0f, 0.0f, 0, 0, NULL, NULL},
};

static DEBUG_WINDOW_TBL debugWinPooh = {"\x83v\x81[", -106, -(SCREEN_HEIGHT >> 1) + 150, 15, 25, 12, 0, 0, DWTEvent::debugWinPoohLine, NULL, DWTEvent::DebugAtelWorkDef};

static DEBUG_WINDOW_LINE debugWinFriendPossibleLine[10] = {
    {"\x83h\x83i\x83\x8b\x83h     :%s", &DebugWork::g_dw.atelwork.iPossibleFriend, 4, {0}, 0, 0.0f, 0.0f, 0.0f, szOffOn, 1, DWTEvent::DebugSetFriendPossibleFlag, NULL},
    {"\x83O\x81[\x83t\x83" "B\x81[   :%s", &DebugWork::g_dw.atelwork.iPossibleFriend, 4, {0}, 0, 0.0f, 0.0f, 0.0f, szOffOn, 2, DWTEvent::DebugSetFriendPossibleFlag, NULL},
    {"\x83" "A\x83\x89\x83W\x83\x93     :%s", &DebugWork::g_dw.atelwork.iPossibleFriend, 4, {0}, 0, 0.0f, 0.0f, 0.0f, szOffOn, 4, DWTEvent::DebugSetFriendPossibleFlag, NULL},
    {"\x83" "A\x83\x8a\x83G\x83\x8b     :%s", &DebugWork::g_dw.atelwork.iPossibleFriend, 4, {0}, 0, 0.0f, 0.0f, 0.0f, szOffOn, 8, DWTEvent::DebugSetFriendPossibleFlag, NULL},
    {"\x83W\x83\x83\x83" "b\x83N     :%s", &DebugWork::g_dw.atelwork.iPossibleFriend, 4, {0}, 0, 0.0f, 0.0f, 0.0f, szOffOn, 0x10, DWTEvent::DebugSetFriendPossibleFlag, NULL},
    {"\x83s\x81[\x83^\x81[\x83p\x83\x93 :%s", &DebugWork::g_dw.atelwork.iPossibleFriend, 4, {0}, 0, 0.0f, 0.0f, 0.0f, szOffOn, 0x20, DWTEvent::DebugSetFriendPossibleFlag, NULL},
    {"\x83r\x81[\x83X\x83g     :%s", &DebugWork::g_dw.atelwork.iPossibleFriend, 4, {0}, 0, 0.0f, 0.0f, 0.0f, szOffOn, 0x40, DWTEvent::DebugSetFriendPossibleFlag, NULL},
    {"\x89\xa4\x97l         :%s", &DebugWork::g_dw.atelwork.iPossibleFriend, 4, {0}, 0, 0.0f, 0.0f, 0.0f, szOffOn, 0x80, DWTEvent::DebugSetFriendPossibleFlag, NULL},
    {"\x83v\x83\x8b\x81[\x83g     :%s", &DebugWork::g_dw.atelwork.iPossibleFriend, 4, {0}, 0, 0.0f, 0.0f, 0.0f, szOffOn, 0x100, DWTEvent::DebugSetFriendPossibleFlag, NULL},
    {NULL, NULL, 0, {0}, 0, 0.0f, 0.0f, 0.0f, NULL, 0, NULL, NULL},
};

static DEBUG_WINDOW_TBL debugWinFriendPossible = {"\x83t\x83\x8c\x83\x93\x83h\x82\xa9\x82\xa2\x82\xab\x82\xf1", -106, -(SCREEN_HEIGHT >> 1) + 150, 15, 29, 12, 0, 0, DWTEvent::debugWinFriendPossibleLine, NULL, DWTEvent::DebugAtelWorkDef};

static DEBUG_WINDOW_LINE debugWinCardPossibleLine[27] = {
    {"\x83G\x83O\x83U\x83~\x83l\x81[\x83V\x83\x87\x83\x93 :%s", &DebugWork::g_dw.atelwork.iPossibleCard, 4, {0}, 0, 0.0f, 0.0f, 0.0f, szOffOn, 1, DWTEvent::DebugSetCardPossibleFlag, NULL},
    {"\x83\x81\x83^\x83\x8b\x83`\x83\x87\x83R\x83{     :%s", &DebugWork::g_dw.atelwork.iPossibleCard, 4, {0}, 0, 0.0f, 0.0f, 0.0f, szOffOn, 2, DWTEvent::DebugSetCardPossibleFlag, NULL},
    {"\x83\x89\x83" "C\x83I\x83\x93\x83n\x81[\x83g     :%s", &DebugWork::g_dw.atelwork.iPossibleCard, 4, {0}, 0, 0.0f, 0.0f, 0.0f, szOffOn, 4, DWTEvent::DebugSetCardPossibleFlag, NULL},
    {"\x96\xf1\x91\xa9\x82\xcc\x82\xa8\x8e\xe7\x82\xe8       :%s", &DebugWork::g_dw.atelwork.iPossibleCard, 4, {0}, 0, 0.0f, 0.0f, 0.0f, szOffOn, 8, DWTEvent::DebugSetCardPossibleFlag, NULL},
    {"\x89\xdf\x82\xac\x8b\x8e\x82\xe8\x82\xb5\x8ev\x82\xa2\x8fo   :%s", &DebugWork::g_dw.atelwork.iPossibleCard, 4, {0}, 0, 0.0f, 0.0f, 0.0f, szOffOn, 0x10, DWTEvent::DebugSetCardPossibleFlag, NULL},
    {"\x83" "A\x83\x8b\x83" "e\x83}\x83" "E\x83G\x83|\x83\x93   :%s", &DebugWork::g_dw.atelwork.iPossibleCard, 4, {0}, 0, 0.0f, 0.0f, 0.0f, szOffOn, 0x20, DWTEvent::DebugSetCardPossibleFlag, NULL},
    {"\x83_\x83" "C\x83" "A\x83\x82\x83\x93\x83h\x83_\x83X\x83g :%s", &DebugWork::g_dw.atelwork.iPossibleCard, 4, {0}, 0, 0.0f, 0.0f, 0.0f, szOffOn, 0x40, DWTEvent::DebugSetCardPossibleFlag, NULL},
    {"\x95\xfb\x97\x83\x82\xcc\x93V\x8eg         :%s", &DebugWork::g_dw.atelwork.iPossibleCard, 4, {0}, 0, 0.0f, 0.0f, 0.0f, szOffOn, 0x80, DWTEvent::DebugSetCardPossibleFlag, NULL},
    {"\x83t\x83@\x83" "C\x83" "A           :%s", &DebugWork::g_dw.atelwork.iPossibleCard, 4, {0}, 0, 0.0f, 0.0f, 0.0f, szOffOn, 0x100, DWTEvent::DebugSetCardPossibleFlag, NULL},
    {"\x83T\x83\x93\x83_\x81[           :%s", &DebugWork::g_dw.atelwork.iPossibleCard, 4, {0}, 0, 0.0f, 0.0f, 0.0f, szOffOn, 0x200, DWTEvent::DebugSetCardPossibleFlag, NULL},
    {"\x83O\x83\x89\x83r\x83" "f           :%s", &DebugWork::g_dw.atelwork.iPossibleCard, 4, {0}, 0, 0.0f, 0.0f, 0.0f, szOffOn, 0x400, DWTEvent::DebugSetCardPossibleFlag, NULL},
    {"\x83X\x83g\x83" "b\x83v           :%s", &DebugWork::g_dw.atelwork.iPossibleCard, 4, {0}, 0, 0.0f, 0.0f, 0.0f, szOffOn, 0x800, DWTEvent::DebugSetCardPossibleFlag, NULL},
    {"\x83G\x83" "A\x83\x8d             :%s", &DebugWork::g_dw.atelwork.iPossibleCard, 4, {0}, 0, 0.0f, 0.0f, 0.0f, szOffOn, 0x1000, DWTEvent::DebugSetCardPossibleFlag, NULL},
    {"\x83V\x83\x93\x83o             :%s", &DebugWork::g_dw.atelwork.iPossibleCard, 4, {0}, 0, 0.0f, 0.0f, 0.0f, szOffOn, 0x2000, DWTEvent::DebugSetCardPossibleFlag, NULL},
    {"\x83W\x81[\x83j\x81[           :%s", &DebugWork::g_dw.atelwork.iPossibleCard, 4, {0}, 0, 0.0f, 0.0f, 0.0f, szOffOn, 0x4000, DWTEvent::DebugSetCardPossibleFlag, NULL},
    {"\x83o\x83\x93\x83r             :%s", &DebugWork::g_dw.atelwork.iPossibleCard, 4, {0}, 0, 0.0f, 0.0f, 0.0f, szOffOn, 0x8000, DWTEvent::DebugSetCardPossibleFlag, NULL},
    {"\x83_\x83\x93\x83{             :%s", &DebugWork::g_dw.atelwork.iPossibleCard, 4, {0}, 0, 0.0f, 0.0f, 0.0f, szOffOn, 0x10000, DWTEvent::DebugSetCardPossibleFlag, NULL},
    {"\x83" "e\x83" "B\x83\x93\x83N           :%s", &DebugWork::g_dw.atelwork.iPossibleCard, 4, {0}, 0, 0.0f, 0.0f, 0.0f, szOffOn, 0x20000, DWTEvent::DebugSetCardPossibleFlag, NULL},
    {"\x83\x80\x81[\x83V\x83\x85\x81[         :%s", &DebugWork::g_dw.atelwork.iPossibleCard, 4, {0}, 0, 0.0f, 0.0f, 0.0f, szOffOn, 0x40000, DWTEvent::DebugSetCardPossibleFlag, NULL},
    {"\x83N\x83\x89\x83" "E\x83h           :%s", &DebugWork::g_dw.atelwork.iPossibleCard, 4, {0}, 0, 0.0f, 0.0f, 0.0f, szOffOn, 0x80000, DWTEvent::DebugSetCardPossibleFlag, NULL},
    {"\x83n\x83" "C\x83|\x81[\x83V\x83\x87\x83\x93     :%s", &DebugWork::g_dw.atelwork.iPossibleCard, 4, {0}, 0, 0.0f, 0.0f, 0.0f, szOffOn, 0x100000, DWTEvent::DebugSetCardPossibleFlag, NULL},
    {"\x83\x81\x83K\x83|\x81[\x83V\x83\x87\x83\x93     :%s", &DebugWork::g_dw.atelwork.iPossibleCard, 4, {0}, 0, 0.0f, 0.0f, 0.0f, szOffOn, 0x200000, DWTEvent::DebugSetCardPossibleFlag, NULL},
    {"\x83G\x81[\x83" "e\x83\x8b           :%s", &DebugWork::g_dw.atelwork.iPossibleCard, 4, {0}, 0, 0.0f, 0.0f, 0.0f, szOffOn, 0x400000, DWTEvent::DebugSetCardPossibleFlag, NULL},
    {"\x83\x81\x83K\x83G\x81[\x83" "e\x83\x8b       :%s", &DebugWork::g_dw.atelwork.iPossibleCard, 4, {0}, 0, 0.0f, 0.0f, 0.0f, szOffOn, 0x800000, DWTEvent::DebugSetCardPossibleFlag, NULL},
    {"\x83G\x83\x8a\x83N\x83T\x81[         :%s", &DebugWork::g_dw.atelwork.iPossibleCard, 4, {0}, 0, 0.0f, 0.0f, 0.0f, szOffOn, 0x1000000, DWTEvent::DebugSetCardPossibleFlag, NULL},
    {"\x83\x89\x83X\x83g\x83G\x83\x8a\x83N\x83T\x81[   :%s", &DebugWork::g_dw.atelwork.iPossibleCard, 4, {0}, 0, 0.0f, 0.0f, 0.0f, szOffOn, 0x2000000, DWTEvent::DebugSetCardPossibleFlag, NULL},
    {NULL, NULL, 0, {0}, 0, 0.0f, 0.0f, 0.0f, NULL, 0, NULL, NULL},
};

static DEBUG_WINDOW_TBL debugWinCardPossible = {"\x83J\x81[\x83h\x82\xa9\x82\xa2\x82\xab\x82\xf1", -106, -(SCREEN_HEIGHT >> 1) + 150, 15, 27, 12, 0, 0, DWTEvent::debugWinCardPossibleLine, NULL, DWTEvent::DebugAtelWorkDef};

static DEBUG_WINDOW_LINE debugWinWorldIntroLine[29] = {
    {"\xf9S----------\xf9V\x83\\\x83\x89\xf9S---------", &CDebugWindow::none, 1, {0}, 2, 0.0f, 0.0f, 0.0f, NULL, 0, NULL, NULL},
    {"\x83g\x83\x89\x83\x94\x83@\x81[\x83X       :%s", &DebugWork::g_dw.atelwork.iWorldIntro, 4, {0}, 0, 0.0f, 1.0f, 0.0f, szOffOn, 1, DWTEvent::DebugSetWorldTitleFlag, NULL},
    {"\x83" "A\x83O\x83\x89\x83o           :%s", &DebugWork::g_dw.atelwork.iWorldIntro, 4, {0}, 0, 0.0f, 1.0f, 0.0f, szOffOn, 2, DWTEvent::DebugSetWorldTitleFlag, NULL},
    {"\x83I\x83\x8a\x83\x93\x83|\x83X         :%s", &DebugWork::g_dw.atelwork.iWorldIntro, 4, {0}, 0, 0.0f, 1.0f, 0.0f, szOffOn, 4, DWTEvent::DebugSetWorldTitleFlag, NULL},
    {"\x83\x8f\x83\x93\x83_\x81[\x83\x89\x83\x93\x83h     :%s", &DebugWork::g_dw.atelwork.iWorldIntro, 4, {0}, 0, 0.0f, 1.0f, 0.0f, szOffOn, 8, DWTEvent::DebugSetWorldTitleFlag, NULL},
    {"\x83\x82\x83\x93\x83X\x83g\x83\x8d         :%s", &DebugWork::g_dw.atelwork.iWorldIntro, 4, {0}, 0, 0.0f, 1.0f, 0.0f, szOffOn, 0x10, DWTEvent::DebugSetWorldTitleFlag, NULL},
    {"\x83n\x83\x8d\x83" "E\x83" "B\x83\x93         :%s", &DebugWork::g_dw.atelwork.iWorldIntro, 4, {0}, 0, 0.0f, 1.0f, 0.0f, szOffOn, 0x20, DWTEvent::DebugSetWorldTitleFlag, NULL},
    {"\x83" "A\x83g\x83\x89\x83\x93\x83" "e\x83" "B\x83J     :%s", &DebugWork::g_dw.atelwork.iWorldIntro, 4, {0}, 0, 0.0f, 1.0f, 0.0f, szOffOn, 0x40, DWTEvent::DebugSetWorldTitleFlag, NULL},
    {"\x83l\x83o\x81[\x83\x89\x83\x93\x83h       :%s", &DebugWork::g_dw.atelwork.iWorldIntro, 4, {0}, 0, 0.0f, 1.0f, 0.0f, szOffOn, 0x80, DWTEvent::DebugSetWorldTitleFlag, NULL},
    {"\x83z\x83\x8d\x83" "E\x83o\x83X\x83" "e\x83" "B\x83I\x83\x93 :%s", &DebugWork::g_dw.atelwork.iWorldIntro, 4, {0}, 0, 0.0f, 1.0f, 0.0f, szOffOn, 0x100, DWTEvent::DebugSetWorldTitleFlag, NULL},
    {"100\x83G\x81[\x83J\x81[        :%s", &DebugWork::g_dw.atelwork.iWorldIntro, 4, {0}, 0, 0.0f, 1.0f, 0.0f, szOffOn, 0x200, DWTEvent::DebugSetWorldTitleFlag, NULL},
    {"\x83g\x83\x8f\x83" "C\x83\x89\x83" "C\x83g       :%s", &DebugWork::g_dw.atelwork.iWorldIntro, 4, {0}, 0, 0.0f, 1.0f, 0.0f, szOffOn, 0x400, DWTEvent::DebugSetWorldTitleFlag, NULL},
    {"\x83" "f\x83X\x83" "e\x83j\x81[         :%s", &DebugWork::g_dw.atelwork.iWorldIntro, 4, {0}, 0, 0.0f, 1.0f, 0.0f, szOffOn, 0x800, DWTEvent::DebugSetWorldTitleFlag, NULL},
    {"\x96Y\x8bp\x82\xcc\x8f\xe9           :%s", &DebugWork::g_dw.atelwork.iWorldIntro, 4, {0}, 0, 0.0f, 1.0f, 0.0f, szOffOn, 0x1000, DWTEvent::DebugSetWorldTitleFlag, NULL},
    {"\xf9S----------\xf9V\x83\x8a\x83N\xf9S---------", &CDebugWindow::none, 1, {0}, 2, 0.0f, 0.0f, 0.0f, NULL, 0, NULL, NULL},
    {"\x83g\x83\x89\x83\x94\x83@\x81[\x83X       :%s", &DebugWork::g_dw.atelwork.iWorldIntro, 4, {0}, 0, 0.0f, 1.0f, 0.0f, szOffOn, 0x10000, DWTEvent::DebugSetWorldTitleFlag, NULL},
    {"\x83" "A\x83O\x83\x89\x83o           :%s", &DebugWork::g_dw.atelwork.iWorldIntro, 4, {0}, 0, 0.0f, 1.0f, 0.0f, szOffOn, 0x20000, DWTEvent::DebugSetWorldTitleFlag, NULL},
    {"\x83I\x83\x8a\x83\x93\x83|\x83X         :%s", &DebugWork::g_dw.atelwork.iWorldIntro, 4, {0}, 0, 0.0f, 1.0f, 0.0f, szOffOn, 0x40000, DWTEvent::DebugSetWorldTitleFlag, NULL},
    {"\x83\x8f\x83\x93\x83_\x81[\x83\x89\x83\x93\x83h     :%s", &DebugWork::g_dw.atelwork.iWorldIntro, 4, {0}, 0, 0.0f, 1.0f, 0.0f, szOffOn, 0x80000, DWTEvent::DebugSetWorldTitleFlag, NULL},
    {"\x83\x82\x83\x93\x83X\x83g\x83\x8d         :%s", &DebugWork::g_dw.atelwork.iWorldIntro, 4, {0}, 0, 0.0f, 1.0f, 0.0f, szOffOn, 0x100000, DWTEvent::DebugSetWorldTitleFlag, NULL},
    {"\x83n\x83\x8d\x83" "E\x83" "B\x83\x93         :%s", &DebugWork::g_dw.atelwork.iWorldIntro, 4, {0}, 0, 0.0f, 1.0f, 0.0f, szOffOn, 0x200000, DWTEvent::DebugSetWorldTitleFlag, NULL},
    {"\x83" "A\x83g\x83\x89\x83\x93\x83" "e\x83" "B\x83J     :%s", &DebugWork::g_dw.atelwork.iWorldIntro, 4, {0}, 0, 0.0f, 1.0f, 0.0f, szOffOn, 0x400000, DWTEvent::DebugSetWorldTitleFlag, NULL},
    {"\x83l\x83o\x81[\x83\x89\x83\x93\x83h       :%s", &DebugWork::g_dw.atelwork.iWorldIntro, 4, {0}, 0, 0.0f, 1.0f, 0.0f, szOffOn, 0x800000, DWTEvent::DebugSetWorldTitleFlag, NULL},
    {"\x83z\x83\x8d\x83" "E\x83o\x83X\x83" "e\x83" "B\x83I\x83\x93 :%s", &DebugWork::g_dw.atelwork.iWorldIntro, 4, {0}, 0, 0.0f, 1.0f, 0.0f, szOffOn, 0x1000000, DWTEvent::DebugSetWorldTitleFlag, NULL},
    {"100\x83G\x81[\x83J\x81[        :%s", &DebugWork::g_dw.atelwork.iWorldIntro, 4, {0}, 0, 0.0f, 1.0f, 0.0f, szOffOn, 0x2000000, DWTEvent::DebugSetWorldTitleFlag, NULL},
    {"\x83g\x83\x8f\x83" "C\x83\x89\x83" "C\x83g       :%s", &DebugWork::g_dw.atelwork.iWorldIntro, 4, {0}, 0, 0.0f, 1.0f, 0.0f, szOffOn, 0x4000000, DWTEvent::DebugSetWorldTitleFlag, NULL},
    {"\x83" "f\x83X\x83" "e\x83j\x81[         :%s", &DebugWork::g_dw.atelwork.iWorldIntro, 4, {0}, 0, 0.0f, 1.0f, 0.0f, szOffOn, 0x8000000, DWTEvent::DebugSetWorldTitleFlag, NULL},
    {"\x96Y\x8bp\x82\xcc\x8f\xe9           :%s", &DebugWork::g_dw.atelwork.iWorldIntro, 4, {0}, 0, 0.0f, 1.0f, 0.0f, szOffOn, 0x10000000, DWTEvent::DebugSetWorldTitleFlag, NULL},
    {NULL, NULL, 0, {0}, 0, 0.0f, 0.0f, 0.0f, NULL, 0, NULL, NULL},
};

static DEBUG_WINDOW_TBL debugWinWorldIntro = {"\x83\x8f\x81[\x83\x8b\x83h\x83^\x83" "C\x83g\x83\x8b", -106, -(SCREEN_HEIGHT >> 1) + 150, 15, 29, 12, 0, 0, DWTEvent::debugWinWorldIntroLine, NULL, DWTEvent::DebugAtelWorkDef};

static BOOL DebugSetGameWorkParam(CDebugWindow* pObj, INT inY, INT inParam) {
    if (inParam & 0x180) {
        switch (inY) {
        case 17:
            CGameWork::setFlag(1, 0x0, DebugWork::g_dw.atelwork.iPlayerDispOff);
            break;
        case 18:
            CGameWork::setFlag(1, 0x2, DebugWork::g_dw.atelwork.iBossKilled);
            break;
        case 21:
            CGameWork::setFlag(1, 0x5, DebugWork::g_dw.atelwork.iNotPcControl);
            break;
        case 22:
            CGameWork::setFlag(1, 0x6, DebugWork::g_dw.atelwork.iNotPause);
            break;
        case 26:
            CGameWork::setFlag(1, 0x25, DebugWork::g_dw.atelwork.iNotHartBtl);
            break;
        case 27:
            CGameWork::setFlag(1, 0x2C, DebugWork::g_dw.atelwork.iIsBtlTutorial);
            break;
        case 28:
            CGameWork::setFlag(1, 0x37, DebugWork::g_dw.atelwork.iIsWorldIntro);
            break;
        }
    }
    return TRUE;
}

static DEBUG_WINDOW_LINE debugWinAtelWorkDefLine[30] = {
    {"\xf9S----------\xf9VWLV_STORAGE\xf9S---------", &CDebugWindow::none, 1, {0}, 2, 0.0f, 0.0f, 0.0f, NULL, 0, NULL, NULL},
    {"\x83\x8f\x81[\x83\x8b\x83h\x83^\x83" "C\x83g\x83\x8b...", &CDebugWindow::none, 1, {0}, 0, 0.0f, 0.0f, 0.0f, NULL, 0, NULL, &DWTEvent::debugWinWorldIntro},
    {"\x83`\x83\x85\x81[\x83g\x83\x8a\x83" "A\x83\x8b...", &CDebugWindow::none, 1, {0}, 0, 0.0f, 0.0f, 0.0f, NULL, 0, NULL, &DWTEvent::debugWinTuEnd},
    {"\x83J\x81[\x83h\x89\xf0\x8b\xd6...", &CDebugWindow::none, 1, {0}, 0, 0.0f, 0.0f, 0.0f, NULL, 0, NULL, &DWTEvent::debugWinCardPossible},
    {"\x83t\x83\x8c\x83\x93\x83h\x89\xf0\x8b\xd6...", &CDebugWindow::none, 1, {0}, 0, 0.0f, 0.0f, 0.0f, NULL, 0, NULL, &DWTEvent::debugWinFriendPossible},
    {"\x82P\x82O\x82O\x83G\x81[\x83J\x81[...", &CDebugWindow::none, 1, {0}, 0, 0.0f, 0.0f, 0.0f, NULL, 0, NULL, &DWTEvent::debugWinPooh},
    {"\x95W\x8f\x80\x83X\x83L\x83\x8b...", &CDebugWindow::none, 1, {0}, 0, 0.0f, 0.0f, 0.0f, NULL, 0, NULL, &DWTEvent::debugWinPcSkill},
    {"\x83\x80\x81[\x83r\x81[\x83\\\x83\x89...", &CDebugWindow::none, 1, {0}, 0, 0.0f, 0.0f, 0.0f, NULL, 0, NULL, &DWTEvent::debugWinMovie},
    {"\x83\x80\x81[\x83r\x81[\x83\x8a\x83N...", &CDebugWindow::none, 1, {0}, 0, 0.0f, 0.0f, 0.0f, NULL, 0, NULL, &DWTEvent::debugWinMovieRiku},
    {"\x82j\x82g\x82Q\x83J\x81[\x83h\x93\xfc\x8e\xe8...", &CDebugWindow::none, 1, {0}, 0, 0.0f, 0.0f, 0.0f, NULL, 0, NULL, &DWTEvent::debugWinKH2CardGet},
    {"\x83W\x83~\x83j\x81[\x83\x81\x83\x82...", &CDebugWindow::none, 1, {0}, 0, 0.0f, 0.0f, 0.0f, NULL, 0, NULL, &DWTEvent::debugWinJiminy},
    {"\x83Q\x81[\x83\x80\x8f\xee\x95\xf1...", &CDebugWindow::none, 1, {0}, 0, 0.0f, 0.0f, 0.0f, NULL, 0, NULL, &DWTEvent::debugGameInfomation},
    {"\x95\xf3\x94\xa0...", &CDebugWindow::none, 1, {0}, 0, 0.0f, 0.0f, 0.0f, NULL, 0, NULL, &DWTEvent::debugKeyTresur},
    {"\x83o\x83g\x83\x8b\x83\x8c\x83|\x81[\x83g...", &CDebugWindow::none, 1, {0}, 0, 0.0f, 0.0f, 0.0f, NULL, 0, NULL, &DWTEvent::debugBtlReport},
    {"\xf9S----------\xf9VWLV_GLOBAL\xf9S---------", &CDebugWindow::none, 1, {0}, 2, 0.0f, 0.0f, 0.0f, NULL, 0, NULL, NULL},
    {"BOSS\x83" "C\x83x\x83\x93\x83g\x83X\x83" "e\x83" "b\x83v : %d", &DebugWork::g_dw.atelwork.iBossEventStep, 4, {0}, 0, 0.0f, 0.0f, 0.0f, 0, 0, NULL, NULL},
    {"\x83\x81\x83j\x83\x85\x81[\x82\xcc\x8e\xed\x97\xde       : %s", &DebugWork::g_dw.atelwork.iKindMenu, 1, {0}, 0, 0.0f, 0.0f, 0.0f, &szMenuKind[DebugWork::g_dw.atelwork.iKindMenu], 0, NULL, NULL},
    {"\x83v\x83\x8c\x83" "C\x83\x84\x81[\x94\xf1\x95\\\x8e\xa6     : %s", &DebugWork::g_dw.atelwork.iPlayerDispOff, 1, {0}, 0, 0.0f, 1.0f, 1.0f, szOffOn, 0, DWTEvent::DebugSetGameWorkParam, NULL},
    {"\x82" "a\x82n\x82r\x82r\x8e\x80\x96S         : %s", &DebugWork::g_dw.atelwork.iBossKilled, 1, {0}, 0, 0.0f, 1.0f, 1.0f, szOffOn, 0, DWTEvent::DebugSetGameWorkParam, NULL},
    {"\x83" "C\x83x\x83\x93\x83g\x83X\x83L\x83" "b\x83v\x95s\x89\xc2 : %s", &DebugWork::g_dw.atelwork.iNotEventSkip, 1, {0}, 0, 0.0f, 0.0f, 0.0f, szOffOn, 0, NULL, NULL},
    {"ATEL\x83" "C\x83x\x83\x93\x83g\x92\x86       : %s", &DebugWork::g_dw.atelwork.iIsAtelEvent, 1, {0}, 0, 0.0f, 0.0f, 0.0f, szOffOn, 0, NULL, NULL},
    {"PC\x91\x80\x8d\xec\x95s\x89\xc2           : %s", &DebugWork::g_dw.atelwork.iNotPcControl, 1, {0}, 0, 0.0f, 1.0f, 1.0f, szOffOn, 0, DWTEvent::DebugSetGameWorkParam, NULL},
    {"PAUSE\x8b\xd6\x8e~            : %s", &DebugWork::g_dw.atelwork.iNotPause, 1, {0}, 0, 0.0f, 1.0f, 1.0f, szOffOn, 0, DWTEvent::DebugSetGameWorkParam, NULL},
    {"\x83`\x83\x85\x81[\x83g\x83\x8a\x83" "A\x83\x8b\x92\x86     : %s", &DebugWork::g_dw.atelwork.iIsTutorial, 1, {0}, 0, 0.0f, 0.0f, 0.0f, szOffOn, 0, NULL, NULL},
    {"\x83\x8b\x81[\x83\x80\x88\xda\x93\xae\x8b\xd6\x8e~       : %s", &DebugWork::g_dw.atelwork.iNotRoomJump, 1, {0}, 0, 0.0f, 0.0f, 0.0f, szOffOn, 0, NULL, NULL},
    {"\x83\x8b\x81[\x83\x80" "Creation\x8b\xd6\x8e~   : %s", &DebugWork::g_dw.atelwork.iNotRoomCreation, 1, {0}, 0, 0.0f, 0.0f, 0.0f, szOffOn, 0, NULL, NULL},
    {"\x83n\x81[\x83g\x8fo\x82\xb3\x82\xc8\x82\xa2       : %s", &DebugWork::g_dw.atelwork.iNotHartBtl, 1, {0}, 0, 0.0f, 1.0f, 1.0f, szOffOn, 0, DWTEvent::DebugSetGameWorkParam, NULL},
    {"BTL\x83`\x83\x85\x81[\x83g\x83\x8a\x83" "A\x83\x8b\x92\x86  : %s", &DebugWork::g_dw.atelwork.iIsBtlTutorial, 1, {0}, 0, 0.0f, 1.0f, 1.0f, szOffOn, 0, DWTEvent::DebugSetGameWorkParam, NULL},
    {"\x83\x8f\x81[\x83\x8b\x83h\x83^\x83" "C\x83g\x83\x8b\x92\x86   : %s", &DebugWork::g_dw.atelwork.iIsWorldIntro, 1, {0}, 0, 0.0f, 1.0f, 1.0f, szOffOn, 0, DWTEvent::DebugSetGameWorkParam, NULL},
    {NULL, NULL, 0, {0}, 0, 0.0f, 0.0f, 0.0f, 0, 0, NULL, NULL},
};

static DEBUG_WINDOW_TBL debugWinAtelWorkDef = {"ATEL WORK", -106, -(SCREEN_HEIGHT >> 1) + 150, 20, 30, 12, 0, 0, DWTEvent::debugWinAtelWorkDefLine, NULL, DWTEvent::DebugAtelWorkDef};

static INT debugWinNPCMainlCall(CDebugWindow* pObj, INT inParam) {
    if (inParam & 1) {
        DebugWork::g_dw.event.pClass = NULL;
        DebugWork::g_dw.event.pEventlineClass = NULL;
    }
    return TRUE;
}

static DEBUG_WINDOW_LINE debugWinEventSelectPlayerLine[6] = {
    {"coord axis\x81" "F%s", &DebugWork::g_dw.event.iAtelCoord, 1, {0}, 0, 0.0f, 1.0f, 1.0f, szOffOn, 0, NULL, NULL},
    {"actor pos \x81" "F%s", &DebugWork::g_dw.event.iNpcCoordDisp, 1, {0}, 0, 0.0f, 1.0f, 1.0f, szOffOn, 0, DWTEvent::DebugCreateNpcWindow, NULL},
    {"actor list...", &CDebugWindow::none, 1, {0}, 0, 0.0f, 0.0f, 0.0f, NULL, 0, DWTEvent::DebugCreateNpcWindow, &DWTEvent::debugWinNpcList},
    {"\x83" "C\x83x\x83\x93\x83g\x83\x89\x83" "C\x83\x93\x81" "F%s", &DebugWork::g_dw.event.iEventline, 1, {0}, 0, 0.0f, 1.0f, 1.0f, szOffOn, 0, DWTEvent::DebugCreateEventLineDisp, NULL},
    {"\x83t\x83\x89\x83O\x83`\x83" "F\x83" "b\x83N...", &CDebugWindow::none, 1, {0}, 0, 0.0f, 1.0f, 1.0f, szOffOn, 0, NULL, &DWTEvent::debugWinAtelWorkDef},
    {NULL, NULL, 0, {0}, 0, 0.0f, 0.0f, 0.0f, NULL, 0, NULL, NULL},
};

static DEBUG_WINDOW_TBL debugWinEventPlayer = {"ATEL\x81@CONFIG", -206, -(SCREEN_HEIGHT >> 1) + 50, 12, 16, 14, 0, 0, DWTEvent::debugWinEventSelectPlayerLine, NULL, DWTEvent::debugWinNPCMainlCall};

static DEBUG_WINDOW_LINE debugNpcInfoLine[7] = {
    {"PosX:  %.2f", &DebugWork::g_dw.event.vPos.x, 4, {0}, 3, 0.0f, 0.0f, 0.0f, NULL, 0, NULL, NULL},
    {"PosY:  %.2f", &DebugWork::g_dw.event.vPos.y, 4, {0}, 3, 0.0f, 0.0f, 0.0f, NULL, 0, NULL, NULL},
    {"PosZ:  %.2f", &DebugWork::g_dw.event.vPos.z, 4, {0}, 3, 0.0f, 0.0f, 0.0f, NULL, 0, NULL, NULL},
    {"RotX:  %.2f", &DebugWork::g_dw.event.vRot.x, 4, {0}, 3, 0.0f, 0.0f, 0.0f, NULL, 0, NULL, NULL},
    {"RotY:  %.2f", &DebugWork::g_dw.event.vRot.y, 4, {0}, 3, 0.0f, 0.0f, 0.0f, NULL, 0, NULL, NULL},
    {"RotZ:  %.2f", &DebugWork::g_dw.event.vRot.z, 4, {0}, 3, 0.0f, 0.0f, 0.0f, NULL, 0, NULL, NULL},
    {NULL, NULL, 0, {0}, 0, 0.0f, 0.0f, 0.0f, NULL, 0, NULL, NULL},
};

} // namespace DWTEvent

namespace DWTGimThrow {

char* szPrizeKind[2] = {"\x83{\x81[\x83\x8b", "\x83J\x81[\x83h"};

char* szPrizeDrop[5] = {"\x82\xcd\x82\xb8\x82\xea", "HP\x83{\x81[\x83\x8b", "MP\x83{\x81[\x83\x8b", "\x83J\x81[\x83h", "\x83o\x83\x8c\x83\x8b"};

static BOOL DebugGimThrowParamSet(CDebugWindow* pObj, INT inY, INT inParam) {
    CGimmickManager* pCGimMan = (CGimmickManager*)CTaskMan::getSysTask(0x11);
    if (!pCGimMan) {
        return TRUE;
    }
    if (inParam & 1) {
        CTask* pM = CTaskMan::getSysTask(2);
        CTask* pR = CTaskMan::getSysTask(1);
        if (pM && pR) {
            DebugWork::g_dw.gimthrow.DirPow = pCGimMan->getThrowDirPow();
            DebugWork::g_dw.gimthrow.MaxAngle = pCGimMan->getThrowMaxAngle();
            DebugWork::g_dw.gimthrow.MinAngle = pCGimMan->getThrowMinAngle();
        }
        DebugWork::g_dw.gimPrize.Pow = 20.0f;
        DebugWork::g_dw.gimPrize.MinSpeed = 0.18f;
        DebugWork::g_dw.gimPrize.MaxRotX = 90;
        DebugWork::g_dw.gimPrize.MinRotX = 0;
        DebugWork::g_dw.gimPrize.MaxRotY = 180;
        DebugWork::g_dw.gimPrize.MinRotY = -180;
        DebugWork::g_dw.gimPrize.Kind = 0;
        DebugWork::g_dw.gimPrize.Num = 1;
    }
    pCGimMan->setThrowDirPow(DebugWork::g_dw.gimthrow.DirPow);
    pCGimMan->setThrowMaxAngle(DebugWork::g_dw.gimthrow.MaxAngle);
    pCGimMan->setThrowMinAngle(DebugWork::g_dw.gimthrow.MinAngle);
    if (inParam & 0x20) {
        if (inY == 14) {
            PRIZE_PARAM param;
            param.uiFlag = 0;
            param.uiPrizeFlag = 0;
            param.vPos = g_pPlayer->getPos();
            param.vPos.y += 1.5f;
            param.iNumber = 0;
            if (DebugWork::g_dw.gimPrize.Kind) {
                param.PrizeKind = 1;
            } else {
                param.PrizeKind = 0xD4;
            }
            for (int i = 0; i < DebugWork::g_dw.gimPrize.Num; i++) {
                INT max = DebugWork::g_dw.gimPrize.MaxRotX;
                INT min = DebugWork::g_dw.gimPrize.MinRotX;
                param.vRot.x = -((RND::genrand_int31() % (max - min) + min) * 0.017453292f);
                max = DebugWork::g_dw.gimPrize.MaxRotY;
                min = DebugWork::g_dw.gimPrize.MinRotY;
                param.vRot.y = (RND::genrand_int31() % (max - min) + min) * 0.017453292f;
                param.vRot.z = 0.0f;
                param.fSpeed = RND::genrand_real1() / DebugWork::g_dw.gimPrize.Pow + DebugWork::g_dw.gimPrize.MinSpeed;
                CEntityFactory::getInstance()->createPrize(param, NULL);
            }
        }
    }
    return TRUE;
}

static BOOL DebugGimDropProbWindow(CDebugWindow* pObj, INT inY, INT inParam) {
    if (CTaskMan::getSysTask(0x11) && (inParam & 0x180)) {
        if (DebugWork::g_dw.gimPrize.iDisp == 1) {
            CGimmickProbDebug* p = new CGimmickProbDebug(CTaskMan::getSysTask(1));
            DebugWork::g_dw.gimPrize.pClass = (INT*)p;
        }
    }
    return TRUE;
}

static INT debugWinThrowTestCallBack(CDebugWindow* pObj, INT inParam) {
    if (inParam & 1) {
        DebugWork::g_dw.gimPrize.pClass = NULL;
        DebugWork::g_dw.gimPrize.iDisp = 0;
    }
    return TRUE;
}

static DEBUG_WINDOW_LINE debugWinThrowTestLine[20] = {
    {"\xf9S--\xf9V\x82\xc8\x82\xb0\x82\xe9\x82\xe2\x82\xc2\xf9S--", &CDebugWindow::none, 1, {0}, 2, 0.0f, 0.0f, 0.0f, NULL, 0, NULL, NULL},
    {"\x95\xfb\x8c\xfc\x91\xac\x93x\x81" "F%2.2f", &DebugWork::g_dw.gimthrow.DirPow, 4, {0}, 1, -10.0f, 10.0f, 0.01f, NULL, 0, DWTGimThrow::DebugGimThrowParamSet, NULL},
    {"\x8d\xc5\x91\xe5\x8ap\x93x\x81" "F%2.1f", &DebugWork::g_dw.gimthrow.MaxAngle, 4, {0}, 1, -90.0f, 90.0f, 0.1f, NULL, 0, DWTGimThrow::DebugGimThrowParamSet, NULL},
    {"\x8d\xc5\x92\xe1\x8ap\x93x\x81" "F%2.1f", &DebugWork::g_dw.gimthrow.MinAngle, 4, {0}, 1, -90.0f, 90.0f, 0.1f, NULL, 0, DWTGimThrow::DebugGimThrowParamSet, NULL},
    {"\xf9S--\xf9V\x83v\x83\x89\x83" "C\x83Y\x90\x81\x82\xc1\x94\xf2\x82\xd1\xf9S--", &CDebugWindow::none, 1, {0}, 2, 0.0f, 0.0f, 0.0f, NULL, 0, NULL, NULL},
    {"\xf9Vrand(0.0 - 1.0) / pow + msp", &CDebugWindow::none, 1, {0}, 2, 0.0f, 0.0f, 0.0f, NULL, 0, NULL, NULL},
    {"pow\x81" "F%4.1f", &DebugWork::g_dw.gimPrize.Pow, 4, {0}, 1, 0.0f, 100.0f, 0.1f, NULL, 0, NULL, NULL},
    {"msp\x81" "F%4.2f", &DebugWork::g_dw.gimPrize.MinSpeed, 4, {0}, 1, 0.0f, 10.0f, 0.01f, NULL, 0, NULL, NULL},
    {"MAX ROT X\x81" "F%3d", &DebugWork::g_dw.gimPrize.MaxRotX, 4, {0}, 0, -90.0f, 90.0f, 1.0f, NULL, 0, NULL, NULL},
    {"MIN ROT X\x81" "F%3d", &DebugWork::g_dw.gimPrize.MinRotX, 4, {0}, 0, -90.0f, 90.0f, 1.0f, NULL, 0, NULL, NULL},
    {"MAX ROT Y\x81" "F%3d", &DebugWork::g_dw.gimPrize.MaxRotY, 4, {0}, 0, -180.0f, 180.0f, 1.0f, NULL, 0, NULL, NULL},
    {"MIN ROT Y\x81" "F%3d", &DebugWork::g_dw.gimPrize.MinRotY, 4, {0}, 0, -180.0f, 180.0f, 1.0f, NULL, 0, NULL, NULL},
    {"\x82\xb5\x82\xe3\x82\xe9\x82\xa2 :%s", &DebugWork::g_dw.gimPrize.Kind, 4, {0}, 0, 0.0f, 1.0f, 1.0f, &szPrizeDrop[DebugWork::g_dw.gimPrize.Kind], 0, NULL, NULL},
    {"\x82\xa9\x82\xb8     :%d", &DebugWork::g_dw.gimPrize.Num, 4, {0}, 0, 0.0f, 10.0f, 1.0f, NULL, 0, NULL, NULL},
    {"\x82\xcd\x82\xc1\x82\xb5\x82\xe1", &CDebugWindow::none, 1, {0}, 0, 0.0f, 0.0f, 0.0f, NULL, 0, DWTGimThrow::DebugGimThrowParamSet, NULL},
    {"\xf9S--\xf9V\x82\xbf\x82\xe3\x82\xa4\x82\xb9\x82\xf1\x8f\xee\x95\xf1\xf9S--", &CDebugWindow::none, 1, {0}, 2, 0.0f, 0.0f, 0.0f, NULL, 0, NULL, NULL},
    {"\x83M\x83~\x83" "b\x83N\x82\xbf\x82\xe3\x82\xa4\x82\xb9\x82\xf1 \x81" "F%s", &DebugWork::g_dw.gimPrize.iDisp, 1, {0}, 0, 0.0f, 1.0f, 1.0f, szOffOn, 0, DWTGimThrow::DebugGimDropProbWindow, NULL},
    {"\xf9S--\xf9V\x83v\x83\x89\x83" "C\x83Y\x8c\xc5\x92\xe8\xf9S--", &CDebugWindow::none, 1, {0}, 2, 0.0f, 0.0f, 0.0f, NULL, 0, NULL, NULL},
    {"%s", &DebugWork::g_dw.gimPrize.Kind, 4, {0}, 0, 0.0f, 4.0f, 1.0f, &szPrizeDrop[DebugWork::g_dw.gimPrize.Kind], 0, NULL, NULL},
    {NULL, NULL, 0, {0}, 0, 0.0f, 0.0f, 0.0f, 0, 0, NULL, NULL},
};

static DEBUG_WINDOW_TBL debugWinThrowTest = {"ATEL\x81@CONFIG", -176, -(SCREEN_HEIGHT >> 1) + 150, 15, 20, 12, 0, 0, DWTGimThrow::debugWinThrowTestLine, NULL, DWTGimThrow::debugWinThrowTestCallBack};

} // namespace DWTGimThrow

#include "Game/Debug/DWTBGM.h"

namespace DWTFog {

static BOOL DebugFogParamSet(CDebugWindow* pObj, INT inY, INT inParam) {
    if (inParam & 1) {
        CTask* pM = CTaskMan::getSysTask(2);
        CTask* pR = CTaskMan::getSysTask(1);
        if (pM && pR) {
            DebugWork::g_dw.fog.Near = (SHORT)CWorldEnv::getFogNear();
            DebugWork::g_dw.fog.Far = (SHORT)CWorldEnv::getFogFar();
            IVECTOR iv = CWorldEnv::getFogColor();
            DebugWork::g_dw.fog.r = iv.x;
            DebugWork::g_dw.fog.g = iv.y;
            DebugWork::g_dw.fog.b = iv.z;
            DebugWork::g_dw.fog.max = 255;
            DebugWork::g_dw.fog.min = 0;
        }
    }
    CTask* pM = CTaskMan::getSysTask(2);
    CTask* pR = CTaskMan::getSysTask(1);
    if (pM && pR) {
        switch (inY) {
        case 0:
            if (inParam & 0x100) {
                DebugWork::g_dw.fog.Near += DebugWork::g_dw.fog.inc;
            }
            if (inParam & 0x80) {
                DebugWork::g_dw.fog.Near -= DebugWork::g_dw.fog.inc;
            }
            if (DebugWork::g_dw.fog.Near <= 0) {
                DebugWork::g_dw.fog.Near = 0;
            }
            CWorldEnv::setFogParam(DebugWork::g_dw.fog.Near, DebugWork::g_dw.fog.Far, DebugWork::g_dw.fog.max, DebugWork::g_dw.fog.min);
            break;
        case 1:
            if (inParam & 0x100) {
                DebugWork::g_dw.fog.Far += DebugWork::g_dw.fog.inc;
            }
            if (inParam & 0x80) {
                DebugWork::g_dw.fog.Far -= DebugWork::g_dw.fog.inc;
            }
            if (DebugWork::g_dw.fog.Far <= 0) {
                DebugWork::g_dw.fog.Far = 0;
            }
            CWorldEnv::setFogParam(DebugWork::g_dw.fog.Near, DebugWork::g_dw.fog.Far, DebugWork::g_dw.fog.max, DebugWork::g_dw.fog.min);
            break;
        case 2:
        case 3:
        case 4:
            CWorldEnv::setFogColor(DebugWork::g_dw.fog.r, DebugWork::g_dw.fog.g, DebugWork::g_dw.fog.b);
            break;
        case 5:
        case 6:
            CWorldEnv::setFogParam(DebugWork::g_dw.fog.Near, DebugWork::g_dw.fog.Far, DebugWork::g_dw.fog.max, DebugWork::g_dw.fog.min);
            break;
        case 7:
            if (inParam & 0x100) {
                DebugWork::g_dw.fog.inc *= 10;
            }
            if (inParam & 0x80) {
                DebugWork::g_dw.fog.inc /= 10;
            }
            if (DebugWork::g_dw.fog.inc <= 0) {
                DebugWork::g_dw.fog.inc = 1;
            }
            break;
        }
    }
    return TRUE;
}

static DEBUG_WINDOW_LINE debugWinFogTestLine[9] = {
    {"NEARE(\x8e\xe8\x91O)\x81" "F%d\x81@m", &DebugWork::g_dw.fog.Near, 2, {0}, 0, 0.0f, 9000.0f, 0.0f, NULL, 0, DWTFog::DebugFogParamSet, NULL},
    {"FAR(\x89\x9c)\x81@\x81@\x81" "F%d\x81@m", &DebugWork::g_dw.fog.Far, 2, {0}, 0, 0.0f, 9000.0f, 0.0f, NULL, 0, DWTFog::DebugFogParamSet, NULL},
    {"RED\x81@\x81@    \x81" "F%d", &DebugWork::g_dw.fog.r, 2, {0}, 0, 0.0f, 255.0f, 1.0f, NULL, 0, DWTFog::DebugFogParamSet, NULL},
    {"GREEN      \x81" "F%d", &DebugWork::g_dw.fog.g, 2, {0}, 0, 0.0f, 255.0f, 1.0f, NULL, 0, DWTFog::DebugFogParamSet, NULL},
    {"BLUE     \x81@\x81" "F%d", &DebugWork::g_dw.fog.b, 2, {0}, 0, 0.0f, 255.0f, 1.0f, NULL, 0, DWTFog::DebugFogParamSet, NULL},
    {"MAX\x81@\x81@    \x81" "F%d", &DebugWork::g_dw.fog.max, 2, {0}, 0, 0.0f, 255.0f, 1.0f, NULL, 0, DWTFog::DebugFogParamSet, NULL},
    {"MIN\x81@\x81@\x81@  \x81" "F%d", &DebugWork::g_dw.fog.min, 2, {0}, 0, 0.0f, 255.0f, 1.0f, NULL, 0, DWTFog::DebugFogParamSet, NULL},
    {"\x91\x9d\x8c\xb8\x92l     \x81" "F%d", &DebugWork::g_dw.fog.inc, 2, {0}, 0, 1.0f, 10000.0f, 0.0f, NULL, 0, DWTFog::DebugFogParamSet, NULL},
    {NULL, NULL, 0, {0}, 0, 0.0f, 0.0f, 0.0f, NULL, 0, NULL, NULL},
};

static DEBUG_WINDOW_TBL debugWinFogTest = {"\x83t\x83H\x83O\x83" "e\x83X\x83g", -156, -(SCREEN_HEIGHT >> 1) + 150, 12, 8, 14, 0, 0, DWTFog::debugWinFogTestLine, NULL, NULL};

} // namespace DWTFog

namespace DWTBlur {

static BOOL DebugBlurParamSet(CDebugWindow* pObj, INT inY, INT inParam) {
    CTask* pM = CTaskMan::getSysTask(2);
    CTask* pR = CTaskMan::getSysTask(1);
    if ((inParam & 0x10) && pM && pR) {
        CPackMan::setFrameEffect(3, DebugWork::g_dw.blur.Xofs, DebugWork::g_dw.blur.Yofs, DebugWork::g_dw.blur.Alpha);
    }
    if (inParam & 0x20) {
        if (inY == 3) {
            CPackMan::setFrameEffect(0, 0, 0, 0);
        }
    }
    return TRUE;
}

static DEBUG_WINDOW_LINE debugWinBlurTestLine[5] = {
    {"X_offset  \x81" "F%d", &DebugWork::g_dw.blur.Xofs, 2, {0}, 0, 0.0f, 500.0f, 1.0f, NULL, 0, DWTBlur::DebugBlurParamSet, NULL},
    {"Y_offset  \x81" "F%d", &DebugWork::g_dw.blur.Yofs, 2, {0}, 0, 0.0f, 500.0f, 1.0f, NULL, 0, DWTBlur::DebugBlurParamSet, NULL},
    {"Alpha     \x81" "F%d", &DebugWork::g_dw.blur.Alpha, 2, {0}, 0, 0.0f, 255.0f, 1.0f, NULL, 0, DWTBlur::DebugBlurParamSet, NULL},
    {"\x83u\x83\x89\x81[\x82n\x82" "e\x82" "e", &CDebugWindow::none, 1, {0}, 0, 0.0f, 0.0f, 0.0f, NULL, 0, DWTBlur::DebugBlurParamSet, NULL},
    {NULL, NULL, 0, {0}, 0, 0.0f, 0.0f, 0.0f, NULL, 0, NULL, NULL},
};

static DEBUG_WINDOW_TBL debugWinBlurTest = {"\x83u\x83\x89\x81[\x83" "e\x83X\x83g", -156, -(SCREEN_HEIGHT >> 1) + 150, 10, 5, 14, 0, 0, DWTBlur::debugWinBlurTestLine, NULL, NULL};

} // namespace DWTBlur

static DEBUG_WINDOW_LINE debugWinEnemyLine[20] = {
    {"\x83\x80\x83" "e\x83L                        %s", &DebugWork::g_dw.enemy.uiFlag, 4, {0}, 0, 0.0f, 1.0f, 1.0f, szOffOn, 1, NULL, NULL},
    {"\x83" "C\x83`\x83Q\x83L\x83V                    %s", &DebugWork::g_dw.enemy.uiFlag, 4, {0}, 0, 0.0f, 1.0f, 1.0f, szOffOn, 2, NULL, NULL},
    {"\x83[\x83\x93\x83\x81\x83" "c                      %s", &DebugWork::g_dw.enemy.uiFlag, 4, {0}, 0, 0.0f, 1.0f, 1.0f, szOffOn, 0x1000, NULL, NULL},
    {"\x83J\x81[\x83h\x83o\x83g\x83\x8b                  %s", &DebugWork::g_dw.enemy.uiFlag, 4, {0}, 0, 0.0f, 1.0f, 1.0f, szOnOff, 0x10, NULL, NULL},
    {"\x83_\x83\x81\x81[\x83W\x82\xc8\x82\xb5\x82\xc5\x83" "E\x83S\x82\xab\x83g\x82\xdf\x82\xe9    %s", &DebugWork::g_dw.enemy.uiFlag, 4, {0}, 0, 0.0f, 1.0f, 1.0f, szOffOn, 0x20, NULL, NULL},
    {"\x83_\x83\x81\x81[\x83W\x82\xa0\x82\xe8\x82\xc5\x83" "E\x83S\x82\xab\x83g\x82\xdf\x82\xe9    %s", &DebugWork::g_dw.enemy.uiFlag, 4, {0}, 0, 0.0f, 1.0f, 1.0f, szOffOn, 0x400, NULL, NULL},
    {"\x83\x8f\x83U\x83Z\x83\x93\x83^\x83N                  %s", &DebugWork::g_dw.enemy.uiFlag, 4, {0}, 0, 0.0f, 1.0f, 1.0f, szOffOn, 8, NULL, NULL},
    {"\x83\x8f\x83U\x83o\x83\x93\x83S\x83" "E                  %d", &DebugWork::g_dw.enemy.sActNo, 2, {0}, 0, 0.0f, 8.0f, 1.0f, NULL, 0, NULL, NULL},
    {"\x83^\x83" "C\x83\x8c\x83" "c\x83Z\x83\x93\x83^\x83N               %s", &DebugWork::g_dw.enemy.uiFlag, 4, {0}, 0, 0.0f, 1.0f, 1.0f, szOffOn, 4, NULL, NULL},
    {"\x83^\x83" "C\x83\x8c\x83" "c\x83o\x83\x93\x83S\x83" "E              %d", &DebugWork::g_dw.enemy.sFormation, 2, {0}, 0, 0.0f, 31.0f, 1.0f, NULL, 0, NULL, NULL},
    {"\x83G\x83\x93\x83J\x83" "E\x83\x93\x83g                  %s", &DebugWork::g_dw.enemy.uiFlag, 4, {0}, 0, 0.0f, 1.0f, 1.0f, szOnOff, 0x40, NULL, NULL},
    {"\x83v\x83\x8c\x83~\x83" "A\x83\x80\x83v\x83\x89\x83" "C\x83Y            %s", &DebugWork::g_dw.enemy.uiFlag, 4, {0}, 0, 0.0f, 1.0f, 1.0f, szOffOn, 0x80, NULL, NULL},
    {"\x83\x8b\x81[\x83\x8c\x83" "b\x83g\x83v\x83\x89\x83" "C\x83Y            %s", &DebugWork::g_dw.enemy.uiFlag, 4, {0}, 0, 0.0f, 1.0f, 1.0f, szOffOn, 0x100, NULL, NULL},
    {"\x83M\x83~\x83" "b\x83N\x83J\x81[\x83h100%%           %s", &DebugWork::g_dw.enemy.uiFlag, 4, {0}, 0, 0.0f, 1.0f, 1.0f, szOffOn, 0x200, NULL, NULL},
    {"\x83G\x83l\x83~\x81[\x83J\x81[\x83h100%%           %s", &DebugWork::g_dw.enemy.uiFlag, 4, {0}, 0, 0.0f, 1.0f, 1.0f, szOffOn, 0x2000, NULL, NULL},
    {"HP\x83Z\x83" "b\x83" "e\x83" "C                    %s", &DebugWork::g_dw.enemy.uiFlag, 4, {0}, 0, 0.0f, 1.0f, 1.0f, szOffOn, 0x800, NULL, NULL},
    {"HP\x82\xe8\x82\xc2                        %d%%", &DebugWork::g_dw.enemy.iHpPercent, 4, {0}, 0, 0.0f, 100.0f, 1.0f, NULL, 0, NULL, NULL},
    {"\x83J\x81[\x83h\x83o\x83\x93\x83S\x83" "E\x83R\x83" "e\x83" "C          %s", &DebugWork::g_dw.enemy.uiFlag, 4, {0}, 0, 0.0f, 1.0f, 1.0f, szOffOn, 0x4000, NULL, NULL},
    {"\x83J\x81[\x83h                    %d", &DebugWork::g_dw.enemy.iCardNumber, 4, {0}, 0, 0.0f, 9.0f, 1.0f, NULL, 0, NULL, NULL},
    {NULL, NULL, 0, {0}, 0, 0.0f, 0.0f, 0.0f, NULL, 0, NULL, NULL},
};

DEBUG_WINDOW_TBL debugWinEnemy = {"\x83" "e\x83L\x83Z\x83" "b\x83" "e\x83" "C", -208, -160, 20, 20, 12, 0, 0, debugWinEnemyLine, NULL, NULL};

DEBUG_WINDOW_LINE debugWinMini100AcreLine[6] = {
    {"\x83\x8f\x83" "C\x83\x8b\x83h\x83_\x83" "C\x83u ...", &CDebugWindow::none, 1, {0}, 0, 0.0f, 0.0f, 0.0f, NULL, 0, NULL, &debugWinMini100AcreWD},
    {"\x83o\x83\x8b\x81[\x83\x93\x83O\x83\x89\x83" "C\x83_\x81[ ...", &CDebugWindow::none, 1, {0}, 0, 0.0f, 0.0f, 0.0f, NULL, 0, NULL, &debugWinMini100AcreBL},
    {"\x83" "e\x83" "B\x83K\x81[\x82\xc6\x83W\x83\x83\x83\x93\x83v ...", &CDebugWindow::none, 1, {0}, 0, 0.0f, 0.0f, 0.0f, NULL, 0, NULL, &debugWinMini100AcreTJ},
    {"\x82\xe2\x82\xb3\x82\xa2\x83p\x83j\x83" "b\x83N ...", &CDebugWindow::none, 1, {0}, 0, 0.0f, 0.0f, 0.0f, NULL, 0, NULL, &debugWinMini100AcreVP},
    {"\x82\xcd\x82\xbf\x83o\x83g\x83\x8b ...", &CDebugWindow::none, 1, {0}, 0, 0.0f, 0.0f, 0.0f, NULL, 0, NULL, &debugWinMini100AcreBB},
    {NULL, NULL, 0, {0}, 0, 0.0f, 0.0f, 0.0f, NULL, 0, NULL, NULL},
};

DEBUG_WINDOW_LINE debugWinMini100AcreWDLine[5] = {
    {"\x8f" "d\x97\xcd           % 2.4f", &DebugWork::g_dw.acre.fWD_Gravity, 4, {0}, 1, 0.0f, 1.0f, 0.0001f, NULL, 0, NULL, NULL},
    {"\x8a\xb5\x90\xab\x97\xcd         % 2.6f", &DebugWork::g_dw.acre.fWD_Rate, 4, {0}, 1, 0.0f, 1.0f, 0.000001f, NULL, 0, NULL, NULL},
    {"\x89\xc1\x82\xb3\x82\xf1\x83X\x83s\x81[\x83h % 2.6f", &DebugWork::g_dw.acre.fWD_AddSpeed, 4, {0}, 1, 0.0f, 1.0f, 0.000001f, NULL, 0, NULL, NULL},
    {"\x8d\xc5\x91\xe5\x83X\x83s\x81[\x83h   % 2.6f", &DebugWork::g_dw.acre.fWD_MaxSpeed, 4, {0}, 1, 0.0f, 1.0f, 0.0001f, NULL, 0, NULL, NULL},
    {NULL, NULL, 0, {0}, 0, 0.0f, 0.0f, 0.0f, NULL, 0, NULL, NULL},
};

DEBUG_WINDOW_LINE debugWinMini100AcreBLLine[9] = {
    {"\x96\xb3\x93G\x83t\x83\x89\x83O       %s", &DebugWork::g_dw.acre.unBL_Flag, 4, {0}, 0, 0.0f, 1.0f, 1.0f, szOffOn, 1, NULL, NULL},
    {"\x8f" "d\x97\xcd             % 2.3f", &DebugWork::g_dw.acre.fBL_Gravity, 4, {0}, 1, 0.0f, 1.0f, 0.001f, NULL, 0, NULL, NULL},
    {"\x8a\xb5\x90\xab\x97\xcdY          % 2.6f", &DebugWork::g_dw.acre.fBL_RateY, 4, {0}, 1, 0.0f, 1.0f, 0.000001f, NULL, 0, NULL, NULL},
    {"\x89\xc1\x82\xb3\x82\xf1\x83X\x83s\x81[\x83hY  % 2.6f", &DebugWork::g_dw.acre.fBL_AddSpeedY, 4, {0}, 1, 0.0f, 1.0f, 0.000001f, NULL, 0, NULL, NULL},
    {"\x8d\xc5\x91\xe5\x83X\x83s\x81[\x83hY    % 2.6f", &DebugWork::g_dw.acre.fBL_MaxSpeedY, 4, {0}, 1, 0.0f, 1.0f, 0.000001f, NULL, 0, NULL, NULL},
    {"\x8a\xb5\x90\xab\x97\xcdXZ         % 2.6f", &DebugWork::g_dw.acre.fBL_RateXZ, 4, {0}, 1, 0.0f, 1.0f, 0.000001f, NULL, 0, NULL, NULL},
    {"\x89\xc1\x82\xb3\x82\xf1\x83X\x83s\x81[\x83hXZ % 2.6f", &DebugWork::g_dw.acre.fBL_AddSpeedXZ, 4, {0}, 1, 0.0f, 1.0f, 0.000001f, NULL, 0, NULL, NULL},
    {"\x8d\xc5\x91\xe5\x83X\x83s\x81[\x83hXZ   % 2.6f", &DebugWork::g_dw.acre.fBL_MaxSpeedXZ, 4, {0}, 1, 0.0f, 1.0f, 0.000001f, NULL, 0, NULL, NULL},
    {NULL, NULL, 0, {0}, 0, 0.0f, 0.0f, 0.0f, NULL, 0, NULL, NULL},
};

char* Mini100AcreTJ_Mugen[4] = {"\x82\xc8\x82\xb5", "\x83\\\x83\x89\x82\xc6\x83v\x81[", "\x83" "e\x83" "B\x83K\x81[\x92\xca\x8f\xed", "\x83" "e\x83" "B\x83K\x81[\x89\xf1\x93]"};

DEBUG_WINDOW_LINE debugWinMini100AcreTJLine[22] = {
    {"\x82\xde\x82\xb0\x82\xf1\x83W\x83\x83\x83\x93\x83v  %s", &DebugWork::g_dw.acre.unTJ_Mugen, 4, {0}, 0, 0.0f, 3.0f, 1.0f, Mini100AcreTJ_Mugen, 0, NULL, NULL},
    {"\x82\xb1\x82\xbd\x82\xa6\x95\\\x8e\xa6      %s", &DebugWork::g_dw.acre.unTJ_Flag, 4, {0}, 0, 0.0f, 1.0f, 1.0f, szOffOn, 1, NULL, NULL},
    {"\x83\\\x83\x89\x82\xc6\x83v\x81[   \x92Z \x8f" "d\x97\xcd    % 2.4f", &DebugWork::g_dw.acre.fTJ_SoraGravity[0], 4, {0}, 1, 0.0f, 1.0f, 0.0001f, NULL, 0, NULL, NULL},
    {"\x83\\\x83\x89\x82\xc6\x83v\x81[   \x92Z \x8f\x89\x91\xac\x93x  % 2.4f", &DebugWork::g_dw.acre.fTJ_SoraFirstSpeedXZ[0], 4, {0}, 1, 0.0f, 1.0f, 0.0001f, NULL, 0, NULL, NULL},
    {"\x83\\\x83\x89\x82\xc6\x83v\x81[   \x92Z \x89\xc1\x91\xac\x93x  % 2.4f", &DebugWork::g_dw.acre.fTJ_SoraAccelSpeedXZ[0], 4, {0}, 1, -1.0f, 1.0f, 0.0001f, NULL, 0, NULL, NULL},
    {"\x83\\\x83\x89\x82\xc6\x83v\x81[   \x92\xb7 \x8f" "d\x97\xcd    % 2.4f", &DebugWork::g_dw.acre.fTJ_SoraGravity[1], 4, {0}, 1, 0.0f, 1.0f, 0.0001f, NULL, 0, NULL, NULL},
    {"\x83\\\x83\x89\x82\xc6\x83v\x81[   \x92\xb7 \x8f\x89\x91\xac\x93x  % 2.4f", &DebugWork::g_dw.acre.fTJ_SoraFirstSpeedXZ[1], 4, {0}, 1, 0.0f, 1.0f, 0.0001f, NULL, 0, NULL, NULL},
    {"\x83\\\x83\x89\x82\xc6\x83v\x81[   \x92\xb7 \x89\xc1\x91\xac\x93x  % 2.4f", &DebugWork::g_dw.acre.fTJ_SoraAccelSpeedXZ[1], 4, {0}, 1, -1.0f, 1.0f, 0.0001f, NULL, 0, NULL, NULL},
    {"\x83" "e\x83" "B\x83K\x81[\x92\xca\x8f\xed \x92Z \x8f" "d\x97\xcd    % 2.4f", &DebugWork::g_dw.acre.fTJ_TiggerGravity[0][0], 4, {0}, 1, 0.0f, 1.0f, 0.0001f, NULL, 0, NULL, NULL},
    {"\x83" "e\x83" "B\x83K\x81[\x92\xca\x8f\xed \x92Z \x8f\x89\x91\xac\x93x  % 2.4f", &DebugWork::g_dw.acre.fTJ_TiggerFirstSpeedXZ[0][0], 4, {0}, 1, 0.0f, 1.0f, 0.0001f, NULL, 0, NULL, NULL},
    {"\x83" "e\x83" "B\x83K\x81[\x92\xca\x8f\xed \x92Z \x89\xc1\x91\xac\x93x  % 2.4f", &DebugWork::g_dw.acre.fTJ_TiggerAccelSpeedXZ[0][0], 4, {0}, 1, -1.0f, 1.0f, 0.0001f, NULL, 0, NULL, NULL},
    {"\x83" "e\x83" "B\x83K\x81[\x92\xca\x8f\xed \x92\xb7 \x8f" "d\x97\xcd    % 2.4f", &DebugWork::g_dw.acre.fTJ_TiggerGravity[0][1], 4, {0}, 1, 0.0f, 1.0f, 0.0001f, NULL, 0, NULL, NULL},
    {"\x83" "e\x83" "B\x83K\x81[\x92\xca\x8f\xed \x92\xb7 \x8f\x89\x91\xac\x93x  % 2.4f", &DebugWork::g_dw.acre.fTJ_TiggerFirstSpeedXZ[0][1], 4, {0}, 1, 0.0f, 1.0f, 0.0001f, NULL, 0, NULL, NULL},
    {"\x83" "e\x83" "B\x83K\x81[\x92\xca\x8f\xed \x92\xb7 \x89\xc1\x91\xac\x93x  % 2.4f", &DebugWork::g_dw.acre.fTJ_TiggerAccelSpeedXZ[0][1], 4, {0}, 1, -1.0f, 1.0f, 0.0001f, NULL, 0, NULL, NULL},
    {"\x83" "e\x83" "B\x83K\x81[\x89\xf1\x93] \x92Z \x8f" "d\x97\xcd    % 2.4f", &DebugWork::g_dw.acre.fTJ_TiggerGravity[1][0], 4, {0}, 1, 0.0f, 1.0f, 0.0001f, NULL, 0, NULL, NULL},
    {"\x83" "e\x83" "B\x83K\x81[\x89\xf1\x93] \x92Z \x8f\x89\x91\xac\x93x  % 2.4f", &DebugWork::g_dw.acre.fTJ_TiggerFirstSpeedXZ[1][0], 4, {0}, 1, 0.0f, 1.0f, 0.0001f, NULL, 0, NULL, NULL},
    {"\x83" "e\x83" "B\x83K\x81[\x89\xf1\x93] \x92Z \x89\xc1\x91\xac\x93x  % 2.4f", &DebugWork::g_dw.acre.fTJ_TiggerAccelSpeedXZ[1][0], 4, {0}, 1, -1.0f, 1.0f, 0.0001f, NULL, 0, NULL, NULL},
    {"\x83" "e\x83" "B\x83K\x81[\x89\xf1\x93] \x92\xb7 \x8f" "d\x97\xcd    % 2.4f", &DebugWork::g_dw.acre.fTJ_TiggerGravity[1][1], 4, {0}, 1, 0.0f, 1.0f, 0.0001f, NULL, 0, NULL, NULL},
    {"\x83" "e\x83" "B\x83K\x81[\x89\xf1\x93] \x92\xb7 \x8f\x89\x91\xac\x93x  % 2.4f", &DebugWork::g_dw.acre.fTJ_TiggerFirstSpeedXZ[1][1], 4, {0}, 1, 0.0f, 1.0f, 0.0001f, NULL, 0, NULL, NULL},
    {"\x83" "e\x83" "B\x83K\x81[\x89\xf1\x93] \x92\xb7 \x89\xc1\x91\xac\x93x  % 2.4f", &DebugWork::g_dw.acre.fTJ_TiggerAccelSpeedXZ[1][1], 4, {0}, 1, -1.0f, 1.0f, 0.0001f, NULL, 0, NULL, NULL},
    {"\x83" "e\x83" "B\x83K\x81[\x89\xf1\x93] \x89\xba\x8d~Motion % 2.4f", &DebugWork::g_dw.acre.fTJ_TiggerDown, 4, {0}, 1, 0.0f, 1.0f, 0.01f, NULL, 0, NULL, NULL},
    {NULL, NULL, 0, {0}, 0, 0.0f, 0.0f, 0.0f, NULL, 0, NULL, NULL},
};

DEBUG_WINDOW_LINE debugWinMini100AcreVPLine[6] = {
    {"REST \x8c\xb8\x82\xe7\x82\xc8\x82\xa2   %s", &DebugWork::g_dw.acre.unVP_Flag, 4, {0}, 0, 0.0f, 1.0f, 1.0f, szOffOn, 1, NULL, NULL},
    {"\x82P\x81|\x82` \x82\xe2\x82\xb3\x82\xa2   %2d \x8c\xc2", &DebugWork::g_dw.acre.nVP_Seq1aPopCount, 4, {0}, 0, 1.0f, 99.0f, 1.0f, NULL, 0, NULL, NULL},
    {"\x82Q     \x82\xe2\x82\xb3\x82\xa2   %2d \x8c\xc2", &DebugWork::g_dw.acre.nVP_Seq2PopCount, 4, {0}, 0, 1.0f, 99.0f, 1.0f, NULL, 0, NULL, NULL},
    {"\x82P\x81|\x82" "a \x82\xe2\x82\xb3\x82\xa2   %2d \x8c\xc2", &DebugWork::g_dw.acre.nVP_Seq1bPopCount, 4, {0}, 0, 1.0f, 99.0f, 1.0f, NULL, 0, NULL, NULL},
    {"\x91\xe5\x82\xa9\x82\xda\x82\xbf\x82\xe1\x8b\x97\x97\xa3  %.1f", &DebugWork::g_dw.acre.fVP_BigPumpkinRngMax, 4, {0}, 1, 24.0f, 50.0f, 0.1f, NULL, 0, NULL, NULL},
    {NULL, NULL, 0, {0}, 0, 0.0f, 0.0f, 0.0f, NULL, 0, NULL, NULL},
};

DEBUG_WINDOW_LINE debugWinMini100AcreBBLine[11] = {
    {"\x83v\x81[ \x96\xb3\x93G       %s", &DebugWork::g_dw.acre.unBB_Flag, 4, {0}, 0, 0.0f, 1.0f, 1.0f, szOffOn, 1, NULL, NULL},
    {"\x82\xcd\x82\xbf \x82\xde\x82\xb0\x82\xf1     %s", &DebugWork::g_dw.acre.unBB_Flag, 4, {0}, 0, 0.0f, 1.0f, 1.0f, szOffOn, 2, NULL, NULL},
    {"\x83v\x81[ MAX HP     %.1f", &DebugWork::g_dw.acre.fBB_PooMaxHP, 4, {0}, 1, 1.0f, 500.0f, 1.0f, NULL, 0, NULL, NULL},
    {"\x82\xcd\x82\xbf \x82P\x89\xf1\x96\xda\x90\x94   %2d", &DebugWork::g_dw.acre.nBB_BeeNum1st, 4, {0}, 0, 10.0f, 99.0f, 1.0f, NULL, 0, NULL, NULL},
    {"\x82\xcd\x82\xbf \x82Q\x89\xf1\x96\xda\x90\x94   %2d", &DebugWork::g_dw.acre.nBB_BeeNum2nd, 4, {0}, 0, 10.0f, 99.0f, 1.0f, NULL, 0, NULL, NULL},
    {"\x82\xcd\x82\xbf \x8dU\x8c\x82\x97\xcd     %.1f", &DebugWork::g_dw.acre.fBB_BeeAtkPoint, 4, {0}, 1, 1.0f, 100.0f, 1.0f, NULL, 0, NULL, NULL},
    {"\x83~\x83" "c \x89\xf1\x95\x9c L1    %.1f", &DebugWork::g_dw.acre.fBB_CureL1, 4, {0}, 1, 0.1f, 20.0f, 0.1f, NULL, 0, NULL, NULL},
    {"          L2    %.1f", &DebugWork::g_dw.acre.fBB_CureL2, 4, {0}, 1, 0.1f, 20.0f, 0.1f, NULL, 0, NULL, NULL},
    {"          L3    %.1f", &DebugWork::g_dw.acre.fBB_CureL3, 4, {0}, 1, 0.1f, 20.0f, 0.1f, NULL, 0, NULL, NULL},
    {"\x89\xf1\x95\x9c\x82\xa9\x82\xf1\x82\xa9\x82\xad    %2d \x83t\x83\x8c\x81[\x83\x80", &DebugWork::g_dw.acre.nBB_CureInterval, 4, {0}, 0, 1.0f, 99.0f, 1.0f, NULL, 0, NULL, NULL},
    {NULL, NULL, 0, {0}, 0, 0.0f, 0.0f, 0.0f, NULL, 0, NULL, NULL},
};

DEBUG_WINDOW_TBL debugWinMini100Acre = {"100\x83G\x81[\x83J\x81[\x83~\x83j\x83Q\x81[\x83\x80", -208, -160, 15, 22, 14, 0, 0, debugWinMini100AcreLine, NULL, NULL};

DEBUG_WINDOW_TBL debugWinMini100AcreWD = {"\x83\x8f\x83" "C\x83\x8b\x83h\x83_\x83" "C\x83u", -192, -144, 15, 22, 14, 0, 0, debugWinMini100AcreWDLine, NULL, NULL};

DEBUG_WINDOW_TBL debugWinMini100AcreBL = {"\x83o\x83\x8b\x81[\x83\x93\x83O\x83\x89\x83" "C\x83_\x81[", -192, -144, 15, 22, 14, 0, 0, debugWinMini100AcreBLLine, NULL, NULL};

DEBUG_WINDOW_TBL debugWinMini100AcreTJ = {"\x83" "e\x83" "B\x83K\x81[\x82\xc6\x83W\x83\x83\x83\x93\x83v", -192, -144, 16, 22, 14, 0, 0, debugWinMini100AcreTJLine, NULL, NULL};

DEBUG_WINDOW_TBL debugWinMini100AcreVP = {"\x82\xe2\x82\xb3\x82\xa2\x83p\x83j\x83" "b\x83N", -192, -144, 15, 22, 14, 0, 0, debugWinMini100AcreVPLine, NULL, NULL};

DEBUG_WINDOW_TBL debugWinMini100AcreBB = {"\x82\xcd\x82\xbf\x83o\x83g\x83\x8b", -192, -144, 15, 22, 14, 0, 0, debugWinMini100AcreBBLine, NULL, NULL};

static BOOL debugWinDispBrightLineCall(CDebugWindow* pObj, INT iLineY, INT inParam) {
    if (inParam & 1) {
        DebugWork::g_dw.sys.m_fMapBright = CSceneEnv::getColor(3).x;
    }
    if (inParam & 0x10) {
        FVECTOR vColor;
        FLOAT _x = DebugWork::g_dw.sys.m_fMapBright;
        vColor.x = _x;
        vColor.y = _x;
        vColor.z = _x;
        vColor.w = 1.0f;
        CSceneEnv::setColor((ObjType::eObjBit)0xF88, 0.0f, vColor, NULL, 3);
    }
    return TRUE;
}

static DEBUG_WINDOW_LINE debugWinDispTestLine[11] = {
    {"\x83}\x83" "b\x83v\x82\xcc\x97\xa0\x95`\x89\xe6      %s", &DebugWork::g_dw.sys.m_iMapBack, 1, {0}, 0, 0.0f, 2.0f, 1.0f, szDefOnOff, 0, NULL, NULL},
    {"\x83}\x83" "b\x83v\x82\xcc\x96\xbe\x82\xe9\x82\xb3      %4.2f", &DebugWork::g_dw.sys.m_fMapBright, 4, {0}, 1, 0.0f, 1.0f, 0.01f, NULL, 0, debugWinDispBrightLineCall, NULL},
    {"\x83}\x83" "b\x83v\x95\\\x8e\xa6(-1:OFF 0:ON) %d", &DebugWork::g_dw.sys.m_iMapDisp, 4, {0}, 0, -10.0f, 9999.0f, 1.0f, NULL, 0, NULL, NULL},
    {"\x83X\x83J\x83" "C\x95\\\x8e\xa6(-1:OFF 0:ON) %d", &DebugWork::g_dw.sys.m_iSkyDisp, 4, {0}, 0, -10.0f, 9999.0f, 1.0f, NULL, 0, NULL, NULL},
    {"\x83G\x83t\x83" "F\x83N\x83g\x95\\\x8e\xa6       %s", &DebugWork::g_dw.sys.m_iEffect, 4, {0}, 0, 0.0f, 1.0f, 1.0f, szOnOff, 0, NULL, NULL},
    {"\x83G\x83t\x83" "F\x83N\x83gBBOX\x95\\\x8e\xa6   %s", &DebugWork::g_dw.sys.m_iEffectBBOX, 1, {0}, 0, 0.0f, 1.0f, 1.0f, szOffOn, 0, NULL, NULL},
    {"\x83G\x83t\x83" "F\x83N\x83g\x83\x8c\x83" "C\x83\x84\x92\xe2\x8e~ %s", &DebugWork::g_dw.sys.m_iStopEffect, 1, {0}, 0, 0.0f, 1.0f, 1.0f, szOffOn, 0, NULL, NULL},
    {"\x94z\x92u\x83G\x83t\x83" "F\x83N\x83g\x90\xb6\x90\xac\x96\xb3\x82\xb5 %s", &DebugWork::g_dw.sys.m_iEffectNoEntry, 1, {0}, 0, 0.0f, 1.0f, 1.0f, szOffOn, 0, NULL, NULL},
    {"\x83t\x83\x8c\x81[\x83\x80\x95\\\x8e\xa6         %s", &DebugWork::g_dw.sys.m_iFukidasiDisp, 2, {0}, 0, 0.0f, 1.0f, 1.0f, szOffOn, 0, NULL, NULL},
    {"\x83_\x83\x81\x81[\x83W\x90\x94\x8e\x9a\x95\\\x8e\xa6     %s", &DebugWork::g_dw.sys.m_iDagameNumber, 4, {0}, 0, 0.0f, 1.0f, 1.0f, szOffOn, 0, NULL, NULL},
    {NULL, NULL, 0, {0}, 0, 0.0f, 0.0f, 0.0f, NULL, 0, NULL, NULL},
};

DEBUG_WINDOW_TBL debugWinDispTest = {"\x8a" "e\x8e\xed\x95\\\x8e\xa6ON/OFF", -208, -168, 14, 24, 12, 0, 0, debugWinDispTestLine, NULL, NULL};

static unsigned short s_MenuSoraCardKindTbl[131] = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 19, 20, 21, 22, 23, 24, 26, 27, 28, 29, 30, 31, 32, 35, 36, 37, 38, 39, 40, 41, 42, 43, 44, 45, 46, 47, 48, 68, 69, 70, 71, 72, 73, 74, 75, 76, 77, 78, 79, 80, 81, 82, 83, 84, 85, 86, 87, 88, 90, 91, 92, 93, 94, 95, 96, 97, 98, 99, 100, 101, 102, 103, 104, 105, 106, 107, 108, 109, 110, 111, 112, 113, 114, 115, 116, 117, 118, 119, 120, 121, 122, 123, 124, 125, 126, 161, 162, 163, 164, 165, 166, 167, 168, 169, 170, 171, 172, 173, 174, 175, 176, 177, 178, 179, 180, 181, 182, 183, 184, 185, 186, 187, 188, 189};

static char* s_MenuSoraCardStringTbl[131] = {"\x83L\x83\x93\x83O\x83_\x83\x80\x83`\x83" "F\x81[\x83\x93", "\x83" "f\x83U\x83" "C\x83" "A\x81[\x83\x89\x83\x93\x83v", "\x83g\x83\x8c\x83W\x83\x83\x81[\x83I\x83u\x83V\x81[", "\x83p\x83\x93\x83v\x83L\x83\x93\x83w\x83" "b\x83h", "\x83t\x83" "F\x83" "A\x83\x8a\x81[\x83n\x81[\x83v", "\x83" "E\x83" "B\x83" "b\x83V\x83\x85\x83X\x83^\x81[", "\x83G\x83O\x83U\x83~\x83l\x83" "C\x83V\x83\x87\x83\x93", "\x83\x81\x83^\x83\x8b\x83`\x83\x87\x83R\x83{", "\x83p\x83\x8f\x81[\x83I\x83u\x83q\x81[\x83\x8d\x81[", "\x83\x89\x83" "C\x83I\x83\x93\x83n\x81[\x83g", "\x83\x89\x83X\x83g\x83\x8a\x83]\x81[\x83g", "\x83\x89\x83\x94\x83" "B\x83" "A\x83\x93\x83\x8d\x81[\x83Y", "\x83\x84\x83N\x83\\\x83N\x82\xcc\x82\xa8\x83}\x83\x82\x82\xe8", "\x83X\x82\xac\x83T\x82\xe8\x82\xb5\x83I\x83\x82\x82\xa2\x83" "f", "\x83" "A\x83\x8b\x83" "e\x83}\x83" "E\x83" "F\x83|\x83\x93", "\x83_\x83" "C\x83\x84\x83\x82\x83\x93\x83h\x83_\x83X\x83g", "\x83J\x83^\x83\x88\x83N\x82\xcc\x83" "e\x83\x93\x83V", "\x83X\x83^\x81[\x83V\x81[\x83J\x81[", "\x83\x82\x83m\x83N\x83\x8d\x81[\x83\x80", "\x83t\x83H\x83\x8d\x81[\x83U\x83" "E\x83" "B\x83\x93\x83h", "\x83q\x83h\x83" "D\x83\x93\x83h\x83\x89\x83S\x83\x93", "\x83t\x83H\x83g\x83\x93\x83" "f\x83o\x83" "b\x83K\x81[", "\x83{\x83\x93\x83h\x83I\x83u\x83t\x83\x8c\x83" "C\x83\x80", "\x83t\x83@\x83" "C\x83" "A", "\x83u\x83\x8a\x83U\x83h", "\x83T\x83\x93\x83_\x81[", "\x83P\x83" "A\x83\x8b", "\x83O\x83\x89\x83r\x83" "f", "\x83X\x83g\x83" "b\x83v", "\x83G\x83" "A\x83\x8d", "\x83V\x83\x93\x83o", "\x83W\x81[\x83j\x81[", "\x83o\x83\x93\x83r", "\x83_\x83\x93\x83{", "\x83" "e\x83" "B\x83\x93\x83N", "\x83\x80\x81[\x83V\x83\x85\x81[", "\x83N\x83\x89\x83" "E\x83h", "\x83|\x81[\x83V\x83\x87\x83\x93", "\x83n\x83" "C\x83|\x81[\x83V\x83\x87\x83\x93", "\x83\x81\x83K\x83|\x81[\x83V\x83\x87\x83\x93", "\x83G\x81[\x83" "e\x83\x8b", "\x83\x81\x83K\x83G\x81[\x83" "e\x83\x8b", "\x83G\x83\x8a\x83N\x83T\x81[", "\x83\x89\x83X\x83g\x83G\x83\x8a\x83N\x83T\x81[", "\x83V\x83\x83\x83h\x83" "E", "\x83\\\x83\x8b\x83W\x83\x83\x81[", "\x83\x89\x81[\x83W\x83{\x83" "f\x83" "B", "\x83\x8c\x83" "b\x83h\x83m\x83N\x83^\x81[\x83\x93", "\x83u\x83\x8b\x81[\x83\x89\x83v\x83\\\x83" "f\x83" "B", "\x83" "C\x83G\x83\x8d\x81[\x83I\x83y\x83\x89", "\x83O\x83\x8a\x81[\x83\x93\x83\x8c\x83N\x83" "C\x83G\x83\x80", "\x83p\x83\x8f\x81[\x83\x8f\x83" "C\x83\x8b\x83h", "\x83o\x83" "E\x83\x93\x83V\x81[\x83\x8f\x83" "C\x83\x8b\x83h", "\x83G\x83" "A\x83\\\x83\x8b\x83W\x83\x83\x81[", "\x83o\x83\x93\x83" "f\x83" "B\x83" "b\x83g", "\x83t\x83@\x83" "b\x83g\x83o\x83\x93\x83" "f\x83" "B\x83" "b\x83g", "\x83o\x83\x8c\x83\x8b\x83X\x83p\x83" "C\x83_\x81[", "\x83T\x81[\x83`\x83S\x81[\x83X\x83g", "\x83V\x81[\x83l\x83I\x83\x93", "\x83X\x83N\x83\x8a\x83\x85\x81[\x83_\x83" "C\x83o\x81[", "\x83" "A\x83N\x83" "A\x83^\x83\x93\x83N", "\x83\x8f\x83" "C\x83g\x83i\x83" "C\x83g", "\x83K\x81[\x83S\x83" "C\x83\x8b", "\x83p\x83" "C\x83\x8c\x81[\x83" "c", "\x83G\x83" "A\x83p\x83" "C\x83\x8c\x81[\x83" "c", "\x83" "f\x83" "B\x83t\x83" "F\x83\x93\x83_\x81[", "\x83\x8f\x83" "C\x83o\x81[\x83\x93", "\x83" "E\x83" "B\x83U\x81[\x83h", "\x83l\x83I\x83V\x83\x83\x83h\x83" "E", "\x83z\x83\x8f\x83" "C\x83g\x83}\x83" "b\x83V\x83\x85\x83\x8b\x81[\x83\x80", "\x83u\x83\x89\x83" "b\x83N\x83t\x83@\x83\x93\x83K\x83X", "\x83N\x83\x8a\x81[\x83v\x83v\x83\x89\x83\x93\x83g", "\x83g\x83\x8b\x83l\x81[\x83h\x83X\x83" "e\x83" "b\x83v", "\x83\x89\x83" "E\x83h\x83l\x83X", "\x83K\x81[\x83h\x83" "A\x81[\x83}\x81[", "\x83p\x83\x89\x83T\x83" "C\x83g\x83P\x83" "C\x83W", "\x83g\x83\x8a\x83" "b\x83N\x83}\x83X\x83^\x81[", "\x83_\x81[\x83N\x83T\x83" "C\x83h", "\x83g\x83\x89\x83\x93\x83v\x83w\x83" "C", "\x83n\x83" "f\x83X", "\x83W\x83\x83\x83t\x83@\x81[", "\x83u\x83M\x81[", "\x83" "A\x81[\x83X\x83\x89", "\x83t\x83" "b\x83N", "\x83}\x83\x8c\x83t\x83" "B\x83Z\x83\x93\x83g\x83h\x83\x89\x83S\x83\x93", "\x83\x8a\x83N", "\x83" "A\x83N\x83Z\x83\x8b", "\x83\x89\x83N\x83V\x81[\x83k", "\x83\x94\x83" "B\x83N\x83Z\x83\x93", "\x83}\x81[\x83\x8b\x81[\x83V\x83\x83", "\x83\x8c\x83N\x83Z\x83" "E\x83X", "\x83" "A\x83\x93\x83Z\x83\x80", "\x83[\x83N\x83V\x83I\x83\x93", "\x83[\x83\x80\x83i\x83X", "\x83V\x83O\x83o\x81[\x83\x8b", "\x83U\x83\x8b\x83" "f\x83" "B\x83\x93", "\x83T\x83" "C\x83N\x83X", "\x83" "f\x83~\x83" "b\x83N\x83X", "\x83\x8b\x83N\x83\\\x81[\x83h", "\x83\x8d\x83N\x83T\x83X", "\x83S\x81[\x83\x8b\x83h", "\x83v\x83\x89\x83`\x83i", "\x83Z\x83" "C\x82\xb6\x82\xe1\x82\xad\x82\xc8\x82\xe9\x83\x84\x83~", "\x82\xd0\x82\xb5\x82\xdf\x82\xad\x83\x84\x83~", "\x82\xbf\x82\xa9\x82\xe7\x82\xc8\x82\xab\x83\x84\x83~", "\x82\xa8\x82\xa8\x82\xa2\x82\xc8\x82\xe9\x83\x84\x83~", "\x83l\x83\x80\x82\xe8\x82\xcc\x83\x84\x83~", "\x82\xb9\x82\xdc\x82\xe8\x82\xad\x82\xe9\x83\x84\x83~", "\x83v\x83\x8c\x83~\x83" "A\x83\x8b\x81[\x83\x80", "\x83z\x83\x8f\x83" "C\x83g\x83\x8b\x81[\x83\x80", "\x83u\x83\x89\x83" "b\x83N\x83\x8b\x81[\x83\x80", "\x83\\\x83R\x83V\x82\xea\x82\xca\x83\x84\x83~", "\x83\x8b\x81[\x83\x8c\x83" "b\x83g\x83\x8b\x81[\x83\x80", "\x83u\x83\x8a\x83\x87\x83N\x82\xd6\x82\xcc\x82\xdf\x82\xb4\x82\xdf", "\x83}\x83\x8a\x83\x87\x83N\x82\xd6\x82\xcc\x82\xdf\x82\xb4\x82\xdf", "\x83\x8c\x83\x93\x83L\x83\x93\x82\xd6\x82\xcc\x82\xdf\x82\xb4\x82\xdf", "\x82\xdf\x82\xae\x82\xe8\x82\xa0\x82\xa2\x82\xcc\x83`", "\x82\xc4\x82\xa2\x82\xbd\x82\xa2\x82\xcc\x83}", "\x83_\x83\x81\x81[\x83W\x83" "A\x83" "b\x83v\x83\x8b\x81[\x83\x80", "\x83X\x83^\x83\x93\x83v\x83\x89\x83X\x83\x8b\x81[\x83\x80", "\x82\xe2\x82\xb7\x82\xe7\x82\xac\x82\xcc\x83^\x83J\x83\x89", "\x83}\x83\x82\x82\xe7\x82\xea\x82\xb5\x83^\x83J\x83\x89", "\x82\xa2\x82\xc2\x82\xed\x82\xe8\x82\xcc\x83^\x83J\x83\x89", "\x83q\x83g\x83g\x83L\x82\xcc\x83L\x83\x85\x83" "E\x83\\\x83N", "\x83}\x82\xb6\x82\xe8\x83" "A\x82\xa4\x83Z\x83J\x83" "C", "\x83\x82\x81[\x83O\x83\x8a\x83\x8b\x81[\x83\x80", "\x83\x89\x83\x93\x83_\x83\x80\x83W\x83\x87\x81[\x83J\x81[", "\x82\xcd\x82\xb6\x82\xdc\x82\xe8\x82\xcc\x83L\x81[\x83J\x81[\x83h", "\x82\xdd\x82\xbf\x82\xd1\x82\xab\x82\xcc\x83L\x81[\x83J\x81[\x83h", "\x83V\x83\x93\x83W\x83" "c\x82\xd6\x82\xcc\x83L\x81[\x83J\x81[\x83h", "\x83~\x83`\x82\xc8\x82\xe9\x83^\x83J\x83\x89\x82\xcc\x83L\x81[\x83J\x81[\x83h"};

static unsigned short s_MenuRikuCardKindTbl[20] = {161, 162, 163, 164, 165, 166, 169, 170, 171, 172, 175, 176, 177, 178, 182, 183, 185, 186, 187, 188};

static char* s_MenuRikuCardStringTbl[20] = {"\x83Z\x83" "C\x82\xb6\x82\xe1\x82\xad\x82\xc8\x82\xe9\x83\x84\x83~", "\x82\xd0\x82\xb5\x82\xdf\x82\xad\x83\x84\x83~", "\x82\xbf\x82\xa9\x82\xe7\x82\xc8\x82\xab\x83\x84\x83~", "\x82\xa8\x82\xa8\x82\xa2\x82\xc8\x82\xe9\x83\x84\x83~", "\x83l\x83\x80\x82\xe8\x82\xcc\x83\x84\x83~", "\x82\xb9\x82\xdc\x82\xe8\x82\xad\x82\xe9\x83\x84\x83~", "\x83u\x83\x89\x83" "b\x83N\x83\x8b\x81[\x83\x80", "\x83\\\x83R\x83V\x82\xea\x82\xca\x83\x84\x83~", "\x83\x8b\x81[\x83\x8c\x83" "b\x83g\x83\x8b\x81[\x83\x80", "\x83u\x83\x8a\x83\x87\x83N\x82\xd6\x82\xcc\x82\xdf\x82\xb4\x82\xdf", "\x82\xdf\x82\xae\x82\xe8\x82\xa0\x82\xa2\x82\xcc\x83`", "\x82\xc4\x82\xa2\x82\xbd\x82\xa2\x82\xcc\x83}", "\x83_\x83\x81\x81[\x83W\x83" "A\x83" "b\x83v\x83\x8b\x81[\x83\x80", "\x83X\x83^\x83\x93\x83v\x83\x89\x83X\x83\x8b\x81[\x83\x80", "\x83" "C\x83g\x83g\x83L\x82\xcc\x83L\x83\x85\x83" "E\x83\\\x83N", "\x83}\x82\xb6\x82\xe8\x83" "A\x82\xa4\x83Z\x83J\x83" "C", "\x83\x89\x83\x93\x83_\x83\x80\x83W\x83\x87\x81[\x83J\x81[", "\x82\xcd\x82\xb6\x82\xdc\x82\xe8\x82\xcc\x83L\x81[\x83J\x81[\x83h", "\x82\xdd\x82\xbf\x82\xd1\x82\xab\x82\xcc\x83L\x81[\x83J\x81[\x83h", "\x83V\x83\x93\x83W\x83" "c\x82\xd6\x82\xcc\x83L\x81[\x83J\x81[\x83h"};

static char* s_MenuAdd[2] = {"", "\x82\xc2\x82\xa2\x82\xa9\x82\xb5\x82\xdc\x82\xb5\x82\xbd\x81I"};


static BOOL debugWCCardFull(CDebugWindow* pObj, INT inY, INT inParam) {
    short int idTbl[8][2] = {{1, 25}, {26, 42}, {42, 49}, {68, 127}, {257, 281}, {282, 298}, {161, 186}, {186, 190}};
    INT i;
    INT j;
    INT k;
    if (!(inParam & 0x20)) {
        return TRUE;
    }
    if (!CTaskMan::getSysTask(1)) {
        return FALSE;
    }
    for (i = idTbl[7][0]; i < idTbl[7][1]; i++) {
        CCard::addCard(i, 0);
    }
    k = 0;
    while (!CCard::isMapCardFull()) {
        for (i = idTbl[6][0]; i < idTbl[6][1]; i++) {
            CCard::addCard(i, k);
            k++;
            if (k >= 10) {
                k = 0;
            }
        }
    }
    if (CGame::isSoraGame()) {
        while (CCard::m_pCardInfo->_nBattleTotal < 999) {
            for (j = 0; j < 6; j++) {
                for (i = idTbl[j][0]; i < idTbl[j][1]; i++) {
                    for (k = 0; k < 10; k++) {
                        CCard::addCard(i, k);
                        if (j == 3) {
                            break;
                        }
                    }
                }
            }
        }
    }
    return TRUE;
}

static BOOL debugWCCardAll(CDebugWindow* pObj, INT inY, INT inParam) {
    return TRUE;
}

BOOL debugWCMPFull(CDebugWindow* pObj, INT inY, INT inParam);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Form/DebugWindowTBL", debugWCMPFull__3DWTP12CDebugWindowii);
#if 0
/* differs only in a register: the original loads %hi(CPlayerManager::m_pInstance) into v1, ours into v0 */
static BOOL debugWCMPFull(CDebugWindow* pObj, INT inY, INT inParam) {
    if (inParam & 0x20) {
        CStatus::setMP(99999);
        PLM::CPlayerManager* plm = PLM::CPlayerManager::getPLM();
        if (plm && plm->getPlayer()) {
            plm->getPlayer()->m_iMP = 99999;
        }
    }
    return TRUE;
}
#endif

static BOOL debugWCEXPFull(CDebugWindow* pObj, INT inY, INT inParam) {
    if (inParam & 0x20) {
        if (CStatus::getLV() < DebugWork::g_dw.menu.nExpLevel) {
            INT exp = CStatus::getLvExp(DebugWork::g_dw.menu.nExpLevel - 1) - 1;
            CStatus::setEXP(exp);
            if (PLM::CPlayerManager::getPLM() && PLM::CPlayerManager::getPLM()->getPlayer()) {
                PLM::CPlayerManager::getPLM()->getPlayer()->m_iEXP = exp;
            }
        }
    }
    return TRUE;
}

static BOOL debugWCTimeFull(CDebugWindow* pObj, INT inY, INT inParam) {
    if (inParam & 0x20) {
        CGameWork::setInt(0, 0xC0, 3600000);
    }
    return TRUE;
}

static BOOL debugWCTechnicClear(CDebugWindow* pObj, INT inY, INT inParam) {
    if (inParam & 0x20) {
        CTechnic::clearAll();
    }
    return TRUE;
}

static BOOL debugWCTechnicFull(CDebugWindow* pObj, INT inY, INT inParam) {
    if (inParam & 0x20) {
        CTechnic::setAll();
    }
    return TRUE;
}

static BOOL debugWCMemoFull(CDebugWindow* pObj, INT inY, INT inParam) {
    if (inParam & 0x20) {
        CJiminyMemoFlag::debugEvent(1, 0, 11, 0, 0);
        CJiminyMemoFlag::debugEvent(1, 1, 11, 0, 0);
    }
    return TRUE;
}

static BOOL debugWCAddCardSora(CDebugWindow* pObj, INT inY, INT inParam) {
    DebugWork::g_dw.menu.unFlag &= ~1;
    DebugWork::g_dw.menu.unFlag &= ~2;
    if ((inParam & 0x20) && CGame::isSoraGame()) {
        USHORT usKind = s_MenuSoraCardKindTbl[DebugWork::g_dw.menu.nSoraCardIdx];
        INT nRet = 0;
        CARD card;
        if (CCard::canPremium(usKind) && DebugWork::g_dw.menu.nSoraCardPremium) {
            usKind += 0x100;
        }
        _setCard(card, usKind, DebugWork::g_dw.menu.nSoraCardRank);
        if (card.m_nCategory20 == 7 || card.m_nCategory20 == 10) {
            _setCard(card, usKind, 0);
        }
        for (INT i = 0; i < DebugWork::g_dw.menu.nSoraCardNum; i++) {
            nRet += CCard::addCard(card);
        }
        if (nRet) {
            DebugWork::g_dw.menu.unFlag |= 1;
        }
    }
    return TRUE;
}

static BOOL debugWCAddCardRiku(CDebugWindow* pObj, INT inY, INT inParam) {
    DebugWork::g_dw.menu.unFlag &= ~1;
    DebugWork::g_dw.menu.unFlag &= ~2;
    if ((inParam & 0x20) && CGame::isRikuGame()) {
        USHORT usKind = s_MenuRikuCardKindTbl[DebugWork::g_dw.menu.nRikuCardIdx];
        INT nRet = 0;
        CARD card;
        _setCard(card, usKind, DebugWork::g_dw.menu.nRikuCardRank);
        if (card.m_nCategory20 == 7 || card.m_nCategory20 == 10) {
            _setCard(card, usKind, 0);
        }
        for (INT i = 0; i < DebugWork::g_dw.menu.nRikuCardNum; i++) {
            nRet += CCard::addCard(card);
        }
        if (nRet) {
            DebugWork::g_dw.menu.unFlag |= 2;
        }
    }
    return TRUE;
}

static INT debugWinMenuConfigAllCall(CDebugWindow* pObj, INT inParam) {
    return TRUE;
}

static DEBUG_WINDOW_LINE debugWinMenuConfigLine[10] = {
    {"\x83J\x81[\x83h FULL", &CDebugWindow::none, 1, {0}, 0, 0.0f, 0.0f, 0.0f, NULL, 0, debugWCCardFull, NULL},
    {"\x83J\x81[\x83h ADD (\x83\\\x83\x89) ...", &CDebugWindow::none, 1, {0}, 0, 0.0f, 0.0f, 0.0f, NULL, 0, NULL, &debugWinMenuAddCardSora},
    {"\x83J\x81[\x83h ADD (\x83\x8a\x83N) ...", &CDebugWindow::none, 1, {0}, 0, 0.0f, 0.0f, 0.0f, NULL, 0, NULL, &debugWinMenuAddCardRiku},
    {"MP               FULL", &CDebugWindow::none, 1, {0}, 0, 0.0f, 0.0f, 0.0f, NULL, 0, debugWCMPFull, NULL},
    {"LVLEXP           : %2d", &DebugWork::g_dw.menu.nExpLevel, 4, {0}, 0, 2.0f, 99.0f, 1.0f, NULL, 0, debugWCEXPFull, NULL},
    {"TIME             FULL", &CDebugWindow::none, 1, {0}, 0, 0.0f, 0.0f, 0.0f, NULL, 0, debugWCTimeFull, NULL},
    {"\x83W\x83~\x83j\x81[\x83\x81\x83\x82 FULL", &CDebugWindow::none, 1, {0}, 0, 0.0f, 0.0f, 0.0f, NULL, 0, debugWCMemoFull, NULL},
    {"\x83X\x83g\x83" "b\x83N\x83\x8f\x83U FULL", &CDebugWindow::none, 1, {0}, 0, 0.0f, 0.0f, 0.0f, NULL, 0, debugWCTechnicFull, NULL},
    {"\x83X\x83g\x83" "b\x83N\x83\x8f\x83U \x83N\x83\x8a\x83" "A", &CDebugWindow::none, 1, {0}, 0, 0.0f, 0.0f, 0.0f, NULL, 0, debugWCTechnicClear, NULL},
    {NULL, NULL, 0, {0}, 0, 0.0f, 0.0f, 0.0f, NULL, 0, NULL, NULL},
};

DEBUG_WINDOW_LINE debugWinMenuAddCardLineSora[9] = {
    {"\x83J\x81[\x83hKIND :  %s", &DebugWork::g_dw.menu.nSoraCardIdx, 4, {0}, 0, 0.0f, 130.0f, 1.0f, s_MenuSoraCardStringTbl, 0, debugWCAddCardSora, NULL},
    {"\x83X\x83" "E\x83`     :  %2d", &DebugWork::g_dw.menu.nSoraCardRank, 4, {0}, 0, 0.0f, 9.0f, 1.0f, NULL, 0, debugWCAddCardSora, NULL},
    {"\x83v\x83\x8c\x83~\x83" "A\x83\x80 :  %s", &DebugWork::g_dw.menu.nSoraCardPremium, 4, {0}, 0, 0.0f, 1.0f, 1.0f, szOffOn, 0, debugWCAddCardSora, NULL},
    {"\x83" "c\x83" "C\x83J\x83X\x83" "E :  %2d", &DebugWork::g_dw.menu.nSoraCardNum, 4, {0}, 0, 1.0f, 99.0f, 1.0f, NULL, 0, debugWCAddCardSora, NULL},
    {"\xf9R\x81\x9b\x83{\x83^\x83\x93 \x83" "c\x83" "C\x83J %s", &DebugWork::g_dw.menu.unFlag, 4, {0}, 0, 0.0f, 0.0f, 0.0f, s_MenuAdd, 1, debugWCAddCardSora, NULL},
    {"------------------------------------------------", &CDebugWindow::none, 1, {0}, 2, 0.0f, 0.0f, 0.0f, NULL, 0, NULL, NULL},
    {"\xf9Q\x83v\x83\x8c\x83~\x83" "A\x83\x80\x82\xa9\x82\xcd\x83" "A\x83^\x83" "b\x83N\x83J\x81[\x83h\x82\xc6\x83}\x83W\x83" "b\x83N\x83J\x81[\x83h\x82\xcc\x82\xdd", &CDebugWindow::none, 1, {0}, 2, 0.0f, 0.0f, 0.0f, NULL, 0, NULL, NULL},
    {"\xf9Q\x83Q\x81[\x83\x80\x82\xc5\x83j\x83\x85\x83" "E\x83V\x83\x85\x82\xa9\x82\xcc\x82\xa4\x82\xc8\x82\xe0\x82\xcc\x82\xcc\x82\xdd", &CDebugWindow::none, 1, {0}, 2, 0.0f, 0.0f, 0.0f, NULL, 0, NULL, NULL},
    {NULL, NULL, 0, {0}, 0, 0.0f, 0.0f, 0.0f, NULL, 0, NULL, NULL},
};

DEBUG_WINDOW_LINE debugWinMenuAddCardLineRiku[7] = {
    {"\x83J\x81[\x83hKIND  :  %s", &DebugWork::g_dw.menu.nRikuCardIdx, 4, {0}, 0, 0.0f, 19.0f, 1.0f, s_MenuRikuCardStringTbl, 0, debugWCAddCardRiku, NULL},
    {"\x83X\x83" "E\x83`      :  %2d", &DebugWork::g_dw.menu.nRikuCardRank, 4, {0}, 0, 0.0f, 9.0f, 1.0f, NULL, 0, debugWCAddCardRiku, NULL},
    {"\x83" "c\x83" "C\x83J\x83X\x83" "E  :  %2d", &DebugWork::g_dw.menu.nRikuCardNum, 4, {0}, 0, 1.0f, 9.0f, 1.0f, NULL, 0, debugWCAddCardRiku, NULL},
    {"\xf9R\x81\x9b\x83{\x83^\x83\x93 \x83" "c\x83" "C\x83J %s", &DebugWork::g_dw.menu.unFlag, 4, {0}, 0, 0.0f, 0.0f, 0.0f, s_MenuAdd, 2, debugWCAddCardRiku, NULL},
    {"------------------------------------------------", &CDebugWindow::none, 1, {0}, 2, 0.0f, 0.0f, 0.0f, NULL, 0, NULL, NULL},
    {"\xf9Q\x83Q\x81[\x83\x80\x82\xc5\x83j\x83\x85\x83" "E\x83V\x83\x85\x82\xa9\x82\xcc\x82\xa4\x82\xc8\x82\xe0\x82\xcc\x82\xcc\x82\xdd", &CDebugWindow::none, 1, {0}, 2, 0.0f, 0.0f, 0.0f, NULL, 0, NULL, NULL},
    {NULL, NULL, 0, {0}, 0, 0.0f, 0.0f, 0.0f, NULL, 0, NULL, NULL},
};

DEBUG_WINDOW_TBL debugWinMenuConfig = {"\x83\x81\x83j\x83\x85\x81[CONFIG", -208, -168, 12, 20, 12, 0, 0, debugWinMenuConfigLine, NULL, debugWinMenuConfigAllCall};

DEBUG_WINDOW_TBL debugWinMenuAddCardSora = {"\x83J\x81[\x83h\x82\xc2\x82\xa2\x82\xa9(\x83\\\x83\x89)", -192, -144, 24, 20, 14, 0, 0, debugWinMenuAddCardLineSora, NULL, NULL};

DEBUG_WINDOW_TBL debugWinMenuAddCardRiku = {"\x83J\x81[\x83h\x82\xc2\x82\xa2\x82\xa9(\x83\x8a\x83N)", -192, -144, 24, 20, 14, 0, 0, debugWinMenuAddCardLineRiku, NULL, NULL};

static BOOL DebugMemoryOn(CDebugWindow* pObj, INT inY, INT inParam) {
    if (inParam & 0x10) {
        DebugWork::g_dw.sys.m_MDispLine = 0;
    }
    return TRUE;
}

static BOOL DebugHeapOn(CDebugWindow* pObj, INT inY, INT inParam) {
    if (inParam & 0x20) {
        DebugWork::g_dw.sys.m_HeapDisp = 1;
    } else {
        DebugWork::g_dw.sys.m_HeapDisp = 0;
    }
    return TRUE;
}

static BOOL DebugReportHeapDetail(CDebugWindow* pObj, INT inY, INT inParam) {
    if (inParam & 0x20) {
        DebugWork::g_dw.sys.m_HeapDispDetail = 1;
    } else {
        DebugWork::g_dw.sys.m_HeapDispDetail = 0;
    }
    return TRUE;
}

static BOOL DebugCameraAngleReset(CDebugWindow* pObj, INT inY, INT inParam) {
    if (inParam & 0x20) {
        DebugWork::g_dw.nakamich.fCamVAngle = ProjectionToViewAngle(CWorldEnv::getDefScreenZ());
    }
    return TRUE;
}

static DEBUG_WINDOW_LINE debugWinTestLine[8] = {
    {"\x83" "e\x83X\x83gNUM  %08x", &DebugWork::g_dw.nakamich.iTest, 2, {0}, 2, 0.0f, 0.0f, 0.0f, NULL, 0, NULL, NULL},
    {"------------------------", &CDebugWindow::none, 1, {0}, 2, 0.0f, 0.0f, 0.0f, NULL, 0, NULL, NULL},
    {"\x83" "e\x83X\x83gNUM    %d", &DebugWork::g_dw.nakamich.iTest, 2, {0}, 0, 0.0f, 16.0f, 1.0f, NULL, 0, NULL, NULL},
    {"\x83" "e\x83X\x83g\x83r\x83" "b\x83g %d", &DebugWork::g_dw.nakamich.iTest, 2, {0}, 0, 0.0f, 1.0f, 1.0f, NULL, 1, NULL, NULL},
    {"\x83" "e\x83X\x83g\x83r\x83" "b\x83g %d", &DebugWork::g_dw.nakamich.iTest, 2, {0}, 0, 0.0f, 2.0f, 1.0f, NULL, 6, NULL, NULL},
    {"\x83" "e\x83X\x83g\x83r\x83" "b\x83g %d", &DebugWork::g_dw.nakamich.iTest, 2, {0}, 0, 0.0f, 3.0f, 1.0f, NULL, 0x30, NULL, NULL},
    {"\x83" "e\x83X\x83g\x83r\x83" "b\x83g %d", &DebugWork::g_dw.nakamich.iTest, 2, {0}, 0, 0.0f, 16.0f, 1.0f, NULL, 0x7F00, NULL, NULL},
    {NULL, NULL, 0, {0}, 0, 0.0f, 0.0f, 0.0f, NULL, 0, NULL, NULL},
};

static DEBUG_WINDOW_TBL debugWinTest = {"\x83" "e\x83X\x83g\x83" "E\x83" "C\x83\x93\x83h\x83" "E", -208, -160, 10, 8, 12, 0, 0, debugWinTestLine, NULL, NULL};

static BOOL DebugCameraOn(CDebugWindow* pObj, INT inY, INT inParam) {
    if (inParam & 0x180) {
        if (!DebugWork::g_dw.nakamich.m_pDCam) {
            CTaskMan::getSysTask(3);
            CTask* pM = CTaskMan::getSysTask(2);
            CTask* pR = CTaskMan::getSysTask(0);
            if (pR) {
                FVECTOR vPos;
                _unitVectorVu0(vPos);
                vPos.y += 1.0f;
                if (!pM) {
                    pM = pR;
                }
#line 450
                DebugWork::g_dw.nakamich.m_pDCam = new (com_malloc_d(sizeof(CNakCamera), __FILE__ "(450)", RET_ADDR())) CNakCamera(pM, 1, 0x300, &vPos, 3.1415927f, (void**)&DebugWork::g_dw.nakamich.m_pDCam);
                DebugWork::g_dw.nakamich.m_pDCam->Action(0);
                DebugWork::g_dw.nakamich.m_pDCam->SetDebugViewAngle(-1e7f);
            }
        } else {
            DebugWork::g_dw.nakamich.m_pDCam->out();
        }
    }
    if (inParam & 1) {
        if (DebugWork::g_dw.nakamich.m_pDCam) {
            DebugWork::g_dw.nakamich.m_pDCam->Action(0);
        }
    }
    if (inParam & 2) {
        if (DebugWork::g_dw.nakamich.m_pDCam) {
            DebugWork::g_dw.nakamich.m_pDCam->Action(0x1F);
        }
    }
    return TRUE;
}

static DEBUG_WINDOW_LINE debugWinCameraLine[6] = {
    {"\x83" "f\x83o\x83" "b\x83O\x83J\x83\x81\x83\x89SPD   %s", &DebugWork::g_dw.nakamich.iCamSpeed, 1, {0}, 0, 0.0f, 3.0f, 1.0f, szCamSpeed, 0, NULL, NULL},
    {"\x83" "f\x83o\x83" "b\x83O\x83J\x83\x81\x83\x89INFO  %s", &DebugWork::g_dw.nakamich.iCamDisp, 1, {0}, 0, 0.0f, 1.0f, 1.0f, szOnOff, 0, NULL, NULL},
    {"\x83" "f\x83o\x83" "b\x83O\x83J\x83\x81\x83\x89" "ANGLE %5.1f", &DebugWork::g_dw.nakamich.fCamVAngle, 4, {0}, 1, 0.1f, 179.9f, 0.1f, NULL, 0, NULL, NULL},
    {"\x83" "f\x83o\x83" "b\x83O\x83J\x83\x81\x83\x89" "ANGLE RESET", &CDebugWindow::none, 1, {0}, 0, 0.0f, 0.0f, 0.0f, NULL, 0, DebugCameraAngleReset, NULL},
    {"\x83" "f\x83o\x83" "b\x83O\x83J\x83\x81\x83\x89ROTZ  %5.2f", &DebugWork::g_dw.nakamich.fCamRotZ, 4, {0}, 1, -3.1415927f, 3.1415927f, 0.01f, NULL, 0, NULL, NULL},
    {NULL, NULL, 0, {0}, 0, 0.0f, 0.0f, 0.0f, NULL, 0, NULL, NULL},
};

static DEBUG_WINDOW_TBL debugWinCamera = {"\x83" "f\x83o\x83" "b\x83O\x83J\x83\x81\x83\x89" "CONFIG", -160, -32, 14, 6, 12, 0, 0, debugWinCameraLine, NULL, NULL};

static BOOL debugPathOpenLineCall(CDebugWindow* pObj, INT inY, INT inParam) {
    if (inParam & 0x20) {
        CTask* pM = CTaskMan::getSysTask(2);
        CTask* pR = CTaskMan::getSysTask(1);
        if (pM && pR && !CDebugPathData::isDebugPathData()) {
            if (CDebugPathData::create(pR, TRUE)) {
                if (DebugWork::g_dw.nakamich.m_pDCam) {
                    DebugWork::g_dw.nakamich.m_pDCam->out();
                }
                pObj->sendMessage(2, 1);
            }
        }
    }
    return TRUE;
}

static BOOL DebugChangeFps(CDebugWindow* pObj, INT inY, INT inParam) {
    if (inParam & 0x10) {
        if (DebugWork::g_dw.sys.m_iFps) {
            CMainApp::setFpsHalf();
        } else {
            CMainApp::setFpsFull();
        }
    }
    return TRUE;
}

static BOOL debugWinRobotPadCallBack(CDebugWindow* pObj, INT inY, INT inParam) {
    if (inParam & 0x10) {
        if (DebugWork::g_dw.sys.m_iRobotPadMode == 0) {
            CPadIO::setRobotEnableFlag(FALSE, 0);
        } else if (DebugWork::g_dw.sys.m_iRobotPadMode == 1) {
            CPadIO::setRobotEnableFlag(TRUE, 0);
            for (INT i = 0; i < 24; i++) {
                CPadIO::setRobotPadRate(i, DebugWork::g_dw.sys.m_iRobotPadTrg[i], DebugWork::g_dw.sys.m_iRobotPadPress[i], 0);
            }
        } else if (DebugWork::g_dw.sys.m_iRobotPadMode == 2) {
            CPadIO::setRobotEnableFlag(TRUE, 0);
            CPadIO::setRobotPadRate(0, 50, 30, 0);
            CPadIO::setRobotPadRate(1, 5, 30, 0);
            CPadIO::setRobotPadRate(2, 50, 30, 0);
            CPadIO::setRobotPadRate(3, 50, 30, 0);
            CPadIO::setRobotPadRate(4, 700, 0, 0);
            CPadIO::setRobotPadRate(5, 600, 0, 0);
            CPadIO::setRobotPadRate(6, 150, 0, 0);
            CPadIO::setRobotPadRate(7, 50, 0, 0);
            CPadIO::setRobotPadRate(8, 50, 0, 0);
            CPadIO::setRobotPadRate(9, 0, 0, 0);
            CPadIO::setRobotPadRate(10, 0, 0, 0);
            CPadIO::setRobotPadRate(11, 0, 0, 0);
            CPadIO::setRobotPadRate(12, 0, 100, 0);
            CPadIO::setRobotPadRate(13, 0, 100, 0);
            CPadIO::setRobotPadRate(14, 0, 50, 0);
            CPadIO::setRobotPadRate(15, 0, 50, 0);
            CPadIO::setRobotPadRate(16, 0, 0, 0);
            CPadIO::setRobotPadRate(17, 0, 0, 0);
            CPadIO::setRobotPadRate(18, 0, 0, 0);
            CPadIO::setRobotPadRate(19, 0, 0, 0);
            CPadIO::setRobotPadRate(20, 0, 200, 0);
            CPadIO::setRobotPadRate(21, 0, 200, 0);
            CPadIO::setRobotPadRate(22, 0, 100, 0);
            CPadIO::setRobotPadRate(23, 0, 100, 0);
        }
    }
    return TRUE;
}

char* szRobotPadMode[3] = {"OFF", "\x83J\x83X\x83^\x83\x80", "\x83o\x83g\x83\x8b"};

static DEBUG_WINDOW_LINE debugWinRobotPadTrgLine[25] = {
    {"U\x83L\x81[        :  %3d", &DebugWork::g_dw.sys.m_iRobotPadTrg[12], 4, {0}, 0, 0.0f, 1000.0f, 10.0f, NULL, 0, debugWinRobotPadCallBack, NULL},
    {"D\x83L\x81[        :  %3d", &DebugWork::g_dw.sys.m_iRobotPadTrg[14], 4, {0}, 0, 0.0f, 1000.0f, 10.0f, NULL, 0, debugWinRobotPadCallBack, NULL},
    {"R\x83L\x81[        :  %3d", &DebugWork::g_dw.sys.m_iRobotPadTrg[13], 4, {0}, 0, 0.0f, 1000.0f, 10.0f, NULL, 0, debugWinRobotPadCallBack, NULL},
    {"L\x83L\x81[        :  %3d", &DebugWork::g_dw.sys.m_iRobotPadTrg[15], 4, {0}, 0, 0.0f, 1000.0f, 10.0f, NULL, 0, debugWinRobotPadCallBack, NULL},
    {"\x81\x9b\x83{\x83^\x83\x93      :  %3d", &DebugWork::g_dw.sys.m_iRobotPadTrg[5], 4, {0}, 0, 0.0f, 1000.0f, 10.0f, NULL, 0, debugWinRobotPadCallBack, NULL},
    {"\x81~\x83{\x83^\x83\x93      :  %3d", &DebugWork::g_dw.sys.m_iRobotPadTrg[6], 4, {0}, 0, 0.0f, 1000.0f, 10.0f, NULL, 0, debugWinRobotPadCallBack, NULL},
    {"\x81\xa2\x83{\x83^\x83\x93      :  %3d", &DebugWork::g_dw.sys.m_iRobotPadTrg[4], 4, {0}, 0, 0.0f, 1000.0f, 10.0f, NULL, 0, debugWinRobotPadCallBack, NULL},
    {"\x81\xa0\x83{\x83^\x83\x93      :  %3d", &DebugWork::g_dw.sys.m_iRobotPadTrg[7], 4, {0}, 0, 0.0f, 1000.0f, 10.0f, NULL, 0, debugWinRobotPadCallBack, NULL},
    {"L\x83X\x83" "e\x83" "B\x83" "b\x83NU:  %3d", &DebugWork::g_dw.sys.m_iRobotPadTrg[20], 4, {0}, 0, 0.0f, 1000.0f, 10.0f, NULL, 0, debugWinRobotPadCallBack, NULL},
    {"L\x83X\x83" "e\x83" "B\x83" "b\x83ND:  %3d", &DebugWork::g_dw.sys.m_iRobotPadTrg[22], 4, {0}, 0, 0.0f, 1000.0f, 10.0f, NULL, 0, debugWinRobotPadCallBack, NULL},
    {"L\x83X\x83" "e\x83" "B\x83" "b\x83NR:  %3d", &DebugWork::g_dw.sys.m_iRobotPadTrg[21], 4, {0}, 0, 0.0f, 1000.0f, 10.0f, NULL, 0, debugWinRobotPadCallBack, NULL},
    {"L\x83X\x83" "e\x83" "B\x83" "b\x83NL:  %3d", &DebugWork::g_dw.sys.m_iRobotPadTrg[23], 4, {0}, 0, 0.0f, 1000.0f, 10.0f, NULL, 0, debugWinRobotPadCallBack, NULL},
    {"R\x83X\x83" "e\x83" "B\x83" "b\x83NU:  %3d", &DebugWork::g_dw.sys.m_iRobotPadTrg[16], 4, {0}, 0, 0.0f, 1000.0f, 10.0f, NULL, 0, debugWinRobotPadCallBack, NULL},
    {"R\x83X\x83" "e\x83" "B\x83" "b\x83ND:  %3d", &DebugWork::g_dw.sys.m_iRobotPadTrg[18], 4, {0}, 0, 0.0f, 1000.0f, 10.0f, NULL, 0, debugWinRobotPadCallBack, NULL},
    {"R\x83X\x83" "e\x83" "B\x83" "b\x83NR:  %3d", &DebugWork::g_dw.sys.m_iRobotPadTrg[17], 4, {0}, 0, 0.0f, 1000.0f, 10.0f, NULL, 0, debugWinRobotPadCallBack, NULL},
    {"R\x83X\x83" "e\x83" "B\x83" "b\x83NL:  %3d", &DebugWork::g_dw.sys.m_iRobotPadTrg[19], 4, {0}, 0, 0.0f, 1000.0f, 10.0f, NULL, 0, debugWinRobotPadCallBack, NULL},
    {"L1\x83{\x83^\x83\x93      :  %3d", &DebugWork::g_dw.sys.m_iRobotPadTrg[2], 4, {0}, 0, 0.0f, 1000.0f, 10.0f, NULL, 0, debugWinRobotPadCallBack, NULL},
    {"R1\x83{\x83^\x83\x93      :  %3d", &DebugWork::g_dw.sys.m_iRobotPadTrg[3], 4, {0}, 0, 0.0f, 1000.0f, 10.0f, NULL, 0, debugWinRobotPadCallBack, NULL},
    {"L2\x83{\x83^\x83\x93      :  %3d", &DebugWork::g_dw.sys.m_iRobotPadTrg[0], 4, {0}, 0, 0.0f, 1000.0f, 10.0f, NULL, 0, debugWinRobotPadCallBack, NULL},
    {"R2\x83{\x83^\x83\x93      :  %3d", &DebugWork::g_dw.sys.m_iRobotPadTrg[1], 4, {0}, 0, 0.0f, 1000.0f, 10.0f, NULL, 0, debugWinRobotPadCallBack, NULL},
    {"L3\x83{\x83^\x83\x93      :  %3d", &DebugWork::g_dw.sys.m_iRobotPadTrg[9], 4, {0}, 0, 0.0f, 1000.0f, 10.0f, NULL, 0, debugWinRobotPadCallBack, NULL},
    {"R3\x83{\x83^\x83\x93      :  %3d", &DebugWork::g_dw.sys.m_iRobotPadTrg[10], 4, {0}, 0, 0.0f, 1000.0f, 10.0f, NULL, 0, debugWinRobotPadCallBack, NULL},
    {"\x83Z\x83\x8c\x83N\x83g\x83{\x83^\x83\x93:  %3d", &DebugWork::g_dw.sys.m_iRobotPadTrg[8], 4, {0}, 0, 0.0f, 1000.0f, 10.0f, NULL, 0, debugWinRobotPadCallBack, NULL},
    {"\x83X\x83^\x81[\x83g\x83{\x83^\x83\x93:  %3d", &DebugWork::g_dw.sys.m_iRobotPadTrg[11], 4, {0}, 0, 0.0f, 1000.0f, 10.0f, NULL, 0, debugWinRobotPadCallBack, NULL},
    {NULL, NULL, 0, {0}, 0, 0.0f, 0.0f, 0.0f, NULL, 0, NULL, NULL},
};

static DEBUG_WINDOW_LINE debugWinRobotPadPressLine[25] = {
    {"U\x83L\x81[        :  %3d", &DebugWork::g_dw.sys.m_iRobotPadPress[12], 4, {0}, 0, 0.0f, 1000.0f, 10.0f, NULL, 0, debugWinRobotPadCallBack, NULL},
    {"D\x83L\x81[        :  %3d", &DebugWork::g_dw.sys.m_iRobotPadPress[14], 4, {0}, 0, 0.0f, 1000.0f, 10.0f, NULL, 0, debugWinRobotPadCallBack, NULL},
    {"R\x83L\x81[        :  %3d", &DebugWork::g_dw.sys.m_iRobotPadPress[13], 4, {0}, 0, 0.0f, 1000.0f, 10.0f, NULL, 0, debugWinRobotPadCallBack, NULL},
    {"L\x83L\x81[        :  %3d", &DebugWork::g_dw.sys.m_iRobotPadPress[15], 4, {0}, 0, 0.0f, 1000.0f, 10.0f, NULL, 0, debugWinRobotPadCallBack, NULL},
    {"\x81\x9b\x83{\x83^\x83\x93      :  %3d", &DebugWork::g_dw.sys.m_iRobotPadPress[5], 4, {0}, 0, 0.0f, 1000.0f, 10.0f, NULL, 0, debugWinRobotPadCallBack, NULL},
    {"\x81~\x83{\x83^\x83\x93      :  %3d", &DebugWork::g_dw.sys.m_iRobotPadPress[6], 4, {0}, 0, 0.0f, 1000.0f, 10.0f, NULL, 0, debugWinRobotPadCallBack, NULL},
    {"\x81\xa2\x83{\x83^\x83\x93      :  %3d", &DebugWork::g_dw.sys.m_iRobotPadPress[4], 4, {0}, 0, 0.0f, 1000.0f, 10.0f, NULL, 0, debugWinRobotPadCallBack, NULL},
    {"\x81\xa0\x83{\x83^\x83\x93      :  %3d", &DebugWork::g_dw.sys.m_iRobotPadPress[7], 4, {0}, 0, 0.0f, 1000.0f, 10.0f, NULL, 0, debugWinRobotPadCallBack, NULL},
    {"L\x83X\x83" "e\x83" "B\x83" "b\x83NU:  %3d", &DebugWork::g_dw.sys.m_iRobotPadPress[20], 4, {0}, 0, 0.0f, 1000.0f, 10.0f, NULL, 0, debugWinRobotPadCallBack, NULL},
    {"L\x83X\x83" "e\x83" "B\x83" "b\x83ND:  %3d", &DebugWork::g_dw.sys.m_iRobotPadPress[22], 4, {0}, 0, 0.0f, 1000.0f, 10.0f, NULL, 0, debugWinRobotPadCallBack, NULL},
    {"L\x83X\x83" "e\x83" "B\x83" "b\x83NR:  %3d", &DebugWork::g_dw.sys.m_iRobotPadPress[21], 4, {0}, 0, 0.0f, 1000.0f, 10.0f, NULL, 0, debugWinRobotPadCallBack, NULL},
    {"L\x83X\x83" "e\x83" "B\x83" "b\x83NL:  %3d", &DebugWork::g_dw.sys.m_iRobotPadPress[23], 4, {0}, 0, 0.0f, 1000.0f, 10.0f, NULL, 0, debugWinRobotPadCallBack, NULL},
    {"R\x83X\x83" "e\x83" "B\x83" "b\x83NU:  %3d", &DebugWork::g_dw.sys.m_iRobotPadPress[16], 4, {0}, 0, 0.0f, 1000.0f, 10.0f, NULL, 0, debugWinRobotPadCallBack, NULL},
    {"R\x83X\x83" "e\x83" "B\x83" "b\x83ND:  %3d", &DebugWork::g_dw.sys.m_iRobotPadPress[18], 4, {0}, 0, 0.0f, 1000.0f, 10.0f, NULL, 0, debugWinRobotPadCallBack, NULL},
    {"R\x83X\x83" "e\x83" "B\x83" "b\x83NR:  %3d", &DebugWork::g_dw.sys.m_iRobotPadPress[17], 4, {0}, 0, 0.0f, 1000.0f, 10.0f, NULL, 0, debugWinRobotPadCallBack, NULL},
    {"R\x83X\x83" "e\x83" "B\x83" "b\x83NL:  %3d", &DebugWork::g_dw.sys.m_iRobotPadPress[19], 4, {0}, 0, 0.0f, 1000.0f, 10.0f, NULL, 0, debugWinRobotPadCallBack, NULL},
    {"L1\x83{\x83^\x83\x93      :  %3d", &DebugWork::g_dw.sys.m_iRobotPadPress[2], 4, {0}, 0, 0.0f, 1000.0f, 10.0f, NULL, 0, debugWinRobotPadCallBack, NULL},
    {"R1\x83{\x83^\x83\x93      :  %3d", &DebugWork::g_dw.sys.m_iRobotPadPress[3], 4, {0}, 0, 0.0f, 1000.0f, 10.0f, NULL, 0, debugWinRobotPadCallBack, NULL},
    {"L2\x83{\x83^\x83\x93      :  %3d", &DebugWork::g_dw.sys.m_iRobotPadPress[0], 4, {0}, 0, 0.0f, 1000.0f, 10.0f, NULL, 0, debugWinRobotPadCallBack, NULL},
    {"R2\x83{\x83^\x83\x93      :  %3d", &DebugWork::g_dw.sys.m_iRobotPadPress[1], 4, {0}, 0, 0.0f, 1000.0f, 10.0f, NULL, 0, debugWinRobotPadCallBack, NULL},
    {"L3\x83{\x83^\x83\x93      :  %3d", &DebugWork::g_dw.sys.m_iRobotPadPress[9], 4, {0}, 0, 0.0f, 1000.0f, 10.0f, NULL, 0, debugWinRobotPadCallBack, NULL},
    {"R3\x83{\x83^\x83\x93      :  %3d", &DebugWork::g_dw.sys.m_iRobotPadPress[10], 4, {0}, 0, 0.0f, 1000.0f, 10.0f, NULL, 0, debugWinRobotPadCallBack, NULL},
    {"\x83Z\x83\x8c\x83N\x83g\x83{\x83^\x83\x93:  %3d", &DebugWork::g_dw.sys.m_iRobotPadPress[8], 4, {0}, 0, 0.0f, 1000.0f, 10.0f, NULL, 0, debugWinRobotPadCallBack, NULL},
    {"\x83X\x83^\x81[\x83g\x83{\x83^\x83\x93:  %3d", &DebugWork::g_dw.sys.m_iRobotPadPress[11], 4, {0}, 0, 0.0f, 1000.0f, 10.0f, NULL, 0, debugWinRobotPadCallBack, NULL},
    {NULL, NULL, 0, {0}, 0, 0.0f, 0.0f, 0.0f, NULL, 0, NULL, NULL},
};

static DEBUG_WINDOW_LINE debugWinRobotPadLine[10] = {
    {"\x83\x8d\x83{\x83" "b\x83g\x83\\\x83" "E\x83T\x83\x82\x81[\x83h      : %s", &DebugWork::g_dw.sys.m_iRobotPadMode, 4, {0}, 0, 0.0f, 2.0f, 1.0f, szRobotPadMode, 0, debugWinRobotPadCallBack, NULL},
    {"\x83g\x83\x8a\x83K\x81[\x83J\x83N\x83\x8a\x83" "c (\x83J\x83X\x83^\x83\x80) ...", &CDebugWindow::none, 1, {0}, 0, 0.0f, 0.0f, 0.0f, NULL, 0, NULL, &debugWinRobotPadTrg},
    {"\x82\xc8\x82\xaa\x82\xa8\x82\xb5\x83J\x83N\x83\x8a\x83" "c (\x83J\x83X\x83^\x83\x80) ...", &CDebugWindow::none, 1, {0}, 0, 0.0f, 0.0f, 0.0f, NULL, 0, NULL, &debugWinRobotPadPress},
    {"------------------------------------------------", &CDebugWindow::none, 1, {0}, 2, 0.0f, 0.0f, 0.0f, NULL, 0, NULL, NULL},
    {"\xf9Q\x83g\x83\x8a\x83K\x81[\x83J\x83N\x83\x8a\x83" "c\x81" "A\x82\xc8\x82\xaa\x82\xa8\x82\xb5\x83J\x83N\x83\x8a\x83" "c\x81" "A\x89\x9f\x82\xb3\x82\xc8\x82\xa2\x83J\x83N\x83\x8a\x83" "c\x82\xcc", &CDebugWindow::none, 1, {0}, 2, 0.0f, 0.0f, 0.0f, NULL, 0, NULL, NULL},
    {"\xf9Q\x82R\x82\xc2\x82\xcc\x83S\x83" "E\x83P\x83" "C\x82\xf0" "1000%\x82\xc6\x82\xdd\x82\xc8\x82\xb7 (\x83{\x83^\x83\x93\x82\xb2\x82\xc6)", &CDebugWindow::none, 1, {0}, 2, 0.0f, 0.0f, 0.0f, NULL, 0, NULL, NULL},
    {"\xf9Q\x83I\x82\xb3\x82\xc8\x82\xa2\x83J\x83N\x83\x8a\x83" "c\x82\xcd\x83W\x83h\x83" "E\x83P\x83" "C\x83T\x83\x93\x82\xb3\x82\xea\x82\xe9", &CDebugWindow::none, 1, {0}, 2, 0.0f, 0.0f, 0.0f, NULL, 0, NULL, NULL},
    {"\xf9Q\x83g\x83\x8a\x83K\x81[\x83J\x83N\x83\x8a\x83" "c\x81{\x82\xc8\x82\xaa\x82\xa8\x82\xb5\x83J\x83N\x83\x8a\x83" "c\x82\xaa" "1000\x82\xf0\x83R\x82\xa6\x82\xe9\x82\xc6", &CDebugWindow::none, 1, {0}, 2, 0.0f, 0.0f, 0.0f, NULL, 0, NULL, NULL},
    {"\xf9Q\x82\xd9\x82\xda\x83I\x82\xb5\x82\xc1\x82\xcf\x82\xc8\x82\xb5\x82\xc9\x82\xc8\x82\xe9\x82\xcc\x82\xc5\x83`\x83\x85\x83" "E\x83" "C", &CDebugWindow::none, 1, {0}, 2, 0.0f, 0.0f, 0.0f, NULL, 0, NULL, NULL},
    {NULL, NULL, 0, {0}, 0, 0.0f, 0.0f, 0.0f, NULL, 0, NULL, NULL},
};

DEBUG_WINDOW_TBL debugWinRobotPadTrg = {"\x83g\x83\x8a\x83K\x81[\x83J\x83N\x83\x8a\x83" "c", -192, -152, 14, 24, 12, 0, 0, debugWinRobotPadTrgLine, NULL, NULL};

DEBUG_WINDOW_TBL debugWinRobotPadPress = {"\x82\xc8\x82\xaa\x82\xa8\x82\xb5\x83J\x83N\x83\x8a\x83" "c", -192, -152, 14, 24, 12, 0, 0, debugWinRobotPadPressLine, NULL, NULL};

DEBUG_WINDOW_TBL debugWinRobotPad = {"\x83\x8d\x83{\x83" "b\x83g\x83\\\x83" "E\x83T", -208, -168, 24, 9, 14, 0, 0, debugWinRobotPadLine, NULL, NULL};

static DEBUG_WINDOW_LINE debugWinTopLine[30] = {
    {"\x83" "f\x83o\x83" "b\x83OINFO        %s", &DebugWork::g_dw.sys.m_iDispCounter, 1, {0}, 0, 0.0f, 1.0f, 1.0f, szOnOff, 0, NULL, NULL},
    {"\x83t\x83\x8c\x81[\x83\x80\x83\x8c\x81[\x83g      %s", &DebugWork::g_dw.sys.m_iFps, 1, {0}, 0, 0.0f, 1.0f, 1.0f, szFps, 0, DebugChangeFps, NULL},
    {"\x83\x81\x83\x82\x83\x8a\x83V\x83\x88\x83" "E        Page:%d", &DebugWork::g_dw.sys.m_MemoryDisp, 1, {0}, 0, 0.0f, 4.0f, 1.0f, NULL, 0, DebugMemoryOn, NULL},
    {"\x83q\x81[\x83v\x83V\x83\x88\x83" "E(-FILE-) TTY\x82\xd6", &CDebugWindow::none, 1, {0}, 0, 0.0f, 0.0f, 0.0f, NULL, 0, DebugReportHeapDetail, NULL},
    {"\x83\x81\x83\x82\x83\x8aINFOTTY\x82\xd6 (2R3) %s", &DebugWork::g_dw.sys.m_iReportMemory, 1, {0}, 0, 0.0f, 1.0f, 1.0f, szOffOn, 0, NULL, NULL},
    {"\x83X\x83i\x83" "b\x83v\x83V\x83\x87\x83" "b\x83g(1L3) %s", &DebugWork::g_dw.sys.m_iSnapShot, 1, {0}, 0, 0.0f, 1.0f, 1.0f, szOffOn, 0, NULL, NULL},
    {"VRAM                %s", &DebugWork::g_dw.nakamich.iVramDisp, 2, {0}, 0, 0.0f, 1.0f, 1.0f, szOffOn, 0, NULL, NULL},
    {"\x83\x8d\x83{\x83" "b\x83g\x83\\\x83" "E\x83TCONFIG ...", &CDebugWindow::none, 1, {0}, 0, 0.0f, 0.0f, 0.0f, NULL, 0, NULL, &debugWinRobotPad},
    {"\xf9S--------\xf9V\x83J\x83\x81\x83\x89" "CONFIG\xf9S--------", &CDebugWindow::none, 1, {0}, 2, 0.0f, 0.0f, 0.0f, NULL, 0, NULL, NULL},
    {"\x83" "f\x83o\x83" "b\x83O\x83J\x83\x81\x83\x89      %s", &DebugWork::g_dw.nakamich.m_pDCam, 4, {0}, 0x24, 0.0f, 0.0f, 0.0f, szOffOn, 0, DebugCameraOn, NULL},
    {"\x83" "f\x83o\x83" "b\x83O\x83J\x83\x81\x83\x89" "CONFIG ...", &CDebugWindow::none, 1, {0}, 0, 0.0f, 0.0f, 0.0f, NULL, 0, NULL, &debugWinCamera},
    {"\xf9S-----------\xf9V\x82\xbb\x82\xcc\x83^\xf9S-----------", &CDebugWindow::none, 1, {0}, 2, 0.0f, 0.0f, 0.0f, NULL, 0, NULL, NULL},
    {"\x83v\x83\x8c\x83" "C\x83\x84" "CONFIG ...", &CDebugWindow::none, 1, {0}, 0, 0.0f, 0.0f, 0.0f, NULL, 0, DWTPlayer::DebugWinPlayer, &DWTPlayer::debugWinPlayer},
    {"\x83" "e\x83LCONFIG ...", &CDebugWindow::none, 1, {0}, 0, 0.0f, 0.0f, 0.0f, NULL, 0, NULL, &debugWinEnemy},
    {"DISPON/OFF ...", &CDebugWindow::none, 1, {0}, 0, 0.0f, 0.0f, 0.0f, NULL, 0, NULL, &debugWinDispTest},
    {"BGM\x83" "e\x83X\x83g ...", &CDebugWindow::none, 1, {0}, 0, 0.0f, 0.0f, 0.0f, NULL, 0, DebugBGMchangeCB, NULL},
    {"SE\x83" "e\x83X\x83g ...", &CDebugWindow::none, 1, {0}, 0, 0.0f, 0.0f, 0.0f, NULL, 0, NULL, &debugWinSeTest},
    {"\x83" "A\x82\xbd\x82\xe8" "CONFIG ...", &CDebugWindow::none, 1, {0}, 0, 0.0f, 0.0f, 0.0f, NULL, 0, NULL, &debugWinCollisionDisp},
    {"\x83t\x83H\x81[\x83J\x83X\x82\xc8\x82\xc7\x83R\x83" "E\x83J ...", &CDebugWindow::none, 1, {0}, 0, 0.0f, 0.0f, 0.0f, NULL, 0, NULL, &debugWinFocusTest},
    {"\x83J\x83Q\x83" "e\x83X\x83g ...", &CDebugWindow::none, 1, {0}, 0, 0.0f, 0.0f, 0.0f, NULL, 0, NULL, &debugWinShadowTest},
    {"\x83t\x83H\x83O\x83" "e\x83X\x83g ...", &CDebugWindow::none, 1, {0}, 0, 0.0f, 0.0f, 0.0f, NULL, 0, NULL, &DWTFog::debugWinFogTest},
    {"\x83u\x83\x89\x81[\x83" "e\x83X\x83g ...", &CDebugWindow::none, 1, {0}, 0, 0.0f, 0.0f, 0.0f, NULL, 0, NULL, &DWTBlur::debugWinBlurTest},
    {"\x83p\x83XCONFIG ...", &CDebugWindow::none, 1, {0}, 0, 0.0f, 0.0f, 0.0f, NULL, 0, debugPathOpenLineCall, NULL},
    {"\x82P\x82O\x82O\x83G\x81[\x83J\x81[ CONFIG ...", &CDebugWindow::none, 1, {0}, 0, 0.0f, 0.0f, 0.0f, NULL, 0, NULL, &debugWinMini100Acre},
    {"\x83M\x83~\x83" "b\x83N\x83`\x83\x87\x83" "E\x83Z\x83" "C ...", &CDebugWindow::none, 1, {0}, 0, 0.0f, 0.0f, 0.0f, NULL, 0, NULL, &DWTGimThrow::debugWinThrowTest},
    {"\xf9S----------\xf9V\x83" "C\x83x\x83\x93\x83g\xf9S----------", &CDebugWindow::none, 1, {0}, 2, 0.0f, 0.0f, 0.0f, NULL, 0, NULL, NULL},
    {"ATEL CONFIG", &CDebugWindow::none, 1, {0}, 0, 0.0f, 0.0f, 0.0f, NULL, 0, NULL, &DWTEvent::debugWinEventPlayer},
    {"\xf9S---------\xf9V\x83" "f\x81[\x83^\x83Z\x83" "b\x83" "e\x83" "C\xf9S---------", &CDebugWindow::none, 1, {0}, 2, 0.0f, 0.0f, 0.0f, NULL, 0, NULL, NULL},
    {"\x83\x81\x83j\x83\x85\x81[CONFIG ...", &CDebugWindow::none, 1, {0}, 0, 0.0f, 0.0f, 0.0f, NULL, 0, NULL, &debugWinMenuConfig},
    {NULL, NULL, 0, {0}, 0, 0.0f, 0.0f, 0.0f, NULL, 0, NULL, NULL},
};

DEBUG_WINDOW_TBL debugWinTop = {"\x83" "f\x83o\x83" "b\x83O\x83g\x83" "b\x83v", -240, -170, 14, 29, 12, 0, 0, debugWinTopLine, NULL, NULL};

} // namespace DWT
// SOURCE DATA: .data .rodata .bss
