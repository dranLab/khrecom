#ifndef FORM_MENU_CAMP_H
#define FORM_MENU_CAMP_H

#include "khtypes.h"
#include "Form/Menu/Menu.h"
#include "System/Vector.h"
#include "System/Graphic/2d/2DSeqCtrl.h"
#include "Game/CardKind.h"

namespace D2 {
class C2DFnt;
}
class C2DCard;
class CButton;
class CScrollBar;
class CStrPrim;
class CTextWnd;
class CZFill;
class CMenuModel;
struct BC_PARAM;
struct RoomInfo;
namespace FLOOR {
struct RoomSrcInfo;
}

struct CampRoomInfo {
    CHAR nEnemyVolume;
    CHAR nRoomVolume;
    USHORT nRoomExplane;
};

class CButtonCursor;

class CCamp : public CMenu {
private:
    static CCamp* m_pInstance;
    static char* m_pMainCmdName[2][2][7];
    static INT m_iWorldRsrc;
    static const unsigned char m_techSoraCateTbl[86][2];
    static const unsigned char m_techRikuCateTbl[14][2];
    static const unsigned char m_itemAWBTbl[7];

    BYTE *m_addrRsrcBuf;                          /* 0x0080 */
    INT m_nSubStep;                               /* 0x0084 */
    INT m_nTechWinStep;                           /* 0x0088 */
    INT m_nErrorWinStep;                          /* 0x008c */
    INT m_nMainMode;                              /* 0x0090 */
    INT m_nMainCmdSel;                            /* 0x0094 */
    INT m_nMainCmdSelSave;                        /* 0x0098 */
    INT m_nMainCmdHistLevel;                      /* 0x009c */
    int m_nMainCmdHistRun[2];                     /* 0x00a0 */
    INT m_fgLightMove;                            /* 0x00a8 */
    FLOAT m_fLightTimer;                          /* 0x00ac */
    INT m_fgCursor2Move;                          /* 0x00b0 */
    FLOAT m_fCursor2Timer;                        /* 0x00b4 */
    INT m_fgCursor3Move;                          /* 0x00b8 */
    FLOAT m_fCursor3Timer;                        /* 0x00bc */
    INT m_fgCursor4Move;                          /* 0x00c0 */
    FLOAT m_fCursor4Timer;                        /* 0x00c4 */
    HANDLE m_hEtcMenu;                            /* 0x00c8 */
    int m_nButtonHelpKind[2];                     /* 0x00cc */
    int m_nButtonHelpRun[2];                      /* 0x00d4 */
    INT m_nError;                                 /* 0x00dc */
    INT m_nSelYesNo;                              /* 0x00e0 */
    BC_PARAM *m_pBCParam;                         /* 0x00e4 */
    CZFill *m_pZFill;                             /* 0x00e8 */
    CTextWnd *m_pTextWnd;                         /* 0x00ec */
    D2::C2DSeqCtrl *m_pBaseCtrl;                      /* 0x00f0 */
    D2::C2DSeqCtrl *m_pTitleCtrl;                     /* 0x00f4 */
    D2::C2DSeqCtrl *m_pWindow0Ctrl;                   /* 0x00f8 */
    D2::C2DSeqCtrl *m_pWindow1Ctrl;                   /* 0x00fc */
    D2::C2DFnt *m_pWindow0Fnt[5];                     /* 0x0100 */
    D2::C2DFnt *m_pWindow1Fnt;                        /* 0x0114 */
    C2DCard *m_pWindow1Card[3];                   /* 0x0118 */
    CButton *m_pWin1EtcButton[4];                 /* 0x0124 */
    D2::C2DSeqCtrl *m_pMainCmdCtrl[7];                /* 0x0134 */
    D2::C2DFnt *m_pMainCmdFnt[7];                     /* 0x0150 */
    D2::C2DSeqCtrl *m_pMainCmdHistCtrl[2];            /* 0x016c */
    D2::C2DFnt *m_pMainCmdHistFnt[2];                 /* 0x0174 */
    D2::C2DSeqCtrl *m_pMCHistLightCtrl[2];            /* 0x017c */
    D2::C2DSeqCtrl *m_pCursorCtrl;                    /* 0x0184 */
    D2::C2DSeqCtrl *m_pLightCtrl;                     /* 0x0188 */
    D2::C2DSeqCtrl *m_pCursor2Ctrl;                   /* 0x018c */
    D2::C2DSeqCtrl *m_pCursor3Ctrl;                   /* 0x0190 */
    D2::C2DSeqCtrl *m_pCursor4Ctrl;                   /* 0x0194 */
    D2::C2DSeqCtrl *m_pCursor5Ctrl;                   /* 0x0198 */
    D2::C2DSeqCtrl *m_pMapCursorCtrl;                 /* 0x019c */
    D2::C2DSeqCtrl *m_pMapGuideCtrl;                  /* 0x01a0 */
    D2::C2DSeqCtrl *m_pCursor4HistCtrl;               /* 0x01a4 */
    D2::C2DSeqCtrl *m_pDeckCardCtrl[3];               /* 0x01a8 */
    D2::C2DFnt *m_pDeckCardFnt[5];                    /* 0x01b4 */
    C2DCard *m_pDeckCardCard[9];                  /* 0x01c8 */
    CScrollBar *m_pDeckCardScrollBar;             /* 0x01ec */
    D2::C2DSeqCtrl *m_pDeckDeckCtrl[3];               /* 0x01f0 */
    D2::C2DFnt *m_pDeckDeckFnt[7];                    /* 0x01fc */
    C2DCard *m_pDeckDeckCard[9];                  /* 0x0218 */
    C2DCard *m_pDeckDeckCardSC;                   /* 0x023c */
    CScrollBar *m_pDeckDeckScrollBar;             /* 0x0240 */
    D2::C2DSeqCtrl *m_pDeckInfoCtrl;                  /* 0x0244 */
    D2::C2DFnt *m_pDeckInfoFnt[5];                    /* 0x0248 */
    C2DCard *m_pDeckInfoCard;                     /* 0x025c */
    D2::C2DSeqCtrl *m_pDeckInfoNumCtrl[10];           /* 0x0260 */
    D2::C2DFnt *m_pDeckInfoNumFnt[10];                /* 0x0288 */
    D2::C2DSeqCtrl *m_pDeckInfoIOCtrl;                /* 0x02b0 */
    D2::C2DSeqCtrl *m_pButtonHelpCtrl[2];             /* 0x02b4 */
    D2::C2DFnt *m_pDeckButtonFnt[2];                  /* 0x02bc */
    CButton *m_pButtonHelpSubButton;              /* 0x02c4 */
    D2::C2DSeqCtrl *m_pDeckOpeCtrl;                   /* 0x02c8 */
    D2::C2DFnt *m_pDeckOpeFnt;                        /* 0x02cc */
    D2::C2DSeqCtrl *m_pWMNormaCtrl;                   /* 0x02d0 */
    D2::C2DSeqCtrl *m_pRoomCtrl[32];                  /* 0x02d4 */
    D2::C2DSeqCtrl *m_pWayCtrl[32];                   /* 0x0354 */
    D2::C2DSeqCtrl *m_pShopAndSaveCtrl[32];           /* 0x03d4 */
    C2DCard *m_pShopAndSave[32];                  /* 0x0454 */
    D2::C2DSeqCtrl *m_pRoomArrowCtrl;                 /* 0x04d4 */
    D2::C2DSeqCtrl *m_pRoomNormaCtrl[4];              /* 0x04d8 */
    C2DCard *m_pRoomNorma[4];                     /* 0x04e8 */
    D2::C2DSeqCtrl *m_pWMCardCtrl;                    /* 0x04f8 */
    D2::C2DSeqCtrl *m_pWMFloorCtrl;                   /* 0x04fc */
    D2::C2DSeqCtrl *m_pWMRoomNameCtrl;                /* 0x0500 */
    D2::C2DFnt *m_pWMRoomNameFnt;                     /* 0x0504 */
    D2::C2DSeqCtrl *m_pWMMoveCtrl;                    /* 0x0508 */
    D2::C2DSeqCtrl *m_pWMNormaArrowCtrl;              /* 0x050c */
    D2::C2DSeqCtrl *m_pWMExplaneCtrl;                 /* 0x0510 */
    D2::C2DFnt *m_pWMExplaneFnt;                      /* 0x0514 */
    D2::C2DSeqCtrl *m_pCurrentRoomPosCtrl;            /* 0x0518 */
    D2::C2DSeqCtrl *m_pCurrentRoomNameCtrl;           /* 0x051c */
    D2::C2DFnt *m_pCurrentRoomNameFnt;                /* 0x0520 */
    D2::C2DSeqCtrl *m_pTimeCtrl;                      /* 0x0524 */
    D2::C2DSeqCtrl *m_pPointCtrl;                     /* 0x0528 */
    CStrPrim *m_pHelpFnt;                         /* 0x052c */
    D2::C2DSeqCtrl *m_pTechWinCtrl;                   /* 0x0530 */
    D2::C2DFnt *m_pTechWinFnt;                        /* 0x0534 */
    D2::C2DFnt *m_pTechCategoryFnt[3];                /* 0x0538 */
    CButton *m_pTechButton[9];                    /* 0x0544 */
    CScrollBar *m_pTechWinScrollBar;              /* 0x0568 */
    INT m_nTechSelect;                            /* 0x056c */
    INT m_nTechSelectTop;                         /* 0x0570 */
    INT m_nTechSelectMax;                         /* 0x0574 */
    INT m_nTechKind;                              /* 0x0578 */
    LPCTSTR m_pHelpTextOld;                       /* 0x057c */
    CButtonCursor *m_pTechWinBC;                  /* 0x0580 */
    D2::C2DSeqCtrl *m_pDeckSetWinCtrl;                /* 0x0584 */
    CButton *m_pDeckSetButton[6];                 /* 0x0588 */
    D2::C2DFnt *m_pDeckSetEquipFnt;                   /* 0x05a0 */
    INT m_nDeckSetSelect;                         /* 0x05a4 */
    CButtonCursor *m_pDeckSetBC;                  /* 0x05a8 */
    D2::C2DSeqCtrl *m_pDeckNameWinCtrl;               /* 0x05ac */
    D2::C2DSeqCtrl *m_pCardDeleteWinCtrl;             /* 0x05b0 */
    CButton *m_pDeckCheckErrorButton;             /* 0x05b4 */
    CButton *m_pButtonYenNo[2];                   /* 0x05b8 */
    D2::C2DSeqCtrl *m_pYNCursorCtrl;                  /* 0x05c0 */
    D2::C2DSeqCtrl *m_pYNLightCtrl;                   /* 0x05c4 */
    INT m_fgYNLightMove;                          /* 0x05c8 */
    FLOAT m_fYNLightTimer;                        /* 0x05cc */
    FVECTOR m_vYNPosLightTarget;                  /* 0x05d0 */
    FVECTOR m_vYNPosLightCurrent;                 /* 0x05e0 */
    FVECTOR m_vYNPosLightOld;                     /* 0x05f0 */
    D2::C2DSeqCtrl *m_pConfigWinCtrl;                 /* 0x0600 */
    CButton *m_pConfigButton[21];                 /* 0x0604 */
    CButtonCursor *m_pConfigBC;                   /* 0x0658 */
    INT m_nConfigSelect;                          /* 0x065c */
    char m_nConfigItemSelect[7];                  /* 0x0660 */
    D2::C2DSeqCtrl *m_pStatusWinCtrl;                 /* 0x0668 */
    D2::C2DSeqCtrl *m_pStatusTechWinCtrl[2];          /* 0x066c */
    CButton *m_pStatusButton[9];                  /* 0x0674 */
    CButton *m_pStatusTechButton[8];              /* 0x0698 */
    CButtonCursor *m_pStatusBC;                   /* 0x06b8 */
    D2::C2DFnt *m_pStatusFnt[9];                      /* 0x06bc */
    CScrollBar *m_pStatusTechScrollBar;           /* 0x06e0 */
    INT m_nStatusTechSelect;                      /* 0x06e4 */
    INT m_nStatusTechSelectTop;                   /* 0x06e8 */
    INT m_nStatusTechSelectMax;                   /* 0x06ec */
    INT m_nStatusTechCateSelect;                  /* 0x06f0 */
    INT m_nStatusMax;                             /* 0x06f4 */
    unsigned char m_techCateTbl[86][2];           /* 0x06f8 */
    int m_nStatusTech[5];                         /* 0x07a4 */
    D2::C2DSeqCtrl *m_pMCInfoWinCtrl;                 /* 0x07b8 */
    D2::C2DSeqCtrl *m_pMCSelWinCtrl[3];               /* 0x07bc */
    CScrollBar *m_pMCSelScrollBar;                /* 0x07c8 */
    C2DCard *m_pMCCard[9];                        /* 0x07cc */
    D2::C2DFnt *m_pMCFnt[5];                          /* 0x07f0 */
    C2DCard *m_pMCInfoCard;                       /* 0x0804 */
    D2::C2DSeqCtrl *m_pMCInfoNumCtrl[10];             /* 0x0808 */
    D2::C2DFnt *m_pMCInfoNumFnt[10];                  /* 0x0830 */
    D2::C2DFnt *m_pMCInfoFnt[4];                      /* 0x0858 */
    INT m_nMCSelSelect;                           /* 0x0868 */
    INT m_nMCSelSelectTop;                        /* 0x086c */
    INT m_nMCSelCateSelect;                       /* 0x0870 */
    INT m_nMCSelNumSelect;                        /* 0x0874 */
    FVECTOR m_vPosLightCurrent;                   /* 0x0880 */
    FVECTOR m_vPosLightTarget;                    /* 0x0890 */
    FVECTOR m_vPosLightOld;                       /* 0x08a0 */
    FVECTOR m_vPosCursor2Current;                 /* 0x08b0 */
    FVECTOR m_vPosCursor2Target;                  /* 0x08c0 */
    FVECTOR m_vPosCursor2Old;                     /* 0x08d0 */
    FVECTOR m_vPosCursor3Current;                 /* 0x08e0 */
    FVECTOR m_vPosCursor3Target;                  /* 0x08f0 */
    FVECTOR m_vPosCursor3Old;                     /* 0x0900 */
    FVECTOR m_vPosCursor4Current;                 /* 0x0910 */
    FVECTOR m_vPosCursor4Target;                  /* 0x0920 */
    FVECTOR m_vPosCursor4Old;                     /* 0x0930 */
    FVECTOR m_vWMPosMinMax;                       /* 0x0940 */
    FVECTOR m_vWMPosCurrent;                      /* 0x0950 */
    FVECTOR m_vWMPosCurrent0;                     /* 0x0960 */
    FVECTOR m_vWMPosTarget;                       /* 0x0970 */
    FVECTOR m_vWMPosOld;                          /* 0x0980 */
    USHORT m_kindTbl[298];            /* 0x0990 */
    unsigned char m_nKind10Tbl[298][10];          /* 0x0be4 */
    unsigned char m_nMapKindTbl[29];              /* 0x1788 */
    CARD m_deck[99];                              /* 0x17a8 */
    CARD *m_pDeckSrc;                             /* 0x1934 */
    CARD *m_pDeck;                                /* 0x1938 */
    INT m_nCard;                                  /* 0x193c */
    USHORT m_nKind[5];                /* 0x1940 */
    USHORT m_nKindTotal[5];           /* 0x194a */
    int m_nDeck[5];                               /* 0x1954 */
    int m_nSCIndex[5];                            /* 0x1968 */
    int m_nMapKind[5];                            /* 0x197c */
    int m_nMapKindTotal[5];                       /* 0x1990 */
    INT m_nKindSelectTop;                         /* 0x19a4 */
    INT m_nKindSelect;                            /* 0x19a8 */
    INT m_nKindCategorySelect;                    /* 0x19ac */
    BOOL m_bCategorySelectMode;                   /* 0x19b0 */
    BOOL m_bCardPermuteMode;                      /* 0x19b4 */
    INT m_nDeckSelectTop;                         /* 0x19b8 */
    INT m_nDeckSelect;                            /* 0x19bc */
    INT m_nDeckSelectHist;                        /* 0x19c0 */
    INT m_nDeckCategorySelect;                    /* 0x19c4 */
    INT m_nNumSelect;                             /* 0x19c8 */
    INT m_nCardInOut;                             /* 0x19cc */
    INT m_nDeckNoSelect;                          /* 0x19d0 */
    INT m_nDeckNoSelectSave;                      /* 0x19d4 */
    BOOL m_bWorldmap;                             /* 0x19d8 */
    INT m_nRoomCurrent;                           /* 0x19dc */
    INT m_nRoomSelect;                            /* 0x19e0 */
    INT m_nRoomNorma;                             /* 0x19e4 */
    FLOAT m_fRoomCurrentTimer;                    /* 0x19e8 */
    FLOAT m_fWMTimer;                             /* 0x19ec */
    INT m_fgWMMove;                               /* 0x19f0 */
    unsigned int m_roomInfo[32];                  /* 0x19f4 */
    unsigned char m_roomMap[48];                  /* 0x1a74 */
    char m_roomDiscover[32];                      /* 0x1aa4 */
    char m_roomDoor[32];                          /* 0x1ac4 */
    BOOL m_bRoomNorma;                            /* 0x1ae4 */
    CHAR m_nWMRoomNameSeq;                        /* 0x1ae8 */
    BYTE m_fgMinute;                              /* 0x1ae9 */
    BYTE m_fgTimeDisp;                            /* 0x1aea */
    INT m_nTotalCP;                               /* 0x1aec */
    BOOL m_bPlayerKind;                           /* 0x1af0 */
    BOOL m_bPooWorld;                             /* 0x1af4 */
    INT m_nCurrentCmdMax;                         /* 0x1af8 */
    BOOL m_bShortCutMode;                         /* 0x1afc */
    CMenuModel *m_pMenuModel;                     /* 0x1b00 */
    INT m_iFrame60;                               /* 0x1b04 */
    char m_fgNewTbl[110];                         /* 0x1b08 */

public:
    CCamp(CTask* p, BOOL bWorldmapMode);
    virtual ~CCamp();
    void dispose();
    virtual void main();
    virtual void disp();
    static void removeRsrc();

private:
    INT selectMainCmd(INT nSelect, BOOL bActive);
    void setCursor(INT nSelect, INT nMode);
    void setCursor2(INT nSelect, INT nMode, BOOL bMapCard);
    void setCursor3(INT nSelect, INT nMode, BOOL bMapCard);
    void setCursor4(INT nSelect, INT nMode);
    void setCursor4Hist(INT nSelect, INT nMode);
    void setLight(INT nSelect, INT nMode);
    void moveLight();
    void moveCursor2();
    void moveCursor3();
    void moveCursor4();
    INT searchNumExist(INT nMethod, INT idx, BOOL bMapCard);
    INT setMainCmd(INT nSetCmd, INT nMode);
    INT setMainCmdHist(INT nSetCmd, INT nMode);
    INT setStatusWin0(INT nSetCmd);
    INT setStatusWin1(INT nSetCmd);
    INT setButtonHelp(INT nSetCmd, INT nMode, INT nKind);
    void setDeckCard(INT nSetCmd);
    INT setDeckDeck(INT nSetCmd);
    INT setDeckInfo(INT nSetCmd);
    INT deckCommonButton();
    INT deckCommonButton2();
    INT setWorldmap(INT nSetCmd);
    void setWorldmapCursor();
    void moveWM();
    INT searchRoom(INT iDir, BOOL bDiscover);
    void setWMRoomName(BYTE cardID, INT nRoomSelect, const RoomInfo* pRoomInfo, BOOL bIn);
    void initCardKind(INT nInDeck, INT nInCategory, BOOL bCardDelete);
    void initDeck(INT nInDeck, INT nInCategory);
    void initCardMapKind(INT nInCategory);
    void adjustSelect(BOOL bMapCard);
    INT deckEditMenu();
    BOOL checkNorma(SHORT idx, USHORT& roomID, INT nRoomSelect);
    void setCurrentRoomName(INT nSetCmd);
    void setPoint(INT nSetCmd);
    void setTime(INT nSetCmd);
    void setCardHelp(SHORT kind);
    void setRoomHelp(SHORT kind, USHORT idxPooRoom);
    LPCTSTR getRoomHelp(USHORT kind, USHORT idxPooRoom);
    INT deckSetWinMenu();
    INT deckClearWinMenu();
    INT setDeckSetWin(INT nSetCmd);
    void setTotalCP();
    INT getCardIndex(INT nIndex);
    BOOL removeDeck(INT nIndex);
    BOOL setShortCut(INT nIndex);
    INT techWinMenu();
    INT setTechWin(INT nSetCmd);
    INT deckCheckErrorWinMenu(BOOL bCardDelete);
    void moveYNLight();
    INT configWinMenu();
    INT setConfigWin(INT nSetCmd);
    INT statusWinMenu();
    INT setStatusWin(INT nSetCmd);
    INT mapCardWinMenu();
    INT setMapCardWin(INT nSetCmd);
    void setMainParty(INT nSetCmd);
    CampRoomInfo* getRoomInfo(USHORT kind);
    INT hasOpenedDoor(const RoomInfo* pRoomInfo, const FLOOR::RoomSrcInfo* pRoomSrcInfo);
};

class CButtonCursor : public D2::C2DSeqCtrl {
private:
    D2::C2DSeqCtrl* m_pLightCtrl; /* 0x160 */
    FVECTOR m_vPosLightCurrent;   /* 0x170 */
    FVECTOR m_vPosLightTarget;    /* 0x180 */
    FVECTOR m_vPosLightOld;       /* 0x190 */
    INT m_fgLightMove;            /* 0x1a0 */
    FLOAT m_fLightTimer;          /* 0x1a4 */
    INT m_fgInit;                 /* 0x1a8 */

public:
    CButtonCursor(INT nLayer, D2::C2DSeqCtrl* pParentCtrl, USHORT nCursorSeq);
    virtual ~CButtonCursor();
    void setDisp(BOOL bDisp, BOOL bLightOnly);
    void setOut();
    void select(INT nSelect, CButton** ppButton, FLOAT* pPosX, FLOAT* pPosY);
    void moveLight(FLOAT fFrameRate);
};

#endif /* FORM_MENU_CAMP_H */
