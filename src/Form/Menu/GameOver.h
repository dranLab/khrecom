#ifndef FORM_MENU_GAMEOVER_H
#define FORM_MENU_GAMEOVER_H

#include "khtypes.h"
#include "Form/Menu/Menu.h"
#include "System/Vector.h"

namespace D2 {
class C2DSeqCtrl;
class C2DFnt;
}

class CGameOver : public CMenu {
private:
    INT m_nSelect;                     /* 0x80 */
    INT m_fgLightMove;                 /* 0x84 */
    FLOAT m_fLightTimer;               /* 0x88 */
    BOOL m_bHeartless;                 /* 0x8c */
    D2::C2DSeqCtrl* m_pScreenCtrl;     /* 0x90 */
    D2::C2DSeqCtrl* m_pSelectCtrl[2];  /* 0x94 */
    D2::C2DSeqCtrl* m_pLightCtrl;      /* 0x9c */
    D2::C2DSeqCtrl* m_pCursorCtrl;     /* 0xa0 */
    D2::C2DFnt* m_pSelectFnt[2];       /* 0xa4 */
    FVECTOR m_vPosLightCurrent;        /* 0xb0 */
    FVECTOR m_vPosLightTarget;         /* 0xc0 */
    FVECTOR m_vPosLightOld;            /* 0xd0 */

public:
    CGameOver(CTask* p, BOOL bHeartless);
    virtual ~CGameOver();
    void dispose();
    virtual void main();
    virtual void disp();

private:
    void moveCursor();
};

#endif /* FORM_MENU_GAMEOVER_H */
