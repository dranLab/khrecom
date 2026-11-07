
#define GPRIM_H_PATH "System/Graphic/GPrim.h" /* MATCH: this unit names Gprim.h so (header default "System/Graphic/Gprim.h") */
#define SAVELOADCTRL_H_PATH "c:/KHCOM/MainApp/Form/Menu/SaveLoadCtrl.h" /* MATCH: this unit names SaveLoadCtrl.h so (header default "Form/Menu/SaveLoadCtrl.h") */
#include <string.h>
#include "System/Graphic/Gprim.h"
#include "System/Graphic/2d/2DMan.h"
#include "Form/Menu/SaveLoadCtrl.h"


#include "System/TaskMan.h"



#include "System/Ctd/CtdMan.h"
#include "Game/CardBattle.h"
#include "Game/Technic.h"

/* MATCH: Placeholder: an empty string follows Technic.h's strings in .rodata; which header it comes from is not known. */
inline void _emptyStringPlaceholder() {
    TRACE("");
}

#include "Game/CardKind.h"
#include "Game/Game.h"
#include "Game/GameWork.h"
#include "Form/Menu/JMemoFlag.h"
#include "Form/Menu/BtlReportCtrl.h"
#include "Form/Menu/WorldCard.h"
#include "System/MainApp.h"
#include "System/Sound/SoundMan.h"
#include "System/Graphic/2d/2DCard.h"
#include "System/Fsm.h"

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

inline void* operator new(size_t, void* p) throw() { return p; }

#define ASSERT(e) if (!(e)) AssertApp(__FILE__, __LINE__, #e, 0)

#include "System/RsrcDB.h"




#include "System/Rcf.h"
#include "System/HeapMan.h"





static inline void _setCard(CARD& c, USHORT kind, BYTE num) {
    c.m_nId = 0;
    c.m_nNumber2 = num;
    c.m_nKind2 = kind;
    c.m_nCategory20 = cardDataTbl[c.m_nKind].nCategory0;
    c.m_nCategory21 = cardDataTbl[c.m_nKind].nCategory1;
}

namespace WorldCard {

struct _LayoutInfo {
    SHORT nLayout;
    SHORT nIdx;
};

#line 70 "c:/KHCOM/MainApp/Form/Menu/WorldCard.cpp"
CWorldCardSeq* CWorldCardSeq::m_pInstance = NULL;

void CWorldCardSeq::create(CTask* in_pCTask, INT in_CreateType) {
    if (!m_pInstance) {
#line 74
        new (com_malloc_d(sizeof(CWorldCardSeq), __FILE__ "(74)", RET_ADDR())) CWorldCardSeq(in_pCTask, in_CreateType);
    }
}

CWorldCardSeq::CWorldCardSeq(CTask* pParent, INT in_CreateType) : CTask(pParent, 0x8000) {
    m_State[0].set(this, &CWorldCardSeq::beginStateInit, &CWorldCardSeq::stateInit, &CWorldCardSeq::endStateInit);
    m_State[1].set(this, &CWorldCardSeq::beginStateMain, &CWorldCardSeq::stateMain, &CWorldCardSeq::endStateMain);
    m_State[2].set(this, &CWorldCardSeq::beginStateExit, &CWorldCardSeq::stateExit, &CWorldCardSeq::endStateExit);
    m_State[3].set(this, &CWorldCardSeq::beginStateEnd, &CWorldCardSeq::stateEnd, &CWorldCardSeq::endStateEnd);
    m_bFpsBackup = CMainApp::isFpsHalf();
    CMainApp::setFpsFull();
    changeState(STATE_INIT);
    m_pInstance = this;
    m_Type = in_CreateType;
    for (INT i = 0; i < 12; i++) m_pStrings[i] = NULL;
    for (INT i = 0; i < 5; i++) m_pCard[i] = NULL;
}

/* MATCH: Placeholder: __FILE__ precedes the destructor's message in .rodata (used later by changeState's assert). */
inline void _filePlaceholder() {
    AssertApp(__FILE__, 0, 0, 0);
}

CWorldCardSeq::~CWorldCardSeq() {
    TRACE("CWorldCardSeq::~CWorldCardSeq()\n");
    m_pInstance = NULL;
    for (INT i = 0; i < 5; i++) {
        if (m_pCard[i]) {
            delete m_pCard[i];
            m_pCard[i] = NULL;
        }
    }
    for (INT i = 0; i < 12; i++) {
        if (m_pStrings[i]) {
            delete m_pStrings[i];
            m_pStrings[i] = NULL;
        }
    }
    CRsrcDB::removeGroup(2, 63);
    if (m_bFpsBackup) CMainApp::setFpsHalf();
}

void CWorldCardSeq::main() {
    update();
}

void CWorldCardSeq::disp() {}

void CWorldCardSeq::beginStateInit() {
    CRcf::reqLoadRCF(2, 63, 0, NULL, 0);
}

void CWorldCardSeq::stateInit() {
    if (CRcf::isCompleteLoadRCF(0, 0)) changeState(STATE_MAIN);
}

void CWorldCardSeq::endStateInit() {}

void CWorldCardSeq::beginStateMain() {
    createGetWorldCardWnd();
    m_Step = 0;
    m_fWait = 0.0f;
    if (m_pStrings[0]) CSound::playSE(0x3A, -1, 0x40);
}

void CWorldCardSeq::stateMain() {
    if (!m_pStrings[0]) {
        changeState(STATE_EXIT);
        return;
    }
    m_fWait += getFrameRateF();
    if (CPadHandler::isTrgOK() && !m_Step && 60.0f <= m_fWait) {
        m_Step = 1;
        for (INT i = 0; i < 12; i++) {
            if (m_pStrings[i] && m_pStrings[i]->getState() == C2DSeqAgent::STATE_IN) {
                m_pStrings[i]->changeState(C2DSeqAgent::STATE_LOOP, TRUE, -1);
            }
        }
    }
    if (m_pStrings[0]->getState() == C2DSeqAgent::STATE_LOOP) {
        D2::C2DSeqCtrl* pCtrl = m_pStrings[0]->C2DSeqAgent::getSeqCtrl();
        if (pCtrl) {
            HANDLE* pHandle = pCtrl->getHandle();
            if (pHandle && D2::C2DMan::isEnd(*pHandle)) {
                for (INT i = 0; i < 12; i++) {
                    if (m_pStrings[i]) m_pStrings[i]->dispOff();
                }
                changeState(STATE_EXIT);
            }
        }
    }
}

void CWorldCardSeq::endStateMain() {}

void CWorldCardSeq::beginStateExit() {}

void CWorldCardSeq::stateExit() {
    out();
    changeState(STATE_END);
}

void CWorldCardSeq::endStateExit() {}

void CWorldCardSeq::beginStateEnd() {}

void CWorldCardSeq::stateEnd() {}

void CWorldCardSeq::endStateEnd() {}

void CWorldCardSeq::changeState(INT state) {
#line 298
    ASSERT((0 <= state) && (state < STATE_MAX));
    TRACE_USER(KYOSHIOK, "StateChange:--->%d\n", state);
    gotoState(m_State[state]);
}

INT CWorldCardSeq::isNowDispCard() {
    return m_pInstance != NULL;
}

C2DSeqStrings* CWorldCardSeq::createSeq(INT in_SeqIndex, INT in_SeqType, INT in_LayoutDef, INT in_DispLayer,
                                        const CHAR* in_pStr) {
    C2DSeqAgent::SeqTbl tblSeqType[4] = { { { 0, 1, 0 } }, { { 2, 3, 0 } }, { { 4, 5, 0 } }, { { 6, 7, 0 } } };
    C2DSeqAgent::SeqTbl& t = tblSeqType[in_SeqType];
    _LayoutInfo info;
    FVECTOR vPos;

    info.nLayout = in_LayoutDef;
    info.nIdx = in_SeqIndex;
    D2::C2DMan::getLayPosition("get_wcard.lad", info.nLayout, info.nIdx, vPos, FALSE);
#line 342
    C2DSeqStrings* pAgent = new (com_malloc_d(sizeof(C2DSeqStrings), __FILE__ "(342)", RET_ADDR())) C2DSeqStrings(this);
    pAgent->setupSeq(t, "get_wcard.lad", 0, in_pStr, in_DispLayer, 16);
    pAgent->setupState(t);
    pAgent->setPos(vPos);
    return pAgent;
}

void CWorldCardSeq::createGetWorldCardWnd() {
    switch (m_Type) {
    case 0:
        WorldWndOne(0xBE, CCtdMan::getText(0x380000) ?: "");
        CJiminyMemoFlag::checkEvent(4, 0xBE, 0);
        CGameWork::setByte(0, 11, 0);
        break;
    case 1: {
        int KindTable[5] = { 0xBF, 0xC0, 0xC1, 0xC2, 0xC3 };
        WorldWndFive(KindTable, CCtdMan::getText(0x380001) ?: "");
        CJiminyMemoFlag::checkEvent(4, 0xBF, 0);
        CJiminyMemoFlag::checkEvent(4, 0xC0, 0);
        CJiminyMemoFlag::checkEvent(4, 0xC1, 0);
        CJiminyMemoFlag::checkEvent(4, 0xC2, 0);
        CJiminyMemoFlag::checkEvent(4, 0xC3, 0);
        CGameWork::setByte(0, 11, 1);
        break;
    }
    case 2: {
        int KindTable[4] = { 0xC4, 0xC5, 0xC6, 0xC7 };
        WorldWndFour(KindTable, CCtdMan::getText(0x380002) ?: "");
        CJiminyMemoFlag::checkEvent(4, 0xC4, 0);
        CJiminyMemoFlag::checkEvent(4, 0xC5, 0);
        CJiminyMemoFlag::checkEvent(4, 0xC6, 0);
        CJiminyMemoFlag::checkEvent(4, 0xC7, 0);
        CGameWork::setByte(0, 11, 6);
        break;
    }
    case 3:
        WorldWndOne(0xC8, CCtdMan::getText(0x380003) ?: "");
        CJiminyMemoFlag::checkEvent(4, 0xC8, 0);
        CGameWork::setByte(0, 11, 10);
        break;
    case 4:
        WorldWndOne(0xC9, CCtdMan::getText(0x380004) ?: "");
        CJiminyMemoFlag::checkEvent(4, 0xC9, 0);
        if (CGame::isSoraGame()) {
            CGameWork::setByte(0, 11, 11);
        } else {
            CGameWork::setByte(0, 11, 9);
        }
        break;
    case 5:
        if (CGame::isRikuGame()) {
            WorldWndOne(0xCA, CCtdMan::getText(0x380005) ?: "");
            CJiminyMemoFlag::checkEvent(4, 0xCA, 0);
            CGameWork::setByte(0, 11, 11);
        } else {
            CGameWork::setByte(0, 11, 12);
        }
        break;
    case 6:
        WorldWndOne(0xC6, CCtdMan::getText(0x380006) ?: "");
        CJiminyMemoFlag::checkEvent(4, 0xC6, 0);
        CGameWork::setByte(0, 11, 0);
        break;
    case 7: {
        int KindTable[4] = { 0xBE, 0xBF, 0xC2, 0xC5 };
        WorldWndFour(KindTable, CCtdMan::getText(0x380007) ?: "");
        CJiminyMemoFlag::checkEvent(4, 0xBE, 0);
        CJiminyMemoFlag::checkEvent(4, 0xBF, 0);
        CJiminyMemoFlag::checkEvent(4, 0xC2, 0);
        CJiminyMemoFlag::checkEvent(4, 0xC5, 0);
        CGameWork::setByte(0, 11, 1);
        break;
    }
    case 8: {
        int KindTable[4] = { 0xC0, 0xC1, 0xC3, 0xC4 };
        WorldWndFour(KindTable, CCtdMan::getText(0x380008) ?: "");
        CJiminyMemoFlag::checkEvent(4, 0xC0, 0);
        CJiminyMemoFlag::checkEvent(4, 0xC1, 0);
        CJiminyMemoFlag::checkEvent(4, 0xC3, 0);
        CJiminyMemoFlag::checkEvent(4, 0xC4, 0);
        CGameWork::setByte(0, 11, 5);
        break;
    }
    }
}

void CWorldCardSeq::WorldWndOne(INT in_CardKind, const CHAR* in_pStr) {
    CARD card;

    m_pStrings[0] = createSeq(0, 2, 0, 11, in_pStr);
    m_pStrings[0]->start();
    m_pStrings[1] = createSeq(1, 0, 0, 11, NULL);
    m_pStrings[1]->start();
    _setCard(card, in_CardKind, 0);
#line 473
    m_pCard[0] = new (com_malloc_d(sizeof(C2DCard), __FILE__ "(473)", RET_ADDR())) C2DCard;
    m_pCard[0]->create(card);
    m_pCard[0]->setParent(m_pStrings[1]->getSeqCtrl());
}

void CWorldCardSeq::WorldWndFour(INT* in_CardKindTable, const CHAR* in_pStr) {
    CARD card;

    m_pStrings[0] = createSeq(0, 3, 1, 11, in_pStr);
    m_pStrings[0]->start();
    for (INT i = 0; i < 4; i++) {
        m_pStrings[i + 1] = createSeq(i + 1, 1, 1, 11, NULL);
        m_pStrings[i + 1]->start();
        _setCard(card, in_CardKindTable[i], 0);
#line 497
        m_pCard[i] = new (com_malloc_d(sizeof(C2DCard), __FILE__ "(497)", RET_ADDR())) C2DCard;
        m_pCard[i]->create(card);
        m_pCard[i]->setParent(m_pStrings[i + 1]->getSeqCtrl());
    }
}

void CWorldCardSeq::WorldWndFive(INT* in_CardKindTable, const CHAR* in_pStr) {
    CARD card;

    m_pStrings[0] = createSeq(0, 3, 2, 11, in_pStr);
    m_pStrings[0]->start();
    for (INT i = 0; i < 5; i++) {
        m_pStrings[i + 1] = createSeq(i + 1, 1, 2, 11, NULL);
        m_pStrings[i + 1]->start();
        _setCard(card, in_CardKindTable[i], 0);
#line 522
        m_pCard[i] = new (com_malloc_d(sizeof(C2DCard), __FILE__ "(522)", RET_ADDR())) C2DCard;
        m_pCard[i]->create(card);
        m_pCard[i]->setParent(m_pStrings[i + 1]->getSeqCtrl());
    }
}

}  // namespace WorldCard
// SOURCE DATA: .data .rodata
