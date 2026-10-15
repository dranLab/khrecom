#ifndef FORM_MENU_ROOMCREATION_H
#define FORM_MENU_ROOMCREATION_H

#include "khtypes.h"
#include "Form/Menu/Menu.h"
#include "System/Vector.h"
#include "Game/Card.h"

namespace D2 {
class C2DSeqCtrl;
class C2DFnt;
}
class C2DCard;
class CPathCamera;
class CGimmickDoor;
class CStrPrim;
class CScrollBar;
class CAttachCard;
struct RoomInfo;
struct DoorInfo;

struct RCRoomInfo {
    CHAR nEnemyVolume;
    CHAR nRoomVolume;
    USHORT nRoomExplane;
};

class CRoomCreation : public CMenu {
private:
    HANDLE m_hDoor;                          /* 0x080 */
    HANDLE m_hSpotEffect;                    /* 0x084 */
    HANDLE m_hCrownEffect;                   /* 0x088 */
    FLOAT m_fMapColor;                       /* 0x08c */
    CPathCamera* m_pPathCamera;              /* 0x090 */
    INT m_nMenuStep;                         /* 0x094 */
    CGimmickDoor* m_pGimmickDoor;            /* 0x098 */
    RoomInfo* m_pRoomInfo;                   /* 0x09c */
    DoorInfo* m_pDoorInfo;                   /* 0x0a0 */
    INT m_doorStat;                          /* 0x0a4 */
    INT m_fgKeyCard;                         /* 0x0a8 */
    INT m_fgEventDoor;                       /* 0x0ac */
    C2DCard* m_pCardNorma[4];                /* 0x0b0 */
    C2DCard* m_pCardWinCard[30];             /* 0x0c0 */
    C2DCard* m_pCardSample;                  /* 0x138 */
    INT m_nNorma;                            /* 0x13c */
    INT m_idxNorma;                          /* 0x140 */
    D2::C2DSeqCtrl* m_pTitleCtrl;            /* 0x144 */
    D2::C2DSeqCtrl* m_pCurrentRoomNameCtrl;  /* 0x148 */
    D2::C2DSeqCtrl* m_pCardWinCtrl;          /* 0x14c */
    D2::C2DSeqCtrl* m_pCardWinHolder[6];     /* 0x150 */
    D2::C2DSeqCtrl* m_pCardWinInfo;          /* 0x168 */
    D2::C2DSeqCtrl* m_pCardWinInfoNum[10];   /* 0x16c */
    D2::C2DSeqCtrl* m_pCardNumCtrl;          /* 0x194 */
    D2::C2DSeqCtrl* m_pCardNumHolder[10];    /* 0x198 */
    D2::C2DSeqCtrl* m_pCursorCtrl;           /* 0x1c0 */
    D2::C2DSeqCtrl* m_pExplainCtrl[2];       /* 0x1c4 */
    D2::C2DSeqCtrl* m_pCardNormaCtrl[4];     /* 0x1cc */
    D2::C2DSeqCtrl* m_pCardSampleCtrl;       /* 0x1dc */
    D2::C2DFnt* m_pRoomNameFnt;              /* 0x1e0 */
    D2::C2DFnt* m_pCardVolFnt;               /* 0x1e4 */
    D2::C2DFnt* m_pExplainFnt[2];            /* 0x1e8 */
    D2::C2DFnt* m_pCardNumFnt[10];           /* 0x1f0 */
    D2::C2DFnt* m_pCurrentRoomNameFnt;       /* 0x218 */
    CStrPrim* m_pFntInstruction;             /* 0x21c */
    CScrollBar* m_pScrollBar;                /* 0x220 */
    INT m_fgEnd;                             /* 0x224 */
    INT m_nCardSelect;                       /* 0x228 */
    INT m_nCardSelectMax;                    /* 0x22c */
    INT m_nNumSelect;                        /* 0x230 */
    FVECTOR m_vPosCursorCurrent;             /* 0x240 */
    FVECTOR m_vPosCursorTarget;              /* 0x250 */
    FVECTOR m_vPosCursorOld;                 /* 0x260 */
    INT m_fgCursorMove;                      /* 0x270 */
    FLOAT m_fCursorTimer;                    /* 0x274 */
    CAttachCard* m_pAttachCard;              /* 0x278 */
    CARD m_card;                             /* 0x27c */
    INT m_fgLoadRCF;                         /* 0x280 */
    INT m_iObjType;                          /* 0x284 */
    INT m_nCategorySelect;                   /* 0x288 */
    char m_normaInfoBuf[256];                /* 0x28c */

    static INT m_iWorldRsrc;
    static INT m_iPlayerRsrc;
    static INT m_iEffectRsrc;

public:
    CRoomCreation(CTask* p, HANDLE hDoor);
    virtual ~CRoomCreation();
    void dispose();
    virtual void main();
    virtual void disp();
    static void removeRsrc();

private:
    INT menuMain();
    void initCard();
    INT initCardCheck(INT nCategorySelect);
    void selectDispCard();
    void initInfoNum(FVECTOR& vec);
    void selectDispInfoNum();
    void initNumHolder(FVECTOR& vec);
    void selectDispNumHolder();
    void moveCursor();
    void changeCursor(INT nSel, INT nIdx);
    INT searchNumExist(INT nMethod, INT idx);
    RCRoomInfo* getRoomInfo(USHORT kind);
    LPCTSTR getNormaInfo();
};

#endif /* FORM_MENU_ROOMCREATION_H */
