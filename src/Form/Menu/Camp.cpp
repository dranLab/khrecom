#include "System/Task.h"
#include "System/TaskMan.h"
#include "System/Graphic/Gprim.h"
#include "System/Ctd/CtdMan.h"
#include "Game/Card.h"
#include "System/Graphic/2d/2DMan.h"
#include "Game/Status.h"
#include "Game/Technic.h"
#include "System/Graphic/Anim.h"
#include "Game/CardBattle.h"
#include "System/Graphic/Str.h"
#include "Game/Collision/CollUtil.h"

/* MATCH: Placeholder: an empty string follows CollUtil.h's strings in .rodata (as in Roulette); which header it comes
 * from is not known. */
inline void _emptyStringPlaceholder() {
    TRACE("");
}

#include "Game/CardKind.h"
#include "Form/Menu/Camp.h"
#include "Form/Menu/MenuParts.h"
#include "System/HeapMan.h"
#include "System/Vector.h"
#include "System/Graphic/2d/2DSeqCtrl.h"
#include "System/Graphic/2d/2DSeqObj.h"
#include "System/Graphic/2d/2DFnt.h"
#include "System/Graphic/2d/2DCard.h"
#include "Game/SceneInfo.h"
#include "Game/SceneEnv.h"
#include "Game/Game.h"
#include "Game/GameWork.h"
#include "Game/Player/PlayerManager.h"
#include "System/Graphic/StrPrim.h"
#include "Form/Menu/TechHelp.h"
#include "System/Sound/SoundMan.h"
#include "Form/Menu/MenuModel.h"
#include "System/Wnd.h"
#include "System/Rcf.h"
#include "Form/Menu/WorldWarp.h"
#include "Form/Menu/DecknameEdit.h"
#include "Form/Menu/JMemoFlag.h"
#include "include_asm.h"


#line 274




#define ASSERT_AT(line, e) KH_DEBUG(if (!(e)) AssertApp(__FILE__, line, #e, 0))



































class CRsrcData;
struct TexInfo;
#include "System/RsrcDB.h"












#include "Game/Player/Player.h"

/* SceneInfo.h's inline */
static inline RoomInfo* _getCurrentRoomInfo() { return CSceneInfo::getRoomInfo(-1); }

#include "System/Graphic/PackMan.h"




extern "C" {
int sprintf(char*, const char*, ...);
float powf(float, float);
size_t strlen(const char*);
char* strchr(const char*, int);
char* strcat(char*, const char*);
}
void AddVectorXYZ(FVECTOR& v0, const FVECTOR& v1, const FVECTOR& v2);

class CFntLF : public CFntM2 {
public:
    LPTSTR text() { return (LPTSTR)m_pText; }
};

using namespace D2;

template <class T> static inline T& _ref(T& v) { return v; }

/* A copy of CARD::init (CardKind.h 553-558) kept for the parked setMapCardWin draft below (#if 0); the
   matched code calls CARD::init. */
static inline void _setCardNK(CARD& c, USHORT kind, BYTE num) {
    c.m_nId = 0;
    c.m_nNumber2 = num;
    c.m_nKind2 = kind;
    c.m_nCategory20 = cardDataTbl[c.m_nKind].nCategory0;
    c.m_nCategory21 = cardDataTbl[c.m_nKind].nCategory1;
}
#line 381
INT CCamp::m_iWorldRsrc = 0;
CCamp* CCamp::m_pInstance = NULL;

char* CCamp::m_pMainCmdName[2][2][7] = {
    {
        { "Review Decks", "World Map", "Map Cards", "World Cards", "Status", "Journal", "Settings" },
        { "Edit Deck", "Equip Deck", "Shortcuts", "Clear Deck", "Rename Deck", "Delete Cards", "" },
    },
    {
        { "Review Decks", "World Map", "Map Cards", "World Cards", "Status", "D Report", "Settings" },
        { "View Decks", "Shortcuts", "", "", "", "", "" },
    },
};

const unsigned char CCamp::m_itemAWBTbl[7] = { 101, 102, 100, 99, 98, 103, 97 };

CCamp::CCamp(CTask* p, BOOL bWorldmapMode) : CMenu(p, 0) {
    KH_DEBUG(if (!(!_ref(m_pInstance))) AssertApp(__FILE__, 88, "!m_pInstance", 0);)
    m_pInstance = this;
    m_addrRsrcBuf = (BYTE*)PLM::getBattleRsrcFirst();
    m_bWorldmap = bWorldmapMode;
    m_pLadName = "camp.lad";
    m_pSeqName = "camp:00";
    m_bPlayerKind = CGame::isRikuGame();
    m_bPooWorld = CSceneInfo::getCurrentWorldID() == 10;
    m_pZFill = new KH_AT("(100)") CZFill;
    m_pZFill->setLayerFG(11);
    m_pTextWnd = NULL;
    m_pBaseCtrl = NULL;
    m_pWindow0Ctrl = NULL;
    m_pWindow1Ctrl = NULL;
    for (INT i = 0; i < 3; i++) {
        m_pWindow1Card[i] = NULL;
    }
    m_pCursor4HistCtrl = NULL;
    m_pCursor4Ctrl = NULL;
    m_pCursor3Ctrl = NULL;
    m_pCursor2Ctrl = NULL;
    m_pCursorCtrl = NULL;
    m_pCursor5Ctrl = NULL;
    m_pLightCtrl = NULL;
    for (INT i = 0; i < 7; i++) {
        m_pMainCmdCtrl[i] = NULL;
    }
    for (INT i = 0; i < 2; i++) {
        m_pMainCmdHistCtrl[i] = NULL;
        m_pButtonHelpCtrl[i] = NULL;
        m_pMCHistLightCtrl[i] = NULL;
    }
    for (INT i = 0; i < 3; i++) {
        m_pDeckDeckCtrl[i] = NULL;
    }
    for (INT i = 0; i < 3; i++) {
        m_pDeckCardCtrl[i] = NULL;
    }
    m_pDeckInfoCtrl = NULL;
    for (INT i = 0; i < 9; i++) {
        m_pDeckCardCard[i] = NULL;
        m_pDeckDeckCard[i] = NULL;
        m_pMCCard[i] = NULL;
    }
    m_pTechWinScrollBar = NULL;
    m_pDeckDeckScrollBar = NULL;
    m_pDeckCardScrollBar = NULL;
    m_pDeckDeckCardSC = NULL;
    m_pDeckInfoCtrl = NULL;
    m_pDeckInfoCard = NULL;
    for (INT i = 0; i < 10; i++) {
        m_pDeckInfoNumCtrl[i] = NULL;
        m_pMCInfoNumCtrl[i] = NULL;
    }
    m_pDeckInfoIOCtrl = NULL;
    m_pDeckOpeCtrl = NULL;
    m_pWMNormaCtrl = NULL;
    for (INT i = 0; i < 32; i++) {
        m_pRoomCtrl[i] = NULL;
        m_pWayCtrl[i] = NULL;
        m_pShopAndSaveCtrl[i] = NULL;
        m_pShopAndSave[i] = NULL;
    }
    m_pMapGuideCtrl = NULL;
    m_pMapCursorCtrl = NULL;
    for (INT i = 0; i < 4; i++) {
        m_pRoomNormaCtrl[i] = NULL;
    }
    for (INT i = 0; i < 4; i++) {
        m_pRoomNorma[i] = NULL;
    }
    m_pHelpFnt = NULL;
    m_pWMExplaneCtrl = NULL;
    m_pWMRoomNameCtrl = NULL;
    m_pWMFloorCtrl = NULL;
    m_pWMCardCtrl = NULL;
    m_pWMMoveCtrl = NULL;
    m_pWMNormaArrowCtrl = NULL;
    m_pCurrentRoomNameCtrl = NULL;
    m_pTitleCtrl = NULL;
    m_pCurrentRoomPosCtrl = NULL;
    m_pPointCtrl = NULL;
    m_pTimeCtrl = NULL;
    m_pTechWinCtrl = NULL;
    for (INT i = 0; i < 9; i++) {
        m_pTechButton[i] = NULL;
    }
    m_pDeckSetWinCtrl = NULL;
    for (INT i = 0; i < 6; i++) {
        m_pDeckSetButton[i] = NULL;
    }
    m_pDeckCheckErrorButton = NULL;
    m_pYNLightCtrl = NULL;
    m_pYNCursorCtrl = NULL;
    for (INT i = 0; i < 4; i++) {
        m_pWin1EtcButton[i] = NULL;
    }
    m_pButtonHelpSubButton = NULL;
    m_pConfigWinCtrl = NULL;
    for (INT i = 0; i < 21; i++) {
        m_pConfigButton[i] = NULL;
    }
    m_pConfigBC = NULL;
    m_pStatusWinCtrl = NULL;
    m_pStatusTechWinCtrl[1] = NULL;
    m_pStatusTechWinCtrl[0] = NULL;
    for (INT i = 0; i < 9; i++) {
        m_pStatusButton[i] = NULL;
    }
    for (INT i = 0; i < 9; i++) {
        m_pStatusTechButton[i] = NULL;
    }
    m_pStatusBC = NULL;
    m_pStatusTechScrollBar = NULL;
    m_pMCInfoWinCtrl = NULL;
    for (INT i = 0; i < 3; i++) {
        m_pMCSelWinCtrl[i] = NULL;
    }
    m_pMCSelScrollBar = NULL;
    m_pMCInfoCard = NULL;
    for (INT i = 0; i < 2; i++) {
        m_pButtonYenNo[i] = NULL;
    }
    m_pMenuModel = NULL;
    m_pDeckSetBC = NULL;
    m_pTechWinBC = NULL;
    m_nMainMode = 0;
    m_nMainCmdSel = 0;
    m_fgLightMove = 0;
    m_fgCursor4Move = 0;
    m_fgCursor3Move = 0;
    m_fgCursor2Move = 0;
    m_nKindSelectTop = 0;
    m_nKindSelect = 0;
    m_nDeckSelectTop = 0;
    m_nDeckSelect = 0;
    m_nNumSelect = 0;
    m_nCardInOut = 0;
    m_nDeckNoSelect = CCard::getCurrentDeckNo();
    m_nButtonHelpRun[1] = 999;
    m_nButtonHelpRun[0] = 999;
    m_nMainCmdHistRun[1] = 999;
    m_nCurrentCmdMax = 7;
    m_nMainCmdHistRun[0] = 999;
    m_fgTimeDisp = 0;
    m_nDeckCategorySelect = 0;
    m_nKindCategorySelect = 0;
    m_bCardPermuteMode = 0;
    m_bCategorySelectMode = 0;
    m_nTechSelectTop = 0;
    m_nTechSelect = 0;
    m_nDeckSetSelect = CCard::getCurrentDeckNo();
    m_bShortCutMode = 0;
    m_nConfigSelect = 0;
    m_nStatusMax = 0;
    m_nStatusTechSelectMax = 0;
    m_nStatusTechSelectTop = 0;
    m_nStatusTechSelect = 0;
    m_nStatusTechCateSelect = 0;
    m_nErrorWinStep = -1;
    m_nTechWinStep = -1;
    for (INT i = 0; i < 110; i++) {
        m_fgNewTbl[i] = CTechnic::isNew(i);
    }
    for (INT i = 0; i < 7; i++) {
        m_nConfigItemSelect[i] = CGameWork::getByte(0, m_itemAWBTbl[i]);
    }
    CSceneEnv::setStop((ObjType::eObjBit)0xFFFEFFFF);
    CGameWork::setByte(1, 0x140, 1);
}

CCamp::~CCamp() {
    INT i;

    m_pInstance = NULL;
    dispose();
    for (i = 0; i < 110; i++) {
        if (m_fgNewTbl[i] < 0) {
            CTechnic::setNewFinish(i);
        }
    }
    for (i = 0; i < 7; i++) {
        CGameWork::setByte(0, m_itemAWBTbl[i], m_nConfigItemSelect[i]);
    }
    CGame::setupConfig();
    CSceneEnv::setStop((ObjType::eObjBit)0);
    C2DMan::changeLevel(C2DMan::D2MAN_LEVEL0);
    CGameWork::setByte(1, 0x140, 0);
    CGameWork::setFlag(1, 0x1E, 0);
    CPackMan::changeDrawMode(0);
}

#define SAFE_DELETE(p) \
    delete p;          \
    p = NULL

void CCamp::dispose() {
    SAFE_DELETE(m_pZFill);
    SAFE_DELETE(m_pTextWnd);
    SAFE_DELETE(m_pBaseCtrl);
    SAFE_DELETE(m_pTitleCtrl);
    SAFE_DELETE(m_pCurrentRoomNameCtrl);
    SAFE_DELETE(m_pWindow0Ctrl);
    SAFE_DELETE(m_pWindow1Ctrl);
    for (INT i = 0; i < 3; i++) {
        SAFE_DELETE(m_pWindow1Card[i]);
    }
    SAFE_DELETE(m_pCursorCtrl);
    SAFE_DELETE(m_pCursor2Ctrl);
    SAFE_DELETE(m_pCursor3Ctrl);
    SAFE_DELETE(m_pCursor4Ctrl);
    SAFE_DELETE(m_pCursor4HistCtrl);
    SAFE_DELETE(m_pLightCtrl);
    SAFE_DELETE(m_pCursor5Ctrl);
    for (INT i = 0; i < 7; i++) {
        SAFE_DELETE(m_pMainCmdCtrl[i]);
    }
    for (INT i = 0; i < 2; i++) {
        SAFE_DELETE(m_pMainCmdHistCtrl[i]);
        SAFE_DELETE(m_pButtonHelpCtrl[i]);
        SAFE_DELETE(m_pMCHistLightCtrl[i]);
    }
    for (INT i = 0; i < 3; i++) {
        SAFE_DELETE(m_pDeckDeckCtrl[i]);
    }
    for (INT i = 0; i < 3; i++) {
        SAFE_DELETE(m_pDeckCardCtrl[i]);
    }
    SAFE_DELETE(m_pDeckInfoCtrl);
    for (INT i = 0; i < 9; i++) {
        SAFE_DELETE(m_pDeckCardCard[i]);
        SAFE_DELETE(m_pDeckDeckCard[i]);
        SAFE_DELETE(m_pMCCard[i]);
    }
    SAFE_DELETE(m_pDeckCardScrollBar);
    SAFE_DELETE(m_pDeckDeckScrollBar);
    SAFE_DELETE(m_pTechWinScrollBar);
    SAFE_DELETE(m_pDeckDeckCardSC);
    SAFE_DELETE(m_pDeckInfoCtrl);
    SAFE_DELETE(m_pDeckInfoCard);
    for (INT i = 0; i < 10; i++) {
        SAFE_DELETE(m_pDeckInfoNumCtrl[i]);
        SAFE_DELETE(m_pMCInfoNumCtrl[i]);
    }
    SAFE_DELETE(m_pDeckInfoIOCtrl);
    SAFE_DELETE(m_pDeckOpeCtrl);
    SAFE_DELETE(m_pWMNormaCtrl);
    for (INT i = 0; i < 32; i++) {
        SAFE_DELETE(m_pRoomCtrl[i]);
        SAFE_DELETE(m_pWayCtrl[i]);
        SAFE_DELETE(m_pShopAndSaveCtrl[i]);
        SAFE_DELETE(m_pShopAndSave[i]);
    }
    SAFE_DELETE(m_pMapCursorCtrl);
    SAFE_DELETE(m_pMapGuideCtrl);
    for (INT i = 0; i < 4; i++) {
        SAFE_DELETE(m_pRoomNormaCtrl[i]);
    }
    for (INT i = 0; i < 4; i++) {
        SAFE_DELETE(m_pRoomNorma[i]);
    }
    SAFE_DELETE(m_pHelpFnt);
    SAFE_DELETE(m_pWMCardCtrl);
    SAFE_DELETE(m_pWMFloorCtrl);
    SAFE_DELETE(m_pWMRoomNameCtrl);
    SAFE_DELETE(m_pWMMoveCtrl);
    SAFE_DELETE(m_pWMNormaArrowCtrl);
    SAFE_DELETE(m_pWMExplaneCtrl);
    SAFE_DELETE(m_pCurrentRoomPosCtrl);
    SAFE_DELETE(m_pTimeCtrl);
    SAFE_DELETE(m_pPointCtrl);
    SAFE_DELETE(m_pTechWinCtrl);
    for (INT i = 0; i < 9; i++) {
        SAFE_DELETE(m_pTechButton[i]);
    }
    SAFE_DELETE(m_pDeckSetWinCtrl);
    for (INT i = 0; i < 6; i++) {
        SAFE_DELETE(m_pDeckSetButton[i]);
    }
    SAFE_DELETE(m_pDeckCheckErrorButton);
    SAFE_DELETE(m_pYNCursorCtrl);
    SAFE_DELETE(m_pYNLightCtrl);
    for (INT i = 0; i < 4; i++) {
        SAFE_DELETE(m_pWin1EtcButton[i]);
    }
    SAFE_DELETE(m_pButtonHelpSubButton);
    SAFE_DELETE(m_pConfigWinCtrl);
    for (INT i = 0; i < 21; i++) {
        SAFE_DELETE(m_pConfigButton[i]);
    }
    SAFE_DELETE(m_pConfigBC);
    SAFE_DELETE(m_pStatusWinCtrl);
    SAFE_DELETE(m_pStatusTechWinCtrl[0]);
    SAFE_DELETE(m_pStatusTechWinCtrl[1]);
    for (INT i = 0; i < 9; i++) {
        SAFE_DELETE(m_pStatusButton[i]);
    }
    for (INT i = 0; i < 8; i++) {
        SAFE_DELETE(m_pStatusTechButton[i]);
    }
    SAFE_DELETE(m_pStatusBC);
    SAFE_DELETE(m_pStatusTechScrollBar);
    SAFE_DELETE(m_pMCInfoWinCtrl);
    for (INT i = 0; i < 3; i++) {
        SAFE_DELETE(m_pMCSelWinCtrl[i]);
    }
    SAFE_DELETE(m_pMCSelScrollBar);
    SAFE_DELETE(m_pMCInfoCard);
    for (INT i = 0; i < 2; i++) {
        SAFE_DELETE(m_pButtonYenNo[i]);
    }
    SAFE_DELETE(m_pMenuModel);
    SAFE_DELETE(m_pDeckSetBC);
    SAFE_DELETE(m_pTechWinBC);
}

void CCamp::removeRsrc() {
    CRsrcDB::removeGroup(2, 100);
    if (m_iWorldRsrc) {
        CRsrcDB::removeGroup(2, m_iWorldRsrc + 0x95);
        m_iWorldRsrc = 0;
    }
    if (m_pInstance) {
        m_pInstance->out();
    }
}

void CCamp::initCardKind(INT nInDeck, INT nInCategory, BOOL bCardDelete) {
    CARD* pDeck;

    for (INT i = 0; i < 5; i++) {
        m_nKind[i] = 0;
        m_nKindTotal[i] = 0;
    }
    m_nCard = 0;
    pDeck = CCard::getDeck(nInDeck);
    for (INT i = 0; i < m_nCardKindMax; i++) {
        for (INT j = 0; j < 10; j++) {
            m_nKind10Tbl[i][j] = m_nCardKind10[i][j];
            m_nCard += m_nKind10Tbl[i][j];
        }
    }
    if (!bCardDelete) {
        for (INT i = 0; i < 99; i++) {
            CARD card = pDeck[i];
            if (card.m_nId) {
                INT idx;
                for (idx = 0; idx < m_nCardKindMax; idx++) {
                    if (m_nCardKind[idx] == card.m_nKind2) {
                        break;
                    }
                }
                m_nKind10Tbl[idx][card.m_nNumber]--;
            }
        }
    } else {
        CARD cardTmp[3][99];
        CARD deckTmp[297];
        CARD* pCardTmp;
        INT nDeckTmpMax;
        for (INT i = 0; i < 297; i++) {
            deckTmp[i].m_nId = 0;
        }
        for (INT i = 0; i < 3; i++) {
            pCardTmp = (*(CARD_INFO* volatile*)&CCard::m_pCardInfo)->_deck[(USHORT)i]; /* MATCH: reloaded per iteration, getDeck's load hoists */
            CARD* pDst = cardTmp[i];
            for (INT j = 0; j < 99; j++) {
                pDst[j] = pCardTmp[j];
            }
        }
        nDeckTmpMax = 0;
        for (INT i = 0; i < 3; i++) {
            pCardTmp = cardTmp[i];
            for (INT j = 0; j < 99; j++) {
                CARD card = pCardTmp[j];
                if (card.m_nId) {
                    deckTmp[nDeckTmpMax++] = card;
                    for (INT k = 0; k < 3; k++) {
                        CARD* pCardTmp2 = cardTmp[k];
                        for (INT l = 0; l < 99; l++) {
                            if (pCardTmp2[l].m_nId == card.m_nId) {
                                pCardTmp2[l].m_nId = 0;
                                break;
                            }
                        }
                    }
                }
            }
        }
        for (INT i = 0; i < nDeckTmpMax; i++) {
            CARD card = deckTmp[i];
            if (card.m_nId) {
                INT idx;
                for (idx = 0; idx < m_nCardKindMax; idx++) {
                    if (m_nCardKind[idx] == card.m_nKind2) {
                        break;
                    }
                }
                m_nKind10Tbl[idx][card.m_nNumber]--;
            }
        }
    }
    for (INT i = 0; i < m_nCardKindMax; i++) {
        INT cnt = 0;
        for (INT j = 0; j < 10; j++) {
            cnt += m_nKind10Tbl[i][j];
        }
        if (cnt) {
            USHORT cardKind = m_nCardKind[i];
            if (bCardDelete && cardKind >= 0x63 && cardKind < 0x7F) {
                continue;
            }
            if (nInCategory == 0) {
                m_kindTbl[m_nKind[0]] = cardKind;
            }
            m_nKind[0]++;
            m_nKindTotal[0] += cnt;
            if ((cardKind >= 0x1 && cardKind < 0x19) || (cardKind >= 0x101 && cardKind < 0x119)) {
                if (nInCategory == 1) {
                    m_kindTbl[m_nKind[1]] = cardKind;
                }
                m_nKind[1]++;
                m_nKindTotal[1] += cnt;
            } else if ((cardKind >= 0x1A && cardKind < 0x2A) || (cardKind >= 0x11A && cardKind < 0x12A)) {
                if (nInCategory == 2) {
                    m_kindTbl[m_nKind[2]] = cardKind;
                }
                m_nKind[2]++;
                m_nKindTotal[2] += cnt;
            } else if (cardKind >= 0x2A && cardKind < 0x31) {
                if (nInCategory == 3) {
                    m_kindTbl[m_nKind[3]] = cardKind;
                }
                m_nKind[3]++;
                m_nKindTotal[3] += cnt;
            } else if (cardKind >= 0x44 && cardKind < 0x7F) {
                if (nInCategory == 4) {
                    m_kindTbl[m_nKind[4]] = cardKind;
                }
                m_nKind[4]++;
                m_nKindTotal[4] += cnt;
            }
        }
    }
}

void CCamp::initDeck(INT nInDeck, INT nInCategory) {
    INT nSCIndex;
    INT idx;

    m_pDeckSrc = CCard::getDeck(nInDeck);
    m_nDeck[0] = CCard::getDeckMax(nInDeck);
    m_nDeck[4] = 0;
    m_nDeck[3] = 0;
    m_nDeck[2] = 0;
    m_nDeck[1] = 0;
    nSCIndex = CCard::getShortCut(nInDeck);
    m_nSCIndex[0] = nSCIndex;
    m_nSCIndex[4] = -1;
    m_nSCIndex[3] = -1;
    m_nSCIndex[2] = -1;
    m_nSCIndex[1] = -1;
    for (INT i = 0; i < 99; i++) {
        m_deck[i].m_nId = 0;
    }
    if (!nInCategory) {
        m_pDeck = m_pDeckSrc;
    } else {
        m_pDeck = m_deck;
    }
    idx = 0;
    for (INT i = 0; i < 99; i++) {
        CARD& card = m_pDeckSrc[i];
        USHORT cardKind = card.m_nKind2;
        if ((cardKind >= 0x1 && cardKind < 0x19) || (cardKind >= 0x101 && cardKind < 0x119)) {
            if (nInCategory == 1) {
                m_deck[idx] = card;
                if (i == nSCIndex) {
                    m_nSCIndex[1] = idx;
                }
                idx++;
            }
            m_nDeck[1]++;
        } else if ((cardKind >= 0x1A && cardKind < 0x2A) || (cardKind >= 0x11A && cardKind < 0x12A)) {
            if (nInCategory == 2) {
                m_deck[idx] = card;
                if (i == nSCIndex) {
                    m_nSCIndex[2] = idx;
                }
                idx++;
            }
            m_nDeck[2]++;
        } else if (cardKind >= 0x2A && cardKind < 0x31) {
            if (nInCategory == 3) {
                m_deck[idx] = card;
                if (i == nSCIndex) {
                    m_nSCIndex[3] = idx;
                }
                idx++;
            }
            m_nDeck[3]++;
        } else if (cardKind >= 0x44 && cardKind < 0x7F) {
            if (nInCategory == 4) {
                m_deck[idx] = card;
                if (i == nSCIndex) {
                    m_nSCIndex[4] = idx;
                }
                idx++;
            }
            m_nDeck[4]++;
        }
    }
}

void CCamp::initCardMapKind(INT nInCategory) {
    for (INT i = 0; i < 5; i++) {
        m_nMapKind[i] = 0;
        m_nMapKindTotal[i] = 0;
    }
    for (INT i = 0; i < 29; i++) {
        m_nMapKindTbl[i] = 0;
    }
    for (INT i = 0; i < m_nCardKindMax; i++) {
        USHORT cardKind = m_nCardKind[i];
        USHORT cardTotal = CCard::searchCardKind(cardKind, -1);
        if (nInCategory == 0) {
            m_nMapKindTbl[m_nMapKind[0]] = cardKind;
        }
        m_nMapKind[0]++;
        if (cardKind < 0xBA) {
            m_nMapKindTotal[0] += cardTotal;
        }
        if (cardKind >= 0xA1 && cardKind < 0xAC) {
            if (nInCategory == 1) {
                m_nMapKindTbl[m_nMapKind[1]] = cardKind;
            }
            m_nMapKind[1]++;
            m_nMapKindTotal[1] += cardTotal;
        } else if (cardKind >= 0xAC && cardKind < 0xB3) {
            if (nInCategory == 2) {
                m_nMapKindTbl[m_nMapKind[2]] = cardKind;
            }
            m_nMapKind[2]++;
            m_nMapKindTotal[2] += cardTotal;
        } else if (cardKind >= 0xB3 && cardKind < 0xBA) {
            if (nInCategory == 3) {
                m_nMapKindTbl[m_nMapKind[3]] = cardKind;
            }
            m_nMapKind[3]++;
            m_nMapKindTotal[3] += cardTotal;
        } else if (cardKind >= 0xBA && cardKind < 0xBE) {
            if (nInCategory == 4) {
                m_nMapKindTbl[m_nMapKind[4]] = cardKind;
            }
            m_nMapKind[4]++;
            m_nMapKindTotal[4] += cardTotal;
        }
    }
}

void CCamp::adjustSelect(BOOL bMapCard) {
    if (!bMapCard) {
        INT nTop = m_nKindSelectTop;
        if (m_nKindSelect >= m_nKind[m_nKindCategorySelect]) {
            m_nKindSelect--;
            if (m_nKind[m_nKindCategorySelect] >= 9 && m_nKind[m_nKindCategorySelect] < nTop + 7) {
                m_nKindSelectTop -= 3;
                m_nKindSelect -= 2;
            }
        } else if (m_nKind[m_nKindCategorySelect] - nTop < 7 && m_nKind[m_nKindCategorySelect] >= 9) {
            m_nKindSelectTop -= 3;
            m_nKindSelect -= 3;
        }
        if (m_nKindSelectTop < 0) {
            m_nKindSelectTop = 0;
        }
        if (m_nKindSelect < 0) {
            m_nKindSelect = 0;
        }
        if (m_nDeckSelect >= 99) {
            if (--m_nDeckSelect < 0) {
                m_nDeckSelect = 0;
            }
        }
        if (m_nKindSelectTop > m_nKindSelect) {
            m_nKindSelectTop = m_nKindSelect / 3 * 3;
        } else if (m_nKindSelect - m_nKindSelectTop >= 9) {
            m_nKindSelectTop = (m_nKindSelect - 6) / 3 * 3;
        }
        if (m_nDeckSelectTop > m_nDeckSelect) {
            m_nDeckSelectTop = m_nDeckSelect / 3 * 3;
        } else if (m_nDeckSelect - m_nDeckSelectTop >= 9) {
            m_nDeckSelectTop = (m_nDeckSelect - 6) / 3 * 3;
        }
    } else {
        if (m_nMCSelSelect >= m_nMapKind[m_nMCSelCateSelect]) {
            m_nMCSelSelect--;
            if (m_nMapKind[m_nMCSelCateSelect] >= 9 && m_nMapKind[m_nMCSelCateSelect] < m_nMCSelSelectTop + 7) {
                m_nMCSelSelectTop -= 3;
                m_nMCSelSelect -= 2;
            }
        } else if (m_nMapKind[m_nMCSelCateSelect] - m_nMCSelSelectTop < 7 && m_nMapKind[m_nMCSelCateSelect] >= 9) {
            m_nMCSelSelectTop -= 3;
            m_nMCSelSelect -= 3;
        }
        if (m_nMCSelSelectTop < 0) {
            m_nMCSelSelectTop = 0;
        }
        if (m_nMCSelSelect < 0) {
            m_nMCSelSelect = 0;
        }
        if (m_nMCSelSelectTop > m_nMCSelSelect) {
            m_nMCSelSelectTop = m_nMCSelSelect / 3 * 3;
        } else if (m_nMCSelSelect - m_nMCSelSelectTop >= 9) {
            m_nMCSelSelectTop = (m_nMCSelSelect - 6) / 3 * 3;
        }
    }
}

void CCamp::main() {
    static LPCTSTR pWTSeqName = "world_title:00";
    FVECTOR vec;
    FVECTOR vec2;
    G_RGBA rgba;
    INT iWorldRsrc;
    unsigned char deckCheckStepTbl[2][6] = { { 0x24, 0x29, 0x2D, 0x31, 0x35, 0x39 }, { 0x24, 0x2D, 0x2D, 0x2D, 0x2D, 0x2D } };
    INT nError;

    m_iFrame60 = CMainApp::getTickCount() % 7000;
    switch (m_nStep) {
    case 0:
        D2::C2DMan::changeLevel(D2::C2DMan::D2MAN_LEVEL1);
        CPackMan::changeDrawMode(1);
        initCardKind(m_nDeckNoSelect, 0, FALSE);
        initDeck(m_nDeckNoSelect, 0);
        m_pBCParam = (BC_PARAM*)CRsrcDB::getRsrcData(1, "BattleCardList");
        m_nStep = 1;
        break;
    case 1:
        m_nStep = 2;
        iWorldRsrc = CSceneInfo::getCurrentWorldID(TRUE);
        if (iWorldRsrc >= 14) {
            iWorldRsrc = 0;
        }
        if (m_iWorldRsrc && m_iWorldRsrc != iWorldRsrc) {
            CRsrcDB::removeGroup(2, m_iWorldRsrc);
        }
        /* through a reference: the original loads m_pSeqName only after this store */
        _ref(m_iWorldRsrc) = iWorldRsrc;
        if (CRsrcDB::getRsrcTex(m_pSeqName, NAME_CASE_SENSITIVE)) {
            goto loaded;
        }
        if (m_addrRsrcBuf) {
            CRcf::reqLoadRCFFront(2, 100, 0x140, m_addrRsrcBuf, 0);
        } else {
            CRcf::reqLoadRCFFront(2, 100, 0x140, NULL, 0);
        }
        if (m_iWorldRsrc) {
            if (m_addrRsrcBuf) {
                CRcf::reqLoadRCFFront(2, m_iWorldRsrc + 0x95, 0x140, m_addrRsrcBuf + 0x90000, 0);
            } else {
                CRcf::reqLoadRCFFront(2, m_iWorldRsrc + 0x95, 0x140, NULL, 0);
            }
        }
        break;
    case 2:
        if (!CRcf::isCompleteLoadRCF(2, 100)) {
            break;
        }
        if (m_iWorldRsrc && !CRcf::isCompleteLoadRCF(2, m_iWorldRsrc)) {
            break;
        }
    loaded:
        rgba.col = 0x80806228;
        m_pHelpFnt = new KH_AT("(791)") CStrPrim(-0x65, -(SCREEN_HEIGHT / 2) + 0x144, &rgba, 0xE, NULL, 0x40, 0xFFFF28);
        m_pHelpFnt->setLayerFG(15);
        m_pHelpFnt->setSpace(-10, 48);
        CPlayer::exportStatus();
        CSound::playSE(3, -1, 0x40);
        m_nStep = 3;
        break;
    case 3:
        D2::C2DMan::setLayer(0xC);
        m_hLay = D2::C2DMan::create(m_pLadName, 8);
        m_pBaseCtrl = new KH_AT("(810)") C2DSeqCtrl;
        m_pBaseCtrl->setLayer(0xB);
        m_pBaseCtrl->addSeq(m_pLadName, 0, 0);
        vec.init();
        m_pBaseCtrl->setPos(vec);
        setCurrentRoomName(0);
        setPoint(0);
        setTime(0);
        if (!m_bWorldmap) {
            m_pBaseCtrl->addSeq(m_pLadName, 0, 0x1A);
            m_pBaseCtrl->addSeq(m_pLadName, 0, 0x16);
            setMainParty(0);
            setMainCmd(0, 0);
            setMainCmdHist(1, 0);
            setStatusWin0(0);
            setStatusWin1(0);
            m_nStep = 6;
        } else {
            m_nStep = 4;
        }
        break;
    case 5:
        if (!D2::C2DMan::isEnd(m_hLay)) {
            break;
        }
        CSound::setBGMMasterVolume(0x40, -1);
        D2::C2DMan::destroy(m_hLay);
        D2::C2DMan::setLayer(0xE);
        m_hLay = D2::C2DMan::create(m_pLadName, 9);
        m_pBaseCtrl->replaceSeq(0, m_pSeqName, 1);
        setCurrentRoomName(3);
        m_nMainCmdSel = 1;
        setMainCmdHist(1, 0);
        setPoint(3);
        setTime(3);
        m_nStep = 0x11;
        break;
    case 4:
    case 6:
        m_nStep++;
        break;
    case 7:
        if (D2::C2DMan::isEnd(m_hLay)) {
            CSound::setBGMMasterVolume(0x40, -1);
            D2::C2DMan::destroy(m_hLay);
            D2::C2DMan::setLayer(0xE);
            m_hLay = D2::C2DMan::create(m_pLadName, 9);
            m_pBaseCtrl->replaceSeq(0, m_pSeqName, 1);
            setMainParty(3);
            setCurrentRoomName(3);
            setPoint(3);
            setTime(3);
            m_nStep = 8;
        }
        if (m_pMainCmdCtrl[0]->isEnd(0)) {
            setMainCmd(1, 0);
        }
        if (!m_pWindow0Ctrl->isEnd(0)) {
            break;
        }
        setStatusWin0(1);
        setStatusWin1(1);
        break;
    case 8:
        if (!m_pCursorCtrl) {
            m_pCursorCtrl = new KH_AT("(928)") C2DSeqCtrl;
            m_pCursorCtrl->setLayer(0xD);
            m_pCursorCtrl->addSeq(m_pSeqName, 0x12);
            m_pLightCtrl = new KH_AT("(931)") C2DSeqCtrl;
            m_pLightCtrl->setLayer(0xD);
            m_pLightCtrl->addSeq(m_pSeqName, 0x14);
        }
        selectMainCmd(0, TRUE);
        moveLight();
        m_nStep = 9;
        break;
    case 9:
        if (CCard::shouldDispBattleCardFullMessage()) {
            CCard::finishBattleCardFullMessage();
            m_nError = 6;
            m_nStep = 10;
            m_nErrorWinStep = 0;
        } else {
            m_nStep = 0xB;
        }
        break;
    case 10:
        if (deckCheckErrorWinMenu(FALSE)) {
            m_nStep = 0xB;
        }
        moveYNLight();
        break;
    case 11:
        if (CCard::shouldDispMapCardFullMessage()) {
            CCard::finishMapCardFullMessage();
            m_nError = 7;
            m_nStep = 0xC;
            m_nErrorWinStep = 0;
        } else {
            m_nStep = 0xE;
        }
        break;
    case 12:
        if (deckCheckErrorWinMenu(FALSE)) {
            m_nStep = 0xE;
        }
        moveYNLight();
        break;
    case 13:
        if (!m_pMainCmdCtrl[0]->isEnd(0)) {
            break;
        }
        setMainCmd(1, 0);
        setStatusWin0(1);
        setStatusWin1(1);
        selectMainCmd(m_nMainCmdSel, TRUE);
        moveLight();
        setMainParty(3);
        m_nStep = 0xE;
        break;
    case 14:
        if (CPadHandler::isButtonTrg(0x800, 0) || CPadHandler::isTrgCANCEL(0)) {
            if (CGameWork::getFlag(1, 0x1E)) {
                break;
            }
            selectMainCmd(m_nMainCmdSel, FALSE);
            m_nStep = 0x4E;
        } else if (CPadHandler::isButtonTrg(0x40, 0)) {
            if (m_nMainCmdSel < 7) {
                RoomInfo* pRoomInfo = _getCurrentRoomInfo();
                if ((m_nMainCmdSel == 1 && !pRoomInfo->_pSrcInfo) || (m_nMainCmdSel == 3 && !CSceneInfo::getWorldID(0))) {
                    CSound::playSE(5, -1, 0x40);
                    break;
                }
                CSound::playSE(2, -1, 0x40);
                selectMainCmd(m_nMainCmdSel, FALSE);
                setMainCmd(2, 0);
                if (m_nMainCmdSel != 3 && m_nMainCmdSel != 5) {
                    setMainCmdHist(1, 0);
                    setMainCmdHist(7, 0);
                }
                setStatusWin0(2);
                setStatusWin1(2);
                switch (m_nMainCmdSel) {
                case 0:
                    m_nStep = 0xF;
                    break;
                case 3:
                    D2::C2DMan::destroy(m_hLay);
                    D2::C2DMan::setLayer(0xE);
                    m_hLay = D2::C2DMan::create(m_pLadName, 10);
                    m_pBaseCtrl->enableDisp(2, FALSE);
                    setCurrentRoomName(4);
                    setPoint(4);
                    setTime(4);
                    m_nStep = 0x10;
                    break;
                case 2:
                case 4:
                case 5:
                case 6:
                    m_pBaseCtrl->enableDisp(2, FALSE);
                case 1:
                    m_nStep = 0x10;
                    break;
                }
                if (m_nStep != 0xE) {
                    m_nMainCmdSelSave = m_nMainCmdSel;
                    setMainParty(4);
                }
            }
        } else if (CPadHandler::isButtonRpt(0x101000, 0)) {
            if (m_nMainCmdSel - 1 < 0 && !CPadHandler::isButtonTrg(0x101000, 0)) {
                break;
            }
            selectMainCmd(m_nMainCmdSel - 1, TRUE);
            CSound::playSE(1, -1, 0x40);
        } else if (CPadHandler::isButtonRpt(0x204000, 0)) {
            if (m_nMainCmdSel + 1 >= m_nCurrentCmdMax && !CPadHandler::isButtonTrg(0x204000, 0)) {
                break;
            }
            selectMainCmd(m_nMainCmdSel + 1, TRUE);
            CSound::playSE(1, -1, 0x40);
        }
        moveLight();
        break;
    case 15:
    case 16:
        if (!m_pMainCmdCtrl[0]->isEnd(0)) {
            break;
        }
        if (m_nStep == 0xF) {
            setMainCmd(0, m_nMainCmdSel + 1);
            setDeckCard(1);
            setDeckCard(0x12);
            setDeckDeck(1);
            setDeckDeck(0x12);
            setDeckDeck(0x13);
            setDeckCard(7);
            setDeckDeck(7);
        } else {
            setCursor(-1, 0);
            setLight(-1, 0);
        }
        {
            unsigned char mainCmdJmpTbl[7] = { 0x1C, 0x15, 0x40, 0x19, 0x43, 0x46, 0x49 };
            m_nStep = mainCmdJmpTbl[m_nMainCmdSel];
        }
        setStatusWin0(3);
        setStatusWin1(3);
        break;
    case 17:
        setMainCmdHist(1, 0);
        setWorldmap(0);
        m_nStep = 0x12;
        break;
    case 18:
        if (setWorldmap(1)) {
            m_nStep = 0x13;
        }
        break;
    case 19:
        if (setWorldmap(10)) {
            if (CPadHandler::isButtonTrg(0x100, 0) || CPadHandler::isTrgCANCEL(0)) {
                m_nStep = 0x14;
            }
        }
        setWorldmap(3);
        break;
    case 20:
        setWorldmap(4);
        m_nStep = 0x4C;
        break;
    case 21:
        setWorldmap(0);
        m_nStep = 0x16;
        break;
    case 22:
        if (setWorldmap(1)) {
            m_nStep = 0x17;
        }
        break;
    case 23:
        if (setWorldmap(10)) {
            if (CPadHandler::isTrgCANCEL(0)) {
                setWorldmap(4);
                setMainCmdHist(4, 0);
                setMainCmd(0, 0);
                CSound::playSE(4, -1, 0x40);
                m_nStep = 0x18;
            }
        }
        setWorldmap(3);
        break;
    case 24:
        if (!setWorldmap(5)) {
            break;
        }
        setWorldmap(8);
        setStatusWin0(0);
        setStatusWin1(0);
        setMainParty(1);
        m_pBaseCtrl->enableDisp(2, TRUE);
        m_nStep = 0xD;
        break;
    case 25:
        m_hEtcMenu = (new KH_AT("(1209)") CWorldWarp(this, TRUE))->getHandle();
        m_nStep = 0x1A;
        break;
    case 26:
        if (CTaskMan::getTask(m_hEtcMenu)) {
            break;
        }
        setMainCmd(0, 0);
        setStatusWin0(0);
        setStatusWin1(0);
        D2::C2DMan::destroy(m_hLay);
        D2::C2DMan::setLayer(0xE);
        m_hLay = D2::C2DMan::create(m_pLadName, 8);
        setMainParty(1);
        m_pBaseCtrl->enableDisp(2, TRUE);
        setCurrentRoomName(0);
        setPoint(0);
        setTime(0);
        m_nStep = 0x1B;
        break;
    case 27:
        if (m_pMainCmdCtrl[0]->isEnd(0)) {
            setMainCmd(1, 0);
            setStatusWin0(1);
            setStatusWin1(1);
            selectMainCmd(m_nMainCmdSel, TRUE);
            moveLight();
        }
        if (!D2::C2DMan::isEnd(m_hLay)) {
            break;
        }
        D2::C2DMan::destroy(m_hLay);
        D2::C2DMan::setLayer(0xE);
        m_hLay = D2::C2DMan::create(m_pLadName, 9);
        setMainParty(3);
        setCurrentRoomName(3);
        setPoint(3);
        setTime(3);
        m_nStep = 0xE;
        break;
    case 28:
        if (!m_pMainCmdCtrl[0]->isEnd(0)) {
            break;
        }
        SAFE_DELETE(m_pMenuModel);
        m_nKindSelect = m_nDeckSelect = 0;
        m_nKindSelectTop = m_nDeckSelectTop = 0;
        m_nKindCategorySelect = 0;
        initCardData(0);
        initCardKind(m_nDeckNoSelect, m_nKindCategorySelect, FALSE);
        setMainCmd(1, 0);
        selectMainCmd(0, TRUE);
        setDeckCard(3);
        setDeckDeck(3);
        m_nStep = 0x1D;
        break;
    case 29:
        if (!m_bPlayerKind || CGameWork::getFlag(0, 0x84)) {
            m_nStep = 0x21;
            break;
        }
        CGameWork::setFlag(0, 0x84, TRUE);
        m_pTextWnd = new KH_AT("(1282)") CTextWnd(*this, FALSE);
        m_pTextWnd->setObjType((ObjType::eObjType)0x10);
        CBaseWnd::setLayerFG(0x17);
        /* m_pTextWnd through a reference: its loads stay after the setLayerFG stores */
        _ref(m_pTextWnd)->createWindowCTD(0x500600, NULL, NULL, 0, NULL, 0);
        CBaseWnd::setLayerFG(9);
        _ref(m_pTextWnd)->open();
        CSound::playSE(3, -1, 0x40);
        m_nStep = 0x1E;
        break;
    case 30:
    case 31:
        if (m_pTextWnd->getStatus() != 5) {
            break;
        }
        if (!CPadHandler::isButtonTrg(0x60, 0)) {
            break;
        }
        if (m_nStep == 0x1E) {
            CBaseWnd::setLayerFG(0x17);
            _ref(m_pTextWnd)->setTextCTD(0x500601, NULL);
            CBaseWnd::setLayerFG(9);
            CSound::playSE(10, -1, 0x40);
            m_nStep = 0x1F;
        } else {
            m_pTextWnd->close();
            CSound::playSE(4, -1, 0x40);
            m_nStep = 0x20;
        }
        break;
    case 32:
        if (m_pTextWnd->getStatus() != 9) {
            break;
        }
        SAFE_DELETE(m_pTextWnd);
        m_nStep = 0x21;
        break;
    case 33:
        if (CPadHandler::isTrgCANCEL(0)) {
            if (!m_bPlayerKind) {
                nError = CCard::checkCurrentDeck();
                if (nError) {
                    m_nError = nError;
                    m_nStep = 0x23;
                    m_nErrorWinStep = 0;
                    break;
                }
            }
            selectMainCmd(m_nMainCmdSel, FALSE);
            setMainCmd(2, 0);
            setMainCmdHist(4, 0);
            setDeckCard(4);
            setDeckDeck(4);
            CSound::playSE(4, -1, 0x40);
            m_nStep = 0x22;
        } else if (CPadHandler::isButtonTrg(0x40, 0)) {
            if (!m_nMainCmdSel) {
                CSound::playSE(2, -1, 0x40);
                if (m_nKind[m_nKindCategorySelect]) {
                    m_nCardInOut = 0;
                } else {
                    if (!m_nDeck[m_nDeckCategorySelect]) {
                        break;
                    }
                    m_nCardInOut = 1;
                }
                setMainCmdHist(1, 1);
                setMainCmdHist(7, 1);
                setDeckCard(10);
                setDeckDeck(3);
                setMainCmd(2, 0);
                setCursor(-1, 0);
                setLight(-1, 0);
                m_nStep = 0x24;
            } else {
                CSound::playSE(2, -1, 0x40);
                setMainCmdHist(1, 1);
                setMainCmdHist(7, 1);
                setMainCmd(2, 0);
                setCursor(-1, 0);
                setLight(-1, 0);
                setDeckCard(4);
                if (m_nMainCmdSel == 4) {
                    setDeckDeck(4);
                }
            }
            if (m_nMainCmdSel < 5) {
                setButtonHelp(1, 0, m_nMainCmdSel);
                setButtonHelp(7, 0, 0);
                if (!m_nMainCmdSel) {
                    setButtonHelp(10, 0, m_nCardInOut);
                }
            }
            m_pHelpFnt->setText(m_pMainCmdName[1][1][6]);
            m_nStep = deckCheckStepTbl[m_bPlayerKind][m_nMainCmdSel];
        } else if (CPadHandler::isButtonRpt(0x101000, 0)) {
            if (m_nMainCmdSel - 1 < 0 && !CPadHandler::isButtonTrg(0x101000, 0)) {
                break;
            }
            selectMainCmd(m_nMainCmdSel - 1, TRUE);
            CSound::playSE(1, -1, 0x40);
        } else if (CPadHandler::isButtonRpt(0x204000, 0)) {
            if (m_nMainCmdSel + 1 >= m_nCurrentCmdMax && !CPadHandler::isButtonTrg(0x204000, 0)) {
                break;
            }
            selectMainCmd(m_nMainCmdSel + 1, TRUE);
            CSound::playSE(1, -1, 0x40);
        }
        moveLight();
        break;
    case 34:
        if (!m_pMainCmdCtrl[0]->isEnd(0)) {
            break;
        }
        setDeckCard(6);
        setDeckDeck(6);
        m_nMainCmdSel = m_nMainCmdSelSave;
        setCursor(m_nMainCmdSel, 0);
        setMainCmd(0, 0);
        setStatusWin0(0);
        setStatusWin1(0);
        setMainParty(1);
        m_nStep = 0xD;
        break;
    case 35:
        if (deckCheckErrorWinMenu(FALSE)) {
            m_nStep = 0x21;
        }
        moveYNLight();
        break;
    case 36:
        if (!m_bPlayerKind && !m_pDeckCardCtrl[0]->isEnd(0)) {
            break;
        }
        setDeckCard(0xB);
        if (m_bPlayerKind) {
            m_nCardInOut = 1;
        }
        setDeckInfo(1);
        setDeckInfo(7);
        setDeckInfo(0xC);
        m_nStep = 0x25;
        break;
    case 37:
        if (!m_pDeckInfoCtrl->isEnd(0)) {
            break;
        }
        setDeckInfo(3);
        if (!m_nCardInOut) {
            setDeckInfo(0xA);
        } else {
            setDeckInfo(0xB);
        }
        m_nMainMode = 2;
        m_nTechWinStep = -1;
        m_nStep = 0x26;
        m_nSubStep = 0;
        break;
    case 38:
        if (deckCommonButton()) {
            break;
        }
        deckEditMenu();
        techWinMenu();
        break;
    case 39:
        if (!m_pDeckInfoCtrl->isEnd(0)) {
            break;
        }
        setMainCmd(0, 1);
        setButtonHelp(4, 0, 0);
        setDeckInfo(6);
        m_nKindSelect = m_nDeckSelect = 0;
        m_nKindSelectTop = m_nDeckSelectTop = 0;
        m_nKindCategorySelect = 0;
        setDeckCard(0x14);
        setDeckCard(0xC);
        setDeckDeck(0x12);
        m_nStep = 0x28;
        break;
    case 40:
        if (!(!m_bPlayerKind ? m_pDeckCardCtrl[0]->isEnd(0) : m_pDeckDeckCtrl[0]->isEnd(0))) {
            break;
        }
        setMainCmd(1, 0);
        selectMainCmd(0, TRUE);
        setDeckInfo(6);
        setDeckCard(3);
        m_nStep = 0x21;
        break;
    case 41:
        m_nMainMode = 2;
        setDeckCard(4);
        m_nSubStep = 0;
        deckSetWinMenu();
        m_nStep = 0x2A;
        break;
    case 42:
        deckSetWinMenu();
        if (!m_pDeckCardCtrl[0]->isEnd(0)) {
            break;
        }
        setDeckCard(6);
        m_nStep = 0x2B;
        break;
    case 43:
        if (deckSetWinMenu() >= 0) {
            break;
        }
        setDeckCard(7);
        setDeckCard(1);
        setMainCmdHist(4, 1);
        setMainCmd(0, 1);
        m_nStep = 0x2C;
        break;
    case 44:
        deckSetWinMenu();
        if (!m_pMainCmdCtrl[0]->isEnd(0)) {
            break;
        }
        setDeckCard(3);
        setMainCmd(1, 0);
        selectMainCmd(m_nMainCmdSel, TRUE);
        m_nStep = 0x21;
        break;
    case 45:
        m_bShortCutMode = TRUE;
        m_nMainMode = 2;
        m_nCardInOut = 1;
        setDeckCard(4);
        setDeckInfo(1);
        setDeckInfo(7);
        setDeckInfo(0xC);
        setDeckInfo(0xF);
        m_nSubStep = 0;
        m_nStep = 0x2E;
        break;
    case 46:
        if (!m_pDeckInfoCtrl->isEnd(0)) {
            break;
        }
        setDeckDeck(3);
        setDeckCard(6);
        setDeckInfo(3);
        m_nStep = 0x2F;
        break;
    case 47:
        if (deckCommonButton2()) {
            break;
        }
        if (deckEditMenu()) {
            setButtonHelp(4, 0, 0);
            setDeckCard(7);
            setDeckCard(1);
            setDeckInfo(4);
            m_nCardInOut = 0;
            setDeckDeck(3);
            setMainCmdHist(4, 1);
            setMainCmd(0, 1);
            CSound::playSE(4, -1, 0x40);
            m_nStep = 0x30;
        }
        techWinMenu();
        break;
    case 48:
        if (!m_pMainCmdCtrl[0]->isEnd(0)) {
            break;
        }
        m_bShortCutMode = FALSE;
        m_nDeckSelect = 0;
        m_nDeckSelectTop = 0;
        setDeckDeck(0x12);
        setDeckCard(3);
        setMainCmd(1, 0);
        selectMainCmd(m_nMainCmdSel, TRUE);
        m_nStep = 0x21;
        break;
    case 49:
        m_nMainMode = 2;
        setDeckCard(4);
        m_nSubStep = 0;
        deckClearWinMenu();
        m_nStep = 0x32;
        break;
    case 50:
        deckClearWinMenu();
        if (!m_pDeckCardCtrl[0]->isEnd(0)) {
            break;
        }
        setDeckCard(6);
        m_nStep = 0x33;
        break;
    case 51:
        if (deckClearWinMenu() >= 0) {
            break;
        }
        setDeckCard(7);
        setDeckCard(1);
        setMainCmdHist(4, 1);
        setMainCmd(0, 1);
        m_nStep = 0x34;
        break;
    case 52:
        deckClearWinMenu();
        if (!m_pMainCmdCtrl[0]->isEnd(0)) {
            break;
        }
        setDeckCard(3);
        setMainCmd(1, 0);
        selectMainCmd(m_nMainCmdSel, TRUE);
        m_nStep = 0x21;
        break;
    case 53:
        m_nMainMode = 2;
        setDeckCard(4);
        setDeckDeck(4);
        CDecknameEditWnd::startEdit(this, m_nDeckNoSelect);
        m_nStep = 0x36;
        break;
    case 54:
        if (!m_pDeckCardCtrl[0]->isEnd(0)) {
            break;
        }
        setDeckCard(6);
        setDeckDeck(6);
        m_pHelpFnt->setText(CCtdMan::getText(0x50040D));
        m_nStep = 0x37;
        break;
    case 55:
        if (!CDecknameEditWnd::isFinish()) {
            break;
        }
        m_nDeckNoSelect = CDecknameEditWnd::getLastEditDeckNo();
        initCardData(0);
        initCardKind(m_nDeckNoSelect, m_nKindCategorySelect, FALSE);
        initDeck(m_nDeckNoSelect, 0);
        setDeckCard(7);
        setDeckCard(1);
        setDeckCard(0x12);
        setDeckDeck(7);
        setDeckDeck(1);
        setDeckDeck(0x13);
        setDeckDeck(0x12);
        setMainCmdHist(4, 1);
        setMainCmd(0, 1);
        setButtonHelp(4, 0, 0);
        m_pHelpFnt->setText(m_pMainCmdName[1][1][6]);
        m_nStep = 0x38;
        break;
    case 56:
        if (!m_pMainCmdCtrl[0]->isEnd(0)) {
            break;
        }
        setDeckCard(3);
        setDeckDeck(3);
        setMainCmd(1, 0);
        selectMainCmd(m_nMainCmdSel, TRUE);
        m_nStep = 0x21;
        break;
    case 57:
        m_bShortCutMode = TRUE;
        m_nKindCategorySelect = 0;
        m_nDeckCategorySelect = 0;
        m_nDeckNoSelectSave = m_nDeckNoSelect;
        m_nDeckNoSelect = 0;
        initCardData(0);
        initCardKind(0, 0, TRUE);
        m_nMainMode = 2;
        m_nCardInOut = 0;
        setDeckDeck(4);
        setDeckCard(0x12);
        setDeckCard(0xA);
        m_nSubStep = 0;
        m_nStep = 0x3A;
        break;
    case 58:
        if (!m_pDeckCardCtrl[0]->isEnd(0)) {
            break;
        }
        setDeckDeck(6);
        setDeckCard(0xB);
        setDeckInfo(1);
        setDeckInfo(7);
        setDeckInfo(0xC);
        setDeckInfo(0xF);
        m_nStep = 0x3B;
        break;
    case 59:
        if (!m_pDeckInfoCtrl->isEnd(0)) {
            break;
        }
        setDeckInfo(3);
        m_nStep = 0x3C;
        break;
    case 60:
        if (deckEditMenu()) {
            setDeckInfo(4);
            CSound::playSE(4, -1, 0x40);
            m_nStep = 0x3D;
        }
        techWinMenu();
        break;
    case 61:
        if (!m_pDeckInfoCtrl->isEnd(0)) {
            break;
        }
        setDeckCard(0xC);
        setDeckDeck(1);
        setDeckDeck(7);
        setMainCmdHist(4, 1);
        setMainCmd(0, 1);
        m_nStep = 0x3E;
        break;
    case 62:
        if (!m_pDeckCardCtrl[0]->isEnd(0)) {
            break;
        }
        m_bShortCutMode = FALSE;
        m_nDeckNoSelect = m_nDeckNoSelectSave;
        initCardData(0);
        initCardKind(m_nDeckNoSelect, m_nKindCategorySelect, FALSE);
        m_nKindSelect = m_nDeckSelect = 0;
        m_nKindSelectTop = m_nDeckSelectTop = 0;
        m_nKindCategorySelect = 0;
        setDeckCard(0x14);
        setDeckDeck(0x12);
        setDeckCard(3);
        setDeckDeck(3);
        setDeckInfo(0xC);
        setMainCmd(1, 0);
        selectMainCmd(m_nMainCmdSel, TRUE);
        m_nStep = 0x21;
        break;
    case 63:
        if (deckCheckErrorWinMenu(FALSE)) {
            m_nStep = 0x3C;
        }
        moveYNLight();
        break;
    case 64:
        m_nMainMode = 2;
        m_nSubStep = 0;
        mapCardWinMenu();
        m_nStep = 0x41;
        break;
    case 65:
        if (mapCardWinMenu() >= 0) {
            break;
        }
        setMainCmdHist(4, 0);
        m_nStep = 0x42;
        break;
    case 67:
        m_nMainMode = 2;
        m_nSubStep = 0;
        statusWinMenu();
        m_nStep = 0x44;
        break;
    case 68:
        if (statusWinMenu() >= 0) {
            break;
        }
        setMainCmdHist(4, 0);
        m_nStep = 0x45;
        break;
    case 70:
        CJiminyMemoFlag::initTask();
        CJiminyMemoFlag::createTask(this, m_bPlayerKind);
        SAFE_DELETE(m_pMenuModel);
        m_nStep = 0x47;
        break;
    case 71:
        if (CJiminyMemoFlag::checkTaskOut()) {
            m_nStep = 0x48;
        }
        break;
    case 72:
        setMainCmd(0, 0);
        setStatusWin0(0);
        setStatusWin1(0);
        setMainParty(0);
        m_pBaseCtrl->enableDisp(2, TRUE);
        m_nStep = 0xD;
        break;
    case 73:
        m_nMainMode = 2;
        m_nSubStep = 0;
        configWinMenu();
        m_nStep = 0x4A;
        break;
    case 74:
        if (configWinMenu() >= 0) {
            break;
        }
        setMainCmdHist(4, 0);
        m_nStep = 0x4B;
        break;
    case 66:
    case 69:
    case 75:
        setMainCmd(0, 0);
        setStatusWin0(0);
        setStatusWin1(0);
        setMainParty(1);
        m_pBaseCtrl->enableDisp(2, TRUE);
        m_nStep = 0xD;
        break;
    case 76:
        CSound::setBGMMasterVolume(-1, -1);
        D2::C2DMan::destroy(m_hLay);
        D2::C2DMan::setLayer(0xE);
        m_hLay = D2::C2DMan::create(m_pLadName, 10);
        m_pBaseCtrl->replaceSeq(0, m_pSeqName, 2);
        setCurrentRoomName(4);
        setMainCmdHist(4, 0);
        setPoint(4);
        setTime(4);
        CSound::playSE(4, -1, 0x40);
        m_nStep++;
        break;
    case 77:
        setWorldmap(5);
        if (!D2::C2DMan::isEnd(m_hLay)) {
            break;
        }
        D2::C2DMan::destroy(m_hLay);
        dispose();
        m_nStep = 0x59;
        break;
    case 78:
        CSound::setBGMMasterVolume(-1, -1);
        D2::C2DMan::destroy(m_hLay);
        m_hLay = D2::C2DMan::create(m_pLadName, 10);
        m_pBaseCtrl->replaceSeq(0, m_pSeqName, 2);
        setMainParty(4);
        m_pBaseCtrl->enableDisp(2, FALSE);
        setCurrentRoomName(4);
        SAFE_DELETE(m_pCursorCtrl);
        SAFE_DELETE(m_pLightCtrl);
        setMainCmd(2, 0);
        setStatusWin0(2);
        setStatusWin1(2);
        SAFE_DELETE(m_pPointCtrl);
        setTime(8);
        CSound::playSE(4, -1, 0x40);
        m_nStep++;
        break;
    case 79:
        if (!m_pWindow0Ctrl->isEnd(0)) {
            break;
        }
        D2::C2DMan::destroy(m_hLay);
        dispose();
        m_nStep = 0x59;
        break;
    case 0x59:
    default:
        out();
        break;
    }
    if (m_nStep < 0x59) {
        setMainCmdHist(m_nMainCmdHistRun[0], 0);
        setMainCmdHist(m_nMainCmdHistRun[1], 1);
        setButtonHelp(m_nButtonHelpRun[0], 0, 0);
        setButtonHelp(m_nButtonHelpRun[1], 1, 0);
        setTime(9);
    }
}
INT CCamp::deckCheckErrorWinMenu(BOOL bCardDelete) {
    static USHORT errorWinSeqTbl[3] = { 72, 73, 74 };
    static USHORT buttonYesNoSeqTbl[4] = { 188, 189, 190, 187 };
    static LPCTSTR pSqdName = "message.sqd";
    FVECTOR vec;
    BOOL ret = FALSE;
    G_RGBA rgba;
    C2DFnt* p2DFnt;
    C2DSeqCtrl* pSeqCtrlTmp;

    switch (m_nErrorWinStep) {
    case 0:
        m_pDeckCheckErrorButton = new KH_AT("(2042)") CButton;
        vec.set(-256.0f, -192.0f, 0.0f, 1.0f);
        m_pDeckCheckErrorButton->init(m_pSeqName, 0x14, errorWinSeqTbl, vec, NULL, CCtdMan::getText(m_nError + 0x5004FF), 0xF, 0.0f,
                                      60.0f, 0x42, NULL, -1);
        m_pDeckCheckErrorButton->get2DSeqCtrl()->addSeqShift(pSqdName, 0x73, -148.0f, 22.0f);
        rgba.col = 0x80080880;
        p2DFnt = new KH_AT("(2052)") C2DFnt(0, 0, &rgba, 0xF, CCtdMan::getText(0x500800), 0x40);
        m_pDeckCheckErrorButton->get2DSeqCtrl()->addFntShift(m_pSeqName, p2DFnt, -120.0f, 18.0f);
        m_pDeckCheckErrorButton->setFontSpace(0, 0x60);
        if (bCardDelete) {
            m_nSelYesNo = 1;
            m_pButtonYenNo[0] = new KH_AT("(2061)") CButton;
            C2DMan::getLayPosition(m_pLadName, 6, 1, vec, 1);
            vec.y += 70.0f;
            pSeqCtrlTmp = m_pDeckCheckErrorButton->get2DSeqCtrl();
            m_pButtonYenNo[0]->init(m_pSeqName, 0x14, buttonYesNoSeqTbl, vec, pSeqCtrlTmp, CCtdMan::getText(0x500700), 0xF, 0.0f,
                                    0.0f, 0x42, NULL, -1);
            m_pButtonYenNo[1] = new KH_AT("(2067)") CButton;
            C2DMan::getLayPosition(m_pLadName, 6, 2, vec, 1);
            vec.y += 70.0f;
            pSeqCtrlTmp = m_pDeckCheckErrorButton->get2DSeqCtrl();
            m_pButtonYenNo[1]->init(m_pSeqName, 0x14, buttonYesNoSeqTbl, vec, pSeqCtrlTmp, CCtdMan::getText(0x500701), 0xF, 0.0f,
                                    0.0f, 0x42, NULL, -1);
        } else {
            m_pButtonYenNo[1] = new KH_AT("(2078)") CButton;
            C2DMan::getLayPosition(m_pLadName, 6, 2, vec, 1);
            vec.y += 70.0f;
            pSeqCtrlTmp = m_pDeckCheckErrorButton->get2DSeqCtrl();
            m_pButtonYenNo[1]->init(m_pSeqName, 0x14, buttonYesNoSeqTbl, vec, pSeqCtrlTmp, CCtdMan::getText(0x500702), 0xF, 0.0f,
                                    0.0f, 0x42, NULL, -1);
        }
        CSound::playSE(3, -1, 0x40);
        m_nErrorWinStep = 1;
        break;
    case 1:
        if (m_pDeckCheckErrorButton->checkEndAfterSet(CButton::BS_LOOP)) {
            if (bCardDelete) {
                m_pButtonYenNo[0]->setSequence(CButton::BS_LOOP, FALSE);
            }
            m_pButtonYenNo[1]->setSequence(CButton::BS_ACTIVE, FALSE);
            pSeqCtrlTmp = m_pButtonYenNo[1]->get2DSeqCtrl();
            if (!m_pYNCursorCtrl) {
                m_pYNCursorCtrl = new KH_AT("(2105)") C2DSeqCtrl;
                m_pYNCursorCtrl->setLayer(0x14);
                m_pYNCursorCtrl->addSeqShift(m_pSeqName, 0x12, -48.0f, -14.0f);
            }
            m_pYNCursorCtrl->setParent(pSeqCtrlTmp);
            vec = pSeqCtrlTmp->m_seqControl.vPos;
            vec.x += 94.0f;
            vec.y += -74.0f;
            m_vYNPosLightTarget = vec;
            if (!m_pYNLightCtrl) {
                m_pYNLightCtrl = new KH_AT("(2117)") C2DSeqCtrl;
                m_pYNLightCtrl->setLayer(0x14);
                m_pYNLightCtrl->addSeq(m_pSeqName, 0x14);
                m_vYNPosLightCurrent = m_vYNPosLightTarget;
            }
            m_fgYNLightMove = 1;
            m_nErrorWinStep = 3;
            m_vYNPosLightOld = m_vYNPosLightCurrent;
            m_fYNLightTimer = 0.0f;
        }
        break;
    case 3:
        if (CPadHandler::isButtonTrg(0x60)) {
            if (bCardDelete) {
                if (CPadHandler::isButtonTrg(0x20)) {
                    m_nSelYesNo = 1;
                    CSound::playSE(4, -1, 0x40);
                } else if (!m_nSelYesNo) {
                    CSound::playSE(m_nMainCmdSel == 3 ? 2 : 0x10, -1, 0x40);
                } else {
                    CSound::playSE(2, -1, 0x40);
                }
                m_pButtonYenNo[0]->setSequence(CButton::BS_OUT, FALSE);
            } else {
                CSound::playSE(CPadHandler::isButtonTrg(0x20) ? 4 : 2, -1, 0x40);
            }
            m_pDeckCheckErrorButton->setSequence(CButton::BS_OUT, FALSE);
            m_pButtonYenNo[1]->setSequence(CButton::BS_OUT, FALSE);
            delete m_pYNCursorCtrl;
            m_pYNCursorCtrl = NULL;
            m_pYNLightCtrl->replaceSeq(0, m_pSeqName, 0x15);
            m_nErrorWinStep = 4;
        } else if (bCardDelete && _isButtonRpt(0xC0A000, 0)) {
            if (!(m_nSelYesNo ^= 1)) {
                m_pButtonYenNo[0]->setSequence(CButton::BS_ACTIVE, FALSE);
                m_pButtonYenNo[1]->setSequence(CButton::BS_LOOP, FALSE);
            } else {
                m_pButtonYenNo[0]->setSequence(CButton::BS_LOOP, FALSE);
                m_pButtonYenNo[1]->setSequence(CButton::BS_ACTIVE, FALSE);
            }
            pSeqCtrlTmp = m_pButtonYenNo[m_nSelYesNo]->get2DSeqCtrl();
            m_pYNCursorCtrl->setParent(pSeqCtrlTmp);
            m_vYNPosLightOld = m_vYNPosLightCurrent;
            vec = pSeqCtrlTmp->m_seqControl.vPos;
            vec.x += 94.0f;
            vec.y += -74.0f;
            m_fYNLightTimer = 0.0f;
            m_fgYNLightMove = 1;
            m_vYNPosLightTarget = vec;
            CSound::playSE(1, -1, 0x40);
        }
        break;
    case 4:
        if (m_pDeckCheckErrorButton->checkEndAfterSet(CButton::BS_NON)) {
            delete m_pDeckCheckErrorButton;
            m_pDeckCheckErrorButton = NULL;
            delete m_pButtonYenNo[0];
            m_pButtonYenNo[0] = NULL;
            delete m_pButtonYenNo[1];
            m_pButtonYenNo[1] = NULL;
            delete m_pYNLightCtrl;
            m_pYNLightCtrl = NULL;
            m_nErrorWinStep = 999;
            ret = TRUE;
        }
        break;
    }
    return ret;
}

void CCamp::moveYNLight() {
    if (m_pYNLightCtrl && m_fgYNLightMove >= 0) {
        m_fYNLightTimer += getFrameRateF();
        if (m_fYNLightTimer >= 40.0f) {
            m_fYNLightTimer = 40.0f;
            m_fgYNLightMove = -1;
        }

        FLOAT t = 1.0f - powf(1.0f - m_fYNLightTimer / 40.0f, 2.5f);
        InterVectorXYZ(m_vYNPosLightCurrent, m_vYNPosLightTarget, m_vYNPosLightOld, t);
        m_pYNLightCtrl->setLocalPos(m_vYNPosLightCurrent);
    }
}

INT CCamp::deckCommonButton() {
    BOOL fgButton = FALSE;

    if (m_nStep != 0x26) {
        ASSERT_AT(2226, 0);
        return 0;
    }
    while (TRUE) {
        if (_isButtonTrg(3, 0) && !m_bPlayerKind) {
            if (m_nSubStep <= 0) {
                break;
            }
            if (m_nSubStep != 1 && m_nSubStep != 4 && m_nSubStep != 5) {
                CSound::playSE(5, -1, 0x40);
            } else {
                if (_isButtonTrg(1, 0)) {
                    if (--m_nDeckNoSelect < 0) {
                        m_nDeckNoSelect = 2;
                    }
                } else {
                    if (++m_nDeckNoSelect >= 3) {
                        m_nDeckNoSelect = 0;
                    }
                }
                setDeckDeck(0x13);
                fgButton = TRUE;
                initCardData(0);
                initCardKind(m_nDeckNoSelect, m_nKindCategorySelect, FALSE);
                initDeck(m_nDeckNoSelect, 0);
                if (m_nSubStep == 4) {
                    setCursor4(m_nDeckSelect, 0);
                    if (!m_nKind[m_nKindCategorySelect]) {
                        m_nKindSelectTop = 0;
                        m_nKindSelect = 0;
                    } else if (m_nKindSelect >= m_nKind[m_nKindCategorySelect]) {
                        m_nKindSelect = m_nKind[m_nKindCategorySelect] - 1;
                        m_nKindSelectTop = m_nKindSelect - 6;
                        if (m_nKindSelectTop < 0) {
                            m_nKindSelectTop = 0;
                        }
                        m_nKindSelectTop -= m_nKindSelectTop % 3;
                    }
                } else if (m_nSubStep == 1) {
                    if (!m_nKind[m_nKindCategorySelect]) {
                        m_nKindSelectTop = 0;
                        m_nKindSelect = 0;
                        m_bCategorySelectMode = 1;
                        setDeckInfo(0xC);
                        setCursor2(m_nKindCategorySelect, 5, FALSE);
                        m_nSubStep = 5;
                    } else {
                        if (m_nKindSelect >= m_nKind[m_nKindCategorySelect]) {
                            m_nKindSelect = m_nKind[m_nKindCategorySelect] - 1;
                            m_nKindSelectTop = m_nKindSelect - 6;
                            if (m_nKindSelectTop < 0) {
                                m_nKindSelectTop = 0;
                            }
                            m_nKindSelectTop -= m_nKindSelectTop % 3;
                        }
                        setCursor2(m_nKindSelect, 0, FALSE);
                    }
                }
                setDeckCard(0x12);
                if (m_nMainMode >= 2) {
                    setDeckInfo(0xC);
                }
                setDeckDeck(0x12);
                CSound::playSE(1, -1, 0x40);
            }
        } else if (_isButtonTrg(0x100, 0) && !m_bPlayerKind) {
            if (m_bCardPermuteMode) {
                CSound::playSE(5, -1, 0x40);
            } else {
                CCard::packDeck(m_nDeckNoSelect);
                initCardData(0);
                initCardKind(m_nDeckNoSelect, m_nKindCategorySelect, FALSE);
                initDeck(m_nDeckNoSelect, 0);
                if (m_nSubStep >= 3) {
                    setDeckInfo(0x11);
                }
                setDeckDeck(0x12);
                fgButton = TRUE;
                CSound::playSE(2, -1, 0x40);
            }
        } else if (_isButtonTrg(0x10, 0) && !m_bPlayerKind && !m_bCardPermuteMode) {
            if (m_nSubStep != 4 || !m_nDeck[0] || !m_pDeck[m_nDeckSelect].m_nId) {
                CSound::playSE(5, -1, 0x40);
            } else {
                m_nDeckSelectHist = m_nDeckSelect;
                m_bCardPermuteMode = 1;
                m_nDeckCategorySelect = 0;
                m_nSubStep = 6;
                setDeckDeck(0x14);
                fgButton = TRUE;
                setDeckInfo(0xE);
                CSound::playSE(0xF, -1, 0x40);
            }
        } else if (_isButtonTrg(0x80, 0)) {
            m_fgPadDisable = 1;
            fgButton = TRUE;
            m_nTechWinStep = 0;
        }
        break;
    }
    return fgButton;
}

INT CCamp::deckCommonButton2() {
    BOOL fgButton = FALSE;

    do {
        if (_isButtonTrg(3, 0) && !m_bPlayerKind) {
            if (_isButtonTrg(1, 0)) {
                if (--m_nDeckNoSelect < 0) {
                    m_nDeckNoSelect = 2;
                }
            } else {
                if (++m_nDeckNoSelect >= 3) {
                    m_nDeckNoSelect = 0;
                }
            }
            initCardData(0);
            fgButton = TRUE;
            initCardKind(m_nDeckNoSelect, m_nKindCategorySelect, FALSE);
            initDeck(m_nDeckNoSelect, 0);
            setDeckDeck(0x13);
            setDeckDeck(0x12);
            setDeckInfo(0xC);
            CSound::playSE(1, -1, 0x40);
        } else if (_isButtonTrg(0x80, 0)) {
            m_nTechWinStep = 0;
            m_fgPadDisable = 1;
            fgButton = TRUE;
        }
    } while (0);
    return fgButton;
}

INT CCamp::techWinMenu() {
    BOOL bChange = FALSE;
    INT ret = 0;

    switch (m_nTechWinStep) {
    case 0:
        setButtonHelp(4, 0, 0);
        setButtonHelp(1, 1, 6);
        setTechWin(0);
        setTechWin(7);
        setTechWin(9);
        m_pHelpTextOld = m_pHelpFnt->getText();
        m_pHelpFnt->setText(m_pMainCmdName[1][1][6]);
        CSound::playSE(3, -1, 0x40);
        m_nTechWinStep = 1;
        break;
    case 1:
        if (setTechWin(1)) {
            m_pHelpFnt->setText(CCtdMan::getText(0x500408));
            m_nTechWinStep = 2;
        }
        break;
    case 2:
        setTechWin(3);
        if (CPadHandler::isButtonTrg(0xA0)) {
            setButtonHelp(1, 0, m_nButtonHelpKind[0]);
            setButtonHelp(4, 1, 0);
            setButtonHelp(0xA, 0, m_nCardInOut);
            setTechWin(4);
            m_pHelpFnt->setText(m_pMainCmdName[1][1][6]);
            CSound::playSE(4, -1, 0x40);
            m_nTechWinStep = 4;
        } else if (CPadHandler::isButtonTrg(0x40)) {
            if (!CTechnic::canUse(m_nTechKind)) {
                CSound::playSE(5, -1, 0x40);
                break;
            }
            new KH_AT("(2453)") CTechHelp(this, m_nTechKind);
            m_nTechWinStep = 3;
        } else if (CPadHandler::isButtonRpt(0x101000)) {
            if (--m_nTechSelect < 0) {
                if (!CPadHandler::isButtonTrg(0x101000)) {
                    m_nTechSelect++;
                    break;
                }
                m_nTechSelect = m_nTechSelectMax - 1;
            }
            bChange = TRUE;
        } else if (CPadHandler::isButtonRpt(0x204000)) {
            if (++m_nTechSelect >= m_nTechSelectMax) {
                if (!CPadHandler::isButtonTrg(0x204000)) {
                    m_nTechSelect--;
                    break;
                }
                m_nTechSelect = 0;
            }
            bChange = TRUE;
        } else if (CPadHandler::isButtonRpt(4)) {
            if ((m_nTechSelect -= 9) < 0) {
                if (m_nTechSelect < -8) {
                    if (!CPadHandler::isButtonTrg(4)) {
                        m_nTechSelect += 9;
                        break;
                    }
                    m_nTechSelect = m_nTechSelectMax - 1;
                } else {
                    m_nTechSelect = 0;
                }
            } else if ((m_nTechSelectTop -= 9) < 0) {
                m_nTechSelectTop = 0;
            }
            bChange = TRUE;
        } else if (CPadHandler::isButtonRpt(8)) {
            if ((m_nTechSelect += 9) >= m_nTechSelectMax) {
                if (m_nTechSelect >= m_nTechSelectMax + 8) {
                    if (!CPadHandler::isButtonTrg(8)) {
                        m_nTechSelect -= 9;
                        break;
                    }
                    m_nTechSelect = 0;
                } else {
                    m_nTechSelect = m_nTechSelectMax - 1;
                }
            } else if ((m_nTechSelectTop += 9) >= m_nTechSelectMax - 9) {
                m_nTechSelectTop = m_nTechSelectMax - 9;
            }
            bChange = TRUE;
        }
        if (bChange) {
            CSound::playSE(1, -1, 0x40);
            setTechWin(9);
        }
        break;
    case 3:
        if (CTechHelp::isEnd()) {
            m_nTechWinStep = 2;
        }
        break;
    case 4:
        if (setTechWin(5)) {
            setTechWin(6);
            ret = 1;
            m_pHelpFnt->setText(m_pHelpTextOld);
            m_nTechWinStep = -1;
        }
        break;
    default:
        if (m_fgPadDisable) {
            m_fgPadDisable--;
        }
        break;
    }
    return ret;
}

INT CCamp::deckSetWinMenu() {
    INT ret = 0;
    BOOL bChange = FALSE;

    switch (m_nSubStep) {
    case 0:
        m_nDeckSetSelect = m_nDeckNoSelect;
        setDeckSetWin(0);
        setDeckSetWin(7);
        setDeckSetWin(9);
        m_nSubStep = 1;
        break;
    case 1:
        if (setDeckSetWin(1)) {
            ret = 1;
            m_nSubStep = 2;
        }
        break;
    case 2:
        setDeckSetWin(3);
        if (_isButtonTrg(0x20, 0)) {
            setDeckSetWin(4);
            ret = -1;
            setButtonHelp(4, 0, 0);
            m_nSubStep = 5;
            CSound::playSE(4, -1, 0x40);
        } else if (_isButtonTrg(0x40, 0)) {
            CSound::playSE(0xD, -1, 0x40);
            m_nSubStep = 3;
        } else if (_isButtonTrg(0x80, 0)) {
            m_nTechWinStep = 0;
            m_nSubStep = 4;
            break;
        } else {
            if (_isButtonTrg(1, 0) || _isButtonRpt(0x101000, 0)) {
                if (--m_nDeckNoSelect < 0) {
                    m_nDeckNoSelect = 2;
                }
            change:
                m_nDeckSetSelect = m_nDeckNoSelect;
                bChange = TRUE;
                initCardData(0);
                initCardKind(m_nDeckNoSelect, m_nKindCategorySelect, FALSE);
                initDeck(m_nDeckNoSelect, 0);
                setDeckSetWin(9);
                setDeckDeck(0x13);
                setDeckDeck(0x12);
                setDeckCard(0x12);
            } else if (_isButtonTrg(2, 0) || _isButtonRpt(0x204000, 0)) {
                if (++m_nDeckNoSelect >= 3) {
                    m_nDeckNoSelect = 0;
                }
                goto change;
            }
        }
        if (bChange) {
            CSound::playSE(1, -1, 0x40);
            setDeckSetWin(9);
        }
        break;
    case 3:
        CCard::setCurrentDeckNo(m_nDeckSetSelect);
        setDeckSetWin(9);
        setDeckDeck(0x13);
        m_nSubStep = 2;
        break;
    case 4:
        if (techWinMenu()) {
            m_nSubStep = 2;
        }
        break;
    case 5:
        if (setDeckSetWin(5)) {
            setDeckSetWin(6);
            ret = 1;
            m_nSubStep = 999;
        }
        break;
    }
    return ret;
}

INT CCamp::deckClearWinMenu() {
    INT ret = 0;

    switch (m_nSubStep) {
    case 0:
        m_nDeckSetSelect = m_nDeckNoSelect;
        setDeckSetWin(0);
        setDeckSetWin(7);
        setDeckSetWin(9);
        m_nSubStep = 1;
        break;
    case 1:
        if (setDeckSetWin(1)) {
            ret = 1;
            m_nSubStep = 2;
        }
        break;
    case 2:
        setDeckSetWin(3);
        if (_isButtonTrg(0x20, 0)) {
            setDeckSetWin(4);
            ret = -1;
            setButtonHelp(4, 0, 0);
            m_nSubStep = 5;
            CSound::playSE(4, -1, 0x40);
        } else if (_isButtonTrg(0x40, 0)) {
            m_nError = 8;
            m_nErrorWinStep = 0;
            m_nSubStep = 3;
        } else if (_isButtonTrg(0x80, 0)) {
            m_nTechWinStep = 0;
            m_nSubStep = 4;
        } else {
            if (_isButtonTrg(1, 0) || _isButtonRpt(0x101000, 0)) {
                if (--m_nDeckNoSelect < 0) {
                    m_nDeckNoSelect = 2;
                }
            } else if (_isButtonTrg(2, 0) || _isButtonRpt(0x204000, 0)) {
                if (++m_nDeckNoSelect >= 3) {
                    m_nDeckNoSelect = 0;
                }
            } else {
                break;
            }
            m_nDeckSetSelect = m_nDeckNoSelect;
            CSound::playSE(1, -1, 0x40);
            goto reinit;
        }
        break;
    case 3:
        if (deckCheckErrorWinMenu(TRUE)) {
            m_nSubStep = 2;
            if (!m_nSelYesNo) {
                CCard::clearDeck(m_nDeckSetSelect);
                if (m_nDeckSetSelect == m_nDeckNoSelect) {
                reinit:
                    initCardData(0);
                    initCardKind(m_nDeckNoSelect, m_nKindCategorySelect, FALSE);
                    initDeck(m_nDeckNoSelect, 0);
                    setDeckSetWin(9);
                    setDeckDeck(0x13);
                    setDeckDeck(0x12);
                    setDeckCard(0x12);
                }
            }
        }
        moveYNLight();
        break;
    case 4:
        if (techWinMenu()) {
            m_nSubStep = 2;
        }
        break;
    case 5:
        if (setDeckSetWin(5)) {
            setDeckSetWin(6);
            ret = 1;
            m_nSubStep = 999;
        }
        break;
    }
    return ret;
}

INT CCamp::mapCardWinMenu() {
    BOOL ret = FALSE;
    BOOL fgCursorMove = FALSE;
    INT nNumSelect;

    switch (m_nSubStep) {
    case 0:
        m_bCategorySelectMode = FALSE;
        setMapCardWin(0);
        setMapCardWin(7);
        setMapCardWin(0xA);
        m_nSubStep = 1;
        break;
    case 1:
        if (setMapCardWin(1)) {
            setCursor2(m_nMCSelSelect, 0, TRUE);
            if (!m_nMapKind[0]) {
                m_bCategorySelectMode = TRUE;
                setCursor2(0, 5, TRUE);
                moveCursor2();
                m_nSubStep = 4;
            } else {
                ret = 1;
                moveCursor2();
                m_nSubStep = 2;
            }
        }
        break;
    case 2:
        if (_isButtonTrg(0x20, 0)) {
        cancel: /* MATCH: label before the shared flag store (ret = 1 fills the setCursor2 delay slot) */
            m_bCategorySelectMode = FALSE;
            ret = 1;
            setCursor2(0, 2, TRUE);
            setCursor3(0, 2, TRUE);
            setMapCardWin(4);
            CSound::playSE(4, -1, 0x40);
            m_nSubStep = 3;
        } else if (_isButtonTrg(0x40, 0)) {
            if ((UINT)(m_nMapKindTbl[m_nMCSelSelect] - 0xA1) < 0x19) {
                CSound::playSE(2, -1, 0x40);
                m_nMCSelNumSelect = searchNumExist(0, 0, TRUE);
                setCursor2(0, 3, TRUE);
                setCursor3(m_nMCSelNumSelect, 0, TRUE);
                moveCursor3();
                m_nSubStep = 5;
                break;
            }
            CSound::playSE(5, -1, 0x40);
            break;
        } else if (_isButtonRpt(0x101000, 0)) {
            m_nMCSelSelect -= 3;
            if (m_nMCSelSelect < 0) {
                m_nMCSelSelect += 3;
                if (!m_buttonTrg) {
                    break;
                }
                m_bCategorySelectMode = TRUE;
                setMapCardWin(0xA);
                setCursor2(m_nMCSelCateSelect, 5, TRUE);
                m_pHelpFnt->setText(m_pMainCmdName[1][1][6]);
                CSound::playSE(1, -1, 0x40);
                m_nSubStep = 4;
                break;
            }
            fgCursorMove = TRUE;
        } else if (_isButtonRpt(0x204000, 0)) {
            if (m_nMCSelSelect / 3 < (m_nMapKind[m_nMCSelCateSelect] - 1) / 3) {
                if ((m_nMCSelSelect += 3) >= m_nMapKind[m_nMCSelCateSelect]) {
                    m_nMCSelSelect = m_nMapKind[m_nMCSelCateSelect] - 1;
                }
            } else {
                if (!m_buttonTrg || m_nMapKind[m_nMCSelCateSelect] < 4) {
                    break;
                }
                if ((m_nMCSelSelect %= 3) >= m_nMapKind[m_nMCSelCateSelect]) {
                    m_nMCSelSelect = m_nMapKind[m_nMCSelCateSelect] - 1;
                }
                m_nMCSelSelectTop = 0;
            }
            fgCursorMove = TRUE;
        } else if (_isButtonRpt(0x408000, 0)) {
            m_nMCSelSelect--;
            if (m_nMCSelSelect < 0 || m_nMCSelSelect % 3 == 2) {
                m_nMCSelSelect++;
                if (!m_buttonTrg || m_nMCSelSelect == m_nMapKind[m_nMCSelCateSelect] - 1) {
                    break;
                }
                m_nMCSelSelect += 2;
                if (m_nMCSelSelect >= m_nMapKind[m_nMCSelCateSelect]) {
                    m_nMCSelSelect = m_nMapKind[m_nMCSelCateSelect] - 1;
                }
            }
            fgCursorMove = TRUE;
        } else if (_isButtonRpt(0x802000, 0)) {
            m_nMCSelSelect++;
            if (m_nMCSelSelect % 3 == 0 || m_nMCSelSelect >= m_nMapKind[m_nMCSelCateSelect]) {
                m_nMCSelSelect--;
                if (!m_buttonTrg || m_nMCSelSelect % 3 == 0) {
                    break;
                }
                m_nMCSelSelect -= m_nMCSelSelect % 3;
                fgCursorMove = TRUE;
            } else {
                fgCursorMove = TRUE;
            }
        }
        if (fgCursorMove) {
            CSound::playSE(0xE, -1, 0x40);
            setCursor2(m_nMCSelSelect, 1, TRUE);
            setMapCardWin(9);
        }
        moveCursor2();
        break;
    case 3:
        if (setMapCardWin(5)) {
            setMapCardWin(6);
            ret = -1;
            m_nSubStep = 999;
        }
        break;
    case 5: {
        if (_isTrgCANCEL(0)) {
            setCursor3(0, 2, TRUE);
            setCursor2(0, 4, TRUE);
            setCursor2(m_nMCSelSelect, 0, TRUE);
            CSound::playSE(4, -1, 0x40);
            m_nSubStep = 2;
        } else if (_isButtonTrg(0x40, 0)) {
            if (CCard::getMapTotal() < 0x15) {
                m_nError = 3;
                m_nErrorWinStep = 0;
                m_nSubStep = 6;
                break;
            }
            m_nError = 4;
            m_nSubStep = 7;
            m_nErrorWinStep = 0;
        } else if (_isButtonRpt(0x101000, 0)) {
            nNumSelect = m_nMCSelNumSelect - 1;
            if (nNumSelect < 0 || nNumSelect % 5 == 4) {
                if (!m_buttonTrg) {
                    break;
                }
                nNumSelect = searchNumExist(2, m_nMCSelNumSelect + 4, TRUE);
            } else {
                nNumSelect = searchNumExist(2, nNumSelect, TRUE);
                if (nNumSelect < 0) {
                    if (!m_buttonTrg) {
                        break;
                    }
                    nNumSelect = 9;
                    if (m_nMCSelNumSelect < 5) nNumSelect = 4;
                    nNumSelect = searchNumExist(2, nNumSelect, TRUE);
                }
            }
            fgCursorMove = TRUE;
        } else if (_isButtonRpt(0x204000, 0)) {
            nNumSelect = m_nMCSelNumSelect + 1;
            if (nNumSelect % 5 == 0) {
                if (!m_buttonTrg) {
                    break;
                }
                nNumSelect = searchNumExist(1, m_nMCSelNumSelect - 4, TRUE);
            } else {
                nNumSelect = searchNumExist(1, nNumSelect, TRUE);
                if (nNumSelect < 0) {
                    if (!m_buttonTrg) {
                        break;
                    }
                    nNumSelect = searchNumExist(1, m_nMCSelNumSelect < 5 ? 0 : 5, TRUE);
                }
            }
            fgCursorMove = TRUE;
        } else if (_isButtonRpt(0x408000, 0)) {
            nNumSelect = m_nMCSelNumSelect - 5;
            if (nNumSelect < 0) {
                if (!m_buttonTrg) {
                    break;
                }
                nNumSelect = searchNumExist(3, m_nMCSelNumSelect + 5, TRUE);
            } else {
                nNumSelect = searchNumExist(3, nNumSelect, TRUE);
                if (nNumSelect < 0) {
                    if (!m_buttonTrg) {
                        break;
                    }
                }
            }
            fgCursorMove = TRUE;
        } else if (_isButtonRpt(0x802000, 0)) {
            nNumSelect = m_nMCSelNumSelect + 5;
            if (nNumSelect >= 10) {
                if (!m_buttonTrg) {
                    break;
                }
                nNumSelect = searchNumExist(4, m_nMCSelNumSelect - 5, TRUE);
            } else {
                nNumSelect = searchNumExist(4, nNumSelect, TRUE);
                if (nNumSelect < 0) {
                    if (!m_buttonTrg) {
                        break;
                    }
                }
            }
            fgCursorMove = TRUE;
        }
        if (fgCursorMove) {
            if (nNumSelect < 0 || nNumSelect == m_nMCSelNumSelect) {
                break;
            }
            CSound::playSE(0xE, -1, 0x40);
            m_nMCSelNumSelect = nNumSelect;
            setCursor3(nNumSelect, 1, TRUE);
        }
        moveCursor2();
        moveCursor3();
        break;
    }
    case 4:
        if (_isTrgCANCEL(0)) {
            goto cancel;
        }
        if (_isButtonTrg(0x101000, 0)) {
            INT nKind = m_nMapKind[m_nMCSelCateSelect];
            if (!nKind) {
                CSound::playSE(5, -1, 0x40);
                break;
            }
            m_nMCSelSelectTop = nKind - 7;
            m_nMCSelSelect = nKind - 1;
            if ((m_nMCSelSelectTop -= m_nMCSelSelectTop % 3) < 0) {
                m_nMCSelSelectTop = 0;
            }
            setMapCardWin(9);
            goto enter;
        } else if (_isButtonTrg(0x204040, 0)) {
            if (!m_nMapKind[m_nMCSelCateSelect]) {
                CSound::playSE(5, -1, 0x40);
                break;
            }
            m_nMCSelSelect = 0;
        enter:
            m_bCategorySelectMode = FALSE;
            setMapCardWin(9);
            setCursor2(m_nMCSelSelect, 7, TRUE);
            setCursor2(m_nMCSelSelect, 0, TRUE);
            m_pHelpFnt->setText(CCtdMan::getText(0x500402));
            CSound::playSE(0xE, -1, 0x40);
            m_nSubStep = 2;
        } else if (_isButtonRpt(0x408000, 0)) {
            if (--m_nMCSelCateSelect < 0) {
                m_nMCSelCateSelect = 4;
            }
            fgCursorMove = TRUE;
        } else if (_isButtonRpt(0x802000, 0)) {
            if (++m_nMCSelCateSelect >= 5) {
                m_nMCSelCateSelect = 0;
            }
            fgCursorMove = TRUE;
        }
        if (fgCursorMove) {
            CSound::playSE(1, -1, 0x40);
            setMapCardWin(0xA);
            setCursor2(m_nMCSelCateSelect, 6, TRUE);
        }
        moveCursor2();
        break;
    case 6:
        if (deckCheckErrorWinMenu(FALSE)) {
            m_nSubStep = 5;
        }
        moveYNLight();
        break;
    case 7:
        if (deckCheckErrorWinMenu(TRUE)) {
            if (!m_nSelYesNo) {
                CCard::deleteCard(m_nMapKindTbl[m_nMCSelSelect], m_nMCSelNumSelect);
                initCardData(1);
                initCardMapKind(m_nMCSelCateSelect);
                adjustSelect(TRUE);
                setCursor2(0, 4, TRUE);
                setCursor3(0, 2, TRUE);
                setCursor2(m_nMCSelSelect, 0, TRUE);
                if (!m_nMapKind[m_nMCSelCateSelect]) {
                    m_bCategorySelectMode = TRUE;
                    setCursor2(m_nMCSelCateSelect, 5, TRUE);
                    m_nSubStep = 4;
                } else {
                    m_nSubStep = 2;
                }
                setMapCardWin(9);
            } else {
                m_nSubStep = 5;
            }
        }
        moveYNLight();
        break;
    }
    return ret;
}

INT CCamp::statusWinMenu() {
    BOOL bChange = FALSE;
    INT ret = 0;
    INT nOld;

    switch (m_nSubStep) {
    case 0:
        m_bCategorySelectMode = FALSE;
        setStatusWin(0);
        setStatusWin(7);
        setStatusWin(0xA);
        if (!m_nStatusTech[0]) {
            m_pStatusBC->enableDisp(0, FALSE);
            m_pStatusBC->enableDisp(1, FALSE);
        }
        setButtonHelp(1, 0, 6);
        m_nSubStep = 1;
        break;
    case 1:
        if (setStatusWin(1)) {
            if (!m_nStatusTech[0]) {
                m_bCategorySelectMode = TRUE;
                m_pStatusBC->enableDisp(1, TRUE);
                m_pStatusBC->setDisp(FALSE, TRUE);
                setStatusWin(0xA);
                m_nSubStep = 4;
            } else {
                m_nSubStep = 2;
            }
            ret = 1;
        }
        break;
    case 2:
        setStatusWin(3);
        if (_isButtonTrg(0x20, 0)) {
        cancel: /* MATCH: label before the shared flag store (ret = 1 fills the setStatusWin delay slot) */
            m_bCategorySelectMode = FALSE;
            ret = 1;
            setStatusWin(4);
            setButtonHelp(4, 0, 0);
            CSound::playSE(4, -1, 0x40);
            m_nSubStep = 3;
        } else if (_isButtonTrg(0x40, 0)) {
            new KH_AT("(3237)") CTechHelp(this, m_techCateTbl[m_nStatusTechSelect][0]);
            m_nSubStep = 5;
            break;
        } else if (_isButtonRpt(0x101000, 0)) {
            nOld = m_nStatusTechSelect;
            if (--m_nStatusTechSelect < 0) {
                if (!m_buttonTrg) {
                    m_nStatusTechSelect = nOld;
                    break;
                }
                m_nStatusTechSelect = m_nStatusTechSelectMax - 1;
                m_pStatusBC->enableDisp(0, FALSE);
                m_pStatusBC->setDisp(FALSE, TRUE);
                m_bCategorySelectMode = TRUE;
                setStatusWin(0xA);
                m_pHelpFnt->setText(m_pMainCmdName[1][1][6]);
                CSound::playSE(1, -1, 0x40);
                m_nSubStep = 4;
                break;
            }
            bChange = TRUE;
        } else if (_isButtonRpt(0x204000, 0)) {
            nOld = m_nStatusTechSelect;
            if (++m_nStatusTechSelect >= m_nStatusTechSelectMax) {
                if (!m_buttonTrg) {
                    m_nStatusTechSelect = nOld;
                    break;
                }
                m_nStatusTechSelect = 0;
            }
            bChange = TRUE;
        } else if (CPadHandler::isButtonRpt(4)) {
            nOld = m_nStatusTechSelect;
            if ((m_nStatusTechSelect -= 8) < 0) {
                if (m_nStatusTechSelect < -7) {
                    if (!CPadHandler::isButtonTrg(4)) {
                        m_nStatusTechSelect += 8;
                        break;
                    }
                    m_nStatusTechSelect = m_nStatusTechSelectMax - 1;
                } else {
                    m_nStatusTechSelect = 0;
                }
            } else if ((m_nStatusTechSelectTop -= 8) < 0) {
                m_nStatusTechSelectTop = 0;
            }
            bChange = TRUE;
        } else if (CPadHandler::isButtonRpt(8)) {
            nOld = m_nStatusTechSelect;
            if ((m_nStatusTechSelect += 8) >= m_nStatusTechSelectMax) {
                if (m_nStatusTechSelect >= m_nStatusTechSelectMax + 7) {
                    if (!CPadHandler::isButtonTrg(8)) {
                        m_nStatusTechSelect -= 8;
                        break;
                    }
                    m_nStatusTechSelect = 0;
                } else {
                    m_nStatusTechSelect = m_nStatusTechSelectMax - 1;
                }
            } else if ((m_nStatusTechSelectTop += 8) >= m_nStatusTechSelectMax - 8) {
                m_nStatusTechSelectTop = m_nStatusTechSelectMax - 8;
            }
            bChange = TRUE;
        }
        if (bChange && nOld != m_nStatusTechSelect) {
            CSound::playSE(1, -1, 0x40);
            setStatusWin(9);
        }
        break;
    case 3:
        if (setStatusWin(5)) {
            setStatusWin(6);
            ret = -1;
            m_nSubStep = 999;
        }
        break;
    case 5:
        if (CTechHelp::isEnd()) {
            m_nSubStep = 2;
        }
        break;
    case 4:
        if (_isTrgCANCEL(0)) {
            goto cancel;
        }
        if (_isButtonTrg(0x101000, 0)) {
            if (!m_nStatusTech[m_nStatusTechCateSelect]) {
                CSound::playSE(5, -1, 0x40);
                break;
            }
            m_nStatusTechSelect = m_nStatusTech[m_nStatusTechCateSelect] - 1;
            goto enter;
        } else if (_isButtonTrg(0x204040, 0)) {
            if (!m_nStatusTech[m_nStatusTechCateSelect]) {
                CSound::playSE(5, -1, 0x40);
                break;
            }
            m_nStatusTechSelect = 0;
        enter:
            m_bCategorySelectMode = FALSE;
            setStatusWin(9);
            m_pStatusBC->enableDisp(0, TRUE);
            m_pStatusBC->setDisp(TRUE, TRUE);
            m_pHelpFnt->setText(CCtdMan::getText(0x500417));
            CSound::playSE(1, -1, 0x40);
            m_nSubStep = 2;
        } else if (_isButtonRpt(0x408000, 0)) {
            if (--m_nStatusTechCateSelect < 0) {
                m_nStatusTechCateSelect = 4 - m_bPlayerKind;
            }
            bChange = TRUE;
        } else if (_isButtonRpt(0x802000, 0)) {
            if (++m_nStatusTechCateSelect >= 5 - m_bPlayerKind) {
                m_nStatusTechCateSelect = 0;
            }
            bChange = TRUE;
        }
        if (bChange) {
            CSound::playSE(1, -1, 0x40);
            setStatusWin(0xA);
        }
        break;
    }
    return ret;
}

INT CCamp::configWinMenu() {
    BOOL bChange = FALSE;
    INT ret = 0;

    switch (m_nSubStep) {
    case 0:
        setConfigWin(0);
        setConfigWin(7);
        setConfigWin(9);
        m_nSubStep = 1;
        break;
    case 1:
        if (setConfigWin(1)) {
            ret = 1;
            m_nSubStep = 2;
        }
        break;
    case 2:
        setConfigWin(3);
        if (_isButtonTrg(0x20, 0)) {
            setConfigWin(4);
            ret = 1;
            CSound::playSE(4, -1, 0x40);
            m_nSubStep = 3;
        } else if (_isButtonRpt(0x101000, 0)) {
            if (--m_nConfigSelect < 0) {
                m_nConfigSelect = 6;
            }
            bChange = TRUE;
        } else if (_isButtonRpt(0x204000, 0)) {
            if (++m_nConfigSelect >= 7) {
                m_nConfigSelect = 0;
            }
            bChange = TRUE;
        }
        if (!bChange && m_nConfigSelect < 6) {
            if (_isButtonRpt(0x408000, 0)) {
                if (--m_nConfigItemSelect[m_nConfigSelect] < 0) {
                    m_nConfigItemSelect[m_nConfigSelect] = m_nConfigSelect == 4 ? 2 : 1;
                }
                bChange = TRUE;
            } else if (_isButtonRpt(0x802000, 0)) {
                CHAR n = ++m_nConfigItemSelect[m_nConfigSelect];
                if (m_nConfigSelect == 4 ? n >= 3 : n >= 2) {
                    m_nConfigItemSelect[m_nConfigSelect] = 0;
                }
                bChange = TRUE;
            }
        }
        if (bChange) {
            CSound::playSE(1, -1, 0x40);
            setConfigWin(9);
        }
        break;
    case 3:
        if (setConfigWin(5)) {
            setConfigWin(6);
            ret = -1;
            m_nSubStep = 999;
        }
        break;
    }
    return ret;
}

INT CCamp::deckEditMenu() {
    BOOL ret = FALSE;
    BOOL fgCursorMove = FALSE;
    INT fgForceChangeInOut = 0;
    INT nNumSelect;
    CARD card;
    BOOL fgInOutChange;

    switch (m_nSubStep) {
    case 0:
        if (m_bPlayerKind) {
            m_nCardInOut = 1;
        }
        if (!m_nCardInOut) {
            if (!m_nKind[m_nKindCategorySelect]) {
                m_nKindSelect = m_nKindSelectTop = 0;
                m_bCategorySelectMode = TRUE;
                setCursor2(0, 0, FALSE);
                setCursor2(m_nKindCategorySelect, 5, FALSE);
                moveCursor2();
                m_nSubStep = 5;
            } else {
                setCursor2(m_nKindSelect, 0, FALSE);
                moveCursor2();
                m_nSubStep = 1;
            }
        } else {
            setCursor4(m_nDeckSelect, 0);
            moveCursor4();
            m_nSubStep = 4;
        }
        break;
    case 1:
        if (_isTrgCANCEL(0)) {
        cancel:
            setCursor2(0, 2, FALSE);
            ret = TRUE;
            if (m_bShortCutMode) {
                break;
            }
            setDeckInfo(4);
            setMainCmdHist(4, 1);
            CSound::playSE(4, -1, 0x40);
            m_nStep = 0x27;
        } else if (_isButtonTrg(0x40, 0)) {
            if (!m_nKind[m_nKindCategorySelect] || !m_kindTbl[m_nKindSelect]) {
                CSound::playSE(5, -1, 0x40);
                break;
            }
            CSound::playSE(2, -1, 0x40);
            m_nNumSelect = searchNumExist(0, 0, FALSE);
            setCursor2(0, 3, FALSE);
            setCursor3(m_nNumSelect, 0, FALSE);
            moveCursor3();
            setDeckInfo(0xD);
            m_nSubStep = 2;
        } else if (_isButtonRpt(0x101000, 0)) {
            m_nKindSelect -= 3;
            if (m_nKindSelect < 0) {
                m_nKindSelect += 3;
                if (!m_buttonTrg) {
                    break;
                }
                m_nKindSelect = m_nKindSelectTop = 0;
                m_bCategorySelectMode = TRUE;
                setDeckInfo(0xC);
                setCursor2(m_nKindCategorySelect, 5, FALSE);
                CSound::playSE(1, -1, 0x40);
                m_nSubStep = 5;
            } else {
                fgCursorMove = TRUE;
            }
        } else if (_isButtonRpt(0x204000, 0)) {
            if (m_nKindSelect / 3 < (m_nKind[m_nKindCategorySelect] - 1) / 3) {
                m_nKindSelect += 3;
            } else {
                if (!m_buttonTrg || m_nKind[m_nKindCategorySelect] < 4) {
                    break;
                }
                m_nKindSelect %= 3;
                m_nKindSelectTop = 0;
            }
            if (m_nKindSelect >= m_nKind[m_nKindCategorySelect]) {
                m_nKindSelect = m_nKind[m_nKindCategorySelect] - 1;
            }
            fgCursorMove = TRUE;
        } else if (_isButtonRpt(0x408000, 0)) {
            m_nKindSelect--;
            if (m_nKindSelect < 0 || m_nKindSelect % 3 == 2) {
                m_nKindSelect++;
                if (!m_buttonTrg || m_nKindSelect == m_nKind[m_nKindCategorySelect] - 1) {
                    break;
                }
                m_nKindSelect += 2;
                if (m_nKindSelect >= m_nKind[m_nKindCategorySelect]) {
                    m_nKindSelect = m_nKind[m_nKindCategorySelect] - 1;
                }
            }
            fgCursorMove = TRUE;
        } else if (_isButtonRpt(0x802000, 0)) {
            m_nKindSelect++;
            if (m_nKindSelect % 3 == 0 || m_nKindSelect >= m_nKind[m_nKindCategorySelect]) {
                m_nKindSelect--;
                if (!m_buttonTrg || m_nKindSelect % 3 == 0) {
                    break;
                }
                m_nKindSelect -= m_nKindSelect % 3;
            }
            fgCursorMove = TRUE;
        }
        if (fgCursorMove) {
            if (m_nKindSelect < 0) {
                m_nKindSelect = 0;
                break;
            }
            CSound::playSE(0xE, -1, 0x40);
            setCursor2(m_nKindSelect, 1, FALSE);
            setDeckCard(0x12);
            setDeckInfo(0xC);
        }
        moveCursor2();
        break;
    case 2:
        if (_isTrgCANCEL(0)) {
            setCursor3(0, 2, FALSE);
            setCursor2(0, 4, FALSE);
            setCursor2(m_nKindSelect, 0, FALSE);
            m_nNumSelect = 1;
            setDeckInfo(0x10);
            CSound::playSE(4, -1, 0x40);
            m_nSubStep = 1;
        } else if (_isButtonTrg(0x40, 0)) {
            if (m_bShortCutMode) {
                if ((BYTE)m_kindTbl[m_nKindSelect] < 0x19 && CCard::getAttackCardMax(0) < 2) {
                    m_nError = 3;
                    m_nStep = 0x3F;
                    m_nErrorWinStep = 0;
                    break;
                }
                m_nError = 4;
                m_nSubStep = 8;
                m_nErrorWinStep = 0;
            } else {
                setCursor3(0, 3, FALSE);
                setCursor4(m_nDeckSelect, 0);
                moveCursor4();
                CSound::playSE(2, -1, 0x40);
                m_nSubStep = 3;
            }
        } else if (_isButtonRpt(0x101000, 0)) {
            nNumSelect = m_nNumSelect - 1;
            if (nNumSelect < 0 || nNumSelect % 5 == 4) {
                if (!m_buttonTrg) {
                    break;
                }
                nNumSelect = searchNumExist(2, m_nNumSelect + 4, FALSE);
            } else {
                nNumSelect = searchNumExist(2, nNumSelect, FALSE);
                if (nNumSelect < 0) {
                    if (!m_buttonTrg) {
                        break;
                    }
                    nNumSelect = 9;
                    if (m_nNumSelect < 5) nNumSelect = 4;
                    nNumSelect = searchNumExist(2, nNumSelect, FALSE);
                }
            }
            fgCursorMove = TRUE;
        } else if (_isButtonRpt(0x204000, 0)) {
            nNumSelect = m_nNumSelect + 1;
            if (nNumSelect % 5 == 0) {
                if (!m_buttonTrg) {
                    break;
                }
                nNumSelect = searchNumExist(1, m_nNumSelect - 4, FALSE);
            } else {
                nNumSelect = searchNumExist(1, nNumSelect, FALSE);
                if (nNumSelect < 0) {
                    if (!m_buttonTrg) {
                        break;
                    }
                    nNumSelect = searchNumExist(1, m_nNumSelect < 5 ? 0 : 5, FALSE);
                }
            }
            fgCursorMove = TRUE;
        } else if (_isButtonRpt(0x408000, 0)) {
            nNumSelect = m_nNumSelect - 5;
            if (nNumSelect < 0) {
                if (!m_buttonTrg) {
                    break;
                }
                nNumSelect = searchNumExist(3, m_nNumSelect + 5, FALSE);
            } else {
                nNumSelect = searchNumExist(3, nNumSelect, FALSE);
                if (nNumSelect < 0) {
                    if (!m_buttonTrg) {
                        break;
                    }
                }
            }
            fgCursorMove = TRUE;
        } else if (_isButtonRpt(0x802000, 0)) {
            nNumSelect = m_nNumSelect + 5;
            if (nNumSelect >= 10) {
                if (!m_buttonTrg) {
                    break;
                }
                nNumSelect = searchNumExist(4, m_nNumSelect - 5, FALSE);
            } else {
                nNumSelect = searchNumExist(4, nNumSelect, FALSE);
                if (nNumSelect < 0) {
                    if (!m_buttonTrg) {
                        break;
                    }
                }
            }
            fgCursorMove = TRUE;
        }
        if (fgCursorMove) {
            if (nNumSelect < 0 || nNumSelect == m_nNumSelect) {
                break;
            }
            CSound::playSE(0xE, -1, 0x40);
            m_nNumSelect = nNumSelect;
            setCursor3(nNumSelect, 1, FALSE);
            setDeckInfo(0xD);
        }
        moveCursor2();
        moveCursor3();
        break;
    case 3:
        if (_isTrgCANCEL(0)) {
            setCursor4(0, 2);
            setCursor3(0, 4, FALSE);
            setCursor3(m_nNumSelect, 0, FALSE);
            CSound::playSE(4, -1, 0x40);
            m_nSubStep = 2;
        } else if (_isButtonTrg(0x40, 0)) {
            if (m_nDeck[0] >= 99) {
                m_nError = 5;
                m_fgPadDisable = 2;
                m_nSubStep = 9;
                m_nErrorWinStep = 0;
                break;
            }
            CARD card;
            card.init(m_kindTbl[m_nKindSelect], m_nNumSelect);
            CCard::insertDeck(card, m_nDeckNoSelect, m_nDeckSelect, FALSE);
#line 3327
            initCardData(0);
            initCardKind(m_nDeckNoSelect, m_nKindCategorySelect, FALSE);
            initDeck(m_nDeckNoSelect, 0);
            adjustSelect(FALSE);
            setDeckCard(0x12);
            setDeckInfo(0xC);
            setDeckDeck(0x12);
            setCursor4(0, 2);
            setCursor3(0, 2, FALSE);
            setCursor2(0, 4, FALSE);
            setCursor2(m_nKindSelect, 0, FALSE);
            CSound::playSE(2, -1, 0x40);
            if (!m_nKind[m_nKindCategorySelect]) {
                m_nKindSelect = m_nKindSelectTop = 0;
                m_bCategorySelectMode = TRUE;
                setCursor2(m_nKindCategorySelect, 5, FALSE);
                m_nSubStep = 5;
            } else {
                m_nSubStep = 1;
            }
        } else if (_isButtonRpt(0x101000, 0)) {
            m_nDeckSelect -= 3;
            if (m_nDeckSelect < 0) {
                m_nDeckSelect += 3;
            } else {
                fgCursorMove = TRUE;
            }
        } else if (_isButtonRpt(0x204000, 0)) {
            if (m_nDeckSelect / 3 < 32) {
                m_nDeckSelect += 3;
                if (m_nDeckSelect >= 99) {
                    m_nDeckSelect = 98;
                }
                fgCursorMove = TRUE;
            }
        } else if (_isButtonRpt(0x408000, 0)) {
            m_nDeckSelect--;
            if (m_nDeckSelect < 0 || m_nDeckSelect % 3 == 2) {
                m_nDeckSelect++;
            } else {
                fgCursorMove = TRUE;
            }
        } else if (_isButtonRpt(0x802000, 0)) {
            m_nDeckSelect++;
            if (m_nDeckSelect % 3 == 0 || m_nDeckSelect >= 99) {
                m_nDeckSelect--;
            } else {
                fgCursorMove = TRUE;
            }
        }
        if (fgCursorMove) {
            CSound::playSE(0xE, -1, 0x40);
            setCursor4(m_nDeckSelect, 1);
            setDeckDeck(0x12);
        }
        moveCursor4();
        break;
    case 4:
        if (_isTrgCANCEL(0)) {
            ret = TRUE;
            setCursor4(0, 2);
            if (m_bShortCutMode) {
                break;
            }
            setDeckInfo(4);
            m_nCardInOut = 0;
            setDeckDeck(3);
            setMainCmdHist(4, 1);
            CSound::playSE(4, -1, 0x40);
            m_nStep = 0x27;
        } else if (_isButtonTrg(0x40, 0)) {
            if (m_bShortCutMode) {
                if (!setShortCut(m_nDeckSelect)) {
                    CSound::playSE(5, -1, 0x40);
                    break;
                }
                initCardData(0);
                initCardKind(m_nDeckNoSelect, m_nKindCategorySelect, FALSE);
                initDeck(m_nDeckNoSelect, m_nDeckCategorySelect);
            } else {
                if (m_bPlayerKind) {
                    break;
                }
                if (!m_nDeck[m_nDeckCategorySelect]) {
                    CSound::playSE(5, -1, 0x40);
                    break;
                }
                if (!removeDeck(m_nDeckSelect)) {
                    CSound::playSE(5, -1, 0x40);
                    break;
                }
                initCardData(0);
                initCardKind(m_nDeckNoSelect, m_nKindCategorySelect, FALSE);
                initDeck(m_nDeckNoSelect, m_nDeckCategorySelect);
                adjustSelect(FALSE);
                setCursor4(m_nDeckSelect, 0);
                setDeckCard(0x12);
                setDeckInfo(0xC);
            }
            setDeckDeck(0x12);
            CSound::playSE(2, -1, 0x40);
        } else if (_isButtonRpt(0x101000, 0)) {
            m_nDeckSelect -= 3;
            if (m_nDeckSelect < 0) {
                m_nDeckSelect += 3;
                if (!m_buttonTrg) {
                    break;
                }
                m_nDeckSelect = m_nDeckSelect % 3 + 96;
                m_nDeckSelectTop = 90;
            }
            fgCursorMove = TRUE;
        } else if (_isButtonRpt(0x204000, 0)) {
            if (m_nDeckSelect / 3 < 32) {
                m_nDeckSelect += 3;
                if (m_nDeckSelect >= 99) {
                    m_nDeckSelect = 98;
                }
            } else {
                if (!m_buttonTrg) {
                    break;
                }
                m_nDeckSelect %= 3;
                m_nDeckSelectTop = 0;
            }
            fgCursorMove = TRUE;
        } else if (_isButtonRpt(0x408000, 0)) {
            m_nDeckSelect--;
            if (m_nDeckSelect < 0 || m_nDeckSelect % 3 == 2) {
                m_nDeckSelect++;
                if (!m_buttonTrg) {
                    break;
                }
                m_nDeckSelect += 2 - m_nDeckSelect % 3;
            }
            fgCursorMove = TRUE;
        } else if (_isButtonRpt(0x802000, 0)) {
            m_nDeckSelect++;
            if (m_nDeckSelect % 3 == 0 || m_nDeckSelect >= 99) {
                m_nDeckSelect--;
                if (!m_buttonTrg) {
                    break;
                }
                m_nDeckSelect -= m_nDeckSelect % 3;
            }
            fgCursorMove = TRUE;
        }
        if (fgCursorMove) {
            CSound::playSE(0xE, -1, 0x40);
            setCursor4(m_nDeckSelect, 1);
            setDeckDeck(0x12);
            setDeckInfo(0xC);
        }
        moveCursor4();
        break;
    case 6:
        setCursor4Hist(m_nDeckSelectHist, 0);
        setCursor4Hist(m_nDeckSelectHist, 1);
        m_nSubStep = 7;
        break;
    case 7:
        if (_isTrgCANCEL(0)) {
            m_bCardPermuteMode = FALSE;
            setDeckInfo(!m_nCardInOut ? 0xA : 0xB);
            setCursor4Hist(0, 2);
            CSound::playSE(4, -1, 0x40);
            m_nSubStep = !m_nCardInOut ? 3 : 4;
        } else if (_isButtonTrg(0x50, 0)) {
            if (m_nDeckSelect == m_nDeckSelectHist) {
                CSound::playSE(5, -1, 0x40);
                break;
            }
            m_bCardPermuteMode = FALSE;
            card = m_pDeck[m_nDeckSelectHist];
            removeDeck(m_nDeckSelectHist);
            CCard::insertDeck(card, m_nDeckNoSelect, m_nDeckSelect, m_nDeckSelectHist == m_nSCIndex[m_nDeckCategorySelect]);
            initCardData(0);
            initCardKind(m_nDeckNoSelect, m_nKindCategorySelect, FALSE);
            initDeck(m_nDeckNoSelect, m_nDeckCategorySelect);
            adjustSelect(FALSE);
            setCursor4(m_nDeckSelect, 0);
            setDeckCard(0x12);
            setDeckInfo(0xC);
            setDeckDeck(0x12);
            CSound::playSE(0xF, -1, 0x40);
            setDeckInfo(!m_nCardInOut ? 0xA : 0xB);
            setCursor4Hist(0, 2);
            m_nSubStep = !m_nCardInOut ? 1 : 4;
        } else if (_isButtonRpt(0x101000, 0)) {
            m_nDeckSelect -= 3;
            if (m_nDeckSelect < 0) {
                m_nDeckSelect += 3;
            } else {
                fgCursorMove = TRUE;
            }
        } else if (_isButtonRpt(0x204000, 0)) {
            if (m_nDeckSelect / 3 < 32) {
                m_nDeckSelect += 3;
                if (m_nDeckSelect >= 99) {
                    m_nDeckSelect = 98;
                }
                fgCursorMove = TRUE;
            }
        } else if (_isButtonRpt(0x408000, 0)) {
            m_nDeckSelect--;
            if (m_nDeckSelect < 0 || m_nDeckSelect % 3 == 2) {
                m_nDeckSelect++;
            } else {
                fgCursorMove = TRUE;
            }
        } else if (_isButtonRpt(0x802000, 0)) {
            m_nDeckSelect++;
            if (m_nDeckSelect % 3 == 0 || m_nDeckSelect >= 99) {
                m_nDeckSelect--;
            } else {
                fgCursorMove = TRUE;
            }
        }
        if (fgCursorMove) {
            CSound::playSE(0xE, -1, 0x40);
            setCursor4(m_nDeckSelect, 1);
            setDeckDeck(0x12);
            setDeckInfo(0xC);
            setCursor4Hist(m_nDeckSelectHist, 1);
        }
        moveCursor4();
        break;
    case 5:
        if (_isTrgCANCEL(0)) {
            m_bCategorySelectMode = FALSE;
            goto cancel;
        }
        if (_isButtonTrg(0x101000, 0)) {
            if (!m_nKind[m_nKindCategorySelect]) {
                CSound::playSE(5, -1, 0x40);
                break;
            }
            m_nKindSelect = m_nKind[m_nKindCategorySelect] - 1;
            m_nKindSelectTop = m_nKindSelect - 6;
            if ((m_nKindSelectTop -= m_nKindSelectTop % 3) < 0) {
                m_nKindSelectTop = 0;
            }
            setDeckCard(0x12);
            goto enter;
        } else if (_isButtonTrg(0x204040, 0)) {
            if (!m_nKind[m_nKindCategorySelect]) {
                CSound::playSE(5, -1, 0x40);
                break;
            }
            m_nKindSelect = m_nKindSelectTop = 0;
        enter:
            m_bCategorySelectMode = FALSE;
            setDeckInfo(0xC);
            setCursor2(m_nKindSelect, 7, FALSE);
            setCursor2(m_nKindSelect, 0, FALSE);
            CSound::playSE(0xE, -1, 0x40);
            m_nSubStep = 1;
        } else if (_isButtonRpt(0x408000, 0)) {
            if (--m_nKindCategorySelect < 0) {
                m_nKindCategorySelect = 4;
            }
            fgCursorMove = TRUE;
        } else if (_isButtonRpt(0x802000, 0)) {
            if (++m_nKindCategorySelect >= 5) {
                m_nKindCategorySelect = 0;
            }
            fgCursorMove = TRUE;
        }
        if (fgCursorMove) {
            CSound::playSE(1, -1, 0x40);
            setDeckCard(0x14);
            setDeckInfo(0xC);
            setCursor2(m_nKindCategorySelect, 6, FALSE);
        }
        moveCursor2();
        break;
    case 8:
        if (deckCheckErrorWinMenu(TRUE)) {
            if (!m_nSelYesNo) {
                CCard::deleteCard(m_kindTbl[m_nKindSelect], m_nNumSelect);
                initCardData(0);
                initCardKind(0, m_nKindCategorySelect, TRUE);
                adjustSelect(FALSE);
                setDeckCard(0x12);
                setDeckInfo(0xC);
                setCursor2(0, 4, FALSE);
                setCursor3(0, 2, FALSE);
                setCursor2(m_nKindSelect, 0, FALSE);
                if (!m_nKind[m_nKindCategorySelect]) {
                    m_bCategorySelectMode = TRUE;
                    setCursor2(m_nKindCategorySelect, 5, FALSE);
                    m_nSubStep = 5;
                } else {
                    m_nSubStep = 1;
                }
            } else {
                m_nSubStep = 2;
            }
        }
        moveYNLight();
        break;
    case 9:
        m_fgPadDisable = 2;
        if (deckCheckErrorWinMenu(FALSE)) {
            m_nSubStep = 3;
        }
        moveYNLight();
        break;
    }
    if (m_bShortCutMode || m_bPlayerKind || m_nStep != 0x26) {
        return ret;
    }
    while (!fgCursorMove) {
        fgInOutChange = FALSE;
        if (_isButtonTrg(4, 0) || fgForceChangeInOut == 2) {
            if (!m_nCardInOut) {
                break;
            }
            if (!m_nKind[0]) {
                CSound::playSE(5, -1, 0x40);
                break;
            }
            m_nCardInOut = 0;
            initCardData(0);
            initCardKind(m_nDeckNoSelect, m_nKindCategorySelect, FALSE);
            initDeck(m_nDeckNoSelect, 0);
            setDeckCard(0x14);
            setDeckInfo(3);
            setDeckInfo(0xA);
            setDeckDeck(0x14);
            setCursor4(0, 2);
            setCursor4Hist(0, 2);
            setButtonHelp(0xA, 0, 0);
            if (!m_nKind[m_nKindCategorySelect]) {
                m_nKindSelect = m_nKindSelectTop = 0;
            } else if (m_nKindSelect >= m_nKind[m_nKindCategorySelect]) {
                m_nKindSelect = m_nKind[m_nKindCategorySelect] - 1;
                m_nKindSelectTop = m_nKindSelect - 6;
                if ((m_nKindSelectTop -= m_nKindSelectTop % 3) < 0) {
                    m_nKindSelectTop = 0;
                }
            }
            fgInOutChange = TRUE;
            CSound::playSE(3, -1, 0x40);
        } else if (_isButtonTrg(8, 0) || fgForceChangeInOut == 1) {
            if (m_nCardInOut) {
                break;
            }
            if (!m_nDeck[0]) {
                CSound::playSE(5, -1, 0x40);
                break;
            }
            m_nCardInOut = 1;
            setDeckInfo(3);
            setDeckInfo(0xB);
            setCursor2(0, 2, FALSE);
            if (m_pCursor3Ctrl) {
                setCursor3(0, 2, FALSE);
            }
            setButtonHelp(0xA, 0, 1);
            fgInOutChange = TRUE;
            CSound::playSE(3, -1, 0x40);
        }
        if (fgInOutChange) {
            m_bCategorySelectMode = FALSE;
            m_bCardPermuteMode = FALSE;
            setDeckCard(0xB);
            setDeckInfo(0xC);
            setDeckDeck(3);
            m_nSubStep = 0;
        }
        break;
    }
    return ret;
}
void CCamp::setDeckCard(INT nSetCmd) {
    static char* pStrNum[2] = { "%03d/%03d", "%03d" };
    FVECTOR vec;
    FVECTOR vec2;
    C2DSeqCtrl* p2DSeqCtrl;
    C2DCard* p2DCard;
    G_RGBA rgba;
    CARD card;
    FLOAT fRatioHeight;
    FLOAT fRatioPosY;
    char chrTmp[64];
    static USHORT tabSeqTbl[10] = { 231, 232, 233, 234, 239, 240, 237, 238, 235, 236 };
    static USHORT scrollBarSeqTbl[3] = { 288, 285, 291 };

    if (m_bPlayerKind) {
        return;
    }
    switch (nSetCmd) {
    case 1:
        if (!m_pDeckCardCtrl[0]) {
            p2DSeqCtrl = m_pDeckCardCtrl[0] = new KH_AT("(4317)") C2DSeqCtrl;
            p2DSeqCtrl->setLayer(0x11);
            p2DSeqCtrl->enableColorCtrl(TRUE);
            C2DMan::getLayPosition(m_pLadName, 5, 8, vec, 0);
            p2DSeqCtrl->setPos(vec);
            p2DSeqCtrl->addSeq(m_pSeqName, 0xA5);
            p2DSeqCtrl = m_pDeckCardCtrl[2] = new KH_AT("(4326)") C2DSeqCtrl;
            p2DSeqCtrl->enableColorCtrl(TRUE);
            p2DSeqCtrl->setLayer(0x10);
            C2DMan::getLayPosition(m_pLadName, 7, 0, vec, 0);
            p2DSeqCtrl->addSeqShift(m_pSeqName, 0xD1, vec.x, vec.y);
            p2DSeqCtrl->setParent(m_pDeckCardCtrl[0]);
            rgba.col = 0x807C7C7C;
            m_pDeckCardFnt[0] = new KH_AT("(4334)") C2DFnt(0, 0, &rgba, 0xC, NULL, 0x40);
            p2DSeqCtrl->addFntShift(m_pSeqName, m_pDeckCardFnt[0], 16.0f, 3.0f);
            rgba.col = 0x80007878;
            for (INT i = 0; i < 4; i++) {
                m_pDeckCardFnt[i + 1] = new KH_AT("(4341)") C2DFnt(0, 0, &rgba, 0xC, NULL, 0x40);
                p2DSeqCtrl->addFntShift(m_pSeqName, m_pDeckCardFnt[i + 1], i * 38.0f + -62.0f, 18.0f);
            }
            p2DSeqCtrl = m_pDeckCardCtrl[1] = new KH_AT("(4347)") C2DSeqCtrl;
            p2DSeqCtrl->setLayer(0x10);
            p2DSeqCtrl->enableColorCtrl(TRUE);
            p2DSeqCtrl->addSeq(m_pSeqName, 0xE8);
            p2DSeqCtrl->setParent(m_pDeckCardCtrl[0]);
            for (INT i = 0; i < 3; i++) {
                for (INT j = 0; j < 3; j++) {
                    p2DCard = m_pDeckCardCard[i * 3 + j] = new KH_AT("(4357)") C2DCard;
                    p2DCard->setParent(m_pDeckCardCtrl[0]);
                    p2DCard->setLayer(0x11);
                    p2DCard->enableColorCtrl(TRUE);
                    C2DMan::getLayPosition(m_pLadName, 7, i * 3 + j + 3, vec, 0);
                    p2DCard->setLocalPos(vec);
                    p2DCard->addSeq(m_pSeqName, 0xB6);
                }
            }
            m_pDeckCardScrollBar = new KH_AT("(4369)") CScrollBar;
            vec.set(68.0f, -56.0f, 0.0f, 1.0f);
            m_pDeckCardScrollBar->init(m_pSeqName, 0x11, scrollBarSeqTbl, vec, 118.0f, m_pDeckCardCtrl[0]);
        } else {
            m_pDeckCardCtrl[0]->replaceSeq(0, m_pSeqName, 0xA5);
        }
        break;
    case 3:
        m_pDeckCardCtrl[0]->replaceSeq(0, m_pSeqName, 0xA6);
        C2DMan::getLayPosition(m_pLadName, 7, 0, vec, 0);
        m_pDeckCardCtrl[2]->replaceSeqShift(0, m_pSeqName, 0xD1, vec.x, vec.y);
        m_pDeckCardCtrl[1]->replaceSeq(0, m_pSeqName, tabSeqTbl[m_nKindCategorySelect * 2 + 1]);
        break;
    case 4:
        m_pDeckCardCtrl[0]->replaceSeq(0, m_pSeqName, 0xA7);
        break;
    case 6:
        for (INT i = 0; i < 3; i++) {
            m_pDeckCardCtrl[i]->enableDisp(FALSE);
        }
        for (INT i = 0; i < 9; i++) {
            m_pDeckCardCard[i]->enableDisp(FALSE);
        }
        m_pDeckCardScrollBar->enableDisp(FALSE);
        break;
    case 7:
        for (INT i = 0; i < 3; i++) {
            m_pDeckCardCtrl[i]->enableDisp(TRUE);
        }
        for (INT i = 0; i < 9; i++) {
            m_pDeckCardCard[i]->enableDisp(TRUE);
        }
        m_pDeckCardScrollBar->enableDisp(TRUE);
        break;
    case 10:
        m_pDeckCardCtrl[0]->replaceSeq(0, m_pSeqName, 0xA9);
        break;
    case 11:
        m_pDeckCardCtrl[0]->replaceSeq(0, m_pSeqName, !m_nCardInOut ? 0xA2 : 0xA3);
        C2DMan::getLayPosition(m_pLadName, 7, 0, vec, 0);
        m_pDeckCardCtrl[2]->replaceSeqShift(0, m_pSeqName, !m_nCardInOut ? 0xD0 : 0xD1, vec.x, vec.y);
        m_pDeckCardCtrl[1]->replaceSeq(0, m_pSeqName, tabSeqTbl[m_nKindCategorySelect * 2 + m_nCardInOut]);
        break;
    case 12:
        m_pDeckCardCtrl[0]->replaceSeq(0, m_pSeqName, 0xA8);
        C2DMan::getLayPosition(m_pLadName, 7, 0, vec, 0);
        m_pDeckCardCtrl[2]->replaceSeqShift(0, m_pSeqName, 0xD1, vec.x, vec.y);
        m_pDeckCardCtrl[1]->replaceSeq(0, m_pSeqName, tabSeqTbl[m_nKindCategorySelect * 2 + 1]);
        break;
    case 20:
        m_pDeckCardCtrl[1]->replaceSeq(0, m_pSeqName, tabSeqTbl[m_nKindCategorySelect * 2 + m_nCardInOut]);
        initCardKind(m_nDeckNoSelect, m_nKindCategorySelect, m_bShortCutMode);
    case 18:
        sprintf(chrTmp, pStrNum[0], m_nDeck[m_nDeckCategorySelect], m_nCard);
        m_pDeckCardFnt[0]->allocText(chrTmp);
        sprintf(chrTmp, pStrNum[1], m_nKindTotal[1]);
        m_pDeckCardFnt[1]->allocText(chrTmp);
        sprintf(chrTmp, pStrNum[1], m_nKindTotal[2]);
        m_pDeckCardFnt[2]->allocText(chrTmp);
        sprintf(chrTmp, pStrNum[1], m_nKindTotal[3]);
        m_pDeckCardFnt[3]->allocText(chrTmp);
        sprintf(chrTmp, pStrNum[1], m_nKindTotal[4]);
        m_pDeckCardFnt[4]->allocText(chrTmp);
        for (INT i = 0; i < 9; i++) {
            p2DCard = m_pDeckCardCard[i];
            INT idx = i + m_nKindSelectTop;
            for (INT j = 4; j > 0; j--) {
                p2DCard->destroy(j);
            }
            if (idx < m_nKind[m_nKindCategorySelect]) {
                card.init(m_kindTbl[idx], 0xFF);
                p2DCard->create(card);
                if (card.m_fgPremium || card.m_nKind == 0x7D || card.m_nKind == 0x7E) {
                    p2DCard->restart(p2DCard->getNumSeq() - 1, m_iFrame60);
                }
            }
        }
        {
            INT nRow = (m_nKind[m_nKindCategorySelect] + 2) / 3;
            if (!nRow) {
                nRow = 1;
            }
            fRatioHeight = 3.0f / nRow;
            if (fRatioHeight > 1.0f) {
                fRatioHeight = 1.0f;
            }
            fRatioPosY = (FLOAT)(m_nKindSelectTop / 3) / nRow;
            if (fRatioPosY > 1.0f) {
                fRatioPosY = 1.0f;
            }
        }
        m_pDeckCardScrollBar->change(fRatioPosY, fRatioHeight);
        break;
    }
}

INT CCamp::setDeckDeck(INT nSetCmd) {
    static char* pStrNum[3] = { "%02d/99", "\xf9X%04d\xf9" "A/%04d", "%02d" };
    FVECTOR vec;
    FVECTOR vec2;
    C2DSeqCtrl* p2DSeqCtrl;
    C2DCard* p2DCard;
    C2DSeqObj* p2DSeqObj;
    G_RGBA rgba;
    CARD card;
    FLOAT fRatioPosY;
    char chrTmp[64];
    static USHORT scrollBarSeqTbl[3] = { 288, 285, 291 };

    switch (nSetCmd) {
    case 1:
        if (!m_pDeckDeckCtrl[0]) {
            p2DSeqCtrl = m_pDeckDeckCtrl[0] = new KH_AT("(4546)") C2DSeqCtrl;
            p2DSeqCtrl->setLayer(0x11);
            p2DSeqCtrl->enableColorCtrl(TRUE);
            C2DMan::getLayPosition(m_pLadName, 5, 9, vec, 0);
            p2DSeqCtrl->setPos(vec);
            p2DSeqCtrl->addSeq(m_pSeqName, 0xAB);
            p2DSeqCtrl = m_pDeckDeckCtrl[1] = new KH_AT("(4554)") C2DSeqCtrl;
            p2DSeqCtrl->enableColorCtrl(TRUE);
            p2DSeqCtrl->setLayer(0x10);
            p2DSeqCtrl->setParent(m_pDeckDeckCtrl[0]);
            C2DMan::getLayPosition(m_pLadName, 4, 0, vec, 0);
            p2DSeqCtrl->addSeqShift(m_pSeqName, !m_bPlayerKind ? 0xC0 : 0x15C, vec.x, vec.y);
            rgba.col = 0x807C7C7C;
            m_pDeckDeckFnt[1] = new KH_AT("(4563)") C2DFnt(0, 0, &rgba, 0xD, NULL, 0x40);
            p2DSeqCtrl->addFntShift(m_pSeqName, m_pDeckDeckFnt[1], -60.0f, 21.0f);
            if (!m_bPlayerKind) {
                m_pDeckDeckFnt[2] = new KH_AT("(4568)") C2DFnt(0, 0, &rgba, 0xD, NULL, 0x40);
                p2DSeqCtrl->addFntShift(m_pSeqName, m_pDeckDeckFnt[2], 0.0f, 21.0f);
                p2DSeqCtrl = m_pDeckDeckCtrl[2] = new KH_AT("(4579)") C2DSeqCtrl;
                p2DSeqCtrl->setLayer(0xF);
                p2DSeqCtrl->enableColorCtrl(TRUE);
                p2DSeqCtrl->setParent(m_pDeckDeckCtrl[1]);
                p2DSeqCtrl->addSeq(m_pSeqName, m_nDeckNoSelect * 2 + 0xF2);
                p2DSeqObj = p2DSeqCtrl->addSeqShift(m_pSeqName, 0xAE, -2.0f, 108.0f);
                p2DSeqObj->changeLayer(0x10);
                if (m_nDeckSelect == CCard::getCurrentDeckNo()) {
                    p2DSeqCtrl->enableDisp(1, TRUE);
                } else {
                    p2DSeqCtrl->enableDisp(1, FALSE);
                }
                m_pDeckDeckFnt[0] = new KH_AT("(4591)") C2DFnt(0, 0, &rgba, 0xE, CCard::getDeckName(m_nDeckNoSelect), 0x40);
                p2DSeqObj = p2DSeqCtrl->addFntShift(m_pSeqName, m_pDeckDeckFnt[0], -73.0f, -9.0f);
                p2DSeqObj->changeLayer(0x10);
            }
            for (INT i = 0; i < 3; i++) {
                for (INT j = 0; j < 3; j++) {
                    C2DMan::getLayPosition(m_pLadName, 4, i * 3 + j + 3, vec, 0);
                    p2DCard = m_pDeckDeckCard[i * 3 + j] = new KH_AT("(4611)") C2DCard;
                    p2DCard->setParent(m_pDeckDeckCtrl[0]);
                    p2DCard->setLayer(0x11);
                    p2DCard->enableColorCtrl(TRUE);
                    p2DCard->setLocalPos(vec);
                    p2DCard->addSeq(m_pSeqName, 0xB6);
                }
            }
            for (INT i = 0; i < 9 && i < m_nDeck[m_nKindCategorySelect]; i++) {
                p2DCard = m_pDeckDeckCard[i];
                card = m_pDeck[i];
                p2DCard->create(card);
            }
            m_pDeckDeckScrollBar = new KH_AT("(4643)") CScrollBar;
            vec.set(68.0f, -56.0f, 0.0f, 1.0f);
            m_pDeckDeckScrollBar->init(m_pSeqName, 0x11, scrollBarSeqTbl, vec, 118.0f, m_pDeckDeckCtrl[0]);
        } else {
            m_pDeckDeckCtrl[0]->replaceSeq(0, m_pSeqName, 0xAB);
        }
        break;
    case 3:
        m_pDeckDeckCtrl[0]->replaceSeq(0, m_pSeqName, !m_nCardInOut ? 0xAC : 0xAA);
        C2DMan::getLayPosition(m_pLadName, 4, 0, vec, 0);
        {
            UINT nSeq = !m_bPlayerKind ? (!m_nCardInOut ? 0xC0 : 0xBF) : (!m_nCardInOut ? 0x15C : 0x15B);
            m_pDeckDeckCtrl[1]->replaceSeqShift(0, m_pSeqName, nSeq, vec.x, vec.y);
        }
        if (m_pDeckDeckCtrl[2]) {
            INT nIdx = m_nDeckNoSelect * 2;
            INT nSeq = !m_nCardInOut ? nIdx + 0xF2 : nIdx + 0xF1;
            m_pDeckDeckCtrl[2]->replaceSeq(0, m_pSeqName, (USHORT)nSeq);
        }
        break;
    case 4:
        m_pDeckDeckCtrl[0]->replaceSeq(0, m_pSeqName, 0xAD);
        break;
    case 6:
        for (INT i = 0; i < 3; i++) {
            if (m_pDeckDeckCtrl[i]) {
                m_pDeckDeckCtrl[i]->enableDisp(FALSE);
            }
        }
        for (INT i = 0; i < 9; i++) {
            m_pDeckDeckCard[i]->enableDisp(FALSE);
        }
        m_pDeckDeckScrollBar->enableDisp(FALSE);
        break;
    case 7:
        for (INT i = 0; i < 3; i++) {
            if (m_pDeckDeckCtrl[i]) {
                m_pDeckDeckCtrl[i]->enableDisp(TRUE);
            }
        }
        for (INT i = 0; i < 9; i++) {
            m_pDeckDeckCard[i]->enableDisp(TRUE);
        }
        m_pDeckDeckScrollBar->enableDisp(TRUE);
        break;
    case 18:
    case 20:
        sprintf(chrTmp, pStrNum[0], m_nDeck[0]);
        m_pDeckDeckFnt[1]->allocText(chrTmp);
        if (!m_bPlayerKind) {
            setTotalCP();
            sprintf(chrTmp, pStrNum[1], m_nTotalCP, CStatus::getCPMax());
            if (CStatus::getCPMax() < m_nTotalCP) {
                chrTmp[0] = 0xF9;
                chrTmp[1] = 0x50;
            }
            m_pDeckDeckFnt[2]->allocText(chrTmp);
        }
        for (INT i = 0; i < 9; i++) {
            p2DCard = m_pDeckDeckCard[i];
            INT idx = i + m_nDeckSelectTop;
            for (INT j = 5; j > 0; j--) {
                p2DCard->destroy(j);
            }
            if (idx < 99 && (card = m_pDeck[idx]).m_nId) {
                if (card.m_nCategory20 >= 7) {
                    card.m_nNumber = -1;
                }
                p2DCard->create(card);
                if (card.m_fgPremium || card.m_nKind == 0x7D || card.m_nKind == 0x7E) {
                    p2DCard->restart(p2DCard->getNumSeq() - 1, m_iFrame60);
                }
                if (idx == m_nSCIndex[m_nDeckCategorySelect]) {
                    p2DCard->addSeq(m_pSeqName, 0x117);
                }
            }
        }
        fRatioPosY = (FLOAT)(m_nDeckSelectTop / 3) / 33.0f;
        if (fRatioPosY > 1.0f) {
            fRatioPosY = 1.0f;
        }
        m_pDeckDeckScrollBar->change(fRatioPosY, 0.09090909f);
        break;
    case 19:
        if (m_pDeckDeckCtrl[2]) {
            INT nIdx = m_nDeckNoSelect * 2;
            m_pDeckDeckCtrl[2]->replaceSeq(0, m_pSeqName, !m_nCardInOut ? nIdx + 0xF2 : nIdx + 0xF1);
            if (m_nDeckNoSelect == CCard::getCurrentDeckNo()) {
                m_pDeckDeckCtrl[2]->enableDisp(1, TRUE);
            } else {
                m_pDeckDeckCtrl[2]->enableDisp(1, FALSE);
            }
            m_pDeckDeckFnt[0]->setText(CCard::getDeckName(m_nDeckNoSelect));
        }
        break;
    }
    return 1;
}

INT CCamp::setDeckInfo(INT nSetCmd) {
    static char* pStrNum[4] = { "%d", "\x81~%02d", "", "--" };
    FVECTOR vec;
    FVECTOR vec2;
    C2DSeqCtrl* p2DSeqCtrl;
    C2DFnt* p2DFnt;
    G_RGBA rgba;
    CARD card;
    INT nKind;
    INT nColSeq;
    INT idxClearAttr;
    INT idx;
    char chrTmp[64];
    const CHAR* pCardAttack;
    const CHAR* pStrAttribute;
    const CHAR* pCardSpeed;
    const CHAR* pCardHelp;
    static unsigned char colSeqTbl[8] = { 0, 116, 75, 75, 95, 95, 95, 115 };

    switch (nSetCmd) {
    case 1:
        if (!m_pDeckInfoCtrl) {
            m_pDeckInfoCtrl = new KH_AT("(4843)") C2DSeqCtrl;
            m_pDeckInfoCtrl->setLayer(0x11);
            m_pDeckInfoCtrl->enableColorCtrl(TRUE);
            C2DMan::getLayPosition(m_pLadName, 3, 0xB, vec, 0);
            m_pDeckInfoCtrl->setPos(vec);
            m_pDeckInfoCtrl->addSeq(m_pSeqName, !m_nCardInOut ? 0x95 : 0x98);
            if (!m_bPlayerKind) {
                m_pDeckInfoCtrl->addSeqShift(m_pSeqName, !m_nCardInOut ? 0xCF : 0xCE, -256.0f, -192.0f);
            }
            rgba.col = 0x807C7C7C;
            for (INT i = 0; i < 5; i++) {
                INT nFntSize;
                if (i == 0) {
                    nFntSize = 0xD;
                    vec.set(33.0f, 2.0f, 0.0f, 1.0f);
                } else if (i == 4) {
                    nFntSize = 0xD;
                    vec.set(-65.0f, -93.0f, 0.0f, 1.0f);
                } else {
                    nFntSize = 0xB;
                    vec.set(16.0f, 8.0f - i * 24.0f, 0.0f, 1.0f);
                }
                p2DFnt = m_pDeckInfoFnt[i] = new KH_AT("(4878)") C2DFnt(0, 0, &rgba, nFntSize, NULL, 0x40);
                m_pDeckInfoCtrl->addFntShift(m_pSeqName, p2DFnt, vec.x, vec.y);
            }
            for (INT i = 0; i < 2; i++) {
                for (INT j = 0; j < 5; j++) {
                    INT idx = i * 5 + j;
                    p2DSeqCtrl = m_pDeckInfoNumCtrl[idx] = new KH_AT("(4888)") C2DSeqCtrl;
                    p2DSeqCtrl->setParent(m_pDeckInfoCtrl);
                    p2DSeqCtrl->setLayer(0x11);
                    p2DSeqCtrl->enableColorCtrl(TRUE);
                    C2DMan::getLayPosition(m_pLadName, 2, idx + 2, vec, 0);
                    p2DSeqCtrl->setLocalPos(vec);
                    p2DSeqCtrl->addSeq(m_pSeqName, idx * 2 + 0x60);
                    p2DFnt = m_pDeckInfoNumFnt[idx] = new KH_AT("(4897)") C2DFnt(0, 0, &rgba, 0xC, NULL, 0x40);
                    p2DFnt->allocText(chrTmp);
                    p2DSeqCtrl->addFntShift(m_pSeqName, p2DFnt, 14.0f, 6.0f);
                }
            }
            m_pDeckInfoCard = new KH_AT("(4906)") C2DCard;
            m_pDeckInfoCard->setLayer(0x11);
            m_pDeckInfoCard->enableColorCtrl(TRUE);
            m_pDeckInfoCard->setParent(m_pDeckInfoCtrl);
            C2DMan::getLayPosition(m_pLadName, 2, 1, vec, 0);
            m_pDeckInfoCard->setLocalPos(vec);
            m_pDeckInfoCard->addSeq(m_pSeqName, 0x158);
            if (!m_bPlayerKind) {
                m_pDeckInfoIOCtrl = new KH_AT("(4917)") C2DSeqCtrl(1);
                m_pDeckInfoIOCtrl->setLayer(0x11);
                vec.set(0.0f, 0.0f, 0.0f, 1.0f);
                m_pDeckInfoIOCtrl->setPos(vec);
                m_pDeckInfoIOCtrl->addSeq(m_pSeqName, 0x92);
                m_pDeckInfoIOCtrl->addSeq(m_pSeqName, 0x9C);
            }
        } else {
            m_pDeckInfoCtrl->replaceSeq(0, m_pSeqName, !m_nCardInOut ? 0x95 : 0x98);
            if (m_pDeckInfoIOCtrl) {
                m_pDeckInfoIOCtrl->replaceSeq(0, m_pSeqName, 0x92);
                m_pDeckInfoIOCtrl->replaceSeq(1, m_pSeqName, 0x9C);
            }
            for (INT i = 0; i < 10; i++) {
                m_pDeckInfoNumCtrl[i]->replaceFnt(1, m_pSeqName, m_pDeckInfoNumFnt[i]);
            }
            m_pDeckInfoCard->m_card.m_nId = 0;
        }
        break;
    case 3:
        m_pDeckInfoCtrl->replaceSeq(0, m_pSeqName, !m_nCardInOut ? 0x96 : 0x99);
        if (m_pDeckInfoIOCtrl) {
            m_pDeckInfoIOCtrl->replaceSeq(0, m_pSeqName, 0x93);
            m_pDeckInfoIOCtrl->replaceSeq(1, m_pSeqName, 0x9D);
        }
        break;
    case 4:
        m_pDeckInfoCtrl->replaceSeq(0, m_pSeqName, !m_nCardInOut ? 0x97 : 0x9A);
        if (m_pDeckInfoIOCtrl) {
            m_pDeckInfoIOCtrl->replaceSeq(0, m_pSeqName, 0x94);
            m_pDeckInfoIOCtrl->replaceSeq(1, m_pSeqName, 0x9E);
        }
        for (INT i = 0; i < 10; i++) {
            m_pDeckInfoNumCtrl[i]->replaceFnt(1, m_pSeqName, m_pDeckInfoNumFnt[i]);
        }
        setCardHelp(0);
        break;
    case 6:
        m_pDeckInfoCtrl->enableDisp(FALSE);
        for (INT i = 0; i < 10; i++) {
            m_pDeckInfoNumCtrl[i]->enableDisp(FALSE);
        }
        if (m_pDeckInfoIOCtrl) {
            m_pDeckInfoIOCtrl->enableDisp(FALSE);
        }
        m_pDeckInfoCard->enableDisp(FALSE);
        break;
    case 7:
        m_pDeckInfoCtrl->enableDisp(TRUE);
        m_pDeckInfoCtrl->enableDisp(1, FALSE);
        for (INT i = 0; i < 10; i++) {
            m_pDeckInfoNumCtrl[i]->enableDisp(TRUE);
        }
        if (m_pDeckInfoIOCtrl) {
            m_pDeckInfoIOCtrl->enableDisp(TRUE);
        }
        m_pDeckInfoCard->enableDisp(TRUE);
        break;
    case 10:
        if (m_pDeckInfoIOCtrl) {
            m_pDeckInfoIOCtrl->replaceSeq(0, m_pSeqName, 0x91);
            m_pDeckInfoIOCtrl->replaceSeq(1, m_pSeqName, 0x9D);
            m_pDeckInfoCtrl->replaceSeqShift(1, m_pSeqName, !m_nCardInOut ? 0xCF : 0xCE, -256.0f, -192.0f);
            m_pDeckInfoCtrl->enableDisp(1, TRUE);
        }
        break;
    case 11:
        if (m_pDeckInfoIOCtrl) {
            m_pDeckInfoIOCtrl->replaceSeq(0, m_pSeqName, 0x93);
            m_pDeckInfoIOCtrl->replaceSeq(1, m_pSeqName, 0x9B);
            m_pDeckInfoCtrl->replaceSeqShift(1, m_pSeqName, !m_nCardInOut ? 0xCF : 0xCE, -256.0f, -192.0f);
            m_pDeckInfoCtrl->enableDisp(1, TRUE);
        }
        break;
    case 14:
        m_pDeckInfoIOCtrl->replaceSeq(0, m_pSeqName, 0x93);
        m_pDeckInfoIOCtrl->replaceSeq(1, m_pSeqName, 0x9D);
        m_pDeckInfoCtrl->enableDisp(1, FALSE);
        break;
    case 15:
        if (m_pDeckInfoIOCtrl) {
            m_pDeckInfoIOCtrl->enableDisp(FALSE);
        }
        m_pDeckInfoCtrl->enableDisp(1, FALSE);
        break;
    case 12:
    case 17: {
        CARD cardOld;
        for (INT i = 4; i > 0; i--) {
            m_pDeckInfoCard->destroy(i);
        }
        cardOld = m_pDeckInfoCard->m_card;
        *(INT*)&card.m_nId &= ~0xFF;
        idxClearAttr = 0;
        nKind = 0;
        if (!m_bCategorySelectMode) {
            if (!m_nCardInOut) {
                if (m_nKind[m_nKindCategorySelect]) {
                    nKind = m_kindTbl[m_nKindSelect];
                    card.init(nKind, 0xFF);
                    if (nKind) {
                        m_pDeckInfoCard->create(card);
                        if (nKind != cardOld.m_nKind) {
                            m_pDeckInfoCard->restart(0, 0.0f);
                        }
                    }
                    card.m_nNumber = 1;
                }
            } else if (m_nDeck[m_nDeckCategorySelect]) {
                card = m_pDeck[m_nDeckSelect];
                if (card.m_nId) {
                    if (card.m_nCategory20 == 7) {
                        card.m_nNumber = -1;
                    }
                    m_pDeckInfoCard->create(card);
                    if (nSetCmd != 0x11 || card.m_nId != cardOld.m_nId) {
                        m_pDeckInfoCard->restart(0, 0.0f);
                    }
                }
            }
        }
        if (card.m_nKind) {
            if (!m_nCardInOut) {
                if (nSetCmd != 0x11) {
                    sprintf(chrTmp, pStrNum[3]);
                    m_pDeckInfoFnt[0]->allocText(chrTmp);
                }
            } else {
                sprintf(chrTmp, pStrNum[0], CCard::getCP(card));
                m_pDeckInfoFnt[0]->allocText(chrTmp);
            }
            if (CCard::getAttribute(card, &pCardAttack, &pStrAttribute, &pCardSpeed, &pCardHelp)) {
                m_pDeckInfoFnt[1]->allocText(pCardSpeed);
                m_pDeckInfoFnt[2]->allocText(pStrAttribute);
                m_pDeckInfoFnt[3]->allocText(pCardAttack);
                m_pHelpFnt->allocText(pCardHelp);
                {
                    LPTSTR pText = ((CFntLF*)(CFntM2*)m_pHelpFnt)->text();
                    LPTSTR pLF;
                    if (pText && (pLF = strchr(pText, '\n'))) {
                        *pLF = 0;
                        strcat(pText, pLF + 1);
                    }
                }
                idxClearAttr = 4;
            } else {
                idxClearAttr = 1;
            }
            for (idx = 0; idx < m_nCardKindMax; idx++) {
                if (m_nCardKind[idx] == nKind) {
                    break;
                }
            }
        } else {
            m_pDeckInfoCard->m_card.m_nId = 0;
            m_pHelpFnt->allocText(pStrNum[2]);
        }
        chrTmp[0] = ' ';
        chrTmp[1] = 0;
        for (INT i = idxClearAttr; i < 4; i++) {
            m_pDeckInfoFnt[i]->allocText(chrTmp);
        }
        m_pDeckInfoFnt[4]->setText(card.getName());
        nColSeq = colSeqTbl[card.m_nCategory0];
        for (INT i = 0; i < 10; i++) {
            p2DSeqCtrl = m_pDeckInfoNumCtrl[i];
            if (!m_nCardInOut && card.m_nKind) {
                if (m_nKind[m_nKindCategorySelect]) {
                  if (nColSeq != 0x73 || i == 0) {
                    char chrNum[16];
                    INT nOff = (m_nKind10Tbl[idx][i] == 0);
                    p2DSeqCtrl->enableDisp(TRUE);
                    p2DSeqCtrl->replaceSeq(0, m_pSeqName, nColSeq + i * 2 + nOff);
                    sprintf(chrNum, pStrNum[1], m_nKind10Tbl[idx][i]);
                    m_pDeckInfoNumFnt[i]->allocText(chrNum);
                  } else {
                    p2DSeqCtrl->enableDisp(FALSE);
                  }
                } else {
                    p2DSeqCtrl->enableDisp(FALSE);
                }
            } else {
                p2DSeqCtrl->enableDisp(FALSE);
            }
        }
        break;
    }
    case 13:
        card.init(m_kindTbl[m_nKindSelect], m_nNumSelect);
        sprintf(chrTmp, pStrNum[0], CCard::getCP(card));
        m_pDeckInfoFnt[0]->allocText(chrTmp);
        break;
    case 16:
        m_pDeckInfoFnt[0]->allocText(pStrNum[3]);
        break;
    }
    return 1;
}

INT CCamp::setButtonHelp(INT nSetCmd, INT nMode, INT nKind) {
    FVECTOR vec;
    FVECTOR vec2;
    C2DSeqCtrl** pp2DSeqCtrl;
    C2DFnt** pp2DFnt;
    G_RGBA rgba;
    static USHORT buttonHelpSeqTbl[10] = { 136, 139, 139, 139, 273, 139, 213, 345, 346, 142 };

    pp2DSeqCtrl = &m_pButtonHelpCtrl[nMode];
    pp2DFnt = &m_pDeckButtonFnt[nMode];
    switch (nSetCmd) {
    case 1:
        if (m_bPlayerKind && nKind < 2) {
            nKind = 9;
        }
        if (!*pp2DSeqCtrl) {
            *pp2DSeqCtrl = new KH_AT("(5220)") C2DSeqCtrl;
            (*pp2DSeqCtrl)->setLayer(0x10);
            (*pp2DSeqCtrl)->enableColorCtrl(TRUE);
            vec.set(0.0f, 0.0f, 0.0f, 1.0f);
            (*pp2DSeqCtrl)->setPos(vec);
            (*pp2DSeqCtrl)->addSeq(m_pSeqName, buttonHelpSeqTbl[nKind]);
            rgba.col = 0x807C7C7C;
            *pp2DFnt = new KH_AT("(5227)") C2DFnt(0, 0, &rgba, 0xD, CCtdMan::getText(nKind + 0x500900), 0x40);
            (*pp2DSeqCtrl)->addFntShift(m_pSeqName, *pp2DFnt, 0.0f, 1.0f);
            if (nMode == 0 && nKind == 0) {
                vec.set(-150.0f, -27.0f, 0.0f, 1.0f);
                m_pButtonHelpSubButton = new KH_AT("(5237)") CButton;
                m_pButtonHelpSubButton->init(m_pSeqName, 0x11, buttonHelpSeqTbl, vec, *pp2DSeqCtrl, CCtdMan::getText(0x500907),
                                             0xD, 0.0f, 0.0f, 0x40, &rgba, 7);
            }
        } else {
            (*pp2DSeqCtrl)->replaceSeq(0, m_pSeqName, buttonHelpSeqTbl[nKind]);
        }
        m_nButtonHelpRun[nMode] = 2;
        m_nButtonHelpKind[nMode] = nKind;
        break;
    case 2:
        if (!(*pp2DSeqCtrl)->isEnd(0)) {
            break;
        }
        m_nButtonHelpRun[nMode] = 999;
    case 3:
        (*pp2DSeqCtrl)->replaceSeq(0, m_pSeqName, buttonHelpSeqTbl[m_nButtonHelpKind[nMode]] + 1);
        break;
    case 4:
        (*pp2DSeqCtrl)->replaceSeq(0, m_pSeqName, buttonHelpSeqTbl[m_nButtonHelpKind[nMode]] + 2);
        m_nButtonHelpRun[nMode] = 5;
        break;
    case 5:
        if (!(*pp2DSeqCtrl)->isEnd(0)) {
            break;
        }
        m_nButtonHelpRun[nMode] = 999;
    case 6:
        delete *pp2DSeqCtrl;
        *pp2DSeqCtrl = NULL;
        if (nMode == 0) {
            delete m_pButtonHelpSubButton;
            m_pButtonHelpSubButton = NULL;
        }
        break;
    case 7:
        (*pp2DSeqCtrl)->enableDisp(TRUE);
        break;
    case 8:
    case 9:
        break;
    case 10:
        if (!nKind) {
            nKind = m_bPlayerKind ? 9 : 0;
        }
        if (nMode == 0 && m_pButtonHelpSubButton) {
            m_pButtonHelpSubButton->setSequence((CButton::BUTTON_SEQ)(nKind ? 8 : 9), FALSE);
        }
        break;
    }
    return 1;
}

/* Parked: 27 of 612 lines (2026-11-13; drop the data block after the INCLUDE_ASM to build the draft): only the s-register
   roles in the sleight-list loop differ (original s3 nTechKind, s4 i, s5 nTechSeqIdx, s6 nTechSelectTop, i + 1 in s7;
   this draft s3 nTechSelectTop, s4 nTechKind, s5 i, s6 nTechSeqIdx, nTechSelectTop + 1 in s7). 2026-11-09: 36 -> 27 by
   `C2DFnt* pFnt = m_pTechCategoryFnt[nTechCateCnt++] = new ...; addFntShift(m_pSeqName, pFnt, ...)`: the nTechCateCnt
   store before the new (HeapMan.h:150 in the original: the post-increment is flushed inside the inlined operator new),
   the 1.0f load order and the spilled nTechCateCnt all come right. pFnt is not in the ref and not in our .mdebug either
   (no hard register, like pButton): a used local invisible in the .mdebug, so the user decides; evidence: the original
   passes the new'd pointer (a2 = a0) to addFntShift without reloading m_pTechCategoryFnt[], and
   `[nTechCateCnt - 1]` reloads (92). Cause of the roles (dumps -dG/-dl/-dg): gcse PRE turns both for-increments into
   copies of temps computed at the end of bb 50 (after canUse; sched1 issues them before getName), so neither is a biv
   (no giv reduction, as the original). Global alloc takes nTechSelectTop first (9 refs / 41 live = 6585; nTechKind
   12/64 = 5625, i 11/80 = 4125 (local-alloc.c:874 doubles i's life for its `i = 0` REG_EQUIV), nTechSeqIdx 12/102 =
   3529); the original's order needs nTechSelectTop below nTechSeqIdx: 7 refs (one in-loop use fewer) or about 77
   insns of life (a doubled life needs a constant first set); flow counts refs per block depth (flow.c:2676), the
   block tree is equal to the original's, so no hidden loop. In-place `nTechSelectTop++` in the body (A: after the
   CButton new) gives the right roles but makes it a biv: loop.c reduces the combined row giv (benefit 21; 46 lines on
   the pFnt draft). Line markers: nTechSelectTop = m_nTechSelectTop is line 5470 (after the 5469 ternary). tried
   (2026-11-09, pFnt draft): increment after getLayPosition (47), `nTechSelectTop++` in the compare (44),
   `nTechSelectTop++, i++` (30), pre-increments / `+= 1` / reversed compare / init after rgba (27, priorities
   unchanged); permuter 15 min on the pFnt draft (5486 candidates): only the init above the ternary again (against
   line 5470). Earlier (2026-11-08): increment at the body end (39), the new as the addFntShift argument (141: the
   instance is evaluated before the new), no pButton local (143), nLine as the loop counter, non-const table
   pointer, _ref() on the table pointer (56-70), pTechCateTbl forced to memory (62/84: the original's is a spilled
   pseudo, slots ret/-204 pTechCateTbl/-200 nTechCateCnt/-196 as here), the ref's G_RGBA techColorTbl[7] (kept),
   split nTechKind/nTechSeqIdx declarations, `(x & 1) == 0`, `nTechSeqIdx /= 2`; .mdebug block tree (applied):
   pTechCateTbl/nTechSelectTop/nTechCateCnt/nLine in the switch body, scrollBarSeqTbl in the if block, len before pBF;
   permuter runs 6 + 15 + 15 min. 2026-11-10 (sched1 -dS dump, vt.py: stabs registers per variant, target
   nTechKind/19 i/20 nTechSeqIdx/21 nTechSelectTop/22, ours 20/21/22/19): the PRE increments are issued at t=2 of
   bb 50 because the m_pTechButton[i] store -> getName call latency leaves that cycle empty (same in the original's
   final code); with refs 9 the original order needs nTechSelectTop live >= 77 (or refs 7 at 41), and i above
   nTechSeqIdx needs nTechSeqIdx live > 87. Init above the ternary (against line 5470) gives nTechKind s3,
   nTechSelectTop s4 (one step); compare operand swap, *pTechCateTbl[..], [1] load first, 2 + i, ++ forms, != 9,
   <= 8: registers unchanged. 2026-11-10 (after the Coord.h unification, re-measured 27): every header line marker
   now has the original's instruction counts (Coord.h:730 FVECTOR::set, HeapMan.h 150-153, 2DSeqCtrl.h, Fnt.h,
   MenuParts.h, Task.h); the block tree differs only in getFrameRateF's nested getFrameRate block, which has no
   `this` symbol in the original (also in the matched setDeckSetWin; const/non-const variants of the two Task.h
   inlines change nothing). sched1 (-dS) of bb 50 issues T_i (i + 1) and T_sel (nTechSelectTop + 1) together at t=2
   and the zero-extended nTechSeqIdx at t=3; even T_sel at the block end would give nTechSelectTop only ~60 insns of
   life (priority 4500: then nTechKind s3, nTechSelectTop s4), not the 77 needed. pFnt (not in the ref, see above)
   still awaits the user's decision; without it the draft is 36. 2026-11-13 (retail, `retail_units.py try --show`): the
   retail original has the same loop with other roles (s3 nTechKind, s4 i, s5 nTechSelectTop, s6 nTechSeqIdx); the draft
   compiled for retail gives this draft's beta roles (4 insns short: nops/reload that follow the roles) and also spills
   the four PRE-hoisted addresses in another order (ours m_pTechButton 0xD8, &rgba 0xDC, fLightX 0xD0, fLightY 0xD4;
   retail &rgba 0xD0, fLightX 0xD4, fLightY 0xD8, m_pTechButton 0xDC): bucket order (raw 13258 + R + C) needs S = 263
   (the only S in 201-331), ours 261 (max_cuid 523): retail has 1-4 more insns at gcse than this draft (beta: our
   max_cuid 553, S 277; 0-2 more keep beta's order). The roles in BOTH builds follow from one number: nTechSelectTop
   with 7 refs instead of 9 (the same live lengths 41 beta / 37 retail): beta 2*7/41 = 3414 < nTechSeqIdx 3529, retail
   2*7/37 = 3783 between nTechSeqIdx 3750 and i 4583. Verified: comparing `i == m_nTechSelect` instead (one in-loop
   use fewer, wrong code) gives the original's roles in both builds and leaves 1 line (that compare) in beta. So the
   original has one in-loop occurrence of nTechSelectTop fewer (index, compare, PRE'd `+ 1`, loop-end copy: weight 2
   each, the init 1); a longer life instead (9 refs) would need >= 77 (beta) with 59-72 (retail), which sched1 cannot
   give. Not it (both builds measured): increment at the body end, `nTechSelectTop++, i++`, pre-increments, do/while
   and while forms, reversed or `- == 0` compares, `(*(tbl + n))[0]`, init after rgba, SHORT/UINT nTechSelectTop,
   `nTechSelectTop + i` without an increment (addu per pass), in-place increments in the body (giv reduced), `++` in
   the compare or index. Scripts: build/stw/var.py, pri.py, sprobe.sh in the 2026-11-13 worktree. */
#if 0
INT CCamp::setTechWin(INT nSetCmd) {
    FVECTOR vec;
    FVECTOR vec2;
    C2DSeqObj* p2DSeqObj;
    G_RGBA rgba;
    FLOAT fRatioHeight;
    FLOAT fRatioPosY;
    static USHORT techSeqTbl[14] = { 224, 217, 226, 219, 230, 223, 225, 218, 227, 220, 229, 222, 228, 221 };
    G_RGBA techColorTbl[7] = { 0x80404080, 0x807A6331, 0x80795080, 0x80137B18, 0x80404080, 0x80505050, 0x807A3163 };
    static char* pTechCateNameTbl[7] = { "Attacks", "Magic", "Summons", "Friends", "Attacks", "Friends", "Duel Attacks" };
    float fLightXTbl[9] = { 556.0f, 556.0f, 556.0f, 556.0f, 556.0f, 556.0f, 556.0f, 556.0f, 556.0f };
    float fLightYTbl[9] = { 160.0f, 160.0f, 160.0f, 160.0f, 160.0f, 160.0f, 160.0f, 160.0f, 160.0f };
    static LPCTSTR pStrListTitle = "Sleights List";
    INT ret = 0;

    switch (nSetCmd) {
        const unsigned char (*pTechCateTbl)[2];
        INT nTechSelectTop;
        INT nTechCateCnt;
        INT nLine;
    case 0:
        if (!m_pTechWinCtrl) {
            static USHORT scrollBarSeqTbl[3] = { 288, 285, 291 };
            m_pTechWinCtrl = new KH_AT("(5381)") C2DSeqCtrl;
            m_pTechWinCtrl->setLayer(0x14);
            m_pTechWinCtrl->enableColorCtrl(TRUE);
            C2DMan::getLayPosition(m_pLadName, 0xF, 1, vec, 0);
            m_pTechWinCtrl->setPos(vec);
            m_pTechWinCtrl->addSeq(m_pSeqName, 0xD2);
            rgba.col = 0x807C7C7C;
            m_pTechWinFnt = new KH_AT("(5390)") C2DFnt(0, 0, &rgba, 0x10, pStrListTitle, 0x42);
            m_pTechWinCtrl->addFntShift(m_pSeqName, m_pTechWinFnt, 0.0f, -98.0f);
            m_pTechWinScrollBar = new KH_AT("(5395)") CScrollBar;
            vec.set(228.0f, -70.0f, 0.0f, 1.0f);
            m_pTechWinScrollBar->init(m_pSeqName, 0x14, scrollBarSeqTbl, vec, 186.0f, m_pTechWinCtrl);
            m_nTechSelectMax = !m_bPlayerKind ? 83 : 13;
        } else {
            m_pTechWinCtrl->replaceSeq(0, m_pSeqName, 0xD2);
        }
        m_pTechWinBC = new KH_AT("(5412)") CButtonCursor(0x15, m_pTechWinCtrl, 0xD8);
        break;
    case 1:
        if (m_pTechWinCtrl->isEnd(0)) {
            ret = 1;
            m_pTechWinCtrl->replaceSeq(0, m_pSeqName, 0xD3);
            m_pTechWinBC->setDisp(TRUE, FALSE);
        }
    case 3:
        if (m_pTechWinBC) {
            m_pTechWinBC->moveLight(getFrameRateF());
        }
        break;
    case 4:
        m_pTechWinCtrl->replaceSeq(0, m_pSeqName, 0xD4);
        if (m_pTechWinBC) {
            m_pTechWinBC->setOut();
        }
        break;
    case 5:
        ret = m_pTechWinCtrl->isEnd(0) != 0;
        break;
    case 6:
        delete m_pTechWinCtrl;
        m_pTechWinCtrl = NULL;
        delete m_pTechWinScrollBar;
        m_pTechWinScrollBar = NULL;
        for (INT i = 0; i < 9; i++) {
            delete m_pTechButton[i];
            m_pTechButton[i] = NULL;
        }
        delete m_pTechWinBC;
        m_pTechWinBC = NULL;
        break;
    case 7:
        m_pTechWinCtrl->enableDisp(TRUE);
        break;
    case 9:
        if (m_nTechSelectTop > m_nTechSelect) {
            m_nTechSelectTop = m_nTechSelect;
        } else if (m_nTechSelectTop + 8 < m_nTechSelect) {
            m_nTechSelectTop = m_nTechSelect - 8;
        }
        for (INT i = 0; i < 9; i++) {
            delete m_pTechButton[i];
            m_pTechButton[i] = NULL;
        }
        pTechCateTbl = !m_bPlayerKind ? m_techSoraCateTbl : m_techRikuCateTbl;
        nTechSelectTop = m_nTechSelectTop;
        nTechCateCnt = 0;
        rgba.col = 0x807C7C7C;
        for (INT i = 0; i < 9; i++, nTechSelectTop++) {
            SHORT nTechKind = pTechCateTbl[nTechSelectTop][0];
            SHORT nTechSeqIdx = pTechCateTbl[nTechSelectTop][1];
            if (nTechSelectTop == m_nTechSelect) {
                m_nTechKind = nTechKind;
                if (m_fgNewTbl[nTechKind] > 0) {
                    m_fgNewTbl[nTechKind] = -1;
                }
            }
            C2DMan::getLayPosition(m_pLadName, 0x10, i + 2, vec, 0);
            CButton* pButton = m_pTechButton[i] = new KH_AT("(5482)") CButton;
            pButton->init(m_pSeqName, 0x14, techSeqTbl, vec, m_pTechWinCtrl, CTechnic::getName(nTechKind), 0xF, 1.0f, 1.0f, 0x40,
                          &rgba, nTechSeqIdx);
            if (!CTechnic::canUse(nTechKind)) {
                INT len;
                CHAR* pBF;
                pBF = (CHAR*)pButton->getButtonFnt();
                len = strlen(pBF);
                for (INT j = 0; j < len; j++) {
                    *pBF++ = '?';
                }
            }
            if (m_fgNewTbl[nTechKind]) {
                p2DSeqObj = pButton->get2DSeqCtrl()->addSeqShift(m_pSeqName, m_fgNewTbl[nTechKind] > 0 ? 0x1E : 0x1D, 278.0f, -13.0f);
                p2DSeqObj->restart(m_iFrame60);
            }
            if (!(nTechSeqIdx & 1)) {
                nTechSeqIdx >>= 1;
                C2DFnt* pFnt = m_pTechCategoryFnt[nTechCateCnt++] = new KH_AT("(5516)") C2DFnt(0, 0, &techColorTbl[nTechSeqIdx], 0x10, pTechCateNameTbl[nTechSeqIdx], 0x42);
                pButton->get2DSeqCtrl()->addFntShift(m_pSeqName, pFnt, -90.0f, 1.0f);
            }
        }
        m_pTechWinBC->select(m_nTechSelect - m_nTechSelectTop, m_pTechButton, fLightXTbl, fLightYTbl);
        nLine = m_nTechSelectMax;
        if (!nLine) {
            nLine = 1;
        }
        fRatioHeight = 9.0f / nLine;
        if (fRatioHeight > 1.0f) {
            fRatioHeight = 1.0f;
        }
        fRatioPosY = (FLOAT)m_nTechSelectTop / nLine;
        if (fRatioPosY > 1.0f) {
            fRatioPosY = 1.0f;
        }
        m_pTechWinScrollBar->change(fRatioPosY, fRatioHeight);
        break;
    }
    return ret;
}
#else
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Form/Menu/Camp", setTechWin__5CCampi);
#endif
// setTechWin__5CCampi: its .rodata/.data, for its INCLUDE_ASM; remove with it.
asm(".section .rodata\n"
    ".align 3\n"
    "D_004E3628:\n" ".byte 0x80,0x40,0x40,0x80,0x31,0x63,0x7A,0x80,0x80,0x50,0x79,0x80,0x18,0x7B,0x13,0x80\n"
    ".byte 0x80,0x40,0x40,0x80,0x50,0x50,0x50,0x80,0x63,0x31,0x7A,0x80,0x00,0x00,0x00,0x00\n"
    "D_004E3648:\n" ".byte 0x41,0x74,0x74,0x61,0x63,0x6B,0x73,0x00\n"
    "D_004E3650:\n" ".byte 0x4D,0x61,0x67,0x69,0x63,0x00,0x00,0x00\n"
    "D_004E3658:\n" ".byte 0x53,0x75,0x6D,0x6D,0x6F,0x6E,0x73,0x00\n"
    "D_004E3660:\n" ".byte 0x46,0x72,0x69,0x65,0x6E,0x64,0x73,0x00\n"
    "D_004E3668:\n" ".byte 0x44,0x75,0x65,0x6C,0x20,0x41,0x74,0x74,0x61,0x63,0x6B,0x73,0x00,0x00,0x00,0x00\n"
    "D_004E3678:\n" ".byte 0x00,0x00,0x0B,0x44,0x00,0x00,0x0B,0x44,0x00,0x00,0x0B,0x44,0x00,0x00,0x0B,0x44\n"
    ".byte 0x00,0x00,0x0B,0x44,0x00,0x00,0x0B,0x44,0x00,0x00,0x0B,0x44,0x00,0x00,0x0B,0x44\n"
    ".byte 0x00,0x00,0x0B,0x44,0x00,0x00,0x00,0x00\n"
    "D_004E36A0:\n" ".byte 0x00,0x00,0x20,0x43,0x00,0x00,0x20,0x43,0x00,0x00,0x20,0x43,0x00,0x00,0x20,0x43\n"
    ".byte 0x00,0x00,0x20,0x43,0x00,0x00,0x20,0x43,0x00,0x00,0x20,0x43,0x00,0x00,0x20,0x43\n"
    ".byte 0x00,0x00,0x20,0x43,0x00,0x00,0x00,0x00\n"
    "D_004E36C8:\n" ".byte 0x53,0x6C,0x65,0x69,0x67,0x68,0x74,0x73,0x20,0x4C,0x69,0x73,0x74,0x00,0x00,0x00\n"
    KH_DEBUG("D_004E36D8:\n" ".byte 0x63,0x3A,0x2F,0x4B,0x48,0x43,0x4F,0x4D,0x2F,0x4D,0x61,0x69,0x6E,0x41,0x70,0x70\n")
    KH_DEBUG(".byte 0x2F,0x46,0x6F,0x72,0x6D,0x2F,0x4D,0x65,0x6E,0x75,0x2F,0x43,0x61,0x6D,0x70,0x2E\n")
    KH_DEBUG(".byte 0x63,0x70,0x70,0x28,0x35,0x33,0x38,0x31,0x29,0x00,0x00,0x00,0x00,0x00,0x00,0x00\n")
    KH_DEBUG("D_004E3708:\n" ".byte 0x63,0x3A,0x2F,0x4B,0x48,0x43,0x4F,0x4D,0x2F,0x4D,0x61,0x69,0x6E,0x41,0x70,0x70\n")
    KH_DEBUG(".byte 0x2F,0x46,0x6F,0x72,0x6D,0x2F,0x4D,0x65,0x6E,0x75,0x2F,0x43,0x61,0x6D,0x70,0x2E\n")
    KH_DEBUG(".byte 0x63,0x70,0x70,0x28,0x35,0x33,0x39,0x30,0x29,0x00,0x00,0x00,0x00,0x00,0x00,0x00\n")
    KH_DEBUG("D_004E3738:\n" ".byte 0x63,0x3A,0x2F,0x4B,0x48,0x43,0x4F,0x4D,0x2F,0x4D,0x61,0x69,0x6E,0x41,0x70,0x70\n")
    KH_DEBUG(".byte 0x2F,0x46,0x6F,0x72,0x6D,0x2F,0x4D,0x65,0x6E,0x75,0x2F,0x43,0x61,0x6D,0x70,0x2E\n")
    KH_DEBUG(".byte 0x63,0x70,0x70,0x28,0x35,0x33,0x39,0x35,0x29,0x00,0x00,0x00,0x00,0x00,0x00,0x00\n")
    KH_DEBUG("D_004E3768:\n" ".byte 0x63,0x3A,0x2F,0x4B,0x48,0x43,0x4F,0x4D,0x2F,0x4D,0x61,0x69,0x6E,0x41,0x70,0x70\n")
    KH_DEBUG(".byte 0x2F,0x46,0x6F,0x72,0x6D,0x2F,0x4D,0x65,0x6E,0x75,0x2F,0x43,0x61,0x6D,0x70,0x2E\n")
    KH_DEBUG(".byte 0x63,0x70,0x70,0x28,0x35,0x34,0x31,0x32,0x29,0x00,0x00,0x00,0x00,0x00,0x00,0x00\n")
    KH_DEBUG("D_004E3798:\n" ".byte 0x63,0x3A,0x2F,0x4B,0x48,0x43,0x4F,0x4D,0x2F,0x4D,0x61,0x69,0x6E,0x41,0x70,0x70\n")
    KH_DEBUG(".byte 0x2F,0x46,0x6F,0x72,0x6D,0x2F,0x4D,0x65,0x6E,0x75,0x2F,0x43,0x61,0x6D,0x70,0x2E\n")
    KH_DEBUG(".byte 0x63,0x70,0x70,0x28,0x35,0x34,0x38,0x32,0x29,0x00,0x00,0x00,0x00,0x00,0x00,0x00\n")
    KH_DEBUG("D_004E37C8:\n" ".byte 0x63,0x3A,0x2F,0x4B,0x48,0x43,0x4F,0x4D,0x2F,0x4D,0x61,0x69,0x6E,0x41,0x70,0x70\n")
    KH_DEBUG(".byte 0x2F,0x46,0x6F,0x72,0x6D,0x2F,0x4D,0x65,0x6E,0x75,0x2F,0x43,0x61,0x6D,0x70,0x2E\n")
    KH_DEBUG(".byte 0x63,0x70,0x70,0x28,0x35,0x35,0x31,0x36,0x29,0x00,0x00,0x00,0x00,0x00,0x00,0x00\n")
    ".byte 0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00\n"
    KH_VER("jtbl_004E3800:\n" ".word .L00315FC8\n" ".word .L003161C8\n" ".word .L003166EC\n" ".word .L00316214\n", "jtbl_004E3800:\n" ".word .L002AC108\n" ".word .L002AC2D8\n" ".word .L002AC7EC\n" ".word .L002AC324\n")
    KH_VER(".word .L00316244\n" ".word .L00316284\n" ".word .L0031629C\n" ".word .L00316348\n", ".word .L002AC354\n" ".word .L002AC394\n" ".word .L002AC3AC\n" ".word .L002AC458\n")
    KH_VER(".word .L003166EC\n" ".word .L0031635C\n", ".word .L002AC7EC\n" ".word .L002AC46C\n")
    ".section .data\n"
    ".align 3\n"
    " techSeqTbl.4440:\n"
    ".byte 0xE0,0x00,0xD9,0x00,0xE2,0x00,0xDB,0x00,0xE6,0x00,0xDF,0x00,0xE1,0x00,0xDA,0x00\n"
    ".byte 0xE3,0x00,0xDC,0x00,0xE5,0x00,0xDE,0x00,0xE4,0x00,0xDD,0x00\n"
    ".byte 0x00,0x00,0x00,0x00\n"
    " pTechCateNameTbl.4441:\n"
    ".word D_004E3648\n" ".word D_004E3650\n" ".word D_004E3658\n" ".word D_004E3660\n"
    ".word D_004E3648\n" ".word D_004E3660\n" ".word D_004E3668\n"
    " pStrListTitle.4442:\n"
    ".word D_004E36C8\n"
    " scrollBarSeqTbl.4443:\n"
    ".byte 0x20,0x01,0x1D,0x01,0x23,0x01\n"
    ".byte 0x00,0x00\n"
    ".section .text");

INT CCamp::setDeckSetWin(INT nSetCmd) {
    FVECTOR vec;
    FVECTOR vec2;
    G_RGBA rgba;
    G_RGBA rgba2;
    static USHORT decksetSeqTbl[8] = { 201, 204, 202, 205, 201, 204, 196, 197 };
    static USHORT decksetWinSeqTbl[8] = { 198, 199, 200, 0, 193, 194, 195, 0 };
    float fLightXTbl[3] = { 128.0f, 128.0f, 128.0f };
    float fLightYTbl[3] = { -4.0f, -4.0f, -4.0f };
    INT ret = 0;
    INT fgSetOrClear = (m_nStep >= 0x29 && m_nStep < 0x2D) ? 0 : 4;

    switch (nSetCmd) {
    case 0:
        if (!m_pDeckSetWinCtrl) {
            m_pDeckSetWinCtrl = new KH_AT("(5594)") C2DSeqCtrl;
            m_pDeckSetWinCtrl->setLayer(0x10);
            m_pDeckSetWinCtrl->enableColorCtrl(TRUE);
            vec.set(0.0f, 0.0f, 0.0f, 1.0f);
            m_pDeckSetWinCtrl->setPos(vec);
            m_pDeckSetWinCtrl->addSeq(m_pSeqName, decksetWinSeqTbl[fgSetOrClear]);
            m_pDeckSetBC = new KH_AT("(5600)") CButtonCursor(0x12, m_pDeckSetWinCtrl, 0xCB);
        } else {
            m_pDeckSetWinCtrl->replaceSeq(0, m_pSeqName, decksetWinSeqTbl[fgSetOrClear]);
        }
        break;
    case 1:
        if (m_pDeckSetWinCtrl->isEnd(0)) {
            m_pDeckSetWinCtrl->replaceSeq(0, m_pSeqName, decksetWinSeqTbl[fgSetOrClear + 1]);
            m_pHelpFnt->setText(CCtdMan::getText(!fgSetOrClear ? 0x500409 : 0x50040B));

            m_pDeckSetBC->setDisp(TRUE, FALSE);
            ret = 1;
        }
    case 3:
        if (m_pDeckSetBC) {
            m_pDeckSetBC->moveLight(getFrameRateF());
        }
        break;
    case 4:
        m_pDeckSetWinCtrl->replaceSeq(0, m_pSeqName, decksetWinSeqTbl[fgSetOrClear + 2]);
        m_pDeckSetBC->setOut();
        m_pHelpFnt->setText(m_pMainCmdName[1][1][6]);
        break;
    case 5:
        ret = m_pDeckSetWinCtrl->isEnd(0) != 0;
        break;
    case 6:
        delete m_pDeckSetWinCtrl;
        m_pDeckSetWinCtrl = NULL;
        for (INT i = 0; i < 6; i++) {
            delete m_pDeckSetButton[i];
            m_pDeckSetButton[i] = NULL;
        }
        delete m_pDeckSetBC;
        m_pDeckSetBC = NULL;
        break;
    case 9:
        rgba.col = 0x80007878;
        for (INT i = 0; i < 6; i++) {
            delete m_pDeckSetButton[i];
            m_pDeckSetButton[i] = NULL;
        }
        C2DMan::getLayPosition(m_pLadName, 0xD, 2, vec, 0);
        C2DMan::getLayPosition(m_pLadName, 0xD, 3, vec2, 0);
        for (INT i = 0; i < 3; i++) {
            INT nSetSeqIdx = i == CCard::getCurrentDeckNo() ? 0 : 2;
            CButton* pButton = m_pDeckSetButton[i] = new KH_AT("(5656)") CButton;
            pButton->init(m_pSeqName, 0x10, decksetSeqTbl, vec, m_pDeckSetWinCtrl, CCard::getDeckName(i), 0xE, 2.0f, 1.0f,
                          0x40, NULL, nSetSeqIdx + fgSetOrClear);
            if (nSetSeqIdx == 0) {
                m_pDeckSetEquipFnt = new KH_AT("(5663)") C2DFnt(0, 0, &rgba, 0xD, CCtdMan::getText(0x500003), 0x40);
                pButton->get2DSeqCtrl()->addFntShift(m_pSeqName, m_pDeckSetEquipFnt, 149.0f, 0.0f);
                rgba2.col = 0x80404080;
            } else {
                rgba2.col = !fgSetOrClear ? 0x80406331 : 0x80404040;
            }
            pButton = m_pDeckSetButton[i + 3] = new KH_AT("(5674)") CButton;
            pButton->init(m_pSeqName, 0x10, decksetSeqTbl, vec2, m_pDeckSetWinCtrl, CCtdMan::getText(i + 0x500000), 0xE, 14.0f, 1.0f,
                          0x40, &rgba2, ++nSetSeqIdx + fgSetOrClear);
            vec.y += 26.0f;
            vec2.y += 26.0f;
        }
        m_pDeckSetBC->select(m_nDeckSetSelect, m_pDeckSetButton, fLightXTbl, fLightYTbl);
        break;
    }
    return ret;
}

/* Parked: 23 of 1407 lines (2026-11-13; drop the data block after the INCLUDE_ASM to build the draft): only the spill
   slots of seven PRE-hoisted values differ (orig 0x74 &m_pMCInfoNumFnt, 0x78 &m_nMapKind, 0x7c i + 1, 0x80 nKind - 161,
   0x84 &m_pMCCard, 0x88 &m_pMCFnt, 0x8c &m_pMCInfoNumCtrl; ours 0x74 NumCtrl first, i + 1 before MapKind). Slots follow
   the PRE pseudo numbers = gcse expression-hash bucket order (-dG: raw hash of (plus (reg R) C) = 13258 + R + C, this
   is r116; bucket = raw % S, S = max_cuid / 2 | 1). Solved (build/camp/solve.py, build/camp/probe.py in the 2026-11-08
   worker's worktree: S and the PRE order of one variant in about a second): the original needs S = 703 (ours 701:
   max_cuid 1400, four more insns at gcse give 703, eight give 705) and the outer i of the 2x5 loop as pseudo >= 313
   (ours 303: ten more pseudos created before that loop); no other S in 681-729 fits. A filler variant matches
   (V1-V3, stable over rebuilds): four empty asms after the scrollBarSeqTbl line plus ten unused INT locals there;
   rejected by the user on 2026-11-08 (invented locals are not the original's source). Ruled out as the pseudo
   source: HeapMan.h's operator new (a block-local UINT nRA without the statement expression: no change; `UINT nRA =
   RET_ADDR()` +1 pseudo per new, S unchanged, a global header change), CtdMan.h instance() without the reference
   local (+2 per getText, global), chains p2DSeqCtrl/p2DCard = at the 5731/5753 news (+1 pseudo, +1 insn each:
   with them two empty asms reach S 703 but i is only 305), `new T()`, the 4-argument vec.set, casts, split chains
   (-2), member-first chains (S 699). Likely natural source, not yet tested where it counts: the original sets cards
   with an inline CARD member at CardKind.h:553-558 (.mdebug block "CARD *this; USHORT inKind", line 553 = the this
   setup), i.e. CARD::init(USHORT inKind, BYTE inNumber = 0xFF), in CardKind.h with cardDataTbl since 2026-11-10; this unit keeps the
   _setCardNK helper; a probe with the member form in case 9 alone changed nothing (it comes after the loop), so the
   missing pseudos must come from header inlines used before it (CardKind.h/Card.h forms, operator new, getText).
   Found 2026-11-08 from the .mdebug block tree (33 -> 23): nColSeq/cardOld/pCampRoomInfo declared in the switch
   body, scrollBarSeqTbl in the if block, case 9 without braces, one if for the number loop's display test, no `n`
   local for the scroll ratio, p2DFnt chains at the four m_pMCInfoFnt news and the two m_pMCFnt news, idx = i +
   m_nMCSelSelectTop. Earlier: permuter 15 min (59 -> 58), shared function-level i/j (256), j only (38). Third pass
   (2026-11-08), two open shape differences in the .mdebug tree: (1) the original keeps cardOld as a register variable
   (cardOld/19 = the lw s3,0x160 target); ours has no cardOld symbol: `cardOld = m_pMCInfoCard->m_card` goes through
   the trivial operator= (ADDRESSOF), loads into a temp and copies, and gcse cprop then replaces the one use (the
   m_nKind test) and deletes cardOld; `cardOld.m_nId = ...m_card.m_nId` makes the same temp + copy (25); case 9 is past
   the loop, so it does not supply the missing pseudos anyway. (2) Every operator new block is `{ nRA { } }` in the
   original, `{ { nRA { } } }` here: System/HeapMan.h written as `{ UINT nRA; { asm } return com_malloc_d(size, msg,
   nRA); }` (the ref's UINT nRA, lines 150-153) gives the original's tree, but S and the pseudo numbers stay (701,
   i r303): no help here, and a global header change (not made). 2026-11-09 (-dr dump): the 184 pseudos before the outer
   i are about 6 per operator new, 4 per new-expression's null check/ctor, 11 per virtual call (vtable load, delta,
   float constants), 14 per CCtdMan::getText; the 10 missing ones are one such construct's worth with no code of its
   own (in setTechWin a used but .mdebug-invisible local, pFnt, was the missing piece). 2026-11-10: our max_cuid is
   exactly 1400 (1-3 empty asms keep S 701, 4 give 703); a wider solve (S 601-1001, this fixed) still allows only
   S 703 with i in r313..~420 (any nKind). Block trees equal except cardOld and the CARD inline's names. Tested
   without effect on S/i: CARD::init(USHORT inKind, BYTE inNumber = 0xFF) as a CARD member for both case-9 uses
   (the .mdebug's "CARD *this; USHORT inKind", CardKind.h:553-558; cardDataTbl is in CardKind.h since 2026-11-10),
   vec.set with 4 arguments, `= NULL` on the three pointer locals; separate `p2DFnt = new ...; m_pMCInfoFnt[0] =
   p2DFnt;` gives S 699/i r301 (the chain is +2 pseudos, ~+4 insns), the 5731/5753 chains +1 pseudo each. Original
   line markers: each C2DFnt new statement ends one line below its "(N)" line (5737-5738, 5740-5741, 5743-5744,
   5747-5748), 5746/5749 carry no code, and vec.set is an inline at Coord.h:730 (not Vector.h). 2026-11-10 (after
   the Coord.h unification, re-measured 23, S 701, i r303): all header line markers have the original's counts
   except CardKind.h 553-558 (54 insns: the CARD::init inline of case 9, ours _setCardNK in this file); probes
   (build/camp/probe2.py: S and the outer i's pseudo per variant): separate stores for the case-0 chain, the 4-argument
   vec.set, a p2DSeqCtrl chain at 5731: no change; CtdMan.h instance() without its reference local i r307, plus a
   reference local in getText r309, S 701 both (header-wide, not kept); cardOld forms: `.m_nId =` S 699,
   `(CARD)`/`CARD(...)` S 701, none of them keeps cardOld as the register variable the original has (ours: the
   trivial operator= stores through ADDRESSOF into a copy that gcse cprop deletes; the original loads into cardOld
   directly). 2026-11-13 (retail, `retail_units.py try --show`): compiled for retail the draft differs from retail's
   function only in the same spill slots (same size; retail 0x74 this+0x7CC, 0x78 this+0x7F0, 0x7C this+0x808, 0x80
   i + 1, 0x84 this+0x197C, 0x88 this+0x830; ours i + 1 at 0x7C, 0x830 at 0x84, 0x197C at 0x88). Our retail S is 631
   (max_cuid 1261, outer i r275); retail needs S 633 (+3..6 insns, i <= r308), 635 (+7..10, i r267..288) or 637
   (+11..14, i r261..268). Lead (not confirmed): setTechWin's retail order needs 1-4 more insns at gcse too (S 263, its
   beta 0-2); "+1 insn per `new C2DFnt`" fits all four (setTechWin 2 news: 555/S 277 beta, 525/S 263 retail; here 7:
   1407/S 703 beta, 1268/S 635 retail, with the outer i needing 10-13 more pseudos in both builds, i.e. about 3 per
   C2DFnt new before the loop). A `C2DFnt* p = m_pX = new` chain where the source has none adds no insn (setTechWin 5390). */
#if 0
INT CCamp::setMapCardWin(INT nSetCmd) {
    static char* pStrNum[4] = { "%02d/99", "%02d", "\x81~%02d", "" };
    static USHORT mcTabSeqTbl[5] = { 330, 329, 327, 326, 328 };
    static unsigned char colSeqTbl[3] = { 116, 95, 75 };
    FVECTOR vec;
    FVECTOR vec2;
    C2DSeqCtrl* p2DSeqCtrl;
    C2DFnt* p2DFnt;
    C2DCard* p2DCard;
    G_RGBA rgba;
    FLOAT fRatioHeight;
    FLOAT fRatioPosY;
    char chrTmp[64];
    CARD card;
    USHORT nKind;
    INT idx;
    INT ret;

    ret = 0;
    switch (nSetCmd) {
        USHORT nColSeq;
        CARD cardOld;
        CampRoomInfo* pCampRoomInfo;
    case 0:
        m_nMCSelCateSelect = m_nMCSelSelect = m_nMCSelSelectTop = m_nMCSelNumSelect = 0;
        initCardData(1);
        initCardMapKind(m_nMCSelCateSelect);
        if (!m_pMCInfoWinCtrl) {
            static USHORT scrollBarSeqTbl[3] = { 288, 285, 291 };
            rgba.col = 0x807C7C7C;
            m_pMCInfoWinCtrl = new KH_AT("(5731)") C2DSeqCtrl;
            m_pMCInfoWinCtrl->setLayer(0x10);
            m_pMCInfoWinCtrl->enableColorCtrl(TRUE);
            C2DMan::getLayPosition(m_pLadName, 0x16, 2, vec, 0);
            m_pMCInfoWinCtrl->setPos(vec);
            m_pMCInfoWinCtrl->addSeq(m_pSeqName, 0x143);
            p2DFnt = m_pMCInfoFnt[0] = new KH_AT("(5737)") C2DFnt(0, 0, &rgba, 0xE, NULL, 0x40);
            m_pMCInfoWinCtrl->addFntShift(m_pSeqName, p2DFnt, -134.0f, -49.0f);
            p2DFnt = m_pMCInfoFnt[1] = new KH_AT("(5740)") C2DFnt(0, 0, &rgba, 0xD, CCtdMan::getText(0x500300), 0x40);
            m_pMCInfoWinCtrl->addFntShift(m_pSeqName, p2DFnt, -72.0f, 6.0f);
            p2DFnt = m_pMCInfoFnt[2] = new KH_AT("(5743)") C2DFnt(0, 0, &rgba, 0xD, CCtdMan::getText(0x500301), 0x40);
            m_pMCInfoWinCtrl->addFntShift(m_pSeqName, p2DFnt, -72.0f, 43.0f);
            p2DFnt = m_pMCInfoFnt[3] = new KH_AT("(5747)") C2DFnt(0, 0, &rgba, 0xE, NULL, 0x40);
            m_pMCInfoWinCtrl->addFntShift(m_pSeqName, p2DFnt, -137.0f, 102.0f);
            m_pMCInfoWinCtrl->addSeq(m_pSeqName, 0x13D);
            m_pMCInfoWinCtrl->addSeq(m_pSeqName, 0x13D);
            m_pMCInfoCard = new KH_AT("(5753)") C2DCard;
            m_pMCInfoCard->setParent(m_pMCInfoWinCtrl);
            m_pMCInfoCard->setLayer(0x11);
            m_pMCInfoCard->enableColorCtrl(TRUE);
            m_pMCInfoCard->addSeqShift(m_pSeqName, 0x158, -110.0f, 26.0f);
            vec.set(33.0f, -23.0f, 0.0f);
            for (INT i = 0; i < 2; i++) {
                for (INT j = 0; j < 5; j++) {
                    idx = i * 5 + j;
                    p2DSeqCtrl = m_pMCInfoNumCtrl[idx] = new KH_AT("(5764)") C2DSeqCtrl;
                    p2DSeqCtrl->setParent(m_pMCInfoWinCtrl);
                    p2DSeqCtrl->setLayer(0x11);
                    p2DSeqCtrl->enableColorCtrl(TRUE);
                    p2DSeqCtrl->setLocalPos(vec);
                    p2DSeqCtrl->addSeq(m_pSeqName, idx * 2 + 0x60);
                    p2DFnt = m_pMCInfoNumFnt[idx] = new KH_AT("(5772)") C2DFnt(0, 0, &rgba, 0xD, NULL, 0x40);
                    p2DFnt->allocText(chrTmp);
                    p2DSeqCtrl->addFntShift(m_pSeqName, p2DFnt, 14.0f, 7.0f);
                    vec.y += 20.0f;
                }
                vec.x += 62.0f;
                vec.y -= 100.0f;
            }
            p2DSeqCtrl = m_pMCSelWinCtrl[0] = new KH_AT("(5784)") C2DSeqCtrl;
            p2DSeqCtrl->setLayer(0x11);
            p2DSeqCtrl->enableColorCtrl(TRUE);
            C2DMan::getLayPosition(m_pLadName, 0x16, 1, vec, 0);
            p2DSeqCtrl->setPos(vec);
            p2DSeqCtrl->addSeq(m_pSeqName, 0x13A);
            p2DSeqCtrl = m_pMCSelWinCtrl[1] = new KH_AT("(5792)") C2DSeqCtrl;
            p2DSeqCtrl->setLayer(0x10);
            p2DSeqCtrl->enableColorCtrl(TRUE);
            p2DSeqCtrl->setParent(m_pMCSelWinCtrl[0]);
            C2DMan::getLayPosition(m_pLadName, 0x15, 0, vec, 0);
            p2DSeqCtrl->addSeqShift(m_pSeqName, 0x14B, vec.x, vec.y);
            p2DFnt = m_pMCFnt[0] = new KH_AT("(5799)") C2DFnt(0, 0, &rgba, 0xC, NULL, 0x40);
            p2DSeqCtrl->addFntShift(m_pSeqName, p2DFnt, 22.0f, 1.0f);
            rgba.col = 0x80007878;
            for (INT i = 0; i < 4; i++) {
                p2DFnt = m_pMCFnt[i + 1] = new KH_AT("(5804)") C2DFnt(0, 0, &rgba, 0xC, NULL, 0x40);
                p2DSeqCtrl->addFntShift(m_pSeqName, p2DFnt, i * 38.0f + -58.0f, 18.0f);
            }
            p2DSeqCtrl = m_pMCSelWinCtrl[2] = new KH_AT("(5810)") C2DSeqCtrl;
            p2DSeqCtrl->setLayer(0xF);
            p2DSeqCtrl->enableColorCtrl(TRUE);
            p2DSeqCtrl->setParent(m_pMCSelWinCtrl[0]);
            p2DSeqCtrl->addSeq(m_pSeqName, 0x14A);
            for (INT i = 0; i < 3; i++) {
                for (INT j = 0; j < 3; j++) {
                    p2DCard = m_pMCCard[i * 3 + j] = new KH_AT("(5820)") C2DCard;
                    p2DCard->setParent(m_pMCSelWinCtrl[0]);
                    p2DCard->setLayer(0x11);
                    p2DCard->enableColorCtrl(TRUE);
                    C2DMan::getLayPosition(m_pLadName, 7, i * 3 + j + 3, vec, 0);
                    p2DCard->setLocalPos(vec);
                    p2DCard->addSeq(m_pSeqName, 0xB6);
                }
            }
            m_pMCSelScrollBar = new KH_AT("(5832)") CScrollBar;
            vec.set(68.0f, -56.0f, 0.0f);
            m_pMCSelScrollBar->init(m_pSeqName, 0x14, scrollBarSeqTbl, vec, 117.0f, m_pMCSelWinCtrl[0]);
        } else {
            m_pMCInfoWinCtrl->replaceSeq(0, m_pSeqName, 0x143);
        }
        break;
    case 1:
        if (m_pMCInfoWinCtrl->isEnd(0)) {
            m_pMCInfoWinCtrl->replaceSeq(0, m_pSeqName, 0x144);
            m_pMCSelWinCtrl[0]->replaceSeq(0, m_pSeqName, 0x13B);
            m_pHelpFnt->setText(CCtdMan::getText(0x500402));
            ret = 1;
        }
        break;
    case 4:
        m_pMCInfoWinCtrl->replaceSeq(0, m_pSeqName, 0x145);
        m_pMCSelWinCtrl[0]->replaceSeq(0, m_pSeqName, 0x13C);
        m_pHelpFnt->setText(m_pMainCmdName[1][1][6]);
        break;
    case 5:
        ret = m_pMCInfoWinCtrl->isEnd(0) != 0;
        break;
    case 6:
        SAFE_DELETE(m_pMCInfoWinCtrl);
        for (INT i = 0; i < 3; i++) {
            SAFE_DELETE(m_pMCSelWinCtrl[i]);
        }
        for (INT i = 0; i < 9; i++) {
            SAFE_DELETE(m_pMCCard[i]);
        }
        SAFE_DELETE(m_pMCSelScrollBar);
        for (INT i = 0; i < 10; i++) {
            SAFE_DELETE(m_pMCInfoNumCtrl[i]);
        }
        SAFE_DELETE(m_pMCInfoCard);
        break;
    case 10:
        m_nMCSelSelectTop = 0;
        m_pMCSelWinCtrl[2]->replaceSeq(0, m_pSeqName, mcTabSeqTbl[m_nMCSelCateSelect]);
        initCardMapKind(m_nMCSelCateSelect);
    case 9:
        nColSeq = 0;
        nKind = 0;
        for (INT i = 3; i > 0; i--) {
            m_pMCInfoCard->destroy(i);
        }
        cardOld = m_pMCInfoCard->m_card;
        if (!m_bCategorySelectMode) {
            if (m_nMapKind[m_nMCSelCateSelect]) {
                nKind = m_nMapKindTbl[m_nMCSelSelect];
                _setCardNK(card, nKind, -1);
                m_pMCInfoCard->create(card);
                if (nKind != cardOld.m_nKind) {
                    m_pMCInfoCard->restart(0, 0.0f);
                }
                m_pMCInfoFnt[0]->setText(card.getName());
                nColSeq = colSeqTbl[card.m_nCategory21 - 9];
            }
        } else {
            m_pMCInfoFnt[0]->setText(pStrNum[3]);
        }
        pCampRoomInfo = getRoomInfo(nKind);
        if (pCampRoomInfo) {
            m_pMCInfoFnt[3]->setText(CCtdMan::getText(pCampRoomInfo->nRoomExplane));
            m_pMCInfoWinCtrl->replaceSeqShift(5, m_pSeqName, pCampRoomInfo->nEnemyVolume + 0x13D, -18.0f, 17.0f);
            m_pMCInfoWinCtrl->replaceSeqShift(6, m_pSeqName, pCampRoomInfo->nRoomVolume + 0x13D, -18.0f, 55.0f);
            m_pMCInfoWinCtrl->enableDisp(5, TRUE);
            m_pMCInfoWinCtrl->enableDisp(6, TRUE);
        } else {
            m_pMCInfoFnt[3]->setText(pStrNum[3]);
            m_pMCInfoWinCtrl->enableDisp(5, FALSE);
            m_pMCInfoWinCtrl->enableDisp(6, FALSE);
        }
        sprintf(chrTmp, pStrNum[0], m_nMapKindTotal[0]);
        m_pMCFnt[0]->allocText(chrTmp);
        sprintf(chrTmp, pStrNum[1], m_nMapKindTotal[1]);
        m_pMCFnt[1]->allocText(chrTmp);
        sprintf(chrTmp, pStrNum[1], m_nMapKindTotal[2]);
        m_pMCFnt[2]->allocText(chrTmp);
        sprintf(chrTmp, pStrNum[1], m_nMapKindTotal[3]);
        m_pMCFnt[3]->allocText(chrTmp);
        sprintf(chrTmp, pStrNum[1], m_nMapKindTotal[4]);
        m_pMCFnt[4]->allocText(chrTmp);
        for (INT i = 0; i < 9; i++) {
            idx = i + m_nMCSelSelectTop;
            p2DCard = m_pMCCard[i];
            p2DCard->destroy(3);
            p2DCard->destroy(2);
            p2DCard->destroy(1);
            if (idx < m_nMapKind[m_nMCSelCateSelect]) {
                _setCardNK(card, m_nMapKindTbl[idx], -1);
                p2DCard->create(card);
                if (card.m_nCategory20 == 10) {
                    p2DCard->restart(p2DCard->getNumSeq() - 1, m_iFrame60);
                }
            }
        }
        if (nKind) {
            for (idx = 0; idx < m_nCardKindMax; idx++) {
                if (m_nCardKind[idx] == nKind) break;
            }
            ASSERT_AT(5964, idx < m_nCardKindMax);
            if (nKind == 0xB9) nColSeq = 0x73;
        }
        for (INT i = 0; i < 10; i++) {
            p2DSeqCtrl = m_pMCInfoNumCtrl[i];
            if ((USHORT)(nKind - 0xA1) < 25) {
                if (!m_bCategorySelectMode && m_nMapKind[m_nMCSelCateSelect] && (nColSeq != 0x73 || i == 0)) {
                    INT n = m_nCardKind10[idx][i];
                    p2DSeqCtrl->enableDisp(TRUE);
                    p2DSeqCtrl->replaceSeq(0, m_pSeqName, nColSeq + i * 2 + (n == 0));
                    sprintf(chrTmp, pStrNum[2], n);
                    m_pMCInfoNumFnt[i]->allocText(chrTmp);
                } else {
                    p2DSeqCtrl->enableDisp(FALSE);
                }
            } else {
                p2DSeqCtrl->enableDisp(FALSE);
            }
        }
        idx = (m_nMapKind[m_nMCSelCateSelect] + 2) / 3;
        if (!idx) idx = 1;
        fRatioHeight = 3.0f / idx;
        if (fRatioHeight > 1.0f) {
            fRatioHeight = 1.0f;
        }
        fRatioPosY = (FLOAT)(m_nMCSelSelectTop / 3) / idx;
        if (fRatioPosY > 1.0f) {
            fRatioPosY = 1.0f;
        }
        m_pMCSelScrollBar->change(fRatioPosY, fRatioHeight);
        break;
    }
    return ret;
}
#else
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Form/Menu/Camp", setMapCardWin__5CCampi);
#endif
// setMapCardWin__5CCampi: its .rodata/.data, for its INCLUDE_ASM; remove with it.
asm(".section .rodata\n"
    ".align 3\n"
    KH_DEBUG("D_004E3968:\n" ".byte 0x63,0x3A,0x2F,0x4B,0x48,0x43,0x4F,0x4D,0x2F,0x4D,0x61,0x69,0x6E,0x41,0x70,0x70\n")
    KH_DEBUG(".byte 0x2F,0x46,0x6F,0x72,0x6D,0x2F,0x4D,0x65,0x6E,0x75,0x2F,0x43,0x61,0x6D,0x70,0x2E\n")
    KH_DEBUG(".byte 0x63,0x70,0x70,0x28,0x35,0x37,0x33,0x31,0x29,0x00,0x00,0x00,0x00,0x00,0x00,0x00\n")
    KH_DEBUG("D_004E3998:\n" ".byte 0x63,0x3A,0x2F,0x4B,0x48,0x43,0x4F,0x4D,0x2F,0x4D,0x61,0x69,0x6E,0x41,0x70,0x70\n")
    KH_DEBUG(".byte 0x2F,0x46,0x6F,0x72,0x6D,0x2F,0x4D,0x65,0x6E,0x75,0x2F,0x43,0x61,0x6D,0x70,0x2E\n")
    KH_DEBUG(".byte 0x63,0x70,0x70,0x28,0x35,0x37,0x33,0x37,0x29,0x00,0x00,0x00,0x00,0x00,0x00,0x00\n")
    KH_DEBUG("D_004E39C8:\n" ".byte 0x63,0x3A,0x2F,0x4B,0x48,0x43,0x4F,0x4D,0x2F,0x4D,0x61,0x69,0x6E,0x41,0x70,0x70\n")
    KH_DEBUG(".byte 0x2F,0x46,0x6F,0x72,0x6D,0x2F,0x4D,0x65,0x6E,0x75,0x2F,0x43,0x61,0x6D,0x70,0x2E\n")
    KH_DEBUG(".byte 0x63,0x70,0x70,0x28,0x35,0x37,0x34,0x30,0x29,0x00,0x00,0x00,0x00,0x00,0x00,0x00\n")
    KH_DEBUG("D_004E39F8:\n" ".byte 0x63,0x3A,0x2F,0x4B,0x48,0x43,0x4F,0x4D,0x2F,0x4D,0x61,0x69,0x6E,0x41,0x70,0x70\n")
    KH_DEBUG(".byte 0x2F,0x46,0x6F,0x72,0x6D,0x2F,0x4D,0x65,0x6E,0x75,0x2F,0x43,0x61,0x6D,0x70,0x2E\n")
    KH_DEBUG(".byte 0x63,0x70,0x70,0x28,0x35,0x37,0x34,0x33,0x29,0x00,0x00,0x00,0x00,0x00,0x00,0x00\n")
    KH_DEBUG("D_004E3A28:\n" ".byte 0x63,0x3A,0x2F,0x4B,0x48,0x43,0x4F,0x4D,0x2F,0x4D,0x61,0x69,0x6E,0x41,0x70,0x70\n")
    KH_DEBUG(".byte 0x2F,0x46,0x6F,0x72,0x6D,0x2F,0x4D,0x65,0x6E,0x75,0x2F,0x43,0x61,0x6D,0x70,0x2E\n")
    KH_DEBUG(".byte 0x63,0x70,0x70,0x28,0x35,0x37,0x34,0x37,0x29,0x00,0x00,0x00,0x00,0x00,0x00,0x00\n")
    KH_DEBUG("D_004E3A58:\n" ".byte 0x63,0x3A,0x2F,0x4B,0x48,0x43,0x4F,0x4D,0x2F,0x4D,0x61,0x69,0x6E,0x41,0x70,0x70\n")
    KH_DEBUG(".byte 0x2F,0x46,0x6F,0x72,0x6D,0x2F,0x4D,0x65,0x6E,0x75,0x2F,0x43,0x61,0x6D,0x70,0x2E\n")
    KH_DEBUG(".byte 0x63,0x70,0x70,0x28,0x35,0x37,0x35,0x33,0x29,0x00,0x00,0x00,0x00,0x00,0x00,0x00\n")
    KH_DEBUG("D_004E3A88:\n" ".byte 0x63,0x3A,0x2F,0x4B,0x48,0x43,0x4F,0x4D,0x2F,0x4D,0x61,0x69,0x6E,0x41,0x70,0x70\n")
    KH_DEBUG(".byte 0x2F,0x46,0x6F,0x72,0x6D,0x2F,0x4D,0x65,0x6E,0x75,0x2F,0x43,0x61,0x6D,0x70,0x2E\n")
    KH_DEBUG(".byte 0x63,0x70,0x70,0x28,0x35,0x37,0x36,0x34,0x29,0x00,0x00,0x00,0x00,0x00,0x00,0x00\n")
    KH_DEBUG("D_004E3AB8:\n" ".byte 0x63,0x3A,0x2F,0x4B,0x48,0x43,0x4F,0x4D,0x2F,0x4D,0x61,0x69,0x6E,0x41,0x70,0x70\n")
    KH_DEBUG(".byte 0x2F,0x46,0x6F,0x72,0x6D,0x2F,0x4D,0x65,0x6E,0x75,0x2F,0x43,0x61,0x6D,0x70,0x2E\n")
    KH_DEBUG(".byte 0x63,0x70,0x70,0x28,0x35,0x37,0x37,0x32,0x29,0x00,0x00,0x00,0x00,0x00,0x00,0x00\n")
    KH_DEBUG("D_004E3AE8:\n" ".byte 0x63,0x3A,0x2F,0x4B,0x48,0x43,0x4F,0x4D,0x2F,0x4D,0x61,0x69,0x6E,0x41,0x70,0x70\n")
    KH_DEBUG(".byte 0x2F,0x46,0x6F,0x72,0x6D,0x2F,0x4D,0x65,0x6E,0x75,0x2F,0x43,0x61,0x6D,0x70,0x2E\n")
    KH_DEBUG(".byte 0x63,0x70,0x70,0x28,0x35,0x37,0x38,0x34,0x29,0x00,0x00,0x00,0x00,0x00,0x00,0x00\n")
    KH_DEBUG("D_004E3B18:\n" ".byte 0x63,0x3A,0x2F,0x4B,0x48,0x43,0x4F,0x4D,0x2F,0x4D,0x61,0x69,0x6E,0x41,0x70,0x70\n")
    KH_DEBUG(".byte 0x2F,0x46,0x6F,0x72,0x6D,0x2F,0x4D,0x65,0x6E,0x75,0x2F,0x43,0x61,0x6D,0x70,0x2E\n")
    KH_DEBUG(".byte 0x63,0x70,0x70,0x28,0x35,0x37,0x39,0x32,0x29,0x00,0x00,0x00,0x00,0x00,0x00,0x00\n")
    KH_DEBUG("D_004E3B48:\n" ".byte 0x63,0x3A,0x2F,0x4B,0x48,0x43,0x4F,0x4D,0x2F,0x4D,0x61,0x69,0x6E,0x41,0x70,0x70\n")
    KH_DEBUG(".byte 0x2F,0x46,0x6F,0x72,0x6D,0x2F,0x4D,0x65,0x6E,0x75,0x2F,0x43,0x61,0x6D,0x70,0x2E\n")
    KH_DEBUG(".byte 0x63,0x70,0x70,0x28,0x35,0x37,0x39,0x39,0x29,0x00,0x00,0x00,0x00,0x00,0x00,0x00\n")
    KH_DEBUG("D_004E3B78:\n" ".byte 0x63,0x3A,0x2F,0x4B,0x48,0x43,0x4F,0x4D,0x2F,0x4D,0x61,0x69,0x6E,0x41,0x70,0x70\n")
    KH_DEBUG(".byte 0x2F,0x46,0x6F,0x72,0x6D,0x2F,0x4D,0x65,0x6E,0x75,0x2F,0x43,0x61,0x6D,0x70,0x2E\n")
    KH_DEBUG(".byte 0x63,0x70,0x70,0x28,0x35,0x38,0x30,0x34,0x29,0x00,0x00,0x00,0x00,0x00,0x00,0x00\n")
    KH_DEBUG("D_004E3BA8:\n" ".byte 0x63,0x3A,0x2F,0x4B,0x48,0x43,0x4F,0x4D,0x2F,0x4D,0x61,0x69,0x6E,0x41,0x70,0x70\n")
    KH_DEBUG(".byte 0x2F,0x46,0x6F,0x72,0x6D,0x2F,0x4D,0x65,0x6E,0x75,0x2F,0x43,0x61,0x6D,0x70,0x2E\n")
    KH_DEBUG(".byte 0x63,0x70,0x70,0x28,0x35,0x38,0x31,0x30,0x29,0x00,0x00,0x00,0x00,0x00,0x00,0x00\n")
    KH_DEBUG("D_004E3BD8:\n" ".byte 0x63,0x3A,0x2F,0x4B,0x48,0x43,0x4F,0x4D,0x2F,0x4D,0x61,0x69,0x6E,0x41,0x70,0x70\n")
    KH_DEBUG(".byte 0x2F,0x46,0x6F,0x72,0x6D,0x2F,0x4D,0x65,0x6E,0x75,0x2F,0x43,0x61,0x6D,0x70,0x2E\n")
    KH_DEBUG(".byte 0x63,0x70,0x70,0x28,0x35,0x38,0x32,0x30,0x29,0x00,0x00,0x00,0x00,0x00,0x00,0x00\n")
    KH_DEBUG("D_004E3C08:\n" ".byte 0x63,0x3A,0x2F,0x4B,0x48,0x43,0x4F,0x4D,0x2F,0x4D,0x61,0x69,0x6E,0x41,0x70,0x70\n")
    KH_DEBUG(".byte 0x2F,0x46,0x6F,0x72,0x6D,0x2F,0x4D,0x65,0x6E,0x75,0x2F,0x43,0x61,0x6D,0x70,0x2E\n")
    KH_DEBUG(".byte 0x63,0x70,0x70,0x28,0x35,0x38,0x33,0x32,0x29,0x00,0x00,0x00,0x00,0x00,0x00,0x00\n")
    KH_DEBUG("D_004E3C38:\n" ".byte 0x69,0x64,0x78,0x20,0x3C,0x20,0x6D,0x5F,0x6E,0x43,0x61,0x72,0x64,0x4B,0x69,0x6E\n")
    KH_VER(".byte 0x64,0x4D,0x61,0x78,0x00,0x00,0x00,0x00\n", ".byte 0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00\n") /* retail: 8 zero bytes before the jump table */
    KH_VER("jtbl_004E3C50:\n" ".word .L00316EB0\n" ".word .L00317830\n" ".word .L00318140\n" ".word .L00318140\n", "jtbl_004E3C50:\n" ".word .L002ACEF0\n" ".word .L002AD768\n" ".word .L002AE000\n" ".word .L002AE000\n")
    KH_VER(".word .L003178E0\n" ".word .L00317940\n" ".word .L00317958\n" ".word .L00318140\n", ".word .L002AD7F0\n" ".word .L002AD850\n" ".word .L002AD868\n" ".word .L002AE000\n")
    KH_VER(".word .L00318140\n" ".word .L00317AD0\n" ".word .L00317A88\n" ".byte 0x00,0x00,0x00,0x00\n", ".word .L002AE000\n" ".word .L002AD9E0\n" ".word .L002AD998\n" ".byte 0x00,0x00,0x00,0x00\n")
    ".section .data\n"
    ".align 3\n"
    " pStrNum.4452:\n"
    ".word D_004E31F0\n" ".word D_004E3208\n" ".word D_004E33E8\n" ".word D_004E2948\n"
    " mcTabSeqTbl.4453:\n"
    ".byte 0x4A,0x01,0x49,0x01,0x47,0x01,0x46,0x01,0x48,0x01\n"
    ".byte 0x00,0x00,0x00,0x00,0x00,0x00\n"
    " colSeqTbl.4454:\n"
    ".byte 0x74,0x5F,0x4B\n"
    ".byte 0x00,0x00,0x00,0x00,0x00\n"
    " scrollBarSeqTbl.4455:\n"
    ".byte 0x20,0x01,0x1D,0x01,0x23,0x01\n"
    ".byte 0x00,0x00\n"
    ".section .text");
INT CCamp::setStatusWin(INT nSetCmd) {
    FVECTOR vec;
    FVECTOR vec2;
    C2DSeqCtrl* p2DSeqCtrl;
    C2DSeqObj* p2DSeqObj;
    G_RGBA rgba;
    FLOAT fRatioHeight;
    FLOAT fRatioPosY;
    static USHORT statusSeqTbl[16] = { 300, 300, 300, 299, 299, 297, 298, 298, 298, 300, 300, 300, 299, 297, 296, 298 };
    static USHORT techSeqTbl[7] = { 302, 307, 305, 306, 303, 352, 304 };
    static USHORT techTabSeqTbl[9] = { 335, 336, 337, 338, 339, 340, 341, 342, 343 };
    static char* statusStr[2] = { "%d", "%d/%d" };
    float fLightXTbl[8] = { 284.0f, 284.0f, 284.0f, 284.0f, 284.0f, 284.0f, 284.0f, 284.0f };
    float fLightYTbl[8] = { 168.0f, 168.0f, 168.0f, 168.0f, 168.0f, 168.0f, 168.0f, 168.0f };
    char buf[64];
    LPCTSTR pBuf;
    INT ret = 0;
    const unsigned char (*pTechCateTbl)[2];
    BYTE* pTechCate;
    INT nLine;
    INT idxStart;
    INT idxStart2;
    INT nFriendCnt;
    PLM::CPlayerManager* pPlayerManager;
    static USHORT scrollBarSeqTbl[3] = { 288, 285, 291 };

    switch (nSetCmd) {
    case 0:
        m_nStatusTechCateSelect = m_nStatusTechSelect = m_nStatusTechSelectTop = 0;
        m_nStatusMax = !m_bPlayerKind ? 9 : 7;
        m_nTechSelectMax = !m_bPlayerKind ? 0x56 : 0xE;
        if (!m_pStatusWinCtrl) {
            m_pStatusWinCtrl = new KH_AT("(6082)") C2DSeqCtrl;
            m_pStatusWinCtrl->setLayer(0x10);
            m_pStatusWinCtrl->enableColorCtrl(TRUE);
            C2DMan::getLayPosition(m_pLadName, 0x11, 3, vec, 0);
            m_pStatusWinCtrl->setPos(vec);
            m_pStatusWinCtrl->addSeq(m_pSeqName, 0x125);
            p2DSeqCtrl = m_pStatusTechWinCtrl[0] = new KH_AT("(6089)") C2DSeqCtrl;
            p2DSeqCtrl->setLayer(0x11);
            p2DSeqCtrl->enableColorCtrl(TRUE);
            C2DMan::getLayPosition(m_pLadName, 0x11, 2, vec, 0);
            p2DSeqCtrl->setPos(vec);
            p2DSeqCtrl->addSeq(m_pSeqName, 0x14C);
            p2DSeqCtrl = m_pStatusTechWinCtrl[1] = new KH_AT("(6096)") C2DSeqCtrl;
            p2DSeqCtrl->setLayer(0x10);
            p2DSeqCtrl->enableColorCtrl(TRUE);
            p2DSeqCtrl->addSeq(m_pSeqName, !m_bPlayerKind ? 0x14F : 0x154);
            p2DSeqCtrl->setParent(m_pStatusTechWinCtrl[0]);
            m_pStatusBC = new KH_AT("(6103)") CButtonCursor(0x12, m_pStatusTechWinCtrl[0], 0x12D);
            idxStart = m_bPlayerKind * 7;
            idxStart2 = m_bPlayerKind * 9;
            nFriendCnt = 0;
            pPlayerManager = PLM::CPlayerManager::getPLM();
            for (INT i = 0; i < m_nStatusMax; i++) {
                C2DMan::getLayPosition(m_pLadName, !m_bPlayerKind ? 0x11 : 0x13, i + 4, vec, 1);
                vec.y += 8.0f;
                CButton* pButton = m_pStatusButton[i] = new KH_AT("(6115)") CButton;
                pButton->init(m_pSeqName, 0x10, statusSeqTbl, vec, m_pStatusWinCtrl,
                                         i < 7 ? CCtdMan::getText(i + idxStart + 0x500200) : NULL, 0xE, 0.0f, 1.0f, 0x40,
                                         NULL, idxStart2 + i);
                pBuf = buf;
                switch (i) {
                case 0:
                    sprintf(buf, statusStr[0], CStatus::getLV());
                    break;
                case 1:
                    sprintf(buf, statusStr[0], CStatus::getEXP());
                    break;
                case 2: {
                    INT nextExp = CStatus::getLvExp(CStatus::getLV()) - CStatus::getEXP();
                    if (nextExp < 0) nextExp = 0;
                    sprintf(buf, statusStr[0], nextExp);
                    break;
                }
                case 3:
                    sprintf(buf, statusStr[1], CStatus::getStaus().HP, CStatus::getStaus().HPMax);
                    break;
                case 4:
                    sprintf(buf, statusStr[0], !m_bPlayerKind ? CStatus::getCPMax() : CStatus::getAP());
                    break;
                case 5:
                    if (!m_bPlayerKind) {
                        pBuf = CCard::getDeckName(CCard::getCurrentDeckNo());
                    } else {
                        sprintf(buf, statusStr[0], CStatus::getDPMax());
                    }
                    break;
                default: {
                    INT kind = 0;
                    while (nFriendCnt < 3) {
                        kind = pPlayerManager->getAppearFriendCard(nFriendCnt, 1);
                        nFriendCnt++;
                        if (kind) break;
                    }
                    if (kind) {
                        CARD card;
                        card.init(kind, 0xFF);
                        pBuf = card.getName();
                    } else {
                        pBuf = NULL;
                    }
                    break;
                }
                }
                rgba.col = i < 5 ? 0x80046060 : (i == 5 && m_bPlayerKind ? 0x80046060 : 0x807C7C7C);
                C2DFnt* p2DFnt = m_pStatusFnt[i] = new KH_AT("(6167)") C2DFnt(0, 0, &rgba, 0xE, NULL, 0x41);
                p2DFnt->allocText(pBuf);
                pButton->get2DSeqCtrl()->addFntShift(m_pSeqName, p2DFnt, 220.0f, 1.0f);
            }
            m_pStatusTechScrollBar = new KH_AT("(6173)") CScrollBar;
            vec.set(133.0f, -56.0f, 0.0f);
            m_pStatusTechScrollBar->init(m_pSeqName, 0x14, scrollBarSeqTbl, vec, 157.0f, m_pStatusTechWinCtrl[0]);
        } else {
            m_pStatusWinCtrl->replaceSeq(0, m_pSeqName, 0x125);
        }
        break;
    case 1:
        if (m_pStatusWinCtrl->isEnd(0)) {
            m_pStatusWinCtrl->replaceSeq(0, m_pSeqName, 0x126);
            m_pStatusTechWinCtrl[0]->replaceSeq(0, m_pSeqName, 0x14D);
            m_pStatusBC->setDisp(TRUE, FALSE);
            m_pHelpFnt->setText(CCtdMan::getText(0x500417));
            ret = 1;
        }
    case 3:
        if (m_pStatusBC) {
            m_pStatusBC->moveLight(getFrameRateF());
        }
        break;
    case 4:
        m_pStatusWinCtrl->replaceSeq(0, m_pSeqName, 0x127);
        m_pStatusTechWinCtrl[0]->replaceSeq(0, m_pSeqName, 0x14E);
        m_pStatusBC->setOut();
        m_pHelpFnt->setText(m_pMainCmdName[1][1][6]);
        break;
    case 5:
        ret = m_pStatusWinCtrl->isEnd(0) != 0;
        break;
    case 6:
        SAFE_DELETE(m_pStatusWinCtrl);
        SAFE_DELETE(m_pStatusTechWinCtrl[0]);
        SAFE_DELETE(m_pStatusTechWinCtrl[1]);
        for (INT i = 0; i < 9; i++) {
            SAFE_DELETE(m_pStatusButton[i]);
        }
        for (INT i = 0; i < 8; i++) {
            SAFE_DELETE(m_pStatusTechButton[i]);
        }
        SAFE_DELETE(m_pStatusBC);
        SAFE_DELETE(m_pStatusTechScrollBar);
        break;
    case 10: {
        m_nStatusTechSelect = m_nStatusTechSelectTop = 0;
        m_pStatusTechWinCtrl[1]->replaceSeq(0, m_pSeqName, techTabSeqTbl[m_nStatusTechCateSelect + m_bPlayerKind * 5]);
        for (INT i = 0; i < 5; i++) {
            m_nStatusTech[i] = 0;
        }
        m_nStatusTechSelectMax = 0;
        pTechCateTbl = !m_bPlayerKind ? m_techSoraCateTbl : m_techRikuCateTbl;
        for (INT i = 0; i < m_nTechSelectMax; i++) {
            pTechCate = (BYTE*)pTechCateTbl[i];
            if (CTechnic::canUse(pTechCate[0])) {
                BYTE nCate;
                if (!m_bPlayerKind) {
                    if (pTechCate[1] < 2) {
                        nCate = 0;
                    } else if (pTechCate[1] < 4) {
                        nCate = 1;
                    } else if (pTechCate[1] <= 7) {
                        nCate = 2;
                    } else {
                        nCate = 3;
                    }
                } else {
                    if (pTechCate[1] < 10) {
                        nCate = 0;
                    } else if (pTechCate[1] <= 11) {
                        nCate = 1;
                    } else {
                        nCate = 2;
                    }
                }
                m_nStatusTech[0]++;
                m_nStatusTech[nCate + 1]++;
                if (!m_nStatusTechCateSelect || nCate == m_nStatusTechCateSelect - 1) {
                    m_techCateTbl[m_nStatusTechSelectMax][0] = pTechCateTbl[i][0];
                    m_techCateTbl[m_nStatusTechSelectMax][1] = nCate + m_bPlayerKind * 4;
                    m_nStatusTechSelectMax++;
                }
            }
        }
        if (m_bCategorySelectMode) {
            vec.set(m_nStatusTechCateSelect ? m_nStatusTechCateSelect * 24.0f + -42.0f : -68.0f, -100.0f, 0.0f);
            m_pStatusBC->setLocalPos(vec);
        }
    }
    case 9: {
        if (m_nStatusTechSelectTop > m_nStatusTechSelect) {
            m_nStatusTechSelectTop = m_nStatusTechSelect;
        } else if (m_nStatusTechSelectTop + 7 < m_nStatusTechSelect) {
            m_nStatusTechSelectTop = m_nStatusTechSelect - 7;
        }
        rgba.col = 0x807C7C7C;
        for (INT i = 0; i < 8; i++) {
            SAFE_DELETE(m_pStatusTechButton[i]);
            INT idx = m_nStatusTechSelectTop + i;
            if (idx < m_nStatusTechSelectMax) {
                pTechCate = m_techCateTbl[idx];
                USHORT nTechKind = pTechCate[0];
                LPCTSTR pStr = CTechnic::getName(nTechKind);
                if (!m_bCategorySelectMode && idx == m_nStatusTechSelect) {
                    if (m_fgNewTbl[nTechKind] > 0) {
                        m_fgNewTbl[nTechKind] = -1;
                    }
                }
                C2DMan::getLayPosition(m_pLadName, 0x12, i + 2, vec, 0);
                CButton* pButton = m_pStatusTechButton[i] = new KH_AT("(6303)") CButton;
                pButton->init(m_pSeqName, 0x11, techSeqTbl, vec, m_pStatusTechWinCtrl[0], pStr, 0xF, 1.0f, 2.0f,
                                             0x40, &rgba, pTechCate[1]);
                if (m_fgNewTbl[nTechKind]) {
                    p2DSeqObj = pButton->get2DSeqCtrl()->addSeqShift(
                        m_pSeqName, m_fgNewTbl[nTechKind] > 0 ? 0x1E : 0x1D, 170.0f, -13.0f);
                    p2DSeqObj->restart(m_iFrame60);
                }
            }
        }
        if (!m_bCategorySelectMode) {
            m_pStatusBC->select(m_nStatusTechSelect - m_nStatusTechSelectTop, m_pStatusTechButton, fLightXTbl, fLightYTbl);
        }
        INT nMax = m_nStatusTechSelectMax;
        if (!nMax) {
            nMax = 1;
        }
        fRatioHeight = 8.0f / nMax;
        if (fRatioHeight > 1.0f) {
            fRatioHeight = 1.0f;
        }
        fRatioPosY = (FLOAT)m_nStatusTechSelectTop / nMax;
        if (fRatioPosY > 1.0f) {
            fRatioPosY = 1.0f;
        }
        m_pStatusTechScrollBar->change(fRatioPosY, fRatioHeight);
        break;
    }
    }
    return ret;
}
INT CCamp::setConfigWin(INT nSetCmd) {
    FVECTOR vec;
    FVECTOR vec2;
    static USHORT configSeqTbl[1] = { 258 };
    static USHORT configMSeqTbl[2] = { 251, 252 };
    static USHORT configSSeqTbl[2] = { 254, 255 };
    static USHORT configLSeqTbl[1] = { 257 };
    static USHORT* pConfigSizeTbl[14] = {
        configMSeqTbl, configMSeqTbl, configMSeqTbl, configMSeqTbl, configMSeqTbl, configMSeqTbl, configMSeqTbl,
        configMSeqTbl, configSSeqTbl, configSSeqTbl, configSSeqTbl, configMSeqTbl, configMSeqTbl, configLSeqTbl,
    };
    unsigned int helpCtdTbl[17] = {
        0x50041A, 0x50041B, 0x50041C, 0x50041D, 0x500418, 0x500419, 0x50041E, 0x50041F, 0x500420,
        0x500421, 0x500422, 0x500423, 0x500424, 0x500425, 0x500426, 0x500427, 0x500428,
    };
    float fLightXTbl[21] = {
        0.0f,   0.0f,   0.0f,   0.0f,   0.0f,   0.0f,  0.0f,  150.0f, 150.0f, 150.0f, 150.0f,
        150.0f, 150.0f, 150.0f, 150.0f, 95.0f, 95.0f, 95.0f, 150.0f, 150.0f, 315.0f,
    };
    static LPCTSTR pStrNull = "";
    INT ret = 0;
    INT ctdIdx;
    INT ctdIdx2;
    CButton* pButton; /* not in the .mdebug (kept in a register); one for both button loops: in retail the first loop's copy of the new-expression then stays apart (cse keeps the longer-living pButton) */
    switch (nSetCmd) {
    case 0:
        if (!m_pConfigWinCtrl) {
            m_pConfigWinCtrl = new KH_AT("(6403)") C2DSeqCtrl;
            m_pConfigWinCtrl->setLayer(0x10);
            m_pConfigWinCtrl->enableColorCtrl(TRUE);
            vec.set(0.0f, 0.0f, 0.0f, 1.0f);
            m_pConfigWinCtrl->setPos(vec);
            m_pConfigWinCtrl->addSeq(m_pSeqName, 0xF7);
            m_pConfigBC = new KH_AT("(6409)") CButtonCursor(0x12, m_pConfigWinCtrl, 0xFA);
        } else {
            m_pConfigWinCtrl->replaceSeq(0, m_pSeqName, 0xF7);
        }
        break;
    case 1:
        if (m_pConfigWinCtrl->isEnd(0)) {
            ret = 1;
            m_pConfigWinCtrl->replaceSeq(0, m_pSeqName, 0xF8);
            m_pConfigBC->setDisp(TRUE, FALSE);
        }
    case 3:
        if (m_pConfigBC) {
            m_pConfigBC->moveLight(getFrameRateF());
        }
        break;
    case 4:
        m_pConfigWinCtrl->replaceSeq(0, m_pSeqName, 0xF9);
        m_pConfigBC->setOut();
        m_pHelpFnt->setText(pStrNull);
        break;
    case 5:
        ret = m_pConfigWinCtrl->isEnd(0) != 0;
        break;
    case 6:
        delete m_pConfigWinCtrl;
        m_pConfigWinCtrl = NULL;
        for (INT i = 0; i < 21; i++) {
            delete m_pConfigButton[i];
            m_pConfigButton[i] = NULL;
        }
        delete m_pConfigBC;
        m_pConfigBC = NULL;
        break;
    case 9:
        for (INT i = 0; i < 21; i++) {
            delete m_pConfigButton[i];
            m_pConfigButton[i] = NULL;
        }
        for (INT i = 0; i < 7; i++) {
            pButton = m_pConfigButton[i] = new KH_AT("(6460)") CButton;
            C2DMan::getLayPosition(m_pLadName, 0xB, i + 2, vec, 0);
            pButton->init(m_pSeqName, 0x10, configSeqTbl, vec, m_pConfigWinCtrl, CCtdMan::getText(i + 0x500100), 0xD, 0.0f, 2.0f,
                          0x42, NULL, -1);
        }
        for (INT i = 0; i < 14; i++) {
            INT idx = i + 7;
            INT fgOffOn = 0;
            INT nLine;
            if (i < 8) {
                nLine = i >> 1;
                if (m_nConfigItemSelect[nLine] == (i & 1)) {
                    fgOffOn = 1;
                }
            } else if (i >= 8 && i < 11) {
                nLine = 4;
                if (m_nConfigItemSelect[4] == i - 8) {
                    fgOffOn = 1;
                }
            } else if (i < 13) {
                nLine = 5;
                if (m_nConfigItemSelect[5] != (i & 1)) {
                    fgOffOn = 1;
                }
            } else {
                nLine = 6;
            }
            pButton = m_pConfigButton[idx] = new KH_AT("(6489)") CButton;
            C2DMan::getLayPosition(m_pLadName, 0xB, i + 9, vec, 0);
            ctdIdx = i;
            if (nLine == 6) {
                ctdIdx = i + m_nConfigItemSelect[6];
            }
            pButton->init(m_pSeqName, 0x10, pConfigSizeTbl[i], vec, m_pConfigWinCtrl, CCtdMan::getText(ctdIdx + 0x500107), 0xD,
                          0.0f, 2.0f, 0x42, NULL, fgOffOn);
            if ((fgOffOn || nLine == 6) && nLine == m_nConfigSelect) {
                UINT nSeq = nLine == 4 ? 0xFD : (nLine == 6 ? 0x100 : 0xFA);
                ctdIdx2 = ctdIdx;
                m_pConfigBC->replaceSeq(0, m_pSeqName, nSeq);
                m_pConfigBC->select(idx, m_pConfigButton, fLightXTbl, NULL);
            }
        }
        m_pHelpFnt->setText(CCtdMan::getText(helpCtdTbl[ctdIdx2]));
        break;
    }
    return ret;
}

/* MATCH: the emulated debug new (an explicit operator new(size_t, char*) call + placement new) here: the original's
   new (__FILE__ "(N)") T (its .mdebug: HeapMan.h 150-153, no placement new) swaps two spill slots (112/116, also
   with HeapMan.h's function-level nRA of 2026-11-09); the other sites of the unit have the original form */
INT CCamp::setWorldmap(INT nSetCmd) {
    static LPCTSTR pSeqName = "worldmap:00";
    static LPCTSTR pSeqName2 = "world_icon:00";
    static char roomSeqTbl[16] = { 12, 17, 16, 21, 15, 20, 19, 25, 18, 24, 23, 28, 22, 27, 26, 29 };
    FVECTOR vec;
    FVECTOR vec2;
    C2DSeqCtrl* p2DSeqCtrl;
    C2DSeqObj* p2DSeqObj;
    G_RGBA rgba;
    CARD card;
    FLOAT fCol;
    INT idx;
    INT ret = FALSE;
    USHORT roomId;
    BYTE cardID;
    FLOOR::FloorLayout* pFloorLayout;
    FLOOR::RoomSrcInfo* pRoomSrcInfo;
    RoomInfo* pRoomInfo;
    DOOR_STAT doorStat;
    RCNORMA::NormaSrc normaSrc;
    static LPCTSTR pStrButton = "Zoom In{     \xF9" "DShow Criteria";
    INT fgExpectStart;
    INT nCard;
    INT j;

    if (!m_bPooWorld) {
        pFloorLayout = CSceneInfo::getCurrentFloorDataSrc();
    } else {
        pFloorLayout = CSceneInfo::getFloorDataSrc(CSceneInfo::getFloorIndex(100, TRUE), TRUE);
    }
    if (!pFloorLayout) {
        return TRUE;
    }
    rgba.col = 0x807C7C7C;
    switch (nSetCmd) {
    case 0:
        m_nSubStep = 0;
        if (!m_pWMNormaCtrl) {
            /* stored through a reference: in the original the next pSeqName load waits for this store */
            _ref(m_pWMMoveCtrl) = new (operator new(sizeof(C2DSeqCtrl) KH_ATARG("(6575)"))) C2DSeqCtrl;
            m_pWMMoveCtrl->addSeq(pSeqName, 0x4B);
            m_pWMMoveCtrl->enableColorCtrl(TRUE);
            m_nRoomCurrent = CSceneInfo::getCurrentRoomID() - 1;
            m_pWMNormaCtrl = new (operator new(sizeof(C2DSeqCtrl) KH_ATARG("(6580)"))) C2DSeqCtrl;
            vec.zero();
            m_pWMNormaCtrl->setCol(vec);
            m_pWMNormaCtrl->setLayer(0xD);
            m_pWMNormaCtrl->setParent(m_pWMMoveCtrl);
            m_pWMNormaCtrl->addSeq(pSeqName, 0x4B);
            m_pWMNormaCtrl->enableDisp(FALSE);
            m_pWMNormaCtrl->setLocalPos(vec);
            vec.init();
            if (!m_bPooWorld) {
                m_pWMExplaneCtrl = new (operator new(sizeof(C2DSeqCtrl) KH_ATARG("(6602)"))) C2DSeqCtrl;
                m_pWMExplaneCtrl->setLayer(0xF);
                m_pWMExplaneCtrl->addSeq(m_pSeqName, 0xB8);
                m_pWMExplaneCtrl->setPos(vec);
                m_pWMExplaneCtrl->enableColorCtrl(TRUE);
                m_pWMExplaneFnt = new (operator new(sizeof(C2DFnt) KH_ATARG("(6607)"))) C2DFnt(0, 0, &rgba, 0xE, pStrButton, 0x40);
                m_pWMExplaneCtrl->addFntShift(m_pSeqName, m_pWMExplaneFnt, 0.0f, 1.0f);
            }
            m_pWMCardCtrl = new (operator new(sizeof(C2DSeqCtrl) KH_ATARG("(6612)"))) C2DSeqCtrl;
            m_pWMCardCtrl->setLayer(0xE);
            m_pWMCardCtrl->addSeq(pSeqName, 0x27);
            m_pWMCardCtrl->addSeq(pSeqName2, (m_bPooWorld && (CGameWork::getInt(0, 0x23) & 0x40)) ? 1 : 0);
            m_pWMCardCtrl->setPos(vec);
            m_pWMFloorCtrl = new (operator new(sizeof(C2DSeqCtrl) KH_ATARG("(6618)"))) C2DSeqCtrl;
            m_pWMFloorCtrl->setLayer(0xE);
            m_pWMFloorCtrl->addSeq(pSeqName, 0x24);
            m_pWMFloorCtrl->setPos(vec);
            idx = CSceneInfo::getCurrentFloorNumber();
            m_pWMFloorCtrl->addSeq(pSeqName, idx > 0 ? idx + 0x3D : -idx + 0x5C);

            pRoomSrcInfo = pFloorLayout->_rooms;
            fgExpectStart = -1;
            vec.set(999.0f, 999.0f, -999.0f, -999.0f);
            for (INT i = 0; i < pFloorLayout->_nRoomCount; i++, pRoomSrcInfo++) {
                if (pRoomSrcInfo->_nPosX < 0 || pRoomSrcInfo->_nPosY < 0) {
                    continue;
                }
                if (pRoomSrcInfo->_nRoomType == 1) {
                    continue;
                }
                if (!pRoomSrcInfo->_nPosY) {
                    fgExpectStart = 0;
                }
                vec2.x = (pRoomSrcInfo->_nPosX + pRoomSrcInfo->_nPosY) * 50.0f;
                vec2.y = pRoomSrcInfo->_nPosX * 40.0f + pRoomSrcInfo->_nPosY * -40.0f;
                if (vec2.x < vec.x) {
                    vec.x = vec2.x;
                }
                if (vec2.x > vec.z) {
                    vec.z = vec2.x;
                }
                if (vec2.y < vec.y) {
                    vec.y = vec2.y;
                }
                if (vec2.y > vec.w) {
                    vec.w = vec2.y;
                }
            }
            m_vWMPosMinMax.x = (vec.z - vec.x) * -0.5f;
            m_vWMPosMinMax.z = -m_vWMPosMinMax.x;
            m_vWMPosMinMax.y = (vec.w - vec.y) * -0.5f;
            m_vWMPosMinMax.w = -m_vWMPosMinMax.y;
            vec.x = (vec.z + vec.x) * -0.5f + 24.0f;
            vec.y = (vec.w + vec.y) * -0.5f + 8.0f - 8.0f;
            vec2.x = vec2.y = 0.0f;
            vec.z = vec2.z = 0.0f;
            vec.w = vec2.w = 1.0f;
            m_vWMPosTarget.init();
            m_vWMPosCurrent.init();
            m_vWMPosCurrent0.init();
            m_fgWMMove = FALSE;

            pRoomSrcInfo = pFloorLayout->_rooms;
            for (INT i = 0; i < 32; i++) {
                m_roomDiscover[i] = -1;
                m_roomDoor[i] = 0;
                m_roomInfo[i] = 0;
            }
            for (INT i = 0; i < 48; i++) {
                m_roomMap[i] = 0;
            }
            for (INT i = 0; i < pFloorLayout->_nRoomCount; i++, pRoomSrcInfo++) {
                pRoomInfo = CSceneInfo::getRoomInfo(pRoomSrcInfo->_nID);
                if (!pRoomInfo) {
                    continue;
                }
                while (hasOpenedDoor(pRoomInfo, pRoomSrcInfo) && pRoomSrcInfo->_nRoomType != 1 && pRoomInfo->hasEntered()) {
                    idx = 0;
                    for (j = 0; j < 4; j++) {
                        DoorInfo* pDoor = &pRoomInfo->_doors[j];
                        if (pDoor->_type == 12) {
                            continue;
                        }
                        doorStat = pDoor->getDoorState();
                        if (doorStat == DOOR_STAT_CLOSED || doorStat == DOOR_STAT_OPENED) {
                            idx |= 1 << pDoor->_id;
                            if (pDoor->_nDstRoom && pDoor->_nDstRoom < 99) {
                                if (m_roomDiscover[pDoor->_nDstRoom - 1] < 0) {
                                    m_roomDiscover[pDoor->_nDstRoom - 1] = 16;
                                }
                            }
                        }
                        INT nState = 0;
                        if (!m_bPooWorld) {
                            if (doorStat == DOOR_STAT_CLOSED) {
                                nState = 1;
                            } else if (doorStat == DOOR_STAT_OPENED) {
                                if (pDoor->_type == 8) {
                                    nState = 3;
                                } else {
                                    nState = 2;
                                }
                            }
                        } else {
                            if (doorStat == DOOR_STAT_OPENED) {
                                if (pDoor->_type == 8) {
                                    nState = 3;
                                } else {
                                    nState = !CSceneInfo::getRoomInfo(pDoor->_nDstRoom)->hasEntered() ? 1 : 2;
                                }
                            }
                        }
                        m_roomDoor[i] |= nState << (pDoor->_id * 2);
                    }
                    m_roomDiscover[i] = idx;
                    break;
                }
                for (j = 0; j < 4; j++) {
                    DoorInfo* pDoor = &pRoomInfo->_doors[j];
                    if (pDoor->_type == 12) {
                        continue;
                    }
                    doorStat = pDoor->getDoorState();
                    if (pDoor->_nDstRoom && pDoor->_nDstRoom < 99) {
                        if (doorStat == DOOR_STAT_CLOSED || doorStat == DOOR_STAT_OPENED) {
                            m_roomInfo[pDoor->_nDstRoom - 1] |= pRoomSrcInfo->_nID << ((pDoor->_id & 3) * 8);
                        }
                    }
                }
                if (pRoomSrcInfo->_nRoomType != 1) {
                    idx = pRoomSrcInfo->_nPosX + pRoomSrcInfo->_nPosY * 6;
                    m_roomMap[idx] = pRoomSrcInfo->_nID;
                    if (m_nRoomCurrent == pRoomSrcInfo->_nID - 1) {
                        m_nRoomSelect = idx;
                    }
                }
            }

            nCard = 0;
            pRoomSrcInfo = pFloorLayout->_rooms;
            for (INT i = 0; i < pFloorLayout->_nRoomCount; i++, pRoomSrcInfo++) {
                INT nSeq; INT fgGuide; CHAR doorInfo;
                pRoomInfo = CSceneInfo::getRoomInfo(pRoomSrcInfo->_nID);
                p2DSeqCtrl = m_pRoomCtrl[i] = new (operator new(sizeof(C2DSeqCtrl) KH_ATARG("(6755)"))) C2DSeqCtrl;
                p2DSeqCtrl->setParent(m_pWMMoveCtrl);
                p2DSeqCtrl->setLayer(0xC);
                p2DSeqCtrl->enableColorCtrl(TRUE);
                if (pRoomSrcInfo->_nPosX < 0 || pRoomSrcInfo->_nPosY < 0) {
                    continue;
                }
                vec2.x = vec.x + (pRoomSrcInfo->_nPosX + pRoomSrcInfo->_nPosY) * 50.0f;
                vec2.y = vec.y + pRoomSrcInfo->_nPosX * 40.0f + pRoomSrcInfo->_nPosY * -40.0f;
                p2DSeqCtrl->setLocalPos(vec2);
                fgGuide = -1;
                switch (pRoomSrcInfo->_nRoomType) {
                case 0:
                case 5:
                case 7:
                case 8:
                    if (m_roomDiscover[i] < 0) {
                        nSeq = 0xE;
                    } else if (m_roomDiscover[i] < 16) {
                        nSeq = roomSeqTbl[m_roomDiscover[i]];
                    } else {
                        nSeq = 0xD;
                    }
                    break;
                case 2:
                    nSeq = 0x22;
                    fgGuide = CCard::searchCard(0xBA, 0);
                    goto chk;
                case 3:
                    nSeq = 0x20;
                    fgGuide = CCard::searchCard(0xBB, 0);
                    goto chk;
                case 4:
                    nSeq = 0x21;
                    fgGuide = CCard::searchCard(0xBC, 0);
                    goto chk;
                case 6:
                    nSeq = 0x1E;
                    fgGuide = 0x1000;
                    if (!CGameWork::progMCW()._KeyRoomsTresureBox[CSceneInfo::getCurrentWorldID(FALSE)]) {
                        if (!pRoomInfo->hasEntered()) {
                            fgGuide = CCard::searchCard(0xBD, 0) | 0x1000;
                        }
                    }
                chk:
                    if (m_roomDiscover[i] < 0 && !pRoomInfo->hasEntered()) {
                        nSeq = 0x23;
                    } else if (m_roomDiscover[i] < 16) {
                        if (pRoomSrcInfo->_nRoomType != 6) {
                            nSeq = 0x1F;
                            fgGuide = 0;
                        } else if (m_roomDiscover[i] >= 0) {
                            nSeq = roomSeqTbl[m_roomDiscover[i]];
                        } else {
                            nSeq = 0x23;
                        }
                    }
                    break;
                default:
                    continue;
                }
                p2DSeqCtrl->addSeq(pSeqName, pRoomSrcInfo->_nPosY + 0x57 + fgExpectStart);
                p2DSeqCtrl->addSeq(pSeqName, nSeq);
                if (fgGuide >= 0) {
                    USHORT effectSeq;
                    USHORT effectSeq2 = 0;
                    if (fgGuide < 0x1000) {
                        if (fgGuide <= 0 || !m_roomInfo[i]) {
                            effectSeq = 0x4E;
                        } else {
                            effectSeq = 0x4D;
                            effectSeq2 = 0x4C;
                        }
                    } else if (fgGuide == 0x1000) {
                        effectSeq = 0x4E;
                    } else {
                        effectSeq = 0x50;
                        effectSeq2 = 0x4F;
                    }
                    p2DSeqObj = p2DSeqCtrl->addSeq(pSeqName, effectSeq);
                    p2DSeqObj->changeLayer(0xD);
                    p2DSeqCtrl->enableDisp(2, FALSE);
                    if (effectSeq2) {
                        p2DSeqObj = p2DSeqCtrl->addSeq(pSeqName, effectSeq2);
                        p2DSeqObj->changeLayer(0xB);
                        p2DSeqCtrl->enableDisp(3, FALSE);
                    }
                }
                p2DSeqCtrl = m_pWayCtrl[i] = new (operator new(sizeof(C2DSeqCtrl) KH_ATARG("(6874)"))) C2DSeqCtrl(1);
                p2DSeqCtrl->setParent(m_pWMMoveCtrl);
                p2DSeqCtrl->setLayer(0xB);
                p2DSeqCtrl->enableColorCtrl(TRUE);
                p2DSeqCtrl->setLocalPos(vec2);
                p2DSeqCtrl->enableDisp(FALSE);
                doorInfo = m_roomDoor[i];
                for (INT j = 0; j < 4; j++) {
                    INT info = (doorInfo >> (j * 2)) & 3;
                    nSeq = -1;
                    if (info == 1) {
                        nSeq = 4;
                    } else if (info == 2) {
                        nSeq = 8;
                    } else if (info == 3) {
                        nSeq = 0;
                    }
                    if (nSeq >= 0) {
                        p2DSeqCtrl->addSeq(pSeqName, nSeq + ((j + 1) & 3));
                    }
                }
                cardID = pRoomInfo->getRoomNameCardID();
                if (cardID == 0xB8 || cardID == 0xB6) {
                    C2DCard* p2DCard;
                    p2DSeqCtrl = m_pShopAndSaveCtrl[nCard] = new (operator new(sizeof(C2DSeqCtrl) KH_ATARG("(6898)"))) C2DSeqCtrl;
                    p2DSeqCtrl->setParent(m_pWMMoveCtrl);
                    p2DSeqCtrl->setLocalPos(vec2);
                    p2DSeqCtrl->addSeq(pSeqName, cardID == 0xB8 ? 0x53 : 0x55);
                    p2DSeqCtrl->enableDisp(FALSE);
                    card.init(cardID, 0xFF);
                    p2DCard = m_pShopAndSave[nCard++] = new (operator new(sizeof(C2DCard) KH_ATARG("(6905)"))) C2DCard;
                    p2DCard->setParent(p2DSeqCtrl);
                    p2DCard->setLayer(0xC);
                    p2DCard->create(card);
                    p2DCard->enableDisp(FALSE);
                }
            }
            m_fRoomCurrentTimer = 0.0f;
            m_pMapCursorCtrl = new (operator new(sizeof(C2DSeqCtrl) KH_ATARG("(6914)"))) C2DSeqCtrl;
            m_pMapCursorCtrl->setLayer(0xD);
            m_pMapCursorCtrl->setParent(m_pWMMoveCtrl);
            vec = m_pRoomCtrl[m_nRoomCurrent]->m_seqControl.vPos;
            m_pMapCursorCtrl->setLocalPos(vec);
            if (vec.x < -105.0f) {
                m_vWMPosTarget.x = ((INT)(-105.0f - vec.x) / 50 + 1) * 50;
            } else if (vec.x > 105.0f) {
                m_vWMPosTarget.x = -(FLOAT)(((INT)(vec.x - 105.0f) / 50 + 1) * 50);
            }
            if (vec.y < -75.0f) {
                m_vWMPosTarget.y = ((INT)(-75.0f - vec.y) / 40 + 1) * 40;
            } else if (vec.y > 85.0f) {
                m_vWMPosTarget.y = -(FLOAT)(((INT)(vec.y - 85.0f) / 40 + 1) * 40);
            }
            m_pWMMoveCtrl->setLocalPos(m_vWMPosTarget);
            m_vWMPosOld = m_vWMPosCurrent = m_vWMPosCurrent0 = m_vWMPosTarget;
            m_pMapCursorCtrl->addSeqShift(m_pSeqName, 0x12, -12.0f, -13.0f);
            m_pMapCursorCtrl->enableDisp(FALSE);
            m_pMapGuideCtrl = new (operator new(sizeof(C2DSeqCtrl) KH_ATARG("(6934)"))) C2DSeqCtrl;
            m_pMapGuideCtrl->setLayer(0xD);
            m_pMapGuideCtrl->setParent(m_pWMMoveCtrl);
            m_pMapGuideCtrl->addSeqShift(m_pSeqName, 0xB7, 4.0f, -20.0f);
            m_pMapGuideCtrl->setLocalPos(vec);
            m_pMapGuideCtrl->enableDisp(FALSE);
            m_pCurrentRoomPosCtrl = new (operator new(sizeof(C2DSeqCtrl) KH_ATARG("(6940)"))) C2DSeqCtrl(1);
            m_pCurrentRoomPosCtrl->setLayer(0xD);
            m_pCurrentRoomPosCtrl->setParent(m_pWMMoveCtrl);
            m_pCurrentRoomPosCtrl->setLocalPos(vec);
            vec.init();
            pRoomInfo = CSceneInfo::getRoomInfo(m_roomMap[m_nRoomSelect]);
            cardID = pRoomInfo->getRoomNameCardID();
            m_pWMRoomNameCtrl = new (operator new(sizeof(C2DSeqCtrl) KH_ATARG("(6948)"))) C2DSeqCtrl;
            m_pWMRoomNameCtrl->setLayer(0xE);
            m_pWMRoomNameCtrl->addSeq(pSeqName, 0x34);
            m_pWMRoomNameCtrl->setPos(vec);
            m_pWMRoomNameFnt = new (operator new(sizeof(C2DFnt) KH_ATARG("(6953)"))) C2DFnt(0, 0, &rgba, 0xF, NULL, 0x40);
            m_pWMRoomNameCtrl->addFntShift(pSeqName, m_pWMRoomNameFnt, 0.0f, 7.0f);
            setWMRoomName(cardID, m_nRoomSelect, pRoomInfo, TRUE);
            m_pWMNormaArrowCtrl = new (operator new(sizeof(C2DSeqCtrl) KH_ATARG("(6958)"))) C2DSeqCtrl;
            m_pWMNormaArrowCtrl->setParent(m_pWMNormaCtrl);
            m_pWMNormaArrowCtrl->setLayer(0x10);
            for (INT i = 0; i < 4; i++) {
                p2DSeqCtrl = m_pRoomNormaCtrl[i] = new (operator new(sizeof(C2DSeqCtrl) KH_ATARG("(6962)"))) C2DSeqCtrl;
                p2DSeqCtrl->setParent(m_pWMNormaCtrl);
                p2DSeqCtrl->setLayer(0xD);
                p2DSeqCtrl->addSeq(pSeqName, 0x3A);
                p2DSeqCtrl->enableDisp(FALSE);
                m_pWMNormaArrowCtrl->addSeq(pSeqName, ((i + 3) & 3) + 0x36);
                m_pWMNormaArrowCtrl->enableDisp(i, FALSE);
            }
            m_bRoomNorma = FALSE;
            m_nRoomNorma = 0;
        }
        m_fWMTimer = 0.0f;
        break;
    case 1:
        if (!m_pWMCardCtrl->isEnd(0)) {
            break;
        }
        if (m_pWMExplaneCtrl) {
            m_pWMExplaneCtrl->replaceSeq(0, m_pSeqName, 0xB9);
        }
        m_pWMCardCtrl->replaceSeq(0, pSeqName, 0x28);
        m_pWMFloorCtrl->replaceSeq(0, pSeqName, 0x25);
        m_pWMRoomNameCtrl->replaceSeq(0, pSeqName, ++m_nWMRoomNameSeq);
        m_pCurrentRoomPosCtrl->addSeq(pSeqName, !m_bPlayerKind ? 0x51 : 0x6A);
        m_pMapCursorCtrl->enableDisp(TRUE);
        m_pMapGuideCtrl->enableDisp(checkNorma(-1, roomId, -1) != 0);
        for (INT i = 0; i < 32; i++) {
            if (m_pWayCtrl[i]) {
                m_pWayCtrl[i]->enableDisp(TRUE);
            }
            if (m_pRoomCtrl[i]) {
                m_pRoomCtrl[i]->enableDisp(2, TRUE);
                m_pRoomCtrl[i]->enableDisp(3, TRUE);
            }
            if (m_pShopAndSaveCtrl[i]) {
                m_pShopAndSaveCtrl[i]->enableDisp(TRUE);
                m_pShopAndSave[i]->enableDisp(TRUE);
            }
        }
        ret = TRUE;
        break;
    case 3:
        m_fRoomCurrentTimer += getFrameRateF();
        if (m_fRoomCurrentTimer >= 165.0f) {
            m_fRoomCurrentTimer = 0.0f;
        }
        fCol = m_fRoomCurrentTimer < 82.5f ? m_fRoomCurrentTimer : 165.0f - m_fRoomCurrentTimer;
        fCol /= 82.5f;
        vec.z = fCol * 0.85f + 0.9f;
        vec.y = fCol * 0.47f + 0.7f;
        vec.x = 0.5f - fCol * 0.2f;
        vec.w = 1.0f;
        m_pRoomCtrl[m_nRoomCurrent]->setCol(vec);
        for (INT i = 0; i < 32; i++) {
            if (m_pShopAndSave[i]) {
                m_pShopAndSave[i]->checkBack();
            }
        }
        break;
    case 4:
        if (m_pWMExplaneCtrl) {
            m_pWMExplaneCtrl->replaceSeq(0, m_pSeqName, 0xBA);
        }
        m_pWMCardCtrl->replaceSeq(0, pSeqName, 0x29);
        m_pWMFloorCtrl->replaceSeq(0, pSeqName, 0x26);
        m_pWMRoomNameCtrl->replaceSeq(0, pSeqName, ++m_nWMRoomNameSeq);
        m_pCurrentRoomPosCtrl->replaceSeq(0, pSeqName, !m_bPlayerKind ? 0x52 : 0x6B);
        for (INT i = 0; i < 32; i++) {
            if (m_pShopAndSaveCtrl[i]) {
                m_pShopAndSaveCtrl[i]->replaceSeq(0, pSeqName, 0x54);
            }
        }
        SAFE_DELETE(m_pMapCursorCtrl);
        SAFE_DELETE(m_pMapGuideCtrl);
        for (INT i = 0; i < 4; i++) {
            SAFE_DELETE(m_pRoomNormaCtrl[i]);
        }
        for (INT i = 0; i < 4; i++) {
            SAFE_DELETE(m_pRoomNorma[i]);
        }
        SAFE_DELETE(m_pWMNormaArrowCtrl);
        setRoomHelp(0, 0);
        m_fWMTimer = 5.0f;
        break;
    case 5:
        if (m_pWMMoveCtrl) {
            vec2.set(1.0f, 1.0f, 1.0f, 1.0f);
            m_fWMTimer -= getFrameRateF();
            if (m_fWMTimer < 0.0f) {
                m_fWMTimer = 0.0f;
            }
            vec2.w = m_fWMTimer / 5.0f;
            m_pWMMoveCtrl->setCol(vec2);
            ret = m_pWMCardCtrl->isEnd(0) != FALSE;
        } else {
            ret = TRUE;
        }
        break;
    case 10:
        setWorldmapCursor();
        ret = TRUE;
        break;
    case 8:
        SAFE_DELETE(m_pWMNormaCtrl);
        for (INT i = 0; i < 32; i++) {
            SAFE_DELETE(m_pRoomCtrl[i]);
            SAFE_DELETE(m_pWayCtrl[i]);
            SAFE_DELETE(m_pShopAndSaveCtrl[i]);
            SAFE_DELETE(m_pShopAndSave[i]);
        }
        SAFE_DELETE(m_pMapCursorCtrl);
        SAFE_DELETE(m_pMapGuideCtrl);
        for (INT i = 0; i < 4; i++) {
            SAFE_DELETE(m_pRoomNormaCtrl[i]);
        }
        for (INT i = 0; i < 4; i++) {
            SAFE_DELETE(m_pRoomNorma[i]);
        }
        SAFE_DELETE(m_pWMCardCtrl);
        SAFE_DELETE(m_pWMFloorCtrl);
        SAFE_DELETE(m_pWMRoomNameCtrl);
        SAFE_DELETE(m_pWMMoveCtrl);
        SAFE_DELETE(m_pWMNormaArrowCtrl);
        SAFE_DELETE(m_pWMExplaneCtrl);
        SAFE_DELETE(m_pCurrentRoomPosCtrl);
        setRoomHelp(0, 0);
        break;
    }
    return ret;
}
void CCamp::setWorldmapCursor() {
    RoomInfo* pRoomInfo;
    DoorInfo* pDoorInfo;
    INT idx;
    INT idx2;
    static LPCTSTR pSeqName = "worldmap:00";
    static char* pStrButton[2] = { "Zoom In{     \xF9" "DShow Criteria", "Zoom Out     \xF9" "DShow Criteria" };
    FVECTOR vec;
    FVECTOR vec2;
    C2DSeqCtrl* p2DSeqCtrl;
    RCNORMA::NormaSrc normaSrc;
    CARD card;
    USHORT roomId;
    INT iMoveDirect;
    vec.init();
    idx = -1;
    if (m_nSubStep < 4) if (m_nSubStep >= 0) {
        if (CPadHandler::isButtonRpt(0x101000, 0)) {
            iMoveDirect = 0;
            idx = searchRoom(0, m_nSubStep & 1);
        } else if (CPadHandler::isButtonRpt(0x204000, 0)) {
            iMoveDirect = 1;
            idx = searchRoom(1, m_nSubStep & 1);
        } else if (CPadHandler::isButtonRpt(0x408000, 0)) {
            iMoveDirect = 2;
            idx = searchRoom(2, m_nSubStep & 1);
        } else if (CPadHandler::isButtonRpt(0x802000, 0)) {
            iMoveDirect = 3;
            idx = searchRoom(3, m_nSubStep & 1);
        }
        if (idx >= 0) {
            m_nRoomSelect = idx;
            idx2 = m_roomMap[idx] - 1;
            vec = m_pRoomCtrl[idx2]->m_seqControl.vPos;
            m_pMapCursorCtrl->setLocalPos(vec);
            m_pMapGuideCtrl->setLocalPos(vec);
            if (checkNorma(-1, roomId, -1) && !(m_nSubStep & 1)) {
                m_pMapGuideCtrl->enableDisp(1);
            } else {
                m_pMapGuideCtrl->enableDisp(0);
                setRoomHelp(0, 0);
            }
            pRoomInfo = CSceneInfo::getRoomInfo(idx2 + 1);
            setWMRoomName(pRoomInfo->getRoomNameCardID(), m_nRoomSelect, pRoomInfo, 0);
            if (m_pRoomCtrl[idx2]->getNumSeq() >= 4) {
                m_pRoomCtrl[idx2]->restart(1, 0.0f);
                m_pRoomCtrl[idx2]->restart(2, 0.0f);
            }
            m_vWMPosOld = m_vWMPosTarget;
            AddVectorXYZ(vec2, vec, m_vWMPosTarget);
            if (vec2.y < -85.0f) {
                m_vWMPosTarget.y += (INT)((-85.0f - vec2.y + 40.0f) / 40.0f) * 40.0f;
            } else if (vec2.y > 85.0f) {
                m_vWMPosTarget.y += (INT)((vec2.y - 85.0f + 40.0f) / 40.0f) * -40.0f;
            } else if (vec2.y < -5.0f && !(iMoveDirect & 1) && vec.y - m_vWMPosMinMax.y > 45.0f) {
                m_vWMPosTarget.y += 40.0f;
            } else if (vec2.y > 5.0f && (iMoveDirect & 1) && m_vWMPosMinMax.w - vec.y > 45.0f) {
                m_vWMPosTarget.y += -40.0f;
            }
            if (vec2.x < -105.0f) {
                m_vWMPosTarget.x += (INT)((-105.0f - vec2.x + 50.0f) / 50.0f) * 50.0f;
            } else if (vec2.x > 105.0f) {
                m_vWMPosTarget.x += (INT)((vec2.x - 105.0f + 50.0f) / 50.0f) * -50.0f;
            }
            if (m_nSubStep < 2) {
                m_fgWMMove = TRUE; m_fWMTimer = 0.0f;
            } else {
                m_vWMPosOld = m_vWMPosTarget;
            }
            CSound::playSE(1, -1, 0x40);
        }
    }
    while (CPadHandler::isButtonTrg(0x40, 0)) {
        if (m_bPooWorld) break;
        if (!(m_nSubStep & 1)) {
            if (!checkNorma(-1, roomId, -1)) break;
            m_pMapGuideCtrl->enableDisp(0);
            idx = 0;
            m_bRoomNorma = TRUE;
            CSound::playSE(2, -1, 0x40);
            m_nSubStep |= 1;
        } else {
            m_pWMNormaCtrl->enableDisp(0);
            for (INT i = 0; i < 4; i++) {
                m_pRoomNormaCtrl[i]->enableDisp(0);
                m_pWMNormaArrowCtrl->enableDisp(i, 0);
            }
            for (INT i = 0; i < 4; i++) {
                SAFE_DELETE(m_pRoomNorma[i]);
            }
            vec.set(1.0f, 1.0f, 1.0f, 1.0f);
            for (INT i = 0; i < 32; i++) {
                if (m_pRoomCtrl[i]) {
                    m_pRoomCtrl[i]->setCol(vec);
                }
                if (m_pWayCtrl[i]) {
                    m_pWayCtrl[i]->setCol(vec);
                }
            }
            m_pMapGuideCtrl->enableDisp(checkNorma(-1, roomId, -1) != 0);
            m_nRoomNorma = 0;
            m_bRoomNorma = FALSE;
            CSound::playSE(4, -1, 0x40);
            m_nSubStep &= ~1;
        }
        break;
    }
    if (!m_bPooWorld) {
        while ((m_nSubStep & 1) && idx >= 0) {
            INT id;
            m_pWMNormaCtrl->enableDisp(0);
            for (INT i = 0; i < 4; i++) {
                m_pRoomNormaCtrl[i]->enableDisp(0);
                m_pWMNormaArrowCtrl->enableDisp(i, 0);
            }
            for (INT i = 0; i < 4; i++) {
                SAFE_DELETE(m_pRoomNorma[i]);
            }
            vec.set(1.0f, 1.0f, 1.0f, 1.0f);
            for (INT i = 0; i < 32; i++) {
                if (m_pRoomCtrl[i]) {
                    m_pRoomCtrl[i]->setCol(vec);
                }
                if (m_pWayCtrl[i]) {
                    m_pWayCtrl[i]->setCol(vec);
                }
            }
            id = m_roomMap[m_nRoomSelect] - 1;
            if (!checkNorma(-1, roomId, -1)) break;
            m_nRoomNorma = 0;
            m_pWMNormaCtrl->setLocalPos(m_pRoomCtrl[id]->m_seqControl.vPos);
            m_pWMNormaCtrl->enableDisp(1);
            for (INT i = 0; i < 4; i++) {
                INT nNormaIndex;
                INT nNorma;
                INT fgKeyCard;
                if (!checkNorma(i, roomId, -1)) continue;
                pRoomInfo = CSceneInfo::getRoomInfo(roomId);
                if (!pRoomInfo) continue;
                for (INT j = 0; j < 4; j++) {
                    pDoorInfo = &pRoomInfo->_doors[j];
                    if (pDoorInfo->_id == i) break;
                }
                nNormaIndex = pDoorInfo->getNextNormaIndex();
                nNorma = pDoorInfo->getNormaCount() - nNormaIndex - 1;
                if (nNorma < 0) continue;
                p2DSeqCtrl = m_pRoomNormaCtrl[i];
                p2DSeqCtrl->enableDisp(1);
                p2DSeqCtrl->setLayer(0x10);
                p2DSeqCtrl->replaceSeq(0, pSeqName, nNorma + 0x3A);
                if (nNorma == 0) {
                    vec.set(i < 2 ? -71.0f : 71.0f, ((i + 1) & 3) < 2 ? 83.0f : -67.0f, 0.0f);
                } else {
                    vec.set(nNorma * -26.0f, ((i + 1) & 3) < 2 ? 95.0f : -81.0f, 0.0f);
                }
                p2DSeqCtrl->setLocalPos(vec);
                m_pWMNormaArrowCtrl->enableDisp(i, 1);
                fgKeyCard = pDoorInfo->getNormaKeyCardID();
                for (INT k = nNorma; k >= 0; k--) {
                    ASSERT_AT(7351, m_nRoomNorma < 4);
                    C2DCard* p2DCard = m_pRoomNorma[m_nRoomNorma++] = new KH_AT("(7352)") C2DCard(1);
                    p2DCard->setParent(p2DSeqCtrl);
                    p2DCard->setLayer(0x10);
                    vec.set(k * 82.0f, 0.0f, 0.0f);
                    p2DCard->setLocalPos(vec);
                    if (k == nNorma && fgKeyCard) {
                        card.init(fgKeyCard, 0);
                        p2DCard->create(card);
                    } else {
                        INT nNI = k + nNormaIndex;
                        normaSrc._nColor = pDoorInfo->getNormaColor(nNI);
                        normaSrc._nNumber = pDoorInfo->getNormaNumber(nNI);
                        normaSrc._nOp = pDoorInfo->getNormaOperation(nNI);
                        { /* MATCH: _NORMA_PIECES (2DCard.h) without its register pins, i.e. the RTL size of the original's by-value argument: gcse's table size orders the hoisted this+0x354/0x4D8 spill slots in retail */
                            RCNORMA::NormaSrc t = normaSrc;
                            _createNormaRaw(p2DCard, ({ ULONGLONG lo = (USHORT)t._nOp; ULONGLONG num = (USHORT)t._nNumber; ULONGLONG hi = (USHORT)t._nColor; lo <<= 16; ULONGLONG top = (USHORT)t.__nDummy; lo |= num; hi <<= 32; hi |= lo; top <<= 48; top | hi; }), 0);
                        }
                    }
                }
            }
            vec.set(0.5f, 0.5f, 0.5f, 1.0f);
            for (INT i = 0; i < 32; i++) {
                if (i != id) {
                    if (m_pRoomCtrl[i]) {
                        m_pRoomCtrl[i]->setCol(vec);
                    }
                }
                if (m_pWayCtrl[i]) {
                    m_pWayCtrl[i]->setCol(vec);
                }
            }
            break;
        }
        if (CPadHandler::isButtonTrg(0x10, 0)) {
            if (!(m_nSubStep & 2)) {
                m_pWMExplaneFnt->setText(pStrButton[0]);
                m_pWMMoveCtrl->setLocalPos(vec);
                m_vWMPosOld = m_vWMPosTarget;
                m_vWMPosTarget.set(0.0f, -3.0f, 0.0f, 0.585f);
                CSound::playSE(2, -1, 0x40);
                m_nSubStep |= 2;
            } else {
                m_pWMExplaneFnt->setText(pStrButton[1]);
                m_pWMMoveCtrl->setLocalPos(m_vWMPosCurrent);
                m_vWMPosTarget = m_vWMPosOld;
                m_vWMPosTarget.w = 1.0f;
                m_vWMPosOld.set(0.0f, 0.0f, 0.0f, 0.585f);
                CSound::playSE(4, -1, 0x40);
                m_nSubStep &= ~2;
            }
            m_fgWMMove = TRUE;
            m_fWMTimer = 0.0f;
        }
    }
    moveWM();
}
INT CCamp::setMainCmd(INT nSetCmd, INT nMode) {
    FVECTOR vec;
    FVECTOR vec2;
    C2DSeqCtrl* p2DSeqCtrl;
    G_RGBA rgba;

    if (nSetCmd == 0) {
        rgba.col = 0x80404040;
        if (!m_pMainCmdCtrl[0]) {
            vec.set(86.0f, 88.0f, 0.0f, 1.0f);



            for (INT i = 0; i < m_nCurrentCmdMax; i++) {
                p2DSeqCtrl = m_pMainCmdCtrl[i] = new KH_AT("(7430)") C2DSeqCtrl;
                p2DSeqCtrl->setLayer(0xC);
                p2DSeqCtrl->enableColorCtrl(TRUE);
                p2DSeqCtrl->addSeq(m_pSeqName, 4);
                p2DSeqCtrl->setPos(vec);
                m_pMainCmdFnt[i] = new KH_AT("(7435)") C2DFnt(0, 0, &rgba, 0xF, m_pMainCmdName[m_bPlayerKind][0][i], 0x40);
                p2DSeqCtrl->addFntShift(m_pSeqName, m_pMainCmdFnt[i], 0.0f, 6.0f);
                vec.y += 26.0f;
            }
        } else {
            if (nMode != m_nMainMode) {
                m_nCurrentCmdMax = nMode ? (!m_bPlayerKind ? 6 : 2) : 7;
                for (INT i = 0; i < m_nCurrentCmdMax; i++) {
                    p2DSeqCtrl = m_pMainCmdCtrl[i];
                    delete m_pMainCmdFnt[i];
                    KH_DEBUG(asm volatile("");) /* MATCH: beta only: one more insn in the loop body: loop.c's second pass then leaves this + 336 in the loop, so &m_pMainCmdFnt[i] is recomputed from i each iteration (only m_pMainCmdCtrl[i] strength-reduced); retail matches without it (p2DSeqCtrl's allocno priority 4*22/80 then beats this's 5*45/206: this in s1) */
                    m_pMainCmdFnt[i] = NULL;
                    m_pMainCmdFnt[i] = new KH_AT("(7448)") C2DFnt(0, 0, &rgba, 0xF, m_pMainCmdName[m_bPlayerKind][nMode][i], 0x40);
                    p2DSeqCtrl->replaceFnt(1, m_pSeqName, m_pMainCmdFnt[i]);
                }
                m_nMainMode = nMode;
            }
            for (INT i = 0; i < m_nCurrentCmdMax; i++) {
                m_pMainCmdCtrl[i]->replaceSeq(0, m_pSeqName, 4);
            }
        }
    } else if (nSetCmd == 1) {
        for (INT i = 0; i < m_nCurrentCmdMax; i++) {
            m_pMainCmdCtrl[i]->replaceSeq(0, m_pSeqName, 5);
        }
    } else if (nSetCmd == 2) {
        for (INT i = 0; i < m_nCurrentCmdMax; i++) {
            m_pMainCmdCtrl[i]->replaceSeq(0, m_pSeqName, 6);
        }
    }
    return 1;
}

INT CCamp::setMainCmdHist(INT nSetCmd, INT nMode) {
    FVECTOR vec;
    FVECTOR vec2;
    C2DSeqCtrl* p2DSeqCtrl;
    G_RGBA rgba;

    switch (nSetCmd) {
    case 1:
        rgba.col = 0x807C7C7C;
        if (!m_pMainCmdHistCtrl[nMode]) {
            vec.set(78.0f, 51.0f, 0.0f, 1.0f);



            for (INT i = 0; i < 2; i++) {
                p2DSeqCtrl = m_pMainCmdHistCtrl[i] = new KH_AT("(7499)") C2DSeqCtrl;
                p2DSeqCtrl->setLayer(0xF);
                p2DSeqCtrl->enableColorCtrl(TRUE);
                p2DSeqCtrl->setPos(vec);
                p2DSeqCtrl->addSeq(m_pSeqName, 8);
                m_pMainCmdHistFnt[i] = new KH_AT("(7504)") C2DFnt(0, 0, &rgba, 0xF, NULL, 0x42);
                p2DSeqCtrl->addFntShift(m_pSeqName, m_pMainCmdHistFnt[i], 0.0f, 6.0f);
                p2DSeqCtrl->enableDisp(FALSE);
                vec.x += 118.0f;
            }
        } else {
            p2DSeqCtrl = m_pMainCmdHistCtrl[nMode];
            p2DSeqCtrl->replaceSeq(0, m_pSeqName, 7);
            delete m_pMainCmdHistFnt[nMode];
            m_pMainCmdHistFnt[nMode] = NULL;
            m_pMainCmdHistFnt[nMode] = new KH_AT("(7515)") C2DFnt(0, 0, &rgba, 0xF, m_pMainCmdName[m_bPlayerKind][nMode][m_nMainCmdSel], 0x40);
            p2DSeqCtrl->replaceFnt(1, m_pSeqName, m_pMainCmdHistFnt[nMode]);
            p2DSeqCtrl->enableDisp(TRUE);
        }
        m_nMainCmdHistRun[nMode] = 2;
        break;
    case 2:
        if (!m_pMainCmdHistCtrl[nMode]->isEnd(0)) {
            break;
        }
        if (!m_pMCHistLightCtrl[nMode]) {
            m_pMCHistLightCtrl[nMode] = new KH_AT("(7532)") C2DSeqCtrl;
            p2DSeqCtrl = m_pMCHistLightCtrl[nMode];
            p2DSeqCtrl->setLayer(0xF);
            vec2 = m_pMainCmdHistCtrl[nMode]->m_seqControl.vPos;
            vec2.x += 48.0f;
            vec2.y += -4.0f;
            p2DSeqCtrl->setLocalPos(vec2);
            p2DSeqCtrl->addSeq(m_pSeqName, 0x14);
        }
        m_nMainCmdHistRun[nMode] = 999;
    case 3:
        m_pMainCmdHistCtrl[nMode]->replaceSeq(0, m_pSeqName, 8);
        break;
    case 4:
        m_pMainCmdHistCtrl[nMode]->replaceSeq(0, m_pSeqName, 9);
        m_pMainCmdHistCtrl[nMode]->replaceFnt(1, m_pSeqName, m_pMainCmdHistFnt[nMode]);
        if (m_pMCHistLightCtrl[nMode]) {
            m_pMCHistLightCtrl[nMode]->replaceSeq(0, m_pSeqName, 0x15);
        }
        m_nMainCmdHistRun[nMode] = 5;
        break;
    case 5:
        if (!m_pMainCmdHistCtrl[nMode]->isEnd(0)) {
            break;
        }
        delete m_pMCHistLightCtrl[nMode];
        m_pMCHistLightCtrl[nMode] = NULL;
        m_nMainCmdHistRun[nMode] = 999;
    case 6:
        m_pMainCmdHistCtrl[nMode]->enableDisp(FALSE);
        break;
    case 7:
        m_pMainCmdHistCtrl[nMode]->enableDisp(TRUE);
        break;
    }
    return 1;
}

INT CCamp::setStatusWin0(INT nSetCmd) {
    static char* pStrEtc[6] = { "Sora", "Riku", "LV", "%d", "HP", "%d/%d" };
    FVECTOR vec;
    FVECTOR vec2;
    C2DFnt* p2DFnt;
    G_RGBA rgba;
    char chrTmp[64];

    if (nSetCmd == 0) {
        if (!m_pWindow0Ctrl) {
            m_pWindow0Ctrl = new KH_AT("(7600)") C2DSeqCtrl;
            m_pWindow0Ctrl->setLayer(0xF);
            m_pWindow0Ctrl->enableColorCtrl(TRUE);
            m_pWindow0Ctrl->addSeq(m_pSeqName, 0xA);
            C2DMan::getLayPosition(m_pLadName, 0, 7, vec, 0);
            if (m_bPlayerKind) {
                vec.x += 30.0f;
            }
            m_pWindow0Ctrl->setPos(vec);
            rgba.col = 0x807C7C7C;
            m_pWindow0Fnt[0] = new KH_AT("(7610)") C2DFnt(0, 0, &rgba, 0xE, pStrEtc[m_bPlayerKind], 0x42);
            m_pWindow0Ctrl->addFntShift(m_pSeqName, m_pWindow0Fnt[0], 0.0f, -17.0f);
            rgba.col = 0x80007878;
            m_pWindow0Fnt[1] = new KH_AT("(7615)") C2DFnt(0, 0, &rgba, 0xD, pStrEtc[2], 0x40);
            m_pWindow0Ctrl->addFntShift(m_pSeqName, m_pWindow0Fnt[1], -41.0f, 5.0f);
            m_pWindow0Fnt[2] = new KH_AT("(7619)") C2DFnt(0, 0, &rgba, 0xD, NULL, 0x41);
            p2DFnt = m_pWindow0Fnt[2];
            sprintf(chrTmp, pStrEtc[3], CStatus::getLV());
            p2DFnt->allocText(chrTmp);
            m_pWindow0Ctrl->addFntShift(m_pSeqName, p2DFnt, 43.0f, 5.0f);
            rgba.col = 0x80007800;
            m_pWindow0Fnt[3] = new KH_AT("(7626)") C2DFnt(0, 0, &rgba, 0xD, pStrEtc[4], 0x40);
            m_pWindow0Ctrl->addFntShift(m_pSeqName, m_pWindow0Fnt[3], -41.0f, 21.0f);
            m_pWindow0Fnt[4] = new KH_AT("(7630)") C2DFnt(0, 0, &rgba, 0xD, NULL, 0x41);
            p2DFnt = m_pWindow0Fnt[4];
            sprintf(chrTmp, pStrEtc[5], CStatus::getHP(), CStatus::getHPMax());
            p2DFnt->allocText(chrTmp);
            m_pWindow0Ctrl->addFntShift(m_pSeqName, p2DFnt, 43.0f, 21.0f);
        } else {
            m_pWindow0Ctrl->replaceSeq(0, m_pSeqName, 0xA);
        }
    } else if (nSetCmd == 1) {
        m_pWindow0Ctrl->replaceSeq(0, m_pSeqName, 0xB);
    } else if (nSetCmd == 2) {
        m_pWindow0Ctrl->replaceSeq(0, m_pSeqName, 0xC);
    } else {
        delete m_pWindow0Ctrl;
        m_pWindow0Ctrl = NULL;
    }
    return 1;
}

INT CCamp::setStatusWin1(INT nSetCmd) {
    static LPCTSTR pStrFriend = "FRIENDS";
    static USHORT baseSeqTbl[2][3] = { { 13, 14, 15 }, { 349, 350, 351 } };
    static USHORT nameSeqTbl[3] = { 311, 312, 313 };
    static USHORT deckNameSeqTbl[3] = { 308, 309, 310 };
    FVECTOR vec;
    FVECTOR vec2;
    CButton* pButton;
    G_RGBA rgba;
    CARD card;
    PLM::CPlayerManager* pPlayerManager;

    if (nSetCmd == 0) {
        if (!m_pWindow1Ctrl) {
            m_pWindow1Ctrl = new KH_AT("(7698)") C2DSeqCtrl;
            m_pWindow1Ctrl->setLayer(0xC);
            m_pWindow1Ctrl->enableColorCtrl(TRUE);
            m_pWindow1Ctrl->addSeq(m_pSeqName, baseSeqTbl[m_bPlayerKind][0]);
            C2DMan::getLayPosition(m_pLadName, 0, 8, vec, 0);
            m_pWindow1Ctrl->setPos(vec);
            rgba.col = 0x807C7C7C;
            m_pWindow1Fnt = new KH_AT("(7706)") C2DFnt(0, 0, &rgba, 0xF, pStrFriend, 0x40);
            m_pWindow1Ctrl->addFntShift(m_pSeqName, m_pWindow1Fnt, !m_bPlayerKind ? -83.0f : -32.0f, -44.0f);
            vec.x = !m_bPlayerKind ? -72.0f : 2.0f;
            register FLOAT fY asm("$f0") = 26.0f; /* MATCH: 26 in f0, 1 in f1 */
            pPlayerManager = PLM::CPlayerManager::getPLM();
            FVECTOR* pVec = &vec; /* the ref's inline "FVECTOR *this": the original calls vec.set() (Coord.h:730); set() here changes the code */
            pVec->y = fY;
            pVec->w = 1.0f;
            pVec->z = 0.0f;
            INT idx = 0;
            if (pPlayerManager) {
                FLOAT fShift = pVec->z;
                for (INT i = 0; i < 3; i++) {
                    INT kind = pPlayerManager->getAppearFriendCard(i, 1);
                    if (kind) {
                        C2DCard* p2DCard = m_pWindow1Card[idx] = new KH_AT("(7721)") C2DCard;
                        p2DCard->setParent(m_pWindow1Ctrl);
                        p2DCard->setLayer(0xC);
                        p2DCard->enableColorCtrl(TRUE);
                        p2DCard->setLocalPos(vec);
                        vec.x += 74.0f;
                        card.init(kind, 0xFF);
                        m_pWindow1Card[idx]->create(card);
                        m_pWin1EtcButton[idx] = new KH_AT("(7731)") CButton;
                        pButton = m_pWin1EtcButton[idx];
                        INT nLay = m_bPlayerKind + 0xF;
                        C2DMan::getLayPosition(m_pLadName, 0, idx + nLay, vec2, 1);
                        idx++;
                        pButton->init(m_pSeqName, 0x14, nameSeqTbl, vec2, NULL, card.getName(), 0xC, fShift, fShift, 0x42, NULL, -1);
                    }
                }
                if (!m_bPlayerKind) {
                    m_pWin1EtcButton[3] = new KH_AT("(7745)") CButton;
                    pButton = m_pWin1EtcButton[3];
                    C2DMan::getLayPosition(m_pLadName, 0, 0x12, vec2, 1);
                    pButton->init(m_pSeqName, 0x14, deckNameSeqTbl, vec2, NULL,
                                  CCard::getDeckName(CCard::getCurrentDeckNo()), 0xE, -3.0f, 2.0f, 0x40,
                                  NULL, -1);
                }
            }
        } else {
            m_pWindow1Ctrl->replaceSeq(0, m_pSeqName, baseSeqTbl[m_bPlayerKind][0]);
            for (INT i = 0; i < 4; i++) {
                if (m_pWin1EtcButton[i]) {
                    m_pWin1EtcButton[i]->setSequence(CButton::BS_IN, FALSE);
                }
            }
        }
    } else if (nSetCmd == 1) {
        m_pWindow1Ctrl->replaceSeq(0, m_pSeqName, baseSeqTbl[m_bPlayerKind][1]);
        for (INT i = 0; i < 4; i++) {
            if (m_pWin1EtcButton[i]) {
                m_pWin1EtcButton[i]->setSequence(CButton::BS_LOOP, FALSE);
            }
        }
    } else if (nSetCmd == 2) {
        m_pWindow1Ctrl->replaceSeq(0, m_pSeqName, baseSeqTbl[m_bPlayerKind][2]);
        for (INT i = 0; i < 4; i++) {
            if (m_pWin1EtcButton[i]) {
                m_pWin1EtcButton[i]->setSequence(CButton::BS_OUT, FALSE);
            }
        }
    } else {
        delete m_pWindow1Ctrl;
        m_pWindow1Ctrl = NULL;
        for (INT i = 0; i < 4; i++) {
            delete m_pWin1EtcButton[i];
            m_pWin1EtcButton[i] = NULL;
        }
        for (INT i = 0; i < 3; i++) {
            delete m_pWindow1Card[i];
            m_pWindow1Card[i] = NULL;
        }
        delete m_pWin1EtcButton[3];
        m_pWin1EtcButton[3] = NULL;
    }
    return 1;
}
void CCamp::setCursor(INT nSelect, INT nMode) {
    FVECTOR vec;

    if (nSelect >= 0) {
        vec.set(48.0f, nSelect * 26.0f + 80.0f, 0.0f, 1.0f);



        m_pCursorCtrl->setPos(vec);
        m_pCursorCtrl->enableDisp(TRUE);
    } else {
        m_pCursorCtrl->enableDisp(FALSE);
    }
}

void CCamp::setLight(INT nSelect, INT nMode) {
    FVECTOR vec;

    if (nSelect >= 0) {
        vec.set(150.0f, nSelect * 26.0f + 82.0f, 0.0f, 1.0f);



        m_vPosLightTarget = vec;
        if (nSelect == m_nMainCmdSel) {
            m_vPosLightCurrent = vec;
        }
        m_vPosLightOld = m_vPosLightCurrent;
        m_fgLightMove = 1;
        m_fLightTimer = 0.0f;
        m_pLightCtrl->enableDisp(TRUE);
    } else {
        m_pLightCtrl->enableDisp(FALSE);
    }
}

void CCamp::setCursor2(INT nSelect, INT nMode, BOOL bMapCard) {
    FVECTOR vec;
    INT& nKindSelectTop = !bMapCard ? m_nKindSelectTop : m_nMCSelSelectTop; // MATCH: ref local; the !bMapCard polarity keeps the movn target order

    if (nMode < 2) {
        nSelect -= nKindSelectTop;
        if (nSelect >= 9) {
            nKindSelectTop += 3;
            nSelect -= 3;
        } else if (nSelect < 0) {
            nKindSelectTop -= 3;
            nSelect += 3;
        }
        C2DCard** ppCard = !bMapCard ? m_pDeckCardCard : m_pMCCard;
        vec = ppCard[nSelect]->m_seqControl.vPos;
    } else if (nMode >= 5) {
        FLOAT x = nSelect ? nSelect * 24.0f + -30.0f : -56.0f;
        vec.set(x, -70.0f, 0.0f, 1.0f);



    }
    switch (nMode) {
    case 0:
        if (!m_pCursor2Ctrl) {
            m_pCursor2Ctrl = new KH_AT("(7862)") C2DSeqCtrl(1);
            m_pCursor2Ctrl->setLayer(0x12);
            m_pCursor2Ctrl->setParent(!bMapCard ? m_pDeckCardCtrl[0] : m_pMCSelWinCtrl[0]);
            m_pCursor2Ctrl->addSeq(m_pSeqName, 0x9F);
            m_pCursor2Ctrl->addSeqShift(m_pSeqName, 0x12, -8.0f, -30.0f);
        } else {
            m_pCursor2Ctrl->setParent(!bMapCard ? m_pDeckCardCtrl[0] : m_pMCSelWinCtrl[0]);
        }
    set:
        m_vPosCursor2Current = vec;
    case 1:
    case 6:
        m_vPosCursor2Target = vec;
        m_vPosCursor2Old = m_vPosCursor2Current;
        m_fCursor2Timer = 0.0f;
        m_fgCursor2Move = 1;
        m_pCursor2Ctrl->enableDisp(TRUE);
        break;
    case 2:
        delete m_pCursor2Ctrl;
        m_pCursor2Ctrl = NULL;
        break;
    case 3:
        if (m_pCursor2Ctrl) {
            m_pCursor2Ctrl->enableDisp(0, FALSE);
            m_pCursor2Ctrl->replaceSeqShift(1, m_pSeqName, 0x11, -8.0f, -30.0f);
        }
        break;
    case 4:
        if (m_pCursor2Ctrl) {
            m_pCursor2Ctrl->enableDisp(0, TRUE);
            m_pCursor2Ctrl->replaceSeqShift(1, m_pSeqName, 0x12, -8.0f, -30.0f);
        }
        break;
    case 5:
        m_pCursor2Ctrl->enableDisp(0, FALSE);
        goto set;
    case 7:
        m_pCursor2Ctrl->enableDisp(0, TRUE);
        break;
    }
}

void CCamp::setCursor3(INT nSelect, INT nMode, BOOL bMapCard) {
    FVECTOR vec;
    C2DSeqCtrl** ppNumCtrl = !bMapCard ? m_pDeckInfoNumCtrl : m_pMCInfoNumCtrl;

    vec = ppNumCtrl[nSelect]->m_seqControl.vPos;
    switch (nMode) {
    case 0:
        if (!m_pCursor3Ctrl) {
            m_pCursor3Ctrl = new KH_AT("(7935)") C2DSeqCtrl;
            m_pCursor3Ctrl->setLayer(0x12);
            m_pCursor3Ctrl->setParent(!bMapCard ? m_pDeckInfoCtrl : m_pMCInfoWinCtrl);
            m_pCursor3Ctrl->addSeqShift(m_pSeqName, 0xA0, 18.0f, 0.0f);
            m_pCursor3Ctrl->addSeqShift(m_pSeqName, 0x12, -18.0f, -6.0f);
        }
        m_vPosCursor3Current = vec;
    case 1:
        m_vPosCursor3Target = vec;
        m_vPosCursor3Old = m_vPosCursor3Current;
        m_fCursor3Timer = 0.0f;
        m_fgCursor3Move = 1;
        m_pCursor3Ctrl->enableDisp(TRUE);
        break;
    case 2:
        delete m_pCursor3Ctrl;
        m_pCursor3Ctrl = NULL;
        break;
    case 3:
        if (m_pCursor3Ctrl) {
            m_pCursor3Ctrl->enableDisp(0, FALSE);
            m_pCursor3Ctrl->replaceSeqShift(1, m_pSeqName, 0x11, -18.0f, -4.0f);
        }
        break;
    case 4:
        if (m_pCursor3Ctrl) {
            m_pCursor3Ctrl->enableDisp(0, TRUE);
            m_pCursor3Ctrl->replaceSeqShift(1, m_pSeqName, 0x12, -18.0f, -4.0f);
        }
        break;
    }
}

void CCamp::setCursor4(INT nSelect, INT nMode) {
    FVECTOR vec;

    if (nMode < 2) {
        nSelect -= m_nDeckSelectTop;
        if (nSelect >= 9) {
            m_nDeckSelectTop += 3;
            nSelect -= 3;
        } else if (nSelect < 0) {
            m_nDeckSelectTop -= 3;
            nSelect += 3;
        }
        vec = m_pDeckDeckCard[nSelect]->m_seqControl.vPos;
    } else if (nMode >= 5) {
        FLOAT x = nSelect ? nSelect * 24.0f + -30.0f : -56.0f;
        vec.set(x, -70.0f, 0.0f, 1.0f);



    }
    switch (nMode) {
    case 0:
        if (!m_pCursor4Ctrl) {
            m_pCursor4Ctrl = new KH_AT("(8007)") C2DSeqCtrl(1);
            m_pCursor4Ctrl->setLayer(0x13);
            m_pCursor4Ctrl->setParent(m_pDeckDeckCtrl[0]);
            m_pCursor4Ctrl->addSeq(m_pSeqName, m_bShortCutMode ? 0x103 : 0x9F);
            m_pCursor4Ctrl->addSeqShift(m_pSeqName, 0x12, -8.0f, -30.0f);
        } else {
            m_pCursor4Ctrl->replaceSeq(0, m_pSeqName, m_bShortCutMode ? 0x103 : 0x9F);
        }
    set:
        m_vPosCursor4Current = vec;
    case 1:
    case 6:
        m_vPosCursor4Target = vec;
        m_vPosCursor4Old = m_vPosCursor4Current;
        m_fCursor4Timer = 0.0f;
        m_fgCursor4Move = 1;
        m_pCursor4Ctrl->enableDisp(TRUE);
        break;
    case 2:
        delete m_pCursor4Ctrl;
        m_pCursor4Ctrl = NULL;
        break;
    case 5:
        m_pCursor4Ctrl->enableDisp(0, FALSE);
        goto set;
    case 7:
        m_pCursor4Ctrl->enableDisp(0, TRUE);
        break;
    }
}

void CCamp::setCursor4Hist(INT nSelect, INT nMode) {
    FVECTOR vec;
    INT idx;

    if (nMode < 2) {
        idx = nSelect - m_nDeckSelectTop;
        if (idx < 0 || idx >= 9) {
            idx = -1;
        } else {
            vec = m_pDeckDeckCard[idx]->m_seqControl.vPos;
        }
    }
    switch (nMode) {
    case 0:
        if (!m_pCursor4HistCtrl) {
            m_pCursor4HistCtrl = new KH_AT("(8073)") C2DSeqCtrl;
            m_pCursor4HistCtrl->setLayer(0x12);
            m_pCursor4HistCtrl->setParent(m_pDeckDeckCtrl[0]);
            m_pCursor4HistCtrl->addSeqShift(m_pSeqName, 0x11, -4.0f, -20.0f);
        }
        m_pCursor4HistCtrl->enableDisp(FALSE);
        break;
    case 1:
        if (idx >= 0) {
            m_pCursor4HistCtrl->setLocalPos(vec);
            m_pCursor4HistCtrl->enableDisp(TRUE);
        } else {
            m_pCursor4HistCtrl->enableDisp(FALSE);
        }
        break;
    case 2:
        if (m_pCursor4HistCtrl) {
            m_pCursor4HistCtrl->enableDisp(FALSE);
        }
        break;
    }
}

CButtonCursor::CButtonCursor(INT nLayer, C2DSeqCtrl* pParentCtrl, USHORT nCursorSeq) : C2DSeqCtrl(1) {
    static LPCTSTR pSeqName = "camp:00";

    setLayer(nLayer);
    setParent(pParentCtrl);
    addSeq(pSeqName, nCursorSeq);
    addSeqShift(pSeqName, 0x12, 8.0f, 0.0f);
    enableDisp(FALSE);
    m_pLightCtrl = new KH_AT("(8145)") C2DSeqCtrl;
    m_pLightCtrl->setLayer(nLayer);
    m_pLightCtrl->addSeqShift(pSeqName, 0x14, 0.0f, 0.0f);
    m_pLightCtrl->enableDisp(FALSE);
    m_fgInit = 0;
}

CButtonCursor::~CButtonCursor() {
    delete m_pLightCtrl;
    m_pLightCtrl = NULL;
}

void CButtonCursor::setDisp(BOOL bDisp, BOOL bLightOnly) {
    if (!bLightOnly) {
        enableDisp(bDisp);
    }
    if (m_pLightCtrl) {
        m_pLightCtrl->enableDisp(bDisp);
    }
}

void CButtonCursor::setOut() {
    static LPCTSTR pSeqName = "camp:00";

    enableDisp(FALSE);
    if (m_pLightCtrl) {
        m_pLightCtrl->replaceSeq(0, pSeqName, 0x15);
    }
}

void CButtonCursor::select(INT nSelect, CButton** ppButton, FLOAT* pPosX, FLOAT* pPosY) {
    FVECTOR vec;

    if (ppButton[nSelect]) {
        C2DSeqCtrl* pCtrl = ppButton[nSelect]->get2DSeqCtrl();
        setLocalPos(pCtrl->m_seqControl.vPos);
        if (m_pLightCtrl) {
            vec = pCtrl->m_seqControl.vPos;
            if (pPosX) {
                vec.x += pPosX[nSelect];
            }
            if (pPosY) {
                vec.y += pPosY[nSelect];
            }
            m_fgLightMove = 1;
            m_vPosLightTarget = vec;
            m_fLightTimer = 0.0f;
            m_vPosLightOld = m_fgInit ? m_vPosLightCurrent : vec;
        }
    }
}

void CButtonCursor::moveLight(FLOAT fFrameRate) {
    if (m_fgLightMove && m_pLightCtrl) {
        if (!m_fgInit) {
            m_fgInit = 1;
            fFrameRate = 100.0f;
        }
        m_fLightTimer += fFrameRate;
        if (m_fLightTimer >= 30.0f) {
            m_fLightTimer = 30.0f;
            m_fgLightMove = 0;
        }

        FLOAT t = 1.0f - powf(1.0f - m_fLightTimer / 30.0f, 2.0f);
        InterVectorXYZ(m_vPosLightCurrent, m_vPosLightTarget, m_vPosLightOld, t);
        m_pLightCtrl->setPos(m_vPosLightCurrent);
    }
}

INT CCamp::selectMainCmd(INT nSelect, BOOL bActive) {
    unsigned int helpCtdTbl[2][2][7] = {
        {
            { 0x500400, 0x500401, 0x500402, 0x500403, 0x500404, 0x500405, 0x500406 },
            { 0x500407, 0x500409, 0x50040A, 0x50040B, 0x50040C, 0x50040E, 0 },
        },
        {
            { 0x50042A, 0x500401, 0x500402, 0x500403, 0x500404, 0x50042C, 0x500406 },
            { 0x50042B, 0x50040A, 0, 0, 0, 0, 0 },
        },
    };
    static LPCTSTR pStrNull = "";

    if (nSelect < 0) {
        nSelect = m_nCurrentCmdMax - 1;
    } else if (nSelect >= m_nCurrentCmdMax) {
        nSelect = 0;
    }
    UINT col = 0x80404040;
    m_pMainCmdCtrl[m_nMainCmdSel]->replaceSeq(0, m_pSeqName, 5);
    m_pMainCmdFnt[m_nMainCmdSel]->m_C0.col = col;
    m_pMainCmdCtrl[m_nMainCmdSel]->changeLayer(0xC);
    m_pHelpFnt->setText(pStrNull);
    if (bActive) {
        UINT col2 = 0x80C0C0C0;
        m_pMainCmdCtrl[nSelect]->replaceSeq(0, m_pSeqName, 3);
        m_pMainCmdFnt[nSelect]->m_C0.col = col2;
        m_pMainCmdCtrl[nSelect]->changeLayer(0xD);
        setCursor(nSelect, 0);
        setLight(nSelect, 0);
        m_nMainCmdSel = nSelect;
        if (m_nMainMode < 2) {
            m_pHelpFnt->setText(CCtdMan::getText(helpCtdTbl[m_bPlayerKind][m_nMainMode][nSelect]));
        }
    }
    return 1;
}

void CCamp::moveLight() {
    if (m_fgLightMove && m_pLightCtrl) {
        m_fLightTimer += getFrameRateF();
        if (m_fLightTimer >= 30.0f) {
            m_fLightTimer = 30.0f;
            m_fgLightMove = 0;
        }

        FLOAT t = 1.0f - powf(1.0f - m_fLightTimer / 30.0f, 2.0f);
        InterVectorXYZ(m_vPosLightCurrent, m_vPosLightTarget, m_vPosLightOld, t);
        m_pLightCtrl->setPos(m_vPosLightCurrent);
    }
}

void CCamp::moveCursor2() {
    if (m_fgCursor2Move && m_pCursor2Ctrl) {
        m_fCursor2Timer += getFrameRateF();
        if (m_fCursor2Timer >= 10.0f) {
            m_fCursor2Timer = 10.0f;
            m_fgCursor2Move = 0;
        }

        FLOAT t = 1.0f - powf(1.0f - m_fCursor2Timer / 10.0f, 2.5f);
        InterVectorXYZ(m_vPosCursor2Current, m_vPosCursor2Target, m_vPosCursor2Old, t);
        m_pCursor2Ctrl->setLocalPos(m_vPosCursor2Current);
    }
}

void CCamp::moveCursor3() {
    if (m_fgCursor3Move && m_pCursor3Ctrl) {
        m_fCursor3Timer += getFrameRateF();
        if (m_fCursor3Timer >= 10.0f) {
            m_fCursor3Timer = 10.0f;
            m_fgCursor3Move = 0;
        }

        FLOAT t = 1.0f - powf(1.0f - m_fCursor2Timer / 10.0f, 2.5f);
        InterVectorXYZ(m_vPosCursor3Current, m_vPosCursor3Target, m_vPosCursor3Old, t);
        m_pCursor3Ctrl->setLocalPos(m_vPosCursor3Current);
    }
}

void CCamp::moveCursor4() {
    if (m_fgCursor4Move && m_pCursor4Ctrl) {
        m_fCursor4Timer += getFrameRateF();
        if (m_fCursor4Timer >= 10.0f) {
            m_fCursor4Timer = 10.0f;
            m_fgCursor4Move = 0;
        }

        FLOAT t = 1.0f - powf(1.0f - m_fCursor4Timer / 10.0f, 2.5f);
        InterVectorXYZ(m_vPosCursor4Current, m_vPosCursor4Target, m_vPosCursor4Old, t);
        m_pCursor4Ctrl->setLocalPos(m_vPosCursor4Current);
    }
}

void CCamp::moveWM() {
    if (m_fgWMMove) {
        m_fWMTimer += getFrameRateF();
        if (m_fWMTimer >= 12.0f) {
            m_fWMTimer = 12.0f;
            m_fgWMMove = 0;
        }
        FLOAT t = 1.0f - powf(1.0f - m_fWMTimer / 12.0f, 3.5f);
        InterVector(m_vWMPosCurrent0, m_vWMPosTarget, m_vWMPosOld, t);
        m_vWMPosCurrent = m_vWMPosCurrent0;
        m_vWMPosCurrent.w = 0.0f;
        m_pWMMoveCtrl->setLocalPos(m_vWMPosCurrent);
        m_pWMMoveCtrl->setScl(m_vWMPosCurrent0.w);
    }
}

INT CCamp::searchNumExist(INT nMethod, INT idx, BOOL bMapCard) {
    unsigned char (*pKind10)[10];
    USHORT kind;
    INT i;

    if (!bMapCard) {
        kind = m_kindTbl[m_nKindSelect];
        pKind10 = m_nKind10Tbl;
    } else {
        kind = m_nMapKindTbl[m_nMCSelSelect];
        pKind10 = m_nCardKind10;
    }
    for (i = 0; i < m_nCardKindMax; i++) {
        if (m_nCardKind[i] == kind) {
            break;
        }
    }
    unsigned char* pNum = pKind10[i];
    switch (nMethod) {
    case 0:
        for (i = 0; i < 10; i++) {
            if (pNum[i]) {
                return i;
            }
        }
        return -1;
    case 1:
        for (INT j = idx; idx < 5 ? j < 5 : j < 10; j++) {
            if (pNum[j]) {
                return j;
            }
        }
        return -1;
    case 2:
        for (INT j = idx; idx >= 5 ? j >= 5 : j >= 0; j--) {
            if (pNum[j]) {
                return j;
            }
        }
        return -1;
    case 3:
    case 4: {
        if (pNum[idx]) {
            return idx;
        }
        INT n1 = searchNumExist(1, idx, bMapCard);
        INT n2 = searchNumExist(2, idx, bMapCard);
        if (n1 < 0) {
            INT ret = -1;
            if (ret < n2) {
                ret = n2;
            }
            return ret;
        }
        if (n2 < 0) {
            return n1;
        }
        if (nMethod == 3) {
            return n1 - idx >= idx - n2 ? n2 : n1;
        }
        return idx - n2 < n1 - idx ? n2 : n1;
    }
    }
    return -1;
}

BOOL CCamp::checkNorma(SHORT idx, USHORT& roomID, INT nRoomSelect) {
    if (m_bPooWorld) {
        return FALSE;
    }
    if (nRoomSelect < 0) {
        nRoomSelect = m_nRoomSelect;
    }
    INT nRoom = m_roomMap[nRoomSelect] - 1;
    RoomInfo* pRoomInfo = CSceneInfo::getRoomInfo(m_roomMap[nRoomSelect]);
    if (pRoomInfo->_pSrcInfo->_nRoomType == 6 && pRoomInfo->hasEntered()) {
        return FALSE;
    }
    if (pRoomInfo->_pSrcInfo->_nRoomType == 7) {
        return FALSE;
    }
    if (m_roomDiscover[nRoom] < 0) {
        return FALSE;
    }
    if (idx >= 0) {
        roomID = (m_roomInfo[nRoom] >> (idx * 8)) & 0xFF;
        if (!roomID) {
            return FALSE;
        }
        INT door = (m_roomDoor[roomID - 1] >> (idx * 2)) & 3;
        if (door == 0 || door == 3) {
            return FALSE;
        }
        return CSceneInfo::getRoomInfo(pRoomInfo->_doors[(idx + 2) & 3]._nDstRoom)->_pSrcInfo->_nRoomType != 7;
    }
    INT i;
    for (i = 0; i < 4; i++) {
        roomID = (m_roomInfo[nRoom] >> (i * 8)) & 0xFF;
        if (roomID) {
            INT door = (m_roomDoor[roomID - 1] >> (i * 2)) & 3;
            if (door == 1 || door == 2) {
                break;
            }
        }
    }
    return i < 4;
}

INT CCamp::searchRoom(INT iDir, BOOL bDiscover) {
    INT nRoomSelect;
    INT idx;
    INT idx2;
    INT idx3;
    INT i;
    USHORT roomID;

    nRoomSelect = m_nRoomSelect;
    switch (iDir) {
    case 0:
        idx = nRoomSelect + 6;
        if (idx >= 48) {
            return -1;
        }
        goto updown;
    case 1:
        idx = nRoomSelect - 6;
        if (idx < 0) {
            return -1;
        }
    updown:
        if (m_roomMap[idx] && (!bDiscover || checkNorma(-1, roomID, idx))) {
            return idx;
        }
        idx2 = idx % 6;
        idx3 = idx - idx2;
        for (i = 1; i < 11; i++) {
            if ((i & 1) == 0) {
                idx2 -= i;
            } else {
                idx2 += i;
            }
            if (idx2 >= 0 && idx2 < 6) {
                idx = idx3 + idx2;
                if (m_roomMap[idx] && (!bDiscover || checkNorma(-1, roomID, idx))) {
                    break;
                }
            }
        }
        return i > 10 ? -1 : idx;
    case 2:
        idx = nRoomSelect - 1;
        if (idx % 6 == 5) {
            return -1;
        }
        goto leftright;
    case 3:
        idx = nRoomSelect + 1;
        if (idx % 6 == 0) {
            return -1;
        }
    leftright:
        if (m_roomMap[idx] && (!bDiscover || checkNorma(-1, roomID, idx))) {
            return idx;
        }
        for (i = 1; i < 15; i++) {
            if ((i & 1) == 0) {
                idx = idx + i * -6;
            } else {
                idx = idx + i * 6;
            }
            idx3 = idx; // MATCH: the extra copy pushes checkNorma's exit test past expand_end_loop's 30-insn scan (not rotated)
            if (idx3 < 0 || idx3 >= 48 || !m_roomMap[idx3]) continue; if (!bDiscover) break; if (checkNorma(-1, roomID, idx3)) break;
        }
        return i > 14 ? -1 : idx;
    }
    return -1;
}

void CCamp::setCardHelp(SHORT kind) {
    static LPCTSTR pStrNull = "";
    CARD card;

    if (kind == 0 || kind >= 0xCC) {
        m_pHelpFnt->setText(pStrNull);
        return;
    }
    card.init(kind, 0);
    m_pHelpFnt->setText(card.getName());
}

void CCamp::setRoomHelp(SHORT kind, USHORT idxPooRoom) {
    m_pHelpFnt->setText(getRoomHelp(kind, idxPooRoom));
}

void CCamp::setPoint(INT nSetCmd) {
    static LPCTSTR pStrNum = "%5d";
    CHAR str[16];
    FVECTOR vec;

    if (m_bPlayerKind) {
        return;
    }
    switch (nSetCmd) {
    case 0:
        if (!m_pPointCtrl) {
            UINT nPoint = CStatus::getMP();
            sprintf(str, pStrNum, nPoint > 99999 ? 99999 : nPoint);
            m_pPointCtrl = new KH_AT("(8586)") C2DSeqCtrl;
            m_pPointCtrl->setLayer(0xF);
            m_pPointCtrl->addSeq(m_pSeqName, 0x3A);
            vec.init();
            m_pPointCtrl->setPos(vec);
            for (INT i = 0; i < 5; i++) {
                if (str[i] != ' ') {
                    m_pPointCtrl->addSeqShift(m_pSeqName, str[i] - 0x11, i * 16.0f + 16.0f, 0.0f);
                }
            }
        } else {
            m_pPointCtrl->replaceSeq(0, m_pSeqName, 0x3A);
        }
        break;
    case 4:
        m_pPointCtrl->replaceSeq(0, m_pSeqName, 0x3C);
        break;
    case 3:
        m_pPointCtrl->replaceSeq(0, m_pSeqName, 0x3B);
        break;
    }
}

void CCamp::setTime(INT nSetCmd) {
    static LPCTSTR pStrNum = "%3d:%02d";
    CHAR str[16];
    FVECTOR vec;

    switch (nSetCmd) {
    case 0:
        m_fgTimeDisp = 1;
        m_fgMinute = 0xFF;
        if (!m_pTimeCtrl) {
            m_pTimeCtrl = new KH_AT("(8627)") C2DSeqCtrl;
            m_pTimeCtrl->setLayer(0xF);
            m_pTimeCtrl->addSeq(m_pSeqName, 0x40);
            vec.init();
            if (m_bPlayerKind) {
                vec.y -= 35.0f;
            }
            m_pTimeCtrl->setPos(vec);
            for (INT i = 0; i < 6; i++) {
                m_pTimeCtrl->addSeq(m_pSeqName, 0x118);
            }
        } else {
            m_pTimeCtrl->replaceSeq(0, m_pSeqName, 0x40);
        }
        break;
    case 8:
        m_fgTimeDisp = 0;
        delete m_pTimeCtrl;
        m_pTimeCtrl = NULL;
        break;
    case 4:
        m_pTimeCtrl->replaceSeq(0, m_pSeqName, 0x42);
        break;
    case 3:
        m_pTimeCtrl->replaceSeq(0, m_pSeqName, 0x41);
        break;
    case 9:
        if (m_pTimeCtrl) {
            UINT nTime = CMainApp::getPlayTime();
            UINT nHour = nTime / 3600;
            UINT nMinute = nTime % 3600 / 60;
            if (nMinute != m_fgMinute) {
                m_fgMinute = nMinute;
                if (nHour >= 1000) {
                    nHour = 999;
                    nMinute = 59;
                }
                sprintf(str, pStrNum, nHour, nMinute);
                FLOAT x = 28.0f;
                for (INT i = 0; i < 6; i++) {
                    if (str[i] != ' ') {
                        if (str[i] != ':') {
                            m_pTimeCtrl->replaceSeqShift(i + 1, m_pSeqName, str[i] - 7, x, 0.0f);
                        } else {
                            x += -8.0f;
                            m_pTimeCtrl->replaceSeqShift(i + 1, m_pSeqName, 0x33, x, 0.0f);
                        }
                    }
                    x += 16.0f;
                }
            }
        }
        break;
    }
}

void CCamp::setWMRoomName(BYTE cardID, INT nRoomSelect, const RoomInfo* pRoomInfo, BOOL bIn) {
    static LPCTSTR pSeqName = "worldmap:00";
    static CHAR roomColorSeqTbl[4] = { 0x31, 0x2E, 0x2B, 0x34 };
    FLOOR::FloorLayout* pFloor;

    if (!m_bPooWorld) {
        pFloor = CSceneInfo::getCurrentFloorDataSrc();
    } else {
        pFloor = CSceneInfo::getFloorDataSrc(CSceneInfo::getFloorIndex(100, TRUE), TRUE);
    }
    INT nRoom = m_roomMap[nRoomSelect] - 1;
    FLOOR::RoomSrcInfo* pRooms = pFloor->_rooms;
    if ((m_roomDiscover[nRoom] >= 0 && !m_bPooWorld) || pRoomInfo->hasEntered()) {
        if (cardID && hasOpenedDoor(pRoomInfo, pRooms)) {
            CARD card;
            card.init(cardID, 0);
            INT idxPooRoom = m_roomMap[nRoomSelect] - 2;
            m_nWMRoomNameSeq = roomColorSeqTbl[card.m_nCategory21 ? card.m_nCategory21 - 9 : 3] - bIn;
            m_pWMRoomNameCtrl->replaceSeq(0, pSeqName, m_nWMRoomNameSeq);
            if (m_bPooWorld) {
                m_pWMRoomNameFnt->setText(CCtdMan::getText(idxPooRoom + 0x100));
            } else {
                m_pWMRoomNameFnt->setText(card.getName());
            }
            m_pWMRoomNameCtrl->enableDisp(TRUE);
            setRoomHelp(cardID, idxPooRoom);
            return;
        }
        m_pWMRoomNameCtrl->enableDisp(FALSE);
        UINT ctdID = 0x50040F;
        if (pRoomInfo->hasEntered()) {
            switch (pRooms[nRoom]._nRoomType) {
            case 2:
                ctdID = 0x500414;
                break;
            case 3:
                ctdID = 0x500415;
                break;
            case 4:
                ctdID = 0x500416;
                break;
            case 6:
                ctdID = 0x50042D;
                break;
            }
        } else {
            switch (pRooms[nRoom]._nRoomType) {
            case 2:
            case 3:
            case 4:
                ctdID = 0x500410;
                break;
            case 6:
                ctdID = 0x50042D;
                break;
            }
        }
        m_pHelpFnt->setText(CCtdMan::getText(ctdID));
    } else {
        m_pWMRoomNameCtrl->enableDisp(FALSE);
        setRoomHelp(0, 0);
    }
}

LPCTSTR CCamp::getRoomHelp(USHORT kind, USHORT idxPooRoom) {
    static LPCTSTR pBlankText = "";

    if (kind < 0xA1) {
        return pBlankText;
    }
    if (kind < 0xDB) {
        return CCtdMan::getText(kind + 0x35F);
    }
    if (kind == 0xDB) {
        return CCtdMan::getText(idxPooRoom + 0x42A);
    }
    return pBlankText;

}

void CCamp::setCurrentRoomName(INT nSetCmd) {
    static LPCTSTR pWTSeqName = "world_title:00";
    FVECTOR vec;
    CARD card;
    G_RGBA rgba;

    if (!m_iWorldRsrc) {
        return;
    }
    BYTE cardID = 0;
    switch (nSetCmd) {
    case 0:
        if (!m_pTitleCtrl) {
            vec.init();
            m_pTitleCtrl = new KH_AT("(8855)") C2DSeqCtrl;
            m_pTitleCtrl->setLayer(0xF);
            m_pTitleCtrl->addSeq(pWTSeqName, 0);
            m_pTitleCtrl->setPos(vec);
            m_pCurrentRoomNameCtrl = new KH_AT("(8860)") C2DSeqCtrl;
            m_pCurrentRoomNameCtrl->setLayer(0xF);
            m_pCurrentRoomNameCtrl->enableColorCtrl(TRUE);
            m_pCurrentRoomNameCtrl->addSeq(m_pSeqName, 0x3D);
            m_pCurrentRoomNameCtrl->setPos(vec);
            RoomInfo* pRoomInfo = CSceneInfo::getRoomInfo(-1);
            if (pRoomInfo) {
                cardID = pRoomInfo->getRoomNameCardID();
            }
            card.init(cardID, 0);
            rgba.col = 0x807C7C7C;
            m_pCurrentRoomNameFnt = new KH_AT("(8871)") C2DFnt(0, 0, &rgba, 0xF, card.getName(), 0x241);
            m_pCurrentRoomNameFnt->setLayer(0xF, 0);
            m_pCurrentRoomNameCtrl->addFnt(m_pSeqName, m_pCurrentRoomNameFnt);
        } else {
            m_pTitleCtrl->replaceSeq(0, pWTSeqName, 0);
            m_pCurrentRoomNameCtrl->replaceSeq(0, m_pSeqName, 0x3D);
        }
        break;
    case 3:
        if (m_pTitleCtrl) {
            m_pTitleCtrl->replaceSeq(0, pWTSeqName, 1);
            m_pCurrentRoomNameCtrl->replaceSeq(0, m_pSeqName, 0x3E);
        }
        break;
    case 4:
        if (m_pTitleCtrl) {
            m_pTitleCtrl->replaceSeq(0, pWTSeqName, 2);
            m_pCurrentRoomNameCtrl->replaceSeq(0, m_pSeqName, 0x3F);
        }
        break;
    }
}

void CCamp::setTotalCP() {
    m_nTotalCP = 0;
    CARD* pDeck = CCard::getDeck(m_nDeckNoSelect);
    for (INT i = 0; i < 99; i++) {
        if (pDeck[i].m_nId) {
            m_nTotalCP += CCard::getCP(pDeck[i]);
        }
    }
}

INT CCamp::getCardIndex(INT nIndex) {
    INT idx;

    if (!m_nDeckCategorySelect) {
        idx = nIndex;
    } else {
        INT i;
        idx = -1;
        for (i = 0; i < nIndex + 1; i++) {
            idx = CCard::searchDeck(m_pDeck[i], m_nDeckNoSelect, ++idx);
            if (idx < 0) {
                return -1;
            }
        }
        if (i < nIndex + 1) {
            return -1;
        }
    }
    return idx;
}

BOOL CCamp::removeDeck(INT nIndex) {
    if (m_pDeck[nIndex].m_nId) {
        INT idx = getCardIndex(nIndex);
        if (idx >= 0) {
            CCard::removeDeck(m_nDeckNoSelect, idx);
            return TRUE;
        }
        return FALSE;
    }
    return FALSE;
}

BOOL CCamp::setShortCut(INT nIndex) {
    INT idx;

    if (!m_pDeck[nIndex].m_nId || m_pDeck[nIndex].m_nCategory20 == 7 || (idx = getCardIndex(nIndex)) < 0) {
        return FALSE;
    }
    INT nShortCut = CCard::getShortCut(m_nDeckNoSelect);
    if (nShortCut >= 0 && nShortCut == idx) {
        CCard::clearShortCut(m_nDeckNoSelect);
        return TRUE;
    }
    CCard::setShortCut(m_nDeckNoSelect, idx);
    return TRUE;
}

CampRoomInfo* CCamp::getRoomInfo(USHORT kind) {
    static CampRoomInfo campRoomInfo[29] = {
        { 1, 2, 1024 }, { 3, 3, 1025 }, { 1, 2, 1026 }, { 2, 2, 1027 }, { 1, 1, 1028 }, { 2, 3, 1029 },
        { 2, 2, 1030 }, { 1, 3, 1031 }, { 1, 1, 1032 }, { 3, 3, 1033 }, { 2, 3, 1034 }, { 2, 2, 1035 },
        { 2, 2, 1036 }, { 1, 2, 1037 }, { 2, 2, 1038 }, { 1, 2, 1039 }, { 2, 3, 1040 }, { 2, 3, 1041 },
        { 0, 1, 1042 }, { 2, 1, 1043 }, { 1, 1, 1044 }, { 0, 1, 1045 }, { 4, 4, 1046 }, { 0, 1, 1047 },
        { 4, 4, 1048 }, { 5, 5, 1049 }, { 5, 5, 1050 }, { 5, 5, 1051 }, { 5, 5, 1052 },
    };

    if (kind < 0xA1 || kind >= 0xA1 + 29) {
        return NULL;
    }
    return &campRoomInfo[kind - 0xA1];
}

INT CCamp::hasOpenedDoor(const RoomInfo* pRoomInfo, const FLOOR::RoomSrcInfo* pRoomSrcInfo) {
    INT cnt = 0;

    for (INT i = 0; i < 4; i++) {
        const DoorInfo& door = pRoomInfo->_doors[i];
        if (door._type == 8 && pRoomSrcInfo->_nRoomType == 5 && !pRoomInfo->hasEntered()) {
            continue;
        }
        if (door.getDoorState() == 2) {
            cnt++;
        }
    }
    return cnt;
}

void CCamp::setMainParty(INT nSetCmd) {
    unsigned char playerRsrcIDTbl[4] = { 1, 11, 101, 111 };
    FVECTOR vec;
    FVECTOR vec2;

    switch (nSetCmd) {
    case 0:
    case 1:
        vec.set(1.0f, 1.0f, 1.0f, 0.0f);
        vec2.setAll(1.0f);
        if (!m_pMenuModel) {
            INT idx = m_bPlayerKind * 2 + (CSceneInfo::getCurrentWorldID(TRUE) == 6);
            m_pMenuModel = new KH_AT("(9037)") CMenuModel(this, playerRsrcIDTbl[idx] + 10000, NULL, !m_bPlayerKind ? -40 : -10, 55);
            m_pMenuModel->changeAnim(2, FALSE);
            m_pMenuModel->setColorFade(vec, vec2, 16.0f);
            m_pMenuModel->setScale(0.1125f);
        } else {
            m_pMenuModel->setColorFade(vec, vec2, 8.0f);
        }
        m_pBaseCtrl->replaceSeq(1, m_pSeqName, 0x1A);
        break;
    case 3:
        m_pBaseCtrl->replaceSeq(1, m_pSeqName, 0x1B);
        break;
    case 4:
        vec.setAll(1.0f);
        vec2.set(1.0f, 1.0f, 1.0f, 0.0f);
        m_pMenuModel->setColorFade(vec, vec2, 8.0f);
        m_pBaseCtrl->replaceSeq(1, m_pSeqName, 0x1C);
        break;
    case 2:
    case 5:
    case 6:
    case 7:
    case 8:
        break;
    }
}

void CCamp::disp() {
    if (m_pHelpFnt) {
        m_pHelpFnt->disp();
    }
    if (m_pZFill) {
        m_pZFill->disp();
    }
}


const unsigned char CCamp::m_techSoraCateTbl[86][2] = {
    { 4, 0 }, { 6, 1 }, { 8, 1 }, { 5, 1 }, { 7, 1 }, { 9, 1 }, { 10, 1 }, { 11, 1 },
    { 12, 1 }, { 13, 2 }, { 20, 3 }, { 14, 3 }, { 21, 3 }, { 15, 3 }, { 22, 3 }, { 16, 3 },
    { 23, 3 }, { 17, 3 }, { 24, 3 }, { 18, 3 }, { 25, 3 }, { 19, 3 }, { 26, 3 }, { 43, 3 },
    { 44, 3 }, { 28, 3 }, { 48, 3 }, { 27, 3 }, { 37, 3 }, { 47, 3 }, { 49, 3 }, { 45, 3 },
    { 40, 3 }, { 36, 3 }, { 42, 3 }, { 50, 3 }, { 29, 3 }, { 46, 3 }, { 32, 3 }, { 30, 3 },
    { 38, 3 }, { 39, 3 }, { 41, 3 }, { 35, 3 }, { 31, 3 }, { 51, 3 }, { 33, 3 }, { 34, 3 },
    { 52, 4 }, { 53, 5 }, { 61, 5 }, { 62, 5 }, { 65, 5 }, { 67, 5 }, { 66, 5 }, { 54, 5 },
    { 55, 5 }, { 63, 5 }, { 64, 5 }, { 56, 5 }, { 57, 5 }, { 58, 5 }, { 59, 5 }, { 60, 5 },
    { 68, 6 }, { 69, 7 }, { 70, 7 }, { 71, 7 }, { 72, 7 }, { 73, 7 }, { 74, 7 }, { 85, 7 },
    { 86, 7 }, { 75, 7 }, { 76, 7 }, { 77, 7 }, { 78, 7 }, { 79, 7 }, { 80, 7 }, { 81, 7 },
    { 82, 7 }, { 83, 7 }, { 84, 7 }, { 90, 14 }, { 91, 15 }, { 92, 15 },
};

const unsigned char CCamp::m_techRikuCateTbl[14][2] = {
    { 95, 8 }, { 96, 9 }, { 97, 9 }, { 98, 10 }, { 99, 11 }, { 100, 11 }, { 101, 11 }, { 102, 12 },
    { 105, 13 }, { 103, 13 }, { 106, 13 }, { 104, 13 }, { 107, 13 }, { 109, 16 },
};
/* (A stopgap stood here: D_004E5374 = 0x004E5374 for string bytes in .sndata that splat had disassembled as a
 * reference. tools/sndll.py now generates .sndata with only real pointers symbolic, so nothing refers to it.)
 */



// SOURCE DATA: .data .rodata
// SOURCE DATA SLUS_217.99_retail: .data .rodata
