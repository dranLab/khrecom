#ifndef GAME_CARD_H_STRUCTS
#define GAME_CARD_H_STRUCTS

#include "khtypes.h"

struct CARD_INFO2 {
    USHORT _nKeyTotal;
    char _nKey[4];
};
#endif /* GAME_CARD_H_STRUCTS */

/* MATCH: with CARD_H_STRUCTS_ONLY defined, only CARD_INFO2: Game/StorageWork.h's MCWork2 embeds it, and this
 * header's strings (CCard's inline allocation) are not in all of its includers' originals */
#if !defined(GAME_CARD_H) && !defined(CARD_H_STRUCTS_ONLY)
#define GAME_CARD_H

#include "System/MainApp.h"
#include "System/HeapMan.h"
#include "System/Debug.h"
#include "Game/CardKind.h"
struct TexInfo;
union FVECTOR;
union IVECTOR;

/* MATCH: operator new for CCard::initialize's allocation, declared only: the includers define it themselves */
inline void* operator new(size_t size, char* msg) throw();

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

class CCard { friend class CCamp; /* MATCH: Form/Menu/Camp reads m_pCardInfo as volatile in initCardKind's draft */
private:
    static CCard* m_pInstance;
    static CARD m_deckCurrent[99];
    static void* m_pCardInitRsrc;
    static struct BC_PARAM* m_pBCParam;
    static CARD_INFO* m_pCardInfo;
    static CARD_INFO2* m_pCardInfo2;

    CCard();
    ~CCard();

public:
#ifndef CARD_H_PATH // MATCH: path override for Game/Status ("c:/KHCOM/MainApp/...")
#define CARD_H_PATH "Game/Card.h"
#endif
#line 64 CARD_H_PATH
    static CCard& initialize() {
        KH_DEBUG(if (!(__null == m_pInstance)) AssertApp(__FILE__, __LINE__, "__null == m_pInstance", 0);)
        m_pInstance = new KH_AT("(66)") CCard;
        return *m_pInstance;
    }
    static void finalize() {
        if (m_pInstance) delete m_pInstance;
        m_pInstance = __null;
    }
#ifdef GAME_SCENEINFO_H /* MATCH: initCard takes WORLD_ID: declared only where SceneInfo.h is included */
    static void initCard(USHORT nDeck, WORLD_ID nWorld, SHORT nFloor);
#endif
    static void initCardWorld();
    static INT deleteCard(USHORT inID, SHORT inNumber);
    static INT searchCard(USHORT inID, SHORT inNumber);
    static INT searchCardKind(USHORT inID, SHORT inNumber);
    static INT addDeck(CARD inCard, USHORT inDeckNum);
    static INT insertDeck(CARD inCard, USHORT inDeckNum, SHORT inIdx, BOOL bShortCut);
    static INT removeDeck(USHORT inDeckNum, SHORT inIdx);
    static INT searchDeck(CARD inCard, USHORT inDeckNum, SHORT nFrom);
    static INT makePremiereDeck(USHORT inDeckNum, SHORT inIdx);
    static void clearDeck(USHORT inDeckNum);
    static CARD* getDeckArray(/* parameters unknown */);
    static CARD* getCurrentDeck();
    static INT getCurrentShortCut();
    /* MATCH: the accessors read m_pCardInfo through a reference: the original reloads it after stores */
    static UINT getCurrentDeckMax() { CARD_INFO*& p = m_pCardInfo; return p->_nDeck[p->_nCurrentDeck]; }
    static UINT getCurrentDeckNo() { CARD_INFO*& p = m_pCardInfo; return p->_nCurrentDeck; }
    static void setCurrentDeckNo(USHORT nDeck) { m_pCardInfo->_nCurrentDeck = nDeck; }
    static CARD* getDeck(USHORT nDeck) { CARD_INFO*& p = m_pCardInfo; return p->_deck[nDeck]; }
    static UINT getDeckMax(USHORT nDeck) { CARD_INFO*& p = m_pCardInfo; return p->_nDeck[nDeck]; }
    static UINT getCardMax();
    static UINT getCardMax(USHORT nCategory);
    static UINT getAttackCardMax(USHORT nCategory);
    static void packCurrentDeck(USHORT inDeckNum);
    static void packDeck(USHORT inDeckNum);
    static INT getCP(CARD card);
    static INT getMP(CARD card);
    static BOOL getAttribute(CARD card, const CHAR** pOutStrAttack, const CHAR** pOutStrAttribute, const CHAR** pOutStrSpeed,
                             const CHAR** pOutHelp);
    static USHORT getBattleTotalMax(/* parameters unknown */);
    static USHORT getMapTotalMax(/* parameters unknown */);
    static USHORT getBattleTotal() { CARD_INFO*& p = m_pCardInfo; return p->_nBattleTotal; }
    static USHORT getMapTotal() { CARD_INFO*& p = m_pCardInfo; return p->_nMapTotal; }
    static LPCTSTR getDeckName(INT nDeck) { CARD_INFO*& p = m_pCardInfo; return p->_deckName[nDeck]; }
    static void setDeckName(INT inDeckIdx, const CHAR* pDeckName);
    static INT checkCurrentDeck();
    static void setShortCut(USHORT inDeckNum, USHORT nIndex);
    static void clearShortCut(USHORT inDeckNum);
    static INT getShortCut(USHORT inDeckNum);
    static BOOL isCurrentDeckPremiumFull();
    static void getCardTexInfo(CARD inCard, CHAR* outTexName, FVECTOR& outST);
    static void getCardTexInfo(CARD inCard, CHAR* outTexName, IVECTOR& outUV);
    static TexInfo* getCardTexInfo(CARD inCARD, FVECTOR& outST);
    static TexInfo* getCardTexInfo(CARD inCard, IVECTOR& outUV);
    static BOOL addCard(CARD card);
    static BOOL addCard(USHORT inID, SHORT inNumber);
    static BOOL canAddCard(CARD card);
    static BOOL canAddCard(USHORT inID, SHORT inNumber);
    static BOOL isBattleCardFull();
    static BOOL isMapCardFull();
    static BOOL isKeyCardFull();
    static BOOL shouldDispBattleCardFullMessage();
    static void finishBattleCardFullMessage();
    static BOOL shouldDispMapCardFullMessage();
    static void finishMapCardFullMessage();
    static CARD correctCardRank(CARD card);
    static BOOL isHaveMapCard(INT nRank);
    static BOOL isSoraValid(USHORT inID);
    static BOOL isRikuValid(USHORT inID);
    static BOOL isValid(USHORT inID);
    static BOOL canPremium(USHORT inID);
    static LPCSTR getName(USHORT inID);
    static void setCardPossible(USHORT usKind2);
    static BOOL isCardPossible(USHORT usKind2);
    static void setCardPossibleWorld(INT nWorldID);
    static BOOL isCardNew(USHORT usKind2);
    static BOOL setCardNewFinish(USHORT usKind2);
    static BOOL isBattleTotalMaxUpCard(USHORT usKind2);

private:
    static void setCard(unsigned char (*pInitCardTbl)[2], INT cnt);
    static BOOL isAddCardValid(USHORT inID, SHORT inNumber);
    static void resetBattleCardFullMessage();
    static void resetMapCardFullMessage();
    static void initCardAppearance();
    static void setCardNew(USHORT usKind2);
    static BOOL isCardAlreadyGet(USHORT usKind2);
    static BOOL isCardForTotalMaxUp(USHORT usKind2);
    static BOOL tryAddBattleCardAppearance(USHORT usKind2);
    static BOOL tryAddMapCardAppearance(USHORT usKind2);
    static BOOL tryAddKeyCardAppearance(USHORT usKind2);
    static INT getCardAppearance(USHORT usKind2);
    static void setCardAppearance(USHORT usKind2, INT nAppear);
};

#endif /* GAME_CARD_H */
