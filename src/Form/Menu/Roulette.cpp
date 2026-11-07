
#define GPRIM_H_PATH "System/Graphic/GPrim.h" /* MATCH: this unit names Gprim.h so (header default "System/Graphic/Gprim.h") */
#include "Game/Card.h"
#include "System/Graphic/Gprim.h"
#include "System/Graphic/2d/2DMan.h"
#include "System/Graphic/Anim.h"
#include "Game/CardBattle.h"
#include "System/Graphic/Str.h"
#include "Game/Collision/CollUtil.h"

/* MATCH: Placeholder: an empty string follows CollUtil.h's strings in .rodata (as in WorldSelect); which header it comes
 * from is not known. */
inline void _emptyStringPlaceholder() {
    TRACE("");
}

#include "Game/Status.h"
#include "Game/Technic.h"
#include "System/Graphic/Model.h"
#include "Game/CardKind.h"
#include "Form/Menu/Roulette.h"
#include "Form/Menu/MenuParts.h"
#include "System/HeapMan.h"
#include "System/Vector.h"
#include "System/Graphic/2d/2DSeqCtrl.h"
#include "System/Graphic/2d/2DSeqObj.h"
#include "System/Graphic/2d/2DFnt.h"
#include "System/Graphic/2d/2DCard.h"
#include "Game/SceneEnv.h"
#include "Game/Game.h"
#include "Game/GameWork.h"
#include "Game/Gimmick/GimmickManager.h"
#include "include_asm.h"

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



























#include "System/RsrcDB.h"





#include "System/Rcf.h"





#include "System/Graphic/PackMan.h"




#include "System/Sound/SoundMan.h"
#line 314

extern "C" {
int sprintf(char*, const char*, ...);
float sinf(float);
float cosf(float);
float powf(float, float);
}

#include "Game/Gimmick/GimmickManager.h"






using namespace D2;

#define ASSERT(e) if (!(e)) AssertApp(__FILE__, __LINE__, #e, 0)

CRoulette::CRoulette(CTask* p, BOOL fgRoulette) : CMenu(p, -1) {
    declareSysTask(0x22);
    m_fgRoulette = fgRoulette;
    m_fgRotate = 0;
    CGame::enablePause(FALSE);
    m_pLadName = "premium.lad";
    m_pSeqName = "premium:00";
    m_pBaseCtrl = NULL;
    m_pCardInfoCtrl = NULL;
    for (INT i = 0; i < 11; i++) {
        m_pCardCtrl[i] = NULL;
    }
    for (INT i = 0; i < 99; i++) {
        m_pCard[i] = NULL;
    }
    m_pGetCardCtrl = NULL;
    m_pCardGetCtrl = NULL;
    m_pEffectCtrl = NULL;
    m_pBaloonCtrl = NULL;
    m_pCursorCtrl = NULL;
    for (INT i = 0; i < 30; i++) {
        m_pNumButton[i] = NULL;
    }
    m_pHelpButton = NULL;
    CSound::pauseAllSE();
    CSceneEnv::setStop((ObjType::eObjBit)0xFFFEFFFF);
    CSceneEnv::setStopGauge(TRUE);
}

CRoulette::~CRoulette() {
    dispose();
    CSound::startAllPausedSE();
    CGame::enablePause(TRUE);
    CSceneEnv::setStop((ObjType::eObjBit)0);
    CSceneEnv::setStopGauge(FALSE);
    C2DMan::changeLevel(C2DMan::D2MAN_LEVEL0);
    CRsrcDB::removeGroup(2, 0x69);
    CPackMan::changeDrawMode(0);
}

void CRoulette::dispose() {
    delete m_pBaseCtrl;
    m_pBaseCtrl = NULL;
    delete m_pCardInfoCtrl;
    m_pCardInfoCtrl = NULL;
    for (INT i = 0; i < 99; i++) {
        delete m_pCard[i];
        m_pCard[i] = NULL;
    }
    for (INT i = 0; i < 11; i++) {
        delete m_pCardCtrl[i];
        m_pCardCtrl[i] = NULL;
    }
    delete m_pCursorCtrl;
    m_pCursorCtrl = NULL;
    delete m_pBaloonCtrl;
    m_pBaloonCtrl = NULL;
    delete m_pEffectCtrl;
    m_pEffectCtrl = NULL;
    delete m_pCardGetCtrl;
    m_pCardGetCtrl = NULL;
    delete m_pGetCardCtrl;
    m_pGetCardCtrl = NULL;
    for (INT i = 0; i < 30; i++) {
        delete m_pNumButton[i];
        m_pNumButton[i] = NULL;
    }
    delete m_pHelpButton;
    m_pHelpButton = NULL;
}

static inline void _setCard(CARD& c, USHORT kind, BYTE num) {
    c.m_nId = 0;
    c.m_nKind2 = kind;
    c.m_nNumber2 = num;
    c.m_nCategory20 = cardDataTbl[c.m_nKind].nCategory0;
    c.m_nCategory21 = cardDataTbl[c.m_nKind].nCategory1;
}

void CRoulette::main() {
    static LPCTSTR pStr[2] = { "Upgrade a card to a Premium Card.         : Cancel", "Choose a Map Card." };
    FVECTOR vec;
    C2DCard* p2DCard;
    CARD* pDeck;
    UINT nCard;
    static USHORT helpSeqTbl[3] = { 56, 57, 58 };

    switch (m_nStep) {
    case 0:
        CPackMan::changeDrawMode(1);
        m_nMenuStep = 0;
        if (!CRsrcDB::getRsrcTex(m_pSeqName, NAME_CASE_SENSITIVE)) {
            CRcf::reqLoadRCFFront(2, 0x69, 0, NULL, 0);
            m_nStep = 1;
            break;
        }
        m_nStep = 2;
        break;
    case 1:
        if (CRcf::isCompleteLoadRCF(2, 0x69)) {
            m_nStep = 2;
        }
        break;
    case 2:
        C2DMan::changeLevel(C2DMan::D2MAN_LEVEL1);
        m_nStep = 3;
        break;
    case 3: {
        vec.set(-200.0f, -100.0f, 0.0f, 1.0f);
        if (!m_fgRoulette) {
            USHORT nDeck = CCard::getCurrentDeckNo();
            nCard = CCard::getDeckMax(nDeck);
            pDeck = CCard::getDeck(nDeck);
        } else {
            CARD card[22];
            CGimmickManager* pGimMan = (CGimmickManager*)getSysTask(0x11);
            if (!pGimMan) {
                out();
                return;
            }
            DropPrize dorpPrize;
            dorpPrize.StockKind = 0;
            dorpPrize.Kind = CARD_KIND_None;
            dorpPrize.Num = 0;
            dorpPrize.No = 0;
            for (INT i = 0; i < 22; i++) {
                dorpPrize = pGimMan->DecideMapCardPrize(0);
                if ((UINT)(dorpPrize.Kind - 0xA1) < 0x19) {
                    _setCard(card[i], dorpPrize.Kind, dorpPrize.No);
                } else {
                    _setCard(card[i], 0xB6, 1);
                }
            }
            pDeck = card;
            nCard = 22;
        }
        m_nCardMax = 0;
        for (INT i = 0; i < 99 && m_nCardMax < nCard; i++) {
            if (m_fgRoulette || (CCard::canPremium(pDeck[i].m_nKind) && !pDeck[i].m_fgPremium)) {
                m_idxDeck[m_nCardMax] = i;
#line 201
                p2DCard = m_pCard[m_nCardMax++] = new (__FILE__ "(201)") C2DCard;
                p2DCard->setLayer(12);
                p2DCard->create(pDeck[i]);
                p2DCard->setScl(0.9f);
                p2DCard->enableColorCtrl(TRUE);
                p2DCard->enableDisp(FALSE);
            }
        }
#line 208
        ASSERT(m_nCardMax);
        m_nCardDummyMax = m_nCardMax;
        if (m_nCardDummyMax) {
            while (m_nCardDummyMax < 11) {
                for (INT j = 0; j < m_nCardMax; j++) {
#line 213
                    p2DCard = m_pCard[m_nCardDummyMax++] = new (__FILE__ "(213)") C2DCard;
                    p2DCard->setLayer(12);
                    p2DCard->create(m_pCard[j]->m_card);
                    p2DCard->setScl(0.9f);
                    p2DCard->enableColorCtrl(TRUE);
                    p2DCard->enableDisp(FALSE);
                }
            }
        }
        for (INT i = 0; i < 11; i++) {
#line 223
            C2DSeqCtrl* pCtrl = m_pCardCtrl[i] = new (__FILE__ "(223)") C2DSeqCtrl;
            pCtrl->enableColorCtrl(TRUE);
            pCtrl->addSeq(m_pSeqName, 0x1B);
            if (m_pCard[10 - i]) {
                m_pCard[10 - i]->setParent(pCtrl);
            }
            pCtrl->enableColorCtrl(TRUE);
        }
        CSound::playSE(3, -1, 0x40);
        m_nStep = 4;
        break;
    }
    case 4:
#line 239
        m_pBaseCtrl = new (__FILE__ "(239)") C2DSeqCtrl;
        m_pBaseCtrl->addSeq(m_pSeqName, 0);
        vec.init();
        m_pBaseCtrl->setPos(vec);
        m_hLay = C2DMan::create(m_pLadName, m_fgRoulette ? 0 : 5);
#line 245
        m_pHelpButton = new (__FILE__ "(245)") CButton;
        vec.set(-256.0f, -192.0f, 0.0f, 1.0f);
        m_pHelpButton->init(m_pSeqName, 0xE, helpSeqTbl, vec, NULL, pStr[m_fgRoulette], 0xF, 0.0f, 0.0f, 0x40, NULL,
                            -1);
        m_nStep++;
        m_nStatusLad = 0;
        break;
    case 5:
        if (!m_nStatusLad && C2DMan::isEnd(m_hLay)) {
            m_pBaseCtrl->replaceSeq(0, m_pSeqName, 1);
            C2DMan::destroy(m_hLay);
            C2DMan::setLayer(13);
            m_hLay = C2DMan::create(m_pLadName, !m_fgRoulette ? 6 : 1);
            m_pHelpButton->setSequence(CButton::BS_LOOP, FALSE);
            m_nStatusLad = 1;
        }
        if ((m_nStatusLad != 0) == TRUE) {
            setCardInfo(0);
            m_nStep = 6;
        }
        break;
    case 6:
        if (setCardInfo(1)) {
            m_nStep = 7;
        }
        break;
    case 7:
        if (menuMain()) {
            m_fgRotate = 0;
            m_nStep = 8;
        }
        break;
    case 8:
        setCardInfo(2);
        m_nStep = 9;
        break;
    case 9:
        m_pBaseCtrl->replaceSeq(0, m_pSeqName, 2);
        C2DMan::destroy(m_hLay);
        m_hLay = C2DMan::create(m_pLadName, !m_fgRoulette ? 7 : 2);
        m_pHelpButton->setSequence(CButton::BS_OUT, FALSE);
        CSound::playSE(4, -1, 0x40);
        m_nStep++;
        break;
    case 10:
        if (!C2DMan::isEnd(m_hLay)) {
            return;
        }
        C2DMan::destroy(m_hLay);
        dispose();
        m_nStep = 12;
        break;
    case 11:
    case 12:
    default:
        out();
        break;
    }
}

template <class T> static inline T& _ref(T& v) { return v; }

INT CRoulette::menuMain() {
    static LPCTSTR pStr = "%s is now\na Premium Card!";
    INT ret = 0;
    FVECTOR vec;
    FVECTOR vec2;
    FVECTOR vScale;
    G_RGBA rgba;
    static LPCTSTR pSeqName2 = "get_card:00";
    char buf[64];

    switch (m_nMenuStep) {
    case 0:
        m_nMenuStep = 1;
        break;
    case 1:
        m_fgRotate = 1;
        m_nMenuStep = 2;
        m_fRouletteRotY = 0.0f;
        m_fSpeed = 0.0f;
        m_fTimer = 0.0f;
        m_idxStart = 0;
        m_fgReplace = 0;
        m_fgSelect = 0;
        break;
    case 2:
        m_fTimer += (FLOAT)getFrameRate() * 0.0200000014f;
        if (m_fTimer > 120.0f) {
#line 371
            m_pCursorCtrl = new (__FILE__ "(371)") C2DSeqCtrl;
            m_pCursorCtrl->setLayer(14);
            m_pCursorCtrl->addSeq(m_pSeqName, 3);
            m_pCursorCtrl->setScl(0.9f);
            if (!m_fgRoulette) {
                vec.set(244.0f, 184.0f, 0.0f, 1.0f);
            } else {
                vec.set(184.0f, 206.0f, 0.0f, 1.0f);
            }
            m_pCursorCtrl->setPos(vec);
#line 381
            m_pEffectCtrl = new (__FILE__ "(381)") C2DSeqCtrl;
            m_pEffectCtrl->setLayer(11);
            m_pEffectCtrl->addSeq(m_pSeqName, 0x20);
            m_pEffectCtrl->setPos(vec);
#line 387
            m_pBaloonCtrl = new (__FILE__ "(387)") C2DSeqCtrl;
            m_pBaloonCtrl->setLayer(14);
            m_pBaloonCtrl->addSeq(m_pSeqName, 7);
            vec.x += 16.0f;
            vec.y += -36.0f;
            m_pBaloonCtrl->setPos(vec);
            m_fSpeed = 1.0f;
            m_nMenuStep = 3;
        } else {
            m_fSpeed = m_fTimer / 120.0f;
        }
        break;
    case 3:
        m_fSpeed = (FLOAT)getFrameRate() * 0.0200000014f;
        if (_isTrgOK(0)) {
            m_fTimer = 0.0f;
            m_fgSelect = 1;
            m_pBaloonCtrl->enableDisp(FALSE);
            if (!m_fgRoulette) {
                m_nSelect = (m_idxStart + 5) % m_nCardMax;
                CCard::makePremiereDeck(CCard::getCurrentDeckNo(), m_idxDeck[m_nSelect]);
                m_nSelect = (m_idxStart + 5) % m_nCardDummyMax;
            } else {
                m_nSelect = (m_idxStart + 5) % m_nCardDummyMax;
                CCard::addCard(m_pCard[m_nSelect]->m_card.m_nKind2, m_pCard[m_nSelect]->m_card.m_nNumber2);
                m_idxGetCard =
                    (m_pCard[m_nSelect]->m_card.m_nCategory21 - 9) * 10 + m_pCard[m_nSelect]->m_card.m_nNumber2;
            }
            CSound::playSE(2, -1, 0x40);
            m_nMenuStep = 4;
        } else if (!m_fgRoulette && _isTrgCANCEL(0)) {
            m_pCursorCtrl->enableDisp(FALSE);
            m_pBaloonCtrl->enableDisp(FALSE);
            m_fTimer = 0.0f;
            CSound::playSE(4, -1, 0x40);
            m_nMenuStep = 13;
        }
        break;
    case 4:
        m_fTimer += (FLOAT)getFrameRate() * 0.0200000014f;
        if (m_fTimer > 60.0f) {
            m_nMenuStep = 5;
            m_fTimer = 0.0f;
        } else {
            m_pCardCtrl[4]->m_seqControl.vCol.w -= 0.05f;
            m_pCardCtrl[6]->m_seqControl.vCol.w -= 0.05f;
            if (m_fTimer >= 10.0f) {
                m_pCardCtrl[3]->m_seqControl.vCol.w -= 0.05f;
                m_pCardCtrl[7]->m_seqControl.vCol.w -= 0.05f;
            }
            if (m_fTimer >= 20.0f) {
                m_pCardCtrl[2]->m_seqControl.vCol.w -= 0.05f;
                m_pCardCtrl[8]->m_seqControl.vCol.w -= 0.05f;
            }
            if (m_fTimer >= 30.0f) {
                m_pCardCtrl[1]->m_seqControl.vCol.w -= 0.05f;
                m_pCardCtrl[9]->m_seqControl.vCol.w -= 0.05f;
            }
            if (m_fTimer >= 40.0f) {
                m_pCardCtrl[0]->m_seqControl.vCol.w -= 0.05f;
                m_pCardCtrl[10]->m_seqControl.vCol.w -= 0.05f;
            }
            for (INT i = 0; i < 11; i++) {
                if (m_pCardCtrl[i]->m_seqControl.vCol.w < 0.0f) {
                    m_pCardCtrl[i]->m_seqControl.vCol.w = 0.0f;
                }
            }
        }
        break;
    case 5:
        m_fTimer += (FLOAT)getFrameRate() * 0.0200000014f;
        if (m_fTimer > 30.0f) {
            m_pCursorCtrl->enableDisp(FALSE);
            m_pEffectCtrl->replaceSeq(0, m_pSeqName, 0x21);
            m_fTimer = 0.0f;
            m_nMenuStep = 6;
        }
        break;
    case 6:
    case 13:
        m_fTimer += (FLOAT)getFrameRate() * 0.0200000014f;
        if (m_fTimer > 30.0f) {
            m_nMenuStep = (m_nMenuStep == 13) ? 14 : 7;
        } else {
            for (INT i = 0; i < 11; i++) {
                m_pCardCtrl[i]->m_seqControl.vCol.w -= 0.05f;
                if (m_pCardCtrl[i]->m_seqControl.vCol.w < 0.0f) {
                    m_pCardCtrl[i]->m_seqControl.vCol.w = 0.0f;
                }
            }
        }
        break;
    case 7:
        m_fTimer = 0.0f;
#line 495
        m_pCardGetCtrl = new (__FILE__ "(495)") C2DSeqCtrl;
        m_pCardGetCtrl->setLayer(14);
        m_pCardGetCtrl->enableColorCtrl(TRUE);
        m_pCardGetCtrl->addSeq(pSeqName2, m_fgRoulette ? 2 : 9);
        vec.init();
        m_pCardGetCtrl->setPos(vec);
        rgba.col = 0x807C7C7C;
#line 503
        m_pGetCardFnt[0] = new (__FILE__ "(503)") C2DFnt(0, 0, &rgba, 16, m_pCard[m_nSelect]->m_card.getName(), 0x42);
        m_pCardGetCtrl->addFnt(pSeqName2, m_pGetCardFnt[0]);
        sprintf(buf, pStr, m_pCard[m_nSelect]->m_card.getName());
        if (!m_fgRoulette) {
#line 509
            m_pGetCardFnt[1] = new (__FILE__ "(509)") C2DFnt(0, 0, &rgba, 16, NULL, 0x42);
            m_pGetCardFnt[1]->allocText(buf);
            m_pGetCardFnt[1]->setSpace(16, 0x60);
            m_pCardGetCtrl->addFntShift(_ref(pSeqName2), m_pGetCardFnt[1], 0.0f, 243.0f);
            m_pCard[m_nSelect]->m_card.m_fgPremium = 1;
        }
#line 516
        m_pGetCardCtrl = new (__FILE__ "(516)") C2DSeqCtrl;
        m_pGetCardCtrl->addSeq(_ref(pSeqName2), 0);
        m_pGetCardCtrl->setPos(vec);
        m_pCard[m_nSelect]->changeLayer(14);
        m_pCard[m_nSelect]->replace(m_pCard[m_nSelect]->m_card);
        m_pCard[m_nSelect]->setLocalPos(vec);
        m_pCard[m_nSelect]->setScl(1.0f);
        m_pCard[m_nSelect]->setParent(m_pGetCardCtrl);
        CSound::playSE(0x3A, -1, 0x40);
        m_nMenuStep = 9;
        break;
    case 9:
        m_fTimer += (FLOAT)getFrameRate() * 0.0200000014f;
        if (m_fTimer > 180.0f) {
            m_pCardGetCtrl->replaceSeq(0, pSeqName2, m_fgRoulette ? 3 : 10);
            m_pGetCardCtrl->replaceSeq(0, pSeqName2, 1);
            m_fTimer = 0.0f;
            m_nMenuStep = 8;
        }
        break;
    case 8:
        m_fTimer += (FLOAT)getFrameRate() * 0.0200000014f;
        if (m_fTimer > 30.0f) {
            if (!m_fgRoulette || m_pCard[m_nSelect]->m_card.m_nKind == 0xB9) {
                m_nMenuStep = 14;
            } else {
                m_nMenuStep = 10;
            }
        }
        break;
    case 10:
        m_pNumButton[m_idxGetCard]->get2DSeqCtrl()->addSeq(m_pSeqName, 0x34);
        CSound::playSE(0x55, -1, 0x40);
        m_fTimer = 0.0f;
        m_nMenuStep = 11;
        break;
    case 11:
        m_fTimer += (FLOAT)getFrameRate() * 0.0200000014f;
        if (m_fTimer > 30.0f) {
            setCardInfo(4);
            m_fTimer = 0.0f;
            m_nMenuStep = 12;
        }
        break;
    case 12:
        m_fTimer += (FLOAT)getFrameRate() * 0.0200000014f;
        if (m_fTimer > 60.0f) {
            m_nMenuStep = 14;
        }
        break;
    case 14:
        ret = 1;
        break;
    }

    if (m_fgRotate && m_pCard[0]) {
        FLOAT fBaseRad;
        FLOAT fStartRad;
        FLOAT fRateX;
        if (!m_fgRoulette) {
            fBaseRad = 0.17453292f;
            fStartRad = -fBaseRad;
            fRateX = 1.5f;
        } else {
            fBaseRad = 0.2617994f;
            fStartRad = -0.5235988f;
            fRateX = 1.0f;
        }
        FLOAT fRate = m_fSpeed;
        FLOAT fRadian = fBaseRad * (fRate * 0.5f + 0.5f) * (!m_fgRoulette ? 0.15f : 0.1f);
        if (fRate > 1.0f) {
            fRate = 1.0f;
        }
        FLOAT fRadius = (1.0f - powf(1.0f - fRate, 2.0f)) * 225.0f;
        if (m_fgSelect) {
            fRadius = 225.0f;
            fRadian = 0.0f;
            m_fgReplace = 0;
            m_fRouletteRotY = fBaseRad * 0.5f;
        }
        m_fRouletteRotY += fRadian;
        if (m_fRouletteRotY >= fBaseRad) {
            m_fRouletteRotY -= fBaseRad;
            m_fgReplace = 0;
            m_pCard[m_idxStart]->enableDisp(FALSE);
            if (++m_idxStart >= m_nCardDummyMax) {
                m_idxStart = 0;
            }
            INT j = m_idxStart;
            for (INT i = 0; i < 11; i++) {
                m_pCard[j]->setParent(m_pCardCtrl[i]);
                m_pCard[j]->replace(m_pCard[j]->m_card);
                m_pCard[j]->enableDisp(TRUE);
                if (++j >= m_nCardDummyMax) {
                    j = 0;
                }
            }
            CSound::playSE(1, -1, 0x40);
        }
        FLOAT fRotStart = m_fRouletteRotY + fStartRad;
        vec.init();
        for (INT i = 10; i >= 0; i--) {
            vec.x = sinf(fRotStart) * fRadius * fRateX;
            vec.y = -cosf(fRotStart) * fRadius;
            vec.x -= 250.0f;
            vec.y += 150.0f;
            m_pCardCtrl[i]->setLocalPos(vec);
            fRotStart += fBaseRad;
        }
    }
    return ret;
}
INT CRoulette::setCardInfo(INT nCmd) {
    INT ret;
    FVECTOR vec;
    FVECTOR vec2;
    FVECTOR vScale;
    CButton* pButton;
    C2DSeqObj* p2DSeqObj;
    static USHORT mapKindTbl[6] = { 161, 172, 172, 179, 179, 185 };
    static USHORT numSeqTbl[15] = { 50, 50, 50, 50, 51, 48, 48, 48, 48, 49, 46, 46, 46, 46, 47 };

    if (!m_fgRoulette) {
        return TRUE;
    }
    const char* pFmt = "%02d";
    ret = 0;
    switch (nCmd) {
    case 0:
#line 693
        m_pCardInfoCtrl = new (__FILE__ "(693)") C2DSeqCtrl;
        m_pCardInfoCtrl->setLayer(11);
        m_pCardInfoCtrl->enableColorCtrl(TRUE);
        m_pCardInfoCtrl->addSeq(m_pSeqName, 0x35);
        C2DMan::getLayPosition(m_pLadName, 4, 2, vec, 0);
        m_pCardInfoCtrl->setPos(vec);
        C2DMan::getLayPosition(m_pLadName, 3, 0x1F, vec, 0);
        p2DSeqObj = m_pCardInfoCtrl->addSeqShift(m_pSeqName, 0x2D, vec.x, vec.y);
        p2DSeqObj->changeLayer(13);
    case 4:
        for (INT i = 0; i < 30; i++) {
            m_nMapCard[i] = 0;
        }
        for (INT i = 0; i < 3; i++) {
            for (INT j = mapKindTbl[i * 2]; j < mapKindTbl[i * 2 + 1]; j++) {
                for (INT k = 0; k < 10; k++) {
                    INT num = CCard::searchCardKind(j, k);
                    if (num > 0) {
                        m_nMapCard[i * 10 + k] += num;
                    }
                }
            }
        }
        for (INT i = 0; i < 30; i++) {
            char buf[16];
            if (!m_pNumButton[i]) {
#line 722
                pButton = m_pNumButton[i] = new (__FILE__ "(722)") CButton;
                C2DMan::getLayPosition(m_pLadName, 3, i + 1, vec, 0);
                sprintf(buf, pFmt, m_nMapCard[i]);
                pButton->init(m_pSeqName, 12, &numSeqTbl[(i / 10) * 5], vec, m_pCardInfoCtrl, buf, 14, 0.0f, 0.0f,
                              0x40, NULL, -1);
            } else {
                pButton = m_pNumButton[i];
                sprintf(buf, pFmt, m_nMapCard[i]);
                pButton->changeFont(buf);
            }
            pButton->setActive(m_nMapCard[i] != 0);
        }
        break;
    case 1:
        if (m_pCardInfoCtrl->isEnd(0)) {
            m_pCardInfoCtrl->replaceSeq(0, m_pSeqName, 0x36);
            ret = 1;
        }
        break;
    case 2:
        m_pCardInfoCtrl->replaceSeq(0, m_pSeqName, 0x37);
        break;
    }
    return ret;
}
// SOURCE DATA: .data .rodata
