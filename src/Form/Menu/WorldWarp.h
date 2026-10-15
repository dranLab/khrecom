#ifndef FORM_MENU_WORLDWARP_H
#define FORM_MENU_WORLDWARP_H

#include "khtypes.h"
#include "System/Vector.h"
#include "Form/Menu/Menu.h"

namespace D2 {
class C2DSeqCtrl;
class C2DFnt;
}
class C2DCard;
class CButton;

class CWorldWarp : public CMenu {
private:
    INT m_nStatusLad;                 /* 0x080 */
    INT m_nMenuStep;                  /* 0x084 */
    BOOL m_bWarpOrCard;               /* 0x088 */
    BOOL m_bPlayerKind;               /* 0x08c */
    INT m_nSelect;                    /* 0x090 */
    INT m_idxSrcWorld;                /* 0x094 */
    INT m_idxMaxWorld;                /* 0x098 */
    INT m_idxDistWorld;               /* 0x09c */
    INT m_idxDistWorldOld;            /* 0x0a0 */
    int m_fgLightMove[2];             /* 0x0a4 */
    float m_fLightTimer[2];           /* 0x0ac */
    INT m_nSelYesNo;                  /* 0x0b4 */
    INT m_nMaxFloor;                  /* 0x0b8 */
    INT m_nMaxFloorWorld;             /* 0x0bc */
    BOOL m_bPooClear;                 /* 0x0c0 */
    INT m_nCardLevel;                 /* 0x0c4 */
    D2::C2DSeqCtrl* m_pCursorCtrl[2]; /* 0x0c8 */
    D2::C2DSeqCtrl* m_pLightCtrl[2];  /* 0x0d0 */
    D2::C2DSeqCtrl* m_pRouteCtrl[12]; /* 0x0d8 */
    D2::C2DSeqCtrl* m_pCardCtrl[13];  /* 0x108 */
    D2::C2DSeqCtrl* m_pCardSrcCtrl;   /* 0x13c */
    D2::C2DSeqCtrl* m_pCardDistCtrl;  /* 0x140 */
    D2::C2DFnt* m_pCardSrcFnt[2];     /* 0x144 */
    D2::C2DFnt* m_pCardDistFnt[2];    /* 0x14c */
    CButton* m_pWinConfirm;           /* 0x154 */
    CButton* m_pButtonYenNo[2];       /* 0x158 */
    C2DCard* m_pWorldCard;            /* 0x160 */
    CButton* m_pHelpButton;           /* 0x164 */
    char m_idxWorld[16];              /* 0x168 */
    char m_idxIcon[16];               /* 0x178 */
    FVECTOR m_vPosLightCurrent[2];    /* 0x190 */
    FVECTOR m_vPosLightTarget[2];     /* 0x1b0 */
    FVECTOR m_vPosLightOld[2];        /* 0x1d0 */

public:
    CWorldWarp(CTask* p, BOOL bWarpOrCard);
    virtual ~CWorldWarp();

private:
    void dispose();
    virtual void main();
    INT menuMain();
    void moveCursorAndLight(INT id, FLOAT fFrameRate);
    BOOL searchSelect(INT nAdd);
    INT changeWorldIdx(INT nSelect, BOOL bChangeOnly);
    void initWorldIdx();
};

#endif /* FORM_MENU_WORLDWARP_H */
