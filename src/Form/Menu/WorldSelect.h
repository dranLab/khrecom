#ifndef FORM_MENU_WORLDSELECT_H
#define FORM_MENU_WORLDSELECT_H

#include "khtypes.h"
#include "Form/Menu/Menu.h"
#include "System/Vector.h"

namespace D2 {
class C2DSeqCtrl;
class C2DFnt;
}
class C2DCard;
class CPathCamera;
class CGimmickDoor;
class CEntity;
struct RoomInfo;
struct DoorInfo;

class CWorldSelect : public CMenu {
private:
    HANDLE m_hDoor;                      /* 0x080 */
    CPathCamera* m_pPathCamera;          /* 0x084 */
    CGimmickDoor* m_pGimmickDoor;        /* 0x088 */
    RoomInfo* m_pRoomInfo;               /* 0x08c */
    DoorInfo* m_pDoorInfo;               /* 0x090 */
    CEntity* m_pWorldCard;               /* 0x094 */
    BOOL m_fgLoadRCF;                    /* 0x098 */
    HANDLE m_hSpotEffect;                /* 0x09c */
    INT m_fgEnd;                         /* 0x0a0 */
    INT m_nSelect;                       /* 0x0a4 */
    INT m_nSelectMax;                    /* 0x0a8 */
    INT m_nMenuStep;                     /* 0x0ac */
    INT m_iObjType;                      /* 0x0b0 */
    unsigned char m_nWorld[8];           /* 0x0b4 */
    INT m_nRotCurrent;                   /* 0x0bc */
    INT m_nRotOne;                       /* 0x0c0 */
    FLOAT m_fRotTarget;                  /* 0x0c4 */
    FLOAT m_fRotOld;                     /* 0x0c8 */
    FLOAT m_fRotCurrent;                 /* 0x0cc */
    INT m_fgRotate;                      /* 0x0d0 */
    FLOAT m_fTimer;                      /* 0x0d4 */
    BYTE m_nSeqNameWnd;                  /* 0x0d8 */
    C2DCard* m_pCard[5];                 /* 0x0dc */
    D2::C2DSeqCtrl* m_pCardCtrl[5];      /* 0x0f0 */
    D2::C2DSeqCtrl* m_pNameWndCtrl;      /* 0x104 */
    D2::C2DSeqCtrl* m_pIconCtrl;         /* 0x108 */
    D2::C2DSeqCtrl* m_pExplainCtrl;      /* 0x10c */
    D2::C2DFnt* m_pExplainFnt;           /* 0x110 */
    D2::C2DFnt* m_pNameWndFnt;           /* 0x114 */
    FVECTOR m_rotCircle;                 /* 0x120 */
    FVECTOR m_posLocal[5];               /* 0x130 */

public:
    CWorldSelect(CTask* p, HANDLE hDoor);
    virtual ~CWorldSelect();
    void dispose();
    virtual void main();
    INT menuMain();
};

#endif /* FORM_MENU_WORLDSELECT_H */
