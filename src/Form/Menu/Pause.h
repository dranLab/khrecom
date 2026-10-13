#ifndef FORM_MENU_PAUSE_H
#define FORM_MENU_PAUSE_H

#include "khtypes.h"
#include "Form/Menu/Menu.h"
#include "System/Vector.h"
#include "Game/SceneEnv.h"

namespace D2 {
class C2DSeqCtrl;
class C2DFnt;
}

class CPause : public CMenu {
private:
    BOOL m_nPauseKind;               /* 0x80 */
    BOOL m_bEventSkip;               /* 0x84 */
    INT m_nSelect;                   /* 0x88 */
    INT m_nSelectMax;                /* 0x8c */
    INT m_fgCursorMove;              /* 0x90 */
    FLOAT m_fCursorTimer;            /* 0x94 */
    FLOAT m_fCursorBaseY;            /* 0x98 */
    HANDLE m_hHelpLay;               /* 0x9c */
    ObjType::eObjBit m_saveObjBit;   /* 0xa0 */
    D2::C2DSeqCtrl* m_pScreenCtrl;   /* 0xa8 */
    D2::C2DSeqCtrl* m_pSelectCtrl;   /* 0xac */
    D2::C2DSeqCtrl* m_pCursorCtrl;   /* 0xb0 */
    D2::C2DFnt* m_pSelectFnt[4];     /* 0xb4 */
    FVECTOR m_vPosCursorCurrent;     /* 0xd0 */
    FVECTOR m_vPosCursorTarget;      /* 0xe0 */
    FVECTOR m_vPosCursorOld;         /* 0xf0 */
    static UINT m_nResult;

public:
    CPause(CTask* p, INT nPauseKind);
    virtual ~CPause();
    void dispose();
    virtual void main();
    virtual void disp();
    static UINT getResult() { return m_nResult; }

private:
    void moveCursor();
};

#endif /* FORM_MENU_PAUSE_H */
