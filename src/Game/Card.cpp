
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

#line 287

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

#line 322

void CCard::setCard(unsigned char (*pInitCardTbl)[2], INT cnt) {
    INT i;
    for (i = 0; i < cnt; i++) {
        if (pInitCardTbl[i][1] < 10) {
            USHORT idxCard = pInitCardTbl[i][0];
            addCard(idxCard, pInitCardTbl[i][1]);
            if (idxCard >= 0x44 && CGame::isRikuGame()) continue;
            CARD card;
            card.init(idxCard, pInitCardTbl[i][1]);
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
    idxCard = CSceneInfo::getCurrentWorldID(1);
    idx = idxCard + 14;
    if (idxCard == 0) idx = 27;
    else if ((UINT)(idxCard - 1) >= 13) idx = 28;
    idx += 1;
    INT cnt = pIdxTbl[idx];
    if (idx) {
        cnt = cnt - pIdxTbl[idx - 1] - 8;
    } else {
        cnt = cnt - 8;
    }
    pInitCardTbl = (unsigned char (*)[2])((BYTE*)pCIHeader + pCIHeader->dataTbl + pIdxTbl[idx - 1] * 2 + 0x10);
    setCard(pInitCardTbl, cnt);
    for (i = 99; i < 127; i++) {
        if (searchCard(i, 0)) {
            card.init(i, 0);
            addDeck(card, 0);
        }
    }
}
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
            if ((UINT)(inCurrentFloorNumber - 1) >= 13) idx = 13;
        } else {
            idx = inDebugWorldID + 14;
            if (inDebugWorldID == 0) idx = 27;
            else if ((UINT)(inDebugWorldID - 1) >= 13) idx = 28;
        }
        idx += 1;
        cnt = pIdxTbl[idx];
        if (idx) {
            cnt = cnt - pIdxTbl[idx - 1] - 8;
        } else {
            cnt = cnt - 8;
        }
        idxCard = pIdxTbl[idx - 1];
        pInitCardTbl = (unsigned char (*)[2])((BYTE*)pCIHeader + pCIHeader->dataTbl + idxCard * 2);
        setCard(pInitCardTbl + 8, cnt);
        idx = (idx < 15) ? 0 : 15;
        cnt = pIdxTbl[idx];
        if (idx) {
            cnt = cnt - pIdxTbl[idx - 1] - 8;
        } else {
            cnt = cnt - 8;
        }
        idxCard = pIdxTbl[idx - 1];
        pInitCardTbl = (unsigned char (*)[2])((BYTE*)pCIHeader + pCIHeader->dataTbl + idxCard * 2);
        setCard(pInitCardTbl + 8, cnt);
    } else {
        if (inNo == 1) {
            idx = 0;
        } else if (inNo == 2) {
            idx = 27;
        } else {
            return;
        }
        idx += 1;
        cnt = pIdxTbl[idx];
        if (idx) {
            cnt = cnt - pIdxTbl[idx - 1] - 8;
        } else {
            cnt = cnt - 8;
        }
        idxCard = pIdxTbl[idx - 1];
        pInitCardTbl = (unsigned char (*)[2])((BYTE*)pCIHeader + pCIHeader->dataTbl + idxCard * 2);
        setCard(pInitCardTbl + 8, cnt);
    }
}
INT CCard::deleteCard(USHORT inID, SHORT inNumber) {
    CARD card;
    card.init(inID, inNumber);
    USHORT fgPremium = card.m_fgPremium;
    USHORT id;
    switch (card.m_nCategory0) {
    case 0:
        return 0;
    case 1:
        if (inNumber >= 10) return 0;
        if (!m_pCardInfo->_nBattleTotal) return -1;
        id = inID - (fgPremium ? 0x101 : 1);
        if (!m_pCardInfo->_nAttak[fgPremium][id][inNumber]) return -2;
        m_pCardInfo->_nAttak[fgPremium][id][inNumber]--;
        m_pCardInfo->_nBattleTotal--;
        resetBattleCardFullMessage();
        break;
    case 2:
    case 3:
        if (inNumber >= 10) return 0;
        if (!m_pCardInfo->_nBattleTotal) return -1;
        id = inID - (fgPremium ? 0x11A : 0x1A);
        if (!m_pCardInfo->_nMagic[fgPremium][id][inNumber]) return -2;
        m_pCardInfo->_nMagic[fgPremium][id][inNumber]--;
        m_pCardInfo->_nBattleTotal--;
        resetBattleCardFullMessage();
        break;
    case 4:
        if (inNumber >= 10) return 0;
        if (!m_pCardInfo->_nBattleTotal) return -1;
        id = inID - 0x2A;
        if (!m_pCardInfo->_nItem[id][inNumber]) return -2;
        m_pCardInfo->_nItem[inID - 0x2A][inNumber]--;
        m_pCardInfo->_nBattleTotal--;
        resetBattleCardFullMessage();
        break;
    case 7:
        if (inNumber != 0) return 0;
        if (!m_pCardInfo->_nBattleTotal) return -1;
        id = inID - 0x44;
        if (!m_pCardInfo->_nEnemy[id]) return -2;
        m_pCardInfo->_nEnemy[id]--;
        m_pCardInfo->_nBattleTotal--;
        resetBattleCardFullMessage();
        break;
    case 9:
        if (inNumber >= 10) return 0;
        if (!m_pCardInfo->_nMapTotal) return -1;
        id = inID - 0xA1;
        if (!m_pCardInfo->_nMap[id][inNumber]) return -2;
        m_pCardInfo->_nMap[id][inNumber]--;
        m_pCardInfo->_nMapTotal--;
        resetMapCardFullMessage();
        break;
    case 10:
        if (inNumber != 0) return 0;
        if (!m_pCardInfo2->_nKeyTotal) return -1;
        id = inID - 0xBA;
        if (!m_pCardInfo2->_nKey[id]) return -2;
        m_pCardInfo2->_nKey[id]--;
        m_pCardInfo2->_nKeyTotal--;
        break;
    }
    return 1;
}

INT CCard::searchCard(USHORT inID, SHORT inNumber) {
    CARD card;
    card.init(inID, inNumber);
    USHORT fgPremium = card.m_fgPremium;
    USHORT id;
    INT nCard = 0;
    switch (card.m_nCategory0) {
    case 0:
        return 0;
    case 1:
        if (inNumber >= 10) return 0;
        id = inID - (fgPremium ? 0x101 : 0x1);
        nCard = m_pCardInfo->_nAttak[fgPremium][id][inNumber];
        break;
    case 2:
    case 3:
        if (inNumber >= 10) return 0;
        id = inID - (fgPremium ? 0x11a : 0x1a);
        nCard = m_pCardInfo->_nMagic[fgPremium][id][inNumber];
        break;
    case 4:
        if (inNumber >= 10) return 0;
        nCard = m_pCardInfo->_nItem[inID - 0x2A][inNumber];
        break;
    case 7:
        if (inNumber != 0) return 0;
        id = inID - 0x44;
        nCard = m_pCardInfo->_nEnemy[id];
        break;
    case 9:
        if (inNumber >= 10) return 0;
        id = inID - 0xA1;
        nCard = m_pCardInfo->_nMap[id][inNumber];
        break;
    case 10:
        if (inNumber != 0) return 0;
        id = inID - 0xBA;
        nCard = m_pCardInfo2->_nKey[id];
        break;
    }
    return nCard;
}

INT CCard::searchCardKind(USHORT inID, SHORT inNumber) {
    INT total = 0;
    INT i;
    INT nCard;
    if (inNumber < 0) {
        for (i = 0; i < 10; i++) {
            nCard = searchCard(inID, i);
            INT b = nCard > 0; // MATCH: a direct (nCard > 0) * nCard folds to (nCard > -1) (li -1)
            total += b * nCard;
        }
    } else {
        nCard = searchCard(inID, inNumber);
        if (nCard > 0) total = nCard;
    }
    return total;
}

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
    if (idxBlank >= 99) {
#line 661
        ASSERT(inIdx);
        for (idxBlank = inIdx - 1; idxBlank >= 0 && pDeck[idxBlank].m_nId; idxBlank--) {}
        for (i = idxBlank; i < inIdx - 1; i++) {
            pDeck[i] = pDeck[i + 1];
            if (i + 1 == idxShortCut) idxShortCut--;
        }
        pDeck[inIdx - 1] = inCard;
        if (bShortCut) idxShortCut = inIdx - 1;
        if (_ref(m_pCardInfo)->_nDeck[inDeckNum] < 96) {
            for (INT j = 0; j < 2; j++) {
                for (idxBlank = 98 - j; idxBlank >= 0 && pDeck[idxBlank].m_nId; idxBlank--) {}
#line 683
                ASSERT(idxBlank >= 0);
                for (INT i = idxBlank; i < 98; i++) {
                    pDeck[i] = pDeck[i + 1];
                    if (i + 1 == idxShortCut) idxShortCut--;
                }
                pDeck[98].m_nId = 0;
            }
        }
        if (idxShortCut >= 0) setShortCut(inDeckNum, idxShortCut);
    } else {
        if (bShortCut) idxShortCut = inIdx;
        if (idxBlank != inIdx) {
            for (INT i = idxBlank; i >= inIdx + 1; i--) {
                pDeck[i] = pDeck[i - 1];
                if (i - 1 == idxShortCut) idxShortCut = bShortCut ? idxShortCut : i;
            }
        }
        pDeck[inIdx] = inCard;
        if (idxShortCut >= 0) setShortCut(inDeckNum, idxShortCut);
    }
    if (inCard.m_nId) m_pCardInfo->_nDeck[inDeckNum]++;
    return 1;
}

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
BOOL CCard::getAttribute(CARD card, const CHAR** pOutStrAttack, const CHAR** pOutStrAttribute, const CHAR** pOutStrSpeed,
                         const CHAR** pOutHelp) {
    static char buf[16];
    if (card.m_nKind < 0x7F) {
        CHAR* pLF;
        strcpy(buf, CCtdMan::getText(card.m_nKind > 0x19 ? 0x319 : card.m_nKind + 0x300));
        pLF = buf;
        *pOutStrAttack = pLF;
        for (INT i = 0; i < 2; i++) {
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
    INT iIdx = inCard.getTexPos();
    outUV.x = (iIdx & 3) * 64 + 1;
    outUV.y = (iIdx & 0xC) * 16 + 1;
    outUV.z = (iIdx & 3) * 64 + 63;
    outUV.w = (iIdx & 0xC) * 16 + 63;
    sprintf(outTexName, "card:%02d", inCard.getTexNo());
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
    INT iIdx = inCard.getTexPos();
    char buf[16];
    outUV.x = (iIdx & 3) * 64 + 1;
    outUV.y = (iIdx & 0xC) * 16 + 1;
    outUV.z = (iIdx & 3) * 64 + 63;
    outUV.w = (iIdx & 0xC) * 16 + 63;
    sprintf(buf, "card:%02d", inCard.getTexNo());
    return CRsrcDB::getRsrcTex(buf, NAME_CASE_SENSITIVE);
}
BOOL CCard::addCard(CARD card) { return addCard(card.m_nKind2, card.m_nNumber2); }

BOOL CCard::addCard(USHORT inID, SHORT inNumber) {
    if (!canAddCard(inID, inNumber)) return FALSE;
    if (inNumber < 0) inNumber = 0;
    CARD card;
    card.init(inID, inNumber);
    USHORT fgPremium = card.m_fgPremium;
    USHORT id;
    switch (card.m_nCategory0) {
    case 1:
        if (!tryAddBattleCardAppearance(inID)) {
#line 1223
            AssertApp(__FILE__, __LINE__, "0", 0);
            return FALSE;
        }
        id = inID - (fgPremium ? 0x101 : 1);
        m_pCardInfo->_nAttak[fgPremium][id][inNumber]++;
        m_pCardInfo->_nBattleTotal++;
        break;
    case 2:
    case 3:
        if (!tryAddBattleCardAppearance(inID)) {
#line 1236
            AssertApp(__FILE__, __LINE__, "0", 0);
            return FALSE;
        }
        id = inID - (fgPremium ? 0x11A : 0x1A);
        m_pCardInfo->_nMagic[fgPremium][id][inNumber]++;
        m_pCardInfo->_nBattleTotal++;
        break;
    case 4:
        if (!tryAddBattleCardAppearance(inID)) {
#line 1248
            AssertApp(__FILE__, __LINE__, "0", 0);
            return FALSE;
        }
        id = inID - 0x2A;
        m_pCardInfo->_nItem[id][inNumber]++;
        m_pCardInfo->_nBattleTotal++;
        break;
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
        break;
    case 9:
        if (!tryAddMapCardAppearance(inID)) {
#line 1275
            AssertApp(__FILE__, __LINE__, "0", 0);
            return FALSE;
        }
        id = inID - 0xA1;
        m_pCardInfo->_nMap[id][inNumber]++;
        m_pCardInfo->_nMapTotal++;
        break;
    case 10:
        if (!tryAddKeyCardAppearance(inID)) {
#line 1287
            AssertApp(__FILE__, __LINE__, "0", 0);
            return FALSE;
        }
        id = inID - 0xBA;
        m_pCardInfo2->_nKey[id]++;
        m_pCardInfo2->_nKeyTotal++;
        break;
    case 0:
    default:
        return FALSE;
    }
    return TRUE;
}

BOOL CCard::canAddCard(CARD card) { return canAddCard(card.m_nKind2, card.m_nNumber2); }

BOOL CCard::canAddCard(USHORT inID, SHORT inNumber) {
    if (!isAddCardValid(inID, inNumber)) return FALSE;
    if (isBattleTotalMaxUpCard(inID)) return TRUE;
    if (inNumber < 0) inNumber = 0;
    CARD card;
    card.init(inID, inNumber);
    USHORT fgPremium = card.m_fgPremium;
    USHORT id;
    switch (card.m_nCategory0) {
    case 1:
        if (isBattleCardFull()) return FALSE;
        id = inID - (fgPremium ? 0x101 : 1);
        if (m_pCardInfo->_nAttak[fgPremium][id][inNumber] >= 99) return FALSE;
        break;
    case 2:
    case 3:
        if (isBattleCardFull()) return FALSE;
        id = inID - (fgPremium ? 0x11A : 0x1A);
        if (m_pCardInfo->_nMagic[fgPremium][id][inNumber] >= 99) return FALSE;
        break;
    case 4:
        if (isBattleCardFull()) return FALSE;
        id = inID - 0x2A;
        if (m_pCardInfo->_nItem[id][inNumber] >= 99) return FALSE;
        break;
    case 7:
        if (isBattleCardFull()) return FALSE;
        id = inID - 0x44;
        if (m_pCardInfo->_nEnemy[id] >= 99) return FALSE;
        break;
    case 9:
        if (isMapCardFull()) return FALSE;
        id = inID - 0xA1;
        if (m_pCardInfo->_nMap[id][inNumber] >= 9) return FALSE;
        break;
    case 10:
        if (isKeyCardFull()) return FALSE;
        id = inID - 0xBA;
        if (m_pCardInfo2->_nKey[id]) return FALSE;
        break;
    case 0:
    default:
        return FALSE;
    }
    return TRUE;
}

BOOL CCard::isAddCardValid(USHORT inID, SHORT inNumber) {
    if (!isValid(inID)) return FALSE;
    USHORT kind = inID & 0xFF;
    CARD card;
    if (inNumber < 0) {
        card.init(inID, 0);
    } else {
        card.init(inID, inNumber);
    }
    switch (card.m_nCategory0) {
    case 0:
        return FALSE;
    case 1:
        if (inNumber < 0) return FALSE;
        if (inNumber >= 10) return FALSE;
        if (CGame::isSoraGame()) {
            if (kind == 0x12) return FALSE;
        } else {
            if (kind != 0x12) return FALSE;
        }
        break;
    case 2:
    case 3:
        if (inNumber < 0) return FALSE;
        if (inNumber >= 10) return FALSE;
        if ((USHORT)(kind - 0x21) < 2) return FALSE;
        break;
    case 4:
        if (inNumber < 0) return FALSE;
        if (inNumber >= 10) return FALSE;
        break;
    case 7:
        if (CGame::isSoraGame()) {
            if (kind == 0x59) return FALSE;
        }
        break;
    case 9:
        if (inNumber < 0) return FALSE;
        if (inNumber >= 10) return FALSE;
        if (inID == 0xB9) {
            if (inNumber != 0) return FALSE;
        }
        break;
    case 10:
        break;
    default:
        return FALSE;
    }
    return TRUE;
}
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
CARD CCard::correctCardRank(CARD card) {
    INT nKind;
    INT nRank;
    INT i;
    INT nCategory = card.m_nCategory0;
    if (nCategory == 0 || (nCategory >= 5 && nCategory != 9) || canAddCard(card)) return card; /* MATCH: one condition (shared return reloads card) */
    nRank = card.m_nNumber;
    nKind = card.m_nKind2;
    if (nRank != 0) {
        for (i = nRank - 1; i > 0; i--) {
            card.init(nKind, i);
            if (canAddCard(card)) return card;
        }
        for (i = nRank + 1; i < 10; i++) {
            card.init(nKind, i);
            if (canAddCard(card)) return card;
        }
        card.init(nKind, 0);
        if (canAddCard(card)) return card;
    } else {
        for (i = 9; i > 0; i--) {
            card.init(nKind, i);
            if (canAddCard(card)) return card;
        }
    }
    card.init(0, 0);
    return card;
}
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
    card.init(inID, 0);
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
    card.init(usKind2, 0);
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

BOOL CCard::isBattleTotalMaxUpCard(USHORT usKind2) {
    if (!CGame::isSoraGame()) return FALSE;
    if (!isCardForTotalMaxUp(usKind2)) return FALSE;
    USHORT usKind = usKind2 & 0xFF;
    USHORT usKindP = usKind | 0x100;
    if (canPremium(usKind)) {
        if (!isCardAlreadyGet(usKind) && !isCardAlreadyGet(usKindP)) return TRUE;
    } else {
        if (!isCardAlreadyGet(usKind)) return TRUE;
    }
    return FALSE;
}

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
        card.init(usKind2, 0);
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
