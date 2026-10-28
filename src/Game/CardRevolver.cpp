#define GAME_CARDREVOLVER_CPP
#include "khtypes.h"
#define GPRIM_H_PATH "System/Graphic/GPrim.h" /* MATCH: the original includes it under this name */
#define TECHNIC_H_PATH "c:/KHCOM/MainApp/Game/Technic.h" /* MATCH: the original includes it under this name */
#define CARDBATTLE_H_PATH "c:/KHCOM/MainApp/Game/CardBattle.h" /* MATCH: the original includes it under this name */
#include "System/Task.h"
#include "System/Graphic/Gprim.h"
#include "Game/Technic.h"
#include "System/Graphic/2d/2DMan.h"
#include "Game/CardBattle.h"
#include "System/Graphic/Anim.h"
#include "System/Graphic/Str.h"
#include "Game/Collision/CollUtil.h"
/* MATCH: placeholder: an empty string follows CollUtil.h's strings in .rodata; which header it comes from is not known */
inline void _emptyStringPlaceholderCardRevolver() { TRACE(""); }
#include "Game/CardRevolver.h"
#include "System/TaskMan.h"
#include "System/Sound/SoundMan.h"
#include "Game/GameWork.h"
#include "Game/Player/Player.h"
#include "Game/Player/PlayerManager.h"
#include "System/Debug.h"
#include "System/HeapMan.h"
#include "Game/Debug/DebugWork.h"
#include "Game/Prize.h"
#include "System/Math3D.h"
#include "System/Coord.h"
#include "Game/Game.h"
#include "Game/SceneInfo.h"
#include "include_asm.h"

using namespace CardBattle;

inline void* operator new(unsigned int, void* p) throw() { return p; }
#define RET_ADDR() ({ UINT nRA; __asm__ volatile("daddu %0, $31, $0" : "=r"(nRA)); nRA; })
inline void* operator new(size_t size, char* msg) throw() { return com_malloc_d(size, msg, RET_ADDR()); }
inline void* operator new[](size_t size, char* msg) throw() { return com_malloc_d(size, msg, RET_ADDR()); }

namespace CardBattle {
static LPCSTR g_szLadCockpit = "Cockpit.lad";
static LPCSTR g_szSeqCockpit = "Cockpit:00";
BTL_DECK g_deckSoraTutorial[17] = {
    {1, 4}, {1, 5}, {1, 6}, {1, 1}, {1, 3}, {1, 9}, {1, 0}, {1, 2},
    {1, 7}, {1, 7}, {1, 6}, {1, 5}, {1, 4}, {1, 5}, {1, 6}, {68, -1},
    {255, 0},
};
BTL_DECK g_deckSoraStarter[16] = {
    {1, 7}, {1, 6}, {1, 5}, {1, 5}, {1, 4}, {1, 3}, {1, 4}, {1, 3},
    {1, 2}, {1, 2}, {1, 1}, {1, 0}, {27, 5}, {42, 6}, {29, 5}, {255, 0},
};
BTL_DECK g_deckRikuStarter[42] = {
    {18, 8}, {18, 8}, {18, 8}, {18, 7}, {18, 7}, {18, 7}, {18, 6}, {18, 6},
    {18, 6}, {18, 9}, {18, 9}, {18, 9}, {18, 6}, {18, 7}, {18, 8}, {18, 9},
    {18, 8}, {18, 6}, {18, 7}, {18, 9}, {18, 9}, {18, 6}, {18, 7}, {18, 8},
    {43, 9}, {43, 0}, {18, 0}, {18, 0}, {18, 0}, {18, 0}, {99, -1}, {100, -1},
    {101, -1}, {102, -1}, {104, -1}, {105, -1}, {106, -1}, {107, -1}, {108, -1}, {109, -1},
    {115, -1}, {255, 0},
};
BTL_DECK g_deckSoraDebug[32] = {
    {1, 7}, {1, 6}, {1, 5}, {1, 5}, {1, 4}, {1, 3}, {1, 4}, {1, 3},
    {1, 2}, {1, 2}, {1, 1}, {1, 0}, {27, 5}, {42, 6}, {29, 5}, {72, -1},
    {106, -1}, {35, 9}, {36, 9}, {37, 9}, {38, 9}, {39, 9}, {40, 9}, {41, 9},
    {68, -1}, {75, -1}, {82, -1}, {83, -1}, {87, -1}, {101, -1}, {94, -1}, {255, 0},
};
BTL_DECK g_deckSoraDebug99[100] = {
    {1, 0}, {1, 1}, {1, 2}, {1, 3}, {1, 4}, {1, 5}, {1, 6}, {1, 7},
    {1, 8}, {1, 9}, {2, 0}, {2, 1}, {2, 2}, {2, 3}, {2, 4}, {2, 5},
    {2, 6}, {2, 7}, {2, 8}, {2, 9}, {3, 0}, {3, 1}, {3, 2}, {3, 3},
    {3, 4}, {3, 5}, {3, 6}, {3, 7}, {3, 8}, {3, 9}, {4, 0}, {4, 1},
    {4, 2}, {4, 3}, {4, 4}, {4, 5}, {4, 6}, {4, 7}, {4, 8}, {4, 9},
    {5, 0}, {5, 1}, {5, 2}, {5, 3}, {5, 4}, {5, 5}, {5, 6}, {5, 7},
    {5, 8}, {5, 9}, {6, 0}, {6, 1}, {6, 2}, {6, 3}, {6, 4}, {6, 5},
    {6, 6}, {6, 7}, {6, 8}, {6, 9}, {7, 0}, {7, 1}, {7, 2}, {7, 3},
    {7, 4}, {7, 5}, {7, 6}, {7, 7}, {7, 8}, {7, 9}, {8, 0}, {8, 1},
    {8, 2}, {8, 3}, {8, 4}, {8, 5}, {8, 6}, {8, 7}, {8, 8}, {8, 9},
    {9, 0}, {9, 1}, {9, 2}, {9, 3}, {9, 4}, {9, 5}, {9, 6}, {9, 7},
    {9, 8}, {9, 9}, {10, 0}, {10, 1}, {10, 2}, {10, 3}, {10, 4}, {10, 5},
    {10, 6}, {10, 7}, {10, 8}, {255, 0},
};
char* g_szDbgDeckName[10] = {
    "OFF (\x92\xca\x8f\xed)", "COM\x94\xc5\x83X\x83^\x81[\x83^", "\x83" "f\x83o\x83" "b\x83O\x97p", "99\x96\x87\x83" "e\x83X\x83g", "4:OFF", "5:OFF",
    "6:OFF", "7:OFF", "8:OFF", "9:OFF",
};
BTL_DECK g_deckSoraPoo[19] = {
    {25, 9}, {25, 0}, {25, 2}, {25, 6}, {25, 9}, {25, 0}, {25, 6}, {25, 2},
    {25, 0}, {33, 7}, {34, 6}, {34, 8}, {25, 0}, {25, 2}, {25, 9}, {25, 3},
    {25, 6}, {25, 8}, {255, 0},
};
static char* g_szStockSkillName[150] = {
    "", "", "", "", "Sliding Dash", "Blitz",
    "Stun Impact", "Zantetsuken", "Strike Raid", "Sonic Blade", "Ars Arcanum", "Ragnarok",
    "Trinity Limit", "Fira", "Blizzara", "Thundara", "Cura", "Gravira",
    "Stopra", "Aerora", "Firaga", "Blizzaga", "Thundaga", "Curaga",
    "Graviga", "Stopga", "Aeroga", "Fire Raid", "Blizzard Raid", "Thunder Raid",
    "Reflect Raid", "Judgment", "Firaga Burst", "Raging Storm", "Mega Flare", "Freeze",
    "Homing Blizzara", "Aqua Splash", "Magnet Spiral", "Lethal Flame", "Shock Impact", "Tornado",
    "Quake", "Warpinator", "Warp", "Bind", "Confuse", "Terror",
    "Synchro", "Gifted Miracle", "Teleport", "Holy", "Proud Roar LV2", "Proud Roar LV3",
    "Splash LV2", "Splash LV3", "Paradise LV2", "Paradise LV3", "Idyll Romp", "Flare Breath LV2",
    "Flare Breath LV3", "Showtime LV2", "Showtime LV3", "Twinkle LV2", "Twinkle LV3", "Cross-slash",
    "Omnislash", "Cross-slash+", "Magic LV2", "Magic LV3", "Stardust Blitz", "Goofy Tornado LV2",
    "Goofy Tornado LV3", "Goofy Smash", "Wild Crush", "Sandstorm LV2", "Sandstorm LV3", "Surprise! LV2",
    "Surprise! LV3", "Spiral Wave LV2", "Spiral Wave LV3", "Hummingbird LV2", "Hummingbird LV3", "Furious Volley LV2",
    "Furious Volley LV3", "Lucky Bounty LV2", "Lucky Bounty LV3", "Honey Storm", "Honey Pot", "Honey Pot",
    "High Jump", "Glide", "Super Glide", "DummyS00", "DummyS01", "Dark Break",
    "Dark Firaga", "Dark Aura", "MM Miracle LV2", "MM Miracle LV3", "Holy Burst", "Inverse Burst",
    "Impulse", "Maelstrom", "Barrage", "Dark Impulse", "Dark Maelstrom", "Dark Barrage",
    "DummyR00", "D Mode", "Nothing", "Cross-slash", "Omnislash", "Temper Flare",
    "Firagaball", "Combo & Present", "Barrel Blast", "Fire Wall", "Firetooth", "Teleport Rush",
    "Mega Volt", "Blade Storm", "Helm Split", "Dark Firaga", "Dark Aura", "Freeze",
    "Ice Needles", "Slide Break", "Diamond Dust", "Deathscythe", "Blossom Shower", "Drop Shot",
    "Omni Laser", "Circle Reject", "Doom", "Whirldwind to the Void", "Gale of Severance", "Rockshatter",
    "Impact Quake", "Ground Impact", "Shock Slash", "Dark Rush", "Dark Shadow", "Cyclone Snatch",
    "Dark Punisher", "Catastrophe", "DummyE00", "DummyE01", "DummyE02", "DummyE03",
};
char* g_szEnemyCardEffName[60] = {
    "", "Incrementor", "Combo Plus", "Guard", "Fire Boost", "Blizzard Boost",
    "Thunder Boost", "Cure Boost", "Retrograde", "Draw", "Reload Kinesis", "Combo Finish",
    "Back Attack", "Quickload", "Drain", "Random Values", "Decrementor", "Auto-Reload",
    "Float", "Vanish", "All Zeros", "Item Bracer", "Duel Trigger", "Protect",
    "Reload Lock", "Magic Boost", "Bio", "Hyper Healing", "Random Flush", "Leaf Bracer",
    "Reload Haste", "Summon Boost", "Wide Attack", "Dispel", "Value Break", "Mimic",
    "Attack Haste", "Berserk", "Attack Bracer", "Regen", "Shell", "Second Chance",
    "Overdrive", "Sleight Lock", "Quick Recovery", "Dash", "Auto-Life", "Double Sleight",
    "Warp Break", "Sleightblind", "Confu-Strike", "Quick Barrier", "Shot Charge", "Aero Guard",
    "Combo Boost", "Water Charge", "Omni Break", "Double Strike", "Premium Guard", "Damage Zero",
};
G_UV g_uvCRCounterNumOfs[4] = {{67, 30}, {52, 30}, {56, 20}, {37, 29}};
}
template <class T> static inline T& _ref(T& v) { return v; }
/* PARTIAL: stand-in for Game/CardBattle CCardBattle (m_cardEnchant, getEnchantKind): gated by GAME_CARDBATTLE_CPP */
struct CB_ENCHANT_VIEW {
    BYTE _pad[0x17C];
    CARD m_cardEnchant[2];
};
/* CCardBattle::getEnchantKind(INT inUser), an inline in Game/CardBattle.h */
inline INT CCardRevolver::_getEnchantKind(CCardBattle* p, INT inUser) {
    if (!inUser) AssertApp("c:/KHCOM/MainApp/Game/CardBattle.h", 641, "inUser", 0);
    return ((CB_ENCHANT_VIEW*)p)->m_cardEnchant[inUser - 1].m_nKind < 0x44 ? 0 : ((CB_ENCHANT_VIEW*)p)->m_cardEnchant[inUser - 1].m_nKind - 0x43;
}
static inline BOOL _isEqual(UINT a, UINT b) { return a == b; }
namespace D2 {
class C2DMan { /* PARTIAL: stand-in for System/Graphic/2d/2DMan C2DMan: gated by SYSTEM_GRAPHIC_2D_2DMAN_CPP */
public:
    static BOOL getLayPosition(const CHAR* pLadName, UINT inNo, UINT inIdx, FVECTOR& outPos, BOOL bLocal);
};
}
extern INT SCREEN_HEIGHT;
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
    c.m_nKind2 = kind;
    c.m_nNumber2 = num;
    c.m_nCategory20 = cardDataTbl[c.m_nKind].nCategory0;
    c.m_nCategory21 = cardDataTbl[c.m_nKind].nCategory1;
}

void CardBattle::convBtlDeckToCard(BTL_DECK* inDeck) {
    for (INT i = 0; i < 100; i++) {
        if (inDeck[i].m_nKind <= 0 || inDeck[i].m_nKind > 202) break;
        _setCard(*(CARD*)&inDeck[i], (BYTE)inDeck[i].m_nKind, inDeck[i].m_nNumber);
    }
}
CCardRevolver::CCardRevolver(CTask* pParent, CARD* inDeck, INT inUser, INT inShortCut) : CTask(pParent, 0x4000) {
    m_uiUser = inUser;
    m_hOwner = pParent->getHandle();
    _init();
    _initialize(inDeck, inShortCut);
    _initCounter();
    CCardBattle* pBattle = (CCardBattle*)CTaskMan::getSysTask(0x16);
    if (pBattle) pBattle->setRevolver(m_uiUser, this);
}
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Game/CardRevolver", _$_Q210CardBattle13CCardRevolver);
void CCardRevolver::_init() {
    INT i;
    m_pCards = NULL;
    m_pCardsE = NULL;
    m_bActive = FALSE;
    m_pActiveCard = NULL;
    m_uiFlag = 0;
    m_iState = 0;
    m_iMode = 0;
    for (i = 0; i < 2; i++) {
        m_nTotal[i] = 0;
        m_nRemain[i] = 0;
        m_nActive[i] = 0;
    }
    m_iChargeCount = 1;
    m_tqChargeRate = 100;
    m_tqRollSpeed = 300;
    m_nShortCutID = -1;
    m_nLimit = 0;
    m_nExtra = 0;
    m_nEnd = 0;
    m_tqTimer = 0;
    m_iCharge = 0;
    m_tqCharge = 0;
    m_iStock = 0;
    m_iStockSum = 0;
    m_iStockSkill = 0;
    m_iExtraSkill = 0;
    m_iFirstSkill = 0;
    m_hPlaySE = 0;
    m_hChargeSE = 0;
    m_iReloadSEID = 0;
    m_pEmpty = NULL;
    m_iAction = 0;
    m_nReloadCard = 0;
    m_nReloadID = 0;
    m_nAdd = 0;
    m_nReturn = 0;
    for (i = 0; i < 39; i++) {
        _setCard(m_cardAdd[i], 0, 0);
    }
    for (i = 0; i < 7; i++) {
        m_pCardsS[i] = NULL;
        m_pDataS[i] = NULL;
    }
    initStock();
    m_hSnatch = 0;
    m_nSnatchCount = 0;
    m_tqSnatch = 0;
    m_tqDiscard = 0;
    m_nPlutoMark = 0;
    m_nReloadCount = 0;
    m_iMax = 0;
    m_pEmpty = NULL;
}
void CCardRevolver::_initialize(CARD* inDeck, INT inShortCut) {
    switch (m_uiUser) {
    case 1:
        m_iReloadSEID = 0x12;
        if (inDeck) {
            if (CGame::isRikuGame()) m_uiFlag |= 0x80;
            else if (CSceneInfo::isMinigameScene()) m_uiFlag |= 0x800;
            setDeck(inDeck, inShortCut);
        } else if (CGame::isSoraGame()) {
            if (CGameWork::getFlag(1, 0x2C)) {
                convBtlDeckToCard(g_deckSoraTutorial);
                setDeck((CARD*)g_deckSoraTutorial, -1);
            } else {
                switch (DebugWork::g_dw.player.iUseDbgDeck) {
                case 3:
                    convBtlDeckToCard(g_deckSoraDebug99);
                    setDeck((CARD*)g_deckSoraDebug99, -1);
                    break;
                case 2:
                    convBtlDeckToCard(g_deckSoraDebug);
                    setDeck((CARD*)g_deckSoraDebug, -1);
                    break;
                case 0:
                default:
                    if (CSceneInfo::isMinigameScene()) {
                        convBtlDeckToCard(g_deckSoraPoo);
                        setDeck((CARD*)g_deckSoraPoo, -1);
                        m_uiFlag |= 0x800;
                    } else {
                        convBtlDeckToCard(g_deckSoraStarter);
                        setDeck((CARD*)g_deckSoraStarter, -1);
                    }
                    break;
                }
            }
        } else {
            m_uiFlag |= 0x80;
            convBtlDeckToCard(g_deckRikuStarter);
            setDeck((CARD*)g_deckRikuStarter, inShortCut);
        }
        break;
    case 2:
        m_iReloadSEID = 0x1B;
        if (inDeck) setDeck(inDeck, -1);
        break;
    }
    if (m_uiFlag & 1) return;
    CARD dummy;
    _setCard(dummy, 0xFF, 0);
    setDeck(&dummy, -1);
}
void CCardRevolver::_initCounter() {
    initCounter();
    m_pEmpty = new (com_malloc_d(sizeof(C2DCard), "c:/KHCOM/MainApp/Game/CardRevolver.cpp(810)", RET_ADDR())) C2DCard;
    m_pEmpty->setLayer(2);
    m_pEmpty->createEmpty();
    FVECTOR vPos;
    if (m_uiUser == 1) vPos.set(64.0f, (FLOAT)SCREEN_HEIGHT - 96.0f, 0.0f, 1.0f);
    else vPos.set(448.0f, (FLOAT)SCREEN_HEIGHT - 96.0f, 0.0f, 1.0f);
    m_pEmpty->setPos(vPos);
    m_pEmpty->enableDisp(FALSE);
}
void CCardRevolver::exeWait() {
    UINT flag = (m_uiUser == 1) ? 2 : 4;
    if (!(m_uiFlag & 0x40)) {
        INT i;
        CBattleCard* card;
        m_bActive = FALSE;
        m_tqTimer = 0;
        m_nRemain[0] = 0;
        m_iMax = 0;
        m_nReloadCount = 0;
        m_nActive[0] = 0;
        for (i = 0; i < m_nLimit; i++) {
            card = &m_pCards[i];
            if (!card) continue;
            if (card->isNowPlaying()) continue;
            if (card->isDuelStock()) continue;
            card->resetReloadSet();
            if (card->getHand() >= 0) {
                card->hideHand();
                card->outHand();
            }
            if (card->getHand() != -1) continue;
            if (card->getStock() != -1) continue;
            if (card->isReloadNG()) continue;
            if ((m_uiFlag & 0x100) && card->isDModeNG()) continue;
            m_iMax++;
            card->setReloadSet();
        }
        m_tqTimer = 400;
        m_uiFlag |= 0x40;
    }
    if (CGameWork::getInt(1, 0xE2) & flag) return;
    if (m_uiFlag & 0x40) {
        m_iState = 2;
        m_uiFlag = (INT)m_uiFlag & ~0x40;
        m_uiFlag = (INT)m_uiFlag & ~8;
        m_hPlaySE = CSound::playSE(m_iReloadSEID, -1, 0x40);
        INT j = m_nLimit - 1;
        for (INT i = 0; i < m_nLimit; i++, j--) {
            CBattleCard* card = &m_pCards[j];
            if (!card) continue;
            if (card->isReloadSet()) {
                card->resetReloadSet();
                m_nActive[0] = j;
                if (card->getCardData()->m_nCategory1 != 8) m_nRemain[0]++;
                card->resetHand(m_iMax - m_nRemain[0] - 1, m_iMax);
                m_nReloadCount++;
            } else {
                card->setStartIn();
            }
        }
    } else {
        m_iState = 1;
    }
}
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Game/CardRevolver", main__Q210CardBattle13CCardRevolver);
__asm__(".rdata\n.align 3\n.word 0x52434C50\n.word 0x3D3D203A\n.word 0x3D3D3D3D\n.word 0x3D3D3D3D\n.word 0x3D3D3D3D\n.word 0x3D3D3D3D\n.word 0x3D3D3D3D\n.word 0x3D3D3D3D\n.word 0x3D3D3D3D\n.word 0x3D3D3D3D\n.word 0x3D3D3D3D\n.word 0x000A3D3D\n.word 0x3A766552\n.word 0x646F4D20\n.word 0x203D2065\n.word 0x45444F4D\n.word 0x524F4E5F\n.word 0x0A4C414D\n.word 0x00000000\n.word 0x00000000\n.word 0x3A766552\n.word 0x6F546E20\n.word 0x206C6174\n.word 0x6425203D\n.word 0x526E202C\n.word 0x69616D65\n.word 0x203D206E\n.word 0x202C6425\n.word 0x7463416E\n.word 0x20657669\n.word 0x6425203D\n.word 0x00000A2C\n.word 0x3A766552\n.word 0x694C6E20\n.word 0x2074696D\n.word 0x6425203D\n.word 0x456E202C\n.word 0x61727478\n.word 0x25203D20\n.word 0x69202C64\n.word 0x2078614D\n.word 0x6425203D\n.word 0x0000000A\n.word 0x00000000\n.word 0x3A766552\n.word 0x63416220\n.word 0x65766974\n.word 0x25203D20\n.word 0x74202C64\n.word 0x6D695471\n.word 0x3D207265\n.word 0x2C642520\n.word 0x46697520\n.word 0x2067616C\n.word 0x3025203D\n.word 0x0A2C7838\n.word 0x00000000\n.word 0x00000000\n.word 0x52434C50\n.word 0x6143203A\n.word 0x2D736472\n.word 0x2D2D2D2D\n.word 0x2D2D2D2D\n.word 0x2D2D2D2D\n.word 0x2D2D2D2D\n.word 0x2D2D2D2D\n.word 0x2D2D2D2D\n.word 0x2D2D2D2D\n.word 0x0A2D2D2D\n.word 0x00000000\n.word 0x3A643325\n.word 0x61435B20\n.word 0x30256472\n.word 0x253A6433\n.word 0x64252864\n.word 0x69205D29\n.word 0x636F7453\n.word 0x64253D6B\n.word 0x4869202C\n.word 0x3D646E61\n.word 0x252F6425\n.word 0x75202C64\n.word 0x616C4669\n.word 0x30253D67\n.word 0x202C7838\n.word 0x774F6975\n.word 0x3D72656E\n.word 0x0A2C6425\n.word 0x00000000\n.word 0x00000000\n.word 0x3A643325\n.word 0x4C554E20\n.word 0x000A2C4C\n.word 0x00000000\n.word 0x52434E45\n.word 0x3D3D203A\n.word 0x3D3D3D3D\n.word 0x3D3D3D3D\n.word 0x3D3D3D3D\n.word 0x3D3D3D3D\n.word 0x3D3D3D3D\n.word 0x3D3D3D3D\n.word 0x3D3D3D3D\n.word 0x3D3D3D3D\n.word 0x3D3D3D3D\n.word 0x000A3D3D\n.word 0x52434E45\n.word 0x6143203A\n.word 0x2D736472\n.word 0x2D2D2D2D\n.word 0x2D2D2D2D\n.word 0x2D2D2D2D\n.word 0x2D2D2D2D\n.word 0x2D2D2D2D\n.word 0x2D2D2D2D\n.word 0x2D2D2D2D\n.word 0x0A2D2D2D\n.word 0x00000000\n.word 0x3A643325\n.word 0x61435B20\n.word 0x30256472\n.word 0x253A6433\n.word 0x69205D64\n.word 0x636F7453\n.word 0x64253D6B\n.word 0x4869202C\n.word 0x3D646E61\n.word 0x252F6425\n.word 0x75202C64\n.word 0x616C4669\n.word 0x30253D67\n.word 0x202C7838\n.word 0x774F6975\n.word 0x3D72656E\n.word 0x0A2C6425\n.word 0x00000000\n.word 0x001E2810\n.word 0x001E2820\n.word 0x001E2888\n.word 0x001E2898\n.word 0x001E28A8\n.word 0x00000000\n.text\n");
void CCardRevolver::exeMain() {
    if (DebugWork::g_dw.player.iDeckDbgDisp) {
        if (!(m_uiFlag & 0x10)) rollDeck(0, 0, 300);
        m_uiFlag |= 0x10;
    } else {
        if (m_uiFlag & 0x10) rollDeck(0, 0, 300);
        m_uiFlag = (INT)m_uiFlag & ~0x10;
    }
    INT mode = m_iMode;
    INT nActive = m_nActive[mode];
    INT nRemain = m_nRemain[mode];
    CBattleCard* pCards;
    if (mode == 0) {
        nRemain += m_nReloadCard;
        pCards = m_pCards;
    } else {
        pCards = m_pCardsE;
    }
    if (pCards) {
        if (nRemain > 0) m_bActive = pCards[nActive].isNowSelect();
        else m_bActive = TRUE;
    } else {
        changeMode(0, 1);
        return;
    }
    if (m_uiUser == 1) {
        if (m_bActive) {
            CBattleCard* pCard = &pCards[nActive];
            pCard->setSelect(1);
            pCard->setEnchantEffect(0x52, 1);
            if (m_pActiveCard && m_pActiveCard != pCard) {
                m_pActiveCard->setSelect(0);
                m_pActiveCard->resetEnchantEffect(0x52);
            }
            m_pActiveCard = &pCards[nActive];
        } else {
            if (m_pActiveCard) {
                m_pActiveCard->setSelect(0);
                m_pActiveCard->resetEnchantEffect(0x52);
            }
            m_pActiveCard = NULL;
        }
    } else {
        if (m_bActive) {
            CBattleCard* pCard = &pCards[nActive];
            pCard->setEnchantEffect(0x52, 1);
            if (m_pActiveCard && m_pActiveCard != pCard) m_pActiveCard->resetEnchantEffect(0x52);
            m_pActiveCard = &pCards[nActive];
        } else {
            if (m_pActiveCard) m_pActiveCard->resetEnchantEffect(0x52);
            m_pActiveCard = NULL;
        }
    }
    if (!m_bActive) return;
    if (m_nReloadCard <= 0) return;
    if (DebugWork::g_dw.player.iAutoReload && m_iMode == 0 && nRemain == m_nReloadCard && m_iStock == 0) {
        initReload(0);
        return;
    }
    CCardBattle* pBattle = (CCardBattle*)CTaskMan::getSysTask(0x16);
    if (_getEnchantKind(pBattle, m_uiUser) != 0x11) return;
    if (pBattle->isExecutingCard(m_uiUser)) return;
    if (m_iMode != 0) return;
    if (nRemain != m_nReloadCard) return;
    initReload(0);
}
void CCardRevolver::destroy() {
    initHide();
}
void CCardRevolver::initHide() {
    if (m_iState == 4) return;
    m_iState = 4;
    CBattleCard* pCards = m_iMode == 0 ? m_pCards : m_pCardsE;
    INT n = m_iMode == 0 ? m_nLimit : m_nTotal[m_iMode];
    INT i;
    for (i = 0; i < n; i++) {
        CBattleCard* pCard = &pCards[i];
        if (!pCard) continue;
        if (pCard->getHand() != -1) pCard->hideHand();
        if (pCard->getStock() != -1) pCard->destroy(1);
    }
    for (i = 0; i < m_iStock; i++) {
        m_pCardsS[i] = NULL;
        m_pDataS[i] = NULL;
    }
    m_iStock = 0;
    m_iStockSum = 0;
    m_iStockSkill = 0;
    m_iExtraSkill = 0;
    m_iFirstSkill = 0;
    m_uiStockState = 0;
    m_uiStockCategory = 0;
    m_pSeqCtrlStock->enableDisp(FALSE);
    m_uiStockFlag = (INT)m_uiStockFlag & ~1;
    m_pSeqCtrlStock_Num01->enableDisp(0, FALSE);
    m_pSeqCtrlStock_Num10->enableDisp(0, FALSE);
    destroyCounter();
    if (m_pEmpty) m_pEmpty->enableDisp(FALSE);
    if (m_hPlaySE > 0) {
        CSound::stopSE(m_hPlaySE, m_iReloadSEID);
        m_hPlaySE = 0;
    }
    if (m_hChargeSE > 0) {
        CSound::stopSE(m_hChargeSE, 0x13);
        m_hChargeSE = 0;
    }
}
inline void operator delete[](void* mem) { com_free(mem); }
void CCardRevolver::setDeck(CARD* pDeck, INT inShortCut) {
    INT i;
    CARD card;
    INT snatch;
    m_nRemain[0] = m_nRemain[1] = m_nActive[0] = m_nActive[1] = 0;
    delete[] m_pCards;
    m_pCards = NULL;
    delete[] m_pCardsE;
    m_pCardsE = NULL;
    for (i = 0; i < 99 && (card = pDeck[i], card.m_nKind && card.m_nKind <= 0xCA); i++) {
        if (card.m_nCategory1 == 7 && card.m_nNumber == -1) m_nRemain[1]++;
        else m_nRemain[0]++;
    }
    m_nTotal[0] = m_nRemain[0] + 1;
    m_nTotal[1] = m_nRemain[1];
    if (m_uiUser == 1) {
        m_nLimit = m_nTotal[0] + 9;
    } else {
        snatch = 0;
        PLM::CPlayerManager* pPLM = (PLM::CPlayerManager*)CTaskMan::getSysTask(0x1F);
        if (pPLM && pPLM->isZexionBattle()) {
            m_uiFlag |= 0x2000;
            snatch = 0x18;
            TRACE("ENCR:: (ZEXION MODE) snatchCard = %d\n", 0x18);
        }
        m_nLimit = m_nTotal[0] + snatch;
    }
    if (m_nTotal[0] > 0) m_pCards = new ("c:/KHCOM/MainApp/Game/CardRevolver.cpp(1359)") CBattleCard[m_nLimit](this, 0, -1, m_uiUser, 2);
    if (m_nTotal[1] > 0) m_pCardsE = new ("c:/KHCOM/MainApp/Game/CardRevolver.cpp(1363)") CBattleCard[m_nTotal[1]](this, 0, -1, m_uiUser, 2);
    m_nRemain[1] = 0;
    m_nRemain[0] = 0;
    for (i = 0; i < 99 && (card = pDeck[i], card.m_nKind && card.m_nKind <= 0xCA); i++) {
        if (card.m_nCategory1 == 7 && card.m_nNumber == -1) {
            m_pCardsE[m_nRemain[1]].setHand(card, m_nRemain[1], m_nTotal[1], 0);
            m_pCardsE[m_nRemain[1]].hideHand();
            m_nRemain[1]++;
        } else {
            m_pCards[m_nRemain[0]].setHand(card, m_nRemain[0], m_nTotal[0], 0);
            if (inShortCut == i) m_nShortCutID = m_nRemain[0];
            m_nRemain[0]++;
        }
    }
    if (m_uiFlag & 0x80) m_iChargeCount = -1;
    else m_iChargeCount = 1;
    _setCard(card, 0xA0, m_iChargeCount);
    m_pCards[m_nRemain[0]].setHand(card, m_nRemain[0], m_nTotal[0], 0);
    m_iCharge = 0;
    m_tqCharge = 0;
    m_nReloadID = m_nRemain[0];
    m_nReloadCard = 1;
    m_uiFlag |= 1;
    rollDeck(0, 1, 300);
    if (m_nTotal[1] == 0) {
        m_pCardsE = new ("c:/KHCOM/MainApp/Game/CardRevolver.cpp(1417)") CBattleCard[1](this, 0, -1, m_uiUser, 2);
        m_pCardsE->setHand(card, 0, 1, 0);
        m_pCardsE->hideHand();
    }
}
/* Parked: 5 lines off, the direction `a`: the original computes it as li t3,1 / slt v0,v1,a1 / movz t3,v1,v0
   with v1 = -1 set in both predecessors of the join, then negates num with bltzl; ours branches (bgez). Worked:
   one `i` for all three loops (s0), !m_iMode-first rc/pCards select, hand++ after setHand, if/else direction.
   Tried: 11 forms of a/abs (ternaries, -1 < num, a = -1 before the rc block); permuter 5 min (5534, no gain). */
#if 0
void CCardRevolver::rollDeck(INT inNum, INT inFlag, TIMEQ inTime) {
    INT iRemain;
    INT i;
    INT nLimit;
    INT iEnd;
    INT iMax;
    CBattleCard* pCards;
    INT rc;
    INT num;
    INT target;
    INT n;
    INT a;
    INT loop;
    INT hand;
    iRemain = m_nRemain[m_iMode];
    if (!inFlag) {
        if (CGameWork::getFlag(1, 9)) return;
        if (m_iState != 1) return;
        if (!m_bActive) return;
        if (!inTime) return;
        if (m_iMode == 0) {
            if (m_nTotal[0] == m_nReloadCard) return;
        } else {
            if (m_nTotal[m_iMode] == 0) return;
        }
        if (m_uiUser == 1) {
            if (m_iMode == 0) {
                if (inTime == 100 && m_pCards[m_nActive[0]].getCardCategory() == 8) return;
                if (inNum && iRemain > 0) {
                    CSound::playSE(0xE, -1, 0x40);
                    CGameWork::setFlag(1, 0x17, 1);
                }
            } else {
                if (inNum && iRemain >= 2) {
                    CSound::playSE(0xE, -1, 0x40);
                    CGameWork::setFlag(1, 0x17, 1);
                }
            }
        }
    }
    if (m_iMode == 0) {
        nLimit = m_nLimit;
        iMax = m_nRemain[0] + m_nReloadCard;
        for (i = nLimit - 1; i >= 0; i--) {
            CBattleCard* pCard = &m_pCards[i];
            if (!pCard) continue;
            if (pCard->getHand() == -1) continue;
            m_nEnd = i;
            break;
        }
        iEnd = m_nEnd + 1;
        if (!m_nReloadCard && iMax == 1) m_nActive[m_iMode] = m_nEnd;
    } else {
        nLimit = m_nTotal[m_iMode];
        iMax = m_nRemain[m_iMode];
        iEnd = nLimit;
    }
    m_bActive = FALSE;
    m_tqTimer = 0;
    SHORT& iActive = m_nActive[m_iMode];
    if (!iMax && m_nReloadCard > 0) return;
    if (!m_iMode) {
        rc = m_nReloadCard;
        pCards = m_pCards;
    } else {
        pCards = m_pCardsE;
        rc = 0;
    }
    num = inNum;
    target = iActive;
    if (!rc && !iMax) {
        num = 0;
        if (!m_iMode) target = m_nReloadID;
    }
    if (num < 0) {
        a = -1;
        num = -num;
    } else {
        a = 1;
    }
    loop = 0;
    while (num > 0) {
        for (i = 0; i < nLimit; i++) {
            target += a;
            if (target < 0) target = iEnd - 1;
            else if (target >= iEnd) target = 0;
            CBattleCard* pCard = &pCards[target];
            if (!pCard) continue;
            if (pCard->getHand() == -1) continue;
            if (pCard->getCardOwner() == 1 || pCard->getCardOwner() == m_uiUser) {
                num--;
                break;
            }
        }
        if (++loop > nLimit) return;
    }
    iActive = target;
    hand = 0;
    m_tqRollSpeed = inTime;
    for (i = 0; i < nLimit; i++) {
        CBattleCard* pCard = &pCards[(iActive + i) % nLimit];
        if (!pCard) continue;
        if (pCard->getHand() == -1) continue;
        pCard->setHand(*pCard->getCardData(), hand, iMax, 1);
        hand++;
    }
}
#else
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Game/CardRevolver", rollDeck__Q210CardBattle13CCardRevolveriii);
#endif
/* Parked: 12 lines off, all in `limit`: the original loads m_nTotal[m_iMode] (lhu + sll 16) above the m_iMode
   branch and sign-extends (sra 16) only in that arm (interblock-scheduled split extension); ours branches first
   and uses lh. Worked: `if (inMode == -1) ^= 1; else =`, `!old_mode ? m_pCards : m_pCardsE`, indexed
   pCards[i] loop (copy into the loop register). Tried for limit: ternary both ways, if/else, assignment +
   override, SHORT temporary (4 forms), (SHORT) cast. */
#if 0
void CCardRevolver::changeMode(INT inMode, INT inFlag) {
    INT old_mode;
    INT limit;
    CBattleCard* pCards;
    INT i;
    if (m_iState != 1) return;
    old_mode = m_iMode;
    if (!inFlag) {
        if (CGameWork::getFlag(1, 0xB)) return;
        if (old_mode == inMode) return;
        if (!m_bActive) return;
    }
    limit = m_iMode ? m_nTotal[m_iMode] : m_nLimit;
    pCards = !old_mode ? m_pCards : m_pCardsE;
    for (i = 0; i < limit; i++) {
        if (&pCards[i] && pCards[i].getHand() != -1) pCards[i].hideHand();
    }
    if (inMode == -1) m_iMode ^= 1;
    else m_iMode = inMode;
    rollDeck(0, 1, 300);
    if (!inFlag) CSound::playSE(3, -1, 0x40);
    if (m_iMode > 0 && m_nRemain[m_iMode] == 0) m_pEmpty->enableDisp(1);
    else m_pEmpty->enableDisp(0);
    if (m_iMode == 0) CGameWork::setFlag(1, 0x12, 1);
}
#else
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Game/CardRevolver", changeMode__Q210CardBattle13CCardRevolverii);
#endif
void CCardRevolver::rollShortCut(INT inMode) {
    if (m_iMode != 0) return;
    if (CGameWork::getFlag(1, 9)) return;
    if (m_iState != 1) return;
    if (!m_bActive) return;
    if (m_nTotal[0] < 2) return;
    INT id;
    switch (inMode) {
    case 1:
        id = m_nReloadID;
        break;
    case 2:
        id = m_nShortCutID;
        break;
    default:
        return;
    }
    if (id == -1) return;
    CBattleCard* p = &m_pCards[id];
    INT n = p->getHand();
    if (n <= 0) return;
    CSound::playSE(14, -1, 64);
    rollDeck(n, 1, 300);
}
CBattleCard* CCardRevolver::getActiveCard() const {
    if (!m_bActive) return NULL;
    INT mode = m_iMode;
    INT total = m_nTotal[mode];
    INT active = m_nActive[mode];
    INT remain = m_nRemain[mode];
    CBattleCard* pCards;
    if (mode != 0) {
        pCards = m_pCardsE;
    } else {
        remain += m_nReloadCard;
        pCards = m_pCards;
    }
    if (!pCards) return NULL;
    if (!total) return NULL;
    if (!remain) return NULL;
    CBattleCard* pCard = &pCards[active];
    if (pCard == NULL || !pCard->isNowSelect()) return NULL;
    return pCard;
}
CARD* CCardRevolver::getActiveCardData() {
    CBattleCard* pCard = getActiveCard();
    if (pCard) return pCard->getCardData();
    return NULL;
}
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Game/CardRevolver", setPlayCard__Q210CardBattle13CCardRevolverii);
__asm__(".rdata\n.align 3\n.word 0x72614343\n.word 0x76655264\n.word 0x65766C6F\n.word 0x733A3A72\n.word 0x6C507465\n.word 0x61437961\n.word 0x81206472\n.word 0x424370A6\n.word 0x73693E2D\n.word 0x79616C50\n.word 0x43676E69\n.word 0x53647261\n.word 0x646E6174\n.word 0x3D776F4E\n.word 0x214B4F3D\n.word 0x42437020\n.word 0x65723E2D\n.word 0x4E746573\n.word 0x6C50776F\n.word 0x6E697961\n.word 0x00000A67\n.word 0x00000000\n.word 0x00000000\n.word 0x00000000\n.word 0x001E3E10\n.word 0x001E3E64\n.word 0x001E3F44\n.word 0x001E3F44\n.word 0x001E3EC4\n.word 0x001E3E04\n.word 0x001E3D6C\n.word 0x001E3F44\n.word 0x001E3F44\n.word 0x001E3F44\n.word 0x001E3F44\n.word 0x001E3F44\n.word 0x001E3EC4\n.word 0x00000000\n.text\n");
INT CCardRevolver::isEnableCharge() {
    if (m_uiFlag & 0x10000) return 0;
    if (m_uiFlag & 2) return m_uiFlag & 4;
    m_uiFlag |= 2;
    if (m_iState != 1) return 0;
    CBattleCard* pCard = getActiveCard();
    if (!pCard) return 0;
    if ((pCard->getCardData()->m_nId & 0xF0000000) != 0x80000000) return 0;
    m_uiFlag |= 4;
    return 1;
}
INT CCardRevolver::chargeReloadCard(INT inMode) {
    if (CGameWork::getFlag(1, 8)) return -1;
    if (m_uiFlag & 0x10000) return -1;
    if (!isEnableCharge() && !inMode) return -1;
    if (m_uiFlag & 0x80) {
        if (!inMode) CSound::playSE(0x15, -1, 0x40);
        initReload(inMode);
        return 1;
    }
    m_tqCharge += getFrameRate() * m_tqChargeRate / 100;
    CBattleCard* card = getActiveCard();
    INT charge = m_tqCharge / 30;
    if (charge - m_iCharge * 100 >= 100) {
        if (m_iCharge > 0) {
            m_iCharge--;
            m_tqCharge = m_iCharge * 3000;
            charge = m_iCharge * 100;
            CSound::playSE(0x14, -1, 0x40);
        } else {
            CCardBattle* pCB = (CCardBattle*)CTaskMan::getSysTask(0x16);
            INT kind = _getEnchantKind(pCB, m_uiUser);
            if (kind != 0x18) {
                if (m_iChargeCount < 3) m_iChargeCount++;
                if (kind == 0x1E) m_iChargeCount = 1;
            }
            m_iCharge = m_iChargeCount - 1;
            m_tqCharge = m_iCharge * 3000;
            card->updateReload(m_iCharge * 100);
            CSound::playSE(0x15, -1, 0x40);
            if (kind == 0xD) {
                setAction(0x50, 0);
                return 1;
            }
            initReload(0);
            return 1;
        }
    }
    card->updateReload(charge);
    m_uiFlag |= 0x20;
    if (!m_hChargeSE) m_hChargeSE = CSound::playSE(0x13, -1, 0x40);
    return 0;
}
void CCardRevolver::initReload(INT inMode) {
    TRACE("CCardRevolver::initReload(%d);\n", inMode);
    m_iState = 2;
    m_bActive = FALSE;
    m_tqTimer = 0;
    m_nRemain[0] = 0;
    m_iMax = 0;
    m_nActive[0] = 0;
    m_nReloadCount = 0;
    for (INT i = 0; i < m_nLimit; i++) {
        CBattleCard* pCard = &m_pCards[i];
        if (!pCard) continue;
        if (pCard->isNowPlaying()) continue;
        if (pCard->isDuelStock()) continue;
        pCard->resetReloadSet();
        if (pCard->getHand() >= 0) {
            pCard->hideHand();
            pCard->outHand();
        }
        if (pCard->getHand() != -1) continue;
        if (pCard->getStock() != -1) continue;
        if (pCard->isReloadNG()) continue;
        if ((m_uiFlag & 0x100) && pCard->isDModeNG()) continue;
        pCard->setReloadSet();
        m_iMax++;
    }
    m_tqTimer = 400;
    m_uiFlag = (INT)m_uiFlag & ~8;
    if (m_hPlaySE > 0) {
        CSound::stopSE(m_hPlaySE, m_iReloadSEID);
        m_hPlaySE = 0;
    }
    m_hPlaySE = CSound::playSE(m_iReloadSEID, -1, 64);
    if (inMode != 2) CCardBattle::decEnchantTime(m_uiUser, 0x29, 0);
    m_uiFlag &= ~0x20000;
}
/* Parked: 14 lines off: register allocation and latch order of the first loop - the original keeps i in t0 and
   the card offset in a2 (decremented first in the latch, in the continue paths' delay slots), ours the reverse;
   the second loop's i likewise. Worked (from 48 bytes over to same size): separate `m_uiFlag & 8` / `& 0x20000`
   ifs, switch with `case 8: break;` first and case 4 falling into default, `card = NULL` in the for-increment,
   the m_uiFlag &= ~0x20000 block placed after the first return (goto notyet out of the second loop), indexed
   second loop. Tried: all 6 increment orders, pointer second loop; permuter 5 min (5990 candidates, no gain). */
#if 0
extern "C" char D_0047AE98[]; /* "CCardRevolver::exeReload(); RELOAD end.\n" */
extern "C" char D_0047AEC8[]; /* "CCardRevolver::exeReload(); RELOAD complete!\n" */
void CCardRevolver::exeReload() {
    CBattleCard* card;
    INT n;
    INT i;
    INT category;
    m_tqTimer -= getFrameRate();
    if (m_tqTimer > 0) return;
    if (m_uiFlag & 8) {
        if (m_uiFlag & 0x20000) {
            rollDeck(0, 1, 300);
            m_tqTimer = 0;
            m_iState = 1;
            CGameWork::setFlag(1, 0x13, 1);
            TRACE(D_0047AE98);
            return;
        }
    }
    if (0) {
    notyet:
        m_uiFlag &= ~0x20000;
        goto check;
    }
    m_tqTimer = 200;
    card = NULL;
    n = m_nLimit - 1;
    for (i = 0; i < m_nLimit; i++, n--, card = NULL) {
        card = &m_pCards[n];
        if (!card) continue;
        if (!card->isReloadSet()) continue;
        card->resetReloadSet();
        category = card->getCardData()->m_nCategory1;
        switch (category) {
        case 8:
            break;
        case 4:
            if (m_uiFlag & 0x100) continue;
        default:
            m_nRemain[0]++;
            break;
        }
        m_nActive[0] = n;
        m_nReloadCount++;
        break;
    }
    if (card) card->resetHand(m_iMax - m_nReloadCount, m_iMax);
    m_uiFlag |= 0x20000;
    for (i = 0; i < m_nLimit; i++) {
        card = &m_pCards[i];
        if (card && card->isReloadSet()) goto notyet;
    }
check:
    if (m_uiFlag & 0x20000) {
        m_tqTimer = 400;
        TRACE(D_0047AEC8);
    }
    m_uiFlag |= 8;
}
#else
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Game/CardRevolver", exeReload__Q210CardBattle13CCardRevolver);
#endif
__asm__(".rdata\n.align 3\n.word 0x72614343\n.word 0x76655264\n.word 0x65766C6F\n.word 0x653A3A72\n.word 0x65526578\n.word 0x64616F6C\n.word 0x203B2928\n.word 0x4F4C4552\n.word 0x65204441\n.word 0x0A2E646E\n.word 0x00000000\n.word 0x00000000\n.word 0x72614343\n.word 0x76655264\n.word 0x65766C6F\n.word 0x653A3A72\n.word 0x65526578\n.word 0x64616F6C\n.word 0x203B2928\n.word 0x4F4C4552\n.word 0x63204441\n.word 0x6C706D6F\n.word 0x21657465\n.word 0x0000000A\n.word 0x00000000\n.word 0x00000000\n.text\n");
INT CCardRevolver::getRemain(INT inMode, INT inKind, INT inCategory) {
    if (inMode < 0) return m_nRemain[m_iMode];
    if (inKind == 0 && inCategory == 0) return m_nRemain[inMode];
    INT total = inMode == 0 ? m_nLimit : m_nTotal[inMode];
    INT n = 0;
    CBattleCard* pCards = inMode == 0 ? m_pCards : m_pCardsE;
    INT i;
    if (inKind) {
        for (i = 0; i < total; i++) {
            CBattleCard* pCard = &pCards[i];
            if (!pCard) continue;
            if (pCard->getHand() == -1) continue;
            if (pCard->getStock() != -1) continue;
            if (pCard->getCardKind() == inKind) n++;
        }
    } else if (inCategory) {
        for (i = 0; i < total; i++) {
            CBattleCard* pCard = &pCards[i];
            if (!pCard) continue;
            if (pCard->getHand() == -1) continue;
            if (pCard->getStock() != -1) continue;
            if (pCard->getCardCategory() == inCategory) n++;
        }
    }
    return n;
}
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Game/CardRevolver", setStockCard__Q210CardBattle13CCardRevolver);
__asm__(".rdata\n.align 4\n.word 0x001E4C70\n.word 0x001E4CC4\n.word 0x001E4D1C\n.word 0x001E4D1C\n.word 0x001E4DBC\n.word 0x001E4C5C\n.word 0x001E4DBC\n.word 0x001E4D1C\n.word 0x001E4D1C\n.word 0x001E4D1C\n.word 0x001E4D1C\n.word 0x001E4D1C\n.word 0x001E4DBC\n.word 0x00000000\n.text\n");
INT CCardRevolver::returnStockCard(INT inFlag) {
    INT nStock;
    BOOL bDestroy;
    INT i;
    if (((CCardBattle*)CTaskMan::getSysTask(0x16))->isNowDuel()) {
        TRACE("CR::returnStockCard NowDuel NG!\n");
        return 0;
    }
    if (!m_nReloadCard) {
        TRACE("CR::returnStockCard Now Reload Lost NG!\n");
        return 0;
    }
    nStock = getStock(1);
    if (!nStock && !m_iStock) return 0;
    bDestroy = FALSE;
    if (!inFlag) {
        if (m_iState != 1) return 0;
        if (m_iMode) return 0;
        if (nStock != m_iStock) return 0;
        if (m_nRemain[0] > 0 && !getActiveCard()) return 0;
        if (CCardBattle::isExecutingCard(m_uiUser)) return 0;
    } else {
        if (m_iState != 1) bDestroy = TRUE;
        if (m_iMode) bDestroy = TRUE;
        if (nStock != m_iStock) bDestroy = TRUE;
        if (m_nRemain[0] > 0 && !m_iMode && !getActiveCard()) bDestroy = TRUE;
        if (CCardBattle::isExecutingCard(m_uiUser)) bDestroy = TRUE;
    }
    m_uiFlag = (INT)m_uiFlag & ~0x200;
    if (bDestroy) {
        TRACE("CR::returnStockCard DESTROY\n");
        for (i = 0; i < 7; i++) {
            if (m_pCardsS[i]) m_pCardsS[i]->destroy(0);
            m_pCardsS[i] = NULL;
            m_pDataS[i] = NULL;
        }
    } else {
        TRACE("CR::returnStockCard RETURN\n");
        for (i = 0; i < 7; i++) {
            if (m_pCardsS[i]) m_pCardsS[i]->resetHand(++m_nRemain[0], m_nTotal[0]);
            m_pCardsS[i] = NULL;
            m_pDataS[i] = NULL;
        }
        m_iState = 3;
        m_tqTimer = 400;
        m_iMax = m_nRemain[0] + m_nReloadCard;
        m_uiFlag = (INT)m_uiFlag & ~8;
        m_bActive = FALSE;
    }
    m_iStock = 0;
    m_iStockSum = 0;
    m_iStockSkill = 0;
    m_iExtraSkill = 0;
    m_iFirstSkill = 0;
    return 1;
}
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Game/CardRevolver", calcStockSkill__Q210CardBattle13CCardRevolver);
__asm__(".rdata\n.align 4\n.word 0x001E511C\n.word 0x001E5124\n.word 0x001E5130\n.word 0x001E5130\n.word 0x001E5130\n.word 0x00000000\n.word 0x00000000\n.word 0x00000000\n.word 0x001E5684\n.word 0x001E568C\n.word 0x001E5694\n.word 0x001E56D8\n.word 0x001E56D8\n.word 0x001E569C\n.word 0x001E56A4\n.word 0x001E56D8\n.word 0x001E56D8\n.word 0x001E56B4\n.word 0x001E56D8\n.word 0x001E56BC\n.word 0x001E56D8\n.word 0x001E56D8\n.word 0x001E56D8\n.word 0x001E56AC\n.word 0x001E56D8\n.word 0x001E56D8\n.word 0x001E56D8\n.word 0x001E56D8\n.word 0x001E56D8\n.word 0x001E56D8\n.word 0x001E56D8\n.word 0x001E56D8\n.word 0x001E56D8\n.word 0x001E56D8\n.word 0x001E56D8\n.word 0x001E56D8\n.word 0x001E56D8\n.word 0x001E56D8\n.word 0x001E56C4\n.word 0x00000000\n.text\n");
INT CCardRevolver::calcEnemyStockSkill() {
    UINT uiKinds = 0;
    INT nSum = 0;
    m_iStockSkill = m_iStock;
    for (INT i = 0; i < m_iStock; i++) {
        uiKinds |= m_pDataS[i]->m_nKind << (i * 8);
        nSum += m_pDataS[i]->m_nNumber;
    }
    switch (uiKinds) {
    case 0x818181: m_iStockSkill = 0x6F; break;
    case 0x828282: m_iStockSkill = 0x70; break;
    case 0x838383: m_iStockSkill = 0x71; break;
    case 0x848484: m_iStockSkill = 0x72; break;
    case 0x878585: m_iStockSkill = 0x73; break;
    case 0x878686: m_iStockSkill = 0x74; break;
    case 0x898989: m_iStockSkill = 0x75; break;
    case 0x8A8A8A: m_iStockSkill = 0x76; break;
    case 0x8B8C8C: m_iStockSkill = 0x77; break;
    case 0x8D8D8C: m_iStockSkill = 0x78; break;
    case 0x8D8D8D: m_iStockSkill = 0x79; break;
    case 0x908E8E: m_iStockSkill = 0x7A; break;
    case 0x919191: m_iStockSkill = 0x7B; break;
    case 0x918F8F: m_iStockSkill = 0x7C; break;
    case 0x929392: m_iStockSkill = 0x7D; break;
    case 0x939393: m_iStockSkill = 0x7E; break;
    case 0x929393: m_iStockSkill = 0x7F; break;
    case 0x939392: m_iStockSkill = 0x80; break;
    case 0x959694: m_iStockSkill = 0x81; break;
    case 0x969496: m_iStockSkill = 0x82; break;
    case 0x959495: m_iStockSkill = 0x83; break;
    case 0x989898:
        if (nSum == 12) m_iStockSkill = 0x84;
        else if (nSum == 15) m_iStockSkill = 0x85;
        else if (nSum == 18) m_iStockSkill = 0x86;
        else if (nSum == 21) m_iStockSkill = 0x87;
        else if (nSum == 24) m_iStockSkill = 0x88;
        else m_iStockSkill = 0x6E;
        break;
    case 0x9B999A: m_iStockSkill = 0x89; break;
    case 0x999B9A: m_iStockSkill = 0x8A; break;
    case 0x9B9A99: m_iStockSkill = 0x8B; break;
    case 0x9C9D9D: m_iStockSkill = 0x8C; break;
    case 0x9C9C9C: m_iStockSkill = 0x8D; break;
    case 0x9D9C9D: m_iStockSkill = 0x8E; break;
    case 0x9E9E9E: m_iStockSkill = 0x8F; break;
    case 0x121212:
        if (nSum < 11) m_iStockSkill = 0x90;
        else m_iStockSkill = 0x91;
        break;
    default:
        m_iStockSkill = 0x6E;
        break;
    }
    m_iStockSum = nSum;
    if (m_iStock < 3) {
        m_iStockSkill = 0x6E;
        setStockInfo();
        m_iStockSkill = m_iStock;
    } else {
        setStockInfo();
    }
    return m_iStockSkill;
}
INT CCardRevolver::getStock(INT inFlag) {
    if (!inFlag) return m_iStock;
    INT n = 0;
    for (INT i = 0; i < 7; i++) {
        if (!m_pCardsS[i]) break;
        if (!m_pCardsS[i]->isNowStock()) break;
        n++;
    }
    return n;
}
INT CCardRevolver::setPlayStock(INT inFlag, INT inTest) {
    if (CGameWork::getFlag(1, 0x2f)) return 0;
    UINT uiEnd = CCardBattle::m_pInstance ? CCardBattle::m_pInstance->m_uiFlag & 0xC0000000 : 0;
    if (uiEnd) return 0;
    if (m_iState != 1) return 0;
    if (m_iStock <= 0) return 0;
    if (m_uiUser == 2 && getStock(0) != 3) return 0;
    if (!m_bActive) return 0;
    if (m_uiUser == 1 && getStock(1) != m_iStock) return 0;
    CCardBattle* pBattle = (CCardBattle*)CTaskMan::getSysTask(0x16);
    if (pBattle->isPlayingGimmickCard()) return 0;
    if (inFlag) {
        if (!pBattle->testBreakingCard(m_uiUser)) return 0;
        TRACE("CCardRevolver::setPlayStock \x81\xa6pCB->isPlayingCardStandNow==OK! pCB->resetNowPlaying\n");
        pBattle->resetNowPlaying(m_uiUser);
    }
    if (!pBattle->setPlayStock(m_hOwner, m_uiUser, m_pCardsS, m_iStockSkill, m_iExtraSkill, m_iFirstSkill, inTest)) return 0;
    for (INT i = 0; i < m_iStock; i++) {
        m_pCardsS[i] = NULL;
        m_pDataS[i] = NULL;
    }
    m_iStockSum = 0;
    m_uiFlag = (INT)m_uiFlag & ~0x200;
    m_iStockSkill = 0;
    m_iExtraSkill = 0;
    m_iFirstSkill = 0;
    m_iStock = 0;
    return 1;
}
INT CCardRevolver::getHave(INT inCategory, INT inPlay, INT inOwner, INT inReload, INT inNoGimmick) {
    INT n = 0;
    for (INT i = 0; i < m_nLimit; i++) {
        CBattleCard* pCard = &m_pCards[i];
        if (!pCard) continue;
        UINT uiFlag = pCard->getFlag();
        if (uiFlag & 0x20) continue;
        if (uiFlag & 0x200000) continue;
        if (!inOwner && pCard->getCardOwner() != m_uiUser) continue;
        INT cat = pCard->getCardCategory();
        if (inNoGimmick) {
            if (cat == 6) continue;
            if (cat == 14) continue;
        }
        if (m_uiFlag & 0x100) {
            if (uiFlag & 0x8000) continue;
        }
        if (inCategory && cat != inCategory) continue;
        if (!inPlay) {
            if (uiFlag & 0x40) continue;
            if (uiFlag & 4) continue;
        }
        if (!inReload && cat == 8) continue;
        n++;
    }
    return n;
}
INT CCardRevolver::checkBreakCard(INT inNum, INT inZero, BOOL inCheckGreater) {
    for (INT i = 0; i < m_nLimit; i++) {
        CBattleCard* pCard = &m_pCards[i];
        if (!pCard) continue;
        if (pCard->isReloadNG()) continue;
        INT num = pCard->getBaseCardNumber();
        if (inCheckGreater && inNum < num) continue;
        if (num >= inNum) return 1;
        if (num == 0 && inZero) return 1;
    }
    return 0;
}
void CCardRevolver::setAction(INT inID, INT inNum) {
    m_iAction = inID;
    if (m_iAction == 0) {
        CCardBattle* pBattle = (CCardBattle*)CTaskMan::getSysTask(0x16);
        m_iAction = pBattle->getPlayingCardID(m_uiUser, inNum);
    }
    if (m_iAction > 0) initAction();
}
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Game/CardRevolver", initAction__Q210CardBattle13CCardRevolver);
__asm__(".rdata\n.align 3\n.word 0x72614343\n.word 0x76655264\n.word 0x65766C6F\n.word 0x693A3A72\n.word 0x4174696E\n.word 0x6F697463\n.word 0x2029286E\n.word 0x0A2C6425\n.word 0x00000000\n.word 0x00000000\n.word 0x2A2A2A2A\n.word 0x55414320\n.word 0x4E4F4954\n.word 0x2A202121\n.word 0x202A2A2A\n.word 0x74696E69\n.word 0x69746341\n.word 0x29286E6F\n.word 0x0000000A\n.word 0x00000000\n.word 0x00000000\n.word 0x00000000\n.word 0x001E68A8\n.word 0x001E68B4\n.word 0x001E68C4\n.word 0x001E68D8\n.word 0x001E68E4\n.word 0x001E68F8\n.word 0x001E6910\n.word 0x001E6C04\n.word 0x001E6C04\n.word 0x001E6C04\n.word 0x001E6C04\n.word 0x001E6C04\n.word 0x001E6C04\n.word 0x001E6C04\n.word 0x001E6950\n.word 0x001E6C04\n.word 0x001E6C04\n.word 0x001E6C04\n.word 0x001E6C04\n.word 0x001E6C04\n.word 0x001E6C04\n.word 0x001E6C04\n.word 0x001E6C04\n.word 0x001E6C04\n.word 0x001E6C04\n.word 0x001E6C04\n.word 0x001E6C04\n.word 0x001E6C04\n.word 0x001E6C04\n.word 0x001E6C04\n.word 0x001E6C04\n.word 0x001E6C04\n.word 0x001E6C04\n.word 0x001E6C04\n.word 0x001E6C04\n.word 0x001E6C04\n.word 0x001E6C04\n.word 0x001E6C04\n.word 0x001E692C\n.word 0x00000000\n.word 0x001E6AF0\n.word 0x001E6B08\n.word 0x001E6B08\n.word 0x001E6B74\n.word 0x001E6B84\n.word 0x001E6B84\n.word 0x001E6B84\n.word 0x001E6B60\n.text\n");
void CCardRevolver::exeAction() {
    m_tqTimer -= getFrameRate();
    if (m_tqTimer > 0) return;
    if (m_uiFlag & 8) {
        rollDeck(0, 1, 300);
        m_tqTimer = 0;
        m_iState = 1;
        TRACE("CCardRevolver::exeAction(); RELOAD end.\n");
    } else {
        m_tqTimer = 200;
        m_uiFlag |= 8;
        rollDeck(0, 1, 300);
        m_tqTimer = 400;
        TRACE("CCardRevolver::exeAction(); RELOAD complete!\n");
    }
}
void CCardRevolver::addCard(CARD* pData, INT inReturn) {
    INT i;
    if (m_uiUser == 1) {
        if (inReturn) {
            m_cardAdd[m_nReturn + 9] = *pData;
            m_nReturn++;
            m_uiFlag |= 0x4000;
            return;
        }
        switch ((INT)(pData->m_nId >> 28)) {
        case 6:
        case 14:
            if (CCardBattle::getGimmickCount() > 0) return;
            m_cardAdd[0] = *pData;
            CCardBattle::addGimmickCount(1);
            break;
        case 5:
            if (pData->m_nKind != 0x39) {
                i = CCardBattle::getFriendCount();
                if (i >= 5) return;
                m_cardAdd[i + 1] = *pData;
                CCardBattle::addFriendCount(1);
            } else {
                if (CCardBattle::getPlutoCount() >= 3) return;
                for (i = 6; i < 9; i++) {
                    CBattleCard* pCard = &m_pCards[m_nReloadID + 1 + i];
                    if (!pCard) continue;
                    if (!pCard->isReloadNG()) continue;
                    if (pCard->isNowPlaying()) continue;
                    m_cardAdd[i] = *pData;
                    CCardBattle::addPlutoCount(1);
                    break;
                }
            }
            break;
        }
        m_uiFlag |= 0x400;
    } else {
        if (!(m_uiFlag & 0x2000)) return;
        if (m_nAdd >= 24) return;
        m_cardAdd[m_nAdd] = *pData;
        m_nAdd++;
        m_uiFlag |= 0x400;
    }
}
INT CCardRevolver::exeAddCard() {
    INT num;
    INT i;
    INT count;
    if (!(m_uiFlag & 0x400)) return 0;
    if (!(m_uiFlag & 0x20000)) return 0;
    num = -1;
    for (i = 0; i < 39; i++) {
        if (m_cardAdd[i].m_nKind) {
            num = i;
            break;
        }
    }
    if (num < 0) {
        m_uiFlag = (INT)m_uiFlag & ~0x400;
        return 0;
    }
    if (m_iMode > 0) changeMode(0, 1);
    else m_pEmpty->enableDisp(0);
    {
        INT next = num + 1; // MATCH: num + 1 as its own value (not reassociated with m_nReloadID)
        count = m_nReloadID + next;
    }
    if (!m_pCards[count].getCardKind()) m_nTotal[0]++;
    m_pCards[count].setHand(m_cardAdd[num], m_nRemain[0], m_nTotal[0], 0);
    m_pCards[count].resetReloadNG();
    if (m_uiFlag & 0x2000) m_pCards[count].setCardOwner(1);
    m_pCards[count].testDisp();
    m_nRemain[0]++;
    _setCard(m_cardAdd[num], 0, 0);
    rollDeck(0, 1, 300);
    return 1;
}
void CCardRevolver::initCounter() {
    FVECTOR pos;
    INT ofsID;
    m_pSeqCtrlCounter = NULL;
    m_pSeqCtrlCounter_Num01 = NULL;
    m_pSeqCtrlCounter_Num10 = NULL;
    m_nOldMode = 0;
    m_nOldRemain = 0;
    m_uiCounterFlag = 0;
    m_uiCounterState = 0;
    m_uiScutState = 0;
    _initNumEffect();
    m_pSeqCtrlTrap = NULL;
    m_uiNumTexID = 0;
    if (m_uiUser == 1) {
        m_uiCounterState = 1;
        m_uiScutState = 1;
        m_pSeqCtrlCounter = new ("c:/KHCOM/MainApp/Game/CardRevolver.cpp(4140)") D2::C2DSeqCtrl;
        m_pSeqCtrlCounter->setLayer(6);
        m_pSeqCtrlCounter->addSeq(g_szSeqCockpit, 0x42);
        m_pSeqCtrlCounter->addSeq(g_szSeqCockpit, 0x3E);
        m_pSeqCtrlCounter->addSeq(g_szSeqCockpit, 0x45);
        D2::C2DMan::getLayPosition(g_szLadCockpit, 9, 0, pos, FALSE);
        m_pSeqCtrlCounter->setPos(pos);
        m_pSeqCtrlCounter->enableDisp(0, FALSE);
        m_pSeqCtrlCounter->enableDisp(1, FALSE);
        m_pSeqCtrlCounter->enableDisp(2, FALSE);
        m_pSeqCtrlCounter->changeTime(2, 0);
        ofsID = 0;
        if (CSceneInfo::isMinigameScene()) {
            ofsID = 2;
            m_uiNumTexID = 4;
        }
        FVECTOR start = pos;
        m_pSeqCtrlCounter_Num01 = new ("c:/KHCOM/MainApp/Game/CardRevolver.cpp(4165)") D2::C2DSeqCtrl;
        m_pSeqCtrlCounter_Num01->setLayer(8);
        m_pSeqCtrlCounter_Num01->addSeq(g_szSeqCockpit, 0x46);
        pos.x = start.x + g_uvCRCounterNumOfs[ofsID].u;
        pos.y = start.y + g_uvCRCounterNumOfs[ofsID].v;
        m_pSeqCtrlCounter_Num01->setPos(pos);
        m_pSeqCtrlCounter_Num01->enableDisp(FALSE);
        m_pSeqCtrlCounter_Num10 = new ("c:/KHCOM/MainApp/Game/CardRevolver.cpp(4173)") D2::C2DSeqCtrl;
        m_pSeqCtrlCounter_Num10->setLayer(8);
        m_pSeqCtrlCounter_Num10->addSeq(g_szSeqCockpit, 0x46);
        pos.x = start.x + g_uvCRCounterNumOfs[ofsID + 1].u;
        pos.y = start.y + g_uvCRCounterNumOfs[ofsID + 1].v;
        m_pSeqCtrlCounter_Num10->setPos(pos);
        m_pSeqCtrlCounter_Num10->enableDisp(FALSE);
    }
}
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Game/CardRevolver", exeCounter__Q210CardBattle13CCardRevolver);
__asm__(".rdata\n.align 4\n.word 0x001E750C\n.word 0x001E7434\n.word 0x001E7434\n.word 0x001E750C\n.word 0x001E74B4\n.word 0x00000000\n.word 0x00000000\n.word 0x00000000\n.word 0x001E75DC\n.word 0x001E7534\n.word 0x001E7534\n.word 0x001E75DC\n.word 0x001E75DC\n.word 0x001E7584\n.word 0x00000000\n.word 0x00000000\n.word 0x001E78F8\n.word 0x001E7604\n.word 0x001E7698\n.word 0x001E788C\n.word 0x001E78B8\n.word 0x00000000\n.word 0x00000000\n.word 0x00000000\n.word 0x001E7A14\n.word 0x001E7920\n.word 0x001E7A14\n.word 0x001E7A14\n.word 0x001E7998\n.word 0x001E79D4\n.text\n");
void CCardRevolver::destroyCounter() {
    if (!m_pSeqCtrlCounter) return;
    m_uiCounterState = 3;
    m_pSeqCtrlCounter->replaceSeq(0, _ref(g_szSeqCockpit), 0x44);
    m_uiCounterFlag |= 1;
    m_pSeqCtrlCounter->enableDisp(2, FALSE);
    m_pSeqCtrlCounter_Num01->enableDisp(FALSE);
    m_pSeqCtrlCounter_Num10->enableDisp(FALSE);
    m_uiScutState = 4;
    m_pSeqCtrlCounter->replaceSeq(1, _ref(g_szSeqCockpit), 0x40);
    m_uiCounterFlag |= 2;
}
void CCardRevolver::deleteCounter() {
    delete m_pSeqCtrlCounter;
    m_pSeqCtrlCounter = NULL;
    delete m_pSeqCtrlCounter_Num01;
    m_pSeqCtrlCounter_Num01 = NULL;
    delete m_pSeqCtrlCounter_Num10;
    m_pSeqCtrlCounter_Num10 = NULL;
    delete m_pSeqCtrlNumEff;
    m_pSeqCtrlNumEff = NULL;
    delete m_pSeqCtrlTrap;
    m_pSeqCtrlTrap = NULL;
}
void CCardRevolver::setNumberEffect(INT inKind) {
    FVECTOR vPos;
    switch (inKind) {
    case 0x44:
        m_pSeqCtrlNumEff->destroy();
        m_pSeqCtrlNumEff->setLayer(8);
        m_pSeqCtrlNumEff->addSeq(g_szSeqCockpit, 0x78);
        m_uiNumEffState = 1;
        D2::C2DMan::getLayPosition(_ref(g_szLadCockpit), 0x13, 0, vPos, FALSE);
        break;
    case 0x53:
        m_pSeqCtrlNumEff->destroy();
        m_pSeqCtrlNumEff->setLayer(8);
        m_pSeqCtrlNumEff->addSeq(g_szSeqCockpit, 0x76);
        m_uiNumEffState = 2;
        D2::C2DMan::getLayPosition(_ref(g_szLadCockpit), 0x12, 0, vPos, FALSE);
        break;
    default:
        return;
    }
    if (m_uiUser != 1) vPos.x += 320.0f;
    m_pSeqCtrlNumEff->setPos(vPos);
}
void CCardRevolver::_initNumEffect() {
    m_uiNumEffState = 0;
    m_pSeqCtrlNumEff = new (com_malloc_d(sizeof(D2::C2DSeqCtrl), "c:/KHCOM/MainApp/Game/CardRevolver.cpp(4430)", RET_ADDR())) D2::C2DSeqCtrl;
}
void CCardRevolver::exeNumEffect() {
    switch (m_uiNumEffState) {
    case 0:
        break;
    case 1:
        if (m_pSeqCtrlNumEff->isEnd(0)) {
            m_uiNumEffState = 3;
            m_pSeqCtrlNumEff->replaceSeq(0, _ref(g_szSeqCockpit), 0x79);
        }
        break;
    case 2:
        if (m_pSeqCtrlNumEff->isEnd(0)) {
            m_uiNumEffState = 3;
            m_pSeqCtrlNumEff->replaceSeq(0, _ref(g_szSeqCockpit), 0x77);
        }
        break;
    case 3:
        if (m_pSeqCtrlNumEff->isEnd(0)) {
            m_uiNumEffState = 0;
            m_pSeqCtrlNumEff->destroy();
        }
        break;
    }
}
void CCardRevolver::setTrapEffect() {
    if (!m_pSeqCtrlTrap) {
        m_pSeqCtrlTrap = new (com_malloc_d(sizeof(D2::C2DSeqCtrl), "c:/KHCOM/MainApp/Game/CardRevolver.cpp(4469)", RET_ADDR())) D2::C2DSeqCtrl;
    }
    m_pSeqCtrlTrap->setLayer(8);
    m_pSeqCtrlTrap->addSeq(g_szSeqCockpit, 0x96);
    CB_LAYOUT& lay = g_LayoutData[58];
    FVECTOR vPos = {{ (FLOAT)lay.x, (FLOAT)lay.y, 0.0f, 1.0f }};
    m_pSeqCtrlTrap->setPos(vPos);
}
INT CCardRevolver::exeTrapEffect(INT inFlag) {
    if (!m_pSeqCtrlTrap) return 0;
    if (!inFlag || m_pSeqCtrlTrap->isEnd(0)) {
        delete m_pSeqCtrlTrap;
        m_pSeqCtrlTrap = NULL;
        return 0;
    }
    return 1;
}
void CCardRevolver::initStock() {
    FVECTOR vPos;
    m_uiStockState = 0;
    m_uiStockFlag = 0;
    m_uiStockCategory = 0;
    m_pSeqCtrlStock = NULL;
    m_pSeqCtrlStock_Num01 = NULL;
    m_pSeqCtrlStock_Num10 = NULL;
    m_pSeqCtrlStock = new (com_malloc_d(sizeof(D2::C2DSeqCtrl), "c:/KHCOM/MainApp/Game/CardRevolver.cpp(4510)", RET_ADDR())) D2::C2DSeqCtrl;
    m_pSeqCtrlStock_Num10 = new (com_malloc_d(sizeof(D2::C2DSeqCtrl), "c:/KHCOM/MainApp/Game/CardRevolver.cpp(4511)", RET_ADDR())) D2::C2DSeqCtrl;
    m_pSeqCtrlStock_Num01 = new (com_malloc_d(sizeof(D2::C2DSeqCtrl), "c:/KHCOM/MainApp/Game/CardRevolver.cpp(4512)", RET_ADDR())) D2::C2DSeqCtrl;
    m_pSeqCtrlStock->setLayer(7);
    m_pSeqCtrlStock_Num01->setLayer(8);
    m_pSeqCtrlStock_Num10->setLayer(8);
    if (m_uiUser == 1) {
        m_pSeqCtrlStock->addSeq(g_szSeqCockpit, 0x1B);
        D2::C2DMan::getLayPosition(g_szLadCockpit, 4, 0, vPos, FALSE);
        FVECTOR vNum = vPos;
        m_pSeqCtrlStock_Num01->addSeq(g_szSeqCockpit, 0xE);
        vNum.x = 41.0f;
        vNum.y = -11.0f;
        m_pSeqCtrlStock_Num01->setPos(vNum);
        m_pSeqCtrlStock_Num01->enableDisp(0, FALSE);
        m_pSeqCtrlStock_Num10->addSeq(g_szSeqCockpit, 0xE);
        vNum.x = 26.0f;
        m_pSeqCtrlStock_Num10->setPos(vNum);
        m_pSeqCtrlStock_Num10->enableDisp(0, FALSE);
    } else {
        m_pSeqCtrlStock->addSeq(g_szSeqCockpit, 0x18);
        D2::C2DMan::getLayPosition(g_szLadCockpit, 3, 0, vPos, FALSE);
        FVECTOR vNum = vPos;
        m_pSeqCtrlStock_Num01->addSeq(g_szSeqCockpit, 0xE);
        vNum.x = -13.0f;
        vNum.y = -12.0f;
        m_pSeqCtrlStock_Num01->setPos(vNum);
        m_pSeqCtrlStock_Num01->enableDisp(0, FALSE);
        m_pSeqCtrlStock_Num10->addSeq(g_szSeqCockpit, 0xE);
        vNum.x = -28.0f;
        m_pSeqCtrlStock_Num10->setPos(vNum);
        m_pSeqCtrlStock_Num10->enableDisp(0, FALSE);
    }
    m_pSeqCtrlStock->setPos(vPos);
    initStockSkillName(m_pSeqCtrlStock);
    m_pSeqCtrlStock->enableDisp(FALSE);
}
void CCardRevolver::initStockSkillName(D2::C2DSeqCtrl* inSeq) {
    G_RGBA rgba;
    rgba.col = 0x80808080;
    m_p2DFntStockName = new (com_malloc_d(sizeof(D2::C2DFnt), "c:/KHCOM/MainApp/Game/CardRevolver.cpp(4567)", RET_ADDR()))
        D2::C2DFnt(0, 0, &rgba, 14, g_szStockSkillName[m_iStockSkill], 0x80);
    inSeq->addFntShift(g_szSeqCockpit, m_p2DFntStockName, -4.0f, 0.0f);
    inSeq->m_seqControl.fgColor = 1;
    inSeq->m_seqControl.fgFntScale = 1;
}
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Game/CardRevolver", exeStock__Q210CardBattle13CCardRevolver);
__asm__(".rdata\n.align 4\n.word 0x001E8590\n.word 0x001E84B0\n.word 0x001E84E8\n.word 0x001E8524\n.word 0x001E8570\n.word 0x001E8590\n.word 0x00000000\n.word 0x00000000\n.text\n");
void CCardRevolver::deleteStock() {
    delete m_pSeqCtrlStock;
    m_pSeqCtrlStock = NULL;
    delete m_pSeqCtrlStock_Num01;
    m_pSeqCtrlStock_Num01 = NULL;
    delete m_pSeqCtrlStock_Num10;
    m_pSeqCtrlStock_Num10 = NULL;
}
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Game/CardRevolver", setStockInfo__Q210CardBattle13CCardRevolver);
__asm__(".rdata\n.align 4\n.word 0x001E8AF4\n.word 0x001E8B00\n.word 0x001E8B0C\n.word 0x001E8B24\n.word 0x001E8B0C\n.word 0x001E8B24\n.word 0x001E8B18\n.word 0x00000000\n.text\n");
void CCardRevolver::changeDarkMode(HANDLE inHandle, INT inFlag) {
    changeOwner(inHandle);
    if (inFlag) m_uiFlag |= 0x100;
    else m_uiFlag = (INT)m_uiFlag & ~0x100;
    calcStockSkill();
}
INT CCardRevolver::setPlayerDuelCard(INT inTest) {
    if (m_iState != 1) return 0;
    if (CGameWork::getFlag(1, 8)) return 0;
    if (CGameWork::getInt(1, 0xE2) & 2) return 0;
    CCardBattle* pBattle = (CCardBattle*)CTaskMan::getSysTask(0x16);
    if (!_isEqual(pBattle->m_uiDuelState, 5)) return 0;
    if (!_isEqual(pBattle->m_uiState[1], 10)) return 0;
    if (pBattle->isExecutingCard(1)) return 0;
    CBattleCard* pCard = getActiveCard();
    if (!pCard) return 0;
    INT cat = pCard->getCardData()->m_nId >> 28;
    if (cat >= 6) {
        if (cat < 9) return 0;
        if (cat == 14) return 0;
    }
    INT ret = pBattle->setPlayCard(m_hOwner, m_uiUser, pCard, NULL, inTest);
    if (!ret) return 0;
    m_nRemain[m_iMode]--;
    m_tqRollSpeed = 300;
    rollDeck(1, 1, 300);
    return ret;
}
void CCardRevolver::setPlayerDuelStock(CBattleCard* inCard, UINT inPri) {
    m_iStock = inPri;
    m_uiFlag |= 0x200;
    m_pCardsS[m_iStock] = inCard;
    m_pDataS[m_iStock] = inCard->getCardData();
    m_iStock++;
    CSound::playSE(15, -1, 64);
}
INT CCardRevolver::setPlayerDuelPlayStock() {
    CCardBattle* pBattle = (CCardBattle*)CTaskMan::getSysTask(0x16);
    pBattle->setDuelPlayStock(m_pCardsS, m_iStockSkill);
    for (INT i = 0; i < m_iStock; i++) {
        m_pCardsS[i] = NULL;
        m_pDataS[i] = NULL;
    }
    m_iStockSum = 0;
    m_uiFlag = (INT)m_uiFlag & ~0x200;
    m_iStockSkill = 0;
    m_iExtraSkill = 0;
    m_iFirstSkill = 0;
    m_iStock = 0;
    return 1;
}
void CCardRevolver::setSnatch(HANDLE inHandle) {
    m_hSnatch = inHandle;
    m_nSnatchCount++;
    if (!(m_uiFlag & 0x1000)) {
        m_tqSnatch = 0;
        if (m_iMode > 0) changeMode(0, 1);
        else m_pEmpty->enableDisp(FALSE);
    }
    if (m_tqSnatch > 0) m_tqSnatch = 100;
    m_uiFlag |= 0x1000;
}
INT CCardRevolver::exeSnatchCard() {
    if (!(m_uiFlag & 0x1000)) return 0;
    if (!(m_uiFlag & 0x20000)) return 0;
    TIMEQ tq = getFrameRate();
    m_tqSnatch -= tq;
    if (m_tqSnatch > 0) {
        m_tqTimer -= tq;
        if (m_tqTimer <= 0) {
            m_tqTimer = 200;
            rollDeck(-1, 0, 200);
        }
        return 0;
    }
    if (m_nSnatchCount > 0) {
        snatchCard();
        m_nSnatchCount--;
        return 0;
    }
    m_tqSnatch = 0;
    m_hSnatch = 0;
    m_uiFlag = (INT)m_uiFlag & ~0x1000;
    CPlayer* pPlayer = _ref(g_pPlayer);
    if (pPlayer) pPlayer->setCardSnatch(0);
    return 1;
}
void CCardRevolver::snatchCard() {
    if (getHave(1, 1, 0, 0, 0) < 7) return;
    m_tqSnatch = 1000;
    if (m_iMode > 0) changeMode(0, 1);
    else m_pEmpty->enableDisp(FALSE);
    for (INT i = 0; i < m_nLimit; i++) {
        CBattleCard* pCard = &m_pCards[i];
        if (!pCard) continue;
        if (pCard->getCardKind() != 0x12) continue;
        if (pCard->isNowPlaying()) continue;
        INT stock = pCard->getStock();
        if (stock != -1) continue;
        if (pCard->isReloadNG()) continue;
        if (pCard->getCardOwner() != 1) continue;
        INT hand = pCard->getHand();
        pCard->setSnatch(m_hSnatch);
        if (hand != stock) m_nRemain[m_iMode]--;
        break;
    }
    m_tqTimer = 200;
    rollDeck(-1, 1, 200);
}
INT CCardRevolver::returnSnatch(INT inNum) {
    return returnSnatchCard(inNum);
}
INT CCardRevolver::returnSnatchCard(INT inNum) {
    PRIZE_PARAM param;
    INT num;
    INT i;
    INT n;
    CBattleCard* card;
    if (m_uiUser != 2) return 0;
    returnStockCard(1);
    if (m_iMode > 0) changeMode(0, 1);
    else m_pEmpty->enableDisp(0);
    num = inNum;
    CTask* pOwner = CTaskMan::getTask(m_hOwner); // MATCH: the call before the flag stores (no such local in the ref)
    param.uiFlag = 0;
    param.uiPrizeFlag = 0;
    param.vPos = pOwner->Crd(0).m_World.v[3];
    param.vPos.y += 1.0f;
    param.fSpeed = 0.25f;
    if (num <= 0) num = m_nAdd;
    n = m_nLimit - 1;
    for (i = 0; i < m_nLimit; i++, n--) {
        card = &m_pCards[n];
        if (!card) continue;
        if (card->getCardOwner() != 1) continue;
        if (card->getHand() != -1) m_nRemain[0]--;
        param.PrizeKind = card->getCardKind();
        param.iNumber = card->getBaseCardNumber();
        FLOAT rx = (-70.0f - RND::genrand_real1() * 10.0f) * 0.017453292f;
        FLOAT ry = RND::genrand_real1() * 360.0f * 0.017453292f;
        param.vRot.set(rx, ry, 0.0f);
        Math3D::LimitPIVector(param.vRot);
        CPrize* pPrize = (CPrize*)CEntityFactory::getInstance()->createPrize(param, 0);
        if (pPrize) pPrize->enableReturnCard(TRUE);
        card->resetSnatch();
        num--;
        if (--m_nAdd == 0) break;
        if (num == 0) break;
    }
    initReload(2);
    CCardBattle::setGimmickOwner(m_hOwner);
    param.PrizeKind = 0x42;
    param.iNumber = 0;
    FLOAT rx = (-70.0f - RND::genrand_real1() * 10.0f) * 0.017453292f;
    FLOAT ry = RND::genrand_real1() * 360.0f * 0.017453292f;
    param.vRot.set(rx, ry, 0.0f);
    Math3D::LimitPIVector(param.vRot);
    CEntityFactory::getInstance()->createPrize(param, 0);
    return 1;
}
INT CCardRevolver::exeReturnCard() {
    INT count;
    INT i;
    CBattleCard* card;
    CARD* cardData;
    if (!(m_uiFlag & 0x4000)) return 0;
    if (!(m_uiFlag & 0x20000)) return 0;
    count = m_nReturn + 8;
    if (count < 9) {
        m_uiFlag = (INT)m_uiFlag & ~0x4000;
        return 0;
    }
    if (m_iMode > 0) changeMode(0, 1);
    else m_pEmpty->enableDisp(0);
    cardData = NULL;
    for (i = 0; i < m_nLimit; i++) {
        card = &m_pCards[i];
        if (!card) continue;
        if (card->getCardOwner() == 1) continue;
        cardData = card->getCardData();
        if (cardData->m_nKind == m_cardAdd[count].m_nKind && card->getBaseCardNumber() == m_cardAdd[count].m_nNumber) {
            card->setBaseCardNumber();
            break;
        }
        cardData = NULL;
    }
    if (cardData) {
        if (card->getCardOwner() == 2) {
            card->setCardOwner(1);
            card->setHand(*cardData, m_nRemain[0], m_nTotal[0], 0);
            card->updateNumberEffect();
            card->testDisp();
            if (cardData->m_nCategory1 == 8) {
                m_nReloadCard = 1;
                card->updateReload(m_tqCharge / 30);
            } else {
                m_nRemain[0]++;
            }
            m_nReturn--;
            _setCard(m_cardAdd[count], 0, 0);
            rollDeck(0, 1, 300);
            return 1;
        }
        card->resetReloadNG();
        card->setCardOwner(1);
        card->updateNumberEffect();
        card->testDisp();
    }
    m_nReturn--;
    _setCard(m_cardAdd[count], 0, 0);
    return 1;
}
INT CCardRevolver::canSnatch() {
    if (getHave(1, 1, 0, 0, 0) > 6) return TRUE;
    return FALSE;
}
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Game/CardRevolver", setPlayerPronounceCard__Q210CardBattle13CCardRevolveri);
__asm__(".rdata\n.align 4\n.word 0x001E9814\n.word 0x001E9840\n.word 0x001E9864\n.word 0x001E9864\n.word 0x001E9A0C\n.word 0x001E9A0C\n.word 0x001E9A0C\n.word 0x001E9864\n.word 0x001E9864\n.word 0x001E9864\n.word 0x001E9864\n.word 0x001E9864\n.word 0x001E9A0C\n.word 0x00000000\n.text\n");
void CCardRevolver::setDiscard() {
    if (isDiscard()) return;
    discard();
    m_uiFlag |= 0x8000;
}
INT CCardRevolver::exeDiscard() {
    if (!isDiscard()) return 0;
    m_tqDiscard -= getFrameRate();
    if (m_tqDiscard > 0) return 0;
    m_tqDiscard = 0;
    m_uiFlag &= ~0x8000;
    return 1;
}
void CCardRevolver::discard() {
    PRIZE_PARAM param;
    INT discard;
    INT hand;
    INT i;
    CBattleCard* card;
    m_tqDiscard = 3000;
    if (m_iMode > 0) changeMode(0, 1);
    else m_pEmpty->enableDisp(0);
    switch (m_iState) {
    case 2:
        m_uiFlag |= 0x20008;
        m_nReloadCount = m_iMax;
        rollDeck(0, 1, 300);
        m_tqTimer = 0;
        m_iState = 1;
        break;
    case 3:
        m_uiFlag |= 0x20008;
        rollDeck(0, 1, 300);
        m_tqTimer = 0;
        m_iState = 1;
        break;
    }
    CCardBattle::decEnchantTime(m_uiUser, 0x29, 0);
    CCardBattle* pCB = (CCardBattle*)CTaskMan::getSysTask(0x16);
    INT kind = _getEnchantKind(pCB, m_uiUser);
    if (kind != 0x18) {
        if (m_iChargeCount < 3) m_iChargeCount++;
        if (kind == 0x1E) m_iChargeCount = 1;
    }
    m_iCharge = m_iChargeCount - 1;
    m_tqCharge = m_iCharge * 3000;
    param.uiFlag = 0;
    param.uiPrizeFlag = 0;
    _ref(g_pPlayer)->getLockOnPos(param.vPos);
    param.fSpeed = 0.25f;
    discard = 1;
    for (i = 0; i < m_nLimit; i++) {
        card = &m_pCards[i];
        if (!card) continue;
        if (card->getCardOwner() != 1) continue;
        if (card->isNowPlaying()) continue;
        if (card->getStock() != -1) continue;
        if (card->isReloadNG()) continue;
        if (discard < 30 || card->getCardData()->m_nCategory1 == 8) {
            if (m_nReloadID < i) {
                hand = card->getHand();
                card->setDiscard(0);
            } else {
                param.PrizeKind = card->getCardKind();
                param.iNumber = card->getBaseCardNumber();
                FLOAT rx = (-70.0f - RND::genrand_real1() * 10.0f) * 0.017453292f;
                FLOAT ry = RND::genrand_real1() * 360.0f * 0.017453292f;
                param.vRot.set(rx, ry, 0.0f);
                Math3D::LimitPIVector(param.vRot);
                CPrize* pPrize = (CPrize*)CEntityFactory::getInstance()->createPrize(param, 0);
                if (pPrize) {
                    pPrize->enableReturnCard(TRUE);
                    pPrize->enableMarluxia3Mode(TRUE);
                    discard++;
                }
                hand = card->getHand();
                card->setDiscard(1);
            }
        } else {
            hand = card->getHand();
            card->setDiscard(0);
        }
        if (hand != -1) m_nRemain[m_iMode]--;
    }
    m_nRemain[m_iMode] = 0;
    m_nReloadCard = 0;
    rollDeck(0, 1, 300);
}
void CCardRevolver::resetCardOwner(INT inOld, INT inNew) {
    for (INT i = 0; i < m_nLimit; i++) {
        CBattleCard* pCard = &m_pCards[i];
        if (!pCard) continue;
        if (pCard->getCardOwner() != inOld) continue;
        if (pCard->getCardCategory() == 8 && inNew == 0) continue;
        pCard->setCardOwner(inNew);
    }
}
INT CCardRevolver::calcPlutoMark() {
    m_nPlutoMark = 0;
    for (INT i = 0; i < m_nReloadID; i++) {
        CBattleCard* pCard = &m_pCards[i];
        if (!pCard) continue;
        BOOL bOn = FALSE;
        if (pCard->getCardOwner() == 1 && pCard->isReloadNG() && pCard->getHand() == -1 && !pCard->isNowPlaying() &&
            pCard->getStock() == -1 && pCard->getCardKind() != 0) {
            INT cat = pCard->getCardCategory();
            if (cat < 5 && cat != 0) {
                bOn = TRUE;
                pCard->setPlutoON();
                m_nPlutoMark++;
            }
        }
        if (!bOn) pCard->resetPlutoON();
    }
    return m_nPlutoMark;
}
extern "C" char D_0047AC08[]; /* "%3d: [Card%03d:%d(%d)] iStock=%d, iHand=%d/%d, uiFlag=%08x, uiOwner=%d,\n" */
static inline void _clearPrizeFlags(PRIZE_PARAM* p) {
    p->uiFlag = 0;
    p->uiPrizeFlag = 0;
}
/* Parked: 1 line off: the TRACE's card-kind argument - the original reloads it (lbu 0x150(s0)) as the last argument
   load; card->getCardKind() / getCardData()->m_nKind schedule it right after a3, param.PrizeKind (below) puts it
   last but from the stack. Worked: _clearPrizeFlags(&param), getCardData()->m_nKind for PrizeKind, BattleCard.h's
   isPlutoON with an `unsigned int flag` local (hoisted mask). Tried: kind sources, card-test/loop shapes;
   permuter 5 min (5834 candidates, no gain). */
#if 0
void CCardRevolver::dropPlutoMarkCard(const FVECTOR* inPos) {
    CBattleCard* card;
    INT i;
    INT n;
    PRIZE_PARAM param;
    if (!m_nPlutoMark) return;
    n = RND::genrand_int31() % m_nReloadID;
    _clearPrizeFlags(&param);
    param.vPos = *inPos;
    param.fSpeed = 0.25f;
    for (i = 0; i < m_nReloadID; i++) {
        card = &m_pCards[n];
        if (card && card->isPlutoON()) {
            param.PrizeKind = card->getCardData()->m_nKind;
            param.iNumber = card->getBaseCardNumber();
            FLOAT rx = (-70.0f - RND::genrand_real1() * 10.0f) * 0.017453292f;
            FLOAT ry = RND::genrand_real1() * 360.0f * 0.017453292f;
            param.vRot.set(rx, ry, 0.0f);
            Math3D::LimitPIVector(param.vRot);
            CPrize* pPrize = (CPrize*)CEntityFactory::getInstance()->createPrize(param, 0);
            if (!pPrize) return;
            pPrize->enableReturnCard(TRUE);
            TRACE(D_0047AC08, n, param.PrizeKind, card->getCardData()->m_nNumber, card->getBaseCardNumber(),
                  card->getStock(), card->getHand(), card->getHandMax(), card->getFlag(), card->getCardOwner());
            card->setCardOwner(2);
            card->resetPlutoON();
            m_nPlutoMark--;
            return;
        }
        if (++n >= m_nReloadID) n = 0;
    }
}
#else
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Game/CardRevolver", dropPlutoMarkCard__Q210CardBattle13CCardRevolverPC7FVECTOR);
#endif
void CCardRevolver::setPlayNG(INT inFlag, INT inCategory) {
    INT i;
    if (inFlag) {
        for (i = 0; i < m_nReloadID; i++) {
            CBattleCard* pCard = &m_pCards[i];
            if (!pCard) continue;
            if (!inCategory || pCard->getCardCategory() == inCategory) pCard->setPlayNG();
            else pCard->resetPlayNG();
        }
    } else {
        for (i = 0; i < m_nReloadID; i++) {
            CBattleCard* pCard = &m_pCards[i];
            if (!pCard) continue;
            if (inCategory && pCard->getCardCategory() != inCategory) continue;
            pCard->resetPlayNG();
        }
    }
}
CEnchantGauge::CEnchantGauge(CTask* pParent, INT inUser, CARD& inCard) : CTask(pParent, 0x4000) {
    m_uiUser = inUser;
    m_cardData = inCard;
    m_pSeqCtrls = NULL;
    m_pFntName = NULL;
    _initialize();
}
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Game/CardRevolver", _$_Q210CardBattle13CEnchantGauge);
void CEnchantGauge::_initialize() {
    FVECTOR pos;
    INT layout;
    G_RGBA rgba;
    BC_PARAM* param;
    INT num;
    m_uiState = 1;
    m_pSeqCtrls = new ("c:/KHCOM/MainApp/Game/CardRevolver.cpp(5825)") D2::C2DSeqCtrl[3];
    if (!m_pSeqCtrls) AssertApp("c:/KHCOM/MainApp/Game/CardRevolver.cpp", 5826, "m_pSeqCtrls", "CEnchantGauge:: m_pSeqCtrls=New\x8e\xb8\x94s");
    m_pSeqCtrls[0].setLayer(6);
    m_pSeqCtrls[0].addSeq(g_szSeqCockpit, 0x36);
    layout = (m_uiUser == 1) ? 6 : 5;
    D2::C2DMan::getLayPosition(g_szLadCockpit, layout, 0, pos, FALSE);
    m_pSeqCtrls[0].setPos(pos);
    rgba.col = 0x80808080;
    _ref(m_pFntName) = new ("c:/KHCOM/MainApp/Game/CardRevolver.cpp(5840)") D2::C2DFnt(0, 0, &rgba, 12, NULL, 2); // MATCH: store via reference keeps it before the m_pSeqCtrls reload
    m_pSeqCtrls[0].addFntShift(g_szSeqCockpit, m_pFntName, 0.0f, 2.0f);
    m_pSeqCtrls[0].enableColorCtrl(TRUE);
    setText();
    param = &g_pBattleCardList[m_cardData.m_nKind];
    m_pSeqCtrls[0].addSeq(g_szSeqCockpit, param->EFF_PART);
    m_nCount = param->EFF_TIMES;
    pos.x -= 35.0f;
    pos.y -= 5.0f;
    num = m_nCount / 10;
    m_pSeqCtrls[1].setLayer(8);
    if (num > 0) m_pSeqCtrls[1].addSeq(g_szSeqCockpit, num + 0x2B);
    else m_pSeqCtrls[1].addSeq(g_szSeqCockpit, 0x35);
    m_pSeqCtrls[1].setPos(pos);
    m_pSeqCtrls[1].enableDisp(0, FALSE);
    pos.x += 16.0f;
    num = m_nCount % 10;
    m_pSeqCtrls[2].setLayer(8);
    m_pSeqCtrls[2].addSeq(g_szSeqCockpit, num + 0x2B);
    m_pSeqCtrls[2].setPos(pos);
    m_pSeqCtrls[2].enableDisp(0, FALSE);
    m_nStack = m_nNext = m_nCount;
}
void CEnchantGauge::setText() {
    if (!m_pSeqCtrls) return;
    if (!m_pFntName) return;
    if (m_cardData.m_nKind < 0x43) return;
    m_pFntName->setText(g_szEnemyCardEffName[m_cardData.m_nKind - 0x43]);
}
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Game/CardRevolver", main__Q210CardBattle13CEnchantGauge);
__asm__(".rdata\n.align 4\n.word 0x001EAB74\n.word 0x001EA934\n.word 0x001EA9B4\n.word 0x001EA9D8\n.word 0x001EAAF4\n.word 0x001EAB4C\n.word 0x001EAB74\n.word 0x00000000\n.text\n");
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Game/CardRevolver", changeCount__Q210CardBattle13CEnchantGaugei);
__asm__(".rdata\n.align 4\n.word 0x001EABD4\n.word 0x001EABDC\n.word 0x001EABD4\n.word 0x001EAC7C\n.word 0x001EABD4\n.word 0x001EABD4\n.text\n");
void CEnchantGauge::destroy() {
    m_nNext = 0;
    CCardBattle* pBattle = _ref(CCardBattle::m_pInstance);
    UINT uiEnd = pBattle ? pBattle->m_uiFlag & 4 : 0;
    if (uiEnd) {
        out();
        return;
    }
    if (m_uiState == 3 || m_uiState == 4) return;
    m_uiState = 4;
    m_pSeqCtrls[0].replaceSeq(0, _ref(g_szSeqCockpit), 0x38);
    m_pSeqCtrls[0].enableDisp(1, FALSE);
    m_pSeqCtrls[2].enableDisp(0, FALSE);
    m_pSeqCtrls[1].enableDisp(0, FALSE);
}
CStockFolder::CStockFolder(CTask* pParent, INT inUser) : CTask(pParent, 0x4000) {
    m_uiUser = inUser;
    m_hOwner = pParent->getHandle();
    _init();
}
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Game/CardRevolver", _$_Q210CardBattle12CStockFolder);
void CStockFolder::_init() {
    m_iStock = 0;
    m_iStockSum = 0;
    m_iStockSkill = 0;
    m_iExtraSkill = 0;
    m_iFirstSkill = 0;
    for (INT i = 0; i < 7; i++) {
        m_pCards[i] = NULL;
        m_pData[i] = NULL;
    }
    initGauge();
}
void CStockFolder::main() {
    exeGauge();
}
void CStockFolder::destroy() {
    for (INT i = 0; i < m_iStock; i++) {
        m_pCards[i] = NULL;
        m_pData[i] = NULL;
    }
    m_iStock = 0;
    m_iStockSum = 0;
    m_iStockSkill = 0;
    m_iExtraSkill = 0;
    m_iFirstSkill = 0;
    m_uiState = 5;
    m_pSeqCtrl->enableDisp(FALSE);
    m_uiFlag = (INT)m_uiFlag & ~1;
    m_uiCategory = 0;
    m_pSeqCtrl_Num01->enableDisp(0, FALSE);
    m_pSeqCtrl_Num10->enableDisp(0, FALSE);
}
void CStockFolder::initGauge() {
    FVECTOR vPos;
    m_uiState = 0;
    m_uiFlag = 0;
    m_uiCategory = 0;
    m_pSeqCtrl = NULL;
    m_pSeqCtrl_Num01 = NULL;
    m_pSeqCtrl_Num10 = NULL;
    m_nNumTexID = 0;
    m_pSeqCtrl = new (com_malloc_d(sizeof(D2::C2DSeqCtrl), "c:/KHCOM/MainApp/Game/CardRevolver.cpp(6211)", RET_ADDR())) D2::C2DSeqCtrl;
    m_pSeqCtrl_Num10 = new (com_malloc_d(sizeof(D2::C2DSeqCtrl), "c:/KHCOM/MainApp/Game/CardRevolver.cpp(6212)", RET_ADDR())) D2::C2DSeqCtrl;
    m_pSeqCtrl_Num01 = new (com_malloc_d(sizeof(D2::C2DSeqCtrl), "c:/KHCOM/MainApp/Game/CardRevolver.cpp(6213)", RET_ADDR())) D2::C2DSeqCtrl;
    m_pSeqCtrl->setLayer(7);
    m_pSeqCtrl_Num01->setLayer(8);
    m_pSeqCtrl_Num10->setLayer(8);
    m_pSeqCtrl->addSeq(g_szSeqCockpit, 0x18);
    D2::C2DMan::getLayPosition(g_szLadCockpit, 3, 0, vPos, FALSE);
    vPos.x += -8.0f;
    FVECTOR vNum = vPos;
    m_pSeqCtrl_Num01->addSeq(g_szSeqCockpit, 0xE);
    vNum.x = -13.0f;
    vNum.y = -12.0f;
    m_pSeqCtrl_Num01->setPos(vNum);
    m_pSeqCtrl_Num01->enableDisp(0, FALSE);
    m_pSeqCtrl_Num10->addSeq(g_szSeqCockpit, 0xE);
    vNum.x = -28.0f;
    m_pSeqCtrl_Num10->setPos(vNum);
    m_pSeqCtrl_Num10->enableDisp(0, FALSE);
    m_pSeqCtrl->setPos(vPos);
    initName(m_pSeqCtrl);
    m_pSeqCtrl->enableDisp(FALSE);
}
void CStockFolder::initName(D2::C2DSeqCtrl* inSeq) {
    G_RGBA rgba;
    rgba.col = 0x80808080;
    m_p2DFntStockName = new (com_malloc_d(sizeof(D2::C2DFnt), "c:/KHCOM/MainApp/Game/CardRevolver.cpp(6245)", RET_ADDR()))
        D2::C2DFnt(0, 0, &rgba, 14, g_szStockSkillName[m_iStockSkill], 0x80);
    inSeq->addFntShift(g_szSeqCockpit, m_p2DFntStockName, -4.0f, 0.0f);
    inSeq->m_seqControl.fgColor = 1;
    inSeq->m_seqControl.fgFntScale = 1;
}
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Game/CardRevolver", exeGauge__Q210CardBattle12CStockFolder);
__asm__(".rdata\n.align 4\n.word 0x001EB268\n.word 0x001EB2B4\n.word 0x001EB2B4\n.word 0x001EB2B4\n.word 0x001EB340\n.word 0x001EB320\n.word 0x001EB2B4\n.word 0x00000000\n.word 0x001EB524\n.word 0x001EB3AC\n.word 0x001EB3E4\n.word 0x001EB408\n.word 0x001EB474\n.word 0x001EB524\n.word 0x001EB4A0\n.word 0x00000000\n.text\n");
void CStockFolder::deleteGauge() {
    delete m_pSeqCtrl;
    m_pSeqCtrl = NULL;
    delete m_pSeqCtrl_Num01;
    m_pSeqCtrl_Num01 = NULL;
    delete m_pSeqCtrl_Num10;
    m_pSeqCtrl_Num10 = NULL;
}
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Game/CardRevolver", setStockInfo__Q210CardBattle12CStockFolder);
__asm__(".rdata\n.align 4\n.word 0x001EB87C\n.word 0x001EB888\n.word 0x001EB894\n.word 0x001EB8AC\n.word 0x001EB894\n.word 0x001EB8AC\n.word 0x001EB8A0\n.word 0x00000000\n.text\n");
INT CStockFolder::setStockCard(INT inKind, INT inNumber) {
    if (CGameWork::getFlag(1, 10)) return 0;
    if (m_uiFlag & 2) return 0;
    CCardBattle* pBattle = (CCardBattle*)CTaskMan::getSysTask(0x16);
    if (!pBattle) return 0;
    if (pBattle->isPlayingCard(m_uiUser)) return 0;
    if (m_iStock == 3) return 0;
    m_pCards[m_iStock] = new (com_malloc_d(sizeof(CBattleCard), "c:/KHCOM/MainApp/Game/CardRevolver.cpp(6495)", RET_ADDR())) CBattleCard(this, inKind, inNumber, m_uiUser, 0);
    CBattleCard* pCard = m_pCards[m_iStock];
    pCard->setFolderON();
    pCard->setRoomEffect(NULL);
    m_pData[m_iStock] = pCard->getCardData();
    pCard->setStock(m_iStock);
    m_iStock++;
    CSound::playSE(15, -1, 64);
    return 1;
}
INT CStockFolder::calcEnemyStockSkill() {
    UINT uiKinds = 0;
    INT nSum = 0;
    m_iStockSkill = m_iStock;
    for (INT i = 0; i < m_iStock; i++) {
        uiKinds |= m_pData[i]->m_nKind << (i * 8);
        nSum += m_pData[i]->m_nNumber;
    }
    if (uiKinds == 0x989898) {
        if (nSum == 12) m_iStockSkill = 0x84;
        else if (nSum == 15) m_iStockSkill = 0x85;
        else if (nSum == 18) m_iStockSkill = 0x86;
        else if (nSum == 21) m_iStockSkill = 0x87;
        else if (nSum == 24) m_iStockSkill = 0x88;
        else m_iStockSkill = 0x6E;
    } else {
        m_iStockSkill = 0x6E;
    }
    m_iStockSum = nSum;
    if (m_iStock < 3) {
        m_iStockSkill = 0x6E;
        setStockInfo();
        m_iStockSkill = m_iStock;
    } else {
        setStockInfo();
    }
    return m_iStockSkill;
}
INT CStockFolder::returnStockCard() {
    if (m_iStock == 0) return 0;
    for (INT i = 0; i < m_iStock; i++) {
        if (m_pCards[i]) m_pCards[i]->destroy(1);
        m_pCards[i] = NULL;
        m_pData[i] = NULL;
    }
    m_iStock = 0;
    m_iStockSum = 0;
    m_iStockSkill = 0;
    m_iExtraSkill = 0;
    m_iFirstSkill = 0;
    return 1;
}
INT CStockFolder::getStock(INT inFlag) {
    if (!inFlag) return m_iStock;
    INT n = 0;
    for (INT i = 0; i < 7; i++) {
        if (!m_pCards[i]) break;
        if (!m_pCards[i]->isNowStock()) break;
        n++;
    }
    return n;
}
INT CStockFolder::setPlayStock(INT inFlag, INT inTest) {
    if (CGameWork::getFlag(1, 0x2f)) return 0;
    UINT uiEnd = CCardBattle::m_pInstance ? CCardBattle::m_pInstance->m_uiFlag & 0xC0000000 : 0;
    if (uiEnd) return 0;
    if (m_iStock <= 0) return 0;
    if (m_uiUser == 2 && getStock(1) != 3) return 0;
    CCardBattle* pBattle = (CCardBattle*)CTaskMan::getSysTask(0x16);
    if (pBattle->isPlayingGimmickCard()) return 0;
    if (inFlag) pBattle->resetNowPlaying(m_uiUser);
    INT ret = pBattle->setPlayStock(m_hOwner, m_uiUser, m_pCards, m_iStockSkill, m_iExtraSkill, m_iFirstSkill, inTest);
    if (!ret) return 0;
    for (INT i = 0; i < m_iStock; i++) {
        m_pCards[i] = NULL;
        m_pData[i] = NULL;
    }
    m_iStock = 0;
    m_iStockSum = 0;
    m_iStockSkill = 0;
    m_iExtraSkill = 0;
    m_iFirstSkill = 0;
    return ret;
}
INT CStockFolder::setPronounceCard(INT inKind, INT inNumber) {
    if (m_uiState >= 6) {
        CCardBattle* pBattle = (CCardBattle*)CTaskMan::getSysTask(0x16);
        if (!pBattle) return 0;
        if (!_isEqual(pBattle->m_uiDuelState, 11) && m_iStock != 6) goto ok;
    }
    return 0;
ok:
    m_pCards[m_iStock] = new (com_malloc_d(sizeof(CBattleCard), "c:/KHCOM/MainApp/Game/CardRevolver.cpp(6700)", RET_ADDR())) CBattleCard(this, inKind, inNumber, m_uiUser, 0);
    CBattleCard* pCard = m_pCards[m_iStock];
    pCard->setRoomEffect(NULL);
    m_pData[m_iStock] = pCard->getCardData();
    pCard->setPronounce(m_iStock);
    m_iStock++;
    return 1;
}
INT CStockFolder::setPlayPronounce() {
    CCardBattle* pCB;
    INT i;
    INT num;
    CBattleCard* card;
    if (m_uiState >= 6) {
        pCB = (CCardBattle*)CTaskMan::getSysTask(0x16);
        if (pCB && pCB->isNowPronounce() && !CCardBattle::isExecutingCard(0) && m_iStock >= 6) goto ok;
    }
    return 0;
ok:
    for (i = 0; i < 6; i++) {
        card = m_pCards[i];
        if (card) {
            num = i;
            break;
        }
    }
    if (!card) return 0;
    if (!pCB->setEnemyPronounceCard(m_hOwner, card)) return 0;
    m_pCards[num] = NULL;
    m_pData[num] = NULL;
    m_uiFlag |= 2;
    return 1;
}
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Game/CardRevolver", getRollSpeed__CQ210CardBattle13CCardRevolver);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Game/CardRevolver", isActive__CQ210CardBattle13CCardRevolver);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Game/CardRevolver", setChargeRate__Q210CardBattle13CCardRevolveri);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Game/CardRevolver", setReloadNG__Q210CardBattle13CCardRevolver);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Game/CardRevolver", getMode__CQ210CardBattle13CCardRevolver);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Game/CardRevolver", isNowAddCount__CQ210CardBattle13CCardRevolver);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Game/CardRevolver", getReloadID__CQ210CardBattle13CCardRevolver);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Game/CardRevolver", getStockSkill__CQ210CardBattle13CCardRevolver);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Game/CardRevolver", changeOwner__Q210CardBattle13CCardRevolveri);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Game/CardRevolver", isDiscard__Q210CardBattle13CCardRevolver);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Game/CardRevolver", isEnableCount__Q210CardBattle13CEnchantGauge);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Game/CardRevolver", getStockSkill__CQ210CardBattle12CStockFolder);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Game/CardRevolver", getStockSum__CQ210CardBattle12CStockFolder);
/* The vtables: their key functions (the destructors) are still assembly. */
__asm__(".rdata\n.align 3\n.globl _vt$Q210CardBattle12CStockFolder\n_vt$Q210CardBattle12CStockFolder:\n.word 0\n.word 0\n.word 0\n.word _$_Q210CardBattle12CStockFolder\n.word 0\n.word main__Q210CardBattle12CStockFolder\n.word 0\n.word disp__5CTask\n.word 0\n.word after__5CTask\n.word 0\n.word onOut__5CTask\n.word 0\n.word getTypeID__C5CTask\n.word 0\n.word 0\n.globl _vt$Q210CardBattle13CEnchantGauge\n_vt$Q210CardBattle13CEnchantGauge:\n.word 0\n.word 0\n.word 0\n.word _$_Q210CardBattle13CEnchantGauge\n.word 0\n.word main__Q210CardBattle13CEnchantGauge\n.word 0\n.word disp__5CTask\n.word 0\n.word after__5CTask\n.word 0\n.word onOut__5CTask\n.word 0\n.word getTypeID__C5CTask\n.word 0\n.word 0\n.globl _vt$Q210CardBattle13CCardRevolver\n_vt$Q210CardBattle13CCardRevolver:\n.word 0\n.word 0\n.word 0\n.word _$_Q210CardBattle13CCardRevolver\n.word 0\n.word main__Q210CardBattle13CCardRevolver\n.word 0\n.word disp__5CTask\n.word 0\n.word after__5CTask\n.word 0\n.word onOut__5CTask\n.word 0\n.word getTypeID__C5CTask\n.word 0\n.word 0\n.text\n");
// SOURCE DATA: .data .rodata
