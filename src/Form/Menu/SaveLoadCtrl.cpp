
#define FORM_MENU_SAVELOADCTRL_CPP /* MATCH: unit gate (its header gives this unit its own parts); without it the link fails (inlines this unit must emit) (tested 2026-11-02) */
#define GPRIM_H_PATH "System/Graphic/GPrim.h" /* MATCH: this unit names Gprim.h so (header default "System/Graphic/Gprim.h") */
#define SAVELOADCTRL_H_PATH "c:/KHCOM/MainApp/Form/Menu/SaveLoadCtrl.h" /* MATCH: this unit names SaveLoadCtrl.h so (header default "Form/Menu/SaveLoadCtrl.h") */
#include "System/Graphic/Gprim.h"
#include "System/Graphic/2d/2DMan.h"
#include "Form/Menu/SaveLoadCtrl.h"
#include "System/Graphic/2d/2DSeqCtrl.h"
#include "System/Graphic/2d/2DSeqObj.h"
#include "System/Graphic/2d/2DFnt.h"
#include "System/MainApp.h"
#include "System/Debug.h"
#include "System/HeapMan.h"



#define ASSERT(e) KH_DEBUG(if (!(e)) AssertApp(__FILE__, __LINE__, #e, 0))

extern INT SCREEN_HEIGHT;

template <class T> static inline T& _ref(T& v) { return v; }
extern "C" int sprintf(char* buf, const char* fmt, ...);

C2DSeqAgent::C2DSeqAgent(CTask* pParent)
    : CTask(pParent, 0x8000), m_pszLad(NULL), m_pszSqd(NULL), m_hSeqSingle(0), m_pSeqGroup(NULL), m_nSeqUnit(-1),
      m_nSeqGroupTarget(-1), m_pcbEndIN(NULL), m_pcbEndOUT(NULL), m_nState(_STATE_NOT_START), m_nCurrentSeq(-1),
      m_nLayer(11) {
    for (INT i = 0; i < 3; i++) m_SeqTbl._tbl[i] = -1;
}

void C2DSeqAgent::dispose() {
    if (m_hSeqSingle) D2::C2DMan::destroy(m_hSeqSingle);
    if (m_pSeqGroup) m_pSeqGroup->enableDisp(FALSE);
    m_pszLad = NULL;
    m_hSeqSingle = 0;
    m_pSeqGroup = NULL;
    m_nSeqGroupTarget = -1;
    m_nSeqUnit = -1;
}

void C2DSeqAgent::setupSeq(D2::C2DSeqCtrl& seq, const CHAR* pszLad, SHORT nUnit, SHORT nTargetIdx) {
    dispose();
    m_nSeqGroupTarget = nTargetIdx;
    m_nSeqUnit = nUnit;
    m_pszLad = (CHAR*)pszLad;
    m_pSeqGroup = &seq;
    m_pszSqd = NULL;
    seq.enableDisp(FALSE);
}

void C2DSeqAgent::setupSeq(D2::C2DSeqCtrl& seq, const CHAR* pszSqd, SHORT nTargetIdx) {
    dispose();
    m_nSeqGroupTarget = nTargetIdx;
    m_pszSqd = (CHAR*)pszSqd;
    m_pSeqGroup = &seq;
    m_nSeqUnit = 0;
    m_pszLad = NULL;
    seq.enableDisp(FALSE);
}

void C2DSeqAgent::setupState(SHORT nIN, SHORT nLOOP, SHORT nOUT, void (*cbEndIN)(C2DSeqAgent&),
                             void (*cbEndOUT)(C2DSeqAgent&)) {
    m_SeqTbl._tbl[0] = nIN;
    m_SeqTbl._tbl[1] = nLOOP;
    m_SeqTbl._tbl[2] = nOUT;
    m_pcbEndIN = cbEndIN;
    m_pcbEndOUT = cbEndOUT;
}

void C2DSeqAgent::setupState(SHORT nIN, SHORT nLOOP, SHORT nOUT) {
    m_SeqTbl._tbl[0] = nIN;
    m_SeqTbl._tbl[1] = nLOOP;
    m_SeqTbl._tbl[2] = nOUT;
}

void C2DSeqAgent::start() {
    INT s;

    for (s = 0; s < 3; s++) {
        if (m_SeqTbl._tbl[s] >= 0) break;
    }
    if (s >= 3) {
        TRACE("C2DSeqAgent : \x83\x58\x83\x65\x81\x5b\x83\x67\x96\xa2\x93\x6f\x98\x5e\x82\xc5\x82\xb7\n");
        return;
    }
    m_nState = (STATE)s;
    SHORT nSeqID = m_SeqTbl._tbl[s];
    if (m_pSeqGroup) {
        m_pSeqGroup->enableDisp(TRUE);
        return;
    }
    if (m_pszLad) {
        if (m_nLayer != 11) D2::C2DMan::setLayer(m_nLayer);
        m_hSeqSingle = D2::C2DMan::create(m_pszLad, nSeqID);
        if (m_nLayer != 11) D2::C2DMan::setLayer(11);
    } else {
        TRACE("C2DSeqAgent : \x83\x56\x81\x5b\x83\x50\x83\x93\x83\x58\x82\xaa\x90\xdd\x92\xe8\x82\xb3\x82\xea\x82\xc4\x82\xa2\x82\xdc\x82\xb9\x82\xf1\n");
    }
}

void C2DSeqAgent::main() {
    BOOL bEnd;
    INT s;

    if (m_nState >= _STATE_MAX) return;
    if (m_nState == STATE_LOOP) {
        procLoop();
        return;
    }
    if (m_pSeqGroup) {
        bEnd = m_pSeqGroup->isEnd(m_nSeqGroupTarget);
    } else {
        bEnd = D2::C2DMan::isEnd(m_hSeqSingle);
    }
    if (!bEnd) return;
    for (s = m_nState + 1; s < 3; s++) {
        if (m_SeqTbl._tbl[s] > 0) break;
    }
    if (s >= 3) {
        outSeq();
    } else {
        changeSeq(m_SeqTbl._tbl[s]);
    }
    switch (m_nState) {
    case STATE_IN:
        if (m_pcbEndIN) m_pcbEndIN(*this);
        break;
    case STATE_OUT:
        if (m_pcbEndOUT) m_pcbEndOUT(*this);
        break;
    }
    m_nState = (STATE)s;
}

void C2DSeqAgent::changeState(STATE nState, BOOL bSkipIfSame, SHORT nExtSeq) {
    BOOL bDispPrev;

    if (nState == STATE_OUT && m_nState >= STATE_OUT) return;
    if (bSkipIfSame && nState == m_nState) return;
    bDispPrev = isSeqCtrlVisible();
    if (nExtSeq < 0) {
        changeSeq(m_SeqTbl._tbl[nState]);
    } else {
        changeSeq(nExtSeq);
    }
    if (nState == STATE_OUT && !bDispPrev && m_pSeqGroup) m_pSeqGroup->enableDisp(FALSE);
    m_nState = nState;
}

void C2DSeqAgent::changeSeq(SHORT nSeqID) {
    if (nSeqID < 0) return;
    if (m_pSeqGroup) {
        if (m_pszSqd) {
            m_pSeqGroup->replaceSeq(m_nSeqGroupTarget, m_pszSqd, nSeqID);
        } else {
            m_pSeqGroup->replaceSeq(m_nSeqGroupTarget, m_pszLad, m_nSeqUnit, nSeqID);
        }
        m_pSeqGroup->enableDisp(TRUE);
    } else {
        if (m_hSeqSingle) D2::C2DMan::destroy(m_hSeqSingle);
        if (m_nLayer != 11) D2::C2DMan::setLayer(m_nLayer);
        m_hSeqSingle = D2::C2DMan::create(m_pszLad, nSeqID);
        if (m_nLayer != 11) D2::C2DMan::setLayer(11);
    }
    m_nCurrentSeq = nSeqID;
}

void C2DSeqAgent::outSeq() {
    if (m_hSeqSingle) D2::C2DMan::destroy(m_hSeqSingle);
    m_hSeqSingle = 0;
    if (m_pSeqGroup) m_pSeqGroup->enableDisp(FALSE);
    onOutSeq();
}

void C2DSeqCursorCtrl::setupSeq(const CHAR* pszLad, SHORT nUnit, SHORT nSeqNormal, SHORT nSeqHistory) {
    m_nSeqUnit = nUnit;
    m_nSeqTbl[1] = nSeqHistory;
    m_pszLad = (CHAR*)pszLad;
    m_nSeqTbl[0] = nSeqNormal;
    addSeq(pszLad, 0, nSeqNormal);
    enableDisp(FALSE);
}

void C2DSeqLightMarkAgent::setupSeq(SeqTbl stbl, const CHAR* pszLad, SHORT nUnit, SHORT nLayer, BOOL bFollow) {
#line 264
    D2::C2DSeqCtrl* pCtrl = new KH_AT("(264)") D2::C2DSeqCtrl;
    pCtrl->addSeq(pszLad, nUnit, stbl._tbl[0]);
    pCtrl->changeLayer(nLayer);
    pCtrl->enableColorCtrl(TRUE);
    C2DSeqAgent::setupSeq(*pCtrl, pszLad, nUnit, 0);
    m_pCtrl = pCtrl;
    if (bFollow) m_nFlag |= 1; else m_nFlag &= ~1;
}

void C2DSeqLightMarkAgent::setPos(FVECTOR& vPos) {
    if (!m_pCtrl) return;
    if ((m_nFlag & 1) == 0 || m_nMoveTime < 0 || getState() >= STATE_OUT) {
        m_pCtrl->setPos(vPos);
        if (m_nFlag & 1) {
            m_vBasePos.set(vPos);
            m_nMoveTime = 0;
        }
    } else {
        m_nMoveTime = 1500;
        m_vTargetPos.set(vPos);
        m_vBasePos.set(m_pCtrl->m_seqControl.vPos);
        m_vBasePos.x += 256.0f;
        m_vBasePos.y += _ref(SCREEN_HEIGHT) >> 1;
    }
}

void C2DSeqLightMarkAgent::updatePos() {
    if (m_nMoveTime <= 0) return;
    m_nMoveTime -= getFrameRate();
    if (m_nMoveTime < 0) m_nMoveTime = 0;
    if (m_pCtrl) {
        FLOAT fRate = (FLOAT)m_nMoveTime / 1500.0f;
        FVECTOR v;
        InterVectorXYZ(v, m_vBasePos, m_vTargetPos, fRate * fRate);
        m_pCtrl->setPos(v);
    }
}

void C2DSeqSelectable::setupSeq(SeqTbl stbl, const CHAR* pszLad, SHORT nUnit, SHORT nActiveSeq, SHORT nLayer) {
#line 338
    D2::C2DSeqCtrl* pCtrl = new KH_AT("(338)") D2::C2DSeqCtrl;
    pCtrl->addSeq(pszLad, nUnit, stbl._tbl[0]);
    pCtrl->changeLayer(nLayer);
    pCtrl->enableColorCtrl(TRUE);
#line 343
    m_pCaption = new KH_AT("(343)") D2::C2DFnt(0, 0, NULL, 16, NULL, 0);
    m_pCaption->setStyle(0x40);
    pCtrl->addFnt(pszLad, nUnit, m_pCaption);
    C2DSeqAgent::setupSeq(*pCtrl, pszLad, nUnit, 0);
    m_pCtrl = pCtrl;
    m_nActiveLoopSeq = nActiveSeq;
}

void C2DSeqSelectable::select(BOOL bSel) {
    FVECTOR vPos;

    if (bSel) {
        changeState(STATE_LOOP, FALSE, m_nActiveLoopSeq);
        if (m_pCursor) {
            getCursorPos(vPos);
            m_pCursor->setPos(vPos);
            m_pCursor->enableDisp(TRUE);
        }
        if (m_pLightMark) {
            getLightMarkPos(vPos);
            m_pLightMark->setPos(vPos);
            C2DSeqAgent* pAgent = m_pLightMark;
            if ((UINT)pAgent->getState() >= STATE_OUT) pAgent->startOrRestart();
        }
    } else {
        changeState(STATE_LOOP, FALSE, -1);
    }
}

void C2DSeqSelectable::setCaptionParam(const CHAR* pszText, INT nSize, const G_RGBA* pCol, UINT nStyle) {
    m_pCaption->setText(pszText);
    m_pCaption->setSize(nSize);
    m_pCaption->setStyle(nStyle);
    if (pCol) m_pCaption->setColor(*pCol);
}

void C2DSeqSelectable::getCursorPos(FVECTOR& vOut) const {
    if (m_pCtrl) {
        vOut.set(m_pCtrl->m_seqControl.vPos);
        vOut.x += m_fptCursorOfs.x + 256.0f;
        vOut.y += m_fptCursorOfs.y + (_ref(SCREEN_HEIGHT) >> 1);
    }
}

void C2DSeqSelectable::getLightMarkPos(FVECTOR& vOut) const {
    if (m_pCtrl) {
        vOut.set(m_pCtrl->m_seqControl.vPos);
        vOut.x += m_fptLightOfs.x + 256.0f;
        vOut.y += m_fptLightOfs.y + (_ref(SCREEN_HEIGHT) >> 1);
    }
}

void C2DSeqScrollbarCtrl::setupSeq(D2::C2DSeqCtrl* pParent, const C2DSeqAgent::SeqTbl* pTbl, const CHAR* pszLad,
                                   SHORT nUnit, SHORT nLayer, SHORT nBodySize) {
    INT i;

    m_pParentCtrl = pParent;
#line 441
    ASSERT(pTbl);
    for (i = 0; i < 3; i++) {
#line 444
        D2::C2DSeqCtrl* pCtrl = new KH_AT("(444)") D2::C2DSeqCtrl;
        pCtrl->addSeq(pszLad, nUnit, pTbl[i]._tbl[0]);
        pCtrl->changeLayer(nLayer);
        pCtrl->enableColorCtrl(TRUE);
        m_pCtrls[i] = pCtrl;
        C2DSeqAgent* pAgent = m_pParts[i];
#line 451
        ASSERT(pAgent);
        pAgent->setupSeq(*pCtrl, pszLad, nUnit, 0);
        pAgent->setupState(pTbl[i]);
        m_pParts[i] = pAgent;
        if (pParent) pCtrl->setParent(pParent);
    }
    m_fBaseBodySize = nBodySize;
}

void C2DSeqScrollbarCtrl::setBasePos(FVECTOR& vPos) {
    D2::C2DSeqCtrl* pHead = m_pCtrls[0];

    if (pHead) {
        if (m_pParentCtrl) {
            pHead->setLocalPos(vPos);
        } else {
            pHead->setPos(vPos);
        }
    }
    m_fBasePosY = vPos.y;
    updateForm();
}

void C2DSeqScrollbarCtrl::setScrollRange(FLOAT fMaxBodySize, USHORT nItemCount, USHORT nDispCount, BOOL bUpdateForm) {
    m_fMaxBodySize = fMaxBodySize;
    m_nAllItemCount = nItemCount;
    if (!nItemCount) m_nAllItemCount = 1;
    m_nDispItemCount = nDispCount;
    if (!nDispCount) m_nDispItemCount = 1;
    m_fBodyRatio = m_nAllItemCount < m_nDispItemCount ? 1.0f : (FLOAT)m_nDispItemCount / (FLOAT)m_nAllItemCount;
    if (bUpdateForm) updateForm();
}

void C2DSeqScrollbarCtrl::setScrollPos(USHORT nPos, BOOL bUpdateForm) {
    if (m_nAllItemCount <= m_nDispItemCount) {
        m_fScrollPos = 0.0f;
    } else {
        USHORT nMax = m_nAllItemCount - m_nDispItemCount;
        if (nPos > nMax) nPos = nMax;
        m_fScrollPos = (FLOAT)nPos / (FLOAT)nMax;
    }
    if (bUpdateForm) updateForm();
}

static inline FLOAT _round1(FLOAT _y) {
    INT n = (INT)((_y + 0.5f) * 10.0f);
    return n / 10;
}

void C2DSeqScrollbarCtrl::updateForm() {
    FLOAT fBodySize;
    FLOAT fBodyScale;
    FVECTOR v;

    fBodySize = _round1(m_fMaxBodySize * m_fBodyRatio);
    fBodyScale = fBodySize / m_fBaseBodySize;
    v.set(1.0f, fBodyScale, 1.0f, 0.0f);



    m_pCtrls[1]->setScl(v);
    if (m_pCtrls[0]) {
        FLOAT fMovable = m_fMaxBodySize - fBodySize;
        FLOAT y = _round1(fMovable * m_fScrollPos);
        v.set(m_pCtrls[0]->m_seqControl.vPos);
        v.y = m_fBasePosY + y;
        m_pCtrls[0]->setLocalPos(v);
        m_pCtrls[1]->setLocalPos(v);
        v.y += fBodySize;
        m_pCtrls[2]->setLocalPos(v);
    }
}

void C2DSeqTimeCtrl::setupSeq(const CHAR* pszLad, SHORT nUnit, SHORT nSeqNum0, SHORT nSeqColon, SHORT nSeqBlank) {
    m_pszLad = (CHAR*)pszLad;
    m_nSeqUnit = nUnit;
    m_nSeqNum0 = nSeqNum0;
    m_nSeqColon = nSeqColon;
    m_nSeqBlank = nSeqBlank;
    m_nState = 0;
    enableDisp(FALSE);
}

void C2DSeqTimeCtrl::show() {
    switch (m_nState) {
    case 0:
        m_nState = 1;
        break;
    case 3:
        m_nState = 2;
        break;
    default:
        return;
    }
    update();
    enableDisp(TRUE);
}

BOOL C2DSeqTimeCtrl::update() {
    BOOL bUpdate;
    HM hm;
    char szTime[8];
    CHAR* p;
    FLOAT fX;
    SHORT nSeq;
    INT i;

    if (m_nState != 1 && m_nState != 2) return FALSE;
    bUpdate = FALSE;
    hm.setTimeSec(CMainApp::getPlayTime());
    if (m_nState == 1 || hm._nValue != m_Time._nValue) {
        m_Time._nValue = hm._nValue;
        sprintf(szTime, "%3d:%02d", m_Time._nHour, m_Time._nMinute);
        bUpdate = TRUE;
        fX = 0.0f;
        for (i = 0, p = szTime; i < 6; i++, p++) {
            FLOAT fDx = 0.0f;
            BOOL bNum = FALSE;
            switch (*p) {
            case ' ':
                nSeq = m_nSeqBlank;
                break;
            case ':':
                fDx = -8.0f;
                nSeq = m_nSeqColon;
                break;
            default:
                bNum = TRUE;
                nSeq = *p - '0' + m_nSeqNum0;
                break;
            }
            fX += fDx;
            if (m_nState == 1) {
                addSeq(m_pszLad, m_nSeqUnit, nSeq)->shiftBase(fX, 0.0f);
            } else if (bNum) {
                replaceSeqShift(i, m_pszLad, m_nSeqUnit, nSeq, fX, 0.0f);
            }
            fX += 16.0f;
        }
        if (m_nState == 1) m_nState = 2;
    }
    return bUpdate;
}

void C2DSeqNumericCtrl::setupSeq(const CHAR* pszLad, SHORT nUnit, SHORT nSeqNum0, SHORT nSeqBlank, UINT nMaxValue) {
    m_pszLad = (CHAR*)pszLad;
    m_nSeqUnit = nUnit;
    m_nSeqNum0 = nSeqNum0;
    m_nSeqBlank = nSeqBlank;
    enableDisp(FALSE);
    m_nState = 0;
    m_nMaxValue = nMaxValue;
    m_nMaxFigNum = 0;
    do {
        m_nMaxFigNum++;
        nMaxValue /= 10;
    } while (nMaxValue);
}

void C2DSeqNumericCtrl::show() {
    switch (m_nState) {
    case 0:
        m_nState = 1;
        break;
    case 3:
        m_nState = 2;
        break;
    default:
        return;
    }
    updateValue(m_nNewValue);
    enableDisp(TRUE);
}

BOOL C2DSeqNumericCtrl::updateValue(UINT nValue) {
    BOOL bUpdate;
    UINT nNewValue;
    char szForm[8];
    char szValue[16];
    CHAR* p;
    INT i;

    m_nNewValue = nValue;
    if (m_nState != 1 && m_nState != 2) return FALSE;
    bUpdate = FALSE;
    nNewValue = nValue;
    if (m_nMaxValue < nNewValue) nNewValue = m_nMaxValue;
    if (m_nState == 1 || nNewValue != m_nValue) {
        m_nValue = nNewValue;
        sprintf(szForm, "%%%dd", m_nMaxFigNum);
        bUpdate = TRUE;
        sprintf(szValue, szForm, m_nValue);
        for (i = 0, p = szValue; i < m_nMaxFigNum; i++, p++) {
            BOOL bNum;
            SHORT nSeq;
            if (*p == ' ') {
                nSeq = m_nSeqBlank;
                bNum = FALSE;
            } else {
                bNum = TRUE;
                nSeq = *p - '0' + m_nSeqNum0;
            }
            FLOAT fX = i * 16;
            if (m_nState == 1) {
                addSeqShift(m_pszLad, m_nSeqUnit, nSeq, fX, 0.0f);
            } else if (bNum) {
                replaceSeqShift(i, m_pszLad, m_nSeqUnit, nSeq, fX, 0.0f);
            }
        }
        if (m_nState == 1) m_nState = 2;
    }
    return bUpdate;
}

// SOURCE DATA: .rodata
// SOURCE DATA SLUS_217.99_retail: .rodata
