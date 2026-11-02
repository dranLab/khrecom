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
union IVECTOR;   // PARTIAL: added for Game/Card

/* PARTIAL: added for Game/Card */
struct CARD_INFO2 {
    USHORT _nKeyTotal;
    char _nKey[4];
};

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
    friend struct MainAppSingletons; // PARTIAL: System/MainApp creates and deletes the instance
    static CCard* m_pInstance;

    CCard();
    ~CCard(); // PARTIAL: added for System/MainApp

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

    // PARTIAL: complete for Game/Card
    static void setCard(unsigned char (*pInitCardTbl)[2], INT cnt);
    static void initCardWorld();
    static INT deleteCard(USHORT inID, SHORT inNumber);
    static INT searchCard(USHORT inID, SHORT inNumber);
    static INT searchCardKind(USHORT inID, SHORT inNumber);
    static INT addDeck(CARD inCard, USHORT inDeckNum);
    static INT insertDeck(CARD inCard, USHORT inDeckNum, SHORT inIdx, BOOL bShortCut);
    static CARD* getCurrentDeck();
    static void packCurrentDeck(USHORT inDeckNum);
    static INT getCurrentShortCut();
    static void packDeck(USHORT inDeckNum);
    static INT removeDeck(USHORT inDeckNum, SHORT inIdx);
    static INT searchDeck(CARD inCard, USHORT inDeckNum, SHORT nFrom);
    static INT makePremiereDeck(USHORT inDeckNum, SHORT inIdx);
    static void clearDeck(USHORT inDeckNum);
    static INT checkCurrentDeck();
    static BOOL isCurrentDeckPremiumFull();
    static INT getCP(CARD card);
    static INT getMP(CARD card);
    static BOOL getAttribute(CARD card, const CHAR** pOutStrAttack, const CHAR** pOutStrAttribute, const CHAR** pOutStrSpeed,
                             const CHAR** pOutHelp);
    static void setShortCut(USHORT inDeckNum, USHORT nIndex);
    static INT getShortCut(USHORT inDeckNum);
    static void clearShortCut(USHORT inDeckNum);
    static UINT getCardMax();
    static UINT getCardMax(USHORT nCategory);
    static UINT getAttackCardMax(USHORT nCategory);
    static void setDeckName(INT inDeckIdx, const CHAR* pDeckName);
    static void getCardTexInfo(CARD inCard, CHAR* outTexName, FVECTOR& outST);
    static void getCardTexInfo(CARD inCard, CHAR* outTexName, IVECTOR& outUV);
    static TexInfo* getCardTexInfo(CARD inCARD, FVECTOR& outST);
    static TexInfo* getCardTexInfo(CARD inCard, IVECTOR& outUV);
    static BOOL addCard(CARD card);
    static BOOL addCard(USHORT inID, SHORT inNumber);
    static BOOL canAddCard(CARD card);
    static BOOL canAddCard(USHORT inID, SHORT inNumber);
    static BOOL isAddCardValid(USHORT inID, SHORT inNumber);
    static BOOL isBattleCardFull();
    static BOOL isMapCardFull();
    static BOOL isKeyCardFull();
    static BOOL shouldDispBattleCardFullMessage();
    static void finishBattleCardFullMessage();
    static void resetBattleCardFullMessage();
    static BOOL shouldDispMapCardFullMessage();
    static void finishMapCardFullMessage();
    static void resetMapCardFullMessage();
    static CARD correctCardRank(CARD card);
    static BOOL isHaveMapCard(INT nRank);
    static BOOL isSoraValid(USHORT inID);
    static BOOL isRikuValid(USHORT inID);
    static BOOL isValid(USHORT inID);
    static BOOL canPremium(USHORT inID);
    static LPCSTR getName(USHORT inID);
    static void initCardAppearance();
    static void setCardPossible(USHORT usKind2);
    static BOOL isCardPossible(USHORT usKind2);
    static void setCardPossibleWorld(INT nWorldID);
    static void setCardNew(USHORT usKind2);
    static BOOL isCardNew(USHORT usKind2);
    static BOOL setCardNewFinish(USHORT usKind2);
    static BOOL isCardAlreadyGet(USHORT usKind2);
    static BOOL isBattleTotalMaxUpCard(USHORT usKind2);
    static BOOL isCardForTotalMaxUp(USHORT usKind2);
    static BOOL tryAddBattleCardAppearance(USHORT usKind2);
    static BOOL tryAddMapCardAppearance(USHORT usKind2);
    static BOOL tryAddKeyCardAppearance(USHORT usKind2);
    static INT getCardAppearance(USHORT usKind2);
    static void setCardAppearance(USHORT usKind2, INT nAppear);
    static CARD_INFO* m_pCardInfo;         // PARTIAL: added for Form/Menu/Roulette (private in the original)
    static CARD_INFO2* m_pCardInfo2;
    static CARD m_deckCurrent[99];
    static void* m_pCardInitRsrc;
    static struct BC_PARAM* m_pBCParam;
#ifdef GAME_SCENEINFO_H /* MATCH: initCard takes WORLD_ID: declared only where SceneInfo.h is included */
    static void initCard(USHORT nDeck, WORLD_ID nWorld, SHORT nFloor); // PARTIAL: added for Form/DebugMenu
#endif
};

#endif /* GAME_CARD_H */
