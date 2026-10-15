#ifndef FORM_MENU_LEVELUP_H
#define FORM_MENU_LEVELUP_H

#include "khtypes.h"
#include "Form/Menu/Menu.h"
#include "System/Vector.h"

namespace D2 {
class C2DSeqCtrl;
class C2DFnt;
}
class CMenuModel;

class CLevelUp : public CMenu {
private:
    D2::C2DSeqCtrl* m_pCursorCtrl;        /* 0x080 */
    D2::C2DSeqCtrl* m_pLightCtrl;         /* 0x084 */
    D2::C2DSeqCtrl* m_pWinInfoCtrl;       /* 0x088 */
    D2::C2DSeqCtrl* m_pWinAbCtrl[3];      /* 0x08c */
    D2::C2DFnt* m_pWinAbFnt[3][2];        /* 0x098 */
    D2::C2DSeqCtrl* m_pInfoCtrl[5];       /* 0x0b0 */
    D2::C2DSeqCtrl* m_pInfoFntCtrl[5][2]; /* 0x0c4 */
    D2::C2DFnt* m_pInfoFnt[5][2];         /* 0x0ec */
    D2::C2DSeqCtrl* m_pInfoSelCtrl;       /* 0x114 */
    D2::C2DSeqCtrl* m_pLevelCtrl;         /* 0x118 */
    D2::C2DFnt* m_pLevelFnt;              /* 0x11c */
    D2::C2DSeqCtrl* m_pUpMsgCtrl;         /* 0x120 */
    D2::C2DFnt* m_pUpMsgFnt;              /* 0x124 */
    D2::C2DSeqCtrl* m_pEffectCtrl;        /* 0x128 */
    D2::C2DSeqCtrl* m_pExplaneCtrl;       /* 0x12c */
    D2::C2DSeqCtrl* m_pUpEffectCtrl;      /* 0x130 */
    D2::C2DFnt* m_pExplaneFnt;            /* 0x134 */
    INT m_nStatusLad;                     /* 0x138 */
    INT m_nStatusInfo;                    /* 0x13c */
    INT m_nStatusAb;                      /* 0x140 */
    INT m_nStatusLvl;                     /* 0x144 */
    INT m_nStatusEffect;                  /* 0x148 */
    INT m_nStepStatusAb;                  /* 0x14c */
    INT m_nSelect;                        /* 0x150 */
    INT m_nSelectOld;                     /* 0x154 */
    INT m_fgCursorMove;                   /* 0x158 */
    FLOAT m_fLightTimer;                  /* 0x15c */
    BOOL m_bPlayerKind;                   /* 0x160 */
    BOOL m_bPlayerMode;                   /* 0x164 */
    int m_nAvailable[3];                  /* 0x168 */
    FLOAT m_fMenuModelAlpha;              /* 0x174 */
    INT m_nStatusUpMsg;                   /* 0x178 */
    LPCTSTR m_pTechName;                  /* 0x17c */
    INT m_nTechHelpKind;                  /* 0x180 */
    BOOL m_bAvailableSoraTech;            /* 0x184 */
    FLOAT m_fTimer;                       /* 0x188 */
    FVECTOR m_vPosLightCurrent;           /* 0x190 */
    FVECTOR m_vPosLightTarget;            /* 0x1a0 */
    FVECTOR m_vPosLightOld;               /* 0x1b0 */
    CMenuModel* m_pMenuModel[2];          /* 0x1c0 */
    static BOOL m_bEnd;

public:
    CLevelUp(CTask* p, USHORT nTechKind);
    virtual ~CLevelUp();
    void dispose();
    virtual void main();
    static BOOL isEnd() { return m_bEnd; }

private:
    INT setWinInfo(INT nSetCmd);
    INT setWinAb(INT nSetCmd);
    INT setWinLvl(INT nSetCmd);
    void moveCursor(FLOAT fFrameRate);
    INT setUpMsg(INT nSetCmd);
    INT setEffect(INT nSetCmd);
};

#endif /* FORM_MENU_LEVELUP_H */
