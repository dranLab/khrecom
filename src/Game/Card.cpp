
#define GPRIM_H_PATH "System/Graphic/GPrim.h" /* MATCH: this unit names Gprim.h so (header default "System/Graphic/Gprim.h") */
#define CARD_H_PATH "c:/KHCOM/MainApp/Game/Card.h" /* MATCH: this unit names Card.h so (header default "Game/Card.h") */
#define TECHNIC_H_PATH "c:/KHCOM/MainApp/Game/Technic.h" /* MATCH: this unit names Technic.h so (header default "Game/Technic.h") */
#define STATUS_H_PATH "c:/KHCOM/MainApp/Game/Status.h" /* MATCH: this unit names Status.h so (header default "Game/Status.h") */
#define CARDBATTLE_H_PATH "c:/KHCOM/MainApp/Game/CardBattle.h" /* MATCH: this unit names CardBattle.h so (header default "Game/CardBattle.h") */
#include "Game/Status.h"
#include "Game/Technic.h"
#include "System/Ctd/CtdMan.h"
#include "System/Graphic/Gprim.h"
#include "System/Graphic/2d/2DMan.h"
#include "Game/CardBattle.h"
#include "Game/SceneInfo.h"
#include "Game/Card.h"
#include "Game/Game.h"
#include "Game/GameWork.h"
#include "Form/Menu/JMemoFlag.h"
#include <string.h>
#include "include_asm.h"





class CRsrcData;

#include "System/RsrcDB.h"






#include "System/Graphic/TexInfo.h"




















extern "C" int sprintf(char* buf, const char* fmt, ...);

#define ASSERT(e) if (!(e)) AssertApp(__FILE__, __LINE__, #e, 0)

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

CCard* CCard::m_pInstance = NULL;
CARD_INFO* CCard::m_pCardInfo = NULL;
CARD_INFO2* CCard::m_pCardInfo2 = NULL;
CARD CCard::m_deckCurrent[99];
void* CCard::m_pCardInitRsrc;
BC_PARAM* CCard::m_pBCParam;

CCard::CCard() {
    m_pCardInfo = (CARD_INFO*)CGameWork::progMCW()._cardInfo;
    m_pCardInfo2 = &CGameWork::progMCW2()._cardInfo2;
    m_pCardInitRsrc = NULL;
    m_pBCParam = NULL;
}

CCard::~CCard() {}

/* read through a reference: the original reloads m_pCardInfo after a store into it */
template <class T> static inline T& _ref(T& v) { return v; }

static inline void _setCard2(CARD& c, USHORT inKind, CHAR num) {
    c.m_nId = 0;
    c.m_nKind2 = inKind;
    c.m_nNumber2 = num;
    c.m_nCategory20 = cardDataTbl[c.m_nKind].nCategory0;
    c.m_nCategory21 = cardDataTbl[c.m_nKind].nCategory1;
}

static inline void _setCard(CARD& c, USHORT kind, BYTE num) {
    c.m_nId = 0;
    c.m_nKind2 = kind;
    c.m_nNumber2 = num;
    c.m_nCategory20 = cardDataTbl[c.m_nKind].nCategory0;
    c.m_nCategory21 = cardDataTbl[c.m_nKind].nCategory1;
}

void CCard::setCard(unsigned char (*pInitCardTbl)[2], INT cnt) {
    INT i;
    for (i = 0; i < cnt; i++) {
        if (pInitCardTbl[i][1] < 10) {
            USHORT idxCard = pInitCardTbl[i][0];
            addCard(idxCard, pInitCardTbl[i][1]);
            if (idxCard >= 0x44 && CGame::isRikuGame()) continue;
            CARD card;
            _setCard(card, idxCard, pInitCardTbl[i][1]);
            addDeck(card, 0);
        } else if (pInitCardTbl[i][1] < 20) {
            for (INT j = 0; j <= pInitCardTbl[i][1] - 10; j++) addCard(pInitCardTbl[i][0], j);
        } else if (pInitCardTbl[i][1] < 30) {
            for (INT j = pInitCardTbl[i][1] - 20; j < 10; j++) addCard(pInitCardTbl[i][0], j);
        } else if (pInitCardTbl[i][1] < 40) {
            for (INT j = 0; j < 10; j += pInitCardTbl[i][1] - 30) addCard(pInitCardTbl[i][0], j);
        }
    }
}
struct CI_HEADER {
    unsigned char sig[4];
    USHORT ver;
    USHORT cnt;
    UINT ofsTbl;
    UINT dataTbl;
};

static inline void _clampIdx(INT& ioIdx, INT inNo, INT inDefault) {
    if ((UINT)(inNo - 1) >= 13) ioIdx = inDefault;
}

/* near-match: kept as INCLUDE_ASM */
#if 0
void CCard::initCardWorld() {
    CI_HEADER* pCIHeader;
    USHORT* pIdxTbl;
    unsigned char (*pInitCardTbl)[2];
    INT idx;
    INT idxCard;
    CARD card;
    INT i;

    if (CGame::isSoraGame()) return;
    for (INT k = 0; k < 99; k++) removeDeck(0, k);
    for (INT i = 0; i < 10; i++) {
        while (deleteCard(0x12, i) > 0) {}
        for (INT j = 0x1A; j < 0x31; j++) {
            while (deleteCard(j, i) > 0) {}
        }
        for (INT j = 0x44; j < 0x63; j++) {
            while (deleteCard(j, i) > 0) {}
        }
    }
    if (!m_pCardInitRsrc) {
        CRsrcData* pData = CRsrcDB::getRsrc(1, "CardInit", NAME_CASE_SENSITIVE);
        void* p = pData ? CRsrcDB::getRsrcData(*pData) : NULL;
        m_pCardInitRsrc = p;
#line 118
        ASSERT(m_pCardInitRsrc);
    }
    pCIHeader = (CI_HEADER*)m_pCardInitRsrc;
#line 121
    ASSERT(*(UINT*)&pCIHeader->sig == 0x00494340);
    pIdxTbl = (USHORT*)((BYTE*)pCIHeader + pCIHeader->ofsTbl);
    INT nWorld = CSceneInfo::getCurrentWorldID(1);
    idx = nWorld + 14;
    if (nWorld == 0) {
        idx = 27;
    } else {
        _clampIdx(idx, nWorld, 28);
    }
    idx += 1;
    INT cnt = pIdxTbl[idx];
    if (idx) {
        cnt = cnt - pIdxTbl[idx - 1] - 8;
    } else {
        cnt = cnt - 8;
    }
    idxCard = pIdxTbl[idx - 1];
    pInitCardTbl = (unsigned char (*)[2])((BYTE*)pCIHeader + pCIHeader->dataTbl + idxCard * 2 + 0x10);
    setCard(pInitCardTbl, cnt);
    for (i = 99; i < 127; i++) {
        if (searchCard(i, 0)) {
            _setCard(card, i, 0);
            addDeck(card, 0);
        }
    }
}
#else
/* placeholder for initCardWorld's .rodata strings */
__asm__(".rdata\n"); /* the preceding INCLUDE_ASM left .text; gcc assumes .rdata for these strings */
inline void _initCardWorldStrings() { /* MATCH: placeholder: an inline function's strings, in the original .rodata order */
    void* m_pCardInitRsrc = CRsrcDB::getRsrc(1, "CardInit", NAME_CASE_SENSITIVE);
#line 118
    if (!m_pCardInitRsrc) AssertApp(__FILE__, __LINE__, "m_pCardInitRsrc", 0);
#line 121
    if (!(*(UINT*)&((CI_HEADER*)m_pCardInitRsrc)->sig == 0x00494340)) AssertApp(__FILE__, __LINE__, "*(UINT*)&pCIHeader->sig == 0x00494340", 0);
}
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Game/Card", initCardWorld__5CCard);
#endif
/* near-match: kept as INCLUDE_ASM */
#if 0
void CCard::initCard(USHORT inNo, WORLD_ID inDebugWorldID, SHORT inCurrentFloorNumber) {
    CI_HEADER* pCIHeader;
    USHORT* pIdxTbl;
    unsigned char (*pInitCardTbl)[2];
    INT idx;
    INT idxCard;
    static char* pDeckName[3] = { "Deck 1", "Deck 2", "Deck 3" };
    INT i;
    INT cnt;

    if (!m_pCardInitRsrc) {
        CRsrcData* pData = CRsrcDB::getRsrc(1, "CardInit", NAME_CASE_SENSITIVE);
        void* p = pData ? CRsrcDB::getRsrcData(*pData) : NULL;
        m_pCardInitRsrc = p;
#line 160
        ASSERT(m_pCardInitRsrc);
    }
    _ref(m_pCardInfo)->_nTotalMax = 999;
    _ref(m_pCardInfo)->_nFlag = 0;
    initCardAppearance();
    pCIHeader = (CI_HEADER*)m_pCardInitRsrc;
#line 173
    ASSERT(*(UINT*)&pCIHeader->sig == 0x00494340);
    pIdxTbl = (USHORT*)((BYTE*)pCIHeader + pCIHeader->ofsTbl);
    for (i = 0; i < 3; i++) strcpy(_ref(m_pCardInfo)->_deckName[i], pDeckName[i]);
    if (inNo == 0) {
        if (CGame::isSoraGame()) {
            idx = inCurrentFloorNumber - 1;
            _clampIdx(idx, inCurrentFloorNumber, 13);
        } else {
            idx = inDebugWorldID + 14;
            if (inDebugWorldID == 0) {
                idx = 27;
            } else {
                _clampIdx(idx, inDebugWorldID, 28);
            }
        }
        idx += 1;
        cnt = pIdxTbl[idx];
        if (idx) {
            cnt = cnt - pIdxTbl[idx - 1] - 8;
        } else {
            cnt = cnt - 8;
        }
        idxCard = pIdxTbl[idx - 1];
        pInitCardTbl = (unsigned char (*)[2])((BYTE*)pCIHeader + pCIHeader->dataTbl + idxCard * 2 + 0x10);
        setCard(pInitCardTbl, cnt);
        idx = (idx < 15) ? 0 : 15;
        cnt = pIdxTbl[idx];
        if (idx) {
            cnt = cnt - pIdxTbl[idx - 1] - 8;
        } else {
            cnt = cnt - 8;
        }
        idxCard = pIdxTbl[idx - 1];
        pInitCardTbl = (unsigned char (*)[2])((BYTE*)pCIHeader + pCIHeader->dataTbl + idxCard * 2 + 0x10);
        setCard(pInitCardTbl, cnt);
    } else if (inNo == 1 || inNo == 2) {
        if (inNo == 1) {
            idx = 0;
        } else {
            idx = 27;
        }
        idx += 1;
        cnt = pIdxTbl[idx];
        if (idx) {
            cnt = cnt - pIdxTbl[idx - 1] - 8;
        } else {
            cnt = cnt - 8;
        }
        idxCard = pIdxTbl[idx - 1];
        pInitCardTbl = (unsigned char (*)[2])((BYTE*)pCIHeader + pCIHeader->dataTbl + idxCard * 2 + 0x10);
        setCard(pInitCardTbl, cnt);
    }
}
#else
/* placeholder for initCard's static (.data) and its strings (emitted first from an inline, in .rodata) */
__asm__(".rdata\n"); /* the preceding INCLUDE_ASM left .text; gcc assumes .rdata for these strings */
inline void _initCardStrings() { /* MATCH: placeholder: an inline function's strings, in the original .rodata order */
    TRACE("Deck 1");
    TRACE("Deck 2");
    TRACE("Deck 3");
}
__asm__(".section .sdata\n");
__attribute__((section(".sdata"))) char* _initCardData(INT i) {
    static char* pDeckName[3] = { "Deck 1", "Deck 2", "Deck 3" };
    return pDeckName[i];
}
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Game/Card", initCard__5CCardUs8WORLD_IDs);
#endif
/* near-match: kept as INCLUDE_ASM */
#if 0
INT CCard::deleteCard(USHORT inID, SHORT inNumber) {
    CARD card;
    _setCard(card, inID, inNumber);
    USHORT fgPremium = card.m_fgPremium;
    switch (card.m_nCategory0) {
    case 0:
        return 0;
    case 1: {
        if (inNumber >= 10) return 0;
        if (!m_pCardInfo->_nBattleTotal) return -1;
        USHORT id = fgPremium ? inID - 0x101 : inID - 1;
        char& c = m_pCardInfo->_nAttak[fgPremium][id][inNumber];
        if (!c) return -2;
        c--;
        m_pCardInfo->_nBattleTotal--;
        resetBattleCardFullMessage();
        return 1;
    }
    case 2:
    case 3: {
        if (inNumber >= 10) return 0;
        if (!m_pCardInfo->_nBattleTotal) return -1;
        USHORT id = fgPremium ? inID - 0x11A : inID - 0x1A;
        char& c = m_pCardInfo->_nMagic[fgPremium][id][inNumber];
        if (!c) return -2;
        c--;
        m_pCardInfo->_nBattleTotal--;
        resetBattleCardFullMessage();
        return 1;
    }
    case 4: {
        if (inNumber >= 10) return 0;
        if (!m_pCardInfo->_nBattleTotal) return -1;
        USHORT id = inID - 0x2A;
        if (!m_pCardInfo->_nItem[id][inNumber]) return -2;
        m_pCardInfo->_nItem[id][inNumber]--;
        m_pCardInfo->_nBattleTotal--;
        resetBattleCardFullMessage();
        return 1;
    }
    case 7: {
        if (inNumber != 0) return 0;
        if (!m_pCardInfo->_nBattleTotal) return -1;
        USHORT id = inID - 0x44;
        char& c = m_pCardInfo->_nEnemy[id];
        if (!c) return -2;
        c--;
        m_pCardInfo->_nBattleTotal--;
        resetBattleCardFullMessage();
        return 1;
    }
    case 9: {
        if (inNumber >= 10) return 0;
        if (!m_pCardInfo->_nMapTotal) return -1;
        USHORT id = inID - 0xA1;
        char& c = m_pCardInfo->_nMap[id][inNumber];
        if (!c) return -2;
        c--;
        m_pCardInfo->_nMapTotal--;
        resetMapCardFullMessage();
        return 1;
    }
    case 10: {
        if (inNumber != 0) return 0;
        if (!m_pCardInfo2->_nKeyTotal) return -1;
        USHORT id = inID - 0xBA;
        char& c = m_pCardInfo2->_nKey[id];
        if (!c) return -2;
        c--;
        m_pCardInfo2->_nKeyTotal--;
        return 1;
    }
    }
    return 1;
}
#else
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Game/Card", deleteCard__5CCardUss);
__asm__(
    ".rdata\n"
    ".align 4\n"
    ".word .L001D6C08\n"
    ".word .L001D69F8\n"
    ".word .L001D6A5C\n"
    ".word .L001D6A5C\n"
    ".word .L001D6AC8\n"
    ".word .L001D6C50\n"
    ".word .L001D6C50\n"
    ".word .L001D6B3C\n"
    ".word .L001D6C50\n"
    ".word .L001D6B9C\n"
    ".word .L001D6C00\n"
    ".text\n");
#endif

/* near-match: kept as INCLUDE_ASM */
#if 0
INT CCard::searchCard(USHORT inID, SHORT inNumber) {
    CARD card;
    _setCard(card, inID, inNumber);
    USHORT fgPremium = card.m_fgPremium;
    INT nCard = 0;
    switch (card.m_nCategory0) {
    case 0:
        return 0;
    case 1: {
        if (inNumber >= 10) return 0;
        USHORT id = fgPremium ? inID - 0x101 : inID - 1;
        nCard = m_pCardInfo->_nAttak[fgPremium][id][inNumber];
        break;
    }
    case 2:
    case 3: {
        if (inNumber >= 10) return 0;
        USHORT id = fgPremium ? inID - 0x11A : inID - 0x1A;
        nCard = m_pCardInfo->_nMagic[fgPremium][id][inNumber];
        break;
    }
    case 4: {
        if (inNumber >= 10) return 0;
        nCard = m_pCardInfo->_nItem[inID - 0x2A][inNumber];
        break;
    }
    case 7: {
        if (inNumber != 0) return 0;
        USHORT id = inID - 0x44;
        nCard = m_pCardInfo->_nEnemy[id];
        break;
    }
    case 9: {
        if (inNumber >= 10) return 0;
        USHORT id = inID - 0xA1;
        nCard = m_pCardInfo->_nMap[id][inNumber];
        break;
    }
    case 10: {
        if (inNumber != 0) return 0;
        USHORT id = inID - 0xBA;
        nCard = m_pCardInfo2->_nKey[id];
        break;
    }
    }
    return nCard;
}
#else
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Game/Card", searchCard__5CCardUss);
__asm__(
    ".rdata\n"
    ".align 4\n"
    ".word .L001D6DFC\n"
    ".word .L001D6D18\n"
    ".word .L001D6D5C\n"
    ".word .L001D6D5C\n"
    ".word .L001D6DA0\n"
    ".word .L001D6E48\n"
    ".word .L001D6E48\n"
    ".word .L001D6DCC\n"
    ".word .L001D6E48\n"
    ".word .L001D6DF0\n"
    ".word .L001D6E28\n"
    ".text\n");
#endif

/* near-match: kept as INCLUDE_ASM */
#if 0
INT CCard::searchCardKind(USHORT inID, SHORT inNumber) {
    INT total = 0;
    if (inNumber < 0) {
        for (INT i = 0; i < 10; i++) {
            INT nCard = searchCard(inID, i);
            total += (nCard > 0) * nCard;
        }
    } else {
        total = searchCard(inID, inNumber);
        if (total < 1) total = 0;
    }
    return total;
}
#else
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Game/Card", searchCardKind__5CCardUss);
#endif

INT CCard::addDeck(CARD inCard, USHORT inDeckNum) {
    if (!inCard.m_nCategory0 || inCard.m_nCategory0 >= 8) return 0;
    if (_ref(m_pCardInfo)->_nDeck[inDeckNum] >= 99) return 0;
    CARD* pDeck = _ref(m_pCardInfo)->_deck[inDeckNum];
    for (INT i = 0; i < 99; i++) {
        if (!pDeck[i].m_nId) {
            pDeck[i] = inCard;
            _ref(m_pCardInfo)->_nDeck[inDeckNum]++;
            return 1;
        }
    }
    return 1;
}
/* near-match: kept as INCLUDE_ASM */
#if 0
INT CCard::insertDeck(CARD inCard, USHORT inDeckNum, SHORT inIdx, BOOL bShortCut) {
    CARD* pDeck;
    SHORT idxBlank;
    INT idxShortCut;
    INT i;

    if (m_pCardInfo->_nDeck[inDeckNum] >= 99) return 0;
    if (inIdx >= 99) return 0;
    pDeck = m_pCardInfo->_deck[inDeckNum];
    idxShortCut = getShortCut(inDeckNum);
    for (idxBlank = inIdx; idxBlank < 99 && pDeck[idxBlank].m_nId; idxBlank++) {}
    if (idxBlank < 99) {
        if (bShortCut) idxShortCut = inIdx;
        if (idxBlank != inIdx) {
            for (i = idxBlank; i > inIdx; i--) {
                pDeck[i] = pDeck[i - 1];
                if (i - 1 == idxShortCut) idxShortCut = bShortCut ? idxShortCut : i;
            }
        }
        pDeck[inIdx] = inCard;
        if (idxShortCut >= 0) setShortCut(inDeckNum, idxShortCut);
    } else {
#line 661
        ASSERT(inIdx);
        for (idxBlank = inIdx - 1; idxBlank >= 0 && pDeck[idxBlank].m_nId; idxBlank--) {}
        for (i = idxBlank; i < inIdx - 1; i++) {
            pDeck[i] = pDeck[i + 1];
            if (i + 1 == idxShortCut) idxShortCut--;
        }
        pDeck[inIdx - 1] = inCard;
        if (bShortCut) idxShortCut = inIdx - 1;
        if (m_pCardInfo->_nDeck[inDeckNum] < 96) {
            for (INT j = 0; j < 2; j++) {
                for (idxBlank = 98 - j; idxBlank >= 0 && pDeck[idxBlank].m_nId; idxBlank--) {}
#line 683
                ASSERT(idxBlank >= 0);
                for (i = idxBlank; i < 98; i++) {
                    pDeck[i] = pDeck[i + 1];
                    if (i + 1 == idxShortCut) idxShortCut--;
                }
                pDeck[98].m_nId = 0;
            }
        }
        if (idxShortCut >= 0) setShortCut(inDeckNum, idxShortCut);
    }
    if (inCard.m_nId) m_pCardInfo->_nDeck[inDeckNum]++;
    return 1;
}
#else
/* placeholder for insertDeck's .rodata strings */
__asm__(".rdata\n"); /* the preceding INCLUDE_ASM left .text; gcc assumes .rdata for these strings */
inline void _insertDeckStrings(SHORT inIdx, SHORT idxBlank) {
#line 661
    ASSERT(inIdx);
#line 683
    ASSERT(idxBlank >= 0);
}
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Game/Card", insertDeck__5CCardG4CARDUssi);
#endif

CARD* CCard::getCurrentDeck() {
    packCurrentDeck(m_pCardInfo->_nCurrentDeck);
    return m_deckCurrent;
}

void CCard::packCurrentDeck(USHORT inDeckNum) {
    CARD* pDeck = m_pCardInfo->_deck[inDeckNum];
    INT idx = 0;
    INT idxShortCut = getShortCut(inDeckNum);
    INT i;
    m_pCardInfo->_nShortCutIndex[3] = 0;
    for (i = 0; i < 99; i++) {
        if (pDeck[i].m_nId) {
            m_deckCurrent[idx] = pDeck[i];
            if (i == idxShortCut) m_pCardInfo->_nShortCutIndex[3] = idx + 1;
            idx++;
        }
    }
    for (INT j = idx; j < 99; j++) m_deckCurrent[j].m_nId = 0;
}

INT CCard::getCurrentShortCut() { return m_pCardInfo->_nShortCutIndex[3] - 1; }

void CCard::packDeck(USHORT inDeckNum) {
    packCurrentDeck(inDeckNum);
    CARD* pDeck = m_pCardInfo->_deck[inDeckNum];
    for (INT i = 0; i < 99; i++) pDeck[i] = m_deckCurrent[i];
    m_pCardInfo->_nShortCutIndex[inDeckNum] = m_pCardInfo->_nShortCutIndex[3];
}

INT CCard::removeDeck(USHORT inDeckNum, SHORT inIdx) {
    if (inIdx >= 99) return 0;
    CARD* pDeck = _ref(m_pCardInfo)->_deck[inDeckNum];
    if (!pDeck[inIdx].m_nId) return 0;
    pDeck[inIdx].m_nId = 0;
    _ref(m_pCardInfo)->_nDeck[inDeckNum]--;
    if (inIdx == getShortCut(inDeckNum)) clearShortCut(inDeckNum);
    return 1;
}

INT CCard::searchDeck(CARD inCard, USHORT inDeckNum, SHORT nFrom) {
    if (!inCard.m_nId || nFrom >= 99) return -1;
    CARD* pDeck = m_pCardInfo->_deck[inDeckNum];
    for (INT i = nFrom; i < 99; i++) {
        if (pDeck[i].m_nId == inCard.m_nId) return i;
    }
    return -1;
}
INT CCard::makePremiereDeck(USHORT inDeckNum, SHORT inIdx) {
    if (inIdx >= 100) return 0;
    CARD& card = m_pCardInfo->_deck[inDeckNum][inIdx];
    if (card.m_fgPremium) return 0;
    if (searchCard(card.m_nKind2, card.m_nNumber2) < 0) return 0;
    USHORT id;
    INT nCard;
    switch (card.m_nCategory0) {
    case 1:
        id = card.m_nKind - 1;
        if (m_pCardInfo->_nAttak[1][id][card.m_nNumber] >= 99) return 0;
        nCard = m_pCardInfo->_nAttak[0][id][card.m_nNumber];
        m_pCardInfo->_nAttak[0][id][card.m_nNumber]--;
        m_pCardInfo->_nAttak[1][id][card.m_nNumber]++;
        break;
    case 2:
    case 3:
        id = card.m_nKind - 0x1A;
        if (m_pCardInfo->_nMagic[1][id][card.m_nNumber] >= 99) return 0;
        nCard = m_pCardInfo->_nMagic[0][id][card.m_nNumber];
        m_pCardInfo->_nMagic[0][id][card.m_nNumber]--;
        m_pCardInfo->_nMagic[1][id][card.m_nNumber]++;
        break;
    default:
        return 0;
    }
    for (INT i = 0; i < 3; i++) {
        if (i == inDeckNum) continue;
        INT cnt = 0;
        INT idx0 = searchDeck(card, i, 0);
        for (INT idx = idx0; idx >= 0; idx = searchDeck(card, i, ++idx)) cnt++;
        if (cnt && cnt == nCard) m_pCardInfo->_deck[i][idx0].m_fgPremium = TRUE;
    }
    card.m_fgPremium = TRUE;
    return 1;
}

void CCard::clearDeck(USHORT inDeckNum) {
    CARD* pDeck = m_pCardInfo->_deck[inDeckNum];
    for (INT i = 98; i >= 0; i--) pDeck[i].m_nId = 0;
    m_pCardInfo->_nDeck[inDeckNum] = 0;
    clearShortCut(inDeckNum);
}

INT CCard::checkCurrentDeck() {
    INT nAttackCard = 0;
    INT nTotalCP = 0;
    CARD* pDeck = m_pCardInfo->_deck[m_pCardInfo->_nCurrentDeck];
    for (INT i = 0; i < 99; i++) {
        if (pDeck[i].m_nId) {
            if ((BYTE)(pDeck[i].m_nKind - 1) < 24) nAttackCard++;
            nTotalCP += getCP(pDeck[i]);
        }
    }
    if (nAttackCard) return (CStatus::getCPMax() < nTotalCP) << 1;
    return 1;
}

BOOL CCard::isCurrentDeckPremiumFull() {
    CARD* pDeck = m_pCardInfo->_deck[m_pCardInfo->_nCurrentDeck];
    for (INT i = 0; i < 99; i++) {
        CARD card = pDeck[i];
        if (card.m_nId && !card.m_fgPremium && canPremium(card.m_nKind)) return FALSE;
    }
    return TRUE;
}

INT CCard::getCP(CARD card) {
    INT nCost;
    if (!m_pBCParam) {
        CRsrcData* pData = CRsrcDB::getRsrc(1, "BattleCardList", NAME_CASE_SENSITIVE);
        void* p = pData ? CRsrcDB::getRsrcData(*pData) : NULL;
        m_pBCParam = (BC_PARAM*)p;
#line 932
        ASSERT(m_pBCParam);
    }
    nCost = 0;
    if ((BYTE)(card.m_nKind - 1) < 0x7E) {
        nCost = m_pBCParam[card.m_nKind].COST;
        if (!card.m_fgPremium && card.m_nKind < 0x44) {
            INT number = card.m_nNumber - 1;
            if (number < 0) number = 9;
            nCost += nCost / 10 * number;
        }
    }
    return nCost;
}

INT CCard::getMP(CARD card) {
    INT nMooglePoint;
    if (!m_pBCParam) {
        CRsrcData* pData = CRsrcDB::getRsrc(1, "BattleCardList", NAME_CASE_SENSITIVE);
        void* p = pData ? CRsrcDB::getRsrcData(*pData) : NULL;
        m_pBCParam = (BC_PARAM*)p;
#line 953
        ASSERT(m_pBCParam);
    }
    nMooglePoint = 0;
    if (card.m_nKind < 0x63) {
        nMooglePoint = getCP(card) / 3 * 2;
        if (card.m_fgPremium) nMooglePoint += 10;
    }
    return nMooglePoint;
}
/* near-match: kept as INCLUDE_ASM */
#if 0
BOOL CCard::getAttribute(CARD card, const CHAR** pOutStrAttack, const CHAR** pOutStrAttribute, const CHAR** pOutStrSpeed,
                         const CHAR** pOutHelp) {
    static char buf[16];
    CHAR* pLF;
    INT i;
    UINT ctdID;
    if (card.m_nKind < 0x7F) {
        ctdID = card.m_nKind > 0x19 ? 0x319 : card.m_nKind + 0x300;
        strcpy(buf, CCtdMan::getText(ctdID));
        *pOutStrAttack = buf;
        pLF = buf;
        for (i = 0; i < 2; i++) {
            pLF = strchr(pLF, '\n');
            if (!pLF) break;
            *pLF = 0;
            pLF++;
            if (i == 0) {
                *pOutStrAttribute = pLF;
            } else {
                *pOutStrSpeed = pLF;
            }
        }
        if (pOutHelp) *pOutHelp = CCtdMan::getText(card.m_nKind + 0x200);
        return TRUE;
    }
    *pOutStrSpeed = NULL;
    *pOutStrAttribute = NULL;
    *pOutStrAttack = NULL;
    if (pOutHelp) *pOutHelp = NULL;
    return FALSE;
}
#else
/* placeholder for getAttribute's static (.bss) */
__asm__(".section .sdata\n");
__attribute__((section(".sdata"))) char* _getAttributeData() {
    static char buf[16];
    return buf;
}
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Game/Card", getAttribute__5CCardG4CARDPPCcN32);
#endif

void CCard::setShortCut(USHORT inDeckNum, USHORT nIndex) {
    if (m_pCardInfo->_deck[inDeckNum][nIndex].m_nId) m_pCardInfo->_nShortCutIndex[inDeckNum] = nIndex + 1;
}

INT CCard::getShortCut(USHORT inDeckNum) { return m_pCardInfo->_nShortCutIndex[inDeckNum] - 1; }

void CCard::clearShortCut(USHORT inDeckNum) { m_pCardInfo->_nShortCutIndex[inDeckNum] = 0; }

UINT CCard::getCardMax() {
    return m_pCardInfo->_nBattleTotal + m_pCardInfo->_nMapTotal + m_pCardInfo2->_nKeyTotal;
}

UINT CCard::getCardMax(USHORT nCategory) {
    if (nCategory == 0) return m_pCardInfo->_nBattleTotal;
    if (nCategory == 1) return m_pCardInfo->_nMapTotal;
    if (nCategory == 2) return m_pCardInfo2->_nKeyTotal;
    return 0;
}
UINT CCard::getAttackCardMax(USHORT nCategory) {
    UINT cardMax = 0;
    CHAR* pRank;
    INT i;
    INT j;
    switch (nCategory) {
    case 0:
        for (i = 0; i < 2; i++) {
            for (j = 0; j < 24; j++) {
                pRank = m_pCardInfo->_nAttak[i][j];
                for (INT k = 0; k < 10; k++) cardMax += pRank[k];
            }
        }
        return cardMax;
    case 1:
        for (i = 0; i < 2; i++) {
            for (j = 0; j < 16; j++) {
                pRank = m_pCardInfo->_nMagic[i][j];
                for (INT k = 0; k < 10; k++) cardMax += pRank[k];
            }
        }
        return cardMax;
    case 2:
        for (j = 0; j < 7; j++) {
            pRank = m_pCardInfo->_nItem[j];
            for (i = 0; i < 10; i++) cardMax += pRank[i];
        }
        return cardMax;
    case 3:
        pRank = m_pCardInfo->_nEnemy;
        for (j = 0; j < 31; j++) cardMax += pRank[j];
        return cardMax;
    case 4:
        pRank = m_pCardInfo->_nEnemy;
        for (j = 0; j < 59; j++) cardMax += pRank[j];
        return cardMax;
    }
    return 0;
}
void CCard::setDeckName(INT inDeckIdx, const CHAR* pDeckName) {
    if (inDeckIdx < 3 && pDeckName && strlen(pDeckName) <= 16) strcpy(m_pCardInfo->_deckName[inDeckIdx], pDeckName);
}
void CCard::getCardTexInfo(CARD inCard, CHAR* outTexName, FVECTOR& outST) {
    IVECTOR vecUV;
    getCardTexInfo(inCard, outTexName, vecUV);
    VTransInfo& transInfo = CRsrcDB::getRsrcTex(outTexName, NAME_CASE_SENSITIVE)->_TexInfo;
    FLOAT fW;
    FLOAT fH;
    INT w = transInfo._nDistW;
    if (!transInfo._nFormat) {
        fW = w * 2;
    } else {
        fW = w;
    }
    INT h = transInfo._nDistH;
    if (!transInfo._nFormat) {
        fH = h * 2;
    } else {
        fH = h;
    }
    outST.set(vecUV.x / fW, vecUV.y / fH, vecUV.z / fW, vecUV.w / fH);
}

void CCard::getCardTexInfo(CARD inCard, CHAR* outTexName, IVECTOR& outUV) {
    INT iIdx = cardDataTbl[inCard.m_nKind].nTexPos;
    outUV.x = (iIdx & 3) * 64 + 1;
    outUV.y = (iIdx & 0xC) * 16 + 1;
    outUV.z = (iIdx & 3) * 64 + 63;
    outUV.w = (iIdx & 0xC) * 16 + 63;
    sprintf(outTexName, "card:%02d", ((CARD_DATA*)cardDataTbl)[inCard.m_nKind].nTexNo);
}

TexInfo* CCard::getCardTexInfo(CARD inCARD, FVECTOR& outST) {
    IVECTOR vecUV;
    TexInfo* pTexInfo = getCardTexInfo(inCARD, vecUV);
    VTransInfo& transInfo = pTexInfo->_TexInfo;
    FLOAT fW;
    FLOAT fH;
    INT w = transInfo._nDistW;
    if (!transInfo._nFormat) {
        fW = w * 2;
    } else {
        fW = w;
    }
    INT h = transInfo._nDistH;
    if (!transInfo._nFormat) {
        fH = h * 2;
    } else {
        fH = h;
    }
    outST.set(vecUV.x / fW, vecUV.y / fH, vecUV.z / fW, vecUV.w / fH);
    return pTexInfo;
}

TexInfo* CCard::getCardTexInfo(CARD inCard, IVECTOR& outUV) {
    INT iIdx = cardDataTbl[inCard.m_nKind].nTexPos;
    char buf[16];
    outUV.x = (iIdx & 3) * 64 + 1;
    outUV.y = (iIdx & 0xC) * 16 + 1;
    outUV.z = (iIdx & 3) * 64 + 63;
    outUV.w = (iIdx & 0xC) * 16 + 63;
    sprintf(buf, "card:%02d", ((CARD_DATA*)cardDataTbl)[inCard.m_nKind].nTexNo);
    return CRsrcDB::getRsrcTex(buf, NAME_CASE_SENSITIVE);
}
BOOL CCard::addCard(CARD card) { return addCard(card.m_nKind2, card.m_nNumber2); }

/* near-match: kept as INCLUDE_ASM */
#if 0
BOOL CCard::addCard(USHORT inID, SHORT inNumber) {
    if (!canAddCard(inID, inNumber)) return FALSE;
    if (inNumber < 0) inNumber = 0;
    CARD card;
    _setCard(card, inID, inNumber);
    USHORT fgPremium = card.m_fgPremium;
    USHORT id;
    switch (card.m_nCategory0) {
    case 1:
        if (!tryAddBattleCardAppearance(inID)) {
#line 1223
            AssertApp(__FILE__, __LINE__, "0", 0);
            return FALSE;
        }
        id = fgPremium ? inID - 0x101 : inID - 1;
        m_pCardInfo->_nAttak[fgPremium][id][inNumber]++;
        m_pCardInfo->_nBattleTotal++;
        return TRUE;
    case 2:
    case 3:
        if (!tryAddBattleCardAppearance(inID)) {
#line 1236
            AssertApp(__FILE__, __LINE__, "0", 0);
            return FALSE;
        }
        id = fgPremium ? inID - 0x11A : inID - 0x1A;
        m_pCardInfo->_nMagic[fgPremium][id][inNumber]++;
        m_pCardInfo->_nBattleTotal++;
        return TRUE;
    case 4:
        if (!tryAddBattleCardAppearance(inID)) {
#line 1248
            AssertApp(__FILE__, __LINE__, "0", 0);
            return FALSE;
        }
        id = inID - 0x2A;
        m_pCardInfo->_nItem[id][inNumber]++;
        m_pCardInfo->_nBattleTotal++;
        return TRUE;
    case 7:
        if (!tryAddBattleCardAppearance(inID)) {
#line 1260
            AssertApp(__FILE__, __LINE__, "0", 0);
            return FALSE;
        }
        id = inID - 0x44;
        m_pCardInfo->_nEnemy[id]++;
        m_pCardInfo->_nBattleTotal++;
        if (CGame::isRikuGame()) addDeck(card, 0);
        return TRUE;
    case 9:
        if (!tryAddMapCardAppearance(inID)) {
#line 1275
            AssertApp(__FILE__, __LINE__, "0", 0);
            return FALSE;
        }
        id = inID - 0xA1;
        m_pCardInfo->_nMap[id][inNumber]++;
        m_pCardInfo->_nMapTotal++;
        return TRUE;
    case 10:
        if (!tryAddKeyCardAppearance(inID)) {
#line 1287
            AssertApp(__FILE__, __LINE__, "0", 0);
            return FALSE;
        }
        id = inID - 0xBA;
        m_pCardInfo2->_nKey[id]++;
        m_pCardInfo2->_nKeyTotal++;
        return TRUE;
    }
    return FALSE;
}
#else
/* placeholder for addCard's .rodata string */
__asm__(".rdata\n"); /* the preceding INCLUDE_ASM left .text; gcc assumes .rdata for these strings */
inline void _addCardStrings() { AssertApp(__FILE__, 0, "0", 0); } /* MATCH: placeholder for the INCLUDE_ASM'd addCard's string */
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Game/Card", addCard__5CCardUss);
__asm__(
    ".rdata\n"
    ".align 4\n"
    ".word .L001D83A4\n"
    ".word .L001D8458\n"
    ".word .L001D84C8\n"
    ".word .L001D84C8\n"
    ".word .L001D8538\n"
    ".word .L001D83A4\n"
    ".word .L001D83A4\n"
    ".word .L001D8594\n"
    ".word .L001D83A4\n"
    ".word .L001D8608\n"
    ".word .L001D8670\n"
    ".text\n");
#endif

BOOL CCard::canAddCard(CARD card) { return canAddCard(card.m_nKind2, card.m_nNumber2); }

/* near-match: kept as INCLUDE_ASM */
#if 0
BOOL CCard::canAddCard(USHORT inID, SHORT inNumber) {
    if (!isAddCardValid(inID, inNumber)) return FALSE;
    if (isBattleTotalMaxUpCard(inID)) return TRUE;
    if (inNumber < 0) inNumber = 0;
    CARD card;
    _setCard(card, inID, inNumber);
    USHORT fgPremium = card.m_fgPremium;
    USHORT id;
    switch (card.m_nCategory0) {
    case 1:
        if (isBattleCardFull()) return FALSE;
        id = fgPremium ? inID - 0x101 : inID - 1;
        return m_pCardInfo->_nAttak[fgPremium][id][inNumber] < 99;
    case 2:
    case 3:
        if (isBattleCardFull()) return FALSE;
        id = fgPremium ? inID - 0x11A : inID - 0x1A;
        return m_pCardInfo->_nMagic[fgPremium][id][inNumber] < 99;
    case 4:
        if (isBattleCardFull()) return FALSE;
        id = inID - 0x2A;
        return m_pCardInfo->_nItem[id][inNumber] < 99;
    case 7:
        if (isBattleCardFull()) return FALSE;
        id = inID - 0x44;
        return m_pCardInfo->_nEnemy[id] < 99;
    case 9:
        if (isMapCardFull()) return FALSE;
        id = inID - 0xA1;
        return m_pCardInfo->_nMap[id][inNumber] < 9;
    case 10:
        if (isKeyCardFull()) return FALSE;
        id = inID - 0xBA;
        return !m_pCardInfo2->_nKey[id];
    }
    return FALSE;
}
#else
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Game/Card", canAddCard__5CCardUss);
__asm__(
    ".rdata\n"
    ".align 4\n"
    ".word .L001D88B4\n"
    ".word .L001D880C\n"
    ".word .L001D8858\n"
    ".word .L001D8858\n"
    ".word .L001D88A4\n"
    ".word .L001D88B4\n"
    ".word .L001D88B4\n"
    ".word .L001D88E4\n"
    ".word .L001D88B4\n"
    ".word .L001D8920\n"
    ".word .L001D8968\n"
    ".text\n");
#endif

/* near-match: kept as INCLUDE_ASM */
#if 0
BOOL CCard::isAddCardValid(USHORT inID, SHORT inNumber) {
    if (!isValid(inID)) return FALSE;
    USHORT kind = inID & 0xFF;
    CARD card;
    if (inNumber < 0) {
        _setCard(card, inID, 0);
    } else {
        _setCard(card, inID, inNumber);
    }
    switch (card.m_nCategory0) {
    case 1:
        if (inNumber < 0) return FALSE;
        if (inNumber >= 10) return FALSE;
        if (CGame::isSoraGame()) return kind != 0x12;
        return kind == 0x12;
    case 2:
    case 3:
        if (inNumber < 0) return FALSE;
        if (inNumber >= 10) return FALSE;
        if ((USHORT)(kind - 0x21) < 2) return FALSE;
        return TRUE;
    case 4:
        if (inNumber < 0) return FALSE;
        return inNumber < 10;
    case 7:
        if (!CGame::isSoraGame()) return TRUE;
        return kind != 0x59;
    case 9:
        if (inNumber < 0) return FALSE;
        if (inNumber >= 10) return FALSE;
        if (inID != 0xB9) return TRUE;
        return inNumber == 0;
    case 10:
        return TRUE;
    }
    return FALSE;
}
#else
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Game/Card", isAddCardValid__5CCardUss);
__asm__(
    ".rdata\n"
    ".align 4\n"
    ".word .L001D8AFC\n"
    ".word .L001D8AD0\n"
    ".word .L001D8B04\n"
    ".word .L001D8B04\n"
    ".word .L001D8B2C\n"
    ".word .L001D8AFC\n"
    ".word .L001D8AFC\n"
    ".word .L001D8B48\n"
    ".word .L001D8AFC\n"
    ".word .L001D8B68\n"
    ".word .L001D8B8C\n"
    ".text\n");
#endif
BOOL CCard::isBattleCardFull() { return m_pCardInfo->_nBattleTotal >= m_pCardInfo->_nBattleTotalMax; }

BOOL CCard::isMapCardFull() { return m_pCardInfo->_nMapTotal >= m_pCardInfo->_nMapTotalMax; }

BOOL CCard::isKeyCardFull() { return m_pCardInfo2->_nKeyTotal >= 4; }

BOOL CCard::shouldDispBattleCardFullMessage() {
    if (isBattleCardFull() && !(m_pCardInfo->_nFlag & 1)) return TRUE;
    return FALSE;
}

void CCard::finishBattleCardFullMessage() { m_pCardInfo->_nFlag |= 1; }

void CCard::resetBattleCardFullMessage() { m_pCardInfo->_nFlag &= ~1; }

BOOL CCard::shouldDispMapCardFullMessage() {
    if (isMapCardFull() && !(m_pCardInfo->_nFlag & 2)) return TRUE;
    return FALSE;
}

void CCard::finishMapCardFullMessage() { m_pCardInfo->_nFlag |= 2; }

void CCard::resetMapCardFullMessage() { m_pCardInfo->_nFlag &= ~2; }
/* near-match: kept as INCLUDE_ASM */
#if 0
CARD CCard::correctCardRank(CARD card) {
    INT nKind;
    INT nRank;
    INT i;
    INT nCategory = card.m_nCategory0;
    if (nCategory == 0) return card;
    if (nCategory < 5 || nCategory == 9) {
        if (canAddCard(card)) return card;
    } else {
        return card;
    }
    nRank = card.m_nNumber;
    nKind = card.m_nKind2;
    if (nRank != 0) {
        for (i = nRank - 1; i > 0; i--) {
            _setCard(card, nKind, i);
            if (canAddCard(card)) return card;
        }
        for (i = nRank + 1; i < 10; i++) {
            _setCard(card, nKind, i);
            if (canAddCard(card)) return card;
        }
        _setCard(card, nKind, 0);
        if (canAddCard(card)) return card;
    } else {
        for (i = 9; i > 0; i--) {
            _setCard(card, nKind, i);
            if (canAddCard(card)) return card;
        }
    }
    _setCard(card, 0, 0);
    return card;
}
#else
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Game/Card", correctCardRank__5CCardG4CARD);
#endif
BOOL CCard::isHaveMapCard(INT nRank) {
    if (nRank <= 0) nRank = 10;
    for (INT nKind = 0xA1; nKind < 0xBA; nKind++) {
        for (INT i = nRank; i < 11; i++) {
            INT n = i;
            if (i >= 10) n = 0;
            if (searchCard(nKind, n) > 0) return TRUE;
        }
    }
    return FALSE;
}

BOOL CCard::isSoraValid(USHORT inID) {
    UINT f = cardDataTbl[(BYTE)inID].nFlag & 2;
    return f != 0;
}

BOOL CCard::isRikuValid(USHORT inID) {
    UINT f = cardDataTbl[(BYTE)inID].nFlag & 4;
    return f != 0;
}

BOOL CCard::isValid(USHORT inID) {
    if (CGame::isSoraGame()) return isSoraValid(inID);
    return isRikuValid(inID);
}

BOOL CCard::canPremium(USHORT inID) { return cardDataTbl[(BYTE)inID].nFlag & 1; }

LPCSTR CCard::getName(USHORT inID) {
    static char dummy[4] = "-";
    CARD card;
    _setCard(card, inID, 0);
    LPCSTR p = card.getName();
    if (!p) p = dummy;
    return p;
}
void CCard::initCardAppearance() {
    INT i;
    TRACE("===== \x83J\x81[\x83h\x82\xcc\x8fo\x8c\xbb\x8f\xf3\x91\xd4\x82\xf0\x8f\x89\x8a\xfa\x89\xbb\x82\xb5\x82\xdc\x82\xb7 =====\n");
    if (!m_pCardInfo) {
        TRACE("===== \x83J\x81[\x83h\x82\xcc\x8fo\x8c\xbb\x8f\xf3\x91\xd4\x82\xcc\x8f\x89\x8a\xfa\x89\xbb\x82\xc9\x8e\xb8\x94s\x81I =====\n");
        return;
    }
    memset(m_pCardInfo->_fgNew, 0, 0x4B);
    for (i = 0; i < 0x12A; i++) setCardAppearance(i, 0);
    if (CGame::isSoraGame()) {
        _ref(m_pCardInfo)->_nBattleTotalMax = 0x38B;
        _ref(m_pCardInfo)->_nMapTotalMax = 99;
        setCardPossible(1);
        setCardPossible(0x1B);
        setCardPossible(0x1D);
        setCardPossible(0x2A);
    } else {
        _ref(m_pCardInfo)->_nBattleTotalMax = 999;
        _ref(m_pCardInfo)->_nMapTotalMax = 99;
        setCardPossible(0x12);
    }
}

void CCard::setCardPossible(USHORT usKind2) {
    USHORT kind = usKind2 & 0xFF;
    if (!kind) return;
    if (!isValid(usKind2)) return;
    if (getCardAppearance(usKind2)) return;
    CARD card;
    _setCard(card, usKind2, 0);
    if (0) TRACE("\x83J\x81[\x83h\x82\xaa\x89\xf0\x8b\xd6\x82\xb3\x82\xea\x82\xdc\x82\xb5\x82\xbd\x81I [Kind %3d: %s]\n", card.m_nKind2, card.getName());
    setCardAppearance(kind, 1);
    usKind2 = kind | 0x100;
    if (canPremium(kind)) setCardAppearance(usKind2, 1);
}

BOOL CCard::isCardPossible(USHORT usKind2) {
    if (!(usKind2 & 0xFF)) return FALSE;
    return getCardAppearance(usKind2) != 0;
}

void CCard::setCardPossibleWorld(INT nWorldID) {
    if (!CGame::isSoraGame()) return;
    switch (nWorldID) {
    case 0:
    case 13:
        break;
    case 2:
        setCardPossible(2);
        break;
    case 3:
        setCardPossible(9);
        break;
    case 4:
        setCardPossible(0xB);
        break;
    case 5:
        setCardPossible(6);
        break;
    case 6:
        setCardPossible(4);
        break;
    case 7:
        setCardPossible(3);
        break;
    case 8:
        setCardPossible(5);
        break;
    case 9:
        setCardPossible(0xC);
        break;
    }
}
void CCard::setCardNew(USHORT usKind2) {
    USHORT kind = usKind2 & 0xFF;
    if (!kind) return;
    if (getCardAppearance(usKind2) != 1) return;
    setCardAppearance(usKind2, 2);
    CJiminyMemoFlag::checkEvent(4, kind, 0);
    switch (kind) {
    case 0x1A:
        CTechnic::addTechnic(0xD);
        CTechnic::addTechnic(0x14);
        break;
    case 0x1B:
        CTechnic::addTechnic(0xE);
        CTechnic::addTechnic(0x15);
        break;
    case 0x1C:
        CTechnic::addTechnic(0xF);
        CTechnic::addTechnic(0x16);
        break;
    case 0x1D:
        CTechnic::addTechnic(0x10);
        CTechnic::addTechnic(0x17);
        break;
    case 0x1E:
        CTechnic::addTechnic(0x11);
        CTechnic::addTechnic(0x18);
        break;
    case 0x1F:
        CTechnic::addTechnic(0x12);
        CTechnic::addTechnic(0x19);
        break;
    case 0x20:
        CTechnic::addTechnic(0x13);
        CTechnic::addTechnic(0x1A);
        break;
    case 0x23:
        CTechnic::addTechnic(0x34);
        CTechnic::addTechnic(0x35);
        break;
    case 0x24:
        CTechnic::addTechnic(0x3D);
        CTechnic::addTechnic(0x3E);
        break;
    case 0x25:
        CTechnic::addTechnic(0x38);
        CTechnic::addTechnic(0x39);
        break;
    case 0x26:
        CTechnic::addTechnic(0x36);
        CTechnic::addTechnic(0x37);
        break;
    case 0x27:
        CTechnic::addTechnic(0x3F);
        CTechnic::addTechnic(0x40);
        break;
    case 0x28:
        CTechnic::addTechnic(0x3B);
        CTechnic::addTechnic(0x3C);
        break;
    case 0x29:
        CTechnic::addTechnic(0x41);
        CTechnic::addTechnic(0x42);
        break;
    }
}

BOOL CCard::isCardNew(USHORT usKind2) {
    if (!(usKind2 & 0xFF)) return FALSE;
    return getCardAppearance(usKind2) == 2;
}

BOOL CCard::setCardNewFinish(USHORT usKind2) {
    if (!(usKind2 & 0xFF)) return FALSE;
    if (getCardAppearance(usKind2) == 2) {
        setCardAppearance(usKind2, 3);
        return TRUE;
    }
    return FALSE;
}

BOOL CCard::isCardAlreadyGet(USHORT usKind2) {
    if (!(usKind2 & 0xFF)) return FALSE;
    return (UINT)(getCardAppearance(usKind2) - 2) < 2;
}

/* near-match: kept as INCLUDE_ASM */
#if 0
BOOL CCard::isBattleTotalMaxUpCard(USHORT usKind2) {
    if (!CGame::isSoraGame()) return FALSE;
    if (!isCardForTotalMaxUp(usKind2)) return FALSE;
    USHORT usKind = usKind2 & 0xFF;
    USHORT usKindP = usKind | 0x100;
    if (canPremium(usKind)) {
        if (isCardAlreadyGet(usKind)) return FALSE;
        if (isCardAlreadyGet(usKindP)) return FALSE;
        return TRUE;
    }
    BOOL bGet = isCardAlreadyGet(usKind);
    if (!bGet) return TRUE;
    return FALSE;
}
#else
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Game/Card", isBattleTotalMaxUpCard__5CCardUs);
#endif

BOOL CCard::isCardForTotalMaxUp(USHORT usKind2) {
    USHORT usKind = usKind2 & 0xFF;
    if (!((usKind >= 1 && usKind <= 24) || (usKind >= 0x1A && usKind <= 0x29) || (usKind >= 0x2A && usKind <= 0x30) ||
          (usKind >= 0x44 && usKind <= 0x7E))) {
        return FALSE;
    }
    if (usKind != 2 && usKind != 9 && usKind != 0xB && usKind != 6 && usKind != 4 && usKind != 3 && usKind != 5 &&
        usKind != 0xC && usKind != 0x10 && usKind != 0x11) {
        return isSoraValid(usKind);
    }
    return FALSE;
}

#define CARD_BTL_TOTAL_MAX 999
#define CARD_APPEARANCE_BUFF_SIZE 75

BOOL CCard::tryAddBattleCardAppearance(USHORT usKind2) {
    if (isBattleTotalMaxUpCard(usKind2)) {
        m_pCardInfo->_nBattleTotalMax++;
        CARD card;
        _setCard(card, usKind2, 0);
        TRACE("\x8f\x8a\x8e\x9d\x89\xc2\x94\\\x82\xc8\x83o\x83g\x83\x8b\x83J\x81[\x83h\x82\xcc\x8d\xc5\x91\xe5\x92l\x82\xaa\x91\x9d\x82\xa6\x82\xdc\x82\xb5\x82\xbd\x81I [BattleTotalMax = %4d] [Kind %3d: %s]\n",
              m_pCardInfo->_nBattleTotalMax, card.m_nKind2, card.getName());
        if (m_pCardInfo->_nBattleTotalMax >= 1000) {
            TRACE("\x92v\x96\xbd\x93I\x82\xc8\x83G\x83\x89\x81[\x81I\x82\xb1\x82\xea\x88\xc8\x8f\xe3\x8d\xc5\x91\xe5\x90\x94\x82\xf0\x91\x9d\x82\xe2\x82\xb9\x82\xdc\x82\xb9\x82\xf1\x81I [BattleTotalMax = %4d] [Kind %3d: %s]\n",
                  m_pCardInfo->_nBattleTotalMax, card.m_nKind2, card.getName());
#line 2106
            ASSERT(m_pCardInfo->_nBattleTotalMax <= CARD_BTL_TOTAL_MAX);
            m_pCardInfo->_nBattleTotalMax = CARD_BTL_TOTAL_MAX;
        }
    }
    if (m_pCardInfo->_nBattleTotal >= m_pCardInfo->_nBattleTotalMax) return FALSE;
    setCardPossible(usKind2);
    setCardNew(usKind2);
    return TRUE;
}

BOOL CCard::tryAddMapCardAppearance(USHORT usKind2) {
    setCardPossible(usKind2);
    setCardNew(usKind2);
    return TRUE;
}

BOOL CCard::tryAddKeyCardAppearance(USHORT usKind2) {
    setCardPossible(usKind2);
    setCardNew(usKind2);
    return TRUE;
}

INT CCard::getCardAppearance(USHORT usKind2) {
    USHORT idx = usKind2 >> 2;
    USHORT shift = (usKind2 & 3) * 2;
    BYTE* pTop = (BYTE*)m_pCardInfo->_fgNew;
#line 2160
    ASSERT(idx < CARD_APPEARANCE_BUFF_SIZE);
    return (pTop[idx] >> shift) & 3;
}

void CCard::setCardAppearance(USHORT usKind2, INT nAppear) {
    USHORT idx = usKind2 >> 2;
    USHORT shift = (usKind2 & 3) * 2;
    BYTE* pTop = (BYTE*)m_pCardInfo->_fgNew;
    BYTE value = nAppear & 3;
#line 2180
    ASSERT(idx < CARD_APPEARANCE_BUFF_SIZE);
    pTop[idx] = (pTop[idx] & ~(3 << shift)) | (value << shift);
}

// SOURCE DATA: .data .rodata .bss
