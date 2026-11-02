#ifndef SYSTEM_WND_H
#define SYSTEM_WND_H

#include "khtypes.h"
#include "System/Component.h"
#include "System/Vector.h"
#include "System/Graphic/Gprim.h"
#include "System/Graphic/Effect/ObjType.h"

namespace D2 {
class C2DSeqCtrl;
}
class CStrPrim;
class CPolyGT4;
class CWnd;
class CText;

/* The inline members are emitted (weak) in System/Wnd.cpp, in this order: keep it. */
class CBaseWnd : public CComponentObj {
protected:
    FLOAT m_frameRateF;  /* 0x80 */
    TIMEQ m_frameRate;   /* 0x84 */
    INT m_step;          /* 0x88 */
    INT m_phaseCnt;      /* 0x8c */
    static INT m_nLayerFG;
    ObjType::eObjType m_nObjType; /* 0x90 */

public:
    CBaseWnd(CTask& inParent);
    virtual ~CBaseWnd();
    INT isEnd() { return m_step >= 9; }
    INT getStatus() { return m_step; }
    INT getPhaseCnt() { return m_phaseCnt; }
    void ignorePause(BOOL bInFlag) {
        if (bInFlag) {
            m_nInfo |= TASK_INFO_IGNOREPAUSE;
        } else {
            m_nInfo &= ~TASK_INFO_IGNOREPAUSE;
        }
    }
    void ignorePlaySpeed(BOOL bInFlag) {
        if (bInFlag) {
            m_nInfo |= TASK_INFO_IGNOREPLAYSPEED;
        } else {
            m_nInfo &= ~TASK_INFO_IGNOREPLAYSPEED;
        }
    }
    void setObjType(ObjType::eObjType objType) { m_nObjType = objType; }
    static void setLayerFG(INT layerFG) { m_nLayerFG = layerFG; }
};

class CTextWnd : public CBaseWnd {
protected:
    char m_voiceName[16];       /* 0x0a0 */
    CPointI m_targetPos;        /* 0x0b0 */
    CPointI m_textOffset;       /* 0x0b8 */
    CSizeI m_targetSize;        /* 0x0c0 */
    CSizeI m_fontSpace;         /* 0x0c8 */
    CWnd* m_pWnd;               /* 0x0d0 */
    CText* m_pText;             /* 0x0d4 */
    INT m_cnt;                  /* 0x0d8 */
    FLOAT m_fCnt;               /* 0x0dc */
    UINT m_textColor;           /* 0x0e0 */
    UINT m_textExColor;         /* 0x0e4 */
    UINT m_fontSize;            /* 0x0e8 */
    UINT m_fontStyle;           /* 0x0ec */
    UINT m_wndStyle;            /* 0x0f0 */
    INT m_textDispDelay;        /* 0x0f4 */
    INT m_status;               /* 0x0f8 */
    INT m_wndArrangement;       /* 0x0fc */
    FLOAT m_fDispWait;          /* 0x100 */
    INT m_fgDestroy;            /* 0x104 */
    UINT m_textSelectOffset;    /* 0x108 */
    FLOAT m_fTextDispInterval;  /* 0x10c */
    UINT m_ctdID;               /* 0x110 */
    BOOL m_fgVoice;             /* 0x114 */
    INT m_nMode;                /* 0x118 */
    UINT m_nVoiceHandle;        /* 0x11c */
    FLOAT m_fOpenDelay;         /* 0x120 */
    FLOAT m_fVoiceTime;         /* 0x124 */
    INT m_fgVoiceTimeInclude;   /* 0x128 */
    CHAR* m_pTmpText;           /* 0x12c */
    TIMEQ m_qCnt;               /* 0x130 */
    BOOL m_bEnableLFDisp;       /* 0x134 */
    FLOAT m_fOpenSpeed;         /* 0x138 */
    FLOAT m_fCloseSpeed;        /* 0x13c */

public:
    CTextWnd(CTask& inParent, BOOL fgVoice);
    virtual ~CTextWnd();
    void init(CPointI& pos, CSizeI& size, UINT wndColor, const CHAR* pText, UINT textColor, UINT fontSize,
              UINT wndArrangement, CPointI& cyobiPos, UINT waitFrame, UINT fontStyle, UINT* pCtdID, UINT cyobiStyle,
              INT cyobiPosX);
    void createWindow(CPointI& pos, CSizeI& size, UINT wndColor, const CHAR* pText, UINT textColor, UINT fontSize,
                      UINT wndArrangement, CPointI& cyobiPos, UINT waitFrame, UINT fontStyle);
    void createWindowC(CPointI& pos, CSizeI& size, UINT wndColor, const CHAR* pText, UINT textColor, UINT fontSize,
                       UINT wndArrangement, CPointI& cyobiPos, UINT waitFrame, UINT fontStyle);
    void createWindowCTD(UINT ctdID, CPointI* pPos, CPointI* pCyobiPos, UINT waitFrame, UINT* pCtdID, UINT nLine);
    void createWindowCTDEz(UINT ctdID, const CHAR** ppWords, UINT waitFrame, CPointI* pPos, CPointI* pCyobiPos);
    void initWindow(CPointI& pos, CSizeI& size, UINT wndColor, CPointI& cyobiPos, UINT cyobiStyle, INT cyobiPosX,
                    INT fgImage);
    void setText(const CHAR* pText, UINT* pCtdID);
    void open();
    void close();
    void closeDestroy();
    void closeFadeOutDestroy(UINT fadeOutTime);
    void setTextColor(UINT textColor);
    void setTextExColor(UINT textExColor);
    void setTextDispDelay(INT textDispDelay);
    void setTextOffset(CPointI textOffset);
    void setFontSize(UINT fontSize);
    void setFontStyle(UINT fontStyle);
    void setFontSpace(CSizeI fontSpace);
    void setWndStyle(UINT wndStyle);
    void setWndRate(FVECTOR& wndRate);
    void changeWndStyle(UINT wndStyle);
    void clearText();
    void changeTextSelect(INT sel, INT fgDispUnderLine);
    void changeText(const CHAR* pText, const CHAR** ppWords);
    void changeText(const CHAR* pText, UINT* pCtdID);
    void setSelectOffset(UINT selectOffset);
    void changeCursorColor(UINT cursorColor);
    void setDispInterval(UINT dispInterval);
    void changeTextNext(UINT* pCtdID);
    void enableWndDisp(UINT fgDisp);
    void enableDisp(UINT fgDisp);
    void setWords(const CHAR** ppWord);
    void setWords(UINT* pCtdID);
    void setTextCTD(UINT ctdID, UINT* pCtdID);
    void changeTextCTD(UINT ctdID, UINT* pCtdID);
    void setPos(CPointI& inPos) { setPosition(inPos); }
    void changeTextBrightness(G_RGBA& inColor);
    void setOpenDelay(UINT nOpenDelay) { m_fOpenDelay = nOpenDelay; }
    void setCharFadeInDelay(FLOAT fCharFadeInDelay);
    void setTextFadeOutTime(FLOAT fTextFadeOutTime);
    void enableLFDisp(BOOL bEnable);
    void setOpenSpeed(FLOAT fOpenSpeed);
    void setCloseSpeed(FLOAT fCloseSpeed);

protected:
    virtual void mainImpl();
    virtual void dispImpl();

private:
    void setVoiceName(const CHAR* pText);
};

class CWnd : public CComponent {
private:
    CPointI m_fukiPosShift;   /* 0x14 */
    CPointI m_fukiPosCenter;  /* 0x1c */
    CPointI m_cyobiPos;       /* 0x24 */
    INT m_dispEnable;         /* 0x2c */
    INT m_dispEnable2;        /* 0x30 */
    INT m_wndStyle;           /* 0x34 */
    FLOAT m_fWndRateX;        /* 0x38 */
    FLOAT m_fWndRateY;        /* 0x3c */
    INT m_nLayerFG;           /* 0x40 */
    USHORT m_wndSeqNo;        /* 0x44 */
    USHORT m_cyobiSeqNo;      /* 0x46 */
    FVECTOR m_vCyobiLocal;    /* 0x50 */
    FVECTOR m_vLFLocal;       /* 0x60 */
    INT m_nStepFukidasi;      /* 0x70 */
    INT m_cyobiStyle;         /* 0x74 */
    INT m_cyobiPosX;          /* 0x78 */
    D2::C2DSeqCtrl* m_pFukiCtrl;  /* 0x7c */

public:
    CWnd(INT layerFG);
    virtual ~CWnd();
    void init(CPointI& pos, CSizeI& size, UINT wndColor, UINT wndStyle, CPointI& cyobiPos, UINT cyobiStyle,
              INT cyobiPosX);
    void _main(FLOAT frameRateF);
    void _disp();
    void enableDisp() { m_dispEnable = TRUE; }
    void disableDisp() { m_dispEnable = FALSE; }
    void enableDisp2(INT fg) { m_dispEnable2 = fg; }
    INT isDispEnable() { return m_dispEnable; }
    INT isDispEnable2() { return m_dispEnable2; }
    void changeFukiStyle(UINT wndStyle);
    void setFukiStyle(UINT wndStyle);
    void setWndRate(FVECTOR& wndRate);
    void changeColor(UINT wndColor, UINT lineColor);
    INT setFukidasi(INT iCmd, TIMEQ timeq);
    CPointI& getCyobiPos() { return m_cyobiPos; }
};

class CText : public CComponent {

private:
    D2::C2DSeqCtrl* m_pSelectCtrl;  /* 0x14 */
    D2::C2DSeqCtrl* m_pIconCtrl;    /* 0x18 */
    G_XY m_textSize;            /* 0x1c */
    G_XY m_charSize;            /* 0x24 */
    CStrPrim* m_pFnt;           /* 0x2c */
    G_XY* m_pLineSize;          /* 0x30 */
    G_XY* m_pIconPos;           /* 0x34 */
    INT m_step;                 /* 0x38 */
    INT m_lines;                /* 0x3c */
    INT m_dispLineMax;          /* 0x40 */
    INT m_dispEnable;           /* 0x44 */
    INT m_textDispDelay;        /* 0x48 */
    FLOAT m_fCnt;               /* 0x4c */
    INT m_nSelect;              /* 0x50 */
    INT m_selectOffset;         /* 0x54 */
    FLOAT m_cursorPos;          /* 0x58 */
    FLOAT m_cursorSpeed;        /* 0x5c */
    INT m_chars;                /* 0x60 */
    INT m_dispCharMax;          /* 0x64 */
    FLOAT m_fDispCharRate;      /* 0x68 */
    INT m_nLayerFG;             /* 0x6c */
    char* m_pWord[4];           /* 0x70 */
    BYTE* m_pAlphaTbl;          /* 0x80 */
    FLOAT* m_pAlphaTblF;        /* 0x84 */
    INT m_nAlphaMax;            /* 0x88 */
    INT m_nAlphaIdx;            /* 0x8c */
    FLOAT m_fAlphaTimer;        /* 0x90 */
    FLOAT m_fCharFadeInDelay;   /* 0x94 */
    FLOAT m_fTextFadeOutTime;   /* 0x98 */
    BOOL m_fgTextFadeOut;       /* 0x9c */
    INT m_nIcon;                /* 0xa0 */

public:
    CText(INT layerFG);
    virtual ~CText();
    void init(CPointI& pos, CSizeI& size, const CHAR* pText, INT delay, UINT fontColor, UINT fontSize,
              UINT fontStyle, CPointI fontOffset, CSizeI fontSpace, UINT* pCtdID);
    void _main(FLOAT frameRateF);
    void _disp();
    void _disp2();
    void enableDisp() { m_dispEnable = TRUE; }
    void disableDisp();
    void initDispLineMax(INT delay);
    BOOL isDispLineMax();
    void getPosLF(G_XY& xy);
    INT getSelect();
    void changeSelect(INT sel, INT fgDispUnderLine);
    void setExColor(UINT fontExColor);
    void setFontStyle(UINT fontStyle);
    void setFontSpace(INT x, INT y);
    void changeText(const CHAR* pText, UINT* pCtdID);
    void changeText(const CHAR* pText, const CHAR** ppWords);
    void setSelectOffset(UINT selectOffset);
    void changeCursorColor(UINT cursorColor);
    void setDispInterval(FLOAT dispInterval);
    void setWords(UINT ctdID0, UINT ctdID1, UINT ctdID2, UINT ctdID3);
    void setWords(const CHAR* pWord0, const CHAR* pWord1, const CHAR* pWord2, const CHAR* pWord3);
    void changeBrightness(G_RGBA& inColor);
    void setCharFadeInDelay(FLOAT fCharFadeInDelay);
    void setTextFadeOutTime(FLOAT fTextFadeOutTime);
    BOOL initTextFadeOut();
    BOOL isEndTextFadeOut();
    void setSelectOut();
    void setIcon();

private:
    void changeTextSub(const CHAR* pText);
};

class CImageWnd : public CBaseWnd {

private:
    FVECTOR m_initPos;       /* 0xa0 */
    FVECTOR m_targetPos;     /* 0xb0 */
    FVECTOR m_currentPos;    /* 0xc0 */
    G_RGBA m_initColor;      /* 0xd0 */
    G_RGBA m_targetColor;    /* 0xd4 */
    G_RGBA m_currentColor;   /* 0xd8 */
    CPolyGT4* m_pPolyGT4;    /* 0xdc */
    FLOAT m_fRate;           /* 0xe0 */
    FLOAT m_fCurrentRate;    /* 0xe4 */
    INT m_fgDestroy;         /* 0xe8 */

public:
    CImageWnd(CTask& inParent);
    virtual ~CImageWnd();
    void createWindow(CPointI& pos0, CSizeI& size0, CPointI& pos1, CSizeI& size1, const CHAR* pTexName, FLOAT fRate,
                      G_RGBA color0, G_RGBA color1, INT fgFlip);
    void init(CPointI& pos0, CSizeI& size0, CPointI& pos1, CSizeI& size1, const CHAR* pTexName, FLOAT fRate,
              G_RGBA color0, G_RGBA color1, INT fgFlip);
    void setDestroy() { m_fgDestroy = TRUE; }
    G_RGBA& getCurrentColor() { return m_currentColor; }

protected:
    virtual void mainImpl();
    virtual void dispImpl();
};

#endif /* SYSTEM_WND_H */
