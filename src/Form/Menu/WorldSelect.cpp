#include "Form/Menu/Menu.h"
#include "System/Graphic/Gprim.h"
#include "System/Graphic/Model.h"
#include "System/Graphic/2d/2DMan.h"
#include "System/Graphic/Anim.h"
#include "Game/CardBattle.h"
#include "System/Graphic/Str.h"
#include "Game/Collision/CollUtil.h"

/* Placeholder: an empty string follows CollUtil.h's strings in .rodata (as in GimmickTutorial); which header
 * it comes from is not known. */
inline void _emptyStringPlaceholder() {
    TRACE("");
}

#include "Game/Card.h"
inline void* operator new(size_t, void* p) throw() { return p; }
#include "Game/Atel/ChatMan.h"
#include "System/Graphic/Effect/effect.h"

#include "Form/Menu/WorldSelect.h"
#include "System/HeapMan.h"
#include "System/TaskMan.h"
#include "System/Math3D.h"
#include "System/Graphic/PathCamera.h"
#include "System/Graphic/2d/2DSeqCtrl.h"
#include "System/Graphic/2d/2DFnt.h"
#include "System/Graphic/2d/2DCard.h"
#include "Game/Entity.h"
#include "Game/Gimmick/GimmickDoor.h"
#include "Game/Collision/CollMapCamera.h"
#include "Game/Player/Player.h"
#include "Game/SceneInfo.h"
#include "Game/SceneEnv.h"
#include "Game/GameWork.h"
#include "Game/Game.h"

#define RET_ADDR() ({ UINT nRA; __asm__ volatile("daddu %0, $31, $0" : "=r"(nRA)); nRA; })
#define NEW(line, T) new (com_malloc_d(sizeof(T), __FILE__ "(" #line ")", RET_ADDR())) T

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

enum NAME_CASE {
    NAME_CASE_SENSITIVE = 0,
    NAME_CASE_UPPER = 1,
    NAME_CASE_LOWER = 2
};

namespace D2 {
class C2DMan {
public:
    enum D2MAN_LEVEL {
        D2MAN_LEVEL0 = 0,
        D2MAN_LEVEL1 = 1
    };
    static HANDLE create(const char* pName, UINT nIdx);
    static void destroy(HANDLE h);
    static BOOL isEnd(HANDLE h);
    static void changeLevel(D2MAN_LEVEL nLevel);
};
}

class CRsrcDB {
public:
    static struct TexInfo* getRsrcTex(const char* pName, NAME_CASE eCase);
};

class CRcf {
public:
    static void reqLoadRCFFront(INT nGroup, INT nID, INT nFlag, unsigned char* pBuf, INT nSize);
    static BOOL isCompleteLoadRCF(INT nGroup, INT nID);
};

class CPackMan {
public:
    static void changeDrawMode(INT nMode);
};

extern "C" {
float sinf(float);
float cosf(float);
float powf(float, float);
int sprintf(char*, const char*, ...);
void sceVu0RotMatrix(FMATRIX& m0, FMATRIX& m1, FVECTOR& rot);
}
void ApplyMatrix(FVECTOR& v0, const FMATRIX& m0, const FVECTOR& v1);

class CSound {
public:
    static HANDLE playSE(INT nID, INT nCh, INT nVol);
    static void pauseAllSE();
    static void startAllPausedSE();
    static void setBGMMasterVolume(INT nVol, INT nFrame);
};

class CGlare : public CTask {
private:
    static CGlare* m_pInstance;
    CGlare(CTask* p);
    void _fade(FLOAT fTime, FLOAT fAim, FLOAT fOld);
    void _off(FLOAT fTime);
    void _on(FLOAT fTime);

public:
    static void fade(FLOAT fTime, FLOAT fAim, FLOAT fOld) {
        if (m_pInstance) m_pInstance->_fade(fTime, fAim, fOld);
    }
    static void off(FLOAT fTime) {
        if (m_pInstance) m_pInstance->_off(fTime);
    }
    static void on(FLOAT fTime) {
        if (m_pInstance) m_pInstance->_on(fTime);
    }
};

CWorldSelect::CWorldSelect(CTask* p, HANDLE hDoor) : CMenu(p, -1) {
    INT i;
    INT j;

    declareSysTask(0x1D);
    CGame::enablePause(FALSE);
    m_hDoor = hDoor;
    m_pPathCamera = NULL;
    m_pLadName = "wselect.lad";
    m_pWorldCard = NULL;
    m_nMenuStep = 0;
    m_fgEnd = 0;
    m_nSelectMax = 0;
    m_nSelect = 0;
    for (INT i = 0; i < 8; i++) { /* gcc runs this one backwards (from m_nWorld[7]) */
        m_nWorld[i] = 0;
    }
    for (INT i = 0; i < 5; i++) {
        m_pCardCtrl[i] = NULL;
        m_pCard[i] = NULL;
    }
    m_pExplainCtrl = NULL;
    m_pIconCtrl = NULL;
    m_pNameWndCtrl = NULL;
    m_fgRotate = 0;
    m_nRotCurrent = 0;
    m_fRotCurrent = 0.0f;
    CGameWork::setInt(1, 0xE2, -1);

    USHORT idxMax = CSceneInfo::getCurrentFloorIndex();
    INT fgSora = CGame::isSoraGame();
    INT nMax;
    if (idxMax == 0) {
        m_nWorld[0] = fgSora ? 1 : 9;
        m_nSelectMax = 1;
    } else if (idxMax < fgSora + 5) {
        static const USHORT worldLowTbl[2][5] = { { 2, 5, 8, 1, 9999 }, { 2, 3, 4, 5, 6 } };
        for (i = 0; i < fgSora + 4; i++) {
            USHORT worldID = worldLowTbl[fgSora][i];
            for (j = 1; j < fgSora + 5; j++) {
                if (worldID == CSceneInfo::getWorldID(j)) break;
            }
            if (j >= fgSora + 5) {
                m_nWorld[m_nSelectMax++] = worldID;
            }
        }
    } else if (idxMax < (nMax = fgSora + 9)) {
        static const USHORT worldMiddleTbl[2][4] = { { 7, 3, 4, 6 }, { 7, 8, 9, 10 } };
        for (i = 0; i < 4; i++) {
            USHORT worldID = worldMiddleTbl[fgSora][i];
            for (j = fgSora + 5; j < fgSora + 9; j++) {
                if (worldID == CSceneInfo::getWorldID(j)) break;
            }
            if (j >= fgSora + 9) {
                m_nWorld[m_nSelectMax++] = worldID;
            }
        }
    } else {
        static const USHORT worldHighTbl[2][3] = { { 12, 11, 13 }, { 11, 12, 13 } };
        m_nSelectMax = 1;
        m_nWorld[0] = worldHighTbl[fgSora][idxMax - nMax];
    }

    m_pGimmickDoor = (CGimmickDoor*)CTaskMan::getTask(m_hDoor);
    m_pRoomInfo = CSceneInfo::getRoomInfo(-1);
    m_pDoorInfo = &m_pRoomInfo->_doors[m_pGimmickDoor->getDoorID()];
    int iDoorType[4] = { 0x80, 0x100, 0x200, 0x400 };
    CSceneEnv::setStop((ObjType::eObjBit)(m_iObjType = iDoorType[m_pDoorInfo->_id] ^ 0x1FFC));
    CGameWork::setByte(1, 0x140, 5);
    CSound::playSE(0x4D, -1, 0x40);
}

CWorldSelect::~CWorldSelect() {
    INT i;
    for (i = 0; i < 5; i++) {
        delete m_pCardCtrl[i];
        m_pCardCtrl[i] = NULL;
        delete m_pCard[i];
        m_pCard[i] = NULL;
    }
    delete m_pNameWndCtrl;
    m_pNameWndCtrl = NULL;
    delete m_pIconCtrl;
    m_pIconCtrl = NULL;
    delete m_pExplainCtrl;
    m_pExplainCtrl = NULL;
    delete m_pPathCamera;
    m_pPathCamera = NULL;
    dispose();
    CGameWork::setByte(1, 0x140, 0);
    CGameWork::setFlag(1, 0x1E, FALSE);
    CSceneEnv::setStop((ObjType::eObjBit)0);
    CGame::enablePause(TRUE);
}

/* CSceneEnv::setColor through an inline function: a constant frame argument then goes straight into $f12 (the
 * original's register use around these calls). */
static inline void _setColor(ObjType::eObjBit iBit, FLOAT fFrame, const FVECTOR& v, const FVECTOR* pv, INT n) {
    CSceneEnv::setColor(iBit, fFrame, v, pv, n);
}

void CWorldSelect::dispose() {
    FVECTOR vColor;
    vColor.set(1.0f, 1.0f, 1.0f, 1.0f);
    _setColor((ObjType::eObjBit)0x187C, 0.0f, vColor, NULL, 0);
    CSceneEnv::setColor((ObjType::eObjBit)4, 0.0f, vColor, NULL, 1);
    CSceneEnv::setStop((ObjType::eObjBit)0);
    CGlare::on(0.0f);
    CGameWork::setInt(1, 0xE2, 0);
}

using namespace D2;

#define ASSERT(e) if (!(e)) AssertApp(__FILE__, __LINE__, #e, 0)

/* CDrawable::setLayer on the fonts, written through a USHORT pointer as in GameOver. */
static inline void _setFntLayer(C2DFnt* p, USHORT nID, USHORT nDepth) {
    ((USHORT*)p)[8] = nID;
    ((USHORT*)p)[9] = nDepth;
}


void CWorldSelect::main() {
    FVECTOR vColor;
    FVECTOR vColor2;
    FVECTOR vPos;
    CCoord camCrd;
    G_RGBA rgba;
    LPCTSTR pSeqName = "wselect:00";
    FLOAT fFrame = getFrameRateF();

    switch (m_nStep) {
    case 0:
        if (!CRsrcDB::getRsrcTex(pSeqName, NAME_CASE_SENSITIVE)) {
            CRcf::reqLoadRCFFront(2, 6, 0, NULL, 0);
            m_fgLoadRCF = TRUE;
        }
        if (!m_pGimmickDoor) {
            m_nStep = 999;
            break;
        }
#line 239
        ASSERT(g_pPlayer);
        m_pWorldCard = NEW(240, CEntity)(this, (ObjType::eObjType)1);
        m_pWorldCard->setEntityFlag(0x400000, 0);
        m_pWorldCard->initalize(10, 0x20);
        m_pWorldCard->Crd(0).setParent(&g_pPlayer->Crd(g_pPlayer->getBoneIndex("root", 0)));
        m_pWorldCard->changeMotion(1, 0, FALSE, 0, 0, 0);
        camCrd = g_pPlayer->Crd(0);
        addCoord(1);
        Crd(0).setParent(NULL);
        Crd(0).m_Local = camCrd.m_Local;
        Crd(0).recalcWorldMatrix();
        vPos.set(0.0f, 0.01f, 0.0f, 1.0f);
        m_hSpotEffect = CEffectMan::attachEffect("rec_mon_he", 0, (ObjType::eObjType)7, &m_pGimmickDoor->Crd(0), &vPos,
                                                 NULL, 0.0f);
        vColor.set(0.0f, 0.0f, 0.0f, 1.0f);
        _setColor((ObjType::eObjBit)m_iObjType, 120.0f, vColor, NULL, 0);
        vColor.set(0.75f, 0.75f, 0.75f, 1.0f);
        _setColor((ObjType::eObjBit)(m_pDoorInfo->_id + 0x80), 120.0f, vColor, NULL, 1);
        CGlare::off(30.0f);
        m_nStep++;
        break;
    case 1:
        if (!m_pWorldCard->isMotionEnd()) {
            return;
        }
        if (m_fgLoadRCF && !CRcf::isCompleteLoadRCF(2, 6)) {
            return;
        }
        vColor._q = 0;
        CSceneEnv::setColor((ObjType::eObjBit)m_iObjType, 10.0f, vColor, NULL, 0);
        m_nStep++;
        break;
    case 2:
        CSound::pauseAllSE();
        C2DMan::changeLevel(C2DMan::D2MAN_LEVEL1);
        CPackMan::changeDrawMode(1);
        m_hLay = C2DMan::create(m_pLadName, 0);
        vPos.init();
#line 290
        m_pExplainCtrl = NEW(290, C2DSeqCtrl);
        m_pExplainCtrl->setLayer(12);
        m_pExplainCtrl->addSeq(pSeqName, 0x19);
        m_pExplainCtrl->setPos(vPos);
        m_pExplainCtrl->enableColorCtrl(TRUE);
        rgba.col = 0x807C7C7C;
#line 296
        m_pExplainFnt = NEW(296, C2DFnt)(0, 0, &rgba, 15, ": Select card         : Confirm", 0x240);
        _setFntLayer(m_pExplainFnt, 12, 0);
        m_pExplainCtrl->addFnt(pSeqName, m_pExplainFnt);
        CSound::playSE(0x4E, -1, 0x40);
        m_nStep++;
        break;
    case 3:
        if (C2DMan::isEnd(m_hLay)) {
            CSound::setBGMMasterVolume(0x40, -1);
            C2DMan::destroy(m_hLay);
            m_hLay = C2DMan::create(m_pLadName, 1);
            m_pExplainCtrl->replaceSeq(0, pSeqName, 0x1A);
            m_nStep++;
        }
        break;
    case 4:
        if (!menuMain()) {
            return;
        }
        if (m_fgEnd) {
            m_nStep = 5;
        } else {
            m_nStep = 7;
        }
        break;
    case 5:
    case 7:
        C2DMan::destroy(m_hLay);
        m_hLay = C2DMan::create(m_pLadName, 2);
        m_pExplainCtrl->replaceSeq(0, pSeqName, 0x1B);
        CSound::setBGMMasterVolume(-1, -1);
        m_nStep++;
        break;
    case 6:
    case 8:
        if (!C2DMan::isEnd(m_hLay)) {
            return;
        }
        C2DMan::destroy(m_hLay);
        m_hLay = 0;
        delete m_pExplainCtrl;
        m_pExplainCtrl = NULL;
        ATEL::CChatMan::instance().reqCommand(0x40F, NULL, 0);
        C2DMan::changeLevel(C2DMan::D2MAN_LEVEL0);
        CSound::startAllPausedSE();
        CPackMan::changeDrawMode(0);
#line 351
        ASSERT(g_pPlayer);
        if (m_nStep == 6) {
            CGameWork::setFlag(1, 0x26, TRUE);
            g_pPlayer->setTrgFlag(0x10000, 0);
            m_pWorldCard->changeMotion(CGame::isSoraGame() ? 2 : 3, 0, FALSE, 0, 0, 0);
            m_nStep = 9;
        } else {
            m_pWorldCard->setupFade(1.0f, 0.0f, 1200, -1);
            vColor.set(1.0f, 1.0f, 1.0f, 1.0f);
            vColor2.set(0.25f, 0.25f, 0.25f, 1.0f);
            _setColor((ObjType::eObjBit)m_iObjType, 30.0f, vColor, &vColor2, 0);
            CSceneEnv::setColor((ObjType::eObjBit)4, 30.0f, vColor, &vColor2, 1);
            CGlare::on(30.0f);
            CEffectMan::killEffect(m_hSpotEffect, 30.0f);
            m_nStep = 16;
        }
        break;
    case 9:
#line 373
        m_pPathCamera = NEW(373, CPathCamera)(this);
        m_pPathCamera->setLocal(TRUE);
        m_pPathCamera->init("world_cr_01", TRUE, FALSE, 0x80);
        m_pPathCamera->run();
        CCollMapCamera::m_fgForceStop = TRUE;
        CCollMapCamera::clearList();
        CSound::playSE(0x4F, -1, 0x40);
        m_fTimer = 240.0f;
        m_nStep++;
        break;
    case 10:
        m_fTimer -= fFrame;
        if (m_fTimer < 0.0f) {
            CGlare::fade(30.0f, 1.0f, -1.0f);
            m_pWorldCard->setupFade(1.0f, 0.0f, 10, -1);
            m_nStep++;
        }
    case 11:
        if (m_pPathCamera->isEnd()) {
            m_pPathCamera->init("world_cr_02", TRUE, FALSE, 0x80);
            m_pPathCamera->run();
            m_fTimer = 30.0f;
            m_nStep++;
        }
        break;
    case 12:
        m_fTimer -= fFrame;
        if (m_fTimer < 0.0f) {
            m_pGimmickDoor->changeMotion(2, 0, FALSE, -1, 0, 0);
            m_pPathCamera->init("world_cr_03", TRUE, FALSE, 0x80);
            m_pPathCamera->run();
            m_fTimer = 180.0f;
            m_nStep++;
        }
        break;
    case 13:
        m_fTimer -= fFrame;
        if (m_fTimer < 0.0f) {
            m_fTimer = 0.0f;
            m_nStep++;
        }
        break;
    case 14:
        m_fTimer -= fFrame;
        if (m_fTimer < 0.0f) {
            CSceneInfo::reqEnterWorld((WORLD_ID)m_nWorld[m_nSelect]);
            m_nStep = 15;
        }
        break;
    case 15:
        break;
    case 16:
        if (CSceneEnv::isColorEnd(0)) {
#line 439
            ASSERT(g_pPlayer);
            g_pPlayer->setTrgFlag(0x8000, 0);
            CGameWork::setInt(1, 0xE2, 0);
            CSceneEnv::setStop((ObjType::eObjBit)0);
            m_nStep = 17;
        }
        break;
    case 17:
    default:
        m_pWorldCard->out();
        out();
        break;
    }
}
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

INT CWorldSelect::menuMain() {
    static const unsigned char cardKindTbl[14] = { 0, 190, 191, 192, 193, 194, 195, 196, 197, 198, 199, 200, 201, 202 };
    static const unsigned char iconSeqNameNoTbl[14] = { 0, 11, 1, 9, 13, 7, 5, 2, 8, 6, 10, 12, 4, 3 };
    INT ret = 0;
    CARD card;
    FVECTOR vec;
    FVECTOR vec2;
    FVECTOR vScale;
    G_RGBA rgba;
    LPCTSTR pSeqName = "wselect:00";
    INT nRotMove = 0;
    char pIconSeqNameTmp[16];

    sprintf(pIconSeqNameTmp, "wselect:%02d", iconSeqNameNoTbl[m_nWorld[m_nSelect]]);
    switch (m_nMenuStep) {
    case 0:
        m_rotCircle._q = 0;
        m_rotCircle.x = 1.0471976f;
        vec2.y = 0.0f;
        m_nRotOne = 360 / m_nSelectMax;
        for (INT i = 0; i < m_nSelectMax; i++) {
            m_posLocal[i].init();
            m_posLocal[i].w = 1.0f; /* again: the second use of 1.0f gets it hoisted out of the loop */
            m_posLocal[i].x = sinf(vec2.y) * 150.0f;
            m_posLocal[i].y = cosf(vec2.y) * 150.0f;
            vec2.y += m_nRotOne * 0.017453292f;
#line 509
            C2DSeqCtrl* pCardCtrl = m_pCardCtrl[i] = NEW(509, C2DSeqCtrl);
            pCardCtrl->addSeq(pSeqName, 0xD);
#line 512
            C2DCard* pCard = m_pCard[i] = NEW(512, C2DCard);
            _setCard(card, cardKindTbl[m_nWorld[i]], 0);
            pCard->create(card);
            pCard->setParent(pCardCtrl);
        }
        m_fgEnd = 0;
        m_nMenuStep = 1;
        break;
    case 1:
        m_nMenuStep = 2;
        break;
    case 2:
        if (!m_pCardCtrl[0]->isEnd(0)) {
            break;
        }
        for (INT i = 0; i < m_nSelectMax; i++) {
            m_pCardCtrl[i]->replaceSeq(0, pSeqName, 0xE);
        }
        m_pCardCtrl[m_nSelect]->replaceSeq(0, pSeqName, 0x11);
        m_pCardCtrl[m_nSelect]->addSeq(pSeqName, 0x12);
#line 534
        m_pNameWndCtrl = NEW(534, C2DSeqCtrl);
        m_pNameWndCtrl->setLayer(13);
        m_nSeqNameWnd = 10;
        m_pNameWndCtrl->addSeq(pSeqName, 10);
        vec.init();
        m_pNameWndCtrl->setPos(vec);
        rgba.col = 0x807C7C7C;
#line 541
        m_pNameWndFnt = NEW(541, C2DFnt)(0, 0, &rgba, 0x11, cardDataTbl[cardKindTbl[m_nWorld[m_nSelect]]].pName, 0x242);
        _setFntLayer(m_pNameWndFnt, 12, 0);
        m_pNameWndCtrl->addFnt(pSeqName, m_pNameWndFnt);
        m_pNameWndCtrl->enableColorCtrl(TRUE);
#line 548
        m_pIconCtrl = NEW(548, C2DSeqCtrl);
        m_pIconCtrl->setLayer(12);
        m_pIconCtrl->setPos(vec);
        m_pIconCtrl->addSeq(pIconSeqNameTmp, 0);
        m_nMenuStep = 3;
        break;
    case 3:
        if (m_nSeqNameWnd != 11) {
            if (!m_pNameWndCtrl->isEnd(0)) {
                break;
            }
            m_nSeqNameWnd = 11;
            m_pNameWndCtrl->replaceSeq(0, pSeqName, 11);
        }
        if (_isTrgOK(0)) {
            m_fgEnd = 1;
            m_pIconCtrl->addSeq(pSeqName, 0x14);
            for (INT i = 0; i < m_nSelectMax; i++) {
                if (i != m_nSelect) {
                    m_pCardCtrl[i]->replaceSeq(0, pSeqName, 0xF);
                }
            }
            CSound::playSE(0x1A, -1, 0x40);
            m_nMenuStep = 6;
        } else if (_isTrgCANCEL(0)) {
            if (CGameWork::getFlag(1, 0x1E)) {
                break;
            }
            CSound::playSE(4, -1, 0x40);
            m_nMenuStep = 7;
        } else if (_isButtonRpt(0x408000, 0)) {
            nRotMove = -m_nRotOne;
        } else if (_isButtonRpt(0x802000, 0)) {
            nRotMove = m_nRotOne;
        }
        if (m_nSelectMax >= 2 && nRotMove != 0) {
            INT nSelectOld = m_nSelect;
            if (nRotMove > 0) {
                if (++m_nSelect >= m_nSelectMax) {
                    m_nSelect = 0;
                }
            } else {
                if (--m_nSelect < 0) {
                    m_nSelect = m_nSelectMax - 1;
                }
            }
            m_fgRotate = 1;
            m_fRotOld = m_nRotCurrent;
            m_nRotCurrent += nRotMove;
            m_fRotTarget = m_nRotCurrent;
            m_fTimer = 0.0f;
            if (m_pCardCtrl[nSelectOld]->getNumSeq() >= 2) {
                m_pCardCtrl[nSelectOld]->replaceSeq(0, pSeqName, 0xE);
                m_pCardCtrl[nSelectOld]->destroy(1);
                m_nSeqNameWnd = 12;
                m_pNameWndCtrl->replaceSeq(0, pSeqName, 12);
                m_pIconCtrl->replaceSeq(0, pIconSeqNameTmp, 1);
            }
            CSound::playSE(0x19, -1, 0x40);
            m_nMenuStep = 4;
        }
        break;
    case 4:
        if (m_fgRotate) {
            break;
        }
        m_pCardCtrl[m_nSelect]->replaceSeq(0, pSeqName, 0x11);
        m_pCardCtrl[m_nSelect]->addSeq(pSeqName, 0x12);
        m_nSeqNameWnd = 10;
        m_pNameWndCtrl->replaceSeq(0, pSeqName, 10);
        m_pNameWndFnt->setText(cardDataTbl[cardKindTbl[m_nWorld[m_nSelect]]].pName);
        m_pIconCtrl->replaceSeq(0, pIconSeqNameTmp, 0);
        m_nMenuStep = 3;
        break;
    case 6:
        if (m_pIconCtrl->isEnd(1)) {
            m_nMenuStep = 7;
        }
        break;
    case 7:
        for (INT i = 0; i < m_nSelectMax; i++) {
            if (!m_fgEnd || i == m_nSelect) {
                m_pCardCtrl[i]->replaceSeq(0, pSeqName, 0xF);
            }
        }
        m_pCardCtrl[m_nSelect]->replaceSeq(0, pSeqName, 0xF);
        m_pIconCtrl->replaceSeq(0, pIconSeqNameTmp, 1);
        m_pNameWndCtrl->replaceSeq(0, pSeqName, 0xC);
        m_nMenuStep = 8;
        break;
    case 8:
        if (m_pNameWndCtrl->isEnd(0)) {
            m_nMenuStep = 9;
        }
        break;
    case 9:
        for (INT i = 0; i < 5; i++) {
            delete m_pCardCtrl[i];
            m_pCardCtrl[i] = NULL;
            delete m_pCard[i];
            m_pCard[i] = NULL;
        }
        delete m_pNameWndCtrl;
        m_pNameWndCtrl = NULL;
        delete m_pIconCtrl;
        m_pIconCtrl = NULL;
        ret = 1;
        break;
    }
    if (m_nMenuStep == 9) {
        return ret;
    }
    if (m_fgRotate) {
        m_fTimer += getFrameRateF();
        if (m_fTimer >= 30.0f) {
            m_fTimer = 30.0f;
            m_fgRotate = 0;
        }
        FLOAT fRate = 1.0f - powf(1.0f - m_fTimer / 30.0f, 1.75f);
        m_fRotCurrent = m_fRotTarget * fRate + m_fRotOld * (1.0f - fRate);
        m_rotCircle.z = m_fRotCurrent = Math3D::LimitPI(m_fRotCurrent * 0.017453292f);
    }
    FMATRIX mat;
    _unitMatrixVu0(mat);
    vec2._q = 0;
    vec2.y = m_rotCircle.z;
    sceVu0RotMatrix(mat, mat, m_rotCircle);
    for (INT i = 0; i < m_nSelectMax; i++) {
        ApplyMatrix(vec, mat, m_posLocal[i]);
        m_pCardCtrl[i]->setLocalPos(vec);
        m_pCardCtrl[i]->setScl((vec.y + 75.0f) * 0.35 / 150.0 + 0.65f);
        vec2.y = Math3D::LimitPI(vec2.y);
        m_pCardCtrl[i]->setRot(vec2);
        m_pCard[i]->checkBack();
        vec2.y -= m_nRotOne * 0.017453292f;
    }
    return ret;
}
// SOURCE DATA: .rodata
