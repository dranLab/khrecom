#include "khtypes.h"
#define BATTLEENEMY_H_PATH "c:/KHCOM/MainApp/Game/Enemy/BattleEnemy.h"
#define GPRIM_H_PATH "System/Graphic/GPrim.h"
/* .rodata starts with the strings of the included headers' inline functions, in this order. */
#include "System/Task.h"
#include "System/Graphic/Gprim.h"
#include "Game/Enemy/BattleEnemyManager.h"
#include "System/Graphic/2d/2DMan.h"
#include "Game/CardBattle.h"
#include "Game/Enemy/BattleEnemy.h"
#include "System/Graphic/Anim.h"
#include "System/Graphic/Str.h"
#include "Game/Collision/CollUtil.h"
/* Placeholder: an empty string follows CollUtil.h's strings in .rodata; which header it comes from is not known. */
inline void _emptyStringPlaceholder0() {
    TRACE("");
}
#include "Game/BattleCard.h"
#include "Game/Entity.h"
#include "Game/Player/Player.h"
#include "Game/Player/PlayerManager.h"
#include "Game/Game.h"
#include "Game/SceneInfo.h"
#include "Game/Debug/DebugWork.h"
#include "Game/Enemy/BattleEnemy.h"
#include "Game/Enemy/BattleEnemyManager.h"
#include "Game/MessageManager.h"
#include "Game/GameWork.h"
#include "System/TaskMan.h"
#include "System/HeapMan.h"
#include "System/Graphic/2d/2DSeqCtrl.h"
#include "System/Sound/SoundMan.h"
#include "include_asm.h"

#define RET_ADDR() ({ UINT nRA; __asm__ volatile("daddu %0, $31, $0" : "=r"(nRA)); nRA; })
inline void* operator new[](size_t size, char* msg) throw() { return com_malloc_d(size, msg, RET_ADDR()); }
inline void* operator new(size_t, void* p) throw() { return p; }

using namespace CardBattle;

namespace CardBattle {
class CStockFolder {
    BYTE _m_0x00[0xb0];

public:
    CStockFolder(::CTask* pParent, INT inUser);
    void destroy();
    INT calcEnemyStockSkill();
    void setStockInfo();
    INT getStock(INT inFlag);
    void setPronounceCard(INT inKind, INT inNumber);
    INT setPlayPronounce();
};
class CEnchantGauge : public ::CTask {
    BYTE _m_0x40[0x20];

public:
    CEnchantGauge(::CTask* pParent, INT inUser, CARD& inCard);
    virtual ~CEnchantGauge();
    void changeCount(INT inCount);
    void destroy();
};
void calcFreezeDamage(INT& ioDamage);
INT getDropPlutoCard(CARD& outData);

class CDuelGauge : public ::CTask {
private:
    UINT m_uiState;        /* 0x40 */
    UINT m_uiTimerState;   /* 0x44 */
    UINT m_uiCountState;   /* 0x48 */
    UINT m_uiCount;        /* 0x4c */
    UINT m_uiCountMax;     /* 0x50 */
    UINT m_uiResult;       /* 0x54 */
    UINT m_uiFlag;         /* 0x58 */
    TIMEQ m_tqTimer;       /* 0x5c */
    TIMEQ m_tqLimit;       /* 0x60 */
    HANDLE m_hTimerSE;     /* 0x64 */
    D2::C2DSeqCtrl* m_pSeqCtrls; /* 0x68 */

public:
    CDuelGauge(::CTask* pParent);
    virtual ~CDuelGauge();
    virtual void main();

private:
    void _initialize();
    void setTimer(UINT inState);
    void exeTimer();
    void changeTimer(INT inTime);
    void setCount(UINT inState);
    void exeCount();

public:
    UINT getState() const { return m_uiState; }
    void setStart(TIMEQ inTime, UINT inCount);
    void setResult(UINT inResult);
    TIMEQ getTimer() const { return m_tqTimer; }
    void changeCount(UINT inCount, UINT inMax);
    void setDuelNG();
    void exeDuelNG();
};
}
extern "C" char D_004772C0[]; /* "...CardBattle.cpp(NNNN)" */
extern "C" char D_004772F0[]; /* "...CardBattle.cpp(NNNN)" */
extern "C" char D_00477320[]; /* break effect name */
extern "C" char D_00477328[]; /* speed break effect name */
namespace PL {
extern FLOAT g_fDefAttr[18][6];
}
/* file-scope constant table (deferred: emitted after the vtables, as in the original) */
static const CARD_DATA cardDataTbl[224] = {
    {{0}, 0, 0, 0, ""},
    {{17}, 2, 0, 3, "Kingdom Key"},
    {{17}, 2, 1, 3, "Three Wishes"},
    {{17}, 2, 2, 3, "Crabclaw"},
    {{17}, 2, 3, 3, "Pumpkinhead"},
    {{17}, 2, 4, 3, "Fairy Harp"},
    {{17}, 2, 5, 3, "Wishing Star"},
    {{17}, 2, 6, 3, "Spellbinder"},
    {{17}, 2, 7, 3, "Metal Chocobo"},
    {{17}, 3, 0, 3, "Olympia"},
    {{17}, 3, 1, 3, "Lionheart"},
    {{17}, 3, 2, 3, "Lady Luck"},
    {{17}, 3, 3, 3, "Divine Rose"},
    {{17}, 3, 4, 3, "Oathkeeper"},
    {{17}, 3, 5, 3, "Oblivion"},
    {{17}, 3, 6, 3, "Ultima Weapon"},
    {{17}, 3, 7, 3, "Diamond Dust"},
    {{17}, 4, 0, 3, "One-Winged Angel"},
    {{17}, 4, 1, 4, "Soul Eater"},
    {{17}, 4, 2, 3, "Star Seeker"},
    {{17}, 4, 3, 3, "Monochrome"},
    {{17}, 4, 4, 3, "Follow the Wind"},
    {{17}, 4, 5, 3, "Hidden Dragon"},
    {{17}, 4, 6, 3, "Photon Debugger"},
    {{17}, 4, 7, 3, "Bond of Flame"},
    {{17}, 26, 0, 0, "-"},
    {{34}, 5, 0, 3, "Fire"},
    {{34}, 5, 1, 3, "Blizzard"},
    {{34}, 5, 2, 3, "Thunder"},
    {{34}, 5, 3, 3, "Cure"},
    {{34}, 5, 4, 3, "Gravity"},
    {{34}, 5, 5, 3, "Stop"},
    {{34}, 5, 6, 3, "Aero"},
    {{34}, 26, 2, 0, "-"},
    {{34}, 26, 1, 0, "-"},
    {{51}, 5, 7, 3, "Simba"},
    {{51}, 6, 0, 3, "Genie"},
    {{51}, 6, 1, 3, "Bambi"},
    {{51}, 6, 2, 3, "Dumbo"},
    {{51}, 6, 3, 3, "Tinker Bell"},
    {{51}, 6, 4, 3, "Mushu"},
    {{51}, 6, 5, 3, "Cloud"},
    {{68}, 6, 6, 6, "Potion"},
    {{68}, 6, 7, 6, "Hi-Potion"},
    {{68}, 7, 0, 2, "Mega-Potion"},
    {{68}, 7, 1, 2, "Ether"},
    {{68}, 7, 2, 2, "Mega-Ether"},
    {{68}, 7, 3, 2, "Elixir"},
    {{68}, 7, 4, 2, "Megalixir"},
    {{85}, 7, 5, 2, "Donald Duck"},
    {{85}, 7, 6, 2, "Goofy"},
    {{85}, 7, 7, 2, "Aladdin"},
    {{85}, 8, 0, 2, "Ariel"},
    {{85}, 8, 1, 2, "Jack"},
    {{85}, 8, 2, 2, "Peter Pan"},
    {{85}, 8, 3, 2, "The Beast"},
    {{85}, 8, 4, 4, "The King"},
    {{85}, 8, 5, 2, "Pluto"},
    {{102}, 8, 6, 6, "Guard Armor"},
    {{102}, 8, 6, 6, "Trickmaster"},
    {{102}, 8, 6, 6, "Jafar"},
    {{102}, 8, 6, 6, "Oogie Boogie"},
    {{102}, 8, 6, 6, "Parasite Cage"},
    {{102}, 8, 6, 6, "Ursula"},
    {{102}, 8, 6, 6, "Dragon Maleficent"},
    {{102}, 8, 6, 6, "Darkside"},
    {{230}, 8, 7, 4, "-"},
    {{230}, 9, 0, 0, "-"},
    {{119}, 9, 1, 6, "Shadow"},
    {{119}, 9, 2, 2, "Soldier"},
    {{119}, 9, 3, 6, "Large Body"},
    {{119}, 9, 4, 2, "Red Nocturne"},
    {{119}, 9, 5, 2, "Blue Rhapsody"},
    {{119}, 9, 6, 2, "Yellow Opera"},
    {{119}, 9, 7, 2, "Green Requiem"},
    {{119}, 10, 0, 6, "Powerwild"},
    {{119}, 10, 1, 2, "Bouncywild"},
    {{119}, 10, 2, 2, "Air Soldier"},
    {{119}, 10, 3, 2, "Bandit"},
    {{119}, 10, 4, 6, "Fat Bandit"},
    {{119}, 10, 5, 2, "Barrel Spider"},
    {{119}, 10, 6, 6, "Search Ghost"},
    {{119}, 10, 7, 6, "Sea Neon"},
    {{119}, 11, 0, 2, "Screwdiver"},
    {{119}, 11, 1, 2, "Aquatank"},
    {{119}, 11, 2, 6, "Wight Knight"},
    {{119}, 11, 3, 2, "Gargoyle"},
    {{119}, 11, 4, 6, "Pirate"},
    {{119}, 11, 5, 2, "Air Pirate"},
    {{119}, 11, 6, 4, "Darkball"},
    {{119}, 11, 7, 6, "Defender"},
    {{119}, 12, 0, 2, "Wyvern"},
    {{119}, 12, 1, 2, "Wizard"},
    {{119}, 12, 2, 2, "Neoshadow"},
    {{119}, 12, 3, 2, "White Mushroom"},
    {{119}, 12, 4, 2, "Black Fungus"},
    {{119}, 12, 5, 2, "Creeper Plant"},
    {{119}, 12, 6, 2, "Tornado Step"},
    {{119}, 12, 7, 2, "Crescendo"},
    {{119}, 13, 0, 6, "Guard Armor"},
    {{119}, 13, 1, 6, "Parasite Cage"},
    {{119}, 13, 2, 6, "Trickmaster"},
    {{119}, 13, 3, 6, "Darkside"},
    {{119}, 13, 4, 2, "Card Soldier"},
    {{119}, 13, 5, 6, "Hades"},
    {{119}, 13, 6, 6, "Jafar"},
    {{119}, 13, 7, 6, "Oogie Boogie"},
    {{119}, 14, 0, 6, "Ursula"},
    {{119}, 14, 1, 6, "Hook"},
    {{119}, 14, 2, 6, "Dragon Maleficent"},
    {{119}, 14, 3, 2, "Riku"},
    {{119}, 14, 4, 2, "Axel"},
    {{119}, 14, 5, 2, "Larxene"},
    {{119}, 14, 6, 2, "Vexen"},
    {{119}, 14, 7, 2, "Marluxia"},
    {{119}, 15, 0, 6, "Lexaeus"},
    {{119}, 15, 1, 2, "Ansem"},
    {{119}, 15, 3, 6, "Zexion"},
    {{119}, 15, 4, 2, "Xemnas"},
    {{119}, 15, 5, 2, "Xigbar"},
    {{119}, 15, 6, 2, "Xaldin"},
    {{119}, 15, 7, 2, "Sa\x83\xc7x"},
    {{119}, 16, 0, 2, "Demyx"},
    {{119}, 16, 1, 2, "Luxord"},
    {{119}, 16, 2, 2, "Roxas"},
    {{119}, 21, 6, 2, "Gold Card"},
    {{119}, 21, 7, 2, "Platinum Card"},
    {{119}, 15, 2, 0, "Card Soldier Spade"},
    {{119}, 13, 4, 0, "Card Soldier Heart"},
    {{23}, 22, 0, 0, "\x83N\x83\x89\x83" "E\x83hA"},
    {{23}, 22, 1, 0, "\x83N\x83\x89\x83" "E\x83hB"},
    {{23}, 22, 2, 0, "\x83n\x83" "f\x83XA"},
    {{39}, 22, 3, 0, "\x83n\x83" "f\x83XB"},
    {{23}, 22, 4, 0, "\x83t\x83" "b\x83NA"},
    {{23}, 22, 5, 0, "\x83t\x83" "b\x83NB"},
    {{23}, 22, 6, 0, "\x83t\x83" "b\x83NC"},
    {{39}, 22, 7, 0, "\x83t\x83" "b\x83ND"},
    {{23}, 23, 0, 0, "\x83" "A\x83N\x83Z\x83\x8b" "A"},
    {{23}, 23, 1, 0, "\x83" "A\x83N\x83Z\x83\x8b" "B"},
    {{23}, 23, 2, 0, "\x83\x89\x83N\x83V\x81[\x83kA"},
    {{23}, 23, 3, 0, "\x83\x89\x83N\x83V\x81[\x83kB"},
    {{39}, 23, 4, 0, "\x83\x89\x83N\x83V\x81[\x83kC"},
    {{23}, 23, 5, 0, "\x83\x8a\x83NA"},
    {{23}, 23, 6, 0, "\x83\x8a\x83NB"},
    {{23}, 23, 7, 0, "\x83\x8a\x83NC"},
    {{23}, 24, 0, 0, "\x83\x8a\x83ND"},
    {{23}, 24, 1, 0, "\x83\x94\x83" "B\x83N\x83Z\x83\x93" "A"},
    {{39}, 24, 2, 0, "\x83\x94\x83" "B\x83N\x83Z\x83\x93" "B"},
    {{23}, 24, 3, 0, "\x83}\x81[\x83\x8b\x81[\x83V\x83\x83" "A"},
    {{23}, 24, 4, 0, "\x83}\x81[\x83\x8b\x81[\x83V\x83\x83" "B"},
    {{39}, 24, 5, 0, "\x83}\x81[\x83\x8b\x81[\x83V\x83\x83" "C"},
    {{119}, 24, 6, 0, "\x83}\x81[\x83\x8b\x81[\x83V\x83\x83" "2"},
    {{119}, 24, 7, 0, "\x83}\x81[\x83\x8b\x81[\x83V\x83\x83" "3"},
    {{23}, 25, 0, 0, "\x83\x8c\x83N\x83Z\x83" "E\x83XA"},
    {{23}, 25, 1, 0, "\x83\x8c\x83N\x83Z\x83" "E\x83XB"},
    {{39}, 25, 2, 0, "\x83\x8c\x83N\x83Z\x83" "E\x83XC"},
    {{23}, 25, 3, 0, "\x83" "A\x83\x93\x83Z\x83\x80" "A"},
    {{39}, 25, 4, 0, "\x83" "A\x83\x93\x83Z\x83\x80" "B"},
    {{39}, 25, 5, 0, "\x83[\x83N\x83V\x83I\x83\x93" "A"},
    {{119}, 26, 3, 0, "-"},
    {{136}, 0, 0, 6, "\x83\x8a\x83\x8d\x81[\x83h"},
    {{153}, 16, 3, 6, "Tranquil Darkness"},
    {{153}, 16, 4, 6, "Teeming Darkness"},
    {{153}, 16, 5, 6, "Feeble Darkness"},
    {{153}, 16, 6, 6, "Almighty Darkness"},
    {{153}, 16, 7, 6, "Sleeping Darkness"},
    {{153}, 17, 0, 6, "Looming Darkness"},
    {{153}, 17, 1, 2, "Premium Room"},
    {{153}, 17, 2, 2, "White Room"},
    {{153}, 17, 3, 6, "Black Room"},
    {{153}, 17, 4, 6, "Bottomless Darkness"},
    {{153}, 17, 5, 6, "Roulette Room"},
    {{169}, 17, 6, 6, "Martial Waking"},
    {{169}, 17, 7, 2, "Sorcerous Waking"},
    {{169}, 18, 0, 2, "Alchemic Waking"},
    {{169}, 18, 1, 6, "Meeting Ground"},
    {{169}, 18, 2, 6, "Stagnant Space"},
    {{169}, 18, 3, 6, "Strong Initiative"},
    {{169}, 18, 4, 6, "Lasting Daze"},
    {{185}, 18, 5, 2, "Calm Bounty"},
    {{185}, 18, 6, 2, "Guarded Trove"},
    {{185}, 18, 7, 2, "False Bounty"},
    {{185}, 19, 0, 6, "Moment's Reprieve"},
    {{185}, 19, 1, 6, "Mingling Worlds"},
    {{185}, 19, 2, 2, "Moogle Room"},
    {{185}, 19, 3, 6, "Random Joker"},
    {{202}, 19, 4, 6, "Key of Beginnings"},
    {{202}, 19, 5, 6, "Key of Guidance"},
    {{202}, 19, 6, 6, "Key to Truth"},
    {{202}, 19, 7, 2, "Key to Rewards"},
    {{219}, 20, 0, 6, "Traverse Town"},
    {{219}, 20, 3, 6, "Agrabah"},
    {{219}, 20, 2, 6, "Olympus Coliseum"},
    {{219}, 20, 1, 6, "Wonderland"},
    {{219}, 20, 5, 6, "Monstro"},
    {{219}, 20, 4, 6, "Halloween Town"},
    {{219}, 20, 6, 6, "Atlantica"},
    {{219}, 20, 7, 6, "Neverland"},
    {{219}, 21, 0, 6, "Hollow Bastion"},
    {{219}, 21, 1, 2, "100 Acre Woods"},
    {{219}, 21, 2, 6, "Twilight Town"},
    {{219}, 21, 3, 6, "Destiny Islands"},
    {{219}, 21, 4, 4, "Castle Oblivion"},
    {{13}, 0, 0, 0, "EXP\x90\xc2\x8f\xac"},
    {{13}, 0, 1, 0, "EXP\x90\xc2\x92\x86"},
    {{13}, 0, 2, 0, "EXP\x90\xc2\x91\xe5"},
    {{13}, 0, 3, 0, "EXP\x90\xd4\x8f\xac"},
    {{13}, 0, 4, 0, "EXP\x90\xd4\x92\x86"},
    {{13}, 0, 5, 0, "EXP\x90\xd4\x91\xe5"},
    {{13}, 0, 6, 0, "EXP\x8b\xe2"},
    {{13}, 1, 0, 0, "HP\x8f\xac"},
    {{13}, 1, 1, 0, "HP\x91\xe5"},
    {{13}, 2, 0, 0, "MP\x8f\xac"},
    {{13}, 2, 1, 0, "MP\x91\xe5"},
    {{13}, 3, 0, 0, "P"},
    {{13}, 4, 0, 0, "R"},
    {{12}, 0, 0, 0, "Unknown Room"},
    {{12}, 0, 0, 0, "Conqueror's Respite"},
    {{12}, 0, 0, 0, "Room of Rewards"},
    {{12}, 0, 0, 0, ""},
    {{12}, 0, 0, 0, ""},
    {{0}, 0, 0, 0, ""},
    {{0}, 0, 0, 0, ""},
    {{0}, 0, 0, 0, ""},
};
/* CARD's inline constructor (Game/Card.cpp's _setCard) */
static inline void _setCard(CARD& c, USHORT nKind, BYTE nNumber) {
    c.m_nId = 0;
    c.m_nNumber2 = nNumber;
    c.m_nKind2 = nKind;
    c.m_nCategory20 = cardDataTbl[c.m_nKind].nCategory0;
    c.m_nCategory21 = cardDataTbl[c.m_nKind].nCategory1;
}
namespace D2 {
class C2DMan { /* PARTIAL: System/Graphic/2d/2DMan.h declares the classes only for its own unit */
public:
    static BOOL getLayPosition(const CHAR* pLadName, UINT inNo, UINT inIdx, FVECTOR& outPos, BOOL bLocal);
};
}
extern "C" char D_00476980[]; /* __FILE__ */
extern "C" int abs(int);
template <class T> static inline T& _ref(T& v) { return v; }

namespace CardBattle {
static LPCSTR g_szBinBattleCardList = "BattleCardList";
static LPCSTR g_szBinStockSkillList = "StockSkillList";
static LPCSTR g_szSeqCockpit = "Cockpit:00";
}
CCardBattle* CCardBattle::m_pInstance = NULL;
namespace CardBattle {
BC_PARAM* g_pBattleCardList = NULL;
SS_PARAM* g_pStockSkillList = NULL;
G_UV g_uvCRCounterNumTex[8][10] = {
    {{0, 0}, {143, 40}, {30, 0}, {49, 0}, {68, 0}, {85, 0}, {103, 0}, {120, 0}, {137, 0}, {156, 0}},
    {{19, 17}, {162, 57}, {49, 17}, {68, 17}, {85, 17}, {103, 17}, {120, 17}, {137, 17}, {156, 17}, {173, 17}},
    {{2, 19}, {16, 19}, {29, 19}, {44, 19}, {59, 19}, {73, 19}, {88, 19}, {102, 19}, {116, 19}, {131, 19}},
    {{16, 33}, {29, 33}, {44, 33}, {58, 33}, {72, 33}, {88, 33}, {102, 33}, {116, 33}, {131, 33}, {145, 33}},
    {{0, 0}, {178, 0}, {30, 0}, {49, 0}, {68, 0}, {85, 0}, {103, 0}, {120, 0}, {137, 0}, {156, 0}},
    {{19, 17}, {196, 17}, {49, 17}, {68, 17}, {85, 17}, {103, 17}, {120, 17}, {137, 17}, {156, 17}, {173, 17}},
    {{0, 20}, {0, 86}, {47, 19}, {70, 18}, {92, 18}, {115, 18}, {139, 18}, {166, 19}, {190, 18}, {218, 19}},
    {{26, 49}, {29, 117}, {70, 50}, {92, 50}, {115, 50}, {138, 51}, {165, 50}, {190, 50}, {218, 50}, {244, 50}},
};
unsigned char g_DropFriendCardList[32][8] = {
    {0, 0, 0, 0, 0, 0, 0, 0},
    {150, 50, 50, 0, 49, 50, 0, 0},
    {150, 30, 20, 50, 49, 50, 51, 0},
    {150, 60, 40, 0, 49, 50, 0, 0},
    {150, 40, 60, 0, 49, 50, 0, 0},
    {150, 50, 50, 0, 49, 50, 0, 0},
    {150, 20, 30, 50, 49, 50, 53, 0},
    {150, 20, 20, 60, 49, 50, 52, 0},
    {150, 20, 30, 50, 49, 50, 54, 0},
    {150, 30, 30, 40, 49, 50, 55, 0},
    {0, 0, 0, 0, 0, 0, 0, 0},
    {150, 50, 50, 0, 49, 50, 0, 0},
    {150, 50, 50, 0, 49, 50, 0, 0},
    {150, 50, 50, 0, 49, 50, 0, 0},
    {125, 100, 0, 0, 56, 0, 0, 0},
    {125, 100, 0, 0, 56, 0, 0, 0},
    {125, 100, 0, 0, 56, 0, 0, 0},
    {125, 100, 0, 0, 56, 0, 0, 0},
    {125, 100, 0, 0, 56, 0, 0, 0},
    {125, 100, 0, 0, 56, 0, 0, 0},
    {125, 100, 0, 0, 56, 0, 0, 0},
    {125, 100, 0, 0, 56, 0, 0, 0},
    {125, 100, 0, 0, 56, 0, 0, 0},
    {0, 0, 0, 0, 0, 0, 0, 0},
    {125, 100, 0, 0, 56, 0, 0, 0},
    {125, 100, 0, 0, 56, 0, 0, 0},
    {125, 100, 0, 0, 56, 0, 0, 0},
    {0, 0, 0, 0, 0, 0, 0, 0},
    {0, 0, 0, 0, 0, 0, 0, 0},
    {0, 0, 0, 0, 0, 0, 0, 0},
    {0, 0, 0, 0, 0, 0, 0, 0},
    {0, 0, 0, 0, 0, 0, 0, 0},
};
}

void CardBattle::calcFreezeDamage(INT& ioDamage) {
    INT nPower = g_pStockSkillList[35].POWER[1];
    ioDamage += nPower / 10;
}
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Game/CardBattle", __Q210CardBattle11CCardBattle);
__asm__(".rdata\n.align 3\n.word 0x4B2F3A63\n.word 0x4D4F4348\n.word 0x69614D2F\n.word 0x7070416E\n.word 0x6D61472F\n.word 0x61432F65\n.word 0x61426472\n.word 0x656C7474\n.word 0x7070632E\n.word 0x00000000\n.word 0x42705F67\n.word 0x6C747461\n.word 0x72614365\n.word 0x73694C64\n.word 0x00000074\n.word 0x00000000\n.word 0x53705F67\n.word 0x6B636F74\n.word 0x6C696B53\n.word 0x73694C6C\n.word 0x00000074\n.word 0x00000000\n.word 0x4D705F6D\n.word 0x67617365\n.word 0x00000065\n.word 0x00000000\n.text\n");
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Game/CardBattle", _$_Q210CardBattle11CCardBattle);
CCardBattle* CCardBattle::getInstance() {
    if (!m_pInstance) {
        m_pInstance = new (com_malloc_d(sizeof(CCardBattle), __FILE__ "(162)", RET_ADDR())) CCardBattle;
        if (!(m_pInstance)) AssertApp(D_00476980, 163, "m_pInstance", 0);
    }
    return m_pInstance;
}
void CCardBattle::init() {
    INT i;
    INT j;
    for (i = 0; i < 2; i++) {
        m_hOwner[i] = 0;
        m_uiState[i] = 0;
        m_uiResult[i] = 0;
        m_tqPlayTimer[i] = 0;
        m_mesCardInfo[i].m_nStock = 0;
        m_mesCardInfo[i].m_nExtra = 0;
        m_mesCardInfo[i].m_nFirst = 0;
        m_mesCardInfo[i].m_nBreak = 0;
        m_mesCardInfo[i].m_nBonus = 0;
        m_mesCardInfo[i].m_nWork0 = 0;
        m_mesCardInfo[i].m_nWork1 = 0;
        for (j = 0; j < 7; j++) {
            _setCard(m_cardData[i][j], 0, 0);
            m_pCard[i][j] = NULL;
            _setCard(m_mesCardInfo[i].m_cardData[j], 0, 0);
        }
        m_iCount[i] = 0;
        m_iPoint[i] = 0;
        m_iStock[i] = 0;
        m_iExtra[i] = 0;
        m_iFirst[i] = 0;
        _setCard(m_cardEnchant[i], 0, 0);
        m_pEnchantGauge[i] = NULL;
        m_nEnchantTime[i] = 0;
        m_pEnchant[i] = NULL;
        m_pRevolver[i] = NULL;
        m_pStockFolder[i] = NULL;
    }
    m_hGimmickOwner = 0;
    m_iCountGimmick = 0;
    m_iCountFriend = 0;
    m_iCountPluto = 0;
    _setCard(m_cardGimmick, 0, 0);
    m_tqMainTimer = 0;
    m_tqSubTimer = 0;
    m_uiFlag = 0;
    m_iPointBreak = 0;
    m_iPointBonus = 0;
    if (CGame::isRikuGame()) m_uiFlag |= 0x20000;
    INT cardID = 0;
    INT cardNum = 0;
    RoomInfo* pRoom = CSceneInfo::getRoomInfo(-1);
    if (pRoom) {
        cardID = pRoom->getCardID();
        cardNum = pRoom->getCardNumber();
    }
    _setCard(m_cardRoom, cardID, cardNum);
    if (!m_pSeqCtrlEnemy) {
        m_pSeqCtrlEnemy = new (com_malloc_d(sizeof(D2::C2DSeqCtrl), __FILE__ "(237)", RET_ADDR())) D2::C2DSeqCtrl(2);
    }
    initCardBreak(0, 0);
    m_uiDuelState = 0;
    m_hDuelEnemy = 0;
    setDuelBreakCount(0, 0);
    m_pDuelGauge = NULL;
    m_iDuelSkill = 0;
    UINT flag = 0x20000;
    if (m_uiFlag & flag) {
        if (CGameWork::getInt(0, 0x24) & 0x20) m_uiDuelState = 1;
        m_pDuelGauge = new (com_malloc_d(sizeof(CDuelGauge), __FILE__ "(265)", RET_ADDR())) CDuelGauge(asTask());
    }
}
void CCardBattle::main() {
    TIMEQ tqFrameRate = getFrameRate();
    UINT flag = 0x20000;
    if ((m_uiFlag & flag) && (CGameWork::getInt(0, 0x24) & 0x20)) {
        if (CGameWork::getFlag(1, 0x2A)) {
            if (m_uiDuelState == 1) m_uiDuelState = 0;
        } else {
            if (!m_uiDuelState) m_uiDuelState = 1;
        }
    }
    calcPlayingCard();
    exePlaying(1);
    exePlaying(2);
    exeRegeneration();
    exeBio();
    exeDuel();
    m_tqMainTimer += tqFrameRate;
    dispPlayingEnemy();
    dispCardBreak();
}
void CCardBattle::disp() {}
void CCardBattle::calcPlayingCard() {
    INT i;
    INT j;
    for (i = 0; i < 2; i++) {
        m_iCount[i] = 0;
        m_iPoint[i] = 0;
        for (j = 0; j < 7; j++) {
            if (m_cardData[i][j].m_nKind) {
                m_iCount[i]++;
                m_iPoint[i] += m_cardData[i][j].m_nNumber;
            }
            m_mesCardInfo[i].m_cardData[j] = m_cardData[i][j];
        }
        m_mesCardInfo[i].m_nStock = m_iStock[i];
        m_mesCardInfo[i].m_nExtra = m_iExtra[i];
        m_mesCardInfo[i].m_nFirst = m_iFirst[i];
    }
    UINT flag = 0x20000;
    if (m_uiFlag & flag) {
        if (m_iCount[1] > 0) {
            m_iPointBreak = calcPointBreak(m_iPoint[0], m_iPoint[1]);
        } else {
            m_iPointBreak = 0;
            m_iPointBonus = 0;
        }
        m_mesCardInfo[0].m_nBreak = 0;
        m_mesCardInfo[0].m_nBonus = 0;
    }
    m_uiFlag = (INT)m_uiFlag & ~0x300;
    m_uiFlag &= ~0x100000;
    for (i = 0; i < 2; i++) {
        switch (m_cardEnchant[i].m_nKind) {
        case 0x58:
            for (j = 0; j < m_iCount[i]; j++) {
                if (m_cardData[i][j].m_nCategory21 == 4) {
                    m_uiFlag |= 0x100 << i;
                    break;
                }
            }
            break;
        case 0x60:
            for (j = 0; j < m_iCount[i]; j++) {
                if (m_cardData[i][j].m_nKind == 0x1D) {
                    m_uiFlag |= 0x100 << i;
                    break;
                }
            }
            break;
        case 0x69:
            for (j = 0; j < m_iCount[i]; j++) {
                if (m_cardData[i][j].m_nCategory21 == 1) {
                    m_uiFlag |= 0x100 << i;
                    break;
                }
            }
            break;
        case 0x7B:
            if (i == 0) setFlag(0x100000, 0);
            break;
        }
        if (m_cardData[i][0].m_nCategory20 == 6) m_uiFlag |= 0x100 << i;
    }
}
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Game/CardBattle", exePlaying__Q210CardBattle11CCardBattlei);
__asm__(".rdata\n.align 3\n.word 0x4C544243\n.word 0x78653A3A\n.word 0x616C5065\n.word 0x676E6979\n.word 0x5453203B\n.word 0x5F455441\n.word 0x52415453\n.word 0x4C503A54\n.word 0x4D49542D\n.word 0x64253D45\n.word 0x4E45202C\n.word 0x4D49542D\n.word 0x64253D45\n.word 0x0000000A\n.word 0x4C544243\n.word 0x78653A3A\n.word 0x616C5065\n.word 0x676E6979\n.word 0xA681203B\n.word 0x85836683\n.word 0x8B834783\n.word 0xB690AD94\n.word 0x64284981\n.word 0x00000A29\n.word 0x4C544243\n.word 0x78653A3A\n.word 0x616C5065\n.word 0x676E6979\n.word 0xA681203B\n.word 0x85836683\n.word 0x8B834783\n.word 0xB690AD94\n.word 0x61284981\n.word 0x00000A29\n.word 0x4C544243\n.word 0x78653A3A\n.word 0x616C5065\n.word 0x676E6979\n.word 0xA681203B\n.word 0x458E8A91\n.word 0x73763028\n.word 0xAD942930\n.word 0x4981B690\n.word 0x0A296128\n.word 0x00000000\n.word 0x00000000\n.word 0x4C544243\n.word 0x78653A3A\n.word 0x616C5065\n.word 0x676E6979\n.word 0xA681203B\n.word 0x73835883\n.word 0x68835B81\n.word 0x8C837583\n.word 0x4E834383\n.word 0xB690AD94\n.word 0x61284981\n.word 0x64252029\n.word 0x25202B20\n.word 0x00000A64\n.word 0x3A3A4243\n.word 0x50657865\n.word 0x6979616C\n.word 0x2020676E\n.word 0x54415453\n.word 0x58455F45\n.word 0x5F203A45\n.word 0x3E2D4C50\n.word 0x42746573\n.word 0x6B616572\n.word 0x000A2928\n.word 0x00000000\n.word 0x4C544243\n.word 0x78653A3A\n.word 0x616C5065\n.word 0x676E6979\n.word 0xA681203B\n.word 0x85836683\n.word 0x8B834783\n.word 0xB690AD94\n.word 0x62284981\n.word 0x00000A29\n.word 0x4C544243\n.word 0x78653A3A\n.word 0x616C5065\n.word 0x676E6979\n.word 0xA681203B\n.word 0x458E8A91\n.word 0x73763028\n.word 0xAD942930\n.word 0x4981B690\n.word 0x0A296228\n.word 0x00000000\n.word 0x00000000\n.word 0x3A3A4243\n.word 0x50657865\n.word 0x6979616C\n.word 0x2020676E\n.word 0x54415453\n.word 0x58455F45\n.word 0x30203A45\n.word 0x64257376\n.word 0x4C505F20\n.word 0x65733E2D\n.word 0x65724274\n.word 0x25286B61\n.word 0x000A2964\n.word 0x00000000\n.word 0x4C544243\n.word 0x78653A3A\n.word 0x616C5065\n.word 0x676E6979\n.word 0xA681203B\n.word 0x85836683\n.word 0x8B834783\n.word 0xB690AD94\n.word 0x63284981\n.word 0x00000A29\n.word 0x3A3A4243\n.word 0x50657865\n.word 0x6979616C\n.word 0x2020676E\n.word 0x54415453\n.word 0x58455F45\n.word 0x25203A45\n.word 0x25737664\n.word 0x505F2064\n.word 0x733E2D4C\n.word 0x72427465\n.word 0x286B6165\n.word 0x0A296425\n.word 0x00000000\n.word 0x3A3A4243\n.word 0x50657865\n.word 0x6979616C\n.word 0x2020676E\n.word 0x54415453\n.word 0x45525F45\n.word 0x43455246\n.word 0x25203A54\n.word 0x25737664\n.word 0x455F2064\n.word 0x733E2D4E\n.word 0x72427465\n.word 0x286B6165\n.word 0x0A296425\n.word 0x00000000\n.word 0x00000000\n.word 0x4C544243\n.word 0x78653A3A\n.word 0x616C5065\n.word 0x676E6979\n.word 0x5453203B\n.word 0x5F455441\n.word 0x54494157\n.word 0x2C642520\n.word 0x52524520\n.word 0x2021524F\n.word 0x452E2E2E\n.word 0x0A2E444E\n.word 0x00000000\n.word 0x00000000\n.word 0x4C544243\n.word 0x78653A3A\n.word 0x616C5065\n.word 0x676E6979\n.word 0x6148203B\n.word 0x656C646E\n.word 0x2064253D\n.word 0x5453203A\n.word 0x5F455441\n.word 0x0A444E45\n.word 0x00000000\n.word 0x00000000\n.word 0x4C544243\n.word 0x78653A3A\n.word 0x616C5065\n.word 0x676E6979\n.word 0x5453203B\n.word 0x5F455441\n.word 0x4C455544\n.word 0x4941575F\n.word 0x64252054\n.word 0x5245202C\n.word 0x21524F52\n.word 0x2E2E2E20\n.word 0x2E444E45\n.word 0x0000000A\n.word 0x3A3A4243\n.word 0x50657865\n.word 0x6979616C\n.word 0x2020676E\n.word 0x54415453\n.word 0x55445F45\n.word 0x505F4C45\n.word 0x5F59414C\n.word 0x3A455845\n.word 0x76642520\n.word 0x20642573\n.word 0x2D4E455F\n.word 0x7465733E\n.word 0x61657242\n.word 0x0A29286B\n.word 0x00000000\n.word 0x4C544243\n.word 0x78653A3A\n.word 0x616C5065\n.word 0x676E6979\n.word 0x6148203B\n.word 0x656C646E\n.word 0x2064253D\n.word 0x5453203A\n.word 0x5F455441\n.word 0x4C455544\n.word 0x414C505F\n.word 0x4E455F59\n.word 0x00000A44\n.word 0x00000000\n.word 0x3A3A4243\n.word 0x50657865\n.word 0x6979616C\n.word 0x2020676E\n.word 0x54415453\n.word 0x52505F45\n.word 0x554F4E4F\n.word 0x5F45434E\n.word 0x3A455845\n.word 0x76642520\n.word 0x20642573\n.word 0x2D4E455F\n.word 0x7465733E\n.word 0x61657242\n.word 0x0A29286B\n.word 0x00000000\n.word 0x4C544243\n.word 0x78653A3A\n.word 0x616C5065\n.word 0x676E6979\n.word 0x6148203B\n.word 0x656C646E\n.word 0x2064253D\n.word 0x5453203A\n.word 0x5F455441\n.word 0x4E4F5250\n.word 0x434E554F\n.word 0x4E455F45\n.word 0x00000A44\n.word 0x00000000\n.word 0x00000000\n.word 0x00000000\n.word .L001DAA6C\n.word .L001DAA80\n.word .L001DB160\n.word .L001DB97C\n.word .L001DBB5C\n.word .L001DBBD8\n.word .L001DBD54\n.word .L001DBDF8\n.word .L001DC2D4\n.word .L001DBE78\n.word .L001DBEBC\n.word .L001DC2D4\n.word .L001DC2D4\n.word .L001DC03C\n.word .L001DC04C\n.word .L001DC090\n.word .L001DC2D4\n.word .L001DC2D4\n.word .L001DC1F4\n.word 0\n.text\n");
void CCardBattle::setNumberBreak(UINT _PLAYER, UINT _ENEMY) {
    if (m_cardEnchant[_PLAYER].m_nKind == 0x65) {
        INT iCountE = m_iCount[_ENEMY];
        INT iPoint = m_iPoint[_PLAYER];
        for (INT i = 0; i < iCountE; i++) {
            CBattleCard* pCard = m_pCard[_ENEMY][i];
            if (pCard) {
                INT iPointE = pCard->getCardData()->m_nNumber;
                pCard->setNumberBreak(iPoint);
                iPoint -= iPointE;
                if (iPoint < 0) iPoint = 0;
                m_cardData[_ENEMY][i] = *m_pCard[_ENEMY][i]->getCardData();
            }
        }
    }
}
BOOL CCardBattle::checkNowPlaying(INT inSide, CBattleCard* pCard) {
    if (!(inSide)) AssertApp(D_00476980, 1425, "inUser", 0);
    for (INT i = 0; i < 7; i++) {
        if (m_pCard[inSide - 1][i] == pCard) return TRUE;
    }
    return FALSE;
}
/* Parked: final return only - the original computes 4 into v0 and movn's 1 from v1 when iDuelOK (the != path sets v0 = 1 in the bne delay slot, one exit); ours keeps 1 in v1/v0 and movz's or branches; tried: ~35 return forms (?: nests, ret locals, if chains, && forms), iDuelOK as INT/UINT/BOOL/SHORT and its if/ternary forms, inPlayer vs _PLAYER uses, permuter 4 min (7128 candidates, no gain) */
#if 0
INT CCardBattle::testPlayingCard(UINT inPlayer) {
    UINT _PLAYER = inPlayer;
    UINT _ENEMY = _PLAYER ^ 1;
    switch (m_uiState[_ENEMY]) {
    case 1:
    case 2:
    case 9:
    case 10:
    case 14:
    case 15:
        break;
    default:
        return 1;
    }
    INT iPoint = 0;
    INT iPointE = m_iPoint[_ENEMY];
    for (INT i = 0; i < 7; i++) {
        if (m_cardData[_PLAYER][i].m_nKind) iPoint += m_cardData[_PLAYER][i].m_nNumber;
    }
    if (m_uiState[_PLAYER] == 9 || m_uiState[_PLAYER] == 14) {
        if (!iPoint || iPoint >= iPointE) return 1;
        return 4;
    }
    if (m_cardEnchant[_PLAYER].m_nKind == 0x7B) return 1;
    if (m_cardData[_PLAYER][0].m_nCategory20 == 6) return 1;
    INT iDuelOK = FALSE;
    if (inPlayer == 0 && m_uiDuelState == 1 && !(m_uiFlag & 0x600000)) iDuelOK = TRUE;
    if (iDuelOK && (m_uiFlag & 0x80000)) return 1;
    if (m_uiFlag & (0x100 << _ENEMY)) return 4;
    if (iPoint > 0 && iPoint < iPointE) return 4;
    return (iPoint != iPointE) ? 1 : (!iDuelOK ? 4 : 1);
}
#else
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Game/CardBattle", testPlayingCard__Q210CardBattle11CCardBattleUi);
#endif
__asm__(".rdata\n.align 3\n.word .L001DC4E0\n.word .L001DC4E0\n.word .L001DC5BC\n.word .L001DC5BC\n.word .L001DC5BC\n.word .L001DC5BC\n.word .L001DC5BC\n.word .L001DC5BC\n.word .L001DC4E0\n.word .L001DC4E0\n.word .L001DC5BC\n.word .L001DC5BC\n.word .L001DC5BC\n.word .L001DC4E0\n.word .L001DC4E0\n.word 0\n.text\n");
INT CCardBattle::testBreakingCard(UINT inPlayer) {
    UINT _PLAYER = inPlayer - 1;
    UINT _ENEMY = _PLAYER ^ 1;
    if (!isPlayingCardStandNow(inPlayer)) {
        TRACE("CCardBattle::testBreakingCard \x81\xa6pCB->isPlayingCardStandNow==NG! \x81\xa6No.2779 \x91\xce\x8f\x88" "1\n");
        return 0;
    }
    switch (m_uiState[_ENEMY]) {
    case 1:
    case 2:
    case 9:
    case 10:
    case 14:
    case 15:
        break;
    default:
        TRACE("CCardBattle::testBreakingCard \x81\xa6pCB->isPlayingCardStandNow==OK! \x81\xa6No.2779 \x91\xce\x8f\x88" "2\n");
        return 2;
    }
    if (m_cardEnchant[_ENEMY].m_nKind == 0x7B) return 0;
    if (m_cardData[_PLAYER][0].m_nCategory20 == 6) return 0;
    INT iDuelOK = FALSE;
    if (_PLAYER == 0 && m_uiDuelState == 1 && !(m_uiFlag & 0x600000)) iDuelOK = TRUE;
    if (iDuelOK && (m_uiFlag & 0x80000)) return 0;
    if (m_uiFlag & (0x100 << _PLAYER)) {
        TRACE("CCardBattle::testBreakingCard \x81\xa6pCB->isPlayingCardStandNow==OK! \x81\xa6No.2779 \x91\xce\x8f\x88" "3\n");
        return 2;
    }
    INT iPoint = m_iPoint[_PLAYER];
    INT iPointE = m_iPoint[_ENEMY];
    if (!iPointE) return 0;
    if (iPointE < iPoint) {
        TRACE("CCardBattle::testBreakingCard \x81\xa6pCB->isPlayingCardStandNow==OK! \x81\xa6No.2779 \x91\xce\x8f\x88" "4\n");
        return 2;
    }
    return 0;
}
INT CCardBattle::setPlayCard(HANDLE inHandle, INT inUser, CBattleCard* inCard, CARD* inData, INT inTest) {
    if (!(inUser)) AssertApp(D_00476980, 1660, "inUser", 0);
    UINT _PLAYER = inUser - 1;
    if (m_uiState[_PLAYER]) return 0;
    m_hOwner[_PLAYER] = inHandle;
    m_uiState[_PLAYER] = 1;
    m_uiResult[_PLAYER] = 0;
    m_tqPlayTimer[_PLAYER] = 0;
    CBattleCard* pOld = m_pCard[_PLAYER][0];
    if (pOld && !pOld->getRevolver()) pOld->out();
    if (inCard) {
        m_pCard[_PLAYER][0] = inCard;
        inCard->setEnchantEffect(0x44, 0);
        m_pCard[_PLAYER][0]->setEnchantEffect(0x52, 0);
        m_pCard[_PLAYER][0]->setEnchantEffect(0x53, 0);
        m_cardData[_PLAYER][0] = *inCard->getCardData();
        checkNowPlaying(inUser, inCard);
    } else {
        if (!(inData)) AssertApp(D_00476980, 1708, "inData", 0);
        m_cardData[_PLAYER][0] = *inData;
        m_pCard[_PLAYER][0] = new (com_malloc_d(sizeof(CBattleCard), __FILE__ "(1710)", RET_ADDR()))
            CBattleCard(asTask(), inData->m_nKind, inData->m_nNumber, inUser, 0);
        if (m_pCard[_PLAYER][0]) m_pCard[_PLAYER][0]->setRoomEffect(NULL);
        m_cardData[_PLAYER][0] = *m_pCard[_PLAYER][0]->getCardData();
    }
    m_iStock[_PLAYER] = 0;
    m_iExtra[_PLAYER] = 0;
    m_iFirst[_PLAYER] = 0;
    m_pCard[_PLAYER][0]->setPlayCard();
    if (isNowDuel()) {
        m_uiState[_PLAYER] = 9;
        m_pCard[_PLAYER][0]->setFlagDuelPlay();
    }
    if (isNowPronounce()) {
        m_uiState[_PLAYER] = 14;
        m_pCard[_PLAYER][0]->setFlagPronouncePlay();
    }
    if (inTest) return testPlayingCard(_PLAYER);
    return 1;
}
/* Parked: induction variable - the original's copy loop runs on &m_pCard[_PLAYER][i] (cardData at -0x38), ours on
 * &m_cardData[_PLAYER][i] (m_pCard at +0x38); all else matches; tried: pointer/index forms of inCard (in-body and
 * for-header increments), the m_pCard store per branch, an m_pCard row pointer, call/copy order, permuter 4 min
 * (7957 candidates, no gain) */
#if 0
INT CCardBattle::setPlayStock(HANDLE inHandle, INT inUser, CBattleCard** inCard, INT inSkill, INT inExtra, INT inFirst,
                              INT inTest) {
    if (!(inUser)) AssertApp(D_00476980, 1754, "inUser", 0);
    UINT _PLAYER = inUser - 1;
    if (m_uiState[_PLAYER]) return 0;
    m_hOwner[_PLAYER] = inHandle;
    m_uiState[_PLAYER] = 1;
    m_uiResult[_PLAYER] = 0;
    m_tqPlayTimer[_PLAYER] = 0;
    INT max = inSkill < 4 ? inSkill : 3;
    INT count = 0;
    INT i;
    for (i = 0; i < max; i++) {
        if (inCard[i]) count++;
    }
    for (i = 0; i < max; i++) {
        CBattleCard* pCard = *inCard;
        m_pCard[_PLAYER][i] = pCard;
        if (pCard) {
            m_cardData[_PLAYER][i] = *pCard->getCardData();
            pCard->setPlayStock(count);
        } else {
            _setCard(m_cardData[_PLAYER][i], 0, 0);
        }
        inCard++;
    }
    m_iStock[_PLAYER] = inSkill;
    m_iExtra[_PLAYER] = inExtra;
    m_iFirst[_PLAYER] = inFirst;
    if (!inTest) return 1;
    return testPlayingCard(_PLAYER);
}
#else
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Game/CardBattle", setPlayStock__Q210CardBattle11CCardBattleiiPPQ210CardBattle11CBattleCardiiii);
#endif
INT CCardBattle::setEnchantCard(HANDLE inHandle, INT inUser, CBattleCard* inCard) {
    if (!(inUser)) AssertApp(D_00476980, 1823, "inUser", 0);
    UINT _PLAYER = inUser - 1;
    if (m_uiState[_PLAYER]) return 0;
    if (m_pEnchant[_PLAYER]) resetEnchantCard(inUser);
    m_pEnchant[_PLAYER] = inCard;
    m_cardEnchant[_PLAYER] = *inCard->getCardData();
    inCard->setEnchant();
    CSound::playSE(0x20, -1, 0x40);
    if (m_pEnchantGauge[_PLAYER]) delete m_pEnchantGauge[_PLAYER];
    m_pEnchantGauge[_PLAYER] = NULL;
    m_pEnchantGauge[_PLAYER] = new (com_malloc_d(sizeof(CEnchantGauge), __FILE__ "(1858)", RET_ADDR()))
        CEnchantGauge(asTask(), inUser, m_cardEnchant[_PLAYER]);
    m_nEnchantTime[_PLAYER] = _ref(g_pBattleCardList)[m_cardEnchant[_PLAYER].m_nKind].EFF_TIMES;
    if (_PLAYER == 0) setPlayerEnchant(m_cardEnchant[0].m_nKind);
    UINT _ENEMY = _PLAYER ^ 1;
    switch (m_cardEnchant[_PLAYER].m_nKind) {
    case 0x55:
        m_uiFlag |= 0x4000;
        break;
    case 0x5D: {
        UINT flag = 0x1000 << _ENEMY;
        m_uiFlag |= flag;
        m_tqEnchantTimer[_ENEMY] = 0;
        break;
    }
    case 0x5F: {
        INT kind = m_cardEnchant[_PLAYER].m_nKind;
        while (kind == 0x5F || kind == 0x66 || (kind == 0x59 && !(m_uiFlag & 0x20000))) {
            if (CGameWork::getInt(0, 0x2D)) {
                kind = RND::genrand_int31() % 0x39 + 0x44;
            } else {
                kind = RND::genrand_int31() % 0x32 + 0x44;
            }
        }
        m_cardEnchant[_PLAYER].m_nKind = kind;
        inCard->changeData(m_cardEnchant[_PLAYER]);
        setEnchantCard(inHandle, inUser, inCard);
        return 1;
    }
    case 0x64:
        if (m_cardEnchant[_ENEMY].m_nKind) resetEnchantCard(_ENEMY + 1);
        resetEnchantCard(inUser);
        return 1;
    case 0x66:
        if (m_cardEnchant[_ENEMY].m_nKind) {
            m_cardEnchant[_PLAYER].m_nKind = m_cardEnchant[_ENEMY].m_nKind;
            inCard->changeData(m_cardEnchant[_PLAYER]);
            setEnchantCard(inHandle, inUser, inCard);
        } else {
            resetEnchantCard(inUser);
        }
        break;
    case 0x6A: {
        UINT flag = 0x400 << _PLAYER;
        m_uiFlag |= flag;
        m_tqEnchantTimer[_PLAYER] = 0;
        break;
    }
    case 0x4B:
    case 0x57:
        inCard->getRevolver()->rollDeck(0, 1, 300);
        return 1;
    case 0x59:
        if (CGame::isRikuGame()) m_uiFlag |= 0x80000;
        break;
    case 0x5C:
        if (inCard->getRevolver()) {
            inCard->getRevolver()->setPlayNG(1, 3);
            return 1;
        }
        break;
    case 0x62:
        if (inCard->getRevolver()) {
            inCard->getRevolver()->setPlayNG(1, 2);
        }
        break;
    }
    return 1;
}
/* Parked: address forms - the original computes &m_cardEnchant[_PLAYER] as (idx*4+0x17c)+base and loads the kind with
 * offset 0 (kept in two registers), and &m_nEnchantTime[_PLAYER] as (base+0x184)+idx*4; ours folds the offsets the
 * other way round; the reload of m_pInstance after the decrement matches via the INT* pTime pointer; tried: CARD view
 * struct / reference / pointer locals for the enchant, BYTE/INT/UINT kind, _ref forms, compare operand order,
 * permuter 4 min (8263 candidates, no gain) */
#if 0
void CCardBattle::decEnchantTime(INT inUser, INT inPart, INT inKind) {
    if (!(inUser)) AssertApp(D_00476980, 1977, "inUser", 0);
    UINT _PLAYER = inUser - 1;
    INT kind = m_pInstance->m_cardEnchant[_PLAYER].m_nKind;
    if (!kind) return;
    if (inPart != g_pBattleCardList[kind].EFF_PART) return;
    if (inKind && inKind != kind - 0x43) return;
    CEnchantGauge* pGauge = m_pInstance->m_pEnchantGauge[_PLAYER];
    if (!pGauge) return;
    INT* pTime = &m_pInstance->m_nEnchantTime[_PLAYER];
    (*pTime)--;
    if (_ref(m_pInstance)->m_nEnchantTime[_PLAYER] <= 0) {
        pGauge->changeCount(0);
        m_pInstance->resetEnchantCard(inUser);
    } else {
        pGauge->changeCount(_ref(m_pInstance)->m_nEnchantTime[_PLAYER]);
    }
}
#else
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Game/CardBattle", decEnchantTime__Q210CardBattle11CCardBattleiii);
#endif
void CCardBattle::destroyPlayCard(INT inFlag, INT inP1) { resetNowPlaying(inFlag); }
void CCardBattle::resetNowPlaying(INT inUser) {
    if (!(inUser)) AssertApp(D_00476980, 2028, "inUser", 0);
    UINT _PLAYER = inUser - 1;
    if (inUser == 1) {
        if (m_iStock[_PLAYER] > 0) {
            CGameWork::setFlag(1, 0x16, TRUE);
        } else {
            CGameWork::setFlag(1, 0x14, TRUE);
        }
        _setCard(m_cardGimmick, 0, 0);
        m_uiFlag &= ~0x8000;
    }
    if (m_iStock[_PLAYER] > 0) decEnchantTime(inUser, 0x2A, 0);
    m_uiResult[_PLAYER] = 0;
    m_uiState[_PLAYER] = 0;
    m_hOwner[_PLAYER] = 0;
    for (INT i = 0; i < 7; i++) {
        _setCard(m_cardData[_PLAYER][i], 0, 0);
        if (m_pCard[_PLAYER][i]) {
            m_pCard[_PLAYER][i]->setUseEnd();
            m_pCard[_PLAYER][i] = NULL;
        }
    }
    m_iStock[_PLAYER] = 0;
    m_iExtra[_PLAYER] = 0;
    m_iFirst[_PLAYER] = 0;
}
void CCardBattle::resetEnchantCard(INT inUser) {
    if (!(inUser)) AssertApp(D_00476980, 2069, "inUser", 0);
    UINT _PLAYER = inUser - 1;
    if (!_PLAYER) {
        resetPlayerEnchant(m_cardEnchant[0].m_nKind);
        m_uiFlag &= ~0x80000;
        m_uiFlag = (INT)m_uiFlag & ~0x4000;
    }
    m_uiFlag &= ~(0x400 << _PLAYER);
    m_uiFlag &= ~(0x1000 << (_PLAYER ^ 1));
    _setCard(m_cardEnchant[_PLAYER], 0, 0);
    m_nEnchantTime[_PLAYER] = 0;
    if (m_pEnchantGauge[_PLAYER]) m_pEnchantGauge[_PLAYER]->destroy();
    m_pEnchantGauge[_PLAYER] = NULL;
    m_pEnchant[_PLAYER] = NULL;
}
HANDLE CCardBattle::getPlayingHandle(INT inSide) {
    if (!m_pInstance) return 0;
    if ((UINT)inSide < 2 && m_pInstance->m_uiState[0]) return m_pInstance->m_hOwner[0];
    if ((inSide == 0 || inSide == 2) && m_pInstance->m_uiState[1]) return m_pInstance->m_hOwner[1];
    return 0;
}
BOOL CCardBattle::isStandingCard(INT inSide) {
    if (!m_pInstance) return FALSE;
    if ((UINT)inSide < 2) {
        switch (m_pInstance->m_uiState[0]) {
        case 1:
        case 2:
        case 6:
        case 7:
        case 8:
        case 9:
        case 10:
        case 14:
        case 15:
            return TRUE;
        }
    }
    if (inSide == 0 || inSide == 2) {
        switch (m_pInstance->m_uiState[1]) {
        case 1:
        case 2:
        case 6:
        case 7:
        case 8:
        case 9:
        case 10:
        case 14:
        case 15:
            return TRUE;
        }
    }
    return FALSE;
}
BOOL CCardBattle::isPlayingCardStandNow(INT inSide) {
    if (!m_pInstance) return FALSE;
    if ((UINT)inSide < 2) {
        switch (m_pInstance->m_uiState[0]) {
        case 2:
        case 10:
        case 15:
            return TRUE;
        }
    }
    if (inSide == 0 || inSide == 2) {
        switch (m_pInstance->m_uiState[1]) {
        case 2:
        case 10:
        case 15:
            return TRUE;
        }
    }
    return FALSE;
}
BOOL CCardBattle::isExecutingCard(INT inSide) {
    if (!m_pInstance) return FALSE;
    if ((UINT)inSide < 2) {
        switch (m_pInstance->m_uiState[0]) {
        case 1:
        case 2:
        case 3:
        case 4:
        case 6:
        case 7:
        case 8:
        case 9:
        case 10:
        case 11:
        case 12:
        case 14:
        case 15:
        case 16:
        case 17:
            return TRUE;
        }
    }
    if (inSide == 0 || inSide == 2) {
        switch (m_pInstance->m_uiState[1]) {
        case 1:
        case 2:
        case 3:
        case 4:
        case 6:
        case 7:
        case 8:
        case 9:
        case 10:
        case 11:
        case 12:
        case 14:
        case 15:
        case 16:
        case 17:
            return TRUE;
        }
    }
    return FALSE;
}
BOOL CCardBattle::isPlayingCard(INT inSide) {
    if (!m_pInstance) return FALSE;
    if ((UINT)inSide < 2 && !m_pInstance->m_uiState[0]) return FALSE;
    if (inSide == 0 || inSide == 2) {
        if (!m_pInstance->m_uiState[1]) return FALSE;
    }
    return TRUE;
}
BOOL CCardBattle::isPlayingEnemyCard() {
    if (!m_pInstance) return FALSE;
    return m_pInstance->m_uiState[1] != 0;
}
INT CCardBattle::getPlayingCardPoint(INT inSide) {
    if (!m_pInstance) return -1;
    if ((UINT)inSide < 2 && m_pInstance->m_uiState[0]) return m_pInstance->m_iPoint[0];
    if ((inSide == 0 || inSide == 2) && m_pInstance->m_uiState[1]) return m_pInstance->m_iPoint[1];
    return -1;
}
INT CCardBattle::getPlayingCardID(INT inSide, INT inIdx) {
    if (!m_pInstance) return -1;
    if ((UINT)inSide < 2 && m_pInstance->m_uiState[0]) return m_pInstance->m_cardData[0][inIdx].m_nKind;
    if ((inSide == 0 || inSide == 2) && m_pInstance->m_uiState[1]) return m_pInstance->m_cardData[1][inIdx].m_nKind;
    return -1;
}
void CCardBattle::addGimmickCount(INT n) {
    if (m_pInstance) m_pInstance->m_iCountGimmick += n;
}
INT CCardBattle::getGimmickCount() {
    if (!m_pInstance) return -1;
    return m_pInstance->m_iCountGimmick;
}
void CCardBattle::addFriendCount(INT n) {
    if (m_pInstance) m_pInstance->m_iCountFriend += n;
}
INT CCardBattle::getFriendCount() {
    if (!m_pInstance) return -1;
    return m_pInstance->m_iCountFriend;
}
void CCardBattle::addPlutoCount(INT n) {
    if (m_pInstance) m_pInstance->m_iCountPluto += n;
}
INT CCardBattle::getPlutoCount() {
    if (!m_pInstance) return -1;
    return m_pInstance->m_iCountPluto;
}
void CCardBattle::setGimmickOwner(HANDLE h) {
    if (m_pInstance) m_pInstance->m_hGimmickOwner = h;
}
HANDLE CCardBattle::getGimmickOwner() {
    if (!m_pInstance) return 0;
    return m_pInstance->m_hGimmickOwner;
}
void CCardBattle::notifyMakeGimmick(CARD* inData, INT inFlag) {
    if (!m_pInstance) return;
    if (inData) m_pInstance->m_cardGimmick = *inData;
    _ref(m_pInstance)->m_uiFlag |= 0x8000;
    if (inFlag) return;
    CCardBattle* p = _ref(m_pInstance);
    if (!p->m_hGimmickOwner) return;
    MES_CARD_INFO mesInfo;
    mesInfo.m_nStock = 0;
    mesInfo.m_nExtra = 0;
    mesInfo.m_nFirst = 0;
    mesInfo.m_cardData[0] = p->m_cardGimmick;
    _setCard(mesInfo.m_cardData[1], 0, 0);
    _setCard(mesInfo.m_cardData[2], 0, 0);
    p->m_pMesage->sendMessage(0, p->getHandle(), p->m_hGimmickOwner, 6, &mesInfo);
}
void CCardBattle::notifyCompleteGimmick() {
    if (m_pInstance) {
        TRACE("CB:: notifyCompleteGimmick() =%d,\n", m_pInstance->m_cardGimmick.m_nKind);
        _setCard(m_pInstance->m_cardGimmick, 0, 0);
        _ref(m_pInstance)->m_uiFlag &= ~0x8000;
    }
}
BOOL CCardBattle::isPlayingGimmickCard() {
    if (!m_pInstance) return FALSE;
    return m_pInstance->m_cardGimmick.m_nKind;
}
void CCardBattle::initPlayingEnemy(HANDLE inHandle) {
    m_uiFlag |= 1;
    m_pSeqCtrlEnemy->destroy();
    m_pSeqCtrlEnemy->setLayer(2);
    m_pSeqCtrlEnemy->addSeq(g_szSeqCockpit, 100);
    CB_LAYOUT* pLayout = &g_LayoutData[16];
    FVECTOR pos = {{pLayout->x, pLayout->y, 0.0f, 1.0f}};
    m_pSeqCtrlEnemy->setPos(pos);
}
void CCardBattle::dispPlayingEnemy() {
    UINT flag = 1;
    if (m_uiFlag & flag) {
        CEntity* pEntity;
        if (m_pSeqCtrlEnemy->isEnd(0) || CGameWork::getInt(1, 0xE2) || !m_hOwner[1] ||
            !(pEntity = (CEntity*)CTaskMan::getTask(m_hOwner[1]))) {
            m_uiFlag = (INT)m_uiFlag & ~1;
            m_pSeqCtrlEnemy->destroy();
        } else {
            FVECTOR pos;
            pEntity->getUseCardPos(pos);
            m_pSeqCtrlEnemy->set3DPos(pos);
        }
    }
}
/* Parked: registers/scheduling only (12 lines) - the m_pSeqCtrlBreak null test loads into v0 (ours a0, reused by the
 * else call), and in the speed branch the original stores m_hSpeed before loading m_pSeqCtrlSpeed for enableDisp
 * (jal delay slot); tried: _ref/== NULL test forms, m_hBreak store placement, permuter 4 min (379 candidates) */
#if 0
void CCardBattle::initCardBreak(HANDLE inHandle, UINT inUser) {
    m_vBreakPos.init();
    if (!inHandle) {
        m_hBreak = 0;
        if (!m_pSeqCtrlBreak) {
            m_pSeqCtrlBreak = new (com_malloc_d(sizeof(D2::C2DSeqCtrl), D_004772C0, RET_ADDR())) D2::C2DSeqCtrl(2);
            m_pSeqCtrlBreak->setLayer(7);
            m_pSeqCtrlBreak->addSeq(g_szSeqCockpit, 1);
            UINT flag = 0x20000;
            if (m_uiFlag & flag) {
                m_pSeqCtrlBreak->addSeq(g_szSeqCockpit, 2);
                m_pSeqCtrlBreak->enableColorCtrl(TRUE);
            }
            m_pSeqCtrlBreak->enableDisp(FALSE);
        } else {
            m_pSeqCtrlBreak->enableDisp(FALSE);
        }
        m_hSpeed = 0;
        if (!m_pSeqCtrlSpeed) {
            m_pSeqCtrlSpeed = new (com_malloc_d(sizeof(D2::C2DSeqCtrl), D_004772F0, RET_ADDR())) D2::C2DSeqCtrl(2);
            UINT flag = 0x20000;
            if (m_uiFlag & flag) {
                m_pSeqCtrlSpeed->setLayer(7);
                m_pSeqCtrlSpeed->addSeq(g_szSeqCockpit, 0x95);
                m_pSeqCtrlSpeed->addSeqShift(g_szSeqCockpit, 2, 60.0f, 0.0f);
                m_pSeqCtrlSpeed->addSeqShift(g_szSeqCockpit, 2, 80.0f, 0.0f);
                m_pSeqCtrlSpeed->enableColorCtrl(TRUE);
                m_pSeqCtrlSpeed->enableDisp(FALSE);
            }
        } else {
            m_pSeqCtrlSpeed->enableDisp(FALSE);
        }
        return;
    }
    CEntity* pEntity = (CEntity*)CTaskMan::getTask(inHandle);
    if (!pEntity) return;
    pEntity->getUseCardPos(m_vBreakPos);
    if (!m_iPointBonus || inUser == 1) {
        pEntity->setEffect(D_00477320, 0, &m_vBreakPos, NULL, 0.0f);
        pEntity->getHeadPos(m_vBreakPos);
        m_hBreak = inHandle;
        m_pSeqCtrlBreak->enableDisp(TRUE);
        m_pSeqCtrlBreak->changeTexture(1, 0, NULL, &g_uvCRCounterNumTex[0][m_iPointBreak],
                                       &g_uvCRCounterNumTex[1][m_iPointBreak]);
        m_pSeqCtrlBreak->restart(0, 0.0f);
    } else {
        pEntity->setEffect(D_00477328, 5, &m_vBreakPos, NULL, 0.0f);
        pEntity->getHeadPos(m_vBreakPos);
        m_hSpeed = inHandle;
        m_pSeqCtrlSpeed->enableDisp(TRUE);
        INT point = m_iPointBreak + m_iPointBonus;
        m_pSeqCtrlSpeed->changeTexture(1, 0, NULL, &g_uvCRCounterNumTex[0][point / 10],
                                       &g_uvCRCounterNumTex[1][point / 10]);
        m_pSeqCtrlSpeed->changeTexture(2, 0, NULL, &g_uvCRCounterNumTex[0][point % 10],
                                       &g_uvCRCounterNumTex[1][point % 10]);
        m_pSeqCtrlSpeed->restart(0, 0.0f);
        m_iPointBonus = 0;
    }
}
#else
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Game/CardBattle", initCardBreak__Q210CardBattle11CCardBattleiUi);
#endif
__asm__(".rdata\n.align 3\n.word 0x4B2F3A63\n.word 0x4D4F4348\n.word 0x69614D2F\n.word 0x7070416E\n.word 0x6D61472F\n.word 0x61432F65\n.word 0x61426472\n.word 0x656C7474\n.word 0x7070632E\n.word 0x39353228\n.word 0x00002938\n.word 0x00000000\n.word 0x4B2F3A63\n.word 0x4D4F4348\n.word 0x69614D2F\n.word 0x7070416E\n.word 0x6D61472F\n.word 0x61432F65\n.word 0x61426472\n.word 0x656C7474\n.word 0x7070632E\n.word 0x31363228\n.word 0x00002936\n.word 0x00000000\n.word 0x61657262\n.word 0x0000006B\n.word 0x65657073\n.word 0x72625F64\n.word 0x006B6165\n.word 0x00000000\n.text\n");
void CCardBattle::dispCardBreak() {
    if (m_hBreak > 0) {
        if (m_pSeqCtrlBreak->isEnd(0)) {
            m_pSeqCtrlBreak->enableDisp(FALSE);
            m_hBreak = 0;
        } else {
            m_pSeqCtrlBreak->enableDisp(TRUE);
            m_pSeqCtrlBreak->set3DPos(m_vBreakPos);
        }
    }
    if (m_hSpeed > 0) {
        if (m_pSeqCtrlSpeed->isEnd(0)) {
            m_pSeqCtrlSpeed->enableDisp(FALSE);
            m_hBreak = 0;
        } else {
            m_pSeqCtrlSpeed->enableDisp(TRUE);
            m_pSeqCtrlSpeed->set3DPos(m_vBreakPos);
        }
    }
}
INT CCardBattle::calcPointBreak(INT a, INT b) {
    INT d = abs(a - b);
    return d < 10 ? d : 9;
}
INT CCardBattle::setEnemyCard(HANDLE inEnemy, INT inKind, INT inNumber) {
    if (m_uiFlag & 0xC0000000) return 0;
    if (m_uiFlag & 0xC0000004) return 0;
    if (m_cardGimmick.m_nKind) return 0;
    if (m_uiDuelState >= 2) return 0;
    CARD card;
    _setCard(card, inKind, inNumber);
    INT result = setPlayCard(inEnemy, 2, NULL, &card, 0);
    if (result > 0) initPlayingEnemy(inEnemy);
    return result;
}
void CCardBattle::destroyEnemyCard(INT inHandle) {
    if (inHandle == m_hOwner[1]) destroyPlayCard(2, inHandle);
}
void CCardBattle::destroyRevolver(INT inSide) {
    if (!(inSide)) AssertApp(D_00476980, 2764, "inUser", 0);
    UINT _PLAYER = inSide - 1;
    if (m_pRevolver[_PLAYER]) {
        m_pRevolver[_PLAYER]->destroy();
        m_pRevolver[_PLAYER] = NULL;
    }
}
void CCardBattle::setRevolver(INT inSide, CCardRevolver* pRevolver) {
    if (!(inSide)) AssertApp(D_00476980, 2774, "inUser", 0);
    m_pRevolver[inSide - 1] = pRevolver;
}
CCardRevolver* CCardBattle::getRevolver(INT inSide) {
    if (!(inSide)) AssertApp(D_00476980, 2779, "inUser", 0);
    return m_pRevolver[inSide - 1];
}
void CCardBattle::setBattleEnd() {
    if (m_pInstance) {
        _ref(m_pInstance->m_uiFlag) |= 0xC0000004;
        m_pInstance->initCardBreak(0, 0);
        m_pInstance->destroyPlayCard(1, 0);
        m_pInstance->resetEnchantCard(1);
        m_pInstance->destroyRevolver(1);
        m_pInstance->destroyPlayCard(2, 0);
        m_pInstance->resetEnchantCard(2);
        m_pInstance->destroyRevolver(2);
        m_pInstance->destroyStockFolder(1);
    }
}
BOOL CCardBattle::isBattleEnd() {
    if (!m_pInstance) return TRUE;
    return (m_pInstance->m_uiFlag & 0xc0000004) != 0;
}
void CCardBattle::setSuspend(BOOL b) {
    if (m_pInstance) {
        if (b) m_pInstance->m_uiFlag |= 0x10000;
        else m_pInstance->m_uiFlag &= ~0x10000;
    }
}
BOOL CCardBattle::isSuspend() {
    if (!m_pInstance) return FALSE;
    UINT uiMask = 0x10000;
    return (m_pInstance->m_uiFlag & uiMask) != 0;
}
BOOL CCardBattle::isLevitate() {
    if (!m_pInstance) return FALSE;
    UINT uiMask = 0x4000;
    return (m_pInstance->m_uiFlag & uiMask) != 0;
}
void CCardBattle::setBlack(BOOL b) {
    if (m_pInstance) {
        if (b) m_pInstance->m_uiFlag |= 0x40000;
        else m_pInstance->m_uiFlag &= ~0x40000;
    }
}
BOOL CCardBattle::isBlack() {
    if (!m_pInstance) return FALSE;
    UINT uiMask = 0x40000;
    return (m_pInstance->m_uiFlag & uiMask) != 0;
}
CStockFolder* CCardBattle::setStockFolder(::CTask* pParent, INT inUser) {
    if (!m_pInstance) return NULL;
    UINT _PLAYER = inUser - 1;
    if (!m_pInstance->m_pStockFolder[_PLAYER]) {
        m_pInstance->m_pStockFolder[_PLAYER] =
            new (com_malloc_d(sizeof(CStockFolder), __FILE__ "(2898)", RET_ADDR())) CStockFolder(pParent, inUser);
        _ref(m_pInstance)->m_hDuelEnemy = pParent->getHandle();
    }
    return m_pInstance->m_pStockFolder[_PLAYER];
}
void CCardBattle::destroyStockFolder(INT inSide) {
    if (!(inSide)) AssertApp(D_00476980, 2905, "inUser", 0);
    UINT _PLAYER = inSide - 1;
    if (m_pStockFolder[_PLAYER]) {
        m_pStockFolder[_PLAYER]->destroy();
        m_pStockFolder[_PLAYER] = NULL;
    }
}
CStockFolder* CCardBattle::getStockFolder(INT inSide) {
    if (!m_pInstance) return NULL;
    return m_pInstance->m_pStockFolder[inSide - 1];
}
INT CCardBattle::getStockFolderStockSkill(INT inSide) {
    if (!m_pInstance) return 0;
    CStockFolder* p = m_pInstance->m_pStockFolder[inSide - 1];
    if (!p) return 0;
    return *(SHORT*)((BYTE*)p + 0x84);
}
INT CCardBattle::calcStockFolderEnemyStockSkill() {
    if (!m_pInstance) return 0;
    CStockFolder* p = m_pInstance->m_pStockFolder[1];
    if (!p) return 0;
    return p->calcEnemyStockSkill();
}
void CCardBattle::setStockFolderStockInfo(INT inSide) {
    if (m_pInstance) {
        CStockFolder* p = m_pInstance->m_pStockFolder[inSide - 1];
        if (p) p->setStockInfo();
    }
}
void ENCHANT_PARAM::init(INT inOwner, UINT inUser) {
    uiUser = inUser;
    hOwner = inOwner;
    AtkKind = (ATK_KIND)0;
    AtkAttr = (ATK_ATTR)0;
    uiState = 0;
    UpKind = 0;
    iAttackPoint = 0;
    iHp = 0;
    iHpMax = 0;
    iFinish = 0;
    iGuard = 0;
    iNormal = 0;
}
void CCardBattle::calcEnchantParam(ENCHANT_PARAM& ioParam) {
    if (!(ioParam.uiUser)) AssertApp(D_00476980, 3103, "ioParam.uiUser", 0);
    UINT _PLAYER = ioParam.uiUser - 1;
    CARD& enchant = m_pInstance->m_cardEnchant[_PLAYER];
    if (enchant.m_nKind < 0x44) return;
    switch (ioParam.uiState) {
    case 1: {
        INT iCount = m_iCount[_PLAYER];
        CARD* pCard = m_cardData[_PLAYER];
        if (iCount == 1 && (pCard->m_nId & 0xF0000000) == 0x10000000) ioParam.iNormal = 1;
        if (ioParam.iNormal) {
            switch (enchant.m_nKind) {
            case 0x68:
                if (ioParam.iHp < ioParam.iHpMax / 4) ioParam.iAttackPoint *= 2;
                break;
            case 0x73:
                if (ioParam.iFinish > 0) {
                    if (RND::genrand_int31() % 100 < 0x43) ioParam.AtkKind = (ATK_KIND)0x1F;
                }
                break;
            case 0x51:
                ioParam.AtkKind = (ATK_KIND)0x15;
                break;
            case 0x75:
                if (RND::genrand_int31() % 100 < 25) ioParam.AtkKind = (ATK_KIND)10;
                break;
            case 0x6D:
                ioParam.iAttackPoint = ioParam.iAttackPoint * 150 / 100;
                break;
            }
        } else {
            switch (enchant.m_nKind) {
            case 0x47:
                if (ioParam.UpKind & 0x8) ioParam.iAttackPoint = ioParam.iAttackPoint * 150 / 100;
                break;
            case 0x48:
                if (ioParam.UpKind & 0x10) ioParam.iAttackPoint = ioParam.iAttackPoint * 150 / 100;
                break;
            case 0x49:
                if (ioParam.UpKind & 0x20) ioParam.iAttackPoint = ioParam.iAttackPoint * 150 / 100;
                break;
            case 0x4A:
                if (ioParam.UpKind & 0x40) ioParam.iAttackPoint = ioParam.iAttackPoint * 150 / 100;
                break;
            case 0x5C:
                if (ioParam.UpKind & 0x4) ioParam.iAttackPoint = ioParam.iAttackPoint * 130 / 100;
                break;
            case 0x62:
                if (ioParam.UpKind & 0x80) ioParam.iAttackPoint = ioParam.iAttackPoint * 130 / 100;
                break;
            case 0x77:
                if (ioParam.UpKind & 0x118) ioParam.iAttackPoint = ioParam.iAttackPoint * 150 / 100;
                break;
            case 0x7A:
                if (ioParam.UpKind & 0x210) ioParam.iAttackPoint *= 2;
                break;
            }
        }
        break;
    }
    case 2:
        switch (enchant.m_nKind) {
        case 0x5A:
            if (!ioParam.AtkAttr) {
                ioParam.iGuard = 1;
                ioParam.iAttackPoint /= 2;
            }
            break;
        case 0x6B:
            if (ioParam.AtkAttr < 5) {
                if (ioParam.AtkAttr >= 2) ioParam.iAttackPoint /= 2;
            }
            break;
        }
        break;
    }
}
void CCardBattle::calcEnchant_AttackPoint(INT& inPoint, ATK_KIND& inAtkKind, ATK_ATTR inAttr, UINT inUpKind) {
    if (m_pInstance && g_pPlayer) {
        ENCHANT_PARAM param;
        param.init(g_pPlayer->getHandle(), 1);
        param.uiState = 1;
        param.AtkKind = inAtkKind;
        param.UpKind = inUpKind;
        param.AtkAttr = inAttr;
        param.iAttackPoint = inPoint;
        param.iHp = g_pPlayer->getHP();
        param.iHpMax = g_pPlayer->getMaxHP();
        m_pInstance->calcEnchantParam(param);
        inPoint = param.iAttackPoint;
        inAtkKind = param.AtkKind;
    }
}
void CCardBattle::setPlayerEnchant(INT inKind) {
    if (!g_pPlayer) return;
    switch (inKind) {
    case 0x68:
        g_pPlayer->setDefAttr(PL::g_fDefAttr[2]);
        break;
    case 0x6E:
        g_pPlayer->setDefAttr(PL::g_fDefAttr[3]);
        break;
    case 0x72:
        g_pPlayer->setDefAttr(PL::g_fDefAttr[7]);
        break;
    case 0x70:
        g_pPlayer->setDefAttr(PL::g_fDefAttr[5]);
        break;
    case 0x74:
        g_pPlayer->setDefAttr(PL::g_fDefAttr[8]);
        break;
    case 0x6F:
        g_pPlayer->setDefAttr(PL::g_fDefAttr[4]);
        break;
    case 0x71:
        g_pPlayer->setDefAttr(PL::g_fDefAttr[6]);
        break;
    case 0x6C:
        g_pPlayer->setDefAttr(PL::g_fDefAttr[1]);
        break;
    case 0x73:
        g_pPlayer->setDefAttr(PL::g_fDefAttr[9]);
        break;
    case 0x45:
        g_pPlayer->m_iMaxCombo = 4;
        break;
    case 0x4E:
        g_pPlayer->m_iMaxCombo = 1;
        break;
    case 0x4F:
        g_pPlayer->setConditionFlag(0x100000, 0);
        break;
    case 0x63:
        g_pPlayer->setConditionFlag(0x80, 0);
        break;
    case 0x67:
        g_pPlayer->setConditionFlag(0x100, 0);
        break;
    case 0x6D:
        if (CGame::isSoraGame()) g_pPlayer->getRevolver()->setChargeRate(0x43);
        break;
    case 0x4C:
        { CPlayer* pPL = g_pPlayer; pPL->m_fPrizeDraw = 30.0f; pPL->m_fCardDraw = 10.0f; }
        break;
    case 0x56:
        g_pPlayer->setBanish(TRUE);
        break;
    case 0x46:
        g_pPlayer->setReflectGuard(TRUE);
        break;
    case 0x75:
        g_pPlayer->setDefAttr(PL::g_fDefAttr[10]);
        break;
    case 0x76:
        g_pPlayer->setDefAttr(PL::g_fDefAttr[11]);
        g_pPlayer->setConditionFlag(0x1000, 0);
        break;
    case 0x77:
        g_pPlayer->setDefAttr(PL::g_fDefAttr[12]);
        break;
    case 0x78:
        g_pPlayer->setDefAttr(PL::g_fDefAttr[13]);
        g_pPlayer->setAeroGuard(TRUE);
        break;
    case 0x79:
        g_pPlayer->setDefAttr(PL::g_fDefAttr[14]);
        g_pPlayer->setConditionFlag(0x40000000, 0);
        break;
    case 0x7A:
        g_pPlayer->setDefAttr(PL::g_fDefAttr[15]);
        break;
    case 0x7B:
        g_pPlayer->setDefAttr(PL::g_fDefAttr[16]);
        break;
    case 0x7C:
        g_pPlayer->setDefAttr(PL::g_fDefAttr[17]);
        g_pPlayer->setConditionFlag(0x20000000, 0);
        break;
    }
}
void CCardBattle::resetPlayerEnchant(INT inKind) {
    if (_ref(g_pPlayer)) {
        _ref(g_pPlayer)->setDefAttr(PL::g_fDefAttr[0]);
        _ref(g_pPlayer)->m_iMaxCombo = 3;
        { CPlayer* pPL = _ref(g_pPlayer); pPL->m_fPrizeDraw = 5.0f; pPL->m_fCardDraw = 5.0f; }
        _ref(g_pPlayer)->setBanish(FALSE);
        _ref(g_pPlayer)->setConditionFlag(0, 0x100000);
        _ref(g_pPlayer)->setConditionFlag(0, 0x80);
        _ref(g_pPlayer)->setConditionFlag(0, 0x100);
        _ref(g_pPlayer)->setConditionFlag(0, 0x20000000);
        _ref(g_pPlayer)->setConditionFlag(0, 0x40000000);
        _ref(g_pPlayer)->setConditionFlag(0, 0x1000);
        _ref(g_pPlayer)->setReflectGuard(FALSE);
        _ref(g_pPlayer)->setAeroGuard(FALSE);
        CCardRevolver* pRev = _ref(g_pPlayer)->getRevolver();
        if (pRev) {
            pRev->setChargeRate(100);
            pRev->setPlayNG(0, 0);
        }
    }
}
void CCardBattle::transferPlayerEnchant(CPlayer* inPL) {
    if (inPL && m_cardEnchant[0].m_nKind >= 0x44) {
        setPlayerEnchant(m_cardEnchant[0].m_nKind);
        inPL->setDefAttr(PL::g_fDefAttr[0]);
        inPL->m_iMaxCombo = 3;
        inPL->m_fPrizeDraw = 5.0f;
        inPL->m_fCardDraw = 5.0f;
        inPL->setBanish(FALSE);
        inPL->setConditionFlag(0, 0x100000);
        inPL->setConditionFlag(0, 0x80);
        inPL->setConditionFlag(0, 0x100);
        inPL->setConditionFlag(0, 0x20000000);
        inPL->setConditionFlag(0, 0x40000000);
        inPL->setConditionFlag(0, 0x1000);
        inPL->setReflectGuard(FALSE);
        inPL->setAeroGuard(FALSE);
    }
}
void CCardBattle::exeRegeneration() {
    UINT flag = 0x400;
    if (m_uiFlag & flag) {
        m_tqEnchantTimer[0] += getFrameRate();
        if (m_tqEnchantTimer[0] >= 6000) {
            if (g_pPlayer) {
                INT iHP = g_pPlayer->getHP();
                INT iMax = g_pPlayer->getMaxHP();
                if (!g_pPlayer->isConditionFlag(2) && iHP < iMax) {
                    INT iHeal = (iMax - iHP) / 8;
                    g_pPlayer->addHP(iHeal, 3000, 1);
                    decEnchantTime(1, 40, 39);
                    m_tqEnchantTimer[0] = 0;
                }
            }
        }
    }
    flag = 0x800;
    if (m_uiFlag & flag) {
        m_tqEnchantTimer[1] += getFrameRate();
        if (m_tqEnchantTimer[1] >= 6000) {
            decEnchantTime(2, 40, 39);
            m_tqEnchantTimer[1] = 0;
        }
    }
}
void CCardBattle::exeBio() {
    UINT flag = 0x1000;
    if (m_uiFlag & flag) {
        m_tqEnchantTimer[0] += getFrameRate();
        if (m_tqEnchantTimer[0] >= 1000) m_tqEnchantTimer[0] = 0;
    }
    flag = 0x2000;
    if (m_uiFlag & flag) {
        m_tqEnchantTimer[1] += getFrameRate();
        if (m_tqEnchantTimer[1] >= 1000) {
            CBattleEnemyManager* pBEM = (CBattleEnemyManager*)CTaskMan::getSysTask(0x19);
            if (pBEM) {
                for (SLContainerT<CTask>* container = pBEM->getFirstBattleEnemy(); container;
                     container = container->getNext()) {
                    CBattleEnemy* pEnemy = (CBattleEnemy*)container->getValue();
                    if (pEnemy && pEnemy->getTypeID() == 0x1004) pEnemy->onBio(1);
                }
            }
            m_tqEnchantTimer[1] = 0;
        }
    }
}
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Game/CardBattle", getDropFriendCard__10CardBattleR4CARDi);
__asm__(".rdata\n.align 4\n.word .L001DF53C\n.word .L001DF550\n.word .L001DF564\n.word .L001DF58C\n.word .L001DF578\n.word .L001DF5A0\n.word .L001DF5B4\n.word .L001DF5D0\n.text\n");
INT CardBattle::getDropPlutoCard(CARD& outData) {
    if (CGame::isRikuGame()) return 0;
    if (CSceneInfo::isMinigameScene()) return 0;
    if (CCardBattle::getPlutoCount() >= 3) return 0;
    CCardRevolver* pRev = g_pPlayer ? g_pPlayer->getRevolver() : NULL;
    if (!pRev) return 0;
    BOOL bCheck2 = TRUE;
    INT r = RND::genrand_int31() % 1000;
    BOOL bCheck = TRUE;
    if (DebugWork::g_dw.player.iFriend > 0) {
        r = 0;
        bCheck = FALSE;
    }
    if (DebugWork::g_dw.player.iPluto > 0) {
        r = 0;
        bCheck = FALSE;
        bCheck2 = FALSE;
    }
    if (r >= 200) return 0;
    if (bCheck && !(CGameWork::getInt(0, 0x22) & 0x100)) return 0;
    if (bCheck2 && pRev->getHave(0, 1, 1, 0, 0) >= 10) return 0;
    _setCard(outData, 0x39, RND::genrand_int31() % 10);
    return 1;
}
void CCardBattle::exeDuel() {
    switch (m_uiDuelState) {
    case 0:
        break;
    case 1:
        if (m_uiFlag & 0x400000) {
            m_uiFlag |= 0x200000;
        } else {
            if (!g_pPlayer) return;
            CCardRevolver* pRV = m_pRevolver[0];
            if (!pRV) break;
            if (pRV->getHave(0, 1, 0, 0, 1) < 8) {
                m_uiFlag |= 0x200000;
            } else {
                m_uiFlag &= ~0x200000;
            }
        }
        break;
    case 2:
        if (g_pPlayer->m_PlayerState != PL::DUEL) {
            setDuelEntry(0);
            return;
        }
        break;
    case 3:
        break;
    case 4:
        if (m_pDuelGauge->getState() == 2) m_uiDuelState = 5;
        break;
    case 5:
        if (m_nDuelBreakCount == m_nDuelBreakCountMax) {
            m_uiDuelState = 7;
            m_pDuelGauge->setResult(1);
            if (m_pRevolver[0]) m_pRevolver[0]->calcStockSkill();
            g_pPlayer->m_uiBattleFlag |= 0x8000000;
        } else if (m_pDuelGauge->getTimer() == 0) {
            m_tqMainTimer = 0;
            m_uiDuelState = 6;
            _ref(g_pPlayer)->m_uiBattleFlag |= 0x8000000;
        }
        break;
    case 6:
        if (m_nDuelBreakCount == m_nDuelBreakCountMax) {
            m_uiDuelState = 7;
            m_pDuelGauge->setResult(1);
            if (m_pRevolver[0]) m_pRevolver[0]->calcStockSkill();
            g_pPlayer->m_uiBattleFlag |= 0x8000000;
        } else if (m_tqMainTimer >= 1000) {
            m_uiDuelState = 7;
            m_pDuelGauge->setResult(0);
            m_iDuelSkill = 0;
        }
        break;
    case 7:
        if (m_pDuelGauge->getState() == 0) {
            setDuelEnd();
            return;
        }
        break;
    case 8:
        if (g_pPlayer->m_PlayerState != PL::DUEL) {
            m_iDuelSkill = 0;
            m_uiDuelState = 1;
        }
        break;
    case 9:
    case 10:
        break;
    case 11:
        if (m_nDuelBreakCount == m_nDuelBreakCountMax) m_uiDuelState = 12;
        break;
    case 12:
        setPronounceEnd(m_nDuelBreakCount);
        break;
    }
}
void CCardBattle::setDuelEntry(INT inFlag) {
    if (inFlag > 0) {
        if (m_uiDuelState == 1) m_uiDuelState = 2;
        return;
    }
    if (inFlag == -1) {
        if (!isDuelEntryingCard(0)) return;
    } else {
        if (m_uiDuelState != 2) return;
    }
    m_uiDuelState = 1;
    for (INT owner = 0; owner < 2; owner++) {
        for (INT i = 0; i < m_iCount[owner]; i++) {
            if (m_pCard[owner][i]) m_pCard[owner][i]->setDuelRefrect();
        }
        m_uiState[owner] = 5;
    }
    m_pMesage->sendMessage(0, getHandle(), m_hDuelEnemy, 9, &m_mesCardInfo[1]);
}
void CCardBattle::setDuelStart(INT inFlag) {
    if (inFlag) {
        if (m_uiDuelState == 3) m_uiDuelState = 4;
        return;
    }
    if (m_uiDuelState != 2) return;
    m_uiDuelState = 3;
    for (INT owner = 0; owner < 2; owner++) {
        for (INT i = 0; i < m_iCount[owner]; i++) {
            if (m_pCard[owner][i]) m_pCard[owner][i]->setBreak(0);
        }
        m_uiState[owner] = 5;
    }
    m_pMesage->sendMessage(0, getHandle(), m_hDuelEnemy, 10, &m_mesCardInfo[1]);
    CGameWork::setFlag(1, 10, TRUE);
    CCardRevolver* pRV = m_pRevolver[0];
    if (pRV) {
        pRV->returnStockCard(1);
        pRV->changeMode(0, 1);
    }
    pRV = m_pRevolver[1];
    if (pRV) {
        pRV->returnStockCard(1);
        pRV->changeMode(0, 1);
    }
    CGameWork::setFlag(1, 11, TRUE);
    CGameWork::setInt(1, 0xE2, CGameWork::getInt(1, 0xE2) | 0x14);
    TIMEQ time = m_nDuelBreakCountMax * 3000 + 3000;
    if (DebugWork::g_dw.player.iDuelTime > 0) time = DebugWork::g_dw.player.iDuelTime * 3000;
    m_pDuelGauge->setStart(time, m_nDuelBreakCountMax);
    PLM::CPlayerManager* pPLM = (PLM::CPlayerManager*)CTaskMan::getSysTask(0x1F);
    INT shiftID = (g_pPlayer->m_uiConditionFlag & 0x400000) ? 3 : 0;
    m_iDuelSkill = 0;
    switch (m_nDuelBreakCountMax) {
    case 3:
        m_iDuelSkill = shiftID + 0x66;
        break;
    case 5:
        m_iDuelSkill = shiftID + 0x67;
        break;
    case 7:
        m_iDuelSkill = shiftID + 0x68;
        break;
    }
    pPLM->setReqLoad(m_iDuelSkill, 0);
    switch (m_iDuelSkill) {
    case 0x66:
        pPLM->setReqLoad(0x93, 0);
        break;
    case 0x67:
        pPLM->setReqLoad(0x94, 0);
        break;
    case 0x68:
        pPLM->setReqLoad(0x95, 0);
        break;
    case 0x69:
        pPLM->setReqLoad(0x93, 0);
        break;
    case 0x6A:
        pPLM->setReqLoad(0x94, 0);
        pPLM->setReqLoad(0x92, 0);
        break;
    case 0x6B:
        pPLM->setReqLoad(0x95, 0);
        pPLM->setReqLoad(0x92, 0);
        break;
    }
    CGameWork::setFlag(1, 0x2B, TRUE);
    PLM::addIntBR(0x43);
}
void CCardBattle::setDuelBreakCount(INT inCount, INT inMax) {
    INT max = inMax;
    if (m_uiDuelState < 9) {
        switch (inMax) {
        case 0:
        case 1:
        case 2:
        case 3:
            max = 3;
            break;
        case 4:
        case 5:
            max = 5;
            break;
        case 6:
        case 7:
        default:
            max = 7;
            break;
        }
    }
    m_nDuelBreakCount = inCount;
    m_nDuelBreakCountMax = max;
}
BOOL CCardBattle::setEnemyDuelCard(HANDLE inHandle, INT inIndex, INT inNumber) {
    if (m_uiDuelState != 5) return 0;
    if (m_uiState[0]) return 0;
    if (m_uiState[1]) return 0;
    CARD card;
    _setCard(card, inIndex, inNumber);
    INT result = setPlayCard(inHandle, 2, NULL, &card, 0);
    if (result > 0) initPlayingEnemy(inHandle);
    return result;
}
void CCardBattle::setDuelEnd() {
    if (!m_iDuelSkill) {
        m_uiDuelState = 1;
        CGameWork::setByte(1, 3, 2);
        m_pMesage->sendMessage(0, getHandle(), g_pPlayer->getHandle(), 11, &m_mesCardInfo[0]);
        m_pMesage->sendMessage(0, getHandle(), m_hDuelEnemy, 11, &m_mesCardInfo[1]);
        if (!CGameWork::getFlag(1, 7)) CGameWork::setFlag(1, 10, FALSE);
        if (m_pRevolver[0]) m_pRevolver[0]->returnStockCard(1);
        CGameWork::setFlag(1, 11, FALSE);
        CGameWork::setInt(1, 0xE2, CGameWork::getInt(1, 0xE2) ^ 0x14);
    } else {
        m_uiDuelState = 8;
        CGameWork::setByte(1, 3, 1);
        _ref(m_mesCardInfo[0].m_nStock) = m_iDuelSkill;
        m_pMesage->sendMessage(0, getHandle(), g_pPlayer->getHandle(), 12, &m_mesCardInfo[0]);
        m_pMesage->sendMessage(0, getHandle(), m_hDuelEnemy, 12, &m_mesCardInfo[1]);
        if (!CGameWork::getFlag(1, 7)) CGameWork::setFlag(1, 10, FALSE);
        CGameWork::setFlag(1, 11, FALSE);
        CGameWork::setInt(1, 0xE2, CGameWork::getInt(1, 0xE2) ^ 0x14);
        PLM::addIntBR(0x42);
    }
    PLM::calcDuelWin();
}
INT CCardBattle::setDuelPlayStock(CBattleCard** inCard, INT inSkill) {
    m_uiState[0] = 12;
    m_uiResult[0] = 0;
    for (INT i = 0; i < m_nDuelBreakCountMax; inCard++, i++) {
        CBattleCard* pCard = *inCard;
        m_pCard[0][i] = pCard;
        if (pCard) {
            m_cardData[0][i] = *pCard->getCardData();
            pCard->setPlayDuelStock(m_nDuelBreakCountMax);
        } else {
            _setCard(m_cardData[0][i], 0, 0);
        }
    }
    m_iStock[0] = inSkill;
    m_iExtra[0] = 0;
    m_iFirst[0] = 0;
    return 1;
}
BOOL CCardBattle::isDuelEntryingCard(INT inSide) {
    if (!m_pInstance) return FALSE;
    if ((UINT)inSide < 2) {
        UINT s = m_pInstance->m_uiState[0];
        if (s < 9) {
            if (s >= 6) return TRUE;
        }
    }
    if (inSide == 0 || inSide == 2) {
        UINT s = m_pInstance->m_uiState[1];
        if (s < 9) {
            if (s >= 6) return TRUE;
        }
    }
    return FALSE;
}
namespace CardBattle {
static LPCSTR g_szLadDuel = "duelmode.lad";
static LPCSTR g_szSeqDuel = "duelmode:00";
INT g_DuelSeqIDX_TimerNumDmy[6][3] = {
    {47, 48, 49}, {50, 51, 52}, {53, 54, 55}, {56, 57, 58}, {59, 60, 61}, {62, 63, 64},
};
G_UV g_uvDuelNumTex[4][10] = {
    {{153, 144}, {178, 144}, {193, 144}, {214, 144}, {236, 144}, {154, 163}, {174, 163}, {193, 163}, {214, 163}, {235, 163}},
    {{174, 163}, {191, 163}, {214, 163}, {235, 163}, {255, 163}, {174, 182}, {193, 182}, {212, 182}, {235, 182}, {254, 182}},
    {{2, 225}, {2, 194}, {24, 194}, {58, 194}, {94, 194}, {125, 194}, {36, 225}, {69, 225}, {102, 225}, {136, 225}},
    {{34, 256}, {23, 224}, {57, 225}, {91, 225}, {123, 224}, {158, 225}, {68, 256}, {100, 256}, {135, 256}, {167, 256}},
};
}
CDuelGauge::CDuelGauge(::CTask* pParent) : ::CTask(pParent, 0x4000) {
    m_pSeqCtrls = NULL;
    _initialize();
}
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Game/CardBattle", _$_Q210CardBattle10CDuelGauge);
void CDuelGauge::_initialize() {
    m_uiFlag = 0;
    m_uiState = 0;
    m_uiTimerState = 0;
    m_uiCountState = 0;
    m_uiCount = 0;
    m_uiCountMax = 0;
    m_uiResult = 0;
    m_tqTimer = 0;
    m_tqLimit = 0;
    m_hTimerSE = 0;
    m_pSeqCtrls = new (__FILE__ "(4378)") D2::C2DSeqCtrl[12];
}
void CDuelGauge::main() {
    switch (m_uiState) {
    case 0:
        break;
    case 1:
        if (m_pSeqCtrls->isEnd(0)) {
            m_uiState = 2;
            m_pSeqCtrls->destroy();
        }
        break;
    case 2:
        break;
    case 3:
        if (m_pSeqCtrls->isEnd(0)) {
            m_uiState = 4;
            m_pSeqCtrls->destroy();
        }
        break;
    case 4:
        m_uiState = 0;
        break;
    }
    exeTimer();
    exeCount();
    exeDuelNG();
}
/* Parked: one store slot - the original schedules the m_tqTimer store (0x5c) after the zero stores of m_uiCount and
 * m_uiResult, ours before them (and the setLayer sb one slot earlier); the statement order is the $LM line order;
 * tried: all 720 orders of the six stores (200 tested), chained/temp assignments for timer/limit, pSeq load forms
 * (_ref, m_pSeqCtrls, &[0]), permuter 4 min (13841 candidates, no gain) */
#if 0
void CDuelGauge::setStart(TIMEQ inTime, UINT inCount) {
    m_uiState = 1;
    m_uiCount = 0;
    m_uiCountMax = inCount;
    m_uiResult = 0;
    m_tqTimer = inTime;
    m_tqLimit = inTime;
    D2::C2DSeqCtrl* pSeq = &m_pSeqCtrls[0];
    pSeq->setLayer(8);
    pSeq->addSeq(g_szSeqDuel, 30);
    FVECTOR pos;
    D2::C2DMan::getLayPosition(g_szLadDuel, 1, 0, pos, FALSE);
    pSeq->setPos(pos);
    setTimer(1);
    setCount(1);
    CSound::playSE(0x14B, -1, 64);
    m_hTimerSE = CSound::playSE(0x14C, -1, 64);
}
#else
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Game/CardBattle", setStart__Q210CardBattle10CDuelGaugeiUi);
#endif
void CDuelGauge::setResult(UINT inResult) {
    m_uiResult = inResult;
    D2::C2DSeqCtrl* pSeq = &m_pSeqCtrls[0];
    m_uiState = 3;
    pSeq->setLayer(8);
    if (m_uiResult) {
        pSeq->addSeq(g_szSeqDuel, 31);
        CSound::playSE(0x14E, -1, 64);
    } else {
        pSeq->addSeq(g_szSeqDuel, 32);
        CSound::playSE(0x14F, -1, 64);
    }
    setTimer(3);
    setCount(3);
    if (m_hTimerSE > 0) {
        CSound::stopSE(m_hTimerSE, 0x14C);
        m_hTimerSE = 0;
    }
}
void CDuelGauge::setTimer(UINT inState) {
    D2::C2DSeqCtrl* pSeq;
    INT i;
    FVECTOR pos;
    switch (inState) {
    case 0:
        m_uiTimerState = inState;
        for (i = 1; i < 8; i++) {
            m_pSeqCtrls[i].destroy();
        }
        break;
    case 1:
        m_uiTimerState = 1;
        D2::C2DMan::getLayPosition(_ref(g_szLadDuel), 1, 0, pos, FALSE);
        pSeq = &m_pSeqCtrls[1];
        pSeq->destroy();
        pSeq->setLayer(2);
        pSeq->addSeq(g_szSeqDuel, 0x21);
        pSeq->setPos(pos);
        pSeq->addSeq(g_szSeqDuel, 0x24);
        pSeq->enableColorCtrl(TRUE);
        for (i = 0; i < 6; i++) {
            pSeq = &m_pSeqCtrls[i + 2];
            pSeq->destroy();
            pSeq->setLayer(2);
            pSeq->addSeq(g_szSeqDuel, g_DuelSeqIDX_TimerNumDmy[i][0]);
            pSeq->addSeq(g_szSeqDuel, 0x25);
            pSeq->enableColorCtrl(TRUE);
            pSeq->setPos(pos);
        }
        changeTimer(m_tqLimit);
        break;
    case 2:
        m_uiTimerState = inState;
        pSeq = &m_pSeqCtrls[1];
        pSeq->destroy();
        pSeq->setLayer(2);
        pSeq->addSeq(g_szSeqDuel, 0x22);
        pSeq->addSeq(g_szSeqDuel, 0x24);
        pSeq->enableColorCtrl(TRUE);
        for (i = 0; i < 6; i++) {
            pSeq = &m_pSeqCtrls[i + 2];
            pSeq->destroy();
            pSeq->setLayer(2);
            pSeq->addSeq(g_szSeqDuel, g_DuelSeqIDX_TimerNumDmy[i][1]);
            pSeq->addSeq(g_szSeqDuel, 0x25);
            pSeq->enableColorCtrl(TRUE);
        }
        changeTimer(m_tqTimer);
        break;
    case 3:
        m_uiTimerState = inState;
        pSeq = &m_pSeqCtrls[1];
        pSeq->destroy();
        pSeq->setLayer(2);
        pSeq->addSeq(g_szSeqDuel, 0x23);
        pSeq->addSeq(g_szSeqDuel, 0x24);
        pSeq->enableColorCtrl(TRUE);
        for (i = 0; i < 6; i++) {
            pSeq = &m_pSeqCtrls[i + 2];
            pSeq->destroy();
            pSeq->setLayer(2);
            pSeq->addSeq(g_szSeqDuel, g_DuelSeqIDX_TimerNumDmy[i][2]);
            pSeq->addSeq(g_szSeqDuel, 0x25);
            pSeq->enableColorCtrl(TRUE);
        }
        changeTimer(m_tqTimer);
        break;
    }
}
void CDuelGauge::changeTimer(TIMEQ inTime) {
    INT n = inTime * 100 / m_tqLimit;
    m_pSeqCtrls[1].changeTime(1, 100 - n);
    n = inTime % 3000 / 30 % 10;
    m_pSeqCtrls[2].changeTexture(1, 0, NULL, &g_uvDuelNumTex[0][n], &g_uvDuelNumTex[1][n]);
    n = inTime % 3000 / 300 % 10;
    m_pSeqCtrls[3].changeTexture(1, 0, NULL, &g_uvDuelNumTex[0][n], &g_uvDuelNumTex[1][n]);
    n = inTime / 3000 % 10;
    m_pSeqCtrls[4].changeTexture(1, 0, NULL, &g_uvDuelNumTex[0][n], &g_uvDuelNumTex[1][n]);
    n = inTime / 30000 % 10;
    m_pSeqCtrls[5].changeTexture(1, 0, NULL, &g_uvDuelNumTex[0][n], &g_uvDuelNumTex[1][n]);
}
void CDuelGauge::exeTimer() {
    D2::C2DSeqCtrl* pSeq = &m_pSeqCtrls[1];
    switch (m_uiTimerState) {
    case 1:
        if (pSeq->isEnd(0)) setTimer(2);
        break;
    case 0:
        break;
    case 2:
        if (m_tqTimer > 0) {
            m_tqTimer -= getFrameRate();
            if (m_tqTimer < 0) m_tqTimer = 0;
            changeTimer(m_tqTimer);
        }
        break;
    case 3:
        if (pSeq->isEnd(0)) setTimer(0);
        break;
    }
}
void CDuelGauge::setCount(UINT inState) {
    D2::C2DSeqCtrl* pSeq;
    FVECTOR pos;
    switch (inState) {
    case 0:
        m_uiCountState = inState;
        m_pSeqCtrls[8].destroy();
        m_pSeqCtrls[9].destroy();
        m_pSeqCtrls[10].destroy();
        break;
    case 1:
        m_uiCountState = inState;
        D2::C2DMan::getLayPosition(_ref(g_szLadDuel), 1, 0, pos, FALSE);
        pSeq = &m_pSeqCtrls[8];
        pSeq->destroy();
        pSeq->setLayer(2);
        pSeq->addSeq(g_szSeqDuel, 0);
        pSeq->setPos(pos);
        pSeq = &m_pSeqCtrls[9];
        pSeq->destroy();
        pSeq->setLayer(2);
        pSeq->addSeq(g_szSeqDuel, 6);
        pSeq->addSeq(g_szSeqDuel, m_uiCountMax + 20);
        pSeq->setPos(pos);
        pSeq->enableColorCtrl(TRUE);
        pSeq = &m_pSeqCtrls[10];
        pSeq->destroy();
        pSeq->setLayer(2);
        pSeq->addSeq(g_szSeqDuel, 3);
        pSeq->addSeq(g_szSeqDuel, m_uiCount + 9);
        pSeq->setPos(pos);
        pSeq->enableColorCtrl(TRUE);
        break;
    case 2:
        m_uiCountState = inState;
        pSeq = &m_pSeqCtrls[8];
        pSeq->destroy();
        pSeq->setLayer(2);
        pSeq->addSeq(g_szSeqDuel, 1);
        pSeq = &m_pSeqCtrls[9];
        pSeq->destroy();
        pSeq->setLayer(2);
        pSeq->addSeq(g_szSeqDuel, 7);
        pSeq->addSeq(g_szSeqDuel, m_uiCountMax + 20);
        pSeq->enableColorCtrl(TRUE);
        pSeq = &m_pSeqCtrls[10];
        pSeq->destroy();
        pSeq->setLayer(2);
        pSeq->addSeq(g_szSeqDuel, 4);
        if (m_uiCount) {
            pSeq->addSeq(g_szSeqDuel, m_uiCount + 9);
        } else {
            pSeq->addSeq(g_szSeqDuel, 9);
        }
        pSeq->enableColorCtrl(TRUE);
        break;
    case 3:
        m_uiCountState = inState;
        pSeq = &m_pSeqCtrls[8];
        pSeq->destroy();
        pSeq->setLayer(2);
        pSeq->addSeq(g_szSeqDuel, 2);
        pSeq = &m_pSeqCtrls[9];
        pSeq->destroy();
        pSeq->setLayer(2);
        pSeq->addSeq(g_szSeqDuel, 8);
        pSeq->addSeq(g_szSeqDuel, m_uiCountMax + 20);
        pSeq->enableColorCtrl(TRUE);
        pSeq = &m_pSeqCtrls[10];
        pSeq->destroy();
        pSeq->setLayer(2);
        pSeq->addSeq(g_szSeqDuel, 5);
        if (m_uiResult) {
            pSeq->addSeq(g_szSeqDuel, m_uiCount + 9);
        } else {
            pSeq->addSeq(g_szSeqDuel, 9);
            m_pSeqCtrls[10].changeTexture(1, 0, NULL, &g_uvDuelNumTex[2][m_uiCount], &g_uvDuelNumTex[3][m_uiCount]);
        }
        pSeq->enableColorCtrl(TRUE);
        break;
    }
}
void CDuelGauge::exeCount() {
    D2::C2DSeqCtrl* pSeq = &m_pSeqCtrls[8];
    switch (m_uiCountState) {
    case 1:
        if (pSeq->isEnd(0)) setCount(2);
        break;
    case 0:
    case 2:
        break;
    case 3:
        if (pSeq->isEnd(0)) setCount(0);
        break;
    }
}
void CDuelGauge::changeCount(UINT inCount, UINT inMax) {
    m_uiCount = inCount % 10;
    if (m_uiCount) {
        m_pSeqCtrls[10].replaceSeq(1, g_szSeqDuel, m_uiCount + 9);
    } else {
        m_pSeqCtrls[10].replaceSeq(1, g_szSeqDuel, 9);
    }
    if (inMax) {
        m_uiCountMax = inMax % 10;
        m_pSeqCtrls[9].changeTexture(1, 0, NULL, &g_uvDuelNumTex[2][m_uiCountMax], &g_uvDuelNumTex[3][m_uiCountMax]);
    }
}
void CDuelGauge::setDuelNG() {
    D2::C2DSeqCtrl* pSeq = &m_pSeqCtrls[11];
    if (pSeq) {
        m_uiFlag |= 1;
        pSeq->destroy();
        pSeq->setLayer(8);
        pSeq->addSeq(g_szSeqCockpit, 0x97);
        CB_LAYOUT* pLayout = &g_LayoutData[1];
        FVECTOR pos = {{pLayout->x, pLayout->y, 0.0f, 1.0f}};
        pSeq->setPos(pos);
    }
}
void CDuelGauge::exeDuelNG() {
    UINT flag = 1;
    if (m_uiFlag & flag) {
        D2::C2DSeqCtrl* pSeq = &m_pSeqCtrls[11];
        if (pSeq && pSeq->isEnd(0)) {
            pSeq->destroy();
            m_uiFlag &= ~flag;
        }
    }
}
INT CCardBattle::setPronounceStart(INT inFlag) {
    if (inFlag) {
        if (m_uiDuelState != 9) return 0;
        m_uiDuelState = 10;
        m_pMesage->sendMessage(0, getHandle(), m_hDuelEnemy, 13, &m_mesCardInfo[1]);
        return 1;
    }
    m_uiFlag |= 0xC0000000;
    m_uiDuelState = 9;
    for (INT i = 0; i < m_iCount[0]; i++) {
        if (m_pCard[0][i]) m_pCard[0][i]->setBreak(0);
    }
    m_uiState[0] = 5;
    CGameWork::setFlag(1, 10, TRUE);
    CCardRevolver* pRV = m_pRevolver[0];
    if (pRV) {
        pRV->returnStockCard(1);
        pRV->changeMode(0, 1);
    }
    CGameWork::setFlag(1, 11, TRUE);
    return 1;
}
INT CCardBattle::setPronouncePlay(INT inFlag) {
    CStockFolder* pSF = m_pStockFolder[1];
    if (!pSF) return 0;
    if (inFlag) {
        while (pSF->getStock(0) < 6) {
            pSF->setPronounceCard(0x98, RND::genrand_int31() % 8 + 1);
        }
    }
    m_uiDuelState = 11;
    setDuelBreakCount(0, 6);
    return pSF->setPlayPronounce();
}
INT CCardBattle::setEnemyPronounceCard(HANDLE inOwner, CBattleCard* inCard) {
    if (m_uiState[1]) return 0;
    m_hOwner[1] = inOwner;
    m_uiState[1] = 14;
    m_uiResult[1] = 0;
    CBattleCard* pOld = m_pCard[1][0];
    if (pOld && !pOld->getRevolver()) pOld->out();
    m_pCard[1][0] = inCard;
    if (inCard) {
        inCard->setEnchantEffect(0x44, 0);
        m_pCard[1][0]->setEnchantEffect(0x52, 0);
        m_pCard[1][0]->setEnchantEffect(0x53, 0);
    }
    m_cardData[1][0] = *inCard->getCardData();
    m_iStock[1] = 0;
    m_iExtra[1] = 0;
    m_iFirst[1] = 0;
    m_pCard[1][0]->setPlayPronounce();
    return 1;
}
void CCardBattle::setPronounceEnd(INT inCount) {
    if (m_uiDuelState < 10) return;
    m_uiDuelState = 0;
    disablePlayCard(0);
    if (inCount > 0) {
        CGameWork::setFlag(1, 10, FALSE);
        CGameWork::setFlag(1, 11, FALSE);
        m_pMesage->sendMessage(0, getHandle(), g_pPlayer->getHandle(), 15, &m_mesCardInfo[0]);
        m_pMesage->sendMessage(0, getHandle(), m_hDuelEnemy, 3, &m_mesCardInfo[1]);
    } else {
        resetEnchantCard(1);
        m_pMesage->sendMessage(0, getHandle(), g_pPlayer->getHandle(), 14, &m_mesCardInfo[0]);
        m_pMesage->sendMessage(0, getHandle(), m_hDuelEnemy, 14, &m_mesCardInfo[1]);
        if (g_pPlayer->getRevolver()) g_pPlayer->getRevolver()->rollDeck(0, 1, 300);
    }
}
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Game/CardBattle", disablePlayCard__Q210CardBattle11CCardBattleUi);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Game/CardBattle", isDisablePlayCard__Q210CardBattle11CCardBattle);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Game/CardBattle", isCardBattleEnd__Q210CardBattle11CCardBattle);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Game/CardBattle", disableDuelEntry__Q210CardBattle11CCardBattleUi);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Game/CardBattle", getDuelEnemy__CQ210CardBattle11CCardBattle);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Game/CardBattle", getDuelBreakCountMax__CQ210CardBattle11CCardBattle);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Game/CardBattle", isNowDuelEntry__CQ210CardBattle11CCardBattle);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Game/CardBattle", isNowDuel__CQ210CardBattle11CCardBattle);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Game/CardBattle", isNowSetEnemyDuelCard__CQ210CardBattle11CCardBattle);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Game/CardBattle", getDuelSkill__Q210CardBattle11CCardBattle);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Game/CardBattle", isPronounceMode__CQ210CardBattle11CCardBattle);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Game/CardBattle", isNowPronounce__CQ210CardBattle11CCardBattle);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Game/CardBattle", isNowSetEnemyPronounce__CQ210CardBattle11CCardBattle);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Game/CardBattle", setSubTimer__Q210CardBattle11CCardBattlei);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Game/CardBattle", getSubTimer__CQ210CardBattle11CCardBattle);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Game/CardBattle", getEnchantKind__CQ210CardBattle11CCardBattlei);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Game/CardBattle", getRoomCard__Q210CardBattle11CCardBattle);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Game/CardBattle", getEnchantCard__Q210CardBattle11CCardBattlei);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Game/CardBattle", getStockCount__Q210CardBattle11CCardBattlei);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Game/CardBattle", getState__CQ210CardBattle10CDuelGauge);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Game/CardBattle", getTimer__CQ210CardBattle10CDuelGauge);
/* The vtables (their key functions, the destructors, are still assembly). */
__asm__(".rdata\n.align 3\n.globl _vt$Q210CardBattle10CDuelGauge\n_vt$Q210CardBattle10CDuelGauge:\n.word 0\n.word 0\n.word 0\n.word _$_Q210CardBattle10CDuelGauge\n.word 0\n.word main__Q210CardBattle10CDuelGauge\n.word 0\n.word disp__5CTask\n.word 0\n.word after__5CTask\n.word 0\n.word onOut__5CTask\n.word 0\n.word getTypeID__C5CTask\n.word 0\n.word 0\n.globl _vt$Q210CardBattle11CCardBattle\n_vt$Q210CardBattle11CCardBattle:\n.word 0\n.word 0\n.word 0\n.word _$_Q210CardBattle11CCardBattle\n.word 0\n.word main__Q210CardBattle11CCardBattle\n.word 0\n.word disp__Q210CardBattle11CCardBattle\n.word 0\n.word after__5CTask\n.word 0\n.word onOut__5CTask\n.word 0\n.word getTypeID__C5CTask\n.word 0\n.word 0\n.text\n");
// SOURCE DATA: .data .rodata
