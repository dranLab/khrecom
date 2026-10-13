#ifndef FORM_MENU_MENUPARTS_H
#define FORM_MENU_MENUPARTS_H

#include "khtypes.h"
#include "System/Vector.h"
#include "System/Graphic/2d/2DFnt.h"

namespace D2 {
class C2DSeqCtrl;
}

class CMenuParts {
public:
    CMenuParts() {}
    virtual ~CMenuParts() {}
    virtual void enableDisp(BOOL bEnableDisp) {}
};

class CScrollBar : public CMenuParts {
private:
    D2::C2DSeqCtrl* m_pScrollBar[3]; /* 0x04 */
    FLOAT m_fBasicHeight;            /* 0x10 */
    FLOAT m_fBasicPoasY;             /* 0x14 */

public:
    CScrollBar();
    virtual ~CScrollBar();
    virtual void enableDisp(BOOL bEnableDisp);
    void init(const CHAR* pSeqName, USHORT nLayer, USHORT* pSeqTbl, FVECTOR& vPos, FLOAT fBasicHeight,
              D2::C2DSeqCtrl* pParentCtrl);
    void change(FLOAT fRatioPosY, FLOAT fRatioHeight);
};

class CButton : public CMenuParts {
public:
    enum BUTTON_SEQ {
        BS_NON = 0,
        BS_IN = 1,
        BS_LOOP = 2,
        BS_OUT = 3,
        BS_ACTIVE = 4,
        BS_NONACTIVE = 5,
        BS_DISPOFF = -1
    };

private:
    const CHAR* m_pSeqName;        /* 0x04 */
    const USHORT* m_pSeqTbl;   /* 0x08 */
    D2::C2DSeqCtrl* m_pButton; /* 0x0c */
    D2::C2DFnt* m_pFnt;        /* 0x10 */
    BUTTON_SEQ m_currentSeq;   /* 0x14 */

public:
    CButton();
    virtual ~CButton();
    virtual void enableDisp(BOOL bEnableDisp);
    void init(const CHAR* pSeqName, USHORT nLayer, const USHORT* pSeqTbl, FVECTOR& vPos, D2::C2DSeqCtrl* pParentCtrl,
              const CHAR* pButtonFont, INT iFontSize, FLOAT fShiftX, FLOAT fShiftY, INT iFontStyle, G_RGBA* pFontColor,
              SHORT nSeqIdx);
    void setActive(BOOL bActive);
    void setSequence(BUTTON_SEQ nButtonSeq, BOOL bDispOn);
    BOOL checkEndAfterSet(BUTTON_SEQ nButtonSeq);
    void changeFont(const CHAR* pButtonFont);
    void setFontSpace(SHORT x10, SHORT y10);
    D2::C2DSeqCtrl* get2DSeqCtrl() { return m_pButton; }
    const CHAR* getButtonFnt() { return m_pFnt->getText(); }
    D2::C2DFnt* get2DFnt() { return m_pFnt; }
};

#endif /* FORM_MENU_MENUPARTS_H */
