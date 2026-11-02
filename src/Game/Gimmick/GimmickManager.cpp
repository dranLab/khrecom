#define GAME_GIMMICK_GIMMICKMANAGER_CPP

#include <string.h>
#include "Game/Gimmick/GimmickManager.h"
extern "C" int sprintf(char* buf, const char* fmt, ...);
#include "Game/OsdDef.h"
#include "System/Graphic/Gprim.h"
#include "System/Graphic/Model.h"
#include "System/RsrcData.h"
#include "System/Graphic/2d/2DMan.h"
#include "Game/CardBattle.h"
#include "Game/Card.h"
#include "System/Graphic/Anim.h"
#include "System/Graphic/Str.h"
#include "Game/Collision/CollUtil.h"

/* Placeholder: an empty string follows CollUtil.h's strings in .rodata (as in GimmickCarry); which header it
 * comes from is not known. */
inline void _emptyStringPlaceholder() {
    TRACE("");
}

#include "Game/Gimmick/GimmickTreasureBox.h"
#include "Game/Gimmick/GimmickSavePoint.h"
#include "Game/Gimmick/GimmickDoor.h"
#include "Game/EntityFactory.h"
#include "Game/SceneInfo.h"
#include "Game/GameWork.h"
#include "Game/StorageWork.h"
#include "Game/Game.h"
#include "Game/Debug/DebugWork.h"
#include "Form/DebugWindow.h"
#include "Form/Menu/JMemoFlag.h"
#include "System/Rnd.h"
#include "System/Vector.h"
#include "System/Com.h"

inline void* operator new(size_t, void* p) throw() { return p; }

#define RET_ADDR() ({ UINT nRA; __asm__ volatile("daddu %0, $31, $0" : "=r"(nRA)); nRA; })







#include "System/Buf.h"






class CRsrcData {
    CBuf* m_pDataSrc;

public:
    CBuf* getDataSrc() { return m_pDataSrc; }
    BYTE* getRsrcData() {
        BYTE* p = NULL;
        if (getDataSrc()) p = getDataSrc()->getBufPtr();
        return p;
    }
};

#include "System/RsrcDB.h"





extern INT SCREEN_HEIGHT;

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

/* gimmicks whose state is kept in the room's storage */
#define IS_STORAGE_GIMMICK(t)                                                                                        \
    ((t)->getTypeID() == 0x1005 || (t)->getTypeID() == 0x2000 || (t)->getTypeID() == 0x2001 ||                       \
     (t)->getTypeID() == 0x2002 || (t)->getTypeID() == 0x2003 || (t)->getTypeID() == 0x2004 ||                       \
     (t)->getTypeID() == 0x2005)

CGimmickManager::CGimmickManager(CTask* inParent) : CTask(inParent, 0x4000) {
    m_iStep = 0;
    declareSysTask(17);
    m_pNomalCardDropProbTable = NULL;
    m_pTreasureCardDropProbTable = NULL;
    m_pGimParam = NULL;
    m_pGBallDropTable = NULL;
    m_pRBallDropTable = NULL;
    m_WorldGimNum = 0;
    memset(&m_MapCardProb, 0, sizeof(m_MapCardProb));
    init_();
    TRACE("---- Constructor CGimmickManager\n");
}

CGimmickManager::~CGimmickManager() {
    if (m_pNomalCardDropProbTable) {
        com_free(m_pNomalCardDropProbTable);
        m_pNomalCardDropProbTable = NULL;
    }
    if (m_pTreasureCardDropProbTable) {
        com_free(m_pTreasureCardDropProbTable);
        m_pTreasureCardDropProbTable = NULL;
    }
    if (m_MapCardProb.pProb) {
        com_free(m_MapCardProb.pProb);
        m_MapCardProb.pProb = NULL;
    }
    if (m_pGimParam) {
        com_free(m_pGimParam);
        m_pGimParam = NULL;
    }
    if (m_pGBallDropTable) {
        com_free(m_pGBallDropTable);
        m_pGBallDropTable = NULL;
    }
    if (m_pRBallDropTable) {
        com_free(m_pRBallDropTable);
        m_pRBallDropTable = NULL;
    }
    TRACE("---- Destructor CGimmickManager\n");
}

void CGimmickManager::main() {
    if (m_iStep == 0) {
        if ((USHORT)(CSceneInfo::getCurrentSceneNum() - 2001) >= 1000) {
            LoadCommonResrc();
            LoadWorldResrc();
            LoadCommonResrcMapCard();
            LoadWorldResrcMapCard();
        }
        AfterInit_();
        m_iStep++;
    }
}

void CGimmickManager::disp() {}

HANDLE CGimmickManager::getNearestGimmick(CEntity& inEntity) {
    FLOAT nearest = 1e+08f;
    HANDLE handle = -1;
    SLContainerT<CTask>* pContainer;
    for (pContainer = getChildren().getHead(); pContainer != NULL; pContainer = pContainer->getNext()) {
        CTask* task = pContainer->getValue();
        if (task != NULL) {
            if (task->getTypeID() == 0x1005 || task->getTypeID() == 0x2000 || task->getTypeID() == 0x2001 ||
                task->getTypeID() == 0x1006 || task->getTypeID() == 0x2002 || task->getTypeID() == 0x2003 ||
                task->getTypeID() == 0x2004 || task->getTypeID() == 0x2005) {
                FLOAT dist = ((CEntity*)task)->calcDistanceSq(inEntity);
                if (dist < nearest) {
                    handle = task->getHandle();
                    nearest = dist;
                }
            }
        }
    }
    return handle;
}

void CGimmickManager::initRoomCreate() {
    INT SaveGimCounter;
    INT count = 0;
    int TrapTabel[3];
    _GimmickRoomInfo* gim;
    INT gimIndex = 0;
    SLContainerT<CTask>* pContainer;

    memset(TrapTabel, 0, sizeof(TrapTabel));
    gim = CSceneInfo::getCurrentRoomGimmickInfo();
    if (gim == NULL) return;
    gim->_cardCount = 0;
    if (CSceneInfo::getCurrentSceneNum() == 8) {
        TrapTabel[getRand() % 3] = 1;
    }
    if (CSceneInfo::getCurrentSceneNum() == 24) {
        if (CGameWork::getFlag(0, 0x11)) {
            CGameWork::progMCW()._KeyRoomsCreateState = 1;
        }
    }
    SaveGimCounter = 0;
    for (pContainer = getChildren().getHead(); pContainer != NULL; pContainer = pContainer->getNext()) {
        CTask* task = pContainer->getValue();
        if (task == NULL) continue;
        if (IS_STORAGE_GIMMICK(task)) {
            switch ((INT)(((CEntity*)task)->getID() % 10000U)) {
            case 4:
            case 20:
            case 21:
            case 103:
            case 114:
            case 134:
            case 135:
            case 203:
            case 303:
            case 403:
            case 503:
            case 550:
            case 551:
            case 552:
            case 553:
            case 554:
            case 603:
            case 617:
            case 703:
            case 704:
            case 803:
            case 837:
            case 838:
            case 903:
            case 926:
            case 1059:
            case 1060:
            case 1077:
            case 1090:
            case 1091:
            case 1092:
            case 1103:
            case 1150:
            case 1203:
            case 1303:
            case 1314:
            case 1410:
            case 1411:
            case 1412:
            case 1413:
            case 9006:
            case 9008:
            case 9010:
                continue;
            }
            count++;
            if (SaveGimCounter >= 48) {
                TRACE("initRoomCreateERROR:\x83M\x83~\x83""b\x83N\x8f\xee\x95\xf1\x95\xdb\x8e\x9d\x83o\x83""b\x83t\x83@\x82\xaa\x83I\x81[\x83o\x81[\x82\xb5\x82\xdc\x82\xb7\x81I\x81I\x94z\x92u\x90\x94\x82\xf0\x8am\x94""F\x82\xb5\x82\xc4\x82\xad\x82\xbe\x82\xb3\x82\xa2\n");
                AssertApp(__FILE__, 273, "0", 0);
                SaveGimCounter = 47;
            }
            gim->_gimmicks[SaveGimCounter]._nID = ((CBaseGimmick*)task)->getUniqueID();
            gim->_gimmicks[SaveGimCounter]._nStat = 0;
            SaveGimCounter++;
        }
        if (CSceneInfo::getCurrentSceneNum() == 8 && task->getTypeID() == 0x2000) {
            if (gimIndex >= 3) {
                TRACE("initRoomCreateERROR:\x8bU\x82\xe8\x82\xcc\x95\x94\x89\xae\x82\xcc\x95\xf3\x94\xa0\x82\xaa\x91\xbd\x82\xb7\x82\xac\x82\xdc\x82\xb7\x81I\x81I\n");
                AssertApp(__FILE__, 292, "0", 0);
                gimIndex = 2;
            }
            if (!TrapTabel[gimIndex]) {
                ((CGimmickTreasureBox*)task)->setState(4);
            }
            gimIndex++;
        }
    }
    TRACE("\x83V\x81[\x83\x93\x82\xc9\x94z\x92u\x82\xb3\x82\xea\x82\xc4\x82\xa2\x82\xe9\x8f\xf3\x91\xd4\x95\xdb\x91\xb6\x82\xb7\x82\xe9\x83M\x83~\x83""b\x83N\x90\x94\x81""F%d\n", count);
}

INT CGimmickManager::getStorageGimmickIndex(INT inUniqueID) {
    _GimmickRoomInfo* gim = CSceneInfo::getCurrentRoomGimmickInfo();
    if (gim == NULL) return -1;
    for (int i = 0; i < 48; i++) {
        if (gim->_gimmicks[i]._nID == inUniqueID) return i;
    }
    return -1;
}

INT CGimmickManager::getStorageGimmickState(INT inUniqueID) {
    INT index = getStorageGimmickIndex(inUniqueID);
    if (index == -1) return -1;
    _GimmickRoomInfo* gim = CSceneInfo::getCurrentRoomGimmickInfo();
    if (gim->_gimmicks[index]._nID != inUniqueID) return -1;
    return gim->_gimmicks[index]._nStat;
}

BOOL CGimmickManager::setStorageGimmickState(INT inUniqueID, BYTE inState) {
    if (!dbgCheckUser((USER)15)) {
        TRACE("\x83M\x83~\x83""b\x83N\x83""f\x81[\x83^\x95\xdb\x91\xb6ID\x81""F%d\n", inUniqueID);
    }
    INT index = getStorageGimmickIndex(inUniqueID);
    if (index != -1) {
        CSceneInfo::getCurrentRoomGimmickInfo()->_gimmicks[index]._nStat = inState;
        return TRUE;
    }
    TRACE("\x83M\x83~\x83""b\x83N\x83""f\x81[\x83^\x95\xdb\x91\xb6\x95s\x89\xc2\n");
    return FALSE;
}

CEntity* CGimmickManager::getDoor(DOOR_ID inDoorID, FVECTOR* outPos, FVECTOR* outRot) {
    SLContainerT<CTask>* pContainer;
    for (pContainer = getChildren().getHead(); pContainer != NULL; pContainer = pContainer->getNext()) {
        CTask* task = pContainer->getValue();
        if (task != NULL && !task->isOut() && task->getTypeID() == 0x1006 &&
            ((CGimmickDoor*)task)->getDoorID() == inDoorID) {
            if (outPos) *outPos = ((CEntity*)task)->getPos();
            if (outRot) *outRot = ((CEntity*)task)->getRot();
            return (CEntity*)task;
        }
    }
    return NULL;
}

BOOL CGimmickManager::init_() {
    OSD_HEADER* header = (OSD_HEADER*)CRsrcDB::getFirstData_Scene(0x22);
    if (header == NULL) return FALSE;
    if (!header->addrMapping()) return FALSE;
    if (header->pFormation[0] == NULL) return FALSE;
    for (UINT i = 0; i < 16; i++) {
        OBJ_GROUP* group = header->pFormation[0]->getGimmickGroup(i);
        if (group == NULL) continue;
        OBJ_INFO* info = group->pObjInfo;
        for (INT j = 0; j < group->iObjCount; j++, info++) {
            CEntity* entity = CEntityFactory::getInstance()->createEntity(*info, this, &info->iUniqueID);
            if (entity != NULL && IS_STORAGE_GIMMICK(entity)) {
                ((CBaseGimmick*)entity)->setParam(info->iParam);
            }
            info->setFlag(1, TRUE);
        }
    }
    return TRUE;
}

BOOL CGimmickManager::AfterInit_() {
    SLContainerT<CTask>* pContainer;
    switch (CSceneInfo::getCurrentWorldID(0)) {
    case 1:
    case 8:
    case 9:
    case 11:
    case 13:
        CGameWork::setFlag(1, 0x27, TRUE);
        break;
    case 23:
        switch (CSceneInfo::getCurrentSceneNum()) {
        case 1:
        case 8:
        case 9:
        case 11:
        case 13:
            CGameWork::setFlag(1, 0x27, TRUE);
            break;
        default:
            CGameWork::setFlag(1, 0x27, FALSE);
            break;
        }
        break;
    default:
        CGameWork::setFlag(1, 0x27, FALSE);
        break;
    }
    m_ThrowMinAngle = 0.0f;
    m_ThrowDirPow = 0.2f;
    m_ThrowMaxAngle = 45.0f;
    CSceneInfo::getCurrentSceneNum();
    for (pContainer = getChildren().getHead(); pContainer != NULL; pContainer = pContainer->getNext()) {
        CTask* task = pContainer->getValue();
        if (task != NULL && IS_STORAGE_GIMMICK(task)) {
            ((CEntity*)task)->AfterInit();
        }
    }
    return TRUE;
}

BOOL CGimmickManager::LoadCommonResrc() {
    CRsrcData* cmnGimData = CRsrcDB::getRsrc(1, "GimCom.bin", NAME_CASE_SENSITIVE);
    if (cmnGimData == NULL) {
        TRACE("\x83M\x83~\x83""b\x83N\x83}\x83l\x81[\x83W\x83\x83::\x91S\x83\x8f\x81[\x83\x8b\x83h\x8b\xa4\x92\xca\x83""f\x81[\x83^\x82\xaa\x82\xa0\x82\xe8\x82\xdc\x82\xb9\x82\xf1\x81""B\n");
        return FALSE;
    }
    BYTE* p = cmnGimData->getRsrcData();
    memcpy(m_CardProbNum, p + 6, sizeof(m_CardProbNum));
    memcpy(m_CardDropRoomMaxTable, p + 0x1A, sizeof(m_CardDropRoomMaxTable));
    return TRUE;
}

BOOL CGimmickManager::LoadWorldResrc() {
    CRsrcData* cmnGimData = CRsrcDB::getRsrc(1, "AtkCardDrop.bin", NAME_CASE_SENSITIVE);
    if (cmnGimData != NULL) {
        SHORT* p = (SHORT*)cmnGimData->getRsrcData();
        p = (SHORT*)((BYTE*)p + 6);
        if ((m_NomalCardDropProbNum = *p++) != 0) {
            m_pNomalCardDropProbTable = (GimmickEveryCardProb*)com_malloc_d(
                m_NomalCardDropProbNum * sizeof(GimmickEveryCardProb), __FILE__ "(602)", RET_ADDR());
            for (int i = 0; i < m_NomalCardDropProbNum; i++) {
                m_pNomalCardDropProbTable[i].CardID = *p++;
                m_pNomalCardDropProbTable[i].Prob = *p++;
            }
        }
        if ((m_TreasureCardDropProbNum = *p++) != 0) {
            m_pTreasureCardDropProbTable = (GimmickEveryCardProb*)com_malloc_d(
                m_TreasureCardDropProbNum * sizeof(GimmickEveryCardProb), __FILE__ "(614)", RET_ADDR());
            for (int i = 0; i < m_TreasureCardDropProbNum; i++) {
                m_pTreasureCardDropProbTable[i].CardID = *p++;
                m_pTreasureCardDropProbTable[i].Prob = *p++;
            }
        }
    } else {
        TRACE("\x83M\x83~\x83""b\x83N\x83}\x83l\x81[\x83W\x83\x83:\x83\x8f\x81[\x83\x8b\x83h\x8c\xc5\x97L\x83""f\x81[\x83^\x82\xaa\x82\xa0\x82\xe8\x82\xdc\x82\xb9\x82\xf1:\x83""A\x83^\x83""b\x83N\x83J\x81[\x83h\x92\x8a\x91I\x83""f\x81[\x83^\n");
    }
    cmnGimData = CRsrcDB::getRsrc(1, "GimProb.bin", NAME_CASE_SENSITIVE);
    if (cmnGimData != NULL) {
        SHORT* p = (SHORT*)cmnGimData->getRsrcData();
        m_WorldGimNum = *p++;
        m_pGimParam = (GimParam*)com_malloc_d(m_WorldGimNum * sizeof(GimParam), __FILE__ "(642)", RET_ADDR());
        for (int i = 0; i < m_WorldGimNum; i++) {
            GimParam info;
            info.GimID = *p;
            p++;
            info.AtkPow = *(BYTE*)p;
            p = (SHORT*)((BYTE*)p + 1);
            info.AtkAttr = *(BYTE*)p;
            p = (SHORT*)((BYTE*)p + 1);
            info.Hp = *(BYTE*)p;
            p = (SHORT*)((BYTE*)p + 1);
            info.LunchType = *(BYTE*)p;
            p = (SHORT*)((BYTE*)p + 1);
            info.G_Index = *(BYTE*)p;
            p = (SHORT*)((BYTE*)p + 1);
            info.R_Index = *(BYTE*)p;
            p = (SHORT*)((BYTE*)p + 1);
            info.G_Prob = *(BYTE*)p;
            p = (SHORT*)((BYTE*)p + 1);
            info.R_Prob = *(BYTE*)p;
            p = (SHORT*)((BYTE*)p + 1);
            info.C_Prob = *(BYTE*)p;
            p = (SHORT*)((BYTE*)p + 1);
            info.B_Prob = *(BYTE*)p;
            p = (SHORT*)((BYTE*)p + 1);
            info.Riku_GIndex = *(BYTE*)p;
            p = (SHORT*)((BYTE*)p + 1);
            info.Riku_GProb = *(BYTE*)p;
            p = (SHORT*)((BYTE*)p + 1);
            m_pGimParam[i] = info;
        }
    } else {
        TRACE("\x83M\x83~\x83""b\x83N\x83}\x83l\x81[\x83W\x83\x83:\x83\x8f\x81[\x83\x8b\x83h\x8c\xc5\x97L\x83""f\x81[\x83^\x82\xaa\x82\xa0\x82\xe8\x82\xdc\x82\xb9\x82\xf1:\x83M\x83~\x83""b\x83N\x8e\xed\x95\xca\x96\x88\x82\xcc\x83""f\x81[\x83^\n");
    }
    return TRUE;
}

BOOL CGimmickManager::LoadCommonResrcMapCard() {
    CRsrcData* cmnGimData = CRsrcDB::getRsrc(1, "MapCardCom.bin", NAME_CASE_SENSITIVE);
    if (cmnGimData == NULL) {
        TRACE("\x83M\x83~\x83""b\x83N\x83}\x83l\x81[\x83W\x83\x83:\x91S\x83\x8f\x81[\x83\x8b\x83h\x8b\xa4\x92\xca\x83""f\x81[\x83^\x82\xaa\x82\xa0\x82\xe8\x82\xdc\x82\xb9\x82\xf1:\x83}\x83""b\x83v\x83J\x81[\x83h\x82\xcc\x90\x94\x92l\x92\x8a\x91I\x83""f\x81[\x83^\n");
        return FALSE;
    }
    BYTE* p = cmnGimData->getRsrcData();
    memcpy(m_MapCardProb.CardNumProb, p + 6, sizeof(m_MapCardProb.CardNumProb));
    return TRUE;
}

BOOL CGimmickManager::LoadWorldResrcMapCard() {
    CRsrcData* cmnGimData = CRsrcDB::getRsrc(1, "MapCardDrop.bin", NAME_CASE_SENSITIVE);
    if (cmnGimData == NULL) {
        TRACE("\x83M\x83~\x83""b\x83N\x83}\x83l\x81[\x83W\x83\x83:\x83\x8f\x81[\x83\x8b\x83h\x8c\xc5\x97L\x83""f\x81[\x83^\x82\xaa\x82\xa0\x82\xe8\x82\xdc\x82\xb9\x82\xf1:\x83}\x83""b\x83v\x83J\x81[\x83h\x82\xcc\x92\x8a\x91I\x83""f\x81[\x83^\n");
        return FALSE;
    }
    SHORT* p;
    p = (SHORT*)cmnGimData->getRsrcData();
    p = (SHORT*)((BYTE*)p + 6);
    if ((m_MapCardProb.ProbNum = *p++) != 0) {
        m_MapCardProb.pProb = (GimmickEveryCardProb*)com_malloc_d(
            m_MapCardProb.ProbNum * sizeof(GimmickEveryCardProb), __FILE__ "(737)", RET_ADDR());
        for (int i = 0; i < m_MapCardProb.ProbNum; i++) {
            m_MapCardProb.pProb[i].CardID = *p++;
            m_MapCardProb.pProb[i].Prob = *p++;
        }
        m_MapCardProb.JokerProb = *p++;
        m_MapCardProb.KeyProb = *p++;
    }
    LoadFloorPrizeDropTable();
    return TRUE;
}

BOOL CGimmickManager::LoadFloorPrizeDropTable() {
    CRsrcData* cmnGimData = CRsrcDB::getRsrc(1, "GimProbTable.bin", NAME_CASE_SENSITIVE);
    if (cmnGimData == NULL) {
        TRACE("\x83M\x83~\x83""b\x83N\x83}\x83l\x81[\x83W\x83\x83:\x83t\x83\x8d\x83""A\x8c\xc5\x97L\x83""f\x81[\x83^\x82\xaa\x82\xa0\x82\xe8\x82\xdc\x82\xb9\x82\xf1:\x92\x8a\x91I\x83""f\x81[\x83^\x83""e\x81[\x83u\x83\x8b\n");
        return FALSE;
    }
    BYTE* pB = cmnGimData->getRsrcData();
    m_GPrizeTableNum = *pB++;
    m_RPrizeTableNum = *pB++;
    m_pGBallDropTable = (BallDropTable*)com_malloc_d(m_GPrizeTableNum * sizeof(BallDropTable), __FILE__ "(783)", RET_ADDR());
    m_pRBallDropTable = (BallDropTable*)com_malloc_d(m_RPrizeTableNum * sizeof(BallDropTable), __FILE__ "(784)", RET_ADDR());
    for (int i = 0; i < m_GPrizeTableNum; i++) {
        for (int j = 0; j < 6; j++) {
            m_pGBallDropTable[i].type[j] = *pB++;
            m_pGBallDropTable[i].prob[j] = *pB++;
            m_pGBallDropTable[i].num[j] = *pB++;
        }
    }
    for (int i = 0; i < m_RPrizeTableNum; i++) {
        for (int j = 0; j < 6; j++) {
            m_pRBallDropTable[i].type[j] = *pB++;
            m_pRBallDropTable[i].prob[j] = *pB++;
            m_pRBallDropTable[i].num[j] = *pB++;
        }
    }
    return TRUE;
}

DropPrize CGimmickManager::DecideGimmickCard(INT DropProbNum, GimmickEveryCardProb* pDropDataTable, SHORT* pCardProbTable) {
    INT Prob = getRand() % 100;
    DropPrize ret;
    ret.StockKind = 0;
    ret.Kind = CARD_KIND_None;
    ret.Num = 0;
    ret.No = 0;
    for (INT i = 0; i < DropProbNum; i++) {
        if (Prob < pDropDataTable[i].Prob) {
            ret.Kind = (CARD_KIND)(pDropDataTable[i].CardID + 1);
            ret.Num = 1;
            ret.No = DecideGimmickCardNo(pCardProbTable);
            ret.StockKind = 0;
            break;
        }
        Prob -= pDropDataTable[i].Prob;
    }
    return ret;
}

DropPrize CGimmickManager::DecideGimmickCard_PossibleCheck(INT DropProbNum, GimmickEveryCardProb* pDropDataTable, SHORT* pCardProbTable) {
    INT Prob = getRand() % 100;
    DropPrize ret;
    TRACE("\x83J\x81[\x83h\x92\x8a\x91I\x90\x94\x92l(\x89\xf0) 0\x81`99 = %d\n", Prob);
    DebugWork::g_dw.gimDropProb.ProbRandomCardPossible = Prob;
    ret.StockKind = 0;
    ret.Kind = CARD_KIND_None;
    ret.Num = 0;
    ret.No = 0;
    for (INT i = 0; i < DropProbNum; i++) {
        switch (pDropDataTable[i].CardID) {
        case 0:
        case 1:
        case 2:
        case 3:
        case 4:
        case 5:
        case 8:
        case 10:
        case 11:
        case 26:
        case 28:
        case 41:
            break;
        default:
            if (Prob < pDropDataTable[i].Prob) {
                ret.Kind = (CARD_KIND)(pDropDataTable[i].CardID + 1);
                ret.Num = 1;
                ret.No = DecideGimmickCardNo(pCardProbTable);
                ret.StockKind = 0;
                goto found;
            }
            Prob -= pDropDataTable[i].Prob;
            TRACE("\x81@-\x8am\x97\xa6(%d)\x81@ = %d\n", pDropDataTable[i].Prob, Prob);
            break;
        }
    }
found:
    if (ret.Kind == CARD_KIND_None) {
        sprintf(DebugWork::g_dw.gimDropProb.PossibleCardName, "%s", cardDataTbl[CARD_KIND_None].pName);
    } else {
        sprintf(DebugWork::g_dw.gimDropProb.PossibleCardName, "\x82\xcd\x82\xb8\x82\xea");
    }
    return ret;
}

DropPrize CGimmickManager::DecideGimmickCard_PossibleNoCheck(INT DropProbNum, GimmickEveryCardProb* pDropDataTable, SHORT* pCardProbTable) {
    INT breakFlag = 0;
    INT Prob = getRand() % 100;
    DropPrize ret;
    TRACE("\x83J\x81[\x83h\x92\x8a\x91I\x90\x94\x92l 0\x81`99 = %d\n", Prob);
    DebugWork::g_dw.gimDropProb.ProbRandomCard = Prob;
    ret.StockKind = 0;
    ret.Kind = CARD_KIND_None;
    ret.Num = 0;
    ret.No = 0;
    for (INT i = 0; i < DropProbNum; i++) {
        switch (pDropDataTable[i].CardID) {
        case 0:
        case 1:
        case 2:
        case 3:
        case 4:
        case 5:
        case 8:
        case 10:
        case 11:
        case 26:
        case 28:
        case 41:
            if (Prob < pDropDataTable[i].Prob) {
                ret.Kind = (CARD_KIND)(pDropDataTable[i].CardID + 1);
                ret.Num = 1;
                ret.No = DecideGimmickCardNo(pCardProbTable);
                breakFlag = 1;
                ret.StockKind = 0;
            } else {
                Prob -= pDropDataTable[i].Prob;
                TRACE("\x81@-\x8am\x97\xa6(%d)\x81@ = %d\n", pDropDataTable[i].Prob, Prob);
            }
            break;
        }
        if (breakFlag) break;
    }
    sprintf(DebugWork::g_dw.gimDropProb.CardName, "%s", cardDataTbl[ret.Kind].pName);
    return ret;
}

DropPrize CGimmickManager::DecideMapCard(INT DropProbNum, GimmickEveryCardProb* pDropDataTable, SHORT* pCardProbTable) {
    DropPrize ret;
    ret.StockKind = 0;
    ret.Kind = CARD_KIND_None;
    ret.Num = 0;
    ret.No = 0;
    if (CGameWork::getFlag(1, 7) == TRUE) {
        ret.Kind = (CARD_KIND)0xA1;
        ret.No = 2;
        ret.StockKind = 0;
        return ret;
    }
    INT Prob = getRand() % 100;
    TRACE("\x83}\x83""b\x83v\x83J\x81[\x83h\x92\x8a\x91I\x90\x94\x92l 0\x81`99 = %d\n", Prob);
    for (INT i = 0; i < DropProbNum; i++) {
        if (Prob < pDropDataTable[i].Prob) {
            ret.Kind = (CARD_KIND)(pDropDataTable[i].CardID + 0xA1);
            ret.Num = 1;
            ret.No = DecideGimmickCardNo(pCardProbTable);
            ret.StockKind = 0;
            break;
        }
        Prob -= pDropDataTable[i].Prob;
        TRACE("\x81@-\x8am\x97\xa6(%d)\x81@ = %d\n", pDropDataTable[i].Prob, Prob);
    }
    if (CGame::isSoraGame()) {
        if (CGameWork::getInt(0, 0x35) == 1) {
            INT Number = CSceneInfo::getRoomInfo(-1)->getCardNumber();
            if (Number == 9) {
                Number = 0;
            } else {
                Number++;
            }
            if (!CCard::isHaveMapCard(Number)) {
                TRACE("\x90i\x8ds\x8f\xe3\x82\xcc\x82\xcd\x82\xdc\x82\xe8\x89\xf1\x94\xf0\x81I\x81I\x83J\x81[\x83h\x83i\x83\x93\x83o\x81[\x8d\xb7\x82\xb5\x91\xd6\x82\xa6\x81@%d\x81@\x81\xa8\x81@0 \n", ret.No);
                ret.No = 0;
            }
        }
    } else {
        if (CGameWork::getInt(0, 0x35) == -12) {
            INT Number = CSceneInfo::getRoomInfo(-1)->getCardNumber();
            if (Number == 9) {
                Number = 0;
            } else {
                Number++;
            }
            if (!CCard::isHaveMapCard(Number)) {
                TRACE("\x90i\x8ds\x8f\xe3\x82\xcc\x82\xcd\x82\xdc\x82\xe8\x89\xf1\x94\xf0\x81I\x81I\x83J\x81[\x83h\x83i\x83\x93\x83o\x81[\x8d\xb7\x82\xb5\x91\xd6\x82\xa6\x81@%d\x81@\x81\xa8\x81@0 \n", ret.No);
                ret.No = 0;
            }
        }
    }
    ret = checkAddCardMapCard(ret);
    return ret;
}

INT CGimmickManager::DecideGimmickCardNo(SHORT* pCardProbTable) {
    INT Prob = getRand() % 100;
    TRACE("\x94\xd4\x8d\x86\x92\x8a\x91I\x90\x94\x92l(\x89\xf0) 0\x81`99 = %d\n", Prob);
    for (INT i = 0; i < 10; i++) {
        if (Prob < pCardProbTable[i]) return i;
        Prob -= pCardProbTable[i];
        TRACE("\x81@-\x8am\x97\xa6(%d)\x81@ = %d\n", pCardProbTable[i], Prob);
    }
    TRACE("\x83M\x83~\x83""b\x83N\x83}\x83l\x81[\x83W\x83\x83::\x83J\x81[\x83h\x82\xcc\x90\x94\x92l\x92\x8a\x91I\x82\xc9\x8e\xb8\x94s\x81I\x81I\x81I\n");
    return -1;
}

DropPrize CGimmickManager::DecideTreasurePrize() {
    DropPrize ret;
    ret.Num = 0;
    ret.No = 0;
    ret.Kind = CARD_KIND_None;
    ret.StockKind = 0;
    if (m_pTreasureCardDropProbTable == NULL) {
        TRACE("\x83M\x83~\x83""b\x83N\x83}\x83l\x81[\x83W\x83\x83\x81""F\x95\xf3\x94\xa0\x82\xcc\x92\x86\x90g\x82\xcc\x92\x8a\x91I\x83""f\x81[\x83^\x83""e\x81[\x83u\x83\x8b\x82\xaa\x82\xa0\x82\xe8\x82\xdc\x82\xb9\x82\xf1\x81@\x83""f\x83t\x83H\x83\x8b\x83g\x8fo\x82\xb5\x82\xdc\x82\xb7\x81""B");
        ret.Kind = (CARD_KIND)0xF;
        ret.No = 9;
        ret.Num = 1;
        ret.StockKind = 0;
        return ret;
    }
    if (CSceneInfo::getCurrentSceneNum() == 24) {
        ret = DecideKeyTreasurePrize();
        if (ret.Kind != CARD_KIND_None || ret.StockKind != 0) {
            TRACE("\x8aJ\x82\xaf\x82\xbd\x89\xf1\x90\x94\x82\xc6\x82\xa9\x8f\xf0\x8c\x8f\x95t\x83L\x81[\x83J\x81[\x83h\x95\xf3\x94\xa0\n");
            CGameWork::progMCW()._KeyRoomsTresureBoxCounter[CSceneInfo::getCurrentWorldID(0)]++;
        }
    } else {
        ret = DecideOpenCountTreasurePrize();
        if (ret.Kind != CARD_KIND_None || ret.StockKind != 0) {
            TRACE("\x8aJ\x82\xaf\x82\xbd\x89\xf1\x90\x94\x82\xc6\x82\xa9\x8f\xf0\x8c\x8f\x95t\x95\xf3\n");
            if (ret.Kind != 0x7D && ret.Kind != 0x7E) {
                CGameWork::progMCW()._OpenTresureBoxCounter[CSceneInfo::getCurrentWorldID(0)]++;
            } else {
                TRACE("\x83S\x81[\x83\x8b\x83h\x82\xc6\x83v\x83\x89\x83`\x83i\x82\xcd\x82\xa0\x82\xaf\x82\xbd\x89\xf1\x90\x94\x83J\x83""E\x83\x93\x83g\x82\xc9\x8a\xdc\x82\xdc\x82\xc8\x82\xa2\n");
            }
        }
    }
    if (ret.Kind == CARD_KIND_None && ret.StockKind == 0) {
        TRACE("\x8f\xf0\x8c\x8f\x95t\x81""A\x83L\x81[\x95\xf3\x82\xc5\x88\xc8\x8aO\x81""F\x95\x81\x92\xca\x82\xcc\x95\xf3\x94\xa0\x82\xa9\x82\xe7\x92\x8a\x91I\n");
        ret = DecideGimmickCard_PossibleCheck(m_TreasureCardDropProbNum, m_pTreasureCardDropProbTable, m_CardProbNum);
        if (!checkPossibleCard(ret.Kind)) {
            ret = DecideGimmickCard_PossibleNoCheck(m_TreasureCardDropProbNum, m_pTreasureCardDropProbTable, m_CardProbNum);
        }
    }
    ret = checkAddCardTresureBox(ret);
    return ret;
}

#define KEY_TREASURE_CHECK()                                                                                          \
    if (CGameWork::getByte(0, 0x61) == 3) {                                                                          \
        ret = DecideGimmickCard_PossibleCheck(m_TreasureCardDropProbNum, m_pTreasureCardDropProbTable, m_CardProbNum); \
        if (ret.Kind == CARD_KIND_None) {                                                                            \
            ret = DecideGimmickCard_PossibleNoCheck(m_TreasureCardDropProbNum, m_pTreasureCardDropProbTable,         \
                                                    m_CardProbNum);                                                  \
        }                                                                                                            \
    }

#define KEY_CARD(kind)                                                                                                \
    if (CGameWork::getFlag(0, 0x11)) {                                                                               \
        ret.Kind = (CARD_KIND)(kind);                                                                                \
        CGameWork::progMCW()._KeyRoomsTresureBox[CSceneInfo::getCurrentWorldID(0)] = count;                          \
    }

DropPrize CGimmickManager::DecideKeyTreasurePrize() {
    DropPrize ret;
    INT count;
    TRACE("\x89""B\x82\xb5\x95\x94\x89\xae\x95\xf3\n");
    ret.StockKind = 0;
    ret.Kind = CARD_KIND_None;
    ret.Num = 0;
    ret.No = 0;
    count = CGameWork::progMCW()._KeyRoomsTresureBoxCounter[CSceneInfo::getCurrentWorldID(0)];
    ret.Kind = CARD_KIND_None;
    ret.StockKind = 0;
    switch (CSceneInfo::getCurrentWorldID(0)) {
    case 0:
        break;
    case 1:
        switch (count) {
        case 0:
                ret.Kind = (CARD_KIND)0xA;
            break;
        case 1:
                KEY_CARD(0x79);
            break;
        }
        break;
    case 2:
        switch (count) {
        case 0:
                KEY_TREASURE_CHECK() else ret.StockKind = 0x2C;
            break;
        case 1:
                KEY_CARD(0x7B);
            break;
        }
        break;
    case 3:
        switch (count) {
        case 0:
                ret.Kind = (CARD_KIND)0x8;
            break;
        case 1:
                KEY_CARD(0x14);
            break;
        }
        break;
    case 4:
        switch (count) {
        case 0:
                KEY_TREASURE_CHECK() else ret.StockKind = 0x30;
            break;
        case 1:
                KEY_CARD(0x76);
            break;
        }
        break;
    case 5:
        switch (count) {
        case 0:
                KEY_TREASURE_CHECK() else ret.StockKind = 0x25;
            break;
        case 1:
                KEY_CARD(0x78);
            break;
        }
        break;
    case 6:
        switch (count) {
        case 0:
                KEY_TREASURE_CHECK() else ret.StockKind = 0x2D;
            break;
        case 1:
                KEY_CARD(0x18);
            break;
        }
        break;
    case 7:
        switch (count) {
        case 0:
                KEY_TREASURE_CHECK() else ret.StockKind = 0x2A;
            break;
        case 1:
                KEY_CARD(0x7A);
            break;
        }
        break;
    case 8:
        switch (count) {
        case 0:
                KEY_TREASURE_CHECK() else ret.StockKind = 0x1D;
            break;
        case 1:
                KEY_CARD(0x15);
            break;
        }
        break;
    case 9:
        switch (count) {
        case 0:
                ret.Kind = (CARD_KIND)0x28;
            break;
        case 1:
                KEY_CARD(0x77);
            break;
        }
        break;
    case 10:
        break;
    case 11:
        switch (count) {
        case 0:
                KEY_TREASURE_CHECK() else ret.StockKind = 0x46;
            break;
        case 1:
                KEY_CARD(0x7C);
            break;
        }
        break;
    case 12:
        switch (count) {
        case 0:
                ret.Kind = (CARD_KIND)0x30;
            break;
        case 1:
                KEY_CARD(0x17);
            break;
        }
        break;
    case 13:
        switch (count) {
        case 0:
                KEY_TREASURE_CHECK() else {
                    CGameWork::setInt(0, 0x24, CGameWork::getInt(0, 0x24) | 4);
                    ret.StockKind = 0x5C;
                }
            break;
        case 1:
                KEY_CARD(0x13);
            break;
        }
        break;
    }
    ret.No = DecideGimmickCardNo(m_CardProbNum);
    ret.Num = 1;
    return ret;
}

#define OPEN_TREASURE_CHECK(stock)                                                                                    \
    if (CGameWork::getByte(0, 0x61) == 3) {                                                                          \
        ret = DecideGimmickCard_PossibleCheck(m_TreasureCardDropProbNum, m_pTreasureCardDropProbTable, m_CardProbNum); \
        if (ret.Kind == CARD_KIND_None) {                                                                            \
            ret = DecideGimmickCard_PossibleNoCheck(m_TreasureCardDropProbNum, m_pTreasureCardDropProbTable,         \
                                                    m_CardProbNum);                                                  \
        }                                                                                                            \
    } else {                                                                                                         \
        ret.StockKind = (stock);                                                                                     \
    }

#define OPEN_FLAG_CARD(kind)                                                                                          \
    if (CGameWork::getFlag(0, 0x12) && CGameWork::getFlag(0, 0x82)) {                                                \
        ret.Kind = (CARD_KIND)(kind);                                                                                \
    }

DropPrize CGimmickManager::DecideOpenCountTreasurePrize() {
    INT count = CGameWork::progMCW()._OpenTresureBoxCounter[CSceneInfo::getCurrentWorldID(0)];
    DropPrize ret;
    ret.Num = 0;
    ret.No = 0;
    ret.Kind = CARD_KIND_None;
    ret.StockKind = 0;
    if (CJiminyMemoFlag::getContentOpenPercentage(CGameWork::getByte(0, 0x60)) == 100) {
        if (!(CGameWork::getInt(0, 0x2D) & 0x2000)) {
            ret.Kind = (CARD_KIND)0x7D;
        } else if (!(CGameWork::getInt(0, 0x2D) & 0x4000)) {
            ret.Kind = (CARD_KIND)0x7E;
        }
        if (ret.Kind != CARD_KIND_None) {
            ret.No = 0;
            ret.Num = 1;
            return ret;
        }
    }
    switch (CSceneInfo::getCurrentWorldID(0)) {
    case 0:
        break;
    case 1:
        if (count == 0) {
            if (CGameWork::getFlag(0, 0x11)) ret.Kind = (CARD_KIND)0x16;
        }
        break;
    case 2:
        if (count == 0) ret.Kind = (CARD_KIND)0x1E;
        break;
    case 3:
        if (count == 0) {
            OPEN_TREASURE_CHECK(0x1C);
        }
        break;
    case 4:
        if (count == 0) ret.Kind = (CARD_KIND)0x1F;
        break;
    case 5:
        if (count == 0) {
            OPEN_TREASURE_CHECK(0x1B);
        }
        break;
    case 6:
        if (count == 0) {
            OPEN_TREASURE_CHECK(0x31);
        }
        break;
    case 7:
        switch (count) {
        case 0:
            OPEN_TREASURE_CHECK(0x28);
            break;
        case 1:
            OPEN_TREASURE_CHECK(0x24);
            break;
        }
        break;
    case 8:
        if (count == 0) {
            OPEN_TREASURE_CHECK(0x32);
        }
        break;
    case 9:
        if (count == 0) {
            OPEN_TREASURE_CHECK(0x1E);
        }
        break;
    case 10:
        if (count == 0) ret.Kind = (CARD_KIND)0x7;
        break;
    case 11:
        switch (count) {
        case 0:
            OPEN_TREASURE_CHECK(0x2B);
            break;
        case 1:
            OPEN_FLAG_CARD(0x74);
            break;
        }
        break;
    case 12:
        switch (count) {
        case 0:
            OPEN_TREASURE_CHECK(0x1F);
            break;
        case 1:
            OPEN_FLAG_CARD(0x75);
            break;
        }
        break;
    case 13:
        switch (count) {
        case 0:
            OPEN_TREASURE_CHECK(0x21);
            break;
        case 1:
            OPEN_FLAG_CARD(0xF);
            break;
        case 2:
            OPEN_FLAG_CARD(0x73);
            break;
        }
        break;
    }
    ret.No = DecideGimmickCardNo(m_CardProbNum);
    ret.Num = 1;
    return ret;
}

BOOL CGimmickManager::checkPossibleCard(CARD_KIND in_Kind) {
    INT ret = TRUE;
    switch (in_Kind) {
    case 0:
        ret = FALSE;
        break;
    case 7:
        if (!(CGameWork::getInt(0, 0x21) & 0x1)) ret = FALSE;
        break;
    case 8:
        if (!(CGameWork::getInt(0, 0x21) & 0x2)) ret = FALSE;
        break;
    case 10:
        if (!(CGameWork::getInt(0, 0x21) & 0x4)) ret = FALSE;
        break;
    case 13:
        if (!(CGameWork::getInt(0, 0x21) & 0x8)) ret = FALSE;
        break;
    case 14:
        if (!(CGameWork::getInt(0, 0x21) & 0x10)) ret = FALSE;
        break;
    case 15:
        if (!(CGameWork::getInt(0, 0x21) & 0x20)) ret = FALSE;
        break;
    case 16:
        if (!(CGameWork::getInt(0, 0x21) & 0x40)) ret = FALSE;
        break;
    case 17:
        if (!(CGameWork::getInt(0, 0x21) & 0x80)) ret = FALSE;
        break;
    case 26:
        if (!(CGameWork::getInt(0, 0x21) & 0x100)) ret = FALSE;
        break;
    case 28:
        if (!(CGameWork::getInt(0, 0x21) & 0x200)) ret = FALSE;
        break;
    case 30:
        if (!(CGameWork::getInt(0, 0x21) & 0x400)) ret = FALSE;
        break;
    case 31:
        if (!(CGameWork::getInt(0, 0x21) & 0x800)) ret = FALSE;
        break;
    case 32:
        if (!(CGameWork::getInt(0, 0x21) & 0x1000)) ret = FALSE;
        break;
    case 35:
        if (!(CGameWork::getInt(0, 0x21) & 0x2000)) ret = FALSE;
        break;
    case 36:
        if (!(CGameWork::getInt(0, 0x21) & 0x4000)) ret = FALSE;
        break;
    case 37:
        if (!(CGameWork::getInt(0, 0x21) & 0x8000)) ret = FALSE;
        break;
    case 38:
        if (!(CGameWork::getInt(0, 0x21) & 0x10000)) ret = FALSE;
        break;
    case 39:
        if (!(CGameWork::getInt(0, 0x21) & 0x20000)) ret = FALSE;
        break;
    case 40:
        if (!(CGameWork::getInt(0, 0x21) & 0x40000)) ret = FALSE;
        break;
    case 41:
        if (!(CGameWork::getInt(0, 0x21) & 0x80000)) ret = FALSE;
        break;
    case 43:
        if (!(CGameWork::getInt(0, 0x21) & 0x100000)) ret = FALSE;
        break;
    case 44:
        if (!(CGameWork::getInt(0, 0x21) & 0x200000)) ret = FALSE;
        break;
    case 45:
        if (!(CGameWork::getInt(0, 0x21) & 0x400000)) ret = FALSE;
        break;
    case 46:
        if (!(CGameWork::getInt(0, 0x21) & 0x800000)) ret = FALSE;
        break;
    case 47:
        if (!(CGameWork::getInt(0, 0x21) & 0x1000000)) ret = FALSE;
        break;
    case 48:
        if (!(CGameWork::getInt(0, 0x21) & 0x2000000)) ret = FALSE;
        break;
    case 19:
        if (!(CGameWork::getInt(0, 0x2D) & 0x80)) ret = FALSE;
        break;
    case 20:
        if (!(CGameWork::getInt(0, 0x2D) & 0x100)) ret = FALSE;
        break;
    case 21:
        if (!(CGameWork::getInt(0, 0x2D) & 0x200)) ret = FALSE;
        break;
    case 22:
        if (!(CGameWork::getInt(0, 0x2D) & 0x400)) ret = FALSE;
        break;
    case 23:
        if (!(CGameWork::getInt(0, 0x2D) & 0x800)) ret = FALSE;
        break;
    case 24:
        if (!(CGameWork::getInt(0, 0x2D) & 0x1000)) ret = FALSE;
        break;
    }
    if (!ret) {
        TRACE("\x82\xdc\x82\xbe\x89\xf0\x8b\xd6\x82\xb3\x82\xea\x82\xc4\x82\xa2\x82\xc8\x82\xa2\n");
    }
    return ret;
}

CARD_KIND CGimmickManager::changePossibleCard(CARD_KIND in_Kind) {
    switch (in_Kind) {
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
    case 14:
    case 15:
    case 16:
    case 17:
    case 19:
    case 20:
    case 21:
    case 22:
    case 23:
    case 24:
        TRACE("\x83""A\x83^\x83""b\x83N\x83J\x81[\x83h\x82\xf0\x95\xcf\x8dX\x81""F\x83L\x83\x93\x83O\x83_\x83\x80\x83`\x83""F\x81[\x83\x93\x82\xd6\n");
        return (CARD_KIND)1;
    case 26:
    case 27:
    case 28:
    case 30:
    case 31:
    case 32:
    case 35:
    case 36:
    case 37:
    case 38:
    case 39:
    case 40:
    case 41:
        TRACE("\x83}\x83W\x83""b\x83N\x83J\x81[\x83h\x82\xf0\x95\xcf\x8dX\x81""F\x83P\x83""A\x83\x8b\x82\xd6\n");
        return (CARD_KIND)0x1D;
    case 43:
    case 44:
    case 45:
    case 46:
    case 47:
    case 48:
        TRACE("\x83""A\x83""C\x83""e\x83\x80\x83J\x81[\x83h\x82\xf0\x95\xcf\x8dX\x81""F\x83|\x81[\x83V\x83\x87\x83\x93\x82\xd6\n");
        return (CARD_KIND)0x2A;
    }
    TRACE("\x92\x8a\x91I\x83G\x83\x89\x81[\x81@\x95\xcf\x82\xc8\x82\xe0\x82\xcc\x92\x8a\x91I%d\n", in_Kind);
    return (CARD_KIND)1;
}

DropPrize CGimmickManager::DecideMapCardPrize(BOOL in_enableKeyCard) {
    DropPrize ret;
    ret.StockKind = 0;
    ret.Kind = CARD_KIND_None;
    ret.Num = 0;
    ret.No = 0;
    INT Prob = getRand() % 10000;
    if (m_MapCardProb.pProb == NULL) {
        TRACE("\x83}\x83""b\x83v\x83J\x81[\x83h\x92\x8a\x91I\x83""f\x81[\x83^\x83""e\x81[\x83u\x83\x8b\x82\xaa\x82\xa0\x82\xe8\x82\xdc\x82\xb9\x82\xf1\n");
        ret.Kind = CARD_KIND_None;
        ret.No = 0;
        ret.Num = 0;
        ret.StockKind = 0;
        return ret;
    }
    ret.Num = 1;
    if (in_enableKeyCard == TRUE) {
        FLOAT JokerProd = 1.0f;
        switch (CGameWork::getByte(0, 0x61)) {
        case 1:
            JokerProd = 3.0f;
            break;
        case 2:
            JokerProd = 6.0f;
            break;
        }
        TRACE("\x83W\x83\x87\x81[\x83J\x81[\x81""FProb = %d, DataProb = %d\n", Prob,
              (INT)(m_MapCardProb.JokerProb / JokerProd * 100.0f));
        if (Prob < (INT)(m_MapCardProb.JokerProb / JokerProd * 100.0f)) {
            ret.Kind = (CARD_KIND)0xB9;
            ret.No = 0;
            ret.StockKind = 0;
            ret = checkAddCardMapCard(ret);
            GimDebugTrace(&ret);
            return ret;
        }
    }
    if (CCard::searchCard(0xBD, 0) == 0 && in_enableKeyCard == TRUE) {
        STORAGE::MCWork& w = CGameWork::progMCW();
        if (!w._KeyRoomsTresureBox[1] || !w._KeyRoomsTresureBox[2] || !w._KeyRoomsTresureBox[3] ||
            !w._KeyRoomsTresureBox[4] || !w._KeyRoomsTresureBox[5] || !w._KeyRoomsTresureBox[6] ||
            !w._KeyRoomsTresureBox[7] || !w._KeyRoomsTresureBox[8] || !w._KeyRoomsTresureBox[9] ||
            !w._KeyRoomsTresureBox[11] || !w._KeyRoomsTresureBox[12] || !w._KeyRoomsTresureBox[13]) {
            Prob = getRand() % 100;
            TRACE("\x83L\x81[\x83J\x81[\x83h\x81""FProb = %d, DataProb %d\n", Prob, m_MapCardProb.KeyProb);
            if (Prob < m_MapCardProb.KeyProb) {
                ret.Kind = (CARD_KIND)0xBD;
                ret.No = 0;
                ret.StockKind = 0;
                GimDebugTrace(&ret);
                return ret;
            }
        }
    }
    ret = DecideMapCard(m_MapCardProb.ProbNum, m_MapCardProb.pProb, m_MapCardProb.CardNumProb);
    GimDebugTrace(&ret);
    return ret;
}

GimParam* CGimmickManager::getGimParam(INT in_GimmickID) {
    if (m_pGimParam == NULL) return NULL;
    INT id = (UINT)in_GimmickID % 10000;
    for (int i = 0; i < m_WorldGimNum; i++) {
        if (m_pGimParam[i].GimID == id) return &m_pGimParam[i];
    }
    TRACE("\x8ew\x92\xe8\x82\xb3\x82\xea\x82\xbd\x83M\x83~\x83""b\x83N\x82h\x82""c\x82\xaa\x8c\xa9\x82\xc2\x82\xa9\x82\xe8\x82\xdc\x82\xb9\x82\xf1\x81""F%d\n", in_GimmickID);
    return NULL;
}

INT CGimmickManager::DecidePrize(INT in_GimmickID, const FVECTOR* Pos, const FVECTOR* Rot, INT NotCardDropFlag) {
    DropPrize Prize;
    GimParam* param;
    INT ret;
    Prize.StockKind = 0;
    Prize.Kind = CARD_KIND_None;
    Prize.Num = 0;
    Prize.No = 0;
    param = getGimParam(in_GimmickID);
    memset(&DebugWork::g_dw.gimDropProb, 0, sizeof(DebugWork::g_dw.gimDropProb));
    if ((CSceneInfo::getCurrentWorldID(0) >= 14 && CSceneInfo::getCurrentWorldID(0) != 23) || param == NULL) {
        Prize.Kind = (CARD_KIND)0xD2;
        Prize.Num = 5;
        createPrize(Prize, 0, Pos, Rot);
        return 0;
    }
    ret = DecidePrizeKind(param);
    DebugWork::g_dw.gimDropProb.ProbCategoryKind = ret;
    if (DebugWork::g_dw.gimPrize.Kind) ret = DebugWork::g_dw.gimPrize.Kind;
    switch (ret) {
    case 0:
        TRACE("\x92\x8a\x91I\x8c\x8b\x89\xca\x81""F\x82\xcd\x82\xb8\x82\xea\n");
        return 0;
    case 1:
        TRACE("\x92\x8a\x91I\x8c\x8b\x89\xca\x81""F\x82g\x82o\x83{\x81[\x83\x8b\n");
        Prize = DecideGPrize(param->G_Index);
        createPrize(Prize, param->LunchType, Pos, Rot);
        break;
    case 2:
        TRACE("\x92\x8a\x91I\x8c\x8b\x89\xca\x81""F\x83\x82\x81[\x83O\x83\x8a\x83|\x83""C\x83\x93\x83g\n");
        Prize = DecideRPrize(param->R_Index);
        createPrize(Prize, param->LunchType, Pos, Rot);
        break;
    case 3:
        if (NotCardDropFlag == 1) return 0;
        if (CGameWork::getFlag(1, 7)) return 0;
        TRACE("\x92\x8a\x91I\x8c\x8b\x89\xca\x81""F\x83J\x81[\x83h\n");
        Prize = DecideGimmickCard_PossibleCheck(m_NomalCardDropProbNum, m_pNomalCardDropProbTable, m_CardProbNum);
        if (!checkPossibleCard(Prize.Kind)) {
            DebugWork::g_dw.gimDropProb.PossibleCheck = 1;
            Prize = DecideGimmickCard_PossibleNoCheck(m_NomalCardDropProbNum, m_pNomalCardDropProbTable, m_CardProbNum);
        }
        Prize = checkAddCardGimmick(Prize);
        if (Prize.Kind == CARD_KIND_None) {
            TRACE("\x92\x8a\x91I\x8c\x8b\x89\xca\x82\xcc\x83J\x81[\x83h\x82\xaa\x93\xfc\x8e\xe8\x95s\x89\xc2\x94\\\x82\xc8\x88\xd7\x89\xbd\x82\xe0\x82\xbe\x82\xb3\x82\xc8\x82\xa2\n");
            return 0;
        }
        createPrize(Prize, param->LunchType, Pos, Rot);
        break;
    case 4:
        TRACE("\x92\x8a\x91I\x8c\x8b\x89\xca\x81""F\x83o\x83\x8c\x83\x8b\x95\xcf\x8c`\n");
        return 4;
    default:
    TRACE("\x92\x8a\x91I\x8c\x8b\x89\xca\x81""F\x92v\x96\xbd\x93I\x82\xc8\x83G\x83\x89\x81[\n");
        break;
    }
    return ret;
}

INT CGimmickManager::DecidePrizeKind(const GimParam* in_pParam) {
    INT Prob = getRand() % 100;
    _GimmickRoomInfo* gim;
    DebugWork::g_dw.gimDropProb.ProbCard = in_pParam->C_Prob;
    DebugWork::g_dw.gimDropProb.ProbSoraGBall = in_pParam->G_Prob;
    DebugWork::g_dw.gimDropProb.ProbSoraRBall = in_pParam->R_Prob;
    DebugWork::g_dw.gimDropProb.ProbSoraBarel = in_pParam->B_Prob;
    DebugWork::g_dw.gimDropProb.ProbRikuGball = in_pParam->Riku_GProb;
    DebugWork::g_dw.gimDropProb.ProbRikuBarel = in_pParam->B_Prob;
    DebugWork::g_dw.gimDropProb.ProbRandomCategory = Prob;
    if (CGameWork::getByte(0, 0x60) == 0) {
        if (Prob < in_pParam->C_Prob) {
            gim = CSceneInfo::getCurrentRoomGimmickInfo();
            if (gim != NULL &&
                CSceneInfo::getCurrentSceneNum() == m_CardDropRoomMaxTable[CSceneInfo::getCurrentSceneNum()].RoomID &&
                gim->_cardCount < m_CardDropRoomMaxTable[CSceneInfo::getCurrentSceneNum()].MaxNum) {
                gim->_cardCount++;
                return 3;
            }
        }
        Prob -= in_pParam->C_Prob;
        if (Prob < in_pParam->G_Prob) return 1;
        Prob -= in_pParam->G_Prob;
        if (Prob < in_pParam->R_Prob) return 2;
        Prob -= in_pParam->R_Prob;
        if (Prob < in_pParam->B_Prob) return 4;
    } else {
        if (Prob < in_pParam->Riku_GProb) return 1;
        Prob -= in_pParam->Riku_GProb;
        if (Prob < in_pParam->B_Prob) return 4;
    }
    return 0;
}

DropPrize CGimmickManager::DecideGPrize(INT in_DataIndex) {
    DropPrize ret;
    INT Prob;
    ret.StockKind = 0;
    ret.Kind = CARD_KIND_None;
    ret.Num = 0;
    ret.No = 0;
    if (m_pGBallDropTable == NULL) {
        TRACE("\x97\xce\x83{\x81[\x83\x8b\x92\x8a\x91I\x83""e\x81[\x83u\x83\x8b\x82\xaa\x82\xa0\x82\xe8\x82\xdc\x82\xb9\x82\xf1\x81I\x81I\x81I\x8c\xc5\x92\xe8\x92l\x95\xd4\x82\xb5\x82\xdc\x82\xb7\n");
        ret.Kind = (CARD_KIND)0xD2;
        ret.Num = 3;
        return ret;
    }
    Prob = getRand() % 100;
    DebugWork::g_dw.gimDropProb.ProbRandomGBall = Prob;
    for (int i = 0; i < 6; i++) {
        if (Prob < m_pGBallDropTable[in_DataIndex].prob[i]) {
            if (m_pGBallDropTable[in_DataIndex].type[i]) {
                ret.Kind = (CARD_KIND)0xD2;
            } else {
                ret.Kind = (CARD_KIND)0xD3;
            }
            ret.Num = m_pGBallDropTable[in_DataIndex].num[i];
            DebugWork::g_dw.gimDropProb.GBallType = ret.Kind - 0xD1;
            DebugWork::g_dw.gimDropProb.GBallNum = ret.Num;
            return ret;
        }
        Prob -= m_pGBallDropTable[in_DataIndex].prob[i];
    }
    TRACE("\x97\xce\x83{\x81[\x83\x8b\x92\x8a\x91I\x83G\x83\x89\x81[\x81""F\x89\xbd\x82\xe0\x91I\x82\xce\x82\xc8\x82\xa9\x82\xc1\x82\xbd\x81I\x81I\n");
    return ret;
}

DropPrize CGimmickManager::DecideRPrize(INT in_DataIndex) {
    DropPrize ret;
    INT Prob;
    ret.StockKind = 0;
    ret.Kind = CARD_KIND_None;
    ret.Num = 0;
    ret.No = 0;
    if (m_pGBallDropTable == NULL) { /* sic: the original checks the G table */
        TRACE("\x90\xd4\x83{\x81[\x83\x8b\x92\x8a\x91I\x83""e\x81[\x83u\x83\x8b\x82\xaa\x82\xa0\x82\xe8\x82\xdc\x82\xb9\x82\xf1\x81I\x81I\x81I\x8c\xc5\x92\xe8\x92l\x95\xd4\x82\xb5\x82\xdc\x82\xb7\n");
        ret.Kind = (CARD_KIND)0xD2;
        ret.Num = 3;
        return ret;
    }
    Prob = getRand() % 100;
    DebugWork::g_dw.gimDropProb.ProbRandomRBall = Prob;
    for (int i = 0; i < 6; i++) {
        if (Prob < m_pRBallDropTable[in_DataIndex].prob[i]) {
            if (m_pRBallDropTable[in_DataIndex].type[i]) {
                ret.Kind = (CARD_KIND)0xD4;
            } else {
                ret.Kind = (CARD_KIND)0xD5;
            }
            ret.Num = m_pRBallDropTable[in_DataIndex].num[i];
            DebugWork::g_dw.gimDropProb.RBallType = ret.Kind - 0xD3;
            DebugWork::g_dw.gimDropProb.RBallNum = ret.Num;
            return ret;
        }
        Prob -= m_pRBallDropTable[in_DataIndex].prob[i];
    }
    TRACE("\x90\xd4\x83{\x81[\x83\x8b\x92\x8a\x91I\x83G\x83\x89\x81[\x81""F\x89\xbd\x82\xe0\x91I\x82\xce\x82\xc8\x82\xa9\x82\xc1\x82\xbd\x81I\x81I\n");
    return ret;
}

CEntity* CGimmickManager::getGimClassEntityFirst(INT in_ClassType) {
    SLContainerT<CTask>* pContainer;
    for (pContainer = getChildren().getHead(); pContainer != NULL; pContainer = pContainer->getNext()) {
        CTask* task = pContainer->getValue();
        if (task->getTypeID() == in_ClassType) return (CEntity*)task;
    }
    return NULL;
}

BOOL CGimmickManager::getSavePointPos(FVECTOR& outPos) {
    CGimmickSavePoint* pSave = (CGimmickSavePoint*)getGimClassEntityFirst(0x2004);
    if (pSave != NULL) {
        outPos = pSave->getPos();
        return TRUE;
    }
    return FALSE;
}

void CGimmickManager::createPrize(const DropPrize& in_Prize, INT in_Style, const FVECTOR* in_pPos, const FVECTOR* in_pRot) {
    GimDebugTrace(&in_Prize);
    if (in_Prize.Kind == CARD_KIND_None) return;
    switch (in_Style) {
    case 0:
    case 1:
        /* not in the original: an extra use of this, so that it gets $s0 before in_Prize (as in the original) */
        __asm__ volatile("" : : "r"(this));
        createPrizeType_00(in_Prize, in_pPos);
        break;
    case 2:
        createPrizeType_02(in_Prize, in_pPos, in_pRot);
        break;
    case 3:
        createPrizeType_03(in_Prize, in_pPos, in_pRot);
        break;
    case 4:
        createPrizeType_04(in_Prize, in_pPos);
        break;
    default:
        createPrizeType_00(in_Prize, in_pPos);
        break;
    }
}

void CGimmickManager::deleteAllGimmicks() {
    SLContainerT<CTask>* pContainer;
    for (pContainer = getChildren().getHead(); pContainer != NULL; pContainer = pContainer->getNext()) {
        CTask* task = pContainer->getValue();
        if (task != NULL) task->out();
    }
}

void CGimmickManager::createPrizeType_00(const DropPrize& in_Prize, const FVECTOR* in_pPos) {
    PRIZE_PARAM param;
    param.uiPrizeFlag = 0;
    param.PrizeKind = in_Prize.Kind;
    param.vPos = *in_pPos;
    param.iNumber = in_Prize.No;
    param.uiFlag = 0;
    FLOAT msp = 0.15f;
    if (in_Prize.Kind < 0xD2) msp = 0.11f;
    for (int i = 0; i < in_Prize.Num; i++) {
        param.vRot.x = -DEG2RAD(getRand() % 35 + 45);
        param.vRot.y = DEG2RAD(getRand() % 360 - 180);
        param.vRot.z = 0.0f;
        param.fSpeed = getRandF() / 30.0f + msp;
        CEntityFactory::getInstance()->createPrize(param, NULL);
    }
}

void CGimmickManager::createPrizeType_01(const DropPrize& in_Prize, const FVECTOR* in_pPos) {
    PRIZE_PARAM param;
    param.uiPrizeFlag = 0;
    param.PrizeKind = in_Prize.Kind;
    param.vPos = *in_pPos;
    param.iNumber = in_Prize.No;
    param.uiFlag = 0;
    FLOAT msp = 0.18f;
    if (in_Prize.Kind < 0xD2) msp = 0.12f;
    for (int i = 0; i < in_Prize.Num; i++) {
        param.vRot.x = -DEG2RAD(getRand() % 20 + 70);
        param.vRot.y = DEG2RAD(getRand() % 360 - 180);
        param.vRot.z = 0.0f;
        param.fSpeed = getRandF() / 30.0f + msp;
        CEntityFactory::getInstance()->createPrize(param, NULL);
    }
}

void CGimmickManager::createPrizeType_02(const DropPrize& in_Prize, const FVECTOR* in_pPos, const FVECTOR* in_pRot) {
    PRIZE_PARAM param;
    param.uiPrizeFlag = 0;
    param.PrizeKind = in_Prize.Kind;
    param.vPos = *in_pPos;
    param.iNumber = in_Prize.No;
    param.uiFlag = 0;
    FLOAT msp = 0.1f;
    for (int i = 0; i < in_Prize.Num; i++) {
        param.vRot.x = -DEG2RAD(getRand() % 20 + 40);
        param.vRot.y = DEG2RAD(getRand() % 90 - 45 + RAD2DEG(in_pRot->y));
        param.vRot.z = 0.0f;
        param.fSpeed = getRandF() / 30.0f + msp;
        CEntityFactory::getInstance()->createPrize(param, NULL);
    }
}

void CGimmickManager::createPrizeType_03(const DropPrize& in_Prize, const FVECTOR* in_pPos, const FVECTOR* in_pRot) {
    PRIZE_PARAM param;
    param.uiPrizeFlag = 0;
    param.PrizeKind = in_Prize.Kind;
    param.vPos = *in_pPos;
    param.iNumber = in_Prize.No;
    param.uiFlag = 0;
    FLOAT msp = 0.18f;
    if (in_Prize.Kind < 0xD2) msp = 0.12f;
    for (int i = 0; i < in_Prize.Num; i++) {
        param.vRot.x = -DEG2RAD(getRand() % 10 + 20);
        param.vRot.y = DEG2RAD(getRand() % 40 - 20 + RAD2DEG(in_pRot->y));
        param.vRot.z = 0.0f;
        param.fSpeed = getRandF() / 20.0f + msp;
        CEntityFactory::getInstance()->createPrize(param, NULL);
    }
}

void CGimmickManager::createPrizeType_04(const DropPrize& in_Prize, const FVECTOR* in_pPos) {
    PRIZE_PARAM param;
    param.uiPrizeFlag = 0;
    param.PrizeKind = in_Prize.Kind;
    param.vPos = *in_pPos;
    param.iNumber = in_Prize.No;
    param.uiFlag = 0;
    FLOAT msp = 0.2f;
    if (in_Prize.Kind < 0xD2) msp = 0.1f;
    for (int i = 0; i < in_Prize.Num; i++) {
        param.vRot.x = -DEG2RAD(getRand() % 5 + 85);
        param.vRot.y = DEG2RAD(getRand() % 360 - 180);
        param.vRot.z = 0.0f;
        param.fSpeed = getRandF() / 13.0f + msp;
        CEntityFactory::getInstance()->createPrize(param, NULL);
    }
}

DropPrize CGimmickManager::checkAddCardTresureBox(DropPrize in_Prize) {
    DropPrize prize = in_Prize;
    CARD card;
    _setCard(card, in_Prize.Kind, in_Prize.No);
    card = CCard::correctCardRank(card);
    if (card.m_nKind == 0) {
        for (INT i = 0; i < m_TreasureCardDropProbNum; i++) {
            INT Kind = m_pTreasureCardDropProbTable[i].CardID + 1;
            if (checkPossibleCard((CARD_KIND)Kind)) {
                _setCard(card, Kind, 0);
                card = CCard::correctCardRank(card);
                if (card.m_nKind != 0) break;
            }
        }
        TRACE("CGimmickManager:\x83J\x81[\x83h\x8e\x9d\x82\xc4\x82\xc8\x82\xa2\x82\xa9\x82\xe7\x82\xb7\x82\xe8\x91\xd6\x82\xa6:%d\x81\xa8%d\n", in_Prize.Kind, card.m_nKind);
        prize.No = getRand() % 10;
        prize.Kind = (CARD_KIND)card.m_nKind;
        _setCard(card, prize.Kind, prize.No);
        card = CCard::correctCardRank(card);
        prize.Kind = (CARD_KIND)card.m_nKind;
        prize.No = card.m_nNumber2;
    }
    return prize;
}

DropPrize CGimmickManager::checkAddCardGimmick(DropPrize in_Prize) {
    DropPrize prize = in_Prize;
    CARD card;
    _setCard(card, in_Prize.Kind, in_Prize.No);
    card = CCard::correctCardRank(card);
    if (card.m_nKind == 0) {
        for (INT i = 0; i < m_NomalCardDropProbNum; i++) {
            INT Kind = m_pNomalCardDropProbTable[i].CardID + 1;
            if (checkPossibleCard((CARD_KIND)Kind)) {
                _setCard(card, Kind, 0);
                card = CCard::correctCardRank(card);
                if (card.m_nKind != 0) break;
            }
        }
        TRACE("CGimmickManager:\x83J\x81[\x83h\x8e\x9d\x82\xc4\x82\xc8\x82\xa2\x82\xa9\x82\xe7\x82\xb7\x82\xe8\x91\xd6\x82\xa6%d\x81\xa8%d\n", in_Prize.Kind, card.m_nKind);
        prize.No = getRand() % 10;
        prize.Kind = (CARD_KIND)card.m_nKind;
        _setCard(card, prize.Kind, prize.No);
        card = CCard::correctCardRank(card);
        prize.Kind = (CARD_KIND)card.m_nKind;
        prize.No = card.m_nNumber2;
    }
    return prize;
}

DropPrize CGimmickManager::checkAddCardMapCard(DropPrize in_Prize) {
    DropPrize prize = in_Prize;
    CARD card;
    _setCard(card, in_Prize.Kind, in_Prize.No);
    card = CCard::correctCardRank(card);
    if (card.m_nKind == 0) {
        for (INT i = 0; i < m_MapCardProb.ProbNum; i++) {
            INT Kind = m_MapCardProb.pProb[i].CardID + 0xA1;
            _setCard(card, Kind, 0);
            card = CCard::correctCardRank(card);
            if (card.m_nKind != 0) break;
        }
        TRACE("CGimmickManager:\x83J\x81[\x83h\x8e\x9d\x82\xc4\x82\xc8\x82\xa2\x82\xa9\x82\xe7\x82\xb7\x82\xe8\x91\xd6\x82\xa6%d\x81\xa8%d\n", in_Prize.Kind, card.m_nKind);
        prize.No = getRand() % 10;
        prize.Kind = (CARD_KIND)card.m_nKind;
        _setCard(card, prize.Kind, prize.No);
        card = CCard::correctCardRank(card);
        prize.Kind = (CARD_KIND)card.m_nKind;
        prize.No = card.m_nNumber2;
    }
    return prize;
}

void CGimmickManager::changeTreasureBoxLock(BOOL in_Enable) {
    SLContainerT<CTask>* pContainer;
    for (pContainer = getChildren().getHead(); pContainer != NULL; pContainer = pContainer->getNext()) {
        CTask* task = pContainer->getValue();
        if (task != NULL && task->getTypeID() == 0x2000) {
            ((CGimmickTreasureBox*)task)->setLock(in_Enable);
        }
    }
}

void CGimmickManager::GimDebugTrace(const DropPrize* prize) {
    if (prize->StockKind == 0) {
        TRACE("[GimPrize_PARAM] Kind:%d, No:%d, Num:%d\n", prize->Kind, prize->No, prize->Num);
        if (prize->Kind != CARD_KIND_None) {
            TRACE("[GimPrize_NAME] %s  \x83J\x81[\x83hNo:%d  \x8c\xc2\x90\x94:%d\n", cardDataTbl[prize->Kind].pName, prize->No, prize->Num);
        } else {
            TRACE("[GimPrize_NAME]\x82\xcd\x82\xb8\x82\xea\n");
        }
    } else {
        TRACE("[GimPrize_NAME]\x83X\x83g\x83""b\x83N\x8bZ\x8fK\x93\xbe\x81""F%d\n", prize->StockKind);
    }
}

static char* szdebugGimProbKind[5] = {
    "\x82\xcd\x82\xb8\x82\xea",
    "\x82\xdd\x82\xc7\x82\xe8\x8b\xca",
    "\x82\xa0\x82\xa9\x8b\xca",
    "\x83J\x81[\x83h",
    "\x83o\x83\x8c\x83\x8b",
};

static char* szdebugGimBallKind[3] = {
    "-----",
    "\x8f\xac\x82\xb3\x82\xa2",
    "\x91\xe5\x82\xab\x82\xa2",
};

static char* szdebugGimCardPossible[2] = {
    "\x82n\x82j",
    "\x82m\x82""f",
};

static LPCSTR szdebugGimPossibleCardName = NULL;
static LPCSTR szdebugGimCardName = NULL;

#define GDP DebugWork::g_dw.gimDropProb

static DEBUG_WINDOW_LINE debugGimProbLine[25] = {
    {"\xf9S--\xf9V\x83J\x83""e\x83S\x83\x8a\xf9S--", &CDebugWindow::none, 1, {0}, 2, 0.0f, 0.0f, 0.0f, NULL, 0, NULL, NULL},
    {"\x83\\\x83\x89\x81""F\x83J\x81[\x83h\x81@:%d", &GDP.ProbCard, 1, {0}, 2, 0.0f, 0.0f, 0.0f, NULL, 0, NULL, NULL},
    {"\x81@\x81@\x81""F\x97\xce\x8b\xca\x81@\x81@:%d", &GDP.ProbSoraGBall, 1, {0}, 2, 0.0f, 0.0f, 0.0f, NULL, 0, NULL, NULL},
    {"\x81@\x81@\x81""F\x90\xd4\x8b\xca\x81@\x81@:%d", &GDP.ProbSoraRBall, 1, {0}, 2, 0.0f, 0.0f, 0.0f, NULL, 0, NULL, NULL},
    {"\x81@\x81@\x81""F\x83o\x83\x8c\x83\x8b\x81@:%d", &GDP.ProbSoraBarel, 1, {0}, 2, 0.0f, 0.0f, 0.0f, NULL, 0, NULL, NULL},
    {"\x83\x8a\x83N\x81""F\x97\xce\x8b\xca\x81@\x81@:%d", &GDP.ProbRikuGball, 1, {0}, 2, 0.0f, 0.0f, 0.0f, NULL, 0, NULL, NULL},
    {"\x81@\x81@\x81""F\x83o\x83\x8c\x83\x8b\x81@:%d", &GDP.ProbRikuBarel, 1, {0}, 2, 0.0f, 0.0f, 0.0f, NULL, 0, NULL, NULL},
    {"\x83\x89\x83\x93\x83_\x83\x80\x92l    :%d", &GDP.ProbRandomCategory, 1, {0}, 2, 0.0f, 0.0f, 0.0f, NULL, 0, NULL, NULL},
    {"\x83J\x83""e\x83S\x83\x8a\x82\xaf\x82\xc1\x82\xa9:%s", &GDP.ProbCategoryKind, 1, {0}, 2, 0.0f, 0.0f, 0.0f, (LPCSTR*)&szdebugGimProbKind[GDP.ProbCategoryKind], 0, NULL, NULL},
    {"\xf9S--\xf9V\x97\xce\x8b\xca\xf9S--", &CDebugWindow::none, 1, {0}, 2, 0.0f, 0.0f, 0.0f, NULL, 0, NULL, NULL},
    {"\x83\x89\x83\x93\x83_\x83\x80\x92l    :%d", &GDP.ProbRandomGBall, 1, {0}, 2, 0.0f, 0.0f, 0.0f, NULL, 0, NULL, NULL},
    {"\x97\xce\x8b\xca:TYPE     :%s", &GDP.GBallType, 1, {0}, 2, 0.0f, 0.0f, 0.0f, (LPCSTR*)&szdebugGimBallKind[GDP.GBallType], 0, NULL, NULL},
    {"\x97\xce\x8b\xca:\x90\x94       :%d", &GDP.GBallNum, 1, {0}, 2, 0.0f, 0.0f, 0.0f, NULL, 0, NULL, NULL},
    {"\xf9S--\xf9V\x97\xce\x8b\xca\xf9S--", &CDebugWindow::none, 1, {0}, 2, 0.0f, 0.0f, 0.0f, NULL, 0, NULL, NULL},
    {"\x83\x89\x83\x93\x83_\x83\x80\x92l    :%d", &GDP.ProbRandomRBall, 1, {0}, 2, 0.0f, 0.0f, 0.0f, NULL, 0, NULL, NULL},
    {"\x90\xd4\x8b\xca:TYPE     :%s", &GDP.RBallType, 1, {0}, 2, 0.0f, 0.0f, 0.0f, (LPCSTR*)&szdebugGimBallKind[GDP.RBallType], 0, NULL, NULL},
    {"\x90\xd4\x8b\xca:\x90\x94       :%d", &GDP.RBallNum, 1, {0}, 2, 0.0f, 0.0f, 0.0f, NULL, 0, NULL, NULL},
    {"\xf9S--\xf9V\x83J\x81[\x83h\x81""F\x89\xf0\x8b\xd6\x97L\x82\xe8\xf9S--", &CDebugWindow::none, 1, {0}, 2, 0.0f, 0.0f, 0.0f, NULL, 0, NULL, NULL},
    {"\x83\x89\x83\x93\x83_\x83\x80\x92l    :%d", &GDP.ProbRandomCardPossible, 1, {0}, 2, 0.0f, 0.0f, 0.0f, NULL, 0, NULL, NULL},
    {"\x83J\x81[\x83h        :%s", &CDebugWindow::none, 1, {0}, 2, 0.0f, 0.0f, 0.0f, &szdebugGimPossibleCardName, 0, NULL, NULL},
    {"\x8fo\x82\xc4\x82\xe0\x82\xa2\x82\xa2\x81H  :%s", &GDP.PossibleCheck, 1, {0}, 2, 0.0f, 0.0f, 0.0f, (LPCSTR*)&szdebugGimCardPossible[GDP.PossibleCheck], 0, NULL, NULL},
    {"\xf9S--\xf9V\x83J\x81[\x83h\x81""F\x89\xf0\x8b\xd6\x96\xb3\x82\xb5\xf9S--", &CDebugWindow::none, 1, {0}, 2, 0.0f, 0.0f, 0.0f, NULL, 0, NULL, NULL},
    {"\x83\x89\x83\x93\x83_\x83\x80\x92l   :%d", &GDP.ProbRandomCard, 1, {0}, 2, 0.0f, 0.0f, 0.0f, NULL, 0, NULL, NULL},
    {"\x83J\x81[\x83h       :%s", &CDebugWindow::none, 1, {0}, 2, 0.0f, 0.0f, 0.0f, &szdebugGimCardName, 0, NULL, NULL},
    {NULL, NULL, 0, {0}, 0, 0.0f, 0.0f, 0.0f, NULL, 0, NULL, NULL},
};

#undef GDP

CGimmickProbDebug::CGimmickProbDebug(CTask* pParent) : CTask(pParent, 0x8000) {
    m_pWindow = NULL;
    DEBUG_WINDOW_TBL debugGimProbInfo = {"\x83M\x83~\x83""b\x83N\x82\xbf\x82\xe3\x82\xa4\x82\xb9\x82\xf1", 0x68, -(SCREEN_HEIGHT >> 1) + 0x28, 13, 24, 12, 0, 6, debugGimProbLine, NULL, NULL};
    m_pWindow = new (com_malloc_d(sizeof(CDebugWindow), __FILE__ "(2870)", RET_ADDR())) CDebugWindow(this, debugGimProbInfo);
    m_pWindow->dispOn(0);
    szdebugGimPossibleCardName = DebugWork::g_dw.gimDropProb.PossibleCardName;
    szdebugGimCardName = DebugWork::g_dw.gimDropProb.CardName;
}

CGimmickProbDebug::~CGimmickProbDebug() {
    if (m_pWindow) m_pWindow->out();
}

void CGimmickProbDebug::main() {
    if (!DebugWork::g_dw.gimPrize.iDisp) {
        DebugWork::g_dw.gimPrize.pClass = NULL;
        delete this;
    }
}

// SOURCE DATA: .data .rodata .bss
