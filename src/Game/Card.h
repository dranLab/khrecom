// PARTIAL: added for Form/Menu/Menu
#ifndef GAME_CARD_H
#define GAME_CARD_H

#include "khtypes.h"
#include "System/MainApp.h"
#include "System/HeapMan.h"
#include "System/Debug.h"
#include "Game/CardKind.h" // PARTIAL: added for Form/Menu/Roulette
struct TexInfo;  // PARTIAL: added for Game/AttachCard
union FVECTOR;   // PARTIAL: added for Game/AttachCard

/* PARTIAL: added for Form/Menu/Roulette */
struct CARD_INFO {
    USHORT _nBattleTotal;          /* 0x000 */
    USHORT _nMapTotal;             /* 0x002 */
    USHORT _nTotal;                /* 0x004 */
    USHORT _nBattleTotalMax;       /* 0x006 */
    USHORT _nMapTotalMax;          /* 0x008 */
    USHORT _nTotalMax;             /* 0x00a */
    USHORT _nFlag;                 /* 0x00c */
    USHORT _dummy4;                /* 0x00e */
    unsigned int _nDeck[3];        /* 0x010 */
    UINT _nCurrentDeck;            /* 0x01c */
    char _nShortCutIndex[4];       /* 0x020 */
    UINT _dummy6;                  /* 0x024 */
    UINT _dummy7;                  /* 0x028 */
    UINT _dummy8;                  /* 0x02c */
    char _nAttak[2][24][10];       /* 0x030 */
    char _nMagic[2][16][10];       /* 0x210 */
    char _nItem[7][10];            /* 0x350 */
    char _nMap[25][10];            /* 0x396 */
    char _nEnemy[59];              /* 0x490 */
    CARD _deck[3][99];             /* 0x4cc */
    char _deckName[3][20];         /* 0x970 */
    char _fgNew[1015];             /* 0x9ac */
};

class CCard {
private:
    static CCard* m_pInstance;

    CCard();

public:
    /* Placeholder for the original singleton creation (CCard::initialize): its strings ("Game/Card.h",
     * "__null == m_pInstance", "Game/Card.h(66)") are in .rodata of every includer. Only line 66 is
     * known (it is part of the allocation message). */
#ifndef CARD_H_PATH // PARTIAL: path override added for Game/Status ("c:/KHCOM/MainApp/...")
#define CARD_H_PATH "Game/Card.h"
#endif
#line 60 CARD_H_PATH
    static void _initializeCheck() {
        if (!(__null == m_pInstance)) AssertApp(__FILE__, __LINE__, "__null == m_pInstance", 0);
        com_malloc_d(4, __FILE__ "(66)", (UINT)-1);
    }

    static INT searchCard(USHORT inID, SHORT inNumber);
    static TexInfo* getCardTexInfo(CARD inCard, CHAR* pTexName, FVECTOR& outST); // PARTIAL: added for Game/AttachCard
    static INT searchCardKind(USHORT inID, SHORT inNumber);
    static BOOL isHaveMapCard(INT inNumber); // PARTIAL: added for Game/Gimmick/GimmickManager
    static CARD correctCardRank(CARD inCard); // PARTIAL: added for Game/Gimmick/GimmickManager
    static BOOL canPremium(USHORT inKind); // PARTIAL: added for Form/Menu/Roulette
    static INT makePremiereDeck(USHORT inDeck, SHORT inIdx); // PARTIAL: added for Form/Menu/Roulette
    static BOOL addCard(USHORT inID, SHORT inNumber);        // PARTIAL: added for Form/Menu/Roulette
    static BOOL deleteCard(USHORT inID, SHORT inNumber);     // PARTIAL: added for Form/Menu/RoomCreation
    static void setDeckName(INT inDeck, const CHAR* inpName); // PARTIAL: added for Form/Menu/DecknameEdit
    static BOOL addCard(CARD inCard);                         // PARTIAL: added for Form/DebugWindowTBL
    static BOOL isMapCardFull();                              // PARTIAL: added for Form/DebugWindowTBL
    static BOOL isCurrentDeckPremiumFull();                   // PARTIAL: added for Game/Enemy/BattleEnemyManager
    static INT removeDeck(USHORT inDeck, SHORT inIdx);       // PARTIAL: added for Form/Menu/Camp
    static BOOL getAttribute(CARD inCard, const CHAR** ppAttack, const CHAR** ppAttribute, const CHAR** ppSpeed, const CHAR** ppHelp); // PARTIAL: added for Form/Menu/Camp
    static INT getCP(CARD inCard);                           // PARTIAL: added for Form/Menu/Camp
    static INT searchDeck(CARD inCard, USHORT inDeck, SHORT inStart); // PARTIAL: added for Form/Menu/Camp
    static INT getShortCut(USHORT inDeck);                   // PARTIAL: added for Form/Menu/Camp
    static void clearShortCut(USHORT inDeck);                // PARTIAL: added for Form/Menu/Camp
    static void setShortCut(USHORT inDeck, USHORT inIdx);    // PARTIAL: added for Form/Menu/Camp
    static void clearDeck(USHORT inDeck);                    // PARTIAL: added for Form/Menu/Camp
    static void packDeck(USHORT inDeck);                     // PARTIAL: added for Form/Menu/Camp
    static CARD_INFO* m_pCardInfo;         // PARTIAL: added for Form/Menu/Roulette (private in the original)
#ifdef GAME_SCENEINFO_H
    static void initCard(USHORT nDeck, WORLD_ID nWorld, SHORT nFloor); // PARTIAL: added for Form/DebugMenu
#endif
};

#endif /* GAME_CARD_H */
