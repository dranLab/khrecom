#ifndef FORM_MENU_ROULETTE_H
#define FORM_MENU_ROULETTE_H

#include "khtypes.h"
#include "Form/Menu/Menu.h"

namespace D2 {
class C2DSeqCtrl;
class C2DFnt;
}
class C2DCard;
class CButton;

class CRoulette : public CMenu {
private:
    D2::C2DSeqCtrl* m_pCardInfoCtrl;  /* 0x080 */
    D2::C2DSeqCtrl* m_pBaseCtrl;      /* 0x084 */
    D2::C2DSeqCtrl* m_pCursorCtrl;    /* 0x088 */
    D2::C2DSeqCtrl* m_pBaloonCtrl;    /* 0x08c */
    D2::C2DSeqCtrl* m_pEffectCtrl;    /* 0x090 */
    D2::C2DSeqCtrl* m_pCardGetCtrl;   /* 0x094 */
    D2::C2DSeqCtrl* m_pGetCardCtrl;   /* 0x098 */
    D2::C2DFnt* m_pGetCardFnt[2];     /* 0x09c */
    D2::C2DSeqCtrl* m_pCardCtrl[11];  /* 0x0a4 */
    C2DCard* m_pCard[99];             /* 0x0d0 */
    CButton* m_pNumButton[30];        /* 0x25c */
    CButton* m_pHelpButton;           /* 0x2d4 */
    INT m_nStatusLad;                 /* 0x2d8 */
    INT m_nMenuStep;                  /* 0x2dc */
    INT m_fgRoulette;                 /* 0x2e0 */
    INT m_nCardMax;                   /* 0x2e4 */
    INT m_nCardDummyMax;              /* 0x2e8 */
    INT m_fgRotate;                   /* 0x2ec */
    INT m_idxStart;                   /* 0x2f0 */
    INT m_fgReplace;                  /* 0x2f4 */
    INT m_fgSelect;                   /* 0x2f8 */
    INT m_idxGetCard;                 /* 0x2fc */
    INT m_nSelect;                    /* 0x300 */
    FLOAT m_fRouletteRotY;            /* 0x304 */
    FLOAT m_fRouletteRadius;          /* 0x308 */
    FLOAT m_fSpeed;                   /* 0x30c */
    FLOAT m_fTimer;                   /* 0x310 */
    unsigned char m_idxDeck[99];      /* 0x314 */
    unsigned char m_nMapCard[30];     /* 0x377 */

public:
    CRoulette(CTask* p, BOOL fgRoulette);
    virtual ~CRoulette();

private:
    void dispose();
    virtual void main();
    INT menuMain();
    INT setCardInfo(INT nCmd);
};

#endif /* FORM_MENU_ROULETTE_H */
