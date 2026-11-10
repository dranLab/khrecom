#define GAME_CARDREVOLVER_CPP
#include "khtypes.h"
#define GPRIM_H_PATH "System/Graphic/GPrim.h" /* MATCH: the original includes it under this name */
#define TECHNIC_H_PATH "c:/KHCOM/MainApp/Game/Technic.h" /* MATCH: the original includes it under this name */
#define CARDBATTLE_H_PATH "c:/KHCOM/MainApp/Game/CardBattle.h" /* MATCH: the original includes it under this name */
#include "System/Task.h"
#include "System/TaskMan.h"
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
extern G_UV g_uvCRCounterNumTex[8][10];
}
template <class T> static inline T& _ref(T& v) { return v; }
/* PARTIAL: stand-in for Game/CardBattle CCardBattle (m_cardEnchant, getEnchantKind): gated by GAME_CARDBATTLE_CPP */
struct CB_ENCHANT_VIEW {
    BYTE _pad[0x17C];
    CARD m_cardEnchant[2];
};
/* CCardBattle::getEnchantKind(INT inUser), an inline in Game/CardBattle.h */
static inline INT _getEnchantKind(CCardBattle* p, INT inUser) {
    if (!inUser) AssertApp("c:/KHCOM/MainApp/Game/CardBattle.h", 641, "inUser", 0);
    return ((CB_ENCHANT_VIEW*)p)->m_cardEnchant[inUser - 1].m_nKind < 0x44 ? 0 : ((CB_ENCHANT_VIEW*)p)->m_cardEnchant[inUser - 1].m_nKind - 0x43;
}
static inline BOOL _isEqual(UINT a, UINT b) { return a == b; }






extern INT SCREEN_HEIGHT;
#line 381

void CardBattle::convBtlDeckToCard(BTL_DECK* inDeck) {
    for (INT i = 0; i < 100; i++) {
        if (inDeck[i].m_nKind <= 0 || inDeck[i].m_nKind > 202) break;
        ((CARD*)&inDeck[i])->init((BYTE)inDeck[i].m_nKind, inDeck[i].m_nNumber);
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
CCardRevolver::~CCardRevolver() {
    if (m_hPlaySE > 0) {
        CSound::stopSE(m_hPlaySE, m_iReloadSEID);
        m_hPlaySE = 0;
    }
    if (m_hChargeSE > 0) {
        CSound::stopSE(m_hChargeSE, 0x13);
        m_hChargeSE = 0;
    }
    deleteCounter();
    deleteStock();
    delete m_pEmpty;
    m_pEmpty = NULL;
    delete[] m_pCards;
    m_pCards = NULL;
    delete[] m_pCardsE;
    m_pCardsE = NULL;
}
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
        m_cardAdd[i].init(0, 0);
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
    dummy.init(0xFF, 0);
    setDeck(&dummy, -1);
}
void CCardRevolver::_initCounter() {
    initCounter();
    m_pEmpty = new ("c:/KHCOM/MainApp/Game/CardRevolver.cpp(810)") C2DCard;
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
/* MATCH: the card dumps read m_cardData directly (friend in BattleCard.h): the original evaluates kind and number
   after the getter-call arguments of the stack-passed TRACE, so they contain no call */
void CCardRevolver::main() {
    INT i;
    CBattleCard* pBC;
    m_uiFlag = (INT)m_uiFlag & ~6;
    if (dbgCheckUser(USR_PROG) && m_uiUser == 1 && dbgButtonDown(USR_SYSTEM, 8, 1)) {
        TRACE("PLCR: ========================================\n");
        TRACE("Rev: Mode = MODE_NORMAL\n");
        TRACE("Rev: nTotal = %d, nRemain = %d, nActive = %d,\n", m_nTotal[0], m_nRemain[0], m_nActive[0]);
        TRACE("Rev: nLimit = %d, nExtra = %d, iMax = %d\n", m_nLimit, m_nExtra, m_iMax);
        TRACE("Rev: bActive = %d, tqTimer = %d, uiFlag = %08x,\n", m_bActive, m_tqTimer, m_uiFlag);
        TRACE("PLCR: Cards--------------------------------\n");
        for (i = 0; i < m_nLimit; i++) {
            pBC = &m_pCards[i];
            if (pBC) {
                TRACE("%3d: [Card%03d:%d(%d)] iStock=%d, iHand=%d/%d, uiFlag=%08x, uiOwner=%d,\n", i, pBC->m_cardData.m_nKind,
                      pBC->m_cardData.m_nNumber, pBC->getBaseCardNumber(), pBC->getStock(), pBC->getHand(),
                      pBC->getHandMax(), pBC->getFlag(), pBC->getCardOwner());
            } else {
                TRACE("%3d: NULL,\n", i);
            }
        }
        TRACE("PLCR: ========================================\n");
    }
    switch (m_iState) {
    case 0:
        exeWait();
        break;
    case 1:
        if (m_hPlaySE > 0) {
            CSound::stopSE(m_hPlaySE, m_iReloadSEID);
            m_hPlaySE = 0;
        }
        if (exeReturnCard()) break;
        if (exeAddCard()) break;
        if (exeSnatchCard()) break;
        if (exeDiscard()) break;
        exeMain();
        break;
    case 2:
        exeReload();
        break;
    case 3:
        exeAction();
        break;
    case 4:
        exeCounter();
        exeNumEffect();
        exeStock();
        return;
    }
    if (m_hChargeSE > 0 && !(m_uiFlag & 0x20)) {
        CSound::stopSE(m_hChargeSE, 0x13);
        m_hChargeSE = 0;
    }
    m_uiFlag &= ~0x10020;
    exeCounter();
    exeNumEffect();
    exeStock();
    if (dbgCheckUser(USR_PROG) && dbgButtonTrg(USR_SYSTEM, 2, 1)) {
        if (dbgButtonDown(USR_SYSTEM, 4, 1) && m_uiUser == 1) {
            INT i;
            CBattleCard* pBC;
            TRACE("PLCR: ========================================\n");
            TRACE("Rev: Mode = MODE_NORMAL\n");
            TRACE("Rev: nTotal = %d, nRemain = %d, nActive = %d,\n", m_nTotal[0], m_nRemain[0], m_nActive[0]);
            TRACE("Rev: nLimit = %d, nExtra = %d, iMax = %d\n", m_nLimit, m_nExtra, m_iMax);
            TRACE("Rev: bActive = %d, tqTimer = %d, uiFlag = %08x,\n", m_bActive, m_tqTimer, m_uiFlag);
            TRACE("PLCR: Cards--------------------------------\n");
            for (i = 0; i < m_nLimit; i++) {
                pBC = &m_pCards[i];
                if (pBC) {
                    TRACE("%3d: [Card%03d:%d(%d)] iStock=%d, iHand=%d/%d, uiFlag=%08x, uiOwner=%d,\n", i, pBC->m_cardData.m_nKind,
                          pBC->m_cardData.m_nNumber, pBC->getBaseCardNumber(), pBC->getStock(), pBC->getHand(),
                          pBC->getHandMax(), pBC->getFlag(), pBC->getCardOwner());
                } else {
                    TRACE("%3d: NULL,\n", i);
                }
            }
            TRACE("PLCR: ========================================\n");
        }
        if (dbgButtonDown(USR_SYSTEM, 1, 1) && m_uiUser == 2) {
            INT i;
            CBattleCard* pBC;
            TRACE("ENCR: ========================================\n");
            TRACE("Rev: nTotal = %d, nRemain = %d, nActive = %d,\n", m_nTotal[0], m_nRemain[0], m_nActive[0]);
            TRACE("Rev: nLimit = %d, nExtra = %d, iMax = %d\n", m_nLimit, m_nExtra, m_iMax);
            TRACE("Rev: bActive = %d, tqTimer = %d, uiFlag = %08x,\n", m_bActive, m_tqTimer, m_uiFlag);
            TRACE("ENCR: Cards--------------------------------\n");
            for (i = 0; i < m_nLimit; i++) {
                pBC = &m_pCards[i];
                if (pBC) {
                    TRACE("%3d: [Card%03d:%d] iStock=%d, iHand=%d/%d, uiFlag=%08x, uiOwner=%d,\n", i, pBC->m_cardData.m_nKind,
                          pBC->m_cardData.m_nNumber, pBC->getStock(), pBC->getHand(), pBC->getHandMax(),
                          pBC->getFlag(), pBC->getCardOwner());
                } else {
                    TRACE("%3d: NULL,\n", i);
                }
            }
            TRACE("ENCR: ========================================\n");
        }
    }
}
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
    card.init(0xA0, m_iChargeCount);
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
    a = 1;
    if (num < 0) a = -1;
    __asm__ volatile(""); /* MATCH: keeps the movz out of the bltzl slot */
    num = num < 0 ? -num : num;
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
    { INT t = (USHORT)m_nTotal[m_iMode] << 16; limit = m_iMode == 0 ? m_nLimit : t >> 16; }
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
#line 1043
INT CCardRevolver::setPlayCard(INT inFlag, INT inTest) {
    UINT flag;
    CBattleCard* card;
    CARD* data;
    INT num;
    INT category;
    BOOL bEnchant;
    CCardBattle* pCB;
    INT result;
    if (CCardBattle::isDisablePlayCard()) return 0;
    if (m_iState != 1) return 0;
    if (CGameWork::getFlag(1, 8)) return 0;
    flag = (m_uiUser == 1) ? 2 : 4;
    if (CGameWork::getInt(1, 0xE2) & flag) return 0;
    card = getActiveCard();
    if (!card) return 0;
    data = card->getCardData();
    num = data->m_nNumber;
    category = data->m_nCategory1;
    bEnchant = FALSE;
    pCB = (CCardBattle*)CTaskMan::getSysTask(0x16);
    if (pCB->isPlayingGimmickCard()) return 0;
    switch (category) {
    case 7:
        bEnchant = (num == -1);
        break;
    case 2:
        if (_getEnchantKind(pCB, m_uiUser) == 0x1F) return 0;
        break;
    case 3:
        if (_getEnchantKind(pCB, m_uiUser) == 0x19) return 0;
        break;
    case 6:
    case 14: {
        CARD gimmick;
        gimmick.init(data->m_nKind, 0);
        pCB->notifyMakeGimmick(&gimmick, 1);
        break;
    }
    case 8:
        return 0;
    }
    if (inFlag) {
        if (!pCB->testBreakingCard(m_uiUser)) return 0;
        TRACE("CCardRevolver::setPlayCard \x81\xa6pCB->isPlayingCardStandNow==OK! pCB->resetNowPlaying\n");
        pCB->resetNowPlaying(m_uiUser);
    }
    if (!bEnchant) {
        if (DebugWork::g_dw.player.iCardNumLock != -1) {
            CARD dbug_card;
            if (DebugWork::g_dw.player.iCardNumLock < 10) {
                dbug_card.init(data->m_nKind, DebugWork::g_dw.player.iCardNumLock);
                card->changeData(dbug_card);
            } else {
                num = CCardBattle::getPlayingCardPoint(0);
                if (num != -1 && num < 10) {
                    dbug_card.init(data->m_nKind, num);
                    card->changeData(dbug_card);
                }
            }
        }
        result = pCB->setPlayCard(m_hOwner, m_uiUser, card, NULL, inTest);
    } else {
        if (DebugWork::g_dw.player.iEnemyCardLock) {
            CARD dbug_card;
            dbug_card.init(DebugWork::g_dw.player.iEnemyCardLock + 0x43, 0xFF);
            card->changeData(dbug_card);
        }
        result = pCB->setEnchantCard(m_hOwner, m_uiUser, card);
    }
    if (!result) return 0;
    if (data->m_fgPremium && _getEnchantKind(pCB, m_uiUser) != 0x3A) {
        if (!DebugWork::g_dw.player.iCardSpend) {
            if (data->m_nCategory1 != 1 || getHave(1, 1, 1, 0, 0) != 1) card->setReloadNG();
        }
    } else {
        switch (category) {
        case 7:
            if (num == -1 && !DebugWork::g_dw.player.iCardSpend) card->setReloadNG();
            break;
        case 4:
        case 5:
        case 6:
            if (!DebugWork::g_dw.player.iCardSpend) card->setReloadNG();
            break;
        case 14:
            card->setReloadNG();
            break;
        }
    }
    m_nRemain[m_iMode]--;
    m_tqRollSpeed = 300;
    if (m_iMode > 0) {
        rollDeck(1, 1, 300);
        changeMode(0, 1);
    } else {
        m_pEmpty->enableDisp(0);
        rollDeck(1, 1, 300);
    }
    return result;
}
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
void CCardRevolver::exeReload() {
    CBattleCard* card;
    INT n;
    INT i;
    INT category;
    m_tqTimer -= getFrameRate();
    if (m_tqTimer > 0) return;
    if (m_uiFlag & 8) if (m_uiFlag & 0x20000) {
        rollDeck(0, 1, 300);
        m_iState = 1;
        m_tqTimer = 0;
        CGameWork::setFlag(1, 0x13, 1);
        TRACE("CCardRevolver::exeReload(); RELOAD end.\n");
        return;
    }
    m_tqTimer = 200;
    card = NULL;
    n = m_nLimit - 1;
    for (i = 0; i < m_nLimit; i++, card = NULL) {
        card = &m_pCards[n];
        if (card) {
            if (card->isReloadSet()) {
                card->resetReloadSet();
                category = card->getCardData()->m_nCategory1;
                switch (category) {
                case 8:
                    break;
                case 4:
                    if (m_uiFlag & 0x100) {
                        n--; /* MATCH: n-- is a body statement (the original's line markers), so the continue needs its own */
                        continue;
                    }
                default:
                    m_nRemain[0]++;
                    break;
                }
                m_nActive[0] = n;
                m_nReloadCount++;
                break;
            }
        }
        n--;
    }
    if (card) card->resetHand(m_iMax - m_nReloadCount, m_iMax);
    m_uiFlag |= 0x20000;
    for (i = 0; i < m_nLimit; i++) {
        card = &m_pCards[i];
        if (card) {
            if (card->isReloadSet()) {
                m_uiFlag &= ~0x20000;
                goto check; /* MATCH: not break: a jump to the loop's end label makes expand_end_loop rotate this block to the bottom with the exit test; loop.c moves it out as in the original */
            }
        }
    }
check:
    if (m_uiFlag & 0x20000) {
        m_tqTimer = 400;
        TRACE("CCardRevolver::exeReload(); RELOAD complete!\n");
    }
    m_uiFlag |= 8;
}
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
void CCardRevolver::setStockCard() {
    CBattleCard* card;
    CCardBattle* pCB;
    CARD* data;
    INT num;
    INT category;
    if (CGameWork::getFlag(1, 10)) return;
    if (m_iState != 1) return;
    if (m_iMode) return;
    card = getActiveCard();
    if (!card) return;
    pCB = (CCardBattle*)CTaskMan::getSysTask(0x16);
    if (CCardBattle::isPlayingCard(m_uiUser)) return;
    if (m_iStock == 3) return;
    data = card->getCardData();
    category = data->m_nCategory1;
    num = data->m_nNumber;
    switch (category) {
    case 7:
        if (num == -1) return;
        break;
    case 2:
        if (_getEnchantKind(pCB, m_uiUser) == 0x1F) return;
        break;
    case 3:
        if (_getEnchantKind(pCB, m_uiUser) == 0x19) return;
        break;
    case 6:
    case 8:
    case 14:
        return;
    }
    card->setStock(m_iStock);
    m_pCardsS[m_iStock] = card;
    m_pDataS[m_iStock] = data;
    m_tqRollSpeed = 300;
    m_iStock++;
    m_nRemain[0]--;
    rollDeck(1, 1, 300);
    CSound::playSE(0xF, -1, 0x40);
    if (m_uiUser == 1 && m_iStock == 3) CGameWork::setFlag(1, 0x15, 1);
}
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
INT CCardRevolver::calcStockSkill() {
    INT nSkill;
    INT nSame;
    INT sameCardID;
    INT nAttack;
    INT nMagic;
    INT nSum;
    INT index;
    INT category;
    INT i;
    UINT uiCode;
    UINT uiCode2;
    UINT uiCode02;
    USHORT nCodeCategory;
    if (m_uiUser != 1) return calcEnemyStockSkill();
    nSkill = 0;
    m_iExtraSkill = 0;
    nSame = 0;
    m_iStockSkill = m_iStock;
    m_iFirstSkill = 0;
    sameCardID = 0;
    nAttack = 0;
    nMagic = 0;
    uiCode = 0;
    nCodeCategory = 0;
    nSum = 0;
    for (i = 0; i < m_iStock; i++) {
        category = m_pDataS[i]->m_nCategory1;
        index = m_pDataS[i]->m_nKind;
        switch (category) {
        case 1:
            nAttack++;
            break;
        case 2:
            nMagic++;
            break;
        case 3:
        case 4:
            break;
        case 5:; // MATCH: 5 falls off the end, so it is not grouped with 3-4 (5 case nodes give the jump table)
        }
        if (index == m_pDataS[0]->m_nKind) {
            nSame++;
            sameCardID = index;
        }
        uiCode |= index << (i * 8);
        nSum += m_pDataS[i]->m_nNumber;
        nCodeCategory |= category << (i * 4);
    }
    uiCode2 = (uiCode >> 8) & 0xFFFF;
    uiCode02 = uiCode & 0xFFFF; // MATCH: the tests below use uiCode & 0xFFFF (gcse copy), the pair switch uiCode02
    if (nSame == 1) {
        nSame = 0;
        for (i = 1; i < m_iStock; i++) {
            index = m_pDataS[i]->m_nKind;
            if (index == m_pDataS[1]->m_nKind) {
                sameCardID = index;
                nSame++;
            }
        }
    }
    if (DebugWork::g_dw.player.iSkillLock >= 4) {
        m_iStockSkill = DebugWork::g_dw.player.iSkillLock;
        goto end;
    }
    if (m_uiFlag & 0x200) {
        m_iStockSkill = CCardBattle::getDuelSkill();
        goto end;
    }
    if (m_iStock == 3) {
        if (nAttack == 3) {
            if (sameCardID == 0x12) {
                if (m_uiFlag & 0x100) {
                    if (nSum == 27) nSkill = 0x61;
                    else if (nSum >= 5 && nSum <= 15) nSkill = 0x5F;
                    else if (nSum >= 16 && nSum <= 25) nSkill = 0x60;
                }
            } else if (nSum == 0 || nSum == 27) {
                nSkill = 7;
            } else if (nSum < 7) {
                nSkill = 10;
            } else if (nSum < 10) {
                nSkill = 11;
            } else if (nSum < 16) {
                if (nSame == 3) nSkill = 4;
                else if (nSame == 1) nSkill = 5;
            } else if (nSum >= 24) {
                nSkill = 8;
            } else if (nSum >= 20) {
                if (nSame == 3) nSkill = 6;
                else if (nSame == 1) nSkill = 9;
            }
            if (nSkill && CTechnic::canUse(nSkill)) {
                m_iStockSkill = nSkill;
                goto end;
            }
        }
        switch (uiCode) {
        case 0x1A1A1A:
            nSkill = 0x14;
            break;
        case 0x1B1B1B:
            nSkill = 0x15;
            break;
        case 0x1C1C1C:
            nSkill = 0x16;
            break;
        case 0x1D1D1D:
            nSkill = 0x17;
            break;
        case 0x1E1E1E:
            nSkill = 0x18;
            break;
        case 0x1F1F1F:
            nSkill = 0x19;
            break;
        case 0x202020:
            nSkill = 0x1A;
            break;
        case 0x201E1F:
            nSkill = 0x2B;
            break;
        case 0x20201F:
            nSkill = 0x2C;
            break;
        case 0x201E1D:
            nSkill = 0x30;
            break;
        case 0x1E1A1A:
            nSkill = 0x20;
            break;
        case 0x1A1A20:
            nSkill = 0x21;
            break;
        case 0x1A1A28:
            nSkill = 0x22;
            break;
        case 0x1F1B1B:
            nSkill = 0x23;
            break;
        case 0x201A1B:
            nSkill = 0x25;
            break;
        case 0x232323:
            nSkill = 0x35;
            break;
        case 0x262626:
            nSkill = 0x37;
            break;
        case 0x252525:
            nSkill = 0x39;
            break;
        case 0x282828:
            nSkill = 0x3C;
            break;
        case 0x242424:
            nSkill = 0x3E;
            break;
        case 0x272727:
            nSkill = 0x40;
            break;
        case 0x292929:
            nSkill = 0x42;
            break;
        case 0x313131:
            nSkill = 0x45;
            break;
        case 0x323232:
            nSkill = 0x48;
            break;
        case 0x333333:
            nSkill = 0x4C;
            break;
        case 0x353535:
            nSkill = 0x4E;
            break;
        case 0x343434:
            nSkill = 0x50;
            break;
        case 0x363636:
            nSkill = 0x52;
            break;
        case 0x373737:
            nSkill = 0x54;
            break;
        case 0x393939:
            nSkill = 0x56;
            break;
        case 0x383838:
            nSkill = 0x63;
            break;
        }
        if (nSkill && CTechnic::canUse(nSkill)) {
            m_iStockSkill = nSkill;
            goto end;
        }
        if (nAttack == 2) {
            switch (uiCode & 0xFF) {
            case 0x1A:
                nSkill = 0x1B;
                break;
            case 0x1B:
                nSkill = 0x1C;
                break;
            case 0x1C:
                nSkill = 0x1D;
                break;
            case 0x1F:
                nSkill = 0x27;
                break;
            case 0x20:
                nSkill = 0x1F;
                break;
            case 0x29:
                nSkill = 0x1E;
                break;
            case 0x23:
                nSkill = 0x28;
                break;
            case 0x25:
                nSkill = 0x3A;
                break;
            case 0x38:
                nSkill = 0x64;
                if (m_uiFlag & 0x100) nSkill = 0x65;
                break;
            }
            if (nSkill && CTechnic::canUse(nSkill)) {
                m_iStockSkill = nSkill;
                goto end;
            }
        }
        switch (nCodeCategory) {
        case 0x551:
            if ((uiCode & 0xFFFF00) == 0x313200 || (uiCode & 0xFFFF00) == 0x323100) nSkill = 0xC;
            break;
        case 0x515:
            if ((uiCode & 0xFF00FF) == 0x310032 || (uiCode & 0xFF00FF) == 0x320031) nSkill = 0xC;
            break;
        case 0x155:
            if ((uiCode & 0xFFFF) != 0x3132) {
                if ((uiCode & 0xFFFF) == 0x3231) nSkill = 0xC;
            } else {
                nSkill = 0x4A;
            }
            break;
        case 0x222:
            switch (uiCode & 0xFFFF) {
            case 0x1B20:
                nSkill = 0x24;
                break;
            case 0x1F1E:
                nSkill = 0x2D;
                break;
            }
            break;
        case 0x322:
            if ((uiCode & 0xFFFF) == 0x1E20) nSkill = 0x29;
            break;
        case 0x232:
            if ((uiCode & 0xFFFF) == 0x231E) nSkill = 0x2A;
            break;
        case 0x333:
            if ((uiCode & 0xFFFF) == 0x2724) nSkill = 0x2E;
            break;
        case 0x533:
            if ((uiCode & 0xFF0000) == 0x350000) nSkill = 0x2F;
            break;
        case 0x433:
            if ((uiCode & 0xFFFF) == 0x2823) nSkill = 0x2F;
            break;
        case 0x523:
            if ((uiCode & 0xFF0000) == 0x350000) nSkill = 0x31;
            break;
        case 0x423:
            if ((uiCode & 0xFFFF) == 0x1B25) nSkill = 0x31;
            break;
        case 0x123:
            if ((uiCode & 0xFFFF) == 0x1F29) nSkill = 0x43;
            break;
        case 0x522:
            if ((uiCode & 0xFF0000) == 0x360000) nSkill = 0x32;
            break;
        case 0x422:
            if ((uiCode & 0xFFFF) == 0x201F) nSkill = 0x32;
            break;
        case 0x444:
            if ((uiCode & 0xFFFF) == 0x302E) nSkill = 0x33;
            break;
        case 0x122:
            if ((uiCode & 0xFFFF) == 0x1E1E) nSkill = 0x26;
            break;
        case 0x133: // MATCH: an empty case between 0x123 and 0x155 (the tree splits at 0x155)
            break;
        }
        if (nSkill && CTechnic::canUse(nSkill)) {
            m_iStockSkill = nSkill;
            goto end;
        }
    }
    if ((m_uiFlag & 0x800) && nMagic >= 2) {
        if ((uiCode & 0xFFFF00) == 0x212200 || (uiCode & 0xFFFF00) == 0x222100) nSkill = 0x57;
        if ((uiCode & 0xFF00FF) == 0x2122 || (uiCode & 0xFF00FF) == 0x2221) nSkill = 0x57;
        if ((uiCode & 0xFFFF) == 0x2122 || (uiCode & 0xFFFF) == 0x2221) nSkill = 0x57;
        if (nSkill) {
            m_iStockSkill = nSkill;
            goto end;
        }
        if (uiCode == 0x222222) {
            m_iStockSkill = 0x59;
            goto end;
        }
        if ((uiCode & 0xFFFF00) == 0x222200) {
            if (m_iStock == 3) m_iFirstSkill = m_pDataS[0]->m_nKind;
            nSkill = 0x58;
        }
        if ((uiCode & 0xFFFF) == 0x2222) {
            if (m_iStock == 3) m_iExtraSkill = m_pDataS[2]->m_nKind;
            nSkill = 0x58;
        }
        if (nSkill) {
            m_iStockSkill = nSkill;
            goto end;
        }
    }
    switch (uiCode02) {
    case 0x1A1A:
        nSkill = 0xD;
        break;
    case 0x1B1B:
        nSkill = 0xE;
        break;
    case 0x1C1C:
        nSkill = 0xF;
        break;
    case 0x1D1D:
        nSkill = 0x10;
        break;
    case 0x1E1E:
        nSkill = 0x11;
        break;
    case 0x1F1F:
        nSkill = 0x12;
        break;
    case 0x2020:
        nSkill = 0x13;
        break;
    case 0x2323:
        nSkill = 0x34;
        break;
    case 0x2626:
        nSkill = 0x36;
        break;
    case 0x2525:
        nSkill = 0x38;
        break;
    case 0x2828:
        nSkill = 0x3B;
        break;
    case 0x2424:
        nSkill = 0x3D;
        break;
    case 0x2727:
        nSkill = 0x3F;
        break;
    case 0x2929:
        nSkill = 0x41;
        break;
    case 0x3131:
        nSkill = 0x44;
        break;
    case 0x3232:
        nSkill = 0x47;
        break;
    case 0x3333:
        nSkill = 0x4B;
        break;
    case 0x3535:
        nSkill = 0x4D;
        break;
    case 0x3434:
        nSkill = 0x4F;
        break;
    case 0x3636:
        nSkill = 0x51;
        break;
    case 0x3737:
        nSkill = 0x53;
        break;
    case 0x3939:
        nSkill = 0x55;
        break;
    case 0x1A31:
        nSkill = 0x46;
        break;
    case 0x3838:
        nSkill = 0x62;
        break;
    default:
        if ((nCodeCategory & 0xFF) == 0x15 && (uiCode02 & 0xFF) == 0x32) nSkill = 0x49;
        break;
    }
    if (nSkill && CTechnic::canUse(nSkill)) {
        m_iStockSkill = nSkill;
        if (m_iStock == 3) m_iExtraSkill = m_pDataS[2]->m_nKind;
        goto end;
    }
    switch (uiCode2) {
    case 0x1A1A:
        nSkill = 0xD;
        break;
    case 0x1B1B:
        nSkill = 0xE;
        break;
    case 0x1C1C:
        nSkill = 0xF;
        break;
    case 0x1D1D:
        nSkill = 0x10;
        break;
    case 0x1E1E:
        nSkill = 0x11;
        break;
    case 0x1F1F:
        nSkill = 0x12;
        break;
    case 0x2020:
        nSkill = 0x13;
        break;
    case 0x2323:
        nSkill = 0x34;
        break;
    case 0x2626:
        nSkill = 0x36;
        break;
    case 0x2525:
        nSkill = 0x38;
        break;
    case 0x2828:
        nSkill = 0x3B;
        break;
    case 0x2424:
        nSkill = 0x3D;
        break;
    case 0x2727:
        nSkill = 0x3F;
        break;
    case 0x2929:
        nSkill = 0x41;
        break;
    case 0x3131:
        nSkill = 0x44;
        break;
    case 0x3232:
        nSkill = 0x47;
        break;
    case 0x3333:
        nSkill = 0x4B;
        break;
    case 0x3535:
        nSkill = 0x4D;
        break;
    case 0x3434:
        nSkill = 0x4F;
        break;
    case 0x3636:
        nSkill = 0x51;
        break;
    case 0x3737:
        nSkill = 0x53;
        break;
    case 0x3939:
        nSkill = 0x55;
        break;
    case 0x1A31:
        nSkill = 0x46;
        break;
    case 0x3838:
        nSkill = 0x62;
        break;
    default:
        if (((nCodeCategory & 0xFF0) >> 4) == 0x15 && (uiCode2 & 0xFF) == 0x32) nSkill = 0x49;
        break;
    }
    if (nSkill && CTechnic::canUse(nSkill)) {
        m_iStockSkill = nSkill;
        m_iFirstSkill = m_pDataS[0]->m_nKind;
    }
end:
    m_iStockSum = nSum;
    setStockInfo();
    ((PLM::CPlayerManager*)CTaskMan::getSysTask(0x1F))->setReqLoad(m_iStockSkill, 0);
    return m_iStockSkill;
}
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
void CCardRevolver::initAction() {
    BOOL bReloadAC;
    BOOL bReloadAllAC;
    BOOL bReloadReset;
    BOOL bReloadMC;
    BOOL bReloadAllMC;
    BOOL bReloadOC;
    INT i;
    CBattleCard* card;
    INT category;
    INT n;
    CBattleCard* pCards;
    TRACE("CCardRevolver::initAction() %d,\n", m_iAction);
    m_iState = 3;
    m_bActive = FALSE;
    bReloadAC = FALSE;
    bReloadAllAC = FALSE;
    bReloadReset = FALSE;
    bReloadMC = FALSE;
    bReloadAllMC = FALSE;
    bReloadOC = FALSE;
    switch (m_iAction) {
    case 0x2A:
        bReloadAC = TRUE;
        CCardBattle::decEnchantTime(m_uiUser, 0x29, 0);
        break;
    case 0x2B:
        bReloadAC = TRUE;
        bReloadAllAC = TRUE;
        CCardBattle::decEnchantTime(m_uiUser, 0x29, 0);
        break;
    case 0x2C:
        bReloadAC = TRUE;
        bReloadAllAC = TRUE;
        bReloadReset = TRUE;
        CCardBattle::decEnchantTime(m_uiUser, 0x29, 0);
        break;
    case 0x2D:
        bReloadMC = TRUE;
        CCardBattle::decEnchantTime(m_uiUser, 0x29, 0);
        break;
    case 0x2E:
        bReloadMC = TRUE;
        bReloadAllMC = TRUE;
        bReloadReset = TRUE;
        CCardBattle::decEnchantTime(m_uiUser, 0x29, 0);
        break;
    case 0x2F:
        bReloadAC = TRUE;
        bReloadAllAC = TRUE;
        bReloadMC = TRUE;
        bReloadAllMC = TRUE;
        CCardBattle::decEnchantTime(m_uiUser, 0x29, 0);
        break;
    case 0x30:
        bReloadAC = TRUE;
        bReloadAllAC = TRUE;
        bReloadMC = TRUE;
        bReloadAllMC = TRUE;
        bReloadReset = TRUE;
        CCardBattle::decEnchantTime(m_uiUser, 0x29, 0);
        break;
    case 0x50:
        bReloadAC = TRUE;
        bReloadMC = TRUE;
        bReloadOC = TRUE;
        CCardBattle::decEnchantTime(m_uiUser, 0x29, 0);
        break;
    case 0x38:
        bReloadAC = TRUE;
        bReloadMC = TRUE;
        bReloadOC = TRUE;
        CCardBattle::decEnchantTime(m_uiUser, 0x29, 0);
        break;
    default:
        return;
    }
    if (m_iMode == 1) {
        pCards = m_pCardsE;
        for (i = 0; i < m_nTotal[1]; i++) {
            if (pCards[i].getHand() != -1) pCards[i].hideHand();
        }
        if (m_pEmpty) m_pEmpty->enableDisp(0);
        m_iMode = 0;
    }
    m_nRemain[0] = 0;
    for (i = 0, n = m_nLimit - 1; i < m_nLimit; i++) {
        card = &m_pCards[n];
        if (card) {
            if (card->isNowPlaying() || card->getStock() != -1 || (card->getCardOwner() != m_uiUser && m_uiUser == 1)) {
                n--;
                continue;
            }
            card->hideHand();
            category = card->getCardData()->m_nCategory1;
            if (card->getHand() != -1) {
                m_nActive[0] = n;
                if (category != 8) {
                    if (category != 4 || !(m_uiFlag & 0x100)) {
                        m_nRemain[0]++;
                    }
                } else {
                    if (bReloadReset) {
                        m_iChargeCount = 1;
                        m_iCharge = 0;
                        m_tqCharge = 0;
                        card->updateReload(0);
                    }
                }
                card->resetHand(m_nActive[0], m_nTotal[0]);
            } else {
                switch (category) {
                case 1:
                    if (bReloadAC) {
                        if (bReloadAllAC) card->resetReloadNG();
                        if (!card->isReloadNG()) {
                            m_nActive[0] = n;
                            m_nRemain[0]++;
                            card->resetHand(m_nActive[0], m_nTotal[0]);
                        }
                    }
                    break;
                case 2:
                case 3:
                    if (bReloadMC) {
                        if (bReloadAllMC) card->resetReloadNG();
                        if (!card->isReloadNG()) {
                            m_nActive[0] = n;
                            m_nRemain[0]++;
                            card->resetHand(m_nActive[0], m_nTotal[0]);
                        }
                    }
                    break;
                case 8:
                    TRACE("**** CAUTION!! **** initAction()\n");
                    break;
                case 4:
                    if (m_uiFlag & 0x100) break;
                default:
                    if (bReloadOC) {
                        if (!card->isReloadNG()) {
                            m_nActive[0] = n;
                            m_nRemain[0]++;
                            card->resetHand(m_nActive[0], m_nTotal[0]);
                        }
                    }
                    break;
                }
            }
        }
        n--;
    }
    m_uiFlag = (INT)m_uiFlag & ~8;
    m_tqTimer = 400;
    m_iMax = m_nRemain[0] + m_nReloadCard;
}
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
    m_cardAdd[num].init(0, 0);
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
void CCardRevolver::exeCounter() {
    INT iMax;
    INT iRemain;
    INT nRemain;
    INT ofsID;
    if (m_uiUser != 1) return;
    switch (m_uiCounterState) {
    case 0:
    case 3:
        break;
    case 1:
    case 2:
        if (CGameWork::getInt(1, 0xE2) & 2) {
            m_uiCounterState = 4;
            m_pSeqCtrlCounter->replaceSeq(0, _ref(g_szSeqCockpit), 0x44);
            m_uiCounterFlag |= 1;
            m_pSeqCtrlCounter->enableDisp(2, FALSE);
            m_pSeqCtrlCounter_Num01->enableDisp(FALSE);
            m_pSeqCtrlCounter_Num10->enableDisp(FALSE);
        }
        break;
    case 4:
        if (!(CGameWork::getInt(1, 0xE2) & 2)) {
            m_uiCounterState = 1;
            m_pSeqCtrlCounter->replaceSeq(0, _ref(g_szSeqCockpit), 0x42);
            m_uiCounterFlag = (INT)m_uiCounterFlag & ~1;
        }
        break;
    }
    switch (m_uiScutState) {
    case 0:
    case 3:
    case 4:
        break;
    case 1:
    case 2:
        if (CGameWork::getInt(1, 0xE2) & 2) {
            m_uiScutState = 5;
            m_pSeqCtrlCounter->replaceSeq(1, _ref(g_szSeqCockpit), 0x40);
            m_uiCounterFlag |= 2;
        }
        break;
    case 5:
        if (!(CGameWork::getInt(1, 0xE2) & 2)) {
            m_uiScutState = 1;
            m_pSeqCtrlCounter->replaceSeq(1, _ref(g_szSeqCockpit), 0x3E);
            m_uiCounterFlag = (INT)m_uiCounterFlag & ~2;
        }
        break;
    }
    switch (m_uiCounterState) {
    case 0:
        break;
    case 1:
        if ((m_uiCounterFlag & 1) == 0) { // MATCH: == 0 keeps andi/bnez (! folds the bit-0 test into xori)
            m_uiCounterFlag |= 1;
            m_pSeqCtrlCounter->enableDisp(0, TRUE);
        }
        if (m_pSeqCtrlCounter->isEnd(0)) {
            m_uiCounterState = 2;
            m_pSeqCtrlCounter->replaceSeq(0, _ref(g_szSeqCockpit), 0x43);
            m_pSeqCtrlCounter->enableDisp(2, TRUE);
            m_pSeqCtrlCounter_Num01->enableDisp(TRUE);
            m_pSeqCtrlCounter_Num10->enableDisp(TRUE);
        }
        break;
    case 2:
        if (m_iMode != m_nOldMode || m_nRemain[m_iMode] != m_nOldRemain) {
            iMax = m_nTotal[m_iMode];
            if (m_iMode == 0) iMax -= m_nReloadCard;
            if (iMax != 0) iRemain = m_nRemain[m_iMode] * 100 / iMax;
            else iRemain = 0;
            if (iRemain > 100) iRemain = 100;
            m_pSeqCtrlCounter->enableDisp(2, TRUE);
            m_pSeqCtrlCounter->changeTime(2, iRemain);
            nRemain = (m_nRemain[m_iMode] > 99) ? 99 : m_nRemain[m_iMode];
            ofsID = CSceneInfo::isMinigameScene() ? m_uiNumTexID + 2 : 0;
            m_pSeqCtrlCounter_Num01->changeTexture(0, 0, NULL, &g_uvCRCounterNumTex[ofsID][nRemain % 10], &g_uvCRCounterNumTex[ofsID + 1][nRemain % 10]);
            nRemain /= 10;
            if (nRemain > 0) {
                if (m_uiCounterFlag & 4) {
                    m_uiCounterFlag = (INT)m_uiCounterFlag & ~4;
                    m_pSeqCtrlCounter_Num10->replaceSeq(0, _ref(g_szSeqCockpit), 0x46);
                }
                m_pSeqCtrlCounter_Num10->changeTexture(0, 0, NULL, &g_uvCRCounterNumTex[ofsID][nRemain], &g_uvCRCounterNumTex[ofsID + 1][nRemain]);
            } else {
                m_uiCounterFlag |= 4;
                m_pSeqCtrlCounter_Num10->replaceSeq(0, _ref(g_szSeqCockpit), 0x50);
            }
        }
        m_nOldMode = m_iMode;
        m_nOldRemain = m_nRemain[m_iMode];
        break;
    case 3:
        if (m_pSeqCtrlCounter->isEnd(0)) {
            m_uiCounterState = 0;
            m_pSeqCtrlCounter->enableDisp(0, FALSE);
        }
        break;
    case 4:
        if (m_uiCounterFlag & 1) {
            if (m_pSeqCtrlCounter->isEnd(0)) {
                m_uiCounterFlag = (INT)m_uiCounterFlag & ~1;
                m_pSeqCtrlCounter->enableDisp(0, FALSE);
            }
        }
        break;
    }
    switch (m_uiScutState) {
    case 0:
    case 2:
    case 3:
        break;
    case 1:
        if (!(m_uiCounterFlag & 2)) {
            m_uiCounterFlag |= 2;
            m_pSeqCtrlCounter->enableDisp(1, TRUE);
        }
        if (m_pSeqCtrlCounter->isEnd(1)) {
            m_uiScutState = 2;
            m_uiCounterFlag |= 2;
            m_pSeqCtrlCounter->replaceSeq(1, _ref(g_szSeqCockpit), 0x3F);
        }
        break;
    case 4:
        if (m_pSeqCtrlCounter->isEnd(1)) {
            m_uiScutState = 0;
            m_uiCounterFlag = (INT)m_uiCounterFlag & ~2;
            m_pSeqCtrlCounter->enableDisp(1, FALSE);
        }
        break;
    case 5:
        if (m_uiCounterFlag & 2) {
            if (m_pSeqCtrlCounter->isEnd(1)) {
                m_uiCounterFlag = (INT)m_uiCounterFlag & ~2;
                m_pSeqCtrlCounter->enableDisp(1, FALSE);
            }
        }
        break;
    }
}
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
    m_pSeqCtrlNumEff = new ("c:/KHCOM/MainApp/Game/CardRevolver.cpp(4430)") D2::C2DSeqCtrl;
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
        m_pSeqCtrlTrap = new ("c:/KHCOM/MainApp/Game/CardRevolver.cpp(4469)") D2::C2DSeqCtrl;
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
    m_pSeqCtrlStock = new ("c:/KHCOM/MainApp/Game/CardRevolver.cpp(4510)") D2::C2DSeqCtrl;
    m_pSeqCtrlStock_Num10 = new ("c:/KHCOM/MainApp/Game/CardRevolver.cpp(4511)") D2::C2DSeqCtrl;
    m_pSeqCtrlStock_Num01 = new ("c:/KHCOM/MainApp/Game/CardRevolver.cpp(4512)") D2::C2DSeqCtrl;
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
    m_p2DFntStockName = new ("c:/KHCOM/MainApp/Game/CardRevolver.cpp(4567)")
        D2::C2DFnt(0, 0, &rgba, 14, g_szStockSkillName[m_iStockSkill], 0x80);
    inSeq->addFntShift(g_szSeqCockpit, m_p2DFntStockName, -4.0f, 0.0f);
    inSeq->m_seqControl.fgColor = 1;
    inSeq->m_seqControl.fgFntScale = 1;
}
void CCardRevolver::exeStock() {
    UINT flag = (m_uiUser == 1) ? 2 : 4;
    switch (m_uiStockState) {
    case 0:
        if (m_iStock > 0 && (CGameWork::getInt(1, 0xE2) & flag)) {
            m_uiStockState = 5;
            m_uiStockFlag = (INT)m_uiStockFlag & ~1;
            m_pSeqCtrlStock_Num01->enableDisp(0, FALSE);
            m_pSeqCtrlStock_Num10->enableDisp(0, FALSE);
        }
        break;
    case 1:
    case 2:
    case 3:
        if (CGameWork::getInt(1, 0xE2) & flag) {
            m_uiStockState = 5;
            m_pSeqCtrlStock->enableDisp(FALSE);
            m_uiStockFlag = (INT)m_uiStockFlag & ~1;
            m_uiStockCategory = 0;
            m_pSeqCtrlStock_Num01->enableDisp(0, FALSE);
            m_pSeqCtrlStock_Num10->enableDisp(0, FALSE);
        }
        break;
    case 5:
        if (!(CGameWork::getInt(1, 0xE2) & flag)) setStockInfo();
        break;
    }
    if (m_iStock == 0 && (m_uiStockFlag & 1)) {
        m_uiStockFlag = (INT)m_uiStockFlag & ~1;
        m_pSeqCtrlStock_Num10->enableDisp(0, FALSE);
        m_pSeqCtrlStock_Num01->enableDisp(0, FALSE);
    }
    switch (m_uiStockState) {
    case 0:
        break;
    case 1:
        if (m_pSeqCtrlStock->isEnd(0)) {
            m_uiStockState = 2;
            m_pSeqCtrlStock->replaceSeq(0, _ref(g_szSeqCockpit), m_uiStockCategory + 1);
        }
        break;
    case 2:
        if (m_iStock) break;
        if (!CCardBattle::isExecutingCard(m_uiUser)) {
            m_uiStockState = 0;
            m_pSeqCtrlStock->enableDisp(FALSE);
        } else {
            m_uiStockState = 3;
        }
        break;
    case 3:
        if (CCardBattle::isExecutingCard(m_uiUser)) break;
        m_uiStockState = 4;
        m_pSeqCtrlStock->replaceSeq(0, _ref(g_szSeqCockpit), m_uiStockCategory + 2);
        break;
    case 4:
        if (m_pSeqCtrlStock->isEnd(0)) {
            m_uiStockState = 0;
            m_pSeqCtrlStock->enableDisp(FALSE);
        }
        break;
    case 5:
        break;
    }
}
void CCardRevolver::deleteStock() {
    delete m_pSeqCtrlStock;
    m_pSeqCtrlStock = NULL;
    delete m_pSeqCtrlStock_Num01;
    m_pSeqCtrlStock_Num01 = NULL;
    delete m_pSeqCtrlStock_Num10;
    m_pSeqCtrlStock_Num10 = NULL;
}
void CCardRevolver::setStockInfo() {
    CB_LAYOUT* pLayout;
    FLOAT ofsY;
    register FLOAT fX asm("$f0"); /* MATCH: pos.x read in each arm below */
    INT num;
    INT category;
    if (m_uiUser == 2) {
        CCardBattle* pCB = (CCardBattle*)CTaskMan::getSysTask(0x16);
        if (_getEnchantKind(pCB, m_uiUser) == 0x31) {
            m_pSeqCtrlStock->enableDisp(FALSE);
            m_uiStockState = 0;
            m_uiStockCategory = 0;
            m_uiStockFlag = (INT)m_uiStockFlag & ~1;
            m_pSeqCtrlStock_Num10->enableDisp(0, FALSE);
            m_pSeqCtrlStock_Num01->enableDisp(0, FALSE);
            return;
        }
    }
    m_p2DFntStockName->setText(g_szStockSkillName[m_iStockSkill]);
    ofsY = 0.0f;
    switch (m_uiUser) {
    case 1:
        if (m_iStockSkill >= 0x66) {
            pLayout = &g_LayoutData[35 + m_iStock];
            ofsY = 20.0f;
        } else if (m_iStockSkill >= 4) {
            pLayout = &g_LayoutData[26 + m_iStock];
        } else {
            pLayout = &g_LayoutData[23 + m_iStock];
        }
        break;
    case 2:
        pLayout = (m_iStockSkill >= 0x6F) ? &g_LayoutData[32 + m_iStock] : &g_LayoutData[29 + m_iStock];
        break;
    }
    FVECTOR pos = { 0.0f, 0.0f, 0.0f, 1.0f };
    num = m_iStockSum % 10;
    m_pSeqCtrlStock_Num01->changeTexture(0, 0, 0, NULL, &g_uvCRCounterNumTex[m_uiNumTexID][num], &g_uvCRCounterNumTex[m_uiNumTexID + 1][num]);
    m_pSeqCtrlStock_Num01->changeTexture(0, 1, 0, NULL, &g_uvCRCounterNumTex[m_uiNumTexID][num], &g_uvCRCounterNumTex[m_uiNumTexID + 1][num]);
    m_pSeqCtrlStock_Num01->enableDisp(0, TRUE);
    num = m_iStockSum / 10;
    if (num > 0) {
        m_pSeqCtrlStock_Num10->changeTexture(0, 0, 0, NULL, &g_uvCRCounterNumTex[m_uiNumTexID][num], &g_uvCRCounterNumTex[m_uiNumTexID + 1][num]);
        m_pSeqCtrlStock_Num10->changeTexture(0, 1, 0, NULL, &g_uvCRCounterNumTex[m_uiNumTexID][num], &g_uvCRCounterNumTex[m_uiNumTexID + 1][num]);
        m_pSeqCtrlStock_Num10->enableDisp(0, TRUE);
        if (m_uiUser == 1) {
            pos.x = pLayout->x + 0x1A;
            pos.y = pLayout->y - 0xB + ofsY;
            m_pSeqCtrlStock_Num10->setPos(pos);
            fX = pos.x;
        } else {
            pos.x = pLayout->x - 0x2B;
            pos.y = pLayout->y - 0xC;
            m_pSeqCtrlStock_Num10->setPos(pos);
            fX = pos.x;
        }
        /* MATCH: the load after each call keeps the call off the block end, so the arms cross-jump; a1 before the add */
        register FVECTOR* pPos asm("$5") = &pos;
        __asm__ volatile("" : : "r"(pPos));
        pos.x = fX + 15.0f;
        m_pSeqCtrlStock_Num01->setPos(*pPos);
    } else {
        if (m_uiUser == 1) {
            pos.x = pLayout->x + 0x1A;
            pos.y = pLayout->y - 0xB + ofsY;
            m_pSeqCtrlStock_Num01->setPos(pos);
        } else {
            pos.x = pLayout->x - 0x1C;
            pos.y = pLayout->y - 0xC;
            m_pSeqCtrlStock_Num01->setPos(pos);
        }
        m_pSeqCtrlStock_Num10->enableDisp(0, FALSE);
    }
    m_uiStockFlag |= 1;
    if (m_iStock > 0) {
        UINT flag = (m_uiUser == 1) ? 2 : 4;
        if (CGameWork::getInt(1, 0xE2) & flag) {
            m_uiStockFlag = (INT)m_uiStockFlag & ~1;
            m_pSeqCtrlStock_Num01->enableDisp(0, FALSE);
            m_pSeqCtrlStock_Num10->enableDisp(0, FALSE);
        }
    }
    category = g_pStockSkillList[m_iStockSkill].CATEGORY;
    switch (category) {
    case 1:
        m_uiStockCategory = 0x1B;
        break;
    case 2:
        m_uiStockCategory = 0x1E;
        break;
    case 3:
    case 5:
        m_uiStockCategory = 0x21;
        break;
    case 7:
        m_uiStockCategory = 0x18;
        break;
    default:
        m_pSeqCtrlStock->enableDisp(FALSE);
        m_uiStockState = 0;
        m_uiStockCategory = 0;
        return;
    }
    m_pSeqCtrlStock->replaceSeq(0, g_szSeqCockpit, m_uiStockCategory);
    m_pSeqCtrlStock->enableDisp(TRUE);
    m_uiStockState = 1;
}
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
            m_cardAdd[count].init(0, 0);
            rollDeck(0, 1, 300);
            return 1;
        }
        card->resetReloadNG();
        card->setCardOwner(1);
        card->updateNumberEffect();
        card->testDisp();
    }
    m_nReturn--;
    m_cardAdd[count].init(0, 0);
    return 1;
}
INT CCardRevolver::canSnatch() {
    if (getHave(1, 1, 0, 0, 0) > 6) return TRUE;
    return FALSE;
}
INT CCardRevolver::setPlayerPronounceCard(INT inTest) {
    CCardBattle* pCB;
    CBattleCard* card;
    CARD* data;
    INT num;
    INT category;
    INT result;
    CARD tmp;
    if (m_iState != 1) return 0;
    if (CGameWork::getFlag(1, 8)) return 0;
    if (CGameWork::getInt(1, 0xE2) & 2) return 0;
    pCB = (CCardBattle*)CTaskMan::getSysTask(0x16);
    if (!pCB->isNowPronounce()) return 0;
    if (!pCB->isNowSetEnemyPronounce()) return 0;
    if (CCardBattle::isExecutingCard(1)) return 0;
    card = getActiveCard();
    if (!card) return 0;
    data = card->getCardData();
    category = data->m_nCategory1;
    switch (category) {
    case 2:
        if (pCB->getEnchantKind() == 0x1F) return 0;
        break;
    case 3:
        if (pCB->getEnchantKind() == 0x19) return 0;
        break;
    case 6:
    case 7:
    case 8:
    case 14:
        return 0;
    }
    if (DebugWork::g_dw.player.iCardNumLock != -1) {
        if (DebugWork::g_dw.player.iCardNumLock < 10) {
            tmp.init(data->m_nKind, DebugWork::g_dw.player.iCardNumLock);
            card->changeData(tmp);
        } else {
            num = CCardBattle::getPlayingCardPoint(0);
            if (num != -1 && num < 10) {
                tmp.init(data->m_nKind, num);
                card->changeData(tmp);
            }
        }
    }
    result = pCB->setPlayCard(m_hOwner, m_uiUser, card, NULL, inTest);
    if (!result) return 0;
    m_nRemain[m_iMode]--;
    m_tqRollSpeed = 300;
    rollDeck(1, 1, 300);
    return result;
}
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
            CARD* pData = card->getCardData(); /* MATCH: one data pointer for the kind and the TRACE number */
            param.PrizeKind = pData->m_nKind;
            param.iNumber = card->getBaseCardNumber();
            FLOAT rx = (-70.0f - RND::genrand_real1() * 10.0f) * 0.017453292f;
            FLOAT ry = RND::genrand_real1() * 360.0f * 0.017453292f;
            param.vRot.set(rx, ry, 0.0f);
            Math3D::LimitPIVector(param.vRot);
            CPrize* pPrize = (CPrize*)CEntityFactory::getInstance()->createPrize(param, 0);
            if (!pPrize) return;
            pPrize->enableReturnCard(TRUE);
            TRACE(D_0047AC08, n, pData->m_nKind, pData->m_nNumber, card->getBaseCardNumber(),
                  card->getStock(), card->getHand(), card->getHandMax(), card->getFlag(), card->getCardOwner());
            card->setCardOwner(2);
            card->resetPlutoON();
            m_nPlutoMark--;
            return;
        }
        if (++n >= m_nReloadID) n = 0;
    }
}
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
CEnchantGauge::~CEnchantGauge() {
    delete[] m_pSeqCtrls;
    m_pSeqCtrls = NULL;
}
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
void CEnchantGauge::main() {
    UINT flag = (m_uiUser == 1) ? 2 : 4;
    INT num;
    switch (m_uiState) {
    case 1:
    case 2:
        if (CGameWork::getInt(1, 0xE2) & flag) {
            m_uiState = 5;
            m_pSeqCtrls[0].replaceSeq(0, _ref(g_szSeqCockpit), 0x38);
            m_pSeqCtrls[0].enableDisp(1, FALSE);
            m_pSeqCtrls[2].enableDisp(0, FALSE);
            m_pSeqCtrls[1].enableDisp(0, FALSE);
        }
        break;
    case 6:
        if (!(CGameWork::getInt(1, 0xE2) & flag)) {
            m_uiState = 1;
            m_pSeqCtrls[0].replaceSeq(0, _ref(g_szSeqCockpit), 0x36);
            m_pSeqCtrls[0].enableDisp(0, TRUE);
            m_pSeqCtrls[0].enableDisp(1, TRUE);
        }
        break;
    }
    switch (m_uiState) {
    case 0:
        break;
    case 1:
        if (m_pSeqCtrls[0].isEnd(0)) {
            m_uiState = 2;
            m_pSeqCtrls[0].replaceSeq(0, _ref(g_szSeqCockpit), 0x37);
            m_pSeqCtrls[2].enableDisp(0, TRUE);
            m_pSeqCtrls[1].enableDisp(0, TRUE);
            CCardBattle::decEnchantTime(m_uiUser, 0, 0);
        }
        break;
    case 2:
        if (m_nStack < m_nNext) changeCount(m_nNext - 1);
        break;
    case 3:
        if (m_pSeqCtrls[2].isEnd(0)) {
            if (!m_nNext) {
                m_nCount = 0;
                m_nStack = 0;
                m_uiState = 4;
                destroy();
                break;
            }
            num = m_nNext / 10;
            if (m_nCount / 10 != num) {
                if (num > 0) m_pSeqCtrls[1].replaceSeq(0, _ref(g_szSeqCockpit), num + 0x2B);
                else m_pSeqCtrls[1].replaceSeq(0, _ref(g_szSeqCockpit), 0x35);
            }
            num = m_nNext % 10;
            m_pSeqCtrls[2].replaceSeq(0, _ref(g_szSeqCockpit), num + 0x2B);
            m_uiState = 2;
            m_nCount = m_nNext;
        }
        break;
    case 4:
        if (m_pSeqCtrls[0].isEnd(0)) {
            m_uiState = 0;
            m_pSeqCtrls[0].enableDisp(0, FALSE);
            out();
        }
        break;
    case 5:
        if (m_pSeqCtrls[0].isEnd(0)) {
            m_uiState = 6;
            m_pSeqCtrls[0].enableDisp(0, FALSE);
        }
        break;
    case 6:
        break;
    }
}
void CEnchantGauge::changeCount(INT inNum) {
    INT num;
    if (m_uiState == 4) return;
    switch (m_uiState) {
    case 1:
    case 3:
    case 5:
    case 6:
        m_nStack = inNum;
        break;
    case 4:
        break;
    default:
        m_nNext = inNum;
        num = m_nCount / 10;
        if (num != inNum / 10) m_pSeqCtrls[1].replaceSeq(0, g_szSeqCockpit, num + 0x6C);
        num = m_nCount % 10;
        m_pSeqCtrls[2].replaceSeq(0, g_szSeqCockpit, num + 0x6C);
        m_uiState = 3;
        break;
    }
}
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
CStockFolder::~CStockFolder() {
    deleteGauge();
}
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
    m_pSeqCtrl = new ("c:/KHCOM/MainApp/Game/CardRevolver.cpp(6211)") D2::C2DSeqCtrl;
    m_pSeqCtrl_Num10 = new ("c:/KHCOM/MainApp/Game/CardRevolver.cpp(6212)") D2::C2DSeqCtrl;
    m_pSeqCtrl_Num01 = new ("c:/KHCOM/MainApp/Game/CardRevolver.cpp(6213)") D2::C2DSeqCtrl;
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
    m_p2DFntStockName = new ("c:/KHCOM/MainApp/Game/CardRevolver.cpp(6245)")
        D2::C2DFnt(0, 0, &rgba, 14, g_szStockSkillName[m_iStockSkill], 0x80);
    inSeq->addFntShift(g_szSeqCockpit, m_p2DFntStockName, -4.0f, 0.0f);
    inSeq->m_seqControl.fgColor = 1;
    inSeq->m_seqControl.fgFntScale = 1;
}
void CStockFolder::exeGauge() {
    switch (m_uiState) {
    case 0:
        if (m_iStock > 0 && (CGameWork::getInt(1, 0xE2) & 4)) {
            m_uiState = 5;
            if (m_uiFlag & 1) {
                m_uiFlag = (INT)m_uiFlag & ~1;
                m_pSeqCtrl_Num01->enableDisp(0, FALSE);
                m_pSeqCtrl_Num10->enableDisp(0, FALSE);
            }
        }
        break;
    case 1:
    case 2:
    case 3:
    case 6:
        if (CGameWork::getInt(1, 0xE2) & 4) {
            m_uiState = 5;
            m_pSeqCtrl->enableDisp(FALSE);
            m_uiCategory = 0;
            if (m_uiFlag & 1) {
                m_uiFlag = (INT)m_uiFlag & ~1;
                m_pSeqCtrl_Num01->enableDisp(0, FALSE);
                m_pSeqCtrl_Num10->enableDisp(0, FALSE);
            }
        }
        break;
    case 4:
        break;
    case 5:
        if (!(CGameWork::getInt(1, 0xE2) & 4)) setStockInfo();
        break;
    }
    if (m_iStock == 0 && (m_uiFlag & 1)) {
        m_uiFlag = (INT)m_uiFlag & ~1;
        m_pSeqCtrl_Num10->enableDisp(0, FALSE);
        m_pSeqCtrl_Num01->enableDisp(0, FALSE);
    }
    switch (m_uiState) {
    case 0:
        break;
    case 1:
        if (m_pSeqCtrl->isEnd(0)) {
            m_uiState = 2;
            m_pSeqCtrl->replaceSeq(0, _ref(g_szSeqCockpit), m_uiCategory + 1);
        }
        break;
    case 2:
        if (m_iStock) break;
        if (!CCardBattle::isExecutingCard(m_uiUser)) {
            m_uiState = 0;
            m_pSeqCtrl->enableDisp(FALSE);
        } else {
            m_uiState = 3;
        }
        break;
    case 3:
        if (!CCardBattle::isExecutingCard(m_uiUser)) {
            m_uiState = 4;
            m_pSeqCtrl->replaceSeq(0, _ref(g_szSeqCockpit), m_uiCategory + 2);
        } else if (((CCardBattle*)CTaskMan::getSysTask(0x16))->isPronounceMode()) {
            m_uiState = 6;
        }
        break;
    case 4:
        if (m_pSeqCtrl->isEnd(0)) {
            m_uiState = 0;
            m_pSeqCtrl->enableDisp(FALSE);
        }
        break;
    case 5:
        break;
    case 6:
        if (((CCardBattle*)CTaskMan::getSysTask(0x16))->isPronounceMode()) {
            if (m_uiFlag & 2) setPlayPronounce();
        } else {
            m_uiState = 4;
            m_pSeqCtrl->replaceSeq(0, _ref(g_szSeqCockpit), m_uiCategory + 2);
            m_uiFlag = (INT)m_uiFlag & ~2;
            returnStockCard();
        }
        break;
    }
}
void CStockFolder::deleteGauge() {
    delete m_pSeqCtrl;
    m_pSeqCtrl = NULL;
    delete m_pSeqCtrl_Num01;
    m_pSeqCtrl_Num01 = NULL;
    delete m_pSeqCtrl_Num10;
    m_pSeqCtrl_Num10 = NULL;
}
void CStockFolder::setStockInfo() {
    CB_LAYOUT* pLayout;
    INT num;
    INT category;
    m_p2DFntStockName->setText(g_szStockSkillName[m_iStockSkill]);
    if (m_uiUser == 2) pLayout = (m_iStockSkill >= 0x6F) ? &g_LayoutData[32 + m_iStock] : &g_LayoutData[29 + m_iStock];
    FVECTOR pos = { 0.0f, 0.0f, 0.0f, 1.0f };
    num = m_iStockSum % 10;
    m_pSeqCtrl_Num01->changeTexture(0, 0, 0, NULL, &g_uvCRCounterNumTex[m_nNumTexID][num], &g_uvCRCounterNumTex[m_nNumTexID + 1][num]);
    m_pSeqCtrl_Num01->changeTexture(0, 1, 0, NULL, &g_uvCRCounterNumTex[m_nNumTexID][num], &g_uvCRCounterNumTex[m_nNumTexID + 1][num]);
    m_pSeqCtrl_Num01->enableDisp(0, TRUE);
    num = m_iStockSum / 10;
    if (num > 0) {
        m_pSeqCtrl_Num10->changeTexture(0, 0, 0, NULL, &g_uvCRCounterNumTex[m_nNumTexID][num], &g_uvCRCounterNumTex[m_nNumTexID + 1][num]);
        m_pSeqCtrl_Num10->changeTexture(0, 1, 0, NULL, &g_uvCRCounterNumTex[m_nNumTexID][num], &g_uvCRCounterNumTex[m_nNumTexID + 1][num]);
        m_pSeqCtrl_Num10->enableDisp(0, TRUE);
        pos.x = pLayout->x - 0x2B;
        pos.y = pLayout->y - 0xC;
        m_pSeqCtrl_Num10->setPos(pos);
        pos.x += 15.0f;
        m_pSeqCtrl_Num01->setPos(pos);
    } else {
        pos.x = pLayout->x - 0x1C;
        pos.y = pLayout->y - 0xC;
        m_pSeqCtrl_Num01->setPos(pos);
        m_pSeqCtrl_Num10->enableDisp(0, FALSE);
    }
    m_uiFlag |= 1;
    category = _ref(g_pStockSkillList)[m_iStockSkill].CATEGORY;
    switch (category) {
    case 1:
        m_uiCategory = 0x1B;
        break;
    case 2:
        m_uiCategory = 0x1E;
        break;
    case 3:
    case 5:
        m_uiCategory = 0x21;
        break;
    case 7:
        m_uiCategory = 0x18;
        break;
    default:
        m_pSeqCtrl->enableDisp(FALSE);
        m_uiState = 0;
        m_uiCategory = 0;
        return;
    }
    m_pSeqCtrl->replaceSeq(0, g_szSeqCockpit, m_uiCategory);
    m_pSeqCtrl->enableDisp(TRUE);
    m_uiState = 1;
}
INT CStockFolder::setStockCard(INT inKind, INT inNumber) {
    if (CGameWork::getFlag(1, 10)) return 0;
    if (m_uiFlag & 2) return 0;
    CCardBattle* pBattle = (CCardBattle*)CTaskMan::getSysTask(0x16);
    if (!pBattle) return 0;
    if (pBattle->isPlayingCard(m_uiUser)) return 0;
    if (m_iStock == 3) return 0;
    m_pCards[m_iStock] = new ("c:/KHCOM/MainApp/Game/CardRevolver.cpp(6495)") CBattleCard(this, inKind, inNumber, m_uiUser, 0);
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
    m_pCards[m_iStock] = new ("c:/KHCOM/MainApp/Game/CardRevolver.cpp(6700)") CBattleCard(this, inKind, inNumber, m_uiUser, 0);
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
// SOURCE DATA: .data .rodata
