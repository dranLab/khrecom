
#define CTDMAN_H_PATH "c:/KHCOM/MainApp/System/CTD/CtdMan.h"
#define GPRIM_H_PATH "System/Graphic/GPrim.h"
#include "System/Ctd/CtdMan.h"
#include "System/Graphic/Gprim.h"
#include "System/Wnd.h"
#include "System/Graphic/2d/2DSeqCtrl.h"
#include "System/Graphic/2d/2DSeqObj.h"
#include "Game/Debug/DebugWork.h"
#include "Game/SceneEnv.h"
#include "System/Graphic/StrPrim.h"
#include "System/Sound/SoundMan.h"
#include <string.h>
#include "include_asm.h"

#define RET_ADDR() ({ UINT nRA; __asm__ volatile("daddu %0, $31, $0" : "=r"(nRA)); nRA; })

extern "C" void sceVu0InterVector(FVECTOR& v0, FVECTOR& v1, FVECTOR& v2, float t);
void ScaleVector(FVECTOR& v0, const FVECTOR& v1, FLOAT f);
void FTOI0Vector(IVECTOR& v0, const FVECTOR& v1);

/* G_XYZ's two-coordinate setter (z at the screen depth) */
static inline void _setXYZ(G_XYZ& xyz, FIX4 _x, FIX4 _y) {
    xyz.x = _x;
    xyz.y = _y;
    xyz.z = 0xffff280;
}

/* SCREEN_HEIGHT read through a reference (the original reloads it after the preceding member stores) */
static inline INT& _screenHeight() { return SCREEN_HEIGHT; }

inline void* operator new[](size_t size) throw() { return com_malloc(size); }
inline void operator delete[](void* mem) { com_free(mem); }

inline void* operator new(size_t size, char* msg) throw() {
    return com_malloc_d(size, msg, RET_ADDR());
}

#include "System/Graphic/TexInfo.h"






class CPolyGT4 : public CGTexPrim {
public:
    G_XYZ m_P0;  /* 0x20 */
    G_XYZ m_P1;  /* 0x2c */
    G_XYZ m_P2;  /* 0x38 */
    G_XYZ m_P3;  /* 0x44 */
    G_RGBA m_C0; /* 0x50 */
    G_RGBA m_C1; /* 0x54 */
    G_RGBA m_C2; /* 0x58 */
    G_RGBA m_C3; /* 0x5c */
    G_UV m_T0;   /* 0x60 */
    G_UV m_T1;   /* 0x64 */
    G_UV m_T2;   /* 0x68 */
    G_UV m_T3;   /* 0x6c */

    CPolyGT4() {
        m_C0.set(0x80, 0x80, 0x80, 0x80);
        m_C1 = m_C0;
        m_C2 = m_C0;
        m_C3 = m_C0;
    }
    virtual ~CPolyGT4();
    virtual UINT getPacketSize() const;
private:
    virtual FIX4 calcZ();
    virtual void makePkt();
};

INT CBaseWnd::m_nLayerFG = 9;

CBaseWnd::CBaseWnd(CTask& inParent) : CComponentObj(inParent) {
    setPriority(0x8000);
    m_nObjType = (ObjType::eObjType)0xe;
    m_phaseCnt = 0;
    m_step = 0;
}

CBaseWnd::~CBaseWnd() {}

CTextWnd::CTextWnd(CTask& inParent, BOOL fgVoice) : CBaseWnd(inParent) {
    m_voiceName[0] = '\0';
    m_pWnd = NULL;
    m_pText = NULL;
    m_status = 0;
    m_textOffset.m_x = 0;
    m_textOffset.m_y = 0;
    m_fontSpace.m_w = 0;
    m_fontSpace.m_h = 0;
    m_textColor = 0x80040404;
    m_textExColor = 0x40040404;
    m_fontSize = 0x10;
    m_fontStyle = 0x1200;
    m_wndStyle = 0;
    m_wndArrangement = 0;
    m_fgDestroy = 0;
    m_textSelectOffset = 0;
    m_textDispDelay = 0;
    m_fDispWait = 0;
    m_fTextDispInterval = 0;
    m_fgVoice = fgVoice;
    m_fOpenDelay = 0;
    m_nMode = 0;
    m_nVoiceHandle = 0;
    m_fgVoiceTimeInclude = 0;
    m_pTmpText = NULL;
    m_bEnableLFDisp = TRUE;
    m_fOpenSpeed = 1.0f;
    m_fCloseSpeed = 1.0f;
}

CTextWnd::~CTextWnd() {
    if (m_pWnd) delete m_pWnd;
    m_pWnd = NULL;
    if (m_pText) delete m_pText;
    m_pText = NULL;
    if (m_pTmpText) delete m_pTmpText;
    m_pTmpText = NULL;
}
void CTextWnd::createWindow(CPointI& pos, CSizeI& size, UINT wndColor, const CHAR* pText, UINT textColor,
                            UINT fontSize, UINT wndArrangement, CPointI& cyobiPos, UINT waitFrame, UINT fontStyle) {
    init(pos, size, wndColor, pText, textColor, fontSize, wndArrangement, cyobiPos, waitFrame, fontStyle, NULL, 0, 0);
}

void CTextWnd::createWindowC(CPointI& pos, CSizeI& size, UINT wndColor, const CHAR* pText, UINT textColor,
                             UINT fontSize, UINT wndArrangement, CPointI& cyobiPos, UINT waitFrame, UINT fontStyle) {
    m_fontSpace.m_h = 2;
    m_wndStyle = 1;
    m_fontSpace.m_w = 0;
    init(pos, size, wndColor, pText, textColor, fontSize, wndArrangement, cyobiPos, waitFrame, fontStyle, NULL, 0, 0);
}
void CTextWnd::createWindowCTD(UINT ctdID, CPointI* pPos, CPointI* pCyobiPos, UINT waitFrame, UINT* pCtdID,
                               UINT nLine) {
    CHAR* pText;
    CtdBlock* pCtdBlock = CCtdMan::getCtdBlock(ctdID, &pText);
    if (!pCtdBlock) return;
    m_ctdID = ctdID;
    m_fgVoiceTimeInclude = waitFrame & 0x80000000;
    waitFrame &= 0x7fffffff;
    m_wndStyle = (pCtdBlock->wndStyle & 0xf00) >> 8;
    INT spaceX = (SHORT)pCtdBlock->fntSpaceX;
    INT spaceY = (SHORT)pCtdBlock->fntSpaceY;
    m_fontSpace.m_w = spaceX;
    m_fontSpace.m_h = spaceY;
    INT marginX = pCtdBlock->txtMarginX;
    INT marginY = pCtdBlock->txtMarginY;
    m_textOffset.m_x = marginX;
    m_textOffset.m_y = marginY;
    m_textExColor = pCtdBlock->txtExColor;
    CPointI pos0;
    if (!pPos || (pPos->m_x < 0 && pPos->m_y < 0)) {
        INT x = pCtdBlock->posX;
        INT y = pCtdBlock->posY;
        pos0.m_x = x;
        pos0.m_y = y;
    } else {
        pos0 = *pPos;
    }
    CSizeI size;
    INT w = pCtdBlock->sizeX;
    INT h0 = pCtdBlock->sizeY;
    size.m_w = w;
    size.m_h = h0;
    CPointI cyobiPos;
    if (pCyobiPos) {
        cyobiPos = *pCyobiPos;
    } else {
        cyobiPos.m_x = 0;
        cyobiPos.m_y = 0;
    }
    if (nLine) {
        INT textLen = strlen(pText);
        if (textLen) {
            if (m_pTmpText) delete m_pTmpText;
            m_pTmpText = NULL;
#line 183
            m_pTmpText = (CHAR*)com_malloc_d(textLen + 1, __FILE__ "(183)", RET_ADDR());
            strcpy(m_pTmpText, pText);
            CHAR* pT = m_pTmpText;
            INT fgOK = FALSE;
            INT nL = 1;
            INT j = 0;
            for (INT i = 0; i < textLen; i++, pT++) {
                if (*pT == '\n') {
                    if (!fgOK) {
                        j++;
                        if (j >= nLine) {
                            *pT = '\0';
                            fgOK = TRUE;
                        }
                    }
                    nL++;
                }
            }
            if (fgOK) {
                pText = m_pTmpText;
                INT h = m_fontSpace.m_h + pCtdBlock->fntSize;
                size.m_h -= h * (nL - nLine);
                if (size.m_h < h) size.m_h = h;
            }
        }
    }
    init(pos0, size, pCtdBlock->wndColor, pText, 0x80808080, pCtdBlock->fntSize, pCtdBlock->wndStyle & 0xff, cyobiPos,
         waitFrame, pCtdBlock->fntStyle, pCtdID, pCtdBlock->cyobiType, pCtdBlock->cyobiPos);
}
void CTextWnd::createWindowCTDEz(UINT ctdID, const CHAR** ppWords, UINT waitFrame, CPointI* pPos,
                                 CPointI* pCyobiPos) {
    createWindowCTD(ctdID, pPos, pCyobiPos, waitFrame, NULL, 0);
    if (m_pText) {
        CHAR* pText;
        if (CCtdMan::getCtdBlock(ctdID, &pText)) m_pText->changeText(pText, ppWords);
    }
}

void CTextWnd::init(CPointI& pos, CSizeI& size, UINT wndColor, const CHAR* pText, UINT textColor, UINT fontSize,
                    UINT wndArrangement, CPointI& cyobiPos, UINT waitFrame, UINT fontStyle, UINT* pCtdID,
                    UINT cyobiStyle, INT cyobiPosX) {
    m_wndArrangement = wndArrangement;
    setTextColor(textColor);
    initWindow(pos, size, wndColor, cyobiPos, cyobiStyle, cyobiPosX, FALSE);
    setFontSize(fontSize);
    setText(pText, pCtdID);
    UINT style = (m_wndStyle == 3) ? fontStyle : fontStyle | 0x1000;
    setFontStyle(m_wndStyle == 5 ? style | 0x2000 : style);
    if (waitFrame) m_fDispWait = waitFrame;
}

void CTextWnd::initWindow(CPointI& pos, CSizeI& size, UINT wndColor, CPointI& cyobiPos, UINT cyobiStyle,
                          INT cyobiPosX, INT fgImage) {
    m_targetPos = pos;
    m_targetSize = size;
    switch (m_wndArrangement) {
    case 0:
        break;
    case 1:
        m_targetPos.m_x = 0x20;
        break;
    case 2:
        m_targetPos.m_x = 0x100 - (size.m_w >> 1);
        break;
    case 3:
        m_targetPos.m_x = 0x1e0 - size.m_w;
        break;
    }
    CPointI pointI(pos.m_x, pos.m_y);
    setPosition(pointI);
    CSizeI sizeI(size.m_w, 0);
    setSize(sizeI);
    if (m_pWnd) delete m_pWnd;
    m_pWnd = NULL;
#line 307
    m_pWnd = new (__FILE__ "(307)") CWnd(m_nLayerFG);
    if (!fgImage) {
        m_pWnd->init(m_targetPos, size, wndColor, m_wndStyle, cyobiPos, cyobiStyle, cyobiPosX);
        if (!wndColor) enableWndDisp(FALSE);
    }
}

void CTextWnd::setText(const CHAR* pText, UINT* pCtdID) {
    if (!m_pWnd && !pText) return;
    if (m_pText) delete m_pText;
    m_pText = NULL;
#line 333
    m_pText = new (__FILE__ "(333)") CText(m_nLayerFG);
    CPointI pos = m_targetPos;
    CSizeI size = m_targetSize;
    m_pText->init(pos, size, pText, m_textDispDelay, m_textColor, m_fontSize, m_fontStyle, m_textOffset, m_fontSpace,
                  pCtdID);
    m_pText->setExColor(m_textExColor);
    m_pText->setSelectOffset(m_textSelectOffset);
    m_pText->setDispInterval(m_fTextDispInterval);
    if (m_status) {
        m_step = 1;
        if (m_pWnd) m_pWnd->setFukidasi(5, 0);
    }
    setVoiceName(pText);
}

void CTextWnd::setVoiceName(const CHAR* pText) {
    if (!m_fgVoice) return;
    const CHAR* pVoiceName = (const CHAR*)((UINT)(pText + strlen(pText) + 4) & ~3);
    if (*pVoiceName == '\0') {
        m_voiceName[0] = '\0';
    } else if (strlen(pVoiceName) < 16) {
        strcpy(m_voiceName, pVoiceName);
    } else {
        m_voiceName[0] = '\0';
    }
}

void CTextWnd::setTextCTD(UINT ctdID, UINT* pCtdID) { setText(CCtdMan::getText(ctdID), pCtdID); }

void CTextWnd::changeText(const CHAR* pText, UINT* pCtdID) {
    if (!m_pText && !pText) return;
    if (m_step == 5) {
        m_pText->changeText(pText, pCtdID);
        setVoiceName(pText);
    }
}

void CTextWnd::changeText(const CHAR* pText, const CHAR** ppWords) {
    if (m_pText || pText || ppWords) m_pText->changeText(pText, ppWords);
}

void CTextWnd::changeTextCTD(UINT ctdID, UINT* pCtdID) { changeText(CCtdMan::getText(ctdID), pCtdID); }

void CTextWnd::changeTextNext(UINT* pCtdID) {
    if (m_pText && m_step == 5) changeTextCTD(++m_ctdID, pCtdID);
}

void CTextWnd::setTextColor(UINT textColor) { m_textColor = textColor; }

void CTextWnd::setTextExColor(UINT textExColor) { m_textExColor = textExColor; }

void CTextWnd::setTextDispDelay(INT textDispDelay) { m_textDispDelay = textDispDelay; }

void CTextWnd::setFontSize(UINT fontSize) { m_fontSize = fontSize; }

void CTextWnd::setFontStyle(UINT fontStyle) {
    m_fontStyle = fontStyle;
    if (m_pText) m_pText->setFontStyle(fontStyle);
}

void CTextWnd::setTextOffset(CPointI textOffset) { m_textOffset = textOffset; }

void CTextWnd::setFontSpace(CSizeI fontSpace) {
    m_fontSpace = fontSpace;
    if (m_pText) m_pText->setFontSpace(fontSpace.m_w, fontSpace.m_h);
}

void CTextWnd::open() {
    if (m_step == 0 || m_step == 9) m_step = 1;
}

void CTextWnd::close() {
    if (m_step <= 0) {
        m_step = 9;
    } else if (m_step < 6) {
        m_step = 6;
    }
}

void CTextWnd::closeDestroy() {
    if (m_step == 5) {
        m_step = 6;
        m_fgDestroy = TRUE;
    } else {
        out();
    }
}

void CTextWnd::closeFadeOutDestroy(UINT fadeOutTime) {
    if (m_step == 5) {
        m_step = 6;
        m_fgDestroy = TRUE;
    } else {
        out();
    }
}

void CTextWnd::clearText() {
    if (m_pText) delete m_pText;
    m_pText = NULL;
}

void CTextWnd::setWndStyle(UINT wndStyle) {
    m_wndStyle = wndStyle;
    if (m_pWnd) m_pWnd->setFukiStyle(wndStyle);
}

void CTextWnd::setWndRate(FVECTOR& wndRate) {}

void CTextWnd::changeWndStyle(UINT wndStyle) {}

void CTextWnd::enableWndDisp(UINT fgDisp) {
    if (m_pWnd) m_pWnd->enableDisp2(fgDisp);
}

void CTextWnd::enableDisp(UINT fgDisp) {
    if (m_pWnd) {
        if (fgDisp) {
            m_pWnd->enableDisp();
        } else {
            m_pWnd->disableDisp();
        }
    }
}

void CTextWnd::changeTextSelect(INT sel, INT fgDispUnderLine) {
    if (m_pText) m_pText->changeSelect(sel, fgDispUnderLine);
}

void CTextWnd::enableLFDisp(BOOL bEnable) { m_bEnableLFDisp = bEnable; }

void CTextWnd::changeTextBrightness(G_RGBA& inColor) {
    if (m_pText) m_pText->changeBrightness(inColor);
}

void CTextWnd::setSelectOffset(UINT selectOffset) {
    m_textSelectOffset = selectOffset;
    if (m_pText) m_pText->setSelectOffset(selectOffset);
}

void CTextWnd::changeCursorColor(UINT cursorColor) {
    if (m_pText) m_pText->changeCursorColor(cursorColor);
}

void CTextWnd::setWords(UINT* pCtdID) {
    if (m_pText) m_pText->setWords(pCtdID[0], pCtdID[1], pCtdID[2], pCtdID[3]);
}

void CTextWnd::setWords(const CHAR** ppWord) {
    if (m_pText) m_pText->setWords(ppWord[0], ppWord[1], ppWord[2], ppWord[3]);
}

void CTextWnd::setDispInterval(UINT dispInterval) {
    if (!dispInterval) {
        m_fTextDispInterval = 0.0f;
    } else {
        m_fTextDispInterval = 60.0f / dispInterval;
    }
}

void CTextWnd::setCharFadeInDelay(FLOAT fCharFadeInDelay) {
    if (m_pText) m_pText->setCharFadeInDelay(fCharFadeInDelay);
}

void CTextWnd::setTextFadeOutTime(FLOAT fTextFadeOutTime) {
    if (m_pText) m_pText->setTextFadeOutTime(fTextFadeOutTime);
}
void CTextWnd::mainImpl() {
    CSizeI sizeI;
    if (!m_pWnd) return;
    if (CSceneEnv::getStopTypeBit() & (1 << m_nObjType)) return;
    m_frameRateF = getFrameRateF();
    m_frameRate = getFrameRate();
    switch (m_step) {
    case 0:
        return;
    case 1:
        m_fOpenDelay -= m_frameRateF;
        if (0.0f <= m_fOpenDelay) return;
        m_fOpenDelay = 0.0f;
        m_fVoiceTime = 0.0f;
        m_qCnt = 0;
        m_pWnd->enableDisp();
        if (m_pText) {
            if (m_fTextDispInterval == 0.0f) {
                m_pText->initDispLineMax(m_textDispDelay);
            } else {
                m_pText->setDispInterval(m_fTextDispInterval);
            }
        }
        m_step++;
    case 2:
        setPosition(m_targetPos);
        setSize(m_targetSize);
        m_pWnd->setPosition(getPosition());
        m_pWnd->setSize(getSize());
        m_qCnt += (INT)(m_frameRate * m_fOpenSpeed);
        if (m_pWnd->setFukidasi(1, m_qCnt)) {
            m_step++;
            if (m_pText) m_pText->enableDisp();
        }
        if (m_step != 2) {
            m_fCnt = 0.0f;
            m_status = 1;
            m_qCnt = 0;
        }
        break;
    case 3:
        if (m_pText) {
            if (m_pText->isDispLineMax()) {
                if (m_bEnableLFDisp) m_pWnd->setFukidasi(4, 0);
                m_step++;
            } else if (0.0f < m_fVoiceTime) {
                m_fVoiceTime -= m_frameRateF;
            }
        } else {
            m_step++;
        }
        m_fCnt = m_fDispWait + (m_fgVoiceTimeInclude ? m_fVoiceTime : 0.0f);
        break;
    case 4:
        m_fCnt -= m_frameRateF;
        if (m_fCnt < 0.0f) m_step++;
        break;
    case 5:
        if (m_step == 5) m_fCnt += m_frameRateF;
        break;
    case 6:
        m_qCnt = 0;
        m_pWnd->setFukidasi(2, 0);
        if (m_pText) {
            m_pText->setSelectOut();
            if (m_pText->initTextFadeOut()) {
                m_step = 7;
                break;
            }
            m_pText->disableDisp();
        }
        m_step = 8;
        break;
    case 7:
        if (m_pText->isEndTextFadeOut()) m_step++;
        break;
    case 8:
        m_qCnt += (INT)(m_frameRate * m_fCloseSpeed);
        if (m_pWnd->setFukidasi(3, m_qCnt)) {
            m_pWnd->disableDisp();
            m_status = 0;
            m_step++;
        }
        break;
    default:
        if (m_fgDestroy) out();
        return;
    }
    CPointI pos = getPosition();
    m_pWnd->setPosition(pos);
    m_pWnd->_main(m_frameRateF);
    if (m_pText) {
        m_pText->setPosition(pos + m_textOffset);
        m_pText->_main(m_frameRateF);
    }
}
void CTextWnd::dispImpl() {
    if (m_pWnd && m_pWnd->isDispEnable() && DebugWork::g_dw.sys.m_iFukidasiDisp) {
        INT fgDispText = FALSE;
        m_pWnd->_disp();
        if (m_pWnd->getSize() == m_targetSize && m_pText) {
            m_pText->_disp();
            fgDispText = TRUE;
        }
        if (fgDispText) m_pText->_disp2();
    }
}

void CTextWnd::setOpenSpeed(FLOAT fOpenSpeed) { m_fOpenSpeed = fOpenSpeed; }

void CTextWnd::setCloseSpeed(FLOAT fCloseSpeed) { m_fCloseSpeed = fCloseSpeed; }
CWnd::CWnd(INT layerFG) {
    m_nLayerFG = layerFG;
    m_dispEnable2 = TRUE;
    m_pFukiCtrl = NULL;
    m_dispEnable = FALSE;
    m_fWndRateX = m_fWndRateY = 1.0f;
}

CWnd::~CWnd() {
    if (m_pFukiCtrl) delete m_pFukiCtrl;
    m_pFukiCtrl = NULL;
}
void CWnd::init(CPointI& pos, CSizeI& size, UINT wndColor, UINT wndStyle, CPointI& cyobiPos, UINT cyobiStyle,
                INT cyobiPosX) {
    setPosition(pos);
    setSize(size);
    m_cyobiPos = cyobiPos;
    m_wndStyle = wndStyle;
    m_cyobiStyle = cyobiStyle;
    m_cyobiPosX = cyobiPosX;
    setFukidasi(0, 0);
}

void CWnd::_main(FLOAT frameRateF) {}

void CWnd::_disp() {
    if (m_pFukiCtrl) {
        if (!m_dispEnable || !m_dispEnable2) {
            m_pFukiCtrl->enableDisp(FALSE);
        } else {
            m_pFukiCtrl->enableDisp(TRUE);
        }
    }
}

void CWnd::setFukiStyle(UINT wndStyle) {
    m_wndStyle = wndStyle;
    setFukidasi(0, 0);
}

void CWnd::changeFukiStyle(UINT wndStyle) {}
INT CWnd::setFukidasi(INT iCmd, TIMEQ timeq) {
    D2::C2DSeqObj* p2DSeqObj;
    FVECTOR vec0;
    INT ret = FALSE;
    CPointI pos;
    CSizeI size = getSize();
    FLOAT fLineY;
    FLOAT fTmp;
    FLOAT fCrossX;
    static USHORT seqNoTbl[5] = {67, 70, 73, 79, 76};
    static USHORT cyobiSeqNoTbl[12] = {41, 43, 45, 47, 49, 51, 53, 55, 57, 59, 61, 63};
    static CHAR* sqdName = "message.sqd";
    switch (iCmd) {
    case 0:
        if (m_pFukiCtrl) {
            delete m_pFukiCtrl;
            m_pFukiCtrl = NULL;
        }
        if (m_wndStyle < 5) {
            m_wndSeqNo = seqNoTbl[m_wndStyle];
#line 1163
            m_pFukiCtrl = new (__FILE__ "(1163)") D2::C2DSeqCtrl;
            m_pFukiCtrl->setLayer(m_nLayerFG);
            p2DSeqObj = m_pFukiCtrl->addSeq(sqdName, m_wndSeqNo);
            pos = getPosition();
            p2DSeqObj->setFukidasiSize(size.m_w, size.m_h);
            m_vCyobiLocal.set(pos.m_x + size.m_w / 2 + 0.5f, pos.m_y + size.m_h / 2 + 0.5f, 0.0f, 1.0f);
            m_pFukiCtrl->setPos(m_vCyobiLocal);
            m_pFukiCtrl->changeTime(0);
            m_nStepFukidasi = 0;
            m_pFukiCtrl->enableDisp(FALSE);
            break;
        }
        break;
    case 1:
        if (!m_pFukiCtrl) {
            ret = TRUE;
            break;
        }
        if (m_nStepFukidasi == 0) {
            if (timeq >= 1000 || !m_dispEnable2) {
                ret = TRUE;
                p2DSeqObj = m_pFukiCtrl->replaceSeq(0, sqdName, m_wndSeqNo + 1);
                p2DSeqObj->setFukidasiSize(size.m_w, size.m_h);
                fLineY = size.m_h * 0.5f - 0.0f;
                if (m_cyobiStyle == 0) {
                    FLOAT cyobiY = m_cyobiPos.m_y;
                    if (cyobiY < m_vCyobiLocal.y) fLineY = -fLineY;
                    vec0.y = cyobiY - m_vCyobiLocal.y;
                    vec0.x = m_cyobiPos.m_x - m_vCyobiLocal.x;
                    fTmp = vec0.x / vec0.y;
                    fCrossX = fTmp * fLineY;
                    if (0.0f <= fLineY) {
                        m_cyobiSeqNo = (0.0f <= fCrossX) ? 0x2b : 0x29;
                        fLineY -= 8.0f;
                    } else {
                        m_cyobiSeqNo = (0.0f <= fCrossX) ? 0x2f : 0x2d;
                        fLineY += 1.0f;
                    }
                    if (m_wndStyle != 2) {
                        fCrossX = (0.0f <= fCrossX) ? fCrossX + -15.0f : fCrossX + 15.0f;
                    } else {
                        m_cyobiSeqNo += 0x10;
                    }
                } else {
                    if ((m_cyobiStyle - 1) & 2) {
                        fLineY = -fLineY;
                        fLineY += 1.0f;
                    } else {
                        fLineY -= 8.0f;
                    }
                    fCrossX = m_cyobiPosX;
                    m_cyobiSeqNo = cyobiSeqNoTbl[m_cyobiStyle - 1];
                }
                if (size.m_w * 0.5f - 10.0f <= fCrossX) {
                    fCrossX = size.m_w * 0.5f - 10.0f;
                } else if (fCrossX < -size.m_w * 0.5f + 10.0f) {
                    fCrossX = -size.m_w * 0.5f + 10.0f;
                }
                if (m_pFukiCtrl->getNumSeq() < 2) {
                    m_pFukiCtrl->addSeqShift(sqdName, m_cyobiSeqNo, fCrossX, fLineY);
                    m_vCyobiLocal.x = fCrossX;
                    m_vCyobiLocal.y = fLineY;
                    if (m_wndStyle == 1 || m_wndStyle >= 3 || (!m_cyobiPos.m_x && !m_cyobiPos.m_y)) {
                        m_pFukiCtrl->enableDisp(1, FALSE);
                    }
                }
                m_nStepFukidasi++;
            } else {
                m_pFukiCtrl->changeTime(0, timeq / 50);
            }
        } else {
            if (timeq >= 1350 || !m_dispEnable2) ret = TRUE;
        }
        break;
    case 2:
        if (!m_pFukiCtrl) break;
        p2DSeqObj = m_pFukiCtrl->replaceSeq(0, sqdName, m_wndSeqNo + 2);
        p2DSeqObj->setFukidasiSize(size.m_w, size.m_h);
        m_pFukiCtrl->changeTime(0, 0);
        if (m_pFukiCtrl->getNumSeq() >= 2) {
            m_pFukiCtrl->replaceSeqShift(1, sqdName, m_cyobiSeqNo + 1, m_vCyobiLocal.x, m_vCyobiLocal.y);
            if (m_pFukiCtrl->getNumSeq() >= 3) {
                m_pFukiCtrl->replaceSeqShift(2, sqdName, 0x42, m_vLFLocal.x, m_vLFLocal.y);
            }
        }
        m_nStepFukidasi = 0;
        break;
    case 3:
        if (!m_pFukiCtrl) {
            ret = TRUE;
            break;
        }
        if (m_nStepFukidasi == 0) {
            if (timeq >= 350 || !m_dispEnable2) m_nStepFukidasi = 1;
        } else {
            if (timeq >= 950 || !m_dispEnable2) ret = TRUE;
            m_pFukiCtrl->changeTime(0, (timeq - 350) / 50);
        }
        break;
    case 4:
        if (!m_pFukiCtrl) break;
        if (m_pFukiCtrl->getNumSeq() < 3) {
            m_vLFLocal.set(size.m_w * 0.5f - 5.0f, size.m_h * 0.5f - 9.0f, 0.0f, 1.0f);
            CHAR*& rName = sqdName;
            m_pFukiCtrl->addSeqShift(rName, 0x41, m_vLFLocal.x, m_vLFLocal.y);
        } else if (m_pFukiCtrl->getNumSeq() == 3) {
            m_pFukiCtrl->replaceSeqShift(2, sqdName, 0x41, m_vLFLocal.x, m_vLFLocal.y);
        }
        break;
    case 5:
        if (!m_pFukiCtrl) break;
        if (m_pFukiCtrl->getNumSeq() == 3) {
            m_pFukiCtrl->replaceSeqShift(2, sqdName, 0x42, m_vLFLocal.x, m_vLFLocal.y);
        }
        break;
    case 6:
        if (!m_pFukiCtrl) break;
        if (m_pFukiCtrl->getNumSeq() == 3) m_pFukiCtrl->destroy(2);
        break;
    }
    return ret;
}
void CWnd::setWndRate(FVECTOR& wndRate) {
    m_fWndRateX = wndRate.x;
    m_fWndRateY = wndRate.y;
}

void CWnd::changeColor(UINT wndColor, UINT lineColor) {}
CText::CText(INT layerFG) {
    m_cursorPos = 0.0f;
    m_nLayerFG = layerFG;
    m_nSelect = -1;
    m_cursorSpeed = 40.0f;
    m_pFnt = NULL;
    m_pSelectCtrl = NULL;
    m_pIconCtrl = NULL;
    m_pLineSize = NULL;
    m_pAlphaTbl = NULL;
    m_pAlphaTblF = NULL;
    m_pIconPos = NULL;
    m_step = 0;
    m_dispEnable = FALSE;
    m_selectOffset = 0;
    m_lines = 0;
    m_dispLineMax = 0;
    m_chars = 0;
    m_dispCharMax = 0;
    *(INT*)&m_fDispCharRate = 0;
    m_fTextFadeOutTime = m_cursorPos;
    m_fCharFadeInDelay = m_cursorPos;
    m_fAlphaTimer = m_cursorPos;
    m_nAlphaIdx = 0;
    m_nAlphaMax = 0;
    m_fgTextFadeOut = FALSE;
}
CText::~CText() {
    if (m_pFnt) delete m_pFnt;
    m_pFnt = NULL;
    if (m_pSelectCtrl) delete m_pSelectCtrl;
    m_pSelectCtrl = NULL;
    if (m_pIconCtrl) delete[] m_pIconCtrl;
    m_pIconCtrl = NULL;
    if (m_pAlphaTbl) delete m_pAlphaTbl;
    m_pAlphaTbl = NULL;
    if (m_pAlphaTblF) delete m_pAlphaTblF;
    m_pAlphaTblF = NULL;
    if (m_pLineSize) delete m_pLineSize;
    m_pLineSize = NULL;
    if (m_pIconPos) delete m_pIconPos;
    m_pIconPos = NULL;
}

void CText::init(CPointI& pos, CSizeI& size, const CHAR* pText, INT delay, UINT fontColor, UINT fontSize,
                 UINT fontStyle, CPointI fontOffset, CSizeI fontSpace, UINT* pCtdID) {
    setPosition(pos + fontOffset);
    setSize(size);
    if (!pText) return;
    if (m_pFnt) delete m_pFnt;
    m_pFnt = NULL;
#line 1412
    m_pFnt = new (__FILE__ "(1412)") CStrPrim;
    m_pFnt->setLayerFG(m_nLayerFG);
    m_pFnt->enableWordReplace();
    m_pFnt->m_C0.col = fontColor;
    if (pCtdID) setWords(pCtdID[0], pCtdID[1], pCtdID[2], pCtdID[3]);
    m_pFnt->setText(pText);
    m_pFnt->setStyle(fontStyle);
    m_pFnt->m_Size = fontSize << 4;
    m_pFnt->setSpace(fontSpace.m_w << 4, fontSpace.m_h << 4);
    m_lines = m_pFnt->getSize(m_textSize, NULL, NULL);
#line 1425
    m_pLineSize = (G_XY*)com_malloc_d(m_lines * sizeof(G_XY), __FILE__ "(1425)", RET_ADDR());
    m_pFnt->getSize(m_textSize, m_pLineSize, &m_chars);
    if ((fontStyle & 7) >= 5) m_pFnt->setLineXYPtr(m_pLineSize);
    m_pFnt->getCharSize(m_charSize);
    initDispLineMax(delay);
    m_dispCharMax = 1;
    setIcon();
}

void CText::changeText(const CHAR* pText, UINT* pCtdID) {
    if (m_pFnt) {
        if (pCtdID) setWords(pCtdID[0], pCtdID[1], pCtdID[2], pCtdID[3]);
        changeTextSub(pText);
    }
}

void CText::changeText(const CHAR* pText, const CHAR** ppWords) {
    if (m_pFnt) {
        if (ppWords) setWords(ppWords[0], ppWords[1], ppWords[2], ppWords[3]);
        changeTextSub(pText);
    }
}

void CText::changeTextSub(const CHAR* pText) {
    m_pFnt->setText(pText);
    m_lines = m_pFnt->getSize(m_textSize, NULL, NULL);
    if (m_pLineSize) delete m_pLineSize;
    m_pLineSize = NULL;
#line 1482
    m_pLineSize = (G_XY*)com_malloc_d(m_lines * sizeof(G_XY), __FILE__ "(1482)", RET_ADDR());
    m_pFnt->getSize(m_textSize, m_pLineSize, &m_chars);
    if ((m_pFnt->m_Style & 7) >= 5) {
        m_pFnt->setLineXYPtr(m_pLineSize);
    } else {
        m_pFnt->setLineXYPtr(NULL);
    }
    m_pFnt->getCharSize(m_charSize);
    initDispLineMax(m_textDispDelay);
    m_dispCharMax = 1;
    setCharFadeInDelay(m_fCharFadeInDelay);
    setIcon();
}

void CText::setExColor(UINT fontExColor) {
    if (m_pFnt) m_pFnt->m_exC0.col = fontExColor;
}

void CText::setFontStyle(UINT fontStyle) {
    if (m_pFnt) m_pFnt->setStyle(fontStyle);
}

void CText::setFontSpace(INT x, INT y) {
    if (m_pFnt) {
        m_pFnt->setSpace(x, y);
        if (m_pFnt->getText()) changeTextSub(m_pFnt->getText());
    }
}

void CText::changeBrightness(G_RGBA& inColor) {
    if (m_pFnt) m_pFnt->setBrightness(inColor);
}

void CText::setSelectOffset(UINT selectOffset) { m_selectOffset = selectOffset << 4; }

void CText::initDispLineMax(INT delay) {
    m_textDispDelay = delay;
    if (delay) {
        m_dispLineMax = 1;
        if (m_pFnt) m_pFnt->setDispLines(1);
    } else {
        m_dispLineMax = 0xff;
    }
    m_fCnt = 0.0f;
}

void CText::setDispInterval(FLOAT dispInterval) {
    if (0.0f < dispInterval) {
        m_fDispCharRate = dispInterval;
        m_dispCharMax = 1;
        m_lines = 1;
        m_fCnt = dispInterval;
        m_dispLineMax = 0;
        if (m_textDispDelay) {
            m_textDispDelay = 0;
            if (m_pFnt) m_pFnt->setDispLines(0xff);
        }
    }
}

void CText::setCharFadeInDelay(FLOAT fCharFadeInDelay) {
    if (m_pFnt && 0.0f < fCharFadeInDelay) {
        m_fCharFadeInDelay = fCharFadeInDelay;
        INT len = m_pFnt->getLength();
        if (len) {
            if (m_pAlphaTbl) delete m_pAlphaTbl;
            m_pAlphaTbl = NULL;
            if (m_pAlphaTblF) delete m_pAlphaTblF;
            m_pAlphaTblF = NULL;
#line 1574
            m_pAlphaTbl = (BYTE*)com_malloc_d(len, __FILE__ "(1574)", RET_ADDR());
            m_pAlphaTblF = (FLOAT*)com_malloc_d(len * sizeof(FLOAT), __FILE__ "(1575)", RET_ADDR());
            for (INT i = 0; i < len; i++) {
                m_pAlphaTbl[i] = 0;
                m_pAlphaTblF[i] = 0.0f;
            }
            m_nAlphaMax = len;
            m_pFnt->setAlphaTbl(m_pAlphaTbl);
        }
    }
}

void CText::setTextFadeOutTime(FLOAT fTextFadeOutTime) {
    if (m_pFnt && 0.0f < fTextFadeOutTime) m_fTextFadeOutTime = fTextFadeOutTime;
}

BOOL CText::initTextFadeOut() {
    if (0.0f < m_fTextFadeOutTime) {
        m_fgTextFadeOut = TRUE;
        m_fAlphaTimer = m_fTextFadeOutTime;
        m_pFnt->setAlphaTbl(NULL);
        return TRUE;
    }
    return FALSE;
}

BOOL CText::isEndTextFadeOut() { return m_fAlphaTimer == 0.0f; }
__asm__(".section .sdata\n");
struct CTextPH : CText { CTextPH();
    void setIcon_c() __attribute__((section(".sdata")));
};
#line 1601
void CTextPH::setIcon_c() {
    unsigned char seqNoTbl[64] = {
        0x0a, 0x0b, 0x0c, 0x0d, 0x0e, 0x12, 0x0f, 0x11, 0x10, 0x15, 0x16, 0x1b, 0x17, 0x18, 0x1c, 0x1d,
        0x19, 0x1a, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x13, 0x14, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
        'a', 'b', 'c', 'd', 'e', 'f', 'g', 'h', 'i', 'j', 'k', 'l', 'm', 'n', 'o', 'p',
        'q', 'r', 's', 't', 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    };
    unsigned char iconCode[64];
    m_nIcon = 0;
    if (m_pIconPos) delete m_pIconPos;
    m_pIconPos = NULL;
    if (m_pIconCtrl) delete[] m_pIconCtrl;
    m_pIconCtrl = NULL;
    CHAR* p = (CHAR*)m_pFnt->getText();
    while (*(BYTE*)p) {
        if (*p >= 0) {
            p++;
        } else if ((BYTE)*p < 0xf9) {
            p += 2;
        } else {
            BYTE c = p[1];
            if ((BYTE)(c + 0x80) < 0x40) {
                if (m_nIcon < 64) iconCode[m_nIcon++] = c;
            }
            p += 2;
        }
    }
    if (m_nIcon) {
        m_pIconPos = new G_XY[m_nIcon];
        m_pIconCtrl = new D2::C2DSeqCtrl[m_nIcon];
        for (INT i = 0; i < m_nIcon; i++) {
            m_pIconPos[i].set(0x80000000, 0x80000000);
            m_pIconCtrl[i].setLayer(m_nLayerFG + 1);
            m_pIconCtrl[i].addSeq("message.sqd", seqNoTbl[iconCode[i] - 0x80]);
            m_pIconCtrl[i].enableDisp(FALSE);
        }
        if (m_pFnt) m_pFnt->setIconXYPtr(m_pIconPos);
    }
}
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/System/Wnd", setIcon__5CText);
#line 1640
void CText::_main(FLOAT frameRateF) {
    if (!m_dispEnable) return;
    if (!m_pFnt) return;
    if (m_fgTextFadeOut) {
        m_fAlphaTimer -= frameRateF;
        if (0.0f < m_fAlphaTimer) {
            m_pFnt->setBrightness(0x80, 0x80, 0x80, (INT)(m_fAlphaTimer / m_fTextFadeOutTime * 128.0f));
        } else {
            m_fAlphaTimer = 0.0f;
            disableDisp();
        }
        return;
    }
    CPointI pos0 = getPosition();
    const CSizeI& sz = getSize();
    CPointI size;
    size.m_x = sz.m_w;
    size.m_y = sz.m_h;
    G_XY xy0;
    xy0.x = (pos0.m_x - 256) << 4;
    xy0.y = (-SCREEN_HEIGHT / 2 + pos0.m_y) << 4;
    G_XY xy = xy0;
    INT styleRC = m_pFnt->m_Style & 3;
    BOOL bBottom = FALSE;
    switch (styleRC) {
    case 3:
        bBottom = TRUE;
        xy.y += (size.m_y << 4) - m_textSize.y;
    case 2:
        xy.x += (size.m_x >> 1) << 4;
        break;
    case 1:
        xy.x += size.m_x << 4;
        break;
    }
    if ((m_pFnt->m_Style & 8) && !bBottom) {
        xy.y += ((size.m_y << 4) >> 1) - (m_textSize.y >> 1) + m_charSize.y - (m_pFnt->m_Space.y >> 1);
    } else {
        xy.y += m_charSize.y - m_pFnt->m_Space.y;
    }
    m_pFnt->m_P0.x = xy.x & 0xfffffff0;
    m_pFnt->m_P0.y = xy.y & 0xfffffff0;
    if (styleRC & 2) m_pFnt->m_P0.x += 0x10 - ((m_textSize.x >> 1) & 0xf);
    if (m_fDispCharRate == 0.0f) {
        if (m_dispLineMax < m_lines) {
            m_fCnt += frameRateF;
            if ((FLOAT)m_textDispDelay < m_fCnt) {
                m_fCnt = 0.0f;
                m_dispLineMax++;
                m_pFnt->setDispLines(m_dispLineMax);
            }
        } else if (m_pFnt->getDispCharsCtrl() > 0) {
            m_fDispCharRate = m_pFnt->getDispCharsCtrl() * 2;
            m_dispCharMax = m_pFnt->getDispChars();
            m_fCnt = m_dispCharMax * m_fDispCharRate;
        }
    } else if (m_dispCharMax < m_chars) {
        m_fCnt += frameRateF;
        m_dispCharMax = (INT)(m_fCnt / m_fDispCharRate);
        m_pFnt->setDispChars(m_dispCharMax);
        if (m_pFnt->getDispCharsCtrl() < 0) {
            m_fDispCharRate = 0.0f;
            m_pFnt->setDispChars(m_chars);
        }
    } else {
        m_dispLineMax = m_lines;
    }
    if (m_nAlphaMax) {
        m_fAlphaTimer += frameRateF;
        if (m_fCharFadeInDelay <= m_fAlphaTimer) {
            INT idx = m_nAlphaIdx;
            m_fAlphaTimer = 0.0f;
            m_nAlphaIdx = idx + 1;
            if (m_nAlphaMax < idx + 1 || m_pFnt->getCurrentDispChars() < idx + 1) m_nAlphaIdx = idx;
        }
        for (INT i = 0; i < m_nAlphaIdx; i++) {
            FLOAT& alphaF = m_pAlphaTblF[i];
            alphaF += 12.8f;
            if (128.0f < alphaF) {
                alphaF = 128.0f;
            } else if (alphaF < 0.0f) {
                alphaF = 0.0f;
            }
            m_pAlphaTbl[i] = (INT)alphaF;
        }
    }
    if (m_nIcon && m_pIconPos) {
        for (INT i = 0; i < m_nIcon; i++) {
            if (m_pIconPos[i].x != (INT)0x80000000) {
                FVECTOR vPos;
                vPos.set(m_pIconPos[i].x >> 4, (m_pIconPos[i].y >> 4) - 2.0f, 0.0f, 1.0f);
                m_pIconCtrl[i].setLocalPos(vPos);
                m_pIconCtrl[i].enableDisp(TRUE);
            }
        }
    }
    if (m_nSelect < 0) return;
    if (!isDispLineMax()) return;
    if (!m_pSelectCtrl) {
#line 1818
        m_pSelectCtrl = new (__FILE__ "(1818)") D2::C2DSeqCtrl;
        m_pSelectCtrl->setLayer(m_nLayerFG + 1);
        m_pSelectCtrl->addSeq("message.sqd", 1);
    }
    INT selY0 = m_charSize.y * m_nSelect + m_charSize.y - (m_pFnt->m_Size >> 1);
    if (m_pFnt->m_Style & 2) xy.x -= m_textSize.x >> 1;
    if (m_pFnt->m_Style & 1) xy.x -= m_textSize.x;
    FVECTOR vPos;
    vPos.set((xy.x + m_pLineSize[m_nSelect].x + m_selectOffset) * 0.0625f, (xy0.y + selY0) * 0.0625f, 0.0f, 1.0f);
    m_pSelectCtrl->setLocalPos(vPos);
}
void CText::disableDisp() {
    m_dispEnable = FALSE;
    for (INT i = 0; i < m_nIcon; i++) m_pIconCtrl[i].enableDisp(FALSE);
}

void CText::_disp() {
    if (m_dispEnable && m_pFnt) m_pFnt->disp();
}

void CText::_disp2() {}

BOOL CText::isDispLineMax() { return m_dispLineMax >= m_lines && m_nAlphaIdx >= m_nAlphaMax; }

void CText::getPosLF(G_XY& xy) {
    CPointI pos0 = getPosition();
    G_XY xy0;
    xy0.x = (pos0.m_x - 256) << 4;
    xy0.y = (-SCREEN_HEIGHT / 2 + pos0.m_y) << 4;
    xy.x = xy0.x + m_textSize.x;
    xy.y = xy0.y + m_textSize.y;
}

void CText::changeSelect(INT sel, INT fgDispUnderLine) {
    if (!isDispLineMax()) return;
    if (m_nSelect < 0 || m_nSelect == sel) {
        m_nSelect = sel;
    } else {
        if (sel >= 0) CSound::playSE(1, -1, 0x40);
        m_nSelect = sel;
    }
    m_cursorPos = 0.0f;
    m_cursorSpeed = 40.0f;
}

INT CText::getSelect() { return m_nSelect; }

void CText::setSelectOut() {
    if (m_pSelectCtrl) m_pSelectCtrl->replaceSeq(0, "message.sqd", 0);
}

void CText::changeCursorColor(UINT cursorColor) {}

void CText::setWords(UINT ctdID0, UINT ctdID1, UINT ctdID2, UINT ctdID3) {
    m_pWord[0] = ((INT)ctdID0 >= 0) ? CCtdMan::getText(ctdID0) : (CHAR*)ctdID0;
    m_pWord[1] = ((INT)ctdID1 >= 0) ? CCtdMan::getText(ctdID1) : (CHAR*)ctdID1;
    m_pWord[2] = ((INT)ctdID2 >= 0) ? CCtdMan::getText(ctdID2) : (CHAR*)ctdID2;
    m_pWord[3] = ((INT)ctdID3 >= 0) ? CCtdMan::getText(ctdID3) : (CHAR*)ctdID3;
    if (m_pFnt) m_pFnt->setWordList((const CHAR**)m_pWord);
}

void CText::setWords(const CHAR* pWord0, const CHAR* pWord1, const CHAR* pWord2, const CHAR* pWord3) {
    m_pWord[0] = (CHAR*)pWord0;
    m_pWord[1] = (CHAR*)pWord1;
    m_pWord[2] = (CHAR*)pWord2;
    m_pWord[3] = (CHAR*)pWord3;
    if (m_pFnt) m_pFnt->setWordList((const CHAR**)m_pWord);
}
CImageWnd::CImageWnd(CTask& inParent) : CBaseWnd(inParent) {
    m_pPolyGT4 = NULL;
    m_fgDestroy = FALSE;
}

CImageWnd::~CImageWnd() {
    if (m_pPolyGT4) delete m_pPolyGT4;
    m_pPolyGT4 = NULL;
}
void CImageWnd::createWindow(CPointI& pos0, CSizeI& size0, CPointI& pos1, CSizeI& size1, const CHAR* pTexName,
                             FLOAT fRate, G_RGBA color0, G_RGBA color1, INT fgFlip) {
    init(pos0, size0, pos1, size1, pTexName, fRate, color0, color1, fgFlip);
}

__asm__(".section .sdata\n");
struct CImageWndPH : CImageWnd { CImageWndPH();
    void init_c(CPointI& pos0, CSizeI& size0, CPointI& pos1, CSizeI& size1, const CHAR* pTexName, FLOAT fRate,
                G_RGBA color0, G_RGBA color1, INT fgFlip) __attribute__((section(".sdata")));
};
#line 1900
void CImageWndPH::init_c(CPointI& pos0, CSizeI& size0, CPointI& pos1, CSizeI& size1, const CHAR* pTexName, FLOAT fRate,
                     G_RGBA color0, G_RGBA color1, INT fgFlip) {
    if (!m_pPolyGT4) {
#line 2015
        CPolyGT4* pPoly = new (__FILE__ "(2015)") CPolyGT4;
        pPoly->setLayerFG(m_nLayerFG);
        pPoly->setTPage(pTexName);
        TexInfo* pTexInfo = pPoly->getTexInfo();
        if (!pTexInfo) {
            delete pPoly;
            return;
        }
        USHORT w = pTexInfo->_TexInfo._nDistW - 1;
        USHORT h = pTexInfo->_TexInfo._nDistH - 1;
        USHORT h2 = 0;
        USHORT w2 = 0;
        if (fgFlip & 1) {
            w2 = w;
            w = 0;
        }
        if (fgFlip & 2) {
            h2 = h;
            h = 0;
        }
        pPoly->m_T0.set(w, h2);
        pPoly->m_T1.set(w, h);
        pPoly->m_T2.set(w2, h2);
        pPoly->m_T3.set(w2, h);
        INT& layerFG = m_nLayerFG;
        pPoly->setLayerFG(layerFG);
        pPoly->setSemiTrans((ALPHA)1);
        m_pPolyGT4 = pPoly;
    }
    TexInfo* pTexInfo = m_pPolyGT4->getTexInfo();
    if (!pTexInfo) return;
    USHORT tw = pTexInfo->_TexInfo._nDistW;
    USHORT th = pTexInfo->_TexInfo._nDistH;
    if (pos0.m_x != (INT)0x80000000) {
        m_initPos.x = pos0.m_x - 256;
        m_initPos.y = pos0.m_y - _screenHeight() / 2;
    } else {
        m_initPos = m_currentPos;
    }
    if (size0.m_w >= 0) {
        m_initPos.z = size0.m_w;
        m_initPos.w = size0.m_h;
    } else {
        m_initPos.z = th;
        m_initPos.w = tw;
    }
    if (pos1.m_x != (INT)0x80000000) {
        m_targetPos.x = pos1.m_x - 256;
        m_targetPos.y = pos1.m_y - _screenHeight() / 2;
    } else {
        m_targetPos = m_currentPos;
    }
    if (size1.m_w >= 0) {
        m_targetPos.z = size1.m_w;
        m_targetPos.w = size1.m_h;
    } else {
        m_targetPos.z = th;
        m_targetPos.w = tw;
    }
    m_step = 1;
    m_currentPos = m_initPos;
    m_currentColor = color0;
    m_initColor = color0;
    m_targetColor = color1;
    m_fCurrentRate = 0.0f;
    m_fRate = 1.0f / fRate;
}
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/System/Wnd", init__9CImageWndRt7CPointT1ZiRt6CSizeT1ZiT1T2PCcfG6G_RGBAT7i);
#line 2082
void CImageWnd::mainImpl() {
    if (!m_pPolyGT4) return;
    switch (m_step) {
    case 0:
    case 1:
        m_step = 2;
    case 2:
    case 3: {
        FVECTOR fvec0;
        IVECTOR ivec;
        FVECTOR fvec1;
        m_frameRateF = getFrameRateF() * m_fRate;
        m_fCurrentRate += m_frameRateF * 0.016666668f;
        if (1.0f <= m_fCurrentRate) {
            m_fCurrentRate = 1.0f;
            m_step = 5;
            m_phaseCnt++;
        }
        sceVu0InterVector(m_currentPos, m_targetPos, m_initPos, m_fCurrentRate);
        fvec0 = m_currentPos;
        ScaleVector(fvec0, fvec0, 16.0f);
        fvec0.x -= fvec0.z * 0.5f;
        fvec0.y -= fvec0.w * 0.5f;
        fvec0.z += fvec0.x;
        fvec0.w += fvec0.y;
        FTOI0Vector(ivec, fvec0);
        _setXYZ(m_pPolyGT4->m_P0, ivec.x, ivec.y);
        _setXYZ(m_pPolyGT4->m_P1, ivec.z, ivec.y);
        _setXYZ(m_pPolyGT4->m_P2, ivec.x, ivec.w);
        _setXYZ(m_pPolyGT4->m_P3, ivec.z, ivec.w);
        fvec0.set(m_initColor.a, m_initColor.b, m_initColor.g, m_initColor.r);
        fvec1.set(m_targetColor.a, m_targetColor.b, m_targetColor.g, m_targetColor.r);
        sceVu0InterVector(fvec0, fvec1, fvec0, m_fCurrentRate);
        FTOI0Vector(ivec, fvec0);
        m_currentColor.a = ivec.x;
        m_currentColor.b = ivec.y;
        m_currentColor.g = ivec.z;
        m_currentColor.r = ivec.w;
        m_pPolyGT4->m_C0 = m_pPolyGT4->m_C1 = m_pPolyGT4->m_C2 = m_pPolyGT4->m_C3 = m_currentColor;
        break;
    }
    case 5:
        if (m_fgDestroy) m_step = 9;
        break;
    case 4:
    case 6:
    case 8:
        break;
    default:
        if (m_fgDestroy) out();
        break;
    }
}
void CImageWnd::dispImpl() {
    if (m_pPolyGT4) m_pPolyGT4->disp();
}
// SOURCE DATA: .data .rodata
