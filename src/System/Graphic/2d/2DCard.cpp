
#define GPRIM_H_PATH "System/Graphic/gprim.h" /* MATCH: this unit names Gprim.h so (header default "System/Graphic/Gprim.h") */
#define D2MAN_H_PATH "c:/KHCOM/MainApp/System/Graphic/2d/2DMan.h" /* MATCH: this unit names 2DMan.h so (header default "System/Graphic/2D/2DMan.h") */
#include "System/Graphic/Gprim.h"
#include "System/Graphic/2d/2DMan.h"
#include "System/Graphic/2d/2DCard.h"
#include "System/Graphic/2d/2DSeqObj.h"
#include "System/Vector.h"
#include "include_asm.h"

extern "C" int sprintf(char* buf, const char* fmt, ...);
void ApplyMatrix(FVECTOR& v0, const FMATRIX& m, const FVECTOR& v1);
FLOAT InnerProduct(const FVECTOR& v0, const FVECTOR& v1);












using namespace D2;

#line 256

LPCTSTR C2DCard::m_pSeqName = "card:00";

C2DCard::C2DCard() {
    m_card.m_nId = 0;
    m_idxSelect = 0;
    m_fgBack = 0;
    m_hSeqHandleBack = 0;
    m_pParentCtrl = NULL;
}

C2DCard::C2DCard(INT nType) : C2DSeqCtrl(nType) {
    m_card.m_nId = 0;
    m_idxSelect = 0;
    m_fgBack = 0;
    m_pParentCtrl = NULL;
}

C2DCard::~C2DCard() {
    if (m_hSeqHandleBack) C2DMan::destroy(m_hSeqHandleBack);
}

void C2DCard::replace(CARD inCard) {
    CHAR nLayer = m_nLayer;
    destroy();
    m_nLayer = nLayer;
    create(inCard);
}

void C2DCard::create(CARD inCard) {
    static LPCTSTR seqName2 = "card:01";
    static LPCTSTR seqName3 = "card:02";
    static LPCTSTR texName[4] = { "card:%02d", "card_b:%02d", "card_p:%02d", "card_n:%02d" };
    static BYTE wakuTbl[15] = { 0x00, 0x07, 0x05, 0x05, 0x06, 0x06, 0x06, 0x04, 0x2F, 0x02, 0x01, 0x00, 0x03, 0x09, 0x07 };
    static BYTE numTbl[15] = { 0, 1, 1, 1, 1, 1, 1, 1, 0, 1, 1, 1, 0, 0, 1 };
    if (inCard.m_nCategory20 == 0xC) return;
    if (inCard.m_nKind == 0xA0) {
        addSeq(m_pSeqName, 0x2E);
        if (inCard.m_nNumber < 0) {
            addSeq(m_pSeqName, 0x20);
            m_card = inCard;
            return;
        }
    } else {
        C2DSeqObj* pObj = addSeq(seqName3, 0);
        if (pObj) {
            const CARD_DATA* pData = &cardDataTbl[inCard.m_nKind];
            UINT nTexPos = pData->nTexPos;
            UINT nTexNo = pData->nTexNo;
            G_UV uvStart, uvEnd;
            CHAR szTex[16];
            USHORT u = ((nTexPos & 3) << 6) | 2;
            USHORT v = ((nTexPos & 0xC) << 4) | 1;
            uvEnd.set(u | 0x3C, v | 0x3E);

            uvStart.set(u, v);

            if (nTexNo < 22) sprintf(szTex, texName[0], nTexNo);
            else if (nTexNo < 26) sprintf(szTex, texName[1], nTexNo - 22);
            else if (nTexNo < 27) sprintf(szTex, texName[2], nTexNo - 26);
            else sprintf(szTex, texName[3], nTexNo - 27);
            pObj->changeTexture(0, szTex, &uvStart, &uvEnd);
        }
    }
    addSeq((inCard.m_nCategory21 != 8) ? seqName2 : m_pSeqName,
           (inCard.m_nKind == 0xB9) ? 0x14 : wakuTbl[inCard.m_nCategory21]);
    if (numTbl[inCard.m_nCategory21]) {
        if (inCard.m_nNumber >= 0 && inCard.m_nKind != 0xB9) {
            addSeq(m_pSeqName, inCard.m_fgPremium ? inCard.m_nNumber + 0x3D : inCard.m_nNumber + 0x23);
        }
    }
    if (inCard.m_fgPremium || inCard.m_nCategory21 == 0xC || inCard.m_nKind == 0x7D || inCard.m_nKind == 0x7E) {
        addSeq(m_pSeqName, 0x2D);
    }
    m_card = inCard;
}

void C2DCard::replaceNumberNormal(BYTE inNumber) {
    if (m_nSeq >= 3) replaceSeq(2, m_pSeqName, m_card.m_fgPremium ? inNumber + 0x3D : inNumber + 0x23);
}

void C2DCard::replaceNumberBlue(BYTE inNumber) {
    if (m_nSeq >= 3) replaceSeq(2, m_pSeqName, inNumber + 0x33);
}

void C2DCard::replaceNumberRoulette() {
    if (m_nSeq >= 3) replaceSeq(2, m_pSeqName, 0x47);
}

void C2DCard::select(BOOL bUnusable) {
    if (m_nSeq && !m_idxSelect) {
        m_idxSelect = m_nSeq;
        addSeq(m_pSeqName, bUnusable ? 0x48 : 0);
    }
}

void C2DCard::unselect() {
    if (m_idxSelect) {
        destroy(m_idxSelect);
        m_idxSelect = 0;
    }
}

void C2DCard::changeTime(UINT inTime) {
    if (!m_idxSelect || m_card.m_nKind != 0xA0) {
        C2DSeqCtrl::changeTime(inTime);
    } else {
        for (INT i = 0; i < m_idxSelect; i++) C2DSeqCtrl::changeTime(i, inTime);
    }
}

void C2DCard::createEmpty() {
    addSeq(m_pSeqName, 1);
}

void C2DCard::replaceNorma(RCNORMA::NormaSrc inNorma, INT inStatus) {
    destroy();
    RCNORMA::NormaSrc t = inNorma;
    _createNormaRaw(this, _NORMA_PIECES(t), inStatus);
}
void C2DCard::createNorma(RCNORMA::NormaSrc inNorma, INT inStatus) {
    static BYTE wakuTbl[4] = { 0x20, 0x22, 0x21, 0x1F };
    static BYTE rgbUpperTbl[4] = { 0x00, 0x07, 0x05, 0x03 };
    static BYTE opTbl[4] = { 0x00, 0x13, 0x14, 0x12 };
    C2DSeqObj* p2DSeqObj;
    G_UV uvStart;
    G_UV uvEnd;
    addSeq(m_pSeqName, wakuTbl[inNorma._nColor]);
    if (inNorma._nColor) addSeq(m_pSeqName, rgbUpperTbl[inNorma._nColor]);
    if (inNorma._nOp) {
        if (inNorma._nOp != 4) {
            addSeq(m_pSeqName, inNorma._nNumber + 8);
            addSeq(m_pSeqName, opTbl[inNorma._nOp]);
        } else {
            INT nLow = inNorma._nNumber % 10;
            p2DSeqObj = addSeq(m_pSeqName, nLow + 0x15);
            if (inNorma._nNumber < 10) p2DSeqObj->shiftBase(-15.0f, 0.0f);
            else {
                INT nNum = inNorma._nNumber;
                INT nHigh = nNum / 10;
                addSeq(m_pSeqName, nHigh + 8);
            }
        }
    }
}

void C2DCard::createBack() {
    static LPCTSTR seqName2 = "card:01";
    static BYTE uraTbl[15] = { 0x00, 0x11, 0x0F, 0x0F, 0x10, 0x10, 0x10, 0x0E, 0x0E, 0x0C, 0x0B, 0x0A, 0x0D, 0x13, 0x11 };
    UINT idx = (m_card.m_nKind == 0xB9) ? 0x15 : uraTbl[m_card.m_nCategory21];
    if (idx) {
        HANDLE h = C2DMan::create(seqName2, idx, &m_seqControl);
        if (h) {
            C2DSeqObj* pObj = C2DMan::getSeqObj(h);
            if (pObj) {
                pObj->setParent();
                if (m_nLayer >= 0) pObj->changeLayer(m_nLayer);
                m_hSeqHandleBack = h;
            }
        }
    }
}

void C2DCard::setRot(FVECTOR& vRot) {
    C2DSeqCtrl::setRot(vRot);
    FVECTOR v;
    v.set(0, 0, 1.0f, 1.0f);



    ApplyMatrix(v, *(FMATRIX*)&m_seqControl, v);
    if (v.z >= 0.0f) {
        if (m_fgBack) {
            m_fgBack = 0;
            if (m_bEnable) enableDisp(TRUE);
            C2DMan::enableDisp(m_hSeqHandleBack, FALSE);
        }
    } else if (!m_fgBack) {
        m_fgBack = 1;
        if (!m_hSeqHandleBack) createBack();
        for (INT i = 0; i < m_nSeq; i++) C2DMan::enableDisp(m_hSeqHandle[i], FALSE);
        if (m_bEnable) C2DMan::enableDisp(m_hSeqHandleBack, TRUE);
    }
}

void C2DCard::checkBack() {
    if (!m_nSeq) return;
    if (m_seqControl.pParentSeqControl) {
        int* pHandle = m_pParentCtrl->getHandle();
        if (!pHandle) return;
        C2DSeqObj* pObj = C2DMan::getSeqObj(*pHandle);
        if (!pObj) return;
        pObj->_main();
        pObj->m_fgMainSkip = 1;
    }
    FMATRIX* pMat = m_seqControl.pParentSeqControl ? (FMATRIX*)((CHAR*)m_seqControl.pParentSeqControl + 0x80)
                                                   : (FMATRIX*)&m_seqControl;
    FVECTOR v;
    v.set(0, 0, 1.0f, 1.0f);



    v.w = InnerProduct(v, pMat->v[2]);
    if (v.w >= 0.0f) {
        if (m_fgBack) {
            m_fgBack = 0;
            enableDisp(TRUE);
            C2DMan::enableDisp(m_hSeqHandleBack, FALSE);
        }
    } else if (!m_fgBack) {
        m_fgBack = 1;
        enableDisp(FALSE);
        if (!m_hSeqHandleBack) createBack();
        C2DMan::enableDisp(m_hSeqHandleBack, TRUE);
    }
}

void C2DCard::setParent(C2DSeqCtrl* p2DSeqCtrl) {
    m_pParentCtrl = p2DSeqCtrl;
    C2DSeqCtrl::setParent(p2DSeqCtrl);
}

void C2DCard::enableDisp(BOOL bEnable) {
    if (m_hSeqHandleBack) {
        if (!m_fgBack) {
            C2DSeqCtrl::enableDisp(bEnable);
        } else {
            C2DMan::enableDisp(m_hSeqHandleBack, bEnable);
            if (bEnable) m_bEnable = bEnable;
            else C2DSeqCtrl::enableDisp(FALSE);
        }
    } else {
        C2DSeqCtrl::enableDisp(bEnable);
    }
}

void C2DCard::changeLayer(INT nLayer) {
    C2DSeqCtrl::changeLayer(nLayer);
    if (m_hSeqHandleBack) C2DMan::changeSeqLayer(m_hSeqHandleBack, nLayer);
}

// SOURCE DATA: .data .rodata
