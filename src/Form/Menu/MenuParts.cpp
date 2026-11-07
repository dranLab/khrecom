#define GPRIM_H_PATH "System/Graphic/GPrim.h" /* MATCH: this unit names Gprim.h so (header default "System/Graphic/Gprim.h") */
#include "System/Graphic/Gprim.h"
#include "System/Graphic/2d/2DMan.h"
#include "Form/Menu/MenuParts.h"
#include "System/HeapMan.h"
#include "System/Vector.h"
#include "System/Graphic/2d/2DSeqCtrl.h"
#include "System/Graphic/2d/2DFnt.h"

inline void* operator new(size_t, void* p) throw() { return p; }



using namespace D2;

CScrollBar::CScrollBar() {
    m_pScrollBar[2] = NULL;
    m_pScrollBar[1] = NULL;
    m_pScrollBar[0] = NULL;
}

CScrollBar::~CScrollBar() {
    for (INT i = 0; i < 3; i++) {
        delete m_pScrollBar[i];
        m_pScrollBar[i] = NULL;
    }
}

void CScrollBar::enableDisp(BOOL bEnableDisp) {
    if (m_pScrollBar[0]) {
        for (INT i = 0; i < 3; i++) {
            m_pScrollBar[i]->enableDisp(bEnableDisp);
        }
    }
}

void CScrollBar::init(const CHAR* pSeqName, USHORT nLayer, USHORT* pSeqTbl, FVECTOR& vPos, FLOAT fBasicHeight,
                      C2DSeqCtrl* pParentCtrl) {
    FVECTOR vec;
    m_fBasicHeight = fBasicHeight;
    m_fBasicPoasY = vPos.y;
    for (INT i = 0; i < 3; i++) {
#line 58
        C2DSeqCtrl* pCtrl = m_pScrollBar[i] = new (__FILE__ "(58)") C2DSeqCtrl;
        pCtrl->setLayer(nLayer);
        pCtrl->enableColorCtrl(TRUE);
        pCtrl->addSeq((LPCTSTR)pSeqName, pSeqTbl[i]);
        if (pParentCtrl) {
            pCtrl->setParent(pParentCtrl);
        }
        pCtrl->setLocalPos(vPos);
        if (i == 1) {
            vec.x = 1.0f;
            vec.y = fBasicHeight * 0.01f;
            vec.z = 1.0f;
            vec.w = 0.0f;
            pCtrl->setScl(vec);
        } else if (i == 2) {
            vec = vPos;
            vec.y += fBasicHeight - 2.0f;
            pCtrl->setLocalPos(vec);
        }
    }
}

void CScrollBar::change(FLOAT fRatioPosY, FLOAT fRatioHeight) {
    C2DSeqCtrl* pTmpCtrl;
    FVECTOR vPos;
    FVECTOR vec;
    if (m_pScrollBar[0]) {
        vPos = m_pScrollBar[0]->m_seqControl.vPos;
        vPos.y = m_fBasicPoasY + m_fBasicHeight * fRatioPosY;
        for (INT i = 0; i < 3; i++) {
            pTmpCtrl = m_pScrollBar[i];
            pTmpCtrl->setLocalPos(vPos);
            if (i == 1) {
                vec.set(1.0f, m_fBasicHeight * 0.01f * fRatioHeight, 1.0f, 0.0f);
                pTmpCtrl->setScl(vec);
            } else if (i == 2) {
                vPos.y += m_fBasicHeight * fRatioHeight - 2.0f;
                pTmpCtrl->setLocalPos(vPos);
            }
        }
    }
}

CButton::CButton() {
    m_pButton = NULL;
}

CButton::~CButton() {
    delete m_pButton;
    m_pButton = NULL;
}

void CButton::enableDisp(BOOL bEnableDisp) {
    if (m_pButton) {
        m_pButton->enableDisp(bEnableDisp);
    }
}

void CButton::init(const CHAR* pSeqName, USHORT nLayer, const USHORT* pSeqTbl, FVECTOR& vPos, C2DSeqCtrl* pParentCtrl,
                   const CHAR* pButtonFont, INT iFontSize, FLOAT fShiftX, FLOAT fShiftY, INT iFontStyle,
                   G_RGBA* pFontColor, SHORT nSeqIdx) {
    FVECTOR vec;
    G_RGBA rgba;
    m_currentSeq = BS_LOOP;
    m_pSeqName = pSeqName;
    m_pSeqTbl = pSeqTbl;
#line 161
    (m_pButton = new (__FILE__ "(161)") C2DSeqCtrl)->setLayer(nLayer);
    m_pButton->enableColorCtrl(TRUE);
    m_currentSeq = (BUTTON_SEQ)(nSeqIdx < 0 ? 1 : nSeqIdx + 1);
    m_pButton->addSeq((LPCTSTR)pSeqName, pSeqTbl[m_currentSeq - 1]);
    if (pParentCtrl) {
        m_pButton->setParent(pParentCtrl);
    }
    m_pButton->setLocalPos(vPos);
    if (pButtonFont) {
        if (pFontColor) {
            rgba = *pFontColor;
        } else {
            rgba.c = 0x807C7C7C;
        }
#line 179
        m_pFnt = new (__FILE__ "(179)") C2DFnt(0, 0, &rgba, iFontSize, NULL, iFontStyle);
        m_pFnt->allocText(pButtonFont);
        m_pButton->addFntShift((LPCTSTR)pSeqName, m_pFnt, fShiftX, fShiftY);
    }
}

void CButton::setActive(BOOL bActive) {
    if (m_pButton) {
        m_currentSeq = bActive ? BS_ACTIVE : BS_NONACTIVE;
        m_pButton->replaceSeq(0, (LPCTSTR)m_pSeqName, m_pSeqTbl[m_currentSeq - 1]);
    }
}

void CButton::setSequence(BUTTON_SEQ nButtonSeq, BOOL bDispOn) {
    if (m_pButton) {
        m_currentSeq = nButtonSeq;
        if (nButtonSeq > BS_NON) {
            m_pButton->replaceSeq(0, (LPCTSTR)m_pSeqName, m_pSeqTbl[nButtonSeq - 1]);
            if (bDispOn) {
                m_pButton->enableDisp(TRUE);
            }
        }
    }
}

BOOL CButton::checkEndAfterSet(BUTTON_SEQ nButtonSeq) {
    if (!m_pButton) {
        return FALSE;
    }
    if (m_currentSeq == nButtonSeq) {
        return TRUE;
    }
    if (m_pButton->isEnd(0)) {
        m_currentSeq = nButtonSeq;
        if (nButtonSeq == BS_NON) {
            return TRUE;
        }
        if (nButtonSeq > BS_NON) {
            m_pButton->replaceSeq(0, (LPCTSTR)m_pSeqName, m_pSeqTbl[nButtonSeq - 1]);
        } else {
            m_pButton->enableDisp(FALSE);
        }
        return TRUE;
    }
    return FALSE;
}

void CButton::changeFont(const CHAR* pButtonFont) {
    if (m_pFnt) {
        m_pFnt->allocText(pButtonFont);
    }
}

void CButton::setFontSpace(SHORT x10, SHORT y10) {
    if (m_pFnt) {
        m_pFnt->setSpace(x10, y10);
    }
}

// SOURCE DATA: .rodata
